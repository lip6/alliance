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
| File    :                   vexeval.c                       |
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
# include "vexeval.h"
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
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  char VexNotLiteral[ VEX_MAX_ID ] =
  {
/*  ------------------------------------------  */
/*   U    X    0    1    Z    W    L    H    -  */
/*  ------------------------------------------  */
    'u', 'x', '1', '0', 'x', 'x', '1', '0', 'x'
  };

  char VexAndLiteral[ VEX_MAX_ID ][ VEX_MAX_ID ] =
  {
/*    ------------------------------------------  */
/*     U    X    0    1    Z    W    L    H    -  */
/*    ------------------------------------------  */
    { 'u', 'u', '0', 'u', 'u', 'u', '0', 'u', 'u' },  /* | u |  */
    { 'u', 'x', '0', 'x', 'x', 'x', '0', 'x', 'x' },  /* | x |  */
    { '0', '0', '0', '0', '0', '0', '0', '0', '0' },  /* | 0 |  */
    { 'u', 'x', '0', '1', 'x', 'x', '0', '1', 'x' },  /* | 1 |  */
    { 'u', 'x', '0', 'x', 'x', 'x', '0', 'x', 'x' },  /* | z |  */
    { 'u', 'x', '0', 'x', 'x', 'x', '0', 'x', 'x' },  /* | w |  */
    { '0', '0', '0', '0', '0', '0', '0', '0', '0' },  /* | l |  */
    { 'u', 'x', '0', '1', 'x', 'x', '0', '1', 'x' },  /* | h |  */
    { 'u', 'x', '0', 'x', 'x', 'x', '0', 'x', 'x' }   /* | - |  */
  };

  char VexOrLiteral[ VEX_MAX_ID ][ VEX_MAX_ID ] =
  {
/*    ------------------------------------------  */
/*     U    X    0    1    Z    W    L    H    -  */
/*    ------------------------------------------  */
    { 'u', 'u', 'u', '1', 'u', 'u', 'u', '1', 'u' },  /* | u |  */
    { 'u', 'x', 'x', '1', 'x', 'x', 'x', '1', 'x' },  /* | x |  */
    { 'u', 'x', '0', '1', 'x', 'x', '0', '1', 'x' },  /* | 0 |  */
    { '1', '1', '1', '1', '1', '1', '1', '1', '1' },  /* | 1 |  */
    { 'u', 'x', 'x', '1', 'x', 'x', 'x', '1', 'x' },  /* | z |  */
    { 'u', 'x', 'x', '1', 'x', 'x', 'x', '1', 'x' },  /* | w |  */
    { 'u', 'x', '0', '1', 'x', 'x', '0', '1', 'x' },  /* | l |  */
    { '1', '1', '1', '1', '1', '1', '1', '1', '1' },  /* | h |  */
    { 'u', 'x', 'x', '1', 'x', 'x', 'x', '1', 'x' }   /* | - |  */
  };

  char VexXorLiteral[ VEX_MAX_ID ][ VEX_MAX_ID ] =
  {
/*    ------------------------------------------  */
/*     U    X    0    1    Z    W    L    H    -  */
/*    ------------------------------------------  */
    { 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u', 'u' },  /* | u |  */
    { 'u', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' },  /* | x |  */
    { 'u', 'x', '0', '1', 'x', 'x', '0', '1', 'x' },  /* | 0 |  */
    { 'u', 'x', '1', '0', 'x', 'x', '1', '0', 'x' },  /* | 1 |  */
    { 'u', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' },  /* | z |  */
    { 'u', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' },  /* | w |  */
    { 'u', 'x', '0', '1', 'x', 'x', '0', '1', 'x' },  /* | l |  */
    { 'u', 'x', '1', '0', 'x', 'x', '1', '0', 'x' },  /* | h |  */
    { 'u', 'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' }   /* | - |  */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Eval Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Eval Vex Atom Long                       |
|                                                             |
\------------------------------------------------------------*/

int evalvexatomlong( Expr, PLong )

  vexexpr *Expr;
  long    *PLong;
{
  char *Literal;
  long  Value;
  int   Index;
  int   IndexStep;
  int   IndexMax;
  int   Error;

  if ( ( ! IsVexNodeAtom( Expr )  ) ||
       ( ! IsVexAtomLiteral( Expr ) ) )
  {
    return( -1 );
  }

  Literal = GetVexAtomValue( Expr );
  Error   = 0;

  Index     = 1;
  IndexStep = 1;
  IndexMax  = Expr->WIDTH + 1;

  if ( ( IsVexNodeSigned( Expr ) ) &&
       ( Literal[ Index ] == '1' ) ) Value = -1;
  else                               Value =  0;

  do
  {
    Value = Value << 1;

    if ( Literal[ Index ] == '1' )
    {
      Value |= 1;
    }
    else
    if ( Literal[ Index ] != '0' )
    {
      Error = -1;
    }

    Index += IndexStep;
  }
  while ( Index != IndexMax );

  *PLong = Value;

  return( Error );
}

/*------------------------------------------------------------\
|                                                             |
|                    Eval Vex Not Literal                     |
|                                                             |
\------------------------------------------------------------*/

int evalvexnotliteral( Literal, PChar )

  char  Literal;
  char *PChar;
{
  int  Index;

  Index = getvexliteralid( Literal );

  if ( Index == -1 )
  {
    return( -1 );
  }

  *PChar = VexNotLiteral[ Index ];

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Eval Vex And Literal                     |
|                                                             |
\------------------------------------------------------------*/

int evalvexandliteral( Literal1, Literal2, PChar )

  char  Literal1;
  char  Literal2;
  char *PChar;
{
  int Index1;
  int Index2;

  Index1 = getvexliteralid( Literal1 );
  Index2 = getvexliteralid( Literal2 );

  if ( ( Index1 == -1 ) ||
       ( Index2 == -1 ) )
  {
    return( -1 );
  }

  *PChar = VexAndLiteral[ Index1 ][ Index2 ];

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Eval Vex Or  Literal                     |
|                                                             |
\------------------------------------------------------------*/

int evalvexorliteral( Literal1, Literal2, PChar )

  char  Literal1;
  char  Literal2;
  char *PChar;
{
  int Index1;
  int Index2;

  Index1 = getvexliteralid( Literal1 );
  Index2 = getvexliteralid( Literal2 );

  if ( ( Index1 == -1 ) ||
       ( Index2 == -1 ) )
  {
    return( -1 );
  }

  *PChar = VexOrLiteral[ Index1 ][ Index2 ];

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Eval Vex Xor Literal                     |
|                                                             |
\------------------------------------------------------------*/

int evalvexxorliteral( Literal1, Literal2, PChar )

  char  Literal1;
  char  Literal2;
  char *PChar;
{
  int Index1;
  int Index2;

  Index1 = getvexliteralid( Literal1 );
  Index2 = getvexliteralid( Literal2 );

  if ( ( Index1 == -1 ) ||
       ( Index2 == -1 ) )
  {
    return( -1 );
  }

  *PChar = VexXorLiteral[ Index1 ][ Index2 ];

  return( 0 );
}
