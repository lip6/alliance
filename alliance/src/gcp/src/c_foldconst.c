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
| File    :                   c_foldconst.c                   |
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

# include "c_flags.h"
# include "c_tree.h"
# include "c_treecompat.h"
# include "c_cdecl.h"
# include "c_convert.h"
# include "c_ctypeck.h"
# include "c_foldconst.h"
# include "c_debug.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

#define c_overflow_sum_sign(a, b, sum) ((~((a) ^ (b)) & ((a) ^ (sum))) < 0)

#define LOWPART(x) \
  ((x) & (((unsigned C_HOST_WIDE_INT) 1 << (C_HOST_BITS_PER_WIDE_INT/2)) - 1))
#define HIGHPART(x) \
  ((unsigned C_HOST_WIDE_INT) (x) >> C_HOST_BITS_PER_WIDE_INT/2)
#define BASE ((unsigned C_HOST_WIDE_INT) 1 << C_HOST_BITS_PER_WIDE_INT/2)


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

/* Nonzero means lvalues are limited to those valid in pedantic ANSI C.
   Zero means allow extended lvalues.  */

  int c_pedantic_lvalues;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/


  extern void c_rshift_double();
  extern void c_lshift_double();
  extern void c_lrotate_double();
  extern void c_rrotate_double();


/* When pedantic, return an expr equal to X but certainly not valid as a
   pedantic lvalue.  Otherwise, return X.  */

c_tree_node *c_get_pedantic_non_lvalue(x)
     c_tree_node *x;
{
  if (c_pedantic_lvalues)
    return c_get_non_lvalue(x);
  else
    return x;
}

/* Given a c_tree_node *comparison code, return the code that is the logical inverse
   of the given code.  It is not safe to do this for floating-point
   comparisons, except for NE_EXPR and EQ_EXPR.  */

static int c_invert_tree_comparison(code)
     int code;
{
  switch (code)
    {
    case C_EQ_EXPR:
      return C_NE_EXPR;
    case C_NE_EXPR:
      return C_EQ_EXPR;
    case C_GT_EXPR:
      return C_LE_EXPR;
    case C_GE_EXPR:
      return C_LT_EXPR;
    case C_LT_EXPR:
      return C_GE_EXPR;
    case C_LE_EXPR:
      return C_GT_EXPR;
    default:
      abort ();
    }
}

/* Return a simplified c_tree_node *node for the truth-negation of ARG.  This
   never alters ARG itself.  We assume that ARG is an operation that
   returns a truth value (0 or 1).  */

c_tree_node *c_invert_truthvalue(arg)
     c_tree_node *arg;
{
  c_tree_node *type = CTreeType(arg);
  int  code = CTreeNodeFirstCode(arg);

  if (code == C_ERROR_NODE)
    return arg;

  code = CTreeExprCode(arg);

  /* If this is a comparison, we can simply invert it, except for
     floating-point non-equality comparisons, in which case we just
     enclose a TRUTH_NOT_EXPR around what we have.  */

  if ( code == C_LT_EXPR ||
       code == C_LE_EXPR ||
       code == C_GT_EXPR ||
       code == C_GE_EXPR ||
       code == C_EQ_EXPR ||
       code == C_NE_EXPR ||
       code == C_SET_LE_EXPR )
    {
      if (FLOAT_TYPE_P (CTreeType(CTreeExprOperand(arg, 0)))
          && code != C_NE_EXPR && code != C_EQ_EXPR)
        return c_tree_build_unary_typed_expr(C_TRUTH_NOT_EXPR, type, arg);
      else
        return c_tree_build_binary_typed_expr(c_invert_tree_comparison(code), type,
                      CTreeExprOperand(arg, 0), CTreeExprOperand(arg, 1));
    }

  switch (code)
    {
    case C_INT_CST_NODE:
      return c_convert_expr(type, c_tree_build_int_2(CTreeIntCstLow(arg) == 0
                                         && CTreeIntCstHigh(arg) == 0, 0));

    case C_TRUTH_AND_EXPR:
      return c_tree_build_binary_typed_expr(C_TRUTH_OR_EXPR, type,
                    c_invert_truthvalue (CTreeExprOperand(arg, 0)),
                    c_invert_truthvalue (CTreeExprOperand(arg, 1)));

    case C_TRUTH_OR_EXPR:
      return c_tree_build_binary_typed_expr(C_TRUTH_AND_EXPR, type,
                    c_invert_truthvalue (CTreeExprOperand(arg, 0)),
                    c_invert_truthvalue (CTreeExprOperand(arg, 1)));

    case C_TRUTH_XOR_EXPR:
      /* Here we can invert either operand.  We invert the first operand
         unless the second operand is a TRUTH_NOT_EXPR in which case our
         result is the XOR of the first operand with the inside of the
         negation of the second operand.  */

      if ( CTreeExprCode(CTreeExprOperand(arg, 1)) == C_TRUTH_NOT_EXPR)
        return c_tree_build_binary_typed_expr(C_TRUTH_XOR_EXPR, type, CTreeExprOperand(arg, 0),
                      CTreeExprOperand(CTreeExprOperand(arg, 1), 0));
      else
        return c_tree_build_binary_typed_expr (C_TRUTH_XOR_EXPR, type,
                      c_invert_truthvalue (CTreeExprOperand(arg, 0)),
                      CTreeExprOperand(arg, 1));

    case C_TRUTH_ANDIF_EXPR:
      return c_tree_build_binary_typed_expr (C_TRUTH_ORIF_EXPR, type,
                    c_invert_truthvalue (CTreeExprOperand(arg, 0)),
                    c_invert_truthvalue (CTreeExprOperand(arg, 1)));

    case C_TRUTH_ORIF_EXPR:
      return c_tree_build_binary_typed_expr (C_TRUTH_ANDIF_EXPR, type,
                    c_invert_truthvalue (CTreeExprOperand(arg, 0)),
                    c_invert_truthvalue (CTreeExprOperand(arg, 1)));

    case C_TRUTH_NOT_EXPR:
      return CTreeExprOperand(arg, 0);

    case C_COND_EXPR:
      return c_tree_build_ternary_typed_expr (C_COND_EXPR, type, CTreeExprOperand(arg, 0),
                    c_invert_truthvalue (CTreeExprOperand(arg, 1)),
                    c_invert_truthvalue (CTreeExprOperand(arg, 2)));

    case C_COMPOUND_EXPR:
      return c_tree_build_binary_typed_expr (C_COMPOUND_EXPR, type, CTreeExprOperand(arg, 0),
                    c_invert_truthvalue (CTreeExprOperand(arg, 1)));

    case C_NON_LVALUE_EXPR:
      return c_invert_truthvalue (CTreeExprOperand(arg, 0));

    case C_NOP_EXPR:
    case C_CONVERT_EXPR:
    case C_FLOAT_EXPR:
      return c_tree_build_unary_typed_expr ( CTreeExprCode(arg), type,
                     c_invert_truthvalue (CTreeExprOperand(arg, 0)));

    case C_BIT_AND_EXPR:
      if (! c_tree_is_integer_onep(CTreeExprOperand(arg, 1)))
        break;
      return c_tree_build_binary_typed_expr(
          C_EQ_EXPR, type, arg, c_convert_expr(type, c_integer_zero_node));

    case C_SAVE_EXPR:
      return c_tree_build_unary_typed_expr(C_TRUTH_NOT_EXPR, type, arg);

    case C_CLEANUP_POINT_EXPR:
      return c_tree_build_unary_typed_expr(C_CLEANUP_POINT_EXPR, type,
                     c_invert_truthvalue(CTreeExprOperand(arg, 0)));

    default:
      break;
    }
  if ( CTreeTypeCode(CTreeType(arg)) != C_BOOLEAN_TYPE)
    abort ();
  return c_tree_build_unary_typed_expr(C_TRUTH_NOT_EXPR, type, arg);
}


/* Return an expr equal to X but certainly not valid as an lvalue.
   Also make sure it is not valid as an null pointer constant.  */

c_tree_node *c_non_lvalue(x)
     c_tree_node *x;
{
  c_tree_node *result;
  int code = CTreeNodeFirstCode(x);

  /* These things are certainly not lvalues.  */
  if ( ( code == C_EXPR_NODE && 
      (CTreeExprCode(x) == C_NON_LVALUE_EXPR || CTreeExprCode(x) == C_ADDR_EXPR))
      || code == C_INT_CST_NODE
      || code == C_REAL_CST_NODE
      || code == C_STRING_NODE )
    {
      if ( code == C_INT_CST_NODE && c_tree_is_integer_zerop(x))
        {
          /* Use NOP_EXPR instead of NON_LVALUE_EXPR
             so convert_for_assignment won't strip it.
             This is so this 0 won't be treated as a null pointer constant.  */
          result = c_tree_build_unary_typed_expr( C_NOP_EXPR, CTreeType(x), x);
          if ( CIsTreeNodeConstant(x) ) CSetTreeNodeConstant( result );
          return result;
        }
      return x;
    }

  result = c_tree_build_unary_typed_expr( C_NON_LVALUE_EXPR, CTreeType(x), x);
  if ( CIsTreeNodeConstant(x) ) CSetTreeNodeConstant( result );

  return result;
}


/* Similar, but return the comparison that results if the operands are
   swapped.  This is safe for floating-point.  */

static int c_swap_tree_comparison(code)
     int code;
{
  switch (code)
    {
    case C_EQ_EXPR:
    case C_NE_EXPR:
      return code;
    case C_GT_EXPR:
      return C_LT_EXPR;
    case C_GE_EXPR:
      return C_LE_EXPR;
    case C_LT_EXPR:
      return C_GT_EXPR;
    case C_LE_EXPR:
      return C_GE_EXPR;
    default:
      abort ();
    }
}

/* Return nonzero if CODE is a c_tree_node *code that represents a truth value.  */

static int
c_truth_value_p (code)
     int code;
{
  return ( code == C_LT_EXPR ||
           code == C_LE_EXPR ||
           code == C_GT_EXPR ||
           code == C_GE_EXPR ||
           code == C_EQ_EXPR ||
           code == C_NE_EXPR ||
           code == C_SET_LE_EXPR ||
           code == C_TRUTH_AND_EXPR ||
           code == C_TRUTH_ANDIF_EXPR || 
           code == C_TRUTH_OR_EXPR ||
           code == C_TRUTH_ORIF_EXPR ||
           code == C_TRUTH_XOR_EXPR ||
           code == C_TRUTH_NOT_EXPR);

}


/* Given T, a c_tree_node *representing type conversion of ARG1, a constant,
   return a constant c_tree_node *representing the result of conversion.  */

static c_tree_node *c_fold_convert_expr(t, arg1)
     c_tree_node *t;
     c_tree_node *arg1;
{
  c_tree_node *type = CTreeType(t);
# ifdef LUDO_NO_SKIP
  int overflow = 0;
# endif

  if (POINTER_TYPE_P (type) || INTEGRAL_TYPE_P (type))
    {
      if ( CTreeNodeCode(arg1) == C_INT_CST_NODE)
        {
          /* If we would build a constant wider than GCC supports,
             leave the conversion unfolded.  */
          if (CTreeTypePrecision(type) > 2 * C_HOST_BITS_PER_WIDE_INT)
            return t;

          /* Given an integer constant, make new constant with new type,
             appropriately sign-extended or truncated.  */
          t = c_tree_build_int_2(CTreeIntCstLow(arg1), CTreeIntCstHigh(arg1));
          CTreeType(t) = type;
          /* Indicate an overflow if (1) ARG1 already overflowed,
             or (2) force_fit_type indicates an overflow.
             Tell force_fit_type that an overflow has already occurred
             if ARG1 is a too-large unsigned value and T is signed.
             But don't indicate an overflow if converting a pointer.  */
          if ((c_force_fit_type(t, (CTreeIntCstHigh(arg1) < 0
                                 & (CIsTreeNodeUnsigned(type)
                                    < CIsTreeNodeUnsigned(CTreeType(arg1)))))
                && ! POINTER_TYPE_P (CTreeType(arg1)))
               || CIsTreeNodeOverflow(arg1))
           CSetTreeNodeOverflow(t);


          if ( CIsTreeNodeOverflow(t) || CIsTreeNodeCstOverflow(arg1))
            CSetTreeNodeCstOverflow(t);
        }
# ifdef LUDO_NO_SKIP
#if !defined (REAL_IS_NOT_DOUBLE) || defined (REAL_ARITHMETIC)
      else if (CTreeNodeCode(arg1) == REAL_CST)
        {
          /* Don't initialize these, use assignments.
             Initialized local aggregates don't work on old compilers.  */
          C_REAL_VALUE_TYPE x;
          C_REAL_VALUE_TYPE l;
          C_REAL_VALUE_TYPE u;
          c_tree_node *type1 = CTreeType(arg1);

          x = TREE_REAL_CST (arg1);
          l = c_real_value_from_int_cst(type1, TYPE_MIN_VALUE (type));
          u = c_real_value_from_int_cst(type1, TYPE_MAX_VALUE (type));
          /* See if X will be in range after truncation towards 0.
             To compensate for truncation, move the bounds away from 0,
             but reject if X exactly equals the adjusted bounds.  */
#ifdef REAL_ARITHMETIC
          REAL_ARITHMETIC (l, MINUS_EXPR, l, dconst1);
          REAL_ARITHMETIC (u, PLUS_EXPR, u, dconst1);
#else
          l--;
          u++;
#endif
          /* If X is a NaN, use zero instead and show we have an overflow.
             Otherwise, range check.  */
          if (REAL_VALUE_ISNAN (x))
            overflow = 1, x = dconst0;
          else if (! (REAL_VALUES_LESS (l, x) && REAL_VALUES_LESS (x, u)))
            overflow = 1;

#ifndef REAL_ARITHMETIC
          {
            C_HOST_WIDE_INT low, high;
            C_HOST_WIDE_INT half_word
              = (HOST_WIDE_INT) 1 << (HOST_BITS_PER_WIDE_INT / 2);

            if (x < 0)
              x = -x;

            high = (HOST_WIDE_INT) (x / half_word / half_word);
            x -= (REAL_VALUE_TYPE) high * half_word * half_word;
            if (x >= (REAL_VALUE_TYPE) half_word * half_word / 2)
              {
                low = x - (REAL_VALUE_TYPE) half_word * half_word / 2;
                low |= (HOST_WIDE_INT) -1 << (HOST_BITS_PER_WIDE_INT - 1);
              }
            else
              low = (HOST_WIDE_INT) x;
            if (TREE_REAL_CST (arg1) < 0)
              neg_double (low, high, &low, &high);
            t = build_int_2 (low, high);
          }
#else
          {
            C_HOST_WIDE_INT low, high;
            REAL_VALUE_TO_INT (&low, &high, x);
            t = build_int_2 (low, high);
          }
#endif
          CTreeType(t) = type;
          TREE_OVERFLOW (t)
            = TREE_OVERFLOW (arg1) | force_fit_type (t, overflow);
          TREE_CONSTANT_OVERFLOW (t)
            = TREE_OVERFLOW (t) | TREE_CONSTANT_OVERFLOW (arg1);
        }
#endif /* not REAL_IS_NOT_DOUBLE, or REAL_ARITHMETIC */
# endif
      CTreeType(t) = type;
    }
  else if ( CTreeNodeCode(type) == C_REAL_TYPE)
    {
# ifdef LUDO_NO_SKIP
#if !defined (REAL_IS_NOT_DOUBLE) || defined (REAL_ARITHMETIC)
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE)
        return c_build_real_from_int_cst(type, arg1);
#endif /* not REAL_IS_NOT_DOUBLE, or REAL_ARITHMETIC */
# endif
      if (CTreeNodeCode(arg1) == C_REAL_CST_NODE)
        {
# ifdef LUDO_NO_SKIP
          if (REAL_VALUE_ISNAN (TREE_REAL_CST (arg1)))
            {
              t = arg1;
              CTreeType(arg1) = type;
              return t;
            }
          else if (setjmp (float_error))
            {
              overflow = 1;
              t = copy_node (arg1);
              goto got_it;
            }
          set_float_handler (float_error);

          t = c_tree_build_real(type, c_real_value_truncate(CTreeTypeMode(type),
                                                     CTreeRealCstValue(arg1)));
          set_float_handler (NULL_PTR);

        got_it:
          TREE_OVERFLOW (t)
            = TREE_OVERFLOW (arg1) | force_fit_type (t, overflow);
          TREE_CONSTANT_OVERFLOW (t)
            = TREE_OVERFLOW (t) | TREE_CONSTANT_OVERFLOW (arg1);
          return t;
# endif
        }
    }

  CSetTreeNodeConstant(t);
  return t;
}


/* If T contains a COMPOUND_EXPR which was inserted merely to evaluate
   S, a SAVE_EXPR, return the expression actually being evaluated.   Note
   that we may sometimes modify the tree.  */

static c_tree_node *c_strip_compound_expr(t, s)
     c_tree_node *t;
     c_tree_node *s;
{
# ifdef LUDO_NO_SKIP
  c_tree_node *type = CTreeType(t);
# endif
  int code = CTreeNodeCode(t);

  /* See if this is the COMPOUND_EXPR we want to eliminate.  */
  if (code == C_COMPOUND_EXPR && CTreeNodeCode(CTreeExprOperand(t, 0)) == C_CONVERT_EXPR
      && CTreeExprOperand(CTreeExprOperand(t, 0), 0) == s)
    return CTreeExprOperand(t, 1);

  /* See if this is a COND_EXPR or a simple arithmetic operator.   We
     don't bother handling any other types.  */
  else if (code == C_COND_EXPR)
    {
      CTreeExprOperand(t, 0) = c_strip_compound_expr(CTreeExprOperand(t, 0), s);
      CTreeExprOperand(t, 1) = c_strip_compound_expr(CTreeExprOperand(t, 1), s);
      CTreeExprOperand(t, 2) = c_strip_compound_expr(CTreeExprOperand(t, 2), s);
    }
  else if ( c_tree_get_code_class(code) == '1')
    CTreeExprOperand(t, 0) = c_strip_compound_expr(CTreeExprOperand(t, 0), s);
  else if (c_tree_get_code_class(code) == '<'
           || c_tree_get_code_class(code) == '2')
    {
      CTreeExprOperand(t, 0) = c_strip_compound_expr(CTreeExprOperand(t, 0), s);
      CTreeExprOperand(t, 1) = c_strip_compound_expr(CTreeExprOperand(t, 1), s);
    }

  return t;
}


/* Split a c_tree_node *IN into a constant and a variable part
   that could be combined with CODE to make IN.
   CODE must be a commutative arithmetic operation.
   Store the constant part into *CONP and the variable in &VARP.
   Return 1 if this was done; zero means the c_tree_node *IN did not decompose
   this way.

   If CODE is PLUS_EXPR we also split trees that use MINUS_EXPR.
   Therefore, we must tell the caller whether the variable part
   was subtracted.  We do this by storing 1 or -1 into *VARSIGNP.
   The value stored is the coefficient for the variable term.
   The constant term we return should always be added;
   we negate it if necessary.  */

static int c_split_tree (in, code, varp, conp, varsignp)
     c_tree_node *in;
     int  code;
     c_tree_node **varp;
     c_tree_node **conp;
     int *varsignp;
{
  c_tree_node *outtype = CTreeType(in);
  *varp = 0;
  *conp = 0;

  /* Strip any conversions that don't change the machine mode.  */
  while ((CTreeNodeCode(in) == C_NOP_EXPR
          || CTreeNodeCode(in) == C_CONVERT_EXPR)
         && (CTreeTypeMode(CTreeType(in))
             == CTreeTypeMode(CTreeType(CTreeExprOperand(in, 0)))))
    in = CTreeExprOperand(in, 0);

  if (CTreeNodeCode(in) == code
      || (! FLOAT_TYPE_P (CTreeType(in))
          /* We can associate addition and subtraction together
             (even though the C standard doesn't say so)
             for integers because the value is not affected.
             For reals, the value might be affected, so we can't.  */
          && ((code == C_PLUS_EXPR && CTreeNodeCode(in) == C_MINUS_EXPR)
              || (code == C_MINUS_EXPR && CTreeNodeCode(in) == C_PLUS_EXPR))))
    {
      int code = CTreeNodeCode(CTreeExprOperand(in, 0));
      if (code == C_INT_CST_NODE)
        {
          *conp = CTreeExprOperand(in, 0);
          *varp = CTreeExprOperand(in, 1);
          if (CTreeTypeMode(CTreeType(*varp)) != CTreeTypeMode(outtype)
              && CTreeType(*varp) != outtype)
            *varp = c_convert_expr(outtype, *varp);
          *varsignp = (CTreeNodeCode(in) == C_MINUS_EXPR) ? -1 : 1;
          return 1;
        }
      if ( CIsTreeNodeConstant(CTreeExprOperand(in, 1)))
        {
          *conp = CTreeExprOperand(in, 1);
          *varp = CTreeExprOperand(in, 0);
          *varsignp = 1;
          if (CTreeTypeMode(CTreeType(*varp)) != CTreeTypeMode(outtype)
              && CTreeType(*varp) != outtype)
            *varp = c_convert_expr(outtype, *varp);
          if (CTreeNodeCode(in) == C_MINUS_EXPR)
            {
              /* If operation is subtraction and constant is second,
                 must negate it to get an additive constant.
                 And this cannot be done unless it is a manifest constant.
                 It could also be the address of a static variable.
                 We cannot negate that, so give up.  */
              if (CTreeNodeCode(*conp) == C_INT_CST_NODE)
                /* Subtracting from integer_zero_node loses for long long.  */
                *conp = c_fold ( 
                    c_tree_build_unary_typed_expr(C_NEGATE_EXPR, CTreeType(*conp), *conp));
              else
                return 0;
            }
          return 1;
        }
      if ( CIsTreeNodeConstant(CTreeExprOperand(in, 0)))
        {
          *conp = CTreeExprOperand(in, 0);
          *varp = CTreeExprOperand(in, 1);
          if (CTreeTypeMode(CTreeType(*varp)) != CTreeTypeMode(outtype)
              && CTreeType(*varp) != outtype)
            *varp = c_convert_expr(outtype, *varp);
          *varsignp = (CTreeNodeCode(in) == C_MINUS_EXPR) ? -1 : 1;
          return 1;
        }
    }
  return 0;
}

/* Unpack a two-word integer into 4 words.
   LOW and HI are the integer, as two `HOST_WIDE_INT' pieces.
   WORDS points to the array of HOST_WIDE_INTs.  */

static void c_encode(words, low, hi)
     C_HOST_WIDE_INT *words;
     C_HOST_WIDE_INT low, hi;
{
  words[0] = LOWPART (low);
  words[1] = HIGHPART (low);
  words[2] = LOWPART (hi);
  words[3] = HIGHPART (hi);
}

/* Pack an array of 4 words into a two-word integer.
   WORDS points to the array of words.
   The integer is stored into *LOW and *HI as two `HOST_WIDE_INT' pieces.  */

static void
c_decode (words, low, hi)
     C_HOST_WIDE_INT *words;
     C_HOST_WIDE_INT *low, *hi;
{
  *low = words[0] | words[1] * BASE;
  *hi = words[2] | words[3] * BASE;
}




/* Negate a doubleword integer with doubleword result.
   Return nonzero if the operation overflows, assuming it's signed.
   The argument is given as two `HOST_WIDE_INT' pieces in L1 and H1.
   The value is stored as two `HOST_WIDE_INT' pieces in *LV and *HV.  */

int c_neg_double (l1, h1, lv, hv)
     C_HOST_WIDE_INT l1, h1;
     C_HOST_WIDE_INT *lv, *hv;
{
  if (l1 == 0)
    {
      *lv = 0;
      *hv = - h1;
      return (*hv & h1) < 0;
    }
  else
    {
      *lv = - l1;
      *hv = ~ h1;
      return 0;
    }
}


/* Divide doubleword integer LNUM, HNUM by doubleword integer LDEN, HDEN
   for a quotient (stored in *LQUO, *HQUO) and remainder (in *LREM, *HREM).
   CODE is a c_tree_node *code for a kind of division, one of
   TRUNC_DIV_EXPR, FLOOR_DIV_EXPR, CEIL_DIV_EXPR, ROUND_DIV_EXPR
   or EXACT_DIV_EXPR
   It controls how the quotient is rounded to a integer.
   Return nonzero if the operation overflows.
   UNS nonzero says do unsigned division.  */

extern int c_mul_double();
extern int c_neg_double();
extern int c_add_double();

int
c_div_and_round_double (code, uns,
                      lnum_orig, hnum_orig, lden_orig, hden_orig,
                      lquo, hquo, lrem, hrem)
     int code;
     int uns;
     C_HOST_WIDE_INT lnum_orig, hnum_orig; /* num == numerator == dividend */
     C_HOST_WIDE_INT lden_orig, hden_orig; /* den == denominator == divisor */
     C_HOST_WIDE_INT *lquo, *hquo, *lrem, *hrem;
{
  int quo_neg = 0;
  C_HOST_WIDE_INT num[4 + 1];        /* extra element for scaling.  */
  C_HOST_WIDE_INT den[4], quo[4];
  int i, j;
  unsigned C_HOST_WIDE_INT work;
  unsigned C_HOST_WIDE_INT carry = 0;
  C_HOST_WIDE_INT lnum = lnum_orig;
  C_HOST_WIDE_INT hnum = hnum_orig;
  C_HOST_WIDE_INT lden = lden_orig;
  C_HOST_WIDE_INT hden = hden_orig;
  int overflow = 0;

  if ((hden == 0) && (lden == 0))
    overflow = 1, lden = 1;

  /* calculate quotient sign and convert operands to unsigned.  */
  if (!uns) 
    {
      if (hnum < 0)
        {
          quo_neg = ~ quo_neg;
          /* (minimum integer) / (-1) is the only overflow case.  */
          if (c_neg_double(lnum, hnum, &lnum, &hnum) && (lden & hden) == -1)
            overflow = 1;
        }
      if (hden < 0) 
        {
          quo_neg = ~ quo_neg;
          c_neg_double (lden, hden, &lden, &hden);
        }
    }

  if (hnum == 0 && hden == 0)
    {                                /* single precision */
      *hquo = *hrem = 0;
      /* This unsigned division rounds toward zero.  */
      *lquo = lnum / (unsigned C_HOST_WIDE_INT) lden;
      goto finish_up;
    }

  if (hnum == 0)
    {                                /* trivial case: dividend < divisor */
      /* hden != 0 already checked.  */
      *hquo = *lquo = 0;
      *hrem = hnum;
      *lrem = lnum;
      goto finish_up;
    }

  bzero ((char *) quo, sizeof quo);

  bzero ((char *) num, sizeof num);        /* to zero 9th element */
  bzero ((char *) den, sizeof den);

  c_encode (num, lnum, hnum); 
  c_encode (den, lden, hden);

  /* Special code for when the divisor < BASE.  */
  if (hden == 0 && lden < BASE)
    {
      /* hnum != 0 already checked.  */
      for (i = 4 - 1; i >= 0; i--)
        {
          work = num[i] + carry * BASE;
          quo[i] = work / (unsigned C_HOST_WIDE_INT) lden;
          carry = work % (unsigned C_HOST_WIDE_INT) lden;
        }
    }
  else
    {
      /* Full double precision division,
         with thanks to Don Knuth's "Seminumerical Algorithms".  */
    int num_hi_sig, den_hi_sig;
    unsigned C_HOST_WIDE_INT quo_est, scale;

    /* Find the highest non-zero divisor digit.  */
    for (i = 4 - 1; ; i--)
      if (den[i] != 0) {
        den_hi_sig = i;
        break;
      }

    /* Insure that the first digit of the divisor is at least BASE/2.
       This is required by the quotient digit estimation algorithm.  */

    scale = BASE / (den[den_hi_sig] + 1);
    if (scale > 1) {                /* scale divisor and dividend */
      carry = 0;
      for (i = 0; i <= 4 - 1; i++) {
        work = (num[i] * scale) + carry;
        num[i] = LOWPART (work);
        carry = HIGHPART (work);
      } num[4] = carry;
      carry = 0;
      for (i = 0; i <= 4 - 1; i++) {
        work = (den[i] * scale) + carry;
        den[i] = LOWPART (work);
        carry = HIGHPART (work);
        if (den[i] != 0) den_hi_sig = i;
      }
    }

    num_hi_sig = 4;

    /* Main loop */
    for (i = num_hi_sig - den_hi_sig - 1; i >= 0; i--) {
      /* guess the next quotient digit, quo_est, by dividing the first
         two remaining dividend digits by the high order quotient digit.
         quo_est is never low and is at most 2 high.  */
      unsigned C_HOST_WIDE_INT tmp;

      num_hi_sig = i + den_hi_sig + 1;
      work = num[num_hi_sig] * BASE + num[num_hi_sig - 1];
      if (num[num_hi_sig] != den[den_hi_sig])
        quo_est = work / den[den_hi_sig];
      else
        quo_est = BASE - 1;

      /* refine quo_est so it's usually correct, and at most one high.   */
      tmp = work - quo_est * den[den_hi_sig];
      if (tmp < BASE
          && den[den_hi_sig - 1] * quo_est > (tmp * BASE + num[num_hi_sig - 2]))
        quo_est--;

      /* Try QUO_EST as the quotient digit, by multiplying the
         divisor by QUO_EST and subtracting from the remaining dividend.
         Keep in mind that QUO_EST is the I - 1st digit.  */

      carry = 0;
      for (j = 0; j <= den_hi_sig; j++)
        {
          work = quo_est * den[j] + carry;
          carry = HIGHPART (work);
          work = num[i + j] - LOWPART (work);
          num[i + j] = LOWPART (work);
          carry += HIGHPART (work) != 0;
        }

      /* if quo_est was high by one, then num[i] went negative and
         we need to correct things.  */

      if (num[num_hi_sig] < carry)
        {
          quo_est--;
          carry = 0;                /* add divisor back in */
          for (j = 0; j <= den_hi_sig; j++)
            {
              work = num[i + j] + den[j] + carry;
              carry = HIGHPART (work);
              num[i + j] = LOWPART (work);
            }
          num [num_hi_sig] += carry;
        }

      /* store the quotient digit.  */
      quo[i] = quo_est;
    }
  }

  c_decode (quo, lquo, hquo);

 finish_up:
  /* if result is negative, make it so.  */
  if (quo_neg)
    c_neg_double (*lquo, *hquo, lquo, hquo);

  /* compute trial remainder:  rem = num - (quo * den)  */
  c_mul_double (*lquo, *hquo, lden_orig, hden_orig, lrem, hrem);
  c_neg_double (*lrem, *hrem, lrem, hrem);
  c_add_double (lnum_orig, hnum_orig, *lrem, *hrem, lrem, hrem);

  switch (code)
    {
    case C_TRUNC_DIV_EXPR:
    case C_TRUNC_MOD_EXPR:        /* round toward zero */
    case C_EXACT_DIV_EXPR:        /* for this one, it shouldn't matter */
      return overflow;

    case C_FLOOR_DIV_EXPR:
    case C_FLOOR_MOD_EXPR:        /* round toward negative infinity */
      if (quo_neg && (*lrem != 0 || *hrem != 0))   /* ratio < 0 && rem != 0 */
        {
          /* quo = quo - 1;  */
          c_add_double(*lquo, *hquo, (C_HOST_WIDE_INT) -1, (C_HOST_WIDE_INT)  -1,
                      lquo, hquo);
        }
      else return overflow;
      break;

    case C_CEIL_DIV_EXPR:
    case C_CEIL_MOD_EXPR:                /* round toward positive infinity */
      if (!quo_neg && (*lrem != 0 || *hrem != 0))  /* ratio > 0 && rem != 0 */
        {
          c_add_double (*lquo, *hquo, (C_HOST_WIDE_INT) 1, (C_HOST_WIDE_INT) 0,
                      lquo, hquo);
        }
      else return overflow;
      break;
    
    case C_ROUND_DIV_EXPR:
    case C_ROUND_MOD_EXPR:        /* round to closest integer */
      {
        C_HOST_WIDE_INT labs_rem = *lrem, habs_rem = *hrem;
        C_HOST_WIDE_INT labs_den = lden, habs_den = hden, ltwice, htwice;

        /* get absolute values */
        if (*hrem < 0) c_neg_double (*lrem, *hrem, &labs_rem, &habs_rem);
        if (hden < 0) c_neg_double (lden, hden, &labs_den, &habs_den);

        /* if (2 * abs (lrem) >= abs (lden)) */
        c_mul_double ((C_HOST_WIDE_INT) 2, (C_HOST_WIDE_INT) 0,
                    labs_rem, habs_rem, &ltwice, &htwice);
        if (((unsigned C_HOST_WIDE_INT) habs_den
             < (unsigned C_HOST_WIDE_INT) htwice)
            || (((unsigned C_HOST_WIDE_INT) habs_den
                 == (unsigned C_HOST_WIDE_INT) htwice)
                && ((C_HOST_WIDE_INT unsigned) labs_den
                    < (unsigned C_HOST_WIDE_INT) ltwice)))
          {
            if (*hquo < 0)
              /* quo = quo - 1;  */
              c_add_double (*lquo, *hquo,
                          (C_HOST_WIDE_INT) -1, (C_HOST_WIDE_INT) -1, lquo, hquo);
            else
              /* quo = quo + 1; */
              c_add_double (*lquo, *hquo, (C_HOST_WIDE_INT) 1, (C_HOST_WIDE_INT) 0,
                          lquo, hquo);
          }
        else return overflow;
      }
      break;

    default:
      abort ();
    }

  /* compute true remainder:  rem = num - (quo * den)  */
  c_mul_double (*lquo, *hquo, lden_orig, hden_orig, lrem, hrem);
  c_neg_double (*lrem, *hrem, lrem, hrem);
  c_add_double (lnum_orig, hnum_orig, *lrem, *hrem, lrem, hrem);
  return overflow;
}


/* Return nonzero if two operands are necessarily equal.
   If ONLY_CONST is non-zero, only return non-zero for constants.
   This function tests whether the operands are indistinguishable;
   it does not test whether they are equal using C's == operation.
   The distinction is important for IEEE floating point, because
   (1) -0.0 and 0.0 are distinguishable, but -0.0==0.0, and
   (2) two NaNs may be indistinguishable, but NaN!=NaN.  */

int c_is_operand_equal_p(arg0, arg1, only_const)
     c_tree_node *arg0;
     c_tree_node *arg1;
     int only_const;
{
  /* If both types don't have the same signedness, then we can't consider
     them equal.  We must check this before the STRIP_NOPS calls
     because they may change the signedness of the arguments.  */
  if (CIsTreeNodeUnsigned(CTreeType(arg0)) != CIsTreeNodeUnsigned(CTreeType(arg1)))
    return 0;

  CStripNops(arg0);
  CStripNops(arg1);

  if ( CTreeNodeCode(arg0) != CTreeNodeCode(arg1)
      /* This is needed for conversions and for COMPONENT_REF.
         Might as well play it safe and always test this.  */
      || CTreeTypeMode(CTreeType(arg0)) != CTreeTypeMode(CTreeType(arg1)))
    return 0;

  /* If ARG0 and ARG1 are the same SAVE_EXPR, they are necessarily equal.
     We don't care about side effects in that case because the SAVE_EXPR
     takes care of that for us. In all other cases, two expressions are
     equal if they have no side effects.  If we have two identical
     expressions with side effects that should be treated the same due
     to the only side effects being identical SAVE_EXPR's, that will
     be detected in the recursive calls below.  */
  if (arg0 == arg1 && ! only_const
      && (CTreeNodeCode(arg0) == C_SAVE_EXPR
          || (!  CIsTreeNodeSideEffect(arg0) && ! CIsTreeNodeSideEffect(arg1))))
    return 1;

  /* Next handle constant cases, those for which we can return 1 even
     if ONLY_CONST is set.  */
  if ( CIsTreeNodeConstant(arg0) &&  CIsTreeNodeConstant(arg1))
    switch (CTreeNodeCode(arg0))
      {
      case C_INT_CST_NODE:
        return (! CIsTreeNodeCstOverflow(arg0)
                && ! CIsTreeNodeCstOverflow(arg1)
                && CTreeIntCstLow(arg0) == CTreeIntCstLow(arg1)
                && CTreeIntCstHigh(arg0) == CTreeIntCstHigh(arg1));

      case C_REAL_CST_NODE:
        return (! CIsTreeNodeCstOverflow(arg0)
                && ! CIsTreeNodeCstOverflow(arg1)
                && REAL_VALUES_IDENTICAL ( CTreeRealCstValue(arg0),
                                           CTreeRealCstValue(arg1)));

      case C_COMPLEX_NODE:
        return (c_is_operand_equal_p( CTreeComplexReal(arg0), CTreeComplexReal(arg1),
                                 only_const)
                && c_is_operand_equal_p( CTreeComplexImag(arg0), CTreeComplexImag(arg1),
                                    only_const));

      case C_STRING_NODE:
        return ( CTreeStringLength(arg0) == CTreeStringLength(arg1)
                && ! strncmp ( CTreeStringPointer(arg0), CTreeStringPointer(arg1),
                               CTreeStringLength(arg0)));

      case C_ADDR_EXPR:
        return c_is_operand_equal_p(CTreeExprOperand(arg0, 0), CTreeExprOperand(arg1, 0),
                                0);
      default:
        break;
      }

  if (only_const)
    return 0;

  switch ( c_tree_get_code_class(CTreeNodeCode(arg0)))
    {
    case '1':
      /* Two conversions are equal only if signedness and modes match.  */
      if ((CTreeNodeCode(arg0) == C_NOP_EXPR || CTreeNodeCode(arg0) == C_CONVERT_EXPR)
          && (CIsTreeNodeUnsigned(CTreeType(arg0))
              != CIsTreeNodeUnsigned(CTreeType(arg1))))
        return 0;

      return c_is_operand_equal_p(CTreeExprOperand(arg0, 0),
                              CTreeExprOperand(arg1, 0), 0);

    case '<':
    case '2':
      if (c_is_operand_equal_p(CTreeExprOperand(arg0, 0), CTreeExprOperand(arg1, 0), 0)
          && c_is_operand_equal_p(CTreeExprOperand(arg0, 1), CTreeExprOperand(arg1, 1),
                              0))
        return 1;

      /* For commutative ops, allow the other order.  */
      return ((CTreeNodeCode(arg0) == C_PLUS_EXPR || CTreeNodeCode(arg0) == C_MULT_EXPR
               || CTreeNodeCode(arg0) == C_MIN_EXPR || CTreeNodeCode(arg0) == C_MAX_EXPR
               || CTreeNodeCode(arg0) == C_BIT_IOR_EXPR
               || CTreeNodeCode(arg0) == C_BIT_XOR_EXPR
               || CTreeNodeCode(arg0) == C_BIT_AND_EXPR
               || CTreeNodeCode(arg0) == C_NE_EXPR || CTreeNodeCode(arg0) == C_EQ_EXPR)
              && c_is_operand_equal_p(CTreeExprOperand(arg0, 0),
                                  CTreeExprOperand(arg1, 1), 0)
              && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                  CTreeExprOperand(arg1, 0), 0));

    case 'r':
      switch (CTreeNodeCode(arg0))
        {
        case C_INDIRECT_REF:
          return c_is_operand_equal_p(CTreeExprOperand(arg0, 0),
                                  CTreeExprOperand(arg1, 0), 0);

        case C_COMPONENT_REF:
        case C_ARRAY_REF:
          return (c_is_operand_equal_p(CTreeExprOperand(arg0, 0),
                                   CTreeExprOperand(arg1, 0), 0)
                  && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                      CTreeExprOperand(arg1, 1), 0));

        case C_BIT_FIELD_REF:
          return (c_is_operand_equal_p(CTreeExprOperand(arg0, 0),
                                   CTreeExprOperand(arg1, 0), 0)
                  && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                      CTreeExprOperand(arg1, 1), 0)
                  && c_is_operand_equal_p(CTreeExprOperand(arg0, 2),
                                      CTreeExprOperand(arg1, 2), 0));
        default:
          return 0;
        }
      
    default:
      return 0;
    }
}


/* Similar to operand_equal_p, but see if ARG0 might have been made by
   shorten_compare from ARG1 when ARG1 was being compared with OTHER. 

   When in doubt, return 0.  */

static int 
c_is_operand_equal_for_comparison_p(arg0, arg1, other)
     c_tree_node *arg0;
     c_tree_node *arg1;
     c_tree_node *other;
{
  int unsignedp1, unsignedpo;
  c_tree_node *primarg1;
  c_tree_node *primother;
  unsigned correct_width;

  if (c_is_operand_equal_p(arg0, arg1, 0))
    return 1;

  if (! INTEGRAL_TYPE_P (CTreeType(arg0))
      || ! INTEGRAL_TYPE_P (CTreeType(arg1)))
    return 0;

  /* Duplicate what shorten_compare does to ARG1 and see if that gives the
     actual comparison operand, ARG0.

     First throw away any conversions to wider types
     already present in the operands.  */

  primarg1 = c_get_narrower(arg1, &unsignedp1);
  primother = c_get_narrower(other, &unsignedpo);

  correct_width = CTreeTypePrecision(CTreeType(arg1));
  if (unsignedp1 == unsignedpo
      && CTreeTypePrecision(CTreeType(primarg1)) < correct_width
      && CTreeTypePrecision(CTreeType(primother)) < correct_width)
    {
      c_tree_node *type = CTreeType(arg0);

      /* Make sure shorter operand is extended the right way
         to match the longer operand.  */
      primarg1 = c_convert_expr(
          c_get_signed_or_unsigned_type(unsignedp1, CTreeType(primarg1)), primarg1);

      if (c_is_operand_equal_p(arg0, c_convert_expr(type, primarg1), 0))
        return 1;
    }

  return 0;
}


/* See if ARG is an expression that is either a comparison or is performing
   arithmetic on comparisons.  The comparisons must only be comparing
   two different values, which will be stored in *CVAL1 and *CVAL2; if
   they are non-zero it means that some operands have already been found.
   No variables may be used anywhere else in the expression except in the
   comparisons.  If SAVE_P is true it means we removed a SAVE_EXPR around
   the expression and save_expr needs to be called with CVAL1 and CVAL2.

   If this is true, return 1.  Otherwise, return zero.  */

static int c_twoval_comparison_p (arg, cval1, cval2, save_p)
     c_tree_node *arg;
     c_tree_node **cval1;
     c_tree_node **cval2;
     int *save_p;
{
  int  code = CTreeNodeCode(arg);
  char class = c_tree_get_code_class(code);

  /* We can handle some of the 'e' cases here.  */
  if (class == 'e' && code == C_TRUTH_NOT_EXPR)
    class = '1';
  else if (class == 'e'
           && (code == C_TRUTH_ANDIF_EXPR || code == C_TRUTH_ORIF_EXPR
               || code == C_COMPOUND_EXPR))
    class = '2';

  switch (class)
    {
    case '1':
      return c_twoval_comparison_p(CTreeExprOperand(arg, 0), cval1, cval2, save_p);

    case '2':
      return (c_twoval_comparison_p(CTreeExprOperand(arg, 0), cval1, cval2, save_p)
              && c_twoval_comparison_p(CTreeExprOperand(arg, 1),
                                      cval1, cval2, save_p));

    case 'c':
      return 1;

    case 'e':
      if (code == C_COND_EXPR)
        return (c_twoval_comparison_p(CTreeExprOperand(arg, 0),
                                     cval1, cval2, save_p)
                && c_twoval_comparison_p(CTreeExprOperand(arg, 1),
                                        cval1, cval2, save_p)
                && c_twoval_comparison_p(CTreeExprOperand(arg, 2),
                                        cval1, cval2, save_p));
      return 0;
          
    case '<':
      /* First see if we can handle the first operand, then the second.  For
         the second operand, we know *CVAL1 can't be zero.  It must be that
         one side of the comparison is each of the values; test for the
         case where this isn't true by failing if the two operands
         are the same.  */

      if (c_is_operand_equal_p(CTreeExprOperand(arg, 0),
                           CTreeExprOperand(arg, 1), 0))
        return 0;

      if (*cval1 == 0)
        *cval1 = CTreeExprOperand(arg, 0);
      else if (c_is_operand_equal_p(*cval1, CTreeExprOperand(arg, 0), 0))
        ;
      else if (*cval2 == 0)
        *cval2 = CTreeExprOperand(arg, 0);
      else if (c_is_operand_equal_p(*cval2, CTreeExprOperand(arg, 0), 0))
        ;
      else
        return 0;

      if ( c_is_operand_equal_p(*cval1, CTreeExprOperand(arg, 1), 0))
        ;
      else if (*cval2 == 0)
        *cval2 = CTreeExprOperand(arg, 1);
      else if (c_is_operand_equal_p(*cval2, CTreeExprOperand(arg, 1), 0))
        ;
      else
        return 0;

      return 1;

    default:
      return 0;
    }
}

/* Combine two integer constants ARG1 and ARG2 under operation CODE
   to produce a new constant.

   If NOTRUNC is nonzero, do not truncate the result to fit the data type.
   If FORSIZE is nonzero, compute overflow for unsigned types.  */

static c_tree_node *c_int_const_binop(code, arg1, arg2, notrunc, forsize)
     int  code;
     c_tree_node *arg1;
     c_tree_node *arg2;
     int notrunc, forsize;
{
  C_HOST_WIDE_INT int1l, int1h, int2l, int2h;
  C_HOST_WIDE_INT low, hi;
  C_HOST_WIDE_INT garbagel, garbageh;
  c_tree_node *t;
  int uns = CIsTreeNodeUnsigned(CTreeType(arg1));
  int overflow = 0;
  int no_overflow = 0;

  int1l = CTreeIntCstLow(arg1);
  int1h = CTreeIntCstHigh(arg1);
  int2l = CTreeIntCstLow(arg2);
  int2h = CTreeIntCstHigh(arg2);

  switch (code)
    {
    case C_BIT_IOR_EXPR:
      low = int1l | int2l, hi = int1h | int2h;
      break;

    case C_BIT_XOR_EXPR:
      low = int1l ^ int2l, hi = int1h ^ int2h;
      break;

    case C_BIT_AND_EXPR:
      low = int1l & int2l, hi = int1h & int2h;
      break;

    case C_BIT_ANDTC_EXPR:
      low = int1l & ~int2l, hi = int1h & ~int2h;
      break;

    case C_RSHIFT_EXPR:
      int2l = - int2l;
    case C_LSHIFT_EXPR:
      /* It's unclear from the C standard whether shifts can overflow.
         The following code ignores overflow; perhaps a C standard
         interpretation ruling is needed.  */
      c_lshift_double(int1l, int1h, int2l,
                     CTreeTypePrecision(CTreeType(arg1)), &low, &hi, !uns);
      no_overflow = 1;
      break;

    case C_RROTATE_EXPR:
      int2l = - int2l;
    case C_LROTATE_EXPR:
      c_lrotate_double(int1l, int1h, int2l,
                      CTreeTypePrecision(CTreeType(arg1)), &low, &hi);
      break;

    case C_PLUS_EXPR:
      overflow = c_add_double(int1l, int1h, int2l, int2h, &low, &hi);
      break;

    case C_MINUS_EXPR:
      c_neg_double (int2l, int2h, &low, &hi);
      c_add_double (int1l, int1h, low, hi, &low, &hi);
      overflow = c_overflow_sum_sign (hi, int2h, int1h);
      break;

    case C_MULT_EXPR:
      overflow = c_mul_double(int1l, int1h, int2l, int2h, &low, &hi);
      break;

    case C_TRUNC_DIV_EXPR:
    case C_FLOOR_DIV_EXPR: 
    case C_CEIL_DIV_EXPR:
    case C_EXACT_DIV_EXPR:
      /* This is a shortcut for a common special case.  */
      if (int2h == 0 && int2l > 0
          && ! CIsTreeNodeCstOverflow(arg1)
          && ! CIsTreeNodeCstOverflow(arg2)
          && int1h == 0 && int1l >= 0)
        {
          if (code == C_CEIL_DIV_EXPR)
            int1l += int2l - 1;
          low = int1l / int2l, hi = 0;
          break;
        }

      /* ... fall through ... */

    case C_ROUND_DIV_EXPR: 
      if (int2h == 0 && int2l == 1)
        {
          low = int1l, hi = int1h;
          break;
        }
      if (int1l == int2l && int1h == int2h
          && ! (int1l == 0 && int1h == 0))
        {
          low = 1, hi = 0;
          break;
        }
      overflow = c_div_and_round_double(code, uns,
                                       int1l, int1h, int2l, int2h,
                                       &low, &hi, &garbagel, &garbageh);
      break;

    case C_TRUNC_MOD_EXPR:
    case C_FLOOR_MOD_EXPR: 
    case C_CEIL_MOD_EXPR:
      /* This is a shortcut for a common special case.  */
      if (int2h == 0 && int2l > 0
          && ! CIsTreeNodeCstOverflow(arg1)
          && ! CIsTreeNodeCstOverflow(arg2)
          && int1h == 0 && int1l >= 0)
        {
          if (code == C_CEIL_MOD_EXPR)
            int1l += int2l - 1;
          low = int1l % int2l, hi = 0;
          break;
        }

      /* ... fall through ... */

    case C_ROUND_MOD_EXPR: 
      overflow = c_div_and_round_double (code, uns,
                                       int1l, int1h, int2l, int2h,
                                       &garbagel, &garbageh, &low, &hi);
      break;

    case C_MIN_EXPR:
    case C_MAX_EXPR:
      if (uns)
        {
          low = (((unsigned C_HOST_WIDE_INT) int1h
                  < (unsigned C_HOST_WIDE_INT) int2h)
                 || (((unsigned C_HOST_WIDE_INT) int1h
                      == (unsigned C_HOST_WIDE_INT) int2h)
                     && ((unsigned C_HOST_WIDE_INT) int1l
                         < (unsigned C_HOST_WIDE_INT) int2l)));
        }
      else
        {
          low = ((int1h < int2h)
                 || ((int1h == int2h)
                     && ((unsigned C_HOST_WIDE_INT) int1l
                         < (unsigned C_HOST_WIDE_INT) int2l)));
        }
      if (low == (code == C_MIN_EXPR))
        low = int1l, hi = int1h;
      else
        low = int2l, hi = int2h;
      break;

    default:
      abort ();
    }

  if (CTreeType(arg1) == c_size_type && hi == 0
      && low >= 0 && low <= CTreeIntCstLow( CTreeTypeMaxValue(c_size_type))
      && ! overflow
      && ! CIsTreeNodeOverflow(arg1) && ! CIsTreeNodeOverflow(arg2))
    t = c_tree_get_size_int(low);
  else
    {
      t = c_tree_build_int_2(low, hi);
      CTreeType(t) = CTreeType(arg1);
    }

# ifdef LUDO_NO_SKIP
  TREE_OVERFLOW (t)
    = ((notrunc ? (!uns || forsize) && overflow
        : force_fit_type (t, (!uns || forsize) && overflow) && ! no_overflow)
       | TREE_OVERFLOW (arg1)
       | TREE_OVERFLOW (arg2));
  /* If we're doing a size calculation, unsigned arithmetic does overflow.
     So check if force_fit_type truncated the value.  */
  if (forsize
      && ! CIsTreeNodeOverflow(t)
      && (CTreeIntCstHigh(t) != hi
          || CTreeIntCstLow(t) != low))
    TREE_OVERFLOW (t) = 1;
  TREE_CONSTANT_OVERFLOW (t) = (TREE_OVERFLOW (t)
                                | TREE_CONSTANT_OVERFLOW (arg1)
                                | TREE_CONSTANT_OVERFLOW (arg2));
# endif
  return t;
}

/* Combine operands OP1 and OP2 with arithmetic operation CODE.
   CODE is a c_tree_node *code.  Data type is taken from `c_size_type',
   If the operands are constant, so is the result.  */

c_tree_node *c_build_size_binop(code, arg0, arg1)
     int code;
     c_tree_node *arg0;
     c_tree_node *arg1;
{
  c_tree_node *tmp_node;
  /* Handle the special case of two integer constants faster.  */
  if ( CTreeNodeFirstCode(arg0) == C_INT_CST_NODE && 
       CTreeNodeFirstCode(arg1) == C_INT_CST_NODE)
    {
      /* And some specific cases even faster than that.  */
      if (code == C_PLUS_EXPR && c_tree_is_integer_zerop(arg0))
        return arg1;
      else if ((code == C_MINUS_EXPR || code == C_PLUS_EXPR)
               && c_tree_is_integer_zerop(arg1))
        return arg0;
      else if (code == C_MULT_EXPR && c_tree_is_integer_onep(arg0))
        return arg1;

      /* Handle general case of two integer constants.  */
      return c_int_const_binop (code, arg0, arg1, 0, 1);
    }

  if (arg0 == c_error_mark_node || arg1 == c_error_mark_node)
    return c_error_mark_node;

  tmp_node = c_tree_build_binary_expr( code, arg0, arg1 );
  CTreeType( tmp_node ) = c_size_type;

  tmp_node = c_fold( tmp_node );


  return( tmp_node );
}



/* Add two doubleword integers with doubleword result.
   Each argument is given as two `HOST_WIDE_INT' pieces.
   One argument is L1 and H1; the other, L2 and H2.
   The value is stored as two `HOST_WIDE_INT' pieces in *LV and *HV.  */

int
c_add_double (l1, h1, l2, h2, lv, hv)
     C_HOST_WIDE_INT l1, h1, l2, h2;
     C_HOST_WIDE_INT *lv, *hv;
{
  C_HOST_WIDE_INT l, h;

  l = l1 + l2;
  h = h1 + h2 + ((unsigned C_HOST_WIDE_INT) l < l1);

  *lv = l;
  *hv = h;
  return c_overflow_sum_sign(h1, h2, h);
}


/* Multiply two doubleword integers with doubleword result.
   Return nonzero if the operation overflows, assuming it's signed.
   Each argument is given as two `HOST_WIDE_INT' pieces.
   One argument is L1 and H1; the other, L2 and H2.
   The value is stored as two `HOST_WIDE_INT' pieces in *LV and *HV.  */

int
c_mul_double (l1, h1, l2, h2, lv, hv)
     C_HOST_WIDE_INT l1, h1, l2, h2;
     C_HOST_WIDE_INT *lv, *hv;
{
  C_HOST_WIDE_INT arg1[4];
  C_HOST_WIDE_INT arg2[4];
  C_HOST_WIDE_INT prod[4 * 2];
  unsigned C_HOST_WIDE_INT carry;
  int i, j, k;
  C_HOST_WIDE_INT toplow, tophigh, neglow, neghigh;

  c_encode(arg1, l1, h1);
  c_encode(arg2, l2, h2);

  bzero ((char *) prod, sizeof prod);

  for (i = 0; i < 4; i++)
    {
      carry = 0;
      for (j = 0; j < 4; j++)
        {
          k = i + j;
          /* This product is <= 0xFFFE0001, the sum <= 0xFFFF0000.  */
          carry += arg1[i] * arg2[j];
          /* Since prod[p] < 0xFFFF, this sum <= 0xFFFFFFFF.  */
          carry += prod[k];
          prod[k] = LOWPART (carry);
          carry = HIGHPART (carry);
        }
      prod[i + 4] = carry;
    }

  c_decode(prod, lv, hv);        /* This ignores prod[4] through prod[4*2-1] */

  /* Check for overflow by calculating the top half of the answer in full;
     it should agree with the low half's sign bit.  */
  c_decode(prod+4, &toplow, &tophigh);
  if (h1 < 0)
    {
      c_neg_double(l2, h2, &neglow, &neghigh);
      c_add_double(neglow, neghigh, toplow, tophigh, &toplow, &tophigh);
    }
  if (h2 < 0)
    {
      c_neg_double(l1, h1, &neglow, &neghigh);
      c_add_double(neglow, neghigh, toplow, tophigh, &toplow, &tophigh);
    }
  return (*hv < 0 ? ~(toplow & tophigh) : toplow | tophigh) != 0;
}

/* Shift the doubleword integer in L1, H1 left by COUNT places
   keeping only PREC bits of result.
   Shift right if COUNT is negative.
   ARITH nonzero specifies arithmetic shifting; otherwise use logical shift.
   Store the value as two `HOST_WIDE_INT' pieces in *LV and *HV.  */


void
c_lshift_double (l1, h1, count, prec, lv, hv, arith)
     C_HOST_WIDE_INT l1, h1, count;
     int prec;
     C_HOST_WIDE_INT *lv, *hv;
     int arith;
{
  if (count < 0)
    {
      c_rshift_double (l1, h1, - count, prec, lv, hv, arith);
      return;
    }
  
#ifdef SHIFT_COUNT_TRUNCATED
  if (SHIFT_COUNT_TRUNCATED)
    count %= prec;
#endif

  if (count >= C_HOST_BITS_PER_WIDE_INT)
    {
      *hv = (unsigned C_HOST_WIDE_INT) l1 << (count - C_HOST_BITS_PER_WIDE_INT);
      *lv = 0;
    }
  else
    {
      *hv = (((unsigned C_HOST_WIDE_INT) h1 << count)
             | ((unsigned C_HOST_WIDE_INT) l1 >> ((C_HOST_BITS_PER_WIDE_INT - count - 1) >> 1)));
      *lv = (unsigned C_HOST_WIDE_INT) l1 << count;
    }
}

/* Shift the doubleword integer in L1, H1 right by COUNT places
   keeping only PREC bits of result.  COUNT must be positive.
   ARITH nonzero specifies arithmetic shifting; otherwise use logical shift.
   Store the value as two `HOST_WIDE_INT' pieces in *LV and *HV.  */

void
c_rshift_double (l1, h1, count, prec, lv, hv, arith)
     C_HOST_WIDE_INT l1, h1, count;
     int prec;
     C_HOST_WIDE_INT *lv, *hv;
     int arith;
{
  unsigned C_HOST_WIDE_INT signmask;
  signmask = (arith
              ? -((unsigned C_HOST_WIDE_INT) h1 >> (C_HOST_BITS_PER_WIDE_INT - 1))
              : 0);

#ifdef SHIFT_COUNT_TRUNCATED
  if (SHIFT_COUNT_TRUNCATED)
    count %= prec;
#endif

  if (count >= C_HOST_BITS_PER_WIDE_INT)
    {
      *hv = signmask;
      *lv = ((signmask << ((2 * C_HOST_BITS_PER_WIDE_INT - count - 1) << 1))
             | ((unsigned C_HOST_WIDE_INT) h1 >> (count - C_HOST_BITS_PER_WIDE_INT)));
    }
  else
    {
      *lv = (((unsigned C_HOST_WIDE_INT) l1 >> count)
             | ((unsigned C_HOST_WIDE_INT) h1 << ((C_HOST_BITS_PER_WIDE_INT - count - 1) << 1)));
      *hv = ((signmask << (C_HOST_BITS_PER_WIDE_INT - count))
             | ((unsigned C_HOST_WIDE_INT) h1 >> count));
    }
}

/* Rotate the doubleword integer in L1, H1 left by COUNT places
   keeping only PREC bits of result.
   Rotate right if COUNT is negative.
   Store the value as two `HOST_WIDE_INT' pieces in *LV and *HV.  */

void
c_lrotate_double (l1, h1, count, prec, lv, hv)
     C_HOST_WIDE_INT l1, h1, count;
     int prec;
     C_HOST_WIDE_INT *lv, *hv;
{
  C_HOST_WIDE_INT s1l, s1h, s2l, s2h;

  count %= prec;
  if (count < 0)
    count += prec;

  c_lshift_double (l1, h1, count, prec, &s1l, &s1h, 0);
  c_rshift_double (l1, h1, prec - count, prec, &s2l, &s2h, 0);
  *lv = s1l | s2l;
  *hv = s1h | s2h;
}

/* Rotate the doubleword integer in L1, H1 left by COUNT places
   keeping only PREC bits of result.  COUNT must be positive.
   Store the value as two `HOST_WIDE_INT' pieces in *LV and *HV.  */

void
c_rrotate_double (l1, h1, count, prec, lv, hv)
     C_HOST_WIDE_INT l1, h1, count;
     int prec;
     C_HOST_WIDE_INT *lv, *hv;
{
  C_HOST_WIDE_INT s1l, s1h, s2l, s2h;

  count %= prec;
  if (count < 0)
    count += prec;

  c_rshift_double (l1, h1, count, prec, &s1l, &s1h, 0);
  c_lshift_double (l1, h1, prec - count, prec, &s2l, &s2h, 0);
  *lv = s1l | s2l;
  *hv = s1h | s2h;
}



/* Combine two constants ARG1 and ARG2 under operation CODE
   to produce a new constant.
   We assume ARG1 and ARG2 have the same data type,
   or at least are the same kind of constant and the same machine mode.

   If NOTRUNC is nonzero, do not truncate the result to fit the data type.  */

static c_tree_node *c_const_binop(code, arg1, arg2, notrunc)
     int  code;
     c_tree_node *arg1;
     c_tree_node *arg2;
     int notrunc;
{
  CStripNops(arg1); CStripNops(arg2);

  if ( CTreeNodeCode(arg1) == C_INT_CST_NODE)
    return c_int_const_binop(code, arg1, arg2, notrunc, 0);

# ifdef LUDO_NO_SKIP
#if ! defined (REAL_IS_NOT_DOUBLE) || defined (REAL_ARITHMETIC)
  if (CTreeNodeCode(arg1) == REAL_CST)
    {
      REAL_VALUE_TYPE d1;
      REAL_VALUE_TYPE d2;
      int overflow = 0;
      REAL_VALUE_TYPE value;
      c_tree_node *t;

      d1 = TREE_REAL_CST (arg1);
      d2 = TREE_REAL_CST (arg2);

      /* If either operand is a NaN, just return it.  Otherwise, set up
         for floating-point trap; we return an overflow.  */
      if (REAL_VALUE_ISNAN (d1))
        return arg1;
      else if (REAL_VALUE_ISNAN (d2))
        return arg2;
      else if (setjmp (float_error))
        {
          t = copy_node (arg1);
          overflow = 1;
          goto got_float;
        }

      set_float_handler (float_error);

#ifdef REAL_ARITHMETIC
      REAL_ARITHMETIC (value, code, d1, d2);
#else
      switch (code)
        {
        case PLUS_EXPR:
          value = d1 + d2;
          break;

        case MINUS_EXPR:
          value = d1 - d2;
          break;

        case MULT_EXPR:
          value = d1 * d2;
          break;

        case RDIV_EXPR:
#ifndef REAL_INFINITY
          if (d2 == 0)
            abort ();
#endif

          value = d1 / d2;
          break;

        case MIN_EXPR:
          value = MIN (d1, d2);
          break;

        case MAX_EXPR:
          value = MAX (d1, d2);
          break;

        default:
          abort ();
        }
#endif /* no REAL_ARITHMETIC */
      t = build_real (CTreeType(arg1),
                      real_value_truncate (CTreeTypeMode(CTreeType(arg1)), value));
    got_float:
      set_float_handler (NULL_PTR);

      TREE_OVERFLOW (t)
        = (force_fit_type (t, overflow)
           | TREE_OVERFLOW (arg1) | TREE_OVERFLOW (arg2));
      TREE_CONSTANT_OVERFLOW (t)
        = TREE_OVERFLOW (t)
          | TREE_CONSTANT_OVERFLOW (arg1)
          | TREE_CONSTANT_OVERFLOW (arg2);
      return t;
    }
#endif /* not REAL_IS_NOT_DOUBLE, or REAL_ARITHMETIC */
# endif
  if (CTreeNodeCode(arg1) == C_COMPLEX_NODE)
    {
      c_tree_node *type = CTreeType(arg1);
      c_tree_node *r1 = CTreeComplexReal(arg1);
      c_tree_node *i1 = CTreeComplexImag(arg1);
      c_tree_node *r2 = CTreeComplexReal(arg2);
      c_tree_node *i2 = CTreeComplexImag(arg2);
      c_tree_node *t;

      switch (code)
        {
        case C_PLUS_EXPR:
          t = c_tree_build_complex (type,
                             c_const_binop(C_PLUS_EXPR, r1, r2, notrunc),
                             c_const_binop(C_PLUS_EXPR, i1, i2, notrunc));
          break;

        case C_MINUS_EXPR:
          t = c_tree_build_complex (type,
                             c_const_binop(C_MINUS_EXPR, r1, r2, notrunc),
                             c_const_binop(C_MINUS_EXPR, i1, i2, notrunc));
          break;

        case C_MULT_EXPR:
          t = c_tree_build_complex(type,
                             c_const_binop(C_MINUS_EXPR,
                                          c_const_binop(C_MULT_EXPR,
                                                       r1, r2, notrunc),
                                          c_const_binop(C_MULT_EXPR,
                                                       i1, i2, notrunc),
                                          notrunc),
                             c_const_binop(C_PLUS_EXPR,
                                          c_const_binop(C_MULT_EXPR,
                                                       r1, i2, notrunc),
                                          c_const_binop(C_MULT_EXPR,
                                                       i1, r2, notrunc),
                                          notrunc));
          break;

        case C_RDIV_EXPR:
          {
            c_tree_node *magsquared
              = c_const_binop(C_PLUS_EXPR,
                             c_const_binop(C_MULT_EXPR, r2, r2, notrunc),
                             c_const_binop(C_MULT_EXPR, i2, i2, notrunc),
                             notrunc);

            t = c_tree_build_complex(type,
                               c_const_binop
                               (INTEGRAL_TYPE_P (CTreeType(r1))
                                ? C_TRUNC_DIV_EXPR : C_RDIV_EXPR,
                                c_const_binop(C_PLUS_EXPR,
                                             c_const_binop(C_MULT_EXPR, r1, r2,
                                                          notrunc),
                                             c_const_binop(C_MULT_EXPR, i1, i2,
                                                          notrunc),
                                             notrunc),
                                magsquared, notrunc),
                               c_const_binop
                               (INTEGRAL_TYPE_P (CTreeType(r1))
                                ? C_TRUNC_DIV_EXPR : C_RDIV_EXPR,
                                c_const_binop(C_MINUS_EXPR,
                                             c_const_binop(C_MULT_EXPR, i1, r2,
                                                          notrunc),
                                             c_const_binop(C_MULT_EXPR, r1, i2,
                                                          notrunc),
                                             notrunc),
                                magsquared, notrunc));
          }
          break;

        default:
          abort ();
        }
      return t;
    }
  return 0;
}


/* Return a c_tree_node *for the case when the result of an expression is RESULT
   converted to TYPE and OMITTED was previously an operand of the expression
   but is now not needed (e.g., we folded OMITTED * 0).

   If OMITTED has side effects, we must evaluate it.  Otherwise, just do
   the conversion of RESULT to TYPE.  */

static c_tree_node *c_omit_one_operand(type, result, omitted)
     c_tree_node *type;
     c_tree_node *result;
     c_tree_node *omitted;
{
  c_tree_node *t = c_convert_expr(type, result);

  if ( CIsTreeNodeSideEffect(omitted))
    return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type, omitted, t);

  return c_get_non_lvalue(t);
}


/* Similar, but call pedantic_non_lvalue instead of non_lvalue.  */

static c_tree_node *c_pedantic_omit_one_operand(type, result, omitted)
     c_tree_node *type;
     c_tree_node *result;
     c_tree_node *omitted;
{
  c_tree_node *t = c_convert_expr(type, result);

  if ( CIsTreeNodeSideEffect(omitted))
    return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type, omitted, t);

  return c_get_pedantic_non_lvalue(t);
}


/* Given a bit-wise operation CODE applied to ARG0 and ARG1, see if both
   operands are another bit-wise operation with a common input.  If so,
   distribute the bit operations to save an operation and possibly two if
   constants are involved.  For example, convert
           (A | B) & (A | C) into A | (B & C)
   Further simplification will occur if B and C are constants.

   If this optimization cannot be done, 0 will be returned.  */

static c_tree_node *c_distribute_bit_expr(code, type, arg0, arg1)
     int  code;
     c_tree_node *type;
     c_tree_node *arg0;
     c_tree_node *arg1;
{
  c_tree_node *common;
  c_tree_node *left;
  c_tree_node *right;

  if (CTreeNodeCode(arg0) != CTreeNodeCode(arg1)
      || CTreeNodeCode(arg0) == code
      || (CTreeNodeCode(arg0) != C_BIT_AND_EXPR
          && CTreeNodeCode(arg0) != C_BIT_IOR_EXPR))
    return 0;

  if (c_is_operand_equal_p(CTreeExprOperand(arg0, 0), CTreeExprOperand(arg1, 0), 0))
    {
      common = CTreeExprOperand(arg0, 0);
      left = CTreeExprOperand(arg0, 1);
      right = CTreeExprOperand(arg1, 1);
    }
  else if (c_is_operand_equal_p(CTreeExprOperand(arg0, 0), CTreeExprOperand(arg1, 1), 0))
    {
      common = CTreeExprOperand(arg0, 0);
      left = CTreeExprOperand(arg0, 1);
      right = CTreeExprOperand(arg1, 0);
    }
  else if (c_is_operand_equal_p(CTreeExprOperand(arg0, 1), CTreeExprOperand(arg1, 0), 0))
    {
      common = CTreeExprOperand(arg0, 1);
      left = CTreeExprOperand(arg0, 0);
      right = CTreeExprOperand(arg1, 1);
    }
  else if (c_is_operand_equal_p(CTreeExprOperand(arg0, 1), CTreeExprOperand(arg1, 1), 0))
    {
      common = CTreeExprOperand(arg0, 1);
      left = CTreeExprOperand(arg0, 0);
      right = CTreeExprOperand(arg1, 0);
    }
  else
    return 0;

  return c_fold( c_tree_build_binary_typed_expr(
          CTreeNodeCode(arg0), type, common,
          c_fold(c_tree_build_binary_typed_expr(code, type, left, right))));
}


/* ARG is a c_tree_node *that is known to contain just arithmetic operations and
   comparisons.  Evaluate the operations in the c_tree_node *substituting NEW0 for
   any occurrence of OLD0 as an operand of a comparison and likewise for
   NEW1 and OLD1.  */

static c_tree_node *c_eval_subst(arg, old0, new0, old1, new1)
     c_tree_node *arg;
     c_tree_node *old0;
     c_tree_node *new0;
     c_tree_node *old1;
     c_tree_node *new1;
{
  c_tree_node *type = CTreeType(arg);
  int code = CTreeNodeCode(arg);
  char class = c_tree_get_code_class(code);

  /* We can handle some of the 'e' cases here.  */
  if (class == 'e' && code == C_TRUTH_NOT_EXPR)
    class = '1';
  else if (class == 'e'
           && (code == C_TRUTH_ANDIF_EXPR || code == C_TRUTH_ORIF_EXPR))
    class = '2';

  switch (class)
    {
    case '1':
      return c_fold( c_tree_build_unary_typed_expr(code, type,
                           c_eval_subst(CTreeExprOperand(arg, 0),
                                       old0, new0, old1, new1)));

    case '2':
      return c_fold( c_tree_build_binary_typed_expr(code, type,
                          c_eval_subst(CTreeExprOperand(arg, 0),
                                      old0, new0, old1, new1),
                          c_eval_subst(CTreeExprOperand(arg, 1),
                                      old0, new0, old1, new1)));

    case 'e':
      switch (code)
        {
        case C_SAVE_EXPR:
          return c_eval_subst(CTreeExprOperand(arg, 0), old0, new0, old1, new1);

        case C_COMPOUND_EXPR:
          return c_eval_subst(CTreeExprOperand(arg, 1), old0, new0, old1, new1);

        case C_COND_EXPR:
          return c_fold(c_tree_build_ternary_typed_expr(code, type,
                              c_eval_subst(CTreeExprOperand(arg, 0),
                                          old0, new0, old1, new1),
                              c_eval_subst(CTreeExprOperand(arg, 1),
                                          old0, new0, old1, new1),
                              c_eval_subst(CTreeExprOperand(arg, 2),
                                          old0, new0, old1, new1)));
        default:
          break;
        }
      /* fall through (???) */

    case '<':
      {
        c_tree_node *arg0 = CTreeExprOperand(arg, 0);
        c_tree_node *arg1 = CTreeExprOperand(arg, 1);

        /* We need to check both for exact equality and c_tree_node *equality.  The
           former will be true if the operand has a side-effect.  In that
           case, we know the operand occurred exactly once.  */

        if (arg0 == old0 || c_is_operand_equal_p(arg0, old0, 0))
          arg0 = new0;
        else if (arg0 == old1 || c_is_operand_equal_p(arg0, old1, 0))
          arg0 = new1;

        if (arg1 == old0 || c_is_operand_equal_p(arg1, old0, 0))
          arg1 = new0;
        else if (arg1 == old1 || c_is_operand_equal_p(arg1, old1, 0))
          arg1 = new1;

        return c_fold( c_tree_build_binary_typed_expr(code, type, arg0, arg1));
      }

    default:
      return arg;
    }
}


c_tree_node *c_fold( expr )

  c_tree_node *expr;
{
  c_tree_node *t = expr;
  c_tree_node *t1 = NULL;
  c_tree_node *tem;
  c_tree_node *type = CTreeType(expr);
  c_tree_node *arg0;
  c_tree_node *arg1;
  int code = CTreeNodeCode(t);
  int kind;
  int invert;

  /* WINS will be nonzero when the switch is done
     if all operands are constant.  */

  int wins = 1;

  /* Don't try to process an RTL_EXPR since its operands aren't trees. 
     Likewise for a SAVE_EXPR that's already been evaluated.  */
  if (code == C_RTL_EXPR 
# ifdef LUDO_NO_SKIP
      || (code == C_SAVE_EXPR && SAVE_EXPR_RTL (t)) != 0
# endif
      )
    return t;

  /* Return right away if already constant.  */
  if (CIsTreeNodeConstant(t))
    {
      if (code == C_CONST_DECL)
        return CTreeDeclInitial(t);
      return t;
    }
  
  kind = c_tree_get_code_class(code);
  if (code == C_NOP_EXPR || code == C_FLOAT_EXPR || code == C_CONVERT_EXPR)
    {
      c_tree_node *subop;

      /* Special case for conversion ops that can have fixed point args.  */
      arg0 = CTreeExprOperand(t, 0);

      /* Don't use STRIP_NOPS, because signedness of argument type matters.  */
      if (arg0 != 0)
      {
        CStripTypeNops(arg0);
      }

      if (arg0 != 0 && CTreeNodeFirstCode(arg0) == C_COMPLEX_NODE)
        subop = CTreeComplexReal(arg0);
      else
        subop = arg0;

      if (subop != 0 &&  CTreeNodeFirstCode(subop) != C_INT_CST_NODE
#if ! defined (REAL_IS_NOT_DOUBLE) || defined (REAL_ARITHMETIC)
          && CTreeNodeFirstCode(subop) != C_REAL_CST_NODE
#endif /* not REAL_IS_NOT_DOUBLE, or REAL_ARITHMETIC */
          )
        /* Note that CIsTreeNodeConstantisn't enough:
           static var addresses are constant but we can't
           do arithmetic on them.  */
        wins = 0;
    }
  else if (kind == 'e' || kind == '<'
           || kind == '1' || kind == '2' || kind == 'r')
    {
      int len = CTreeExprNumOper(t);
      int i;
      for (i = 0; i < len; i++)
        {
          c_tree_node *op = CTreeExprOperand(t, i);
          c_tree_node *subop;

          if (op == 0)
            continue;                /* Valid for CALL_EXPR, at least.  */

          if (kind == '<' || code == C_RSHIFT_EXPR)
            {
              /* Signedness matters here.  Perhaps we can refine this
                 later.  */
              CStripTypeNops(op);
            }
          else
            {
              /* Strip any conversions that don't change the mode.  */
              CStripNops(op);
            }
          
          if ( CTreeNodeFirstCode(op) == C_COMPLEX_NODE)
            subop = CTreeComplexReal(op);
          else
            subop = op;

          if ( CTreeNodeFirstCode(subop) != C_INT_CST_NODE
#if ! defined (REAL_IS_NOT_DOUBLE) || defined (REAL_ARITHMETIC)
              && CTreeNodeFirstCode(subop) != C_REAL_CST_NODE
#endif /* not REAL_IS_NOT_DOUBLE, or REAL_ARITHMETIC */
              )
            /* Note that CIsTreeNodeConstantisn't enough:
               static var addresses are constant but we can't
               do arithmetic on them.  */
            wins = 0;

          if (i == 0)
            arg0 = op;
          else if (i == 1)
            arg1 = op;
        }
    }

  /* If this is a commutative operation, and ARG0 is a constant, move it
     to ARG1 to reduce the number of tests below.  */
  if ((code == C_PLUS_EXPR || code == C_MULT_EXPR || code == C_MIN_EXPR
       || code == C_MAX_EXPR || code == C_BIT_IOR_EXPR || code == C_BIT_XOR_EXPR
       || code == C_BIT_AND_EXPR)
      && ( CTreeNodeFirstCode(arg0) == C_INT_CST_NODE ||
          CTreeNodeFirstCode(arg0) == C_REAL_CST_NODE))
    {
      tem = arg0; arg0 = arg1; arg1 = tem;

      tem = CTreeExprOperand(t, 0); CTreeExprOperand(t, 0) = CTreeExprOperand(t, 1);
      CTreeExprOperand(t, 1) = tem;
    }

  /* Now WINS is set as described above,
     ARG0 is the first operand of EXPR,
     and ARG1 is the second operand (if it has more than one operand).

     First check for cases where an arithmetic operation is applied to a
     compound, conditional, or comparison operation.  Push the arithmetic
     operation inside the compound or conditional to see if any folding
     can then be done.  Convert comparison to conditional for this purpose.
     The also optimizes non-constant cases that used to be done in
     expand_expr.

     Before we do that, see if this is a BIT_AND_EXPR or a BIT_OR_EXPR,
     one of the operands is a comparison and the other is a comparison, a
     BIT_AND_EXPR with the constant 1, or a truth value.  In that case, the
     code below would make the expression more complex.  Change it to a
     TRUTH_{AND,OR}_EXPR.  Likewise, c_convert_expra similar NE_EXPR to 
     TRUTH_XOR_EXPR and an EQ_EXPR to the inversion of a TRUTH_XOR_EXPR.  */

  if ((code == C_BIT_AND_EXPR || code == C_BIT_IOR_EXPR
       || code == C_EQ_EXPR || code == C_NE_EXPR)
      && ((c_truth_value_p(CTreeNodeCode(arg0))
           && (c_truth_value_p(CTreeNodeCode(arg1))
               || (CTreeNodeCode(arg1) == C_BIT_AND_EXPR
                   && c_tree_is_integer_onep (CTreeExprOperand(arg1, 1)))))
          || (c_truth_value_p(CTreeNodeCode(arg1))
              && (c_truth_value_p(CTreeNodeCode(arg0))
                  || (CTreeNodeCode(arg0) == C_BIT_AND_EXPR
                      && c_tree_is_integer_onep (CTreeExprOperand(arg0, 1)))))))
    {
      t = c_fold( c_tree_build_binary_typed_expr(
          code == C_BIT_AND_EXPR ? C_TRUTH_AND_EXPR
                       : code == C_BIT_IOR_EXPR ? C_TRUTH_OR_EXPR
                       : C_TRUTH_XOR_EXPR, type, arg0, arg1));

      if (code == C_EQ_EXPR)
        t = c_invert_truthvalue(t);

      return t;
    }

  if (c_tree_get_code_class(code) == '1')
    {
      if (CTreeNodeCode(arg0) == C_COMPOUND_EXPR)
        return c_tree_build_binary_typed_expr(
            C_COMPOUND_EXPR, type, CTreeExprOperand(arg0, 0),
            c_fold( c_tree_build_unary_typed_expr(code, type, CTreeExprOperand(arg0, 1))));
      else if (CTreeNodeCode(arg0) == C_COND_EXPR)
        {
          t = c_fold(
            c_tree_build_ternary_typed_expr(C_COND_EXPR, type, CTreeExprOperand(arg0, 0),
              c_fold(c_tree_build_unary_typed_expr(code, type, CTreeExprOperand(arg0, 1))),
              c_fold(c_tree_build_unary_typed_expr(code, type, CTreeExprOperand(arg0, 2)))));

          /* If this was a conversion, and all we did was to move into
             inside the COND_EXPR, bring it back out.  But leave it if
             it is a conversion from integer to integer and the
             result precision is no wider than a word since such a
             conversion is cheap and may be optimized away by combine,
             while it couldn't if it were outside the COND_EXPR.  Then return
             so we don't get into an infinite recursion loop taking the
             conversion out and then back in.  */

          if ((code == C_NOP_EXPR || code == C_CONVERT_EXPR
               || code == C_NON_LVALUE_EXPR)
              && CTreeNodeCode(t) == C_COND_EXPR
              && CTreeNodeCode(CTreeExprOperand(t, 1)) == code
              && CTreeNodeCode(CTreeExprOperand(t, 2)) == code
              && (CTreeType(CTreeExprOperand(CTreeExprOperand(t, 1), 0))
                  == CTreeType(CTreeExprOperand(CTreeExprOperand(t, 2), 0)))
              && ! ( INTEGRAL_TYPE_P(CTreeType(t))
                    && INTEGRAL_TYPE_P(CTreeType(CTreeExprOperand(CTreeExprOperand(t, 1), 0)))
                    && CTreeTypePrecision(CTreeType(t)) <= C_BITS_PER_WORD))
            t = c_tree_build_unary_typed_expr(code, type,
                        c_tree_build_ternary_typed_expr(C_COND_EXPR,
                               CTreeType(CTreeExprOperand(CTreeExprOperand(t, 1), 0)),
                               CTreeExprOperand(t, 0),
                               CTreeExprOperand(CTreeExprOperand(t, 1), 0),
                               CTreeExprOperand(CTreeExprOperand(t, 2), 0)));
          return t;
        }
      else if ( c_tree_get_code_class(CTreeNodeCode(arg0)) == '<') 
        return c_fold(c_tree_build_ternary_typed_expr(C_COND_EXPR, type, arg0,
                c_fold(c_tree_build_unary_typed_expr(code, type, c_integer_one_node)),
                c_fold(c_tree_build_unary_typed_expr(code, type, c_integer_zero_node))));
   }
  else if (c_tree_get_code_class(code) == '2'
           || c_tree_get_code_class(code) == '<')
    {
      if ( CTreeNodeCode(arg1) == C_COMPOUND_EXPR)
        return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type,
            CTreeExprOperand(arg1, 0),
            c_fold(c_tree_build_binary_typed_expr(code, type,
                                   arg0, CTreeExprOperand(arg1, 1))));
      else if ((CTreeNodeCode(arg1) == C_COND_EXPR
                || (c_tree_get_code_class(CTreeNodeCode(arg1)) == '<'
                    && c_tree_get_code_class(code) != '<'))
               && (! CIsTreeNodeSideEffect(arg0) || c_current_function_decl != 0))
        {
          c_tree_node *test;
          c_tree_node *true_value;
          c_tree_node *false_value;

          if (CTreeNodeCode(arg1) == C_COND_EXPR)
            {
              test = CTreeExprOperand(arg1, 0);
              true_value = CTreeExprOperand(arg1, 1);
              false_value = CTreeExprOperand(arg1, 2);
            }
          else
            {
              c_tree_node *testtype = CTreeType(arg1);
              test = arg1;
              true_value = c_convert_expr(testtype, c_integer_one_node);
              false_value = c_convert_expr(testtype, c_integer_zero_node);
            }

          /* If ARG0 is complex we want to make sure we only evaluate
             it once.  Though this is only required if it is volatile, it
             might be more efficient even if it is not.  However, if we
             succeed in folding one part to a constant, we do not need
             to make this SAVE_EXPR.  Since we do this optimization
             primarily to see if we do end up with constant and this
             SAVE_EXPR interferes with later optimizations, suppressing
             it when we can is important.  */

          if (CTreeNodeCode(arg0) != C_SAVE_EXPR
              && ((CTreeNodeCode(arg0) != C_VAR_DECL
                   && CTreeNodeCode(arg0) != C_PARAM_DECL)
                  || CIsTreeNodeSideEffect(arg0)))
            {
              c_tree_node *lhs = 
                c_fold(c_tree_build_binary_typed_expr(code, type, arg0, true_value));
              c_tree_node *rhs = 
                c_fold(c_tree_build_binary_typed_expr(code, type, arg0, false_value));

              if (CIsTreeNodeConstant(lhs) || CIsTreeNodeConstant(rhs))
                return c_fold(
                    c_tree_build_ternary_typed_expr(C_COND_EXPR, type, test, lhs, rhs));

              if (c_current_function_decl != 0)
              {
# ifdef LUDO_NO_SKIP
                arg0 = save_expr (arg0);
# endif
              }
            }

          test = c_fold(c_tree_build_ternary_typed_expr(C_COND_EXPR, type, test,
                c_fold(c_tree_build_binary_typed_expr(code, type, arg0, true_value)),
                c_fold(c_tree_build_binary_typed_expr(code, type, arg0, false_value))));
          if (CTreeNodeCode(arg0) == C_SAVE_EXPR)
            return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type,
                          c_convert_expr(c_void_type_node, arg0),
                          c_strip_compound_expr(test, arg0));
          else
            return c_convert_expr(type, test);
        }

      else if (CTreeNodeCode(arg0) == C_COMPOUND_EXPR)
        return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type,
            CTreeExprOperand(arg0, 0),
            c_fold(c_tree_build_binary_typed_expr(code, type, 
                CTreeExprOperand(arg0, 1), arg1)));
      else if ((CTreeNodeCode(arg0) == C_COND_EXPR
                || (c_tree_get_code_class(CTreeNodeCode(arg0)) == '<'
                    && c_tree_get_code_class(code) != '<'))
               && (! CIsTreeNodeSideEffect(arg1) || c_current_function_decl != 0))
        {
          c_tree_node *test;
          c_tree_node *true_value;
          c_tree_node *false_value;

          if (CTreeNodeCode(arg0) == C_COND_EXPR)
            {
              test = CTreeExprOperand(arg0, 0);
              true_value = CTreeExprOperand(arg0, 1);
              false_value = CTreeExprOperand(arg0, 2);
            }
          else
            {
              c_tree_node *testtype = CTreeType(arg0);
              test = arg0;
              true_value = c_convert_expr(testtype, c_integer_one_node);
              false_value = c_convert_expr(testtype, c_integer_zero_node);
            }

          if (CTreeNodeCode(arg1) != C_SAVE_EXPR
              && ((CTreeNodeCode(arg1) != C_VAR_DECL
                   && CTreeNodeCode(arg1) != C_PARAM_DECL)
                  || CIsTreeNodeSideEffect(arg1)))
            {
              c_tree_node *lhs = c_fold(
                  c_tree_build_binary_typed_expr(code, type, true_value, arg1));
              c_tree_node *rhs = c_fold(
                  c_tree_build_binary_typed_expr(code, type, false_value, arg1));

              if (CIsTreeNodeConstant(lhs) || CIsTreeNodeConstant(rhs)
                  || CIsTreeNodeConstant(arg1))
                return c_fold(
                    c_tree_build_ternary_typed_expr(C_COND_EXPR, type, test, lhs, rhs));

# ifdef LUDO_NO_SKIP
              if (c_current_function_decl != 0)
                arg1 = save_expr (arg1);
# endif
            }

          test = c_fold(
              c_tree_build_ternary_typed_expr(C_COND_EXPR, type, test,
                c_fold(c_tree_build_binary_typed_expr(code, type, true_value, arg1)),
                c_fold(c_tree_build_binary_typed_expr(code, type, false_value, arg1))));
          if (CTreeNodeCode(arg1) == C_SAVE_EXPR)
            return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type,
                          c_convert_expr(c_void_type_node, arg1),
                          c_strip_compound_expr(test, arg1));
          else
            return c_convert_expr(type, test);
        }
    }
  else if (c_tree_get_code_class(code) == '<'
           && CTreeNodeCode(arg0) == C_COMPOUND_EXPR)
    return 
      c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type, CTreeExprOperand(arg0, 0),
        c_fold( c_tree_build_binary_typed_expr(code, type, CTreeExprOperand(arg0, 1), arg1)));

  else if (c_tree_get_code_class(code) == '<'
           && CTreeNodeCode(arg1) == C_COMPOUND_EXPR)
    return c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, type, CTreeExprOperand(arg1, 0),
      c_fold(c_tree_build_binary_typed_expr(code, type, arg0, CTreeExprOperand(arg1, 1))));
          
  switch (code)
    {
    case C_INT_CST_NODE:
    case C_REAL_CST_NODE:
    case C_STRING_NODE:
    case C_COMPLEX_NODE:
    case C_CONSTRUCTOR_EXPR:
      return t;

    case C_CONST_DECL:
      return c_fold(CTreeDeclInitial(t));

    case C_NOP_EXPR:
    case C_FLOAT_EXPR:
    case C_CONVERT_EXPR:
    case C_FIX_TRUNC_EXPR:
      /* Other kinds of FIX are not handled properly by fold_convert.  */

      if (CTreeType(CTreeExprOperand(t, 0)) == CTreeType(t))
        return CTreeExprOperand(t, 0);

      /* Handle cases of two conversions in a row.  */
      if (CTreeNodeCode(CTreeExprOperand(t, 0)) == C_NOP_EXPR
          || CTreeNodeCode(CTreeExprOperand(t, 0)) == C_CONVERT_EXPR)
        {
          c_tree_node *inside_type = CTreeType(CTreeExprOperand(CTreeExprOperand(t, 0), 0));
          c_tree_node *inter_type = CTreeType(CTreeExprOperand(t, 0));
          c_tree_node *final_type = CTreeType(t);
          int inside_int = INTEGRAL_TYPE_P (inside_type);
          int inside_ptr = POINTER_TYPE_P (inside_type);
          int inside_float = FLOAT_TYPE_P (inside_type);
          int inside_prec = CTreeTypePrecision(inside_type);
          int inside_unsignedp = CIsTreeNodeUnsigned(inside_type);
          int inter_int = INTEGRAL_TYPE_P (inter_type);
          int inter_ptr = POINTER_TYPE_P (inter_type);
          int inter_float = FLOAT_TYPE_P (inter_type);
          int inter_prec = CTreeTypePrecision(inter_type);
          int inter_unsignedp = CIsTreeNodeUnsigned(inter_type);
          int final_int = INTEGRAL_TYPE_P (final_type);
          int final_ptr = POINTER_TYPE_P (final_type);
          int final_float = FLOAT_TYPE_P (final_type);
          int final_prec = CTreeTypePrecision(final_type);
          int final_unsignedp = CIsTreeNodeUnsigned(final_type);

          /* In addition to the cases of two conversions in a row 
             handled below, if we are converting something to its own
             type via an object of identical or wider precision, neither
             conversion is needed.  */
          if (inside_type == final_type
              && ((inter_int && final_int) || (inter_float && final_float))
              && inter_prec >= final_prec)
            return CTreeExprOperand(CTreeExprOperand(t, 0), 0);

          /* Likewise, if the intermediate and final types are either both
             float or both integer, we don't need the middle conversion if
             it is wider than the final type and doesn't change the signedness
             (for integers).  Avoid this if the final type is a pointer
             since then we sometimes need the inner conversion.  Likewise if
             the outer has a precision not equal to the size of its mode.  */
          if ((((inter_int || inter_ptr) && (inside_int || inside_ptr))
               || (inter_float && inside_float))
              && inter_prec >= inside_prec
              && (inter_float || inter_unsignedp == inside_unsignedp)
# ifdef LUDO_NO_SKIP
              && ! (final_prec != GET_MODE_BITSIZE (CTreeTypeMode(final_type))
                    && CTreeTypeMode(final_type) == CTreeTypeMode(inter_type))
# endif
              && ! final_ptr)
            return c_convert_expr(final_type, CTreeExprOperand(CTreeExprOperand(t, 0), 0));

          /* Two conversions in a row are not needed unless:
             - some conversion is floating-point (overstrict for now), or
             - the intermediate type is narrower than both initial and
               final, or
             - the intermediate type and innermost type differ in signedness,
               and the outermost type is wider than the intermediate, or
             - the initial type is a pointer type and the precisions of the
               intermediate and final types differ, or
             - the final type is a pointer type and the precisions of the 
               initial and intermediate types differ.  */
          if (! inside_float && ! inter_float && ! final_float
              && (inter_prec > inside_prec || inter_prec > final_prec)
              && ! (inside_int && inter_int
                    && inter_unsignedp != inside_unsignedp
                    && inter_prec < final_prec)
              && ((inter_unsignedp && inter_prec > inside_prec)
                  == (final_unsignedp && final_prec > inter_prec))
              && ! (inside_ptr && inter_prec != final_prec)
              && ! (final_ptr && inside_prec != inter_prec)
# ifdef LUDO_NO_SKIP
              && ! (final_prec != GET_MODE_BITSIZE (CTreeTypeMode(final_type))
                    && CTreeTypeMode(final_type) == CTreeTypeMode(inter_type))
# endif
              && ! final_ptr)
            return c_convert_expr(final_type, CTreeExprOperand(CTreeExprOperand(t, 0), 0));
        }

      if (CTreeNodeCode(CTreeExprOperand(t, 0)) == C_MODIFY_EXPR
          && CIsTreeNodeConstant(CTreeExprOperand(CTreeExprOperand(t, 0), 1))
          /* Detect assigning a bitfield.  */
          && !(CTreeNodeCode(CTreeExprOperand(CTreeExprOperand(t, 0), 0)) == C_COMPONENT_REF
          && CIsTreeDeclBitField(
              CTreeExprOperand(CTreeExprOperand(CTreeExprOperand(t, 0), 0), 1))))
        {
          /* Don't leave an assignment inside a conversion
             unless assigning a bitfield.  */
          c_tree_node *prev = CTreeExprOperand(t, 0);
          CTreeExprOperand(t, 0) = CTreeExprOperand(prev, 1);
          /* First do the assignment, then return converted constant.  */
          t = c_tree_build_binary_typed_expr(C_COMPOUND_EXPR, CTreeType(t), prev, c_fold(t));
          CSetTreeNodeUsed(t);
          return t;
        }
      if (!wins)
        {
          if ( CIsTreeNodeConstant(arg0) )
            CSetTreeNodeConstant(t);
          return t;
        }
      return c_fold_convert_expr(t, arg0);

    case C_COMPONENT_REF:
      if (CTreeNodeCode(arg0) == C_CONSTRUCTOR_EXPR)
        {
          c_tree_node *m = c_tree_get_purpose_member(arg1, CTreeExprOperand(arg0,1));
          if (m)
            t = CTreeListValue(m);
        }
      return t;

    case C_RANGE_EXPR:
      if ( wins ) CSetTreeNodeConstant(t);
      else        CClearTreeNodeConstant(t);

      return t;

    case C_NEGATE_EXPR:
      if (wins)
        {
          if (CTreeNodeCode(arg0) == C_INT_CST_NODE)
            {
              C_HOST_WIDE_INT low, high;
              int overflow = c_neg_double (CTreeIntCstLow(arg0),
                                          CTreeIntCstHigh(arg0),
                                          &low, &high);
              t = c_tree_build_int_2(low, high);
              CTreeType(t) = type;
              if ( (CIsTreeNodeOverflow(arg0)
                   || c_force_fit_type(t, overflow && !CIsTreeNodeUnsigned(type))))
                CSetTreeNodeOverflow(t);

              if ( CIsTreeNodeOverflow(t) || CIsTreeNodeCstOverflow(arg0))
                CSetTreeNodeCstOverflow(t);
            }
          else if (CTreeNodeCode(arg0) == C_REAL_CST_NODE)
            t = c_tree_build_real(type, REAL_VALUE_NEGATE( CTreeRealCstValue(arg0)));
        }
      else if (CTreeNodeCode(arg0) == C_NEGATE_EXPR)
        return CTreeExprOperand(arg0, 0);

      /* Convert - (a - b) to (b - a) for non-floating-point.  */
      else if (CTreeNodeCode(arg0) == C_MINUS_EXPR && ! FLOAT_TYPE_P (type))
        return c_tree_build_binary_typed_expr(C_MINUS_EXPR, type, CTreeExprOperand(arg0, 1),
                      CTreeExprOperand(arg0, 0));

      return t;

    case C_ABS_EXPR:
      if (wins)
        {
          if (CTreeNodeCode(arg0) == C_INT_CST_NODE)
            {
              if (! CIsTreeNodeUnsigned(type)
                  && CTreeIntCstHigh(arg0) < 0)
                {
                  C_HOST_WIDE_INT low, high;
                  int overflow = c_neg_double (CTreeIntCstLow(arg0),
                                             CTreeIntCstHigh(arg0),
                                             &low, &high);
                  t = c_tree_build_int_2(low, high);
                  CTreeType(t) = type;
                  if ( CIsTreeNodeOverflow(arg0)
                       | c_force_fit_type(t, overflow))
                     CSetTreeNodeOverflow(t);
                  if (CIsTreeNodeOverflow(t) || CIsTreeNodeCstOverflow(arg0))
                    CSetTreeNodeCstOverflow(t);
                }
            }
          else if (CTreeNodeCode(arg0) == C_REAL_CST_NODE)
            {
              if (REAL_VALUE_NEGATIVE ( CTreeRealCstValue(arg0)))
                t = c_tree_build_real (type,
                                REAL_VALUE_NEGATE( CTreeRealCstValue(arg0)));
            }
        }
      else if (CTreeNodeCode(arg0) == C_ABS_EXPR || CTreeNodeCode(arg0) == C_NEGATE_EXPR)
        return c_tree_build_unary_typed_expr(C_ABS_EXPR, type, CTreeExprOperand(arg0, 0));
      return t;

    case C_CONJ_EXPR:
      if (CTreeNodeCode(CTreeType(arg0)) != C_COMPLEX_TYPE)
        return arg0;
      else if (CTreeNodeCode(arg0) == C_COMPLEX_EXPR)
        return c_tree_build_binary_typed_expr(C_COMPLEX_EXPR, CTreeType(arg0),
                      CTreeExprOperand(arg0, 0),
                      c_fold(c_tree_build_unary_typed_expr(C_NEGATE_EXPR,
                                    CTreeType(CTreeType(arg0)),
                                    CTreeExprOperand(arg0, 1))));
      else if (CTreeNodeCode(arg0) == C_COMPLEX_NODE)
        return c_tree_build_complex(type, CTreeExprOperand(arg0, 0),
                              c_fold(c_tree_build_unary_typed_expr(C_NEGATE_EXPR,
                                            CTreeType(CTreeType(arg0)),
                                            CTreeExprOperand(arg0, 1))));
      else if (CTreeNodeCode(arg0) == C_PLUS_EXPR || CTreeNodeCode(arg0) == C_MINUS_EXPR)
        return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type,
                            c_fold(c_tree_build_unary_typed_expr(C_CONJ_EXPR, type,
                                          CTreeExprOperand(arg0, 0))),
                            c_fold(c_tree_build_unary_typed_expr(C_CONJ_EXPR,
                                          type, CTreeExprOperand(arg0, 1)))));
      else if (CTreeNodeCode(arg0) == C_CONJ_EXPR)
        return CTreeExprOperand(arg0, 0);
      return t;

    case C_BIT_NOT_EXPR:
      if (wins)
        {
          t = c_tree_build_int_2(~ CTreeIntCstLow(arg0), ~ CTreeIntCstHigh(arg0));
          CTreeType(t) = type;
          c_force_fit_type(t, 0);
          if ( CIsTreeNodeOverflow(arg0)) CSetTreeNodeOverflow(t);
          if ( CIsTreeNodeCstOverflow(arg0)) CSetTreeNodeCstOverflow(arg0);
        }
      else if (CTreeNodeCode(arg0) == C_BIT_NOT_EXPR)
        return CTreeExprOperand(arg0, 0);
      return t;

    case C_PLUS_EXPR:
      /* A + (-B) -> A - B */
      if (CTreeNodeCode(arg1) == C_NEGATE_EXPR)
        return c_fold(
            c_tree_build_binary_typed_expr(
                C_MINUS_EXPR, type, arg0, CTreeExprOperand(arg1, 0)));
      else if (! FLOAT_TYPE_P (type))
        {
          if (c_tree_is_integer_zerop(arg1))
            return c_get_non_lvalue(c_convert_expr(type, arg0));

          /* If we are adding two BIT_AND_EXPR's, both of which are and'ing
             with a constant, and the two constants have no bits in common,
             we should treat this as a BIT_IOR_EXPR since this may produce more
             simplifications.  */
          if (CTreeNodeCode(arg0) == C_BIT_AND_EXPR
              && CTreeNodeCode(arg1) == C_BIT_AND_EXPR
              && CTreeNodeCode(CTreeExprOperand(arg0, 1)) == C_INT_CST_NODE
              && CTreeNodeCode(CTreeExprOperand(arg1, 1)) == C_INT_CST_NODE
              && c_tree_is_integer_zerop(
                  c_const_binop(C_BIT_AND_EXPR, 
                      CTreeExprOperand(arg0, 1), CTreeExprOperand(arg1, 1), 0)))
            {
              code = C_BIT_IOR_EXPR;
              goto bit_ior;
            }

          /* (A * C) + (B * C) -> (A+B) * C.  Since we are most concerned
             about the case where C is a constant, just try one of the
             four possibilities.  */

          if (CTreeNodeCode(arg0) == C_MULT_EXPR && 
              CTreeNodeCode(arg1) == C_MULT_EXPR
              && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                  CTreeExprOperand(arg1, 1), 0))
            return c_fold(c_tree_build_binary_typed_expr(C_MULT_EXPR, type,
                                c_fold(c_tree_build_binary_typed_expr(C_PLUS_EXPR, type,
                                             CTreeExprOperand(arg0, 0),
                                             CTreeExprOperand(arg1, 0))),
                                CTreeExprOperand(arg0, 1)));
        }
      /* In IEEE floating point, x+0 may not equal x.  */
      else if ((
# ifdef LUDO_NO_SKIP
           TARGET_FLOAT_FORMAT != IEEE_FLOAT_FORMAT
# else
           1
# endif
                || c_flag_fast_math)
               && c_tree_is_real_zerop(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));
    associate:
      /* In most languages, can't associate operations on floats
         through parentheses.  Rather than remember where the parentheses
         were, we don't associate floats at all.  It shouldn't matter much.
         However, associating multiplications is only very slightly
         inaccurate, so do that if -ffast-math is specified.  */
      if (FLOAT_TYPE_P (type)
          && ! (c_flag_fast_math && code == C_MULT_EXPR))
        goto binary;

      /* The varsign == -1 cases happen only for addition and subtraction.
         It says that the arg that was split was really CON minus VAR.
         The rest of the code applies to all associative operations.  */
      if (!wins)
        {
          c_tree_node *var;
          c_tree_node *con;
          int varsign;

          if (c_split_tree(arg0, code, &var, &con, &varsign))
            {
              if (varsign == -1)
                {
                  /* EXPR is (CON-VAR) +- ARG1.  */
                  /* If it is + and VAR==ARG1, return just CONST.  */
                  if (code == C_PLUS_EXPR && c_is_operand_equal_p(var, arg1, 0))
                    return c_convert_expr(CTreeType(t), con);
                    
                  /* If ARG0 is a constant, don't change things around;
                     instead keep all the constant computations together.  */

                  if (CIsTreeNodeConstant(arg0))
                    return t;

                  /* Otherwise return (CON +- ARG1) - VAR.  */
                  t = c_tree_build_binary_typed_expr(C_MINUS_EXPR, type,
                        c_fold(c_tree_build_binary_typed_expr(code, type, con, arg1)), var);
                }
              else
                {
                  /* EXPR is (VAR+CON) +- ARG1.  */
                  /* If it is - and VAR==ARG1, return just CONST.  */
                  if (code == C_MINUS_EXPR && c_is_operand_equal_p(var, arg1, 0))
                    return c_convert_expr(CTreeType(t), con);
                    
                  /* If ARG0 is a constant, don't change things around;
                     instead keep all the constant computations together.  */

                  if (CIsTreeNodeConstant(arg0))
                    return t;

                  /* Otherwise return VAR +- (ARG1 +- CON).  */
                  tem = c_fold(c_tree_build_binary_typed_expr(code, type, arg1, con));
                  t = c_tree_build_binary_typed_expr(code, type, var, tem);

                  if (c_tree_is_integer_zerop(tem)
                      && (code == C_PLUS_EXPR || code == C_MINUS_EXPR))
                    return c_convert_expr(type, var);
                  /* If we have x +/- (c - d) [c an explicit integer]
                     change it to x -/+ (d - c) since if d is relocatable
                     then the latter can be a single immediate insn
                     and the former cannot.  */
                  if (CTreeNodeCode(tem) == C_MINUS_EXPR
                      && CTreeNodeCode(CTreeExprOperand(tem, 0)) == C_INT_CST_NODE)
                    {
                      c_tree_node *tem1 = CTreeExprOperand(tem, 1);
                      CTreeExprOperand(tem, 1) = CTreeExprOperand(tem, 0);
                      CTreeExprOperand(tem, 0) = tem1;
                      CTreeNodeCode(t) = (code == C_PLUS_EXPR ? C_MINUS_EXPR : C_PLUS_EXPR);
                    }
                }
              return t;
            }

          if (c_split_tree(arg1, code, &var, &con, &varsign))
            {
              if (CIsTreeNodeConstant(arg1))
                return t;

              if (varsign == -1)
                CTreeNodeCode(t) = (code == C_PLUS_EXPR ? C_MINUS_EXPR : C_PLUS_EXPR);

              /* EXPR is ARG0 +- (CON +- VAR).  */
              if (CTreeNodeCode(t) == C_MINUS_EXPR
                  && c_is_operand_equal_p(var, arg0, 0))
                {
                  /* If VAR and ARG0 cancel, return just CON or -CON.  */
                  if (code == C_PLUS_EXPR)
                    return c_convert_expr(CTreeType(t), con);
                  return c_fold(c_tree_build_unary_typed_expr(C_NEGATE_EXPR, CTreeType(t),
                                       c_convert_expr(CTreeType(t), con)));
                }

              t = c_tree_build_binary_typed_expr(CTreeNodeCode(t), type,
                 c_fold(c_tree_build_binary_typed_expr(code, CTreeType(t), arg0, con)), var);

              if (c_tree_is_integer_zerop(CTreeExprOperand(t, 0))
                  && CTreeNodeCode(t) == C_PLUS_EXPR)
                return c_convert_expr(CTreeType(t), var);
              return t;
            }
        }
    binary:
# ifdef LUDO_NO_SKIP
#if defined (REAL_IS_NOT_DOUBLE) && ! defined (REAL_ARITHMETIC)
      if (CTreeNodeCode(arg1) == C_REAL_CST_NODE)
        return t;
#endif /* REAL_IS_NOT_DOUBLE, and no REAL_ARITHMETIC */
# endif
      if (wins)
        t1 = c_const_binop(code, arg0, arg1, 0);
      if (t1 != NULL)
        {
          /* The return value should always have
             the same type as the original expression.  */
          if (CTreeType(t1) != CTreeType(t))
            t1 = c_convert_expr(CTreeType(t), t1);

          return t1;
        }
      return t;

    case C_MINUS_EXPR:
      if (! FLOAT_TYPE_P (type))
        {
          if (! wins && c_tree_is_integer_zerop(arg0))
            return c_tree_build_unary_typed_expr(C_NEGATE_EXPR, type, arg1);
          if (c_tree_is_integer_zerop(arg1))
            return c_get_non_lvalue(c_convert_expr(type, arg0));

          /* (A * C) - (B * C) -> (A-B) * C.  Since we are most concerned
             about the case where C is a constant, just try one of the
             four possibilities.  */

          if (CTreeNodeCode(arg0) == C_MULT_EXPR && CTreeNodeCode(arg1) == C_MULT_EXPR
              && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                  CTreeExprOperand(arg1, 1), 0))
            return c_fold(c_tree_build_binary_typed_expr(C_MULT_EXPR, type,
                                c_fold(c_tree_build_binary_typed_expr(C_MINUS_EXPR, type,
                                             CTreeExprOperand(arg0, 0),
                                             CTreeExprOperand(arg1, 0))),
                                CTreeExprOperand(arg0, 1)));
        }
      /* Convert A - (-B) to A + B.  */
      else if (CTreeNodeCode(arg1) == C_NEGATE_EXPR)
        return c_fold(
            c_tree_build_binary_typed_expr(
                C_PLUS_EXPR, type, arg0, CTreeExprOperand(arg1, 0)));

      else if (
# ifdef LUDO_NO_SKIP
          TARGET_FLOAT_FORMAT != IEEE_FLOAT_FORMAT
# else
          1
# endif
               || c_flag_fast_math)
        {
          /* Except with IEEE floating point, 0-x equals -x.  */
          if (! wins && c_tree_is_real_zerop(arg0))
            return c_tree_build_unary_typed_expr(C_NEGATE_EXPR, type, arg1);
          /* Except with IEEE floating point, x-0 equals x.  */
          if (c_tree_is_real_zerop(arg1))
            return c_get_non_lvalue(c_convert_expr(type, arg0));
        }

      /* Fold &x - &x.  This can happen from &x.foo - &x. 
         This is unsafe for certain floats even in non-IEEE formats.
         In IEEE, it is unsafe because it does wrong for NaNs.
         Also note that c_is_operand_equal_pis always false if an operand
         is volatile.  */

      if ((! FLOAT_TYPE_P (type) || c_flag_fast_math)
          && c_is_operand_equal_p(arg0, arg1, 0))
        return c_convert_expr(type, c_integer_zero_node);

      goto associate;

    case C_MULT_EXPR:
      if (! FLOAT_TYPE_P (type))
        {
          if (c_tree_is_integer_zerop(arg1))
            return c_omit_one_operand(type, arg1, arg0);
          if (c_tree_is_integer_onep(arg1))
            return c_get_non_lvalue(c_convert_expr(type, arg0));

          /* ((A / C) * C) is A if the division is an
             EXACT_DIV_EXPR.   Since C is normally a constant,
             just check for one of the four possibilities.  */

          if (CTreeNodeCode(arg0) == C_EXACT_DIV_EXPR
              && c_is_operand_equal_p(CTreeExprOperand(arg0, 1), arg1, 0))
            return CTreeExprOperand(arg0, 0);

          /* (a * (1 << b)) is (a << b)  */
          if (CTreeNodeCode(arg1) == C_LSHIFT_EXPR
              && c_tree_is_integer_onep(CTreeExprOperand(arg1, 0)))
            return c_fold(c_tree_build_binary_typed_expr(C_LSHIFT_EXPR, type, arg0,
                                CTreeExprOperand(arg1, 1)));
          if (CTreeNodeCode(arg0) == C_LSHIFT_EXPR
              && c_tree_is_integer_onep(CTreeExprOperand(arg0, 0)))
            return c_fold(c_tree_build_binary_typed_expr(C_LSHIFT_EXPR, type, arg1,
                                CTreeExprOperand(arg0, 1)));
        }
      else
        {
          /* x*0 is 0, except for IEEE floating point.  */
          if ((
# ifdef LUDO_NO_SKIP
              TARGET_FLOAT_FORMAT != IEEE_FLOAT_FORMAT
# else
              1
# endif
               || c_flag_fast_math)
              && c_tree_is_real_zerop(arg1))
            return c_omit_one_operand(type, arg1, arg0);
          /* In IEEE floating point, x*1 is not equivalent to x for snans.
             However, ANSI says we can drop signals,
             so we can do this anyway.  */
          if (c_tree_is_real_onep(arg1))
            return c_get_non_lvalue(c_convert_expr(type, arg0));
          /* x*2 is x+x */
          if (! wins && c_tree_is_real_twop(arg1) && c_current_function_decl != 0)
            {
# ifdef LUDO_NO_SKIP
              c_tree_node *arg = save_expr (arg0);
# else
              c_tree_node *arg = arg0;
# endif
              return c_tree_build_binary_typed_expr(C_PLUS_EXPR, type, arg, arg);
            }
        }
      goto associate;

    case C_BIT_IOR_EXPR:
    bit_ior:
      {
        int code0, code1;

      if (c_tree_is_integer_all_onesp(arg1))
        return c_omit_one_operand(type, arg1, arg0);
      if (c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));
      t1 = c_distribute_bit_expr(code, type, arg0, arg1);
      if (t1 != NULL)
        return t1;

      /* (A << C1) | (A >> C2) if A is unsigned and C1+C2 is the size of A
         is a rotate of A by C1 bits.  */
      /* (A << B) | (A >> (Z - B)) if A is unsigned and Z is the size of A
         is a rotate of A by B bits.  */

      code0 = CTreeNodeCode(arg0);
      code1 = CTreeNodeCode(arg1);
      if (((code0 == C_RSHIFT_EXPR && code1 == C_LSHIFT_EXPR)
          || (code1 == C_RSHIFT_EXPR && code0 == C_LSHIFT_EXPR))
          && c_is_operand_equal_p(CTreeExprOperand(arg0, 0), CTreeExprOperand(arg1,0), 0)
          && CIsTreeNodeUnsigned(CTreeType(CTreeExprOperand(arg0, 0))))
        {
          c_tree_node *tree01;
          c_tree_node *tree11;
          int code01, code11;

          tree01 = CTreeExprOperand(arg0, 1);
          tree11 = CTreeExprOperand(arg1, 1);
          code01 = CTreeNodeCode(tree01);
          code11 = CTreeNodeCode(tree11);
          if (code01 == C_INT_CST_NODE
            && code11 == C_INT_CST_NODE
            && CTreeIntCstHigh(tree01) == 0
            && CTreeIntCstHigh(tree11) == 0
            && ((CTreeIntCstLow(tree01) + CTreeIntCstLow(tree11))
              == CTreeTypePrecision(CTreeType(CTreeExprOperand(arg0, 0)))))
            return 
              c_tree_build_binary_typed_expr(C_LROTATE_EXPR, type, CTreeExprOperand(arg0, 0),
                      code0 == C_LSHIFT_EXPR ? tree01 : tree11);
          else if (code11 == C_MINUS_EXPR
                && CTreeNodeCode(CTreeExprOperand(tree11, 0)) == C_INT_CST_NODE
                && CTreeIntCstHigh(CTreeExprOperand(tree11, 0)) == 0
                && CTreeIntCstLow(CTreeExprOperand(tree11, 0))
                  == CTreeTypePrecision(CTreeType(CTreeExprOperand(arg0, 0)))
                && c_is_operand_equal_p(tree01, CTreeExprOperand(tree11, 1), 0))
            return 
            c_tree_build_binary_typed_expr(
                code0 == C_LSHIFT_EXPR ? C_LROTATE_EXPR : C_RROTATE_EXPR,
                        type, CTreeExprOperand(arg0, 0), tree01);
          else if (code01 == C_MINUS_EXPR
                && CTreeNodeCode(CTreeExprOperand(tree01, 0)) == C_INT_CST_NODE
                && CTreeIntCstHigh(CTreeExprOperand(tree01, 0)) == 0
                && CTreeIntCstLow(CTreeExprOperand(tree01, 0))
                  == CTreeTypePrecision(CTreeType(CTreeExprOperand(arg0, 0)))
                && c_is_operand_equal_p(tree11, CTreeExprOperand(tree01, 1), 0))
            return
            c_tree_build_binary_typed_expr(
                code0 != C_LSHIFT_EXPR ? C_LROTATE_EXPR : C_RROTATE_EXPR,
                        type, CTreeExprOperand(arg0, 0), tree11);
        }

      goto associate;
      }

    case C_BIT_XOR_EXPR:
      if (c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));
      if (c_tree_is_integer_all_onesp(arg1))
        return c_fold(c_tree_build_unary_typed_expr(C_BIT_NOT_EXPR, type, arg0));
      goto associate;

    case C_BIT_AND_EXPR:
    bit_and:
      if (c_tree_is_integer_all_onesp(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));
      if (c_tree_is_integer_zerop(arg1))
        return c_omit_one_operand(type, arg1, arg0);
      t1 = c_distribute_bit_expr(code, type, arg0, arg1);
      if (t1 != NULL)
        return t1;
      /* Simplify ((int)c & 0x377) into (int)c, if c is unsigned char.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE && CTreeNodeCode(arg1) == C_NOP_EXPR
          && CIsTreeNodeUnsigned(CTreeType(CTreeExprOperand(arg1, 0))))
        {
          int prec = CTreeTypePrecision(CTreeType(CTreeExprOperand(arg1, 0)));
          if (prec < C_BITS_PER_WORD && prec < C_HOST_BITS_PER_WIDE_INT
              && (~CTreeIntCstLow(arg0)
                  & (((C_HOST_WIDE_INT) 1 << prec) - 1)) == 0)
            return c_tree_build_unary_typed_expr(C_NOP_EXPR, type, CTreeExprOperand(arg1, 0));
        }
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE && CTreeNodeCode(arg0) == C_NOP_EXPR
          && CIsTreeNodeUnsigned(CTreeType(CTreeExprOperand(arg0, 0))))
        {
          int prec = CTreeTypePrecision(CTreeType(CTreeExprOperand(arg0, 0)));
          if (prec < C_BITS_PER_WORD && prec < C_HOST_BITS_PER_WIDE_INT
              && (~CTreeIntCstLow(arg1)
                  & (((C_HOST_WIDE_INT) 1 << prec) - 1)) == 0)
            return c_tree_build_unary_typed_expr(C_NOP_EXPR, type, CTreeExprOperand(arg0, 0));
        }
      goto associate;

    case C_BIT_ANDTC_EXPR:
      if (c_tree_is_integer_all_onesp(arg0))
        return c_get_non_lvalue(c_convert_expr(type, arg1));
      if (c_tree_is_integer_zerop(arg0))
        return c_omit_one_operand(type, arg0, arg1);
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE)
        {
          arg1 = c_fold(c_tree_build_unary_typed_expr(C_BIT_NOT_EXPR, type, arg1));
          code = C_BIT_AND_EXPR;
          goto bit_and;
        }
      goto binary;

    case C_RDIV_EXPR:
      /* In most cases, do nothing with a divide by zero.  */
#if !defined (REAL_IS_NOT_DOUBLE) || defined (REAL_ARITHMETIC)
#ifndef REAL_INFINITY
      if (CTreeNodeCode(arg1) == C_REAL_CST_NODE && c_tree_is_real_zerop(arg1))
        return t;
#endif
#endif /* not REAL_IS_NOT_DOUBLE, or REAL_ARITHMETIC */

      /* In IEEE floating point, x/1 is not equivalent to x for snans.
         However, ANSI says we can drop signals, so we can do this anyway.  */
      if (c_tree_is_real_onep(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));

      /* If ARG1 is a constant, we can c_convert_exprthis to a multiply by the
         reciprocal.  This does not have the same rounding properties,
         so only do this if -ffast-math.  We can actually always safely
         do it if ARG1 is a power of two, but it's hard to tell if it is
         or not in a portable manner.  */
      if (CTreeNodeCode(arg1) == C_REAL_CST_NODE)
        {
          if (c_flag_fast_math
              && 0 != (tem = c_const_binop(code, 
                  c_tree_build_real(type, 1.0), arg1, 0)))
            return c_fold(c_tree_build_binary_typed_expr(C_MULT_EXPR, type, arg0, tem));
          /* Find the reciprocal if optimizing and the result is exact. */
# ifdef LUDO_NO_SKIP
          else if (optimize)
            {
              REAL_VALUE_TYPE r;
              r = CTreeRealCstValue(arg1);
              if (c_exact_real_inverse(TYPE_MODE(TREE_TYPE(arg0)), &r))
                  {
                    tem = c_tree_build_real(type, r);
                    return 
                    c_fold(c_tree_build_binary_typed_expr(C_MULT_EXPR, type, arg0, tem));
                  }
            }
# endif
        }
      goto binary;

    case C_TRUNC_DIV_EXPR:
    case C_ROUND_DIV_EXPR:
    case C_FLOOR_DIV_EXPR:
    case C_CEIL_DIV_EXPR:
    case C_EXACT_DIV_EXPR:
      if (c_tree_is_integer_onep(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));
      if (c_tree_is_integer_zerop(arg1))
        return t;

      /* If we have ((a / C1) / C2) where both division are the same type, try
         to simplify.  First see if C1 * C2 overflows or not.  */
      if (CTreeNodeCode(arg0) == code && CTreeNodeCode(arg1) == C_INT_CST_NODE
          && CTreeNodeCode(CTreeExprOperand(arg0, 1)) == C_INT_CST_NODE)
        {
          c_tree_node *new_divisor;

          new_divisor = c_const_binop(C_MULT_EXPR, CTreeExprOperand(arg0, 1), arg1, 0);
          tem = c_const_binop(C_FLOOR_DIV_EXPR, new_divisor, arg1, 0);

          if (CTreeIntCstLow(CTreeExprOperand(arg0, 1)) == CTreeIntCstLow(tem)
              && CTreeIntCstHigh(CTreeExprOperand(arg0, 1)) == CTreeIntCstHigh(tem))
            {
              /* If no overflow, divide by C1*C2.  */
              return
              c_fold(
                c_tree_build_binary_typed_expr(
                    code, type, CTreeExprOperand(arg0, 0), new_divisor));
            }
        }

      /* Look for ((a * C1) / C3) or (((a * C1) + C2) / C3),
         where C1 % C3 == 0 or C3 % C1 == 0.  We can simplify these
         expressions, which often appear in the offsets or sizes of
         objects with a varying size.  Only deal with positive divisors
         and multiplicands.   If C2 is negative, we must have C2 % C3 == 0.

         Look for NOPs and SAVE_EXPRs inside.  */

      if (CTreeNodeCode(arg1) == C_INT_CST_NODE
          && c_tree_is_int_cst_sgn(arg1) >= 0)
        {
          int have_save_expr = 0;
          c_tree_node *c2 = c_integer_zero_node;
          c_tree_node *xarg0 = arg0;

          if (CTreeNodeCode(xarg0) == C_SAVE_EXPR 
# ifdef LUDO_NO_SKIP
              && SAVE_EXPR_RTL (xarg0) == 0
# endif
              )
            have_save_expr = 1, xarg0 = CTreeExprOperand(xarg0, 0);

          CStripNops(xarg0);

          if (CTreeNodeCode(xarg0) == C_PLUS_EXPR
              && CTreeNodeCode(CTreeExprOperand(xarg0, 1)) == C_INT_CST_NODE)
            c2 = CTreeExprOperand(xarg0, 1), xarg0 = CTreeExprOperand(xarg0, 0);
          else if (CTreeNodeCode(xarg0) == C_MINUS_EXPR
                   && CTreeNodeCode(CTreeExprOperand(xarg0, 1)) == C_INT_CST_NODE
                   /* If we are doing this computation unsigned, the negate
                      is incorrect.  */
                   && ! CIsTreeNodeUnsigned(type))
            {
              c2 = 
                c_fold( c_tree_build_unary_typed_expr(
                        C_NEGATE_EXPR, type, CTreeExprOperand(xarg0, 1)));
              xarg0 = CTreeExprOperand(xarg0, 0);
            }

          if (CTreeNodeCode(xarg0) == C_SAVE_EXPR 
# ifdef LUDO_NO_SKIP
              && SAVE_EXPR_RTL (xarg0) == 0
# endif
              )
            have_save_expr = 1, xarg0 = CTreeExprOperand(xarg0, 0);

          CStripNops(xarg0);

          if (CTreeNodeCode(xarg0) == C_MULT_EXPR
              && CTreeNodeCode(CTreeExprOperand(xarg0, 1)) == C_INT_CST_NODE
              && c_tree_is_int_cst_sgn(CTreeExprOperand(xarg0, 1)) >= 0
              && (c_tree_is_integer_zerop(c_const_binop(C_TRUNC_MOD_EXPR,
                                              CTreeExprOperand(xarg0, 1), arg1, 1))
                  || c_tree_is_integer_zerop(c_const_binop(C_TRUNC_MOD_EXPR, arg1,
                                                 CTreeExprOperand(xarg0, 1), 1)))
              && (c_tree_is_int_cst_sgn(c2) >= 0
                  || c_tree_is_integer_zerop(c_const_binop(C_TRUNC_MOD_EXPR, c2,
                                                 arg1, 1))))
            {
              c_tree_node *outer_div = c_integer_one_node;
              c_tree_node *c1 = CTreeExprOperand(xarg0, 1);
              c_tree_node *c3 = arg1;

              /* If C3 > C1, set them equal and do a divide by
                 C3/C1 at the end of the operation.  */
              if (c_tree_is_int_cst_lt(c1, c3))
                outer_div = c_const_binop(code, c3, c1, 0), c3 = c1;
                
              /* The result is A * (C1/C3) + (C2/C3).  */
              t = c_fold(c_tree_build_binary_typed_expr(C_PLUS_EXPR, type,
                               c_fold(c_tree_build_binary_typed_expr(C_MULT_EXPR, type,
                                            CTreeExprOperand(xarg0, 0),
                                            c_const_binop(code, c1, c3, 1))),
                               c_const_binop(code, c2, c3, 1)));

              if (! c_tree_is_integer_onep(outer_div))
                t = c_fold( c_tree_build_binary_typed_expr(
                        code, type, t, c_convert_expr(type, outer_div)));

# ifdef LUDO_NO_SKIP
              if (have_save_expr)
                t = save_expr (t);
# endif
              return t;
            }
        }

      goto binary;

    case C_CEIL_MOD_EXPR:
    case C_FLOOR_MOD_EXPR:
    case C_ROUND_MOD_EXPR:
    case C_TRUNC_MOD_EXPR:
      if (c_tree_is_integer_onep(arg1))
        return c_omit_one_operand(type, c_integer_zero_node, arg0);
      if (c_tree_is_integer_zerop(arg1))
        return t;

      /* Look for ((a * C1) % C3) or (((a * C1) + C2) % C3),
         where C1 % C3 == 0.  Handle similarly to the division case,
         but don't bother with SAVE_EXPRs.  */

      if (CTreeNodeCode(arg1) == C_INT_CST_NODE
          && ! c_tree_is_integer_zerop(arg1))
        {
          c_tree_node *c2 = c_integer_zero_node;
          c_tree_node *xarg0 = arg0;

          if (CTreeNodeCode(xarg0) == C_PLUS_EXPR
              && CTreeNodeCode(CTreeExprOperand(xarg0, 1)) == C_INT_CST_NODE)
            c2 = CTreeExprOperand(xarg0, 1), xarg0 = CTreeExprOperand(xarg0, 0);
          else if (CTreeNodeCode(xarg0) == C_MINUS_EXPR
                   && CTreeNodeCode(CTreeExprOperand(xarg0, 1)) == C_INT_CST_NODE
                   && ! CIsTreeNodeUnsigned(type))
            {
              c2 = c_fold(c_tree_build_unary_typed_expr(
                  C_NEGATE_EXPR, type, CTreeExprOperand(xarg0, 1)));
              xarg0 = CTreeExprOperand(xarg0, 0);
            }

          CStripNops(xarg0);

          if (CTreeNodeCode(xarg0) == C_MULT_EXPR
              && CTreeNodeCode(CTreeExprOperand(xarg0, 1)) == C_INT_CST_NODE
              && c_tree_is_integer_zerop(c_const_binop(C_TRUNC_MOD_EXPR,
                                             CTreeExprOperand(xarg0, 1),
                                             arg1, 1))
              && c_tree_is_int_cst_sgn(c2) >= 0)
            /* The result is (C2%C3).  */
            return c_omit_one_operand(type, c_const_binop(code, c2, arg1, 1),
                                     CTreeExprOperand(xarg0, 0));
        }

      goto binary;

    case C_LSHIFT_EXPR:
    case C_RSHIFT_EXPR:
    case C_LROTATE_EXPR:
    case C_RROTATE_EXPR:
      if (c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(c_convert_expr(type, arg0));
      /* Since negative shift count is not well-defined,
         don't try to compute it in the compiler.  */
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE && c_tree_is_int_cst_sgn(arg1) < 0)
        return t;
      /* Rewrite an LROTATE_EXPR by a constant into an
         RROTATE_EXPR by a new constant.  */
# ifdef LUDO_NO_SKIP
      if (code == C_LROTATE_EXPR && CTreeNodeCode(arg1) == C_INT_CST_NODE)
        {
          CTreeNodeCode(t) = C_RROTATE_EXPR;
          code = C_RROTATE_EXPR;
          CTreeExprOperand(t, 1) = arg1
            = c_const_binop(C_MINUS_EXPR,
               c_convert_expr(CTreeType(arg1),
                        c_tree_build_int_2(GET_MODE_BITSIZE(CTreeTypeMode(type)), 0)),
               arg1, 0);
          if (c_tree_is_int_cst_sgn(arg1) < 0)
            return t;
        }
# endif

      /* If we have a rotate of a bit operation with the rotate count and
         the second operand of the bit operation both constant,
         permute the two operations.  */
      if (code == C_RROTATE_EXPR && CTreeNodeCode(arg1) == C_INT_CST_NODE
          && (CTreeNodeCode(arg0) == C_BIT_AND_EXPR
              || CTreeNodeCode(arg0) == C_BIT_ANDTC_EXPR
              || CTreeNodeCode(arg0) == C_BIT_IOR_EXPR
              || CTreeNodeCode(arg0) == C_BIT_XOR_EXPR)
          && CTreeNodeCode(CTreeExprOperand(arg0, 1)) == C_INT_CST_NODE)
        return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type,
                            c_fold(c_tree_build_binary_typed_expr(code, type,
                                         CTreeExprOperand(arg0, 0), arg1)),
                            c_fold(c_tree_build_binary_typed_expr(code, type,
                                         CTreeExprOperand(arg0, 1), arg1))));

# ifdef LUDO_NO_SKIP
      /* Two consecutive rotates adding up to the width of the mode can
         be ignored.  */
      if (code == C_RROTATE_EXPR && CTreeNodeCode(arg1) == C_INT_CST_NODE
          && CTreeNodeCode(arg0) == C_RROTATE_EXPR
          && CTreeNodeCode(CTreeExprOperand(arg0, 1)) == C_INT_CST_NODE
          && CTreeIntCstHigh(arg1) == 0
          && CTreeIntCstHigh(CTreeExprOperand(arg0, 1)) == 0
          && ((CTreeIntCstLow(arg1)
               + CTreeIntCstLow(CTreeExprOperand(arg0, 1)))
              == GET_MODE_BITSIZE(CTreeTypeMode(type))))
        return CTreeExprOperand(arg0, 0);
# endif

      goto binary;

    case C_MIN_EXPR:
      if (c_is_operand_equal_p(arg0, arg1, 0))
        return arg0;
      if (INTEGRAL_TYPE_P (type)
          && c_is_operand_equal_p(arg1, CTreeTypeMinValue(type), 1))
        return c_omit_one_operand(type, arg1, arg0);
      goto associate;

    case C_MAX_EXPR:
      if (c_is_operand_equal_p(arg0, arg1, 0))
        return arg0;
      if (INTEGRAL_TYPE_P (type)
          && c_is_operand_equal_p(arg1, CTreeTypeMaxValue(type), 1))
        return c_omit_one_operand(type, arg1, arg0);
      goto associate;

    case C_TRUTH_NOT_EXPR:
      /* Note that the operand of this must be an int
         and its values must be 0 or 1.
         ("true" is a fixed value perhaps depending on the language,
         but we don't handle values other than 1 correctly yet.)  */
      tem = c_invert_truthvalue(arg0);
      /* Avoid infinite recursion.  */
      if (CTreeNodeCode(tem) == C_TRUTH_NOT_EXPR)
        return t;
      return c_convert_expr(type, tem);

    case C_TRUTH_ANDIF_EXPR:
      /* Note that the operands of this must be ints
         and their values must be 0 or 1.
         ("true" is a fixed value perhaps depending on the language.)  */
      /* If first arg is constant zero, return it.  */
      if (c_tree_is_integer_zerop(arg0))
        return arg0;
    case C_TRUTH_AND_EXPR:
      /* If either arg is constant true, drop it.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE && ! c_tree_is_integer_zerop(arg0))
        return c_get_non_lvalue(arg1);
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE && ! c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(arg0);
      /* If second arg is constant zero, result is zero, but first arg
         must be evaluated.  */
      if (c_tree_is_integer_zerop(arg1))
        return c_omit_one_operand(type, arg1, arg0);
      /* Likewise for first arg, but note that only the TRUTH_AND_EXPR
         case will be handled here.  */
      if (c_tree_is_integer_zerop(arg0))
        return c_omit_one_operand(type, arg0, arg1);

    truth_andor:
      /* We only do these simplifications if we are optimizing.  */
# ifdef LUDO_NO_SKIP
      if (!optimize)
        return t;

      /* Check for things like (A || B) && (A || C).  We can c_convert_exprthis
         to A || (B && C).  Note that either operator can be any of the four
         truth and/or operations and the transformation will still be
         valid.   Also note that we only care about order for the
         ANDIF and ORIF operators.  If B contains side effects, this
         might change the truth-value of A. */
      if (CTreeNodeCode(arg0) == CTreeNodeCode(arg1)
          && (CTreeNodeCode(arg0) == TRUTH_ANDIF_EXPR
              || CTreeNodeCode(arg0) == TRUTH_ORIF_EXPR
              || CTreeNodeCode(arg0) == TRUTH_AND_EXPR
              || CTreeNodeCode(arg0) == TRUTH_OR_EXPR)
          && ! TREE_SIDE_EFFECTS (CTreeExprOperand(arg0, 1)))
        {
          c_tree_node *a00 = CTreeExprOperand(arg0, 0);
          c_tree_node *a01 = CTreeExprOperand(arg0, 1);
          c_tree_node *a10 = CTreeExprOperand(arg1, 0);
          c_tree_node *a11 = CTreeExprOperand(arg1, 1);
          int commutative = ((CTreeNodeCode(arg0) == TRUTH_OR_EXPR
                              || CTreeNodeCode(arg0) == TRUTH_AND_EXPR)
                             && (code == TRUTH_AND_EXPR
                                 || code == TRUTH_OR_EXPR));

          if (c_is_operand_equal_p(a00, a10, 0))
            return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type, a00,
                                c_fold(c_tree_build_binary_typed_expr(code, type, a01, a11))));
          else if (commutative && c_is_operand_equal_p(a00, a11, 0))
            return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type, a00,
                                c_fold(c_tree_build_binary_typed_expr(code, type, a01, a10))));
          else if (commutative && c_is_operand_equal_p(a01, a10, 0))
            return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type, a01,
                                c_fold(c_tree_build_binary_typed_expr(code, type, a00, a11))));

          /* This case if tricky because we must either have commutative
             operators or else A10 must not have side-effects.  */

          else if ((commutative || ! TREE_SIDE_EFFECTS (a10))
                   && c_is_operand_equal_p(a01, a11, 0))
            return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type,
                                c_fold(c_tree_build_binary_typed_expr(code, type, a00, a10)),
                                a01));
        }

      /* See if we can c_tree_build_binary_typed_expra range comparison.  */
      if (0 != (tem = fold_range_test (t)))
        return tem;

      /* Check for the possibility of merging component references.  If our
         lhs is another similar operation, try to merge its rhs with our
         rhs.  Then try to merge our lhs and rhs.  */
      if (CTreeNodeCode(arg0) == code
          && 0 != (tem = fold_truthop (code, type,
                                       CTreeExprOperand(arg0, 1), arg1)))
        return 
          c_fold(c_tree_build_binary_typed_expr(code, type, CTreeExprOperand(arg0, 0), tem));

      if ((tem = fold_truthop (code, type, arg0, arg1)) != 0)
        return tem;
# endif

      return t;

    case C_TRUTH_ORIF_EXPR:
      /* Note that the operands of this must be ints
         and their values must be 0 or true.
         ("true" is a fixed value perhaps depending on the language.)  */
      /* If first arg is constant true, return it.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE && ! c_tree_is_integer_zerop(arg0))
        return arg0;
    case C_TRUTH_OR_EXPR:
      /* If either arg is constant zero, drop it.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE && c_tree_is_integer_zerop(arg0))
        return c_get_non_lvalue(arg1);
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE && c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(arg0);
      /* If second arg is constant true, result is true, but we must
         evaluate first arg.  */
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE && ! c_tree_is_integer_zerop(arg1))
        return c_omit_one_operand(type, arg1, arg0);
      /* Likewise for first arg, but note this only occurs here for
         TRUTH_OR_EXPR.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE && ! c_tree_is_integer_zerop(arg0))
        return c_omit_one_operand(type, arg0, arg1);
      goto truth_andor;

    case C_TRUTH_XOR_EXPR:
      /* If either arg is constant zero, drop it.  */
      if (c_tree_is_integer_zerop(arg0))
        return c_get_non_lvalue(arg1);
      if (c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(arg0);
      /* If either arg is constant true, this is a logical inversion.  */
      if (c_tree_is_integer_onep(arg0))
        return c_get_non_lvalue(c_invert_truthvalue(arg1));
      if (c_tree_is_integer_onep(arg1))
        return c_get_non_lvalue(c_invert_truthvalue(arg0));
      return t;

    case C_EQ_EXPR:
    case C_NE_EXPR:
    case C_LT_EXPR:
    case C_GT_EXPR:
    case C_LE_EXPR:
    case C_GE_EXPR:
      /* If one arg is a constant integer, put it last.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE
          && CTreeNodeCode(arg1) != C_INT_CST_NODE)
        {
          CTreeExprOperand(t, 0) = arg1;
          CTreeExprOperand(t, 1) = arg0;
          arg0 = CTreeExprOperand(t, 0);
          arg1 = CTreeExprOperand(t, 1);
          code = c_swap_tree_comparison(code);
          CTreeNodeCode(t) = code;
        }

      /* Convert foo++ == CONST into ++foo == CONST + INCR.
         First, see if one arg is constant; find the constant arg
         and the other one.  */
      {
        c_tree_node *constop = 0;
        c_tree_node *varop;
        int constopnum = -1;

        if (CIsTreeNodeConstant(arg1))
          constopnum = 1, constop = arg1, varop = arg0;
        if (CIsTreeNodeConstant(arg0))
          constopnum = 0, constop = arg0, varop = arg1;

        if (constop && CTreeNodeCode(varop) == C_POSTINCREMENT_EXPR)
          {
            /* This optimization is invalid for ordered comparisons
               if CONST+INCR overflows or if foo+incr might overflow.
               This optimization is invalid for floating point due to rounding.
               For pointer types we assume overflow doesn't happen.  */
            if (POINTER_TYPE_P (CTreeType(varop))
                || (! FLOAT_TYPE_P (CTreeType(varop))
                    && (code == C_EQ_EXPR || code == C_NE_EXPR)))
              {
                c_tree_node *newconst
                  = c_fold(c_tree_build_binary_typed_expr(C_PLUS_EXPR, CTreeType(varop),
                                 constop, CTreeExprOperand(varop, 1)));
                CTreeNodeCode(varop) = C_PREINCREMENT_EXPR;

                /* If VAROP is a reference to a bitfield, we must mask
                   the constant by the width of the field.  */
                if (CTreeNodeCode(CTreeExprOperand(varop, 0)) == C_COMPONENT_REF
                    && CIsTreeDeclBitField(CTreeExprOperand(CTreeExprOperand(varop, 0), 1)))
                  {
                    int size
                      = CTreeIntCstLow(CTreeDeclSize(
                          CTreeExprOperand(CTreeExprOperand(varop, 0), 1)));

                    newconst = c_fold(c_tree_build_binary_typed_expr(C_BIT_AND_EXPR,
                                            CTreeType(varop), newconst,
                                            c_convert_expr(CTreeType(varop),
                                                     c_tree_build_int_2(size, 0))));
                  }
                                                         

                t = c_tree_build_binary_typed_expr(code, type, CTreeExprOperand(t, 0),
                           CTreeExprOperand(t, 1));
                CTreeExprOperand(t, constopnum) = newconst;
                return t;
              }
          }
        else if (constop && CTreeNodeCode(varop) == C_POSTDECREMENT_EXPR)
          {
            if (POINTER_TYPE_P (CTreeType(varop))
                || (! FLOAT_TYPE_P (CTreeType(varop))
                    && (code == C_EQ_EXPR || code == C_NE_EXPR)))
              {
                c_tree_node *newconst
                  = c_fold(c_tree_build_binary_typed_expr(C_MINUS_EXPR, CTreeType(varop),
                                 constop, CTreeExprOperand(varop, 1)));
                CTreeNodeCode(varop) = C_PREDECREMENT_EXPR;

                if (CTreeNodeCode(CTreeExprOperand(varop, 0)) == C_COMPONENT_REF
                    && CIsTreeDeclBitField(CTreeExprOperand(CTreeExprOperand(varop, 0), 1)))
                  {
                    int size
                      = CTreeIntCstLow(CTreeDeclSize(CTreeExprOperand(
                                           CTreeExprOperand(varop, 0), 1)));

                    newconst = c_fold(c_tree_build_binary_typed_expr(C_BIT_AND_EXPR,
                                            CTreeType(varop), newconst,
                                            c_convert_expr(CTreeType(varop),
                                                     c_tree_build_int_2(size, 0))));
                  }
                                                         

                t = c_tree_build_binary_typed_expr(code, type, CTreeExprOperand(t, 0),
                           CTreeExprOperand(t, 1));
                CTreeExprOperand(t, constopnum) = newconst;
                return t;
              }
          }
      }

      /* Change X >= CST to X > (CST - 1) if CST is positive.  */
      if (CTreeNodeCode(arg1) == C_INT_CST_NODE
          && CTreeNodeCode(arg0) != C_INT_CST_NODE
          && c_tree_is_int_cst_sgn(arg1) > 0)
        {
          switch (CTreeNodeCode(t))
            {
            case C_GE_EXPR:
              code = C_GT_EXPR;
              arg1 = c_const_binop(C_MINUS_EXPR, arg1, c_integer_one_node, 0);
              t = c_tree_build_binary_typed_expr(code, type, CTreeExprOperand(t, 0), arg1);
              break;

            case C_LT_EXPR:
              code = C_LE_EXPR;
              arg1 = c_const_binop(C_MINUS_EXPR, arg1, c_integer_one_node, 0);
              t = c_tree_build_binary_typed_expr(code, type, CTreeExprOperand(t, 0), arg1);
              break;

            default:
              break;
            }
        }

      /* If this is an EQ or NE comparison with zero and ARG0 is
         (1 << foo) & bar, c_convert_exprit to (bar >> foo) & 1.  Both require
         two operations, but the latter can be done in one less insn
         on machines that have only two-operand insns or on which a
         constant cannot be the first operand.  */
      if (c_tree_is_integer_zerop(arg1) && (code == C_EQ_EXPR || code == C_NE_EXPR)
          && CTreeNodeCode(arg0) == C_BIT_AND_EXPR)
        {
          if (CTreeNodeCode(CTreeExprOperand(arg0, 0)) == C_LSHIFT_EXPR
              && c_tree_is_integer_onep(CTreeExprOperand(CTreeExprOperand(arg0, 0), 0)))
            return
              c_fold(c_tree_build_binary_typed_expr(code, type,
                           c_tree_build_binary_typed_expr(C_BIT_AND_EXPR, CTreeType(arg0),
                                  c_tree_build_binary_typed_expr(C_RSHIFT_EXPR,
                                         CTreeType(CTreeExprOperand(arg0, 0)),
                                         CTreeExprOperand(arg0, 1),
                                         CTreeExprOperand(CTreeExprOperand(arg0, 0), 1)),
                                  c_convert_expr(CTreeType(arg0),
                                           c_integer_one_node)),
                           arg1));
          else if (CTreeNodeCode(CTreeExprOperand(arg0, 1)) == C_LSHIFT_EXPR
                   && c_tree_is_integer_onep(CTreeExprOperand(CTreeExprOperand(arg0, 1), 0)))
            return
              c_fold(c_tree_build_binary_typed_expr(code, type,
                           c_tree_build_binary_typed_expr(C_BIT_AND_EXPR, CTreeType(arg0),
                                  c_tree_build_binary_typed_expr(C_RSHIFT_EXPR,
                                         CTreeType(CTreeExprOperand(arg0, 1)),
                                         CTreeExprOperand(arg0, 0),
                                         CTreeExprOperand(CTreeExprOperand(arg0, 1), 1)),
                                  c_convert_expr(CTreeType(arg0),
                                           c_integer_one_node)),
                           arg1));
        }

      /* If this is an NE or EQ comparison of zero against the result of a
         signed MOD operation whose second operand is a power of 2, make
         the MOD operation unsigned since it is simpler and equivalent.  */
      if ((code == C_NE_EXPR || code == C_EQ_EXPR)
          && c_tree_is_integer_zerop(arg1)
          && ! CIsTreeNodeUnsigned(CTreeType(arg0))
          && (CTreeNodeCode(arg0) == C_TRUNC_MOD_EXPR
              || CTreeNodeCode(arg0) == C_CEIL_MOD_EXPR
              || CTreeNodeCode(arg0) == C_FLOOR_MOD_EXPR
              || CTreeNodeCode(arg0) == C_ROUND_MOD_EXPR)
          && c_tree_is_integer_pow2p(CTreeExprOperand(arg0, 1)))
        {
          c_tree_node *newtype = c_get_unsigned_type(CTreeType(arg0));
          c_tree_node *newmod = c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), newtype,
                               c_convert_expr(newtype, CTreeExprOperand(arg0, 0)),
                               c_convert_expr(newtype, CTreeExprOperand(arg0, 1)));

          return c_tree_build_binary_typed_expr(
              code, type, newmod, c_convert_expr(newtype, arg1));
        }

      /* If this is an NE comparison of zero with an AND of one, remove the
         comparison since the AND will give the correct value.  */
      if (code == C_NE_EXPR && c_tree_is_integer_zerop(arg1)
          && CTreeNodeCode(arg0) == C_BIT_AND_EXPR
          && c_tree_is_integer_onep(CTreeExprOperand(arg0, 1)))
        return c_convert_expr(type, arg0);

      /* If we have (A & C) == C where C is a power of 2, c_convert_exprthis into
         (A & C) != 0.  Similarly for NE_EXPR.  */
      if ((code == C_EQ_EXPR || code == C_NE_EXPR)
          && CTreeNodeCode(arg0) == C_BIT_AND_EXPR
          && c_tree_is_integer_pow2p(CTreeExprOperand(arg0, 1))
          && c_is_operand_equal_p(CTreeExprOperand(arg0, 1), arg1, 0))
        return 
          c_tree_build_binary_typed_expr(code == C_EQ_EXPR ? C_NE_EXPR : C_EQ_EXPR, type,
                      arg0, c_integer_zero_node);

      /* If X is unsigned, c_convert_exprX < (1 << Y) into X >> Y == 0
         and similarly for >= into !=.  */
      if ((code == C_LT_EXPR || code == C_GE_EXPR)
          && CIsTreeNodeUnsigned(CTreeType(arg0))
          && CTreeNodeCode(arg1) == C_LSHIFT_EXPR
          && c_tree_is_integer_onep(CTreeExprOperand(arg1, 0)))
        return c_tree_build_binary_typed_expr(code == C_LT_EXPR ? C_EQ_EXPR : C_NE_EXPR, type, 
                      c_tree_build_binary_typed_expr(C_RSHIFT_EXPR, CTreeType(arg0), arg0,
                             CTreeExprOperand(arg1, 1)),
                      c_convert_expr(CTreeType(arg0), c_integer_zero_node));

      else if ((code == C_LT_EXPR || code == C_GE_EXPR)
               && CIsTreeNodeUnsigned(CTreeType(arg0))
               && (CTreeNodeCode(arg1) == C_NOP_EXPR
                   || CTreeNodeCode(arg1) == C_CONVERT_EXPR)
               && CTreeNodeCode(CTreeExprOperand(arg1, 0)) == C_LSHIFT_EXPR
               && c_tree_is_integer_onep(CTreeExprOperand(CTreeExprOperand(arg1, 0), 0)))
        return
          c_tree_build_binary_typed_expr(code == C_LT_EXPR ? C_EQ_EXPR : C_NE_EXPR, type,
                 c_convert_expr(CTreeType(arg0),
                          c_tree_build_binary_typed_expr(C_RSHIFT_EXPR, CTreeType(arg0), arg0,
                                 CTreeExprOperand(CTreeExprOperand(arg1, 0), 1))),
                 c_convert_expr(CTreeType(arg0), c_integer_zero_node));

      /* Simplify comparison of something with itself.  (For IEEE
         floating-point, we can only do some of these simplifications.)  */
      if (c_is_operand_equal_p(arg0, arg1, 0))
        {
          switch (code)
            {
            case C_EQ_EXPR:
            case C_GE_EXPR:
            case C_LE_EXPR:
              if (INTEGRAL_TYPE_P (CTreeType(arg0)))
                {
                  if (type == c_integer_type_node)
                    return c_integer_one_node;

                  t = c_tree_build_int_2(1, 0);
                  CTreeType(t) = type;
                  return t;
                }
              code = C_EQ_EXPR;
              CTreeNodeCode(t) = code;
              break;

            case C_NE_EXPR:
              /* For NE, we can only do this simplification if integer.  */
              if (! INTEGRAL_TYPE_P (CTreeType(arg0)))
                break;
              /* ... fall through ...  */
            case C_GT_EXPR:
            case C_LT_EXPR:
              if (type == c_integer_type_node)
                return c_integer_zero_node;

              t = c_tree_build_int_2(0, 0);
              CTreeType(t) = type;
              return t;
            default:
              abort ();
            }
        }

      /* An unsigned comparison against 0 can be simplified.  */
      if (c_tree_is_integer_zerop(arg1)
          && (INTEGRAL_TYPE_P (CTreeType(arg1))
              || POINTER_TYPE_P (CTreeType(arg1)))
          && CIsTreeNodeUnsigned(CTreeType(arg1)))
        {
          switch (CTreeNodeCode(t))
            {
            case C_GT_EXPR:
              code = C_NE_EXPR;
               CTreeNodeCode(t) = C_NE_EXPR;
              break;
            case C_LE_EXPR:
              code = C_EQ_EXPR;
              CTreeNodeCode(t) = C_EQ_EXPR;
              break;
            case C_GE_EXPR:
              return c_omit_one_operand(type,
                                       c_convert_expr(type, c_integer_one_node),
                                       arg0);
            case C_LT_EXPR:
              return c_omit_one_operand(type,
                                       c_convert_expr(type, c_integer_zero_node),
                                       arg0);
            default:
              break;
            }
        }

      /* An unsigned <= 0x7fffffff can be simplified.  */
      {
        int width = CTreeTypePrecision(CTreeType(arg1));
        if (CTreeNodeCode(arg1) == C_INT_CST_NODE
            && ! CIsTreeNodeCstOverflow(arg1)
            && width <= C_HOST_BITS_PER_WIDE_INT
            && CTreeIntCstLow(arg1) == ((C_HOST_WIDE_INT) 1 << (width - 1)) - 1
            && CTreeIntCstHigh(arg1) == 0
            && (INTEGRAL_TYPE_P (CTreeType(arg1))
                || POINTER_TYPE_P (CTreeType(arg1)))
            && CIsTreeNodeUnsigned(CTreeType(arg1)))
          {
            switch (CTreeNodeCode(t))
              {
              case C_LE_EXPR:
                return c_fold(c_tree_build_binary_typed_expr(C_GE_EXPR, type,
                                    c_convert_expr(c_get_signed_type(CTreeType(arg0)),
                                             arg0),
                                    c_convert_expr(c_get_signed_type(CTreeType(arg1)),
                                             c_integer_zero_node)));
              case C_GT_EXPR:
                return c_fold(c_tree_build_binary_typed_expr(C_LT_EXPR, type,
                                    c_convert_expr( c_get_signed_type(CTreeType(arg0)),
                                             arg0),
                                    c_convert_expr( c_get_signed_type(CTreeType(arg1)),
                                             c_integer_zero_node)));
              }
          }
      }

      /* If we are comparing an expression that just has comparisons
         of two integer values, arithmetic expressions of those comparisons,
         and constants, we can simplify it.  There are only three cases
         to check: the two values can either be equal, the first can be
         greater, or the second can be greater.  Fold the expression for
         those three values.  Since each value must be 0 or 1, we have
         eight possibilities, each of which corresponds to the constant 0
         or 1 or one of the six possible comparisons.

         This handles common cases like (a > b) == 0 but also handles
         expressions like  ((x > y) - (y > x)) > 0, which supposedly
         occur in macroized code.  */

      if (CTreeNodeCode(arg1) == C_INT_CST_NODE && 
          CTreeNodeCode(arg0) != C_INT_CST_NODE)
        {
          c_tree_node *cval1 = 0;
          c_tree_node *cval2 = 0;
          int save_p = 0;

          if (c_twoval_comparison_p(arg0, &cval1, &cval2, &save_p)
              /* Don't handle degenerate cases here; they should already
                 have been handled anyway.  */
              && cval1 != 0 && cval2 != 0
              && ! (CIsTreeNodeConstant(cval1) && CIsTreeNodeConstant(cval2))
              && CTreeType(cval1) == CTreeType(cval2)
              && INTEGRAL_TYPE_P (CTreeType(cval1))
              && ! c_is_operand_equal_p(CTreeTypeMinValue(CTreeType(cval1)),
                                    CTreeTypeMaxValue(CTreeType(cval2)), 0))
            {
              c_tree_node *maxval = CTreeTypeMaxValue(CTreeType(cval1));
              c_tree_node *minval = CTreeTypeMinValue(CTreeType(cval1));

              /* We can't just pass T to c_eval_substin case cval1 or cval2
                 was the same as ARG1.  */

              c_tree_node *high_result
                = c_fold(c_tree_build_binary_typed_expr(code, type,
                               c_eval_subst(arg0, cval1, maxval, cval2, minval),
                               arg1));
              c_tree_node *equal_result
                = c_fold(c_tree_build_binary_typed_expr(code, type,
                               c_eval_subst(arg0, cval1, maxval, cval2, maxval),
                               arg1));
              c_tree_node *low_result
                = c_fold(c_tree_build_binary_typed_expr(code, type,
                               c_eval_subst(arg0, cval1, minval, cval2, maxval),
                               arg1));

              /* All three of these results should be 0 or 1.  Confirm they
                 are.  Then use those values to select the proper code
                 to use.  */

              if ((c_tree_is_integer_zerop(high_result)
                   || c_tree_is_integer_onep(high_result))
                  && (c_tree_is_integer_zerop(equal_result)
                      || c_tree_is_integer_onep(equal_result))
                  && (c_tree_is_integer_zerop(low_result)
                      || c_tree_is_integer_onep(low_result)))
                {
                  /* Make a 3-bit mask with the high-order bit being the
                     value for `>', the next for '=', and the low for '<'.  */
                  switch ((c_tree_is_integer_onep(high_result) * 4)
                          + (c_tree_is_integer_onep(equal_result) * 2)
                          + c_tree_is_integer_onep(low_result))
                    {
                    case 0:
                      /* Always false.  */
                      return c_omit_one_operand(type, c_integer_zero_node, arg0);
                    case 1:
                      code = C_LT_EXPR;
                      break;
                    case 2:
                      code = C_EQ_EXPR;
                      break;
                    case 3:
                      code = C_LE_EXPR;
                      break;
                    case 4:
                      code = C_GT_EXPR;
                      break;
                    case 5:
                      code = C_NE_EXPR;
                      break;
                    case 6:
                      code = C_GE_EXPR;
                      break;
                    case 7:
                      /* Always true.  */
                      return c_omit_one_operand(type, c_integer_one_node, arg0);
                    }

                  t = c_tree_build_binary_typed_expr(code, type, cval1, cval2);
                  if (save_p)
# ifdef LUDO_NO_SKIP
                    return save_expr (t);
# else
                    return t;
# endif
                  else
                    return c_fold(t);
                }
            }
        }

      /* If this is a comparison of a field, we may be able to simplify it.  */
# ifdef LUDO_NO_SKIP
      if ((CTreeNodeCode(arg0) == C_COMPONENT_REF
           || CTreeNodeCode(arg0) == C_BIT_FIELD_REF)
          && (code == C_EQ_EXPR || code == C_NE_EXPR)
          /* Handle the constant case even without -O
             to make sure the warnings are given.  */
          && (optimize || CTreeNodeCode(arg1) == C_INT_CST_NODE))
        {
          t1 = c_optimize_bit_field_compare(code, type, arg0, arg1);
          return t1 ? t1 : t;
        }
# endif

      /* If this is a comparison of complex values and either or both
         sizes are a COMPLEX_EXPR, it is best to split up the comparisons
         and join them with a TRUTH_ANDIF_EXPR or TRUTH_ORIF_EXPR.  This
         may prevent needless evaluations.  */
      if ((code == C_EQ_EXPR || code == C_NE_EXPR)
          && CTreeNodeCode(CTreeType(arg0)) == C_COMPLEX_TYPE
          && (CTreeNodeCode(arg0) == C_COMPLEX_EXPR
              || CTreeNodeCode(arg1) == C_COMPLEX_EXPR))
        {
          c_tree_node *subtype = CTreeType(CTreeType(arg0));
          c_tree_node *real0 = 
            c_fold(c_tree_build_unary_typed_expr(C_REALPART_EXPR, subtype, arg0));
          c_tree_node *imag0 = 
            c_fold(c_tree_build_unary_typed_expr(C_IMAGPART_EXPR, subtype, arg0));
          c_tree_node *real1 =
            c_fold(c_tree_build_unary_typed_expr(C_REALPART_EXPR, subtype, arg1));
          c_tree_node *imag1 = 
            c_fold(c_tree_build_unary_typed_expr(C_IMAGPART_EXPR, subtype, arg1));

          return c_fold(c_tree_build_binary_typed_expr(
              (code == C_EQ_EXPR ? C_TRUTH_ANDIF_EXPR : C_TRUTH_ORIF_EXPR), type,
              c_fold(c_tree_build_binary_typed_expr(code, type, real0, real1)),
              c_fold(c_tree_build_binary_typed_expr(code, type, imag0, imag1))));
        }

      /* From here on, the only cases we handle are when the result is
         known to be a constant.

         To compute GT, swap the arguments and do LT.
         To compute GE, do LT and invert the result.
         To compute LE, swap the arguments, do LT and invert the result.
         To compute NE, do EQ and invert the result.

         Therefore, the code below must handle only EQ and LT.  */

      if (code == C_LE_EXPR || code == C_GT_EXPR)
        {
          tem = arg0, arg0 = arg1, arg1 = tem;
          code = c_swap_tree_comparison(code);
        }

      /* Note that it is safe to invert for real values here because we
         will check below in the one case that it matters.  */

      invert = 0;
      if (code == C_NE_EXPR || code == C_GE_EXPR)
        {
          invert = 1;
          code = c_invert_tree_comparison(code);
        }

      /* Compute a result for LT or EQ if args permit;
         otherwise return T.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE && CTreeNodeCode(arg1) == C_INT_CST_NODE)
        {
          if (code == C_EQ_EXPR)
            t1 = c_tree_build_int_2((CTreeIntCstLow(arg0)
                               == CTreeIntCstLow(arg1))
                              && (CTreeIntCstHigh(arg0)
                                  == CTreeIntCstHigh(arg1)),
                              0);
          else
            t1 = c_tree_build_int_2((CIsTreeNodeUnsigned(CTreeType(arg0))
                               ? INT_CST_LT_UNSIGNED (arg0, arg1)
                               : INT_CST_LT (arg0, arg1)),
                              0);
        }
      /* Two real constants can be compared explicitly.  */
      else if (CTreeNodeCode(arg0) == C_REAL_CST_NODE && 
               CTreeNodeCode(arg1) == C_REAL_CST_NODE)
        {
          /* If either operand is a NaN, the result is false with two
             exceptions: First, an NE_EXPR is true on NaNs, but that case
             is already handled correctly since we will be inverting the
             result for NE_EXPR.  Second, if we had inverted a LE_EXPR
             or a GE_EXPR into a LT_EXPR, we must return true so that it
             will be inverted into false.  */

# ifdef LUDO_NO_SKIP
          if (REAL_VALUE_ISNAN (TREE_REAL_CST (arg0))
              || REAL_VALUE_ISNAN (TREE_REAL_CST (arg1)))
            t1 = c_tree_build_int_2(invert && code == LT_EXPR, 0);
          else
# endif
            if (code == C_EQ_EXPR)
              t1 = c_tree_build_int_2(REAL_VALUES_EQUAL (
                  CTreeRealCstValue(arg0), CTreeRealCstValue(arg1)),
                                0);
          else
            t1 = c_tree_build_int_2(REAL_VALUES_LESS (CTreeRealCstValue(arg0),
                                                CTreeRealCstValue(arg1)),
                              0);
        }

      if (t1 == NULL)
        return t;

      if (invert)
        CTreeIntCstLow(t1) ^= 1;

      CTreeType(t1) = type;
      return t1;

    case C_COND_EXPR:
      /* Pedantic ANSI C says that a conditional expression is never an lvalue,
         so all simple results must be passed through pedantic_non_lvalue.  */
      if (CTreeNodeCode(arg0) == C_INT_CST_NODE)
        return c_get_pedantic_non_lvalue(
            CTreeExprOperand(t, (c_tree_is_integer_zerop(arg0) ? 2 : 1)));
      else if (c_is_operand_equal_p(arg1, CTreeExprOperand(expr, 2), 0))
        return c_pedantic_omit_one_operand(type, arg1, arg0);

      /* If the second operand is zero, invert the comparison and swap
         the second and third operands.  Likewise if the second operand
         is constant and the third is not or if the third operand is
         equivalent to the first operand of the comparison.  */

      if (c_tree_is_integer_zerop(arg1)
          || (CIsTreeNodeConstant(arg1) && ! CIsTreeNodeConstant(CTreeExprOperand(t, 2)))
          || (c_tree_get_code_class(CTreeNodeCode(arg0)) == '<'
              && c_is_operand_equal_for_comparison_p(
                  CTreeExprOperand(arg0, 0), CTreeExprOperand(t, 2),
                  CTreeExprOperand(arg0, 1))))
        {
          /* See if this can be inverted.  If it can't, possibly because
             it was a floating-point inequality comparison, don't do
             anything.  */
          tem = c_invert_truthvalue(arg0);

          if (CTreeNodeCode(tem) != C_TRUTH_NOT_EXPR)
            {
              t = c_tree_build_ternary_typed_expr(code, type, tem,
                         CTreeExprOperand(t, 2), CTreeExprOperand(t, 1));
              arg0 = tem;
              arg1 = CTreeExprOperand(t, 2);
              CStripNops(arg1);
            }
        }

      /* If we have A op B ? A : C, we may be able to c_convert_exprthis to a
         simpler expression, depending on the operation and the values
         of B and C.  IEEE floating point prevents this though,
         because A or B might be -0.0 or a NaN.  */

      if (c_tree_get_code_class(CTreeNodeCode(arg0)) == '<'
          && (
# ifdef LUDO_NO_SKIP
              TARGET_FLOAT_FORMAT != IEEE_FLOAT_FORMAT ||
# endif
              ! FLOAT_TYPE_P (CTreeType(CTreeExprOperand(arg0, 0)))
              || c_flag_fast_math)
          && c_is_operand_equal_for_comparison_p(CTreeExprOperand(arg0, 0),
                                             arg1, CTreeExprOperand(arg0, 1)))
        {
          c_tree_node *arg2 = CTreeExprOperand(t, 2);
          int comp_code = CTreeNodeCode(arg0);

          CStripNops(arg2);

          /* If we have A op 0 ? A : -A, this is A, -A, abs (A), or abs (-A),
             depending on the comparison operation.  */
          if ((FLOAT_TYPE_P (CTreeType(CTreeExprOperand(arg0, 1)))
               ? c_tree_is_real_zerop(CTreeExprOperand(arg0, 1))
               : c_tree_is_integer_zerop(CTreeExprOperand(arg0, 1)))
              && CTreeNodeCode(arg2) == C_NEGATE_EXPR
              && c_is_operand_equal_p(CTreeExprOperand(arg2, 0), arg1, 0))
            switch (comp_code)
              {
              case C_EQ_EXPR:
                return c_get_pedantic_non_lvalue(
                    c_fold(c_tree_build_unary_typed_expr(C_NEGATE_EXPR, type, arg1)));
              case C_NE_EXPR:
                return c_get_pedantic_non_lvalue(c_convert_expr(type, arg1));
              case C_GE_EXPR:
              case C_GT_EXPR:
                return c_get_pedantic_non_lvalue(
                    c_convert_expr(type, c_fold(c_tree_build_unary_typed_expr(C_ABS_EXPR,
                                                CTreeType(arg1), arg1))));
              case C_LE_EXPR:
              case C_LT_EXPR:
                return c_get_pedantic_non_lvalue(
                    c_fold(c_tree_build_unary_typed_expr(C_NEGATE_EXPR, type,
                                 c_convert_expr(type,
                                          c_fold(c_tree_build_unary_typed_expr(C_ABS_EXPR,
                                                        CTreeType(arg1),
                                                        arg1))))));
              default:
                abort ();
              }

          /* If this is A != 0 ? A : 0, this is simply A.  For ==, it is
             always zero.  */

          if (c_tree_is_integer_zerop(CTreeExprOperand(arg0, 1)) && 
              c_tree_is_integer_zerop(arg2))
            {
              if (comp_code == C_NE_EXPR)
                return c_get_pedantic_non_lvalue(c_convert_expr(type, arg1));
              else if (comp_code == C_EQ_EXPR)
                return c_get_pedantic_non_lvalue(c_convert_expr(type, c_integer_zero_node));
            }

          /* If this is A op B ? A : B, this is either A, B, min (A, B),
             or max (A, B), depending on the operation.  */

          if (c_is_operand_equal_for_comparison_p(CTreeExprOperand(arg0, 1),
                                              arg2, CTreeExprOperand(arg0, 0)))
            {
              c_tree_node *comp_op0 = CTreeExprOperand(arg0, 0);
              c_tree_node *comp_op1 = CTreeExprOperand(arg0, 1);
              c_tree_node *comp_type = CTreeType(comp_op0);

              switch (comp_code)
                {
                case C_EQ_EXPR:
                  return c_get_pedantic_non_lvalue(c_convert_expr(type, arg2));
                case C_NE_EXPR:
                  return c_get_pedantic_non_lvalue(c_convert_expr(type, arg1));
                case C_LE_EXPR:
                case C_LT_EXPR:
                  /* In C++ a ?: expression can be an lvalue, so put the
                     operand which will be used if they are equal first
                     so that we can c_convert_exprthis back to the 
                     corresponding COND_EXPR.  */
                  return c_get_pedantic_non_lvalue(
                      c_convert_expr(
                          type, (c_fold(c_tree_build_binary_typed_expr(C_MIN_EXPR, comp_type,
                                                  (comp_code == C_LE_EXPR
                                                   ? comp_op0 : comp_op1),
                                                  (comp_code == C_LE_EXPR
                                                   ? comp_op1 : comp_op0))))));
                  break;
                case C_GE_EXPR:
                case C_GT_EXPR:
                  return c_get_pedantic_non_lvalue(
                      c_convert_expr(type,
                          c_fold(c_tree_build_binary_typed_expr(C_MAX_EXPR, comp_type,
                                                 (comp_code == C_GE_EXPR
                                                  ? comp_op0 : comp_op1),
                                                 (comp_code == C_GE_EXPR
                                                  ? comp_op1 : comp_op0)))));
                  break;
                default:
                  abort ();
                }
            }

          /* If this is A op C1 ? A : C2 with C1 and C2 constant integers,
             we might still be able to simplify this.  For example,
             if C1 is one less or one more than C2, this might have started
             out as a MIN or MAX and been transformed by this function.
             Only good for INTEGER_TYPEs, because we need TYPE_MAX_VALUE.  */

          if (INTEGRAL_TYPE_P (type)
              && CTreeNodeCode(CTreeExprOperand(arg0, 1)) == C_INT_CST_NODE
              && CTreeNodeCode(arg2) == C_INT_CST_NODE)
            switch (comp_code)
              {
              case C_EQ_EXPR:
                /* We can replace A with C1 in this case.  */
                arg1 = c_convert_expr(type, CTreeExprOperand(arg0, 1));
                t = c_tree_build_ternary_typed_expr(code, type, CTreeExprOperand(t, 0), arg1,
                           CTreeExprOperand(t, 2));
                break;

              case C_LT_EXPR:
                /* If C1 is C2 + 1, this is min(A, C2).  */
                if (! c_is_operand_equal_p(arg2, CTreeTypeMaxValue(type), 1)
                    && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                        c_const_binop(C_PLUS_EXPR, arg2,
                                                     c_integer_one_node, 0), 1))
                  return c_get_pedantic_non_lvalue(
                      c_fold(c_tree_build_binary_typed_expr(C_MIN_EXPR, type, arg1, arg2)));
                break;

              case C_LE_EXPR:
                /* If C1 is C2 - 1, this is min(A, C2).  */
                if (! c_is_operand_equal_p(arg2, CTreeTypeMinValue(type), 1)
                    && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                        c_const_binop(C_MINUS_EXPR, arg2,
                                                     c_integer_one_node, 0), 1))
                  return c_get_pedantic_non_lvalue(
                      c_fold(c_tree_build_binary_typed_expr(C_MIN_EXPR, type, arg1, arg2)));
                break;

              case C_GT_EXPR:
                /* If C1 is C2 - 1, this is max(A, C2).  */
                if (! c_is_operand_equal_p(arg2, CTreeTypeMinValue(type), 1)
                    && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                        c_const_binop(C_MINUS_EXPR, arg2,
                                                     c_integer_one_node, 0), 1))
                  return c_get_pedantic_non_lvalue(
                      c_fold(c_tree_build_binary_typed_expr(C_MAX_EXPR, type, arg1, arg2)));
                break;

              case C_GE_EXPR:
                /* If C1 is C2 + 1, this is max(A, C2).  */
                if (! c_is_operand_equal_p(arg2, CTreeTypeMaxValue(type), 1)
                    && c_is_operand_equal_p(CTreeExprOperand(arg0, 1),
                                        c_const_binop(C_PLUS_EXPR, arg2,
                                                     c_integer_one_node, 0), 1))
                  return c_get_pedantic_non_lvalue(
                      c_fold(c_tree_build_binary_typed_expr(C_MAX_EXPR, type, arg1, arg2)));
                break;
              case C_NE_EXPR:
                break;
              default:
                abort ();
              }
        }

      /* If the second operand is simpler than the third, swap them
         since that produces better jump optimization results.  */
      if ((CIsTreeNodeConstant(arg1) || c_tree_get_code_class(CTreeNodeCode(arg1)) == 'd'
           || CTreeNodeCode(arg1) == C_SAVE_EXPR)
          && ! (CIsTreeNodeConstant(CTreeExprOperand(t, 2))
                || c_tree_get_code_class(CTreeNodeCode(CTreeExprOperand(t, 2))) == 'd'
                || CTreeNodeCode(CTreeExprOperand(t, 2)) == C_SAVE_EXPR))
        {
          /* See if this can be inverted.  If it can't, possibly because
             it was a floating-point inequality comparison, don't do
             anything.  */
          tem = c_invert_truthvalue(arg0);

          if (CTreeNodeCode(tem) != C_TRUTH_NOT_EXPR)
            {
              t = c_tree_build_ternary_typed_expr(code, type, tem,
                         CTreeExprOperand(t, 2), CTreeExprOperand(t, 1));
              arg0 = tem;
              arg1 = CTreeExprOperand(t, 2);
              CStripNops(arg1);
            }
        }

      /* Convert A ? 1 : 0 to simply A.  */
      if (c_tree_is_integer_onep(CTreeExprOperand(t, 1))
          && c_tree_is_integer_zerop(CTreeExprOperand(t, 2))
          /* If we try to c_convert_exprCTreeExprOperand(t, 0) to our type, the
             call to c_foldwill try to move the conversion inside 
             a COND, which will recurse.  In that case, the COND_EXPR
             is probably the best choice, so leave it alone.  */
          && type == CTreeType(arg0))
        return c_get_pedantic_non_lvalue(arg0);

      /* Look for expressions of the form A & 2 ? 2 : 0.  The result of this
         operation is simply A & 2.  */

      if (c_tree_is_integer_zerop(CTreeExprOperand(t, 2))
          && CTreeNodeCode(arg0) == C_NE_EXPR
          && c_tree_is_integer_zerop(CTreeExprOperand(arg0, 1))
          && c_tree_is_integer_pow2p(arg1)
          && CTreeNodeCode(CTreeExprOperand(arg0, 0)) == C_BIT_AND_EXPR
          && c_is_operand_equal_p(CTreeExprOperand(CTreeExprOperand(arg0, 0), 1),
                              arg1, 1))
        return c_get_pedantic_non_lvalue(c_convert_expr(type, CTreeExprOperand(arg0, 0)));

      return t;

    case C_COMPOUND_EXPR:
      /* When pedantic, a compound expression can be neither an lvalue
         nor an integer constant expression.  */
      if ( CIsTreeNodeSideEffect(arg0) || c_pedantic)
        return t;
      /* Don't let (0, 0) be null pointer constant.  */
      if (c_tree_is_integer_zerop(arg1))
        return c_get_non_lvalue(arg1);
      return arg1;

    case C_COMPLEX_EXPR:
      if (wins)
        return c_tree_build_complex(type, arg0, arg1);
      return t;

    case C_REALPART_EXPR:
      if (CTreeNodeCode(CTreeType(arg0)) != C_COMPLEX_TYPE)
        return t;
      else if (CTreeNodeCode(arg0) == C_COMPLEX_EXPR)
        return c_omit_one_operand(type, CTreeExprOperand(arg0, 0),
                                 CTreeExprOperand(arg0, 1));
      else if (CTreeNodeCode(arg0) == C_COMPLEX_NODE)
        return CTreeComplexReal(arg0);
      else if (CTreeNodeCode(arg0) == C_PLUS_EXPR || CTreeNodeCode(arg0) == C_MINUS_EXPR)
        return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type,
                            c_fold(c_tree_build_unary_typed_expr(C_REALPART_EXPR, type,
                                          CTreeExprOperand(arg0, 0))),
                            c_fold(c_tree_build_unary_typed_expr(C_REALPART_EXPR,
                                          type, CTreeExprOperand(arg0, 1)))));
      return t;

    case C_IMAGPART_EXPR:
      if (CTreeNodeCode(CTreeType(arg0)) != C_COMPLEX_TYPE)
        return c_convert_expr(type, c_integer_zero_node);
      else if (CTreeNodeCode(arg0) == C_COMPLEX_EXPR)
        return c_omit_one_operand(type, CTreeExprOperand(arg0, 1),
                                 CTreeExprOperand(arg0, 0));
      else if (CTreeNodeCode(arg0) == C_COMPLEX_NODE)
        return CTreeComplexImag(arg0);
      else if (CTreeNodeCode(arg0) == C_PLUS_EXPR || CTreeNodeCode(arg0) == C_MINUS_EXPR)
        return c_fold(c_tree_build_binary_typed_expr(CTreeNodeCode(arg0), type,
                            c_fold(c_tree_build_unary_typed_expr(C_IMAGPART_EXPR, type,
                                          CTreeExprOperand(arg0, 0))),
                            c_fold(c_tree_build_unary_typed_expr(C_IMAGPART_EXPR, type,
                                          CTreeExprOperand(arg0, 1)))));
      return t;

      /* Pull arithmetic ops out of the CLEANUP_POINT_EXPR where
         appropriate.  */
    case C_CLEANUP_POINT_EXPR:
      if (! CIsTreeNodeSideEffect(arg0))
        return CTreeExprOperand(t, 0);

      {
        int code0 = CTreeNodeCode(arg0);
        int kind0 = c_tree_get_code_class(code0);
        c_tree_node *arg00 = CTreeExprOperand(arg0, 0);
        c_tree_node *arg01;

        if (kind0 == '1' || code0 == C_TRUTH_NOT_EXPR)
          return c_fold(c_tree_build_unary_typed_expr(code0, type, 
                               c_fold(c_tree_build_unary_typed_expr(C_CLEANUP_POINT_EXPR,
                                             CTreeType(arg00), arg00))));

        if (kind0 == '<' || kind0 == '2'
            || code0 == C_TRUTH_ANDIF_EXPR || code0 == C_TRUTH_ORIF_EXPR
            || code0 == C_TRUTH_AND_EXPR   || code0 == C_TRUTH_OR_EXPR
            || code0 == C_TRUTH_XOR_EXPR)
          {
            arg01 = CTreeExprOperand(arg0, 1);

            if (! CIsTreeNodeSideEffect(arg00))
              return c_fold(c_tree_build_binary_typed_expr(code0, type, arg00,
                                  c_fold(c_tree_build_unary_typed_expr(C_CLEANUP_POINT_EXPR,
                                                CTreeType(arg01), arg01))));

            if (! CIsTreeNodeSideEffect(arg01))
              return c_fold(c_tree_build_binary_typed_expr(code0, type,
                                  c_fold(c_tree_build_unary_typed_expr(C_CLEANUP_POINT_EXPR,
                                                CTreeType(arg00), arg00)),
                                  arg01));
          }

        return t;
      }

    default:
      return t;
    } /* switch (code) */
  return expr;
}
