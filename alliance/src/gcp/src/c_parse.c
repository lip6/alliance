/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VBL                           |
|                                                             |
| File    :                  c_parse.c                        |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   01.01.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "gcp.h"

# include <unistd.h>
# include <sys/stat.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "c_tree.h"
# include "c_cdecl.h"
# include "c_clex.h"
# include "c_debug.h"
# include "c_flags.h"

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

  extern FILE        *c_y_in;
  extern int          c_y_parse();

  static char         BuffTmpFile[ 64 ];
  static int          UnlinkTmpFile = 0;

void gcpexit( Code )

   int Code;
{
  if ( UnlinkTmpFile )
  {
    unlink( BuffTmpFile );
  }

  autexit( Code );
}

/*------------------------------------------------------------\
|                                                             |
|                         c_tree_parse_file                   |
|                                                             |
\------------------------------------------------------------*/

c_tree_node *c_tree_parse_file( InputFileName, Argc, Argv )

  char  *InputFileName;
  int    Argc;
  char **Argv;
{
  struct stat Stat;
  char        BuffExec[ 2048 ];
  char       *ScanArg;
  int         Index;
  int         Error;

  if ( stat( InputFileName, &Stat ) )
  {
    fprintf( stdout, "Error unable to open file %s\n", InputFileName );
    gcpexit( 1 );
  }

  sprintf( BuffTmpFile, "/tmp/gcp%d", getpid() );

  ScanArg = BuffExec;
  sprintf( ScanArg, "gcc -E %s ", InputFileName );
  ScanArg += strlen( ScanArg );

  for ( Index = 0; Index < Argc; Index++ )
  {
    strcpy( ScanArg, "'" );
    ScanArg += 1;
    strcpy( ScanArg, Argv[ Index ] );
    ScanArg += strlen( ScanArg );
    strcpy( ScanArg, "' " );
    ScanArg += 2;
  }

  sprintf( ScanArg, "-o %s", BuffTmpFile );

  UnlinkTmpFile = 1;

  errno = 0;
  Error = system( BuffExec );

  if ( ( Error       ) &&
       ( Error != -1 ) )
  {
    fprintf( stdout,
        "An error occured while preprocessing %s, exit code %d (errno %d)\n",
        InputFileName, Error, errno );
    gcpexit( 2 );
  }

  c_y_in = fopen( BuffTmpFile, "r" );

  if ( c_y_in == (FILE *)0 )
  {
    fprintf( stdout, "Error unable to open file %s\n", BuffTmpFile );
    gcpexit( 3 );
  }

  c_parse_flags( Argc, Argv );

  c_init_lex( InputFileName );
  c_init_decl_processing();

  Error = c_y_parse();

  fclose( c_y_in );

  unlink( BuffTmpFile );
  UnlinkTmpFile = 0;

  return( c_tree_program_node );
}
