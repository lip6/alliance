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

/************************************************************************
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
*                                                                              *
*******************************************************************************/

/*
* actual layout generator call
*/
#include <stdio.h>
#include "genlib.h"
#define AL_AL 5        /* pitch rule for alu1 */

void groglayout();
void block();   /* for 512 to 4K words */
void poke();
void ublock();  /* for 256 words       */
void upoke();
void nblock();  /* for 128 words       */
void npoke();
void pblock();  /* for 64 words        */
void ppoke();

char reference[32]; /* fuse name in grbl4_c  */
char instance[32];  /* grbl4_c instance name */

main()
{
long *data;
int i;

   data = (long *)malloc(4096 * sizeof(long));
   for (i = 0; i < 4096; i++)
      data[i] = (i << 8) | i;
   data[12] = ~0;

   /* parameters :
      bloc name, number of bits, #words, feed through, zh, reverse. */
   groglayout("grog", 12, 128, 0, 0, 0, data, 1L);
}

void
   groglayout(name, nb, nw, tr, zh, r, data, save)
char *name;
long nb, nw, tr, zh, r;
long *data;
long save;
{
   DEF_PHFIG(name);
   switch (nw) {
      case 64 :
         pblock(nw, nb, tr, zh, r);
         if (save)
            ppoke(nw, nb, zh, data);
         break;
      case 128 :
         nblock(nw, nb, tr, zh, r);
         if (save)
            npoke(nw, nb, zh, data);
         break;
      case 256 :
         ublock(nw, nb, tr, zh, r);
         if (save)
            upoke(nw, nb, data);
         break;
      default :
         block(nw, nb, tr, zh, r);
         if (save)
            poke(nw, nb, data);
   }
   if (save)
      SAVE_PHFIG();
}

/*
* computes ln2(n) by excess : for example ln2(512) = 9, ln2(513) = 10
*/
long
   ln2p(n)
long n;
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
* rom floorplan generation
*/
void
   block(nw, nb, tr, zh, r)
long nw, nb, tr, zh, r;
{
long lnw; /* number of address lines */
long blk; /* number of 512 word blocks */
long lnblk; /* ln2 of block numbers +/- 1 */
long i, j, k, l;
char *refname; /* reference instance name */

   lnw = ln2p(nw);
   blk = nw / 512;
   lnblk = blk == 1L ? 0L : ln2p(blk);

   /* output line : body ties, output buffers, address line inverters */
   /* let's put the first stone */
   PLACE(blk == 1 ? "grmbob_c" : "grrbob_c", "rbol", NOSYM, 0L, 0L);
   for (i = 0L; i < nb; i++)
      if (zh) {
         PLACE_RIGHT(blk == 1 ? "grmobh_c" : "grrobh_c",
                     NAME("rob/%d", i), NOSYM);
         if (tr)
            PLACE_RIGHT(blk == 1 ? "grmoth_c" : "grroth_c",
                        NAME("rot/%d", i), NOSYM);
      }
      else {
         PLACE_RIGHT(blk == 1 ? "grmob_c" : "grrob_c", NAME("rob/%d", i), NOSYM);
         if (tr)
            PLACE_RIGHT(blk == 1 ? "grmbt_c" : "grrbt_c",
                        NAME("rot/%d", i), NOSYM);
      }
   if (zh)
      PLACE_RIGHT(blk == 1 ? "grmoebh_c" : "grroebh_c", NAME("rzhb", i), NOSYM);
   else
      PLACE_RIGHT(blk == 1 ? "grmfill_c" : "grrfill_c", NAME("rzhb", i), NOSYM);
   PLACE_RIGHT(blk == 1 ? "grmfeed_c" : "grrfeed_c", NAME("rept", i), NOSYM);
   for (i = lnw - 1; i >= 0L; i--)
      PLACE_RIGHT(blk == 1 ? "grmli_c" : "grrli_c", NAME("rli/%d", i), NOSYM);

   /* blocks generation : transistor matrix, word and bit decoders */
   for (k = 0L; k < blk; k++) { /* for each block */
      DEF_PHINS(k == 0L ? "rbol" : NAME("rboh/%d", k - 1));
      PLACE_TOP("grmrbom_c", NAME("rboh/%d", k), k & 1 ? SYM_Y : NOSYM);
      /* matrix building */
      if (!(k & 1)) { /* even blocks */
         for (j = 0L; j < nb; j ++) { /* for each bit */
            PLACE_RIGHT(blk == 1 ? "grmmx_c" : "grrmx_c",
                        NAME("rmx14/%d/%d", k, j), NOSYM);
            PLACE_TOP("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j), NOSYM);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j), NOSYM);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j), NOSYM);
            PLACE_TOP("grp4_c", NAME("rp4/%d/%d", k, 4 * j), NOSYM);

            DEF_PHINS(NAME("rmx04/%d/%d", k, 4 * j));
            PLACE_RIGHT("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j + 1), SYM_X);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j + 1), SYM_X);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j + 1), SYM_X);
            PLACE_TOP("grp4_c", NAME("rp4/%d/%d", k, 4 * j + 1), SYM_X);

            DEF_PHINS(NAME("rmx04/%d/%d", k, 4 * j + 1));
            PLACE_RIGHT("grmrst_c", NAME("rp1/%d/%d", k, j), NOSYM);
            PLACE_RIGHT("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j + 2), NOSYM);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j + 2), NOSYM);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j + 2), NOSYM);
            PLACE_TOP("grp4_c", NAME("rp4/%d/%d", k, 4 * j + 2), NOSYM);

            DEF_PHINS(NAME("rmx04/%d/%d", k, 4 * j + 2));
            PLACE_RIGHT("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j + 3), SYM_X);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j + 3), SYM_X);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j + 3), SYM_X);
            PLACE_TOP("grp4_c", NAME("rp4/%d/%d", k, 4 * j + 3), SYM_X);

            DEF_PHINS(NAME("rmx04/%d/%d", k, 4 * j + 3));
            PLACE_RIGHT(blk == 1 ? "grmmot_c" : "grrmo_c",
                        NAME("rout/%d/%d", k, j), NOSYM);
         
            DEF_PHINS(NAME("rmx14/%d/%d", k, j));
            if (tr)
               PLACE_RIGHT(blk == 1 ? "grmmt_c" : "grrmt_c",
                           NAME("rmt/%d/%d", k, j), NOSYM);
         }
         /* decoder building */
         PLACE_RIGHT("grmrx3_c", NAME("rx416/%d", k), NOSYM);
         PLACE_TOP("grmrx2_c", NAME("rx316/%d", k), NOSYM);
         PLACE_TOP("grmrx1_c", NAME("rx216/%d", k), NOSYM);
         PLACE_TOP("grmrx0_c", NAME("rx116/%d", k), NOSYM);
         DEF_PHINS(NAME("rx416/%d", k));
         PLACE_RIGHT("grmrick_c", NAME("rc116/%d", k), NOSYM);
         PLACE_RIGHT("grmrw0_c", NAME("rw0/%d", k), NOSYM);
         PLACE_RIGHT("grmrw1_c", NAME("rw1/%d", k), NOSYM);
         PLACE_RIGHT("grmrw2_c", NAME("rw2/%d", k), NOSYM);
         PLACE_RIGHT("grmrw3_c", NAME("rw3/%d", k), NOSYM);
         if (blk == 1)
            PLACE_RIGHT("grmbs_c", NAME("rbs/%d", k), NOSYM);
         else
            PLACE_RIGHT(NAME("rrbs%ld_f", lnblk), NAME("rbs/%d", k), NOSYM);
         for (i = 0; i < 8; i++) {
            if (!i)
               DEF_PHINS(NAME("rx116/%d", k));
            else 
               DEF_PHINS(NAME("rbu16/%d/%d", k, i - 1));
            PLACE_TOP("grmrwb_c", NAME("rbu16/%d/%d", k, i), NOSYM);
            PLACE_RIGHT("grmrl_c", NAME("rl/%d/%d", k, i), NOSYM);
            PLACE_RIGHT("grmrs_c", NAME("rs/%d/%d", k, i), NOSYM);
         }
         DEF_PHINS(NAME("rbu16/%d/%d", k, i - 1));
         PLACE_TOP("grmrck_c", NAME("rck/%d", k), NOSYM);
      } else { /* odd blocks */
         /* matrix */
         for (j = 0; j < nb; j ++) { /* for each bit */
            PLACE_RIGHT("grp4_c", NAME("rp4/%d/%d", k, 4 * j), SYM_Y);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j), SYM_Y);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j), SYM_Y);
            PLACE_TOP("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j), SYM_Y);
            PLACE_TOP("grrmx_c", NAME("rmx14/%d/%d", k, j), SYM_Y);
   
            DEF_PHINS(NAME("rp4/%d/%d", k, 4 * j));
            PLACE_RIGHT("grp4_c", NAME("rp4/%d/%d", k, 4 * j + 1), SYMXY);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j + 1), SYMXY);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j + 1), SYMXY);
            PLACE_TOP("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j + 1), SYMXY);
   
            DEF_PHINS(NAME("rp4/%d/%d", k, 4 * j + 1));
            PLACE_RIGHT("grmrst_c", NAME("rp1/%d/%d", k, j), SYM_Y);
            PLACE_RIGHT("grp4_c", NAME("rp4/%d/%d", k, 4 * j + 2), SYM_Y);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j + 2), SYM_Y);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j + 2), SYM_Y);
            PLACE_TOP("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j + 2), SYM_Y);
   
            DEF_PHINS(NAME("rp4/%d/%d", k, 4 * j + 2));
            PLACE_RIGHT("grp4_c", NAME("rp4/%d/%d", k, 4 * j + 3), SYMXY);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/1/%d", k, 4 * j + 3), SYMXY);
            PLACE_TOP("grbl4_c", NAME("rbl4/%d/0/%d", k, 4 * j + 3), SYMXY);
            PLACE_TOP("grmx4_c", NAME("rmx04/%d/%d", k, 4 * j + 3), SYMXY);

            DEF_PHINS(NAME("rp4/%d/%d", k, 4 * j + 3));
            PLACE_RIGHT("grrmo_c", NAME("rout/%d/%d", k, j), SYM_Y);
         
            if (tr)
               PLACE_RIGHT("grrmt_c", NAME("rmt/%d/%d", k, j), SYM_Y);
         }
         PLACE_RIGHT("grmrck_c", NAME("rck/%d", k), SYM_Y);
         for (i = 0; i < 8; i++) {
            if (i)
               DEF_PHINS(NAME("rbu16/%d/%d", k, 7 - i + 1));
            PLACE_TOP("grmrwb_c", NAME("rbu16/%d/%d", k, 7 - i), SYM_Y);
            PLACE_RIGHT("grmrl_c", NAME("rl/%d/%d", k, 7 - i), SYM_Y);
            PLACE_RIGHT("grmrs_c", NAME("rs/%d/%d", k, 7 - i), SYM_Y);
         }
         DEF_PHINS(NAME("rbu16/%d/%d", k, 7 - i + 1));
         PLACE_TOP("grmrx0_c", NAME("rx116/%d", k), SYM_Y);
         PLACE_TOP("grmrx1_c", NAME("rx216/%d", k), SYM_Y);
         PLACE_TOP("grmrx2_c", NAME("rx316/%d", k), SYM_Y);
         PLACE_TOP("grmrx3_c", NAME("rx416/%d", k), SYM_Y);
         DEF_PHINS(NAME("rx116/%d", k));
         PLACE_RIGHT("grmrick_c", NAME("rc116/%d", k), SYM_Y);
         PLACE_RIGHT("grmrw0_c", NAME("rw0/%d", k), SYM_Y);
         PLACE_RIGHT("grmrw1_c", NAME("rw1/%d", k), SYM_Y);
         PLACE_RIGHT("grmrw2_c", NAME("rw2/%d", k), SYM_Y);
         PLACE_RIGHT("grmrw3_c", NAME("rw3/%d", k), SYM_Y);
         PLACE_RIGHT(NAME("rrbs%ld_f", lnblk), NAME("rbs/%d", k), SYM_Y);
      }
   }
   DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      COPY_UP_CON(0L, "i", NAME("rli/%d", i), NAME("adr[%d]", i));
      COPY_UP_CON(0L, "f", NAME("rli/%d", i), NAME("adrb[%d]", i));
   }
   /* draw crossing lines and appropriate vias for address decod */
   j = blk == 1 ? 0 : ln2p(blk);
   for (k = 0; k < blk; k++) {
      for (i = 0; i < 8; i++) {
         for (l = 0; l < 3; l++) {
            refname = (i & (1 << l)) ? NAME("adr[%d]", j + 2 - l)
                                     : NAME("adrb[%d]", j + 2 - l);
            COPY_UP_CON(0L, NAME("e%d", 3 - l), NAME("rs/%d/%d", k, i), refname);
            PHVIA(CONT_VIA,
                  GET_CON_X(NAME("rli/%d", j + 2 - l),
                                (i & (1 << l)) ? "i" : "f", 0L),
                  GET_CON_Y(NAME("rs/%d/%d", k, i), NAME("e%d", 3 - l), 0L));
         }
         COPY_UP_CON(1L, "vdd", NAME("rs/%d/%d", k, i), "vdd");
         COPY_UP_CON(1L, "vss", NAME("rs/%d/%d", k, i), "vss");
      }
      for (i = 0; i < lnblk; i++) {
         refname = (k & (1 << j - i - 1)) ? NAME("adr[%d]", i)
                                  : NAME("adrb[%d]", i);
         COPY_UP_CON(0L, NAME("e%d", i + 1), NAME("rbs/%d", k), refname);
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", i),
                             (k & (1 << j - i - 1)) ? "i" : "f", 0L),
               GET_CON_Y(NAME("rbs/%d", k), NAME("e%d", i + 1), 0L));
      }

      for (i = 0; i < 4; i++) {
         COPY_UP_CON(i & 1, NAME("e%d", 12 + j - i), NAME("rbs/%d", k),
                                (i & 1) ? NAME("adr[%d]", 5 + j + i / 2 )
                                        : NAME("adrb[%d]", 5 + j + i / 2));
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", 5 + j + i / 2), i & 1 ? "i" : "f", 0L),
               GET_CON_Y(NAME("rbs/%d", k), NAME("e%d", 12 + j - i), i & 1));
      }
      for (i = 0; i < 4; i++) {
         COPY_UP_CON(1L, NAME("e%d", 8 + j - i), NAME("rbs/%d", k),
                                (i & 1) ? NAME("adr[%d]", 7 + j + i / 2 )
                                        : NAME("adrb[%d]", 7 + j + i / 2));
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", 7 + j + i / 2), i & 1 ? "i" : "f", 0L),
               GET_CON_Y(NAME("rbs/%d", k), NAME("e%d", 8 + j - i), 1L));
      }
      for (i = 0; i < 4; i++) {
         COPY_UP_CON(1L, NAME("e%d", 4 + j - i), NAME("rbs/%d", k),
                                (i & 1) ? NAME("adr[%d]", 3 + j + i / 2 )
                                        : NAME("adrb[%d]", 3 + j + i / 2));
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", 3 + j + i / 2), i & 1 ? "i" : "f", 0L),
               GET_CON_Y(NAME("rbs/%d", k), NAME("e%d", 4 + j - i), 1L));
      }

      COPY_UP_CON(1L, "vss", NAME("rbs/%d", k), "vss");
      COPY_UP_CON(1L, "vdd", NAME("rbs/%d", k), "vdd");
      COPY_UP_CON(3L, "vdd", NAME("rbs/%d", k), "vdd");
      COPY_UP_CON(0L, "ck", NAME("rck/%d", k), NAME("ck[%d]", k / 2));
      COPY_UP_CON(2L, "vdd", NAME("rck/%d", k), "vdd");

      for (i = 1; i <= 35; i += 2) 
         COPY_UP_CON(i, "vss", NAME("rboh/%d", k), "vss");
      COPY_UP_CON(0L, "vdd0", NAME("rboh/%d", k), "vdd");
      COPY_UP_CON(0L, "vdd1", NAME("rboh/%d", k), "vdd");
      COPY_UP_CON(0L, "vdd2", NAME("rboh/%d", k), "vdd");
   }

   if (blk & 1) {
      for (i = 0; i < 4 * nb; i++) {
        COPY_UP_CON(2L, "vdd", NAME("rp4/%d/%d", blk - 1, i), "vdd");
        COPY_UP_CON(1L, "vss", NAME("rp4/%d/%d", blk - 1, i), "vss");
      }
      COPY_UP_CON(3L, "vdd", NAME("rck/%d", blk - 1), "vdd");
      COPY_UP_CON(1L, "vss0", NAME("rck/%d", blk - 1), "vss");
      COPY_UP_CON(1L, "vss1", NAME("rck/%d", blk - 1), "vss");
      COPY_UP_CON(1L, "vss2", NAME("rck/%d", blk - 1), "vss");
   } else {
      for (i = 0; i < nb; i++) {
        COPY_UP_CON(0L, "vdd0", NAME("rmx14/%d/%d", blk - 1, i), "vdd");
        COPY_UP_CON(0L, "vss", NAME("rmx14/%d/%d", blk - 1, i), "vss");
      }
      COPY_UP_CON(0L, "vdd0", NAME("rx416/%d", blk - 1), "vdd");
      COPY_UP_CON(1L, "vdd0", NAME("rx416/%d", blk - 1), "vdd");
      COPY_UP_CON(0L, "vdd1", NAME("rx416/%d", blk - 1), "vdd");
      COPY_UP_CON(0L, "vss", NAME("rx416/%d", blk - 1), "vss");
      COPY_UP_CON(1L, "vss", NAME("rx416/%d", blk - 1), "vss");
      COPY_UP_CON(2L, "vss", NAME("rx416/%d", blk - 1), "vss");

      COPY_UP_CON(0L, "vdd", NAME("rc116/%d", blk - 1), "vdd");
      COPY_UP_CON(0L, "vdd", NAME("rw0/%d", blk - 1), "vdd");
      COPY_UP_CON(0L, "vss", NAME("rw2/%d", blk - 1), "vss");
   }

   COPY_UP_CON(0L, "vss", "rbol", "vss");
   COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(1L, "vss", "rli/0", "vss");

   COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   COPY_UP_CON(0L, "vss", "rzhb", "vss");
   COPY_UP_CON(1L, "vss", "rzhb", "vss");

   COPY_UP_CON(0L, "vdd", "rept", "vdd");
   COPY_UP_CON(0L, "vss", "rept", "vss");

   /* import output connectors */
   for (i = 0; i < nb; i++)
      if (r)
         COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", nb - i - 1));
      else
         COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", i));

   /* now import the metal thru routes */
   if (tr)
      for (k = 0; k < blk; k++)
         for (i = 0; i < nb; i++)
            COPY_UP_SEG("tr", NAME("rmt/%d/%d", k, i), NAME("tr_%d", i));
   for (i = 0; i < nb; i++) {
      if (tr)
         COPY_UP_SEG("tr", NAME("rot/%d", i), NAME("tr_%d", i));
      if (blk == 1) {
         COPY_UP_SEG("tr", NAME("rob/%d", i), NAME("tr_%d", i));
         COPY_UP_SEG("tr", NAME("rout/0/%d", i), NAME("tr_%d", i));
         COPY_UP_SEG("tr", NAME("rmx14/0/%d", i), NAME("tr_%d", i));
      }
   }
}

void
   ublock(nw, nb, tr, zh, r)
long nw, nb, tr, zh, r;
{
long lnw; /* number of address lines */
long i, j, l;
char *refname; /* reference instance name */

   lnw = ln2p(nw);

   /* output line : body ties, output buffers, address line inverters */
   PLACE("grmbob_c", "rbol", NOSYM, 0, 0);
   PLACE_TOP("grubom_c", "rboh", NOSYM);
   DEF_PHINS("rbol");
   for (i = 0; i < nb; i++) {
      if (i & 1) {
         if (zh)
            PLACE_RIGHT(tr ? "gruoboht_c" : "gruoboh_c",
                        NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         else
            PLACE_RIGHT(tr ? "gruobot_c" : "gruobo_c",
                        NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         PLACE_TOP(tr ? "grumx2ot_c" : "grumx2o_c",
                        NAME("rmx2/%d", i), i & 1 ? SYM_X : NOSYM);
      } else {
         if (zh)
            PLACE_RIGHT(tr ? "gruobeht_c" : "gruobeh_c",
                        NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         else
            PLACE_RIGHT(tr ? "gruobet_c" : "gruobe_c",
                        NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         PLACE_TOP(tr ? "grumx2et_c" : "grumx2e_c",
                        NAME("rmx2/%d", i), i & 1 ? SYM_X : NOSYM);
      }
      DEF_PHINS(NAME("rob/%d", i));
   }
   if (zh) {
      PLACE_RIGHT("gruobfh_c", "bf", NOSYM);
      PLACE_RIGHT("gruoebh_c", "rzhb", NOSYM);
   } else {
      if (nb & 1)
         PLACE_RIGHT("gruobfo_c", "bf", NOSYM);
      else
         PLACE_RIGHT("gruobfe_c", "bf", NOSYM);
      PLACE_RIGHT("grufill_c", NAME("rzhb", i), NOSYM);
   }

   PLACE_RIGHT("grmfeed_c", NAME("feed", i), NOSYM);
   for (i = lnw - 1; i >= 0; i--)
      PLACE_RIGHT("grmli_c", NAME("rli/%d", i), NOSYM);


   /* blocks generation : transistor matrix, word and bit decoders */
   /* matrix building */
   DEF_PHINS("rmx2/0");
   j = 0;
   for (i = 0; i < nb; i ++) { /* for each bit */
      if (i == 0)
         PLACE_TOP("grmx4_c", NAME("rmx4/%d", j), NOSYM);
      else
         PLACE_RIGHT("grmx4_c", NAME("rmx4/%d", j), NOSYM);
      PLACE_TOP("grbl4_c", NAME("rbl4/0/%d", j), NOSYM);
      PLACE_TOP("grbl4_c", NAME("rbl4/1/%d", j), NOSYM);
      PLACE_TOP("grp4_c", NAME("rp4/%d", j), NOSYM);

      DEF_PHINS(NAME("rmx4/%d", j++));
      if (tr)
         PLACE_RIGHT("grnmht_c", NAME("rmt/%d", i), NOSYM);

      PLACE_RIGHT("grmx4_c", NAME("rmx4/%d", j), SYM_X);
      PLACE_TOP("grbl4_c", NAME("rbl4/0/%d", j), SYM_X);
      PLACE_TOP("grbl4_c", NAME("rbl4/1/%d", j), SYM_X);
      PLACE_TOP("grp4_c", NAME("rp4/%d", j), SYM_X);

      DEF_PHINS(NAME("rmx4/%d", j++));
      if ((i & 1) && i == nb - 1)
         PLACE_RIGHT("grumf_c", "umf", NOSYM);
      if (!(i & 1))
         if (i != nb - 1)
            PLACE_RIGHT("grmrst_c", NAME("rp1/%d", i), NOSYM);
         else
            PLACE_RIGHT("grnrste_c", NAME("rp1/%d", i), NOSYM);
   }
   DEF_PHINS("rzhb");
   /* decoder building */
   PLACE_TOP("grurx2_c", "x2", NOSYM);
   PLACE_TOP("grurx1_c", "x1", NOSYM);
   PLACE_TOP("grprx0_c", "x0", NOSYM);
   DEF_PHINS("feed");
   PLACE_TOP("gruwi_c", "wires", NOSYM);
   PLACE_TOP("grpick_c", "rc116", NOSYM);
   PLACE_RIGHT("grprw0_c", "rw0", NOSYM);
   PLACE_RIGHT("grprw1_c", "rw1", NOSYM);
   PLACE_RIGHT("grprw2_c", "rw2", NOSYM);
   PLACE_RIGHT("grprw3_c", "rw3", NOSYM);
   PLACE_RIGHT("grnbs_c", "rbs", NOSYM);
   for (i = 0; i < 8; i++) {
      if (!i)
         DEF_PHINS("x0");
      else 
         DEF_PHINS(NAME("rbu/%d", i - 1));
      PLACE_TOP("grmrwb_c", NAME("rbu/%d", i), NOSYM);
      PLACE_RIGHT("grmrl_c", NAME("rl/%d", i), NOSYM);
      PLACE_RIGHT("grmrs_c", NAME("rs/%d", i), NOSYM);
   }
   DEF_PHINS(NAME("rbu/%d", i - 1));
   PLACE_TOP("grmrck_c", "rck", NOSYM);
   DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      COPY_UP_CON(0L, "i", NAME("rli/%d", i), NAME("adr[%d]", i));
      COPY_UP_CON(0L, "f", NAME("rli/%d", i), NAME("adrb[%d]", i));
   }
   /* draw crossing lines and appropriate vias for address decod */
   for (i = 0; i < 8; i++) {
      for (l = 0; l < 3; l++) {
         refname = (i & (1 << l)) ? NAME("adr[%d]", 2 - l)
                                  : NAME("adrb[%d]", 2 - l);
         COPY_UP_CON(0L, NAME("e%d", 3 - l), NAME("rs/%d", i), refname);
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", 2 - l),
                             (i & (1 << l)) ? "i" : "f", 0L),
               GET_CON_Y(NAME("rs/%d", i), NAME("e%d", 3 - l), 0L));
      }
      COPY_UP_CON(1L, "vdd", NAME("rs/%d", i), "vdd");
      COPY_UP_CON(1L, "vss", NAME("rs/%d", i), "vss");
   }
   /* rprwi decoding */
   for (i = 0; i < 4; i++) {
      COPY_UP_CON(1L, NAME("e%d", i + 1), "rbs",
                             (i & 1) ? NAME("adr[%d]", 3 + i / 2 )
                                     : NAME("adrb[%d]", 3 + i / 2));
      PHVIA(CONT_VIA,
            GET_CON_X(NAME("rli/%d", 3 + i / 2), i & 1 ? "i" : "f", 0L),
            GET_CON_Y("rbs", NAME("e%d", i + 1), 1L));
   }
   /* rmx04 decoding */
   for (i = 0; i < 4; i++) {
      COPY_UP_CON(1L, NAME("e%d", i + 5), "rbs",
                             !(i & 1) ? NAME("adr[%d]", 7 - i / 2 )
                                     : NAME("adrb[%d]", 7 - i / 2));
      PHVIA(CONT_VIA,
            GET_CON_X(NAME("rli/%d", 7 - i / 2), !(i & 1) ? "i" : "f", 0L),
            GET_CON_Y("rbs", NAME("e%d", i + 5), 1L));
   }
   /* low addresses : rmx2/n */
   for (i = 0; i < 2; i++) {
         COPY_UP_CON(1L, NAME("s%d", i), "wires",
                    (i & 1) ? "adr[5]" : "adrb[5]");
         PHVIA(CONT_VIA,
               GET_CON_X("rli/5", (i & 1) ? "i" : "f", 0L),
               GET_CON_Y("wires", NAME("s%d", i ), 1L));
   }

   COPY_UP_CON(2L, "vss", "wires", "vss");
   COPY_UP_CON(2L, "vdd", "wires", "vdd");

   COPY_UP_CON(2L, "vss", "rbs", "vss");
   COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   COPY_UP_CON(2L, "vss", "rbs", "vss");
   COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   COPY_UP_CON(4L, "vdd", "rbs", "vdd");
   COPY_UP_CON(0L, "ck", "rbs", "ck[0]");

   COPY_UP_CON(0L, "ck", "rck", "ck[1]");
   COPY_UP_CON(2L, "vdd", "rck", "vdd");
   COPY_UP_CON(3L, "vdd", "rck", "vdd");
   COPY_UP_CON(1L, "vss0", "rck", "vss");
   COPY_UP_CON(1L, "vss1", "rck", "vss");
   COPY_UP_CON(1L, "vss2", "rck", "vss");

   for (i = 0; i < nb; i += 2 - tr) 
      COPY_UP_CON(1L, "vss", NAME("rp4/%d", i), "vss");

   COPY_UP_CON(0L, "vss", "rbol", "vss");
   COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   for (i = 1; i <= 35; i += 2) 
      COPY_UP_CON(i, "vss", "rboh", "vss");
   COPY_UP_CON(0L, "vdd0", "rboh", "vdd");
   COPY_UP_CON(0L, "vdd1", "rboh", "vdd");
   COPY_UP_CON(0L, "vdd2", "rboh", "vdd");

   COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   COPY_UP_CON(0L, "vss", "rzhb", "vss");
   COPY_UP_CON(1L, "vss", "rzhb", "vss");

   COPY_UP_CON(0L, "vdd", "feed", "vdd");
   COPY_UP_CON(0L, "vss", "feed", "vss");

   COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(1L, "vss", "rli/0", "vss");

   /* import output connectors */
   for (i = 0; i < nb; i++)
      if (r)
         COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", nb - i - 1));
      else
         COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", i));

      /* through routes now */
      if (tr)
         for (i = 0; i < nb; i++) {
            COPY_UP_SEG("tr", NAME("rob/%d", i), NAME("tr_%d", i));
            COPY_UP_SEG("tr", NAME("rmx2/%d", i), NAME("tr_%d", i));
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i));
         }
}

void
   nblock(nw, nb, tr, zh, r)
long nw, nb, tr, zh, r;
{
long lnw; /* number of address lines */
long i, j, l;
char *refname; /* reference instance name */
int nosym, sym_x;

   lnw = ln2p(nw);

   /* output line : body ties, output buffers, address line inverters */
   if (!(nb & 1)) {
      nosym = NOSYM;
      sym_x = SYM_X;
   } else {
      nosym = SYM_X;
      sym_x = NOSYM;
   }
   if (zh) {
      PLACE("grpubobh_c", "rbol", NOSYM, 0, 0);
      if (nb & 1)
         PLACE_RIGHT("grpobhs_c", "robert", NOSYM);
      for (i = 0; i < nb / 2; i++) {
         if (tr)
            PLACE_RIGHT("grpobhtc_c", NAME("rob/%d", i), NOSYM);
         else
            PLACE_RIGHT("grpobhc_c", NAME("rob/%d", i), NOSYM);
      }
      PLACE_RIGHT("grroebh_c", NAME("rzhb", i), NOSYM);
      PLACE_RIGHT("grpfeedh_c", NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         PLACE_RIGHT("grpli_c", NAME("rli/%d", i), NOSYM);
   } else {
      PLACE("grpubob_c", "rbol", NOSYM, 0, 0);
      for (i = 0; i < nb; i++) {
         PLACE_RIGHT("grpob_c", NAME("rob/%d", i), i & 1 ? sym_x : nosym);
         if (i != nb - 1 && (!(nb & 1)) && (i & 1))
            PLACE_RIGHT("grpf_c", NAME("mrpl/%d", i), NOSYM);
         else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
            PLACE_RIGHT("grpf_c", NAME("mrpl/%d", i), NOSYM);
         if (tr && (nb & 1) && (i & 1))
            PLACE_RIGHT("grpubt_c", NAME("rot/%d", i), NOSYM);
         if (tr && !(nb & 1) && !(i & 1))
            PLACE_RIGHT("grpubt_c", NAME("rot/%d", i), NOSYM);
      }
      PLACE_RIGHT("grpf_c", NAME("rpf/%d", i), NOSYM);
      PLACE_RIGHT("grpfill_c", NAME("rzhb", i), NOSYM);
      PLACE_RIGHT("grpfeed_c", NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         PLACE_RIGHT("grpli_c", NAME("rli/%d", i), NOSYM);
   }

   /* blocks generation : transistor matrix, word and bit decoders */
   DEF_PHINS("rbol");
   PLACE_TOP("grnbom_c", "rboh", NOSYM);

   /* matrix building */
   for (i = 0; i < nb; i ++) { /* for each bit */
      PLACE_RIGHT("grmx4_c", NAME("rmx4/%d", i), i & 1 ? sym_x : nosym);
      PLACE_TOP("grbl4_c", NAME("rbl4/0/%d", i), i & 1 ? sym_x : nosym);
      PLACE_TOP("grbl4_c", NAME("rbl4/1/%d", i), i & 1 ? sym_x : nosym);
      PLACE_TOP("grp4_c", NAME("rp4/%d", i), i & 1 ? sym_x : nosym);

      DEF_PHINS(NAME("rmx4/%d", i));
      if (zh && i == nb - 1)
         PLACE_RIGHT("grnrste_c", NAME("rp1/%d", i), NOSYM);
      if (i != nb - 1 && (!(nb & 1)) && (i & 1))
         PLACE_RIGHT("grmrst_c", NAME("rp1/%d", i), NOSYM);
      else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
         PLACE_RIGHT("grmrst_c", NAME("rp1/%d", i), NOSYM);

      if (zh && tr && (i & 1) && (nb & 1))
         PLACE_RIGHT("grnmht_c", NAME("rmt/%d", i), NOSYM);
      else if (zh && tr && !(i & 1) && !(nb & 1))
         PLACE_RIGHT("grnmht_c", NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && (i & 1) && (nb & 1))
         PLACE_RIGHT("grnmht_c", NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && !(nb & 1) && !(i & 1))
         PLACE_RIGHT("grnmht_c", NAME("rmt/%d", i), NOSYM);
   }
   if (!zh)
      PLACE_RIGHT("grnrste_c", NAME("rp1/%d", i), NOSYM);
   /* decoder building */
   PLACE_RIGHT("grprx1_c", "x1", NOSYM);
   PLACE_TOP("grprx0_c", "x0", NOSYM);
   DEF_PHINS("feed");
   PLACE_TOP("grpick_c", "rc116", NOSYM);
   PLACE_RIGHT("grprw0_c", "rw0", NOSYM);
   PLACE_RIGHT("grprw1_c", "rw1", NOSYM);
   PLACE_RIGHT("grprw2_c", "rw2", NOSYM);
   PLACE_RIGHT("grprw3_c", "rw3", NOSYM);
   PLACE_RIGHT("grnbs_c", "rbs", NOSYM);
   for (i = 0; i < 8; i++) {
      if (!i)
         DEF_PHINS("x0");
      else 
         DEF_PHINS(NAME("rbu/%d", i - 1));
      PLACE_TOP("grmrwb_c", NAME("rbu/%d", i), NOSYM);
      PLACE_RIGHT("grmrl_c", NAME("rl/%d", i), NOSYM);
      PLACE_RIGHT("grmrs_c", NAME("rs/%d", i), NOSYM);
   }
   DEF_PHINS(NAME("rbu/%d", i - 1));
   PLACE_TOP("grmrck_c", "rck", NOSYM);
   DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      COPY_UP_CON(0L, "i", NAME("rli/%d", i), NAME("adr[%d]", i));
      COPY_UP_CON(0L, "f", NAME("rli/%d", i), NAME("adrb[%d]", i));
   }
   /* draw crossing lines and appropriate vias for address decod */
   for (i = 0; i < 8; i++) {
      for (l = 0; l < 3; l++) {
         refname = (i & (1 << l)) ? NAME("adr[%d]", 2 - l)
                                  : NAME("adrb[%d]", 2 - l);
         COPY_UP_CON(0L, NAME("e%d", 3 - l), NAME("rs/%d", i), refname);
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", 2 - l),
                             (i & (1 << l)) ? "i" : "f", 0L),
               GET_CON_Y(NAME("rs/%d", i), NAME("e%d", 3 - l), 0L));
      }
      COPY_UP_CON(1L, "vdd", NAME("rs/%d", i), "vdd");
      COPY_UP_CON(1L, "vss", NAME("rs/%d", i), "vss");
   }
   /* rprwi decoding */
   for (i = 0; i < 4; i++) {
      COPY_UP_CON(1L, NAME("e%d", i + 1), "rbs",
                             (i & 1) ? NAME("adr[%d]", 3 + i / 2 )
                                     : NAME("adrb[%d]", 3 + i / 2));
      PHVIA(CONT_VIA,
            GET_CON_X(NAME("rli/%d", 3 + i / 2), i & 1 ? "i" : "f", 0L),
            GET_CON_Y("rbs", NAME("e%d", i + 1), 1L));
   }
   /* rmx04 decoding */
   for (i = 0; i < 4; i++) {
      COPY_UP_CON(1L, NAME("e%d", i + 5), "rbs",
                             !(i & 1) ? NAME("adr[%d]", 6 - i / 2 )
                                     : NAME("adrb[%d]", 6 - i / 2));
      PHVIA(CONT_VIA,
            GET_CON_X(NAME("rli/%d", 6 - i / 2), !(i & 1) ? "i" : "f", 0L),
            GET_CON_Y("rbs", NAME("e%d", i + 5), 1L));
   }

   /* copy power supplies and clocks */
   COPY_UP_CON(2L, "vss", "rbs", "vss");
   COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   COPY_UP_CON(4L, "vdd", "rbs", "vdd");
   COPY_UP_CON(0L, "ck", "rbs", "ck[0]");

   COPY_UP_CON(0L, "ck", "rck", "ck[1]");
   COPY_UP_CON(3L, "vdd", "rck", "vdd");
   COPY_UP_CON(2L, "vdd", "rck", "vdd");
   COPY_UP_CON(1L, "vss0", "rck", "vss");
   COPY_UP_CON(1L, "vss1", "rck", "vss");
   COPY_UP_CON(1L, "vss2", "rck", "vss");

   COPY_UP_CON(0L, "vss", "rbol", "vss");
   COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   for (i = 1; i <= 35; i += 2) 
      COPY_UP_CON(i, "vss", "rboh", "vss");
   COPY_UP_CON(0L, "vdd0", "rboh", "vdd");
   COPY_UP_CON(0L, "vdd1", "rboh", "vdd");
   COPY_UP_CON(0L, "vdd", "rboh", "vdd");

   for (i = 0; i < nb; i += 2 - tr) 
      COPY_UP_CON(1L, "vss", NAME("rp4/%d", i), "vss");

   COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   COPY_UP_CON(0L, "vss", "rzhb", "vss");
   COPY_UP_CON(1L, "vss", "rzhb", "vss");

   COPY_UP_CON(0L, "vdd", "feed", "vdd");
   COPY_UP_CON(0L, "vss", "feed", "vss");

   COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(1L, "vss", "rli/0", "vss");


   /* import output connectors */
   if (zh) {
      if (r) {
         j = nb - 1;
         if (nb & 1)
            COPY_UP_CON(0L, "f1", "robert", NAME("f[%d]", j--));
         for (i = 0; i < nb / 2; i++) {
            COPY_UP_CON(0L, "f0", NAME("rob/%d", i), NAME("f[%d]", j--));
            COPY_UP_CON(0L, "f1", NAME("rob/%d", i), NAME("f[%d]", j--));
         }
      } else {
         j = 0;
         if (nb & 1)
            COPY_UP_CON(0L, "f1", "robert", NAME("f[%d]", j++));
         for (i = 0; i < nb / 2; i++) {
            COPY_UP_CON(0L, "f0", NAME("rob/%d", i), NAME("f[%d]", j++));
            COPY_UP_CON(0L, "f1", NAME("rob/%d", i), NAME("f[%d]", j++));
         }
      }
   } else
      for (i = 0; i < nb; i++)
         if (r)
            COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", nb - i - 1));
         else
            COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", i));

   /* metal one feed through */
   if (tr) {
      for (i = 0; i < nb; i++) {
         if (zh && (i & 1) && (nb & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
         else if (zh && !(i & 1) && !(nb & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
         else if (!zh && (i & 1) && (nb & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
         else if (!zh && !(nb & 1) && !(i & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
      }
      if (zh)
         for (i = 0; i < nb / 2; i++)
            COPY_UP_SEG("tr", NAME("rob/%d", i), NAME("tr_%d", i));
      else
         for (i = 0; i < nb; i++) {
            if (((nb & 1) && (i & 1)) || (!(nb & 1) && !(i & 1)))
               COPY_UP_SEG("tr", NAME("rot/%d", i), NAME("tr_%d", i / 2));
         }
   }
}

void
   pblock(nw, nb, tr, zh, r)
long nw, nb, tr, zh, r;
{
long lnw; /* number of address lines */
long i, j, l;
char *refname; /* reference instance name */
int nosym, sym_x;

   lnw = ln2p(nw);

   /* output line : body ties, output buffers, address line inverters */
   if (!(nb & 1)) {
      nosym = NOSYM;
      sym_x = SYM_X;
   } else {
      nosym = SYM_X;
      sym_x = NOSYM;
   }
   if (zh) {
      PLACE("grpubobh_c", "rbol", NOSYM, 0, 0);
      if (nb & 1)
         PLACE_RIGHT("grpobhs_c", "robert", NOSYM);
      for (i = 0; i < nb / 2; i++) {
         if (tr)
            PLACE_RIGHT("grpobhtc_c", NAME("rob/%d", i), NOSYM);
         else
            PLACE_RIGHT("grpobhc_c", NAME("rob/%d", i), NOSYM);
      }
      PLACE_RIGHT("grroebh_c", NAME("rzhb", i), NOSYM);
      PLACE_RIGHT("grpfeedh_c", NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         PLACE_RIGHT("grpli_c", NAME("rli/%d", i), NOSYM);
   } else {
      PLACE("grpubob_c", "rbol", NOSYM, 0, 0);
      for (i = 0; i < nb; i++) {
         PLACE_RIGHT("grpob_c", NAME("rob/%d", i), i & 1 ? sym_x : nosym);
         if (i != nb - 1 && (!(nb & 1)) && (i & 1))
            PLACE_RIGHT("grpf_c", NAME("mrpl/%d", i), NOSYM);
         else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
            PLACE_RIGHT("grpf_c", NAME("mrpl/%d", i), NOSYM);
         if (tr && (nb & 1) && (i & 1))
            PLACE_RIGHT("grpubt_c", NAME("rot/%d", i), NOSYM);
         if (tr && !(nb & 1) && !(i & 1))
            PLACE_RIGHT("grpubt_c", NAME("rot/%d", i), NOSYM);
      }
      PLACE_RIGHT("grpf_c", NAME("rpf/%d", i), NOSYM);
      PLACE_RIGHT("grpfill_c", NAME("rzhb", i), NOSYM);
      PLACE_RIGHT("grpfeed_c", NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         PLACE_RIGHT("grpli_c", NAME("rli/%d", i), NOSYM);
   }

   /* blocks generation : transistor matrix, word and bit decoders */
   DEF_PHINS("rbol");
   PLACE_TOP("grpbom_c", "rboh", NOSYM);

   /* matrix building */
   for (i = 0; i < nb; i ++) { /* for each bit */
      PLACE_RIGHT("grmx4_c", NAME("rmx4/%d", i), i & 1 ? sym_x : nosym);
      PLACE_TOP("grbl4_c", NAME("rbl4/0/%d", i), i & 1 ? sym_x : nosym);
      PLACE_TOP("grp4_c", NAME("rp4/%d", i), i & 1 ? sym_x : nosym);

      DEF_PHINS(NAME("rmx4/%d", i));
      if (zh && i == nb - 1)
         PLACE_RIGHT("grprste_c", NAME("rp1/%d", i), NOSYM);
      if (i != nb - 1 && (!(nb & 1)) && (i & 1))
         PLACE_RIGHT("grprst_c", NAME("rp1/%d", i), NOSYM);
      else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
         PLACE_RIGHT("grprst_c", NAME("rp1/%d", i), NOSYM);

      if (zh && tr && (i & 1) && (nb & 1))
         PLACE_RIGHT("grpmht_c", NAME("rmt/%d", i), NOSYM);
      else if (zh && tr && !(i & 1) && !(nb & 1))
         PLACE_RIGHT("grpmht_c", NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && (i & 1) && (nb & 1))
         PLACE_RIGHT("grpmht_c", NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && !(nb & 1) && !(i & 1))
         PLACE_RIGHT("grpmht_c", NAME("rmt/%d", i), NOSYM);
   }
   if (!zh)
      PLACE_RIGHT("grprste_c", NAME("rp1/%d", i), NOSYM);
   /* decoder building */
   PLACE_RIGHT("grprx1_c", "x1", NOSYM);
   PLACE_TOP("grprx0_c", "x0", NOSYM);
   DEF_PHINS("feed");
   PLACE_TOP("grpick_c", "rc116", NOSYM);
   PLACE_RIGHT("grprw0_c", "rw0", NOSYM);
   PLACE_RIGHT("grprw1_c", "rw1", NOSYM);
   PLACE_RIGHT("grprw2_c", "rw2", NOSYM);
   PLACE_RIGHT("grprw3_c", "rw3", NOSYM);
   PLACE_RIGHT("grpbs_c", "rbs", NOSYM);
   for (i = 0; i < 4; i++) {
      if (!i)
         DEF_PHINS("x0");
      else 
         DEF_PHINS(NAME("rbu/%d", i - 1));
      PLACE_TOP("grmrwb_c", NAME("rbu/%d", i), NOSYM);
      PLACE_RIGHT("grmrl_c", NAME("rl/%d", i), NOSYM);
      PLACE_RIGHT("grprs_c", NAME("rs/%d", i), NOSYM);
   }
   DEF_PHINS(NAME("rbu/%d", i - 1));
   PLACE_TOP("grmrck_c", "rck", NOSYM);
   DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      COPY_UP_CON(0L, "i", NAME("rli/%d", i), NAME("adr[%d]", i));
      COPY_UP_CON(0L, "f", NAME("rli/%d", i), NAME("adrb[%d]", i));
   }
   /* draw crossing lines and appropriate vias for address decod */
   for (i = 0; i < 4; i++) {
      for (l = 0; l < 2; l++) {
         refname = (i & (1 << l)) ? NAME("adr[%d]", 1 - l)
                                  : NAME("adrb[%d]", 1 - l);
         COPY_UP_CON(0L, NAME("e%d", l + 1), NAME("rs/%d", i), refname);
         PHVIA(CONT_VIA,
               GET_CON_X(NAME("rli/%d", 1 - l),
                             (i & (1 << l)) ? "i" : "f", 0L),
               GET_CON_Y(NAME("rs/%d", i), NAME("e%d", l + 1), 0L));
      }
      COPY_UP_CON(1L, "vdd", NAME("rs/%d", i), "vdd");
      COPY_UP_CON(1L, "vss", NAME("rs/%d", i), "vss");
   }
   /* rprwi decoding */
   for (i = 0; i < 4; i++) {
      COPY_UP_CON(1L, NAME("e%d", i + 1), "rbs",
                             (i & 1) ? NAME("adr[%d]", 2 + i / 2 )
                                     : NAME("adrb[%d]", 2 + i / 2));
      PHVIA(CONT_VIA,
            GET_CON_X(NAME("rli/%d", 2 + i / 2), i & 1 ? "i" : "f", 0L),
            GET_CON_Y("rbs", NAME("e%d", i + 1), 1L));
   }
   /* rmx04 decoding */
   for (i = 0; i < 4; i++) {
      COPY_UP_CON(1L, NAME("e%d", i + 5), "rbs",
                             !(i & 1) ? NAME("adr[%d]", 5 - i / 2 )
                                     : NAME("adrb[%d]", 5 - i / 2));
      PHVIA(CONT_VIA,
            GET_CON_X(NAME("rli/%d", 5 - i / 2), !(i & 1) ? "i" : "f", 0L),
            GET_CON_Y("rbs", NAME("e%d", i + 5), 1L));
   }

   /* copy power supplies and clocks */
   COPY_UP_CON(0L, "vss", "rbol", "vss");
   COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   for (i = 1; i <= 19; i += 2) 
      COPY_UP_CON(i, "vss", "rboh", "vss");
   COPY_UP_CON(0L, "vdd0", "rboh", "vdd");
   COPY_UP_CON(0L, "vdd1", "rboh", "vdd");
   COPY_UP_CON(0L, "vdd", "rboh", "vdd");

   for (i = 0; i < nb; i += 2 - tr) 
      COPY_UP_CON(1L, "vss", NAME("rp4/%d", i), "vss");

   COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   COPY_UP_CON(0L, "vss", "rzhb", "vss");
   COPY_UP_CON(1L, "vss", "rzhb", "vss");

   COPY_UP_CON(0L, "vdd", "feed", "vdd");
   COPY_UP_CON(0L, "vss", "feed", "vss");

   COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   COPY_UP_CON(1L, "vss", "rli/0", "vss");

   COPY_UP_CON(1L, "vss", "rbs", "vss");
   COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   COPY_UP_CON(3L, "vdd", "rbs", "vdd");
   COPY_UP_CON(0L, "ck", "rbs", "ck");

   COPY_UP_CON(0L, "ck", "rck", "ck");
   COPY_UP_CON(3L, "vdd", "rck", "vdd");
   COPY_UP_CON(2L, "vdd", "rck", "vdd");
   COPY_UP_CON(1L, "vss0", "rck", "vss");
   COPY_UP_CON(1L, "vss1", "rck", "vss");
   COPY_UP_CON(1L, "vss2", "rck", "vss");

   /* import output connectors */
   if (zh) {
      if (r) {
         j = nb - 1;
         if (nb & 1)
            COPY_UP_CON(0L, "f1", "robert", NAME("f[%d]", j--));
         for (i = 0; i < nb / 2; i++) {
            COPY_UP_CON(0L, "f0", NAME("rob/%d", i), NAME("f[%d]", j--));
            COPY_UP_CON(0L, "f1", NAME("rob/%d", i), NAME("f[%d]", j--));
         }
      } else {
         j = 0;
         if (nb & 1)
            COPY_UP_CON(0L, "f1", "robert", NAME("f[%d]", j++));
         for (i = 0; i < nb / 2; i++) {
            COPY_UP_CON(0L, "f0", NAME("rob/%d", i), NAME("f[%d]", j++));
            COPY_UP_CON(0L, "f1", NAME("rob/%d", i), NAME("f[%d]", j++));
         }
      }
   } else
      for (i = 0; i < nb; i++)
         if (r)
            COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", nb - i - 1));
         else
            COPY_UP_CON(0L, "f", NAME("rob/%d", i), NAME("f[%d]", i));

   /* metal one feed through */
   if (tr) {
      for (i = 0; i < nb; i++) {
         if (zh && (i & 1) && (nb & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
         else if (zh && !(i & 1) && !(nb & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
         else if (!zh && (i & 1) && (nb & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
         else if (!zh && !(nb & 1) && !(i & 1))
            COPY_UP_SEG("tr", NAME("rmt/%d", i), NAME("tr_%d", i / 2)); 
      }
      if (zh)
         for (i = 0; i < nb / 2; i++)
            COPY_UP_SEG("tr", NAME("rob/%d", i), NAME("tr_%d", i));
      else
         for (i = 0; i < nb; i++) {
            if (((nb & 1) && (i & 1)) || (!(nb & 1) && !(i & 1)))
               COPY_UP_SEG("tr", NAME("rot/%d", i), NAME("tr_%d", i / 2));
         }
   }
}

#define bit(n,x)  (n < 0 ? 0 : (x >> (n)) & 1) /* value of the nth bit if x */

/*
* rom data coding
*/
void
   poke(nw, nb, data)
long nw, nb;
long *data;
{
long a, b, wl, i, j, k;

   for (k = i = 0; i < nw; i++) {
      if (i != 0 && !(i % 4))
          k += 1;
      if (k == 4)
          k = 0;
      b = i / 512;
      a = (i % 512) / 256;
      wl = 15 - ((i % 512) % 256) / 16;
      for (j = 0; j < nb; j++) {
         (void)sprintf(instance, "rbl4/%d/%d/%d", b, a, 4 * nb - 1 - 4 * j - k);
         (void)sprintf(reference, "fuse/%d/%d", wl, 3 - i % 4);
         if (bit(j, data[i]))
            PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

void
   upoke(nw, nb, data)
long nw, nb;
long *data;
{
long i, j, k;

   for (i = 0; i < nw; i++) {
      for (j = 0; j < nb; j++) {
         k = i / 128;  /* lower or upper bit lines rows */
         (void)sprintf(instance, "rbl4/%d/%d", k, 2 * (nb - j) - (i % 8) / 4 - 1);
         (void)sprintf(reference, "fuse/%d/%d", 15 - (i % 128) / 8, 3 - i % 4);
         if (bit(j, data[i]))
            PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

void
   npoke(nw, nb, zh, data)
long nw, nb, zh;
long *data;
{
long i, j, k;
long value = zh ? 1 : 0; /* poke depends on output buffer */

   for (i = 0; i < nw; i++) {
      for (j = 0; j < nb; j++) {
         k = i / 64;  /* lower or upper bit lines rows */
         (void)sprintf(instance, "rbl4/%d/%d", k, nb - 1 - j);
         (void)sprintf(reference, "fuse/%d/%d", 15 - (i % 64) / 4, 3 - i % 4);
         if (bit(j, data[i]) == value)
            PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

void
   ppoke(nw, nb, zh, data)
long nw, nb, zh;
long *data;
{
long i, j;
long value = zh ? 1 : 0; /* poke depends on output buffer */

   for (i = 0; i < nw; i++) {
      for (j = 0; j < nb; j++) {
         (void)sprintf(instance, "rbl4/0/%d", nb - 1 - j);
         (void)sprintf(reference, "fuse/%d/%d", 15 - i / 4, 3 - i % 4);
         if (bit(j, data[i]) == value)
            PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}






