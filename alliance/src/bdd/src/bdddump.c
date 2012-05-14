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
| File    :                   bdddump.c                       |
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
# include <memory.h>
# include "bdddump.h"
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

  static FILE *BddDumpFile = (FILE *)0;
  static char  BddDumpBuffer[ BDD_UNDUMP_BUFFER_SIZE ];
  static long  BddDumpLine = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static long loc_undumpbddhkey( Table, Key )

   authtable *Table;
   char      *Key;
{
  return( ( (unsigned long)Key ) % Table->TABLE_SIZE );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Undump Get Line                   |
|                                                             |
\------------------------------------------------------------*/

static void loc_undumpbddgetline()
{
  char *Scan;

  while( fgets( BddDumpBuffer, BDD_UNDUMP_BUFFER_SIZE, BddDumpFile ) )
  {
    Scan = strchr( BddDumpBuffer, '\n' );
    if ( Scan != (char *)0 )
    {
      *Scan = '\0'; BddDumpLine++;
    }

    if ( ( BddDumpBuffer[ 0 ] != '#'  ) &&
         ( BddDumpBuffer[ 0 ] != '\0' ) )
    {
      return;
    }
  }

  bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Undump Circuit                    |
|                                                             |
\------------------------------------------------------------*/

bddcircuit *undumpbddcircuit( BddSystem, FileName )
 
  bddsystem *BddSystem;
  char      *FileName;
{
  bddcircuit *BddCircuit;
  authtable  *HashInput;
  authtable  *HashNode;
  authelem   *Element;
  bddnode    *BddNode;
  bddnode    *BddHigh;
  bddnode    *BddLow;
  char       *Name;
  char       *Scan;
  int         Value;
  long        Index;
  long        NumberIn;
  long        NumberOut;
  long        NumberNode;
  long        BddVar;
  long        ZeroAddr;
  long        OneAddr;
  long        NodeAddr;
  long        HighAddr;
  long        LowAddr;

  FileName    = autbasename( FileName, "bdd" );
  BddDumpFile = mbkfopen( FileName, "bdd", "r" );
  BddDumpLine = 0;

  if ( BddDumpFile == (FILE *)0 )
  {
    bdderror( BDD_UNDUMP_FILE_ERROR, FileName );
  }

  mbkfree( FileName );

  loc_undumpbddgetline();
  Name = BddDumpBuffer + 9;

  loc_undumpbddgetline();
  Value = sscanf( BddDumpBuffer, ".port %ld %ld", &NumberIn, &NumberOut );
  if ( Value != 2 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );

  loc_undumpbddgetline();

  BddCircuit = createbddcircuit( Name, NumberIn, NumberOut, BddSystem );
  HashInput  = createauthtable( NumberIn + 1 );
  setauthfunc( HashInput, 0, loc_undumpbddhkey, 0 );

  for ( Index = 0; Index < NumberIn; Index++ )
  {
    loc_undumpbddgetline();

    Value = 0;
    Scan  = strchr( BddDumpBuffer, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
      Value = sscanf( BddDumpBuffer, "%ld", &BddVar );
      Name  = namealloc( Scan + 1 );
    }
    
    if ( Value != 1 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );

    BddNode = addbddcircuitin( (bddcircuit *)0, Name, 0, BDD_IN_MODE_LAST );

    addauthelem( HashInput, (char *)BddVar + 2, (long)BddNode->INDEX );
  }

  loc_undumpbddgetline();
  Value = sscanf( BddDumpBuffer, ".node %ld %lX %lX",
                  &NumberNode, &ZeroAddr, &OneAddr );
  if ( Value != 3 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );

  HashNode = createauthtable( NumberNode + 1 );
  setauthfunc( HashNode, 0, loc_undumpbddhkey, 0 );

  addauthelem( HashNode, (char *)ZeroAddr, (long)BddLocalSystem->ZERO );
  addauthelem( HashNode, (char *)OneAddr , (long)BddLocalSystem->ONE  );

  loc_undumpbddgetline();

  while ( BddDumpBuffer[ 0 ] != '.' )
  {
    Value = sscanf( BddDumpBuffer, "%lX %ld %lX %lX",
            &NodeAddr, &BddVar, &HighAddr, &LowAddr );
    if ( Value != 4 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );

    Element = searchauthelem( HashNode, (char *)HighAddr );
    if ( Element == (authelem *)0 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );
    BddHigh = (bddnode *)( Element->VALUE );

    Element = searchauthelem( HashNode, (char *)LowAddr );
    if ( Element == (authelem *)0 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );
    BddLow  = (bddnode *)( Element->VALUE );

    Element = searchauthelem( HashInput, (char *)( BddVar + 2 ) );
    if ( Element == (authelem *)0 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );
    BddNode = addbddnode( (bddsystem *)0, Element->VALUE, BddHigh, BddLow );

    addauthelem( HashNode, (char *)NodeAddr, (long)decbddrefext( BddNode ) );
    loc_undumpbddgetline();
  }

  for ( Index = 0; Index < NumberOut; Index++ )
  {
    loc_undumpbddgetline();

    Value = 0;
    Scan  = strchr( BddDumpBuffer, ' ' );

    if ( Scan != (char *)0 )
    {
      *Scan = '\0';
      Value = sscanf( BddDumpBuffer, "%lX", &NodeAddr );
      Name  = namealloc( Scan + 1 );
    }
    
    if ( Value != 1 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );

    Element = searchauthelem( HashNode, (char *)NodeAddr );
    if ( Element == (authelem *)0 ) bdderror( BDD_UNDUMP_LINE_ERROR, BddDumpLine );
    BddNode = (bddnode *)( Element->VALUE );

    addbddcircuitout( (bddcircuit *)0, Name, BddNode );
  }
  
  fclose( BddDumpFile );

  return( BddCircuit );
}

/*------------------------------------------------------------\
|                                                             |
|                     Bdd Dump Circuit Output                 |
|                                                             |
\------------------------------------------------------------*/

static void loc_dumpbddcircuitout( BddNode )

  bddnode *BddNode;
{
  if ( BddNode->INDEX >= BDD_INDEX_MIN )
  {
    if ( ! ( BddNode->FLAG & BDD_FLAG_NUM_MASK ) )
    {
      loc_dumpbddcircuitout( BddNode->HIGH );
      loc_dumpbddcircuitout( BddNode->LOW  );

      BddNode->FLAG |= BDD_FLAG_NUM_MASK;

      fprintf( BddDumpFile, "%lX %d %lX %lX\n", 
               (long)BddNode,
               BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ],
               (long)BddNode->HIGH,
               (long)BddNode->LOW );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Dump Circuit                      |
|                                                             |
\------------------------------------------------------------*/

static long loc_dumpbddcompare( ValueArray, Index1, Index2 )

   authelem *ValueArray;
   long      Index1;
   long      Index2;
{
  return( ValueArray[ Index2 ].VALUE - ValueArray[ Index1 ].VALUE );
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Dump Circuit                      |
|                                                             |
\------------------------------------------------------------*/

void dumpbddcircuit( BddCircuit, FileName )

  bddcircuit *BddCircuit;
  char       *FileName;
{
  authelem  *HashTable;
  authelem  *HashElement;
  authelem  *ValueArray;
  long      *IndexArray;
  long       Index;
  long       HashSize;
  long       HashIndex;
  long       NumberIn;
  long       NumberOut;
  bddindex   BddIndex;

  setbddlocalcircuit( BddCircuit );

  FileName    = autbasename( FileName, "bdd" );
  BddDumpFile = mbkfopen( FileName, "bdd", "w" );

  if ( BddDumpFile == (FILE *)0 )
  {
    bdderror( BDD_DUMP_FILE_ERROR, FileName );
  }

  mbkfree( FileName );
  fprintf( BddDumpFile, ".circuit %s\n", BddLocalCircuit->NAME );

  NumberIn  = BddLocalCircuit->HASH_IN->NUMBER_ELEM;
  NumberOut = BddLocalCircuit->HASH_OUT->NUMBER_ELEM;

  HashTable = BddLocalCircuit->HASH_IN->TABLE;
  HashSize  = BddLocalCircuit->HASH_IN->TABLE_SIZE;

  fprintf( BddDumpFile, ".port %ld %ld\n", NumberIn, NumberOut );

  fprintf( BddDumpFile, ".in\n" );

  IndexArray = (long     *)autallocblock( sizeof( long     ) * ( NumberIn + 1 ));
  ValueArray = (authelem *)autallocblock( sizeof( authelem ) * ( NumberIn + 1 ));
  Index      = 0;

  for ( HashIndex = 0; HashIndex < HashSize; HashIndex++ )
  {
    HashElement = &HashTable[ HashIndex ];

    if ( checkauthkey( HashElement->KEY, 0 ) )
    {
      BddIndex = ((bddnode *) HashElement->VALUE )->INDEX;

      ValueArray[ Index ].KEY   = HashElement->KEY;
      ValueArray[ Index ].VALUE = BddLocalSystem->INDEX_TO_VAR[ BddIndex ];

      Index++;
    }
  }

  sortautarray( ValueArray, IndexArray, NumberIn, loc_dumpbddcompare );

  for ( HashIndex = 0; HashIndex < NumberIn; HashIndex++ )
  {
    Index = IndexArray[ HashIndex ];

    fprintf( BddDumpFile, "%ld %s\n",
             ValueArray[ Index ].VALUE, ValueArray[ Index ].KEY );
  }

  autfreeblock( IndexArray );
  autfreeblock( ValueArray );

  HashTable = BddLocalCircuit->HASH_OUT->TABLE;
  HashSize  = BddLocalCircuit->HASH_OUT->TABLE_SIZE;

  fprintf( BddDumpFile, ".node %ld %lX %lX\n",
           BddLocalSystem->NUMBER_NODE,
           (long)BddLocalSystem->ZERO,
           (long)BddLocalSystem->ONE );

  for ( HashIndex = 0; HashIndex < HashSize; HashIndex++ )
  {
    HashElement = &HashTable[ HashIndex ];

    if ( checkauthkey( HashElement->KEY, 0 ) )
    {
      loc_dumpbddcircuitout( (bddnode *)( HashElement->VALUE ) );
    }
  }

  fprintf( BddDumpFile, ".out\n" );

  for ( HashIndex = 0; HashIndex < HashSize; HashIndex++ )
  {
    HashElement = &HashTable[ HashIndex ];

    if ( checkauthkey( HashElement->KEY, 0 ) )
    {
      fprintf( BddDumpFile, "%lX %s\n", 
              (long)HashElement->VALUE, HashElement->KEY );

      unflagbddnode( (bddsystem *)0,
                     (bddnode *)( HashElement->VALUE ),
                     BDD_FLAG_NUM_MASK );
    }
  }

  fclose( BddDumpFile );
}
