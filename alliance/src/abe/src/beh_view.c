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
| File    :                   beh_view.c                      |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   09.11.99                        |
|                                                             |
\------------------------------------------------------------*/

#include MUT_H 
#include AUT_H
#include ABL_H 
#include ABE_H

static void loc_beh_viewablexprln( Expr )

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

void beh_viewbiabl( BiAbl )

  biabl_list *BiAbl;
{
  if ( BiAbl == (biabl_list *)0 )
  {
    fprintf( stdout, "NULL\n" );
  }
  else
  {
    while ( BiAbl != (biabl_list *)0 )
    {
      fprintf( stdout, "--> biabl_list\n" );
      fprintf( stdout, "    LABEL  : %s\n", BiAbl->LABEL );
      fprintf( stdout, "    CNDABL : " );
      loc_beh_viewablexprln( BiAbl->CNDABL );
      fprintf( stdout, "    VALABL : " );
      loc_beh_viewablexprln( BiAbl->VALABL );
      fprintf( stdout, "<-- biabl_list\n" );

      BiAbl = BiAbl->NEXT;
    }
  }
}

void beh_viewberin( ScanRin )

  berin_list *ScanRin; 
{
  if ( ScanRin != (berin_list *)0 )
  {
    fprintf( stdout, "--> berin_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanRin->NAME );
    fprintf( stdout, "<-- berin_list\n" );
  }
}

void beh_viewbereg( ScanReg )

  bereg_list *ScanReg; 
{
  if ( ScanReg != (bereg_list *)0 )
  {
    fprintf( stdout, "--> bereg_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanReg->NAME      );
    fprintf( stdout, "    BIABL :\n" );
    beh_viewbiabl( ScanReg->BIABL );
    fprintf( stdout, "<-- bereg_list\n" );
  }
}

void beh_viewbebux( ScanBux )

  bebux_list *ScanBux; 
{
  if ( ScanBux != (bebux_list *)0 )
  {
    fprintf( stdout, "--> bebux_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanBux->NAME      );
    fprintf( stdout, "    TYPE  : %c\n", (int)ScanBux->TYPE );
    fprintf( stdout, "    BIABL :\n" );
    beh_viewbiabl( ScanBux->BIABL );
    fprintf( stdout, "<-- bebux_list\n" );
  }
}

void beh_viewbebus( ScanBus )

  bebus_list *ScanBus; 
{
  if ( ScanBus != (bebus_list *)0 )
  {
    fprintf( stdout, "--> bebus_list\n" );
    fprintf( stdout, "    NAME  : %s\n", ScanBus->NAME      );
    fprintf( stdout, "    TYPE  : %c\n", (int)ScanBus->TYPE );
    fprintf( stdout, "    BIABL :\n" );
    beh_viewbiabl( ScanBus->BIABL );
    fprintf( stdout, "<-- bebus_list\n" );
  }
}

void beh_viewbeaux( ScanAux )

  beaux_list *ScanAux; 
{
  if ( ScanAux != (beaux_list *)0 )
  {
    fprintf( stdout, "--> beaux_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanAux->NAME      );
    fprintf( stdout, "    ABL  :" );
    loc_beh_viewablexprln( ScanAux->ABL, ABL_VIEW_VHDL );
    fprintf( stdout, "<-- beaux_list\n" );
  }
}

void beh_viewbeout( ScanOut )

  beout_list *ScanOut; 
{
  if ( ScanOut != (beout_list *)0 )
  {
    fprintf( stdout, "--> beout_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanOut->NAME      );
    fprintf( stdout, "    ABL  :" );
    loc_beh_viewablexprln( ScanOut->ABL );
    fprintf( stdout, "<-- beout_list\n" );
  }
}

void beh_viewbepor( ScanPort )

  bepor_list *ScanPort; 
{
  if ( ScanPort != (bepor_list *)0 )
  {
    fprintf( stdout, "--> bepor_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanPort->NAME      );
    fprintf( stdout, "    DIR  : %c\n", ScanPort->DIRECTION );
    fprintf( stdout, "    TYPE : %c\n", (int)ScanPort->TYPE );
    fprintf( stdout, "<-- bepor_list\n" );
  }
}

void beh_viewbefig( ScanFigure )

  befig_list *ScanFigure; 
{
  bepor_list *ScanPort;
  beaux_list *ScanAux;
  bebus_list *ScanBus;
  bebux_list *ScanBux;
  bereg_list *ScanReg;
  beout_list *ScanOut;
  berin_list *ScanRin;

  if ( ScanFigure != (befig_list *)0 )
  {
    fprintf( stdout, "--> befig_list\n" );
    fprintf( stdout, "    NAME : %s\n", ScanFigure->NAME );

    fprintf( stdout, "    BEPOR:\n" );

    for ( ScanPort  = ScanFigure->BEPOR;
          ScanPort != (bepor_list *)0;
          ScanPort  = ScanPort->NEXT )
    {
      beh_viewbepor( ScanPort );
    }

    fprintf( stdout, "    BERIN:\n" );

    for ( ScanRin  = ScanFigure->BERIN;
          ScanRin != (berin_list *)0;
          ScanRin  = ScanRin->NEXT )
    {
      beh_viewberin( ScanRin );
    }

    fprintf( stdout, "    BEAUX:\n" );

    for ( ScanAux  = ScanFigure->BEAUX;
          ScanAux != (beaux_list *)0;
          ScanAux  = ScanAux->NEXT )
    {
      beh_viewbeaux( ScanAux );
    }

    fprintf( stdout, "    BEBUS:\n" );

    for ( ScanBus  = ScanFigure->BEBUS;
          ScanBus != (bebus_list *)0;
          ScanBus  = ScanBus->NEXT )
    {
      beh_viewbebus( ScanBus );
    }

    fprintf( stdout, "    BEBUX:\n" );

    for ( ScanBux  = ScanFigure->BEBUX;
          ScanBux != (bebux_list *)0;
          ScanBux  = ScanBux->NEXT )
    {
      beh_viewbebux( ScanBux );
    }

    fprintf( stdout, "    BEREG:\n" );

    for ( ScanReg  = ScanFigure->BEREG;
          ScanReg != (bereg_list *)0;
          ScanReg  = ScanReg->NEXT )
    {
      beh_viewbereg( ScanReg );
    }

    fprintf( stdout, "    BEOUT:\n" );

    for ( ScanOut  = ScanFigure->BEOUT;
          ScanOut != (beout_list *)0;
          ScanOut  = ScanOut->NEXT )
    {
      beh_viewbeout( ScanOut );
    }

    fprintf( stdout, "<-- befig_list\n" );
  }
}
