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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     C                           |
|                                                             |
| File    :                   c_convert.c                   |
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

# include "mut.h"
# include "aut.h"
# include "gcp.h"

# include "c_tree.h"
# include "c_treecompat.h"
# include "c_cdecl.h"
# include "c_ctypeck.h"
# include "c_flags.h"
# include "c_convert.h"
# include "c_foldconst.h"
# include "c_debug.h"

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/


/* Convert EXPR to some pointer or reference type TYPE.

   EXPR must be pointer, reference, integer, enumeral, or literal zero;
   in other cases error is called.  */

c_tree_node *c_convert_to_pointer( type, expr )

     c_tree_node *type;
     c_tree_node *expr;
{
  c_tree_node *intype = CTreeType(expr);
  int form = CTreeTypeCode(intype);
  
  if ( c_tree_is_integer_zerop(expr))
  {
      expr = c_tree_build_int_2(0, 0);
      CTreeType(expr) = type;
      return expr;
  }

  if (form == C_POINTER_TYPE || form == C_REFERENCE_TYPE)
  {
    expr = c_tree_build_unary_expr( C_NOP_EXPR, expr );
    CTreeType( expr ) = type;

    return( expr );
  }


  if ( form == C_INTEGER_TYPE || form == C_ENUMERAL_TYPE )
  {
    if (c_type_precision(intype) == C_POINTER_TYPE_SIZE)
    {
      expr = c_tree_build_unary_expr( C_CONVERT_EXPR, expr );
      CTreeType( expr ) = type;

      return expr;
    }

    expr = c_convert_expr(c_get_type_for_size(C_POINTER_TYPE_SIZE, 0), expr);
    /* Modes may be different but sizes should be the same.  */
# ifdef LUDO_DONE
      if (GET_MODE_SIZE (TYPE_MODE (TREE_TYPE (expr)))
          != GET_MODE_SIZE (TYPE_MODE (type)))
        /* There is supposed to be some integral type
           that is the same width as a pointer.  */
        abort ();
# endif
    return c_convert_to_pointer(type, expr);
  }

  error ("cannot convert to a pointer type");

  expr = c_tree_build_int_2(0, 0);
  CTreeType(expr) = type;

  return expr;
}

/* Convert EXPR to some floating-point type TYPE.

   EXPR must be float, integer, or enumeral;
   in other cases error is called.  */

c_tree_node *c_convert_to_real(type, expr)
     c_tree_node *type;
     c_tree_node *expr;
{
  int form = CTreeTypeCode( CTreeType(expr));

  if (form == C_REAL_TYPE)
  {
    expr = c_tree_build_unary_expr( 
             c_flag_float_store ? C_CONVERT_EXPR : C_NOP_EXPR, expr );

    CTreeType( expr ) = type;

    return( expr );
  }

  if ( INTEGRAL_TYPE_P( CTreeType(expr)))
  {
    expr = c_tree_build_unary_expr( C_FLOAT_EXPR, expr );
    CTreeType( expr ) = type;

    return( expr );
  }

  if (form == C_COMPLEX_TYPE)
  {
    c_tree_node *tmp;

    tmp = c_tree_build_unary_expr( C_REALPART_EXPR, expr );
    CTreeType( tmp ) = CTreeType( CTreeType( expr ) );
    tmp = c_fold( tmp );

    expr = c_convert_expr( type, tmp );

    return( expr );
  }

  if (form == C_POINTER_TYPE || form == C_REFERENCE_TYPE)
  {
    error ("pointer value used where a floating point value was expected");
  }
  else
  {
    error ("aggregate value used where a float was expected");
  }

  {
    c_tree_node *tem = c_tree_add_node( C_REAL_CST_NODE );
    CTreeType(tem) = type;
    CTreeRealCstValue(tem) = 0.0;
# ifdef LUDO_DONE
    TREE_REAL_CST (tem) = REAL_VALUE_ATOF ("0.0", TYPE_MODE (type));
# endif
    return tem;
  }
}

/* Convert EXPR to the complex type TYPE in the usual ways.  */

c_tree_node *c_convert_to_complex(type, expr)
     c_tree_node *type;
     c_tree_node *expr;
{
  int form = CTreeTypeCode( CTreeType(expr));
  c_tree_node *subtype = CTreeType(type);
  
  if (form == C_REAL_TYPE || form == C_INTEGER_TYPE || form == C_ENUMERAL_TYPE)
  {
      expr = c_convert_expr( subtype, expr );

      expr = c_tree_build_binary_expr( 
               C_COMPLEX_EXPR, expr,
               c_convert_expr(subtype, c_integer_zero_node));

      CTreeType( expr ) = type;

      return( expr );
  }

  if ( form == C_COMPLEX_TYPE )
  {
    c_tree_node *elt_type =  CTreeType( CTreeType(expr));
    if ( CTreeTypeMainVariant(elt_type) == CTreeTypeMainVariant(subtype))
    {
      return expr;
    }
    else if ( CTreeExprCode(expr) == C_COMPLEX_EXPR)
    {
      expr = c_tree_build_binary_expr(
                C_COMPLEX_EXPR,
                c_convert_expr(subtype, CTreeExprOperand(expr, 0)),
                c_convert_expr(subtype, CTreeExprOperand(expr, 1)));
      CTreeType( expr ) = type;
      expr = c_fold( expr );

      return( expr );
    }
    else
    {
      c_tree_node *oper1;
      c_tree_node *oper2;

      oper1 = c_tree_build_unary_expr( C_REALPART_EXPR, expr);
      CTreeType( oper1 ) = CTreeType( CTreeType(expr));
      oper1 = c_fold( oper1 );

      oper2 = c_tree_build_unary_expr( C_IMAGPART_EXPR, expr);
      CTreeType( oper2 ) = CTreeType( CTreeType(expr));
      oper2 = c_fold( oper2 );

      expr = c_tree_build_binary_expr(
               C_COMPLEX_EXPR,
               c_convert_expr(subtype,oper1),
               c_convert_expr(subtype,oper2) );

      CTreeType( expr ) = type;
      expr = c_fold( expr );

      return( expr );
    }
  }

  if (form == C_POINTER_TYPE || form == C_REFERENCE_TYPE)
  {
    error ("pointer value used where a complex was expected");
  }
  else
  {
    error ("aggregate value used where a complex was expected");
  }
  
  expr = c_tree_build_binary_expr( C_COMPLEX_EXPR,
                c_convert_expr(subtype, c_integer_zero_node),
                c_convert_expr(subtype, c_integer_zero_node));

  CTreeType( expr ) = type;

  return( expr );
}

/* Convert EXPR to some integer (or enum) type TYPE.
   EXPR must be pointer, integer, discrete (enum, char, or bool), or float;
   in other cases error is called.

   The result of this is always supposed to be a newly created tree node
   not in use in any existing structure.  */

c_tree_node *c_convert_to_integer(type, expr)
     c_tree_node *type;
     c_tree_node *expr;
{
  c_tree_node *intype = CTreeType(expr);
  int form = CTreeTypeCode(intype);

  if (form == C_POINTER_TYPE || form == C_REFERENCE_TYPE)
  {
    if (c_tree_is_integer_zerop (expr))
    {
     expr = c_integer_zero_node;
     intype = CTreeType( expr );
    }
    else
    {
      intype = c_get_type_for_size( C_POINTER_TYPE_SIZE, 0);
      expr = c_tree_build_unary_expr( C_CONVERT_EXPR, expr);
      CTreeType( expr ) = intype;
      expr = c_fold( expr );
    }

    form = CTreeTypeCode(intype);

    if (intype == type)
    {
      return expr;
    }
  }

  if (form == C_INTEGER_TYPE || form == C_ENUMERAL_TYPE
      || form == C_BOOLEAN_TYPE || form == C_CHAR_TYPE)
  {
    unsigned outprec = CTreeTypePrecision(type);
    unsigned inprec = CTreeTypePrecision(intype);
    int ex_form = CTreeExprCode(expr);

    /* If we are widening the type, put in an explicit conversion.
       Similarly if we are not changing the width.  However, if this is
       a logical operation that just returns 0 or 1, we can change the
       type of the expression.  For logical operations, we must
       also change the types of the operands to maintain type
       correctness.  */

    if ( (ex_form == C_LT_EXPR) ||
         (ex_form == C_LE_EXPR) ||
         (ex_form == C_GT_EXPR) ||
         (ex_form == C_GE_EXPR) ||
         (ex_form == C_EQ_EXPR) ||
         (ex_form == C_NE_EXPR) ||
         (ex_form == C_SET_LE_EXPR) )
      {
        CTreeType(expr) = type;
        return expr;
      }
    else if (ex_form == C_TRUTH_AND_EXPR || ex_form == C_TRUTH_ANDIF_EXPR
             || ex_form == C_TRUTH_OR_EXPR || ex_form == C_TRUTH_ORIF_EXPR
             || ex_form == C_TRUTH_XOR_EXPR)
      {
        CTreeExprOperand(expr, 0) = c_convert_expr(type, CTreeExprOperand(expr, 0));
        CTreeExprOperand(expr, 1) = c_convert_expr(type, CTreeExprOperand(expr, 1));
        CTreeType(expr) = type;

        return expr;
      }
    else if (ex_form == C_TRUTH_NOT_EXPR)
      {
        CTreeExprOperand(expr, 0) = c_convert_expr(type, CTreeExprOperand(expr, 0));
        CTreeType(expr) = type;
        return expr;
      }
    else if (outprec >= inprec)
    {
      expr = c_tree_build_unary_expr( C_NOP_EXPR, expr);
      CTreeType( expr ) = type;
      return expr;
    }

    /* If TYPE is an enumeral type or a type with a precision less
       than the number of bits in its mode, do the conversion to the
       type corresponding to its mode, then do a nop conversion
       to TYPE.  */
# ifdef LUDO_DONE
    else if ( CTreeTypeCode(type) == C_ENUMERAL_TYPE
             || outprec != GET_MODE_BITSIZE(TYPE_MODE (type)))
    {
      return build1 (NOP_EXPR, type,
                     convert (type_for_mode (TYPE_MODE (type),
                                             TREE_UNSIGNED (type)),
                              expr));
    }
# endif

    /* Here detect when we can distribute the truncation down past some
       arithmetic.  For example, if adding two longs and converting to an
       int, we can equally well convert both to ints and then add.
       For the operations handled here, such truncation distribution
       is always safe.
       It is desirable in these cases:
       1) when truncating down to full-word from a larger size
       2) when truncating takes no work.
       3) when at least one operand of the arithmetic has been extended
       (as by C's default conversions).  In this case we need two conversions
       if we do the arithmetic as already requested, so we might as well
       truncate both and then combine.  Perhaps that way we need only one.

       Note that in general we cannot do the arithmetic in a type
       shorter than the desired result of conversion, even if the operands
       are both extended from a shorter type, because they might overflow
       if combined in that type.  The exceptions to this--the times when
       two narrow values can be combined in their narrow type even to
       make a wider result--are handled by "shorten" in build_binary_op.  */

    switch (ex_form)
      {
      case C_RSHIFT_EXPR:
        /* We can pass truncation down through right shifting
           when the shift count is a nonpositive constant.  */
        if ( CTreeNodeFirstCode( CTreeExprOperand(expr, 1)) == C_INT_CST_NODE
            && c_tree_is_int_cst_lt( CTreeExprOperand(expr, 1),
                                c_convert_expr( CTreeType(CTreeExprOperand (expr, 1)),
                                         c_integer_one_node)))
          goto trunc1;
        break;

      case C_LSHIFT_EXPR:
        /* We can pass truncation down through left shifting
           when the shift count is a nonnegative constant.  */
        if ( CTreeNodeFirstCode(CTreeExprOperand(expr, 1)) == C_INT_CST_NODE
            && c_tree_is_int_cst_sgn(CTreeExprOperand(expr, 1)) >= 0
            && CTreeNodeFirstCode( CTreeTypeSize(type)) == C_INT_CST_NODE)
         {
            /* If shift count is less than the width of the truncated type,
               really shift.  */
            if ( c_tree_is_int_cst_lt(CTreeExprOperand(expr, 1), 
                    CTreeTypeSize(type)))
              /* In this case, shifting is like multiplication.  */
              goto trunc1;
            else
              {
                /* If it is >= that width, result is zero.
                   Handling this with trunc1 would give the wrong result:
                   (int) ((long long) a << 32) is well defined (as 0)
                   but (int) a << 32 is undefined and would get a
                   warning.  */

                c_tree_node *t = c_convert_to_integer(type, c_integer_zero_node);

                /* If the original expression had side-effects, we must
                   preserve it.  */
                if ( CIsTreeNodeSideEffect(expr))
                {
                  c_tree_node *tmp_node;

                  tmp_node = c_tree_build_binary_expr( C_COMPOUND_EXPR, expr, t );
                  CTreeType( tmp_node ) = type;

                  return tmp_node;
                }
                else
                  return t;
              }
        }
        break;

      case C_MAX_EXPR:
      case C_MIN_EXPR:
      case C_MULT_EXPR:
        {
          c_tree_node *arg0 = c_get_unwidened( CTreeExprOperand(expr, 0), type);
          c_tree_node *arg1 = c_get_unwidened( CTreeExprOperand(expr, 1), type);

          /* Don't distribute unless the output precision is at least as big
             as the actual inputs.  Otherwise, the comparison of the
             truncated values will be wrong.  */
          if (outprec >= CTreeTypePrecision( CTreeType(arg0))
              && outprec >= CTreeTypePrecision( CTreeType(arg1))
              /* If signedness of arg0 and arg1 don't match,
                 we can't necessarily find a type to compare them in.  */
              && ( CIsTreeNodeUnsigned( CTreeType(arg0))
                  ==  CIsTreeNodeUnsigned( CTreeType(arg1))))
            goto trunc1;
          break;
        }

      case C_PLUS_EXPR:
      case C_MINUS_EXPR:
      case C_BIT_AND_EXPR:
      case C_BIT_IOR_EXPR:
      case C_BIT_XOR_EXPR:
      case C_BIT_ANDTC_EXPR:
      trunc1:
        {
# ifdef LUDO_DONE
          c_tree_node * arg0 = c_get_unwidened(CTreeExprOperand (expr, 0), type);
          c_tree_node * arg1 = c_get_unwidened(CTreeExprOperand (expr, 1), type);

          if (outprec >= C_BITS_PER_WORD
              || TRULY_NOOP_TRUNCATION (outprec, inprec)
              || inprec > TYPE_PRECISION (TREE_TYPE (arg0))
              || inprec > TYPE_PRECISION (TREE_TYPE (arg1)))
            {
              /* Do the arithmetic in type TYPEX,
                 then convert result to TYPE.  */
              register tree typex = type;

              /* Can't do arithmetic in enumeral types
                 so use an integer type that will hold the values.  */
              if (TREE_CODE (typex) == ENUMERAL_TYPE)
                typex = type_for_size (TYPE_PRECISION (typex),
                                       TREE_UNSIGNED (typex));

              /* But now perhaps TYPEX is as wide as INPREC.
                 In that case, do nothing special here.
                 (Otherwise would recurse infinitely in convert.  */
              if (TYPE_PRECISION (typex) != inprec)
                {
                  /* Don't do unsigned arithmetic where signed was wanted,
                     or vice versa.
                     Exception: if either of the original operands were
                     unsigned then can safely do the work as unsigned.
                     And we may need to do it as unsigned
                     if we truncate to the original size.  */
                  typex = ((TREE_UNSIGNED (TREE_TYPE (expr))
                            || TREE_UNSIGNED (TREE_TYPE (arg0))
                            || TREE_UNSIGNED (TREE_TYPE (arg1)))
                           ? unsigned_type (typex) : signed_type (typex));
                  return convert (type,
                                  c_fold (build (ex_form, typex,
                                               convert (typex, arg0),
                                               convert (typex, arg1),
                                               0)));
                }
            }
# endif
        }
        break;

      case C_NEGATE_EXPR:
      case C_BIT_NOT_EXPR:
        /* This is not correct for ABS_EXPR,
           since we must test the sign before truncation.  */
        {
          c_tree_node *typex = type;

          /* Can't do arithmetic in enumeral types
             so use an integer type that will hold the values.  */
          if ( CTreeTypeCode(typex) == C_ENUMERAL_TYPE)
            typex = c_get_type_for_size( CTreeTypePrecision(typex),
                                   CIsTreeNodeUnsigned(typex));

          /* But now perhaps TYPEX is as wide as INPREC.
             In that case, do nothing special here.
             (Otherwise would recurse infinitely in convert.  */
          if ( CTreeTypePrecision(typex) != inprec)
            {
              c_tree_node *tmp_node;
              /* Don't do unsigned arithmetic where signed was wanted,
                 or vice versa.  */
              typex = ( CIsTreeNodeUnsigned( CTreeType(expr))
                       ? c_get_unsigned_type(typex) : c_get_signed_type(typex));

              tmp_node = c_tree_build_unary_expr( ex_form, 
                   c_convert_expr( typex, CTreeExprOperand( expr, 0 ) ) );
              CTreeType( tmp_node ) = typex;
              tmp_node = c_fold( tmp_node );

              return c_convert_expr(type, tmp_node );
            }
        }

      case C_NOP_EXPR:
        /* If truncating after truncating, might as well do all at once.
           If truncating after extending, we may get rid of wasted work.  */
        return c_convert_expr(type, c_get_unwidened(CTreeExprOperand(expr, 0), type));

      case C_COND_EXPR:
        /* Can treat the two alternative values like the operands
           of an arithmetic expression.  */
        {
# ifdef LUDO_DONE
          c_tree_node * arg1 = c_get_unwidened(CTreeExprOperand (expr, 1), type);
          c_tree_node * arg2 = c_get_unwidened(CTreeExprOperand (expr, 2), type);

          if (outprec >= BITS_PER_WORD
              || TRULY_NOOP_TRUNCATION (outprec, inprec)
              || inprec > TYPE_PRECISION (TREE_TYPE (arg1))
              || inprec > TYPE_PRECISION (TREE_TYPE (arg2)))
            {
              /* Do the arithmetic in type TYPEX,
                 then convert result to TYPE.  */
              register tree typex = type;

              /* Can't do arithmetic in enumeral types
                 so use an integer type that will hold the values.  */
              if (TREE_CODE (typex) == ENUMERAL_TYPE)
                typex = type_for_size (TYPE_PRECISION (typex),
                                       TREE_UNSIGNED (typex));

              /* But now perhaps TYPEX is as wide as INPREC.
                 In that case, do nothing special here.
                 (Otherwise would recurse infinitely in convert.  */
              if (TYPE_PRECISION (typex) != inprec)
                {
                  /* Don't do unsigned arithmetic where signed was wanted,
                     or vice versa.  */
                  typex = (TREE_UNSIGNED (TREE_TYPE (expr))
                           ? unsigned_type (typex) : signed_type (typex));
                  return convert (type,
                                  fold (build (COND_EXPR, typex,
                                               CTreeExprOperand (expr, 0),
                                               convert (typex, arg1),
                                               convert (typex, arg2))));
                }
              else
                /* It is sometimes worthwhile
                   to push the narrowing down through the conditional.  */
                return fold (build (COND_EXPR, type,
                                    CTreeExprOperand (expr, 0),
                                    convert (type, CTreeExprOperand (expr, 1)), 
                                    convert (type, CTreeExprOperand (expr, 2))));
            }
# endif
        }
        break;

      default:
        break;
      }

    expr = c_tree_build_unary_expr( C_NOP_EXPR, expr);
    CTreeType( expr ) = type;
    return expr;
  }

  if (form == C_REAL_TYPE)
  {
    expr = c_tree_build_unary_expr( C_FIX_TRUNC_EXPR, expr );
    CTreeType( expr ) = type;

    return( expr );
  }

  if (form == C_COMPLEX_TYPE)
  {
    c_tree_node *tmp_type;

    tmp_type = CTreeType( CTreeType( expr ) );
    expr     = c_tree_build_unary_expr( C_REALPART_EXPR, expr );
    CTreeType( expr ) = tmp_type;
    expr = c_fold( expr );
    expr = c_convert_expr( type, expr );

    return( expr );
  }

  error ("aggregate value used where an integer was expected");

  {
    c_tree_node *tem = c_tree_build_int_2 (0, 0);
    CTreeType(tem) = type;

    return tem;
  }
}

/* Create an expression whose value is that of EXPR,
   converted to type TYPE.  The TREE_TYPE of the value is always TYPE.  This
   function implements all reasonable conversions; callers should filter out
   those that are not permitted by the language being compiled.  */

c_tree_node *c_convert_expr( type, expr )

  c_tree_node *type;
  c_tree_node *expr;
{
  c_tree_node *e = expr;
  c_tree_node *result = c_error_mark_node;
  int type_type = CTreeTypeCode(type);

  if (type == CTreeType(expr)
      || CTreeNodeFirstCode(expr) == C_ERROR_NODE)
    result = expr;
  else
  if (CTreeTypeMainVariant(type) == CTreeTypeMainVariant( CTreeType(expr)))
  {
    e = c_tree_build_unary_expr( C_NOP_EXPR, e);
    CTreeType( e ) = type;

    result = c_fold( e );
  }
  else
  if ( CTreeNodeFirstCode( CTreeType(expr)) == C_ERROR_NODE)
  {
    result = c_error_mark_node;
  }
  else
  if ( CTreeTypeCode( CTreeType(expr)) == C_VOID_TYPE)
  {
     error("void value not ignored as it ought to be");
     result = c_error_mark_node;
  }
  else
  if (type_type == C_VOID_TYPE)
  {
    e = c_tree_build_unary_expr( C_CONVERT_EXPR, e);
    CTreeType( e ) = type;

    result = e;
  }
  else
  if (type_type == C_INTEGER_TYPE || type_type == C_ENUMERAL_TYPE)
  {
    result = c_fold( c_convert_to_integer(type, e) );
  }
  else
  if (type_type == C_POINTER_TYPE)
  {
    result = c_fold( c_convert_to_pointer(type, e) );
  }
  else
  if (type_type == C_REAL_TYPE)
  {
    result = c_fold( c_convert_to_real(type, e) );
  }
  else
  if (type_type == C_COMPLEX_TYPE)
  {
    result = c_fold( c_convert_to_complex(type, e) );
  }
  else
  {
    error("conversion to non-scalar type requested");
  }

  return result;
}


/* Return the innermost context enclosing DECL that is
   a FUNCTION_DECL, or zero if none.  */

c_tree_node *c_get_decl_function_context(decl)
     c_tree_node *decl;
{
  c_tree_node *context;

  if ( CTreeNodeFirstCode(decl) == C_ERROR_NODE)
    return 0;

  if ( CTreeNodeFirstCode(decl) == C_EXPR_NODE &&
       CTreeDeclCode(decl) == C_SAVE_EXPR )
    context = CTreeExprOperand(decl,1);
  else
    context = CTreeDeclContext(decl);

  while (context && ( CTreeNodeFirstCode(context) != C_DECL_NODE ||
                      CTreeDeclCode(context) != C_FUNCTION_DECL) )
    {
      if ( (CTreeNodeFirstCode(context) == C_TYPE_NODE) &&
          (  CTreeTypeCode(context) == C_RECORD_TYPE
          || CTreeTypeCode(context) == C_UNION_TYPE
          || CTreeTypeCode(context) == C_QUAL_UNION_TYPE))
        context = CTreeTypeContext(context);
      else if ( CTreeNodeFirstCode(context) == C_DECL_NODE &&
                CTreeDeclCode(context) == C_TYPE_DECL)
        context = CTreeDeclContext(context);
      else if ( CTreeNodeFirstCode(context) == C_BLOCK_NODE)
        context = CTreeBlockSuperContext(context);
      else
        /* Unhandled CONTEXT !?  */
        abort ();
    }

  return context;
}


/* Return nonzero if arg is static -- a reference to an object in
   static storage.  This is not the same as the C meaning of `static'.  */

int
c_is_staticp(arg)

     c_tree_node *arg;
{
  int code;

  code = CTreeNodeFirstCode(arg);

  if ( code == C_DECL_NODE )
  {
    switch ( CTreeDeclCode( arg ) )
    {
    case C_FUNCTION_DECL:
      /* Nested functions aren't static, since taking their address
	 involves a trampoline.  */
       return c_get_decl_function_context(arg) == 0 || CIsTreeDeclRegister(arg);
    case C_VAR_DECL:
      return CIsTreeNodeStatic(arg) || CIsTreeDeclExternal(arg);
    }
  }
  else
  if ( code == C_EXPR_NODE )
  {
    switch ( CTreeExprCode(arg))
    {
    case C_CONSTRUCTOR_EXPR:
      return CIsTreeNodeStatic(arg);

      /* If we are referencing a bitfield, we can't evaluate an
	 ADDR_EXPR at compile time and so it isn't a constant.  */
    case C_COMPONENT_REF:
      return (! CIsTreeDeclBitField( CTreeExprOperand(arg, 1))
	      && c_is_staticp( CTreeExprOperand(arg, 0)));

    case C_BIT_FIELD_REF:
      return 0;

    case C_ARRAY_REF:
      if ( CTreeNodeFirstCode( CTreeTypeSize( CTreeType(arg))) == C_INT_CST_NODE
	  &&  CTreeNodeFirstCode( CTreeExprOperand(arg, 1)) == C_INT_CST_NODE)
	return c_is_staticp( CTreeExprOperand(arg, 0));
    }
  }
  else
  if ( code == C_STRING_NODE )
  {
    return 1;
  }

  return 0;
}

/* Perform default promotions for C data used in expressions.
   Arrays and functions are converted to pointers;
   enumeral types or short or char, to int.
   In addition, manifest constants symbols are replaced by their values.  */

c_tree_node *c_default_conversion( exp )

     c_tree_node *exp;
{
  c_tree_node *type = CTreeType(exp);
  int code = CTreeTypeCode(type);

  /* Constants can be used directly unless they're not loadable.  */
  if ( ( CTreeNodeFirstCode(exp) == C_DECL_NODE ) &&
       ( CTreeDeclCode(exp) == C_CONST_DECL) )
  {
    exp = CTreeDeclInitial(exp);
  }

  /* Replace a nonvolatile const static variable with its value unless
     it is an array, in which case we must be sure that taking the
     address of the array produces consistent results.  */
# ifdef LUDO_DONE
  else if (optimize && TREE_CODE (exp) == VAR_DECL && code != ARRAY_TYPE)
    {
      exp = c_decl_constant_value(exp);
      type = TREE_TYPE (exp);
    }
# endif

  /* Strip NON_LVALUE_EXPRs and no-op conversions, since we aren't using as
     an lvalue.  */
  /* Do not use STRIP_NOPS here!  It will remove conversions from pointer
     to integer and cause infinite recursion.  */
  while ( CTreeExprCode(exp) == C_NON_LVALUE_EXPR
         || ( CTreeExprCode(exp) == C_NOP_EXPR
             && CTreeType( CTreeExprOperand(exp, 0)) == CTreeType(exp)))
  {
    exp = CTreeExprOperand(exp, 0);
  }

  /* Normally convert enums to int,
     but convert wide enums to something wider.  */
  if (code == C_ENUMERAL_TYPE)
  {
#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

      type = c_get_type_for_size( 
          MAX( CTreeTypePrecision(type), 
               CTreeTypePrecision(c_integer_type_node)),
                            ((c_flag_traditional
                              || (CTreeTypePrecision(type)
                                  >=  CTreeTypePrecision(c_integer_type_node)))
                             && CIsTreeNodeUnsigned(type)));
      return c_convert_expr(type, exp);
  }

  if ( CTreeExprCode(exp) == C_COMPONENT_REF
      && CIsTreeDeclCBitField( CTreeExprOperand(exp, 1)))
  {
    c_tree_node *width =  CTreeDeclSize( CTreeExprOperand(exp, 1));
    int low = CTreeIntCstLow(width);

    /* If it's thinner than an int, promote it like a
       C_PROMOTING_INTEGER_TYPE_P, otherwise leave it alone.  */

    if (low <  CTreeTypePrecision(c_integer_type_node))
    {
      if (c_flag_traditional && CIsTreeNodeUnsigned(type))
      {
        return c_convert_expr(c_unsigned_type_node, exp);
      }
      else
      {
        return c_convert_expr(c_integer_type_node, exp);
      }
    }
  }

#define C_PROMOTING_INTEGER_TYPE_P(t)				\
  ( CTreeTypeCode((t)) == C_INTEGER_TYPE				\
   && ( CTreeTypeMainVariant(t) == c_char_type_node			\
       || CTreeTypeMainVariant(t) == c_signed_char_type_node	\
       || CTreeTypeMainVariant(t) == c_unsigned_char_type_node	\
       || CTreeTypeMainVariant(t) == c_short_integer_type_node	\
       || CTreeTypeMainVariant(t) == c_short_unsigned_type_node))

  if (C_PROMOTING_INTEGER_TYPE_P (type))
  {
      /* Traditionally, unsignedness is preserved in default promotions.
         Also preserve unsignedness if not really getting any wider.  */
    if ( CIsTreeNodeUnsigned(type)
          && (c_flag_traditional
              || CTreeTypePrecision(type) == CTreeTypePrecision(c_integer_type_node)))
        return c_convert_expr(c_unsigned_type_node, exp);
      return c_convert_expr(c_integer_type_node, exp);
  }
  if (c_flag_traditional && !c_flag_allow_single_precision
      && CTreeTypeMainVariant(type) == c_float_type_node)
    return c_convert_expr(c_double_type_node, exp);

  if (code == C_VOID_TYPE)
  {
    error ("void value not ignored as it ought to be");
    return c_error_mark_node;
  }

  if (code == C_FUNCTION_TYPE)
  {
    return c_parser_build_unary_op( C_ADDR_EXPR, exp, 0);
  }

  if (code == C_ARRAY_TYPE)
  {
    c_tree_node *adr;
    c_tree_node *restype = CTreeType(type);
    c_tree_node *ptrtype;
    int constp = 0;
    int volatilep = 0;

    if ( ( CTreeNodeFirstCode(exp) == C_EXPR_NODE &&
           ( CTreeExprCode(exp) == C_COMPONENT_REF 
           || CTreeExprCode(exp ) == C_BIT_FIELD_REF
           || CTreeExprCode(exp ) == C_INDIRECT_REF
           || CTreeExprCode(exp ) == C_BUFFER_REF
           || CTreeExprCode(exp ) == C_ARRAY_REF))
       || ( CTreeNodeFirstCode(exp) == C_DECL_NODE ) )
    {
      constp = CIsTreeNodeReadOnly(exp);
      volatilep = CIsTreeNodeVolatile(exp);
    }

    if ( CIsTreeNodeReadOnly(type) || 
         CIsTreeNodeVolatile(type)
        || constp || volatilep)
    {
      restype = c_tree_build_type_variant(restype,
                                      CIsTreeNodeReadOnly(type) || constp,
                                      CIsTreeNodeVolatile(type) || volatilep);
    }
    if ( CTreeExprCode(exp) == C_INDIRECT_REF)
    {
      return c_convert_expr( CTreeTypePointerTo(restype), CTreeExprOperand(exp, 0));
    }

    if ( CTreeExprCode(exp) == C_COMPOUND_EXPR)
    {
      c_tree_node *op1 = c_default_conversion( CTreeExprOperand(exp, 1));

      exp = c_tree_build_binary_expr( C_COMPOUND_EXPR, CTreeExprOperand( exp, 0 ), op1 );
      CTreeType( exp ) = CTreeType( op1 );

      return exp;
    }

    if (! c_is_lvalue_p(exp)
        && ! ( CTreeExprCode(exp) == C_CONSTRUCTOR_EXPR && CIsTreeNodeStatic(exp)))
    {
        error ("invalid use of non-lvalue array");
        return c_error_mark_node;
    }

    ptrtype = c_tree_build_pointer_type(restype);

    if ( ( CTreeNodeFirstCode(exp)   == C_DECL_NODE ) &&
         ( CTreeDeclCode( exp ) == C_VAR_DECL  ) )
    {
        /* ??? This is not really quite correct
           in that the type of the operand of ADDR_EXPR
           is not the target type of the type of the ADDR_EXPR itself.
           Question is, can this lossage be avoided?  */
      adr = c_tree_build_unary_typed_expr(C_ADDR_EXPR, ptrtype, exp);
        if (c_mark_addressable(exp) == 0)
          return c_error_mark_node;
        if ( c_is_staticp(exp) ) CSetTreeNodeConstant(adr);
        CClearTreeNodeSideEffect(adr); /* Default would be, same as EXP.  */
        return adr;
    }
    /* This way is better for a COMPONENT_REF since it can
       simplify the offset for a component.  */
    adr = c_parser_build_unary_op( C_ADDR_EXPR, exp, 1);
    return c_convert_expr (ptrtype, adr);
  }

  return exp;
}

/* Prepare expr to be an argument of a TRUTH_NOT_EXPR,
   or validate its data type for an `if' or `while' statement or ?..: exp.

   This preparation consists of taking the ordinary
   representation of an expression expr and producing a valid tree
   boolean expression describing whether expr is nonzero.  We could
   simply always do build_binary_op (NE_EXPR, expr, boolean_false_node, 1),
   but we optimize comparisons, &&, ||, and !.

   The resulting type should always be `boolean_type_node'.  */

c_tree_node *c_truthvalue_conversion(expr)

  c_tree_node *expr;
{
  c_tree_node *tmp_node;

  if ( CTreeNodeFirstCode(expr) == C_ERROR_NODE)
  {
    return expr;
  }

  if ( CTreeNodeFirstCode(expr) == C_INT_CST_NODE )
  {
    return c_tree_is_integer_zerop(expr) ? c_boolean_false_node : c_boolean_true_node;
  }

  if ( CTreeNodeFirstCode(expr) == C_REAL_CST_NODE )
  {
    return c_tree_is_real_zerop(expr) ? c_boolean_false_node : c_boolean_true_node;
  }

  switch ( CTreeExprCode(expr))
  {
    case C_EQ_EXPR:
    case C_NE_EXPR:
    case C_LE_EXPR: 
    case C_GE_EXPR:
    case C_LT_EXPR:
    case C_GT_EXPR:
    case C_TRUTH_ANDIF_EXPR:
    case C_TRUTH_ORIF_EXPR:
    case C_TRUTH_AND_EXPR:
    case C_TRUTH_OR_EXPR:
    case C_TRUTH_XOR_EXPR:
    case C_TRUTH_NOT_EXPR:

      CTreeType(expr) = c_boolean_type_node;

    return expr;

    case C_ADDR_EXPR:
      /* If we are taking the address of a external decl, it might be zero
         if it is weak, so we cannot optimize.  */
      if ( CTreeNodeFirstCode(CTreeExprOperand (expr, 0)) == C_DECL_NODE
          && CIsTreeDeclExternal(CTreeExprOperand(expr, 0))) break;

      if (CIsTreeNodeSideEffect( CTreeExprOperand(expr, 0)))
      {
        tmp_node = c_tree_build_binary_expr( 
            C_COMPOUND_EXPR, CTreeExprOperand(expr, 0), c_boolean_true_node);
        CTreeType( tmp_node ) = c_boolean_type_node;

        return( tmp_node );
      }
      else
        return c_boolean_true_node;

    case C_COMPLEX_EXPR:

      tmp_node = c_build_binary_op(
          ( CIsTreeNodeSideEffect( CTreeExprOperand(expr, 1))
          ? C_TRUTH_OR_EXPR : C_TRUTH_ORIF_EXPR),
          c_truthvalue_conversion( CTreeExprOperand(expr, 0)),
          c_truthvalue_conversion( CTreeExprOperand(expr, 1)), 0 );

    case C_NEGATE_EXPR:
    case C_ABS_EXPR:
    case C_FLOAT_EXPR:
    case C_FFS_EXPR:
      /* These don't change whether an object is non-zero or zero.  */
      return c_truthvalue_conversion( CTreeExprOperand(expr, 0));

    case C_LROTATE_EXPR:
    case C_RROTATE_EXPR:
      /* These don't change whether an object is zero or non-zero, but
         we can't ignore them if their second arg has side-effects.  */
      if ( CIsTreeNodeSideEffect( CTreeExprOperand(expr, 1)))
      {
        tmp_node = c_tree_build_binary_expr( 
            C_COMPOUND_EXPR, CTreeExprOperand(expr, 1),
            c_truthvalue_conversion( CTreeExprOperand(expr, 0)));
        CTreeType( tmp_node ) = c_boolean_type_node;

        return( tmp_node );
      }
      else
        return c_truthvalue_conversion( CTreeExprOperand(expr, 0));
      
    case C_COND_EXPR:
      /* Distribute the conversion into the arms of a COND_EXPR.  */
      tmp_node = c_tree_build_ternary_expr( 
          C_COND_EXPR,
          CTreeExprOperand(expr, 0),
          c_truthvalue_conversion( CTreeExprOperand(expr, 1)),
          c_truthvalue_conversion( CTreeExprOperand(expr, 2)));

      CTreeType( tmp_node ) = c_boolean_type_node;

      return( tmp_node );

    case C_CONVERT_EXPR:
      /* Don't cancel the effect of a CONVERT_EXPR from a REFERENCE_TYPE,
         since that affects how `default_conversion' will behave.  */
      if ( CTreeTypeCode(CTreeType(expr)) == C_REFERENCE_TYPE
          ||  CTreeTypeCode(CTreeType(CTreeExprOperand(expr,0))) == C_REFERENCE_TYPE)
        break;
      /* fall through...  */
    case C_NOP_EXPR:
      /* If this is widening the argument, we can ignore it.  */
      if ( CTreeTypePrecision(CTreeType(expr))
          >= CTreeTypePrecision(CTreeType( CTreeExprOperand(expr, 0))))
        return c_truthvalue_conversion(CTreeExprOperand(expr, 0));
      break;

    case C_MINUS_EXPR:
      /* With IEEE arithmetic, x - x may not equal 0, so we can't optimize
         this case.  */
# ifdef LUDO_DONE
      if (TARGET_FLOAT_FORMAT == IEEE_FLOAT_FORMAT
          && TREE_CODE (CTreeType(expr)) == REAL_TYPE)
        break;
# endif
      /* fall through...  */
    case C_BIT_XOR_EXPR:
      /* This and MINUS_EXPR can be changed into a comparison of the
         two objects.  */
      if (CTreeType(CTreeExprOperand(expr, 0))
          == CTreeType(CTreeExprOperand(expr, 1)))
      {
        return( c_build_binary_op(
            C_NE_EXPR, CTreeExprOperand(expr, 0),
                                        CTreeExprOperand(expr, 1), 1 ) );
      }

      tmp_node = c_tree_build_unary_expr( C_NOP_EXPR, CTreeExprOperand(expr, 1));
      CTreeType( tmp_node ) = CTreeType(CTreeExprOperand(expr, 0));

      return c_build_binary_op( C_NE_EXPR, CTreeExprOperand(expr, 0), tmp_node, 1 );

    case C_BIT_AND_EXPR:

      if (c_tree_is_integer_onep(CTreeExprOperand (expr, 1))
          && CTreeType(expr) != c_boolean_type_node)
        /* Using convert here would cause infinite recursion.  */
      {
        tmp_node = c_tree_build_unary_expr( C_NOP_EXPR, expr );
        CTreeType( tmp_node ) = c_boolean_type_node;

        return( tmp_node );
      }
      break;

    case C_MODIFY_EXPR:

      if (c_warn_parentheses && CTreeExprComplexity(expr) == C_MODIFY_EXPR)
      {
        warning ("suggest parentheses around assignment used as truth value");
      }
      break;
      
    default:
      break;
  }

  if ( CTreeTypeCode(CTreeType(expr)) == C_COMPLEX_TYPE)
  {
    return ( c_build_binary_op(
        (CIsTreeNodeSideEffect(expr) ? C_TRUTH_OR_EXPR : C_TRUTH_ORIF_EXPR),
             c_truthvalue_conversion( 
                 c_parser_build_unary_op( C_REALPART_EXPR, expr, 0)),
             c_truthvalue_conversion(
                 c_parser_build_unary_op( C_IMAGPART_EXPR, expr, 0)), 0 ));
  }

  return c_build_binary_op( C_NE_EXPR, expr, c_integer_zero_node, 1 );
}


/* Print a warning if a large constant is truncated to unsigned,
   or if -Wconversion is used and a constant < 0 is converted to unsigned.
   Invoke this function on every expression that might be implicitly
   converted to an unsigned type.  */

void c_unsigned_conversion_warning(result, operand)
     c_tree_node *result;
     c_tree_node *operand;
{
  if ( CTreeNodeFirstCode(operand) == C_INT_CST_NODE
      && CTreeTypeCode(CTreeType(result)) == C_INTEGER_TYPE
      && CIsTreeNodeUnsigned(CTreeType(result))
      && c_skip_evaluation == 0
      && !c_int_fits_type_p(operand, CTreeType(result)))
    {
      if (!c_int_fits_type_p(operand, c_get_signed_type(CTreeType(result))))
        /* This detects cases like converting -129 or 256 to unsigned char.  */
      {
        warning ("large integer implicitly truncated to unsigned type");
      }
      else if (c_warn_conversion)
      {
        warning ("negative integer implicitly converted to unsigned type");
      }
    }
}

/* Convert EXPR to TYPE, warning about conversion problems with constants.
   Invoke this function on every expression that is converted implicitly,
   i.e. because of language rules and not because of an explicit cast.  */

c_tree_node *c_convert_and_check(type, expr)
     c_tree_node *type;
     c_tree_node *expr;
{
  c_tree_node *t = c_convert_expr(type, expr);
  if ( CTreeNodeFirstCode(t) == C_INT_CST_NODE)
    {
      if (CIsTreeNodeOverflow(t))
        {
          CClearTreeNodeOverflow(t);

          /* Do not diagnose overflow in a constant expression merely
             because a conversion overflowed.  */
          if ( CIsTreeNodeCstOverflow(expr) ) CSetTreeNodeCstOverflow(t);
          else                                CClearTreeNodeCstOverflow(t);

          /* No warning for converting 0x80000000 to int.  */
          if (!(CIsTreeNodeUnsigned(type) < CIsTreeNodeUnsigned(CTreeType(expr))
                && CTreeNodeCode(CTreeType(expr)) == C_INTEGER_TYPE
                && CTreeTypePrecision(type) == CTreeTypePrecision(CTreeType(expr))))
            /* If EXPR fits in the unsigned version of TYPE,
               don't warn unless pedantic.  */
            if ((c_pedantic
                 || CIsTreeNodeUnsigned(type)
                 || ! c_int_fits_type_p(expr, c_get_unsigned_type(type)))
                && c_skip_evaluation == 0)
            {
                warning ("overflow in implicit constant conversion");
            }
        }
      else
        c_unsigned_conversion_warning(t, expr);
    }
  return t;
}
