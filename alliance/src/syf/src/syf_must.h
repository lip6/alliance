/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  syf_must.h                       |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef SYF_MUSTANG_H
# define SYF_MUSTANG_H

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define HEAP_FATHER( Node )      ( ( ( ( Node ) + 1 ) >> 1 ) - 1 )
# define HEAP_LEFT_CHILD( Node )  ( ( ( ( Node ) + 1 ) << 1 ) - 1 )
# define HEAP_RIGHT_CHILD( Node )   ( ( ( Node ) + 1 ) << 1 )

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

  extern void SyfFsmMustangEncode();
  extern void SyfFsmGetMustangTrans();
  extern void SyfFsmGetMustangOut();
  extern void SyfFsmGetMustangCtrl();
  extern void SyfFsmGetMustangEdge();
  extern long SyfFsmGetMustangCost();

  extern void SyfFsmGetSortArray();


# ifdef SYF_DEBUG

/*------------------------------------------------------------\
|                                                             |
|                        View Functions                       |
|                                                             |
\------------------------------------------------------------*/

  extern void SyfFsmViewMustangOut();
  extern void SyfFsmViewMustangTrans();
  extern void SyfFsmViewMustangCtrl();
  extern void SyfFsmViewMustangEdge();

# endif

# endif
