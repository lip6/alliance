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
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for VPN         |
|                                                       |
|  Date    :            17.01.2000                      |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef VPN_105_H
# define VPN_105_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                    Vpn Attribute Type                 |
|                                                       |
\------------------------------------------------------*/

# define VPN_ATTR_NONE           0
# define VPN_ATTR_ENTITY         1
# define VPN_ATTR_ARCHITECTURE   2
# define VPN_ATTR_PACKAGE        3
# define VPN_ATTR_FUNCTION       4
# define VPN_ATTR_PROCEDURE      5
# define VPN_ATTR_SUBTYPE        6
# define VPN_ATTR_CONSTANT       7
# define VPN_ATTR_VARIABLE       8
# define VPN_ATTR_SIGNAL         9
# define VPN_ATTR_LABEL         10
# define VPN_ATTR_TYPE          11
# define VPN_ATTR_CONFIGURATION 12
# define VPN_ATTR_COMPONENT     13
# define VPN_MAX_ATTR_TYPE      14

/*------------------------------------------------------\
|                                                       |
|                    Vpn Father Type                    |
|                                                       |
\------------------------------------------------------*/

# define VPN_FATHER_PROCESS          0
# define VPN_FATHER_FUNCTION         1
# define VPN_MAX_FATHER_TYPE         2

/*------------------------------------------------------\
|                                                       |
|                    Vpn Trans Type                     |
|                                                       |
\------------------------------------------------------*/

# define VPN_TRANS_UNKNOWN           0
# define VPN_TRANS_ACT_EXEC          1
# define VPN_TRANS_INF_WAIT          2
# define VPN_TRANS_SUP_WAIT          3
# define VPN_TRANS_GUARDED           4
# define VPN_TRANS_IMMEDIATE         5
# define VPN_TRANS_ACT_GUARDED       6
# define VPN_TRANS_ACT_EXEC_CONC     7
# define VPN_TRANS_ACT_GUARDED_CONC  8
# define VPN_MAX_TRANS_TYPE          9

/*------------------------------------------------------\
|                                                       |
|                    Vpn Place Type                     |
|                                                       |
\------------------------------------------------------*/

# define VPN_PLACE_UNKNOWN           0
# define VPN_PLACE_ASSIGN            1
# define VPN_PLACE_IF                2
# define VPN_PLACE_END_IF            3
# define VPN_PLACE_CASE              4
# define VPN_PLACE_END_CASE          5
# define VPN_PLACE_WAIT              6
# define VPN_PLACE_UNTIL             7
# define VPN_PLACE_UNTIL_TEST        8
# define VPN_PLACE_END_UNTIL         9
# define VPN_PLACE_WHILE            10
# define VPN_PLACE_WHILE_TEST       11
# define VPN_PLACE_END_WHILE        12
# define VPN_PLACE_FOR              13
# define VPN_PLACE_FOR_LOOP         14
# define VPN_PLACE_FOR_TEST         15
# define VPN_PLACE_FOR_INC          16
# define VPN_PLACE_END_FOR          17
# define VPN_PLACE_NEXT             18
# define VPN_PLACE_EXIT             19
# define VPN_PLACE_CALL             20
# define VPN_PLACE_RETURN           21
# define VPN_PLACE_PROCESS          22
# define VPN_PLACE_FUNCTION         23
# define VPN_PLACE_LOOP             24
# define VPN_PLACE_END_LOOP         25
# define VPN_PLACE_END_EXIT         26
# define VPN_PLACE_END_NEXT         27
# define VPN_PLACE_GOTO             28
# define VPN_PLACE_LABEL            29
# define VPN_PLACE_DO               30
# define VPN_PLACE_DO_TEST          31
# define VPN_PLACE_DO_WHILE         32
# define VPN_PLACE_END_DO           33
# define VPN_PLACE_SWITCH           34
# define VPN_PLACE_END_SWITCH       35
# define VPN_PLACE_C_CASE           36
# define VPN_PLACE_BREAK            37
# define VPN_PLACE_C_RETURN         38
# define VPN_MAX_PLACE_TYPE         39

/*------------------------------------------------------\
|                                                       |
|                      Vpn Argument Class               |
|                                                       |
\------------------------------------------------------*/

# define VPN_CLASS_NONE       0
# define VPN_CLASS_CONSTANT   1
# define VPN_CLASS_VARIABLE   2
# define VPN_CLASS_SIGNAL     3
# define VPN_MAX_CLASS_TYPE   4

/*------------------------------------------------------\
|                                                       |
|                      Vpn Direction                    |
|                                                       |
\------------------------------------------------------*/

# define VPN_DIR_IN         0
# define VPN_DIR_OUT        1
# define VPN_DIR_INOUT      2
# define VPN_MAX_DIR_TYPE   3

/*------------------------------------------------------\
|                                                       |
|                      Vpn Kind                         |
|                                                       |
\------------------------------------------------------*/

# define VPN_KIND_NONE       0
# define VPN_KIND_BUS        1
# define VPN_KIND_REGISTER   2
# define VPN_MAX_KIND_TYPE   3

/*------------------------------------------------------\
|                                                       |
|                       Vpn Declar Type                 |
|                                                       |
\------------------------------------------------------*/

# define VPN_DECLAR_ALL             0
# define VPN_DECLAR_PORT            1
# define VPN_DECLAR_SIGNAL          2
# define VPN_DECLAR_VARIABLE        3
# define VPN_DECLAR_DEFINE          4
# define VPN_DECLAR_ARGUMENT        5
# define VPN_DECLAR_CONSTANT        6
# define VPN_DECLAR_GENERIC         7
# define VPN_MAX_DECLAR_TYPE        8

/*------------------------------------------------------\
|                                                       |
|                    Vpn Action Type                    |
|                                                       |
\------------------------------------------------------*/

# define VPN_ACT_UNKNOWN          0
# define VPN_ACT_ASG_PORT         1
# define VPN_ACT_ASG_SIGNAL       2
# define VPN_ACT_ASG_VARIABLE     3
# define VPN_ACT_ASG_DEFINE       4
# define VPN_ACT_ASG_ARG_SIG      5
# define VPN_ACT_ASG_ARG_VAR      6
# define VPN_ACT_CALL             7
# define VPN_ACT_RETURN           8
# define VPN_MAX_ACT_TYPE         9

/*------------------------------------------------------\
|                                                       |
|                       Vpn Arc Type                    |
|                                                       |
\------------------------------------------------------*/

# define VPN_ARC_PLACE_TRANS   0
# define VPN_ARC_TRANS_PLACE   1
# define VPN_MAX_ARC_TYPE      2

/*------------------------------------------------------\
|                                                       |
|                      Vpn Process Type                 |
|                                                       |
\------------------------------------------------------*/

# define VPN_PROCESS_SEQUENTIAL_MASK       0x01
# define VPN_PROCESS_WITHSELECT_MASK       0x02
# define VPN_PROCESS_GUARDED_MASK          0x04
# define VPN_PROCESS_CONDITIONNAL_MASK     0x08

/*------------------------------------------------------\
|                                                       |
|                        Macros                         |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                  Vpn Is Process Type                  |
|                                                       |
\------------------------------------------------------*/

# define IsVpnProcSequential( P )      ( (P)->TYPE & VPN_PROCESS_SEQUENTIAL_MASK     )
# define IsVpnProcWithSelect( P )      ( (P)->TYPE & VPN_PROCESS_WITHSELECT_MASK     )
# define IsVpnProcConditionnal( P )    ( (P)->TYPE & VPN_PROCESS_CONDITIONNAL_MASK   )
# define IsVpnProcGuarded( P )         ( (P)->TYPE & VPN_PROCESS_GUARDED_MASK        )
                                       
# define SetVpnProcSequential( P )     ( (P)->TYPE |= VPN_PROCESS_SEQUENTIAL_MASK    )
# define SetVpnProcWithSelect( P )     ( (P)->TYPE |= VPN_PROCESS_WITHSELECT_MASK    )
# define SetVpnProcConditionnal( P )   ( (P)->TYPE |= VPN_PROCESS_CONDITIONNAL_MASK  )
# define SetVpnProcGuarded( P )        ( (P)->TYPE |= VPN_PROCESS_GUARDED_MASK       )

# define ClearVpnProcSequential( P )   ( (P)->TYPE &= ~VPN_PROCESS_SEQUENTIAL_MASK   )
# define ClearVpnProcWithSelect( P )   ( (P)->TYPE &= ~VPN_PROCESS_WITHSELECT_MASK   )
# define ClearVpnProcConditionnal( P ) ( (P)->TYPE &= ~VPN_PROCESS_CONDITIONNAL_MASK )
# define ClearVpnProcGuarded( P )      ( (P)->TYPE &= ~VPN_PROCESS_GUARDED_MASK      )

/*------------------------------------------------------\
|                                                       |
|                      Vpn Get Symbol                   |
|                                                       |
\------------------------------------------------------*/

# define GetVpnSymbol( C ) ((vpnsym       *)( (C)->DATA ))
# define GetVpnDeclar( C ) ((vpndecl_list *)( (C)->DATA ))

/*------------------------------------------------------\
|                                                       |
|                        Vpn Get Arc                    |
|                                                       |
\------------------------------------------------------*/

# define GetVpnArc( C ) \
   ((vpnarc *)( (C)->DATA ))

# define GetVpnArcTargetPlace( A ) \
   ((vpnplace_list *)( (vpnarc *)(A)->TARGET ))

# define GetVpnArcSourcePlace( A ) \
   ((vpnplace_list *)( (vpnarc *)(A)->SOURCE ))

# define GetVpnArcTargetTrans( A ) \
   ((vpntrans_list *)( (vpnarc *)(A)->TARGET ))

# define GetVpnArcSourceTrans( A ) \
   ((vpntrans_list *)( (vpnarc *)(A)->SOURCE ))

/*------------------------------------------------------\
|                                                       |
|                        Vpn Add Arc                    |
|                                                       |
\------------------------------------------------------*/

# define addvpnarctrans( F, T, P ) \
  (addvpnarc( (F), (T), (P), VPN_ARC_TRANS_PLACE ))

# define addvpnarcplace( F, P, T ) \
  (addvpnarc( (F), (P), (T), VPN_ARC_PLACE_TRANS ))

/*------------------------------------------------------\
|                                                       |
|                        Vpn Add Action                 |
|                                                       |
\------------------------------------------------------*/

# define addvpnactcall( F, T, E )  \
  (addvpnact( (F), (T), (vexexpr *)0, (E), VPN_ACT_CALL ))

# define addvpnactreturn( F, T, E ) \
  (addvpnact( (F), (T), (vexexpr *)0, (E), VPN_ACT_RETURN ))

# define addvpnactcallafter( F, T, P, E )  \
  (addvpnactafter( (F), (T), (P), (vexexpr *)0, (E), VPN_ACT_CALL ))

# define addvpnactreturnafter( F, T, P, E ) \
  (addvpnactafter( (F), (T), (P), (vexexpr *)0, (E), VPN_ACT_RETURN ))

# define addvpnactcallbefore( F, T, P, E )  \
  (addvpnactbefore( (F), (T), (P), (vexexpr *)0, (E), VPN_ACT_CALL ))

# define addvpnactreturnbefore( F, T, P, E ) \
  (addvpnactafter( (F), (T), (P), (vexexpr *)0, (E), VPN_ACT_RETURN ))

# define addvpnactcallfirst( F, T, E )  \
  (addvpnactfirst( (F), (T), (vexexpr *)0, (E), VPN_ACT_CALL ))

# define addvpnactreturnfirst( F, T, E ) \
  (addvpnactfirst( (F), (T), (vexexpr *)0, (E), VPN_ACT_RETURN ))


/*------------------------------------------------------\
|                                                       |
|                        Vpn Get Number                 |
|                                                       |
\------------------------------------------------------*/

# define GetVpnNumProc( F )     ((F)->HASH_PROCESS->NUMBER_ELEM)
# define GetVpnNumFunc( F )     ((F)->HASH_FUNCTION->NUMBER_ELEM)
# define GetVpnNumTrans( F )    ((F)->HASH_TRANS->NUMBER_ELEM)
# define GetVpnNumPlace( F )    ((F)->HASH_PLACE->NUMBER_ELEM)
# define GetVpnNumArc( F )      ((F)->HASH_ARC->NUMBER_ELEM)

# define GetVpnNumDecl( F )     ((F)->HASH_DECLAR[ VPN_DECLAR_ALL      ]->NUMBER_ELEM)
# define GetVpnNumDeclPort( F ) ((F)->HASH_DECLAR[ VPN_DECLAR_PORT     ]->NUMBER_ELEM)
# define GetVpnNumDeclSig( F )  ((F)->HASH_DECLAR[ VPN_DECLAR_SIGNAL   ]->NUMBER_ELEM)
# define GetVpnNumDeclVar( F )  ((F)->HASH_DECLAR[ VPN_DECLAR_VARIABLE ]->NUMBER_ELEM)
# define GetVpnNumDeclCst( F )  ((F)->HASH_DECLAR[ VPN_DECLAR_CONSTANT ]->NUMBER_ELEM)
# define GetVpnNumDeclDef( F )  ((F)->HASH_DECLAR[ VPN_DECLAR_DEFINE   ]->NUMBER_ELEM)
# define GetVpnNumDeclArg( F )  ((F)->HASH_DECLAR[ VPN_DECLAR_ARGUMENT ]->NUMBER_ELEM)

/*------------------------------------------------------\
|                                                       |
|                    Vpn Search Symbol                  |
|                                                       |
\------------------------------------------------------*/

# define searchvpnsymport( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_PORT ))
# define searchvpnsymsig( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_SIGNAL ))
# define searchvpnsymvar( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_VARIABLE ))
# define searchvpnsymcst( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_CONSTANT ))
# define searchvpnsymdef( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_DEFINE ))
# define searchvpnsymarg( F, N, I ) \
    (searchvpnsym( (F), (N) (I), VPN_DECLAR_ARGUMENT ))
# define searchvpnsymgen( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_GENERIC ))
# define searchvpnsymall( F, N, I ) \
    (searchvpnsym( (F), (N), (I), VPN_DECLAR_ALL ))

/*------------------------------------------------------\
|                                                       |
|                    Vpn Search Declar                  |
|                                                       |
\------------------------------------------------------*/

# define searchvpndeclport( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_PORT ))
# define searchvpndeclsig( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_SIGNAL ))
# define searchvpndeclvar( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_VARIABLE ))
# define searchvpndeclcst( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_CONSTANT ))
# define searchvpndecldef( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_DEFINE ))
# define searchvpndeclarg( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_ARGUMENT ))
# define searchvpndeclgen( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_GENERIC ))
# define searchvpndeclall( F, N ) \
    (searchvpndecl( (F), (N), VPN_DECLAR_ALL ))


/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                        Vpn Line                       |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnline_list
  {
    struct vpnline_list *NEXT;
    char                *NAME;
    long                 LINE;

  } vpnline_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Attribute                  |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnattr
  {
    struct vpnattr        *NEXT;
    char                  *NAME;
    char                  *ENTITY;
    char                  *VALUE;
    unsigned char          TYPE;
    vpnline_list          *LINE;
    long                   FLAGS;
    void                  *USER;

  } vpnattr_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Symbol                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnsym
  {
    char                  *NAME;
    struct vpndecl_list   *DECL;
    short                  INDEX;
    unsigned char          INIT;
    unsigned char          DRIVE;
    unsigned char          EFFEC;
    unsigned char          EVENT;
    long                   FLAGS;
    void                  *USER;

  } vpnsym;

/*------------------------------------------------------\
|                                                       |
|                     Vpn Declaration                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpndecl_list
  {
    struct vpndecl_list  *NEXT;
    struct vpndecl_list **PREV;
    vexexpr              *VEX_ATOM;
    void                 *BDD_ATOM;
    vexexpr              *VEX_INIT;
    void                 *BDD_INIT;
    vpnsym               *DECL_SYM;
    unsigned char         DIR;
    unsigned char         TYPE;
    unsigned char         KIND;
    unsigned char         CLASS;
    unsigned char         BASE;
    vpnline_list         *LINE;
    long                  FLAGS;
    void                 *USER;

  } vpndecl_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Action                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnact_list
  {
    struct vpnact_list    *NEXT;
    struct vpnact_list   **PREV;
    vexexpr               *VEX_ATOM;
    void                  *BDD_ATOM;
    vexexpr               *VEX_EXPR;
    void                  *BDD_EXPR;
    unsigned char          TYPE;
    vpnline_list          *LINE;
    long                   FLAGS;
    void                  *USER;

  } vpnact_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Arc                        |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnarc
  {
    void                  *SOURCE;
    void                  *TARGET;
    unsigned char          TYPE;
    long                   FLAGS;
    void                  *USER;

  } vpnarc;
  
/*------------------------------------------------------\
|                                                       |
|                        Vpn Place                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnplace_list
  {
    struct vpnplace_list  *NEXT;
    struct vpnplace_list **PREV;
    char                  *NAME;
    unsigned char          FATHER;
    unsigned char          TYPE;
    unsigned short         TOKEN;
    unsigned short         NUMBER_IN;
    unsigned short         NUMBER_OUT;
    chain_list            *TRANS_IN;
    chain_list            *TRANS_OUT;
    struct vpnplace_list  *LINK;
    vpnline_list          *LINE;
    long                   FLAGS;
    void                  *USER;

  } vpnplace_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Trans                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpntrans_list
  {
    struct vpntrans_list  *NEXT;
    struct vpntrans_list **PREV;
    char                  *NAME;
    unsigned char          FATHER;
    unsigned char          TYPE;
    chain_list            *WAIT_SYM;
    unsigned short         NUMBER_IN;
    unsigned short         NUMBER_OUT;
    chain_list            *PLACE_IN;
    chain_list            *PLACE_OUT;
    vpnact_list           *ACT;
    vpnact_list          **LAST_ACT;
    vexexpr               *VEX_GUARD;
    void                  *BDD_GUARD;
    long                   FLAGS;
    void                  *USER;

  } vpntrans_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Process                    |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnproc_list
  {
    struct vpnproc_list  *NEXT;
    struct vpnproc_list **PREV;
    char                 *NAME;
    unsigned char         TYPE;
    vpntrans_list        *FIRST;
    vpntrans_list        *ELABO;
    vpnplace_list        *PLACE;
    vpntrans_list        *TRANS;
    chain_list           *VAR_DECL;
    chain_list           *DEF_DECL;
    vpnline_list         *LINE;
    long                  FLAGS;
    void                 *USER;

  } vpnproc_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Function                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnfunc_list
  {
    struct vpnfunc_list  *NEXT;
    struct vpnfunc_list **PREV;
    char                 *NAME;
    unsigned char         TYPE;
    vpntrans_list        *FIRST;
    vpntrans_list        *ELABO;
    vpnplace_list        *PLACE;
    vpntrans_list        *TRANS;
    vexexpr              *VEX_RET;
    chain_list           *ARG_DECL;
    chain_list          **LAST_ARG;
    chain_list           *VAR_DECL;
    chain_list           *DEF_DECL;
    vpnline_list         *LINE;
    long                  FLAGS;
    void                 *USER;

  } vpnfunc_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Instance                   |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnmap_list 
  {
    struct vpnmap_list *NEXT;
    vexexpr            *VEX_FORMAL;
    vexexpr            *VEX_ACTUAL;
    vpnline_list       *LINE;
    long                FLAGS;
    void               *USER;

  } vpnmap_list;

  typedef struct vpngen_list 
  {
    struct vpngen_list *NEXT;
    vexexpr            *VEX_ATOM;
    vexexpr            *VEX_EXPR;
    vpnline_list       *LINE;
    unsigned char       BASE;
    long                FLAGS;
    void               *USER;

  } vpngen_list;


  typedef struct vpnins_list 
  {
    struct vpnins_list *NEXT;
    char               *NAME;
    struct vpnmod_list *MODEL;
    vpnmap_list        *MAP;
    vpngen_list        *GENERIC;
    vpnline_list       *LINE;
    long                FLAGS;
    void               *USER;

  } vpnins_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Model                      |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnport_list 
  {
    struct vpnport_list  *NEXT;
    vexexpr              *VEX_ATOM;
    unsigned char         DIR;
    unsigned char         BASE;
    vpnline_list         *LINE;
    long                  FLAGS;
    void                 *USER;

  } vpnport_list;

  typedef struct vpnmod_list 
  {
    struct vpnmod_list *NEXT;
    char               *NAME;
    vpnport_list       *PORT;
    vpngen_list        *GENERIC;
    vpnline_list       *LINE;
    long                FLAGS;
    void               *USER;

  } vpnmod_list;

/*------------------------------------------------------\
|                                                       |
|                        Vpn Figure                     |
|                                                       |
\------------------------------------------------------*/

  typedef struct vpnfig_list
  {
    struct vpnfig_list *NEXT;
    char               *NAME;
    vpnproc_list       *PROCESS;
    vpnfunc_list       *FUNCTION;
    vpnmod_list        *MODEL;
    vpnins_list        *INSTANCE;
    chain_list         *VAR_DECL;
    chain_list         *DEF_DECL;
    vpndecl_list       *DECLAR[ VPN_MAX_DECLAR_TYPE ];
    vpnattr_list       *ATTRIBUTE;
    authtable          *HASH_DECLAR[ VPN_MAX_DECLAR_TYPE ];
    authtable          *HASH_PROCESS;
    authtable          *HASH_FUNCTION;
    authtable          *HASH_TRANS;
    authtable          *HASH_PLACE;
    authtable          *HASH_MODEL;
    authtable          *HASH_INSTANCE;
    auth2table         *HASH_ARC;
    void               *CIRCUIT;
    long                FLAGS;
    void               *USER;

  } vpnfig_list;

/*------------------------------------------------------\
|                                                       |
|                     Global Variables                  |
|                                                       |
\------------------------------------------------------*/

  extern vpnfig_list *HEAD_VPNFIG;

  extern char  *VPN_FATHER_TYPE   [ VPN_MAX_FATHER_TYPE ];
  extern char  *VPN_ACT_TYPE      [ VPN_MAX_ACT_TYPE    ];
  extern char  *VPN_TRANS_TYPE    [ VPN_MAX_TRANS_TYPE  ];
  extern char  *VPN_PLACE_TYPE    [ VPN_MAX_PLACE_TYPE  ];
  extern char  *VPN_DECLAR_TYPE   [ VPN_MAX_DECLAR_TYPE ];
  extern char  *VPN_ARC_TYPE      [ VPN_MAX_ARC_TYPE    ];
  extern char  *VPN_DIR_TYPE      [ VPN_MAX_DIR_TYPE    ];
  extern char  *VPN_CLASS_TYPE    [ VPN_MAX_CLASS_TYPE  ];
  extern char  *VPN_KIND_TYPE     [ VPN_MAX_KIND_TYPE   ];
  extern char  *VPN_ATTRIBUTE_TYPE[ VPN_MAX_ATTR_TYPE   ];

  extern char *VPN_IN;
  extern char *VPN_OUT;

  extern char *VPN_VHDL_FORMAT;
  extern char *VPN_VPN_FORMAT;
  extern char *VPN_VBE_FORMAT;
  extern char *VPN_VST_FORMAT;
  extern char *VPN_SYN_FORMAT;
  extern char *VPN_FSM_FORMAT;

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                       Env Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void vpnenv __P(());

/*------------------------------------------------------\
|                                                       |
|                    Alloc Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern   vpnfig_list *allocvpnfig __P(());
  extern  vpnproc_list *allocvpnproc __P(());
  extern  vpnfunc_list *allocvpnfunc __P(());
  extern vpnplace_list *allocvpnplace __P(());
  extern vpntrans_list *allocvpntrans __P(());
  extern        vpnarc *allocvpnarc __P(());
  extern  vpndecl_list *allocvpndecl __P(());
  extern        vpnsym *allocvpnsym __P((unsigned int Width));
  extern   vpnact_list *allocvpnact __P(());
  extern  vpnattr_list *allocvpnattr __P(());
  extern  vpnline_list *allocvpnline __P(());

  extern  vpnmod_list  *allocvpnmod __P(());
  extern  vpnins_list  *allocvpnins __P(());
  extern  vpnport_list *allocvpnport __P(());
  extern  vpnmap_list  *allocvpnmap __P(());
  extern  vpngen_list  *allocvpngen __P(());

/*------------------------------------------------------\
|                                                       |
|                      Free Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern void  freevpnfig __P((vpnfig_list *Figure));
  extern void  freevpnproc __P((vpnproc_list *Process));
  extern void  freevpnfunc __P((vpnfunc_list *Function));
  extern void  freevpnplace __P((vpnplace_list *Place));
  extern void  freevpntrans __P((vpntrans_list *Trans));
  extern void  freevpnarc __P((vpnarc *Arc));
  extern void  freevpndecl __P((vpndecl_list *Decl));
  extern void  freevpnsym __P((vpnsym *Sym));
  extern void  freevpnact __P((vpnact_list *Act));
  extern void  freevpnattr __P((vpnattr_list *Attr));
  extern void  freevpnline __P((vpnline_list *Line));

  extern void  freevpnmod __P((vpnmod_list *Model));
  extern void  freevpnins __P((vpnins_list *Instance));
  extern void  freevpnport __P((vpnport_list *Port));
  extern void  freevpnmap __P((vpnmap_list *Map));
  extern void  freevpngen __P((vpngen_list *Generic));

/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern  vpnfig_list *addvpnfig __P((char *Name));
  extern vpnproc_list *addvpnproc __P((vpnfig_list *Figure, char *Name));
  extern vpnfunc_list *addvpnfunc __P((vpnfig_list *Figure, char *Name));
  extern       vpnarc *addvpnarc __P((vpnfig_list *Figure, void *Source, void *Target, unsigned char ArcType));
  extern  vpnact_list *addvpnact __P((vpnfig_list *Figure, vpntrans_list *Trans, vexexpr *Atom, vexexpr *Expr, unsigned char Type));
  extern  vpnact_list *addvpnactasg __P((vpnfig_list *Figure, vpntrans_list *Trans, vexexpr *Atom, vexexpr *Expr));
  extern  vpnact_list *addvpnactafter __P((vpnfig_list *Figure, vpntrans_list *Trans, vpnact_list *PrevAction, vexexpr *Atom, vexexpr *Expr, unsigned char Type));
  extern  vpnact_list *addvpnactasgafter __P((vpnfig_list *Figure, vpntrans_list *Trans, vpnact_list *PrevAction, vexexpr *Atom, vexexpr *Expr));
  extern  vpnact_list *addvpnactbefore __P((vpnfig_list *Figure, vpntrans_list *Trans, vpnact_list *NextAction, vexexpr *Atom, vexexpr *Expr, unsigned char Type));
  extern  vpnact_list *addvpnactasgbefore __P((vpnfig_list *Figure, vpntrans_list *Trans, vpnact_list *NextAction, vexexpr *Atom, vexexpr *Expr));
  extern  vpnact_list *addvpnactfirst __P((vpnfig_list *Figure, vpntrans_list *Trans, vexexpr *Atom, vexexpr *Expr, unsigned char Type));
  extern  vpnact_list *addvpnactasgfirst __P((vpnfig_list *Figure, vpntrans_list *Trans, vexexpr *Atom, vexexpr *Expr));

  extern vpndecl_list *addvpndecl __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Type, unsigned char Base));
  extern vpndecl_list *addvpndeclport __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Base, unsigned char Dir, unsigned char Kind));
  extern vpndecl_list *addvpndeclsig __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Base, unsigned char Kind));
  extern vpndecl_list *addvpndeclcst __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclgen __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclvar __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndecldef __P((vpnfig_list *Figure, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclfuncvar __P((vpnfig_list *Figure, vpnfunc_list *Function, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclprocvar __P((vpnfig_list *Figure, vpnproc_list *Process, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclfuncdef __P((vpnfig_list *Figure, vpnfunc_list *Function, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclprocdef __P((vpnfig_list *Figure, vpnproc_list *Process, vexexpr *Atom, unsigned char Base));
  extern vpndecl_list *addvpndeclfuncarg __P((vpnfig_list *Figure, vpnfunc_list *Function, vexexpr *Atom, unsigned char Base, unsigned char Dir, unsigned char Class));

  extern vpnplace_list *addvpnprocplace __P((vpnfig_list *Figure, vpnproc_list *Process, char *Name));
  extern vpntrans_list *addvpnproctrans __P((vpnfig_list *Figure, vpnproc_list *Process, char *Name));

  extern vpnplace_list *addvpnfuncplace __P((vpnfig_list *Figure, vpnfunc_list *Function, char *Name));
  extern vpntrans_list *addvpnfunctrans __P((vpnfig_list *Figure, vpnfunc_list *Function, char *Name));

  extern  vpnattr_list *addvpnattr __P((vpnfig_list *Figure, char *Name, char *Entity, unsigned char Type, char *Value ));
  extern  vpnline_list *addvpnline __P((vpnfig_list *Figure, vpnline_list **HeadLine, long Line));
  extern  vpnline_list *addvpnfileline __P((vpnfig_list *Figure, vpnline_list **HeadLine, char *File, long Line));

  extern  vpnmod_list  *addvpnmod __P((vpnfig_list *Figure, char *Name));
  extern  vpnport_list *addvpnmodport __P((vpnfig_list *Figure, vpnmod_list *Model, vexexpr *Atom, unsigned char Base, unsigned char Dir ));
  extern  vpngen_list *addvpnmodgen __P((vpnfig_list *Figure, vpnmod_list *Model, vexexpr *Atom, vexexpr *Expr, unsigned char Base ));
  extern  vpnins_list  *addvpnins __P((vpnfig_list *Figure, char *Name, vpnmod_list *Model));
  extern  vpnmap_list  *addvpninsmap __P((vpnfig_list *Figure, vpnins_list *Instance, vexexpr *Formal, vexexpr *Actual ));
  extern vpngen_list   *addvpninsgen __P((vpnfig_list *Figure, vpnins_list *Instance, vexexpr *Atom, vexexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                     Union Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern  vpnline_list *unionvpnline __P((vpnfig_list *VpnFigure, vpnline_list **HeadLine, vpnline_list *SourceLine));
  extern  vpnline_list *unionvpnfileline __P((vpnfig_list *VpnFigure, vpnline_list **HeadLine, vpnline_list *SourceLine));

/*------------------------------------------------------\
|                                                       |
|                  Simplify Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern           void  simpvpnact __P((vpnact_list *VpnAct));
  extern           void  simpvpntrans __P((vpntrans_list *VpnTrans));
  extern           void  simpvpnproc __P((vpnproc_list *VpnProc));
  extern           void  simpvpnfunc __P((vpnfunc_list *VpnFunc));
  extern           void  simpvpnfig __P((vpnfig_list *VpnFigure));

/*------------------------------------------------------\
|                                                       |
|                       Get Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern        vpnsym *getvpnsymdecl __P((vpndecl_list *Declar, short Index));
  extern        vpnsym *getvpnsymdeclpos __P((vpndecl_list *Declar, int Position));

/*------------------------------------------------------\
|                                                       |
|                    Search Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern   vpnfig_list *searchvpnfig __P((char *Name));
  extern  vpnproc_list *searchvpnproc __P((vpnfig_list *Figure, char *Name));
  extern  vpnfunc_list *searchvpnfunc __P((vpnfig_list *Figure, char *Name));
  extern vpnplace_list *searchvpnplace __P((vpnfig_list *Figure, char *Name));
  extern vpntrans_list *searchvpntrans __P((vpnfig_list *Figure, char *Name));
  extern        vpnarc *searchvpnarc __P((vpnfig_list *Figure, void *Source, void *Target));
  extern        vpnsym *searchvpnsym __P((vpnfig_list *Figure, char *Name, short Index, unsigned char Type));
  extern  vpndecl_list *searchvpndecl __P((vpnfig_list *Figure, char *Name, unsigned char Type));

  extern  vpnmod_list *searchvpnmod __P((vpnfig_list *Figure, char *Name));
  extern  vpnins_list *searchvpnins __P((vpnfig_list *Figure, char *Name));

/*------------------------------------------------------\
|                                                       |
|                    Delete Functions                   |
|                                                       |
\------------------------------------------------------*/

  extern int  delvpnfig __P((char *Name));
  extern int  delvpnplace __P((vpnfig_list *Figure, vpnplace_list *Place));
  extern int  delvpntrans __P((vpnfig_list *Figure, vpntrans_list *Trans));
  extern int  delvpnarc __P((vpnfig_list *Figure, vpnarc *Arc));
  extern int  delvpnact __P((vpnfig_list *Figure, vpntrans_list *Trans, vpnact_list *Action));
  extern int  delvpnline __P((vpnfig_list *Figure, vpnline_list **PrevLine, vpnline_list *Line));
  extern int  delvpnproc __P((vpnfig_list *Figure, vpnproc_list *Process));
  extern int  delvpnfunc __P((vpnfig_list *Figure, vpnfunc_list *Function));

/*------------------------------------------------------\
|                                                       |
|                     View Functions                    |
|                                                       |
\------------------------------------------------------*/

  extern void  viewvpnfig __P((vpnfig_list *Figure));
  extern void  viewvpnsym __P((vpnsym *Symbol));
  extern void  viewvpndecl __P((vpndecl_list *Decl));
  extern void  viewvpnact __P((vpnact_list *Action));
  extern void  viewvpnplace __P((vpnplace_list *Place));
  extern void  viewvpntrans __P((vpntrans_list *Trans));
  extern void  viewvpnproc __P((vpnproc_list *Process));
  extern void  viewvpnfunc __P((vpnfunc_list *Function));
  extern void  viewvpnline __P((vpnline_list *Line));

  extern void  viewvpnins __P((vpnins_list *Instance));
  extern void  viewvpnmod __P((vpnmod_list *Model));
  extern void  viewvpnport __P((vpnport_list *Port));
  extern void  viewvpnmap __P((vpnmap_list *Map));
  extern void  viewvpngen __P((vpngen_list *Generic));

# endif
