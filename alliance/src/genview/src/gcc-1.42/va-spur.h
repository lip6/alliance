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

/*  varargs.h for SPUR */

/* NB.  This is NOT the definition needed for the new ANSI proposed
   standard */
 

struct __va_struct { char __regs[20]; };

#define va_alist __va_regs, __va_stack

#define va_dcl struct __va_struct __va_regs; int __va_stack;

typedef struct {
    int __pnt;
    char *__regs;
    char *__stack;
} va_list;

#define va_start(pvar) \
     ((pvar).__pnt = 0, (pvar).__regs = __va_regs.__regs, \
      (pvar).__stack = (char *) &__va_stack)
#define va_end(pvar)

#define va_arg(pvar,type)  \
    ({  type __va_result; \
        if ((pvar).__pnt >= 20) { \
           __va_result = *( (type *) ((pvar).__stack + (pvar).__pnt - 20)); \
	   (pvar).__pnt += (sizeof(type) + 7) & ~7; \
	} \
	else if ((pvar).__pnt + sizeof(type) > 20) { \
	   __va_result = * (type *) (pvar).__stack; \
	   (pvar).__pnt = 20 + ( (sizeof(type) + 7) & ~7); \
	} \
	else if (sizeof(type) == 8) { \
	   union {double d; int i[2];} __u; \
	   __u.i[0] = *(int *) ((pvar).__regs + (pvar).__pnt); \
	   __u.i[1] = *(int *) ((pvar).__regs + (pvar).__pnt + 4); \
	   __va_result = * (type *) &__u; \
	   (pvar).__pnt += 8; \
	} \
	else { \
	   __va_result = * (type *) ((pvar).__regs + (pvar).__pnt); \
	   (pvar).__pnt += (sizeof(type) + 3) & ~3; \
	} \
	__va_result; })
