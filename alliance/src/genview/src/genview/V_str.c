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
| File    :                   V_str.c                         |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
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
# include "G_string.h"
# include "C_utils.h"
# include "V_str.h"

# include "v_view.h"
# include "v_extern.h"
# include "v_menu.h"

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
|                          Put_str                            |
|                                                             |
\------------------------------------------------------------*/

void Put_str( pt, drawable)

    rdsrec_list *pt;
    Drawable     drawable;
{
    int   i, j;
    long  xstr, ystr;
    static char buffer[BUFSIZ];

  /* font choice */

  XSetFont(display, V_gcCao[GC_FGND], V_vr[0]);

  for (i = 1, j = 0; j < 5; j++, i += 3)
  {
    if (scale >= i && scale < i + 3) 
    {
      XSetFont(display, V_gcCao[GC_FGND], V_vr[j]);
      break;
    }
  }

  XSetForeground(display, V_gcCao[GC_FGND], V_colors[COLOR_FGND]);

  /* going thru strings */

  String_coord(pt, &xstr, &ystr);

  if ( IsRdsConnector( pt ) )
  {
    if ( C_Config_List.C_VISIBLE[ OBJ_CONNECTOR_TEXT ] == 0 ) return;

    if ( pt->NAME ) 
    {
      XDrawString(display, drawable, V_gcCao[GC_FGND],
              (int)((xstr - xscreen) * scale),
              YMENU - 1 +
              (int)((yscreen - ystr + dyscreen) * scale),
              pt->NAME, strlen(pt->NAME));
    }
  }
  else
  if ( IsRdsReference( pt ) )
  {
    if ( C_Config_List.C_VISIBLE[ OBJ_REFERENCE_TEXT ] == 0 ) return;

    if ( pt->NAME )
    {
      XDrawString(display, drawable, V_gcCao[GC_FGND],
               (int)((xstr - xscreen) * scale),
               YMENU - 1 
               + (int)((yscreen - ystr + dyscreen) * scale),
               pt->NAME, strlen(pt->NAME));
    } 
  }
  else
  if ( IsRdsSegment( pt ) )
  {
    if ( C_Config_List.C_VISIBLE[ OBJ_SEGMENT_TEXT ] == 0 ) return;
    if ( pt->NAME )
    {
      XDrawString(display, drawable, V_gcCao[GC_FGND],
               (int)((xstr - xscreen) * scale),
               YMENU - 1 
               + (int)((yscreen - ystr + dyscreen) * scale),
              pt->NAME, strlen(pt->NAME));
    }
  }
  else 
  if ( IsRdsInstance( pt ) )
  {
    if ( C_Config_List.C_VISIBLE[ OBJ_INSTANCE_TEXT ] == 0 ) return;

    if (j < 4) 
    {
      if (j) XSetFont(display, V_gcCao[GC_FGND], V_vr[j + 1]);

       XDrawString(display, drawable, V_gcCao[GC_FGND],
               (int)((xstr - xscreen) * scale),
               YMENU - 1 + (int)((yscreen - ystr + dyscreen) * scale),
               pt->NAME, strlen(pt->NAME));
       XSetFont(display, V_gcCao[GC_FGND], V_vr[j]);
    } 
    else
    {
      XDrawString(display, drawable, V_gcCao[GC_FGND],
           (int)((xstr - xscreen) * scale),
           YMENU - 1 + (int)((yscreen - ystr + dyscreen) * scale),
           pt->NAME, strlen(pt->NAME));
    }
  }
}
