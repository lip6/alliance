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
| File    :                  ablctlnorm.c                     |
|                                                             |
| Date    :                   03.12.96                        |
|                                                             |
| Author  :                     C G.                          |
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
# include "ablctlnorm.h"
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
|                      Normalisation Functions                |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *local_normablctl( Expr )

  ablexpr *Expr;

{
  long      Oper;
  ablexpr   *NormExpr;
  ablexpr   *ScanExpr;
  ablexpr   *ExprG1;
  ablexpr   *ExprG2;
  ablexpr   *ExprTemp;
  ablexpr   *ExprTemp2;

  if ( ABL_ATOM( Expr ) )
  {
    return( Expr );
  }

  Oper = ABL_OPER( Expr );

  if ( ( Oper == ABL_EX ||
         Oper == ABL_EG ) &&
       ( ABL_ATOM( ABL_CADR( Expr ) ) ) )
  {
    return( Expr );
  }

        /*----------------------------------------- */
        /*              AX(f) -> !EX(!f)            */
        /*----------------------------------------- */
  if ( Oper == ABL_AX )
  {
    NormExpr = local_normablctl( ABL_CADR( Expr ) );
    if ( NormExpr != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = NormExpr;
    ABL_CADR( Expr ) = createablnotexpr( ABL_CADR( Expr ) );
    SET_ABL_OPER( Expr, ABL_EX );
    NormExpr = createablnotexpr( Expr );
    return( NormExpr );
  }

        /*----------------------------------------- */
        /*              EF(f) -> EU(True f)         */
        /*----------------------------------------- */
  if ( Oper == ABL_EF )
  {
    NormExpr = local_normablctl( ABL_CADR( Expr ) );
    if ( NormExpr != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = NormExpr;
    ExprTemp = createablatom( ABL_ATOM_NAME_ONE );
    NormExpr = createablbinexpr( ABL_EU,
                         ExprTemp,
                         ABL_CADR( Expr )
                       );
    freechain( ABL_CAR( Expr ) );
    freechain( Expr );
    return( NormExpr );
  }

        /*----------------------------------------- */
        /*              AF(f) -> !EG(!f)            */
        /*----------------------------------------- */
  if ( Oper == ABL_AF )
  {
    NormExpr = local_normablctl( ABL_CADR( Expr ) );
    if ( NormExpr != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = NormExpr;
    ABL_CADR( Expr ) = createablnotexpr( ABL_CADR( Expr ) );
    SET_ABL_OPER( Expr, ABL_EG );
    NormExpr = createablnotexpr( Expr );
    return( NormExpr );
  }

        /*----------------------------------------- */
        /*              AG(f) -> !EF(!f)            */
        /*----------------------------------------- */
  if ( Oper == ABL_AG )
  {
    NormExpr = local_normablctl( ABL_CADR( Expr ) );
    if ( NormExpr != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = NormExpr;
    ABL_CADR( Expr ) = createablnotexpr( ABL_CADR( Expr ) );
    SET_ABL_OPER( Expr, ABL_EF );
    NormExpr = createablnotexpr( Expr );
    NormExpr = local_normablctl( NormExpr );
    return( NormExpr );
  }

       /*------------------------------------------- */
       /* AU(f g) -> AND(!EU(!g AND(!f !g)) !EG(!g)) */
       /*------------------------------------------- */
  if ( Oper == ABL_AU )
  {
    NormExpr = local_normablctl( ABL_CADR( Expr ) );
    if ( NormExpr != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = NormExpr;
    NormExpr = local_normablctl( ABL_CAR( ABL_CDDR( Expr ) ) );
    if ( NormExpr != ABL_CAR( ABL_CDDR( Expr ) ) )
      ABL_CAR( ABL_CDDR( Expr ) ) = NormExpr;

    ABL_CADR( Expr ) = createablnotexpr( ABL_CADR( Expr ) );
    ABL_CAR( ABL_CDDR( Expr ) ) = createablnotexpr( ABL_CAR( ABL_CDDR( Expr ) ) );
    ExprG1 = dupablexpr( ABL_CAR( ABL_CDDR( Expr ) ) );  
    ExprG2 = dupablexpr( ABL_CAR( ABL_CDDR( Expr ) ) );  
    ExprTemp = createablbinexpr( ABL_AND,
                 ABL_CADR( Expr ),
                 ABL_CAR( ABL_CDDR( Expr ) )
               ); 
    ExprTemp = createablbinexpr( ABL_EU,
                 ExprG1,
                 ExprTemp
               );
    ExprTemp = createablnotexpr( ExprTemp );
    ExprTemp2 = createablunaryexpr( ABL_EG,
                  ExprG2
                );
    ExprTemp2 = createablnotexpr( ExprTemp2 );
    NormExpr = createablbinexpr( ABL_AND,
                 ExprTemp,
                 ExprTemp2
               );
    freechain( ABL_CAR( Expr ) );
    freechain( Expr );
    return( NormExpr );
  }


  ScanExpr = Expr;
  NormExpr = Expr;

  while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
  {
    NormExpr = local_normablctl( ABL_CAR( ScanExpr ) );
    if ( NormExpr != ABL_CAR( ScanExpr ) )
      ABL_CAR( ScanExpr ) = NormExpr;
    NormExpr = Expr;
  }

  return( NormExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   Abl Normalisation Functions               |
|                                                             |
\------------------------------------------------------------*/

ablexpr *normablctlexpr( Expr )

  ablexpr *Expr;
{
  return( local_normablctl( Expr ) );
}
