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

#define BOOL int
#define TRUE 1
#define FALSE 0
#define MAXCAR 132 
#define INSERT 0
#define INDENT (-1)
#define ORIGINEX 2
#define ORIGINEY 4 
#define SPACE 32
#define Larg_Tab 3 
#define NORMAL	0 
#define INVERSE 1 
#define BREAKPOINT 1	
#define NOTFOUND (-1)

typedef struct point_ligne {
	struct point_ligne *pt_suiv,*pt_prec;
	char *prem_mot;
	int attribut;
} list_ligne;

extern		int E_font_height;
extern		int E_font_width;
extern		int E_font_ascent;
extern		int E_font_descent;
extern 		int E_interligne;
extern		GC E_gc,E_gcc, E_gci;
extern		int E_pos_x;
extern		int E_pos_y;
extern		list_ligne *E_debut;
extern		list_ligne *E_page_courant;
extern		list_ligne *E_ligne_courant;
extern		list_ligne *E_ligne_inter;
extern		int E_mode;
extern		int E_scroll_up;
extern		int E_scroll_down;
extern		int E_maxligne;
extern		int E_nbre_ligne;
extern		int E_numligne_debbloc;
extern		int E_numligne_finbloc; 
extern		int E_numligne;
extern		int E_offnum;
extern		int E_scrtxt;
extern		BOOL E_Modif;
extern		char *font_name;
extern		XFontStruct *font_info;
