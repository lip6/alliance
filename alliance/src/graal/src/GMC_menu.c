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
| Tool    :                   GRAAL                           |
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
# include "GRM.h"
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

   GraalMenuItem GraalCreateMenu[] =

         {
           {
             "Instance",
             'I',
             "Ctrl<Key>I",
             "Ctrl I",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateInstance,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
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
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Segment",
             'S',
             "Ctrl<Key>E",
             "Ctrl E",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateSegment,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Via",
             'V',
             "Ctrl<Key>V",
             "Ctrl V",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateVia,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Big Via",
             'B',
             "Ctrl<Key>B",
             "Ctrl B",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateBigVia,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Connector     ",
             'o',
             "Ctrl<Key>N",
             "Ctrl N",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateConnector,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Transistor       ",
             'T',
             "Ctrl<Key>T",
             "Ctrl T",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateTransistor,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL
           }
           ,
           {
             "Reference       ",
             'R',
             "Ctrl<Key>F",
             "Ctrl F",
             &xmPushButtonGadgetClass,
             False,
             False,
             False,
             CallbackCreateReference,
             (XtPointer)NULL,
             (Widget)NULL,
             (Widget)NULL,
             (GraalMenuItem *)NULL,
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
|                    CallbackCreateSegment                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateSegment( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( GraalSegmentWidth == -1.0 )
  {
    GraalSegmentWidth = GRAAL_SEGMENT_VALUE_TABLE[ GraalSegmentLayer ][0];
  }

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

  GraalEnterPanel( &GraalCreateSegmentPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     CallbackCreateReference                 |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateReference( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_CREATE_REFERENCE,
                       GraalPromptCreateReference );

  GraalEnterPanel( &GraalCreateReferencePanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackCreateVia                         |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateVia( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_CREATE_VIA,
                       GraalPromptCreateVia );

  GraalEnterPanel( &GraalCreateViaPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackCreateBigVia                      |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateBigVia( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_CREATE_BIGVIA,
                       GraalPromptCreateBigVia );

  GraalEnterPanel( &GraalCreateBigViaPanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackCreateInstance                    |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateInstance( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  GraalChangeEditMode( GRAAL_CREATE_INSTANCE,
                       GraalPromptCreateInstance );

  GraalEnterPanel( &GraalCreateInstancePanel );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackCreateConnector                   |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateConnector( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( ( GraalFigureMbk == (phfig_list *)NULL ) || 
       ( ( GraalFigureMbk->XAB1 == 0 ) &&     
         ( GraalFigureMbk->XAB2 == 0 ) &&     
         ( GraalFigureMbk->YAB1 == 0 ) &&     
         ( GraalFigureMbk->YAB2 == 0 ) ) )
  {
    GraalErrorMessage( GraalMainWindow, "No abutment box, can't create connector !" );
  }
  else
  {
    if ( GraalConnectorWidth == -1.0 )
    {
      GraalConnectorWidth = GRAAL_SEGMENT_VALUE_TABLE[ GraalConnectorLayer ][0];
    }

    GraalChangeEditMode( GRAAL_CREATE_CONNECTOR,
                         GraalPromptCreateConnector );

    GraalEnterPanel( &GraalCreateConnectorPanel );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   CallbackCreateTransistor                  |
|                                                             |
\------------------------------------------------------------*/

void CallbackCreateTransistor( MyWidget, ClientData, CallData )

     Widget  MyWidget;
     caddr_t ClientData;
     caddr_t CallData;
{
  rdsbegin();

  if ( GraalTransistorWidth == -1.0 )
  {
    GraalTransistorWidth = GRAAL_SEGMENT_VALUE_TABLE[ GraalTransistorType ][0];
  }

  GraalChangeEditMode( GRAAL_CREATE_TRANSISTOR,
                       GraalPromptCreateTransistor );
 
  GraalEnterPanel ( &GraalCreateTransistorPanel );

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

  if ( ( GraalFigureMbk != (phfig_list *)NULL ) &&
       ( ( GraalFigureMbk->XAB1 != 0 ) ||
         ( GraalFigureMbk->XAB2 != 0 ) ||
         ( GraalFigureMbk->YAB1 != 0 ) ||
         ( GraalFigureMbk->YAB2 != 0 ) ) )
  {
    GraalErrorMessage( GraalMainWindow, "There is already one abutment box !");
  }
  else
  {
    GraalChangeEditMode( GRAAL_CREATE_ABUTMENTBOX,
                         GraalPromptCreateAbutmentBox );
  }

  rdsend();
}
