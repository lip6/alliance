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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Panel.c                         |
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
# include "GRD.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GME.h"

# include "GME_panel.h"
# include "GME_message.h"
# include "GME_dialog.h"
# include "GME_modify.h"
# include "GME_search.h"
# include "GME_select.h"

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
|                             Panel                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Search View Panel                    |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealEditSearchViewButton[] =

         {
           {
             "Continue",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackEditSearchViewContinue,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Abort",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackEditSearchViewAbort,
             (XtPointer)NULL,
             (Widget)NULL   
           }
         };

   DrealPanelItem DrealEditSearchViewPanel  =

         {
           "View search",
           1,
           0,
           DREAL_EDIT_VIEW_SEARCH_X,
           DREAL_EDIT_VIEW_SEARCH_Y,
           100, 
           50,
           1,  
           2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           DrealEditSearchViewButton
         };

  int DrealEditSearchViewDefaultValues[ 5 ] =

         {
           DREAL_EDIT_VIEW_SEARCH_X, 
           DREAL_EDIT_VIEW_SEARCH_Y,
           100, 50, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Identify Edit Panel                  |
|                                                             |
\------------------------------------------------------------*/


   DrealPanelButtonItem DrealEditIdentifyButton[] =

         {
           {
             "Text",
             "Nothing", NULL, 0, 0,
             NULL,
             NULL,
              0, 0,
              8, 9,
             NULL,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             3, 9,
             2, 1,
             CallbackEditCloseIdentify,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   DrealPanelItem DrealEditIdentifyPanel =

         {
           "Identify",
           1,
           0,
           DREAL_EDIT_IDENTIFY_X,
           DREAL_EDIT_IDENTIFY_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           DrealEditIdentifyButton
         };

  int DrealEditIdentifyDefaultValues[ 5 ] =

         {
           DREAL_EDIT_IDENTIFY_X, 
           DREAL_EDIT_IDENTIFY_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Select Panel                         |
|                                                             |
\------------------------------------------------------------*/

   DrealPanelButtonItem DrealEditSelectButton[] =

         {
           {
             "Select",
             "List",
             "List", 0, 0,
             NULL,
             NULL,
             0, 0,
             8, 9,
             CallbackEditSelectList,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             "Accept",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             1, 9,
             2, 1,
             CallbackEditSelectAccept,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Cancel",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             5, 9,
             2, 1,
             CallbackEditSelectCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   DrealPanelItem DrealEditSelectPanel =

         {
           "Select",
           1,
           0,
           DREAL_EDIT_SELECT_X,
           DREAL_EDIT_SELECT_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           3,
           DrealEditSelectButton
         };

  int DrealEditSelectDefaultValues[ 5 ] =

         {
           DREAL_EDIT_IDENTIFY_X,
           DREAL_EDIT_IDENTIFY_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Search Element Panel                   |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealEditSearchButton[] =

         {
           {
             "Rectangle",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackEditSearchRectangle,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackEditCloseSearch,
             (XtPointer)NULL,
             (Widget)NULL
           } 
         };
           
   DrealPanelItem DrealEditSearchPanel  =

         {
           "Search",
           1,
           0,
           DREAL_EDIT_SEARCH_X,
           DREAL_EDIT_SEARCH_Y,
           100,
           50, 
           1, 
           2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           DrealEditSearchButton
         };

  int DrealEditSearchDefaultValues[ 5 ] =

         {
           DREAL_EDIT_SEARCH_X,
           DREAL_EDIT_SEARCH_Y,
           100, 50, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                   Modify Rectangle Panel                    |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealModifyRectangleButton[RDS_ALL_LAYER + 3] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyRectangleLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Name",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyRectangleName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Apply",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyRectangleApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Cancel",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyRectangleCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   DrealPanelItem DrealModifyRectanglePanel  =

         {
           "Modify Rectangle",
           1,
           0,
           DREAL_MODIFY_RECTANGLE_X,
           DREAL_MODIFY_RECTANGLE_Y, 
           100,
           300,
           2,  
           (RDS_ALL_LAYER + 3 ) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           RDS_ALL_LAYER + 3,
           DrealModifyRectangleButton
         };

  int DrealModifyRectangleDefaultValues[ 5 ] =

         {
           DREAL_MODIFY_RECTANGLE_X, 
           DREAL_MODIFY_RECTANGLE_Y,
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                         Global Edit                         |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealEditGlobalButton[] =
 
         {
           {
             "Undo",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackEditGlobalUndo,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Redo",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackEditGlobalRedo,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Copy",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackEditGlobalCopy,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Move",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackEditGlobalMove,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Delete",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 4,
             1, 1,
             CallbackEditGlobalDelete,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Stretch",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             1, 1,
             CallbackEditGlobalStretch,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Modify",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 6,
             1, 1,
             CallbackEditGlobalModify,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Identify",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 7,
             1, 1,
             CallbackEditGlobalIdentify,
             (XtPointer)0,
             (Widget)NULL
           } 
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 8,
             1, 1,
             CallbackEditCloseGlobal,
             (XtPointer)0,
             (Widget)NULL   
           }
         };

   DrealPanelItem DrealEditGlobalPanel  =

         {
           "Edit",
           1,
           0,
           DREAL_EDIT_GLOBAL_X,
           DREAL_EDIT_GLOBAL_Y,
           100, 
           225,
           1, 
           9,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           9,
           DrealEditGlobalButton
         };

  int DrealEditGlobalDefaultValues[ 5 ] =

         {
           DREAL_EDIT_GLOBAL_X, 
           DREAL_EDIT_GLOBAL_Y,
           100, 225, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                         Global Window                       |
|                                                             |
\------------------------------------------------------------*/

   static DrealPanelButtonItem DrealWindowGlobalButton[] =
 
         {
           {
             "Undo",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackEditGlobalUndo,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Redo",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackEditGlobalRedo,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Copy",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackEditGlobalCopy,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             "Move",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackEditGlobalMove,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             "Delete",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 4,
             1, 1,
             CallbackEditGlobalDelete,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             "Modify",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             1, 1,
             CallbackEditGlobalModify,
             (XtPointer)1,
             (Widget)NULL
           }
           ,
           {
             "Identify",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 6,
             1, 1,
             CallbackEditGlobalIdentify,
             (XtPointer)1,
             (Widget)NULL
           } 
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 7,
             1, 1,
             CallbackEditCloseGlobal,
             (XtPointer)1,
             (Widget)NULL   
           }
           ,
           NULL
         };

   DrealPanelItem DrealWindowGlobalPanel  =

         {
           "Window",
           1,
           0,
           DREAL_WINDOW_GLOBAL_X,
           DREAL_WINDOW_GLOBAL_Y,
           100,
           200,
           1,
           8,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           8,
           DrealWindowGlobalButton
         };

  int DrealWindowGlobalDefaultValues[ 5 ] =

         {
           DREAL_WINDOW_GLOBAL_X,
           DREAL_WINDOW_GLOBAL_Y,
           100, 200, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    DrealBuildPanelModify                    |
|                                                             |
\------------------------------------------------------------*/

void DrealBuildPanelModify()

{
  DrealPanelButtonItem *Button;
  long                  Layer;
  long                  Index;
  unsigned char         X1;
  unsigned char         Y1;

  X1    = 0;
  Y1    = 0;
  Index = 0;

  for ( Layer = 0; Layer < RDS_LAYER_ABOX; Layer++ )
  {
    if ( ( DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ] != NULL ) &&
         ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED   ) )
    {
      Button = &DrealModifyRectangleButton[ Index ];

      Button->LABEL      = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 0 ];
      Button->FOREGROUND = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 1 ];
      Button->BACKGROUND = DREAL_RDS_LAYER_NAME_TABLE[ Layer ][ 2 ];

      Button->X        = X1;
      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)(long)RDS_DYNAMIC_LAYER[ Layer ];

      if ( X1 == 1 )
      {
        X1 = 0; Y1 = Y1 + 1;
      }
      else
      {
        X1 = X1 + 1;
      }

      Index = Index + 1;
    }
  }

  if ( X1 == 1 ) Y1 = Y1 + 1;

  for ( Index = 0; Index < 3; Index++ )
  {
    DrealModifyRectangleButton[ RDS_ALL_LAYER + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  DrealModifyRectanglePanel.ROW = Y1;
  DrealModifyRectangleDefaultValues[ 3 ] = Y1 * 25;
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Global Edit                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditGlobalUndo                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalUndo( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEditUndo();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditGlobalRedo                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalRedo( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEditRedo();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditGlobalCopy                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalCopy( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_COPY,
                         DrealPromptSelectWindowCopy );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_COPY,
                         DrealPromptSelectPointCopy );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditGlobalMove                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalMove( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_MOVE,
                         DrealPromptSelectWindowMove );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_MOVE,
                         DrealPromptSelectPointMove );
  }

  rdsend();
}
/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditGlobalDelete                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalDelete( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_DELETE,
                         DrealPromptSelectWindowDelete );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_DELETE,
                         DrealPromptSelectPointDelete );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditGlobalStretch                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalStretch( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealChangeEditMode( DREAL_SELECT_POINT_STRETCH,
                       DrealPromptSelectPointStretch );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditGlobalModify                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalModify( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_MODIFY,
                         DrealPromptSelectWindowModify );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_MODIFY,
                         DrealPromptSelectPointModify );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditGlobalIdentify                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditGlobalIdentify( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    DrealChangeEditMode( DREAL_SELECT_WINDOW_IDENTIFY,
                         DrealPromptSelectWindowIdentify );
  }
  else
  {
    DrealChangeEditMode( DREAL_SELECT_POINT_IDENTIFY,
                         DrealPromptSelectPointIdentify );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditCloseGlobal                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditCloseGlobal( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ClientData )
  {
    DrealExitPanel( &DrealWindowGlobalPanel );
  }
  else
  {
    DrealExitPanel( &DrealEditGlobalPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       Callback For Search                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSearchRectangle              |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackEditSearchRectangle( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  DrealEnterDialog( &DrealSearchRectangleDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditCloseSearch                  |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackEditCloseSearch( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  DrealExitPanel( &DrealEditSearchPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For View Search                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditSearchViewContinue             |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearchViewContinue( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackEditSearchViewAbort                |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearchViewAbort( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealSearchAbort = DREAL_TRUE; 

  DrealExitDialog();

  rdsbegin();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Identify                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditCloseIdentify                |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditCloseIdentify(  MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealEditIdentifyPanel );

  DrealExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    DrealDisplayIdentifyMessage              |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplayIdentifyMessage( Message )

  char *Message;
{
  rdsbegin();

  XmTextSetString( DrealEditIdentifyButton[0].BUTTON, Message );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Select                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSelectList                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSelectList(  MyWidget, ClientData, CallData )

     Widget                MyWidget;
     caddr_t               ClientData;
     XmListCallbackStruct *CallData;
{
  rdsbegin();

  if ( ClientData == (caddr_t)NULL )
  {
    CallbackEditSelectAccept( MyWidget, (caddr_t)NULL, (caddr_t)NULL );
  }
  else
  {
    DrealEditSelectRectangle( CallData->item_position - 1 );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSelectAccept                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSelectAccept(  MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealEditSelectPanel );

  DrealExitDialog();

  DrealEditSelectAccept();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSelectCancel                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSelectCancel(  MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealExitPanel( &DrealEditSelectPanel );

  DrealExitDialog();

  DrealEditSelectCancel();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  DrealDisplaySelectList                     |
|                                                             |
\------------------------------------------------------------*/

void DrealDisplaySelectList( Message )

  char *Message;
{
  XmString     Label;

  rdsbegin();

  if ( Message == (char *)NULL )
  {
    XmListDeleteAllItems( DrealEditSelectButton[ 0 ].BUTTON );
  }
  else
  {
    Label = XmStringCreateSimple( Message );
    XmListAddItem( DrealEditSelectButton[ 0 ].BUTTON , Label , 0 );
    XmStringFree( Label );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Modify Rectangle            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyRectangleLayer             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyRectangleLayer( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealRectangleMLayer = (int)ClientData;

  DrealPromptModifyRectangle();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyRectangleName              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyRectangleName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealEnterDialog( &DrealModifyRectangleNameDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyRectangleApply             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyRectangleApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealModifyLock   = DREAL_FALSE;
  DrealModifyAbort  = DREAL_FALSE;
  DrealModifyApply  = DREAL_TRUE;

  DrealExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyRectangleCancel            |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyRectangleCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  DrealModifyLock   = DREAL_FALSE;
  DrealModifyAbort  = DREAL_TRUE;
  DrealModifyApply  = DREAL_FALSE;

  DrealExitDialog();

  rdsend();
}
