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
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     VBL                           |
|                                                             |
| File    :                  vbl_parse.c                      |
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
# include "vex.h"
# include "vbh.h"
# include "vbl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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

  extern FILE       *vbl_y_in;
  extern int         vbl_y_parse();
  extern char        VBL_ERRFLG;
  extern vbfig_list *VBL_HEADFIG;
  extern vbmap_list *VBL_GENMAP;
  extern char       *VBL_FILENAME;

/*------------------------------------------------------------\
|                                                             |
|                        vpnloadvpnfig                        |
|                                                             |
\------------------------------------------------------------*/

vbfig_list *getvbfiggenmap( InputFileName, Extention, GenericMap  )

  char       *InputFileName;
  char       *Extention;
  vbmap_list *GenericMap;
{
  int   Value;
  int   Index;

static  char *AllianceExtention[] = { "vst", "vbe", "fsm" };

  if ( ! strcmp( Extention, "vst" ) )
  {
    Index = 0;

    for ( Index = 0; Index < 3; Index++ )
    {
      vbl_y_in = mbkfopen( InputFileName, AllianceExtention[ Index ], "r" );

      if ( vbl_y_in != (FILE *)0 ) break;
    }
  }
  else
  {
    vbl_y_in = mbkfopen( InputFileName, Extention, "r" );
  }

  if ( vbl_y_in == (FILE *)0 )
  {
    fprintf( stdout, "Error opening file %s\n", InputFileName );
    autexit( 1 );
  }

  VBL_GENMAP   = GenericMap;
  VBL_FILENAME = InputFileName;

  Value = vbl_y_parse();

  fclose( vbl_y_in );

  if ( ( Value      ) ||
       ( VBL_ERRFLG ) )
  {
    fprintf( stdout, "Error parsing file %s\n", InputFileName );
    autexit( 1 );
  }

  VBL_HEADFIG->BEGEN = (vbgen_list *)reverse( (chain_list *)VBL_HEADFIG->BEGEN );
  VBL_HEADFIG->BECST = (vbcst_list *)reverse( (chain_list *)VBL_HEADFIG->BECST );

  return( VBL_HEADFIG );
}

/*------------------------------------------------------------\
|                                                             |
|                           getvbfig                          |
|                                                             |
\------------------------------------------------------------*/

vbfig_list *getvbfig( InputFileName, Extention )

  char  *InputFileName;
  char  *Extention;
{
  return( getvbfiggenmap( InputFileName, Extention, (vbmap_list *)0 ) );
}

/*------------------------------------------------------------\
|                                                             |
|                           getvbpkg                          |
|                                                             |
\------------------------------------------------------------*/

void getvbpkg( FileName, Extention )

  char  *FileName;
  char  *Extention;
{
  vbfig_list *VbhFigure;
  FILE       *File;
  char        Buffer[ 512 ];
  char        LogicalName[ 512 ];
  char        PhysicalName[ 512 ];
  long        LineNumber;

  if ( FileName == (char *)0 ) return;

  File = mbkfopen( FileName, "pkg",  "r" );

  if ( File != (FILE *)0 )
  {
    LineNumber = 0;

    while ( fgets( Buffer, 512, File ) != (char *)0 )
    {
      LineNumber++;

      if ( Buffer[ 0 ] == '#' ) continue;

      if ( sscanf( Buffer, "%s : %s\n", LogicalName, PhysicalName ) == 2 )
      {
        VbhFigure = getvbfiggenmap( PhysicalName, Extention, (vbmap_list *)0 );

        if ( ! VbhFigure->IS_PACKAGE )
        {
          fprintf( stdout, "ERROR file %s.pkg is not a package\n", PhysicalName );
          autexit( 1 );
        }

        VbhFigure->NAME = namealloc( LogicalName );
      }
      else
      {
        fprintf( stdout, "ERROR syntax error line %ld, in file %s\n", LineNumber, FileName );
        autexit( 1 );
      }
    }
  }
  else
  {
    fprintf( stdout, "Error opening package file list %s.pkg\n", FileName );
    autexit( 1 );
  }
}
