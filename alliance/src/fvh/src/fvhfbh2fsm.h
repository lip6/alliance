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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     FVH                           |
|                                                             |
| File    :                  fvhfbh2fsm.h                     |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef FVH_FBH2FSM_H
# define FVH_FBH2FSM_H

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

  typedef struct fbpcsinfo
  {
    chain_list *READ_LIST;
    chain_list *WRITE_LIST;
    authtable  *HASH_READ;
    authtable  *HASH_WRITE;

  } fbpcsinfo;

  typedef struct fvhfig_list
  {
    struct fvhfig_list *NEXT;
    char               *NAME;
    fsmfig_list        *FSM_FIGURE;
    fbpcs_list         *PROCESS_CLOCK;
    fbpcs_list         *PROCESS_MAIN;
    char               *CLOCK;
    char               *CURRENT_STATE;
    char               *NEXT_STATE;
    char               *FIRST_STATE;
    char               *RETURN_STATE;
    char               *CONTROL;
    char               *STACK_CONTROL[ FSM_MAX_CTRL ];
    fbtyp_list         *STATE_TYPE;
    fbtyp_list         *CONTROL_TYPE;
    unsigned char       CLOCK_OK;
    unsigned char       WHEN_CONDITION;
    unsigned char       STAR_STATE_LOCOUT;
    chain_list         *DEFAULT_STACK_NAME;
    long                DEFAULT_STACK_SIZE;
    long                STACK_PUSH_SIZE;
    long                STACK_POP_SIZE;

  } fvhfig_list;

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

  extern   fsmfig_list * FvhFbh2Fsm __P((fbfig_list *FbhFigure, fsmfig_list *FsmFigure));

  extern           void  FvhFbhTreatInstruction __P((fbfig_list *FbhFigure, fsmfig_list *FsmFigure, ptype_list *Instruct, fsmstate_list *StateFrom, fsmstate_list *StateTo, fsmstate_list *Return, int Control, ablexpr *Equation));
  extern           void  FvhFbhTreatWhen __P((fbfig_list *FbhFigure, fsmfig_list *FsmFigure, fbcas_list *ScanCase, fsmstate_list **PStateFrom, fsmstate_list *StateTo, fsmstate_list *Return, int *PControl, ablexpr *Equation));
  extern           void  FvhFbhTreatIf __P((fbfig_list *FbhFigure, fsmfig_list *FsmFigure, fbifs_list *ScanIfs, fsmstate_list *StateFrom, fsmstate_list *StateTo, fsmstate_list *Return, int Control, ablexpr *Equation));
  extern           void  FvhFbhTreatAsg __P((fbfig_list *FbhFigure, fsmfig_list *FsmFigure, fbasg_list *ScanAsg, fsmstate_list **PStateFrom, fsmstate_list **PStateTo, fsmstate_list **PReturn, int *PControl, ablexpr *Equation));

# endif
