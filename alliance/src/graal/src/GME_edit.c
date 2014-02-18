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
| File    :                   Edit.c                          |
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
# include "GME.h"
# include "GMV.h"
# include "GMT.h"

# include "GME_edit.h"
# include "GME_panel.h"
# include "GME_message.h"

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
|                        Identify Buffer                      |
|                                                             |
\------------------------------------------------------------*/

  static char  GraalIdentifyMessage[ GRAAL_IDENTIFY_MESSAGE_SIZE ];
  static char  GraalIdentifyBuffer [ GRAAL_IDENTIFY_BUFFER_SIZE  ];
  static char *GraalScanIdentify;
  static long  GraalIdentifyLength;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalEditTreatUndo                     |
|                                                             |
\------------------------------------------------------------*/

void GraalEditTreatUndo( HeadUndo )

  graalundo *HeadUndo;
{
  rdsrec_list  *Rec;
  graalundorec *UndoRec;
  char          OneElement;

  if ( ( HeadUndo->UNDO->NEXT != (graalundorec *)NULL ) ||
       ( GraalHeadEqui        != (rdsrec_list  *)NULL ) ||
       ( GraalHeadPeek        != (graalpeek    *)NULL ) )
  { 
    OneElement = GRAAL_FALSE;
  }
  else
  {
    OneElement = (GraalHeadSelect == (graalselect *)NULL);
  }

  GraalDelSelect();
  GraalDelEqui();
  GraalDelPeek();
  GraalRecomputeBound = GRAAL_TRUE;

  for ( UndoRec  = HeadUndo->UNDO;
        UndoRec != (graalundorec *)NULL;
        UndoRec  = UndoRec->NEXT )
  {
    Rec = UndoRec->RECTANGLE;

    if ( GetRdsLayer( Rec ) != RDS_ABOX )
    {
      if ( IsGraalDeleted( Rec ) )
      {
        GraalUndeleteRectangle( Rec );
        if ( OneElement ) GraalDisplayRectangle( Rec );
      }
      else
      {   
        GraalDeleteRectangle( Rec );
        if ( OneElement ) GraalDisplayRectangle( Rec );
      }
    }
    else
    {
      if ( IsRdsFigure( Rec ) )
      {
        if ( IsGraalDeleted( Rec ) )
        { 
          ClearGraalDeleted( Rec );

          GraalFigureMbk->XAB1 = (long)( Rec->X * GRAAL_SCALE_X / GRAAL_RDS_LAMBDA );
          GraalFigureMbk->YAB1 = (long)( Rec->Y * GRAAL_SCALE_X / GRAAL_RDS_LAMBDA );
          GraalFigureMbk->XAB2 = (long)( ( Rec->X + Rec->DX ) * GRAAL_SCALE_X / GRAAL_RDS_LAMBDA );
          GraalFigureMbk->YAB2 = (long)( ( Rec->Y + Rec->DY ) * GRAAL_SCALE_X / GRAAL_RDS_LAMBDA );
        }
        else
        {
          GraalFigureMbk->XAB1 = 0;
          GraalFigureMbk->XAB2 = 0;
          GraalFigureMbk->YAB1 = 0;
          GraalFigureMbk->YAB2 = 0;

          GraalDeleteRectangle( Rec );
        }
      }
      else
      {
        if ( IsGraalDeleted( Rec ) )
        {
          ClearGraalDeleted( Rec );
        }
        else
        {
          GraalDeleteRectangle( Rec );
        }
      }

      OneElement = GRAAL_FALSE;
    }
  }

  HeadUndo->UNDO = (graalundorec *)reverse( (chain_list *)HeadUndo->UNDO );

  if ( ! OneElement )
  {
    GraalZoomRefresh();
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditRedo                          |
|                                                             |
\------------------------------------------------------------*/

void GraalEditRedo()

{
  graalundo *NewUndo;

  rdsbegin();

  if ( GraalHeadRedo != (graalundo *)NULL )
  {
    GraalEditTreatUndo( GraalHeadRedo );

    NewUndo       = GraalHeadRedo;
    GraalHeadRedo = GraalHeadRedo->NEXT;
    NewUndo->NEXT = GraalHeadUndo;
    GraalHeadUndo = NewUndo;
  }
  else
  {
    GraalWarningMessage( GraalMainWindow, "Nothing to Redo !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalEditUndo                          |
|                                                             |
\------------------------------------------------------------*/

void GraalEditUndo()

{
  graalundo *NewRedo;

  rdsbegin();

  if ( GraalHeadUndo != (graalundo *)NULL )
  {
    GraalEditTreatUndo( GraalHeadUndo );

    NewRedo       = GraalHeadUndo;
    GraalHeadUndo = GraalHeadUndo->NEXT;
    NewRedo->NEXT = GraalHeadRedo;
    GraalHeadRedo = NewRedo;
  }
  else
  {
    GraalWarningMessage( GraalMainWindow, "Nothing to undo !" );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalAddIdentify                    |
|                                                             |
\------------------------------------------------------------*/

char GraalAddIdentify( Rectangle )

  rdsrec_list *Rectangle;
{
  void *Pointer;
  char  Orient;
  char  MbkLayer;
  long  Length;

  rdsbegin();

  GraalIdentifyBuffer[0] = '\0';
  Pointer = GRAAL_MBK( Rectangle );

  if ( IsRdsSegment( Rectangle ) )
  {
    MbkLayer = ((phseg_list *)Pointer)->LAYER;

    if ( GET_LYNX_TRANSISTOR_TYPE( MbkLayer ) != RDS_LYNX_TRANSISTOR_EMPTY )
    {
      sprintf( GraalIdentifyBuffer, 
      "TRANSISTOR :\n\n  NAME : %s\n  TYPE : %s\n  X1 : %.2f\n  Y1 : %.2f\n  X2 : %.2f  \n  Y2 : %.2f\n  WIDTH : %.2f\n\n",
      (((phseg_list *)Pointer)->NAME != (char *)0) ? ((phseg_list *)Pointer)->NAME : "None",
      GRAAL_TRANSISTOR_NAME_TABLE[ (int)((phseg_list *)Pointer)->LAYER ][0],
      (float)((phseg_list *)Pointer)->X1    / (float)SCALE_X, 
      (float)((phseg_list *)Pointer)->Y1    / (float)SCALE_X,
      (float)((phseg_list *)Pointer)->X2    / (float)SCALE_X,
      (float)((phseg_list *)Pointer)->Y2    / (float)SCALE_X,
      (float)((phseg_list *)Pointer)->WIDTH / (float)SCALE_X );
    }
    else
    {
      sprintf( GraalIdentifyBuffer, 
      "SEGMENT :\n\n  NAME : %s\n  LAYER : %s\n  X1 : %.2f\n  Y1 : %.2f\n  X2 : %.2f\n  Y2 : %.2f\n  WIDTH : %.2f\n\n",
      (((phseg_list *)Pointer)->NAME != (char *)0) ? ((phseg_list *)Pointer)->NAME : "None",
      GRAAL_SEGMENT_NAME_TABLE[ (int)((phseg_list *)Pointer)->LAYER ][0],
      (float)((phseg_list *)Pointer)->X1    / (float)SCALE_X, 
      (float)((phseg_list *)Pointer)->Y1    / (float)SCALE_X,
      (float)((phseg_list *)Pointer)->X2    / (float)SCALE_X,
      (float)((phseg_list *)Pointer)->Y2    / (float)SCALE_X,
      (float)((phseg_list *)Pointer)->WIDTH / (float)SCALE_X );
    }
  }
  else
  if ( IsRdsConnector( Rectangle ) )
  {
    switch( ((phcon_list *)Pointer)->ORIENT )
    {
      case NORTH : Orient = GRAAL_NORTH;
      break;
      case SOUTH : Orient = GRAAL_SOUTH;
      break;
      case EAST  : Orient = GRAAL_EAST;
      break;
      default    : Orient = GRAAL_WEST;
    }

    sprintf( GraalIdentifyBuffer,
    "CONNECTOR :\n\n  NAME : %s\n  LAYER : %s\n  XCON : %.2f\n  YCON : %.2f\n  WIDTH : %.2f\n  ORIENT : %s\n  INDEX : %ld\n\n",
    (((phcon_list *)Pointer)->NAME != (char *)0) ? ((phcon_list *)Pointer)->NAME : "None",
    GRAAL_CONNECTOR_NAME_TABLE[ (int)((phcon_list *)Pointer)->LAYER ][0],
    (float)((phcon_list *)Pointer)->XCON  / (float)SCALE_X,
    (float)((phcon_list *)Pointer)->YCON  / (float)SCALE_X,
    (float)((phcon_list *)Pointer)->WIDTH / (float)SCALE_X,
    GRAAL_ORIENT_NAME_TABLE[ (int)Orient ][0],
    ((phcon_list *)Pointer)->INDEX );
  }
  else
  if ( IsRdsVia( Rectangle ) )
  {
    if ( ( ((phvia_list *)Pointer)->DX == 0 ) &&
         ( ((phvia_list *)Pointer)->DY == 0 ) )
    {
      sprintf( GraalIdentifyBuffer,
      "VIA :\n\n  NAME : %s\n  TYPE: %s\n  XVIA : %.2f\n  YVIA : %.2f\n\n",
      (((phvia_list *)Pointer)->NAME != (char *)0) ? ((phvia_list *)Pointer)->NAME : "None",
      GRAAL_VIA_NAME_TABLE[ (int)((phvia_list *)Pointer)->TYPE ][0],
      (float)((phvia_list *)Pointer)->XVIA  / (float)SCALE_X,
      (float)((phvia_list *)Pointer)->YVIA  / (float)SCALE_X );
    }
    else
    {
      sprintf( GraalIdentifyBuffer,
      "BIGVIA :\n\n  NAME: %s\n  TYPE: %s\n  XVIA : %.2f\n  YVIA : %.2f\n  DX   : %.2f\n  DY  : %.2f\n\n",
      (((phvia_list *)Pointer)->NAME != (char *)0) ? ((phvia_list *)Pointer)->NAME : "None",
      GRAAL_BIGVIA_NAME_TABLE[ (int)((phvia_list *)Pointer)->TYPE ][0],
      (float)((phvia_list *)Pointer)->XVIA / (float)SCALE_X,
      (float)((phvia_list *)Pointer)->YVIA / (float)SCALE_X,
      (float)((phvia_list *)Pointer)->DX   / (float)SCALE_X,
      (float)((phvia_list *)Pointer)->DY   / (float)SCALE_X );
    }
  }
  else
  if ( IsRdsReference( Rectangle ) )
  {
    sprintf( GraalIdentifyBuffer,
    "REFERENCE :\n\n  NAME : %s\n  TYPE : %s\n  XREF : %.2f\n  YREF : %.2f\n\n",
    (((phref_list *)Pointer)->NAME != (char *)0) ? ((phref_list *)Pointer)->NAME : "None",
    ( ! strcmp( ((phref_list *)Pointer)->FIGNAME, "ref_ref" ) ) ? "Ref_Ref" : "Ref_Con",
    (float)((phref_list *)Pointer)->XREF  / (float)SCALE_X,
    (float)((phref_list *)Pointer)->YREF  / (float)SCALE_X );
  }
  else
  if ( IsRdsInstance( Rectangle ) )
  {
    Orient = ((phins_list *)Pointer)->TRANSF;

    sprintf( GraalIdentifyBuffer,
    "INSTANCE :\n\n  NAME : %s\n  MODEL : %s\n  TRANSF : %s\n  XINS : %.2f\n  YINS : %.2f\n\n",
    ((phins_list *)Pointer)->INSNAME,
    ((phins_list *)Pointer)->FIGNAME,
    GRAAL_SYMMETRY_NAME_TABLE[ (int)Orient ][0], 
    (float)((phins_list *)Pointer)->XINS  / (float)SCALE_X,
    (float)((phins_list *)Pointer)->YINS  / (float)SCALE_X );
  }
  if ( IsRdsFigure( Rectangle ) )
  {
    sprintf( GraalIdentifyBuffer,
    "ABUTMENT BOX :\n\n  NAME : %s\n  XAB1 : %.2f\n  YAB1 : %.2f\n  XAB2 : %.2f\n  YAB2 : %.2f\n\n",
    ((phfig_list *)Pointer)->NAME,
    (float)((phfig_list *)Pointer)->XAB1  / (float)SCALE_X,
    (float)((phfig_list *)Pointer)->YAB1  / (float)SCALE_X,
    (float)((phfig_list *)Pointer)->XAB2  / (float)SCALE_X,
    (float)((phfig_list *)Pointer)->YAB2  / (float)SCALE_X );
  }

  Length = strlen( GraalIdentifyBuffer );

  if ( ( GraalIdentifyLength + Length ) < ( GRAAL_IDENTIFY_MESSAGE_SIZE - 1 ) )
  {
    strcpy( GraalScanIdentify, GraalIdentifyBuffer );

    GraalScanIdentify   += Length; 
    GraalIdentifyLength += Length;

    rdsend();
    return( GRAAL_TRUE );
  }

  strcpy( GraalScanIdentify, "#" );
 
  rdsend();
  return( GRAAL_FALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalEditIdentify                       |
|                                                             |
\------------------------------------------------------------*/

void GraalEditIdentify()
{
  graalselect *Select;

  rdsbegin();

  strcpy( GraalIdentifyMessage, "No element found !" );
  GraalScanIdentify   = GraalIdentifyMessage;
  GraalIdentifyLength = 0;

  for ( Select  = GraalHeadSelect; 
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    if ( ! GraalAddIdentify( Select->RECTANGLE ) ) break;
  }

  GraalDelSelect();

  GraalDisplayIdentifyMessage( GraalIdentifyMessage );
  GraalEnterPanel( &GraalEditIdentifyPanel       );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalEditCopy                        |
|                                                             |
\------------------------------------------------------------*/

void GraalEditCopy( LambdaX1, LambdaY1, LambdaX2, LambdaY2, Mode )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
   char Mode; 
{
  graalselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  void        *Element;
  void        *Pointer;
  char         FirstUndo;
  long         DeltaX;
  long         DeltaY;
  long         X1;
  long         X2;
  long         Y1;
  long         Y2;

  rdsbegin();

  DeltaX = (long)( ( LambdaX2 - LambdaX1 ) * GRAAL_SCALE_X );
  DeltaY = (long)( ( LambdaY2 - LambdaY1 ) * GRAAL_SCALE_X );

  FirstUndo = 1;

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( IsRdsSegment( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phseg_list *)Pointer)->X1 + DeltaX;
      Y1 = ((phseg_list *)Pointer)->Y1 + DeltaY;
      X2 = ((phseg_list *)Pointer)->X2 + DeltaX;
      Y2 = ((phseg_list *)Pointer)->Y2 + DeltaY;

      Element = (void *)

         addphseg( GraalFigureMbk, 
                   ((phseg_list *)Pointer)->LAYER,
                   ((phseg_list *)Pointer)->WIDTH,
                   X1, Y1, X2, Y2,
                   ((phseg_list *)Pointer)->NAME );

      ((phseg_list *)Element)->TYPE = ((phseg_list *)Pointer)->TYPE;
      ((phseg_list *)Element)->USER = (void *)(&GraalFigureMbk->PHSEG);

      if ( ((phseg_list *)Element)->NEXT != (phseg_list *)NULL )
      {
        ((phseg_list *)Element)->NEXT->USER = (void *)(&((phseg_list *)Element)->NEXT);
      }

      NewRec = GraalAddSegment( ((phseg_list *)Element) );

      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsVia( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phvia_list *)Pointer)->XVIA + DeltaX;
      Y1 = ((phvia_list *)Pointer)->YVIA + DeltaY;

      Element = (void *)

         addphvia( GraalFigureMbk,
                   ((phvia_list *)Pointer)->TYPE, X1, Y1,
                   ((phvia_list *)Pointer)->DX, ((phvia_list *)Pointer)->DY,
                   ((phvia_list *)Pointer)->NAME );

      ((phvia_list *)Element)->USER = (void *)(&GraalFigureMbk->PHVIA);

      if ( ((phvia_list *)Element)->NEXT != (phvia_list *)NULL )
      {
        ((phvia_list *)Element)->NEXT->USER = (void *)(&((phvia_list *)Element)->NEXT);
      }

      NewRec = GraalAddVia( ((phvia_list *)Element) );

      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsReference( Rectangle ) )
    { 
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phref_list *)Pointer)->XREF + DeltaX;
      Y1 = ((phref_list *)Pointer)->YREF + DeltaY;

      Element = (void *)

         addphref( GraalFigureMbk,
                   ((phref_list *)Pointer)->FIGNAME,
                   Rectangle->NAME, X1, Y1 );

      ((phref_list *)Element)->USER = (void *)(&GraalFigureMbk->PHREF);

      if ( ((phref_list *)Element)->NEXT != (phref_list *)NULL )
      {
        ((phref_list *)Element)->NEXT->USER = (void *)(&((phref_list *)Element)->NEXT);
      }

      NewRec = GraalAddReference( ((phvia_list *)Element) );

      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsConnector( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phcon_list *)Pointer)->XCON + DeltaX;
      Y1 = ((phcon_list *)Pointer)->YCON + DeltaY;

      Element = (void *)

        addphcon( GraalFigureMbk,
                  ((phcon_list *)Pointer)->ORIENT,
                  Rectangle->NAME, 
                  X1, Y1,
                  ((phcon_list *)Pointer)->LAYER,
                  ((phcon_list *)Pointer)->WIDTH );

      ((phcon_list *)Element)->USER = (void *)(&GraalFigureMbk->PHCON);

      if ( ((phcon_list *)Element)->NEXT != (phcon_list *)NULL )
      {
        ((phcon_list *)Element)->NEXT->USER = (void *)(&((phcon_list *)Element)->NEXT);
      }

      NewRec = GraalAddConnector( ((phcon_list *)Element) );
 
      GraalAddUndoRec( NewRec );
    }
  }

  GraalDelEqui();
  GraalDelPeek();
  GraalDelSelect();
  GraalZoomRefresh();

  if ( Mode == 0 )
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_COPY,
                         GraalPromptSelectPointCopy );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_COPY,
                         GraalPromptSelectWindowCopy );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalEditMove                        |
|                                                             |
\------------------------------------------------------------*/

void GraalEditMove( LambdaX1, LambdaY1, LambdaX2, LambdaY2, Mode )

   long LambdaX1;
   long LambdaY1;
   long LambdaX2;
   long LambdaY2;
   char Mode;
{
  graalselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  rdsins_list *Instance;
  phins_list  *SaveInstance;
  graalconrec *ConRec;
  void        *Element;
  void        *Pointer;
  char         FirstUndo;
  long         DeltaX;
  long         DeltaY;
  long         X1;
  long         X2;
  long         Y1;
  long         Y2;

  rdsbegin();

  DeltaX = (long)( ( LambdaX2 - LambdaX1 ) * GRAAL_SCALE_X );
  DeltaY = (long)( ( LambdaY2 - LambdaY1 ) * GRAAL_SCALE_X );

  FirstUndo = 1;

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  { 
    if ( IsRdsFigure( Select->RECTANGLE ) ) 
    {
      FirstUndo = 0;

      GraalAddUndo();
      GraalDeleteRectangle( Select->RECTANGLE );
      GraalAddUndoRec( Select->RECTANGLE );

      GraalFigureMbk->XAB1 += DeltaX;
      GraalFigureMbk->XAB2 += DeltaX;
      GraalFigureMbk->YAB1 += DeltaY;
      GraalFigureMbk->YAB2 += DeltaY;

      NewRec = GraalAddAbox();
      GraalAddUndoRec( NewRec );

      for ( ConRec  = GraalHeadConRec;
            ConRec != (graalconrec *)NULL;
            ConRec  = ConRec->NEXT )
      {
        Rectangle = ConRec->RECTANGLE;

        if ( ! IsGraalDeleted( Rectangle ) ) 
        {
          Pointer = GRAAL_MBK( Rectangle );

          X1 = ((phcon_list *)Pointer)->XCON + DeltaX;
          Y1 = ((phcon_list *)Pointer)->YCON + DeltaY;

          if ( ( X1 < GraalFigureMbk->XAB1 ) ||
               ( X1 > GraalFigureMbk->XAB2 ) ||
               ( Y1 < GraalFigureMbk->YAB1 ) ||
               ( Y1 > GraalFigureMbk->YAB2 ) ) continue;

          if ( ( X1 != GraalFigureMbk->XAB1 ) &&
               ( X1 != GraalFigureMbk->XAB2 ) &&
               ( Y1 != GraalFigureMbk->YAB1 ) &&
               ( Y1 != GraalFigureMbk->YAB2 ) ) continue;

          GraalDeleteRectangle( Rectangle );
          GraalRejectRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );
     
          Element = (void *)

            addphcon( GraalFigureMbk,
                      ((phcon_list *)Pointer)->ORIENT,
                      Rectangle->NAME,
                      X1, Y1,
                      ((phcon_list *)Pointer)->LAYER,
                      ((phcon_list *)Pointer)->WIDTH );

          ((phcon_list *)Element)->USER = (void *)(&GraalFigureMbk->PHCON);

          if ( ((phcon_list *)Element)->NEXT != (phcon_list *)NULL )
          {
            ((phcon_list *)Element)->NEXT->USER = (void *)(&((phcon_list *)Element)->NEXT);
          }

          NewRec = GraalAddConnector( ((phcon_list *)Element) );
          GraalAddUndoRec( NewRec );
        }
      }

      break;
    }
  }

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( ! IsGraalAccepted( Rectangle ) ) continue;

    if ( IsRdsSegment( Rectangle ) )
    {
      if ( FirstUndo )
      {
        GraalAddUndo();

        FirstUndo = 0;
      }

      GraalDeleteRectangle( Rectangle );
      GraalAddUndoRec( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phseg_list *)Pointer)->X1 + DeltaX;
      Y1 = ((phseg_list *)Pointer)->Y1 + DeltaY;
      X2 = ((phseg_list *)Pointer)->X2 + DeltaX;
      Y2 = ((phseg_list *)Pointer)->Y2 + DeltaY;

      Element = (void *)

         addphseg( GraalFigureMbk, 
                   ((phseg_list *)Pointer)->LAYER,
                   ((phseg_list *)Pointer)->WIDTH,
                   X1, Y1, X2, Y2,
                   ((phseg_list *)Pointer)->NAME );

      ((phseg_list *)Element)->TYPE = ((phseg_list *)Pointer)->TYPE;
      ((phseg_list *)Element)->USER = (void *)(&GraalFigureMbk->PHSEG);

      if ( ((phseg_list *)Element)->NEXT != (phseg_list *)NULL )
      {
        ((phseg_list *)Element)->NEXT->USER = (void *)(&((phseg_list *)Element)->NEXT);
      }

      NewRec = GraalAddSegment( ((phseg_list *)Element) );
      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsVia( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      GraalDeleteRectangle( Rectangle );
      GraalAddUndoRec( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phvia_list *)Pointer)->XVIA + DeltaX;
      Y1 = ((phvia_list *)Pointer)->YVIA + DeltaY;

      Element = (void *)

         addphvia( GraalFigureMbk,
                   ((phvia_list *)Pointer)->TYPE, X1, Y1,
                   ((phvia_list *)Pointer)->DX, ((phvia_list *)Pointer)->DY,
                   ((phvia_list *)Pointer)->NAME );

      ((phvia_list *)Element)->USER = (void *)(&GraalFigureMbk->PHVIA);

      if ( ((phvia_list *)Element)->NEXT != (phvia_list *)NULL )
      {
        ((phvia_list *)Element)->NEXT->USER = (void *)(&((phvia_list *)Element)->NEXT);
      }

      NewRec = GraalAddVia( ((phvia_list *)Element) );

      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsReference( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      GraalDeleteRectangle( Rectangle );
      GraalAddUndoRec( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phref_list *)Pointer)->XREF + DeltaX;
      Y1 = ((phref_list *)Pointer)->YREF + DeltaY;

      Element = (void *)

         addphref( GraalFigureMbk,
                   ((phref_list *)Pointer)->FIGNAME,
                   Rectangle->NAME, X1, Y1 );

      ((phref_list *)Element)->USER = (void *)(&GraalFigureMbk->PHREF);

      if ( ((phref_list *)Element)->NEXT != (phref_list *)NULL )
      {
        ((phref_list *)Element)->NEXT->USER = (void *)(&((phref_list *)Element)->NEXT);
      }

      NewRec = GraalAddReference( ((phvia_list *)Element) );

      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsConnector( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      GraalDeleteRectangle( Rectangle );
      GraalAddUndoRec( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phcon_list *)Pointer)->XCON + DeltaX;
      Y1 = ((phcon_list *)Pointer)->YCON + DeltaY;

      Element = (void *)

        addphcon( GraalFigureMbk,
                 ((phcon_list *)Pointer)->ORIENT,
                  Rectangle->NAME, 
                  X1, Y1,
                  ((phcon_list *)Pointer)->LAYER,
                  ((phcon_list *)Pointer)->WIDTH );

      ((phcon_list *)Element)->USER = (void *)(&GraalFigureMbk->PHCON);

      if ( ((phcon_list *)Element)->NEXT != (phcon_list *)NULL )
      {
        ((phcon_list *)Element)->NEXT->USER = (void *)(&((phcon_list *)Element)->NEXT);
      }

      NewRec = GraalAddConnector( ((phcon_list *)Element) );
 
      GraalAddUndoRec( NewRec );
    }
    else
    if ( IsRdsInstance( Rectangle ) )
    {
      if ( FirstUndo )
      { 
        GraalAddUndo(); 

        FirstUndo = 0; 
      }

      GraalDeleteRectangle( Rectangle );
      GraalAddUndoRec( Rectangle );

      Pointer = GRAAL_MBK( Rectangle );

      X1 = ((phins_list *)Pointer)->XINS + DeltaX;
      Y1 = ((phins_list *)Pointer)->YINS + DeltaY;

      SaveInstance          = GraalFigureMbk->PHINS;
      GraalFigureMbk->PHINS = (phins_list *)NULL;

      Element = (void *)

         addphins( GraalFigureMbk,
                   ((phins_list *)Pointer)->FIGNAME,
                   ((phins_list *)Pointer)->INSNAME,
                   ((phins_list *)Pointer)->TRANSF,
                   X1, Y1 );

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
        GraalErrorMessage( GraalMainWindow, "Unable to load instance model !" );
      }
      else
      {
        NewRec   = Instance->LAYERTAB[ RDS_ABOX ];

        GraalAddUndoRec( NewRec );
      }
    }
  }

  GraalDelEqui();
  GraalDelPeek();
  GraalDelSelect();
  GraalZoomRefresh();

  if ( Mode == 0 )
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_MOVE,
                         GraalPromptSelectPointMove );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_MOVE,
                         GraalPromptSelectWindowMove );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalEditDelete                      |
|                                                             |
\------------------------------------------------------------*/

void GraalEditDelete()
{
  graalselect *Select;
  rdsrec_list *Rectangle;
  char         FirstUndo;
  graalconrec *ConRec;

  rdsbegin();

  FirstUndo = 1;

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    if ( IsRdsFigure( Select->RECTANGLE ) )
    {
      FirstUndo = 0;

      GraalAddUndo();
      GraalDeleteRectangle( Select->RECTANGLE );
      GraalAddUndoRec( Select->RECTANGLE );

      GraalFigureMbk->XAB1 = 0;
      GraalFigureMbk->XAB2 = 0;
      GraalFigureMbk->YAB1 = 0;
      GraalFigureMbk->YAB2 = 0;

      for ( ConRec  = GraalHeadConRec;
            ConRec != (graalconrec *)NULL;
            ConRec  = ConRec->NEXT )
      {
        Rectangle = ConRec->RECTANGLE;

        if ( ! IsGraalDeleted( Rectangle ) )
        {
          GraalRejectRectangle( Rectangle );
          GraalDeleteRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );
        }
      }

      break;
    }
  }

  for ( Select  = GraalHeadSelect;
        Select != (graalselect *)NULL;
        Select  = Select->NEXT )
  {
    Rectangle = Select->RECTANGLE;

    if ( ! IsGraalAccepted( Rectangle ) ) continue;

    if ( ( IsRdsSegment( Rectangle   ) ) ||
         ( IsRdsVia( Rectangle       ) ) ||
         ( IsRdsReference( Rectangle ) ) ||
         ( IsRdsConnector( Rectangle ) ) ||
         ( IsRdsInstance( Rectangle  ) ) ) 
    {
      if ( FirstUndo ) 
      {
        GraalAddUndo();
        FirstUndo = 0;
      }

      GraalDeleteRectangle( Rectangle );
      GraalAddUndoRec( Rectangle );
    }
  }

  GraalDelEqui();
  GraalDelPeek();
  GraalDelSelect();
  GraalZoomRefresh();

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalEditStretch                      |
|                                                             |
\------------------------------------------------------------*/

void GraalEditStretch( LambdaX1, LambdaY1, LambdaX2, LambdaY2, Mode )

  long LambdaX1;
  long LambdaY1;
  long LambdaX2;
  long LambdaY2;
  char Mode;
{
  graalselect *Select;
  rdsrec_list *Rectangle;
  rdsrec_list *NewRec;
  graalconrec *ConRec;
  void        *Element;
  void        *Pointer;
  char         FirstUndo;
  char         Orient;
  char         Stretch;
  long         Delta;
  long         Delta1;
  long         Delta2;
  long         Length;
  long         X1;
  long         X2;
  long         Y1;
  long         Y2;
  long         Xab1;
  long         Xab2;
  long         Yab1;
  long         Yab2;
  long         Xvia;
  long         Yvia;
  long         DXvia;
  long         DYvia;

  rdsbegin();

  Orient = 0;

  LambdaX1 = (long)( LambdaX1 * GRAAL_SCALE_X );
  LambdaX2 = (long)( LambdaX2 * GRAAL_SCALE_X );
  LambdaY1 = (long)( LambdaY1 * GRAAL_SCALE_X );
  LambdaY2 = (long)( LambdaY2 * GRAAL_SCALE_X );

  if ( LambdaX2 != LambdaX1 )
  {
    Delta  = LambdaX2 - LambdaX1;
    Orient = ( Delta > 0 ) ? GRAAL_EAST : GRAAL_WEST;
  }
  else
  {
    Delta  = LambdaY2 - LambdaY1;
    Orient = ( Delta > 0 ) ? GRAAL_NORTH : GRAAL_SOUTH;
  }
 
  if ( Delta != 0 )
  {
    FirstUndo = 1;

    for ( Select  = GraalHeadSelect;
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    { 
      Rectangle = Select->RECTANGLE;

      if ( IsRdsFigure( Rectangle ) ) 
      {
        X1 = GraalFigureMbk->XAB1;
        X2 = GraalFigureMbk->XAB2;
        Y1 = GraalFigureMbk->YAB1;
        Y2 = GraalFigureMbk->YAB2;

        Stretch = 0;

        if ( ( Orient == GRAAL_EAST ) ||
             ( Orient == GRAAL_WEST ) )
        {
          Delta1 = X1 - LambdaX1;
          Delta2 = X2 - LambdaX1;

          if ( Delta1 < 0 ) Delta1 = -Delta1;
          if ( Delta2 < 0 ) Delta2 = -Delta2;

          if ( Delta1 < Delta2 ) X1 += Delta;
          else                   X2 += Delta;

          if ( X2 > X1 )
          {
            Stretch = 1;
          }
        }
        else
        {
          Delta1 = Y1 - LambdaY1;
          Delta2 = Y2 - LambdaY1;

          if ( Delta1 < 0 ) Delta1 = -Delta1;
          if ( Delta2 < 0 ) Delta2 = -Delta2;

          if ( Delta1 < Delta2 ) Y1 += Delta;
          else                   Y2 += Delta;

          if ( Y2 > Y1 )
          {
            Stretch = 1;
          }
        }

        if ( Stretch )
        {
          if ( FirstUndo )
          {
            GraalAddUndo();

            FirstUndo = 0;
          }

          GraalDeleteRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );


          Xab1 = GraalFigureMbk->XAB1;
          Xab2 = GraalFigureMbk->XAB2;
          Yab1 = GraalFigureMbk->YAB1;
          Yab2 = GraalFigureMbk->YAB2;

          GraalFigureMbk->XAB1 = X1;
          GraalFigureMbk->YAB1 = Y1;
          GraalFigureMbk->XAB2 = X2;
          GraalFigureMbk->YAB2 = Y2;

          NewRec = GraalAddAbox();
          GraalAddUndoRec( NewRec );
     
          for ( ConRec  = GraalHeadConRec;
                ConRec != (graalconrec *)NULL;
                ConRec  = ConRec->NEXT )
          {
            Rectangle = ConRec->RECTANGLE;

            if ( ! IsGraalDeleted( Rectangle ) ) 
            {
              Stretch = 0;
              Pointer = GRAAL_MBK( Rectangle );

              X1 = ((phcon_list *)Pointer)->XCON;
              Y1 = ((phcon_list *)Pointer)->YCON;

              if ( ( X1 < Xab1 ) ||
                   ( X1 > Xab2 ) ||
                   ( Y1 < Yab1 ) ||
                   ( Y1 > Yab2 ) ) continue;

              if ( ( X1 != Xab1 ) &&
                   ( X1 != Xab2 ) &&
                   ( Y1 != Yab1 ) &&
                   ( Y1 != Yab2 ) ) continue;

              if ( ( X1 != GraalFigureMbk->XAB1 ) &&
                   ( X1 != GraalFigureMbk->XAB2 ) &&
                   ( Y1 != GraalFigureMbk->YAB1 ) &&
                   ( Y1 != GraalFigureMbk->YAB2 ) )
              {
                switch ( Orient )
                {
                  case GRAAL_SOUTH:
                  case GRAAL_NORTH: Y1 += Delta;

                  break;

                  case GRAAL_WEST:
                  case GRAAL_EAST:  X1 += Delta;

                  break;
                }
       
                Stretch = 1;
              }

              if ( X1 < GraalFigureMbk->XAB1 )
              {
                X1 = GraalFigureMbk->XAB1; Stretch = 1;
              }

              if ( X1 > GraalFigureMbk->XAB2 )
              {
                X1 = GraalFigureMbk->XAB2; Stretch = 1;
              }

              if ( Y1 < GraalFigureMbk->YAB1 )
              {
                Y1 = GraalFigureMbk->YAB1; Stretch = 1;
              }

              if ( Y1 > GraalFigureMbk->YAB2 )
              {
                Y1 = GraalFigureMbk->YAB2; Stretch = 1;
              }

              if ( Stretch )
              {
                GraalDeleteRectangle( Rectangle );
                GraalRejectRectangle( Rectangle );
                GraalAddUndoRec( Rectangle );
     
                Element = (void *)

                  addphcon( GraalFigureMbk,
                            ((phcon_list *)Pointer)->ORIENT,
                            Rectangle->NAME,
                            X1, Y1,
                            ((phcon_list *)Pointer)->LAYER,
                            ((phcon_list *)Pointer)->WIDTH );

                ((phcon_list *)Element)->USER = (void *)(&GraalFigureMbk->PHCON);

                if ( ((phcon_list *)Element)->NEXT != (phcon_list *)NULL )
                {
                  ((phcon_list *)Element)->NEXT->USER = 

                         (void *)(&((phcon_list *)Element)->NEXT);
                }

                NewRec = GraalAddConnector( ((phcon_list *)Element) );
                GraalAddUndoRec( NewRec );
              }
            }
          }
        }

        break;
      }
    }

    for ( Select  = GraalHeadSelect;
          Select != (graalselect *)NULL;
          Select  = Select->NEXT )
    {
      Rectangle = Select->RECTANGLE;

      if ( IsRdsSegment( Rectangle ) )
      {
        Pointer = GRAAL_MBK( Rectangle );

        X1 = ((phseg_list *)Pointer)->X1;
        Y1 = ((phseg_list *)Pointer)->Y1;
        X2 = ((phseg_list *)Pointer)->X2;
        Y2 = ((phseg_list *)Pointer)->Y2;

        Length = (long)( GRAAL_SEGMENT_VALUE_TABLE[ (int)((phseg_list *)Pointer)->LAYER ][1] * SCALE_X );

        Stretch = 0;

        switch ( ((phseg_list *)Pointer)->TYPE )
        {
          case RIGHT :
          case LEFT  :

            if ( ( Orient == GRAAL_EAST ) || 
                 ( Orient == GRAAL_WEST ) )
            {
              Delta1 = X1 - LambdaX1;
              Delta2 = X2 - LambdaX1;

              if ( Delta1 < 0 ) Delta1 = -Delta1;
              if ( Delta2 < 0 ) Delta2 = -Delta2;

              if ( Delta1 < Delta2 ) X1 += Delta;
              else                   X2 += Delta;

              if ( ( X2 - X1 ) >= Length )
              {
                Stretch = 1;
              }
            }

            break;

          case UP    :
          case DOWN  :

            if ( ( Orient == GRAAL_NORTH ) || 
                 ( Orient == GRAAL_SOUTH ) )
            {
              Delta1 = Y1 - LambdaY1;
              Delta2 = Y2 - LambdaY1;

              if ( Delta1 < 0 ) Delta1 = -Delta1;
              if ( Delta2 < 0 ) Delta2 = -Delta2;

              if ( Delta1 < Delta2 ) Y1 += Delta;
              else                   Y2 += Delta;

              if ( ( Y2 - Y1 ) >= Length )
              {
                Stretch = 1;
              }
            }

            break;
        }

        if ( Stretch ) 
        {
          if ( FirstUndo )
          {
            GraalAddUndo();

            FirstUndo = 0;
          }

          GraalDeleteRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );

          Element = (void *)

             addphseg( GraalFigureMbk, 
                       ((phseg_list *)Pointer)->LAYER,
                       ((phseg_list *)Pointer)->WIDTH,
                       X1, Y1, X2, Y2,
                       ((phseg_list *)Pointer)->NAME );

          ((phseg_list *)Element)->TYPE = ((phseg_list *)Pointer)->TYPE;
          ((phseg_list *)Element)->USER = (void *)(&GraalFigureMbk->PHSEG);

          if ( ((phseg_list *)Element)->NEXT != (phseg_list *)NULL )
          {
            ((phseg_list *)Element)->NEXT->USER = (void *)(&((phseg_list *)Element)->NEXT);
          }

          NewRec = GraalAddSegment( ((phseg_list *)Element) );
          GraalAddUndoRec( NewRec );
        }
      }
      else
      if ( IsRdsVia( Rectangle ) )
      {
        Pointer = GRAAL_MBK( Rectangle );

        Xvia  = ((phvia_list *)Pointer)->XVIA;
        Yvia  = ((phvia_list *)Pointer)->YVIA;
        DXvia = ((phvia_list *)Pointer)->DX;
        DYvia = ((phvia_list *)Pointer)->DY;

        X1 = Xvia - (DXvia >> 1);
        Y1 = Yvia - (DYvia >> 1);
        X2 = X1 + DXvia;
        Y2 = Y1 + DYvia;

        Length = (long)( GRAAL_BIGVIA_VALUE_TABLE[ (int)((phvia_list *)Pointer)->TYPE ][0] * SCALE_X );

        Stretch = 0;

        if ( ( Orient == GRAAL_EAST ) || 
             ( Orient == GRAAL_WEST ) )
        {
          Delta1 = X1 - LambdaX1;
          Delta2 = X2 - LambdaX1;

          if ( Delta1 < 0 ) Delta1 = -Delta1;
          if ( Delta2 < 0 ) Delta2 = -Delta2;

          if ( Delta1 < Delta2 ) X1 += Delta;
          else                   X2 += Delta;

          if ( ( X2 - X1 ) >= Length )
          {
            Stretch = 1;
          }
        }
        else
        if ( ( Orient == GRAAL_NORTH ) || 
             ( Orient == GRAAL_SOUTH ) )
        {
          Delta1 = Y1 - LambdaY1;
          Delta2 = Y2 - LambdaY1;

          if ( Delta1 < 0 ) Delta1 = -Delta1;
          if ( Delta2 < 0 ) Delta2 = -Delta2;

          if ( Delta1 < Delta2 ) Y1 += Delta;
          else                   Y2 += Delta;

          if ( ( Y2 - Y1 ) >= Length )
          {
            Stretch = 1;
          }
        }

        if ( Stretch ) 
        {
          DXvia = X2 - X1;
          DYvia = Y2 - Y1;
          Xvia  = X1 + (DXvia >> 1);
          Yvia  = Y1 + (DYvia >> 1);

          if ( FirstUndo )
          {
            GraalAddUndo();

            FirstUndo = 0;
          }

          GraalDeleteRectangle( Rectangle );
          GraalAddUndoRec( Rectangle );

          Element = (void *)

             addphvia( GraalFigureMbk,
                      ((phvia_list *)Pointer)->TYPE, Xvia, Yvia, DXvia, DYvia,
                      ((phvia_list *)Pointer)->NAME );

          ((phvia_list *)Element)->USER = (void *)(&GraalFigureMbk->PHVIA);

          if ( ((phvia_list *)Element)->NEXT != (phvia_list *)NULL )
          {
            ((phvia_list *)Element)->NEXT->USER = (void *)(&((phvia_list *)Element)->NEXT);
          }

          NewRec = GraalAddVia( ((phvia_list *)Element) );

          GraalAddUndoRec( NewRec );
        }
      }
    }
  } 

  GraalDelEqui();
  GraalDelPeek();
  GraalDelSelect();
  GraalZoomRefresh();

  if ( Mode == 0 )
  {
    GraalChangeEditMode( GRAAL_SELECT_POINT_STRETCH,
                         GraalPromptSelectPointStretch );
  }
  else
  {
    GraalChangeEditMode( GRAAL_SELECT_WINDOW_STRETCH,
                         GraalPromptSelectWindowStretch );
  }

  rdsend();
}
