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
# include "c_menu.h"
# include "v_menu.h"
# include "v_extern.h"

extern long xscreen, yscreen, dxscreen, dyscreen;
extern Font V_menuFont;

char *helli = "Inspecting";
int I_x, I_y, I_dx, I_dy;
FILE *insp;

void I_open ()
{
XSizeHints hint;
XWindowAttributes attribs;

	hint.width = 222;
	hint.height = 380;
	hint.min_width = 222;
	hint.min_height = 100;
	hint.flags = PPosition | PSize | PMinSize;
	hint.x = 250;
	hint.y = 250;

	I_window = XCreateSimpleWindow (display, DefaultRootWindow (display),
			hint.x, hint.y, hint.width, hint.height,
			5, V_colors[COLOR_FGND], V_colors[COLOR_BGND]);

	XSelectInput (display, I_window, ButtonReleaseMask
			| ButtonPressMask | ExposureMask);

	XSetStandardProperties (display, I_window, helli,
			"Inspect", None, NULL, 0, &hint);
	XMapRaised (display, I_window);
	XGetWindowAttributes (display, I_window, &attribs);
	I_x = attribs.x;
	I_y = attribs.y;
	I_dx = attribs.width;
	I_dy = attribs.height;
}

int I_event ()
{
static int witchIcon;

	switch (V_event.type) {
		case Expose:
			I_setMenu ();
			I_drawMenu ();
			if (V_event.xexpose.count == 0)
				inspect (xscreen + (long) (V_I_pos.x / scale),
								yscreen + dyscreen + (long) (YMENU / scale)
								- (long) (((V_I_pos.y + V_I_pos.height) / scale)),
								(long) (V_I_pos.width / scale),
								(long) (V_I_pos.height / scale));
			I_out (0);
			break;

		case ButtonPress:
			witchIcon = I_clickedIcon (V_event.xbutton.x, V_event.xbutton.y);
			if (witchIcon != INSCREEN)
				I_drawIcon(witchIcon, ON);
			break;
		case ButtonRelease:
			switch(witchIcon) {
				case INS_PGUP:
					i_pageUp();
					I_drawIcon(INS_PGUP, OFF);
					break;
				case INS_PGDN:
					i_pageDn ();
					I_drawIcon(INS_PGDN, OFF);
					break;
				case INS_LNUP:
					i_lineUp ();
					I_drawIcon(INS_LNUP, OFF);
					break;
				case INS_LNDN:
					i_lineDn ();
					I_drawIcon(INS_LNDN, OFF);
					break;
				case INS_EXIT:
					I_out (1);
					I_drawIcon(INS_EXIT, OFF);
					XDestroyWindow (display, I_window);
					return CLOSE_WINDOW;
			}
			break;
		}
	return NO_WINDOW_ACTION;
}
