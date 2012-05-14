/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_shared.h                   |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   07.08.02                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef MOCHA_SHARED_H
# define MOCHA_SHARED_H

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define MOCHA_BEH_INFO( Figure ) ( (mochabehinfo  *)( ( Figure )->USER ) )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

   typedef struct mochastate_list
   {
     struct mochastate_list *NEXT;
     char                   *NAME;
     bddnode                *BDD_STATE;
     long                    CODE;

   } mochastate_list;
   

   typedef struct mochafsm_list
   {
     struct mochafsm_list *NEXT;
     char                 *NAME;
     mochastate_list      *STATE;
     char                 *CURRENT_STATE;
     char                 *NEXT_STATE;
     authtable            *HASH_STATE;
     bddassoc             *BDD_ASSOC_STATE;
     long                  NUMBER_STATE;
     long                  NUMBER_BIT;
     long                  LEFT;
     long                  RIGHT;

   } mochafsm_list;

   typedef struct mochafig_list
   {
     struct mochafig_list *NEXT;
     char                 *NAME;
     mochafsm_list        *FSM;
     befig_list           *BEH_FIGURE;
     ctlfig_list          *CTL_FIGURE;
     chain_list           *CTL_SUPPORT;
     authtable            *HASH_CTL_SUPPORT;
     authtable            *HASH_BEH_AUX;
     authtable            *HASH_BEH_REG;
     authtable            *HASH_BEH_OUT;
     bddsystem            *BDD_SYSTEM;
     bddcircuit           *BDD_CIRCUIT;
     btrtransfunc         *BTR_TRANS_FUNC;
     bddassoc             *BDD_ASSOC_REG;
     bddnode              *BDD_FIRST_STATE;
     bddnode              *BDD_ASSUME;
     bddnode              *BDD_REACHED_STATE;
     short                 FLAG_DEBUG;

   } mochafig_list;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern mochafig_list *HEAD_MOCHA_FIG;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

   extern mochafig_list   *MochaAddFigure();
   extern mochafsm_list   *MochaAddFsm();
   extern mochastate_list *MochaAddState();

# endif
