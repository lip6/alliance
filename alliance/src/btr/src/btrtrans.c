/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                     Btr                           |
|                                                             |
| File    :            Btr Transition Relation                |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <memory.h>
# include <string.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "btr.h"
# include "btrtrans.h"
# include "btrerror.h"

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
|                 Btr Image Transition Function               |
|                                                             |
\------------------------------------------------------------*/

bddnode *imagebtrtransrel( TransRel, StateSet )

   btrtransrel *TransRel;
   bddnode      *StateSet;
{
  bddnode   *BddImage;
  btrvarrel *VarRel;
  long       NumberVar;
  long       Variable;

  setbddlocalsystem( TransRel->BDD_SYSTEM );

  NumberVar = TransRel->NUMBER_VAR;

  if ( NumberVar == 0 )
  {
    btrerror( BTR_BAD_VAR_ERROR, NumberVar );
  }

  BddImage = BddLocalSystem->ONE;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    VarRel   = &TransRel->VAR_REL[ Variable ];
    BddImage = applybddnode( (bddsystem *)0, ABL_AND, 
                             decbddrefext( BddImage ), VarRel->REL );
  }

  BddImage = relprodbddnodeassoc( (bddsystem *)0,
                                  decbddrefext( BddImage ), StateSet,
                                  TransRel->VAR_ASSOC );

  BddImage = substbddnodeassoc( (bddsystem *)0, 
                                decbddrefext( BddImage ),
                                TransRel->PRIME_ASSOC );
  return( BddImage );
}

/*------------------------------------------------------------\
|                                                             |
|                 Btr Create Transition Relation              |
|                                                             |
\------------------------------------------------------------*/

btrtransrel *createbtrtransrel( BddSystem, MaxVar )

  bddsystem  *BddSystem;
  bddvar      MaxVar;
{
  btrtransrel *TransRel;

  setbddlocalsystem( BddSystem );

  if ( MaxVar >= BDD_MAX_VAR )
  {
    btrerror( BTR_BAD_VAR_ERROR, MaxVar );
  }

  TransRel = allocbtrtransrel();

  TransRel->BDD_SYSTEM = BddLocalSystem;

  TransRel->MAX_VAR = MaxVar;
  TransRel->VAR_REL = allocbtrvarrel( MaxVar );

  TransRel->PRIME_ASSOC = addbddassoc( (bddsystem *)0 );
  TransRel->VAR_ASSOC   = addbddassoc( (bddsystem *)0 );

  return( TransRel );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr Add Transition Relation              |
|                                                             |
\------------------------------------------------------------*/

void addbtrtransrel( TransRel, VarNode, PrimeNode, FuncNode )

  btrtransrel *TransRel;
  bddnode     *VarNode;
  bddnode     *PrimeNode;
  bddnode     *FuncNode;
{
  btrvarrel *VarRel;
  bddnode   *RelNode;
  bddvar     VarPrime;
  bddvar     Variable;
  long       MaxVar;
  long       NumberVar;

  setbddlocalsystem( TransRel->BDD_SYSTEM );

  NumberVar = TransRel->NUMBER_VAR;
  MaxVar    = TransRel->MAX_VAR;

  if ( NumberVar >= BDD_MAX_VAR )
  {
    btrerror( BTR_NO_MORE_VAR_ERROR, 0 );
  }

  if ( NumberVar == MaxVar )
  {
    MaxVar = MaxVar * BTR_VAR_STRETCH_FACTOR;

    if ( MaxVar >= BDD_MAX_VAR )
    {
      MaxVar = BDD_MAX_VAR - 1;
    }

    VarRel = resizebtrvarrel( TransRel->VAR_REL,
                              TransRel->MAX_VAR, MaxVar );

    TransRel->VAR_REL = VarRel;
    TransRel->MAX_VAR = MaxVar;
  }
  else
  {
    VarRel = TransRel->VAR_REL;
  }

  VarPrime = BddLocalSystem->INDEX_TO_VAR[ PrimeNode->INDEX ];
  Variable = BddLocalSystem->INDEX_TO_VAR[ VarNode->INDEX   ];

  addbddnodeassoc( (bddsystem *)0, TransRel->PRIME_ASSOC, VarPrime, VarNode   );
  addbddnodeassoc( (bddsystem *)0, TransRel->VAR_ASSOC  , Variable, PrimeNode );

  RelNode = applybddnode( (bddsystem *)0, ABL_NXOR, FuncNode, PrimeNode );

  VarRel[ NumberVar ].REL   = RelNode;
  VarRel[ NumberVar ].VAR   = VarNode;
  VarRel[ NumberVar ].PRIME = PrimeNode;

  TransRel->NUMBER_VAR++;
}

/*------------------------------------------------------------\
|                                                             |
|                   Btr Reset Transition Relation             |
|                                                             |
\------------------------------------------------------------*/

void resetbtrtransrel( TransRel )

  btrtransrel *TransRel;
{
  btrvarrel *VarRel;
  long        Variable;
  long        NumberVar;
 
  setbddlocalsystem( TransRel->BDD_SYSTEM );

  NumberVar = TransRel->NUMBER_VAR;
  VarRel    = TransRel->VAR_REL;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    decbddrefext( VarRel[ Variable ].REL );
  }

  memset( (void *)VarRel, 0,
          (size_t)NumberVar * sizeof( btrvarrel ) );

  TransRel->NUMBER_VAR = 0;

  resetbddassoc( (bddsystem *)0, TransRel->PRIME_ASSOC );
  resetbddassoc( (bddsystem *)0, TransRel->VAR_ASSOC   );
}

/*------------------------------------------------------------\
|                                                             |
|                 Btr Destroy Transition Relation             |
|                                                             |
\------------------------------------------------------------*/

void destroybtrtransrel( TransRel )

  btrtransrel *TransRel;
{
  btrvarrel *VarRel;
  long       Variable;
  long       NumberVar;

  setbddlocalsystem( TransRel->BDD_SYSTEM );
  
  NumberVar = TransRel->NUMBER_VAR;
  VarRel    = TransRel->VAR_REL;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    decbddrefext( VarRel[ Variable ].REL );
  }

  freebtrvarrel( TransRel->VAR_REL );
  freebtrtransrel( TransRel );

  delbddassoc( (bddsystem *)0, TransRel->PRIME_ASSOC );
  delbddassoc( (bddsystem *)0, TransRel->VAR_ASSOC   );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr View Transition Relation             |
|                                                             |
\------------------------------------------------------------*/

void viewbtrtransrel( TransRel, FuncView )

  btrtransrel *TransRel;
  void         (*FuncView)();
{
  btrvarrel *VarRel;
  long       Variable;
  long       NumberVar;
  
  fprintf( stdout, "--> BtrTransRel\n" );

  fprintf( stdout, "  MAX_VAR    : %ld\n", (long)TransRel->MAX_VAR    );
  fprintf( stdout, "  NUMBER_VAR : %ld\n", (long)TransRel->NUMBER_VAR );

  NumberVar = TransRel->NUMBER_VAR;
  VarRel    = TransRel->VAR_REL;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    fprintf( stdout, "  VAR_REL[ %ld ] : ", Variable );

    if ( FuncView )
    {
      (*FuncView)( &VarRel[ Variable ] );
    }
    else
    {
      fprintf( stdout, "\n" );
    }
  }

  fprintf( stdout, "<-- BtrTransRel\n" );
}
