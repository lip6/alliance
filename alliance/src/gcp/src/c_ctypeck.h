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
| Tool    :                     C                           |
|                                                             |
| File    :                c_ctypeck.h                      |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_CTYPECK_H
# define C_CTYPECK_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
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

  extern            int  c_is_comptypes __P((c_tree_node *type1, c_tree_node *type2));
  extern   c_tree_node * c_parser_build_indirect_ref __P((c_tree_node *ptr, char *errorstring));
  extern   c_tree_node * c_parser_build_conditional_expr __P((c_tree_node *TreeIf, c_tree_node *TreeArg1, c_tree_node *TreeArg2));
  extern c_tree_node   * c_parser_build_modify_expr __P(( c_tree_node *lhs, int modifycode, c_tree_node *rhs));
  extern   c_tree_node * c_get_non_lvalue __P((c_tree_node *x));
  extern   c_tree_node * c_parser_build_compound_expr __P((c_tree_node *list));
  extern   c_tree_node * c_is_initializer_constant_valid_p __P((c_tree_node *value, c_tree_node *endtype));
  extern           void  c_store_init_value __P((c_tree_node *decl, c_tree_node *init));
  extern   c_tree_node * c_get_signed_or_unsigned_type __P((int unsignedp, c_tree_node *type));
  extern   c_tree_node * c_get_signed_type __P((c_tree_node *type));
  extern   c_tree_node * c_get_unsigned_type __P((c_tree_node *type));
  extern            int  c_int_fits_type_p __P((c_tree_node *c, c_tree_node *type));
  extern            int  c_is_tree_int_cst_lt __P((c_tree_node *t1, c_tree_node *t2));
  extern   c_tree_node * c_get_type_for_size __P((unsigned bits, int unsignedp));
  extern            int  c_type_precision __P((c_tree_node *type));
  extern   c_tree_node * c_c_build_type_variant __P((c_tree_node *type, int constp, int volatilep));
  extern           void  c_set_init_label __P((c_tree_node *fieldname));
  extern           void  c_set_init_index __P((c_tree_node *first, c_tree_node *last));
  extern   c_tree_node * c_build_c_cast __P((c_tree_node *type, c_tree_node *expr));
  extern   c_tree_node * c_get_c_sizeof __P((c_tree_node *type));
  extern   c_tree_node * c_get_c_alignof __P((c_tree_node *type));
  extern   c_tree_node * c_get_c_alignof_expr __P((c_tree_node *expr));
  extern            int  c_get_int_size_in_bytes __P((c_tree_node *type));
  extern   c_tree_node * c_parser_build_function_call __P((c_tree_node *function, c_tree_node *params));
  extern           void  c_incomplete_type_error __P((c_tree_node *value, c_tree_node *type));
  extern   c_tree_node * c_require_complete_type __P((c_tree_node *value));
  extern            int  c_is_lvalue_p __P((c_tree_node *ref));
  extern   c_tree_node * c_parser_build_array_ref __P((c_tree_node *array, c_tree_node *index));
  extern   c_tree_node * c_parser_build_component_ref __P((c_tree_node *datum, c_tree_node *component));
  extern c_tree_node   * c_get_size_in_bytes __P((c_tree_node *type));

  extern c_tree_node * c_build_binary_op __P(( int code, c_tree_node *orig_op0, c_tree_node *orig_op1, int convert_p));
  extern c_tree_node * c_parser_build_binary_op __P(( int code, c_tree_node *arg1, c_tree_node *arg2));
  extern c_tree_node * c_parser_build_unary_op __P((int code, c_tree_node *xarg, int noconvert));
  extern c_tree_node * c_get_narrower __P((c_tree_node *op, int *unsignedp_ptr));
  extern c_tree_node * c_get_unwidened __P((c_tree_node *op, c_tree_node *for_type));
  extern int           c_mark_addressable __P((c_tree_node *exp));
  extern void          c_process_init_element __P((c_tree_node *value));
  extern c_tree_node * c_pop_init_level __P((int implicit));
  extern void          c_push_init_level __P((int implicit));
  extern void          c_really_start_incremental_init __P((c_tree_node *type));
  extern c_tree_node * c_finish_init __P(());
  extern void          c_start_init __P((c_tree_node *decl, c_tree_node *asmspec_tree, int top_level));
  extern c_tree_node * c_get_common_type __P((c_tree_node *t1, c_tree_node *t2));
  extern int c_force_fit_type __P((c_tree_node *t, int overflow ));

# endif
