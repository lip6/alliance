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
| Tool    :                   XPAT                           |
|                                                             |
| File    :                  menu.c                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# include <Xm/Xm.h>
# include <X11/Intrinsic.h>
# include <Xm/RowColumn.h>
# include <Xm/Label.h>
# include <Xm/SeparatoG.h>
# include <Xm/Separator.h>

# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XTB.h"
# include "XSB.h"
# include "XTB_menu.h"

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/
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

  static char MenuBuffer [ MENU_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       XpatBuildMenus                       |
|                                                             |
\------------------------------------------------------------*/

void XpatBuildMenus( Father, Menu )

     Widget          Father;
     XpatMenuItem  *Menu;
{

  Widget   Button;
  Widget   SubMenu;
  XmString Text;
  Arg      Args[ 2 ];
  int      Counter;

  autbegin();

  for ( Counter = 0; 
        Menu[ Counter ].LABEL != NULL; 
        Counter++ )
  {
    if ( Menu[ Counter ].SEPARATOR == True )
    {
	XtCreateManagedWidget( "Separator", 
                             xmSeparatorWidgetClass, Father,
                             NULL                  ,
                             0 );
    }

    Button = XtVaCreateManagedWidget(  Menu[ Counter ].LABEL,
                                       *Menu[ Counter ].CLASS, Father, 
                                       XmNmnemonic, 
                                       Menu[ Counter ].MNEMONIC,
                                       NULL );
    Menu[ Counter ].BUTTON = Button;

    if ( Menu[ Counter ].SHORT_KEY != NULL )
    {
      XtVaSetValues( Button, 
                     XmNaccelerator, 
                     Menu[ Counter ].SHORT_KEY, 
                     NULL );
    }

    if ( Menu[ Counter ].SHORT_KEY != NULL )
    {
      Text = XmStringCreateSimple( Menu[ Counter ].SHORT_KEY_TEXT );

      XtVaSetValues( Button, 
                     XmNacceleratorText, 
                     Text,
                     NULL );

      XmStringFree( Text );
    }

    if ( Menu[ Counter ].CALLBACK != NULL )
    {
      XtAddCallback( Button, 
                     XmNactivateCallback, 
                     Menu[ Counter ].CALLBACK,
                     Menu[ Counter ].CALLDATA );
    }

    if ( Menu[ Counter ].HELP == True )
    {
	XtSetArg( Args[ 0 ], XmNmenuHelpWidget, Button );

	XtSetValues( Father, Args, 1 );
    }

    if ( Menu[ Counter ].NEXT != NULL)
    {
	strcpy( MenuBuffer, Menu[ Counter ].LABEL );
	strcat( MenuBuffer, " Menu"               );

      SubMenu = XmCreatePulldownMenu( Father, 
                                      MenuBuffer,
                                      Args, 0 );

      Menu[ Counter ].MENU = SubMenu;

      if ( Menu[ Counter ].TITLE == True )
      {
	  Text = XmStringCreateLtoR( MenuBuffer, XmSTRING_DEFAULT_CHARSET );

	  XtVaCreateManagedWidget( "MenuTitle", 
                                 xmLabelWidgetClass, 
                                 SubMenu,
                                 XmNlabelString, Text,
                                 NULL );

        XmStringFree( Text );

        XtCreateManagedWidget( "Separator",
                               xmSeparatorWidgetClass,
                               SubMenu,
                               NULL,
                               0 );
	}

	XtSetArg( Args[ 0 ], XmNsubMenuId, SubMenu ); 
	XtSetValues( Button, Args, 1 );

      XpatBuildMenus( SubMenu, Menu[ Counter ].NEXT );
    }
    else Menu[ Counter ].MENU = (Widget)NULL;
  }

  autend();
}

