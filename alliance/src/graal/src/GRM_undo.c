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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Undo.c                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rfm.h"
# include "GRM.h"
# include "GSB.h"
# include "GRM_window.h"
# include "GRM_undo.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  graalundo *GraalHeadUndo = (graalundo *)NULL;
  graalundo *GraalHeadRedo = (graalundo *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Alloc Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        GraalAllocUndo                       |
|                                                             |
\------------------------------------------------------------*/

graalundo *GraalAllocUndo()
{
  return((graalundo *)rdsalloc(sizeof(graalundo), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalAllocUndoRec                      |
|                                                             |
\------------------------------------------------------------*/

graalundorec *GraalAllocUndoRec()
{
  return((graalundorec *)rdsalloc(sizeof(graalundorec), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalFreeUndo                       |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeUndo( FreeUndo )

   graalundo *FreeUndo;
{
  rdsfree((char *)FreeUndo, sizeof(graalundo));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalFreeUndoRec                      |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeUndoRec( FreeUndoRec )

   graalundorec *FreeUndoRec;
{
  rdsfree((char *)FreeUndoRec, sizeof(graalundorec));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddUndoRec                       |
|                                                             |
\------------------------------------------------------------*/

void GraalAddUndoRec( Rectangle )

   rdsrec_list *Rectangle;
{
  graalundorec *UndoRec;

  rdsbegin();

  UndoRec             = GraalAllocUndoRec();
  UndoRec->RECTANGLE  = Rectangle;
  UndoRec->NEXT       = GraalHeadUndo->UNDO;
  GraalHeadUndo->UNDO = UndoRec;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddUndo                          |
|                                                             |
\------------------------------------------------------------*/

void GraalAddUndo()
 
{
  graalundo *NewUndo;

  rdsbegin();

  GraalDelUndo( &GraalHeadRedo );

  NewUndo       = GraalAllocUndo();
  NewUndo->NEXT = GraalHeadUndo;
  GraalHeadUndo = NewUndo;

  GraalRecomputeBound = GRAAL_TRUE;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalEraseUndo                        |
|                                                             |
\------------------------------------------------------------*/

void GraalEraseUndo( HeadUndo )

  graalundo **HeadUndo;
{
  graalundo    *ScanUndo;
  graalundo    *DelUndo;
  graalundorec *ScanUndoRec;
  graalundorec *DelUndoRec;

  rdsbegin();

  ScanUndo  = *HeadUndo;
  *HeadUndo = (graalundo *)NULL;

  while ( ScanUndo != (graalundo *)NULL )
  {
    ScanUndoRec = ScanUndo->UNDO;

    while( ScanUndoRec != (graalundorec *)NULL )
    {
      DelUndoRec  = ScanUndoRec;
      ScanUndoRec = ScanUndoRec->NEXT;
    
      GraalFreeUndoRec( DelUndoRec );
    }

    DelUndo  = ScanUndo;
    ScanUndo = ScanUndo->NEXT;

    GraalFreeUndo( DelUndo );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalInitializeUndo                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeUndo()

{
  rdsbegin();

  GraalEraseUndo( &GraalHeadUndo );
  GraalEraseUndo( &GraalHeadRedo );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalDelUndo                         |
|                                                             |
\------------------------------------------------------------*/

void GraalDelUndo( HeadUndo )

  graalundo **HeadUndo;
{
  graalundo     *ScanUndo;
  graalundo     *DelUndo;
  graalundorec  *ScanUndoRec;
  graalundorec  *DelUndoRec;
  rdsrec_list   *ScanRec;
  graalundorec  *HeadDelete;
  graalundorec  *Delete;

  rdsbegin();
 
  HeadDelete = (graalundorec *)NULL;
  ScanUndo   = *HeadUndo;
  *HeadUndo  = (graalundo *)NULL;

  while ( ScanUndo != (graalundo *)NULL )
  {
    ScanUndoRec = ScanUndo->UNDO;
 
    while( ScanUndoRec != (graalundorec *)NULL )
    {
      ScanRec = ScanUndoRec->RECTANGLE; 
 
      if ( IsGraalDeleted( ScanRec ) )
      {
        GraalUndeleteRectangle( ScanRec );
 
        Delete       = ScanUndoRec;
        ScanUndoRec  = ScanUndoRec->NEXT;
        Delete->NEXT = HeadDelete;
        HeadDelete   = Delete;
      }
      else
      {
        DelUndoRec   = ScanUndoRec;
        ScanUndoRec  = ScanUndoRec->NEXT;

        GraalFreeUndoRec( DelUndoRec );
      }
    }

    DelUndo  = ScanUndo;
    ScanUndo = ScanUndo->NEXT;

    GraalFreeUndo( DelUndo );
  }

  while( HeadDelete != (graalundorec *)NULL )
  {
    Delete     = HeadDelete;
    HeadDelete = HeadDelete->NEXT;
 
    if ( IsRdsInstance( Delete->RECTANGLE ) )
    {
      GraalDelInstance( GRAAL_PREVIOUS( Delete->RECTANGLE ) );
    }
    else
    {
      GraalDelRectangle( Delete->RECTANGLE );
    }
 
    GraalFreeUndoRec( Delete );
  }
 
  rdsend();
}
