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
| Tool    :                     SYF                           |
|                                                             |
| File    :                   syf_fsm.h                       |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef SYF_FSM_H
# define SYF_FSM_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define SYF_FSM_TYPE_MOORE 0
# define SYF_FSM_TYPE_MEALY 1
# define SYF_FSM_MAX_TYPE   2

/*------------------------------------------------------------\
|                                                             |
|                           Encode Type                       |
|                                                             |
\------------------------------------------------------------*/

# define SYF_ENCODE_RANDOM         'r'
# define SYF_ENCODE_CHAKER         'c'
# define SYF_ENCODE_ANNEAL         's'
# define SYF_ENCODE_USER           'u'
# define SYF_ENCODE_MUSTANG        'm'
# define SYF_ENCODE_ASP            'a'
# define SYF_ENCODE_JEDI           'j'
# define SYF_ENCODE_ONE_HOT        'o'
# define SYF_ENCODE_FRANCK         'f'

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define FSM_SYF_INFO( Figure ) ( (syfinfo  *)( ( Figure )->USER ) )
# define FSM_SYF_STATE( State ) ( (syfstate *)( ( State  )->USER ) )
# define FSM_SYF_OUT( Out     ) ( (syfout   *)( ( Out    )->USER ) )

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Syf Control Structure                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct syfctrl
  {
    char       *NAME;
    chain_list *ABL;

  } syfctrl;

/*------------------------------------------------------------\
|                                                             |
|                     Syf Output Structure                    |
|                                                             |
\------------------------------------------------------------*/

  typedef struct syfout
  {
    long        INDEX;
    long        NUMBER_ZERO;
    long        NUMBER_ONE;
    long        NUMBER_ASSIGN;
    long        POLARITY;
    chain_list *ABL;

  } syfout;

/*------------------------------------------------------------\
|                                                             |
|                     Syf Code Structure                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct syfcode
  {
    long        VALUE;
    long        USED;
    chain_list *ABL;
    chain_list *ABL_OUT;

  } syfcode;

/*------------------------------------------------------------\
|                                                             |
|                     Syf State Structure                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct syfstate
  {
    char       *CURRENT_NAME;
    char       *RETURN_NAME;
    char       *NEXT_NAME;
    char       *NEXT_IN_NAME;
    long        INDEX;
    syfcode    *CODE;
    chain_list *ABL_TRANS;
    chain_list *ABL_RETURN;

  } syfstate;

/*------------------------------------------------------------\
|                                                             |
|                    Syf Register Structure                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct syfregout
  {
    char       *NAME_MASTER;
    char       *NAME_OUT;
    chain_list *ABL;

  } syfregout;

  typedef struct syfregstate
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

  } syfregstate;

  typedef struct syfregstack
  {
    char       *NAME_MASTER;
    char       *NAME_OUT;
    chain_list *ABL;
    long        FLAGS;

  } syfregstack;

/*------------------------------------------------------------\
|                                                             |
|                      Syf Info Structure                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct syfinfo
  {
    char        *CURRENT_STATE;
    char        *NEXT_STATE;
    char        *RETURN_STATE;
    char        *SCAN_IN;
    char        *SCAN_OUT;
    char        *SCAN_TEST;
    char        *SCAN_STACK;

    char         STACK;
    char         SCAN_PATH;
    char         REG_OUT;

    long         STACK_MASK;
    long         STACK_CSTE;

    char         FSM_TYPE;
    char         ENCODE;

    long         NUMBER_BIT;
    long         NUMBER_REG;
    syfregstate *REG_ARRAY;

    long         NUMBER_OUT;
    syfregout   *OUT_ARRAY;

    long         NUMBER_STACK;
    syfregstack *STACK_ARRAY;

    long         NUMBER_CTRL;
    syfctrl     *CTRL_ARRAY;

    long         NUMBER_CODE;
    syfcode     *CODE_ARRAY;

    long        *MUSTANG_OUT;
    long        *MUSTANG_TRANS;
    long        *MUSTANG_CTRL;
    long        *MUSTANG_EDGE;

    long        *ASP_STATE;

  } syfinfo;
       
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *SYF_FSM_TYPE_NAME[ SYF_FSM_MAX_TYPE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Add Functions                         |
|                                                             |
\------------------------------------------------------------*/

  extern fsmfig_list    *Syfaddfsmfig();
  extern syfinfo        *Syfaddfsmsyfinfo();
  extern syfout         *Syfaddfsmsyfout();
  extern syfstate       *Syfaddfsmsyfstate();

# endif
