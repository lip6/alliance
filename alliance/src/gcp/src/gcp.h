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
/*------------------------------------------------------\
|                                                       |
|  Title   :   Structures and fonctions for GCP         |
|                                                       |
|  Date    :            09.01.99                        |
|                                                       |
|  Author  :        Jacomme Ludovic                     |
|                                                       |
\------------------------------------------------------*/

# ifndef GCP_101_H
# define GCP_101_H

/*------------------------------------------------------\
|                                                       |
|                      Constants                        |
|                                                       |
\------------------------------------------------------*/

# ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
# endif

/*------------------------------------------------------------\
|                                                             |
|                            Tree Mode Class                  |
|                                                             |
\------------------------------------------------------------*/

# define C_MODE_CLASS_RANDOM           0
# define C_MODE_CLASS_INT              1
# define C_MODE_CLASS_FLOAT            2
# define C_MODE_CLASS_PARTIAL_INT      3
# define C_MODE_CLASS_CC               4
# define C_MODE_CLASS_COMPLEX_INT      5
# define C_MODE_CLASS_COMPLEX_FLOAT    6

# define C_MAX_MODE_CLASS              7

/*------------------------------------------------------------\
|                                                             |
|                            Tree Node                        |
|                                                             |
\------------------------------------------------------------*/

# define C_INT_CST_NODE         0
# define C_REAL_CST_NODE        1
# define C_STRING_NODE          2
# define C_COMPLEX_NODE         3
# define C_IDENTIFIER_NODE      4
# define C_LIST_NODE            5
# define C_VEC_NODE             6
# define C_EXPR_NODE            7
# define C_BLOCK_NODE           8
# define C_TYPE_NODE            9
# define C_DECL_NODE           10
# define C_INST_NODE           11
# define C_ERROR_NODE          12

# define C_MAX_NODE_CODE       13

/*------------------------------------------------------------\
|                                                             |
|                           Tree Type                         |
|                                                             |
\------------------------------------------------------------*/

# define C_TREE_NODE_FIRST_CODE_MASK      0x00FF
# define C_TREE_NODE_EXTRA_CODE_MASK      0xFF00
# define C_TREE_NODE_EXTRA_CODE_SHIFT     8

# define CConcatCode(E,C)   (((E) << C_TREE_NODE_EXTRA_CODE_SHIFT) | C)
# define CFirstCode(C)   ((C) & C_TREE_NODE_FIRST_CODE_MASK)
# define CExtraCode(C)   ((C) >> C_TREE_NODE_EXTRA_CODE_SHIFT)

# define C_UNKNOWN_TYPE          CConcatCode(0,C_TYPE_NODE)
# define C_VOID_TYPE             CConcatCode(1,C_TYPE_NODE)
# define C_INTEGER_TYPE          CConcatCode(2,C_TYPE_NODE)
# define C_REAL_TYPE             CConcatCode(3,C_TYPE_NODE)
# define C_COMPLEX_TYPE          CConcatCode(4,C_TYPE_NODE)
# define C_ENUMERAL_TYPE         CConcatCode(5,C_TYPE_NODE)
# define C_BOOLEAN_TYPE          CConcatCode(6,C_TYPE_NODE)
# define C_CHAR_TYPE             CConcatCode(7,C_TYPE_NODE)
# define C_POINTER_TYPE          CConcatCode(8,C_TYPE_NODE)
# define C_OFFSET_TYPE           CConcatCode(9,C_TYPE_NODE)
# define C_REFERENCE_TYPE        CConcatCode(10,C_TYPE_NODE)
# define C_METHOD_TYPE           CConcatCode(11,C_TYPE_NODE)
# define C_FILE_TYPE             CConcatCode(12,C_TYPE_NODE)
# define C_ARRAY_TYPE            CConcatCode(13,C_TYPE_NODE)
# define C_SET_TYPE              CConcatCode(14,C_TYPE_NODE)
# define C_RECORD_TYPE           CConcatCode(15,C_TYPE_NODE)
# define C_UNION_TYPE            CConcatCode(16,C_TYPE_NODE)
# define C_QUAL_UNION_TYPE       CConcatCode(17,C_TYPE_NODE)
# define C_FUNCTION_TYPE         CConcatCode(18,C_TYPE_NODE)
# define C_LANG_TYPE             CConcatCode(19,C_TYPE_NODE)

# define C_MAX_TYPE_CODE        20

/*------------------------------------------------------------\
|                                                             |
|                           Tree Declaration                  |
|                                                             |
\------------------------------------------------------------*/

# define C_UNKNOWN_DECL         CConcatCode(0,C_DECL_NODE)
# define C_FUNCTION_DECL        CConcatCode(1,C_DECL_NODE)
# define C_LABEL_DECL           CConcatCode(2,C_DECL_NODE)
# define C_CONST_DECL           CConcatCode(3,C_DECL_NODE)
# define C_TYPE_DECL            CConcatCode(4,C_DECL_NODE)
# define C_VAR_DECL             CConcatCode(5,C_DECL_NODE)
# define C_PARAM_DECL           CConcatCode(6,C_DECL_NODE)
# define C_RESULT_DECL          CConcatCode(7,C_DECL_NODE)
# define C_FIELD_DECL           CConcatCode(8,C_DECL_NODE)

# define C_MAX_DECL_CODE        9

/*------------------------------------------------------------\
|                                                             |
|                           Tree Expression                   |
|                                                             |
\------------------------------------------------------------*/

# define C_UNKNOWN_EXPR         CConcatCode(0,C_EXPR_NODE)
# define C_CONSTRUCTOR_EXPR     CConcatCode(1,C_EXPR_NODE)
# define C_COMPOUND_EXPR        CConcatCode(2,C_EXPR_NODE)
# define C_MODIFY_EXPR          CConcatCode(3,C_EXPR_NODE)
# define C_INIT_EXPR            CConcatCode(4,C_EXPR_NODE)
# define C_TARGET_EXPR          CConcatCode(5,C_EXPR_NODE)
# define C_COND_EXPR            CConcatCode(6,C_EXPR_NODE)
# define C_BIND_EXPR            CConcatCode(7,C_EXPR_NODE)
# define C_CALL_EXPR            CConcatCode(8,C_EXPR_NODE)
# define C_METHOD_CALL_EXPR     CConcatCode(9,C_EXPR_NODE)
# define C_WITH_CLEANUP_EXPR    CConcatCode(10,C_EXPR_NODE)
# define C_CLEANUP_POINT_EXPR   CConcatCode(11,C_EXPR_NODE)
# define C_PLACEHOLDER_EXPR     CConcatCode(12,C_EXPR_NODE)
# define C_WITH_RECORD_EXPR     CConcatCode(13,C_EXPR_NODE)
# define C_PLUS_EXPR            CConcatCode(14,C_EXPR_NODE)
# define C_MINUS_EXPR           CConcatCode(15,C_EXPR_NODE)
# define C_MULT_EXPR            CConcatCode(16,C_EXPR_NODE)
# define C_TRUNC_DIV_EXPR       CConcatCode(17,C_EXPR_NODE)
# define C_CEIL_DIV_EXPR        CConcatCode(18,C_EXPR_NODE)
# define C_FLOOR_DIV_EXPR       CConcatCode(19,C_EXPR_NODE)
# define C_ROUND_DIV_EXPR       CConcatCode(20,C_EXPR_NODE)
# define C_TRUNC_MOD_EXPR       CConcatCode(21,C_EXPR_NODE)
# define C_CEIL_MOD_EXPR        CConcatCode(22,C_EXPR_NODE)
# define C_FLOOR_MOD_EXPR       CConcatCode(23,C_EXPR_NODE)
# define C_ROUND_MOD_EXPR       CConcatCode(24,C_EXPR_NODE)
# define C_RDIV_EXPR            CConcatCode(25,C_EXPR_NODE)
# define C_EXACT_DIV_EXPR       CConcatCode(26,C_EXPR_NODE)
# define C_FIX_TRUNC_EXPR       CConcatCode(27,C_EXPR_NODE)
# define C_FIX_CEIL_EXPR        CConcatCode(28,C_EXPR_NODE)
# define C_FIX_FLOOR_EXPR       CConcatCode(29,C_EXPR_NODE)
# define C_FIX_ROUND_EXPR       CConcatCode(30,C_EXPR_NODE)
# define C_FLOAT_EXPR           CConcatCode(31,C_EXPR_NODE)
# define C_EXPON_EXPR           CConcatCode(32,C_EXPR_NODE)
# define C_NEGATE_EXPR          CConcatCode(33,C_EXPR_NODE)
# define C_MIN_EXPR             CConcatCode(34,C_EXPR_NODE)
# define C_MAX_EXPR             CConcatCode(35,C_EXPR_NODE)
# define C_ABS_EXPR             CConcatCode(36,C_EXPR_NODE)
# define C_FFS_EXPR             CConcatCode(37,C_EXPR_NODE)
# define C_LSHIFT_EXPR          CConcatCode(38,C_EXPR_NODE)
# define C_RSHIFT_EXPR          CConcatCode(39,C_EXPR_NODE)
# define C_LROTATE_EXPR         CConcatCode(40,C_EXPR_NODE)
# define C_RROTATE_EXPR         CConcatCode(41,C_EXPR_NODE)
# define C_BIT_IOR_EXPR         CConcatCode(42,C_EXPR_NODE)
# define C_BIT_XOR_EXPR         CConcatCode(43,C_EXPR_NODE)
# define C_BIT_AND_EXPR         CConcatCode(44,C_EXPR_NODE)
# define C_BIT_ANDTC_EXPR       CConcatCode(45,C_EXPR_NODE)
# define C_BIT_NOT_EXPR         CConcatCode(46,C_EXPR_NODE)
# define C_TRUTH_ANDIF_EXPR     CConcatCode(47,C_EXPR_NODE)
# define C_TRUTH_ORIF_EXPR      CConcatCode(48,C_EXPR_NODE)
# define C_TRUTH_AND_EXPR       CConcatCode(49,C_EXPR_NODE)
# define C_TRUTH_OR_EXPR        CConcatCode(50,C_EXPR_NODE)
# define C_TRUTH_XOR_EXPR       CConcatCode(51,C_EXPR_NODE)
# define C_TRUTH_NOT_EXPR       CConcatCode(52,C_EXPR_NODE)
# define C_LT_EXPR              CConcatCode(53,C_EXPR_NODE)
# define C_LE_EXPR              CConcatCode(54,C_EXPR_NODE)
# define C_GT_EXPR              CConcatCode(55,C_EXPR_NODE)
# define C_GE_EXPR              CConcatCode(56,C_EXPR_NODE)
# define C_EQ_EXPR              CConcatCode(57,C_EXPR_NODE)
# define C_NE_EXPR              CConcatCode(58,C_EXPR_NODE)
# define C_IN_EXPR              CConcatCode(59,C_EXPR_NODE)
# define C_SET_LE_EXPR          CConcatCode(60,C_EXPR_NODE)
# define C_CARD_EXPR            CConcatCode(61,C_EXPR_NODE)
# define C_RANGE_EXPR           CConcatCode(62,C_EXPR_NODE)
# define C_CONVERT_EXPR         CConcatCode(63,C_EXPR_NODE)
# define C_NOP_EXPR             CConcatCode(64,C_EXPR_NODE)
# define C_NON_LVALUE_EXPR      CConcatCode(65,C_EXPR_NODE)
# define C_SAVE_EXPR            CConcatCode(66,C_EXPR_NODE)
# define C_UNSAVE_EXPR          CConcatCode(67,C_EXPR_NODE)
# define C_RTL_EXPR             CConcatCode(68,C_EXPR_NODE)
# define C_ADDR_EXPR            CConcatCode(69,C_EXPR_NODE)
# define C_REFERENCE_EXPR       CConcatCode(70,C_EXPR_NODE)
# define C_ENTRY_VALUE_EXPR     CConcatCode(71,C_EXPR_NODE)
# define C_COMPLEX_EXPR         CConcatCode(72,C_EXPR_NODE)
# define C_CONJ_EXPR            CConcatCode(73,C_EXPR_NODE)
# define C_REALPART_EXPR        CConcatCode(74,C_EXPR_NODE)
# define C_IMAGPART_EXPR        CConcatCode(75,C_EXPR_NODE)
# define C_PREDECREMENT_EXPR    CConcatCode(76,C_EXPR_NODE)
# define C_PREINCREMENT_EXPR    CConcatCode(77,C_EXPR_NODE)
# define C_POSTDECREMENT_EXPR   CConcatCode(78,C_EXPR_NODE)
# define C_POSTINCREMENT_EXPR   CConcatCode(79,C_EXPR_NODE)
# define C_TRY_CATCH_EXPR       CConcatCode(80,C_EXPR_NODE)
# define C_POPDHC_EXPR          CConcatCode(81,C_EXPR_NODE)
# define C_POPDCC_EXPR          CConcatCode(82,C_EXPR_NODE)
# define C_LABEL_EXPR           CConcatCode(83,C_EXPR_NODE)
# define C_GOTO_EXPR            CConcatCode(84,C_EXPR_NODE)
# define C_RETURN_EXPR          CConcatCode(85,C_EXPR_NODE)
# define C_EXIT_EXPR            CConcatCode(86,C_EXPR_NODE)
# define C_LOOP_EXPR            CConcatCode(87,C_EXPR_NODE)

# define C_COMPONENT_REF        CConcatCode(88,C_EXPR_NODE)
# define C_BIT_FIELD_REF        CConcatCode(89,C_EXPR_NODE)
# define C_INDIRECT_REF         CConcatCode(90,C_EXPR_NODE)
# define C_BUFFER_REF           CConcatCode(91,C_EXPR_NODE)
# define C_ARRAY_REF            CConcatCode(92,C_EXPR_NODE)

# define C_MAX_EXPR_CODE         93

/*------------------------------------------------------------\
|                                                             |
|                         Tree Instruction                    |
|                                                             |
\------------------------------------------------------------*/

# define C_UNKNOWN_INST     CConcatCode(0,C_INST_NODE)
# define C_FOR_INST         CConcatCode(1,C_INST_NODE)
# define C_WHILE_INST       CConcatCode(2,C_INST_NODE)
# define C_DO_INST          CConcatCode(3,C_INST_NODE)
# define C_IF_INST          CConcatCode(4,C_INST_NODE)
# define C_GOTO_INST        CConcatCode(5,C_INST_NODE)
# define C_RETURN_INST      CConcatCode(6,C_INST_NODE)
# define C_CONTINUE_INST    CConcatCode(7,C_INST_NODE)
# define C_BREAK_INST       CConcatCode(8,C_INST_NODE)
# define C_SWITCH_INST      CConcatCode(9,C_INST_NODE)
# define C_CASE_INST        CConcatCode(10,C_INST_NODE)

# define C_MAX_INST_CODE   11

/*------------------------------------------------------------\
|                                                             |
|                            Case Code                        |
|                                                             |
\------------------------------------------------------------*/

# define C_STANDARD_CASE   0
# define C_RANGE_CASE      1
# define C_DEFAULT_CASE    2

# define C_MAX_CASE_CODE   3

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define c_sizeof_numbits( N )    (sizeof( N ) * 8)

# define CStripNops( E ) \
  \
  while ((   CTreeExprCode( E ) == C_NOP_EXPR         \
          || CTreeExprCode( E ) == C_CONVERT_EXPR     \
          || CTreeExprCode( E ) == C_NON_LVALUE_EXPR) \
    && ( CTreeTypeMode( CTreeType( E ))          \
       == CTreeTypeMode( CTreeType( CTreeExprOperand( E, 0))))) \
    ( E ) = CTreeExprOperand( E, 0);

# define CStripTypeNops( E ) \
    \
  while (( CTreeExprCode( E ) == C_NOP_EXPR        \
          || CTreeExprCode( E ) == C_CONVERT_EXPR  \
          || CTreeExprCode( E ) == C_NON_LVALUE_EXPR)   \
         && ( CTreeType( E )                           \
             == CTreeType( CTreeExprOperand( E , 0))))    \
    ( E ) = CTreeExprOperand( E, 0);

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

# define CTreeNodeFirstCode(N)  (CFirstCode((N)->COMMON.NODE_CODE))
# define CTreeNodeExtraCode(N)  (CExtraCode((N)->COMMON.NODE_CODE))

# define CTreeNodeCode(N)     ((N)->COMMON.NODE_CODE)
# define CTreeChain(N)        ((N)->COMMON.CHAIN)
# define CTreeType(N)         ((N)->COMMON.TYPE)
# define CTreeFlags(N)        ((N)->COMMON.FLAGS)
# define CTreeUser(N)         ((N)->COMMON.USER)
# define CTreeFileName(N)     ((N)->COMMON.FILE_NAME)
# define CTreeLineNum(N)      ((N)->COMMON.LINE_NUM)

# define C_TREE_NODE_MARK_MASK         0x00001
# define C_TREE_NODE_EXTERNAL_MASK     0x00002
# define C_TREE_NODE_STATIC_MASK       0x00004
# define C_TREE_NODE_PUBLIC_MASK       0x00008
# define C_TREE_NODE_PRIVATE_MASK      0x00010
# define C_TREE_NODE_UNSIGNED_MASK     0x00020
# define C_TREE_NODE_READONLY_MASK     0x00040
# define C_TREE_NODE_VOLATILE_MASK     0x00080
# define C_TREE_NODE_CONSTANT_MASK     0x00100
# define C_TREE_NODE_SIDE_EFFECT_MASK  0x00200
# define C_TREE_NODE_CDECL_LABEL_MASK  0x00400
# define C_TREE_NODE_USED_MASK         0x00800
# define C_TREE_NODE_ASM_WRITTEN_MASK  0x01000
# define C_TREE_NODE_PERMANENT_MASK    0x02000
# define C_TREE_NODE_RAISES_MASK       0x04000
# define C_TREE_NODE_ADDRESSABLE_MASK  0x08000
# define C_TREE_NODE_OVERFLOW_MASK     0x10000
# define C_TREE_NODE_CST_OVERFLOW_MASK 0x20000

# define CIsTreeNodeMark( N )        ((N)->COMMON.FLAGS & C_TREE_NODE_MARK_MASK )
# define CSetTreeNodeMark( N )       ((N)->COMMON.FLAGS |= C_TREE_NODE_MARK_MASK)
# define CClearTreeNodeMark( N )     ((N)->COMMON.FLAGS &= ~C_TREE_NODE_MARK_MASK)

# define CIsTreeNodeExternal( N )    ((N)->COMMON.FLAGS & C_TREE_NODE_EXTERNAL_MASK )
# define CSetTreeNodeExternal( N )   ((N)->COMMON.FLAGS |= C_TREE_NODE_EXTERNAL_MASK)
# define CClearTreeNodeExternal( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_EXTERNAL_MASK)

# define CIsTreeNodeStatic( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_STATIC_MASK )
# define CSetTreeNodeStatic( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_STATIC_MASK)
# define CClearTreeNodeStatic( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_STATIC_MASK)

# define CIsTreeNodePublic( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_PUBLIC_MASK )
# define CSetTreeNodePublic( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_PUBLIC_MASK)
# define CClearTreeNodePublic( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_PUBLIC_MASK)

# define CIsTreeNodePrivate( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_PRIVATE_MASK )
# define CSetTreeNodePrivate( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_PRIVATE_MASK)
# define CClearTreeNodePrivate( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_PRIVATE_MASK)

# define CIsTreeNodeUnsigned( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_UNSIGNED_MASK )
# define CSetTreeNodeUnsigned( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_UNSIGNED_MASK)
# define CClearTreeNodeUnsigned( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_UNSIGNED_MASK)

# define CIsTreeNodeReadOnly( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_READONLY_MASK )
# define CSetTreeNodeReadOnly( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_READONLY_MASK)
# define CClearTreeNodeReadOnly( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_READONLY_MASK)

# define CIsTreeNodeVolatile( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_VOLATILE_MASK )
# define CSetTreeNodeVolatile( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_VOLATILE_MASK)
# define CClearTreeNodeVolatile( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_VOLATILE_MASK)

# define CIsTreeNodeConstant( N )      ((N)->COMMON.FLAGS & C_TREE_NODE_CONSTANT_MASK )
# define CSetTreeNodeConstant( N )     ((N)->COMMON.FLAGS |= C_TREE_NODE_CONSTANT_MASK)
# define CClearTreeNodeConstant( N )   ((N)->COMMON.FLAGS &= ~C_TREE_NODE_CONSTANT_MASK)

# define CIsTreeNodeSideEffect( N )    ((N)->COMMON.FLAGS & C_TREE_NODE_SIDE_EFFECT_MASK )
# define CSetTreeNodeSideEffect( N )   ((N)->COMMON.FLAGS |= C_TREE_NODE_SIDE_EFFECT_MASK)
# define CClearTreeNodeSideEffect( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_SIDE_EFFECT_MASK)

# define CIsTreeNodeCDeclLabel( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_CDECL_LABEL_MASK )
# define CSetTreeNodeCDeclLabel( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_CDECL_LABEL_MASK)
# define CClearTreeNodeCDeclLabel( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_CDECL_LABEL_MASK)

# define CIsTreeNodeUsed( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_USED_MASK )
# define CSetTreeNodeUsed( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_USED_MASK)
# define CClearTreeNodeUsed( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_USED_MASK)

# define CIsTreeNodeAsmWritten( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_ASM_WRITTEN_MASK )
# define CSetTreeNodeAsmWritten( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_ASM_WRITTEN_MASK)
# define CClearTreeNodeAsmWritten( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_ASM_WRITTEN_MASK)

# define CIsTreeNodePermanent( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_PERMANENT_MASK )
# define CSetTreeNodePermanent( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_PERMANENT_MASK)
# define CClearTreeNodePermanent( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_PERMANENT_MASK)

# define CIsTreeNodeRaises( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_RAISES_MASK )
# define CSetTreeNodeRaises( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_RAISES_MASK)
# define CClearTreeNodeRaises( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_RAISES_MASK)

# define CIsTreeNodeAddressable( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_ADDRESSABLE_MASK )
# define CSetTreeNodeAddressable( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_ADDRESSABLE_MASK)
# define CClearTreeNodeAddressable( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_ADDRESSABLE_MASK)

# define CIsTreeNodeOverflow( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_OVERFLOW_MASK )
# define CSetTreeNodeOverflow( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_OVERFLOW_MASK)
# define CClearTreeNodeOverflow( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_OVERFLOW_MASK)

# define CIsTreeNodeCstOverflow( N )  ((N)->COMMON.FLAGS & C_TREE_NODE_CST_OVERFLOW_MASK )
# define CSetTreeNodeCstOverflow( N ) ((N)->COMMON.FLAGS |= C_TREE_NODE_CST_OVERFLOW_MASK)
# define CClearTreeNodeCstOverflow( N ) ((N)->COMMON.FLAGS &= ~C_TREE_NODE_CST_OVERFLOW_MASK)

  typedef struct c_tree_common
  {
    union     c_tree_node *CHAIN;
    union     c_tree_node *TYPE;
    union     c_tree_node *NEXT;
    char                  *FILE_NAME;
    int                    LINE_NUM;
    short                  NODE_CODE;
    long                   FLAGS;
    void                  *USER;

  } c_tree_common;

# define CTreeIntCstLow(N)   ((N)->INT_CST.LOW)
# define CTreeIntCstHigh(N)  ((N)->INT_CST.HIGH)

  typedef struct c_tree_int_cst
  {
    struct c_tree_common COMMON;
    long                 LOW;
    long                 HIGH;

  } c_tree_int_cst;

# define CTreeRealCstValue(N)   ((N)->REAL_CST.VALUE)

  typedef struct c_tree_real_cst
  {
    struct c_tree_common COMMON;
    double               VALUE;

  } c_tree_real_cst;

# define CTreeStringLength(N)  ((int   )(N)->STRING.LENGTH)
# define CTreeStringPointer(N) ((char *)(N)->STRING.POINTER)

  typedef struct c_tree_string
  {
    struct c_tree_common COMMON;
    int                  LENGTH;
    char                *POINTER;

  } c_tree_string;

# define CTreeComplexReal(N)  ((N)->COMPLEX.REAL)
# define CTreeComplexImag(N)  ((N)->COMPLEX.IMAG)

  typedef struct c_tree_complex
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *REAL;
    union c_tree_node    *IMAG;

  } c_tree_complex;

# define CTreeIdentLength(N)       ((N)->IDENT.LENGTH )
# define CTreeIdentPointer(N)      ((N)->IDENT.POINTER)
# define CTreeIdentGlobalValue(N)  ((N)->IDENT.GLOBAL_VALUE)
# define CTreeIdentLocalValue(N)   ((N)->IDENT.LOCAL_VALUE)
# define CTreeIdentLabelValue(N)   ((N)->IDENT.LABEL_VALUE)
# define CTreeIdentImplicitDecl(N) ((N)->IDENT.IMPLICIT_DECL)
# define CTreeIdentErrorLocus(N)   ((N)->IDENT.ERROR_LOCUS)
# define CTreeIdentLimboValue(N)   ((N)->IDENT.LIMBO_VALUE)

  typedef struct c_tree_ident
  {
    struct c_tree_common  COMMON;
    int                   LENGTH;
    char                 *POINTER;
    union c_tree_node    *GLOBAL_VALUE;
    union c_tree_node    *LOCAL_VALUE;
    union c_tree_node    *LABEL_VALUE;
    union c_tree_node    *IMPLICIT_DECL;
    union c_tree_node    *ERROR_LOCUS;
    union c_tree_node    *LIMBO_VALUE;

  } c_tree_ident;

# define CTreeListPurpose( N ) ((N)->LIST.PURPOSE)
# define CTreeListValue( N )   ((N)->LIST.VALUE)

  typedef struct c_tree_list
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *PURPOSE;
    union c_tree_node    *VALUE;

  } c_tree_list;

# define CTreeVecLength( N )       ((N)->VEC.LENGTH)
# define CTreeVecElem( N, I )      ((N)->VEC.ARRAY[(I)])

  typedef struct c_tree_vec
  {
    struct c_tree_common  COMMON;
    int                   LENGTH;
    union c_tree_node   **ARRAY;

  } c_tree_vec;

# define CTreeExprCode( N )        CTreeNodeCode(N)
# define CTreeExprComplexity( N )  ((N)->EXPR.COMPLEXITY)
# define CTreeExprNumOper( N )     ((N)->EXPR.NUM_OPER)
# define CTreeExprOperand( N, I )  ((N)->EXPR.OPERANDS[(I)])

  typedef struct c_tree_expr
  {
    struct c_tree_common  COMMON;
    int                   COMPLEXITY;
    int                   NUM_OPER;
    union c_tree_node   **OPERANDS;

  } c_tree_expr;

# define CTreeBlockVars( N )            ((N)->BLOCK.VARS)
# define CTreeBlockTypeTags( N )        ((N)->BLOCK.TYPE_TAGS)
# define CTreeBlockSubBlocks( N )       ((N)->BLOCK.SUB_BLOCKS)
# define CTreeBlockSuperContext( N )    ((N)->BLOCK.SUPER_CONTEXT)
# define CTreeBlockAbstractOrigin( N )  ((N)->BLOCK.ABSTRACT_ORIGIN)
# define CTreeBlockBody( N )            ((N)->BLOCK.BODY)

  typedef struct c_tree_block
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *VARS;
    union c_tree_node    *TYPE_TAGS;
    union c_tree_node    *SUB_BLOCKS;
    union c_tree_node    *SUPER_CONTEXT;
    union c_tree_node    *ABSTRACT_ORIGIN;
    union c_tree_node    *BODY;

  } c_tree_block;

# define CTreeTypeCode( N )            CTreeNodeCode(N)
# define CTreeTypeValues( N )          ((N)->TYPE.VALUES)
# define CTreeTypeSize( N )            ((N)->TYPE.SIZE)
# define CTreeTypeAttributes( N )      ((N)->TYPE.ATTRIBUTES)
# define CTreeTypeId( N )              ((N)->TYPE.TYPE_ID)
# define CTreeTypeMode( N )            ((N)->TYPE.MODE)
# define CTreeTypePrecision( N )       ((N)->TYPE.PRECISION)
# define CTreeTypeAlign( N )           ((N)->TYPE.ALIGN)
# define CTreeTypePointerTo( N )       ((N)->TYPE.POINTER_TO)
# define CTreeTypeReferenceTo( N )     ((N)->TYPE.REFERENCE_TO)
# define CTreeTypeName( N )            ((N)->TYPE.NAME)
# define CTreeTypeMinValue( N )        ((N)->TYPE.MIN_VALUE)
# define CTreeTypeMaxValue( N )        ((N)->TYPE.MAX_VALUE)
# define CTreeTypeNextVariant( N )     ((N)->TYPE.NEXT_VARIANT)
# define CTreeTypeMainVariant( N )     ((N)->TYPE.MAIN_VARIANT)
# define CTreeTypeBInfo( N )           ((N)->TYPE.BINFO)
# define CTreeTypeNonCopiedParts( N )  ((N)->TYPE.NONCOPIED_PARTS)
# define CTreeTypeContext( N )         ((N)->TYPE.CONTEXT)

# define C_TREE_TYPE_VAR_SIZE_MASK         0x01
# define C_TREE_TYPE_TRANS_UNION_MASK      0x02
# define C_TREE_TYPE_FIELDS_RD_ONLY_MASK   0x04
# define C_TREE_TYPE_FIELDS_VOLAT_MASK     0x08

# define CIsTreeTypeVarSize( N )    ((N)->TYPE.FLAGS & C_TREE_TYPE_VAR_SIZE_MASK )
# define CSetTreeTypeVarSize( N )   ((N)->TYPE.FLAGS |= C_TREE_TYPE_VAR_SIZE_MASK)
# define CClearTreeTypeVarSize( N ) ((N)->TYPE.FLAGS &= ~C_TREE_TYPE_VAR_SIZE_MASK)

# define CIsTreeTypeTransUnion( N )    ((N)->TYPE.FLAGS & C_TREE_TYPE_TRANS_UNION_MASK )
# define CSetTreeTypeTransUnion( N )   ((N)->TYPE.FLAGS |= C_TREE_TYPE_TRANS_UNION_MASK)
# define CClearTreeTypeTransUnion( N ) ((N)->TYPE.FLAGS &= ~C_TREE_TYPE_TRANS_UNION_MASK)

# define CIsTreeTypeFieldsReadOnly(N) ((N)->TYPE.FLAGS & C_TREE_TYPE_FIELDS_RD_ONLY_MASK )
# define CSetTreeTypeFieldsReadOnly(N) ((N)->TYPE.FLAGS |= C_TREE_TYPE_FIELDS_RD_ONLY_MASK)
# define CClearTreeTypeFieldsReadOnly(N) ((N)->TYPE.FLAGS &= ~C_TREE_TYPE_FIELDS_RD_ONLY_MASK)

# define CIsTreeTypeFieldsVolatile( N ) ((N)->TYPE.FLAGS & C_TREE_TYPE_FIELDS_VOLAT_MASK )
# define CSetTreeTypeFieldsVolatile( N ) ((N)->TYPE.FLAGS |= C_TREE_TYPE_FIELDS_VOLAT_MASK)
# define CClearTreeTypeFieldsVolatile( N ) ((N)->TYPE.FLAGS &= ~C_TREE_TYPE_FIELDS_VOLAT_MASK)



  typedef struct c_tree_type
  {
    struct c_tree_common  COMMON;
    union  c_tree_node   *VALUES;
    union  c_tree_node   *SIZE;
    union  c_tree_node   *ATTRIBUTES; 
    int                   TYPE_ID;
    int                   MODE;
    unsigned char         PRECISION;
    unsigned char         UNSIGNED;
    int                   ALIGN;
    union c_tree_node    *POINTER_TO;
    union c_tree_node    *REFERENCE_TO;
    union c_tree_node    *NAME;
    union c_tree_node    *MIN_VALUE;
    union c_tree_node    *MAX_VALUE;
    union c_tree_node    *NEXT_VARIANT;
    union c_tree_node    *MAIN_VARIANT;
    union c_tree_node    *BINFO;
    union c_tree_node    *NONCOPIED_PARTS;
    union c_tree_node    *CONTEXT;
    long                  FLAGS;

  } c_tree_type;

# define CTreeDeclCode(N)            CTreeNodeCode(N)
# define CTreeDeclFileName(N)        CTreeFileName(N)
# define CTreeDeclLineNum(N)         CTreeLineNum(N)
# define CTreeDeclId(N)              ((N)->DECL.DECL_ID)
# define CTreeDeclSize(N)            ((N)->DECL.SIZE)
# define CTreeDeclMode(N)            ((N)->DECL.MODE)
# define CTreeDeclFieldSize(N)       ((N)->DECL.FIELD_SIZE)
# define CTreeDeclName(N)            ((N)->DECL.NAME)
# define CTreeDeclContext(N)         ((N)->DECL.CONTEXT)
# define CTreeDeclAbstractOrigin(N)  ((N)->DECL.ABSTRACT_ORIGIN)
# define CTreeDeclArguments(N)       ((N)->DECL.ARGUMENTS)
# define CTreeDeclResult(N)          ((N)->DECL.RESULT)
# define CTreeDeclInitial(N)         ((N)->DECL.INITIAL)
# define CTreeDeclFlags(N)           ((N)->DECL.FLAGS)

# define C_TREE_DECL_EXTERNAL_MASK       0x0001
# define C_TREE_DECL_NON_LOCAL_MASK      0x0002
# define C_TREE_DECL_COMMON_MASK         0x0004
# define C_TREE_DECL_VAR_SIZE_MASK       0x0008
# define C_TREE_DECL_C_BIT_FIELD_MASK    0x0010
# define C_TREE_DECL_ITERATOR_MASK       0x0020
# define C_TREE_DECL_REGISTER_MASK       0x0040
# define C_TREE_DECL_BUILT_IN_MASK       0x0080
# define C_TREE_DECL_BIT_FIELD_MASK      0x0100
# define C_TREE_DECL_SYSTEM_HEADER_MASK  0x0100
# define C_TREE_DECL_IGNORED_MASK        0x0200
# define C_TREE_DECL_ANTICIPATED_MASK    0x0400
# define C_TREE_DECL_DECLARED_LABEL_MASK 0x0800
# define C_TREE_DECL_WEAK_MASK           0x1000
# define C_TREE_DECL_INLINE_MASK         0x2000

# define CIsTreeDeclExternal( N )     ((N)->DECL.FLAGS & C_TREE_DECL_EXTERNAL_MASK )
# define CSetTreeDeclExternal( N )    ((N)->DECL.FLAGS |= C_TREE_DECL_EXTERNAL_MASK)
# define CClearTreeDeclExternal( N )  ((N)->DECL.FLAGS &= ~C_TREE_DECL_EXTERNAL_MASK)

# define CIsTreeDeclNonLocal( N )     ((N)->DECL.FLAGS & C_TREE_DECL_NON_LOCAL_MASK )
# define CSetTreeDeclNonLocal( N )    ((N)->DECL.FLAGS |= C_TREE_DECL_NON_LOCAL_MASK)
# define CClearTreeDeclNonLocal( N )  ((N)->DECL.FLAGS &= ~C_TREE_DECL_NON_LOCAL_MASK)

# define CIsTreeDeclCommon( N )     ((N)->DECL.FLAGS & C_TREE_DECL_COMMON_MASK )
# define CSetTreeDeclCommon( N )    ((N)->DECL.FLAGS |= C_TREE_DECL_COMMON_MASK)
# define CClearTreeDeclCommon( N )  ((N)->DECL.FLAGS &= ~C_TREE_DECL_COMMON_MASK)

# define CIsTreeDeclVarSize( N )     ((N)->DECL.FLAGS & C_TREE_DECL_VAR_SIZE_MASK )
# define CSetTreeDeclVarSize( N )    ((N)->DECL.FLAGS |= C_TREE_DECL_VAR_SIZE_MASK)
# define CClearTreeDeclVarSize( N )  ((N)->DECL.FLAGS &= ~C_TREE_DECL_VAR_SIZE_MASK)

# define CIsTreeDeclCBitField( N )    ((N)->DECL.FLAGS & C_TREE_DECL_C_BIT_FIELD_MASK )
# define CSetTreeDeclCBitField( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_C_BIT_FIELD_MASK)
# define CClearTreeDeclCBitField( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_C_BIT_FIELD_MASK)

# define CIsTreeDeclIterator( N )    ((N)->DECL.FLAGS & C_TREE_DECL_ITERATOR_MASK)
# define CSetTreeDeclIterator( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_ITERATOR_MASK)
# define CClearTreeDeclIterator( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_ITERATOR_MASK)

# define CIsTreeDeclRegister( N )    ((N)->DECL.FLAGS & C_TREE_DECL_REGISTER_MASK)
# define CSetTreeDeclRegister( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_REGISTER_MASK)
# define CClearTreeDeclRegister( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_REGISTER_MASK)

# define CIsTreeDeclBuiltIn( N )    ((N)->DECL.FLAGS & C_TREE_DECL_BUILT_IN_MASK)
# define CSetTreeDeclBuiltIn( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_BUILT_IN_MASK)
# define CClearTreeDeclBuiltIn( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_BUILT_IN_MASK)

# define CIsTreeDeclBitField( N )    ((N)->DECL.FLAGS & C_TREE_DECL_BIT_FIELD_MASK )
# define CSetTreeDeclBitField( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_BIT_FIELD_MASK)
# define CClearTreeDeclBitField( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_BIT_FIELD_MASK)

# define CIsTreeDeclSystemHeader( N )    ((N)->DECL.FLAGS & C_TREE_DECL_SYSTEM_HEADER_MASK )
# define CSetTreeDeclSystemHeader( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_SYSTEM_HEADER_MASK)
# define CClearTreeDeclSystemHeader( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_SYSTEM_HEADER_MASK)

# define CIsTreeDeclIgnored( N )    ((N)->DECL.FLAGS & C_TREE_DECL_IGNORED_MASK )
# define CSetTreeDeclIgnored( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_IGNORED_MASK)
# define CClearTreeDeclIgnored( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_IGNORED_MASK)

# define CIsTreeDeclAnticipated( N )    ((N)->DECL.FLAGS & C_TREE_DECL_ANTICIPATED_MASK )
# define CSetTreeDeclAnticipated( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_ANTICIPATED_MASK)
# define CClearTreeDeclAnticipated( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_ANTICIPATED_MASK)

# define CIsTreeDeclDeclaredLabel( N )    ((N)->DECL.FLAGS & C_TREE_DECL_DECLARED_LABEL_MASK )
# define CSetTreeDeclDeclaredLabel( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_DECLARED_LABEL_MASK)
# define CClearTreeDeclDeclaredLabel( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_DECLARED_LABEL_MASK)

# define CIsTreeDeclWeak( N )    ((N)->DECL.FLAGS & C_TREE_DECL_WEAK_MASK )
# define CSetTreeDeclWeak( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_WEAK_MASK)
# define CClearTreeDeclWeak( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_WEAK_MASK)

# define CIsTreeDeclInline( N )    ((N)->DECL.FLAGS & C_TREE_DECL_INLINE_MASK )
# define CSetTreeDeclInline( N )   ((N)->DECL.FLAGS |= C_TREE_DECL_INLINE_MASK)
# define CClearTreeDeclInline( N ) ((N)->DECL.FLAGS &= ~C_TREE_DECL_INLINE_MASK)

  typedef struct c_tree_decl
  {
    struct c_tree_common    COMMON;
    int                     DECL_ID;
    union c_tree_node      *SIZE;
    int                     MODE;
    int                     FIELD_SIZE;
    union c_tree_node      *NAME;
    union c_tree_node      *CONTEXT;
    union c_tree_node      *ABSTRACT_ORIGIN;
    union c_tree_node      *ARGUMENTS;
    union c_tree_node      *RESULT;
    union c_tree_node      *INITIAL;
    long                    FLAGS;

  } c_tree_decl;

# define CTreeInstCaseCode(N) ((N)->INST.CLASS.CASE.CASE_CODE)
# define CTreeInstCaseFrom(N) ((N)->INST.CLASS.CASE.FROM)
# define CTreeInstCaseTo(N)   ((N)->INST.CLASS.CASE.TO)

  typedef struct c_tree_inst_case
  {
    struct c_tree_common  COMMON;
    short                 CASE_CODE;
    union c_tree_node    *FROM;
    union c_tree_node    *TO;

  } c_tree_inst_case;

# define CTreeInstSwitchOn(N)   ((N)->INST.CLASS.SWITCH.ON)
# define CTreeInstSwitchBody(N) ((N)->INST.CLASS.SWITCH.BODY)

  typedef struct c_tree_inst_switch
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *ON;
    union c_tree_node    *BODY;

  } c_tree_inst_switch;

  typedef struct c_tree_inst_break
  {
    struct c_tree_common  COMMON;

  } c_tree_inst_break;

  typedef struct c_tree_inst_continue
  {
    struct c_tree_common  COMMON;

  } c_tree_inst_continue;

# define CTreeInstReturnValue(N) ((N)->INST.CLASS.RETURN.VALUE)

  typedef struct c_tree_inst_return
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *VALUE;

  } c_tree_inst_return;

# define CTreeInstGotoLabel(N) ((N)->INST.CLASS.GOTO.LABEL)

  typedef struct c_tree_inst_goto
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *LABEL;

  } c_tree_inst_goto;

# define CTreeInstForStart(N) ((N)->INST.CLASS.FOR.START)
# define CTreeInstForCond(N)  ((N)->INST.CLASS.FOR.COND)
# define CTreeInstForInc(N)   ((N)->INST.CLASS.FOR.INC)
# define CTreeInstForBody(N)  ((N)->INST.CLASS.FOR.BODY)

  typedef struct c_tree_inst_for
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *START;
    union c_tree_node    *COND;
    union c_tree_node    *INC;
    union c_tree_node    *BODY;

  } c_tree_inst_for;

# define CTreeInstWhileCond(N)  ((N)->INST.CLASS.WHILE.COND)
# define CTreeInstWhileBody(N)  ((N)->INST.CLASS.WHILE.BODY)

  typedef struct c_tree_inst_while
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *COND;
    union c_tree_node    *BODY;

  } c_tree_inst_while;

# define CTreeInstDoCond(N)  ((N)->INST.CLASS.DO.COND)
# define CTreeInstDoBody(N)  ((N)->INST.CLASS.DO.BODY)

  typedef struct c_tree_inst_do
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *COND;
    union c_tree_node    *BODY;

  } c_tree_inst_do;

# define CTreeInstIfCond(N) ((N)->INST.CLASS.IF.COND)
# define CTreeInstIfThen(N) ((N)->INST.CLASS.IF.THEN)
# define CTreeInstIfElse(N) ((N)->INST.CLASS.IF.ELSE)

  typedef struct c_tree_inst_if
  {
    struct c_tree_common  COMMON;
    union c_tree_node    *COND;
    union c_tree_node    *THEN;
    union c_tree_node    *ELSE;

  } c_tree_inst_if;

# define CTreeInstCode(N) CTreeNodeCode(N)

  typedef struct c_tree_inst
  {
    struct c_tree_common  COMMON;
    union
    {
      c_tree_inst_for      FOR;
      c_tree_inst_while    WHILE;
      c_tree_inst_do       DO;
      c_tree_inst_if       IF;
      c_tree_inst_goto     GOTO;
      c_tree_inst_return   RETURN;
      c_tree_inst_continue CONTINUE;
      c_tree_inst_break    BREAK;
      c_tree_inst_switch   SWITCH;
      c_tree_inst_case     CASE;

    }                     CLASS;
  } c_tree_inst;

  typedef union c_tree_node
  {
    c_tree_common     COMMON;
    c_tree_int_cst    INT_CST;
    c_tree_real_cst   REAL_CST;
    c_tree_string     STRING;
    c_tree_complex    COMPLEX;
    c_tree_ident      IDENT;
    c_tree_decl       DECL;
    c_tree_type       TYPE;
    c_tree_list       LIST;
    c_tree_vec        VEC;
    c_tree_expr       EXPR;
    c_tree_block      BLOCK;
    c_tree_inst       INST;

  } c_tree_node;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char        *C_NODE_CODE_NAME[ C_MAX_NODE_CODE ];
  extern int          C_NODE_SIZE[ C_MAX_NODE_CODE ];
  extern char        *C_EXPR_CODE_NAME[ C_MAX_EXPR_CODE ];
  extern char        *C_TYPE_CODE_NAME[ C_MAX_TYPE_CODE ];
  extern char        *C_DECL_CODE_NAME[ C_MAX_DECL_CODE ];
  extern char        *C_INST_CODE_NAME[ C_MAX_INST_CODE ];
  extern char        *C_CASE_CODE_NAME[ C_MAX_CASE_CODE ];

  extern short        C_TREE_FORCE_VIEW_NODE;
  extern short        C_TREE_VIEW_TYPE_NODE;
  extern short        C_TREE_VIEW_SIZE_NODE;
  extern c_tree_node *C_HEAD_NODE_LIST[ C_MAX_NODE_CODE ];

  extern c_tree_node *c_error_mark_node;
  extern c_tree_node *c_size_type;
  extern c_tree_node *c_size_zero_node;
  extern c_tree_node *c_size_one_node;
  extern c_tree_node *c_short_integer_type_node;
  extern c_tree_node *c_integer_type_node;
  extern c_tree_node *c_long_integer_type_node;
  extern c_tree_node *c_long_long_integer_type_node;
  extern c_tree_node *c_short_unsigned_type_node;
  extern c_tree_node *c_unsigned_type_node;
  extern c_tree_node *c_long_unsigned_type_node;
  extern c_tree_node *c_long_long_unsigned_type_node;
  extern c_tree_node *c_boolean_type_node;
  extern c_tree_node *c_boolean_false_node;
  extern c_tree_node *c_boolean_true_node;
  extern c_tree_node *c_ptrdiff_type_node;
  extern c_tree_node *c_unsigned_char_type_node;
  extern c_tree_node *c_signed_char_type_node;
  extern c_tree_node *c_char_type_node;
  extern c_tree_node *c_wchar_type_node;
  extern c_tree_node *c_signed_wchar_type_node;
  extern c_tree_node *c_unsigned_wchar_type_node;
  extern c_tree_node *c_float_type_node;
  extern c_tree_node *c_double_type_node;
  extern c_tree_node *c_long_double_type_node;
  extern c_tree_node *c_complex_integer_type_node;
  extern c_tree_node *c_complex_float_type_node;
  extern c_tree_node *c_complex_double_type_node;
  extern c_tree_node *c_complex_long_double_type_node;
  extern c_tree_node *c_void_type_node;
  extern c_tree_node *c_ptr_type_node;
  extern c_tree_node *c_const_ptr_type_node;
  extern c_tree_node *c_string_type_node;
  extern c_tree_node *c_const_string_type_node;
  extern c_tree_node *c_char_array_type_node;
  extern c_tree_node *c_int_array_type_node;
  extern c_tree_node *c_wchar_array_type_node;
  extern c_tree_node *c_default_function_type;
  extern c_tree_node *c_double_ftype_double;
  extern c_tree_node *c_double_ftype_double_double;
  extern c_tree_node *c_int_ftype_int;
  extern c_tree_node *c_long_ftype_long;
  extern c_tree_node *c_float_ftype_float;
  extern c_tree_node *c_ldouble_ftype_ldouble;
  extern c_tree_node *c_void_ftype_ptr_ptr_int; 
  extern c_tree_node *c_int_ftype_ptr_ptr_int;
  extern c_tree_node *c_void_ftype_ptr_int_int;
  extern c_tree_node *c_string_ftype_ptr_ptr;
  extern c_tree_node *c_int_ftype_string_string;
  extern c_tree_node *c_int_ftype_cptr_cptr_sizet;
  extern c_tree_node *c_integer_zero_node;
  extern c_tree_node *c_null_pointer_node;
  extern c_tree_node *c_integer_one_node;
  extern c_tree_node *c_size_type;
  extern c_tree_node *c_size_zero_node;
  extern c_tree_node *c_size_one_node;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Tree Function                      |
|                                                             |
\------------------------------------------------------------*/

  extern   c_tree_node * c_tree_alloc_node __P(());
  extern  c_tree_node ** c_tree_alloc_node_array __P((int Length));
  extern   c_tree_node * c_tree_add_node __P((int NodeCode));
  extern   c_tree_node * c_tree_copy_node __P((c_tree_node *TreeNode));
  extern   c_tree_node * c_tree_copy_list __P((c_tree_node *TreeNode));
  extern   c_tree_node * c_tree_build_vec __P((int Length));
  extern   c_tree_node * c_tree_build_list __P((c_tree_node *Purpose, c_tree_node *Value));
  extern   c_tree_node * c_tree_build_complex __P((c_tree_node *Type, c_tree_node *Real, c_tree_node *Imag));
  extern   c_tree_node * c_tree_build_decl __P((int DeclType, c_tree_node *TreeName, c_tree_node *TreeType));
  extern   c_tree_node * c_tree_build_inst __P(( int InstCode ));
  extern   c_tree_node * c_tree_build_type __P((int TypeType));
  extern   c_tree_node * c_tree_build_type_copy __P((c_tree_node *TreeType));
  extern   c_tree_node * c_tree_build_type_variant __P((c_tree_node *TreeType, int ConstP, int VolatileP));
  extern   c_tree_node * c_tree_build_function_type __P((c_tree_node *ValueType, c_tree_node *ArgTypes));
  extern   c_tree_node * c_tree_build_complex_type __P((c_tree_node *Component));
  extern   c_tree_node * c_tree_build_signed_type __P((int Precision));
  extern   c_tree_node * c_tree_build_unsigned_type __P((int Precision));
  extern   c_tree_node * c_tree_build_pointer_type __P((c_tree_node *TreeType));
  extern   c_tree_node * c_tree_build_array_type __P((c_tree_node *ElemType, c_tree_node *IndexType));
  extern   c_tree_node * c_tree_build_index_type __P((c_tree_node *MaxVal));
  extern   c_tree_node * c_tree_build_unary_expr __P((int ExprCode, c_tree_node *TreeExpr));
  extern   c_tree_node * c_tree_build_binary_expr __P((int ExprCode, c_tree_node *TreeExpr1, c_tree_node *TreeExpr2));
  extern   c_tree_node * c_tree_build_ternary_expr __P((int ExprCode, c_tree_node *TreeExpr1, c_tree_node *TreeExpr2, c_tree_node *TreeExpr3));
  extern   c_tree_node * c_tree_build_real __P((c_tree_node *TreeType, double Value));
  extern   c_tree_node * c_tree_build_int_2 __P((long Low, long High));
  extern   c_tree_node * c_tree_build_string __P((int Length, char *Text));
  extern   c_tree_node * c_tree_build_pointer_declarator __P((c_tree_node *TreeTypeQual, c_tree_node *TreeTarget));
  extern   c_tree_node * c_tree_combine_strings __P((c_tree_node *TreeString));
  extern   c_tree_node * c_tree_get_size_int __P((int Number));
  extern            int  c_tree_get_list_length __P((c_tree_node *ListTree));
  extern   c_tree_node * c_tree_get_identifier __P((char *Text));
  extern   c_tree_node * c_tree_concat_list __P((c_tree_node *ListTree1, c_tree_node *ListTree2));
  extern   c_tree_node * c_tree_add_head_list __P((c_tree_node *Purpose, c_tree_node *Value, c_tree_node *ListTree));
  extern            int  c_tree_is_int_cst_sgn __P((c_tree_node *t));
  extern            int  c_tree_is_real_zerop __P((c_tree_node *expr));
  extern            int  c_tree_is_real_onep __P((c_tree_node *expr));
  extern            int  c_tree_is_real_twop __P((c_tree_node *expr));
  extern            int  c_tree_is_integer_zerop __P((c_tree_node *expr));
  extern            int  c_tree_is_integer_onep __P((c_tree_node *expr));
  extern            int c_tree_is_integer_all_onesp __P((c_tree_node *expr));
  extern            int  c_tree_is_int_cst_lt __P((c_tree_node *t1, c_tree_node *t2));
  extern            int c_tree_is_int_cst_equal __P((c_tree_node *t1, c_tree_node *t2));
  extern            int c_tree_is_integer_pow2p __P((c_tree_node *expr));
  extern   c_tree_node * c_tree_reverse_list __P((c_tree_node *t));
  extern  c_tree_node  * c_tree_get_last_list __P((c_tree_node *chain));
  extern           void  c_indent __P(());
  extern           void  c_tree_view_node __P((c_tree_node *TreeNode));
  extern           void  c_tree_view_node_list __P((c_tree_node *TreeNode));

  extern c_tree_node * c_tree_build_type_attribute_variant __P((c_tree_node *TreeType, c_tree_node *TreeAttr ));
  extern c_tree_node * c_tree_build_unary_typed_expr __P((int code, c_tree_node *type, c_tree_node *node));
  extern c_tree_node * c_tree_build_binary_typed_expr __P((int code, c_tree_node *type, c_tree_node * arg0, c_tree_node * arg1 ));
  extern c_tree_node * c_tree_build_ternary_typed_expr __P((int code, c_tree_node *type, c_tree_node *arg0, c_tree_node *arg1, c_tree_node *arg2 ));
  extern c_tree_node * c_tree_get_purpose_member __P((c_tree_node *elem, c_tree_node *list));

  extern char c_tree_get_code_class __P(( int NodeCode ));

  extern void c_tree_free_all_node __P(());

/*------------------------------------------------------------\
|                                                             |
|                          Parse Function                     |
|                                                             |
\------------------------------------------------------------*/

  extern c_tree_node *c_tree_parse_file __P((char *InputFileName, int Argc, char **Argv));

# endif
