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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  cut.h                            |
|                                                             |
| Authors :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.08.93                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef LYNX_CUTELBOW
#define LYNX_CUTELBOW

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define LYNX_CUT            (long)0x00200000
# define LYNX_DELETED        (long)0x00400000

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/

# define IsLynxCut( Rectangle )            \
                                           \
   ( ( Rectangle )->FLAGS & LYNX_CUT       )

# define SetLynxCut( Rectangle )           \
                                           \
   ( ( Rectangle )->FLAGS |= LYNX_CUT      )

# define ClearLynxCut( Rectangle )         \
                                           \
   ( ( Rectangle )->FLAGS &= ~LYNX_CUT     )


# define IsLynxDeleted( Rectangle )        \
                                           \
   ( ( Rectangle )->FLAGS & LYNX_DELETED   )

# define SetLynxDeleted( Rectangle )       \
                                           \
   ( ( Rectangle )->FLAGS |= LYNX_DELETED  )

# define ClearLynxDeleted( Rectangle )     \
                                           \
   ( ( Rectangle )->FLAGS &= ~LYNX_DELETED )

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

  extern long  LynxCutTransistorElbow __P((rdsfig_list*, rdswindow*, char ));
  extern void  LynxInitializeCutCxTable __P((void));

# endif
