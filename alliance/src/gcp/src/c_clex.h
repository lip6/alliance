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
/* Define constants for communication with c-parse.y.
   Copyright (C) 1987, 1992 Free Software Foundation, Inc.

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



enum rid
{
  RID_UNUSED,
  RID_INT,
  RID_CHAR,
  RID_FLOAT,
  RID_DOUBLE,
  RID_VOID,
  RID_UNUSED1,

  RID_UNSIGNED,
  RID_SHORT,
  RID_LONG,
  RID_AUTO,
  RID_STATIC,
  RID_EXTERN,
  RID_REGISTER,
  RID_TYPEDEF,
  RID_SIGNED,
  RID_CONST,
  RID_VOLATILE,
  RID_INLINE,
  RID_NOALIAS,
  RID_ITERATOR,
  RID_COMPLEX,

  RID_IN,
  RID_OUT,
  RID_INOUT,
  RID_BYCOPY,
  RID_ONEWAY,
  RID_ID,

  RID_MAX
};

typedef struct c_stmt_or_label
{
  c_tree_node *stmt;
  int          ends_in_label;

} c_stmt_or_label;


#define NORID RID_UNUSED

#define RID_FIRST_MODIFIER RID_UNSIGNED

/* The elements of `ridpointers' are identifier nodes
   for the reserved type names and storage classes.
   It is indexed by a RID_... value.  */
extern c_tree_node *c_ridpointers[(int) RID_MAX];

/* the declaration found for the last IDENTIFIER token read in.
   yylex must look this up to detect typedefs, which get token type TYPENAME,
   so it is left around in case the identifier is not a typedef but is
   used in a context which makes it a reference to a variable.  */
extern c_tree_node *c_lastiddecl;

extern char *token_buffer;	/* Pointer to token buffer.  */

# ifdef LUDO_NO_SKIP
extern tree make_pointer_declarator();
extern int yylex ();
# else
  extern            int  c_y_lex __P(());
# endif
  extern           void  c_reinit_parse_for_function __P(());
  extern           void  c_position_after_white_space __P(()); 
  extern           void  c_init_lex __P(( char *InputFileName ));

   extern char *get_directive_line ();
