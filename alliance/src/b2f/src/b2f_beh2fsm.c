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
| Tool    :                     B2F                           |
|                                                             |
| File    :                  b2f_beh2fsm.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.11.94                        |
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
# include "btr.h"
# include "btr.h"
# include "fsm.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <memory.h>
# include "b2f_error.h"
# include "b2f_beh2fsm.h"

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

  static char          *B2fStateArray   = (char          *)0;
  static char          *B2fStateName    = (char          *)0;
  static long          *B2fStateIndex   = (long          *)0;
  static authtable     *B2fHashState    = (authtable     *)0;
  static authtable     *B2fHashOutput   = (authtable     *)0;

  static fsmstate_list *B2fCurrentState = (fsmstate_list *)0;
  static fsmfig_list   *B2fFsmFigure    = (fsmfig_list   *)0;

  static btrvarfunc    *B2fLocalVarFunc   = (btrvarfunc *)0;
  static long           B2fLocalNumberVar = 0;
  static long           B2fLocalIndexVar  = 0;
  static bddnode       *B2fLocalState     = (bddnode    *)0;

  static long           B2fNumberRegister = 0;
  static char          *B2fRegisterName   = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Convert State Name                   |
|                                                             |
\------------------------------------------------------------*/

char *B2fConvertStateName( StateName )

  char *StateName;
{
  long Index;
  long Target;
  long Sum;

  Target = 1;
  Sum    = 0;

  for ( Index = 0; StateName[ Index ]; Index++ )
  {
    Sum = ( Sum << 1 ) + ( StateName[ Index ] - '0' );

    if ( ( Index & 0x3 ) == 0x3 )
    {
      if ( Sum > 9 ) B2fStateName[ Target++ ] = Sum + 'A' - 10;
      else           B2fStateName[ Target++ ] = Sum + '0';

      Sum = 0;
    }
  }

  if ( Index & 0x3 )
  {
    if ( Sum > 9 ) B2fStateName[ Target++ ] = Sum + 'A' - 10;
    else           B2fStateName[ Target++ ] = Sum + '0';
  }

  B2fStateName[ Target ] = '\0';

  StateName = namealloc( B2fStateName );

  return( StateName );
}

/*------------------------------------------------------------\
|                                                             |
|                        Treat Add State                      |
|                                                             |
\------------------------------------------------------------*/

fsmstate_list *B2fAddFsmState( StateName )

  char *StateName;
{
  authelem      *Element;
  fsmstate_list *FsmState;

  StateName = B2fConvertStateName( StateName );
  Element = searchauthelem( B2fHashState, StateName );

  if ( Element == (authelem *)0 )
  {
# ifdef DEBUG
fprintf( stdout, "%s *\n", StateName );
# endif
    FsmState = addfsmstate( B2fFsmFigure, StateName );
    addauthelem( B2fHashState, StateName, (long)FsmState );
  }
  else
  {
# ifdef DEBUG
fprintf( stdout, "%s\n", StateName );
# endif
    FsmState = (fsmstate_list *)Element->VALUE;
  }

  return( FsmState );
}

/*------------------------------------------------------------\
|                                                             |
|                        Treat State Image                    |
|                                                             |
\------------------------------------------------------------*/

void B2fAddFsmCurrentState( BddNode )

  bddnode *BddNode;
{
  long Index;

  if ( BddNode->INDEX < BDD_INDEX_MIN )
  {
    if ( BddNode->INDEX == BDD_INDEX_ONE )
    {
      B2fCurrentState = B2fAddFsmState( B2fStateArray );
    }
  }
  else
  {
    Index = B2fStateIndex[ BddNode->INDEX - BDD_INDEX_MIN ];

    B2fStateArray[ Index ] = '1';
    B2fAddFsmCurrentState( BddNode->HIGH );

    B2fStateArray[ Index ] = '0';
    B2fAddFsmCurrentState( BddNode->LOW  );

    B2fStateArray[ Index ] = '*';
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Add Fsm Locout                       |
|                                                             |
\------------------------------------------------------------*/

void B2fAddFsmLocalOutput( FsmState, BddState )

  fsmstate_list *FsmState;
  bddnode       *BddState;
{
  authelem    *Element;
  fsmout_list *FsmOut;
  bddnode     *BddOut;
  chain_list  *ExprOut;

  if ( FsmState->LOCOUT == (fsmlocout_list *)0 )
  {
    for ( FsmOut  = B2fFsmFigure->OUT;
          FsmOut != (fsmout_list *)0;
          FsmOut  = FsmOut->NEXT )
    {
      Element = searchauthelem( B2fHashOutput, FsmOut->NAME );
      BddOut  = (bddnode *)Element->VALUE;
      BddOut  = cofactorbddnode( (bddsystem *)0, BddOut, BddState );
      ExprOut = convertbddcircuitabl( (bddcircuit *)0, decbddrefext( BddOut ) );

      addfsmlocout( FsmState, FsmOut, ExprOut, (chain_list *)0 );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      B2f View Bdd Node                      |
|                                                             |
\------------------------------------------------------------*/

void B2fAddFsmNextState( BddNode )

  bddnode *BddNode;
{
  fsmstate_list *NextState;
  chain_list    *Expr;

  BddNode = cofactorbddnode( (bddsystem *)0, BddNode, B2fLocalState );
  Expr = convertbddcircuitabl( (bddcircuit *)0, decbddrefext( BddNode ) );

  NextState = B2fAddFsmState( B2fStateArray );

  addfsmtrans( B2fFsmFigure, B2fCurrentState, NextState, Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                 Local B2f Image Transition Function         |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_imageb2ftransfunc( StateSet )

  bddnode *StateSet;
{
  btrvarfunc *VarFunc;
  bddnode    *BddSetIn;
  bddnode    *BddSetOut;
  bddnode    *BddResult;
  bddnode    *BddHigh;
  bddnode    *BddLow;
  long        Index;

  if ( StateSet->INDEX == BDD_INDEX_ZERO )
  {
    return( StateSet );
  }

  VarFunc = &B2fLocalVarFunc[ B2fLocalIndexVar ];
  Index   = B2fStateIndex[ VarFunc->VAR->INDEX - BDD_INDEX_MIN ];

  BddSetIn  = decbddrefext( applybddnodenot( (bddsystem *)0, StateSet ) );
  BddSetIn  = applybddnode( (bddsystem *)0, ABL_OR  , VarFunc->FUNC, BddSetIn );
  BddSetOut = applybddnode( (bddsystem *)0, ABL_NAND, VarFunc->FUNC, StateSet );

  B2fLocalIndexVar++;

  if ( B2fLocalIndexVar >= B2fLocalNumberVar )
  {
/*
** StateSet => Fn,  Rn
*/
    if ( BddSetIn->INDEX == BDD_INDEX_ONE )
    {
      B2fStateArray[ Index ] = '1';

      B2fAddFsmNextState( StateSet );

      BddResult = incbddrefext( VarFunc->VAR );
    }
    else
/*
** StateSet => ! Fn, ! Rn
*/
    if ( BddSetOut->INDEX == BDD_INDEX_ONE )
    {
      B2fStateArray[ Index ] = '0';

      B2fAddFsmNextState( StateSet );

      BddResult = applybddnodenot( (bddsystem *)0, VarFunc->VAR );
    }
    else
    {
      BddSetOut = applybddnodenot( (bddsystem *)0, decbddrefext( BddSetOut ) );
      BddSetIn  = applybddnodenot( (bddsystem *)0, decbddrefext( BddSetIn  ) );

      B2fStateArray[ Index ] = '0';

      B2fAddFsmNextState( BddSetIn );

      B2fStateArray[ Index ] = '1';

      B2fAddFsmNextState( BddSetOut );

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
      B2fStateArray[ Index ] = '1';

      BddResult = decbddrefext( loc_imageb2ftransfunc( StateSet ) );
      BddResult = applybddnode( (bddsystem *)0, ABL_AND, VarFunc->VAR, BddResult );
    }
    else
/*
** StateSet => ! Fi, ! Ri . Img( StateSet )
*/
    if ( BddSetOut->INDEX == BDD_INDEX_ONE )
    {
      B2fStateArray[ Index ] = '0';

      BddLow  = loc_imageb2ftransfunc( StateSet );
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

      B2fStateArray[ Index ] = '0';

      BddHigh = applybddnodenot( (bddsystem *)0, VarFunc->VAR );
      BddLow  = loc_imageb2ftransfunc( BddSetIn  );
      BddLow  = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddHigh ),
                              decbddrefext( BddLow  ) );

      B2fStateArray[ Index ] = '1';

      BddHigh = decbddrefext( loc_imageb2ftransfunc( BddSetOut ) );
      BddHigh = applybddnode( (bddsystem *)0, ABL_AND, VarFunc->VAR, BddHigh );

      BddResult = applybddnode( (bddsystem *)0, ABL_OR,
                                decbddrefext( BddHigh ),
                                decbddrefext( BddLow  ) );
    }
  }

  B2fStateArray[ Index ] = '*';

  B2fLocalIndexVar--;

  decbddrefext( BddSetIn  );
  decbddrefext( BddSetOut );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                 B2f Image Transition Function               |
|                                                             |
\------------------------------------------------------------*/

bddnode *imageb2ftransfunc( TransFunc, StateSet )

   btrtransfunc *TransFunc;
   bddnode      *StateSet;
{
  bddnode *BddResult;

  setbddlocalsystem( TransFunc->BDD_SYSTEM );

  B2fLocalVarFunc   = TransFunc->VAR_FUNC;
  B2fLocalNumberVar = TransFunc->NUMBER_VAR;
  B2fLocalIndexVar  = 0;
  B2fLocalState     = StateSet;

  BddResult = loc_imageb2ftransfunc( StateSet );

  return( BddResult );
}

/*------------------------------------------------------------\
|                                                             |
|                        Treat State Image                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Treat Beh Initial State                  |
|                                                             |
\------------------------------------------------------------*/

bddnode *B2fTreatInitialState( BehFigure, FlagInitial, StringInitial )

   befig_list *BehFigure;
   int         FlagInitial;
   char       *StringInitial;
{
  bddnode      *BddCurrentSet;
  bddnode      *BddNode;
  bddnode      *BddReg;
  bddnode      *BddReset;
  ablexpr      *AblReset;
  bereg_list   *BehReg;
  binode_list  *BiNode;
  authtable    *HashTable;
  authelem     *Element;
  char         *ScanString;
  long          Value;
  int           Index;
  int           Scan;
  int           Length;
  int           BitZero;
  char          CurChar;
  char          Buffer[ 512 ];

  BddReset      = BddLocalSystem->ZERO;
  BddCurrentSet = BddLocalSystem->ONE;

  HashTable = createauthtable( 1000 );
/*
** Convert the reset condition to Bdd
*/
  if ( FlagInitial == 'R' )
  {
    AblReset = strablexpr( StringInitial, ABL_VIEW_INFIX );

    if ( AblReset == (ablexpr *)0 )
    {
      B2fError( B2F_ILLEGAL_RESET_CONDITION, StringInitial );
    }

    BddReset = addbddcircuitabl( (bddcircuit *)0, AblReset );
  }
  else
  if ( FlagInitial == 'I' )
/*
** Put the 'StringInitial' into hash table
*/
  {
    for ( Scan = 0; StringInitial[ Scan ] != '\0'; Scan++ )
    {
      if ( StringInitial[ Scan ] == '0' ) Value = 0;
      else
      if ( StringInitial[ Scan ] == '1' ) Value = 1;
      else
      {
        B2fError( B2F_BAD_INITIAL_STRING, StringInitial );
      }

      Index = B2fNumberRegister - (Scan + 1);

      sprintf( Buffer, "%s %d", B2fRegisterName, Index );
      addauthelem( HashTable, namealloc( Buffer ), Value );
    }

    if ( Scan != B2fNumberRegister )
    {
      B2fError( B2F_BAD_INITIAL_STRING, StringInitial );
    }
  }
  else
  if ( ( FlagInitial == 'O' ) ||
       ( FlagInitial == 'Z' ) )
  {
    if ( FlagInitial == 'O' ) Value = 0;
    else                      Value = 1;

    for ( Index = 0; Index < B2fNumberRegister; Index++ )
    {
      sprintf( Buffer, "%s %d", B2fRegisterName, Index );
      addauthelem( HashTable, namealloc( Buffer ), Value );
    }

    Length     = strlen( StringInitial );
    ScanString = StringInitial;

    for ( Scan = 0; Scan <= Length; Scan++ )
    {
      CurChar = StringInitial[ Scan ];

      if ( ( CurChar == ','  ) ||
           ( CurChar == '\0' ) )
      {
        StringInitial[ Scan ] = '\0';
        Index = atoi( ScanString );
        StringInitial[ Scan ] = CurChar;

        if ( ( Index < 0                 ) ||
             ( Index > B2fNumberRegister ) )
        {
          B2fError( B2F_BAD_INITIAL_STRING, StringInitial );
        }

        sprintf( Buffer, "%s %d", B2fRegisterName, Index );
        addauthelem( HashTable, namealloc( Buffer ), ! Value );

        ScanString = &StringInitial[ Scan + 1];
      }
    }
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BiNode  = BehReg->BINODE;
    BddReg  = searchbddcircuitin( (bddcircuit *)0, BehReg->NAME );
    BitZero = 1;
/*
** Search the initial value of the register
*/
    if ( FlagInitial == 'R' )
    {
      BddNode = cofactorbddnode( (bddsystem *)0, BiNode->VALNODE, BddReset );

      if ( BddNode == BddLocalSystem->ONE  ) BitZero = 0;
      else
      if ( BddNode != BddLocalSystem->ZERO )
      {
        B2fError( B2F_ILLEGAL_RESET_CONDITION, StringInitial );
      }
    }
    else
    if ( ( FlagInitial == 'O' ) ||
         ( FlagInitial == 'I' ) ||
         ( FlagInitial == 'Z' ) )
    {
      Element = searchauthelem( HashTable, BehReg->NAME );

      if ( Element == (authelem *)0 )
      {
        B2fError( B2F_BAD_INITIAL_STRING, StringInitial );
      }

      BitZero = ! Element->VALUE;
    }

    if ( BitZero ) BddNode = applybddnodenot( (bddsystem *)0, BddReg );
    else           BddNode = BddReg;

    BddCurrentSet = applybddnode( (bddsystem *)0, ABL_AND,
                                  decbddrefext( BddCurrentSet ),
                                  decbddrefext( BddNode       ) );
  }

  destroyauthtable( HashTable );

# ifdef DEBUG
  addbddcircuitout( (bddcircuit *)0, "init", BddCurrentSet );
  testbddcircuit( (bddcircuit *)0 );
# endif

  return( BddCurrentSet );
}

/*------------------------------------------------------------\
|                                                             |
|                        Treat Beh State                      |
|                                                             |
\------------------------------------------------------------*/

void B2fTreatState( BehFigure, FlagInitial, StringInitial )

  befig_list  *BehFigure;
  int          FlagInitial;
  char        *StringInitial;
{
  btrtransfunc *BtrTransFunc;
  bddassoc     *BddAssoc;
  bereg_list   *BehReg;
  beout_list   *BehOut;
  binode_list  *BiNode;
  bddnode      *BddNode;
  bddnode      *BddCurrentState;
  bddnode      *BddCurrentSet;
  bddnode      *BddReachedSet;
  bddnode      *BddImageSet;
  bddnode      *BddImage;
  bddvar        Variable;
  int           InitialExist;
  long          NumberIndex;
  long          StateIndex;

  NumberIndex = BddLocalSystem->NUMBER_INDEX;

  B2fHashState  = createauthtable( 1000 );
  B2fStateName  = (char *)autallocblock( ( ( B2fNumberRegister + 3 ) >> 2 ) + 2 );
  B2fStateArray = (char *)autallocblock( B2fNumberRegister + 1 );
  B2fStateName [ 0 ] = 's';

  memset( B2fStateArray, '*', B2fNumberRegister );
  B2fStateIndex = (long *)autallocblock( sizeof( long ) * NumberIndex );

  BtrTransFunc  = createbtrtransfunc( (bddsystem *)0, B2fNumberRegister );
  BddAssoc      = addbddassoc( (bddsystem *)0 );
  StateIndex    = 0;

  BddCurrentSet = B2fTreatInitialState( BehFigure, FlagInitial, StringInitial );

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    BiNode = BehReg->BINODE;

    BddNode = searchbddcircuitin( (bddcircuit *)0, BehReg->NAME );
    addbtrtransfunc( BtrTransFunc, BddNode, BiNode->VALNODE );

    Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];
    addbddnodeassoc( (bddsystem *)0, BddAssoc, Variable, BddLocalSystem->ONE );

    B2fStateIndex[ BddNode->INDEX - BDD_INDEX_MIN ] = StateIndex++;
  }

  B2fHashOutput = createauthtable( B2fFsmFigure->NUMBER_OUT << 1 );

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    addauthelem( B2fHashOutput, BehOut->NAME, (long)BehOut->NODE );
  }

  B2fAddFsmCurrentState( BddCurrentSet );
  B2fFsmFigure->FIRST_STATE = B2fCurrentState;
  SetFsmFirstState( B2fCurrentState );

  B2fAddFsmLocalOutput( B2fCurrentState, BddCurrentSet );

  BddReachedSet = incbddrefext( BddCurrentSet );

  do
  {
    BddImageSet = BddLocalSystem->ZERO;

    do
    {
      BddCurrentState = satisfybddnodeassoc( (bddsystem *)0, BddCurrentSet, BddAssoc );
# ifdef DEBUG
fprintf( stdout, "CurrentState:\n" );
# endif
      B2fAddFsmCurrentState( BddCurrentState );
      B2fAddFsmLocalOutput( B2fCurrentState, BddCurrentState );

# ifdef DEBUG
fprintf( stdout, "Image:\n" );
# endif
      BddImage = imageb2ftransfunc( BtrTransFunc, BddCurrentState );
      BddImageSet = applybddnode( (bddsystem *)0, ABL_OR, 
                                  decbddrefext( BddImageSet ),
                                  decbddrefext( BddImage    ) );
      
      BddCurrentState = applybddnodenot( (bddsystem *)0, decbddrefext( BddCurrentState ) );
      BddCurrentSet   = applybddnode( (bddsystem *)0, ABL_AND,
                                      decbddrefext( BddCurrentSet   ), 
                                      decbddrefext( BddCurrentState ) );
    }
    while ( BddCurrentSet != BddLocalSystem->ZERO );

    BddCurrentSet = applybddnodenot( (bddsystem *)0, BddReachedSet );
    BddCurrentSet = applybddnode( (bddsystem *)0, ABL_AND,
                                  decbddrefext( BddImageSet   ),
                                  decbddrefext( BddCurrentSet ) );

    BddReachedSet = applybddnode( (bddsystem *)0, ABL_OR,
                                  decbddrefext( BddReachedSet ), BddCurrentSet );
  }
  while ( BddCurrentSet != BddLocalSystem->ZERO );

  decbddrefext( BddReachedSet );

  destroybtrtransfunc( BtrTransFunc );
  delbddassoc( (bddsystem *)0, BddAssoc );

  autfreeblock( B2fStateArray );
  autfreeblock( B2fStateName  );
  autfreeblock( B2fStateIndex );

  destroyauthtable( B2fHashState  );
  destroyauthtable( B2fHashOutput );
}

/*------------------------------------------------------------\
|                                                             |
|                         Treat Beh Port                      |
|                                                             |
\------------------------------------------------------------*/

void B2fTreatPort( BehFigure )

  befig_list  *BehFigure;
{
  bepor_list  *ScanPort;
  char        *ClockName;
  char        *Vector;
  long         Index;
  int          FsmType;

  for ( ScanPort  = BehFigure->BEPOR;
        ScanPort != (bepor_list *)0;
        ScanPort  = ScanPort->NEXT )
  {
    if ( ScanPort->DIRECTION == 'I' )
    {
      addfsmin( B2fFsmFigure, ScanPort->NAME );
    }
    else
    {
      addfsmout( B2fFsmFigure, ScanPort->NAME );
    }

    Vector = vhdlablvector( ScanPort->NAME, &Index );

    switch ( ScanPort->TYPE )
    {
      case 'B' : if ( Vector != (char *)0 ) FsmType = FSM_TYPE_BIT_VEC;
                 else                       FsmType = FSM_TYPE_BIT;
      break;
      case 'M' : if ( Vector != (char *)0 ) FsmType = FSM_TYPE_MUX_VEC;
                 else                       FsmType = FSM_TYPE_MUX_BIT;
      break;
      case 'W' : if ( Vector != (char *)0 ) FsmType = FSM_TYPE_WOR_VEC;
                 else                       FsmType = FSM_TYPE_WOR_BIT;
      break;
    }

    addfsmport( B2fFsmFigure, ScanPort->NAME, ScanPort->DIRECTION, FsmType );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Treat Beh Register                  |
|                                                             |
\------------------------------------------------------------*/

void B2fTreatRegister( BehFigure )

  befig_list  *BehFigure;
{
  bddcircuit  *BddCircuit;
  bereg_list  *ScanReg;
  binode_list *BiNode;
  bddnode     *BddNode;
  chain_list  *Expr;
  chain_list  *Support;
  char        *RegName;
  long         Index;

  B2fNumberRegister = 0;
  B2fRegisterName   = (char *)0;
  BddCircuit        = BehFigure->CIRCUI;
  BddNode           = (bddnode    *)0;
  Expr              = (chain_list *)0;
/*
** Verify Register's conditions
*/
  for ( ScanReg  = BehFigure->BEREG;
        ScanReg != (bereg_list *)0;
        ScanReg  = ScanReg->NEXT )
  {
    RegName = vhdlablvector( ScanReg->NAME, &Index );

    if ( RegName == (char *)0 )
    {
      B2fError( B2F_REGISTER_NOT_A_VECTOR, ScanReg->NAME );
    }

    BiNode = ScanReg->BINODE;

    if ( BddNode == (bddnode *)0 )
    {
      Expr    = ScanReg->BIABL->CNDABL;
      BddNode = BiNode->CNDNODE;

      B2fRegisterName = RegName;
    }
    else
    if ( BiNode->CNDNODE != BddNode )
    {
      B2fError( B2F_BAD_CONDITION_REGISTER, ScanReg->NAME );
    }
    else
    if ( B2fRegisterName != RegName )
    {
      B2fError( B2F_BAD_REGISTER_NAME, B2fRegisterName );
    }

    B2fNumberRegister = B2fNumberRegister + 1;
  }

  if ( BddNode != (bddnode *)0 )
  {
    B2fFsmFigure->CLOCK_ABL = dupablexpr( Expr );
    Support = getablexprsupport( Expr, ABL_SUPPORT_CHAIN );

    if ( ( Support       == (chain_list *)0 ) ||
         ( Support->NEXT != (chain_list *)0 ) )
    {
      B2fError( B2F_BAD_CONDITION_REGISTER, RegName );
    }

    B2fFsmFigure->CLOCK = (char *)Support->DATA;
  }
  else
  {
    B2fError( B2F_NO_REGISTER, BehFigure->NAME );
  }

  B2fRegisterName   = RegName;

# ifdef DEBUG
  fprintf( stdout, "Found %ld registers\n", NumberReg );
# endif
}

/*------------------------------------------------------------\
|                                                             |
|                        Beh To Fsm Figure                    |
|                                                             |
\------------------------------------------------------------*/

fsmfig_list *B2fBeh2Fsm( BehFigure, FlagInitial, StringInitial )

  befig_list *BehFigure;
  char       *StringInitial;
  int         FlagInitial;
{
  bddcircuit  *BddCircuit;
  bddsystem   *BddSystem;

  BddSystem = createbddsystem( 100, 1000, 100, 500000 );
  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 10000, 100 );

  BehFigure->BEDLY = (beaux_list *)0;
  beh_makbdd( BehFigure, 0, 0 );

  BddCircuit = (bddcircuit *)BehFigure->CIRCUI;
# ifdef DEBUG
  testbddcircuit( BddCircuit );
# endif

  B2fFsmFigure = addfsmfig( BehFigure->NAME );

  B2fTreatPort( BehFigure );
  B2fTreatRegister( BehFigure );
  B2fTreatState( BehFigure, FlagInitial, StringInitial );

# ifdef DEBUG
  testbddcircuit( (bddcircuit *)0 );
# endif

  destroybddcircuit( BddCircuit );
  destroybddsystem( BddSystem );

  return( B2fFsmFigure );
}
