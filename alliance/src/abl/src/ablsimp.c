/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr         |
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
| Tool    :                     Abl                           |
|                                                             |
| File    :                   ablsimp.c                       |
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

# include <stdio.h>
# include "ablsimp.h"
# include "ablerror.h"

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

  static long AblSimplifyTable[ ABL_MAX_OPERATOR ][ 4 ] =

  {
    {  1,  1,  0,  0  }, /*  ABL_OR          */
    {  0,  0,  1,  1  }, /*  ABL_AND         */
    {  1, -1,  0,  0  }, /*  ABL_XOR         */

    { -1, -1, -1, -1  }, /*  ABL_NOT         */
    { -1, -1, -1, -1  }, /*  ABL_NOR         */
    { -1, -1, -1, -1  }, /*  ABL_NAND        */
    { -1, -1, -1, -1  }, /*  ABL_NXOR        */
    { -1, -1, -1, -1  }, /*  ABL_STABLE      */
    { -1, -1, -1, -1  }, /*  ABL_AF          */
    { -1, -1, -1, -1  }, /*  ABL_AG          */
    { -1, -1, -1, -1  }, /*  ABL_AX          */
    { -1, -1, -1, -1  }, /*  ABL_AU          */
    { -1, -1, -1, -1  }, /*  ABL_EF          */
    { -1, -1, -1, -1  }, /*  ABL_EG          */
    { -1, -1, -1, -1  }, /*  ABL_EX          */
    { -1, -1, -1, -1  }  /*  ABL_EU          */
  }; 

  static char AblFirstSimplify = 1;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Simplify Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    Local Simplify Initialize                |
|                                                             |
\------------------------------------------------------------*/

void loc_initablsimp()
{
  int Line;
  int Row;

  if ( AblFirstSimplify )
  {
    AblFirstSimplify = 0;

    for ( Line = 0; Line < ABL_MAX_OPERATOR; Line++ )
    {
      for ( Row = 0; Row < 4; Row++ )
      {
        if ( AblSimplifyTable[ Line ][ Row ] == 1 )
        {
          AblSimplifyTable[ Line ][ Row ] = (long)ABL_ATOM_NAME_ONE;
        }
        else
        if ( AblSimplifyTable[ Line ][ Row ] == 0 )
        {
          AblSimplifyTable[ Line ][ Row ] = (long)ABL_ATOM_NAME_ZERO;
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 Simplify Duplicate Abl Expr                 |
|                                                             |
\------------------------------------------------------------*/

ablexpr *simpdupexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ExprSimp;
  ablexpr *ExprQueue;
  ablexpr *ScanExpr;
  char       *AtomValue;
  long        Oper;
  char        Negative;

  if ( ABL_ATOM( Expr ) )
  {
    return( addchain( (ablexpr *)0, ABL_CAR( Expr ) ) );
  }

  Oper = ABL_OPER( Expr );

  if ( Oper == ABL_NOT )
  {
    ExprSimp = simpdupexpr( ABL_CADR( Expr ) );
    ExprSimp = optimablnotexpr( ExprSimp );

    return( ExprSimp );
  }

  if ( getabloperpolar( Oper ) == ABL_POLAR_NEGATIVE )
  {
    Oper = getablopernot( Oper );
    Negative = 1;
  }
  else
  {
    Negative = 0;
  }

  ScanExpr = Expr;
  Expr     = addchain( (ablexpr *)0,
                       (void *)addchain( (ablexpr *)0,
                                         (void *)Oper ) );
  if ( isablunaryoper( Oper ) )
  {
    ExprSimp = simpdupexpr( ABL_CADR( ScanExpr ) );
    ABL_CDR( Expr ) = addchain( (ablexpr *)0, (void *)ExprSimp );

    return( Expr );
  }

  ExprQueue = Expr;

  while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
  {
    ExprSimp = simpdupexpr( ABL_CAR( ScanExpr ) );

    if ( ABL_ATOM( ExprSimp ) )
    {
      AtomValue = ABL_ATOM_VALUE( ExprSimp );

      if ( AtomValue == (char *)AblSimplifyTable[ Oper ][ 0 ] )
      {
        if ( AblSimplifyTable[ Oper ][ 1 ] != -1 )
        {
          freeablexpr( Expr );
          ABL_ATOM_VALUE( ExprSimp ) = (char *)AblSimplifyTable[ Oper ][ 1 ];

          if ( Negative == 1 )
          {
            return( optimablnotexpr( ExprSimp ) );
          }

          return( ExprSimp );
        }

        freeablexpr( ExprSimp );
        Negative = ! Negative;
      }
      else
      if ( AtomValue == (char *)AblSimplifyTable[ Oper ][ 2 ] )
      {
        freeablexpr( ExprSimp );
      }
      else
      {
        ABL_CDR( ExprQueue ) = addchain( (ablexpr *)0, (void *)ExprSimp );
        ExprQueue = ABL_CDR( ExprQueue );
      }
    }
    else
    {
      ABL_CDR( ExprQueue ) = addchain( (ablexpr *)0, (void *)ExprSimp );
      ExprQueue = ABL_CDR( ExprQueue );
    }
  }

  if ( ExprQueue == Expr )
  {
    ExprSimp = ABL_CAR( Expr );
    ABL_ATOM_VALUE( ExprSimp ) = (char *)AblSimplifyTable[ Oper ][ 3 ];
    freechain( Expr );
  }
  else
  if ( ExprQueue == ABL_CDR( Expr ) )
  {
    ExprSimp = ABL_CADR( Expr );
    ABL_CADR( Expr ) = (void *)0;
    freeablexpr( Expr );
  }
  else
  {
    ExprSimp = Expr;
  }

  if ( Negative == 1 )
  {
    return( optimablnotexpr( ExprSimp ) );
  }

  return( ExprSimp );
}

/*------------------------------------------------------------\
|                                                             |
|                      Simplify Duplicate Abl Expr            |
|                                                             |
\------------------------------------------------------------*/

ablexpr *simpdupablexpr( Expr )

  ablexpr *Expr;
{
  loc_initablsimp();

  return( simpdupexpr( Expr ) );
}

/*------------------------------------------------------------\
|                                                             |
|                 Local Simplify Abl Expr                     |
|                                                             |
\------------------------------------------------------------*/

ablexpr *loc_simpablexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *ExprSimp;
  ablexpr *ScanExpr;
  ablexpr *PrevExpr;
  char    *AtomValue;
  long     Oper;
  char     Negative;

  if ( ABL_ATOM( Expr ) )
  {
    return( Expr );
  }

  Oper = ABL_OPER( Expr );

  if ( Oper == ABL_NOT )
  {
    ExprSimp = loc_simpablexpr( ABL_CADR( Expr ) );
    freechain( ABL_CAR( Expr ) );
    freechain( Expr );
    ExprSimp = optimablnotexpr( ExprSimp );

    return( ExprSimp );
  }

  if ( isablunaryoper( Oper ) )
  {
    ABL_CADR( Expr ) = loc_simpablexpr( ABL_CADR( Expr ) );

    return( Expr );
  }

  if ( getabloperpolar( Oper ) == ABL_POLAR_NEGATIVE )
  {
    Oper = getablopernot( Oper );
    SET_ABL_OPER( Expr, Oper );
    Negative = 1;
  }
  else
  {
    Negative = 0;
  }

  ScanExpr = Expr;
  PrevExpr = Expr;

  while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
  {
    ExprSimp = loc_simpablexpr( ABL_CAR( ScanExpr ) );

    if ( ABL_ATOM( ExprSimp ) )
    {
      AtomValue = ABL_ATOM_VALUE( ExprSimp );

      if ( AtomValue == (char *)AblSimplifyTable[ Oper ][ 0 ] )
      {
        if ( AblSimplifyTable[ Oper ][ 1 ] != -1 )
        {
          ABL_CAR( ScanExpr ) = (ablexpr *)0;
          freeablexpr( Expr );
          ABL_ATOM_VALUE( ExprSimp ) = (char *)AblSimplifyTable[ Oper ][ 1 ];

          if ( Negative == 1 )
          {
            ExprSimp = optimablnotexpr( ExprSimp );
          }

          return( ExprSimp );
        }

        freeablexpr( ExprSimp );

        ABL_CDR( PrevExpr ) = ABL_CDR( ScanExpr );
        ABL_CDR( ScanExpr ) = (ablexpr *)0;
        freechain( ScanExpr );

        ScanExpr = PrevExpr;
        Negative = ! Negative;
      }
      else
      if ( AtomValue == (char *)AblSimplifyTable[ Oper ][ 2 ] )
      {
        freeablexpr( ExprSimp );

        ABL_CDR( PrevExpr ) = ABL_CDR( ScanExpr );
        ABL_CDR( ScanExpr ) = (ablexpr *)0;
        freechain( ScanExpr );

        ScanExpr = PrevExpr;
      }
      else
      {
        ABL_CAR( ScanExpr ) = ExprSimp;
      }
    }
    else
    {
      ABL_CAR( ScanExpr ) = ExprSimp;
    }

    PrevExpr = ScanExpr;
  }

  if ( PrevExpr == Expr )
  {
    ABL_ATOM_VALUE( Expr ) = (char *)AblSimplifyTable[ Oper ][ 3 ];
    ExprSimp = Expr;
  }
  else
  if ( PrevExpr == ABL_CDR( Expr ) )
  {
    ExprSimp = ABL_CADR( Expr );
    ABL_CADR( Expr ) = (void *)0;
    freeablexpr( Expr );
  }
  else
  {
    ExprSimp = Expr;
  }

  if ( Negative == 1 )
  {
    ExprSimp = optimablnotexpr( ExprSimp );
  }

  return( ExprSimp );
}

/*------------------------------------------------------------\
|                                                             |
|                      Simplify Abl Expr                      |
|                                                             |
\------------------------------------------------------------*/

ablexpr *simpablexpr( Expr )

  ablexpr *Expr;
{
  loc_initablsimp();

  return( loc_simpablexpr( Expr ) );
}
