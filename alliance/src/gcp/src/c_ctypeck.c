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
| File    :                 c_ctypeck.c                       |
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
# include "c_flags.h"
# include "c_cdecl.h"
# include "c_ctypeck.h"
# include "c_convert.h"
# include "c_debug.h"
# include "c_foldconst.h"
# include "c_stmt.h"
# include "c_common.h"

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

/* Type of object we are accumulating a constructor for.
   This type is always a RECORD_TYPE, UNION_TYPE or ARRAY_TYPE.  */
static c_tree_node *c_constructor_type;

/* For a RECORD_TYPE or UNION_TYPE, this is the chain of fields
   left to fill.  */
static c_tree_node *c_constructor_fields;

/* For an ARRAY_TYPE, this is the specified index
   at which to store the next element we get.
   This is a special INTEGER_CST node that we modify in place.  */
static c_tree_node *c_constructor_index;

/* For an ARRAY_TYPE, this is the end index of the range
   to initialize with the next element, or NULL in the ordinary case
   where the element is used just once.  */
static c_tree_node *c_constructor_range_end;

/* For an ARRAY_TYPE, this is the maximum index.  */
static c_tree_node *c_constructor_max_index;

/* For a RECORD_TYPE, this is the first field not yet written out.  */
static c_tree_node *c_constructor_unfilled_fields;

/* For an ARRAY_TYPE, this is the index of the first element
   not yet written out.
   This is a special INTEGER_CST node that we modify in place.  */
static c_tree_node *c_constructor_unfilled_index;

/* In a RECORD_TYPE, the byte index of the next consecutive field.
   This is so we can generate gaps between fields, when appropriate.
   This is a special INTEGER_CST node that we modify in place.  */
static c_tree_node *c_constructor_bit_index;

/* If we are saving up the elements rather than allocating them,
   this is the list of elements so far (in reverse order,
   most recent first).  */
static c_tree_node *c_constructor_elements;

/* 1 if so far this constructor's elements are all compile-time constants.  */
static int c_constructor_constant;

/* 1 if so far this constructor's elements are all valid address constants.  */
static int c_constructor_simple;

/* 1 if this constructor is erroneous so far.  */
static int c_constructor_erroneous;

/* 1 if have called defer_addressed_constants.  */
static int c_constructor_subconstants_deferred;

/* List of pending elements at this constructor level.
   These are elements encountered out of order
   which belong at places we haven't reached yet in actually
   writing the output.  */
static c_tree_node *c_constructor_pending_elts;

/* The SPELLING_DEPTH of this constructor.  */
static int c_constructor_depth;

/* 0 if implicitly pushing constructor levels is allowed.  */
int c_constructor_no_implicit = 0; /* 0 for C; 1 for some other languages.  */

/* 1 if this constructor level was entered implicitly.  */
# ifdef LUDO_NO_SKIP
static int c_constructor_implicit;
# endif

static int c_require_constant_value;
static int c_require_constant_elements;

/* 1 if it is ok to output this constructor as we read it.
   0 means must accumulate a CONSTRUCTOR expression.  */
static int c_constructor_incremental;

/* DECL node for which an initializer is being read.
   0 means we are reading a constructor expression
   such as (struct foo) {...}.  */
static c_tree_node *c_constructor_decl;

/* start_init saves the ASMSPEC arg here for really_start_incremental_init.  */
static char *c_constructor_asmspec;

/* Nonzero if this is an initializer for a top-level decl.  */
static int c_constructor_top_level;

/* When we finish reading a constructor expression
   (constructor_decl is 0), the CONSTRUCTOR goes here.  */
# ifdef LUDO_NO_SKIP
static c_tree_node *c_constructor_result;
# endif

/* This stack has a level for each implicit or explicit level of
   structuring in the initializer, including the outermost one.  It
   saves the values of most of the variables above.  */

struct constructor_stack
{
  struct constructor_stack *next;
  c_tree_node *type;
  c_tree_node *fields;
  c_tree_node *index;
  c_tree_node *range_end;
  c_tree_node *max_index;
  c_tree_node *unfilled_index;
  c_tree_node *unfilled_fields;
  c_tree_node *bit_index;
  c_tree_node *elements;
  int offset;
  c_tree_node *pending_elts;
  int depth;
  /* If nonzero, this value should replace the entire
     constructor at this level.  */
  c_tree_node *replacement_value;
  char constant;
  char simple;
  char implicit;
  char incremental;
  char erroneous;
  char outer;
};

struct constructor_stack *c_constructor_stack;

/* This stack records separate initializers that are nested.
   Nested initializers can't happen in ANSI C, but GNU C allows them
   in cases like { ... (struct foo) { ... } ... }.  */

struct initializer_stack
{
  struct initializer_stack *next;
  c_tree_node *decl;
  char *asmspec;
  struct constructor_stack *constructor_stack;
  c_tree_node *elements;
  struct spelling *spelling;
  struct spelling *spelling_base;
  int spelling_size;
  char top_level;
  char incremental;
  char require_constant_value;
  char require_constant_elements;
  char deferred;
};

struct initializer_stack *c_initializer_stack;


/* Nonzero if we've already printed a "missing braces around initializer"
   message within this initializer.  */
static int c_missing_braces_mentioned;


/* Ajoute par Ludo pour obtenir la liste des constantes lors
 * d'une initialisation de tableau ou de structure */
static c_tree_node *c_output_constant_list = (c_tree_node *)0;


/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/* Methods for storing and printing names for error messages.  */

/* Implement a spelling stack that allows components of a name to be pushed
   and popped.  Each element on the stack is this structure.  */

struct spelling
{
  int kind;
  union
    {
      int i;
      char *s;
    } u;
};

#define SPELLING_STRING 1
#define SPELLING_MEMBER 2
#define SPELLING_BOUNDS 3

static struct spelling *c_spelling;        /* Next stack element (unused).  */
static struct spelling *c_spelling_base;        /* Spelling stack base.  */
static int c_spelling_size;                /* Size of the spelling stack.  */

/* Macros to save and restore the spelling stack around push_... functions.
   Alternative to SAVE_SPELLING_STACK.  */

#define SPELLING_DEPTH() (c_spelling - c_spelling_base)
#define RESTORE_SPELLING_DEPTH(depth) (c_spelling = c_spelling_base + depth)

/* Save and restore the spelling stack around arbitrary C code.  */

#define SAVE_SPELLING_DEPTH(code)                \
{                                                \
  int __depth = SPELLING_DEPTH ();                \
  code;                                                \
  RESTORE_SPELLING_DEPTH (__depth);                \
}

/* Push an element on the spelling stack with type KIND and assign VALUE
   to MEMBER.  */

#define PUSH_SPELLING(KIND, VALUE, MEMBER)                                \
{                                                                        \
  int depth = SPELLING_DEPTH ();                                        \
                                                                        \
  if (depth >= c_spelling_size)                                                \
    {                                                                        \
      c_spelling_size += 10;                                                \
      if (c_spelling_base == 0)                                                \
        c_spelling_base                                                        \
          = (struct spelling *)autallocblock(c_spelling_size * sizeof (struct spelling)); \
      else                                                                \
        c_spelling_base                                                        \
          = (struct spelling *)mbkrealloc(c_spelling_base,                \
                                          c_spelling_size * sizeof (struct spelling));        \
      RESTORE_SPELLING_DEPTH (depth);                                        \
    }                                                                        \
                                                                        \
  c_spelling->kind = (KIND);                                                \
  c_spelling->MEMBER = (VALUE);                                                \
  c_spelling++;                                                                \
}

/* Push STRING on the stack.  Printed literally.  */

static void
c_push_string (string)
     char *string;
{
  PUSH_SPELLING (SPELLING_STRING, string, u.s);
}

/* Push a member name on the stack.  Printed as '.' STRING.  */

static void
c_push_member_name (decl)
     c_tree_node *decl;
     
{
  char *string
    = CTreeDeclName(decl) ? CTreeIdentPointer(CTreeDeclName(decl)) : "<anonymous>";
  PUSH_SPELLING (SPELLING_MEMBER, string, u.s);
}

/* Push an array bounds on the stack.  Printed as [BOUNDS].  */

static void
c_push_array_bounds (bounds)
     int bounds;
{
  PUSH_SPELLING (SPELLING_BOUNDS, bounds, u.i);
}

/* Compute the maximum size in bytes of the printed spelling.  */

static int
c_spelling_length ()
{
  register int size = 0;
  register struct spelling *p;

  for (p = c_spelling_base; p < c_spelling; p++)
    {
      if (p->kind == SPELLING_BOUNDS)
        size += 25;
      else
        size += strlen (p->u.s) + 1;
    }

  return size;
}

/* Print the spelling to BUFFER and return it.  */

static char *
c_print_spelling (buffer)
     register char *buffer;
{
  register char *d = buffer;
  register char *s;
  register struct spelling *p;

  for (p = c_spelling_base; p < c_spelling; p++)
    if (p->kind == SPELLING_BOUNDS)
      {
        sprintf (d, "[%d]", p->u.i);
        d += strlen (d);
      }
    else
      {
        if (p->kind == SPELLING_MEMBER)
          *d++ = '.';
        for (s = p->u.s; (*d = *s++) != 0; d++)
          ;
      }
  *d++ = '\0';
  return buffer;
}

/* Provide a means to pass component names derived from the spelling stack.  */

char c_initialization_message;

/* Interpret the spelling of the given ERRTYPE message.  */

static char *
c_get_spelling (errtype)
     char *errtype;
{
  static char *buffer;
  static int size = -1;

  if (errtype == &c_initialization_message)
    {
      /* Avoid counting chars */
      static char message[] = "initialization of `%s'";
      register int needed = sizeof (message) + c_spelling_length () + 1;
      char *temp;

      if (size < 0)
        buffer = (char *) autallocblock(size = needed);
      if (needed > size)
        buffer = (char *) mbkrealloc(buffer, size = needed);

      temp = (char *) alloca (needed);
      sprintf (buffer, message, c_print_spelling (temp));
      return buffer;
    }

  return errtype;
}

/* Issue an error message for a bad initializer component.
   FORMAT describes the message.  OFWHAT is the name for the component.
   LOCAL is a format string for formatting the insertion of the name
   into the message.

   If OFWHAT is null, the component name is stored on the spelling stack.
   If the component name is a null string, then LOCAL is omitted entirely.  */

void
c_error_init (format, local, ofwhat)
     char *format, *local, *ofwhat;
{
  char *buffer;

  if (ofwhat == 0)
    ofwhat = c_print_spelling ((char *) autallocblock(c_spelling_length () + 1));
  buffer = (char *) autallocblock(strlen (local) + strlen (ofwhat) + 2);

  if (*ofwhat)
    sprintf (buffer, local, ofwhat);
  else
    buffer[0] = 0;

  error (format, buffer);
}

/* Issue a pedantic warning for a bad initializer component.
   FORMAT describes the message.  OFWHAT is the name for the component.
   LOCAL is a format string for formatting the insertion of the name
   into the message.

   If OFWHAT is null, the component name is stored on the spelling stack.
   If the component name is a null string, then LOCAL is omitted entirely.  */

void
c_pedwarn_init (format, local, ofwhat)
     char *format, *local, *ofwhat;
{
  char *buffer;

  if (ofwhat == 0)
    ofwhat = c_print_spelling ((char *) autallocblock(c_spelling_length () + 1));
  buffer = (char *) autallocblock(strlen (local) + strlen (ofwhat) + 2);

  if (*ofwhat)
    sprintf (buffer, local, ofwhat);
  else
    buffer[0] = 0;

  c_pedwarn(format, buffer);
}

/* Issue a warning for a bad initializer component.
   FORMAT describes the message.  OFWHAT is the name for the component.
   LOCAL is a format string for formatting the insertion of the name
   into the message.

   If OFWHAT is null, the component name is stored on the spelling stack.
   If the component name is a null string, then LOCAL is omitted entirely.  */

static void
c_warning_init (format, local, ofwhat)
     char *format, *local, *ofwhat;
{
  char *buffer;

  if (ofwhat == 0)
    ofwhat = c_print_spelling ((char *) autallocblock(c_spelling_length () + 1));
  buffer = (char *) autallocblock(strlen (local) + strlen (ofwhat) + 2);

  if (*ofwhat)
    sprintf (buffer, local, ofwhat);
  else
    buffer[0] = 0;

  c_warning (format, buffer);
}


/* If pedantic, warn about improper lvalue.   CODE is either COND_EXPR
   COMPOUND_EXPR, or CONVERT_EXPR (for casts).  */

static void
c_pedantic_lvalue_warning(code)
     int code;
{
  if (c_pedantic)
  {
    c_pedwarn ("ANSI C forbids use of %s expressions as lvalues",
	     code == C_COND_EXPR ? "conditional"
	     : code == C_COMPOUND_EXPR ? "compound" : "cast");
  }
}


/* Warn about storing in something that is `const'.  */

void c_readonly_warning(arg, string)
     c_tree_node *arg;
     char *string;
{
  char buf[80];
  strcpy (buf, string);

  /* Forbid assignments to iterators.  */
  if ( CTreeNodeFirstCode(arg) == C_DECL_NODE && 
      CTreeDeclCode( arg ) == C_VAR_DECL &&
      CIsTreeDeclIterator(arg))
    {
      strcat (buf, " of iterator `%s'");
      pedwarn (buf, CTreeIdentPointer(CTreeDeclName(arg)));
    }

  if (CTreeNodeFirstCode(arg ) == C_EXPR_NODE && CTreeExprCode(arg) == C_COMPONENT_REF)
    {
      if ( CIsTreeNodeReadOnly(CTreeType(CTreeExprOperand(arg, 0))))
      {
        c_readonly_warning (CTreeExprOperand(arg, 0), string);
      }
      else
        {
          strcat (buf, " of read-only member `%s'");
          pedwarn (buf, CTreeIdentPointer(CTreeDeclName(CTreeExprOperand(arg, 1))));
        }
    }
  else if ( CTreeNodeFirstCode(arg) == C_DECL_NODE && CTreeDeclCode(arg) == C_VAR_DECL)
    {
      strcat (buf, " of read-only variable `%s'");
      pedwarn (buf, CTreeIdentPointer(CTreeDeclName(arg)));
    }
  else
    {
      pedwarn ("%s of read-only location", buf);
    }
}


/* Print a warning using MSG.
   It gets OPNAME as its one parameter.
   If OPNAME is null, it is replaced by "passing arg ARGNUM of `FUNCTION'".
   FUNCTION and ARGNUM are handled specially if we are building an
   Objective-C selector.  */

static void
c_warn_for_assignment(msg, opname, function, argnum)
     char *msg;
     char *opname;
     c_tree_node *function;
     int argnum;
{
  static char argstring[] = "passing arg %d of `%s'";
  static char argnofun[] =  "passing arg %d";

  if (opname == 0)
    {
# ifdef LUDO_NO_SKIP
      c_tree_node *selector = maybe_building_objc_message_expr ();
# else
      c_tree_node *selector = 0;
# endif
      
      if (selector && argnum > 2)
        {
          function = selector;
          argnum -= 2;
        }
      if (function)
        {
          /* Function name is known; supply it.  */
          opname = (char *) autallocblock(CTreeIdentLength(function)
                                    + sizeof (argstring) + 25 /*%d*/ + 1);
          sprintf (opname, argstring, argnum, CTreeIdentPointer(function));
        }
      else
        {
          /* Function name unknown (call through ptr); just give arg number.  */
          opname = (char *) autallocblock(sizeof (argnofun) + 25 /*%d*/ + 1);
          sprintf (opname, argnofun, argnum);
        }
    }
  c_pedwarn (msg, opname);
}


/* Apply unary lvalue-demanding operator CODE to the expression ARG
   for certain kinds of expressions which are not really lvalues
   but which we can accept as lvalues.

   If ARG is not a kind of expression we can handle, return zero.  */
   
static c_tree_node *c_unary_complex_lvalue(code, arg)
     int code;
     c_tree_node *arg;
{
  /* Handle (a, b) used as an "lvalue".  */
  if ( CTreeNodeFirstCode(arg) == C_EXPR_NODE && 
       CTreeExprCode(arg) == C_COMPOUND_EXPR)
    {
      c_tree_node *real_result = c_parser_build_unary_op(code, CTreeExprOperand(arg, 1), 0);

      /* If this returns a function type, it isn't really being used as
         an lvalue, so don't issue a warning about it.  */
      if ( CTreeTypeCode(CTreeType(arg)) != C_FUNCTION_TYPE)
      {
        c_pedantic_lvalue_warning (C_COMPOUND_EXPR);
      }

      return c_tree_build_binary_typed_expr( C_COMPOUND_EXPR, CTreeType(real_result),
                    CTreeExprOperand(arg, 0), real_result);
    }

  /* Handle (a ? b : c) used as an "lvalue".  */
  if ( CTreeNodeFirstCode(arg) == C_EXPR_NODE && 
       CTreeExprCode(arg) == C_COND_EXPR)
    {
      c_pedantic_lvalue_warning(C_COND_EXPR);
      if ( CTreeTypeCode(CTreeType(arg)) != C_FUNCTION_TYPE)
      {
        c_pedantic_lvalue_warning (C_COMPOUND_EXPR);
      }

      return (c_parser_build_conditional_expr
              (CTreeExprOperand(arg, 0),
               c_parser_build_unary_op(code, CTreeExprOperand(arg, 1), 0),
               c_parser_build_unary_op(code, CTreeExprOperand(arg, 2), 0)));
    }

  return 0;
}


/* Subroutine of stabilize_reference; this is called for subtrees of
   references.  Any expression with side-effects must be put in a SAVE_EXPR
   to ensure that it is only evaluated once.

   We don't put SAVE_EXPR nodes around everything, because assigning very
   simple expressions to temporaries causes us to miss good opportunities
   for optimizations.  Among other things, the opportunity to fold in the
   addition of a constant into an addressing mode often gets lost, e.g.
   "y[i+1] += x;".  In general, we take the approach that we should not make
   an assignment unless we are forced into it - i.e., that any non-side effect
   operator should be allowed, and that cse should take care of coalescing
   multiple utterances of the same expression should that prove fruitful.  */

c_tree_node *c_stabilize_reference_1(e)
     c_tree_node *e;
{
  return( e );
}

/* Return either DECL or its known constant value (if it has one).  */

c_tree_node *c_decl_constant_value(decl)
     c_tree_node *decl;
{
  if (/* Don't change a variable array bound or initial value to a constant
         in a place where a variable is invalid.  */
      c_current_function_decl != 0
      && ! c_pedantic
      && ! CIsTreeNodeVolatile(decl)
      && CIsTreeNodeReadOnly(decl) && ! CIsTreeDeclIterator(decl)
      && CTreeDeclInitial(decl) != 0
      && CTreeNodeFirstCode(CTreeDeclInitial(decl)) != C_ERROR_NODE
      /* This is invalid if initial value is not constant.
         If it has either a function call, a memory reference,
         or a variable, then re-evaluating it could give different results.  */
      && CIsTreeNodeConstant(CTreeDeclInitial(decl))
      /* Check for cases where this is sub-optimal, even though valid.  */
      && ( CTreeNodeFirstCode(CTreeDeclInitial(decl)) != C_EXPR_NODE ||
           CTreeExprCode(CTreeDeclInitial(decl)) != C_CONSTRUCTOR_EXPR)
# ifdef LUDO_DONE
      && CTreeDeclMode(decl) != BLKmode
# endif
      )
    return CTreeDeclInitial(decl);
  return decl;
}


/* Stabilize a reference so that we can use it any number of times
   without causing its operands to be evaluated more than once.
   Returns the stabilized reference.  This works by means of save_expr,
   so see the caveats in the comments about save_expr.

   Also allows conversion expressions whose operands are references.
   Any other kind of expression is returned unchanged.  */

c_tree_node *c_stabilize_reference(ref)
     c_tree_node *ref;
{
  c_tree_node *result;
  int  code = CTreeNodeFirstCode(ref);

  if ( code == C_DECL_NODE )
  {
    code = CTreeDeclCode( ref );

    switch (code)
    {
    case C_VAR_DECL:
    case C_PARAM_DECL:
    case C_RESULT_DECL:
      /* No action is needed in this case.  */
      return ref;
    }

    return ref;
  }
  else
  if ( code == C_EXPR_NODE )
  {
    code = CTreeExprCode( ref );

    switch (code)
    {
    case C_NOP_EXPR:
    case C_CONVERT_EXPR:
    case C_FLOAT_EXPR:
    case C_FIX_TRUNC_EXPR:
    case C_FIX_FLOOR_EXPR:
    case C_FIX_ROUND_EXPR:
    case C_FIX_CEIL_EXPR:
      result = c_tree_build_unary_expr(code, c_stabilize_reference(CTreeExprOperand(ref, 0)));
      break;

    case C_INDIRECT_REF:
      result = c_tree_build_unary_expr(C_INDIRECT_REF,
                         c_stabilize_reference_1 (CTreeExprOperand(ref, 0)));
      break;

    case C_COMPONENT_REF:
      result = c_tree_build_binary_expr(C_COMPONENT_REF,
                         c_stabilize_reference (CTreeExprOperand(ref, 0)),
                         CTreeExprOperand(ref, 1));
      break;

    case C_BIT_FIELD_REF:
      result = c_tree_build_ternary_expr(C_BIT_FIELD_REF,
                         c_stabilize_reference (CTreeExprOperand(ref, 0)),
                         c_stabilize_reference_1 (CTreeExprOperand(ref, 1)),
                         c_stabilize_reference_1 (CTreeExprOperand(ref, 2)));
      break;

    case C_ARRAY_REF:
      result = c_tree_build_binary_expr(C_ARRAY_REF,
                         c_stabilize_reference (CTreeExprOperand(ref, 0)),
                         c_stabilize_reference_1 (CTreeExprOperand(ref, 1)));
      break;

    case C_COMPOUND_EXPR:
      /* We cannot wrap the first expression in a SAVE_EXPR, as then
         it wouldn't be ignored.  This matters when dealing with
         volatiles.  */
      return c_stabilize_reference_1 (ref);

    case C_RTL_EXPR:
      result = c_tree_build_unary_typed_expr( C_INDIRECT_REF, CTreeType(ref),
                 c_tree_build_unary_typed_expr(
                     C_ADDR_EXPR, c_tree_build_pointer_type(CTreeType(ref)), ref));
      break;


      /* If arg isn't a kind of lvalue we recognize, make no change.
         Caller should recognize the error for an invalid lvalue.  */
    default:
      return ref;

    }
  }
  else
  if ( code == C_ERROR_NODE )
  {
     return c_error_mark_node;
  }
  else
  {
     return ref;
  }

  CTreeType(result) = CTreeType(ref);
  if ( CIsTreeNodeReadOnly( ref ) ) CSetTreeNodeReadOnly( result );
  if ( CIsTreeNodeSideEffect( ref ) ) CSetTreeNodeSideEffect( result );
  if ( CIsTreeNodeVolatile( ref ) ) CSetTreeNodeVolatile( result );
  if ( CIsTreeNodeRaises( ref ) ) CSetTreeNodeRaises( result );

  return result;
}

/* Return OP, stripped of any conversions to wider types as much as is safe.
   Converting the value back to OP's type makes a value equivalent to OP.

   If FOR_TYPE is nonzero, we return a value which, if converted to
   type FOR_TYPE, would be equivalent to converting OP to type FOR_TYPE.

   If FOR_TYPE is nonzero, unaligned bit-field references may be changed to the
   narrowest type that can hold the value, even if they don't exactly fit.
   Otherwise, bit-field references are changed to a narrower type
   only if they can be fetched directly from memory in that type.

   OP must have integer, real or enumeral type.  Pointers are not allowed!

   There are some cases where the obvious value we could return
   would regenerate to OP if converted to OP's type, 
   but would not extend like OP to wider types.
   If FOR_TYPE indicates such extension is contemplated, we eschew such values.
   For example, if OP is (unsigned short)(signed char)-1,
   we avoid returning (signed char)-1 if FOR_TYPE is int,
   even though extending that to an unsigned short would regenerate OP,
   since the result of extending (signed char)-1 to (int)
   is different from (int) OP.  */

c_tree_node *c_get_unwidened(op, for_type)
     c_tree_node *op;
     c_tree_node *for_type;
{
  /* Set UNS initially if converting OP to FOR_TYPE is a zero-extension.  */
  /* CTreeTypePrecisionis safe in place of type_precision since
     pointer types are not allowed.  */
  c_tree_node *type = CTreeType(op);
  unsigned final_prec = CTreeTypePrecision(for_type != 0 ? for_type : type);
  int uns = (for_type != 0 && for_type != type
       && final_prec > CTreeTypePrecision(type)
       && CIsTreeNodeUnsigned(type));
  c_tree_node *win = op;

  while ( CTreeExprCode(op) == C_NOP_EXPR)
    {
      int bitschange
        = CTreeTypePrecision(CTreeType(op))
          - CTreeTypePrecision(CTreeType(CTreeExprOperand(op, 0)));

      /* Truncations are many-one so cannot be removed.
         Unless we are later going to truncate down even farther.  */
      if (bitschange < 0
          && final_prec > CTreeTypePrecision(CTreeType(op)))
        break;

      /* See what's inside this conversion.  If we decide to strip it,
         we will set WIN.  */
      op = CTreeExprOperand(op, 0);

      /* If we have not stripped any zero-extensions (uns is 0),
         we can strip any kind of extension.
         If we have previously stripped a zero-extension,
         only zero-extensions can safely be stripped.
         Any extension can be stripped if the bits it would produce
         are all going to be discarded later by truncating to FOR_TYPE.  */

      if (bitschange > 0)
        {
          if (! uns || final_prec <= CTreeTypePrecision(CTreeType(op)))
            win = op;
          /* CIsTreeNodeUnsignedsays whether this is a zero-extension.
             Let's avoid computing it if it does not affect WIN
             and if UNS will not be needed again.  */
          if ((uns || CTreeExprCode(op) == C_NOP_EXPR)
              && CIsTreeNodeUnsigned(CTreeType(op)))
            {
              uns = 1;
              win = op;
            }
        }
    }

  if ( CTreeExprCode(op) == C_COMPONENT_REF
      /* Since type_for_size always gives an integer type.  */
      && CTreeTypeCode(type) != C_REAL_TYPE
      /* Don't crash if field not laid out yet.  */
      && CTreeDeclSize(CTreeExprOperand(op, 1)) != 0)
    {
      unsigned innerprec = CTreeIntCstLow( CTreeDeclSize(CTreeExprOperand(op, 1)));
      type = c_get_type_for_size(innerprec, CIsTreeNodeUnsigned(CTreeExprOperand(op, 1)));

      /* We can get this structure field in the narrowest type it fits in.
         If FOR_TYPE is 0, do this only for a field that matches the
         narrower type exactly and is aligned for it
         The resulting extension to its nominal type (a fullword type)
         must fit the same conditions as for other extensions.  */

      if (innerprec < CTreeTypePrecision(CTreeType(op))
          && (for_type || ! CIsTreeDeclBitField(CTreeExprOperand(op, 1)))
          && (! uns || final_prec <= innerprec
              || CIsTreeNodeUnsigned(CTreeExprOperand(op, 1)))
          && type != 0)
        {
          win = c_tree_build_binary_expr(C_COMPONENT_REF, 
              CTreeExprOperand(op, 0), CTreeExprOperand(op, 1));
          CTreeType( win ) = type;
          if ( CIsTreeNodeSideEffect(op) ) CSetTreeNodeSideEffect(win);
          if ( CIsTreeNodeVolatile(op) ) CSetTreeNodeVolatile(win);
          if ( CIsTreeNodeRaises(op) ) CSetTreeNodeRaises( win );
        }
    }
  return win;
}


/* Return OP or a simpler expression for a narrower value
   which can be sign-extended or zero-extended to give back OP.
   Store in *UNSIGNEDP_PTR either 1 if the value should be zero-extended
   or 0 if the value should be sign-extended.  */

c_tree_node *c_get_narrower(op, unsignedp_ptr)
     c_tree_node *op;
     int *unsignedp_ptr;
{
  int uns = 0;
  int first = 1;
  c_tree_node *win = op;

  while ( CTreeExprCode(op) == C_NOP_EXPR)
    {
      int bitschange
        = CTreeTypePrecision(CTreeType(op))
          - CTreeTypePrecision(CTreeType(CTreeExprOperand(op, 0)));

      /* Truncations are many-one so cannot be removed.  */
      if (bitschange < 0)
        break;

      /* See what's inside this conversion.  If we decide to strip it,
         we will set WIN.  */
      op = CTreeExprOperand(op, 0);

      if (bitschange > 0)
        {
          /* An extension: the outermost one can be stripped,
             but remember whether it is zero or sign extension.  */
          if (first)
            uns = !! CIsTreeNodeUnsigned(CTreeType(op));
          /* Otherwise, if a sign extension has been stripped,
             only sign extensions can now be stripped;
             if a zero extension has been stripped, only zero-extensions.  */
          else if (uns != !! CIsTreeNodeUnsigned(CTreeType(op)))
            break;
          first = 0;
        }
      else /* bitschange == 0 */
        {
          /* A change in nominal type can always be stripped, but we must
             preserve the unsignedness.  */
          if (first)
            uns = !! CIsTreeNodeUnsigned(CTreeType(op));
          first = 0;
        }

      win = op;
    }

  if ( CTreeExprCode(op) == C_COMPONENT_REF
      /* Since type_for_size always gives an integer type.  */
      && CTreeTypeCode(CTreeType(op)) != C_REAL_TYPE)
    {
      unsigned innerprec = CTreeIntCstLow( CTreeDeclSize(CTreeExprOperand(op, 1)));
      c_tree_node *type = c_get_type_for_size(innerprec, CIsTreeNodeUnsigned(op));

      /* We can get this structure field in a narrower type that fits it,
         but the resulting extension to its nominal type (a fullword type)
         must satisfy the same conditions as for other extensions.

         Do this only for fields that are aligned (not bit-fields),
         because when bit-field insns will be used there is no
         advantage in doing this.  */

      if (innerprec < CTreeTypePrecision(CTreeType(op))
          && ! CIsTreeDeclBitField(CTreeExprOperand(op, 1))
          && (first || uns ==  CIsTreeNodeUnsigned(CTreeExprOperand(op, 1)))
          && type != 0)
        {
          if (first)
            uns = CIsTreeNodeUnsigned(CTreeExprOperand(op, 1));
          win = c_tree_build_binary_expr( 
              C_COMPONENT_REF, CTreeExprOperand(op, 0), CTreeExprOperand(op, 1));
          CTreeType( win ) = type;

          if ( CIsTreeNodeSideEffect(op) ) CSetTreeNodeSideEffect(win);
          if ( CIsTreeNodeVolatile(op) ) CSetTreeNodeVolatile(win);
          if ( CIsTreeNodeRaises(op) ) CSetTreeNodeRaises( win );
        }
    }
  *unsignedp_ptr = uns;
  return win;
}


/* Make the integer constant T valid for its type
   by setting to 0 or 1 all the bits in the constant
   that don't belong in the type.
   Yield 1 if a signed overflow occurs, 0 otherwise.
   If OVERFLOW is nonzero, a signed overflow has already occurred
   in calculating T, so propagate it.

   Make the real constant T valid for its type by calling CHECK_FLOAT_VALUE,
   if it exists.  */

int c_force_fit_type(t, overflow)
     c_tree_node *t;
     int overflow;
{
  int low, high;
  int prec;

  if ( CTreeNodeFirstCode(t) == C_REAL_CST_NODE)
    {
#ifdef CHECK_FLOAT_VALUE
      CHECK_FLOAT_VALUE (CTreeTypeMode(CTreeType(t)), TREE_REAL_CST (t),
                         overflow);
#endif
      return overflow;
    }

  else if ( CTreeNodeFirstCode(t) != C_INT_CST_NODE)
    return overflow;

  low = CTreeIntCstLow(t);
  high = CTreeIntCstHigh(t);

  if (POINTER_TYPE_P (CTreeType(t)))
    prec = C_POINTER_SIZE;
  else
    prec = CTreeTypePrecision(CTreeType(t));

  /* First clear all bits that are beyond the type's precision.  */

  if (prec == 2 * C_HOST_BITS_PER_WIDE_INT)
    ;
  else if (prec > C_HOST_BITS_PER_WIDE_INT)
    {
      CTreeIntCstHigh(t)
        &= ~((C_HOST_WIDE_INT) (-1) << (prec - C_HOST_BITS_PER_WIDE_INT));
    }
  else
    {
      CTreeIntCstHigh(t) = 0;
      if (prec < C_HOST_BITS_PER_WIDE_INT)
        CTreeIntCstLow(t) &= ~((C_HOST_WIDE_INT) (-1) << prec);
    }

  /* Unsigned types do not suffer sign extension or overflow.  */
  if ( CIsTreeNodeUnsigned(CTreeType(t)))
    return overflow;

  /* If the value's sign bit is set, extend the sign.  */
  if (prec != 2 * C_HOST_BITS_PER_WIDE_INT
      && (prec > C_HOST_BITS_PER_WIDE_INT
          ? (CTreeIntCstHigh(t)
             & ((C_HOST_WIDE_INT) 1 << (prec - C_HOST_BITS_PER_WIDE_INT - 1)))
          : CTreeIntCstLow(t) & ((C_HOST_WIDE_INT) 1 << (prec - 1))))
    {
      /* Value is negative:
         set to 1 all the bits that are outside this type's precision.  */
      if (prec > C_HOST_BITS_PER_WIDE_INT)
        {
          CTreeIntCstHigh(t)
            |= ((C_HOST_WIDE_INT) (-1) << (prec - C_HOST_BITS_PER_WIDE_INT));
        }
      else
        {
          CTreeIntCstHigh(t) = -1;
          if (prec < C_HOST_BITS_PER_WIDE_INT)
            CTreeIntCstLow(t) |= ((C_HOST_WIDE_INT) (-1) << prec);
        }
    }

  /* Yield nonzero if signed overflow occurred.  */
  return
    ((overflow | (low ^ CTreeIntCstLow(t)) | (high ^ CTreeIntCstHigh(t)))
     != 0);
}

/* Return 1 if TTL and TTR are pointers to types that are equivalent,
   ignoring their qualifiers.  */

static int c_is_comp_target_types(ttl, ttr)
     c_tree_node *ttl;
     c_tree_node *ttr;
{
  int val;

  /* Give maybe_objc_comptypes a crack at letting these types through.  */
# ifdef LUDO_DONE
  if (val = maybe_objc_comptypes (ttl, ttr, 1) >= 0)
    return val;
# endif

  val = c_is_comptypes(CTreeTypeMainVariant(CTreeType(ttl)),
                   CTreeTypeMainVariant(CTreeType(ttr)));

  if (val == 2 && c_pedantic)
  {
    pedwarn ("types are not quite compatible");
  }
  return val;
}

/* Nonzero if the type T promotes to itself.
   ANSI C states explicitly the list of types that promote;
   in particular, short promotes to int even if they have the same width.  */
#define C_PROMOTING_INTEGER_TYPE_P(t)                           \
  ( CTreeTypeCode((t)) == C_INTEGER_TYPE                        \
   && (CTreeTypeMainVariant(t) == c_char_type_node              \
       || CTreeTypeMainVariant(t) == c_signed_char_type_node    \
       || CTreeTypeMainVariant(t) == c_unsigned_char_type_node  \
       || CTreeTypeMainVariant(t) == c_short_integer_type_node  \
       || CTreeTypeMainVariant(t) == c_short_unsigned_type_node))


/* Return 1 if TYPE is not affected by default promotions.  */

static int c_is_self_promoting_type_p(type)
     c_tree_node *type;
{
  if (CTreeTypeMainVariant(type) == c_float_type_node)
    return 0;

  if (C_PROMOTING_INTEGER_TYPE_P(type))
    return 0;

  return 1;
}


/* Check two lists of types for compatibility,
   returning 0 for incompatible, 1 for compatible,
   or 2 for compatible with warning.  */

static int c_is_type_lists_compatible_p(args1, args2)
     c_tree_node *args1;
     c_tree_node *args2;
{
  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  int val = 1;
  int newval = 0;

  while (1)
    {
      if (args1 == 0 && args2 == 0)
        return val;
      /* If one list is shorter than the other,
         they fail to match.  */
      if (args1 == 0 || args2 == 0)
        return 0;
      /* A null pointer instead of a type
         means there is supposed to be an argument
         but nothing is specified about what type it has.
         So match anything that self-promotes.  */
      if ( CTreeListValue(args1) == 0)
        {
          if (! c_is_self_promoting_type_p( CTreeListValue(args2)))
            return 0;
        }
      else if (CTreeListValue(args2) == 0)
        {
          if (! c_is_self_promoting_type_p(CTreeListValue(args1)))
            return 0;
        }
      else if (! (newval = c_is_comptypes(CTreeListValue(args1), CTreeListValue(args2))))
        {
          /* Allow  wait (union {union wait *u; int *i} *)
             and  wait (union wait *)  to be compatible.  */
          if ( CTreeTypeCode(CTreeListValue(args1)) == C_UNION_TYPE
              && ( CTreeTypeName(CTreeListValue(args1)) == 0
                  || CIsTreeTypeTransUnion(CTreeListValue(args1)))
              && CTreeNodeFirstCode(CTreeTypeSize(CTreeListValue(args1))) == C_INT_CST_NODE
              && c_tree_is_int_cst_equal(CTreeTypeSize(CTreeListValue(args1)),
                                     CTreeTypeSize(CTreeListValue(args2))))
            {
              c_tree_node *memb;
              for (memb = CTreeTypeValues(CTreeListValue(args1));
                   memb; memb = CTreeChain(memb))
                if ( c_is_comptypes(CTreeType(memb), CTreeListValue(args2)))
                  break;
              if (memb == 0)
                return 0;
            }
          else if ( CTreeTypeCode(CTreeListValue(args2)) == C_UNION_TYPE
                   && ( CTreeTypeName(CTreeListValue(args2)) == 0
                       || CIsTreeTypeTransUnion(CTreeListValue(args2)))
                   && CTreeNodeFirstCode(CTreeTypeSize(CTreeListValue(args2))) == C_INT_CST_NODE
                   && c_tree_is_int_cst_equal(CTreeTypeSize(CTreeListValue(args2)),
                                          CTreeTypeSize(CTreeListValue(args1))))
            {
              c_tree_node *memb;
              for (memb = CTreeTypeValues(CTreeListValue(args2));
                   memb; memb = CTreeChain(memb))
                if ( c_is_comptypes(CTreeType(memb), CTreeListValue(args1)))
                  break;
              if (memb == 0)
                return 0;
            }
          else
            return 0;
        }

      /* comptypes said ok, but record if it said to warn.  */
      if (newval > val)
        val = newval;

      args1 = CTreeChain(args1);
      args2 = CTreeChain(args2);
    }
}

/* Return 1 if two function types F1 and F2 are compatible.
   If either type specifies no argument types,
   the other must specify a fixed number of self-promoting arg types.
   Otherwise, if one type specifies only the number of arguments, 
   the other must specify that number of self-promoting arg types.
   Otherwise, the argument types must match.  */

static int c_is_function_types_compatible_p(f1, f2)
     c_tree_node *f1;
     c_tree_node *f2;
{
  c_tree_node *args1;
  c_tree_node *args2;
  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  int val = 1;
  int val1;

  if (!(CTreeType(f1) == CTreeType(f2)
        || (val = c_is_comptypes(CTreeType(f1), CTreeType(f2)))))
    return 0;

  args1 = CTreeTypeValues(f1);
  args2 = CTreeTypeValues(f2);

  /* An unspecified parmlist matches any specified parmlist
     whose argument types don't need default promotions.  */

  if (args1 == 0)
    {
      if (! c_is_self_promoting_args_p(args2))
        return 0;
      /* If one of these types comes from a non-prototype fn definition,
         compare that with the other type's arglist.
         If they don't match, ask for a warning (but no error).  */
      if ( CTreeTypeNonCopiedParts(f1)
          && 1 != c_is_type_lists_compatible_p(args2, CTreeTypeNonCopiedParts(f1)))
        val = 2;
      return val;
    }
  if (args2 == 0)
    {
      if (! c_is_self_promoting_args_p(args1))
        return 0;
      if ( CTreeTypeNonCopiedParts(f2)
          && 1 != c_is_type_lists_compatible_p(args1, CTreeTypeNonCopiedParts(f2)))
        val = 2;
      return val;
    }

  /* Both types have argument lists: compare them and propagate results.  */
  val1 = c_is_type_lists_compatible_p(args1, args2);
  return val1 != 1 ? val1 : val;
}


/* Return 1 if TYPE1 and TYPE2 are compatible types for assignment
   or various other operations.  Return 2 if they are compatible
   but a warning may be needed if you use them together.  */

int c_is_comptypes(type1, type2)

     c_tree_node *type1;
     c_tree_node *type2;
{
  c_tree_node *t1 = type1;
  c_tree_node *t2 = type2;
  int attrval, val;

  /* Suppress errors caused by previously reported errors.  */

  if (t1 == t2 || 
      CTreeNodeFirstCode(t1) == C_ERROR_NODE ||
      CTreeNodeFirstCode(t2) == C_ERROR_NODE )
  {
    return 1;
  }

  /* Treat an enum type as the integer type of the same width and 
     signedness.  */

  if ( CTreeTypeCode(t1) == C_ENUMERAL_TYPE)
    t1 = c_get_type_for_size( CTreeTypePrecision(t1), CIsTreeNodeUnsigned(t1));

  if ( CTreeTypeCode(t2) == C_ENUMERAL_TYPE)
    t2 = c_get_type_for_size(CTreeTypePrecision(t2), CIsTreeNodeUnsigned(t2));

  if (t1 == t2)
    return 1;

  /* Different classes of types can't be compatible.  */

  if ( CTreeTypeCode(t1) != CTreeTypeCode(t2)) return 0;

  /* Qualifiers must match.  */

  if ( CIsTreeNodeReadOnly(t1) != CIsTreeNodeReadOnly(t2))
    return 0;
  if ( CIsTreeNodeVolatile(t1) != CIsTreeNodeVolatile(t2))
    return 0;

  /* Allow for two different type nodes which have essentially the same
     definition.  Note that we already checked for equality of the type
     type qualifiers (just above).  */

  if (CTreeTypeMainVariant(t1) == CTreeTypeMainVariant(t2))
    return 1;

#ifndef COMP_TYPE_ATTRIBUTES
#define COMP_TYPE_ATTRIBUTES(t1,t2)        1
#endif

  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  if (! (attrval = COMP_TYPE_ATTRIBUTES(t1, t2)))
     return 0;

  /* 1 if no need for warning yet, 2 if warning cause has been seen.  */
  val = 0;

  switch ( CTreeTypeCode(t1))
    {
    case C_POINTER_TYPE:
      val = (CTreeType(t1) == CTreeType(t2)
              ? 1 : c_is_comptypes(CTreeType(t1), CTreeType(t2)));
      break;

    case C_FUNCTION_TYPE:
      val = c_is_function_types_compatible_p(t1, t2);
      break;

    case C_ARRAY_TYPE:
      {
        c_tree_node *d1 = CTreeTypeValues(t1);
        c_tree_node *d2 = CTreeTypeValues(t2);
        val = 1;

        /* Target types must match incl. qualifiers.  */
        if (CTreeType(t1) != CTreeType(t2)
            && 0 == (val = c_is_comptypes(CTreeType(t1), CTreeType(t2))))
          return 0;

        /* Sizes must match unless one is missing or variable.  */
        if (d1 == 0 || d2 == 0 || d1 == d2
            ||  CTreeNodeFirstCode( CTreeTypeMinValue(d1)) != C_INT_CST_NODE
            ||  CTreeNodeFirstCode( CTreeTypeMinValue(d2)) != C_INT_CST_NODE
            ||  CTreeNodeFirstCode( CTreeTypeMaxValue(d1)) != C_INT_CST_NODE
            ||  CTreeNodeFirstCode( CTreeTypeMaxValue(d2)) != C_INT_CST_NODE)
          break;

        if (! ((CTreeIntCstLow(CTreeTypeMinValue(d1))
                  == CTreeIntCstLow(CTreeTypeMinValue(d2)))
                 && (CTreeIntCstHigh(CTreeTypeMinValue(d1))
                     == CTreeIntCstHigh(CTreeTypeMinValue(d2)))
                 && (CTreeIntCstLow(CTreeTypeMaxValue(d1))
                     == CTreeIntCstLow(CTreeTypeMaxValue(d2)))
                 && (CTreeIntCstHigh(CTreeTypeMaxValue(d1))
                     == CTreeIntCstHigh(CTreeTypeMaxValue(d2)))))
           val = 0;
        break;
      }

    case C_RECORD_TYPE:
# ifdef LUDO_DONE
      if (maybe_objc_comptypes (t1, t2, 0) == 1)
        val = 1;
# endif
      break;

    default:
      break;
    }
  return attrval == 2 && val == 1 ? 2 : val;
}


/* Convert value RHS to type TYPE as preparation for an assignment
   to an lvalue of type TYPE.
   The real work of conversion is done by `convert'.
   The purpose of this function is to generate error messages
   for assignments that are not allowed in C.
   ERRTYPE is a string to use in error messages:
   "assignment", "return", etc.  If it is null, this is parameter passing
   for a function call (and different error messages are output).  Otherwise,
   it may be a name stored in the spelling stack and interpreted by
   get_spelling.

   FUNNAME is the name of the function being called,
   as an IDENTIFIER_NODE, or null.
   PARMNUM is the number of the argument, for printing in error messages.  */

static c_tree_node *c_convert_for_assignment(
    type, rhs, errtype, fundecl, funname, parmnum)
     c_tree_node *type;
     c_tree_node *rhs;
     char *errtype;
     c_tree_node *fundecl;
     c_tree_node *funname;
     int parmnum;
{
  int codel = CTreeTypeCode(type);
  c_tree_node *rhstype;
  c_tree_node *tmp_node;
  int coder;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  /* Do not use STRIP_NOPS here.  We do not want an enumerator
     whose value is 0 to count as a null pointer constant.  */
  if ( ( CTreeNodeFirstCode(rhs) == C_EXPR_NODE ) &&
       ( CTreeExprCode(rhs) == C_NON_LVALUE_EXPR) )
  {
    rhs = CTreeExprOperand (rhs, 0);
  }

  if ( CTreeTypeCode(CTreeType(rhs)) == C_ARRAY_TYPE
      ||  CTreeTypeCode(CTreeType(rhs)) == C_FUNCTION_TYPE)
  {
    rhs = c_default_conversion(rhs);
  }
# ifdef LUDO_DONE
  else if (optimize && TREE_CODE (rhs) == VAR_DECL)
    rhs = c_decl_constant_value (rhs);
# endif

  rhstype = CTreeType(rhs);

  if ( CTreeNodeFirstCode( rhstype ) == C_ERROR_NODE)
    return c_error_mark_node;

  coder = CTreeTypeCode(rhstype);

  if (CTreeTypeMainVariant(type) == CTreeTypeMainVariant(rhstype))
    {
# ifdef LUDO_DONE
      overflow_warning (rhs);
      /* Check for Objective-C protocols.  This will issue a warning if
         there are protocol violations.  No need to use the return value.  */
      maybe_objc_comptypes (type, rhstype, 0);
# endif
      return rhs;
    }

  if (coder == C_VOID_TYPE)
    {
      error ("void value not ignored as it ought to be");
      return c_error_mark_node;
    }
  /* Arithmetic types all interconvert, and enum is treated like int.  */
  if ((codel == C_INTEGER_TYPE || codel == C_REAL_TYPE || codel == C_ENUMERAL_TYPE
       || codel == C_COMPLEX_TYPE)
      && (coder == C_INTEGER_TYPE || coder == C_REAL_TYPE || coder == C_ENUMERAL_TYPE
          || coder == C_COMPLEX_TYPE))
  {
    return c_convert_and_check(type, rhs);
  }

  /* Conversion to a transparent union from its member types.
     This applies only to function arguments.  */
  else if (codel == C_UNION_TYPE && CIsTreeTypeTransUnion(type) && ! errtype)
    {
      c_tree_node *memb_types;
      c_tree_node *marginal_memb_type = 0;

      for (memb_types = CTreeTypeValues(type); memb_types;
           memb_types = CTreeChain(memb_types))
        {
          c_tree_node *memb_type = CTreeType(memb_types);

          if (c_is_comptypes(CTreeTypeMainVariant(memb_type),
                         CTreeTypeMainVariant(rhstype)))
            break;

          if ( CTreeTypeCode(memb_type) != C_POINTER_TYPE)
            continue;

          if (coder == C_POINTER_TYPE)
            {
              c_tree_node *ttl = CTreeType(memb_type);
              c_tree_node *ttr = CTreeType(rhstype);

              /* Any non-function converts to a [const][volatile] void *
                 and vice versa; otherwise, targets must be the same.
                 Meanwhile, the lhs target must have all the qualifiers of
                 the rhs.  */
              if (CTreeTypeMainVariant(ttl) == c_void_type_node
                  || CTreeTypeMainVariant(ttr) == c_void_type_node
                  || c_is_comp_target_types(memb_type, rhstype))
                {
                  /* If this type won't generate any warnings, use it.  */
                  if ((CTreeTypeCode(ttr) == C_FUNCTION_TYPE
                       && CTreeTypeCode(ttl) == C_FUNCTION_TYPE)
                      ? ((! CIsTreeNodeReadOnly(ttl) | CIsTreeNodeReadOnly(ttr))
                         & (! CIsTreeNodeVolatile(ttl) | CIsTreeNodeVolatile(ttr)))
                      : ((CIsTreeNodeReadOnly(ttl) | ! CIsTreeNodeReadOnly(ttr))
                         & (CIsTreeNodeVolatile(ttl) | ! CIsTreeNodeVolatile(ttr))))
                    break;

                  /* Keep looking for a better type, but remember this one.  */
                  if (! marginal_memb_type)
                    marginal_memb_type = memb_type;
                }
            }

          /* Can convert integer zero to any pointer type.  */
          if (c_tree_is_integer_zerop(rhs)
              || ( CTreeExprCode(rhs) == C_NOP_EXPR
                  && c_tree_is_integer_zerop(CTreeExprOperand(rhs, 0))))
            {
              rhs = c_null_pointer_node;
              break;
            }
        }

      if (memb_types || marginal_memb_type)
        {
          if (! memb_types)
            {
              /* We have only a marginally acceptable member type;
                 it needs a warning.  */
              c_tree_node *ttl = CTreeType(marginal_memb_type);
              c_tree_node *ttr = CTreeType(rhstype);

              /* Const and volatile mean something different for function
                 types, so the usual warnings are not appropriate.  */
              if ( CTreeTypeCode(ttr) == C_FUNCTION_TYPE
                  &&  CTreeTypeCode(ttl) == C_FUNCTION_TYPE)
                {
                  /* Because const and volatile on functions are
                     restrictions that say the function will not do
                     certain things, it is okay to use a const or volatile
                     function where an ordinary one is wanted, but not
                     vice-versa.  */
                  if ( CIsTreeNodeReadOnly(ttl) && ! CIsTreeNodeReadOnly(ttr))
                  {
                    warn_for_assignment(
                        "%s makes `const *' function pointer from non-const",
                        c_get_spelling(errtype), funname, parmnum);
                  }
                  if ( CIsTreeNodeVolatile(ttl) && ! CIsTreeNodeVolatile(ttr))
                  {
                    warn_for_assignment(
                        "%s makes `volatile *' function pointer from non-volatile",
                        c_get_spelling (errtype), funname, parmnum);
                  }
                }
              else
                {
                  if (! CIsTreeNodeReadOnly(ttl) && CIsTreeNodeReadOnly(ttr))
                  {
                    warn_for_assignment(
                        "%s discards `const' from pointer target type",
                        c_get_spelling (errtype), funname, parmnum);
                  }
                  if (! CIsTreeNodeVolatile(ttl) && CIsTreeNodeVolatile(ttr))
                  {
                    warn_for_assignment(
                        "%s discards `volatile' from pointer target type",
                        c_get_spelling (errtype), funname, parmnum);
                  }
                }
            }
          
          if (c_pedantic && ! CIsTreeDeclSystemHeader(fundecl))
          {
            pedwarn ("ANSI C prohibits argument conversion to union type");
          }

          tmp_node = c_tree_build_unary_expr( C_NOP_EXPR, rhs );
          CTreeType( tmp_node ) = type;
          return( tmp_node );
        }
    }

  /* Conversions among pointers */
  else if (codel == C_POINTER_TYPE && coder == C_POINTER_TYPE)
    {
      c_tree_node *ttl = CTreeType(type);
      c_tree_node *ttr = CTreeType(rhstype);

      /* Any non-function converts to a [const][volatile] void *
         and vice versa; otherwise, targets must be the same.
         Meanwhile, the lhs target must have all the qualifiers of the rhs.  */
      if (CTreeTypeMainVariant(ttl) == c_void_type_node
          || CTreeTypeMainVariant(ttr) == c_void_type_node
          || c_is_comp_target_types(type, rhstype)
          || (c_get_unsigned_type(CTreeTypeMainVariant(ttl))
              == c_get_unsigned_type(CTreeTypeMainVariant(ttr))))
        {
          if (c_pedantic
              && ((CTreeTypeMainVariant(ttl) == c_void_type_node
                   && CTreeTypeCode(ttr) == C_FUNCTION_TYPE)
                  ||
                  (CTreeTypeMainVariant(ttr) == c_void_type_node
                   /* Check TREE_CODE to catch cases like (void *) (char *) 0
                      which are not ANSI null ptr constants.  */
                   && (!c_tree_is_integer_zerop(rhs) || CTreeExprCode(rhs) == C_NOP_EXPR)
                   && CTreeTypeCode(ttl) == C_FUNCTION_TYPE)))
          {
            warn_for_assignment( "ANSI forbids %s between function pointer and `void *'",
                c_get_spelling (errtype), funname, parmnum);
          }
          /* Const and volatile mean something different for function types,
             so the usual warnings are not appropriate.  */
          else if ( CTreeTypeCode(ttr) != C_FUNCTION_TYPE
                   && CTreeTypeCode(ttl) != C_FUNCTION_TYPE)
          {
              if (! CIsTreeNodeReadOnly(ttl) && CIsTreeNodeReadOnly(ttr))
              {
                warn_for_assignment("%s discards `const' from pointer target type",
                   c_get_spelling (errtype), funname, parmnum);
              }
              else if (! CIsTreeNodeVolatile(ttl) && CIsTreeNodeVolatile(ttr))
              {
                c_warn_for_assignment ("%s discards `volatile' from pointer target type",
                                     c_get_spelling (errtype), funname, parmnum);
              }
              /* If this is not a case of ignoring a mismatch in signedness,
                 no warning.  */
              else if (CTreeTypeMainVariant(ttl) == c_void_type_node
                       || CTreeTypeMainVariant(ttr) == c_void_type_node
                       || c_is_comp_target_types(type, rhstype))
                ;
              /* If there is a mismatch, do warn.  */
              else if (c_pedantic)
              {
                warn_for_assignment ("pointer targets in %s differ in signedness",
                                     c_get_spelling (errtype), funname, parmnum);
              }
          }
          else if ( CTreeTypeCode(ttl) == C_FUNCTION_TYPE
                   && CTreeTypeCode(ttr) == C_FUNCTION_TYPE)
          {
              /* Because const and volatile on functions are restrictions
                 that say the function will not do certain things,
                 it is okay to use a const or volatile function
                 where an ordinary one is wanted, but not vice-versa.  */
              if ( CIsTreeNodeReadOnly(ttl) && ! CIsTreeNodeReadOnly(ttr))
              {
                warn_for_assignment ("%s makes `const *' function pointer from non-const",
                                     c_get_spelling (errtype), funname, parmnum);
              }
              if ( CIsTreeNodeVolatile(ttl) && ! CIsTreeNodeVolatile(ttr))
              {
                warn_for_assignment("%s makes `volatile *' function pointer from non-volatile",
                                     c_get_spelling (errtype), funname, parmnum);
              }
          }
        }
      else
      {
        warn_for_assignment("%s from incompatible pointer type",
                             c_get_spelling (errtype), funname, parmnum);
      }

      return c_convert_expr(type, rhs);
    }
  else if (codel == C_POINTER_TYPE && coder == C_INTEGER_TYPE)
    {
      /* An explicit constant 0 can convert to a pointer,
         or one that results from arithmetic, even including
         a cast to integer type.  */
      if (! ( CTreeNodeFirstCode(rhs) == C_INT_CST_NODE && c_tree_is_integer_zerop(rhs))
          &&
          ! ( CTreeExprCode(rhs) == C_NOP_EXPR
             && CTreeTypeCode(CTreeType(rhs)) == C_INTEGER_TYPE
             && CTreeNodeFirstCode(CTreeExprOperand(rhs, 0)) == C_INT_CST_NODE
             && c_tree_is_integer_zerop(CTreeExprOperand(rhs, 0))))
        {
          warn_for_assignment("%s makes pointer from integer without a cast",
                               c_get_spelling (errtype), funname, parmnum);
          return c_convert_expr(type, rhs);
        }
      return c_null_pointer_node;
    }
  else if (codel == C_INTEGER_TYPE && coder == C_POINTER_TYPE)
    {
      warn_for_assignment("%s makes integer from pointer without a cast",
                           c_get_spelling (errtype), funname, parmnum);
      return c_convert_expr(type, rhs);
    }

  if (!errtype)
    {
      if (funname)
         {
# ifdef LUDO_NO_SKIP
           c_tree_node *selector = maybe_building_objc_message_expr ();
# else
           c_tree_node *selector = 0;
# endif
 
           if (selector && parmnum > 2)
           {
             error ("incompatible type for argument %d of `%s'",
                   parmnum - 2, CTreeIdentPointer(selector));
           }
           else
           {
            error ("incompatible type for argument %d of `%s'",
                   parmnum,  CTreeIdentPointer(funname));
           }
        }
      else
      {
        error ("incompatible type for argument %d of indirect function call",
               parmnum);
      }
    }
  else
  {
    error ("incompatible types in %s", c_get_spelling(errtype));
  }

  return c_error_mark_node;
}

/*------------------------------------------------------------\
|                                                             |
|                  C Parser Build Indirect Reference          |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_parser_build_indirect_ref( ptr, errorstring)
     c_tree_node *ptr;
     char *errorstring;
{
  c_tree_node *pointer = c_default_conversion(ptr);
  c_tree_node *type = CTreeType(pointer);

  if ( CTreeTypeCode(type) == C_POINTER_TYPE)
  {
    if ( CTreeExprCode(pointer) == C_ADDR_EXPR
        && !c_flag_volatile
        && ( CTreeType(CTreeExprOperand(pointer, 0))
            == CTreeType(type)))
    {
      return CTreeExprOperand(pointer, 0);
    }
    else
    {
      c_tree_node *t = CTreeType(type);
      c_tree_node *ref = c_tree_build_unary_expr( C_INDIRECT_REF, pointer );
      CTreeType( ref ) = CTreeTypeMainVariant(t);

      if ( CTreeTypeSize(t) == 0 &&  CTreeTypeCode(t) != C_ARRAY_TYPE)
      {
          error ("dereferencing pointer to incomplete type");
          return c_error_mark_node;
      }
      if ( CTreeTypeCode(t) == C_VOID_TYPE && c_skip_evaluation == 0)
      {
        warning ("dereferencing `void *' pointer");
      }

      /* We *must* set TREE_READONLY when dereferencing a pointer to const,
         so that we get the proper error message if the result is used
         to assign to.  Also, &* is supposed to be a no-op.
         And ANSI C seems to specify that the type of the result
         should be the const type.  */
      /* A de-reference of a pointer to const is not a const.  It is valid
         to change it via some other pointer.  */
      if ( CIsTreeNodeReadOnly(t) )
      {
        CSetTreeNodeReadOnly(ref);
      }
      if ( CIsTreeNodeVolatile(t) ||
           CIsTreeNodeSideEffect(pointer) ||
           c_flag_volatile ) CSetTreeNodeSideEffect( ref );

      if ( CIsTreeNodeVolatile(t) ) CSetTreeNodeVolatile( ref );
      return ref;
    }
  }
  else if ( CTreeNodeFirstCode(pointer) != C_ERROR_NODE)
  {
    error ("invalid type argument of `%s'", errorstring);
  }
  return c_error_mark_node;
}

/*------------------------------------------------------------\
|                                                             |
|                      C Build Unary Oper                     |
|                                                             |
\------------------------------------------------------------*/

/* Return nonzero if REF is an lvalue valid for this language.
   Lvalues can be assigned, unless their type has TYPE_READONLY.
   Lvalues can have their address taken, unless they have DECL_REGISTER.  */

int
c_lvalue_p (ref)
     c_tree_node *ref;
{
  int  code = CTreeNodeFirstCode(ref);

  if ( code == C_STRING_NODE)
  {
    return 1;
  }
  else
  if ( code == C_DECL_NODE )
  {
    code = CTreeExprCode( ref );
    switch (code)
    {
    case C_VAR_DECL:
    case C_PARAM_DECL:
    case C_RESULT_DECL:
      return ( CTreeTypeCode(CTreeType(ref)) != C_FUNCTION_TYPE
              &&  CTreeTypeCode(CTreeType(ref)) != C_METHOD_TYPE);
    }
  }
  else
  if ( code == C_ERROR_NODE )
  {
     return ( CTreeTypeCode(CTreeType(ref)) != C_FUNCTION_TYPE
              &&  CTreeTypeCode(CTreeType(ref)) != C_METHOD_TYPE);
  }
  else
  if ( code == C_EXPR_NODE )
  {
    code = CTreeExprCode( ref );
    switch (code)
    {
    case C_REALPART_EXPR:
    case C_IMAGPART_EXPR:
    case C_COMPONENT_REF:
      return c_lvalue_p( CTreeExprOperand(ref, 0));

    case C_INDIRECT_REF:
    case C_ARRAY_REF:
      return ( CTreeTypeCode(CTreeType(ref)) != C_FUNCTION_TYPE
              &&  CTreeTypeCode(CTreeType(ref)) != C_METHOD_TYPE);

    case C_BIND_EXPR:
    case C_RTL_EXPR:
      return CTreeTypeCode(CTreeType(ref)) == C_ARRAY_TYPE;

    default:
      return 0;
    }
  }

  return 0;
}


/* Return nonzero if REF is an lvalue valid for this language;
   otherwise, print an error message and return zero.  */

int c_lvalue_or_else (ref, string)
     c_tree_node *ref;
     char *string;
{
  int win = c_lvalue_p (ref);
  if (! win)
  {
    error ("invalid lvalue in %s", string);
  }
  return win;
}


/* Mark EXP saying that we need to be able to take the
   address of it; it should not be allocated in a register.
   Value is 1 if successful.  */

int c_mark_addressable (exp)
     c_tree_node *exp;
{
  c_tree_node *x = exp;
  while (1)
  {
    if (CTreeNodeFirstCode(x) == C_EXPR_NODE &&
        CTreeExprCode(x) == C_COMPONENT_REF )
    {
        if ( CIsTreeDeclBitField(CTreeExprOperand(x, 1)))
          {
            error ("cannot take address of bitfield `%s'",
                   CTreeIdentPointer(CTreeDeclName(CTreeExprOperand(x, 1))));
            return 0;
          }
    }
    else
        /* ... fall through ...  */
    if ( CTreeNodeFirstCode(x) == C_EXPR_NODE &&
         (CTreeExprCode(x) == C_ADDR_EXPR ||
          CTreeExprCode(x) == C_ARRAY_REF ||
          CTreeExprCode(x) == C_REALPART_EXPR ||
          CTreeExprCode(x) == C_IMAGPART_EXPR ))
   {
     x = CTreeExprOperand(x, 0); continue;
   }
   else
   if ( CTreeNodeFirstCode(x) == C_EXPR_NODE &&
        CTreeExprCode(x) == C_CONSTRUCTOR_EXPR)
   {
     CSetTreeNodeAddressable(x);
     return 1;
   }
   else
   if ( CTreeNodeFirstCode( x ) == C_DECL_NODE &&
        (CTreeDeclCode(x ) == C_VAR_DECL ||
         CTreeDeclCode(x ) == C_CONST_DECL ||
         CTreeDeclCode(x ) == C_PARAM_DECL ||
         CTreeDeclCode(x ) == C_RESULT_DECL ))
   {
     if ( CIsTreeDeclRegister(x) && ! CIsTreeNodeAddressable(x)
            && CIsTreeDeclNonLocal(x))
     {
       if ( CIsTreeNodePublic(x))
       {
         error ("global register variable `%s' used in nested function",
                        CTreeIdentPointer(CTreeDeclName(x)));
         return 0;
       }
       pedwarn ("register variable `%s' used in nested function",
                      CTreeIdentPointer(CTreeDeclName(x)));
     }
     else if ( CIsTreeDeclRegister(x) && ! CIsTreeNodeAddressable(x))
     {
            if ( CIsTreeNodePublic(x))
              {
                error ("address of global register variable `%s' requested",
                       CTreeIdentPointer(CTreeDeclName(x)));
                return 0;
              }

            /* If we are making this addressable due to its having
               volatile components, give a different error message.  Also
               handle the case of an unnamed parameter by not trying
               to give the name.  */

            else if (CIsTreeTypeFieldsVolatile(CTreeType(x)))
              {
                error ("cannot put object with volatile field into register");
                return 0;
              }

            pedwarn ("address of register variable `%s' requested",
                     CTreeIdentPointer(CTreeDeclName(x)));
      }
# ifdef LUDO_NO_SKIP
      put_var_into_stack (x);
# endif
    }
    else if ( CTreeNodeFirstCode( x) == C_DECL_NODE &&
             CTreeDeclCode( x ) == C_FUNCTION_DECL )
    {
      CSetTreeNodeAddressable(x);
    }

    return 1;
  }
}

/* Construct and perhaps optimize a c_tree_node *representation
   for a unary operation.  CODE, a tree_code, specifies the operation
   and XARG is the operand.  NOCONVERT nonzero suppresses
   the default promotions (such as from short to int).  */

c_tree_node *c_parser_build_unary_op(code, xarg, noconvert)
     int code;
     c_tree_node *xarg;
     int noconvert;
{
  /* No default_conversion here.  It causes trouble for ADDR_EXPR.  */
  c_tree_node *arg = xarg;
  c_tree_node *argtype = 0;
  int  typecode = CTreeTypeCode(CTreeType(arg));
  char *errstring = NULL;
  c_tree_node *val;

  if ( CTreeNodeFirstCode( CTreeType(arg) ) == C_ERROR_NODE)
    return c_error_mark_node;
  if (typecode == C_ENUMERAL_TYPE)
    typecode = C_INTEGER_TYPE;

  switch (code)
    {
    case C_CONVERT_EXPR:
      /* This is used for unary plus, because a CONVERT_EXPR
         is enough to prevent anybody from looking inside for
         associativity, but won't generate any code.  */
      if (!(typecode == C_INTEGER_TYPE || 
            typecode == C_REAL_TYPE || 
            typecode == C_COMPLEX_TYPE))
        errstring = "wrong type argument to unary plus";
      else if (!noconvert)
        arg = c_default_conversion(arg);
      break;

    case C_NEGATE_EXPR:
      if (!(typecode == C_INTEGER_TYPE || typecode == C_REAL_TYPE
            || typecode == C_COMPLEX_TYPE))
        errstring = "wrong type argument to unary minus";
      else if (!noconvert)
        arg = c_default_conversion(arg);
      break;

    case C_BIT_NOT_EXPR:
      if (typecode == C_COMPLEX_TYPE)
        {
          code = C_CONJ_EXPR;
          if (!noconvert)
            arg = c_default_conversion(arg);
        }
      else if (typecode != C_INTEGER_TYPE)
        errstring = "wrong type argument to bit-complement";
      else if (!noconvert)
        arg = c_default_conversion (arg);
      break;

    case C_ABS_EXPR:
      if (!(typecode == C_INTEGER_TYPE || typecode == C_REAL_TYPE
            || typecode == C_COMPLEX_TYPE))
        errstring = "wrong type argument to abs";
      else if (!noconvert)
        arg = c_default_conversion (arg);
      break;

    case C_CONJ_EXPR:
      /* Conjugating a real value is a no-op, but allow it anyway.  */
      if (!(typecode == C_INTEGER_TYPE || typecode == C_REAL_TYPE
            || typecode == C_COMPLEX_TYPE))
        errstring = "wrong type argument to conjugation";
      else if (!noconvert)
        arg = c_default_conversion (arg);
      break;

    case C_TRUTH_NOT_EXPR:
      if (typecode != C_INTEGER_TYPE
          && typecode != C_REAL_TYPE && typecode != C_POINTER_TYPE
          && typecode != C_COMPLEX_TYPE
          /* These will convert to a pointer.  */
          && typecode != C_ARRAY_TYPE && typecode != C_FUNCTION_TYPE)
        {
          errstring = "wrong type argument to unary exclamation mark";
          break;
        }
      arg = c_truthvalue_conversion(arg);
      return c_invert_truthvalue(arg);

    case C_NOP_EXPR:
      break;

    case C_REALPART_EXPR:
      if ( CTreeNodeFirstCode(arg) == C_COMPLEX_NODE)
        return CTreeComplexReal(arg);
      else if ( CTreeTypeCode(CTreeType(arg)) == C_COMPLEX_TYPE)
      {
        return c_fold ( c_tree_build_unary_typed_expr ( C_REALPART_EXPR, CTreeType(CTreeType(arg)), arg));
      }
      else
        return arg;

    case C_IMAGPART_EXPR:
      if ( CTreeNodeFirstCode(arg) == C_COMPLEX_NODE)
        return CTreeComplexImag(arg);
      else if ( CTreeNodeFirstCode(CTreeType(arg)) == C_COMPLEX_TYPE)
      {
        return c_fold ( c_tree_build_unary_typed_expr (C_IMAGPART_EXPR, CTreeType(CTreeType(arg)), arg));
      }
      else
        return c_convert_expr(CTreeType(arg), c_integer_zero_node);
      
    case C_PREINCREMENT_EXPR:
    case C_POSTINCREMENT_EXPR:
    case C_PREDECREMENT_EXPR:
    case C_POSTDECREMENT_EXPR:
      /* Handle complex lvalues (when permitted)
         by reduction to simpler cases.  */

      val = c_unary_complex_lvalue(code, arg);
      if (val != 0)
        return val;

      /* Increment or decrement the real part of the value,
         and don't change the imaginary part.  */
      if (typecode == C_COMPLEX_TYPE)
        {
          c_tree_node *real;
          c_tree_node *imag;

          arg = c_stabilize_reference(arg);
          real = c_parser_build_unary_op(C_REALPART_EXPR, arg, 1);
          imag = c_parser_build_unary_op(C_IMAGPART_EXPR, arg, 1);
          return c_tree_build_binary_typed_expr(C_COMPLEX_EXPR, CTreeType(arg),
                        c_parser_build_unary_op(code, real, 1), imag);
        }

      /* Report invalid types.  */

      if (typecode != C_POINTER_TYPE
          && typecode != C_INTEGER_TYPE && typecode != C_REAL_TYPE)
        {
          if (code == C_PREINCREMENT_EXPR || code == C_POSTINCREMENT_EXPR)
            errstring ="wrong type argument to increment";
          else
            errstring ="wrong type argument to decrement";
          break;
        }

      {
        c_tree_node *inc;
        c_tree_node *result_type = CTreeType(arg);

        arg = c_get_unwidened(arg, 0);
        argtype = CTreeType(arg);

        /* Compute the increment.  */

        if (typecode == C_POINTER_TYPE)
          {
            /* If pointer target is an undefined struct,
               we just cannot know how to do the arithmetic.  */
            if (CTreeTypeSize(CTreeType(result_type)) == 0)
            {
              error ("%s of pointer to unknown structure",
                       ((code == C_PREINCREMENT_EXPR
                         || code == C_POSTINCREMENT_EXPR)
                        ? "increment" : "decrement"));
            }
            else if ((c_pedantic || c_warn_pointer_arith)
                     && (CTreeTypeCode(CTreeType(result_type)) == C_FUNCTION_TYPE
                         || CTreeTypeCode(CTreeType(result_type)) == C_VOID_TYPE))
            {
              pedwarn ("wrong type argument to %s",
                       ((code == C_PREINCREMENT_EXPR
                         || code == C_POSTINCREMENT_EXPR)
                        ? "increment" : "decrement"));
            }
            inc = c_get_size_in_bytes(CTreeType(result_type));
          }
        else
          inc = c_integer_one_node;

        inc = c_convert_expr(argtype, inc);

        /* Handle incrementing a cast-expression.  */

        while (1)
          switch ( CTreeExprCode(arg))
            {
            case C_NOP_EXPR:
            case C_CONVERT_EXPR:
            case C_FLOAT_EXPR:
            case C_FIX_TRUNC_EXPR:
            case C_FIX_FLOOR_EXPR:
            case C_FIX_ROUND_EXPR:
            case C_FIX_CEIL_EXPR:
              c_pedantic_lvalue_warning(C_CONVERT_EXPR);
              /* If the real type has the same machine representation
                 as the type it is cast to, we can make better output
                 by adding directly to the inside of the cast.  */
              if (( CTreeTypeCode(CTreeType(arg))
                   ==  CTreeTypeCode(CTreeType(CTreeExprOperand(arg, 0))))
                  && (CTreeTypeMode(CTreeType(arg))
                      == CTreeTypeMode(CTreeType(CTreeExprOperand(arg, 0)))))
                arg = CTreeExprOperand(arg, 0);
              else
                {
                  c_tree_node *incremented;
                  c_tree_node *modify;
                  c_tree_node *value;
                  arg = c_stabilize_reference (arg);
                  if (code == C_PREINCREMENT_EXPR || code == C_PREDECREMENT_EXPR)
                    value = arg;
                  else
# ifdef LUDO_DONE
                    value = save_expr (arg);
# else
                    value = arg;

# endif
                  incremented = c_tree_build_binary_typed_expr (((code == C_PREINCREMENT_EXPR
                                         || code == C_POSTINCREMENT_EXPR)
                                        ? C_PLUS_EXPR : C_MINUS_EXPR),
                                       argtype, value, inc);
                  CSetTreeNodeSideEffect(incremented);
                  modify = c_parser_build_modify_expr(arg, C_NOP_EXPR, incremented);
                  value = c_tree_build_binary_typed_expr(
                      C_COMPOUND_EXPR, CTreeType(arg), modify, value);
                  CSetTreeNodeUsed(value);
                  return value;
                }
              break;

            default:
              goto give_up;
            }
      give_up:

        /* Complain about anything else that is not a true lvalue.  */
        if (!c_lvalue_or_else (arg, ((code == C_PREINCREMENT_EXPR
                                    || code == C_POSTINCREMENT_EXPR)
                                   ? "increment" : "decrement")))
          return c_error_mark_node;

        /* Report a read-only lvalue.  */
        if ( CIsTreeNodeReadOnly(arg))
        {
          c_readonly_warning (arg, 
                            ((code == C_PREINCREMENT_EXPR
                              || code == C_POSTINCREMENT_EXPR)
                             ? "increment" : "decrement"));
        }

        val = c_tree_build_binary_typed_expr (code, CTreeType(arg), arg, inc);
        CSetTreeNodeSideEffect(val);
        val = c_convert_expr(result_type, val);
        if ( CTreeTypeCode(val) != code)
        {
          CSetTreeNodeStatic(val);
        }
        return val;
      }

    case C_ADDR_EXPR:
      /* Note that this operation never does default_conversion
         regardless of NOCONVERT.  */

      /* Let &* cancel out to simplify resulting code.  */
      if ( CTreeExprCode(arg) == C_INDIRECT_REF)
        {
          /* Don't let this be an lvalue.  */
          if (c_lvalue_p (CTreeExprOperand(arg, 0)))
            return c_non_lvalue (CTreeExprOperand(arg, 0));
          return CTreeExprOperand(arg, 0);
        }

      /* For &x[y], return x+y */
      if ( CTreeExprCode(arg) == C_ARRAY_REF)
        {
          if (c_mark_addressable (CTreeExprOperand(arg, 0)) == 0)
            return c_error_mark_node;
          return c_build_binary_op(C_PLUS_EXPR, CTreeExprOperand(arg, 0),
                                  CTreeExprOperand(arg, 1), 1);
        }

      /* Handle complex lvalues (when permitted)
         by reduction to simpler cases.  */
      val = c_unary_complex_lvalue(code, arg);
      if (val != 0)
        return val;

      /* Allow the address of a constructor if all the elements
         are constant.  */
      if ( CTreeExprCode(arg) == C_CONSTRUCTOR_EXPR && CIsTreeNodeConstant(arg))
        ;
      /* Anything not already handled and not a true memory reference
         is an error.  */
      else if (typecode != C_FUNCTION_TYPE && !c_lvalue_or_else (arg, "unary `&'"))
        return c_error_mark_node;

      /* Ordinary case; arg is a COMPONENT_REF or a decl.  */
      argtype = CTreeType(arg);
      /* If the lvalue is const or volatile,
         merge that into the type that the address will point to.  */
      if ( (CTreeNodeFirstCode(arg) == C_DECL_NODE)
          || (CTreeNodeFirstCode(arg) == C_EXPR_NODE &&
              (CTreeExprCode(arg) == C_COMPONENT_REF ||
               CTreeExprCode(arg) == C_BIT_FIELD_REF ||
               CTreeExprCode(arg) == C_INDIRECT_REF ||
               CTreeExprCode(arg) == C_BUFFER_REF ||
               CTreeExprCode(arg) == C_ARRAY_REF
               )))
        {
          if ( CIsTreeNodeReadOnly(arg) || CIsTreeNodeVolatile(arg))
            argtype = c_tree_build_type_variant(
                argtype, CIsTreeNodeReadOnly(arg),
                         CIsTreeNodeVolatile(arg));
        }

      argtype = c_tree_build_pointer_type(argtype);

      if (c_mark_addressable (arg) == 0)
        return c_error_mark_node;

      {
        c_tree_node *addr;

        if ( CTreeExprCode(arg) == C_COMPONENT_REF)
          {
            c_tree_node *field = CTreeExprOperand(arg, 1);

            addr = c_parser_build_unary_op( C_ADDR_EXPR, CTreeExprOperand(arg, 0), 0);

            if ( CIsTreeDeclBitField(field))
              {
                error ("attempt to take address of bit-field structure member `%s'",
                       CTreeIdentPointer(CTreeDeclName(field)));
                return c_error_mark_node;
              }

            addr = c_convert_expr(argtype, addr);

            if (! c_tree_is_integer_zerop( CTreeDeclArguments(field)))
              {
                c_tree_node *offset
                  = c_build_size_binop( C_TRUNC_DIV_EXPR, 
                      CTreeDeclArguments(field), c_tree_get_size_int( C_BITS_PER_UNIT));
                int flag = CIsTreeNodeConstant(addr);

                addr = c_fold (c_tree_build_binary_typed_expr(C_PLUS_EXPR, argtype,
                                    addr, c_convert_expr(argtype, offset)));
                if ( flag ) CSetTreeNodeConstant( addr );
              }
          }
        else
          addr = c_tree_build_unary_typed_expr(code, argtype, arg);

        /* Address of a static or external variable or
           file-scope function counts as a constant.  */
        if (c_is_staticp(arg)
            && ! ( CTreeNodeFirstCode(arg) == C_DECL_NODE && 
                   CTreeDeclCode(arg) == C_FUNCTION_DECL &&
                   CTreeDeclContext(arg) != 0))
        {
          CSetTreeNodeConstant( addr );
        }

        return addr;
      }

    default:
      break;
    }

  if (!errstring)
    {
      if (argtype == 0)
        argtype = CTreeType(arg);
      return c_fold(c_tree_build_unary_typed_expr(code, argtype, arg));
    }

  error (errstring);
  return c_error_mark_node;
}
/*------------------------------------------------------------\
|                                                             |
|                  C Parser Build Binary Oper                 |
|                                                             |
\------------------------------------------------------------*/


/* Return a c_tree_node *for the sum or difference (RESULTCODE says which)
   of pointer PTROP and integer INTOP.  */

static c_tree_node *c_pointer_int_sum(resultcode, ptrop, intop)
     int resultcode;
     c_tree_node *ptrop;
     c_tree_node *intop;
{
  c_tree_node *size_exp;

  c_tree_node *result;
  c_tree_node *folded;

  /* The result is a pointer of the same type that is being added.  */

  c_tree_node *result_type = CTreeType(ptrop);

  if ( CTreeTypeCode(CTreeType(result_type)) == C_VOID_TYPE)
    {
      if (c_pedantic || c_warn_pointer_arith)
      {
        pedwarn ("pointer of type `void *' used in arithmetic");
      }
      size_exp = c_integer_one_node;
    }
  else if ( CTreeTypeCode(CTreeType(result_type)) == C_FUNCTION_TYPE)
    {
      if (c_pedantic || c_warn_pointer_arith)
      {
        pedwarn ("pointer to a function used in arithmetic");
      }
      size_exp = c_integer_one_node;
    }
  else
    size_exp = c_get_size_in_bytes(CTreeType(result_type));

  /* If what we are about to multiply by the size of the elements
     contains a constant term, apply distributive law
     and multiply that constant term separately.
     This helps produce common subexpressions.  */

  if (( CTreeExprCode(intop) == C_PLUS_EXPR || 
        CTreeExprCode(intop) == C_MINUS_EXPR)
      && ! CIsTreeNodeConstant(intop)
      && CIsTreeNodeConstant(CTreeExprOperand(intop, 1))
      && CIsTreeNodeConstant(size_exp)
      /* If the constant comes from pointer subtraction,
         skip this optimization--it would cause an error.  */
      && CTreeTypeCode(CTreeType(CTreeExprOperand(intop, 0))) == C_INTEGER_TYPE
      /* If the constant is unsigned, and smaller than the pointer size,
         then we must skip this optimization.  This is because it could cause
         an overflow error if the constant is negative but INTOP is not.  */
      && (! CIsTreeNodeUnsigned(CTreeType(intop))
          || (CTreeTypePrecision(CTreeType(intop))
              == CTreeTypePrecision(CTreeType(ptrop)))))
    {
      int subcode = resultcode;
      c_tree_node *int_type = CTreeType(intop);
      if ( CTreeExprCode(intop) == C_MINUS_EXPR)
        subcode = (subcode == C_PLUS_EXPR ? C_MINUS_EXPR : C_PLUS_EXPR);
      /* Convert both subexpression types to the type of intop,
         because weird cases involving pointer arithmetic
         can result in a sum or difference with different type args.  */
      ptrop = c_build_binary_op(subcode, ptrop,
                               c_convert_expr(int_type, CTreeExprOperand(intop, 1)), 1);
      intop = c_convert_expr(int_type, CTreeExprOperand(intop, 0));
    }

  /* Convert the integer argument to a type the same size as c_size_type
     so the multiply won't overflow spuriously.  */

  if (CTreeTypePrecision(CTreeType(intop)) != CTreeTypePrecision(c_size_type)
      || CIsTreeNodeUnsigned(CTreeType(intop)) != CIsTreeNodeUnsigned(c_size_type))
    intop = c_convert_expr(c_get_type_for_size( CTreeTypePrecision(c_size_type), 
                                    CIsTreeNodeUnsigned(c_size_type)), intop);

  /* Replace the integer argument with a suitable product by the object size.
     Do this multiplication as signed, then convert to the appropriate
     pointer type (actually unsigned integral).  */

  intop = c_convert_expr( result_type,
                   c_build_binary_op( C_MULT_EXPR, intop,
                                    c_convert_expr(CTreeType(intop), size_exp), 1));

  /* Create the sum or difference.  */

  result = c_tree_build_binary_expr(resultcode, ptrop, intop);
  CTreeType( result ) = result_type;

  folded = c_fold (result);

  if (folded == result)
  {
    if ( CIsTreeNodeConstant( ptrop ) &&
         CIsTreeNodeConstant( intop ) )
    {
      CSetTreeNodeConstant( folded );
    }
  }
  return folded;
}


/* Return a c_tree_node *for the difference of pointers OP0 and OP1.
   The resulting c_tree_node *has type int.  */

static c_tree_node *c_pointer_diff(op0, op1)
     c_tree_node *op0;
     c_tree_node *op1;
{
  c_tree_node *result;
  c_tree_node *folded;
  c_tree_node *restype = c_ptrdiff_type_node;

  c_tree_node *target_type = CTreeType(CTreeType(op0));

  if (c_pedantic || c_warn_pointer_arith)
    {
      if ( CTreeTypeCode(target_type) == C_VOID_TYPE)
      {
        pedwarn ("pointer of type `void *' used in subtraction");
      }
      if ( CTreeTypeCode(target_type) == C_FUNCTION_TYPE)
      {
        pedwarn ("pointer to a function used in subtraction");
      }
    }

  /* First do the subtraction as integers;
     then drop through to build the divide operator.
     Do not do default conversions on the minus operator
     in case restype is a short type.  */

  op0 = c_build_binary_op( C_MINUS_EXPR, c_convert_expr(restype, op0),
                         c_convert_expr(restype, op1), 0);
  /* This generates an error if op1 is pointer to incomplete type.  */
  if (CTreeTypeSize(CTreeType(CTreeType(op1))) == 0)
  {
    error ("arithmetic on pointer to an incomplete type");
  }

  /* This generates an error if op0 is pointer to incomplete type.  */
  op1 = c_get_size_in_bytes(target_type);

  /* Divide by the size, in easiest possible way.  */

  result = c_tree_build_binary_expr( C_EXACT_DIV_EXPR, op0, c_convert_expr(restype, op1));
  CTreeType( result ) = restype;

  folded = c_fold (result);

  if (folded == result)
  {
    if ( CIsTreeNodeConstant(op0) &&
         CIsTreeNodeConstant(op1) )
    {
      CSetTreeNodeConstant( folded );
    }
  }
  return folded;
}

/* Return the common type of two types.
   We assume that comptypes has already been done and returned 1;
   if that isn't so, this may crash.  In particular, we assume that qualifiers
   match.

   This is the type for the result of most arithmetic operations
   if the operands have the given two types.  */

c_tree_node *c_get_common_type(t1, t2)
     c_tree_node *t1;
     c_tree_node *t2;
{
  int code1;
  int code2;
  c_tree_node *attributes;

  /* Save time if the two types are the same.  */

  if (t1 == t2) return t1;

  /* If one type is nonsense, use the other.  */
  if (t1 == c_error_mark_node)
    return t2;
  if (t2 == c_error_mark_node)
    return t1;

# ifdef LUDO_NO_SKIP
  /* Merge the attributes */
  attributes = c_merge_attributes (TYPE_ATTRIBUTES (t1), TYPE_ATTRIBUTES (t2));
# else
  attributes = NULL;
# endif

  /* Treat an enum type as the unsigned integer type of the same width.  */

  if ( CTreeTypeCode (t1) == C_ENUMERAL_TYPE)
    t1 = c_get_type_for_size(CTreeTypePrecision(t1), 1);
  if ( CTreeTypeCode (t2) == C_ENUMERAL_TYPE)
    t2 = c_get_type_for_size(CTreeTypePrecision(t2), 1);

  code1 = CTreeTypeCode(t1);
  code2 = CTreeTypeCode(t2);

  /* If one type is complex, form the common type of the non-complex
     components, then make that complex.  Use T1 or T2 if it is the
     required type.  */
  if (code1 == C_COMPLEX_TYPE || code2 == C_COMPLEX_TYPE)
    {
      c_tree_node *subtype1 = code1 == C_COMPLEX_TYPE ? CTreeType(t1) : t1;
      c_tree_node *subtype2 = code2 == C_COMPLEX_TYPE ? CTreeType(t2) : t2;
      c_tree_node *subtype = c_get_common_type(subtype1, subtype2);

      if (code1 == C_COMPLEX_TYPE && CTreeType(t1) == subtype)
        return c_tree_build_type_attribute_variant(t1, attributes);
      else if (code2 == C_COMPLEX_TYPE && CTreeType(t2) == subtype)
        return c_tree_build_type_attribute_variant(t2, attributes);
      else
        return c_tree_build_type_attribute_variant( 
                  c_tree_build_complex_type(subtype), attributes);
    }

  switch (code1)
    {
    case C_INTEGER_TYPE:
    case C_REAL_TYPE:
      /* If only one is real, use it as the result.  */

      if (code1 == C_REAL_TYPE && code2 != C_REAL_TYPE)
        return c_tree_build_type_attribute_variant(t1, attributes);

      if (code2 == C_REAL_TYPE && code1 != C_REAL_TYPE)
        return c_tree_build_type_attribute_variant(t2, attributes);

      /* Both real or both integers; use the one with greater precision.  */

      if (CTreeTypePrecision(t1) > CTreeTypePrecision(t2))
        return c_tree_build_type_attribute_variant(t1, attributes);
      else if (CTreeTypePrecision(t2) > CTreeTypePrecision(t1))
        return c_tree_build_type_attribute_variant(t2, attributes);

      /* Same precision.  Prefer longs to ints even when same size.  */

      if (CTreeTypeMainVariant(t1) == c_long_unsigned_type_node
          || CTreeTypeMainVariant(t2) == c_long_unsigned_type_node)
        return c_tree_build_type_attribute_variant(c_long_unsigned_type_node,
                                             attributes);

      if (CTreeTypeMainVariant(t1) == c_long_integer_type_node
          || CTreeTypeMainVariant(t2) == c_long_integer_type_node)
        {
          /* But preserve unsignedness from the other type,
             since long cannot hold all the values of an unsigned int.  */
          if (CIsTreeNodeUnsigned (t1) || CIsTreeNodeUnsigned (t2))
             t1 = c_long_unsigned_type_node;
          else
             t1 = c_long_integer_type_node;
          return c_tree_build_type_attribute_variant(t1, attributes);
        }

      /* Likewise, prefer long double to double even if same size.  */
      if (CTreeTypeMainVariant(t1) == c_long_double_type_node
          || CTreeTypeMainVariant(t2) == c_long_double_type_node)
        return c_tree_build_type_attribute_variant(c_long_double_type_node,
                                             attributes);

      /* Otherwise prefer the unsigned one.  */

      if (CIsTreeNodeUnsigned (t1))
        return c_tree_build_type_attribute_variant(t1, attributes);
      else
        return c_tree_build_type_attribute_variant(t2, attributes);

    case C_POINTER_TYPE:
      /* For two pointers, do this recursively on the target type,
         and combine the qualifiers of the two types' targets.  */
      /* This code was turned off; I don't know why.
         But ANSI C specifies doing this with the qualifiers.
         So I turned it on again.  */
      {
        c_tree_node *target = c_get_common_type (CTreeTypeMainVariant(CTreeType(t1)),
                                   CTreeTypeMainVariant(CTreeType(t2)));
        int constp
          = CIsTreeNodeReadOnly (CTreeType(t1)) || CIsTreeNodeReadOnly (CTreeType(t2));
        int volatilep
          = CIsTreeNodeVolatile (CTreeType(t1)) || CIsTreeNodeVolatile (CTreeType(t2));
        t1 = c_tree_build_pointer_type(c_tree_build_type_variant(target, constp,
                                 volatilep));
        return c_tree_build_type_attribute_variant(t1, attributes);
      }

    case C_ARRAY_TYPE:
      {
        c_tree_node *elt = c_get_common_type (CTreeType(t1), CTreeType(t2));
        /* Save space: see if the result is identical to one of the args.  */
        if (elt == CTreeType(t1) && CTreeTypeValues (t1))
          return c_tree_build_type_attribute_variant(t1, attributes);
        if (elt == CTreeType(t2) && CTreeTypeValues (t2))
          return c_tree_build_type_attribute_variant(t2, attributes);
        /* Merge the element types, and have a size if either arg has one.  */
        t1 = c_tree_build_array_type (elt, CTreeTypeValues(CTreeTypeValues (t1) ? t1 : t2));
        return c_tree_build_type_attribute_variant(t1, attributes);
      }

    case C_FUNCTION_TYPE:
      /* Function types: prefer the one that specified arg types.
         If both do, merge the arg types.  Also merge the return types.  */
      {
        c_tree_node *valtype = c_get_common_type (CTreeType(t1), CTreeType(t2));
        c_tree_node *p1 = CTreeTypeValues (t1);
        c_tree_node *p2 = CTreeTypeValues (t2);
        int len;
        c_tree_node *newargs;
        c_tree_node *n;
        int i;

        /* Save space: see if the result is identical to one of the args.  */
        if (valtype == CTreeType(t1) && ! CTreeTypeValues (t2))
          return c_tree_build_type_attribute_variant(t1, attributes);
        if (valtype == CTreeType(t2) && ! CTreeTypeValues (t1))
          return c_tree_build_type_attribute_variant(t2, attributes);

        /* Simple way if one arg fails to specify argument types.  */
        if (CTreeTypeValues (t1) == 0)
         {
           t1 = c_tree_build_function_type (valtype, CTreeTypeValues(t2));
           return c_tree_build_type_attribute_variant(t1, attributes);
         }
        if (CTreeTypeValues (t2) == 0)
         {
           t1 = c_tree_build_function_type (valtype, CTreeTypeValues(t1));
           return c_tree_build_type_attribute_variant(t1, attributes);
         }

        /* If both args specify argument types, we must merge the two
           lists, argument by argument.  */

        len = c_tree_get_list_length(p1);
        newargs = 0;

        for (i = 0; i < len; i++)
          newargs = c_tree_add_head_list( NULL,  NULL, newargs);

        n = newargs;

        for (; p1;
             p1 = CTreeChain(p1), p2 = CTreeChain(p2), n = CTreeChain(n))
          {
            /* A null type means arg type is not specified.
               Take whatever the other function type has.  */
            if ( CTreeListValue (p1) == 0)
              {
                CTreeListValue (n) = CTreeListValue (p2);
                goto parm_done;
              }
            if ( CTreeListValue (p2) == 0)
              {
                CTreeListValue (n) = CTreeListValue (p1);
                goto parm_done;
              }
              
            /* Given  wait (union {union wait *u; int *i} *)
               and  wait (union wait *),
               prefer  union wait *  as type of parm.  */
            if ( CTreeTypeCode(CTreeListValue (p1)) == C_UNION_TYPE
                && CTreeListValue (p1) != CTreeListValue (p2))
              {
                c_tree_node *memb;
                for (memb = CTreeTypeValues(CTreeListValue (p1));
                     memb; memb = CTreeChain(memb))
                  if (c_is_comptypes(CTreeType(memb), CTreeListValue (p2)))
                    {
                      CTreeListValue (n) = CTreeListValue (p2);
                      if (c_pedantic)
                      {
                        pedwarn ("function types not truly compatible in ANSI C");
                      }
                      goto parm_done;
                    }
              }
            if ( CTreeTypeCode( CTreeListValue (p2)) == C_UNION_TYPE
                &&  CTreeListValue(p2) !=  CTreeListValue(p1))
              {
                c_tree_node *memb;
                for (memb = CTreeTypeValues( CTreeListValue (p2));
                     memb; memb = CTreeChain(memb))
                  if (c_is_comptypes(CTreeType(memb),  CTreeListValue (p1)))
                    {
                       CTreeListValue (n) =  CTreeListValue (p1);
                      if (c_pedantic)
                      {
                        pedwarn ("function types not truly compatible in ANSI C");
                      }
                      goto parm_done;
                    }
              }
             CTreeListValue(n) = c_get_common_type( CTreeListValue(p1),  CTreeListValue(p2));
          parm_done: ;
          }

        t1 = c_tree_build_function_type(valtype, newargs);
        /* ... falls through ...  */
      }

    default:
      return c_tree_build_type_attribute_variant(t1, attributes);
    }
}


/* Print an error message for invalid operands to arith operation CODE.
   NOP_EXPR is used as a special case (see truthvalue_conversion).  */

void c_binary_op_error (code)
     int code;
{
  char *opname;

  switch (code)
    {
    case C_NOP_EXPR:
      error ("invalid truth-value expression");
      return;

    case C_PLUS_EXPR:
      opname = "+"; break;
    case C_MINUS_EXPR:
      opname = "-"; break;
    case C_MULT_EXPR:
      opname = "*"; break;
    case C_MAX_EXPR:
      opname = "max"; break;
    case C_MIN_EXPR:
      opname = "min"; break;
    case C_EQ_EXPR:
      opname = "=="; break;
    case C_NE_EXPR:
      opname = "!="; break;
    case C_LE_EXPR:
      opname = "<="; break;
    case C_GE_EXPR:
      opname = ">="; break;
    case C_LT_EXPR:
      opname = "<"; break;
    case C_GT_EXPR:
      opname = ">"; break;
    case C_LSHIFT_EXPR:
      opname = "<<"; break;
    case C_RSHIFT_EXPR:
      opname = ">>"; break;
    case C_TRUNC_MOD_EXPR:
    case C_FLOOR_MOD_EXPR:
      opname = "%"; break;
    case C_TRUNC_DIV_EXPR:
    case C_FLOOR_DIV_EXPR:
      opname = "/"; break;
    case C_BIT_AND_EXPR:
      opname = "&"; break;
    case C_BIT_IOR_EXPR:
      opname = "|"; break;
    case C_TRUTH_ANDIF_EXPR:
      opname = "&&"; break;
    case C_TRUTH_ORIF_EXPR:
      opname = "||"; break;
    case C_BIT_XOR_EXPR:
      opname = "^"; break;
    case C_LROTATE_EXPR:
    case C_RROTATE_EXPR:
      opname = "rotate"; break;
    default:
      opname = "unknown"; break;
    }
  error ("invalid operands to binary %s", opname);
}

/* Build a binary-operation expression without default conversions.
   CODE is the kind of expression to build.
   This function differs from `build' in several ways:
   the data type of the result is computed and recorded in it,
   warnings are generated if arg data types are invalid,
   special handling for addition and subtraction of pointers is known,
   and some optimization is done (operations on narrow ints
   are done in the narrower type when that gives the same result).
   Constant folding is also done before the result is returned.

   Note that the operands will never have enumeral types, or function
   or array types, because either they will have the default conversions
   performed or they have both just been converted to some other type in which
   the arithmetic is to be done.  */

c_tree_node *c_build_binary_op(code, orig_op0, orig_op1, convert_p)
     int code;
     c_tree_node *orig_op0;
     c_tree_node *orig_op1;
     int convert_p;
{
  c_tree_node *type0;
  c_tree_node *type1;
  int code0, code1;
  c_tree_node *op0;
  c_tree_node *op1;

  /* Expression code to give to the expression when it is built.
     Normally this is CODE, which is what the caller asked for,
     but in some special cases we change it.  */
  int resultcode = code;

  /* Data type in which the computation is to be performed.
     In the simplest cases this is the common type of the arguments.  */
  c_tree_node *result_type = NULL;

  /* Nonzero means operands have already been type-converted
     in whatever way is necessary.
     Zero means they need to be converted to RESULT_TYPE.  */
  int converted = 0;

  /* Nonzero means create the expression with this type, rather than
     RESULT_TYPE.  */
  c_tree_node *build_type = 0;

  /* Nonzero means after finally constructing the expression
     convert it to this type.  */
  c_tree_node *final_type = 0;

  /* Nonzero if this is an operation like MIN or MAX which can
     safely be computed in short if both args are promoted shorts.
     Also implies COMMON.
     -1 indicates a bitwise operation; this makes a difference
     in the exact conditions for when it is safe to do the operation
     in a narrower mode.  */
  int shorten = 0;

  /* Nonzero if this is a comparison operation;
     if both args are promoted shorts, compare the original shorts.
     Also implies COMMON.  */
  int short_compare = 0;

  /* Nonzero if this is a right-shift operation, which can be computed on the
     original short and then promoted if the operand is a promoted short.  */
  int short_shift = 0;

  /* Nonzero means set RESULT_TYPE to the common type of the args.  */
  int common = 0;

  if (convert_p)
    {
      op0 = c_default_conversion(orig_op0);
      op1 = c_default_conversion(orig_op1);
    }
  else
    {
      op0 = orig_op0;
      op1 = orig_op1;
    }

  type0 = CTreeType(op0);
  type1 = CTreeType(op1);

  /* The expression codes of the data types of the arguments tell us
     whether the arguments are integers, floating, pointers, etc.  */
  code0 = CTreeTypeCode(type0);
  code1 = CTreeTypeCode(type1);

  /* Strip NON_LVALUE_EXPRs, etc., since we aren't using as an lvalue.  */
  CStripTypeNops(op0);
  CStripTypeNops(op1);

  /* If an error was already reported for one of the arguments,
     avoid reporting another error.  */

  if ( CTreeNodeFirstCode(type0) == C_ERROR_NODE || 
       CTreeNodeFirstCode(type1) == C_ERROR_NODE)
  {
    return c_error_mark_node;
  }

  switch (code)
    {
    case C_PLUS_EXPR:
      /* Handle the pointer + int case.  */
      if (code0 == C_POINTER_TYPE && code1 == C_INTEGER_TYPE)
        return c_pointer_int_sum(C_PLUS_EXPR, op0, op1);
      else if (code1 == C_POINTER_TYPE && code0 == C_INTEGER_TYPE)
        return c_pointer_int_sum(C_PLUS_EXPR, op1, op0);
      else
        common = 1;
      break;

    case C_MINUS_EXPR:
      /* Subtraction of two similar pointers.
         We must subtract them as integers, then divide by object size.  */
      if (code0 == C_POINTER_TYPE && code1 == C_POINTER_TYPE
          && c_is_comp_target_types(type0, type1))
        return c_pointer_diff(op0, op1);
      /* Handle pointer minus int.  Just like pointer plus int.  */
      else if (code0 == C_POINTER_TYPE && code1 == C_INTEGER_TYPE)
        return c_pointer_int_sum(C_MINUS_EXPR, op0, op1);
      else
        common = 1;
      break;

    case C_MULT_EXPR:
      common = 1;
      break;

    case C_TRUNC_DIV_EXPR:
    case C_CEIL_DIV_EXPR:
    case C_FLOOR_DIV_EXPR:
    case C_ROUND_DIV_EXPR:
    case C_EXACT_DIV_EXPR:
      if ((code0 == C_INTEGER_TYPE || code0 == C_REAL_TYPE
           || code0 == C_COMPLEX_TYPE)
          && (code1 == C_INTEGER_TYPE || code1 == C_REAL_TYPE
              || code1 == C_COMPLEX_TYPE))
        {
          if (!(code0 == C_INTEGER_TYPE && code1 == C_INTEGER_TYPE))
            resultcode = C_RDIV_EXPR;
          else
            {
              /* Although it would be tempting to shorten always here, that
                 loses on some targets, since the modulo instruction is
                 undefined if the quotient can't be represented in the
                 computation mode.  We shorten only if unsigned or if
                 dividing by something we know != -1.  */
              shorten = ( CIsTreeNodeUnsigned(CTreeType(orig_op0))
                         || ( CTreeNodeFirstCode(op1) == C_INT_CST_NODE
                             && ( CTreeIntCstLow(op1) != -1
                                 || CTreeIntCstHigh(op1) != -1)));
            }
          common = 1;
        }
      break;

    case C_BIT_AND_EXPR:
    case C_BIT_ANDTC_EXPR:
    case C_BIT_IOR_EXPR:
    case C_BIT_XOR_EXPR:
      if (code0 == C_INTEGER_TYPE && code1 == C_INTEGER_TYPE)
        shorten = -1;
      /* If one operand is a constant, and the other is a short type
         that has been converted to an int,
         really do the work in the short type and then convert the
         result to int.  If we are lucky, the constant will be 0 or 1
         in the short type, making the entire operation go away.  */
      if ( CTreeNodeFirstCode(op0) == C_INT_CST_NODE
          && CTreeNodeFirstCode(op1) == C_EXPR_NODE
          && CTreeExprCode(op1) == C_NOP_EXPR
          && CTreeTypePrecision(type1) > 
             CTreeTypePrecision(CTreeType(CTreeExprOperand(op1, 0)))
          && CIsTreeNodeUnsigned(CTreeType(CTreeExprOperand(op1, 0))))
        {
          final_type = result_type;
          op1 = CTreeExprOperand(op1, 0);
          result_type = CTreeType(op1);
        }
      if ( CTreeNodeFirstCode(op1) == C_INT_CST_NODE
          && CTreeNodeFirstCode(op0) == C_EXPR_NODE
          && CTreeExprCode(op0) == C_NOP_EXPR
          && CTreeTypePrecision(type0) > 
             CTreeTypePrecision(CTreeType( CTreeExprOperand(op0, 0)))
          && CIsTreeNodeUnsigned(CTreeType( CTreeExprOperand(op0, 0))))
        {
          final_type = result_type;
          op0 = CTreeExprOperand(op0, 0);
          result_type = CTreeType(op0);
        }
      break;

    case C_TRUNC_MOD_EXPR:
    case C_FLOOR_MOD_EXPR:
      if (code0 == C_INTEGER_TYPE && code1 == C_INTEGER_TYPE)
        {
          /* Although it would be tempting to shorten always here, that loses
             on some targets, since the modulo instruction is undefined if the
             quotient can't be represented in the computation mode.  We shorten
             only if unsigned or if dividing by something we know != -1.  */
          shorten = ( CIsTreeNodeUnsigned(CTreeType(orig_op0))
                     || ( CTreeNodeFirstCode(op1) == C_INT_CST_NODE
                         && ( CTreeIntCstLow(op1) != -1
                             || CTreeIntCstHigh(op1) != -1)));
          common = 1;
        }
      break;

    case C_TRUTH_ANDIF_EXPR:
    case C_TRUTH_ORIF_EXPR:
    case C_TRUTH_AND_EXPR:
    case C_TRUTH_OR_EXPR:
    case C_TRUTH_XOR_EXPR:
      if ((code0 == C_INTEGER_TYPE || code0 == C_POINTER_TYPE
           || code0 == C_REAL_TYPE || code0 == C_COMPLEX_TYPE)
          && (code1 == C_INTEGER_TYPE || code1 == C_POINTER_TYPE
              || code1 == C_REAL_TYPE || code1 == C_COMPLEX_TYPE))
        {
          /* Result of these operations is always an int,
             but that does not mean the operands should be
             converted to ints!  */
          result_type = c_integer_type_node;
          op0 = c_truthvalue_conversion(op0);
          op1 = c_truthvalue_conversion(op1);
          converted = 1;
        }
      break;

      /* Shift operations: result has same type as first operand;
         always convert second operand to int.
         Also set SHORT_SHIFT if shifting rightward.  */

    case C_RSHIFT_EXPR:
      if (code0 == C_INTEGER_TYPE && code1 == C_INTEGER_TYPE)
        {
          if ( CTreeNodeFirstCode(op1) == C_INT_CST_NODE && c_skip_evaluation == 0)
            {
              if (c_tree_is_int_cst_sgn(op1) < 0)
              {
                warning ("right shift count is negative");
              }
              else
                {
                  if ( CTreeIntCstLow(op1) | CTreeIntCstHigh(op1))
                  {
                    short_shift = 1;
                  }
                  if ( CTreeIntCstHigh(op1) != 0
                      || ((unsigned int) CTreeIntCstLow(op1)
                          >= CTreeTypePrecision(type0)))
                  {
                    warning ("right shift count >= width of type");
                  }
                }
            }
          /* Use the type of the value to be shifted.
             This is what most traditional C compilers do.  */
          result_type = type0;
          /* Unless traditional, convert the shift-count to an integer,
             regardless of size of value being shifted.  */
          if (! c_flag_traditional)
            {
              if (CTreeTypeMainVariant(CTreeType(op1)) != c_integer_type_node)
              {
                op1 = c_convert_expr( c_integer_type_node, op1);
              }
              /* Avoid converting op1 to result_type later.  */
              converted = 1;
            }
        }
      break;

    case C_LSHIFT_EXPR:
      if (code0 == C_INTEGER_TYPE && code1 == C_INTEGER_TYPE)
        {
          if ( CTreeNodeFirstCode(op1) == C_INT_CST_NODE && c_skip_evaluation == 0)
            {
              if ( c_tree_is_int_cst_sgn(op1) < 0)
              {
                warning ("left shift count is negative");
              }
              else if ( CTreeIntCstHigh(op1) != 0
                       || ((unsigned int) CTreeIntCstLow(op1)
                           >= CTreeTypePrecision(type0)))
              {
                warning ("left shift count >= width of type");
              }
            }
          /* Use the type of the value to be shifted.
             This is what most traditional C compilers do.  */
          result_type = type0;
          /* Unless traditional, convert the shift-count to an integer,
             regardless of size of value being shifted.  */
          if (! c_flag_traditional)
            {
              if (CTreeTypeMainVariant(CTreeType(op1)) != c_integer_type_node)
                op1 = c_convert_expr(c_integer_type_node, op1);
              /* Avoid converting op1 to result_type later.  */
              converted = 1;
            }
        }
      break;

    case C_RROTATE_EXPR:
    case C_LROTATE_EXPR:
      if (code0 == C_INTEGER_TYPE && code1 == C_INTEGER_TYPE)
        {
          if ( CTreeNodeFirstCode(op1) == C_INT_CST_NODE && c_skip_evaluation == 0)
            {
              if (c_tree_is_int_cst_sgn (op1) < 0)
              {
                warning ("shift count is negative");
              }
              else if ( CTreeIntCstHigh(op1) != 0
                       || ((unsigned int) CTreeIntCstLow(op1)
                           >= CTreeTypePrecision(type0)))
              {
                warning ("shift count >= width of type");
              }
            }
          /* Use the type of the value to be shifted.
             This is what most traditional C compilers do.  */
          result_type = type0;
          /* Unless traditional, convert the shift-count to an integer,
             regardless of size of value being shifted.  */
          if (! c_flag_traditional)
            {
              if (CTreeTypeMainVariant(CTreeType(op1)) != c_integer_type_node)
                op1 = c_convert_expr(c_integer_type_node, op1);
              /* Avoid converting op1 to result_type later.  */
              converted = 1;
            }
        }
      break;

    case C_EQ_EXPR:
    case C_NE_EXPR:
      /* Result of comparison is always int,
         but don't convert the args to int!  */
      build_type = c_integer_type_node;
      if ((code0 == C_INTEGER_TYPE || code0 == C_REAL_TYPE
           || code0 == C_COMPLEX_TYPE)
          && (code1 == C_INTEGER_TYPE || code1 == C_REAL_TYPE
              || code1 == C_COMPLEX_TYPE))
        short_compare = 1;
      else if (code0 == C_POINTER_TYPE && code1 == C_POINTER_TYPE)
        {
          c_tree_node *tt0 = CTreeType(type0);
          c_tree_node *tt1 = CTreeType(type1);
          /* Anything compares with void *.  void * compares with anything.
             Otherwise, the targets must be compatible
             and both must be object or both incomplete.  */
          if ( c_is_comp_target_types(type0, type1))
          {
            result_type = c_get_common_type (type0, type1);
          }
          else if (CTreeTypeMainVariant(tt0) == c_void_type_node)
            {
              /* op0 != orig_op0 detects the case of something
                 whose value is 0 but which isn't a valid null ptr const.  */
              if (c_pedantic && (! c_tree_is_integer_zerop(op0) || op0 != orig_op0)
                  && CTreeTypeCode(tt1) == C_FUNCTION_TYPE)
              {
                pedwarn ("ANSI C forbids comparison of `void *' with function pointer");
              }
            }
          else if (CTreeTypeMainVariant(tt1) == c_void_type_node)
            {
              if (c_pedantic && (!c_tree_is_integer_zerop(op1) || op1 != orig_op1)
                  && CTreeTypeCode(tt0) == C_FUNCTION_TYPE)
              {
                pedwarn ("ANSI C forbids comparison of `void *' with function pointer");
              }
            }
          else
          {
            pedwarn ("comparison of distinct pointer types lacks a cast");
          }

          if (result_type == NULL)
            result_type = c_ptr_type_node;
        }
      else if (code0 == C_POINTER_TYPE && CTreeNodeFirstCode(op1) == C_INT_CST_NODE
               && c_tree_is_integer_zerop(op1))
        result_type = type0;
      else if (code1 == C_POINTER_TYPE && CTreeNodeFirstCode(op0) == C_INT_CST_NODE
               && c_tree_is_integer_zerop(op0))
        result_type = type1;
      else if (code0 == C_POINTER_TYPE && code1 == C_INTEGER_TYPE)
        {
          result_type = type0;
          if (! c_flag_traditional)
          {
            pedwarn ("comparison between pointer and integer");
          }
        }
      else if (code0 == C_INTEGER_TYPE && code1 == C_POINTER_TYPE)
        {
          result_type = type1;
          if (! c_flag_traditional)
          {
            pedwarn ("comparison between pointer and integer");
          }
        }
      break;

    case C_MAX_EXPR:
    case C_MIN_EXPR:
      if ((code0 == C_INTEGER_TYPE || code0 == C_REAL_TYPE)
          && (code1 == C_INTEGER_TYPE || code1 == C_REAL_TYPE))
        shorten = 1;
      else if (code0 == C_POINTER_TYPE && code1 == C_POINTER_TYPE)
        {
          if ( c_is_comp_target_types(type0, type1))
            {
              result_type = c_get_common_type(type0, type1);
              if (c_pedantic 
                  && CTreeTypeCode(CTreeType(type0)) == C_FUNCTION_TYPE)
              {
                pedwarn ("ANSI C forbids ordered comparisons of pointers to functions");
              }
            }
          else
            {
              result_type = c_ptr_type_node;
              pedwarn ("comparison of distinct pointer types lacks a cast");
            }
        }
      break;

    case C_LE_EXPR:
    case C_GE_EXPR:
    case C_LT_EXPR:
    case C_GT_EXPR:
      build_type = c_integer_type_node;
      if ((code0 == C_INTEGER_TYPE || code0 == C_REAL_TYPE)
          && (code1 == C_INTEGER_TYPE || code1 == C_REAL_TYPE))
        short_compare = 1;
      else if (code0 == C_POINTER_TYPE && code1 == C_POINTER_TYPE)
        {
          if ( c_is_comp_target_types(type0, type1))
            {
              result_type = c_get_common_type (type0, type1);
              if ((CTreeTypeSize(CTreeType(type0)) != 0)
                  != (CTreeTypeSize(CTreeType(type1)) != 0))
              {
                pedwarn ("comparison of complete and incomplete pointers");
              }
              else if (c_pedantic 
                       && CTreeTypeCode(CTreeType(type0)) == C_FUNCTION_TYPE)
              {
                pedwarn ("ANSI C forbids ordered comparisons of pointers to functions");
              }
            }
          else
            {
              result_type = c_ptr_type_node;
              pedwarn ("comparison of distinct pointer types lacks a cast");
            }
        }
      else if (code0 == C_POINTER_TYPE && CTreeNodeFirstCode(op1) == C_INT_CST_NODE
               && c_tree_is_integer_zerop (op1))
        {
          result_type = type0;
          if (c_pedantic || c_extra_warnings)
          {
            pedwarn ("ordered comparison of pointer with integer zero");
          }
        }
      else if (code1 == C_POINTER_TYPE && CTreeNodeFirstCode(op0) == C_INT_CST_NODE
               && c_tree_is_integer_zerop (op0))
        {
          result_type = type1;
          if (c_pedantic)
          {
            pedwarn ("ordered comparison of pointer with integer zero");
          }
        }
      else if (code0 == C_POINTER_TYPE && code1 == C_INTEGER_TYPE)
        {
          result_type = type0;
          if (! c_flag_traditional)
          {
            pedwarn ("comparison between pointer and integer");
          }
        }
      else if (code0 == C_INTEGER_TYPE && code1 == C_POINTER_TYPE)
        {
          result_type = type1;
          if (! c_flag_traditional)
          {
            pedwarn ("comparison between pointer and integer");
          }
        }
      break;
      
    default:
      break;
    }

  if ((code0 == C_INTEGER_TYPE || code0 == C_REAL_TYPE || code0 == C_COMPLEX_TYPE)
      &&
      (code1 == C_INTEGER_TYPE || code1 == C_REAL_TYPE || code1 == C_COMPLEX_TYPE))
    {
      int none_complex = (code0 != C_COMPLEX_TYPE && code1 != C_COMPLEX_TYPE);

      if (shorten || common || short_compare)
        result_type = c_get_common_type (type0, type1);

      /* For certain operations (which identify themselves by shorten != 0)
         if both args were extended from the same smaller type,
         do the arithmetic in that type and then extend.

         shorten !=0 and !=1 indicates a bitwise operation.
         For them, this optimization is safe only if
         both args are zero-extended or both are sign-extended.
         Otherwise, we might change the result.
         Eg, (short)-1 | (unsigned short)-1 is (int)-1
         but calculated in (unsigned short) it would be (unsigned short)-1.  */

      if (shorten && none_complex)
        {
          int unsigned0, unsigned1;
          c_tree_node *arg0 = c_get_narrower(op0, &unsigned0);
          c_tree_node *arg1 = c_get_narrower(op1, &unsigned1);
          /* UNS is 1 if the operation to be done is an unsigned one.  */
          int uns = !! CIsTreeNodeUnsigned(result_type);
          c_tree_node *type;

          final_type = result_type;

          /* Handle the case that OP0 (or OP1) does not *contain* a conversion
             but it *requires* conversion to FINAL_TYPE.  */

          if (( CTreeTypePrecision(CTreeType(op0))
               ==  CTreeTypePrecision(CTreeType(arg0)))
              && CTreeType(op0) != final_type)
            unsigned0 = !!CIsTreeNodeUnsigned(CTreeType(op0));
          if (( CTreeTypePrecision(CTreeType(op1))
               ==  CTreeTypePrecision(CTreeType(arg1)))
              && CTreeType(op1) != final_type)
            unsigned1 = !!CIsTreeNodeUnsigned(CTreeType(op1));

          /* Now UNSIGNED0 is 1 if ARG0 zero-extends to FINAL_TYPE.  */

          /* For bitwise operations, signedness of nominal type
             does not matter.  Consider only how operands were extended.  */
          if (shorten == -1)
            uns = unsigned0;

          /* Note that in all three cases below we refrain from optimizing
             an unsigned operation on sign-extended args.
             That would not be valid.  */

          /* Both args variable: if both extended in same way
             from same width, do it in that width.
             Do it unsigned if args were zero-extended.  */
          if ((CTreeTypePrecision(CTreeType(arg0))
               < CTreeTypePrecision(result_type))
              && (CTreeTypePrecision(CTreeType(arg1))
                  == CTreeTypePrecision(CTreeType(arg0)))
              && unsigned0 == unsigned1
              && (unsigned0 || !uns))
            result_type
              = c_get_signed_or_unsigned_type(unsigned0,
                                         c_get_common_type (CTreeType(arg0), CTreeType(arg1)));
          else if ( CTreeNodeFirstCode(arg0) == C_INT_CST_NODE
                   && (unsigned1 || !uns)
                   && ( CTreeTypePrecision(CTreeType(arg1))
                       <  CTreeTypePrecision(result_type))
                   && (type = c_get_signed_or_unsigned_type(unsigned1, CTreeType(arg1)),
                       c_int_fits_type_p (arg0, type)))
            result_type = type;
          else if ( CTreeNodeFirstCode(arg1) == C_INT_CST_NODE
                   && (unsigned0 || !uns)
                   && ( CTreeTypePrecision(CTreeType(arg0))
                       <  CTreeTypePrecision(result_type))
                   && (type = c_get_signed_or_unsigned_type(unsigned0, CTreeType(arg0)),
                       c_int_fits_type_p (arg1, type)))
            result_type = type;
        }

      /* Shifts can be shortened if shifting right.  */

      if (short_shift)
        {
          int unsigned_arg;
          c_tree_node *arg0 = c_get_narrower(op0, &unsigned_arg);

          final_type = result_type;

          if (arg0 == op0 && final_type == CTreeType(op0))
            unsigned_arg = !!CIsTreeNodeUnsigned(CTreeType(op0));

          if (CTreeTypePrecision(CTreeType(arg0)) < CTreeTypePrecision(result_type)
              /* We can shorten only if the shift count is less than the
                 number of bits in the smaller type size.  */
              && CTreeIntCstHigh(op1) == 0
              && CTreeTypePrecision(CTreeType(arg0)) > CTreeIntCstLow(op1)
              /* If arg is sign-extended and then unsigned-shifted,
                 we can simulate this with a signed shift in arg's type
                 only if the extended result is at least twice as wide
                 as the arg.  Otherwise, the shift could use up all the
                 ones made by sign-extension and bring in zeros.
                 We can't optimize that case at all, but in most machines
                 it never happens because available widths are 2**N.  */
              && (! CIsTreeNodeUnsigned(final_type)
                  || unsigned_arg
                  || 2 * CTreeTypePrecision(CTreeType(arg0)) <= CTreeTypePrecision(result_type)))
            {
              /* Do an unsigned shift if the operand was zero-extended.  */
              result_type
                = c_get_signed_or_unsigned_type(unsigned_arg, CTreeType(arg0));
              /* Convert value-to-be-shifted to that type.  */
              if (CTreeType(op0) != result_type)
                op0 = c_convert_expr(result_type, op0);
              converted = 1;
            }
        }

      /* Comparison operations are shortened too but differently.
         They identify themselves by setting short_compare = 1.  */

      if (short_compare)
        {
          /* Don't write &op0, etc., because that would prevent op0
             from being kept in a register.
             Instead, make copies of the our local variables and
             pass the copies by reference, then copy them back afterward.  */
          c_tree_node *xop0 = op0;
          c_tree_node *xop1 = op1;
          c_tree_node *xresult_type = result_type;
          int xresultcode = resultcode;
          c_tree_node *val 
            = c_shorten_compare(&xop0, &xop1, &xresult_type, &xresultcode);
          if (val != 0)
            return val;
          op0 = xop0, op1 = xop1;
          converted = 1;
          resultcode = xresultcode;

          if ((c_warn_sign_compare < 0 ? c_extra_warnings : c_warn_sign_compare != 0)
              && c_skip_evaluation == 0)
            {
              int op0_signed = ! CIsTreeNodeUnsigned(CTreeType(orig_op0));
              int op1_signed = ! CIsTreeNodeUnsigned(CTreeType(orig_op1));

              int unsignedp0, unsignedp1;
              c_tree_node *primop0 = c_get_narrower(op0, &unsignedp0);
              c_tree_node *primop1 = c_get_narrower(op1, &unsignedp1);

              /* Avoid spurious warnings for comparison with enumerators.  */
 
              xop0 = orig_op0;
              xop1 = orig_op1;
              CStripTypeNops(xop0);
              CStripTypeNops(xop1);

              /* Give warnings for comparisons between signed and unsigned
                 quantities that may fail.  */
              /* Do the checking based on the original operand trees, so that
                 casts will be considered, but default promotions won't be.  */

              /* Do not warn if the comparison is being done in a signed type,
                 since the signed type will only be chosen if it can represent
                 all the values of the unsigned type.  */
              if (! CIsTreeNodeUnsigned(result_type))
                /* OK */;
              /* Do not warn if both operands are unsigned.  */
              else if (op0_signed == op1_signed)
                /* OK */;
              /* Do not warn if the signed quantity is an unsuffixed
                 integer literal (or some static constant expression
                 involving such literals) and it is non-negative.  */
              else if ((op0_signed && CTreeNodeFirstCode(xop0) == C_INT_CST_NODE
                        && c_tree_is_int_cst_sgn(xop0) >= 0)
                       || (op1_signed && CTreeNodeFirstCode(xop1) == C_INT_CST_NODE
                           && c_tree_is_int_cst_sgn(xop1) >= 0))
                /* OK */;
              /* Do not warn if the comparison is an equality operation,
                 the unsigned quantity is an integral constant and it does
                 not use the most significant bit of result_type.  */
              else if ((resultcode == C_EQ_EXPR || resultcode == C_NE_EXPR)
                       && ((op0_signed && CTreeNodeFirstCode(xop1) == C_INT_CST_NODE
                            && c_int_fits_type_p(xop1, c_get_signed_type(result_type)))
                           || (op1_signed && CTreeNodeFirstCode(xop0) == C_INT_CST_NODE
                               && c_int_fits_type_p(xop0, c_get_signed_type(result_type)))))
                /* OK */;
              else
              {
                warning ("comparison between signed and unsigned");
              }

              /* Warn if two unsigned values are being compared in a size
                 larger than their original size, and one (and only one) is the
                 result of a `~' operator.  This comparison will always fail.

                 Also warn if one operand is a constant, and the constant
                 does not have all bits set that are set in the ~ operand
                 when it is extended.  */

              if (( CTreeNodeFirstCode(primop0) == C_EXPR_NODE && 
                    CTreeExprCode(primop0) == C_BIT_NOT_EXPR)
                  != 
                  ( CTreeNodeFirstCode(primop0) == C_EXPR_NODE &&
                    CTreeExprCode(primop1) == C_BIT_NOT_EXPR ))
                {
                  if ( CTreeNodeFirstCode(primop0) == C_EXPR_NODE &&
                       CTreeExprCode(primop0) == C_BIT_NOT_EXPR)
                    primop0 = c_get_narrower(CTreeExprOperand (primop0, 0),
                                            &unsignedp0);
                  else
                    primop1 = c_get_narrower(CTreeExprOperand (primop1, 0),
                                            &unsignedp1);
              
                  if ( CTreeNodeFirstCode(primop0) == C_INT_CST_NODE
                      || CTreeNodeFirstCode(primop1) == C_INT_CST_NODE)
                    {
                      c_tree_node *primop;
                      long constant, mask;
                      int unsignedp, bits;

                      if ( CTreeNodeFirstCode(primop0) == C_INT_CST_NODE)
                        {
                          primop = primop1;
                          unsignedp = unsignedp1;
                          constant = CTreeIntCstLow(primop0);
                        }
                      else
                        {
                          primop = primop0;
                          unsignedp = unsignedp0;
                          constant = CTreeIntCstLow(primop1);
                        }

                      bits = CTreeTypePrecision(CTreeType(primop));
                      if (bits < CTreeTypePrecision(result_type)
                          && bits < C_HOST_BITS_PER_LONG && unsignedp)
                        {
                          mask = (~0L) << bits;
                          if ((mask & constant) != mask)
                          {
                            warning ("comparison of promoted ~unsigned with constant");
                          }
                        }
                    }
                  else if (unsignedp0 && unsignedp1
                           && (CTreeTypePrecision(CTreeType(primop0))
                               < CTreeTypePrecision(result_type))
                           && (CTreeTypePrecision(CTreeType(primop1))
                               < CTreeTypePrecision(result_type)))
                  {
                    warning ("comparison of promoted ~unsigned with unsigned");
                  }
                }
            }
        }
    }

  /* At this point, RESULT_TYPE must be nonzero to avoid an error message.
     If CONVERTED is zero, both args will be converted to type RESULT_TYPE.
     Then the expression will be built.
     It will be given type FINAL_TYPE if that is nonzero;
     otherwise, it will be given type RESULT_TYPE.  */

  if (!result_type)
    {
      c_binary_op_error(code);
      return c_error_mark_node;
    }

  if (! converted)
    {
      if (CTreeType(op0) != result_type)
        op0 = c_convert_expr(result_type, op0); 
      if (CTreeType(op1) != result_type)
        op1 = c_convert_expr(result_type, op1); 
    }

  if (build_type == NULL)
    build_type = result_type;

  {
    c_tree_node *result;
    c_tree_node *folded;

    result = c_tree_build_binary_expr(resultcode, op0, op1);
    CTreeType( result ) = result_type;


    folded = c_fold (result);
    if (folded == result)
    {
      if ( CIsTreeNodeConstant( op0 ) &&
           CIsTreeNodeConstant( op1 ) )
      {
        CSetTreeNodeConstant( folded );
      }
    }
    if (final_type != 0)
    {
      return c_convert_expr(final_type, folded);
    }
    return folded;
  }
}


/* This is the entry point used by the parser
   for binary operators in the input.
   In addition to constructing the expression,
   we check for operands that were written with other binary operators
   in a way that is likely to confuse the user.  */

c_tree_node *c_parser_build_binary_op(code, arg1, arg2)
     int code;
     c_tree_node *arg1;
     c_tree_node *arg2;
{
  c_tree_node *result = c_build_binary_op(code, arg1, arg2, 1);

# ifdef LUDO_NO_SKIP
  char class;
  char class1 = TREE_CODE_CLASS (TREE_CODE (arg1));
  char class2 = TREE_CODE_CLASS (TREE_CODE (arg2));
  int code1 = C_ERROR_NODE;
  int code2 = C_ERROR_NODE;

  if (class1 == 'e' || class1 == '1'
      || class1 == '2' || class1 == '<')
    code1 = CTreeExprComplexity(arg1);
  if (class2 == 'e' || class2 == '1'
      || class2 == '2' || class2 == '<')
    code2 = CTreeExprComplexity(arg2);

  /* Check for cases such as x+y<<z which users are likely
     to misinterpret.  If parens are used, C_EXP_ORIGINAL_CODE
     is cleared to prevent these warnings.  */
  if (warn_parentheses)
    {
      if (code == LSHIFT_EXPR || code == RSHIFT_EXPR)
        {
          if (code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around + or - inside shift");
        }

      if (code == TRUTH_ORIF_EXPR)
        {
          if (code1 == TRUTH_ANDIF_EXPR
              || code2 == TRUTH_ANDIF_EXPR)
            warning ("suggest parentheses around && within ||");
        }

      if (code == BIT_IOR_EXPR)
        {
          if (code1 == BIT_AND_EXPR || code1 == BIT_XOR_EXPR
              || code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == BIT_AND_EXPR || code2 == BIT_XOR_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around arithmetic in operand of |");
          /* Check cases like x|y==z */
          if (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<')
            warning ("suggest parentheses around comparison in operand of |");
        }

      if (code == BIT_XOR_EXPR)
        {
          if (code1 == BIT_AND_EXPR
              || code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == BIT_AND_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around arithmetic in operand of ^");
          /* Check cases like x^y==z */
          if (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<')
            warning ("suggest parentheses around comparison in operand of ^");
        }

      if (code == BIT_AND_EXPR)
        {
          if (code1 == PLUS_EXPR || code1 == MINUS_EXPR
              || code2 == PLUS_EXPR || code2 == MINUS_EXPR)
            warning ("suggest parentheses around + or - in operand of &");
          /* Check cases like x&y==z */
          if (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<')
            warning ("suggest parentheses around comparison in operand of &");
        }
    }

  /* Similarly, check for cases like 1<=i<=10 that are probably errors.  */
  if (TREE_CODE_CLASS (code) == '<' && extra_warnings
      && (TREE_CODE_CLASS (code1) == '<' || TREE_CODE_CLASS (code2) == '<'))
    warning ("comparisons like X<=Y<=Z do not have their mathematical meaning");

  unsigned_conversion_warning (result, arg1);
  unsigned_conversion_warning (result, arg2);
  overflow_warning (result);

  class = TREE_CODE_CLASS (TREE_CODE (result));

  /* Record the code that was specified in the source,
     for the sake of warnings about confusing nesting.  */
  if (class == 'e' || class == '1'
      || class == '2' || class == '<')
    C_SET_EXP_ORIGINAL_CODE (result, code);
  else
    {
      int flag = TREE_CONSTANT (result);
      /* We used to use NOP_EXPR rather than NON_LVALUE_EXPR
         so that convert_for_assignment wouldn't strip it.
         That way, we got warnings for things like p = (1 - 1).
         But it turns out we should not get those warnings.  */
      result = build1 (NON_LVALUE_EXPR, CTreeType(result), result);
      C_SET_EXP_ORIGINAL_CODE (result, code);
      TREE_CONSTANT (result) = flag;
    }
# endif

  return result;
}


/* Return a variant of TYPE which has all the type qualifiers of LIKE
   as well as those of TYPE.  */

static c_tree_node *c_qualify_type(type, like)
     c_tree_node *type;
     c_tree_node *like;
{
  int constflag = CIsTreeNodeReadOnly(type) || CIsTreeNodeReadOnly(like);
  int volflag = CIsTreeNodeVolatile(type) || CIsTreeNodeVolatile(like);

  return c_tree_build_type_variant (type, constflag, volflag);
}

/*------------------------------------------------------------\
|                                                             |
|               C Parser Build Conditionnal Expr            |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_parser_build_conditional_expr(ifexp, op1, op2)
     c_tree_node *ifexp;
     c_tree_node *op1;
     c_tree_node *op2;
{
  c_tree_node *result;
  c_tree_node *type1;
  c_tree_node *type2;
  int          code1;
  int          code2;
  c_tree_node *result_type = NULL;
  c_tree_node *orig_op1 = op1;
  c_tree_node *orig_op2 = op2;

  ifexp = c_truthvalue_conversion(c_default_conversion(ifexp));

  /* Promote both alternatives.  */

  if (CTreeNodeCode( CTreeType(op1)) != C_VOID_TYPE)
    op1 = c_default_conversion(op1);
  if (CTreeNodeCode( CTreeType(op2)) != C_VOID_TYPE)
    op2 = c_default_conversion(op2);

  if (CTreeNodeCode(ifexp) == C_ERROR_NODE
      || CTreeNodeCode(CTreeType(op1)) == C_ERROR_NODE
      || CTreeNodeCode(CTreeType(op2)) == C_ERROR_NODE)
    return c_error_mark_node;

  type1 = CTreeType(op1);
  code1 = CTreeNodeCode(type1);
  type2 = CTreeType(op2);
  code2 = CTreeNodeCode(type2);
      
  /* Quickly detect the usual case where op1 and op2 have the same type
     after promotion.  */
  if (CTreeTypeMainVariant(type1) == CTreeTypeMainVariant(type2))
    {
      if (type1 == type2)
        result_type = type1;
      else
        result_type = CTreeTypeMainVariant(type1);
    }
  else if ((code1 == C_INTEGER_TYPE || code1 == C_REAL_TYPE)
           && (code2 == C_INTEGER_TYPE || code2 == C_REAL_TYPE))
    {
      result_type = c_get_common_type(type1, type2);
    }
  else if (code1 == C_VOID_TYPE || code2 == C_VOID_TYPE)
    {
      if (c_pedantic && (code1 != C_VOID_TYPE || code2 != C_VOID_TYPE))
      {
        pedwarn ("ANSI C forbids conditional expr with only one void side");
      }
      result_type = c_void_type_node;
    }
  else if (code1 == C_POINTER_TYPE && code2 == C_POINTER_TYPE)
    {
      if (c_is_comp_target_types (type1, type2))
        result_type = c_get_common_type (type1, type2);
      else if (c_tree_is_integer_zerop (op1) && CTreeType(type1) == c_void_type_node
               && CTreeNodeCode(orig_op1) != C_NOP_EXPR)
        result_type = c_qualify_type (type2, type1);
      else if (c_tree_is_integer_zerop (op2) && CTreeType(type2) == c_void_type_node
               && CTreeNodeCode(orig_op2) != C_NOP_EXPR)
        result_type = c_qualify_type (type1, type2);
      else if (CTreeTypeMainVariant(CTreeType(type1)) == c_void_type_node)
        {
          if (c_pedantic && CTreeNodeCode(CTreeType(type2)) == C_FUNCTION_TYPE)
          {
            pedwarn ("ANSI C forbids conditional expr between `void *' and function pointer");
          }
          result_type = c_qualify_type(type1, type2);
        }
      else if (CTreeTypeMainVariant(CTreeType(type2)) == c_void_type_node)
        {
          if (c_pedantic && CTreeNodeCode(CTreeType(type1)) == C_FUNCTION_TYPE)
          {
            pedwarn ("ANSI C forbids conditional expr between `void *' and function pointer");
          }
          result_type = c_qualify_type (type2, type1);
        }
      else
        {
          pedwarn ("pointer type mismatch in conditional expression");
          result_type = c_tree_build_pointer_type(c_void_type_node);
        }
    }
  else if (code1 == C_POINTER_TYPE && code2 == C_INTEGER_TYPE)
    {
      if (! c_tree_is_integer_zerop (op2))
      {
        pedwarn ("pointer/integer type mismatch in conditional expression");
      }
      else
        {
          op2 = c_null_pointer_node;
        }
      result_type = type1;
    }
  else if (code2 == C_POINTER_TYPE && code1 == C_INTEGER_TYPE)
    {
      if (!c_tree_is_integer_zerop (op1))
      {
        pedwarn ("pointer/integer type mismatch in conditional expression");
      }
      else
        {
          op1 = c_null_pointer_node;
        }
      result_type = type2;
    }

  if (!result_type)
    {
      if (c_flag_cond_mismatch)
      {
        result_type = c_void_type_node;
      }
      else
        {
          error ("type mismatch in conditional expression");
          return c_error_mark_node;
        }
    }

  /* Merge const and volatile flags of the incoming types.  */
  result_type
    = c_tree_build_type_variant(result_type,
                          CIsTreeNodeReadOnly(op1) || CIsTreeNodeReadOnly(op2),
                          CIsTreeNodeVolatile(op1) || CIsTreeNodeVolatile(op2));

  if (result_type != CTreeType(op1))
    op1 = c_convert_and_check (result_type, op1);
  if (result_type != CTreeType(op2))
    op2 = c_convert_and_check (result_type, op2);
    
  if (CTreeNodeCode(ifexp) == C_INT_CST_NODE)
    return c_get_pedantic_non_lvalue( c_tree_is_integer_zerop(ifexp) ? op2 : op1);

  result = c_tree_build_ternary_expr(C_COND_EXPR, ifexp, op1, op2);
  CTreeType( result ) = result_type;

  return c_fold( result );
}

/*------------------------------------------------------------\
|                                                             |
|               C Parser Build Modify Expr                    |
|                                                             |
\------------------------------------------------------------*/


/* Build an assignment expression of lvalue LHS from value RHS.
   MODIFYCODE is the code for a binary operator that we use
   to combine the old value of LHS with RHS to get the new value.
   Or else MODIFYCODE is NOP_EXPR meaning do a simple assignment.  */

c_tree_node *c_parser_build_modify_expr(lhs, modifycode, rhs)
     c_tree_node *lhs;
     c_tree_node *rhs;
     int  modifycode;
{
  c_tree_node *result;
  c_tree_node *newrhs;
  c_tree_node *lhstype = CTreeType(lhs);
  c_tree_node *olhstype = lhstype;
  c_tree_node *tmp_node;

  /* Types that aren't fully specified cannot be used in assignments.  */
  lhs = c_require_complete_type(lhs);

  /* Avoid duplicate error messages from operands that had errors.  */
  if ( CTreeNodeFirstCode(lhs) == C_ERROR_NODE ||  
       CTreeNodeFirstCode(rhs) == C_ERROR_NODE)
  {
    return c_error_mark_node;
  }

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  /* Do not use STRIP_NOPS here.  We do not want an enumerator
     whose value is 0 to count as a null pointer constant.  */
  if ( CTreeExprCode(rhs) == C_NON_LVALUE_EXPR)
  {
    rhs = CTreeExprOperand(rhs, 0);
  }

  newrhs = rhs;

  /* Handle control structure constructs used as "lvalues".  */

  if ( CTreeNodeFirstCode(lhs) == C_EXPR_NODE )
  {
  switch ( CTreeExprCode(lhs))
    {
      /* Handle (a, b) used as an "lvalue".  */
    case C_COMPOUND_EXPR:
      c_pedantic_lvalue_warning (C_COMPOUND_EXPR);
      newrhs = c_parser_build_modify_expr( CTreeExprOperand(lhs, 1), modifycode, rhs);
      if ( CTreeNodeFirstCode(newrhs) == C_ERROR_NODE)
      {
        return c_error_mark_node;
      }

      tmp_node = c_tree_build_binary_expr( C_COMPOUND_EXPR, CTreeExprOperand(lhs,0), newrhs);
      CTreeType( tmp_node ) = lhstype;

      return( tmp_node );
 
      /* Handle (a ? b : c) used as an "lvalue".  */
    case C_COND_EXPR:
      c_pedantic_lvalue_warning (C_COND_EXPR);
# ifdef LUDO_DONE
      rhs = save_expr (rhs);
# endif
      {
        /* Produce (a ? (b = rhs) : (c = rhs))
           except that the RHS goes through a save-expr
           so the code to compute it is only emitted once.  */
        c_tree_node *cond
          = c_parser_build_conditional_expr(
              CTreeExprOperand(lhs, 0),
              c_parser_build_modify_expr( CTreeExprOperand(lhs, 1), modifycode, rhs),
              c_parser_build_modify_expr( CTreeExprOperand(lhs, 2), modifycode, rhs));

        if ( CTreeNodeFirstCode(cond) == C_ERROR_NODE)
        {
          return cond;
        }
        /* Make sure the code to compute the rhs comes out
           before the split.  */

        /* But cast it to void to avoid an "unused" error.  */
        tmp_node = c_tree_build_binary_expr( C_COMPOUND_EXPR, 
               c_convert_expr(c_void_type_node, rhs), cond );
        CTreeType( tmp_node ) = CTreeType( lhs );

      return( tmp_node );
      }
    default:
      break;
    }
  }

  /* If a binary op has been requested, combine the old LHS value with the RHS
     producing the value we should actually store into the LHS.  */

  if (modifycode != C_NOP_EXPR)
    {
      lhs = c_stabilize_reference (lhs);
      newrhs = c_build_binary_op(modifycode, lhs, rhs, 1 );
    }

  /* Handle a cast used as an "lvalue".
     We have already performed any binary operator using the value as cast.
     Now convert the result to the cast type of the lhs,
     and then true type of the lhs and store it there;
     then convert result back to the cast type to be the value
     of the assignment.  */

  switch ( CTreeExprCode(lhs))
    {
    case C_NOP_EXPR:
    case C_CONVERT_EXPR:
    case C_FLOAT_EXPR:
    case C_FIX_TRUNC_EXPR:
    case C_FIX_FLOOR_EXPR:
    case C_FIX_ROUND_EXPR:
    case C_FIX_CEIL_EXPR:
      if ( CTreeTypeCode(CTreeType(newrhs)) == C_ARRAY_TYPE
          || CTreeTypeCode(CTreeType(newrhs)) == C_FUNCTION_TYPE)
      {
        newrhs = c_default_conversion(newrhs);
      }

      {
        c_tree_node *inner_lhs = CTreeExprOperand(lhs, 0);
        c_tree_node *result;
        result = c_parser_build_modify_expr(inner_lhs, C_NOP_EXPR,
                            c_convert_expr(CTreeType(inner_lhs),
                            c_convert_expr(lhstype, newrhs)));

        if ( CTreeNodeFirstCode(result) == C_ERROR_NODE)
        {
          return result;
        }
        c_pedantic_lvalue_warning (C_CONVERT_EXPR);
        return c_convert_expr(CTreeType(lhs), result);
      }
      
    default:
      break;
    }

  /* Now we have handled acceptable kinds of LHS that are not truly lvalues.
     Reject anything strange now.  */

  if (!c_lvalue_or_else (lhs, "assignment"))
  {
    return c_error_mark_node;
  }

  /* Warn about storing in something that is `const'.  */

  if ( CIsTreeNodeReadOnly(lhs) || CIsTreeNodeReadOnly(lhstype)
      || ((CTreeTypeCode(lhstype) == C_RECORD_TYPE
           || CTreeTypeCode(lhstype) == C_UNION_TYPE)
          && CIsTreeTypeFieldsReadOnly(lhstype)))
  {
    c_readonly_warning(lhs, "assignment");
  }

  /* If storing into a structure or union member,
     it has probably been given type `int'.
     Compute the type that would go with
     the actual amount of storage the member occupies.  */

  if ( CTreeExprCode(lhs) == C_COMPONENT_REF
      && ( CTreeTypeCode(lhstype) == C_INTEGER_TYPE
          ||  CTreeTypeCode(lhstype) == C_REAL_TYPE
          ||  CTreeTypeCode(lhstype) == C_ENUMERAL_TYPE))
  {
    lhstype = CTreeType( c_get_unwidened(lhs, 0));
  }

  /* If storing in a field that is in actuality a short or narrower than one,
     we must store in the field in its actual type.  */

  if (lhstype != CTreeType(lhs))
    {
      lhs = c_tree_copy_node(lhs);
      CTreeType(lhs) = lhstype;
    }

  /* Convert new value to destination type.  */

  newrhs = c_convert_for_assignment(lhstype, newrhs, "assignment", NULL,  NULL, 0);
  if ( CTreeNodeFirstCode(newrhs) == C_ERROR_NODE)
  {
    return c_error_mark_node;
  }

  result = c_tree_build_binary_expr( C_MODIFY_EXPR, lhs, newrhs);
  CTreeType( result ) = lhstype;
  CSetTreeNodeSideEffect( result );

  /* If we got the LHS in a different type for storing in,
     convert the result back to the nominal type of LHS
     so that the value we return always has the same type
     as the LHS argument.  */

  if (olhstype == CTreeType(result))
  {
    return result;
  }
  return c_convert_for_assignment(olhstype, result, "assignment", NULL, NULL, 0);
}

/*------------------------------------------------------------\
|                                                             |
|                   C Get Non LValue                        |
|                                                             |
\------------------------------------------------------------*/

/* Return an expr equal to X but certainly not valid as an lvalue.
   Also make sure it is not valid as an null pointer constant.  */

c_tree_node *c_get_non_lvalue( x )

     c_tree_node *x;
{
  c_tree_node *result;

  /* These things are certainly not lvalues.  */
  if (   CTreeNodeFirstCode(x) == C_INT_CST_NODE 
      || CTreeNodeFirstCode(x) == C_REAL_CST_NODE
      || CTreeNodeFirstCode(x) == C_STRING_NODE
      || ( ( CTreeNodeFirstCode(x) == C_EXPR_NODE ) &&
           ( ( CTreeExprCode(x) == C_NON_LVALUE_EXPR ) ||
             ( CTreeExprCode(x) == C_ADDR_EXPR       ) ) ) )
  {
    if ( CTreeNodeFirstCode(x) == C_INT_CST_NODE && c_tree_is_integer_zerop(x))
    {
        /* Use NOP_EXPR instead of NON_LVALUE_EXPR
           so convert_for_assignment won't strip it.
           This is so this 0 won't be treated as a null pointer constant.  */
      result = c_tree_build_unary_expr( C_NOP_EXPR, x );
      CTreeType( result ) = CTreeType( x );

      if ( CIsTreeNodeConstant( x ) ) CSetTreeNodeConstant( x );

      return result;
    }

    return x;
  }

  result = c_tree_build_unary_expr( C_NON_LVALUE_EXPR, x );
  CTreeType( result ) = CTreeType( x );

  if ( CIsTreeNodeConstant( x ) ) CSetTreeNodeConstant( x );

  return result;
}

/*------------------------------------------------------------\
|                                                             |
|               C Parser Build Compound Expr                |
|                                                             |
\------------------------------------------------------------*/


/* Warn if EXP contains any computations whose results are not used.
   Return 1 if a warning is printed; 0 otherwise.  */

int c_warn_if_unused_value (exp)
     c_tree_node *exp;
{
  if ( CIsTreeNodeUsed(exp))
    return 0;

  if ( CTreeNodeFirstCode(exp) == C_EXPR_NODE )
  {
    switch ( CTreeExprCode(exp))
    {
    case C_PREINCREMENT_EXPR:
    case C_POSTINCREMENT_EXPR:
    case C_PREDECREMENT_EXPR:
    case C_POSTDECREMENT_EXPR:
    case C_MODIFY_EXPR:
    case C_INIT_EXPR:
    case C_TARGET_EXPR:
    case C_CALL_EXPR:
    case C_METHOD_CALL_EXPR:
    case C_RTL_EXPR:
    case C_WITH_CLEANUP_EXPR:
    case C_EXIT_EXPR:
      /* We don't warn about COND_EXPR because it may be a useful
         construct if either arm contains a side effect.  */
    case C_COND_EXPR:
      return 0;

    case C_BIND_EXPR:
      /* For a binding, warn if no side effect within it.  */
      return c_warn_if_unused_value (CTreeExprOperand(exp, 1));

    case C_SAVE_EXPR:
      return c_warn_if_unused_value (CTreeExprOperand(exp, 1));

    case C_TRUTH_ORIF_EXPR:
    case C_TRUTH_ANDIF_EXPR:
      /* In && or ||, warn if 2nd operand has no side effect.  */
      return c_warn_if_unused_value (CTreeExprOperand(exp, 1));

    case C_COMPOUND_EXPR:
      if (CIsTreeNodeStatic(exp))
        return 0;
      if (c_warn_if_unused_value (CTreeExprOperand(exp, 0)))
        return 1;
      /* Let people do `(foo (), 0)' without a warning.  */
      if ( CIsTreeNodeConstant(CTreeExprOperand(exp, 1)))
        return 0;
      return c_warn_if_unused_value (CTreeExprOperand(exp, 1));

    case C_NOP_EXPR:
    case C_CONVERT_EXPR:
    case C_NON_LVALUE_EXPR:
      /* Don't warn about values cast to void.  */
      if (CTreeType(exp) == c_void_type_node)
        return 0;
      /* Don't warn about conversions not explicit in the user's program.  */
      if ( CIsTreeNodeStatic(exp))
        return 0;
      /* Assignment to a cast usually results in a cast of a modify.
         Don't complain about that.  There can be an arbitrary number of
         casts before the modify, so we must loop until we find the first
         non-cast expression and then test to see if that is a modify.  */
      {
        c_tree_node *tem = CTreeExprOperand(exp, 0);

        while ( ( CTreeNodeFirstCode(tem) == C_EXPR_NODE ) && 
            (CTreeExprCode(tem) == C_CONVERT_EXPR || CTreeExprCode(tem) == C_NOP_EXPR))
          tem = CTreeExprOperand(tem, 0);

        if ( (CTreeNodeFirstCode(tem) == C_EXPR_NODE) && 
            (CTreeExprCode(tem) == C_MODIFY_EXPR || CTreeExprCode(tem) == C_INIT_EXPR
            || CTreeExprCode(tem) == C_CALL_EXPR))
          return 0;
      }
      goto warn;

    case C_INDIRECT_REF:
      /* Don't warn about automatic dereferencing of references, since
         the user cannot control it.  */
      if (CTreeTypeCode(CTreeType(CTreeExprOperand(exp, 0))) == C_REFERENCE_TYPE)
        return c_warn_if_unused_value (CTreeExprOperand(exp, 0));
      /* ... fall through ...  */

    default :
       if ( CIsTreeNodeVolatile(exp) && 
           (CTreeExprCode(exp) == C_COMPONENT_REF ||
            CTreeExprCode(exp) == C_BIT_FIELD_REF ||
            CTreeExprCode(exp) == C_INDIRECT_REF ||
            CTreeExprCode(exp) == C_BUFFER_REF ||
            CTreeExprCode(exp) == C_ARRAY_REF) )
        return 0;
    }
  }
      
  /* Referencing a volatile value is a side effect, so don't warn.  */
  if ( CIsTreeNodeVolatile(exp) && 
       CTreeNodeFirstCode(exp) == C_DECL_NODE) return 0;

  warn:
     c_warning_with_file_and_line(c_emit_filename, c_emit_lineno,
                                 "value computed is not used");
  return 1;
}

/* Given a list of expressions, return a compound expression
   that performs them all and returns the value of the last of them.  */

static c_tree_node *c_internal_build_compound_expr(list, first_p)
     c_tree_node *list;
     int            first_p;
{
  c_tree_node *rest;
  c_tree_node *type;

  if ( CTreeChain(list) == 0)
  {
      /* Don't let (0, 0) be null pointer constant.  */
      if (!first_p && c_tree_is_integer_zerop( CTreeListValue(list)))
      {
        return c_get_non_lvalue( CTreeListValue(list));
      }

      return  CTreeListValue(list);
  }

  if ( CTreeChain(list) != 0 &&  CTreeChain(  CTreeChain(list)) == 0)
  {
    c_tree_node *tmp_node;

    tmp_node = CTreeChain(list);
    tmp_node = CTreeListValue( tmp_node );
    tmp_node = CTreeType( tmp_node );

    /*
    if ( tmp_node != NULL )
    */
    {
      /* Convert arrays to pointers when there really is a comma operator.  */
      if ( CTreeTypeCode( tmp_node ) == C_ARRAY_TYPE )
      {
         CTreeListValue( CTreeChain(list))
          = c_default_conversion( CTreeListValue( CTreeChain(list)));
      }
    }
  }

  rest = c_internal_build_compound_expr( CTreeChain(list), 0);

  if (!  CIsTreeNodeSideEffect( CTreeListValue(list)))
  {
      /* The left-hand operand of a comma expression is like an expression
         statement: with -W or -Wunused, we should warn if it doesn't have
         any side-effects, unless it was explicitly cast to (void).  */
      if ((c_extra_warnings || c_warn_unused)
           && ! ( CTreeExprCode( CTreeListValue(list)) == C_CONVERT_EXPR
                && CTreeType( CTreeListValue(list)) == c_void_type_node))
      {
        warning ("left-hand operand of comma expression has no effect");
      }

      /* When pedantic, a compound expression can be neither an lvalue
         nor an integer constant expression.  */
      if (! c_pedantic)
      {
        return rest;
      }
  }

  /* With -Wunused, we should also warn if the left-hand operand does have
     side-effects, but computes a value which is not used.  For example, in
     `foo() + bar(), baz()' the result of the `+' operator is not used,
     so we should issue a warning.  */
  else if (c_warn_unused)
  {
    c_warn_if_unused_value( CTreeListValue(list));
  }

  type = CTreeType( rest );
  rest = c_tree_build_binary_expr( C_COMPOUND_EXPR, CTreeListValue( list ), rest );
  CTreeType( rest ) = type;

  return( rest );
}

c_tree_node *c_parser_build_compound_expr(list)
     c_tree_node *list;
{
  return c_internal_build_compound_expr(list, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                    C Digest Init                          |
|                                                             |
\------------------------------------------------------------*/

/* Return nonzero if VALUE is a valid constant-valued expression
   for use in initializing a static variable; one that can be an
   element of a "constant" initializer.

   Return null_pointer_node if the value is absolute;
   if it is relocatable, return the variable that determines the relocation.
   We assume that VALUE has been folded as much as possible;
   therefore, we do not need to check for such things as
   arithmetic-combinations of integers.  */

c_tree_node *c_is_initializer_constant_valid_p(value, endtype)
     c_tree_node *value;
     c_tree_node *endtype;
{
  int code;

  code = CTreeNodeFirstCode(value);

  if (  code == C_INT_CST_NODE
     || code == C_REAL_CST_NODE
     || code == C_STRING_NODE
     || code == C_COMPLEX_NODE )
  {
    return c_null_pointer_node;
  }
  else
  if ( code == C_EXPR_NODE )
  {

  switch ( CTreeExprCode(value))
    {
    case C_CONSTRUCTOR_EXPR:
      if (( CTreeTypeCode(CTreeType(value)) == C_UNION_TYPE
           ||  CTreeTypeCode(CTreeType(value)) == C_RECORD_TYPE)
          && CIsTreeNodeConstant(value)
          && CTreeExprOperand(value,1))
        return
          c_is_initializer_constant_valid_p( 
              CTreeListValue( CTreeExprOperand(value,1)), endtype);
        
      return CIsTreeNodeStatic(value) ? c_null_pointer_node : 0;

    case C_ADDR_EXPR:
      return CTreeExprOperand(value, 0);

    case C_NON_LVALUE_EXPR:
      return c_is_initializer_constant_valid_p( CTreeExprOperand(value, 0), endtype);

    case C_CONVERT_EXPR:
    case C_NOP_EXPR:
      /* Allow conversions between pointer types.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_POINTER_TYPE
          &&  CTreeTypeCode(CTreeType(CTreeExprOperand(value, 0))) == C_POINTER_TYPE)
        return c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0), endtype);

      /* Allow conversions between real types.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_REAL_TYPE
          &&  CTreeTypeCode(CTreeType( CTreeExprOperand(value, 0))) == C_REAL_TYPE)
        return c_is_initializer_constant_valid_p( CTreeExprOperand(value, 0), endtype);

      /* Allow length-preserving conversions between integer types.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_INTEGER_TYPE
          &&  CTreeTypeCode(CTreeType(CTreeExprOperand(value, 0))) == C_INTEGER_TYPE
          && (CTreeTypePrecision(CTreeType(value))
              == CTreeTypePrecision(CTreeType(CTreeExprOperand(value, 0)))))
        return c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0), endtype);

      /* Allow conversions between other integer types only if
         explicit value.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_INTEGER_TYPE
          &&  CTreeTypeCode(CTreeType(CTreeExprOperand(value, 0))) == C_INTEGER_TYPE)
        {
          c_tree_node *inner = c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0),
                                                     endtype);
          if (inner == c_null_pointer_node)
            return c_null_pointer_node;
          return 0;
        }

      /* Allow (int) &foo provided int is as wide as a pointer.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_INTEGER_TYPE
          &&  CTreeTypeCode(CTreeType(CTreeExprOperand(value, 0))) == C_POINTER_TYPE
          && (CTreeTypePrecision(CTreeType(value))
              >= CTreeTypePrecision(CTreeType(CTreeExprOperand(value, 0)))))
        return c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0),
                                             endtype);

      /* Likewise conversions from int to pointers.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_POINTER_TYPE
          &&  CTreeTypeCode(CTreeType(CTreeExprOperand(value, 0))) == C_INTEGER_TYPE
          && (CTreeTypePrecision(CTreeType(value))
              <= CTreeTypePrecision(CTreeType(CTreeExprOperand(value, 0)))))
        return c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0),
                                             endtype);

      /* Allow conversions to union types if the value inside is okay.  */
      if ( CTreeTypeCode(CTreeType(value)) == C_UNION_TYPE)
        return c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0),
                                             endtype);
      return 0;

    case C_PLUS_EXPR:
      if ( CTreeTypeCode(endtype) == C_INTEGER_TYPE
          && CTreeTypePrecision(endtype) < C_POINTER_SIZE)
        return 0;
      {
        c_tree_node *valid0 = c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0),
                                                    endtype);
        c_tree_node *valid1 = c_is_initializer_constant_valid_p(CTreeExprOperand(value, 1),
                                                    endtype);
        /* If either term is absolute, use the other terms relocation.  */
        if (valid0 == c_null_pointer_node)
          return valid1;
        if (valid1 == c_null_pointer_node)
          return valid0;
        return 0;
      }

    case C_MINUS_EXPR:
      if ( CTreeTypeCode(endtype) == C_INTEGER_TYPE
          && CTreeTypePrecision(endtype) < C_POINTER_SIZE)
        return 0;
      {
        c_tree_node *valid0 = c_is_initializer_constant_valid_p(CTreeExprOperand(value, 0),
                                                    endtype);
        c_tree_node *valid1 = c_is_initializer_constant_valid_p(CTreeExprOperand(value, 1),
                                                    endtype);
        /* Win if second argument is absolute.  */
        if (valid1 == c_null_pointer_node)
          return valid0;
        /* Win if both arguments have the same relocation.
           Then the value is absolute.  */
        if (valid0 == valid1)
          return c_null_pointer_node;
        return 0;
      }

    default:
      return 0;
    }
  }

  return 0;
}


/* If VALUE is a compound expr all of whose expressions are constant, then
   return its value.  Otherwise, return error_mark_node.

   This is for handling COMPOUND_EXPRs as initializer elements
   which is allowed with a warning when -pedantic is specified.  */

static c_tree_node *c_is_valid_compound_expr_initializer(value, endtype)
     c_tree_node *value;
     c_tree_node *endtype;
{
  if ( CTreeExprCode(value) == C_COMPOUND_EXPR)
    {
      if ( c_is_valid_compound_expr_initializer(CTreeExprOperand(value, 0), endtype)
          == c_error_mark_node)
        return c_error_mark_node;
      return c_is_valid_compound_expr_initializer(CTreeExprOperand (value, 1), endtype);
    }
  else if (! CIsTreeNodeConstant(value)
           && ! c_is_initializer_constant_valid_p(value, endtype))
    return c_error_mark_node;
  else
    return value;
}

/* Digest the parser output INIT as an initializer for type TYPE.
   Return a C expression of type TYPE to represent the initial value.

   The arguments REQUIRE_CONSTANT and CONSTRUCTOR_CONSTANT request errors
   if non-constant initializers or elements are seen.  CONSTRUCTOR_CONSTANT
   applies only to elements of constructors.  */

static c_tree_node *c_digest_init( type, init, require_constant, constructor_constant )
     c_tree_node *type;
     c_tree_node *init;
     int require_constant, constructor_constant;
{
  int code = CTreeTypeCode(type);
  c_tree_node *inside_init = init;

  if (init == c_error_mark_node)
    return init;

  /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
  /* Do not use STRIP_NOPS here.  We do not want an enumerator
     whose value is 0 to count as a null pointer constant.  */
  if ( CTreeExprCode(init) == C_NON_LVALUE_EXPR)
  {
    inside_init = CTreeExprOperand (init, 0);
  }

  /* Initialization of an array of chars from a string constant
     optionally enclosed in braces.  */

  if (code == C_ARRAY_TYPE)
    {
      c_tree_node *typ1 = CTreeTypeMainVariant(CTreeType(type));
      if ((typ1 == c_char_type_node
           || typ1 == c_signed_char_type_node
           || typ1 == c_unsigned_char_type_node
           || typ1 == c_unsigned_wchar_type_node
           || typ1 == c_signed_wchar_type_node)
          && ((inside_init && CTreeNodeFirstCode(inside_init) == C_STRING_NODE)))
        {
          if (c_is_comptypes(CTreeTypeMainVariant(CTreeType(inside_init)),
                         CTreeTypeMainVariant(type)))
          {
            return inside_init;
          }

          if ((CTreeTypeMainVariant(CTreeType(CTreeType(inside_init)))
               != c_char_type_node)
              && CTreeTypePrecision(typ1) == CTreeTypePrecision(c_char_type_node))
            {
              error("char-array initialized from wide string" );
              return c_error_mark_node;
            }
          if ((CTreeTypeMainVariant(CTreeType(CTreeType(inside_init)))
               == c_char_type_node)
              && CTreeTypePrecision(typ1) != CTreeTypePrecision(c_char_type_node))
            {
              error("int-array initialized from non-wide string");
              return c_error_mark_node;
            }

          CTreeType(inside_init) = type;
          if ( CTreeTypeValues(type) != 0
              && CTreeNodeFirstCode(CTreeTypeSize(type)) == C_INT_CST_NODE)
            {
              register int size = CTreeIntCstLow(CTreeTypeSize(type));
              size = (size + C_BITS_PER_UNIT - 1) / C_BITS_PER_UNIT;
              /* Subtract 1 (or sizeof (wchar_t))
                 because it's ok to ignore the terminating null char
                 that is counted in the length of the constant.  */
              if (size < CTreeStringLength(inside_init)
                  - (CTreeTypePrecision(typ1) != CTreeTypePrecision(c_char_type_node)
                     ? CTreeTypePrecision(c_wchar_type_node) / C_BITS_PER_UNIT
                     : 1))
                pedwarn( "initializer-string for array of chars is too long");
            }
          return inside_init;
        }
    }

  /* Any type can be initialized
     from an expression of the same type, optionally with braces.  */

  if (inside_init && CTreeType(inside_init) != 0
      && (c_is_comptypes(CTreeTypeMainVariant(CTreeType(inside_init)),
                     CTreeTypeMainVariant(type))
          || (code == C_ARRAY_TYPE
              && c_is_comptypes(CTreeType(inside_init), type))
          || (code == C_POINTER_TYPE
              && ( CTreeNodeFirstCode(CTreeType(inside_init)) == C_ARRAY_TYPE
                  || CTreeNodeFirstCode(CTreeType(inside_init)) == C_FUNCTION_TYPE)
              && c_is_comptypes(CTreeType(CTreeType(inside_init)),
                            CTreeType(type)))))
    {
      if (code == C_POINTER_TYPE
          && ( CTreeNodeFirstCode(CTreeType(inside_init)) == C_ARRAY_TYPE
              || CTreeNodeFirstCode(CTreeType(inside_init)) == C_FUNCTION_TYPE))
      {
        inside_init = c_default_conversion(inside_init);
      }
      else if (code == C_ARRAY_TYPE && CTreeNodeFirstCode(inside_init) != C_STRING_NODE
               && 
               ( CTreeNodeFirstCode(inside_init) != C_EXPR_NODE || 
                 CTreeExprCode(inside_init) != C_CONVERT_EXPR) )
        {
          error("array initialized from non-constant array expression" );
          return c_error_mark_node;
        }

# ifdef LUDO_DONE
      if (optimize && CTreeDeclCode(inside_init) == C_VAR_DECL)
      {
        inside_init = c_decl_constant_value(inside_init);
      }
# endif

      /* Compound expressions can only occur here if -pedantic or
         -pedantic-errors is specified.  In the later case, we always want
         an error.  In the former case, we simply want a warning.  */
      if (require_constant && c_pedantic
          && CTreeExprCode(inside_init) == C_COMPOUND_EXPR)
        {
          inside_init
            = c_is_valid_compound_expr_initializer(inside_init,
                                               CTreeType(inside_init));
          if (inside_init == c_error_mark_node)
          {
            error("initializer element is not constant");
          }
          else
          {
            pedwarn("initializer element is not constant" );
          }

          if (c_flag_pedantic_errors)
          {
            inside_init = c_error_mark_node;
          }
        }
      else if (require_constant && ! CIsTreeNodeConstant(inside_init))
        {
          error("initializer element is not constant" );
          inside_init = c_error_mark_node;
        }
      else if (require_constant
               && c_is_initializer_constant_valid_p(inside_init, CTreeType(inside_init)) == 0)
        {
          error("initializer element is not computable at load time" );
          inside_init = c_error_mark_node;
        }

      return inside_init;
    }

  /* Handle scalar types, including conversions.  */

  if (code == C_INTEGER_TYPE || code == C_REAL_TYPE || code == C_POINTER_TYPE
      || code == C_ENUMERAL_TYPE || code == C_COMPLEX_TYPE)
    {
      /* Note that convert_for_assignment calls default_conversion
         for arrays and functions.  We must not call it in the
         case where inside_init is a null pointer constant.  */
      inside_init
        = c_convert_for_assignment(type, init, "initialization", NULL, NULL, 0);

      if (require_constant && ! CIsTreeNodeConstant(inside_init))
        {
          error("initializer element is not constant" );
          inside_init = c_error_mark_node;
        }
      else if (require_constant
               && c_is_initializer_constant_valid_p(inside_init, CTreeType(inside_init)) == 0)
        {
          error("initializer element is not computable at load time" );
          inside_init = c_error_mark_node;
        }

      return inside_init;
    }

  /* Come here only for records and arrays.  */

  if (CTreeTypeSize(type) && CTreeNodeFirstCode(CTreeTypeSize(type)) != C_INT_CST_NODE)
    {
      error("variable-sized object may not be initialized" );
      return c_error_mark_node;
    }

  /* Traditionally, you can write  struct foo x = 0;
     and it initializes the first element of x to 0.  */
  if (c_flag_traditional)
    {
      c_tree_node *top = 0;
      c_tree_node *prev = 0;
      c_tree_node *otype = type;

      while (    CTreeTypeCode(type) == C_RECORD_TYPE
             ||  CTreeTypeCode(type) == C_ARRAY_TYPE
             ||  CTreeTypeCode(type) == C_QUAL_UNION_TYPE
             ||  CTreeTypeCode(type) == C_UNION_TYPE)
        {
          c_tree_node *temp = c_tree_build_unary_expr( C_CONSTRUCTOR_EXPR, NULL );
          CTreeType( temp ) = type;

          if (prev == 0)
          {
            top = temp;
          }
          else
          {
            CTreeExprOperand(prev, 1) = c_tree_build_list(NULL, temp);
          }
          prev = temp;
          if ( CTreeTypeCode(type) == C_ARRAY_TYPE)
          {
            type = CTreeType(type);
          }
          else if ( CTreeTypeValues(type))
          {
            type = CTreeType( CTreeTypeValues(type));
          }
          else
            {
              error("invalid initializer" );
              return c_error_mark_node;
            }
        }

      if (otype != type)
        {
          CTreeExprOperand(prev, 1)
            = c_tree_build_list( NULL,
                                 c_digest_init(type, init, require_constant,
                                            constructor_constant));
          return top;
        }
      else
        return c_error_mark_node;
    }
  error("invalid initializer");
  return c_error_mark_node;
}

/*------------------------------------------------------------\
|                                                             |
|               C Store Init Value                          |
|                                                             |
\------------------------------------------------------------*/


/* Perform appropriate conversions on the initial value of a variable,
   store it in the declaration DECL,
   and print any error messages that are appropriate.
   If the init is invalid, store an ERROR_MARK.  */

void c_store_init_value (decl, init)

  c_tree_node *decl;
  c_tree_node *init;
{
  c_tree_node *value;
  c_tree_node *type;

  /* If variable's type was invalidly declared, just ignore it.  */

  type = CTreeType(decl);
  if ( CTreeNodeFirstCode(type) == C_ERROR_NODE) return;

  /* Digest the specified initializer into an expression.  */

  value = c_digest_init(type, init, CIsTreeNodeStatic(decl),
                         CIsTreeNodeStatic(decl) || c_pedantic);

  /* Store the expression if valid; else report error.  */
  CTreeDeclInitial(decl) = value;

  /* ANSI wants warnings about out-of-range constant initializers.  */
  CStripTypeNops(value);
  c_constant_expression_warning(value);
}

/*------------------------------------------------------------\
|                                                             |
|               C Get Signed Or Unsigned Type               |
|                                                             |
\------------------------------------------------------------*/

/* Return a type the same as TYPE except unsigned or
   signed according to UNSIGNEDP.  */

c_tree_node *c_get_signed_or_unsigned_type( unsignedp, type )
     int unsignedp;
     c_tree_node *type;
{
  int Precision;

  if ((! INTEGRAL_TYPE_P (type) && ! POINTER_TYPE_P (type))
      || !!CIsTreeNodeUnsigned(type) == unsignedp)
    return type;

  Precision =  CTreeTypePrecision(type);

  if ( Precision == CTreeTypePrecision( c_signed_char_type_node))
  {
    return unsignedp ?  c_unsigned_char_type_node :  c_signed_char_type_node;
  }
  if ( Precision ==  CTreeTypePrecision( c_integer_type_node)) 
  {
    return unsignedp ?  c_unsigned_type_node :  c_integer_type_node;
  }
  if ( Precision ==  CTreeTypePrecision( c_short_integer_type_node)) 
  {
    return unsignedp ?  c_short_unsigned_type_node :  c_short_integer_type_node;
  }
  if ( Precision ==  CTreeTypePrecision( c_long_integer_type_node)) 
  {
    return unsignedp ?  c_long_unsigned_type_node :  c_long_integer_type_node;
  }
  if ( Precision ==  CTreeTypePrecision( c_long_long_integer_type_node)) 
  {
    return (unsignedp ?  c_long_long_unsigned_type_node :  c_long_long_integer_type_node);
  }

  return type;
}

/*------------------------------------------------------------\
|                                                             |
|               C Get Signed Type                           |
|                                                             |
\------------------------------------------------------------*/

/* Return a signed type the same as TYPE in other respects.  */

c_tree_node *c_get_signed_type( type )

  c_tree_node *type;
{
  c_tree_node *type1 = CTreeTypeMainVariant(type);

  if (type1 == c_unsigned_char_type_node || type1 == c_char_type_node)
    return c_signed_char_type_node;
  if (type1 == c_unsigned_type_node)
    return c_integer_type_node;
  if (type1 == c_short_unsigned_type_node)
    return c_short_integer_type_node;
  if (type1 == c_long_unsigned_type_node)
    return c_long_integer_type_node;
  if (type1 == c_long_long_unsigned_type_node)
    return c_long_long_integer_type_node;
# ifdef LUDO_NO_SKIP
  if (type1 == c_unsigned_intDI_type_node)
    return c_intDI_type_node;
  if (type1 == c_unsigned_intSI_type_node)
    return c_intSI_type_node;
  if (type1 == c_unsigned_intHI_type_node)
    return c_intHI_type_node;
  if (type1 == c_unsigned_intQI_type_node)
    return c_intQI_type_node;
# endif

  return c_get_signed_or_unsigned_type(0, type);
}

/* Return an unsigned type the same as TYPE in other respects.  */

c_tree_node *c_get_unsigned_type( type )

  c_tree_node *type;
{
  c_tree_node *type1 = CTreeTypeMainVariant(type);

  if (type1 == c_signed_char_type_node || type1 == c_char_type_node)
    return c_unsigned_char_type_node;
  if (type1 == c_integer_type_node)
    return c_unsigned_type_node;
  if (type1 == c_short_integer_type_node)
    return c_short_unsigned_type_node;
  if (type1 == c_long_integer_type_node)
    return c_long_unsigned_type_node;
  if (type1 == c_long_long_integer_type_node)
    return c_long_long_unsigned_type_node;
# ifdef LUDO_NO_SKIP
  if (type1 == intDI_type_node)
    return unsigned_intDI_type_node;
  if (type1 == intSI_type_node)
    return unsigned_intSI_type_node;
  if (type1 == intHI_type_node)
    return unsigned_intHI_type_node;
  if (type1 == intQI_type_node)
    return unsigned_intQI_type_node;
# endif

  return c_get_signed_or_unsigned_type(1, type);
}

/* Nonzero if integer constant C has a value that is permissible
   for type TYPE (an INTEGER_TYPE).  */

int c_int_fits_type_p(c, type)
     c_tree_node *c;
     c_tree_node *type;
{
  if (CIsTreeNodeUnsigned(type))
  {
    return (! (CTreeNodeFirstCode( CTreeTypeMaxValue(type)) == C_INT_CST_NODE
               && INT_CST_LT_UNSIGNED( CTreeTypeMaxValue(type), c))
            && ! ( CTreeNodeFirstCode( CTreeTypeMinValue(type)) == C_INT_CST_NODE
                  && INT_CST_LT_UNSIGNED(c, CTreeTypeMinValue(type)))
            /* Negative ints never fit unsigned types.  */
            && ! ( CTreeIntCstHigh(c) < 0
                  && !  CIsTreeNodeUnsigned( CTreeType(c))));
  }
  else
  {
    return (! ( CTreeNodeFirstCode(CTreeTypeMaxValue(type)) == C_INT_CST_NODE
               && INT_CST_LT( CTreeTypeMaxValue(type), c))
            && ! ( CTreeNodeFirstCode( CTreeTypeMinValue(type)) == C_INT_CST_NODE
                  && INT_CST_LT(c, CTreeTypeMinValue(type)))
            /* Unsigned ints with top bit set never fit signed types.  */
            && ! ( CTreeIntCstHigh(c) < 0
                  && CIsTreeNodeUnsigned(CTreeType(c))));
  }
}

/* Return an integer type with BITS bits of precision,
   that is unsigned if UNSIGNEDP is nonzero, otherwise signed.  */

c_tree_node *c_get_type_for_size( bits, unsignedp )
     unsigned bits;
     int unsignedp;
{
  if (bits == CTreeTypePrecision( c_integer_type_node))
    return unsignedp ?  c_unsigned_type_node :  c_integer_type_node;

  if (bits == CTreeTypePrecision( c_signed_char_type_node))
    return unsignedp ?  c_unsigned_char_type_node :  c_signed_char_type_node;

  if (bits == CTreeTypePrecision( c_short_integer_type_node))
    return unsignedp ?  c_short_unsigned_type_node :  c_short_integer_type_node;

  if (bits == CTreeTypePrecision( c_long_integer_type_node))
    return unsignedp ?  c_long_unsigned_type_node :  c_long_integer_type_node;

  if (bits == CTreeTypePrecision( c_long_long_integer_type_node))
    return (unsignedp ?  c_long_long_unsigned_type_node
            :  c_long_long_integer_type_node);

# ifdef LUDO_NO_SKIP
  if (bits <= CTreeTypePrecision(intQI_type_node))
    return unsignedp ? unsigned_intQI_type_node : intQI_type_node;

  if (bits <= CTreeTypePrecision(intHI_type_node))
    return unsignedp ? unsigned_intHI_type_node : intHI_type_node;

  if (bits <= CTreeTypePrecision(intSI_type_node))
    return unsignedp ? unsigned_intSI_type_node : intSI_type_node;

  if (bits <= CTreeTypePrecision(intDI_type_node))
    return unsignedp ? unsigned_intDI_type_node : intDI_type_node;
# endif

  return (c_tree_node *)0;
}

/* Return the precision of a type, for arithmetic purposes.
   Supports all types on which arithmetic is possible
   (including pointer types).
   It's not clear yet what will be right for complex types.  */

int c_type_precision(type)

     c_tree_node *type;
{
  int TypeType;

  TypeType = CTreeTypeCode( type );

  return (( TypeType == C_INTEGER_TYPE ||
            TypeType == C_ENUMERAL_TYPE ||
            TypeType == C_REAL_TYPE)
          ? CTreeTypePrecision(type) : C_POINTER_TYPE_SIZE);
}

/* Make a variant type in the proper way for C/C++, propagating qualifiers
   down to the element type of an array.  */

c_tree_node *c_c_build_type_variant(type, constp, volatilep)
     c_tree_node *type;
     int constp, volatilep;
{
  if ( CTreeTypeCode(type) == C_ARRAY_TYPE)
  {
    return c_tree_build_array_type( 
        c_c_build_type_variant( CTreeType(type), constp, volatilep),
        CTreeTypeValues(type));
  }
  return c_tree_build_type_variant(type, constp, volatilep);
}


/* Within a struct initializer, specify the next field to be initialized.  */

void c_set_init_label(fieldname)

     c_tree_node *fieldname;
{
  c_tree_node *tail;
  int passed = 0;

  /* Don't die if an entire brace-pair level is superfluous
     in the containing level.  */
  if (c_constructor_type == 0) return;

  for (tail = CTreeTypeValues(c_constructor_type); tail;
       tail = CTreeChain(tail))
    {
      if (tail == c_constructor_unfilled_fields)
      {
        passed = 1;
      }
      if (CTreeDeclName(tail) == fieldname) break;
    }

  if (tail == 0)
  {
    error ("unknown field `%s' specified in initializer", CTreeIdentPointer(fieldname));
  }
  else if (!passed)
  {
    error ("field `%s' already initialized", CTreeIdentPointer(fieldname));
  }
  else
  {
    c_constructor_fields = tail;

    if (c_pedantic)
    {
      pedwarn ("ANSI C forbids specifying structure member to initialize");
    }
  }
}


/* Within an array initializer, specify the next index to be initialized.
   FIRST is that index.  If LAST is nonzero, then initialize a range
   of indices, running from FIRST through LAST.  */

void c_set_init_index(first, last)
     c_tree_node *first;
     c_tree_node *last;
{
  while (( CTreeExprCode(first) == C_NOP_EXPR
          || CTreeExprCode(first) == C_CONVERT_EXPR
          || CTreeExprCode(first) == C_NON_LVALUE_EXPR)
         && (CTreeTypeMode(CTreeType(first))
             == CTreeTypeMode(CTreeType( CTreeExprOperand(first, 0)))))
  {
    (first) = CTreeExprOperand(first, 0);
  }

  if (last)
  {
    while (( CTreeExprCode(last) == C_NOP_EXPR
            || CTreeExprCode(last) == C_CONVERT_EXPR
            || CTreeExprCode(last) == C_NON_LVALUE_EXPR)
           && (CTreeTypeMode(CTreeType(last))
               == CTreeTypeMode(CTreeType( CTreeExprOperand(last, 0)))))
    {
      (last) = CTreeExprOperand(last, 0);
    }
  }

  if ( CTreeNodeFirstCode(first) != C_INT_CST_NODE)
  {
    error("nonconstant array index in initializer");
  }
  else if (last != 0 && CTreeNodeFirstCode(last) != C_INT_CST_NODE)
  {
    error("nonconstant array index in initializer" );
  }
  else if (! c_constructor_unfilled_index)
  {
    error("array index in non-array initializer" );
  }
  else if (c_tree_is_int_cst_lt(first, c_constructor_unfilled_index))
  {
    error("duplicate array index in initializer" );
  }
  else
  {
    CTreeIntCstLow(c_constructor_index) = CTreeIntCstLow(first);
    CTreeIntCstHigh(c_constructor_index) = CTreeIntCstHigh(first);

    if (last != 0 && c_tree_is_int_cst_lt(last, first))
    {
      error("empty index range in initializer" );
    }
    else
    {
      if (c_pedantic)
      {
        pedwarn ("ANSI C forbids specifying element to initialize");
      }

      c_constructor_range_end = last;
    }
  }
}


/* Build an expression representing a cast to type TYPE of expression EXPR.  */

c_tree_node *c_build_c_cast(type, expr)

     c_tree_node *type;
     c_tree_node *expr;
{
  c_tree_node *value = expr;
  
  if (type == c_error_mark_node || expr == c_error_mark_node)
  {
    return c_error_mark_node;
  }

  type = CTreeTypeMainVariant(type);

  if ( CTreeTypeCode(type) == C_ARRAY_TYPE)
  {
    error ("cast specifies array type");
    return c_error_mark_node;
  }

  if ( CTreeTypeCode(type) == C_FUNCTION_TYPE)
  {
    error ("cast specifies function type");
    return c_error_mark_node;
  }

  if (type == CTreeType(value))
  {
    if (c_pedantic)
    {
      if ( CTreeTypeCode(type) == C_RECORD_TYPE
              || CTreeTypeCode(type) == C_UNION_TYPE)
      {
        pedwarn ("ANSI C forbids casting nonscalar to the same type");
      }
    }
  }
  else if ( CTreeTypeCode(type) == C_UNION_TYPE)
  {
    c_tree_node *field;

    if ( CTreeTypeCode(CTreeType(value)) == C_ARRAY_TYPE
          || CTreeTypeCode(CTreeType(value)) == C_FUNCTION_TYPE)
    {
        value = c_default_conversion(value);
    }

    for (field = CTreeTypeValues(type); field; field = CTreeChain(field))
    {
        if (c_is_comptypes(
              CTreeTypeMainVariant(CTreeType(field)),
               CTreeTypeMainVariant(CTreeType(value)))) break;
    }

    if (field)
    {
      char *name;
      c_tree_node *t;

      if (c_pedantic)
      {
        pedwarn ("ANSI C forbids casts to union type");
      }

      if ( CTreeTypeName(type) != 0)
      {
        if ( CTreeNodeFirstCode( CTreeTypeName(type)) == C_IDENTIFIER_NODE)
        {
          name = CTreeIdentPointer( CTreeTypeName(type));
        }
        else
        {
          name = CTreeIdentPointer(CTreeDeclName( CTreeTypeName(type)));
        }
      }
      else
      {
        name = "";
      }

      t = c_tree_build_list(field, value);
      t = c_tree_build_binary_expr( C_CONSTRUCTOR_EXPR, NULL, t );
      CTreeType( t ) = type;

      t = c_digest_init(type, t, 0, 0 );

      if ( CIsTreeNodeConstant( value ) )
      {
        CSetTreeNodeConstant(t);
      }
      return t;
    }

    error("cast to union type from type not present in union");
    return c_error_mark_node;
  }
  else
  {
    c_tree_node *otype;
    c_tree_node *ovalue;

    /* If casting to void, avoid the error that would come
       from default_conversion in the case of a non-lvalue array.  */
    if (type == c_void_type_node)
    {
      value = c_tree_build_unary_expr( C_CONVERT_EXPR, value );
      CTreeType( value ) = type;
      return value;
    }

    /* Convert functions and arrays to pointers,
       but don't convert any other types.  */
    if ( CTreeTypeCode(CTreeType(value)) == C_FUNCTION_TYPE
          || CTreeTypeCode(CTreeType(value)) == C_ARRAY_TYPE)
    {
      value = c_default_conversion(value);
    }

    otype = CTreeType(value);

    /* Optionally warn about potentially worrisome casts.  */

    if (c_warn_cast_qual
          && CTreeTypeCode(type) == C_POINTER_TYPE
          && CTreeTypeCode(otype) == C_POINTER_TYPE)
    {
      if ( CIsTreeNodeVolatile(CTreeType(otype))
              && ! CIsTreeNodeVolatile(CTreeType(type)))
      {
        pedwarn("cast discards `volatile' from pointer target type");
      }
      if ( CIsTreeNodeReadOnly(CTreeType(otype))
              && ! CIsTreeNodeReadOnly(CTreeType(type)))
      {
        pedwarn("cast discards `const' from pointer target type");
      }
    }

# ifdef LUDO_NO_SKIP
    /* Warn about possible alignment problems.  */
    if (STRICT_ALIGNMENT && warn_cast_align
        && TREE_CODE (type) == POINTER_TYPE
        && TREE_CODE (otype) == POINTER_TYPE
        && TREE_CODE (CTreeType(otype)) != VOID_TYPE
        && TREE_CODE (CTreeType(otype)) != FUNCTION_TYPE
        /* Don't warn about opaque types, where the actual alignment
           restriction is unknown.  */
        && !((TREE_CODE (CTreeType(otype)) == UNION_TYPE
              || TREE_CODE (CTreeType(otype)) == RECORD_TYPE)
             && CTreeTypeMode(CTreeType(otype)) == VOIDmode)
        && TYPE_ALIGN (CTreeType(type)) > TYPE_ALIGN (CTreeType(otype)))
      warning ("cast increases required alignment of target type");
# endif

    if ( CTreeTypeCode(type) == C_INTEGER_TYPE
        && CTreeTypeCode(otype) == C_POINTER_TYPE
        && CTreeTypePrecision(type) != CTreeTypePrecision(otype)
        && ! CIsTreeNodeConstant(value))
    {
      warning ("cast from pointer to integer of different size");
    }

    if (c_warn_bad_function_cast
        && ( CTreeNodeFirstCode(value) == C_EXPR_NODE && CTreeExprCode(value) == C_CALL_EXPR)
        && CTreeTypeCode(type) != CTreeTypeCode(otype))
    {
      warning ("cast does not match function type");
    }

    if ( CTreeTypeCode(type) == C_POINTER_TYPE
        && CTreeTypeCode(otype) == C_INTEGER_TYPE
        && CTreeTypePrecision(type) != CTreeTypePrecision(otype)
          /* Don't warn about converting any constant.  */
          && ! CIsTreeNodeConstant(value))
    {
      warning ("cast to pointer from integer of different size");
    }

    ovalue = value;
    value = c_convert_expr(type, value);

    /* Ignore any integer overflow caused by the cast.  */
    if ( CTreeNodeFirstCode(value) == C_INT_CST_NODE)
    {
      if ( CIsTreeNodeOverflow(ovalue) ) CSetTreeNodeOverflow(value);
      if ( CIsTreeNodeStatic(ovalue) ) CSetTreeNodeStatic(value);
    }
  }

  /* Pedantically, don't ley (void *) (FOO *) 0 be a null pointer constant.  */
  if (c_pedantic && CTreeNodeFirstCode(value) == C_INT_CST_NODE
      && CTreeNodeFirstCode(expr) == C_INT_CST_NODE
      && CTreeTypeCode(CTreeType(expr)) != C_INTEGER_TYPE)
  {
    value = c_get_non_lvalue(value);
  }

  /* If pedantic, don't let a cast be an lvalue.  */
  if (value == expr && c_pedantic)
  {
    value = c_get_non_lvalue(value);
  }

  return value;
}

/* Compute the value of the `sizeof' operator.  */

c_tree_node *c_get_c_sizeof(type)

  c_tree_node *type;
{
  int code;
  c_tree_node *t;

  if ( CTreeNodeFirstCode( type ) == C_ERROR_NODE )
  {
    return c_tree_get_size_int(1);
  }

  code = CTreeTypeCode(type);

  if (code == C_FUNCTION_TYPE)
  {
    if (c_pedantic || c_warn_pointer_arith)
    {
      pedwarn ("sizeof applied to a function type");
    }

    return c_tree_get_size_int(1);
  }

  if (code == C_VOID_TYPE)
  {
    if (c_pedantic || c_warn_pointer_arith)
    {
      pedwarn ("sizeof applied to a void type");
    }

    return c_tree_get_size_int(1);
  }

  if (CTreeTypeSize(type) == 0)
  {
    error ("sizeof applied to an incomplete type");

    return c_tree_get_size_int(0);
  }

  /* Convert in case a char is more than one unit.  */
  t = c_tree_build_binary_expr(
        C_CEIL_DIV_EXPR, 
        CTreeTypeSize(type), 
        c_tree_get_size_int( CTreeTypePrecision(c_char_type_node)));
  CTreeType( t ) = c_size_type;

  /* size_binop does not put the constant in range, so do it now.  */
  if (CTreeNodeFirstCode(t) == C_INT_CST_NODE && c_force_fit_type(t, 0))
  {
    CSetTreeNodeStatic(t);
    CSetTreeNodeOverflow(t);
  }

  return t;
}


/* Implement the __alignof keyword: Return the minimum required
   alignment of TYPE, measured in bytes.  */

c_tree_node *c_get_c_alignof(type)

     c_tree_node *type;
{
  int code;

  if ( CTreeNodeFirstCode( type ) == C_ERROR_NODE )
  {
    return c_tree_get_size_int(1);
  }
    
  code = CTreeTypeCode(type);

# ifdef LUDO_NO_SKIP
  if (code == C_FUNCTION_TYPE)
  {
    return c_tree_get_size_int( FUNCTION_BOUNDARY / C_BITS_PER_UNIT);
  }
# endif

  if (code == C_VOID_TYPE)
  {
    return c_tree_get_size_int(1);
  }

  return c_tree_get_size_int( CTreeTypeAlign(type) / C_BITS_PER_UNIT);
}

/* Implement the __alignof keyword: Return the minimum required
   alignment of EXPR, measured in bytes.  For VAR_DECL's and
   FIELD_DECL's return DECL_ALIGN (which can be set from an
   "aligned" __attribute__ specification).  */

c_tree_node *c_get_c_alignof_expr(expr)

     c_tree_node *expr;
{
  if ( ( CTreeNodeFirstCode(expr) == C_DECL_NODE ) &&
       ( CTreeDeclCode( expr ) == C_VAR_DECL ) )
  {
# ifdef LUDO_NO_SKIP
    return c_tree_get_size_int(DECL_ALIGN (expr) / C_BITS_PER_UNIT);
# else
    return c_get_c_alignof(CTreeType(expr));

# endif
  }
 
  if ( CTreeExprCode(expr) == C_COMPONENT_REF
      && CIsTreeDeclCBitField( CTreeExprOperand(expr, 1)))
  {
    error("`__alignof' applied to a bit-field");
    return c_tree_get_size_int(1);
  }
  else if ( CTreeExprCode(expr) == C_COMPONENT_REF
      && CTreeDeclCode( CTreeExprOperand(expr, 1)) == C_FIELD_DECL)
  {
# ifdef LUDO_NO_SKIP
    return c_tree_get_size_int( 
         DECL_ALIGN( CTreeExprOperand(expr, 1)) / C_BITS_PER_UNIT);
# else
    return c_tree_get_size_int(1);
# endif
  }
 
  if ( CTreeExprCode(expr) == C_INDIRECT_REF)
  {
    c_tree_node *t = CTreeExprOperand(expr, 0);
    c_tree_node *best = t;
    int bestalign = CTreeTypeAlign(CTreeType(CTreeType(t)));
 
    while ( CTreeExprCode(t) == C_NOP_EXPR
            && CTreeTypeCode(CTreeType( CTreeExprOperand(t, 0))) == C_POINTER_TYPE)
    {
      int thisalign;

      t = CTreeExprOperand(t, 0);
      thisalign = CTreeTypeAlign(CTreeType(CTreeType(t)));

      if (thisalign > bestalign) best = t, bestalign = thisalign;
    }

    return c_get_c_alignof(CTreeType(CTreeType(best)));
  }
  else
  {
    return c_get_c_alignof(CTreeType(expr));
  }
}

/* Return the size of TYPE (in bytes) as a wide integer
   or return -1 if the size can vary or is larger than an integer.  */

int c_get_int_size_in_bytes(type)
     c_tree_node *type;
{
  c_tree_node *t;

  if (type == c_error_mark_node)
  {
    return 0;
  }

  type = CTreeTypeMainVariant(type);
  if (CTreeTypeSize(type) == 0
      ||  CTreeNodeFirstCode(CTreeTypeSize(type)) != C_INT_CST_NODE)
  {
    return -1;
  }

  if ( CTreeIntCstHigh(CTreeTypeSize(type)) == 0)
    return (( CTreeIntCstLow(CTreeTypeSize(type)) + C_BITS_PER_UNIT - 1)
          / C_BITS_PER_UNIT);

  t = c_tree_build_binary_expr(
        C_CEIL_DIV_EXPR, 
        CTreeTypeSize(type), 
        c_tree_get_size_int( C_BITS_PER_UNIT ));
  CTreeType( t ) = c_size_type;

  if ( CTreeNodeFirstCode(t) != C_INT_CST_NODE || CTreeIntCstHigh(t) != 0)
  {
    return -1;
  }

  return CTreeIntCstLow(t);
}



/* Compute the size to increment a pointer by.  */

c_tree_node *c_get_size_in_bytes(type)
     c_tree_node *type;
{
  int code = CTreeTypeCode(type);
  c_tree_node *t;

  if ( CTreeNodeFirstCode(type) == C_ERROR_NODE)
  {
    return c_tree_get_size_int (1);
  }

  if (code == C_FUNCTION_TYPE)
    return c_tree_get_size_int (1);
  if (code == C_VOID_TYPE)
    return c_tree_get_size_int (1);

  if (CTreeTypeSize(type) == 0)
    {
      error("arithmetic on pointer to an incomplete type");
      return c_tree_get_size_int(1);
    }

  /* Convert in case a char is more than one unit.  */
  t = c_build_size_binop(C_CEIL_DIV_EXPR, CTreeTypeSize(type), 
                     c_tree_get_size_int(C_BITS_PER_UNIT));
  c_force_fit_type(t, 0);
  return t;
}


/* Convert the argument expressions in the list VALUES
   to the types in the list TYPELIST.  The result is a list of converted
   argument expressions.

   If TYPELIST is exhausted, or when an element has NULL as its type,
   perform the default conversions.

   PARMLIST is the chain of parm decls for the function being called.
   It may be 0, if that info is not available.
   It is used only for generating error messages.

   NAME is an IDENTIFIER_NODE or 0.  It is used only for error messages.

   This is also where warnings about wrong number of args are generated.

   Both VALUES and the returned value are chains of TREE_LIST nodes
   with the elements of the list in the TREE_VALUE slots of those nodes.  */

static c_tree_node *c_convert_arguments(typelist, values, name, fundecl)
     c_tree_node *typelist;
     c_tree_node *values;
     c_tree_node *name;
     c_tree_node *fundecl;
{
  c_tree_node *typetail;
  c_tree_node *valtail;
  c_tree_node *result = NULL;
  int parmnum;

  /* Scan the given expressions and types, producing individual
     converted arguments and pushing them on RESULT in reverse order.  */

  for (valtail = values, typetail = typelist, parmnum = 0;
       valtail;
       valtail = CTreeChain(valtail), parmnum++)
    {
      c_tree_node *type = typetail ? CTreeListValue(typetail) : 0;
      c_tree_node *val = CTreeListValue(valtail);

      if (type == c_void_type_node)
        {
          if (name)
          {
            error ("too many arguments to function `%s'",
                    CTreeIdentPointer(name));
          }
          else
          {
            error ("too many arguments to function");
          }
          break;
        }

      /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */
      /* Do not use STRIP_NOPS here!  We do not want an enumerator with value 0
         to convert automatically to a pointer.  */
      if ( CTreeExprCode(val) == C_NON_LVALUE_EXPR)
      {
        val = CTreeExprOperand(val, 0);
      }

      if ( CTreeTypeCode(CTreeType(val)) == C_ARRAY_TYPE
          || CTreeTypeCode(CTreeType(val)) == C_FUNCTION_TYPE)
      {
        val = c_default_conversion(val);
      }

      val = c_require_complete_type(val);

      if (type != 0)
        {
          /* Formal parm type is specified by a function prototype.  */
          c_tree_node *parmval;

          if (CTreeTypeSize(type) == 0)
            {
              error ("type of formal parameter %d is incomplete", parmnum + 1);
              parmval = val;
            }
          else
            {
              /* Optionally warn about conversions that
                 differ from the default conversions.  */
              if (c_warn_conversion)
                {
                  int formal_prec = CTreeTypePrecision(type);

                  if (INTEGRAL_TYPE_P (type)
                      && CTreeTypeCode(CTreeType(val)) == C_REAL_TYPE)
                  {
                    warn_for_assignment(
                        "%s as integer rather than floating due to prototype",
                        (char *) 0, name, parmnum + 1);
                  }
                  else if ( CTreeTypeCode(type) == C_COMPLEX_TYPE
                           && CTreeTypeCode(CTreeType(val)) == C_REAL_TYPE)
                  {
                    warn_for_assignment(
                        "%s as complex rather than floating due to prototype",
                        (char *) 0, name, parmnum + 1);
                  }
                  else if (CTreeTypeCode(type) == C_REAL_TYPE
                           && INTEGRAL_TYPE_P (CTreeType(val)))
                  {
                    warn_for_assignment(
                        "%s as floating rather than integer due to prototype",
                        (char *) 0, name, parmnum + 1);
                  }
                  else if (CTreeTypeCode(type) == C_REAL_TYPE
                           && CTreeTypeCode(CTreeType(val)) == C_COMPLEX_TYPE)
                  {
                    warn_for_assignment (
                        "%s as floating rather than complex due to prototype",
                        (char *) 0, name, parmnum + 1);
                  }
                  /* ??? At some point, messages should be written about
                     conversions between complex types, but that's too messy
                     to do now.  */
                  else if (CTreeTypeCode(type) == C_REAL_TYPE
                           && CTreeTypeCode(CTreeType(val)) == C_REAL_TYPE)
                    {
                      /* Warn if any argument is passed as `float',
                         since without a prototype it would be `double'.  */
                      if (formal_prec == CTreeTypePrecision(c_float_type_node))
                      {
                        warn_for_assignment (
                            "%s as `float' rather than `double' due to prototype",
                            (char *) 0, name, parmnum + 1);
                      }
                    }
                  /* Detect integer changing in width or signedness.  */
                  else if (INTEGRAL_TYPE_P (type)
                           && INTEGRAL_TYPE_P (CTreeType(val)))
                    {
                      c_tree_node *would_have_been = c_default_conversion (val);
                      c_tree_node *type1 = CTreeType(would_have_been);

                      if (CTreeTypeCode(type) == C_ENUMERAL_TYPE
                          && type == CTreeType(val))
                        /* No warning if function asks for enum
                           and the actual arg is that enum type.  */
                        ;
                      else if (formal_prec != CTreeTypePrecision(type1))
                      {
                        warn_for_assignment (
                            "%s with different width due to prototype",
                            (char *) 0, name, parmnum + 1);
                      }
                      else if ( CIsTreeNodeUnsigned(type) == CIsTreeNodeUnsigned(type1))
                        ;
                      /* Don't complain if the formal parameter type
                         is an enum, because we can't tell now whether
                         the value was an enum--even the same enum.  */
                      else if (CTreeTypeCode(type) == C_ENUMERAL_TYPE)
                        ;
                      else if ( CTreeNodeFirstCode(val) == C_INT_CST_NODE
                               && c_int_fits_type_p(val, type))
                        /* Change in signedness doesn't matter
                           if a constant value is unaffected.  */
                        ;
                      /* Likewise for a constant in a NOP_EXPR.  */
                      else if ( CTreeNodeFirstCode(val) == C_EXPR_NODE &&
                                CTreeExprCode(val) == C_NOP_EXPR 
                               && CTreeNodeFirstCode(CTreeExprOperand (val, 0)) == C_INT_CST_NODE
                               && c_int_fits_type_p(CTreeExprOperand (val, 0), type))
                        ;
                      /* If the value is extended from a narrower
                         unsigned type, it doesn't matter whether we
                         pass it as signed or unsigned; the value
                         certainly is the same either way.  */
                      else if (CTreeTypePrecision(CTreeType(val)) < CTreeTypePrecision(type)
                               && CIsTreeNodeUnsigned(CTreeType(val)))
                        ;
                      else if ( CIsTreeNodeUnsigned(type))
                      {
                        warn_for_assignment (
                            "%s as unsigned due to prototype",
                            (char *) 0, name, parmnum + 1);
                      }
                      else
                      {
                        warn_for_assignment (
                            "%s as signed due to prototype",
                            (char *) 0, name, parmnum + 1);
                      }
                    }
                }

              parmval = c_convert_for_assignment(type, val, 
                                                (char *) 0, /* arg passing  */
                                                fundecl, name, parmnum + 1);
              
#ifdef PROMOTE_PROTOTYPES
              if ((TREE_CODE (type) == INTEGER_TYPE
                   || TREE_CODE (type) == ENUMERAL_TYPE)
                  && (CTreeTypePrecision(type) < CTreeTypePrecision(integer_type_node)))
                parmval = default_conversion (parmval);
#endif
            }
          result = c_tree_add_head_list( NULL, parmval, result);
        }
      else if ( CTreeTypeCode(CTreeType(val)) == C_REAL_TYPE
               && ( CTreeTypePrecision(CTreeType(val))
                   < CTreeTypePrecision(c_double_type_node)))
      {
        /* Convert `float' to `double'.  */
        result =  c_tree_add_head_list( NULL, 
             c_convert_expr(c_double_type_node, val), result);
      }
      else
      {
        /* Convert `short' and `char' to full-size `int'.  */
        result = c_tree_add_head_list( NULL, c_default_conversion(val), result);
      }

      if (typetail)
      {
        typetail = CTreeChain(typetail);
      }
    }

  if (typetail != 0 &&  CTreeListValue(typetail) != c_void_type_node)
    {
      if (name)
      {
        error ("too few arguments to function `%s'", CTreeIdentPointer(name));
      }
      else
      {
        error ("too few arguments to function");
      }
    }

  return c_tree_reverse_list(result);
}


/* Build a function call to function FUNCTION with parameters PARAMS.
   PARAMS is a list--a chain of TREE_LIST nodes--in which the
   TREE_VALUE of each node is a parameter-expression.
   FUNCTION's data type may be a function type or a pointer-to-function.  */

c_tree_node *c_parser_build_function_call(function, params)
     c_tree_node *function;
     c_tree_node *params;
{
  c_tree_node *fntype  = NULL;
  c_tree_node *fundecl = NULL;
  c_tree_node *coerced_params;
  c_tree_node *name = NULL;
# ifdef LUDO_NO_SKIP
  c_tree_node *assembler_name = NULL;
# endif

  /* Strip NON_LVALUE_EXPRs, etc., since we aren't using as an lvalue.  */
  CStripTypeNops(function);

  /* Convert anything with function type to a pointer-to-function.  */
  if ( CTreeDeclCode(function) == C_FUNCTION_DECL)
  {
      name = CTreeDeclName(function);
# ifdef LUDO_NO_SKIP
      assembler_name = DECL_ASSEMBLER_NAME (function);
# endif

      /* Differs from default_conversion by not setting TREE_ADDRESSABLE
         (because calling an inline function does not mean the function
         needs to be separately compiled).  */
      fntype = c_tree_build_type_variant(CTreeType(function),
                                   CIsTreeNodeReadOnly(function),
                                   CIsTreeNodeVolatile(function));
      fundecl = function;
      function = c_tree_build_unary_expr( C_ADDR_EXPR, function);
      CTreeType( function ) = c_tree_build_pointer_type(fntype);
  }
  else
  {
    function = c_default_conversion(function);
  }

  fntype = CTreeType(function);

  if ( CTreeNodeFirstCode(fntype) == C_ERROR_NODE)
  {
    return c_error_mark_node;
  }

  if (!( CTreeTypeCode(fntype) == C_POINTER_TYPE
        && CTreeTypeCode(CTreeType(fntype)) == C_FUNCTION_TYPE))
  {
    error ("called object is not a function");
    return c_error_mark_node;
  }

  /* fntype now gets the type of function pointed to.  */
  fntype = CTreeType(fntype);

  /* Convert the parameters to the types declared in the
     function prototype, or apply default promotions.  */

  coerced_params
    = c_convert_arguments( CTreeTypeValues(fntype), params, name, fundecl);

  /* Check for errors in format strings.  */

# ifdef LUDO_NO_SKIP
  if (c_warn_format && (name || assembler_name))
  {
    c_check_function_format(name, assembler_name, coerced_params);
  }
# endif

  /* Recognize certain built-in functions so we can make tree-codes
     other than CALL_EXPR.  We do this when it enables fold-const.c
     to do something useful.  */

# ifdef LUDO_NO_SKIP
  if (   CTreeNodeFirstCode( function ) == C_EXPR_NODE
      && CTreeExprCode(function) == C_ADDR_EXPR
      && CTreeDeclCode(CTreeExprOperand(function, 0)) == C_FUNCTION_DECL
      && CIsTreeDeclBuiltIn( CTreeExprOperand(function, 0)))
  {
    switch (DECL_FUNCTION_CODE ( CTreeExprOperand(function, 0)))
      {
      case BUILT_IN_ABS:
      case BUILT_IN_LABS:
      case BUILT_IN_FABS:
        if (coerced_params == 0)
          return integer_zero_node;
        return build_unary_op (ABS_EXPR, TREE_VALUE (coerced_params), 0);
      default:
        break;
      }

  }
# endif
  {
    c_tree_node *result
      = c_tree_build_binary_expr( C_CALL_EXPR, function, coerced_params );

    CTreeType( result ) = CTreeType(fntype),

    CSetTreeNodeSideEffect( result);

    if (CTreeType(result) == c_void_type_node)
    {
      return result;
    }
    return c_require_complete_type(result);
  }
}

/* Print an error message for invalid use of an incomplete type.
   VALUE is the expression that was used (or 0 if that isn't known)
   and TYPE is the type that was invalid.  */

void c_incomplete_type_error(value, type)
     c_tree_node *value;
     c_tree_node *type;
{
  char *errmsg;

  /* Avoid duplicate error message.  */
  if ( CTreeNodeFirstCode(type) == C_ERROR_NODE)
    return;

  if (value != 0 && ( CTreeDeclCode(value) == C_VAR_DECL
                     || CTreeDeclCode(value) == C_PARAM_DECL))
  {
    error ("`%s' has an incomplete type", CTreeIdentPointer(CTreeDeclName(value)));
  }
  else
    {
    retry:
      /* We must print an error message.  Be clever about what it says.  */

      switch ( CTreeTypeCode(type))
        {
        case C_RECORD_TYPE:
          errmsg = "invalid use of undefined type `struct %s'";
          break;

        case C_UNION_TYPE:
          errmsg = "invalid use of undefined type `union %s'";
          break;

        case C_ENUMERAL_TYPE:
          errmsg = "invalid use of undefined type `enum %s'";
          break;

        case C_VOID_TYPE:
          error ("invalid use of void expression");
          return;

        case C_ARRAY_TYPE:
          if ( CTreeTypeValues(type))
            {
              type = CTreeType(type);
              goto retry;
            }
          error ("invalid use of array with unspecified bounds");
          return;

        default:
          abort ();
        }

      if ( CTreeNodeFirstCode( CTreeTypeName(type)) == C_IDENTIFIER_NODE)
      {
        error (errmsg, CTreeIdentPointer( CTreeTypeName(type)));
      }
      else
      {
        /* If this type has a typedef-name, the TYPE_NAME is a TYPE_DECL.  */
        error ("invalid use of incomplete typedef `%s'",
               CTreeIdentPointer(CTreeDeclName( CTreeTypeName(type))));
      }
    }
}

/* Do `exp = require_complete_type (exp);' to make sure exp
   does not have an incomplete type.  (That includes void types.)  */

c_tree_node *c_require_complete_type(value)
     c_tree_node *value;
{
  c_tree_node *type = CTreeType(value);

  /* First, detect a valid value with a complete type.  */
  if (CTreeTypeSize(type) != 0
      && type != c_void_type_node)
    return value;

  c_incomplete_type_error(value, type);
  return c_error_mark_node;
}


/* Return nonzero if REF is an lvalue valid for this language.
   Lvalues can be assigned, unless their type has TYPE_READONLY.
   Lvalues can have their address taken, unless they have DECL_REGISTER.  */

int c_is_lvalue_p(ref)
     c_tree_node *ref;
{
  int code;

  if ( CTreeNodeFirstCode(ref) == C_STRING_NODE)
  {
    return 1;
  }

  if ( CTreeNodeFirstCode(ref) == C_ERROR_NODE)
  {
    return ( CTreeTypeCode(CTreeType(ref)) != C_FUNCTION_TYPE
           &&  CTreeTypeCode(CTreeType(ref)) != C_METHOD_TYPE);
  }

  if ( CTreeNodeFirstCode(ref) == C_DECL_NODE)
  {
    code = CTreeDeclCode(ref);

    if (code == C_VAR_DECL || 
        code == C_PARAM_DECL ||
        code == C_RESULT_DECL )
    {
      return ( CTreeTypeCode(CTreeType(ref)) != C_FUNCTION_TYPE
              &&  CTreeTypeCode(CTreeType(ref)) != C_METHOD_TYPE);
    }

    return 0;
  }

  if ( CTreeNodeFirstCode(ref) == C_EXPR_NODE)
  {
    code =  CTreeExprCode(ref);

    switch (code)
    {
    case C_REALPART_EXPR:
    case C_IMAGPART_EXPR:
    case C_COMPONENT_REF:
      return c_is_lvalue_p(CTreeExprOperand(ref, 0));

    case C_INDIRECT_REF:
    case C_ARRAY_REF:
      return ( CTreeTypeCode(CTreeType(ref)) != C_FUNCTION_TYPE
              &&  CTreeTypeCode(CTreeType(ref)) != C_METHOD_TYPE);

    case C_BIND_EXPR:
    case C_RTL_EXPR:
      return  CTreeTypeCode(CTreeType(ref)) == C_ARRAY_TYPE;

    }
  }

  return 0;
}

/* This handles expressions of the form "a[i]", which denotes
   an array reference.

   This is logically equivalent in C to *(a+i), but we may do it differently.
   If A is a variable or a member, we generate a primitive ARRAY_REF.
   This avoids forcing the array out of registers, and can work on
   arrays that are not lvalues (for example, members of structures returned
   by functions).  */

c_tree_node *c_parser_build_array_ref(array, index)

     c_tree_node *array;
     c_tree_node *index;
{
  if (index == 0)
    {
      error ("subscript missing in array reference");
      return c_error_mark_node;
    }

  if (CTreeType(array) == c_error_mark_node
      || CTreeType(index) == c_error_mark_node)
    return c_error_mark_node;

  if ( CTreeTypeCode(CTreeType(array)) == C_ARRAY_TYPE
      && CTreeExprCode(array) != C_INDIRECT_REF)
    {
      c_tree_node *rval;
      c_tree_node *type;

      /* Subscripting with type char is likely to lose
         on a machine where chars are signed.
         So warn on any machine, but optionally.
         Don't warn for unsigned char since that type is safe.
         Don't warn for signed char because anyone who uses that
         must have done so deliberately.  */
      if (c_warn_char_subscripts
          && CTreeTypeMainVariant(CTreeType(index)) == c_char_type_node)
      {
        warning ("array subscript has type `char'");
      }

      /* Apply default promotions *after* noticing character types.  */
      index = c_default_conversion(index);

      /* Require integer *after* promotion, for sake of enums.  */
      if ( CTreeTypeCode(CTreeType(index)) != C_INTEGER_TYPE)
        {
          error ("array subscript is not an integer");
          return c_error_mark_node;
        }

      /* An array that is indexed by a non-constant
         cannot be stored in a register; we must be able to do
         address arithmetic on its address.
         Likewise an array of elements of variable size.  */
      if ( CTreeNodeFirstCode(index) != C_INT_CST_NODE
          || (CTreeTypeSize(CTreeType(CTreeType(array))) != 0
              && 
          CTreeNodeFirstCode(CTreeTypeSize(CTreeType(CTreeType(array)))) != C_INT_CST_NODE))
        {
          if ( c_mark_addressable (array) == 0)
            return c_error_mark_node;
        }
      /* An array that is indexed by a constant value which is not within
         the array bounds cannot be stored in a register either; because we
         would get a crash in store_bit_field/extract_bit_field when trying
         to access a non-existent part of the register.  */
      if ( CTreeNodeFirstCode(index) == C_INT_CST_NODE
          &&  CTreeTypeValues(CTreeType(array))
          && ! c_int_fits_type_p(index,  CTreeTypeValues(CTreeType(array))))
        {
          if (c_mark_addressable (array) == 0)
            return c_error_mark_node;
        }

      if (c_pedantic && ! c_is_lvalue_p(array))
        {
          if ( CIsTreeDeclRegister(array))
          {
            pedwarn ("ANSI C forbids subscripting `register' array");
          }
          else
          {
            pedwarn ("ANSI C forbids subscripting non-lvalue array");
          }
        }

      if (c_pedantic)
        {
          c_tree_node *foo = array;

          while ( CTreeNodeFirstCode(foo) == C_EXPR_NODE &&
                  CTreeExprCode(foo) == C_COMPONENT_REF )
          {
            foo = CTreeExprOperand(foo, 0);
          }

          if ( CTreeNodeFirstCode(foo) == C_DECL_NODE && CTreeDeclCode(foo) == C_VAR_DECL && 
                CIsTreeDeclRegister(foo))
          {
            pedwarn ("ANSI C forbids subscripting non-lvalue array");
          }
        }

      type = CTreeTypeMainVariant(CTreeType(CTreeType(array)));
      rval = c_tree_build_binary_expr(C_ARRAY_REF, array, index);
      CTreeType( rval ) = type;

      /* Array ref is const/volatile if the array elements are
         or if the array is.  */
      if ( CIsTreeNodeReadOnly(CTreeType(CTreeType(array)))
            | CIsTreeNodeReadOnly(array))
      {
        CSetTreeNodeReadOnly(rval);
      }

      if (CIsTreeNodeVolatile(CTreeType(CTreeType(array)))
            | CIsTreeNodeSideEffect(array))
      {
        CSetTreeNodeSideEffect(rval);
      }

      if ( CIsTreeNodeVolatile(CTreeType(CTreeType(array)))
            /* This was added by rms on 16 Nov 91.
               It fixes  vol struct foo *a;  a->elts[1] 
               in an inline function.
               Hope it doesn't break something else.  */
            | CIsTreeNodeVolatile(array))
      {
        CSetTreeNodeVolatile(rval);
      }

      return c_require_complete_type( c_fold(rval));
    }

  {
    c_tree_node *ar = c_default_conversion(array);
    c_tree_node *ind = c_default_conversion(index);

    /* Do the same warning check as above, but only on the part that's
       syntactically the index and only if it is also semantically
       the index.  */
    if (c_warn_char_subscripts
        && CTreeTypeCode(CTreeType(index)) == C_INTEGER_TYPE
        && CTreeTypeMainVariant(CTreeType(index)) == c_char_type_node)
    {
      warning ("subscript has type `char'");
    }

    /* Put the integer in IND to simplify error checking.  */
    if ( CTreeTypeCode(CTreeType(ar)) == C_INTEGER_TYPE)
      {
        c_tree_node *temp = ar;
        ar = ind;
        ind = temp;
      }

    if (ar == c_error_mark_node)
      return ar;

    if (CTreeTypeCode(CTreeType(ar)) != C_POINTER_TYPE
        || CTreeTypeCode(CTreeType(CTreeType(ar))) == C_FUNCTION_TYPE)
      {
        error ("subscripted value is neither array nor pointer");
        return c_error_mark_node;
      }
    if ( CTreeTypeCode(CTreeType(ind)) != C_INTEGER_TYPE)
      {
        error ("array subscript is not an integer");
        return c_error_mark_node;
      }

    return c_parser_build_indirect_ref(
           c_build_binary_op( C_PLUS_EXPR, ar, ind, 0), "array indexing");
  }
}


/* Look up component name in the structure type definition.

   If this component name is found indirectly within an anonymous union,
   store in *INDIRECT the component which directly contains
   that anonymous union.  Otherwise, set *INDIRECT to 0.  */
     
static c_tree_node *c_lookup_field(type, component, indirect)
     c_tree_node *type;
     c_tree_node *component;
     c_tree_node **indirect;
{
  c_tree_node *field;

  /* If TYPE_LANG_SPECIFIC is set, then it is a sorted array of pointers
     to the field elements.  Use a binary search on this array to quickly
     find the element.  Otherwise, do a linear search.  TYPE_LANG_SPECIFIC
     will always be set for structures which have many elements.  */

# ifdef LUDO_NO_SKIP
  if (TYPE_LANG_SPECIFIC (type))
    {
      int bot, top, half;
      c_tree_node **field_array = &TYPE_LANG_SPECIFIC (type)->elts[0];

      field = TYPE_FIELDS (type);
      bot = 0;
      top = TYPE_LANG_SPECIFIC (type)->len;
      while (top - bot > 1)
        {
          half = (top - bot + 1) >> 1;
          field = field_array[bot+half];

          if (CTreeDeclName(field) == NULL)
            {
              /* Step through all anon unions in linear fashion.  */
              while (CTreeDeclName(field_array[bot]) == NULL)
                {
                  c_tree_node *anon = 0, junk;

                  field = field_array[bot++];
                  if (TREE_CODE (CTreeType(field)) == RECORD_TYPE
                      || TREE_CODE (CTreeType(field)) == UNION_TYPE)
                    anon = lookup_field (CTreeType(field), component, &junk);

                  if (anon != NULL)
                    {
                      *indirect = field;
                      return anon;
                    }
                }

              /* Entire record is only anon unions.  */
              if (bot > top)
                return NULL;

              /* Restart the binary search, with new lower bound.  */
              continue;
            }

          if (CTreeDeclName(field) == component)
            break;
          if (CTreeDeclName(field) < component)
            bot += half;
          else
            top = bot + half;
        }

      if (CTreeDeclName(field_array[bot]) == component)
        field = field_array[bot];
      else if (CTreeDeclName(field) != component)
        field = 0;
    }
  else
# endif
    {
      for (field = CTreeTypeValues(type); field; field = CTreeChain(field))
        {
          if (CTreeDeclName(field) == NULL)
            {
              c_tree_node *junk;
              c_tree_node *anon = 0;

              if ( CTreeTypeCode(CTreeType(field)) == C_RECORD_TYPE
                  || CTreeTypeCode(CTreeType(field)) == C_UNION_TYPE)
              {
                anon = c_lookup_field(CTreeType(field), component, &junk);
              }

              if (anon != NULL)
                {
                  *indirect = field;
                  return anon;
                }
            }

          if (CTreeDeclName(field) == component)
            break;
        }
    }

  *indirect = NULL;
  return field;
}


/* Make an expression to refer to the COMPONENT field of
   structure or union value DATUM.  COMPONENT is an IDENTIFIER_NODE.  */

c_tree_node *c_parser_build_component_ref(datum, component)
     c_tree_node *datum;
     c_tree_node *component;
{
  c_tree_node *type = CTreeType(datum);
  int code = CTreeTypeCode(type);
  c_tree_node *field = NULL;
  c_tree_node *ref;
  c_tree_node *tmp_node;

  /* If DATUM is a COMPOUND_EXPR or COND_EXPR, move our reference inside it
     unless we are not to support things not strictly ANSI.  */
  switch ( CTreeExprCode(datum))
    {
    case C_COMPOUND_EXPR:
      {

        c_tree_node *value = c_parser_build_component_ref(
            CTreeExprOperand(datum, 1), component);

        tmp_node = c_tree_build_binary_expr( C_COMPOUND_EXPR, 
                      CTreeExprOperand(datum, 0), value);
        CTreeType( tmp_node ) = CTreeType( value );
      }
    case C_COND_EXPR:
      return 
        c_parser_build_conditional_expr(
         CTreeExprOperand(datum, 0),
         c_parser_build_component_ref(CTreeExprOperand (datum, 1), component),
         c_parser_build_component_ref(CTreeExprOperand (datum, 2), component));

    default:
      break;
    }

  /* See if there is a field or component with name COMPONENT.  */

  if (code == C_RECORD_TYPE || code == C_UNION_TYPE)
    {
      c_tree_node *indirect = 0;

      if (CTreeTypeSize(type) == 0)
        {
          c_incomplete_type_error(NULL, type);
          return c_error_mark_node;
        }

      field = c_lookup_field(type, component, &indirect);

      if (!field)
        {
          error (code == C_RECORD_TYPE
                 ? "structure has no member named `%s'"
                 : "union has no member named `%s'",
                 CTreeIdentPointer(component));
          return c_error_mark_node;
        }
      if (CTreeType(field) == c_error_mark_node)
      {
        return c_error_mark_node;
      }

      /* If FIELD was found buried within an anonymous union,
         make one COMPONENT_REF to get that anonymous union,
         then fall thru to make a second COMPONENT_REF to get FIELD.  */
      if (indirect != 0)
        {
          ref = c_tree_build_binary_expr( C_COMPONENT_REF,datum, indirect );
          CTreeType( ref ) = CTreeType(indirect);
                  
          if ( CIsTreeNodeReadOnly(datum) ||  CIsTreeNodeReadOnly(indirect))
          {
            CSetTreeNodeReadOnly(ref);
          }

          if ( CIsTreeNodeVolatile(datum) || CIsTreeNodeVolatile(indirect))
          {
            CSetTreeNodeVolatile(ref);
          }
          datum = ref;
        }

      ref = c_tree_build_binary_expr( C_COMPONENT_REF, datum, field);
      CTreeType( ref ) = CTreeType(field);

      if ( CIsTreeNodeReadOnly(datum) || CIsTreeNodeReadOnly(field))
      {
        CSetTreeNodeReadOnly(ref);
      }

      if ( CIsTreeNodeVolatile(datum) || CIsTreeNodeVolatile(field))
      {
         CSetTreeNodeVolatile(ref);
      }

      return ref;
    }
  else if ( CTreeNodeFirstCode(type) != C_ERROR_NODE)
  {
    error ("request for member `%s' in something not a structure or union",
            CTreeIdentPointer(component));
  }

  return c_error_mark_node;
}


/* Prepare to parse and output the initializer for variable DECL.  */

void c_start_init(decl, asmspec_tree, top_level)
     c_tree_node *decl;
     c_tree_node *asmspec_tree;
     int top_level;
{
  char *locus;
  struct initializer_stack *p
    = (struct initializer_stack *) autallocblock (sizeof (struct initializer_stack));
  char *asmspec = 0;

  if (asmspec_tree)
    asmspec = CTreeStringPointer(asmspec_tree);

  p->decl = c_constructor_decl;
  p->asmspec = c_constructor_asmspec;
  p->incremental = c_constructor_incremental;
  p->require_constant_value = c_require_constant_value;
  p->require_constant_elements = c_require_constant_elements;
  p->constructor_stack = c_constructor_stack;
  p->elements = c_constructor_elements;
  p->spelling = c_spelling;
  p->spelling_base = c_spelling_base;
  p->spelling_size = c_spelling_size;
  p->deferred = c_constructor_subconstants_deferred;
  p->top_level = c_constructor_top_level;
  p->next = c_initializer_stack;
  c_initializer_stack = p;

  c_constructor_decl = decl;
  c_constructor_incremental = top_level;
  c_constructor_asmspec = asmspec;
  c_constructor_subconstants_deferred = 0;
  c_constructor_top_level = top_level;

  if (decl != 0)
    {
      c_require_constant_value = CIsTreeNodeConstant(decl);
      c_require_constant_elements
        = ((CIsTreeNodeStatic(decl) || c_pedantic)
           /* For a scalar, you can always use any value to initialize,
              even within braces.  */
           && ( CTreeTypeCode(CTreeType(decl)) == C_ARRAY_TYPE
               || CTreeTypeCode(CTreeType(decl)) == C_RECORD_TYPE
               || CTreeTypeCode(CTreeType(decl)) == C_UNION_TYPE
               || CTreeTypeCode(CTreeType(decl)) == C_QUAL_UNION_TYPE));
      locus = CTreeIdentPointer(CTreeDeclName(decl));
      c_constructor_incremental |= CIsTreeNodeStatic(decl);
    }
  else
    {
      c_require_constant_value = 0;
      c_require_constant_elements = 0;
      locus = "(anonymous)";
    }

  c_constructor_stack = 0;

  c_missing_braces_mentioned = 0;

  c_spelling_base = 0;
  c_spelling_size = 0;
  RESTORE_SPELLING_DEPTH (0);

  if (locus)
    c_push_string(locus);
}

c_tree_node *c_finish_init()
{
  struct initializer_stack *p = c_initializer_stack;
  c_tree_node *tmp_node;

  /* Output subconstants (string constants, usually)
     that were referenced within this initializer and saved up.
     Must do this if and only if we called defer_addressed_constants.  */
# ifdef LUDO_NO_SKIP
  if (c_constructor_subconstants_deferred)
    c_output_deferred_addressed_constants();
# endif

  /* Free the whole constructor stack of this initializer.  */
  while (c_constructor_stack)
    {
      struct constructor_stack *q = c_constructor_stack;
      c_constructor_stack = q->next;
      autfreeblock (q);
    }

  /* Pop back to the data of the outer initializer (if any).  */
  c_constructor_decl = p->decl;
  c_constructor_asmspec = p->asmspec;
  c_constructor_incremental = p->incremental;
  c_require_constant_value = p->require_constant_value;
  c_require_constant_elements = p->require_constant_elements;
  c_constructor_stack = p->constructor_stack;
  c_constructor_elements = p->elements;
  c_spelling = p->spelling;
  c_spelling_base = p->spelling_base;
  c_spelling_size = p->spelling_size;
  c_constructor_subconstants_deferred = p->deferred;
  c_constructor_top_level = p->top_level;
  c_initializer_stack = p->next;
  autfreeblock (p);

  tmp_node = c_tree_reverse_list( c_output_constant_list );
  c_output_constant_list = (c_tree_node *)0;

  return( tmp_node );
}


/* Don't read a struct incrementally if it has any bitfields,
   because the incremental reading code doesn't know how to
   handle bitfields yet.  */

static void c_check_init_type_bitfields (type)
     c_tree_node *type;
{
  if ( CTreeTypeCode(type) == C_RECORD_TYPE)
    {
      c_tree_node *tail;
      for (tail = CTreeTypeValues(type); tail;
           tail = CTreeChain(tail))
        {
          if ( CIsTreeDeclCBitField(tail)
              /* This catches cases like `int foo : 8;'.  */
              || CTreeDeclMode(tail) != CTreeTypeMode(CTreeType(tail)))
            {
              c_constructor_incremental = 0;
              break;
            }

          c_check_init_type_bitfields (CTreeType(tail));
        }
    }

  else if ( CTreeTypeCode(type) == C_ARRAY_TYPE)
  {
    c_check_init_type_bitfields (CTreeType(type));
  }
}

/* Call here when we see the initializer is surrounded by braces.
   This is instead of a call to push_init_level;
   it is matched by a call to pop_init_level.

   TYPE is the type to initialize, for a constructor expression.
   For an initializer for a decl, TYPE is zero.  */

void
c_really_start_incremental_init (type)
     c_tree_node *type;
{
  struct constructor_stack *p
    = (struct constructor_stack *)autallocblock(sizeof (struct constructor_stack));

  if (type == 0)
    type = CTreeType(c_constructor_decl);

  /* Turn off constructor_incremental if type is a struct with bitfields.
     Do this before the first push, so that the corrected value
     is available in c_finish_init.  */
  c_check_init_type_bitfields (type);

  p->type = c_constructor_type;
  p->fields = c_constructor_fields;
  p->index = c_constructor_index;
  p->range_end = c_constructor_range_end;
  p->max_index = c_constructor_max_index;
  p->unfilled_index = c_constructor_unfilled_index;
  p->unfilled_fields = c_constructor_unfilled_fields;
  p->bit_index = c_constructor_bit_index;
  p->elements = c_constructor_elements;
  p->constant = c_constructor_constant;
  p->simple = c_constructor_simple;
  p->erroneous = c_constructor_erroneous;
  p->pending_elts = c_constructor_pending_elts;
  p->depth = c_constructor_depth;
  p->replacement_value = 0;
  p->implicit = 0;
  p->incremental = c_constructor_incremental;
  p->outer = 0;
  p->next = 0;
  c_constructor_stack = p;

  c_constructor_constant = 1;
  c_constructor_simple = 1;
  c_constructor_depth = SPELLING_DEPTH ();
  c_constructor_elements = 0;
  c_constructor_pending_elts = 0;
  c_constructor_type = type;

  if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
      || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
    {
      c_constructor_fields = CTreeTypeValues(c_constructor_type);
      /* Skip any nameless bit fields at the beginning.  */
      while (c_constructor_fields != 0 && CIsTreeDeclCBitField(c_constructor_fields)
             && CTreeDeclName(c_constructor_fields) == 0)
        c_constructor_fields = CTreeChain(c_constructor_fields);
      c_constructor_unfilled_fields = c_constructor_fields;
      c_constructor_bit_index = c_tree_copy_node(c_integer_zero_node);
    }
  else if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
    {
      c_constructor_range_end = 0;
      if ( CTreeTypeValues(c_constructor_type))
        {
          c_constructor_max_index
            = CTreeTypeMaxValue( CTreeTypeValues(c_constructor_type));
          c_constructor_index
            = c_tree_copy_node( CTreeTypeMinValue(CTreeTypeValues(c_constructor_type)));
        }
      else
      {
        c_constructor_index = c_tree_copy_node(c_integer_zero_node);
      }
      c_constructor_unfilled_index = c_tree_copy_node(c_constructor_index);
    }
  else
    {
      /* Handle the case of int x = {5}; */
      c_constructor_fields = c_constructor_type;
      c_constructor_unfilled_fields = c_constructor_type;
    }

  if (c_constructor_incremental)
    {
# ifdef LUDO_NO_SKIP
      int momentary = suspend_momentary ();
      push_obstacks_nochange ();
      if ( CIsTreeNodePermanent(c_constructor_decl))
        end_temporary_allocation ();
      make_decl_rtl (constructor_decl, constructor_asmspec,
                     constructor_top_level);
      assemble_variable(c_constructor_decl, c_constructor_top_level, 0, 1);
      pop_obstacks ();
      resume_momentary (momentary);
# endif
    }

  if (c_constructor_incremental)
    {
# ifdef LUDO_NO_SKIP
      c_defer_addressed_constants ();
# endif
      c_constructor_subconstants_deferred = 1;
    }
}

static void c_assemble_zeros( Size )

  int Size;
{
  c_tree_node *size_node;

  if ( Size > 0 )
  {
    size_node = c_tree_build_int_2( Size, Size );

    c_output_constant_list = 
      c_tree_add_head_list( size_node, c_integer_zero_node, c_output_constant_list );
  }
}

static void c_output_constant( TreeCst, Size )

  c_tree_node *TreeCst;
  int          Size;
{
  c_output_constant_list =
    c_tree_add_head_list( (c_tree_node *)0, TreeCst, c_output_constant_list );
}

/* Push down into a subobject, for initialization.
   If this is for an explicit set of braces, IMPLICIT is 0.
   If it is because the next element belongs at a lower level,
   IMPLICIT is 1.  */

void
c_push_init_level(implicit)
     int implicit;
{
  struct constructor_stack *p;

  /* If we've exhausted any levels that didn't have braces,
     pop them now.  */
  while (c_constructor_stack->implicit)
    {
      if (( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
           || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
          && c_constructor_fields == 0)
      {
        c_process_init_element(c_pop_init_level(1));
      }
      else if (CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE
               && c_tree_is_int_cst_lt(c_constructor_max_index, c_constructor_index))
     {
        c_process_init_element( c_pop_init_level(1));
     }
      else
        break;
    }

  /* Structure elements may require alignment.  Do this now if necessary
     for the subaggregate, and if it comes next in sequence.  Don't do
     this for subaggregates that will go on the pending list.  */
  if (c_constructor_incremental && c_constructor_type != 0
      && CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE && c_constructor_fields
      && c_constructor_fields == c_constructor_unfilled_fields)
    {
      /* Advance to offset of this element.  */
      if (! c_tree_is_int_cst_equal(c_constructor_bit_index,
                                CTreeDeclArguments(c_constructor_fields)))
        {
          int next = (CTreeIntCstLow(CTreeDeclArguments(c_constructor_fields))
                      / C_BITS_PER_UNIT);
          int here = (CTreeIntCstLow(c_constructor_bit_index)
                      / C_BITS_PER_UNIT);
          c_assemble_zeros(next - here);
        }
      /* Indicate that we have now filled the structure up to the current
         field.  */
      c_constructor_unfilled_fields = c_constructor_fields;
    }

  p = (struct constructor_stack *)autallocblock(sizeof (struct constructor_stack));
  p->type = c_constructor_type;
  p->fields = c_constructor_fields;
  p->index = c_constructor_index;
  p->range_end = c_constructor_range_end;
  p->max_index = c_constructor_max_index;
  p->unfilled_index = c_constructor_unfilled_index;
  p->unfilled_fields = c_constructor_unfilled_fields;
  p->bit_index = c_constructor_bit_index;
  p->elements = c_constructor_elements;
  p->constant = c_constructor_constant;
  p->simple = c_constructor_simple;
  p->erroneous = c_constructor_erroneous;
  p->pending_elts = c_constructor_pending_elts;
  p->depth = c_constructor_depth;
  p->replacement_value = 0;
  p->implicit = implicit;
  p->incremental = c_constructor_incremental;
  p->outer = 0;
  p->next = c_constructor_stack;
  c_constructor_stack = p;

  c_constructor_constant = 1;
  c_constructor_simple = 1;
  c_constructor_depth = SPELLING_DEPTH ();
  c_constructor_elements = 0;
  c_constructor_pending_elts = 0;

  /* Don't die if an entire brace-pair level is superfluous
     in the containing level.  */
  if (c_constructor_type == 0)
    ;
  else if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
           || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
    {
      /* Don't die if there are extra init elts at the end.  */
      if (c_constructor_fields == 0)
        c_constructor_type = 0;
      else
        {
          c_constructor_type = CTreeType(c_constructor_fields);
          c_push_member_name (c_constructor_fields);
          c_constructor_depth++;
          if (c_constructor_fields != c_constructor_unfilled_fields)
            c_constructor_incremental = 0;
        }
    }
  else if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
    {
      c_constructor_type = CTreeType(c_constructor_type);
      c_push_array_bounds (CTreeIntCstLow(c_constructor_index));
      c_constructor_depth++;
      if (! c_tree_is_int_cst_equal(c_constructor_index, c_constructor_unfilled_index)
          || c_constructor_range_end != 0)
        c_constructor_incremental = 0;
    }

  if (c_constructor_type == 0)
    {
      error_init ("extra brace group at end of initializer%s",
                  " for `%s'", NULL);
      c_constructor_fields = 0;
      c_constructor_unfilled_fields = 0;
      return;
    }

  /* Turn off constructor_incremental if type is a struct with bitfields.  */
  c_check_init_type_bitfields(c_constructor_type);

  if (implicit && c_warn_missing_braces && !c_missing_braces_mentioned)
    {
      c_missing_braces_mentioned = 1;
      warning_init ("missing braces around initializer%s", " for `%s'", NULL);
    }

  if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
           || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
    {
      c_constructor_fields = CTreeTypeValues(c_constructor_type);
      /* Skip any nameless bit fields at the beginning.  */
      while (c_constructor_fields != 0 && CIsTreeDeclCBitField(c_constructor_fields)
             && CTreeDeclName(c_constructor_fields) == 0)
        c_constructor_fields = CTreeChain(c_constructor_fields);
      c_constructor_unfilled_fields = c_constructor_fields;
      c_constructor_bit_index = c_tree_copy_node(c_integer_zero_node);
    }
  else if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
    {
      c_constructor_range_end = 0;
      if ( CTreeTypeValues(c_constructor_type))
        {
          c_constructor_max_index
            = CTreeTypeMaxValue( CTreeTypeValues(c_constructor_type));
          c_constructor_index
            = c_tree_copy_node( CTreeTypeMinValue( CTreeTypeValues(c_constructor_type)));
        }
      else
        c_constructor_index = c_tree_copy_node(c_integer_zero_node);
      c_constructor_unfilled_index = c_tree_copy_node(c_constructor_index);
    }
  else
    {
      warning_init ("braces around scalar initializer%s", " for `%s'", NULL);
      c_constructor_fields = c_constructor_type;
      c_constructor_unfilled_fields = c_constructor_type;
    }
}

static void c_output_init_element();

/* Output any pending elements which have become next.
   As we output elements, constructor_unfilled_{fields,index}
   advances, which may cause other elements to become next;
   if so, they too are output.

   If ALL is 0, we return when there are
   no more pending elements to output now.

   If ALL is 1, we output space as necessary so that
   we can output all the pending elements.  */

static void c_output_pending_init_elements (all)
     int all;
{
  c_tree_node *tail;
  c_tree_node *next;

 retry:

  /* Look thru the whole pending list.
     If we find an element that should be output now,
     output it.  Otherwise, set NEXT to the element
     that comes first among those still pending.  */
     
  next = 0;
  for (tail = c_constructor_pending_elts; tail;
       tail = CTreeChain(tail))
    {
      if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          if (c_tree_is_int_cst_equal(CTreeListPurpose(tail),
                                  c_constructor_unfilled_index))
            {
              c_output_init_element(CTreeListValue(tail),
                                   CTreeType(c_constructor_type),
                                   c_constructor_unfilled_index, 0);
              goto retry;
            }
          else if (c_tree_is_int_cst_lt(CTreeListPurpose(tail),
                                    c_constructor_unfilled_index))
            ;
          else if (next == 0
                   || c_tree_is_int_cst_lt(CTreeListPurpose(tail), next))
            next = CTreeListPurpose(tail);
        }
      else if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
               ||  CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        {
          if (CTreeListPurpose(tail) == c_constructor_unfilled_fields)
            {
              c_output_init_element(CTreeListValue(tail),
                                   CTreeType(c_constructor_unfilled_fields),
                                   c_constructor_unfilled_fields,
                                   0);
              goto retry;
            }
          else if (c_constructor_unfilled_fields == 0
                   || c_tree_is_int_cst_lt( CTreeDeclArguments(CTreeListPurpose(tail)),
                                       CTreeDeclArguments(c_constructor_unfilled_fields)))
            ;
          else if (next == 0
                   || c_tree_is_int_cst_lt( CTreeDeclArguments(CTreeListPurpose(tail)),
                                       CTreeDeclArguments(next)))
            next = CTreeListPurpose(tail);
        }
    }

  /* Ordinarily return, but not if we want to output all
     and there are elements left.  */
  if (! (all && next != 0))
    return;

  /* Generate space up to the position of NEXT.  */
  if (c_constructor_incremental)
    {
      c_tree_node *filled;
      c_tree_node *nextpos_tree = c_tree_get_size_int(0);

      if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
          || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        {
          /* Find the last field written out, if any.  */
          for (tail = CTreeTypeValues(c_constructor_type); tail;
               tail = CTreeChain(tail))
            if (CTreeChain(tail) == c_constructor_unfilled_fields)
              break;

          if (tail)
            /* Find the offset of the end of that field.  */
            filled = c_build_size_binop(C_CEIL_DIV_EXPR,
                                 c_build_size_binop(C_PLUS_EXPR,
                                             CTreeDeclArguments(tail),
                                             CTreeDeclSize(tail)),
                                 c_tree_get_size_int(C_BITS_PER_UNIT));
          else
            filled = c_tree_get_size_int(0);

          nextpos_tree = c_build_size_binop(C_CEIL_DIV_EXPR,
                                     CTreeDeclArguments(next),
                                     c_tree_get_size_int(C_BITS_PER_UNIT));

          CTreeIntCstHigh(c_constructor_bit_index)
            = CTreeIntCstHigh(CTreeDeclArguments(next));
          CTreeIntCstLow(c_constructor_bit_index)
            = CTreeIntCstLow(CTreeDeclArguments(next));
          c_constructor_unfilled_fields = next;
        }
      else if (CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          filled = c_build_size_binop(C_MULT_EXPR, c_constructor_unfilled_index,
                               c_get_size_in_bytes(CTreeType(c_constructor_type)));
          nextpos_tree
            = c_build_size_binop(C_MULT_EXPR, next,
                          c_get_size_in_bytes(CTreeType(c_constructor_type)));
          CTreeIntCstLow(c_constructor_unfilled_index)
            = CTreeIntCstLow(next);
          CTreeIntCstHigh(c_constructor_unfilled_index)
            = CTreeIntCstHigh(next);
        }
      else
        filled = 0;

      if (filled)
        {
          int nextpos = CTreeIntCstLow(nextpos_tree);

          c_assemble_zeros(nextpos - CTreeIntCstLow(filled));
        }
    }
  else
    {
      /* If it's not incremental, just skip over the gap,
         so that after jumping to retry we will output the next
         successive element.  */
      if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
          || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        c_constructor_unfilled_fields = next;
      else if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          CTreeIntCstLow(c_constructor_unfilled_index)
            = CTreeIntCstLow(next);
          CTreeIntCstHigh(c_constructor_unfilled_index)
            = CTreeIntCstHigh(next);
        }
    }

  goto retry;
}

/* "Output" the next constructor element.
   At top level, really output it to assembler code now.
   Otherwise, collect it in a list from which we will make a CONSTRUCTOR.
   TYPE is the data type that the containing data type wants here.
   FIELD is the field (a FIELD_DECL) or the index that this element fills.

   PENDING if non-nil means output pending elements that belong
   right after this element.  (PENDING is normally 1;
   it is 0 while outputting pending elements, to avoid recursion.)  */

static void
c_output_init_element (value, type, field, pending)
     c_tree_node *value;
     c_tree_node *type;
     c_tree_node *field;
     int pending;
{
  int duplicate = 0;

  if ( CTreeTypeCode(CTreeType(value)) == C_FUNCTION_TYPE
      || ( CTreeTypeCode(CTreeType(value)) == C_ARRAY_TYPE
          && !( CTreeNodeFirstCode(value) == C_STRING_NODE
               && CTreeTypeCode(type) == C_ARRAY_TYPE
               && CTreeTypeCode(CTreeType(type)) == C_INTEGER_TYPE)
          && !c_is_comptypes(CTreeTypeMainVariant(CTreeType(value)),
                         CTreeTypeMainVariant(type))))
  {
    value = c_default_conversion(value);
  }

  if (value == c_error_mark_node)
    c_constructor_erroneous = 1;
  else if (! CIsTreeNodeConstant(value))
    c_constructor_constant = 0;
  else if (c_is_initializer_constant_valid_p(value, CTreeType(value)) == 0
           || (( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
                || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
               && CIsTreeDeclCBitField(field)
               && CTreeNodeFirstCode(value) != C_INT_CST_NODE))
    c_constructor_simple = 0;

  if (c_require_constant_value && ! CIsTreeNodeConstant(value))
    {
      error_init ("initializer element%s is not constant",
                  " for `%s'", NULL);
      value = c_error_mark_node;
    }
  else if (c_require_constant_elements
           && c_is_initializer_constant_valid_p(value, CTreeType(value)) == 0)
    {
      error_init ("initializer element%s is not computable at load time",
                  " for `%s'", NULL);
      value = c_error_mark_node;
    }

  /* If this element duplicates one on constructor_pending_elts,
     print a message and ignore it.  Don't do this when we're
     processing elements taken off constructor_pending_elts,
     because we'd always get spurious errors.  */
  if (pending)
    {
      if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
          || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        {
          if (c_tree_get_purpose_member(field, c_constructor_pending_elts))
            {
              error_init ("duplicate initializer%s", " for `%s'", NULL);
              duplicate = 1;
            }
        }
      if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          c_tree_node *tail;
          for (tail = c_constructor_pending_elts; tail;
               tail = CTreeChain(tail))
            if (CTreeListPurpose(tail) != 0
                && CTreeNodeFirstCode(CTreeListPurpose(tail)) == C_INT_CST_NODE
                && c_tree_is_int_cst_equal(CTreeListPurpose(tail), c_constructor_index))
              break;

          if (tail != 0)
            {
              error_init ("duplicate initializer%s", " for `%s'", NULL);
              duplicate = 1;
            }
        }
    }

  /* If this element doesn't come next in sequence,
     put it on constructor_pending_elts.  */
  if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE
      && !c_tree_is_int_cst_equal(field, c_constructor_unfilled_index))
    {
      if (! duplicate)
        /* The copy_node is needed in case field is actually
           constructor_index, which is modified in place.  */
        c_constructor_pending_elts
          = c_tree_add_head_list(c_tree_copy_node(field),
                       c_digest_init(type, value, c_require_constant_value, 
                                    c_require_constant_elements),
                       c_constructor_pending_elts);
    }
  else if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
           && field != c_constructor_unfilled_fields)
    {
      /* We do this for records but not for unions.  In a union,
         no matter which field is specified, it can be initialized
         right away since it starts at the beginning of the union.  */
      if (!duplicate)
        c_constructor_pending_elts
          = c_tree_add_head_list(field,
                       c_digest_init(type, value, c_require_constant_value, 
                                    c_require_constant_elements),
                       c_constructor_pending_elts);
    }
  else
    {
      /* Otherwise, output this element either to
         constructor_elements or to the assembler file.  */

      if (!duplicate)
        {
          if (! c_constructor_incremental)
            {
              if (field && CTreeNodeFirstCode(field) == C_INT_CST_NODE)
                field = c_tree_copy_node (field);
              c_constructor_elements
                = c_tree_add_head_list(field, c_digest_init(type, value,
                                                 c_require_constant_value, 
                                                 c_require_constant_elements),
                             c_constructor_elements);
            }
          else
            {
              /* Structure elements may require alignment.
                 Do this, if necessary.  */
              if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE)
              {
                /* Advance to offset of this element.  */
                if (! c_tree_is_int_cst_equal(c_constructor_bit_index,
                                          CTreeDeclArguments(field)))
                {
                  int next = (CTreeIntCstLow(CTreeDeclArguments(field))
                              / C_BITS_PER_UNIT);
                  int here = (CTreeIntCstLow(c_constructor_bit_index)
                              / C_BITS_PER_UNIT);

                  c_assemble_zeros(next - here);
                }
              }
              {
                c_tree_node *tmp_node;
                tmp_node = c_digest_init(type, value, c_require_constant_value,
                                         c_require_constant_elements);
              c_output_constant( tmp_node, c_get_int_size_in_bytes(type));

              /* For a record or union,
                 keep track of end position of last field.  */
              if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
                  || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
                {
                  c_tree_node *temp = c_build_size_binop(
                      C_PLUS_EXPR, CTreeDeclArguments(field), CTreeDeclSize(field));
                  CTreeIntCstLow(c_constructor_bit_index)
                    = CTreeIntCstLow(temp);
                  CTreeIntCstHigh(c_constructor_bit_index)
                    = CTreeIntCstHigh(temp);
                }
              }
            }
        }

      /* Advance the variable that indicates sequential elements output.  */
      if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          c_tree_node *tem = c_build_size_binop(C_PLUS_EXPR, 
              c_constructor_unfilled_index, c_integer_one_node);
          CTreeIntCstLow(c_constructor_unfilled_index)
            = CTreeIntCstLow(tem);
          CTreeIntCstHigh(c_constructor_unfilled_index)
            = CTreeIntCstHigh(tem);
        }
      else if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE)
        c_constructor_unfilled_fields = CTreeChain(c_constructor_unfilled_fields);
      else if ( CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        c_constructor_unfilled_fields = 0;

      /* Now output any pending elements which have become next.  */
      if (pending)
        c_output_pending_init_elements(0);
    }
}


/* At the end of an implicit or explicit brace level, 
   finish up that level of constructor.
   If we were outputting the elements as they are read, return 0
   from inner levels (process_init_element ignores that),
   but return error_mark_node from the outermost level
   (that's what we want to put in DECL_INITIAL).
   Otherwise, return a CONSTRUCTOR expression.  */

c_tree_node *c_pop_init_level(implicit)
     int implicit;
{
  struct constructor_stack *p;
  int size = 0;
  c_tree_node *constructor = 0;

  if (implicit == 0)
    {
      /* When we come to an explicit close brace,
         pop any inner levels that didn't have explicit braces.  */
      while (c_constructor_stack->implicit)
      {
        c_process_init_element(c_pop_init_level(1));
      }
    }

  p = c_constructor_stack;

  if (c_constructor_type != 0)
    size = c_get_int_size_in_bytes(c_constructor_type);

  /* Now output all pending elements.  */
  c_output_pending_init_elements(1);

  /* Pad out the end of the structure.  */
  
  if (p->replacement_value)
    {
      /* If this closes a superfluous brace pair,
         just pass out the element between them.  */
      constructor = p->replacement_value;
      /* If this is the top level thing within the initializer,
         and it's for a variable, then since we already called
         assemble_variable, we must output the value now.  */
      if (p->next == 0 && c_constructor_decl != 0
          && c_constructor_incremental)
        {
          constructor = c_digest_init(c_constructor_type, constructor,
                                     c_require_constant_value,
                                     c_require_constant_elements);

          /* If initializing an array of unknown size,
             determine the size now.  */
          if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE
              && CTreeTypeValues(c_constructor_type) == 0)
            {
              int failure;

# ifdef LUDO_NO_SKIP
              int momentary_p;
              push_obstacks_nochange ();
              if (TREE_PERMANENT (constructor_type))
                end_temporary_allocation ();

              momentary_p = suspend_momentary ();
# endif

              /* We shouldn't have an incomplete array type within
                 some other type.  */
              if (c_constructor_stack->next)
                abort ();

              failure
                = c_complete_array_type(c_constructor_type, constructor, 0);
              if (failure)
                abort ();

              size = c_get_int_size_in_bytes(c_constructor_type);
# ifdef LUDO_NO_SKIP
              resume_momentary (momentary_p);
              pop_obstacks ();
# endif
            }

# ifdef LUDO_NO_SKIP
          c_output_constant(constructor, size);
# endif
        }
    }
  else if (c_constructor_type == 0)
    ;
  else if ( CTreeTypeCode(c_constructor_type)    != C_RECORD_TYPE
           &&  CTreeTypeCode(c_constructor_type) != C_UNION_TYPE
           &&  CTreeTypeCode(c_constructor_type) != C_ARRAY_TYPE
           && ! c_constructor_incremental)
    {
      /* A nonincremental scalar initializer--just return
         the element, after verifying there is just one.  */
      if (c_constructor_elements == 0)
        {
          error_init ("empty scalar initializer%s",
                      " for `%s'", NULL);
          constructor = c_error_mark_node;
        }
      else if (CTreeChain(c_constructor_elements) != 0)
        {
          error_init("extra elements in scalar initializer%s", " for `%s'", NULL);
          constructor = CTreeListValue(c_constructor_elements);
        }
      else
        constructor = CTreeListValue(c_constructor_elements);
    }
  else if (! c_constructor_incremental)
    {
      if (c_constructor_erroneous)
        constructor = c_error_mark_node;
      else
        {
# ifdef LUDO_NO_SKIP
          int momentary = suspend_momentary ();
# endif

          constructor = c_tree_build_binary_typed_expr(
              C_CONSTRUCTOR_EXPR, c_constructor_type, NULL,
              c_tree_reverse_list(c_constructor_elements));
          if (c_constructor_constant)
            CSetTreeNodeConstant(constructor);
          if (c_constructor_constant && c_constructor_simple)
            CSetTreeNodeStatic(constructor);

# ifdef LUDO_NO_SKIP
          resume_momentary (momentary);
# endif
        }
    }
  else
    {
      c_tree_node *filled;
# ifdef LUDO_NO_SKIP
      int momentary = suspend_momentary ();
# endif

      if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
          || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        {
          /* Find the offset of the end of that field.  */
          filled = c_build_size_binop(C_CEIL_DIV_EXPR,
                               c_constructor_bit_index,
                               c_tree_get_size_int(C_BITS_PER_UNIT));
        }
      else if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          /* If initializing an array of unknown size,
             determine the size now.  */
          if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE
              && CTreeTypeValues(c_constructor_type) == 0)
            {
              c_tree_node *maxindex
                = c_build_size_binop(C_MINUS_EXPR,
                              c_constructor_unfilled_index,
                              c_integer_one_node);

# ifdef LUDO_NO_SKIP
              push_obstacks_nochange ();
              if (CIsTreeNodePermanent(c_constructor_type))
                end_temporary_allocation ();
# endif
              maxindex = c_tree_copy_node(maxindex);
              CTreeTypeValues(c_constructor_type) = c_tree_build_index_type(maxindex);
              CTreeType(maxindex) = CTreeTypeValues(c_constructor_type);

              /* TYPE_MAX_VALUE is always one less than the number of elements
                 in the array, because we start counting at zero.  Therefore,
                 warn only if the value is less than zero.  */
              if (c_pedantic
                  && (c_tree_is_int_cst_sgn(
                      CTreeTypeMaxValue(CTreeTypeValues(c_constructor_type))) < 0))
              {
                error_with_decl(c_constructor_decl, "zero or negative array size `%s'");
              }
              c_layout_type(c_constructor_type);
              size = c_get_int_size_in_bytes(c_constructor_type);
# ifdef LUDO_NO_SKIP
              pop_obstacks ();
# endif
            }

          filled = c_build_size_binop(C_MULT_EXPR, c_constructor_unfilled_index,
                               c_get_size_in_bytes(CTreeType(c_constructor_type)));
        }
      else
        filled = 0;

      if (filled != 0)
      {
        c_assemble_zeros(size - CTreeIntCstLow(filled));
      }

# ifdef LUDO_NO_SKIP
      resume_momentary (momentary);
# endif
    }

          
  c_constructor_type = p->type;
  c_constructor_fields = p->fields;
  c_constructor_index = p->index;
  c_constructor_range_end = p->range_end;
  c_constructor_max_index = p->max_index;
  c_constructor_unfilled_index = p->unfilled_index;
  c_constructor_unfilled_fields = p->unfilled_fields;
  c_constructor_bit_index = p->bit_index;
  c_constructor_elements = p->elements;
  c_constructor_constant = p->constant;
  c_constructor_simple = p->simple;
  c_constructor_erroneous = p->erroneous;
  c_constructor_pending_elts = p->pending_elts;
  c_constructor_depth = p->depth;
  c_constructor_incremental = p->incremental;
  RESTORE_SPELLING_DEPTH(c_constructor_depth);

  c_constructor_stack = p->next;
  autfreeblock(p);

  if (constructor == 0)
    {
      if (c_constructor_stack == 0)
        return c_error_mark_node;
      return NULL;
    }
  return constructor;
}

/* Add one non-braced element to the current constructor level.
   This adjusts the current position within the constructor's type.
   This may also start or terminate implicit levels
   to handle a partly-braced initializer.

   Once this has found the correct level for the new element,
   it calls output_init_element.

   Note: if we are incrementally outputting this constructor,
   this function may be called with a null argument
   representing a sub-constructor that was already incrementally output.
   When that happens, we output nothing, but we do the bookkeeping
   to skip past that element of the current constructor.  */

void c_process_init_element(value)
     c_tree_node *value;
{
  c_tree_node *orig_value = value;
  int string_flag = value != 0 && CTreeNodeFirstCode(value) == C_STRING_NODE;

  /* Handle superfluous braces around string cst as in
     char x[] = {"foo"}; */
  if (string_flag
      && c_constructor_type
      && CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE
      && CTreeTypeCode(CTreeType(c_constructor_type)) == C_INTEGER_TYPE
      && c_tree_is_integer_zerop(c_constructor_unfilled_index))
    {
      c_constructor_stack->replacement_value = value;
      return;
    }

  if (c_constructor_stack->replacement_value != 0)
    {
      error_init("excess elements in struct initializer%s", " after `%s'", NULL);
      return;
    }

  /* Ignore elements of a brace group if it is entirely superfluous
     and has already been diagnosed.  */
  if (c_constructor_type == 0)
    return;

  /* If we've exhausted any levels that didn't have braces,
     pop them now.  */
  while (c_constructor_stack->implicit)
    {
      if ((CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE
           || CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
          && c_constructor_fields == 0)
      {
        c_process_init_element(c_pop_init_level(1));
      }
      else if (CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE
               && (c_constructor_max_index == 0
                   || c_tree_is_int_cst_lt(c_constructor_max_index, c_constructor_index)))
      {
        c_process_init_element( c_pop_init_level(1));
      }
      else
        break;
    }

  while (1)
    {
      if ( CTreeTypeCode(c_constructor_type) == C_RECORD_TYPE)
        {
          c_tree_node *fieldtype;
          int  fieldcode;

          if (c_constructor_fields == 0)
            {
              pedwarn_init ("excess elements in struct initializer%s",
                            " after `%s'", NULL);
              break;
            }

          fieldtype = CTreeType(c_constructor_fields);
          if (fieldtype != c_error_mark_node)
          {
            fieldtype = CTreeTypeMainVariant(fieldtype);
          }
          fieldcode = CTreeTypeCode(fieldtype);

          /* Accept a string constant to initialize a subarray.  */
          if (value != 0
              && fieldcode == C_ARRAY_TYPE
              && CTreeTypeCode(CTreeType(fieldtype)) == C_INTEGER_TYPE
              && string_flag)
          {
            value = orig_value;
          }
          /* Otherwise, if we have come to a subaggregate,
             and we don't have an element of its type, push into it.  */
          else if (value != 0 && !c_constructor_no_implicit
                   && value != c_error_mark_node
                   && CTreeTypeMainVariant(CTreeType(value)) != fieldtype
                   && (fieldcode == C_RECORD_TYPE || fieldcode == C_ARRAY_TYPE
                       || fieldcode == C_UNION_TYPE))
            {
              c_push_init_level(1);
              continue;
            }

          if (value)
            {
              c_push_member_name(c_constructor_fields);
              c_output_init_element(value, fieldtype, c_constructor_fields, 1);
              RESTORE_SPELLING_DEPTH(c_constructor_depth);
            }
          else
            /* Do the bookkeeping for an element that was
               directly output as a constructor.  */
            {
              /* For a record, keep track of end position of last field.  */
              c_tree_node *temp = c_build_size_binop(C_PLUS_EXPR,
                                      CTreeDeclArguments(c_constructor_fields),
                                      CTreeDeclSize(c_constructor_fields));
              CTreeIntCstLow(c_constructor_bit_index)
                = CTreeIntCstLow(temp);
              CTreeIntCstHigh(c_constructor_bit_index)
                = CTreeIntCstHigh(temp);

              c_constructor_unfilled_fields = CTreeChain(c_constructor_fields);
            }

          c_constructor_fields = CTreeChain(c_constructor_fields);
          /* Skip any nameless bit fields at the beginning.  */
          while (c_constructor_fields != 0
                 && CIsTreeDeclCBitField(c_constructor_fields)
                 && CTreeDeclName(c_constructor_fields) == 0)
          {
            c_constructor_fields = CTreeChain(c_constructor_fields);
          }
          break;
        }
      if ( CTreeTypeCode(c_constructor_type) == C_UNION_TYPE)
        {
          c_tree_node *fieldtype;
          int  fieldcode;

          if (c_constructor_fields == 0)
            {
              pedwarn_init ("excess elements in union initializer%s", " after `%s'", NULL);
              break;
            }

          fieldtype = CTreeType(c_constructor_fields);
          if (fieldtype != c_error_mark_node)
            fieldtype = CTreeTypeMainVariant(fieldtype);
          fieldcode = CTreeTypeCode(fieldtype);

          /* Accept a string constant to initialize a subarray.  */
          if (value != 0
              && fieldcode == C_ARRAY_TYPE
              && CTreeTypeCode(CTreeType(fieldtype)) == C_INTEGER_TYPE
              && string_flag)
          {
            value = orig_value;
          }
          /* Otherwise, if we have come to a subaggregate,
             and we don't have an element of its type, push into it.  */
          else if (value != 0 && !c_constructor_no_implicit
                   && value != c_error_mark_node
                   && CTreeTypeMainVariant(CTreeType(value)) != fieldtype
                   && (fieldcode == C_RECORD_TYPE || fieldcode == C_ARRAY_TYPE
                       || fieldcode == C_UNION_TYPE))
            {
              c_push_init_level(1);
              continue;
            }

          if (value)
            {
              c_push_member_name(c_constructor_fields);
              c_output_init_element(value, fieldtype, c_constructor_fields, 1);
              RESTORE_SPELLING_DEPTH(c_constructor_depth);
            }
          else
            /* Do the bookkeeping for an element that was
               directly output as a constructor.  */
            {
              CTreeIntCstLow(c_constructor_bit_index)
                = CTreeIntCstLow( CTreeDeclSize(c_constructor_fields));
              CTreeIntCstHigh(c_constructor_bit_index)
                = CTreeIntCstHigh( CTreeDeclSize(c_constructor_fields));

              c_constructor_unfilled_fields = CTreeChain(c_constructor_fields);
            }

          c_constructor_fields = 0;
          break;
        }
      if ( CTreeTypeCode(c_constructor_type) == C_ARRAY_TYPE)
        {
          c_tree_node *elttype = CTreeTypeMainVariant(CTreeType(c_constructor_type));
          int eltcode = CTreeTypeCode(elttype);

          /* Accept a string constant to initialize a subarray.  */
          if (value != 0
              && eltcode == C_ARRAY_TYPE
              && CTreeTypeCode(CTreeType(elttype)) == C_INTEGER_TYPE
              && string_flag)
          {
            value = orig_value;
          }
          /* Otherwise, if we have come to a subaggregate,
             and we don't have an element of its type, push into it.  */
          else if (value != 0 && !c_constructor_no_implicit
                   && value != c_error_mark_node
                   && CTreeTypeMainVariant(CTreeType(value)) != elttype
                   && (eltcode == C_RECORD_TYPE || eltcode == C_ARRAY_TYPE
                       || eltcode == C_UNION_TYPE))
            {
              c_push_init_level(1);
              continue;
            }

          if (c_constructor_max_index != 0
              && c_tree_is_int_cst_lt(c_constructor_max_index, c_constructor_index))
            {
              pedwarn_init ("excess elements in array initializer%s", " after `%s'", NULL);
              break;
            }

          /* In the case of [LO .. HI] = VALUE, only evaluate VALUE once.  */
          if (c_constructor_range_end)
            {
              if (c_constructor_max_index != 0
                  && c_tree_is_int_cst_lt(c_constructor_max_index, c_constructor_range_end))
                {
                  pedwarn_init ("excess elements in array initializer%s",
                                " after `%s'", NULL);
                  CTreeIntCstHigh(c_constructor_range_end)
                    = CTreeIntCstHigh(c_constructor_max_index);
                  CTreeIntCstLow(c_constructor_range_end)
                    = CTreeIntCstLow(c_constructor_max_index);
                }

# ifdef LUDO_NO_SKIP
              value = save_expr (value);
# endif
            }

          /* Now output the actual element.
             Ordinarily, output once.
             If there is a range, repeat it till we advance past the range.  */
          do
            {
              c_tree_node *tem;

              if (value)
                {
                  c_push_array_bounds (CTreeIntCstLow(c_constructor_index));
                  c_output_init_element(value, elttype, c_constructor_index, 1);
                  RESTORE_SPELLING_DEPTH(c_constructor_depth);
                }

              tem = c_build_size_binop(C_PLUS_EXPR, c_constructor_index,
                                c_integer_one_node);
              CTreeIntCstLow(c_constructor_index) = CTreeIntCstLow(tem);
              CTreeIntCstHigh(c_constructor_index) = CTreeIntCstHigh(tem);

              if (!value)
                /* If we are doing the bookkeeping for an element that was
                   directly output as a constructor,
                   we must update constructor_unfilled_index.  */
                {
                  CTreeIntCstLow(c_constructor_unfilled_index)
                    = CTreeIntCstLow(c_constructor_index);
                  CTreeIntCstHigh(c_constructor_unfilled_index)
                    = CTreeIntCstHigh(c_constructor_index);
                }
            }
          while (! (c_constructor_range_end == 0
                    || c_tree_is_int_cst_lt(c_constructor_range_end,
                                        c_constructor_index)));

          break;
        }

      /* Handle the sole element allowed in a braced initializer
         for a scalar variable.  */
      if (c_constructor_fields == 0)
        {
          pedwarn_init ("excess elements in scalar initializer%s",
                        " after `%s'", NULL);
          break;
        }

      if (value)
      {
        c_output_init_element(value, c_constructor_type, NULL, 1);
      }
      c_constructor_fields = 0;
      break;
    }

  /* If the (lexically) previous elments are not now saved,
     we can discard the storage for them.  */
# ifdef LUDO_NO_SKIP
  if (c_constructor_incremental && c_constructor_pending_elts == 0 && value != 0
      && c_constructor_stack == 0)
    clear_momentary ();
# endif
}
