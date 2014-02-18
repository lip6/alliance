/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

# ifndef PROOF_UTIL_H
# define PROOF_UTIL_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

extern chain_list* berinToChain_list __P(( befig_list *beh ));
extern char*       nameIntVect       __P(( char* name ));
extern int         mayCoverBdd       __P(( pNode on1, pNode dc1, pNode on2, pNode dc2 ));
extern void        displayExprLaxInt __P(( chain_list* expr ));
extern void        displayExprLax    __P(( chain_list* expr ));
extern chain_list* searchExprOutBeh  __P(( befig_list* beh, char* name));
extern void        substExprBeh      __P(( befig_list* beh, char* name, chain_list* sexpr));
extern void        polarBDBeh        __P(( befig_list* beh ));
extern void        elimInoutBeh      __P(( befig_list* beh ));
extern pNode       sumCNDBinode      __P(( binode_list* binode ));
extern chain_list* AND2              __P(( chain_list* expr1, chain_list* expr2));
extern chain_list* OR2               __P(( chain_list* expr1, chain_list* expr2));
extern chain_list* sumBiabl          __P(( biabl_list* biabl ));
extern void        orderProofBeh     __P(( befig_list* bef1,  befig_list* bef2, char varaux ));
extern int         elimBusMuxBeh     __P(( befig_list* beh ));
extern int         proofBeh          __P(( befig_list* beh1,  befig_list* beh2, int mode, int exitIO));

# endif
