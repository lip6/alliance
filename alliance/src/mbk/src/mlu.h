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
 * Purpose : logical utility functions
 * Date    : 05/08/93
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * Modified by pnt <Pierre.Nguyen-Tuong@lip6.fr> 2002
 * $Id: mlu.h,v 1.6 2012/05/14 14:20:23 alliance Exp $
 */

#ifndef _MLU_H_
#define _MLU_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__) || defined(__cplusplus)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

  extern           void  flattenlofig __P((lofig_list *ptfig, const char *insname, char concat));
  extern    lofig_list * unflattenlofig __P((lofig_list *ptfig, const char *figname, const char *insname, chain_list *list));
  extern           void  rflattenlofig __P((lofig_list *ptfig, char concat, char catal));
  extern           void  loadlofig __P((lofig_list *ptfig, const char *name, char mode));
  extern           void  savelofig __P((lofig_list *ptfig));
  extern           void  mlodebug __P((void *head_pnt, char *stru_name));
  extern    losig_list * duplosiglst __P((losig_list *losig_ptr, ptype_list **BKSIG_ptr,int));
  extern    losig_list * duplosig __P((losig_list *losig_ptr, ptype_list **BKSIG_ptr, int));
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
  extern            void sortlocon __P((locon_list **connectors));
  extern            void sortlosig __P((losig_list **signals));
  extern    losig_list * givelosig __P((lofig_list *ptfig, long index));
  extern             int checkloconorder __P((locon_list *c));
  extern            int  restoredirvbe __P((lofig_list *lof));
  extern            int  guessextdir __P((lofig_list *lof));
  extern            int  restorealldir __P((lofig_list *lf));

/*************************** Analogical world ***************************************/

extern locap_list *duplocaplst __P((locap_list *locap_ptr))    ;
extern locap_list *duplocap __P((locap_list *locap_ptr))       ;
extern lores_list *duploreslst __P((lores_list *lores_ptr))    ;
extern lores_list *duplores __P((lores_list *lores_ptr))       ;
extern loself_list *duploselflst __P((loself_list *loself_ptr)) ;
extern loself_list *duploself __P((loself_list *loself_ptr))    ;

/************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MLU */
