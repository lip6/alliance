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
| File    :                   c_cdecl.c                       |
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
# include "c_clex.h"
# include "c_ctypeck.h"
# include "c_cdecl.h"
# include "c_convert.h"
# include "c_common.h"
# include "c_foldconst.h"
# include "c_debug.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))

#define C_PROMOTING_INTEGER_TYPE_P(t)				\
  ( CTreeTypeCode((t)) == C_INTEGER_TYPE				\
   && ( CTreeTypeMainVariant(t) == c_char_type_node			\
       || CTreeTypeMainVariant(t) == c_signed_char_type_node	\
       || CTreeTypeMainVariant(t) == c_unsigned_char_type_node	\
       || CTreeTypeMainVariant(t) == c_short_integer_type_node	\
       || CTreeTypeMainVariant(t) == c_short_unsigned_type_node))


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

  c_tree_node *c_tree_program_node;

/* Set nonzero by jump_optimize if control can fall through
   to the end of the function.  */
    int c_can_reach_end;

/* The binding level currently in effect.  */
static c_binding_level *c_current_binding_level;

/* The outermost binding level, for names of file scope.
   This is created when the compiler is started and exists
   through the entire run.  */
static c_binding_level *c_global_binding_level;

/* Nonzero means unconditionally make a BLOCK for the next level pushed.  */

static int c_keep_next_level_flag;

/* Nonzero means make a BLOCK for the next level pushed
   if it has subblocks.  */

static int c_keep_next_if_subblocks;
  
/* The chain of outer levels of label scopes.
   This uses the same data structure used for binding levels,
   but it works differently: each link in the chain records
   saved values of named_labels and shadowed_labels for
   a label binding level outside the current one.  */

# ifdef LUDO_NO_SKIP
static struct binding_level *c_label_level_chain;
# endif

/* a node which has tree code ERROR_MARK, and whose type is itself.
   All erroneous expressions are replaced with this node.  All functions
   that accept nodes as arguments should avoid generating error messages
   if this node is one of the arguments, since it is undesirable to get
   multiple error messages from one error in the input.  */

  c_tree_node *c_error_mark_node;


/* Data type for the expressions representing sizes of data types.
   It is the first integer type laid out.
   In C, this is int.  */
  c_tree_node *c_size_type;

/* An integer constant with value 0 whose type is sizetype.  */
  c_tree_node *c_size_zero_node;

/* An integer constant with value 1 whose type is sizetype.  */

  c_tree_node *c_size_one_node;
  

/* INTEGER_TYPE and REAL_TYPE nodes for the standard data types */

  c_tree_node *c_short_integer_type_node;
  c_tree_node *c_integer_type_node;
  c_tree_node *c_long_integer_type_node;
  c_tree_node *c_long_long_integer_type_node;

  c_tree_node *c_short_unsigned_type_node;
  c_tree_node *c_unsigned_type_node;
  c_tree_node *c_long_unsigned_type_node;
  c_tree_node *c_long_long_unsigned_type_node;

  c_tree_node *c_boolean_type_node;
  c_tree_node *c_boolean_false_node;
  c_tree_node *c_boolean_true_node;

  c_tree_node *c_ptrdiff_type_node;

  c_tree_node *c_unsigned_char_type_node;
  c_tree_node *c_signed_char_type_node;
  c_tree_node *c_char_type_node;
  c_tree_node *c_wchar_type_node;
  c_tree_node *c_signed_wchar_type_node;
  c_tree_node *c_unsigned_wchar_type_node;

  c_tree_node *c_float_type_node;
  c_tree_node *c_double_type_node;
  c_tree_node *c_long_double_type_node;

  c_tree_node *c_complex_integer_type_node;
  c_tree_node *c_complex_float_type_node;
  c_tree_node *c_complex_double_type_node;
  c_tree_node *c_complex_long_double_type_node;

/*I
  c_tree_node *intQI_type_node;
  c_tree_node *intHI_type_node;
  c_tree_node *intSI_type_node;
  c_tree_node *intDI_type_node;

  c_tree_node *unsigned_intQI_type_node;
  c_tree_node *unsigned_intHI_type_node;
  c_tree_node *unsigned_intSI_type_node;
  c_tree_node *unsigned_intDI_type_node;
*/

/* a VOID_TYPE node.  */

  c_tree_node *c_void_type_node;

/* Nodes for types `void *' and `const void *'.  */

  c_tree_node *c_ptr_type_node;
  c_tree_node *c_const_ptr_type_node;

/* Nodes for types `char *' and `const char *'.  */

  c_tree_node *c_string_type_node;
  c_tree_node *c_const_string_type_node;

/* Type `char[SOMENUMBER]'.
   Used when an array of char is needed and the size is irrelevant.  */

  c_tree_node *c_char_array_type_node;

/* Type `int[SOMENUMBER]' or something like it.
   Used when an array of int needed and the size is irrelevant.  */

  c_tree_node *c_int_array_type_node;

/* Type `wchar_t[SOMENUMBER]' or something like it.
   Used when a wide string literal is created.  */

  c_tree_node *c_wchar_array_type_node;

/* type `int ()' -- used for implicit declaration of functions.  */

  c_tree_node *c_default_function_type;

/* function types `double (double)' and `double (double, double)', etc.  */

  c_tree_node *c_double_ftype_double;
  c_tree_node *c_double_ftype_double_double;
  c_tree_node *c_int_ftype_int;
  c_tree_node *c_long_ftype_long;
  c_tree_node *c_float_ftype_float;
  c_tree_node *c_ldouble_ftype_ldouble;

/* Function type `void (void *, void *, int)' and similar ones */

  c_tree_node *c_void_ftype_ptr_ptr_int; 
  c_tree_node *c_int_ftype_ptr_ptr_int;
  c_tree_node *c_void_ftype_ptr_int_int;

/* Function type `char *(char *, char *)' and similar ones */
  c_tree_node *c_string_ftype_ptr_ptr;
  c_tree_node *c_int_ftype_string_string;

/* Function type `int (const void *, const void *, size_t)' */
  c_tree_node *c_int_ftype_cptr_cptr_sizet;

/* Two expressions that are constants with value zero.
   The first is of type `int', the second of type `void *'.  */
  c_tree_node *c_integer_zero_node;
  c_tree_node *c_null_pointer_node;

/* A node for the integer constant 1.  */
  c_tree_node *c_integer_one_node;

/* Nonzero if we have seen an invalid cross reference
   to a struct, union, or enum, but not yet printed the message.  */

  c_tree_node *c_pending_invalid_xref;
/* File and line to appear in the eventual error message.  */
char *c_pending_invalid_xref_file;
int c_pending_invalid_xref_line;

/* While defining an enum type, this is 1 plus the last enumerator
   constant value.  Note that will do not have to save this or `enum_overflow'
   around nested function definition since such a definition could only
   occur in an enum value expression and we don't use these variables in
c   that case.  */

static c_tree_node *c_enum_next_value;

/* Nonzero means that there was overflow computing enum_next_value.  */

static int c_enum_overflow;

/* Parsing a function declarator leaves a list of parameter names
   or a chain or parameter decls here.  */

static c_tree_node *c_last_function_parms;

/* Parsing a function declarator leaves here a chain of structure
   and enum types declared in the parmlist.  */

static c_tree_node *c_last_function_parm_tags;

/* After parsing the declarator that starts a function definition,
   `c_start_function' puts here the list of parameter names or chain of decls.
   `store_parm_decls' finds it here.  */

static c_tree_node *c_current_function_parms;

/* Similar, for last_function_parm_tags.  */
static c_tree_node *c_current_function_parm_tags;

/* Similar, for the file and line that the prototype came from if this is
   an old-style definition.  */
static char *c_current_function_prototype_file;
static int   c_current_function_prototype_line;

/* A list (chain of TREE_LIST nodes) of all LABEL_DECLs in the function
   that have names.  Here so we can clear out their names' definitions
   at the end of the function.  */

static c_tree_node *c_named_labels;

/* A list of LABEL_DECLs from outer contexts that are currently shadowed.  */

static c_tree_node *c_shadowed_labels;

/* Nonzero when store_parm_decls is called indicates a varargs function.
   Value not meaningful after store_parm_decls.  */

static int c_c_function_varargs;

/* The FUNCTION_DECL for the function currently being compiled,
   or 0 if between functions.  */
  c_tree_node *c_current_function_decl;

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement that specifies a return value is seen.  */

int c_current_function_returns_value;

/* Set to 0 at beginning of a function definition, set to 1 if
   a return statement with no argument is seen.  */

int c_current_function_returns_null;

/* Set to nonzero by `c_grokdeclarator' for a function
   whose return type is defaulted, if warnings for this are desired.  */

static int c_warn_about_return_type;

/* Nonzero when starting a function declared `extern inline'.  */

static int c_current_extern_inline;

/* Nonzero means the expression being parsed will never be evaluated.
   This is a count, since unevaluated expressions can nest.  */
  int c_skip_evaluation;

/* Nonzero if current function uses varargs.h or equivalent.
   Zero for functions that use stdarg.h.  */

  int c_current_function_varargs;

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

c_binding_level *c_alloc_binding_level()
{
  return( (c_binding_level *)autallocheap( sizeof( c_binding_level ) ) );
}

void c_free_binding_level( Binding )

  c_binding_level *Binding;
{
  autfreeheap( Binding, sizeof( c_binding_level ) );
}

/* Nonzero if we are currently in the global binding level.  */

int c_global_bindings_p()
{
  return c_current_binding_level == c_global_binding_level;
}

/*------------------------------------------------------------\
|                                                             |
|                          View  Functions                    |
|                                                             |
\------------------------------------------------------------*/

void c_view_binding( Binding )

   c_binding_level *Binding;
{
  fprintf( stdout, "NAME LIST\n" );
  c_tree_view_node_list( Binding->NAME_LIST );
  fprintf( stdout, "TAG LIST\n" );
  c_tree_view_node_list( Binding->TAG_LIST  );
  fprintf( stdout, "SHADOWED LIST\n" );
  c_tree_view_node_list( Binding->SHADOWED_LIST  );
  fprintf( stdout, "BLOCK LIST\n" );
  c_tree_view_node_list( Binding->BLOCK_LIST  );
  fprintf( stdout, "THIS BLOCK\n" );
  c_tree_view_node_list( Binding->THIS_BLOCK  );
  fprintf( stdout, "PARAM LIST\n" );
  c_tree_view_node_list( Binding->PARAM_LIST  );
}

void c_view_current_binding()
{
  c_view_binding( c_current_binding_level );
}

/*------------------------------------------------------------\
|                                                             |
|                           C Clear Parameter Order         |
|                                                             |
\------------------------------------------------------------*/

/* Clear the given order of parms in `parm_order'.
   Used at start of parm list,
   and also at semicolon terminating forward decls.  */

void c_clear_parm_order()
{
  c_current_binding_level->PARAM_LIST = (c_tree_node *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                           C Push Level                    |
|                                                             |
\------------------------------------------------------------*/

/* Enter a new binding level.
   If TAG_TRANSPARENT is nonzero, do so only for the name space of variables,
   not for that of tags.  */

void c_push_level( tag_transparent )

  int tag_transparent;
{
  c_binding_level *newlevel = (c_binding_level *)0;

  /* If this is the top level of a function,
     just make sure that NAMED_LABELS is 0.  */

  if ( c_current_binding_level == c_global_binding_level )
  {
    c_named_labels = 0;
  }

  newlevel = c_alloc_binding_level();

  /* Add this level to the front of the chain (stack) of levels that
     are active.  */

  newlevel->TAG_TRANSPARENT
    = (tag_transparent
       || (c_current_binding_level
           ? c_current_binding_level->SUBBLOCKS_TAG_TRANSPARENT
           : 0));
  newlevel->LEVEL_CHAIN = c_current_binding_level;
  c_current_binding_level = newlevel;

  newlevel->KEEP = c_keep_next_level_flag;
  c_keep_next_level_flag = 0;
  newlevel->KEEP_IF_SUBBLOCKS = c_keep_next_if_subblocks;
  c_keep_next_if_subblocks = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                           C Pop Level                     |
|                                                             |
\------------------------------------------------------------*/


/* Clear the limbo values of all identifiers defined in BLOCK or a subblock. */

static void c_clear_limbo_values (block)
     c_tree_node *block;
{
  c_tree_node *tem;

  for (tem = CTreeBlockVars(block); tem; tem = CTreeChain(tem))
  {
    if (CTreeDeclName(tem) != 0)
    {
      CTreeIdentLimboValue( CTreeDeclName(tem)) = 0;
    }
  }

  for (tem = CTreeBlockSubBlocks(block); tem; tem = CTreeChain(tem))
  {
    c_clear_limbo_values(tem);
  }
}

/* Exit a binding level.
   Pop the level off, and restore the state of the identifier-decl mappings
   that were in effect when this level was entered.

   If KEEP is nonzero, this level had explicit declarations, so
   and create a "block" (a BLOCK node) for the level
   to record its declarations and subblocks for symbol table output.

   If FUNCTIONBODY is nonzero, this level is the body of a function,
   so create a block as if KEEP were set and also clear out all
   label names.

   If REVERSE is nonzero, reverse the order of decls before putting
   them into the BLOCK.  */

c_tree_node *c_pop_level( keep, reverse, functionbody )
     int keep;
     int reverse;
     int functionbody;
{
  c_tree_node *link;
  /* The chain of decls was accumulated in reverse order.
     Put it into forward order, just for cleanliness.  */
  c_tree_node *decls;
  c_tree_node *tags = c_current_binding_level->TAG_LIST;
  c_tree_node *subblocks = c_current_binding_level->BLOCK_LIST;
  c_tree_node *block = (c_tree_node *)0;
  c_tree_node *decl;
  int block_previously_created;

  /*
  loc_c_view_binding( c_current_binding_level );
  */

  keep |= c_current_binding_level->KEEP;

  /* Get the decls in the order they were written.
     Usually current_binding_level->names is in reverse order.
     But parameter decls were previously put in forward order.  */

  if (reverse)
  {
    c_current_binding_level->NAME_LIST
      = decls = c_tree_reverse_list(c_current_binding_level->NAME_LIST);
  }
  else
  {
    decls = c_current_binding_level->NAME_LIST;
  }

  /* Output any nested inline functions within this block
     if they weren't already output.  */

  for (decl = decls; decl; decl = CTreeChain(decl))
  {
    if ( CTreeDeclCode(decl) == C_FUNCTION_DECL
      && ! CIsTreeNodeAsmWritten(decl)
      && CTreeDeclInitial(decl) != 0
      && CIsTreeNodeAddressable(decl))
      {
      /* If this decl was copied from a file-scope decl
         on account of a block-scope extern decl,
         propagate TREE_ADDRESSABLE to the file-scope decl.

         DECL_ABSTRACT_ORIGIN can be set to itself if warn_return_type is
         true, since then the decl goes through save_for_inline_copying.  */
      if ( CTreeDeclAbstractOrigin(decl) != 0
          && CTreeDeclAbstractOrigin(decl) != decl)
        CSetTreeNodeAddressable( CTreeDeclAbstractOrigin(decl));
# ifdef LUDO_NO_SKIP
      else if (DECL_SAVED_INSNS (decl) != 0)
        {
          c_push_function_context ();
          c_output_inline_function (decl);
          c_pop_function_context ();
        }
# endif
      }
  }

  /* If there were any declarations or structure tags in that level,
     or if this level is a function body,
     create a BLOCK to record them for the life of this function.  */

  block = 0;
  block_previously_created = (c_current_binding_level->THIS_BLOCK != 0);
  if (block_previously_created)
  {
    block = c_current_binding_level->THIS_BLOCK;
  }
  else if (keep || functionbody
         || (c_current_binding_level->KEEP_IF_SUBBLOCKS && subblocks != 0))
  {
    block = c_tree_add_node( C_BLOCK_NODE );
  }
  if (block != 0)
  {
      CTreeBlockVars(block) = decls;
      CTreeBlockTypeTags(block) = tags;
      CTreeBlockSubBlocks(block) = subblocks;
# ifdef LUDO_NO_SKIP
      remember_end_note (block);
# endif
  }

  /* In each subblock, record that this is its superior.  */

  for (link = subblocks; link; link = CTreeChain(link))
  {
    CTreeBlockSuperContext(link) = block;
  }

  /* Clear out the meanings of the local variables of this level.  */

  for (link = decls; link; link = CTreeChain(link))
    {
      if ( CTreeDeclName(link) != 0)
      {
        /* If the ident. was used or addressed via a local extern decl,
           don't forget that fact.  */
        if ( CIsTreeDeclExternal(link))
          {
            if ( CIsTreeNodeUsed(link))
            {
              CSetTreeNodeUsed( CTreeDeclName(link));
            }
# ifdef LUDO_NO_SKIP
            if (TREE_ADDRESSABLE (link))
            TREE_ADDRESSABLE (DECL_ASSEMBLER_NAME (link)) = 1;
# endif
          }
        CTreeIdentLocalValue( CTreeDeclName(link)) = 0;
      }
    }

  /* Restore all name-meanings of the outer levels
     that were shadowed by this level.  */

  for (link = c_current_binding_level->SHADOWED_LIST; link; link = CTreeChain(link))
  {
    CTreeIdentLocalValue( CTreeListPurpose(link)) = CTreeListValue(link);
  }

  /* If the level being exited is the top level of a function,
     check over all the labels, and clear out the current
     (function local) meanings of their names.  */

  if (functionbody)
  {
    c_clear_limbo_values (block);

    /* If this is the top level block of a function,
     the vars are the function's parameters.
     Don't leave them in the BLOCK because they are
     found in the FUNCTION_DECL instead.  */

    CTreeBlockVars(block) = 0;

    /* Clear out the definitions of all label names,
     since their scopes end here,
     and add them to BLOCK_VARS.  */

    for (link = c_named_labels; link; link = CTreeChain(link))
    {
      c_tree_node *label = CTreeListValue(link);

      if ( CTreeDeclInitial(label) == 0)
      {
        error_with_decl (label, "label `%s' used but not defined");
        /* Avoid crashing later.  */
        c_define_label(c_input_filename, c_lineno, CTreeDeclName(label));
      }
      else if (c_warn_unused && ! CIsTreeNodeUsed(label))
      {
        warning_with_decl(label, "label `%s' defined but not used");
      }
      CTreeIdentLabelValue( CTreeDeclName(label)) = 0;

      /* Put the labels into the "variables" of the
         top-level block, so debugger can see them.  */
      CTreeChain(label) = CTreeBlockVars(block);
      CTreeBlockVars(block) = label;
    }
  }

  /* Pop the current level, and free the structure for reuse.  */

  {
    c_binding_level *level = c_current_binding_level;
    c_current_binding_level = c_current_binding_level->LEVEL_CHAIN;

# ifdef LUDO_DONE
    level->level_chain = free_binding_level;
    free_binding_level = level;
# else
    c_free_binding_level( level );
# endif
  }

  /* Dispose of the block that we just made inside some higher level.  */
  if (functionbody)
  {
    CTreeDeclInitial(c_current_function_decl) = block;
  }
  else if (block)
  {
    if (!block_previously_created)
    {
        c_current_binding_level->BLOCK_LIST
          = c_tree_concat_list(c_current_binding_level->BLOCK_LIST, block);
    }
  }
  /* If we did not make a block for the level just exited,
     any blocks made for inner levels
     (since they cannot be recorded as subblocks in that level)
     must be carried forward so they will later become subblocks
     of something else.  */
  else if (subblocks)
  {
    c_current_binding_level->BLOCK_LIST
      = c_tree_concat_list(c_current_binding_level->BLOCK_LIST, subblocks);
  }

  /* Set the TYPE_CONTEXTs for all of the tagged types belonging to this
     binding contour so that they point to the appropriate construct, i.e.
     either to the current FUNCTION_DECL node, or else to the BLOCK node
     we just constructed.

     Note that for tagged types whose scope is just the formal parameter
     list for some function type specification, we can't properly set
     their TYPE_CONTEXTs here, because we don't have a pointer to the
     appropriate FUNCTION_TYPE node readily available to us.  For those
     cases, the TYPE_CONTEXTs of the relevant tagged type nodes get set
     in `grokdeclarator' as soon as we have created the FUNCTION_TYPE
     node which will represent the "scope" for these "parameter list local"
     tagged types.
  */

  if (functionbody)
  {
    for (link = tags; link; link = CTreeChain (link))
    {
      CTreeTypeContext( CTreeListValue(link)) = c_current_function_decl;
    }
  }
  else if (block)
  {
    for (link = tags; link; link = CTreeChain (link))
    {
      CTreeTypeContext( CTreeListValue(link)) = block;
    }
  }

  if (block) CSetTreeNodeUsed(block);

  return block;
}

/*------------------------------------------------------------\
|                                                             |
|                          Lookup Name                        |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_lookup_name( TreeName )

  c_tree_node *TreeName;
{
  c_tree_node *TreeVal;

  if ( ( c_current_binding_level != c_global_binding_level    ) &&
       ( CTreeIdentLocalValue( TreeName ) != (c_tree_node *)0 ) )
  {
    TreeVal = CTreeIdentLocalValue( TreeName );
  }
  else
  {
    TreeVal = CTreeIdentGlobalValue( TreeName );
  }

  return TreeVal;
}

/*------------------------------------------------------------\
|                                                             |
|                     Lookup Name Current Level               |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_lookup_name_current_level( TreeName )

  c_tree_node *TreeName;
{
  c_tree_node *ScanTree;

  if ( c_current_binding_level == c_global_binding_level )
  {
    return( CTreeIdentGlobalValue( TreeName ) );
  }

  if ( CTreeIdentLocalValue( TreeName ) == (c_tree_node *)0 )
  {
    return( (c_tree_node *)0 );
  }

  for ( ScanTree  = c_current_binding_level->NAME_LIST;
        ScanTree != (c_tree_node *)0;
        ScanTree  = CTreeChain( ScanTree ) )
  {
    if ( CTreeDeclName( ScanTree ) == TreeName ) break;
  }

  return ScanTree;
}


/* Return zero if the declaration NEWDECL is valid
   when the declaration OLDDECL (assumed to be for the same name)
   has already been seen.
   Otherwise return an error message format string with a %s
   where the identifier should go.  */

static char *c_redeclaration_error_message (newdecl, olddecl)
     c_tree_node *newdecl;
     c_tree_node *olddecl;
{
  if ( CTreeDeclCode(newdecl) == C_TYPE_DECL)
    {
      if (c_flag_traditional && CTreeType(newdecl) == CTreeType(olddecl))
        return 0;
      /* pushdecl creates distinct types for TYPE_DECLs by calling
         build_type_copy, so the above comparison generally fails.  We do
         another test against the CTreeTypeMainVariantof the olddecl, which
         is equivalent to what this code used to do before the build_type_copy
         call.  The variant type distinction should not matter for traditional
         code, because it doesn't have type qualifiers.  */
      if (c_flag_traditional 
          && CTreeTypeMainVariant(CTreeType(olddecl)) == CTreeType(newdecl))
        return 0;
      if (CIsTreeDeclSystemHeader(olddecl) || CIsTreeDeclSystemHeader(newdecl))
        return 0;
      return "redefinition of `%s'";
    }
  else if ( CTreeDeclCode(newdecl) == C_FUNCTION_DECL)
    {
      /* Declarations of functions can insist on internal linkage
         but they can't be inconsistent with internal linkage,
         so there can be no error on that account.
         However defining the same name twice is no good.  */
      if (CTreeDeclInitial(olddecl) != 0 && CTreeDeclInitial(newdecl) != 0
          /* However, defining once as extern inline and a second
             time in another way is ok.  */
          && !( CIsTreeDeclInline (olddecl) && CIsTreeDeclExternal (olddecl)
               && !( CIsTreeDeclInline (newdecl) && CIsTreeDeclExternal (newdecl)))
          )
        return "redefinition of `%s'";
      return 0;
    }
  else if (c_current_binding_level == c_global_binding_level)
    {
      /* Objects declared at top level:  */
      /* If at least one is a reference, it's ok.  */
      if (CIsTreeDeclExternal(newdecl) || CIsTreeDeclExternal(olddecl))
        return 0;
      /* Reject two definitions.  */
      if (CTreeDeclInitial(olddecl) != 0 && CTreeDeclInitial(newdecl) != 0)
        return "redefinition of `%s'";
      /* Now we have two tentative defs, or one tentative and one real def.  */
      /* Insist that the linkage match.  */
      if ( CIsTreeNodePublic(olddecl) != CIsTreeNodePublic(newdecl))
        return "conflicting declarations of `%s'";
      return 0;
    }
  else if (c_current_binding_level->PARAM_FLAG
           && CIsTreeNodeAsmWritten(olddecl) && !CIsTreeNodeAsmWritten(newdecl))
    return 0;
  else
    {
      /* Newdecl has block scope.  If olddecl has block scope also, then
         reject two definitions, and reject a definition together with an
         external reference.  Otherwise, it is OK, because newdecl must
         be an extern reference to olddecl.  */
      if (!(CIsTreeDeclExternal(newdecl) && CIsTreeDeclExternal(olddecl))
          && CTreeDeclContext(newdecl) == CTreeDeclContext(olddecl))
        return "redeclaration of `%s'";
      return 0;
    }
}

/* Return 1 if PARMS specifies a fixed number of parameters
   and none of their types is affected by default promotions.  */

int c_is_self_promoting_args_p(parms)
     c_tree_node *parms;
{
  c_tree_node *t;
  for (t = parms; t; t = CTreeChain(t))
    {
      c_tree_node *type = CTreeListValue(t);

      if (CTreeChain(t) == 0 && type != c_void_type_node)
        return 0;

      if (type == 0)
        return 0;

      if (CTreeTypeMainVariant(type) == c_float_type_node)
        return 0;

      if (C_PROMOTING_INTEGER_TYPE_P(type))
        return 0;
    }
  return 1;
}


/* Handle when a new declaration NEWDECL
   has the same name as an old one OLDDECL
   in the same binding contour.
   Prints an error message if appropriate.

   If safely possible, alter OLDDECL to look like NEWDECL, and return 1.
   Otherwise, return 0.

   When DIFFERENT_BINDING_LEVEL is true, NEWDECL is an external declaration,
   and OLDDECL is in an outer binding level and should thus not be changed.  */

static int
c_is_duplicate_decls(newdecl, olddecl, different_binding_level)
     c_tree_node *newdecl;
     c_tree_node *olddecl;
     int different_binding_level;
{
  int types_match = c_is_comptypes(CTreeType(newdecl), CTreeType(olddecl));
  int new_is_definition = ( CTreeDeclCode(newdecl) == C_FUNCTION_DECL
                           && CTreeDeclInitial(newdecl) != 0);
  c_tree_node *oldtype = CTreeType(olddecl);
  c_tree_node *newtype = CTreeType(newdecl);
  char *errmsg = 0;

# ifdef LUDO_NO_SKIP
  if ( CTreeNodeFirstCode(olddecl) == C_DECL_NODE )
    DECL_MACHINE_ATTRIBUTES (newdecl) = DECL_MACHINE_ATTRIBUTES (olddecl);
# endif

  if ( CTreeNodeFirstCode(newtype) == C_ERROR_NODE
      || CTreeNodeFirstCode(oldtype) == C_ERROR_NODE)
    types_match = 0;

  /* New decl is completely inconsistent with the old one =>
     tell caller to replace the old one.
     This is always an error except in the case of shadowing a builtin.  */
  if ( CTreeDeclCode(olddecl) != CTreeDeclCode(newdecl))
    {
      if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
          && ( CIsTreeDeclBuiltIn(olddecl)
              || CIsTreeNodeUnsigned(olddecl)))
        {
          /* If you declare a built-in or predefined function name as static,
             the old definition is overridden,
             but optionally warn this was a bad choice of name.  */
          if (! CIsTreeNodePublic(newdecl))
            {
              if (!c_warn_shadow)
                ;
              else if ( CIsTreeDeclBuiltIn(olddecl))
              {
                warning_with_decl (newdecl, "shadowing built-in function `%s'");
              }
              else
              {
                warning_with_decl (newdecl, "shadowing library function `%s'");
              }
            }
          /* Likewise, if the built-in is not ansi, then programs can
             override it even globally without an error.  */
          else if (! CIsTreeDeclBuiltIn(olddecl))
          {
            warning_with_decl (newdecl,
                               "library function `%s' declared as non-function");
          }
          else if ( CIsTreeNodeUnsigned(olddecl))
          {
            warning_with_decl (newdecl,
                               "built-in function `%s' declared as non-function");
          }
          else
          {
            warning_with_decl (newdecl,
                             "built-in function `%s' declared as non-function");
          }
        }
      else
        {
          error_with_decl (newdecl, "`%s' redeclared as different kind of symbol");
          error_with_decl (olddecl, "previous declaration of `%s'");
        }

      return 0;
    }

  /* For real parm decl following a forward decl,
     return 1 so old decl will be reused.  */
  if (types_match && CTreeDeclCode(newdecl) == C_PARAM_DECL
      && CIsTreeNodeAsmWritten(olddecl) && ! CIsTreeNodeAsmWritten(newdecl))
    return 1;

  /* The new declaration is the same kind of object as the old one.
     The declarations may partially match.  Print warnings if they don't
     match enough.  Ultimately, copy most of the information from the new
     decl to the old one, and keep using the old one.  */

  if (c_flag_traditional && CTreeDeclCode(newdecl) == C_FUNCTION_DECL
      && CTreeIdentImplicitDecl(CTreeDeclName(newdecl)) == olddecl
      && CTreeDeclInitial(olddecl) == 0)
    /* If -traditional, avoid error for redeclaring fcn
       after implicit decl.  */
    ;
  else if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
           && CIsTreeDeclBuiltIn(olddecl))
    {
      /* A function declaration for a built-in function.  */
      if (! CIsTreeNodePublic(newdecl))
        {
          /* If you declare a built-in function name as static, the
             built-in definition is overridden,
             but optionally warn this was a bad choice of name.  */
          if (c_warn_shadow)
          {
            warning_with_decl (newdecl, "shadowing built-in function `%s'");
          }
          /* Discard the old built-in function.  */
          return 0;
        }
      else if (!types_match)
        {
          /* Accept the return type of the new declaration if same modes.  */
          c_tree_node *oldreturntype = CTreeType(oldtype);
          c_tree_node *newreturntype = CTreeType(newtype);

          /* Make sure we put the new type in the same obstack as the old ones.
             If the old types are not both in the same obstack, use the
             permanent one.  */
# ifdef LUDO_NO_SKIP
          if (TYPE_OBSTACK (oldtype) == TYPE_OBSTACK (newtype))
            push_obstacks (TYPE_OBSTACK (oldtype), TYPE_OBSTACK (oldtype));
          else
            {
              push_obstacks_nochange ();
              end_temporary_allocation ();
            }
# endif

          if (CTreeTypeMode(oldreturntype) == CTreeTypeMode(newreturntype))
            {
              /* Function types may be shared, so we can't just modify
                 the return type of olddecl's function type.  */
              c_tree_node *trytype
                = c_tree_build_function_type(newreturntype, CTreeTypeValues(oldtype));
              
              types_match = c_is_comptypes(newtype, trytype);
              if (types_match)
                oldtype = trytype;
            }
          /* Accept harmless mismatch in first argument type also.
             This is for ffs.  */
          if ( CTreeTypeValues(CTreeType(newdecl)) != 0
              && CTreeTypeValues(oldtype) != 0
              && CTreeListValue( CTreeTypeValues(newtype)) != 0
              && CTreeListValue( CTreeTypeValues(oldtype)) != 0
              && (CTreeTypeMode( CTreeListValue( CTreeTypeValues(newtype)))
                  == CTreeTypeMode( CTreeListValue( CTreeTypeValues(oldtype)))))
            {
              /* Function types may be shared, so we can't just modify
                 the return type of olddecl's function type.  */
              c_tree_node *trytype
                = c_tree_build_function_type(CTreeType(oldtype),
                                  c_tree_add_head_list( NULL, 
                                                 CTreeListValue( CTreeTypeValues(newtype)),
                                                  CTreeChain( CTreeTypeValues(oldtype))));
              
              types_match = c_is_comptypes(newtype, trytype);
              if (types_match)
                oldtype = trytype;
            }
          if (! different_binding_level)
            CTreeType(olddecl) = oldtype;

# ifdef LUDO_NO_SKIP
          pop_obstacks ();
# endif
        }
      if (!types_match)
        {
          /* If types don't match for a built-in, throw away the built-in.  */
          warning_with_decl (newdecl, "conflicting types for built-in function `%s'");
          return 0;
        }
    }
  else if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
           && CTreeDeclLineNum(olddecl) == 0)
    {
      /* A function declaration for a predeclared function
         that isn't actually built in.  */
      if (! CIsTreeNodePublic(newdecl))
        {
          /* If you declare it as static, the
             default definition is overridden.  */
          return 0;
        }
      else if (!types_match)
        {
          /* If the types don't match, preserve volatility indication.
             Later on, we will discard everything else about the
             default declaration.  */
          if ( CIsTreeNodeVolatile(olddecl) )
          {
            CSetTreeNodeVolatile(newdecl);
          }
        }
    }
  /* Permit char *foo () to match void *foo (...) if not pedantic,
     if one of them came from a system header file.  */
  else if (!types_match
           && CTreeDeclCode(olddecl) == C_FUNCTION_DECL
           && CTreeDeclCode(newdecl) == C_FUNCTION_DECL
           && CTreeTypeCode(CTreeType(oldtype)) == C_POINTER_TYPE
           && CTreeTypeCode(CTreeType(newtype)) == C_POINTER_TYPE
           && (CIsTreeDeclSystemHeader(olddecl)
               || CIsTreeDeclSystemHeader(newdecl))
           && ((CTreeTypeMainVariant(CTreeType(CTreeType(newtype))) == c_void_type_node
                && CTreeTypeValues(oldtype) == 0
                && c_is_self_promoting_args_p( CTreeTypeValues(newtype))
                && CTreeType(CTreeType(oldtype)) == c_char_type_node)
               ||
               (CTreeType(CTreeType(newtype)) == c_char_type_node
                && CTreeTypeValues(newtype) == 0
                && c_is_self_promoting_args_p( CTreeTypeValues(oldtype))
                && CTreeTypeMainVariant(CTreeType(CTreeType(oldtype))) == c_void_type_node)))
    {
      if (c_pedantic)
      {
        pedwarn_with_decl (newdecl, "conflicting types for `%s'");
      }
      /* Make sure we keep void * as ret type, not char *.  */
      if (CTreeTypeMainVariant(CTreeType(CTreeType(oldtype))) == c_void_type_node)
        CTreeType(newdecl) = newtype = oldtype;

      /* Set DECL_IN_SYSTEM_HEADER, so that if we see another declaration
         we will come back here again.  */
      CSetTreeDeclSystemHeader(newdecl);
    }
  else if (!types_match
           /* Permit char *foo (int, ...); followed by char *foo ();
              if not pedantic.  */
           && ! ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
                 && ! c_pedantic
                 /* Return types must still match.  */
                 && c_is_comptypes (CTreeType(oldtype), CTreeType(newtype))
                 && CTreeTypeValues(newtype) == 0))
    {
      error_with_decl (newdecl, "conflicting types for `%s'");
      /* Check for function type mismatch
         involving an empty arglist vs a nonempty one.  */
      if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
          && c_is_comptypes(CTreeType(oldtype), CTreeType(newtype))
          && ((CTreeTypeValues(oldtype) == 0
               && CTreeDeclInitial(olddecl) == 0)
              ||
              ( CTreeTypeValues(newtype) == 0
               && CTreeDeclInitial(newdecl) == 0)))
        {
          /* Classify the problem further.  */
          c_tree_node *t = CTreeTypeValues(oldtype);
          if (t == 0)
            t = CTreeTypeValues(newtype);
          for (; t; t = CTreeChain(t))
            {
              c_tree_node *type = CTreeListValue(t);

              if (CTreeChain(t) == 0
                  && CTreeTypeMainVariant(type) != c_void_type_node)
                {
                  error ("A parameter list with an ellipsis can't match");
                  error ("an empty parameter name list declaration.");
                  break;
                }

              if (CTreeTypeMainVariant(type) == c_float_type_node
                  || C_PROMOTING_INTEGER_TYPE_P(type))
                {
                  error ("An argument type that has a default promotion");
                  error ("can't match an empty parameter name list declaration.");
                  break;
                }
            }
        }
      error_with_decl (olddecl, "previous declaration of `%s'");
    }
  else
    {
      errmsg = c_redeclaration_error_message (newdecl, olddecl);
      if (errmsg)
        {
          error_with_decl (newdecl, errmsg);
          error_with_decl (olddecl,
                           ((CTreeDeclInitial(olddecl)
                             && c_current_binding_level == c_global_binding_level)
                            ? "`%s' previously defined here"
                            : "`%s' previously declared here"));
        }
      else if ( CTreeDeclCode(newdecl) == C_TYPE_DECL
               && (CIsTreeDeclSystemHeader(olddecl) 
                   || CIsTreeDeclSystemHeader(newdecl)))
        {
          warning_with_decl (newdecl, "redefinition of `%s'");
          warning_with_decl 
            (olddecl,
             ((CTreeDeclInitial(olddecl)
               && c_current_binding_level == c_global_binding_level)
              ? "`%s' previously defined here"
              : "`%s' previously declared here"));
        }
      else if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
               && CTreeDeclInitial(olddecl) != 0
               && CTreeTypeValues(oldtype) == 0
               && CTreeTypeValues(newtype) != 0
               && CTreeTypeNonCopiedParts(oldtype) != 0)
        {
          c_tree_node *type;
          c_tree_node *parm;
          int nargs;
          /* Prototype decl follows defn w/o prototype.  */

          for (parm = CTreeTypeNonCopiedParts(oldtype),
               type = CTreeTypeValues(newtype),
               nargs = 1;
               (CTreeTypeMainVariant( CTreeListValue(parm)) != c_void_type_node
                || CTreeTypeMainVariant( CTreeListValue(type)) != c_void_type_node);
               parm = CTreeChain(parm), type = CTreeChain(type), nargs++)
            {
              if (CTreeTypeMainVariant( CTreeListValue(parm)) == c_void_type_node
                  || CTreeTypeMainVariant( CTreeListValue(type)) == c_void_type_node)
                {
                  errmsg = "prototype for `%s' follows and number of arguments";
                  break;
                }
              /* Type for passing arg must be consistent
                 with that declared for the arg.  */
              if (! c_is_comptypes( CTreeListValue(parm), CTreeListValue(type))
                  /* If -traditional, allow `unsigned int' instead of `int'
                     in the prototype.  */
                  && 
                  (! (c_flag_traditional
                    && CTreeTypeMainVariant( CTreeListValue(parm)) == c_integer_type_node
                    && CTreeTypeMainVariant( CTreeListValue(type)) == c_unsigned_type_node)))
                {
                  errmsg = "prototype for `%s' follows and argument %d";
                  break;
                }
            }
          if (errmsg)
            {
              error_with_decl (newdecl, errmsg, nargs);
              error_with_decl (olddecl,
                               "doesn't match non-prototype definition here");
            }
          else
            {
              warning_with_decl (newdecl, "prototype for `%s' follows");
              warning_with_decl (olddecl, "non-prototype definition here");
            }
        }
      /* Warn about mismatches in various flags.  */
      else
        {
          /* Warn if function is now inline
             but was previously declared not inline and has been called.  */
          if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
              && ! CIsTreeDeclInline (olddecl)
              && CIsTreeDeclInline (newdecl)
              && CIsTreeNodeUsed(olddecl))
          {
            warning_with_decl (newdecl,
                               "`%s' declared inline after being called");
          }
          if ( CTreeDeclCode(olddecl) == C_FUNCTION_DECL
              && ! CIsTreeDeclInline (olddecl) && CIsTreeDeclInline (newdecl)
              && CTreeDeclInitial(olddecl) != 0)
          {
            warning_with_decl (newdecl,
                               "`%s' declared inline after its definition");
          }

          /* If pedantic, warn when static declaration follows a non-static
             declaration.  Otherwise, do so only for functions.  */
          if ((c_pedantic || CTreeDeclCode(olddecl) == C_FUNCTION_DECL)
              && CIsTreeNodePublic(olddecl)
              && !CIsTreeNodePublic(newdecl))
          {
            warning_with_decl (newdecl, "static declaration for `%s' follows non-static");
          }

          /* Warn when const declaration follows a non-const
             declaration, but not for functions.  */
          if ( CTreeDeclCode(olddecl) != C_FUNCTION_DECL
              && ! CIsTreeNodeReadOnly(olddecl)
              &&  CIsTreeNodeReadOnly(newdecl))
          {
            warning_with_decl (newdecl, "const declaration for `%s' follows non-const");
          }
          /* These bits are logically part of the type, for variables.
             But not for functions
             (where qualifiers are not valid ANSI anyway).  */
          else if (c_pedantic && CTreeDeclCode(olddecl) != C_FUNCTION_DECL
              && ( CIsTreeNodeReadOnly(newdecl) != CIsTreeNodeReadOnly(olddecl)
                  || CIsTreeNodeVolatile(newdecl) != CIsTreeNodeVolatile(olddecl)))
          {
            pedwarn_with_decl (newdecl, "type qualifiers for `%s' conflict with previous decl");
          }
        }
    }

  /* Optionally warn about more than one declaration for the same name.  */
  if (errmsg == 0 && c_warn_redundant_decls && CTreeDeclLineNum(olddecl) != 0
      /* Don't warn about a function declaration
         followed by a definition.  */
      && !( CTreeDeclCode(newdecl) == C_FUNCTION_DECL && CTreeDeclInitial(newdecl) != 0
           && CTreeDeclInitial(olddecl) == 0)
      /* Don't warn about extern decl followed by (tentative) definition.  */
      && !(CIsTreeDeclExternal(olddecl) && ! CIsTreeDeclExternal(newdecl)))
    {
      warning_with_decl (newdecl, "redundant redeclaration of `%s' in same scope");
      warning_with_decl (olddecl, "previous declaration of `%s'");
    }

  /* Copy all the DECL_... slots specified in the new decl
     except for any that we copy here from the old type.

     Past this point, we don't change OLDTYPE and NEWTYPE
     even if we change the types of NEWDECL and OLDDECL.  */

  if (types_match)
    {
      /* When copying info to olddecl, we store into write_olddecl
         instead.  This allows us to avoid modifying olddecl when
         different_binding_level is true.  */
      c_tree_node *write_olddecl = different_binding_level ? newdecl : olddecl;

      /* Make sure we put the new type in the same obstack as the old ones.
         If the old types are not both in the same obstack, use the permanent
         one.  */
# ifdef LUDO_NO_SKIP
      if (TYPE_OBSTACK (oldtype) == TYPE_OBSTACK (newtype))
        push_obstacks (TYPE_OBSTACK (oldtype), TYPE_OBSTACK (oldtype));
      else
        {
          push_obstacks_nochange ();
          end_temporary_allocation ();
        }
                       
      /* Merge the data types specified in the two decls.  */
      if ( CTreeDeclCode(newdecl) != C_FUNCTION_DECL || ! 
           CIsTreeDeclBuiltIn(olddecl))
        {
          if (different_binding_level)
            CTreeType(newdecl)
              = c_tree_build_type_attribute_variant
                (newtype,
                 merge_attributes (TYPE_ATTRIBUTES (newtype),
                                   TYPE_ATTRIBUTES (oldtype)));
          else
            CTreeType(newdecl)
              = CTreeType(olddecl)
                = common_type (newtype, oldtype);
        }
# endif

      /* Lay the type out, unless already done.  */
      if (oldtype != CTreeType(newdecl))
        {
          if (CTreeType(newdecl) != c_error_mark_node)
            c_layout_type(CTreeType(newdecl));
          if ( CTreeDeclCode(newdecl) != C_FUNCTION_DECL
              && CTreeDeclCode(newdecl) != C_TYPE_DECL
              && CTreeDeclCode(newdecl) != C_CONST_DECL)
            c_layout_decl(newdecl, 0);
        }
      else
        {
          /* Since the type is OLDDECL's, make OLDDECL's size go with.  */
          CTreeDeclSize(newdecl) = CTreeDeclSize(olddecl);
# ifdef LUDO_NO_SKIP
          if ( CTreeDeclCode(olddecl) != C_FUNCTION_DECL)
            if (DECL_ALIGN (olddecl) > DECL_ALIGN (newdecl))
              DECL_ALIGN (newdecl) = DECL_ALIGN (olddecl);
# endif
        }

# ifdef LUDO_NO_SKIP
      /* Keep the old rtl since we can safely use it.  */
      DECL_RTL (newdecl) = DECL_RTL (olddecl);

      /* Merge the type qualifiers.  */
      if (DECL_BUILT_IN_NONANSI (olddecl) && TREE_THIS_VOLATILE (olddecl)
          && !TREE_THIS_VOLATILE (newdecl))
        TREE_THIS_VOLATILE (write_olddecl) = 0;
# endif
      if ( CIsTreeNodeReadOnly(newdecl))
         CSetTreeNodeReadOnly(write_olddecl);
# ifdef LUDO_NO_SKIP
      if (TREE_THIS_VOLATILE (newdecl))
        {
          TREE_THIS_VOLATILE (write_olddecl) = 1;
          if (TREE_CODE (newdecl) == VAR_DECL)
            make_var_volatile (newdecl);
        }
# endif

      /* Keep source location of definition rather than declaration.  */
      /* When called with different_binding_level set, keep the old
         information so that meaningful diagnostics can be given.  */
      if (CTreeDeclInitial(newdecl) == 0 && CTreeDeclInitial(olddecl) != 0
          && ! different_binding_level)
        {
          CTreeDeclLineNum(newdecl) = CTreeDeclLineNum(olddecl);
          CTreeDeclFileName(newdecl) = CTreeDeclFileName(olddecl);
        }

      /* Merge the unused-warning information.  */
      if (CIsTreeDeclSystemHeader(olddecl))
        CSetTreeDeclSystemHeader(newdecl);
      else if (CIsTreeDeclSystemHeader(newdecl))
        CSetTreeDeclSystemHeader(write_olddecl);

      /* Merge the initialization information.  */
      /* When called with different_binding_level set, don't copy over
         DECL_INITIAL, so that we don't accidentally change function
         declarations into function definitions.  */
      if (CTreeDeclInitial(newdecl) == 0 && ! different_binding_level)
        CTreeDeclInitial(newdecl) = CTreeDeclInitial(olddecl);

      /* Merge the section attribute.
         We want to issue an error if the sections conflict but that must be
         done later in decl_attributes since we are called before attributes
         are assigned.  */
# ifdef LUDO_NO_SKIP
      if (DECL_SECTION_NAME (newdecl) == NULL_TREE)
        DECL_SECTION_NAME (newdecl) = DECL_SECTION_NAME (olddecl);

      if (CTreeDeclCode(newdecl) == C_FUNCTION_DECL)
        {
          DECL_STATIC_CONSTRUCTOR(newdecl) |= DECL_STATIC_CONSTRUCTOR(olddecl);
          DECL_STATIC_DESTRUCTOR (newdecl) |= DECL_STATIC_DESTRUCTOR (olddecl);
        }

      pop_obstacks ();
# endif
    }
  /* If cannot merge, then use the new type and qualifiers,
     and don't preserve the old rtl.  */
  else if (! different_binding_level)
    {
      CTreeType(olddecl) = CTreeType(newdecl);
      if ( CIsTreeNodeReadOnly(newdecl) )
      {
       CSetTreeNodeReadOnly(olddecl);
      }

      if ( CIsTreeNodeVolatile(newdecl) )
      {
        CSetTreeNodeVolatile(olddecl);
      }

      if ( CIsTreeNodeSideEffect(newdecl) )
      {
        CSetTreeNodeSideEffect(olddecl);
      }
    }

  /* Merge the storage class information.  */
  if ( CIsTreeDeclWeak( olddecl ) ) CSetTreeDeclWeak( newdecl );

  /* For functions, static overrides non-static.  */
  if ( CTreeDeclCode(newdecl) == C_FUNCTION_DECL)
    {
      if ( ! CIsTreeNodePublic(newdecl) ||
           ! CIsTreeNodePublic(olddecl) )
      {
        CClearTreeNodePublic(newdecl);
      }

      /* This is since we don't automatically
         copy the attributes of NEWDECL into OLDDECL.  */
      /* No need to worry about different_binding_level here because
         then TREE_PUBLIC (newdecl) was true.  */
      if ( CIsTreeNodePublic(newdecl) ) CSetTreeNodePublic(olddecl);
      else                              CClearTreeNodePublic(olddecl);

      /* If this clears `static', clear it in the identifier too.  */
      if (! CIsTreeNodePublic(olddecl))
      {
        CClearTreeNodePublic(CTreeDeclName(olddecl));
      }
    }
  if ( CIsTreeNodeExternal(newdecl))
    {
      if ( CIsTreeNodeStatic(olddecl) ) CSetTreeNodeStatic(newdecl);
      else                              CClearTreeNodeStatic(newdecl);

      if ( CIsTreeNodeExternal(olddecl) ) CSetTreeNodeExternal(newdecl);
      else                                CClearTreeNodeExternal(newdecl);

      /* An extern decl does not override previous storage class.  */
      if ( CIsTreeNodePublic(olddecl) ) CSetTreeNodePublic(newdecl);
      else                              CClearTreeNodePublic(newdecl);

      if (! CIsTreeDeclExternal(newdecl))
      {
        CTreeDeclContext(newdecl) = CTreeDeclContext(olddecl);
      }
    }
  else
    {
      if ( CIsTreeNodeStatic(newdecl) ) CSetTreeNodeStatic(olddecl);
      else                              CClearTreeNodeStatic(olddecl);

      if ( CIsTreeNodePublic(newdecl) ) CSetTreeNodePublic(olddecl);
      else                              CClearTreeNodePublic(olddecl);

    }

  /* If either decl says `inline', this fn is inline,
     unless its definition was passed already.  */
  if ( CIsTreeDeclInline(newdecl) && CTreeDeclInitial(olddecl) == 0)
    CSetTreeDeclInline(olddecl);

  if ( CIsTreeDeclInline(olddecl) ) CSetTreeDeclInline( newdecl );
  else                              CClearTreeDeclInline( newdecl );

  if ( CTreeDeclCode(newdecl) == C_FUNCTION_DECL)
    {
      if ( CIsTreeDeclBuiltIn(olddecl))
        {
          /* Get rid of any built-in function if new arg types don't match it
             or if we have a function definition.  */
          if (! types_match || new_is_definition)
            {
              if (! different_binding_level)
                {
                  CTreeType(olddecl) = CTreeType(newdecl);
                  CClearTreeDeclBuiltIn(olddecl);
                }
            }
          else
            {
              /* If redeclaring a builtin function, and not a definition,
                 it stays built in.  */
              CSetTreeDeclBuiltIn(newdecl);
# ifdef LUDO_NO_SKIP
              DECL_FUNCTION_CODE (newdecl) = DECL_FUNCTION_CODE (olddecl);
# endif
            }
        }
      /* Also preserve various other info from the definition.  */
      else if (! new_is_definition)
      {
# ifdef LUDO_NO_SKIP
        DECL_FRAME_SIZE (newdecl) = DECL_FRAME_SIZE (olddecl);
# endif
      }
      if (! new_is_definition)
        {
          CTreeDeclResult(newdecl) = CTreeDeclResult(olddecl);
          /* When called with different_binding_level set, don't copy over
             DECL_INITIAL, so that we don't accidentally change function
             declarations into function definitions.  */
          if (! different_binding_level)
            CTreeDeclInitial(newdecl) = CTreeDeclInitial(olddecl);
# ifdef LUDO_NO_SKIP
          DECL_SAVED_INSNS (newdecl) = DECL_SAVED_INSNS (olddecl);
# endif
          CTreeDeclArguments(newdecl) = CTreeDeclArguments(olddecl);

          if ( CIsTreeDeclInline(newdecl))
            CTreeDeclAbstractOrigin(newdecl) = olddecl;
        }
    }
  if (different_binding_level)
    {
      /* Don't output a duplicate symbol or debugging information for this
         declaration.  */
      CSetTreeNodeAsmWritten(newdecl);
      CSetTreeDeclIgnored(newdecl);
      return 0;
    }

  /* Copy most of the decl-specific fields of NEWDECL into OLDDECL.
     But preserve OLDDECL's DECL_UID.  */
  {
    unsigned olddecl_uid = CTreeDeclId(olddecl);

    bcopy ((char *) newdecl + sizeof (c_tree_common),
           (char *) olddecl + sizeof (c_tree_common),
           sizeof (c_tree_decl) - sizeof (c_tree_common));

    CTreeDeclId(olddecl) = olddecl_uid;
  }

  return 1;
}


/*------------------------------------------------------------\
|                                                             |
|                    Push  Declaration                        |
|                                                             |
\------------------------------------------------------------*/


/* Return nonzero if ELEM is part of the chain CHAIN.  */

int
c_is_chain_member(elem, chain)
     c_tree_node *elem;
     c_tree_node *chain;
{
  while (chain)
    {
      if (elem == chain)
        return 1;
      chain = CTreeChain(chain);
    }

  return 0;
}

c_tree_node *c_push_decl( TreeDecl )

   c_tree_node *TreeDecl;
{
  c_binding_level *Binding;
  c_tree_node     *TreeName;
  c_tree_node     *TreeNode;
  
  TreeName = TreeDecl->DECL.NAME;
  Binding  = c_current_binding_level;

  CTreeDeclContext( TreeDecl ) = c_current_function_decl;
  /* A local extern declaration for a function doesn't constitute nesting.
     A local auto declaration does, since it's a forward decl
     for a nested function coming later.  */
  if ( ( CTreeDeclCode( TreeDecl )    == C_FUNCTION_DECL ) && 
       ( CTreeDeclInitial( TreeDecl ) == 0                 ) &&
       ( CIsTreeDeclExternal( TreeDecl )                      ) )
  {
    CTreeDeclContext( TreeDecl ) = (c_tree_node *)0;
  }

  if ( c_warn_nested_externs && CIsTreeDeclExternal( TreeDecl ) && 
       Binding != c_global_binding_level
      && TreeDecl != CTreeIdentImplicitDecl( TreeName )
      /* Don't print error messages for __FUNCTION__ and __PRETTY_FUNCTION__ */
      && !CIsTreeDeclSystemHeader( TreeDecl ))
  {
    warning ("nested extern declaration of `%s'", CTreeIdentPointer( TreeName ));
  }

  if ( TreeName != (c_tree_node *)0 )
  {
    char *file;
    int line;
    int different_binding_level = 0;

    TreeNode = c_lookup_name_current_level( TreeName );

    /* Don't type check externs here when -traditional.  This is so that
       code with conflicting declarations inside blocks will get warnings
       not errors.  X11 for instance depends on this.  */
    if (! TreeNode && CIsTreeDeclExternal( TreeDecl ) && 
         CIsTreeNodePublic( TreeDecl ) && ! c_flag_traditional)
      {
        TreeNode = CTreeIdentGlobalValue( TreeName );
        /* Type decls at global scope don't conflict with externs declared
           inside lexical blocks.  */
        if ( TreeNode && CTreeDeclCode( TreeNode ) == C_TYPE_DECL)
           TreeNode  = 0;
        different_binding_level = 1;
      }
    if ( TreeNode != 0 &&  TreeNode == c_error_mark_node)
      /* error_mark_node is 0 for a while during initialization!  */
      {
        TreeNode = 0;
        error_with_decl ( TreeDecl, "`%s' used prior to declaration");
      }

    if (TreeNode != (c_tree_node *)0)
      {
        file = CTreeDeclFileName( TreeNode );
        line = CTreeDeclLineNum( TreeNode );
      }

    /* If this decl is `static' and an implicit decl was seen previously,
       warn.  But don't complain if -traditional,
       since traditional compilers don't complain.  */
    if (! c_flag_traditional && CIsTreeNodePublic( TreeName )
        /* Don't test for DECL_EXTERNAL, because grokdeclarator
           sets this for all functions.  */
        && ! CIsTreeNodePublic( TreeDecl )
        && ( CTreeDeclCode( TreeDecl ) == C_FUNCTION_DECL || Binding == c_global_binding_level)
        /* We used to warn also for explicit extern followed by static,
           but sometimes you need to do it that way.  */
        && CTreeIdentImplicitDecl( TreeName ) != 0)
      {
        pedwarn ("`%s' was declared implicitly `extern' and later `static'",
                 CTreeIdentPointer( TreeName ));
        pedwarn_with_file_and_line(
            CTreeDeclFileName( CTreeIdentImplicitDecl( TreeName )),
           CTreeDeclLineNum( CTreeIdentImplicitDecl( TreeName )),
           "previous declaration of `%s'",
           CTreeIdentPointer( TreeName ));
      }

    if ( TreeNode != 0 && c_is_duplicate_decls( TreeDecl, TreeNode, different_binding_level))
      {
        if ( CTreeDeclCode( TreeNode ) == C_PARAM_DECL)
          {
            /* Don't allow more than one "real" duplicate
               of a forward parm decl.  */
            if ( CIsTreeNodeAsmWritten( TreeDecl ) ) CSetTreeNodeAsmWritten( TreeNode );

            return TreeNode;
          }
        return TreeNode;
      }

    /* If we are processing a typedef statement, generate a whole new
       ..._TYPE node (which will be just an variant of the existing
       ..._TYPE node with identical properties) and then install the
       TYPE_DECL node generated to represent the typedef name as the
       TYPE_NAME of this brand new (duplicate) ..._TYPE node.

       The whole point here is to end up with a situation where each
       and every ..._TYPE node the compiler creates will be uniquely
       associated with AT MOST one node representing a typedef name.
       This way, even though the compiler substitutes corresponding
       ..._TYPE nodes for TYPE_DECL (i.e. "typedef name") nodes very
       early on, later parts of the compiler can always do the reverse
       translation and get back the corresponding typedef name.  For
       example, given:

              typedef struct S MY_TYPE;
              MY_TYPE object;

       Later parts of the compiler might only know that `object' was of
       type `struct S' if if were not for code just below.  With this
       code however, later parts of the compiler see something like:

              struct S' == struct S
              typedef struct S' MY_TYPE;
              struct S' object;

       And they can then deduce (from the node for type struct S') that
       the original object declaration was:

              MY_TYPE object;

       Being able to do this is important for proper support of protoize,
       and also for generating precise symbolic debugging information
       which takes full account of the programmer's (typedef) vocabulary.

       Obviously, we don't want to generate a duplicate ..._TYPE node if
       the TYPE_DECL node that we are now processing really represents a
       standard built-in type.

       Since all standard types are effectively declared at line zero
       in the source file, we can easily check to see if we are working
       on a standard type by checking the current value of lineno.  */

      if ( CTreeDeclCode( TreeDecl ) == C_TYPE_DECL)
      {
        if ( CTreeDeclLineNum( TreeDecl ) == 0)
        {
          if ( CTreeTypeName( CTreeType( TreeDecl ) ) == (c_tree_node *)0)
          {
            CTreeTypeName( CTreeType( TreeDecl ) ) = TreeDecl;
           }
        }
        else if ( CTreeType( TreeDecl ) != c_error_mark_node
                 && CTreeDeclResult( TreeDecl ) == NULL)
          {
            c_tree_node *tt = CTreeType( TreeDecl );
            CTreeDeclResult( TreeDecl ) = tt;
            tt = c_tree_build_type_copy(tt);
            CTreeTypeName(tt) =  TreeDecl ;
            CTreeType( TreeDecl ) = tt;
          }
      }

    /* Multiple external decls of the same identifier ought to match.
       Check against both global declarations (when traditional) and out of
       scope (limbo) block level declarations.

       We get warnings about inline functions where they are defined.
       Avoid duplicate warnings where they are used.  */
    if ( CIsTreeNodePublic( TreeDecl )
        && ! CIsTreeDeclInline( TreeDecl )
        )
      {
        c_tree_node *decl;

        if (c_flag_traditional && CTreeIdentGlobalValue( TreeName ) != 0
            && ( CIsTreeDeclExternal( CTreeIdentGlobalValue( TreeName ))
                || CIsTreeNodePublic( CTreeIdentGlobalValue( TreeName ))))
          decl =  CTreeIdentGlobalValue( TreeName );
        else if ( CTreeIdentLimboValue( TreeName ) != 0)
          /* Decls in limbo are always extern, so no need to check that.  */
          decl =  CTreeIdentLimboValue( TreeName );
        else
          decl = 0;

        if (decl && ! c_is_comptypes( CTreeType( TreeDecl ), CTreeType(decl))
            /* If old decl is built-in, we already warned if we should.  */
            && ! CIsTreeDeclBuiltIn(decl))
          {
            pedwarn_with_decl ( TreeDecl,
                               "type mismatch with previous external decl");
            pedwarn_with_decl (decl, "previous external decl of `%s'");
          }
      }

    /* If a function has had an implicit declaration, and then is defined,
       make sure they are compatible.  */

    if ( CTreeIdentImplicitDecl( TreeName ) != 0
        && CTreeIdentGlobalValue( TreeName ) == 0
        && CTreeDeclCode( TreeDecl ) == C_FUNCTION_DECL
        && ! c_is_comptypes( CTreeType( TreeDecl ),
                        CTreeType( CTreeIdentImplicitDecl( TreeName ))))
      {
        warning_with_decl( TreeDecl, "type mismatch with previous implicit declaration");
        warning_with_decl( CTreeIdentImplicitDecl( TreeName ),
                           "previous implicit declaration of `%s'");
      }

    /* In PCC-compatibility mode, extern decls of vars with no current decl
       take effect at top level no matter where they are.  */
    if (c_flag_traditional && CIsTreeDeclExternal( TreeDecl )
        && c_lookup_name( TreeName ) == 0)
      {
        c_tree_node *type = CTreeType( TreeDecl );

        /* But don't do this if the type contains temporary nodes.  */
        while (type)
          {
            if (type == c_error_mark_node)
              break;
            if (! CIsTreeNodePermanent(type))
              {
                warning_with_decl( TreeDecl, "type of external `%s' is not global");
                /* By exiting the loop early, we leave TYPE nonzero,
                   and thus prevent globalization of the decl.  */
                break;
              }
            else if ( CTreeDeclCode(type) == C_FUNCTION_TYPE
                     && CTreeTypeValues(type) != 0)
              /* The types might not be truly local,
                 but the list of arg types certainly is temporary.
                 Since prototypes are nontraditional,
                 ok not to do the traditional thing.  */
              break;
            type = CTreeType(type);
          }

        if (type == 0)
          Binding = c_global_binding_level;
      }

    /* This name is new in its binding level.
       Install the new declaration and return it.  */
    if ( Binding == c_global_binding_level)
      {
        /* Install a global value.  */
        
        /* If the first global decl has external linkage,
           warn if we later see static one.  */
        if (CTreeIdentGlobalValue( TreeName ) == 0 && CIsTreeNodePublic( TreeDecl ))
          CSetTreeNodePublic( TreeName );

        CTreeIdentGlobalValue( TreeName ) = TreeDecl;

        /* We no longer care about any previous block level declarations.  */
        CTreeIdentLimboValue( TreeName ) = 0;

        /* Don't forget if the function was used via an implicit decl.  */
        if (CTreeIdentImplicitDecl( TreeName )
            && CIsTreeNodeUsed(CTreeIdentImplicitDecl( TreeName )))
        {
          CSetTreeNodeUsed( TreeDecl );
          CSetTreeNodeUsed( TreeName );
        }

        /* Don't forget if its address was taken in that way.  */
        if (CTreeIdentImplicitDecl( TreeName )
            && CIsTreeNodeAddressable(CTreeIdentImplicitDecl( TreeName )))
        {
          CSetTreeNodeAddressable( TreeDecl );
        }

        /* Warn about mismatches against previous implicit decl.  */
        if (CTreeIdentImplicitDecl( TreeName ) != 0
            /* If this real decl matches the implicit, don't complain.  */
            && ! ( CTreeDeclCode( TreeDecl ) == C_FUNCTION_DECL
                  && (CTreeTypeMainVariant( CTreeType( CTreeType( TreeDecl )))
                      == c_integer_type_node)))
        {
          pedwarn ("`%s' was previously implicitly declared to return `int'",
                   CTreeIdentPointer( TreeName ));
        }

        /* If this decl is `static' and an `extern' was seen previously,
           that is erroneous.  */
        if ( CIsTreeNodePublic( TreeName )
            && ! CIsTreeNodePublic( TreeDecl ) && ! CIsTreeDeclExternal( TreeDecl ))
          {
            /* Okay to redeclare an ANSI built-in as static.  */
            if ( TreeNode != 0 && CIsTreeDeclBuiltIn( TreeNode ))
              ;
            /* Okay to declare a non-ANSI built-in as anything.  */
# ifdef LUDO_NO_SKIP
            else if (t != 0 && DECL_BUILT_IN_NONANSI (t))
              ;
# endif
            /* Okay to have global type decl after an earlier extern
               declaration inside a lexical block.  */
            else if ( CTreeDeclCode( TreeDecl ) == C_TYPE_DECL)
              ;
            else if (CTreeIdentImplicitDecl( TreeName ))
            {
              pedwarn ("`%s' was declared implicitly `extern' and later `static'",
                       CTreeIdentPointer( TreeName ));
            }
            else
            {
              pedwarn ("`%s' was declared `extern' and later `static'",
                       CTreeIdentPointer( TreeName ));
            }
          }
      }
    else
      {
        /* Here to install a non-global value.  */
        c_tree_node * oldlocal = CTreeIdentGlobalValue( TreeName );
        c_tree_node * oldglobal = CTreeIdentGlobalValue( TreeName );
        CTreeIdentLocalValue( TreeName ) = TreeDecl;

        /* If this is an extern function declaration, see if we
           have a global definition or declaration for the function.  */
        if (oldlocal == 0
            && CIsTreeDeclExternal( TreeDecl )
            && !CIsTreeDeclInline( TreeDecl )
            && oldglobal != 0
            && CTreeDeclCode( TreeDecl ) == C_FUNCTION_DECL
            && CTreeDeclCode(oldglobal) == C_FUNCTION_DECL)
          {
            /* We have one.  Their types must agree.  */
            if (! c_is_comptypes( CTreeType( TreeDecl ),
                             CTreeType( CTreeIdentGlobalValue( TreeName ))))
            {
              pedwarn_with_decl( TreeDecl,
                  "extern declaration of `%s' doesn't match global one");
            }
            else
              {
                /* Inner extern decl is inline if global one is.
                   Copy enough to really inline it.  */
                if ( CIsTreeDeclInline(oldglobal))
                  {
                    CSetTreeDeclInline( TreeDecl );
                    CTreeDeclInitial( TreeDecl ) = (c_current_function_decl == oldglobal
                                        ? 0 : CTreeDeclInitial(oldglobal));
# ifdef LUDO_NO_SKIP
                    DECL_SAVED_INSNS ( TreeDecl ) = DECL_SAVED_INSNS (oldglobal);
                    DECL_FRAME_SIZE ( TreeDecl ) = DECL_FRAME_SIZE (oldglobal);
# endif
                    CTreeDeclArguments( TreeDecl ) = CTreeDeclArguments(oldglobal);
                    CTreeDeclResult( TreeDecl ) = CTreeDeclResult(oldglobal);
                    if ( CIsTreeNodeAsmWritten(oldglobal))
                      CSetTreeNodeAsmWritten( TreeDecl );
                    CTreeDeclAbstractOrigin( TreeDecl ) = oldglobal;
                  }
                /* Inner extern decl is built-in if global one is.  */
                if ( CIsTreeDeclBuiltIn(oldglobal))
                  {
                    CSetTreeDeclBuiltIn( TreeDecl );
# ifdef LUDO_NO_SKIP
                    DECL_FUNCTION_CODE ( TreeDecl ) = DECL_FUNCTION_CODE (oldglobal);
# endif
                  }
                /* Keep the arg types from a file-scope fcn defn.  */
                if ( CTreeTypeValues(CTreeType(oldglobal)) != 0
                    && CTreeDeclInitial(oldglobal)
                    && CTreeTypeValues(CTreeType( TreeDecl )) == 0)
                {
                  CTreeType( TreeDecl ) = CTreeType(oldglobal);
                }
              }
          }

        /* If we have a local external declaration,
           and no file-scope declaration has yet been seen,
           then if we later have a file-scope decl it must not be static.  */
        if (oldlocal == 0
            && CIsTreeDeclExternal( TreeDecl )
            && CIsTreeNodePublic( TreeDecl ))
          {
            if (oldglobal == 0)
              CSetTreeNodePublic( TreeName );

            /* Save this decl, so that we can do type checking against
               other decls after it falls out of scope.

               Only save it once.  This prevents temporary decls created in
               expand_inline_function from being used here, since this
               will have been set when the inline function was parsed.
               It also helps give slightly better warnings.  */
            if (CTreeIdentLimboValue( TreeName ) == 0)
              CTreeIdentLimboValue( TreeName ) = TreeDecl;
          }

        /* Warn if shadowing an argument at the top level of the body.  */
        if (oldlocal != 0 && !CIsTreeDeclExternal( TreeDecl )
            /* This warning doesn't apply to the parms of a nested fcn.  */
            && ! c_current_binding_level->PARAM_FLAG
            /* Check that this is one level down from the parms.  */
            && c_current_binding_level->LEVEL_CHAIN->PARAM_FLAG
            /* Check that the decl being shadowed
               comes from the parm level, one level up.  */
            && c_is_chain_member(oldlocal, c_current_binding_level->LEVEL_CHAIN->NAME_LIST))
          {
            if ( CTreeDeclCode(oldlocal) == C_PARAM_DECL)
            {
              pedwarn ("declaration of `%s' shadows a parameter",
                       CTreeIdentPointer( TreeName ));
            }
            else
            {
              pedwarn ("declaration of `%s' shadows a symbol from the parameter list",
                       CTreeIdentPointer( TreeName ));
            }
          }

        /* Maybe warn if shadowing something else.  */
        else if (c_warn_shadow && !CIsTreeDeclExternal( TreeDecl )
                 /* No shadow warnings for internally generated vars.  */
                 && CTreeDeclLineNum( TreeDecl ) != 0
                 /* No shadow warnings for vars made for inlining.  */
                 && ! (CTreeDeclAbstractOrigin( TreeDecl ) != NULL)
                 )
          {
            char *warnstring = 0;

            if ( CTreeDeclCode( TreeDecl ) == C_PARAM_DECL
                && c_current_binding_level->LEVEL_CHAIN->PARAM_FLAG)
              /* Don't warn about the parm names in function declarator
                 within a function declarator.
                 It would be nice to avoid warning in any function
                 declarator in a declaration, as opposed to a definition,
                 but there is no way to tell it's not a definition.  */
              ;
            else if (oldlocal != 0 && CTreeDeclCode(oldlocal) == C_PARAM_DECL)
            {
              warnstring = "declaration of `%s' shadows a parameter";
            }
            else if (oldlocal != 0)
            {
              warnstring = "declaration of `%s' shadows previous local";
            }
            else if ( CTreeIdentGlobalValue( TreeName ) != 0
                     &&  CTreeIdentGlobalValue( TreeName ) != c_error_mark_node)
            {
              warnstring = "declaration of `%s' shadows global declaration";
            }

            if (warnstring)
            {
              warning (warnstring, CTreeIdentPointer( TreeName ));
            }
          }

        /* If storing a local value, there may already be one (inherited).
           If so, record it for restoration when this binding level ends.  */
        if (oldlocal != 0)
           Binding->SHADOWED_LIST = 
             c_tree_add_head_list( TreeName , oldlocal,  Binding->SHADOWED_LIST);
      }

    /* Keep count of variables in this level with incomplete type.  */
    if ( CTreeTypeSize(CTreeType( TreeDecl )) == 0)
      ++ Binding->N_INCOMPLETE;
  }

  /* Put decls on list in reverse order.
     We will reverse them later if necessary.  */

  TreeDecl->COMMON.CHAIN = Binding->NAME_LIST;
  Binding->NAME_LIST     = TreeDecl;

  return( TreeDecl );
}


/* Decode the parameter-list info for a function type or function definition.
   The argument is the value returned by `get_parm_info' (or made in parse.y
   if there is an identifier list instead of a parameter decl list).
   These two functions are separate because when a function returns
   or receives functions then each is called multiple times but the order
   of calls is different.  The last call to `grokparms' is always the one
   that contains the formal parameter names of a function definition.

   Store in `last_function_parms' a chain of the decls of parms.
   Also store in `last_function_parm_tags' a chain of the struct, union,
   and enum tags declared among the parms.

   Return a list of arg types to use in the FUNCTION_TYPE for this function.

   FUNCDEF_FLAG is nonzero for a function definition, 0 for
   a mere declaration.  A nonempty identifier-list gets an error message
   when FUNCDEF_FLAG is zero.  */

static c_tree_node *c_grokparms(parms_info, Funcdef_flag)
     c_tree_node *parms_info;
     int Funcdef_flag;
{
  c_tree_node *first_parm = CTreeChain(parms_info);

  c_last_function_parms = CTreeListPurpose(parms_info);
  c_last_function_parm_tags = CTreeListValue(parms_info);

  if (c_warn_strict_prototypes && first_parm == 0 && !Funcdef_flag
      && !c_in_system_header)
  {
    warning ("function declaration isn't a prototype");
  }

  if (first_parm != 0
      &&  CTreeNodeFirstCode( CTreeListValue(first_parm)) == C_IDENTIFIER_NODE)
    {
      if (! Funcdef_flag)
      {
        pedwarn ("parameter names (without types) in function declaration");
      }

      c_last_function_parms = first_parm;
      return 0;
    }
  else
    {
      c_tree_node *parm;
      c_tree_node *typelt;
      /* We no longer test FUNCDEF_FLAG.
         If the arg types are incomplete in a declaration,
         they must include undefined tags.
         These tags can never be defined in the scope of the declaration,
         so the types can never be completed,
         and no call can be compiled successfully.  */

        for (parm = c_last_function_parms, typelt = first_parm;
             parm;
             parm = CTreeChain(parm))
        {
          /* Skip over any enumeration constants declared here.  */
          if ( CTreeDeclCode(parm) == C_PARAM_DECL)
            {
              /* Barf if the parameter itself has an incomplete type.  */
              c_tree_node *type = CTreeListValue(typelt);
              if (CTreeTypeSize(type) == 0)
                {
                  if (Funcdef_flag && CTreeDeclName(parm) != 0)
                  {
                    error ("parameter `%s' has incomplete type",
                           CTreeIdentPointer(CTreeDeclName(parm)));
                  }
                  else
                  {
                    warning ("parameter has incomplete type");
                  }
                  if (Funcdef_flag)
                    {
                      CTreeListValue(typelt) = c_error_mark_node;
                      CTreeType(parm) = c_error_mark_node;
                    }
                }
              typelt = CTreeChain(typelt);
            }
        }

      /* Allocate the list of types the way we allocate a type.  */
      if (first_parm && ! CIsTreeNodePermanent(first_parm))
        {
          /* Construct a copy of the list of types
             on the saveable obstack.  */
          c_tree_node *result = NULL;

          for (typelt = first_parm; typelt; typelt = CTreeChain(typelt))
          {
            result = c_tree_add_head_list( NULL, CTreeListValue(typelt), result);
          }

          return c_tree_reverse_list(result);
        }
      else
        /* The list we have is permanent already.  */
      {
        return first_parm;
      }
    }
}

/*------------------------------------------------------------\
|                                                             |
|                    Grant Check Declaration                  |
|                                                             |
\------------------------------------------------------------*/

static c_tree_node *c_grokdeclarator( Declarator, Declspecs, Decl_context, Initialized )

  c_tree_node *Declarator;
  c_tree_node *Declspecs;
  int            Decl_context;
  int            Initialized;
{
  c_tree_node *TreeDecl;
  c_tree_node *ScanDeclarator;
  c_tree_node *ScanSpec;
  c_tree_node *TreeId;
  c_tree_node *TreeNode;
  c_tree_node *TreeType      = (c_tree_node *)0;
  c_tree_node *Typedef_decl  = (c_tree_node *)0;
  c_tree_node *Typedef_type  = (c_tree_node *)0;

  char          *Name;

  int            Explicit_int  = 0;
  int            Explicit_char = 0;
  int            Defaulted_int = 0;
  int            Specbits      = 0;
  int            Longlong      = 0;
  int            Funcdef_flag  = 0;
  int            Bitfield      = 0;
  int            Size_varies   = 0;

  int            Constp;
  int            Volatilep;
  int            Inlinep;
  int            Innermost_code = C_ERROR_NODE;
  int            NodeType;
  int            ExprType;
  int            TreeIdType;
  int            Index;

# ifdef LUDO_NO_SKIP
  c_tree_node *spec;
  tree typedef_type = 0;
  tree decl_machine_attr = NULL;
# endif

  if ( Decl_context == C_CDECL_BIT_FIELD)
  {
    Bitfield = 1; Decl_context = C_CDECL_FIELD;
  }

  if ( Decl_context == C_CDECL_FUNCDEF)
  {
    Funcdef_flag = 1; Decl_context = C_CDECL_NORMAL;
  }

# ifdef LUDO_NO_SKIP
  push_obstacks_nochange ();
  if (c_flag_traditional && allocation_temporary_p ())
    end_temporary_allocation ();
# endif

  /* Look inside a declarator for the name being declared
     and get it as a string, for an error message.  */

  ScanDeclarator = Declarator;
  Name           = (char *)0;

  while ( ScanDeclarator != (c_tree_node *)0 )
  {
    NodeType = CTreeNodeFirstCode( ScanDeclarator );

    if ( NodeType == C_EXPR_NODE )
    {
      ExprType = CTreeExprCode( ScanDeclarator );

      if ( ( ExprType == C_ARRAY_REF    ) ||
           ( ExprType == C_INDIRECT_REF ) ||
           ( ExprType == C_CALL_EXPR    ) )
      {
        Innermost_code = ExprType;
        ScanDeclarator = CTreeExprOperand( ScanDeclarator, 0 );
      }
    }
    else
    if ( NodeType == C_IDENTIFIER_NODE )
    {
      Name           = CTreeIdentPointer( ScanDeclarator );
      ScanDeclarator = (c_tree_node *)0;
    }
    else
    {
      abort();
    }
  }

  if ( Name == (char *)0 )
  {
    Name = "type name";
  }

  /* A function definition's declarator must have the form of
     a function declarator.  */

  if ( Funcdef_flag && ( Innermost_code != C_CALL_EXPR ) )
  {
    return( (c_tree_node *)0 );
  }

  /* Anything declared one level down from the top level
     must be one of the parameters of a function
     (because the body is at least two levels down).  */

  /* If this looks like a function definition, make it one,
     even if it occurs where parms are expected.
     Then store_parm_decls will reject it and not use it as a parm.  */

  if ( ( Decl_context == C_CDECL_NORMAL      ) && 
       ( ! Funcdef_flag                        ) &&
       ( c_current_binding_level->PARAM_FLAG ) )
  {
    Decl_context = C_CDECL_PARAM;
  }

  /* Look through the decl specs and record which ones appear.
     Some typespecs are defined as built-in typenames.
     Others, the ones that are modifiers of other types,
     are represented by bits in SPECBITS: set the bits for
     the modifiers that appear.  Storage class keywords are also in SPECBITS.

     If there is a typedef name or a type, store the type in TYPE.
     This includes builtin typedefs such as `int'.

     Set EXPLICIT_INT or EXPLICIT_CHAR if the type is `int' or `char'
     and did not come from a user typedef.

     Set LONGLONG if `long' is mentioned twice.  */

  for ( ScanSpec  = Declspecs; 
        ScanSpec != (c_tree_node *)0;
        ScanSpec  = CTreeChain( ScanSpec ) )
  {
    TreeId     = CTreeListValue( ScanSpec );
    TreeIdType = CTreeNodeFirstCode( TreeId ); 

    if ( TreeId == c_ridpointers[(int)RID_INT] )
    {
      Explicit_int = 1;
    }

    if ( TreeId == c_ridpointers[(int)RID_CHAR] )
    {
      Explicit_char = 1;
    }

    if ( TreeIdType == C_IDENTIFIER_NODE )
    {
      for ( Index = (int)RID_FIRST_MODIFIER; Index < (int) RID_MAX; Index++)
      {
        if ( c_ridpointers[ Index ] == TreeId )
        {
          if ( ( Index == (int)RID_LONG ) &&
               ( Specbits & (1<< Index) ) )
          {
            if ( Longlong )
            {
              error ("`long long long' is too long for GCC");
            }
            else
            {
              if ( ( c_pedantic ) && ( ! c_in_system_header ) )
              {
                pedwarn ("ANSI C does not support `long long'");
              }

              Longlong = 1;
            }
          }
          else
          if ( Specbits & (1 << Index) )
          {
            pedwarn ("duplicate `%s'", CTreeIdentPointer( TreeId ) );
          }

          Specbits |= 1 << Index;
          goto found;
        }
      }
    }

    if ( TreeType )
    {
      error("two or more data types in declaration of `%s'", Name);
    }
    /* Actual typedefs come to us as TYPE_DECL nodes.  */
    else if ( ( TreeIdType                == C_DECL_NODE ) &&
              ( CTreeDeclCode( TreeId ) == C_TYPE_DECL ) )
    {
      TreeType = CTreeType( TreeId );
# ifdef LUDO_NO_SKIP
      Decl_machine_attr = DECL_MACHINE_ATTRIBUTES (id);
# endif
      Typedef_decl = TreeId;
    }
    /* Built-in types come as identifiers.  */
    else
    if ( TreeIdType == C_IDENTIFIER_NODE )
    {
      TreeNode = c_lookup_name( TreeId );

      if ( ( TreeNode                       == (c_tree_node *)0 ) || 
           ( CTreeDeclCode( TreeNode ) != C_TYPE_DECL      ) )
      {
        error("`%s' fails to be a typedef or built in type", CTreeIdentPointer( TreeId ));
      }
      else
      if ( CTreeType( TreeNode ) != c_error_mark_node )
      {
        TreeType     = CTreeType( TreeNode );
        Typedef_decl = TreeNode;
      }
    }
    else
    if ( TreeIdType != C_ERROR_NODE )
    {
      TreeType = TreeId;
    }

    found: {}
  }

  Typedef_type = TreeType;

  if ( TreeType != (c_tree_node *)0 )
  {
    Size_varies = CIsTreeTypeVarSize(TreeType);
  }

  /* No type at all: default to `int', and set DEFAULTED_INT
     because it was not a user-defined typedef.  */

  if ( TreeType == (c_tree_node *)0 )
  {
    if (! (Specbits & ((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                       | (1 << (int) RID_SIGNED)
                       | (1 << (int) RID_UNSIGNED))))
      {
        /* C9x will probably require a diagnostic here.
           For now, issue a warning if -Wreturn-type and this is a function,
           or if -Wimplicit; prefer the former warning since it is more
           explicit.  */
        if ((c_warn_implicit_int || c_warn_return_type) && Funcdef_flag)
          c_warn_about_return_type = 1;
        else if (c_warn_implicit_int)
        {
          warning ("type defaults to `int' in declaration of `%s'", Name);
        }
      }

    Defaulted_int = 1;
    TreeType = c_integer_type_node;
  }

  /* Now process the modifiers that were specified and check for invalid
   * combinations.  */
  /* Long double is a special combination.  */

  if (( Specbits & 1 << (int) RID_LONG) &&
      (! Longlong) && 
      (CTreeTypeMainVariant( TreeType ) == c_double_type_node))
  {
    Specbits &= ~(1 << (int) RID_LONG);
    TreeType = c_long_double_type_node;
  }

  /* Check all other uses of type modifiers.  */

  if (Specbits & ((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                | (1 << (int) RID_UNSIGNED) | (1 << (int) RID_SIGNED)))
  {
    int Ok = 0;

    if ((Specbits & 1 << (int) RID_LONG)
        && (Specbits & 1 << (int) RID_SHORT))
    {
      error ("both long and short specified for `%s'", Name);
    }
    else if (((Specbits & 1 << (int) RID_LONG)
              || (Specbits & 1 << (int) RID_SHORT))
             && Explicit_char)
    {
      error ("long or short specified with char for `%s'", Name);
    }
    else if (((Specbits & 1 << (int) RID_LONG)
              || (Specbits & 1 << (int) RID_SHORT))
             && CTreeTypeCode( TreeType ) == C_REAL_TYPE)
    {
      static int Already = 0;

      error ("long or short specified with floating type for `%s'", Name);
      if (! Already && ! c_pedantic)
      {
        error ("the only valid combination is `long double'");
        Already = 1;
      }
    }
    else if ((Specbits & 1 << (int) RID_SIGNED)
             && (Specbits & 1 << (int) RID_UNSIGNED))
    {
      error ("both signed and unsigned specified for `%s'", Name);
    }
    else if ( CTreeTypeCode( TreeType ) != C_INTEGER_TYPE)
    {
      error ("long, short, signed or unsigned invalid for `%s'", Name);
    }
    else
    {
      Ok = 1;

      if (!Explicit_int && !Defaulted_int && !Explicit_char && c_pedantic)
      {
        pedwarn ("long, short, signed or unsigned used invalidly for `%s'", Name);

        if (c_flag_pedantic_errors) Ok = 0;
      }
    }

    /* Discard the type modifiers if they are invalid.  */
    if (! Ok)
    {
      Specbits &= ~((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                    | (1 << (int) RID_UNSIGNED) | (1 << (int) RID_SIGNED));
      Longlong = 0;
    }
  }

  if ((Specbits & (1 << (int) RID_COMPLEX)               ) && 
      (CTreeTypeCode( TreeType ) != C_INTEGER_TYPE) && 
      (CTreeTypeCode( TreeType ) != C_REAL_TYPE   ) )
  {
    error ("complex invalid for `%s'", Name);
    Specbits &= ~ (1 << (int) RID_COMPLEX);
  }

  /* Decide whether an integer type is signed or not.
     Optionally treat bitfields as signed by default.  */
  if ((Specbits & 1 << (int) RID_UNSIGNED) ||
      /* Traditionally, all bitfields are unsigned.  */
      (Bitfield && c_flag_traditional
          && (! c_explicit_flag_signed_bitfields || !c_flag_signed_bitfields)) ||
      (Bitfield && ! c_flag_signed_bitfields
          && (Explicit_int || Defaulted_int || Explicit_char
              /* A typedef for plain `int' without `signed'
                 can be controlled just like plain `int'.  */
# ifdef LUDO_NO_SKIP
              || ! (Typedef_decl != 0 && C_TYPEDEF_EXPLICITLY_SIGNED(typedef_decl))
# endif
          )
          && CTreeTypeCode( TreeType ) != C_ENUMERAL_TYPE
          && !(Specbits & 1 << (int) RID_SIGNED)))
  {
    if (Longlong)
    {
      TreeType = c_long_long_unsigned_type_node;
    }
    else if (Specbits & 1 << (int) RID_LONG)
    {
      TreeType = c_long_unsigned_type_node;
    }
    else if (Specbits & 1 << (int) RID_SHORT)
    {
      TreeType = c_short_unsigned_type_node;
    }
    else if (TreeType == c_char_type_node)
    {
      TreeType = c_unsigned_char_type_node;
    }
    else if ( Typedef_decl != (c_tree_node *)0 )
    {
      TreeType = c_get_unsigned_type(TreeType);
    }
    else
    {
      TreeType = c_unsigned_type_node;
    }
  }
  else if ((Specbits & 1 << (int) RID_SIGNED) && (TreeType == c_char_type_node))
  {
    TreeType = c_signed_char_type_node;
  }
  else if (Longlong)
  {
    TreeType = c_long_long_integer_type_node;
  }
  else if (Specbits & 1 << (int) RID_LONG)
  {
    TreeType = c_long_integer_type_node;
  }
  else if (Specbits & 1 << (int) RID_SHORT)
  {
    TreeType = c_short_integer_type_node;
  }

  if (Specbits & 1 << (int) RID_COMPLEX)
  {
    /* If we just have "complex", it is equivalent to
       "complex double", but if any modifiers at all are specified it is
       the complex form of TYPE.  E.g, "complex short" is
       "complex short int".  */

    if (Defaulted_int && ! Longlong
        && ! (Specbits & ((1 << (int) RID_LONG) | (1 << (int) RID_SHORT)
                          | (1 << (int) RID_SIGNED)
                          | (1 << (int) RID_UNSIGNED))))
    {
      TreeType = c_complex_double_type_node;
    }
    else if (TreeType == c_integer_type_node)
    {
      TreeType = c_complex_integer_type_node;
    }
    else if (TreeType == c_float_type_node)
    {
      TreeType = c_complex_float_type_node;
    }
    else if (TreeType == c_double_type_node)
    {
      TreeType = c_complex_double_type_node;
    }
    else if (TreeType == c_long_double_type_node)
    {
      TreeType = c_complex_long_double_type_node;
    }
    else
    {
      TreeType = c_tree_build_complex_type (TreeType);
    }
  }

  /* Set CONSTP if this declaration is `const', whether by
     explicit specification or via a typedef.
     Likewise for VOLATILEP.  */

  Constp = (!! (Specbits & 1 << (int) RID_CONST)) + 
           (!! CIsTreeNodeReadOnly(TreeType)) ;

  Volatilep = (!! (Specbits & 1 << (int) RID_VOLATILE)) +
              (!! CIsTreeNodeVolatile(TreeType));

  Inlinep = !! (Specbits & (1 << (int) RID_INLINE));
  if (Constp > 1)
  {
    pedwarn ("duplicate `const'");
  }
  if (Volatilep > 1)
  {
    pedwarn ("duplicate `volatile'");
  }

  if (! c_flag_gen_aux_info && 
      (CIsTreeNodeReadOnly(TreeType) || CIsTreeNodeVolatile(TreeType)))
  {
    TreeType = CTreeTypeMainVariant(TreeType);
  }

  /* Warn if two storage classes are given. Default to `auto'.  */

  {
    int nclasses = 0;

    if (Specbits & 1 << (int) RID_AUTO) nclasses++;
    if (Specbits & 1 << (int) RID_STATIC) nclasses++;
    if (Specbits & 1 << (int) RID_EXTERN) nclasses++;
    if (Specbits & 1 << (int) RID_REGISTER) nclasses++;
    if (Specbits & 1 << (int) RID_TYPEDEF) nclasses++;
    if (Specbits & 1 << (int) RID_ITERATOR) nclasses++;

    /* Warn about storage classes that are invalid for certain
       kinds of declarations (parameters, typenames, etc.).  */

    if (nclasses > 1)
    {
      error ("multiple storage classes in declaration of `%s'", Name);
    }
    else if (Funcdef_flag
             && (Specbits
                 & ((1 << (int) RID_REGISTER)
                    | (1 << (int) RID_AUTO)
                    | (1 << (int) RID_TYPEDEF))))
    {
        if (Specbits & 1 << (int) RID_AUTO
            && (c_pedantic || c_current_binding_level == c_global_binding_level))
          pedwarn ("function definition declared `auto'");
        if (Specbits & 1 << (int) RID_REGISTER)
          error ("function definition declared `register'");
        if (Specbits & 1 << (int) RID_TYPEDEF)
          error ("function definition declared `typedef'");
        Specbits &= ~ ((1 << (int) RID_TYPEDEF) | (1 << (int) RID_REGISTER)
                       | (1 << (int) RID_AUTO));
    }
    else if (Decl_context != C_CDECL_NORMAL && nclasses > 0)
    {
        if (Decl_context == C_CDECL_PARAM && Specbits & 1 << (int) RID_REGISTER) ;
        else
        {
            error ((Decl_context == C_CDECL_FIELD
                    ? "storage class specified for structure field `%s'"
                    : (Decl_context == C_CDECL_PARAM
                       ? "storage class specified for parameter `%s'"
                       : "storage class specified for typename")),
                   Name);
            Specbits &= ~ ((1 << (int) RID_TYPEDEF) | (1 << (int) RID_REGISTER)
                           | (1 << (int) RID_AUTO) | (1 << (int) RID_STATIC)
                           | (1 << (int) RID_EXTERN));
        }
    }
    else if (Specbits & 1 << (int) RID_EXTERN && Initialized && ! Funcdef_flag)
    {
      /* `extern' with initialization is invalid if not at top level.  */
      if (c_current_binding_level == c_global_binding_level)
      {
        warning ("`%s' initialized and declared `extern'", Name);
      }
      else
      {
        error ("`%s' has both `extern' and initializer", Name);
      }
    }
    else if (Specbits & 1 << (int) RID_EXTERN && Funcdef_flag
             && c_current_binding_level != c_global_binding_level)
    {
      error ("nested function `%s' declared `extern'", Name);
    }
    else if (c_current_binding_level == c_global_binding_level
             && Specbits & (1 << (int) RID_AUTO))
    {
      error ("top-level declaration of `%s' specifies `auto'", Name);
    }
    else if ((Specbits & 1 << (int) RID_ITERATOR)
             && CTreeNodeFirstCode(Declarator) != C_IDENTIFIER_NODE)
    {
      error ("iterator `%s' has derived type", Name);
      TreeType = c_error_mark_node;
    }
    else if ((Specbits & 1 << (int) RID_ITERATOR)
             && CTreeTypeCode(TreeType) != C_INTEGER_TYPE)
    {
        error ("iterator `%s' has noninteger type", Name);
        TreeType = c_error_mark_node;
    }
  }

  /* Now figure out the structure of the declarator proper.
     Descend through it, creating more complex types, until we reach
     the declared identifier (or NULL, in an absolute declarator).  */

  while ( ( Declarator                    != (c_tree_node *)0 ) &&
          ( CTreeNodeFirstCode( Declarator ) != C_IDENTIFIER_NODE) )
  {
    if ( TreeType == c_error_mark_node )
    {
      Declarator = CTreeExprOperand( Declarator, 0 );
      continue;
    }

  /* Each level of DECLARATOR is either an ARRAY_REF (for ...[..]),
     an INDIRECT_REF (for *...),
     a CALL_EXPR (for ...(...)),
     an identifier (for the name being declared)
     or a null pointer (for the place in an absolute declarator
     where the name was omitted).
     For the last two cases, we have just exited the loop.

     At this point, TYPE is the TreeType of elements of an array,
     or for a function to return, or for a pointer to point to.
     After this sequence of ifs, TYPE is the TreeType of the
     array or function or pointer, and DECLARATOR has had its
     outermost layer removed.  */

    NodeType = CTreeNodeFirstCode( Declarator );

    if ( ( NodeType                    == C_EXPR_NODE ) &&
         ( CTreeExprCode( Declarator)  == C_ARRAY_REF ) )
    {
      c_tree_node *itype = (c_tree_node *)0;
      c_tree_node *size  =  CTreeExprOperand(Declarator, 1);
      /* An uninitialized decl with `extern' is a reference.  */
# ifdef LUDO_NO_SKIP
      int extern_ref = !Initialized && (Specbits & (1 << (int) RID_EXTERN));
# endif
      /* The index is a signed object `sizetype' bits wide.  */
      c_tree_node *index_type = c_get_signed_type(c_size_type);

      Declarator = CTreeExprOperand(Declarator, 0);

      /* Check for some types that there cannot be arrays of.  */

      if ( CTreeTypeMainVariant(TreeType) == c_void_type_node)
        {
          error ("declaration of `%s' as array of voids", Name);
          TreeType = c_error_mark_node;
        }

      if ( CTreeTypeCode(TreeType) == C_FUNCTION_TYPE)
        {
          error ("declaration of `%s' as array of functions", Name);
          TreeType = c_error_mark_node;
        }

      if (size == c_error_mark_node)
      {
        TreeType = c_error_mark_node;
      }

      if (TreeType == c_error_mark_node)
      {
        continue;
      }

      /* If this is a block level extern, it must live past the end
         of the function so that we can check it against other extern
         declarations (IDENTIFIER_LIMBO_VALUE).  */
# ifdef LUDO_NO_SKIP
      if (extern_ref && allocation_temporary_p ())
        end_temporary_allocation ();
# endif

      /* If size was specified, set ITYPE to a range-TreeType for that size.
         Otherwise, ITYPE remains null.  finish_decl may figure it out
         from an initial value.  */

      if (size)
      {
        /* Strip NON_LVALUE_EXPRs since we aren't using as an lvalue.  */

        CStripTypeNops(size);

        if ( CTreeTypeCode( CTreeType(size)) != C_INTEGER_TYPE
            && CTreeTypeCode( CTreeType(size)) != C_ENUMERAL_TYPE)
        {
          error ("size of array `%s' has non-integer type", Name);
          size = c_integer_one_node;
        }

        if (c_pedantic && c_tree_is_integer_zerop(size))
        {
          pedwarn ("ANSI C forbids zero-size array `%s'", Name);
        }

        if (CTreeNodeFirstCode(size) == C_INT_CST_NODE)
        {
          c_constant_expression_warning(size);
          if (c_tree_is_int_cst_sgn(size) < 0)
          {
            error ("size of array `%s' is negative", Name);
            size = c_integer_one_node;
          }
        }
        else
          {
            /* Make sure the array size remains visibly nonconstant
               even if it is (eg) a const variable with known value.  */
            Size_varies = 1;

            if (c_pedantic)
            {
              if ( CIsTreeNodeConstant(size))
              {
                  pedwarn("ANSI C forbids array `%s' whose size can't be evaluated", Name);
              }
              else
              {
                pedwarn("ANSI C forbids variable-size array `%s'", Name);
              }
            }
          }

        /* Convert size to index_type, so that if it is a variable
           the computations will be done in the proper mode.  */
        itype = c_fold( c_tree_build_binary_typed_expr(
                  C_MINUS_EXPR, index_type,
                  c_convert_expr(index_type, size),
                  c_convert_expr(index_type, c_size_one_node)) );

        /* If that overflowed, the array is too big.
           ??? While a size of INT_MAX+1 technically shouldn't cause
           an overflow (because we subtract 1), the overflow is recorded
           during the conversion to index_type, before the subtraction.
           Handling this case seems like an unnecessary complication.  */
        if ( CIsTreeNodeOverflow(itype))
          {
            error ("size of array `%s' is too large", Name);
            TreeType = c_error_mark_node;
            continue;
          }

# ifdef LUDO_NO_SKIP
        if (Size_varies)
        {
          itype = variable_size (itype);
        }
# endif
        itype = c_tree_build_index_type(itype);
      }

      /* Build the array TreeType itself, then merge any constancy or
         volatility into the target TreeType.  We must do it in this order
         to ensure that the TYPE_MAIN_VARIANT field of the array TreeType
         is set correctly.  */

      TreeType = c_tree_build_array_type(TreeType, itype);
      if (Constp || Volatilep)
      {
        TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
      }

      if (Size_varies)
      {
        CSetTreeTypeVarSize(TreeType);
      }
    }
    else
    if ( ( NodeType                      == C_EXPR_NODE ) &&
         ( CTreeExprCode( Declarator)  == C_CALL_EXPR ) )
    {
# ifdef LUDO_NO_SKIP
      int Extern_ref = (!(Specbits & (1 << (int) RID_AUTO))
                        || c_current_binding_level == c_global_binding_level);
# endif
      c_tree_node *arg_types;

      /* Declaring a function TreeType.
         Make sure we have a valid TreeType for the function to return.  */
      if (TreeType == c_error_mark_node)
        continue;

      Size_varies = 0;

      /* Warn about some types functions can't return.  */

      if ( CTreeTypeCode(TreeType) == C_FUNCTION_TYPE)
        {
          error ("`%s' declared as function returning a function", Name);
          TreeType = c_integer_type_node;
        }
      if ( CTreeTypeCode(TreeType) == C_ARRAY_TYPE)
        {
          error ("`%s' declared as function returning an array", Name);
          TreeType = c_integer_type_node;
        }

#ifndef TRADITIONAL_RETURN_FLOAT
      /* Traditionally, declaring return TreeType float means double.  */

      if (c_flag_traditional && CTreeTypeMainVariant(TreeType) == c_float_type_node)
      {
        TreeType = c_double_type_node;
      }
#endif /* TRADITIONAL_RETURN_FLOAT */

      /* If this is a block level extern, it must live past the end
         of the function so that we can check it against other extern
         declarations (IDENTIFIER_LIMBO_VALUE).  */
# ifdef LUDO_NO_SKIP
      if (extern_ref && allocation_temporary_p ())
        end_temporary_allocation ();
# endif

      /* Construct the function TreeType and go to the next
         inner layer of declarator.  */

      arg_types = c_grokparms( CTreeExprOperand( Declarator, 1),
                             Funcdef_flag
        /* Say it's a definition only for the CALL_EXPR closest to the identifier.  */
          && CTreeNodeFirstCode( CTreeExprOperand(Declarator, 0)) == C_IDENTIFIER_NODE);
      /* ANSI seems to say that `const int foo ();'
         does not make the function foo const.  */
      if (Constp || Volatilep)
      {
        TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
      }
      Constp = 0;
      Volatilep = 0;

      TreeType = c_tree_build_function_type(TreeType, arg_types);
      Declarator = CTreeExprOperand(Declarator, 0);

      /* Set the TYPE_CONTEXTs for each tagged TreeType which is local to
         the formal parameter list of this FUNCTION_TYPE to point to
         the FUNCTION_TYPE node itself.  */

      {
        c_tree_node *link;

        for (link = c_current_function_parm_tags;
             link;
             link = CTreeChain (link))
          CTreeTypeContext( CTreeListValue(link)) = TreeType;
      }
    }
    else
    if ( ( NodeType                      == C_EXPR_NODE    ) &&
         ( CTreeExprCode( Declarator)  == C_INDIRECT_REF ) )
    {
      /* Merge any constancy or volatility into the target TreeType
         for the pointer.  */

      if (c_pedantic && CTreeTypeCode(TreeType) == C_FUNCTION_TYPE
          && (Constp || Volatilep))
      {
        pedwarn ("ANSI C forbids const or volatile function types");
      }
      if (Constp || Volatilep)
      {
        TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
      }

      Constp = 0;
      Volatilep = 0;
      Size_varies = 0;

      TreeType = c_tree_build_pointer_type(TreeType);

      /* Process a list of TreeType modifier keywords
         (such as const or volatile) that were given inside the `*'.  */

      if ( CTreeType(Declarator) != (c_tree_node *)0 )
      {
          c_tree_node *typemodlist;
          int erred = 0;
          for (typemodlist = CTreeType(Declarator); typemodlist;
               typemodlist = CTreeChain(typemodlist))
          {
              if ( CTreeListValue(typemodlist) == c_ridpointers[(int) RID_CONST])
              {
                Constp++;
              }
              else if ( CTreeListValue(typemodlist) == c_ridpointers[(int) RID_VOLATILE])
              {
                Volatilep++;
              }
              else if (!erred)
              {
                  erred = 1;
                  error ("invalid TreeType modifier within pointer declarator");
              }
          }
          if (Constp > 1)
          {
            pedwarn ("duplicate `const'");
          }
          if (Volatilep > 1)
          {
            pedwarn ("duplicate `volatile'");
          }
      }

      Declarator = CTreeExprOperand(Declarator, 0);
    }
    else abort ();
  }

  /* Now TYPE has the actual TreeType.  */

  /* Did array size calculations overflow?  */

  if ( CTreeTypeCode(TreeType) == C_ARRAY_TYPE
      && CTreeTypeSize(TreeType)
      && CIsTreeNodeOverflow( CTreeTypeSize(TreeType)))
  {
    error ("size of array `%s' is too large", Name);
  }

  /* If this is declaring a typedef name, return a TYPE_DECL.  */

  if (Specbits & (1 << (int) RID_TYPEDEF))
  {
# ifdef LUDO_NO_SKIP
    c_tree_node *decl;
# endif
    /* Note that the grammar rejects storage classes
       in typenames, fields or parameters */
    if ( c_pedantic &&  
         ( CTreeTypeCode( TreeType ) == C_FUNCTION_TYPE ) &&
         ( Constp || Volatilep ) )
    {
      pedwarn ("ANSI C forbids const or volatile function types");
    }
    if (Constp || Volatilep)
    {
      TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
    }

    TreeDecl = c_tree_build_decl( C_TYPE_DECL, Declarator, TreeType);

# ifdef LUDO_NO_SKIP
    if ((Specbits & (1 << (int) RID_SIGNED))
        || (typedef_decl && C_TYPEDEF_EXPLICITLY_SIGNED (typedef_decl)))
      C_TYPEDEF_EXPLICITLY_SIGNED (decl) = 1;
    pop_obstacks ();
# endif

    return( TreeDecl );
  }

  /* Detect the case of an array TreeType of unspecified size
     which came, as such, direct from a typedef name.
     We must copy the TreeType, so that each identifier gets
     a distinct TreeType, so that each identifier's size can be
     controlled separately by its own initializer.  */

  if ( ( TreeType     != (c_tree_node *)0 ) &&
       ( Typedef_type != (c_tree_node *)0 ) &&
       ( CTreeTypeMainVariant(TreeType) == CTreeTypeMainVariant(Typedef_type) ) &&
       ( CTreeTypeCode(TreeType) == C_ARRAY_TYPE ) && 
       ( CTreeTypeValues(TreeType) == 0) )
  {
    TreeType = c_tree_build_array_type( CTreeType(TreeType), 0);
    if (Size_varies)
    {
      CSetTreeTypeVarSize(TreeType);
    }
  }

  /* If this is a TreeType name (such as, in a cast or sizeof),
     compute the TreeType and return it now.  */

  if (Decl_context == C_CDECL_TYPENAME)
    {
      /* Note that the grammar rejects storage classes
         in typenames, fields or parameters */
      if (c_pedantic &&  CTreeTypeCode(TreeType) == C_FUNCTION_TYPE
          && (Constp || Volatilep))
      {
        pedwarn ("ANSI C forbids const or volatile function types");
      }

      if (Constp || Volatilep)
      {
        TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
      }

# ifdef LUDO_NO_SKIP
      pop_obstacks ();
# endif
      return TreeType;
    }

  /* Aside from typedefs and TreeType names (handle above),
     `void' at top level (not within pointer)
     is allowed only in public variables.
     We don't complain about parms either, but that is because
     a better error message can be made later.  */

  if ( CTreeTypeMainVariant(TreeType) == c_void_type_node && 
      Decl_context != C_CDECL_PARAM
      && ! ((Decl_context != C_CDECL_FIELD && 
             CTreeTypeCode(TreeType) != C_FUNCTION_TYPE)
            && ((Specbits & (1 << (int) RID_EXTERN))
                || (c_current_binding_level == c_global_binding_level
                    && !(Specbits
                         & ((1 << (int) RID_STATIC) | (1 << (int) RID_REGISTER)))))))
    {
      error ("variable or field `%s' declared void", Name);
      TreeType = c_integer_type_node;
    }

  /* Now create the decl, which may be a VAR_DECL, a PARM_DECL
     or a FUNCTION_DECL, depending on DECL_CONTEXT and TYPE.  */

  if ( Decl_context == C_CDECL_PARAM )
  {
# ifdef LUDO_NO_SKIP
    c_tree_node *type_as_written = TreeType;
# endif
    c_tree_node *main_type;

    /* A parameter declared as an array of T is really a pointer to T.
       One declared as a function is really a pointer to a function.  */

    if ( CTreeTypeCode(TreeType) == C_ARRAY_TYPE)
    {
      /* Transfer const-ness of array into that of TreeType pointed to.  */
      TreeType = CTreeType(TreeType);

      if (Constp || Volatilep)
      {
        TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
      }
      TreeType = c_tree_build_pointer_type(TreeType);
      Volatilep = Constp = 0;
      Size_varies = 0;
    }
    else if ( CTreeTypeCode(TreeType) == C_FUNCTION_TYPE)
    {
      if (c_pedantic && (Constp || Volatilep))
      {
          pedwarn ("ANSI C forbids const or volatile function types");
      }
      if (Constp || Volatilep)
      {
          TreeType = c_c_build_type_variant(TreeType, Constp, Volatilep);
      }
      TreeType = c_tree_build_pointer_type(TreeType);
      Volatilep = Constp = 0;
    }

    TreeDecl = c_tree_build_decl( C_PARAM_DECL, Declarator, TreeType);

    if (Size_varies)
    {
      CSetTreeDeclVarSize( TreeDecl );
    }

    /* Compute the TreeType actually passed in the parmlist,
       for the case where there is no prototype.
       (For example, shorts and chars are passed as ints.)
       When there is a prototype, this is overridden later.  */

    CTreeDeclInitial( TreeDecl ) = TreeType;
    main_type = (TreeType == c_error_mark_node
                 ? c_error_mark_node : CTreeTypeMainVariant(TreeType));

    if (main_type == c_float_type_node)
    {
      CTreeDeclInitial( TreeDecl ) = c_double_type_node;
    }
    /* Don't use TYPE_PRECISION to decide whether to promote,
       because we should convert short if it's the same size as int,
       but we should not convert long if it's the same size as int.  */

    else if ( CTreeNodeFirstCode(main_type) != C_ERROR_NODE
             && C_PROMOTING_INTEGER_TYPE_P(main_type))
    {
      if ( CTreeTypePrecision(TreeType) ==  CTreeTypePrecision(c_integer_type_node)
            && CIsTreeNodeUnsigned(TreeType))
      {
        CTreeDeclInitial( TreeDecl ) = c_unsigned_type_node;
      }
      else
      {
        CTreeDeclInitial( TreeDecl ) = c_integer_type_node;
      }
    }

# ifdef LUDO_NO_SKIP
    DECL_ARG_TYPE_AS_WRITTEN ( TreeDecl ) = type_as_written;
# endif
  }
  else if ( Decl_context == C_CDECL_FIELD)
  {
    /* Structure field.  It may not be a function.  */

    if ( CTreeTypeCode(TreeType) == C_FUNCTION_TYPE)
    {
      error ("field `%s' declared as a function", Name);
      TreeType = c_tree_build_pointer_type(TreeType);
    }
    else if ( CTreeNodeFirstCode(TreeType) != C_ERROR_NODE && CTreeTypeSize(TreeType) == 0)
    {
      error ("field `%s' has incomplete type", Name);
      TreeType = c_error_mark_node;
    }
    /* Move TreeType qualifiers down to element of an array.  */
    if ( CTreeTypeCode(TreeType) == C_ARRAY_TYPE && (Constp || Volatilep))
    {
      TreeType = c_tree_build_array_type(c_c_build_type_variant(
         CTreeType(TreeType), Constp, Volatilep),
                                 CTreeTypeValues(TreeType));
    }

    TreeDecl = c_tree_build_decl( C_FIELD_DECL, Declarator, TreeType );

    if (Size_varies)
    {
      CSetTreeDeclVarSize(TreeDecl);
    }
  }
  else if ( CTreeTypeCode(TreeType) == C_FUNCTION_TYPE)
  {
    /* Every function declaration is "external"
       except for those which are inside a function body
       in which `auto' is used.
       That is a case not specified by ANSI C,
       and we use it for forward declarations for nested functions.  */
    int extern_ref = (!(Specbits & (1 << (int) RID_AUTO))
                      || c_current_binding_level == c_global_binding_level);

    if (Specbits & (1 << (int) RID_AUTO)
        && (c_pedantic || c_current_binding_level == c_global_binding_level))
    {
      pedwarn ("invalid storage class for function `%s'", Name);
    }
    if (Specbits & (1 << (int) RID_REGISTER))
    {
      error ("invalid storage class for function `%s'", Name);
    }
    /* Function declaration not at top level.
       Storage classes other than `extern' are not allowed
       and `extern' makes no difference.  */
    if (c_current_binding_level != c_global_binding_level
        && (Specbits & ((1 << (int) RID_STATIC) | (1 << (int) RID_INLINE)))
        && c_pedantic)
    {
      pedwarn ("invalid storage class for function `%s'", Name);
    }

    /* If this is a block level extern, it must live past the end
       of the function so that we can check it against other
       extern declarations (IDENTIFIER_LIMBO_VALUE).  */
# ifdef LUDO_NO_SKIP
    if (extern_ref && allocation_temporary_p ())
      end_temporary_allocation ();
# endif

    TreeDecl = c_tree_build_decl( C_FUNCTION_DECL, Declarator, TreeType );

# ifdef LUDO_NO_SKIP
    decl = c_build_decl_attribute_variant(decl, decl_machine_attr);
# endif

    if (c_pedantic && (Constp || Volatilep)
        && ! CIsTreeDeclSystemHeader( TreeDecl ))
    {
      pedwarn ("ANSI C forbids const or volatile functions");
    }

    if (c_pedantic
        && CTreeTypeMainVariant( 
            CTreeType( CTreeType( TreeDecl ))) == c_void_type_node
        && ( CIsTreeNodeReadOnly( CTreeType( CTreeType( TreeDecl )))
            || CIsTreeNodeVolatile( CTreeType( CTreeType( TreeDecl ))))
        && ! CIsTreeDeclSystemHeader( TreeDecl ))
    {
      pedwarn ("ANSI C forbids const or volatile void function return type");
    }

    if (Volatilep
        &&  CTreeType( CTreeType( TreeDecl )) != c_void_type_node)
    {
      warning ("`noreturn' function returns non-void value");
    }

    if (extern_ref)
    {
      CSetTreeDeclExternal( TreeDecl );
    }

    /* Record absence of global scope for `static' or `auto'.  */
    if ( !(Specbits & ((1 << (int) RID_STATIC) | (1 << (int) RID_AUTO))))
    {
      CSetTreeNodePublic( TreeDecl );
    }

    /* Record presence of `inline', if it is reasonable.  */
    if (Inlinep)
    {
# ifdef LUDO_NO_SKIP
        c_tree_node *last = c_tree_get_last_list( CTreeTypeValues(TreeType));
# endif
        if (! strcmp( CTreeIdentPointer(Declarator), "main"))
        {
          warning ("cannot inline function `main'");
        }
        else
          /* Assume that otherwise the function can be inlined.  */
          CSetTreeDeclInline( TreeDecl );

        if (Specbits & (1 << (int) RID_EXTERN))
        {
          c_current_extern_inline = 1;
        }
    }
  }
  else
  {
      /* It's a variable.  */
      /* An uninitialized decl with `extern' is a reference.  */
      int extern_ref = !Initialized && (Specbits & (1 << (int) RID_EXTERN));

      /* Move TreeType qualifiers down to element of an array.  */
      if ( CTreeTypeCode(TreeType) == C_ARRAY_TYPE && (Constp || Volatilep))
        {
          TreeType = c_tree_build_array_type(
                 c_c_build_type_variant( CTreeType(TreeType), Constp, Volatilep),
                                   CTreeTypeValues(TreeType));
        }

      /* If this is a block level extern, it must live past the end
         of the function so that we can check it against other
         extern declarations (IDENTIFIER_LIMBO_VALUE).  */
# ifdef LUDO_NO_SKIP
      if (extern_ref && allocation_temporary_p ())
        end_temporary_allocation ();
# endif

      TreeDecl = c_tree_build_decl( C_VAR_DECL, Declarator, TreeType );

      if (Size_varies)
      {
        CSetTreeDeclVarSize( TreeDecl );
      }

      if (Inlinep)
      {
        pedwarn_with_decl( TreeDecl, "variable `%s' declared `inline'");
      }

      if ( extern_ref )
      {
        CSetTreeNodeExternal( TreeDecl);
      }
      /* At top level, the presence of a `static' or `register' storage
         class specifier, or the absence of all storage class specifiers
         makes this declaration a definition (perhaps tentative).  Also,
         the absence of both `static' and `register' makes it public.  */
      if (c_current_binding_level == c_global_binding_level)
      {
        if ( !(Specbits
                & ((1 << (int) RID_STATIC) | (1 << (int) RID_REGISTER))) )
        {
          CSetTreeNodePublic( TreeDecl );
        }

        if ( ! CIsTreeNodeExternal( TreeDecl ) )
        {
          CSetTreeNodeStatic( TreeDecl );
        }
      }
      /* Not at top level, only `static' makes a static definition.  */
      else
      {
        if ( (Specbits & (1 << (int) RID_STATIC)) != 0 )
        {
          CSetTreeNodeStatic( TreeDecl );
        }

        if ( CIsTreeNodeExternal( TreeDecl ) )
        {
          CSetTreeNodePublic( TreeDecl );
        }
      }

      if (Specbits & 1 << (int) RID_ITERATOR)
      {
        CSetTreeDeclIterator( TreeDecl );
      }
  }

  /* Record `register' declaration for warnings on &
     and in case doing stupid register allocation.  */

  if (Specbits & (1 << (int) RID_REGISTER))
  {
    CSetTreeDeclRegister( TreeDecl );
  }

  /* Record constancy and volatility.  */

  if (Constp)
  {
    CSetTreeNodeReadOnly( TreeDecl );
  }

  if (Volatilep)
  {
    CSetTreeNodeSideEffect( TreeDecl );
    CSetTreeNodeVolatile( TreeDecl );
  }

  /* If a type has volatile components, it should be stored in memory.
     Otherwise, the fact that those components are volatile
     will be ignored, and would even crash the compiler.  */
  if ( CIsTreeTypeFieldsVolatile( CTreeType( TreeDecl )))
  {
    c_mark_addressable ( TreeDecl );
  }

  return( TreeDecl ); 
}



/* Given a parsed parameter declaration,
   decode it into a PARM_DECL and push that on the current binding level.
   Also, for the sake of forward parm decls,
   record the given order of parms in `parm_order'.  */

void c_push_parm_decl(parm)

     c_tree_node *parm;
{
  c_tree_node *decl;
# ifdef LUDO_NO_SKIP
  int old_immediate_size_expand = c_immediate_size_expand;
  /* Don't try computing parm sizes now -- wait till fn is called.  */
  immediate_size_expand = 0;

  /* The corresponding pop_obstacks is in finish_decl.  */
  push_obstacks_nochange ();
# endif

  decl = c_grokdeclarator( CTreeListValue( CTreeListPurpose(parm)),
			  CTreeListPurpose( CTreeListPurpose(parm)), C_CDECL_PARAM, 0);
  c_decl_attributes( decl,
       CTreeListValue( CTreeListValue(parm)),
		   CTreeListPurpose( CTreeListValue(parm)));

  decl = c_push_decl(decl);

# ifdef LUDO_NO_SKIP
  immediate_size_expand = old_immediate_size_expand;
# endif

  c_current_binding_level->PARAM_LIST
    = c_tree_add_head_list( (c_tree_node *)0,decl,c_current_binding_level->PARAM_LIST );

  /* Add this decl to the current binding level.  */
  c_finish_decl(decl, (c_tree_node *)0, (c_tree_node *)0);
}

/*------------------------------------------------------------\
|                                                             |
|                    Initialize Predefined Declaration        |
|                                                             |
\------------------------------------------------------------*/

void c_init_decl_processing()
{
  static int FirstTime = 1;

# ifdef LUDO_NO_SKIP
  int wchar_type_size;
# endif
  
  c_tree_node *TreeDecl;
  c_tree_node *TreeIdent;
  c_tree_node *EndLink;
  c_tree_node *array_domain_type;
  c_tree_node *void_ftype_any;
  c_tree_node *ptr_ftype_void;
  c_tree_node *ptr_ftype_ptr;
  c_tree_node *memcpy_ftype;
  c_tree_node *memset_ftype;
  c_tree_node *strlen_ftype;
  c_tree_node *traditional_ptr_type_node;
  

  c_current_binding_level = (c_binding_level *)0;
  c_current_function_decl = (c_tree_node     *)0;
  c_named_labels          = (c_tree_node     *)0;

  c_push_level( 0 );

  c_global_binding_level = c_current_binding_level;

  c_integer_type_node    = c_tree_build_signed_type( C_INT_TYPE_SIZE );
  TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                  c_ridpointers[(int)RID_INT],
                                  c_integer_type_node );
  c_push_decl( TreeDecl );

  if ( FirstTime )
  {
      FirstTime = 0;
    /* Define `char', which is like either `signed char' or `unsigned char'
       but not the same as either.  */

    if ( c_flag_signed_char )
    { 
      c_char_type_node = c_tree_build_signed_type( C_CHAR_TYPE_SIZE );
    }
    else
    {
      c_char_type_node = c_tree_build_unsigned_type( C_CHAR_TYPE_SIZE );
    }
    TreeDecl = c_tree_build_decl( C_TYPE_DECL,
                                    c_tree_get_identifier("char"),
                                    c_char_type_node );
    c_push_decl( TreeDecl );

    c_long_integer_type_node = c_tree_build_signed_type( C_LONG_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL,
                                    c_tree_get_identifier("long int"),
                                    c_long_integer_type_node );
    c_push_decl( TreeDecl ); 

    c_unsigned_type_node = c_tree_build_unsigned_type( C_INT_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("unsigned int"),
                                    c_unsigned_type_node );
    c_push_decl( TreeDecl );

    c_long_unsigned_type_node = c_tree_build_unsigned_type( C_LONG_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL,
                                    c_tree_get_identifier("long unsigned int"),
                                    c_long_unsigned_type_node );

    c_push_decl( TreeDecl );

    c_long_long_integer_type_node = c_tree_build_signed_type( C_LONG_LONG_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL,
                                    c_tree_get_identifier("long long int"),
                                    c_long_long_integer_type_node );
    c_push_decl( TreeDecl );

    c_long_long_unsigned_type_node = c_tree_build_unsigned_type( C_LONG_LONG_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL,
                                    c_tree_get_identifier("long long unsigned int"),
                                    c_long_long_unsigned_type_node );
    c_push_decl( TreeDecl );

    c_short_integer_type_node = c_tree_build_signed_type( C_SHORT_TYPE_SIZE );
    TreeDecl =c_tree_build_decl ( C_TYPE_DECL,
                                    c_tree_get_identifier("short int"),
                                    c_short_integer_type_node );
    c_push_decl( TreeDecl );

    c_short_unsigned_type_node = c_tree_build_unsigned_type( C_SHORT_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("short unsigned int"),
                                    c_short_unsigned_type_node );
    c_push_decl( TreeDecl );

    /* `unsigned long' is the standard type for sizeof.
       Traditionally, use a signed type.
       Note that stddef.h uses `unsigned long',
       and this must agree, even of long and int are the same size.  */

    TreeIdent    = c_tree_get_identifier( "long unsigned int" );
    c_size_type = CTreeType( CTreeIdentGlobalValue( TreeIdent ) );

    if ( c_flag_traditional &&  CIsTreeNodeUnsigned( c_size_type ) )
    {
      c_size_type = c_get_signed_type( c_size_type );
    }

    TreeIdent = c_tree_get_identifier( "long int" );
    c_ptrdiff_type_node = CTreeType( CTreeIdentGlobalValue( TreeIdent ) );

    c_integer_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_char_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_unsigned_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_long_unsigned_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_long_integer_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_long_long_integer_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_long_long_unsigned_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_short_integer_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;
    c_short_unsigned_type_node->TYPE.SIZE->COMMON.TYPE = c_size_type;

    c_error_mark_node = c_tree_add_node( C_ERROR_NODE );
    c_error_mark_node->COMMON.TYPE = c_error_mark_node;

    /* Define both `signed char' and `unsigned char'.  */

    c_signed_char_type_node = c_tree_build_signed_type( C_CHAR_TYPE_SIZE );
    TreeDecl =c_tree_build_decl( C_TYPE_DECL, 
                                   c_tree_get_identifier("signed char"),
                                   c_signed_char_type_node );
    c_push_decl( TreeDecl );

    c_unsigned_char_type_node = c_tree_build_unsigned_type( C_CHAR_TYPE_SIZE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("unsigned char"),
                                    c_unsigned_char_type_node );
    c_push_decl( TreeDecl );

# ifdef LUDO_NO_SKIP
    intQI_type_node = c_tree_build_signed_type(GET_MODE_BITSIZE (QImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, intQI_type_node));

    intHI_type_node = c_tree_build_signed_type(GET_MODE_BITSIZE (HImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, intHI_type_node));

    intSI_type_node = c_tree_build_signed_type(GET_MODE_BITSIZE (SImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, intSI_type_node));

    intDI_type_node = c_tree_build_signed_type(GET_MODE_BITSIZE (DImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, intDI_type_node));

    unsigned_intQI_type_node = c_tree_build_unsigned_type (GET_MODE_BITSIZE (QImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, unsigned_intQI_type_node));

    unsigned_intHI_type_node = c_tree_build_unsigned_type (GET_MODE_BITSIZE (HImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, unsigned_intHI_type_node));

    unsigned_intSI_type_node = c_tree_build_unsigned_type (GET_MODE_BITSIZE (SImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, unsigned_intSI_type_node));

    unsigned_intDI_type_node = c_tree_build_unsigned_type (GET_MODE_BITSIZE (DImode));
    c_push_decl (c_tree_build_decl ( C_TYPE_DECL, NULL, unsigned_intDI_type_node));
# endif

    c_float_type_node = c_tree_build_type( C_REAL_TYPE );
    c_float_type_node->TYPE.PRECISION = C_FLOAT_TYPE_SIZE;
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_ridpointers[(int) RID_FLOAT],
                                    c_float_type_node );
    c_push_decl( TreeDecl );
    c_layout_type( c_float_type_node);

    c_double_type_node = c_tree_build_type( C_REAL_TYPE);
    if ( c_flag_short_double )
    {
      c_double_type_node->TYPE.PRECISION = C_FLOAT_TYPE_SIZE;
    }
    else
    {
      c_double_type_node->TYPE.PRECISION = C_DOUBLE_TYPE_SIZE;
    }
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_ridpointers[(int) RID_DOUBLE],
                                    c_double_type_node );
    c_push_decl( TreeDecl );
    c_layout_type( c_double_type_node);

    c_long_double_type_node = c_tree_build_type( C_REAL_TYPE );
    c_long_double_type_node->TYPE.PRECISION = C_LONG_DOUBLE_TYPE_SIZE;
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("long double"),
                                    c_long_double_type_node );
    c_push_decl( TreeDecl );
    c_layout_type(c_long_double_type_node);

    c_complex_integer_type_node = c_tree_build_type( C_COMPLEX_TYPE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("complex int"),
                                    c_complex_integer_type_node );
    c_push_decl( TreeDecl );
    c_complex_integer_type_node->COMMON.TYPE = c_integer_type_node;
    c_layout_type(c_complex_integer_type_node);

    c_complex_float_type_node = c_tree_build_type( C_COMPLEX_TYPE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("complex float"),
                                    c_complex_float_type_node );
    c_push_decl( TreeDecl );
    c_complex_float_type_node->COMMON.TYPE = c_float_type_node;
    c_layout_type(c_complex_float_type_node);

    c_complex_double_type_node = c_tree_build_type( C_COMPLEX_TYPE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("complex double"),
                                    c_complex_double_type_node );
    c_push_decl( TreeDecl );
    c_complex_double_type_node->COMMON.TYPE = c_double_type_node;
    c_layout_type(c_complex_double_type_node);

    c_complex_long_double_type_node = c_tree_build_type( C_COMPLEX_TYPE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL, 
                                    c_tree_get_identifier("complex long double"),
                                    c_complex_long_double_type_node );
    c_push_decl( TreeDecl );
    c_complex_long_double_type_node->COMMON.TYPE = c_long_double_type_node;
    c_layout_type( c_complex_long_double_type_node);

    c_wchar_type_node = CTreeType( 
        CTreeIdentGlobalValue( c_tree_get_identifier( "int" )));
    c_signed_wchar_type_node = c_get_signed_type( c_wchar_type_node);
    c_unsigned_wchar_type_node = c_get_unsigned_type( c_wchar_type_node);

    c_integer_zero_node = c_tree_build_int_2( 0, 0 );
    c_integer_zero_node->COMMON.TYPE = c_integer_type_node;
    c_integer_one_node  = c_tree_build_int_2( 1, 0 );
    c_integer_one_node->COMMON.TYPE  = c_integer_type_node;

    c_boolean_type_node  = c_integer_type_node;
    c_boolean_true_node  = c_integer_one_node;
    c_boolean_false_node = c_integer_zero_node;

    c_size_zero_node = c_tree_build_int_2( 0, 0 );
    c_size_zero_node->COMMON.TYPE = c_size_type;
    c_size_one_node  = c_tree_build_int_2( 1, 0 );
    c_size_one_node->COMMON.TYPE = c_size_type;

    c_void_type_node = c_tree_build_type( C_VOID_TYPE );
    TreeDecl = c_tree_build_decl( C_TYPE_DECL,
                                    c_ridpointers[(int) RID_VOID], 
                                    c_void_type_node );
    c_push_decl( TreeDecl );
                  /* Uses integer_zero_node */
    c_layout_type( c_void_type_node);       
    /* We are not going to have real types in C with less than byte alignment,
       so we might as well not have any types that claim to have it.  */
    CTreeTypeAlign(c_void_type_node) = C_BITS_PER_UNIT;

    c_null_pointer_node = c_tree_build_int_2( 0, 0 );
    c_null_pointer_node->COMMON.TYPE = c_tree_build_pointer_type( c_void_type_node );
    c_layout_type( CTreeType( c_null_pointer_node ) );

    c_string_type_node = c_tree_build_pointer_type( c_char_type_node );

    c_const_string_type_node = c_tree_build_pointer_type( 
                   c_tree_build_type_variant( c_char_type_node, 1, 0));

    /* Make a type to be the domain of a few array types
       whose domains don't really matter.
       200 is small enough that it always fits in size_t
       and large enough that it can hold most function names for the
       initializations of __FUNCTION__ and __PRETTY_FUNCTION__.  */
    array_domain_type = c_tree_build_index_type( c_tree_build_int_2( 200, 0 ) );

    /* make a type for arrays of characters.
       With luck nothing will ever really depend on the length of this
       array type.  */
    c_char_array_type_node
      = c_tree_build_array_type( c_char_type_node, array_domain_type );
    /* Likewise for arrays of ints.  */
    c_int_array_type_node
      = c_tree_build_array_type( c_integer_type_node, array_domain_type );
    /* This is for wide string constants.  */
    c_wchar_array_type_node
      = c_tree_build_array_type(c_wchar_type_node, array_domain_type);

    c_default_function_type
      = c_tree_build_function_type(c_integer_type_node, (c_tree_node *)0);

    c_ptr_type_node = c_tree_build_pointer_type( c_void_type_node);
    c_const_ptr_type_node
      = c_tree_build_pointer_type( c_tree_build_type_variant( c_void_type_node, 1, 0));

    EndLink = c_tree_add_head_list( (c_tree_node *)0, 
                      c_void_type_node, (c_tree_node *)0);

    void_ftype_any
      = c_tree_build_function_type(c_void_type_node, NULL);

    c_float_ftype_float
      = c_tree_build_function_type(c_float_type_node,
                          c_tree_add_head_list (NULL, c_float_type_node, EndLink));

    c_double_ftype_double
      = c_tree_build_function_type(c_double_type_node,
                          c_tree_add_head_list(NULL, c_double_type_node, EndLink));

    c_ldouble_ftype_ldouble
      = c_tree_build_function_type(c_long_double_type_node,
                          c_tree_add_head_list(NULL, c_long_double_type_node,
                                    EndLink));

    c_double_ftype_double_double
      = c_tree_build_function_type(c_double_type_node,
                          c_tree_add_head_list(NULL, c_double_type_node,
                                    c_tree_add_head_list(NULL,
                                             c_double_type_node, EndLink)));

    c_int_ftype_int
      = c_tree_build_function_type(c_integer_type_node,
                          c_tree_add_head_list(NULL, c_integer_type_node, EndLink));

    c_long_ftype_long
      = c_tree_build_function_type(c_long_integer_type_node,
                          c_tree_add_head_list(NULL,
                                    c_long_integer_type_node, EndLink));

    c_void_ftype_ptr_ptr_int
      = c_tree_build_function_type(c_void_type_node,
                          c_tree_add_head_list(NULL, c_ptr_type_node,
                                    c_tree_add_head_list(NULL, c_ptr_type_node,
                                             c_tree_add_head_list(NULL,
                                                       c_integer_type_node,
                                                       EndLink))));

    c_int_ftype_cptr_cptr_sizet
      = c_tree_build_function_type(c_integer_type_node,
                          c_tree_add_head_list(NULL, c_const_ptr_type_node,
                                    c_tree_add_head_list(NULL, c_const_ptr_type_node,
                                             c_tree_add_head_list(NULL,
                                                       c_size_type,
                                                       EndLink))));

    c_void_ftype_ptr_int_int
      = c_tree_build_function_type(c_void_type_node,
                          c_tree_add_head_list(NULL, c_ptr_type_node,
                                    c_tree_add_head_list(NULL, c_integer_type_node,
                                             c_tree_add_head_list(NULL,
                                                       c_integer_type_node,
                                                       EndLink))));

    c_string_ftype_ptr_ptr              /* strcpy prototype */
      = c_tree_build_function_type(c_string_type_node,
                          c_tree_add_head_list(NULL, c_string_type_node,
                                    c_tree_add_head_list(NULL,
                                             c_const_string_type_node,
                                             EndLink)));

    c_int_ftype_string_string       /* strcmp prototype */
      = c_tree_build_function_type(c_integer_type_node,
                          c_tree_add_head_list(NULL, c_const_string_type_node,
                                    c_tree_add_head_list(NULL,
                                             c_const_string_type_node,
                                             EndLink)));

    strlen_ftype              /* strlen prototype */
      = c_tree_build_function_type(c_flag_traditional ? c_integer_type_node : c_size_type,
                          c_tree_add_head_list(NULL, c_const_string_type_node,
                                    EndLink));

    traditional_ptr_type_node
      = (c_flag_traditional ? c_string_type_node : c_ptr_type_node);

    memcpy_ftype       /* memcpy prototype */
      = c_tree_build_function_type(traditional_ptr_type_node,
                          c_tree_add_head_list(NULL, c_ptr_type_node,
                                    c_tree_add_head_list(NULL, c_const_ptr_type_node,
                                             c_tree_add_head_list(NULL,
                                                       c_size_type,
                                                       EndLink))));

    memset_ftype       /* memset prototype */
      = c_tree_build_function_type(traditional_ptr_type_node,
                          c_tree_add_head_list(NULL, c_ptr_type_node,
                                    c_tree_add_head_list(NULL, c_integer_type_node,
                                             c_tree_add_head_list(NULL,
                                                       c_size_type,
                                                       EndLink))));

    ptr_ftype_void = c_tree_build_function_type(c_ptr_type_node, EndLink);
    ptr_ftype_ptr = c_tree_build_function_type(c_ptr_type_node,
                          c_tree_add_head_list(NULL, c_ptr_type_node, EndLink));

# ifdef LUDO_NO_SKIP
    builtin_function ("__builtin_constant_p", c_default_function_type,
                    BUILT_IN_CONSTANT_P, NULL_PTR);

    builtin_function ("__builtin_return_address",
                    c_tree_build_function_type(ptr_type_node, 
                                      c_tree_add_head_list(NULL,
                                                unsigned_type_node,
                                                EndLink)),
                    BUILT_IN_RETURN_ADDRESS, NULL_PTR);

    builtin_function ("__builtin_frame_address",
                    c_tree_build_function_type(ptr_type_node, 
                                      c_tree_add_head_list(NULL,
                                                unsigned_type_node,
                                                EndLink)),
                    BUILT_IN_FRAME_ADDRESS, NULL_PTR);

    builtin_function ("__builtin_aggregate_incoming_address",
                    c_tree_build_function_type(ptr_type_node, NULL),
                    BUILT_IN_AGGREGATE_INCOMING_ADDRESS, NULL_PTR);

    /* Hooks for the DWARF 2 __throw routine.  */
    builtin_function ("__builtin_unwind_init",
                    c_tree_build_function_type(void_type_node, EndLink),
                    BUILT_IN_UNWIND_INIT, NULL_PTR);
    builtin_function ("__builtin_fp", ptr_ftype_void, BUILT_IN_FP, NULL_PTR);
    builtin_function ("__builtin_sp", ptr_ftype_void, BUILT_IN_SP, NULL_PTR);
    builtin_function ("__builtin_dwarf_fp_regnum",
                    c_tree_build_function_type(unsigned_type_node, EndLink),
                    BUILT_IN_DWARF_FP_REGNUM, NULL_PTR);
    builtin_function ("__builtin_dwarf_reg_size", int_ftype_int,
                    BUILT_IN_DWARF_REG_SIZE, NULL_PTR);                  
    builtin_function ("__builtin_frob_return_addr", ptr_ftype_ptr,
                    BUILT_IN_FROB_RETURN_ADDR, NULL_PTR);
    builtin_function ("__builtin_extract_return_addr", ptr_ftype_ptr,
                    BUILT_IN_EXTRACT_RETURN_ADDR, NULL_PTR);
    builtin_function ("__builtin_set_return_addr_reg",
                    c_tree_build_function_type(void_type_node, 
                                      c_tree_add_head_list(NULL,
                                                ptr_type_node,
                                                EndLink)),
                    BUILT_IN_SET_RETURN_ADDR_REG, NULL_PTR);
    builtin_function ("__builtin_eh_stub", ptr_ftype_void,
                    BUILT_IN_EH_STUB, NULL_PTR);
    builtin_function
      ("__builtin_set_eh_regs",
       c_tree_build_function_type(void_type_node,
                         c_tree_add_head_list(NULL, ptr_type_node,
                                   c_tree_add_head_list(NULL,
                                            type_for_mode (ptr_mode, 0),
                                            EndLink))),
       BUILT_IN_SET_EH_REGS, NULL_PTR);

    builtin_function ("__builtin_alloca",
                    c_tree_build_function_type(ptr_type_node,
                                      c_tree_add_head_list(NULL,
                                                sizetype,
                                                EndLink)),
                    BUILT_IN_ALLOCA, "alloca");
    builtin_function ("__builtin_ffs", int_ftype_int, BUILT_IN_FFS, NULL_PTR);
    /* Define alloca, ffs as builtins.
       Declare _exit just to mark it as volatile.  */
    if (! c_flag_no_builtin && !c_flag_no_nonansi_builtin)
      {
        temp = builtin_function ("alloca",
                              c_tree_build_function_type(ptr_type_node,
                                                c_tree_add_head_list(NULL,
                                                          c_size_type,
                                                          EndLink)),
                              BUILT_IN_ALLOCA, NULL_PTR);
        /* Suppress error if redefined as a non-function.  */
        DECL_BUILT_IN_NONANSI (temp) = 1;
        temp = builtin_function ("ffs", int_ftype_int, BUILT_IN_FFS, NULL_PTR);
        /* Suppress error if redefined as a non-function.  */
        DECL_BUILT_IN_NONANSI (temp) = 1;
        temp = builtin_function ("_exit", void_ftype_any, NOT_BUILT_IN,
                              NULL_PTR);
        TREE_THIS_VOLATILE (temp) = 1;
        TREE_SIDE_EFFECTS (temp) = 1;
        /* Suppress error if redefined as a non-function.  */
        DECL_BUILT_IN_NONANSI (temp) = 1;
      }

    builtin_function ("__builtin_abs", int_ftype_int, BUILT_IN_ABS, NULL_PTR);
    builtin_function ("__builtin_fabsf", float_ftype_float, BUILT_IN_FABS,
                    NULL_PTR);
    builtin_function ("__builtin_fabs", double_ftype_double, BUILT_IN_FABS,
                    NULL_PTR);
    builtin_function ("__builtin_fabsl", ldouble_ftype_ldouble, BUILT_IN_FABS,
                    NULL_PTR);
    builtin_function ("__builtin_labs", long_ftype_long, BUILT_IN_LABS,
                    NULL_PTR);
    builtin_function ("__builtin_saveregs",
                    c_tree_build_function_type(ptr_type_node, NULL),
                    BUILT_IN_SAVEREGS, NULL_PTR);
# endif
  }

  c_pedantic_lvalues = c_pedantic;

# ifdef LUDO_NO_SKIP
  /* Create the global bindings for __FUNCTION__ and __PRETTY_FUNCTION__.  */
  declare_function_name ();

  start_identifier_warnings ();

  /* Prepare to check format strings against argument lists.  */
  init_function_format_info ();

  init_iterators ();

  incomplete_decl_finalize_hook = finish_incomplete_decl;
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                    Declaration Attributes                   |
|                                                             |
\------------------------------------------------------------*/

void c_decl_attributes( Node, Attributes, Prefix_attributes )

  c_tree_node *Node;
  c_tree_node *Attributes;
  c_tree_node *Prefix_attributes;
{
# ifdef LUDO_NO_SKIP
  c_tree_node *decl = 0;
  c_tree_node *type;
  int is_type;
  c_tree_node *a;

  if (attrtab_idx == 0)
    init_attributes ();

  if ( CTreeNodeFirstCode( node ) == C_DECL_NODE )
  {
      decl = node;
      type = CTreeType(decl);
      is_type = CTreeDeclCode(node) == C_TYPE_DECL;
  }
  else if ( CTreeNodeFirstCode( node ) == C_TYPE_NODE )
  {
    type = node, is_type = 1;
  }

  attributes = c_tree_concat_list(prefix_attributes, attributes);

  for (a = attributes; a; a = CTreeChain(a))
  {
    c_tree_node *name = CTreeListPurpose(a);
    c_tree_node *args = CTreeListValue(a);
    int i;
    int id;

# ifdef LUDO_NO_SKIP
    for (i = 0; i < attrtab_idx; i++)
      if (attrtab[i].name == name)
        break;
# else
    i = attrtab_idx;
# endif

# ifdef LUDO_NO_SKIP
    if (i == attrtab_idx)
    {
        if (! valid_machine_attribute(name, args, decl, type))
        {
          warning ("`%s' attribute directive ignored", CTreeIdentPointer(name));
        }
        else if (decl != 0)
          type = TREE_TYPE (decl);
        continue;
    }
    else if (attrtab[i].decl_req && decl == 0)
    {
        warning ("`%s' attribute does not apply to types",
                 IDENTIFIER_POINTER (name));
        continue;
    }
    else if (list_length (args) < attrtab[i].min
             || list_length (args) > attrtab[i].max)
    {
        error ("wrong number of arguments specified for `%s' attribute",
               IDENTIFIER_POINTER (name));
        continue;
    }

    id = attrtab[i].id;
    switch (id)
    {
      case A_PACKED:
        if (is_type)
          TYPE_PACKED (type) = 1;
        else if (TREE_CODE (decl) == FIELD_DECL)
          DECL_PACKED (decl) = 1;
        /* We can't set DECL_PACKED for a VAR_DECL, because the bit is
           used for DECL_REGISTER.  It wouldn't mean anything anyway.  */
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_NOCOMMON:
        if (TREE_CODE (decl) == VAR_DECL)
          DECL_COMMON (decl) = 0;
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_COMMON:
        if (TREE_CODE (decl) == VAR_DECL)
          DECL_COMMON (decl) = 1;
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_NORETURN:
        if (TREE_CODE (decl) == FUNCTION_DECL)
          TREE_THIS_VOLATILE (decl) = 1;
        else if (TREE_CODE (type) == POINTER_TYPE
                 && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
          TREE_TYPE (decl) = type 
            = c_tree_build_pointer_type
              (build_type_variant (TREE_TYPE (type),
                                   TREE_READONLY (TREE_TYPE (type)), 1));
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_UNUSED:
        if (is_type)
          TREE_USED (type) = 1;
        else if (TREE_CODE (decl) == PARM_DECL 
                 || TREE_CODE (decl) == VAR_DECL
                 || TREE_CODE (decl) == FUNCTION_DECL)
          CSetTreeNodeUsed(decl);
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_CONST:
        if (TREE_CODE (decl) == FUNCTION_DECL)
          TREE_READONLY (decl) = 1;
        else if (TREE_CODE (type) == POINTER_TYPE
                 && TREE_CODE (TREE_TYPE (type)) == FUNCTION_TYPE)
          TREE_TYPE (decl) = type
            = build_pointer_type
              (build_type_variant (TREE_TYPE (type), 1,
                                   TREE_THIS_VOLATILE (TREE_TYPE (type))));
        else
          warning ( "`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_T_UNION:
        if (is_type
            && TREE_CODE (type) == UNION_TYPE
            && (decl == 0
                || (TYPE_FIELDS (type) != 0
                    && TYPE_MODE (type) == DECL_MODE (TYPE_FIELDS (type)))))
          TYPE_TRANSPARENT_UNION (type) = 1;
        else if (decl != 0 && TREE_CODE (decl) == PARM_DECL
                 && TREE_CODE (type) == UNION_TYPE
                 && TYPE_MODE (type) == DECL_MODE (TYPE_FIELDS (type)))
          DECL_TRANSPARENT_UNION (decl) = 1;
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_CONSTRUCTOR:
        if (TREE_CODE (decl) == FUNCTION_DECL
            && TREE_CODE (type) == FUNCTION_TYPE
            && decl_function_context (decl) == 0)
          {
            DECL_STATIC_CONSTRUCTOR (decl) = 1;
            CSetTreeNodeUsed(decl);
          }
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_DESTRUCTOR:
        if (TREE_CODE (decl) == FUNCTION_DECL
            && TREE_CODE (type) == FUNCTION_TYPE
            && decl_function_context (decl) == 0)
          {
            DECL_STATIC_DESTRUCTOR (decl) = 1;
            CSetTreeNodeUsed(decl);
          }
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;

      case A_MODE:
        if (TREE_CODE (TREE_VALUE (args)) != IDENTIFIER_NODE)
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        else
          {
            int j;
            char *p = IDENTIFIER_POINTER (TREE_VALUE (args));
            int len = strlen (p);
            enum machine_mode mode = VOIDmode;
            tree typefm;

            if (len > 4 && p[0] == '_' && p[1] == '_'
                && p[len - 1] == '_' && p[len - 2] == '_')
              {
                char *newp = (char *) alloca (len - 1);

                strcpy (newp, &p[2]);
                newp[len - 4] = '\0';
                p = newp;
              }

            /* Give this decl a type with the specified mode.
               First check for the special modes.  */
            if (! strcmp (p, "byte"))
              mode = byte_mode;
            else if (!strcmp (p, "word"))
              mode = word_mode;
            else if (! strcmp (p, "pointer"))
              mode = ptr_mode;
            else
              for (j = 0; j < NUM_MACHINE_MODES; j++)
                if (!strcmp (p, GET_MODE_NAME (j)))
                  mode = (enum machine_mode) j;

            if (mode == VOIDmode)
              error ("unknown machine mode `%s'", p);
            else if (0 == (typefm = type_for_mode (mode,
                                                   TREE_UNSIGNED (type))))
              error ("no data type for mode `%s'", p);
            else
              {
                TREE_TYPE (decl) = type = typefm;
                DECL_SIZE (decl) = 0;
                c_layout_decl(decl, 0);
              }
          }
        break;

      case A_SECTION:
#ifdef ASM_OUTPUT_SECTION_NAME
        if ((TREE_CODE (decl) == FUNCTION_DECL
             || TREE_CODE (decl) == VAR_DECL)
            && TREE_CODE (TREE_VALUE (args)) == STRING_CST)
          {
            if (TREE_CODE (decl) == VAR_DECL 
                && current_function_decl != NULL
                && ! TREE_STATIC (decl))
            {
              error_with_decl (decl,
                "section attribute cannot be specified for local variables");
            /* The decl may have already been given a section attribute from
               a previous declaration.  Ensure they match.  */
            }
            else if (DECL_SECTION_NAME (decl) != NULL
                     && strcmp (TREE_STRING_POINTER (DECL_SECTION_NAME (decl)),
                                TREE_STRING_POINTER (TREE_VALUE (args))) != 0)
            {
              error_with_decl (node,
                               "section of `%s' conflicts with previous declaration");
            }
            else
              DECL_SECTION_NAME (decl) = TREE_VALUE (args);
          }
        else
          error_with_decl (node,
                         "section attribute not allowed for `%s'");
#else
        error_with_decl (node,
                "section attributes are not supported for this target");
#endif
        break;

      case A_ALIGNED:
        {
          tree align_expr
            = (args ? TREE_VALUE (args)
               : size_int (BIGGEST_ALIGNMENT / BITS_PER_UNIT));
          int align;

          /* Strip any NOPs of any kind.  */
          while (TREE_CODE (align_expr) == NOP_EXPR
                 || TREE_CODE (align_expr) == CONVERT_EXPR
                 || TREE_CODE (align_expr) == NON_LVALUE_EXPR)
            align_expr = TREE_OPERAND (align_expr, 0);
        
          if (TREE_CODE (align_expr) != INTEGER_CST)
            {
              error ("requested alignment is not a constant");
              continue;
            }

          align = TREE_INT_CST_LOW (align_expr) * BITS_PER_UNIT;

          if (exact_log2 (align) == -1)
            error ("requested alignment is not a power of 2");
          else if (is_type)
            TYPE_ALIGN (type) = align;
          else if (TREE_CODE (decl) != VAR_DECL
                   && TREE_CODE (decl) != FIELD_DECL)
          {
            error_with_decl (decl,
                             "alignment may not be specified for `%s'");
          }
          else
            DECL_ALIGN (decl) = align;
        }
        break;

      case A_FORMAT:
        {
          tree format_type = TREE_VALUE (args);
          tree format_num_expr = TREE_VALUE (TREE_CHAIN (args));
          tree first_arg_num_expr
            = TREE_VALUE (TREE_CHAIN (TREE_CHAIN (args)));
          int format_num;
          int first_arg_num;
          int is_scan;
          tree argument;
          int arg_num;
      
          if (TREE_CODE (decl) != FUNCTION_DECL)
            {
              error_with_decl (decl,
                       "argument format specified for non-function `%s'");
              continue;
            }
      
          if (TREE_CODE (format_type) == IDENTIFIER_NODE
              && (!strcmp (IDENTIFIER_POINTER (format_type), "printf")
                  || !strcmp (IDENTIFIER_POINTER (format_type),
                              "__printf__")))
            is_scan = 0;
          else if (TREE_CODE (format_type) == IDENTIFIER_NODE
                   && (!strcmp (IDENTIFIER_POINTER (format_type), "scanf")
                       || !strcmp (IDENTIFIER_POINTER (format_type),
                                   "__scanf__")))
            is_scan = 1;
          else if (TREE_CODE (format_type) == IDENTIFIER_NODE)
            {
              error ("`%s' is an unrecognized format function type",
                     IDENTIFIER_POINTER (format_type));
              continue;
            }
          else
            {
              error ("unrecognized format specifier");
              continue;
            }

          /* Strip any conversions from the string index and first arg number
             and verify they are constants.  */
          while (TREE_CODE (format_num_expr) == NOP_EXPR
                 || TREE_CODE (format_num_expr) == CONVERT_EXPR
                 || TREE_CODE (format_num_expr) == NON_LVALUE_EXPR)
            format_num_expr = TREE_OPERAND (format_num_expr, 0);

          while (TREE_CODE (first_arg_num_expr) == NOP_EXPR
                 || TREE_CODE (first_arg_num_expr) == CONVERT_EXPR
                 || TREE_CODE (first_arg_num_expr) == NON_LVALUE_EXPR)
            first_arg_num_expr = TREE_OPERAND (first_arg_num_expr, 0);

          if (TREE_CODE (format_num_expr) != INTEGER_CST
              || TREE_CODE (first_arg_num_expr) != INTEGER_CST)
            {
              error ("format string has non-constant operand number");
              continue;
            }

          format_num = TREE_INT_CST_LOW (format_num_expr);
          first_arg_num = TREE_INT_CST_LOW (first_arg_num_expr);
          if (first_arg_num != 0 && first_arg_num <= format_num)
            {
              error ("format string arg follows the args to be formatted");
              continue;
            }

          /* If a parameter list is specified, verify that the format_num
             argument is actually a string, in case the format attribute
             is in error.  */
          argument = TYPE_ARG_TYPES (type);
          if (argument)
            {
              for (arg_num = 1; ; ++arg_num)
                {
                  if (argument == 0 || arg_num == format_num)
                    break;
                  argument = TREE_CHAIN (argument);
                }
              if (! argument
                  || TREE_CODE (TREE_VALUE (argument)) != POINTER_TYPE
                || (CTreeTypeMainVariant (TREE_TYPE (TREE_VALUE (argument)))
                    != char_type_node))
                {
                  error ("format string arg not a string type");
                  continue;
                }
              if (first_arg_num != 0)
                {
                  /* Verify that first_arg_num points to the last arg,
                     the ...  */
                  while (argument)
                    arg_num++, argument = TREE_CHAIN (argument);
                if (arg_num != first_arg_num)
                  {
                    error ("args to be formatted is not ...");
                    continue;
                  }
                }
            }

          record_function_format (DECL_NAME (decl),
                                  DECL_ASSEMBLER_NAME (decl),
                                  is_scan, format_num, first_arg_num);
          break;
        }

      case A_FORMAT_ARG:
        {
          tree format_num_expr = TREE_VALUE (args);
          int format_num, arg_num;
          tree argument;
      
          if (TREE_CODE (decl) != FUNCTION_DECL)
            {
              error_with_decl (decl,
                       "argument format specified for non-function `%s'");
              continue;
            }
      
          /* Strip any conversions from the first arg number and verify it
             is a constant.  */
          while (TREE_CODE (format_num_expr) == NOP_EXPR
                 || TREE_CODE (format_num_expr) == CONVERT_EXPR
                 || TREE_CODE (format_num_expr) == NON_LVALUE_EXPR)
            format_num_expr = TREE_OPERAND (format_num_expr, 0);

          if (TREE_CODE (format_num_expr) != INTEGER_CST)
            {
              error ("format string has non-constant operand number");
              continue;
            }

          format_num = TREE_INT_CST_LOW (format_num_expr);

          /* If a parameter list is specified, verify that the format_num
             argument is actually a string, in case the format attribute
             is in error.  */
          argument = TYPE_ARG_TYPES (type);
          if (argument)
            {
              for (arg_num = 1; ; ++arg_num)
                {
                  if (argument == 0 || arg_num == format_num)
                    break;
                  argument = TREE_CHAIN (argument);
                }
              if (! argument
                  || TREE_CODE (TREE_VALUE (argument)) != POINTER_TYPE
                || (CTreeTypeMainVariant (TREE_TYPE (TREE_VALUE (argument)))
                    != char_type_node))
                {
                  error ("format string arg not a string type");
                  continue;
                }
            }

          if (TREE_CODE (TREE_TYPE (TREE_TYPE (decl))) != POINTER_TYPE
              || (CTreeTypeMainVariant (TREE_TYPE (TREE_TYPE (TREE_TYPE (decl))))
                  != char_type_node))
            {
              error ("function does not return string type");
              continue;
            }

          record_international_format (DECL_NAME (decl),
                                       DECL_ASSEMBLER_NAME (decl),
                                       format_num);
          break;
        }

      case A_WEAK:
        declare_weak (decl);
        break;

      case A_ALIAS:
        if ((TREE_CODE (decl) == FUNCTION_DECL && DECL_INITIAL (decl))
            || (TREE_CODE (decl) != FUNCTION_DECL && ! DECL_EXTERNAL (decl)))
        {
          error_with_decl (decl,
                           "`%s' defined both normally and as an alias");
        }
        else if (decl_function_context (decl) == 0)
          {
            tree id = get_identifier (TREE_STRING_POINTER
                                      (TREE_VALUE (args)));
            if (TREE_CODE (decl) == FUNCTION_DECL)
              DECL_INITIAL (decl) = error_mark_node;
            else
              DECL_EXTERNAL (decl) = 0;
            assemble_alias (decl, id);
          }
        else
          warning ("`%s' attribute ignored", IDENTIFIER_POINTER (name));
        break;
    }
# endif
  }
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                    Grant Fiel Declaration                   |
|                                                             |
\------------------------------------------------------------*/

/* Process the specs, declarator (NULL if omitted) and width (NULL if omitted)
   of a structure component, returning a FIELD_DECL node.
   WIDTH is non-NULL for bit fields only, and is an INTEGER_CST node.

   This is done during the parsing of the struct declaration.
   The FIELD_DECL nodes are chained together and the lot of them
   are ultimately passed to `build_struct' to make the RECORD_TYPE node.  */

c_tree_node *c_grokfield( Filename, Line, Declarator, Declspecs, Width)

     char          *Filename;
     int            Line;
     c_tree_node *Declarator;
     c_tree_node *Declspecs;
     c_tree_node *Width;
{
  c_tree_node *Value;

  Value = c_grokdeclarator( Declarator, Declspecs, 
              ( Width != NULL ) ? C_CDECL_BIT_FIELD : C_CDECL_FIELD, 0 );

  c_finish_decl( Value, NULL, NULL);
  Value->DECL.INITIAL = Width;

# ifdef LUDO_NO_SKIP
  c_maybe_objc_check_decl(value);
# endif
  return Value;
}

/*------------------------------------------------------------\
|                                                             |
|                    Grant Check Type Name                    |
|                                                             |
\------------------------------------------------------------*/

/* Decode a "typename", such as "int **", returning a ..._TYPE node.  */

c_tree_node *c_groktypename( Typename )

     c_tree_node *Typename;
{
  if ( CTreeNodeFirstCode( Typename ) == C_LIST_NODE )
  {
    Typename =  c_grokdeclarator( CTreeListValue( Typename ),
                                    CTreeListPurpose( Typename ), C_CDECL_TYPENAME, 0);
  }

  return( Typename );
}

/*------------------------------------------------------------\
|                                                             |
|                    Start Declaration                        |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_start_decl( Declarator, Declspecs, Initialized, Attributes, 
                               Prefix_attributes )
     c_tree_node *Declarator;
     c_tree_node *Declspecs;
     int            Initialized;
     c_tree_node *Attributes;
     c_tree_node *Prefix_attributes;
{
  c_tree_node *TreeDecl;
  c_tree_node *TreeType;
  c_tree_node *TreeTypeSize; 
  c_tree_node *TreeNode;
  int            Init_written;
  
  Init_written = Initialized;
  TreeDecl     = c_grokdeclarator( Declarator, Declspecs, C_CDECL_NORMAL, Initialized);

  if (c_warn_main && !strcmp( CTreeIdentPointer( Declarator ), "main" ) )
  {
    warning_with_decl( TreeDecl, "`%s' is usually a function");
  }

  if ( Initialized )
  {
    /* Is it valid for this decl to have an initializer at all?
       If not, set INITIALIZED to zero, which will indirectly
       tell `finish_decl' to ignore the initializer once it is parsed.  */
    switch ( CTreeDeclCode( TreeDecl ) )
    {
      case C_TYPE_DECL:
        /* typedef foo = bar  means give foo the same type as bar.
           We haven't parsed bar yet, so `finish_decl' will fix that up.
           Any other case of an initialization in a TYPE_DECL is an error.  */
        if ( c_pedantic || c_tree_get_list_length( Declspecs ) > 1)
        {
          error( "typedef `%s' is initialized",
                  CTreeIdentPointer( CTreeDeclName( TreeDecl ) ) );
          Initialized = 0;
        }
        break;

      case C_FUNCTION_DECL:
        error( "function `%s' is initialized like a variable",
               CTreeIdentPointer( CTreeDeclName( TreeDecl ) ) );
        Initialized = 0;
      break;

      case C_PARAM_DECL:
        /* DECL_INITIAL in a PARM_DECL is really DECL_ARG_TYPE.  */
        error( "parameter `%s' is initialized",
               CTreeIdentPointer( CTreeDeclName( TreeDecl ) ) );
        Initialized = 0;
      break;

      default:
        /* Don't allow initializations for incomplete types
           except for arrays which might be completed by the initialization.  */

        TreeType     = CTreeType( TreeDecl );
        TreeTypeSize = CTreeTypeSize( TreeType ); 

        if ( CTreeTypeSize( TreeType ) != 0)
        {
            /* A complete type is ok if size is fixed.  */
          if ( ( CTreeNodeFirstCode( TreeTypeSize ) != C_INT_CST_NODE )
                || CIsTreeDeclVarSize( TreeDecl ) )
          {
             error ("variable-sized object may not be initialized");
             Initialized = 0;
          }
        }
        else if ( CTreeTypeCode( TreeType ) != C_ARRAY_TYPE )
        {
          error ("variable `%s' has initializer but incomplete type",
                 CTreeIdentPointer( CTreeDeclName( TreeDecl ) ) );
          Initialized = 0;
        }
        else if ( CTreeTypeSize( CTreeType( TreeType ) ) == 0 )
        {
          error ("elements of array `%s' have incomplete type",
                  CTreeIdentPointer( CTreeDeclName( TreeDecl ) ) );
          Initialized = 0;
        }

      break;
    }
  }

  if ( Initialized )
  {
    CClearTreeNodeExternal( TreeDecl );

    if ( c_current_binding_level ==  c_global_binding_level)
    {
      CSetTreeNodeStatic( TreeDecl );
    }

      /* Tell `c_push_decl' this is an Initialized decl
         even though we don't yet have the initializer expression.
         Also tell `finish_decl' it may store the real initializer.  */

    TreeDecl->DECL.INITIAL = c_error_mark_node;
  }

  /* ANSI specifies that a tentative definition which is not merged with
     a non-tentative definition behaves exactly like a definition with an
     initializer equal to zero.  (Section 3.7.2)
     -fno-common gives strict ANSI behavior.  Usually you don't want it.
     This matters only for variables with external linkage.  */
  if ( ! c_flag_no_common || ! CIsTreeNodePublic( TreeDecl ) )
  {
    CSetTreeDeclCommon( TreeDecl );
  }

  /* Set attributes here so if duplicate decl, will have proper attributes.  */
  c_decl_attributes( TreeDecl, Attributes, Prefix_attributes );

  /* Add this decl to the current binding level.
     TEM may equal DECL or it may be a previous decl of the same name.  */
  TreeNode = c_push_decl( TreeDecl );

  /* For a local variable, define the RTL now.  */
# ifdef LUDO_NO_SKIP
  if ( c_current_binding_level !=  c_global_binding_level
      /* But not if this is a duplicate decl
         and we preserved the rtl from the previous one
         (which may or may not happen).  */
      && DECL_RTL (tem) == 0)
  {
    if (TYPE_SIZE (TREE_TYPE (tem)) != 0)
      expand_decl (tem);
    else if (TREE_CODE (TREE_TYPE (tem)) == ARRAY_TYPE
             && DECL_INITIAL (tem) != 0)
      expand_decl (tem);
  }
# endif

  return( TreeNode );
}

/*------------------------------------------------------------\
|                                                             |
|                   Finish Declaration                        |
|                                                             |
\------------------------------------------------------------*/

void c_finish_decl( decl, init, asmspec_tree)

  c_tree_node *decl;
  c_tree_node *init;
  c_tree_node *asmspec_tree;
{
  c_tree_node *type;
  int was_incomplete;
  char *asmspec = 0;

# ifdef LUDO_NO_SKIP
  int temporary = allocation_temporary_p ();
# endif

  type = CTreeType( decl );

  was_incomplete = ( CTreeDeclSize(decl) == (c_tree_node *)0);
  /* If a name was specified, get the string.   */
  if (asmspec_tree)
  {
    asmspec = CTreeStringPointer(asmspec_tree);
  }

  /* If `start_decl' didn't like having an initialization, ignore it now.  */
  if ((init != (c_tree_node *)0) && 
      (CTreeDeclInitial(decl) == (c_tree_node *)0))
  {
    init = (c_tree_node *)0;
  }

  /* Don't crash if parm is initialized.  */
  if ( CTreeDeclCode(decl) == C_PARAM_DECL)
  {
    init = (c_tree_node *)0;
  }

  if (CIsTreeDeclIterator(decl))
    {
      if (init == 0)
      {
        error_with_decl (decl, "iterator has no initial value");
      }
      else
      {
# ifdef LUDO_NO_SKIP
        init = save_expr (init);
# endif
      }
    }

  if (init != (c_tree_node *)0 )
  {
    if ( CTreeDeclCode(decl) != C_TYPE_DECL)
    {
      c_store_init_value( decl, init );
    }
    else
    {
        /* typedef foo = bar; store the type of bar as the type of foo.  */
      decl->COMMON.TYPE = CTreeType( init->COMMON.TYPE );
      decl->DECL.INITIAL = (c_tree_node *)0;
      init->DECL.INITIAL = (c_tree_node *)0;
    }
  }

  /* Pop back to the obstack that is current for this binding level.
     This is because MAXINDEX, rtl, etc. to be made below
     must go in the permanent obstack.  But don't discard the
     temporary data yet.  */
# ifdef LUDO_NO_SKIP
  pop_obstacks ();
# endif

  /* Deduce size of array from initialization, if not already known */

  if ( CTreeTypeCode(type) == C_ARRAY_TYPE
      && CTreeTypeValues(type) == 0
      && CTreeDeclCode(decl) != C_TYPE_DECL)
  {
      int do_default
        = ( CIsTreeNodeStatic(decl)
           /* Even if pedantic, an external linkage array
              may have incomplete type at first.  */
           ? c_pedantic && ! CIsTreeNodePublic(decl)
           : ! CIsTreeDeclExternal(decl));
      int failure
        = c_complete_array_type(type, CTreeDeclInitial(decl), do_default);

      /* Get the completed type made by complete_array_type.  */
      type = CTreeType(decl);

      if (failure == 1)
      {
        error_with_decl (decl, "initializer fails to determine size of `%s'");
      }

      if (failure == 2)
      {
          if (do_default)
          {
            error_with_decl (decl, "array size missing in `%s'");
          }
          /* If a `static' var's size isn't known,
             make it extern as well as static, so it does not get
             allocated.
             If it is not `static', then do not mark extern;
             finish_incomplete_decl will give it a default size
             and it will get allocated.  */
          else if (!c_pedantic && CIsTreeNodeStatic(decl) && ! CIsTreeNodePublic(decl))
          {
            CSetTreeDeclExternal( decl );
          }
      }

      /* TYPE_MAX_VALUE is always one less than the number of elements
         in the array, because we start counting at zero.  Therefore,
         warn only if the value is less than zero.  */
      if (c_pedantic && CTreeTypeValues(type) != 0
          && c_tree_is_int_cst_sgn( CTreeTypeMaxValue( CTreeTypeValues(type))) < 0)
      {
        error_with_decl (decl, "zero or negative size array `%s'");
      }

      c_layout_decl(decl, 0);
  }

  if ( CTreeDeclCode( decl ) == C_VAR_DECL )
  {
    if ( CTreeDeclSize(decl) == 0
        && CTreeTypeSize( CTreeType(decl)) != 0)
    {
      c_layout_decl(decl, 0);
    }

    if ( CTreeDeclSize(decl) == 0
          && ( CIsTreeNodeStatic(decl)
              ?
                /* A static variable with an incomplete type
                   is an error if it is initialized.
                   Also if it is not file scope.
                   Otherwise, let it through, but if it is not `extern'
                   then it may cause an error message later.  */
              /* A duplicate_decls call could have changed an extern
                 declaration into a file scope one.  This can be detected
                 by TREE_ASM_WRITTEN being set.  */
                ( CTreeDeclInitial(decl) != 0
                 || (CTreeDeclContext(decl) != 0 && ! CIsTreeNodeAsmWritten(decl)))
              :
                /* An automatic variable with an incomplete type
                   is an error.  */
                ! CIsTreeDeclExternal(decl)))
    {
      error_with_decl (decl, "storage size of `%s' isn't known");
      CTreeType(decl) = c_error_mark_node;
    }

    if ((  CIsTreeNodeExternal(decl) || CIsTreeNodeStatic(decl))
          && CTreeDeclSize(decl) != 0)
    {
      if ( CTreeNodeFirstCode( CTreeDeclSize(decl)) == C_INT_CST_NODE)
      {
        c_constant_expression_warning( CTreeDeclSize(decl));
      }
      else
      {
        error_with_decl (decl, "storage size of `%s' isn't constant");
      }
    }

    if ( CIsTreeNodeUsed(type)) CSetTreeNodeUsed(decl);
  }

  /* If this is a function and an assembler name is specified, it isn't
     builtin any more.  Also reset DECL_RTL so we can give it its new
     name.  */
  if ( CTreeDeclCode(decl) == C_FUNCTION_DECL && asmspec)
  {
    CClearTreeDeclBuiltIn(decl);
# ifdef LUDO_NO_SKIP
    DECL_RTL (decl) = 0;
# endif
  }

  /* Output the assembler code and/or RTL code for variables and functions,
     unless the type is an undefined structure or union.
     If not, it will get done when the type is completed.  */

  if ( CTreeDeclCode(decl) == C_VAR_DECL || CTreeDeclCode(decl) == C_FUNCTION_DECL)
  {
# ifdef LUDO_NO_SKIP
    if ((c_flag_traditional || CIsTreeNodePermanent(decl)) && 
        allocation_temporary_p())
    {
      push_obstacks_nochange ();
      end_temporary_allocation ();
      /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
      maybe_objc_check_decl (decl);
      rest_of_decl_compilation (decl, asmspec,
                                (DECL_CONTEXT (decl) == 0
                                 || CIsTreeNodeAsmWritten(decl)),
                                0);
      pop_obstacks ();
    }
    else
    {
      /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
      maybe_objc_check_decl (decl);
      rest_of_decl_compilation (decl, asmspec, DECL_CONTEXT (decl) == 0,
                                0);
    }
# endif

    if ( CTreeDeclContext(decl) != 0)
    {
      /* Recompute the RTL of a local array now
         if it used to be an incomplete type.  */
      if (was_incomplete
          && ! CIsTreeNodeStatic(decl) && ! CIsTreeDeclExternal(decl))
      {
        /* If we used it already as memory, it must stay in memory.  */
        if ( CIsTreeNodeUsed( decl ) ) CSetTreeNodeAddressable( decl );
        else                           CClearTreeNodeAddressable( decl );
        /* If it's still incomplete now, no init will save it.  */
        if ( CTreeDeclSize(decl) == 0) CTreeDeclInitial(decl) = 0;

# ifdef LUDO_NO_SKIP
        expand_decl (decl);
# endif
      }
      /* Compute and store the initial value.  */

# ifdef LUDO_NO_SKIP
      if ( CTreeDeclCode(decl) != C_FUNCTION_DECL)
        expand_decl_init (decl);
# endif
    }
  }

  if ( CTreeDeclCode(decl) == C_TYPE_DECL)
  {
# ifdef LUDO_NO_SKIP
      /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
    maybe_objc_check_decl (decl);
    rest_of_decl_compilation (decl, NULL_PTR, DECL_CONTEXT (decl) == 0, 0);
# endif
  }

  /* ??? After 2.3, test (init != 0) instead of TREE_CODE.  */
  /* This test used to include TREE_PERMANENT, however, we have the same
     problem with initializers at the function level.  Such initializers get
     saved until the end of the function on the momentary_obstack.  */
# ifdef LUDO_NO_SKIP
  if (!( CTreeDeclCode(decl) == C_FUNCTION_DECL && CIsTreeDeclInline(decl))
      && temporary
      /* DECL_INITIAL is not defined in PARM_DECLs, since it shares
         space with DECL_ARG_TYPE.  */
      && CTreeDeclCode(decl) != C_PARAM_DECL)
    {
      /* We need to remember that this array HAD an initialization,
         but discard the actual temporary nodes,
         since we can't have a permanent node keep pointing to them.  */
      /* We make an exception for inline functions, since it's
         normal for a local extern redeclaration of an inline function
         to have a copy of the top-level decl's DECL_INLINE.  */
      if ( CTreeDeclInitial(decl) != 0 && CTreeDeclInitial(decl) != c_error_mark_node)
        {
          /* If this is a const variable, then preserve the
             initializer instead of discarding it so that we can optimize
             references to it.  */
          /* This test used to include TREE_STATIC, but this won't be set
             for function level initializers.  */
          if ( CIsTreeNodeReadOnly(decl) || CIsTreeDeclIterator(decl))
            {
# ifdef LUDO_NO_SKIP
              c_preserve_initializer ();
# endif
              /* Hack?  Set the permanent bit for something that is permanent,
                 but not on the permanent obstack, so as to convince
                 output_constant_def to make its rtl on the permanent
                 obstack.  */
              CSetTreeNodePermanent( CTreeDeclInitial(decl));

              /* The initializer and DECL must have the same (or equivalent
                 types), but if the initializer is a STRING_CST, its type
                 might not be on the right obstack, so copy the type
                 of DECL.  */
              CTreeType( CTreeDeclInitial(decl)) = type;
            }
          else
            CTreeDeclInitial(decl) = c_error_mark_node;
        }
    }
# endif

  /* If requested, warn about definitions of large data objects.  */

  if (c_warn_larger_than
      && ( CTreeDeclCode(decl) == C_VAR_DECL || 
           CTreeDeclCode(decl) == C_PARAM_DECL)
      && ! CIsTreeDeclExternal(decl))
    {
      c_tree_node *decl_size = CTreeDeclSize(decl);

      if (decl_size && CTreeNodeFirstCode(decl_size) == C_INT_CST_NODE)
        {
           unsigned units = CTreeIntCstLow(decl_size) / C_BITS_PER_UNIT;

          if (units > c_larger_than_size)
          {
            warning_with_decl(decl, "size of `%s' is %u bytes", units);
          }
        }
    }

# ifdef LUDO_NO_SKIP
  /* If we have gone back from temporary to permanent allocation,
     actually free the temporary space that we no longer need.  */
  if (temporary && !allocation_temporary_p ())
    permanent_allocation (0);

  /* At the end of a declaration, throw away any variable type sizes
     of types defined inside that declaration.  There is no use
     computing them in the following function definition.  */
  if (current_binding_level == global_binding_level)
    c_get_pending_sizes ();
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                   Split Specification and Attributes        |
|                                                             |
\------------------------------------------------------------*/

/* Split SPECS_ATTRS, a list of declspecs and prefix attributes, into two
   lists.  SPECS_ATTRS may also be just a typespec (eg: RECORD_TYPE).

   The head of the declspec list is stored in DECLSPECS.
   The head of the attribute list is stored in PREFIX_ATTRIBUTES.

   Note that attributes in SPECS_ATTRS are stored in the TREE_PURPOSE of
   the list elements.  We drop the containing TREE_LIST nodes and link the
   resulting attributes together the way decl_attributes expects them.  */

void c_split_specs_attrs( Specs_attrs, Declspecs, Prefix_attributes )
     c_tree_node  *Specs_attrs;
     c_tree_node **Declspecs;
     c_tree_node **Prefix_attributes;
{
  c_tree_node *t, *s, *a, *next, *specs, *attrs;

  /* This can happen in c++ (eg: decl: typespec initdecls ';').  */
  if ((Specs_attrs != NULL)
      && CTreeNodeFirstCode(Specs_attrs) != C_LIST_NODE)
    {
      *Declspecs = Specs_attrs;
      *Prefix_attributes = NULL;
      return;
    }

  /* Remember to keep the lists in the same order, element-wise.  */

  specs = s = NULL;
  attrs = a = NULL;
  for (t = Specs_attrs; t; t = next)
    {
      next = CTreeChain(t);
      /* Declspecs have a non-NULL TREE_VALUE.  */
      if ( CTreeListValue(t) != NULL)
        {
          if (specs == NULL)
            specs = s = t;
          else
            {
              s->COMMON.CHAIN = t;
              s = t;
            }
        }
      else
        {
          if (attrs == NULL)
          {
            attrs = a = CTreeListPurpose(t);
          }
          else
            {
              a->COMMON.CHAIN = t->LIST.PURPOSE;
              a = t->LIST.PURPOSE;
            }
          /* More attrs can be linked here, move A to the end.  */
          while ( CTreeChain(a) != NULL)
            a = CTreeChain(a);
        }
    }

  /* Terminate the lists.  */
  if (s != NULL)
    s->COMMON.CHAIN = NULL;
  if (a != NULL)
    a->COMMON.CHAIN = NULL;

  /* All done.  */
  *Declspecs = specs;
  *Prefix_attributes = attrs;
}

/*------------------------------------------------------------\
|                                                             |
|                     Layout  Record                          |
|                                                             |
\------------------------------------------------------------*/


/* Lay out a RECORD_TYPE type (a C struct).
   This means laying out the fields, determining their positions,
   and computing the overall size and required alignment of the record.
   Note that if you set the TYPE_ALIGN before calling this
   then the struct is aligned to at least that boundary.

   If the type has basetypes, you must call layout_basetypes
   before calling this function.

   The return value is a list of static members of the record.
   They still need to be laid out.  */

static c_tree_node *c_layout_record(rec)
     c_tree_node *rec;
{
  c_tree_node *field;
# ifdef LUDO_NO_SKIP
  unsigned record_align = MAX(C_BITS_PER_UNIT, CTreeTypeAlign(rec));
# endif
  /* These must be laid out *after* the record is.  */
  c_tree_node *pending_statics = NULL;
  /* Record size so far is CONST_SIZE + VAR_SIZE bits,
     where CONST_SIZE is an integer
     and VAR_SIZE is a c_tree_node *expression.
     If VAR_SIZE is null, the size is just CONST_SIZE.
     Naturally we try to avoid using VAR_SIZE.  */
  int const_size = 0;
  c_tree_node *var_size = 0;
  /* Once we start using VAR_SIZE, this is the maximum alignment
     that we know VAR_SIZE has.  */
  int var_align = C_BITS_PER_UNIT;

#ifdef STRUCTURE_SIZE_BOUNDARY
  /* Packed structures don't need to have minimum size.  */
  if (! TYPE_PACKED (rec))
    record_align = MAX (record_align, STRUCTURE_SIZE_BOUNDARY);
#endif

  for (field = CTreeTypeValues(rec); field; field = CTreeChain(field))
  {
      int known_align = var_size ? var_align : const_size;
# ifdef LUDO_NO_SKIP
      int desired_align;
# endif

      /* If FIELD is static, then treat it like a separate variable,
         not really like a structure field.
         If it is a FUNCTION_DECL, it's a method.
         In both cases, all we do is lay out the decl,
         and we do it *after* the record is laid out.  */

      if ( CTreeDeclCode(field) == C_VAR_DECL)
        {
          pending_statics = c_tree_add_head_list(NULL, field, pending_statics);
          continue;
        }
      /* Enumerators and enum types which are local to this class need not
         be laid out.  Likewise for initialized constant fields.  */
      if ( CTreeDeclCode(field) != C_FIELD_DECL)
        continue;

      /* Lay out the field so we know what alignment it needs.
         For a packed field, use the alignment as specified,
         disregarding what the type would want.  */
# ifdef LUDO_NO_SKIP
      if (DECL_PACKED (field))
        desired_align = DECL_ALIGN (field);
# endif
      c_layout_decl(field, known_align);

# ifdef LUDO_NO_SKIP
      if (! DECL_PACKED (field))
        desired_align = DECL_ALIGN (field);
      /* Some targets (i.e. VMS) limit struct field alignment
         to a lower boundary than alignment of variables.  */
#ifdef BIGGEST_FIELD_ALIGNMENT
      desired_align = MIN (desired_align, BIGGEST_FIELD_ALIGNMENT);
#endif
#ifdef ADJUST_FIELD_ALIGN
      desired_align = ADJUST_FIELD_ALIGN (field, desired_align);
#endif

      /* Record must have at least as much alignment as any field.
         Otherwise, the alignment of the field within the record
         is meaningless.  */

#ifndef PCC_BITFIELD_TYPE_MATTERS
      record_align = MAX (record_align, desired_align);
#else
      if (PCC_BITFIELD_TYPE_MATTERS && CTreeType(field) != error_mark_node
          && DECL_BIT_FIELD_TYPE (field)
          && ! integer_zerop (CTreeTypeSize(CTreeType(field))))
        {
          /* For these machines, a zero-length field does not
             affect the alignment of the structure as a whole.
             It does, however, affect the alignment of the next field
             within the structure.  */
          if (! integer_zerop (DECL_SIZE (field)))
            record_align = MAX (record_align, desired_align);
          else if (! DECL_PACKED (field))
            desired_align = TYPE_ALIGN (CTreeType(field));
          /* A named bit field of declared type `int'
             forces the entire structure to have `int' alignment.  */
          if (CTreeDeclName(field) != 0)
            {
              int type_align = TYPE_ALIGN (CTreeType(field));
              if (maximum_field_alignment != 0)
                type_align = MIN (type_align, maximum_field_alignment);
              else if (DECL_PACKED (field))
                type_align = MIN (type_align, BITS_PER_UNIT);

              record_align = MAX (record_align, type_align);
            }
        }
      else
        record_align = MAX (record_align, desired_align);
#endif

      /* Does this field automatically have alignment it needs
         by virtue of the fields that precede it and the record's
         own alignment?  */

      if (const_size % desired_align != 0
          || (var_align % desired_align != 0
              && var_size != 0))
        {
          /* No, we need to skip space before this field.
             Bump the cumulative size to multiple of field alignment.  */

          if (var_size == 0
              || var_align % desired_align == 0)
            const_size
              = CEIL (const_size, desired_align) * desired_align;
          else
            {
              if (const_size > 0)
                var_size = c_build_size_binop(PLUS_EXPR, var_size,
                                       size_int (const_size));
              const_size = 0;
              var_size = round_up (var_size, desired_align);
              var_align = MIN (var_align, desired_align);
            }
        }

#ifdef PCC_BITFIELD_TYPE_MATTERS
      if (PCC_BITFIELD_TYPE_MATTERS
          && TREE_CODE (field) == FIELD_DECL
          && CTreeType(field) != error_mark_node
          && DECL_BIT_FIELD_TYPE (field)
          && !DECL_PACKED (field)
          && maximum_field_alignment == 0
          && !integer_zerop (DECL_SIZE (field)))
        {
          int type_align = TYPE_ALIGN (CTreeType(field));
          register c_tree_node *dsize = DECL_SIZE (field);
          int field_size = TREE_INT_CST_LOW (dsize);

          /* A bit field may not span more units of alignment of its type
             than its type itself.  Advance to next boundary if necessary.  */
          if (((const_size + field_size + type_align - 1) / type_align
               - const_size / type_align)
              > TREE_INT_CST_LOW (CTreeTypeSize(CTreeType(field))) / type_align)
            const_size = CEIL (const_size, type_align) * type_align;
        }
#endif

/* No existing machine description uses this parameter.
   So I have made it in this aspect identical to PCC_BITFIELD_TYPE_MATTERS.  */
#ifdef BITFIELD_NBYTES_LIMITED
      if (BITFIELD_NBYTES_LIMITED
          && TREE_CODE (field) == FIELD_DECL
          && CTreeType(field) != error_mark_node
          && DECL_BIT_FIELD_TYPE (field)
          && !DECL_PACKED (field)
          && !integer_zerop (DECL_SIZE (field)))
        {
          int type_align = TYPE_ALIGN (CTreeType(field));
          register c_tree_node *dsize = DECL_SIZE (field);
          int field_size = TREE_INT_CST_LOW (dsize);

          if (maximum_field_alignment != 0)
            type_align = MIN (type_align, maximum_field_alignment);
          /* ??? This test is opposite the test in the containing if
             statement, so this code is unreachable currently.  */
          else if (DECL_PACKED (field))
            type_align = MIN (type_align, BITS_PER_UNIT);

          /* A bit field may not span the unit of alignment of its type.
             Advance to next boundary if necessary.  */
          /* ??? This code should match the code above for the
             PCC_BITFIELD_TYPE_MATTERS case.  */
          if (const_size / type_align
              != (const_size + field_size - 1) / type_align)
            const_size = CEIL (const_size, type_align) * type_align;
        }
#endif

      /* Size so far becomes the position of this field.  */

      if (var_size && const_size)
        CTreeDeclArguments(field)
          = c_build_size_binop(PLUS_EXPR, var_size, size_int (const_size));
      else if (var_size)
        CTreeDeclArguments(field) = var_size;
      else
# endif
        {
          CTreeDeclArguments(field) = c_tree_get_size_int(const_size);

          /* If this field ended up more aligned than we thought it
             would be (we approximate this by seeing if its position
             changed), lay out the field again; perhaps we can use an
             integral mode for it now.  */
          if (known_align != const_size)
          {
            c_layout_decl(field, const_size);
          }
        }

      /* Now add size of this field to the size of the record.  */

      {
        c_tree_node *dsize = CTreeDeclSize(field);

        /* This can happen when we have an invalid nested struct definition,
           such as struct j { struct j { int i; } }.  The error message is
           printed in finish_struct.  */
        if (dsize == 0)
          /* Do nothing.  */;
        else if ( CTreeNodeFirstCode(dsize) == C_INT_CST_NODE
                 && ! CIsTreeNodeStatic(dsize)
                 && CTreeIntCstHigh(dsize) == 0
                 && CTreeIntCstLow(dsize) + const_size >= const_size)
        {
          /* Use const_size if there's no overflow.  */
          const_size += CTreeIntCstLow(dsize);
        }
        else
        {
          if (var_size == 0)
          {
            var_size = dsize;
          }
          else
          {
            var_size = c_build_size_binop( C_PLUS_EXPR, var_size, dsize);
          }
        }
      }
  }

  /* Work out the total size and alignment of the record
     as one expression and store in the record type.
     Round it up to a multiple of the record's alignment.  */

  if (var_size == 0)
    {
      CTreeTypeSize(rec) = c_tree_get_size_int(const_size);
    }
  else
    {
      if (const_size)
      {
        var_size = c_build_size_binop( C_PLUS_EXPR, var_size, c_tree_get_size_int( const_size ) );
      }
      CTreeTypeSize(rec) = var_size;
    }

# ifdef LUDO_NO_SKIP
  /* Determine the desired alignment.  */
#ifdef ROUND_TYPE_ALIGN
  TYPE_ALIGN (rec) = ROUND_TYPE_ALIGN (rec, TYPE_ALIGN (rec), record_align);
#else
  TYPE_ALIGN (rec) = MAX (TYPE_ALIGN (rec), record_align);
#endif

#ifdef ROUND_TYPE_SIZE
  CTreeTypeSize(rec) = ROUND_CTreeTypeSize(rec, CTreeTypeSize(rec), TYPE_ALIGN (rec));
#else
  /* Round the size up to be a multiple of the required alignment */
  CTreeTypeSize(rec) = round_up (CTreeTypeSize(rec), TYPE_ALIGN (rec));
#endif
# endif

  return pending_statics;
}

/* Return the value of VALUE, rounded up to a multiple of DIVISOR.  */

c_tree_node *c_round_up(value, divisor)
     c_tree_node *value;
     int divisor;
{
  c_tree_node *tmp;

  tmp = c_build_size_binop( C_CEIL_DIV_EXPR, value, c_tree_get_size_int(divisor));
  tmp = c_build_size_binop( C_MULT_EXPR, tmp, c_tree_get_size_int(divisor));

  return( tmp );
}

/*------------------------------------------------------------\
|                                                             |
|                     Layout  Union                           |
|                                                             |
\------------------------------------------------------------*/

/* Lay out a UNION_TYPE or QUAL_UNION_TYPE type.
   Lay out all the fields, set their positions to zero,
   and compute the size and alignment of the union (maximum of any field).
   Note that if you set the TYPE_ALIGN before calling this
   then the union align is aligned to at least that boundary.  */

static void c_layout_union (rec)
     c_tree_node *rec;
{
  c_tree_node *field;
# ifdef LUDO_NO_SKIP
  unsigned union_align = C_BITS_PER_UNIT;
# endif

  /* The size of the union, based on the fields scanned so far,
     is max (CONST_SIZE, VAR_SIZE).
     VAR_SIZE may be null; then CONST_SIZE by itself is the size.  */
  int const_size = 0;
  c_tree_node *var_size = 0;

#ifdef STRUCTURE_SIZE_BOUNDARY
  /* Packed structures don't need to have minimum size.  */
  if (! TYPE_PACKED (rec))
    union_align = STRUCTURE_SIZE_BOUNDARY;
#endif

  /* If this is a QUAL_UNION_TYPE, we want to process the fields in
     the reverse order in building the COND_EXPR that denotes its
     size.  We reverse them again later.  */
  if ( CTreeTypeCode(rec) == C_QUAL_UNION_TYPE)
  {
    CTreeTypeValues(rec) = c_tree_reverse_list( CTreeTypeValues(rec));
  }

  for (field = CTreeTypeValues(rec); field; field = CTreeChain(field))
    {
      /* Enums which are local to this class need not be laid out.  */
      if ( CTreeDeclCode(field) == C_CONST_DECL || CTreeDeclCode(field) == C_TYPE_DECL)
        continue;

      c_layout_decl(field, 0);
      CTreeDeclArguments(field) = c_tree_get_size_int(0);

      /* Union must be at least as aligned as any field requires.  */

# ifdef LUDO_NO_SKIP
      union_align = MAX (union_align, DECL_ALIGN (field));

#ifdef PCC_BITFIELD_TYPE_MATTERS
      /* On the m88000, a bit field of declare type `int'
         forces the entire union to have `int' alignment.  */
      if (PCC_BITFIELD_TYPE_MATTERS && DECL_BIT_FIELD_TYPE (field))
        union_align = MAX (union_align, TYPE_ALIGN (CTreeType(field)));
#endif
# endif

      if ( CTreeTypeCode(rec) == C_UNION_TYPE)
        {
          /* Set union_size to max (decl_size, union_size).
             There are more and less general ways to do this.
             Use only CONST_SIZE unless forced to use VAR_SIZE.  */

          if ( CTreeNodeFirstCode( CTreeDeclSize(field)) == C_INT_CST_NODE)
          {
            const_size
              = MAX(const_size, CTreeIntCstLow( CTreeDeclSize(field)));
          }
          else if (var_size == 0)
          {
            var_size = CTreeDeclSize(field);
          }
          else
          {
            var_size = c_build_size_binop(
                 C_MAX_EXPR, var_size, CTreeDeclSize(field));
          }
        }
      else if ( CTreeTypeCode(rec) == C_QUAL_UNION_TYPE)
      {
        var_size = c_tree_build_ternary_expr( C_COND_EXPR,
            CTreeDeclInitial( field ), CTreeDeclSize( field ),
            var_size ? var_size : c_integer_zero_node );
        CTreeType( var_size ) = c_size_type;
      }
    }

  if ( CTreeTypeCode(rec) == C_QUAL_UNION_TYPE)
  {
    CTreeTypeValues(rec) = c_tree_reverse_list( CTreeTypeValues(rec));
  }

  /* Determine the ultimate size of the union (in bytes).  */
  if (var_size == NULL)
  {
#define CEIL(x,y) (((x) + (y) - 1) / (y))

    CTreeTypeSize(rec) = c_tree_get_size_int(
        CEIL(const_size, C_BITS_PER_UNIT) * C_BITS_PER_UNIT);
  }
  else if (const_size == 0)
  {
    CTreeTypeSize(rec) = var_size;
  }
  else
  {
    c_tree_node *tmp_node;

    tmp_node = c_round_up( c_tree_get_size_int(const_size), C_BITS_PER_UNIT);
    tmp_node = c_build_size_binop( C_MAX_EXPR, var_size, tmp_node );
    CTreeTypeSize(rec) = tmp_node;
  }

# ifdef LUDO_NO_SKIP
  /* Determine the desired alignment.  */
#ifdef ROUND_TYPE_ALIGN
  TYPE_ALIGN (rec) = ROUND_TYPE_ALIGN (rec, TYPE_ALIGN (rec), union_align);
#else
  TYPE_ALIGN (rec) = MAX (TYPE_ALIGN (rec), union_align);
#endif
# endif

#ifdef ROUND_TYPE_SIZE
  CTreeTypeSize(rec) = ROUND_CTreeTypeSize(rec, CTreeTypeSize(rec), TYPE_ALIGN (rec));
#else
  /* Round the size up to be a multiple of the required alignment */
  CTreeTypeSize(rec) = c_round_up( CTreeTypeSize(rec), CTreeTypeAlign(rec));
#endif
}


/*------------------------------------------------------------\
|                                                             |
|                     Layout  Declaration                     |
|                                                             |
\------------------------------------------------------------*/

/* Set the size, mode and alignment of a ..._DECL node.
   TYPE_DECL does need this for C++.
   Note that LABEL_DECL and CONST_DECL nodes do not need this,
   and FUNCTION_DECL nodes have them set up in a special (and simple) way.
   Don't call layout_decl for them.

   KNOWN_ALIGN is the amount of alignment we can assume this
   decl has with no special effort.  It is relevant only for FIELD_DECLs
   and depends on the previous fields.
   All that matters about KNOWN_ALIGN is which powers of 2 divide it.
   If KNOWN_ALIGN is 0, it means, "as much alignment as you like":
   the record will be aligned to suit.  */

void c_layout_decl(decl, known_align)
     c_tree_node *decl;
     unsigned known_align;
{
  c_tree_node *type = CTreeType(decl);
  int code = CTreeDeclCode(decl);
  int spec_size = CTreeDeclFieldSize(decl);

  if (code == C_CONST_DECL)
  {
    return;
  }

  if (code != C_VAR_DECL && code != C_PARAM_DECL && code != C_RESULT_DECL
      && code != C_FIELD_DECL && code != C_TYPE_DECL)
  {
    abort ();
  }

  if (type == c_error_mark_node)
  {
    type = c_void_type_node;
    spec_size = 0;
  }

  /* Usually the size and mode come from the data type without change.  */

  decl->DECL.MODE = CTreeTypeMode(type);
  if ( CIsTreeNodeUnsigned( type ) )
  {
    CSetTreeNodeUnsigned( decl );
  }

  if ( CTreeDeclSize(decl) == (c_tree_node *)0)
  {
    decl->DECL.SIZE = type->TYPE.SIZE;
  }

  if (code == C_FIELD_DECL && CIsTreeDeclBitField(decl))
  {
    if (spec_size == 0 && CTreeDeclName(decl) != 0)
    {
      abort ();
    }

    /* Size is specified number of bits.  */
    CTreeDeclSize(decl) = c_tree_get_size_int(spec_size);
  }
  /* Force alignment required for the data type.
     But if the decl itself wants greater alignment, don't override that.
     Likewise, if the decl is packed, don't override it.  */
# ifdef LUDO_NO_SKIP
  else if (DECL_ALIGN (decl) == 0
         || (! DECL_PACKED (decl) &&  TYPE_ALIGN (type) > DECL_ALIGN (decl)))
    DECL_ALIGN (decl) = TYPE_ALIGN (type);
# endif

  /* See if we can use an ordinary integer mode for a bit-field.  */
  /* Conditions are: a fixed size that is correct for another mode
     and occupying a complete byte or bytes on proper boundary.  */
  if (code == C_FIELD_DECL)
    {
      CTreeDeclResult(decl) = CIsTreeDeclBitField(decl) ? type : 0;
# ifdef LUDO_NO_SKIP
      if ( maximum_field_alignment != 0)
      DECL_ALIGN (decl) = MIN (DECL_ALIGN (decl), maximum_field_alignment);
      else if (DECL_PACKED (decl))
      DECL_ALIGN (decl) = MIN (DECL_ALIGN (decl), BITS_PER_UNIT);
# endif
    }

# ifdef LUDO_NO_SKIP
  if ( CIsTreeDeclBitField(decl)
      && CTreeTypeSize(type) != 0
      && CTreeNodeFirstCode( CTreeTypeSize(type)) == C_INT_CST_NODE
      && GET_MODE_CLASS (TYPE_MODE (type)) == MODE_INT
      )
    {
      register enum machine_mode xmode
      = mode_for_size (TREE_INT_CST_LOW (DECL_SIZE (decl)), MODE_INT, 1);

      if (xmode != BLKmode
        && known_align % GET_MODE_ALIGNMENT (xmode) == 0)
      {
        DECL_ALIGN (decl) = MAX (GET_MODE_ALIGNMENT (xmode),
                           DECL_ALIGN (decl));
        DECL_MODE (decl) = xmode;
        DECL_SIZE (decl) = size_int (GET_MODE_BITSIZE (xmode));
        /* This no longer needs to be accessed as a bit field.  */
        CClearTreeDeclBitField(decl);
      }
    }

  /* Turn off DECL_BIT_FIELD if we won't need it set.  */
  if (DECL_BIT_FIELD (decl) && TYPE_MODE (type) == BLKmode
      && known_align % TYPE_ALIGN (type) == 0
      && DECL_SIZE (decl) != 0
      && (TREE_CODE (DECL_SIZE (decl)) != INTEGER_CST
        || (TREE_INT_CST_LOW (DECL_SIZE (decl)) % BITS_PER_UNIT) == 0)
      && DECL_ALIGN (decl) >= TYPE_ALIGN (type))
    DECL_BIT_FIELD (decl) = 0;

  /* Evaluate nonconstant size only once, either now or as soon as safe.  */
  if (CTreeDeclSize(decl) != 0 && CTreeNodeFirstCode(CTreeDeclSize(decl)) != C_INT_CST_NODE)
    CTreeDeclSize(decl) = variable_size (DECL_SIZE (decl));
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                     Layout  Type                            |
|                                                             |
\------------------------------------------------------------*/

/* Calculate the mode, size, and alignment for TYPE.
   For an array type, calculate the element separation as well.
   Record TYPE on the chain of permanent or temporary types
   so that dbxout will find out about it.

   TYPE_SIZE of a type is nonzero if the type has been laid out already.
   c_layout_type does nothing on such a type.

   If the type is incomplete, its TYPE_SIZE remains zero.  */

void c_layout_type(type)
     c_tree_node *type;
{
# ifdef LUDO_NO_SKIP
  int old;
# endif
  c_tree_node *pending_statics;

  if (type == 0)
  {
    abort ();
  }

  /* Do nothing if type has been laid out before.  */
  if ( CTreeTypeSize(type)) return;

# ifdef LUDO_NO_SKIP
  /* Make sure all nodes we allocate are not momentary;
     they must last past the current statement.  */
  old = suspend_momentary ();

  /* Put all our nodes into the same obstack as the type.  Also,
     make expressions saveable (this is a no-op for permanent types).  */

  push_obstacks (TYPE_OBSTACK (type), TYPE_OBSTACK (type));
  saveable_allocation ();
# endif

  switch ( CTreeTypeCode( type ) )
  {
    case C_LANG_TYPE:
      /* This kind of type is the responsibility
         of the language-specific code.  */
      abort ();

    case C_BOOLEAN_TYPE:  /* Used for Java, Pascal, and Chill. */
      if ( CTreeTypePrecision(type) == 0)
      {
        type->TYPE.PRECISION = 1; /* default to one byte/boolean. */
      }
      /* ... fall through ... */

    case C_INTEGER_TYPE:
    case C_ENUMERAL_TYPE:
    case C_CHAR_TYPE:
      if ( (CTreeNodeFirstCode(TYPE_MIN_VALUE(type)) == C_INT_CST_NODE )
          && c_tree_is_int_cst_sgn(TYPE_MIN_VALUE (type)) >= 0)
      {
        CSetTreeNodeUnsigned(type);
      }
# ifdef LUDO_NO_SKIP
      type->TYPE.MODE = c_get_smallest_mode_for_size( TYPE_PRECISION(type), MODE_INT );
      type->TYPE.SIZE = c_get_size_int( GET_MODE_BITSIZE( CTreeTypeMode(type)));
# else
      type->TYPE.SIZE = c_tree_get_size_int( CTreeTypePrecision( type ) );
# endif

      break;

    case C_REAL_TYPE:
# ifdef LUDO_NO_SKIP
      type->TYPE.MODE = c_get_mode_for_size( CTreeTypePrecision(type), MODE_FLOAT, 0);
      type->TYPE.SIZE = c_get_size_int( GET_MODE_BITSIZE( CTreeTypeMode(type)));
# else
      type->TYPE.SIZE = c_tree_get_size_int( CTreeTypePrecision( type ) );
# endif
      break;

    case C_COMPLEX_TYPE:
      if ( CIsTreeNodeUnsigned( CTreeType(type)))
      {
        CSetTreeNodeUnsigned( type );
      }
# ifdef LUDO_NO_SKIP
      TYPE_MODE (type)
        = mode_for_size (2 * TYPE_PRECISION (TREE_TYPE (type)),
                         (TREE_CODE (TREE_TYPE (type)) == INTEGER_TYPE
                          ? MODE_COMPLEX_INT : MODE_COMPLEX_FLOAT),
                         0);
      TYPE_SIZE (type) = size_int (GET_MODE_BITSIZE (TYPE_MODE (type)));
# else
      type->TYPE.SIZE = c_tree_get_size_int( CTreeTypePrecision( type ) );
# endif
      break;

    case C_VOID_TYPE:
      CTreeTypeSize(type) = c_size_zero_node;
      CTreeTypeAlign( type ) = 1;
# ifdef LUDO_NO_SKIP
      TYPE_MODE (type) = VOIDmode;
# endif
      break;

    case C_OFFSET_TYPE:
       CTreeTypeSize(type) = c_tree_get_size_int( C_POINTER_TYPE_SIZE );
# ifdef LUDO_NO_SKIP
      TYPE_MODE (type) = ptr_mode;
# endif
      break;

    case C_FUNCTION_TYPE:
    case C_METHOD_TYPE:
# ifdef LUDO_NO_SKIP
      TYPE_MODE (type) = mode_for_size (2 * POINTER_SIZE, MODE_INT, 0);
# endif
      CTreeTypeSize(type) = c_tree_get_size_int( C_POINTER_TYPE_SIZE * 2 );
      break;

    case C_POINTER_TYPE:
    case C_REFERENCE_TYPE:
# ifdef LUDO_NO_SKIP
      TYPE_MODE (type) = ptr_mode;
# endif
      CTreeTypeSize(type) = c_tree_get_size_int( C_POINTER_TYPE_SIZE);
      CSetTreeNodeUnsigned(type);
      CTreeTypePrecision(type) = C_POINTER_TYPE_SIZE;
      break;

    case C_ARRAY_TYPE:
      {
        c_tree_node * index = CTreeTypeValues(type);
        c_tree_node * element = CTreeType(type);

        c_tree_build_pointer_type(element);

        /* We need to know both bounds in order to compute the size.  */
        if (index && CTreeTypeMaxValue(index) && CTreeTypeMinValue(index)
            && CTreeTypeSize(element))
          {
            c_tree_node * ub = CTreeTypeMaxValue(index);
            c_tree_node * lb = CTreeTypeMinValue(index);
            c_tree_node * length;
            c_tree_node * tmp_node;

            /* If UB is max (lb - 1, x), remove the MAX_EXPR since the
               test for negative below covers it.  */
            if ( CTreeNodeCode(ub) == C_MAX_EXPR
                &&  CTreeNodeCode( CTreeExprOperand(ub, 0)) == C_MINUS_EXPR
                && c_tree_is_integer_onep ( CTreeExprOperand( CTreeExprOperand(ub, 0), 1))
                && c_is_operand_equal_p ( CTreeExprOperand( CTreeExprOperand(ub, 0), 0),
                                    lb, 0))
              ub = CTreeExprOperand(ub, 1);
            else if ( CTreeNodeCode(ub) == C_MAX_EXPR
                     && CTreeNodeCode( CTreeExprOperand(ub, 1)) == C_MINUS_EXPR
                     && c_tree_is_integer_onep( CTreeExprOperand( CTreeExprOperand(ub, 1), 1))
                     && c_is_operand_equal_p( CTreeExprOperand( CTreeExprOperand(ub, 1),
                                                       0),
                                         lb, 0))
              ub = CTreeExprOperand(ub, 0);
            tmp_node = c_build_size_binop( C_MINUS_EXPR, ub, lb );
            length = c_build_size_binop( C_PLUS_EXPR, c_size_one_node, tmp_node );

            /* If neither bound is a constant and sizetype is signed, make
               sure the size is never negative.  We should really do this
               if *either* bound is non-constant, but this is the best
               compromise between C and Ada.  */
            if (! CIsTreeNodeUnsigned(c_size_type)
                && CTreeNodeFirstCode( CTreeTypeMinValue(index)) != C_INT_CST_NODE
                && CTreeNodeFirstCode( CTreeTypeMaxValue(index)) != C_INT_CST_NODE)
            {
              length = c_build_size_binop( C_MAX_EXPR, length, c_size_zero_node );
            }

            tmp_node = c_build_size_binop( C_MULT_EXPR, length, CTreeTypeSize( element ) );
            CTreeTypeSize(type) = tmp_node;
          }

        /* Now round the alignment and size,
           using machine-dependent criteria if any.  */

# ifdef LUDO_NO_SKIP
#ifdef ROUND_TYPE_ALIGN
        TYPE_ALIGN (type)
          = ROUND_TYPE_ALIGN (type, TYPE_ALIGN (element), BITS_PER_UNIT);
#else
        TYPE_ALIGN (type) = MAX (TYPE_ALIGN (element), BITS_PER_UNIT);
#endif

#ifdef ROUND_TYPE_SIZE
        if (TYPE_SIZE (type) != 0)
          TYPE_SIZE (type)
            = ROUND_TYPE_SIZE (type, TYPE_SIZE (type), TYPE_ALIGN (type));
#endif

        TYPE_MODE (type) = BLKmode;
        if ( CTreeTypeSize(type) != 0
            && CTreeNodeFirstCode( CTreeTypeSize(type)) == C_INT_CST_NODE
            /* BLKmode elements force BLKmode aggregate;
               else extract/store fields may lose.  */
            && (TYPE_MODE (TREE_TYPE (type)) != BLKmode
                || TYPE_NO_FORCE_BLK (TREE_TYPE (type))))
        {
          TYPE_MODE (type)
            = mode_for_size (TREE_INT_CST_LOW (TYPE_SIZE (type)),
                              MODE_INT, 1);

          if (STRICT_ALIGNMENT && TYPE_ALIGN (type) < BIGGEST_ALIGNMENT
              && TYPE_ALIGN (type) < TREE_INT_CST_LOW (TYPE_SIZE (type))
              && TYPE_MODE (type) != BLKmode)
            {
              TYPE_NO_FORCE_BLK (type) = 1;
              TYPE_MODE (type) = BLKmode;
            }
        }
# endif
        break;
      }

    case C_RECORD_TYPE:
      pending_statics = c_layout_record(type);
# ifdef LUDO_NO_SKIP
      TYPE_MODE (type) = BLKmode;
      if ( CTreeNodeFirstCode( CTreeTypeSize(type)) == C_INT_CST_NODE)
        {
          c_tree_node * field;
          enum machine_mode mode = VOIDmode;

          /* A record which has any BLKmode members must itself be BLKmode;
             it can't go in a register.
             Unless the member is BLKmode only because it isn't aligned.  */
          for (field = CTreeTypeValues(type); field; field = CTreeChain(field))
            {
              int bitpos;

              if ( CTreeDeclCode(field) != C_FIELD_DECL) continue;

              if (TYPE_MODE (TREE_TYPE (field)) == BLKmode
                  && ! TYPE_NO_FORCE_BLK (TREE_TYPE (field)))
                goto record_lose;

              if ( CTreeNodeFirstCode( CTreeDeclArguments(field)) != C_INT_CST_NODE)
              {
                goto record_lose;
              }

              bitpos = CTreeIntCstLow( CTreeDeclArguments(field));

              /* Must be BLKmode if any field crosses a word boundary,
                 since extract_bit_field can't handle that in registers.  */
              if (bitpos / C_BITS_PER_WORD
                  != (( CTreeIntCstLow( CTreeDeclSize(field)) + bitpos - 1)
                      / C_BITS_PER_WORD)
                  /* But there is no problem if the field is entire words.  */
                  && CTreeIntCstLow( CTreeDeclSize(field)) % C_BITS_PER_WORD != 0)
              {
                goto record_lose;
              }

              /* If this field is the whole struct, remember its mode so
                 that, say, we can put a double in a class into a DF
                 register instead of forcing it to live in the stack.  */
              if (simple_cst_equal (TYPE_SIZE (type), DECL_SIZE (field)))
              {
                mode = DECL_MODE (field);
              }
            }

          if (mode != VOIDmode)
            /* We only have one real field; use its mode.  */
            TYPE_MODE (type) = mode;
          else
            TYPE_MODE (type)
              = mode_for_size (TREE_INT_CST_LOW (TYPE_SIZE (type)),
                               MODE_INT, 1);

          /* If structure's known alignment is less than
             what the scalar mode would need, and it matters,
             then stick with BLKmode.  */
          if (STRICT_ALIGNMENT
              && ! (TYPE_ALIGN (type) >= BIGGEST_ALIGNMENT
                    || (TYPE_ALIGN (type)
                        >= TREE_INT_CST_LOW (TYPE_SIZE (type)))))
            {
              if (TYPE_MODE (type) != BLKmode)
                /* If this is the only reason this type is BLKmode,
                   then don't force containing types to be BLKmode.  */
                TYPE_NO_FORCE_BLK (type) = 1;
              TYPE_MODE (type) = BLKmode;
            }

        record_lose: ;
        }
# endif

      /* Lay out any static members.  This is done now
         because their type may use the record's type.  */
      while (pending_statics)
        {
          c_layout_decl( CTreeListValue(pending_statics), 0);
          pending_statics = CTreeChain(pending_statics);
        }
      break;

    case C_UNION_TYPE:
    case C_QUAL_UNION_TYPE:
      c_layout_union(type);

# ifdef LUDO_NO_SKIP
      TYPE_MODE (type) = BLKmode;
      if (TREE_CODE (TYPE_SIZE (type)) == INTEGER_CST
          /* If structure's known alignment is less than
             what the scalar mode would need, and it matters,
             then stick with BLKmode.  */
          && (! STRICT_ALIGNMENT
              || TYPE_ALIGN (type) >= BIGGEST_ALIGNMENT
              || TYPE_ALIGN (type) >= TREE_INT_CST_LOW (TYPE_SIZE (type))))
        {
          c_tree_node * field;
          /* A union which has any BLKmode members must itself be BLKmode;
             it can't go in a register.
             Unless the member is BLKmode only because it isn't aligned.  */
          for (field = TYPE_FIELDS (type); field; field = TREE_CHAIN (field))
            {
              if (TREE_CODE (field) != FIELD_DECL)
                continue;

              if (TYPE_MODE (TREE_TYPE (field)) == BLKmode
                  && ! TYPE_NO_FORCE_BLK (TREE_TYPE (field)))
                goto union_lose;
            }

          TYPE_MODE (type)
            = mode_for_size (TREE_INT_CST_LOW (TYPE_SIZE (type)),
                             MODE_INT, 1);

        union_lose: ;
        }
# endif
      break;

    case C_SET_TYPE:  /* Used by Chill and Pascal. */
      if ( CTreeNodeFirstCode( CTreeTypeMaxValue( CTreeTypeValues(type))) != C_INT_CST_NODE
          ||  CTreeNodeFirstCode( CTreeTypeMinValue( CTreeTypeValues(type))) != C_INT_CST_NODE)
        abort();
      else
        {
#ifndef SET_WORD_SIZE
#define SET_WORD_SIZE BITS_PER_WORD
#endif
# ifdef LUDO_NO_SKIP
          int alignment = set_alignment ? set_alignment : SET_WORD_SIZE;
# endif
          int size_in_bits
            = ( CTreeIntCstLow( CTreeTypeMaxValue( CTreeTypeValues(type)))
               - CTreeIntCstLow( CTreeTypeMinValue( CTreeTypeValues(type))) + 1);
# ifdef LUDO_NO_SKIP
          int rounded_size
            = ((size_in_bits + alignment - 1) / alignment) * alignment;
          if (rounded_size > alignment)
            TYPE_MODE (type) = BLKmode;
          else
            TYPE_MODE (type) = mode_for_size (alignment, MODE_INT, 1);
          TYPE_SIZE (type) = c_tree_get_size_int(rounded_size);
          TYPE_ALIGN (type) = alignment;
# endif
          CTreeTypePrecision(type) = size_in_bits;
        }
      break;

    case C_FILE_TYPE:
# ifdef LUDO_NO_SKIP
      /* The size may vary in different languages, so the language front end
         should fill in the size.  */
      TYPE_ALIGN (type) = BIGGEST_ALIGNMENT;
      TYPE_MODE  (type) = BLKmode;
# endif
      break;

    default:

      abort ();
  } /* end switch */

  /* Normally, use the alignment corresponding to the mode chosen.
     However, where strict alignment is not required, avoid
     over-aligning structures, since most compilers do not do this
     alignment.  */

# ifdef LUDO_NO_SKIP
  if (TYPE_MODE (type) != BLKmode && TYPE_MODE (type) != VOIDmode
      && (STRICT_ALIGNMENT
          || (TREE_CODE (type) != RECORD_TYPE && TREE_CODE (type) != UNION_TYPE
              && TREE_CODE (type) != QUAL_UNION_TYPE
              && TREE_CODE (type) != ARRAY_TYPE)))
    TYPE_ALIGN (type) = GET_MODE_ALIGNMENT (TYPE_MODE (type));

  /* Evaluate nonconstant size only once, either now or as soon as safe.  */
  if (TYPE_SIZE (type) != 0 && TREE_CODE (TYPE_SIZE (type)) != INTEGER_CST)
    TYPE_SIZE (type) = variable_size (TYPE_SIZE (type));
# endif

  /* Also layout any other variants of the type.  */
  if ( CTreeTypeNextVariant(type)
      || type != CTreeTypeMainVariant(type))
    {
      c_tree_node * variant;
      /* Record layout info of this variant.  */
      c_tree_node * size = CTreeTypeSize (type);
# ifdef LUDO_NO_SKIP
      int align = TYPE_ALIGN (type);
      enum machine_mode mode = TYPE_MODE (type);
# endif

      /* Copy it into all variants.  */
      for (variant = CTreeTypeMainVariant(type);
           variant;
           variant = CTreeTypeNextVariant(variant))
        {
          CTreeTypeSize(variant) = size;
# ifdef LUDO_NO_SKIP
          TYPE_ALIGN (variant) = align;
          TYPE_MODE (variant) = mode;
# endif
        }
    }
        
# ifdef LUDO_NO_SKIP
  pop_obstacks ();
  resume_momentary (old);
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                     Param List Tags Warning                 |
|                                                             |
\------------------------------------------------------------*/

/* At end of parameter list, warn about any struct, union or enum tags
   defined within.  Do so because these types cannot ever become complete.  */

void c_parmlist_tags_warning()
{
  c_tree_node *elt;
  static int already;

  for (elt = c_current_binding_level->TAG_LIST; elt; elt = CTreeChain(elt))
  {
      int code = CTreeTypeCode( CTreeListValue(elt));
      /* An anonymous union parm type is meaningful as a GNU extension.
       So don't warn for that.  */
      if (code == C_UNION_TYPE && CTreeListPurpose(elt) == 0 && !c_pedantic) continue;
      if ( CTreeListPurpose(elt) != 0)
      {
        warning ("`%s %s' declared inside parameter list",
             (code == C_RECORD_TYPE ? "struct"
              : code == C_UNION_TYPE ? "union"
              : "enum"),
             CTreeIdentPointer( CTreeListPurpose(elt)));
      }
      else
      {
        warning ("anonymous %s declared inside parameter list",
             (code == C_RECORD_TYPE ? "struct"
              : code == C_UNION_TYPE ? "union"
              : "enum"));
      }

      if (! already)
      {
        warning ("its scope is only this definition or declaration,");
        warning ("which is probably not what you want.");
        already = 1;
      }
  }
}

/* Identify this binding level as a level of parameters.
   DEFINITION_FLAG is 1 for a definition, 0 for a declaration.
   But it turns out there is no way to pass the right value for
   DEFINITION_FLAG, so we ignore it.  */

void c_declare_parm_level( definition_flag )
     int definition_flag;
{
  c_current_binding_level->PARAM_FLAG = 1;
}


/* Nonzero if currently making parm declarations.  */

int c_in_parm_level_p()
{
  return c_current_binding_level->PARAM_FLAG;
}

/* Nonzero if the current level needs to have a BLOCK made.  */

int
c_kept_level_p ()
{
  return ((c_current_binding_level->KEEP_IF_SUBBLOCKS
           && c_current_binding_level->BLOCK_LIST != 0)
          || c_current_binding_level->KEEP
          || c_current_binding_level->NAME_LIST != 0
          || (c_current_binding_level->TAG_LIST != 0
              && !c_current_binding_level->TAG_TRANSPARENT));
}

/* Given NAME, an IDENTIFIER_NODE,
   return the structure (or union or enum) definition for that name.
   Searches binding levels from BINDING_LEVEL up to the global level.
   If THISLEVEL_ONLY is nonzero, searches only the specified context
   (but skips any tag-transparent contexts to find one that is
   meaningful for tags).
   CODE says which kind of type the caller wants;
   it is RECORD_TYPE or UNION_TYPE or ENUMERAL_TYPE.
   If the wrong kind of type is found, an error is reported.  */

static c_tree_node *c_lookup_tag(code, name, binding_level, thislevel_only)

     int code;
     c_binding_level *binding_level;
     c_tree_node *name;
     int thislevel_only;
{
  c_binding_level *level;
  c_tree_node     *tail;

  for (level = binding_level; level; level = level->LEVEL_CHAIN)
  {
    for (tail = level->TAG_LIST; tail; tail = CTreeChain(tail))
    {
      if ( CTreeListPurpose(tail) == name)
      {
        if ( CTreeTypeCode( CTreeListValue(tail)) != code)
        {
              /* Definition isn't the kind we were looking for.  */
          c_pending_invalid_xref = name;
          c_pending_invalid_xref_file = c_input_filename;
          c_pending_invalid_xref_line = c_lineno;
        }
        return  CTreeListValue(tail);
      }
    }

    if ( thislevel_only &&  ! level->TAG_TRANSPARENT)
    {
      return ( (c_tree_node *)0 );
    }
  }

  return ( (c_tree_node *)0 );
}

/* Push a definition or a declaration of struct, union or enum tag "name".
   "type" should be the type node.
   We assume that the tag "name" is not already defined.

   Note that the definition may really be just a forward reference.
   In that case, the TYPE_SIZE will be zero.  */

void c_push_tag(name, type)

  c_tree_node *name;
  c_tree_node *type;
{
  c_binding_level *b;

  /* Find the proper binding level for this type tag.  */

  for (b = c_current_binding_level; b->TAG_TRANSPARENT; b = b->LEVEL_CHAIN)
  {
    continue;
  }

  if (name)
  {
    /* Record the identifier as the type's name if it has none.  */

    if ( CTreeTypeName(type) == (c_tree_node *)0)
    {
      CTreeTypeName(type) = name;
    }
  }

  if (b == c_global_binding_level)
  {
    b->TAG_LIST = c_tree_add_head_list(name, type, b->TAG_LIST);
  }
  else
  {
    b->TAG_LIST = c_tree_add_head_list(name, type, b->TAG_LIST);
  }

  /* Create a fake NULL-named TYPE_DECL node whose TREE_TYPE will be the
     tagged type we just added to the current binding level.  This fake
     NULL-named TYPE_DECL node helps dwarfout.c to know when it needs
     to output a representation of a tagged type, and it also gives
     us a convenient place to record the "scope start" address for the
     tagged type.  */

  CTreeChain(type) = c_push_decl( c_tree_build_decl(C_TYPE_DECL, NULL, type));

  /* An approximation for now, so we can tell this is a function-scope tag.
     This will be updated in poplevel.  */
  CTreeTypeContext(type) = CTreeDeclContext( CTreeChain(type));
}

/* Make sure that the tag NAME is defined *in the current binding level*
   at least as a forward reference.
   CODE says which kind of tag NAME ought to be.

   We also do a push_obstacks_nochange
   whose matching pop is in finish_struct.  */

c_tree_node *c_start_struct(code_type, name)

     int code_type;
     c_tree_node *name;
{
  /* If there is already a tag defined at this binding level
     (as a forward reference), just return it.  */

  c_tree_node *ref = 0;

# ifdef LUDO_NO_SKIP
  push_obstacks_nochange ();

  if (c_current_binding_level == c_global_binding_level)
  {
    end_temporary_allocation ();
  }
# endif

  if (name != 0)
  {
    ref = c_lookup_tag(code_type, name, c_current_binding_level, 1);
  }

  if (ref && CTreeTypeCode(ref) == code_type)
  {
# ifdef LUDO_NO_SKIP
    C_TYPE_BEING_DEFINED(ref) = 1;
    TYPE_PACKED (ref) = c_flag_pack_struct;
# endif
    if ( CTreeTypeValues(ref) != (c_tree_node *)0 )
    {
	error((code_type == C_UNION_TYPE ? "redefinition of `union %s'"
		: "redefinition of `struct %s'"),
	       CTreeIdentPointer(name));
    }

    return ref;
  }

  /* Otherwise create a forward-reference just so the tag is in scope.  */

  ref = c_tree_build_type( code_type );

  c_push_tag(name, ref);

# ifdef LUDO_NO_SKIP
  C_TYPE_BEING_DEFINED (ref) = 1;
  TYPE_PACKED (ref) = flag_pack_struct;
# endif
  return ref;
}

/* Lay out the type T, and its element type, and so on.  */

static void c_layout_array_type(t)

     c_tree_node *t;
{
  if ( CTreeTypeCode( CTreeType(t)) == C_ARRAY_TYPE)
  {
    c_layout_array_type( CTreeType(t));
  }

  c_layout_type (t);
}

/* Fill in the fields of a RECORD_TYPE or UNION_TYPE node, T.
   FIELDLIST is a chain of FIELD_DECL nodes for the fields.
   ATTRIBUTES are attributes to be applied to the structure.

   We also do a pop_obstacks to match the push in start_struct.  */

c_tree_node *c_finish_struct(t, fieldlist, attributes)

     c_tree_node *t;
     c_tree_node *fieldlist;
     c_tree_node *attributes;
{
  c_tree_node *x;
# ifdef LUDO_NO_SKIP
  int old_momentary;
  int toplevel = c_global_binding_level == c_current_binding_level;
# endif

  /* If this type was previously laid out as a forward reference,
     make sure we lay it out again.  */

  CTreeTypeSize(t) = 0;

  c_decl_attributes(t, attributes, NULL);

  /* Nameless union parm types are useful as GCC extension.  */
  if (! (CTreeTypeCode(t) == C_UNION_TYPE && 
         CTreeTypeName(t) == 0) && !c_pedantic)
  {
    /* Otherwise, warn about any struct or union def. in parmlist.  */
    if (c_in_parm_level_p ())
      {
        if (c_pedantic)
        {
          pedwarn (( CTreeTypeCode(t) == C_UNION_TYPE ? "union defined inside parms"
                    : "structure defined inside parms"));
        }
        else if (! c_flag_traditional)
        {
          warning (( CTreeTypeCode(t) == C_UNION_TYPE ? "union defined inside parms"
                    : "structure defined inside parms"));
        }
      }
  }

# ifdef LUDO_NO_SKIP
  old_momentary = suspend_momentary ();
# endif

  if (c_pedantic)
  {
    for (x = fieldlist; x; x = CTreeChain(x))
    {
      if ( CTreeDeclName(x) != NULL ) break;
    }

    if (x == 0)
    {
      pedwarn ("%s has no %smembers",
               ( CTreeTypeCode(t) == C_UNION_TYPE ? "union" : "structure"),
               (fieldlist != NULL ? "named " : ""));
    }
  }

  /* Install struct as DECL_CONTEXT of each field decl.
     Also process specified field sizes.
     Set DECL_FIELD_SIZE to the specified size, or 0 if none specified.
     The specified size is found in the DECL_INITIAL.
     Store 0 there, except for ": 0" fields (so we can find them
     and delete them, below).  */

  for (x = fieldlist; x; x = CTreeChain(x))
  {
    CTreeDeclContext(x) = t;

# ifdef LUDO_NO_SKIP
    DECL_PACKED (x) |= TYPE_PACKED (t);
# endif
    CTreeDeclFieldSize(x) = 0;

    /* If any field is const, the structure type is pseudo-const.  */
    if ( CIsTreeNodeReadOnly(x))
    {
      CSetTreeTypeFieldsReadOnly(t);
    }
    else
    {
        /* A field that is pseudo-const makes the structure likewise.  */
        tree t1 = CTreeType(x);
        while (CTreeTypeCode(t1) == C_ARRAY_TYPE)
          t1 = CTreeType(t1);
        if ((CTreeTypeCode(t1) == C_RECORD_TYPE || 
             CTreeTypeCode(t1) == C_UNION_TYPE)
            && CIsTreeTypeFieldsReadOnly(t1))
          CSetTreeTypeFieldsReadOnly(t);
    }

    /* Any field that is volatile means variables of this type must be
       treated in some ways as volatile.  */
    if ( CIsTreeNodeVolatile(x))
      CSetTreeTypeFieldsVolatile(t);

    /* Any field of nominal variable size implies structure is too.  */
    if ( CIsTreeDeclVarSize (x))
      CSetTreeTypeVarSize(t);

    /* Detect invalid nested redefinition.  */
    if ( CTreeType(x) == t)
    {
      error ("nested redefinition of `%s'", CTreeIdentPointer( CTreeTypeName(t)));
    }

    /* Detect invalid bit-field size.  */
    if ( CTreeDeclInitial(x) )
    {
      CStripNops( CTreeDeclInitial(x));
    }

    if ( CTreeDeclInitial(x))
    {
      if ( CTreeNodeFirstCode( CTreeDeclInitial(x)) == C_INT_CST_NODE)
      {
        c_constant_expression_warning (CTreeDeclInitial(x));
      }
      else
      {
        error_with_decl (x, "bit-field `%s' width not an integer constant");
        CTreeDeclInitial(x) = NULL;
      }
    }

    /* Detect invalid bit-field type.  */
    if ( CTreeDeclInitial(x)
        && CTreeTypeCode( CTreeType(x)) != C_INTEGER_TYPE
        && CTreeTypeCode( CTreeType(x)) != C_ENUMERAL_TYPE)
    {
      error_with_decl (x, "bit-field `%s' has invalid type");
      CTreeDeclInitial(x) = NULL;
    }

    if ( CTreeDeclInitial(x) && c_pedantic
        && CTreeTypeMainVariant( CTreeType(x)) != c_integer_type_node
        && CTreeTypeMainVariant( CTreeType(x)) != c_unsigned_type_node
        /* Accept an enum that's equivalent to int or unsigned int.  */
        && !( CTreeTypeCode( CTreeType(x)) == C_ENUMERAL_TYPE
             && ( CTreeTypePrecision( CTreeType(x))
                 == CTreeTypePrecision(c_integer_type_node))))
    {
      pedwarn_with_decl (x, "bit-field `%s' type invalid in ANSI C");
    }

    /* Detect and ignore out of range field width.  */
    if ( CTreeDeclInitial(x))
    {
        unsigned int width = CTreeIntCstLow( CTreeDeclInitial(x));

        if (c_tree_is_int_cst_sgn( CTreeDeclInitial(x)) < 0)
          {
            CTreeDeclInitial(x) = NULL;
            error_with_decl (x, "negative width in bit-field `%s'");
          }
        else if ( CTreeIntCstHigh(CTreeDeclInitial(x)) != 0
                 || width > CTreeTypePrecision(CTreeType(x)))
          {
            CTreeDeclInitial(x) = NULL;
            pedwarn_with_decl (x, "width of `%s' exceeds its type");
          }
        else if (width == 0 && CTreeDeclName(x) != 0)
          {
            error_with_decl (x, "zero width for bit-field `%s'");
            CTreeDeclInitial(x) = NULL;
          }
    }

    /* Process valid field width.  */
    if ( CTreeDeclInitial(x) != NULL )
    {
      int width = CTreeIntCstLow(CTreeDeclInitial(x));

      if ( CTreeTypeCode(CTreeType(x)) == C_ENUMERAL_TYPE
          && (width < c_min_precision( CTreeTypeMinValue(CTreeType(x)),
                                      CIsTreeNodeUnsigned(CTreeType(x)))
              || width < c_min_precision( CTreeTypeMaxValue(CTreeType(x)),
                                         CIsTreeNodeUnsigned(CTreeType(x)))))
      {
        c_warning_with_decl(x, "`%s' is narrower than values of its type");
      }

      CTreeDeclFieldSize(x) = width;
      CSetTreeDeclBitField(x);
      CSetTreeDeclCBitField(x);
      CTreeDeclInitial(x) = NULL;

# ifdef LUDO_NO_SKIP
      if (width == 0)
      {
            /* field size 0 => force desired amount of alignment.  */
#ifdef EMPTY_FIELD_BOUNDARY
            DECL_ALIGN (x) = MAX (DECL_ALIGN (x), EMPTY_FIELD_BOUNDARY);
#endif
#ifdef PCC_BITFIELD_TYPE_MATTERS
            if (PCC_BITFIELD_TYPE_MATTERS)
              DECL_ALIGN (x) = MAX (DECL_ALIGN (x),
                                    TYPE_ALIGN (CTreeType(x)));
#endif
      }
# endif
    }
    else if ( CTreeType(x) != c_error_mark_node)
    {
# ifdef LUDO_NO_SKIP
        int min_align = (DECL_PACKED (x) ? BITS_PER_UNIT
                         : TYPE_ALIGN (CTreeType(x)));
        /* Non-bit-fields are aligned for their type, except packed
           fields which require only BITS_PER_UNIT alignment.  */
        DECL_ALIGN (x) = MAX (DECL_ALIGN (x), min_align);
# endif
    }
  }

  /* Now CTreeDeclInitialis null on all members.  */

  /* Delete all duplicate fields from the fieldlist */
  for (x = fieldlist; x && CTreeChain(x);)
  {
    /* Anonymous fields aren't duplicates.  */
    if (CTreeDeclName(CTreeChain(x)) == NULL)
    {
      x = CTreeChain(x);
    }
    else
    {
      c_tree_node *y = fieldlist;
          
      while (1)
      {
        if (CTreeDeclName(y) == CTreeDeclName(CTreeChain(x))) break;
        if (y == x) break;
        y = CTreeChain(y);
      }

      if (CTreeDeclName(y) == CTreeDeclName(CTreeChain(x)))
      {
        error_with_decl (CTreeChain(x), "duplicate member `%s'");
        CTreeChain(x) = CTreeChain(CTreeChain(x));
      }
      else
      {
        x = CTreeChain(x);
      }
    }
  }

  /* Now we have the nearly final fieldlist.  Record it,
     then lay out the structure or union (including the fields).  */

  CTreeTypeValues(t) = fieldlist;

  c_layout_type(t);

  /* Delete all zero-width bit-fields from the front of the fieldlist */
  while (fieldlist
         && CTreeDeclInitial(fieldlist))
  {
    fieldlist = CTreeChain(fieldlist);
  }
  /* Delete all such members from the rest of the fieldlist */
  for (x = fieldlist; x;)
  {
    if (CTreeChain(x) && CTreeDeclInitial(CTreeChain(x)))
    {
        CTreeChain(x) = CTreeChain(CTreeChain(x));
    }
    else
    {
      x = CTreeChain(x);
    }
  }

  /*  Now we have the truly final field list.
      Store it in this type and in the variants.  */

  CTreeTypeValues(t) = fieldlist;

  /* If there are lots of fields, sort so we can look through them fast.
     We arbitrarily consider 16 or more elts to be "a lot".  */
# ifdef LUDO_NO_SKIP
  {
    int len = 0;

    for (x = fieldlist; x; x = CTreeChain(x))
    {
      if (len > 15) break;
      len += 1;
    }

    if (len > 15)
    {
        tree *field_array;
        char *space;

        len += c_tree_get_list_length (x);
        /* Use the same allocation policy here that make_node uses, to
           ensure that this lives as long as the rest of the struct decl.
           All decls in an inline function need to be saved.  */
        if (allocation_temporary_p ())
          space = savealloc (sizeof (struct lang_type) + len * sizeof (tree));
        else
          space = oballoc (sizeof (struct lang_type) + len * sizeof (tree));

        TYPE_LANG_SPECIFIC (t) = (struct lang_type *) space;
        TYPE_LANG_SPECIFIC (t)->len = len;

        field_array = &TYPE_LANG_SPECIFIC (t)->elts[0];
        len = 0;
        for (x = fieldlist; x; x = CTreeChain(x))
          field_array[len++] = x;

        qsort (field_array, len, sizeof (tree), field_decl_cmp);
    }
  }
# endif

  for (x = CTreeTypeMainVariant(t); x; x = CTreeTypeNextVariant(x))
  {
    CTreeTypeValues(x) = CTreeTypeValues(t);
# ifdef LUDO_NO_SKIP
      TYPE_LANG_SPECIFIC (x) = TYPE_LANG_SPECIFIC (t);
      TYPE_ALIGN (x) = TYPE_ALIGN (t);
# endif
  }

# ifdef LUDO_NO_SKIP
  /* If this was supposed to be a transparent union, but we can't
     make it one, warn and turn off the flag.  */
  if (  CTreeTypeCode(t) == C_UNION_TYPE
      && TYPE_TRANSPARENT_UNION (t)
      && CTreeTypeMode(t) != CTreeDeclMode( CTreeTypeValues(t)))
  {
    TYPE_TRANSPARENT_UNION (t) = 0;
    warning ("union cannot be made transparent");
  }
# endif

  /* If this structure or union completes the type of any previous
     variable declaration, lay it out and output its rtl.  */

  if (c_current_binding_level->N_INCOMPLETE != 0)
  {
    c_tree_node *decl;

    for (decl = c_current_binding_level->NAME_LIST; decl; decl = CTreeChain(decl))
    {
      if ( ( CTreeType(decl) == t                 ) && 
           ( CTreeDeclCode(decl) != C_TYPE_DECL ) )
      {
        c_layout_decl(decl, 0);
          /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
# ifdef LUDO_NO_SKIP
          maybe_objc_check_decl (decl);
          rest_of_decl_compilation (decl, NULL_PTR, toplevel, 0);
          if (! toplevel)
          {
            expand_decl (decl);
          }
# endif
          --c_current_binding_level->N_INCOMPLETE;
      }
      else if (CTreeTypeSize(CTreeType(decl)) == 0
               && CTreeTypeCode(CTreeType(decl)) == C_ARRAY_TYPE)
      {
          c_tree_node *element = CTreeType(decl);

          while ( CTreeTypeCode(element) == C_ARRAY_TYPE)
          {
            element = CTreeType(element);
          }

          if (element == t)
          {
            c_layout_array_type(CTreeType(decl));
          }
      }
    }
  }

# ifdef LUDO_NO_SKIP
  resume_momentary (old_momentary);

  /* Finish debugging output for this type.  */
  rest_of_type_compilation (t, toplevel);

  /* The matching push is in start_struct.  */
  pop_obstacks ();
# endif

  return t;
}

/* Get the struct, enum or union (CODE says which) with tag NAME.
   Define the tag as a forward-reference if it is not defined.  */

c_tree_node *c_xref_tag(code, name)

     int code;
     c_tree_node *name;
{
# ifdef LUDO_NO_SKIP
  int temporary = allocation_temporary_p ();
# endif

  /* If a cross reference is requested, look up the type
     already defined for this tag and return it.  */

  c_tree_node *ref = c_lookup_tag(code, name, c_current_binding_level, 0);
  /* Even if this is the wrong type of tag, return what we found.
     There will be an error message anyway, from pending_xref_error.
     If we create an empty xref just for an invalid use of the type,
     the main result is to create lots of superfluous error messages.  */
  if (ref)
  {
    return ref;
  }

# ifdef LUDO_NO_SKIP
  push_obstacks_nochange ();

  if (c_current_binding_level == c_global_binding_level && temporary)
  {
    end_temporary_allocation ();
  }
# endif

  /* If no such tag is yet defined, create a forward-reference node
     and record it as the "definition".
     When a real declaration of this type is found,
     the forward-reference will be altered into a real type.  */

  ref = c_tree_build_type(code);

  if (code == C_ENUMERAL_TYPE)
  {
      /* (In ANSI, Enums can be referred to only if already defined.)  */
      if (c_pedantic)
      {
        pedwarn ("ANSI C forbids forward references to `enum' types");
      }
      /* Give the type a default layout like unsigned int
         to avoid crashing if it does not get defined.  */
      CTreeTypeMode(ref) = CTreeTypeMode(c_unsigned_type_node);
# ifdef LUDO_NO_SKIP
      TYPE_ALIGN (ref) = TYPE_ALIGN (c_unsigned_type_node);
# endif
      CSetTreeNodeUnsigned( ref );
      CTreeTypePrecision(ref) = CTreeTypePrecision( c_unsigned_type_node);
      CTreeTypeMinValue(ref)  = CTreeTypeMinValue( c_unsigned_type_node);
      CTreeTypeMaxValue(ref)  = CTreeTypeMaxValue( c_unsigned_type_node);
  }

  c_push_tag(name, ref);

# ifdef LUDO_NO_SKIP
  pop_obstacks ();
# endif

  return ref;
}


/* Given a type, find the tag that was defined for it and return the tag name.
   Otherwise return 0.  */

static c_tree_node *c_lookup_tag_reverse(type)

     c_tree_node *type;
{
  c_binding_level *level;

  for (level = c_current_binding_level; level; level = level->LEVEL_CHAIN)
  {
    c_tree_node *tail;

    for (tail = level->TAG_LIST; tail; tail = CTreeChain(tail))
    {
      if ( CTreeListValue(tail) == type)
      {
        return  CTreeListPurpose(tail);
      }
    }
  }

  return NULL;
}


/* Called when a declaration is seen that contains no names to declare.
   If its type is a reference to a structure, union or enum inherited
   from a containing scope, shadow that tag name for the current scope
   with a forward reference.
   If its type defines a new named structure or union
   or defines an enum, it is valid but we need not do anything here.
   Otherwise, it is an error.  */



void c_shadow_tag_warned(declspecs, warned)

     c_tree_node *declspecs;
     int warned;
     /* 1 => we have done a pedwarn.  2 => we have done a warning, but
        no pedwarn.  */
{
  int found_tag = 0;
  c_tree_node *link;

  c_pending_invalid_xref = NULL;

  for (link = declspecs; link; link = CTreeChain(link))
  {
    c_tree_node *value = CTreeListValue(link);
    int code = CTreeTypeCode(value);

    if (code == C_RECORD_TYPE || code == C_UNION_TYPE || code == C_ENUMERAL_TYPE)
        /* Used to test also that CTreeTypeSize(value) != 0.
           That caused warning for `struct foo;' at top level in the file.  */
    {
       c_tree_node *name = c_lookup_tag_reverse(value);
       c_tree_node *t;

       found_tag++;

       if (name == 0)
       {
         if (warned != 1 && code != C_ENUMERAL_TYPE)
                /* Empty unnamed enum OK */
         {
           pedwarn ("unnamed struct/union that defines no instances");
           warned = 1;
         }
       }
       else
       {
         t = c_lookup_tag(code, name, c_current_binding_level, 1);

         if (t == 0)
         {
           t = c_tree_build_type(code);
           c_push_tag(name, t);
         }
       }
    }
    else
    {
      if (!warned && ! c_in_system_header)
      {
        warning ("useless keyword or type name in empty declaration");
        warned = 2;
      }
    }
  }

  if (found_tag > 1)
  {
    error ("two types specified in one empty declaration");
  }

  if (warned != 1)
    {
      if (found_tag == 0)
      {
        pedwarn ("empty declaration");
      }
    }
}

void c_shadow_tag(declspecs)

     c_tree_node *declspecs;
{
  c_shadow_tag_warned(declspecs, 0);
}

/* Make a label named NAME in the current function,
   shadowing silently any that may be inherited from containing functions
   or containing scopes.

   Note that valid use, if the label being shadowed
   comes from another scope in the same function,
   requires calling declare_nonlocal_label right away.  */

c_tree_node *c_shadow_label(name)

     c_tree_node *name;
{
  c_tree_node *decl = CTreeIdentLabelValue(name);

  if (decl != 0)
  {
    c_tree_node *dup;

    /* Check to make sure that the label hasn't already been declared
       at this label scope */
    for (dup = c_named_labels; dup; dup = CTreeChain(dup))
    {
      if ( CTreeListValue(dup) == decl)
      {
          error ("duplicate label declaration `%s'", CTreeIdentPointer(name));
          error_with_decl (CTreeListValue(dup), "this is a previous declaration");
          /* Just use the previous declaration.  */
          return c_lookup_label(name);
        }
    }

    c_shadowed_labels = c_tree_add_head_list( NULL, decl, c_shadowed_labels);
    CTreeIdentLabelValue(name) = decl = NULL;
  }

  return c_lookup_label(name);
}


/* Get the LABEL_DECL corresponding to identifier ID as a label.
   Create one if none exists so far for the current function.
   This function is called for both label definitions and label references.  */

c_tree_node *c_lookup_label(id)

     c_tree_node *id;
{
  c_tree_node *decl = CTreeIdentLabelValue(id);

  if (c_current_function_decl == 0)
  {
    error ("label %s referenced outside of any function", CTreeIdentPointer(id));
    return 0;
  }

  /* Use a label already defined or ref'd with this name.  */
  if (decl != 0)
  {
    /* But not if it is inherited and wasn't declared to be inheritable.  */
    if ( CTreeDeclContext(decl) != c_current_function_decl
          && ! CIsTreeNodeCDeclLabel(decl))
    {
      return c_shadow_label(id);
    }

    return decl;
  }

  decl = c_tree_build_decl( C_LABEL_DECL, id, c_void_type_node);

  /* Make sure every label has an rtx.  */
# ifdef LUDO_NO_SKIP
  label_rtx (decl);
# endif

  /* A label not explicitly declared must be local to where it's ref'd.  */
  CTreeDeclContext(decl) = c_current_function_decl;

# ifdef LUDO_NO_SKIP
  CTreeDeclMode(decl) = VOIDmode;
# endif

  /* Say where one reference is to the label,
     for the sake of the error if it is not defined.  */
  CTreeDeclLineNum(decl) = c_lineno;
  CTreeDeclFileName(decl) = c_input_filename;

  CTreeIdentLabelValue(id) = decl;

  c_named_labels = c_tree_add_head_list( NULL, decl, c_named_labels);

  return decl;
}

/* Define a label, specifying the location in the source file.
   Return the LABEL_DECL node for the label, if the definition is valid.
   Otherwise return 0.  */

c_tree_node *c_define_label(filename, line, name)

     char *filename;
     int line;
     c_tree_node *name;
{
  c_tree_node *decl = c_lookup_label(name);

  /* If label with this name is known from an outer context, shadow it.  */
  if (decl != 0 && CTreeDeclContext(decl) != c_current_function_decl)
  {
    c_shadowed_labels = c_tree_add_head_list( NULL, decl, c_shadowed_labels);
    CTreeIdentLabelValue(name) = 0;
    decl = c_lookup_label(name);
  }

  if (CTreeDeclInitial(decl) != 0)
  {
    error ("duplicate label `%s'", CTreeIdentPointer(name));
    return 0;
  }
  else
  {
    /* Mark label as having been defined.  */
    CTreeDeclInitial(decl) = c_error_mark_node;
    /* Say where in the source.  */
    CTreeDeclFileName(decl) = filename;
    CTreeDeclLineNum(decl) = line;

    return decl;
  }
}

/* Begin compiling the definition of an enumeration type.
   NAME is its name (or null if anonymous).
   Returns the type object, as yet incomplete.
   Also records info about it so that build_enumerator
   may be used to declare the individual values as they are read.  */

c_tree_node *c_start_enum(name)

     c_tree_node *name;
{
  c_tree_node *enumtype = 0;

  /* If this is the real definition for a previous forward reference,
     fill in the contents in the same object that used to be the
     forward reference.  */

  if (name != 0)
  {
    enumtype = c_lookup_tag( C_ENUMERAL_TYPE, name, c_current_binding_level, 1);
  }

# ifdef LUDO_NO_SKIP
  /* The corresponding pop_obstacks is in finish_enum.  */
  push_obstacks_nochange ();
  /* If these symbols and types are global, make them permanent.  */
  if (c_current_binding_level == c_global_binding_level)
  {
    end_temporary_allocation ();
  }
# endif

  if (enumtype == 0 || CTreeTypeCode(enumtype) != C_ENUMERAL_TYPE)
  {
      enumtype = c_tree_build_type(C_ENUMERAL_TYPE);
      c_push_tag(name, enumtype);
  }

# ifdef LUDO_NO_SKIP
  C_TYPE_BEING_DEFINED (enumtype) = 1;
# endif

  if ( CTreeTypeValues(enumtype) != 0)
  {
      /* This enum is a named one that has been declared already.  */
      error("redeclaration of `enum %s'", CTreeIdentPointer(name));

      /* Completely replace its old definition.
         The old enumerators remain defined, however.  */
      CTreeTypeValues(enumtype) = 0;
  }

  c_enum_next_value = c_integer_zero_node;
  c_enum_overflow = 0;

# ifdef LUDO_NO_SKIP
  if (c_flag_short_enums)
  {
    TYPE_PACKED (enumtype) = 1;
  }
# endif

  return enumtype;
}

/* After processing and defining all the values of an enumeration type,
   install their decls in the enumeration type and finish it off.
   ENUMTYPE is the type object, VALUES a list of decl-value pairs,
   and ATTRIBUTES are the specified attributes.
   Returns ENUMTYPE.  */

c_tree_node *c_finish_enum(enumtype, values, attributes)
     c_tree_node *enumtype;
     c_tree_node *values;
     c_tree_node *attributes;
{
  c_tree_node *pair;
  c_tree_node *tem;
  c_tree_node *minnode = 0;
  c_tree_node *maxnode = 0;
  int lowprec, highprec, precision;
# ifdef LUDO_NO_SKIP
  int toplevel = c_global_binding_level == c_current_binding_level;
# endif

  if (c_in_parm_level_p ())
  {
    warning ("enum defined inside parms");
  }

  c_decl_attributes(enumtype, attributes, NULL);

  /* Calculate the maximum value of any enumerator in this type.  */

  if (values == c_error_mark_node)
  {
    minnode = maxnode = c_integer_zero_node;
  }
  else
  {
    for (pair = values; pair; pair = CTreeChain(pair))
    {
      c_tree_node *value = CTreeListValue(pair);

      if (pair == values)
      {
        minnode = maxnode = CTreeListValue(pair);
      }
      else
      {
        if ( c_tree_is_int_cst_lt(maxnode, value))
        {
          maxnode = value;
        }
        if ( c_tree_is_int_cst_lt(value, minnode))
        {
          minnode = value;
        }
      }
    }
  }

  CTreeTypeMinValue(enumtype) = minnode;
  CTreeTypeMaxValue(enumtype) = maxnode;

  /* An enum can have some negative values; then it is signed.  */
  if ( c_tree_is_int_cst_sgn(minnode) >= 0 )
  {
    CSetTreeNodeUnsigned(enumtype);
  }

  /* Determine the precision this type needs.  */

  lowprec = c_min_precision(minnode, CIsTreeNodeUnsigned(enumtype));
  highprec = c_min_precision(maxnode, CIsTreeNodeUnsigned(enumtype));
  precision = MAX (lowprec, highprec);

  if (
# ifdef LUDO_NO_SKIP
      TYPE_PACKED (enumtype) || 
# endif
       precision > CTreeTypePrecision(c_integer_type_node))
  {
      c_tree_node *narrowest = c_get_type_for_size(precision, 1);
      if (narrowest == 0)
        {
          warning ("enumeration values exceed range of largest integer");
          narrowest = c_long_long_integer_type_node;
        }

      CTreeTypePrecision(enumtype) = CTreeTypePrecision(narrowest);
  }
  else
  {
    CTreeTypePrecision(enumtype) = CTreeTypePrecision(c_integer_type_node);
  }

  CTreeTypeSize(enumtype) = 0;
  c_layout_type(enumtype);

  if (values != c_error_mark_node)
  {
      /* Change the type of the enumerators to be the enum type.
         Formerly this was done only for enums that fit in an int,
         but the comment said it was done only for enums wider than int.
         It seems necessary to do this for wide enums,
         and best not to change what's done for ordinary narrower ones.  */
    for (pair = values; pair; pair = CTreeChain(pair))
    {
      CTreeType( CTreeListPurpose(pair)) = enumtype;
      CTreeDeclSize( CTreeListPurpose(pair)) = CTreeTypeSize(enumtype);

# ifdef LUDO_NO_SKIP
      if ( CTreeDeclCode( CTreeListPurpose(pair)) != C_FUNCTION_DECL)
      {
        DECL_ALIGN (TREE_PURPOSE (pair)) = TYPE_ALIGN (enumtype);
      }
# endif
    }

    /* Replace the decl nodes in VALUES with their names.  */
    for (pair = values; pair; pair = CTreeChain(pair))
    {
      CTreeListPurpose(pair) = CTreeDeclName( CTreeListPurpose(pair));
    }

    CTreeTypeValues(enumtype) = values;
  }

  /* Fix up all variant types of this enum type.  */
  for (tem = CTreeTypeMainVariant(enumtype); tem; tem = CTreeTypeNextVariant(tem))
  {
    CTreeTypeValues(tem)    = CTreeTypeValues(enumtype);
    CTreeTypeMinValue(tem)  = CTreeTypeMinValue(enumtype);
    CTreeTypeMaxValue(tem)  = CTreeTypeMaxValue(enumtype);
    CTreeTypeSize(tem)      = CTreeTypeSize(enumtype);
    CTreeTypeMode(tem)      = CTreeTypeMode(enumtype);
    CTreeTypePrecision(tem) = CTreeTypePrecision(enumtype);
    CTreeTypeAlign(tem)     = CTreeTypeAlign(enumtype);

    if ( CIsTreeNodeUnsigned( enumtype ) )
    {
      CSetTreeNodeUnsigned( tem );
    }
  }

# ifdef LUDO_NO_SKIP
  /* Finish debugging output for this type.  */
  rest_of_type_compilation (enumtype, toplevel);

  /* This matches a push in start_enum.  */
  pop_obstacks ();
# endif

  return enumtype;
}

/* Build and install a CONST_DECL for one value of the
   current enumeration type (one that was begun with start_enum).
   Return a tree-list containing the CONST_DECL and its value.
   Assignment of sequential values by default is handled here.  */

c_tree_node *c_build_enumerator(name, value)
     c_tree_node *name;
     c_tree_node *value;
{
  c_tree_node *decl;
  c_tree_node *type;

  /* Validate and default VALUE.  */

  /* Remove no-op casts from the value.  */
  if (value)
  {
    CStripNops(value);
  }

  if (value != 0)
  {
    if ( CTreeNodeFirstCode(value) == C_INT_CST_NODE)
    {
      value = c_default_conversion(value);
      c_constant_expression_warning(value);
    }
    else
    {
      error ("enumerator value for `%s' not integer constant", CTreeIdentPointer(name));
      value = 0;
    }
  }

  /* Default based on previous value.  */
  /* It should no longer be possible to have NON_LVALUE_EXPR
     in the default.  */
  if (value == 0)
  {
    value = c_enum_next_value;

    if (c_enum_overflow)
    {
      error ("overflow in enumeration values");
    }
  }

  if (c_pedantic && ! c_int_fits_type_p(value, c_integer_type_node))
  {
    pedwarn ("ANSI C restricts enumerator values to range of `int'");
    value = c_integer_zero_node;
  }

  /* Set basis for default for next value.  */
  c_enum_next_value = c_build_binary_op(C_PLUS_EXPR, value, c_integer_one_node, 0);
  c_enum_overflow   = c_tree_is_int_cst_lt(c_enum_next_value, value);

  /* Now create a declaration for the enum value name.  */


  type = CTreeType(value);
  type = c_get_type_for_size(
            MAX( CTreeTypePrecision(type), CTreeTypePrecision(c_integer_type_node)),
            ((c_flag_traditional || 
              CTreeTypePrecision(type) >= CTreeTypePrecision(c_integer_type_node))
                         && CIsTreeNodeUnsigned(type)));

  decl = c_tree_build_decl( C_CONST_DECL, name, type);
  CTreeDeclInitial(decl) = value;
  CTreeType(value) = type;
  c_push_decl(decl);

  return c_tree_add_head_list(decl, value, NULL);
}


/* Create the FUNCTION_DECL for a function definition.
   DECLSPECS, DECLARATOR, PREFIX_ATTRIBUTES and ATTRIBUTES are the parts of
   the declaration; they describe the function's name and the type it returns,
   but twisted together in a fashion that parallels the syntax of C.

   This function creates a binding context for the function body
   as well as setting up the FUNCTION_DECL in current_function_decl.

   Returns 1 on success.  If the DECLARATOR is not suitable for a function
   (it defines a datum instead), we return 0, which tells
   yyparse to report a parse error.

   NESTED is nonzero for a function nested within another function.  */

int c_start_function( declspecs, declarator, prefix_attributes, attributes, nested )
     c_tree_node *declarator;
     c_tree_node *declspecs;
     c_tree_node *prefix_attributes;
     c_tree_node *attributes;
     int nested;
{
  c_tree_node *decl1;
  c_tree_node *old_decl;
  c_tree_node *restype;
# ifdef LUDO_NO_SKIP
  int old_immediate_size_expand = immediate_size_expand;
# endif

  c_current_function_returns_value = 0;  /* Assume, until we see it does.  */
  c_current_function_returns_null = 0;
  c_warn_about_return_type = 0;
  c_current_extern_inline = 0;
  c_c_function_varargs = 0;
  c_named_labels = 0;
  c_shadowed_labels = 0;

# ifdef LUDO_NO_SKIP
  /* Don't expand any sizes in the return type of the function.  */
  immediate_size_expand = 0;
# endif

  decl1 = c_grokdeclarator(declarator, declspecs, C_CDECL_FUNCDEF, 1);

  /* If the declarator is not suitable for a function definition,
     cause a syntax error.  */
  if (decl1 == 0)
  {
# ifdef LUDO_NO_SKIP
    immediate_size_expand = old_immediate_size_expand;
# endif
    return 0;
  }

  c_decl_attributes(decl1, prefix_attributes, attributes);

# ifdef LUDO_NO_SKIP
  announce_function(decl1);
# endif

  if (CTreeTypeSize(CTreeType(CTreeType(decl1))) == 0)
  {
    error ("return-type is an incomplete type");
      /* Make it return void instead.  */
    CTreeType(decl1)
        = c_tree_build_function_type( c_void_type_node,
                               CTreeTypeValues(CTreeType(decl1)));
  }

  if (c_warn_about_return_type)
  {
    warning("return-type defaults to `int'");
  }

  /* Save the parm names or decls from this function's declarator
     where store_parm_decls will find them.  */
  c_current_function_parms = c_last_function_parms;
  c_current_function_parm_tags = c_last_function_parm_tags;

  /* Make the init_value nonzero so pushdecl knows this is not tentative.
     error_mark_node is replaced below (in poplevel) with the BLOCK.  */
  CTreeDeclInitial(decl1) = c_error_mark_node;

  /* If this definition isn't a prototype and we had a prototype declaration
     before, copy the arg type info from that prototype.
     But not if what we had before was a builtin function.  */
  old_decl = c_lookup_name_current_level(CTreeDeclName(decl1));
  if (old_decl != 0 &&  CTreeTypeCode(CTreeType(old_decl)) == C_FUNCTION_TYPE
      && ! CIsTreeDeclBuiltIn(old_decl)
      && (CTreeTypeMainVariant(CTreeType(CTreeType(decl1)))
          == CTreeTypeMainVariant(CTreeType(CTreeType(old_decl))))
      && CTreeTypeValues(CTreeType(decl1)) == 0)
  {
    CTreeType(decl1) = CTreeType(old_decl);
    c_current_function_prototype_file = CTreeDeclFileName(old_decl);
    c_current_function_prototype_line = CTreeDeclLineNum(old_decl);
  }

  /* If there is no explicit declaration, look for any out-of-scope implicit
     declarations.  */
  if (old_decl == 0)
  {
    old_decl = CTreeIdentImplicitDecl(CTreeDeclName(decl1));
  }

  /* Optionally warn of old-fashioned def with no previous prototype.  */
  if (c_warn_strict_prototypes
      && CTreeTypeValues(CTreeType(decl1)) == 0
      && !(old_decl != 0 && CTreeTypeValues(CTreeType(old_decl)) != 0))
  {
    warning ("function declaration isn't a prototype");
  }
  /* Optionally warn of any global def with no previous prototype.  */
  else if (c_warn_missing_prototypes
           && CIsTreeNodePublic(decl1)
           && !(old_decl != 0 && CTreeTypeValues(CTreeType(old_decl)) != 0)
           && strcmp("main", CTreeIdentPointer(CTreeDeclName(decl1))))
  {
    warning_with_decl(decl1, "no previous prototype for `%s'");
  }
  /* Optionally warn of any def with no previous prototype
     if the function has already been used.  */
  else if (c_warn_missing_prototypes
           && old_decl != 0 
           && CIsTreeNodeUsed(old_decl)
           && CTreeTypeValues(CTreeType(old_decl)) == 0)
  {
    warning_with_decl(decl1, "`%s' was used with no prototype before its definition");
  }
  /* Optionally warn of any global def with no previous declaration.  */
  else if (c_warn_missing_declarations
           && CIsTreeNodePublic(decl1)
           && old_decl == 0
           && strcmp ("main", CTreeIdentPointer(CTreeDeclName(decl1))))
  {
    warning_with_decl(decl1, "no previous declaration for `%s'");
  }
  /* Optionally warn of any def with no previous declaration
     if the function has already been used.  */
  else if (c_warn_missing_declarations
           && old_decl != 0 
           && CIsTreeNodeUsed(old_decl)
           && old_decl == CTreeIdentImplicitDecl(CTreeDeclName(decl1))
           )
  {
    warning_with_decl(decl1, "`%s' was used with no declaration before its definition");
  }

  /* This is a definition, not a reference.
     So normally clear DECL_EXTERNAL.
     However, `extern inline' acts like a declaration
     except for defining how to inline.  So set DECL_EXTERNAL in that case.  */
  if ( c_current_extern_inline )
  {
    CSetTreeDeclExternal(decl1);
  }

  /* This function exists in static storage.
     (This does not mean `static' in the C sense!)  */
  CSetTreeNodeStatic(decl1);

  /* A nested function is not global.  */
  if (c_current_function_decl != 0)
  {
    CClearTreeNodePublic(decl1);
  }

  /* Warn for unlikely, improbable, or stupid declarations of `main'. */
  if (c_warn_main
      && strcmp ("main", CTreeIdentPointer(CTreeDeclName(decl1))) == 0)
  {
    c_tree_node *args;
    int argct = 0;

    if (CTreeTypeMainVariant(CTreeType(CTreeType(decl1)))
           != c_integer_type_node)
    {
      pedwarn_with_decl(decl1, "return type of `%s' is not `int'");
    }

    for ( args = CTreeTypeValues(CTreeType(decl1)); args;
          args = CTreeChain(args))
    {
      c_tree_node *type = args ? CTreeListValue(args) : 0;

      if (type == c_void_type_node) break;

      ++argct;
      switch (argct)
      {
        case 1:
          if (CTreeTypeMainVariant(type) != c_integer_type_node)
          {
            pedwarn_with_decl(decl1, "first argument of `%s' should be `int'");
          }
          break;

        case 2:
          if ( CTreeTypeCode(type) != C_POINTER_TYPE
              || CTreeTypeCode(CTreeType(type)) != C_POINTER_TYPE
              || (CTreeTypeMainVariant(CTreeType(CTreeType(type)))
                  != c_char_type_node))
          {
            pedwarn_with_decl(decl1, "second argument of `%s' should be `char **'");
          }
          break;

        case 3:
          if ( CTreeTypeCode(type) != C_POINTER_TYPE
              || CTreeTypeCode(CTreeType(type)) != C_POINTER_TYPE
              || (CTreeTypeMainVariant(CTreeType(CTreeType(type)))
                  != c_char_type_node))
          {
            pedwarn_with_decl(decl1, "third argument of `%s' should probably be `char **'");
          }
          break;
      }
    }

    /* It is intentional that this message does not mention the third
       argument, which is warned for only pedantically, because it's
       blessed by mention in an appendix of the standard. */

    if (argct > 0 && (argct < 2 || argct > 3))
    {
      pedwarn_with_decl(decl1, "`%s' takes only zero or two arguments");
    }

    if (argct == 3 && c_pedantic)
    {
      pedwarn_with_decl(decl1, "third argument of `%s' is deprecated");
    }

    if (! CIsTreeNodePublic(decl1))
    {
      pedwarn_with_decl (decl1, "`%s' is normally a non-static function");
    }
  }

  /* Record the decl so that the function name is defined.
     If we already have a decl for this name, and it is a FUNCTION_DECL,
     use the old decl.  */

  c_current_function_decl = c_push_decl(decl1);

  c_push_level(0);
  c_declare_parm_level(1);
  c_current_binding_level->SUBBLOCKS_TAG_TRANSPARENT = 1;

# ifdef LUDO_NO_SKIP
  make_function_rtl(current_function_decl);
# endif

  restype = CTreeType(CTreeType(c_current_function_decl));
  /* Promote the value to int before returning it.  */

  if (C_PROMOTING_INTEGER_TYPE_P(restype))
  {
      /* It retains unsignedness if traditional
         or if not really getting wider.  */
      if ( CIsTreeNodeUnsigned(restype)
          && (c_flag_traditional
              || (CTreeTypePrecision(restype)
                  == CTreeTypePrecision(c_integer_type_node))))
        restype = c_unsigned_type_node;
      else
        restype = c_integer_type_node;
  }

  CTreeDeclResult(c_current_function_decl) = 
    c_tree_build_decl( C_RESULT_DECL, NULL, restype);

# ifdef LUDO_NO_SKIP
  if (!nested)
  {
    /* Allocate further c_tree_node *nodes temporarily during compilation
       of this function only.  */
    temporary_allocation ();
  }

  /* If this fcn was already referenced via a block-scope `extern' decl
     (or an implicit decl), propagate certain information about the usage.  */
  if (TREE_ADDRESSABLE (DECL_ASSEMBLER_NAME (current_function_decl)))
    TREE_ADDRESSABLE (current_function_decl) = 1;

  immediate_size_expand = old_immediate_size_expand;
# endif

  return 1;
}

/* Finish up a function declaration and compile that function
   all the way to assembler language output.  The free the storage
   for the function definition.

   This is called after parsing the body of the function definition.

   NESTED is nonzero if the function being finished is nested in another.  */

c_tree_node *c_finish_function(nested, body)
     int nested;
     c_tree_node *body;
{
  c_tree_node *fndecl = c_current_function_decl;

/*  TREE_READONLY (fndecl) = 1;
    This caused &foo to be of type ptr-to-const-function
    which then got a warning when stored in a ptr-to-function variable.  */

  c_pop_level(1, 0, 1);

  if ( CTreeBlockBody( CTreeDeclInitial( fndecl ) ) == (c_tree_node *)0 )
  {
    CTreeBlockBody( CTreeDeclInitial( fndecl ) ) = body;
  }

  CTreeBlockSuperContext(CTreeDeclInitial(fndecl)) = fndecl;

  /* Must mark the RESULT_DECL as being in this function.  */

  CTreeDeclContext( CTreeDeclResult(fndecl)) = fndecl;

  /* Obey `register' declarations if `setjmp' is called in this fn.  */
# ifdef LUDO_NO_SKIP
  if (c_flag_traditional && c_current_function_calls_setjmp)
  {
    setjmp_protect (CTreeDeclInitial(fndecl));
    setjmp_protect_args ();
  }
# endif

  if (! strcmp( CTreeIdentPointer(CTreeDeclName(fndecl)), "main"))
  {
    if (CTreeTypeMainVariant(CTreeType(CTreeType(fndecl))) != c_integer_type_node)
    {
          /* You would expect the sense of this test to be the other way
             around, but if warn_main is set, we will already have warned,
             so this would be a duplicate.  This is the warning you get
             in some environments even if you *don't* ask for it, because
             these are environments where it may be more of a problem than
             usual.  */
      if (! c_warn_main)
      {
        pedwarn_with_decl( fndecl, "return type of `%s' is not `int'");
      }
    }
    else
    {
#ifdef DEFAULT_MAIN_RETURN
          /* Make it so that `main' always returns success by default.  */
          DEFAULT_MAIN_RETURN;
#endif
    }
  }

# ifdef LUDO_NO_SKIP
  /* Generate rtl for function exit.  */
  expand_function_end (input_filename, lineno, 0);
# endif

  /* So we can tell if jump_optimize sets it to 1.  */
  c_can_reach_end = 0;

# ifdef LUDO_NO_SKIP
  /* Run the optimizers and output the assembler code for this function.  */
  rest_of_compilation (fndecl);
# endif

  c_current_function_returns_null |= c_can_reach_end;

  if ( CIsTreeNodeVolatile(fndecl) && c_current_function_returns_null)
  {
    warning ("`noreturn' function does return");
  }
  else if (c_warn_return_type && c_can_reach_end
           && CTreeTypeMainVariant(CTreeType(CTreeType(fndecl))) != c_void_type_node)
    /* If this function returns non-void and control can drop through,
       complain.  */
  {
    warning ("control reaches end of non-void function");
  }
  /* With just -W, complain only if function returns both with
     and without a value.  */
  else if (c_extra_warnings
           && c_current_function_returns_value && c_current_function_returns_null)
  {
    warning ("this function may return with or without a value");
  }

  /* If requested, warn about function definitions where the function will
     return a value (usually of some struct or union type) which itself will
     take up a lot of stack space.  */

  if (c_warn_larger_than && ! CIsTreeDeclExternal(fndecl) && CTreeType(fndecl))
  {
    c_tree_node *ret_type = CTreeType(CTreeType(fndecl));

    if (ret_type)
    {
      c_tree_node *ret_type_size = CTreeTypeSize(ret_type);

      if (CTreeNodeFirstCode(ret_type_size) == C_INT_CST_NODE)
      {
        unsigned units = CTreeIntCstLow(ret_type_size) / C_BITS_PER_UNIT;

        if (units > c_larger_than_size)
        {
          warning_with_decl(fndecl, "size of return value of `%s' is %u bytes", units);
        }
      }
    }
  }

  /* Free all the c_tree_node *nodes making up this function.  */
  /* Switch back to allocating nodes permanently
     until we start another function.  */
# ifdef LUDO_NO_SKIP
  if (! nested)
  {
    permanent_allocation (1);
  }

  if (DECL_SAVED_INSNS(fndecl) == 0 && ! nested)
  {
      /* Stop pointing to the local nodes about to be freed.  */
      /* But CTreeDeclInitialmust remain nonzero so we know this
         was an actual function definition.  */
      /* For a nested function, this is done in pop_c_function_context.  */
      /* If rest_of_compilation set this to 0, leave it 0.  */
      if (CTreeDeclInitial(fndecl) != 0)
        CTreeDeclInitial(fndecl) = error_mark_node;
      DECL_ARGUMENTS (fndecl) = 0;
  }
# endif

# ifdef LUDO_NO_SKIP
  if (DECL_STATIC_CONSTRUCTOR (fndecl))
    {
#ifndef ASM_OUTPUT_CONSTRUCTOR
      if (! flag_gnu_linker)
        static_ctors = perm_tree_cons (NULL_TREE, fndecl, static_ctors);
      else
#endif
      assemble_constructor (IDENTIFIER_POINTER (CTreeDeclName(fndecl)));
    }
  if (DECL_STATIC_DESTRUCTOR (fndecl))
    {
#ifndef ASM_OUTPUT_DESTRUCTOR
      if (! flag_gnu_linker)
        static_dtors = perm_tree_cons (NULL_TREE, fndecl, static_dtors);
      else
#endif
      assemble_destructor (IDENTIFIER_POINTER (CTreeDeclName(fndecl)));
    }
# endif

  /*
  loc_c_tree_view_node( c_current_function_decl );
  */

  if (! nested)
  {
    /* Let the error reporting routines know that we're outside a
       function.  For a nested function, this value is used in
       pop_c_function_context and then reset via pop_function_context.  */
    c_current_function_decl = NULL;
  }

  return( fndecl );
}

/* Store the list of declarations of the current level.
   This is done for the parameter declarations of a function being defined,
   after they are modified in the light of any missing parameters.  */

static void c_store_decls(decls)

     c_tree_node *decls;
{
  c_current_binding_level->NAME_LIST = decls;
}

/* Similarly, store the list of tags of the current level.  */

static void c_store_tags(tags)

     c_tree_node *tags;
{
  c_current_binding_level->TAG_LIST = tags;
}


/* Return the list of type-tags (for structs, etc) of the current level.  */

c_tree_node *c_get_tags()
{
  return c_current_binding_level->TAG_LIST;
}

/* Return the list of declarations of the current level.
   Note that this list is in reverse order unless/until
   you nreverse it; and when you do nreverse it, you must
   store the result back using `storedecls' or you will lose.  */

c_tree_node *c_get_decls()
{
  return c_current_binding_level->NAME_LIST;
}

/* Return a tree_list node with info on a parameter list just parsed.
   The TREE_PURPOSE is a chain of decls of those parms.
   The TREE_VALUE is a list of structure, union and enum tags defined.
   The CTreeChainis a list of argument types to go in the FUNCTION_TYPE.
   This tree_list node is later fed to `grokparms'.

   VOID_AT_END nonzero means append `void' to the end of the type-list.
   Zero means the parmlist ended with an ellipsis so don't append `void'.  */

c_tree_node *c_get_parm_info(void_at_end)
     int void_at_end;
{
  c_tree_node *decl;
  c_tree_node *result;
  c_tree_node *t;
  c_tree_node *types = 0;
  int erred = 0;
  c_tree_node *tags = c_get_tags();
  c_tree_node *parms = c_get_decls();
  c_tree_node *new_parms = 0;
  c_tree_node *order = c_current_binding_level->PARAM_LIST;


  /* Just `void' (and no ellipsis) is special.  There are really no parms.  */
  if (void_at_end && parms != 0
      && CTreeChain(parms) == 0
      && CTreeTypeMainVariant(CTreeType(parms)) == c_void_type_node
      && CTreeDeclName(parms) == 0)
  {
    parms = NULL;
    c_store_decls(NULL);
    return c_tree_add_head_list( NULL,  NULL,
                  c_tree_add_head_list(NULL, c_void_type_node, NULL));
  }

  /* Extract enumerator values and other non-parms declared with the parms.
     Likewise any forward parm decls that didn't have real parm decls.  */
  for (decl = parms; decl; )
  {
    c_tree_node *next = CTreeChain(decl);

    if ( CTreeDeclCode(decl) != C_PARAM_DECL)
    {
      CTreeChain(decl) = new_parms;
      new_parms = decl;
    }
    else if ( CIsTreeNodeAsmWritten(decl))
    {
      error_with_decl (decl, "parameter `%s' has just a forward declaration");
      CTreeChain(decl) = new_parms;
      new_parms = decl;
    }

    decl = next;
  }

  /* Put the parm decls back in the order they were in in the parm list.  */
  for (t = order; t; t = CTreeChain(t))
  {
    if (CTreeChain(t))
    {
      CTreeChain( CTreeListValue(t)) = CTreeListValue(CTreeChain(t));
    }
    else
    {
      CTreeChain( CTreeListValue(t)) = 0;
    }
  }

  new_parms = c_tree_concat_list(order ? 
        c_tree_reverse_list(CTreeListValue(order)) : 0, new_parms);

  /* Store the parmlist in the binding level since the old one
     is no longer a valid list.  (We have changed the chain pointers.)  */
  c_store_decls(new_parms);

  for (decl = new_parms; decl; decl = CTreeChain(decl))
    /* There may also be declarations for enumerators if an enumeration
       type is declared among the parms.  Ignore them here.  */
  {
    if ( CTreeDeclCode(decl) == C_PARAM_DECL)
    {
      /* Since there is a prototype, args are passed in their declared types.  */
      c_tree_node *type = CTreeType(decl);
      CTreeDeclInitial(decl) = type;

#ifdef PROMOTE_PROTOTYPES
      if ((TREE_CODE (type) == INTEGER_TYPE
           || TREE_CODE (type) == ENUMERAL_TYPE)
          && TYPE_PRECISION (type) < TYPE_PRECISION (integer_type_node))
      {
        CTreeDeclInitial(decl) = c_integer_type_node;
      }
#endif

      types = c_tree_add_head_list( NULL, CTreeType(decl), types);

      if (CTreeTypeMainVariant( CTreeListValue(types)) == c_void_type_node && 
          ! erred && CTreeDeclName(decl) == 0)
      {
        error ("`void' in parameter list must be the entire list");
        erred = 1;
      }
    }
  }

  if (void_at_end)
  {
    result = c_tree_add_head_list( new_parms, tags,
        c_tree_reverse_list( c_tree_add_head_list( NULL, c_void_type_node, types)));
  }
  else
  {
    result = c_tree_add_head_list(new_parms, tags, c_tree_reverse_list( types));
  }

  return( result );
}

/* Store the parameter declarations into the current function declaration.
   This is called after parsing the parameter declarations, before
   digesting the body of the function.

   For an old-style definition, modify the function's type
   to specify at least the number of arguments.  */

void c_store_parm_decls()
{
  c_tree_node *fndecl = c_current_function_decl;
  c_tree_node *parm;

  /* This is either a chain of PARM_DECLs (if a prototype was used)
     or a list of IDENTIFIER_NODEs (for an old-fashioned C definition).  */
  c_tree_node *specparms = c_current_function_parms;

  /* This is a list of types declared among parms in a prototype.  */
  c_tree_node *parmtags = c_current_function_parm_tags;

  /* This is a chain of PARM_DECLs from old-style parm declarations.  */
  c_tree_node *parmdecls = c_get_decls();

  /* This is a chain of any other decls that came in among the parm
     declarations.  If a parm is declared with  enum {foo, bar} x;
     then CONST_DECLs for foo and bar are put here.  */
  c_tree_node *nonparms = 0;

  /* Nonzero if this definition is written with a prototype.  */
  int prototype = 0;

  if (specparms != 0 && CTreeNodeFirstCode(specparms) != C_LIST_NODE)
  {
    /* This case is when the function was defined with an ANSI prototype.
       The parms already have decls, so we need not do anything here
       except record them as in effect
       and complain if any redundant old-style parm decls were written.  */

    c_tree_node *next;
    c_tree_node *others = 0;

    prototype = 1;

    if (parmdecls != 0)
    {
      c_tree_node *decl;
      c_tree_node *link;

      error_with_decl (fndecl, "parm types given both in parmlist and separately");
      /* Get rid of the erroneous decls; don't keep them on
         the list of parms, since they might not be PARM_DECLs.  */
      for (decl = c_current_binding_level->NAME_LIST; decl; decl = CTreeChain(decl))
      {
        if (CTreeDeclName(decl))
        {
          CTreeIdentLocalValue(CTreeDeclName(decl)) = 0;
        }
      }

      for (link = c_current_binding_level->SHADOWED_LIST;
             link; link = CTreeChain(link))
      {
          CTreeIdentLocalValue( CTreeListPurpose(link)) = CTreeListValue(link);
      }

      c_current_binding_level->NAME_LIST = 0;
      c_current_binding_level->SHADOWED_LIST = 0;
    }

    specparms = c_tree_reverse_list(specparms);

    for (parm = specparms; parm; parm = next)
    {
      next = CTreeChain(parm);

      if ( CTreeDeclCode(parm) == C_PARAM_DECL)
      {
        if (CTreeDeclName(parm) == 0)
        {
          error_with_decl (parm, "parameter name omitted");
        }
        else if (CTreeTypeMainVariant(CTreeType(parm)) == c_void_type_node)
        {
          error_with_decl (parm, "parameter `%s' declared void");
                /* Change the type to error_mark_node so this parameter
                   will be ignored by assign_parms.  */
          CTreeType(parm) = c_error_mark_node;
        }

        c_push_decl(parm);
      }
      else
      {
            /* If we find an enum constant or a type tag,
               put it aside for the moment.  */
         CTreeChain(parm) = 0;
         others = c_tree_concat_list(others, parm);
      }
    }

    /* Get the decls in their original chain order
       and record in the function.  */
    CTreeDeclArguments(fndecl) = c_get_decls();

    /* Now pushdecl the enum constants.  */
    for (parm = others; parm; parm = next)
    {
      next = CTreeChain(parm);
      if (CTreeDeclName(parm) == 0) ;
      else if (CTreeTypeMainVariant(CTreeType(parm)) == c_void_type_node) ;
      else if ( CTreeDeclCode(parm) != C_PARAM_DECL)
      {
        c_push_decl(parm);
      }
    }

    c_store_tags( c_tree_concat_list( parmtags, c_get_tags()));
  }
  else
  {
    /* SPECPARMS is an identifier list--a chain of TREE_LIST nodes
       each with a parm name as the TREE_VALUE.

       PARMDECLS is a chain of declarations for parameters.
       Warning! It can also contain CONST_DECLs which are not parameters
       but are names of enumerators of any enum types
       declared among the parameters.

       First match each formal parameter name with its declaration.
       Associate decls with the names and store the decls
       into the TREE_PURPOSE slots.  */

    for (parm = parmdecls; parm; parm = CTreeChain(parm))
    {
      CTreeDeclResult(parm) = 0;
    }

    for (parm = specparms; parm; parm = CTreeChain(parm))
    {
      c_tree_node *tail = NULL;
      c_tree_node *found = NULL;

      if (CTreeListValue(parm) == 0)
      {
        error_with_decl (fndecl, "parameter name missing from parameter list");
        CTreeListPurpose(parm) = 0;
        continue;
      }

      /* See if any of the parmdecls specifies this parm by name.
         Ignore any enumerator decls.  */
      for (tail = parmdecls; tail; tail = CTreeChain(tail))
      {
        if (CTreeDeclName(tail) == CTreeListValue(parm) && 
            CTreeDeclCode(tail) == C_PARAM_DECL)
        {
          found = tail; break;
        }
      }

        /* If declaration already marked, we have a duplicate name.
           Complain, and don't use this decl twice.   */
      if (found && CTreeDeclResult(found) != 0)
      {
        error_with_decl (found, "multiple parameters named `%s'");
        found = 0;
      }

        /* If the declaration says "void", complain and ignore it.  */
      if (found && CTreeTypeMainVariant(CTreeType(found)) == c_void_type_node)
      {
        error_with_decl (found, "parameter `%s' declared void");
        CTreeType(found) = c_integer_type_node;
        CTreeDeclInitial(found) = c_integer_type_node;
        c_layout_decl(found, 0);
      }

        /* Traditionally, a parm declared float is actually a double.  */
      if (found && c_flag_traditional
            && CTreeTypeMainVariant(CTreeType(found)) == c_float_type_node)
      {
        CTreeType(found) = c_double_type_node;
        CTreeDeclInitial(found) = c_double_type_node;
        c_layout_decl(found, 0);
      }

        /* If no declaration found, default to int.  */
      if (!found)
      {
        found = c_tree_build_decl( C_PARAM_DECL,
                     CTreeListValue(parm), c_integer_type_node);
        CTreeDeclInitial(found)  = CTreeType(found);
        CTreeDeclLineNum(found)  = CTreeDeclLineNum(fndecl);
        CTreeDeclFileName(found) = CTreeDeclFileName(fndecl);

        if (c_extra_warnings)
        {
          warning_with_decl (found, "type of `%s' defaults to `int'");
        }
        c_push_decl(found);
      }

      CTreeListPurpose(parm) = found;

        /* Mark this decl as "already found" -- see test, above.
           It is safe to use DECL_RESULT for this
           since it is not used in PARM_DECLs or CONST_DECLs.  */
      CTreeDeclResult(found) = c_error_mark_node;
    }

    /* Put anything which is on the parmdecls chain and which is
       not a PARM_DECL onto the list NONPARMS.  (The types of
       non-parm things which might appear on the list include
       enumerators and NULL-named TYPE_DECL nodes.) Complain about
       any actual PARM_DECLs not matched with any names.  */

    nonparms = 0;
    for (parm = parmdecls; parm; )
    {
      c_tree_node *next = CTreeChain(parm);
      CTreeChain(parm) = 0;

      if ( CTreeDeclCode(parm) != C_PARAM_DECL )
      {
        nonparms = c_tree_concat_list(nonparms, parm);
      }
      else
      {
        /* Complain about args with incomplete types.  */
        if (CTreeTypeSize(CTreeType(parm)) == 0)
        {
          error_with_decl(parm, "parameter `%s' has incomplete type");
          CTreeType(parm) = c_error_mark_node;
        }

        if ( CTreeDeclResult(parm) == 0)
        {
          error_with_decl(parm, "declaration for parameter `%s' but no such parameter");
            /* Pretend the parameter was not missing.
               This gets us to a standard state and minimizes
               further error messages.  */
          specparms = c_tree_concat_list(specparms, 
               c_tree_add_head_list(parm, NULL_TREE, NULL_TREE));
        }
      }

      parm = next;
    }

    /* Chain the declarations together in the order of the list of names.  */
    /* Store that chain in the function decl, replacing the list of names.  */
    parm = specparms;
    CTreeDeclArguments(fndecl) = 0;

    {
      c_tree_node *last;

      for (last = 0; parm; parm = CTreeChain(parm))
      {
        if ( CTreeListPurpose(parm))
        {
          if (last == 0)
          {
            CTreeDeclArguments(fndecl) = CTreeListPurpose(parm);
          }
          else
          {
            CTreeChain(last) = CTreeListPurpose(parm);
          }

          last = CTreeListPurpose(parm);
          CTreeChain(last) = 0;
        }
      }
    }

    /* If there was a previous prototype,
       set the DECL_ARG_TYPE of each argument according to
       the type previously specified, and report any mismatches.  */

    if ( CTreeTypeValues(CTreeType(fndecl)))
    {
      c_tree_node *type;

      for (parm = CTreeDeclArguments(fndecl),
           type = CTreeTypeValues(CTreeType(fndecl));
           parm || 
           (type && (CTreeTypeMainVariant( CTreeListValue(type)) != c_void_type_node));
             parm = CTreeChain(parm), type = CTreeChain(type))
      {
        if (parm == 0 || 
            type == 0 || 
            CTreeTypeMainVariant( CTreeListValue(type)) == c_void_type_node)
        {
          error ("number of arguments doesn't match prototype");
          error_with_file_and_line (c_current_function_prototype_file,
                                    c_current_function_prototype_line,
                                    "prototype declaration");
          break;
        }
        /* Type for passing arg must be consistent
           with that declared for the arg.  */
        if (! c_is_comptypes( CTreeTypeValues(parm), CTreeListValue(type)))
        {
          if (CTreeTypeMainVariant(CTreeType(parm)) == 
              CTreeTypeMainVariant(CTreeListValue(type)))
          {
            /* Adjust argument to match prototype.  E.g. a previous
               `int foo(float);' prototype causes
               `int foo(x) float x; {...}' to be treated like
               `int foo(float x) {...}'.  This is particularly
               useful for argument types like uid_t.  */
            CTreeTypeValues(parm) = CTreeType(parm);
#ifdef PROMOTE_PROTOTYPES
             if ((TREE_CODE (CTreeType(parm)) == INTEGER_TYPE
               || TREE_CODE (CTreeType(parm)) == ENUMERAL_TYPE)
               && TYPE_PRECISION (CTreeType(parm))
               < TYPE_PRECISION (integer_type_node))
             DECL_ARG_TYPE (parm) = integer_type_node;
#endif
             if (c_pedantic)
             {
               pedwarn("promoted argument `%s' doesn't match prototype",
                               CTreeIdentPointer(CTreeDeclName(parm)));
               warning_with_file_and_line(
                   c_current_function_prototype_file,
                   c_current_function_prototype_line, "prototype declaration");
            }
          }
                /* If -traditional, allow `int' argument to match
                   `unsigned' prototype.  */
          else
          if (! (c_flag_traditional
                    && CTreeTypeMainVariant(CTreeType(parm)) == 
                            c_integer_type_node
                    && CTreeTypeMainVariant(CTreeListValue(type)) ==
                            c_unsigned_type_node))
          {
            error ("argument `%s' doesn't match prototype",
                          CTreeIdentPointer(CTreeDeclName(parm)));
            error_with_file_and_line( c_current_function_prototype_file,
                                      c_current_function_prototype_line,
                                      "prototype declaration");
          }
        }
      }

      CTreeTypeNonCopiedParts(CTreeType(fndecl)) = 0;
    }

    /* Otherwise, create a prototype that would match.  */

    else
    {
      c_tree_node *actual = 0;
      c_tree_node *last = 0;
      c_tree_node *type;

      for (parm = CTreeDeclArguments(fndecl); parm; parm = CTreeChain(parm))
      {
        type = c_tree_add_head_list( NULL, CTreeDeclInitial(parm), NULL);

        if (last)
        {
          CTreeChain(last) = type;
        }
        else
        {
          actual = type;
        }
        last = type;
      }

      type = c_tree_add_head_list(NULL, c_void_type_node, NULL);

      if (last)
      {
        CTreeChain(last) = type;
      }
      else
      {
        actual = type;
      }

      /* We are going to assign a new value for the TYPE_ACTUAL_ARG_TYPES
         of the type of this function, but we need to avoid having this
         affect the types of other similarly-typed functions, so we must
         first force the generation of an identical (but separate) type
         node for the relevant function type.  The new node we create
         will be a variant of the main variant of the original function
         type.  */

      CTreeType(fndecl) = c_tree_build_type_copy(CTreeType(fndecl));
      CTreeTypeNonCopiedParts(CTreeType(fndecl)) = actual;
    }

    /* Now store the final chain of decls for the arguments
       as the decl-chain of the current lexical scope.
       Put the enumerators in as well, at the front so that
       DECL_ARGUMENTS is not modified.  */

    c_store_decls( c_tree_concat_list(nonparms, CTreeDeclArguments(fndecl)));
  }

  /* Make sure the binding level for the top of the function body
     gets a BLOCK if there are any in the function.
     Otherwise, the dbx output is wrong.  */

  c_keep_next_if_subblocks = 1;

  /* Write a record describing this function definition to the prototypes
     file (if requested).  */

# ifdef LUDO_NO_SKIP
  gen_aux_info_record (fndecl, 1, 0, prototype);

  /* Initialize the RTL code for the function.  */

  init_function_start(fndecl, input_filename, lineno);
# endif

  /* If this is a varargs function, inform function.c.  */

  if (c_c_function_varargs)
  {
    c_mark_varargs ();
  }

# ifdef LUDO_NO_SKIP
  /* Declare __FUNCTION__ and __PRETTY_FUNCTION__ for this function.  */

  declare_function_name();

  /* Set up parameters and prepare for return, for the function.  */

  expand_function_start(fndecl, 0);

  /* If this function is `main', emit a call to `__main'
     to run global initializers, etc.  */
  if (CTreeDeclName(fndecl)
      && strcmp (IDENTIFIER_POINTER (CTreeDeclName(fndecl)), "main") == 0
      && DECL_CONTEXT (fndecl) == NULL_TREE)
  {
    expand_main_function ();
  }
# endif
}

/* Make TYPE a complete type based on INITIAL_VALUE.
   Return 0 if successful, 1 if INITIAL_VALUE can't be deciphered,
   2 if there was no information (in which case assume 1 if DO_DEFAULT).  */

int c_complete_array_type(type, initial_value, do_default)

     c_tree_node *type;
     c_tree_node *initial_value;
     int do_default;
{
  c_tree_node *maxindex = NULL;
  int value = 0;

  if (initial_value)
    {
      /* Note MAXINDEX  is really the maximum index,
         one less than the size.  */
      if ( CTreeNodeFirstCode(initial_value) == C_STRING_NODE)
      {
        int eltsize
          = c_get_int_size_in_bytes(CTreeType(CTreeType(initial_value)));
        maxindex = c_tree_build_int_2(( CTreeStringLength(initial_value)
                                 / eltsize) - 1, 0);
      }
      else if ( ( CTreeNodeFirstCode(initial_value) == C_EXPR_NODE ) &&
                ( CTreeExprCode(initial_value) == C_CONSTRUCTOR_EXPR) )
      {
        c_tree_node *elts = CTreeExprOperand(initial_value,1);
        maxindex = c_build_size_binop( C_MINUS_EXPR,
                        c_integer_zero_node, c_size_one_node );

        for (; elts; elts = CTreeChain(elts))
        {
          if ( CTreeListPurpose(elts))
          {
            maxindex = CTreeListPurpose(elts);
          }
          else
          {
            maxindex = c_build_size_binop( C_PLUS_EXPR, maxindex, c_size_one_node );
          }
        }
        maxindex = c_tree_copy_node(maxindex);
      }
      else
      {
          /* Make an error message unless that happened already.  */
          if (initial_value != c_error_mark_node)
          {
            value = 1;
          }

          /* Prevent further error messages.  */
          maxindex = c_tree_build_int_2(0, 0);
      }
    }

  if (!maxindex)
    {
      if (do_default)
      {
        maxindex = c_tree_build_int_2(0, 0);
      }
      value = 2;
    }

  if (maxindex)
    {
      CTreeTypeValues(type) = c_tree_build_index_type(maxindex);

      if (!CTreeType(maxindex))
      {
        CTreeType(maxindex) = CTreeTypeValues(type);
      }
    }

  /* Lay out the type now that we can get the real answer.  */

  c_layout_type(type);

  return value;
}


/* Generate an implicit declaration for identifier FUNCTIONID
   as a function of type int ().  Print a warning if appropriate.  */

c_tree_node *c_implicitly_declare(functionid)
     c_tree_node *functionid;
{
  c_tree_node *decl;
  int traditional_warning = 0;
  /* Only one "implicit declaration" warning per identifier.  */
  int implicit_warning;

  /* Save the decl permanently so we can warn if definition follows.  */
# ifdef LUDO_NO_SKIP
  push_obstacks_nochange ();
  end_temporary_allocation ();
# endif

  /* We used to reuse an old implicit decl here,
     but this loses with inline functions because it can clobber
     the saved decl chains.  */
/*  if (IDENTIFIER_IMPLICIT_DECL (functionid) != 0)
    decl = IDENTIFIER_IMPLICIT_DECL (functionid);
  else  */
    decl = c_tree_build_decl(C_FUNCTION_DECL, functionid, c_default_function_type);

  /* Warn of implicit decl following explicit local extern decl.
     This is probably a program designed for traditional C.  */
  if ( CIsTreeNodePublic(functionid) && 
       CTreeIdentGlobalValue(functionid) == 0)
  {
    traditional_warning = 1;
  }

  /* Warn once of an implicit declaration.  */
  implicit_warning = ( CTreeIdentImplicitDecl(functionid) == 0);

  CSetTreeDeclExternal(decl);
  CSetTreeNodePublic(decl);

  /* Record that we have an implicit decl and this is it.  */
  CTreeIdentImplicitDecl(functionid) = decl;

  /* ANSI standard says implicit declarations are in the innermost block.
     So we record the decl in the standard fashion.
     If flag_traditional is set, pushdecl does it top-level.  */
  c_push_decl(decl);

# ifdef LUDO_NO_SKIP
  /* This is a no-op in c-lang.c or something real in objc-actions.c.  */
  maybe_objc_check_decl (decl);

  rest_of_decl_compilation (decl, NULL_PTR, 0, 0);
# endif

  if (c_mesg_implicit_function_declaration && implicit_warning)
    {
      if (c_mesg_implicit_function_declaration == 2)
      {
        error ("implicit declaration of function `%s'", CTreeIdentPointer(functionid));
      }
      else
      {
        warning ("implicit declaration of function `%s'", CTreeIdentPointer(functionid));
      }
    }
  else if (c_warn_traditional && traditional_warning)
  {
    warning ("function `%s' was previously declared within a block",
             CTreeIdentPointer(functionid));
  }

  /* Write a record describing this implicit function declaration to the
     prototypes file (if requested).  */

# ifdef LUDO_NO_SKIP
  gen_aux_info_record (decl, 0, 1, 0);

  pop_obstacks ();
# endif

  return decl;
}

/* Record that this function is going to be a varargs function.
   This is called before store_parm_decls, which is too early
   to call mark_varargs directly.  */

void c_c_mark_varargs()
{
  c_c_function_varargs = 1;
}

/* Indicate that the current function uses extra args
   not explicitly mentioned in the argument list in any fashion.  */

void c_mark_varargs ()
{
  c_current_function_varargs = 1;
}

/*------------------------------------------------------------\
|                                                             |
|                         c_finish_file                       |
|                                                             |
\------------------------------------------------------------*/

void c_finish_file( TreeNode )

  c_tree_node *TreeNode;
{
  c_tree_program_node = TreeNode;

  for ( TreeNode  = C_HEAD_NODE_LIST[ C_IDENTIFIER_NODE ];
        TreeNode != (c_tree_node *)0;
        TreeNode  = TreeNode->COMMON.NEXT )
  {
    CTreeIdentGlobalValue( TreeNode )  = 0;
    CTreeIdentLocalValue( TreeNode )   = 0;
    CTreeIdentLabelValue( TreeNode )   = 0;
    CTreeIdentImplicitDecl( TreeNode ) = 0;
    CTreeIdentErrorLocus( TreeNode )   = 0;
    CTreeIdentLimboValue( TreeNode )   = 0;
  }
}

