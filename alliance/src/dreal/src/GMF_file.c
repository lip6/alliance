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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   File.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include "GRD.h"
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
 
  char DrealFileFilter[ 10 ]    = "*.";
  char DrealFileExtention[ 10 ] = ".";

  char DrealFileBuffer[ 128 ];
  char DrealDirectoryBuffer[ 512 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        DrealFileNew                         |
|                                                             |
\------------------------------------------------------------*/

void DrealFileNew()
{
  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR  );

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    DrealDelSelect();
    DrealDelFigure();
  }

  DrealChangeTopLevelTitle( (char *)NULL );

  DrealRecomputeBound = DREAL_TRUE;

  DrealInitializeZoom();
  DrealInitializeUndo();
  DrealClearGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );

  DrealRefreshGraphicWindow( 0, 0,
                             DrealGraphicDx, DrealGraphicDy );

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealFileSave                        |
|                                                             |
\------------------------------------------------------------*/

void DrealFileSave()
{
  char Ok;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR  );

  Ok = DrealSaveFigure();

  DrealDisplayToolsMessage();

  if ( ! Ok )
  {
    DrealErrorMessage( DrealMainWindow, "Unable to save this figure !" );
  }

  DrealInitializeUndo();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR  );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealFileSaveAs                      |
|                                                             |
\------------------------------------------------------------*/

void DrealFileSaveAs( FileName )

   char *FileName;
{
  rdsins_list *InstanceRds;
  rdsrec_list *Rectangle;
  char         Ok;

  rdsbegin();

  if ( FileName == (char *)NULL )
  {
    FileName = DrealFigureRds->NAME;
  }
  else
  {
    FileName = namealloc( FileName );
  }

  for ( InstanceRds  = DrealFigureRds->INSTANCE;
        InstanceRds != (rdsins_list *)NULL;
        InstanceRds  = InstanceRds->NEXT )
  {
    if ( InstanceRds->FIGNAME == FileName ) break;
  }

  if ( InstanceRds != (rdsins_list *)NULL )
  {
    DrealErrorMessage( DrealMainWindow,
                       "A figure cannot be part of itself !" );
  }
  else
  {
    DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR );

    DrealFigureRds->NAME = FileName;

    DrealChangeTopLevelTitle( DrealFigureRds->NAME );

    for ( Rectangle  = DrealFigureRds->LAYERTAB[ RDS_ABOX ];
          Rectangle != (rdsrec_list *)0;
          Rectangle  = Rectangle->NEXT )
    {
      Rectangle->NAME = FileName;
    }

    Ok = DrealSaveFigure();

    DrealDisplayToolsMessage();

    if ( ! Ok )
    {
      DrealErrorMessage( DrealMainWindow, "Unable to save this figure !" );
    }

    DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
  }

  DrealInitializeUndo();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void DrealFileOpen( FileName )

  char *FileName;
{
  char       *Directory;
  char       *File;
  int         Index;
  char       *SWAP_WORK_LIB = WORK_LIB;

  rdsbegin();

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_WATCH_CURSOR  );
  DrealUndisplayCursor();

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    DrealDelSelect();
    DrealInitializeUndo();
    DrealDelFigure();
  }

  DrealRecomputeBound = DREAL_TRUE;

  if ( DREAL_WORK_LIB == (char *)NULL )
  {
    DREAL_WORK_LIB = WORK_LIB;
  }

  for ( Index = strlen( FileName ); Index >= 0; Index-- )
  {
    if ( FileName[ Index ] == '/' ) break;
  }
  
  if ( Index >= 0 ) 
  {
    strcpy( DrealDirectoryBuffer, FileName );
    strcpy( DrealFileBuffer, FileName + Index + 1);

    DrealDirectoryBuffer[ Index + 1 ] = '\0';

    WORK_LIB = DrealDirectoryBuffer;
  }
  else
  {
    strcpy( DrealFileBuffer, FileName );

    WORK_LIB = DREAL_WORK_LIB;
  }

  Index = strlen( DrealFileBuffer ) - strlen( DrealFileExtention );

  if ( Index >= 0 )
  {
    if ( ! strcmp( DrealFileBuffer + Index, DrealFileExtention ) )
    {
      DrealFileBuffer[ Index ] = '\0';
    }
  }

  DrealAddFigure( DrealFileBuffer );

  DrealInitializeUndo();

  DrealDisplayToolsMessage();

  if ( DrealFigureRds == (rdsfig_list *)NULL )
  {
    DrealErrorMessage( DrealMainWindow, "Unable to open this figure !" );
    DrealChangeTopLevelTitle( (char *)NULL );
    DrealInitializeZoom();
    DrealClearGraphicWindow( 0, 0, DrealGraphicDx, DrealGraphicDy );
  }
  else
  {
    DrealChangeTopLevelTitle( DrealFigureRds->NAME );
    DrealInitializeZoom();
    DrealDisplayFigure( 0, 0, DrealGraphicDx, DrealGraphicDy );
  }

  DrealRefreshGraphicWindow( 0, 0,
                             DrealGraphicDx, DrealGraphicDy );

  DrealSetMouseCursor( DrealGraphicWindow, DREAL_NORMAL_CURSOR );
  DrealDisplayCursor();

  WORK_LIB = SWAP_WORK_LIB;

  rdsend();
}
