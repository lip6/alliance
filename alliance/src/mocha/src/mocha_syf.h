/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     MOCHA                         |
|                                                             |
| File    :                  mocha_syf.h                      |
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

# ifndef MOCHA_SYF_H
# define MOCHA_SYF_H

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define MOCHA_SYF_INFO( Figure ) ( (mochasyfinfo  *)( ( Figure )->USER ) )
# define MOCHA_SYF_STATE( State ) ( (mochasyfstate *)( ( State  )->USER ) )
# define MOCHA_SYF_OUT( Out     ) ( (mochasyfout   *)( ( Out    )->USER ) )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Syf Output Structure                    |
|                                                             |
\------------------------------------------------------------*/

  typedef struct mochasyfout
  {
    long        INDEX;
    long        NUMBER_ZERO;
    long        NUMBER_ONE;
    long        NUMBER_ASSIGN;
    long        POLARITY;
    chain_list *ABL;

  } mochasyfout;

/*------------------------------------------------------------\
|                                                             |
|                     Syf Code Structure                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct mochasyfcode
  {
    long        VALUE;
    long        USED;
    chain_list *ABL;
    chain_list *ABL_OUT;

  } mochasyfcode;

/*------------------------------------------------------------\
|                                                             |
|                     Syf State Structure                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct mochasyfstate
  {
    char          *CURRENT_NAME;
    char          *NEXT_NAME;
    char          *NEXT_IN_NAME;
    long           INDEX;
    mochasyfcode  *CODE;
    chain_list    *ABL_TRANS;

  } mochasyfstate;

/*------------------------------------------------------------\
|                                                             |
|                    Syf Register Structure                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct mochasyfregout
  {
    char       *NAME_MASTER;
    char       *NAME_OUT;
    chain_list *ABL;

  } mochasyfregout;

  typedef struct mochasyfregstate
  {
    char       *NAME_IN;
    bddnode    *NODE_IN;
    char       *NAME_MASTER;
    char       *NAME_OUT;
    bddnode    *NODE_OUT;
    chain_list *ABL_IN;
    chain_list *ABL_SET;
    chain_list *ABL_RESET;
    chain_list *ABL;

  } mochasyfregstate;

/*------------------------------------------------------------\
|                                                             |
|                      Syf Info Structure                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct mochasyfinfo
  {
    char             *CURRENT_STATE;
    char             *NEXT_STATE;

    long              NUMBER_BIT;
    long              NUMBER_REG;
    mochasyfregstate *REG_ARRAY;

    long              NUMBER_OUT;
    mochasyfregout   *OUT_ARRAY;

    long              NUMBER_CODE;
    mochasyfcode     *CODE_ARRAY;

  } mochasyfinfo;
       
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

  extern fsmfig_list *MochaSyfFbhParse();
  extern void         MochaSyfFsmSimplify();
  extern void         MochaSyfFsmEncode();
  extern void         MochaSyfSynthFreeFsmAbl();
  extern void         MochaSyfSynthFsm();
  extern befig_list  *MochaSyfFsm2Beh();
  extern void         MochaSyfFreeFsm();

# endif
