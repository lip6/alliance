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
/* Lexical analyzer for C and Objective C.
   Copyright (C) 1987, 88, 89, 92, 94-97, 1998 Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

# include <stdio.h>
# include <errno.h>
# include <setjmp.h>
# include <malloc.h>
# include <string.h>
# include <math.h>
# include <stdlib.h>

# include "mut.h"
# include "aut.h"
# include "gcp.h"

# include "c_flags.h"
# include "c_tree.h"
# include "c_treecompat.h"
# include "c_clex.h"
# include "c_cdecl.h"
# include "c_ccomp.h"
# include "c_convert.h"
# include "c_ctypeck.h"
# include "c_debug.h"
# include "c_parse.h"

#include <ctype.h>

/* MULTIBYTE_CHARS support only works for native compilers.
   ??? Ideally what we want is to model widechar support after
   the current floating point support.  */
#ifdef CROSS_COMPILE
#undef MULTIBYTE_CHARS
#endif

#ifdef MULTIBYTE_CHARS
#include <stdlib.h>
#include <locale.h>
#endif

#ifndef errno
extern int errno;
#endif

#if USE_CPPLIB
#include "cpplib.h"
cpp_reader parse_in;
cpp_options parse_options;
static enum cpp_token cpp_token;
#endif

/* The elements of `c_ridpointers' are identifier nodes
   for the reserved type names and storage classes.
   It is indexed by a RID_... value.  */

c_tree_node *c_ridpointers[(int) RID_MAX];
FILE *c_y_in  = (FILE *)0;
int c_lineno;

/* Cause the `c_y_debug' variable to be defined.  */
#define C_Y_DEBUG 1

#if USE_CPPLIB
static unsigned char *c_y__cur, *c_y__lim;

int
c_y__get_token ()
{
  for (;;)
    {
      parse_in.limit = parse_in.token_buffer;
      cpp_token = cpp_get_token (&parse_in);
      if (cpp_token == CPP_EOF)
        return -1;
      c_y__lim = CPP_PWRITTEN (&parse_in);
      c_y__cur = parse_in.token_buffer;
      if (c_y__cur < c_y__lim)
        return *c_y__cur++;
    }
}

#define GETC() (c_y__cur < c_y__lim ? *c_y__cur++ : c_y__get_token ())
#define UNGETC(c) ((c), c_y__cur--)
#else
#define GETC() getc (c_y_in)
#define UNGETC(c) ungetc (c, c_y_in)
#endif

/* the declaration found for the last IDENTIFIER token read in.
   c_y_lex must look this up to detect typedefs, which get token type TYPENAME,
   so it is left around in case the identifier is not a typedef but is
   used in a context which makes it a reference to a variable.  */
c_tree_node *c_lastiddecl;

/* Nonzero enables objc features.  */

# ifdef LUDO_DONE
extern tree c_is_class_name ();
# else
# define c_is_class_name(N)    0
# endif

extern int c_y_debug;

/* Number of bytes in a wide character.  */

static int maxtoken;                /* Current nominal length of token buffer.  */
char *token_buffer;        /* Pointer to token buffer.
                           Actual allocated length is maxtoken + 2.
                           This is not static because objc-parse.y uses it.  */

static int indent_level = 0;        /* Number of { minus number of }. */

/* Nonzero if end-of-file has been seen on input.  */
static int end_of_file;

# ifdef LUDO_DONE
#if !USE_CPPLIB
/* Buffered-back input character; faster than using ungetc.  */
static int nextchar = -1;
#endif
# else
#if !USE_CPPLIB
/* Buffered-back input character; faster than using ungetc.  */
static int nextchar = '\n';
#endif
# endif


int check_newline ();

/* Do not insert generated code into the source, instead, include it.
   This allows us to build gcc automatically even for targets that
   need to add or modify the reserved keyword lists.  */
#include "c_gperf.h"

/* Return something to represent absolute declarators containing a *.
   TARGET is the absolute declarator that the * contains.
   TYPE_QUALS is a list of modifiers such as const or volatile
   to apply to the pointer type, represented as identifiers.

   We return an INDIRECT_REF whose "contents" are TARGET
   and whose type is the modifier list.  */

# ifdef LUDO_DONE
tree
make_pointer_declarator (type_quals, target)
     tree type_quals, target;
{
  return build1 (INDIRECT_REF, type_quals, target);
}
# endif

static void forget_protocol_qualifiers ()
{
  int i, n = sizeof wordlist / sizeof (struct resword);

  for (i = 0; i < n; i++)
    if ((int) wordlist[i].rid >= (int) RID_IN
        && (int) wordlist[i].rid <= (int) RID_ONEWAY)
      wordlist[i].name = "";
}

# ifdef LUDO_NO_SKIP
static void remember_protocol_qualifiers ()
{
  int i, n = sizeof wordlist / sizeof (struct resword);

  for (i = 0; i < n; i++)
    if (wordlist[i].rid == RID_IN)
      wordlist[i].name = "in";
    else if (wordlist[i].rid == RID_OUT)
      wordlist[i].name = "out";
    else if (wordlist[i].rid == RID_INOUT)
      wordlist[i].name = "inout";
    else if (wordlist[i].rid == RID_BYCOPY)
      wordlist[i].name = "bycopy";
    else if (wordlist[i].rid == RID_ONEWAY)
      wordlist[i].name = "oneway";   
}
# endif

#if USE_CPPLIB
void
init_parse (filename)
     char *filename;
{
  init_lex ();
  c_y__cur = "\n";
  c_y__lim = c_y__cur+1;

  cpp_reader_init (&parse_in);
  parse_in.data = &parse_options;
  cpp_options_init (&parse_options);
  cpp_handle_options (&parse_in, 0, NULL); /* FIXME */
  parse_in.show_column = 1;
  if (! cpp_start_read (&parse_in, filename))
    abort ();
}

void
finish_parse ()
{
  cpp_finish (&parse_in);
}
#endif

void
c_init_lex ( InputFileName )

   char *InputFileName;
{
  char *buffer;

  c_input_file_stack = 
     (struct c_file_stack *)autallocblock(sizeof (struct c_file_stack));

  buffer = autallocblock( strlen( InputFileName ) + 3 );
  sprintf( buffer, "\"%s\"", InputFileName );

  c_input_file_stack->NAME = autnamealloc( buffer );
  c_input_filename = c_input_file_stack->NAME;

  autfreeblock( buffer);

  /* Make identifier nodes long enough for the language-specific slots.  */
# ifdef LUDO_DONE
  set_identifier_size(sizeof (struct lang_identifier));
# endif

  /* Start it at 0, because check_newline is called at the very beginning
     and will increment it to 1.  */
  c_lineno = 0;

#ifdef MULTIBYTE_CHARS
  /* Change to the native locale for multibyte conversions.  */
  setlocale (LC_CTYPE, "");
#endif

  c_lastiddecl = 0;
  indent_level = 0;
  end_of_file = 0;

# ifdef LUDO_DONE
#if !USE_CPPLIB
/* Buffered-back input character; faster than using ungetc.  */
   nextchar = -1;
#endif
# else
#if !USE_CPPLIB
/* Buffered-back input character; faster than using ungetc.  */
   nextchar = '\n';
#endif
# endif

  maxtoken     = 40;
  token_buffer = (char *)autallocblock( maxtoken + 2);

  c_ridpointers[(int) RID_INT] = c_tree_get_identifier ("int");
  c_ridpointers[(int) RID_CHAR] = c_tree_get_identifier ("char");
  c_ridpointers[(int) RID_VOID] = c_tree_get_identifier ("void");
  c_ridpointers[(int) RID_FLOAT] = c_tree_get_identifier ("float");
  c_ridpointers[(int) RID_DOUBLE] = c_tree_get_identifier ("double");
  c_ridpointers[(int) RID_SHORT] = c_tree_get_identifier ("short");
  c_ridpointers[(int) RID_LONG] = c_tree_get_identifier ("long");
  c_ridpointers[(int) RID_UNSIGNED] = c_tree_get_identifier ("unsigned");
  c_ridpointers[(int) RID_SIGNED] = c_tree_get_identifier ("signed");
  c_ridpointers[(int) RID_INLINE] = c_tree_get_identifier ("inline");
  c_ridpointers[(int) RID_CONST] = c_tree_get_identifier ("const");
  c_ridpointers[(int) RID_VOLATILE] = c_tree_get_identifier ("volatile");
  c_ridpointers[(int) RID_AUTO] = c_tree_get_identifier ("auto");
  c_ridpointers[(int) RID_STATIC] = c_tree_get_identifier ("static");
  c_ridpointers[(int) RID_EXTERN] = c_tree_get_identifier ("extern");
  c_ridpointers[(int) RID_TYPEDEF] = c_tree_get_identifier ("typedef");
  c_ridpointers[(int) RID_REGISTER] = c_tree_get_identifier ("register");
  c_ridpointers[(int) RID_ITERATOR] = c_tree_get_identifier ("iterator");
  c_ridpointers[(int) RID_COMPLEX] = c_tree_get_identifier ("complex");
  c_ridpointers[(int) RID_ID] = c_tree_get_identifier ("id");
  c_ridpointers[(int) RID_IN] = c_tree_get_identifier ("in");
  c_ridpointers[(int) RID_OUT] = c_tree_get_identifier ("out");
  c_ridpointers[(int) RID_INOUT] = c_tree_get_identifier ("inout");
  c_ridpointers[(int) RID_BYCOPY] = c_tree_get_identifier ("bycopy");
  c_ridpointers[(int) RID_ONEWAY] = c_tree_get_identifier ("oneway");
  forget_protocol_qualifiers();

  /* Some options inhibit certain reserved words.
     Clear those words out of the hash table so they won't be recognized.  */
#define UNSET_RESERVED_WORD(STRING) \
  do { struct resword *s = c_is_reserved_word (STRING, sizeof (STRING) - 1); \
       if (s) s->name = ""; } while (0)

  if (! c_doing_objc_thang)
    UNSET_RESERVED_WORD ("id");

  if (c_flag_traditional)
    {
      UNSET_RESERVED_WORD ("const");
      UNSET_RESERVED_WORD ("volatile");
      UNSET_RESERVED_WORD ("typeof");
      UNSET_RESERVED_WORD ("signed");
      UNSET_RESERVED_WORD ("inline");
      UNSET_RESERVED_WORD ("iterator");
      UNSET_RESERVED_WORD ("complex");
    }
  if (c_flag_no_asm)
    {
      UNSET_RESERVED_WORD ("asm");
      UNSET_RESERVED_WORD ("typeof");
      UNSET_RESERVED_WORD ("inline");
      UNSET_RESERVED_WORD ("iterator");
      UNSET_RESERVED_WORD ("complex");
    }
}

void
c_reinit_parse_for_function ()
{
}

/* Function used when c_y_debug is set, to print a token in more detail.  */

void
c_y_print (file, c_y_char, c_y_lval)
     FILE *file;
     int c_y_char;
     C_Y_STYPE c_y_lval;
{
  /*
  if ( c_y_char >= tok_IDENTIFIER )
  {
    fprintf( stdout, "\n" );
    c_fprintf( stdout, "token %d\n", c_y_char );
    loc_c_tree_view_node( c_y_lval.ttype );
  }
  */
  /*
  switch (c_y_char)
  {
    case tok_IDENTIFIER:
    case tok_TYPENAME:
    case tok_OBJECTNAME:
    case tok_CONSTANT:
      loc_c_tree_view_node( c_y_lval.ttype );
    break;

    case tok_SCSPEC     :
    case tok_TYPESPEC   :
    case tok_TYPE_QUAL  :
    case tok_STRING     :
    case tok_ELLIPSIS   :
    case tok_SIZEOF     :
    case tok_ENUM       :
    case tok_STRUCT     :
    case tok_UNION      :
    case tok_IF :
    case tok_ELSE       :
    case tok_WHILE      :
    case tok_DO :
    case tok_FOR :
    case tok_SWITCH     :
    case tok_CASE       :
    case tok_DEFAULT    :
    case tok_BREAK      :
    case tok_CONTINUE   :
    case tok_RETURN     :
    case tok_GOTO       :
    case tok_ASM_KEYWORD :
    case tok_TYPEOF      :
    case tok_ALIGNOF     :
    case tok_ATTRIBUTE   :
    case tok_EXTENSION   :
    case tok_LABEL       :
    case tok_REALPART    :
    case tok_IMAGPART    :
    case tok_ASSIGN      :
    case tok_OROR        :
    case tok_ANDAND      :
    case tok_EQCOMPARE   :
    case tok_ARITHCOMPARE :
    case tok_LSHIFT :
    case tok_RSHIFT :
    case tok_UNARY :
    case tok_PLUSPLUS :
    case tok_MINUSMINUS :
    case tok_HYPERUNARY :
    case tok_POINTSAT :
    case tok_INTERFACE :
    case tok_IMPLEMENTATION :
    case tok_END :
    case tok_SELECTOR :
    case tok_DEFS :
    case tok_ENCODE :
    case tok_CLASSNAME :
    case tok_PUBLIC :
    case tok_PRIVATE :
    case tok_PROTECTED :
    case tok_PROTOCOL :
    case tok_CLASS :
    case tok_ALIAS :
    case tok_OBJC_STRING :
    break;
  }
  */
}

/* If C is not whitespace, return C.
   Otherwise skip whitespace and return first nonwhite char read.  */

static int
skip_white_space (c)
     register int c;
{
  static int newline_warning = 0;

  for (;;)
    {
      switch (c)
        {
          /* We don't recognize comments here, because
             cpp output can include / and * consecutively as operators.
             Also, there's no need, since cpp removes all comments.  */

        case '\n':
          c = check_newline ();
          break;

        case ' ':
        case '\t':
        case '\f':
        case '\v':
        case '\b':
          c = GETC();
          break;

        case '\r':
          /* ANSI C says the effects of a carriage return in a source file
             are undefined.  */
          if (c_pedantic && !newline_warning)
            {
              warning ("carriage return in source file");
              warning ("(we only warn about the first carriage return)");
              newline_warning = 1;
            }
          c = GETC();
          break;

        case '\\':
          c = GETC();
          if (c == '\n')
            c_lineno++;
          else
          {
            error ("stray '\\' in program");
          }
          c = GETC();
          break;

        default:
          return (c);
        }
    }
}

/* Skips all of the white space at the current location in the input file.
   Must use and reset nextchar if it has the next character.  */

void c_position_after_white_space ()
{
  register int c;

#if !USE_CPPLIB
  if (nextchar != -1)
    c = nextchar, nextchar = -1;
  else
#endif
    c = GETC();

  UNGETC (skip_white_space (c));
}

/* Make the token buffer longer, preserving the data in it.
   P should point to just beyond the last valid character in the old buffer.
   The value we return is a pointer to the new buffer
   at a place corresponding to P.  */

static char *
extend_token_buffer (p)
     char *p;
{
  int offset = p - token_buffer;

  maxtoken     = maxtoken * 2 + 10;
  token_buffer = (char *)realloc( token_buffer, maxtoken + 2 );

  return token_buffer + offset;
}

#if !USE_CPPLIB
#define GET_DIRECTIVE_LINE() get_directive_line (c_y_in)
#else /* USE_CPPLIB */
/* Read the rest of a #-directive from input stream FINPUT.
   In normal use, the directive name and the white space after it
   have already been read, so they won't be included in the result.
   We allow for the fact that the directive line may contain
   a newline embedded within a character or string literal which forms
   a part of the directive.

   The value is a string in a reusable buffer.  It remains valid
   only until the next time this function is called.  */

static char *
GET_DIRECTIVE_LINE ()
{
  static char *directive_buffer = NULL;
  static unsigned buffer_length = 0;
  register char *p;
  register char *buffer_limit;
  register int looking_for = 0;
  register int char_escaped = 0;

  if (buffer_length == 0)
    {
      directive_buffer = (char *)xmalloc (128);
      buffer_length = 128;
    }

  buffer_limit = &directive_buffer[buffer_length];

  for (p = directive_buffer; ; )
    {
      int c;

      /* Make buffer bigger if it is full.  */
      if (p >= buffer_limit)
        {
          register unsigned bytes_used = (p - directive_buffer);

          buffer_length *= 2;
          directive_buffer
            = (char *)xrealloc (directive_buffer, buffer_length);
          p = &directive_buffer[bytes_used];
          buffer_limit = &directive_buffer[buffer_length];
        }

      c = GETC ();

      /* Discard initial whitespace.  */
      if ((c == ' ' || c == '\t') && p == directive_buffer)
        continue;

      /* Detect the end of the directive.  */
      if (c == '\n' && looking_for == 0)
        {
          UNGETC (c);
          c = '\0';
        }

      *p++ = c;

      if (c == 0)
        return directive_buffer;

      /* Handle string and character constant syntax.  */
      if (looking_for)
        {
          if (looking_for == c && !char_escaped)
            looking_for = 0;        /* Found terminator... stop looking.  */
        }
      else
        if (c == '\'' || c == '"')
          looking_for = c;        /* Don't stop buffering until we see another
                                   another one of these (or an EOF).  */

      /* Handle backslash.  */
      char_escaped = (c == '\\' && ! char_escaped);
    }
}
#endif /* USE_CPPLIB */

/* At the beginning of a line, increment the line number
   and process any #-directive on this line.
   If the line is a #-directive, read the entire line and return a newline.
   Otherwise, return the line's first non-whitespace character.  */

int
check_newline ()
{
  register int c;
  register int token;

  c_lineno++;

  /* Read first nonwhite char on the line.  */

  c = GETC();
  while (c == ' ' || c == '\t')
    c = GETC();

  if (c != '#')
    {
      /* If not #, return it so caller will use it.  */
      return c;
    }

  /* Read first nonwhite char after the `#'.  */

  c = GETC();
  while (c == ' ' || c == '\t')
    c = GETC();

  /* If a letter follows, then if the word here is `line', skip
     it and ignore it; otherwise, ignore the line, with an error
     if the word isn't `pragma', `ident', `define', or `undef'.  */

  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
    {
      if (c == 'p')
        {
          if (GETC() == 'r'
              && GETC() == 'a'
              && GETC() == 'g'
              && GETC() == 'm'
              && GETC() == 'a'
              && ((c = GETC()) == ' ' || c == '\t' || c == '\n'))
            {
              while (c == ' ' || c == '\t')
                c = GETC ();
              if (c == '\n')
                return c;
#ifdef HANDLE_SYSV_PRAGMA
              UNGETC (c);
              token = c_y_lex ();
              if (token != IDENTIFIER)
                goto skipline;
              return handle_sysv_pragma (token);
#else /* !HANDLE_SYSV_PRAGMA */
#ifdef HANDLE_PRAGMA
#if !USE_CPPLIB
              UNGETC (c);
              token = c_y_lex ();
              if (token != IDENTIFIER)
                goto skipline;
              if (HANDLE_PRAGMA (c_y_in, c_y_lval.ttype))
                {
                  c = GETC ();
                  return c;
                }
#else
              ??? do not know what to do ???;
#endif /* !USE_CPPLIB */
#endif /* HANDLE_PRAGMA */
#endif /* !HANDLE_SYSV_PRAGMA */
              goto skipline;
            }
        }

      else if (c == 'd')
        {
          if (GETC() == 'e'
              && GETC() == 'f'
              && GETC() == 'i'
              && GETC() == 'n'
              && GETC() == 'e'
              && ((c = GETC()) == ' ' || c == '\t' || c == '\n'))
            {
              if (c != '\n')
              {
# ifdef LUDO_DONE
                debug_define (c_lineno, GET_DIRECTIVE_LINE ());
# endif
              }
              goto skipline;
            }
        }
      else if (c == 'u')
        {
          if (GETC() == 'n'
              && GETC() == 'd'
              && GETC() == 'e'
              && GETC() == 'f'
              && ((c = GETC()) == ' ' || c == '\t' || c == '\n'))
            {
              if (c != '\n')
              {
# ifdef LUDO_DONE
                debug_undef (c_lineno, GET_DIRECTIVE_LINE ());
# endif
              }
              goto skipline;
            }
        }
      else if (c == 'l')
        {
          if (GETC() == 'i'
              && GETC() == 'n'
              && GETC() == 'e'
              && ((c = GETC()) == ' ' || c == '\t'))
            goto linenum;
        }
      else if (c == 'i')
        {
          if (GETC() == 'd'
              && GETC() == 'e'
              && GETC() == 'n'
              && GETC() == 't'
              && ((c = GETC()) == ' ' || c == '\t'))
            {
              /* #ident.  The c_pedantic warning is now in cccp.c.  */

              /* Here we have just seen `#ident '.
                 A string constant should follow.  */

              while (c == ' ' || c == '\t')
                c = GETC();

              /* If no argument, ignore the line.  */
              if (c == '\n')
                return c;

              UNGETC (c);
              token = c_y_lex ();
              if (( token != tok_STRING ) || 
                  ( CTreeNodeFirstCode( c_y_lval.ttype ) != C_STRING_NODE ))
                {
                  error ("invalid #ident");
                  goto skipline;
                }
              if (!c_flag_no_ident)
                {
#ifdef ASM_OUTPUT_IDENT
                  ASM_OUTPUT_IDENT (asm_out_file, TREE_STRING_POINTER (c_y_lval.ttype));
#endif
                }

              /* Skip the rest of this line.  */
              goto skipline;
            }
        }

      error ("undefined or invalid # directive");
      goto skipline;
    }

linenum:
  /* Here we have either `#line' or `# <nonletter>'.
     In either case, it should be a line number; a digit should follow.  */

  while (c == ' ' || c == '\t')
    c = GETC();

  /* If the # is the only nonwhite char on the line,
     just ignore it.  Check the new newline.  */
  if (c == '\n')
    return c;

  /* Something follows the #; read a token.  */

  UNGETC (c);
  token = c_y_lex ();

  if ((token == tok_CONSTANT) && 
      (CTreeNodeFirstCode( c_y_lval.ttype ) == C_INT_CST_NODE))
  {
      int old_lineno = c_lineno;
      int used_up = 0;
      /* subtract one, because it is the following line that
         gets the specified number */

      int l = CTreeIntCstLow(c_y_lval.ttype) - 1;

      /* Is this the last nonwhite stuff on the line?  */
      c = GETC();
      while (c == ' ' || c == '\t')
        c = GETC();
      if (c == '\n')
        {
          /* No more: store the line number and check following line.  */
          c_lineno = l;
          return c;
        }
      UNGETC (c);

      /* More follows: it must be a string constant (filename).  */

      /* Read the string constant.  */
      token = c_y_lex ();

      if ((token != tok_STRING) || 
          ( CTreeNodeFirstCode( c_y_lval.ttype ) != C_STRING_NODE))
        {
          error ("invalid #line");
          goto skipline;
        }

      c_input_filename
        = (char *) autallocblock( CTreeStringLength(c_y_lval.ttype) + 1);
      strcpy (c_input_filename, CTreeStringPointer(c_y_lval.ttype));
      c_lineno = l;

      /* Each change of file name
         reinitializes whether we are now in a system header.  */
      c_in_system_header = 0;

      if (c_main_input_filename == 0)
        c_main_input_filename = c_input_filename;

      /* Is this the last nonwhite stuff on the line?  */
      c = GETC();
      while (c == ' ' || c == '\t')
        c = GETC();
      if (c == '\n')
        {
          /* Update the name in the top element of input_file_stack.  */
          if (c_input_file_stack)
            c_input_file_stack->NAME = c_input_filename;

          return c;
        }
      UNGETC (c);

      token = c_y_lex ();
      used_up = 0;

      /* `1' after file name means entering new file.
         `2' after file name means just left a file.  */

      if ((token == tok_CONSTANT)
          && (CTreeNodeFirstCode(c_y_lval.ttype ) == C_INT_CST_NODE ))
        {
          if (CTreeIntCstLow(c_y_lval.ttype) == 1)
            {
              /* Pushing to a new file.  */
              struct c_file_stack *p
                = (struct c_file_stack *) autallocblock(sizeof (struct c_file_stack));
              c_input_file_stack->LINE = old_lineno;
              p->NEXT = c_input_file_stack;
              p->NAME = c_input_filename;
              p->INDENT_LEVEL = indent_level;
              c_input_file_stack = p;
              c_input_file_stack_tick++;
# ifdef LUDO_DONE
              debug_start_source_file (c_input_filename);
# endif
              used_up = 1;
            }
          else if (CTreeIntCstLow(c_y_lval.ttype) == 2)
            {
              /* Popping out of a file.  */
              if (c_input_file_stack->NEXT)
                {
                  struct c_file_stack *p = c_input_file_stack;
                  if (indent_level != p->INDENT_LEVEL)
                    {
                      c_warning_with_file_and_line( p->NAME, old_lineno,
                         "This file contains more `%c's than `%c's.",
                         indent_level > p->INDENT_LEVEL ? '{' : '}',
                         indent_level > p->INDENT_LEVEL ? '}' : '{');
                    }
                  c_input_file_stack = p->NEXT;
                  free (p);
                  c_input_file_stack_tick++;
# ifdef LUDO_DONE
                  debug_end_source_file (c_input_file_stack->line);
# endif
                }
              else
              {
                error ("#-lines for entering and leaving files don't match");
              }

              used_up = 1;
            }
        }

      /* Now that we've pushed or popped the input stack,
         update the name in the top element.  */
      if (c_input_file_stack)
        c_input_file_stack->NAME = c_input_filename;

      /* If we have handled a `1' or a `2',
         see if there is another number to read.  */
      if (used_up)
        {
          /* Is this the last nonwhite stuff on the line?  */
          c = GETC();
          while (c == ' ' || c == '\t')
            c = GETC();
          if (c == '\n')
            return c;
          UNGETC (c);

          token = c_y_lex ();
          used_up = 0;
        }

      /* `3' after file name means this is a system header file.  */

      if (( token == tok_CONSTANT )
          && ( CTreeNodeFirstCode(c_y_lval.ttype) == C_INT_CST_NODE )
          && ( CTreeIntCstLow( c_y_lval.ttype) == 3 ))
        c_in_system_header = 1, used_up = 1;

      if (used_up)
        {
          /* Is this the last nonwhite stuff on the line?  */
          c = GETC();
          while (c == ' ' || c == '\t')
            c = GETC();
          if (c == '\n')
            return c;
          UNGETC (c);
        }

      warning ("unrecognized text at end of #line");
  }
  else
  {
    error ("invalid #-line");
  }

  /* skip the rest of this line.  */
 skipline:
#if !USE_CPPLIB
  if (c != '\n' && c != EOF && nextchar >= 0)
    c = nextchar, nextchar = -1;
#endif
  while (c != '\n' && c != EOF)
    c = GETC();
  return c;
}

#ifdef HANDLE_SYSV_PRAGMA

/* Handle a #pragma directive.
   TOKEN is the token we read after `#pragma'.  Processes the entire input
   line and returns a character for the caller to reread: either \n or EOF.  */

/* This function has to be in this file, in order to get at
   the token types.  */

int
handle_sysv_pragma (token)
     register int token;
{
  register int c;

  for (;;)
    {
      switch (token)
        {
        case tok_IDENTIFIER:
        case tok_TYPENAME:
        case tok_STRING:
        case tok_CONSTANT:
          handle_pragma_token (token_buffer, c_y_lval.ttype);
          break;
        default:
          handle_pragma_token (token_buffer, 0);
        }
#if !USE_CPPLIB
      if (nextchar >= 0)
        c = nextchar, nextchar = -1;
      else
#endif
        c = GETC ();

      while (c == ' ' || c == '\t')
        c = GETC ();
      if (c == '\n' || c == EOF)
        {
          handle_pragma_token (0, 0);
          return c;
        }
      UNGETC (c);
      token = c_y_lex ();
    }
}

#endif /* HANDLE_SYSV_PRAGMA */

#define ENDFILE -1  /* token that represents end-of-file */

/* Read an escape sequence, returning its equivalent as a character,
   or store 1 in *ignore_ptr if it is backslash-newline.  */

static int
readescape (ignore_ptr)
     int *ignore_ptr;
{
  register int c = GETC();
  register int code;
  register unsigned count;
  unsigned firstdig = 0;
  int nonnull;

  switch (c)
    {
    case 'x':
      if (c_warn_traditional)
      {
        warning ("the meaning of `\\x' varies with -traditional");
      }

      if (c_flag_traditional)
        return c;

      code = 0;
      count = 0;
      nonnull = 0;
      while (1)
        {
          c = GETC();
          if (!(c >= 'a' && c <= 'f')
              && !(c >= 'A' && c <= 'F')
              && !(c >= '0' && c <= '9'))
            {
              UNGETC (c);
              break;
            }
          code *= 16;
          if (c >= 'a' && c <= 'f')
            code += c - 'a' + 10;
          if (c >= 'A' && c <= 'F')
            code += c - 'A' + 10;
          if (c >= '0' && c <= '9')
            code += c - '0';
          if (code != 0 || count != 0)
            {
              if (count == 0)
                firstdig = code;
              count++;
            }
          nonnull = 1;
        }
      if (! nonnull)
      {
        error ("\\x used with no following hex digits");
      }
      else if (count == 0)
        /* Digits are all 0's.  Ok.  */
        ;
      else 
        if ((count - 1) * 4 >= CTreeTypePrecision( c_integer_type_node)
               || (count > 1
                   && ((1 << (CTreeTypePrecision(c_integer_type_node) - (count - 1)*4))
                       <= firstdig)))
        {
          pedwarn ("hex escape out of range");
        }
      return code;

    case '0':  case '1':  case '2':  case '3':  case '4':
    case '5':  case '6':  case '7':
      code = 0;
      count = 0;
      while ((c <= '7') && (c >= '0') && (count++ < 3))
        {
          code = (code * 8) + (c - '0');
          c = GETC();
        }
      UNGETC (c);
      return code;

    case '\\': case '\'': case '"':
      return c;

    case '\n':
      c_lineno++;
      *ignore_ptr = 1;
      return 0;

    case 'n':
      return '\n'; /* TARGET_NEWLINE */

    case 't':
      return '\t'; /* TARGET_TAB */

    case 'r':
      return '\r'; /* TARGET_CR */

    case 'f':
      return '\f'; /* TARGET_FF */

    case 'b':
      return '\b'; /* TARGET_BS */

    case 'a':
      if (c_warn_traditional)
      {
        warning ("the meaning of `\\a' varies with -traditional");
      }

      if (c_flag_traditional)
        return c;

      return '\a'; /* TARGET_BELL */

    case 'v':
#if 0 /* Vertical tab is present in common usage compilers.  */
      if (c_flag_traditional)
        return c;
#endif
      return '\v'; /* TARGET_VT */

    case 'e':
    case 'E':
      if (c_pedantic)
      {
        pedwarn ("non-ANSI-standard escape sequence, `\\%c'", c);
      }
      return 033;

    case '?':
      return c;

      /* `\(', etc, are used at beginning of line to avoid confusing Emacs.  */
    case '(':
    case '{':
    case '[':
      /* `\%' is used to prevent SCCS from getting confused.  */
    case '%':
      if (c_pedantic)
      {
        pedwarn ("non-ANSI escape sequence `\\%c'", c);
      }
      return c;
    }
  if (c >= 040 && c < 0177)
  {
    pedwarn ("unknown escape sequence `\\%c'", c);
  }
  else
  {
    pedwarn ("unknown escape sequence: `\\' followed by char code 0x%x", c);
  }
  return c;
}

void
c_y_error (string)
     char *string;
{
  char buf[200];

  strcpy (buf, string);

  /* We can't print string and character constants well
     because the token_buffer contains the result of processing escapes.  */
  if (end_of_file)
    strcat (buf, " at end of input");
  else if (token_buffer[0] == 0)
    strcat (buf, " at null character");
  else if (token_buffer[0] == '"')
    strcat (buf, " before string constant");
  else if (token_buffer[0] == '\'')
    strcat (buf, " before character constant");
  else if (token_buffer[0] < 040 || (unsigned char) token_buffer[0] >= 0177)
    sprintf (buf + strlen (buf), " before character 0%o",
             (unsigned char) token_buffer[0]);
  else
    strcat (buf, " before `%s'");

  /* fprintf( stdout, "Line %d, File %s ::", c_lineno, c_input_filename ); */
  error (buf, token_buffer);
  gcpexit(1);
}

#if 0

struct try_type
{
  tree *node_var;
  char unsigned_flag;
  char long_flag;
  char long_long_flag;
};

struct try_type type_sequence[] = 
{
  { &integer_type_node, 0, 0, 0},
  { &unsigned_type_node, 1, 0, 0},
  { &long_integer_type_node, 0, 1, 0},
  { &long_unsigned_type_node, 1, 1, 0},
  { &long_long_integer_type_node, 0, 1, 1},
  { &long_long_unsigned_type_node, 1, 1, 1}
};
#endif /* 0 */

int
c_y_lex ()
{
  register int c;
  register char *p;
  register int value;
  int wide_flag = 0;
  int objc_flag = 0;

#if !USE_CPPLIB
  if (nextchar >= 0)
    c = nextchar, nextchar = -1;
  else
#endif
    c = GETC();

  /* Effectively do c = skip_white_space (c)
     but do it faster in the usual cases.  */
  while (1)
    switch (c)
      {
      case ' ':
      case '\t':
      case '\f':
      case '\v':
      case '\b':
        c = GETC();
        break;

      case '\r':
        /* Call skip_white_space so we can warn if appropriate.  */

      case '\n':
      case '/':
      case '\\':
        c = skip_white_space (c);
      default:
        goto found_nonwhite;
      }
 found_nonwhite:

  token_buffer[0] = c;
  token_buffer[1] = 0;

/*  c_y_lloc.first_line = lineno; */

  switch (c)
    {
    case EOF:
      end_of_file = 1;
      token_buffer[0] = 0;
      value = ENDFILE;
      break;

    case 'L':
      /* Capital L may start a wide-string or wide-character constant.  */
      {
        register int c = GETC();
        if (c == '\'')
          {
            wide_flag = 1;
            goto char_constant;
          }
        if (c == '"')
          {
            wide_flag = 1;
            goto string_constant;
          }
        UNGETC (c);
      }
      goto letter;

    case '@':
      if (!c_doing_objc_thang)
        {
          value = c;
          break;
        }
      else
        {
          /* '@' may start a constant string object.  */
          register int c = GETC ();
          if (c == '"')
            {
              objc_flag = 1;
              goto string_constant;
            }
          UNGETC (c);
          /* Fall through to treat '@' as the start of an identifier.  */
        }

    case 'A':  case 'B':  case 'C':  case 'D':  case 'E':
    case 'F':  case 'G':  case 'H':  case 'I':  case 'J':
    case 'K':                  case 'M':  case 'N':  case 'O':
    case 'P':  case 'Q':  case 'R':  case 'S':  case 'T':
    case 'U':  case 'V':  case 'W':  case 'X':  case 'Y':
    case 'Z':
    case 'a':  case 'b':  case 'c':  case 'd':  case 'e':
    case 'f':  case 'g':  case 'h':  case 'i':  case 'j':
    case 'k':  case 'l':  case 'm':  case 'n':  case 'o':
    case 'p':  case 'q':  case 'r':  case 's':  case 't':
    case 'u':  case 'v':  case 'w':  case 'x':  case 'y':
    case 'z':
    case '_':
    case '$':
    letter:
      p = token_buffer;
      while (isalnum (c) || c == '_' || c == '$' || c == '@')
        {
          /* Make sure this char really belongs in an identifier.  */
          if (c == '@' && ! c_doing_objc_thang)
            break;
          if (c == '$')
            {
              if (! c_dollars_in_ident)
              {
                error ("`$' in identifier");
              }
              else if (c_pedantic)
              {
                pedwarn ("`$' in identifier");
              }
            }

          if (p >= token_buffer + maxtoken)
            p = extend_token_buffer (p);

          *p++ = c;
          c = GETC();
        }

      *p = 0;
#if USE_CPPLIB
      UNGETC (c);
#else
      nextchar = c;
#endif

      value = tok_IDENTIFIER;
      c_y_lval.itype = 0;

      /* Try to recognize a keyword.  Uses minimum-perfect hash function */

      {
        register struct resword *ptr;

        ptr = c_is_reserved_word (token_buffer, p - token_buffer);
        if ( ptr )
          {
            if (ptr->rid)
              c_y_lval.ttype = c_ridpointers[(int) ptr->rid];
            value = (int) ptr->token;

            /* Only return OBJECTNAME if it is a typedef.  */
            if (c_doing_objc_thang && value == tok_OBJECTNAME)
              {
                c_lastiddecl = c_lookup_name(c_y_lval.ttype);

                if ((c_lastiddecl == (c_tree_node *)0) || 
                    (CTreeDeclCode(c_lastiddecl) != C_TYPE_DECL))
                {
                  value = tok_IDENTIFIER;
                }
              }

            /* Even if we decided to recognize asm, still perhaps warn.  */
            if (c_pedantic
                && (value == tok_ASM_KEYWORD || value == tok_TYPEOF
                    || ptr->rid == RID_INLINE)
                && token_buffer[0] != '_')
            {
              pedwarn ("ANSI does not permit the keyword `%s'",
                       token_buffer);
            }
          }
      }

      /* If we did not find a keyword, look for an identifier
         (or a typename).  */

      if (value == tok_IDENTIFIER)
        {
           if (token_buffer[0] == '@')
          {
            error("invalid identifier `%s'", token_buffer);
          }

          c_y_lval.ttype = c_tree_get_identifier( token_buffer );
          c_lastiddecl   = c_lookup_name( c_y_lval.ttype );

          if ((c_lastiddecl != 0) && (CTreeDeclCode(c_lastiddecl) == C_TYPE_DECL))
          {
            value = tok_TYPENAME;
          }
          /* A user-invisible read-only initialized variable
             should be replaced by its value.
             We handle only strings since that's the only case used in C.  */
          else if ((c_lastiddecl != 0) && 
                   (CTreeDeclCode(c_lastiddecl) == C_VAR_DECL) &&
                   (CIsTreeDeclIgnored(c_lastiddecl)) &&
                   (CIsTreeNodeReadOnly(c_lastiddecl)) &&
                   (CTreeDeclInitial(c_lastiddecl) != (c_tree_node *)0) &&
                   (CTreeNodeFirstCode( CTreeDeclInitial(c_lastiddecl)) == C_STRING_NODE))
            {
              c_tree_node *stringval = CTreeDeclInitial(c_lastiddecl);
              
              /* Copy the string value so that we won't clobber anything
                 if we put something in the TREE_CHAIN of this one.  */
              c_y_lval.ttype = c_tree_build_string(
                                           CTreeStringLength(stringval),
                                           CTreeStringPointer(stringval));
              value = tok_STRING;
            }
# ifdef LUDO_DONE
          else if (c_doing_objc_thang)
            {
              c_tree_node *objc_interface_decl = c_is_class_name (c_y_lval.ttype);

              if (objc_interface_decl)
                {
                  value = CLASSNAME;
                  c_y_lval.ttype = objc_interface_decl;
                }
            }
# endif
        }

      break;

    case '0':  case '1':
      {
        int next_c;
        /* Check first for common special case:  single-digit 0 or 1.  */

        next_c = GETC ();
        UNGETC (next_c);        /* Always undo this lookahead.  */
        if (!isalnum (next_c) && next_c != '.')
          {
            token_buffer[0] = (char)c,  token_buffer[1] = '\0';
            c_y_lval.ttype = (c == '0') ? c_integer_zero_node : c_integer_one_node;
            value = tok_CONSTANT;
            break;
          }
        /*FALLTHRU*/
      }

    case '2':  case '3':  case '4':
    case '5':  case '6':  case '7':  case '8':  case '9':
    case '.':
      {
        int base = 10;
        int count = 0;
        int largest_digit = 0;
        int numdigits = 0;
        /* for multi-precision arithmetic,
           we actually store only C_HOST_BITS_PER_CHAR bits in each part.
           The number of parts is chosen so as to be sufficient to hold
           the enough bits to fit into the two C_HOST_WIDE_INTs that contain
           the integer value (this is always at least as many bits as are
           in a target `long long' value, but may be wider).  */
#define TOTAL_PARTS ((C_HOST_BITS_PER_WIDE_INT / C_HOST_BITS_PER_CHAR) * 2 + 2)

        int parts[TOTAL_PARTS];
        int overflow = 0;

        enum anon1 { NOT_FLOAT, AFTER_POINT, TOO_MANY_POINTS} floatflag
          = NOT_FLOAT;

        for (count = 0; count < TOTAL_PARTS; count++)
        {
          parts[count] = 0;
        }

        p = token_buffer;
        *p++ = c;

        if (c == '0')
          {
            *p++ = (c = GETC());
            if ((c == 'x') || (c == 'X'))
              {
                base = 16;
                *p++ = (c = GETC());
              }
            /* Leading 0 forces octal unless the 0 is the only digit.  */
            else if (c >= '0' && c <= '9')
              {
                base = 8;
                numdigits++;
              }
            else
              numdigits++;
          }

        /* Read all the digits-and-decimal-points.  */

        while (c == '.'
               || (isalnum (c) && c != 'l' && c != 'L'
                   && c != 'u' && c != 'U'
                   && c != 'i' && c != 'I' && c != 'j' && c != 'J'
                   && (floatflag == NOT_FLOAT || ((c != 'f') && (c != 'F')))))
          {
            if (c == '.')
              {
                if (base == 16)
                {
                  error ("floating constant may not be in radix 16");
                }
                if (floatflag == TOO_MANY_POINTS)
                  /* We have already emitted an error.  Don't need another.  */
                  ;
                else if (floatflag == AFTER_POINT)
                  {
                    error ("malformed floating constant");
                    floatflag = TOO_MANY_POINTS;
                    /* Avoid another error from atof by forcing all characters
                       from here on to be ignored.  */
                    p[-1] = '\0';
                  }
                else
                  floatflag = AFTER_POINT;

                base = 10;
                *p++ = c = GETC();
                /* Accept '.' as the start of a floating-point number
                   only when it is followed by a digit.
                   Otherwise, unread the following non-digit
                   and use the '.' as a structural token.  */
                if (p == token_buffer + 2 && !isdigit (c))
                  {
                    if (c == '.')
                      {
                        c = GETC();
                        if (c == '.')
                          {
                            *p++ = c;
                            *p = 0;
                            return tok_ELLIPSIS;
                          }
                        error ("parse error at `..'");
                      }
                    UNGETC (c);
                    token_buffer[1] = 0;
                    value = '.';
                    goto done;
                  }
              }
            else
              {
                /* It is not a decimal point.
                   It should be a digit (perhaps a hex digit).  */

                if (isdigit (c))
                  {
                    c = c - '0';
                  }
                else if (base <= 10)
                  {
                    if (c == 'e' || c == 'E')
                      {
                        base = 10;
                        floatflag = AFTER_POINT;
                        break;   /* start of exponent */
                      }
                    error ("nondigits in number and not hexadecimal");
                    c = 0;
                  }
                else if (c >= 'a')
                  {
                    c = c - 'a' + 10;
                  }
                else
                  {
                    c = c - 'A' + 10;
                  }
                if (c >= largest_digit)
                  largest_digit = c;
                numdigits++;

                for (count = 0; count < TOTAL_PARTS; count++)
                  {
                    parts[count] *= base;
                    if (count)
                      {
                        parts[count]
                          += (parts[count-1] >> C_HOST_BITS_PER_CHAR);
                        parts[count-1]
                          &= (1 << C_HOST_BITS_PER_CHAR) - 1;
                      }
                    else
                      parts[0] += c;
                  }

                /* If the extra highest-order part ever gets anything in it,
                   the number is certainly too big.  */
                if (parts[TOTAL_PARTS - 1] != 0)
                  overflow = 1;

                if (p >= token_buffer + maxtoken - 3)
                  p = extend_token_buffer (p);
                *p++ = (c = GETC());
              }
          }

        if (numdigits == 0)
        {
          error ("numeric constant with no digits");
        }

        if (largest_digit >= base)
        {
          error ("numeric constant contains digits beyond the radix");
        }

        /* Remove terminating char from the token buffer and delimit the string */
        *--p = 0;

        if (floatflag != NOT_FLOAT)
          {
            c_tree_node *type = c_double_type_node;
            int imag = 0;
            int conversion_errno = 0;
            double value;
# ifdef LUDO_DONE
            jmp_buf handler;
# endif
            /* Read explicit exponent if any, and put it in tokenbuf.  */

            if ((c == 'e') || (c == 'E'))
              {
                if (p >= token_buffer + maxtoken - 3)
                  p = extend_token_buffer(p);
                *p++ = c;
                c = GETC();
                if ((c == '+') || (c == '-'))
                  {
                    *p++ = c;
                    c = GETC();
                  }
                if (! isdigit (c))
                {
                  error ("floating constant exponent has no digits");
                }
                while (isdigit (c))
                  {
                    if (p >= token_buffer + maxtoken - 3)
                      p = extend_token_buffer (p);
                    *p++ = c;
                    c = GETC();
                  }
              }

            *p = 0;

            /* Convert string to a double, checking for overflow.  */
# ifdef LUDO_DONE
            if (setjmp (handler))
              {
                error ("floating constant out of range");
                value = dconst0;
              }
            else
# endif
              {
                int fflag = 0, lflag = 0;
                /* Copy token_buffer now, while it has just the number
                   and not the suffixes; once we add `f' or `i',
                   REAL_VALUE_ATOF may not work any more.  */
                char *copy = (char *) autallocblock(p - token_buffer + 1);
                bcopy (token_buffer, copy, p - token_buffer + 1);

# ifdef LUDO_DONE
                set_float_handler (handler);
# endif
                while (1)
                  {
                    int lose = 0;

                    /* Read the suffixes to choose a data type.  */
                    switch (c)
                      {
                      case 'f': case 'F':
                        if (fflag)
                        {
                          error ("more than one `f' in numeric constant");
                        }
                        fflag = 1;
                        break;

                      case 'l': case 'L':
                        if (lflag)
                        {
                          error ("more than one `l' in numeric constant");
                        }
                        lflag = 1;
                        break;

                      case 'i': case 'I':
                        if (imag)
                        {
                          error ("more than one `i' or `j' in numeric constant");
                        }
                        else if (c_pedantic)
                        {
                          pedwarn ("ANSI C forbids imaginary numeric constants");
                        }
                        imag = 1;
                        break;

                      default:
                        lose = 1;
                      }

                    if (lose)
                      break;

                    if (p >= token_buffer + maxtoken - 3)
                      p = extend_token_buffer (p);
                    *p++ = c;
                    *p = 0;
                    c = GETC();
                  }

                /* The second argument, machine_mode, of REAL_VALUE_ATOF
                   tells the desired precision of the binary result
                   of decimal-to-binary conversion.  */

                if (fflag)
                  {
                    if (lflag)
                    {
                      error ("both `f' and `l' in floating constant");
                    }

                    type = c_float_type_node;
                    errno = 0;
                    value = atof(copy);

                    conversion_errno = errno;
# ifdef LUDO_DONE
                    /* A diagnostic is required here by some ANSI C testsuites.
                       This is not pedwarn, become some people don't want
                       an error for this.  */
                    if (REAL_VALUE_ISINF (value) && c_pedantic)
                    {
                      warning ("floating point number exceeds range of `float'");
                    }
# endif
                  }
                else if (lflag)
                  {
                    type = c_long_double_type_node;
                    errno = 0;
                    value = atof(copy);
                    conversion_errno = errno;
# ifdef LUDO_DONE
                    if (REAL_VALUE_ISINF (value) && c_pedantic)
                    {
                      warning ("floating point number exceeds range of `long double'");
                    }
# endif
                  }
                else
                  {
                    errno = 0;
                    value = atof(copy);
                    conversion_errno = errno;
# ifdef LUDO_DONE
                    if (REAL_VALUE_ISINF (value) && c_pedantic)
                    {
                      warning ("floating point number exceeds range of `double'");
                    }
# endif
                  }

# ifdef LUDO_DONE
                set_float_handler (NULL_PTR);
# endif
            }

# ifdef LUDO_DONE
#ifdef ERANGE
            /* ERANGE is also reported for underflow,
               so test the value to distinguish overflow from that.  */
            if (conversion_errno == ERANGE && !c_flag_traditional && c_pedantic
                && (REAL_VALUES_LESS (dconst1, value)
                    || REAL_VALUES_LESS (value, dconstm1)))
            {
              warning ("floating point number exceeds range of `double'");
            }
#endif

            /* If the result is not a number, assume it must have been
               due to some error message above, so silently convert
               it to a zero.  */
            if (REAL_VALUE_ISNAN (value))
              value = dconst0;
# endif

            /* Create a node with determined type and value.  */
            if (imag)
            {
              c_y_lval.ttype = c_tree_build_complex( (c_tree_node *)0,
                                            c_convert_expr(type, c_integer_zero_node),
                                            c_tree_build_real(type, value));
            }
            else
            {
              c_y_lval.ttype = c_tree_build_real(type, value);
            }
          }
        else
          {
            c_tree_node *traditional_type;
            c_tree_node *ansi_type;
            c_tree_node *type;
            int high, low;
            int spec_unsigned = 0;
            int spec_long = 0;
            int spec_long_long = 0;
            int spec_imag = 0;
            int bytes, warn, i;

            while (1)
              {
                if (c == 'u' || c == 'U')
                  {
                    if (spec_unsigned)
                    {
                      error ("two `u's in integer constant");
                    }
                    spec_unsigned = 1;
                  }
                else if (c == 'l' || c == 'L')
                  {
                    if (spec_long)
                      {
                        if (spec_long_long)
                        {
                          error ("three `l's in integer constant");
                        }
                        else if (c_pedantic)
                        {
                          pedwarn ("ANSI C forbids long long integer constants");
                        }
                        spec_long_long = 1;
                      }
                    spec_long = 1;
                  }
                else if (c == 'i' || c == 'j' || c == 'I' || c == 'J')
                  {
                    if (spec_imag)
                    {
                      error ("more than one `i' or `j' in numeric constant");
                    }
                    else if (c_pedantic)
                    {
                      pedwarn ("ANSI C forbids imaginary numeric constants");
                    }
                    spec_imag = 1;
                  }
                else
                  break;
                if (p >= token_buffer + maxtoken - 3)
                {
                  p = extend_token_buffer (p);
                }
                *p++ = c;
                c = GETC();
              }

            /* If the constant won't fit in an unsigned long long,
               then warn that the constant is out of range.  */

            /* ??? This assumes that long long and long integer types are
               a multiple of 8 bits.  This better than the original code
               though which assumed that long was exactly 32 bits and long
               long was exactly 64 bits.  */

            bytes = CTreeTypePrecision(c_long_long_integer_type_node) / 8;

            warn = overflow;
            for (i = bytes; i < TOTAL_PARTS; i++)
            {
              if (parts[i]) warn = 1;
            }
            if (warn)
            {
              pedwarn ("integer constant out of range");
            }

            /* This is simplified by the fact that our constant
               is always positive.  */

            high = low = 0;

            for (i = 0; i < C_HOST_BITS_PER_WIDE_INT / C_HOST_BITS_PER_CHAR; i++)
              {
                high |= ((int) parts[i + (C_HOST_BITS_PER_WIDE_INT
                                                    / C_HOST_BITS_PER_CHAR)]
                         << (i * C_HOST_BITS_PER_CHAR));
                low |= (int) parts[i] << (i * C_HOST_BITS_PER_CHAR);
              }
            
            c_y_lval.ttype = c_tree_build_int_2(low, high);
            CTreeType( c_y_lval.ttype ) = c_long_long_unsigned_type_node;

            /* If warn_traditional, calculate both the ANSI type and the
               traditional type, then see if they disagree.
               Otherwise, calculate only the type for the dialect in use.  */
            if (c_warn_traditional || c_flag_traditional)
            {
                /* Calculate the traditional type.  */
                /* Traditionally, any constant is signed;
                   but if unsigned is specified explicitly, obey that.
                   Use the smallest size with the right number of bits,
                   except for one special case with decimal constants.  */
                if (! spec_long && base != 10
                    && c_int_fits_type_p(c_y_lval.ttype, c_unsigned_type_node))
                  traditional_type = (spec_unsigned ? c_unsigned_type_node
                                      : c_integer_type_node);
                /* A decimal constant must be long
                   if it does not fit in type int.
                   I think this is independent of whether
                   the constant is signed.  */
                else if (! spec_long && base == 10
                         && c_int_fits_type_p(c_y_lval.ttype, c_integer_type_node))
                  traditional_type = (spec_unsigned ? c_unsigned_type_node
                                      : c_integer_type_node);
                else if (! spec_long_long)
                  traditional_type = (spec_unsigned ? c_long_unsigned_type_node
                                      : c_long_integer_type_node);
                else
                  traditional_type = (spec_unsigned
                                      ? c_long_long_unsigned_type_node
                                      : c_long_long_integer_type_node);
            }
            if (c_warn_traditional || ! c_flag_traditional)
            {
                /* Calculate the ANSI type.  */
                if (! spec_long && ! spec_unsigned
                    && c_int_fits_type_p(c_y_lval.ttype, c_integer_type_node))
                  ansi_type = c_integer_type_node;
                else if (! spec_long && (base != 10 || spec_unsigned)
                         && c_int_fits_type_p(c_y_lval.ttype, c_unsigned_type_node))
                  ansi_type = c_unsigned_type_node;
                else if (! spec_unsigned && !spec_long_long
                         && c_int_fits_type_p(c_y_lval.ttype, c_long_integer_type_node))
                  ansi_type = c_long_integer_type_node;
                else if (! spec_long_long
                         && c_int_fits_type_p(c_y_lval.ttype,
                                             c_long_unsigned_type_node))
                  ansi_type = c_long_unsigned_type_node;
                else if (! spec_unsigned
                         && c_int_fits_type_p(c_y_lval.ttype,
                                             c_long_long_integer_type_node))
                  ansi_type = c_long_long_integer_type_node;
                else
                  ansi_type = c_long_long_unsigned_type_node;
            }

            type = c_flag_traditional ? traditional_type : ansi_type;

            if (c_warn_traditional && traditional_type != ansi_type)
            {
              if ( CTreeTypePrecision(traditional_type) != 
                   CTreeTypePrecision(ansi_type))
              {
                warning ("width of integer constant changes with -traditional");
              }
              else if ( CIsTreeNodeUnsigned(traditional_type) != 
                        CIsTreeNodeUnsigned(ansi_type))
              {
                warning("integer constant is unsigned in ANSI C, signed with -traditional");
              }
              else
              {
                warning("width of integer constant may change on other systems with -traditional");
              }
            }

            if (c_pedantic && !c_flag_traditional && !spec_long_long && !warn
                && (CTreeTypePrecision(c_long_integer_type_node)
                    < CTreeTypePrecision(type)))
            {
              pedwarn("integer constant out of range");
            }

            if (base == 10 && ! spec_unsigned && CIsTreeNodeUnsigned(type))
            {
              warning("decimal constant is so large that it is unsigned");
            }

            if (spec_imag)
            {
                if ( CTreeTypePrecision(type)
                    <= CTreeTypePrecision(c_integer_type_node))
                  c_y_lval.ttype
                    = c_tree_build_complex((c_tree_node *)0, c_integer_zero_node,
                                c_convert_expr(c_integer_type_node, c_y_lval.ttype));
                else
              {
                error ("complex integer constant is too wide for `complex int'");
              }
            }
            else if (c_flag_traditional && !c_int_fits_type_p (c_y_lval.ttype, type))
              /* The traditional constant 0x80000000 is signed
                 but doesn't fit in the range of int.
                 This will change it to -0x80000000, which does fit.  */
            {
                CTreeType(c_y_lval.ttype) = c_get_unsigned_type(type);
                c_y_lval.ttype = c_convert_expr(type, c_y_lval.ttype);
                CClearTreeNodeCstOverflow( c_y_lval.ttype );
                CClearTreeNodeOverflow( c_y_lval.ttype );
            }
            else
            {
              CTreeType(c_y_lval.ttype) = type;
            }
          }

        UNGETC (c);
        *p = 0;

        if (isalnum (c) || c == '.' || c == '_' || c == '$'
            || (!c_flag_traditional && (c == '-' || c == '+')
                && (p[-1] == 'e' || p[-1] == 'E')))
        {
          error ("missing white space after number `%s'", token_buffer);
        }

        value = tok_CONSTANT; break;
      }

    case '\'':
    char_constant:
      {
        register int result = 0;
        register int num_chars = 0;
        unsigned width = CTreeTypePrecision(c_char_type_node);
        int max_chars;

        if (wide_flag)
          {
            width = C_WCHAR_TYPE_SIZE;
#ifdef MULTIBYTE_CHARS
            max_chars = MB_CUR_MAX;
#else
            max_chars = 1;
#endif
          }
        else
          max_chars = CTreeTypePrecision( c_integer_type_node ) / width;

        while (1)
          {
          tryagain:

            c = GETC();

            if (c == '\'' || c == EOF)
              break;

            if (c == '\\')
              {
                int ignore = 0;
                c = readescape (&ignore);
                if (ignore)
                  goto tryagain;
                if (width < C_HOST_BITS_PER_INT
                     && (unsigned) c >= (1 << width))
                {
                  pedwarn ("escape sequence out of range for character");
                }
#ifdef MAP_CHARACTER
                if (isprint (c))
                  c = MAP_CHARACTER (c);
#endif
              }
            else if (c == '\n')
              {
                if (c_pedantic)
                {
                  pedwarn ("ANSI C forbids newline in character constant");
                }
                c_lineno++;
              }
#ifdef MAP_CHARACTER
            else
              c = MAP_CHARACTER (c);
#endif

            num_chars++;
            if (num_chars > maxtoken - 4)
              extend_token_buffer( token_buffer );

            token_buffer[num_chars] = c;

            /* Merge character into result; ignore excess chars.  */
            if (num_chars < max_chars + 1)
              {
                if (width < C_HOST_BITS_PER_INT)
                  result = (result << width) | (c & ((1 << width) - 1));
                else
                  result = c;
              }
          }

        token_buffer[num_chars + 1] = '\'';
        token_buffer[num_chars + 2] = 0;

        if (c != '\'')
        {
          error("malformatted character constant");
        }
        else if (num_chars == 0)
        {
          error("empty character constant");
        }
        else if (num_chars > max_chars)
          {
            num_chars = max_chars;
            {
              error ("character constant too long");
            }
          }
        else if (num_chars != 1 && ! c_flag_traditional)
        {
          warning("multi-character character constant");
        }

        /* If char type is signed, sign-extend the constant.  */
        if (! wide_flag)
          {
            int num_bits = num_chars * width;
            if (num_bits == 0)
              /* We already got an error; avoid invalid shift.  */
              c_y_lval.ttype = c_tree_build_int_2 (0, 0);
            else if ( CIsTreeNodeUnsigned( c_char_type_node )
                     || ((result >> (num_bits - 1)) & 1) == 0)
              c_y_lval.ttype
                = c_tree_build_int_2( result & ((unsigned int) ~0
                                         >> (C_HOST_BITS_PER_WIDE_INT - num_bits)), 0);
            else
              c_y_lval.ttype
                = c_tree_build_int_2 (result | ~((unsigned int) ~0
                                          >> (C_HOST_BITS_PER_WIDE_INT - num_bits)), -1);
            CTreeType( c_y_lval.ttype ) = c_integer_type_node;
          }
        else
          {
#ifdef MULTIBYTE_CHARS
            /* Set the initial shift state and convert the next sequence.  */
            result = 0;
            /* In all locales L'\0' is zero and mbtowc will return zero,
               so don't use it.  */
            if (num_chars > 1
                || (num_chars == 1 && token_buffer[1] != '\0'))
              {
                wchar_t wc;
                (void) mbtowc (NULL_PTR, NULL_PTR, 0);
                if (mbtowc (& wc, token_buffer + 1, num_chars) == num_chars)
                  result = wc;
                else
                {
                  warning ("Ignoring invalid multibyte character");
                }
              }
#endif
            c_y_lval.ttype = c_tree_build_int_2(result, 0);
            CTreeType( c_y_lval.ttype ) = c_wchar_type_node;
          }
        value = tok_CONSTANT;
        break;
      }

    case '"':
    string_constant:
      {
        c = GETC();
        p = token_buffer + 1;

        while (c != '"' && c >= 0)
        {
            if (c == '\\')
              {
                int ignore = 0;
                c = readescape (&ignore);
                if (ignore)
                  goto skipnewline;
                if (!wide_flag
                    && CTreeTypePrecision( c_char_type_node) < C_HOST_BITS_PER_INT
                    && c >= (1 << CTreeTypePrecision( c_char_type_node )))
                {
                  pedwarn ("escape sequence out of range for character");
                }
              }
            else if (c == '\n')
              {
                if (c_pedantic)
                {
                  pedwarn ("ANSI C forbids newline in string constant");
                }
                c_lineno++;
              }

            if (p == token_buffer + maxtoken)
              p = extend_token_buffer (p);
            *p++ = c;

          skipnewline:
            c = GETC();
        }

        *p = 0;

        if (c < 0)
        {
          error ("Unterminated string constant");
        }

        /* We have read the entire constant.
           Construct a STRING_CST for the result.  */

	if (wide_flag)
	  {
	    /* If this is a L"..." wide-string, convert the multibyte string
	       to a wide character string.  */
	    char *widep = (char *) autallocblock((p - token_buffer) * C_WCHAR_BYTES);
	    int len;

#ifdef MULTIBYTE_CHARS
	    len = mbstowcs ((wchar_t *) widep, token_buffer + 1, p - token_buffer);
	    if (len < 0 || len >= (p - token_buffer))
	      {
		warning ("Ignoring invalid multibyte string");
		len = 0;
	      }
	    bzero (widep + (len * WCHAR_BYTES), WCHAR_BYTES);
#else
	    {
	      char *wp, *cp;

	      wp = widep + (C_BYTES_BIG_ENDIAN ? C_WCHAR_BYTES - 1 : 0);
	      bzero (widep, (p - token_buffer) * C_WCHAR_BYTES);
	      for (cp = token_buffer + 1; cp < p; cp++)
		*wp = *cp, wp += C_WCHAR_BYTES;
	      len = p - token_buffer - 1;
	    }
#endif
	    c_y_lval.ttype = c_tree_build_string ((len + 1) * C_WCHAR_BYTES, widep);
	    CTreeType( c_y_lval.ttype ) = c_wchar_array_type_node;
	    value = tok_STRING;
	  }
# ifdef LUDO_DONE
        else if (objc_flag)
          {
            extern tree build_objc_string();
            /* Return an Objective-C @"..." constant string object.  */
            c_y_lval.ttype = build_objc_string (p - token_buffer,
                                              token_buffer + 1);
            TREE_TYPE (c_y_lval.ttype) = char_array_type_node;
            value = tok_OBJC_STRING;
          }
# endif
        else
          {
            *p++ = '"';
            *p = 0;

            c_y_lval.ttype = c_tree_build_string(p - token_buffer, token_buffer );
            CTreeType( c_y_lval.ttype ) = c_char_array_type_node;
            value = tok_STRING;
          }


        break;
      }

    case '+':
    case '-':
    case '&':
    case '|':
    case ':':
    case '<':
    case '>':
    case '*':
    case '/':
    case '%':
    case '^':
    case '!':
    case '=':
      {
        register int c1;

      combine:

        switch (c)
          {
          case '+':
            c_y_lval.code = C_PLUS_EXPR; break;
          case '-':
            c_y_lval.code = C_MINUS_EXPR; break;
          case '&':
            c_y_lval.code = C_BIT_AND_EXPR; break;
          case '|':
            c_y_lval.code = C_BIT_IOR_EXPR; break;
          case '*':
            c_y_lval.code = C_MULT_EXPR; break;
          case '/':
            c_y_lval.code = C_TRUNC_DIV_EXPR; break;
          case '%':
            c_y_lval.code = C_TRUNC_MOD_EXPR; break;
          case '^':
            c_y_lval.code = C_BIT_XOR_EXPR; break;
          case tok_LSHIFT:
            c_y_lval.code = C_LSHIFT_EXPR; break;
          case tok_RSHIFT:
            c_y_lval.code = C_RSHIFT_EXPR; break;
          case '<':
            c_y_lval.code = C_LT_EXPR; break;
          case '>':
            c_y_lval.code = C_GT_EXPR; break;
          }

        token_buffer[1] = c1 = GETC();
        token_buffer[2] = 0;

        if (c1 == '=')
          {
            switch (c)
              {
              case '<':
                value = tok_ARITHCOMPARE; c_y_lval.code = C_LE_EXPR; goto done;
              case '>':
                value = tok_ARITHCOMPARE; c_y_lval.code = C_GE_EXPR; goto done;
              case '!':
                value = tok_EQCOMPARE; c_y_lval.code = C_NE_EXPR; goto done;
              case '=':
                value = tok_EQCOMPARE; c_y_lval.code = C_EQ_EXPR; goto done;
              }
            value = tok_ASSIGN; goto done;
          }
        else if (c == c1)
          switch (c)
            {
            case '+':
              value = tok_PLUSPLUS; goto done;
            case '-':
              value = tok_MINUSMINUS; goto done;
            case '&':
              value = tok_ANDAND; goto done;
            case '|':
              value = tok_OROR; goto done;
            case '<':
              c = tok_LSHIFT;
              goto combine;
            case '>':
              c = tok_RSHIFT;
              goto combine;
            }
        else
          switch (c)
            {
            case '-':
              if (c1 == '>')
                { value = tok_POINTSAT; goto done; }
              break;
            case ':':
              if (c1 == '>')
                { value = ']'; goto done; }
              break;
            case '<':
              if (c1 == '%')
                { value = '{'; indent_level++; goto done; }
              if (c1 == ':')
                { value = '['; goto done; }
              break;
            case '%':
              if (c1 == '>')
                { value = '}'; indent_level--; goto done; }
              break;
            }
        UNGETC (c1);
        token_buffer[1] = 0;

        if ((c == '<') || (c == '>'))
          value = tok_ARITHCOMPARE;
        else value = c;
        goto done;
      }

    case 0:
      /* Don't make c_y_parse think this is eof.  */
      value = 1;
      break;

    case '{':
      indent_level++;
      value = c;
      break;

    case '}':
      indent_level--;
      value = c;
      break;

    default:
      value = c;
    }

done:
/*  c_y_lloc.last_line = lineno; */

  return value;
}

/* Sets the value of the 'c_y_debug' variable to VALUE.
   This is a function so we don't have to have C_Y_DEBUG defined
   in order to build the compiler.  */

void
set_c_y_debug (value)
     int value;
{
#if C_Y_DEBUG != 0
  c_y_debug = value;
#else
  warning ("C_Y_DEBUG not defined.");
#endif
}
