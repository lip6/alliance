/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     BOOM                          |
|                                                             |
| File    :                  boom_param.c                     |
|                                                             |
| Author  :                 Ludovic Jacomme                   |
|                                                             |
| Date    :                   03/07/2000                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "abe.h"
# include "abv.h"
# include "abt.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "boom_shared.h"
# include "boom_debug.h"
# include "boom_param.h"

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

   static  authtable *BoomHashAux = (authtable *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       BoomParseSetExprKeepAux               |
|                                                             |
\------------------------------------------------------------*/

static void BoomParseSetExprKeepAux( Expr )

  ablexpr *Expr;
{
  char       *Value;
  beaux_list *BehAux;
  authelem   *Element;

  if ( ABL_ATOM( Expr ) )
  {
    Value = ABL_ATOM_VALUE( Expr );

    if ( ( Value != ABL_ATOM_NAME_ONE      ) &&
         ( Value != ABL_ATOM_NAME_ZERO     ) &&
         ( Value != ABL_ATOM_NAME_DC       ) &&
         ( Value != ABL_ATOM_NAME_TRISTATE ) )
    {
      Element = searchauthelem( BoomHashAux, Value );

      if ( Element != (authelem *)0 )
      {
        BehAux = (beaux_list *)Element->VALUE;
        SetBoomBehAuxKeep( BehAux );
      }
    }
  }

  while ( ( Expr = ABL_CDR( Expr ) ) != (ablexpr *)0 )
  {
    BoomParseSetExprKeepAux( ABL_CAR( Expr ) );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomParseSetBiAblKeepAux              |
|                                                             |
\------------------------------------------------------------*/

static void BoomParseSetBiAblKeepAux( BiAbl )

  biabl_list *BiAbl;
{
  while ( BiAbl != (biabl_list *)0 )
  {
    if ( BiAbl->CNDABL != (ablexpr *)0 )
    {
      BoomParseSetExprKeepAux( BiAbl->CNDABL );
    }

    if ( BiAbl->VALABL != (ablexpr *)0 )
    {
      BoomParseSetExprKeepAux( BiAbl->VALABL );
    }

    BiAbl = BiAbl->NEXT;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomParseCheckParam                   |
|                                                             |
\------------------------------------------------------------*/

void BoomParseCheckParam( BehFigure, FileName )

  befig_list *BehFigure;
  char       *FileName;
{
  boombefiginfo *FigInfo;
  beaux_list    *BehAux;
  bereg_list    *BehReg;
  beout_list    *BehOut;
  bebux_list    *BehBux;
  bebus_list    *BehBus;
  berin_list    *BehRin;
  chain_list   **PrevChain;
  chain_list    *ScanChain;
  chain_list    *DelChain;
  authtable     *HashOut;
  authtable     *HashReg;
  authtable     *HashBux;
  authtable     *HashBus;
  authtable     *HashRin;
  authelem      *Element;

  FigInfo = GetBoomBehFigInfo( BehFigure );

  BoomHashAux = createauthtable( 100 );

  HashOut = createauthtable( 100 );
  HashReg = createauthtable( 100 );
  HashBux = createauthtable( 100 );
  HashBus = createauthtable( 100 );
  HashRin = createauthtable( 100 );

  for ( BehRin  = BehFigure->BERIN;
        BehRin != (berin_list *)0;
        BehRin  = BehRin->NEXT )
  {
    addauthelem( HashRin, BehRin->NAME, (long)BehRin );
  }

  for ( BehAux  = BehFigure->BEAUX;
        BehAux != (beaux_list *)0;
        BehAux  = BehAux->NEXT )
  {
    addauthelem( BoomHashAux, BehAux->NAME, (long)BehAux );
  }

  for ( BehOut  = BehFigure->BEOUT;
        BehOut != (beout_list *)0;
        BehOut  = BehOut->NEXT )
  {
    addauthelem( HashOut, BehOut->NAME, (long)BehOut );
  }

  for ( BehReg  = BehFigure->BEREG;
        BehReg != (bereg_list *)0;
        BehReg  = BehReg->NEXT )
  {
    addauthelem( HashReg, BehReg->NAME, (long)BehReg );
  }

  for ( BehBux  = BehFigure->BEBUX;
        BehBux != (bebux_list *)0;
        BehBux  = BehBux->NEXT )
  {
    addauthelem( HashBux, BehBux->NAME, (long)BehBux );
  }

  for ( BehBus  = BehFigure->BEBUS;
        BehBus != (bebus_list *)0;
        BehBus  = BehBus->NEXT )
  {
    addauthelem( HashBus, BehBus->NAME, (long)BehBus );
  }

  PrevChain = &FigInfo->KEEP_LIST;
  ScanChain = FigInfo->KEEP_LIST;

  while ( ScanChain != (chain_list *)0 )
  {
    Element = searchauthelem( BoomHashAux, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehAux = (beaux_list *)Element->VALUE;
      SetBoomBehAuxKeep( BehAux );

      fprintf( stderr, "\t    keep signal '%s'\n", BehAux->NAME );
    }
    else
    {
      fprintf( stderr, "\t### unknown signal '%s' in parameter file %s.boom\n",
               (char *)ScanChain->DATA, FileName  );

      *PrevChain = ScanChain->NEXT;
      DelChain   = ScanChain;
      ScanChain  = ScanChain->NEXT;

      DelChain->NEXT = (chain_list *)0;
      freechain( DelChain );
    }
  }

  PrevChain = &FigInfo->DONT_TOUCH_LIST;
  ScanChain = FigInfo->DONT_TOUCH_LIST;

  while ( ScanChain != (chain_list *)0 )
  {
    Element = searchauthelem( BoomHashAux, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehAux = (beaux_list *)Element->VALUE;
      SetBoomBehAuxDontTouch( BehAux );
      SetBoomBehAuxKeep( BehAux );

      BoomParseSetExprKeepAux( BehAux->ABL );

      continue;
    }

    Element = searchauthelem( HashOut, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehOut = (beout_list *)Element->VALUE;
      SetBoomBehAuxDontTouch( BehOut );

      BoomParseSetExprKeepAux( BehOut->ABL );

      continue;
    }

    Element = searchauthelem( HashReg, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehReg = (bereg_list *)Element->VALUE;
      SetBoomBehAuxDontTouch( BehReg );

      BoomParseSetBiAblKeepAux( BehReg->BIABL );

      continue;
    }

    Element = searchauthelem( HashBus, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehBus = (bebus_list *)Element->VALUE;
      SetBoomBehAuxDontTouch( BehBus );

      BoomParseSetBiAblKeepAux( BehBus->BIABL );

      continue;
    }

    Element = searchauthelem( HashBux, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehBux = (bebux_list *)Element->VALUE;
      SetBoomBehAuxDontTouch( BehBux );

      BoomParseSetBiAblKeepAux( BehBux->BIABL );

      continue;
    }

    if ( Element == (authelem *)0 )
    {
      fprintf( stderr, "\t### unknown signal '%s' in parameter file %s.boom\n",
               (char *)ScanChain->DATA, FileName  );

      *PrevChain = ScanChain->NEXT;
      DelChain   = ScanChain;
      ScanChain  = ScanChain->NEXT;

      DelChain->NEXT = (chain_list *)0;
      freechain( DelChain );
    }
  }

  FigInfo->BDD_ORDER_LIST = reverse( FigInfo->BDD_ORDER_LIST );

  PrevChain = &FigInfo->BDD_ORDER_LIST;
  ScanChain = FigInfo->BDD_ORDER_LIST;

  while ( ScanChain != (chain_list *)0 )
  {
    Element = searchauthelem( HashRin, (char *)ScanChain->DATA );

    if ( Element != (authelem *)0 )
    {
      PrevChain = &ScanChain->NEXT;
      ScanChain = ScanChain->NEXT;

      BehRin = (berin_list *)Element->VALUE;
      SetBoomBehRinOrder( BehRin );

      fprintf( stderr, "\t    order input/signal/register '%s'\n", BehRin->NAME );
    }
    else
    {
      fprintf( stderr, "\t### unknown signal '%s' in parameter file %s.boom\n",
               (char *)ScanChain->DATA, FileName  );

      *PrevChain = ScanChain->NEXT;
      DelChain   = ScanChain;
      ScanChain  = ScanChain->NEXT;

      DelChain->NEXT = (chain_list *)0;
      freechain( DelChain );
    }
  }

  destroyauthtable( HashReg );
  destroyauthtable( HashOut );
  destroyauthtable( HashBus );
  destroyauthtable( HashBux );
  destroyauthtable( HashRin );

  destroyauthtable( BoomHashAux );
  BoomHashAux = (authtable *)0; 
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomParseParamFile                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          BoomParseGetWord                   |
|                                                             |
\------------------------------------------------------------*/

static char *BoomParseGetWord( Buffer )

  char *Buffer;
{
  char *String;

  if ( ( String = (char *)strtok( Buffer, " =,\t\n" ) )  != (char *)0 )
  {
    String = namealloc( String );
  }

  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                       BoomParseParamFile                    |
|                                                             |
\------------------------------------------------------------*/

void BoomParseParamFile( BehFigure, FileName )

  befig_list *BehFigure;
  char       *FileName;
{
  boombefiginfo    *FigInfo;
  chain_list      **PrevChain;
  char             *FirstWord;
  char             *Scan;
  char             *Pointer;
  char             *RegisterName;
  int               Left;
  int               Right;
  int               Step;
  int               Index;
  int               CurrentDef;
  long              LineNumber;
  FILE             *ParamFile;
  char              ParamBuffer[ 512 ];
  char              Buffer[ 128 ];

  FigInfo = GetBoomBehFigInfo( BehFigure );

  FileName  = autbasename( FileName, "boom" );
  ParamFile = mbkfopen( FileName, "boom", "r" );

  if ( ParamFile == (FILE *)0 )
  {
    fprintf( stderr, "\t### Unable to parse parameter file %s.boom\n", FileName );
    autexit( 1 );
  }

  LineNumber = 0;
  CurrentDef = BOOM_PARAM_NONE;

  while ( fgets( ParamBuffer, 512, ParamFile ) != (char *)0 )
  {
    LineNumber++;

    if ( ParamBuffer[ 0 ] == '#' ) continue;

    FirstWord = BoomParseGetWord( ParamBuffer ); 

    if ( FirstWord == (char *)0 ) continue;

    if ( ! strncmp( FirstWord, "end", 3 ) ) CurrentDef = BOOM_PARAM_NONE;
    else
    if ( CurrentDef != BOOM_PARAM_NONE )
    {
      /*
      if ( CurrentDef == BOOM_PARAM_CLOCK )
      {
        SecondWord = BoomParseGetWord( (char *)0 );

        if ( SecondWord == (char *)0 )
        {
          fprintf( stderr, "Syntax error line %ld in parameter file %s.boom\n",
                   LineNumber, FileName  );
          autexit( 1 );
        }
      }
      else
      */
      if ( ( CurrentDef == BOOM_PARAM_KEEP_AUX       ) ||
           ( CurrentDef == BOOM_PARAM_DONT_TOUCH_AUX ) ||
           ( CurrentDef == BOOM_PARAM_BDD_ORDER      ) )
      {
        if ( CurrentDef == BOOM_PARAM_KEEP_AUX )  PrevChain = &FigInfo->KEEP_LIST;
        else
        if ( CurrentDef == BOOM_PARAM_BDD_ORDER ) PrevChain = &FigInfo->BDD_ORDER_LIST;
        else
                                                  PrevChain = &FigInfo->DONT_TOUCH_LIST;
        do
        {
          Scan = strchr( FirstWord, '[' );
          if ( Scan == NULL ) Scan = strchr( FirstWord, '(' );

          if ( Scan != NULL )
          {
            strcpy( Buffer, FirstWord );
            Scan = Buffer;

            Pointer  = strchr( Scan, '[' );
            if ( Pointer == NULL ) Pointer = strchr( Scan, '(' );
            *Pointer = '\0';

            RegisterName = namealloc( Buffer );
            Scan     = Pointer + 1;

            Pointer = strchr( Scan, ':' );

            if ( Pointer != (char *)0 )
            {
              *Pointer = '\0';
              Left = atoi( Scan );
              Scan = Pointer + 1;

              Pointer = strchr( Scan, ']' );
              if ( Pointer == (char *)0 ) Pointer = strchr( Scan, ')' );

              if ( Pointer == (char *)0 )
              {
                fprintf( stderr, "\t### Syntax error line %ld in parameter file %s.boom\n",
                         LineNumber, FileName  );
                autexit(1);
              }

              *Pointer = '\0';
              Right = atoi( Scan );
            }
            else
            {
              Left  = atoi( Scan ); 
              Right = Left;
            }

            if ( Left < Right ) Step = -1;
            else                Step =  1;
               
            for ( Index = Right; Index != (Left + Step) ; Index += Step )
            {
              sprintf( Buffer, "%s %d", RegisterName, Index );
              *PrevChain = addchain( *PrevChain, namealloc( Buffer ) );
            }
          }
          else
          {
            *PrevChain = addchain( *PrevChain, FirstWord );
          }
        } while ( ( FirstWord = BoomParseGetWord( NULL ) ) != NULL );
      }
      else
      {
        fprintf( stderr, "Syntax error line %ld in parameter file %s.boom\n",
                 LineNumber, FileName  );
        autexit( 1 );
      }
    }
    else
    if ( ! strcmp( FirstWord, "begin_keep" ) )
    {
      CurrentDef = BOOM_PARAM_KEEP_AUX;
    }
    else
    if ( ! strcmp( FirstWord, "begin_dont_touch" ) )
    {
      CurrentDef = BOOM_PARAM_DONT_TOUCH_AUX;
    }
    else
    if ( ! strcmp( FirstWord, "begin_bdd_order" ) )
    {
      CurrentDef = BOOM_PARAM_BDD_ORDER;
    }
    else
    {
      fprintf( stderr, "Syntax error line %ld in parameter file %s.boom\n",
               LineNumber, FileName  );
      autexit( 1 );
    }
  }

  fclose( ParamFile  );

  BoomParseCheckParam( BehFigure, FileName );
}
