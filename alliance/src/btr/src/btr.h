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
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for BTR         |
|                                                       |
|  Date    :            14.02.96                        |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef BTR_102_H
# define BTR_102_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                 Bdd Transition Function               |
|                                                       |
\------------------------------------------------------*/

   typedef long btrvarorder;

   typedef struct btrvarfunc
   {
     bddnode *VAR;
     bddnode *FUNC;

   } btrvarfunc;

   typedef struct btrtransfunc
   {
     btrvarfunc  *VAR_FUNC;
     btrvarorder *VAR_ORDER;
     bddvar       MAX_VAR;
     bddvar       NUMBER_VAR;
     bddsystem   *BDD_SYSTEM;

   } btrtransfunc;

/*------------------------------------------------------\
|                                                       |
|                 Bdd Transition Relation               |
|                                                       |
\------------------------------------------------------*/

   typedef struct btrvarrel
   {
     bddnode *VAR;
     bddnode *PRIME;
     bddnode *REL;
     
   } btrvarrel;

   typedef struct btrtransrel
   {
     btrvarrel *VAR_REL;
     bddassoc  *PRIME_ASSOC;
     bddassoc  *VAR_ASSOC;
     bddvar     MAX_VAR;
     bddvar     NUMBER_VAR;
     bddsystem *BDD_SYSTEM;

   } btrtransrel;

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                      Env  Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  btrenv __P(());

/*------------------------------------------------------\
|                                                       |
|                  Allocation Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern  btrtransfunc * allocbtrtransfunc __P(());
  extern    btrvarfunc * allocbtrvarfunc __P((long Number));
  extern   btrvarorder * allocbtrvarorder __P((long Number));

  extern   btrtransrel * allocbtrtransrel __P(());
  extern     btrvarrel * allocbtrvarrel __P((long Number));

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  freebtrtransfunc __P((btrtransfunc *TransFunc));
  extern           void  freebtrvarfunc __P((btrvarfunc *VarFunc));
  extern           void  freebtrvarorder __P((btrvarorder *VarOrder));

  extern           void  freebtrtransrel __P((btrtransrel *TransRel));
  extern           void  freebtrvarrel __P((btrvarrel *VarRel));

/*------------------------------------------------------\
|                                                       |
|                    Resize Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern    btrvarfunc * resizebtrvarfunc __P((btrvarfunc *VarFunc, long OldNum, long NewNum));
  extern   btrvarorder * resizebtrvarorder __P((btrvarorder *VarOrder, long OldNum, long NewNum));

  extern     btrvarrel * resizebtrvarrel __P((btrvarrel *VarRel, long OldNum, long NewNum));

/*------------------------------------------------------\
|                                                       |
|                Transition Function Functions          |
|                                                       |
\------------------------------------------------------*/

  extern  btrtransfunc * createbtrtransfunc __P((bddsystem *BddSystem, bddvar MaxVar));
  extern           void  resetbtrtransfunc __P((btrtransfunc *TransFunc));
  extern           void  destroybtrtransfunc __P((btrtransfunc *TransFunc));
  extern           void  viewbtrtransfunc __P((btrtransfunc *TransFunc, void (*FuncView)()));
  extern           void  addbtrtransfunc __P((btrtransfunc *TransFunc, bddnode *VarNode, bddnode *FuncNode));

/*------------------------------------------------------\
|                                                       |
|          Image and PreImage Transition Function       |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * imagebtrtransfunc __P((btrtransfunc *TransFunc, bddnode *StateSet));
  extern       bddnode * preimagebtrtransfunc __P((btrtransfunc *TransFunc, bddnode *StateSet));

/*------------------------------------------------------\
|                                                       |
|                Reorder Transition Function            |
|                                                       |
\------------------------------------------------------*/

  extern           void  reorderbtrtransfunc __P((btrtransfunc *TransFunc));

/*------------------------------------------------------\
|                                                       |
|                Transition Relation Functions          |
|                                                       |
\------------------------------------------------------*/

  extern   btrtransrel * createbtrtransrel __P((bddsystem *BddSystem, bddvar MaxVar));
  extern           void  resetbtrtransrel __P((btrtransrel *TransRel));
  extern           void  destroybtrtransrel __P((btrtransrel *TransRel));
  extern           void  viewbtrtransrel __P((btrtransrel *TransRel, void (*FuncView)()));

/*------------------------------------------------------\
|                                                       |
|          Image and PreImage Transition Relation       |
|                                                       |
\------------------------------------------------------*/

  extern       bddnode * imagebtrtransrel __P((btrtransrel *TransRel, bddnode *StateSet));

# endif 
