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

/*
 * Description de la machine virtuelle
 *
 * Ecrit par Arnaud COMPAN & Francois PECHEUX ** avril-juin 1990
 *
 */

#define MY_SUN

#define FALSE 0
#define TRUE 1

#define HOST_BITS_PER_CHAR 8
#define HOST_BITS_PER_SHORT 16
#define HOST_BITS_PER_INT 32
#define HOST_BITS_PER_LONG 32

#define SUCCESS_EXIT_CODE 0
#define FATAL_EXIT_CODE 33

#ifdef MY_SUN
#define USG
#endif

#include "tm.h"

#ifdef MY_SUN
#define bcopy(a,b,c) memcpy (b,a,c)
#define bzero(a,b)   memset (a,0,b)
#define bcmp(a,b,c)  memcmp (a,b,c)
#endif

#ifdef __GNUC__
#define alloca(n) __builtin_alloca(n)
#endif

