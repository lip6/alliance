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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
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
| Tool    :                     FVH                           |
|                                                             |
| File    :                  fvherror.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef FVH_ERROR_H
# define FVH_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define FVH_ERROR_OPEN_FILE               1
# define FVH_ERROR_PARSE_FILE              2
# define FVH_ERROR_SIGNAL_NOT_FOUND        3
# define FVH_ERROR_ILLEGAL_DEFAULT_ASSIGN  4
# define FVH_ERROR_NO_STATE                5
# define FVH_ERROR_CONTROL_TYPE            6
# define FVH_ERROR_ILLEGAL_CASE            7
# define FVH_ERROR_ILLEGAL_STATE           8
# define FVH_ERROR_ILLEGAL_CONTROL         9
# define FVH_ERROR_ILLEGAL_INSTRUCTION    10
# define FVH_ERROR_ILLEGAL_STATE_ASSIGN   11
# define FVH_ERROR_ILLEGAL_STACK_ASSIGN   12
# define FVH_ERROR_ILLEGAL_CONTROL_ASSIGN 13
# define FVH_ERROR_ILLEGAL_ASSIGNATION    14
# define FVH_ERROR_MAIN_PROCESS           15
# define FVH_ERROR_PROCESS_SENSITIVITY    16
# define FVH_ERROR_IN_MAIN_PROCESS        17
# define FVH_ERROR_IN_CLOCK_PROCESS       18
# define FVH_ERROR_ILLEGAL_OUT_ASSIGN     19
# define FVH_ERROR_MISSING_CLOCK_PORT     22
# define FVH_ERROR_STACK_SIZE_ZERO        23
# define FVH_ERROR_MISSING_STACK_CONTROL  24
# define FVH_ERROR_MISSING_STATE_CONTROL  25
# define FVH_ERROR_NO_CONTROL_CONDITION   26
# define FVH_ERROR_BAD_STACK_CONTROL      27
# define FVH_ERROR_ILLEGAL_IF_CONDITION   28
# define FVH_ERROR_TWO_PROCESS            29
# define FVH_ERROR_CLOCK_PROCESS          30
# define FVH_ERROR_MISSING_SIGNAL         31
# define FVH_ERROR_ILLEGAL_PROCESS_BODY   32
# define FVH_ERROR_PRAGMA_SPECIFICATION   33
# define FVH_ERROR_BAD_PROCESS            34
# define FVH_ERROR_NOT_YET_IMPLEMENTED    35

/*------------------------------------------------------------\
|                                                             |
|                           Warnings                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define fvherror( E, T, V ) \
  (fvh_error((int)(E),(char *)(T),(long)(V),autbasename(__FILE__,NULL),__LINE__))
# define fvhwarning( E, T, V ) \
  (fvh_warning((int)(E),(char *)(T),(long)(V),autbasename(__FILE__,NULL),__LINE__))

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern           void  fvh_error __P((char Error, char *Text, long Value, char *File, long Line));
  extern           void  fvh_warning __P((char Warning, char *Text, long Value, char *File, long Line));

# endif
