/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                   G_peek.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   08.05.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "G_parse.h"
# include "C_utils.h"
# include "G_peek.h"
# include "G_rpeek.h"

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

/* special concern :
   In case one doesn't want an overlap of the actual figure contents on
   the peek, the figure structure is duplicated into the peek data structure
   so the layers are sorted the right way.
   There is no way to have names or any inspect information when this mode is
   in use. */

static int Peekfig(xwin, ywin, dxwin, dywin)

  int xwin, ywin, dxwin, dywin;
{
  rdsrec_list *pt;
  long         x, y, dx, dy;
  int          layer;
  char         flag;

  for (layer = 0; layer < RDS_ABOX; layer++) 
  {
    pt = Visu_db->LAYERTAB[ layer ];
    while (pt) 
    {
      x  = pt->X;
      dx = pt->DX;
      dy = pt->DY;
      y  = pt->Y;

      if ( Appart_win( &flag, x, y, dx, dy, xwin, ywin, dxwin, dywin ) ) 
      {
        /* Descriptors are adjusted to the peek window */

        Troncate(&x, &y, &dx, &dy, xwin, ywin, dxwin, dywin, flag);

        addrdsfigrec( Peek_db, pt->NAME, layer, x, y, dx, dy );
      }
      pt = pt->NEXT;
    }
  }

  return 1;
}

static void Peek_instance( ModelMbk, InstanceMbk, xwin, ywin, dxwin, dywin )

  phfig_list *ModelMbk;
  phins_list *InstanceMbk;
  long        xwin, ywin, dxwin, dywin;
{
  rdsins_list  *InstanceRds;
  rdsrec_list  *Rectangle;
  rdsrec_list  *New;
  rdsrec_list **Previous;
  long          Xins;
  long          Yins;
  unsigned char flag;
  char          Layer;

  xyflat
  ( &Xins              , &Yins,
    0                  , 0,
    InstanceMbk->XINS  , InstanceMbk->YINS,
    ModelMbk->XAB1     , ModelMbk->YAB1,
    ModelMbk->XAB2     , ModelMbk->YAB2,
    InstanceMbk->TRANSF
  );

  Xins = RfmRoundLow( Xins * RDS_LAMBDA / SCALE_X );
  Yins = RfmRoundLow( Yins * RDS_LAMBDA / SCALE_X );

  InstanceRds = addrdsins( Peek_db,
                           InstanceMbk->FIGNAME, InstanceMbk->INSNAME,
                           InstanceMbk->TRANSF,
                           Xins, Yins );

  inssegmbkrds( InstanceMbk, ModelMbk, InstanceRds, 0 );
  insconmbkrds( InstanceMbk, ModelMbk, InstanceRds    );
  insrefmbkrds( InstanceMbk, ModelMbk, InstanceRds    );
  insviambkrds( InstanceMbk, ModelMbk, InstanceRds, 0 );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    Rectangle = InstanceRds->LAYERTAB[ Layer ];
    Previous  = &InstanceRds->LAYERTAB[ Layer ];
    
    while ( Rectangle != (rdsrec_list *)NULL )
    {
      if ( Appart_win( &flag,
                       Rectangle->X, Rectangle->Y,
                       Rectangle->DX, Rectangle->DY,
                       xwin, ywin, dxwin, dywin ) )
      {
        Troncate( &Rectangle->X , &Rectangle->Y, 
                  &Rectangle->DX, &Rectangle->DY,
                  xwin, ywin, dxwin, dywin, flag );
        *Previous = Rectangle->NEXT;
        New       = Rectangle;
        Rectangle = Rectangle->NEXT;
      
        New->NEXT = Peek_db->LAYERTAB[ Layer ];
        Peek_db->LAYERTAB[ Layer ] = New;
      }
      else
      {
        Previous  = &Rectangle->NEXT;
        Rectangle = Rectangle->NEXT;
      }
    }
  }

  delrdsins( Peek_db, InstanceRds->INSNAME );
}

/* Peek : gets all instances in the peek window and flattens them. */

int Peek( xwin, ywin, dxwin, dywin )

  long xwin, ywin, dxwin, dywin;
{
  unsigned char flag;
  long          x, y, dx, dy;
  rdsins_list  *instance;
  rdsrec_list  *insabox;
  int           insflag = 0;
  phfig_list   *ptfig;
  phins_list   *ptins;
  char          flat;

  /* Seeks instances that intersect the peek window ...  */

  instance = Visu_db->INSTANCE;

  while ( instance ) 
  {
    insabox = instance->LAYERTAB[ RDS_ABOX ];
    x  = insabox->X;
    y  = insabox->Y;
    dx = insabox->DX;
    dy = insabox->DY;

    if ( Appart_win( &flag, x, y, dx, dy, 
                     xwin - GENVIEW_PEEK_BOUND, 
                     ywin - GENVIEW_PEEK_BOUND, 
                     dxwin + ( GENVIEW_PEEK_BOUND << 1 ),
                     dywin + ( GENVIEW_PEEK_BOUND << 1 ) ) )
    {
      ptins = (phins_list *)GENVIEW_MBK( insabox );
      ptfig = dup_phfig( getphfig( ptins->FIGNAME, 'A'), &flat );
/*
** Modified by L.Jacomme 07/06/95
*/
      delphfig( ptins->FIGNAME );

      r_peek_flat( ptfig );

      Peek_instance( ptfig, ptins, xwin, ywin, dxwin, dywin);

      insflag = 1;
    }
    instance = instance->NEXT;
  }

  if ( C_atomicValue != 0 )
  {
    if ( Peekfig( xwin, ywin, dxwin, dywin ) == 1 )
    {
      insflag = 1;
    }
  }

  return insflag;
}

/* Unpeek : detroys the peek data structure. */

void Unpeek()
{
  delrdsfig( Peek_db->NAME );
  Peek_db = addrdsfig( "_PEEK_DB", GENVIEW_SIZE );
}
