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
# include <Xm/CascadeBG.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "GTB.h"
# include "GSB.h"
# include "GRD.h"
# include "GMX.h"
# include "GMC.h"

# include "GMC_menu.h"
# include "GMC_create.h"
# include "GMC_message.h"

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

   DrealMenuItem DrealCreateMenu[] =

         {
           {
             "Abutment Box",
             'A',
             "Ctrl<Key>A",
             "Ctrl A",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateAbutmentBox,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (DrealMenuItem *)NULL
           }
           ,
           {
             "Rectangle        ",
             't',
             "Ctrl<Key>T",
             "Ctrl T",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateRectangle,
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
|                    CallbackCreateRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateRectangle( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealChangeEditMode( DREAL_CREATE_RECTANGLE,
                       DrealPromptCreateRectangle );

  DrealEnterPanel( &DrealCreateRectanglePanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackCreateAbutmentBox                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateAbutmentBox( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ( DrealFigureRds    != (rdsfig_list *)0   ) &&
       ( ( DrealFigureXAB1 != 0                ) ||
         ( DrealFigureXAB2 != 0                ) ||
         ( DrealFigureYAB1 != 0                ) ||
         ( DrealFigureYAB2 != 0                ) ) )
  {
    DrealErrorMessage( DrealMainWindow, "There is already one abutment box !");
  }
  else
  {
    DrealChangeEditMode( DREAL_CREATE_ABUTMENTBOX,
                         DrealPromptCreateAbutmentBox );
  }

  rdsend();
}
