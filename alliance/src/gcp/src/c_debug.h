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
| File    :                  c_debug.h                      |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_DEBUG_H
# define C_DEBUG_H

# define c_header_debug()     fprintf( stdout, "%s:%d: *%s*\n", \
                                       basename(__FILE__),__LINE__, __FUNCTION__)
# define c_fprintf            c_header_debug(); fprintf
# define loc_c_tree_view_node c_header_debug(); fprintf( stdout, "\n" ); c_tree_view_node
# define loc_c_tree_view_node_list c_header_debug(); fprintf( stdout, "\n" ); c_tree_view_node_list
# define loc_c_view_binding   c_header_debug(); fprintf( stdout, "\n" ); c_view_binding

# define warning                    c_header_debug(); c_warning
# define pedwarn                    c_header_debug(); c_pedwarn
# define error                      c_header_debug(); c_error
# define warning_with_decl          c_header_debug(); c_warning_with_decl
# define error_with_decl            c_header_debug(); c_error_with_decl
# define pedwarn_with_decl          c_header_debug(); c_pedwarn_with_decl
# define warning_with_file_and_line c_header_debug(); c_warning_with_file_and_line
# define error_with_file_and_line   c_header_debug(); c_error_with_file_and_line
# define pedwarn_with_file_and_line   c_header_debug(); c_pedwarn_with_file_and_line
# define warning_init               c_header_debug(); c_warning_init
# define pedwarn_init               c_header_debug(); c_pedwarn_init
# define error_init                 c_header_debug(); c_error_init
# define warn_for_assignment        c_header_debug(); c_warn_for_assignment

  extern void c_error __P((char *s, ...));
  extern void c_pedwarn __P((char *s, ...));
  extern void c_warning __P((char *s, ...));

  extern void c_warning_with_file_and_line __P((char *file, int line, char *s, ...));
  extern void c_pedwarn_with_file_and_line __P((char *file, int line, char *s, ...));
  extern void c_error_with_file_and_line __P((char *file, int line, char *s, ...));

  extern void c_error_with_decl __P((c_tree_node *decl, char *s, ...));
  extern void c_warning_with_decl __P((c_tree_node *decl, char *s, ...));
  extern void c_pedwarn_with_decl __P((c_tree_node *decl, char *s, ...));

# endif
