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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     C                             |
|                                                             |
| File    :                   c_tree.c                        |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "gcp.h"

# include "c_tree.h"
# include "c_treecompat.h"
# include "c_debug.h"
# include "c_cdecl.h"
# include "c_debug.h"
# include "c_convert.h"
# include "c_flags.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static authtable *CTreeHashIdentifier = (authtable *)0;
  static char      *CTreeNameBuiltIn    = (char      *)0;

  char *C_NODE_CODE_NAME[ C_MAX_NODE_CODE ] =
  {
    "INT_CST",
    "REAL_CST",
    "STRING",
    "COMPLEX",
    "IDENTIFIER",
    "LIST",
    "VEC",
    "EXPR",
    "BLOCK",
    "TYPE",
    "DECL",
    "INST",
    "ERROR"
  };

  int C_NODE_SIZE[ C_MAX_NODE_CODE ] =
  {
    sizeof( c_tree_int_cst  ), /* INT_CST_NODE     */
    sizeof( c_tree_real_cst ), /* REAL_CST_NODE    */
    sizeof( c_tree_string   ), /* STRING_NODE      */
    sizeof( c_tree_complex  ), /* COMPLEX_NODE     */
    sizeof( c_tree_ident    ), /* IDENTIFIER_NODE  */
    sizeof( c_tree_list     ), /* LIST_NODE        */
    sizeof( c_tree_vec      ), /* VEC_NODE         */
    sizeof( c_tree_expr     ), /* EXPR_NODE        */
    sizeof( c_tree_block    ), /* BLOCK_NODE       */
    sizeof( c_tree_type     ), /* TYPE_NODE        */
    sizeof( c_tree_decl     ), /* DECL_NODE        */
    sizeof( c_tree_inst     ), /* INST_NODE        */
    sizeof( c_tree_common   )  /* ERROR_NODE       */
  };

  char *C_EXPR_CODE_NAME[ C_MAX_EXPR_CODE ] =
  {
    "UNKNOWN",
    "CONSTRUCTOR",
    "COMPOUND",
    "MODIFY",
    "INIT",
    "TARGET",
    "COND",
    "BIND",
    "CALL",
    "METHOD_CALL",
    "WITH_CLEANUP",
    "CLEANUP_POINT",
    "PLACEHOLDER",
    "WITH_RECORD",
    "PLUS",
    "MINUS",
    "MULT",
    "TRUNC_DIV",
    "CEIL_DIV",
    "FLOOR_DIV",
    "ROUND_DIV",
    "TRUNC_MOD",
    "CEIL_MOD",
    "FLOOR_MOD",
    "ROUND_MOD",
    "RDIV",
    "EXACT_DIV",
    "FIX_TRUNC",
    "FIX_CEIL",
    "FIX_FLOOR",
    "FIX_ROUND",
    "FLOAT",
    "EXPON",
    "NEGATE",
    "MIN",
    "MAX",
    "ABS",
    "FFS",
    "LSHIFT",
    "RSHIFT",
    "LROTATE",
    "RROTATE",
    "BIT_IOR",
    "BIT_XOR",
    "BIT_AND",
    "BIT_ANDTC",
    "BIT_NOT",
    "TRUTH_ANDIF",
    "TRUTH_ORIF",
    "TRUTH_AND",
    "TRUTH_OR",
    "TRUTH_XOR",
    "TRUTH_NOT",
    "LT",
    "LE",
    "GT",
    "GE",
    "EQ",
    "NE",
    "IN",
    "SET_LE",
    "CARD",
    "RANGE",
    "CONVERT",
    "NOP",
    "NON_LVALUE",
    "SAVE",
    "UNSAVE",
    "RTL",
    "ADDR",
    "REFERENCE",
    "ENTRY_VALUE",
    "COMPLEX",
    "CONJ",
    "REALPART",
    "IMAGPART",
    "PREDECREMENT",
    "PREINCREMENT",
    "POSTDECREMENT",
    "POSTINCREMENT",
    "TRY_CATCH",
    "POPDHC",
    "POPDCC",
    "LABEL",
    "GOTO",
    "RETURN",
    "EXIT",
    "LOOP",
    "COMPONENT_REF",
    "BIT_FIELD_REF",
    "INDIRECT_REF",
    "BUFFER_REF",
    "ARRAY_REF"
  };

  char *C_TYPE_CODE_NAME[ C_MAX_TYPE_CODE ] =
  {
    "UNKNOWN",
    "VOID",
    "INTEGER",
    "REAL",
    "COMPLEX",
    "ENUMERAL",
    "BOOLEAN",
    "CHAR",
    "POINTER",
    "OFFSET",
    "REFERENCE",
    "METHOD",
    "FILE",
    "ARRAY",
    "SET",
    "RECORD",
    "UNION",
    "QUAL_UNION",
    "FUNCTION",
    "LANG"
  };

  char *C_DECL_CODE_NAME[ C_MAX_DECL_CODE ] =
  {
    "UNKNOWN",
    "FUNCTION",
    "LABEL",
    "CONST",
    "TYPE",
    "VAR",
    "PARAM",
    "RESULT",
    "FIELD"
  };

  char *C_INST_CODE_NAME[ C_MAX_INST_CODE ] =
  {
    "UNKNOWN",
    "FOR",
    "WHILE",
    "DO",
    "IF",
    "GOTO",
    "RETURN",
    "CONTINUE",
    "BREAK",
    "SWITCH",
    "CASE"
  };

  char *C_CASE_CODE_NAME[ C_MAX_CASE_CODE ] =
  {
    "STANDARD",
    "RANGE",
    "DEFAULT"
  };

  short C_TREE_FORCE_VIEW_NODE = 0;
  short C_TREE_VIEW_TYPE_NODE  = 0;
  short C_TREE_VIEW_SIZE_NODE  = 0;

  c_tree_node *C_HEAD_NODE_LIST[ C_MAX_NODE_CODE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       C Alloc Function                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       C Alloc Tree Node                     |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_alloc_node( NodeCode )

  int NodeCode;
{
  c_tree_node *TreeNode;
  int          FirstCode;

  FirstCode = CFirstCode( NodeCode );
  TreeNode  = (c_tree_node *)autallocheap( C_NODE_SIZE[ FirstCode ] );
  CTreeNodeCode( TreeNode ) = NodeCode;

  TreeNode->COMMON.NEXT = C_HEAD_NODE_LIST[ FirstCode ];
  C_HEAD_NODE_LIST[ FirstCode ] = TreeNode;

  return( TreeNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       C Alloc Tree Node Array               |
|                                                             |
\------------------------------------------------------------*/

c_tree_node **c_tree_alloc_node_array( Length )

  int Length;
{
  return( (c_tree_node **)autallocheap( sizeof( c_tree_node *) * Length ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        C Make Function                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       C Tree Make Node                      |
|                                                             |
\------------------------------------------------------------*/

extern int c_lineno;
extern char *c_input_filename;

c_tree_node *c_tree_add_node( NodeCode )
 
  int NodeCode;
{
  c_tree_node *TreeNode;
  int          FirstCode;

  TreeNode  = c_tree_alloc_node( NodeCode );
  FirstCode = CFirstCode( NodeCode );

  CTreeLineNum( TreeNode ) = c_lineno;

  if ( c_input_filename )
  {
    CTreeFileName( TreeNode ) = c_input_filename;
  }
  else
  {
    if ( CTreeNameBuiltIn == (char *)0 )
    {
      CTreeNameBuiltIn = autnamealloc( "<built-in>" );
    }

    CTreeFileName( TreeNode ) = CTreeNameBuiltIn;
  }


  if ( FirstCode == C_TYPE_NODE )
  {
    CTreeTypeAlign( TreeNode ) = 1;
    CTreeTypeMainVariant( TreeNode ) = TreeNode;
  }
  else
  if ( ( FirstCode == C_INT_CST_NODE  ) ||
       ( FirstCode == C_REAL_CST_NODE ) ||
       ( FirstCode == C_STRING_NODE   ) ||
       ( FirstCode == C_COMPLEX_NODE  ) )
  {
    CSetTreeNodeConstant( TreeNode );
  }


  return( TreeNode );
}

/*------------------------------------------------------------\
|                                                             |
|                        C Copy Function                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       C Tree Copy Node                      |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_copy_node( TreeNode )

  c_tree_node *TreeNode;
{
  c_tree_node *DupNode;
  int          NodeCode;
  int          FirstCode;

  NodeCode  = CTreeNodeCode( TreeNode );
  FirstCode = CFirstCode( NodeCode );
  DupNode   = c_tree_alloc_node( NodeCode );
  memcpy( DupNode, TreeNode, C_NODE_SIZE[ FirstCode ] );

  CTreeChain( DupNode ) = (c_tree_node *)0;

  return( DupNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Copy List                        |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_copy_list( TreeNode )

  c_tree_node *TreeNode;
{
  c_tree_node *PrevNode;
  c_tree_node *NextNode;
  c_tree_node *HeadNode;

  if ( TreeNode == (c_tree_node *)0 )
  {
    return( (c_tree_node *)0 );
  }

  HeadNode = c_tree_copy_node( TreeNode );
  PrevNode = HeadNode;

  NextNode = TreeNode->COMMON.CHAIN;

  while ( NextNode != (c_tree_node *)0 )
  {
    PrevNode->COMMON.CHAIN = c_tree_copy_node( NextNode );
    PrevNode = PrevNode->COMMON.CHAIN;
    NextNode = NextNode->COMMON.CHAIN;
  }

  return( HeadNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       C Build Function                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Vector                     |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_vec( Length )

  int Length;
{
  c_tree_node *TreeNode;

  TreeNode = c_tree_add_node( C_VEC_NODE );
  TreeNode->VEC.LENGTH = Length;
  TreeNode->VEC.ARRAY  = c_tree_alloc_node_array( Length );

  return( TreeNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build List                       |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_list( Purpose, Value )

  c_tree_node *Purpose;
  c_tree_node *Value;
{
  c_tree_node *ListTree;

  ListTree = c_tree_add_node( C_LIST_NODE );
  ListTree->LIST.PURPOSE = Purpose;
  ListTree->LIST.VALUE   = Value;

  return( ListTree );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Complex                    |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_complex( Type, Real, Imag )

  c_tree_node *Type;
  c_tree_node *Real;
  c_tree_node *Imag;
{
  c_tree_node *ComplexTree;

  ComplexTree = c_tree_add_node( C_COMPLEX_NODE );
  ComplexTree->COMPLEX.REAL = Real;
  ComplexTree->COMPLEX.IMAG = Imag;

  if ( Type == (c_tree_node *)0 )
  {
    Type = c_tree_build_complex_type( CTreeType( Real ) );
  }

  return( ComplexTree );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Declaration                |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_decl( DeclCode, TreeName, TreeType )

  int          DeclCode;
  c_tree_node *TreeName;
  c_tree_node *TreeType;
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_add_node( DeclCode );
  TreeResult->DECL.NAME   = TreeName;
  TreeResult->COMMON.TYPE = TreeType;

  return( TreeResult ); 
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Instruction                |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_inst( InstCode )

  int InstCode;
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_add_node( InstCode );

  return( TreeResult ); 

}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Type                       |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_type( TypeCode )

  int TypeCode;
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_add_node( TypeCode );

  return( TreeResult ); 
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Type Copy                  |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_type_copy( TreeType )

  c_tree_node *TreeType;
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_copy_node( TreeType );

  CTreeTypeNextVariant( TreeResult ) = CTreeTypeNextVariant( TreeType );
  CTreeTypeNextVariant( CTreeTypeMainVariant( TreeType ) ) = TreeResult;

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Type Variant               |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_type_variant( TreeType, ConstP, VolatileP )

  c_tree_node *TreeType;
  int            ConstP;
  int            VolatileP;
{
  c_tree_node *TreeResult;

  ConstP    = !! ConstP;
  VolatileP = !! VolatileP;

  for ( TreeResult  = CTreeTypeMainVariant( TreeType );
        TreeResult != (c_tree_node *)0;
        TreeResult  = CTreeTypeNextVariant( TreeResult ) )
  {
    if ( ( ( !! CIsTreeNodeReadOnly( TreeResult ) ) == ConstP         ) &&
         ( ( !! CIsTreeNodeVolatile( TreeResult ) ) == VolatileP      ) &&
         ( CTreeTypeName( TreeResult ) == CTreeTypeName( TreeType ) ) )
    {
      return( TreeResult ); 
    }
  }

  TreeResult = c_tree_build_type_copy( TreeType );

  if ( ConstP    ) CSetTreeNodeReadOnly( TreeType );
  if ( VolatileP ) CSetTreeNodeVolatile( TreeType );

  return( TreeResult ); 
}

/*------------------------------------------------------------\
|                                                             |
|                 C Tree Build Type Attribute Variant         |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_type_attribute_variant( TreeType, TreeAttr )

  c_tree_node *TreeType;
  c_tree_node *TreeAttr;
{
  /* Rien a voir avec l'original car les attributs ne nous interessent pas */

  return( c_tree_build_type_variant( TreeType, 
                                     CIsTreeNodeReadOnly( TreeType ), 
                                     CIsTreeNodeVolatile( TreeType ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Function Type              |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_function_type( ValueType, ArgTypes )

  c_tree_node *ValueType;
  c_tree_node *ArgTypes;
{
  c_tree_node *TreeResult;

  if ( CTreeTypeCode( ValueType ) == C_FUNCTION_TYPE )
  {
    error("function return type cannot be function");
    ValueType = c_integer_type_node;
  }

  TreeResult = c_tree_build_type( C_FUNCTION_TYPE );
  CTreeType( TreeResult ) = ValueType;
  CTreeTypeValues( TreeResult ) = ArgTypes;

  /*
  if ( CTreeTypeSize( TreeResult ) == 0 )
  {
    c_layout_type( TreeResult );
  }
  */

  c_layout_type( TreeResult );

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Complex Type               |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_complex_type( Component )

  c_tree_node *Component;
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_build_type( C_COMPLEX_TYPE );
  TreeResult->COMMON.TYPE = CTreeTypeMainVariant( Component ); 

  if ( CIsTreeNodeVolatile( Component ) )
  {
    CSetTreeNodeVolatile( TreeResult );
  }

  if ( CIsTreeNodeReadOnly( Component ) )
  {
    CSetTreeNodeReadOnly( TreeResult );
  }

  c_layout_type( TreeResult );

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Signed Type                |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_signed_type( Precision )

  int Precision;
{
  c_tree_node *TreeResult;
  c_tree_node *TreeMin;
  c_tree_node *TreeMax;

  TreeResult = c_tree_build_type( C_INTEGER_TYPE );
  CTreeTypePrecision( TreeResult ) = Precision;

  TreeMin = c_tree_build_int_2((Precision - C_HOST_BITS_PER_WIDE_INT > 0
                    ? 0 : (C_HOST_WIDE_INT) (-1) << (Precision - 1)),
                   (((C_HOST_WIDE_INT) (-1)
                     << (Precision - C_HOST_BITS_PER_WIDE_INT - 1 > 0
                         ? Precision - C_HOST_BITS_PER_WIDE_INT - 1
                         : 0))));
  TreeMax = c_tree_build_int_2((Precision - C_HOST_BITS_PER_WIDE_INT > 0
                    ? -1 : ((C_HOST_WIDE_INT) 1 << (Precision - 1)) - 1),
                   (Precision - C_HOST_BITS_PER_WIDE_INT - 1 > 0
                    ? (((C_HOST_WIDE_INT) 1
                        << (Precision - C_HOST_BITS_PER_WIDE_INT - 1))) - 1
                    : 0));

  CTreeType( TreeMin ) = TreeResult;
  CTreeType( TreeMax ) = TreeResult;

  CTreeTypeMinValue( TreeResult ) = TreeMin;
  CTreeTypeMaxValue( TreeResult ) = TreeMax;

  if ( c_size_type == (c_tree_node *)0 )
  {
    c_size_type = TreeResult;
  }

  c_layout_type( TreeResult );

  return( TreeResult ); 
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Unsigned Type            |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_unsigned_type( Precision )

  int Precision;
{
  c_tree_node *TreeResult;
  c_tree_node *TreeMin;
  c_tree_node *TreeMax;

  TreeResult = c_tree_build_type( C_INTEGER_TYPE );
  CTreeTypePrecision( TreeResult ) = Precision;
  CSetTreeNodeUnsigned( TreeResult );

  TreeMin = c_tree_build_int_2( 0, 0 );
  TreeMax = c_tree_build_int_2( Precision - C_HOST_BITS_PER_WIDE_INT >= 0
		   ? -1 : ((C_HOST_WIDE_INT) 1 << Precision) - 1,
		   Precision - C_HOST_BITS_PER_WIDE_INT > 0
		   ? ((unsigned C_HOST_WIDE_INT) ~0
		      >> (C_HOST_BITS_PER_WIDE_INT
			  - (Precision - C_HOST_BITS_PER_WIDE_INT)))
		   : 0);

  CTreeType( TreeMin ) = TreeResult;
  CTreeType( TreeMax ) = TreeResult;

  CTreeTypeMinValue( TreeResult ) = TreeMin;
  CTreeTypeMaxValue( TreeResult ) = TreeMax;

  c_layout_type( TreeResult );

  return( TreeResult ); 
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Pointer Type             |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_pointer_type( TreeType )

  c_tree_node *TreeType;
{
  c_tree_node *TreeResult;

  TreeResult = TreeType->TYPE.POINTER_TO;

  if ( TreeResult == (c_tree_node *)0 )
  {
    TreeResult = c_tree_build_type( C_POINTER_TYPE );
    TreeResult->COMMON.TYPE   = TreeType;
    TreeType->TYPE.POINTER_TO = TreeResult;

    c_layout_type( TreeResult );
  }

  return( TreeResult ); 
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Array Type               |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_array_type( ElemType, IndexType )

  c_tree_node *ElemType;
  c_tree_node *IndexType;
{
  c_tree_node *TreeResult;

  if ( CTreeTypeCode( ElemType ) == C_FUNCTION_TYPE )
  {
    error("arrays of functions are not meaningful");
    ElemType = c_integer_type_node;
  }

  c_tree_build_pointer_type( ElemType );

  TreeResult = c_tree_build_type( C_ARRAY_TYPE );

  CTreeType( TreeResult ) = ElemType;
  CTreeTypeValues( TreeResult ) = IndexType;

  if ( IndexType == (c_tree_node *)0 )
  {
    return( TreeResult );
  }

  if ( CTreeTypeSize( TreeResult ) == (c_tree_node *)0 )
  {
    c_layout_type( TreeResult );
  }

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Index Type                 |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_index_type( MaxVal )

  c_tree_node *MaxVal;
{
  c_tree_node *IndexType;

  IndexType = c_tree_build_type( C_INTEGER_TYPE );
  CTreeTypePrecision( IndexType ) = CTreeTypePrecision( c_size_type );

  CTreeTypeMinValue( IndexType )  = c_size_zero_node;
  CTreeTypeMaxValue( IndexType )  = c_convert_expr( c_size_type, MaxVal );

  CTreeTypeMode( IndexType )  = CTreeTypeMode( c_size_type );
  CTreeTypeSize( IndexType )  = CTreeTypeSize( c_size_type );
  CTreeTypeAlign( IndexType ) = CTreeTypeAlign( c_size_type );


  if ( CTreeNodeFirstCode( MaxVal ) == C_INST_NODE )
  {
    /* If the domain should be empty, make sure the maxval remains -1 and is not
     * spoiled by truncation.  */

    if ( INT_CST_LT( MaxVal, c_integer_zero_node))
    {
      CTreeTypeMaxValue( IndexType ) = c_tree_build_int_2(-1, -1);
      CTreeType( CTreeTypeMaxValue( IndexType ) ) = c_size_type;
    }
  }

  return( IndexType );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Unary Expr                 |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_unary_expr( ExprCode, TreeExpr )

  int            ExprCode;
  c_tree_node *TreeExpr; 
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_add_node( ExprCode );
  TreeResult->EXPR.OPERANDS  = c_tree_alloc_node_array( 1 );
  TreeResult->EXPR.NUM_OPER  = 1;

  TreeResult->EXPR.OPERANDS[ 0 ] = TreeExpr;

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Binary Expr                |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_binary_expr( ExprCode, TreeExpr1, TreeExpr2 )

  int          ExprCode;
  c_tree_node *TreeExpr1; 
  c_tree_node *TreeExpr2; 
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_add_node( ExprCode );
  TreeResult->EXPR.OPERANDS  = c_tree_alloc_node_array( 2 );
  TreeResult->EXPR.NUM_OPER  = 2;

  TreeResult->EXPR.OPERANDS[ 0 ] = TreeExpr1;
  TreeResult->EXPR.OPERANDS[ 1 ] = TreeExpr2;

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Ternary Expr             |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_ternary_expr( ExprCode, TreeExpr1, TreeExpr2, TreeExpr3 )

  int            ExprCode;
  c_tree_node *TreeExpr1; 
  c_tree_node *TreeExpr2; 
  c_tree_node *TreeExpr3; 
{
  c_tree_node *TreeResult;

  TreeResult = c_tree_add_node( ExprCode );
  TreeResult->EXPR.OPERANDS  = c_tree_alloc_node_array( 3 );
  TreeResult->EXPR.NUM_OPER  = 3;

  TreeResult->EXPR.OPERANDS[ 0 ] = TreeExpr1;
  TreeResult->EXPR.OPERANDS[ 1 ] = TreeExpr2;
  TreeResult->EXPR.OPERANDS[ 2 ] = TreeExpr3;

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Real                     |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_real( TreeType, Value )

  c_tree_node *TreeType;
  double         Value;
{
  c_tree_node *RealCstTree;

  RealCstTree = c_tree_add_node( C_REAL_CST_NODE );
  RealCstTree->REAL_CST.VALUE = Value;
  RealCstTree->COMMON.TYPE    = TreeType;

  return( RealCstTree );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build Integer                  |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_int_2( Low, High )

  long Low;
  long High;
{
  c_tree_node *IntCstTree;

  IntCstTree = c_tree_add_node( C_INT_CST_NODE );
  IntCstTree->INT_CST.LOW  = Low;
  IntCstTree->INT_CST.HIGH = High;
  IntCstTree->COMMON.TYPE  = c_integer_type_node;

  return( IntCstTree );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Build String                     |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_string( Length, Text )

  int   Length;
  char *Text;
{
  c_tree_node *TreeString;

  Text = autnamealloc( Text );

  TreeString = c_tree_add_node( C_STRING_NODE );
  TreeString->STRING.LENGTH  = Length;
  TreeString->STRING.POINTER = Text;

  return( TreeString );
}

/*------------------------------------------------------------\
|                                                             |
|                 C Tree Build Pointer Declarator             |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_build_pointer_declarator( TreeTypeQual, TreeTarget )

  c_tree_node *TreeTypeQual;
  c_tree_node *TreeTarget;
{
  c_tree_node *TreeResult;

  TreeResult =  c_tree_build_unary_expr( C_INDIRECT_REF, TreeTarget );
  CTreeType( TreeResult ) = TreeTypeQual;

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                       C Combine Function                    |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_combine_strings( TreeString )

  c_tree_node *TreeString;
{
  c_tree_node *TreeResult;
  c_tree_node *ScanString;
  c_tree_node *TreeType;
  char          *Name;
  char          *Buffer;
  char          *Scan;
  int            Length;
  int            TotalLength;

  if ( CTreeChain( TreeString ) != (c_tree_node *)0 )
  {
    TotalLength = 1;

    for ( ScanString  = TreeString;
          ScanString != (c_tree_node *)0;
          ScanString  = CTreeChain( ScanString ) )
    {
      c_fprintf( stdout, "%s %d\n", 
          CTreeStringPointer( ScanString ),
          CTreeStringLength( ScanString ) );
      TotalLength += (CTreeStringLength( ScanString ) - 1);
    }

      /* If anything is wide, the non-wides will be converted,
         which makes them take more space.  */

    Buffer = autallocblock( TotalLength );
    Buffer[ 0 ] = '"';
    Scan        = Buffer + 1;

    for ( ScanString  = TreeString;
          ScanString != (c_tree_node *)0;
          ScanString  = CTreeChain( ScanString ) )
    {
      Name   = CTreeStringPointer( ScanString ) + 1;
      Length = CTreeStringLength( ScanString ) - 1;

      strncpy( Scan, Name, Length );
      Scan += (Length - 1);
    }

    TreeResult = c_tree_build_string( TotalLength, Buffer );
    autfreeblock( Buffer );
  }
  else
  {
    TreeResult  = TreeString;
    TotalLength = CTreeStringLength( TreeString );
  }

  /* Create the array type for the string constant.
     -Wwrite-strings says make the string constant an array of const char
     so that copying it to a non-const pointer will get a warning.  */

  if ( (  c_warn_write_strings    ) && 
       (! c_flag_traditional      ) && 
       (! c_flag_writable_strings ) )
  {
      TreeType = c_tree_build_type_variant( c_char_type_node, 1, 0);

      CTreeType( TreeResult ) = 
         c_tree_build_array_type( TreeType, 
              c_tree_build_index_type( c_tree_build_int_2( TotalLength - 1, 0)));
  }
  else
  {
    CTreeType( TreeResult ) = 
      c_tree_build_array_type( c_char_type_node,
              c_tree_build_index_type( c_tree_build_int_2( TotalLength - 1, 0)));
  }

  CSetTreeNodeConstant( TreeResult );
  CSetTreeNodeStatic( TreeResult );

  return( TreeResult );
}

/*------------------------------------------------------------\
|                                                             |
|                       C Get Function                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     C Tree Get Size Int                   |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_get_size_int( Number )

  int Number;
{
  c_tree_node *TreeNode;

  TreeNode = c_tree_build_int_2( Number, 0 );
  CTreeType( TreeNode ) = c_size_type;

  return( TreeNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Get List Length                |
|                                                             |
\------------------------------------------------------------*/

int c_tree_get_list_length( ListTree )

   c_tree_node *ListTree;
{
  int Length;

  Length = 0;

  while ( ListTree != (c_tree_node *)0 )
  {
    ListTree = CTreeChain( ListTree );
    Length++;
  }

  return( Length );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Get Identifier                   |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_get_identifier( Text )

   char *Text;
{
  c_tree_node *TreeNode;
  authelem      *Element;

  Text = autnamealloc( Text );

  if ( CTreeHashIdentifier == (authtable *)0 )
  {
    CTreeHashIdentifier = createauthtable( 1000 );
  }

  Element = searchauthelem( CTreeHashIdentifier, Text );

  if ( Element == (authelem *)0 )
  {
    TreeNode = c_tree_add_node( C_IDENTIFIER_NODE );
    TreeNode->IDENT.LENGTH  = strlen( Text );
    TreeNode->IDENT.POINTER = Text;

    addauthelem( CTreeHashIdentifier, Text, (long)TreeNode );
  }
  else
  {
    TreeNode = (c_tree_node *)Element->VALUE;
  }

  return( TreeNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Concat List                    |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_concat_list( ListTree1, ListTree2 )

  c_tree_node *ListTree1;
  c_tree_node *ListTree2;
{
  c_tree_node *ScanTree;

  if ( ListTree1 == (c_tree_node *)0 )
  {
    return( ListTree2 );
  }

  for ( ScanTree  = ListTree1;
        ScanTree != (c_tree_node *)0;
        ScanTree  = ScanTree->COMMON.CHAIN )
  {
    if ( ScanTree->COMMON.CHAIN == (c_tree_node *)0 )
    {
      ScanTree->COMMON.CHAIN = ListTree2; break;
    }
  }

  return( ListTree1 );
}

/* Return first list element whose TREE_PURPOSE is ELEM.
   Return 0 if ELEM is not in LIST.  */

c_tree_node *c_tree_get_purpose_member(elem, list)
     c_tree_node *elem;
     c_tree_node *list;
{
  while (list)
    {
      if (elem == CTreeListPurpose(list))
	return list;
      list = CTreeChain(list);
    }
  return (c_tree_node *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                     C Tree Add Head List                  |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_add_head_list( Purpose, Value, ListTree )

  c_tree_node *Purpose;
  c_tree_node *Value;
  c_tree_node *ListTree;
{
  c_tree_node *HeadTree;

  HeadTree = c_tree_add_node( C_LIST_NODE );
  HeadTree->LIST.PURPOSE = Purpose;
  HeadTree->LIST.VALUE   = Value;
  HeadTree->COMMON.CHAIN = ListTree;

  return( HeadTree );
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Int Cst Signed               |
|                                                             |
\------------------------------------------------------------*/

/* Return an indication of the sign of the integer constant T.
   The return value is -1 if T < 0, 0 if T == 0, and 1 if T > 0.
   Note that -1 will never be returned it T's type is unsigned.  */

int c_tree_is_int_cst_sgn (t)

     c_tree_node *t;
{
  if ( ( CTreeIntCstLow(t)  == 0 ) && 
       ( CTreeIntCstHigh(t) == 0 ) )
  {
    return 0;
  }
  else if ( CIsTreeNodeUnsigned( CTreeType(t)))
  {
    return 1;
  }
  else if ( CTreeIntCstHigh(t) < 0)
  {
    return -1;
  }
  else
  {
    return 1;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Real Zero                    |
|                                                             |
\------------------------------------------------------------*/

/* Return 1 if EXPR is the integer constant zero or a complex constant
   of zero.  */

int c_tree_is_real_zerop(expr)

     c_tree_node *expr;
{
  CStripNops(expr);

  if ( CTreeNodeFirstCode( expr ) == C_REAL_CST_NODE )
  {
    return( CTreeRealCstValue( expr ) == 0.0 );
  }

  if ( CTreeNodeFirstCode( expr ) == C_COMPLEX_NODE )
  {
    return( c_tree_is_real_zerop( CTreeComplexReal( expr ) ) &&
            c_tree_is_real_zerop( CTreeComplexImag( expr ) ) );

  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Real One                       |
|                                                             |
\------------------------------------------------------------*/

/* Return 1 if EXPR is the integer constant one or the corresponding
   complex constant.  */

int c_tree_is_real_onep(expr)

     c_tree_node *expr;
{
  CStripNops(expr);

  if ( CTreeNodeFirstCode( expr ) == C_REAL_CST_NODE )
  {
    return( CTreeRealCstValue( expr ) == 1.0 );
  }

  if ( CTreeNodeFirstCode( expr ) == C_COMPLEX_NODE )
  {
    return( c_tree_is_real_onep( CTreeComplexReal( expr ) ) &&
            c_tree_is_real_zerop( CTreeComplexImag( expr ) ) );

  }

  return( 0 );
}

/* Return 1 if EXPR is the integer constant two or the corresponding
   complex constant.  */

int c_tree_is_real_twop(expr)

     c_tree_node *expr;
{
  CStripNops(expr);

  if ( CTreeNodeFirstCode( expr ) == C_REAL_CST_NODE )
  {
    return( CTreeRealCstValue( expr ) == 2.0 );
  }

  if ( CTreeNodeFirstCode( expr ) == C_COMPLEX_NODE )
  {
    return( c_tree_is_real_twop( CTreeComplexReal( expr ) ) &&
            c_tree_is_real_twop( CTreeComplexImag( expr ) ) );

  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Int Zero                       |
|                                                             |
\------------------------------------------------------------*/

/* Return 1 if EXPR is the integer constant zero or a complex constant
   of zero.  */

int c_tree_is_integer_zerop(expr)

     c_tree_node *expr;
{
  CStripNops(expr);

  if ( CTreeNodeFirstCode( expr ) == C_INT_CST_NODE )
  {
    return( ( CTreeIntCstLow( expr ) == 0 ) &&
            ( CTreeIntCstHigh( expr ) == 0 ) );
  }

  if ( CTreeNodeFirstCode( expr ) == C_COMPLEX_NODE )
  {
    return( c_tree_is_integer_zerop( CTreeComplexReal( expr ) ) &&
            c_tree_is_integer_zerop( CTreeComplexImag( expr ) ) );

  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Int One                        |
|                                                             |
\------------------------------------------------------------*/

/* Return 1 if EXPR is the integer constant one or the corresponding
   complex constant.  */

int c_tree_is_integer_onep(expr)

     c_tree_node *expr;
{
  CStripNops(expr);

  if ( CTreeNodeFirstCode( expr ) == C_INT_CST_NODE )
  {
    return( ( CTreeIntCstLow( expr ) == 1 ) &&
            ( CTreeIntCstHigh( expr ) == 0 ) );
  }

  if ( CTreeNodeFirstCode( expr ) == C_COMPLEX_NODE )
  {
    return( c_tree_is_integer_onep( CTreeComplexReal( expr ) ) &&
            c_tree_is_integer_zerop( CTreeComplexImag( expr ) ) );

  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Int Less Than                |
|                                                             |
\------------------------------------------------------------*/

int c_tree_is_int_cst_lt(t1, t2)

     c_tree_node *t1;
     c_tree_node *t2;
{
  if (t1 == t2)
  {
    return 0;
  }

  if (! CIsTreeNodeUnsigned( CTreeType(t1)))
  {
    return INT_CST_LT(t1, t2);
  }
  return INT_CST_LT_UNSIGNED(t1, t2);
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Is Int Cst Equal                  |
|                                                             |
\------------------------------------------------------------*/

/* Nonzero if integer constants T1 and T2
   represent the same constant value.  */

int c_tree_is_int_cst_equal(t1, t2)
     c_tree_node *t1;
     c_tree_node *t2;
{
  if (t1 == t2)
    return 1;
  if (t1 == 0 || t2 == 0)
    return 0;
  if ( CTreeNodeFirstCode(t1) == C_INT_CST_NODE
      &&  CTreeNodeFirstCode(t2) == C_INT_CST_NODE
      && CTreeIntCstLow(t1) == CTreeIntCstLow(t2)
      && CTreeIntCstHigh(t1) == CTreeIntCstHigh(t2))
    return 1;
  return 0;
}


/* Return 1 if EXPR is an integer constant that is a power of 2 (i.e., has only
   one bit on).  */

int c_tree_is_integer_pow2p(expr)
     c_tree_node *expr;
{
  int prec;
  C_HOST_WIDE_INT high, low;

  CStripNops(expr);

  if (CTreeNodeCode(expr) == C_COMPLEX_NODE
      && c_tree_is_integer_pow2p( CTreeComplexReal(expr))
      && c_tree_is_integer_zerop( CTreeComplexImag(expr)))
    return 1;

  if (CTreeNodeCode(expr) != C_INT_CST_NODE || CIsTreeNodeCstOverflow(expr))
    return 0;

  prec = (POINTER_TYPE_P (CTreeType(expr))
          ? C_POINTER_SIZE : CTreeTypePrecision(CTreeType(expr)));
  high = CTreeIntCstHigh(expr);
  low = CTreeIntCstLow(expr);

  /* First clear all bits that are beyond the type's precision in case
     we've been sign extended.  */

  if (prec == 2 * C_HOST_BITS_PER_WIDE_INT)
    ;
  else if (prec > C_HOST_BITS_PER_WIDE_INT)
    high &= ~((C_HOST_WIDE_INT) (-1) << (prec - C_HOST_BITS_PER_WIDE_INT));
  else
    {
      high = 0;
      if (prec < C_HOST_BITS_PER_WIDE_INT)
        low &= ~((C_HOST_WIDE_INT) (-1) << prec);
    }

  if (high == 0 && low == 0)
    return 0;

  return ((high == 0 && (low & (low - 1)) == 0)
          || (low == 0 && (high & (high - 1)) == 0));
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Reverse List                      |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_reverse_list( t )

  c_tree_node *t;
{
  c_tree_node *prev = 0;
  c_tree_node *decl;
  c_tree_node *next;

  for (decl = t; decl; decl = next)
    {
      next = CTreeChain(decl);
      CTreeChain(decl) = prev;
      prev = decl;
    }

  return prev;
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree Get Last List                     |
|                                                             |
\------------------------------------------------------------*/

/* Return the last node in a chain of nodes (chained through TREE_CHAIN).  */

c_tree_node *c_tree_get_last_list(chain)

     c_tree_node *chain;
{
  c_tree_node *next;
  if (chain)
  {
    while ((next = CTreeChain(chain)) != NULL )
    {
      chain = next;
    }
  }
  return chain;
}

/* Same as above, but only builds for unary operators.
   Saves lions share of calls to `build'; cuts down use
   of varargs, which is expensive for RISC machines.  */

c_tree_node *c_tree_build_unary_typed_expr(code, type, node)
     int code;
     c_tree_node *type;
     c_tree_node *node;
{
  c_tree_node *t;

  t = c_tree_build_unary_expr( code, node );

  CTreeType(t) = type;
  if (node)
    {
      if ( CIsTreeNodeSideEffect(node)) CSetTreeNodeSideEffect(t);
      if ( CIsTreeNodeRaises(node)) CSetTreeNodeRaises(t);
    }

  return t;
}


c_tree_node *c_tree_build_binary_typed_expr(code, type, arg0, arg1 )
     int code;
     c_tree_node *type;
     c_tree_node *arg0;
     c_tree_node *arg1;
{
  c_tree_node *t;

  t = c_tree_build_binary_expr( code, arg0, arg1 );

  CTreeType(t) = type;
  if (arg0)
    {
      if ( CIsTreeNodeSideEffect(arg0)) CSetTreeNodeSideEffect(t);
      if ( CIsTreeNodeRaises(arg0)) CSetTreeNodeRaises(t);
    }

  if (arg1)
    {
      if ( CIsTreeNodeSideEffect(arg1)) CSetTreeNodeSideEffect(t);
      if ( CIsTreeNodeRaises(arg1)) CSetTreeNodeRaises(t);
    }

  return t;
}

c_tree_node *c_tree_build_ternary_typed_expr(code, type, arg0, arg1, arg2 )
     int code;
     c_tree_node *type;
     c_tree_node *arg0;
     c_tree_node *arg1;
     c_tree_node *arg2;
{
  c_tree_node *t;

  t = c_tree_build_ternary_expr( code, arg0, arg1, arg2 );

  CTreeType(t) = type;
  if (arg0)
    {
      if ( CIsTreeNodeSideEffect(arg0)) CSetTreeNodeSideEffect(t);
      if ( CIsTreeNodeRaises(arg0)) CSetTreeNodeRaises(t);
    }

  if (arg1)
    {
      if ( CIsTreeNodeSideEffect(arg1)) CSetTreeNodeSideEffect(t);
      if ( CIsTreeNodeRaises(arg1)) CSetTreeNodeRaises(t);
    }

  if (arg2)
    {
      if ( CIsTreeNodeSideEffect(arg2)) CSetTreeNodeSideEffect(t);
      if ( CIsTreeNodeRaises(arg2)) CSetTreeNodeRaises(t);
    }

  return t;
}

/*------------------------------------------------------------\
|                                                             |
|                    C Tree View Node                       |
|                                                             |
\------------------------------------------------------------*/


static long CViewIdent = 0;

void c_indent()
{
  long Index;

  for ( Index = 1; Index < CViewIdent; Index++ )
  {
    fprintf( stdout, "| " );
  }
}

# define c_printf c_indent(); fprintf

void c_tree_view_node( TreeNode )

  c_tree_node *TreeNode;
{
  c_tree_node *TreeScan;
  c_tree_node *TreeHead;
  c_tree_node *TreeName;
  char        *Name;
  int          FirstCode;
  int          ExprCode;
  int          TypeCode;
  int          DeclCode;
  int          InstCode;
  int          CaseCode;
  int          Index;
  int          ViewList;

  if ( TreeNode == (c_tree_node *)0 )
  {
    fprintf( stdout, " <NULL>\n" );
    return;
  }

  if ( TreeNode == c_error_mark_node )
  {
    fprintf( stdout, " #ERROR#\n" );
    return;
  }

  if ( CIsTreeNodeMark( TreeNode ) )
  {
    FirstCode = CTreeNodeFirstCode( TreeNode );
    fprintf( stdout, " %s", C_NODE_CODE_NAME[ FirstCode ] );

    switch ( FirstCode )
    {
      case C_INT_CST_NODE    :
        fprintf( stdout, ",%ld,%ld",
                 CTreeIntCstLow(TreeNode), CTreeIntCstHigh(TreeNode));
      break;

      case C_REAL_CST_NODE   :
        fprintf( stdout, ",%g", CTreeRealCstValue( TreeNode ) );
      break;

      case C_STRING_NODE     :
        fprintf( stdout, ",/*%s*/", CTreeStringPointer( TreeNode ) );
      break;

      case C_COMPLEX_NODE    :
      break;

      case C_IDENTIFIER_NODE :
        fprintf( stdout, ",/*%s*/", CTreeIdentPointer( TreeNode ) );
      break;

      case C_LIST_NODE       :
      break;

      case C_VEC_NODE        :
      break;

      case C_EXPR_NODE       :
        ExprCode = CExtraCode( CTreeExprCode( TreeNode ) );
        fprintf( stdout, ",%s", C_EXPR_CODE_NAME[ ExprCode ] );
      break;

      case C_BLOCK_NODE      :
      break;

      case C_TYPE_NODE       :
        TypeCode = CExtraCode( CTreeTypeCode( TreeNode ) );
        TreeName = CTreeTypeName( TreeNode );
        fprintf( stdout, ",%s", C_TYPE_CODE_NAME[ TypeCode ] );

        if ( ( TreeName                    != (c_tree_node *)0 ) &&
             ( CTreeNodeFirstCode( TreeName ) == C_DECL_NODE      ) )
        {
          TreeName = CTreeDeclName( TreeName );
          if ( TreeName != (c_tree_node *)0 )
          {
            fprintf( stdout, ",%s", CTreeIdentPointer( TreeName ) );
          }
        }

      break;
      case C_DECL_NODE       :
        DeclCode = CExtraCode( CTreeDeclCode( TreeNode ) );
        TreeName = CTreeDeclName( TreeNode );
        fprintf( stdout, ",%s", C_DECL_CODE_NAME[ DeclCode ] );

        if ( ( TreeName                    != (c_tree_node *)0  ) &&
             ( CTreeNodeFirstCode( TreeName ) == C_IDENTIFIER_NODE ) )
        {
          fprintf( stdout, ",/*%s*/", CTreeIdentPointer( TreeName ) );
        }

      break;
      case C_INST_NODE       :
        InstCode = CExtraCode( CTreeInstCode( TreeNode ) );
        fprintf( stdout, ",%s", C_INST_CODE_NAME[ InstCode ] );
      break;
    }

    fprintf( stdout, ",[0x%08lx]\n", (long)TreeNode );

    return;
  }

  CViewIdent++;
  fprintf( stdout, "\n" );

  TreeHead = TreeNode;
  ViewList = 0;

  while ( TreeNode != (c_tree_node *)0 )
  {
    CSetTreeNodeMark( TreeNode );

    c_printf( stdout, "{-- COMMON ------# [0x%08lx]\n", (long)TreeNode );

    FirstCode = CTreeNodeFirstCode( TreeNode );
    c_printf( stdout, "NODE_CODE : %s\n" , C_NODE_CODE_NAME[ FirstCode ] ); 

    if ( CTreeChain( TreeNode ) != (c_tree_node *)0 )
    {
       c_printf( stdout, "CHAIN     : [0x%08lx]\n", (long)CTreeChain( TreeNode ) );
    }
    else
    {
       c_printf( stdout, "CHAIN     : <NULL>\n" );
    }

    c_printf( stdout, "TYPE      :" );

    if ( ! C_TREE_VIEW_TYPE_NODE )
    {
      if ( CTreeType( TreeNode ) != (c_tree_node *)0 )
      {
        CSetTreeNodeMark( CTreeType( TreeNode ) );
      }
    }

    c_tree_view_node( CTreeType( TreeNode ) );

    c_printf( stdout, "FLAGS     : 0x%08lx\n", CTreeFlags( TreeNode ) );
    c_printf( stdout, "USER      : 0x%08lx\n", (long)CTreeUser( TreeNode ) );
    Name = CTreeFileName( TreeNode );
    c_printf( stdout, "FILE_NAME : %s\n", ( Name != (char *)0 ) ? Name : "null" );
    c_printf( stdout, "LINE_NUM  : %d\n", CTreeLineNum( TreeNode ) );

    c_printf( stdout, "#-- %-10s --#\n", C_NODE_CODE_NAME[ FirstCode ] );

    switch ( FirstCode )
    {
      case C_INT_CST_NODE    :
        c_printf( stdout, "LOW       : %ld\n", CTreeIntCstLow( TreeNode ) );
        c_printf( stdout, "HIGH      : %ld\n", CTreeIntCstHigh( TreeNode ) );
      break;
      case C_REAL_CST_NODE   :
        c_printf( stdout, "VALUE     : %g\n", CTreeRealCstValue( TreeNode ) );
      break;
      case C_STRING_NODE     :
        c_printf( stdout, "LENGTH    : %d\n", CTreeStringLength( TreeNode ) );
        c_printf( stdout, "POINTER   : /*%s*/\n", CTreeStringPointer( TreeNode ) );
      break;
      case C_COMPLEX_NODE    :
        c_printf( stdout, "REAL      : %ld\n", (long)CTreeComplexReal( TreeNode ) );
        c_printf( stdout, "IMAG      : %ld\n", (long)CTreeComplexImag( TreeNode ) );
      break;
      case C_IDENTIFIER_NODE :
        c_printf( stdout, "LENGTH    : %d\n", CTreeIdentLength( TreeNode ) );
        c_printf( stdout, "POINTER   : /*%s*/\n", CTreeIdentPointer( TreeNode ) );
        c_printf( stdout, "GLOBAL    :" );
        c_tree_view_node( CTreeIdentGlobalValue( TreeNode ) );
        c_printf( stdout, "LOCAL     :" );
        c_tree_view_node( CTreeIdentLocalValue( TreeNode ) );
        c_printf( stdout, "LABEL     :" );
        c_tree_view_node( CTreeIdentLabelValue( TreeNode ) );
        c_printf( stdout, "IMPLICIT  :" );
        c_tree_view_node( CTreeIdentImplicitDecl( TreeNode ) );
        c_printf( stdout, "ERROR     :" );
        c_tree_view_node( CTreeIdentErrorLocus( TreeNode ) );
        c_printf( stdout, "LIMBO     :" );
        c_tree_view_node( CTreeIdentLimboValue( TreeNode ) );
      break;
      case C_LIST_NODE       :
        c_printf( stdout, "PURPOSE   :" );
        c_tree_view_node( CTreeListPurpose( TreeNode ) );
        c_printf( stdout, "VALUE     :" );
        c_tree_view_node( CTreeListValue( TreeNode ) );

        ViewList = 1;
      break;
      case C_VEC_NODE        :
        c_printf( stdout, "LENGTH  : %d\n", CTreeVecLength( TreeNode ) );

        for ( Index = 0; Index < CTreeVecLength( TreeNode ); Index++ )
        {
          TreeScan = CTreeVecElem( TreeNode, Index );
          c_printf( stdout, "ARRAY     : %d", Index );
          c_tree_view_node( TreeScan );
        }
      break;
      case C_EXPR_NODE       :
        ExprCode = CExtraCode( CTreeExprCode( TreeNode ) );
        c_printf( stdout, "EXPR_CODE : %s\n", C_EXPR_CODE_NAME[ ExprCode ] );
        c_printf( stdout, "COMPLEXITY: %d\n", CTreeExprComplexity( TreeNode ) );
        c_printf( stdout, "NUM_OPER  : %d\n", CTreeExprNumOper( TreeNode ) );

        for ( Index = 0; Index < CTreeExprNumOper( TreeNode ); Index++ )
        {
          TreeScan = CTreeExprOperand( TreeNode, Index );
          c_printf( stdout, "OPERANDS  : %d", Index );
          c_tree_view_node( TreeScan );
        }
      break;
      case C_BLOCK_NODE      :
        c_printf( stdout, "VARS      :" );
        c_tree_view_node_list( CTreeBlockVars( TreeNode ) );
        c_printf( stdout, "TYPE TAGS :" );
        c_tree_view_node( CTreeBlockTypeTags( TreeNode ) );
        c_printf( stdout, "BODY      :" );
        c_tree_view_node( CTreeBlockBody( TreeNode ) );
        c_printf( stdout, "SUB BLOCK :" );
        c_tree_view_node( CTreeBlockSubBlocks( TreeNode ) );
        c_printf( stdout, "SUP CONT  :" );
        c_tree_view_node( CTreeBlockSuperContext( TreeNode ) );
        c_printf( stdout, "AB ORIGIN :" );
        c_tree_view_node( CTreeBlockAbstractOrigin( TreeNode ) );
      break;
      case C_TYPE_NODE       :
        TypeCode = CExtraCode( CTreeTypeCode( TreeNode ) );
        c_printf( stdout, "TYPE_CODE : %s\n", C_TYPE_CODE_NAME[ TypeCode ] );
        c_printf( stdout, "VALUES    :" );
        c_tree_view_node( CTreeTypeValues( TreeNode ) );

        if ( C_TREE_VIEW_SIZE_NODE )
        {
          c_printf( stdout, "SIZE      :" );
          c_tree_view_node( CTreeTypeSize( TreeNode ) );
        }

        c_printf( stdout, "ATTRIBUTES:" );
        c_tree_view_node( CTreeTypeAttributes( TreeNode ) );
        c_printf( stdout, "TYPE_ID   : %d\n", CTreeTypeId( TreeNode ) );
        c_printf( stdout, "MODE      : %d\n", CTreeTypeMode( TreeNode ) );
        c_printf( stdout, "PRECISION : %d\n", CTreeTypePrecision( TreeNode ) );
        c_printf( stdout, "ALIGN     : %d\n", CTreeTypeAlign( TreeNode ) );
        c_printf( stdout, "POINTER_TO:" );
        c_tree_view_node( CTreeTypePointerTo( TreeNode ) );
        c_printf( stdout, "REF_TO    :" );
        c_tree_view_node( CTreeTypeReferenceTo( TreeNode ) );
        c_printf( stdout, "NAME      :" );
        c_tree_view_node( CTreeTypeName( TreeNode ) );
        c_printf( stdout, "MIN_VALUE :" );
        c_tree_view_node( CTreeTypeMinValue( TreeNode ) );
        c_printf( stdout, "MAX_VALUE :" );
        c_tree_view_node( CTreeTypeMaxValue( TreeNode ) );
        c_printf( stdout, "NEXT_VARIA:" );
        c_tree_view_node( CTreeTypeNextVariant( TreeNode ) );
        c_printf( stdout, "MAIN_VARIA:" );
        c_tree_view_node( CTreeTypeMainVariant( TreeNode ) );
        c_printf( stdout, "BINFO     :" );
        c_tree_view_node( CTreeTypeBInfo( TreeNode ) );
        c_printf( stdout, "NONCOPIED :" );
        c_tree_view_node( CTreeTypeNonCopiedParts( TreeNode ) );
        c_printf( stdout, "CONTEXT   :" );
        c_tree_view_node( CTreeTypeContext( TreeNode ) );
      break;
      case C_DECL_NODE       :
        DeclCode = CExtraCode( CTreeDeclCode( TreeNode ) );
        c_printf( stdout, "DECL_CODE : %s\n", C_DECL_CODE_NAME[ DeclCode ] );
        Name = CTreeDeclFileName( TreeNode );
        c_printf( stdout, "DECL_ID   : %d\n", CTreeDeclId( TreeNode ) );

        if ( C_TREE_VIEW_SIZE_NODE )
        {
          c_printf( stdout, "SIZE      :" );
          c_tree_view_node( CTreeDeclSize( TreeNode ) );
        }

        c_printf( stdout, "MODE      : %d\n", CTreeDeclMode( TreeNode ) );
        c_printf( stdout, "FIELD SIZE: %d\n", CTreeDeclFieldSize( TreeNode ) );
        c_printf( stdout, "NAME      :" );
        c_tree_view_node( CTreeDeclName( TreeNode ) );
        c_printf( stdout, "CONTEXT   :" );
        c_tree_view_node( CTreeDeclContext( TreeNode ) );
        c_printf( stdout, "AB ORIGIN :" );
        c_tree_view_node( CTreeDeclAbstractOrigin( TreeNode ) );
        c_printf( stdout, "ARGUMENTS :" );
        c_tree_view_node_list( CTreeDeclArguments( TreeNode ) );
        c_printf( stdout, "RESULT    :" );
        c_tree_view_node( CTreeDeclResult( TreeNode ) );
        c_printf( stdout, "INITIAL   :" );
        c_tree_view_node( CTreeDeclInitial( TreeNode ) );
        c_printf( stdout, "FLAGS     : 0x%08lx\n", CTreeDeclFlags( TreeNode ) );

        ViewList = CTreeDeclCode( TreeNode ) == C_FIELD_DECL;
      break;
      case C_INST_NODE       :
        InstCode = CExtraCode( CTreeInstCode( TreeNode ) );
        c_printf( stdout, "INST_CODE : %s\n", C_INST_CODE_NAME[ InstCode ] );

        InstCode = CTreeNodeCode( TreeNode );

        switch ( InstCode )
        {
          case C_FOR_INST :
            c_printf( stdout, "START     :" );
            c_tree_view_node( CTreeInstForStart( TreeNode ) );
            c_printf( stdout, "COND      :" );
            c_tree_view_node( CTreeInstForCond( TreeNode ) );
            c_printf( stdout, "INC       :" );
            c_tree_view_node( CTreeInstForInc( TreeNode ) );
            c_printf( stdout, "BODY      :" );
            c_tree_view_node( CTreeInstForBody( TreeNode ) );
          break;
          case C_WHILE_INST :
            c_printf( stdout, "COND      :" );
            c_tree_view_node( CTreeInstWhileCond( TreeNode ) );
            c_printf( stdout, "BODY      :" );
            c_tree_view_node( CTreeInstWhileBody( TreeNode ) );
          break;
          case C_DO_INST :
            c_printf( stdout, "BODY      :" );
            c_tree_view_node( CTreeInstDoBody( TreeNode ) );
            c_printf( stdout, "COND      :" );
            c_tree_view_node( CTreeInstDoCond( TreeNode ) );
          break;
          case C_IF_INST :
            c_printf( stdout, "COND      :" );
            c_tree_view_node( CTreeInstIfCond( TreeNode ) );
            c_printf( stdout, "THEN      :" );
            c_tree_view_node( CTreeInstIfThen( TreeNode ) );
            c_printf( stdout, "ELSE      :" );
            c_tree_view_node( CTreeInstIfElse( TreeNode ) );
          break;
          case C_GOTO_INST :
            c_printf( stdout, "LABEL     :" );
            c_tree_view_node( CTreeInstGotoLabel( TreeNode ) );
          break;
          case C_RETURN_INST :
            c_printf( stdout, "VALUE     :" );
            c_tree_view_node( CTreeInstReturnValue( TreeNode ) );
          break;
          case C_CONTINUE_INST :
          break;
          case C_BREAK_INST :
          break;
          case C_SWITCH_INST :
            c_printf( stdout, "ON        :" );
            c_tree_view_node( CTreeInstSwitchOn( TreeNode ) );
            c_printf( stdout, "BODY      :" );
            c_tree_view_node( CTreeInstSwitchBody( TreeNode ) );
          break;
          case C_CASE_INST :
            CaseCode = CTreeInstCaseCode( TreeNode );
            c_printf( stdout, "CASE_CODE : %s\n", C_CASE_CODE_NAME[ CaseCode ] );
            c_printf( stdout, "FROM      :" );
            c_tree_view_node( CTreeInstCaseFrom( TreeNode ) );
            c_printf( stdout, "TO        :" );
            c_tree_view_node( CTreeInstCaseTo( TreeNode ) );
          break;
        }
      break;
    }

    TreeNode = CTreeChain( TreeNode );
    c_printf( stdout, "}\n" );

    if ( ! ViewList ) break;
  }

  if ( C_TREE_FORCE_VIEW_NODE )
  {
    for ( TreeNode  = TreeHead;
          TreeNode != (c_tree_node *)0;
          TreeNode  = CTreeChain( TreeNode ) )
    {
      CClearTreeNodeMark( TreeNode );
    }
  }

  CViewIdent--;
}

void c_tree_view_node_list( TreeNode )

  c_tree_node *TreeNode;
{
  if ( TreeNode == (c_tree_node *)0 )
  {
    c_tree_view_node( TreeNode );
  }

  while ( TreeNode != (c_tree_node *)0 )
  {
    c_tree_view_node( TreeNode );
    TreeNode = CTreeChain( TreeNode );
  }
}

char c_tree_get_code_class( NodeCode )

  int NodeCode;
{
static int  LastCode  = -1;
static char LastClass = 'x';

  int FirstCode;

  if ( NodeCode == LastCode ) return LastClass;

  LastCode  = NodeCode;
  FirstCode = CFirstCode( NodeCode );

  if ( FirstCode == C_TYPE_NODE )
  {
    LastClass = 't';
  }
  else
  if ( FirstCode == C_BLOCK_NODE )
  {
    LastClass = 'b';
  }
  else
  if ( FirstCode == C_INT_CST_NODE ||
       FirstCode == C_REAL_CST_NODE ||
       FirstCode == C_STRING_NODE ||
       FirstCode == C_COMPLEX_NODE )
  {
    LastClass = 'c';
  }
  else
  if ( FirstCode == C_DECL_NODE )
  {
    LastClass = 'd';
  }
  else
  if ( NodeCode == C_COMPONENT_REF ||
       NodeCode == C_BIT_FIELD_REF ||
       NodeCode == C_INDIRECT_REF ||
       NodeCode == C_BUFFER_REF ||
       NodeCode == C_ARRAY_REF)
  {
    LastClass = 'r';
  }
  else
  if ( FirstCode == C_EXPR_NODE )
  {
    if ( NodeCode == C_LT_EXPR ||
         NodeCode == C_LE_EXPR ||
         NodeCode == C_GT_EXPR ||
         NodeCode == C_GE_EXPR ||
         NodeCode == C_EQ_EXPR ||
         NodeCode == C_NE_EXPR ||
         NodeCode == C_SET_LE_EXPR ) LastClass = '<';
    else
    if ( NodeCode == C_POPDHC_EXPR ||
         NodeCode == C_POPDCC_EXPR  ||
         NodeCode == C_LABEL_EXPR ||
         NodeCode == C_GOTO_EXPR ||
         NodeCode == C_RETURN_EXPR ||
         NodeCode == C_EXIT_EXPR ||
         NodeCode == C_LOOP_EXPR ) LastClass = 's';
    else
    if ( NodeCode == C_FIX_TRUNC_EXPR ||
         NodeCode == C_FIX_CEIL_EXPR ||
         NodeCode == C_FIX_FLOOR_EXPR ||
         NodeCode == C_FIX_ROUND_EXPR ||
         NodeCode == C_FLOAT_EXPR ||
         NodeCode == C_NEGATE_EXPR ||
         NodeCode == C_ABS_EXPR ||
         NodeCode == C_FFS_EXPR ||
         NodeCode == C_BIT_NOT_EXPR ||
         NodeCode == C_CARD_EXPR ||
         NodeCode == C_CONVERT_EXPR ||
         NodeCode == C_NOP_EXPR ||
         NodeCode == C_NON_LVALUE_EXPR ||
         NodeCode == C_CONJ_EXPR ||
         NodeCode == C_REALPART_EXPR ||
         NodeCode == C_IMAGPART_EXPR ) LastClass = '1';
    else
    if ( NodeCode == C_PLUS_EXPR ||
         NodeCode == C_MINUS_EXPR ||
         NodeCode == C_MULT_EXPR ||
         NodeCode == C_TRUNC_DIV_EXPR ||
         NodeCode == C_CEIL_DIV_EXPR ||
         NodeCode == C_FLOOR_DIV_EXPR ||
         NodeCode == C_ROUND_DIV_EXPR ||
         NodeCode == C_TRUNC_MOD_EXPR ||
         NodeCode == C_CEIL_MOD_EXPR ||
         NodeCode == C_FLOOR_MOD_EXPR ||
         NodeCode == C_ROUND_MOD_EXPR ||
         NodeCode == C_RDIV_EXPR ||
         NodeCode == C_EXACT_DIV_EXPR ||
         NodeCode == C_EXPON_EXPR ||
         NodeCode == C_MIN_EXPR ||
         NodeCode == C_MAX_EXPR ||
         NodeCode == C_LSHIFT_EXPR ||
         NodeCode == C_RSHIFT_EXPR ||
         NodeCode == C_LROTATE_EXPR ||
         NodeCode == C_RROTATE_EXPR ||
         NodeCode == C_BIT_IOR_EXPR ||
         NodeCode == C_BIT_XOR_EXPR ||
         NodeCode == C_BIT_AND_EXPR ||
         NodeCode == C_BIT_ANDTC_EXPR ||
         NodeCode == C_IN_EXPR ||
         NodeCode == C_RANGE_EXPR ||
         NodeCode == C_COMPLEX_EXPR ) LastClass = '2';
    else
         LastClass = 'e';
  }
  else
  {
    LastClass = 'x';
  }

  return LastClass;
}


/* Return 1 if EXPR is an integer containing all 1's in as much precision as
   it contains.  Likewise for the corresponding complex constant.  */

int c_tree_is_integer_all_onesp(expr)
     c_tree_node *expr;
{
  int prec;
  int uns;

  CStripNops(expr);

  if ( CTreeNodeCode(expr) == C_COMPLEX_NODE
      && c_tree_is_integer_all_onesp( CTreeComplexReal(expr))
      && c_tree_is_integer_zerop( CTreeComplexImag(expr)))
    return 1;

  else if (CTreeNodeCode(expr) != C_INT_CST_NODE
           || CIsTreeNodeCstOverflow(expr))
    return 0;

  uns = CIsTreeNodeUnsigned(CTreeType(expr));
  if (!uns)
    return CTreeIntCstLow(expr) == -1 && CTreeIntCstHigh(expr) == -1;

  /* Note that using CTreeTypePrecisionhere is wrong.  We care about the
     actual bits, not the (arbitrary) range of the type.  */
# ifdef LUDO_NO_SKIP
  prec = GET_MODE_BITSIZE (CTreeTypeMode(CTreeType(expr)));
  if (prec >= HOST_BITS_PER_WIDE_INT)
    {
      int high_value, shift_amount;

      shift_amount = prec - HOST_BITS_PER_WIDE_INT;

      if (shift_amount > HOST_BITS_PER_WIDE_INT)
        /* Can not handle precisions greater than twice the host int size.  */
        abort ();
      else if (shift_amount == HOST_BITS_PER_WIDE_INT)
        /* Shifting by the host word size is undefined according to the ANSI
           standard, so we must handle this as a special case.  */
        high_value = -1;
      else
        high_value = ((C_C_HOST_WIDE_INT) 1 << shift_amount) - 1;

      return CTreeIntCstLow(expr) == -1
        && CTreeIntCstHigh(expr) == high_value;
    }
  else
# endif
    return CTreeIntCstLow(expr) == ((C_HOST_WIDE_INT) 1 << prec) - 1;
}

void c_tree_free_all_node()
{
  c_tree_node *FreeNode;
  int          Index;
  int          FirstCode;

  for ( Index = 0; Index < C_MAX_NODE_CODE; Index++ )
  {
    if ( Index == C_IDENTIFIER_NODE ) continue;

    while ( C_HEAD_NODE_LIST[ Index ] != (c_tree_node *)0 )
    {
      FreeNode = C_HEAD_NODE_LIST[ Index ];
      C_HEAD_NODE_LIST[ Index ] = FreeNode->COMMON.NEXT;

      FirstCode = CTreeNodeFirstCode( FreeNode );
      autfreeheap( FreeNode, C_NODE_SIZE[ FirstCode ] );
    }
  }
}
