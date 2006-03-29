/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU General Public |
| License as  published by the Free Software Foundation;      |
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
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  boom_auxinit.c                   |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   06.06.00                        |
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
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "boom_error.h"
# include "boom_debug.h"
# include "boom_shared.h"
# include "boom_abl.h"
# include "boom_cost.h"
# include "boom_aux.h"
# include "boom_auxinit.h"

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

  static befig_list    *BoomBehFigure = (befig_list    *)0;
  static boombefiginfo *BoomFigInfo   = (boombefiginfo *)0;
  static authtable     *BoomHashAux   = (authtable     *)0;
  static long           BoomAuxNumber = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   BoomGetNumberAtomicXorAbl                 |
|                                                             |
\------------------------------------------------------------*/

static long BoomGetNumberAtomicXorAbl( Expr ) 

  ablexpr *Expr;
{
  ablexpr *ScanExpr;
  long     Oper;
  long     Count;

  if ( ABL_ATOM( Expr ) )
  {
    return( 0 );
  }
  else
  {
    Oper  = ABL_OPER( Expr );
    Count = 0;

    if ( ( Oper == ABL_XOR  ) ||
         ( Oper == ABL_NXOR ) )
    {
      Count    = 1;
      ScanExpr = Expr;

      while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
      {
        if ( ! ABL_ATOM( ABL_CAR( ScanExpr ) ) )
        {
          Count = 0; break;
        }
      }
    }

    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      Count += BoomGetNumberAtomicXorAbl( ABL_CAR( Expr ) );
    }
  }

  return( Count );
}

/*------------------------------------------------------------\
|                                                             |
|                    BoomGetNewAuxInitName                    |
|                                                             |
\------------------------------------------------------------*/

static char *BoomGetNewAuxInitName()
{
  authelem *Element;
  char     *Name;
  char      Buffer[ 32 ];

  do
  {
    sprintf( Buffer, "boom_init_%ld", BoomAuxNumber++ );
    Name    = namealloc( Buffer ); 
    Element = searchauthelem( BoomHashAux, Name );
  }
  while ( Element != (authelem *)0 );

  return( Name );
}

/*------------------------------------------------------------\
|                                                             |
|                  BoomCreateAuxInitComparatorAbl             |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *BoomCreateAuxInitComparatorAbl( Expr ) 

  ablexpr *Expr;
{
  ablexpr *ScanExpr;
  long     Oper;
  ablexpr *Operand1;
  ablexpr *Operand2;
  ablexpr *AuxExpr;
  char    *AuxName;

  if ( ! ABL_ATOM( Expr ) )
  {
    Oper = ABL_OPER( Expr );

    if ( ( Oper == ABL_XOR  ) ||
         ( Oper == ABL_NXOR ) )
    {
      ScanExpr = Expr;

      while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
      {
        if ( ! ABL_ATOM( ABL_CAR( ScanExpr ) ) ) break;
      }

      if ( ScanExpr == (ablexpr *)0 )
      {
/*
** Create auxiliary variables for each xor couple
*/
        ScanExpr = Expr;
        Operand1 = (ablexpr *)0;

        while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
        {
          if ( Operand1 == (ablexpr *)0 )
          {
            Operand1 = ABL_CAR( ScanExpr );
            ABL_CAR_L( ScanExpr ) = (ablexpr *)0;

            continue;
          }

          Operand2 = ABL_CAR( ScanExpr );
          ABL_CAR_L( ScanExpr ) = (ablexpr *)0;

          AuxExpr = createablbinexpr( ABL_XOR, Operand1, Operand2 );
          AuxName = BoomGetNewAuxInitName();

          if ( IsBoomDebugLevel0() )
          {
            BoomPrintf( stdout, "Found a comparator, add aux init %s\n", AuxName );
          }
  
          BoomBehFigure->BEAUX = beh_addbeaux( BoomBehFigure->BEAUX, AuxName, AuxExpr, (void *)0 );
          BoomBehFigure->BERIN = beh_addberin( BoomBehFigure->BERIN, AuxName );
          BoomAddBehAuxInfo( BoomBehFigure->BEAUX );
          SetBoomBehAuxKeep( BoomBehFigure->BEAUX );

          Operand1 = createablatom( AuxName ); 
        }

        freeablexpr( Expr );

        Expr = Operand1;

        if ( Oper == ABL_NXOR ) Expr = optimablnotexpr( Expr );

        return( Expr );
      }
    }

    ScanExpr = Expr;

    while ( ( ScanExpr = ABL_CDR( ScanExpr ) ) != (ablexpr *)0 )
    {
      ABL_CAR_L( ScanExpr ) = BoomCreateAuxInitComparatorAbl( ABL_CAR( ScanExpr ) );
    }
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                   BoomIdentifiyComparatorAbl                |
|                                                             |
\------------------------------------------------------------*/

static ablexpr *BoomIdentifiyComparatorAbl( Expr ) 

  ablexpr *Expr;
{
  long Number;

  Number = BoomGetNumberAtomicXorAbl( Expr );

  if ( Number > BOOM_AUX_INIT_MAX_XOR )
  {
    Expr = BoomCreateAuxInitComparatorAbl( Expr );
  }

  return( Expr );
}

/*------------------------------------------------------------\
|                                                             |
|                   BoomKeepStableAuxAbl                      |
|                                                             |
\------------------------------------------------------------*/

static void BoomKeepStableAuxAbl( Expr ) 

  ablexpr *Expr;
{
  authelem   *Element;
  char       *Value;
  beaux_list *BehAux;
  long        Oper;

  if ( ABL_ATOM( Expr ) ) return;

  Oper = ABL_OPER( Expr );

  if ( Oper == ABL_STABLE )
  {
    Expr = ABL_CADR( Expr );

    if ( ABL_ATOM( Expr ) )
    {
      Value   = ABL_ATOM_VALUE( Expr );
      Element = searchauthelem( BoomHashAux, Value );

      if ( Element != (authelem *)0 )
      {
        BehAux = (beaux_list *)Element->VALUE;
        SetBoomBehAuxKeep( BehAux );
      }
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
    {
      BoomKeepStableAuxAbl( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                   BoomBehCreateAuxInit                      |
|                                                             |
\------------------------------------------------------------*/

void BoomBehCreateAuxInit( BehFigure )

  befig_list *BehFigure;
{
  beaux_list    *BehAux;

  BoomBehFigure = BehFigure;
  BoomFigInfo   = GetBoomBehFigInfo( BehFigure );

  BoomHashAux = createauthtable( 100 );

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    addauthelem( BoomHashAux, BehAux->NAME, (long)BehAux );

    if ( BoomFigInfo->KEEP_AUX )
    {
      SetBoomBehAuxKeep( BehAux );
    }
  }

  BoomBehScanAbl( BehFigure, BoomKeepStableAuxAbl, 1 );

  BoomBehDeleteUselessAux( BehFigure, (beaux_list *)0, 1 );

  BoomBehMapAbl( BehFigure, simpablexpr, 1 );

  BoomBehMapAbl( BehFigure, BoomIdentifiyComparatorAbl, 0 );

  destroyauthtable( BoomHashAux );
}
