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
| File    :                   ablis.c                         |
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
# include "ablis.h"
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

  int AblOperBinary[ ABL_MAX_OPERATOR ] =
  {
    1, /* ABL_OR         */
    1, /* ABL_AND        */
    1, /* ABL_XOR        */
    0, /* ABL_NOT        */
    1, /* ABL_NOR        */
    1, /* ABL_NAND       */
    1, /* ABL_NXOR       */
    0, /* ABL_STABLE     */
    0, /* ABL_AF         */
    0, /* ABL_AG         */
    0, /* ABL_AX         */
    2, /* ABL_AU         */
    0, /* ABL_EF         */
    0, /* ABL_EG         */
    0, /* ABL_EX         */
    2, /* ABL_EU         */
  };

  static char *AblIsName = (char *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Is  Functions                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Unary Operator                   |
|                                                             |
\------------------------------------------------------------*/

int isablunaryoper( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( ! AblOperBinary[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Binary Operator                  |
|                                                             |
\------------------------------------------------------------*/

int isablbinaryoper( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( AblOperBinary[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Operator In Expr                 |
|                                                             |
\------------------------------------------------------------*/

int isabloperinexpr( Expr, Oper )

  ablexpr *Expr;
  long        Oper;
{
  if ( ABL_ATOM( Expr ) )
  {
    return( 0 );
  }

  if ( ABL_OPER( Expr ) == Oper )
  {
    return( 1 );
  }

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    if ( isabloperinexpr( ABL_CAR( Expr ), Oper ) ) return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Local Name In Expr                   |
|                                                             |
\------------------------------------------------------------*/

static int loc_isablnamein( Expr )

  ablexpr *Expr;
{
  if ( ( ABL_ATOM( Expr )                    ) &&
       ( ABL_ATOM_VALUE( Expr ) == AblIsName ) )
  {
    return( 1 );
  }

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    if ( loc_isablnamein( ABL_CAR( Expr ) ) ) return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Name In Expr                     |
|                                                             |
\------------------------------------------------------------*/

int isablnameinexpr( Expr, Name )

  ablexpr *Expr;
  char       *Name;
{
  AblIsName = namealloc( Name );

  return( loc_isablnamein( Expr ) );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Equal Expr                       |
|                                                             |
\------------------------------------------------------------*/

int isablequalexpr( Expr1, Expr2 )

  ablexpr *Expr1;
  ablexpr *Expr2;
{
  if ( ABL_ATOM( Expr1 ) )
  {
    if ( ( ABL_ATOM( Expr2 )                                  ) &&
         ( ABL_ATOM_VALUE( Expr1 ) == ABL_ATOM_VALUE( Expr2 ) ) )
    {
      return( 1 );
    }

    return( 0 );
  }

  if ( ABL_OPER( Expr1 ) != ABL_OPER( Expr2 ) )
  {
    return( 0 );
  }

  Expr1 = ABL_CDR( Expr1 );
  Expr2 = ABL_CDR( Expr2 );

  while ( ( Expr1 != (ablexpr *)0 ) &&
          ( Expr2 != (ablexpr *)0 ) )
  {
    if ( ! isablequalexpr( ABL_CAR( Expr1 ), ABL_CAR( Expr2 ) ) )
    {
      return( 0 );
    }

    Expr1 = ABL_CDR( Expr1 );
    Expr2 = ABL_CDR( Expr2 );
  }

  if ( ( Expr1 != (ablexpr *)0 ) ||
       ( Expr2 != (ablexpr *)0 ) )
  {
    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Similar Expr                     |
|                                                             |
\------------------------------------------------------------*/

int isablsimilarexpr( Expr1, Expr2 )

  ablexpr *Expr1;
  ablexpr *Expr2;
{
  if ( ABL_ATOM( Expr1 ) )
  {
    if ( ABL_ATOM( Expr2 ) )
    {
      return( 1 );
    }

    return( 0 );
  }

  if ( ABL_OPER( Expr1 ) != ABL_OPER( Expr2 ) )
  {
    return( 0 );
  }

  Expr1 = ABL_CDR( Expr1 );
  Expr2 = ABL_CDR( Expr2 );

  while ( ( Expr1 != (ablexpr *)0 ) &&
          ( Expr2 != (ablexpr *)0 ) )
  {
    if ( ! isablequalexpr( ABL_CAR( Expr1 ), ABL_CAR( Expr2 ) ) )
    {
      return( 0 );
    }

    Expr1 = ABL_CDR( Expr1 );
    Expr2 = ABL_CDR( Expr2 );
  }

  if ( ( Expr1 != (ablexpr *)0 ) ||
       ( Expr2 != (ablexpr *)0 ) )
  {
    return( 0 );
  }

  return( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Is Abl Not Expr                         |
|                                                             |
\------------------------------------------------------------*/

int isablnotexpr( Expr1, Expr2 )

  ablexpr *Expr1;
  ablexpr *Expr2;
{
  long Oper;
  long NotOper;

  if ( ABL_ATOM( Expr1 ) )
  {
    if ( ( ! ABL_ATOM( Expr2 )                                            ) &&
         ( ABL_OPER( Expr2 ) == ABL_NOT                                   ) &&
         ( ABL_ATOM_VALUE( Expr1 ) == ABL_ATOM_VALUE( ABL_CADR( Expr2 ) ) ) )
    {
      return( 1 );
    }

    return( 0 );
  }

  if ( ABL_ATOM( Expr2 ) )
  {
    if ( ( ABL_OPER( Expr1 ) == ABL_NOT                                   ) &&
         ( ABL_ATOM_VALUE( ABL_CADR( Expr1 ) ) == ABL_ATOM_VALUE( Expr2 ) ) )
    {
      return( 1 );
    }

    return( 0 );
  }

  if ( ABL_OPER( Expr1 ) == ABL_NOT )
  {
    if ( isablequalexpr( ABL_CADR( Expr1 ), Expr2 ) ) return( 1 );
  }

  if ( ABL_OPER( Expr2 ) == ABL_NOT )
  {
    if ( isablequalexpr( Expr1, ABL_CADR( Expr2 ) ) ) return ( 1 );
  }

  Oper  = ABL_OPER( Expr1 );
  NotOper = getablopernot( Oper );

  if ( ABL_OPER( Expr2 ) == NotOper )
  {
    if ( ( isablequalexpr(ABL_CADR( Expr1 ),
                          ABL_CADR( Expr2 )  )           ) &&
         ( isablequalexpr(ABL_CAR( ABL_CDDR( Expr1 ) ),
                          ABL_CAR( ABL_CDDR( Expr2 ) ) ) ) )
        return( 1 );
  }

  return( 0 );
}

