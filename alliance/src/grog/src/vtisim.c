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

#include <stdio.h>
#include <stdlib.h>
#include MUT_H
#include MPH_H
#include "grog.h"

/* Normally defined in values.h, but not available on all systems */
#ifndef BITS
#define  BITS(type)  (8 * (int)sizeof(type))
#endif

/*
* data generator for test patterns
*/
void randata(int pattern, int nb, int nw, biglong *data)
{
long i;
static char *values[8] = {"random", "address", "not(address)", "1010...",
                           "0101...", "full one", "full zero", "pwet"};
   
   fprintf(stderr, "data value is %s", values[pattern < 7 ? pattern : 0]);
   if (pattern >= 10 && pattern <= 90) 
      fprintf(stderr, " %d%% drains connected to bit lines\n", pattern);
   else
      fprintf(stderr, "\n");
   if (pattern == 1)
      for (i = 0; i < nw; i++)
         data[i].low = i, data[i].high = 0;
   else if (pattern == 2)
      for (i = 0; i < nw; i++)
         data[i].low = ~i, data[i].high = ~0;
   else if (pattern == 3)
      for (i = 0; i < nw; i++)
         data[i].low = data[i].high = 0xAAAAAAAA;
   else if (pattern == 4)
      for (i = 0; i < nw; i++)
         data[i].low = data[i].high = 0x55555555;
   else if (pattern == 5)
      for (i = 0; i < nw; i++)
         data[i].low = data[i].high = ~0;
   else if (pattern == 6)
      for (i = 0; i < nw; i++)
         data[i].low = data[i].high = 0;
   else if (pattern == 7)  {
      for (i = 0; i < nw; i++)
         if ((i & 1) && (i % 16) < 8)
            data[i].low = data[i].high = 0xAAAAAAAA;
         else if ((i & 1) && (i % 16) > 8)
            data[i].low = data[i].high = 0x55555555;
         else if (!(i & 1) && (i % 16) > 8)
            data[i].low = data[i].high = 0xAAAAAAAA;
         else if (!(i & 1) && (i % 16) < 8)
            data[i].low = data[i].high = 0x55555555;
   } else if (pattern < 10 || pattern > 90) {
      srandom(getpid());
      for (i = 0; i < nw; i++)
         data[i].low = random(), data[i].high = random();
   } else { /* random percentage per bit lines */
      int n;

      pattern /= 10; /* make it a simple percentage */
      srandom(getpid());
      for (i = 0; i < nw; i++) {
         data[i].low = data[i].high = 0;
         for (n = 0; n < BITS(long); n++) {
            (data[i].low) |= (((random() % 10) < pattern ? 0 : 1) << n);
            (data[i].high) |= (((random() % 10) < pattern ? 0 : 1) << n);
         }
      }
   }
}

/*
* vtisim driver
*/
void sim(char *s, int nb, int nw, int zh, int r)
{
int i, j;
char t[33];
FILE *f;

   sprintf(t, "%s.sim", s);
   if ((f = fopen(t, "w")) == NULL) {
      fprintf(stderr, "grog : cannot open simuation file\n");
      EXIT(1);
   }

   fprintf(f, "load (echo) [fne]%s\n", s);
   fprintf(f, "set output %s only\n", s);
   fputs("set power low vss* bulk*\n", f);
   fputs("set power high vdd*\n", f);

   fputs("set external output ", f);
   for (i = 0; i < nb; i ++)
      fprintf(f, "f[%d] ", i);
   fputs("\n", f);

   fputs("set radix 2\n", f);
   fputs("set mode logic\n", f);

   if (r) {
      fputs("vector ", f);
      fprintf(f, "f[%d:0] ", nb - 1);
      for (i = 0; i < nb; i ++)
         fprintf(f, "f[%d] ", nb - i - 1);
      fputs("\n", f);
   } else {
      fputs("vector ", f);
      fprintf(f, "f[0:%d] ", nb - 1);
      for (i = 0; i < nb; i ++)
         fprintf(f, "f[%d] ", i);
      fputs("\n", f);
   }

   fputs("vector ", f);
   fprintf(f, "adr[0:%d] ", (int)ln2p(nw) - 1);
   for (i = 0; i < ln2p(nw); i ++)
      fprintf(f, "adr[%d] ", i);
   fputs("\n", f);

   if (nw == 64)
      fputs("set clock ck 1(100) 0(100)\n", f);
   else if (nw == 128 || nw == 256)
      fputs("set clock ck[0] 1(100) 0(100)\nset clock ck[1] 1(100) 0(100)\n",f);
   else for (i = 0; i < nw / 512; i += 2)
      fprintf(f, "set clock ck[%d] 1(100) 0(100)\n", i / 2);
   fputs("set trace mode tabular\n", f);
   if (nw == 64)
      fprintf(f, "watch (always) ck %s adr f\n", zh ? "oe" : "vdd");
   else
      fprintf(f, "watch (always) ck[0] %s adr f\n", zh ? "oe" : "vdd");
   if (zh)
      fputs("set inputs high oe\n", f);

   j = ln2p(nw);
   for (i = 0; i < nw + 1; i++) {
      long_bin(t, i, j);
      fprintf(f, "set inputs vector adr 'B%s\n", t);
      fprintf(f, "phase\n");
      fprintf(f, "phase\n");
   }
   fclose(f);
}

/*
* first clock cycle needs this one
*/
char *ini(char *s, char i, int n)
{
int j;

   /* make a string :
      a string n characters long is build using the character i.
      this is needed for the first clock pulse at initialization. */
   for (j = 0; j < n; j++)
      *(s + j) = i;
   return s;
}

/*
* expected simulation results
*/
void mis(char *s, int nb, int nw, int zh, biglong *data)
{
int i, j;
char v[256], t[256], u[256];
FILE *f;

   sprintf(t, "%s.res", s);
   if ((f = fopen(t, "w")) == NULL) {
      fprintf(stderr, "grog : cannot open expexted results file\n");
      EXIT(1);
   }

   j = ln2p(nw);
   fprintf(f, "1 1 %s %s %s\n", long_bin(v, 0, j),
                                 ini(t, 'u', j), ini(u, 'x', nb));
   for (i = 0; i < nw; i++) {
      fprintf(f, "0 1 %s %s %s\n", long_bin(v, i, j),
                                   long_bin(t, ~i, j),
                                   biglong_bin(u, data[i], nb));
      fprintf(f, "1 1 %s %s %s\n", long_bin(v, i + 1, j), t, u);
   }
   fclose(f);
}

