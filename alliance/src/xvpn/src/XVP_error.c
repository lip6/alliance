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
| Tool    :                   XVPN                            |
|                                                             |
| File    :                  Error.c                          |
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
# include <signal.h>
# include <setjmp.h>
# include <unistd.h>
# include "mut.h"
# include "aut.h"
# include "vex.h"
# include "vpn.h"
# include "XSB.h" 
# include "XVP.h"
# include "XVP_error.h"

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

  static FILE   *XvpnStreamErr = (FILE *)0;
  static FILE   *XvpnStreamOut = (FILE *)0;
  static FILE   *XvpnStreamAll = (FILE *)0;

  static char    XvpnErrFileName[ 40 ];
  static char    XvpnOutFileName[ 40 ];
  static char    XvpnAllFileName[ 40 ];

  static char    XvpnNormalMode = 1;

/*------------------------------------------------------------\
|                                                             |
|                       Xvpn Error Message                    |
|                                                             |
\------------------------------------------------------------*/

  static char *XvpnErrorBuffer = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       Xvpn Informations                     |
|                                                             |
\------------------------------------------------------------*/

  static char XvpnInformationsBuffer[ XVPN_INFORMATIONS_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                  Xvpn Error Message Functions               |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                    XvpnExitErrorMessage                     |
|                                                             |
\------------------------------------------------------------*/

void XvpnExitErrorMessage( Error )

  int Error;
{
  autbegin();

  if ( XvpnStreamErr != (FILE *)0 )
  {
    fclose( XvpnStreamErr );
    unlink( XvpnErrFileName );
  }

  if ( XvpnStreamAll != (FILE *)0 )
  {
    fclose( XvpnStreamAll );
    unlink( XvpnAllFileName );
  }

  if ( ( XvpnNormalMode             ) &&
       ( XvpnStreamOut != (FILE *)0 ) )
  {
    fclose( XvpnStreamOut );
    unlink( XvpnOutFileName ); 
  }

  exit( Error );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                    XvpnInitializeErrorMessage               |
|                                                             |
\------------------------------------------------------------*/

void XvpnInitializeErrorMessage( Debug )

  char Debug;
{
  autbegin();

  XvpnNormalMode = ! Debug;

  if ( XvpnNormalMode )
  {
    sprintf( XvpnOutFileName, "/tmp/%s_out_%d", PACKAGE, (int)getpid() ); 
  }

  sprintf( XvpnErrFileName, "/tmp/%s_err_%d", PACKAGE, (int)getpid() );
  sprintf( XvpnAllFileName, "/tmp/%s_all_%d", PACKAGE, (int)getpid() );

  XvpnStreamErr = freopen( XvpnErrFileName, "w+", stderr);
  XvpnStreamAll = fopen  ( XvpnAllFileName, "w+"        );

  if ( XvpnNormalMode )
  {
    XvpnStreamOut = freopen( XvpnOutFileName, "w+", stdout);
  }
  else
  {
    XvpnStreamOut = XvpnStreamErr;
  }

  if ( ( XvpnStreamAll == (FILE *)NULL ) ||
       ( XvpnStreamOut == (FILE *)NULL ) ||
       ( XvpnStreamErr == (FILE *)NULL ) )
  {
    fprintf( stdout, "Grm: Unable to open trace window !\n");
    exit( 1 );
  }

  if ( XvpnNormalMode )
  {
    unlink( XvpnOutFileName ); 
  }

  unlink( XvpnErrFileName );

  signal( SIGINT, XvpnExitErrorMessage );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                       XvpnFlushErrorMessage                 |
|                                                             |
\------------------------------------------------------------*/

void XvpnFlushErrorMessage()
{
  int Data;

  autbegin();

  fflush( XvpnStreamErr );
  fseek( XvpnStreamErr, 0L, 0 );

  while( ( Data = fgetc( XvpnStreamErr ) ) != EOF )
  {
    fputc( Data, XvpnStreamAll );
  }

  fclose( XvpnStreamErr );

  XvpnStreamErr = freopen( XvpnErrFileName, "w+", stderr );

  if ( XvpnNormalMode )
  {
    fflush( XvpnStreamOut );
    fseek( XvpnStreamOut, 0L, 0 );

    while( ( Data = fgetc( XvpnStreamOut ) ) != EOF )
    {
      fputc( Data, XvpnStreamAll );
    } 
  
    fclose( XvpnStreamOut );

    XvpnStreamOut = freopen( XvpnOutFileName, "w+", stdout );
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnGetErrorMessage                    |
|                                                             |
\------------------------------------------------------------*/

char *XvpnGetErrorMessage()

{
  char *Message;
  long  Length;
  long  Index;
  int   Data;

  autbegin();

  XvpnFlushErrorMessage();

  fflush( XvpnStreamAll );

  Length = ftell( XvpnStreamAll );

  if ( XvpnErrorBuffer != (char *)NULL )
  {
    autfreeblock( XvpnErrorBuffer );
  }

  XvpnErrorBuffer = autallocblock( Length + 1 );
  Index            = 0;

  while ( ( ( Data = fgetc( XvpnStreamAll ) ) != EOF ) &&
          ( Index  < Length ) )
  {
    XvpnErrorBuffer[ Index++ ] = (char)Data;
  }

  rewind( XvpnStreamAll );

  XvpnErrorBuffer[ Index ] = '\0';

  if ( Index == 0 )
  {
    Message = (char *)NULL;
  }
  else
  {
    Message = XvpnErrorBuffer;
  }

  autend();
  return( Message );
}

/*------------------------------------------------------------\
|                                                             |
|                      XvpnGetInformations                    |
|                                                             |
\------------------------------------------------------------*/

char *XvpnGetInformations()

{
  vpndecl_list *VpnDeclar;
  vpnattr_list *VpnAttr;
  vpnline_list *VpnLine;
  char         *Scan;
  int           Type;

  autbegin();

  XvpnComputeBound();

  Scan = XvpnInformationsBuffer;

  if ( XvpnFigureVpn != (vpnfig_list *)NULL )
  {
    sprintf( Scan, "  FIGURE : %s\n\n  NUMBER_PLACE : %ld\n  NUMBER_TRANS : %ld\n NUMBER_ARC : %ld\n\n",
             XvpnFigureVpn->NAME, 
             GetVpnNumPlace( XvpnFigureVpn ),
             GetVpnNumTrans( XvpnFigureVpn ),
             GetVpnNumArc( XvpnFigureVpn ) );
  }
  else
  {
    sprintf( Scan, "  FIGURE : No current figure !\n\n" );
  }

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  BOUNDING BOX : \n\n  XMIN : %ld\n  YMIN : %ld\n  XMAX : %ld\n  YMAX : %ld\n\n",
           XvpnBoundXmin / XVPN_UNIT, XvpnBoundYmin / XVPN_UNIT,
           XvpnBoundXmax / XVPN_UNIT, XvpnBoundYmax / XVPN_UNIT );

  Scan = Scan + strlen( Scan );

  sprintf( Scan, "  XVPN PARAM : %s\n\n", XVPN_PARAM_NAME );
  Scan = Scan + strlen( Scan );

  if ( XvpnFigureVpn != (vpnfig_list *)0 )
  {
    sprintf( Scan, "\n  DECLAR : \n\n" );
    Scan = Scan + strlen( Scan );
  
    for ( Type = 0; Type < VPN_MAX_DECLAR_TYPE; Type++ )
    {
      for ( VpnDeclar  = XvpnFigureVpn->DECLAR[ Type ];
            VpnDeclar != (vpndecl_list *)0;
            VpnDeclar  = VpnDeclar->NEXT )
      {
        strcpy( Scan, "  LINE :" );
        Scan += 8;
  
        for ( VpnLine  = VpnDeclar->LINE;
              VpnLine != (vpnline_list *)0;
              VpnLine  = VpnLine->NEXT )
        {
          sprintf( Scan, "  [%ld]", VpnLine->LINE );
          Scan = Scan + strlen( Scan );
        }
  
        Scan = Scan + strlen( Scan );
  
        sprintf( Scan, "\n  NAME: %s\n", XvpnVex2String( VpnDeclar->VEX_ATOM ) );
        Scan = Scan + strlen( Scan );
  
        sprintf( Scan, "   - INIT: %s\n  - DIR : %s\n  - TYPE: %s\n  - KIND: %s\n  - CLASS: %s\n  - BASE: %s\n",
                 XvpnVex2String( VpnDeclar->VEX_INIT ),
                 VPN_DIR_TYPE[ VpnDeclar->DIR ],
                 VPN_DECLAR_TYPE[ VpnDeclar->TYPE ],
                 VPN_KIND_TYPE[ VpnDeclar->KIND ],
                 VPN_CLASS_TYPE[ VpnDeclar->CLASS ],
                 VEX_TYPE_NAME[ VpnDeclar->BASE ] );
        Scan = Scan + strlen( Scan );
      }
    }

    sprintf( Scan, "\n  ATTRIBUTE :\n\n" );
    Scan = Scan + strlen( Scan );
  
    for ( VpnAttr  = XvpnFigureVpn->ATTRIBUTE;
          VpnAttr != (vpnattr_list *)0;
          VpnAttr  = VpnAttr->NEXT )
    {
      strcpy( Scan, "  LINE :" );
      Scan += 8;

      for ( VpnLine  = VpnAttr->LINE;
            VpnLine != (vpnline_list *)0;
            VpnLine  = VpnLine->NEXT )
      {
        sprintf( Scan, "  [%ld]", VpnLine->LINE );
        Scan = Scan + strlen( Scan );
      }

      Scan = Scan + strlen( Scan );

      sprintf( Scan, "\n  NAME: %s\n", VpnAttr->NAME );
      Scan = Scan + strlen( Scan );

      sprintf( Scan, "   - ENTITY : %s\n  - TYPE   : %s\n  - VALUE  : %s\n",
               VpnAttr->ENTITY, 
               VPN_ATTRIBUTE_TYPE[ VpnAttr->TYPE ],
               VpnAttr->VALUE );
      Scan = Scan + strlen( Scan );
    }
  }

  autend();
  return( XvpnInformationsBuffer );
}
