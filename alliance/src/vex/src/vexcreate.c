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
| Tool    :                     Vex                           |
|                                                             |
| File    :                  vexcreate.c                      |
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
# include "vex.h"

# include <stdio.h>
# include <string.h>
# include "vexcreate.h"
# include "vexerror.h"

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
|                          Varivexes                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Create Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Create Vex Atom Vector               |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexatomvec( Name, Left, Right )

  char *Name;
  short Left;
  short Right;
{
  vexexpr  *Node;
  short     Width;

  Name = namealloc( Name );
  Node = allocvexnode();

  SetVexNodeAtom( Node );
  SetVexAtomValue( Node, Name );
  SetVexAtomVector( Node );

  if ( ( Left  < 0 ) ||
       ( Right < 0 ) )
  {
    Node->WIDTH = 1;
    SetVexNodeVarWidth( Node );
  }
  else
  {
    if ( Left < Right )
    {
      Width = 1 + Right - Left;
    }
    else
    {
      Width = 1 + Left  - Right;
      SetVexAtomDown( Node );
    }

    Node->LEFT  = Left;
    Node->RIGHT = Right;
    Node->WIDTH = Width;
  }

  if ( Name[ 0 ] == '"' )
  {
    SetVexAtomLiteral( Node );
  }

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Vex Atom Literal              |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexatomlit( Name )

  char *Name;
{
  vexexpr  *Node;
  short     Width;

  Name = namealloc( Name );
  Node = allocvexnode();

  SetVexNodeAtom( Node );
  SetVexAtomValue( Node, Name );
  SetVexAtomLiteral( Node );

  Width = strlen( Name ) - 2;

  Node->LEFT  = Width - 1;
  Node->RIGHT = 0;
  Node->WIDTH = Width;

  if ( Width > 1 ) SetVexAtomVector( Node );
  else             SetVexAtomBit( Node );

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                     Create Vex Atom Literal Vector          |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexatomveclit( Literal, Width  )

  unsigned char Literal;
  short         Width;
{
  vexexpr *Node;
  char    *Name;
  char     Buffer[ 512 ];
  char     Cote;

  if ( Width > 1 ) Cote = '\"';
  else             Cote = '\'';

  Buffer[ 0         ] = Cote;
  Buffer[ Width + 1 ] = Cote;
  Buffer[ Width + 2 ] = '\0';

  memset( Buffer + 1, Literal, Width );

  Name = namealloc( Buffer );
  Node = allocvexnode();

  SetVexNodeAtom( Node );
  SetVexAtomValue( Node, Name );
  SetVexAtomLiteral( Node );

  Node->LEFT  = Width - 1;
  Node->RIGHT = 0;
  Node->WIDTH = Width;

  if ( Width > 1 ) SetVexAtomVector( Node );
  else             SetVexAtomBit( Node );

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Vex Atom Bit                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexatombit( Name )

  char *Name;
{
  vexexpr *Node;

  Name = namealloc( Name );

  Node = allocvexnode();

  SetVexNodeAtom( Node );
  SetVexAtomValue( Node, Name );
  SetVexAtomBit( Node );

  Node->WIDTH = 1;

  if ( Name[ 0 ] == '\'' )
  {
    SetVexAtomLiteral( Node );
  }

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                      Create Vex Oper                        |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexoper( Oper, Width )

  long  Oper;
  short Width;
{
  vexexpr *Node;

  if ( ( Oper < 0                 ) ||
       ( Oper >= VEX_MAX_OPERATOR ) )
  {
    vexerror( VEX_OPERATOR_ERROR, Oper );
  }

  Node = allocvexnode();

  if ( Width <= 0 )
  {
    Width = 1;
    SetVexNodeVarWidth( Node );
  }

  Node->WIDTH = Width;
  Node->LEFT  = Width - 1;

  SetVexNodeOper( Node );
  SetVexOperValue( Node, Oper );

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Vex Bin Expr                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexbinexpr( Oper, Width, Expr1, Expr2 )

  long      Oper;
  short     Width;
  vexexpr  *Expr1;
  vexexpr  *Expr2;
{
  vexexpr *Expr;

  if ( ! isvexbinaryoper( Oper ) )
  {
    vexerror( VEX_OPERATOR_ERROR, Oper );
  }

  Expr = createvexoper( Oper, Width );

  addvexhexpr( Expr, Expr2 );
  addvexhexpr( Expr, Expr1 );

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Create Vex Unary Expr                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexunaryexpr( Oper, Width, Expr )

  long      Oper;
  short     Width;
  vexexpr  *Expr;
{
  vexexpr *Expr1;

  if ( ! isvexunaryoper( Oper ) )
  {
    vexerror( VEX_NOT_UNARY_ERROR, Oper );
  }

  Expr1 = createvexoper( Oper, Width );
  addvexhexpr( Expr1, Expr );

  return( Expr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                   Create Vex Ternary Expr                   |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexternaryexpr( Oper, Width, Expr1, Expr2, Expr3 )

  long      Oper;
  short     Width;
  vexexpr  *Expr1;
  vexexpr  *Expr2;
  vexexpr  *Expr3;
{
  vexexpr *Expr;

  if ( ! isvexternaryoper( Oper ) )
  {
    vexerror( VEX_OPERATOR_ERROR, Oper );
  }

  Expr = createvexoper( Oper, Width );

  addvexhexpr( Expr, Expr3 );
  addvexhexpr( Expr, Expr2 );
  addvexhexpr( Expr, Expr1 );

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Create Vex Func                        |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexfunc( Func, Width )

  char  *Func;
  short  Width;
{
  vexexpr *Node;

  Node = allocvexnode();

  if ( Width <= 0 )
  {
    Width = 1;
    SetVexNodeVarWidth( Node );
  }

  Node->WIDTH = Width;
  Node->LEFT  = Width - 1;

  SetVexNodeFunc( Node );
  SetVexFuncValue( Node, Func );

  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                   Create Vex Atom Long Array                |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexatomlongarray( Array, Size, Width, Signed )

  long  *Array;
  short  Size;
  short  Width;
  short  Signed;
{
  vexexpr       *Node;
  char           Buffer[ 512 ];
  long           Integer;
  short          Index;
  short          MaxWidth;
  int            Scan;
  unsigned long  Mask;

  if ( Size == 0 ) return( (vexexpr *)0 );

  Node = allocvexnode();

  MaxWidth = Size * 32;

  if ( Width == 0 )
  {
    Width = MaxWidth;
    Index = 0;
    Mask  = 1L << 31;

    Integer = Array[ 0 ];

    if ( Integer & Mask )
    {
      if ( Signed )
      {
        while ( ( Integer & Mask ) != 0 )
        {
          Width--; Mask = Mask >> 1;
  
          if ( Mask == 0 )
          {
            Index++;
  
            if ( Index == Size ) break;

            Integer = Array[ Index ];
            Mask    = 1L << 31;
          }
        }
  
        Width++;

        Signed = 1;
      }
    }
    else
    {
      while ( ( Integer & Mask ) == 0 )
      {
        Width--; Mask = Mask >> 1;
  
        if ( Mask == 0 )
        {
          Index++;
  
          if ( Index == Size ) break;

          Integer = Array[ Index ];
          Mask    = 1L << 31;
        }
      }

      if ( ( Signed == 1 ) || 
           ( Width  == 0 ) ) Width++;
    }
  }

  if ( Width > MaxWidth ) return( (vexexpr *)0 );

  Node->WIDTH = Width;
  Node->LEFT  = Width - 1;
  Node->RIGHT = 0;

  Scan  = 1;
  Index = Size - ((Width + 31) / 32);
  Mask  = 1L << ((Width - 1) % 32);

  do
  {
    if ( Array[ Index ] & Mask ) Buffer[ Scan ] = '1';
    else                         Buffer[ Scan ] = '0';

    Scan = Scan + 1;
    Mask = Mask >> 1;

    if ( Mask == 0 )
    {
      Mask = 1L << 31; Index++;
    }
  }
  while ( Index < Size );

  if ( Width == 1 )
  {
    Buffer[ 0      ] = '\'';
    Buffer[ Scan++ ] = '\'';

    SetVexAtomBit( Node );
  }
  else
  {
    Buffer[ 0      ] = '"';
    Buffer[ Scan++ ] = '"';

    SetVexAtomVector( Node );
  }

  Buffer[ Scan ] = '\0';

  if ( Signed == 1 ) SetVexNodeSigned( Node );

  SetVexNodeAtom( Node );
  SetVexAtomValue( Node, namealloc( Buffer ) );
  SetVexAtomLiteral( Node );
  
  return( Node );
}

/*------------------------------------------------------------\
|                                                             |
|                   Create Vex Atom Long                      |
|                                                             |
\------------------------------------------------------------*/

vexexpr *createvexatomlong( Integer, Width, Signed )

  long  Integer;
  short Width;
  short Signed;
{
  return( createvexatomlongarray( &Integer, 1, Width, Signed ) );
}

