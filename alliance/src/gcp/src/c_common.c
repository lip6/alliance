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
| Tool    :                     C                             |
|                                                             |
| File    :                   c_common.c                      |
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

# include "c_flags.h"
# include "c_tree.h"
# include "c_treecompat.h"
# include "c_cdecl.h"
# include "c_convert.h"
# include "c_ctypeck.h"
# include "c_common.h"
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


/* Print a warning if a constant expression had overflow in folding.
   Invoke this function on every expression that the language
   requires to be a constant expression.
   Note the ANSI C standard says it is erroneous for a
   constant expression to overflow.  */

void c_constant_expression_warning (value)
     c_tree_node *value;
{
  if (( CTreeNodeFirstCode(value) == C_INT_CST_NODE || CTreeNodeFirstCode(value) == C_REAL_CST_NODE
       || CTreeNodeFirstCode(value) == C_COMPLEX_NODE)
      && CIsTreeNodeStatic(value) && c_pedantic)
  {
    pedwarn ("overflow in constant expression");
  }
}

/* Given X, an unsigned number, return the largest int Y such that 2**Y <= X.
   If X is 0, return -1.

   This should be used via the floor_log2 macro.  */

int
c_floor_log2_wide (x)
     register unsigned C_HOST_WIDE_INT x;
{
  register int log = -1;
  while (x != 0)
    log++,
    x >>= 1;
  return log;
}


/* Return the minimum number of bits needed to represent VALUE in a
   signed or unsigned type, UNSIGNEDP says which.  */

int c_min_precision (value, unsignedp)
     c_tree_node *value;
     int unsignedp;
{
  int log;

  /* If the value is negative, compute its negative minus 1.  The latter
     adjustment is because the absolute value of the largest negative value
     is one larger than the largest positive value.  This is equivalent to
     a bit-wise negation, so use that operation instead.  */

  if (c_tree_is_int_cst_sgn(value) < 0)
    value = c_fold (c_tree_build_unary_typed_expr(C_BIT_NOT_EXPR, CTreeType(value), value));

  /* Return the number of bits needed, taking into account the fact
     that we need one more bit for a signed than unsigned type.  */

  if (c_tree_is_integer_zerop (value))
    log = 0;
  else if (CTreeIntCstHigh(value) != 0)
    log = C_HOST_BITS_PER_WIDE_INT + c_floor_log2_wide (CTreeIntCstHigh(value));
  else
    log = c_floor_log2_wide (CTreeIntCstLow(value));

  return log + 1 + ! unsignedp;
}


/* Validate the expression after `case' and apply default promotions.  */

c_tree_node *c_check_case_value(value)
     c_tree_node *value;
{
  if (value == NULL)
    return value;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  CStripTypeNops(value);

  if ( CTreeNodeFirstCode(value) != C_INT_CST_NODE
      && value != c_error_mark_node)
    {
      error ("case label does not reduce to an integer constant");
      value = c_error_mark_node;
    }
  else
    /* Promote char or short to int.  */
    value = c_default_conversion(value);

  c_constant_expression_warning(value);

  return value;
}


/* Subroutine of build_binary_op, used for comparison operations.
   See if the operands have both been converted from subword integer types
   and, if so, perhaps change them both back to their original type.
   This function is also responsible for converting the two operands
   to the proper common type for comparison.

   The arguments of this function are all pointers to local variables
   of build_binary_op: OP0_PTR is &OP0, OP1_PTR is &OP1,
   RESTYPE_PTR is &RESULT_TYPE and RESCODE_PTR is &RESULTCODE.

   If this function returns nonzero, it means that the comparison has
   a constant value.  What this function returns is an expression for
   that value.  */

c_tree_node *c_shorten_compare(op0_ptr, op1_ptr, restype_ptr, rescode_ptr)
     c_tree_node **op0_ptr;
     c_tree_node **op1_ptr;
     c_tree_node **restype_ptr;
     int  *rescode_ptr;
{
  c_tree_node *type;
  c_tree_node *op0 = *op0_ptr;
  c_tree_node *op1 = *op1_ptr;
  int unsignedp0, unsignedp1;
  int real1, real2;
  c_tree_node *primop0;
  c_tree_node *primop1;
  int  code = *rescode_ptr;

  /* Throw away any conversions to wider types
     already present in the operands.  */

  primop0 = c_get_narrower(op0, &unsignedp0);
  primop1 = c_get_narrower(op1, &unsignedp1);

  /* Handle the case that OP0 does not *contain* a conversion
     but it *requires* conversion to FINAL_TYPE.  */

  if (op0 == primop0 && CTreeType(op0) != *restype_ptr)
    unsignedp0 = CIsTreeNodeUnsigned(CTreeType(op0));
  if (op1 == primop1 && CTreeType(op1) != *restype_ptr)
    unsignedp1 = CIsTreeNodeUnsigned(CTreeType(op1));

  /* If one of the operands must be floated, we cannot optimize.  */
  real1 = CTreeTypeCode(CTreeType(primop0)) == C_REAL_TYPE;
  real2 = CTreeTypeCode(CTreeType(primop1)) == C_REAL_TYPE;

  /* If first arg is constant, swap the args (changing operation
     so value is preserved), for canonicalization.  Don't do this if
     the second arg is 0.  */

  if ( CIsTreeNodeConstant(primop0)
      && ! c_tree_is_integer_zerop(primop1) && ! 
           c_tree_is_real_zerop(primop1))
    {
      c_tree_node *tem = primop0;
      int temi = unsignedp0;
      primop0 = primop1;
      primop1 = tem;
      tem = op0;
      op0 = op1;
      op1 = tem;
      *op0_ptr = op0;
      *op1_ptr = op1;
      unsignedp0 = unsignedp1;
      unsignedp1 = temi;
      temi = real1;
      real1 = real2;
      real2 = temi;

      switch (code)
        {
        case C_LT_EXPR:
          code = C_GT_EXPR;
          break;
        case C_GT_EXPR:
          code = C_LT_EXPR;
          break;
        case C_LE_EXPR:
          code = C_GE_EXPR;
          break;
        case C_GE_EXPR:
          code = C_LE_EXPR;
          break;
        default:
          break;
        }
      *rescode_ptr = code;
    }

  /* If comparing an integer against a constant more bits wide,
     maybe we can deduce a value of 1 or 0 independent of the data.
     Or else truncate the constant now
     rather than extend the variable at run time.

     This is only interesting if the constant is the wider arg.
     Also, it is not safe if the constant is unsigned and the
     variable arg is signed, since in this case the variable
     would be sign-extended and then regarded as unsigned.
     Our technique fails in this case because the lowest/highest
     possible unsigned results don't follow naturally from the
     lowest/highest possible values of the variable operand.
     For just EQ_EXPR and NE_EXPR there is another technique that
     could be used: see if the constant can be faithfully represented
     in the other operand's type, by truncating it and reextending it
     and see if that preserves the constant's value.  */

  if (!real1 && !real2
      &&  CTreeNodeFirstCode(primop1) == C_INT_CST_NODE
      && CTreeTypePrecision(CTreeType(primop0)) < CTreeTypePrecision(*restype_ptr))
    {
      int min_gt, max_gt, min_lt, max_lt;
      c_tree_node *maxval;
      c_tree_node *minval;
      /* 1 if comparison is nominally unsigned.  */
      int unsignedp = CIsTreeNodeUnsigned(*restype_ptr);
      c_tree_node *val;

      type = c_get_signed_or_unsigned_type(unsignedp0, CTreeType(primop0));

      maxval = CTreeTypeMaxValue(type);
      minval = CTreeTypeMinValue(type);

      if (unsignedp && !unsignedp0)
        *restype_ptr = c_get_signed_type(*restype_ptr);

      if (CTreeType(primop1) != *restype_ptr)
        primop1 = c_convert_expr(*restype_ptr, primop1);
      if (type != *restype_ptr)
        {
          minval = c_convert_expr(*restype_ptr, minval);
          maxval = c_convert_expr(*restype_ptr, maxval);
        }

      if (unsignedp && unsignedp0)
        {
          min_gt = INT_CST_LT_UNSIGNED (primop1, minval);
          max_gt = INT_CST_LT_UNSIGNED (primop1, maxval);
          min_lt = INT_CST_LT_UNSIGNED (minval, primop1);
          max_lt = INT_CST_LT_UNSIGNED (maxval, primop1);
        }
      else
        {
          min_gt = INT_CST_LT (primop1, minval);
          max_gt = INT_CST_LT (primop1, maxval);
          min_lt = INT_CST_LT (minval, primop1);
          max_lt = INT_CST_LT (maxval, primop1);
        }

      val = 0;
      /* This used to be a switch, but Genix compiler can't handle that.  */
      if (code == C_NE_EXPR)
        {
          if (max_lt || min_gt)
            val = c_boolean_true_node;
        }
      else if (code == C_EQ_EXPR)
        {
          if (max_lt || min_gt)
            val = c_boolean_false_node;
        }
      else if (code == C_LT_EXPR)
        {
          if (max_lt)
            val = c_boolean_true_node;
          if (!min_lt)
            val = c_boolean_false_node;
        }
      else if (code == C_GT_EXPR)
        {
          if (min_gt)
            val = c_boolean_true_node;
          if (!max_gt)
            val = c_boolean_false_node;
        }
      else if (code == C_LE_EXPR)
        {
          if (!max_gt)
            val = c_boolean_true_node;
          if (min_gt)
            val = c_boolean_false_node;
        }
      else if (code == C_GE_EXPR)
        {
          if (!min_lt)
            val = c_boolean_true_node;
          if (max_lt)
            val = c_boolean_false_node;
        }

      /* If primop0 was sign-extended and unsigned comparison specd,
         we did a signed comparison above using the signed type bounds.
         But the comparison we output must be unsigned.

         Also, for inequalities, VAL is no good; but if the signed
         comparison had *any* fixed result, it follows that the
         unsigned comparison just tests the sign in reverse
         (positive values are LE, negative ones GE).
         So we can generate an unsigned comparison
         against an extreme value of the signed type.  */

      if (unsignedp && !unsignedp0)
        {
          if (val != 0)
            switch (code)
              {
              case C_LT_EXPR:
              case C_GE_EXPR:
                primop1 = CTreeTypeMinValue(type);
                val = 0;
                break;

              case C_LE_EXPR:
              case C_GT_EXPR:
                primop1 = CTreeTypeMaxValue(type);
                val = 0;
                break;

              default:
                break;
              }
          type = c_get_unsigned_type(type);
        }

      if (!max_gt && !unsignedp0 && CTreeNodeFirstCode(primop0) != C_INT_CST_NODE)
        {
          /* This is the case of (char)x >?< 0x80, which people used to use
             expecting old C compilers to change the 0x80 into -0x80.  */
          if (val == c_boolean_false_node)
          {
            warning ("comparison is always 0 due to limited range of data type");
          }
          if (val == c_boolean_true_node)
          {
            warning ("comparison is always 1 due to limited range of data type");
          }
        }

      if (!min_lt && unsignedp0 && CTreeNodeFirstCode(primop0) != C_INT_CST_NODE)
        {
          /* This is the case of (unsigned char)x >?< -1 or < 0.  */
          if (val == c_boolean_false_node)
          {
            warning ("comparison is always 0 due to limited range of data type");
          }
          if (val == c_boolean_true_node)
          {
            warning ("comparison is always 1 due to limited range of data type");
          }
        }

      if (val != 0)
        {
          /* Don't forget to evaluate PRIMOP0 if it has side effects.  */
          if ( CIsTreeNodeSideEffect(primop0))
            return c_tree_build_binary_typed_expr(
                C_COMPOUND_EXPR, CTreeType(val), primop0, val);
          return val;
        }

      /* Value is not predetermined, but do the comparison
         in the type of the operand that is not constant.
         TYPE is already properly set.  */
    }
  else if (real1 && real2
           && (CTreeTypePrecision(CTreeType(primop0))
               == CTreeTypePrecision(CTreeType(primop1))))
    type = CTreeType(primop0);

  /* If args' natural types are both narrower than nominal type
     and both extend in the same manner, compare them
     in the type of the wider arg.
     Otherwise must actually extend both to the nominal
     common type lest different ways of extending
     alter the result.
     (eg, (short)-1 == (unsigned short)-1  should be 0.)  */

  else if (unsignedp0 == unsignedp1 && real1 == real2
           && CTreeTypePrecision(CTreeType(primop0)) < CTreeTypePrecision(*restype_ptr)
           && CTreeTypePrecision(CTreeType(primop1)) < CTreeTypePrecision(*restype_ptr))
    {
      type = c_get_common_type(CTreeType(primop0), CTreeType(primop1));
      type = c_get_signed_or_unsigned_type(unsignedp0
                                      || CIsTreeNodeUnsigned(*restype_ptr),
                                      type);
      /* Make sure shorter operand is extended the right way
         to match the longer operand.  */
      primop0 = c_convert_expr(
          c_get_signed_or_unsigned_type(unsignedp0, CTreeType(primop0)), primop0);
      primop1 = c_convert_expr(
          c_get_signed_or_unsigned_type(unsignedp1, CTreeType(primop1)), primop1);
    }
  else
    {
      /* Here we must do the comparison on the nominal type
         using the args exactly as we received them.  */
      type = *restype_ptr;
      primop0 = op0;
      primop1 = op1;

      if (!real1 && !real2 && c_tree_is_integer_zerop(primop1)
          && CIsTreeNodeUnsigned(*restype_ptr))
        {
          c_tree_node *value = 0;
          switch (code)
            {
            case C_GE_EXPR:
              /* All unsigned values are >= 0, so we warn if extra warnings
                 are requested.  However, if OP0 is a constant that is
                 >= 0, the signedness of the comparison isn't an issue,
                 so suppress the warning.  */
              if (c_extra_warnings
                  && ! ( CTreeNodeFirstCode(primop0) == C_INT_CST_NODE
                        && ! CIsTreeNodeOverflow(
                            c_convert_expr(c_get_signed_type(type), primop0))))
              {
                warning ("unsigned value >= 0 is always 1");
              }
              value = c_boolean_true_node;
              break;

            case C_LT_EXPR:
              if (c_extra_warnings
                  && ! ( CTreeNodeFirstCode(primop0) == C_INT_CST_NODE
                        && ! CIsTreeNodeOverflow(
                            c_convert_expr(c_get_signed_type(type), primop0))))
              {
                warning ("unsigned value < 0 is always 0");
              }
              value = c_boolean_false_node;
              break;

            default:
              break;
            }

          if (value != 0)
            {
              /* Don't forget to evaluate PRIMOP0 if it has side effects.  */
              if ( CIsTreeNodeSideEffect(primop0))
                return c_tree_build_binary_typed_expr(
                    C_COMPOUND_EXPR, CTreeType(value), primop0, value);
              return value;
            }
        }
    }

  *op0_ptr = c_convert_expr(type, primop0);
  *op1_ptr = c_convert_expr(type, primop1);

  *restype_ptr = c_boolean_type_node;

  return 0;
}
