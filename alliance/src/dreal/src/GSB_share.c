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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Share.c                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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
# include <string.h>
# include <stdlib.h>
# include <Xm/Xm.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "GSB.h"
# include "GSB_share.h"

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

  char DREAL_RDS_ACTIVE_LAYER_TABLE[ RDS_ALL_LAYER ] =

  {
    1,  /* RDS_NWELL   */
    1,  /* RDS_PWELL   */
    1,  /* RDS_NIMP    */
    1,  /* RDS_PIMP    */
    1,  /* RDS_ACTIV   */
    1,  /* RDS_NDIF    */
    1,  /* RDS_PDIF    */
    1,  /* RDS_NTIE    */
    1,  /* RDS_PTIE    */
    1,  /* RDS_POLY    */
    1,  /* RDS_VPOLY   */
    1,  /* RDS_GATE    */
    1,  /* RDS_TPOLY   */
    1,  /* RDS_POLY2   */
    1,  /* RDS_CONT    */
    1,  /* RDS_TCONT   */
    1,  /* RDS_CONT2   */
    1,  /* RDS_ALU1    */
    1,  /* RDS_VALU1   */
    1,  /* RDS_TALU1   */
    1,  /* RDS_VIA1    */
    1,  /* RDS_TVIA1   */
    1,  /* RDS_ALU2    */
    1,  /* RDS_VALU2   */
    1,  /* RDS_TALU2   */
    1,  /* RDS_VIA2    */
    1,  /* RDS_TVIA2   */
    1,  /* RDS_ALU3    */
    1,  /* RDS_VALU3   */
    1,  /* RDS_TALU3   */
    1,  /* RDS_VIA3    */
    1,  /* RDS_TVIA3   */
    1,  /* RDS_ALU4    */
    1,  /* RDS_CPAS    */
    1,  /* RDS_REF     */
    1,  /* RDS_USER0   */
    1,  /* RDS_USER1   */
    1,  /* RDS_USER2   */
    1   /* RDS_ABOX    */
  };

  char DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_MAX_ACTIVE_NAME ] = 

  {
    1, /* FIGURE             NAME */
    1, /* INSTANCE           NAME */
    1, /* FIGURE CONNECTOR   NAME */
    1, /* INSTANCE CONNECTOR NAME */
    1, /* FIGURE SEGMENT     NAME */
    1, /* INSTANCE SEGMENT   NAME */
    1, /* FIGURE REFERENCE   NAME */
    1  /* INSTANCE REFERENCE NAME */
  };

/*------------------------------------------------------------\
|                                                             |
|             Fill Mode and Instance Interface                |
|                                                             |
\------------------------------------------------------------*/

  char DREAL_FILL_MODE          = DREAL_FILL_MODE_PATTERN;
  char DREAL_INSTANCE_INTERFACE = DREAL_TRUE;

  char DREAL_XOR_CURSOR       = DREAL_FALSE;
  char DREAL_FORCE_DISPLAY    = DREAL_FALSE;
  char DREAL_BLACK_BOX_STRING = DREAL_FALSE;
  char DREAL_SWITCH_COLOR_MAP = DREAL_FALSE;

/*------------------------------------------------------------\
|                                                             |
|                       File Name                             |
|                                                             |
\------------------------------------------------------------*/

  char *DREAL_GMS_FILE_NAME = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                      DrealPostTreatString                   |
|                                                             |
\------------------------------------------------------------*/

char *DrealPostTreatString( Text )

  char *Text;
{
  char OnlySpace;
  int  Counter;

  rdsbegin();

  if ( Text != (char *)NULL )
  {
    OnlySpace = 1;

    for ( Counter = 0; Text[ Counter ]; Counter++ )
    {
      if ( Text[ Counter ] != ' ' ) OnlySpace = 0;
    }

    if ( OnlySpace ) Text = (char *)NULL;
  }

  rdsend();
  return( Text );
}

/*------------------------------------------------------------\
|                                                             |
|                         Drealenv                            |
|                                                             |
\------------------------------------------------------------*/

void Drealenv()
{
  char *Env;
  FILE *File;
  char *Buffer;

  Env = mbkgetenv( "HOME" );

  if ( Env == (char *)NULL )
  {
    DREAL_GMS_FILE_NAME = GMS_FILE_NAME;
  }
  else
  {
    DREAL_GMS_FILE_NAME =

     (char *)rdsallocblock( strlen( Env ) + strlen( GMS_FILE_NAME ) + 2 );

    strcpy( DREAL_GMS_FILE_NAME, Env );
    strcat( DREAL_GMS_FILE_NAME, "/" );
    strcat( DREAL_GMS_FILE_NAME, GMS_FILE_NAME );
  }

  Buffer = rdsallocblock( strlen( WORK_LIB ) + 32 );
  sprintf( Buffer, "%s/_ludo_%d", WORK_LIB, getpid() );

  File = fopen( Buffer, "w" );

  if ( File == (FILE *)0 )
  {
    fprintf( stdout, "Gsb: fatal error, invalid MBK_WORK_LIB '%s'\n", WORK_LIB );
    exit( 3 );
  }
  else
  {
    unlink( Buffer );
    fclose( File );
  }

  rdsfreeblock( Buffer );
}
