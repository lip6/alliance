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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                   Pat.c                           |
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
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "ppt.h"
# include "XSB.h" 
# include "XPT.h"
# include "XPT_pat.h"

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

  patfig_list *XpatFigurePat = (patfig_list *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       For Set Long Jump                     |
|                                                             |
\------------------------------------------------------------*/

  static jmp_buf XpatJumpBuffer;

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                 Xpat Exit Handler Functions                 |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      XpatExitHandler                        |
|                                                             |
\------------------------------------------------------------*/

void XpatExitHandler()
{
  XpatFlushErrorMessage();

  longjmp( XpatJumpBuffer, 1);
}

/*------------------------------------------------------------\
|                                                             |
|                      XpatActiveExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XpatActiveExitHandler()
{
  MBK_EXIT_FUNCTION = XpatExitHandler;
}

/*------------------------------------------------------------\
|                                                             |
|                       XpatResetExitHandler                  |
|                                                             |
\------------------------------------------------------------*/

void XpatResetExitHandler()
{
  MBK_EXIT_FUNCTION = NULL;
}

/*------------------------------------------------------------\
|                                                             |
|                          Get Functions                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          pat_frepaseq                       |
|                                                             |
\------------------------------------------------------------*/

static void pat_frepaseq( Sequence )

  paseq_list *Sequence;
{
  pagrp_list *PaGrp;
  pains_list *PaIns;

  mbkfree( Sequence->PAIOL );

  pat_frepapat( Sequence->OLDPAT );
  pat_frepapat( Sequence->CURPAT );
  pat_frepapat( Sequence->NXTPAT );

  pat_frepacom( Sequence->DECCOM );
  pat_frepacom( Sequence->OLDCOM );
  pat_frepacom( Sequence->CURCOM );

  while ( Sequence->PAGRP != (pagrp_list *)0 )
  {
    PaGrp = Sequence->PAGRP;
    Sequence->PAGRP = PaGrp->NEXT;

    mbkfree( PaGrp );
  }

  while ( Sequence->PAINS != (pains_list *)0 )
  {
    PaIns = Sequence->PAINS;
    Sequence->PAINS = PaIns->NEXT;

    mbkfree( PaIns );
  }

  mbkfree( Sequence );
}

/*------------------------------------------------------------\
|                                                             |
|                          pat_delpatfig                      |
|                                                             |
\------------------------------------------------------------*/

void pat_delpatfig( FigurePat )

  patfig_list *FigurePat;
{
  pat_frepaseq( FigurePat->SEQUENCE );

  autfreeblock( FigurePat->IO_NAME_ARRAY   );
  autfreeblock( FigurePat->IO_FORMAT_ARRAY );
  autfreeblock( FigurePat->IO_MODE_ARRAY   );
  autfreeblock( FigurePat->IO_WIDTH_ARRAY  );

  autfreeblock( FigurePat );
}

/*------------------------------------------------------------\
|                                                             |
|                          pat_addpatfig                      |
|                                                             |
\------------------------------------------------------------*/

patfig_list *pat_addpatfig( Sequence )

  paseq_list *Sequence;
{
  patfig_list  *FigurePat;
  char        **ModeArray;
  char        **NameArray;
  char         *FormatArray;
  long         *WidthArray;
  char         *ScanBuffer;

  char          Buffer[ 4096 ];
  char          Name  [ 256 ];
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

  if ( Sequence == (paseq_list *)0 )
  {
    return( (patfig_list *)0 );
  }

  if ( Sequence->ERRFLG != 0 )
  {
    pat_frepaseq( Sequence );

    return( (patfig_list *)0 );
  }

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
|                          Xpatlodpaseq                       |
|                                                             |
\------------------------------------------------------------*/

patfig_list *Xpatloadpatfig( FileName )

  char *FileName;
{
  paseq_list *Sequence;

  autbegin();

  Sequence = (paseq_list *)NULL;

  XpatActiveExitHandler();

  if ( setjmp( XpatJumpBuffer ) == 0 )
  {
    Sequence = pat_lodpaseq( FileName, (paseq_list *)0, 0, 0 );

    XpatResetExitHandler();

    autend();
    return( pat_addpatfig( Sequence ) );
  }

  XpatResetExitHandler();

  if ( Sequence != (paseq_list *)NULL )
  {
    pat_frepaseq( Sequence );
  }

  autend();
  return( (patfig_list *)NULL );
}
