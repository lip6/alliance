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
| File    :                  Panel.c                          |
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
# include "GMX.h"
# include "GRM.h"
# include "GMC.h"

# include "GMC_panel.h"
# include "GMC_dialog.h"
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
/*------------------------------------------------------------\
|                                                             |
|                         Create Panel                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Segment Panel                         |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateSegmentButton[ MBK_MAX_LAYER + 4 ] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentLayer,
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
             CallbackChangeSegmentWidth,
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
             CallbackChangeSegmentName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Simple",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeSegmentWire,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeSegmentClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalCreateSegmentPanel  =

         {
           "Segment",
           1,
           0,
           GRAAL_CREATE_SEGMENT_X,
           GRAAL_CREATE_SEGMENT_Y, 
           100,
           300,
           2,  
           ( MBK_MAX_LAYER + 4 ) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_LAYER + 4,
           GraalCreateSegmentButton
         };

   int GraalCreateSegmentDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_SEGMENT_X,
           GRAAL_CREATE_SEGMENT_Y, 
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Via Panel                          |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateViaButton[ MBK_MAX_VIA + 2 ] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaType,
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
             CallbackChangeViaName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeViaClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalCreateViaPanel  =

         {
           "Via",
           1,
           0,
           GRAAL_CREATE_VIA_X,
           GRAAL_CREATE_VIA_Y,
           100,
           300,
           1,
           MBK_MAX_VIA + 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_VIA + 2,
           GraalCreateViaButton
         };

   int GraalCreateViaDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_VIA_X,
           GRAAL_CREATE_VIA_Y,
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Big Via Panel                      |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateBigViaButton[ MBK_MAX_VIA + 2 ] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaType,
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
             CallbackChangeBigViaName,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeBigViaClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalCreateBigViaPanel  =

         {
           "BigVia",
           1,
           0,
           GRAAL_CREATE_BIGVIA_X,
           GRAAL_CREATE_BIGVIA_Y,
           100,
           275,
           1,
           MBK_MAX_VIA + 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_VIA + 2,
           GraalCreateBigViaButton
         };

   int GraalCreateBigViaDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_BIGVIA_X,
           GRAAL_CREATE_BIGVIA_Y,
           100, 275, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Transistor Panel                     |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateTransistorButton[ MBK_MAX_LAYER + 4 ] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorType,
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
             CallbackChangeTransistorWidth,
             (XtPointer) NULL,
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
             CallbackChangeTransistorName,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Simple",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeTransistorWire,
             (XtPointer)NULL,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeTransistorClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalCreateTransistorPanel  =

         {
           "Transistor",
           1,
           0,
           GRAAL_CREATE_TRANSISTOR_X,
           GRAAL_CREATE_TRANSISTOR_Y,
           100, 
           300,
           2,  
           ( MBK_MAX_LAYER + 4 ) / 2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_LAYER + 4,
           GraalCreateTransistorButton
         };

   int GraalCreateTransistorDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_TRANSISTOR_X,
           GRAAL_CREATE_TRANSISTOR_Y,
           100, 300, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                       Connector Panel                       |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateConnectorButton[MBK_MAX_LAYER+MBK_MAX_ORIENT+2] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorLayer,
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
             CallbackChangeConnectorWidth,
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
             CallbackChangeConnectorOrient,
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
             2, 1,
             CallbackChangeConnectorOrient,
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
             2, 1,
             CallbackChangeConnectorOrient,
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
             2, 1,
             CallbackChangeConnectorOrient,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             2, 1,
             CallbackChangeConnectorClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
         };

   GraalPanelItem GraalCreateConnectorPanel  =

         {
           "Connector",
           1,
           0,
           GRAAL_CREATE_CONNECTOR_X,
           GRAAL_CREATE_CONNECTOR_Y,
           100, 
           325,
           2,  
           (MBK_MAX_LAYER+MBK_MAX_ORIENT+2)/2,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_LAYER+MBK_MAX_ORIENT+2,
           GraalCreateConnectorButton
         };

   int GraalCreateConnectorDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_CONNECTOR_X,
           GRAAL_CREATE_CONNECTOR_Y,
           100, 325, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Reference Panel                      |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateReferenceButton[ MBK_MAX_REFERENCE + 1 ] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeReferenceType,
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
             CallbackChangeReferenceType,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeReferenceClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
        };

   GraalPanelItem GraalCreateReferencePanel  =

         {
           "Reference",
           1,
           0,
           GRAAL_CREATE_REFERENCE_X,
           GRAAL_CREATE_REFERENCE_Y,
           100, 
           75,
           1,  
           MBK_MAX_REFERENCE + 1,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_REFERENCE + 1,
           GraalCreateReferenceButton
         };

   int GraalCreateReferenceDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_REFERENCE_X,
           GRAAL_CREATE_REFERENCE_Y,
           100, 75, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                        Instance  Panel                      |
|                                                             |
\------------------------------------------------------------*/

   static GraalPanelButtonItem GraalCreateInstanceButton[ MBK_MAX_SYMMETRY + 1 ] =

         {
           {
             NULL,
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
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
             CallbackChangeInstanceSym,
             (XtPointer)0,
             (Widget)NULL
           }
           ,
           {
             "Close",
             NULL, NULL, 0, 0,
             NULL,
             NULL,
             0, 0,
             1, 1,
             CallbackChangeInstanceClose,
             (XtPointer)NULL,
             (Widget)NULL
           }
        };  

   GraalPanelItem GraalCreateInstancePanel  =

         {
           "Instance",
           1,
           0,
           GRAAL_CREATE_INSTANCE_X,
           GRAAL_CREATE_INSTANCE_Y,
           100, 
           225,
           1,  
           MBK_MAX_SYMMETRY + 1,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           (Widget)NULL,
           MBK_MAX_SYMMETRY + 1,
           GraalCreateInstanceButton
         };

   int GraalCreateInstanceDefaultValues[ 5 ] =

         {
           GRAAL_CREATE_INSTANCE_X,
           GRAAL_CREATE_INSTANCE_Y,
           100, 225, 0
         };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    GraalBuildPanelCreate                    |
|                                                             |
\------------------------------------------------------------*/

void GraalBuildPanelCreate()

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
      Button = &GraalCreateSegmentButton[ Index ];

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
    GraalCreateSegmentButton[ MBK_MAX_LAYER + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalCreateSegmentPanel.ROW = Y1;
  GraalCreateSegmentDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_VIA; Index++ )
  {
    if ( GRAAL_VIA_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateViaButton[ Index ];

      Button->LABEL      = GRAAL_VIA_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_VIA_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_VIA_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  GraalCreateViaButton[ MBK_MAX_VIA     ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateViaButton[ MBK_MAX_VIA + 1 ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateViaPanel.ROW = Y1;
  GraalCreateViaDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_VIA; Index++ )
  {
    if ( GRAAL_BIGVIA_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateBigViaButton[ Index ];

      Button->LABEL      = GRAAL_BIGVIA_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_BIGVIA_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_BIGVIA_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  GraalCreateBigViaButton[ MBK_MAX_VIA     ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateBigViaButton[ MBK_MAX_VIA + 1 ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateBigViaPanel.ROW = Y1;
  GraalCreateBigViaDefaultValues[ 3 ] = Y1 * 25;

  X1 = 0;
  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_LAYER; Index++ )
  {
    if ( GRAAL_TRANSISTOR_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateTransistorButton[ Index ];

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
    GraalCreateTransistorButton[ MBK_MAX_LAYER + Index ].Y = Y1;
    Y1 = Y1 + 1;
  }

  GraalCreateTransistorPanel.ROW = Y1;
  GraalCreateTransistorDefaultValues[ 3 ] = Y1 * 25;

  X1 = 0;
  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_LAYER; Index++ )
  {
    if ( GRAAL_CONNECTOR_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateConnectorButton[ Index ];

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

  GraalCreateConnectorButton[ MBK_MAX_LAYER ].Y = Y1;

  Y1 = Y1 + 1;

  for ( Index = 0; Index < MBK_MAX_ORIENT; Index++ )
  {
    if ( GRAAL_ORIENT_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateConnectorButton[ Index + MBK_MAX_LAYER + 1 ];

      Button->LABEL      = GRAAL_ORIENT_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_ORIENT_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_ORIENT_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  GraalCreateConnectorButton[ MBK_MAX_LAYER + MBK_MAX_ORIENT + 1 ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateConnectorPanel.ROW = Y1;
  GraalCreateConnectorDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_REFERENCE; Index++ )
  {
    if ( GRAAL_REFERENCE_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateReferenceButton[ Index ];

      Button->LABEL      = GRAAL_REFERENCE_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_REFERENCE_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_REFERENCE_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  GraalCreateReferenceButton[ MBK_MAX_REFERENCE ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateReferencePanel.ROW = Y1;
  GraalCreateReferenceDefaultValues[ 3 ] = Y1 * 25;

  Y1 = 0;

  for ( Index = 0; Index < MBK_MAX_SYMMETRY; Index++ )
  {
    if ( GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 0 ] != (char *)NULL )
    {
      Button = &GraalCreateInstanceButton[ Index ];

      Button->LABEL      = GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 0 ];
      Button->FOREGROUND = GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 1 ];
      Button->BACKGROUND = GRAAL_SYMMETRY_NAME_TABLE[ Index ][ 2 ];

      Button->Y        = Y1;
      Button->CALLDATA = (XtPointer)Index;

      Y1 = Y1 + 1;
    }
  }

  GraalCreateInstanceButton[ MBK_MAX_SYMMETRY ].Y = Y1;
  Y1 = Y1 + 1;

  GraalCreateInstancePanel.ROW = Y1;
  GraalCreateInstanceDefaultValues[ 3 ] = Y1 * 25;
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Segment                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeSegmentLayer                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentLayer( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalSegmentLayer = (long)ClientData;
  GraalSegmentWidth = GRAAL_SEGMENT_VALUE_TABLE[ (int)GraalSegmentLayer ][0];

  if ( GraalSegmentWire == GRAAL_FALSE )
  {
    GraalChangeEditMode( GRAAL_CREATE_SEGMENT,
                         GraalPromptCreateSegment );
  }
  else
  {
    GraalChangeEditMode( GRAAL_CREATE_SEGMENT_WIRE,
                         GraalPromptCreateSegment );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalChangeSegmentLayer                      |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeSegmentLayer( LayerUp )

  int LayerUp;
{
  int  MbkLayer;
  int  Step;

  if ( LayerUp ) Step =  1;
  else           Step = -1;

  MbkLayer = GraalSegmentLayer;

  do
  {
    MbkLayer += Step;

    if ( MbkLayer >= MBK_MAX_LAYER ) MbkLayer = 0;
    else
    if ( MbkLayer <  0             ) MbkLayer = MBK_MAX_LAYER - 1;
  }
  while ( GRAAL_SEGMENT_NAME_TABLE[ MbkLayer ][ 0 ] == (char *)0 );

  GraalSegmentLayer = MbkLayer;
  GraalSegmentWidth = GRAAL_SEGMENT_VALUE_TABLE[ MbkLayer ][0];
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeSegmentWire                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentWire( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Label;

  rdsbegin();

  GraalSegmentWire = ! GraalSegmentWire;

  if ( GraalSegmentWire == GRAAL_FALSE )
  {
    Label = XmStringCreateSimple( "Simple" );

    GraalChangeEditMode( GRAAL_CREATE_SEGMENT,
                         GraalPromptCreateSegment );
  }
  else
  {   
    Label = XmStringCreateSimple( "Wire" );

    GraalChangeEditMode( GRAAL_CREATE_SEGMENT_WIRE,
                         GraalPromptCreateSegment );
  }

  XtVaSetValues( MyWidget, XmNlabelString, Label,
                           XmNresizable  , False,
                           NULL );
 
  XmStringFree( Label );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeSegmentWidth                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentWidth( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeSegmentWidthDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeSegmentName                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentName( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeSegmentNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeSegmentClose                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeSegmentClose( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateSegmentPanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Via                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeViaType                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeViaType( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalViaType = (long)ClientData;

  GraalChangeEditMode( GRAAL_CREATE_VIA,
                       GraalPromptCreateVia );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalChangeViaType                   |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeViaType( TypeUp )

  int TypeUp;
{
  int MbkType;
  int Step;

  if ( TypeUp ) Step =  1;
  else          Step = -1;

  MbkType = GraalViaType;

  do
  {
    MbkType += Step;

    if ( MbkType >= MBK_MAX_VIA ) MbkType = 0;
    else
    if ( MbkType <  0           ) MbkType = MBK_MAX_VIA - 1;
  }
  while ( GRAAL_VIA_NAME_TABLE[ MbkType ][ 0 ] == (char *)0 );

  GraalViaType = MbkType;
}

/*------------------------------------------------------------\
|                                                             |
|                  CallbackChangeViaName                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeViaName( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeViaNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeViaClose                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeViaClose( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateViaPanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     Callback For Big Via                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeBigViaType                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeBigViaType( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalBigViaType = (long)ClientData;

  GraalChangeEditMode( GRAAL_CREATE_BIGVIA,
                       GraalPromptCreateBigVia );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalChangeBigViaType                  |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeBigViaType( TypeUp )

  int TypeUp;
{
  int MbkType;
  int Step;

  if ( TypeUp ) Step =  1;
  else          Step = -1;

  MbkType = GraalBigViaType;

  do
  {
    MbkType += Step;

    if ( MbkType >= MBK_MAX_VIA ) MbkType = 0;
    else
    if ( MbkType <  0           ) MbkType = MBK_MAX_VIA - 1;
  }
  while ( GRAAL_BIGVIA_NAME_TABLE[ MbkType ][ 0 ] == (char *)0 );

  GraalBigViaType = MbkType;
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeBigViaName                     |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeBigViaName( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeBigViaNameDialog );

  rdsend();
}


/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeBigViaClose                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeBigViaClose( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateBigViaPanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Callback For Transistor                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeTransistorType             |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeTransistorType( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalTransistorType  = (long)ClientData;
  GraalTransistorWidth = GRAAL_SEGMENT_VALUE_TABLE[ (int)GraalTransistorType ][0];

  if ( GraalTransistorWire == GRAAL_FALSE )
  {
    GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR,
                         GraalPromptCreateTransistor );
  }
  else
  {   
    GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR_WIRE,
                         GraalPromptCreateTransistor );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalChangeTransistorType                    |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeTransistorType( TypeUp )

  int TypeUp;
{
  int  MbkType;
  int  Step;

  if ( TypeUp ) Step =  1;
  else          Step = -1;

  MbkType = GraalTransistorType;

  do
  {
    MbkType += Step;

    if ( MbkType >= MBK_MAX_LAYER ) MbkType = 0;
    else
    if ( MbkType <  0             ) MbkType = MBK_MAX_LAYER - 1;
  }
  while ( GRAAL_TRANSISTOR_NAME_TABLE[ MbkType ][ 0 ] == (char *)0 );

  GraalTransistorType  = MbkType;
  GraalTransistorWidth = GRAAL_SEGMENT_VALUE_TABLE[ MbkType ][0];
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeTransistorWidth                |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeTransistorWidth( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeTransistorWidthDialog );

  rdsend();
}    

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeTransistorName                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeTransistorName( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeTransistorNameDialog );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                CallbackChangeTransistorWire                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeTransistorWire( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  XmString Label;

  rdsbegin();

  GraalTransistorWire = ! GraalTransistorWire;

  if ( GraalTransistorWire == GRAAL_FALSE )
  {
    Label = XmStringCreateSimple( "Simple" );

    GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR,
                         GraalPromptCreateTransistor );
  }
  else
  {   
    Label = XmStringCreateSimple( "Wire" );

    GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR_WIRE,
                         GraalPromptCreateTransistor );
  }

  XtVaSetValues( MyWidget, XmNlabelString, Label,
                           XmNresizable  , False,
                           NULL );

  XmStringFree( Label );

  rdsend();
}


/*------------------------------------------------------------\
|                                                             |
|                   CallbackChangeTransistorClose             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeTransistorClose( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateTransistorPanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Connector                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeConnectorLayer             |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeConnectorLayer( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalConnectorLayer = (long)ClientData;
  GraalConnectorWidth = GRAAL_SEGMENT_VALUE_TABLE[ (int)GraalConnectorLayer ][0];

  GraalChangeEditMode( GRAAL_CREATE_CONNECTOR,
                       GraalPromptCreateConnector );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                GraalChangeConnectorLayer                    |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeConnectorLayer( LayerUp )

  int LayerUp;
{
  int  MbkLayer;
  int  Step;

  if ( LayerUp ) Step =  1;
  else           Step = -1;

  MbkLayer = GraalConnectorLayer;

  do
  {
    MbkLayer += Step;

    if ( MbkLayer >= MBK_MAX_LAYER ) MbkLayer = 0; 
    else
    if ( MbkLayer <  0             ) MbkLayer = MBK_MAX_LAYER - 1;
  }
  while ( GRAAL_CONNECTOR_NAME_TABLE[ MbkLayer ][ 0 ] == (char *)0 );

  GraalConnectorLayer = MbkLayer;
  GraalConnectorWidth = GRAAL_SEGMENT_VALUE_TABLE[ MbkLayer ][0];
}

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeConnectorWidth                |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeConnectorWidth( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalEnterDialog( &GraalChangeConnectorWidthDialog );

  rdsend();
}    

/*------------------------------------------------------------\
|                                                             |
|                 CallbackChangeConnectorOrient               |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeConnectorOrient( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalConnectorOrient = (long)ClientData;

  GraalChangeEditMode( GRAAL_CREATE_CONNECTOR,
                       GraalPromptCreateConnector );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeConnectorClose             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeConnectorClose( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateConnectorPanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Reference                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeReferenceType              |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeReferenceType( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalReferenceType = (long)ClientData;

  GraalChangeEditMode( GRAAL_CREATE_REFERENCE,
                       GraalPromptCreateReference );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   GraalChangeReferenceType                  |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeReferenceType( TypeUp )

  int TypeUp;
{
  int MbkType;
  int Step;

  if ( TypeUp ) Step =  1;
  else          Step = -1;

  MbkType = GraalReferenceType;

  do
  {
    MbkType += Step;

    if ( MbkType >= MBK_MAX_REFERENCE ) MbkType = 0;
    else
    if ( MbkType <  0                 ) MbkType = MBK_MAX_REFERENCE - 1;
  }
  while ( GRAAL_REFERENCE_NAME_TABLE[ MbkType ][ 0 ] == (char *)0 );

  GraalReferenceType = MbkType;
}

/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeReferenceClose             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeReferenceClose( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateReferencePanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Callback For Instance                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    CallbackChangeInstanceSym                |
|                                                             |
\------------------------------------------------------------*/

void CallbackChangeInstanceSym( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalInstanceSym = (long)ClientData;

  GraalChangeEditMode( GRAAL_CREATE_INSTANCE,
                       GraalPromptCreateInstance );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackChangeInstanceClose             |
|                                                             |
\------------------------------------------------------------*/
 
void CallbackChangeInstanceClose( MyWidget, ClientData, CallData )
 
     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalExitPanel( &GraalCreateInstancePanel );

  GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                       GraalPromptEditMeasure );

  rdsend();
}
