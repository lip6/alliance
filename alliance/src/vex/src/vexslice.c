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
| File    :                 vexslice.c                        |
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
# include "vexslice.h"
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

  static char VexBuffer[ 128 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Slice Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Slice Vex Atom Vector                  |
|                                                             |
\------------------------------------------------------------*/

vexexpr *slicevexatomvec( Expr, Left, Right )

  vexexpr *Expr;
  short    Left;
  short    Right;
{
  short   Width;
  short   PosLeft;
  short   PosRight;
  short   PosMin;
  short   PosMax;
  char    Separ;

/*\
fprintf( stdout, "slicevexatomvec %d %d\n", Left, Right );
viewvexexprbound( Expr );
fprintf( stdout, " --> " );
\*/

  if ( ( IsVexNodeAtom( Expr   ) ) &&
       ( IsVexAtomVector( Expr ) ) )
  {
    PosLeft  = getvexvectorpos( Expr, Left  );
    PosRight = getvexvectorpos( Expr, Right );

    if ( ( PosLeft  >= 0 ) &&
         ( PosRight >= 0 ) )
    {
      if ( Left < Right )
      {
        Width = 1 + Right - Left;
        SetVexAtomUp( Expr );
      }
      else
      {
        Width = 1 + Left  - Right;
        SetVexAtomDown( Expr );
      }
  
      if ( IsVexAtomLiteral( Expr ) )
      {
        if ( Width == 1 ) Separ = '\'';
        else              Separ = '"';
  
        if ( PosLeft > PosRight )
        {
          PosMin = PosRight;
          PosMax = PosLeft;
        }
        else
        {
          PosMin = PosLeft;
          PosMax = PosRight;
        }
  
        strcpy( VexBuffer, GetVexAtomValue( Expr ) );
        VexBuffer[ PosMin     ] = Separ;
        VexBuffer[ PosMax + 2 ] = Separ;
        VexBuffer[ PosMax + 3 ] = '\0';
  
        SetVexAtomValue( Expr, namealloc( &VexBuffer[ PosMin ] ) );
      }

      Expr->LEFT  = Left;
      Expr->RIGHT = Right;
      Expr->WIDTH = Width;
/*\
viewvexexprbound( Expr );
fprintf( stdout, "\n" );
\*/
      return( Expr );
    }
  }

  return( (vexexpr *)0 );
}
