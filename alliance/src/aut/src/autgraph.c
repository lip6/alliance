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
| Tool    :                     AUT                           |
|                                                             |
| File    :                  autgraph.c                       |
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
# include <stdlib.h>

# include <mut.h>
# include "aut.h"

# include "auterror.h"
# include "autgraph.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Create                        |
|                                                             |
\------------------------------------------------------------*/

autgraph *createautgraph()
{
  autgraph *Graph;

  Graph = (autgraph *)autallocheap( sizeof( autgraph ) );
  Graph->HASH_NODE = createauthtable( 100 );
  Graph->HASH_ARC  = createauth2table( 100 );

  return( Graph );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Search Node                   |
|                                                             |
\------------------------------------------------------------*/

autnode_list *searchautgraphnode( Graph, Name )

  autgraph *Graph;
  char     *Name;
{
  authelem *Element;

  Name = namealloc( Name );
  Element = searchauthelem( Graph->HASH_NODE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (autnode_list *)Element->VALUE );
  }

  return( (autnode_list *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Search Arc                    |
|                                                             |
\------------------------------------------------------------*/

autarc_list *searchautgrapharc( Graph, NodeFrom, NodeTo )

  autgraph     *Graph;
  autnode_list *NodeFrom;
  autnode_list *NodeTo;
{
  auth2elem *Element;

  Element = searchauth2elem( Graph->HASH_ARC, NodeFrom->NAME, NodeTo->NAME );

  if ( Element != (auth2elem *)0 )
  {
    return( (autarc_list *)Element->VALUE );
  }

  return( (autarc_list *)0 );
}


/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Add Node                      |
|                                                             |
\------------------------------------------------------------*/

autnode_list *addautgraphnode( Graph, Name )

  autgraph *Graph;
  char     *Name;
{
  autnode_list *Node;
  authelem     *Element;

  Name    = namealloc( Name );
  Element = searchauthelem( Graph->HASH_NODE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (autnode_list *)Element->VALUE );
  }

  Node = (autnode_list *)autallocheap( sizeof( autnode_list ) );
  Node->PREV = &Graph->NODE;
  Node->NAME = Name;

  if ( Graph->NODE != (autnode_list *)0 )
  {
    Node->NEXT        = Graph->NODE;
    Graph->NODE->PREV = &Node->NEXT;
  }

  addauthelem( Graph->HASH_NODE, Name, (long)Node );

  Graph->NODE = Node;
  Graph->NUMBER_NODE++;

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Add Arc                       |
|                                                             |
\------------------------------------------------------------*/

autarc_list *addautgrapharc( Graph, NodeFrom, NodeTo )

  autgraph     *Graph;
  autnode_list *NodeFrom;
  autnode_list *NodeTo;
{
  autarc_list *Arc;
  chain_list  *ScanArc;

  Arc = searchautgrapharc( Graph, NodeFrom, NodeTo );

  if ( Arc != (autarc_list *)0 )
  {
    return( Arc );
  }

  Arc = (autarc_list *)autallocheap( sizeof( autarc_list ) );
  Arc->NODE_FROM = NodeFrom;
  Arc->NODE_TO   = NodeTo;
  Arc->PREV      = &Graph->ARC;

  if ( Graph->ARC != (autarc_list *)0 )
  {
    Arc->NEXT        = Graph->ARC;
    Graph->ARC->PREV = &Arc->NEXT;
  }

  Graph->ARC = Arc;
  Graph->NUMBER_ARC++;

  NodeFrom->ARC_FROM = addchain( NodeFrom->ARC_FROM, Arc );
  NodeTo->ARC_TO     = addchain( NodeTo->ARC_TO    , Arc );

  addauth2elem( Graph->HASH_ARC, (char *)NodeFrom, (char *)NodeTo, (long)Arc );

  return( Arc );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Del Node                      |
|                                                             |
\------------------------------------------------------------*/

void delautgraphnode( Graph, Node )

  autgraph     *Graph;
  autnode_list *Node;
{
  autarc_list  *Arc;
  autnode_list *ScanNode;
  chain_list   *ScanArc;
  chain_list  **PrevChain;
  chain_list   *ScanChain;

  if ( Node->NEXT != (autnode_list *)0 )
  {
    Node->NEXT->PREV = Node->PREV;
  }

  *Node->PREV = Node->NEXT;

  for ( ScanArc  = Node->ARC_FROM;
        ScanArc != (chain_list *)0;
        ScanArc  = ScanArc->NEXT )
  {
    Arc = (autarc_list *)ScanArc->DATA;
    ScanNode = Arc->NODE_TO;
    
    PrevChain = &ScanNode->ARC_TO;
  
    for ( ScanChain  = ScanNode->ARC_TO;
          ScanChain != (chain_list *)0; 
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == Arc ) break;

      PrevChain = &ScanChain->NEXT;
    }

    *PrevChain = ScanChain->NEXT;
    ScanChain->NEXT = (chain_list *)0;
    freechain( ScanChain );

    if ( Arc->NEXT != (autarc_list *)0 )
    {
      Arc->NEXT->PREV = Arc->PREV;
    }

    delauth2elem( Graph->HASH_ARC, Arc->NODE_FROM->NAME, Arc->NODE_TO->NAME );

    autfreeheap( Arc, sizeof( autarc_list ) );
    Graph->NUMBER_ARC--;
  }

  freechain( Node->ARC_FROM );

  for ( ScanChain  = Node->ARC_TO;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    Arc = (autarc_list *)ScanChain->DATA;
    ScanNode  = Arc->NODE_FROM;
    PrevChain = (chain_list **)0;

    for ( ScanChain  = ScanNode->ARC_FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      if ( ScanChain->DATA == Arc ) break;

      PrevChain = &ScanChain->NEXT;
    }

    *PrevChain = ScanChain->NEXT;
    ScanChain->NEXT = (chain_list *)0;
    freechain( ScanChain );

    if ( Arc->NEXT != (autarc_list *)0 )
    {
      Arc->NEXT->PREV = Arc->PREV;
    }

    delauth2elem( Graph->HASH_ARC, Arc->NODE_FROM->NAME, Arc->NODE_TO->NAME );

    autfreeheap( Arc, sizeof( autarc_list ) );
    Graph->NUMBER_ARC--;
  }

  freechain( Node->ARC_TO );

  delauthelem( Graph->HASH_NODE, Node->NAME );

  autfreeheap( Node, sizeof( autnode_list ) );
  Graph->NUMBER_NODE--;
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Del Arc                       |
|                                                             |
\------------------------------------------------------------*/

void delautgrapharc( Graph, Arc )

  autgraph     *Graph;
  autarc_list  *Arc;
{
  chain_list  **PrevChain;
  chain_list   *ScanChain;
  autnode_list *Node;

  Node = Arc->NODE_FROM;
  PrevChain = &Node->ARC_FROM;

  for ( ScanChain  = Node->ARC_FROM;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    if ( ScanChain->DATA == Arc ) break;
    PrevChain = &ScanChain->NEXT;
  }

  if ( ScanChain != (chain_list *)0 )
  {
    *PrevChain = ScanChain->NEXT;
    ScanChain->NEXT = (chain_list *)0;
    freechain( ScanChain );
  }

  Node = Arc->NODE_TO;
  PrevChain = &Node->ARC_TO;

  for ( ScanChain  = Node->ARC_TO;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    if ( ScanChain->DATA == Arc ) break;
    PrevChain = &ScanChain->NEXT;
  }

  if ( ScanChain != (chain_list *)0 )
  {
    *PrevChain = ScanChain->NEXT;
    ScanChain->NEXT = (chain_list *)0;
    freechain( ScanChain );
  }

  if ( Arc->NEXT != (autarc_list *)0 )
  {
    Arc->NEXT->PREV = Arc->PREV;
  }

  delauth2elem( Graph->HASH_ARC, Arc->NODE_FROM->NAME, Arc->NODE_TO->NAME );

  *Arc->PREV = Arc->NEXT;
  autfreeheap( Arc, sizeof( autarc_list ) );
  Graph->NUMBER_ARC--;
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph View                          |
|                                                             |
\------------------------------------------------------------*/

void viewautgraph( Graph, FuncViewNode, FuncViewArc )

  autgraph  *Graph;
  void     (*FuncViewNode)();
  void     (*FuncViewArc)();
{
  autnode_list *ScanNode;
  chain_list   *ScanChain;
  autarc_list  *ScanArc;

  for ( ScanNode  = Graph->NODE;
        ScanNode != (autnode_list *)0;
        ScanNode  = ScanNode->NEXT )
  {
    if ( FuncViewNode )
    {
      FuncViewNode( Graph, ScanNode );
    }
    else               
    {
      fprintf( stdout, "NODE: %s\n", ScanNode->NAME );
    }

    fprintf( stdout, "> ARC_FROM:\n" );

    for ( ScanChain  = ScanNode->ARC_FROM;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (autarc_list *)ScanChain->DATA;

      if ( FuncViewArc )
      {
        FuncViewArc( Graph, ScanArc );
      }
      else
      {
        fprintf( stdout, "> TO %s\n", ScanArc->NODE_TO->NAME );
      }
    }

    fprintf( stdout, "> ARC_TO:\n" );

    for ( ScanChain  = ScanNode->ARC_TO;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      ScanArc = (autarc_list *)ScanChain->DATA;

      if ( FuncViewArc )
      {
        FuncViewArc( Graph, ScanArc );
      }
      else
      {
        fprintf( stdout, "> FROM %s\n", ScanArc->NODE_FROM->NAME );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Destroy                       |
|                                                             |
\------------------------------------------------------------*/

void destroyautgraph( Graph )

  autgraph *Graph;
{
  autnode_list *ScanNode;
  autnode_list *DelNode;
  autarc_list  *ScanArc;
  autarc_list  *DelArc;

  ScanNode = Graph->NODE;

  while( ScanNode != (autnode_list *)0 )
  {
    DelNode  = ScanNode;
    ScanNode = ScanNode->NEXT;

    freechain( DelNode->ARC_FROM );
    freechain( DelNode->ARC_TO   );

    autfreeheap( DelNode, sizeof( autnode_list ) );
  }

  ScanArc = Graph->ARC;

  while( ScanArc != (autarc_list *)0 )
  {
    DelArc  = ScanArc;
    ScanArc = ScanArc->NEXT;

    autfreeheap( DelArc, sizeof( autarc_list ) );
  }

  destroyauthtable( Graph->HASH_NODE );
  destroyauth2table( Graph->HASH_ARC );

  autfreeheap( Graph, sizeof( autgraph ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Load Graph                    |
|                                                             |
\------------------------------------------------------------*/

autgraph *loadautgraph( FileName )

  char  *FileName;
{
  FILE         *GraphFile;
  autgraph     *Graph;
  char          Buffer[ 512 ];
  char         *Scan;
  char         *Name;
  char         *NameTo;
  autnode_list *NodeFrom; 
  autnode_list *NodeTo;
  autarc_list  *Arc;

  GraphFile = mbkfopen( FileName, "gr", "r" );

  if ( GraphFile == (FILE *)0 )
  {
    auterror( AUT_OPEN_FILE_ERROR, FileName );
  }

  Graph = createautgraph();

  while ( fgets( Buffer, 512, GraphFile ) != (char *)0 )
  {
    Scan = strchr( Buffer, '\n' );
    if ( Scan != (char *)0 ) *Scan = '\0';
    Scan = Buffer;

    if ( ( Buffer[ 0 ] == 'n' ) ||
         ( Buffer[ 0 ] == 'a' ) )
    {
      Scan = strchr( Scan, '\t' );
      if ( Scan == (char *)0 ) continue;

      Name = Scan + 1;
      Scan = strchr( Name, '\t' );
      if ( Scan == (char *)0 ) continue;
      *Scan = '\0';

      if ( Buffer[ 0 ] == 'n' )
      {
        NodeFrom = addautgraphnode( Graph, Name );
        NodeFrom->FLAGS = atol( Scan + 1 );
      }
      else
      {
        NameTo = Scan + 1;
        Scan   = strchr( NameTo, '\t' );
        if ( Scan == (char *)0 ) continue;
        *Scan = '\0';

        NodeFrom = addautgraphnode( Graph, Name   );
        NodeTo   = addautgraphnode( Graph, NameTo );

        Arc = addautgrapharc( Graph, NodeFrom, NodeTo );
        Arc->FLAGS = atol( Scan + 1 );
      }
    }
  }

  if ( fclose( GraphFile ) )
  {
    auterror( AUT_CLOSE_FILE_ERROR, FileName );
  }

  return( Graph );
}

/*------------------------------------------------------------\
|                                                             |
|                     Aut Graph Drive Graph                   |
|                                                             |
\------------------------------------------------------------*/

void driveautgraph( Graph, FileName )

  autgraph *Graph;
  char     *FileName;
{
  FILE         *GraphFile;
  autarc_list  *ScanArc;
  autnode_list *ScanNode; 

  GraphFile = mbkfopen( FileName, "gr", "w" );

  if ( GraphFile == (FILE *)0 )
  {
    auterror( AUT_OPEN_FILE_ERROR, FileName );
  }

  for ( ScanNode  = Graph->NODE;
        ScanNode != (autnode_list *)0;
        ScanNode  = ScanNode->NEXT )
  {
    fprintf( GraphFile, "node\t%s\t%ld\n",
             ScanNode->NAME,
             ScanNode->FLAGS );
  }

  for ( ScanArc  = Graph->ARC;
        ScanArc != (autarc_list *)0;
        ScanArc  = ScanArc->NEXT )
  {
    fprintf( GraphFile, "arc\t%s\t%s\t%ld\n", 
             ScanArc->NODE_FROM->NAME, 
             ScanArc->NODE_TO->NAME,
             ScanArc->FLAGS );
  }

  if ( fclose( GraphFile ) )
  {
    auterror( AUT_CLOSE_FILE_ERROR, FileName );
  }
}
