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

/* This is just like the default gvarargs.h
   except for differences decribed below.  */
#ifndef __PWET_VA_LIST__
#define __PWET_VA_LIST__

/* va_list is a structure instead of a char*.  */
typedef struct __va_ctl
{
  char *__stack;   /* Current pointer for fetching args.  */
  char *__beg;     /* Pointer to position of first saved register arg.  */
} va_list;

#define va_alist  __builtin_va_alist
#define va_dcl    int __builtin_va_alist;

/* The difference is to store the stack address in both components
   instead of in AP itself.  */
#define va_start(AP) 						\
 (__builtin_saveregs (),					\
 (AP).__beg = (AP).__stack = ((char *) &__builtin_va_alist))
#define va_end(pvar)

#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

/* The difference is that, for an aggregate that is not word-aligned,
   we advance (pvar).__stack to the first non-reg slot.  */
#define va_arg(pvar,TYPE)					\
({ TYPE __va_temp;						\
   ((__builtin_classify_type (__va_temp) < 12			\
     || __alignof__ __va_temp >= 4)				\
    ? ((pvar).__stack += __va_rounded_size (TYPE),		\
       *((TYPE *) ((pvar).__stack - __va_rounded_size (TYPE))))	\
    : ((((pvar).__stack - (pvar).__beg < 24)			\
	? (pvar).__stack = (pvar).__beg + 24 : 0),		\
       (pvar).__stack += __va_rounded_size (TYPE),		\
       *((TYPE *) ((pvar).__stack - __va_rounded_size (TYPE)))));})
#endif
