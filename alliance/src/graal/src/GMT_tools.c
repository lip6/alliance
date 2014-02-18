/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Tools.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <Xm/Xm.h>
# include <Xm/PushBG.h>
# ifdef HAVE_XPM
# include <X11/xpm.h>
# endif

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rfm.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"
# include "GTB.h"
# include "GSB.h"
# include "GMX.h"
# include "GMF.h"
# include "GRM.h"
# include "GMT.h"
# include "GMV.h"

# include "GMT_tools.h"
# include "GMT_panel.h"
# include "GMT_message.h"
# include "GRM_select.h"
# include "GMF_menu.h"
# include "GMF_file.h"
# include "GRM_window.h"

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

   graalhierarchy *GraalHeadHierarchy = (graalhierarchy *)0;

   char GraalToolsFilter[ 10 ]    = "*.";
   char GraalToolsExtention[ 10 ] = ".";

   char GraalToolsBuffer[ 128 ];
   char GraalToolsDirectoryBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                   Hierarchy Show Buffer                     |
|                                                             |
\------------------------------------------------------------*/

  static char  GraalHierarchyShowMessage[ GRAAL_HIERARCHY_SHOW_MESSAGE_SIZE ];
  static char *GraalScanHierarchyShow;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       GraalAllocHierarchy                   |
|                                                             |
\------------------------------------------------------------*/

graalhierarchy *GraalAllocHierarchy ()

{
  return ( (graalhierarchy *) rdsalloc ( sizeof ( graalhierarchy ), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalFreeHierarchy                    |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeHierarchy ( FreeHierarchy )

  graalhierarchy *FreeHierarchy;

{
  rdsfree( (char *)FreeHierarchy, sizeof(FreeHierarchy) );
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddHierarchy                     |
|                                                             |
\------------------------------------------------------------*/
 
void GraalAddHierarchy( Name )
 
  char *Name;
{
  graalhierarchy *GraalHierarchy;

  rdsbegin();
 
  GraalHierarchy       = GraalAllocHierarchy ();
  GraalHierarchy->NAME = Name;
  GraalHierarchy->X    = GraalLambdaGridX;
  GraalHierarchy->Y    = GraalLambdaGridY;
  GraalHierarchy->DX   = GraalLambdaGridDx;
  GraalHierarchy->DY   = GraalLambdaGridDy;
  GraalHierarchy->NEXT = GraalHeadHierarchy;
  GraalHeadHierarchy   = GraalHierarchy;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalDelHierarchy                   |
|                                                             |
\------------------------------------------------------------*/

char GraalDelHierarchy()

{
  graalhierarchy *GraalHierarchy;

  if ( GraalHeadHierarchy != (graalhierarchy *) NULL )
  {
    rdsbegin();

    GraalHierarchy     = GraalHeadHierarchy;
    GraalHeadHierarchy = GraalHeadHierarchy->NEXT;
    GraalFreeHierarchy( GraalHierarchy );

    rdsend();
    return( GRAAL_TRUE ); 
  }

  return( GRAAL_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalToolsEqui                       |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsEqui()

{ 
  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

  if ( GraalHeadSelect != (graalselect *)NULL )
  {
    GraalExtractEqui( GraalHeadSelect->RECTANGLE );
  }

  GraalDelSelect();
  GraalZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsRealFlatten                   |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsRealFlatten()

{ 
  graalselect  *Select;
  graalselect  *DelSelect;
  graalselect **PrevSelect;
  char          Buffer[ 512 ];

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

  GraalDelPeek();
  
  Select     =  GraalHeadSelect;
  PrevSelect = &GraalHeadSelect;

  while ( Select != (graalselect *)NULL )
  {
    if ( GraalRealFlatInstance( Select->RECTANGLE ) )
    {
      DelSelect   = Select;
      Select      = Select->NEXT;
      *PrevSelect = Select;

      GraalFreeSelect( DelSelect );
    }
    else
    {
      PrevSelect = &Select->NEXT;
      Select     = Select->NEXT;
    }
  }

  GraalDelSelect();

  GraalZoomRefresh();
  GraalDisplayToolsMessage();

  sprintf( Buffer, "%s_real_flat", GraalFigureRds->NAME );
  GraalFigureMbk->NAME = namealloc( Buffer );
  
  GraalChangeTopLevelTitle( GraalFigureMbk->NAME );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalToolsFlatten                     |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsFlatten()

{
  graalselect *Select;
  rdsrec_list *Rectangle;
  rdsins_list *Instance;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

  GraalDelPeek();

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    Instance = (rdsins_list *)GRAAL_PREVIOUS( Rectangle );

    GraalFlatInstance( Instance );
  }

  GraalDelSelect();
  GraalZoomRefresh();
  GraalDisplayToolsMessage();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsUnflatten                     |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsUnflatten()

{ 
  graalselect *Select;
  rdsrec_list *Rectangle;
  rdsins_list *Instance;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );
  
  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    Instance = (rdsins_list *)GRAAL_PREVIOUS( Rectangle );
    GraalUnflatInstance( Instance );
  }

  GraalDelSelect();
  GraalZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalToolsPeek                       |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsPeek( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{ 
  long         Offset;
  graalwin    *ScanWin;
  graalwinrec *ScanWinRec;
  rdsrec_list *Rec;
  graalpeek   *Peek;
  char         ScanRec;
  long         XabMin;
  long         XabMax;
  long         YabMin;
  long         YabMax;
  long         Xmin;
  long         Xmax;
  long         Ymin;
  long         Ymax;
  long         X;
  long         Y;
  long         Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }
  }

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

    GraalDelEqui();
    GraalDelPeek();

    LambdaX1 = LambdaX1 * GRAAL_RDS_LAMBDA; 
    LambdaX2 = LambdaX2 * GRAAL_RDS_LAMBDA; 
    LambdaY1 = LambdaY1 * GRAAL_RDS_LAMBDA; 
    LambdaY2 = LambdaY2 * GRAAL_RDS_LAMBDA; 

    XabMin  = LambdaX1 - GRAAL_PEEK_BOUND;
    YabMin  = LambdaY1 - GRAAL_PEEK_BOUND;
    XabMax  = LambdaX2 + GRAAL_PEEK_BOUND;
    YabMax  = LambdaY2 + GRAAL_PEEK_BOUND;

    Xmin = ( LambdaX1 - GraalWindowXmin ) / GraalWindowSide;
    Xmax = ( LambdaX2 - GraalWindowXmin ) / GraalWindowSide;
    Ymin = ( LambdaY1 - GraalWindowYmin ) / GraalWindowSide;
    Ymax = ( LambdaY2 - GraalWindowYmin ) / GraalWindowSide;

    if ( Xmin < 0             ) Xmin = 0;
    if ( Xmin > GraalWindowDx ) Xmin = GraalWindowDx - 1;
    if ( Ymin < 0             ) Ymin = 0;
    if ( Ymin > GraalWindowDy ) Ymin = GraalWindowDy - 1;

    if ( Xmax < 0             ) Xmax = 0;
    if ( Xmax > GraalWindowDx ) Xmax = GraalWindowDx - 1;
    if ( Ymax < 0             ) Ymax = 0;
    if ( Ymax > GraalWindowDy ) Ymax = GraalWindowDy - 1;

    Y = Ymin;

    while ( Y <= Ymax )
    {
      X = Xmin;

      while ( X <= Xmax )
      {
        Offset = ( Y * GraalWindowDx ) + X;

        ScanWin = &GraalWindowTable[ Offset ];

        if ( ScanWin->LAYERTAB != (graalwinrec **)NULL )
        {
          for ( ScanWinRec  = ScanWin->LAYERTAB[ RDS_ABOX ];
                ScanWinRec != (graalwinrec *)NULL;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRec = 0; ScanRec < GRAAL_MAX_REC ; ScanRec++ )
            {
              Rec = ScanWinRec->RECTAB[ (int)ScanRec ];

              if ( ( Rec != (rdsrec_list *)NULL      ) &&
                   ( ! IsRdsFigRec( Rec )            ) &&
                   ( ! IsGraalDeleted( Rec )         ) &&
                   ( ! IsGraalFlatten( Rec )         ) &&
                   ( ! IsGraalPeek( Rec )            ) &&
                   ( ( Rec->X + Rec->DX ) >=  XabMin ) &&
                   ( ( Rec->Y + Rec->DY ) >=  YabMin ) &&
                   ( ( Rec->X           ) <=  XabMax ) &&
                   ( ( Rec->Y           ) <=  YabMax ) )
              {
                GraalAddPeek( Rec );
              }
            }
          }
        }

        X = X + 1;
      }

      Y = Y + 1;
    }

    for ( Peek  = GraalHeadPeek;
          Peek != (graalpeek *)NULL;
          Peek  = Peek->NEXT )
    {
      GraalPeekInstance( Peek->RECTANGLE,
                         LambdaX1, LambdaY1,
                         LambdaX2, LambdaY2 );
    }

    GraalZoomRefresh();
    GraalDisplayToolsMessage();

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR  );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalToolsUnpeek                     |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsUnpeek()
{
  if ( GraalHeadPeek != (graalpeek *)NULL )
  {
    rdsbegin();

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );
    GraalDelEqui();
    GraalDelPeek();
    GraalZoomRefresh();
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR  );

    rdsend();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsHierarchyDown                 |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsHierarchyDown()
{
  rdsrec_list *Rectangle;
  rdsins_list *Instance;
  char        *ModelName;

  rdsbegin();

  if ( GraalHeadSelect != (graalselect *)0 )
  {
    Rectangle = GraalHeadSelect->RECTANGLE;
    Instance  = (rdsins_list *)GRAAL_PREVIOUS( Rectangle );
    ModelName = Instance->FIGNAME;

    GraalDelSelect();

    if ( GraalHeadUndo != (graalundo *)NULL )
    {
      CallbackFileSaveAs( NULL, NULL, NULL );
    }

    GraalAddHierarchy( GraalFigureRds->NAME );

    GraalFileOpen( ModelName, 1 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsHierarchyShow                 |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsHierarchyShow( LambdaX1, LambdaY1 )

  long LambdaX1;
  long LambdaY1;
{
  graalselect *Select;
  phins_list  *InsMbk;
  phins_list  *ScanInsMbk;
  phfig_list  *ModelMbk;
  long         X;
  long         Y;
  long         Xab1;
  long         Yab1;
  long         Xab2;
  long         Yab2;

  rdsbegin();

  GraalScanHierarchyShow = GraalHierarchyShowMessage;

  if ( GraalFigureMbk == (phfig_list *)0 )
  {
    sprintf( GraalScanHierarchyShow, "No current figure !" );
    GraalScanHierarchyShow += strlen( GraalScanHierarchyShow );
  }
  else
  {
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

    LambdaX1 = (long)( LambdaX1 * GRAAL_SCALE_X );
    LambdaY1 = (long)( LambdaY1 * GRAAL_SCALE_X );

    sprintf( GraalScanHierarchyShow, "FIGURE %s : (%.2f, %.2f)\n",
             GraalFigureMbk->NAME,
             (float)LambdaX1 / (float)SCALE_X,
             (float)LambdaY1 / (float)SCALE_X );

    GraalScanHierarchyShow += strlen( GraalScanHierarchyShow );

    InsMbk = (phins_list *)0;

    for ( Select  = GraalHeadSelect; 
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    {
      if ( IsRdsInstance( Select->RECTANGLE ) )
      {
        InsMbk = (phins_list *)GRAAL_MBK( Select->RECTANGLE );
      }
    }

    GraalDelSelect();

    while ( InsMbk != (phins_list *)0 )
    {
      ModelMbk = Graalgetphfig( InsMbk->FIGNAME, 'A' );

      sprintf( GraalScanHierarchyShow, "INSTANCE %s (%s,%s)\n",
               InsMbk->INSNAME,
               InsMbk->FIGNAME,
             GRAAL_SYMMETRY_NAME_TABLE[ (int)InsMbk->TRANSF ][0] );

      GraalScanHierarchyShow += strlen( GraalScanHierarchyShow );

      if ( ModelMbk == (phfig_list *)0 )
      {
        sprintf( GraalScanHierarchyShow, "Unable to load model %s !!\n",
                 InsMbk->FIGNAME );

        break;
      }

      invxyflat( &X, &Y, LambdaX1, LambdaY1,
                 InsMbk->XINS  , InsMbk->YINS  ,
                 ModelMbk->XAB1, ModelMbk->YAB1,
                 ModelMbk->XAB2, ModelMbk->YAB2,
                 InsMbk->TRANSF );

      sprintf( GraalScanHierarchyShow, "MODEL %s : (%.2f, %.2f)\n",
               InsMbk->FIGNAME,
               (float)X / (float)SCALE_X,
               (float)Y / (float)SCALE_X );

      GraalScanHierarchyShow += strlen( GraalScanHierarchyShow );

      LambdaX1 = X;
      LambdaY1 = Y;

      for ( ScanInsMbk  = ModelMbk->PHINS;
            ScanInsMbk != (phins_list *)0;
            ScanInsMbk  = ScanInsMbk->NEXT )
      {
        ModelMbk = Graalgetphfig( ScanInsMbk->FIGNAME, 'P' );

        if ( ModelMbk != (phfig_list *)0 )
        {
          xyflat
          ( &Xab1              , &Yab1,
            ModelMbk->XAB1     , ModelMbk->YAB1,
            ScanInsMbk->XINS   , ScanInsMbk->YINS,
            ModelMbk->XAB1     , ModelMbk->YAB1,
            ModelMbk->XAB2     , ModelMbk->YAB2,
            ScanInsMbk->TRANSF
          );
   
          xyflat
          ( &Xab2              , &Yab2,
            ModelMbk->XAB2     , ModelMbk->YAB2,
            ScanInsMbk->XINS   , ScanInsMbk->YINS,
            ModelMbk->XAB1     , ModelMbk->YAB1,
            ModelMbk->XAB2     , ModelMbk->YAB2,
            ScanInsMbk->TRANSF
          );

          if ( ( LambdaX1 >= Xab1 ) &&
               ( LambdaX1 <= Xab2 ) &&
               ( LambdaY1 >= Yab1 ) &&
               ( LambdaY1 <= Yab2 ) ) break;
        }
      }

      InsMbk = ScanInsMbk;
    }

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
  }

  GraalDisplayHierarchyShowMessage( GraalHierarchyShowMessage );
  GraalEnterPanel( &GraalToolsHierarchyShowPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsHierarchyUp                   |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsHierarchyUp()
{
  if ( GraalHeadHierarchy == (graalhierarchy *)0 )
  {
    GraalErrorMessage( GraalMainWindow, "No previous model !" ); 
  }
  else
  {
    if ( GraalHeadUndo != (graalundo *)NULL )
    {
      CallbackFileSaveAs( NULL, NULL, NULL );
    }

    GraalFileOpen( GraalHeadHierarchy->NAME, 0 );

    GraalLambdaGridX  = GraalHeadHierarchy->X;
    GraalLambdaGridY  = GraalHeadHierarchy->Y;
    GraalLambdaGridDx = GraalHeadHierarchy->DX;
    GraalLambdaGridDy = GraalHeadHierarchy->DY;

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();

    GraalZoomRefresh();
    GraalDelHierarchy();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsHierarchyReload               |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsHierarchyReload()
{
  long SaveLambdaGridX;
  long SaveLambdaGridY;
  long SaveLambdaGridDx;
  long SaveLambdaGridDy;

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    if ( GraalHeadUndo != (graalundo *)NULL )
    {
      CallbackFileSaveAs( NULL, NULL, NULL );
    }

    SaveLambdaGridX  = GraalLambdaGridX;
    SaveLambdaGridY  = GraalLambdaGridY;
    SaveLambdaGridDx = GraalLambdaGridDx;
    SaveLambdaGridDy = GraalLambdaGridDy;

    GraalFileOpen( GraalFigureRds->NAME, 0 );

    GraalLambdaGridX  = SaveLambdaGridX;
    GraalLambdaGridY  = SaveLambdaGridY;
    GraalLambdaGridDx = SaveLambdaGridDx;
    GraalLambdaGridDy = SaveLambdaGridDy;

    GraalComputeLambdaGrid();
    GraalComputeAndDisplayMap();

    GraalZoomRefresh();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalToolsDelAllHierarchy               |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsDelAllHierarchy()
{
  while ( GraalHeadHierarchy != (graalhierarchy *)0 )
  {
    GraalDelHierarchy();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalToolsLoadError                  |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsLoadError( FileName )

  char *FileName;
{
  rdsfig_list *FigureRds;
  rdsfig_list *HeadFigureRds;
  rdsrec_list *ScanRec;
  rdsrec_list *ErrorRec;
  int          Layer;
  int          Index;
  char        *SWAP_WORK_LIB = WORK_LIB;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );
  GraalUndisplayCursor();

  GraalRecomputeBound = GRAAL_TRUE;

  if ( GRAAL_WORK_LIB == (char *)NULL )
  {
    GRAAL_WORK_LIB = WORK_LIB;
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }
  
  if ( Index >= 0 ) 
  {
    strcpy( GraalToolsDirectoryBuffer, FileName );
    strcpy( GraalToolsBuffer, FileName + Index + 1);

    GraalToolsDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = GraalToolsDirectoryBuffer;
  }
  else
  {
    strcpy( GraalToolsBuffer, FileName );

    WORK_LIB = GRAAL_WORK_LIB;
  }

  Index = strlen( GraalToolsBuffer ) - strlen( GraalToolsExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( GraalToolsBuffer + Index, GraalToolsExtention ) )
    {
      GraalToolsBuffer[ Index ] = '\0';
    }
  }

  HeadFigureRds = HEAD_RDSFIG;
  HEAD_RDSFIG   = (rdsfig_list *)0;

  FigureRds     = Graalgetrdsfig( GraalToolsBuffer, 'V' );

  if ( FigureRds == (rdsfig_list *)0 )
  {
    GraalDisplayToolsMessage();
    GraalErrorMessage( GraalMainWindow, "Unable to open this figure !" );
  }
  else
  {
    FigureRds = Graalrflattenrdsfig( FigureRds, RDS_YES, RDS_NO );

    if ( FigureRds == (rdsfig_list *)0 )
    {
      GraalDisplayToolsMessage();
      GraalErrorMessage( GraalMainWindow, "Unable to open this figure !" );
    }
    else
    {
      for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
      {
        for ( ScanRec  = FigureRds->LAYERTAB[ Layer ];
              ScanRec != (rdsrec_list *)0;
              ScanRec  = ScanRec->NEXT )
        {
          ErrorRec = 
     
            addrdsfigrec( GraalFigureRds, ScanRec->NAME, 
                          GetRdsLayer( ScanRec ),
                          ScanRec->X , ScanRec->Y,
                          ScanRec->DX, ScanRec->DY );
  
          SetGraalDruc( ErrorRec );
  
          if ( ErrorRec->NEXT != (rdsrec_list *)NULL )
          {
            GRAAL_PREVIOUS( ErrorRec->NEXT ) = &ErrorRec->NEXT;
          }
  
          GRAAL_PREVIOUS( ErrorRec ) = 
  
          &GraalFigureRds->LAYERTAB[ GetRdsLayer( ErrorRec ) ];
  
          GraalInsertRectangle( ErrorRec );
          ErrorRec->USER = (void *)ErrorRec;
        }
      }
  
      delrdsfig( FigureRds->NAME );
    }
  }

  while ( HEAD_RDSFIG != (rdsfig_list *)0 )
  {
    delrdsfig( HEAD_RDSFIG->NAME );
  }

  HeadFigureRds = HEAD_RDSFIG;
  HEAD_RDSFIG   = HeadFigureRds;

  GraalZoomRefresh();
  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
  GraalDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalToolsSavePixmap                 |
|                                                             |
\------------------------------------------------------------*/

void GraalToolsSavePixmap( FileName )

  char *FileName;
{
# ifdef HAVE_XPM
  XpmAttributes Attribute;
  char         *XpmFileName;
  int           Error;

  XpmFileName = rdsallocblock( strlen( FileName ) + 4 );
  sprintf( XpmFileName, "%s.xpm", FileName );

  Attribute.colormap  = GraalColorMap;
  Attribute.valuemask = XpmColormap;

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

  Error = XpmWriteFileFromPixmap( GraalGraphicDisplay,
                                  XpmFileName,
                                  GraalGraphicPixmap,
                                  (Pixmap)0, &Attribute );

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR  );

  if ( Error )
  {
    GraalErrorMessage( GraalMainWindow, "Unable to save xpm file !" );
  }

  rdsfreeblock( XpmFileName );
# else

  GraalErrorMessage( GraalMainWindow, "XPM cannot be used: Not enabled at compile time !" );

# endif
}
