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
| File    :                  c_convert.h                    |
|                                                             |
| Date    :                   09.07.99                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_CONVERT_H
# define C_CONVERT_H

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

  extern   c_tree_node * c_convert_to_pointer __P((c_tree_node *type, c_tree_node *expr));
  extern   c_tree_node * c_convert_to_real __P((c_tree_node *type, c_tree_node *expr));
  extern   c_tree_node * c_convert_to_complex __P((c_tree_node *type, c_tree_node *expr));
  extern   c_tree_node * c_convert_to_integer __P((c_tree_node *type, c_tree_node *expr));
  extern   c_tree_node * c_convert_expr __P((c_tree_node *type, c_tree_node *expr));
  extern   c_tree_node * c_default_conversion __P((c_tree_node *exp));
  extern   c_tree_node * c_truthvalue_conversion __P((c_tree_node *expr));
  extern   c_tree_node * c_convert_and_check __P((c_tree_node *type, c_tree_node *expr));
  extern   int           c_is_staticp __P((c_tree_node *arg));

     c_tree_node *arg;

# endif
