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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     Sch                           |
|                                                             |
| File    :                   schdel.c                        |
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
# include "schdel.h"
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
|                      Sch Del Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Sch Del Connector List               |
|                                                             |
\------------------------------------------------------------*/

static void loc_delschcon( HeadCon )
  
   schcon_list *HeadCon;
{
  schcon_list *ScanCon;
  schcon_list *DelCon;

  ScanCon = HeadCon;
  
  while ( ScanCon != (schcon_list *)0 )
  {
    DelCon  = ScanCon;
    ScanCon = ScanCon->NEXT;
    
    freeschcon( DelCon );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Del Wire List                    |
|                                                             |
\------------------------------------------------------------*/

static void loc_delschwir( HeadWire )
  
   schwir_list *HeadWire;
{
  schwir_list *ScanWire;
  schwir_list *DelWire;

  ScanWire = HeadWire;
  
  while ( ScanWire != (schwir_list *)0 )
  {
    DelWire  = ScanWire;
    ScanWire = ScanWire->NEXT;
    
    freeschwir( DelWire );
  }
}


/*------------------------------------------------------------\
|                                                             |
|                        Sch Del Net List                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_delschnet( HeadNet )
  
   schnet_list *HeadNet;
{
  schnet_list *ScanNet;
  schnet_list *DelNet;

  ScanNet = HeadNet;
  
  while ( ScanNet != (schnet_list *)0 )
  {
    DelNet  = ScanNet;
    ScanNet = ScanNet->NEXT;
    
    loc_delschwir( DelNet->WIRE );

    if ( DelNet->CON_NET != (chain_list *)0 )
    {
      freechain( DelNet->CON_NET );
    }
    
    freeschnet( DelNet );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Del Box List                     |
|                                                             |
\------------------------------------------------------------*/

static void loc_delschbox( HeadBox )
  
   schbox_list *HeadBox;
{
  schbox_list *ScanBox;
  schbox_list *DelBox;

  ScanBox = HeadBox;
  
  while ( ScanBox != (schbox_list *)0 )
  {
    DelBox  = ScanBox;
    ScanBox = ScanBox->NEXT;
    
    loc_delschcon( DelBox->CON_IN  );
    loc_delschcon( DelBox->CON_OUT );
    
    freeschbox( DelBox );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch Del Figure                       |
|                                                             |
\------------------------------------------------------------*/

int delschfig( Name )

  char *Name;
{
  schfig_list    *Figure;
  schfig_list   **Previous;

  Name     = namealloc( Name );
  Previous = &HEAD_SCHFIG;

  for ( Figure  = HEAD_SCHFIG;
        Figure != (schfig_list *)0;
        Figure  = Figure->NEXT )
  {
    if ( Figure->NAME == Name ) break;

    Previous = &Figure->NEXT;
  }

  if ( Figure == (schfig_list *)0 ) return( 0 );

  *Previous = Figure->NEXT;

  loc_delschcon( Figure->CON_IN  );
  loc_delschcon( Figure->CON_OUT );

  loc_delschnet( Figure->NET );
  loc_delschbox( Figure->BOX );

  freeschfig( Figure );

  return( 1 );
}

