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

/* ---------------------------------------- */
/*           VARARGS  for MIPS/GNU CC       */
/*                                          */
/*                                          */
/*                                          */
/*                                          */
/* ---------------------------------------- */


/* These macros implement varargs for GNU C--either traditional or ANSU.  */

/* Define __gnuc_va_list.  */

#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef char * __gnuc_va_list;
#endif /* not __GNUC_VA_LIST */

/* If this is for internal libc use, don't define anything but
   __gnuc_va_list.  */
#if defined (_STDARG_H) || defined (_VARARGS_H)

/* In GCC version 2, we want an ellipsis at the end of the declaration
   of the argument list.  GCC version 1 can't parse it.  */

#if __GNUC__ > 1
#define __va_ellipsis ...
#else
#define __va_ellipsis
#endif

#define __va_rounded_size(__TYPE)  \
  (((sizeof (__TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#ifdef _STDARG_H
#define va_start(__AP, __LASTARG) 						\
 (__AP = ((char *) &(__LASTARG) + __va_rounded_size (__LASTARG)))

#else
#define va_alist  __builtin_va_alist
#define va_dcl    int __builtin_va_alist; __va_ellipsis
#define va_start(__AP)  __AP = (char *) &__builtin_va_alist
#endif

#ifndef va_end
void va_end (__gnuc_va_list);		/* Defined in libgcc.a */
#endif
#define va_end(__AP)
/* Quick and Dirty hack */
#define va_list char *

#ifdef lint	/* complains about constant in conditional context */
#define va_arg(list, mode) ((mode *)(list += __va_rounded_size(mode)))[-1]

#else		/* !lint */
/* We cast to void * and then to TYPE * because this avoids
   a warning about increasing the alignment requirement.  */
#define va_arg(__AP, __type)						    \
  ((__type *) (void *) (__AP = (char *) ((__alignof(__type) > 4		    \
					  ? ((int)__AP + 8 - 1) & -8	    \
					  : ((int)__AP + 4 - 1) & -4)	    \
					 + __va_rounded_size(__type))))[-1]
#endif		/* lint */

#endif /* defined (_STDARG_H) || defined (_VARARGS_H) */
