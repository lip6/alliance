/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| File    :                  Select.c                         |
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
# include "GRM_select.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  graalselect *GraalHeadSelect  = (graalselect *)NULL;

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
|                        GraalAllocSelect                     |
|                                                             |
\------------------------------------------------------------*/

graalselect *GraalAllocSelect()
{
  return((graalselect *)rdsalloc(sizeof(graalselect), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalFreeSelect                     |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeSelect( FreeSelect )

   graalselect *FreeSelect;
{
  rdsfree((char *)FreeSelect, sizeof(graalselect));
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAcceptRectangle                  |
|                                                             |
\------------------------------------------------------------*/
 
void GraalAcceptRectangle( Rectangle )
 
  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();
 
  ScanRec = Rectangle;
 
  do
  {
    SetGraalAccepted( ScanRec );
 
    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while( Rectangle != ScanRec );

  rdsend();
}

/*------------------------------------------------------------\ 
|                                                             | 
|                       GraalRejectRectangle                  | 
|                                                             |
\------------------------------------------------------------*/ 

void GraalRejectRectangle( Rectangle ) 

  rdsrec_list *Rectangle;
{ 
  rdsrec_list *ScanRec; 

  rdsbegin();

  ScanRec = Rectangle;

  do
  { 
    ClearGraalAccepted( ScanRec ); 

    ScanRec = (rdsrec_list *)ScanRec->USER;
  } 
  while( Rectangle != ScanRec ); 

  rdsend();
} 

/*------------------------------------------------------------\
|                                                             |
|                       GraalSelectRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void GraalSelectRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle;

  do
  {
    SetGraalSelected( ScanRec );

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while( Rectangle != ScanRec );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      GraalUnselectRectangle                 |
|                                                             |
\------------------------------------------------------------*/

void GraalUnselectRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle;

  do
  {
    ClearGraalSelected( ScanRec );

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while( Rectangle != ScanRec );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDeleteRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void GraalDeleteRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle;

  do
  { 
    SetGraalDeleted( ScanRec );

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while( Rectangle != ScanRec );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalUndeleteRectangle                  |
|                                                             |
\------------------------------------------------------------*/

void GraalUndeleteRectangle( Rectangle )

  rdsrec_list *Rectangle;
{
  rdsrec_list *ScanRec;

  rdsbegin();

  ScanRec = Rectangle;

  do
  { 
    ClearGraalDeleted( ScanRec );

    ScanRec = (rdsrec_list *)ScanRec->USER;
  }
  while( Rectangle != ScanRec );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalAddSelect                        |
|                                                             |
\------------------------------------------------------------*/

void GraalAddSelect( Rectangle )

  rdsrec_list *Rectangle;
{
  graalselect *Select;

  rdsbegin();

  Select = GraalAllocSelect();

  Select->RECTANGLE = Rectangle;
  Select->NEXT      = GraalHeadSelect;
  GraalHeadSelect   = Select;

  GraalSelectRectangle( Rectangle );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDelSelect                        |
|                                                             |
\------------------------------------------------------------*/

void GraalDelSelect()
{
  graalselect    *DelSelect;
  graalselect    *Select;

  rdsbegin();

  Select          = GraalHeadSelect;
  GraalHeadSelect = (graalselect *)NULL;

  while( Select != (graalselect *)NULL )
  {
    GraalRejectRectangle( Select->RECTANGLE );

    DelSelect  = Select;
    Select     = Select->NEXT;
    
    GraalFreeSelect( DelSelect );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalPurgeSelect                      |
|                                                             |
\------------------------------------------------------------*/

void GraalPurgeSelect()
{
  graalselect    *DelSelect;
  graalselect    *Select;
  graalselect   **Previous;

  rdsbegin();

  Previous = &GraalHeadSelect;
  Select   =  GraalHeadSelect;

  while( Select != (graalselect *)NULL )
  {
    if ( ! IsGraalAccepted( Select->RECTANGLE ) )
    {
      DelSelect  = Select;
      Select     = Select->NEXT;
      *Previous  = Select;
    
      GraalFreeSelect( DelSelect );
    }
    else
    {
      Previous = &Select->NEXT;
      Select   = Select->NEXT;
    }
  }

  rdsend();
}
