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

# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xos.h>
# include <X11/keysym.h>
# include <stdio.h>
# include <string.h>
# include <ctype.h>

# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "v_view.h"
# include "c_menu.h"
# include "v_extern.h"
# include "e_edit.h"

extern E_backspace();
extern E_homekey();
extern E_endkey();
extern E_delkey();
extern E_inskey();
extern E_rightkey();
extern E_leftkey();
extern E_upkey();
extern E_downkey();
extern E_pgupkey();
extern E_pgdnkey();
extern E_returnkey();
extern E_traitement();
extern E_delinekey();
extern E_insert_ligne();
extern E_affiche_page();
extern E_init_var();
extern E_cleoln();
extern E_del_curseur();
extern E_aff_curseur();
extern E_LectFic();
extern E_delbloc();
extern E_CopyBlocToLine();
extern E_ClearBuffer();
extern int	E_Effacer();
extern int	E_SauveFic();
extern int	E_NumeroLigne();
extern E_BlocToBuffer();
extern int	E_BreakPoint();
extern void E_EndText();
extern list_ligne *E_AllocBuf();

void traitedeuxpoints();
void traiteshift();
void traitecontrol();
void traitechiffre();
void traited();
void traitey();
void E_PrintError();
void E_Printatlastline();
void analysecmd();
int detect_w();
int detect_se();
void traiteslash();
int E_findPattern();

extern char E_LoadStr[];
extern int E_dx_win, E_dy_win;

#define rblk(i)   while((chaine[i] == ' ') && (chaine[i] != '\0')) i++
#define prblk(i)  i++; rblk(i);
#define rtblk(i)  rblk(i); if (chaine[i] == '\0')
#define prtblk(i) i++; rtblk(i)

char *E_fontname;
GC E_gcc, E_gci;
BOOL E_underControl = FALSE;
BOOL E_Modif;
int E_pos_x = 0;
int E_pos_y = 0;
list_ligne *buff_commande;
list_ligne *buff_search;
GC E_gc, E_gcgraphic;
XGCValues E_gc1_values;
BOOL commande = TRUE;
int E_font_height, E_font_width, E_font_ascent, E_font_descent;
int E_interligne = 0;

void
	traitedeuxpoints()
{
int pos = 1;
int i;
int out = FALSE;

	buff_commande = E_AllocBuf();
	buff_commande->prem_mot = (char *)mbkalloc(MAXCAR + 1);
	buff_commande->prem_mot[0] = ':';
	for (i = 1; i < MAXCAR + 1; i++)
		buff_commande->prem_mot[i] = '\0';
	E_Printatlastline(buff_commande->prem_mot);
	while(1) {
	char buffer;
	KeySym keysym;
		XNextEvent(display, &V_event);
		if (V_event.type == KeyPress && V_event.xany.window == E_window) {
			XLookupString(&V_event.xkey, &buffer, 1, &keysym, NULL);
			if (keysym == XK_Return) {
				out = FALSE;
				break;
			}
			if (pos < MAXCAR && keysym >= XK_space && keysym <= XK_ydiaeresis)
				buff_commande->prem_mot[pos++] = keysym;
			if (keysym == XK_Delete)
				if (pos > 1)
					buff_commande->prem_mot[--pos] = '\0';
				else {
					out = TRUE;
					break;
				}
			if (keysym == XK_Escape || keysym == XK_F11) {
				out = TRUE;
				break;
			}
			E_Printatlastline(buff_commande->prem_mot);
		} else
			allEvent();
	}
	if (strlen(buff_commande->prem_mot) > 1 && out != TRUE)
		analysecmd(&(buff_commande->prem_mot[1]));
	mbkfree(buff_commande->prem_mot);
	mbkfree(buff_commande);
}

void traiteslash()
{
int pos = 1;
int i;
int out = FALSE;

	if (buff_search != NULL) {
		mbkfree(buff_search->prem_mot);
		mbkfree(buff_search);
	}
	buff_search = E_AllocBuf();
	buff_search->prem_mot = (char *)mbkalloc(MAXCAR + 1);
	buff_search->prem_mot[0] = '/';
	for (i = 1; i < MAXCAR + 1; i++)
		buff_search->prem_mot[i] = '\0';
	E_Printatlastline(buff_search->prem_mot);
	while(1) {
	char buffer;
	KeySym keysym;
		XNextEvent(display, &V_event);
		if (V_event.type == KeyPress && V_event.xany.window == E_window) {
			XLookupString(&V_event.xkey, &buffer, 1, &keysym, NULL);
			if (keysym == XK_Return) {
				out = FALSE;
				break;
			}
			if (pos < MAXCAR && (keysym >= XK_space) && (keysym <= XK_ydiaeresis))
				buff_search->prem_mot[pos++] = keysym;
			if (keysym == XK_Delete)
				if (pos > 0)
					 buff_search->prem_mot[--pos] = '\0';
				else {
					out = TRUE;
					break;
				}
			if (keysym == XK_Escape || keysym == XK_F11) {
				out = TRUE;
				break;
			}
			E_Printatlastline(buff_search->prem_mot);
		} else
			allEvent();
	}
	if (out != TRUE) {
	int i, xpos;
		if (buff_search->prem_mot[1] == '\0') {
			E_PrintError(7);
			return;
		}
		if((i = E_findPattern(&(buff_search->prem_mot[1]),
				E_NumeroLigne(E_ligne_courant), &xpos)) == -1) {
			E_PrintError(4);
			return;
		}
		gotoline(i);
		E_del_curseur();
		for (i = 0; i < xpos; i++)
			droite();
		E_aff_curseur();
	}
}

char
	*E_strstr(s, t)
char *s, *t;
{
int i, j, k;
	for(i = 0; s[i] != '\0'; i++) {
		for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; j++, k++);
		if (k > 0 && t[k] == '\0')
			return &s[i];
	}
	return NULL;
}

void
	E_strscat(s, t)
char *s, *t;
{
	while(*s != '\0')
		s++;
	s--;
	while(*s == ' ')
		s--;
	while(*t == ' ')
		t++;
	*++s = ' ';
	while(*++s = *t++);
}

int
	E_findPattern(pattern, from, pos)
char *pattern;
int from;
int *pos;
{
static int i;
static char *str;
static list_ligne *l;
BOOL stop = FALSE;

	if (i != from) {
		for (l = E_debut, i = 1; i <= from; i++)
			l = l->pt_suiv;
		if (l == NULL) 
			l = E_debut;
		str = l->prem_mot;
	}
	while(stop == FALSE) {
	char *tmpstr;
		if ((tmpstr = E_strstr(str, pattern)) != NULL) {
			*pos = (int)(tmpstr - l->prem_mot);
			str = tmpstr + strlen(pattern);
			return i;
		}
		if ((l = l->pt_suiv) == NULL) {
			l = E_debut;
			i = 0;
		}
		str = l->prem_mot;
		if (++i == from)
			stop = TRUE;
	}
	return -1;
}

void
	traitechiffre(keysym, value)
KeySym keysym;
int *value;
{
	*value = (*value * 10) + (keysym - '0');
}

void
	traited(value, from)
int value;
int from;
{
char data[80];
int line;

	sprintf(data, "%d %s deleted", value == 0 ? 1 : value,
			value <= 1 ? "line" : "lines");
	if (value != 0)
		value--;
	line = from == -1 ? E_NumeroLigne(E_ligne_courant) : from;
	if ((line + value) <= E_nbre_ligne) {
		E_numligne_debbloc = line;
		E_numligne_finbloc = line + value;
		E_delbloc();
		E_Printatlastline(data);
		return;
	}
	E_PrintError(1);
}

void
	traitey(value, from)
int value;
int from;
{
char data[80];
int line;

	sprintf(data, "%d %s yanked", value == 0 ? 1 : value,
			value <= 1 ? "line" : "lines");
	if (value != 0)
		value--;
	line = from == -1 ? E_NumeroLigne(E_ligne_courant) : from;
	if ((line + value) <= E_nbre_ligne) {
		E_numligne_debbloc = line;
		E_numligne_finbloc = line + value;
		E_BlocToBuffer();
		E_Printatlastline(data);
	} else
		E_PrintError(1);
}

void
	E_PrintError(data)
int data;
{
	switch (data) {
		case 1:
			E_Printatlastline("Not that many lines in buffer");
			break;
		case 2:
			E_Printatlastline("Not an editor command");
			break;
		case 3:
			E_Printatlastline("Second address exeed first");
			break;
		case 4:
			E_Printatlastline("Pattern not found");
			break;
		case 5:
			E_Printatlastline("Extra characters at end of command");
			break;
		case 6:
			E_Printatlastline("File exists - use \"!\" to overwrite");
			break;
		case 7:
			E_Printatlastline("No previous regular expression (pattern)");
			break;
		case 8:
			E_Printatlastline("Substitute pattern match failed");
			break;
		default:
			break;
	}
}

void
	E_Printatlastline(str)
char *str;
{
	XClearArea(display, E_window, (ORIGINEX) * E_font_width,
					(((E_maxligne - 1 + ORIGINEY - 1) * E_interligne) +
					2 * E_font_descent), 0, E_font_height, FALSE);
	XDrawImageString(display, E_window, C_gcIconTextInv, ORIGINEX * E_font_width,
					(E_maxligne + ORIGINEY - 1) * E_interligne, str, strlen(str));
}

int
	strToInt(chaine, pos_chaine)
char *chaine;
int *pos_chaine;
{
int value = 0;

	while(chaine[*pos_chaine] <= '9' && chaine[*pos_chaine] >= '0')
		value = value * 10 + (chaine[(*pos_chaine)++] - '0');
	return value > 0 ? value : -1;
}

int
	detect_w(chaine, line1, line2)
char *chaine;
int line1, line2;
{
BOOL overwrite;
int i = 0, j;
char filename[40];

	rtblk(i) {
		sprintf(filename, "%s", E_LoadStr);
		E_SauveFic(filename);
		sprintf(filename, "\"%s\" %d %s", E_LoadStr, E_nbre_ligne,
					E_nbre_ligne == 1 ? "line" : "lines");
		E_Printatlastline(filename);
		return TRUE;
	}
	if (chaine[i] == '!') {
		i++;
		overwrite = TRUE;
		rblk(i);
	} else
		overwrite = FALSE;
	for (j = 0; (j < 14) && (chaine[i] != ' ') && (chaine[i] != '\0'); j++, i++)
		filename[j] = chaine[i];
	filename[j] = '\0';
	if (chaine[i] == '\0') {
		if (overwrite == FALSE)
			if (fopen(filename, "r") != NULL) {
				E_PrintError(6);
				return FALSE;
			}
		E_SauveFic(filename);
		sprintf(filename, "\"%s\" %d %s", E_LoadStr, E_nbre_ligne,
					E_nbre_ligne == 1 ? "line" : "lines");
		E_Printatlastline(filename);
		return TRUE;
	}
	E_PrintError(5);
	return FALSE;
}

int
	detect_se(chaine)
char *chaine;
{
int i = 0;

	rblk(i);
	if (strcmp(&chaine[i], "nu") == 0) {
		E_del_curseur();
		E_offnum = 5;
		E_affiche_page(E_page_courant, NULL);
		E_aff_curseur();
		return TRUE;
	} else if (strcmp(&chaine[i], "nonu") == 0) {
		E_del_curseur();
		E_offnum = 0;
		E_affiche_page(E_page_courant, NULL);
		E_aff_curseur();
		return TRUE;
	} else {
		E_Printatlastline("Allowed options are 'nu' or 'nonu'");
		return FALSE;
	}
}

detect_s(cmdstr, from, to)
char *cmdstr;
int from, to;
{
list_ligne *pln;
char *search, *replace, *mode, c;
int next, l, ls, lr, len = strlen(cmdstr) - 1;
BOOL first = TRUE;

	search = cmdstr;
	if ((replace = strchr(cmdstr, '/')) == NULL) {
		E_PrintError(8);
		return;
	}
	*replace++ = '\0';
	if ((mode = strchr(replace,'/')) != NULL) {
		*mode = '\0';
		if (len > (int)(mode - cmdstr))
			++mode;
		else
			mode = NULL;
	}
	c = mode == NULL ? 'n' : *mode;
	if (c != 'g' && c != 'n' && c != 'c') {
		E_PrintError(5);
		return;
	}
	ls = strlen(search);
	lr = strlen(replace);
	l = lr > ls ? ls : lr;
	next = from;
	E_del_curseur();
	while(TRUE) {
	int i, line, xpos;
		if ((line = E_findPattern(search, next, &xpos)) == -1) {
			if (first == TRUE) {
				E_PrintError(8);
				return;
			} else
				break;
		}
		if (line < from && line > to && first == TRUE) {
			E_PrintError(8);
			break;
		}
		for (i = 1, pln = E_debut; i < line; i++, pln = pln->pt_suiv);
/*
		if (c == 'c') {
		char s[MAXCAR];
		char b;
		KeySym k;
			sprintf(s,"%s :y/n",pln->prem_mot);
			E_Printatlastline(s);
			XNextEvent(display, &V_event);
			XLookupString(&V_event.xkey, &b, 1, &k, NULL);
			if (b == 'n')
				continue;
		}
*/
		for (i = 0; i < l; i++)
			pln->prem_mot[xpos + i] = replace[i];
		if (lr > ls)
			for (i = ls; i < lr; i ++)
				insertion(replace[i], xpos + i, pln);
		else if (ls > lr)
			for (i = lr; i < ls; i ++)
				suppression(xpos + lr, pln);
		first = FALSE;
		E_pos_x = xpos;
	}
	E_del_curseur();
	gotoline(E_NumeroLigne(pln));
	E_aff_curseur();
}
		
void
	analysecmd (chaine)
char *chaine;
{
int ligne1 = 0;
int ligne2 = 0;
int debcmd = 0;
char c;

	rblk(debcmd);
	switch(c = chaine[debcmd++]) { /* treat the first field */
		case 'd':
		case 'y':
			rtblk(debcmd) {
				if (c == 'd') {
					traited(0, -1);
					E_Modif = TRUE;
				} else
					traitey(0, -1);
			} else if (isdigit(chaine[debcmd])) {
			int nbline = strToInt(chaine, &debcmd);
				rblk(debcmd);
				if (chaine[debcmd] != '\0') {
					E_PrintError(5);
					return;
				}
				if (c == 'd')
					traited(nbline, -1);
				else
					traitey(nbline, -1);
			} else E_PrintError(5);
			return;
		case 'w':
			rblk(debcmd);
			detect_w(&chaine[debcmd], 1, E_nbre_ligne);
			return;
		case 'f':
			{
			char status[40];
				rblk(debcmd);
				if (chaine[debcmd] != '\0') {
					E_PrintError(5);
					return;
				}
				sprintf(status, "\"%s\"%sline %d of %d --%d%%--", E_LoadStr,
							E_Modif == TRUE ? " [Modified] " : " ",
							E_NumeroLigne(E_ligne_courant), E_nbre_ligne,
							100 * E_NumeroLigne(E_ligne_courant) / E_nbre_ligne);
				E_Printatlastline(status);
				return;
			}
		case '.':
			rtblk(debcmd) {
				gotoline(E_NumeroLigne(E_ligne_courant));
				return;
			}
			if (chaine[debcmd] == ',') {
				ligne1 = E_NumeroLigne(E_ligne_courant);
				break;
			} else if (chaine[debcmd] == '+') {
				prtblk(debcmd) {
					E_PrintError(5);
					return;
				}
				ligne1 = E_NumeroLigne(E_ligne_courant) + strToInt(chaine, &debcmd);
				rtblk(debcmd) {
					gotoline(ligne1);
					return;
				}
				break;
			} else if (chaine[debcmd] == '-') {
				prtblk(debcmd) {
					E_PrintError(5);
					return;
				}
				ligne1 = E_NumeroLigne(E_ligne_courant) - strToInt(chaine, &debcmd);
				rtblk(debcmd) {
					gotoline(ligne1);
					return;
				}
			}
			break;
		case '%':
			ligne1 = 1;
			ligne2 = E_nbre_ligne;
			break;
		case '$':
			E_EndText();
			return;
		case 's':
			if (chaine[debcmd] == 'e') {
				prblk(debcmd);
				detect_se(&chaine[debcmd]);
				return;
			} else if (chaine[debcmd] == '/') {
				ligne1 = ligne2 = E_NumeroLigne(E_ligne_courant);
				detect_s(&chaine[++debcmd], ligne1, ligne2);
				return;
			} else
				E_PrintError(2);
			return;
		case 'r':
			E_Printatlastline("Function \":r\" not yet implemented, sorry");
			return;
		case 'e':
			E_Printatlastline("Use the menu bar to load a file");
			return;
		case 'q':
			E_Printatlastline("Use the menu bar to quit");
			return;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			--debcmd;
			ligne1 = strToInt(chaine, &debcmd);
			ligne1 = ligne1 > 0 ? ligne1 : 1;
			rtblk(debcmd) {
				if (ligne1 <= E_nbre_ligne) {
					gotoline(ligne1);
					return;
				} else {
					E_PrintError(1);
					return;
				}
			}
		default:
			break;
	}

	switch(c = chaine[debcmd++]) {
		case 'd':
		case 'y':
			rtblk(debcmd) {
				if (c == 'd') {
					traited(0, ligne1);
					E_Modif = TRUE;
				} else
					traitey(0, ligne1);
			} else if (isdigit(chaine[debcmd])) {
			int nbline = strToInt(chaine, &debcmd);
				rblk(debcmd);
				if (chaine[debcmd] != '\0') {
					E_PrintError(5);
					return;
				}
				if (c == 'd')
					traited(nbline, ligne1);
				else
					traitey(nbline, ligne1);
			} else E_PrintError(5);
			return;
		case 's':
			if (chaine[debcmd] == '/') {
				if (ligne2 == 0)
					ligne2 = ligne1;
				detect_s(&chaine[++debcmd], ligne1, ligne2);
				return;
			} else
				E_PrintError(2);
		case ',':
			rblk(debcmd);
			switch(c = chaine[debcmd]) {
				case '.':
					prtblk(debcmd) {
						E_PrintError(1);
						return;
					}
					if (chaine[debcmd] == '+') {
						prtblk(debcmd) {
							E_PrintError(5);
							return;
						}
						ligne2 = E_NumeroLigne(E_ligne_courant) +
														strToInt(chaine, &debcmd);
					} else if (chaine[debcmd] == '-') {
						prtblk(debcmd) {
							E_PrintError(5);
							return;
						}
						ligne2 = E_NumeroLigne(E_ligne_courant) -
														strToInt(chaine, &debcmd);
					}
					break;
				case '$':
					debcmd++;
					ligne2 = E_nbre_ligne;
					break;
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					ligne2 = strToInt(chaine, &debcmd);
					break;
		}
		break;
	}

	rblk(debcmd);
	switch(c = chaine[debcmd++]) {
		case 'd':
		case 'y':
			rtblk(debcmd) {
				if (ligne2 < ligne1) {
					E_PrintError(3);
					return;
				}
				if (c == 'd') {
					traited(ligne2 - ligne1, ligne1);
					E_Modif = TRUE;
				} else
					traitey(ligne2 - ligne1, ligne1);
			} else {
				E_PrintError(3);
				return;
			}
			break;
		case 's':
			if (chaine[debcmd] == '/') {
				if (ligne2 > E_nbre_ligne) {
					E_PrintError(1);
					return;
				}
				if (ligne1 > ligne2) {
					E_PrintError(3);
					return;
				}
				detect_s(&chaine[++debcmd], ligne1, ligne2);
				return;
			} else
				E_PrintError(2);
			break;
		case 'w':
			detect_w(&chaine[debcmd], ligne1, ligne2);
			break;
		default:
			E_PrintError(2);
	}
}

XFontStruct *font;

void
	E_Init_Edit()
{
	E_gcc = XCreateGC(display, E_window, 0, NULL);
	XSetForeground(display, E_gcc, V_colors[COLOR_RED]);
	XSetLineAttributes(display, E_gcc, 3, LineSolid, CapButt, JoinRound);
	E_gci = XCreateGC(display, E_window, 0, NULL);
	XSetForeground(display, E_gci, V_colors[COLOR_BLUE]);
	XSetLineAttributes(display, E_gci, 3, LineSolid, CapButt, JoinRound);
	E_gc = XCreateGC(display, E_window, 0, NULL);
	XSetFont(display, E_gc, font->fid);
	E_init_var();
	E_ClearBuffer();
	E_interligne = E_font_height + E_font_descent;
	buff_search = NULL;
	commande = TRUE;
}

void
	E_Set_Font_Edit()
{
	if (!(font = XLoadQueryFont(display, E_fontname))) {
		fprintf(stderr, "GenView: Error: cannot access %s font (X11 ressource)\n",
				 E_fontname);
		exit( 7 );
	}
	/* I'd like to know if it's a true fixed width font, but I can't.
 	if (font->max_bounds.rbearing - font->min_bounds.lbearing !=
			font->min_bounds.rbearing - font->max_bounds.lbearing) {
		fprintf(stderr, "GenView: Error: %s font is not fixed-width\n",
				 E_fontname);
		exit( 8 );
	} */
	E_font_height = font->ascent + font->descent;
	E_font_width = font->max_bounds.rbearing - font->min_bounds.lbearing;
	E_font_ascent = font->max_bounds.ascent;
	E_font_descent = font->descent;
}

void
	E_Expose_Edit()
{
	E_affiche_page(E_page_courant, NULL);
	E_aff_curseur();
}

void
	E_ConfigNotify_Edit()
{
int x;
Window root;

	E_maxligne = E_dy_win / E_interligne - ORIGINEY;
/* E_maxcar = E_dx_win / E_font_width - ORIGINEX; */
}

void
	E_KeyPress(keysym)
KeySym keysym;
{
static int value;
static BOOL is_d;
static BOOL is_y;

	if (commande == FALSE) {
		switch (keysym) {
			case XK_Delete:
			case XK_BackSpace:
				E_Modif = TRUE;
				E_backspace();
				break;
			case XK_F11:	
			case XK_Escape:
				commande = TRUE;
				E_aff_curseur();
				break;
			case XK_Right:
				E_rightkey();
				break;
			case XK_Left:
				E_leftkey();
				break;
			case XK_Up:
				E_upkey();
				break;
			case XK_Down:
				E_downkey();
				break;
			case XK_Prior:
				E_pgupkey();
				break;
			case XK_Next:
				E_pgdnkey();
				break;
			case XK_Return:
			case XK_Linefeed:
			case XK_KP_Enter:
				E_Modif = TRUE;
				E_returnkey();
				break;
			default:	
				if (keysym >= XK_space && keysym <= XK_ydiaeresis) {
					E_Modif = TRUE;
					E_traitement (keysym);
				}
				break;
		}
	} else {
		switch (keysym) {
			case XK_Shift_L:
			case XK_Shift_R:
				break;
			case XK_Control_L:
			case XK_Control_R:
				E_underControl = TRUE;
				break;
			case XK_R:
				E_mode = INDENT;
				commande = FALSE;
				E_aff_curseur();
				E_Modif = TRUE;
				break;
			case XK_O:
				E_upkey();
				E_end();
				E_returnkey();
				E_mode = INSERT;
				E_Modif = TRUE;
				commande = FALSE;
				E_aff_curseur();
				break;
			case XK_dollar:
				E_endkey();
				break;
			case XK_I:
				E_homekey();
				E_mode = INSERT;
				commande = FALSE;
				E_aff_curseur();
				E_Modif = TRUE;
				break;
			case XK_A:
				E_endkey();
				E_rightkey();
				E_affiche_page(E_page_courant, 0);
				E_del_curseur();
				E_aff_curseur();
				E_mode = INSERT;
				E_Modif = TRUE;
				commande = FALSE;
				E_aff_curseur();
				break;
			case XK_colon:
				traitedeuxpoints();
				break;
			case XK_0:
				if (value != 0)
					traitechiffre(keysym, &value);
				else
					E_homekey();
				break;
			case XK_J:
				{
				int l;
				if (E_ligne_courant == NULL)
					break;
				if (E_ligne_courant->pt_suiv == NULL)
					break;
				if ((l = strlen(E_ligne_courant->prem_mot)) + 1 +
						strlen(E_ligne_courant->pt_suiv->prem_mot) > MAXCAR ) {
					E_Printatlastline("Line too long");
					break;
				}
				E_strscat(E_ligne_courant->prem_mot,
							E_ligne_courant->pt_suiv->prem_mot);
				E_numligne_finbloc = E_numligne_debbloc =
					E_NumeroLigne(E_ligne_courant->pt_suiv);
				E_delbloc();
				E_del_curseur();
				zero();
				while(--l >= 0)
					droite();
				E_Modif = TRUE;
				E_aff_curseur();
				break;
				}
			case XK_1:
			case XK_2:
			case XK_3:
			case XK_4:
			case XK_5:
			case XK_6:
			case XK_7:
			case XK_8:
			case XK_9:
				traitechiffre(keysym, &value);
				break;
			case XK_d:
				if (is_d == FALSE)
					is_d = TRUE;
				else {
					traited (value == 0 ? 1 : value, -1);
					value = 0;
					is_d = FALSE;
					E_Modif = TRUE;
				}
				break;
			case XK_n:
				if (buff_search != NULL) {
				int i, xpos;
					if ((i = E_findPattern(&(buff_search->prem_mot[1]),
							E_NumeroLigne(E_ligne_courant), &xpos)) == -1) {
						E_PrintError(4);
						break;
					}
					gotoline(i);
					E_del_curseur();
					for (i = 0; i < xpos; i++)
						droite();
					E_aff_curseur();
				} else
					E_PrintError(7);
				break;
			case XK_y:
				if (is_y == FALSE)
					is_y = TRUE;
				else {
					traitey (value, -1);
					value = 0;
					is_y = FALSE;
				}
				break;
			case XK_Right:
			case XK_l:
				if (E_underControl == FALSE)
					E_rightkey();
				else {
					E_affiche_page(E_page_courant, NULL);
					E_aff_curseur();
				}
				break;
			case XK_Left:
			case XK_h:
				E_leftkey();
				break;
			case XK_Up:
			case XK_k:
				E_upkey();
				break;
			case XK_Down:
			case XK_j:
				E_downkey();
				break;
			case XK_b:
				if (E_underControl == FALSE)
					break;
			case XK_Prior:	/* page up */
				E_pgupkey();
				break;
			case XK_f:
				if (E_underControl == FALSE)
					break;
			case XK_Next:	/*page down */
				E_pgdnkey();
				break;
			case XK_o:	/* cree une ligne en E_mode insertion */
				E_end();
				E_returnkey();
				E_mode = INSERT;
				commande = FALSE;
				E_aff_curseur();
				E_Modif = TRUE;
				break;
			case XK_r:
				{
				char	buffer;
				KeySym key;
					while(1) {
						XNextEvent(display, &V_event);
						if (V_event.type == KeyPress 
								&& V_event.xany.window == E_window) {
							XLookupString(&V_event.xkey, &buffer, 1, &key, NULL);
							if (key == XK_Shift_R || key == XK_Shift_L)
								continue;
							if (key >= XK_space && key <= XK_ydiaeresis) {
								E_mode = INDENT;
								E_traitement (key);
								E_leftkey();
								E_mode = INSERT;
								E_Modif = TRUE;
								break;
							}
						} else 
							allEvent();
					}
				}
				break;
			case XK_asciitilde:
				{
				char buffer;
					buffer = E_ligne_inter->prem_mot[E_pos_x];
					if (isupper(buffer))
						buffer = tolower(buffer);
					else if (islower(buffer))
						buffer = toupper(buffer);
					else {
						E_rightkey();
						break;
					}
					E_mode = INDENT;
					keysym = buffer;
					E_traitement(keysym);
					E_mode = INSERT;
					E_Modif = TRUE;
					break;
				}
			case XK_i:	/* insert at current cursor position */
				E_mode = INSERT;
				E_Modif = TRUE;
				commande = FALSE;
				E_aff_curseur();
				break;
			case XK_a:	/* insert after current cursor position */
				E_mode = INSERT;
				E_rightkey();
				E_Modif = TRUE;
				commande = FALSE;
				E_aff_curseur();
				break;
			case XK_P:	/* paste a block up */
				E_upkey();
				E_CopyBlocToLine();
				E_Modif = TRUE;
				break;
			case XK_p:	/* paste a block down */
				E_CopyBlocToLine();
				E_Modif = TRUE;
				break;
			case XK_x:	/* delete character */
				E_delkey();
				E_Modif = TRUE;
				break;
			case XK_X:
				E_leftkey();
				E_delkey();
				E_Modif = TRUE;
				break;
			case XK_slash:
				traiteslash();
				break;
			case XK_g:
				if (E_underControl == TRUE) {
				char status[40];
					sprintf(status, "\"%s\"%sline %d of %d --%d%%--", E_LoadStr,
								E_Modif == TRUE ? " [Modified] " : " ",
								E_NumeroLigne(E_ligne_courant), E_nbre_ligne,
								100 * E_NumeroLigne(E_ligne_courant) / E_nbre_ligne);
					E_Printatlastline(status);
					break;
				}
				if (value < E_nbre_ligne && value > 0)
					gotoline(value);
				else
					E_PrintError(1);
				break;
			default:
				E_affiche_page(E_page_courant, NULL);
				E_aff_curseur();
				break;
		}	/* command */

		if (keysym != XK_d && is_d == TRUE) {
			is_d = FALSE;
			value = 0;
		}
		if (keysym != XK_y && is_y == TRUE) {
			is_y = FALSE;
			value = 0;
		}
		if (keysym != XK_d && keysym != XK_y &&
				(keysym < XK_0 || keysym > XK_9) && value != 0)
			value = 0;
	}
}

void
	E_KeyRelease(key)
KeySym key;
{
	switch (key) {
		case XK_Control_L:
		case XK_Control_R:
			E_underControl = FALSE;
			break;
		default:
			break;
	}
}
