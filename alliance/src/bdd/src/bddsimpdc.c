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
| Tool    :                     Bdd                           |
|                                                             |
| File    :                  bddsimpdc.c                      |
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
# include "bdd.h"

# include <stdio.h>
# include "bddapply.h"
# include "bddsimpdc.h"
# include "bdderror.h"

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
|                  Local Simplify Node Dc On                  |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_simpbdddcon( BddOn, BddDc )

  bddnode *BddOn;
  bddnode *BddDc;
{
  bddhoper *HashOper;
  bddnode  *BddNode;
  bddnode  *BddNode1;

  if ( BddDc == BddLocalSystem->ZERO )
  {
    return( incbddrefext( BddOn ) );
  }

  if ( BddOn == BddDc )
  {
    return( BddLocalSystem->ZERO );
  }

  if ( BddOn->INDEX < BDD_INDEX_MIN )
  {
    return( BddOn );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_SIMP_DC_ON, BddOn, BddDc );

  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( BddOn->INDEX == BddDc->INDEX )
  {
    if ( BddDc->HIGH == BddLocalSystem->ONE )
    {
      return( loc_simpbdddcon( BddOn->LOW, BddDc->LOW ) );
    }

    if ( BddDc->LOW == BddLocalSystem->ONE )
    {
      return( loc_simpbdddcon( BddOn->HIGH, BddDc->HIGH ) );
    }

    if ( BddOn->HIGH == BddLocalSystem->ONE )
    {
      BddNode  = decbddrefext( loc_applybddnot( BddDc->HIGH ) );
      BddNode1 = decbddrefext( loc_applybdd( ABL_AND, BddOn->LOW, BddNode ) );

      if ( BddNode1 == BddNode )
      {
        BddNode = decbddrefext( loc_applybdd( ABL_AND, BddDc->HIGH, BddDc->LOW ) );

        return( loc_simpbdddcon( BddOn->LOW, BddNode ) );
      }
    }

    if ( BddOn->LOW == BddLocalSystem->ONE )
    {
      BddNode  = decbddrefext( loc_applybddnot( BddDc->LOW ) );
      BddNode1 = decbddrefext( loc_applybdd( ABL_AND, BddOn->HIGH, BddNode ) );

      if ( BddNode1 == BddNode )
      {
        BddNode = decbddrefext( loc_applybdd( ABL_AND, BddDc->HIGH, BddDc->LOW ) );

        return( loc_simpbdddcon( BddOn->HIGH, BddNode ) );
      }
    }

    BddNode1 = decbddrefext( loc_simpbdddcon( BddOn->HIGH, BddDc->HIGH ) );
    BddNode  = decbddrefext( loc_simpbdddcon( BddOn->LOW , BddDc->LOW  ) );
    BddNode  = addbddnode( (bddsystem *)0, BddOn->INDEX, BddNode1, BddNode );
  }
  else
  if ( BddLocalSystem->INDEX_TO_VAR[ BddOn->INDEX ] <
       BddLocalSystem->INDEX_TO_VAR[ BddDc->INDEX ] )
  {
    BddNode1 = decbddrefext( loc_simpbdddcon( BddOn->HIGH, BddDc ) );
    BddNode  = decbddrefext( loc_simpbdddcon( BddOn->LOW , BddDc ) );
    BddNode  = addbddnode( (bddsystem *)0, BddOn->INDEX, BddNode1, BddNode );
  }
  else
  {
    BddNode1 = decbddrefext( loc_applybdd( ABL_AND, BddDc->HIGH, BddDc->LOW ) );
    BddNode  = loc_simpbdddcon( BddOn, BddNode1 );
  }

  addbddhoper( BddLocalSystem->HASH_OPER, BDD_OPER_SIMP_DC_ON,
               BddOn, BddDc, BddNode );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                  Simplify Bdd Node Dc On                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *simpbddnodedcon( BddSystem, BddOn, BddDc )

  bddsystem *BddSystem;
  bddnode   *BddOn;
  bddnode   *BddDc;
{
  bddnode *BddDcOn;

  setbddlocalsystem( BddSystem );

  BddDcOn = loc_simpbdddcon( BddOn, BddDc );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddDcOn );
}

/*------------------------------------------------------------\
|                                                             |
|                      Simplify Node Dc Off                   |
|                                                             |
\------------------------------------------------------------*/

bddnode *loc_simpbdddcoff( BddOff, BddDc )

  bddnode *BddOff;
  bddnode *BddDc;
{
  bddhoper *HashOper;
  bddnode  *BddNode;
  bddnode  *BddNode1;

  if ( BddDc == BddLocalSystem->ZERO )
  {
    return( incbddrefext( BddOff ) );
  }

  if ( BddOff->INDEX < BDD_INDEX_MIN )
  {
    return( BddOff );
  }

  HashOper = searchbddhoper( BddLocalSystem->HASH_OPER,
                             BDD_OPER_SIMP_DC_OFF, BddOff, BddDc );

  if ( HashOper != (bddhoper *)0 )
  {
    return( incbddrefext( HashOper->FATHER ) );
  }

  if ( BddOff->INDEX == BddDc->INDEX )
  {
    if ( BddDc->HIGH == BddLocalSystem->ONE )
    {
      return( loc_simpbdddcoff( BddOff->LOW, BddDc->LOW ) );
    }

    if ( BddDc->LOW == BddLocalSystem->ONE )
    {
      return( loc_simpbdddcoff( BddOff->HIGH, BddDc->HIGH ) );
    }

    if ( BddOff->HIGH == BddLocalSystem->ZERO )
    {
      BddNode1 = loc_applybdd( ABL_AND, BddOff->LOW, BddDc->HIGH );

      if ( BddNode1 == BddOff->LOW )
      {
        return( BddNode1 );
      }

      decbddrefext( BddNode1 );
    }

    if ( BddOff->LOW == BddLocalSystem->ZERO )
    {
      BddNode1 = loc_applybdd( ABL_AND, BddOff->HIGH, BddDc->LOW );

      if ( BddNode1 == BddOff->HIGH )
      {
        return( BddNode1 );
      }

      decbddrefext( BddNode1 );
    }
/*\
    BddOrH  = decbddrefext( loc_applybdd( ABL_OR , BddOff->LOW , BddDc->LOW  ) );
    BddOrL  = decbddrefext( loc_applybdd( ABL_OR , BddOff->HIGH, BddDc->HIGH ) );
    BddAndH = decbddrefext( loc_applybdd( ABL_AND, BddOff->HIGH, BddOrL      ) );
    BddAndL = decbddrefext( loc_applybdd( ABL_AND, BddOff->LOW , BddOrH      ) );

    if ( ( BddAndH == BddOff->HIGH ) &&
         ( BddAndL == BddOff->LOW  ) )
    {
      BddOrL  = decbddrefext( loc_applybdd( ABL_OR , BddOff->LOW, BddOff->HIGH ) );
      BddAndL = decbddrefext( loc_applybdd( ABL_AND, BddDc->LOW , BddDc->HIGH  ) );
      BddNode = loc_simpbdddcoff( BddOrL, BddAndL );
    }
    else
\*/
    BddNode1 = decbddrefext( loc_simpbdddcoff( BddOff->HIGH, BddDc->HIGH  ) );
    BddNode  = decbddrefext( loc_simpbdddcoff( BddOff->LOW , BddDc->LOW   ) );
    BddNode  = addbddnode( (bddsystem *)0, BddOff->INDEX, BddNode1, BddNode );
  }
  else
  if ( BddLocalSystem->INDEX_TO_VAR[ BddOff->INDEX ] <
       BddLocalSystem->INDEX_TO_VAR[ BddDc->INDEX  ] )
  {
    BddNode1 = decbddrefext( loc_simpbdddcoff( BddOff->HIGH, BddDc ) );
    BddNode  = decbddrefext( loc_simpbdddcoff( BddOff->LOW , BddDc ) );
    BddNode  = addbddnode( (bddsystem *)0, BddOff->INDEX, BddNode1, BddNode );
  }
  else
  {
    BddNode1 = decbddrefext( loc_applybdd( ABL_AND, BddDc->HIGH, BddDc->LOW ) );
    BddNode  = loc_simpbdddcoff( BddOff, BddNode1 );
  }

  addbddhoper( BddLocalSystem->HASH_OPER,
               BDD_OPER_SIMP_DC_OFF, BddOff, BddDc, BddNode );

  return( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                  Simplify Bdd Node Dc On                    |
|                                                             |
\------------------------------------------------------------*/

bddnode *simpbddnodedcoff( BddSystem, BddOff, BddDc )

  bddsystem *BddSystem;
  bddnode   *BddOff;
  bddnode   *BddDc;
{
  bddnode *BddDcOff;

  setbddlocalsystem( BddSystem );

  BddDcOff = loc_simpbdddcoff( BddOff, BddDc );

  checkbddmaxnode( (bddsystem *)0, 1 );

  return( BddDcOff );
}
