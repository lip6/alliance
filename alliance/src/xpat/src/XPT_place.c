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
| Tool    :                   XPAT                            |
|                                                             |
| File    :                  Place.c                          |
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
# include <math.h>
# include <signal.h>
# include <string.h>
# include <setjmp.h>
# include "mut.h"
# include "aut.h"
# include "pat.h"
# include "XSB.h" 
# include "XPT.h"
# include "XPT_place.h"
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
/*------------------------------------------------------------\
|                                                             |
|                           Place Step                        |
|                                                             |
\------------------------------------------------------------*/

  long XPAT_PATTERN_STEP_X  =  1;
  long XPAT_PATTERN_STEP_Y  =  2;

  extern char *XpatTimeUnit[ PAT_TU__MS + 1 ];

/*------------------------------------------------------------\
|                                                             |
|                           Functions                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          XpatPlaceInputOutput               |
|                                                             |
\------------------------------------------------------------*/

void XpatPlaceInputOutput()
{
  xpatobj_list *Text;
  char        **NameArray;
  long          NumberIO;
  long          Index;
  long          StepX;
  long          StepY;
  long          Y;
  long          X;

  autbegin();

  StepX = ( XPAT_UNIT * XPAT_PATTERN_STEP_X );
  StepY = ( XPAT_UNIT * XPAT_PATTERN_STEP_Y );

  NameArray = XpatFigurePat->IO_NAME_ARRAY;
  NumberIO  = XpatFigurePat->IO_NUMBER;

  X = - 2 * StepX;
  Y = StepY / 4;

  for ( Index = 0; Index < NumberIO; Index++ )
  {
    Text = XpatAddText( X, Y, NameArray[ Index ], 6 );
    Text->LINK = Text;

    SetXpatIO( Text );

    Y += StepY;
  }

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatGetBitStringHexa               |
|                                                             |
\------------------------------------------------------------*/

char *XpatGetBitStringHexa( BitString, Width )

  char *BitString;
  long  Width;
{
  char  Buffer[ 512 ];
  char  ScanChar;
  long  Index;
  long  Target;
  long  Sum;
  long  Shift;

  Target = ( ( Width - 1 ) >> 2 );
  Sum    = 0;
  Shift  = 0;

  Buffer[ 0          ] = ' ';
  Buffer[ Target + 1 ] = '\0';

  for ( Index = ( Width - 1 ); Index >= 0; Index-- )
  {
    ScanChar = BitString[ Index ];

    if ( ScanChar == '*' ) 
    {
      return( autnamealloc( "*" ) );
    }

    Sum   += ( ( ScanChar - '0' ) << Shift );
    Shift += 1;

    if ( Shift == 4 )
    {
      if ( Sum > 9 ) Buffer[ Target-- ] = Sum + 'A' - 10;
      else           Buffer[ Target-- ] = Sum + '0';

      Sum   = 0;
      Shift = 0;
    }
  }

  if ( Shift != 0 )
  {
    if ( Sum > 9 ) Buffer[ Target-- ] = Sum + 'A' - 10;
    else           Buffer[ Target-- ] = Sum + '0';
  }

  return( autnamealloc( Buffer ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatGetBitStringOctal              |
|                                                             |
\------------------------------------------------------------*/

char *XpatGetBitStringOctal( BitString, Width )

  char *BitString;
  long  Width;
{
  char  Buffer[ 512 ];
  char *ScanBuffer;
  char  ScanChar;
  long  Index;
  long  Target;
  long  Sum;
  long  Shift;

  Target = ( ( Width - 1 ) / 3 );
  Sum    = 0;
  Shift  = 0;

  Buffer[ 0          ] = ' ';
  Buffer[ Target + 2 ] = '\0';
  ScanBuffer = &Buffer[ 1 ];

  for ( Index = ( Width - 1 ); Index >= 0; Index-- )
  {
    ScanChar = BitString[ Index ];

    if ( ScanChar == '*' ) 
    {
      return( autnamealloc( "*" ) );
    }

    Sum   += ( ( ScanChar - '0' ) << Shift );
    Shift += 1;

    if ( Shift == 3 )
    {
      ScanBuffer[ Target-- ] = Sum + '0';

      Sum   = 0;
      Shift = 0;
    }
  }

  if ( Shift != 0 )
  {
    ScanBuffer[ Target-- ] = Sum + '0';
  }

  return( autnamealloc( Buffer ) );
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatPlacePattern                   |
|                                                             |
\------------------------------------------------------------*/

void XpatPlacePattern()
{
  xpatobj_list  *Object;
  xpatobj_list  *ScanObj;
  paseq_list    *PaSeq;
  papat_list    *PaPat;
  paevt_list    *PaEvt;
  long          *WidthArray;
  char          *FormatArray;
  char          *CurrentEvent;
  char          *NextEvent;
  xpatobj_list **ObjectEvent;
  char           Event;
  char          *NextString;
  char          *CurrentString;
  char          *Name;
  char           FirstTime;
  long           NumberIO;
  long           NumberBit;
  long           IndexIO;
  long           IndexBit;
  long           Width;
  long           X;
  long           Y;
  long           StepX;
  long           StepY;
  long           Height;
  long           HalfHeight;

  unsigned long  CurrentTime;
  unsigned long  NextTime;
  long           DeltaX;

  autbegin();

  NumberIO    = XpatFigurePat->IO_NUMBER;
  NumberBit   = XpatFigurePat->IO_NUMBER_BIT;
  PaSeq       = XpatFigurePat->SEQUENCE;
  WidthArray  = XpatFigurePat->IO_WIDTH_ARRAY;
  FormatArray = XpatFigurePat->IO_FORMAT_ARRAY;

  CurrentEvent = autallocblock( NumberBit + 1 );
  NextEvent    = autallocblock( NumberBit + 1 );
  ObjectEvent  = (xpatobj_list **)autallocblock( sizeof( xpatobj_list *) * NumberIO );

  memset( NextEvent, '*', NumberBit );

  XPAT_PATTERN_STEP_X = 1;
  XPAT_PATTERN_STEP_Y = 2;
  
  StepX      = ( XPAT_UNIT * XPAT_PATTERN_STEP_X );
  StepY      = ( XPAT_UNIT * XPAT_PATTERN_STEP_Y );

  HalfHeight = ( StepY / 4 );
  Height     = ( StepY / 2 );
  X          = 0;
  FirstTime  = 1;

  for ( PaPat  = PaSeq->CURPAT;
        PaPat != (papat_list *)0;
        PaPat  = PaPat->NEXT )
  {
    CurrentTime = PaPat->TIME / XpatFigurePat->TIME_SCALE;
    CurrentTime = CurrentTime / XpatFigurePat->TIME_DELTA;

    if ( PaPat->NEXT != (papat_list *)0 )
    {
      NextTime = PaPat->NEXT->TIME / XpatFigurePat->TIME_SCALE;
      NextTime = NextTime / XpatFigurePat->TIME_DELTA;
    }
    else
    {
      NextTime = CurrentTime + 1;
    }

    DeltaX = (NextTime - CurrentTime) * StepX;

    if ( DeltaX == 0 ) DeltaX = StepX;

    if ( ! FirstTime )
    {
      memcpy( CurrentEvent, NextEvent, NumberBit );
    }

    for ( PaEvt  = PaPat->PAEVT;
          PaEvt != (paevt_list *)0;
          PaEvt  = PaEvt->NEXT )
    {
      Event = PaEvt->USRVAL;

      if ( Event == '-' ) Event = '0';
      else
      if ( Event == '+' ) Event = '1';

      NextEvent[ PaEvt->INDEX ] = Event;
    }

    if ( FirstTime )
    {
      FirstTime = 0;
      memcpy( CurrentEvent, NextEvent, NumberBit );
    }

    Y        = 0;
    IndexBit = 0;
    IndexIO  = 0;

    while ( IndexBit < NumberBit )
    {
      Width = WidthArray[ IndexIO ];

      if ( Width == 1 )
      {
        Object = ObjectEvent[ IndexIO ];

        if ( ( NextEvent[ IndexBit ] == CurrentEvent[ IndexBit ] ) &&
             ( Object                != (xpatobj_list *)0        ) )
        {
          Object->DX += DeltaX;
        }
        else
        {
          switch ( NextEvent[ IndexBit ] )
          {
            case '0' : 
    
              Object = XpatAddLine( X, Y, X + DeltaX, Y, 0 );
              Name   = autnamealloc( "0" );
    
              if ( CurrentEvent[ IndexBit ] == '1' )
              {
                Object->LINK = XpatAddArrow( X, Y, X, Y + Height, 2 );
                Object->LINK->LINK = XpatAddLine( X, Y, X, Y + Height, 0 );
                Object->LINK->LINK->LINK = Object;
              }
              else
              {
                Object->LINK = Object;
              }
            break;
    
            case '1' : 
    
              Object = XpatAddLine( X, Y + Height, X + DeltaX, Y + Height, 1 );
              Name   = autnamealloc( "1" );
    
              if ( CurrentEvent[ IndexBit ] == '0' )
              {
                Object->LINK = XpatAddArrow( X, Y + Height, X, Y, 3 );
                Object->LINK->LINK = XpatAddLine( X, Y + Height, X, Y, 1 );
                Object->LINK->LINK->LINK = Object;
              }
              else
              {
                Object->LINK = Object;
              }
            break;
    
            case '*' : 
    
              Object = XpatAddRectangle( X, Y, DeltaX, Height, 4 );
              Name   = autnamealloc( "*" );
              Object->LINK = Object;

            break;
          }

          ScanObj = Object;

          do
          {
            ScanObj->NAME = Name;
            ScanObj = ScanObj->LINK;
          }
          while ( ScanObj != Object );
        }
      }
      else
      {
        Object = ObjectEvent[ IndexIO ];

        if ( FormatArray[ IndexIO ] == 'O' )
        {
          CurrentString = XpatGetBitStringOctal( CurrentEvent + IndexBit, Width );
          NextString    = XpatGetBitStringOctal( NextEvent    + IndexBit, Width );
        }
        else
        {
          CurrentString = XpatGetBitStringHexa( CurrentEvent + IndexBit, Width );
          NextString    = XpatGetBitStringHexa( NextEvent    + IndexBit, Width );
        }

        if ( ( NextString == CurrentString     ) &&
             ( Object     != (xpatobj_list *)0 ) )
        {
          Object->DX += DeltaX;
        }
        else
        {
          if ( NextString[ 0 ] == '*' )
          {
            Object = XpatAddHexagon( X, Y, DeltaX, Height, 4 );
            Object->LINK = Object;
          }
          else
          {
            Object = XpatAddHexagon( X, Y, DeltaX, Height, 5 );
            SetXpatVector( Object );
            Object->LINK = Object;
          }

          Name    = autnamealloc( NextString );
          ScanObj = Object;

          do
          {
            ScanObj->NAME = Name;
            ScanObj = ScanObj->LINK;
          }
          while ( ScanObj != Object );
        }
      }

      ObjectEvent[ IndexIO++ ] = Object;

      IndexBit += Width;
      Y        += StepY;
    }

    X += DeltaX;
  }

  autfreeblock( CurrentEvent );
  autfreeblock( NextEvent    );
  autfreeblock( ObjectEvent  );
  
  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatPlaceFigure                    |
|                                                             |
\------------------------------------------------------------*/

void XpatPlaceFigure()
{
  autbegin();

  XpatPlacePattern();
  XpatPlaceInputOutput();

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatAddCursor                      |
|                                                             |
\------------------------------------------------------------*/

xpatobj_list *XpatAddCursor( X )

  long X;
{
  xpatobj_list *Obj;
  char         *Unit;
  long          NumberIO;
  long          Y1;
  long          Y2;
  char          Buffer[ 32 ];

  autbegin();

  if ( X >= 0 ) 
  {
    NumberIO = XpatFigurePat->IO_NUMBER;
    Unit     = XpatTimeUnit[ XpatFigurePat->TIME_UNIT ];

    sprintf( Buffer, "<%d %s>", X, Unit );

    X *= XPAT_UNIT;
    Y1 = (           -1 )  * XPAT_PATTERN_STEP_Y * XPAT_UNIT;
    Y2 = ( NumberIO + 1 )  * XPAT_PATTERN_STEP_Y * XPAT_UNIT;

    Obj = XpatAddLine( X, Y1, X, Y2, 6 );
    Obj->LINK = XpatAddText( X, Y2, Buffer, 6 );
    Obj->LINK->LINK = Obj;

    SetXpatCursor( Obj );
    SetXpatCursor( Obj->LINK );
  }
  else
  {
    Obj = (xpatobj_list *)0;
  }

  return( Obj );

  autend();
}

/*------------------------------------------------------------\
|                                                             |
|                          XpatDelCursor                      |
|                                                             |
\------------------------------------------------------------*/

void XpatDelCursor( Object )

  xpatobj_list *Object;
{
  xpatobj_list *Scan;

  autbegin();

  Scan = Object;

  do
  {
    SetXpatDelete( Scan );

    Scan = Scan->LINK;
  }
  while ( Scan != Object );

  autend();
}


/*------------------------------------------------------------\
|                                                             |
|                          XpatReScaleFigure                  |
|                                                             |
\------------------------------------------------------------*/

double XpatReScaleFigure( Less )

   int Less;
{
  xpatobj_list  *ScanObj;
  long           PatternStepY;
  double         Ratio;
  int            Layer;

  autbegin();

  if ( XpatFigure == (xpatfig_list *)0 ) return( 1.0 );

  if ( Less )
  {
    PatternStepY = XPAT_PATTERN_STEP_Y >> 1;

    if ( PatternStepY < 1 )
    {
      PatternStepY = 1;
    }
  }
  else
  {
    PatternStepY = XPAT_PATTERN_STEP_Y << 1;

    if ( PatternStepY > 0x10000 )
    {
      PatternStepY = 0x10000;
    }
  }

  Ratio = (double)PatternStepY / (double)XPAT_PATTERN_STEP_Y;

  for ( Layer = 0; Layer < XPAT_MAX_LAYER; Layer++ )
  {
    for ( ScanObj  = XpatFigure->OBJECT[ Layer ];
          ScanObj != (xpatobj_list *)0;
          ScanObj  = ScanObj->NEXT )
    {
      ScanObj->Y  *= Ratio;
      ScanObj->DY *= Ratio;
    }
  }

  XPAT_PATTERN_STEP_Y = PatternStepY;

  autend();

  return( Ratio );
}
