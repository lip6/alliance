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
#include "mbkgen.h"
#include "grog.h"
#define AL_AL 5        /* pitch rule for alu1 */

static char reference[32]; /* fuse name in grbl4_c  */
static char instance[32];  /* grbl4_c instance name */

static void grogcontdifnonly(char *name)
{
phfig_list *p;
phvia_list *v, *pv;

   p = getphfig(name, 'A');
   /* Quite brutal now, but we are about to exit, so why bother, ... */
   p->MODELCHAIN = NULL;
   p->PHSEG      = NULL;
   p->PHREF      = NULL;
   p->PHINS      = NULL;
   p->PHCON      = NULL;
   for (pv = NULL, v = p->PHVIA; v; v = v->NEXT) {
      if (v->TYPE != CONT_DIF_N) {
         if (pv == NULL) {
            p->PHVIA = v->NEXT;
         } else
            pv->NEXT = v->NEXT;
         continue;
      }
      pv = v;
   }
}

/*
* rom floorplan generation
*/
static void block(int nw, int nb, int tr, int zh, int r)
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
   GENLIB_PLACE(blk == 1 ? "grmbob_c" : "grrbob_c", "rbol", NOSYM, 0L, 0L);
   for (i = 0L; i < nb; i++)
      if (zh) {
         GENLIB_PLACE_RIGHT(blk == 1 ? "grmobh_c" : "grrobh_c",
                     GENLIB_NAME("rob/%d", i), NOSYM);
         if (tr)
            GENLIB_PLACE_RIGHT(blk == 1 ? "grmoth_c" : "grroth_c",
                        GENLIB_NAME("rot/%d", i), NOSYM);
      }
      else {
         GENLIB_PLACE_RIGHT(blk == 1 ? "grmob_c" : "grrob_c", GENLIB_NAME("rob/%d", i), NOSYM);
         if (tr)
            GENLIB_PLACE_RIGHT(blk == 1 ? "grmbt_c" : "grrbt_c",
                        GENLIB_NAME("rot/%d", i), NOSYM);
      }
   if (zh)
      GENLIB_PLACE_RIGHT(blk == 1 ? "grmoebh_c" : "grroebh_c", GENLIB_NAME("rzhb", i), NOSYM);
   else
      GENLIB_PLACE_RIGHT(blk == 1 ? "grmfill_c" : "grrfill_c", GENLIB_NAME("rzhb", i), NOSYM);
   GENLIB_PLACE_RIGHT(blk == 1 ? "grmfeed_c" : "grrfeed_c", GENLIB_NAME("rept", i), NOSYM);
   for (i = lnw - 1; i >= 0L; i--)
      GENLIB_PLACE_RIGHT(blk == 1 ? "grmli_c" : "grrli_c", GENLIB_NAME("rli/%d", i), NOSYM);

   /* blocks generation : transistor matrix, word and bit decoders */
   for (k = 0L; k < blk; k++) { /* for each block */
      GENLIB_DEF_PHINS(k == 0L ? "rbol" : GENLIB_NAME("rboh/%d", k - 1));
      GENLIB_PLACE_TOP("grmrbom_c", GENLIB_NAME("rboh/%d", k), k & 1 ? SYM_Y : NOSYM);
      /* matrix building */
      if (!(k & 1)) { /* even blocks */
         for (j = 0L; j < nb; j ++) { /* for each bit */
            GENLIB_PLACE_RIGHT(blk == 1 ? "grmmx_c" : "grrmx_c",
                        GENLIB_NAME("rmx14/%d/%d", k, j), NOSYM);
            GENLIB_PLACE_TOP("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j), NOSYM);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j), NOSYM);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j), NOSYM);
            GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j), NOSYM);

            GENLIB_DEF_PHINS(GENLIB_NAME("rmx04/%d/%d", k, 4 * j));
            GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 1), SYM_X);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j + 1), SYM_X);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j + 1), SYM_X);
            GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j + 1), SYM_X);

            GENLIB_DEF_PHINS(GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 1));
            GENLIB_PLACE_RIGHT("grmrst_c", GENLIB_NAME("rp1/%d/%d", k, j), NOSYM);
            GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 2), NOSYM);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j + 2), NOSYM);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j + 2), NOSYM);
            GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j + 2), NOSYM);

            GENLIB_DEF_PHINS(GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 2));
            GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 3), SYM_X);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j + 3), SYM_X);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j + 3), SYM_X);
            GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j + 3), SYM_X);

            GENLIB_DEF_PHINS(GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 3));
            GENLIB_PLACE_RIGHT(blk == 1 ? "grmmot_c" : "grrmo_c",
                        GENLIB_NAME("rout/%d/%d", k, j), NOSYM);
         
            GENLIB_DEF_PHINS(GENLIB_NAME("rmx14/%d/%d", k, j));
            if (tr)
               GENLIB_PLACE_RIGHT(blk == 1 ? "grmmt_c" : "grrmt_c",
                           GENLIB_NAME("rmt/%d/%d", k, j), NOSYM);
         }
         /* decoder building */
         GENLIB_PLACE_RIGHT("grmrx3_c", GENLIB_NAME("rx416/%d", k), NOSYM);
         GENLIB_PLACE_TOP("grmrx2_c", GENLIB_NAME("rx316/%d", k), NOSYM);
         GENLIB_PLACE_TOP("grmrx1_c", GENLIB_NAME("rx216/%d", k), NOSYM);
         GENLIB_PLACE_TOP("grmrx0_c", GENLIB_NAME("rx116/%d", k), NOSYM);
         GENLIB_DEF_PHINS(GENLIB_NAME("rx416/%d", k));
         GENLIB_PLACE_RIGHT("grmrick_c", GENLIB_NAME("rc116/%d", k), NOSYM);
         GENLIB_PLACE_RIGHT("grmrw0_c", GENLIB_NAME("rw0/%d", k), NOSYM);
         GENLIB_PLACE_RIGHT("grmrw1_c", GENLIB_NAME("rw1/%d", k), NOSYM);
         GENLIB_PLACE_RIGHT("grmrw2_c", GENLIB_NAME("rw2/%d", k), NOSYM);
         GENLIB_PLACE_RIGHT("grmrw3_c", GENLIB_NAME("rw3/%d", k), NOSYM);
         if (blk == 1)
            GENLIB_PLACE_RIGHT("grmbs_c", GENLIB_NAME("rbs/%d", k), NOSYM);
         else
            GENLIB_PLACE_RIGHT(GENLIB_NAME("grrbs%ld_c", lnblk), GENLIB_NAME("rbs/%d", k), NOSYM);
         for (i = 0; i < 8; i++) {
            if (!i)
               GENLIB_DEF_PHINS(GENLIB_NAME("rx116/%d", k));
            else 
               GENLIB_DEF_PHINS(GENLIB_NAME("rbu16/%d/%d", k, i - 1));
            GENLIB_PLACE_TOP("grmrwb_c", GENLIB_NAME("rbu16/%d/%d", k, i), NOSYM);
            GENLIB_PLACE_RIGHT("grmrl_c", GENLIB_NAME("rl/%d/%d", k, i), NOSYM);
            GENLIB_PLACE_RIGHT("grmrs_c", GENLIB_NAME("rs/%d/%d", k, i), NOSYM);
         }
         GENLIB_DEF_PHINS(GENLIB_NAME("rbu16/%d/%d", k, i - 1));
         GENLIB_PLACE_TOP("grmrck_c", GENLIB_NAME("rck/%d", k), NOSYM);
      } else { /* odd blocks */
         /* matrix */
         for (j = 0; j < nb; j ++) { /* for each bit */
            GENLIB_PLACE_RIGHT("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j), SYM_Y);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j), SYM_Y);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j), SYM_Y);
            GENLIB_PLACE_TOP("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j), SYM_Y);
            GENLIB_PLACE_TOP("grrmx_c", GENLIB_NAME("rmx14/%d/%d", k, j), SYM_Y);
   
            GENLIB_DEF_PHINS(GENLIB_NAME("rp4/%d/%d", k, 4 * j));
            GENLIB_PLACE_RIGHT("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j + 1), SYMXY);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j + 1), SYMXY);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j + 1), SYMXY);
            GENLIB_PLACE_TOP("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 1), SYMXY);
   
            GENLIB_DEF_PHINS(GENLIB_NAME("rp4/%d/%d", k, 4 * j + 1));
            GENLIB_PLACE_RIGHT("grmrst_c", GENLIB_NAME("rp1/%d/%d", k, j), SYM_Y);
            GENLIB_PLACE_RIGHT("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j + 2), SYM_Y);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j + 2), SYM_Y);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j + 2), SYM_Y);
            GENLIB_PLACE_TOP("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 2), SYM_Y);
   
            GENLIB_DEF_PHINS(GENLIB_NAME("rp4/%d/%d", k, 4 * j + 2));
            GENLIB_PLACE_RIGHT("grp4_c", GENLIB_NAME("rp4/%d/%d", k, 4 * j + 3), SYMXY);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/1/%d", k, 4 * j + 3), SYMXY);
            GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/%d/0/%d", k, 4 * j + 3), SYMXY);
            GENLIB_PLACE_TOP("grmx4_c", GENLIB_NAME("rmx04/%d/%d", k, 4 * j + 3), SYMXY);

            GENLIB_DEF_PHINS(GENLIB_NAME("rp4/%d/%d", k, 4 * j + 3));
            GENLIB_PLACE_RIGHT("grrmo_c", GENLIB_NAME("rout/%d/%d", k, j), SYM_Y);
         
            if (tr)
               GENLIB_PLACE_RIGHT("grrmt_c", GENLIB_NAME("rmt/%d/%d", k, j), SYM_Y);
         }
         GENLIB_PLACE_RIGHT("grmrck_c", GENLIB_NAME("rck/%d", k), SYM_Y);
         for (i = 0; i < 8; i++) {
            if (i)
               GENLIB_DEF_PHINS(GENLIB_NAME("rbu16/%d/%d", k, 7 - i + 1));
            GENLIB_PLACE_TOP("grmrwb_c", GENLIB_NAME("rbu16/%d/%d", k, 7 - i), SYM_Y);
            GENLIB_PLACE_RIGHT("grmrl_c", GENLIB_NAME("rl/%d/%d", k, 7 - i), SYM_Y);
            GENLIB_PLACE_RIGHT("grmrs_c", GENLIB_NAME("rs/%d/%d", k, 7 - i), SYM_Y);
         }
         GENLIB_DEF_PHINS(GENLIB_NAME("rbu16/%d/%d", k, 7 - i + 1));
         GENLIB_PLACE_TOP("grmrx0_c", GENLIB_NAME("rx116/%d", k), SYM_Y);
         GENLIB_PLACE_TOP("grmrx1_c", GENLIB_NAME("rx216/%d", k), SYM_Y);
         GENLIB_PLACE_TOP("grmrx2_c", GENLIB_NAME("rx316/%d", k), SYM_Y);
         GENLIB_PLACE_TOP("grmrx3_c", GENLIB_NAME("rx416/%d", k), SYM_Y);
         GENLIB_DEF_PHINS(GENLIB_NAME("rx116/%d", k));
         GENLIB_PLACE_RIGHT("grmrick_c", GENLIB_NAME("rc116/%d", k), SYM_Y);
         GENLIB_PLACE_RIGHT("grmrw0_c", GENLIB_NAME("rw0/%d", k), SYM_Y);
         GENLIB_PLACE_RIGHT("grmrw1_c", GENLIB_NAME("rw1/%d", k), SYM_Y);
         GENLIB_PLACE_RIGHT("grmrw2_c", GENLIB_NAME("rw2/%d", k), SYM_Y);
         GENLIB_PLACE_RIGHT("grmrw3_c", GENLIB_NAME("rw3/%d", k), SYM_Y);
         GENLIB_PLACE_RIGHT(GENLIB_NAME("grrbs%ld_c", lnblk), GENLIB_NAME("rbs/%d", k), SYM_Y);
      }
   }
   GENLIB_DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      GENLIB_COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      GENLIB_COPY_UP_CON(0L, "i", GENLIB_NAME("rli/%d", i), GENLIB_NAME("adr[%d]", i));
      GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rli/%d", i), NULL);
   }
   /* decoding :
	   draw crossing lines and appropriate vias for address decod.
		The complement of the address sould not appear on the interface,
	   so it is copied up with the name NULL.
		It they are to be copied up, just change the NULL with the other part
	   of the expression with "adrb" and not "adr". */
   j = blk == 1 ? 0 : ln2p(blk);
   for (k = 0; k < blk; k++) {
      for (i = 0; i < 8; i++) {
         for (l = 0; l < 3; l++) {
            refname = (i & (1 << l)) ? GENLIB_NAME("adr[%d]", j + 2 - l) : NULL;
            GENLIB_COPY_UP_CON(0L, GENLIB_NAME("e%d", 3 - l), GENLIB_NAME("rs/%d/%d", k, i),
									refname);
            GENLIB_PHVIA(CONT_VIA,
                  GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", j + 2 - l),
                                (i & (1 << l)) ? "i" : "f", 0L),
                  GENLIB_GET_CON_Y(GENLIB_NAME("rs/%d/%d", k, i), GENLIB_NAME("e%d", 3 - l), 0L));
         }
         GENLIB_COPY_UP_CON(1L, "vdd", GENLIB_NAME("rs/%d/%d", k, i), "vdd");
         GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rs/%d/%d", k, i), "vss");
      }
      for (i = 0; i < lnblk; i++) {
         refname = (k & (1 << j - i - 1)) ? GENLIB_NAME("adr[%d]", i) : NULL;
         GENLIB_COPY_UP_CON(0L, GENLIB_NAME("e%d", i + 1), GENLIB_NAME("rbs/%d", k), refname);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", i),
                             (k & (1 << j - i - 1)) ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rbs/%d", k), GENLIB_NAME("e%d", i + 1), 0L));
      }

      for (i = 0; i < 4; i++) {
         GENLIB_COPY_UP_CON(i & 1, GENLIB_NAME("e%d", 12 + j - i), GENLIB_NAME("rbs/%d", k),
                     (i & 1) ? GENLIB_NAME("adr[%d]", 5 + j + i / 2 ) : NULL);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 5 + j + i / 2), i & 1 ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rbs/%d", k), GENLIB_NAME("e%d", 12 + j - i), i & 1));
      }
      for (i = 0; i < 4; i++) {
         GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", 8 + j - i), GENLIB_NAME("rbs/%d", k),
                     (i & 1) ? GENLIB_NAME("adr[%d]", 7 + j + i / 2 ) : NULL);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 7 + j + i / 2), i & 1 ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rbs/%d", k), GENLIB_NAME("e%d", 8 + j - i), 1L));
      }
      for (i = 0; i < 4; i++) {
         GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", 4 + j - i), GENLIB_NAME("rbs/%d", k),
                     (i & 1) ? GENLIB_NAME("adr[%d]", 3 + j + i / 2 ) : NULL);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 3 + j + i / 2), i & 1 ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rbs/%d", k), GENLIB_NAME("e%d", 4 + j - i), 1L));
      }

      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rbs/%d", k), "vss");
      GENLIB_COPY_UP_CON(1L, "vdd", GENLIB_NAME("rbs/%d", k), "vdd");
      GENLIB_COPY_UP_CON(3L, "vdd", GENLIB_NAME("rbs/%d", k), "vdd");
      GENLIB_COPY_UP_CON(0L, "ck", GENLIB_NAME("rck/%d", k), GENLIB_NAME("ck[%d]", k / 2));
      GENLIB_COPY_UP_CON(2L, "vdd", GENLIB_NAME("rck/%d", k), "vdd");

      for (i = 1; i <= 35; i += 2) 
         GENLIB_COPY_UP_CON(i, "vss", GENLIB_NAME("rboh/%d", k), "vss");
      GENLIB_COPY_UP_CON(0L, "vdd0", GENLIB_NAME("rboh/%d", k), "vdd");
      GENLIB_COPY_UP_CON(0L, "vdd1", GENLIB_NAME("rboh/%d", k), "vdd");
      GENLIB_COPY_UP_CON(0L, "vdd2", GENLIB_NAME("rboh/%d", k), "vdd");
   }

   if (blk & 1) {
      for (i = 0; i < 4 * nb; i++) {
        GENLIB_COPY_UP_CON(2L, "vdd", GENLIB_NAME("rp4/%d/%d", blk - 1, i), "vdd");
        GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rp4/%d/%d", blk - 1, i), "vss");
      }
      GENLIB_COPY_UP_CON(3L, "vdd", GENLIB_NAME("rck/%d", blk - 1), "vdd");
      GENLIB_COPY_UP_CON(1L, "vss0", GENLIB_NAME("rck/%d", blk - 1), "vss");
      GENLIB_COPY_UP_CON(1L, "vss1", GENLIB_NAME("rck/%d", blk - 1), "vss");
      GENLIB_COPY_UP_CON(1L, "vss2", GENLIB_NAME("rck/%d", blk - 1), "vss");
   } else {
      for (i = 0; i < nb; i++) {
        GENLIB_COPY_UP_CON(0L, "vdd0", GENLIB_NAME("rmx14/%d/%d", blk - 1, i), "vdd");
        GENLIB_COPY_UP_CON(0L, "vss", GENLIB_NAME("rmx14/%d/%d", blk - 1, i), "vss");
      }
      GENLIB_COPY_UP_CON(0L, "vdd0", GENLIB_NAME("rx416/%d", blk - 1), "vdd");
      GENLIB_COPY_UP_CON(1L, "vdd0", GENLIB_NAME("rx416/%d", blk - 1), "vdd");
      GENLIB_COPY_UP_CON(0L, "vdd1", GENLIB_NAME("rx416/%d", blk - 1), "vdd");
      GENLIB_COPY_UP_CON(0L, "vss", GENLIB_NAME("rx416/%d", blk - 1), "vss");
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rx416/%d", blk - 1), "vss");
      GENLIB_COPY_UP_CON(2L, "vss", GENLIB_NAME("rx416/%d", blk - 1), "vss");

      GENLIB_COPY_UP_CON(0L, "vdd", GENLIB_NAME("rc116/%d", blk - 1), "vdd");
      GENLIB_COPY_UP_CON(0L, "vdd", GENLIB_NAME("rw0/%d", blk - 1), "vdd");
      GENLIB_COPY_UP_CON(0L, "vss", GENLIB_NAME("rw2/%d", blk - 1), "vss");
   }

   GENLIB_COPY_UP_CON(0L, "vss", "rbol", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   GENLIB_COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss", "rli/0", "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "rzhb", "vss");
   GENLIB_COPY_UP_CON(1L, "vss", "rzhb", "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "rept", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "rept", "vss");

   /* import output connectors */
   for (i = 0; i < nb; i++)
      if (r)
         GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", nb - i - 1));
      else
         GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", i));

   /* now import the metal thru routes */
   if (tr)
      for (k = 0; k < blk; k++)
         for (i = 0; i < nb; i++)
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d/%d", k, i), GENLIB_NAME("tr_%d", i));
   for (i = 0; i < nb; i++) {
      if (tr)
         GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rot/%d", i), GENLIB_NAME("tr_%d", i));
      if (blk == 1) {
         GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rob/%d", i), GENLIB_NAME("tr_%d", i));
         GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rout/0/%d", i), GENLIB_NAME("tr_%d", i));
         GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmx14/0/%d", i), GENLIB_NAME("tr_%d", i));
      }
   }
}

static void ublock(int nw, int nb, int tr, int zh, int r)
{
long lnw; /* number of address lines */
long i, j, l;
char *refname; /* reference instance name */

   lnw = ln2p(nw);

   /* output line : body ties, output buffers, address line inverters */
   GENLIB_PLACE("grmbob_c", "rbol", NOSYM, 0, 0);
   GENLIB_PLACE_TOP("grubom_c", "rboh", NOSYM);
   GENLIB_DEF_PHINS("rbol");
   for (i = 0; i < nb; i++) {
      if (i & 1) {
         if (zh)
            GENLIB_PLACE_RIGHT(tr ? "gruoboht_c" : "gruoboh_c",
                        GENLIB_NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         else
            GENLIB_PLACE_RIGHT(tr ? "gruobot_c" : "gruobo_c",
                        GENLIB_NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         GENLIB_PLACE_TOP(tr ? "grumx2ot_c" : "grumx2o_c",
                        GENLIB_NAME("rmx2/%d", i), i & 1 ? SYM_X : NOSYM);
      } else {
         if (zh)
            GENLIB_PLACE_RIGHT(tr ? "gruobeht_c" : "gruobeh_c",
                        GENLIB_NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         else
            GENLIB_PLACE_RIGHT(tr ? "gruobet_c" : "gruobe_c",
                        GENLIB_NAME("rob/%d", i), i & 1 ? SYM_X : NOSYM);
         GENLIB_PLACE_TOP(tr ? "grumx2et_c" : "grumx2e_c",
                        GENLIB_NAME("rmx2/%d", i), i & 1 ? SYM_X : NOSYM);
      }
      GENLIB_DEF_PHINS(GENLIB_NAME("rob/%d", i));
   }
   if (zh) {
      GENLIB_PLACE_RIGHT("gruobfh_c", "bf", NOSYM);
      GENLIB_PLACE_RIGHT("gruoebh_c", "rzhb", NOSYM);
   } else {
      if (nb & 1)
         GENLIB_PLACE_RIGHT("gruobfo_c", "bf", NOSYM);
      else
         GENLIB_PLACE_RIGHT("gruobfe_c", "bf", NOSYM);
      GENLIB_PLACE_RIGHT("grufill_c", GENLIB_NAME("rzhb", i), NOSYM);
   }

   GENLIB_PLACE_RIGHT("grmfeed_c", GENLIB_NAME("feed", i), NOSYM);
   for (i = lnw - 1; i >= 0; i--)
      GENLIB_PLACE_RIGHT("grmli_c", GENLIB_NAME("rli/%d", i), NOSYM);


   /* blocks generation : transistor matrix, word and bit decoders */
   /* matrix building */
   GENLIB_DEF_PHINS("rmx2/0");
   j = 0;
   for (i = 0; i < nb; i ++) { /* for each bit */
      if (i == 0)
         GENLIB_PLACE_TOP("grmx4_c", GENLIB_NAME("rmx4/%d", j), NOSYM);
      else
         GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx4/%d", j), NOSYM);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/0/%d", j), NOSYM);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/1/%d", j), NOSYM);
      GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d", j), NOSYM);

      GENLIB_DEF_PHINS(GENLIB_NAME("rmx4/%d", j++));
      if (tr)
         GENLIB_PLACE_RIGHT("grnmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);

      GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx4/%d", j), SYM_X);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/0/%d", j), SYM_X);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/1/%d", j), SYM_X);
      GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d", j), SYM_X);

      GENLIB_DEF_PHINS(GENLIB_NAME("rmx4/%d", j++));
      if ((i & 1) && i == nb - 1)
         GENLIB_PLACE_RIGHT("grumf_c", "umf", NOSYM);
      if (!(i & 1))
         if (i != nb - 1)
         {
            GENLIB_PLACE_RIGHT("grmrst_c", GENLIB_NAME("rp1/%d", i), NOSYM);
         }
         else
         {
            GENLIB_PLACE_RIGHT("grnrste_c", GENLIB_NAME("rp1/%d", i), NOSYM);
         }
   }
   GENLIB_DEF_PHINS("rzhb");
   /* decoder building */
   GENLIB_PLACE_TOP("grurx2_c", "x2", NOSYM);
   GENLIB_PLACE_TOP("grurx1_c", "x1", NOSYM);
   GENLIB_PLACE_TOP("grprx0_c", "x0", NOSYM);
   GENLIB_DEF_PHINS("feed");
   GENLIB_PLACE_TOP("gruwi_c", "wires", NOSYM);
   GENLIB_PLACE_TOP("grpick_c", "rc116", NOSYM);
   GENLIB_PLACE_RIGHT("grprw0_c", "rw0", NOSYM);
   GENLIB_PLACE_RIGHT("grprw1_c", "rw1", NOSYM);
   GENLIB_PLACE_RIGHT("grprw2_c", "rw2", NOSYM);
   GENLIB_PLACE_RIGHT("grprw3_c", "rw3", NOSYM);
   GENLIB_PLACE_RIGHT("grnbs_c", "rbs", NOSYM);
   for (i = 0; i < 8; i++) {
      if (!i)
         GENLIB_DEF_PHINS("x0");
      else 
         GENLIB_DEF_PHINS(GENLIB_NAME("rbu/%d", i - 1));
      GENLIB_PLACE_TOP("grmrwb_c", GENLIB_NAME("rbu/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grmrl_c", GENLIB_NAME("rl/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grmrs_c", GENLIB_NAME("rs/%d", i), NOSYM);
   }
   GENLIB_DEF_PHINS(GENLIB_NAME("rbu/%d", i - 1));
   GENLIB_PLACE_TOP("grmrck_c", "rck", NOSYM);
   GENLIB_DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      GENLIB_COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      GENLIB_COPY_UP_CON(0L, "i", GENLIB_NAME("rli/%d", i), GENLIB_NAME("adr[%d]", i));
      GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rli/%d", i), NULL);
   }
   /* draw crossing lines and appropriate vias for address decod */
   for (i = 0; i < 8; i++) {
      for (l = 0; l < 3; l++) {
         refname = (i & (1 << l)) ? GENLIB_NAME("adr[%d]", 2 - l) : NULL;
         GENLIB_COPY_UP_CON(0L, GENLIB_NAME("e%d", 3 - l), GENLIB_NAME("rs/%d", i), refname);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 2 - l),
                             (i & (1 << l)) ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rs/%d", i), GENLIB_NAME("e%d", 3 - l), 0L));
      }
      GENLIB_COPY_UP_CON(1L, "vdd", GENLIB_NAME("rs/%d", i), "vdd");
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rs/%d", i), "vss");
   }
   /* rprwi decoding */
   for (i = 0; i < 4; i++) {
      GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", i + 1), "rbs",
                  (i & 1) ? GENLIB_NAME("adr[%d]", 3 + i / 2 ) : NULL);
      GENLIB_PHVIA(CONT_VIA,
            GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 3 + i / 2), i & 1 ? "i" : "f", 0L),
            GENLIB_GET_CON_Y("rbs", GENLIB_NAME("e%d", i + 1), 1L));
   }
   /* rmx04 decoding */
   for (i = 0; i < 4; i++) {
      GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", i + 5), "rbs",
                  !(i & 1) ? GENLIB_NAME("adr[%d]", 7 - i / 2 ) : NULL);
      GENLIB_PHVIA(CONT_VIA,
            GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 7 - i / 2), !(i & 1) ? "i" : "f", 0L),
            GENLIB_GET_CON_Y("rbs", GENLIB_NAME("e%d", i + 5), 1L));
   }
   /* low addresses : rmx2/n */
   for (i = 0; i < 2; i++) {
         GENLIB_COPY_UP_CON(1L, GENLIB_NAME("s%d", i), "wires",
                    (i & 1) ? "adr[5]" : NULL);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X("rli/5", (i & 1) ? "i" : "f", 0L),
               GENLIB_GET_CON_Y("wires", GENLIB_NAME("s%d", i ), 1L));
   }

   GENLIB_COPY_UP_CON(2L, "vss", "wires", "vss");
   GENLIB_COPY_UP_CON(2L, "vdd", "wires", "vdd");

   GENLIB_COPY_UP_CON(2L, "vss", "rbs", "vss");
   GENLIB_COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(2L, "vss", "rbs", "vss");
   GENLIB_COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(4L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(0L, "ck", "rbs", "ck[0]");

   GENLIB_COPY_UP_CON(0L, "ck", "rck", "ck[1]");
   GENLIB_COPY_UP_CON(2L, "vdd", "rck", "vdd");
   GENLIB_COPY_UP_CON(3L, "vdd", "rck", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss0", "rck", "vss");
   GENLIB_COPY_UP_CON(1L, "vss1", "rck", "vss");
   GENLIB_COPY_UP_CON(1L, "vss2", "rck", "vss");

   for (i = 0; i < nb; i += 2 - tr) 
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rp4/%d", i), "vss");

   GENLIB_COPY_UP_CON(0L, "vss", "rbol", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   for (i = 1; i <= 35; i += 2) 
      GENLIB_COPY_UP_CON(i, "vss", "rboh", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd0", "rboh", "vdd");
   GENLIB_COPY_UP_CON(0L, "vdd1", "rboh", "vdd");
   GENLIB_COPY_UP_CON(0L, "vdd2", "rboh", "vdd");

   GENLIB_COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "rzhb", "vss");
   GENLIB_COPY_UP_CON(1L, "vss", "rzhb", "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "feed", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "feed", "vss");

   GENLIB_COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss", "rli/0", "vss");

   /* import output connectors */
   for (i = 0; i < nb; i++)
      if (r)
         GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", nb - i - 1));
      else
         GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", i));

      /* through routes now */
      if (tr)
         for (i = 0; i < nb; i++) {
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rob/%d", i), GENLIB_NAME("tr_%d", i));
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmx2/%d", i), GENLIB_NAME("tr_%d", i));
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i));
         }
}

static void nblock(int nw, int nb, int tr, int zh, int r)
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
      GENLIB_PLACE("grpubobh_c", "rbol", NOSYM, 0, 0);
      if (nb & 1)
         GENLIB_PLACE_RIGHT("grpobhs_c", "robert", NOSYM);
      for (i = 0; i < nb / 2; i++) {
         if (tr)
            GENLIB_PLACE_RIGHT("grpobhtc_c", GENLIB_NAME("rob/%d", i), NOSYM);
         else
            GENLIB_PLACE_RIGHT("grpobhc_c", GENLIB_NAME("rob/%d", i), NOSYM);
      }
      GENLIB_PLACE_RIGHT("grroebh_c", GENLIB_NAME("rzhb", i), NOSYM);
      GENLIB_PLACE_RIGHT("grpfeedh_c", GENLIB_NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         GENLIB_PLACE_RIGHT("grpli_c", GENLIB_NAME("rli/%d", i), NOSYM);
   } else {
      GENLIB_PLACE("grpubob_c", "rbol", NOSYM, 0, 0);
      for (i = 0; i < nb; i++) {
         GENLIB_PLACE_RIGHT("grpob_c", GENLIB_NAME("rob/%d", i), i & 1 ? sym_x : nosym);
         if (i != nb - 1 && (!(nb & 1)) && (i & 1))
            GENLIB_PLACE_RIGHT("grpf_c", GENLIB_NAME("mrpl/%d", i), NOSYM);
         else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
            GENLIB_PLACE_RIGHT("grpf_c", GENLIB_NAME("mrpl/%d", i), NOSYM);
         if (tr && (nb & 1) && (i & 1))
            GENLIB_PLACE_RIGHT("grpubt_c", GENLIB_NAME("rot/%d", i), NOSYM);
         if (tr && !(nb & 1) && !(i & 1))
            GENLIB_PLACE_RIGHT("grpubt_c", GENLIB_NAME("rot/%d", i), NOSYM);
      }
      GENLIB_PLACE_RIGHT("grpf_c", GENLIB_NAME("rpf/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grpfill_c", GENLIB_NAME("rzhb", i), NOSYM);
      GENLIB_PLACE_RIGHT("grpfeed_c", GENLIB_NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         GENLIB_PLACE_RIGHT("grpli_c", GENLIB_NAME("rli/%d", i), NOSYM);
   }

   /* blocks generation : transistor matrix, word and bit decoders */
   GENLIB_DEF_PHINS("rbol");
   GENLIB_PLACE_TOP("grnbom_c", "rboh", NOSYM);

   /* matrix building */
   for (i = 0; i < nb; i ++) { /* for each bit */
      GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx4/%d", i), i & 1 ? sym_x : nosym);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/0/%d", i), i & 1 ? sym_x : nosym);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/1/%d", i), i & 1 ? sym_x : nosym);
      GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d", i), i & 1 ? sym_x : nosym);

      GENLIB_DEF_PHINS(GENLIB_NAME("rmx4/%d", i));
      if (zh && i == nb - 1)
         GENLIB_PLACE_RIGHT("grnrste_c", GENLIB_NAME("rp1/%d", i), NOSYM);
      if (i != nb - 1 && (!(nb & 1)) && (i & 1))
         GENLIB_PLACE_RIGHT("grmrst_c", GENLIB_NAME("rp1/%d", i), NOSYM);
      else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
         GENLIB_PLACE_RIGHT("grmrst_c", GENLIB_NAME("rp1/%d", i), NOSYM);

      if (zh && tr && (i & 1) && (nb & 1))
         GENLIB_PLACE_RIGHT("grnmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
      else if (zh && tr && !(i & 1) && !(nb & 1))
         GENLIB_PLACE_RIGHT("grnmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && (i & 1) && (nb & 1))
         GENLIB_PLACE_RIGHT("grnmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && !(nb & 1) && !(i & 1))
         GENLIB_PLACE_RIGHT("grnmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
   }
   if (!zh)
      GENLIB_PLACE_RIGHT("grnrste_c", GENLIB_NAME("rp1/%d", i), NOSYM);
   /* decoder building */
   GENLIB_PLACE_RIGHT("grprx1_c", "x1", NOSYM);
   GENLIB_PLACE_TOP("grprx0_c", "x0", NOSYM);
   GENLIB_DEF_PHINS("feed");
   GENLIB_PLACE_TOP("grpick_c", "rc116", NOSYM);
   GENLIB_PLACE_RIGHT("grprw0_c", "rw0", NOSYM);
   GENLIB_PLACE_RIGHT("grprw1_c", "rw1", NOSYM);
   GENLIB_PLACE_RIGHT("grprw2_c", "rw2", NOSYM);
   GENLIB_PLACE_RIGHT("grprw3_c", "rw3", NOSYM);
   GENLIB_PLACE_RIGHT("grnbs_c", "rbs", NOSYM);
   for (i = 0; i < 8; i++) {
      if (!i)
         GENLIB_DEF_PHINS("x0");
      else 
         GENLIB_DEF_PHINS(GENLIB_NAME("rbu/%d", i - 1));
      GENLIB_PLACE_TOP("grmrwb_c", GENLIB_NAME("rbu/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grmrl_c", GENLIB_NAME("rl/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grmrs_c", GENLIB_NAME("rs/%d", i), NOSYM);
   }
   GENLIB_DEF_PHINS(GENLIB_NAME("rbu/%d", i - 1));
   GENLIB_PLACE_TOP("grmrck_c", "rck", NOSYM);
   GENLIB_DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      GENLIB_COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      GENLIB_COPY_UP_CON(0L, "i", GENLIB_NAME("rli/%d", i), GENLIB_NAME("adr[%d]", i));
      GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rli/%d", i), NULL);
   }
   /* draw crossing lines and appropriate vias for address decod */
   for (i = 0; i < 8; i++) {
      for (l = 0; l < 3; l++) {
         refname = (i & (1 << l)) ? GENLIB_NAME("adr[%d]", 2 - l) : NULL;
         GENLIB_COPY_UP_CON(0L, GENLIB_NAME("e%d", 3 - l), GENLIB_NAME("rs/%d", i), refname);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 2 - l),
                             (i & (1 << l)) ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rs/%d", i), GENLIB_NAME("e%d", 3 - l), 0L));
      }
      GENLIB_COPY_UP_CON(1L, "vdd", GENLIB_NAME("rs/%d", i), "vdd");
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rs/%d", i), "vss");
   }
   /* rprwi decoding */
   for (i = 0; i < 4; i++) {
      GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", i + 1), "rbs",
                  (i & 1) ? GENLIB_NAME("adr[%d]", 3 + i / 2 ) : NULL);
      GENLIB_PHVIA(CONT_VIA,
            GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 3 + i / 2), i & 1 ? "i" : "f", 0L),
            GENLIB_GET_CON_Y("rbs", GENLIB_NAME("e%d", i + 1), 1L));
   }
   /* rmx04 decoding */
   for (i = 0; i < 4; i++) {
      GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", i + 5), "rbs",
                             !(i & 1) ? GENLIB_NAME("adr[%d]", 6 - i / 2 ) : NULL);
      GENLIB_PHVIA(CONT_VIA,
            GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 6 - i / 2), !(i & 1) ? "i" : "f", 0L),
            GENLIB_GET_CON_Y("rbs", GENLIB_NAME("e%d", i + 5), 1L));
   }

   /* copy power supplies and clocks */
   GENLIB_COPY_UP_CON(2L, "vss", "rbs", "vss");
   GENLIB_COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(4L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(0L, "ck", "rbs", "ck[0]");

   GENLIB_COPY_UP_CON(0L, "ck", "rck", "ck[1]");
   GENLIB_COPY_UP_CON(3L, "vdd", "rck", "vdd");
   GENLIB_COPY_UP_CON(2L, "vdd", "rck", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss0", "rck", "vss");
   GENLIB_COPY_UP_CON(1L, "vss1", "rck", "vss");
   GENLIB_COPY_UP_CON(1L, "vss2", "rck", "vss");

   GENLIB_COPY_UP_CON(0L, "vss", "rbol", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   for (i = 1; i <= 35; i += 2) 
      GENLIB_COPY_UP_CON(i, "vss", "rboh", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd0", "rboh", "vdd");
   GENLIB_COPY_UP_CON(0L, "vdd1", "rboh", "vdd");
   GENLIB_COPY_UP_CON(0L, "vdd", "rboh", "vdd");

   for (i = 0; i < nb; i += 2 - tr) 
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rp4/%d", i), "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "rzhb", "vss");
   GENLIB_COPY_UP_CON(1L, "vss", "rzhb", "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "feed", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "feed", "vss");

   GENLIB_COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss", "rli/0", "vss");


   /* import output connectors */
   if (zh) {
      if (r) {
         j = nb - 1;
         if (nb & 1)
            GENLIB_COPY_UP_CON(0L, "f1", "robert", GENLIB_NAME("f[%d]", j--));
         for (i = 0; i < nb / 2; i++) {
            GENLIB_COPY_UP_CON(0L, "f0", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j--));
            GENLIB_COPY_UP_CON(0L, "f1", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j--));
         }
      } else {
         j = 0;
         if (nb & 1)
            GENLIB_COPY_UP_CON(0L, "f1", "robert", GENLIB_NAME("f[%d]", j++));
         for (i = 0; i < nb / 2; i++) {
            GENLIB_COPY_UP_CON(0L, "f0", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j++));
            GENLIB_COPY_UP_CON(0L, "f1", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j++));
         }
      }
   } else
      for (i = 0; i < nb; i++)
         if (r)
            GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", nb - i - 1));
         else
            GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", i));

   /* metal one feed through */
   if (tr) {
      for (i = 0; i < nb; i++) {
         if (zh && (i & 1) && (nb & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
         else if (zh && !(i & 1) && !(nb & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
         else if (!zh && (i & 1) && (nb & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
         else if (!zh && !(nb & 1) && !(i & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
      }
      if (zh)
         for (i = 0; i < nb / 2; i++)
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rob/%d", i), GENLIB_NAME("tr_%d", i));
      else
         for (i = 0; i < nb; i++) {
            if (((nb & 1) && (i & 1)) || (!(nb & 1) && !(i & 1)))
               GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rot/%d", i), GENLIB_NAME("tr_%d", i / 2));
         }
   }
}

static void pblock(int nw, int nb, int tr, int zh, int r)
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
      GENLIB_PLACE("grpubobh_c", "rbol", NOSYM, 0, 0);
      if (nb & 1)
         GENLIB_PLACE_RIGHT("grpobhs_c", "robert", NOSYM);
      for (i = 0; i < nb / 2; i++) {
         if (tr)
            GENLIB_PLACE_RIGHT("grpobhtc_c", GENLIB_NAME("rob/%d", i), NOSYM);
         else
            GENLIB_PLACE_RIGHT("grpobhc_c", GENLIB_NAME("rob/%d", i), NOSYM);
      }
      GENLIB_PLACE_RIGHT("grroebh_c", GENLIB_NAME("rzhb", i), NOSYM);
      GENLIB_PLACE_RIGHT("grpfeedh_c", GENLIB_NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         GENLIB_PLACE_RIGHT("grpli_c", GENLIB_NAME("rli/%d", i), NOSYM);
   } else {
      GENLIB_PLACE("grpubob_c", "rbol", NOSYM, 0, 0);
      for (i = 0; i < nb; i++) {
         GENLIB_PLACE_RIGHT("grpob_c", GENLIB_NAME("rob/%d", i), i & 1 ? sym_x : nosym);
         if (i != nb - 1 && (!(nb & 1)) && (i & 1))
            GENLIB_PLACE_RIGHT("grpf_c", GENLIB_NAME("mrpl/%d", i), NOSYM);
         else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
            GENLIB_PLACE_RIGHT("grpf_c", GENLIB_NAME("mrpl/%d", i), NOSYM);
         if (tr && (nb & 1) && (i & 1))
            GENLIB_PLACE_RIGHT("grpubt_c", GENLIB_NAME("rot/%d", i), NOSYM);
         if (tr && !(nb & 1) && !(i & 1))
            GENLIB_PLACE_RIGHT("grpubt_c", GENLIB_NAME("rot/%d", i), NOSYM);
      }
      GENLIB_PLACE_RIGHT("grpf_c", GENLIB_NAME("rpf/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grpfill_c", GENLIB_NAME("rzhb", i), NOSYM);
      GENLIB_PLACE_RIGHT("grpfeed_c", GENLIB_NAME("feed", i), NOSYM);
      for (i = lnw - 1; i >= 0; i--)
         GENLIB_PLACE_RIGHT("grpli_c", GENLIB_NAME("rli/%d", i), NOSYM);
   }

   /* blocks generation : transistor matrix, word and bit decoders */
   GENLIB_DEF_PHINS("rbol");
   GENLIB_PLACE_TOP("grpbom_c", "rboh", NOSYM);

   /* matrix building */
   for (i = 0; i < nb; i ++) { /* for each bit */
      GENLIB_PLACE_RIGHT("grmx4_c", GENLIB_NAME("rmx4/%d", i), i & 1 ? sym_x : nosym);
      GENLIB_PLACE_TOP("grbl4_c", GENLIB_NAME("rbl4/0/%d", i), i & 1 ? sym_x : nosym);
      GENLIB_PLACE_TOP("grp4_c", GENLIB_NAME("rp4/%d", i), i & 1 ? sym_x : nosym);

      GENLIB_DEF_PHINS(GENLIB_NAME("rmx4/%d", i));
      if (zh && i == nb - 1)
         GENLIB_PLACE_RIGHT("grprste_c", GENLIB_NAME("rp1/%d", i), NOSYM);
      if (i != nb - 1 && (!(nb & 1)) && (i & 1))
         GENLIB_PLACE_RIGHT("grprst_c", GENLIB_NAME("rp1/%d", i), NOSYM);
      else if (i != nb - 1 && (nb & 1) && (!(i & 1)))
         GENLIB_PLACE_RIGHT("grprst_c", GENLIB_NAME("rp1/%d", i), NOSYM);

      if (zh && tr && (i & 1) && (nb & 1))
         GENLIB_PLACE_RIGHT("grpmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
      else if (zh && tr && !(i & 1) && !(nb & 1))
         GENLIB_PLACE_RIGHT("grpmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && (i & 1) && (nb & 1))
         GENLIB_PLACE_RIGHT("grpmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
      else if (!zh && tr && !(nb & 1) && !(i & 1))
         GENLIB_PLACE_RIGHT("grpmht_c", GENLIB_NAME("rmt/%d", i), NOSYM);
   }
   if (!zh)
      GENLIB_PLACE_RIGHT("grprste_c", GENLIB_NAME("rp1/%d", i), NOSYM);
   /* decoder building */
   GENLIB_PLACE_RIGHT("grprx1_c", "x1", NOSYM);
   GENLIB_PLACE_TOP("grprx0_c", "x0", NOSYM);
   GENLIB_DEF_PHINS("feed");
   GENLIB_PLACE_TOP("grpick_c", "rc116", NOSYM);
   GENLIB_PLACE_RIGHT("grprw0_c", "rw0", NOSYM);
   GENLIB_PLACE_RIGHT("grprw1_c", "rw1", NOSYM);
   GENLIB_PLACE_RIGHT("grprw2_c", "rw2", NOSYM);
   GENLIB_PLACE_RIGHT("grprw3_c", "rw3", NOSYM);
   GENLIB_PLACE_RIGHT("grpbs_c", "rbs", NOSYM);
   for (i = 0; i < 4; i++) {
      if (!i)
         GENLIB_DEF_PHINS("x0");
      else 
         GENLIB_DEF_PHINS(GENLIB_NAME("rbu/%d", i - 1));
      GENLIB_PLACE_TOP("grmrwb_c", GENLIB_NAME("rbu/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grmrl_c", GENLIB_NAME("rl/%d", i), NOSYM);
      GENLIB_PLACE_RIGHT("grprs_c", GENLIB_NAME("rs/%d", i), NOSYM);
   }
   GENLIB_DEF_PHINS(GENLIB_NAME("rbu/%d", i - 1));
   GENLIB_PLACE_TOP("grmrck_c", "rck", NOSYM);
   GENLIB_DEF_AB(0L, 0L, 0L, 0L);
   /* output zh enable connector if needed */
   if (zh)
      GENLIB_COPY_UP_CON(0L, "i", "rzhb", "oe");
   /* draw address lines */
   for (i = lnw - 1; i >= 0; i--) {
      GENLIB_COPY_UP_CON(0L, "i", GENLIB_NAME("rli/%d", i), GENLIB_NAME("adr[%d]", i));
      GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rli/%d", i), NULL);
   }
   /* draw crossing lines and appropriate vias for address decod */
   for (i = 0; i < 4; i++) {
      for (l = 0; l < 2; l++) {
         refname = (i & (1 << l)) ? GENLIB_NAME("adr[%d]", 1 - l) : NULL;
         GENLIB_COPY_UP_CON(0L, GENLIB_NAME("e%d", l + 1), GENLIB_NAME("rs/%d", i), refname);
         GENLIB_PHVIA(CONT_VIA,
               GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 1 - l),
                             (i & (1 << l)) ? "i" : "f", 0L),
               GENLIB_GET_CON_Y(GENLIB_NAME("rs/%d", i), GENLIB_NAME("e%d", l + 1), 0L));
      }
      GENLIB_COPY_UP_CON(1L, "vdd", GENLIB_NAME("rs/%d", i), "vdd");
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rs/%d", i), "vss");
   }
   /* rprwi decoding */
   for (i = 0; i < 4; i++) {
      GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", i + 1), "rbs",
                             (i & 1) ? GENLIB_NAME("adr[%d]", 2 + i / 2 ) : NULL);
      GENLIB_PHVIA(CONT_VIA,
            GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 2 + i / 2), i & 1 ? "i" : "f", 0L),
            GENLIB_GET_CON_Y("rbs", GENLIB_NAME("e%d", i + 1), 1L));
   }
   /* rmx04 decoding */
   for (i = 0; i < 4; i++) {
      GENLIB_COPY_UP_CON(1L, GENLIB_NAME("e%d", i + 5), "rbs",
                             !(i & 1) ? GENLIB_NAME("adr[%d]", 5 - i / 2 ) : NULL);
      GENLIB_PHVIA(CONT_VIA,
            GENLIB_GET_CON_X(GENLIB_NAME("rli/%d", 5 - i / 2), !(i & 1) ? "i" : "f", 0L),
            GENLIB_GET_CON_Y("rbs", GENLIB_NAME("e%d", i + 5), 1L));
   }

   /* copy power supplies and clocks */
   GENLIB_COPY_UP_CON(0L, "vss", "rbol", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd", "rbol", "vdd");

   for (i = 1; i <= 19; i += 2) 
      GENLIB_COPY_UP_CON(i, "vss", "rboh", "vss");
   GENLIB_COPY_UP_CON(0L, "vdd0", "rboh", "vdd");
   GENLIB_COPY_UP_CON(0L, "vdd1", "rboh", "vdd");
   GENLIB_COPY_UP_CON(0L, "vdd", "rboh", "vdd");

   for (i = 0; i < nb; i += 2 - tr) 
      GENLIB_COPY_UP_CON(1L, "vss", GENLIB_NAME("rp4/%d", i), "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "rzhb", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "rzhb", "vss");
   GENLIB_COPY_UP_CON(1L, "vss", "rzhb", "vss");

   GENLIB_COPY_UP_CON(0L, "vdd", "feed", "vdd");
   GENLIB_COPY_UP_CON(0L, "vss", "feed", "vss");

   GENLIB_COPY_UP_CON(1L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(3L, "vdd", "rli/0", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss", "rli/0", "vss");

   GENLIB_COPY_UP_CON(1L, "vss", "rbs", "vss");
   GENLIB_COPY_UP_CON(1L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(3L, "vdd", "rbs", "vdd");
   GENLIB_COPY_UP_CON(0L, "ck", "rbs", "ck");

   GENLIB_COPY_UP_CON(0L, "ck", "rck", "ck");
   GENLIB_COPY_UP_CON(3L, "vdd", "rck", "vdd");
   GENLIB_COPY_UP_CON(2L, "vdd", "rck", "vdd");
   GENLIB_COPY_UP_CON(1L, "vss0", "rck", "vss");
   GENLIB_COPY_UP_CON(1L, "vss1", "rck", "vss");
   GENLIB_COPY_UP_CON(1L, "vss2", "rck", "vss");

   /* import output connectors */
   if (zh) {
      if (r) {
         j = nb - 1;
         if (nb & 1)
            GENLIB_COPY_UP_CON(0L, "f1", "robert", GENLIB_NAME("f[%d]", j--));
         for (i = 0; i < nb / 2; i++) {
            GENLIB_COPY_UP_CON(0L, "f0", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j--));
            GENLIB_COPY_UP_CON(0L, "f1", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j--));
         }
      } else {
         j = 0;
         if (nb & 1)
            GENLIB_COPY_UP_CON(0L, "f1", "robert", GENLIB_NAME("f[%d]", j++));
         for (i = 0; i < nb / 2; i++) {
            GENLIB_COPY_UP_CON(0L, "f0", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j++));
            GENLIB_COPY_UP_CON(0L, "f1", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", j++));
         }
      }
   } else
      for (i = 0; i < nb; i++)
         if (r)
            GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", nb - i - 1));
         else
            GENLIB_COPY_UP_CON(0L, "f", GENLIB_NAME("rob/%d", i), GENLIB_NAME("f[%d]", i));

   /* metal one feed through */
   if (tr) {
      for (i = 0; i < nb; i++) {
         if (zh && (i & 1) && (nb & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
         else if (zh && !(i & 1) && !(nb & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
         else if (!zh && (i & 1) && (nb & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
         else if (!zh && !(nb & 1) && !(i & 1))
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rmt/%d", i), GENLIB_NAME("tr_%d", i / 2)); 
      }
      if (zh)
         for (i = 0; i < nb / 2; i++)
            GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rob/%d", i), GENLIB_NAME("tr_%d", i));
      else
         for (i = 0; i < nb; i++) {
            if (((nb & 1) && (i & 1)) || (!(nb & 1) && !(i & 1)))
               GENLIB_COPY_UP_SEG("tr", GENLIB_NAME("rot/%d", i), GENLIB_NAME("tr_%d", i / 2));
         }
   }
}

/* value of the nth bit of x */
#define bit(n,x) \
        (n < 0 ? 0 : n < 32 ? (x.low >> (n)) & 1 : (x.high >> (n - 32)) & 1)

/*
* rom data coding
*/
static void poke(int nw, int nb, biglong *data)
{
int a, b, wl, i, j, k;

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
            GENLIB_PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

static void upoke(int nw, int nb, biglong *data)
{
int i, j, k;

   for (i = 0; i < nw; i++) {
      for (j = 0; j < nb; j++) {
         k = i / 128;  /* lower or upper bit lines rows */
         (void)sprintf(instance, "rbl4/%d/%d", k, 2 * (nb - j) - (i % 8) / 4 - 1);
         (void)sprintf(reference, "fuse/%d/%d", 15 - (i % 128) / 8, 3 - i % 4);
         if (bit(j, data[i]))
            GENLIB_PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

static void npoke(int nw, int nb, int zh, biglong *data)
{
int i, j, k;
int value = zh ? 1 : 0; /* poke depends on output buffer */

   for (i = 0; i < nw; i++) {
      for (j = 0; j < nb; j++) {
         k = i / 64;  /* lower or upper bit lines rows */
         (void)sprintf(instance, "rbl4/%d/%d", k, nb - 1 - j);
         (void)sprintf(reference, "fuse/%d/%d", 15 - (i % 64) / 4, 3 - i % 4);
         if (bit(j, data[i]) == value)
            GENLIB_PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

static void ppoke(int nw, int nb, int zh, biglong *data)
{
int i, j;
int value = zh ? 1 : 0; /* poke depends on output buffer */

   for (i = 0; i < nw; i++) {
      for (j = 0; j < nb; j++) {
         (void)sprintf(instance, "rbl4/0/%d", nb - 1 - j);
         (void)sprintf(reference, "fuse/%d/%d", 15 - i / 4, 3 - i % 4);
         if (bit(j, data[i]) == value)
            GENLIB_PLACE_VIA_REF(instance, reference, CONT_DIF_N); 
      }
   }
}

void groglayout(char *name, int nb, int nw, int tr, int zh, int r, int msb, int nc, int co, biglong *data, int save)
{
   GENLIB_DEF_PHFIG(name);
   switch (nw) {
      case 64 :
         pblock(nw, nb, tr, zh, r);
         if (save && !nc)
            ppoke(nw, nb, zh, data);
         break;
      case 128 :
         nblock(nw, nb, tr, zh, r);
         if (save && !nc)
            npoke(nw, nb, zh, data);
         break;
      case 256 :
         ublock(nw, nb, tr, zh, r);
         if (save && !nc)
            upoke(nw, nb, data);
         break;
      default :
         block(nw, nb, tr, zh, r);
         if (save && !nc)
            poke(nw, nb, data);
   }
	if (!msb)
        {
		GENLIB_REVERSE_PHCON("adr");
        }

   if (co)
      grogcontdifnonly(name);

   if (save)
      GENLIB_SAVE_PHFIG();
}
