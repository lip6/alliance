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
| Tool    :                     Scp                           |
|                                                             |
| File    :                   scpadd.c                        |
|                                                             |
| Date    :                   28.04.98                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"

# include <stdio.h>
# include "scp.h"
# include "scpadd.h"

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Scp Add Functions                      |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                        Sch Add Cell                         |
|                                                             |
\------------------------------------------------------------*/

scpcel_list *addscpcel( Column )

  scpcol_list *Column;
{
  scpcel_list *Cell;


  Cell = allocscpcel(); 

  Cell->COL = Column;

  Cell->NEXT = Column->CELL;
  
  Cell->PREV = NULL; 
  
  if ( Column->CELL != NULL )
  {
    Column->CELL->PREV = Cell;
  }
  else
  {
    Column->LAST_CELL = Cell;
  }

  Column->CELL = Cell;
  
  Column->NUMBER_CELL++;

  return( Cell );
}

scpcel_list *addschboxtoscpcel( SchBox, ScpCel)
schbox_list *SchBox;
scpcel_list *ScpCel;
{
    ScpCel->ROOT = SchBox;
    ScpCel->TYPE = SCP_CEL_BOX;

    SchBox->USER = (void *)ScpCel;

    return( ScpCel );
}

scpcel_list *addschcontoscpcel( SchCon, ScpCel)
schcon_list *SchCon;
scpcel_list *ScpCel;
{
    ScpCel->ROOT = SchCon;
    ScpCel->TYPE = SCP_CEL_CON;

    SchCon->USER = (void *)ScpCel;

    return( ScpCel );
}
/*------------------------------------------------------------\
|                                                             |
|                        Scp Add Column                       |
|                                                             |
\------------------------------------------------------------*/

scpcol_list *addscpcol( )

{
  scpcol_list *Column;

  Column = allocscpcol();

  Column->NEXT = HEAD_SCPCOL;
  HEAD_SCPCOL  = Column;

  return( Column );
}
