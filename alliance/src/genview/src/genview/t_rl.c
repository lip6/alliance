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
# include <stdlib.h>
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

extern int V_colors[];
extern Display *display;
extern Window T_window;
extern float scale;

void T_clearScreen();

typedef struct text_line
{
  char *line;
  int size;
} lines;

static lines *text;      /* text head pointer              */
static int top_line;     /* line where to print from       */
static int last_line;    /* last counted line              */
static int lpp;          /* lines per page                 */
static int cpp;          /* colums per page                */
static int text_pos;     /* text pos in the trace window   */

static FILE *allStream;  /* stream associated all traces   */
static FILE *outStream;  /* stream associated to stdout    */
static FILE *errStream;  /* stream associated to stderr    */
static char allFile[40]; /* stream name for traces         */
static char outFile[40]; /* stream name for new stdout     */
static char errFile[40]; /* stream name for new stderr     */

T_std()
{
  /* catches the std streams in order to get them in tracefile */
  (void)sprintf(allFile, "/usr/tmp/all_%d",getpid());
  (void)sprintf(outFile, "/usr/tmp/out_%d",getpid());
  (void)sprintf(errFile, "/usr/tmp/err_%d",getpid());

  allStream = fopen(allFile, "w+");

# ifndef GENVIEW_DEBUG
  errStream = freopen(errFile, "w+", stderr);
  outStream = freopen(outFile, "w+", stdout);
# else    
  outStream = allStream;
  errStream = allStream;
# endif
  
  if (outStream == NULL || errStream == NULL) {
    (void)fprintf(stderr, "Cannot open trace window for standard output\n");
    EXIT(-8);
  }

  T_print("\n\n");
  unlink(allFile);

# ifndef GENVIEW_DEBUG
  unlink(errFile);
  unlink(outFile);
# endif
}

static merge_io()
{
  int c;

  (void)fseek(allStream, 0L, 2);

# ifndef GENVIEW_DEBUG

  (void)fseek(errStream, 0L, 0);
  while ((c = fgetc(errStream)) != EOF)
    (void)putc(c, allStream);

  (void)fseek(outStream, 0L, 0); 
  while ((c = fgetc(outStream)) != EOF)
    (void)putc(c, allStream);
  
  (void)fclose(errStream);
  (void)fclose(outStream); 

  outStream = freopen(outFile, "w+", stdout);
  errStream = freopen(errFile, "w+", stderr);

# endif
}

T_initText()
{
  T_free();
  T_std();
  (void)fseek(allStream, 0L, 0);
  top_line = last_line = text_pos = 0;
  T_clearScreen();
  T_print("\n");
}

T_print(str)
char *str;
{
  (void)fseek(allStream, 0L, 2);
  (void)fputs(str, allStream);
  (void)fflush(allStream);
  merge_io();
  rewind(allStream);
  last_line = T_readLine();
  T_clearScreen();
  top_line = ((last_line - lpp) < 0) ? top_line : last_line - lpp; 
  T_writeLine(top_line, top_line + lpp);
  XFlush(display);
}

T_readLine()
{
register int len, nl = 0;
int n = 1;
char *p, line[100];

  text = (lines *)mbkalloc(BUFSIZ * sizeof (lines));
  while (fgets(line, 100, allStream)) {
    len = strlen(line) + 1;
    if ((p = mbkalloc(len)) == NULL)
      exit(12);
    line[--len] = '\0';
    (void)strcpy(p, line);
    text[nl].size = --len;
    text[nl++].line = p;
    if (nl > (n * BUFSIZ))
      text = (lines *)mbkrealloc(text, ++n * BUFSIZ * sizeof (lines));
  }
  return nl - 1;
}

void T_writeLine(first, last)
int first, last;
{
register int i;

  if (last > last_line)
    last = last_line;
  if (first > last)
    return;
  if (last <= 0) 
   return;
  for (i = first; i <= last; i++) {
    XDrawImageString(display, T_window, C_gcText,
                2, text_pos, text[i].line, text[i].size);
    text_pos += 12;
  }
}

void T_refresh()
{
  T_clearScreen();
  T_writeLine(top_line, top_line + lpp);
}

void T_pageUp()
{
  T_clearScreen();
  if (top_line > lpp)
    top_line -= lpp;
  else
    top_line = 0;
  T_writeLine(top_line, top_line + lpp);
}

void T_pageDn()
{
  T_clearScreen();
  if (top_line + lpp < last_line)
    top_line += lpp;
  else
    top_line = last_line;
  if (top_line < 0)
    top_line = 0;
  T_writeLine(top_line, top_line + lpp);
}

void T_lineUp()
{
  T_clearScreen();
  if (top_line > 1)
    top_line --;
  else
    top_line = 0;
  T_writeLine(top_line, top_line + lpp);
}

void T_lineDn()
{
register int old;

  T_clearScreen();
  old = top_line;
  if (top_line + 1 < last_line)
    top_line ++;
  else
    top_line = last_line;
  if (top_line < 0)
    top_line = old;
  T_writeLine(top_line, top_line + lpp);
}

void T_clearScreen()
{
int dx, dy;
int forget_me;
Window w;

  text_pos = 16 + 16 + 12;
  XGetGeometry(display, T_window, &w, &forget_me, &forget_me,
      &dx, &dy, &forget_me, &forget_me);
  lpp = (long)((long)(dy - text_pos) / 12) - 1;
  cpp = (long)((long)(dx - 10) / 6) - 1;
  XFillRectangle(display, T_window, C_gcBgnd, 0, text_pos - 12, dx, dy);
}

void T_free()
{
register int i;

  if (last_line == 0)
    return;
  for (i = 0; i <= last_line; i++)
    mbkfree((void *)text[i].line);
  mbkfree((void *)text);
  (void)fclose(allStream);
}

T_execute(cmd)
char *cmd;
{
int s;

  s = system(cmd);
  merge_io();
  return s;
}

