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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                 Bdd Errors                        |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef BDD_ERROR_H
# define BDD_ERROR_H

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Error                           |
|                                                             |
\------------------------------------------------------------*/

# define BDD_MODEL_VAR_ERROR           0
# define BDD_MODEL_OPER_ERROR          1
# define BDD_HNODE_SIZE_ERROR          2
# define BDD_HNODE_KEY_ERROR           3
# define BDD_HOPER_SIZE_ERROR          4
# define BDD_HOPER_KEY_ERROR           5
# define BDD_BAD_INDEX_ERROR           6
# define BDD_BAD_VAR_ERROR             7
# define BDD_BAD_CHILD_ERROR           8
# define BDD_BAD_IN_ERROR              9
# define BDD_OPERATOR_ERROR           10
# define BDD_BAD_LIST_ERROR           11
# define BDD_NO_MORE_VAR_ERROR        12
# define BDD_NEGATIVE_REF_INT_ERROR   13
# define BDD_NEGATIVE_REF_EXT_ERROR   14
# define BDD_UNKNOWN_IN_MODE_ERROR    15
# define BDD_MAX_NODE_ERROR           16
# define BDD_BAD_ASSOC_ERROR          17
# define BDD_NO_MORE_ASSOC_ERROR      18
# define BDD_UNKNOWN_AUX_MODE_ERROR   19
# define BDD_DUMP_FILE_ERROR          20
# define BDD_UNDUMP_FILE_ERROR        21
# define BDD_UNDUMP_LINE_ERROR        22

/*------------------------------------------------------------\
|                                                             |
|                           Warning                           |
|                                                             |
\------------------------------------------------------------*/

# define BDD_DYNAMIC_REORDER_WARNING   0
# define BDD_OVERFLOW_REF_INT_WARNING  1
# define BDD_OVERFLOW_REF_EXT_WARNING  2
# define BDD_DYNAMIC_EXPLOSION_WARNING 3

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define bdderror( E, V ) \
  (bdd_error( (int)(E), (long)(V), basename(__FILE__), __LINE__ ))
# define bddwarning( W, V1, V2 ) \
  (bdd_warning( (int)(W), (long)(V1), (long)(V2), basename(__FILE__), __LINE__ ))

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void  bdd_error __P((int Error, long Value, char *File, long Line));
  extern void  bdd_warning __P((int Warning, long Value1, long Value2, char *File, long Line));

# endif
