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
| File    :                   ablget.c                        |
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
# include "ablget.h"
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

  static chain_list *AblGetHeadChain = (chain_list *)0;
  static ptype_list *AblGetHeadPType = (ptype_list *)0;

  static char       *AblGetName = (char *)0;

  static int AblOperPolarity[ ABL_MAX_OPERATOR ] =
  {
    ABL_POLAR_POSITIVE, /* ABL_OR         */
    ABL_POLAR_POSITIVE, /* ABL_AND        */
    ABL_POLAR_POSITIVE, /* ABL_XOR        */
    ABL_POLAR_NEGATIVE, /* ABL_NOT        */
    ABL_POLAR_NEGATIVE, /* ABL_NOR        */
    ABL_POLAR_NEGATIVE, /* ABL_NAND       */
    ABL_POLAR_NEGATIVE, /* ABL_NXOR       */
    ABL_POLAR_POSITIVE, /* ABL_STABLE     */
    ABL_POLAR_POSITIVE, /* ABL_AF         */
    ABL_POLAR_POSITIVE, /* ABL_AG         */
    ABL_POLAR_POSITIVE, /* ABL_AX         */
    ABL_POLAR_POSITIVE, /* ABL_AU         */
    ABL_POLAR_POSITIVE, /* ABL_EF         */
    ABL_POLAR_POSITIVE, /* ABL_EG         */
    ABL_POLAR_POSITIVE, /* ABL_EX         */
    ABL_POLAR_POSITIVE  /* ABL_EU         */
  };

  static long AblOperNotTable[ ABL_MAX_OPERATOR ] =
  {
    ABL_NOR,  /* ABL_OR        */
    ABL_NAND, /* ABL_AND       */
    ABL_NXOR, /* ABL_XOR       */
    -1,       /* ABL_NOT       */
    ABL_OR,   /* ABL_NOR       */
    ABL_AND,  /* ABL_NAND      */
    ABL_XOR,  /* ABL_NXOR      */
    -1,       /* ABL_STABLE    */
    -1,       /* ABL_AF        */
    -1,       /* ABL_AG        */
    -1,       /* ABL_AX        */
    -1,       /* ABL_AU        */
    -1,       /* ABL_EF        */
    -1,       /* ABL_EG        */
    -1,       /* ABL_EX        */
    -1        /* ABL_EU        */
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Get Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     Get Abl Operator Name                   |
|                                                             |
\------------------------------------------------------------*/

char *getablopername( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    return( (char *)0 );
  }

  return( ABL_OPERATOR_NAME[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Abl Upper Operator Name             |
|                                                             |
\------------------------------------------------------------*/

char *getabloperuppername( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    return( (char *)0 );
  }

  return( ABL_OPERATOR_UPPER_NAME[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Abl Operator Polarity               |
|                                                             |
\------------------------------------------------------------*/

int getabloperpolar( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    return( -1 );
  }

  return( AblOperPolarity[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Abl Not Operator                    |
|                                                             |
\------------------------------------------------------------*/

int getablopernot( Oper )

  long Oper;
{
  if ( ( Oper < 0                 ) ||
       ( Oper >= ABL_MAX_OPERATOR ) )
  {
    return( 0 );
  }

  return( AblOperNotTable[ Oper ] );
}

/*------------------------------------------------------------\
|                                                             |
|                Get Abl Operator By Name                     |
|                                                             |
\------------------------------------------------------------*/

long getabloperbyname( Name )

  char *Name;
{
  int Oper;

  Name = namealloc( Name );

  for ( Oper = 0; Oper < ABL_MAX_OPERATOR; Oper++ )
  {
    if ( ABL_OPERATOR_NAME[ Oper ] == Name )
    {
      return( Oper );
    }
  }

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      Get Abl Expr Depth                     |
|                                                             |
\------------------------------------------------------------*/

long getablexprdepth( Expr )

  ablexpr *Expr;
{
  long  NumberOper;
  long  MaxDepth;
  long  CarDepth;

  if ( ABL_ATOM( Expr ) )
  {
    return( 0 );
  }

  MaxDepth   = 0;
  NumberOper = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (chain_list *)0 )
  {
    CarDepth = getablexprdepth( ABL_CAR( Expr ) );

    if ( CarDepth > MaxDepth ) MaxDepth = CarDepth;

    NumberOper++;
  }

  if ( NumberOper > 1 ) NumberOper--;

  return( MaxDepth + NumberOper );
}

/*------------------------------------------------------------\
|                                                             |
|                      Get Abl Expr Length                    |
|                                                             |
\------------------------------------------------------------*/

long getablexprlength( Expr )

  ablexpr *Expr;
{
  long Length;

  if ( ABL_ATOM( Expr ) )
  {
    return( 1 );
  }

  Length = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Length += 1;
  }

  return( Length );
}

/*------------------------------------------------------------\
|                                                             |
|                       Get Abl Expr Number Atom              |
|                                                             |
\------------------------------------------------------------*/

long getablexprnumatom( Expr )

  ablexpr *Expr;
{
  long Number;

  if ( ABL_ATOM( Expr ) )
  {
    return( 1 );
  }

  Number = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Number += getablexprnumatom( ABL_CAR( Expr ) );
  }

  return( Number );
}

/*------------------------------------------------------------\
|                                                             |
|                       Get Abl Expr Number Oper              |
|                                                             |
\------------------------------------------------------------*/

long getablexprnumbinoper( Expr )

  ablexpr *Expr;
{
  long Number;

  if ( ABL_ATOM( Expr ) )
  {
    return( 0 );
  }

  if ( isablunaryoper( ABL_OPER( Expr ) ) )
  {
    return( getablexprnumbinoper( ABL_CADR( Expr ) ) );
  }

  Number = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Number += 1 + getablexprnumbinoper( ABL_CAR( Expr ) );
  }

  return( Number -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Abl Expr Atom Chain                 |
|                                                             |
\------------------------------------------------------------*/

static void loc_getablsupportchain( Expr )

  ablexpr *Expr;
{
  ablexpr  *ScanName;
  ablexpr **PrevName;
  char     *AtomValue;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( ( AtomValue != ABL_ATOM_NAME_ONE      ) &&
         ( AtomValue != ABL_ATOM_NAME_ZERO     ) &&
         ( AtomValue != ABL_ATOM_NAME_DC       ) &&
         ( AtomValue != ABL_ATOM_NAME_TRISTATE ) )
    {
      PrevName = &AblGetHeadChain;

      for ( ScanName  = AblGetHeadChain;
            ScanName != (chain_list *)0;
            ScanName  = ScanName->NEXT )
      {
        if ( ScanName->DATA == (void *)AtomValue ) return;

        PrevName = &ScanName->NEXT;
      }

      *PrevName = addchain( (chain_list *)0, (void *)AtomValue );
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      loc_getablsupportchain( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     Get Abl Expr Atom PType                 |
|                                                             |
\------------------------------------------------------------*/

static void loc_getablsupportptype( Expr )

  ablexpr *Expr;
{
  ptype_list  *ScanName;
  ptype_list **PrevName;
  char        *AtomValue;

  if ( ABL_ATOM( Expr ) )
  {
    AtomValue = ABL_ATOM_VALUE( Expr );

    if ( ( AtomValue != ABL_ATOM_NAME_ONE      ) &&
         ( AtomValue != ABL_ATOM_NAME_ZERO     ) &&
         ( AtomValue != ABL_ATOM_NAME_DC       ) &&
         ( AtomValue != ABL_ATOM_NAME_TRISTATE ) )
    {
      PrevName = &AblGetHeadPType;

      for ( ScanName  = AblGetHeadPType;
            ScanName != (ptype_list *)0;
            ScanName  = ScanName->NEXT )
      {
        if ( ScanName->DATA == (void *)AtomValue ) return;

        PrevName = &ScanName->NEXT;
      }

      *PrevName = addptype( (ptype_list *)0, 0, (void *)AtomValue );
    }
  }
  else
  {
    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      loc_getablsupportptype( ABL_CAR( Expr ) );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Get Abl Expr Support                 |
|                                                             |
\------------------------------------------------------------*/

void *getablexprsupport( Expr, Mode )

  ablexpr *Expr;
  int      Mode;
{
  if ( Mode == ABL_SUPPORT_CHAIN )
  {
    AblGetHeadChain = (chain_list *)0;
    loc_getablsupportchain( Expr );
  
    return( (void *)AblGetHeadChain );
  }

  AblGetHeadPType = (ptype_list *)0;
  loc_getablsupportptype( Expr );

  return( (void *)AblGetHeadPType );
}

/*------------------------------------------------------------\
|                                                             |
|                        Get Abl Expr Support                 |
|                                                             |
\------------------------------------------------------------*/

void *unionablexprsupport( Support, Expr, Mode )

  void    *Support;
  ablexpr *Expr;
  int      Mode;
{
  if ( Mode == ABL_SUPPORT_CHAIN )
  {
    AblGetHeadChain = (chain_list *)Support;
    loc_getablsupportchain( Expr );
  
    return( (void *)AblGetHeadChain );
  }

  AblGetHeadPType = (ptype_list *)Support;
  loc_getablsupportptype( Expr );

  return( (void *)AblGetHeadPType );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Local Expr Number Occur               |
|                                                             |
\------------------------------------------------------------*/

static long loc_getablnumocc( Expr )

  ablexpr *Expr;
{
  long Number;

  if ( ( ABL_ATOM( Expr )                     ) &&
       ( ABL_ATOM_VALUE( Expr ) == AblGetName ) )
  {
    return( 1 );
  }

  Number = 0;

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Number += loc_getablnumocc( ABL_CAR( Expr ) );
  }

  return( Number );
}

/*------------------------------------------------------------\
|                                                             |
|                       Get Abl Expr Number Occur             |
|                                                             |
\------------------------------------------------------------*/

long getablexprnumocc( Expr, Name )

  ablexpr *Expr;
  char    *Name;
{
  AblGetName = namealloc( Name );

  return( loc_getablnumocc( Expr ) );
}

/*------------------------------------------------------------\
|                                                             |
|                    Get Abl Max Expression                   |
|                                                             |
\------------------------------------------------------------*/

ablexpr *getablexprmax( Function, Expr )

  int       (*Function)();
  ablexpr    *Expr;
{
  ablexpr    *MaxExpr;
  long        Maximum;
  long        Value;

  if ( ABL_ATOM( Expr ) )
  {
    return( (ablexpr *)0 );
  }

  Expr    = ABL_CDR( Expr );
  MaxExpr = ABL_CAR( Expr );
  Maximum = (*Function)( MaxExpr );

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Value = (*Function)( ABL_CAR( Expr ) );

    if ( Value > Maximum )
    {
      Maximum = Value;
      MaxExpr = ABL_CAR( Expr );
    }
  }

  return( MaxExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                       Get Abl Min Expression                |
|                                                             |
\------------------------------------------------------------*/

ablexpr *getablexprmin( Function, Expr )

  int       (*Function)();
  ablexpr   *Expr;
{
  ablexpr *MinExpr;
  long     Minimum;
  long     Value;

  if ( ABL_ATOM( Expr ) )
  {
    return( (ablexpr *)0 );
  }

  Expr    = ABL_CDR( Expr );
  MinExpr = ABL_CAR( Expr );
  Minimum = (*Function)( MinExpr );

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    Value = (*Function)( ABL_CAR( Expr ) );

    if ( Value < Minimum )
    {
      Minimum = Value;
      MinExpr = ABL_CAR( Expr );
    }
  }

  return( MinExpr );
}

/*------------------------------------------------------------\
|                                                             |
|                        Get Abl Expression Number            |
|                                                             |
\------------------------------------------------------------*/

ablexpr *getablexprnum( Expr, Number )

  ablexpr *Expr;
  int      Number;
{
  int Counter;

  if ( ! ABL_ATOM( Expr ) ) 
  {
    Counter = 0;

    while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
    {
      if ( Counter ==  Number ) return( ABL_CAR( Expr ) );

      Counter = Counter + 1;
    }
  }
  
  return( (ablexpr *)0 );
}
