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
# include <stdio.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"

static char Buffer[ 512 ];

ablexpr *bitstring2abl( Variable, Left, Right, BitString )

  char *Variable;
  int   Left;
  int   Right;
  char *BitString;
{
  ablexpr *Equation;
  ablexpr *Expr;
  int         Reverse;
  int         BitIndex;
  int         Index;

  if ( Left > Right )
  {
    Reverse  = Right;
    Right    = Left;
    Left     = Reverse;

    Reverse  = 1;
    BitIndex = Right - Left;
  }
  else
  {
    Reverse  = 0;
    BitIndex = 0;
  }

  Equation = createabloper( ABL_AND );

  for ( Index = Left; Index <= Right; Index++ )
  {
    sprintf( Buffer, "%s %d", Variable, Index );

    if ( BitString[ BitIndex ] == '0' )
    {
      addablhexpr( Equation, createablnotexpr( createablatom( Buffer ) ) );
    }
    else
    if ( BitString[ BitIndex ] == '1' )
    {
      addablhexpr( Equation, createablatom( Buffer ) );
    }
    else break;

    if ( Reverse ) BitIndex--;
    else           BitIndex++;
  }

  if ( ABL_CDR( Equation ) == (ablexpr *)0 )
  {
    delablexpr( Equation );
    Expr = createablatom( "'1'" );
  }
  else
  if ( ABL_CDDR( Equation ) == (ablexpr *)0 )
  {
    Expr = dupablexpr( ABL_CADR( Equation ) );
    delablexpr( Equation );
  }
  else
  {
    Expr = Equation;
  }

  return( Expr );
}

int main( argc, argv )

  int    argc;
  char **argv;
{
  ablexpr *Expr1;

  mbkenv();
  autenv();
  ablenv();

  Expr1 = createablbinexpr( ABL_XOR,
                         createablatom( "'1'" ),
                         createablatom( "a" ) );
  Expr1 = simpablexpr( Expr1 );
  viewablexpr( Expr1, ABL_VIEW_VHDL );
 
  return( 0 );

/*\
  if ( argc < 5 )
  {
    fprintf( stdout, "%s Variable Left Right BitString\n", argv[0] );
  }
  else
  {
    Expr = bitstring2abl( argv[1], atoi( argv[2] ), atoi( argv[3] ), argv[4] );
    viewablexpr( Expr, ABL_VIEW_VHDL );
    fprintf( stdout, "\n" );
    delablexpr( Expr );
  }
\*/
}
