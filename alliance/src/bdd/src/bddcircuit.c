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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddcircuit.c                     |
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
# include <memory.h>
# include "bddcircuit.h"
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

  bddcircuit *BddLocalCircuit = (bddcircuit *)0;

/*------------------------------------------------------------\
|                                                             |
|                        Private Variables                    |
|                                                             |
\------------------------------------------------------------*/

  static char       BddStableName[ 512 ];
  static authtable *BddHashStable    = (authtable *)0;
  static authtable *BddHashInvStable = (authtable *)0;
 
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Local Get Bdd Stable Name                  |
|                                                             |
\------------------------------------------------------------*/

char *loc_getbddstablename( Name )

  char *Name;
{
  authelem *Element;
  char     *StableName;

  if ( BddHashStable == (authtable *)0 )
  {
    BddHashStable    = createauthtable( 100 );
    BddHashInvStable = createauthtable( 100 );
  }

  Element = searchauthelem( BddHashStable, Name );

  if ( Element == (authelem *)0 )
  {
    sprintf( BddStableName, "%s'stable", Name );
    StableName = namealloc( BddStableName );

    Element = addauthelem( BddHashInvStable, StableName, (long)Name );
    Element = addauthelem( BddHashStable   , Name, (long)StableName );
  }

  return( (char *)( Element->VALUE ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Get Bdd Stable Name                      |
|                                                             |
\------------------------------------------------------------*/

char *getbddstablename( Name )

  char *Name;
{
  return( loc_getbddstablename( namealloc( Name ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Get Bdd Input Name                       |
|                                                             |
\------------------------------------------------------------*/

char *getbddcircuitinname( BddCircuit, BddIndex )

  bddcircuit *BddCircuit;
  bddindex    BddIndex;
{
  char  *Name;
  int    Index;

  setbddlocalcircuit( BddCircuit );

  checkbddindex( BddLocalSystem, BddIndex, 1 );

  Index = BddLocalCircuit->INDEX_IN[ BddIndex - BDD_INDEX_MIN ];
  Name  = BddLocalCircuit->NAME_IN[ Index ];

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Bdd Is Stable Name                   |
|                                                             |
\------------------------------------------------------------*/

char *loc_isbddstablename( StableName )

  char *StableName;
{
  authelem *Element;

  if ( BddHashStable == (authtable *)0 )
  {
    return( (char *)0 );
  }

  Element = searchauthelem( BddHashInvStable, StableName );

  if ( Element == (authelem *)0 )
  {
    return( (char *)0 );
  }

  return( (char *)( Element->VALUE ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Bdd Stable Name                      |
|                                                             |
\------------------------------------------------------------*/

char *isbddstablename( StableName )

  char *StableName;
{
  return( loc_isbddstablename( namealloc( StableName ) ) );
}


/*------------------------------------------------------------\
|                                                             |
|                       Bdd Create Circuit                    |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *createbddcircuit( Name, NumberIn, NumberOut, BddSystem )

  char      *Name;
  long       NumberIn;
  long       NumberOut;
  bddsystem *BddSystem;
{
  bddcircuit *BddCircuit;

  setbddlocalsystem( BddSystem );

  BddCircuit = allocbddcircuit();
  
  BddCircuit->NAME          = namealloc( Name );
  BddCircuit->HASH_IN       = createauthtable( NumberIn  << 1 );
  BddCircuit->HASH_OUT      = createauthtable( NumberOut << 1 );
  BddCircuit->NAME_IN       = allocbddnamein( NumberIn );
  BddCircuit->NAME_IN_SIZE  = NumberIn;
  BddCircuit->INDEX_IN      = allocbddindexin( NumberIn );
  BddCircuit->INDEX_IN_SIZE = NumberIn;
  BddCircuit->BDD_SYSTEM    = BddLocalSystem;

  BddLocalCircuit = BddCircuit;

  return( BddCircuit );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Reset Circuit                    |
|                                                             |
\------------------------------------------------------------*/

void resetbddcircuit( BddCircuit )

  bddcircuit *BddCircuit;
{
  setbddlocalcircuit( BddCircuit );

  resetauthtable( BddLocalCircuit->HASH_IN  );
  resetauthtable( BddLocalCircuit->HASH_OUT );

  memset( (void *)BddLocalCircuit->NAME_IN, 0,
          (size_t)BddLocalCircuit->NAME_IN_SIZE * sizeof( bddnamein ) );

  memset( (void *)BddLocalCircuit->INDEX_IN, 0,
          (size_t)BddLocalCircuit->INDEX_IN_SIZE * sizeof( bddindex ) );

  BddLocalCircuit->NUMBER_NAME_IN  = 0;
  BddLocalCircuit->NUMBER_NAME_OUT = 0;
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Destroy Circuit                   |
|                                                             |
\------------------------------------------------------------*/

void destroybddcircuit( BddCircuit )

  bddcircuit *BddCircuit;
{
  setbddlocalcircuit( BddCircuit );

  destroyauthtable( BddLocalCircuit->HASH_IN  );
  destroyauthtable( BddLocalCircuit->HASH_OUT );

  freebddnamein( BddLocalCircuit->NAME_IN  );
  freebddindexin( BddLocalCircuit->INDEX_IN  );

  freebddcircuit( BddLocalCircuit );

  BddLocalCircuit = (bddcircuit *)0;
}

/*------------------------------------------------------------\
|                                                             |
|                          Bdd Input Circuit                  |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Bdd Search Input Circuit              |
|                                                             |
\------------------------------------------------------------*/

bddnode *searchbddcircuitin( BddCircuit, InputName )

  bddcircuit *BddCircuit;
  char       *InputName;
{
  authelem *Element;

  setbddlocalcircuit( BddCircuit );

  if ( InputName != (char *)0 )
  {
    InputName = namealloc( InputName );
    Element   = searchauthelem( BddLocalCircuit->HASH_IN, InputName );

    if ( Element != (authelem *)0 )
    {
      return( (bddnode *)Element->VALUE );
    }
  }

  return( (bddnode *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Add Circuit Input                     |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddcircuitin( BddCircuit, InputName, Index, Mode )

  bddcircuit *BddCircuit;
  char       *InputName;
  bddindex    Index;
  long        Mode;
{
  char     **NameIn;
  bddindex  *IndexIn;
  authelem  *Element;
  bddnode   *BddNode;
  long       NameInSize;      
  long       NumberNameIn;
  long       IndexInSize;      

  setbddlocalcircuit( BddCircuit );
  setbddlocalsystem( BddLocalCircuit->BDD_SYSTEM );

  BddNode   = (bddnode *)0;
  InputName = namealloc( InputName );
  Element   = searchauthelem( BddLocalCircuit->HASH_IN, InputName );

  if ( Element != (authelem *)0 )
  {
    return( (bddnode *)Element->VALUE );
  }

  if ( Mode & BDD_IN_MODE_IMPOSE )
  {
    if ( checkbddindex( (bddsystem *)0, Index, 0 ) )
    {
      BddNode = getbddvarnodebyindex( (bddsystem *)0, Index );
    }
    else
    {
      Mode &= ~BDD_IN_MODE_IMPOSE;
    }
  }

  if ( ! ( Mode & BDD_IN_MODE_IMPOSE ) )
  {
    switch ( Mode )
    {
      case BDD_IN_MODE_FIRST  : BddNode = addbddvarfirst( (bddsystem *)0 );
      break;

      case BDD_IN_MODE_LAST   : BddNode = addbddvarlast( (bddsystem *)0 );
      break;

      case BDD_IN_MODE_BEFORE : BddNode = addbddvarbefore( (bddsystem *)0, Index );
      break;

      case BDD_IN_MODE_AFTER  : BddNode = addbddvarafter( (bddsystem *)0, Index );
      break;

      default : bdderror( BDD_UNKNOWN_IN_MODE_ERROR, Mode );
    }
  }

  addauthelem( BddLocalCircuit->HASH_IN, InputName, (long)BddNode );

  NumberNameIn = BddLocalCircuit->NUMBER_NAME_IN;
  NameIn       = BddLocalCircuit->NAME_IN;
  NameInSize   = BddLocalCircuit->NAME_IN_SIZE;

  if ( NumberNameIn >= NameInSize )
  {
    NameIn = resizebddnamein( NameIn, NameInSize, ( NameInSize << 1 ) );

    BddLocalCircuit->NAME_IN      = NameIn;
    BddLocalCircuit->NAME_IN_SIZE = NameInSize << 1;
  }

  NameIn[ NumberNameIn ] = InputName;
  BddLocalCircuit->NUMBER_NAME_IN++;

  IndexIn     = BddLocalCircuit->INDEX_IN;
  IndexInSize = BddLocalCircuit->INDEX_IN_SIZE;
  Index       = BddNode->INDEX - BDD_INDEX_MIN;

  if ( Index >= IndexInSize )
  {
    IndexIn = resizebddindexin( IndexIn, IndexInSize, (long)( Index ) << 1 );

    BddLocalCircuit->INDEX_IN      = IndexIn;
    BddLocalCircuit->INDEX_IN_SIZE = (long)( Index ) << 1;
  }

  IndexIn[ Index ] = (bddindex)NumberNameIn;

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Rename Input Circuit              |
|                                                             |
\------------------------------------------------------------*/

bddnode *renamebddcircuitin( BddCircuit, InputName, NewName )

  bddcircuit *BddCircuit;
  char       *InputName;
  char       *NewName;
{
  authelem  *Element1;
  authelem  *Element2;
  bddnode   *BddNode;
  char     **NameIn;
  bddindex  *IndexIn;
  bddindex   Index;

  setbddlocalcircuit( BddCircuit );

  InputName = namealloc( InputName );
  NewName   = namealloc( NewName   );

  Element1 = searchauthelem( BddLocalCircuit->HASH_IN, InputName );
  Element2 = searchauthelem( BddLocalCircuit->HASH_IN, NewName   );

  if ( ( Element1 != (authelem *)0 ) &&
       ( Element2 == (authelem *)0 ) )
  {
    BddNode = (bddnode *)Element1->VALUE;

    delauthelem( BddLocalCircuit->HASH_IN, InputName );
    addauthelem( BddLocalCircuit->HASH_IN, NewName, (long)BddNode );

    NameIn  = BddLocalCircuit->NAME_IN;
    IndexIn = BddLocalCircuit->INDEX_IN;
    Index   = BddNode->INDEX - BDD_INDEX_MIN;

    NameIn[ IndexIn[ Index ] ] = NewName;

    return( BddNode );
  }

  return( (bddnode *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                   Add Circuit Auxiliary Signal              |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddcircuitaux( BddCircuit, InputName, BddAux, UserFunc, Mode )

  bddcircuit *BddCircuit;
  char       *InputName;
  bddnode    *BddAux;
  int       (*UserFunc)();
  long        Mode;
{
  char     **NameIn;
  bddindex  *IndexIn;
  bddindex   Index;
  authelem  *Element;
  bddnode   *BddNode;
  long       NameInSize;      
  long       NumberNameIn;
  long       IndexInSize;      

  setbddlocalcircuit( BddCircuit );

  BddNode   = (bddnode *)0;
  InputName = namealloc( InputName );
  Element   = searchauthelem( BddLocalCircuit->HASH_IN, InputName );

  if ( Element != (authelem *)0 )
  {
    bdderror( BDD_BAD_IN_ERROR, InputName );
  }

  switch ( Mode )
  {
    case BDD_AUX_MODE_SINGLE : 

      BddNode = addbddvarauxsingle( (bddsystem *)0, BddAux );

    break;

    case BDD_AUX_MODE_GLOBAL : 

      BddNode = addbddvarauxglobal( (bddsystem *)0, BddAux, UserFunc );

    break;

    default : bdderror( BDD_UNKNOWN_AUX_MODE_ERROR, Mode );
  }

  addbddcircuitout( (bddcircuit *)0, InputName, BddNode );
  addauthelem( BddLocalCircuit->HASH_IN, InputName, (long)BddAux );

  NumberNameIn = BddLocalCircuit->NUMBER_NAME_IN;
  NameIn       = BddLocalCircuit->NAME_IN;
  NameInSize   = BddLocalCircuit->NAME_IN_SIZE;

  if ( NumberNameIn >= NameInSize )
  {
    NameIn = resizebddnamein( NameIn, NameInSize, ( NameInSize << 1 ) );

    BddLocalCircuit->NAME_IN      = NameIn;
    BddLocalCircuit->NAME_IN_SIZE = NameInSize << 1;
  }

  NameIn[ NumberNameIn ] = InputName;
  BddLocalCircuit->NUMBER_NAME_IN++;

  IndexIn     = BddLocalCircuit->INDEX_IN;
  IndexInSize = BddLocalCircuit->INDEX_IN_SIZE;
  Index       = BddAux->INDEX - BDD_INDEX_MIN;

  if ( Index >= IndexInSize )
  {
    IndexIn = resizebddindexin( IndexIn, IndexInSize, (long)( Index ) << 1 );

    BddLocalCircuit->INDEX_IN      = IndexIn;
    BddLocalCircuit->INDEX_IN_SIZE = (long)( Index ) << 1;
  }

  IndexIn[ Index ] = (bddindex)NumberNameIn;

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                          Bdd Output Circuit                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Bdd Search Output Circuit              |
|                                                             |
\------------------------------------------------------------*/

bddnode *searchbddcircuitout( BddCircuit, OutputName )

  bddcircuit *BddCircuit;
  char       *OutputName;
{
  authelem *Element;

  setbddlocalcircuit( BddCircuit );

  OutputName = namealloc( OutputName );
  Element    = searchauthelem( BddLocalCircuit->HASH_OUT, OutputName );

  if ( Element != (authelem *)0 )
  {
    return( (bddnode *)Element->VALUE );
  }

  return( (bddnode *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Add Output Circuit                 |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddcircuitout( BddCircuit, OutputName, BddNode )

  bddcircuit *BddCircuit;
  char       *OutputName;
  bddnode    *BddNode;
{
  authelem *Element;

  setbddlocalcircuit( BddCircuit );

  OutputName = namealloc( OutputName );
  Element    = searchauthelem( BddLocalCircuit->HASH_OUT, OutputName );

  if ( Element != (authelem *)0 )
  {
    decbddrefext( (bddnode *)Element->VALUE );
    Element->VALUE = (long)( BddNode );
  }
  else
  {
    BddLocalCircuit->NUMBER_NAME_OUT++;
    Element = addauthelem( BddLocalCircuit->HASH_OUT, OutputName, (long)BddNode );
  }

  return( incbddrefext( BddNode ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Bdd Del Output Circuit                 |
|                                                             |
\------------------------------------------------------------*/

int delbddcircuitout( BddCircuit, OutputName )

  bddcircuit *BddCircuit;
  char       *OutputName;
{
  authelem *Element;

  setbddlocalcircuit( BddCircuit );

  OutputName = namealloc( OutputName );
  Element    = searchauthelem( BddLocalCircuit->HASH_OUT, OutputName );

  if ( Element != (authelem *)0 )
  {
    decbddrefext( (bddnode *)Element->VALUE );

    BddLocalCircuit->NUMBER_NAME_OUT--;

    return( delauthelem( BddLocalCircuit->HASH_OUT, OutputName ) );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Add Circuit Abl                      |
|                                                             |
\------------------------------------------------------------*/

static bddnode *loc_addbddcircuitabl( Expr )

  ablexpr *Expr;
{
  bddnode    *BddNode;
  bddnode    *BddFirst;
  char       *AtomValue;
  long        Oper;
  int         Negative;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( AtomValue == ABL_ATOM_NAME_ONE )
    {
      return( BddLocalSystem->ONE );
    }

    if ( AtomValue == ABL_ATOM_NAME_ZERO )
    {
      return( BddLocalSystem->ZERO );
    }

    BddNode = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode == (bddnode *)0 )
    {
      BddNode = searchbddcircuitout( (bddcircuit *)0, AtomValue );

      if ( BddNode == (bddnode *)0 )
      {
        if ( AtomValue == ABL_ATOM_NAME_DC )
        {
          return( BddLocalSystem->ZERO );
        }
        else
        {
          BddNode = addbddcircuitin( (bddcircuit *)0, AtomValue, 
                                     (bddindex    )0, BDD_IN_MODE_LAST );
        }
      }
    }

    return( incbddrefext( BddNode ) );
  }

  Oper = ABL_OPER( Expr );
  Expr = ABL_CDR( Expr );

  if ( Oper == ABL_NOT )
  {
    if ( ABL_CDR( Expr ) != (ablexpr *)0 )
    {
      bdderror( BDD_OPERATOR_ERROR, Oper );
    }

    BddFirst = loc_addbddcircuitabl( ABL_CAR( Expr ) );
    BddNode  = applybddnodenot( (bddsystem *)0, decbddrefext( BddFirst ) );

    return( BddNode );
  }

  if ( Oper == ABL_STABLE )
  {
    Expr = ABL_CAR( Expr );

    if ( ! ABL_ATOM( Expr ) )
    {
      bdderror( BDD_OPERATOR_ERROR, Oper );
    }

    AtomValue = ABL_ATOM_VALUE( Expr );
    BddFirst  = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddFirst == (bddnode *)0 )
    {
      BddFirst = addbddcircuitin( (bddcircuit *)0, AtomValue,
                                  (bddindex    )0, BDD_IN_MODE_LAST );
    }

    AtomValue = loc_getbddstablename( AtomValue );
    BddNode   = searchbddcircuitin( (bddcircuit *)0, AtomValue );

    if ( BddNode == (bddnode *)0 )
    {
      BddNode = addbddcircuitin( (bddcircuit *)0, AtomValue,
                                 (bddindex    )0, BDD_IN_MODE_LAST );
    }

    return( BddNode );
  }

  if ( ( isablunaryoper( Oper )          ) ||
       ( ABL_CDR( Expr ) == (ablexpr *)0 ) )
  {
    bdderror( BDD_OPERATOR_ERROR, Oper );
  }

  if ( ( getabloperpolar( Oper ) == ABL_POLAR_POSITIVE ) ||
       ( ABL_CDDR( Expr )        == (ablexpr *)0       ) )
  {
    Negative = 0;
  }
  else
  {
    Negative = 1;
    Oper     = getablopernot( Oper );
  }

  BddFirst = loc_addbddcircuitabl( ABL_CAR( Expr ) );

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    BddNode  = loc_addbddcircuitabl( ABL_CAR( Expr ) );
    BddFirst = applybddnode( (bddsystem *)0, Oper,
                             decbddrefext( BddFirst ),
                             decbddrefext( BddNode  ) );
  }

  if ( Negative )
  {
    BddFirst = applybddnodenot( (bddsystem *)0,
                                decbddrefext( BddFirst ) );
  }

  return( BddFirst );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd Add Abl Circuit                  |
|                                                             |
\------------------------------------------------------------*/

bddnode *addbddcircuitabl( BddCircuit, Expr )

  bddcircuit *BddCircuit;
  ablexpr    *Expr;
{
  bddnode *BddNode;

  setbddlocalcircuit( BddCircuit );

  BddNode = loc_addbddcircuitabl( Expr );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Stable Circuit Abl                  |
|                                                             |
\------------------------------------------------------------*/

void loc_stablebddcircuitabl( Expr )

  ablexpr *Expr;
{
  char *Name;

  if ( ABL_ATOM( Expr ) )
  {
    Name = loc_isbddstablename( ABL_ATOM_VALUE( Expr ) );

    if ( Name != (char *)0 )
    {
      ABL_CAR( Expr ) = addchain( (chain_list *)0, (void *)ABL_STABLE );
      addablhexpr( Expr, createablatom( Name ) );
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      loc_stablebddcircuitabl( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Convert Circuit Abl                 |
|                                                             |
\------------------------------------------------------------*/

ablexpr *convertbddcircuitabl( BddCircuit, BddNode )

  bddcircuit *BddCircuit;
  bddnode    *BddNode;
{
  ablexpr *Expr;

  setbddlocalcircuit( BddCircuit );

  Expr = convertbddnodeabl( (bddsystem *)0,
                            BddLocalCircuit->NAME_IN,
                            BddLocalCircuit->INDEX_IN, BddNode );

  loc_stablebddcircuitabl( Expr );

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                   Bdd Convert Circuit Sum Abl               |
|                                                             |
\------------------------------------------------------------*/

ablexpr *convertbddcircuitsumabl( BddCircuit, BddNode )

  bddcircuit *BddCircuit;
  bddnode    *BddNode;
{
  ablexpr *Expr;

  setbddlocalcircuit( BddCircuit );

  Expr = convertbddnodesumabl( (bddsystem *)0,
                               BddLocalCircuit->NAME_IN,
                               BddLocalCircuit->INDEX_IN, BddNode );

  loc_stablebddcircuitabl( Expr );

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                        Bdd View Circuit                     |
|                                                             |
\------------------------------------------------------------*/

void viewbddcircuit( BddCircuit, ViewName )

  bddcircuit *BddCircuit;
  char        ViewName;
{
  bddnamein *NameIn;
  long       ScanName;
  long       NumberName;

  if ( BddCircuit == (bddcircuit *)0 )
  {
    BddCircuit = BddLocalCircuit;
  }

  fprintf( stdout, "--> BddCircuit\n" );

  fprintf( stdout, "  NAME            : %s \n", BddCircuit->NAME            );
  fprintf( stdout, "  NAME_IN_SIZE    : %ld\n", BddCircuit->NAME_IN_SIZE    );
  fprintf( stdout, "  NUMBER_NAME_IN  : %ld\n", BddCircuit->NUMBER_NAME_IN  );
  fprintf( stdout, "  NUMBER_NAME_OUT : %ld\n", BddCircuit->NUMBER_NAME_OUT );

  if ( ViewName )
  {
    NameIn     = BddCircuit->NAME_IN;
    NumberName = BddCircuit->NUMBER_NAME_IN;

    for ( ScanName = 0; ScanName < NumberName; ScanName++ )
    {
      fprintf( stdout,"  NAME_IN[ %ld ] = %s\n", ScanName, NameIn[ ScanName ] );
    }
  }

  fprintf( stdout, "  HASH_IN :\n" );
  viewauthtable( BddCircuit->HASH_IN, (void *)0 );
  fprintf( stdout, "  HASH_OUT :\n" );
  viewauthtable( BddCircuit->HASH_OUT, (void *)0 );

  fprintf( stdout, "<-- BddCircuit\n" );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Bdd Circuit Node Num                  |
|                                                             |
\------------------------------------------------------------*/

long getbddcircuitnumnode( BddCircuit )

  bddcircuit *BddCircuit;
{
  authelem *Element;
  long      HashIndex;
  bddnode  *BddNode;
  long      NumNode;

  setbddlocalcircuit( BddCircuit );

  NumNode = 0;

  for ( HashIndex = 0; HashIndex < BddLocalCircuit->HASH_OUT->TABLE_SIZE; HashIndex++ ) 
  {
    Element = &BddLocalCircuit->HASH_OUT->TABLE[ HashIndex ];
		      
    if ( checkauthkey( Element->KEY, 0 ) ) 
    {
      BddNode = (bddnode *)Element->VALUE;
      NumNode += getbddnodenum( (bddsystem *)0, BddNode );
    }
  }
  return( NumNode );
}

/*------------------------------------------------------------\
|                                                             |
|                         Get Bdd Node Size                   |
|                                                             |
\------------------------------------------------------------*/

long getbddcircuitsize( BddCircuit )

  bddcircuit *BddCircuit;
{
  authelem *Element;
  long      HashIndex;
  bddnode  *BddNode;
  long      SizeNode;

  setbddlocalcircuit( BddCircuit );

  SizeNode = 0;

  for ( HashIndex = 0; HashIndex < BddLocalCircuit->HASH_OUT->TABLE_SIZE; HashIndex++ ) 
  {
    Element = &BddLocalCircuit->HASH_OUT->TABLE[ HashIndex ];
		      
    if ( checkauthkey( Element->KEY, 0 ) ) 
    {
      BddNode = (bddnode *)Element->VALUE;
      SizeNode += getbddnodenum( (bddsystem *)0, BddNode );
    }
  }
  return( SizeNode );
}
