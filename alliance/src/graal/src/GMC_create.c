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
| File    :                  Create.c                         |
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
# include "GRM.h"
# include "GMX.h"
# include "GTB.h"
# include "GSB.h"
# include "GMC.h"
# include "GMV.h"
# include "GMT.h"

# include "GMC_create.h"
# include "GMC_dialog.h"

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
|                        Segment & Wire                       |
|                                                             |
\------------------------------------------------------------*/

  char  GraalSegmentLayer = ALU1;
  float GraalSegmentWidth = -1.0;
  char *GraalSegmentName  = (char *)NULL;
  char  GraalSegmentWire  = GRAAL_FALSE;

/*------------------------------------------------------------\
|                                                             |
|                              Via                            |
|                                                             |
\------------------------------------------------------------*/

  char  GraalViaType      = CONT_POLY;
  char *GraalViaName      = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                            Big Via                          |
|                                                             |
\------------------------------------------------------------*/

  char  GraalBigViaType   = CONT_VIA;
  char *GraalBigViaName   = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Transistor                        |
|                                                             |
\------------------------------------------------------------*/

  char  GraalTransistorType  = NTRANS;
  float GraalTransistorWidth = -1.0;
  char *GraalTransistorName  = (char *)NULL;
  char  GraalTransistorWire  = GRAAL_FALSE;
 
/*------------------------------------------------------------\
|                                                             |
|                           Connector                         |
|                                                             |
\------------------------------------------------------------*/

  char  GraalConnectorLayer  = ALU1;
  float GraalConnectorWidth  = -1.0;
  char *GraalConnectorName   = (char *)NULL;
  char  GraalConnectorOrient = GRAAL_NORTH;

/*------------------------------------------------------------\
|                                                             |
|                           Reference                         |
|                                                             |
\------------------------------------------------------------*/

  char  GraalReferenceType   = MBK_REF_REF;
  char *GraalReferenceName   = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Instance                          |
|                                                             |
\------------------------------------------------------------*/

  char  GraalInstanceSym      = NOSYM;
  char *GraalInstanceName     = (char *)NULL;
  char *GraalInstanceModel    = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalCreateFigureMbk                   |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateFigureMbk()

{
  if ( GraalFigureMbk == (phfig_list *)NULL )
  {
    rdsbegin();

    GraalFigureMbk = addphfig( GRAAL_DEFAULT_FIGURE_NAME );

    GraalAddFigure();
    GraalChangeTopLevelTitle( GraalFigureMbk->NAME );

    rdsend();
  }
}  

/*------------------------------------------------------------\
|                                                             |
|                     GraalCreateSegmentMbk                   |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateSegmentMbk( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  rdsrec_list *Rectangle;
  phseg_list  *Segment;
  char        *SaveName;
  char        *SegmentName;
  long         Delta;

  rdsbegin();

  if ( LambdaX1 == LambdaX2 )
  {
    Delta = LambdaY2 - LambdaY1;
  }
  else
  {
    Delta = LambdaX2 - LambdaX1;
  }

  if ( Delta < 0 ) Delta = - Delta;
  
  if ( Delta < GRAAL_SCALE * GRAAL_SEGMENT_VALUE_TABLE[(int)GraalSegmentLayer][1] )
  {
    GraalErrorMessage( GraalMainWindow, "This segment is too small !" );

    rdsend();
    return;
  }

  if ( ( GraalSegmentLayer >= CALU1 ) &&
       ( GraalSegmentLayer <= CALU9 ) )
  {
    SaveName = GraalSegmentName;

    GraalEnterDialog( &GraalChangeSegmentNameDialog );

    if ( GraalCreateDialogCancel )
    {
      rdsend();
      return;
    }

    SegmentName      = GraalSegmentName;
    GraalSegmentName = SaveName;

    if ( SegmentName == (char *)NULL )
    {
      GraalErrorMessage( GraalMainWindow, "Connector must have name !" );

      rdsend();
      return;
    }
  }
  else
  {
    SegmentName = GraalSegmentName;
  }

  GraalCreateFigureMbk();

  Segment = 

     addphseg( GraalFigureMbk, GraalSegmentLayer, 
               (long)( GraalSegmentWidth * SCALE_X ),
               (long)( LambdaX1 * GRAAL_SCALE_X ),
               (long)( LambdaY1 * GRAAL_SCALE_X ),
               (long)( LambdaX2 * GRAAL_SCALE_X ),
               (long)( LambdaY2 * GRAAL_SCALE_X ), 
               SegmentName );
                         
  Segment->USER = (void *)(&GraalFigureMbk->PHSEG);

  if ( Segment->NEXT != (phseg_list *)NULL )
  {
    Segment->NEXT->USER = (void *)(&Segment->NEXT);
  }

  Rectangle = GraalAddSegment( Segment );

  if ( Rectangle != (rdsrec_list *)NULL )
  {
    GraalAddUndo();
    GraalAddUndoRec( Rectangle );

    if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
         ( GraalHeadPeek != (graalpeek   *)NULL ) )
    {
      GraalDelEqui();
      GraalDelPeek();
      GraalZoomRefresh();
    }
    else 
    {
      GraalDisplayRectangle( Rectangle );
    }
  }
  else
  {
    delphseg( GraalFigureMbk, Segment );

    GraalErrorMessage( GraalMainWindow, "Can't create any segment of this layer !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalCreateViaMbk                     |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateViaMbk( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  rdsrec_list *Rectangle;
  phvia_list  *Via;

  rdsbegin();

  GraalCreateFigureMbk();

  Via = addphvia( GraalFigureMbk, GraalViaType, 
                  (long)( LambdaX1 * GRAAL_SCALE_X ),
                  (long)( LambdaY1 * GRAAL_SCALE_X ), 0, 0, GraalViaName );

  Via->USER = (void *)(&GraalFigureMbk->PHVIA);

  if ( Via->NEXT != (phvia_list *)NULL )
  {
    Via->NEXT->USER = (void *)(&Via->NEXT);
  }

  Rectangle = GraalAddVia( Via );

  if ( Rectangle != (rdsrec_list *)NULL )
  {
    GraalAddUndo();
    GraalAddUndoRec( Rectangle );

    if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
         ( GraalHeadPeek != (graalpeek   *)NULL ) ) 
    {
      GraalDelEqui();
      GraalDelPeek(); 
      GraalZoomRefresh();
    }
    else
    {   
      GraalDisplayRectangle( Rectangle );
    }
  }
  else
  {
    delphvia( GraalFigureMbk, Via );

    GraalErrorMessage( GraalMainWindow, "Can't create any via of this type !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalCreateBigViaMbk                  |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateBigViaMbk( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
{
  rdsrec_list *Rectangle;
  phvia_list  *Via;
  long         Swap;
  long         ViaDx;
  long         ViaDy;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }

    ViaDx = LambdaX2 - LambdaX1;
    ViaDy = LambdaY2 - LambdaY1;

    if ( ( ViaDx < GRAAL_SCALE * GRAAL_BIGVIA_VALUE_TABLE[ (int)GraalBigViaType ][0] ) ||
         ( ViaDy < GRAAL_SCALE * GRAAL_BIGVIA_VALUE_TABLE[ (int)GraalBigViaType ][0] ) )
    {
      GraalErrorMessage( GraalMainWindow, "This big via is too small !" );

      rdsend();
      return;
    }
  }  
  else
  {
    GraalErrorMessage( GraalMainWindow, "The big via must be rectangular !" );

    rdsend();
    return;
  }

  GraalCreateFigureMbk();

  Via = addphvia( GraalFigureMbk, GraalBigViaType, 
                  (long)( (( LambdaX1 + LambdaX2 ) / 2.0) * GRAAL_SCALE_X ),
                  (long)( (( LambdaY1 + LambdaY2 ) / 2.0) * GRAAL_SCALE_X ),
                  (long)( ViaDx * GRAAL_SCALE_X ), 
                  (long)( ViaDy * GRAAL_SCALE_X ), GraalBigViaName );

  Via->USER = (void *)(&GraalFigureMbk->PHVIA);

  if ( Via->NEXT != (phvia_list *)NULL )
  {
    Via->NEXT->USER = (void *)(&Via->NEXT);
  }

  Rectangle = GraalAddVia( Via );

  if ( Rectangle != (rdsrec_list *)NULL )
  {
    GraalAddUndo();
    GraalAddUndoRec( Rectangle );

    if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
         ( GraalHeadPeek != (graalpeek   *)NULL ) ) 
    {
      GraalDelEqui();
      GraalDelPeek(); 
      GraalZoomRefresh();
    }
    else
    {   
      GraalDisplayRectangle( Rectangle );
    }
  }
  else
  {
    delphvia( GraalFigureMbk, Via );

    GraalErrorMessage( GraalMainWindow, "Can't create any big via of this type !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalCreateTransistorMbk                 |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateTransistorMbk( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
{
  rdsrec_list *Rectangle;
  phseg_list  *Transistor;
  long         Delta;

  rdsbegin();

  if ( LambdaX1 == LambdaX2 )
  {
    Delta = LambdaY2 - LambdaY1;
  } 
  else
  {   
    Delta = LambdaX2 - LambdaX1;
  } 

  if ( Delta < 0 ) Delta = - Delta;
  
  if ( Delta < GRAAL_SCALE * GRAAL_SEGMENT_VALUE_TABLE[(int)GraalTransistorType][1] )
  { 
    GraalErrorMessage( GraalMainWindow, "This transistor is too small !" );

    rdsend();
    return;
  } 

  GraalCreateFigureMbk();
 
  Transistor =
 
     addphseg( GraalFigureMbk, GraalTransistorType,
               (long)( GraalTransistorWidth * SCALE_X ),
               (long)( LambdaX1 * GRAAL_SCALE_X ),
               (long)( LambdaY1 * GRAAL_SCALE_X ),
               (long)( LambdaX2 * GRAAL_SCALE_X ),
               (long)( LambdaY2 * GRAAL_SCALE_X ), 
               GraalTransistorName );
     
  Transistor->USER = (void *)(&GraalFigureMbk->PHSEG);
    
  if ( Transistor->NEXT != (phseg_list *)NULL )
  {
    Transistor->NEXT->USER = (void *)(&Transistor->NEXT);
  }
    
  Rectangle = GraalAddSegment( Transistor );

  if ( Rectangle != (rdsrec_list *)NULL )
  {
    GraalAddUndo();
    GraalAddUndoRec( Rectangle );

    if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
         ( GraalHeadPeek != (graalpeek   *)NULL ) ) 
    {
      GraalDelEqui();
      GraalDelPeek(); 
      GraalZoomRefresh();
    }
    else
    {   
      GraalDisplayRectangle( Rectangle );
    }
  }
  else
  {   
    delphseg( GraalFigureMbk, Transistor );

    GraalErrorMessage( GraalMainWindow, "Can't create any transistor of this type !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalCreateConnectorMbk                 |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateConnectorMbk( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  rdsrec_list *Rectangle;
  phcon_list  *Connector;
  char         MbkOrient;

  rdsbegin();

  LambdaX1 = (long)(LambdaX1 * GRAAL_SCALE_X);
  LambdaY1 = (long)(LambdaY1 * GRAAL_SCALE_X);

  switch ( GraalConnectorOrient )
  {
    case GRAAL_NORTH : MbkOrient = NORTH;
    break;
    case GRAAL_SOUTH : MbkOrient = SOUTH;
    break;
    case GRAAL_EAST  : MbkOrient = EAST;
    break;
    default          : MbkOrient = WEST;
  }

  GraalEnterDialog( &GraalChangeConnectorNameDialog );

  if ( GraalCreateDialogCancel )
  {
    rdsend();
    return;
  }

  if ( GraalConnectorName != (char *)NULL )
  {
    GraalCreateFigureMbk();

    Connector =

       addphcon( GraalFigureMbk, MbkOrient,
                 GraalConnectorName,
                 LambdaX1, LambdaY1,
                 GraalConnectorLayer,
                 (long)( GraalConnectorWidth * SCALE_X ) );
     
    Connector->USER = (void *)(&GraalFigureMbk->PHCON);
      
    if ( Connector->NEXT != (phcon_list *)NULL )
    {
      Connector->NEXT->USER = (void *)(&Connector->NEXT);
    }
   
    Rectangle = GraalAddConnector( Connector );
      
    if ( Rectangle != (rdsrec_list *)NULL )
    {
      GraalAddUndo();
      GraalAddUndoRec( Rectangle );

      if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
           ( GraalHeadPeek != (graalpeek   *)NULL ) ) 
      {
        GraalDelEqui();
        GraalDelPeek(); 
        GraalZoomRefresh();
      }
      else
      {   
        GraalDisplayRectangle( Rectangle );
      }
    }
    else
    {   
      delphcon( GraalFigureMbk, Connector );

      GraalErrorMessage( GraalMainWindow, "Can't create any connector of this layer !" );
    }
  }
  else
  {
    GraalErrorMessage( GraalMainWindow, "Connector must have name !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    GraalCreateReferenceMbk                  |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateReferenceMbk( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  rdsrec_list *Rectangle;
  phref_list  *Reference;

  rdsbegin();

  GraalEnterDialog( &GraalChangeReferenceNameDialog );

  if ( GraalCreateDialogCancel )
  {
    rdsend();
    return;
  }

  if ( GraalReferenceName != (char *)NULL )
  {
    GraalCreateFigureMbk();

    Reference = 

      addphref( GraalFigureMbk, 
                (GraalReferenceType == MBK_REF_REF) ? "ref_ref":"ref_con" ,
                GraalReferenceName,
                (long)( LambdaX1 * GRAAL_SCALE_X ),
                (long)( LambdaY1 * GRAAL_SCALE_X ) );

    Reference->USER = (void *)(&GraalFigureMbk->PHREF);

    if ( Reference->NEXT != (phref_list *)NULL )
    {
      Reference->NEXT->USER = (void *)(&Reference->NEXT);
    }
 
    Rectangle = GraalAddReference( Reference );
 
    if ( Rectangle != (rdsrec_list *)NULL )
    {
      GraalAddUndo();
      GraalAddUndoRec( Rectangle );

      if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
           ( GraalHeadPeek != (graalpeek   *)NULL ) ) 
      {
        GraalDelEqui();
        GraalDelPeek(); 
        GraalZoomRefresh();
      }
      else
      {   
        GraalDisplayRectangle( Rectangle );
      }
    }
    else
    {
      delphref( GraalFigureMbk, Reference );
 
      GraalErrorMessage( GraalMainWindow, "Can't create any reference of this type !" );
    }
  }
  else
  {
    GraalErrorMessage( GraalMainWindow, "Reference must have name !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalCreateInstanceMbk                  |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateInstanceMbk( LambdaX1, LambdaY1 )

   long LambdaX1;
   long LambdaY1;
{
  phins_list  *InstanceMbk;
  phins_list  *SaveInstance;
  rdsins_list *InstanceRds;

  rdsbegin();

  GraalEnterChangeInstanceModelDialog();

  if ( GraalCreateDialogCancel ) 
  {
    rdsend();
    return;
  }

  if ( GraalInstanceModel != (char *)NULL )
  {
    GraalEnterDialog( &GraalChangeInstanceNameDialog  );

    if ( GraalCreateDialogCancel )
    {
      rdsend();
      return;
    }

    if ( GraalInstanceName != (char *)NULL )
    {
      GraalCreateFigureMbk();

      if ( GraalFigureMbk->NAME == GraalInstanceModel )
      {
        GraalErrorMessage( GraalMainWindow, 
                           "A figure cannot be part of itself !" );
        rdsend();
        return;
      }

      for ( InstanceRds  = GraalFigureRds->INSTANCE;
            InstanceRds != (rdsins_list *)NULL;
            InstanceRds  = InstanceRds->NEXT )
      { 
        if ( InstanceRds->INSNAME == GraalInstanceName )
        {
          if ( ! IsGraalDeleted( InstanceRds->LAYERTAB[ RDS_ABOX ] ) )
          {
            GraalErrorMessage( GraalMainWindow, 
                               "All instances must have different names !" );
            rdsend();
            return;
          }
        }
      }

      SaveInstance          = GraalFigureMbk->PHINS;
      GraalFigureMbk->PHINS = (phins_list *)NULL;

      InstanceMbk =

        addphins( GraalFigureMbk,
                  GraalInstanceModel,
                  GraalInstanceName,
                  GraalInstanceSym,
                  (long)( LambdaX1 * GRAAL_SCALE_X ),
                  (long)( LambdaY1 * GRAAL_SCALE_X ) );

      InstanceMbk->NEXT = SaveInstance;
      InstanceMbk->USER = (void *)(&GraalFigureMbk->PHINS);
   
      if ( SaveInstance != (phins_list *)NULL )
      {
        SaveInstance->USER = (void *)(&InstanceMbk->NEXT);
      }
   
      InstanceRds = GraalAddInstance( InstanceMbk );

      GraalDisplayToolsMessage();

      if ( InstanceRds == (rdsins_list *)NULL )
      {
        GraalFigureMbk->PHINS = SaveInstance;
        mbkfree( InstanceMbk );

        GraalErrorMessage( GraalMainWindow, "Unable to load instance model !" );
      }
      else
      {
        GraalDelEqui();
        GraalAddUndo();
        GraalAddUndoRec( InstanceRds->LAYERTAB[ RDS_ABOX ] );
        GraalZoomRefresh();
      }
    }
    else
    {
      GraalErrorMessage( GraalMainWindow, "An instance must have a name !" );
    }
  }
  else
  {
    GraalErrorMessage( GraalMainWindow, "An instance model name must specified !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                   Graal Create Abutmentbox                  |
|                                                             |
\------------------------------------------------------------*/

void GraalCreateAbutmentBoxMbk( LambdaX1, LambdaY1, LambdaX2, LambdaY2 )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
{
  rdsrec_list *Rectangle;
  long         Swap;

  rdsbegin();

  if ( ( LambdaX1 != LambdaX2 ) &&
       ( LambdaY1 != LambdaY2 ) )
  {
    if ( LambdaX1 > LambdaX2 )
    {
      Swap = LambdaX1; LambdaX1 = LambdaX2; LambdaX2 = Swap;
    }

    if ( LambdaY1 > LambdaY2 )
    {
      Swap = LambdaY1; LambdaY1 = LambdaY2; LambdaY2 = Swap;
    }
  }  
  else
  {
    GraalErrorMessage( GraalMainWindow, "The abutment box must be rectangular !" );

    rdsend();
    return;
  }

  GraalCreateFigureMbk();

  GraalFigureMbk->XAB1 = (long)( LambdaX1 * GRAAL_SCALE_X );
  GraalFigureMbk->YAB1 = (long)( LambdaY1 * GRAAL_SCALE_X );
  GraalFigureMbk->XAB2 = (long)( LambdaX2 * GRAAL_SCALE_X );
  GraalFigureMbk->YAB2 = (long)( LambdaY2 * GRAAL_SCALE_X );

  Rectangle = GraalAddAbox();

  if ( Rectangle != (rdsrec_list *)NULL )
  {
    GraalAddUndo();
    GraalAddUndoRec( Rectangle );

    if ( ( GraalHeadEqui != (rdsrec_list *)NULL ) || 
         ( GraalHeadPeek != (graalpeek   *)NULL ) ) 
    {
      GraalDelEqui();
      GraalDelPeek(); 
      GraalZoomRefresh();
    }
    else
    {   
      GraalDisplayRectangle( Rectangle );
    }
  
    GraalChangeEditMode( GRAAL_EDIT_MEASURE,
                         GraalPromptEditMeasure );
  }
  else
  {   
    GraalFigureMbk->XAB1 = 0;
    GraalFigureMbk->YAB1 = 0;
    GraalFigureMbk->XAB2 = 0;
    GraalFigureMbk->YAB2 = 0;
  
    GraalErrorMessage( GraalMainWindow, "Can't create this abutment box !" );
  }

  rdsend();
}
