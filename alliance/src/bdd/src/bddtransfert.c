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
/*----------------------------------------------------------------------*/
/*                               Includes                               */
/*----------------------------------------------------------------------*/

# include MUT_H
# include AUT_H
# include ABL_H
# include BDD_H

# include <stdio.h>
# include "bddtransfert.h"

/*----------------------------------------------------------------------*/
/*                        Constant declarations                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                          Type declarations                           */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                        Stucture declarations                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                        Variable declarations                         */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                          Macro declarations                          */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                      Static function prototypes                      */
/*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*/
/*                    Definition of static functions                    */
/*----------------------------------------------------------------------*/

/* Performs the recursive step of bddtransfertrec
 */
static bddnode *
transfertbddrec( SystemSrc, SystemDst, Node, HashTable )

  bddsystem *SystemSrc;
  bddsystem *SystemDst;
  bddnode   *Node;
  authtable *HashTable;
{
  authelem *Element;
  bddindex  Index;
  bddnode  *High;
  bddnode  *Low;
  bddnode  *Res;

  if ( Node == SystemSrc->ONE )
  {
    return( SystemDst->ONE );
  }
  
  if ( Node == SystemSrc->ZERO )
  {
    return( SystemDst->ZERO );
  }
    
  if( ( Element = searchauthelem( HashTable, (char *)Node ) ) != NULL )
      return( (bddnode *)( Element->VALUE ) );
  
  Index = Node->INDEX;

  High = transfertbddrec( SystemSrc, SystemDst, Node->HIGH, HashTable );
  Low  = transfertbddrec( SystemSrc, SystemDst, Node->LOW,  HashTable );

  Res = addbddnode( SystemDst, Index, High, Low );

  addauthelem( HashTable, (char *)Node, (long)Res );
  return( Res );
}

/*----------------------------------------------------------------------*/
/*                   Definition of exported functions                   */
/*----------------------------------------------------------------------*/

/* Convert a BDD from a system to another one.
 */
bddnode *
transfertbdd( SystemSrc, SystemDst, Node )

  bddsystem *SystemSrc;
  bddsystem *SystemDst;
  bddnode   *Node;
{
  bddnode   *Res;
  authtable *HashTable;

  HashTable = createauthtable( 1000 );
  Res = transfertbddrec( SystemSrc, SystemDst, Node, HashTable );
  destroyauthtable( HashTable );
  return( Res );

}

