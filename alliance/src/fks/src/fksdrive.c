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
| Tool    :                     FSM                           |
|                                                             |
| File    :                   fksdrive.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   04.12.96                        |
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
# include "fsm.h"

# include <stdio.h>
# include <memory.h>
# include <stdlib.h>
# include <string.h>
# include "fkserror.h"
# include "fksdrive.h"

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

  static FILE       *KissFile;

  static char       *KissBitString;
  static long        KissBitLength;

  static char       *KissStateFrom;
  static char       *KissStateTo;

  static long        KissBitInput;

  static bddsystem  *KissBddSystem;
  static bddcircuit *KissBddCircuit;

  static long        KissNumberLine;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       KissMakeFsmBddNode                    |
|                                                             |
\------------------------------------------------------------*/

static void KissMakeFsmBddNode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmin_list     *ScanIn;
  fsmout_list    *ScanOut;
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;
  fsmlocout_list *ScanLocout;
  bddnode        *BddNode;
  bddnode        *BddNodeOut;
  bddnode        *BddNodeLocOn;
  bddnode        *BddNodeLocOff;
  bddnode        *BddNodeLocDc;
  bddnode        *BddNodeLocout;

  KissBddSystem  = createbddsystem( FKS_BDD_MODEL_VAR, FKS_BDD_MODEL_OPER, 
                                    FKS_BDD_MAX_VAR  , FKS_BDD_MAX_NODE );

  KissBddCircuit = createbddcircuit( FsmFigure->NAME,
                                     FsmFigure->NUMBER_IN,
                                     FsmFigure->NUMBER_OUT, KissBddSystem );

  for ( ScanIn  = FsmFigure->IN;
        ScanIn != (fsmin_list *)0;
        ScanIn  = ScanIn->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanIn->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  for ( ScanOut  = FsmFigure->OUT;
        ScanOut != (fsmout_list *)0;
        ScanOut  = ScanOut->NEXT )
  {
    addbddcircuitin( (bddcircuit *)0, ScanOut->NAME,
                     (bddindex    )0, BDD_IN_MODE_LAST );
  }

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    ScanTrans->USER = (void *)addbddcircuitabl( (bddcircuit *)0, ScanTrans->ABL );
  }

  ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    BddNode = BddLocalSystem->ONE;

    for ( ScanLocout  = ScanState->LOCOUT;
          ScanLocout != (fsmlocout_list *)0;
          ScanLocout  = ScanLocout->NEXT )
    {
      BddNodeOut = searchbddcircuitin( (bddcircuit *)0, ScanLocout->OUT->NAME );

      if ( ScanLocout->ABL != (ablexpr    *)0 )
      {
        BddNodeLocOn = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL );
      }
      else
      {
        BddNodeLocOn = BddLocalSystem->ZERO;
      }

      if ( ScanLocout->ABL_DC != (ablexpr    *)0 )
      {
        BddNodeLocDc  = addbddcircuitabl( (bddcircuit *)0, ScanLocout->ABL_DC );
        BddNodeLocOff = applybddnode( (bddsystem *)0, ABL_OR, 
                                      BddNodeLocDc  , BddNodeOut );
      }
      else
      {
        BddNodeLocDc  = BddLocalSystem->ZERO;
        BddNodeLocOff = BddNodeOut;
      }

      BddNodeLocOff = applybddnode( (bddsystem *)0, ABL_NOR, 
                                    decbddrefext( BddNodeLocOff ),
                                    BddNodeLocOn );

      BddNodeLocOn  = applybddnode( (bddsystem *)0, ABL_AND, 
                                    decbddrefext( BddNodeLocOn  ),
                                    BddNodeOut );

      BddNodeLocout = applybddnode( (bddsystem *)0, ABL_OR , 
                                    decbddrefext( BddNodeLocOn  ), 
                                    decbddrefext( BddNodeLocOff ) );

      BddNodeLocout = applybddnode( (bddsystem *)0, ABL_OR, 
                                    decbddrefext( BddNodeLocout ),
                                    decbddrefext( BddNodeLocDc  ) );

      BddNode = applybddnode( (bddsystem *)0, ABL_AND, 
                              decbddrefext( BddNode       ),
                              decbddrefext( BddNodeLocout ) );
    }

    BddNode = addbddcircuitout( (bddcircuit *)0, ScanState->NAME, BddNode );

    ScanState->USER = (void *)BddNode;
    ScanState = ScanState->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      KissDestroyFsmBddNode                  |
|                                                             |
\------------------------------------------------------------*/

static void KissDestroyFsmBddNode( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmstate_list  *ScanState;
  fsmtrans_list  *ScanTrans;

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    ScanTrans->USER = (void *)0;
  }

  ScanState = FsmFigure->STATE;

  while ( ScanState != (fsmstate_list *)0 )
  {
    ScanState->USER = (void *)0;
    ScanState = ScanState->NEXT;
  }

  destroybddcircuit( KissBddCircuit );
  destroybddsystem( KissBddSystem );
}

/*------------------------------------------------------------\
|                                                             |
|                      KissDriveBitString                     |
|                                                             |
\------------------------------------------------------------*/

static void KissDriveBitString( BddNode )

  bddnode *BddNode;
{
  bddindex Index;
  char     Cut;

  if ( BddNode->INDEX < BDD_INDEX_MIN )
  {
    if ( BddNode->INDEX == BDD_INDEX_ONE )
    {
      KissNumberLine++;

      Cut = KissBitString[ KissBitInput ];
      KissBitString[ KissBitInput ] = '\0';;

      fprintf( KissFile, "%s %s %s", 
               KissBitString, KissStateFrom, KissStateTo );

      KissBitString[ KissBitInput ] = Cut;

      fprintf( KissFile, " %s\n", KissBitString + KissBitInput );
    }
  }
  else
  {
    Index = BddNode->INDEX - BDD_INDEX_MIN;

    KissBitString[ Index ] = '1';
    KissDriveBitString( BddNode->HIGH );
    
    KissBitString[ Index ] = '0';
    KissDriveBitString( BddNode->LOW );

    KissBitString[ Index ] = '-';
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        KissDriveFigure                      |
|                                                             |
\------------------------------------------------------------*/

static void KissDriveFigure( FsmFigure )

  fsmfig_list *FsmFigure;
{
  fsmtrans_list *ScanTrans;
  bddnode       *BddNode;
  bddnode       *BddTrans;
  bddnode       *BddOut;

  KissNumberLine = 0;
  KissBitInput   = FsmFigure->NUMBER_IN;
  KissBitLength  = KissBitInput + FsmFigure->NUMBER_OUT + 1;
  KissBitString  = (char *)autallocblock( KissBitLength );

  memset( (void *)KissBitString, '-', (size_t)( KissBitLength - 1 ) );

  FsmFigure->TRANS = (fsmtrans_list *)reverse( (chain_list *)FsmFigure->TRANS );

  for ( ScanTrans  = FsmFigure->TRANS;
        ScanTrans != (fsmtrans_list *)0;
        ScanTrans  = ScanTrans->NEXT )
  {
    KissStateFrom = ScanTrans->FROM->NAME;
    KissStateTo   = ScanTrans->TO->NAME;

    BddOut   = (bddnode *)ScanTrans->FROM->USER;
    BddTrans = (bddnode *)ScanTrans->USER;

/*\
    if ( BddOut->INDEX < BDD_INDEX_MIN )
    {
      BddNode = BddTrans;
    }
    else
\*/
    {
      BddNode = applybddnode( (bddsystem *)0 , ABL_AND, BddTrans, BddOut );
    }

    KissDriveBitString( BddNode );
  }

  FsmFigure->TRANS = (fsmtrans_list *)reverse( (chain_list *)FsmFigure->TRANS );

  autfreeblock( KissBitString  );
}

/*------------------------------------------------------------\
|                                                             |
|                        kisssavefsmfig                       |
|                                                             |
\------------------------------------------------------------*/

void kisssavefsmfig( FsmFigure )

  fsmfig_list *FsmFigure;
{
  bddsystem  *BddSystem;
  bddcircuit *BddCircuit;
  long        Position;
  long        NumberState;

  if ( FsmFigure->STACK != (fsmstack_list *)0 )
  {
    fkserror( KISS_ERROR_FSM_STACK, FsmFigure->NAME );
  }

  KissFile = mbkfopen( FsmFigure->NAME, "kiss2", "w" );

  if ( KissFile == (FILE *)0 )
  {
    fkserror( KISS_ERROR_OPEN_FILE, FsmFigure->NAME );
  }

  NumberState = FsmFigure->NUMBER_STATE;

  if ( FsmFigure->STAR_STATE != (fsmstate_list *)0 )
  {
    NumberState = NumberState - 1;
  }

  fprintf( KissFile, ".i %ld\n", FsmFigure->NUMBER_IN    );
  fprintf( KissFile, ".o %ld\n", FsmFigure->NUMBER_OUT   );
  fprintf( KissFile, ".s %ld\n", NumberState             );

  if ( FsmFigure->FIRST_STATE != (fsmstate_list *)0 )
  {
    fprintf( KissFile, ".r %s\n", FsmFigure->FIRST_STATE->NAME );
  }

  Position = ftell( KissFile );
  fprintf( KissFile, ".p           \n" );

  BddSystem  = BddLocalSystem;
  BddCircuit = BddLocalCircuit;

  KissMakeFsmBddNode( FsmFigure );
  KissDriveFigure( FsmFigure );
  KissDestroyFsmBddNode( FsmFigure );

  BddLocalSystem  = BddSystem;
  BddLocalCircuit = BddCircuit;

  fprintf( KissFile, ".e\n" );

  fseek( KissFile, Position, 0 );
  fprintf( KissFile, ".p %ld\n", KissNumberLine );

  fclose( KissFile );
}
