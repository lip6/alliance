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
| Tool    :                     Btr                           |
|                                                             |
| File    :                    main.c                         |
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

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "log.h"
# include "beh.h"
# include "bvl.h"
# include "bhl.h"
# include "btr.h"

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

  static char *BtrStateArray = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Btr Usage                          |
|                                                             |
\------------------------------------------------------------*/

void BtrUsage()
{
  fprintf( stderr, "Usage: btrtest [-tfip] filename\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          BtrViewBddState                    |
|                                                             |
\------------------------------------------------------------*/

void BtrViewBddState( BddNode )

  bddnode *BddNode;
{
  bddindex BddIndex;

  if ( BddNode->INDEX < BDD_INDEX_MIN )
  {
    if ( BddNode->INDEX == BDD_INDEX_ONE )
    {
      fprintf( stdout, "%s\n", BtrStateArray );
    }
  }
  else
  {
    BddIndex = BddNode->INDEX - BDD_INDEX_MIN;

    BtrStateArray[ BddIndex ] = '1';
    BtrViewBddState( BddNode->HIGH );

    BtrStateArray[ BddIndex ] = '0';
    BtrViewBddState( BddNode->LOW  );

    BtrStateArray[ BddIndex ] = '*';
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          BtrViewBddNode                     |
|                                                             |
\------------------------------------------------------------*/

void BtrViewBddNode( BddNode )

  bddnode *BddNode;
{
  chain_list *Expr;

  Expr = convertbddcircuitabl( (bddcircuit *)0, BddNode );
  viewablexpr( Expr, ABL_VIEW_VHDL );
  delablexpr( Expr );

  fprintf( stdout, "\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                          BtrViewFunc                        |
|                                                             |
\------------------------------------------------------------*/

void BtrViewFunc( VarFunc )

  btrvarfunc *VarFunc;
{
  fprintf( stdout, "\nVAR : " );
  BtrViewBddNode( VarFunc->VAR );
  fprintf( stdout, "FUNC: " );
  BtrViewBddNode( VarFunc->FUNC );
}

/*------------------------------------------------------------\
|                                                             |
|                          BtrViewRel                         |
|                                                             |
\------------------------------------------------------------*/

void BtrViewRel( VarRel )

  btrvarrel *VarRel;
{
  fprintf( stdout, "\nVAR : " );
  BtrViewBddNode( VarRel->VAR );
  fprintf( stdout, "PRIME: " );
  BtrViewBddNode( VarRel->PRIME );
  fprintf( stdout, "REL: " );
  BtrViewBddNode( VarRel->REL );
}

/*------------------------------------------------------------\
|                                                             |
|                          PreImage                           |
|                                                             |
\------------------------------------------------------------*/

void BtrPreImageFunc( BtrTransFunc, BddInitial, BddAssoc )

  btrtransfunc *BtrTransFunc;
  bddnode      *BddInitial;
  bddassoc     *BddAssoc;
{
  bddnode      *BddCurrent;
  bddnode      *BddReached;
  bddnode      *BddNew;
  bddnode      *BddPreImage;

  BddCurrent = BddInitial;

  fprintf( stdout, "Initial:\n" );
/*\
  BtrViewBddState( BddCurrent );
\*/

  BddReached = incbddrefext( BddCurrent );

  do
  {
    BddPreImage = (bddnode *)preimagebtrtransfunc( BtrTransFunc, BddCurrent );
    decbddrefext( BddCurrent );

    BddCurrent = missbddnodeassocon( (bddsystem *)0, BddPreImage, BddAssoc );
    decbddrefext( BddPreImage );

    fprintf( stdout, "PreImage:\n" );
/*\
    BtrViewBddState( BddCurrent );
\*/
    BddNew = applybddnodenot( (bddsystem *)0, BddReached );
    BddNew = applybddnode( (bddsystem *)0, ABL_AND, BddCurrent, decbddrefext( BddNew ) );

    BddReached = applybddnode( (bddsystem *)0, ABL_OR, 
                               decbddrefext( BddReached ), 
                               decbddrefext( BddCurrent ) );

    BddCurrent = BddNew;
  }
  while ( BddNew != BddLocalSystem->ZERO );

  decbddrefext( BddReached );
}

/*------------------------------------------------------------\
|                                                             |
|                          Image                              |
|                                                             |
\------------------------------------------------------------*/

void BtrImageFunc( BtrTransFunc, BddInitial )

  btrtransfunc *BtrTransFunc;
  bddnode      *BddInitial;
{
  bddnode      *BddCurrent;
  bddnode      *BddReached;
  bddnode      *BddNew;
  bddnode      *BddImage;

  BddCurrent = BddInitial;

  fprintf( stdout, "Initial:\n" );
  BtrViewBddState( BddCurrent );

  BddReached = incbddrefext( BddCurrent );

  do
  {
    BddImage = (bddnode *)imagebtrtransfunc( BtrTransFunc, BddCurrent );

    decbddrefext( BddCurrent );
    BddCurrent = BddImage;

    fprintf( stdout, "Image:\n" );
    BtrViewBddState( BddImage );

    BddNew = applybddnodenot( (bddsystem *)0, BddReached );
    BddNew = applybddnode( (bddsystem *)0, ABL_AND, BddCurrent, decbddrefext( BddNew ) );

    BddReached = applybddnode( (bddsystem *)0, ABL_OR, 
                               decbddrefext( BddReached ), 
                               decbddrefext( BddCurrent ) );

    BddCurrent = BddNew;
  }
  while ( BddNew != BddLocalSystem->ZERO );

  decbddrefext( BddReached );
}

/*------------------------------------------------------------\
|                                                             |
|                          Image                              |
|                                                             |
\------------------------------------------------------------*/

void BtrImageRel( BtrTransRel, BddInitial, BddAssoc )

  btrtransrel *BtrTransRel;
  bddnode     *BddInitial;
  bddassoc    *BddAssoc;
{
  bddnode      *BddCurrent;
  bddnode      *BddReached;
  bddnode      *BddNew;
  bddnode      *BddImage;

  BddCurrent = BddInitial;

  fprintf( stdout, "Initial:\n" );
  BtrViewBddState( BddCurrent );

  BddReached = incbddrefext( BddCurrent );

  do
  {
    BddImage = (bddnode *)imagebtrtransrel( BtrTransRel, BddCurrent );
    BddImage = missbddnodeassocon( (bddsystem *)0, 
                                   decbddrefext( BddImage ), BddAssoc );

    decbddrefext( BddCurrent );
    BddCurrent = BddImage;

    fprintf( stdout, "Image:\n" );
    BtrViewBddState( BddImage );

    BddNew = applybddnodenot( (bddsystem *)0, BddReached );
    BddNew = applybddnode( (bddsystem *)0, ABL_AND, BddCurrent, decbddrefext( BddNew ) );

    BddReached = applybddnode( (bddsystem *)0, ABL_OR, 
                               decbddrefext( BddReached ), 
                               decbddrefext( BddCurrent ) );

    BddCurrent = BddNew;
  }
  while ( BddNew != BddLocalSystem->ZERO );

  decbddrefext( BddReached );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

  int   argc;
  char *argv[];
{
  befig_list   *BehFigure;
  bereg_list   *BehReg;
  binode_list  *BiNode;
  bddnode      *BddNode;
  bddnode      *BddPrime;

  bddassoc     *BddAssoc;

  bddnode      *BddInitial;
  bddsystem    *BddSystem;

  btrtransfunc *BtrTransFunc;
  btrtransrel  *BtrTransRel;
  char          Buffer[ 512 ];

  char         *InputFileName;
  int           FlagImage;
  int           FlagFunc;
  int           FlagDebug;

  int           Number;
  int           Index;
  char          Option;

  mbkenv();

  InputFileName = (char *)0;
  FlagImage     = 1;
  FlagFunc      = 1;
  FlagDebug     = 0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        switch ( Option )
        {
          case 'f' : FlagFunc  = 1;
          break;
          case 't' : FlagFunc  = 0;
          break;
          case 'i' : FlagImage = 1;
          break;
          case 'p' : FlagImage = 0;
          break;
          case 'd' : FlagDebug = 1;
          break;
          default  : BtrUsage();
        }
      }
    }
    else
    {
      InputFileName = argv[ Number ];
    }
  }

  if ( ( argc          <   2        ) ||
       ( InputFileName == (char *)0 ) )
  {
    BtrUsage();
  }

  fprintf( stdout, "vhdlloadbefig %s\n", InputFileName );
  BehFigure = vhdlloadbefig( (befig_list *)0, InputFileName, 3 );
  fprintf( stdout, "file %s loaded\n", InputFileName );

  BddSystem = createbddsystem( 100, 1000, 100, 500000 );
  reorderbddsystemdynamic( BddSystem, reorderbddsystemsimple, 30000, 100 );

  BehFigure->BEDLY = (beaux_list *)0;

  fprintf( stdout, "beh_makbdd\n" );
  beh_makbdd( BehFigure, 0, 0 );

  if ( FlagFunc )
  {
    BtrTransFunc = createbtrtransfunc( (bddsystem *)0, 100 );
  }
  else
  {
    BtrTransRel  = createbtrtransrel( (bddsystem *)0, 100 );
  }

  BddAssoc = addbddassoc( (bddsystem *)0 );

  BehReg = BehFigure->BEREG;

  while ( BehReg != (bereg_list *)0 )
  {
    BiNode = BehReg->BINODE;

    BddNode = searchbddcircuitin( (bddcircuit *)0, BehReg->NAME );

    if ( FlagFunc )
    {
      addbtrtransfunc( BtrTransFunc, BddNode, BiNode->VALNODE );
    }
    else
    {
      sprintf( Buffer, "'%s", BehReg->NAME );
      BddPrime = addbddcircuitin( (bddcircuit *)0, Buffer, 
                                  BddNode->INDEX, BDD_IN_MODE_AFTER );

      addbtrtransrel( BtrTransRel, BddNode, BddPrime, BiNode->VALNODE );
    }

    addbddnodeassoc( (bddsystem *)0, BddAssoc, 
                     getbddvarbyindex( (bddsystem *)0, BddNode->INDEX ),
                     BddLocalSystem->ONE );

    BehReg = BehReg->NEXT;
  }

  BtrStateArray = autallocblock( BddLocalCircuit->NUMBER_NAME_IN + 1 );
  memset( BtrStateArray, '*', BddLocalCircuit->NUMBER_NAME_IN );

  fprintf( stdout, "Node: %ld\n", BddSystem->NUMBER_NODE );

  testbddcircuit( (bddcircuit *)0 );

  BddInitial = searchbddcircuitout( (bddcircuit *)0, "initial" );

  if ( BddInitial == (bddnode *)0 )
  {
    fprintf( stdout, "Missing output 'initial'\n" );
    EXIT( 1 );
  }

  if ( FlagFunc )
  {
    if ( FlagDebug )
    {
      viewbtrtransfunc( BtrTransFunc, BtrViewFunc );
    }

    if ( FlagImage ) BtrImageFunc   ( BtrTransFunc, BddInitial );
    else             BtrPreImageFunc( BtrTransFunc, BddInitial, BddAssoc );

    destroybtrtransfunc( BtrTransFunc );
  }
  else
  {
    if ( FlagDebug )
    {
      viewbtrtransrel( BtrTransRel, BtrViewRel );
    }

    if ( FlagImage ) BtrImageRel( BtrTransRel, BddInitial, BddAssoc );

    destroybtrtransrel( BtrTransRel );
  }

  delbddassoc( (bddsystem *)0, BddAssoc );

  testbddcircuit( (bddcircuit *)0 );

  destroybddcircuit( BehFigure->CIRCUI );
  destroybddsystem( BddSystem );

  beh_frebefig( BehFigure );

  return( 0 );
}
