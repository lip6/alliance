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
| Tool    :                     RTD                           |
|                                                             |
| File    :                  rtderror.c                       |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.01.95                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RTD_ERROR_H
# define RTD_ERROR_H

# include <libgen.h>

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define RTD_ERROR_OPEN_FILE               1
# define RTD_ERROR_PARSE_FILE              2

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

# define rtderror( E, T, V )   \
  (rtd_error  ( (E), (T), (V), basename(__FILE__), __LINE__ ))
# define rtdwarning( E, T, V ) \
  (rtd_warning( (E), (T), (V), basename(__FILE__), __LINE__ ))

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

  extern           void  rtd_error __P((int Error, char *Text, long Value, char *File, int Line));
  extern           void  rtd_warning __P((int Warning, char *Text, long Value, char *File, int Line));

# endif
