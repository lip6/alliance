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
# include "GRM.h"
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

   static GraalPanelButtonItem GraalEditSearchViewButton[] =

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

   GraalPanelItem GraalEditSearchViewPanel  =

         {
           "View search",
           1,
           0,
           GRAAL_EDIT_VIEW_SEARCH_X,
           GRAAL_EDIT_VIEW_SEARCH_Y,
           100, 
           50,
           1,  
           2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           GraalEditSearchViewButton
         };

  int GraalEditSearchViewDefaultValues[ 5 ] =

         {
           GRAAL_EDIT_VIEW_SEARCH_X, 
           GRAAL_EDIT_VIEW_SEARCH_Y,
           100, 50, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Identify Edit Panel                  |
|                                                             |
\------------------------------------------------------------*/


   GraalPanelButtonItem GraalEditIdentifyButton[] =

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

   GraalPanelItem GraalEditIdentifyPanel =

         {
           "Identify",
           1,
           0,
           GRAAL_EDIT_IDENTIFY_X,
           GRAAL_EDIT_IDENTIFY_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           2,
           GraalEditIdentifyButton
         };

  int GraalEditIdentifyDefaultValues[ 5 ] =

         {
           GRAAL_EDIT_IDENTIFY_X, 
           GRAAL_EDIT_IDENTIFY_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Select Panel                         |
|                                                             |
\------------------------------------------------------------*/

   GraalPanelButtonItem GraalEditSelectButton[] =

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

   GraalPanelItem GraalEditSelectPanel =

         {
           "Select",
           1,
           0,
           GRAAL_EDIT_SELECT_X,
           GRAAL_EDIT_SELECT_Y,
           360,
           250,
           8,
           10,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           3,
           GraalEditSelectButton
         };

  int GraalEditSelectDefaultValues[ 5 ] =

         {
           GRAAL_EDIT_IDENTIFY_X,
           GRAAL_EDIT_IDENTIFY_Y,
           360, 250, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                      Search Element Panel                   |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalEditSearchButton[] =

         {
           {
             "Connector",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackEditSearchConnector,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Instance",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 1,
             1, 1,
             CallbackEditSearchInstance,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Reference",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 2,
             1, 1,
             CallbackEditSearchReference,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             "Segment",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 3,
             1, 1,
             CallbackEditSearchSegment,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             "Via",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 4,
             1, 1,
             CallbackEditSearchVia,
             (XtPointer)NULL,
             (Widget)NULL   
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             1, 1,
             CallbackEditCloseSearch,
             (XtPointer)NULL,
             (Widget)NULL
           } 
         };
           
   GraalPanelItem GraalEditSearchPanel  =

         {
           "Search",
           1,
           0,
           GRAAL_EDIT_SEARCH_X,
           GRAAL_EDIT_SEARCH_Y,
           100,
           150, 
           1, 
           6,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           6,
           GraalEditSearchButton
         };

  int GraalEditSearchDefaultValues[ 5 ] =

         {
           GRAAL_EDIT_SEARCH_X,
           GRAAL_EDIT_SEARCH_Y,
           100, 150, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                   Modify Segment Panel                      |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifySegmentButton[MBK_MAX_LAYER+4] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
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
             CallbackModifySegmentLayer,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Width",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifySegmentWidth,
             (XtPointer)NULL,
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
             CallbackModifySegmentName,
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
             CallbackModifySegmentApply,
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
             CallbackModifySegmentCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalModifySegmentPanel  =

         {
           "Modify Segment",
           1,
           0,
           GRAAL_MODIFY_SEGMENT_X,
           GRAAL_MODIFY_SEGMENT_Y, 
           100,
           500,
           2,  
           (MBK_MAX_LAYER + 4) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_LAYER + 4,
           GraalModifySegmentButton
         };

  int GraalModifySegmentDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_SEGMENT_X, 
           GRAAL_MODIFY_SEGMENT_Y,
           100, 500, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Via Panel                       |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifyViaButton[MBK_MAX_VIA + 3] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
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
             CallbackModifyViaType,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Name",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyViaName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Apply",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 10,
             1, 1,
             CallbackModifyViaApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Cancel",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 11,
             1, 1,
             CallbackModifyViaCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalModifyViaPanel  =

         {
           "Modify Via",
           1,
           0,
           GRAAL_MODIFY_VIA_X,
           GRAAL_MODIFY_VIA_Y, 
           100, 
           300,
           1,  
           MBK_MAX_VIA + 3,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_VIA + 3,
           GraalModifyViaButton
         };

  int GraalModifyViaDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_VIA_X, 
           GRAAL_MODIFY_VIA_Y,
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          BigVia Panel                       |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifyBigViaButton[MBK_MAX_VIA + 3] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
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
             CallbackModifyBigViaType,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Name",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyBigViaName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Apply",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 10,
             1, 1,
             CallbackModifyBigViaApply,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Cancel",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 11,
             1, 1,
             CallbackModifyBigViaCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalModifyBigViaPanel  =

         {
           "Modify BigVia",
           1,
           0,
           GRAAL_MODIFY_BIGVIA_X,
           GRAAL_MODIFY_BIGVIA_Y, 
           100, 
           300,
           1,  
           MBK_MAX_VIA + 3,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_VIA + 3,
           GraalModifyBigViaButton
         };

  int GraalModifyBigViaDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_BIGVIA_X, 
           GRAAL_MODIFY_BIGVIA_Y,
           100, 300, 0
         };


/*------------------------------------------------------------\
|                                                             |
|                        Transistor Panel                     |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifyTransistorButton[MBK_MAX_LAYER+4] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
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
             CallbackModifyTransistorType,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Width",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyTransistorWidth,
             (XtPointer)NULL,
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
             CallbackModifyTransistorName,
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
             CallbackModifyTransistorApply,
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
             CallbackModifyTransistorCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalModifyTransistorPanel  =

         {
           "Modify Transistor",
           1,
           0,
           GRAAL_MODIFY_TRANSISTOR_X,
           GRAAL_MODIFY_TRANSISTOR_Y,
           100, 
           300,
           2,  
           (MBK_MAX_LAYER + 4) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_LAYER + 4,
           GraalModifyTransistorButton
         };

  int GraalModifyTransistorDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_TRANSISTOR_X, 
           GRAAL_MODIFY_TRANSISTOR_Y,
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                       Connector Panel                       |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifyConnectorButton[MBK_MAX_LAYER+MBK_MAX_ORIENT+4] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
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
             CallbackModifyConnectorLayer,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Width",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyConnectorWidth,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyConnectorOrient,
             (XtPointer)GRAAL_NORTH,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyConnectorOrient,
             (XtPointer)GRAAL_SOUTH,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyConnectorOrient,
             (XtPointer)GRAAL_EAST,
             (Widget)NULL
           }
           ,
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackModifyConnectorOrient,
             (XtPointer)GRAAL_WEST,
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
             CallbackModifyConnectorName,
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
             CallbackModifyConnectorApply,
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
             CallbackModifyConnectorCancel,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalModifyConnectorPanel  =

         {
           "Modify Connector",
           1,
           0,
           GRAAL_MODIFY_CONNECTOR_X,
           GRAAL_MODIFY_CONNECTOR_Y,
           100, 
           375,
           2,
           (MBK_MAX_LAYER+MBK_MAX_ORIENT+4) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (MBK_MAX_LAYER+MBK_MAX_ORIENT+4),
           GraalModifyConnectorButton
         };

  int GraalModifyConnectorDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_CONNECTOR_X, 
           GRAAL_MODIFY_CONNECTOR_Y,
           100, 375, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Reference Panel                      |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifyReferenceButton[MBK_MAX_REFERENCE+3] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyReferenceType,
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
             CallbackModifyReferenceType,
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
             1, 1,
             CallbackModifyReferenceName,
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
             1, 1,
             CallbackModifyReferenceApply,
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
             1, 1,
             CallbackModifyReferenceCancel,
             (XtPointer)NULL,
             (Widget)NULL   
           }
        };

   GraalPanelItem GraalModifyReferencePanel  =

         {
           "Modify Reference",
           1,
           0,
           GRAAL_MODIFY_REFERENCE_X,
           GRAAL_MODIFY_REFERENCE_Y,
           100, 
           125,
           1,  
           MBK_MAX_REFERENCE + 3,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_REFERENCE + 3,
           GraalModifyReferenceButton
         };

  int GraalModifyReferenceDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_REFERENCE_X,
           GRAAL_MODIFY_REFERENCE_Y, 
           100, 125, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Instance  Panel                      |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalModifyInstanceButton[MBK_MAX_SYMMETRY+4] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             CallbackModifyInstanceSym,
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
             1, 1,
             CallbackModifyInstanceName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Model",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackModifyInstanceModel,
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
             1, 1,
             CallbackModifyInstanceApply,
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
             1, 1,
             CallbackModifyInstanceCancel,
             (XtPointer)NULL,
             (Widget)NULL   
           }
        };  

   GraalPanelItem GraalModifyInstancePanel  =

         {
           "Modify Instance",
           1,
           0,
           GRAAL_MODIFY_INSTANCE_X,
           GRAAL_MODIFY_INSTANCE_Y,
           100, 
           300,
           1,  
           MBK_MAX_SYMMETRY + 4,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_SYMMETRY + 4,
           GraalModifyInstanceButton
         };

  int GraalModifyInstanceDefaultValues[ 5 ] =

         {
           GRAAL_MODIFY_INSTANCE_X,
           GRAAL_MODIFY_INSTANCE_Y,
           100, 300, 0
         };


/*------------------------------------------------------------\
|                                                             |
|                         Global Edit                         |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalEditGlobalButton[] =
 
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

   GraalPanelItem GraalEditGlobalPanel  =

         {
           "Edit",
           1,
           0,
           GRAAL_EDIT_GLOBAL_X,
           GRAAL_EDIT_GLOBAL_Y,
           100, 
           225,
           1, 
           9,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           9,
           GraalEditGlobalButton
         };

  int GraalEditGlobalDefaultValues[ 5 ] =

         {
           GRAAL_EDIT_GLOBAL_X, 
           GRAAL_EDIT_GLOBAL_Y,
           100, 225, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                         Global Window                       |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalWindowGlobalButton[] =
 
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
             "Stretch",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 5,
             1, 1,
             CallbackEditGlobalStretch,
             (XtPointer)1,
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
             (XtPointer)1,
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
             (XtPointer)1,
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
             (XtPointer)1,
             (Widget)NULL   
           }
           ,
           NULL
         };

   GraalPanelItem GraalWindowGlobalPanel  =

         {
           "Window",
           1,
           0,
           GRAAL_WINDOW_GLOBAL_X,
           GRAAL_WINDOW_GLOBAL_Y,
           100,
           225,
           1,
           9,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           9,
           GraalWindowGlobalButton
         };

  int GraalWindowGlobalDefaultValues[ 5 ] =

         {
           GRAAL_WINDOW_GLOBAL_X,
           GRAAL_WINDOW_GLOBAL_Y,
           100, 225, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    GraalBuildPanelModify                    |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildPanelModify()

{
  GraalPanelButtonItem *Button;
  long                  Index;
  unsigned char         X1;
  unsigned char         Y1;

  X1 = 0;
  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_LAYER; Index++ )
  {
    if ( GRAAL_SEGMENT_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifySegmentButton[ Index ];

      Button->LABEL      = GRAAL_SEGMENT_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_SEGMENT_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_SEGMENT_NAME_TABLE[ Index ][ 2 ];

      Button->X        = X1;
      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      if ( X1 == 1 )
      {
        X1 = 0; Y1 = Y1 + 1;
      }
      else
      {
        X1 = X1 + 1;
      }
    }
  }

  if ( X1 == 1 ) Y1 = Y1 + 1;

  for ( Index = 0; Index < 4; Index++ )
  {
    GraalModifySegmentButton[ MBK_MAX_LAYER + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifySegmentPanel.ROW = Y1;
  GraalModifySegmentDefaultValues[ 3 ] = Y1 * 25;


  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_VIA; Index++ )
  {
    if ( GRAAL_VIA_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyViaButton[ Index ];

      Button->LABEL      = GRAAL_VIA_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_VIA_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_VIA_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  for ( Index = 0; Index < 3; Index++ )
  {
    GraalModifyViaButton[ MBK_MAX_VIA + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifyViaPanel.ROW = Y1;
  GraalModifyViaDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_VIA; Index++ )
  {
    if ( GRAAL_BIGVIA_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyBigViaButton[ Index ];

      Button->LABEL      = GRAAL_BIGVIA_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_BIGVIA_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_BIGVIA_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  for ( Index = 0; Index < 3; Index++ )
  {
    GraalModifyBigViaButton[ MBK_MAX_VIA + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifyBigViaPanel.ROW = Y1;
  GraalModifyBigViaDefaultValues[ 3 ] = Y1 * 25;

  X1 = 0;
  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_LAYER; Index++ )
  {
    if ( GRAAL_TRANSISTOR_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyTransistorButton[ Index ];

      Button->LABEL      = GRAAL_TRANSISTOR_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_TRANSISTOR_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_TRANSISTOR_NAME_TABLE[ Index ][ 2 ];


      Button->X        = X1;
      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      if ( X1 == 1 )
      {
        X1 = 0; Y1 = Y1 + 1;
      }
      else
      {
        X1 = X1 + 1;
      }
    }
  }

  if ( X1 == 1 ) Y1 = Y1 + 1;

  for ( Index = 0; Index < 4; Index++ )
  {
    GraalModifyTransistorButton[ MBK_MAX_LAYER + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifyTransistorPanel.ROW = Y1;
  GraalModifyTransistorDefaultValues[ 3 ] = Y1 * 25;

  X1 = 0;
  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_LAYER; Index++ )
  {
    if ( GRAAL_CONNECTOR_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyConnectorButton[ Index ];

      Button->LABEL      = GRAAL_CONNECTOR_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_CONNECTOR_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_CONNECTOR_NAME_TABLE[ Index ][ 2 ];

      Button->X        = X1;
      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      if ( X1 == 1 )
      {
        X1 = 0; Y1 = Y1 + 1;
      }
      else
      {
        X1 = X1 + 1;
      }
    }
  }

  if ( X1 == 1 ) 
  {
    Y1 = Y1 + 1; X1 = 0;
  }

  GraalModifyConnectorButton[ MBK_MAX_LAYER ].Y = Y1;

  Y1 = Y1 + 1;

  for ( Index = 0; Index < MBK_MAX_ORIENT; Index++ )
  {
    if ( GRAAL_ORIENT_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyConnectorButton[ Index + MBK_MAX_LAYER + 1 ];

      Button->LABEL      = GRAAL_ORIENT_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_ORIENT_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_ORIENT_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  for ( Index = 0; Index < 3; Index++ )
  {
    GraalModifyConnectorButton[ MBK_MAX_LAYER  + 
                                MBK_MAX_ORIENT + Index + 1 ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifyConnectorPanel.ROW = Y1;
  GraalModifyConnectorDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_REFERENCE; Index++ )
  {
    if ( GRAAL_REFERENCE_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyReferenceButton[ Index ];

      Button->LABEL      = GRAAL_REFERENCE_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_REFERENCE_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_REFERENCE_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  for ( Index = 0; Index < 3; Index++ )
  {
    GraalModifyReferenceButton[ MBK_MAX_REFERENCE + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifyReferencePanel.ROW = Y1;
  GraalModifyReferenceDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_SYMMETRY; Index++ )
  {
    if ( GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalModifyInstanceButton[ Index ];

      Button->LABEL      = GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  for ( Index = 0; Index < 4; Index++ )
  {
    GraalModifyInstanceButton[ MBK_MAX_SYMMETRY + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalModifyInstancePanel.ROW = Y1;
  GraalModifyInstanceDefaultValues[ 3 ] = Y1 * 25;
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

  GraalEditUndo();

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

  GraalEditRedo();

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
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_COPY,
                         GraalPromptSelectWindowCopy );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_COPY,
                         GraalPromptSelectPointCopy );
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
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_MOVE,
                         GraalPromptSelectWindowMove );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_MOVE,
                         GraalPromptSelectPointMove );
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
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_DELETE,
                         GraalPromptSelectWindowDelete );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_DELETE,
                         GraalPromptSelectPointDelete );
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

  if ( ClientData )
  {
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_STRETCH,
                         GraalPromptSelectWindowStretch );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_STRETCH,
                         GraalPromptSelectPointStretch );
  }

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
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_MODIFY,
                         GraalPromptSelectWindowModify );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_MODIFY,
                         GraalPromptSelectPointModify );
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
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_IDENTIFY,
                         GraalPromptSelectWindowIdentify );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_IDENTIFY,
                         GraalPromptSelectPointIdentify );
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
    GraalExitPanel( &GraalWindowGlobalPanel );
  }
  else
  {
    GraalExitPanel( &GraalEditGlobalPanel );
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
|                    CallbackEditSearchConnector              |
|                                                             |
\------------------------------------------------------------*/

void CallbackEditSearchConnector( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalSearchConnectorDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSearchInstance               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackEditSearchInstance( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  GraalEnterDialog( &GraalSearchInstanceDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSearchReference              |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackEditSearchReference( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  GraalEnterDialog( &GraalSearchReferenceDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSearchSegment                |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackEditSearchSegment( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  GraalEnterDialog( &GraalSearchSegmentDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackEditSearchVia                    |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackEditSearchVia( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData; 
{
  rdsbegin();

  GraalEnterDialog( &GraalSearchViaDialog );

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

  GraalExitPanel( &GraalEditSearchPanel );

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

  GraalExitDialog();

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

  GraalSearchAbort = GRAAL_TRUE; 

  GraalExitDialog();

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

  GraalExitPanel( &GraalEditIdentifyPanel );

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalDisplayIdentifyMessage              |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplayIdentifyMessage( Message )

  char *Message;
{
  rdsbegin();

  XmTextSetString( GraalEditIdentifyButton[0].BUTTON, Message );

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
    GraalEditSelectRectangle( CallData->item_position - 1 );
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

  GraalExitPanel( &GraalEditSelectPanel );

  GraalExitDialog();

  GraalEditSelectAccept();

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

  GraalExitPanel( &GraalEditSelectPanel );

  GraalExitDialog();

  GraalEditSelectCancel();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  GraalDisplaySelectList                     |
|                                                             |
\------------------------------------------------------------*/

void GraalDisplaySelectList( Message )

  char *Message;
{
  XmString     Label;

  rdsbegin();

  if ( Message == (char *)NULL )
  {
    XmListDeleteAllItems( GraalEditSelectButton[ 0 ].BUTTON );
  }
  else
  {
    Label = XmStringCreateSimple( Message );
    XmListAddItem( GraalEditSelectButton[ 0 ].BUTTON , Label , 0 );
    XmStringFree( Label );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Modify Segment              |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifySegmentLayer               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentLayer( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  float MinWidth;
  int   NewLayer;

  rdsbegin();

  NewLayer = (long)ClientData;

  if ( ( GraalSegmentMName == (char *)0 ) &&
       ( ( NewLayer >= CALU1          ) &&
         ( NewLayer <= CALU9          ) ) )
  {
    NewLayer = GraalSegmentMLayer;
    GraalErrorMessage( GraalMainWindow, "Connector must have name !" );
  }

  GraalSegmentMLayer = NewLayer;
  MinWidth           = GRAAL_SEGMENT_VALUE_TABLE[ GraalSegmentMLayer ][0];

  if ( GraalSegmentMWidth < MinWidth ) GraalSegmentMWidth = MinWidth;

  GraalPromptModifySegment();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifySegmentWidth               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentWidth( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifySegmentWidthDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifySegmentName                |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifySegmentNameDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifySegmentApply               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifySegmentCancel              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifySegmentCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 Callback For Modify Transistor              |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyTransistorType              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorType( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  float MinWidth;

  rdsbegin();

  GraalTransistorMType  = (long)ClientData;
  MinWidth              = GRAAL_SEGMENT_VALUE_TABLE[ GraalTransistorMType ][0];

  if ( GraalTransistorMWidth < MinWidth ) GraalTransistorMWidth = MinWidth;

  GraalPromptModifyTransistor();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyTransistorWidth              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorWidth( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyTransistorWidthDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyTransistorName             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyTransistorNameDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyTransistorApply             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyTransistorCancel             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyTransistorCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   Callback For Modify Connector             |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyConnectorLayer             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorLayer( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  float MinWidth;

  rdsbegin();

  GraalConnectorMLayer = (long)ClientData;
  MinWidth             = GRAAL_SEGMENT_VALUE_TABLE[ GraalConnectorMLayer ][0];

  if ( GraalConnectorMWidth < MinWidth ) GraalConnectorMWidth = MinWidth;

  GraalPromptModifyConnector();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyConnectorOrient            |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorOrient( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalConnectorMOrient = (long)ClientData;

  GraalPromptModifyConnector();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyConnectorName              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyConnectorNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyConnectorWidth               |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackModifyConnectorWidth( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyConnectorWidthDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyConnectorApply               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyConnectorCancel              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyConnectorCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Modify Reference            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyReferenceType              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyReferenceType( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalReferenceMType = (long)ClientData;

  GraalPromptModifyReference();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyReferenceName              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyReferenceName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyReferenceNameDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyReferenceApply               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyReferenceApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyReferenceCancel              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyReferenceCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Modify Instance             |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     CallbackModifyInstanceSym               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceSym( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalInstanceMSym = (long)ClientData;

  GraalPromptModifyInstance();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyInstanceName               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceName( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyInstanceNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyInstanceModel              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceModel( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyInstanceModelDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                  CallbackModifyInstanceApply                |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyInstanceCancel              |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyInstanceCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Modify Via                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      CallbackModifyViaType                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyViaType( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalViaMType = (long)ClientData;

  GraalPromptModifyVia();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyViaName                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyViaName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyViaNameDialog );

  rdsend();
}
 
 
/*------------------------------------------------------------\
|                                                             |
|                      CallbackModifyViaApply                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyViaApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                      CallbackModifyViaCancel                |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyViaCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Modify BigVia               |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      CallbackModifyBigViaType               |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyBigViaType( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalViaMType = (long)ClientData;

  GraalPromptModifyBigVia();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackModifyBigViaName                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyBigViaName( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalModifyBigViaNameDialog );

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                   CallbackModifyBigViaApply                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyBigViaApply( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_FALSE;
  GraalModifyApply  = GRAAL_TRUE;

  GraalExitDialog();

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                      CallbackModifyBigViaCancel             |
|                                                             |
\------------------------------------------------------------*/

void CallbackModifyBigViaCancel( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalModifyLock   = GRAAL_FALSE;
  GraalModifyAbort  = GRAAL_TRUE;
  GraalModifyApply  = GRAAL_FALSE;

  GraalExitDialog();

  rdsend();
}
