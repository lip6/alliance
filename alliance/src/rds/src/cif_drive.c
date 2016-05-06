/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSCIF                         |
|                                                             |
| File    :                 cif_drive.c                       |
|                                                             |
| Authors :       Petrot Frederic & Jacomme Ludovic           |
|                   ---------          ---------              |
| Date    :    The king ^     04/07/93     ^ The slave        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdlib.h>
# include <stdio.h>
# include <time.h>

# include <mut.h>
# include <mph.h>
# include <rds.h>
# include <rpr.h>
# include <rfm.h>
# include <rwi.h>
# include <rut.h>
# include <rtl.h>

# include "cif.h"
# include "cif_error.h"
# include "cif_drive.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

 static  char  CifBuffer[ 256 ];

 static  long  CIF_DS_A = 0;
 static  long  CIF_DS_B = 0;

 static  ptype_list *HEAD_CIF_MODEL = (ptype_list *)NULL;
 static  long        CifModelIndex  = 0;

 static  int RDS_CIF_BERKLEY_DRIVER = 0;


/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          rdstocifname                       |
|                                                             |
\------------------------------------------------------------*/

  char *rdstocifname( Name )

   char *Name;
 {
   char *Source;
   char *Target;
   char  One;

   if ( Name == (char *)NULL ) 
   {
     return( (char *)NULL );
   }

   One = 1;

   Source = Name;
   Target = CifBuffer;

   while (  *Source != '\0' ) 
   {
     if ( *Source == ' ' )
     {
       if ( One ) 
       {
         *Target++ = '[';
         Source++;

         One = 0;
       } 
       else 
       {
         *Target++ = ']';
         *Target++ = '[';
          Source++;
       }
     }

     if ( ( *Source == SEPAR ) && ( ! One ) )
     {
       *Target++ = ']';
       One       = 1;
     }

     *Target++ = *Source++;
   }

   if ( ! One )
   {
     *Target++ = ']';
   }

   *Target = '\0';

   return ( CifBuffer );
 }

/*------------------------------------------------------------\
|                                                             |
|                       r_cif_model_list                      |
|                                                             |
\------------------------------------------------------------*/

  static ptype_list *r_cif_model_list( Figure )
  
     rdsfig_list *Figure;
  {
    rdsins_list *Instance;
    rdsfig_list *Model;
    ptype_list  *List;
  
    for ( Instance  = Figure->INSTANCE;
          Instance != (rdsins_list *)NULL;
          Instance  = Instance->NEXT )
    {
      for ( List  = HEAD_CIF_MODEL;
            List != (ptype_list *)NULL;
            List  = List->NEXT )
      {
        if (((rdsfig_list *)List->DATA)->NAME == Instance->FIGNAME )
        {
          break;
        }
      }

      if ( List == (ptype_list *)NULL )
      {
        for ( Model  = HEAD_RDSFIG;
              Model != (rdsfig_list *)NULL;
              Model  = Model->NEXT )
        {
          if ( Model->NAME == Instance->FIGNAME ) break;
        }
  
        if ( Model == (rdsfig_list *)NULL )
        {
          ciferror( CIF_ERROR_MODEL,  Instance->FIGNAME, 0 );
        }

        (void)r_cif_model_list( Model );
      }
   
    }

    HEAD_CIF_MODEL = addptype( HEAD_CIF_MODEL,
                               ++CifModelIndex, (char *)Figure );
    return( HEAD_CIF_MODEL );
  }

/*------------------------------------------------------------\
|                                                             |
|                          cif_model_list                     |
|                                                             |
\------------------------------------------------------------*/

  ptype_list *cif_model_list( Figure )

    rdsfig_list *Figure;
  {
    CifModelIndex = 0;

    if ( HEAD_CIF_MODEL != (ptype_list *)NULL )
    {
      freeptype( HEAD_CIF_MODEL);
    }

    HEAD_CIF_MODEL = NULL;

    return( r_cif_model_list( Figure ) );
  }

/*------------------------------------------------------------\
|                                                             |
|                          write_figure                       |
|                                                             |
\------------------------------------------------------------*/

  void write_figure( File, Model )

    FILE *File;
    long  Model;
  {
    (void)fprintf( File, "C%ld;\n", Model );
  }

/*------------------------------------------------------------\
|                                                             |
|                          write_instance                     |
|                                                             |
\------------------------------------------------------------*/

  void write_instance( File, Name, Model, X, Y, Trans )

    FILE *File;
    char *Name;
    long  Model;
    long  X;
    long  Y;
    long  Trans;
  {
    char Buffer[ 100 ];

    long Xcif;
    long Ycif;

    Xcif = RDS_TO_CIF_UNIT( X );
    Ycif = RDS_TO_CIF_UNIT( Y );

    switch ( Trans ) 
    {
      case RDS_NOSYM :
         (void)sprintf( Buffer, "T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_SYM_X :
         (void)sprintf( Buffer, "MX T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_SYM_Y :
         (void)sprintf( Buffer, "MY T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_SYMXY :
         (void)sprintf( Buffer, "R-1, 0 T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_ROT_P :
         (void)sprintf( Buffer, "R0, 1 T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_ROT_M :
         (void)sprintf( Buffer, "R0, -1 T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_SY_RP :
         (void)sprintf( Buffer, "MX R0, -1 T%ld, %ld", Xcif, Ycif );
         break;
      case RDS_SY_RM :
         (void)sprintf( Buffer, "MX R0, 1 T%ld, %ld", Xcif, Ycif );
         break;
      default :
         ciferror( CIF_ERROR_BAD_TRANS, Name, 0 );
   }

   if ( ! RDS_CIF_BERKLEY_DRIVER )
   {
     (void)fprintf( File, "4I %s;", Name);
   }
   else
   {
     (void)fprintf( File, "91 %s;", Name);
   }

   (void)fprintf( File, "C%ld %s;\n", Model, Buffer );
}

/*------------------------------------------------------------\
|                                                             |
|                          write_connector                    |
|                                                             |
\------------------------------------------------------------*/

   void write_connector( File, Name, Index, Width, X, Y, Dx, Dy, Layer, WriteLayer )

    FILE *File;
    char *Name;
    long  Index;
    long  Width;
    long  X;
    long  Y;
    long  Dx;
    long  Dy;
    char *Layer;
    char  WriteLayer;
  {
    if ( Name == (char *)NULL )
    {
      ciferror( CIF_ERROR_CONNECTOR_NAME, (char *)X, Y );
    }

    if ( WriteLayer )
    {
      (void)fprintf( File, "L%s;\n", Layer );
    }

    Name = rdstocifname( Name );

    if ( ! RDS_CIF_BERKLEY_DRIVER )
    {
      (void)fprintf( File, "4X %s %ld %ld %ld %ld %s;\n",
                     Name, Index,
                     RDS_TO_CIF_UNIT(  X ) + RDS_TO_CIF_UNIT( Dx ) / 2,
                     RDS_TO_CIF_UNIT(  Y ) + RDS_TO_CIF_UNIT( Dy ) / 2,
                     RDS_TO_CIF_UNIT( Width ), Name );
    }
    else
    {
        (void)fprintf( File, "94 %s %ld %ld %s;\n",
                       Name,
                       RDS_TO_CIF_UNIT(  X ) + RDS_TO_CIF_UNIT( Dx ) / 2,
                       RDS_TO_CIF_UNIT(  Y ) + RDS_TO_CIF_UNIT( Dy ) / 2,
                       Layer );
    }
  }

/*------------------------------------------------------------\
|                                                             |
|                          write_rectangle                    |
|                                                             |
\------------------------------------------------------------*/

  void write_rectangle( File, Name, X, Y, Dx, Dy, Layer, WriteLayer )

     FILE *File;
     char *Name;
     long  X;
     long  Y;
     long  Dx;
     long  Dy;
     char *Layer;
     char  WriteLayer;
  {
    if ( WriteLayer )
    {
      (void)fprintf( File, "L%s;\n", Layer );
    }

    if ( (  Name != (char *)NULL ) && 
         ( *Name != '*'          ) && 
         ( *Name != '\0'         ) )
    {
      if ( ! RDS_CIF_BERKLEY_DRIVER )
      {
        (void)fprintf( File, "4N %s %ld %ld;\n",
                       rdstocifname( Name ),
                       RDS_TO_CIF_UNIT(  X ) + RDS_TO_CIF_UNIT( Dx ) / 2,
                       RDS_TO_CIF_UNIT(  Y ) + RDS_TO_CIF_UNIT( Dy ) / 2);
      }
      else
      {
      (void)fprintf( File, "95 %s %ld %ld %ld %ld %s;\n",
                     rdstocifname( Name ), 
                     RDS_TO_CIF_UNIT( Dx ), RDS_TO_CIF_UNIT( Dy ),
                     RDS_TO_CIF_UNIT(  X ) + RDS_TO_CIF_UNIT( Dx ) / 2,
                     RDS_TO_CIF_UNIT(  Y ) + RDS_TO_CIF_UNIT( Dy ) / 2,
                     Layer );
      }
    }

    (void)fprintf( File, "B%ld %ld %ld %ld;\n",
                   RDS_TO_CIF_UNIT( ABSOLUTE( Dx ) ),
                   RDS_TO_CIF_UNIT( ABSOLUTE( Dy ) ),
                   RDS_TO_CIF_UNIT(  X ) + RDS_TO_CIF_UNIT( Dx ) / 2,
                   RDS_TO_CIF_UNIT(  Y ) + RDS_TO_CIF_UNIT( Dy ) / 2);
  }

/*------------------------------------------------------------\
|                                                             |
|                          write_header                       |
|                                                             |
\------------------------------------------------------------*/

  void write_header( File, Name )

    FILE *File;
    char *Name;
  {
    long Counter;

    (void)time( &Counter );
    (void)fprintf( File, "(rds to CIF driver cvs version $Revision: 1.1 $\n");
    (void)fprintf( File, "technology %s\n", RDS_TECHNO_NAME );
    (void)fprintf( File, "%s"  , ctime( &Counter )          );
    (void)fprintf( File, "%s\n", Name                       );
    (void)fprintf( File, "%s);\n\n", mbkgetenv("USER")         );
  }

/*------------------------------------------------------------\
|                                                             |
|                          write_header                       |
|                                                             |
\------------------------------------------------------------*/

  void write_tail( File, X, Y, Dx, Dy )

    FILE *File;
    long  X;
    long  Y;
    long  Dx;
    long  Dy;
  {
    (void)fprintf( File, "(AB : %.2f, %.2f %.2f, %.2f in micron); \n",
                   (float)X / RDS_UNIT, 
                   (float)Y / RDS_UNIT,
                   (float)(X + Dx) / RDS_UNIT,
                   (float)(Y + Dy) / RDS_UNIT );
  }

/*------------------------------------------------------------\
|                                                             |
|                        write_model_header                   |
|                                                             |
\------------------------------------------------------------*/

  void write_model_header( File, ModelName, ModelNum )

    FILE *File;
    char *ModelName;
    long  ModelNum;
  {
    (void)fprintf( File, "DS%ld %ld %ld;\n", ModelNum, CIF_DS_A, CIF_DS_B * 2 );
    (void)fprintf( File, "9 %s;\n", ModelName );
  }

/*------------------------------------------------------------\
|                                                             |
|                        write_model_abox                     |
|                                                             |
\------------------------------------------------------------*/

   void write_model_abox( File, X, Y, Dx, Dy )


    FILE *File;
    long  X;
    long  Y;
    long  Dx;
    long  Dy;
  {
    (void)fprintf( File, "(AB : %.2f, %.2f %.2f, %.2f in micron);\n",
                   (float)X / RDS_UNIT,
                   (float)Y / RDS_UNIT,
                   (float)(X + Dx) / RDS_UNIT,
                   (float)(Y + Dy) / RDS_UNIT);

    if ( ! RDS_CIF_BERKLEY_DRIVER )
    {
      (void)fprintf( File, "4A %ld %ld %ld %ld; \n",
                     RDS_TO_CIF_UNIT( X ), 
                     RDS_TO_CIF_UNIT( Y ),
                     RDS_TO_CIF_UNIT( X + Dx ),
                     RDS_TO_CIF_UNIT( Y + Dy ) );
    }
  }

/*------------------------------------------------------------\
|                                                             |
|                        write_model_tail                     |
|                                                             |
\------------------------------------------------------------*/

  void write_model_tail( File )

    FILE *File;
  {
    (void)fprintf( File, "DF;\n\n" );
  }

/*------------------------------------------------------------\
|                                                             |
|                          getcifmodel                        |
|                                                             |
\------------------------------------------------------------*/

  long getcifmodel( Head, Name )

    ptype_list *Head;
    char       *Name;
  {
    ptype_list *Scan;

    for ( Scan  = Head; 
          Scan != (ptype_list *)NULL;
          Scan  = Scan->NEXT )
    {
      if (((rdsfig_list *)Scan->DATA)->NAME == Name )
      {
        return( Scan->TYPE );
      }
    }

    return( 0 );
  }

/*------------------------------------------------------------\
|                                                             |
|                      compute_cif_unit                       |
|                                                             |
\------------------------------------------------------------*/

 void compute_cif_unit()
 {
   /* why CIF_DS_A, CIF_DS_B ?
      the simple of driving CIF would be to take both as 1, and have on
      each coordinate a (x * CIF_UNIT)/rds_unit).
      The interest here is to shorten the length of the numbers in the
      CIF output, and to be able to express a precision better that a
      centh of a micron.
      CIF_DS_A and CIF_DS_B are defined as follow :
      (CIF_DS_A / CIF_DS_B) * rds_unit = CIF_UNIT
      see CIF documentation for details.
      the coodinates for CIF will be computed like
      (x * CIF_DS_B) / (CIF_DS_A * rds_unit).
      The computation algorithm is equivalent to the one given in the
      function RprComputeRdsUnit, so take a look there for details. 

      Friendly, The king ... */

   /* why 2 * CIF_UNIT ?
      because all the rectangles in the CIF file are written like
      BOXES, ie dx and dy must be even since we drive dx/2 dy/2
      and we don't want to lose any information! */
   int cif_unit = 2 * CIF_UNIT;

   if ( RDS_UNIT < cif_unit ) 
   {
     for ( CIF_DS_A = 1, CIF_DS_B = 1; 
           ( CIF_DS_A * RDS_UNIT ) != ( cif_unit * CIF_DS_B ); 
           CIF_DS_B++ ) 
     {
       for ( CIF_DS_A--; 
             ( CIF_DS_A * RDS_UNIT ) < ( cif_unit * CIF_DS_B ); 
             (CIF_DS_A)++ );

       if ( ( CIF_DS_A * RDS_UNIT ) == ( cif_unit * CIF_DS_B ) ) break;
     }
   } 
   else 
   {
     for ( CIF_DS_A = 1, CIF_DS_B = 1; 
           ( CIF_DS_A * RDS_UNIT ) != ( cif_unit * CIF_DS_B ); 
           CIF_DS_A++ ) 
     {
       for ( CIF_DS_B--; 
             ( CIF_DS_A * RDS_UNIT ) > ( cif_unit * CIF_DS_B ); 
             CIF_DS_B++ );

       if ( ( CIF_DS_A * RDS_UNIT ) == ( cif_unit * CIF_DS_B ) ) break;
     }
   }
 }

/*------------------------------------------------------------\
|                                                             |
|                         drive_model                         |
|                                                             |
\------------------------------------------------------------*/

  void drive_model( File, Model, Figure )

    FILE        *File;
    ptype_list  *Model;
    rdsfig_list *Figure;
{
    rdsrec_list *Rectangle;
    rdsins_list *Instance;
    int          Layer;
    int          WriteLayer;
    long         Index;
    char        *CifLayer;

    Index = 0;

    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) 
    {
      if ( Layer == RDS_ABOX ) continue;

      CifLayer = GET_CIF_LAYER( Layer );

      if ( CifLayer == RDS_CIF_LAYER_EMPTY ) continue;

      WriteLayer = 1;

      for ( Rectangle  = Figure->LAYERTAB[ Layer ]; 
            Rectangle != (rdsrec_list *)NULL; 
            Rectangle  = Rectangle->NEXT ) 
      {
        /* drive connector :
           put the connector name in vti cif dialect.
           if the rds doesn't come from mbk, the type may be set but the
           mbk pointer may be null. */

        if ( ( ( IsRdsConExter( Rectangle ) ) ||
               ( IsRdsRefCon  ( Rectangle ) )    ) &&
             ! IsRdsVia( Rectangle )
           )
        {
          write_connector( File, 
                           Rectangle->NAME, ++Index,
                           ( Rectangle->DX > Rectangle->DY ) ? 
                           Rectangle->DX : Rectangle->DY,
                           Rectangle->X  , Rectangle->Y, 
                           Rectangle->DX , Rectangle->DY,
                           CifLayer, WriteLayer );
        }  

        write_rectangle( File, 
                         Rectangle->NAME, 
                         Rectangle->X , Rectangle->Y, 
                         Rectangle->DX, Rectangle->DY, 
                         CifLayer, WriteLayer );
        /* layer choice :

           the layer is driven only once, since in cif when a layer is defined,
           all further objects are build with this layer. This is close of the
           rds representation, so the writting functions print a layer when the
           parameter is not NULL, nothing if it is. The king ... */

        WriteLayer = 0;
      }
    }

    for ( Instance  = Figure->INSTANCE; 
          Instance != (rdsins_list *)NULL; 
          Instance  = Instance->NEXT )
    {
      write_instance( File, Instance->INSNAME, 
                      getcifmodel( Model, Instance->FIGNAME ),
                      Instance->X, Instance->Y, 
                      (long)Instance->TRANSF ); 
    }
}

/*------------------------------------------------------------\
|                                                             |
|                         cifsaverdsfig                       |
|                                                             |
\------------------------------------------------------------*/

 void cifsaverdsfig( Figure )

    rdsfig_list *Figure;
 {
   FILE        *File;
   ptype_list  *Model;
   ptype_list  *List;
   rdsrec_list *Abox;

   if ( mbkgetenv( "RDS_CIF_BERKLEY_DRIVER" ) != (char *)0 )
   {
     RDS_CIF_BERKLEY_DRIVER = 1;
   }
   else
   {
     RDS_CIF_BERKLEY_DRIVER = 0;
   }

   compute_cif_unit();

   List = Model = (ptype_list *)reverse((chain_list *)cif_model_list( Figure ) );

   if ( ( File = mbkfopen( Figure->NAME, "cif", "w" ) ) == NULL )
   {
     ciferror( CIF_ERROR_FILE_OPEN, Figure->NAME, 0 );
   }

   write_header( File, Figure->NAME );

   while ( Model != (ptype_list *)NULL ) 
   {
     write_model_header( File, ((rdsfig_list *)Model->DATA)->NAME, 
                         Model->TYPE );

     Abox = ((rdsfig_list *)Model->DATA)->LAYERTAB[ RDS_ABOX ];
 
     if ( Abox != (rdsrec_list *)NULL )
     {
       write_model_abox( File, Abox->X, Abox->Y, 
                         Abox->DX, Abox->DY );
     }

     drive_model( File, List, ((rdsfig_list *)Model->DATA) );
     write_model_tail( File );

     Model = Model->NEXT;
   }

   write_figure( File, getcifmodel( List, Figure->NAME ) );

   Abox = Figure->LAYERTAB[ RDS_ABOX ];

   if ( Abox != (rdsrec_list *)NULL )
   {
     write_tail( File, Abox->X, Abox->Y, Abox->DX, Abox->DY );
   }

   (void)fprintf( File, "E \n");

   if ( fclose( File ) )
   {
     ciferror( CIF_ERROR_FILE_CLOSE, Figure->NAME, 0 );
   }
 }
