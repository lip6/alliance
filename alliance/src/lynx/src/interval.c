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
| File    :                  inter.c                          |
|                                                             |
| Author  :               Jacomme Ludovic                     |
|                                                             |
| Date    :                  01.07.95                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"

# include "mbkrds.h"
# include "interval.h"
# include "error.h"

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
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                      LynxAddInterval                        |
|                                                             |
\------------------------------------------------------------*/

static lynxinter_list *LynxAddInterval( Min, Max )

  long            Min;
  long            Max;
{
  lynxinter_list *Interval;

  Interval = (lynxinter_list *)rdsallocheap( sizeof( lynxinter_list ) );
  Interval->MIN  = Min;
  Interval->MAX  = Max;

  return( Interval );
}

/*------------------------------------------------------------\
|                                                             |
|                        LynxDelInterval                      |
|                                                             |
\------------------------------------------------------------*/

static void LynxDelInterval( Interval )

  lynxinter_list *Interval;
{
  rdsfreeheap( Interval, sizeof( lynxinter_list ) );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxDelAllInterval                     |
|                                                             |
\------------------------------------------------------------*/

void LynxDelAllInterval( Interval )

  lynxinter_list *Interval;
{
  lynxinter_list *ScanInter;

  ScanInter = Interval;

  while ( ScanInter != (lynxinter_list *)0 )
  {
    Interval = ScanInter->NEXT;
    rdsfreeheap( ScanInter, sizeof( lynxinter_list ) );
    ScanInter = Interval;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       LynxViewInterval                      |
|                                                             |
\------------------------------------------------------------*/

# ifdef LYNX_DEBUG

void LynxViewInterval( Interval )

  lynxinter_list *Interval;
{
  lynxinter_list *ScanInter;

  fprintf( stdout, "-->Interval:\n" );

  for ( ScanInter  = Interval;
        ScanInter != (lynxinter_list *)0;
        ScanInter  = ScanInter->NEXT )
  {
    fprintf( stdout, "[%d,%d]",
             ScanInter->MIN * SCALE_X / RDS_LAMBDA,
             ScanInter->MAX * SCALE_X / RDS_LAMBDA );
  }

  fprintf( stdout, "<--Interval\n" );
}

# endif

/*------------------------------------------------------------\
|                                                             |
|                      LynxUnionInterval                      |
|                                                             |
\------------------------------------------------------------*/

lynxinter_list *LynxUnionInterval( Interval, Min, Max )

  lynxinter_list *Interval;
  long            Min;
  long            Max;
{
  lynxinter_list  *ScanInter;
  lynxinter_list  *NextInter;
  lynxinter_list  *NewInter;
  lynxinter_list  *DelInter;
  lynxinter_list **PrevInter;
  char             Stop;

  ScanInter =  Interval;
  PrevInter = &Interval;
  Stop      =  0;

  while ( ( ScanInter != (lynxinter_list *)0 ) &&
          ( Stop      ==                   0 ) )
  {
    if ( Max < ScanInter->MIN )
    {
      NewInter       = LynxAddInterval( Min, Max );
      NewInter->NEXT = *PrevInter;
     *PrevInter      = NewInter;

      Stop = 1;
    }
    else
    if ( Min > ScanInter->MAX )
    {
      PrevInter = &ScanInter->NEXT;
      ScanInter =  ScanInter->NEXT;
    }
    else
    if ( Max <= ScanInter->MAX )
    {
      if ( Min < ScanInter->MIN )
      {
        ScanInter->MIN = Min;
      }

      Stop = 1;
    }
    else
    {
      if ( Min < ScanInter->MIN )
      {
        ScanInter->MIN = Min;
      }

      ScanInter->MAX = Max;

      NextInter = ScanInter->NEXT;

      while ( NextInter != (lynxinter_list *)0 )
      {
        if ( NextInter->MAX <= Max )
        {
          ScanInter->NEXT = NextInter->NEXT;
          DelInter        = NextInter;
          NextInter       = NextInter->NEXT;

          LynxDelInterval( DelInter );
        }
        else
        {
          if ( NextInter->MIN <= Max )
          {
            ScanInter->MAX  = NextInter->MAX;
            ScanInter->NEXT = NextInter->NEXT;
            DelInter        = NextInter;

            LynxDelInterval( DelInter );
          }

          break;
        }
      }

      Stop = 1;
    }
  }

  if ( ! Stop )
  {
     NewInter  = LynxAddInterval( Min, Max );
    *PrevInter = NewInter;
  }

  return( Interval );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxSplitInterval                      |
|                                                             |
\------------------------------------------------------------*/

lynxinter_list *LynxSplitInterval( Interval, Min, Max )

  lynxinter_list *Interval;
  long            Min;
  long            Max;
{
  lynxinter_list  *ScanInter;
  lynxinter_list  *NewInter;
  lynxinter_list  *DelInter;
  lynxinter_list **PrevInter;
  char             Stop;

  ScanInter =  Interval;
  PrevInter = &Interval;
  Stop      =  0;

  while ( ( ScanInter != (lynxinter_list *)0 ) &&
          ( Stop      ==                   0 ) )
  {
    if ( Max < ScanInter->MIN )
    {
      Stop = 1;
    }
    else
    if ( Min > ScanInter->MAX )
    {
      PrevInter = &ScanInter->NEXT;
      ScanInter =  ScanInter->NEXT;
    }
    else
    if ( Max < ScanInter->MAX )
    {
      if ( Min > ScanInter->MIN )
      {
        NewInter = LynxAddInterval( ScanInter->MIN, Min );

       *PrevInter      = NewInter;
        NewInter->NEXT = ScanInter;
      }

      ScanInter->MIN = Max;

      Stop = 1;
    }
    else
    {
      if ( Min <= ScanInter->MIN )
      {
        *PrevInter = ScanInter->NEXT;
        DelInter   = ScanInter;
        ScanInter  = ScanInter->NEXT;

        LynxDelInterval( DelInter );
      }
      else
      {
        ScanInter->MAX = Min;

        PrevInter = &ScanInter->NEXT;
        ScanInter =  ScanInter->NEXT;
      }
    }
  }

  return( Interval );
}

/*------------------------------------------------------------\
|                                                             |
|                      LynxIntersectInterval                  |
|                                                             |
\------------------------------------------------------------*/

lynxinter_list *LynxIntersectInterval( Interval1, Interval2 )

  lynxinter_list *Interval1;
  lynxinter_list *Interval2;
{
  lynxinter_list  *ScanInter1;
  lynxinter_list  *ScanInter2;
  lynxinter_list  *Interval;
  lynxinter_list **PrevInter;
  lynxinter_list  *NewInter;
  char             Stop;
  long             Min;
  long             Max;

  PrevInter = &Interval;
  Interval  = (lynxinter_list *)0;

  ScanInter1 = Interval1;
  ScanInter2 = Interval2;

  while ( ScanInter1 != (lynxinter_list *)0 )
  {
    Stop = 0;

    while ( ( ScanInter2 != (lynxinter_list *)0 ) &&
            ( Stop       ==                   0 ) )
    {
      if ( ScanInter1->MAX <= ScanInter2->MIN )
      {
        Stop = 1;
      }
      else
      if ( ScanInter1->MIN < ScanInter2->MAX )
      {
        if ( ScanInter1->MAX > ScanInter2->MAX )
        {
          Max = ScanInter2->MAX;
        }
        else
        {
          Max  = ScanInter1->MAX;
          Stop = 1;
        }

        if ( ScanInter1->MIN > ScanInter2->MIN )
        {
          Min = ScanInter1->MIN;
        }
        else
        {
          Min = ScanInter2->MIN;
        }
       
        NewInter  =  LynxAddInterval( Min, Max );
       *PrevInter =  NewInter;
        PrevInter = &NewInter->NEXT;
      }

      if ( Stop == 0 ) ScanInter2 = ScanInter2->NEXT;
    }

    ScanInter1 = ScanInter1->NEXT;
  }

  return( Interval );
}

/*------------------------------------------------------------\
|                                                             |
|                         LynxTestInterval                    |
|                                                             |
\------------------------------------------------------------*/

# ifdef LYNX_DEBUG

void LynxTestInterval()
{
  lynxinter_list *Interval1;
  lynxinter_list *Interval2;
  lynxinter_list *Interval;
  char            Buffer[ 32 ];
  long            Min;
  long            Max;
 
  Interval  = (lynxinter_list *)0;
  Interval1 = (lynxinter_list *)0;
  Interval2 = (lynxinter_list *)0;

  fprintf( stdout, "Union Interval1\n" );

  while ( fgets( Buffer, 32, stdin ) )
  {
    if ( sscanf( Buffer, "%d %d", &Min, &Max ) != 2 ) break;

    Interval1 = LynxUnionInterval( Interval1, Min, Max );
    LynxViewInterval( Interval1 );
  }

  fprintf( stdout, "Split Interval1\n" );

  while ( fgets( Buffer, 32, stdin ) )
  {
    if ( sscanf( Buffer, "%d %d", &Min, &Max ) != 2 ) break;

    Interval1 = LynxSplitInterval( Interval1, Min, Max );
    LynxViewInterval( Interval1 );
  }

  fprintf( stdout, "Interval2\n" );

  while ( fgets( Buffer, 32, stdin ) )
  {
    if ( sscanf( Buffer, "%d %d", &Min, &Max ) != 2 ) break;

    Interval2 = LynxUnionInterval( Interval2, Min, Max );
    LynxViewInterval( Interval2 );

    fprintf( stdout, "Intersect\n" );
    Interval = LynxIntersectInterval( Interval1, Interval2 );
    LynxViewInterval( Interval );
    LynxDelAllInterval( Interval );
  }

  LynxDelAllInterval( Interval1 );
  LynxDelAllInterval( Interval2 );
}

# endif
