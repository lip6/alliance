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
# include <string.h>

# include <mut.h>
# include "aut.h"

/*------------------------------------------------------------\
|                                                             |
|                       Aut Sort Compare                      |
|                                                             |
\------------------------------------------------------------*/

long SortCompare( ValueArray, Index1, Index2 )

  long *ValueArray;
  long  Index1;
  long  Index2;
{
  return( ValueArray[ Index2 ] - ValueArray[ Index1 ] );
}

int main( argc, argv )

  int argc;
  char **argv;
{
  FILE *File;
  long  Value;
  char *Buffer;
  char *ScanName;
  long *ValueArray;
  long *IndexArray;
  long  SizeArray;
  long  Index;

  if ( argc < 2 ) return( 0 );

  autenv();

  if ( !strcmp( argv[1], "-sort" ) )
  {
    File = fopen( argv[2], "r" );
    
    if ( File != (FILE *)0 )
    {
      Buffer = autallocblock( 1024 );
      Value  = 0;
      Index  = -1;
    
      while ( fgets( Buffer, 1024, File ) != (char *)0 )
      {
        Value = atoi( Buffer );
  
        if ( Index == -1 )
        {
          SizeArray  = Value;
          ValueArray = (long *)autallocblock( sizeof( long ) * SizeArray );
          IndexArray = (long *)autallocblock( sizeof( long ) * SizeArray );
          Index      = 0;
        }
        else
        {
          ValueArray[ Index++ ] = Value;
        }
      }
    
      fclose( File );
  
      sortautarray( ValueArray, IndexArray, SizeArray, 0 );
  
      for ( Index = 0; Index < SizeArray; Index++ )
      {
        fprintf( stdout, "%ld\n", ValueArray[ IndexArray[ Index ] ] );
      }
    }
  }
  else
  if ( !strcmp( argv[1], "-namealloc" ) )
  {
    Buffer = autallocblock( 1024 );

    while ( fgets( Buffer, 1024, stdin ) != (char *)0 )
    {
      ScanName = strchr( Buffer, '\n' );
      if ( ScanName != (char *)0 ) *ScanName = '\0';

      ScanName = autnamealloc( Buffer );
      fprintf( stdout, "%s %lx\n", ScanName, (long)ScanName );
    }
  }

  return( 0 );
}
