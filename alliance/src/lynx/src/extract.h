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
| File    :                  extract.h                        |
|                                                             |
| Authors :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef LYNX_EXTRACT
# define LYNX_EXTRACT

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_TREAT_EQUI     (long)0x00080000
# define LYNX_END_EQUI       (long)0x00100000

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Lynx Equi                          |
|                                                             |
\------------------------------------------------------------*/

# define IsLynxEqui( Rectangle )              \
                                              \
   ( ( Rectangle )->FLAGS & LYNX_TREAT_EQUI   )

# define SetLynxEqui( Rectangle )             \
                                              \
   ( ( Rectangle )->FLAGS |= LYNX_TREAT_EQUI  )

# define ClearLynxEqui( Rectangle )           \
                                              \
   ( ( Rectangle )->FLAGS &= ~LYNX_TREAT_EQUI )

/*------------------------------------------------------------\
|                                                             |
|                       Lynx End Equi                         |
|                                                             |
\------------------------------------------------------------*/

# define IsLynxEndEqui( Rectangle )             \
                                                \
   ( ( Rectangle )->FLAGS & LYNX_END_EQUI       )

# define SetLynxEndEqui( Rectangle )            \
                                                \
   ( ( Rectangle )->FLAGS |= LYNX_END_EQUI      )

# define ClearLynxEndEqui( Rectangle )          \
                                                \
   ( ( Rectangle )->FLAGS &= ~LYNX_END_EQUI     )

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

  extern long LynxBuildEqui();
  extern void LynxDumpAllEqui();
  extern void LynxDumpEqui();

# endif
