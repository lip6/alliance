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

# include "v_view.h"
# include "i_menu.h"
# include "v_menu.h"
# include "c_menu.h"
# include "v_extern.h"

static YRectangle *I_icon_pos;

static char *I_menu[] = {
    "PAGE UP",
    "PAGE DOWN",
    "LINE UP",
    "LINE DOWN",
    "OK"
};

void I_setMenu ()
{
static int first;

	if(first == 0) {
		I_icon_pos = (YRectangle *)mbkalloc(5 * sizeof (YRectangle));
		first ++;
	}
	I_icon_pos[INS_PGUP].x = INS_PGUP_X;
	I_icon_pos[INS_PGUP].y = INS_PGUP_Y;
	I_icon_pos[INS_PGUP].width = INS_PGUP_DX;
	I_icon_pos[INS_PGUP].height = INS_PGUP_DY;
	I_icon_pos[INS_PGUP].t = INS_PGUP_T;

	I_icon_pos[INS_PGDN].x = INS_PGDN_X;
	I_icon_pos[INS_PGDN].y = INS_PGDN_Y;
	I_icon_pos[INS_PGDN].width = INS_PGDN_DX;
	I_icon_pos[INS_PGDN].height = INS_PGDN_DY;
	I_icon_pos[INS_PGDN].t = INS_PGDN_T;

	I_icon_pos[INS_LNUP].x = INS_LNUP_X;
	I_icon_pos[INS_LNUP].y = INS_LNUP_Y;
	I_icon_pos[INS_LNUP].width = INS_LNUP_DX;
	I_icon_pos[INS_LNUP].height = INS_LNUP_DY;
	I_icon_pos[INS_LNUP].t = INS_LNUP_T;

	I_icon_pos[INS_LNDN].x = INS_LNDN_X;
	I_icon_pos[INS_LNDN].y = INS_LNDN_Y;
	I_icon_pos[INS_LNDN].width = INS_LNDN_DX;
	I_icon_pos[INS_LNDN].height = INS_LNDN_DY;
	I_icon_pos[INS_LNDN].t = INS_LNDN_T;

	I_icon_pos[INS_EXIT].x = INS_EXIT_X;
	I_icon_pos[INS_EXIT].y = INS_EXIT_Y;
	I_icon_pos[INS_EXIT].width = INS_EXIT_DX;
	I_icon_pos[INS_EXIT].height = INS_EXIT_DY;
	I_icon_pos[INS_EXIT].t = INS_EXIT_T;
}

void I_drawMenu ()
{
void I_drawIcon();
register int i;
Window w;
int fgm;
unsigned int dx, dy;

	XGetGeometry (display, I_window, &w, &fgm, &fgm,
			&dx, &dy, &fgm, &fgm);
	XFillRectangle (display, I_window, C_gcBgnd, 0, 0, dx, dy);
	/* centering the menu each time it appears */
	dy = (int)((dx/2) - (INS_EXIT_X + INS_EXIT_DX - INS_PGUP_X)/2);
	for (i = 0; i <= INS_EXIT; i++) {
		I_icon_pos[i].t += I_icon_pos[i].x += dy;
		I_drawIcon(i, OFF);
	}
}

void I_drawIcon(icon, state)
int icon, state;
{
	XFillRectangle(display, I_window, C_gcIcon, (int)I_icon_pos[icon].x,
						(int)I_icon_pos[icon].y,
						(unsigned int)(I_icon_pos[icon].width + 1),
						(unsigned int)(I_icon_pos[icon].height + 1));
	V_surround(I_window, I_icon_pos[icon].x, I_icon_pos[icon].y,
					I_icon_pos[icon].width, I_icon_pos[icon].height, state);
	XDrawImageString(display, I_window, C_gcIconText,
							(int)(I_icon_pos[icon].t - state),
							(int)(I_icon_pos[icon].y + 12 - state),
							I_menu[icon], strlen(I_menu[icon]));
}

int I_clickedIcon (xClick, yClick)
int xClick, yClick;
{
register int i;

	for (i = 0; i <= INS_EXIT; i++)
		if (((xClick > I_icon_pos[i].x) &&
			(xClick < I_icon_pos[i].x + I_icon_pos[i].width)) &&
			((yClick > I_icon_pos[i].y) &&
			(yClick < I_icon_pos[i].y + I_icon_pos[i].height)))
			return (i);
	return INSCREEN;
}
