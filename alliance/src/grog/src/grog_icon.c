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

#include IAC_H
#include ICN_H
#include ICU_H
#include "genlib.h"
#include "grog.h"

/* some defines :
   for normalisation purposes
   to ease the description */
/* to use the usual system of coordinates */
#define ICONX  30
#define ICONY -30
/* to make homogeneous connector placement */
#define CON_NORTH(d) SAX_NORTH, d, SAX_CENTER, SAX_UP,     SAX_HORIZONTAL
#define CON_SOUTH(d) SAX_SOUTH, d, SAX_CENTER, SAX_DOWN,   SAX_HORIZONTAL
#define CON_WEST(d)  SAX_WEST,  d, SAX_LEFT,   SAX_CENTER, SAX_HORIZONTAL
#define CON_EAST(d)  SAX_EAST,  d, SAX_RIGHT,  SAX_CENTER, SAX_HORIZONTAL
/* globals for the use of the macros */
IconGate_list *icon; 
IconComp_list *comp = NULL;
#define ADDICONGATE(name, xm, ym, xi, yi, xl, yl)                    \
        (icon = addicongate(name, (xm) * ICONX, (ym) * ICONY,        \
                             SAX_CENTER, SAX_CENTER, SAX_HORIZONTAL, \
                             (xi) * ICONX, (yi) * ICONY,             \
                             SAX_CENTER, SAX_CENTER, SAX_HORIZONTAL, \
                             (xl) * ICONX, (yl) * ICONY,             \
                             SAX_CENTER, SAX_CENTER, SAX_HORIZONTAL))
#define ADDICONCON(name, xc, yc, face, dir) \
        do {                           \
            switch (face) { \
               case SAX_NORTH: \
                  addiconcon(icon, name, (xc) * ICONX, (yc) * ICONY, \
                             CON_NORTH(dir), \
                             (xc) * ICONX, ((yc) - 22) * ICONY); \
                  break; \
               case SAX_SOUTH: \
                  addiconcon(icon, name, (xc) * ICONX, (yc) * ICONY, \
                             CON_SOUTH(dir), \
                             (xc) * ICONX, ((yc) + 22) * ICONY); \
                  break; \
               case SAX_EAST: \
                  addiconcon(icon, name, (xc) * ICONX, (yc) * ICONY, \
                             CON_EAST(dir), \
                             ((xc) - 22) * ICONX, (yc) * ICONY); \
                  break; \
               case SAX_WEST: \
                  addiconcon(icon, name, (xc) * ICONX, (yc) * ICONY, \
                             CON_WEST(dir), \
                             ((xc) + 22) * ICONX, (yc) * ICONY); \
                  break; \
            } \
         } while (0)
#define ADDICONLINE(x0, y0, x1, y1) \
        addiconline(icon, (x0) * ICONX, (y0) * ICONY, \
                    (x1) * ICONX, (y1) * ICONY)
#define ADDICONSHAPE() do { \
                          addiconshape(icon, comp); \
                          comp = NULL; \
                       } while (0)
#define ADDICONCOMP(type, x, y) \
        (comp = addiconcomp(comp, type, (x) * ICONX, (y) * ICONY))
#define SAVEICON() do { \
                      mbkenv(); \
                      update_icon(icon); \
                      saveicon(icon); \
                   } while(0)

void grogicon(char *name, int nb, int nw, int zh, int r, int msb)
{
long lnw = ln2p(nw);  /* number of address lines */
long d = zh ? 20 : 0; /* delta to be applied */

   ADDICONGATE(name, 50 + d / 2, 25, 50 + d / 2, 15, 50 + d / 2, 20);
   if (!msb)
      ADDICONCON(GENLIB_NAME("adr[%d:0]", lnw - 1), 0, 20, SAX_WEST, IN);
   else
      ADDICONCON(GENLIB_NAME("adr[0:%d]", lnw - 1), 0, 20, SAX_WEST, IN);
   if (nw == 64)
      ADDICONCON("ck", 40, 60, SAX_NORTH, IN);
   else if (nw == 128 || nw == 256)
      ADDICONCON("ck[0:1]", 40, 60, SAX_NORTH, IN);
   else
      ADDICONCON(GENLIB_NAME("ck[0:%d]", nw / 1025), 40, 60, SAX_NORTH, IN);
   if (zh) {
      ADDICONCON("oe", 80, 60, SAX_NORTH, IN);
      ADDICONLINE(80, 40, 80, 60);
   }
   if (!r)
      ADDICONCON(GENLIB_NAME("f[0:%d]", nb - 1), 100 + d, 20, SAX_EAST, OUT);
   else
      ADDICONCON(GENLIB_NAME("f[%d:0]", nb - 1), 100 + d, 20, SAX_EAST, OUT);
#ifdef PWET
   ADDICONCON("vdd", 60, 60, SAX_NORTH, IN);
   ADDICONCON("vss", 50 + d / 2, -20, SAX_SOUTH, IN);
   ADDICONLINE(60, 40, 60, 60);
   ADDICONLINE(50 + d / 2, 0, 50 + d / 2, -20);
#endif

   ADDICONLINE(0, 20, 20, 20);
   ADDICONLINE(80 + d, 20, 100 + d, 20);
   ADDICONLINE(40, 40, 40, 60);
   ADDICONLINE(35, 40, 40, 35);
   ADDICONLINE(40, 35, 45, 40);

   ADDICONCOMP(COMP_ISLINE, 20, 0);
   ADDICONCOMP(COMP_ISLINE, 80 + d, 0);
   ADDICONCOMP(COMP_ISLINE, 80 + d, 40);
   ADDICONCOMP(COMP_ISLINE, 20, 40);
   ADDICONSHAPE();
   SAVEICON();
}
