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
| File    :                   hole.c                          |
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
# include "hole.h"
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        LynxHoleFigure                       |
|                                                             |
\------------------------------------------------------------*/

void LynxHoleFigure( FigureHole, WindowRds, X1, Y1, X2, Y2, Layer )

  rdsfig_list *FigureHole;
  rdswindow   *WindowRds;
  long         X1;
  long         Y1;
  long         X2;
  long         Y2;
  int          Layer;
{
  rdsrec_list    *NewRec;
  rdsrec_list    *ScanRec;
  rdswin_list    *WinList;
  rdswin_list    *ScanWin;
  rdswinrec_list *ScanWinRec;
  lynxbox_list    RectBox;
  lynxbox_list    HoleBox;
  lynxbox_list   *ScanBox;
  lynxbox_list   *HeadBox;
  int             ScanRecIndex;
  long            FirstX;
  long            WX1;
  long            WY1;
  long            WX2;
  long            WY2;

  HoleBox.NEXT = (lynxbox_list *)0;
  RectBox.NEXT = (lynxbox_list *)0;

  HoleBox.X1 = X1;
  HoleBox.Y1 = Y1;
  HoleBox.X2 = X2;
  HoleBox.Y2 = Y2;

  WY1 = ( Y1 - 1 - WindowRds->YMIN ) / WindowRds->SIDE;
  WY2 = ( Y2     - WindowRds->YMIN ) / WindowRds->SIDE;
  WX1 = ( X1 - 1 - WindowRds->XMIN ) / WindowRds->SIDE;
  WX2 = ( X2     - WindowRds->XMIN ) / WindowRds->SIDE;
  
  if ( WX1 < 0              ) WX1 = 0;
  if ( WX2 < 0              ) WX2 = 0;
  if ( WX1 >= WindowRds->DX ) WX1 = WindowRds->DX - 1;
  if ( WX2 >= WindowRds->DX ) WX2 = WindowRds->DX - 1;
  
  if ( WY1 < 0              ) WY1 = 0;
  if ( WY2 < 0              ) WY2 = 0;
  if ( WY1 >= WindowRds->DY ) WY1 = WindowRds->DY - 1;
  if ( WY2 >= WindowRds->DY ) WY2 = WindowRds->DY - 1;
  
  FirstX = WX1;
  
  while ( WY1 <= WY2 )
  {
    WinList = WindowRds->WINTAB + ( WY1 * WindowRds->DX );
   
    WX1 = FirstX;
  
    while ( WX1 <= WX2 )
    {
      ScanWin = WinList + WX1;
  
      if ( ScanWin->LAYERTAB != (rdswinrec_list **)NULL )
      {
        for ( ScanWinRec  = ScanWin->LAYERTAB[ Layer ];
              ScanWinRec != (rdswinrec_list *)NULL;
              ScanWinRec  = ScanWinRec->NEXT )
        {
          for ( ScanRecIndex = 0;
                ScanRecIndex < RWI_MAX_REC;
                ScanRecIndex++ )
          {
            ScanRec = ScanWinRec->RECTAB[ ScanRecIndex ];

            if ( ( ScanRec == (rdsrec_list *)0 ) ||
                 ( IsLynxDeleted( ScanRec )    ) ) continue;
       
            RectBox.X1 = ScanRec->X;
            RectBox.Y1 = ScanRec->Y;
            RectBox.X2 = ScanRec->DX + RectBox.X1;
            RectBox.Y2 = ScanRec->DY + RectBox.Y1;

            if ( ( RectBox.X1 < X2 ) &&
                 ( RectBox.Y1 < Y2 ) &&
                 ( RectBox.X2 > X1 ) &&
                 ( RectBox.Y2 > Y1 ) )
            {
              HeadBox = LynxSplitBox( &HoleBox, &RectBox );

              for ( ScanBox  = HeadBox;
                    ScanBox != (lynxbox_list *)0;
                    ScanBox  = ScanBox->NEXT )
              {
                if ( ScanBox == HeadBox )
                {
                  ScanRec->X  = ScanBox->X1;
                  ScanRec->Y  = ScanBox->Y1;
                  ScanRec->DX = ScanBox->X2 - ScanBox->X1;
                  ScanRec->DY = ScanBox->Y2 - ScanBox->Y1;
                }
                else
                {
                  NewRec = addrdsfigrec( FigureHole, (char *)0, Layer, 
                                         ScanBox->X1 , ScanBox->Y1,
                                         ScanBox->X2 - ScanBox->X1,
                                         ScanBox->Y2 - ScanBox->Y1 );

                  addrdsrecwindow( NewRec, WindowRds );
                }
              }

              if ( HeadBox == (lynxbox_list *)0 )
              {
                SetLynxDeleted( ScanRec );
              }
              else                          
              {
                LynxDelAllBox( HeadBox );
              }
            }
          }
        }
      }
  
      WX1 = WX1 + 1;
    }
  
    WY1 = WY1 + 1;
  }
}
