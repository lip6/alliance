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
| Tool    :                     C                           |
|                                                             |
| File    :                   c_flags.c                     |
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

/* Nonzero for -pedantic switch: warn about anything that standard C forbids.  */
  int c_pedantic = 0;

/* Nonzero means try to imitate old fashioned non-ANSI preprocessor.  */
  int c_flag_traditional = 0;

/* Nonzero means `$' can be in an identifier.  */
   int c_dollars_in_ident = 0;

/* Nonzero enables objc features.  */
   int c_doing_objc_thang = 0;

/* Nonzero means don't recognize the keyword `asm'.  */
   int c_flag_no_asm = 0;

/* Nonzero means warn about any identifiers that match in the first N
   characters.  The value N is in `id_clash_len'.  */
   int      c_warn_id_clash;
   unsigned c_id_clash_len;

/* Nonzero for -fwritable-strings:
   store string constants in data segment and don't uniquize them.  */
   int c_flag_writable_strings;

/* Nonzero means `char' should be signed.  */
   int c_flag_signed_char;

/* Nonzero means give an enum type only as many bytes as it needs.  */
   int c_flag_short_enums;

/* Nonzero means warn about function definitions that default the return type
   or that use a null return and have a return-type other than void.  */
   int c_warn_return_type;

/* Nonzero to warn about variables used before they are initialized.  */
   int c_warn_uninitialized;

/* Nonzero to warn about unused local variables.  */
   int c_warn_unused;

/* Warn if a switch on an enum fails to have a case for every enum value.  */
   int c_warn_switch;

/* Nonzero means warn about all declarations which shadow others.   */
   int c_warn_shadow;

/* Nonzero means don't place uninitialized global data in common storage
   by default.  */
   int c_flag_no_common;

/* Nonzero means warn about any objects definitions whose size is larger
   than N bytes.  Also want about function definitions whose returned
   values are larger than N bytes. The value N is in `larger_than_size'.  */

   int      c_warn_larger_than;
   unsigned c_larger_than_size;

/* Nonzero means change certain warnings into errors.
   Usually these are warnings about failure to conform to some standard.  */
   int c_flag_pedantic_errors = 0;

/* Tag all structures with __attribute__(packed) */
   int c_flag_pack_struct = 0;

/* Warn about traditional constructs whose meanings changed in ANSI C.  */
   int c_warn_traditional;

/* Nonzero means that we have builtin functions, and main is an int */
   int c_flag_hosted = 1;

/* Nonzero means don't recognize any builtin functions.  */
   int c_flag_no_builtin;

/* Nonzero means to allow single precision math even if we're generally
   being traditional.  */
   int flag_allow_single_precision = 0;

/* Warn if main is suspicious.  */
   int c_warn_main;

/* Nonzero means to treat bitfields as signed unless they say `unsigned'.  */
   int c_flag_signed_bitfields = 1;
   int c_explicit_flag_signed_bitfields = 0;

/* Nonzero means allow type mismatches in conditional expressions;
   just make their values `void'.   */
   int c_flag_cond_mismatch;

/* Nonzero means give `double' the same size as `float'.  */
   int c_flag_short_double;

/* Nonzero means handle `#ident' directives.  0 means ignore them.  */
   int c_flag_no_ident = 0;

/* Nonzero means don't recognize the non-ANSI builtin functions.
   -ansi sets this.  */
   int c_flag_no_nonansi_builtin;

/* Nonzero means message about use of implicit function declarations;
 1 means warning; 2 means error. */
   int c_mesg_implicit_function_declaration;

/* Nonzero means warn about use of implicit int. */
   int c_warn_implicit_int;

/* Nonzero means to allow single precision math even if we're generally
   being traditional.  */
   int c_flag_allow_single_precision = 0;

/* Nonzero means give string constants the type `const char *'
   to get extra warnings from them.  These warnings will be too numerous
   to be useful, except in thoroughly ANSIfied programs.  */
   int c_warn_write_strings;

/* Nonzero means warn about pointer casts that can drop a type qualifier
   from the pointer target type.  */
   int c_warn_cast_qual;

/* Nonzero means warn when casting a function call to a type that does
   not match the return type (e.g. (float)sqrt() or (anything*)malloc()
   when there is no previous declaration of sqrt or malloc.  */
   int c_warn_bad_function_cast;


/* Nonzero means warn about sizeof(function) or addition/subtraction
   of function pointers.  */
   int c_warn_pointer_arith;

/* Nonzero means warn for non-prototype function decls
   or non-prototyped defs without previous prototype.  */
   int c_warn_strict_prototypes;

/* Nonzero means warn for any global function def
   without separate previous prototype decl.  */
   int c_warn_missing_prototypes;

/* Nonzero means warn for any global function def
   without separate previous decl.  */
   int c_warn_missing_declarations;

/* Nonzero means warn about multiple (redundant) decls for the same single
   variable or function.  */
   int c_warn_redundant_decls = 0;

/* Nonzero means warn about extern declarations of objects not at
   file-scope level and about *all* declarations of functions (whether
   extern or static) not at file-scope level.  Note that we exclude
   implicit function declarations.  To get warnings about those, use
   -Wimplicit.  */
   int c_warn_nested_externs = 0;

/* Warn about *printf or *scanf format/argument anomalies.  */
   int c_warn_format;

/* Warn about a subscript that has type char.  */
   int c_warn_char_subscripts = 0;

/* Warn if a type conversion is done that might have confusing results.  */
   int c_warn_conversion;

/* Warn if adding () is suggested.  */
   int c_warn_parentheses;

/* Warn if initializer is not completely bracketed.  */
   int c_warn_missing_braces;

/* Warn about comparison of signed and unsigned values.
   If -1, neither -Wsign-compare nor -Wno-sign-compare has been specified.  */
   int c_warn_sign_compare = -1;

/* Temporarily suppress certain warnings.
   This is set while reading code from a system header file.  */
   int c_in_system_header = 0;

/* Name of current original source file (what was input to cpp).
   This comes from each #-command in the actual input.  */
   char *c_input_filename;

/* Name of top-level original source file (what was input to cpp).
   This comes from the #-command at the beginning of the actual input.
   If there isn't any there, then this is the cc1 input file name.  */
   char *c_main_input_filename;

/* Stack of currently pending input files.  */
   c_file_stack *c_input_file_stack;

/* Incremented on each change to input_file_stack.  */
   int c_input_file_stack_tick;

/* Nonzero for -ffloat-store: don't allocate floats and doubles
   in extended-precision registers.  */
   int c_flag_float_store = 0;


/* Nonzero means all references through pointers are volatile.  */
   int c_flag_volatile;

/* Don't print warning messages.  -w.  */
   int c_inhibit_warnings = 0;

/* Print various extra warnings.  -W.  */
   int c_extra_warnings = 0;

/* Treat warnings as errors.  -Werror.  */
   int c_warnings_are_errors = 0;

/* Nonzero means we should be saving declaration info into a .X file.  */
   int c_flag_gen_aux_info = 0;

/* Nonzero allows GCC to violate some IEEE or ANSI rules regarding math
   operations in the interest of optimization.  For example it allows
   GCC to assume arguments to sqrt are nonnegative numbers, allowing
   faster code for sqrt to be generated.  */
   int c_flag_fast_math = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static void c_init_flags()
{
/* Nonzero for -pedantic switch: warn about anything that standard C forbids.  */
  c_pedantic = 0;

/* Nonzero means try to imitate old fashioned non-ANSI preprocessor.  */
  c_flag_traditional = 0;

/* Nonzero means `$' can be in an identifier.  */
  c_dollars_in_ident = 0;

/* Nonzero enables objc features.  */
  c_doing_objc_thang = 0;

/* Nonzero means don't recognize the keyword `asm'.  */
  c_flag_no_asm = 0;

/* Nonzero means warn about any identifiers that match in the first N
   characters.  The value N is in `id_clash_len'.  */
  c_warn_id_clash = 0;
  c_id_clash_len = 0;

/* Nonzero for -fwritable-strings:
   store string constants in data segment and don't uniquize them.  */
   c_flag_writable_strings = 0;

/* Nonzero means `char' should be signed.  */
   c_flag_signed_char = 0;

/* Nonzero means give an enum type only as many bytes as it needs.  */
   c_flag_short_enums = 0;

/* Nonzero means warn about function definitions that default the return type
   or that use a null return and have a return-type other than void.  */
  c_warn_return_type = 0;

/* Nonzero to warn about variables used before they are initialized.  */
  c_warn_uninitialized = 0;

/* Nonzero to warn about unused local variables.  */
  c_warn_unused = 0;

/* Warn if a switch on an enum fails to have a case for every enum value.  */
  c_warn_switch = 0;

/* Nonzero means warn about all declarations which shadow others.   */
  c_warn_shadow = 0;

/* Nonzero means don't place uninitialized global data in common storage
   by default.  */
  c_flag_no_common = 0;

/* Nonzero means warn about any objects definitions whose size is larger
   than N bytes.  Also want about function definitions whose returned
   values are larger than N bytes. The value N is in `larger_than_size'.  */

   c_warn_larger_than = 0;
   c_larger_than_size = 0;

/* Nonzero means change certain warnings into errors.
   Usually these are warnings about failure to conform to some standard.  */
  c_flag_pedantic_errors = 0;

/* Tag all structures with __attribute__(packed) */
  c_flag_pack_struct = 0;

/* Warn about traditional constructs whose meanings changed in ANSI C.  */
  c_warn_traditional = 0;

/* Nonzero means that we have builtin functions, and main is an*/
  c_flag_hosted = 1;

/* Nonzero means don't recognize any builtin functions.  */
  c_flag_no_builtin = 0;

/* Nonzero means to allow single precision math even if we're generally
   being traditional.  */
  flag_allow_single_precision = 0;

/* Warn if main is suspicious.  */
  c_warn_main = 0;

/* Nonzero means to treat bitfields as signed unless they say `unsigned'.  */
  c_flag_signed_bitfields = 1;
  c_explicit_flag_signed_bitfields = 0;

/* Nonzero means allow type mismatches in conditional expressions = 0;
   just make their values `void'.   */
  c_flag_cond_mismatch = 0;

/* Nonzero means give `double' the same size as `float'.  */
  c_flag_short_double = 0;

/* Nonzero means handle `#ident' directives.  0 means ignore them.  */
  c_flag_no_ident = 0;

/* Nonzero means don't recognize the non-ANSI builtin functions.
   -ansi sets this.  */
  c_flag_no_nonansi_builtin = 0;

/* Nonzero means message about use of implicit function declarations = 0;
 1 means warning; 2 means error. */
  c_mesg_implicit_function_declaration = 0;

/* Nonzero means warn about use of implicit int. */
  c_warn_implicit_int = 0;

/* Nonzero means to allow single precision math even if we're generally
   being traditional.  */
  c_flag_allow_single_precision = 0;

/* Nonzero means give string constants the type `const char *'
   to get extra warnings from them.  These warnings will be too numerous
   to be useful, except in thoroughly ANSIfied programs.  */
  c_warn_write_strings = 0;

/* Nonzero means warn about pointer casts that can drop a type qualifier
   from the pointer target type.  */
  c_warn_cast_qual = 0;

/* Nonzero means warn when casting a function call to a type that does
   not match the return type (e.g. (float)sqrt() or (anything*)malloc()
   when there is no previous declaration of sqrt or malloc.  */
  c_warn_bad_function_cast = 0;


/* Nonzero means warn about sizeof(function) or addition/subtraction
   of function pointers.  */
  c_warn_pointer_arith = 0;

/* Nonzero means warn for non-prototype function decls
   or non-prototyped defs without previous prototype.  */
  c_warn_strict_prototypes = 0;

/* Nonzero means warn for any global function def
   without separate previous prototype decl.  */
  c_warn_missing_prototypes = 0;

/* Nonzero means warn for any global function def
   without separate previous decl.  */
  c_warn_missing_declarations = 0;

/* Nonzero means warn about multiple (redundant) decls for the same single
   variable or function.  */
  c_warn_redundant_decls = 0;

/* Nonzero means warn about extern declarations of objects not at
   file-scope level and about *all* declarations of functions (whether
   extern or static) not at file-scope level.  Note that we exclude
   implicit function declarations.  To get warnings about those, use
   -Wimplicit.  */
  c_warn_nested_externs = 0;

/* Warn about *printf or *scanf format/argument anomalies.  */
  c_warn_format = 0;

/* Warn about a subscript that has type char.  */
  c_warn_char_subscripts = 0;

/* Warn if a type conversion is done that might have confusing results.  */
  c_warn_conversion = 0;

/* Warn if adding () is suggested.  */
  c_warn_parentheses = 0;

/* Warn if initializer is not completely bracketed.  */
  c_warn_missing_braces = 0;

/* Warn about comparison of signed and unsigned values.
   If -1, neither -Wsign-compare nor -Wno-sign-compare has been specified.  */
  c_warn_sign_compare = -1;

/* Temporarily suppress certain warnings.
   This is set while reading code from a system header file.  */
  c_in_system_header = 0;

/* Name of current original source file (what was input to cpp).
   This comes from each #-command in the actual input.  */
  c_input_filename = 0;

/* Name of top-level original source file (what was input to cpp).
   This comes from the #-command at the beginning of the actual input.
   If there isn't any there, then this is the cc1 input file name.  */
  c_main_input_filename = 0;

/* Stack of currently pending input files.  */
   c_input_file_stack = 0;

/* Incremented on each change to input_file_stack.  */
  c_input_file_stack_tick = 0;

/* Nonzero for -ffloat-store: don't allocate floats and doubles
   in extended-precision registers.  */
  c_flag_float_store = 0;


/* Nonzero means all references through pointers are volatile.  */
  c_flag_volatile = 0;

/* Don't print warning messages.  -w.  */
  c_inhibit_warnings = 0;

/* Print various extra warnings.  -W.  */
  c_extra_warnings = 0;

/* Treat warnings as errors.  -Werror.  */
  c_warnings_are_errors = 0;

/* Nonzero means we should be saving declaration info into a .X file.  */
  c_flag_gen_aux_info = 0;

/* Nonzero allows GCC to violate some IEEE or ANSI rules regarding math
   operations in the interest of optimization.  For example it allows
   GCC to assume arguments to sqrt are nonnegative numbers, allowing
   faster code for sqrt to be generated.  */
  c_flag_fast_math = 0;
}

static int c_parse_one_flags( p )

  char *p;
{
  c_init_flags();

  if (!strcmp (p, "-ftraditional") || !strcmp (p, "-traditional"))
    {
      c_flag_traditional = 1;
      c_flag_writable_strings = 1;
    }
  else if (!strcmp (p, "-fallow-single-precision"))
    c_flag_allow_single_precision = 1;
  else if (!strcmp (p, "-fhosted") || !strcmp (p, "-fno-freestanding"))
    {
      c_flag_hosted = 1;
      c_flag_no_builtin = 0;
    }
  else if (!strcmp (p, "-ffreestanding") || !strcmp (p, "-fno-hosted"))
    {
      c_flag_hosted = 0;
      c_flag_no_builtin = 1;
      /* warn_main will be 2 if set by -Wall, 1 if set by -Wmain */
      if (c_warn_main == 2)
	c_warn_main = 0;
    }
  else if (!strcmp (p, "-fnotraditional") || !strcmp (p, "-fno-traditional"))
    {
      c_flag_traditional = 0;
      c_flag_writable_strings = 0;
    }
  else if (!strcmp (p, "-fdollars-in-identifiers"))
    c_dollars_in_ident = 1;
  else if (!strcmp (p, "-fno-dollars-in-identifiers"))
    c_dollars_in_ident = 0;
  else if (!strcmp (p, "-fsigned-char"))
    c_flag_signed_char = 1;
  else if (!strcmp (p, "-funsigned-char"))
    c_flag_signed_char = 0;
  else if (!strcmp (p, "-fno-signed-char"))
    c_flag_signed_char = 0;
  else if (!strcmp (p, "-fno-unsigned-char"))
    c_flag_signed_char = 1;
  else if (!strcmp (p, "-fsigned-bitfields")
	   || !strcmp (p, "-fno-unsigned-bitfields"))
    {
      c_flag_signed_bitfields = 1;
      c_explicit_flag_signed_bitfields = 1;
    }
  else if (!strcmp (p, "-funsigned-bitfields")
	   || !strcmp (p, "-fno-signed-bitfields"))
    {
      c_flag_signed_bitfields = 0;
      c_explicit_flag_signed_bitfields = 1;
    }
  else if (!strcmp (p, "-fshort-enums"))
    c_flag_short_enums = 1;
  else if (!strcmp (p, "-fno-short-enums"))
    c_flag_short_enums = 0;
  else if (!strcmp (p, "-fcond-mismatch"))
    c_flag_cond_mismatch = 1;
  else if (!strcmp (p, "-fno-cond-mismatch"))
    c_flag_cond_mismatch = 0;
  else if (!strcmp (p, "-fshort-double"))
    c_flag_short_double = 1;
  else if (!strcmp (p, "-fno-short-double"))
    c_flag_short_double = 0;
  else if (!strcmp (p, "-fasm"))
    c_flag_no_asm = 0;
  else if (!strcmp (p, "-fno-asm"))
    c_flag_no_asm = 1;
  else if (!strcmp (p, "-fbuiltin"))
    c_flag_no_builtin = 0;
  else if (!strcmp (p, "-fno-builtin"))
    c_flag_no_builtin = 1;
  else if (!strcmp (p, "-fno-ident"))
    c_flag_no_ident = 1;
  else if (!strcmp (p, "-fident"))
    c_flag_no_ident = 0;
  else if (!strcmp (p, "-ansi"))
    c_flag_no_asm = 1, c_flag_no_nonansi_builtin = 1;
  else if (!strcmp (p, "-Werror-implicit-function-declaration"))
    c_mesg_implicit_function_declaration = 2;
  else if (!strcmp (p, "-Wimplicit-function-declaration"))
    c_mesg_implicit_function_declaration = 1;
  else if (!strcmp (p, "-Wno-implicit-function-declaration"))
    c_mesg_implicit_function_declaration = 0;
  else if (!strcmp (p, "-Wimplicit-int"))
    c_warn_implicit_int = 1;
  else if (!strcmp (p, "-Wno-implicit-int"))
    c_warn_implicit_int = 0;
  else if (!strcmp (p, "-Wimplicit"))
    {
      c_warn_implicit_int = 1;
      if (c_mesg_implicit_function_declaration != 2)
        c_mesg_implicit_function_declaration = 1;
    }
  else if (!strcmp (p, "-Wno-implicit"))
    c_warn_implicit_int = 0, c_mesg_implicit_function_declaration = 0;
  else if (!strcmp (p, "-Wwrite-strings"))
    c_warn_write_strings = 1;
  else if (!strcmp (p, "-Wno-write-strings"))
    c_warn_write_strings = 0;
  else if (!strcmp (p, "-Wcast-qual"))
    c_warn_cast_qual = 1;
  else if (!strcmp (p, "-Wno-cast-qual"))
    c_warn_cast_qual = 0;
  else if (!strcmp (p, "-Wbad-function-cast"))
    c_warn_bad_function_cast = 1;
  else if (!strcmp (p, "-Wno-bad-function-cast"))
    c_warn_bad_function_cast = 0;
  else if (!strcmp (p, "-Wpointer-arith"))
    c_warn_pointer_arith = 1;
  else if (!strcmp (p, "-Wno-pointer-arith"))
    c_warn_pointer_arith = 0;
  else if (!strcmp (p, "-Wstrict-prototypes"))
    c_warn_strict_prototypes = 1;
  else if (!strcmp (p, "-Wno-strict-prototypes"))
    c_warn_strict_prototypes = 0;
  else if (!strcmp (p, "-Wmissing-prototypes"))
    c_warn_missing_prototypes = 1;
  else if (!strcmp (p, "-Wno-missing-prototypes"))
    c_warn_missing_prototypes = 0;
  else if (!strcmp (p, "-Wmissing-declarations"))
    c_warn_missing_declarations = 1;
  else if (!strcmp (p, "-Wno-missing-declarations"))
    c_warn_missing_declarations = 0;
  else if (!strcmp (p, "-Wredundant-decls"))
    c_warn_redundant_decls = 1;
  else if (!strcmp (p, "-Wno-redundant-decls"))
    c_warn_redundant_decls = 0;
  else if (!strcmp (p, "-Wnested-externs"))
    c_warn_nested_externs = 1;
  else if (!strcmp (p, "-Wno-nested-externs"))
    c_warn_nested_externs = 0;
  else if (!strcmp (p, "-Wtraditional"))
    c_warn_traditional = 1;
  else if (!strcmp (p, "-Wno-traditional"))
    c_warn_traditional = 0;
  else if (!strcmp (p, "-Wformat"))
    c_warn_format = 1;
  else if (!strcmp (p, "-Wno-format"))
    c_warn_format = 0;
  else if (!strcmp (p, "-Wchar-subscripts"))
    c_warn_char_subscripts = 1;
  else if (!strcmp (p, "-Wno-char-subscripts"))
    c_warn_char_subscripts = 0;
  else if (!strcmp (p, "-Wconversion"))
    c_warn_conversion = 1;
  else if (!strcmp (p, "-Wno-conversion"))
    c_warn_conversion = 0;
  else if (!strcmp (p, "-Wparentheses"))
    c_warn_parentheses = 1;
  else if (!strcmp (p, "-Wno-parentheses"))
    c_warn_parentheses = 0;
  else if (!strcmp (p, "-Wreturn-type"))
    c_warn_return_type = 1;
  else if (!strcmp (p, "-Wno-return-type"))
    c_warn_return_type = 0;
  else if (!strcmp (p, "-Wcomment"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-comment"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wcomments"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-comments"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wtrigraphs"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-trigraphs"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wundef"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-undef"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wimport"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wno-import"))
    ; /* cpp handles this one.  */
  else if (!strcmp (p, "-Wmissing-braces"))
    c_warn_missing_braces = 1;
  else if (!strcmp (p, "-Wno-missing-braces"))
    c_warn_missing_braces = 0;
  else if (!strcmp (p, "-Wmain"))
    c_warn_main = 1;
  else if (!strcmp (p, "-Wno-main"))
    c_warn_main = 0;
  else if (!strcmp (p, "-Wsign-compare"))
    c_warn_sign_compare = 1;
  else if (!strcmp (p, "-Wno-sign-compare"))
    c_warn_sign_compare = 0;
  else if (!strcmp (p, "-Wall"))
    {
      /* We save the value of warn_uninitialized, since if they put
	 -Wuninitialized on the command line, we need to generate a
	 warning about not using it without also specifying -O.  */
      if (c_warn_uninitialized != 1)
	c_warn_uninitialized = 2;
      c_warn_implicit_int = 1;
      c_mesg_implicit_function_declaration = 1;
      c_warn_return_type = 1;
      c_warn_unused = 1;
      c_warn_switch = 1;
      c_warn_format = 1;
      c_warn_char_subscripts = 1;
      c_warn_parentheses = 1;
      c_warn_missing_braces = 1;
      /* We set this to 2 here, but 1 in -Wmain, so -ffreestanding can turn
	 it off only if it's not explicit.  */
      c_warn_main = 2;
    }
  else
    return 0;

  return 1;
}

int c_parse_flags( Argc, Argv )

  int    Argc;
  char **Argv;
{
  int Index;
  int Error;

  Error = 0;

  for ( Index = 0; Index < Argc; Index++ )
  {
    if ( ! c_parse_one_flags( Argv[ Index ] ) ) Error = 1;
  }

  return( Error );
}
