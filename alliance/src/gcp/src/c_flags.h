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
| File    :                  c_flags.h                      |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_FLAGS_H
# define C_FLAGS_H

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

  typedef struct c_file_stack
  {
    struct c_file_stack *NEXT;
    char                  *NAME;
    int                    LINE;
    int                    INDENT_LEVEL;

  } c_file_stack;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

/* Nonzero for -pedantic switch: warn about anything that standard C forbids.  */
  int c_pedantic;

/* Nonzero means try to imitate old fashioned non-ANSI preprocessor.  */
  int c_flag_traditional;

/* Nonzero means `$' can be in an identifier.  */
   extern int c_dollars_in_ident;

/* Nonzero enables objc features.  */
   extern int c_doing_objc_thang;

/* Nonzero means don't recognize the keyword `asm'.  */
   extern int c_flag_no_asm;

/* Temporarily suppress certain warnings.
   extern This is set while reading code from a system header file.  */
   extern int c_in_system_header;

/* Nonzero means warn about any identifiers that match in the first N
   extern characters.  The value N is in `id_clash_len'.  */
   extern int      c_warn_id_clash;
   extern unsigned c_id_clash_len;

/* Nonzero for -fwritable-strings:
   extern store string constants in data segment and don't uniquize them.  */
   extern int c_flag_writable_strings;

/* Nonzero means `char' should be signed.  */
   extern int c_flag_signed_char;

/* Nonzero means give an enum type only as many bytes as it needs.  */
   extern int c_flag_short_enums;

/* Nonzero means warn about function definitions that default the return type
   extern or that use a null return and have a return-type other than void.  */
   extern int c_warn_return_type;

/* Nonzero to warn about variables used before they are initialized.  */
   extern int c_warn_uninitialized;

/* Nonzero to warn about unused local variables.  */
   extern int c_warn_unused;

/* Warn if a switch on an enum fails to have a case for every enum value.  */
   extern int c_warn_switch;

/* Nonzero means warn about all declarations which shadow others.   */
   extern int c_warn_shadow;

/* Nonzero means don't place uninitialized global data in common storage
   extern by default.  */
   extern int c_flag_no_common;

/* Nonzero means warn about any objects definitions whose size is larger
   extern than N bytes.  Also want about function definitions whose returned
   extern values are larger than N bytes. The value N is in `larger_than_size'.  */

   extern int      c_warn_larger_than;
   extern unsigned c_larger_than_size;

/* Nonzero means change certain warnings into errors.
   extern Usually these are warnings about failure to conform to some standard.  */
   extern int c_flag_pedantic_errors;

/* Tag all structures with __attribute__(packed) */
   extern int c_flag_pack_struct;

/* Warn about traditional constructs whose meanings changed in ANSI C.  */
   extern int c_warn_traditional;

/* Nonzero means that we have builtin functions, and main is an int */
   extern int c_flag_hosted;

/* Nonzero means don't recognize any builtin functions.  */
   extern int c_flag_no_builtin;

/* Nonzero means to allow single precision math even if we're generally
   extern being traditional.  */
   extern int flag_allow_single_precision;

/* Warn if main is suspicious.  */
   extern int c_warn_main;

/* Nonzero means to treat bitfields as signed unless they say `unsigned'.  */
   extern int c_flag_signed_bitfields;
   extern int c_explicit_flag_signed_bitfields;

/* Nonzero means allow type mismatches in conditional expressions;
   extern just make their values `void'.   */
   extern int c_flag_cond_mismatch;

/* Nonzero means give `double' the same size as `float'.  */
   extern int c_flag_short_double;

/* Nonzero means handle `#ident' directives.  0 means ignore them.  */
   extern int c_flag_no_ident;

/* Nonzero means don't recognize the non-ANSI builtin functions.
   extern -ansi sets this.  */
   extern int c_flag_no_nonansi_builtin;

/* Nonzero means message about use of implicit function declarations;
 1 means warning; 2 means error. */
   extern int c_mesg_implicit_function_declaration;

/* Nonzero means warn about use of implicit int. */
   extern int c_warn_implicit_int;

/* Nonzero means to allow single precision math even if we're generally
   extern being traditional.  */
   extern int c_flag_allow_single_precision;

/* Nonzero means give string constants the type `const char *'
   extern to get extra warnings from them.  These warnings will be too numerous
   extern to be useful, except in thoroughly ANSIfied programs.  */
   extern int c_warn_write_strings;

/* Nonzero means warn about pointer casts that can drop a type qualifier
   extern from the pointer target type.  */
   extern int c_warn_cast_qual;

/* Nonzero means warn when casting a function call to a type that does
   extern not match the return type (e.g. (float)sqrt() or (anything*)malloc()
   extern when there is no previous declaration of sqrt or malloc.  */
   extern int c_warn_bad_function_cast;


/* Nonzero means warn about sizeof(function) or addition/subtraction
   extern of function pointers.  */
   extern int c_warn_pointer_arith;

/* Nonzero means warn for non-prototype function decls
   extern or non-prototyped defs without previous prototype.  */
   extern int c_warn_strict_prototypes;

/* Nonzero means warn for any global function def
   extern without separate previous prototype decl.  */
   extern int c_warn_missing_prototypes;

/* Nonzero means warn for any global function def
   extern without separate previous decl.  */
   extern int c_warn_missing_declarations;

/* Nonzero means warn about multiple (redundant) decls for the same single
   extern variable or function.  */
   extern int c_warn_redundant_decls;

/* Nonzero means warn about extern declarations of objects not at
   extern file-scope level and about *all* declarations of functions (whether
   extern extern or static) not at file-scope level.  Note that we exclude
   extern implicit function declarations.  To get warnings about those, use
   extern -Wimplicit.  */
   extern int c_warn_nested_externs;

/* Warn about *printf or *scanf format/argument anomalies.  */
   extern int c_warn_format;

/* Warn about a subscript that has type char.  */
   extern int c_warn_char_subscripts;

/* Warn if a type conversion is done that might have confusing results.  */
   extern int c_warn_conversion;

/* Warn if adding () is suggested.  */
   extern int c_warn_parentheses;

/* Warn if initializer is not completely bracketed.  */
   extern int c_warn_missing_braces;

/* Warn about comparison of signed and unsigned values.
   extern If -1, neither -Wsign-compare nor -Wno-sign-compare has been specified.  */
   extern int c_warn_sign_compare;

/* Temporarily suppress certain warnings.
   This is set while reading code from a system header file.  */
   extern int c_in_system_header;

/* Name of current original source file (what was input to cpp).
   This comes from each #-command in the actual input.  */
   extern char *c_input_filename;

/* Name of top-level original source file (what was input to cpp).
   This comes from the #-command at the beginning of the actual input.
   If there isn't any there, then this is the cc1 input file name.  */
   extern char *c_main_input_filename;

/* Current line number in real source file.  */
   extern int c_lineno;

/* Stack of currently pending input files.  */
   extern c_file_stack *c_input_file_stack;

/* Incremented on each change to input_file_stack.  */
   extern int c_input_file_stack_tick;

/* Nonzero for -ffloat-store: don't allocate floats and doubles
   in extended-precision registers.  */
   extern int c_flag_float_store;

/* Nonzero means all references through pointers are volatile.  */
   extern int c_flag_volatile;

/* Don't print warning messages.  -w.  */
   extern int c_inhibit_warnings;

/* Print various extra warnings.  -W.  */
   extern int c_extra_warnings;

/* Treat warnings as errors.  -Werror.  */
   extern int c_warnings_are_errors;

/* Nonzero means we should be saving declaration info into a .X file.  */
   extern int c_flag_gen_aux_info;

/* Nonzero allows GCC to violate some IEEE or ANSI rules regarding math
   operations in the interest of optimization.  For example it allows
   GCC to assume arguments to sqrt are nonnegative numbers, allowing
   faster code for sqrt to be generated.  */
   extern int c_flag_fast_math;
   
   extern int c_parse_flags __P((int Argc, char **Argv));

# endif
