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

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_global.h"

#include "v_view.h"
#include "t_menu.h"
#include "v_menu.h"
#include "c_menu.h"
#include "v_extern.h"

static YRectangle *T_icon_pos;
static char *T_menu[] = {
		"PAGE UP",
		"PAGE DOWN",
		"LINE UP",
		"LINE DOWN",
		"CLEAR"
};

void T_setMenu ()
{
static int first;
	
	if (first == 0) {
		T_icon_pos = (YRectangle *)mbkalloc(5 * sizeof (YRectangle));
		first ++;
	}
	T_icon_pos[TR_PGUP].x = TR_PGUP_X;
	T_icon_pos[TR_PGUP].y = TR_PGUP_Y;
	T_icon_pos[TR_PGUP].width = TR_PGUP_DX;
	T_icon_pos[TR_PGUP].height = TR_PGUP_DY;
	T_icon_pos[TR_PGUP].t = TR_PGUP_T;

	T_icon_pos[TR_PGDN].x = TR_PGDN_X;
	T_icon_pos[TR_PGDN].y = TR_PGDN_Y;
	T_icon_pos[TR_PGDN].width = TR_PGDN_DX;
	T_icon_pos[TR_PGDN].height = TR_PGDN_DY;
	T_icon_pos[TR_PGDN].t = TR_PGDN_T;

	T_icon_pos[TR_LNUP].x = TR_LNUP_X;
	T_icon_pos[TR_LNUP].y = TR_LNUP_Y;
	T_icon_pos[TR_LNUP].width = TR_LNUP_DX;
	T_icon_pos[TR_LNUP].height = TR_LNUP_DY;
	T_icon_pos[TR_LNUP].t = TR_LNUP_T;

	T_icon_pos[TR_LNDN].x = TR_LNDN_X;
	T_icon_pos[TR_LNDN].y = TR_LNDN_Y;
	T_icon_pos[TR_LNDN].width = TR_LNDN_DX;
	T_icon_pos[TR_LNDN].height = TR_LNDN_DY;
	T_icon_pos[TR_LNDN].t = TR_LNDN_T;

	T_icon_pos[TR_EXIT].x = TR_EXIT_X;
	T_icon_pos[TR_EXIT].y = TR_EXIT_Y;
	T_icon_pos[TR_EXIT].width = TR_EXIT_DX;
	T_icon_pos[TR_EXIT].height = TR_EXIT_DY;
	T_icon_pos[TR_EXIT].t = TR_EXIT_T;
}

void T_drawMenu ()
{
void T_drawIcon();
register int i;
Window w;
int forget_me;
unsigned int dx, dy;

	XGetGeometry (display, T_window, &w, &forget_me, &forget_me,
			&dx, &dy, &forget_me, &forget_me);
	XFillRectangle (display, T_window, C_gcBgnd, 0, 0, dx, dy);
	/* centering the menu each time it appears */
	dy = (int)((dx/2) - (TR_EXIT_X + TR_EXIT_DX - TR_PGUP_X)/2);
	for (i = 0; i <= TR_EXIT; i++) {
		T_icon_pos[i].t += T_icon_pos[i].x += dy;
		T_drawIcon(i, OFF);
	}
}

void T_drawIcon(icon, state)
int icon, state;
{
	XFillRectangle(display, T_window, C_gcIcon, (int)T_icon_pos[icon].x,
						(int)T_icon_pos[icon].y,
						(unsigned int)(T_icon_pos[icon].width + 1),
						(unsigned int)(T_icon_pos[icon].height + 1));
	V_surround(T_window, T_icon_pos[icon].x, T_icon_pos[icon].y,
					T_icon_pos[icon].width, T_icon_pos[icon].height, state);
	XDrawImageString(display, T_window, C_gcIconText,
							(int)(T_icon_pos[icon].t - state),
							(int)(T_icon_pos[icon].y + 12 - state),
							T_menu[icon], strlen (T_menu[icon]));
}

int T_clickedIcon(xClick, yClick)
int xClick, yClick;
{
register int i;

	for (i = 0; i <= TR_EXIT; i++)
		if (((xClick > T_icon_pos[i].x) &&
			 	(xClick < T_icon_pos[i].x + T_icon_pos[i].width)) &&
	 			((yClick > T_icon_pos[i].y) &&
			 	(yClick < T_icon_pos[i].y + T_icon_pos[i].height)))
			return (i);
	return INSCREEN;
}
