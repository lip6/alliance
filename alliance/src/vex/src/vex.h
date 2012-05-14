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
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for VEX         |
|                                                       |
|  Date    :            04.02.97                        |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef VEX_103_H
# define VEX_103_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/

# ifndef __P
#  if defined(__STDC__) ||  defined(__GNUC__)
#   define __P(x) x
#  else
#   define __P(x) ()
#  endif
# endif

/*------------------------------------------------------\
|                                                       |
|                       View Mode                       |
|                                                       |
\------------------------------------------------------*/

# define VEX_VIEW_VHDL      0
# define VEX_VIEW_VERILOG   1

/*------------------------------------------------------\
|                                                       |
|                       Node Type                       |
|                                                       |
\------------------------------------------------------*/

# define VEX_NODE_ATOM_MASK      (unsigned short)(0x1000)
# define VEX_NODE_OPER_MASK      (unsigned short)(0x2000)
# define VEX_NODE_FUNC_MASK      (unsigned short)(0x4000)
# define VEX_NODE_TYPE_MASK      (unsigned short)(0x7000)

# define VEX_NODE_TAG_MASK       (unsigned short)(0x8000)

# define VEX_ATOM_LITERAL_MASK   (unsigned short)(0x0100)  /*Literal Name    */
# define VEX_NODE_SIGNED_MASK    (unsigned short)(0x0200)  /*Signed Unsigned */
# define VEX_NODE_BIT_MASK       (unsigned short)(0x0400)  /*Bit Vector      */
# define VEX_NODE_DOWN_MASK      (unsigned short)(0x0800)  /*Down Up         */
# define VEX_NODE_VAR_WIDTH_MASK (unsigned short)(0x0080)  /*Variable Width  */

# define VEX_NODE_USER1_MASK     (unsigned short)(0x0020)
# define VEX_NODE_USER2_MASK     (unsigned short)(0x0040)

# define VEX_NODE_VHDL_TYPE_MASK (unsigned short)(0x001f)

/*------------------------------------------------------\
|                                                       |
|                    Macro Node Type                    |
|                                                       |
\------------------------------------------------------*/

# define IsVexNodeAtom( N )  ( (N)->TYPE &   VEX_NODE_ATOM_MASK )
# define SetVexNodeAtom( N ) ( (N)->TYPE = ((N)->TYPE & ~VEX_NODE_TYPE_MASK )\
                                                      |  VEX_NODE_ATOM_MASK )
# define IsVexNodeOper( N )  ( (N)->TYPE &   VEX_NODE_OPER_MASK )
# define SetVexNodeOper( N ) ( (N)->TYPE = ((N)->TYPE & ~VEX_NODE_TYPE_MASK )\
                                                      |  VEX_NODE_OPER_MASK )
# define IsVexNodeFunc( N )  ( (N)->TYPE &   VEX_NODE_FUNC_MASK )
# define SetVexNodeFunc( N ) ( (N)->TYPE = ((N)->TYPE & ~VEX_NODE_TYPE_MASK )\
                                                        |  VEX_NODE_FUNC_MASK )

/*------------------------------------------------------\
|                                                       |
|                    Macro Node Width                   |
|                                                       |
\------------------------------------------------------*/

# define IsVexNodeVarWidth( N )    ( (N)->TYPE &   VEX_NODE_VAR_WIDTH_MASK )
# define SetVexNodeVarWidth( N )   ( (N)->TYPE |=  VEX_NODE_VAR_WIDTH_MASK )
# define ClearVexNodeVarWidth( N ) ( (N)->TYPE &= ~VEX_NODE_VAR_WIDTH_MASK )

/*------------------------------------------------------\
|                                                       |
|                    Macro Node Signed                  |
|                                                       |
\------------------------------------------------------*/

# define IsVexNodeSigned( N )    ( (N)->TYPE &   VEX_NODE_SIGNED_MASK )
# define SetVexNodeSigned( N )   ( (N)->TYPE |=  VEX_NODE_SIGNED_MASK )
# define ClearVexNodeSigned( N ) ( (N)->TYPE &= ~VEX_NODE_SIGNED_MASK )

/*------------------------------------------------------\
|                                                       |
|                    Macro Node Bit                     |
|                                                       |
\------------------------------------------------------*/

# define IsVexNodeBit( N )    ( (N)->TYPE &   VEX_NODE_BIT_MASK )
# define SetVexNodeBit( N )   ( (N)->TYPE |=  VEX_NODE_BIT_MASK )
# define ClearVexNodeBit( N ) ( (N)->TYPE &= ~VEX_NODE_BIT_MASK )

/*------------------------------------------------------\
|                                                       |
|                    Macro Node Direction               |
|                                                       |
\------------------------------------------------------*/

# define IsVexNodeDown( N )    ( (N)->TYPE &   VEX_NODE_DOWN_MASK )
# define SetVexNodeDown( N )   ( (N)->TYPE |=  VEX_NODE_DOWN_MASK )
# define ClearVexNodeDown( N ) ( (N)->TYPE &= ~VEX_NODE_DOWN_MASK )

/*------------------------------------------------------\
|                                                       |
|                    Macro Node Tag                     |
|                                                       |
\------------------------------------------------------*/

# define IsVexNodeTag( N )    ( (N)->TYPE &   VEX_NODE_TAG_MASK )
# define SetVexNodeTag( N )   ( (N)->TYPE |=  VEX_NODE_TAG_MASK )
# define ClearVexNodeTag( N ) ( (N)->TYPE &= ~VEX_NODE_TAG_MASK )

/*------------------------------------------------------\
|                                                       |
|                       Macro Atom                      |
|                                                       |
\------------------------------------------------------*/

# define IsVexAtomLiteral( N )    ( (N)->TYPE &   VEX_ATOM_LITERAL_MASK )
# define SetVexAtomLiteral( N )   ( (N)->TYPE |=  VEX_ATOM_LITERAL_MASK )
# define ClearVexAtomLiteral( N ) ( (N)->TYPE &= ~VEX_ATOM_LITERAL_MASK )

# define IsVexAtomVarWidth( N )  ( IsVexNodeVarWidth( N ) )
# define SetVexAtomVarWidth( N ) ( SetVexNodeVarWidth( N ) )
# define IsVexAtomStaWidth( N )  ( ! IsVexNodeVarWidth( N ) )
# define SetVexAtomStaWidth( N ) ( ClearVexNodeVarWidth( N ) )

# define IsVexAtomBit( N )       ( IsVexNodeBit( N ) )
# define SetVexAtomBit( N )      ( SetVexNodeBit( N ) )
# define IsVexAtomVector( N )    ( ! IsVexNodeBit( N ) )
# define SetVexAtomVector( N )   ( ClearVexNodeBit( N ) )

# define IsVexAtomSigned( N )    ( IsVexNodeSigned( N ) )
# define SetVexAtomSigned( N )   ( SetVexNodeSigned( N ) )
# define IsVexAtomUnsigned( N )  ( ! IsVexNodeSigned( N ) )
# define SetVexAtomUnsigned( N ) ( ClearVexNodeSigned( N ) )

# define IsVexAtomDown( N )      ( IsVexNodeDown( N ) )
# define SetVexAtomDown( N )     ( SetVexNodeDown( N ) )
# define IsVexAtomUp( N )        ( ! IsVexNodeDown( N ) )
# define SetVexAtomUp( N )       ( ClearVexNodeDown( N ) )

/*------------------------------------------------------\
|                                                       |
|                       Macro Oper                      |
|                                                       |
\------------------------------------------------------*/

# define IsVexOperVarWidth( N )  ( IsVexNodeVarWidth( N ) )
# define SetVexOperVarWidth( N ) ( SetVexNodeVarWidth( N ) )
# define IsVexOperStaWidth( N )  ( ! IsVexNodeVarWidth( N ) )
# define SetVexOperStaWidth( N ) ( ClearVexNodeVarWidth( N ) )

# define IsVexOperBit( N )       ( IsVexNodeBit( N ) )
# define SetVexOperBit( N )      ( SetVexNodeBit( N ) )
# define IsVexOperVector( N )    ( ! IsVexNodeBit( N ) )
# define SetVexOperVector( N )   ( ClearVexNodeBit( N ) )

# define IsVexOperSigned( N )    ( IsVexNodeSigned( N ) )
# define SetVexOperSigned( N )   ( SetVexNodeSigned( N ) )
# define IsVexOperUnsigned( N )  ( ! IsVexNodeSigned( N ) )
# define SetVexOperUnsigned( N ) ( ClearVexNodeSigned( N ) )

# define IsVexOperDown( N )      ( IsVexNodeDown( N ) )
# define SetVexOperDown( N )     ( SetVexNodeDown( N ) )
# define IsVexOperUp( N )        ( ! IsVexNodeDown( N ) )
# define SetVexOperUp( N )       ( ClearVexNodeDown( N ) )

/*------------------------------------------------------\
|                                                       |
|                       Macro Func                      |
|                                                       |
\------------------------------------------------------*/

# define IsVexFuncVarWidth( N )  ( IsVexNodeVarWidth( N ) )
# define SetVexFuncVarWidth( N ) ( SetVexNodeVarWidth( N ) )
# define IsVexFuncStaWidth( N )  ( ! IsVexNodeVarWidth( N ) )
# define SetVexFuncStaWidth( N ) ( ClearVexNodeVarWidth( N ) )

# define IsVexFuncBit( N )       ( IsVexNodeBit( N ) )
# define SetVexFuncBit( N )      ( SetVexNodeBit( N ) )
# define IsVexFuncVector( N )    ( ! IsVexNodeBit( N ) )
# define SetVexFuncVector( N )   ( ClearVexNodeBit( N ) )

# define IsVexFuncSigned( N )    ( IsVexNodeSigned( N ) )
# define SetVexFuncSigned( N )   ( SetVexNodeSigned( N ) )
# define IsVexFuncUnsigned( N )  ( ! IsVexNodeSigned( N ) )
# define SetVexFuncUnsigned( N ) ( ClearVexNodeSigned( N ) )

# define IsVexFuncDown( N )      ( IsVexNodeDown( N ) )
# define SetVexFuncDown( N )     ( SetVexNodeDown( N ) )
# define IsVexFuncUp( N )        ( ! IsVexNodeDown( N ) )
# define SetVexFuncUp( N )       ( ClearVexNodeDown( N ) )

/*------------------------------------------------------\
|                                                       |
|                       Macro Type                      |
|                                                       |
\------------------------------------------------------*/

# define SetVexVhdlType( N, T ) ((N)->TYPE = ((N)->TYPE & ~VEX_NODE_VHDL_TYPE_MASK)|(T))
# define GetVexVhdlType( N )    ((N)->TYPE & VEX_NODE_VHDL_TYPE_MASK)

/*------------------------------------------------------\
|                                                       |
|                      Operators                        |
|                                                       |
\------------------------------------------------------*/

# define VEX_CONCAT           0
# define VEX_NOT              1
# define VEX_NEG              2
# define VEX_EVENT            3
# define VEX_OR               4
# define VEX_AND              5
# define VEX_XOR              6
# define VEX_NOR              7
# define VEX_NAND             8
# define VEX_NXOR             9
# define VEX_EQ              10
# define VEX_NE              11
# define VEX_LT              12
# define VEX_LE              13
# define VEX_GT              14
# define VEX_GE              15
# define VEX_ADD             16
# define VEX_SUB             17
# define VEX_MUL             18
# define VEX_DIV             19
# define VEX_EXP             20
# define VEX_MOD             21
# define VEX_REM             22
# define VEX_TO              23
# define VEX_DOWNTO          24
# define VEX_INDEX           25
# define VEX_LEFT            26
# define VEX_RIGHT           27
# define VEX_LOW             28
# define VEX_HIGH            29
# define VEX_LENGTH          30
# define VEX_RANGE           31
# define VEX_REV_RANGE       32
# define VEX_DRIVER          33
# define VEX_IFT             34
# define VEX_ARRAY           35
# define VEX_INDEX_N         36
# define VEX_OTHERS          37
# define VEX_NUM_BIT         38
# define VEX_ABS             39

# define VEX_AF              40
# define VEX_AG              41
# define VEX_AX              42
# define VEX_AU              43
# define VEX_EF              44
# define VEX_EG              45
# define VEX_EX              46
# define VEX_EU              47
# define VEX_EQUIV           48
# define VEX_IMPLY           49

# define VEX_MAX_OPERATOR    50

/*------------------------------------------------------\
|                                                       |
|                       Vhdl Type                       |
|                                                       |
\------------------------------------------------------*/

# define VEX_TYPE_SEVERITY            0
# define VEX_TYPE_BOOLEAN             1
# define VEX_TYPE_CHARACTER           2
# define VEX_TYPE_STRING              3
# define VEX_TYPE_BIT                 4
# define VEX_TYPE_INTEGER             5
# define VEX_TYPE_NATURAL             6
# define VEX_TYPE_BIT_VECTOR          7
# define VEX_TYPE_STD_ULOGIC          8
# define VEX_TYPE_STD_LOGIC           9
# define VEX_TYPE_STD_ULOGIC_VECTOR  10
# define VEX_TYPE_STD_LOGIC_VECTOR   11
# define VEX_TYPE_X01                12
# define VEX_TYPE_X01Z               13
# define VEX_TYPE_UX01               14
# define VEX_TYPE_UX01Z              15
# define VEX_TYPE_UNSIGNED           16
# define VEX_TYPE_SIGNED             17
# define VEX_TYPE_SMALL_INT          18
# define VEX_TYPE_REG_BIT            19
# define VEX_TYPE_REG_VECTOR         20
# define VEX_TYPE_MUX_BIT            21
# define VEX_TYPE_MUX_VECTOR         22
# define VEX_TYPE_WOR_BIT            23
# define VEX_TYPE_WOR_VECTOR         24
# define VEX_TYPE_ENUMERATE          25
# define VEX_TYPE_ARRAY              26

# define VEX_MAX_TYPE                27

/*------------------------------------------------------\
|                                                       |
|                      Std Function                     |
|                                                       |
\------------------------------------------------------*/

# define VEX_STD_TO_BIT                  0
# define VEX_STD_TO_BITVECTOR            1
# define VEX_STD_TO_STDULOGIC            2
# define VEX_STD_TO_STDLOGICVECTOR       3
# define VEX_STD_TO_STDULOGICVECTOR      4
# define VEX_STD_TO_X01                  5
# define VEX_STD_TO_X01Z                 6
# define VEX_STD_TO_UX01                 7
# define VEX_STD_RISING_EDGE             8
# define VEX_STD_FALLING_EDGE            9
# define VEX_STD_IS_X                   10
# define VEX_STD_ABS                    11
# define VEX_STD_SHL                    12
# define VEX_STD_SHR                    13
# define VEX_STD_CONV_INTEGER           14
# define VEX_STD_CONV_UNSIGNED          15
# define VEX_STD_CONV_SIGNED            16
# define VEX_STD_CONV_STD_LOGIC_VECTOR  17
# define VEX_STD_EXT                    18
# define VEX_STD_SXT                    19
# define VEX_STD_SHIFT_LEFT             20
# define VEX_STD_SHIFT_RIGHT            21
# define VEX_STD_ROTATE_LEFT            22
# define VEX_STD_ROTATE_RIGHT           23
# define VEX_STD_RESIZE                 24
# define VEX_STD_TO_INTEGER             25
# define VEX_STD_TO_UNSIGNED            26
# define VEX_STD_TO_SIGNED              27
# define VEX_STD_STD_LOGIC_VECTOR       28
# define VEX_STD_STD_ULOGIC_VECTOR      29
# define VEX_STD_SIGNED                 30
# define VEX_STD_UNSIGNED               31

# define VEX_MAX_STD_FUNC               32

/*------------------------------------------------------\
|                                                       |
|                        Literals 9                     |
|                                                       |
\------------------------------------------------------*/

# define VEX_UNINIT_ID       0      /*  Uninitialized    */
# define VEX_UNKNOWN_ID      1      /*  Forcing  Unknown */
# define VEX_ZERO_ID         2      /*  Forcing  0       */
# define VEX_ONE_ID          3      /*  Forcing  1       */
# define VEX_TRISTATE_ID     4      /*  High Impedance   */
# define VEX_WEAK_UNKNOWN_ID 5      /*  Weak     Unknown */
# define VEX_WEAK_ZERO_ID    6      /*  Weak     0       */
# define VEX_WEAK_ONE_ID     7      /*  Weak     1       */  
# define VEX_DC_ID           8      /*  Don't care       */
# define VEX_MAX_ID          9

/*------------------------------------------------------\
|                                                       |
|                    Macro Literals 9                   |
|                                                       |
\------------------------------------------------------*/

# define VEX_UNINIT         VEX_LITERAL_BY_ID[ VEX_UNINIT_ID       ]
# define VEX_UNKNOWN        VEX_LITERAL_BY_ID[ VEX_UNKNOWN_ID      ]
# define VEX_ZERO           VEX_LITERAL_BY_ID[ VEX_ZERO_ID         ]
# define VEX_ONE            VEX_LITERAL_BY_ID[ VEX_ONE_ID          ]
# define VEX_TRISTATE       VEX_LITERAL_BY_ID[ VEX_TRISTATE_ID     ]
# define VEX_WEAK_UNKNOWN   VEX_LITERAL_BY_ID[ VEX_WEAK_UNKNOWN_ID ]
# define VEX_WEAK_ZERO      VEX_LITERAL_BY_ID[ VEX_WEAK_ZERO_ID    ]
# define VEX_WEAK_ONE       VEX_LITERAL_BY_ID[ VEX_WEAK_ONE_ID     ]
# define VEX_DC             VEX_LITERAL_BY_ID[ VEX_DC_ID           ]

/*------------------------------------------------------\
|                                                       |
|                    Macro Atom Literals 9              |
|                                                       |
\------------------------------------------------------*/

# define VEX_ATOM_UNINIT        VEX_ATOM_BY_ID[ VEX_UNINIT_ID       ]
# define VEX_ATOM_UNKNOWN       VEX_ATOM_BY_ID[ VEX_UNKNOWN_ID      ]
# define VEX_ATOM_ZERO          VEX_ATOM_BY_ID[ VEX_ZERO_ID         ]
# define VEX_ATOM_ONE           VEX_ATOM_BY_ID[ VEX_ONE_ID          ]
# define VEX_ATOM_TRISTATE      VEX_ATOM_BY_ID[ VEX_TRISTATE_ID     ]
# define VEX_ATOM_WEAK_UNKNOWN  VEX_ATOM_BY_ID[ VEX_WEAK_UNKNOWN_ID ]
# define VEX_ATOM_WEAK_ZERO     VEX_ATOM_BY_ID[ VEX_WEAK_ZERO_ID    ]
# define VEX_ATOM_WEAK_ONE      VEX_ATOM_BY_ID[ VEX_WEAK_ONE_ID     ]
# define VEX_ATOM_DC            VEX_ATOM_BY_ID[ VEX_DC_ID           ]

/*------------------------------------------------------\
|                                                       |
|                    Macro Oper Types                   |
|                                                       |
\------------------------------------------------------*/

# define GetVexFuncValue( N )     ( (char    *)(N)->VALUE    )
# define SetVexFuncValue( N, F )  ( (N)->VALUE = (void *)(F) )

# define GetVexOperValue( N )     ( (long     )(N)->VALUE    )
# define SetVexOperValue( N, O )  ( (N)->VALUE = (void *)(O) )

# define GetVexAtomValue( N )     ( (char    *)(N)->VALUE    )
# define SetVexAtomValue( N, A )  ( (N)->VALUE = (void *)(A) )

# define GetVexOperand( C )       ( (vexexpr *)(C)->DATA     )
# define GetVexOperandL( C )      ( (C)->DATA                )
# define SetVexOperand( C, O )    ( (C)->DATA = (void *)(O)  )

/*------------------------------------------------------\
|                                                       |
|                         Type                          |
|                                                       |
\------------------------------------------------------*/

  typedef  struct  vexexpr
  {
    chain_list *OPERAND;
    void       *VALUE;
    short       TYPE;
    short       LEFT;
    short       RIGHT;
    short       WIDTH;
   
  } vexexpr;

/*------------------------------------------------------\
|                                                       |
|                     Global Variable                   |
|                                                       |
\------------------------------------------------------*/

   extern char *VEX_OPER_NAME[ VEX_MAX_OPERATOR ];
   extern char *VEX_OPER_UPPER_NAME[ VEX_MAX_OPERATOR ];
   extern char *VEX_OPER_VERILOG_NAME[ VEX_MAX_OPERATOR ];
   extern char *VEX_TYPE_NAME[ VEX_MAX_TYPE ];
   extern long  VEX_OPER_NOT[ VEX_MAX_OPERATOR ];
   extern char  VEX_LITERAL_BY_ID[ VEX_MAX_ID ];
   extern char *VEX_ATOM_BY_ID[ VEX_MAX_ID ];
   extern char *VEX_STD_FUNC_NAME[ VEX_MAX_STD_FUNC ];
   extern char *VEX_STD_FUNC_UPPER_NAME[ VEX_MAX_STD_FUNC ];

/*------------------------------------------------------\
|                                                       |
|                        Functions                      |
|                                                       |
\------------------------------------------------------*/
/*------------------------------------------------------\
|                                                       |
|                Initialize Functions                   |
|                                                       |
\------------------------------------------------------*/

   extern void  vexenv __P(());

/*------------------------------------------------------\
|                                                       |
|                      Node Functions                   |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *allocvexnode __P(());
   extern vexexpr *dupvexnode __P((vexexpr *Node));
   extern vexexpr *copyvexnode __P((vexexpr *Target, vexexpr *Source));
   extern void     freevexnode __P((vexexpr *Node));

/*------------------------------------------------------\
|                                                       |
|                    Create Functions                   |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *createvexatombit __P((char *Name));
   extern vexexpr *createvexatomvec __P((char *Name, short Left, short Right));
   extern vexexpr *createvexatomlit __P((char *Name));
   extern vexexpr *createvexatomveclit __P((unsigned char Literal, short Width));
   extern vexexpr *createvexatomlong __P((long Integer, short Width, short Signed));
   extern vexexpr *createvexatomlongarray __P((long *Array, short Size, short Width, short Signed));
  
   extern vexexpr *createvexoper __P((long Oper, short Width));
   extern vexexpr *createvexbinexpr __P((long Oper, short Width, vexexpr *Expr1, vexexpr *Expr2));
   extern vexexpr *createvexternaryexpr __P((long Oper, short Width, \
                                      vexexpr *Expr1, vexexpr *Expr2, vexexpr *Expr3));
   extern vexexpr *createvexunaryexpr __P((long Oper, short Width, vexexpr *Expr));

   extern vexexpr *createvexfunc __P((char *Func, short Width));

/*------------------------------------------------------\
|                                                       |
|                   Simplify Functions                  |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *simpvexexpr __P((vexexpr *Expr));
   extern vexexpr *simpvexexpreq __P((vexexpr *Expr));
   extern vexexpr *simpvexexprothers __P((vexexpr *Expr, short Width ));

/*------------------------------------------------------\
|                                                       |
|                   Optimize Functions                  |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *optimvexnotexpr __P((vexexpr *Expr));
   extern vexexpr *optimvexbinexpr __P((long Oper, short Width, vexexpr *Expr1, vexexpr *Expr2));

/*------------------------------------------------------\
|                                                       |
|                      Add Functions                    |
|                                                       |
\------------------------------------------------------*/

   extern void  addvexqexpr __P((vexexpr *Expr1, vexexpr *Expr2));
   extern void  addvexhexpr __P((vexexpr *Expr1, vexexpr *Expr2));

/*------------------------------------------------------\
|                                                       |
|                    Duplicate Functions                |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *dupvexexpr __P((vexexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                      Del Functions                    |
|                                                       |
\------------------------------------------------------*/

   extern void  delvexexpr    __P((vexexpr *Expr));
   extern void  delvexexprnum __P((vexexpr *Expr, int Number));

/*------------------------------------------------------\
|                                                       |
|                     Free Functions                    |
|                                                       |
\------------------------------------------------------*/

   extern void  freevexexpr __P((vexexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                      Is Functions                     |
|                                                       |
\------------------------------------------------------*/

   extern int  isvexunaryoper       __P((long Oper));
   extern int  isvexbinaryoper      __P((long Oper));
   extern int  isvexternaryoper     __P((long Oper));
   extern int  isvexoperinexpr      __P((vexexpr *Expr, long Oper));
   extern int  isvexarithoperinexpr __P((vexexpr *Expr));
   extern int  isvexnameinexpr      __P((vexexpr *Expr, char *Name));
   extern int  isvexpositiveoper    __P((long Oper));
   extern int  isvexnegativeoper    __P((long Oper));
   extern int  isvexequalexpr       __P((vexexpr *Expr1, vexexpr *Expr2));
   extern int  isvextypedivisible   __P((int Type));
   extern int  isvextypevector      __P((int Type));

/*------------------------------------------------------\
|                                                       |
|                      Get Functions                    |
|                                                       |
\------------------------------------------------------*/

   extern char *getvexvectorname __P((char *Vector, long *Index));
   extern char *getvexopername __P((long Oper));
   extern char *getvexatomname  __P((vexexpr *Expr));
   extern char *getvexoperuppername __P((long Oper));
   extern char *getvexoperverilogname __P((long Oper));
   extern long  getvexoperbyname __P((char *Name));
   extern long  getvexnotoper __P((long Oper));

   extern char    *getvexarrayname __P((vexexpr *Atom));
   extern vexexpr *getvexarrayatom __P((vexexpr *Expr));

   extern chain_list *getvexatombitname __P((vexexpr *Expr));
   extern chain_list *getvexexprsupport __P((vexexpr *Expr));
   extern chain_list *unionvexexprsupport __P((chain_list *Support, vexexpr *Expr));

   extern ptype_list *getvexatombitnameptype __P((vexexpr *Expr));
   extern ptype_list *getvexexprsupportptype __P((vexexpr *Expr));
   extern ptype_list *unionvexexprsupportptype __P((ptype_list *Support, vexexpr *Expr));

   extern chain_list *getvexexprallname __P((vexexpr *Expr));
   extern chain_list *unionvexexprallname __P((chain_list *AllName, vexexpr *Expr));

   extern int  getvextypescalar __P((int Type));

   extern long getvexexprdepth   __P((vexexpr *Expr));
   extern long getvexexprnumnode __P((vexexpr *Expr));
   extern int  getvexliteralid   __P((char Literal));
   extern int  getvexstdfuncid   __P((char *FuncName));

   extern int  getvexvectorpos __P((vexexpr *Vector, short Index));
   extern int  getvexvectorindex __P((vexexpr *Atom, short Position));
   extern int  getvexvectormin __P((vexexpr *Expr));
   extern int  getvexvectormax __P((vexexpr *Expr));

   extern int getvexintnumbit __P((long Size));
   extern int getvexintervalnumbit __P((long Left, long Right));

/*------------------------------------------------------\
|                                                       |
|                    Eval Functions                     |
|                                                       |
\------------------------------------------------------*/

   extern int  evalvexatomlong __P((vexexpr *Expr, long *PLong));

   extern int  evalvexnotliteral __P((char Literal, char *PChar));
   extern int  evalvexandliteral __P((char Literal1, char Literal2, char *PChar));
   extern int  evalvexorliteral __P((char Literal1, char Literal2, char *PChar));
   extern int  evalvexxorliteral __P((char Literal1, char Literal2, char *PChar));

/*------------------------------------------------------\
|                                                       |
|                   Extend Sign Functions               |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *extendvexatomsign __P((vexexpr *Expr, short Width));

/*------------------------------------------------------\
|                                                       |
|                       Shift  Functions                |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *shiftvexatomleft __P((vexexpr *Expr, short Count ));
   extern vexexpr *shiftvexatomright __P((vexexpr *Expr, short Count ));

/*------------------------------------------------------\
|                                                       |
|                      Slice Functions                  |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *slicevexatomvec __P((vexexpr *Expr, short Left, short Right));

/*------------------------------------------------------\
|                                                       |
|                    Unflat Functions                   |
|                                                       |
\------------------------------------------------------*/

   extern vexexpr *unflatvexexpr __P((vexexpr *Expr));

/*------------------------------------------------------\
|                                                       |
|                      View Functions                   |
|                                                       |
\------------------------------------------------------*/

   extern void  viewvexexprfile __P((FILE *VexFile, vexexpr *Expr));
   extern void  viewvexexpr __P((vexexpr *Expr));
   extern void  viewvexexprln __P((vexexpr *Expr));
   extern void  viewvexexprbound __P((vexexpr *Expr));
   extern void  viewvexexprboundln __P((vexexpr *Expr));

   extern char *viewvexexprstr __P((vexexpr *Expr));
   extern char *viewvexexprstrbound __P((vexexpr *Expr));

   extern int  getvexviewmode __P(());
   extern void  setvexviewmode __P((int Mode));

# endif
