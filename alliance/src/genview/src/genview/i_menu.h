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

/* inspect window */

#define INS_PGUP 0
#define INS_PGUP_X 0
#define INS_PGUP_Y 2
#define INS_PGUP_DX 68
#define INS_PGUP_DY 16
#define INS_PGUP_T 13

#define INS_PGDN 1
#define INS_PGDN_X INS_PGUP_X
#define INS_PGDN_Y INS_PGUP_Y+INS_PGUP_DY+2
#define INS_PGDN_DX 89
#define INS_PGDN_DY 16
#define INS_PGDN_T 17

#define INS_LNUP 2
#define INS_LNUP_X INS_PGUP_X+INS_PGUP_DX+2
#define INS_LNUP_Y 2
#define INS_LNUP_DX 68
#define INS_LNUP_DY 16
#define INS_LNUP_T 13

#define INS_LNDN 3
#define INS_LNDN_X INS_PGDN_X+INS_PGDN_DX+2
#define INS_LNDN_Y INS_LNUP_Y+INS_LNUP_DY+2
#define INS_LNDN_DX 89
#define INS_LNDN_DY 16
#define INS_LNDN_T 18

#define INS_EXIT 4
#define INS_EXIT_X INS_LNUP_X+INS_LNUP_DX+2
#define INS_EXIT_Y 2
#define INS_EXIT_DX 40
#define INS_EXIT_DY 16
#define INS_EXIT_T 14

