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
| Tool    :                     FBH                           |
|                                                             |
| File    :                   fbh_view.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   09.11.99                        |
|                                                             |
\------------------------------------------------------------*/

#include "mut.h" 
#include "aut.h"
#include "abl.h" 
#include "bdd.h"
#include "fvh.h"

static void loc_fbh_viewablexprln( Expr )

  ablexpr *Expr;
{
  if ( Expr == (ablexpr *)0 )
  {
    fprintf( stdout, "NULL\n" );
  }
  else
  {
    viewablexprln( Expr, ABL_VIEW_VHDL );
  }
}

void fbh_viewbiabl( BiAbl )

  fbbiabl_list *BiAbl;
{
  if ( BiAbl == (fbbiabl_list *)0 )
  {
    fprintf( stdout, "NULL\n" );
  }
  else
  {
    while ( BiAbl != (fbbiabl_list *)0 )
    {
      fprintf( stdout, "--> fbbiabl_list\n" );
      fprintf( stdout, "    LABEL  : %s\n", BiAbl->LABEL );
      fprintf( stdout, "    CNDABL : " );
      loc_fbh_viewablexprln( BiAbl->CNDABL );
      fprintf( stdout, "    VALABL : " );
      loc_fbh_viewablexprln( BiAbl->VALABL );
      fprintf( stdout, "<-- fbbiabl_list\n" );

      BiAbl = BiAbl->NEXT;
    }
  }
}

void fbh_viewfbreg( ScanReg )

  fbreg_list *ScanReg; 
{
  if ( ScanReg != (fbreg_list *)0 )
  {
    fprintf( stdout, "--> fbreg_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanReg->NAME      );
    fprintf( stdout, "    FLAG  : %d\n", (int)ScanReg->FLAG );
    fprintf( stdout, "    BIABL :\n" );
    fbh_viewbiabl( ScanReg->BIABL );
    fprintf( stdout, "<-- fbreg_list\n" );
  }
}

void fbh_viewfbbux( ScanBux )

  fbbux_list *ScanBux; 
{
  if ( ScanBux != (fbbux_list *)0 )
  {
    fprintf( stdout, "--> fbbux_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanBux->NAME      );
    fprintf( stdout, "    TYPE  : %d\n", (int)ScanBux->TYPE );
    fprintf( stdout, "    FLAG  : %d\n", (int)ScanBux->FLAG );
    fprintf( stdout, "    BIABL :\n" );
    fbh_viewbiabl( ScanBux->BIABL );
    fprintf( stdout, "<-- fbbux_list\n" );
  }
}

void fbh_viewfbbus( ScanBus )

  fbbus_list *ScanBus; 
{
  if ( ScanBus != (fbbus_list *)0 )
  {
    fprintf( stdout, "--> fbbus_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanBus->NAME      );
    fprintf( stdout, "    TYPE  : %d\n", (int)ScanBus->TYPE );
    fprintf( stdout, "    FLAG  : %d\n", (int)ScanBus->FLAG );
    fprintf( stdout, "    BIABL :\n" );
    fbh_viewbiabl( ScanBus->BIABL );
    fprintf( stdout, "<-- fbbus_list\n" );
  }
}

void fbh_viewfbaux( ScanAux )

  fbaux_list *ScanAux; 
{
  if ( ScanAux != (fbaux_list *)0 )
  {
    fprintf( stdout, "--> fbaux_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanAux->NAME      );
    fprintf( stdout, "    TYPE : %d\n", (int)ScanAux->TYPE );
    fprintf( stdout, "    FLAG : %d\n", (int)ScanAux->FLAG );
    fprintf( stdout, "    ABL  :" );
    loc_fbh_viewablexprln( ScanAux->ABL, ABL_VIEW_VHDL );
    fprintf( stdout, "<-- fbaux_list\n" );
  }
}

void fbh_viewfbout( ScanOut )

  fbout_list *ScanOut; 
{
  if ( ScanOut != (fbout_list *)0 )
  {
    fprintf( stdout, "--> fbout_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanOut->NAME      );
    fprintf( stdout, "    TYPE : %d\n", (int)ScanOut->TYPE );
    fprintf( stdout, "    FLAG : %d\n", (int)ScanOut->FLAG );
    fprintf( stdout, "    ABL  :" );
    loc_fbh_viewablexprln( ScanOut->ABL );
    fprintf( stdout, "<-- fbout_list\n" );
  }
}

void fbh_viewfbpor( ScanPort )

  fbpor_list *ScanPort; 
{
  if ( ScanPort != (fbpor_list *)0 )
  {
    fprintf( stdout, "--> fbpor_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanPort->NAME      );
    fprintf( stdout, "    DIR  : %c\n", ScanPort->DIRECTION );
    fprintf( stdout, "    TYPE : %d\n", (int)ScanPort->TYPE );
    fprintf( stdout, "<-- fbpor_list\n" );
  }
}

void fbh_viewfbfig( ScanFigure )

  fbfig_list *ScanFigure; 
{
  fbpor_list *ScanPort;
  fbaux_list *ScanAux;
  fbbus_list *ScanBus;
  fbbux_list *ScanBux;
  fbreg_list *ScanReg;
  fbout_list *ScanOut;

  if ( ScanFigure != (fbfig_list *)0 )
  {
    fprintf( stdout, "--> fbfig_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanFigure->NAME );

    fprintf( stdout, "    BEPOR:\n" );

    for ( ScanPort  = ScanFigure->BEPOR;
          ScanPort != (fbpor_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      fbh_viewfbpor( ScanPort );
    }

    fprintf( stdout, "    BEAUX:\n" );

    for ( ScanAux  = ScanFigure->BEAUX;
          ScanAux != (fbaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      fbh_viewfbaux( ScanAux );
    }

    fprintf( stdout, "    BEBUS:\n" );

    for ( ScanBus  = ScanFigure->BEBUS;
          ScanBus != (fbbus_list *)0;
          ScanBus  = ScanBus->NEXT )
    {
      fbh_viewfbbus( ScanBus );
    }

    fprintf( stdout, "    BEBUX:\n" );

    for ( ScanBux  = ScanFigure->BEBUX;
          ScanBux != (fbbux_list *)0;
          ScanBux  = ScanBux->NEXT )
    {
      fbh_viewfbbux( ScanBux );
    }

    fprintf( stdout, "    BEREG:\n" );

    for ( ScanReg  = ScanFigure->BEREG;
          ScanReg != (fbreg_list *)0;
          ScanReg  = ScanReg->NEXT )
    {
      fbh_viewfbreg( ScanReg );
    }

    fprintf( stdout, "    BEOUT:\n" );

    for ( ScanOut  = ScanFigure->BEOUT;
          ScanOut != (fbout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      fbh_viewfbout( ScanOut );
    }

    fprintf( stdout, "<-- fbfig_list\n" );
  }
}
