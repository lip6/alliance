/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                  rutacces.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <math.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "rwi.h"
# include "rut.h"

# include "rutacces.h"
# include "ruterror.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
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
|                         Symmetry Table                      |
|                                                             |
\------------------------------------------------------------*/

  char RDS_SYMMETRY_TABLE[ RDS_MAX_TRANSF ][ RDS_MAX_TRANSF ] =

  {
    { RDS_NOSYM, RDS_ROT_P, RDS_SYMXY, RDS_ROT_M, RDS_SYM_X, RDS_SY_RM, RDS_SYM_Y, RDS_SY_RP },
    { RDS_ROT_P, RDS_SYMXY, RDS_ROT_M, RDS_NOSYM, RDS_SY_RP, RDS_SYM_X, RDS_SY_RM, RDS_SYM_Y },
    { RDS_SYMXY, RDS_ROT_M, RDS_NOSYM, RDS_ROT_P, RDS_SYM_Y, RDS_SY_RP, RDS_SYM_X, RDS_SY_RM },
    { RDS_ROT_M, RDS_NOSYM, RDS_ROT_P, RDS_SYMXY, RDS_SY_RM, RDS_SYM_Y, RDS_SY_RP, RDS_SYM_X },
    { RDS_SYM_X, RDS_SY_RM, RDS_SYM_Y, RDS_SY_RP, RDS_NOSYM, RDS_ROT_P, RDS_SYMXY, RDS_ROT_M },
    { RDS_SY_RM, RDS_SYM_Y, RDS_SY_RP, RDS_SYM_X, RDS_ROT_M, RDS_NOSYM, RDS_ROT_P, RDS_SYMXY },
    { RDS_SYM_Y, RDS_SY_RP, RDS_SYM_X, RDS_SY_RM, RDS_SYMXY, RDS_ROT_M, RDS_NOSYM, RDS_ROT_P },
    { RDS_SY_RP, RDS_SYM_X, RDS_SY_RM, RDS_SYM_Y, RDS_ROT_P, RDS_SYMXY, RDS_ROT_M, RDS_NOSYM }
  };

/*------------------------------------------------------------\
|                                                             |
|                       Privates variables                    |
|                                                             |
\------------------------------------------------------------*/

  static ptype_list *HEAD_RDSMODEL = (ptype_list *)NULL;
  static long        RdsIndexModel = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Search Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Search Rds Figure                     |
|                                                             |
\------------------------------------------------------------*/

rdsfig_list *searchrdsfig( Name )

  char *Name;
{
  rdsfig_list *Figure;

  Name = namealloc( Name );

  for ( Figure  = HEAD_RDSFIG;
        Figure != (rdsfig_list *)NULL;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;
  }

  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                        Search Rds Instance                  |
|                                                             |
\------------------------------------------------------------*/

rdsins_list *searchrdsins( Figure, Name )

   rdsfig_list *Figure;
   char        *Name;
{
  rdsins_list *Instance;

  Name = namealloc( Name );

  for ( Instance  = Figure->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT )
  {
    if ( Instance->INSNAME == Name ) break;
  }

  return( Instance );
}

/*------------------------------------------------------------\
|                                                             |
|                      getrdsmodellist                        |
|                                                             |
\------------------------------------------------------------*/

ptype_list *getrdsmodellist( Figure )

  rdsfig_list *Figure;
{
  ptype_list  *Head;
  ptype_list  *Stop;
  ptype_list  *Scan;
  ptype_list  *Search;
  rdsfig_list *Model;
  rdsins_list *Instance;

  if ( HEAD_RDSMODEL != (ptype_list *)NULL )
  {
    freeptype( HEAD_RDSMODEL );
    HEAD_RDSMODEL = (ptype_list *)NULL;
  }

  HEAD_RDSMODEL = addptype( HEAD_RDSMODEL, ++RdsIndexModel, (char*)Figure );
  Stop          = (ptype_list *)NULL;

  do
  {
    Head = HEAD_RDSMODEL;

    for ( Scan  = Head;
          Scan != Stop;
          Scan  = Scan->NEXT )
    {
      for ( Instance  = ((rdsfig_list *)(Scan->DATA))->INSTANCE;
            Instance != (rdsins_list *)NULL;
            Instance  = Instance->NEXT )
      {
        for ( Search  = HEAD_RDSMODEL;
              Search != (ptype_list *)NULL;
              Search  = Search->NEXT )
        {
          if (((rdsfig_list *)Search->DATA)->NAME == Instance->FIGNAME )
          {
            break;
          }
        }

        if ( Search == (ptype_list *)NULL )
        {
          Model = searchrdsfig( Instance->FIGNAME );

          if ( Model == (rdsfig_list *)NULL )
          {
            ruterror( RDS_MODEL_NOT_FOUND, Instance->FIGNAME );
          }

          HEAD_RDSMODEL = addptype( HEAD_RDSMODEL, ++RdsIndexModel, (char*)Model );
        }
      }
    }

    Stop = Head;
  }
  while ( Head != HEAD_RDSMODEL );

  return( HEAD_RDSMODEL );
}

/*------------------------------------------------------------\
|                                                             |
|                    Apply Rds Symmetry                       |
|                                                             |
\------------------------------------------------------------*/

void applyrdssym( X, Y, Dx, Dy, Xins, Yins, Sym )

   long  *X;
   long  *Y; 
   long  *Dx;
   long  *Dy;
   long  Xins;
   long  Yins;
   char  Sym;
{
  long  OldX;
  long  OldY;
  long  OldDx;
  long  OldDy;

  OldX  = *X;
  OldY  = *Y;
  OldDx = *Dx;
  OldDy = *Dy;

  switch ( Sym ) 
  {
    case RDS_NOSYM : *X += Xins;
                     *Y += Yins;

                   break;

    case RDS_SYM_X : *X  = Xins - OldX - OldDx;
                     *Y += Yins;
    
                   break;

    case RDS_SYM_Y : *X += Xins ;
                     *Y  = Yins - OldY - OldDy;
    
                   break;

    case RDS_SYMXY : *X  = Xins - OldX - OldDx;
                     *Y  = Yins - OldY - OldDy;
    
                   break;

    case RDS_ROT_P : *X  = Xins - OldY - OldDy;
                     *Y  = Yins + OldX;
                     *Dx = OldDy;
                     *Dy = OldDx;

                   break;

    case RDS_ROT_M : *X  = Xins + OldY;
                     *Y  = Yins - OldX - OldDx;
                     *Dx = OldDy;
                     *Dy = OldDx;
    
                   break;

    case RDS_SY_RP : *X  = Xins + OldY;
                     *Y  = Yins + OldX;
                     *Dx = OldDy;
                     *Dy = OldDx;

                   break;

    case RDS_SY_RM : *X  = Xins - OldY - OldDy;
                     *Y  = Yins - OldX - OldDx;
                     *Dx = OldDy;
                     *Dy = OldDx;

                   break;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         applyrdsmode                        |
|                                                             |
\------------------------------------------------------------*/

void applyrdsmode( Figure, Mode )

  rdsfig_list *Figure;
  char         Mode;
{
  int           Layer; 
  rdsrec_list  *Rectangle;
  rdsrec_list  *InsRec;
  rdsrec_list **Previous;
  rdsins_list  *Instance;
  rdsfig_list  *Model;

  if ( Figure->MODE == Mode ) return;

  if ( Mode == 'P' )
  {
    for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
    {
      Rectangle =  Figure->LAYERTAB[ Layer ];
      Previous  = &Figure->LAYERTAB[ Layer ];

      while ( Rectangle != (rdsrec_list *)NULL )
      {
        if ( ( IsRdsConnector( Rectangle ) ) ||
             ( IsRdsConInter( Rectangle  ) ) ||
             ( IsRdsConExter( Rectangle  ) ) ||
             ( IsRdsFigure( Rectangle    ) ) ||
             ( ( IsRdsSegment( Rectangle ) &&
               ( ( Layer == RDS_TPOLY  ) ||
                 ( Layer == RDS_TPOLY2 ) ||
                 ( Layer == RDS_TALU1  ) ||
                 ( Layer == RDS_TALU2  ) ||
                 ( Layer == RDS_TALU3  ) ||
                 ( Layer == RDS_TALU4  ) ||
                 ( Layer == RDS_TALU5  ) ||
                 ( Layer == RDS_TALU6  ) ||
                 ( Layer == RDS_TALU7  ) ||
                 ( Layer == RDS_TALU8  ) ) ) ) )
        {
          Previous  = &Rectangle->NEXT;
          Rectangle = Rectangle->NEXT;
        }
        else 
        {
          *Previous = Rectangle->NEXT;
          freerdsrec( Rectangle, Figure->SIZE );
          Rectangle = *Previous;
        }
      }
    }

    while ( Figure->INSTANCE != (rdsins_list *)NULL )
    {
      delrdsins( Figure, Figure->INSTANCE->INSNAME );
    }
  }
  else
  if ( Mode == 'V' )
  {
    for ( Instance  = Figure->INSTANCE;
          Instance != (rdsins_list *)NULL;
          Instance  = Instance->NEXT )
    {
      Model = searchrdsfig( Instance->FIGNAME );

      if ( Model == (rdsfig_list *)NULL )
      {
        ruterror( RDS_MODEL_NOT_FOUND, Instance->FIGNAME );
      }
      
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        for ( Rectangle  = Model->LAYERTAB[ Layer ];
              Rectangle != (rdsrec_list *)NULL;
              Rectangle  = Rectangle->NEXT )
        {
          if ( ( IsRdsConnector( Rectangle ) ) ||
               ( IsRdsConInter( Rectangle  ) ) ||
               ( IsRdsConExter( Rectangle  ) ) ||
               ( Layer == RDS_ABOX           ) ||
               ( ( IsRdsSegment( Rectangle ) &&
               ( ( Layer == RDS_TPOLY  ) ||
                 ( Layer == RDS_TPOLY2 ) ||
                 ( Layer == RDS_TALU1  ) ||
                 ( Layer == RDS_TALU2  ) ||
                 ( Layer == RDS_TALU3  ) ||
                 ( Layer == RDS_TALU4  ) ||
                 ( Layer == RDS_TALU5  ) ||
                 ( Layer == RDS_TALU6  ) ||
                 ( Layer == RDS_TALU7  ) ||
                 ( Layer == RDS_TALU8  ) ) ) ) )
          {
             InsRec = 

               addrdsinsrec( Instance, 
                             ( Layer == RDS_ABOX ) 
                           ? Instance->INSNAME : Rectangle->NAME,
                             Layer,
                             Rectangle->X, Rectangle->Y,
                             Rectangle->DX, Rectangle->DY );

            ClearRdsFigRec( InsRec );
            
            if ( IsRdsConnector( Rectangle ) ) 
            {
              SetRdsConnector( InsRec );
              SetRdsConInter( InsRec );
            }
            else
            if ( IsRdsSegment( Rectangle ) )
            {
              SetRdsSegment( InsRec );
              if ( IsRdsConExter( Rectangle ) )
              {
                SetRdsConInter( InsRec );
              }
            }
            else
            {
              SetRdsInstance( InsRec );
            }

            applyrdssym( &InsRec->X, &InsRec->Y, &InsRec->DX, &InsRec->DY,
                         Instance->X, Instance->Y, Instance->TRANSF );
          }
        }
      }
    }
  }
  else
  if ( Mode == 'A' )
  {
    for ( Instance  = Figure->INSTANCE;
          Instance != (rdsins_list *)NULL;
          Instance  = Instance->NEXT )
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        while ( Instance->LAYERTAB[ Layer ] != (rdsrec_list *)NULL )
        {
          delrdsinsrec( Instance, Instance->LAYERTAB[ Layer ] );
        }
      }
    }
  }

  Figure->MODE = Mode;
}

/*------------------------------------------------------------\
|                                                             |
|                    Flatten Functions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Flatten Rds Figure                       |
|                                                             |
\------------------------------------------------------------*/
 
void flattenrdsfig( Figure, InsName, Concat )

  rdsfig_list *Figure;
  char        *InsName;
  char         Concat;
{
  rdsins_list *Instance;
  rdsfig_list *Model;
  rdsrec_list *NewRec;
  rdsrec_list *ScanRec;
  rdsins_list *ScanIns;
  char        *RecName;
  int          Layer;
  long         Xins;
  long         Yins;
  long         Dxins;

  Instance = searchrdsins( Figure, InsName );

  if ( Instance == (rdsins_list *)NULL ) return;

  Model = searchrdsfig( Instance->FIGNAME );

  if ( Model == (rdsfig_list *)NULL )
  {
    ruterror( RDS_MODEL_NOT_FOUND, Instance->FIGNAME );
  }
      
  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    if ( Layer == RDS_ABOX ) continue;

    for ( ScanRec  = Model->LAYERTAB[ Layer ];
          ScanRec != (rdsrec_list *)NULL;
          ScanRec  = ScanRec->NEXT )
    {
      RecName = ScanRec->NAME;

      if ( ( Concat  == RDS_YES   ) &&
           ( RecName != (char *)0 ) )
      {
        RecName = concatname( InsName, RecName );
      }

      NewRec = addrdsfigrec( Figure, RecName, Layer,
                             ScanRec->X, ScanRec->Y,
                             ScanRec->DX, ScanRec->DY );

      SetRdsMbkType( NewRec, GetRdsMbkType( ScanRec ) );

      if ( IsRdsConnector( NewRec ) ) SetRdsSegment( NewRec );

      applyrdssym( &NewRec->X, &NewRec->Y, &NewRec->DX, &NewRec->DY,
                   Instance->X, Instance->Y, Instance->TRANSF );
    }
  }

  for ( ScanIns  = Model->INSTANCE;
        ScanIns != (rdsins_list *)NULL;
        ScanIns  = ScanIns->NEXT )
  {
    Xins  = ScanIns->X;
    Yins  = ScanIns->Y;
    Dxins = 0;

    applyrdssym( &Xins, &Yins, &Dxins, &Dxins,
                 Instance->X, Instance->Y, Instance->TRANSF );

    addrdsins( Figure, ScanIns->FIGNAME, 
               Concat == RDS_NO ? 
               ScanIns->INSNAME : concatname( InsName, ScanIns->INSNAME ),
               RDS_SYMMETRY_TABLE[ (int)ScanIns->TRANSF ][ (int)Instance->TRANSF ],
               Xins, Yins );
  }

  delrdsins( Figure, InsName );
}

/*------------------------------------------------------------\
|                                                             |
|                   rflatten Rds Figure                       |
|                                                             |
\------------------------------------------------------------*/

void rflattenrdsfig( Figure, Concat, Catal )

  rdsfig_list *Figure;
  char         Concat;
  char         Catal;
{
  rdsins_list *Instance;

  Instance = Figure->INSTANCE;

  while ( Instance != (rdsins_list *)NULL )
  {
    if ( ( Catal == RDS_NO                      ) ||     
         ( ! incatalog( Instance->FIGNAME )     ) || 
         ( incatalogdelete( Instance->FIGNAME ) ) )
    {
      flattenrdsfig( Figure, Figure->INSTANCE->INSNAME, Concat );

      Instance = Figure->INSTANCE;
    }
    else
    {
      Instance = Instance->NEXT;
    }
  }
}
