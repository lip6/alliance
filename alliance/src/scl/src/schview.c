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
| File    :                  schview.c                        |
|                                                             |
| Date    :                   11.03.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"

# include "schview.h"
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
|                       Sch View Functions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Sch View Wire                        |
|                                                             |
\------------------------------------------------------------*/

void viewschwir( Wire )

  schwir_list *Wire;
{
  fprintf( stdout, "\n--> Wire" );
  fprintf( stdout, "\n  X     : %ld", Wire->X );
  fprintf( stdout, "\n  Y     : %ld", Wire->Y );     
  fprintf( stdout, "\n  DX    : %ld", Wire->DX ); 
  fprintf( stdout, "\n  DX    : %ld", Wire->DY ); 
  fprintf( stdout, "\n  FLAGS : %lx", Wire->FLAGS );
  fprintf( stdout, "\n  USER  : %lx", (long)Wire->USER );
  fprintf( stdout, "\n<-- Wire" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch View Net                         |
|                                                             |
/------------------------------------------------------------*/

void viewschnet( Net )

  schnet_list *Net ;
{
  schwir_list *ScanWire;
  schcon_list *ScanCon;
  chain_list  *ScanChain;
  char        *RootName;

  fprintf( stdout, "\n  --> Net" );
  fprintf( stdout, "\n  NUMBER_IN   : %ld", Net->NUMBER_IN );
  fprintf( stdout, "\n  NUMBER_OUT  : %ld", Net->NUMBER_OUT ); 
  fprintf( stdout, "\n  FLAGS       : %lx", Net->FLAGS );
  
  fprintf( stdout, "\n  SOURCE_TYPE : %d", (int)Net->SOURCE_TYPE );
  fprintf( stdout, "\n  USER        : %lx", (long)Net->USER );

  fprintf( stdout, "\n  CON_NET     :" );

  for ( ScanChain  = Net->CON_NET;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanCon = (schcon_list *)ScanChain->DATA;

    if ( ScanCon->ROOT_TYPE == SCH_ROOT_CON_FIG )
    {
      RootName = ((schfig_list *)ScanCon->ROOT)->NAME;
      fprintf( stdout, "\n  \t # %s\tof fig %s", ScanCon->NAME, RootName );
    }
    else
    {
      RootName = ((schbox_list *)ScanCon->ROOT)->NAME;
      fprintf( stdout, "\n  \t # %s\tof box %s", ScanCon->NAME, RootName );
    }
  }

  fprintf( stdout, "\n  WIRE :" );

  for ( ScanWire  = Net->WIRE;
        ScanWire != (schwir_list *)0;
        ScanWire  = ScanWire->NEXT )
  {
    viewschwir( ScanWire );
  }

  fprintf( stdout, "\n<-- Net" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch View Connector                   |
|                                                             |
\------------------------------------------------------------*/

void viewschcon( Con )

  schcon_list *Con ;
{
  fprintf( stdout, "\n  --> Con" );
  fprintf( stdout, "\n  \tNAME        : %s", Con->NAME );
  fprintf( stdout, "\n  \tSOURCE_TYPE : %d", (int)Con->SOURCE_TYPE );
  fprintf( stdout, "\n  \tDIR         : %d", (int)Con->DIR );
  fprintf( stdout, "\n  \tTYPE        : Ox%x", (int)Con->TYPE );
  fprintf( stdout, "\n  \tROOT_TYPE   : %d", (int)Con->ROOT_TYPE );
  fprintf( stdout, "\n  \tX_REL       : %lx", Con->X_REL );
  fprintf( stdout, "\n  \tY_REL       : %lx", Con->Y_REL );
  fprintf( stdout, "\n  \tFLAGS       : %lx", Con->FLAGS );
  fprintf( stdout, "\n  \tUSER        : %lx", (long)Con->USER );
  fprintf( stdout, "\n  <-- Con" );
}

/*------------------------------------------------------------\
|                                                             |
|                     Sch View Box                            |
|                                                             |
\------------------------------------------------------------*/

void viewschbox( Box )

  schbox_list *Box;
{
  schcon_list *ScanCon;

  fprintf( stdout, "\n--> Box" );
  fprintf( stdout, "\n  NAME        : %s", Box->NAME );
  fprintf( stdout, "\n  SOURCE_TYPE : %d", (int)Box->SOURCE_TYPE );
  fprintf( stdout, "\n  NUMBER_IN   : %ld", Box->NUMBER_IN );
  fprintf( stdout, "\n  NUMBER_OUT  : %ld", Box->NUMBER_OUT ); 
  fprintf( stdout, "\n  X           : %ld", Box->X );
  fprintf( stdout, "\n  Y           : %ld", Box->Y );
  fprintf( stdout, "\n  DX          : %ld", Box->DX );
  fprintf( stdout, "\n  DY          : %ld", Box->DY );
  fprintf( stdout, "\n  FLAGS       : %lx", Box->FLAGS );
  fprintf( stdout, "\n  USER        : %lx", (long)Box->USER );

  fprintf( stdout, "\n  CON_IN : " );

  for ( ScanCon  = Box->CON_IN;
        ScanCon != (schcon_list *)0;
        ScanCon  = ScanCon->NEXT )
  {
    viewschcon( ScanCon );
  }

  fprintf( stdout, "\n  CON_OUT : " );

  for ( ScanCon  = Box->CON_OUT;
        ScanCon != (schcon_list *)0;
        ScanCon  = ScanCon->NEXT )
  {
    viewschcon( ScanCon );
  }

  fprintf( stdout, "\n<-- Box" );
}

/*------------------------------------------------------------\
|                                                             |
|                        Sch View Figure                      |
|                                                             |
\------------------------------------------------------------*/

void viewschfig( Figure )

  schfig_list *Figure;
{
  schcon_list   *ScanCon;
  schbox_list   *ScanBox;
  schnet_list   *ScanNet;

  fprintf( stdout, "\n--> Figure" );
  fprintf( stdout, "\n  NAME        : %s", Figure->NAME  );
  fprintf( stdout, "\n  SOURCE_TYPE : %d", (int)Figure->SOURCE_TYPE );
  fprintf( stdout, "\n  NUMBER_IN   : %ld", Figure->NUMBER_IN );
  fprintf( stdout, "\n  NUMBER_OUT  : %ld", Figure->NUMBER_OUT );
  fprintf( stdout, "\n  X           : %ld", Figure->X );
  fprintf( stdout, "\n  Y           : %ld", Figure->Y );
  fprintf( stdout, "\n  FLAGS       : %lx", Figure->FLAGS );
  fprintf( stdout, "\n  USER        : %lx", (long)Figure->USER );

  fprintf( stdout, "\n  CON_IN : " );

  for ( ScanCon  = Figure->CON_IN;
        ScanCon != (schcon_list *)0;
        ScanCon  = ScanCon->NEXT )
  {
    viewschcon( ScanCon );
  }

  fprintf( stdout, "\n  CON_OUT : " );

  for ( ScanCon  = Figure->CON_OUT;
        ScanCon != (schcon_list *)0;
        ScanCon  = ScanCon->NEXT )
  {
    viewschcon( ScanCon );
  }

  fprintf( stdout, "\n  BOX : " );

  for ( ScanBox  = Figure->BOX;
        ScanBox != (schbox_list *)0;
        ScanBox  = ScanBox->NEXT )
  {
    viewschbox( ScanBox );
  }

  fprintf( stdout, "\n  NET : " );

  for ( ScanNet  = Figure->NET;
        ScanNet != (schnet_list *)0;
        ScanNet  = ScanNet->NEXT )
  {
    viewschnet( ScanNet );
  }

  fprintf( stdout, "\n<-- Figure\n" );
}
