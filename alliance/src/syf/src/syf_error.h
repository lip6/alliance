/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     SYF                           |
|                                                             |
| File    :                 syf_error.h                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef SYF_ERROR_H
# define SYF_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Errors                            |
|                                                             |
\------------------------------------------------------------*/

# define SYF_ERROR_OPEN_FILE               1
# define SYF_ERROR_MISSING_SCAN_PORT       2
# define SYF_ERROR_UNKNOWN_ENCODE_TYPE     4
# define SYF_ERROR_BAD_STACK_NO_PUSH       5
# define SYF_ERROR_BAD_STACK_NO_POP        6
# define SYF_ERROR_SYNTAX                  7
# define SYF_ERROR_UNEXPECTED_EOF          8
# define SYF_ERROR_UNKNOWN_STATE           9
# define SYF_ERROR_DUPLICATE_CODE         10
# define SYF_ERROR_WRONG_CODE             11
# define SYF_ERROR_ILLEGAL_FSM_TYPE       12
# define SYF_ERROR_BDD_ATOM_UNKNOWN       13
# define SYF_ERROR_BDD_OPER               14
# define SYF_ERROR_BDD_EXPR_NULL          15
# define SYF_ERROR_BDD_LOOP               16
# define SYF_ERROR_ORDER_ATOM_UNKNOWN     17
# define SYF_ERROR_ORDER_OPER             18
# define SYF_ERROR_ORDER_EXPR_NULL        19
# define SYF_ERROR_ORDER_LOOP             20

/*------------------------------------------------------------\
|                                                             |
|                           Warnings                          |
|                                                             |
\------------------------------------------------------------*/

# define SYF_WARNING_NO_TRANS_TO           0
# define SYF_WARNING_NO_TRANS_FROM         1
# define SYF_WARNING_REMOVE_UNUSED_STACK   2
# define SYF_WARNING_REMOVE_STACK          3
# define SYF_WARNING_REMOVE_LOCOUT         4
# define SYF_WARNING_REMOVE_TRANS          5
# define SYF_WARNING_INTER_TRANS           6
# define SYF_WARNING_SUM_TRANS             7
# define SYF_WARNING_OUTPUT_NOT_ASSIGNED   8
# define SYF_WARNING_MODIFY_POLARITY       9

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define SyfError( ERROR, TEXT )                             \
                                                             \
    SyfDisplayError( basename(__FILE__), __LINE__, (ERROR), (TEXT) )

# define SyfWarning( WARNING, T1, T2 )                           \
                                                                 \
    SyfDisplayWarning( basename(__FILE__), __LINE__, (WARNING), (T1), (T2) )

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

  extern char SyfDebugMode;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void SyfDisplayError();
  extern void SyfDisplayWarning();

# endif
