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
| File    :                  syf_tools.c                      |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H
# include FSM_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_tools.h"
# include "syf_error.h"

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
|                       SyfGetNumberBit                       |
|                                                             |
\------------------------------------------------------------*/

int SyfGetNumberBit( Value )

  long Value;
{
  long Mask;
  int  NumberBit;
  
  Mask = 1 << 31;

  for ( NumberBit = 32; NumberBit > 0; NumberBit-- )
  {
    if ( Mask & Value ) return( NumberBit );

    Mask = Mask >> 1;
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     SyfGetHamingDistance                    |
|                                                             |
\------------------------------------------------------------*/

int SyfGetHamingDistance( First, Second, Bits )

  int First;
  int Second;
  int Bits;
{
  register int Mask;
  register int Counter;
  register int Distance;

  First    = First ^ Second;
  Distance = 0;
  Mask     = 1;

  for( Counter = 0; Counter < Bits; Counter++ )
  {
    if( Mask & First ) Distance++;

    Mask = Mask << 1;
  }

  return( Distance );
}

/*------------------------------------------------------------\
|                                                             |
|                     SyfGetNumberBitSet                      |
|                                                             |
\------------------------------------------------------------*/

int SyfGetNumberBitSet( Number, Bits )

  int Number;
  int Bits;
{
  int Mask;
  int Counter;
  int BitSet;

  Mask   = 1;
  BitSet = 0;

  for( Counter = 0; Counter < Bits; Counter++ )
  {
    if( Mask & Number ) BitSet++;

    Mask = Mask << 1;
  }

  return( BitSet );
}
