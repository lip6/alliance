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
| File    :                   Menu.c                          |
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
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMF.h"

# include "GMF_menu.h"
# include "GMF_dialog.h"
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
/*------------------------------------------------------------\
|                                                             |
|                            Menu                             |
|                                                             |
\------------------------------------------------------------*/

   DrealMenuItem DrealFileMenu[] =

         {
           {
             "New",
             'N',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileNew,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Open",
             'O',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileOpen,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Save     ",
             'S',
             "Ctrl<Key>W",
             "Ctrl W",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileSave,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Save As",
             'A',
             NULL,
             NULL,
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileSaveAs,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Quit      ",
             'Q',
             "Ctrl<Key>Q",
             "Ctrl Q",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackFileQuit,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           NULL
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileNew                         |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileNew( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( DrealHeadUndo != (drealundo *)NULL )
  {
    CallbackFileSaveAs( NULL, NULL, NULL );
  }

  DrealFileNew();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileOpen                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileOpen( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Filter;

  rdsbegin();

  if ( DrealFileFilter[2] == '\0' )
  {
    strcat( DrealFileFilter, RDS_IN );
    strcat( DrealFileExtention, RDS_IN );

    Filter    = XmStringCreateSimple( DrealFileFilter ); 

    XtVaSetValues( DrealFileOpenDialog.WIDGET, 
                   XmNpattern, Filter, NULL);

    XmStringFree( Filter );
  }

  DrealEnterDialog( &DrealFileOpenDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileSave                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileSave( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( DrealFigureRds != (rdsfig_list *)NULL )
  {
    if ( ( DrealFigureRds->NAME == (char *)NULL  ) ||
         ( ! strcmp( DrealFigureRds->NAME,
                     DREAL_DEFAULT_FIGURE_NAME ) ) )

    {
      CallbackFileSaveAs( NULL, NULL, NULL );
    }
    else
    {
      DrealFileSave();
    }
  }
  else
  {
    DrealErrorMessage( DrealMainWindow, "No current figure !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackFileSaveAs                       |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileSaveAs( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  Arg      Args;
  XmString SaveAsString;
  
  rdsbegin();

  if ( DrealFigureRds == (rdsfig_list *)NULL )
  {
    DrealErrorMessage( DrealMainWindow, "No current figure !" );
  }
  else
  {
    SaveAsString = XmStringCreateLtoR( DrealFigureRds->NAME,
                                       XmSTRING_DEFAULT_CHARSET );

    XtVaSetValues( DrealFileSaveAsDialog.WIDGET,
                   XmNtextString, SaveAsString, NULL );

    XmStringFree( SaveAsString );

    DrealEnterDialog( &DrealFileSaveAsDialog );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackFileQuit                        |
|                                                             |
\------------------------------------------------------------*/

void CallbackFileQuit( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealFileQuitDialog );

  rdsend();
}
