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
| File    :                  c_cdecl.h                      |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_CDECL_H
# define C_CDECL_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define C_INT_TYPE_SIZE           (sizeof(int )*8)
# define C_CHAR_TYPE_SIZE          (sizeof(char)*8)
# define C_LONG_TYPE_SIZE          (sizeof(long)*8)
# define C_LONG_LONG_TYPE_SIZE     (sizeof(long long int)*8)
# define C_SHORT_TYPE_SIZE         (sizeof(short)*8)
# define C_FLOAT_TYPE_SIZE         (sizeof(float)*8)
# define C_DOUBLE_TYPE_SIZE        (sizeof(double)*8)
# define C_LONG_DOUBLE_TYPE_SIZE   (sizeof(long double)*8)
# define C_POINTER_TYPE_SIZE       (sizeof(char *)*8)
# define C_POINTER_SIZE            C_POINTER_TYPE_SIZE

# define C_HOST_BITS_PER_WIDE_INT  (int)C_INT_TYPE_SIZE
# define C_HOST_BITS_PER_LONG      (int)C_LONG_TYPE_SIZE
# define C_HOST_BITS_PER_INT       (int)C_INT_TYPE_SIZE
# define C_HOST_BITS_PER_CHAR      (int)C_CHAR_TYPE_SIZE

# define C_HOST_WIDE_INT           int

# define C_WCHAR_TYPE_SIZE         C_INT_TYPE_SIZE
# define C_BITS_PER_UNIT           C_CHAR_TYPE_SIZE
# define C_WCHAR_BYTES             1
# define C_BYTES_BIG_ENDIAN        0
# define C_BITS_PER_WORD           (int)C_INT_TYPE_SIZE

/*------------------------------------------------------------\
|                                                             |
|                           Declar                            |
|                                                             |
\------------------------------------------------------------*/

# define C_CDECL_NORMAL    1
# define C_CDECL_FUNCDEF   2
# define C_CDECL_PARAM     3
# define C_CDECL_FIELD     4
# define C_CDECL_BIT_FIELD 5
# define C_CDECL_TYPENAME  6

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define c_resume_momentary(N)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct c_binding_level
  {
    struct c_binding_level *LEVEL_CHAIN;
    c_tree_node            *NAME_LIST;
    c_tree_node            *TAG_LIST;
    c_tree_node            *SHADOWED_LIST;
    c_tree_node            *BLOCK_LIST;
    c_tree_node            *THIS_BLOCK;
    c_tree_node            *PARAM_LIST;
    char                      PARAM_FLAG;
    char                      KEEP;
    char                      KEEP_IF_SUBBLOCKS;
    char                      TAG_TRANSPARENT;
    char                      N_INCOMPLETE;
    char                      SUBBLOCKS_TAG_TRANSPARENT;
  } c_binding_level;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern  c_tree_node *c_tree_program_node;

  extern  c_tree_node *c_pending_invalid_xref;
  extern  char          *c_pending_invalid_xref_file;
  extern  int            c_pending_invalid_xref_line;
  extern  c_tree_node *c_current_function_decl;
  extern  int            c_current_function_returns_value;
  extern  int            c_current_function_returns_null;

  extern  c_tree_node *c_lastiddecl;

  extern int            c_skip_evaluation;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern c_binding_level * c_alloc_binding_level __P(());
  extern            int  c_global_bindings_p __P(());
  extern           void  c_view_binding __P((c_binding_level *Binding));
  extern           void  c_view_current_binding __P(());
  extern           void  c_clear_parm_order __P(());
  extern           void  c_push_level __P((int tag_transparent));
  extern   c_tree_node * c_pop_level __P((int keep, int reverse, int functionbody));
  extern   c_tree_node * c_lookup_name __P((c_tree_node *TreeName));
  extern   c_tree_node * c_lookup_name_current_level __P((c_tree_node *TreeName));
  extern            int  c_is_self_promoting_args_p __P((c_tree_node *parms));
  extern            int  c_is_chain_member __P((c_tree_node *elem, c_tree_node *chain));
  extern   c_tree_node * c_push_decl __P((c_tree_node *TreeDecl));
  extern           void  c_push_parm_decl __P((c_tree_node *parm));
  extern           void  c_init_decl_processing __P(());
  extern           void  c_decl_attributes __P((c_tree_node *Node, c_tree_node *Attributes, c_tree_node *Prefix_attributes));
  extern   c_tree_node * c_grokfield __P((char *Filename, int Line, c_tree_node *Declarator, c_tree_node *Declspecs, c_tree_node *Width));
  extern   c_tree_node * c_groktypename __P((c_tree_node *Typename));
  extern   c_tree_node * c_start_decl __P((c_tree_node *Declarator, c_tree_node *Declspecs, int Initialized, c_tree_node *Attributes, c_tree_node *Prefix_attributes));
  extern           void  c_finish_decl __P((c_tree_node *decl, c_tree_node *init, c_tree_node *asmspec_tree));
  extern           void  c_split_specs_attrs __P((c_tree_node *Specs_attrs, c_tree_node **Declspecs, c_tree_node **Prefix_attributes));
  extern   c_tree_node * c_round_up __P((c_tree_node *value, int divisor));
  extern           void  c_layout_decl __P((c_tree_node *decl, unsigned known_align));
  extern           void  c_layout_type __P((c_tree_node *type));
  extern           void  c_parmlist_tags_warning __P(());
  extern           void  c_declare_parm_level __P((int definition_flag));
  extern            int  c_in_parm_level_p __P(());
  extern            int  c_kept_level_p __P(());
  extern           void  c_push_tag __P((c_tree_node *name, c_tree_node *type));
  extern   c_tree_node * c_start_struct __P((int code_type, c_tree_node *name));
  extern   c_tree_node * c_finish_struct __P((c_tree_node *t, c_tree_node *fieldlist, c_tree_node *attributes));
  extern   c_tree_node * c_xref_tag __P((int code, c_tree_node *name));
  extern           void  c_shadow_tag_warned __P((c_tree_node *declspecs, int warned));
  extern           void  c_shadow_tag __P((c_tree_node *declspecs));
  extern   c_tree_node * c_shadow_label __P((c_tree_node *name));
  extern   c_tree_node * c_lookup_label __P((c_tree_node *id));
  extern   c_tree_node * c_define_label __P((char *filename, int line, c_tree_node *name));
  extern   c_tree_node * c_start_enum __P((c_tree_node *name));
  extern   c_tree_node * c_finish_enum __P((c_tree_node *enumtype, c_tree_node *values, c_tree_node *attributes));
  extern   c_tree_node * c_build_enumerator __P((c_tree_node *name, c_tree_node *value));
  extern            int  c_start_function __P((c_tree_node *declarator, c_tree_node *declspecs, c_tree_node *prefix_attributes, c_tree_node *attributes, int nested));
  extern   c_tree_node * c_finish_function __P((int nested, c_tree_node *body));
  extern   c_tree_node * c_get_tags __P(());
  extern   c_tree_node * c_get_decls __P(());
  extern   c_tree_node * c_get_parm_info __P((int void_at_end));
  extern           void  c_store_parm_decls __P(());
  extern            int  c_complete_array_type __P((c_tree_node *type, c_tree_node *initial_value, int do_default));
  extern   c_tree_node * c_implicitly_declare __P((c_tree_node *functionid));
  extern           void  c_c_mark_varargs __P(());
  extern           void  c_mark_varargs __P(());

  extern void c_finish_file __P((c_tree_node *TreeNode ));


# endif
