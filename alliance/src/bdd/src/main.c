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
# include <stdio.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"

int main( argc, argv )

  int argc;
  char **argv;
{
  bddcircuit *BddCircuit;
  bddsystem  *BddSystem;

  mbkenv();
  autenv();
  ablenv();
  bddenv();

  BddSystem  = createbddsystem( 10, 1000, 3, 100000 );

  if ( argc > 1 )
  {
    BddCircuit = (bddcircuit *)undumpbddcircuit( BddSystem, argv[1] );
  }
  else
  {
    BddCircuit = createbddcircuit( "Hello", 3, 3, BddSystem );
  }

  testbddcircuit( BddCircuit );

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem   );

  return( 0 );
}
