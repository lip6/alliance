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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                   box.c                           |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.07.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"

# include "mbkrds.h"
# include "box.h"
# include "error.h"

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

  static int LYNX_SPLIT_POINT_INDEX[ 4 ][ 4 ] =
  {
    { 0, 0, 1, 3 },
    { 0, 0, 3, 1 },
    { 2, 0, 3, 3 },
    { 0, 2, 3, 3 }
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      LynxAddBox                             |
|                                                             |
\------------------------------------------------------------*/

lynxbox_list *LynxAddBox( X1, Y1, X2, Y2 )

  long X1;
  long Y1;
  long X2;
  long Y2;
{
  lynxbox_list *Box;

  Box = (lynxbox_list *)rdsallocheap( sizeof( lynxbox_list ) );

  Box->X1 = X1;
  Box->Y1 = Y1;
  Box->X2 = X2;
  Box->Y2 = Y2;

  return( Box );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxDelAllBox                          |
|                                                             |
\------------------------------------------------------------*/

void LynxDelAllBox( Box )

  lynxbox_list *Box;
{
  lynxbox_list *ScanBox;

  ScanBox = Box;

  while ( ScanBox != (lynxbox_list *)0 )
  {
    Box = ScanBox->NEXT;
    rdsfreeheap( ScanBox, sizeof( lynxbox_list ) );
    ScanBox = Box;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       LynxViewBox                           |
|                                                             |
\------------------------------------------------------------*/

# ifdef LYNX_DEBUG

void LynxViewBox( Box )

  lynxbox_list *Box;
{
  lynxbox_list *ScanBox;

  fprintf( stdout, "-->Box:\n" );

  for ( ScanBox  = Box;
        ScanBox != (lynxbox_list *)0;
        ScanBox  = ScanBox->NEXT )
  {
    fprintf( stdout, "X1:%d, Y1:%d, X2:%d, Y2:%d\n",
             ScanBox->X1 * SCALE_X / RDS_LAMBDA,
             ScanBox->Y1 * SCALE_X / RDS_LAMBDA,
             ScanBox->X2 * SCALE_X / RDS_LAMBDA,
             ScanBox->Y2 * SCALE_X / RDS_LAMBDA );
  }

  fprintf( stdout, "<--Box\n" );
}

# endif

/*------------------------------------------------------------\
|                                                             |
|                      LynxIntersectBox                       |
|                                                             |
\------------------------------------------------------------*/

lynxbox_list *LynxIntersectBox( Box1, Box2 )

  lynxbox_list *Box1;
  lynxbox_list *Box2;
{
  long          X1;
  long          Y1;
  long          X2;
  long          Y2;

  if ( ( Box1->X1 < Box2->X2 ) &&
       ( Box1->Y1 < Box2->Y2 ) &&
       ( Box1->X2 > Box2->X1 ) &&
       ( Box1->Y2 > Box2->Y1 ) )
  {
    if ( Box1->X1 > Box2->X1 ) X1 = Box1->X1;
    else                       X1 = Box2->X1;

    if ( Box1->X2 < Box2->X2 ) X2 = Box1->X2;
    else                       X2 = Box2->X2;

    if ( Box1->Y1 > Box2->Y1 ) Y1 = Box1->Y1;
    else                       Y1 = Box2->Y1;

    if ( Box1->Y2 < Box2->Y2 ) Y2 = Box1->Y2;
    else                       Y2 = Box2->Y2;

    return( LynxAddBox( X1, Y1, X2, Y2 ) );
  }

  return( (lynxbox_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       LynxSplitBox                          |
|                                                             |
\------------------------------------------------------------*/

lynxbox_list *LynxSplitBox( Box1, Box2 )

  lynxbox_list *Box1;
  lynxbox_list *Box2;
{
  lynxbox_list *HeadBox;
  lynxbox_list *NewBox;
  long          X1H;
  long          Y1H;
  long          X2H;
  long          Y2H;
  long          PointX[ 4 ];
  long          PointY[ 4 ];
  int           Index;

  HeadBox = (lynxbox_list *)0;

  if ( ( Box1->X1 < Box2->X2 ) &&
       ( Box1->Y1 < Box2->Y2 ) &&
       ( Box1->X2 > Box2->X1 ) &&
       ( Box1->Y2 > Box2->Y1 ) )
  {
    PointX[ 1 ] = Box1->X1;
    PointY[ 1 ] = Box1->Y1;
    PointX[ 2 ] = Box1->X2;
    PointY[ 2 ] = Box1->Y2;

    PointX[ 0 ] = Box2->X1;
    PointY[ 0 ] = Box2->Y1;
    PointX[ 3 ] = Box2->X2;
    PointY[ 3 ] = Box2->Y2;

    for ( Index = 0; Index < 4; Index++ )
    {
      X1H = PointX[ LYNX_SPLIT_POINT_INDEX[ Index ][ 0 ] ];
      Y1H = PointY[ LYNX_SPLIT_POINT_INDEX[ Index ][ 1 ] ];
      X2H = PointX[ LYNX_SPLIT_POINT_INDEX[ Index ][ 2 ] ];
      Y2H = PointY[ LYNX_SPLIT_POINT_INDEX[ Index ][ 3 ] ];

      if ( ( X1H < X2H ) &&
           ( Y1H < Y2H ) )
      {
        if ( ( X1H >= Box2->X1 ) &&
             ( Y1H >= Box2->Y1 ) &&
             ( X2H <= Box2->X2 ) &&
             ( Y2H <= Box2->Y2 ) )
        {
          NewBox = LynxAddBox( X1H, Y1H, X2H, Y2H );

          NewBox->NEXT = HeadBox;
          HeadBox      = NewBox;
        }
      }
    }
  }

  return( HeadBox );
}
