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
| Tool    :                     SCAN                          |
|                                                             |
| File    :                  scan_param.c                     |
|                                                             |
| Author  :                                                   |
|                                                             |
| Date    :                                                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "mlu.h"

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

# include "scan_param.h"

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
|                         ScanParseCheckModel                 |
|                                                             |
\------------------------------------------------------------*/

static int ScanParseCheckModel( ModelName )

  char *ModelName;
{
  FILE *File;

  File = mbkfopen( ModelName, IN_LO, "r" );

  if ( File == (FILE *)0 )
  {
    File = mbkfopen( ModelName, "vbe", "r" );
  }

  if ( File != (FILE *)0 )
  {
    fclose( File );

    return( 1 );
  }

  return( 0 );
}

/*------------------------------------------------------------\
|                                                             |
|                          ScanParseGetWord                   |
|                                                             |
\------------------------------------------------------------*/

static char *ScanParseGetWord( Buffer )

  char *Buffer;
{
  char *String;

  if ( ( String = (char *)strtok( Buffer, " =\t\n" ) )  != (char *)0 )
  {
    String = namealloc( String );
  }

  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                         ScanParseViewParam                  |
|                                                             |
\------------------------------------------------------------*/

static void ScanParseViewParam( Param )

  scanparam *Param;
{
  scanmux_list    *ScanMux;
  scanreg_list    *ScanReg;
  scanregmux_list *ScanRegmux;
  scanbuf_list    *ScanBuffer;


  for ( ScanMux  = Param->MUX_LIST;
        ScanMux != (scanmux_list *)0;
        ScanMux  = ScanMux->NEXT )
  {
    fprintf( stdout, "BEGIN_MUX\n" );

    fprintf( stdout, "MUX_MODEL      : %s\n", ScanMux->MODEL      );
    fprintf( stdout, "MUX_SEL        : %s\n", ScanMux->SEL        );
    fprintf( stdout, "MUX_INPUT_SEL  : %s\n", ScanMux->INPUT_SEL  );
    fprintf( stdout, "MUX_INPUT_NSEL : %s\n", ScanMux->INPUT_NSEL );
    fprintf( stdout, "MUX_VDD        : %s\n", ScanMux->VDD        );
    fprintf( stdout, "MUX_VSS        : %s\n", ScanMux->VSS        );
    fprintf( stdout, "MUX_OUTPUT     : %s\n", ScanMux->OUTPUT     );

    fprintf( stdout, "END_MUX\n" );
  }
  
  for  ( ScanReg  = Param->REG_LIST;
         ScanReg != (scanreg_list *)0;
         ScanReg  = ScanReg->NEXT )
  {
    fprintf( stdout, "BEGIN_REG\n" );
 
    fprintf( stdout, "REG_MODEL      : %s\n", ScanReg->MODEL    );
    fprintf( stdout, "REG_CLK        : %s\n", ScanReg->CLK      );
    fprintf( stdout, "REG_INPUT      : %s\n", ScanReg->INPUT    );
    fprintf( stdout, "REG_VDD        : %s\n", ScanReg->VDD      );
    fprintf( stdout, "REG_VSS        : %s\n", ScanReg->VSS      );
    fprintf( stdout, "REG_OUTPUT     : %s\n", ScanReg->OUTPUT   );           
    fprintf( stdout, "REG_MUX        : %s\n", ScanReg->MUX      );           
    fprintf( stdout, "REG_REG_MUX    : %s\n", ScanReg->REG_MUX  );           

    fprintf( stdout, "END_REG\n" );
                              
  }

  for ( ScanRegmux  = Param->REG_MUX_LIST;
        ScanRegmux != (scanregmux_list *)0;
        ScanRegmux  = ScanRegmux->NEXT )
  {
    fprintf( stdout, "BEGIN_REG_MUX\n" );        
  
    fprintf( stdout, "REG_MUX_MODEL      : %s\n", ScanRegmux->MODEL      );
    fprintf( stdout, "REG_MUX_SEL        : %s\n", ScanRegmux->SEL        );
    fprintf( stdout, "REG_MUX_INPUT_SEL  : %s\n", ScanRegmux->INPUT_SEL  );
    fprintf( stdout, "REG_MUX_INPUT_NSEL : %s\n", ScanRegmux->INPUT_NSEL );
    fprintf( stdout, "REG_MUX_CLK        : %s\n", ScanRegmux->CLK        );
    fprintf( stdout, "REG_MUX_VDD        : %s\n", ScanRegmux->VSS        );
    fprintf( stdout, "REG_MUX_VSS        : %s\n", ScanRegmux->VDD        );
    fprintf( stdout, "REG_MUX_OUTPUT     : %s\n", ScanRegmux->OUTPUT     );
    fprintf( stdout, "REG_MUX_MUX        : %s\n", ScanRegmux->MUX        );
    fprintf( stdout, "REG_MUX_REG        : %s\n", ScanRegmux->REG        );

    fprintf( stdout, "END_REG_MUX\n" );
  }                                     

  for ( ScanBuffer  = Param->BUF_LIST;
        ScanBuffer != (scanbuf_list *)0;
        ScanBuffer  = ScanBuffer->NEXT )
  {
    fprintf( stdout, "BEGIN_BUF\n" );        
  
    fprintf( stdout, "REG_BUF_MODEL      : %s\n", ScanBuffer->MODEL  );
    fprintf( stdout, "REG_BUF_INPUT      : %s\n", ScanBuffer->INPUT  );
    fprintf( stdout, "REG_BUF_VDD        : %s\n", ScanBuffer->VSS    );
    fprintf( stdout, "REG_BUF_VSS        : %s\n", ScanBuffer->VDD    );
    fprintf( stdout, "REG_BUF_OUTPUT     : %s\n", ScanBuffer->OUTPUT );

    fprintf( stdout, "END_BUF\n" );
  }                                     
}

/*------------------------------------------------------------\
|                                                             |
|                       ScanParseVerify                       |
|                                                             |
\------------------------------------------------------------*/

static void ScanParseVerifyParam( ScanParam )

  scanparam *ScanParam;
{
  scanmux_list    *ScanMux;
  scanreg_list    *ScanReg;
  scanregmux_list *ScanRegMux;
  scanbuf_list    *ScanBuf;
  char            *Model;
  int              Error;
  authtable       *HashMux;
  authtable       *HashReg;
  authtable       *HashRegMux;
  authelem        *Element;

  HashMux    = createauthtable( 20 );
  HashReg    = createauthtable( 20 );
  HashRegMux = createauthtable( 20 );

  Error = 0;

/*
** Hash All definitions
*/
  for ( ScanMux  = ScanParam->MUX_LIST;
        ScanMux != (scanmux_list *)0; 
        ScanMux  = ScanMux->NEXT )
  {
    if ( ScanMux->MODEL != (char *)0 ) addauthelem( HashMux, ScanMux->MODEL, 0 );
  }

  for ( ScanReg  = ScanParam->REG_LIST;
        ScanReg != (scanreg_list *)0; 
        ScanReg  = ScanReg->NEXT )
  {
    if ( ScanReg->MODEL != (char *)0 ) addauthelem( HashReg, ScanReg->MODEL, 0 );
  }

  for ( ScanRegMux  = ScanParam->REG_MUX_LIST;
        ScanRegMux != (scanregmux_list *)0;
        ScanRegMux  = ScanRegMux->NEXT )
  {
    if ( ScanRegMux->MODEL != (char *)0 ) addauthelem( HashRegMux, ScanRegMux->MODEL, 0 );
  }

/*
** Verify the correctness of those definitions
*/
  
  for ( ScanMux  = ScanParam->MUX_LIST;
        ScanMux != (scanmux_list *)0;
        ScanMux  = ScanMux ->NEXT )
  {
    Model = ScanMux->MODEL;

    if ( ScanMux->MODEL == NULL )
    {
      fprintf( stdout, "\t### Error MODEL field empty in a MUX definition\n" );
      Error++;
      Model = "<undefined>";
    }
    else
    if ( ! ScanParseCheckModel( Model ) )
    {
      fprintf( stdout, "\t### Error unable to find MUX model %s !\n", Model );
    }
    
    if ( ScanMux->SEL == NULL )
    {
      fprintf( stdout, "\t### Error SEL field empty for MUX %s\n", Model );
      Error++;
    }
   
    if ( ScanMux->INPUT_SEL == NULL )           
    {
      fprintf( stdout, "\t### Error INPUT_SEL field empty for MUX %s\n", Model );        
      Error++;
    }

    if ( ScanMux->INPUT_NSEL == NULL )           
    {
      fprintf( stdout, "\t### Error INPUT_NSEL field empty for MUX %s\n", Model );        
      Error++;
    }

    if ( ScanMux->OUTPUT == NULL )
    { 
      fprintf( stdout, "\t### Error OUTPUT filed empty for MUX %s\n", Model );      
      Error++;
    }
    
    if ( ScanMux->VDD == NULL )      
    {
      fprintf( stdout, "\t### Error VDD field empty for MUX %s\n", Model );     
      Error++;
    }
 
    if ( ScanMux->VSS == NULL )
    {
      fprintf( stdout, "\t### Error VSS field empty for MUX %s\n", Model );
      Error++;
    }
  }
      
  for ( ScanReg  = ScanParam->REG_LIST;
        ScanReg != (scanreg_list *)0;
        ScanReg  = ScanReg ->NEXT )
  {
    Model = ScanReg->MODEL;

    if ( ScanReg->MODEL == NULL )
    {
      fprintf( stdout, "\t### Error MODEL field empty in a REG definition\n" );
      Error++;
      Model = "<undefined>";
    }
    else
    if ( ! ScanParseCheckModel( Model ) )
    {
      fprintf( stdout, "\t### Error unable to find REG model %s !\n", Model );
    }
    
    if ( ScanReg->CLK == NULL )
    {
      fprintf( stdout, "\t### Error CLK field empty for REG %s\n", Model );
      Error++;
    }
   
    if ( ScanReg->INPUT == NULL )           
    {
      fprintf( stdout, "\t### Error INPUT field empty for REG %s\n", Model );        
      Error++;
    }

    if ( ScanReg->OUTPUT == NULL )
    { 
      fprintf( stdout, "\t### Error OUTPUT filed empty for REG %s\n", Model );      
      Error++;
    }

    if ( ScanReg->VDD == NULL )      
    {
      fprintf( stdout, "\t### Error VDD field empty for REG %s\n", Model );     
      Error++;
    }
 
    if ( ScanReg->VSS == NULL )
    {
      fprintf( stdout, "\t### Error VSS field empty for REG %s\n", Model );
      Error++;
    }

    if ( ScanReg->MUX == NULL )
    { 
      fprintf( stdout, "\t### Error MUX filed empty for REG %s\n", Model );      
      Error++;
    }
    else
    {
      Element = searchauthelem( HashMux, ScanReg->MUX );
      if ( Element == (authelem *)0 )
      {
        fprintf( stdout, "\t### Error unknown MUX model %s for REG %s\n",
                 ScanReg->MUX, Model );
        Error++;
      }
    }

    if ( ScanReg->REG_MUX == NULL )
    { 
      fprintf( stdout, "\t### Error REG_MUX filed empty for REG %s\n", Model );      
      Error++;
    }
    else
    {
      Element = searchauthelem( HashRegMux, ScanReg->REG_MUX );
      if ( Element == (authelem *)0 )
      {
        fprintf( stdout, "\t### Error unknown REG_MUX model %s for REG %s\n",
                 ScanReg->REG_MUX, Model );
        Error++;
      }
    }
  }

  for ( ScanRegMux  = ScanParam->REG_MUX_LIST;
        ScanRegMux != (scanregmux_list *)0;
        ScanRegMux  = ScanRegMux->NEXT )
  {
    Model = ScanRegMux->MODEL;

    if ( ScanRegMux->MODEL == NULL )
    {
      fprintf( stdout, "\t### Error MODEL field empty in a REG_MUX definition\n" );
      Error++;
      Model = "<undefined>";
    }
    else
    if ( ! ScanParseCheckModel( Model ) )
    {
      fprintf( stdout, "\t### Error unable to find REG_MUX model %s !\n", Model );
    }

    if ( ScanRegMux->CLK == NULL )
    {
      fprintf( stdout, "\t### Error CLK field empty for REG_MUX %s\n", Model );
      Error++;
    }
    
    if ( ScanRegMux->SEL == NULL )
    {
      fprintf( stdout, "\t### Error SEL field empty for REG_MUX %s\n", Model );
      Error++;
    }
   
    if ( ScanRegMux->INPUT_SEL == NULL )           
    {
      fprintf( stdout, "\t### Error INPUT_SEL field empty for REG_MUX %s\n", Model );        
      Error++;
    }

    if ( ScanRegMux->INPUT_NSEL == NULL )           
    {
      fprintf( stdout, "\t### Error INPUT_NSEL field empty for REG_MUX %s\n", Model );        
      Error++;
    }
    
    if ( ScanRegMux->OUTPUT == NULL )
    { 
      fprintf( stdout, "\t### Error OUTPUT filed empty for REG_MUX %s\n", Model );      
      Error++;
    }
    
    if ( ScanRegMux->VDD == NULL )      
    {
      fprintf( stdout, "\t### Error VDD field empty for REG_MUX %s\n", Model );     
      Error++;
    }
 
    if ( ScanRegMux->VSS == NULL )
    {
      fprintf( stdout, "\t### Error VSS field empty for REG_MUX %s\n", Model );
      Error++;
    }                                                       

    if ( ScanRegMux->MUX == NULL )
    { 
      fprintf( stdout, "\t### Error MUX filed empty for REG_MUX %s\n", Model );      
      Error++;
    }
    else
    {
      Element = searchauthelem( HashMux, ScanRegMux->MUX );
      if ( Element == (authelem *)0 )
      {
        fprintf( stdout, "\t### Error unknown MUX model %s for REG_MUX %s\n",
                 ScanRegMux->MUX, Model );
        Error++;
      }
    }

    if ( ScanRegMux->REG == NULL )
    { 
      fprintf( stdout, "\t### Error REG filed empty for REG_MUX %s\n", Model );      
      Error++;
    }
    else
    {
      Element = searchauthelem( HashReg, ScanRegMux->REG );
      if ( Element == (authelem *)0 )
      {
        fprintf( stdout, "\t### Error unknown REG model %s for REG_MUX %s\n",
                 ScanRegMux->REG, Model );
        Error++;
      }
    }
  }

  if ( ScanParam->BUF_LIST == (scanbuf_list *)0 )
  {
    fprintf( stdout, "\t### Error missing buffer declaration !\n" );
    Error++;
  }

  for ( ScanBuf  = ScanParam->BUF_LIST;
        ScanBuf != (scanbuf_list *)0;
        ScanBuf  = ScanBuf ->NEXT )
  {
    Model = ScanBuf->MODEL;

    if ( ScanBuf->MODEL == NULL )
    {
      fprintf( stdout, "\t### Error MODEL field empty in a BUF definition\n" );
      Error++;
      Model = "<undefined>";
    }
    else
    if ( ! ScanParseCheckModel( Model ) )
    {
      fprintf( stdout, "\t### Error unable to find BUF model %s !\n", Model );
    }
    
    if ( ScanBuf->INPUT == NULL )           
    {
      fprintf( stdout, "\t### Error INPUT field empty for BUF %s\n", Model );        
      Error++;
    }

    if ( ScanBuf->OUTPUT == NULL )
    { 
      fprintf( stdout, "\t### Error OUTPUT filed empty for BUF %s\n", Model );      
      Error++;
    }
    
    if ( ScanBuf->VDD == NULL )      
    {
      fprintf( stdout, "\t### Error VDD field empty for BUF %s\n", Model );     
      Error++;
    }
 
    if ( ScanBuf->VSS == NULL )
    {
      fprintf( stdout, "\t### Error VSS field empty for BUF %s\n", Model );
      Error++;
    }
  }

  destroyauthtable( HashMux    );
  destroyauthtable( HashReg    );
  destroyauthtable( HashRegMux );

  if ( Error )
  {
    fprintf( stdout, "\t### Found %d error(s), exit !\n", Error );
    autexit(1);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       ScanParseParamFile                    |
|                                                             |
\------------------------------------------------------------*/

scanparam *ScanParseParamFile( FileName, FlagDebug )

  char *FileName;
  int   FlagDebug;
{
  scanparam        *ScanParam;
  scanmux_list     *ScanMux;
  scanreg_list     *ScanReg;
  scanregmux_list  *ScanRegmux;
  scanbuf_list     *ScanBuf;
  char             *FirstWord;
  char             *SecondWord;
  int               CurrentDef;
  long              LineNumber;
  FILE             *ParamFile;
  char              ParamBuffer[ 512 ];

#if 0 /*error: autbasename() removes the path.
        easier not to touch the filename*/
  FileName  = autbasename( FileName, "scapin" );
  sprintf( ParamBuffer, "%s.scapin", FileName );
#endif

  ParamFile = fopen( FileName, "r" );

  if ( ParamFile == (FILE *)0 )
  {
    fprintf( stderr, "\t### Unable to open parameter file %s\n", FileName );
    autexit( 1 );
  }

  ScanParam  = (scanparam       *)autallocblock( sizeof( scanparam ) );
  ScanMux    = (scanmux_list    *)0;
  ScanReg    = (scanreg_list    *)0;
  ScanRegmux = (scanregmux_list *)0;
  ScanBuf    = (scanbuf_list    *)0;
  LineNumber = 0;

  CurrentDef = SCAN_PARAM_NONE;

  while ( fgets( ParamBuffer, 512, ParamFile ) != (char *)0 )
  {
    LineNumber++;

    if ( ParamBuffer[ 0 ] == '#' ) continue;

    FirstWord = ScanParseGetWord( ParamBuffer ); 

    if ( FirstWord == (char *)0 ) continue;

    if ( ! strcmp( FirstWord, "end_mux" ) ) CurrentDef = SCAN_PARAM_NONE;
    else
    if ( ! strcmp( FirstWord, "end_reg" ) ) CurrentDef = SCAN_PARAM_NONE;
    else
    if ( ! strcmp( FirstWord, "end_reg_mux" ) ) CurrentDef = SCAN_PARAM_NONE;  
    else
    if ( ! strcmp( FirstWord, "end_buf" ) ) CurrentDef = SCAN_PARAM_BUF;  
    else
        
    if ( CurrentDef != SCAN_PARAM_NONE )
    {
      SecondWord = ScanParseGetWord( (char *)0 );

      if ( SecondWord == (char *)0 )
      {
        fprintf( stderr, "Syntax error line %ld in parameter file %s.scan\n",
                 LineNumber, FileName  );
        autexit( 1 );
      }

      switch ( CurrentDef )
      {
        case SCAN_PARAM_MUX :
         
          if ( ! strcmp( FirstWord, "mux_model" ) )
          {
            ScanMux->MODEL = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "mux_sel" ) )
          {
            ScanMux->SEL = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "mux_input_sel" ) )
          {   
            ScanMux->INPUT_SEL = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "mux_input_nsel" ) ) 
          {    
            ScanMux->INPUT_NSEL = SecondWord;     
          }
          else
          if ( ! strcmp( FirstWord, "mux_vdd" ) )
          {
            ScanMux->VDD = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "mux_vss" ) )
          {
            ScanMux->VSS = SecondWord;  
          }
          else  
          if ( ! strcmp( FirstWord, "mux_output" ) )
          {
            ScanMux->OUTPUT = SecondWord;
          }
          else
          {
            fprintf( stderr, "Syntax error line %ld in parameter file %s.scan\n",
                     LineNumber, FileName  );
            autexit( 1 );
          }
          

        break;

        case SCAN_PARAM_REG :
         
          if ( ! strcmp( FirstWord, "reg_model" ) )
          {
            ScanReg->MODEL= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_clk" ) )                      
          {    
            ScanReg->CLK= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_input" ) )
          {
            ScanReg->INPUT= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_vdd" ) )
          {
            ScanReg->VDD= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_vss" ) )
          {
            ScanReg->VSS= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_output" ) ) 
          {
            ScanReg->OUTPUT= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux" ) ) 
          {
            ScanReg->MUX= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_reg_mux" ) ) 
          {
            ScanReg->REG_MUX= SecondWord;
          }
          else
          {
            fprintf( stderr, "Syntax error line %ld in parameter file %s.scan\n",
                     LineNumber, FileName  );
            autexit( 1 );
          }
          
          break;
          
               case SCAN_PARAM_BUF :
         
          if ( ! strcmp( FirstWord, "buf_model" ) ) 
          {
           ScanBuf ->MODEL= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "buf_input" ) )
          {
            ScanBuf->INPUT= SecondWord;
          }
          else  
          if ( ! strcmp( FirstWord, "buf_output" ) )        
          {
            ScanBuf->OUTPUT = SecondWord; 
          }
          else
          if ( ! strcmp( FirstWord, "buf_vdd" ) )
          {
            ScanBuf->VDD= SecondWord;
          }
          else    
          if ( ! strcmp( FirstWord, "buf_vss" ) )        
          {
            ScanBuf->VSS= SecondWord; 
          }
          else
          {
            fprintf( stderr, "Syntax error line %ld in parameter file %s.scan\n",
                     LineNumber, FileName  );
            autexit( 1 );
          }
          

        break;

        case SCAN_PARAM_REG_MUX :
         
          if ( ! strcmp( FirstWord, "reg_mux_model" ) ) 
          {
            ScanRegmux->MODEL= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_sel" ) )
          {
            ScanRegmux->SEL= SecondWord;   
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_input_sel" ) )
          {
            ScanRegmux->INPUT_SEL= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_input_nsel" ) )
          {
            ScanRegmux->INPUT_NSEL= SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_clk" ) )
          {
            ScanRegmux->CLK= SecondWord;  
          }
          else  
          if ( ! strcmp( FirstWord, "reg_mux_output" ) )        
          {
            ScanRegmux->OUTPUT = SecondWord; 
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_vdd" ) )
          {
            ScanRegmux->VDD= SecondWord;
          }
          else    
          if ( ! strcmp( FirstWord, "reg_mux_vss" ) )        
          {
            ScanRegmux->VSS= SecondWord; 
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_mux" ) ) 
          {
            ScanRegmux->MUX = SecondWord;
          }
          else
          if ( ! strcmp( FirstWord, "reg_mux_reg" ) ) 
          {
            ScanRegmux->REG = SecondWord;
          }
          else
          {
            fprintf( stderr, "Syntax error line %ld in parameter file %s.scan\n",
                     LineNumber, FileName  );
            autexit( 1 );
          }
          
        break;
      }
    }
    else
    if ( ! strcmp( FirstWord, "begin_mux" ) )
    {
      ScanMux = (scanmux_list *)autallocblock( sizeof( scanmux_list ) );

      ScanMux->NEXT       = ScanParam->MUX_LIST;
      ScanParam->MUX_LIST = ScanMux;

      CurrentDef = SCAN_PARAM_MUX;
    }
    else
    if ( ! strcmp( FirstWord, "begin_reg" ) )
    {
      ScanReg = (scanreg_list *)autallocblock( sizeof( scanreg_list ) );

      ScanReg->NEXT       = ScanParam->REG_LIST;
      ScanParam->REG_LIST = ScanReg;

      CurrentDef = SCAN_PARAM_REG;
    }
    else
    if ( ! strcmp( FirstWord, "begin_reg_mux" ) )
    { 
        ScanRegmux = (scanregmux_list *)autallocblock( sizeof( scanregmux_list ) );
   
        ScanRegmux->NEXT        = ScanParam->REG_MUX_LIST;
        ScanParam->REG_MUX_LIST = ScanRegmux;
   
        CurrentDef = SCAN_PARAM_REG_MUX;
    }
    else
    if( ! strcmp( FirstWord, "begin_buf" ) )
    {
      ScanBuf = (scanbuf_list *)autallocblock( sizeof(scanbuf_list) );

      ScanBuf->NEXT       = ScanParam->BUF_LIST;
      ScanParam->BUF_LIST = ScanBuf;
   
      CurrentDef = SCAN_PARAM_BUF;
    }
  }

  fclose( ParamFile  );

  if ( FlagDebug )
  {
    ScanParseViewParam( ScanParam );
  }

  ScanParseVerifyParam( ScanParam );

  return( ScanParam );
}

