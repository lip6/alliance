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

/****************************************************************************/
/*                                                                          */
/*                      ALLIANCE CAO & VLSI CAD                             */
/*                                                                          */
/*    Product : (S)ymbolic (2)to (R)eal translater                          */
/*    File    : statistics.c                                                */
/*                                                                          */
/*    (c) copyright 1992 MASI laboratory CAO & VLSI team                    */
/*    All Right Reserved                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*                           Statistics functions                           */
/*                                                                          */
/****************************************************************************/

#include <stdio.h>
#include "statistics.h"

long STAT_RECT_REQRD = 0;       /* number of rectangles required to be allocated */
long STAT_RECT_ALLOC = 0;       /* number of rectangles really allocated in memory */
long STAT_SCTCH_RQRD = 0;       /* number of scotchs needed */
long STAT_SCTCH_CRTD = 0;       /* number of scotchs created */
extern long mbkalloc_stat;      /* memory allocated by mbk */

/*******************************************************************************
 * print_statistics: prints statistics
 ******************************************************************************/

void print_statistics (scotch_on_flag)
     int scotch_on_flag;
{
   printf ("\to memory allocation informations\n");
   printf ("\t--> required rectangles = %ld ", STAT_RECT_REQRD);
   printf (" really allocated = %ld\n", STAT_RECT_ALLOC);
   if (scotch_on_flag)
   {
      printf ("\t--> required scotchs = %ld ", STAT_SCTCH_RQRD);
      printf (" really created = %ld\n", STAT_SCTCH_CRTD);
   }
   printf ("\t--> Number of allocated bytes: %ld\n", mbkalloc_stat);
}
