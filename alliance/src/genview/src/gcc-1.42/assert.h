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

/* Allow this file to be included multiple times
   with different settings of NDEBUG.  */
#undef assert
#undef __assert

#ifdef NDEBUG
#define assert(ignore)  ((void)0)
#else

void __eprintf ();		/* Defined in gnulib */

#ifdef __STDC__

#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (#expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (__eprintf ("Failed assertion `%s' at line %d of `%s'.\n",	\
	      expression, lineno, file), 0)

#else /* no __STDC__; i.e. -traditional.  */

#define assert(expression)  \
  ((void) ((expression) ? 0 : __assert (expression, __FILE__, __LINE__)))

#define __assert(expression, file, lineno)  \
  (__eprintf ("Failed assertion `%s' at line %d of `%s'.\n",	\
	      "expression", lineno, file), 0)

#endif /* no __STDC__; i.e. -traditional.  */

#endif
