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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  lynx.c                           |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                         Grégoire Avot                       |
|                        Picault Stephane     *4p*            |
|                                                             |
| Date    :                  10.11.96                         |
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
# include <signal.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "mpu.h"
# include "mlu.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"

# include "parse.h"
# include "mbkrds.h"
# include "pattern.h"
# include "cutelbow.h"
# include "extract.h"
# include "netlist.h"
# include "bulk.h"
# include "error.h"
# include "lynx.h" 

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/

# include "addlynxrcn.h"

/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

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

static rdsfig_list *LynxFigureRds;
static phfig_list  *LynxFigureMbk;
static lofig_list  *LynxLogicalFigure;
static void        *LynxFigureTrace;
static rdswindow   *LynxWindow;
static rdsrec_list *LynxHeadEqui;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Presents                          |
|                                                             |
\------------------------------------------------------------*/

void LynxPresents()

{ 
  alliancebanner_with_contrib( "Cougar", "1.21", 
                  "Netlist extractor ... formerly Lynx", "1998", 
                  ALLIANCE_VERSION,
                  "Ludovic Jacomme and Gregoire Avot",
                  "Picault Stephane" );
}

/*------------------------------------------------------------\
|                                                             |
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void LynxUsage()

{
  fprintf( stderr,
           "\n\tSyntax : cougar [-v] [-c] [ -f or -t ] [-nl] Input_name [ Output_name ]\n\n");

  fprintf( stderr,"\t  -v   Sets verbose mode on\n");
  fprintf( stderr,"\t  -r   Extracts real layout\n"); 
  fprintf( stderr,"\t  -c   Generates a core file when an error occurs\n"); 
  fprintf( stderr,"\t  -f   Flattens the layout to the catalog level before extracting\n" );
  fprintf( stderr,"\t  -t   Flattens the layout to transistors level before extracting\n" );
  fprintf( stderr,"\t  -ax  Extracts analogics parameters (only for symbolic layout)\n");
  fprintf( stderr,"\t             if x is c, capacitance are extracted\n");
  fprintf( stderr,"\t             if x is r, rc are extracted\n");
  exit( 1 ); 
}

/*------------------------------------------------------------\
|                                                             |
|                             Main                            |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv)

    int   argc;
    char *argv[];

{ 
  long  Number;

  char *Name;
  char *LynxInputName  = (char *)0;
  char *LynxOutputName = (char *)0;

  char *LynxTraceMode  = (char *)0;
  char  LynxTrace      = LYNX_TRACE_NONE;

  char CoreFloat  = LYNX_FALSE;
  char Core       = LYNX_FALSE;
  char Verbose    = LYNX_FALSE;
  char Flatten    = LYNX_FALSE;
  char Transistor = LYNX_FALSE;
  char Warning    = LYNX_FALSE;
  char Debug      = LYNX_FALSE;
  char Real       = LYNX_FALSE;
  char Rcnet      = 0;

  LynxPresents();

  if ( argc < 2 ) 
  {
    LynxUsage();
  }

  mbkenv();
  rdsenv();

  for ( Number  = 1; 
        Number  < argc;
        Number++ ) 
  {
    if ( ! strcmp( argv[ Number ], "-c" )) Core = LYNX_TRUE;
    else 
    if ( ! strcmp( argv[ Number ], "-v" )) Verbose = LYNX_TRUE;
    else 
    if ( ! strcmp( argv[ Number ], "-t" )) Transistor = LYNX_TRUE;
    else 
    if ( ! strcmp( argv[ Number ], "-f" )) Flatten = LYNX_TRUE;
    else
    if ( ! strcmp( argv[ Number ], "-r" )) Real = LYNX_TRUE;
    else
    if ( ! strcmp( argv[ Number ], "-debug" )) Debug = LYNX_TRUE;
    else
    if ( ! strcmp( argv[ Number ], "-w" )) Warning = LYNX_TRUE;
    else
    if ( ! strcmp( argv[ Number ], "-ac" )) Rcnet = 'c'; 
    else
    if ( ! strcmp( argv[ Number ], "-ar" )) Rcnet = 'r'; 
    else
    if ( LynxInputName == 0 ) LynxInputName = argv[ Number ];
    else
    if ( LynxOutputName == 0 ) LynxOutputName = argv[ Number ];
    else
    LynxError( LYNX_ERROR_ILLEGAL_OPTION, argv[ Number ], 0 );
  }

  if ( LynxInputName == 0 ) 
  {
    LynxError( LYNX_ERROR_ILLEGAL_FILENAME, argv[ Number - 1 ], 0 );
  }

  if ( LynxOutputName == 0 ) 
  {
    LynxOutputName = LynxInputName;
  }

  if ( Transistor && Flatten )
  {
    LynxError( LYNX_ERROR_ILLEGAL_COMBINATION, "-t", "-f" );
  }

  if ( Real && Rcnet )
  {
    LynxError( LYNX_ERROR_ILLEGAL_COMBINATION, "-r", "-n" );
  }

  LynxTraceMode = mbkgetenv( "LYNX_TRACE" );

  if ( LynxTraceMode != (char *)0 )
  {
    LynxTrace = LynxTraceMode[ 0 ] - '0';

    if ( LynxTrace == LYNX_TRACE_EQUI )
    {
      if ( LynxParseEqui() == LYNX_FALSE ) 
      {
        LynxTrace = LYNX_TRACE_NONE;
      }
    }

    if ( LynxTrace > LYNX_MAX_TRACE ) 
    {
      LynxTrace = LYNX_TRACE_NONE;
    }
  }

  if ( Debug ) 
  {
    rdsdebug();
  }

  rdsbegin();

  if ( Verbose )
  {
    fprintf( stdout, "\t---> Parse technological file %s\n\n" , RDS_TECHNO_NAME );
  }

  loadrdsparam();

  if ( Verbose )
  {
    fprintf( stdout, "\t\t  RDS_LAMBDA        = %ld\n", RDS_LAMBDA        );
    fprintf( stdout, "\t\t  RDS_UNIT          = %ld\n", RDS_UNIT          );
    fprintf( stdout, "\t\t  RDS_PHYSICAL_GRID = %ld\n", RDS_PHYSICAL_GRID );
    fprintf( stdout, "\t\t  MBK_SCALE_X       = %ld\n", SCALE_X           );
  }

  if ( ! Real )
  {
    LynxInitializeCutCxTable();
  }

  if ( ! Real )
  {
    fprintf( stdout, "\n\t---> Extract symbolic figure %s\n", LynxInputName );

    LynxFigureMbk = getphfig( LynxInputName, 'A' );

    if ( Flatten || Transistor ) 
    {
      if ( Verbose ) 
      {
        fprintf( stdout, "\n\t\t---> Flatten figure\n");
      }

      rflattenphfig( LynxFigureMbk, YES, ( Flatten ) ? YES : NO );
    }

    if ( Verbose ) 
    {
      fprintf( stdout, "\n\t\t---> Translate Mbk -> Rds\n\n");

    }
  
    LynxFigureRds = Lynxfigmbkrds( LynxFigureMbk );

    if ( LynxTrace == LYNX_TRACE_MBKRDS ) 
    {
      LynxFigureTrace = (void *)addphfig( "trace_mbkrds" );
      Lynxfigrdsmbk( LynxFigureRds, LynxFigureTrace );
      LynxSaveFigure( LynxFigureTrace, Real );
      LynxDelFigure( LynxFigureTrace, Real );
    }
  }
  else
  {
    fprintf( stdout, "\n\t---> Extract real figure %s\n", LynxInputName );

    LynxFigureRds = getrdsfig( LynxInputName, 'V', LYNX_MODE );

    if ( Flatten || Transistor )
    {
      if ( Verbose )
      {
        fprintf( stdout, "\n\t\t---> Flatten figure\n");
      }

      rflattenrdsfig( LynxFigureRds, RDS_YES, ( Flatten ) ? RDS_YES : RDS_NO );
    }

    Lynxfigrds( LynxFigureRds );

    if ( Verbose ) 
    {
      fprintf( stdout, "\n\t\t---> Recognize Transistors\n");
    }
    Number = LynxReconizeTransistor( LynxFigureRds );

    if ( Verbose )
    {
      fprintf( stdout, "\t\t<--- %ld\n", Number );
    }

    if ( LynxTrace == LYNX_TRACE_REAL )
    {
      LynxFigureRds->NAME = namealloc( "trace_real" );
      saverdsfig( LynxFigureRds );
    }
  }

  if ( Verbose ) 
  {
    fprintf( stdout, "\t\t---> Build windows\n");
  }
  LynxWindow = buildrdswindow( LynxFigureRds );

  if ( LynxWindow->SIZE == 0 ) 
  {
    LynxError( LYNX_ERROR_MASK_COUNTER_NULL, 0 , 0 );
  }

  if ( Verbose )
  {
    fprintf( stdout, "\t\t<--- %ld\n", LynxWindow->SIZE );
    fprintf( stdout, "\n\t\t---> Rectangles    : %ld", LynxWindow->MASK );
    fprintf( stdout, "\n\t\t---> Figure size   : ( %6ld, %6ld )",
                     LynxWindow->XMIN * SCALE_X / RDS_LAMBDA, 
                     LynxWindow->YMIN * SCALE_X / RDS_LAMBDA );
    fprintf( stdout, "\n\t\t                     ( %6ld, %6ld )\n", 
                     LynxWindow->XMAX * SCALE_X / RDS_LAMBDA, 
                     LynxWindow->YMAX * SCALE_X / RDS_LAMBDA );
  }

  LynxAddLayerImplicit( LynxFigureRds, LynxWindow);   /*4p*/

  if ( ! Real )
  {
    if ( Verbose )
    {
      fprintf( stdout, "\n\t\t---> Cut transistors\n");
    }
    Number = LynxCutTransistorElbow( LynxFigureRds, LynxWindow, Core );

    if ( Number == -1 )
    {
      if ( Core )
      {
        LynxWarning( LYNX_WARNING_CORE_FILE, "core_cx" );
      }

      LynxError( LYNX_ERROR_CUT_TRANSISTOR_ELBOW, 0, 0 );
    }

    if ( Verbose )
    {
      fprintf( stdout, "\t\t<--- %ld", Number );
    }
  }

  if ( Verbose )
  {
    fprintf( stdout, "\n\t\t---> Build equis\n");
  }
  Number = LynxBuildEqui( &LynxHeadEqui, LynxFigureRds, LynxWindow );

  if ( LynxTrace == LYNX_TRACE_CHAIN )
  {
    if ( ! Real )
    {
      LynxFigureTrace = (void *)addphfig( "trace_chain" );
    }
    else
    {
      LynxFigureTrace = (void *)addrdsfig( "trace_chain", 0 );
    }

    LynxDumpAllEqui( LynxFigureRds, LynxHeadEqui, LynxFigureTrace, Real );

    LynxSaveFigure( LynxFigureTrace, Real );
    LynxDelFigure( LynxFigureTrace, Real );
  }

  if ( Verbose )
  {
    fprintf( stdout, "\t\t<--- %ld\n", Number);
    fprintf( stdout, "\t\t---> Delete windows\n");
  }
  destroyrdswindow( LynxFigureRds, LynxWindow );

  LynxLogicalFigure = addlofig( LynxOutputName );

  if ( Verbose )
  {
    fprintf( stdout, "\t\t---> Build signals\n");
  }

  Number = LynxBuildLogicalSignal( LynxFigureRds, LynxHeadEqui, 
                                   LynxLogicalFigure, Core, Real,
                                   Rcnet, Warning );
  if ( Number < 0 )
  {
    Number    = -Number;
    CoreFloat =  LYNX_TRUE;
  }

  if ( Verbose )
  {
    fprintf( stdout, "\t\t<--- %ld\n", Number);
    fprintf( stdout, "\t\t---> Build instances\n");
  }
  Number = LynxBuildLogicalInstance( LynxFigureRds, LynxLogicalFigure );

  if ( Verbose )
  {
    fprintf( stdout, "\t\t<--- %ld\n", Number);
    fprintf( stdout, "\t\t---> Build transistors\n");
  }
  Number = LynxBuildLogicalTransistor( LynxFigureRds, LynxLogicalFigure );

/*************************************************************
* MODIFICATIONS                                              *
*************************************************************/

  lofigchain( LynxLogicalFigure );

  if( Rcnet == 'r' )
    generate_RCN(LynxLogicalFigure);

/*************************************************************
* END OF MODIFICATIONS                                       *
*************************************************************/

  if ( Verbose )
  {
    fprintf( stdout, "\t\t<--- %ld\n", Number);
    fprintf( stdout, "\t\t---> Save netlist\n\n");
  }

  delrdsfig( LynxFigureRds->NAME );

  Name = LynxVectorizeConnector( LynxLogicalFigure );

  if ( Name != (char *)0 )
  {
    LynxError( LYNX_ERROR_MISSING_CONNECTOR, Name, 0 );
  }

  savelofig( LynxLogicalFigure );

  fprintf( stdout, "\t<--- done !\n\n");

  if ( CoreFloat )
  {
    LynxWarning( LYNX_WARNING_CORE_FILE, "core_float" );
  }

  fprintf( stdout, "\t---> Total extracted capacitance\n" );
  fprintf( stdout, "\t<--- %.1fpF\n", LynxCapaInfo( LynxLogicalFigure ) );

  rdsend(); 
  return( 0 );
}
