# include <stdio.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"
# include "rwi.h"
# include "rut.h"

void Syntax( Name )

  char *Name;
{
  fprintf( stdout, "%s [options] in out\n", Name );
  fprintf( stdout, " -r : real figure\n" );
  fprintf( stdout, " -f : flatten figure\n" );
  fprintf( stdout, " -v : display figure \n" );
  fprintf( stdout, " -s : save figure \n" );

  exit( 1 );
}

int main( argc, argv )

    int   argc;
    char *argv[];
{
  rdsfig_list    *FigureRds;
  phfig_list     *FigureMbk;

  char           *InputName;
  char           *OutputName;
  char            FlagSymbolic;
  char            FlagFlatten;
  char            FlagVerbose;
  char            FlagSave;
  char            FlagParam;

  int             Number;

  if ( argc < 2 )
  {
    Syntax( argv[ 0 ] );
  }

  mbkenv();
  rdsenv();

  if ( argc < 2 )
  {
    Syntax( argv[ 0 ] );
  }

  FlagSymbolic = 1;
  FlagFlatten  = 0;
  FlagVerbose  = 0;
  FlagSave     = 0;
  FlagParam    = 0;

  rdsbegin();
  loadrdsparam();

  InputName  = (char *)NULL;
  OutputName = (char *)NULL;

  for ( Number  = 1;
        Number  < argc;
        Number++ )
  {
    if ( ! strcmp( argv[ Number ], "-r" )) FlagSymbolic = 0;
    else
    if ( ! strcmp( argv[ Number ], "-f" )) FlagFlatten = 1;
    else
    if ( ! strcmp( argv[ Number ], "-v" )) FlagVerbose = 1;
    else
    if ( ! strcmp( argv[ Number ], "-s" )) FlagSave = 1;
    else
    if ( ! strcmp( argv[ Number ], "-p" )) FlagParam = 1;
    else
    if ( InputName == NULL ) InputName = argv[ Number ];
    else
    if ( OutputName == NULL ) OutputName = argv[ Number ];
  }

  if ( InputName == NULL )
  {
    Syntax( argv[ 0 ] );
  }

  if ( OutputName == NULL )
  {
    OutputName = InputName;
  }

  if ( FlagParam )
  {
    viewrdsparam();
  }

  if ( FlagSymbolic )
  {
    fprintf( stdout, "Load figure MBK\n" );
    FigureMbk = getphfig( InputName, 'A' );

    if ( FlagFlatten )
    {
      fprintf( stdout, "Flatten figure MBK\n" );
      rflattenphfig( FigureMbk, YES, NO );
    }

    fprintf( stdout, "Translate figure MBK -> RDS\n" );
    FigureRds = figmbkrds( FigureMbk, RDS_EQUI_SIZE, 0 );

    fprintf( stdout, "Translate model MBK -> RDS\n" );
    modelmbkrds( FigureRds, 0 );
  }
  else
  {
    fprintf( stdout, "Load figure RDS\n" );
    FigureRds = getrdsfig( InputName, 'A', RDS_EQUI_SIZE  );

    if ( FlagFlatten )
    {
      fprintf( stdout, "Flatten figure RDS\n" );
      rflattenrdsfig( FigureRds, RDS_YES, RDS_NO );
    }
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "Display figure RDS\n" );
    viewrdsfig( FigureRds );
  }

  if ( FlagSave )
  {
    FigureRds->NAME = namealloc( OutputName );
    fprintf( stdout, "Save figure RDS\n" );
    saverdsfig( FigureRds );
  }

  rdsend();
  return( 0 );
 }
