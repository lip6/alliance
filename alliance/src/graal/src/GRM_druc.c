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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Druc.c                          |
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
# include "vrd.h"
# include "GRM.h"
# include "GSB.h"
# include "GRM_window.h"
# include "GRM_druc.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

   rdsfig_list *GraalFigureDruc    = (rdsfig_list *)0;
   rdsrec_list *GraalRectangleDruc = (rdsrec_list *)0;
   rdsrec_list *GraalCurrentDruc   = (rdsrec_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       GraalAddDruc                          |
|                                                             |
\------------------------------------------------------------*/

rdsrec_list *GraalAddDruc( DrucRec )

   rdsrec_list *DrucRec;
{
  rdsrec_list *ScanDrucRec;
  rdsrec_list *ScanRec;
  rdsrec_list *First;
  rdsrec_list *Link;

  rdsbegin();

  First = (rdsrec_list *)NULL;

  if ( ! IsGraalDruc( DrucRec ) )
  {
    ScanDrucRec = DrucRec;

    do
    {
      ScanRec = 
   
        addrdsfigrec( GraalFigureRds, ScanDrucRec->NAME, 
                      GetRdsLayer( ScanDrucRec ),
                      ScanDrucRec->X, ScanDrucRec->Y,
                      ScanDrucRec->DX, ScanDrucRec->DY );

      SetGraalDruc( ScanRec );
      SetGraalDruc( ScanDrucRec );

      if ( ScanRec->NEXT != (rdsrec_list *)NULL )
      {
        GRAAL_PREVIOUS( ScanRec->NEXT ) = &ScanRec->NEXT;
      }

      GRAAL_PREVIOUS( ScanRec ) = 

        &GraalFigureRds->LAYERTAB[ GetRdsLayer( ScanRec ) ];

      GraalInsertRectangle( ScanRec );

      if ( First == (rdsrec_list *)NULL )
      {
        First = ScanRec;
        Link  = ScanRec;
      }
      else
      {
        Link->USER = (void *)ScanRec;
        Link       = ScanRec;
      }

      ScanDrucRec = MACRO_NEXT_EQUI( ScanDrucRec );
    }
    while ( ScanDrucRec != DrucRec );

    Link->USER = (void *)First;
  }

  rdsend();
  return( First );
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDelDruc                          |
|                                                             |
\------------------------------------------------------------*/

void GraalDelDruc( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;
  rdsrec_list *DelRec;

  rdsbegin();

  ScanRec = Rectangle;

  do
  {
    DelRec  = ScanRec;
    ScanRec = (rdsrec_list *)(ScanRec->USER);

    *(GRAAL_PREVIOUS( DelRec )) = DelRec->NEXT;

    if ( DelRec->NEXT != (rdsrec_list *)NULL )
    {
      GRAAL_PREVIOUS( DelRec->NEXT ) = GRAAL_PREVIOUS( DelRec );
    }

    GraalEraseRectangle( DelRec );

    freerdsrec( DelRec, GRAAL_SIZE );
  }
  while ( ScanRec != Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDelDrucFigure                    |
|                                                             |
\------------------------------------------------------------*/

void GraalDelDrucFigure()
{
  if ( GraalFigureDruc != (rdsfig_list *)0 )
  {
    GraalCurrentDruc = (rdsrec_list *)0;

    delrdsfig( GraalFigureDruc->NAME );
    GraalFigureDruc = (rdsfig_list *)0;

    if ( GraalRectangleDruc != (rdsrec_list *)0 )
    {
      GraalDelDruc( GraalRectangleDruc );
      GraalRectangleDruc = (rdsrec_list *)0;
    }
  }
}
