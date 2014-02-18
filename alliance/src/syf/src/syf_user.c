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
| File    :                   syf_user.c                      |
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

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <ctype.h>

# include "mut.h"
# include "aut.h"
# include "abl.h"
# include "bdd.h"
# include "fsm.h"

# include "syf_fsm.h"
# include "syf_error.h"
# include "syf_user.h"

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

  static char SyfUserBuffer[ SYF_USER_BUFFER_SIZE ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        SyfFsmUserSaveCode                   |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmUserSaveCode( FsmFigure, FileName )

  fsmfig_list *FsmFigure;
  char        *FileName;
{
  fsmfig_list   *ScanFigure;
  FILE          *UserFile;
  fsmstate_list *ScanState;
  chain_list    *ScanChain;
  syfinfo       *SyfInfo;
  long           Value;
  long           Bit;
  char           OneHot;

  UserFile = mbkfopen( FileName, "enc", "w" );

  if ( UserFile == (FILE *)0 )
  {
    SyfError( SYF_ERROR_OPEN_FILE, FileName );
  }

  for ( ScanChain  = FsmFigure->MULTI;
        ScanChain != (chain_list *)0;
        ScanChain  = ScanChain->NEXT )
  {
    ScanFigure = (fsmfig_list *)ScanChain->DATA;
    SyfInfo  = FSM_SYF_INFO( ScanFigure );
  
    fprintf( UserFile, "# Encoding figure \"%s\"\n", ScanFigure->NAME );
    fprintf( UserFile, "-%s %c %ld\n"  , ScanFigure->NAME, SyfInfo->ENCODE, SyfInfo->NUMBER_BIT );
  
    OneHot = ( SyfInfo->ENCODE == SYF_ENCODE_ONE_HOT );
  
    for ( ScanState  = ScanFigure->STATE;
          ScanState != (fsmstate_list *)0;
          ScanState  = ScanState->NEXT )
    {
      Value = FSM_SYF_STATE( ScanState )->CODE->VALUE;
  
      fprintf( UserFile,"%s\t%lX", ScanState->NAME, Value );
  
      if ( OneHot )
      {
        Bit = Value & 0x03;
  
        fprintf( UserFile, "\t%lX", (long)( 1 << Bit ) );
  
        Value = Value - Bit;
  
        while ( Value > 0 )
        {
          Value = Value - 4;
  
          fprintf( UserFile, "0" );
        }
      }
  
      if ( IsFsmFirstState( ScanState ) ) fprintf( UserFile, "\tFirst state" );
  
      fprintf( UserFile, "\n" );
    }
  }

  fclose( UserFile );
}

/*------------------------------------------------------------\
|                                                             |
|                        SyfFsmUserEncode                     |
|                                                             |
\------------------------------------------------------------*/

void SyfFsmUserEncode( FsmFigure, FileName )

  fsmfig_list *FsmFigure;
  char        *FileName;
{
  FILE          *UserFile;
  syfinfo       *SyfInfo;
  syfcode       *CodeArray;
  fsmstate_list *ScanState;
  syfstate      *ScanSyfState;
  char          *ScanBuffer;
  authtable     *HashTable;
  authelem      *Element;
  char          *StateName;
  char          *ScanName;
  char          *FigureName;
  unsigned long  StateCode;
  long           NumberState;
  long           NumberBit;
  long           CurrentLine;
  unsigned long  CodeMax;
  long           Index;
  long           Error;
  long           Skip;


  SyfInfo     = FSM_SYF_INFO( FsmFigure );
  CodeMax     = SyfInfo->NUMBER_CODE;
  CodeArray   = SyfInfo->CODE_ARRAY;
  NumberState = FsmFigure->NUMBER_STATE;

  for ( Index = 0; Index < CodeMax; Index++ )
  {
    CodeArray[ Index ].VALUE = Index;
  }

  HashTable = createauthtable( NumberState << 1 );

  for ( ScanState  = FsmFigure->STATE;
        ScanState != (fsmstate_list *)0;
        ScanState  = ScanState->NEXT )
  {
    addauthelem( HashTable, ScanState->NAME, (long)ScanState );
  }

  UserFile = mbkfopen( FileName, "enc", "r" );

  if ( UserFile == (FILE *)0 )
  {
    SyfError( SYF_ERROR_OPEN_FILE, FileName );
  }

  CurrentLine = 1;
  Error       = 0;
  Skip        = 1;

  while ( fgets( SyfUserBuffer, 
                 SYF_USER_BUFFER_SIZE,
                 UserFile ) != (char *)0 )
  {
    for ( Index = 0; SyfUserBuffer[ Index ] != '\0'; Index++ )
    {
      if ( ! isspace( SyfUserBuffer[ Index ] ) ) break;
    }

    ScanBuffer = SyfUserBuffer + Index; 

    if ( ScanBuffer[ 0 ] == '-' )
    {
      ScanBuffer = ScanBuffer + 1;
      ScanName   = strchr( ScanBuffer, ' ' );

      if ( ScanName == (char *)0 )
      {
        Error = SYF_ERROR_SYNTAX; break;
      }

      *ScanName  = '\0';
      FigureName = namealloc( ScanBuffer );
      ScanBuffer = ScanName + 1;  

      if ( FigureName == FsmFigure->NAME )
      {
        if ( ScanBuffer[ 0 ] == SYF_ENCODE_ONE_HOT )
        {
          CodeMax = NumberState;
  
          autfreeblock( (char *)CodeArray );
          CodeArray = (syfcode *)autallocblock( sizeof( syfcode ) * CodeMax );
  
          SyfInfo->CODE_ARRAY  = CodeArray;
          SyfInfo->NUMBER_CODE = CodeMax;
          SyfInfo->NUMBER_BIT  = CodeMax;
          SyfInfo->ENCODE      = SYF_ENCODE_ONE_HOT;
  
          for ( Index = 0; Index < CodeMax; Index++ )
          {
            CodeArray[ Index ].VALUE = Index;
          }
  
          Skip = 0;
        }
        else
        if ( ScanBuffer[ 0 ] == SYF_ENCODE_FRANCK )
        {
          NumberBit = atoi( &ScanBuffer[ 2 ] );
  
          if ( ( NumberBit > SyfInfo->NUMBER_BIT ) &&
               ( NumberBit < 32                  ) )
          {
            CodeMax = 1 << NumberBit;
  
            autfreeblock( (char *)CodeArray );
            CodeArray = (syfcode *)autallocblock( sizeof( syfcode ) * NumberState );
  
            SyfInfo->CODE_ARRAY  = CodeArray;
            SyfInfo->NUMBER_CODE = NumberState;
            SyfInfo->NUMBER_BIT  = NumberBit;
            SyfInfo->ENCODE      = SYF_ENCODE_FRANCK;
          
            Skip = 0;
          }
          else
          {
            Error = SYF_ERROR_SYNTAX; break;
          }
        }
        else
        {
          Skip = 0; 
        }
      }
      else
      {
        Skip = 1;
      }
    }
    else
    if ( ! Skip )
    {
      if ( ( ScanBuffer[ 0 ] != '#'  ) &&
           ( ScanBuffer[ 0 ] != '\0' ) )
      {
        StateCode = -1;
        StateName = (char *)0;
  
        for ( Index = 0; ScanBuffer[ Index ] != '\0'; Index++ )
        {
          if ( ( isspace( ScanBuffer[ Index ] )  ) &&
               ( ScanBuffer[ Index + 1 ] != '\0' ) )
          {
            StateName = ScanBuffer;
            ScanBuffer[ Index ] = '\0';
            sscanf( ScanBuffer + Index + 1, "%22lx", &StateCode );
  
            break;
          }
        }
  
        if ( StateName == (char *)0 )
        {
          Error = SYF_ERROR_SYNTAX; break;
        }
  
        if ( ( (long)StateCode < 0  ) ||
             ( StateCode >= CodeMax ) )
        {
          Error = SYF_ERROR_WRONG_CODE; break;
        }
  
        StateName = namealloc( StateName );
        Element   = searchauthelem( HashTable, StateName );
  
        if ( Element == (authelem *)0 )
        {
          Error = SYF_ERROR_UNKNOWN_STATE; break;
        }
  
        ScanState    = (fsmstate_list *)Element->VALUE;
        ScanSyfState = FSM_SYF_STATE( ScanState );
  
        if ( ScanSyfState->CODE != (syfcode *)0 )
        {
          Error = SYF_ERROR_DUPLICATE_CODE; break;
        }
  
        if ( SyfInfo->ENCODE != SYF_ENCODE_FRANCK )
        {
          if ( CodeArray[ StateCode ].USED )
          {
            Error = SYF_ERROR_DUPLICATE_CODE; break;
          }
  
          CodeArray[ StateCode ].USED  = 1;
          ScanSyfState->CODE = &CodeArray[ StateCode ];
        }
        else
        {
          CodeArray[ NumberState - 1 ].USED  = 1;
          CodeArray[ NumberState - 1 ].VALUE = StateCode;
          ScanSyfState->CODE = &CodeArray[ NumberState - 1 ];
        }
  
        NumberState--;
      }
    }

    CurrentLine++;
  }

  if ( ( ! Error          ) &&
       ( NumberState != 0 ) ) Error = SYF_ERROR_UNEXPECTED_EOF;

  if ( Error )
  {
    sprintf( SyfUserBuffer, "line %ld in file \"%s.enc\"", CurrentLine, FileName );
    SyfError( Error, SyfUserBuffer );
  }

  destroyauthtable( HashTable );

  fclose( UserFile );
}
