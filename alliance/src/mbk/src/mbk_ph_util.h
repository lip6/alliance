/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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
 * Purpose : mbk_ph_util.c header file for utilities functions declarations
 * Date    : 18/12/91
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * $Id: mbk_ph_util.h,v 1.2 2002/09/30 16:20:50 czo Exp $
 */

#ifndef _MBK_PH_UTIL_H_
#define _MBK_PH_UTIL_H_

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

  extern    phfig_list * getphfig __P((const char *figname, char mode));
  extern           void  loadphfig __P((phfig_list *ptfig, const char *figname, char mode));
  extern           void  savephfig __P((phfig_list *ptfig));
  extern           void  rflattenphfig __P((phfig_list *ptfig, char concat, char catal));
  extern           void  flattenphfig __P((phfig_list *ptfig, const char *insname, char concat));
  
#endif

