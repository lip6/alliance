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
| File    :                   boom_post.c                     |
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
# include "boom_abl.h"
# include "boom_post.h"

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

  static authtable *BoomHashRename = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      BoomRenameAuxVectorAbl                 |
|                                                             |
\------------------------------------------------------------*/

static void BoomRenameAuxVectorAbl( Expr )

  ablexpr *Expr;
{
  authelem *Element;
  char     *Value;

  if ( ABL_ATOM( Expr ) )
  {
    Value = ABL_ATOM_VALUE( Expr );

    if ( ( Value != ABL_ATOM_NAME_ONE      ) &&
         ( Value != ABL_ATOM_NAME_ZERO     ) &&
         ( Value != ABL_ATOM_NAME_DC       ) &&
         ( Value != ABL_ATOM_NAME_TRISTATE ) )
    {
      Element = searchauthelem( BoomHashRename, Value );

      if ( Element != (authelem *)0 )
      {
        ABL_ATOM_VALUE( Expr ) = (char *)Element->VALUE;
      }
    }

    return;
  }

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    BoomRenameAuxVectorAbl( ABL_CAR( Expr ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      BoomPostTreatBehFigure                 |
|                                                             |
\------------------------------------------------------------*/

void BoomPostTreatBehFigure( BehFigure )

  befig_list *BehFigure;
{
  beaux_list *BehAux;
  char        Buffer[ 512 ];
  char       *Scan;

  if ( BoomHashRename == (authtable *)0 )
  {
    BoomHashRename = createauthtable( 100 );
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    if ( strchr( BehAux->NAME, ' ' ) != (char *)0 )
    {
      strcpy( Buffer, BehAux->NAME );
      Scan  = strchr( Buffer, ' ' );
      *Scan = '_';

      Scan  = namealloc( Buffer );
      addauthelem( BoomHashRename, BehAux->NAME, (long)Scan );
      BehAux->NAME = Scan;
    }
  }

  BoomBehScanAbl( BehFigure, BoomRenameAuxVectorAbl, 1 );

  resetauthtable( BoomHashRename );
}
