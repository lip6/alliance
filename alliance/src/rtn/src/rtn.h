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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RTL                           |
|                                                             |
| File    :                   rtn101.h                        |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RTN_101_H
# define RTN_101_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Rtl Direction                     |
|                                                             |
\------------------------------------------------------------*/

# define RTL_DIR_IN         0
# define RTL_DIR_OUT        1
# define RTL_DIR_INOUT      2
# define RTL_MAX_DIR_TYPE   3

/*------------------------------------------------------------\
|                                                             |
|                           Rtl Kind                          |
|                                                             |
\------------------------------------------------------------*/

# define RTL_KIND_NONE      0
# define RTL_KIND_BUS       1
# define RTL_KIND_REGISTER  2
# define RTL_MAX_KIND_TYPE  3

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Declar Type                    |
|                                                             |
\------------------------------------------------------------*/

# define RTL_DECLAR_PORT       0
# define RTL_DECLAR_SIGNAL     1
# define RTL_DECLAR_CONSTANT   2
# define RTL_DECLAR_GENERIC    3
# define RTL_MAX_DECLAR_TYPE   4

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Assgin Type                      |
|                                                             |
\------------------------------------------------------------*/

# define RTL_ASG_COMBINATORIAL      0
# define RTL_ASG_MULTIPLEXOR        1
# define RTL_ASG_CONDITIONAL        2
# define RTL_ASG_REGISTER           3
# define RTL_ASG_TRISTATE           4
# define RTL_ASG_PULL_UP            5
# define RTL_ASG_PULL_DOWN          6
# define RTL_MAX_ASG_TYPE           7

/*------------------------------------------------------------\
|                                                             |
|                   Rtl Assgin Register Type                  |
|                                                             |
\------------------------------------------------------------*/

# define RTL_ASG_REGISTER_NONE      0
# define RTL_ASG_REGISTER_SYNC      1
# define RTL_ASG_REGISTER_ASYNC     2
# define RTL_ASG_REGISTER_MIXED     3
# define RTL_MAX_ASG_REGISTER_TYPE  4

/*------------------------------------------------------------\
|                                                             |
|                        Rtl Bi Vex Type                      |
|                                                             |
\------------------------------------------------------------*/

# define RTL_BIVEX_NONE               0
# define RTL_BIVEX_FALLING_EDGE       1
# define RTL_BIVEX_RISING_EDGE        2
# define RTL_BIVEX_ASYNC_RESET        3
# define RTL_BIVEX_ASYNC_SET          4
# define RTL_BIVEX_ASYNC_WEN          5
# define RTL_BIVEX_SYNC_RESET         6
# define RTL_BIVEX_SYNC_SET           7
# define RTL_BIVEX_SYNC_WEN           8
# define RTL_BIVEX_MULTIPLEXOR        9
# define RTL_BIVEX_CONDITIONAL       10
# define RTL_MAX_BIVEX_TYPE          11

/*------------------------------------------------------------\
|                                                             |
|                           Rtl Fsm Type                      |
|                                                             |
\------------------------------------------------------------*/

# define RTL_FSM_STANDARD           0
# define RTL_FSM_REGOUT             1
# define RTL_MAX_FSM_TYPE           2

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define GetRtlFsmTrans( C ) ((rtlfsmtrans_list *)((C)->DATA))

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Rtl Symbol                         |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rtlsym
  {
    char                  *NAME;
    struct rtldecl_list   *DECL;
    short                  INDEX;
    unsigned char          INIT;
    unsigned char          DRIVE;
    unsigned char          EFFEC;
    unsigned char          EVENT;
    long                   FLAGS;
    void                  *USER;

  } rtlsym;

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Declaration                    |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rtldecl_list
  {
    struct rtldecl_list  *NEXT;
    vexexpr              *VEX_ATOM;
    vexexpr              *VEX_INIT;
    rtlsym               *DECL_SYM;
    unsigned char         DIR;
    unsigned char         TYPE;
    unsigned char         KIND;
    unsigned char         BASE;
    long                  FLAGS;
    void                 *USER;

  } rtldecl_list;

/*------------------------------------------------------------\
|                                                             |
|                           Rtl Bi Vex                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rtlbivex_list 
  {
    struct rtlbivex_list *NEXT;
    unsigned char         TYPE;
    vexexpr              *VEX_COND;
    vexexpr              *VEX_DATA;
    long                  FLAGS;
    void                 *USER;

  } rtlbivex_list;

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Assignment                        |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rtlasg_list
  {
    struct rtlasg_list    *NEXT;
    struct rtlasg_list   **PREV;
    vexexpr               *VEX_ATOM;
    vexexpr               *VEX_DATA;
    rtlbivex_list         *BIVEX;
    rtlbivex_list        **LAST_BIVEX;
    unsigned char          TYPE;
    unsigned char          REG_TYPE;
    long                   FLAGS;
    void                  *USER;

  } rtlasg_list;

/*------------------------------------------------------------\
|                                                             |
|                       Rtl Fsm Assignment                    |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rtlfsmasg_list
  {
    struct rtlfsmasg_list    *NEXT;
    vexexpr                  *VEX_ATOM;
    vexexpr                  *VEX_DATA;
    long                      FLAGS;
    void                     *USER;

  } rtlfsmasg_list;

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Fsm State                      |
|                                                             |
\------------------------------------------------------------*/

 typedef struct rtlfsmstate_list
 {
   struct rtlfsmstate_list *NEXT;
   char                    *NAME;
   chain_list              *FROM;
   chain_list              *TO;
   rtlfsmasg_list          *ASSIGN;
   long                     FLAGS;
   void                    *USER;
   
 } rtlfsmstate_list;

/*------------------------------------------------------------\
|                                                             |
|                          Rtl Fsm Transition                 |
|                                                             |
\------------------------------------------------------------*/

 typedef struct rtlfsmtrans_list
 {
   struct rtlfsmtrans_list *NEXT;
   struct rtlfsmstate_list *FROM;
   struct rtlfsmstate_list *TO;
   rtlfsmasg_list          *ASSIGN;
   vexexpr                 *VEX_COND;
   long                     FLAGS;
   void                    *USER;

 } rtlfsmtrans_list;

/*------------------------------------------------------------\
|                                                             |
|                           Rtl Fsm                           |
|                                                             |
\------------------------------------------------------------*/

 typedef struct rtlfsm_list
 {
   struct rtlfsm_list    *NEXT;
   char                  *NAME;
   rtlfsmstate_list      *STATE;
   rtlfsmtrans_list      *TRANS;
   authtable             *HASH_STATE;
   rtlbivex_list         *BIVEX;
   rtlbivex_list        **LAST_BIVEX;
   unsigned char          TYPE;
   long                   FLAGS;
   void                  *USER;

 } rtlfsm_list;

/*------------------------------------------------------\
|                                                       |
|                        Rtl Model                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct rtlgen_list 
  {
    struct rtlgen_list *NEXT;
    vexexpr            *VEX_ATOM;
    vexexpr            *VEX_EXPR;
    unsigned char       BASE;
    long                FLAGS;
    void               *USER;

  } rtlgen_list;

  typedef struct rtlport_list 
  {
    struct rtlport_list  *NEXT;
    vexexpr              *VEX_ATOM;
    unsigned char         DIR;
    unsigned char         BASE;
    long                  FLAGS;
    void                 *USER;

  } rtlport_list;

  typedef struct rtlmod_list 
  {
    struct rtlmod_list *NEXT;
    char               *NAME;
    rtlport_list       *PORT;
    rtlgen_list        *GENERIC;
    long                FLAGS;
    void               *USER;

  } rtlmod_list;

/*------------------------------------------------------\
|                                                       |
|                        Rtl Instance                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct rtlmap_list 
  {
    struct rtlmap_list *NEXT;
    vexexpr            *VEX_FORMAL;
    vexexpr            *VEX_ACTUAL;
    long                FLAGS;
    void               *USER;

  } rtlmap_list;

  typedef struct rtlins_list 
  {
    struct rtlins_list *NEXT;
    char               *NAME;
    rtlmod_list        *MODEL;
    rtlmap_list        *MAP;
    rtlgen_list        *GENERIC;
    long                FLAGS;
    void               *USER;

  } rtlins_list;


/*------------------------------------------------------------\
|                                                             |
|                       Rtl Figure                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct rtlfig_list
  {
    struct rtlfig_list *NEXT;
    char               *NAME;
    rtlasg_list        *ASSIGN;
    rtlfsm_list        *FSM;
    rtlmod_list        *MODEL;
    rtlins_list        *INSTANCE;
    rtldecl_list       *DECLAR[ RTL_MAX_DECLAR_TYPE ];
    authtable          *HASH_DECLAR;
    authtable          *HASH_MODEL;
    authtable          *HASH_INSTANCE;
    long                FLAGS;
    void               *USER;

  } rtlfig_list;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern rtlfig_list *HEAD_RTLFIG;

  extern char        *RTL_DECLAR_TYPE[ RTL_MAX_DECLAR_TYPE ];
  extern char        *RTL_KIND_TYPE[ RTL_MAX_KIND_TYPE ];
  extern char        *RTL_DIR_TYPE[ RTL_MAX_DIR_TYPE ];
  extern char        *RTL_BIVEX_TYPE[ RTL_MAX_BIVEX_TYPE ];
  extern char        *RTL_ASG_TYPE[ RTL_MAX_ASG_TYPE ];
  extern char        *RTL_ASG_REGISTER_TYPE[ RTL_MAX_ASG_REGISTER_TYPE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Alloc Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern   rtlfig_list *allocrtlfig __P(());
  extern  rtldecl_list *allocrtldecl __P(());
  extern        rtlsym *allocrtlsym __P((unsigned int Width));
  extern   rtlasg_list *allocrtlasg __P(());
  extern rtlbivex_list *allocrtlbivex __P(());
  extern   rtlfsm_list *allocrtlfsm __P(());
  extern rtlfsmtrans_list *allocrtlfsmtrans __P(());
  extern rtlfsmstate_list *allocrtlfsmstate __P(());
  extern rtlfsmasg_list *allocrtlfsmasg __P(());

  extern  rtlmod_list  *allocrtlmod __P(());
  extern  rtlins_list  *allocrtlins __P(());
  extern  rtlport_list *allocrtlport __P(());
  extern  rtlmap_list  *allocrtlmap __P(());
  extern  rtlgen_list  *allocrtlgen __P(());

/*------------------------------------------------------------\
|                                                             |
|                           Free Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern void  freertlfig __P((rtlfig_list *Figure));
  extern void  freertlasg __P((rtlasg_list *Asg));
  extern void  freertlbivex __P((rtlbivex_list *BiVex));
  extern void  freertldecl __P((rtldecl_list *Decl));
  extern void  freertlsym __P((rtlsym *Sym));
  extern void  freertlfsmasg __P((rtlfsmasg_list *FsmAsg));
  extern void  freertlfsmstate __P((rtlfsmstate_list *fsmstate));
  extern void  freertlfsmtrans __P((rtlfsmtrans_list *fsmtrans));
  extern void  freertlfsm __P((rtlfsm_list *Fsm));

  extern void  freertlmod __P((rtlmod_list *Model));
  extern void  freertlins __P((rtlins_list *Instance));
  extern void  freertlport __P((rtlport_list *Port));
  extern void  freertlmap __P((rtlmap_list *Map));
  extern void  freertlgen __P((rtlgen_list *Generic));

/*------------------------------------------------------------\
|                                                             |
|                            Add Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern   rtlfig_list *addrtlfig __P((char *Name));
  extern  rtldecl_list *addrtldecl __P((rtlfig_list *Figure, vexexpr *Atom, int Type));
  extern   rtlasg_list *addrtlasg __P((rtlfig_list *Figure, vexexpr *VexAtom, int Type));
  extern   rtlasg_list *addrtlasgafter __P((rtlfig_list *Figure, rtlasg_list *PrevAssign, vexexpr *VexAtom, int Type));
  extern rtlbivex_list *addrtlasgbivex __P((rtlfig_list *Figure, rtlasg_list *Assign, vexexpr *VexCond, vexexpr *VexData, int Type));

  extern rtlfsmasg_list *addrtlfsmstateasg __P((rtlfig_list *Figure, rtlfsm_list *Fsm, rtlfsmstate_list *State, vexexpr *VexAtom, vexexpr *VexData));
  extern rtlfsmasg_list *addrtlfsmtransasg __P((rtlfig_list *Figure, rtlfsm_list *Fsm, rtlfsmtrans_list *Trans, vexexpr *VexAtom, vexexpr *VexData));
  extern rtlfsmtrans_list *addrtlfsmtrans __P((rtlfig_list *Figure, rtlfsm_list *Fsm, rtlfsmstate_list *StateFrom, rtlfsmstate_list *StateTo, vexexpr *VexCond));
  extern rtlfsmstate_list *addrtlfsmstate __P((rtlfig_list *Figure, rtlfsm_list *Fsm, char *Name));
  extern   rtlfsm_list *addrtlfsm __P((rtlfig_list *Figure, char *Name));
  extern rtlbivex_list *addrtlfsmbivex __P((rtlfig_list *Figure, rtlfsm_list *Fsm, vexexpr *VexCond, vexexpr *VexData, int Type));

  extern  rtlmod_list  *addrtlmod __P((rtlfig_list *Figure, char *Name));
  extern  rtlport_list *addrtlmodport __P((rtlfig_list *Figure, rtlmod_list *Model, vexexpr *Atom, unsigned char Base, unsigned char Dir ));
  extern  rtlgen_list *addrtlmodgen __P((rtlfig_list *Figure, rtlmod_list *Model, vexexpr *Atom, vexexpr *Expr, unsigned char Base ));
  extern  rtlins_list  *addrtlins __P((rtlfig_list *Figure, char *Name, rtlmod_list *Model));
  extern  rtlmap_list  *addrtlinsmap __P((rtlfig_list *Figure, rtlins_list *Instance, vexexpr *Formal, vexexpr *Actual ));
  extern  rtlgen_list  *addrtlinsgen __P((rtlfig_list *Figure, rtlins_list *Instance, vexexpr *Atom, vexexpr *Expr ));

/*------------------------------------------------------------\
|                                                             |
|                            Del Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern            int  delrtlfig __P((char *Name));
  extern            int  delrtlasg __P((rtlfig_list *Figure, rtlasg_list *Assign));

/*------------------------------------------------------------\
|                                                             |
|                         Search Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern   rtlfig_list *searchrtlfig __P((char *Name));
  extern  rtldecl_list *searchrtldecl __P((rtlfig_list *Figure, char *Name));
  extern        rtlsym *searchrtlsym __P((rtlfig_list *Figure, char *Name, short Index));
  extern rtlfsmstate_list *searchrtlfsmstate __P((rtlfig_list *Figure, rtlfsm_list *Fsm, char *Name));

  extern  rtlmod_list  *searchrtlmod __P((rtlfig_list *Figure, char *Name));
  extern  rtlins_list  *searchrtlins __P((rtlfig_list *Figure, char *Name));
  extern  rtlport_list *searchrtlmodport __P((rtlfig_list *Figure, rtlmod_list *Model, char *Name));

/*------------------------------------------------------------\
|                                                             |
|                            Get Functions                    |
|                                                             |
\------------------------------------------------------------*/

  extern  rtlsym *getrtlsymdecl __P((rtldecl_list *Declar, short Index));
  extern  rtlsym *getrtlsymdeclpos __P((rtldecl_list *Declar, int Position));

/*------------------------------------------------------------\
|                                                             |
|                          View Functions                     |
|                                                             |
\------------------------------------------------------------*/

  extern void  viewrtlsym __P((rtlsym *Symbol));
  extern void  viewrtldecl __P((rtldecl_list *Decl));
  extern void  viewrtlbivex __P((rtlbivex_list *BiVex));
  extern void  viewrtlasg __P((rtlasg_list *Asg));
  extern void  viewrtlfig __P((rtlfig_list *Figure));
  extern void  viewrtlfsm __P((rtlfsm_list *Fsm));

  extern void  viewrtlins __P((rtlins_list *Instance));
  extern void  viewrtlmod __P((rtlmod_list *Model));
  extern void  viewrtlport __P((rtlport_list *Port));
  extern void  viewrtlmap __P((rtlmap_list *Map));
  extern void  viewrtlgen __P((rtlgen_list *Generic));

# endif
