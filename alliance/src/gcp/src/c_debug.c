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
| File    :                   c_debug.c                     |
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
# include <varargs.h>
# include <stdarg.h>
# include <ctype.h>


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

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

#ifndef VPROTO
#ifdef __STDC__
#define PVPROTO(ARGS)		ARGS
#define VPROTO(ARGS)            ARGS
#define VA_START(va_list,var)  va_start(va_list,var)
#else
#define PVPROTO(ARGS)		()
#define VPROTO(ARGS)            (va_alist) va_dcl
#define VA_START(va_list,var)  va_start(va_list)
#endif
#endif

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

static int c_need_error_newline;

/* Function of last error message;
   more generally, function such that if next error message is in it
   then we don't have to mention the function name.  */
static c_tree_node *c_last_error_function = NULL;

/* Used to detect when input_file_stack has changed since last described.  */
static int c_last_error_tick;

/* Called when the start of a function definition is parsed,
   this function prints on stdout the name of the function.  */

/* Number of error messages and warning messages so far.  */

   int c_errorcount = 0;
   int c_warningcount = 0;
   int c_sorrycount = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/* Count an error or warning.  Return 1 if the message should be printed.  */

int c_count_error(warningp)
     int warningp;
{
  if (warningp && c_inhibit_warnings)
    return 0;

  if (warningp && !c_warnings_are_errors)
    c_warningcount++;
  else
    {
      static int warning_message = 0;

      if (warningp && !warning_message)
	{
	  fprintf (stdout, "warnings being treated as errors\n" );
	  warning_message = 1;
	}
      c_errorcount++;
    }

  return 1;
}

static char *c_print_decl_name(decl, verbosity)
     c_tree_node *decl;
     int verbosity;
{
  return CTreeIdentPointer( CTreeDeclName(decl));
}

static void c_default_print_error_function(file)
     char *file;
{
  if (c_last_error_function != c_current_function_decl)
    {
      char *kind = "function";
      if (c_current_function_decl != 0
          && CTreeTypeCode( CTreeType(c_current_function_decl)) == C_METHOD_TYPE)
      {
        kind = "method";
      }

      if (file)
      {
        fprintf (stdout, "%s: ", file);
      }

      if (c_current_function_decl == NULL)
      {
        fprintf (stdout, "At top level:\n");
      }
      else
      {
        char *name = c_print_decl_name(c_current_function_decl, 2);
        fprintf (stdout, "In %s `%s':\n", kind, name);
      }

      c_last_error_function = c_current_function_decl;
    }
}

/* Prints out, if necessary, the name of the current function
  that caused an error.  Called from all error and warning functions.  */

void c_report_error_function(file)
     char *file;
{
  c_file_stack *p;

  if (c_need_error_newline)
    {
      fprintf (stdout, "\n");
      c_need_error_newline = 0;
    }

  c_default_print_error_function(file);

  if (c_input_file_stack && c_input_file_stack->NEXT != 0
      && c_input_file_stack_tick != c_last_error_tick
      && file == c_input_filename)
    {
      fprintf (stdout, "In file included");
      for (p = c_input_file_stack->NEXT; p; p = p->NEXT)
        {
          fprintf (stdout, " from %s:%d", p->NAME, p->LINE);
          if (p->NEXT)
            fprintf (stdout, ",\n                ");
        }
      fprintf (stdout, ":\n");
      c_last_error_tick = c_input_file_stack_tick;
    }
}

/* Print a message.  */

static void
c_vmessage(prefix, s, ap)
     char *prefix;
     char *s;
     va_list ap;
{
  if (prefix)
    fprintf (stdout, "%s: ", prefix);

#ifdef HAVE_VPRINTF
  vfprintf (stdout, s, ap);
#else
  {
    int v1 = va_arg(ap, int);
    int v2 = va_arg(ap, int);
    int v3 = va_arg(ap, int);
    int v4 = va_arg(ap, int);
    fprintf (stdout, s, v1, v2, v3, v4);
  }
#endif
}

/* Print a message relevant to line LINE of file FILE.  */

static void
c_v_message_with_file_and_line(file, line, prefix, s, ap)
     char *file;
     int line;
     char *prefix;
     char *s;
     va_list ap;
{
  if (file)
    fprintf (stdout, "%s:%d: ", file, line);

  c_vmessage(prefix, s, ap);
  fputc('\n', stdout);
}

/* Print a message relevant to the given DECL.  */

static void
c_v_message_with_decl(decl, prefix, s, ap)
     c_tree_node *decl;
     char *prefix;
     char *s;
     va_list ap;
{
  char *p;

  fprintf (stdout, "%s:%d: ",
           CTreeDeclFileName(decl), CTreeDeclLineNum(decl));

  if (prefix)
    fprintf (stdout, "%s: ", prefix);

  /* Do magic to get around lack of varargs support for insertion
     of arguments into existing list.  We know that the decl is first;
     we ass_u_me that it will be printed with "%s".  */

  for (p = s; *p; ++p)
    {
      if (*p == '%')
        {
          if (*(p + 1) == '%')
            ++p;
          else
            break;
        }
    }

  if (p > s)                        /* Print the left-hand substring.  */
    {
      char fmt[sizeof "%.255s"];
      long width = p - s;
             
      if (width > 255L) width = 255L;        /* arbitrary */
      sprintf (fmt, "%%.%lds", width);
      fprintf (stdout, fmt, s);
    }

  if (*p == '%')                /* Print the name.  */
    {
      char *n = (CTreeDeclName(decl)
                 ? c_print_decl_name(decl, 2)
                 : "((anonymous))");
      fputs (n, stdout);
      while (*p)
        {
          ++p;
          if (isalpha (*(p - 1) & 0xFF))
            break;
        }
    }

  if (*p)                        /* Print the rest of the message.  */
    c_vmessage((char *)NULL, p, ap);

  fputc ('\n', stdout);
}

/* Report an error at line LINE of file FILE.  */

static void
c_v_error_with_file_and_line(file, line, s, ap)
     char *file;
     int line;
     char *s;
     va_list ap;
{
  c_count_error(0);
  c_report_error_function(file);
  c_v_message_with_file_and_line(file, line, (char *)NULL, s, ap);
}

void
c_error_with_file_and_line(char *file, int line, char *s, ...)
{
#ifndef __STDC__
  char *file;
  int line;
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  file = va_arg (ap, char *);
  line = va_arg (ap, int);
  s = va_arg (ap, char *);
#endif

  c_v_error_with_file_and_line (file, line, s, ap);
  va_end (ap);
}

/* Report an error at the declaration DECL.
   S is a format string which uses %s to substitute the declaration
   name; subsequent substitutions are a la printf.  */

static void c_v_error_with_decl (decl, s, ap)
     c_tree_node *decl;
     char *s;
     va_list ap;
{
  c_count_error(0);
  c_report_error_function( CTreeDeclFileName(decl));
  c_v_message_with_decl(decl, (char *)NULL, s, ap);
}

void
c_error_with_decl (tree decl, char *s, ...)
{
#ifndef __STDC__
  c_tree_node *decl;
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  decl = va_arg (ap, tree);
  s = va_arg (ap, char *);
#endif

  c_v_error_with_decl (decl, s, ap);
  va_end (ap);
}

/* Report an error at the line number of the insn INSN.
   This is used only when INSN is an `asm' with operands,
   and each ASM_OPERANDS records its own source file and line.  */

static void
c_verror (s, ap)
     char *s;
     va_list ap;
{
  c_v_error_with_file_and_line(c_input_filename, c_lineno, s, ap);
}

void c_error(char *s, ...)
{
#ifndef __STDC__
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  s = va_arg (ap, char *);
#endif

  c_verror(s, ap);
  va_end (ap);
}

/* Report a fatal error at the current line number.  */

static void
c_vfatal (s, ap)
     char *s;
     va_list ap;
{
  c_verror (s, ap);
  gcpexit( 1 );
}

void
c_fatal (char *s, ...)
{
#ifndef __STDC__
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  s = va_arg (ap, char *);
#endif

  c_vfatal (s, ap);
  va_end (ap);
}

/* Report a warning at line LINE of file FILE.  */

static void
c_v_warning_with_file_and_line (file, line, s, ap)
     char *file;
     int line;
     char *s;
     va_list ap;
{
  if (c_count_error(1))
    {
      c_report_error_function(file);
      c_v_message_with_file_and_line(file, line, "warning", s, ap);
    }
}

void
c_warning_with_file_and_line (char *file, int line, char *s, ...)
{
#ifndef __STDC__
  char *file;
  int line;
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  file = va_arg (ap, char *);
  line = va_arg (ap, int);
  s = va_arg (ap, char *);
#endif

  c_v_warning_with_file_and_line (file, line, s, ap);
  va_end (ap);
}

/* Report a warning at the declaration DECL.
   S is a format string which uses %s to substitute the declaration
   name; subsequent substitutions are a la printf.  */

static void c_v_warning_with_decl (decl, s, ap)
     c_tree_node *decl;
     char *s;
     va_list ap;
{
  if (c_count_error(1))
    {
      c_report_error_function ( CTreeDeclFileName(decl));
      c_v_message_with_decl(decl, "warning", s, ap);
    }
}

void c_warning_with_decl (tree decl, char *s, ...)
{
#ifndef __STDC__
  c_tree_node *decl;
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  decl = va_arg (ap, tree);
  s = va_arg (ap, char *);
#endif

  c_v_warning_with_decl (decl, s, ap);
  va_end (ap);
}

/* Report a warning at the current line number.  */

static void
c_vwarning (s, ap)
     char *s;
     va_list ap;
{
  c_v_warning_with_file_and_line(c_input_filename, c_lineno, s, ap);
}

void
c_warning (char *s, ...)
{
#ifndef __STDC__
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  s = va_arg (ap, char *);
#endif

  c_vwarning(s, ap);
  va_end (ap);
}

/* These functions issue either warnings or errors depending on
   -pedantic-errors.  */

static void
c_vpedwarn (s, ap)
     char *s;
     va_list ap;
{
  if (c_flag_pedantic_errors)
    c_verror (s, ap);
  else
    c_vwarning (s, ap);
}

void c_pedwarn (char *s, ...)
{
#ifndef __STDC__
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  s = va_arg (ap, char *);
#endif

  c_vpedwarn (s, ap);
  va_end (ap);
}

static void
c_v_pedwarn_with_decl(decl, s, ap)
     c_tree_node *decl;
     char *s;
     va_list ap;
{
  /* We don't want -pedantic-errors to cause the compilation to fail from
     "errors" in system header files.  Sometimes fixincludes can't fix what's
     broken (eg: unsigned char bitfields - fixing it may change the alignment
     which will cause programs to mysteriously fail because the C library
     or kernel uses the original layout).  There's no point in issuing a
     warning either, it's just unnecessary noise.  */

  if (! CIsTreeDeclSystemHeader(decl))
    {
      if (c_flag_pedantic_errors)
        c_v_error_with_decl(decl, s, ap);
      else
        c_v_warning_with_decl(decl, s, ap);
    }
}

void
c_pedwarn_with_decl (tree decl, char *s, ...)
{
#ifndef __STDC__
  c_tree_node *decl;
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  decl = va_arg (ap, tree);
  s = va_arg (ap, char *);
#endif

  c_v_pedwarn_with_decl (decl, s, ap);
  va_end (ap);
}

static void
c_v_pedwarn_with_file_and_line (file, line, s, ap)
     char *file;
     int line;
     char *s;
     va_list ap;
{
  if (c_flag_pedantic_errors)
    c_v_error_with_file_and_line (file, line, s, ap);
  else
    c_v_warning_with_file_and_line (file, line, s, ap);
}

void
c_pedwarn_with_file_and_line (char *file, int line, char *s, ...)
{
#ifndef __STDC__
  char *file;
  int line;
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  file = va_arg (ap, char *);
  line = va_arg (ap, int);
  s = va_arg (ap, char *);
#endif

  c_v_pedwarn_with_file_and_line (file, line, s, ap);
  va_end (ap);
}

/* Apologize for not implementing some feature.  */

static void c_vsorry(s, ap)
     char *s;
     va_list ap;
{
  c_sorrycount++;
  if (c_input_filename)
    fprintf (stdout, "%s:%d: ", c_input_filename, c_lineno);
  else
  c_vmessage ("sorry, not implemented", s, ap);
  fputc ('\n', stdout);
}

void c_sorry(char *s, ...)
{
#ifndef __STDC__
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  s = va_arg (ap, char *);
#endif

  c_vsorry (s, ap);
  va_end (ap);
}

/* Apologize for not implementing some feature, then quit.  */

static void
c_v_really_sorry (s, ap)
     char *s;
     va_list ap;
{
  c_sorrycount++;
  if (c_input_filename)
    fprintf (stdout, "%s:%d: ", c_input_filename, c_lineno);
  else
  c_vmessage ("sorry, not implemented", s, ap);
  c_fatal (" (fatal)\n");
}

void c_really_sorry (char *s, ...)
{
#ifndef __STDC__
  char *s;
#endif
  va_list ap;

  VA_START (ap, s);

#ifndef __STDC__
  s = va_arg (ap, char *);
#endif

  c_v_really_sorry (s, ap);
  va_end (ap);
}
