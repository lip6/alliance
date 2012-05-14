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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* 
 * Purpose : mbk_ph.c header file for functions declarations
 * Date    : 18/12/91
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * $Id: mbk_ph.h,v 1.3 2012/05/14 14:20:23 alliance Exp $
 */

#ifndef _MBK_PH_H_
#define _MBK_PH_H_

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

  extern    phcon_list * addphcon __P((phfig_list *ptfig, char orient, const char *conname, long x, long y, char layer, long width));
  extern    phfig_list * addphfig __P((const char *figname));
  extern    phins_list * addphins __P((phfig_list *ptfig, const char *figname, const char *insname, char sym, long x, long y));
  extern    phref_list * addphref __P((phfig_list *ptfig, const char *type, const char *name, long x, long y));
  extern    phseg_list * addphseg __P((phfig_list *ptfig, char layer, long width, long x1, long y1, long x2, long y2, const char *nodename));
  extern    phvia_list * addphvia __P((phfig_list *ptfig, char viatype, long x, long y, long dx, long dy, const char *name));
static chain_list *addsorted();
  extern           void  defab __P((phfig_list *ptfig, long x1, long y1, long x2, long y2));
  extern            int  delphcon __P((phfig_list *ptfig, phcon_list *ptdelcon));
  extern            int  delphfig __P((const char *name));
  extern            int  delphins __P((phfig_list *ptfig, const char *insname));
  extern            int  delphref __P((phfig_list *ptfig, phref_list *ptref));
  extern            int  delphseg __P((phfig_list *ptfig, phseg_list *ptseg));
  extern            int  delphvia __P((phfig_list *ptfig, phvia_list *ptvia));
  extern    phcon_list * getphcon __P((phfig_list *ptfig, const char *conname, long index));
  extern    phins_list * getphins __P((phfig_list *ptfig, const char *insname));
  extern    phref_list * getphref __P((phfig_list *ptfig, const char *refname));
  extern           void  viewphcon __P((phcon_list *pt_con));
  extern           void  viewphins __P((phins_list *pt_ins));
  extern           void  viewphseg __P((phseg_list *pt_seg));
  extern           void  viewphvia __P((phvia_list *pt_via));
  extern           void  viewphref __P((phref_list *pt_ref));
  extern           void  viewph __P(());
  extern           void  viewphfig __P((phfig_list *pt_fig));
  extern           void  xyflat __P((long *xout, long *yout, long x, long y, long xins, long yins, long x1, long y1, long x2, long y2, char trsf));

#endif
  
