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
| Tool    :                   DREAL                           |
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
# include "GRD.h"
# include "GSB.h"
# include "GRD_undo.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  drealundo *DrealHeadUndo = (drealundo *)NULL;
  drealundo *DrealHeadRedo = (drealundo *)NULL;

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
|                        DrealAllocUndo                       |
|                                                             |
\------------------------------------------------------------*/

drealundo *DrealAllocUndo()
{
  return((drealundo *)rdsalloc(sizeof(drealundo), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                      DrealAllocUndoRec                      |
|                                                             |
\------------------------------------------------------------*/

drealundorec *DrealAllocUndoRec()
{
  return((drealundorec *)rdsalloc(sizeof(drealundorec), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrealFreeUndo                       |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeUndo( FreeUndo )

   drealundo *FreeUndo;
{
  rdsfree((char *)FreeUndo, sizeof(drealundo));
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealFreeUndoRec                      |
|                                                             |
\------------------------------------------------------------*/

void DrealFreeUndoRec( FreeUndoRec )

   drealundorec *FreeUndoRec;
{
  rdsfree((char *)FreeUndoRec, sizeof(drealundorec));
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddUndoRec                       |
|                                                             |
\------------------------------------------------------------*/

void DrealAddUndoRec( Rectangle )

   rdsrec_list *Rectangle;
{
  drealundorec *UndoRec;

  rdsbegin();

  UndoRec             = DrealAllocUndoRec();
  UndoRec->RECTANGLE  = Rectangle;
  UndoRec->NEXT       = DrealHeadUndo->UNDO;
  DrealHeadUndo->UNDO = UndoRec;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealAddUndo                          |
|                                                             |
\------------------------------------------------------------*/

void DrealAddUndo()
 
{
  drealundo *NewUndo;

  rdsbegin();

  DrealDelUndo( &DrealHeadRedo );

  NewUndo       = DrealAllocUndo();
  NewUndo->NEXT = DrealHeadUndo;
  DrealHeadUndo = NewUndo;

  DrealRecomputeBound = DREAL_TRUE;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       DrealEraseUndo                        |
|                                                             |
\------------------------------------------------------------*/

void DrealEraseUndo( HeadUndo )

  drealundo **HeadUndo;
{
  drealundo    *ScanUndo;
  drealundo    *DelUndo;
  drealundorec *ScanUndoRec;
  drealundorec *DelUndoRec;

  rdsbegin();

  ScanUndo  = *HeadUndo;
  *HeadUndo = (drealundo *)NULL;

  while ( ScanUndo != (drealundo *)NULL )
  {
    ScanUndoRec = ScanUndo->UNDO;

    while( ScanUndoRec != (drealundorec *)NULL )
    {
      DelUndoRec  = ScanUndoRec;
      ScanUndoRec = ScanUndoRec->NEXT;
    
      DrealFreeUndoRec( DelUndoRec );
    }

    DelUndo  = ScanUndo;
    ScanUndo = ScanUndo->NEXT;

    DrealFreeUndo( DelUndo );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealInitializeUndo                     |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeUndo()

{
  rdsbegin();

  DrealEraseUndo( &DrealHeadUndo );
  DrealEraseUndo( &DrealHeadRedo );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        DrealDelUndo                         |
|                                                             |
\------------------------------------------------------------*/

void DrealDelUndo( HeadUndo )

  drealundo **HeadUndo;
{
  drealundo     *ScanUndo;
  drealundo     *DelUndo;
  drealundorec  *ScanUndoRec;
  drealundorec  *DelUndoRec;
  rdsrec_list   *ScanRec;
  drealundorec  *HeadDelete;
  drealundorec  *Delete;

  rdsbegin();
 
  HeadDelete = (drealundorec *)NULL;
  ScanUndo   = *HeadUndo;
  *HeadUndo  = (drealundo *)NULL;

  while ( ScanUndo != (drealundo *)NULL )
  {
    ScanUndoRec = ScanUndo->UNDO;
 
    while( ScanUndoRec != (drealundorec *)NULL )
    {
      ScanRec = ScanUndoRec->RECTANGLE; 
 
      if ( IsDrealDeleted( ScanRec ) )
      {
        DrealUndeleteRectangle( ScanRec );
 
        Delete       = ScanUndoRec;
        ScanUndoRec  = ScanUndoRec->NEXT;
        Delete->NEXT = HeadDelete;
        HeadDelete   = Delete;
      }
      else
      {
        DelUndoRec   = ScanUndoRec;
        ScanUndoRec  = ScanUndoRec->NEXT;

        DrealFreeUndoRec( DelUndoRec );
      }
    }

    DelUndo  = ScanUndo;
    ScanUndo = ScanUndo->NEXT;

    DrealFreeUndo( DelUndo );
  }

  while( HeadDelete != (drealundorec *)NULL )
  {
    Delete     = HeadDelete;
    HeadDelete = HeadDelete->NEXT;
 
    DrealDelRectangle( Delete->RECTANGLE );
 
    DrealFreeUndoRec( Delete );
  }
 
  rdsend();
}
