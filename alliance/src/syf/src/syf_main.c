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
| Tool    :                     SYF                           |
|                                                             |
| File    :                  syf_main.c                       |
|                                                             |
| Authors :                  C. Sarwary                       |
|                     Modified by Jacomme Ludovic             |
|                                                             |
| Date    :                   01.11.94                        |
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
# include "fsm.h"
# include "fvh.h"
# include "ftl.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "syf_main.h"
# include "syf_error.h"
# include "syf_env.h"
# include "syf_fsm.h"
# include "syf_comp.h"
# include "syf_fsm2fbh.h"
# include "syf_cost.h"
# include "syf_verify.h"
# include "syf_synth.h"
# include "syf_dc.h"
# include "syf_encod.h"
# include "syf_simp.h"
# include "syf_bdd.h"
# include "syf_verify.h"

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

   fsmfig_list *SyfFsmFigure = (fsmfig_list    *)0;
   fbfig_list  *SyfFbhFigure = (fbfig_list *)0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Syf Usage                          |
|                                                             |
\------------------------------------------------------------*/

void SyfUsage()
{
  fprintf( stdout, "\t\tsyf Encoding [Options] Input_name [Output_name]\n\n" );

  fprintf( stdout, "\t\tEncoding : -a Asp encoding\n"                   );
  fprintf( stdout, "\t\t           -j Jedi encoding\n"                  );
  fprintf( stdout, "\t\t           -m Mustang encoding\n"               );
  fprintf( stdout, "\t\t           -u User 'Input_name'.enc file\n"     );
  fprintf( stdout, "\t\t           -o One hot encoding\n"               );
  fprintf( stdout, "\t\t           -r Random encoding\n\n"              );

  fprintf( stdout, "\t\tOptions  : -C Checks FSM transitions\n"            );
  fprintf( stdout, "\t\t           -D Doesn't treat unused codes as DC\n"  );
  fprintf( stdout, "\t\t           -E Saves 'Output_name'.enc file\n"      );
  fprintf( stdout, "\t\t           -O Uses registers for outputs\n"        );
  fprintf( stdout, "\t\t           -P Uses a scan-path for registers\n"    );
  fprintf( stdout, "\t\t           -I Doesn't inverse outputs polarity \n" );
  fprintf( stdout, "\t\t           -S Doesn't simplify FSM equations\n"    );
  fprintf( stdout, "\t\t           -T Sets Trace mode on\n"                );
  fprintf( stdout, "\t\t           -V Sets Verbose mode on\n"              );
  fprintf( stdout, "\t\t           -F Sets synopsys output Format\n"       );
  fprintf( stdout, "\n" );

  exit( 1 );
}

/*------------------------------------------------------------\
|                                                             |
|                            Main                             |
|                                                             |
\------------------------------------------------------------*/

int main( argc, argv )

   int   argc;
   char *argv[];
{
  chain_list  *ScanChain;
  fsmfig_list *FsmFigure;
  char        *InputFileName;
  char        *OutputFileName;
  char        *FileName;
  long         Literals;
  int          Number;
  int          Index;
  char         Option;

  int   FlagSave      = 0;
  int   FlagScan      = 0;
  int   FlagCode      = -1;
  int   FlagVerbose   = 0;
  int   FlagTrace     = 0;
  int   FlagRegOut    = 0;
  int   FlagDc        = 1;
  int   FlagCheck     = 0;
  int   FlagSimplify  = 1;
  int   FlagInvPolar  = 1;
  int   FlagMustCost  = 0;
  int   FlagFormat    = 0;

  alliancebanner_with_authors( 
      "SYF", VERSION, "FSM Synthesizer", "1995",
      ALLIANCE_VERSION, "Ludovic Jacomme and Chaker Sarwary" );

  mbkenv();
  autenv();
  ablenv();
  bddenv();
  fsmenv();

  if ( argc < 3 ) SyfUsage();

  OutputFileName = (char *)0;
  InputFileName  = (char *)0;

  for ( Number = 1; Number  < argc; Number++ )
  {
    if ( argv[ Number ][ 0 ] == '-' )
    {
      for ( Index = 1; argv[ Number ][ Index ] != '\0'; Index++ )
      {
        Option = argv[ Number ][ Index ];

        switch ( Option )
        {
          case 'C' : FlagCheck = 1;
          break;
          case 'D' : FlagDc = 0;
          break;
          case 'E' : FlagSave = 1;
          break;
          case 'F' : FlagFormat = 1;
          break;
          case 'O' : FlagRegOut = 1;
          break;
          case 'P' : FlagScan = 1;
          break;
          case 'S' : FlagSimplify = 0;
          break;
          case 'T' : FlagTrace = 1;
          break;
          case 'V' : FlagVerbose = 1;
          break;
          case 'M' : FlagMustCost = 1;
          break;
          case 'I' : FlagInvPolar = 0;
          break;
          case 'r' : FlagCode = SYF_ENCODE_RANDOM;
          break;
          case 'c' : FlagCode = SYF_ENCODE_CHAKER;
          break;
          case 'u' : FlagCode = SYF_ENCODE_USER;
          break;
          case 'm' : FlagCode = SYF_ENCODE_MUSTANG;
          break;
          case 'a' : FlagCode = SYF_ENCODE_ASP;
          break;
          case 'j' : FlagCode = SYF_ENCODE_JEDI;
          break;
          case 'o' : FlagCode = SYF_ENCODE_ONE_HOT;
          break;
          default  : SyfUsage();
        }
      }
    }
    else
    if ( InputFileName == (char *)0 ) InputFileName = argv[ Number ];
    else
    if ( OutputFileName == (char *)0 ) OutputFileName = argv[ Number ];
    else
    SyfUsage();
  }

  if ( ( InputFileName == (char *)0 ) ||
       ( FlagCode      == -1        ) ) SyfUsage();

  InputFileName  = autbasename( InputFileName, FSM_IN );

  FileName = autallocblock( strlen( InputFileName ) + 7 );
  strcpy( FileName, InputFileName );
  InputFileName = FileName;

  if ( OutputFileName == (char *)0 )
  {
    OutputFileName = autallocblock( strlen( InputFileName ) + 2 );
    sprintf( OutputFileName, "%s%c", InputFileName, FlagCode );
  }
  else
  {
    OutputFileName = autbasename( OutputFileName, FSM_OUT );
  }

  SyfEnv( FlagTrace );

  fprintf( stdout, "\t\t--> Compile FSM file %s\n", InputFileName );

  SyfFsmFigure = SyfCompile( InputFileName, FlagScan, FlagFormat );

  if ( FlagInvPolar )
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "\t\t--> Check FSM figure outputs\n" );
    }

    for ( ScanChain  = SyfFsmFigure->MULTI;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      FsmFigure = (fsmfig_list *)ScanChain->DATA;
      SyfFsmOutputPolarity( FsmFigure );
    }
  }

  if ( FlagVerbose )
  {
    if ( FlagSimplify )
    {
      fprintf( stdout, "\t\t--> Simplify FSM figure\n" );
    }
  }

  for ( ScanChain  = SyfFsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FsmFigure = (fsmfig_list *)ScanChain->DATA;
    SyfFsmSimplify( FsmFigure, FlagSimplify );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t\t--> Verify FSM figure\n" );
  }

  for ( ScanChain  = SyfFsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FsmFigure = (fsmfig_list *)ScanChain->DATA;
    SyfFsmVerify( FsmFigure, FlagCheck );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t\t--> Identify reset conditions\n" );
  }

  for ( ScanChain  = SyfFsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FsmFigure = (fsmfig_list *)ScanChain->DATA;
    SyfFsmTreatReset( FsmFigure );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\n" );

    if ( IsFsmFigMixedRtl( SyfFsmFigure ) )
    {
      fprintf( stdout, "\t\t--> Mixed DataFlow / Fsm\n\n" );
    }

    for ( ScanChain  = SyfFsmFigure->MULTI;
          ScanChain != (chain_list *)0;
          ScanChain  = ScanChain->NEXT )
    {
      FsmFigure = (fsmfig_list *)ScanChain->DATA;

      fprintf( stdout, "\t\t--> Name    : %s\n" , FsmFigure->NAME         );
      fprintf( stdout, "\t\t--> States  : %ld\n", FsmFigure->NUMBER_STATE );
      fprintf( stdout, "\t\t--> Inputs  : %ld\n", FsmFigure->NUMBER_IN    );
      fprintf( stdout, "\t\t--> Outputs : %ld\n", FsmFigure->NUMBER_OUT   );
      fprintf( stdout, "\t\t--> Edges   : %ld\n", FsmFigure->NUMBER_TRANS );
      fprintf( stdout, "\t\t--> Stacks  : %ld\n", FsmFigure->NUMBER_STACK );
      fprintf( stdout, "\n" );
    }

    fprintf( stdout, "\t\t--> Encode FSM figure\n" );
  }

  for ( ScanChain  = SyfFsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FsmFigure = (fsmfig_list *)ScanChain->DATA;
    SyfFsmEncode( FsmFigure, FlagCode, FlagVerbose,
                  FlagDc, FlagRegOut, FlagMustCost, InputFileName );
  }

  if ( FlagSave )
  {
    if ( FlagVerbose )
    {
      fprintf( stdout, "\t\t--> Save FSM codes\n" );
    }

    SyfFsmUserSaveCode( SyfFsmFigure, OutputFileName );
  }

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t\t--> Translate FSM to BEH\n" );
  }

  for ( ScanChain  = SyfFsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    FsmFigure = (fsmfig_list *)ScanChain->DATA;
    SyfSynthFsmInit( FsmFigure );
    SyfSynthCode2Abl( FsmFigure, FlagDc );
    SyfSynthFsm2Abl( FsmFigure );
  }

  SyfFbhFigure = SyfFsm2Fbh( SyfFsmFigure );

  if ( FlagVerbose )
  {
    fprintf( stdout, "\t\t--> Simplify BEH figure\n" );
  }

  SyfFbhSimplify( SyfFbhFigure );

  if ( FlagVerbose )
  {
    Literals = SyfFbhAblGiveCost( SyfFbhFigure );

    fprintf( stdout, "\n" );
    fprintf( stdout, "\t\t--> Literals : %ld\n", Literals );
    fprintf( stdout, "\n" );
  }

  fprintf( stdout, "\t\t--> Save BEH file %s\n\n", OutputFileName );

  SyfFbhFigure->NAME = namealloc( OutputFileName );

  vhdlsavefbfig( SyfFbhFigure, OutputFileName, FlagFormat );

  return( 0 );
}
