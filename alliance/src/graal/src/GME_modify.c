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
| File    :                  Modify.c                         |
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
# include "rfm.h"
# include "GSB.h"
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GME.h"
# include "GMT.h"
# include "GMV.h"

# include "GME_modify.h"
# include "GME_panel.h"
# include "GME_edit.h"
# include "GME_message.h"
# include "GME_dialog.h"

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

  char GraalModifyAbort  = 0;
  char GraalModifyLock   = 0;
  char GraalModifyApply  = 0;

/*------------------------------------------------------------\
|                                                             |
|                        Segment & Wire                       |
|                                                             |
\------------------------------------------------------------*/

  char  GraalSegmentMLayer;
  float GraalSegmentMWidth;
  char *GraalSegmentMName;

/*------------------------------------------------------------\
|                                                             |
|                              Via                            |
|                                                             |
\------------------------------------------------------------*/

  char  GraalViaMType;
  char *GraalViaMName;

/*------------------------------------------------------------\
|                                                             |
|                           Transistor                        |
|                                                             |
\------------------------------------------------------------*/

  char  GraalTransistorMType;
  char *GraalTransistorMName;
  float GraalTransistorMWidth;

/*------------------------------------------------------------\
|                                                             |
|                           Connector                         |
|                                                             |
\------------------------------------------------------------*/

  char  GraalConnectorMLayer; 
  float GraalConnectorMWidth; 
  char *GraalConnectorMName;  
  char  GraalConnectorMOrient;

  char  GraalSaveConnectorMOrient;

/*------------------------------------------------------------\
|                                                             |
|                           Reference                         |
|                                                             |
\------------------------------------------------------------*/

  char  GraalReferenceMType;  
  char *GraalReferenceMName;  

  char  GraalSaveReferenceMType;

/*------------------------------------------------------------\
|                                                             |
|                           Instance                          |
|                                                             |
\------------------------------------------------------------*/

 char  GraalInstanceMSym;
 char *GraalInstanceMName;
 char *GraalInstanceMModel;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Changed Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalTransistorChanged                  |
|                                                             |
\------------------------------------------------------------*/

char GraalTransistorChanged( Transistor )

  phseg_list *Transistor; 
{
  return 

   ( (   Transistor->NAME                     != GraalTransistorMName  ) ||
     (   Transistor->LAYER                    != GraalTransistorMType  ) ||
     ( ( Transistor->WIDTH / (float)SCALE_X ) != GraalTransistorMWidth ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalSegmentChanged                   |
|                                                             |
\------------------------------------------------------------*/

char GraalSegmentChanged( Segment )

  phseg_list *Segment;
{
  return

   ( (   Segment->NAME                     != GraalSegmentMName  ) ||
     (   Segment->LAYER                    != GraalSegmentMLayer ) ||
     ( ( Segment->WIDTH / (float)SCALE_X ) != GraalSegmentMWidth ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalViaChanged                      |
|                                                             |
\------------------------------------------------------------*/

char GraalViaChanged( Via )

  phseg_list *Via;
{
  return ( ( Via->TYPE != GraalViaMType ) ||
           ( Via->NAME != GraalViaMName ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalConnectorChanged                   |
|                                                             |
\------------------------------------------------------------*/

char GraalConnectorChanged( Connector )

  phcon_list *Connector;
{
  return

   ( (   Connector->NAME                     != GraalConnectorMName   ) ||
     (   Connector->LAYER                    != GraalConnectorMLayer  ) ||
     ( ( Connector->WIDTH / (float)SCALE_X ) != GraalConnectorMWidth  ) ||
     (  GraalSaveConnectorMOrient            != GraalConnectorMOrient ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalReferenceChanged                   |
|                                                             |
\------------------------------------------------------------*/

char GraalReferenceChanged( Reference )

  phref_list *Reference;
{
  return

   ( (   Reference->NAME          != GraalReferenceMName ) ||
     (   GraalSaveReferenceMType  != GraalReferenceMType ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalInstanceChanged                   |
|                                                             |
\------------------------------------------------------------*/

char GraalInstanceChanged( Instance )

  phins_list *Instance;
{
  return

   ( ( Instance->INSNAME != GraalInstanceMName  ) ||
     ( Instance->TRANSF  != GraalInstanceMSym   ) ||
     ( Instance->FIGNAME != GraalInstanceMModel ) );
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalEditModify                       |
|                                                             |
\------------------------------------------------------------*/

void GraalEditModify()

{
  graalselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  rdsins_list *Instance;
  phins_list  *SaveInstance;
  void        *Pointer;
  void        *Element;
  char         FirstUndo;
  char         MbkLayer;
  char         MbkOrient = '-';

  rdsbegin();

  FirstUndo = 1;

  GraalModifyAbort = 0;
  GraalModifyApply = 0;
  GraalModifyLock  = 0;
  
  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( IsRdsSegment( Rectangle ) )
    {
      Pointer = GRAAL_MBK( Rectangle );

      MbkLayer = ((phseg_list *)Pointer)->LAYER;

      if ( GET_LYNX_TRANSISTOR_TYPE( MbkLayer ) != RDS_LYNX_TRANSISTOR_EMPTY )
      {
        GraalAcceptRectangle( Rectangle );
        GraalDisplayRectangle( Rectangle );

        GraalTransistorMWidth = (float)( ((phseg_list *)Pointer)->WIDTH / (float)SCALE_X );
        GraalTransistorMType  = ((phseg_list *)Pointer)->LAYER;
        GraalTransistorMName  = ((phseg_list *)Pointer)->NAME;

        GraalPromptModifyTransistor();
        GraalEnterPanel( &GraalModifyTransistorPanel );

        GraalModifyLock = GRAAL_TRUE;

        while ( GraalModifyLock )
        {
          GraalLimitedLoop( GraalModifyTransistorPanel.PANEL );
        }

        GraalRejectRectangle( Rectangle );
        GraalDisplayRectangle( Rectangle );

        if ( GraalModifyAbort ) break;

        if ( GraalModifyApply )
        {
          if ( ! GraalTransistorChanged( (phseg_list *)Pointer ) ) 

            continue; 

          if ( FirstUndo )
          {
            GraalAddUndo();

            FirstUndo = 0;
          }
 
          GraalDeleteRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );

          Element = (void *)

            addphseg( GraalFigureMbk,
                      GraalTransistorMType ,
                      (long)( GraalTransistorMWidth * SCALE_X ),
                      ((phseg_list *)Pointer)->X1,
                      ((phseg_list *)Pointer)->Y1,
                      ((phseg_list *)Pointer)->X2,
                      ((phseg_list *)Pointer)->Y2,
                      GraalTransistorMName );

          ((phseg_list *)Element)->TYPE = ((phseg_list *)Pointer)->TYPE;
          ((phseg_list *)Element)->USER = (void *)(&GraalFigureMbk->PHSEG);

          if ( ((phseg_list *)Element)->NEXT != (phseg_list *)NULL )
          {
            ((phseg_list *)Element)->NEXT->USER = (void *)(&((phseg_list *)Element)->NEXT);
          }

          NewRec = GraalAddSegment( ((phseg_list *)Element) );
          GraalAddUndoRec( NewRec );

          GraalDisplayRectangle( NewRec );
        }
      }
    }
  }

  GraalExitPanel( &GraalModifyTransistorPanel );

  if ( ! GraalModifyAbort )

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    Pointer = GRAAL_MBK( Rectangle );

    if ( IsRdsSegment( Rectangle ) )
    {
      if ( ( ((phseg_list *)Pointer)->LAYER != NTRANS ) &&
           ( ((phseg_list *)Pointer)->LAYER != PTRANS ) )
      {
        GraalAcceptRectangle( Rectangle );
        GraalDisplayRectangle( Rectangle );

        GraalSegmentMWidth = (float)( ((phseg_list *)Pointer)->WIDTH / (float)SCALE_X );
        GraalSegmentMLayer = ((phseg_list *)Pointer)->LAYER;
        GraalSegmentMName  = ((phseg_list *)Pointer)->NAME;

        GraalPromptModifySegment();
        GraalEnterPanel( &GraalModifySegmentPanel );

        GraalModifyLock = GRAAL_TRUE;

        while ( GraalModifyLock )
        {
          GraalLimitedLoop( GraalModifySegmentPanel.PANEL );
        }

        GraalRejectRectangle( Rectangle );
        GraalDisplayRectangle( Rectangle );
  
        if ( GraalModifyAbort ) break;

        if ( GraalModifyApply )
        {
          if ( ! GraalSegmentChanged( (phseg_list *)Pointer ) ) 

            continue; 

          if ( FirstUndo )
          {
            GraalAddUndo();

            FirstUndo = 0;
          }
 
          GraalDeleteRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );

          Element = (void *)

            addphseg( GraalFigureMbk,
                      GraalSegmentMLayer,
                      (long)( GraalSegmentMWidth * SCALE_X ),
                      ((phseg_list *)Pointer)->X1,
                      ((phseg_list *)Pointer)->Y1,
                      ((phseg_list *)Pointer)->X2,
                      ((phseg_list *)Pointer)->Y2,
                      GraalSegmentMName );

          ((phseg_list *)Element)->TYPE = ((phseg_list *)Pointer)->TYPE;
          ((phseg_list *)Element)->USER = (void *)(&GraalFigureMbk->PHSEG);
  
          if ( ((phseg_list *)Element)->NEXT != (phseg_list *)NULL )
          {
            ((phseg_list *)Element)->NEXT->USER = (void *)(&((phseg_list *)Element)->NEXT);
          }

          NewRec = GraalAddSegment( ((phseg_list *)Element) );
          GraalAddUndoRec( NewRec );

          GraalDisplayRectangle( NewRec );
        }
      }
    }
  }

  GraalExitPanel( &GraalModifySegmentPanel );

  if ( ! GraalModifyAbort )

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( IsRdsVia( Rectangle ) )
    {
      GraalAcceptRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      if ( ( ((phvia_list *)Pointer)->DX == 0 ) &&
           ( ((phvia_list *)Pointer)->DY == 0 ) )
      {
        GraalViaMType = ((phvia_list *)Pointer)->TYPE;
        GraalViaMName = ((phvia_list *)Pointer)->NAME;

        GraalPromptModifyVia();
        GraalEnterPanel( &GraalModifyViaPanel );

        GraalModifyLock = GRAAL_TRUE;

        while ( GraalModifyLock )
        {
          GraalLimitedLoop( GraalModifyViaPanel.PANEL );
        }
      }
      else
      {
        GraalViaMType = ((phvia_list *)Pointer)->TYPE;
        GraalViaMName = ((phvia_list *)Pointer)->NAME;

        GraalPromptModifyBigVia();
        GraalEnterPanel( &GraalModifyBigViaPanel );

        GraalModifyLock = GRAAL_TRUE;

        while ( GraalModifyLock )
        {
          GraalLimitedLoop( GraalModifyBigViaPanel.PANEL );
        }
      }

      GraalRejectRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );

      if ( GraalModifyAbort ) break;

      if ( GraalModifyApply )
      {
        if ( ! GraalViaChanged( (phvia_list *)Pointer ) )

          continue;

        if ( FirstUndo )
        {
          GraalAddUndo();

          FirstUndo = 0;
        }
 
        GraalDeleteRectangle( Rectangle );
        GraalAddUndoRec( Rectangle );

        Element = (void *)

           addphvia( GraalFigureMbk,
                     GraalViaMType, 
                     ((phvia_list *)Pointer)->XVIA,
                     ((phvia_list *)Pointer)->YVIA,
                     ((phvia_list *)Pointer)->DX,
                     ((phvia_list *)Pointer)->DY, GraalViaMName );
 
        ((phvia_list *)Element)->USER = (void *)(&GraalFigureMbk->PHVIA);
 
        if ( ((phvia_list *)Element)->NEXT != (phvia_list *)NULL )
        {
          ((phvia_list *)Element)->NEXT->USER = (void *)(&((phvia_list *)Element)->NEXT);
        }
 
        NewRec = GraalAddVia( ((phvia_list *)Element) );
        GraalAddUndoRec( NewRec );

        GraalDisplayRectangle( NewRec );
      }
    }
  }

  GraalExitPanel( &GraalModifyViaPanel    );
  GraalExitPanel( &GraalModifyBigViaPanel );

  if ( ! GraalModifyAbort )

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( IsRdsReference( Rectangle ) )
    {
      GraalAcceptRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      GraalReferenceMName = ((phref_list *)Pointer)->NAME;
      GraalReferenceMType = 
 
         (strcmp( ((phref_list *)Pointer)->FIGNAME, "ref_ref" )) ?
          MBK_REF_CON : MBK_REF_REF;

      GraalSaveReferenceMType = GraalReferenceMType;

      GraalPromptModifyReference();
      GraalEnterPanel( &GraalModifyReferencePanel );

      GraalModifyLock = GRAAL_TRUE;

      while ( GraalModifyLock )
      {
        GraalLimitedLoop( GraalModifyReferencePanel.PANEL ); 
      }

      GraalRejectRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );

      if ( GraalModifyAbort ) break;

      if ( GraalModifyApply )
      {
        if ( ! GraalReferenceChanged( (phref_list *)Pointer ) )

         continue;

        if ( FirstUndo )
        {
          GraalAddUndo();

          FirstUndo = 0;
        }
 
        GraalDeleteRectangle( Rectangle );
        GraalAddUndoRec( Rectangle );

        Element = (void *)

           addphref( GraalFigureMbk,
                     (GraalReferenceMType == MBK_REF_REF) ? "ref_ref":"ref_con" ,
                     GraalReferenceMName,
                     ((phref_list *)Pointer)->XREF,
                     ((phref_list *)Pointer)->YREF );
 
        ((phref_list *)Element)->USER = (void *)(&GraalFigureMbk->PHREF);
 
        if ( ((phref_list *)Element)->NEXT != (phref_list *)NULL )
        {
          ((phref_list *)Element)->NEXT->USER = (void *)(&((phref_list *)Element)->NEXT);
        }
 
        NewRec = GraalAddReference( ((phref_list *)Element) );
        GraalAddUndoRec( NewRec );

        GraalDisplayRectangle( NewRec );
      }
    }
  }

  GraalExitPanel( &GraalModifyReferencePanel );

  if ( ! GraalModifyAbort )

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( IsRdsConnector( Rectangle ) )
    {
      GraalAcceptRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );
 
      Pointer = GRAAL_MBK( Rectangle );

      GraalConnectorMName   = ((phcon_list *)Pointer)->NAME;
      GraalConnectorMLayer  = ((phcon_list *)Pointer)->LAYER;
      GraalConnectorMWidth  = (float)( ((phcon_list *)Pointer)->WIDTH / (float)SCALE_X );

      switch( ((phcon_list *)Pointer)->ORIENT )
      {
        case NORTH : GraalConnectorMOrient = GRAAL_NORTH; break;
        case SOUTH : GraalConnectorMOrient = GRAAL_SOUTH; break;
        case EAST  : GraalConnectorMOrient = GRAAL_EAST;  break;
        case WEST  : GraalConnectorMOrient = GRAAL_WEST;  break;
      }

      GraalSaveConnectorMOrient = GraalConnectorMOrient;

      GraalPromptModifyConnector();
      GraalEnterPanel( &GraalModifyConnectorPanel );

      GraalModifyLock = GRAAL_TRUE;

      while ( GraalModifyLock )
      {
        GraalLimitedLoop( GraalModifyConnectorPanel.PANEL );
      }

      GraalRejectRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );

      if ( GraalModifyAbort ) break;

      if ( GraalModifyApply )
      {
        if ( ! GraalConnectorChanged( (phcon_list *)Pointer ) )

          continue;

        if ( FirstUndo )
        {
          GraalAddUndo();

          FirstUndo = 0;
        }

        switch( GraalConnectorMOrient )
        {
          case GRAAL_NORTH : MbkOrient = NORTH; break;
          case GRAAL_SOUTH : MbkOrient = SOUTH; break;
          case GRAAL_EAST  : MbkOrient = EAST;  break;
          case GRAAL_WEST  : MbkOrient = WEST;  break;
        }
 
        GraalDeleteRectangle( Rectangle );
        GraalAddUndoRec( Rectangle );

        Element = (void *)

         addphcon( GraalFigureMbk, 
                   MbkOrient,
                   GraalConnectorMName,
                   ((phcon_list *)Pointer)->XCON,
                   ((phcon_list *)Pointer)->YCON,
                   GraalConnectorMLayer,
                   (long)( GraalConnectorMWidth * SCALE_X ) );
 
        ((phcon_list *)Element)->USER = (void *)(&GraalFigureMbk->PHCON);
 
        if ( ((phcon_list *)Element)->NEXT != (phcon_list *)NULL )
        {
          ((phcon_list *)Element)->NEXT->USER = (void *)(&((phcon_list *)Element)->NEXT);
        }
 
        NewRec = GraalAddConnector( ((phcon_list *)Element) );
        GraalAddUndoRec( NewRec );

        GraalDisplayRectangle( NewRec );
      }
    }
  }

  GraalExitPanel( &GraalModifyConnectorPanel );

  if ( ! GraalModifyAbort )

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( IsRdsInstance( Rectangle ) )
    {
      GraalAcceptRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );
 
      Pointer = GRAAL_MBK( Rectangle );

      GraalInstanceMName  = ((phins_list *)Pointer)->INSNAME;
      GraalInstanceMModel = ((phins_list *)Pointer)->FIGNAME;
      GraalInstanceMSym   = ((phins_list *)Pointer)->TRANSF;

      GraalPromptModifyInstance();
      GraalEnterPanel( &GraalModifyInstancePanel );

      GraalModifyLock = GRAAL_TRUE;

      while ( GraalModifyLock )
      {
        GraalLimitedLoop( GraalModifyInstancePanel.PANEL );
      }

      GraalRejectRectangle( Rectangle );
      GraalDisplayRectangle( Rectangle );

      if ( GraalModifyAbort ) break;

      if ( GraalModifyApply )
      {
        if ( ! GraalInstanceChanged( (phins_list *)Pointer ) )

          continue;

        GraalDeleteRectangle( Rectangle );

        for ( Instance  = GraalFigureRds->INSTANCE;
              Instance != (rdsins_list *)NULL;
              Instance  = Instance->NEXT )
        { 
          if ( Instance->INSNAME == GraalInstanceMName )
          {
            if ( ! IsGraalDeleted( Instance->LAYERTAB[ RDS_ABOX ] ) )
            {
              GraalErrorMessage( GraalMainWindow,
                                 "All instances must have different names !" );
              break;
            }
          }  
        }

        if ( Instance == (rdsins_list *)NULL )
        {
          if ( FirstUndo )
          {
            GraalAddUndo();

            FirstUndo = 0;
          }
 
          GraalAddUndoRec( Rectangle );

          SaveInstance          = GraalFigureMbk->PHINS;
          GraalFigureMbk->PHINS = (phins_list *)NULL;
 
          Element = (void *)
 
            addphins( GraalFigureMbk,
                      GraalInstanceMModel,
                      GraalInstanceMName,
                      GraalInstanceMSym,
                      ((phins_list *)Pointer)->XINS,
                      ((phins_list *)Pointer)->YINS );
 
          ((phins_list *)Element)->NEXT = SaveInstance;
          ((phins_list *)Element)->USER = (void *)(&GraalFigureMbk->PHINS);
    
          if ( SaveInstance != (phins_list *)NULL )
          {
            SaveInstance->USER = (void *)(&((phins_list *)Element)->NEXT);
          }
 
          Instance = GraalAddInstance( ((phins_list *)Element) );

          GraalDisplayToolsMessage();

          if ( Instance == (rdsins_list *)NULL )
          {
            GraalFigureMbk->PHINS = SaveInstance;
            mbkfree( Element );

            GraalErrorMessage( GraalMainWindow, "Unable to load instance model !" );
          }
          else
          {   
            NewRec = Instance->LAYERTAB[ RDS_ABOX ];

            GraalAddUndoRec( NewRec );
          }

          GraalZoomRefresh();
        }
        else
        {
          GraalUndeleteRectangle( Rectangle );
        }
      }
    }  
  } 

  GraalExitPanel( &GraalModifyInstancePanel );

  GraalDelEqui();
  GraalDelPeek();
  GraalDelSelect();
  GraalZoomRefresh();


  rdsend();
}
