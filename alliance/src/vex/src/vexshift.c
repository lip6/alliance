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
| Tool    :                     Vex                           |
|                                                             |
| File    :                  vexshift.c                       |
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
# include "vexshift.h"
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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Shift Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Shift Vex Atom Left                   |
|                                                             |
\------------------------------------------------------------*/

vexexpr *shiftvexatomleft( Expr, Count  )

  vexexpr *Expr;
  short    Count;
{
  char    *Value;
  vexexpr *VexZero;
  short    Width;

  if ( IsVexNodeAtom( Expr ) )
  {
    Value = GetVexAtomValue( Expr );
    Width = Expr->WIDTH;

    if ( Count >= Width )
    {
/*
**    "xxxx...x" -> "0000...0"
*/
      VexZero = createvexatomveclit( VEX_ZERO, Width );
      freevexexpr( Expr );

      Expr = VexZero;
    }
    else
    if ( Count >= 1 )
    {
/*
**    "xxxx...x" -> "xxxx...0"
*/
      VexZero = createvexatomveclit( VEX_ZERO, Count );

      if ( IsVexAtomDown( Expr ) )
      {
        Expr = slicevexatomvec( Expr, Expr->LEFT - Count, Expr->RIGHT );
      }
      else
      {
        Expr = slicevexatomvec( Expr, Expr->LEFT + Count, Expr->RIGHT );
      }

      Expr = createvexbinexpr( VEX_CONCAT, Width, Expr, VexZero );
      Expr = simpvexexpr( Expr );
    }

    return( Expr );
  }

  return( (vexexpr *)0 );
}

/*------------------------------------------------------------\
|                                                             |
|                       Shift Vex Atom Right                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *shiftvexatomright( Expr, Count  )

  vexexpr *Expr;
  short     Count;
{
  char    *Value;
  vexexpr *VexSign;
  vexexpr *VexSignBit;
  short    Width;
  short    Index;

  if ( IsVexNodeAtom( Expr ) )
  {
    Value = GetVexAtomValue( Expr );
    Width = Expr->WIDTH;

    if ( Count >= Width )
    {
      if ( IsVexAtomSigned( Expr ) )
      {
/*
**    "sxxx...x" -> "ssss...s"
*/
        if ( IsVexAtomLiteral( Expr ) )
        {
          VexSign = createvexatomveclit( Value[ 1 ], Width );
        }
        else
        {
          VexSignBit = createvexatomvec( Value, Expr->LEFT, Expr->LEFT );
          VexSign    = VexSignBit;

          for ( Index = 1; Index < Width; Index++ )
          {
            VexSign = createvexbinexpr( VEX_CONCAT, VexSign->WIDTH + 1,
                                        VexSign, dupvexexpr( VexSignBit ) );
          }
        }
      }
      else
      {
/*
**    "xxxx...x" -> "0000...0"
*/
        VexSign = createvexatomveclit( VEX_ZERO, Width );
        freevexexpr( Expr );
      }

      Expr = VexSign;
    }
    else
    if ( Count >= 1 )
    {
      if ( IsVexAtomSigned( Expr ) )
      {
/*
**    "sxxx...x" -> "ss..xxxx"
*/
        if ( IsVexAtomLiteral( Expr ) )
        {
          VexSign = createvexatomveclit( Value[ 1 ], Count );
        }
        else
        {
          VexSignBit = createvexatomvec( Value, Expr->LEFT, Expr->LEFT );
          VexSign    = VexSignBit;

          for ( Index = 1; Index < Count; Index++ )
          {
            VexSign = createvexbinexpr( VEX_CONCAT, VexSign->WIDTH + 1,
                                        VexSign, dupvexexpr( VexSignBit ) );
          }
        }
      }
      else
      {
/*
**    "xxxx...x" -> "00..xxxx"
*/
        VexSign = createvexatomveclit( VEX_ZERO, Count );
      }

      if ( IsVexAtomDown( Expr ) )
      {
        Expr = slicevexatomvec( Expr, Expr->LEFT, Expr->RIGHT + Count );
      }
      else
      {
        Expr = slicevexatomvec( Expr, Expr->LEFT, Expr->RIGHT - Count );
      }

      Expr = createvexbinexpr( VEX_CONCAT, Width, VexSign, Expr );
      Expr = simpvexexpr( Expr );
    }

    return( Expr );
  }

  return( (vexexpr *)0 );
}
