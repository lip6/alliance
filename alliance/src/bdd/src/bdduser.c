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
| File    :                  bdduser.c                        |
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

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H

# include <stdio.h>
# include "bdduser.h"
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
|                           Add Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Bdd Add  User Function              |
|                                                             |
\------------------------------------------------------------*/

bdduserfunc *addbdduserfunc( BddSystem, Type, Func, Data )

  bddsystem *BddSystem;
  long       Type;
  void     (*Func)();
  void      *Data;
{
  bdduserfunc *BddUserFunc;

  setbddlocalsystem( BddSystem );

  BddUserFunc               = allocbdduserfunc();
  BddUserFunc->NEXT         = BddLocalSystem->USER_FUNC;
  BddLocalSystem->USER_FUNC = BddUserFunc;

  BddUserFunc->FUNC = Func;
  BddUserFunc->DATA = Data;
  BddUserFunc->TYPE = Type;

  return( BddUserFunc );
}

/*------------------------------------------------------------\
|                                                             |
|                           Del Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Bdd Del User Function               |
|                                                             |
\------------------------------------------------------------*/

int delbdduserfunc( BddSystem, BddUserFunc )

  bddsystem   *BddSystem;
  bdduserfunc *BddUserFunc;
{
  bdduserfunc  *ScanUserFunc;
  bdduserfunc **PrevUserFunc;

  setbddlocalsystem( BddSystem );

  PrevUserFunc = &BddLocalSystem->USER_FUNC;

  for ( ScanUserFunc  = BddLocalSystem->USER_FUNC;
        ScanUserFunc != (bdduserfunc *)0;
        ScanUserFunc  = ScanUserFunc->NEXT )
  {
    if ( ScanUserFunc == BddUserFunc )
    {
      *PrevUserFunc = ScanUserFunc->NEXT;
      freebdduserfunc( ScanUserFunc );

      return( 1 );
    }

    PrevUserFunc = &ScanUserFunc->NEXT;
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Exec Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Bdd Exec User Function              |
|                                                             |
\------------------------------------------------------------*/

void execbdduserfunc( BddSystem, Type )

  bddsystem *BddSystem;
  long       Type;
{
  bdduserfunc *ScanUserFunc;

  setbddlocalsystem( BddSystem );

  for ( ScanUserFunc  = BddLocalSystem->USER_FUNC;
        ScanUserFunc != (bdduserfunc *)0;
        ScanUserFunc  = ScanUserFunc->NEXT )
  {
    if ( ScanUserFunc->TYPE == Type )
    {
      (*ScanUserFunc->FUNC)( ScanUserFunc->DATA );
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Destroy Functions                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                   Bdd Destroy User Function                 |
|                                                             |
\------------------------------------------------------------*/

void destroybdduserfunc( BddSystem )

  bddsystem *BddSystem;
{
  bdduserfunc *ScanUserFunc;
  bdduserfunc *DelUserFunc;

  setbddlocalsystem( BddSystem );

  ScanUserFunc = BddLocalSystem->USER_FUNC;
  BddLocalSystem->USER_FUNC = (bdduserfunc *)0;

  while ( ScanUserFunc != (bdduserfunc *)0 )
  {
    DelUserFunc  = ScanUserFunc;
    ScanUserFunc = ScanUserFunc->NEXT;

    freebdduserfunc( DelUserFunc );
  }
}
