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

/*******************************************************************************
* Grog : generic rom generator                                                 *
*                                                                              *
* Architecture and leaf cells defined at Bull's research center at les Clayes  *
*                                                                              *
* Leaf cells modifications to meet idps design rules                           *
* version 0.0 on April/June 1992, Frederic Petrot                              *
*                                                                              *
* All programming in Genlib                                                    *
* version 0.0 on April/June 1992, Frederic Petrot                              *
*                                                                              *
* version 0.1, by Frederic Petrot                                              *
* version 1.0, by Frederic Petrot                                              *
* version 1.1, by Frederic Petrot                                              *
* version 1.2, by Frederic Petrot                                              *
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include MUT_H
#include "grog.h"

/*
* computes ln2(n) by excess : for example ln2(512) = 9, ln2(513) = 10
*/
long ln2p(long n)
{
long i = 0L, j = (n & 1) ? 1L : 0L;

   if (n)
      for (i = -1; n > 0L; n >>= 1) {
         if (n & 1L)
            j++;
         i++;
      }
   return i + (j > 1L ? 1L : 0L);
}

/*
* main routine that calls the adquate routines for needed views
* here begins the real stuff
*/
#define SAVE_LAYOUT 1
#define SKIP_LAYOUT 0
char *grog(char *name,
           int nb, int nw,
           char *codefile,
           int tr, int zh, int r, int msb, int nc, int co,
           int layout, int netlist, int vhdl, int verilog, int pat, int icon, int outline, int datasheet)
{
biglong data[8192]; /* to be filled by the .vbe parser */

/* verifications :
   since the generator can be called as a standalone library, it must
   check the parameters in this function again. */

   if (!(layout | netlist | vhdl | pat | icon | outline | datasheet)) {
      fprintf(stderr, "grog : at least one view is to be generated\n");
      EXIT(1);
   }

   if (nb < 1 || nb > 64) {
      fprintf(stderr, "The bits argument ranges from 1 to 64\n");
      EXIT(1);
   }
   
   if (!(nw == 64 || nw == 128 || nw == 256 || nw == 512 || nw == 1024 ||
         nw == 1536 || nw == 2048 || nw == 2560 || nw == 3072 ||
         nw == 3584 || nw == 4096)) {
      fprintf(stderr, "The words argument must range from 64 to 4096\n");
      EXIT(1);
   }

#ifdef VTICHECK
   randata(atoi(codefile), nb, nw, data);
#else
   if (layout || outline || netlist || vhdl || verilog) {
      if (isdigit((int)*codefile)) 
         randata(atoi(codefile), nb, nw, data);
      else
         grogcode(codefile, nb, nw, r, msb, data);
   }
#endif
   
   if (layout)
      groglayout(name, nb, nw, tr, zh, r, msb, nc, co, data, SAVE_LAYOUT);

#ifdef VTICHECK
   if (layout) {
      sim(name, nb, nw, zh, r);
      mis(name, nb, nw, zh, data);
   }
#endif
   if (netlist)
      grognetlist(name, nb, nw, zh, msb);

   if (vhdl)
      grogvhdl(name, nb, nw, zh, r, msb, data);

   if (verilog)
      grogverilog(name, nb, nw, zh, r, msb, data);

   if (pat)
      grogpat(name, nb, nw, zh, r, msb, data);

   if (icon)
      grogicon(name, nb, nw, zh, r, msb);

   if (datasheet) {
      if (!layout) {
         groglayout(name, nb, nw, tr, zh, r, msb, nc, co, data, SKIP_LAYOUT);
         layout = 1;
      }
      grogdata(name, nb, nw, tr, zh, msb, data);
   }

   if (outline) {
      if (!layout)
         groglayout(name, nb, nw, tr, zh, r, msb, nc, co, data, SKIP_LAYOUT);
      grogoutline();
   }

   return name;
}
