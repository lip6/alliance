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
| Tool    :                     Abl                           |
|                                                             |
| File    :                  ablctlsimp.c                     |
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
# include "ablctlsimp.h"
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
|                      Abl Simplification                     |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *loc_simpablctl( Expr )

  ablexpr *Expr;
{
  long      Oper;
  ablexpr   *SimpExpr;
  ablexpr   *ScanExpr;
  ablexpr   *ExprTemp;

  if ( ABL_ATOM( Expr ) )
  {
    return( Expr );
  }

  Oper = ABL_OPER( Expr );

  if ( ( Oper == ABL_AF ||
         Oper == ABL_AG ||
         Oper == ABL_AX ||
         Oper == ABL_EF ||
         Oper == ABL_EG ||
         Oper == ABL_EX ) &&
       ( ABL_ATOM( ABL_CADR( Expr ) ) ) )
  {
    return( Expr );
  }


    /*----------------------------------------------*/
    /*          AG(AG(f)) -> AG(f)      */
    /*          EF(EF(f)) -> EF(f)      */
    /*          EG(EG(f)) -> EG(f)      */
    /*          AF(AF(f)) -> AF(f)      */
    /*      AG(EF(AG(EF(f)))) -> EF(AG(EF(f)))  */
    /*      EF(AG(EF(AG(f)))) -> AG(EF(AG(f)))  */
    /*          AF(EG(AF(f))) -> EG(AF(f))      */
    /*          EG(AF(EG(f))) -> AF(EG(f))      */
    /*          AF(AG(AF(f))) -> AG(AF(f))      */
    /*          AG(AF(AG(f))) -> AF(AG(f))      */
    /*----------------------------------------------*/
  if ( ( ( Oper == ABL_AG ||
           Oper == ABL_EF ||
           Oper == ABL_EG ||
           Oper == ABL_AF ) &&
         ( ABL_OPER( ABL_CADR( Expr ) ) == Oper ) ) ||
       ( Oper == ABL_AG  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_EF &&
         ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_AG &&
         ABL_OPER( ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) ) == ABL_EF ) ||
       ( Oper == ABL_EF  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_AG &&
         ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_EF &&
         ABL_OPER( ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) ) == ABL_AG ) ||
       ( Oper == ABL_AF  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_EG &&
         ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_AF ) ||
       ( Oper == ABL_EG  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_AF &&
         ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_EG ) ||
       ( Oper == ABL_AF  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_AG &&
         ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_AF ) ||
       ( Oper == ABL_AG  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_AF &&
         ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_AG ) )
  {
    SimpExpr = ABL_CADR( Expr );
    freechain( ABL_CAR( Expr ) );
    freechain( Expr );

    ExprTemp = loc_simpablctl( SimpExpr );
    if ( ExprTemp != SimpExpr )
      SimpExpr = ExprTemp;

    return( SimpExpr );
  }


    /*----------------------------------------------*/
    /*          AG(EG(f)) -> AG(f)      */
    /*          EF(AF(f)) -> EF(f)      */
    /*----------------------------------------------*/
  if ( ( Oper == ABL_AG  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_EG ) ||
       ( Oper == ABL_EF  &&
         ABL_OPER( ABL_CADR( Expr ) ) == ABL_AF ) )
  {
    ExprTemp = ABL_CADR( ABL_CADR( Expr ) );
    freechain( ABL_CAR( ABL_CADR( Expr ) ) );
    freechain( ABL_CADR( Expr ) );
    ABL_CADR( Expr ) = ExprTemp;
    SimpExpr = Expr;

    ExprTemp = loc_simpablctl( SimpExpr );
    if ( ExprTemp != SimpExpr )
      SimpExpr = ExprTemp;

    return( SimpExpr );
  }



    /*----------------------------------------------*/
    /*           AU(f AU(f g)) -> AU(f g)       */
    /*----------------------------------------------*/
  if ( Oper == ABL_AU )
  {
    ExprTemp = loc_simpablctl( ABL_CADR( Expr ) );
    if ( ExprTemp != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = ExprTemp;
    ExprTemp = loc_simpablctl( ABL_CAR( ABL_CDDR( Expr ) ) );
    if ( ExprTemp != ABL_CAR( ABL_CDDR( Expr ) ) )
      ABL_CAR( ABL_CDDR( Expr ) ) = ExprTemp;

    if ( ABL_OPER( ABL_CAR( ABL_CDDR( Expr ) ) ) == Oper && 
     ABL_CADR( Expr ) != (ablexpr *)0 &&
     ABL_CADR( ABL_CAR( ABL_CDDR ( Expr ) ) ) != (ablexpr *)0 &&
         isablequalexpr( ABL_CADR( Expr ),
                         ABL_CADR( ABL_CAR( ABL_CDDR ( Expr ) ) ) ) )
    {
      SimpExpr = ABL_CAR( ABL_CDDR( Expr ) );
      delablexpr( ABL_CADR( Expr ) );
      freechain( ABL_CAR( Expr ) );
      freechain( Expr );

      return( SimpExpr );
    }
  }


    /*----------------------------------------------*/
    /*  AND(AG(f) AG(g)) -> AG(AND(f g))    */
    /*  OR(EF(f) AG(g)) -> EF(OR(f g))      */
    /*----------------------------------------------*/
  if ( Oper == ABL_AND ||
       Oper == ABL_OR )
  {
    ExprTemp = loc_simpablctl( ABL_CADR( Expr ) );
    if ( ExprTemp != ABL_CADR( Expr ) )
      ABL_CADR( Expr ) = ExprTemp;
    ExprTemp = loc_simpablctl( ABL_CAR( ABL_CDDR( Expr ) ) );
    if ( ExprTemp != ABL_CAR( ABL_CDDR( Expr ) ) )
      ABL_CAR( ABL_CDDR( Expr ) ) = ExprTemp;

    if ( ( Oper == ABL_AND  &&
           ABL_OPER( ABL_CADR( Expr ) ) == ABL_AG &&
           ABL_OPER( ABL_CAR( ABL_CDDR( Expr ) ) ) == ABL_AG ) ||
         ( Oper == ABL_OR &&
           ABL_OPER( ABL_CADR( Expr ) ) == ABL_EF &&
           ABL_OPER( ABL_CAR( ABL_CDDR( Expr ) ) ) == ABL_EF ) )
    {
      SimpExpr = createablbinexpr( Oper,
                   ABL_CADR( ABL_CADR( Expr ) ),
                   ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) )
                 );
      SimpExpr = createablunaryexpr( ABL_OPER( ABL_CADR( Expr ) ), SimpExpr );
      freechain( ABL_CAR( ABL_CAR( ABL_CDDR( Expr ) ) ) );
      freechain( ABL_CAR( ABL_CDDR( Expr ) ) );
      freechain( ABL_CAR( ABL_CADR( Expr ) ) );
      freechain( ABL_CADR( Expr ) );
      freechain( ABL_CAR( Expr ) );
      freechain( Expr );

      return( SimpExpr );
    }
  }


    /*----------------------------------------------*/
    /*      EG(OR(OR(EG(f) g) OR(f EG(g))))         */
    /*              -> OR(EG(f) EG(g))          */
    /*----------------------------------------------*/
   if ( Oper == ABL_EG &&
        ABL_OPER( ABL_CADR( Expr ) ) == ABL_OR &&
        ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_OR &&
        ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) == ABL_OR && 
        ABL_OPER( ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) ) == ABL_EG &&
        ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) ) == ABL_EG &&
    ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) != (ablexpr *)0 &&
    ABL_CADR( ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) ) != (ablexpr *)0 &&
        isablequalexpr( ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ),
                        ABL_CADR( ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) ) ) &&
    ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR ( ABL_CADR( Expr ) ) ) ) ) ) != (ablexpr *)0 &&
    ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) != (ablexpr *)0 &&
        isablequalexpr( ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) ),
                        ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) ) )
   {
      SimpExpr = createablbinexpr( ABL_OR,
                   ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ),
                   ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) );
      ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) = (ablexpr *)0;
      ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) = (ablexpr *)0;
      freeablexpr( Expr );

      ExprTemp = loc_simpablctl( ABL_CADR( SimpExpr ) );
      if ( ExprTemp != ABL_CADR( SimpExpr ) )
        ABL_CADR( SimpExpr ) = ExprTemp;
      ExprTemp = loc_simpablctl( ABL_CAR( ABL_CDDR( SimpExpr ) ) );
      if ( ExprTemp != ABL_CAR( ABL_CDDR( SimpExpr ) ) )
        ABL_CAR( ABL_CDDR( SimpExpr ) ) = ExprTemp;
      
      return( SimpExpr );
   } 


    /*----------------------------------------------*/
    /*      AF(OR(AND(f AF(g)) AND(AF(f) g)))       */
    /*             -> AND(AF(f) AF(g))          */
    /*----------------------------------------------*/
   if ( Oper == ABL_AF &&
        ABL_OPER( ABL_CADR( Expr ) ) == ABL_OR &&
        ABL_OPER( ABL_CADR( ABL_CADR( Expr ) ) ) == ABL_AND &&
        ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) == ABL_AND && 
        ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) ) == ABL_AF &&
        ABL_OPER( ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) == ABL_AF &&
    ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ) != (ablexpr *)0 &&
    ABL_CADR( ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) != (ablexpr *)0 &&
        isablequalexpr( ABL_CADR( ABL_CADR( ABL_CADR( Expr ) ) ),
                        ABL_CADR( ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) ) ) &&
    ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) ) != (ablexpr *)0 &&
    ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR ( ABL_CADR( Expr ) ) ) ) ) != (ablexpr *)0 &&
        isablequalexpr( ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) ),
                        ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR ( ABL_CADR( Expr ) ) ) ) ) ) )
   {
      SimpExpr = createablbinexpr( ABL_AND,
                   ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ),
                   ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) );
      ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) ) = (ablexpr *)0;
      ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CADR( Expr ) ) ) ) = (ablexpr *)0;
      freeablexpr( Expr );

      ExprTemp = loc_simpablctl( ABL_CADR( SimpExpr ) );
      if ( ExprTemp != ABL_CADR( SimpExpr ) )
        ABL_CADR( SimpExpr ) = ExprTemp;
      ExprTemp = loc_simpablctl( ABL_CAR( ABL_CDDR( SimpExpr ) ) );
      if ( ExprTemp != ABL_CAR( ABL_CDDR( SimpExpr ) ) )
        ABL_CAR( ABL_CDDR( SimpExpr ) ) = ExprTemp;
      
      return( SimpExpr );
   } 


    /*----------------------------------------------------*/
    /* AU(AND(f f') OR(AND(g AU(f' g')) AND(g' AU(f g)))) */
    /*            -> AND(AU(f g) EU(f' g'))               */
    /*----------------------------------------------------*/
  if ( Oper == ABL_AU &&
       ABL_OPER( ABL_CADR( Expr ) ) == ABL_AND &&
       ABL_OPER( ABL_CAR( ABL_CDDR( Expr ) ) ) == ABL_OR &&
       ABL_OPER( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) == ABL_AND &&
       ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) == ABL_AND &&
       ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) == ABL_AU &&
       ABL_OPER( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) == ABL_AU &&
       ABL_CADR( ABL_CADR( Expr ) ) != (ablexpr *)0 &&
       ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) != (ablexpr *)0 &&
       isablequalexpr( ABL_CADR( ABL_CADR( Expr ) ),
               ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) ) &&
       ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) != (ablexpr *)0 &&
       ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) != (ablexpr *)0 &&
       isablequalexpr( ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ),
               ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) &&
       ABL_CADR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) != (ablexpr *)0 &&
       ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) ) != (ablexpr *)0 &&
       isablequalexpr( ABL_CADR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ),
               ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) ) ) &&
       ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ) != (ablexpr *)0 &&
       ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) != (ablexpr *)0 &&
       isablequalexpr( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) ) ),
               ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) )
     )
  {
    ExprTemp = createablbinexpr( ABL_AU,
                 ABL_CADR( ABL_CADR( Expr ) ),
                 ABL_CADR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) );
    ABL_CADR( ABL_CADR( Expr ) ) = (ablexpr *)0;
    ABL_CADR( ABL_CADR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) = (ablexpr *)0;
    SimpExpr = createablbinexpr( ABL_EU,
                 ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ),
                 ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) );
    ABL_CAR( ABL_CDDR( ABL_CADR( Expr ) ) ) = (ablexpr *)0;
    ABL_CADR( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( Expr ) ) ) ) ) = (ablexpr *)0;
    SimpExpr = createablbinexpr( ABL_AND,
                 ExprTemp,
                 SimpExpr );
    freeablexpr( Expr );

    ExprTemp = loc_simpablctl( ABL_CADR( ABL_CADR( SimpExpr ) ) );
    if ( ExprTemp != ABL_CADR( ABL_CADR( SimpExpr ) ) )
      ABL_CADR( ABL_CADR( SimpExpr ) ) = ExprTemp;
    ExprTemp = loc_simpablctl( ABL_CAR( ABL_CDDR( ABL_CADR( SimpExpr ) ) ) );
    if ( ExprTemp != ABL_CAR( ABL_CDDR( ABL_CADR( SimpExpr ) ) ) )
      ABL_CAR( ABL_CDDR( ABL_CADR( SimpExpr ) ) ) = ExprTemp;
    ExprTemp = loc_simpablctl( ABL_CADR( ABL_CAR( ABL_CDDR( SimpExpr ) ) ) );
    if ( ExprTemp != ABL_CADR( ABL_CAR( ABL_CDDR( SimpExpr ) ) ) )
      ABL_CADR( ABL_CAR( ABL_CDDR( SimpExpr ) ) ) = ExprTemp;
    ExprTemp = loc_simpablctl( ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( SimpExpr ) ) ) ) );
    if ( ExprTemp != ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( SimpExpr ) ) ) ) )
      ABL_CAR( ABL_CDDR( ABL_CAR( ABL_CDDR( SimpExpr ) ) ) ) = ExprTemp;

    return( SimpExpr );
  }


  ScanExpr = Expr;
  SimpExpr = Expr;

  while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
  {
    SimpExpr = loc_simpablctl( ABL_CAR( ScanExpr ) );
    if ( SimpExpr != ABL_CAR( ScanExpr ) )
      ABL_CAR( ScanExpr ) = SimpExpr;
    SimpExpr = Expr;
  }

  return( SimpExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                   Abl Simplification Functions              |
|                                                             |
\------------------------------------------------------------*/

ablexpr *simpablctlexpr( Expr )

  ablexpr *Expr;
{
  return( loc_simpablctl( Expr ) );
}
