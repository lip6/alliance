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
| Tool    :                   XSCH                            |
|                                                             |
| File    :                 XSC_place.c                       |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                        Miramond  Benoit                     |
| 			 Picault Stephane                     |
|                                                             |
| Date    :                   10.03.98                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h" 
# include "XSB.h"
# include "XSC.h"

# include "XSC_place.h"
# include "XSC_schem.h"

# define IsSchWireHorizontal( Z )    (  ( Z )->DY == ( Z )->Y )
# define IsSchWireVertical( Z )      (  ( Z )->DX == ( Z )->X )

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

  static authtable *XschHashBoxInfo  = (authtable *)0;
  static authtable *XschHashNetInfo  = (authtable *)0;
  static authtable *XschHashConInfo  = (authtable *)0;
  static authtable *XschHashPathInfo = (authtable *)0;

  static ptype_list    *XschLayerInfoList = (ptype_list    *)0;
  static xschpath_list *XschLayerPathList = (xschpath_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Xsch Add Layer Path                  |
|                                                             |
\------------------------------------------------------------*/

static xschpath_list *XschAddLayerPath( HeadList, From, To, Layer, String )

  xschpath_list *HeadList;
  char          *From;
  char          *To;
  long           Layer;
  char          *String;
{
  xschpath_list *NewPath;

  NewPath = (xschpath_list *)autallocheap( sizeof( xschpath_list ) );
  NewPath->NEXT    = HeadList;
  NewPath->FROM    = From;
  NewPath->TO      = To;
  NewPath->LAYER   = Layer;
  NewPath->COMMENT = String;

  return( NewPath );
}

/*------------------------------------------------------------\
|                                                             |
|                        Xsch Free Layer Path                 |
|                                                             |
\------------------------------------------------------------*/

static void XschFreeLayerPath( HeadList )

  xschpath_list *HeadList;
{
  xschpath_list *ScanPath;
  xschpath_list *DelPath;

  ScanPath = HeadList;

  while ( ScanPath != (xschpath_list *)0 )
  {
    DelPath  = ScanPath;
    ScanPath = ScanPath->NEXT;

    autfreeheap( DelPath, sizeof( xschpath_list ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Xsch Parse Layer Info                |
|                                                             |
\------------------------------------------------------------*/

static void XschParseLayerInfo( SchFig )

  schfig_list *SchFig;
{
  FILE       *File;
  long        Layer; 
  char       *String;
  char       *From;
  char       *To;
  char       *Name;
  char       *ScanString;
  char       *ScanLayer;
  char       *ScanName;
  char       *ScanFrom;
  char       *ScanTo;
  char        Buffer[ 512 ];

  File = mbkfopen( SchFig->NAME, "xsc", "r" );

  if ( File == (FILE *)0 ) return;

  while ( fgets( Buffer, 512, File ) != (char *)0 )
  {
    if ( ( Buffer[ 0 ] == 'B' ) ||
         ( Buffer[ 0 ] == 'N' ) ||
         ( Buffer[ 0 ] == 'C' ) )
    {
      ScanName = strchr( Buffer, ':' );
      if ( ScanName == (char *)0 ) continue;

      ScanLayer = strchr( ScanName + 1, ':' );
      if ( ScanLayer == (char *)0 ) continue;

      ScanString = strchr( ScanLayer + 1, ':' );
      if ( ScanString == (char *)0 ) continue;

      *ScanName   = '\0';
      *ScanLayer  = '\0';
      *ScanString = '\0';

      Layer  = atol( ScanLayer + 1 );

      Name   = namealloc( ScanName   + 1 );

      ScanName = strchr( ScanString + 1, '\n' );
      if ( ScanName != (char *)0 ) *ScanName = '\0';

      String = autnamealloc( ScanString + 1 );

      if ( ( Layer <  0  ) ||
           ( Layer >  32 ) )
      {
        Layer = -1;
      }

      XschLayerInfoList = addptype( XschLayerInfoList, Layer, String );

      if ( Buffer[ 0 ] == 'B' )
      {
        addauthelem( XschHashBoxInfo, Name, (long)XschLayerInfoList );
      }
      else
      if ( Buffer[ 0 ] == 'N' )
      {
        addauthelem( XschHashNetInfo, Name, (long)XschLayerInfoList );
      }
      else
      {
        addauthelem( XschHashConInfo, Name, (long)XschLayerInfoList );
      }
    }
    else
    if ( Buffer[ 0 ] == 'P' )
    {
      ScanFrom = strchr( Buffer, ':' );
      if ( ScanFrom == (char *)0 ) continue;

      ScanName = strchr( ScanFrom + 1, ':' );
      if ( ScanName == (char *)0 ) continue;

      ScanTo = strchr( ScanName + 1, ':' );
      if ( ScanTo == (char *)0 ) continue;

      ScanLayer = strchr( ScanTo + 1, ':' );
      if ( ScanLayer == (char *)0 ) continue;

      ScanString = strchr( ScanLayer + 1, ':' );
      if ( ScanString == (char *)0 ) continue;

      *ScanFrom   = '\0';
      *ScanName   = '\0';
      *ScanTo     = '\0';
      *ScanLayer  = '\0';
      *ScanString = '\0';

      Layer  = atol( ScanLayer + 1 );

      From   = namealloc( ScanFrom + 1 );
      Name   = namealloc( ScanName + 1 );
      To     = namealloc( ScanTo   + 1 );

      ScanName = strchr( ScanString + 1, '\n' );
      if ( ScanName != (char *)0 ) *ScanName = '\0';

      String = autnamealloc( ScanString + 1 );

      if ( ( Layer <  0  ) ||
           ( Layer >  32 ) )
      {
        Layer = -1;
      }

      XschLayerPathList = XschAddLayerPath( XschLayerPathList, From, To, Layer, String );

      addauthelem( XschHashPathInfo, Name, (long)XschLayerPathList );
    }
  }

  fclose( File );
}

/*------------------------------------------------------------\
|                                                             |
|                           Place Con                         |
|                                                             |
\------------------------------------------------------------*/

void XschPlaceConnector( HeadList, X, Y, LayerCon ) 

   schcon_list   *HeadList;
   long           X;
   long           Y;
   unsigned char  LayerCon;
{
  authelem      *Element;
  schcon_list   *SchCon;
  xschobj_list  *Object1;
  xschobj_list  *Object2;
  ptype_list    *InfoPType;
  long           X_trace;
  long           X_con;
  long           Y_trace;
  long           Y_con;
  int            ObjectType;
  long           DeltaX;
  long           DeltaY;
  unsigned char  Layer;

  for ( SchCon  = HeadList;
        SchCon != (schcon_list *)0;
        SchCon  = SchCon->NEXT )
  {
    Layer = LayerCon;

    Element = searchauthelem( XschHashConInfo, SchCon->NAME );

    if ( Element != (authelem *)0 )
    {
      InfoPType    = (ptype_list *)Element->VALUE;
      SchCon->USER = (void *)InfoPType->DATA;

      if ( InfoPType->TYPE != -1 ) Layer = InfoPType->TYPE + XSCH_FIRST_USER_LAYER;  
    }
    else
    {
      SchCon->USER = (void *)0;
    }

    X_con = ( SchCon->X_REL * XSCH_UNIT ); 
    Y_con = ( SchCon->Y_REL * XSCH_UNIT ) - XSCH_UNIT; 

    if ( IsSchConIn( SchCon ) )
    {
      ObjectType = XSCH_OBJECT_TEXT_LEFT;
      DeltaX     = - XSCH_UNIT;
    }
    else
    {
      ObjectType = XSCH_OBJECT_TEXT_RIGHT;
      DeltaX     = XSCH_UNIT;
    }

    X_trace =  X_con + X;
    Y_trace =  Y_con + Y; 

    if ( IsSchConExternal( SchCon ) )
    {
      DeltaX *= 2;
      DeltaY  = 0;

      if ( IsSchConIn( SchCon ) )
      {
        X_trace -= SCP_PRIMARY_CON_DX * XSCH_UNIT;
      }

      Object1 = XschAddPentagon( X_trace, Y_trace, 
                                 SCP_PRIMARY_CON_DX * XSCH_UNIT,
                                 SCP_PRIMARY_CON_DY * XSCH_UNIT, Layer );

      if ( IsSchConOut( SchCon ) )
      {
        X_trace += SCP_PRIMARY_CON_DX * XSCH_UNIT;
      }
    }
    else
    {
      Object1 = (xschobj_list *)0;
      DeltaY  = - XSCH_UNIT;
    }

    Object2 = XschAddText( X_trace + DeltaX,
                           Y_trace + DeltaY + ((SCP_PRIMARY_CON_DY * XSCH_UNIT) / 2 ),
                           ObjectType, SchCon->NAME, Layer );

    if ( Object1 == (xschobj_list *)0 )
    {
      Object1 = Object2;
    }

    Object1->LINK = Object2;
    Object2->LINK = Object1;
    
    Object1->USER = (void *)SchCon;
    Object2->USER = (void *)SchCon;
    
    SetXschSchCon( Object1 );
    SetXschSchCon( Object2 );

    if ( IsSchConExternal( SchCon ) )
    {
      SetXschSchConExt( Object1 );
      SetXschSchConExt( Object2 );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XschPlaceCutWire                   |
|                                                             |
\------------------------------------------------------------*/

static schwir_list *XschPlaceCutWire( HeadWire )

  schwir_list *HeadWire;
{
  schwir_list *SchWire1;
  schwir_list *SchWire2;
  schwir_list *SchWire3;

  for ( SchWire1  = HeadWire;
        SchWire1 != (schwir_list *)0;
        SchWire1  = SchWire1->NEXT )
  {
    for ( SchWire2  = HeadWire;
          SchWire2 != (schwir_list *)0;
          SchWire2  = SchWire2->NEXT )
    {
      if ( ( IsSchWireHorizontal( SchWire1 ) ) &&
           ( IsSchWireVertical( SchWire2   ) ) )
      {
        if ( (   SchWire1->Y <=   SchWire2->Y  ) ||
             ( ( SchWire1->Y >= ( SchWire2->DY ) ) ) ) continue;

        if ( ( SchWire2->X >= SchWire1->X  ) &&
             ( SchWire2->X <= SchWire1->DX ) )
        {
          /* Split SchWire2 */

          SchWire3 = allocschwir();
          SchWire3->NET   = SchWire2->NET;
          SchWire3->FLAGS = SchWire2->FLAGS;
          SchWire3->USER  = SchWire2->USER;
          SchWire3->X     = SchWire2->X;
          SchWire3->DX    = SchWire2->DX;

          SchWire3->Y  = SchWire1->Y;
          SchWire3->DY = SchWire2->DY;

          SchWire2->DY = SchWire1->Y;

          SchWire3->NEXT = SchWire2->NEXT;
          SchWire2->NEXT = SchWire3;
        }
      }
      else
      if ( ( IsSchWireVertical( SchWire1 ) ) &&
           ( IsSchWireHorizontal( SchWire2 ) ) )
      {
        if ( (   SchWire1->X <=   SchWire2->X  ) ||
             ( ( SchWire1->X >= ( SchWire2->DX ) ) ) ) continue;

        if ( ( SchWire2->Y >= SchWire1->Y  ) &&
             ( SchWire2->Y <= SchWire1->DY ) )
        {
          /* Split SchWire2 */

          SchWire3 = allocschwir();
          SchWire3->NET   = SchWire2->NET;
          SchWire3->FLAGS = SchWire2->FLAGS;
          SchWire3->USER  = SchWire2->USER;
          SchWire3->Y     = SchWire2->Y;
          SchWire3->DY    = SchWire2->DY;

          SchWire3->X  = SchWire1->X;
          SchWire3->DX = SchWire2->DX;

          SchWire2->DX = SchWire1->X;

          SchWire3->NEXT = SchWire2->NEXT;
          SchWire2->NEXT = SchWire3;
        }
      }
    }
  }

  return( HeadWire );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschPlaceScanWirePath              |
|                                                             |
\------------------------------------------------------------*/

static int XschPlaceScanWirePath( SchNet, X, Y, SchLastWire )

  schnet_list *SchNet;
  long         X;
  long         Y;
  schwir_list *SchLastWire;
{
  schwir_list *SchWire;
  int          Found;

  for ( SchWire  = SchNet->WIRE;
        SchWire != (schwir_list *)0;
        SchWire  = SchWire->NEXT )
  {
    if ( IsXschWireTag( SchWire ) ) continue;

    if ( ( SchWire->X == X ) &&
         ( SchWire->Y == Y ) )
    {
      SetXschWireTag( SchWire );

      if ( SchWire == SchLastWire )
      {
        Found = 1;
      }
      else
      {
        Found = XschPlaceScanWirePath( SchNet, SchWire->DX, SchWire->DY, SchLastWire );
      }

      if ( Found )
      {
        SetXschWirePath( SchWire );

        return( 1 );
      }
    }
    else
    if ( ( SchWire->DX == X ) &&
         ( SchWire->DY == Y ) )
    {
      SetXschWireTag( SchWire );

      if ( SchWire == SchLastWire )
      {
        Found = 1;
      }
      else
      {
        Found = XschPlaceScanWirePath( SchNet, SchWire->X, SchWire->Y, SchLastWire );
      }

      if ( Found )
      {
        SetXschWirePath( SchWire );

        return( 1 );
      }
    }
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                          XschPlaceTreatPath                 |
|                                                             |
\------------------------------------------------------------*/

static void XschPlaceTreatPath( SchFig, SchNet, PathInfo )

  schfig_list   *SchFig;
  schnet_list   *SchNet;
  xschpath_list *PathInfo;
{
  schbox_list *SchBox;
  schcon_list *SchCon;
  schwir_list *SchWire;
  schwir_list *SchLastWire;
  schwir_list *SchFirstWire;
/*
** Cut wire
*/
  SchNet->WIRE = XschPlaceCutWire( SchNet->WIRE );
/*
** Find the first wire !
*/
  SchFirstWire = (schwir_list *)0;

  for ( SchBox  = SchFig->BOX;
        SchBox != (schbox_list *)0;
        SchBox  = SchBox->NEXT )
  {
    if ( SchBox->NAME == PathInfo->FROM )
    {
      for ( SchWire  = SchNet->WIRE;
            SchWire != (schwir_list *)0;
            SchWire  = SchWire->NEXT )
      {
        if ( ( SchWire->X  == ( SchBox->X + SchBox->DX ) ) &&
             ( SchWire->Y  >= SchBox->Y                  ) &&
             ( SchWire->DY <= ( SchBox->Y + SchBox->DY ) ) )
        {
          SchFirstWire = SchWire; break;
        }
      }
    }

    if ( SchFirstWire != (schwir_list *)0 ) break;
  }

  if ( SchFirstWire == (schwir_list *)0 )
  {
    for ( SchCon  = SchFig->CON_IN;
          SchCon != (schcon_list *)0;
          SchCon  = SchCon->NEXT )
    {
      if ( PathInfo->FROM == SchCon->NAME )
      {
        for ( SchWire  = SchNet->WIRE;
              SchWire != (schwir_list *)0;
              SchWire  = SchWire->NEXT )
        {
          if ( ( SchWire->X  == ( SchCon->X_REL + SchFig->X ) ) &&
               ( SchWire->Y  == ( SchCon->Y_REL + SchFig->Y ) ) )
          {
            SchFirstWire = SchWire; break;
          }
        }
      }

      if ( SchFirstWire != (schwir_list *)0 ) break;
    }
  }

  if ( SchFirstWire == (schwir_list *)0 ) return;
  SetXschWirePath( SchFirstWire );
/*
** Find the last wire !
*/
  SchLastWire  = (schwir_list *)0;

  for ( SchBox  = SchFig->BOX;
        SchBox != (schbox_list *)0;
        SchBox  = SchBox->NEXT )
  {
    if ( SchBox->NAME == PathInfo->TO )
    {
      for ( SchWire  = SchNet->WIRE;
            SchWire != (schwir_list *)0;
            SchWire  = SchWire->NEXT )
      {
        if ( ( SchWire->DX == SchBox->X                  ) &&
             ( SchWire->Y  >= SchBox->Y                  ) &&
             ( SchWire->DY <= ( SchBox->Y + SchBox->DY ) ) )
        {
          SchLastWire = SchWire; break;
        }
      }
    }

    if ( SchLastWire != (schwir_list *)0 ) break;
  }

  if ( SchLastWire == (schwir_list *)0 )
  {
    for ( SchCon  = SchFig->CON_OUT;
          SchCon != (schcon_list *)0;
          SchCon  = SchCon->NEXT )
    {
      if ( PathInfo->TO == SchCon->NAME )
      {
        for ( SchWire  = SchNet->WIRE;
              SchWire != (schwir_list *)0;
              SchWire  = SchWire->NEXT )
        {
          if ( ( SchWire->DX == ( SchCon->X_REL + SchFig->X ) ) &&
               ( SchWire->DY == ( SchCon->Y_REL + SchFig->Y ) ) )
          {
            SchLastWire = SchWire; break;
          }
        }
      }

      if ( SchLastWire != (schwir_list *)0 ) break;
    }
  }

  if ( SchLastWire == (schwir_list *)0 ) return;
  SetXschWirePath( SchLastWire  );

  if ( SchLastWire == SchFirstWire ) return;

  XschPlaceScanWirePath( SchNet, SchFirstWire->DX, SchFirstWire->DY, SchLastWire );

  for ( SchWire  = SchNet->WIRE;
        SchWire != (schwir_list *)0;
        SchWire  = SchWire->NEXT )
  {
    ClearXschWireTag( SchWire );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          XschPlace                          |
|                                                             |
\------------------------------------------------------------*/

void XschPlaceFigure( SchFig )

  schfig_list *SchFig; 
{
  schbox_list   *SchBox;
  schnet_list   *SchNet;
  schwir_list   *SchWir;
  schwir_list   *ScanWir;
  xschobj_list  *Object;
  xschobj_list  *FirstObj;
  xschobj_list  *PrevObj;
  ptype_list    *InfoPType;
  xschpath_list *PathInfo;
  authelem      *Element;
  xschicon      *XschIcon;
  loins_list    *LoIns;
  lotrs_list    *LoTrs;
  losig_list    *LoSig;
  char          *ModelName;
  char          *WireName;
  char          *Comment;
  long           X_trace;
  long           X_fig;
  long           X_box;
  long           X_wir;
  long           Dx;
  long           DX_wir;
  long           MaxY_wire;
  long           MinY_wire;
  long           Y_trace;
  long           Y_fig;
  long           Y_box;
  long           Y_wir;
  long           Dy;
  long           DY_wir;
  unsigned char  Layer;
  unsigned char  WireLayer;
  unsigned char  PathMode;
  long           cptmax_wire;
  long           cptmin_wire;
  
  cptmax_wire = 0;
  cptmin_wire = 0;

  X_fig = 0;
  Y_fig = 0;

  XschLayerInfoList = (ptype_list *)0;

  if ( XschHashBoxInfo == (authtable *)0 )
  {
    XschHashBoxInfo   = createauthtable( 100 );
    XschHashNetInfo   = createauthtable( 100 );
    XschHashConInfo   = createauthtable( 100 );
    XschHashPathInfo  = createauthtable( 100 );
  }

  XschParseLayerInfo( SchFig );

  XschPlaceConnector( SchFig->CON_IN , X_fig, Y_fig, XSCH_SCHCONIN_LAYER  );
  XschPlaceConnector( SchFig->CON_OUT, X_fig, Y_fig, XSCH_SCHCONOUT_LAYER );
/*
** Place all nets
*/
  for ( SchNet  = SchFig->NET;
        SchNet != (schnet_list *)0;
        SchNet  = SchNet->NEXT )
  {
    WireName  = (char *)0;
    Comment   = (char *)0;
    PathMode  = 0;
    Layer     = XSCH_SCHNET_LAYER;

    if ( SchNet->SOURCE_TYPE == SCH_SOURCE_LOSIG )
    {
      LoSig = (losig_list *)SchNet->SOURCE;

      if ( LoSig->NAMECHAIN != (chain_list *)0 )
      {
        WireName = (char *)LoSig->NAMECHAIN->DATA;
      }
    }

    if ( WireName != (char *)0 )
    {
      Element = searchauthelem( XschHashNetInfo, WireName );

      if ( Element != (authelem *)0 )
      {
        InfoPType = (ptype_list *)Element->VALUE;
        Comment   = (char *)InfoPType->DATA;

        if ( InfoPType->TYPE != -1 ) Layer = InfoPType->TYPE + XSCH_FIRST_USER_LAYER;  
      }

      Element = searchauthelem( XschHashPathInfo, WireName );

      if ( Element != (authelem *)0 )
      {
        PathInfo  = (xschpath_list *)Element->VALUE;
        PathMode  = 1;

        Comment = PathInfo->COMMENT;
      }
    }

    SchNet->USER = (void *)Comment;

    FirstObj = NULL;
    PrevObj  = NULL;

# if 0
    fprintf( stdout, "--> NET !\n" );
# endif

    for ( SchWir  = SchNet->WIRE;
          SchWir != (schwir_list *)0;
          SchWir  = SchWir->NEXT )
    {
# if 0
      fprintf( stdout, "Wire (%ld %ld) (%ld %ld)\n", 
          SchWir->X, SchWir->Y, SchWir->DX, SchWir->DY );
# endif
      X_wir   = SchWir->X * XSCH_UNIT;
      Y_wir   = SchWir->Y * XSCH_UNIT;
      DX_wir  = SchWir->DX * XSCH_UNIT;
      DY_wir  = SchWir->DY * XSCH_UNIT;

      X_trace = X_fig + X_wir;
      Y_trace = Y_fig + Y_wir;

      if ( IsSchWireVertical(SchWir) )
      {
        cptmax_wire=0;
        cptmin_wire=0;
 
        if (SchWir->Y < SchWir->DY) 
        { 
          MaxY_wire = SchWir->DY;
          MinY_wire = SchWir->Y;
        } 
        else
        { 
          MaxY_wire = SchWir->Y;
          MinY_wire = SchWir->DY;
        }

        for ( ScanWir  = SchNet->WIRE;
              ScanWir != (schwir_list *)0;
              ScanWir  = ScanWir->NEXT)
        {
          if ( IsSchWireHorizontal(ScanWir) )
          {
            if ( ( ScanWir->DX == SchWir->X ) || 
                 ( ScanWir->X  == SchWir->X ) )
            {
              if ( ( MinY_wire  < ScanWir->Y ) && 
                   ( ScanWir->Y < MaxY_wire  ) )
              {
                Object = XschAddCrossBox( X_trace - ( XSCH_UNIT / 2 ), 
                                       Y_fig + (ScanWir->DY * XSCH_UNIT ) - (XSCH_UNIT / 2),
                                       XSCH_UNIT, XSCH_UNIT, Layer );
                SetXschSchNet( Object );

                if ( PrevObj !=  NULL ) PrevObj->LINK = Object;            
                if ( FirstObj == NULL ) FirstObj = Object;

                Object->USER  = (void *)SchNet;
                PrevObj       = Object;
              }

              if ( MaxY_wire != MinY_wire )
              {
                if ( MaxY_wire == ScanWir->Y ) cptmax_wire++;
                if ( MinY_wire == ScanWir->Y ) cptmin_wire++;
              }
            }
          }
        }

        if ( cptmax_wire >= 2 )
        {
          Object = XschAddCrossBox( X_trace - ( XSCH_UNIT / 2 ), 
                                 Y_fig + ( MaxY_wire * XSCH_UNIT ) - (XSCH_UNIT / 2),
                                 XSCH_UNIT, XSCH_UNIT, Layer );
          SetXschSchNet( Object );

          Object->USER  = (void *)SchNet;
          if ( PrevObj != NULL ) PrevObj->LINK = Object;            
          PrevObj       = Object;
        }

        if (cptmin_wire >= 2)
        {
          Object = XschAddCrossBox( X_trace - ( XSCH_UNIT / 2 ),
                                    Y_fig + ( MinY_wire * XSCH_UNIT ) - (XSCH_UNIT / 2),
                                    XSCH_UNIT, XSCH_UNIT, Layer );
          SetXschSchNet( Object );

          if ( PrevObj !=  NULL ) PrevObj->LINK = Object;            
          if ( FirstObj == NULL ) FirstObj = Object;

          Object->USER  = (void *)SchNet;
          PrevObj       = Object;
        }
      }
      else
      if ( WireName != (char *)0 )
      {
        Object = XschAddTextWidth( X_trace,
            /* + ((DX_wir - X_wir ) >> 1), */
                                   Y_trace + XSCH_UNIT, DX_wir - X_wir,
                                   XSCH_OBJECT_TEXT_CENTER, 
                                   WireName, Layer );

        /*
        Object->DX = DX_wir - X_wir;
        */

        SetXschSchNet( Object );

        if ( PrevObj !=  NULL ) PrevObj->LINK = Object;            
        if ( FirstObj == NULL ) FirstObj = Object;

        Object->USER  = (void *)SchNet;
        PrevObj       = Object;
      }
    }
/*
** Special treatment for highlighted path !
*/
    if ( PathMode )
    {
      XschPlaceTreatPath( SchFig, SchNet, PathInfo );
    }

    for ( SchWir  = SchNet->WIRE;
          SchWir != (schwir_list *)0;
          SchWir  = SchWir->NEXT )
    {
      WireLayer = Layer;

      if ( ( PathMode                 ) &&
           ( IsXschWirePath( SchWir ) ) &&
           ( PathInfo->LAYER != -1    ) )
      {
        WireLayer = PathInfo->LAYER + XSCH_FIRST_USER_LAYER;
      }

      X_wir   = SchWir->X * XSCH_UNIT;
      Y_wir   = SchWir->Y * XSCH_UNIT;
      DX_wir  = SchWir->DX * XSCH_UNIT;
      DY_wir  = SchWir->DY * XSCH_UNIT;

      X_trace = X_fig + X_wir;
      Y_trace = Y_fig + Y_wir;

      Object = XschAddLine( X_trace, Y_trace, DX_wir+X_fig, DY_wir+Y_fig ,
                            WireLayer, WireLayer );
      SetXschSchNet( Object );

      if ( FirstObj == NULL ) FirstObj = Object;

      Object->USER = (void *)SchNet;
      Object->LINK = Object;            

      if ( PrevObj != NULL ) PrevObj->LINK = Object;

      PrevObj = Object;
    }

    if ( PrevObj != NULL ) PrevObj->LINK = FirstObj;
  }

/*
** Place all boxes
*/
  for ( SchBox  = SchFig->BOX;
        SchBox != (schbox_list *)0;
        SchBox  = SchBox->NEXT )
  {
    if ( ( IsSchBoxTransparence( SchBox ) ) ||
         ( IsSchBoxCluster( SchBox )      ) ||
         ( IsSchBoxClusterNet( SchBox )   ) )
    {
      Layer = XSCH_CONSTRUCTION_LAYER;
    }
    else
    {
      Layer = XSCH_SCHBOX_LAYER;
    }

    Element = searchauthelem( XschHashBoxInfo, SchBox->NAME );

    if ( Element != (authelem *)0 )
    {
      InfoPType    = (ptype_list *)Element->VALUE;
      SchBox->USER = (char *)InfoPType->DATA;

      if ( InfoPType->TYPE != -1 ) Layer = InfoPType->TYPE + XSCH_FIRST_USER_LAYER;  
    }
    else
    {
      SchBox->USER = (void *)0;
    }

    X_box  = SchBox->X  * XSCH_UNIT;
    Y_box  = SchBox->Y  * XSCH_UNIT;
    Dx     = SchBox->DX * XSCH_UNIT;
    Dy     = SchBox->DY * XSCH_UNIT;

    X_trace = X_box + X_fig;
    Y_trace = Y_box + Y_fig;
  
    Object    = (xschobj_list *)0;
    ModelName = (char *)0;

    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_LOINS )
    {
      LoIns     = (loins_list *)SchBox->SOURCE;
      ModelName = LoIns->FIGNAME;

      Element = searchauthelem( XSCH_HASH_ICON, ModelName );

      if ( Element != (authelem *)0 )
      {
        XschIcon = (xschicon *)Element->VALUE;

        if ( XschIcon->TYPE == XSCH_ICON_CELL_BUFFER )
        {
          Object = XschAddBuffer( X_trace, Y_trace, Dx, Dy,
                                  XschIcon->ARGS[ 0 ], XschIcon->ARGS[ 1 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_AND )
        {
          Object = XschAddAnd( X_trace, Y_trace, Dx, Dy,
                               XschIcon->ARGS[ 0 ],
                               XschIcon->ARGS[ 1 ],
                               XschIcon->ARGS[ 2 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_OR )
        {
          Object = XschAddOr( X_trace, Y_trace, Dx, Dy,
                              XschIcon->ARGS[ 0 ],
                              XschIcon->ARGS[ 1 ],
                              XschIcon->ARGS[ 2 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_XOR )
        {
          Object = XschAddXor( X_trace, Y_trace, Dx, Dy,
                               XschIcon->ARGS[ 0 ],
                               XschIcon->ARGS[ 1 ],
                               XschIcon->ARGS[ 2 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_TRISTATE )
        {
          Object = XschAddTristate( X_trace, Y_trace, Dx, Dy,
                                    XschIcon->ARGS[ 0 ],
                                    XschIcon->ARGS[ 1 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_REGISTER )
        {
          Object = XschAddRegister( X_trace, Y_trace, Dx, Dy,
                                    XschIcon->ARGS[ 0 ],
                                    XschIcon->ARGS[ 1 ],
                                    XschIcon->ARGS[ 2 ],
                                    XschIcon->ARGS[ 3 ],
                                    XschIcon->ARGS[ 4 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_CONSTANT )
        {
          Object = XschAddConstant( X_trace, Y_trace, Dx, Dy,
                                    XschIcon->ARGS[ 0 ],
                                    XschIcon->ARGS[ 1 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_ANDOR  )
        {
          Object = XschAddAndOr( X_trace, Y_trace, Dx, Dy,
                                 XschIcon->ARGS[ 0 ],
                                 XschIcon->ARGS[ 1 ],
                                 XschIcon->ARGS[ 2 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_ORAND )
        {
          Object = XschAddOrAnd( X_trace, Y_trace, Dx, Dy,
                                 XschIcon->ARGS[ 0 ],
                                 XschIcon->ARGS[ 1 ],
                                 XschIcon->ARGS[ 2 ], Layer );
        }
        else
        if ( XschIcon->TYPE == XSCH_ICON_CELL_MUX )
        {
          Object = XschAddMux( X_trace, Y_trace, Dx, Dy,
                               XschIcon->ARGS[ 0 ],
                               XschIcon->ARGS[ 1 ],
                               XschIcon->ARGS[ 2 ],
                               XschIcon->ARGS[ 3 ], Layer );
        }

        ModelName = (char *)0;
      }
    }
    else
    if ( SchBox->SOURCE_TYPE == SCH_SOURCE_LOTRS )
    {
      LoTrs = (lotrs_list *)SchBox->SOURCE;


      Object = XschAddTransistor( X_trace, Y_trace, Dx, Dy,
                                  ( LoTrs->TYPE == TRANSN ) ? 1 : 0, Layer );
      ModelName = (char *)0;
    }

    if ( Object == (xschobj_list *)0 )
    {
      Object = XschAddRectangle( X_trace, Y_trace, Dx, Dy, Layer );
    }

    Object->LINK = XschAddText( X_trace + ( Dx >> 1 ), 
                                Y_trace + ( Dy >> 1 ), XSCH_OBJECT_TEXT_CENTER, 
                                SchBox->NAME, Layer );

    SetXschSchBox( Object       );
    SetXschSchBox( Object->LINK );

    Object->USER       = (void *)SchBox;
    Object->LINK->USER = (void *)SchBox;

    if ( ModelName == (char *)0 )
    {
      Object->LINK->LINK = Object;
    }
    else
    {
      Object->LINK->LINK = XschAddText( X_trace + ( Dx >> 1 ), 
                                        Y_trace + ( SCP_BOX_CON_BASE_Y * XSCH_UNIT ),
                                        XSCH_OBJECT_TEXT_CENTER, 
                                        ModelName, Layer );

      SetXschSchBox( Object->LINK->LINK );

      Object->LINK->LINK->USER = (void *)SchBox;
      Object->LINK->LINK->LINK = Object;
    }

    XschPlaceConnector( SchBox->CON_IN , X_trace, Y_trace, Layer );
    XschPlaceConnector( SchBox->CON_OUT, X_trace, Y_trace, Layer );
  }

  resetauthtable( XschHashBoxInfo  );
  resetauthtable( XschHashNetInfo  );
  resetauthtable( XschHashConInfo  );
  resetauthtable( XschHashPathInfo );

  freeptype( XschLayerInfoList );
  XschLayerInfoList = (ptype_list    *)0;

  XschFreeLayerPath( XschLayerPathList );
  XschLayerPathList = (xschpath_list *)0;
}
