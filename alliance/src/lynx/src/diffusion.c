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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                 diffusion.c                       |
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
# include "diffusion.h"
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
|                        LynxDiffusionFigure                  |
|                                                             |
\------------------------------------------------------------*/

void LynxDiffusionFigure( FigureRds, WindowRds )

  rdsfig_list *FigureRds;
  rdswindow   *WindowRds;
{
  rdsrec_list    *ScanRec;
  rdsrec_list    *NewRec;
  rdsrec_list    *ActiveRec;
  rdswin_list    *ScanWin;
  rdswinrec_list *ScanWinRec;
  rdsrecwin_list *RecWin;
  rdsrecwin_list *ScanRecWin;
  rdsrecwin_list  StaticRecWin;
  lynxbox_list   *DiffBox;
  lynxbox_list   *ScanBox;
  lynxbox_list   *ActiveBox;
  lynxbox_list   *InterBox;
  lynxbox_list   *SplitBox;
  lynxbox_list  **PrevBox;
  lynxbox_list    RectBox;
  int             ScanRecIndex;
  int             Index;
  int             RdsLayer;
  int             DiffLayer;
  int             ActiveLayer;
  char            Present;

  StaticRecWin.NEXT = (rdsrecwin_list *)0;
  RectBox.NEXT      = (lynxbox_list   *)0;

  for ( RdsLayer = 0; RdsLayer < RDS_MAX_LAYER; RdsLayer++ )
  {
    ActiveLayer = GET_LYNX_DIFFUSION_LAYER( RdsLayer, 0 );

    if ( ActiveLayer == RDS_LYNX_DIFFUSION_EMPTY ) continue;

    for ( ActiveRec  = FigureRds->LAYERTAB[ ActiveLayer ];
          ActiveRec != (rdsrec_list *)0;
          ActiveRec  = ActiveRec->NEXT )
    {
      ActiveBox = LynxAddBox( ActiveRec->X,  ActiveRec->Y, 
                              ActiveRec->X + ActiveRec->DX,
                              ActiveRec->Y + ActiveRec->DY );
      Index     = 1;
      DiffLayer = GET_LYNX_DIFFUSION_LAYER( RdsLayer, Index );
      Present   = GET_LYNX_DIFFUSION_Y_OR_N( RdsLayer, Index );

      while ( DiffLayer != RDS_LYNX_DIFFUSION_EMPTY )
      {
        if ( IsRdsOneWindow( ActiveRec ) )
        {
          StaticRecWin.WINDOW = (rdswin_list *)ActiveRec->USER;
          RecWin              = &StaticRecWin;
        }
        else
        {
          RecWin = (rdsrecwin_list *)ActiveRec->USER;
        }
    
        DiffBox = (lynxbox_list *)0;

        for ( ScanRecWin  = RecWin;
              ScanRecWin != (rdsrecwin_list *) 0;
              ScanRecWin  = ScanRecWin->NEXT )
        {
          ScanWin = ScanRecWin->WINDOW;
      
          for ( ScanWinRec  = ScanWin->LAYERTAB[ DiffLayer ];
                ScanWinRec != (rdswinrec_list *)0;
                ScanWinRec  = ScanWinRec->NEXT )
          {
            for ( ScanRecIndex = 0; 
                  ScanRecIndex < RWI_MAX_REC;
                  ScanRecIndex++ )
            {
              ScanRec = ScanWinRec->RECTAB[ ScanRecIndex ];
    
              if ( ScanRec != (rdsrec_list *)0 )
              {
                ScanBox =  ActiveBox;
                PrevBox = &ActiveBox;

                RectBox.X1 = ScanRec->X;
                RectBox.Y1 = ScanRec->Y;
                RectBox.X2 = ScanRec->DX + RectBox.X1;
                RectBox.Y2 = ScanRec->DY + RectBox.Y1;

                while ( ScanBox != (lynxbox_list *)0 )
                {
                  InterBox = LynxIntersectBox( &RectBox, ActiveBox );

                  if ( InterBox != (lynxbox_list *)0 )
                  {
                    SplitBox = LynxSplitBox( &RectBox, ActiveBox );

                    if ( Present )
                    {
                      InterBox->NEXT = DiffBox;
                      DiffBox        = InterBox;
                    }
                    else
                    {
                      LynxDelAllBox( InterBox );
                    }

                    if ( SplitBox != (lynxbox_list *)0 )
                    {
                      *PrevBox = SplitBox;

                      while ( SplitBox->NEXT != (lynxbox_list *)0 )
                      {
                        SplitBox = SplitBox->NEXT;
                      }

                      SplitBox->NEXT = ScanBox->NEXT;
                    }
                    else
                    {
                      *PrevBox = ScanBox->NEXT;
                    }

                    ScanBox->NEXT = (lynxbox_list *)0;
                    LynxDelAllBox( ScanBox );

                    ScanBox = *PrevBox;
                  }
                  else
                  {
                    PrevBox = &ScanBox->NEXT;
                    ScanBox =  ScanBox->NEXT;
                  }
                }
              }

              if ( ActiveBox == (lynxbox_list *)0 ) break;
            }

            if ( ActiveBox == (lynxbox_list *)0 ) break;
          }

          if ( ActiveBox == (lynxbox_list *)0 ) break;
        }

        if ( Present )
        {
          LynxDelAllBox( ActiveBox );

          ActiveBox = DiffBox;
        }

        if ( ActiveBox == (lynxbox_list *)0 ) break;

        Index     = Index + 1;
        DiffLayer = GET_LYNX_DIFFUSION_LAYER( RdsLayer, Index );
        Present   = GET_LYNX_DIFFUSION_Y_OR_N( RdsLayer, Index );
      }

      if ( DiffLayer == RDS_LYNX_DIFFUSION_EMPTY )
      {
        for ( ScanBox  = ActiveBox;
              ScanBox != (lynxbox_list *)0;
              ScanBox  = ScanBox->NEXT )
        {
          NewRec = addrdsfigrec( FigureRds, ActiveRec->NAME, RdsLayer,
                                 ScanBox->X1              , ScanBox->Y1,
                                 ScanBox->X2 - ScanBox->X1, ScanBox->Y2 - ScanBox->Y1 );

          addrdsrecwindow( NewRec, WindowRds );
        }
      }

      LynxDelAllBox( ActiveBox );
    }
  }
}
