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
# include <stdio.h>
# include <malloc.h>

# include "mut.h"
# include "aut.h"
# include "gcp.h"

/*------------------------------------------------------------\
|                                                             |
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void CUsage()
{
  fprintf( stderr, "\t\tgcptest --[Options] Input_name_1 ... Input_name_N -[GCC Options]\n\n" );
  fprintf( stdout, "\t\tOptions : --V Sets Verbose mode on\n" );
  fprintf( stdout, "\t\t          --D Sets Debug mode on\n" );
  fprintf( stdout, "\t\t          --F Sets Force display\n" );
  fprintf( stdout, "\t\t          --T Sets display Types\n" );
  fprintf( stdout, "\t\t          --S Sets display Sizes\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern int c_y_debug;

int main( argc, argv )

  int    argc;
  char **argv;
{
  c_tree_node *TreeNode;
  char        *InputFileName[ 10 ];
  int          Last;
  int          Number;
  int          Index;
  char         Option;
  char        *Extention;
  char       **Argv;
  int          Argc;

  int         FlagVerbose = 0;
  int         FlagDebug   = 0;
  int         NoMoreFileName = 0;

  mbkenv();
  autenv();

  if ( argc < 2 ) CUsage();

  InputFileName[ 0 ] = (char *)0;
  Last               = 0;
  Argc               = 0;
  Argv               = (char **)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      if ( argv[ Number ][ 1 ] == '-' )
      {
        for ( Index = 2; argv[ Number ][ Index ] != '\0'; Index++ )
        {
          Option = argv[ Number ][ Index ];
  
          if ( Option == 'I' )
          {
            Number++;
  
            if ( Number < argc ) Extention = argv[ Number ];
            else                 CUsage();
  
            break;
          }
  
          switch ( Option )
          {
            case 'V' : FlagVerbose = 1;
            break;
            case 'D' : FlagDebug  |= 1;
            break;
            case 'F' : FlagDebug  |= 2;
            break;
            case 'T' : FlagDebug  |= 4;
            break;
            case 'S' : FlagDebug  |= 8;
            break;
            default  : CUsage();
          }
        }
      }
      else
      if ( ! NoMoreFileName )
      {
        NoMoreFileName = 1;
        Argv = &argv[ Number ];
        Argc = argc - Number;
      }
    }
    else
    if ( ! NoMoreFileName )
    {
      InputFileName[ Last ] = argv[ Number ];
      Last++;

      if ( Last >= 10 ) NoMoreFileName = 1;
      else              InputFileName[ Last ] = (char *)0;
    }
    else
    CUsage();
  }

  if ( InputFileName[ 0 ] == (char *)0 ) CUsage();

  if ( FlagDebug & 1 )
  {
    c_y_debug = 1;
  }

  if ( FlagDebug & 2 )
  {
    C_TREE_FORCE_VIEW_NODE = 1;
  }

  if ( FlagDebug & 4 )
  {
    C_TREE_VIEW_TYPE_NODE = 1;
  }

  if ( FlagDebug & 8 )
  {
    C_TREE_VIEW_SIZE_NODE = 1;
  }

  for ( Index = 0; Index < Last; Index++ )
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "--> Processing file %s\n", InputFileName[ Index ] );
    }

    TreeNode = c_tree_parse_file( InputFileName[ Index ], Argc, Argv );

    if ( FlagVerbose )
    {
      fprintf( stdout, "<-- done\n\n" );
      c_tree_view_node( TreeNode );
    }
  }

  c_tree_free_all_node();

  return( 0 );
}
