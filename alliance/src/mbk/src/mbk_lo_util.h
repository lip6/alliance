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
 * Purpose : mbk_lo_util.c header file for functions declarations
 * Date    : 06/03/92
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * Modified by pnt <Pierre.Nguyen-Tuong@lip6.fr> 2002
 * $Id: mbk_lo_util.h,v 1.3 2002/09/30 16:20:49 czo Exp $
 */

/* $Id: mbk_lo_util.h,v 1.3 2002/09/30 16:20:49 czo Exp $ */

#ifndef _MBK_LO_UTIL_H_
#define _MBK_LO_UTIL_H_

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

  extern           void  flattenlofig __P((lofig_list *ptfig, const char *insname, char concat));
lofig_list *givelofig();
  extern    losig_list * givelosig __P((lofig_list *ptfig, long index));
  extern           void  loadlofig __P((lofig_list *ptfig, const char *name, char mode));
  extern           void  rflattenlofig __P((lofig_list *ptfig, char concat, char catal));
  extern           void  savelofig __P((lofig_list *ptfig));
  extern    lofig_list * unflattenlofig __P((lofig_list *ptfig, const char *figname, const char *insname, chain_list *list));
  extern    losig_list * duplosiglst __P((losig_list *losig_ptr, ptype_list **BKSIG_ptr,int));
  extern    losig_list * duplosig __P((losig_list *losig_ptr, ptype_list **BKSIG_ptr,int));
  extern    chain_list * dupchainlst __P((chain_list *chain_ptr));
  extern    locon_list * duploconlst __P((locon_list *locon_ptr));
  extern    locon_list * duplocon __P((locon_list *locon_ptr));
  extern    loins_list * duploinslst __P((loins_list *loins_ptr));
  extern    loins_list * duploins __P((loins_list *loins_ptr));
  extern    lotrs_list * duplotrslst __P((lotrs_list *lotrs_ptr));
  extern    lotrs_list * duplotrs __P((lotrs_list *lotrs_ptr));
  extern    lofig_list * duplofiglst __P((lofig_list *lofig_ptr));
  extern    lofig_list * duplofig __P((lofig_list *lofig_ptr));
  extern    lofig_list * rduplofig __P((lofig_list *lofig_ptr));

/*************************** Analogical world ***************************************/

extern locap_list *duplocaplst __P((locap_list *locap_ptr))    ;
extern locap_list *duplocap __P((locap_list *locap_ptr))       ;
extern lores_list *duploreslst __P((lores_list *lores_ptr))    ;
extern lores_list *duplores __P((lores_list *lores_ptr))       ;
extern loself_list *duploselflst __P((loself_list *loself_ptr)) ;
extern loself_list *duploself __P((loself_list *loself_ptr))    ;

/************************************************************************************/

#endif

