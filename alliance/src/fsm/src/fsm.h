/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for FSM         |
|                                                       |
|  Date    :            04.12.96                        |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef FSM_104_H
# define FSM_104_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                    Fsm Port Direction                 |
|                                                       |
\------------------------------------------------------*/

# define FSM_DIR_IN        'I'
# define FSM_DIR_OUT       'O'
# define FSM_DIR_INOUT     'B'
# define FSM_DIR_OUT_BUS   'Z'
# define FSM_DIR_INOUT_BUS 'T'

/*------------------------------------------------------\
|                                                       |
|                      Fsm Port Type                    |
|                                                       |
\------------------------------------------------------*/

# define FSM_TYPE_SEVERITY  1
# define FSM_TYPE_BOOLEAN   2
# define FSM_TYPE_BIT       3
# define FSM_TYPE_INTEGER   4
# define FSM_TYPE_MUX_BIT   5
# define FSM_TYPE_WOR_BIT   6
# define FSM_TYPE_REG_BIT   7
# define FSM_TYPE_NATURAL   8
# define FSM_TYPE_BIT_VEC   9
# define FSM_TYPE_MUX_VEC   10
# define FSM_TYPE_WOR_VEC   11
# define FSM_TYPE_REG_VEC   12

# define FSM_MAX_TYPE       13

/*------------------------------------------------------\
|                                                       |
|                    Fsm Stack Control                  |
|                                                       |
\------------------------------------------------------*/

# define FSM_CTRL_NOP   0
# define FSM_CTRL_PUSH  1
# define FSM_CTRL_POP   2
# define FSM_MAX_CTRL   3

/*------------------------------------------------------\
|                                                       |
|                      Fsm Flag                         |
|                                                       |
\------------------------------------------------------*/

# define FSM_STAR_MASK      (long)( 0x0001 )
# define FSM_FIRST_MASK     (long)( 0x0002 )

/*------------------------------------------------------\
|                                                       |
|                      Fsm Fig Flag                     |
|                                                       |
\------------------------------------------------------*/

# define FSM_MIXED_RTL_MASK  (long)( 0x0001 )
# define FSM_MULTI_MASK      (long)( 0x0002 )
# define FSM_MULTI_LEAF_MASK (long)( 0x0004 )

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                  State Type Macros                    |
|                                                       |
\------------------------------------------------------*/

# define IsFsmFirstState( T )    ( (T)->FLAGS &   FSM_FIRST_MASK )
# define SetFsmFirstState( T )   ( (T)->FLAGS |=  FSM_FIRST_MASK )
# define ClearFsmFirstState( T ) ( (T)->FLAGS &= ~FSM_FIRST_MASK )

# define IsFsmStarState( T )     ( (T)->FLAGS &   FSM_STAR_MASK )
# define SetFsmStarState( T )    ( (T)->FLAGS |=  FSM_STAR_MASK )
# define ClearFsmStarState( T )  ( (T)->FLAGS &= ~FSM_STAR_MASK )

/*------------------------------------------------------\
|                                                       |
|                Transition Type Macros                 |
|                                                       |
\------------------------------------------------------*/

# define IsFsmStarTrans( T )    ( (T)->FLAGS &   FSM_STAR_MASK )
# define SetFsmStarTrans( T )   ( (T)->FLAGS |=  FSM_STAR_MASK )
# define ClearFsmStarTrans( T ) ( (T)->FLAGS &= ~FSM_STAR_MASK )

/*------------------------------------------------------\
|                                                       |
|                     Stack Type Macros                 |
|                                                       |
\------------------------------------------------------*/

# define IsFsmStarStack( S )    ( (S)->FLAGS &   FSM_STAR_MASK )
# define SetFsmStarStack( S )   ( (S)->FLAGS |=  FSM_STAR_MASK )
# define ClearFsmStarStack( S ) ( (S)->FLAGS &= ~FSM_STAR_MASK )

/*------------------------------------------------------\
|                                                       |
|                   Figure Type Macros                  |
|                                                       |
\------------------------------------------------------*/

# define IsFsmFigMixedRtl( F )    ( (F)->FLAGS &   FSM_MIXED_RTL_MASK )
# define SetFsmFigMixedRtl( F )   ( (F)->FLAGS |=  FSM_MIXED_RTL_MASK )
# define ClearFsmFigMixedRtl( F ) ( (F)->FLAGS &= ~FSM_MIXED_RTL_MASK )

# define IsFsmFigMulti( F )    ( (F)->FLAGS &   FSM_MULTI_MASK )
# define SetFsmFigMulti( F )   ( (F)->FLAGS |=  FSM_MULTI_MASK )
# define ClearFsmFigMulti( F ) ( (F)->FLAGS &= ~FSM_MULTI_MASK )

# define IsFsmFigMultiLeaf( F )    ( (F)->FLAGS &   FSM_MULTI_LEAF_MASK )
# define SetFsmFigMultiLeaf( F )   ( (F)->FLAGS |=  FSM_MULTI_LEAF_MASK )
# define ClearFsmFigMultiLeaf( F ) ( (F)->FLAGS &= ~FSM_MULTI_LEAF_MASK )

/*------------------------------------------------------\
|                                                       |
|                      Structures                       |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                     Fsm Input List                    |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmin_list
  {
    struct fsmin_list *NEXT;
    char              *NAME;
    long               FLAGS;
    void              *USER;

  } fsmin_list;

/*------------------------------------------------------\
|                                                       |
|                     Fsm Output List                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmout_list
  {
    struct fsmout_list *NEXT;
    char               *NAME;
    long                FLAGS;
    void               *USER;

  } fsmout_list;

/*------------------------------------------------------\
|                                                       |
|                     Fsm Port List                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmport_list
  {
    struct fsmport_list *NEXT;
    char                *NAME;
    char                 DIR;
    char                 TYPE;
    long                 FLAGS;
    void                *USER;

  } fsmport_list;

/*------------------------------------------------------\
|                                                       |
|                Fsm Local Output List                  |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmlocout_list

  {
    struct fsmlocout_list  *NEXT;
    fsmout_list            *OUT;
    ablexpr                *ABL;
    bddnode                *BDD;
    ablexpr                *ABL_DC;
    bddnode                *BDD_DC;
    long                    FLAGS;
    void                   *USER;

  } fsmlocout_list;

/*------------------------------------------------------\
|                                                       |
|                  Fsm Transition List                  |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmtrans_list
  
  {
    struct fsmtrans_list  *NEXT;
    struct fsmtrans_list **PREV;
    struct fsmstate_list  *FROM;
    struct fsmstate_list  *TO;
    ablexpr               *ABL;
    bddnode               *BDD;
    long                   FLAGS;
    void                  *USER;

  } fsmtrans_list;

/*------------------------------------------------------\
|                                                       |
|                        Fsm State                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmstate_list

  {
    struct fsmstate_list   *NEXT;
    struct fsmstate_list  **PREV;
    char                   *NAME;
    chain_list             *FROM;
    chain_list             *TO;
    chain_list             *STACK;
    fsmlocout_list         *LOCOUT;
    long                    FLAGS;
    void                   *USER;

  } fsmstate_list;

/*------------------------------------------------------\
|                                                       |
|                     Fsm Stack List                    |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmstack_list
  {
    struct fsmstack_list  *NEXT;
    struct fsmstack_list **PREV;
    char                   CTRL;
    fsmstate_list         *CURRENT;
    fsmstate_list         *RETURN;
    ablexpr               *ABL;
    bddnode               *BDD;
    long                   FLAGS;
    void                  *USER;

  } fsmstack_list;

/*------------------------------------------------------\
|                                                       |
|                     Fsm Pragma List                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmpragma_list
  {
    struct fsmpragma_list *NEXT;
    char                  *TYPE;
    char                  *NAME;
    char                  *VALUE;
    long                   FLAGS;
    void                  *USER;

  } fsmpragma_list;

/*------------------------------------------------------\
|                                                       |
|                   Fsm Figure List                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct fsmfig_list 
  {
    struct fsmfig_list *NEXT;
    char               *NAME;
    long                NUMBER_IN;
    long                NUMBER_OUT;
    long                NUMBER_PORT;
    long                NUMBER_STATE;
    long                NUMBER_TRANS;
    long                NUMBER_STACK;
    fsmin_list         *IN;
    fsmout_list        *OUT;
    fsmport_list       *PORT;
    fsmstate_list      *STATE;
    fsmtrans_list      *TRANS;
    fsmstack_list      *STACK;

    authtable          *HASH_PORT;
    authtable          *HASH_IN;
    authtable          *HASH_OUT;
    authtable          *HASH_STATE;
    auth2table         *HASH_TRANS;

    long                STACK_SIZE;

    fsmstate_list      *FIRST_STATE;
    fsmstate_list      *STAR_STATE;

    char               *CLOCK;
    ablexpr            *CLOCK_ABL;

    bddcircuit         *CIRCUIT;

    long                FLAGS;
    fsmpragma_list     *PRAGMA;

    chain_list         *MULTI;
    void               *FIGURE;
    void               *USER;
    
  } fsmfig_list;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

 extern fsmfig_list   *HEAD_FSMFIG;
 extern char          *FSM_CTRL_NAME [ FSM_MAX_CTRL ];
 extern char          *FSM_TYPE_NAME [ FSM_MAX_TYPE ];

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                    Alloc Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern   fsmfig_list * allocfsmfig __P(());
  extern fsmstate_list * allocfsmstate __P(());
  extern fsmstack_list * allocfsmstack __P(());
  extern fsmtrans_list * allocfsmtrans __P(());
  extern fsmlocout_list * allocfsmlocout __P(());
  extern   fsmout_list * allocfsmout __P(());
  extern    fsmin_list * allocfsmin __P(());
  extern fsmpragma_list * allocfsmpragma __P(());
  extern  fsmport_list * allocfsmport __P(());

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  freefsmfig __P((fsmfig_list *Figure));
  extern           void  freefsmstate __P((fsmstate_list *State));
  extern           void  freefsmstack __P((fsmstack_list *Stack));
  extern           void  freefsmtrans __P((fsmtrans_list *Trans));
  extern           void  freefsmlocout __P((fsmlocout_list *Locout));
  extern           void  freefsmout __P((fsmout_list *Output));
  extern           void  freefsmin __P((fsmin_list *Input));
  extern           void  freefsmpragma __P((fsmpragma_list *Pragma));
  extern           void  freefsmport __P((fsmport_list *Port));

/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern   fsmfig_list * addfsmfig __P((char *Name));
  extern fsmstate_list * addfsmstate __P((fsmfig_list *Figure, char *Name));
  extern fsmstack_list * addfsmstack __P((fsmfig_list *Figure, char Control, fsmstate_list *Current, fsmstate_list *Return, ablexpr *Condition));
  extern fsmtrans_list * addfsmtrans __P((fsmfig_list *Figure, fsmstate_list *StateFrom, fsmstate_list *StateTo, ablexpr *Condition));
  extern fsmlocout_list * addfsmlocout __P((fsmstate_list *State, fsmout_list *Output, ablexpr *Equation, ablexpr *EquationDC));
  extern   fsmout_list * addfsmout __P((fsmfig_list *Figure, char *Name));
  extern    fsmin_list * addfsmin __P((fsmfig_list *Figure, char *Name));
  extern  fsmpragma_list * addfsmpragma __P((fsmfig_list *Figure, char *Type, char *Name, char *Value ));
  extern  fsmport_list * addfsmport __P((fsmfig_list *Figure, char *Name, char Dir, char Type));

/*------------------------------------------------------\
|                                                       |
|                      Del Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern            int  delfsmfig __P((char *Name));
  extern            int  delfsmstate __P((fsmfig_list *Figure, fsmstate_list *State));
  extern            int  delfsmstack __P((fsmfig_list *Figure, fsmstack_list *Stack));
  extern            int  delfsmtrans __P((fsmfig_list *Figure, fsmtrans_list *Trans));
  extern            int  delfsmlocout __P((fsmstate_list *State, fsmlocout_list *Locout));

/*------------------------------------------------------\
|                                                       |
|                      Search Functions                 |
|                                                       |
\------------------------------------------------------*/

  extern   fsmfig_list * searchfsmfig __P((char *Name));
  extern fsmstate_list * searchfsmstate __P((fsmfig_list *Figure, char *Name));
  extern fsmstack_list * searchfsmstack __P((char Control, fsmstate_list *Current, fsmstate_list *Return));
  extern fsmtrans_list * searchfsmtrans __P((fsmfig_list *Figure, fsmstate_list *StateFrom, fsmstate_list *StateTo));
  extern fsmlocout_list * searchfsmlocout __P((fsmstate_list *State, fsmout_list *Out));
  extern   fsmout_list * searchfsmout __P((fsmfig_list *Figure, char *Name));
  extern    fsmin_list * searchfsmin __P((fsmfig_list *Figure, char *Name));
  extern  fsmport_list * searchfsmport __P((fsmfig_list *Figure, char *Name));

/*------------------------------------------------------\
|                                                       |
|                        Abl Functions                  |
|                                                       |
\------------------------------------------------------*/

  extern           void  simpfsmablexpr __P((fsmfig_list *FsmFigure));

/*------------------------------------------------------\
|                                                       |
|                       Bdd Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  makefsmbddcircuit __P((fsmfig_list *FsmFigure, bddsystem *BddSystem, int Order));
  extern           void  makefsmbddnode __P((fsmfig_list *FsmFigure));

  extern           void  convertfsmbddnodeabl __P((fsmfig_list *FsmFigure));

/*------------------------------------------------------\
|                                                       |
|                     View Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern           void  viewfsmfig __P((fsmfig_list *Figure));
  extern           void  viewfsmstate __P((fsmstate_list *State));
  extern           void  viewfsmstack __P((fsmstack_list *Stack));
  extern           void  viewfsmtrans __P((fsmtrans_list *Trans));
  extern           void  viewfsmlocout __P((fsmlocout_list *Locout));
  extern           void  viewfsmout __P((fsmout_list *Output));
  extern           void  viewfsmin __P((fsmin_list *Input));
  extern           void  viewfsmport __P((fsmport_list *Port));

# endif 
