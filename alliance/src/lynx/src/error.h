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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  lyerror.h                        |
|                                                             |
| Authors :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef LYNX_ERROR
# define LYNX_ERROR

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

# define LYNX_ERROR_PHYSICAL_CONNECTOR            0
# define LYNX_ERROR_PHYSICAL_CONNECTORS           1
# define LYNX_ERROR_MISSING_CONNECTOR             2
# define LYNX_ERROR_CUT_TRANSISTOR_ELBOW          3
# define LYNX_ERROR_MASK_COUNTER_NULL             4
# define LYNX_ERROR_ILLEGAL_COMBINATION           5
# define LYNX_ERROR_ILLEGAL_OPTION                6
# define LYNX_ERROR_ILLEGAL_FILENAME              7
# define LYNX_ERROR_MINIMUM_LENGTH                8
# define LYNX_ERROR_TRANSISTOR_GATE               9
# define LYNX_ERROR_TRANSISTOR_LOGICAL           10

/*------------------------------------------------------------\
|                                                             |
|                             Warning                         |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_WARNING_PHYSICAL_CONNECTOR          0
# define LYNX_WARNING_CORE_FILE                   1

/*------------------------------------------------------------\
|                                                             |
|                             Macros                          |
|                                                             |
\------------------------------------------------------------*/

# define LynxError( E, T1, T2 ) \
  (lynx_error( (E), (T1), (T2), basename(__FILE__), __LINE__ ))
# define LynxWarning( W, T ) \
  (lynx_warning( (W), (T), basename(__FILE__), __LINE__ ))

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

  extern void lynx_error();
  extern void lynx_warning();

# endif
