/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   File.c                          |
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
# include <string.h>
# include <Xm/Xm.h>
# include <Xm/FileSB.h>
# include <Xm/SelectioB.h>
# include <Xm/PushBG.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMF.h"

# include "GMF_file.h"

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
 
  char GraalFileFilter[ 10 ]    = "*.";
  char GraalFileExtention[ 10 ] = ".";

  char GraalFileBuffer[ 128 ];
  char GraalFileDirectoryBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        GraalFileNew                         |
|                                                             |
\------------------------------------------------------------*/

void GraalFileNew()
{
  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    GraalDelEqui();
    GraalDelPeek();
    GraalDelSelect();
    GraalDelDrucFigure();
    GraalDelFigure();
  }

  GraalChangeTopLevelTitle( (char *)NULL );

  GraalRecomputeBound = GRAAL_TRUE;

  GraalInitializeZoom();
  GraalInitializeUndo();
  GraalClearGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );

  GraalRefreshGraphicWindow( 0, 0, 
                             GraalGraphicDx, GraalGraphicDy );

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalFileSave                        |
|                                                             |
\------------------------------------------------------------*/

void GraalFileSave()
{
  char Ok;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );

  Ok = GraalSaveFigure();

  GraalDisplayToolsMessage();

  if ( ! Ok )
  {
    GraalErrorMessage( GraalMainWindow, "Unable to save this figure !" );
  }

  GraalInitializeUndo();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR  );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalFileSaveAs                      |
|                                                             |
\------------------------------------------------------------*/

void GraalFileSaveAs( FileName )

   char *FileName;
{
  rdsins_list *InstanceRds;
  char         Ok;

  rdsbegin();

  if ( FileName == (char *)NULL ) 
  {
    FileName = GraalFigureMbk->NAME;
  }
  else
  {
    FileName = namealloc( FileName );
  }

  for ( InstanceRds  = GraalFigureRds->INSTANCE; 
        InstanceRds != (rdsins_list *)NULL;
        InstanceRds  = InstanceRds->NEXT )
  {
    if ( InstanceRds->FIGNAME == FileName ) break;
  }

  if ( InstanceRds != (rdsins_list *)NULL )
  {
    GraalErrorMessage( GraalMainWindow,
                       "A figure cannot be part of itself !" );
  }
  else
  {
    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR );

    GraalFigureMbk->NAME = FileName;
    GraalFigureRds->NAME = FileName;

    GraalChangeTopLevelTitle( GraalFigureMbk->NAME );

    if ( GraalFigureRds->LAYERTAB[ RDS_ABOX ] != (rdsrec_list *)NULL ) 
    {
      GraalFigureRds->LAYERTAB[ RDS_ABOX ]->NAME = FileName;
    }

    Ok = GraalSaveFigure();

    GraalDisplayToolsMessage();

    if ( ! Ok )
    {
      GraalErrorMessage( GraalMainWindow, "Unable to save this figure !" );
    }

    GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
  }

  GraalInitializeUndo();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void GraalFileOpen( FileName, DisplayFigure  )

  char *FileName;
  int   DisplayFigure;
{
  int         Index;
  char       *SWAP_WORK_LIB = WORK_LIB;

  rdsbegin();

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_WATCH_CURSOR  );
  GraalUndisplayCursor();

  if ( GraalFigureMbk != (phfig_list *)NULL )
  {
    GraalDelEqui();
    GraalDelPeek();
    GraalDelSelect();
    GraalDelDrucFigure();
    GraalInitializeUndo();
    GraalDelFigure();
  }

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
    strcpy( GraalFileDirectoryBuffer, FileName );
    strcpy( GraalFileBuffer, FileName + Index + 1);

    GraalFileDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = GraalFileDirectoryBuffer;
  }
  else
  {
    strcpy( GraalFileBuffer, FileName );

    WORK_LIB = GRAAL_WORK_LIB;
  }

  Index = strlen( GraalFileBuffer ) - strlen( GraalFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( GraalFileBuffer + Index, GraalFileExtention ) )
    {
      GraalFileBuffer[ Index ] = '\0';
    }
  }

  GraalFigureMbk = Graalgetphfig( GraalFileBuffer, 'A' );

  GraalInitializeUndo();

  GraalDisplayToolsMessage();

  if ( GraalFigureMbk == (phfig_list *)NULL )
  {
    GraalErrorMessage( GraalMainWindow, "Unable to open this figure !" );
    GraalChangeTopLevelTitle( (char *)NULL );
    GraalInitializeZoom();
    GraalClearGraphicWindow( 0, 0, GraalGraphicDx, GraalGraphicDy );
  }
  else
  {
    GraalAddFigure();
    GraalChangeTopLevelTitle( GraalFigureMbk->NAME );
    GraalInitializeZoom();

    if ( DisplayFigure )
    {
      GraalDisplayFigure( 0, 0, GraalGraphicDx, GraalGraphicDy );
    }
  }

  if ( DisplayFigure )
  {
    GraalRefreshGraphicWindow( 0, 0,
                               GraalGraphicDx, GraalGraphicDy );
  }

  GraalSetMouseCursor( GraalGraphicWindow, GRAAL_NORMAL_CURSOR );
  GraalDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  rdsend();
}
