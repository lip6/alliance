# include <stdio.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"
# include "rwi.h"
# include "rut.h"

rdsrec_list *reverseequi( HeadEqui )

  rdsrec_list *HeadEqui;
{
  rdsrec_list   *PrevEqui;
  rdsrec_list   *NextEqui;
  rdsrec_list   *ScanEqui;
  unsigned char  EndEqui;

  PrevEqui = (rdsrec_list *)NULL;
  ScanEqui = HeadEqui;
  EndEqui  = 1;

  while ( ScanEqui != (rdsrec_list *)NULL )
  {
    NextEqui = RDS_EQUI( ScanEqui );
    RDS_EQUI( ScanEqui ) = PrevEqui;
    PrevEqui = ScanEqui;

    if ( IsRdsEndEqui( ScanEqui ) )
    {
      if ( ! EndEqui )
      {
        ClearRdsEndEqui( ScanEqui );
        EndEqui = 1;
      }
    }
    else
    if ( EndEqui )
    {
      SetRdsEndEqui( ScanEqui );
      EndEqui = 0;
    }

    ScanEqui = NextEqui;
  }

  return( PrevEqui );
}

void viewrdsequi( HeadEqui )

  rdsrec_list *HeadEqui;
{
  rdsrec_list *ScanEqui;

  for ( ScanEqui  = HeadEqui;
        ScanEqui != (rdsrec_list *)NULL;
        ScanEqui  = RDS_EQUI( ScanEqui ) )
  {
    viewrdsrec( ScanEqui );

    if ( IsRdsEndEqui( ScanEqui ) )
    {
      fprintf( stdout, "EndEqui\n" );
    }
  }
}

void Syntax( Name )

  char *Name;
{
  fprintf( stdout, "%s [-r -f -R -u] in out\n", Name );
  fprintf( stdout, " -r : real figure\n" );
  fprintf( stdout, " -f : flatten figure\n" );
  fprintf( stdout, " -R : reverse equi\n" );
  fprintf( stdout, " -V : display figure \n" );
  fprintf( stdout, " -S : save figure \n" );
  fprintf( stdout, " -u : unify\n" );

  exit( 1 );
}

int main( argc, argv )

    int   argc;
    char *argv[];
{
  rdsfig_list    *FigureRds;
  phfig_list     *FigureMbk;
  rdswindow      *WindowRds;
  rdsrec_list    *HeadEqui;

  char           *InputName;
  char           *OutputName;
  char            FlagSymbolic;
  char            FlagFlatten;
  char            FlagReverse;
  char            FlagUnify;
  char            FlagDebug;
  char            FlagVerbose;
  char            FlagSave;

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
  FlagReverse  = 0;
  FlagUnify    = 0;
  FlagDebug    = 0;
  FlagVerbose  = 0;
  FlagSave     = 0;

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
    if ( ! strcmp( argv[ Number ], "-R" )) FlagReverse = 1;
    else
    if ( ! strcmp( argv[ Number ], "-u" )) FlagUnify = 1;
    else
    if ( ! strcmp( argv[ Number ], "-debug" )) FlagDebug = 1;
    else
    if ( ! strcmp( argv[ Number ], "-R" )) FlagReverse = 1;
    else
    if ( ! strcmp( argv[ Number ], "-V" )) FlagVerbose = 1;
    else
    if ( ! strcmp( argv[ Number ], "-S" )) FlagSave = 1;
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

  if ( FlagDebug )
  {
    rdsdebug();
  }

  rdsbegin();
  loadrdsparam();

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

  if ( FlagSave )
  {
    fprintf( stdout, "Save figure RDS in prout\n" );
    FigureRds->NAME = namealloc( "prout" );
    saverdsfig( FigureRds );
  }

  if ( FlagVerbose )
  {
    viewrdsfig( FigureRds );
  }
  
  fprintf( stdout, "Build window RDS\n" );
  WindowRds = buildrdswindow( FigureRds );
  fprintf( stdout, "Build equi\n" );
  HeadEqui  = equirdsfig( FigureRds, WindowRds );

  fprintf( stdout, "Destroy window RDS\n" );
  destroyrdswindow( FigureRds, WindowRds );

  if ( FlagReverse )
  {
    fprintf( stdout, "Reverse equi\n" );
    HeadEqui = reverseequi( HeadEqui );
  }

  if ( FlagUnify )
  {
    fprintf( stdout, "Unify figure RDS\n" );
    unifyrdsfig( FigureRds, HeadEqui );
  }

  FigureRds->NAME = namealloc( OutputName );
  fprintf( stdout, "Save figure RDS\n" );
  saverdsfig( FigureRds );

  rdsend();
  return( 0 );
 }
