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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Share.c                          |
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

  char GRAAL_RDS_ACTIVE_LAYER_TABLE[ RDS_ALL_LAYER ] =

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

  char GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_MAX_ACTIVE_NAME ] = 

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

  char GRAAL_FILL_MODE          = GRAAL_FILL_MODE_PATTERN;
  char GRAAL_INSTANCE_INTERFACE = GRAAL_TRUE;
  char GRAAL_CONNECTOR_INDEX    = GRAAL_FALSE;
  char GRAAL_FORCE_DISPLAY      = GRAAL_FALSE;
  char GRAAL_BLACK_BOX_STRING   = GRAAL_FALSE;
  char GRAAL_SWITCH_COLOR_MAP   = GRAAL_FALSE;

/*------------------------------------------------------------\
|                                                             |
|                       Cursor Type                           |
|                                                             |
\------------------------------------------------------------*/

  char GRAAL_XOR_CURSOR = GRAAL_FALSE;

/*------------------------------------------------------------\
|                                                             |
|                       File Name                             |
|                                                             |
\------------------------------------------------------------*/

  char *GRAAL_GMS_FILE_NAME = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       Cut Transistor Table                  |
|                                                             |
\------------------------------------------------------------*/

  static long GRAAL_CUT_C_X_PARAM[ 5 ];

  /* 0 -> Constant 0    */
  /* 1 -> Nothing to do */
  /* 2 -> D             */
  /* 3 -> ( D + d ) / 2 */
  /* 4 -> ( D - d ) / 2 */

  static long GRAAL_CUT_C_X_TABLE [ 16 ] [ 4 ] [ 4 ] =

  {
    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /*   O   */
    { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /*   O   */
    { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /*   O   */
    { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /*   O   */
    { { 0, 0, 4, 2 }, { 3, 0, 4, 2 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /* --O   */
    { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /* --O   */
    { { 0, 0, 2, 4 }, { 0, 3, 4, 4 }, { 3, 4, 4, 3 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /* --O   */
    { { 0, 0, 4, 4 }, { 3, 0, 4, 2 }, { 0, 3, 3, 4 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /* --O   */
    { { 0, 0, 4, 4 }, { 3, 0, 4, 2 }, { 0, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /*   O-- */
    { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /*   O-- */
    { { 0, 0, 4, 2 }, { 4, 0, 3, 4 }, { 3, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /*   O-- */
    { { 0, 0, 4, 2 }, { 3, 0, 4, 4 }, { 4, 3, 3, 4 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /*   O-- */
    { { 0, 0, 4, 2 }, { 3, 0, 4, 4 }, { 3, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /* --O-- */
    { { 0, 0, 2, 4 }, { 0, 3, 2, 4 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /* --O-- */
    { { 0, 0, 2, 4 }, { 0, 3, 4, 4 }, { 3, 3, 4, 4 }, { 1, 1, 1, 1 } }, /*       */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy              */
                                                                        /* --O-- */
    { { 0, 0, 4, 4 }, { 3, 0, 4, 4 }, { 0, 3, 2, 4 }, { 1, 1, 1, 1 } }, /*   |   */

    /*  X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy     X  Y  Dx Dy          |   */
                                                                        /* --O-- */
    { { 0, 0, 4, 4 }, { 3, 0, 4, 4 }, { 0, 3, 4, 4 }, { 3, 3, 4, 4 } }, /*   |   */

  };

  long *GRAAL_CUT_C_X_ARRAY[ MBK_MAX_LAYER ];
  char  GRAAL_CUT_C_X_LIST[ MBK_MAX_VIA ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      GraalPostTreatString                   |
|                                                             |
\------------------------------------------------------------*/

char *GraalPostTreatString( Text )

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
|                  GraalInitializeCutCxTable                  |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeCutCxTable()

{
  int   Index;
  long  DeltaGate;
  long  DeltaDiff;
  int   Layer;
  int   GateLayer;
  int   DiffLayer;
  long *Table;
  int   MbkLayer;
  int   TransType;
  int   CXType;

  rdsbegin();

  for ( MbkLayer = 0; MbkLayer < MBK_MAX_LAYER; MbkLayer++ )
  {
    TransType = GET_LYNX_TRANSISTOR_TYPE( MbkLayer );
    CXType    = GET_LYNX_TRANSISTOR_CX( MbkLayer );

    if ( ( TransType == RDS_LYNX_TRANSISTOR_EMPTY ) ||
         ( CXType    == RDS_LYNX_TRANSISTOR_EMPTY ) )
    {
      GRAAL_CUT_C_X_ARRAY[ MbkLayer ] = (long *)NULL;
    }
    else
    {
      Table = (long *)rdsalloc( sizeof( GRAAL_CUT_C_X_TABLE ), RDS_ALLOC_BLOCK );
      memcpy( Table, GRAAL_CUT_C_X_TABLE, sizeof( GRAAL_CUT_C_X_TABLE ) );

      GRAAL_CUT_C_X_ARRAY[ MbkLayer ] = Table;

      DeltaGate = -1;
      DeltaDiff = -1;
      Index     = 0;
      GateLayer = GET_LYNX_TRANSISTOR_GATE_LAYER( MbkLayer );
      DiffLayer = GET_LYNX_TRANSISTOR_DIFF_LAYER( MbkLayer );
      Layer     = GET_VIA_LAYER( CXType, Index );

      while ( Layer != RDS_VIA_EMPTY )
      {
        if ( Layer == GateLayer )
        {
          DeltaGate = GET_VIA_SIZE( CXType, Index );
        }
        else
        if ( Layer == DiffLayer )
        {
          DeltaDiff = GET_VIA_SIZE( CXType, Index );
        }

        Index = Index + 1;

        Layer = GET_VIA_LAYER( CXType, Index );
      }

      if ( ( DeltaGate == -1 ) ||
           ( DeltaDiff == -1 ) )
      {
        for ( Index = 0; Index < 256; Index++ )
        {
          Table[ Index ] = -1;
        }
      }
      else
      {
        /* 0 -> Constant 0    */
        /* 1 -> Nothing to do */
        /* 2 -> D             */
        /* 3 -> ( D + d ) / 2 */
        /* 4 -> ( D - d ) / 2 */

        GRAAL_CUT_C_X_PARAM[ 0 ] =  0;
        GRAAL_CUT_C_X_PARAM[ 1 ] = -1;
        GRAAL_CUT_C_X_PARAM[ 2 ] = DeltaDiff;
        GRAAL_CUT_C_X_PARAM[ 3 ] = ( DeltaDiff + DeltaGate ) >> 1;
        GRAAL_CUT_C_X_PARAM[ 4 ] = ( DeltaDiff - DeltaGate ) >> 1;

        for ( Index = 0; Index < 256; Index++ )
        {
          Table[ Index ] = GRAAL_CUT_C_X_PARAM[ Table[ Index ] ];
        }
      }
    }
  }

  for ( Index = 0; Index < MBK_MAX_VIA; Index++ )  
  {
    for ( MbkLayer = 0; MbkLayer  < MBK_MAX_LAYER; MbkLayer++ )
    {
      if ( GET_LYNX_TRANSISTOR_CX( MbkLayer ) == Index ) break;
    }

    GRAAL_CUT_C_X_LIST[ Index ] = MbkLayer;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                         Graalenv                            |
|                                                             |
\------------------------------------------------------------*/

void Graalenv()
{
  char *Env;
  FILE *File;
  char *Buffer;

  Env = mbkgetenv( "HOME" );

  if ( Env == (char *)NULL )
  {
    GRAAL_GMS_FILE_NAME = GMS_FILE_NAME;
  }
  else
  {
    GRAAL_GMS_FILE_NAME =

     (char *)rdsallocblock( strlen( Env ) + strlen( GMS_FILE_NAME ) + 2 );

    strcpy( GRAAL_GMS_FILE_NAME, Env );
    strcat( GRAAL_GMS_FILE_NAME, "/" );
    strcat( GRAAL_GMS_FILE_NAME, GMS_FILE_NAME );
  }

  Buffer = rdsallocblock( strlen( WORK_LIB ) + 32 );
  sprintf( Buffer, "%s/_ludo_%ld", WORK_LIB, getpid() );

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
