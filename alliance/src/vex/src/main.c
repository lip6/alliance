/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
# include <stdio.h>
# include <stdlib.h>

# include "mut.h"
# include "aut.h"
# include "vex.h"

int main( argc, argv )

  int    argc;
  char **argv;
{
  long     Integer;
  long     Value;
  vexexpr *Vex;
  vexexpr *Atom;
  int      Width = 8;

  mbkenv();
  autenv();
  vexenv();

  if ( argc > 1 ) Width = atoi( argv[ 1 ] );

fprintf( stdout, "Width %d\n", Width );

fprintf( stdout, "width 0, signed 0\n" );
  for ( Integer = -10; Integer < 10; Integer++ )
  {
    Vex = createvexatomlong( Integer, 0, 0 );
    fprintf( stdout, "%ld -> ", Integer );
    viewvexexprboundln( Vex );

    if ( evalvexatomlong( Vex, &Value ) != -1 )
      fprintf( stdout, "<- %ld\n", Value );
    else
      fprintf( stdout, "<- ERROR\n" );

    Vex = extendvexatomsign( Vex, Width );
    if ( Vex != (vexexpr *)0 )
    {
      viewvexexprboundln( Vex );
      fprintf( stdout, "\n" );
    }
    else
      fprintf( stdout, "<- ERROR\n" );
  }

fprintf( stdout, "width 0, signed 1\n" );
  for ( Integer = -10; Integer < 10; Integer++ )
  {
    Vex = createvexatomlong( Integer, 0, 1 );
    fprintf( stdout, "%ld -> ", Integer );
    viewvexexprboundln( Vex );
    if ( evalvexatomlong( Vex, &Value ) != -1 )
      fprintf( stdout, "<- %ld\n", Value );
    else
      fprintf( stdout, "<- ERROR\n" );

    Vex = extendvexatomsign( Vex, Width );
    if ( Vex != (vexexpr *)0 )
    {
      viewvexexprboundln( Vex );
      fprintf( stdout, "\n" );
    }
    else
      fprintf( stdout, "<- ERROR\n" );
  }

fprintf( stdout, "width 6, signed 0\n" );
  for ( Integer = -10; Integer < 10; Integer++ )
  {
    Vex = createvexatomlong( Integer, 6, 0 );
    fprintf( stdout, "%ld -> ", Integer );
    viewvexexprboundln( Vex );
    if ( evalvexatomlong( Vex, &Value ) != -1 )
      fprintf( stdout, "<- %ld\n", Value );
    else
      fprintf( stdout, "<- ERROR\n" );

    Vex = extendvexatomsign( Vex, Width );
    if ( Vex != (vexexpr *)0 )
    {
      viewvexexprboundln( Vex );
      fprintf( stdout, "\n" );
    }
    else
      fprintf( stdout, "<- ERROR\n" );
  }

fprintf( stdout, "width 6, signed 1\n" );
  for ( Integer = -10; Integer < 10; Integer++ )
  {
    Vex = createvexatomlong( Integer, 6, 1 );
    fprintf( stdout, "%ld -> ", Integer );
    viewvexexprboundln( Vex );
    if ( evalvexatomlong( Vex, &Value ) != -1 )
      fprintf( stdout, "<- %ld\n", Value );
    else
      fprintf( stdout, "<- ERROR\n" );

    Vex = extendvexatomsign( Vex, Width );
    if ( Vex != (vexexpr *)0 )
    {
      viewvexexprboundln( Vex );
      fprintf( stdout, "\n" );
    }
    else
      fprintf( stdout, "<- ERROR\n" );
  }

  Atom = createvexatomvec( "v", 3, 0 );
  viewvexexprboundln( Atom );
  fprintf( stdout, "\n ici " );
  Vex = extendvexatomsign( Atom, 5 );
  viewvexexprboundln( Vex );
  fprintf( stdout, "\n" );

  Atom = createvexatomvec( "v", 3, 0 );
  SetVexNodeSigned( Atom );
  viewvexexprboundln( Atom );
  fprintf( stdout, "\n" );
  Vex = extendvexatomsign( Atom, 5 );
  viewvexexprboundln( Vex );
  fprintf( stdout, "\n" );

  return( 1 );
}
