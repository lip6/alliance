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
| Tool    :                   XGRA                            |
|                                                             |
| File    :                  Place.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "XSB.h"
# include "XGR.h"
# include "XGR_place.h"
# include "XGR_graph.h"

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

  static xgrapoint *XgraPointArray = (xgrapoint *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XgraPlaceFigure                    |
|                                                             |
\------------------------------------------------------------*/

void XgraPlaceFigure()
{
  autnode_list  *ScanNode;
  autnode_list  *NodeTo;
  autnode_list  *NodeFrom;
  autarc_list   *ScanArc;
  chain_list    *ScanChain;
  int            NumberNode;
  int            PointIndex;
  float          X;
  float          Y;
  float          VectorX;
  float          VectorY;
  float          Alpha;
  float          CosAlpha;
  float          SinAlpha;

  autbegin();

  if ( XgraPointArray != (xgrapoint *)NULL )
  {
    autfreeblock( (char *)XgraPointArray );
    XgraPointArray = (xgrapoint *)NULL;
  }

  NumberNode    = XgraFigureGra->NUMBER_NODE;
  XgraPointArray = (xgrapoint *)autallocblock( sizeof( xgrapoint ) * NumberNode );

  Alpha    = ( M_PI * 2.0 ) / (float)( NumberNode );
  CosAlpha = cos( Alpha );
  SinAlpha = sin( Alpha );

  VectorX  = (float)( NumberNode << 1 );
  VectorY  = 0.0;

  PointIndex = 0;

  for ( ScanNode  = XgraFigureGra->NODE;
        ScanNode != (autnode_list *)NULL;
        ScanNode  = ScanNode->NEXT )
  {
    X = VectorX;
    Y = VectorY;

    ScanNode->USER = (void *)( &XgraPointArray[ PointIndex ] );

    XgraPointArray[ PointIndex   ].X = (long)X;
    XgraPointArray[ PointIndex++ ].Y = (long)Y;

    VectorX = ( X * CosAlpha ) - ( Y * SinAlpha );
    VectorY = ( X * SinAlpha ) + ( Y * CosAlpha );
  }

  autend();
}
