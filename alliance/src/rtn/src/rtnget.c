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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Rtl                           |
|                                                             |
| File    :                   rtnget.c                        |
|                                                             |
| Date    :                   04.07.96                        |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "rtn.h"

# include <stdio.h>
# include "rtnget.h"
# include "rtnerror.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
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
/*------------------------------------------------------------\
|                                                             |
|                      Rtl Get Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Get Rtl Symbol From Declaration             |
|                                                             |
\------------------------------------------------------------*/

rtlsym *getrtlsymdecl( Declar, Index )

  rtldecl_list *Declar;
  short         Index;
{
  vexexpr  *Atom;
  int       Position;

  Atom     = Declar->VEX_ATOM;
  Position = getvexvectorpos( Atom, Index );

  if ( Position == -1 )
  {
    rtlerror( RTL_ILLEGAL_INDEX_ERROR, GetVexAtomValue( Atom ), Index );
  }

  return( &Declar->DECL_SYM[ Position ] );
}

/*------------------------------------------------------------\
|                                                             |
|                 Get Rtl Symbol From Declaration             |
|                                                             |
\------------------------------------------------------------*/

rtlsym *getrtlsymdeclpos( Declar, Position )

  rtldecl_list *Declar;
  int           Position;
{
  vexexpr  *Atom;

  Atom = Declar->VEX_ATOM;

  if ( ( Position >= Atom->WIDTH   ) ||
       ( Position <  0             ) )
  {
    rtlerror( RTL_ILLEGAL_POSITION_ERROR, GetVexAtomValue( Atom ), Position );
  }

  return( &Declar->DECL_SYM[ Position ] );
}
