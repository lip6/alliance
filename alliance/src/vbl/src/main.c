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
# include "vex.h"
# include "vbh.h"
# include "vbl.h"

# ifdef VBL_DEBUG
  extern int vbl_y_debug;
# endif

/*------------------------------------------------------------\
|                                                             |
|                            Usage                            |
|                                                             |
\------------------------------------------------------------*/

void VblUsage()
{
  fprintf( stderr, "\t\tvbltest [Options] Input_name\n\n" );
  fprintf( stdout, "\t\tOptions : -V Sets Verbose mode on\n" );
# ifdef VBL_DEBUG
  fprintf( stdout, "\t\t          -D        Sets Debug mode on\n" );
# endif
  fprintf( stdout, "\t\t          -G        Load generic instances\n" );
  fprintf( stdout, "\t\t          -S        Simplify Vex\n" );
  fprintf( stdout, "\t\t          -P File   packages list file name\n" );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

  int    argc;
  char **argv;
{
  char       *InputFileName;
  char       *PackageFileName;
  int         Number;
  int         Index;
  char        Option;
  vbfig_list *VbhFigure;
  vbfig_list *VbhGenFigure;
  vbins_list *VbhInstance;
  char       *Extention;

  int   FlagVerbose = 0;
  int   FlagGeneric = 0;
# ifdef VBL_DEBUG
  int   FlagDebug   = 0;
# endif
  int   FlagSimplif = 0;

  mbkenv();
  autenv();
  vexenv();

  InputFileName   = (char *)0;
  PackageFileName = (char *)0;
  Extention       = "vhdl";

  if ( argc < 2 ) VblUsage();

  InputFileName = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        if ( Option == 'I' )
        {
          Number++;

          if ( Number < argc ) Extention = argv[ Number ];
          else                 VblUsage();

          break;
        }
        else
        if ( Option == 'P' )
        {
          Number++;

          if ( Number < argc ) PackageFileName = argv[ Number ];
          else                 VblUsage();

          break;
        }

        switch ( Option )
        {
          case 'V' : FlagVerbose = 1;
          break;
# ifdef VBL_DEBUG
          case 'D' : FlagDebug = 1;
          break;
# endif
          case 'S' : FlagSimplif = 1;
          break;
          case 'G' : FlagGeneric = 1;
          break;
          default  : VblUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    VblUsage();
  }

  if ( InputFileName == (char *)0 ) VblUsage();

  if ( PackageFileName != (char *)0 )
  {
    getvbpkg( PackageFileName, Extention );
  }

# ifdef VBL_DEBUG
  if ( FlagDebug ) vbl_y_debug = 1;
# endif

  VbhFigure = getvbfig( InputFileName, Extention );

  if ( ! FlagGeneric )
  {
    if ( FlagSimplif ) vbh_simpvbfig( VbhFigure );
    if ( FlagVerbose ) vbh_viewvbfig( VbhFigure );

    vbh_frevbfig( VbhFigure );
  }
  else
  {
    for ( VbhInstance  = VbhFigure->BEINS;
          VbhInstance != (vbins_list *)0;
          VbhInstance  = VbhInstance->NEXT )
    {
      if ( VbhInstance->GEN_MAP != (vbmap_list *)0 )
      {
        VbhGenFigure = getvbfiggenmap( VbhInstance->MODEL, Extention, VbhInstance->GEN_MAP );
      }
    }

    for ( VbhFigure  = VBL_HEADFIG;
          VbhFigure != (vbfig_list *)0;
          VbhFigure  = VbhFigure->NEXT )
    {
      if ( FlagSimplif ) vbh_simpvbfig( VbhFigure );
      if ( FlagVerbose ) vbh_viewvbfig( VbhFigure );

    }

    vbh_frevbfig( VBL_HEADFIG );
  }


  return( 0 );
}
