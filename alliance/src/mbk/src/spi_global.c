#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include "spi_global.h"

#define TRUE 1

spimodel *SPIHEADMODEL = NULL;

void spiloaderror( char*, int, char* );

void spiloadmodel( void )
{
  char *env, place[80], word[80], *pt, *buffer;
  FILE *ptf;
  int line;
  spimodel *new;

  env = mbkgetenv( "MBK_SPI_MODEL" );

  if( !env )
  {
    fflush( stdout );
    fprintf( stderr, "*** ERROR *** : Variable MBK_SPI_MODEL not found.\n" );
    fprintf( stderr, "                try ""man spi""\n");
    EXIT(1);
  }

  ptf = fopen( env, "r" );
  if( !ptf )
  {
    fflush( stdout );
    fprintf( stderr, "*** ERROR *** : Can't open file %s.\n", env );
    EXIT(1);
  }
  
  line = 0; 
  while( ! feof( ptf ) )
  {
    line++;
    fgets( place, 80, ptf );
   
    /* On remplace le \n par un caractere de fin de chaine */
    buffer=place;
    while( *buffer )
    {
      if( *buffer=='\n' )
      {
        *buffer=0;
        break;
      }
      buffer++;
    }

    buffer=place;
    /* On passe les espaces en tête de fichier */
    while( *buffer==' ' && *buffer )
      buffer++;
    
    if( !*buffer )
      continue;
    if( *buffer == '#' )
      continue;

    /* On recopie le nom */
    pt=word;
    while( *buffer!=' ' && *buffer )
    {
       *pt=*buffer;
       pt++;
       buffer++;
    }
    *pt=0;

    new = mbkalloc( sizeof( spimodel ) );
    new->NEXT  = SPIHEADMODEL ;
    new->MODEL = namealloc( word );
    new->TYPE  = 0;
    new->SUBCKT  = 0;
    SPIHEADMODEL = new;

    while( *buffer==' ' && *buffer )
      buffer++;
    if( !*buffer )
      spiloaderror( env, line, "No type found" ); /*le type est obligatoire */

    /* La suite est le type, N ou P */
    
    switch( *buffer )
    {
      case 'N':
      case 'n':
        new->TYPE = new->TYPE | TRANSN ;
        break;
      case 'P':
      case 'p':
        new->TYPE = new->TYPE | TRANSP ;
        break;
      default :
        spiloaderror( env, line, "Bad type" );
    }
    buffer++;
    
    if( *buffer!=' ' && *buffer!='\0' )
      spiloaderror( env, line, "Bad type" );

    while( *buffer==' ' && *buffer )
      buffer++;

    while( *buffer )
    {
      pt=word;
      while( *buffer!=' ' && *buffer )
      {
        *pt=*buffer;
        pt++;
        buffer++;
      }
      *pt=0;

      if( strcasecmp( word, "FAST" ) == 0 )
        new->TYPE = new->TYPE | TRANSFAST;
      else
      if( strcasecmp( word, "HVIO" ) == 0 )
        new->TYPE = new->TYPE | TRANSHVIO;
      else
      if( strcasecmp( word, "SUBCKT" ) == 0 )
        new->SUBCKT = TRUE;
      else
        spiloaderror( env, line, "Unknown option" );
 
      while( *buffer==' ' && *buffer )
        buffer++;
    }
  }

  fclose( ptf );
}

void spiloaderror( char *name, int line, char *reason )
{
  fflush( stdout );
  fprintf( stderr, "*** ERROR *** bad model file %s line %d\n", name, line );
  fprintf( stderr, "Reason is : %s.\n", reason );
  EXIT(1);
}

char* spitransmodel( char type )
{
  spimodel *scan;

  if( !SPIHEADMODEL )
    spiloadmodel();

  for( scan = SPIHEADMODEL ; scan ; scan = scan->NEXT )
  {
    if( scan->TYPE == type )
      return( scan->MODEL );
  }

  fflush( stdout );
  fprintf( stderr, "*** ERROR *** No known type.\n" );
  fprintf( stderr, "Flags : %c %s %s\n",
           IsTransN(type)?'N':'P',
           IsTransFast(type)?"FAST":"---",
           IsTransHvio(type)?"HVIO":"---"
         );
  EXIT(1);

  return(0);
}

char spitranstype( char *model )
{
  spimodel *scan;

  if( !SPIHEADMODEL )
    spiloadmodel();

  for( scan = SPIHEADMODEL ; scan ; scan = scan->NEXT )
  {
    if( strcasecmp(scan->MODEL, model) == 0 )
      return( scan->TYPE );
  }

  return(SPI_UNK_TRANS_TYPE);
}

char spitranssubckt( char *model )
{
  spimodel *scan;

  if( !SPIHEADMODEL )
    spiloadmodel();

  for( scan = SPIHEADMODEL ; scan ; scan = scan->NEXT )
  {
    if( strcasecmp(scan->MODEL, model) == 0 )
      return( scan->SUBCKT );
  }

  return(SPI_UNK_TRANS_TYPE);
}
