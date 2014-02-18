/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/

#include <stdio.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "fvh.h"

static void (*FbhFuncLeft)();
static void (*FbhFuncRight)();

static void FbhScanExprInstruction();

/*------------------------------------------------------------\
|                                                             |
|                         FbhScanExprIf                       |
|                                                             |
\------------------------------------------------------------*/

void FbhScanExprIf( ScanIf )

  fbifs_list *ScanIf;
{
  FbhFuncRight( ScanIf->CND );

  FbhScanExprInstruction( ScanIf->CNDTRUE  );
  FbhScanExprInstruction( ScanIf->CNDFALSE );
}

/*------------------------------------------------------------\
|                                                             |
|                         FbhScanExprAsg                      |
|                                                             |
\------------------------------------------------------------*/

void FbhScanExprAsg( ScanAsg )

  fbasg_list *ScanAsg;
{
  FbhFuncLeft( ScanAsg->NAME );
  FbhFuncRight( ScanAsg->ABL );
}

/*------------------------------------------------------------\
|                                                             |
|                          FbhScanExprWhen                    |
|                                                             |
\------------------------------------------------------------*/


static void FbhScanExprWhen( ScanCase )

  fbcas_list  *ScanCase;
{
  int   Index;

  FbhFuncRight( ScanCase->ABL );

  for ( Index = 0; Index < ScanCase->SIZE; Index++ )
  {
    FbhScanExprInstruction( ScanCase->CHOICE[ Index ].INSTRUCTION );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        FbhScanExprInstruction               |
|                                                             |
\------------------------------------------------------------*/

static void FbhScanExprInstruction( Instruction )

  ptype_list *Instruction;
{
  ptype_list     *ScanIns;

  for ( ScanIns  = Instruction;
        ScanIns != (ptype_list *)0;
        ScanIns  = ScanIns->NEXT )
  {
    switch ( ScanIns->TYPE )
    {
      case FBH_BECAS : FbhScanExprWhen( ScanIns->DATA );
      break;

      case FBH_BEIFS : FbhScanExprIf( ScanIns->DATA );
      break;

      case FBH_BEASG : FbhScanExprAsg( ScanIns->DATA );
      break;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      FbhScanExprProcess                     |
|                                                             |
\------------------------------------------------------------*/

void FbhScanExprProcess( Process, FuncLeft, FuncRight )

fbpcs_list *Process;
void      (*FuncLeft)();
void      (*FuncRight)();
{
  FbhFuncLeft  = FuncLeft;
  FbhFuncRight = FuncRight;

  FbhScanExprInstruction( Process->INSTRUCTION );
}
