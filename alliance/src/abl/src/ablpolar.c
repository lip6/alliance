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
| Tool    :                     Abl                           |
|                                                             |
| File    :                  ablpolar.c                       |
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
# include "ablpolar.h"
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

  static long AblPolarOperTable[ ABL_MAX_OPERATOR ][ 3 ] =
  {
  /*                                                      */
  /*  Negative   Positive   Polarity                      */
  /*                                                      */
    { ABL_AND  , ABL_OR   , 0        },  /* ABL_OR        */
    { ABL_OR   , ABL_AND  , 0        },  /* ABL_AND       */
    { ABL_NXOR , ABL_XOR  , -1       },  /* ABL_XOR       */
    { -1       , -1       , 0        },  /* ABL_NOT       */
    { ABL_OR   , ABL_AND  , 1        },  /* ABL_NOR       */
    { ABL_AND  , ABL_OR   , 1        },  /* ABL_NAND      */
    { ABL_XOR  , ABL_NXOR , -1       },  /* ABL_NXOR      */
    { -1       , -1       , 0        },  /* ABL_STABLE    */
    { -1       , -1       , 0        },  /* ABL_AF        */
    { -1       , -1       , 0        },  /* ABL_AG        */
    { -1       , -1       , 0        },  /* ABL_AX        */
    { -1       , -1       , 0        },  /* ABL_AU        */
    { -1       , -1       , 0        },  /* ABL_EF        */
    { -1       , -1       , 0        },  /* ABL_EG        */
    { -1       , -1       , 0        },  /* ABL_EX        */
    { -1       , -1       , 0        }   /* ABL_EU        */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Flat  Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Duplicate Abl Polar Expr                |
|                                                             |
\------------------------------------------------------------*/

ablexpr *polardupablexpr( Expr, Polar )

  ablexpr *Expr;
  int         Polar;
{
  ablexpr *ExprFlat;
  ablexpr *ExprQueue;
  long        Oper;
  long        NewOper;

  if ( ABL_ATOM( Expr ) )
  {
    ExprFlat = addchain( (ablexpr *)0, ABL_CAR( Expr ) );

    if ( Polar == ABL_POLAR_NEGATIVE )
    {
      ExprFlat = addchain( (ablexpr *)0, (void *)ExprFlat );
      ExprFlat = addchain( ExprFlat,
                           (void *)addchain( (ablexpr *)0,
                                             (void *)ABL_NOT ) );
    }

    return( ExprFlat );
  }

  Oper = ABL_OPER( Expr );

  if ( isablunaryoper( Oper ) )
  {
    if ( Oper == ABL_NOT )
    {
      ExprFlat = polardupablexpr( ABL_CADR( Expr ), ! Polar );
    }
    else
    {
      ExprFlat = polardupablexpr( ABL_CADR( Expr ), ABL_POLAR_POSITIVE );
      ExprFlat = addchain( (ablexpr *)0, (void *)ExprFlat );
      ExprFlat = addchain( ExprFlat,
                           (void *)addchain( (ablexpr *)0,
                                             (void *)Oper ) );
      if ( Polar == ABL_POLAR_NEGATIVE )
      {
        ExprFlat = addchain( (ablexpr *)0, (void *)ExprFlat );
        ExprFlat = addchain( ExprFlat,
                             (void *)addchain( (ablexpr *)0,
                                               (void *)ABL_NOT ) );
      }
    }
  }
  else
  {
    NewOper = AblPolarOperTable[ Oper ][ Polar ];

    if ( AblPolarOperTable[ Oper ][ 2 ] ==  1 ) Polar = ! Polar;
    if ( AblPolarOperTable[ Oper ][ 2 ] == -1 ) Polar = ABL_POLAR_POSITIVE;

    ExprFlat = addchain( (ablexpr *)0,
                         (void *)addchain( (ablexpr *)0,
                                           (void *)NewOper ) );
    ExprQueue = ExprFlat;

    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      ABL_CDR( ExprQueue ) = 

        addchain( (ablexpr *)0,
                  (void *)polardupablexpr( ABL_CAR( Expr ), Polar ) );

      ExprQueue = ABL_CDR( ExprQueue );
    }
  }

  return( ExprFlat );
}

/*------------------------------------------------------------\
|                                                             |
|                        Abl Polar Expr                       |
|                                                             |
\------------------------------------------------------------*/

ablexpr *polarablexpr( Expr, Polar )

  ablexpr *Expr;
  int         Polar;
{
  ablexpr *ScanExpr;
  long        Oper;
  long        NewOper;

  if ( ABL_ATOM( Expr ) )
  {
    if ( Polar != ABL_POLAR_POSITIVE )
    {
      Expr = addchain( (ablexpr *)0, (void *)Expr );
      Expr = addchain( Expr,
                       (void *)addchain( (ablexpr *)0,
                                         (void *)ABL_NOT ) );
    }

    return( Expr );
  }

  Oper = ABL_OPER( Expr );

  if ( isablunaryoper( Oper ) )
  {
    if ( Oper == ABL_NOT )
    {
      ScanExpr = Expr;
      Expr     = polarablexpr( ABL_CADR( Expr ), ! Polar );

      freechain( ABL_CAR( ScanExpr ) );
      freechain( ScanExpr );
    }
    else
    {
      ABL_CADR( Expr ) = polarablexpr( ABL_CADR( Expr ), ABL_POLAR_POSITIVE );

      if ( Polar == ABL_POLAR_NEGATIVE )
      {
        Expr = addchain( (ablexpr *)0, (void *)Expr );
        Expr = addchain( Expr,
                         (void *)addchain( (ablexpr *)0,
                                           (void *)ABL_NOT ) );
      }
    }
  }
  else
  {
    NewOper = AblPolarOperTable[ Oper ][ Polar ];

    if ( AblPolarOperTable[ Oper ][ 2 ] ==  1 ) Polar = ! Polar;
    if ( AblPolarOperTable[ Oper ][ 2 ] == -1 ) Polar = ABL_POLAR_POSITIVE;

    ABL_OPER( Expr ) = NewOper;
    ScanExpr         = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
    {
      ABL_CAR( ScanExpr ) = polarablexpr( ABL_CAR( ScanExpr ), Polar );
    }
  }

  return( Expr );
}
