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

# include <stdio.h>
# include <string.h>
# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/Xos.h>
# include <X11/keysym.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

# include "v_view.h"
# include "c_menu.h"
# include "v_extern.h"
# include "e_edit.h"

list_ligne *E_debut;
list_ligne *E_page_courant;
list_ligne *E_ligne_courant;
list_ligne *E_ligne_inter;
list_ligne *E_NEXT;
list_ligne *E_BufferSauve = NULL;
int E_mode = INSERT;  /* insertion ou indentation*/
int E_scroll_up = FALSE;
int E_scroll_down = FALSE;
int E_maxligne = 5;
int E_nbre_ligne = 0;
int E_numligne_debbloc = 0;
int E_numligne_finbloc = 0;
int E_numligne = 0;
int E_offnum = 0;
int E_strtxt = 0;

extern BOOL commande;
extern int E_dx_win, E_dy_win;

int E_NumeroLigne();
void init_ligne();
list_ligne *alloc_ligne();
void E_affiche_page();
void affiche_rc();
void aff_indent();
void aff_insert();
void E_aff_curseur();
void E_init_var();
void insertion();
void indentation();
void suppression();
void add_space();
void zero();
void droite();
void gauche();
void haut();
void bas();
void E_backspace();
void E_homekey();
void E_endkey();
void E_delkey();
void E_inskey();
void E_rightkey();
void E_leftkey();
void E_upkey();
void E_downkey();
void E_pgupkey();
void E_pgdnkey();
void E_returnkey();
void E_traitement();
void E_delinekey();
void E_insert_ligne();
void E_cleoln();
void E_del_curseur();
list_ligne *E_LectFic();
list_ligne *OpenFic();
void E_delbloc();
void E_ClearBuffer();
void E_CopyBlocToLine();
int E_Effacer();
int E_SauveFic();
int E_BreakPoint();
void cercle();
void clear_break();
void clear_attribVideo();
void clear_attribBreak();
void E_EndText();
int gotoline();
void E_BlocToBuffer();
void E_Printatlinenumber();
int E_Ligne();

void
  E_backspace()
{
  E_del_curseur();
  if (E_pos_x > 0) {
    gauche();
    suppression(E_pos_x, E_ligne_inter);
    aff_insert(E_pos_x, E_ligne_inter);
  }
  E_aff_curseur();
}

void
  E_homekey()
{
  E_del_curseur();
  zero();
  E_aff_curseur();
}

void
  E_endkey()
{
int i,l  = strlen(E_ligne_inter->prem_mot);

  E_del_curseur();
  E_pos_x = 0;
  for (i = 0; i < l; i++)
    droite();
  E_aff_curseur();
}

void
  E_end()
{
  E_del_curseur();
  E_pos_x = strlen(E_ligne_inter->prem_mot) + 1;
  E_del_curseur();
}

void
  E_delkey()
{
  E_del_curseur();
  suppression(E_pos_x, E_ligne_inter);
  aff_insert(E_pos_x, E_ligne_inter);
  E_aff_curseur();
}

void
  E_inskey()
{
  E_del_curseur();
  E_mode = ~E_mode;
  E_aff_curseur();
}

void
  E_rightkey()
{
  E_del_curseur();
  droite();
  E_aff_curseur();
}

void
  E_leftkey()
{
  E_del_curseur();
  gauche();
  E_aff_curseur();
}

void
  E_upkey()
{
  E_del_curseur();
  if (E_pos_y != 0)
    haut();
  else
    E_scroll_up = TRUE;

  if (E_ligne_courant->pt_prec != NULL) {
    E_ligne_courant = E_ligne_courant->pt_prec;
    E_ligne_inter = E_ligne_courant;
  }
  if (E_scroll_up == TRUE && E_page_courant->pt_prec != NULL)
    affiche_rc();
  E_scroll_up = FALSE;
  E_aff_curseur();
}

void
  E_downkey()
{
  E_del_curseur();
  if (E_pos_y != E_maxligne - 1 && E_ligne_courant->pt_suiv != NULL)
    bas();
  else
    E_scroll_down = TRUE;
  if (E_ligne_courant->pt_suiv != NULL) {
    E_ligne_courant = E_ligne_courant->pt_suiv;
    E_ligne_inter = E_ligne_courant;
  } else
    E_scroll_down = FALSE;
  if (E_scroll_down == TRUE && E_page_courant->pt_suiv != NULL)
    affiche_rc();
  E_scroll_down = FALSE;
  E_aff_curseur();
}

void
  E_pgupkey()
{
int i;

  XClearArea(display, E_window, 0, ORIGINEY * E_interligne, 
            E_dx_win, E_dy_win, False);
  for (i = 0; i < E_maxligne && E_page_courant->pt_prec != NULL; i++) {
    E_page_courant = E_page_courant->pt_prec;
    E_ligne_courant = E_ligne_courant->pt_prec;
  }
  if (E_page_courant == E_debut) {
    E_ligne_courant = E_debut;
    zero();
    E_pos_y = 0;
  }
  E_ligne_inter = E_ligne_courant;
  E_affiche_page(E_page_courant, 0);
  E_aff_curseur();
}

void
  E_pgdnkey()
{
int i, j = 0;

  XClearArea(display, E_window, 0, ORIGINEY * E_interligne, 
            E_dx_win, E_dy_win, False);
  for (i = 0; i < E_maxligne && E_page_courant->pt_suiv != NULL; i++) {
    E_page_courant = E_page_courant->pt_suiv;
    if (E_ligne_courant->pt_suiv != NULL) {
      E_ligne_courant = E_ligne_courant->pt_suiv;
      j++;
    }
  }
  E_affiche_page(E_page_courant, 0);
  if (j != i) {
    for (E_ligne_courant = E_page_courant, j = 0; 
        E_ligne_courant->pt_suiv != NULL; 
        E_ligne_courant = E_ligne_courant->pt_suiv, j++);
    zero();
    E_pos_y = j;
  }
  E_ligne_inter = E_ligne_courant;
  E_aff_curseur();
}

void
  E_returnkey()
{
int  taille, i;

  E_del_curseur();
  taille = strlen(E_ligne_inter->prem_mot);
  if (E_mode == INSERT) {
  int j = 0, pos = E_pos_x;
    E_NEXT = E_ligne_courant->pt_suiv;
    E_ligne_courant->pt_suiv = alloc_ligne();
    E_ligne_courant->pt_suiv->prem_mot = (char *)mbkalloc(MAXCAR + 1);
    init_ligne(E_ligne_courant->pt_suiv, MAXCAR + 1);
    E_ligne_courant->pt_suiv->pt_suiv = E_NEXT;
    E_ligne_courant->pt_suiv->pt_prec = E_ligne_courant;
    for (i = pos; i <= taille; i++) {
      E_ligne_courant->pt_suiv->prem_mot[j++] =
          E_ligne_inter->prem_mot[i];
      E_ligne_inter->prem_mot[i] = '\0';
    }
    E_ligne_courant->prem_mot = E_ligne_inter->prem_mot;
    E_ligne_courant->pt_suiv->attribut = E_ligne_courant->attribut & 0x01;
    E_ligne_courant = E_ligne_courant->pt_suiv;
    E_ligne_inter = E_ligne_courant;
    if (E_NEXT != NULL)
      E_ligne_courant->pt_suiv->pt_prec = E_ligne_courant;
    affiche_rc();
    if (E_pos_y != E_maxligne - 1)
      E_pos_y++;
    zero();
    E_aff_curseur();
  } else if (E_mode == INDENT) {
    if (E_ligne_courant->pt_suiv != NULL) {
      E_ligne_courant = E_ligne_courant->pt_suiv;
      E_ligne_inter = E_ligne_courant;
      affiche_rc();
      if (E_pos_y != E_maxligne - 1)
        E_pos_y++;
      zero();
      E_aff_curseur();
    } else {
      for (i = 0; i <= taille; i++)
        E_ligne_courant->prem_mot[i] = E_ligne_inter->prem_mot[i];
      E_ligne_courant->prem_mot[taille] = '\0';
      E_ligne_courant->pt_suiv = alloc_ligne();
      E_ligne_courant->pt_suiv->prem_mot = (char *)mbkalloc(MAXCAR + 1);
      E_ligne_courant->pt_suiv->pt_prec = E_ligne_courant;
      E_ligne_courant->pt_suiv->pt_suiv = NULL;
      E_ligne_courant->pt_suiv->attribut = E_ligne_courant->attribut & 0x01;
      E_ligne_courant = E_ligne_courant->pt_suiv;
      init_ligne(E_ligne_courant, MAXCAR + 1);
      E_ligne_inter = E_ligne_courant;
      affiche_rc();
      if (E_pos_y != E_maxligne - 1)
        E_pos_y++;
      zero();
      E_aff_curseur();
    }
  }
}

void
  E_traitement(carac)
int  carac;
{
int pos;

  E_del_curseur();
  pos = E_pos_x;
  if (E_mode == INDENT) {
    if (E_ligne_inter->prem_mot[E_pos_x] == '\0')
      add_space(pos, E_ligne_inter);
    indentation(carac, pos, E_ligne_inter);
    aff_indent(pos, E_ligne_inter);
    if (E_pos_x == MAXCAR - 1) {
      E_Printatlastline("Line too long");
      E_aff_curseur();
      return;
    } else {
      droite();
      E_aff_curseur();
    }
  } else {
    E_ligne_courant = E_ligne_inter;
    if (E_ligne_courant->prem_mot[E_pos_x] == '\0')
      add_space(pos, E_ligne_courant);
    if (strlen(E_ligne_courant->prem_mot) >= MAXCAR) {
      E_Printatlastline("Line too long");
      E_aff_curseur();
      return;
    } else {
      insertion(carac, pos, E_ligne_inter);
      aff_insert(pos, E_ligne_inter);
      droite();
      E_aff_curseur();
    }
  }
}

void
  init_ligne(line, nbre)
list_ligne *line;
int nbre;
{
int i;

  line->attribut = NORMAL;
  for (i = nbre - 1; i >= 0; i--)
    line->prem_mot[i] = '\0';
}

list_ligne *
  E_AllocBuf()
{
list_ligne *aloue;

  aloue = (list_ligne *)mbkalloc(sizeof(list_ligne));
  aloue->pt_suiv = aloue->pt_prec = NULL;  
  aloue->prem_mot = NULL;
  return aloue;
}

list_ligne *
  alloc_ligne()
{
list_ligne *aloue;

  E_nbre_ligne++;
  aloue = (list_ligne *)mbkalloc(sizeof(list_ligne)) ;
  aloue->pt_suiv = aloue->pt_prec = NULL;
  aloue->prem_mot = NULL;
  return aloue;
}

void
  clear_carac(position_x, position_y)
int position_x;
int position_y;
{
  XClearArea(display, E_window,
          (position_x + ORIGINEX + E_offnum + E_strtxt) * E_font_width,
          (((position_y + ORIGINEY - 1) * E_interligne) 
          + 2 * E_font_descent), E_font_width, E_font_height, FALSE);
}

void
  E_cleoln(position_x, position_y)
int position_x;
int position_y;
{
  XClearArea(display, E_window,
          (position_x + ORIGINEX + E_offnum + E_strtxt) * E_font_width,
          (((position_y + ORIGINEY - 1) * E_interligne) 
          + 2 * E_font_descent), 0, E_font_height, FALSE);
}

void
  E_affiche_page(initline, inity)
list_ligne *initline;
int inity;
{
list_ligne *param = initline;

  while ((inity + ORIGINEY + 1) * E_interligne <= E_dy_win) {
    XClearArea(display, E_window, 0,
        (((inity + ORIGINEY - 1) * E_interligne) + 2 * E_font_descent),
        0, E_font_height, FALSE);
    if (param != NULL) {
      if ((param->attribut & 0x01) == NORMAL)
        XCopyGC(display, C_gcText, (GCForeground | GCBackground), E_gc);
      else
        XCopyGC(display, C_gcIconTextInv, (GCForeground | GCBackground),
              E_gc);
      XDrawImageString(display, E_window, E_gc,
                  (ORIGINEX + E_offnum + E_strtxt) * E_font_width,
                  (inity + ORIGINEY) * E_interligne,
                  param->prem_mot, strlen(param->prem_mot));
      if (((param->attribut) >> 4) == BREAKPOINT)
        cercle(inity);
      else
        clear_break(inity);
      if (E_offnum > 0 && E_strtxt == 0)
        E_Printatlinenumber(inity, E_NumeroLigne(param));
      param = param->pt_suiv;
    }
    inity++;
  }
}

void
  affiche_rc()
{
  if (E_pos_y == E_maxligne - 1 || E_scroll_down == TRUE) {
    E_page_courant = E_page_courant->pt_suiv;  /*scrolling haut*/
    E_affiche_page(E_page_courant, 0);  /*a partie de la ligne 0*/
  } else {
    if (E_scroll_up == TRUE) {
      E_page_courant = E_page_courant->pt_prec;
      E_affiche_page(E_page_courant, 0);
    } else {
      E_affiche_page(E_ligne_courant->pt_prec, E_pos_y);
    }
  }
}

void
  aff_indent(pos, line)
int pos;
list_ligne *line;
{
char lettre[2];

  lettre[0] = line->prem_mot[pos];
  lettre[1] = '\0';
  clear_carac(pos, E_pos_y);
  if ((line->attribut & 0x01) == NORMAL)
    XCopyGC(display, C_gcText, (GCForeground | GCBackground), E_gc);
  else
    XCopyGC(display, C_gcIconTextInv, (GCForeground | GCBackground), E_gc);
  XDrawImageString(display, E_window, E_gc,
              (ORIGINEX + E_offnum + E_strtxt + pos) * E_font_width,
              (E_pos_y + ORIGINEY) * E_interligne, lettre, 1);
  if (E_offnum > 0 && E_strtxt == 0)
    E_Printatlinenumber(E_pos_y, E_NumeroLigne(line));
}

void
  aff_insert(pos, line)
int pos;
list_ligne *line;
{
char  finligne[MAXCAR + 1];

  E_cleoln(pos, E_pos_y);
  strcpy(finligne, &line->prem_mot[pos]);
  if ((line->attribut & 0x01) == NORMAL)
    XCopyGC(display, C_gcText, (GCForeground | GCBackground), E_gc);
  else
    XCopyGC(display, C_gcIconTextInv, (GCForeground | GCBackground), E_gc);
  XDrawImageString(display, E_window, E_gc,
              ((ORIGINEX + pos + E_offnum + E_strtxt) * E_font_width),
              (E_pos_y + ORIGINEY) * E_interligne, 
              finligne, strlen(finligne));
  if (((line->attribut) >> 4) == BREAKPOINT)
    cercle(E_pos_y);
  else
    clear_break(E_pos_y);
  if (E_offnum > 0 && E_strtxt == 0)
    E_Printatlinenumber(E_pos_y, E_NumeroLigne(line));
}

void
  E_aff_curseur()
{
  if (commande == TRUE)
    XDrawLine(display, E_window, E_gcc,
            (E_pos_x + ORIGINEX + E_offnum + E_strtxt) * E_font_width,
            (E_pos_y + ORIGINEY) * E_interligne + E_font_descent + 1,
            (E_pos_x + ORIGINEX + E_offnum + E_strtxt + 1) * E_font_width,
            (E_pos_y + ORIGINEY) * E_interligne + E_font_descent + 1);
  else 
    XDrawLine(display, E_window, E_gci,
            (E_pos_x + ORIGINEX + E_offnum + E_strtxt) * E_font_width,
            (E_pos_y + ORIGINEY) * E_interligne + E_font_descent + 1,
            (E_pos_x + ORIGINEX + E_offnum + E_strtxt + 1) * E_font_width,
            (E_pos_y + ORIGINEY) * E_interligne + E_font_descent + 1);
}

void
  E_del_curseur()
{
  XClearArea(display, E_window,
          (E_pos_x + ORIGINEX + E_offnum + E_strtxt) * E_font_width - 1,
          (E_pos_y + ORIGINEY) * E_interligne + E_font_descent - 1,
          0, 4, FALSE);
}

void
  E_init_var()
{
  E_nbre_ligne = 0;
  E_ligne_inter = alloc_ligne();
  E_ligne_inter->prem_mot = (char *)mbkalloc(MAXCAR + 1);
  init_ligne(E_ligne_inter, MAXCAR + 1);
  E_page_courant = E_ligne_inter;
  E_debut = E_ligne_inter;
  E_ligne_courant = E_ligne_inter;
  E_ligne_inter->pt_suiv = NULL;
  E_ligne_inter->pt_prec = NULL;
  zero();
  E_pos_y = 0;
}

void
  insertion(c, position, line)
char c;
int position;
list_ligne *line;
{
int i;

  for (i = MAXCAR - 1; i > position; i--)
    line->prem_mot[i] = line->prem_mot[i - 1];
  line->prem_mot[position] = c;
}

void
  indentation(carac, position, line)
char carac;
int position;
list_ligne *line;
{
  line->prem_mot[position] = carac;
}


void
  suppression(position, line)
int position;
list_ligne *line;
{
int i;

  for (i = position; i < MAXCAR; i++)
    line->prem_mot[i] = line->prem_mot[i + 1];
}

void
  add_space(pos, line)
int pos;
list_ligne *line;
{
int i;

  for (i = pos - 1; line->prem_mot[i] == '\0' && pos >= 0; i--)
    line->prem_mot[i] = SPACE;

  if ((line->attribut & 0x01) == NORMAL)
    XCopyGC(display, C_gcText, (GCForeground | GCBackground), E_gc);
  else
    XCopyGC(display, C_gcIconTextInv, (GCForeground | GCBackground), E_gc);
  XDrawImageString(display, E_window, E_gc,
              (ORIGINEX + E_offnum + E_strtxt) * E_font_width,
              (E_pos_y + ORIGINEY) * E_interligne,
              line->prem_mot, strlen(line->prem_mot));
  if (((line->attribut) >> 4) == BREAKPOINT)
    cercle(E_pos_y);
  else
    clear_break(E_pos_y);
}

void
  zero()
{
  E_pos_x = 0;
  if (E_strtxt != 0) {
    E_strtxt = 0;
    E_affiche_page(E_page_courant, 0);
  }
}

void
  droite()
{
  if (E_pos_x >= MAXCAR - 1) {
    E_Printatlastline("Line too long");
    return;
  }
  E_pos_x++;
  if ((E_pos_x - E_strtxt)%(((90 * E_dx_win)/(100 * E_font_width)) + 1) == 0) {
    E_strtxt -= (50 * E_dx_win)/(100 * E_font_width);
    E_affiche_page(E_page_courant, 0);
  }
}

void
  gauche()
{
  if (E_pos_x > 0) {
    E_pos_x--;
    if ((E_pos_x + E_strtxt)%((90 * E_dx_win)/(100 * E_font_width)) == 0) {
      E_strtxt += (50 * E_dx_win)/(100 * E_font_width);
      if (E_strtxt > 0) 
        E_strtxt = 0;
      E_affiche_page(E_page_courant, 0);
    }
  }
}

void
  haut()
{
  if (E_pos_y > 0)
    E_pos_y--;
}

void
  bas()
{
  if (E_pos_y < E_maxligne - 1)
    E_pos_y++;
}

void
  E_insert_ligne()
{
  E_ligne_inter = alloc_ligne();
  E_ligne_inter->prem_mot = (char *)mbkalloc(MAXCAR + 1);
  init_ligne(E_ligne_inter, MAXCAR + 1);
  E_ligne_inter->pt_prec = E_ligne_courant->pt_prec;
  E_ligne_inter->pt_suiv = E_ligne_courant;
  E_ligne_courant->pt_prec->pt_suiv = E_ligne_inter;
  E_ligne_courant->pt_prec = E_ligne_inter;
  E_ligne_courant = E_ligne_inter;
  E_affiche_page(E_ligne_courant, E_pos_y);
  zero();
  E_aff_curseur();
}

list_ligne *
  E_LectFic(nomf)
char *nomf;
{
FILE *fichier;
int taille;
int FinLect = FALSE;
char buffer[BUFSIZ];
int point_buffer = 0;
int point_tab = 0;
int i;

  if (!(fichier = fopen(nomf, "r")))
    return NULL;
  E_ligne_inter = E_ligne_courant;
  while (FinLect != TRUE) {
    if ((taille = fread(buffer, 1, BUFSIZ, fichier)) < BUFSIZ)
      FinLect = TRUE;
    point_buffer = 0;
    while (point_buffer < taille) {
      if (point_tab == MAXCAR || buffer[point_buffer] == '\n' || 
          buffer[point_buffer] == '\r') {
        E_ligne_inter->pt_suiv = alloc_ligne();
        E_ligne_inter->pt_suiv->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(E_ligne_inter->pt_suiv, MAXCAR + 1);
        E_ligne_inter->pt_suiv->pt_prec = E_ligne_inter;
        E_ligne_inter->pt_suiv->pt_suiv = NULL;
        E_ligne_inter = E_ligne_inter->pt_suiv;
        if (buffer[point_buffer] == '\n' || buffer[point_buffer] == '\r')
          point_buffer++;
        point_tab = 0;
      } else if (buffer[point_buffer] == '\t') {
      int j = point_tab % Larg_Tab;
        for (i = 0; i < Larg_Tab - j; i++)
          E_ligne_inter->prem_mot[point_tab++] = ' ';
        point_buffer++;
      } else {
        E_ligne_inter->prem_mot[point_tab++] = buffer[point_buffer++];
      }
    }
  }
  E_ligne_inter = E_ligne_courant;
  fclose(fichier);
  E_ClearBuffer();
  E_affiche_page(E_page_courant, 0);
  return E_debut;
}

list_ligne *
  OpenFic(nom)
char *nom;
{
  E_Effacer();
  return E_LectFic(nom);
}

int
  E_SauveFic(nmf)
char *nmf;
{
FILE *fichier;
list_ligne *deplace = E_debut;
char buffer[BUFSIZ];
int point_buffer = 0, point_tab = 0, size = 0;

  if (!(fichier = fopen(nmf, "w+"))) {
    E_Printatlastline("Cannot open the given file for writting");
    return -1;
  }
  while (deplace != NULL) {
    for (point_tab = 0; deplace->prem_mot[point_tab] != '\0';) {
      if (point_buffer != BUFSIZ)
        buffer[point_buffer++] = deplace->prem_mot[point_tab++];
      else {
        size += fwrite(buffer, sizeof(char), BUFSIZ, fichier);
        point_buffer = 0;
      }
    }
    if (point_buffer != BUFSIZ)
      buffer[point_buffer++] = '\n';
    else {
      size += fwrite(buffer, sizeof(char), BUFSIZ, fichier);
      buffer[0] = '\n';
      point_buffer = 1;
    }
    deplace = deplace->pt_suiv;  /*on passe a la ligne suivante*/
  }
  size += fwrite(buffer, sizeof(char), point_buffer, fichier);
  fclose(fichier);
  return 0;
}

void
  desalouer(line)
list_ligne *line;
{
  --E_nbre_ligne;
  mbkfree((void *)line->prem_mot);
  mbkfree((void *)line);
}

void
  E_delinekey()
{
list_ligne *lignedel = E_ligne_courant;

  E_del_curseur();
  /* Si 1 line */
  if (lignedel->pt_prec == NULL && lignedel->pt_suiv == NULL) {
    desalouer(lignedel);
    E_init_var();  /*creation de la premiere ligne*/
    E_cleoln(E_pos_x, E_pos_y);  /*efface la premiere ligne a l'ecran*/
    E_aff_curseur();
  } else {
    if (lignedel == E_debut)  /*premiere ligne du texte*/ {
      E_ligne_courant = E_ligne_courant->pt_suiv;
      E_debut = E_ligne_courant;
      E_ligne_courant->pt_prec = NULL;
      E_page_courant = E_debut;
      desalouer(lignedel);
      E_affiche_page(E_ligne_courant, 0);
      E_aff_curseur();
    } else if (lignedel->pt_suiv == NULL)  /*derniere ligne du texte*/ {
      E_ligne_courant = E_ligne_courant->pt_prec;
      E_ligne_courant->pt_suiv = NULL;
      desalouer(lignedel);
      haut();
      E_affiche_page(E_ligne_courant, E_pos_y);
      E_aff_curseur();
    } else {  /* ligne dans le texte */
      E_ligne_courant = E_ligne_courant->pt_suiv;
      E_ligne_courant->pt_prec = lignedel->pt_prec;
      lignedel->pt_prec->pt_suiv = E_ligne_courant;
      desalouer(lignedel);
      E_affiche_page(E_ligne_courant, E_pos_y);
      E_aff_curseur();
    }
  }
}

list_ligne *
  deline(line)
list_ligne *line;
{
list_ligne *lignedel = line;
int modif = FALSE;

  E_del_curseur();
  if (lignedel->pt_prec == NULL && lignedel->pt_suiv == NULL) {/*1 line*/
    desalouer(lignedel);
    E_init_var();  /*creation de la premiere ligne*/
    return E_ligne_courant;
  } else {
    if (lignedel == E_debut)  /*premiere ligne du texte*/ {
      if (E_page_courant == E_debut)
        modif = TRUE;
      line = line->pt_suiv;
      E_debut = line;
      line->pt_prec = NULL;
      if (modif == TRUE)
        E_page_courant = E_debut;
      desalouer(lignedel);
      return line;
    } else if (lignedel->pt_suiv == NULL)  /*derniere ligne du texte*/ {
      if (lignedel == E_page_courant)
        E_page_courant = E_page_courant->pt_prec;
      line = line->pt_prec;
      line->pt_suiv = NULL;
      desalouer(lignedel);
      /* cas tres specifique du au test qui sera fait dans E_delbloc*/
      return NULL;
      /* permet de donner la bonne position du curseur a la derniere ligne*/
    } else {  /* ligne dans le texte */
      if (lignedel == E_page_courant)
        E_page_courant = E_page_courant->pt_prec;
      line = line->pt_suiv;
      line->pt_prec = lignedel->pt_prec;
      lignedel->pt_prec->pt_suiv = line;
      desalouer(lignedel);
      return line;
    }
  }
}

int
  E_NumeroLigne(pointeur)
list_ligne *pointeur;
{
list_ligne *deplace = E_debut;
int param = 1;

  while (deplace != pointeur) {
    deplace = deplace->pt_suiv;
    param++;
  }
  return param;
}

void
  clear_attribVideo()
{
list_ligne *chemin = E_debut;

  while (chemin != NULL) {
    chemin->attribut &= 0xf0;
    chemin = chemin->pt_suiv;
  }
}

void
  E_ClearBuffer()
{
  if (E_BufferSauve != NULL) {
    while (E_BufferSauve->pt_suiv != NULL) {
      E_BufferSauve = E_BufferSauve->pt_suiv;
      mbkfree((void *)E_BufferSauve->pt_prec->prem_mot);
      mbkfree((void *)E_BufferSauve->pt_prec);
    }
    mbkfree((void *)E_BufferSauve->prem_mot);
    mbkfree((void *)E_BufferSauve);
  }
  E_BufferSauve = NULL;
}

void
  E_delbloc()
{
list_ligne *point_ligne;
list_ligne *point_buffer;
list_ligne *sauvegarde;
int i, j;
int E_numlignepage;
int code_gestion = 0;
int offset;

  E_numligne = E_NumeroLigne(E_ligne_courant);
  if (E_numligne_debbloc != 0 && E_numligne_finbloc != 0) {
    point_ligne = E_debut;
    for (i = 1; i < E_numligne_debbloc; i++)
      point_ligne = point_ligne->pt_suiv;
    E_numlignepage = E_NumeroLigne(E_page_courant);
    /* test si le bloc est dans la fenetre */
    if (E_numligne_finbloc < E_numlignepage && 
        E_numligne_debbloc > E_numlignepage + E_maxligne - 1)
      code_gestion = 1;
    else {
      /* le bloc est contenu entierement dans la fenetre graphique */
      if (E_numligne_debbloc >= E_numlignepage &&
          E_numligne_finbloc <= E_numlignepage + E_maxligne - 1)
        code_gestion = 2;
      else
        code_gestion = 3;  /* le bloc chevauche notre fenetre */
    }
    E_ClearBuffer();
    for (i = E_numligne_debbloc; i <= E_numligne_finbloc; i++) {
      if (E_BufferSauve == NULL) {
        point_buffer = E_AllocBuf();
        point_buffer->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(point_buffer, MAXCAR + 1);
        E_BufferSauve = point_buffer;
      } else {
        point_buffer->pt_suiv = E_AllocBuf();
        point_buffer->pt_suiv->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(point_buffer->pt_suiv, MAXCAR + 1);
        point_buffer->pt_suiv->pt_prec = point_buffer;
        point_buffer = point_buffer->pt_suiv;
        point_buffer->pt_suiv = NULL;
      }
      for (j = 0; point_ligne->prem_mot[j] != '\0'; j++)
        point_buffer->prem_mot[j] = point_ligne->prem_mot[j];
      sauvegarde = point_ligne->pt_prec;
      /*l'affectation de la E_page_courante est faite ds deline*/
      point_ligne = deline(point_ligne);
    }
    switch (code_gestion) {
      case 1:
        break;
      case 2:
        if (E_numligne < E_numligne_debbloc) {
          /* le curseur est au dessus du bloc marque */
        } else if (E_numligne > E_numligne_finbloc) { 
          /* curseur sous le bloc */
          E_numligne = E_NumeroLigne(E_ligne_courant);
          E_numlignepage = E_NumeroLigne(E_page_courant);
          E_pos_y = E_numligne - E_numlignepage;
        } else {
        /* le curseur est dans le bloc marque */
        /* il reste des lignes non marquees en dessous du bloc */
          offset = 0;
          if (point_ligne == NULL)
            offset = 1;
        /* on teste si on est arrive sur la derniere ligne*/
          E_pos_y = E_numligne_debbloc - E_numlignepage - offset;
          if (offset == 0) {
            E_ligne_courant = point_ligne;
            E_ligne_inter = E_ligne_courant;
          } else {
            E_ligne_courant = sauvegarde;
            E_ligne_inter = E_ligne_courant;
          }
        }
        break;
      case 3:
        if ((E_numligne_finbloc >= E_numlignepage) &&
            (E_numligne_finbloc < E_numlignepage + E_maxligne - 1)) {
        /* on regarde si la fin du bloc est dans notre fenetre */
          if (E_numligne <= E_numligne_finbloc) {
          /* le curseur est dans le bloc */
            E_pos_y = 0;
            E_ligne_courant = point_ligne;
            E_ligne_inter = point_ligne;
          } else {
            E_numlignepage = E_NumeroLigne(E_page_courant);
            E_numligne = E_NumeroLigne(E_ligne_courant);
            E_pos_y = E_numligne - E_numlignepage;
          }
        } else if ((E_numligne_debbloc > E_numlignepage) &&
              (E_numligne_debbloc <= E_numlignepage + E_maxligne - 1)) {
        /* on regarde que le de but du bloc est dans notre fenetre */
          if (E_numligne >= E_numligne_debbloc) {
          /* le curseur est dans le bloc */
            E_ligne_courant = point_ligne;
            E_ligne_inter = point_ligne;
            E_numligne = E_NumeroLigne(E_ligne_courant);
            E_numlignepage = E_NumeroLigne(E_page_courant);
            E_pos_y = E_numligne - E_numlignepage;
          } else
            E_pos_y = E_numligne - E_numlignepage;
        } else {
          E_numlignepage = E_NumeroLigne(E_page_courant);
          E_ligne_courant = point_ligne;
          E_ligne_inter = E_ligne_courant;
          E_numligne = E_NumeroLigne(E_ligne_courant);
          E_pos_y = E_numligne - E_numlignepage;
        }
        break;
      default:
        break;
    }
    E_numligne_debbloc = 0;
    E_numligne_finbloc = 0;
    E_del_curseur();
    E_affiche_page(E_page_courant, 0);
    E_aff_curseur();
  }
}


void
  E_CopyBlocToLine()
{
int i;
int po = 0;
int val = 0;
char data[80];
int j;
list_ligne *point_buffer = E_BufferSauve;
list_ligne *point_bloc = NULL;
list_ligne *tete;

  if (E_BufferSauve != NULL)  /* on teste si la copie est possible */ {
    while (point_buffer != NULL) {
      if (point_bloc == NULL)  /* on cree un bloc intermediaire */ {
        point_bloc = alloc_ligne();
        point_bloc->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(point_bloc, MAXCAR + 1);
        point_bloc->pt_prec = NULL;
        tete = point_bloc;
      } else {
        point_bloc->pt_suiv = alloc_ligne();
        point_bloc->pt_suiv->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(point_bloc->pt_suiv, MAXCAR + 1);
        point_bloc->pt_suiv->pt_prec = point_bloc;
        point_bloc = point_bloc->pt_suiv;
        point_bloc->pt_suiv = NULL;
      }
      /*remplissage de notre bloc*/
      for (j = 0; point_buffer->prem_mot[j] != '\0'; j++)
        point_bloc->prem_mot[j] = point_buffer->prem_mot[j];
      point_buffer = point_buffer->pt_suiv;
      val++;
    }
    point_bloc->pt_suiv = E_ligne_courant->pt_suiv;
    if (E_ligne_courant->pt_suiv != NULL)
      point_bloc->pt_suiv->pt_prec = point_bloc;
    E_ligne_courant->pt_suiv = tete;
    tete->pt_prec = E_ligne_inter = E_ligne_courant;
    E_del_curseur();
    E_affiche_page(E_page_courant, 0);
    E_downkey();
    E_aff_curseur();
    sprintf(data, "%d more %s", val, val <= 1 ? "line" : "lines");
    E_Printatlastline(data);
  }
}

int
  E_Effacer()
{
  E_numligne_debbloc = 1;
  E_numligne_finbloc = E_nbre_ligne;
  E_delbloc();
  return 0;
}

void
  cercle(pos)
int pos;
{
  XFillRectangle(display, E_window, E_gcc, 
            (ORIGINEX - 3) + 4,
            (pos + ORIGINEY - 1) * E_interligne 
            + 2 * (E_font_descent - 1) + 4,
            (int)(1.5 * E_font_width) - 4, E_font_height - 4);
}

void
  clear_break(pos)
int pos;
{
  XClearArea(display, E_window, (ORIGINEX - 3) + 4,
          (pos + ORIGINEY - 1) * E_interligne 
          + 2 * (E_font_descent - 1) + 4,
          (int)(1.5 * E_font_width) - 4, E_font_height - 4, FALSE);
}

int
  E_BreakPoint()
{
  E_ligne_courant->attribut ^= 0x10;
  if (((E_ligne_courant->attribut) >> 4) == BREAKPOINT) {
    cercle(E_pos_y);
    return E_NumeroLigne(E_ligne_courant);
  }
  clear_break(E_pos_y);
  return 0;
}

void
  clear_attribBreak()
{
list_ligne *chemin = E_debut;

  while (chemin != NULL) {
    chemin->attribut &= 0x0f;
    chemin = chemin->pt_suiv;
  }
  E_del_curseur();
  E_affiche_page(E_page_courant, 0);
  E_aff_curseur();
}

void
  E_EndText()
{
list_ligne *deplace = E_debut;
int ligne_ecran;

  while (deplace->pt_suiv != NULL)
    deplace = deplace->pt_suiv;
  E_ligne_courant = deplace;
  E_ligne_inter = deplace;
  for (ligne_ecran = 0; 
      ligne_ecran < E_maxligne - 1 && deplace->pt_prec != NULL;
      ligne_ecran++)
    deplace = deplace->pt_prec;
  E_page_courant = deplace;
  E_del_curseur();
  zero();
  E_pos_y = ligne_ecran;
  E_affiche_page(E_page_courant, 0);
  E_aff_curseur();
}


int
  gotoline(num)
int num;
{
list_ligne *deplace = E_debut;
int i;

  if (num > E_nbre_ligne)
    return ;
  for (i = 0; i < num - 1; i++)
    deplace = deplace->pt_suiv;
  E_ligne_courant = E_ligne_inter = deplace;
  for (i = 0; i < (E_maxligne - 1) / 2 && deplace != E_debut; i++)
    deplace = deplace->pt_prec;
  E_page_courant = deplace;
  E_del_curseur();
  zero();
  E_pos_y = i;
  E_affiche_page(E_page_courant, 0);
  E_aff_curseur();
}

int
  E_Ligne(num)
int num;
{
list_ligne *deplace = E_debut;
static list_ligne *memorise;
extern BOOL D_FinInterp;
int i;

  if (num > E_nbre_ligne)
    return;
  if (memorise != NULL)
    memorise->attribut &= 0xF0; /* NORMAL */
  if (D_FinInterp == TRUE) {
    memorise = NULL;
    gotoline(num);
    return;
  }
  for (i = 0; i < num - 1; i++)
    deplace = deplace->pt_suiv;
  deplace->attribut |= 0x01; /* INVERSE */
  memorise = deplace;
  E_ligne_courant = E_ligne_inter = deplace;
  for (i = 0; i < (E_maxligne - 1) / 2 && deplace != E_debut; i++)
    deplace = deplace->pt_prec;
  E_page_courant = deplace;
  E_del_curseur();
  zero();
  E_pos_y = i;
  E_affiche_page(E_page_courant, 0);
  E_aff_curseur();
}

void
  E_BlocToBuffer()
{
int i;
int j;
list_ligne *point_ligne;
list_ligne *point_buffer;

  if (E_numligne_debbloc != 0 && E_numligne_finbloc != 0) {
    point_ligne = E_debut;
    for (i = 1; i < E_numligne_debbloc; i++)
      point_ligne = point_ligne->pt_suiv;
    E_ClearBuffer();
    for (i = E_numligne_debbloc; i <= E_numligne_finbloc; i++) {
      if (E_BufferSauve == NULL) {
        point_buffer = E_AllocBuf();
        point_buffer->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(point_buffer, MAXCAR + 1);
        E_BufferSauve = point_buffer;
      } else {
        point_buffer->pt_suiv = E_AllocBuf();
        point_buffer->pt_suiv->prem_mot = (char *)mbkalloc(MAXCAR + 1);
        init_ligne(point_buffer->pt_suiv, MAXCAR + 1);
        point_buffer->pt_suiv->pt_prec = point_buffer;
        point_buffer = point_buffer->pt_suiv;
        point_buffer->pt_suiv = NULL;
      }
      for (j = 0; point_ligne->prem_mot[j] != '\0'; j++)
        point_buffer->prem_mot[j] = point_ligne->prem_mot[j];
      point_ligne = point_ligne->pt_suiv;
    }
  }
  clear_attribVideo();
  E_numligne_debbloc = 0;
  E_numligne_finbloc = 0;
  E_affiche_page(E_page_courant, 0);
  E_aff_curseur();
}


void
  E_Printatlinenumber(data, nbre)
int data;
int nbre;
{
char message[6];
int i;

  sprintf(message, "%-.4d", nbre);
  for (i = 0; message[i] == '0'; i++)
    message[i] = ' ';
  XDrawImageString(display, E_window, E_gc, ORIGINEX * E_font_width,
              (data + ORIGINEY) * E_interligne, 
              message, strlen(message));
}

void
  E_updateBp()
{
int i;
list_ligne *line;

  DelAllBp();
  for (line = E_debut, i = 1; line != NULL; line = line->pt_suiv, i++)
    if (((line->attribut) >> 4) == BREAKPOINT)
      AddBp(i);
}
