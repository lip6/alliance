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
| File    :                  bddsupport.c                     |
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
# include "bddsupport.h"
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

  static chain_list *BddGetHeadChain   = (chain_list *)0;
  static ptype_list *BddGetHeadPType   = (ptype_list *)0;
  static bddvar      BddLocalVariable  = (bddvar)0;
  static long        BddGetSupportSize = 0L;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Local Get Bdd Node Support PType           |
|                                                             |
\------------------------------------------------------------*/

static void loc_getbddsupportptype( BddNode )

  bddnode *BddNode;
{
  bddnode *BddVarNode;

  if ( ( BddNode->INDEX >= BDD_INDEX_MIN          ) &&
       ( ! testbddnodemark( BddLocalSystem, BddNode ) ) )
  {
    BddVarNode = BddLocalSystem->VAR_NODE[ BddNode->INDEX ];

    if ( ! testbddnodemark( BddLocalSystem, BddVarNode ) )
    {
      setbddnodemark( BddLocalSystem, BddVarNode );

      BddGetHeadPType = addptype( BddGetHeadPType, 0, (void *)BddVarNode );
    }

    setbddnodemark( BddLocalSystem, BddNode );

    loc_getbddsupportptype( BddNode->HIGH );
    loc_getbddsupportptype( BddNode->LOW  );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                  Local Get Bdd Node Support Chain           |
|                                                             |
\------------------------------------------------------------*/

static void loc_getbddsupportchain( BddNode )

  bddnode *BddNode;
{
  bddnode *BddVarNode;

  if ( ( BddNode->INDEX >= BDD_INDEX_MIN          ) &&
       ( ! testbddnodemark( BddLocalSystem, BddNode ) ) )
  {
    BddVarNode = BddLocalSystem->VAR_NODE[ BddNode->INDEX ];

    if ( ! testbddnodemark( BddLocalSystem, BddVarNode ) )
    {
      setbddnodemark( BddLocalSystem, BddVarNode );

      BddGetHeadChain = addchain( BddGetHeadChain, (void *)BddVarNode );
    }

    setbddnodemark( BddLocalSystem, BddNode );

    loc_getbddsupportchain( BddNode->HIGH );
    loc_getbddsupportchain( BddNode->LOW  );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Bdd Get Node Support                  |
|                                                             |
\------------------------------------------------------------*/

void *
#ifdef __STDC__
getbddnodesupport( bddsystem *BddSystem, bddnode *BddNode, int Mode )
#else
getbddnodesupport( BddSystem, BddNode, Mode )

  bddsystem *BddSystem;
  bddnode   *BddNode;
  int        Mode;
#endif
{
  setbddlocalsystem( BddSystem );
  newbddsystemmark( BddLocalSystem );

  if ( Mode == BDD_SUPPORT_CHAIN )
  {
    BddGetHeadChain = (chain_list *)0;
    loc_getbddsupportchain( BddNode );
    return (void *)BddGetHeadChain;
  }
  else
  {
    BddGetHeadPType = (ptype_list *)0;
    loc_getbddsupportptype( BddNode );
    return (void *)BddGetHeadPType;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                Local Mark Bdd Variable In Support           |
|                                                             |
\------------------------------------------------------------*/

static int
#ifdef __STDC__
loc_isbddvarinsupport( bddnode *BddNode )
#else
loc_isbddvarinsupport( BddNode )

  bddnode *BddNode;
#endif
{
  bddvar Variable;

  Variable = BddLocalSystem->INDEX_TO_VAR[ BddNode->INDEX ];

  if ( ( BddLocalVariable < Variable            ) ||
       ( testbddnodemark( BddLocalSystem, BddNode ) ) )
  {
    return( 0 );
  }

  if ( BddLocalVariable == Variable )
  {
    return( 1 );
  }

  setbddnodemark( BddLocalSystem, BddNode );

  if ( loc_isbddvarinsupport( BddNode->HIGH ) )
  {
    return( 1 );
  }

  return loc_isbddvarinsupport( BddNode->LOW );
}

/*------------------------------------------------------------\
|                                                             |
|                    Is Bdd Variable In Support               |
|                                                             |
\------------------------------------------------------------*/

int
#ifdef __STDC__
isbddvarinsupport( bddsystem *BddSystem, bddnode *BddNode, bddvar Variable )
#else
isbddvarinsupport( BddSystem, BddNode, Variable )

  bddsystem *BddSystem;
  bddnode   *BddNode;
  bddvar     Variable;
#endif
{
  checkbddvar( BddSystem, Variable, 1 );

  newbddsystemmark( BddLocalSystem );
  BddLocalVariable = Variable;

  return loc_isbddvarinsupport( BddNode );
}

/*------------------------------------------------------------\
|                                                             |
|                   Get Bdd Node Support Size                 |
|                                                             |
\------------------------------------------------------------*/

static void
#ifdef __STDC__
loc_getbddsupportsize( bddnode *BddNode )
#else
loc_getbddsupportsize( BddNode )

  bddnode *BddNode;
#endif
{
  bddnode *BddVarNode;

  if ( ( BddNode->INDEX >= BDD_INDEX_MIN          ) &&
       ( ! testbddnodemark( BddLocalSystem, BddNode ) ) )
  {
    BddVarNode = BddLocalSystem->VAR_NODE[ BddNode->INDEX ];

    if ( ! testbddnodemark( BddLocalSystem, BddVarNode ) )
    {
      setbddnodemark( BddLocalSystem, BddVarNode );

      BddGetSupportSize += 1;
    }

    setbddnodemark( BddLocalSystem, BddNode );

    loc_getbddsupportsize( BddNode->HIGH );
    loc_getbddsupportsize( BddNode->LOW  );
  }
}

long
#ifdef __STDC__
getbddnodesupportsize( bddsystem *BddSystem, bddnode *BddNode )
#else
getbddnodesupportsize( BddSystem, BddNode )

  bddsystem *BddSystem;
  bddnode   *BddNode;
#endif
{
  setbddlocalsystem( BddSystem );
  newbddsystemmark( BddLocalSystem );

  BddGetSupportSize = 0L;
  loc_getbddsupportsize( BddNode );
  return BddGetSupportSize;
}
