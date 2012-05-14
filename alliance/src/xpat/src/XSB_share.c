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
| Tool    :                    XPAT                           |
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
# include "pat.h"
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

  char XPAT_ACTIVE_NAME_TABLE [ XPAT_MAX_ACTIVE_NAME ] = 
  {
    1, /* PATTERN TEXT */
    1  /* IO      TEXT */
  };

  char XPAT_ACTIVE_LAYER_TABLE [ XPAT_MAX_LAYER ] = 
  {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
  };

  char *XpatFirstFileName  = (char *)NULL;
  char *XPAT_XMS_FILE_NAME = (char *)NULL;

  char XPAT_FORCE_DISPLAY    = XPAT_FALSE;
  char XPAT_XOR_CURSOR       = XPAT_FALSE;
  char XPAT_SWITCH_COLOR_MAP = XPAT_FALSE;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatPostTreatString                    |
|                                                             |
\------------------------------------------------------------*/

char *XpatPostTreatString( Text )

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
|                        Xpatgetenv                           |
|                                                             |
\------------------------------------------------------------*/

void Xpatgetenv()
{
  char *Env;

  Env = mbkgetenv( "HOME" );

  if ( Env == (char *)NULL )
  {
    XPAT_XMS_FILE_NAME = XMS_FILE_NAME;
  }
  else
  {
    XPAT_XMS_FILE_NAME =

     (char *)autallocblock( strlen( Env ) + strlen( XMS_FILE_NAME ) + 2 );

    strcpy( XPAT_XMS_FILE_NAME, Env );
    strcat( XPAT_XMS_FILE_NAME, "/" );
    strcat( XPAT_XMS_FILE_NAME, XMS_FILE_NAME );
  }
}
