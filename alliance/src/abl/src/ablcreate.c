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
| Tool    :                     Abl                           |
|                                                             |
| File    :                  ablcreate.c                      |
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
# include "ablcreate.h"
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
|                        Create Abl Atom                      |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablatom( Atom )

  char *Atom;
{
  return( addchain( (ablexpr *)0, (void *)namealloc( Atom ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Abl Atom Zero                 |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablatomzero()
{
  return( addchain( (ablexpr *)0, (void *)ABL_ATOM_NAME_ZERO ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Abl Atom One                  |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablatomone()
{
  return( addchain( (ablexpr *)0, (void *)ABL_ATOM_NAME_ONE ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Abl Atom Dc                   |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablatomdc()
{
  return( addchain( (ablexpr *)0, (void *)ABL_ATOM_NAME_DC ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Abl Atom Tristate             |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablatomtristate()
{
  return( addchain( (ablexpr *)0, (void *)ABL_ATOM_NAME_TRISTATE ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Create Abl Oper                        |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createabloper( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    ablerror( ABL_OPERATOR_ERROR, Oper );
  }

  return( addchain( (ablexpr *)0,
                    (void *)addchain( (ablexpr *)0,
                                      (void *)Oper ) ) );
}

/*------------------------------------------------------------\
|                                                             |
|                        Create Abl Bin Expr                  |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablbinexpr( Oper, Expr1, Expr2 )

  long     Oper;
  ablexpr *Expr1;
  ablexpr *Expr2;
{
  ablexpr *Expr;

  if ( ! isablbinaryoper( Oper ) )
  {
    ablerror( ABL_OPERATOR_ERROR, Oper );
  }

  Expr = createabloper( Oper );

  addablhexpr( Expr, Expr2 );
  addablhexpr( Expr, Expr1 );

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Create Abl Not Expr                    |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablnotexpr( Expr )

  ablexpr *Expr;
{
  ablexpr *Expr1;
  char    *AtomValue;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( AtomValue == ABL_ATOM_NAME_ONE )
    {
      ABL_CAR_L( Expr ) = (void *)ABL_ATOM_NAME_ZERO;

      return( Expr );
    }
    else
    if ( AtomValue == ABL_ATOM_NAME_ZERO )
    {
      ABL_CAR_L( Expr ) = (void *)ABL_ATOM_NAME_ONE;

      return( Expr );
    }
  }

  Expr1 = createabloper( ABL_NOT );
  addablhexpr( Expr1, Expr );

  return( Expr1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Create Abl Xor Expr                     |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablxorbinexpr( Oper, Expr1, Expr2 )

  long        Oper;
  ablexpr *Expr1;
  ablexpr *Expr2;
{
  ablexpr *Expr;
  ablexpr *NotExpr1;
  ablexpr *NotExpr2;

  if ( ( Oper != ABL_XOR  ) &&
       ( Oper != ABL_NXOR ) )
  {
    ablerror( ABL_OPERATOR_ERROR, Oper );
  }

  NotExpr1 = createablnotexpr( dupablexpr( Expr1 ) );
  NotExpr2 = createablnotexpr( dupablexpr( Expr2 ) );

  if ( Oper == ABL_XOR )
  {
    Expr = createablbinexpr( ABL_OR,
                             createablbinexpr( ABL_AND, NotExpr1, Expr2    ),
                             createablbinexpr( ABL_AND, Expr1   , NotExpr2 ) );
  }
  else
  {
    Expr = createablbinexpr( ABL_OR,
                             createablbinexpr( ABL_AND, NotExpr1, NotExpr2 ),
                             createablbinexpr( ABL_AND, Expr1   , Expr2    ) );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                      Create Abl Unary Expr                  |
|                                                             |
\------------------------------------------------------------*/

ablexpr *createablunaryexpr( Oper, Expr )

  long     Oper;
  ablexpr *Expr;
{
  ablexpr *Expr1;

  if ( ! isablunaryoper( Oper ) )
  {
    ablerror( ABL_NOT_UNARY_ERROR, Oper );
  }

  Expr1 = createabloper( Oper );
  addablhexpr( Expr1, Expr );

  return( Expr1 );
}
