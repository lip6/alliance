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
| File    :                  Search.c                         |
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
# include "GRM_search.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  graalsearch *GraalHeadSearch = (graalsearch *)NULL;

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
|                        GraalAllocSearch                     |
|                                                             |
\------------------------------------------------------------*/

graalsearch *GraalAllocSearch()
{
  return((graalsearch *)rdsalloc(sizeof(graalsearch), 1));
}

/*------------------------------------------------------------\
|                                                             |
|                        Free Functions                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalFreeSearch                     |
|                                                             |
\------------------------------------------------------------*/

void GraalFreeSearch( FreeSearch )

   graalsearch *FreeSearch;
{
  rdsfree((char *)FreeSearch, sizeof(graalsearch));
}

/*------------------------------------------------------------\ 
|                                                             | 
|                       GraalSetSearchRectangle               | 
|                                                             |
\------------------------------------------------------------*/ 

void GraalSetSearchRectangle( Rectangle ) 

  rdsrec_list *Rectangle;
{ 
  rdsrec_list *ScanRec; 

  rdsbegin();

  ScanRec = Rectangle;

  do
  { 
    SetGraalSearch( ScanRec ); 

    ScanRec = (rdsrec_list *)ScanRec->USER;
  } 
  while( Rectangle != ScanRec ); 

  rdsend();
} 

/*------------------------------------------------------------\ 
|                                                             | 
|                       GraalClearSearchRectangle             | 
|                                                             |
\------------------------------------------------------------*/ 

void GraalClearSearchRectangle( Rectangle ) 

  rdsrec_list *Rectangle;
{ 
  rdsrec_list *ScanRec; 

  rdsbegin();

  ScanRec = Rectangle;

  do
  { 
    ClearGraalSearch( ScanRec ); 

    ScanRec = (rdsrec_list *)ScanRec->USER;
  } 
  while( Rectangle != ScanRec ); 

  rdsend();
} 


/*------------------------------------------------------------\
|                                                             |
|                       GraalAddSearch                        |
|                                                             |
\------------------------------------------------------------*/

void GraalAddSearch( Rectangle )

   rdsrec_list *Rectangle;
{
  graalsearch *Search;

  rdsbegin();

  if ( ! IsGraalSearch( Rectangle ) )
  {
    Search = GraalAllocSearch();

    Search->RECTANGLE = Rectangle;
    Search->NEXT      = GraalHeadSearch;
    GraalHeadSearch   = Search;

    GraalSetSearchRectangle( Rectangle );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                       GraalDelSearch                        |
|                                                             |
\------------------------------------------------------------*/

void GraalDelSearch()

{
  graalsearch *Search;

  rdsbegin();

  while ( GraalHeadSearch != (graalsearch *)NULL )
  {
    Search          = GraalHeadSearch;
    GraalHeadSearch = GraalHeadSearch->NEXT;

    GraalClearSearchRectangle( Search->RECTANGLE );

    GraalFreeSearch( Search );
  }

  rdsend();
}
