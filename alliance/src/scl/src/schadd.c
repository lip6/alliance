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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Sch                           |
|                                                             |
| File    :                   schadd.c                        |
|                                                             |
| Date    :                   04.03.98                        |
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
# include "schadd.h"
# include "scherror.h"

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
|                      Sch Add Functions                      |
|                                                             |
\------------------------------------------------------------*/

schnet_list *addschnetcon( Net, Con)

  schnet_list *Net;
  schcon_list *Con;

{
  Con->NET = Net;
  Net->CON_NET = addchain( Net->CON_NET, (void *)Con );

  if ( IsSchConExternal( Con ) )
  {
    if ( IsSchConIn( Con ) ) Net->NUMBER_IN++; 
    else                     Net->NUMBER_OUT++;
  }
  else
  {
    if ( IsSchConIn( Con ) ) Net->NUMBER_OUT++; 
    else                     Net->NUMBER_IN++;
  }

  return ( Net );
}  

/*------------------------------------------------------------\
|                                                             |
|                        Sch Add Figure                       |
|                                                             |
\------------------------------------------------------------*/

schfig_list *addschfig( Name )

     char *Name;

{
  schfig_list *Figure;

  Figure = allocschfig();

  Figure->NAME = namealloc( Name );

  Figure->NEXT = HEAD_SCHFIG;
  HEAD_SCHFIG  = Figure;
 
  return( Figure );
}

/*------------------------------------------------------------\
|                                                             |
|                          Sch Add Box                        |
|                                                             |
\------------------------------------------------------------*/

schbox_list *addschbox( Figure, Name )

     schfig_list *Figure;
     char        *Name;
{
  schbox_list *Box;

  Box = allocschbox();

  Box->NAME = namealloc( Name );

  Box->NEXT    = Figure->BOX;
  Figure->BOX  = Box;

  return( Box );
}

/*------------------------------------------------------------\
|                                                             |
|                       Sch Add Box Connector                 |
|                                                             |
\------------------------------------------------------------*/

schcon_list *addschboxcon( Figure, Box, Name, Type )

  schfig_list   *Figure;
  schbox_list   *Box;
  char          *Name;
  unsigned char  Type;
{
  schcon_list  *Connector;

  Connector = allocschcon();

  Connector->NAME = namealloc( Name  );

  if ( Type == SCH_CON_IN )
  {
    Connector->NEXT = Box->CON_IN;
    Box->CON_IN     = Connector;

    Box->NUMBER_IN++;
  }
  else
  {
    Connector->NEXT = Box->CON_OUT;
    Box->CON_OUT    = Connector;

    Box->NUMBER_OUT++;
  }

  if ( Type == SCH_CON_IN ) SetSchConIn( Connector );
  else                      SetSchConOut( Connector );

  SetSchConInternal( Connector );

  Connector->ROOT_TYPE = SCH_ROOT_CON_BOX;
  Connector->ROOT      = (void *)Box;
 
  return( Connector );
}

/*------------------------------------------------------------\
|                                                             |
|                   Sch Add Figure Connector                  |
|                                                             |
\------------------------------------------------------------*/

schcon_list *addschfigcon( Figure, Name, Type )

  schfig_list   *Figure;
  char          *Name;
  unsigned char  Type;
{
  schcon_list  *Connector;

  Connector = allocschcon();

  Connector->NAME = namealloc( Name  );

  if ( Type == SCH_CON_IN )
  {
    Connector->NEXT = Figure->CON_IN;
    Figure->CON_IN  = Connector;

    Figure->NUMBER_IN++;
  }
  else
  {
    Connector->NEXT = Figure->CON_OUT;
    Figure->CON_OUT = Connector;

    Figure->NUMBER_OUT++;
  }

  if ( Type == SCH_CON_IN ) SetSchConIn( Connector );
  else                      SetSchConOut( Connector );

  SetSchConExternal( Connector );
  
  Connector->ROOT_TYPE = SCH_ROOT_CON_FIG;
  Connector->ROOT      = (void *)Figure;

  return( Connector );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Add Net                          |
|                                                             |
\------------------------------------------------------------*/

schnet_list *addschnet( Figure  )

  schfig_list *Figure;
{
  schnet_list *Net;

  Net = allocschnet();

  Net->NEXT   = Figure->NET;
  Figure->NET = Net;

  return( Net );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Add Wire                         |
|                                                             |
\------------------------------------------------------------*/

schwir_list *addschwir( Figure, Net )

  schfig_list *Figure;
  schnet_list *Net;
{
  schwir_list *Wire;

  Wire = allocschwir();

  Wire->NEXT = Net->WIRE;
  Net->WIRE  = Wire;

  return( Wire );
}

