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
| Tool    :                    DREAL                          |
|                                                             |
| File    :                  GsbParse.c                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   28.03.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdlib.h>
# include <string.h>

# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "GSB.h"

# include "GSB_error.h"
# include "GSB_parse.h"

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
|                        Global variables                     |
|                                                             |
\------------------------------------------------------------*/

  char  *DREAL_TECHNO_NAME = (char *)NULL;

/*------------------------------------------------------------\
|                                                             |
|                       Privates Variables                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Table variables                       |
|                                                             |
\------------------------------------------------------------*/

  char  *DREAL_RDS_LAYER_NAME_TABLE[ RDS_ALL_LAYER ][ 3 ];
  int    DREAL_RDS_LAYER_PATTERN_TABLE[ RDS_ALL_LAYER ];
  char  *DREAL_CURSOR_COLOR_NAME;

  float  DREAL_LOWER_GRID_STEP;
  float  DREAL_LOWER_FIGURE_STEP;
  float  DREAL_LOWER_INSTANCE_STEP;
  float  DREAL_LOWER_CONNECTOR_STEP;
  float  DREAL_LOWER_SEGMENT_STEP;
  float  DREAL_LOWER_REFERENCE_STEP;
  long   DREAL_CURSOR_SIZE;
  float  DREAL_RDS_UNIT_TO_MICRON;
  float  DREAL_PHYSICAL_GRID_TO_MICRON;

/*------------------------------------------------------------\
|                                                             |
|                      Keywords variables                     |
|                                                             |
\------------------------------------------------------------*/

  static char  KeywordDefined = 0;

  static char *DefineKeyword;
  static char *TableKeyword;
  static char *EndTableKeyword;
  static char *EndRecordKeyword;
  static char *RdsLayerNameKeyword;
  static char *LowerGridStepKeyword;
  static char *LowerFigureStepKeyword;
  static char *LowerInstanceStepKeyword;
  static char *LowerSegmentStepKeyword;
  static char *LowerReferenceStepKeyword;
  static char *LowerConnectorStepKeyword;
  static char *CursorColorNameKeyword;
  static char *CursorSizeKeyword;

  static keyword KeywordDefine[ GSB_MAX_KEYWORD ] =
 
  {
    { "rds_abox",     RDS_LAYER_ABOX   },
    { "rds_activ",    RDS_LAYER_ACTIV  },
    { "rds_alu1",     RDS_LAYER_ALU1   },
    { "rds_alu2",     RDS_LAYER_ALU2   },
    { "rds_alu3",     RDS_LAYER_ALU3   },
    { "rds_alu4",     RDS_LAYER_ALU4   },
    { "rds_alu5",     RDS_LAYER_ALU5   },
    { "rds_alu6",     RDS_LAYER_ALU6   },
    { "rds_alu7",     RDS_LAYER_ALU7   },
    { "rds_alu8",     RDS_LAYER_ALU8   },
    { "rds_alu9",     RDS_LAYER_ALU9   },
    { "rds_cont",     RDS_LAYER_CONT   },
    { "rds_cont2",    RDS_LAYER_CONT2  },
    { "rds_cpas",     RDS_LAYER_CPAS   },
    { "rds_gate",     RDS_LAYER_GATE   },
    { "rds_ndif",     RDS_LAYER_NDIF   },
    { "rds_nimp",     RDS_LAYER_NIMP   },
    { "rds_ntie",     RDS_LAYER_NTIE   },
    { "rds_nwell",    RDS_LAYER_NWELL  },
    { "rds_pdif",     RDS_LAYER_PDIF   },
    { "rds_pimp",     RDS_LAYER_PIMP   },
    { "rds_poly",     RDS_LAYER_POLY   },
    { "rds_poly2",    RDS_LAYER_POLY2  },
    { "rds_ptie",     RDS_LAYER_PTIE   },
    { "rds_pwell",    RDS_LAYER_PWELL  },
    { "rds_ref",      RDS_LAYER_REF    },
    { "rds_talu1",    RDS_LAYER_TALU1  },
    { "rds_talu2",    RDS_LAYER_TALU2  },
    { "rds_talu3",    RDS_LAYER_TALU3  },
    { "rds_talu4",    RDS_LAYER_TALU4  },
    { "rds_talu5",    RDS_LAYER_TALU5  },
    { "rds_talu6",    RDS_LAYER_TALU6  },
    { "rds_talu7",    RDS_LAYER_TALU7  },
    { "rds_talu8",    RDS_LAYER_TALU8  },
    { "rds_tcont",    RDS_LAYER_TCONT  },
    { "rds_tpoly",    RDS_LAYER_TPOLY  },
    { "rds_tpoly2",   RDS_LAYER_TPOLY2 },
    { "rds_tvia1",    RDS_LAYER_TVIA1  },
    { "rds_tvia2",    RDS_LAYER_TVIA2  },
    { "rds_tvia3",    RDS_LAYER_TVIA3  },
    { "rds_tvia4",    RDS_LAYER_TVIA4  },
    { "rds_tvia5",    RDS_LAYER_TVIA5  },
    { "rds_tvia6",    RDS_LAYER_TVIA6  },
    { "rds_tvia7",    RDS_LAYER_TVIA7  },
    { "rds_tvia8",    RDS_LAYER_TVIA8  },
    { "rds_user0",    RDS_LAYER_USER0  },
    { "rds_user1",    RDS_LAYER_USER1  },
    { "rds_user2",    RDS_LAYER_USER2  },
    { "rds_user3",    RDS_LAYER_USER3  },
    { "rds_user4",    RDS_LAYER_USER4  },
    { "rds_user5",    RDS_LAYER_USER5  },
    { "rds_user6",    RDS_LAYER_USER6  },
    { "rds_user7",    RDS_LAYER_USER7  },
    { "rds_user8",    RDS_LAYER_USER8  },
    { "rds_user9",    RDS_LAYER_USER9  },
    { "rds_valu1",    RDS_LAYER_VALU1  },
    { "rds_valu2",    RDS_LAYER_VALU2  },
    { "rds_valu3",    RDS_LAYER_VALU3  },
    { "rds_valu4",    RDS_LAYER_VALU4  },
    { "rds_valu5",    RDS_LAYER_VALU5  },
    { "rds_valu6",    RDS_LAYER_VALU6  },
    { "rds_valu7",    RDS_LAYER_VALU7  },
    { "rds_valu8",    RDS_LAYER_VALU8  },
    { "rds_via1",     RDS_LAYER_VIA1   },
    { "rds_via2",     RDS_LAYER_VIA2   },
    { "rds_via3",     RDS_LAYER_VIA3   },
    { "rds_via4",     RDS_LAYER_VIA4   },
    { "rds_via5",     RDS_LAYER_VIA5   },
    { "rds_via6",     RDS_LAYER_VIA6   },
    { "rds_via7",     RDS_LAYER_VIA7   },
    { "rds_via8",     RDS_LAYER_VIA8   },
    { "rds_vpoly",    RDS_LAYER_VPOLY  },
    { "ref_con",      MBK_REF_CON      },
    { "ref_ref",      MBK_REF_REF      }
  };

/*------------------------------------------------------------\
|                                                             |
|                       File variables                        |
|                                                             |
\------------------------------------------------------------*/

  static  FILE *GsbFile;
  static  char  GsbBuffer[ GSB_MAX_BUFFER ];
  static  long  GsbCurrentLine;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Gsb File Get String                   |
|                                                             |
\------------------------------------------------------------*/

char *GsbFileGetString( String, Size )

  char *String;
  int   Size;
{
  register char *RegisterString;
  register       Register;

  rdsbegin();

  RegisterString = String;

  while (--Size > 0 && (Register = getc( GsbFile )) != EOF )
  {
    if ((*RegisterString++ = Register) == '\\') 
    {
      if ((Register = getc( GsbFile )) == '\n') 
      {
        *(RegisterString - 1) = ' ';
      } 
      else
      {
        ungetc( Register, GsbFile );
      }
    }
    else
    {
      if ( Register == '\n') break;
    }
  }

  *RegisterString = '\0';

  rdsend();
  return ( ( Register       == EOF    ) && 
           ( RegisterString == String ) ) ? (char *)NULL : String;
}

/*------------------------------------------------------------\
|                                                             |
|                       Gsb File Get Line                     |
|                                                             |
\------------------------------------------------------------*/

void GsbGetLine( Buffer )

  char   *Buffer;
{
  char   *Check;
  char   *String;
  char    OneComment;

  rdsbegin();

  do 
  {
    OneComment = 0;

    Check = GsbFileGetString( Buffer, GSB_MAX_BUFFER );

    if ( Check != (char *)NULL ) 
    {
      GsbCurrentLine++;
    }
    else 
    {
      GsbError( UNEXPECTED_EOF, (char *)NULL, GsbCurrentLine );
    }

    if ( String = strchr( Buffer, GSB_COMMENT_CHAR ))
    {
      if ( String == Buffer )
      {
        OneComment = 1;
      }
      else
      {
        *(String - 1) = '\0'; 
      }
    }

    while (*Buffer != '\0' && strchr( GSB_SEPARATORS_STRING, *Buffer))
    {
      Buffer = Buffer + 1;;
    }

    if (*Buffer == '\0') OneComment = 1;

  } 
  while ( OneComment == 1);

  rdsend();
}      

/*------------------------------------------------------------\
|                                                             |
|                        Gsb Check Keyword                    |
|                                                             |
\------------------------------------------------------------*/

char GsbCheckKeyword( Word )

   char *Word;
{
  if ( ( Word == DefineKeyword ) ||
       ( Word == TableKeyword  ) )
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Gsb Get First word                   |
|                                                             |
\------------------------------------------------------------*/

char *GsbGetFirstWord( Buffer, IsKeyword, Hash )

      char *Buffer;
      char  IsKeyword;
      char  Hash;
{
  char   *String;
  char   *Text;
  int     Index;

  rdsbegin();

  if ( String = (char *)strtok( Buffer, GSB_SEPARATORS_STRING )) 
  {
    if ( Hash )
    {
      String = namealloc( String );
    }
    else
    {
      Text = malloc( strlen( String ) + 1 );
      strcpy( Text, String );
      String = Text;

      for ( Index = 0; Text[ Index ]; Index++ )
      {
        if ( Text[ Index ] == '_' ) Text[ Index ] = ' '; 
      }
    }

    if ( ( IsKeyword ) && GsbCheckKeyword( String ) )
    {
      GsbError( UNEXPECTED_LINE, String, GsbCurrentLine );
    }
  }

  rdsend();
  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                        Gsb Get Next word                    |
|                                                             |
\------------------------------------------------------------*/

char *GsbGetNextWord( IsKeyword, Hash )

      char IsKeyword;
      char Hash;
{
  char   *String;
  char   *Text;
  int     Index;

  rdsbegin();

  if ( String = (char *)strtok( (char *)NULL, GSB_SEPARATORS_STRING )) 
  {
     if ( Hash )
     {
       String = namealloc( String );
     }
     else
     {
       Text = malloc( strlen( String ) + 1 );
       strcpy( Text, String );
       String = Text;

       for ( Index = 0; Text[ Index ]; Index++ )
       {
         if ( Text[ Index ] == '_' ) Text[ Index ] = ' ';
       }
     }
 
     if ( ( IsKeyword ) && GsbCheckKeyword( String ) )
     {
       GsbError( UNEXPECTED_LINE, String, GsbCurrentLine );
     }
  }


  rdsend();
  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Keyword Compare                    |
|                                                             |
\------------------------------------------------------------*/

 int GsbKeywordCompare( FirstKey, SecondKey )

     keyword *FirstKey;
     keyword *SecondKey;
 {
   return strcmp( FirstKey->NAME, SecondKey->NAME );
 }

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Get String Value                   |
|                                                             |
\------------------------------------------------------------*/

long GsbGetStringValue( String )

   char     *String;
{
  long      Value;
  keyword  *Keyword;
  keyword   Entry;

  rdsbegin();

  if ( sscanf( String, "%d", &Value) )
  {
    rdsend();
    return ( Value );
  }

  Entry.NAME = String;

  Keyword = (keyword *)bsearch( (char *)(&Entry), 
                                (char *)KeywordDefine, 
                                GSB_MAX_KEYWORD, sizeof( keyword ),
                                GsbKeywordCompare );

  if ( Keyword == (keyword *)NULL )
  {
    GsbError( UNEXPECTED_LINE, String, GsbCurrentLine );
  }

  rdsend();
  return( Keyword->VALUE );
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Get String Float                   |
|                                                             |
\------------------------------------------------------------*/

float GsbGetStringFloat( String )

   char     *String;
{
  float     Value;
  keyword  *Keyword;
  keyword   Entry;

  rdsbegin();

  if ( ! sscanf( String, "%g", &Value) )
  {
    GsbError( ILLEGAL_FLOAT, String, GsbCurrentLine );
  }

  rdsend();
  return ( Value );
}

/*------------------------------------------------------------\
|                                                             |
|                        Gsb Get Number                       |
|                                                             |
\------------------------------------------------------------*/

long GsbGetNumber( String )

     char *String;
{
  long Value;

  rdsbegin();

  if ( ! sscanf( String, "%d", &Value ))
  {
    GsbError( UNEXPECTED_LINE, "number", GsbCurrentLine );
  }

  rdsend();
  return Value;
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Rds Layer Name                  |
|                                                             |
\------------------------------------------------------------*/

void GsbReadRdsLayerName()

{
  char  Layer;
  char  Pattern;
  char  LayerCount;
  char  EndTable;
  char  Field;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;

  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    GsbGetLine( GsbBuffer );

    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer = GsbGetStringValue( FirstWord );

      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 1, 0 );

        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED )
        {
          DREAL_RDS_LAYER_NAME_TABLE [ Layer ][ Field ] = FirstWord;
        }
      }
   
      FirstWord = GsbGetNextWord( 0, 1 );

      if ( FirstWord != EndRecordKeyword )
      {
        Pattern = GsbGetStringValue( FirstWord );

        if ( RDS_DYNAMIC_LAYER[ Layer ] != RDS_LAYER_UNUSED )
        {
          DREAL_RDS_LAYER_PATTERN_TABLE[ Layer ] = (int)Pattern;
        }

        FirstWord = GsbGetNextWord( 0, 1 );
        if ( FirstWord != EndRecordKeyword )
        {
          GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
        }
      }
    }

    LayerCount = LayerCount + 1;
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Read Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void GsbReadParam()

{
  char *FirstWord;
  char *SecondWord;
  long  Continue;

  rdsbegin();

  Continue = 0;

  while ( Continue != GSB_ALL_DEFINED_MASK )  
  {
    GsbGetLine( GsbBuffer );

    FirstWord = GsbGetFirstWord( GsbBuffer, 0, 1);

    if ( FirstWord == DefineKeyword ) 
    {
      FirstWord = GsbGetNextWord( 1, 1 );

      if (! FirstWord )
      {
        GsbError( MISSING_NAME, DefineKeyword, GsbCurrentLine );
      }

      SecondWord = GsbGetNextWord( 1, 0 );

      if (! SecondWord )
      {
        GsbError( MISSING_VALUE, SecondWord );
      }

      if ( FirstWord == LowerGridStepKeyword )
      {
        DREAL_LOWER_GRID_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_GRID_STEP_MASK;
      }
      else
      if ( FirstWord == LowerFigureStepKeyword )
      {
        DREAL_LOWER_FIGURE_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_FIGURE_STEP_MASK;
      }
      else
      if ( FirstWord == LowerInstanceStepKeyword )
      {
        DREAL_LOWER_INSTANCE_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_INSTANCE_STEP_MASK;
      }
      else
      if ( FirstWord == LowerSegmentStepKeyword )
      {
        DREAL_LOWER_SEGMENT_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_SEGMENT_STEP_MASK;
      }
      else
      if ( FirstWord == LowerConnectorStepKeyword )
      {
        DREAL_LOWER_CONNECTOR_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_CONNECTOR_STEP_MASK;
      }
      else
      if ( FirstWord == LowerReferenceStepKeyword )
      {
        DREAL_LOWER_REFERENCE_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_REFERENCE_STEP_MASK;
      }
      else
      if ( FirstWord == CursorColorNameKeyword )
      {
        DREAL_CURSOR_COLOR_NAME = namealloc( SecondWord );

        Continue |= GSB_CURSOR_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == CursorSizeKeyword )
      {
        DREAL_CURSOR_SIZE = GsbGetStringValue( SecondWord );

        Continue |= GSB_CURSOR_SIZE_MASK;
      }
      else
      {
        GsbError( UNKNOWN_DEFINE, FirstWord, GsbCurrentLine );
      }
    } 
    else 
    if ( FirstWord == TableKeyword )
    {
      if (!( FirstWord = GsbGetNextWord(1, 1)))
      {
        GsbError( MISSING_NAME, TableKeyword );
      }

      if ( FirstWord == RdsLayerNameKeyword )
      {
        GsbReadRdsLayerName();

        Continue |= GSB_RDS_LAYER_NAME_MASK;
      }
      else
      {
        GsbError( UNKNOWN_TABLE, FirstWord, GsbCurrentLine );
      }
    } 
    else
    { 
      GsbError( SYNTAX_ERROR, FirstWord, GsbCurrentLine );
    } 
  }       

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Load Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void DrealLoadParameters()

{
  char  Layer;
  char  Buffer[ 512 ];
  char *String;

  rdsbegin();

  DREAL_TECHNO_NAME = mbkgetenv( "DREAL_TECHNO_NAME" );

  if ( DREAL_TECHNO_NAME == (char *)NULL )
  {
    String = mbkgetenv( "ALLIANCE_TOP" );
    if ( String == (char *)NULL ) String = ALLIANCE_TOP;

    sprintf( Buffer, "%s/%s", String, DREAL_DEFAULT_TECHNO_NAME );
    DREAL_TECHNO_NAME = mbkstrdup( Buffer );
  }

  if ( ! KeywordDefined )
  {
    DefineKeyword             = namealloc( DEFINE_KEYWORD               );
    TableKeyword              = namealloc( TABLE_KEYWORD                );
    EndTableKeyword           = namealloc( END_TABLE_KEYWORD            );
    EndRecordKeyword          = namealloc( END_RECORD_KEYWORD           );
    RdsLayerNameKeyword       = namealloc( RDS_LAYER_NAME_KEYWORD       );
    LowerGridStepKeyword      = namealloc( LOWER_GRID_STEP_KEYWORD      );
    LowerFigureStepKeyword    = namealloc( LOWER_FIGURE_STEP_KEYWORD    );
    LowerInstanceStepKeyword  = namealloc( LOWER_INSTANCE_STEP_KEYWORD  );
    LowerReferenceStepKeyword = namealloc( LOWER_REFERENCE_STEP_KEYWORD );
    LowerSegmentStepKeyword   = namealloc( LOWER_SEGMENT_STEP_KEYWORD   );
    LowerConnectorStepKeyword = namealloc( LOWER_CONNECTOR_STEP_KEYWORD );
    CursorColorNameKeyword    = namealloc( CURSOR_COLOR_NAME_KEYWORD    );
    CursorSizeKeyword         = namealloc( CURSOR_SIZE_KEYWORD          );

    KeywordDefined = 1;
  }

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    DREAL_RDS_LAYER_NAME_TABLE[ Layer ][0] = (char *)NULL;
    DREAL_RDS_LAYER_NAME_TABLE[ Layer ][1] = (char *)NULL;
    DREAL_RDS_LAYER_NAME_TABLE[ Layer ][2] = (char *)NULL;
    DREAL_RDS_LAYER_PATTERN_TABLE[ Layer ] = -1;
  }

  DREAL_LOWER_GRID_STEP       = 0.0;
  DREAL_LOWER_FIGURE_STEP     = 0.0;
  DREAL_LOWER_INSTANCE_STEP   = 0.0;
  DREAL_LOWER_CONNECTOR_STEP  = 0.0;
  DREAL_LOWER_SEGMENT_STEP    = 0.0;
  DREAL_LOWER_REFERENCE_STEP  = 0.0;
  DREAL_CURSOR_COLOR_NAME     = (char *)NULL;
  DREAL_CURSOR_SIZE           = 5;

  if ( !( GsbFile = fopen( DREAL_TECHNO_NAME, "r")))

    GsbError( OPEN_FILE, NULL, 0);

  GsbReadParam();

  fclose( GsbFile );

  DREAL_RDS_UNIT_TO_MICRON      = 1.0 / (float)( RDS_UNIT );
  DREAL_PHYSICAL_GRID_TO_MICRON = (float)( RDS_PHYSICAL_GRID ) / (float)( RDS_UNIT );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb View Parameters                    |
|                                                             |
\------------------------------------------------------------*/

# ifdef DEBUG

void DrealViewParameters()

{
  char Layer;

  rdsbegin();

  fprintf( stdout, "\nRDS_LAYER TABLE\n" );

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s %s %s", 
             Layer, 
             DREAL_RDS_LAYER_NAME_TABLE[ Layer ][0],
             DREAL_RDS_LAYER_NAME_TABLE[ Layer ][1],
             DREAL_RDS_LAYER_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  rdsend();
}

# endif
