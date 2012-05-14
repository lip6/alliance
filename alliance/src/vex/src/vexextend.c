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
| Tool    :                     Vex                           |
|                                                             |
| File    :                 vexextend.c                       |
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
# include "vexextend.h"
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
|                        Extend Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Extend Vex Atom Sign                   |
|                                                             |
\------------------------------------------------------------*/

vexexpr *extendvexatomsign( Expr, Width )

  vexexpr *Expr;
  short    Width;
{
  char    *Value;
  vexexpr *Atom;
  vexexpr *Result;
  char     Buffer[ 128 ];
  int      Index;
  int      Target;
  int      Extend;
  char     Sign;
  char     Limit;

  if ( IsVexNodeAtom( Expr ) )
  {
    Value = GetVexAtomValue( Expr );

    if ( IsVexAtomLiteral( Expr ) )
    {
      if ( Expr->WIDTH < Width )
      {
/*
**    "1xxxx" -> "1111xxxx"
*/
        Extend = Width - Expr->WIDTH;

        if ( IsVexNodeSigned( Expr ) ) Sign = Value[ 1 ];
        else                           Sign = '0';

        Target = 0;
        Buffer[ Target++ ] = '"';

        for ( Index = 1; Index <= Extend; Index++ )
        {
          Buffer[ Target++ ] = Sign;
        }

        for ( Index = 1; Index <= Expr->WIDTH; Index++ )
        {
          Buffer[ Target++ ] = Value[ Index ];
        }

        Buffer[ Target++ ] = '"';
        Buffer[ Target++ ] = '\0';

        SetVexAtomValue( Expr, namealloc( Buffer ) );

        Expr->WIDTH = Width;
        Expr->LEFT  = Width - 1;
        Expr->RIGHT = 0;
      }
      else
      if ( Expr->WIDTH > Width )
      {
/*
**    "1111xxxx" -> "1xxxx"
*/
        Extend = (Expr->WIDTH + 1) - Width;

        if ( IsVexNodeSigned( Expr ) ) Sign = Value[ 1 ];
        else                           Sign = '0';

        Target = 0;

        if ( Width > 1 ) Limit = '"';
        else             Limit = '\'';

        Buffer[ Target++ ] = Limit;

        for ( Index = 1; Index <= Extend; Index++ )
        {
          if ( Value[ Index ] != Sign ) return( (vexexpr *)0 );
        }

        for ( Index = Extend; Index <= Expr->WIDTH; Index++ )
        {
          Buffer[ Target++ ] = Value[ Index ];
        }

        Buffer[ Target++ ] = Limit;
        Buffer[ Target++ ] = '\0';

        SetVexAtomValue( Expr, namealloc( Buffer ) );

        Expr->WIDTH = Width;
        Expr->LEFT  = Width - 1;
        Expr->RIGHT = 0;
      }

      return( Expr );
    }
    else
    {
      if ( Expr->WIDTH < Width )
      {
/*
**    "1xxxx" -> "1111xxxx"
*/
        Extend = Width - Expr->WIDTH;

        if ( IsVexNodeSigned( Expr ) )
        {
          Value  = GetVexAtomValue( Expr );
          Result = createvexoper( VEX_CONCAT, Width );

          addvexhexpr( Result, Expr );

          for ( Index = 1; Index <= Extend; Index++ )
          {
            if ( IsVexAtomVector( Expr ) )
            {
              Atom = createvexatomvec( Value, Expr->LEFT, Expr->LEFT );
            }
            else
            {
              Atom = createvexatombit( Value );
            }

            addvexhexpr( Result, Atom );
          }

          Expr = Result;

          Expr->WIDTH = Width;
          Expr->LEFT  = Width - 1;
          Expr->RIGHT = 0;

          SetVexNodeSigned( Expr );
        }
        else
        {
          Target = 0;

          if ( Extend > 1 ) Limit = '"';
          else              Limit = '\'';

          Buffer[ Target++ ] = Limit;

          for ( Index = 1; Index <= Extend; Index++ )
          {
            Buffer[ Target++ ] = '0';
          }

          Buffer[ Target++ ] = Limit;
          Buffer[ Target++ ] = '\0';

          Atom = createvexatomlit( Buffer );
          Expr = createvexbinexpr( VEX_CONCAT, 0, Atom, Expr );

          Expr->LEFT  = Width - 1;
          Expr->RIGHT = 0;
          Expr->WIDTH = Width;
        }

        return( Expr );
      }
      else
      {
        Expr->WIDTH = Width;

        if ( IsVexAtomDown( Expr ) )
        {
          Expr->LEFT = Expr->RIGHT + Width - 1;
        }
        else
        {
          Expr->LEFT = Expr->RIGHT - Width + 1;
        }

        return( Expr );
      }
    }
  }

  return( (vexexpr *)0 );
}
