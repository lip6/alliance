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

/*******************************************************************************
* Declarations                                                                 *
*******************************************************************************/
/* nice :
   we should be able to read up to 64 bits.
   Tough for a 32 bits machine without cheating. */
typedef struct blong {long high, low;} biglong;

extern char *grog(char *name,
                  int nb, int nw,
                  char *codefile,
                  int tr, int zh, int r, int msb, int nc, int co,
                  int layout, int netlist, int vhdl, int verilog,
                  int pat, int icon, int outline, int datasheet);

extern void grogdata(char *name, int nb, int nw, int tr, int zh, int msb, biglong *data);
extern void groglayout(char *name, int nb, int nw, int tr, int zh, int r, int msb, int nc, int co, biglong *data, int save);
extern void grognetlist(char *name, int nb, int nw, int zh, int msb);
extern void grogoutline(void);
extern void grogvhdl(char *s, int nb, int nw, int zh, int r, int msb, biglong *data);
extern void grogpat(char *s, int nb, int nw, int zh, int r, int msb, biglong *data);
extern void grogverilog(char *s, int nb, int nw, int zh, int r, int msb, biglong *data);
void grogcode(/* s, nw, nb, data */);
extern void grogicon(char *name, int nb, int nw, int zh, int r, int msb);
extern void randata(int pattern, int nb, int nw, biglong *data);
extern char *biglong_bin(char *s, biglong i, int n);
extern char *biglong_hex(char *s, biglong i, int n);
extern char *long_bin(char *s, long i, int n);
extern char *long_hex(char *s, long i, int n);
long ln2p(long n);

