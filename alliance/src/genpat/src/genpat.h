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

/***************************************************************/
/* module   : libpat_h.h                                       */
/* date     : 28/03/92                                         */
/* auteurs  : RENAUD & VENOT                                   */
/***************************************************************/

/* $Id: genpat.h,v 1.1 2002/05/30 13:28:15 xtof Exp $ */

#ifndef GENPAT_H
#define GENPAT_H


#define DEF_GENPAT GNP_LINE = __LINE__ , d_DEF_GENPAT
#define DECLAR     GNP_LINE = __LINE__ , d_DECLAR
#define ARRAY      GNP_LINE = __LINE__ , d_ARRAY
#define AFFECT     GNP_LINE = __LINE__ , d_AFFECT
#define INIT       GNP_LINE = __LINE__ , d_INIT
#define LABEL      GNP_LINE = __LINE__ , d_LABEL
#define SETTUNIT   GNP_LINE = __LINE__ , d_SETTUNIT
#define GETCPAT    GNP_LINE = __LINE__ , d_GETCPAT
#define CONV       GNP_LINE = __LINE__ , d_CONV
#define SAV_GENPAT GNP_LINE = __LINE__ , d_SAV_GENPAT


#define		IN		"I"
#define		OUT		"O"
#define		INOUT		"T"
#define		REGISTER	"R"
#define		SIGNAL		"S"

#define		SPY		"S"

extern unsigned int GNP_LINE;

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

  extern           void  d_DEF_GENPAT __P((char *chaine));
  extern           void  d_DECLAR __P((char *ident, char *space, char *format, char *mode, char *size, char *option));
  extern           void  d_ARRAY __P(());
  extern           void  d_AFFECT __P((char *pat_numb, char *ident, char *val));
  extern           void  d_INIT __P((char *pat_numb, char *ident, char *val));
  extern           void  d_LABEL __P((char *ident));
  extern           void  d_SETTUNIT __P((char *ident));
  extern            unsigned long  d_GETCPAT __P(());
  extern          char * d_CONV __P((int entier));
  extern           void  d_SAV_GENPAT __P(());


#endif
