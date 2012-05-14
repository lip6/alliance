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
| File    :                 Connector.c                       |
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
# include "GRM_connector.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  graalconrec *GraalHeadConRec = (graalconrec *)NULL;

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
|                        GraalAllocConRec                     |
|                                                             |
\------------------------------------------------------------*/

graalconrec *GraalAllocConRec()
{
  return((graalconrec *)rdsalloc(sizeof(graalconrec), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalFreeConRec                     |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeConRec( FreeConRec )

   graalconrec *FreeConRec;
{
  rdsfree((char *)FreeConRec, sizeof(graalconrec));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDelAllConRec                     |
|                                                             |
\------------------------------------------------------------*/

void GraalDelAllConRec()

{
  graalconrec *ConRec;
  graalconrec *DelConRec;

  rdsbegin();

  ConRec = GraalHeadConRec;

  while ( ConRec != (graalconrec *)NULL )
  {
    DelConRec = ConRec;
    ConRec    = ConRec->NEXT;

    GraalFreeConRec( DelConRec );
  }

  GraalHeadConRec = (graalconrec *)NULL;

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                        GraalDelConRec                       |
|                                                             |
\------------------------------------------------------------*/

void GraalDelConRec( Rectangle )
  
  rdsrec_list *Rectangle;
{
  rdsrec_list  *ScanRec;
  graalconrec  *ConRec;
  graalconrec **Previous;

  rdsbegin();

  Previous = &GraalHeadConRec;

  for ( ConRec  = GraalHeadConRec;
        ConRec != (graalconrec *)NULL;
        ConRec  = ConRec->NEXT )
  {
    ScanRec = Rectangle;

    do
    {
      if ( ConRec->RECTANGLE == ScanRec )
      {
        *Previous = ConRec->NEXT;

        GraalFreeConRec( ConRec );

        return;
      }

      ScanRec = (rdsrec_list *)ScanRec->USER;
    }
    while ( ScanRec != Rectangle );

    Previous = &ConRec->NEXT;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         GraalAddConRec                      |
|                                                             |
\------------------------------------------------------------*/

void GraalAddConRec( Rectangle )
  
  rdsrec_list *Rectangle;
{
  graalconrec *ConRec;

  rdsbegin();

  ConRec = GraalAllocConRec();

  ConRec->NEXT      = GraalHeadConRec;
  ConRec->RECTANGLE = Rectangle; 
  GraalHeadConRec   = ConRec;

  rdsend();
}
