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
| Tool    :                    XVPN                           |
|                                                             |
| File    :                  Share.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <Xm/Xm.h>

# include "mut.h"
# include "aut.h"
# include "XSB.h"
# include "XSB_share.h"

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

  short XVPN_ACTIVE_NAME_TABLE [ XVPN_MAX_ACTIVE_NAME ] = 
  {
    1, /* PLACE TEXT  */
    1  /* PLACE TRANS */
  };

  short XVPN_ACTIVE_LAYER_TABLE [ XVPN_MAX_LAYER ] = 
  {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  char *XvpnFirstFileName   = (char *)NULL;
  char *XVPN_XMS_FILE_NAME  = (char *)NULL;

  char XVPN_FORCE_DISPLAY = XVPN_FALSE;
  char XVPN_XOR_CURSOR    = XVPN_FALSE;
  char XVPN_SWITCH_COLOR_MAP = XVPN_FALSE;

/*------------------------------------------------------------\
|                                                             |
|                       Table variables                       |
|                                                             |
\------------------------------------------------------------*/

  char  *XVPN_CURSOR_COLOR_NAME;
  char  *XVPN_BACKGROUND_COLOR_NAME;
  char  *XVPN_FOREGROUND_COLOR_NAME;
  char  *XVPN_ACCEPT_COLOR_NAME;
  char  *XVPN_CONNECT_COLOR_NAME;

  long   XVPN_CURSOR_SIZE;
  float  XVPN_LOWER_GRID_STEP;
  long   XVPN_UNIT;

/*------------------------------------------------------------\
|                                                             |
|                       Global Variables                      |
|                                                             |
\------------------------------------------------------------*/

  char *XVPN_PARAM_NAME = (char *)NULL;
  char *XVPN_LAYER_NAME_TABLE[ XVPN_MAX_LAYER ][ 3 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XvpnPostTreatString                    |
|                                                             |
\------------------------------------------------------------*/

char *XvpnPostTreatString( Text )

  char *Text;
{
  char OnlySpace;
  int  Counter;

  autbegin();

  if ( Text != (char *)NULL )
  {
    OnlySpace = 1;

    for ( Counter = 0; Text[ Counter ]; Counter++ )
    {
      if ( Text[ Counter ] != ' ' ) OnlySpace = 0;
    }

    if ( OnlySpace ) Text = (char *)NULL;
  }

  autend();
  return( Text );
}

/*------------------------------------------------------------\
|                                                             |
|                        Xvpngetenv                           |
|                                                             |
\------------------------------------------------------------*/

void Xvpngetenv()
{
  char *Env;

  Env = mbkgetenv( "HOME" );

  if ( Env == (char *)NULL )
  {
    XVPN_XMS_FILE_NAME = XMS_FILE_NAME;
  }
  else
  {
    XVPN_XMS_FILE_NAME =

     (char *)autallocblock( strlen( Env ) + strlen( XMS_FILE_NAME ) + 2 );

    strcpy( XVPN_XMS_FILE_NAME, Env );
    strcat( XVPN_XMS_FILE_NAME, "/" );
    strcat( XVPN_XMS_FILE_NAME, XMS_FILE_NAME );
  }
}
