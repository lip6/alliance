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
| Tool    :                     VASY                          |
|                                                             |
| File    :                  vasy_error.c                     |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   25.08.97                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "vex.h"
# include "vpn.h"
# include "vtl.h"

# include "vasy_error.h"

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
|                      Private variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      Local Display Lines                    |
|                                                             |
\------------------------------------------------------------*/

void VasyDisplayLines( VpnLine )

  vpnline_list *VpnLine;
{
  if ( VpnLine != (vpnline_list *)0 )
  {
    fprintf( stderr, " in line [" );

    while ( VpnLine != (vpnline_list *)0 )
    {
      fprintf( stderr, "%ld", VpnLine->LINE );

      if ( VpnLine->NEXT != (vpnline_list *)0 )
      {
        fprintf( stderr, "," );
      }

      VpnLine = VpnLine->NEXT;
    }

    fprintf( stderr, "]" );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyDisplayError                       |
|                                                             |
\------------------------------------------------------------*/

void VasyDisplayError( File, Line, Error, VpnLine, Text )

   char         *File;
   int           Line;
   int           Error;
   vpnline_list *VpnLine;
   char         *Text;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%d ", Name, Line );
  mbkfree( Name );

  switch( Error )
  {
    case VASY_ILLEGAL_ASSIGN_ERROR :
      fprintf( stderr, "Illegal dynamic assignation of symbol %s", Text );
    break;
    case VASY_NO_WAIT_IN_PROCESS_ERROR :
      fprintf( stderr, "No wait in process %s", Text );
    break;
    case VASY_NOT_YET_IMPLEMENTED_ERROR :
      fprintf( stderr, "%s not yet implemented", Text );
    break;
    case VASY_LOOP_IN_SIMULATION :
      fprintf( stderr, "Loop in elaborate place %s", Text );
    break;
    case VASY_ERROR_IN_SIMULATION :
      fprintf( stderr, "Elaborate error %s", Text );
    break;
    case VASY_ERROR_IN_SUPPORT :
      fprintf( stderr, "Support error bad operator %s", Text );
    break;
    case VASY_ERROR_IN_VEX2BDD :
      fprintf( stderr, "Vex2Bdd error bad operator %s", Text );
    break;
    case VASY_ERROR_IN_ABL2VEX :
      fprintf( stderr, "Abl2Vex error bad operator %s", Text );
    break;
    case VASY_ERROR_IN_DRVVEX :
      fprintf( stderr, "DrvVex error bad operator %ld", (long)Text );
    break;
    case VASY_ERROR_OPERATOR_NOT_SUPPORTED :
      fprintf( stderr, "SimpRtl error bad operator %s", Text );
    break;
    case VASY_ERROR_IN_UNROLL_LOOP :
      fprintf( stderr, "Non static range in FOR loop" );
    break;
    case VASY_LOOP_IN_REDUCTION :
      fprintf( stderr, "Loop in reduction transition %s", Text );
    break;
    case VASY_ERROR_ILLEGAL_USE_OF :
      fprintf( stderr, "Illegale use of %s", Text );
    break;
    case VASY_ERROR_CONVERT_WRONG_TYPE :
      fprintf( stderr, "Unable to convert, wrong type %s", Text );
      return;
    break;

    default : fprintf( stderr, "Internal error number %d", Error );
  }

  VasyDisplayLines( VpnLine );
  fprintf( stderr, "\n" );

  autexit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                      VasyDisplayWarning                     |
|                                                             |
\------------------------------------------------------------*/

void VasyDisplayWarning( File, Line, Warning, VpnLine, Text )

   char         *File;
   int           Line;
   int           Warning;
   vpnline_list *VpnLine;
   char         *Text;
{
  char *Name;

  Name = mbkstrdup( File );
  Name[ strlen( File ) - 1 ] = '\0';

  fprintf( stderr, "%s%d ", Name, Line );

  switch ( Warning )
  {
    case VASY_WARNING_IN_SIMULATION :
      fprintf( stderr, "Elaborate warning %s", Text );
    break;
    default : fprintf( stderr, "Internal warning number %d", Warning );
  }

  VasyDisplayLines( VpnLine );
  fprintf( stderr, "\n" );
}
