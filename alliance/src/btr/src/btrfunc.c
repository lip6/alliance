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
| File    :            Btr Transition Function                |
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
# include <string.h>
# include <memory.h>
# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "btr.h"
# include "btrfunc.h"
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

  static btrvarfunc  *BtrLocalVarFunc   = (btrvarfunc  *)0;
  static btrvarorder *BtrLocalVarOrder  = (btrvarorder *)0;
  static long         BtrLocalNumberVar = 0;
  static long         BtrLocalIndexVar  = 0;

  static long        *BtrLocalCostArray  = (long *)0;
  static long         BtrLocalCostLength = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Local Btr PreImage Transition Function      |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_preimagebtrtransfunc( StateSet )

  bddnode *StateSet;
{
  btrvarfunc *VarFunc;
  bddnode    *BddSetIn;
  bddnode    *BddSetOut;
  bddnode    *BddResult;
  bddnode    *BddHigh;
  bddnode    *BddLow;

  if ( StateSet->INDEX < BDD_INDEX_MIN )
  {
    return( StateSet );
  }

  VarFunc = &BtrLocalVarFunc[ BtrLocalVarOrder[ BtrLocalIndexVar ] ];

  BddSetIn  = decbddrefext( applybddnodenot( (bddsystem *)0, StateSet ) );
  BddSetIn  = applybddnode( (bddsystem *)0, ABL_OR  , VarFunc->VAR , BddSetIn );
  BddSetOut = applybddnode( (bddsystem *)0, ABL_NAND, VarFunc->VAR , StateSet );

  BtrLocalIndexVar++;

  if ( BtrLocalIndexVar >= BtrLocalNumberVar )
  {
/*
** StateSet => Rn,  Fn
*/
    if ( BddSetIn->INDEX == BDD_INDEX_ONE )
    {
      BddResult = incbddrefext( VarFunc->FUNC );
    }
    else
/*
** StateSet => ! Rn, ! Fn
*/
    if ( BddSetOut->INDEX == BDD_INDEX_ONE )
    {
      BddResult = applybddnodenot( (bddsystem *)0, VarFunc->FUNC );
    }
    else
    {
      BddResult = BddLocalSystem->ONE;
    }
  }
  else
  {
/*
** StateSet => Ri, Fi . Img( StateSet )
*/
    if ( BddSetIn->INDEX == BDD_INDEX_ONE )
    {
      BddResult = decbddrefext( loc_preimagebtrtransfunc( StateSet ) );
      BddResult = applybddnode( (bddsystem *)0, ABL_AND, VarFunc->FUNC, BddResult );
    }
    else
/*
** StateSet => ! Ri, ! Fi . Img( StateSet )
*/
    if ( BddSetOut->INDEX == BDD_INDEX_ONE )
    {
      BddLow  = loc_preimagebtrtransfunc( StateSet );
      BddHigh = applybddnodenot( (bddsystem *)0, VarFunc->FUNC );

      BddResult = applybddnode( (bddsystem *)0, ABL_AND, 
                                decbddrefext( BddHigh ), 
                                decbddrefext( BddLow  ) );
    }
    else
    {
/*
**  Fi . Img( StateSet . Ri ) +  ! Fi . Img( StateSet . ! Ri )
*/
      BddSetOut = applybddnodenot( (bddsystem *)0, decbddrefext( BddSetOut ) );
      BddSetIn  = applybddnodenot( (bddsystem *)0, decbddrefext( BddSetIn  ) );

      BddHigh = applybddnodenot( (bddsystem *)0, VarFunc->FUNC );
      BddLow  = loc_preimagebtrtransfunc( BddSetIn  );
      BddLow  = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddHigh ),
                              decbddrefext( BddLow  ) );

      BddHigh = decbddrefext( loc_preimagebtrtransfunc( BddSetOut ) );
      BddHigh = applybddnode( (bddsystem *)0, ABL_AND, VarFunc->FUNC, BddHigh );

      BddResult = applybddnode( (bddsystem *)0, ABL_OR,
                                decbddrefext( BddHigh ),
                                decbddrefext( BddLow  ) );
    }
  }

  BtrLocalIndexVar--;

  decbddrefext( BddSetIn  );
  decbddrefext( BddSetOut );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                 Btr PreImage Transition Function            |
|                                                             |
\------------------------------------------------------------*/

bddnode *preimagebtrtransfunc( TransFunc, StateSet )

   btrtransfunc *TransFunc;
   bddnode      *StateSet;
{
  bddnode *BddResult;

  setbddlocalsystem( TransFunc->BDD_SYSTEM );

  BtrLocalVarFunc   = TransFunc->VAR_FUNC;
  BtrLocalVarOrder  = TransFunc->VAR_ORDER;
  BtrLocalNumberVar = TransFunc->NUMBER_VAR;
  BtrLocalIndexVar  = 0;

  if ( BtrLocalNumberVar == 0 )
  {
    btrerror( BTR_BAD_VAR_ERROR, BtrLocalNumberVar );
  }

  BddResult = loc_preimagebtrtransfunc( StateSet );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                 Local Btr Image Transition Function         |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_imagebtrtransfunc( StateSet )

  bddnode *StateSet;
{
  btrvarfunc *VarFunc;
  bddnode    *BddSetIn;
  bddnode    *BddSetOut;
  bddnode    *BddResult;
  bddnode    *BddHigh;
  bddnode    *BddLow;

  if ( StateSet->INDEX == BDD_INDEX_ZERO )
  {
    return( StateSet );
  }

  VarFunc = &BtrLocalVarFunc[ BtrLocalVarOrder[ BtrLocalIndexVar ] ];

  BddSetIn  = decbddrefext( applybddnodenot( (bddsystem *)0, StateSet ) );
  BddSetIn  = applybddnode( (bddsystem *)0, ABL_OR  , VarFunc->FUNC, BddSetIn );
  BddSetOut = applybddnode( (bddsystem *)0, ABL_NAND, VarFunc->FUNC, StateSet );

  BtrLocalIndexVar++;

  if ( BtrLocalIndexVar >= BtrLocalNumberVar )
  {
/*
** StateSet => Fn,  Rn
*/
    if ( BddSetIn->INDEX == BDD_INDEX_ONE )
    {
      BddResult = incbddrefext( VarFunc->VAR );
    }
    else
/*
** StateSet => ! Fn, ! Rn
*/
    if ( BddSetOut->INDEX == BDD_INDEX_ONE )
    {
      BddResult = applybddnodenot( (bddsystem *)0, VarFunc->VAR );
    }
    else
    {
      BddResult = BddLocalSystem->ONE;
    }
  }
  else
  {
/*
** StateSet => Fi, Ri . Img( StateSet )
*/
    if ( BddSetIn->INDEX == BDD_INDEX_ONE )
    {
      BddResult = decbddrefext( loc_imagebtrtransfunc( StateSet ) );
      BddResult = applybddnode( (bddsystem *)0, ABL_AND, VarFunc->VAR, BddResult );
    }
    else
/*
** StateSet => ! Fi, ! Ri . Img( StateSet )
*/
    if ( BddSetOut->INDEX == BDD_INDEX_ONE )
    {
      BddLow  = loc_imagebtrtransfunc( StateSet );
      BddHigh = applybddnodenot( (bddsystem *)0, VarFunc->VAR );

      BddResult = applybddnode( (bddsystem *)0, ABL_AND, 
                                decbddrefext( BddHigh ), 
                                decbddrefext( BddLow  ) );
    }
    else
    {
/*
**  Ri . Img( StateSet . Fi ) +  ! Ri . Img( StateSet . ! Fi )
*/
      BddSetOut = applybddnodenot( (bddsystem *)0, decbddrefext( BddSetOut ) );
      BddSetIn  = applybddnodenot( (bddsystem *)0, decbddrefext( BddSetIn  ) );

      BddHigh = applybddnodenot( (bddsystem *)0, VarFunc->VAR );
      BddLow  = loc_imagebtrtransfunc( BddSetIn  );
      BddLow  = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddHigh ),
                              decbddrefext( BddLow  ) );

      BddHigh = decbddrefext( loc_imagebtrtransfunc( BddSetOut ) );
      BddHigh = applybddnode( (bddsystem *)0, ABL_AND, VarFunc->VAR, BddHigh );

      BddResult = applybddnode( (bddsystem *)0, ABL_OR,
                                decbddrefext( BddHigh ),
                                decbddrefext( BddLow  ) );
    }
  }

  BtrLocalIndexVar--;

  decbddrefext( BddSetIn  );
  decbddrefext( BddSetOut );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                 Btr Image Transition Function               |
|                                                             |
\------------------------------------------------------------*/

bddnode *imagebtrtransfunc( TransFunc, StateSet )

   btrtransfunc *TransFunc;
   bddnode      *StateSet;
{
  bddnode *BddResult;

  setbddlocalsystem( TransFunc->BDD_SYSTEM );

  BtrLocalVarFunc   = TransFunc->VAR_FUNC;
  BtrLocalVarOrder  = TransFunc->VAR_ORDER;
  BtrLocalNumberVar = TransFunc->NUMBER_VAR;
  BtrLocalIndexVar  = 0;

  if ( BtrLocalNumberVar == 0 )
  {
    btrerror( BTR_BAD_VAR_ERROR, BtrLocalNumberVar );
  }

  BddResult = loc_imagebtrtransfunc( StateSet );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                 Btr Creates Transition Function             |
|                                                             |
\------------------------------------------------------------*/

btrtransfunc *createbtrtransfunc( BddSystem, MaxVar )

  bddsystem  *BddSystem;
  bddvar      MaxVar;
{
  btrtransfunc *TransFunc;

  setbddlocalsystem( BddSystem );

  if ( MaxVar >= BDD_MAX_VAR )
  {
    btrerror( BTR_BAD_VAR_ERROR, MaxVar );
  }

  TransFunc = allocbtrtransfunc();

  TransFunc->BDD_SYSTEM = BddLocalSystem;

  TransFunc->MAX_VAR   = MaxVar;
  TransFunc->VAR_FUNC  = allocbtrvarfunc( MaxVar );
  TransFunc->VAR_ORDER = allocbtrvarorder( MaxVar );

  return( TransFunc );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr Add Transition Function              |
|                                                             |
\------------------------------------------------------------*/

void addbtrtransfunc( TransFunc, VarNode, FuncNode )

  btrtransfunc *TransFunc;
  bddnode      *VarNode;
  bddnode      *FuncNode;
{
  btrvarfunc  *VarFunc;
  btrvarorder *VarOrder;
  long         MaxVar;
  long         NumberVar;

  NumberVar = TransFunc->NUMBER_VAR;
  MaxVar    = TransFunc->MAX_VAR;

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

    VarFunc = resizebtrvarfunc( TransFunc->VAR_FUNC,
                                TransFunc->MAX_VAR, MaxVar );

    VarOrder = resizebtrvarorder( TransFunc->VAR_ORDER,
                                  TransFunc->MAX_VAR, MaxVar );

    TransFunc->VAR_FUNC  = VarFunc;
    TransFunc->VAR_ORDER = VarOrder;
    TransFunc->MAX_VAR   = MaxVar;
  }
  else
  {
    VarFunc  = TransFunc->VAR_FUNC;
    VarOrder = TransFunc->VAR_ORDER;
  }

  VarFunc [ NumberVar ].FUNC = incbddrefext( FuncNode );
  VarFunc [ NumberVar ].VAR  = VarNode;
  VarOrder[ NumberVar ]      = NumberVar;

  TransFunc->NUMBER_VAR++;
}

/*------------------------------------------------------------\
|                                                             |
|                   Btr Reset Transition Function             |
|                                                             |
\------------------------------------------------------------*/

void resetbtrtransfunc( TransFunc )

  btrtransfunc *TransFunc;
{
  btrvarfunc  *VarFunc;
  btrvarorder *VarOrder;
  long         Variable;
  long         NumberVar;
 
  NumberVar = TransFunc->NUMBER_VAR;
  VarFunc   = TransFunc->VAR_FUNC;
  VarOrder  = TransFunc->VAR_ORDER;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    decbddrefext( VarFunc[ Variable ].FUNC );
  }

  memset( (void *)VarFunc, 0,
          (size_t)NumberVar * sizeof( btrvarfunc ) );

  memset( (void *)VarOrder, 0,
          (size_t)NumberVar * sizeof( btrvarorder ) );

  TransFunc->NUMBER_VAR = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                 Btr Destroys Transition Function            |
|                                                             |
\------------------------------------------------------------*/

void destroybtrtransfunc( TransFunc )

  btrtransfunc *TransFunc;
{
  btrvarfunc *VarFunc;
  long        Variable;
  long        NumberVar;
  
  NumberVar = TransFunc->NUMBER_VAR;
  VarFunc   = TransFunc->VAR_FUNC;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    decbddrefext( VarFunc[ Variable ].FUNC );
  }

  freebtrvarfunc( TransFunc->VAR_FUNC );
  freebtrvarorder( TransFunc->VAR_ORDER );
  freebtrtransfunc( TransFunc );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr View Transition Function             |
|                                                             |
\------------------------------------------------------------*/

void viewbtrtransfunc( TransFunc, FuncView )

  btrtransfunc *TransFunc;
  void         (*FuncView)();
{
  btrvarfunc   *VarFunc;
  long          Variable;
  long          NumberVar;
  
  fprintf( stdout, "--> BtrTransFunc\n" );

  fprintf( stdout, "  MAX_VAR    : %ld\n", (long)TransFunc->MAX_VAR    );
  fprintf( stdout, "  NUMBER_VAR : %ld\n", (long)TransFunc->NUMBER_VAR );

  NumberVar = TransFunc->NUMBER_VAR;
  VarFunc   = TransFunc->VAR_FUNC;

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    fprintf( stdout, "  VAR_FUNC[ %ld ] : ", Variable );

    if ( FuncView )
    {
      (*FuncView)( &VarFunc[ Variable ] );
    }
    else
    {
      fprintf( stdout, "\n" );
    }
  }

  fprintf( stdout, "<-- BtrTransFunc\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr Local Reorder Compare                |
|                                                             |
\------------------------------------------------------------*/

static long loc_ordercompare( ValueArray, Index1, Index2 )

  long *ValueArray;
  long  Index1;
  long  Index2;
{
  return( ValueArray[ Index1 ] - ValueArray[ Index2 ] );
}

/*------------------------------------------------------------\
|                                                             |
|                    Btr Reorder Transition Function          |
|                                                             |
\------------------------------------------------------------*/

void reorderbtrtransfunc( TransFunc )

  btrtransfunc *TransFunc;
{
  btrvarfunc  *VarFunc;
  btrvarorder *VarOrder;
  long         NumberVar;
  long         Variable;
  bddnode     *BddNode;

  NumberVar = TransFunc->NUMBER_VAR;
  VarFunc   = TransFunc->VAR_FUNC;
  VarOrder  = TransFunc->VAR_ORDER;

  setbddlocalsystem( TransFunc->BDD_SYSTEM );

  if ( BtrLocalCostArray == (long *)0 )
  {
    BtrLocalCostArray  = (long *)autallocblock( sizeof(long) * NumberVar );
    BtrLocalCostLength = NumberVar;
  }
  else
  if ( BtrLocalCostLength < NumberVar )
  {
    BtrLocalCostArray = 

      (long *)autresizeblock( (char *)BtrLocalCostArray,
                              sizeof( long ) * BtrLocalCostLength,
                              sizeof( long ) * NumberVar );

    BtrLocalCostLength = NumberVar;
  }

  for ( Variable = 0; Variable < NumberVar; Variable++ )
  {
    BddNode = VarFunc[ Variable ].FUNC;
    BtrLocalCostArray[ Variable ] = getbddnodesize( (bddsystem *)0, BddNode );
    
  }

  sortautarray( BtrLocalCostArray, VarOrder, NumberVar, loc_ordercompare );
}
