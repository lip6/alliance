#include <stdio.h>

#include <mut.h> 
#include <mlo.h> 
#include <rcn.h> 

void Syntax( Name )

  char *Name;
{
  fprintf( stdout, "%s [options] in out\n", Name );

  exit( 1 );
}

int main( argc, argv )

    int   argc;
    char *argv[];
{
  lofig_list     *FigureMbk;

  char           *InputName;
  char           *OutputName;
  int             Number;

  if ( argc < 2 )
  {
    Syntax( argv[ 0 ] );
  }

  mbkenv();

  if ( argc < 2 )
  {
    Syntax( argv[ 0 ] );
  }

  InputName  = (char *)NULL;
  OutputName = (char *)NULL;

  for ( Number  = 1;
        Number  < argc;
        Number++ )
  {
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

  FigureMbk = getlofig( InputName, 'A' );
  FigureMbk->NAME = namealloc( OutputName );

  savelofig( FigureMbk );

  return( 0 );
 }
