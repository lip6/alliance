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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Bdd                           |
|                                                             |
| File    :                   bddlog.c                        |
|                                                             |
| Date    :                   03.12.96                        |
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
# include "abl.h"
# include "bdd.h"

# include <stdio.h>
# include <string.h>
# include "bddlog.h"
# include "bdderror.h"

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
|                         Get Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Get Bdd Node One                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *getlogbddnodeone()
{
  return( BddLocalSystem->ONE );
}

/*------------------------------------------------------------\
|                                                             |
|                         Get Bdd Node Zero                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *getlogbddnodezero()
{
  return( BddLocalSystem->ZERO );
}

/*------------------------------------------------------------\
|                                                             |
|                         Get Bdd Node Term                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *getlogbddnodeterm( Index )

  bddindex Index;
{
  if ( Index < BddLocalSystem->NUMBER_INDEX )
  {
    return( getbddvarnodebyindex( (bddsystem *)0, Index ) );
  }

  return( addbddvarfirst( (bddsystem *)0 ) );
}

/*------------------------------------------------------------\
|                                                             |
|                         Get Bdd Node Not                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *getlogbddnodenot( BddNode )

  bddnode *BddNode;
{
  return( applybddnodenot( (bddsystem *)0, BddNode ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Apply Bdd Node                       |
|                                                             |
\------------------------------------------------------------*/

bddnode *applylogbddnode( Oper, BddNode1, BddNode2 )

  long     Oper;
  bddnode *BddNode1;
  bddnode *BddNode2;
{
  return( applybddnode( (bddsystem *)0, Oper, BddNode1, BddNode2 ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Convert Bdd Node                     |
|                                                             |
\------------------------------------------------------------*/

ablexpr *convertlogbddnode( BddCircuit, BddNode )

  bddcircuit *BddCircuit;
  bddnode    *BddNode;
{
  return( convertbddnodeabl( (bddsystem *)0, BddCircuit->NAME_IN,
                             (bddindex  *)0, BddNode ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Bdd Circuit Input                |
|                                                             |
\------------------------------------------------------------*/

short addlogbddcircuitin( BddCircuit, Name )

  bddcircuit *BddCircuit;
  char       *Name;
{
  bddnode *BddNode;

  BddNode = addbddcircuitin( BddCircuit, Name, 0, BDD_IN_MODE_LAST );

  if ( BddNode == (bddnode *)0 )
  {
    return( 0 );
  }

  return( BddNode->INDEX );
}

