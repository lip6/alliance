/*------------------------------------------------------------\
|                                                             |
| Tool    :                   PAT2SPI                         |
|                                                             |
| File    :                   pat2spi.c                       |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   10.08.02                        |
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
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "ppt.h"

# include "pat2spi.h"

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
|                          pat_addpatfig                      |
|                                                             |
\------------------------------------------------------------*/

patfig_list *pat_addpatfig( paseq_list *Sequence )
{
  patfig_list  *FigurePat;
  char        **ModeArray;
  char        **NameArray;
  char         *FormatArray;
  long         *WidthArray;
  char         *ScanBuffer;

  char          Buffer[ 4096 ];
  char          Name  [ 512 ];
  char          Format;
  int           Left;
  int           Right;        
  char         *Direction;
  char         *Mode;

  long          NumberIO;
  long          NumberBit;
  long          IndexBit;

  char          TimeUnit;
  unsigned long TimeScale;
  unsigned long TimeDelta;
  unsigned long Delta;
  unsigned long TimeCurrent;
  unsigned long TimeNext;
  char          TimeAllZero;

  paiol_list   *ScanIol;
  pagrp_list   *ScanGrp;
  papat_list   *ScanPaPat;

  FigurePat = (patfig_list *)autallocblock( sizeof( patfig_list ) );
 
  NumberBit = Sequence->IOLNBR;
  ScanGrp   = Sequence->PAGRP;

  ScanIol = Sequence->PAIOL;

  NameArray   = (char **)autallocblock( sizeof( char * ) * NumberBit );
  FormatArray = (char  *)autallocblock( sizeof( char   ) * NumberBit );
  ModeArray   = (char **)autallocblock( sizeof( char * ) * NumberBit );
  WidthArray  = (long  *)autallocblock( sizeof( long   ) * NumberBit );

  NumberIO  = 0;
  IndexBit  = 0;

  while ( ScanIol != (paiol_list *)0 )
  {
    switch ( ScanIol->MODE)
    {
      case 'I': Mode = "in";
      break;
      case 'O':
      case 'B':
      case 'Z': Mode = "out";
      break;
      case 'R': Mode = "register";
      break;
      case 'T': Mode = "inout";
      break;
      case 'S':
      case 'W':
      case 'X':
      case 'U': Mode = "signal";
      break;
    }

    Format = ScanIol->FORMAT;

# ifdef DEBUG
    if ( ScanGrp != (pagrp_list *)0 )
    {
      fprintf( stdout, "ScanGrp %s %d %d %d\n",
          ScanGrp->NAME, ScanGrp->FINDEX, ScanGrp->LENGTH, ScanGrp->FLAG );
    }
# endif

    if ( ( ScanGrp  != (pagrp_list *)0 ) &&
         ( IndexBit == ScanGrp->FINDEX ) )
    {
      if ( Format == 'B' ) Format = 'X';
      /*
      if ( ScanIol->FORMAT != 'B' )
      */
      {
        if ( ! ScanGrp->FLAG )
        {
          sscanf( ScanIol->NAME, "%s %d", Name, &Left );

          ScanIol  += ScanGrp->LENGTH - 1;
          IndexBit += ScanGrp->LENGTH;

          sscanf( ScanIol->NAME, "%s %d", Name, &Right );

          if ( Left < Right ) Direction = "to";
          else                Direction = "downto";

          sprintf( Buffer, "%s (%d %s %d) %c", Name, Left, Direction, Right, Format );

# ifdef DEBUG
  fprintf( stdout, "ICI %s\n", Buffer );
# endif
          ScanIol = ScanIol->NEXT;
        }
        else
        {
# ifdef DEBUG
  fprintf( stdout, "NAME %s\n", ScanGrp->NAME );
# endif

          ScanBuffer = Buffer;
          strcpy( ScanBuffer, ScanGrp->NAME );
          ScanBuffer += strlen( ScanBuffer );
          strcpy( ScanBuffer, " (" );
          ScanBuffer += 2;

          for ( Left = 0; Left < ScanGrp->LENGTH; Left++ )
          {
# ifdef DEBUG
  fprintf( stdout, "IOLNAME %s\n", ScanIol->NAME );
# endif
            strcpy( ScanBuffer, ScanIol->NAME );
            ScanBuffer += strlen( ScanBuffer );

            if ( Left != (ScanGrp->LENGTH - 1) )
            {
              strcpy( ScanBuffer, ", " );
              ScanBuffer += 2;
            }

            ScanIol = ScanIol->NEXT;
          }

          strcpy( ScanBuffer, ")" );
          IndexBit += ScanGrp->LENGTH;
# ifdef DEBUG
  fprintf( stdout, "Buffer %s\n", Buffer );
# endif
        }

        NameArray  [ NumberIO ] = autnamealloc( Buffer );
        ModeArray  [ NumberIO ] = autnamealloc( Mode   );
        FormatArray[ NumberIO ] = Format;
        WidthArray [ NumberIO ] = ScanGrp->LENGTH;
        NumberIO = NumberIO + 1;

        ScanGrp = ScanGrp->NEXT;

        continue;
      }
    }

    NameArray  [ NumberIO ] = ScanIol->NAME;
    FormatArray[ NumberIO ] = Format;
    ModeArray  [ NumberIO ] = autnamealloc( Mode );
    WidthArray [ NumberIO ] = 1;
    NumberIO = NumberIO + 1;

    ScanIol  = ScanIol->NEXT;
    IndexBit = IndexBit + 1;
  }

  FigurePat->NAME            = Sequence->NAME;
  FigurePat->SEQUENCE        = Sequence;
  FigurePat->IO_NUMBER_BIT   = NumberBit;
  FigurePat->IO_NUMBER       = NumberIO;
  FigurePat->IO_NAME_ARRAY   = NameArray;
  FigurePat->IO_MODE_ARRAY   = ModeArray;
  FigurePat->IO_FORMAT_ARRAY = FormatArray;
  FigurePat->IO_WIDTH_ARRAY  = WidthArray;

  switch ( Sequence->TIME_UNIT & PAT_SEQ__TIMEUNIT )
  {
    case PAT_TU__FS : TimeScale = 1000000000;
                      TimeUnit  = PAT_TU__US;
    break;
    case PAT_TU__PS : TimeScale = 1000000000;
                      TimeUnit  = PAT_TU__MS;
    break;
    case PAT_TU__NS : TimeScale = 1000000;
                      TimeUnit  = PAT_TU__MS;
    break;
    case PAT_TU__US : TimeScale = 1000;
                      TimeUnit  = PAT_TU__MS;
    break;
    default         : TimeScale = 1;
                      TimeUnit  = Sequence->TIME_UNIT;
    break;
  }

  TimeDelta = 0;

# ifdef DEBUG
  fprintf( stdout, "PAT_TIMEUNIT %d\n", PAT_TIMEUNIT );
# endif

  if ( ( Sequence->TIME_UNIT & PAT_SEQ__TIMEUNIT ) != PAT_TU__VU )
  {
    for ( ScanPaPat  = Sequence->CURPAT;
          ScanPaPat != (papat_list *)0;
          ScanPaPat  = ScanPaPat->NEXT )
    {
# ifdef DEBUG
      fprintf( stdout, "%ld\n", ScanPaPat->TIME );
# endif
      if ( ScanPaPat->TIME != 0 ) TimeAllZero = 0;

      if ( ScanPaPat->NEXT != (papat_list *)0 )
      {
        Delta = ScanPaPat->NEXT->TIME - ScanPaPat->TIME;

        if ( ( TimeDelta == 0     ) ||
             ( TimeDelta >  Delta ) )
        {
          TimeDelta = Delta;
        }
      }

      while ( ( TimeScale                       != 1 ) &&
              ( ( ScanPaPat->TIME % TimeScale ) != 0 ) )
      {
        TimeScale /= 1000;
        TimeUnit--;
      }
    }

    TimeDelta /= TimeScale;

    if ( TimeDelta == 0 ) TimeDelta = 1;
  }
  else
  {
    TimeUnit  = Sequence->TIME_UNIT;
    TimeScale = 1;
    TimeDelta = 1;
  }

# ifdef DEBUG
  fprintf( stdout, "TIME_UNIT  %ld\n", TimeUnit );
  fprintf( stdout, "TIME_SCALE %ld\n", TimeScale );
  fprintf( stdout, "TIME_DELTA %ld\n", TimeDelta );
# endif

  FigurePat->TIME_UNIT  = TimeUnit;
  FigurePat->TIME_SCALE = TimeScale;
  FigurePat->TIME_DELTA = TimeDelta;

  return( FigurePat );
}

/*------------------------------------------------------------\
|                                                             |
|                          P2Sloadpatfig                      |
|                                                             |
\------------------------------------------------------------*/

patfig_list *P2Sloadpatfig( char *FileName )
{
  paseq_list *Sequence;

  Sequence = pat_lodpaseq( FileName, (paseq_list *)0, 0, 0 );

  if ( Sequence == (paseq_list *)0 )
  {
    autexit( 1 );
  }

  return( pat_addpatfig( Sequence ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          P2Sgetvectorname                   |
|                                                             |
\------------------------------------------------------------*/

static char *P2Sgetvectorname( char *name )
{
  char        *new_name;
  char        *blank;
  unsigned int length;
  unsigned int pos_blank;

  if ( name != (char *)0 )
  {
    blank = strchr( name, ' ' );
    if ( blank != (char *)0 )
    {
      length    = strlen( name );
      pos_blank = blank - name;
      new_name = (char *)mbkalloc( (length + 2) * sizeof(char) );
      strcpy(new_name, name);
      new_name[ pos_blank ] = '[';
      new_name[ length    ] = ']';
      new_name[ length + 1] = '\0';

      name = namealloc( new_name );
      mbkfree( new_name );
    }
  }

  return( name );
}

/*------------------------------------------------------------\
|                                                             |
|                          P2Sdrivepatfig                     |
|                                                             |
\------------------------------------------------------------*/

void P2Sdrivepatfig( patfig_list *PatFigure, char *FileName, long Slope )
{
  paseq_list    *PaSeq;
  papat_list    *PaPat;
  paevt_list    *PaEvt;
  paiol_list    *PaIol;
  char           CurrentEvent;
  char           NextEvent;
  char           Event;
  char           FirstTime;
  long           NumberBit;
  long           CurrentBit;
  char          *TimeUnitName;
  char          *IolName;
  FILE          *File;

  unsigned long  CurrentTime;
  unsigned long  NextTime;

  NumberBit = PatFigure->IO_NUMBER_BIT;
  PaSeq     = PatFigure->SEQUENCE;

  File = mbkfopen( FileName, "spi", "w" );

  if ( File == (FILE *)0 )
  {
    fprintf( stdout, "Unable to open output file %s.spi\n", FileName );
    autexit( 1 );
  }

  CurrentBit = 0;

  switch ( PaSeq->TIME_UNIT & PAT_SEQ__TIMEUNIT )
  {
    case PAT_TU__FS : TimeUnitName = "FS";
    break;
    case PAT_TU__PS : TimeUnitName = "PS";
    break;
    case PAT_TU__NS : TimeUnitName = "NS";
    break;
    case PAT_TU__US : TimeUnitName = "US";
    break;
    default         : TimeUnitName = "NS";
    break;
  }

  for ( CurrentBit = 0; CurrentBit < NumberBit; CurrentBit++ )
  {
    NextEvent    = '*';
    CurrentEvent = '*';
    FirstTime    = 1;

    PaIol = &PaSeq->PAIOL[ CurrentBit ];

    if ( ( PaIol->MODE != 'I' ) &&
	 ( PaIol->MODE != 'T' ) ) continue;

    IolName = P2Sgetvectorname( PaIol->NAME );

    if ( isvss( IolName ) ||
         isvdd( IolName ) ) continue;

    for ( PaPat  = PaSeq->CURPAT;
          PaPat != (papat_list *)0;
          PaPat  = PaPat->NEXT )
    {
      CurrentTime = PaPat->TIME;
  
      if ( PaPat->NEXT != (papat_list *)0 )
      {
        NextTime = PaPat->NEXT->TIME;
      }
      else
      {
        NextTime = CurrentTime + Slope;
      }
  
      if ( ! FirstTime )
      {
        CurrentEvent = NextEvent;
      }
  
      for ( PaEvt  = PaPat->PAEVT;
            PaEvt != (paevt_list *)0;
            PaEvt  = PaEvt->NEXT )
      {
        if ( PaEvt->INDEX != CurrentBit ) continue;

        Event = PaEvt->USRVAL;
  
        if ( Event == '-' ) Event = '0';
        else
        if ( Event == '+' ) Event = '1';
  
        NextEvent = Event; break;
      }
  
      if ( FirstTime )
      {
        FirstTime    = 0;
        CurrentEvent = NextEvent;

	fprintf( File, "vinput%ld %s 0 PWL (\n", CurrentBit, IolName );
        fprintf( File, "+ %ld%s %s %ld%s %s\n", 
	         CurrentTime    , TimeUnitName,
		 (( CurrentEvent == '0' ) ? "LOW" : "HIGH"),
		 CurrentTime + Slope, TimeUnitName,
		 (( CurrentEvent == '0' ) ? "LOW" : "HIGH") );

      }
      else
      if ( PaEvt != (paevt_list *)0 )
      {
        fprintf( File, "+ %ld%s %s %ld%s %s\n", 
	         CurrentTime    , TimeUnitName,
		 (( CurrentEvent == '0' ) ? "LOW" : "HIGH"),
		 CurrentTime + Slope, TimeUnitName,
		 (( NextEvent == '0' ) ? "LOW" : "HIGH") );
      }
    }

    if ( ! FirstTime )
    {
      fprintf( File, "+  )\n\n" );
    }
  }

  fclose( File );

  fprintf( stdout, "\t  o final time %ld ps\n", CurrentTime );
}

/*------------------------------------------------------------\
|                                                             |
|                             main                            |
|                                                             |
\------------------------------------------------------------*/

void P2Ssyntax()
{
  fprintf( stdout, "\tSyntax: pat2spi [-slope natural] in_file out_file\n" );
  autexit( 1 );
}

int main( int argc, char **argv )
{
  patfig_list *PatFigure;
  char        *InputName;
  char        *OutputName;
  long         Slope;
  long         Number;

  mbkenv();

  InputName  = (char *)0;
  OutputName = (char *)0;
  Slope      = -1;

  alliancebanner ("pat2spi", "1.0", "PAT Format Translator", "2002", ALLIANCE_VERSION);

  if ( argc < 3 ) P2Ssyntax();

  for ( Number  = 1; 
        Number  < argc;
        Number++ ) 
  {
    if ( ! strcmp( argv[ Number ], "-slope" ))
    {
      Slope = atol( argv[ ++Number ] );
      if ( Slope <= 0 ) P2Ssyntax();
    }
    else
    if ( InputName == (char *)0 ) InputName = argv[ Number ];
    else
    if ( OutputName == (char *)0 ) OutputName = argv[ Number ];
    else
    P2Ssyntax();
  }

  fprintf( stdout, "\t--> Loading pat file %s\n", InputName );
  PatFigure = P2Sloadpatfig( InputName );

  if ( Slope == -1 )
  {
    fprintf( stdout, "\t  o Use a default slope of 1000 ps\n" );
  }
  else
  {
    fprintf( stdout, "\t  o Use a user slope of %ld ps\n", Slope );
  }

  fprintf( stdout, "\t--> Driving spi file %s\n", OutputName );
  P2Sdrivepatfig( PatFigure, OutputName, Slope );

  return( 0 );
}
