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
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
| Tool    :                     SYF                           |
|                                                             |
| File    :                  syf_encod.c                      |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "syf_env.h"
# include "syf_fsm.h"
# include "syf_tools.h"
# include "syf_error.h"
# include "syf_encod.h"
# include "syf_asp.h"
# include "syf_random.h"
# include "syf_user.h"
# include "syf_must.h"
# include "syf_jedi.h"
# include "syf_onehot.h"
# include "syf_synth.h"

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         SyfFsmEncode                        |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmEncode( FsmFigure, Encode, Verbose,
                   TreatDc, RegOut, MustCost, FileName )

  fsmfig_list *FsmFigure;
  int          Encode;
  int          Verbose;
  int          TreatDc;
  int          RegOut;
  int          MustCost;
  char        *FileName;
{
  syfinfo       *SyfInfo;
  syfcode       *CodeArray;
  long           CodeMax;
  long           NumberBit;

  SyfInfo = FSM_SYF_INFO( FsmFigure );

  SyfInfo->REG_OUT = RegOut;

  if ( Encode != SYF_ENCODE_ONE_HOT )
  {
    NumberBit = SyfGetNumberBit( FsmFigure->NUMBER_STATE - 1 );
    CodeMax   = ( 1 << NumberBit );
  }
  else
  {
    CodeMax = NumberBit = FsmFigure->NUMBER_STATE;
  }

  CodeArray = (syfcode *)autallocblock( sizeof( syfcode ) * CodeMax );

  SyfInfo->CODE_ARRAY  = CodeArray;
  SyfInfo->NUMBER_CODE = CodeMax;
  SyfInfo->NUMBER_BIT  = NumberBit;
  SyfInfo->ENCODE      = Encode;

  switch( Encode )
  {
      case SYF_ENCODE_RANDOM :
  
        if ( Verbose ) fprintf( stdout, "\t\t--> Random encoding\n" );
        SyfFsmRandomEncode( FsmFigure );
  
      break;
  
      case SYF_ENCODE_USER :
  
        if ( Verbose ) fprintf( stdout, "\t\t--> User encoding\n" );
        SyfFsmUserEncode( FsmFigure, FileName );
  
      break;
  
      case SYF_ENCODE_MUSTANG :
  
        if ( Verbose ) fprintf( stdout, "\t\t--> Mustang encoding\n" );
        SyfFsmMustangEncode( FsmFigure );
  
      break;
  
      case SYF_ENCODE_ASP :
  
        if ( Verbose ) fprintf( stdout, "\t\t--> Asp encoding\n" );

        SyfSynthTrans2Abl( FsmFigure );
        SyfFsmAspEncode( FsmFigure );
        SyfSynthFreeTransAbl( FsmFigure );
  
      break;
  
      case SYF_ENCODE_JEDI :
  
       if ( Verbose ) fprintf( stdout, "\t\t--> Jedi encoding\n" );
       SyfFsmJediEncode( FsmFigure );
  
      break;
  
      case SYF_ENCODE_ONE_HOT :
  
       if ( Verbose ) fprintf( stdout, "\t\t--> One hot encoding\n" );
       SyfFsmOneHotEncode( FsmFigure );
  
      break;
  
      default :
  
        SyfError( SYF_ERROR_UNKNOWN_ENCODE_TYPE, Encode );
  }

  if ( ( MustCost ) &&
       ( Verbose  ) )
  {
    fprintf( stdout, "\t\t--> Mustang cost %ld\n", 
                     SyfFsmGetMustangCost( FsmFigure ) );
  }
}
