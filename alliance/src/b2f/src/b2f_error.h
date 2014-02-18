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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                     B2F                           |
|                                                             |
| File    :                 b2f_error.h                       |
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

# ifndef B2F_ERROR_H
# define B2F_ERROR_H

/*------------------------------------------------------------\
|                                                             |
|                           Errors                            |
|                                                             |
\------------------------------------------------------------*/

# define B2F_ILLEGAL_RESET_CONDITION 1
# define B2F_REGISTER_NOT_A_VECTOR   2
# define B2F_BAD_CONDITION_REGISTER  3
# define B2F_NO_REGISTER             4
# define B2F_BAD_INITIAL_STRING      5
# define B2F_BAD_REGISTER_NAME       6

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

# define B2fError( ERROR, TEXT )                             \
                                                             \
    B2fDisplayError( basename(__FILE__), __LINE__, (ERROR), (TEXT) )

# define B2fWarning( WARNING, TEXT )                         \
                                                             \
    B2fDisplayWarning( basename(__FILE__), __LINE__, (WARNING), (TEXT) )

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

extern void B2fDisplayError  ();
extern void B2fDisplayWarning();

# endif
