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
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_global.h"

# include "v_view.h"
# include "i_lines.h"
# include "c_menu.h"
# include "v_extern.h"

FILE *flush1;
lines *i_text;

static int top_line;
static int last_line;
static int lpl;
static int text_pos;

void I_out(inorout)
int inorout;
{
static int first;

  if (inorout == 1) {
    first = 0;
    i_free();
    return;
  }
  if (first == 0) {
    if ((flush1 = fopen(".xinspect", "r")) == NULL) {
      fprintf(stderr, "cannot open file 'xinspect'.\nExiting in I_out.\n");
      EXIT(-7);
    }
    top_line = 0;
    last_line = readLine();
  }
  first = 1;
  unlink(".xinspect");
  clearscreen();
  writeLine(top_line, top_line + lpl);
}

int readLine()
{
register int len, nl = 0;
int n = 1;
char *p, line[100];

  i_text = (lines *)mbkalloc(BUFSIZ * sizeof (lines));
  while (fgets(line, 100, flush1) != 0) {
    len = strlen(line);
    if ((p = mbkalloc(len)) == NULL)
      exit(11);
    line[--len] = '\0';
    strcpy(p, line);
    i_text[nl].size = len;
    i_text[nl++].line = p;
    if (nl > (n * BUFSIZ))
      i_text = (lines *)mbkrealloc(i_text, ++n * BUFSIZ * sizeof (lines));
  }
  return nl - 1;
}

void writeLine(first, last)
int first, last;
{
register int i;

  if (last > last_line)
    last = last_line;
  if (first > last)
    return;
  for (i = first; i <= last; i++) {
    XDrawImageString(display, I_window, C_gcText, 20, text_pos,
                i_text[i].line, i_text[i].size);
    text_pos += 12;
  }
}

void i_pageUp()
{
  clearscreen();
  if (top_line > lpl)
    top_line -= lpl;
  else
    top_line = 0;
  writeLine(top_line, top_line + lpl);
}

void i_pageDn()
{
  clearscreen();
  if (top_line + lpl < last_line)
    top_line += lpl;
  else
    top_line = last_line - 1;
  if (top_line < 0)
    top_line = 0;
  writeLine(top_line, top_line + lpl);
}

void i_lineUp()
{
  clearscreen();
  if (top_line > 1)
    top_line -= 1;
  else
    top_line = 0;
  writeLine(top_line, top_line + lpl);
}

void i_lineDn()
{
int old;

  clearscreen();
  old = top_line;
  if (top_line + 1 < last_line)
    top_line += 1;
  else
    top_line = last_line - 1;
  if (top_line < 0)
    top_line = old;
  writeLine(top_line, top_line + lpl);
}

void clearscreen()
{
int dx, dy;
int forget_me;
Window w;

  text_pos = 32 + 16 + 12;
  XGetGeometry(display, I_window, &w, &forget_me, &forget_me,
            &dx, &dy, &forget_me, &forget_me);
  lpl = (long)((long)(dy - text_pos) / 12) - 1;
  XFillRectangle(display, I_window, C_gcBgnd, 0, text_pos - 12, dx, dy);
}

i_free()
{
register int i;

  for (i = 0; i <= last_line; i++)
    mbkfree(i_text[i].line);
  mbkfree(i_text);
  fclose(flush1);
}
