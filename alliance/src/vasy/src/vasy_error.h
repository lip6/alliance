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
| Tool    :                     VASY                          |
|                                                             |
| File    :                 vasy_error.h                      |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef VASY_ERROR_H
# define VASY_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Errors                            |
|                                                             |
\------------------------------------------------------------*/

# define VASY_ILLEGAL_ASSIGN_ERROR          0
# define VASY_NO_WAIT_IN_PROCESS_ERROR      1
# define VASY_NOT_YET_IMPLEMENTED_ERROR     2
# define VASY_LOOP_IN_SIMULATION            3
# define VASY_ERROR_IN_SIMULATION           4
# define VASY_ERROR_IN_SUPPORT              5
# define VASY_ERROR_IN_VEX2BDD              6
# define VASY_ERROR_IN_ABL2VEX              7
# define VASY_ERROR_IN_DRVVEX               8
# define VASY_ERROR_OPERATOR_NOT_SUPPORTED  9
# define VASY_ERROR_IN_UNROLL_LOOP         10
# define VASY_LOOP_IN_REDUCTION            11
# define VASY_ERROR_ILLEGAL_USE_OF         12
# define VASY_ERROR_CONVERT_WRONG_TYPE     13

# define VASY_WARNING_IN_SIMULATION         1

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

# define VasyError( E, T ) \
                              \
  VasyDisplayError( basename(__FILE__), __LINE__, (E), (vpnline_list *)0, (T) )

# define VasyWarning( W, T ) \
                                \
  VasyDisplayWarning( basename(__FILE__), __LINE__, (W), (vpnline_list *)0, (T) )

# define VasyErrorLine( E, L, T ) \
                              \
  VasyDisplayError( basename(__FILE__), __LINE__, (E), (vpnline_list *)(L), (T) )

# define VasyWarningLine( W, L, T ) \
                                \
  VasyDisplayWarning( basename(__FILE__), __LINE__, (W), (vpnline_list *)(L), (T) )

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

  extern void VasyDisplayError();
  extern void VasyDisplayWarning();
  extern void VasyDisplayLines();

# endif
