/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
| File    :                 G_inspect.c                       |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   12/05/94                        |
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
# include "G_user.h"
# include "G_inspect.h"

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


static void print_desc( ptdesc, type, code )

  rdsrec_list *ptdesc;
{
  void *MbkObject;
  char  Orient;

  MbkObject = GENVIEW_MBK( ptdesc );
  
  if ( IsRdsSegment( ptdesc ) ) 
  {
    fprintf( xinspect, "SEGMENT :\n");
    fprintf( xinspect, "     X1 : %-6ld Y1 : %-6ld\n",
             ((phseg_list *)MbkObject)->X1 / SCALE_X, 
             ((phseg_list *)MbkObject)->Y1 / SCALE_X );
    fprintf( xinspect, "     X2 : %-6ld Y2 : %-6ld\n",
             ((phseg_list *)MbkObject)->X2 / SCALE_X, 
             ((phseg_list *)MbkObject)->Y2 / SCALE_X );
    fprintf( xinspect, "     WIDTH : %-6ld\n",
             ((phseg_list *)MbkObject)->WIDTH / SCALE_X );

    if (((phseg_list *)MbkObject)->NAME)
    {
      fprintf( xinspect, "     NAME  : %s\n",
               ((phseg_list *)MbkObject)->NAME);
    }

    fprintf( xinspect, "     LAYER : %s\n\n", 
    GENVIEW_SEGMENT_NAME_TABLE[ ((phseg_list *)MbkObject)->LAYER ][ 0 ] );
  }
  else
  if ( IsRdsVia( ptdesc ) )
  {
    fprintf( xinspect, "MOTIF :\n");
    fprintf( xinspect, "     X : %-6ld Y : %-6ld\n     DX : %-6ld DY : %-6ld\n", 
             ((phvia_list *)MbkObject)->XVIA / SCALE_X,
             ((phvia_list *)MbkObject)->YVIA / SCALE_X,
             ((phvia_list *)MbkObject)->DX / SCALE_X,
             ((phvia_list *)MbkObject)->DY / SCALE_X );
    fprintf( xinspect, "     TYPE : %s\n\n",
    GENVIEW_VIA_NAME_TABLE[ ((phvia_list *)MbkObject)->TYPE ][ 0 ] );
  }
  else
  if ( IsRdsReference( ptdesc ) )
  {
    fprintf( xinspect, "REFERENCE :\n");
    fprintf( xinspect, "     X : %-6ld Y : %-6ld\n", 
             ((phref_list *)MbkObject)->XREF / SCALE_X,
             ((phref_list *)MbkObject)->YREF / SCALE_X );
    fprintf( xinspect, "     NAME  : %s\n",
             ((phref_list *)MbkObject)->NAME );
    fprintf( xinspect, "     MODEL NAME  : %s\n",
             ((phref_list *)MbkObject)->FIGNAME );

    if ( IsRdsRefRef( ptdesc ) )
    {
      fprintf( xinspect, "     TYPE : %s\n\n",
      GENVIEW_REFERENCE_NAME_TABLE[ MBK_REF_REF ][ 0 ] );
    }
    else
    {
      fprintf( xinspect, "     TYPE : %s\n\n",
      GENVIEW_REFERENCE_NAME_TABLE[ MBK_REF_CON ][ 0 ] );
    }
  }
  else
  if ( IsRdsConnector( ptdesc ) )
  {
    fprintf( xinspect, "FIGURE CONNECTOR :\n");
    fprintf( xinspect, "     X : %-6ld Y : %-6ld\n",
             ((phcon_list *)MbkObject)->XCON / SCALE_X,
             ((phcon_list *)MbkObject)->YCON / SCALE_X );
    fprintf( xinspect, "     WIDTH  : %-6ld\n", 
             ((phcon_list *)MbkObject)->WIDTH / SCALE_X );
    fprintf( xinspect, "     NAME   : %s\n",
             ((phcon_list *)MbkObject)->NAME );
    fprintf( xinspect, "     INDEX  : %ld\n",
             ((phcon_list *)MbkObject)->INDEX );
    switch ( ((phcon_list *)MbkObject)->ORIENT )
    {
      case 'N' : Orient = GENVIEW_NORTH;
      break;
      case 'S' : Orient = GENVIEW_SOUTH;
      break;
      case 'E' : Orient = GENVIEW_EAST;
      break;
      default  : Orient = GENVIEW_WEST;
    }
    fprintf( xinspect, "     ORIENT : %s\n",
    GENVIEW_ORIENT_NAME_TABLE[ Orient ][ 0 ] );
    fprintf( xinspect, "     LAYER : %s\n\n",
    GENVIEW_CONNECTOR_NAME_TABLE[ ((phcon_list *)MbkObject)->LAYER ][ 0 ] );
  }
  else
  if ( IsRdsInstance( ptdesc ) )
  {
    fprintf( xinspect, "INSTANCE :\n");
    fprintf( xinspect, "     X : %-6ld  Y : %-6ld\n",
             ((phins_list *)MbkObject)->XINS / SCALE_X,
             ((phins_list *)MbkObject)->YINS / SCALE_X );
    fprintf( xinspect, "     dX : %-5ld dY : %-5ld\n",
             ptdesc->DX / RDS_LAMBDA, ptdesc->DY / RDS_LAMBDA );
    fprintf( xinspect, "     NAME  : %s\n",
             ((phins_list *)MbkObject)->INSNAME );
    fprintf( xinspect, "     MODEL : %s\n",
             ((phins_list *)MbkObject)->FIGNAME );
    fprintf( xinspect, "     TRANSFORMATION : %s\n\n",
    GENVIEW_SYMMETRY_NAME_TABLE[ ((phins_list *)MbkObject)->TRANSF ][ 0 ] );
  }
}

/* inspect : prints textual information about a subwindow */

void inspect( xwin, ywin, dxwin, dywin )

  long xwin, ywin, dxwin, dywin;
{
  unsigned char flag;
  long x, y, dx, dy;
  unsigned char layer;
  rdsins_list  *ptins;
  rdsrec_list  *Scan;
  rdsrec_list  *First;
  rdsrec_list  *pt;

  /* Opens the file that receive information */

  if ((xinspect = fopen (".xinspect", "w")) == NULL)
  {
    G_error( "inspect : unable to open file !", -1 );
  }

  for ( layer = 0; layer < RDS_MAX_LAYER; layer++ ) 
  {
    pt = Visu_db->LAYERTAB[ layer ];

    while ( pt != (rdsrec_list *)NULL )
    {
      if ( ! IsTagGenview( pt ) )
      {
        x  = pt->X;
        y  = pt->Y;
        dx = pt->DX;
        dy = pt->DY;

        if ( Appart_win( &flag, x, y, dx, dy,
                         xwin, ywin, dxwin, dywin ) )
        {
          /* even surround descriptors are printed */

          if (flag == APSURROUND)
          {
            fprintf(xinspect, "SURROUND ");
          }

          /* print info into the file */

          print_desc( pt );

          First = pt;
          Scan  = pt;

          do
          {
            SetTagGenview( Scan );
            Scan = (rdsrec_list *)(Scan->USER);
          }
          while ( Scan != First );
        }
      }

      pt = pt->NEXT;
    }
  }

  for ( layer = 0; layer < RDS_MAX_LAYER; layer++ ) 
  {
    pt = Visu_db->LAYERTAB[ layer ];

    while ( pt != (rdsrec_list *)NULL )
    {
      if ( IsTagGenview( pt ) )
      {
        First = pt;
        Scan  = pt;

        do
        {
          ClearTagGenview( Scan );
          Scan = (rdsrec_list *)(Scan->USER);
        }
        while ( Scan != First );
      }

      pt = pt->NEXT;
    }
  }

  for ( ptins  = Visu_db->INSTANCE; 
        ptins != (rdsins_list *)NULL;
        ptins  = ptins->NEXT )
  {
    pt = ptins->LAYERTAB[ RDS_ABOX ];
    x  = pt->X;
    y  = pt->Y;
    dx = pt->DX;
    dy = pt->DY;

    if ( Appart_win( &flag, x, y, dx, dy,
                     xwin, ywin, dxwin, dywin ))
    {
      /* even surround descriptors are printed */

      if (flag == APSURROUND)
      {
        fprintf(xinspect, "SURROUND ");
      }

      /* print info into the file */

      print_desc( pt );
    }
  }

  fclose( xinspect );
}
