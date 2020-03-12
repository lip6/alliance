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
| Tool    :                    GRAAL                          |
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
|                       Global Variables                      |
|                                                             |
\------------------------------------------------------------*/

  char *GRAAL_TECHNO_NAME = (char *)NULL;

  long  GRAAL_RDS_LAMBDA  = 0;
  float GRAAL_SCALE_X     = 0.0;
  long  GRAAL_SCALE       = 1;

/*------------------------------------------------------------\
|                                                             |
|                      Private Variables                      |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       Table variables                       |
|                                                             |
\------------------------------------------------------------*/

  char  *GRAAL_VIA_NAME_TABLE[ MBK_MAX_VIA ][ 3 ];
  char  *GRAAL_BIGVIA_NAME_TABLE[ MBK_MAX_VIA ][ 3 ];
  char  *GRAAL_SYMMETRY_NAME_TABLE[ MBK_MAX_SYMMETRY ][ 3 ];
  char  *GRAAL_ORIENT_NAME_TABLE[ MBK_MAX_ORIENT ][ 3 ];
  char  *GRAAL_REFERENCE_NAME_TABLE[ MBK_MAX_REFERENCE ][ 3 ];
  char  *GRAAL_SEGMENT_NAME_TABLE[ MBK_MAX_LAYER ][ 3 ];
  char  *GRAAL_TRANSISTOR_NAME_TABLE[ MBK_MAX_LAYER ][ 3 ];
  char  *GRAAL_CONNECTOR_NAME_TABLE[ MBK_MAX_LAYER ][ 3 ];
  char  *GRAAL_RDS_LAYER_NAME_TABLE[ RDS_ALL_LAYER ][ 3 ];
  char  *GRAAL_CURSOR_COLOR_NAME;
  int    GRAAL_RDS_LAYER_PATTERN_TABLE[ RDS_ALL_LAYER ];

  float  GRAAL_SEGMENT_VALUE_TABLE[ MBK_MAX_LAYER ][ 2 ];
  float  GRAAL_BIGVIA_VALUE_TABLE[ MBK_MAX_LAYER ][ 1 ];
  long   GRAAL_PEEK_BOUND;
  long   GRAAL_CURSOR_SIZE;

  float  GRAAL_LOWER_GRID_STEP;
  float  GRAAL_LOWER_FIGURE_STEP;
  float  GRAAL_LOWER_INSTANCE_STEP;
  float  GRAAL_LOWER_CONNECTOR_STEP;
  float  GRAAL_LOWER_SEGMENT_STEP;
  float  GRAAL_LOWER_REFERENCE_STEP;

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
  static char *PeekBoundKeyword;
  static char *LowerGridStepKeyword;
  static char *LowerFigureStepKeyword;
  static char *LowerInstanceStepKeyword;
  static char *LowerSegmentStepKeyword;
  static char *LowerReferenceStepKeyword;
  static char *LowerConnectorStepKeyword;
  static char *SegmentNameKeyword;
  static char *TransistorNameKeyword;
  static char *ConnectorNameKeyword;
  static char *SegmentValueKeyword;
  static char *ViaNameKeyword;
  static char *BigViaNameKeyword;
  static char *BigViaValueKeyword;
  static char *SymmetryNameKeyword;
  static char *OrientNameKeyword;
  static char *ReferenceNameKeyword;
  static char *RdsLayerNameKeyword;
  static char *CursorColorNameKeyword;
  static char *CursorSizeKeyword;


  static keyword GsbKeywordDefine[ GSB_MAX_KEYWORD ] =
 

  {
    { "alu1",         ALU1             },
    { "alu2",         ALU2             },
    { "alu3",         ALU3             },
    { "alu4",         ALU4             },
    { "alu5",         ALU5             },
    { "alu6",         ALU6             },
    { "alu7",         ALU7             },
    { "alu8",         ALU8             },
    { "alu9",         ALU9             },
    { "c_x_n",        C_X_N            },
    { "c_x_p",        C_X_P            },
    { "calu1",        CALU1            },
    { "calu2",        CALU2            },
    { "calu3",        CALU3            },
    { "calu4",        CALU4            },
    { "calu5",        CALU5            },
    { "calu6",        CALU6            },
    { "calu7",        CALU7            },
    { "calu8",        CALU8            },
    { "calu9",        CALU9            },
    { "cont_body_n",  CONT_BODY_N      },
    { "cont_body_p",  CONT_BODY_P      },
    { "cont_dif_n",   CONT_DIF_N       },
    { "cont_dif_p",   CONT_DIF_P       },
    { "cont_poly",    CONT_POLY        },
    { "cont_poly2",   CONT_POLY2       },
    { "cont_turn1",   CONT_TURN1       },
    { "cont_turn2",   CONT_TURN2       },
    { "cont_turn3",   CONT_TURN3       },
    { "cont_turn4",   CONT_TURN4       },
    { "cont_turn5",   CONT_TURN5       },
    { "cont_turn6",   CONT_TURN6       },
    { "cont_turn7",   CONT_TURN7       },
    { "cont_turn8",   CONT_TURN8       },
    { "cont_turn9",   CONT_TURN9       },
    { "cont_via",     CONT_VIA         },
    { "cont_via2",    CONT_VIA2        },
    { "cont_via3",    CONT_VIA3        },
    { "cont_via4",    CONT_VIA4        },
    { "cont_via5",    CONT_VIA5        },
    { "cont_via6",    CONT_VIA6        },
    { "cont_via7",    CONT_VIA7        },
    { "cont_via8",    CONT_VIA8        },
    { "east",         GRAAL_EAST       }, 
    { "ndif",         NDIF             },
    { "north",        GRAAL_NORTH      }, 
    { "nosym",        NOSYM            }, 
    { "ntie",         NTIE             },
    { "ntrans",       NTRANS           },
    { "ntrans_fast",  NTRANS_FAST      },
    { "ntrans_hvio",  NTRANS_HVIO      },
    { "nwell",        NWELL            },
    { "pdif",         PDIF             },
    { "poly",         POLY             },
    { "poly2",        POLY2            },
    { "ptie",         PTIE             },
    { "ptrans",       PTRANS           },
    { "ptrans_fast",  PTRANS_FAST      },
    { "ptrans_hvio",  PTRANS_HVIO      },
    { "pwell",        PWELL            },
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
    { "ref_ref",      MBK_REF_REF      },
    { "rot_m",        ROT_M            },
    { "rot_p",        ROT_P            },
    { "south",        GRAAL_SOUTH      }, 
    { "sy_rm",        SY_RM            }, 
    { "sy_rp",        SY_RP            }, 
    { "sym_x",        SYM_X            },
    { "sym_y",        SYM_Y            },
    { "symxy",        SYMXY            },
    { "talu1",        TALU1            },
    { "talu2",        TALU2            },
    { "talu3",        TALU3            },
    { "talu4",        TALU4            },
    { "talu5",        TALU5            },
    { "talu6",        TALU6            },
    { "talu7",        TALU7            },
    { "talu8",        TALU8            },
    { "tpoly",        TPOLY            },
    { "tpoly2",       TPOLY2           },
    { "west",         GRAAL_WEST       }
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
  register int   Register = 0;

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
 
    if ( (String = strchr( Buffer, GSB_COMMENT_CHAR )) )
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
 
  if ( (String = (char *)strtok( Buffer, GSB_SEPARATORS_STRING )) ) 
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
 
  if ( (String = (char *)strtok( (char *)NULL, GSB_SEPARATORS_STRING )) ) 
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
 
  if ( sscanf( String, "%ld", &Value) )
  {
    rdsend();
    return ( Value );
  }

  Entry.NAME = String;
 
  Keyword = (keyword *)bsearch( (char *)(&Entry), 
                                (char *)GsbKeywordDefine, 
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

  if ( ! sscanf( String, "%22ld", &Value ))
  {
    GsbError( UNEXPECTED_LINE, "number", GsbCurrentLine );
  }

  rdsend();
  return Value;
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Segment Name                    |
|                                                             |
\------------------------------------------------------------*/

void GsbReadSegmentName()

{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( GET_SEGMENT_LAYER( Layer, 0 ) != RDS_SEGMENT_EMPTY )

        {
          GRAAL_SEGMENT_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
 
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Transistor Name                 |
|                                                             |
\------------------------------------------------------------*/

void GsbReadTransistorName()

{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( GET_SEGMENT_LAYER( Layer, 0 ) != RDS_SEGMENT_EMPTY )
        {
          GRAAL_TRANSISTOR_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
 
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Connector Name                  |
|                                                             |
\------------------------------------------------------------*/

void GsbReadConnectorName()

{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( GET_SEGMENT_LAYER( Layer, 0 ) != RDS_SEGMENT_EMPTY )
        {
          GRAAL_CONNECTOR_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
 
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Segment Value                   |
|                                                             |
\------------------------------------------------------------*/

void GsbReadSegmentValue()

{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 2; Field++ ) 
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( GET_SEGMENT_LAYER( Layer, 0 ) != RDS_SEGMENT_EMPTY )
        {
          GRAAL_SEGMENT_VALUE_TABLE [ (int)Layer ][ (int)Field ] = GsbGetStringFloat( FirstWord );
        }
      }
 
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read BigVia  Value                   |
|                                                             |
\------------------------------------------------------------*/

void GsbReadBigViaValue()

{
  char  Layer;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer = GsbGetStringValue( FirstWord );
 
      FirstWord = GsbGetNextWord( 0, 0 );
 
      if ( FirstWord == EndRecordKeyword )
      {
        GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
      }
      else
      if ( ( GET_BIGVIA_HOLE_LAYER( Layer, 0 ) != RDS_BIGVIA_HOLE_EMPTY ) ||
           ( GET_TURNVIA_LAYER( Layer, 0 )     != RDS_TURNVIA_EMPTY     ) )
      {
        GRAAL_BIGVIA_VALUE_TABLE [ (int)Layer ][ 0 ] = GsbGetStringFloat( FirstWord );
      }
 
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Reference Name                  |
|                                                             |
\------------------------------------------------------------*/

void GsbReadReferenceName()

{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1                 ) &&
          ( LayerCount <= MBK_MAX_REFERENCE ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_REFERENCE )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ ) 
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( GET_REFERENCE_LAYER( Layer, 0 ) != RDS_REFERENCE_EMPTY )
        {
          GRAAL_REFERENCE_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
  
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Read Orient Name                   |
|                                                             |
\------------------------------------------------------------*/

void GsbReadOrientName()

{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1              ) &&
          ( LayerCount <= MBK_MAX_ORIENT ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_ORIENT )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ ) 
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        {
          GRAAL_ORIENT_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
  
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb Read Symmetry Name                 |
|                                                             |
\------------------------------------------------------------*/

void GsbReadSymmetryName()
 
{
  char  Layer;
  char  Field;
  char  LayerCount;
  char  EndTable;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1                ) &&
          ( LayerCount <= MBK_MAX_SYMMETRY ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_SYMMETRY )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ ) 
      {
        FirstWord = GsbGetNextWord( 0, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        {
          GRAAL_SYMMETRY_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
  
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Big Via Name                    |
|                                                             |
\------------------------------------------------------------*/
 
void GsbReadBigViaName()
 
{
  char  Layer;
  char  LayerCount;
  char  EndTable;
  char  Field;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 1, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( ( GET_BIGVIA_HOLE_LAYER( Layer, 0 )  != RDS_BIGVIA_HOLE_EMPTY ) ||
             ( GET_TURNVIA_LAYER( Layer, 0 )      != RDS_TURNVIA_EMPTY     ) )
        {
          GRAAL_BIGVIA_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
   
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
}
 
/*------------------------------------------------------------\
|                                                             |
|                    Gsb Read Via Name                        |
|                                                             |
\------------------------------------------------------------*/
 
void GsbReadViaName()
 
{
  char  Layer;
  char  LayerCount;
  char  EndTable;
  char  Field;
  char *FirstWord; 

  rdsbegin();
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    GsbGetLine( GsbBuffer );
 
    FirstWord  = GsbGetFirstWord( GsbBuffer, 1, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 1, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( GET_VIA_LAYER( Layer, 0 ) != RDS_VIA_EMPTY )
        {
          GRAAL_VIA_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
   
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        GsbError( TOO_MANY_WORDS, FirstWord, GsbCurrentLine );
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    GsbError( LINE_EXPECTED, EndTableKeyword, GsbCurrentLine);
  }

  rdsend();
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
      Layer     = GsbGetStringValue( FirstWord );
 
      for ( Field = 0; Field < 3; Field++ )
      {
        FirstWord = GsbGetNextWord( 1, 0 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          GsbError( MISSING_VALUE, (char *)NULL, GsbCurrentLine );
        }
        else
        if ( RDS_DYNAMIC_LAYER[ (int)Layer ] != RDS_LAYER_UNUSED )
        {
          GRAAL_RDS_LAYER_NAME_TABLE [ (int)Layer ][ (int)Field ] = FirstWord;
        }
      }
   
      FirstWord = GsbGetNextWord( 0, 1 );
  
      if ( FirstWord != EndRecordKeyword )
      {
        Pattern = GsbGetStringValue( FirstWord );

        if ( RDS_DYNAMIC_LAYER[ (int)Layer ] != RDS_LAYER_UNUSED )
        {
          GRAAL_RDS_LAYER_PATTERN_TABLE[ (int)Layer ] = (int)Pattern;
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
        GsbError( MISSING_VALUE, SecondWord, GsbCurrentLine );
      }

      if ( FirstWord == PeekBoundKeyword )
      {
        GRAAL_PEEK_BOUND = GsbGetStringValue( SecondWord ) * GRAAL_RDS_LAMBDA;

        Continue |= GSB_PEEK_BOUND_MASK;
      }
      else
      if ( FirstWord == LowerGridStepKeyword )
      {
        GRAAL_LOWER_GRID_STEP = GsbGetStringFloat( SecondWord );

        Continue |= GSB_LOWER_GRID_STEP_MASK;
      }
      else
      if ( FirstWord == LowerFigureStepKeyword )
      {
        GRAAL_LOWER_FIGURE_STEP = GsbGetStringFloat( SecondWord ) / GRAAL_SCALE;

        Continue |= GSB_LOWER_FIGURE_STEP_MASK;
      }
      else
      if ( FirstWord == LowerInstanceStepKeyword )
      {
        GRAAL_LOWER_INSTANCE_STEP = GsbGetStringFloat( SecondWord ) / GRAAL_SCALE;

        Continue |= GSB_LOWER_INSTANCE_STEP_MASK;
      }
      else
      if ( FirstWord == LowerSegmentStepKeyword )
      {
        GRAAL_LOWER_SEGMENT_STEP = GsbGetStringFloat( SecondWord ) / GRAAL_SCALE;

        Continue |= GSB_LOWER_SEGMENT_STEP_MASK;
      }
      else
      if ( FirstWord == LowerConnectorStepKeyword )
      {
        GRAAL_LOWER_CONNECTOR_STEP = GsbGetStringFloat( SecondWord ) / GRAAL_SCALE;

        Continue |= GSB_LOWER_CONNECTOR_STEP_MASK;
      }
      else
      if ( FirstWord == LowerReferenceStepKeyword )
      {
        GRAAL_LOWER_REFERENCE_STEP = GsbGetStringFloat( SecondWord ) / GRAAL_SCALE;

        Continue |= GSB_LOWER_REFERENCE_STEP_MASK;
      }
      else
      if ( FirstWord == CursorColorNameKeyword )
      {
        GRAAL_CURSOR_COLOR_NAME = namealloc( SecondWord );

        Continue |= GSB_CURSOR_COLOR_NAME_MASK;
      }
      else
      if ( FirstWord == CursorSizeKeyword )
      {
        GRAAL_CURSOR_SIZE = GsbGetStringValue( SecondWord );

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

      if ( FirstWord == ConnectorNameKeyword )
      {
        GsbReadConnectorName();

        Continue |= GSB_CONNECTOR_NAME_MASK;
      }
      else
      if ( FirstWord == SegmentNameKeyword )
      {
        GsbReadSegmentName();

        Continue |= GSB_SEGMENT_NAME_MASK;
      }
      else
      if ( FirstWord == SegmentValueKeyword )
      {
        GsbReadSegmentValue();

        Continue |= GSB_SEGMENT_VALUE_MASK;
      }
      else
      if ( FirstWord == BigViaValueKeyword )
      {
        GsbReadBigViaValue();

        Continue |= GSB_BIGVIA_VALUE_MASK;
      }
      else
      if ( FirstWord == TransistorNameKeyword )
      {
        GsbReadTransistorName();

        Continue |= GSB_TRANSISTOR_NAME_MASK;
      }
      else
      if ( FirstWord == ViaNameKeyword )
      {
        GsbReadViaName();

        Continue |= GSB_VIA_NAME_MASK;
      } 
      else
      if ( FirstWord == BigViaNameKeyword )
      {
        GsbReadBigViaName();

        Continue |= GSB_BIGVIA_NAME_MASK;
      } 
      else
      if ( FirstWord == ReferenceNameKeyword )
      {
        GsbReadReferenceName();

        Continue |= GSB_REFERENCE_NAME_MASK;
      }
      else
      if ( FirstWord == OrientNameKeyword )
      {
        GsbReadOrientName();
 
        Continue |= GSB_ORIENT_NAME_MASK;
      }
      else
      if ( FirstWord == SymmetryNameKeyword )
      {
        GsbReadSymmetryName();
 
        Continue |= GSB_SYMMETRY_NAME_MASK;
      }
      else
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

void GraalLoadParameters()

{
  char  Layer;
  char  Buffer[ 512 ];
  char *String;

  rdsbegin();
  
  GRAAL_TECHNO_NAME = mbkgetenv( "GRAAL_TECHNO_NAME" );

  if ( GRAAL_TECHNO_NAME == (char *)NULL )
  {
    String = mbkgetenv( "ALLIANCE_TOP" );
    if ( String == (char *)NULL ) String = ALLIANCE_TOP;

    sprintf( Buffer, "%s/%s", String, GRAAL_DEFAULT_TECHNO_NAME );
    GRAAL_TECHNO_NAME = mbkstrdup( Buffer );
  }

  GRAAL_RDS_LAMBDA = RDS_LAMBDA / GRAAL_SCALE;
  GRAAL_SCALE_X    = (float)SCALE_X / (float)GRAAL_SCALE;

  if ( ! KeywordDefined )
  {
    DefineKeyword             = namealloc( DEFINE_KEYWORD               );
    TableKeyword              = namealloc( TABLE_KEYWORD                );
    EndTableKeyword           = namealloc( END_TABLE_KEYWORD            );
    EndRecordKeyword          = namealloc( END_RECORD_KEYWORD           );
    PeekBoundKeyword          = namealloc( PEEK_BOUND_KEYWORD           );
    LowerGridStepKeyword      = namealloc( LOWER_GRID_STEP_KEYWORD      );
    LowerFigureStepKeyword    = namealloc( LOWER_FIGURE_STEP_KEYWORD    );
    LowerInstanceStepKeyword  = namealloc( LOWER_INSTANCE_STEP_KEYWORD  );
    LowerReferenceStepKeyword = namealloc( LOWER_REFERENCE_STEP_KEYWORD );
    LowerSegmentStepKeyword   = namealloc( LOWER_SEGMENT_STEP_KEYWORD   );
    LowerConnectorStepKeyword = namealloc( LOWER_CONNECTOR_STEP_KEYWORD );
    CursorColorNameKeyword    = namealloc( CURSOR_COLOR_NAME_KEYWORD    );
    CursorSizeKeyword         = namealloc( CURSOR_SIZE_KEYWORD          );
    SegmentNameKeyword        = namealloc( SEGMENT_NAME_KEYWORD         );
    TransistorNameKeyword     = namealloc( TRANSISTOR_NAME_KEYWORD      );
    ConnectorNameKeyword      = namealloc( CONNECTOR_NAME_KEYWORD       );
    SegmentValueKeyword       = namealloc( SEGMENT_VALUE_KEYWORD        );
    ViaNameKeyword            = namealloc( VIA_NAME_KEYWORD             );
    BigViaNameKeyword         = namealloc( BIGVIA_NAME_KEYWORD          );
    BigViaValueKeyword        = namealloc( BIGVIA_VALUE_KEYWORD         );
    ReferenceNameKeyword      = namealloc( REFERENCE_NAME_KEYWORD       );
    OrientNameKeyword         = namealloc( ORIENT_NAME_KEYWORD          );
    SymmetryNameKeyword       = namealloc( SYMMETRY_NAME_KEYWORD        );
    RdsLayerNameKeyword       = namealloc( RDS_LAYER_NAME_KEYWORD       );

    KeywordDefined = 1;
  }

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    GRAAL_CONNECTOR_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_CONNECTOR_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_CONNECTOR_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;

    GRAAL_SEGMENT_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_SEGMENT_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_SEGMENT_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;

    GRAAL_TRANSISTOR_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_TRANSISTOR_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_TRANSISTOR_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;

    GRAAL_SEGMENT_VALUE_TABLE[ (int)Layer ][0] = 0;
    GRAAL_SEGMENT_VALUE_TABLE[ (int)Layer ][1] = 0;
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    GRAAL_VIA_NAME_TABLE[ (int)Layer ][0]  = (char *)NULL;;
    GRAAL_VIA_NAME_TABLE[ (int)Layer ][1]  = (char *)NULL;
    GRAAL_VIA_NAME_TABLE[ (int)Layer ][2]  = (char *)NULL;

    GRAAL_BIGVIA_NAME_TABLE[ (int)Layer ][0]  = (char *)NULL;;
    GRAAL_BIGVIA_NAME_TABLE[ (int)Layer ][1]  = (char *)NULL;
    GRAAL_BIGVIA_NAME_TABLE[ (int)Layer ][2]  = (char *)NULL;

    GRAAL_BIGVIA_VALUE_TABLE[ (int)Layer ][0] = 0;
  }

  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    GRAAL_REFERENCE_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_REFERENCE_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_REFERENCE_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;
  }

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    GRAAL_RDS_LAYER_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_RDS_LAYER_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_RDS_LAYER_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;
    GRAAL_RDS_LAYER_PATTERN_TABLE[ (int)Layer ] = -1;
  }
  
  for ( Layer = 0; Layer < MBK_MAX_SYMMETRY; Layer++ )
  {
    GRAAL_SYMMETRY_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_SYMMETRY_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_SYMMETRY_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;
  }

  for ( Layer = 0; Layer < MBK_MAX_ORIENT; Layer++ )
  {
    GRAAL_ORIENT_NAME_TABLE[ (int)Layer ][0] = (char *)NULL;;
    GRAAL_ORIENT_NAME_TABLE[ (int)Layer ][1] = (char *)NULL;
    GRAAL_ORIENT_NAME_TABLE[ (int)Layer ][2] = (char *)NULL;
  }

  GRAAL_PEEK_BOUND            = 0;
  GRAAL_LOWER_GRID_STEP       = 0.0;
  GRAAL_LOWER_FIGURE_STEP     = 0.0;
  GRAAL_LOWER_INSTANCE_STEP   = 0.0;
  GRAAL_LOWER_CONNECTOR_STEP  = 0.0;
  GRAAL_LOWER_SEGMENT_STEP    = 0.0;
  GRAAL_LOWER_REFERENCE_STEP  = 0.0;
  GRAAL_CURSOR_COLOR_NAME     = (char *)NULL;
  GRAAL_CURSOR_SIZE           = 5;

  if ( !( GsbFile = fopen( GRAAL_TECHNO_NAME, "r")))

    GsbError( OPEN_FILE, NULL, 0);

  GsbReadParam();

  fclose( GsbFile );

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                      Gsb View Parameters                    |
|                                                             |
\------------------------------------------------------------*/

# ifdef DEBUG

void GraalViewParameters()

{
  char Layer;

  rdsbegin();

  fprintf( stdout, "\nPEEK_BOUND DEFINE %d\n", 
                   GRAAL_PEEK_BOUND / GRAAL_RDS_LAMBDA );

  fprintf( stdout, "\nSEGMENT TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s %s %s %e %e", 
             Layer,
             GRAAL_SEGMENT_NAME_TABLE[ Layer ][0],
             GRAAL_SEGMENT_NAME_TABLE[ Layer ][1],
             GRAAL_SEGMENT_NAME_TABLE[ Layer ][2],
             GRAAL_SEGMENT_VALUE_TABLE[ Layer ][0],
             GRAAL_SEGMENT_VALUE_TABLE[ Layer ][1] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nTRANSISTOR TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s %s %s", 
             Layer,
             GRAAL_TRANSISTOR_NAME_TABLE[ Layer ][0],
             GRAAL_TRANSISTOR_NAME_TABLE[ Layer ][1],
             GRAAL_TRANSISTOR_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nCONNECTOR TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s %s %s",
             Layer,
             GRAAL_CONNECTOR_NAME_TABLE[ Layer ][0],
             GRAAL_CONNECTOR_NAME_TABLE[ Layer ][1],
             GRAAL_CONNECTOR_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nVIA TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    fprintf( stdout, "\nVia:%d %s %s %s", 
             Layer,
             GRAAL_VIA_NAME_TABLE[ Layer ][0],
             GRAAL_VIA_NAME_TABLE[ Layer ][1],
             GRAAL_VIA_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nBIGVIA TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    fprintf( stdout, "\nVia:%d %s %s %s", 
             Layer,
             GRAAL_BIGVIA_NAME_TABLE[ Layer ][0],
             GRAAL_BIGVIA_NAME_TABLE[ Layer ][1],
             GRAAL_BIGVIA_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nREFERENCE TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    fprintf( stdout, "\nReference:%d %s %s %s", 
             Layer, 
             GRAAL_REFERENCE_NAME_TABLE[ Layer ][0],
             GRAAL_REFERENCE_NAME_TABLE[ Layer ][1],
             GRAAL_REFERENCE_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\nORIENT TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_ORIENT; Layer++ )
  {
    fprintf( stdout, "\nSymmetry:%d %s %s %s", 
             Layer, 
             GRAAL_ORIENT_NAME_TABLE[ Layer ][0],
             GRAAL_ORIENT_NAME_TABLE[ Layer ][1],
             GRAAL_ORIENT_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\nSYMMETRY TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_SYMMETRY; Layer++ )
  {
    fprintf( stdout, "\nReference:%d %s %s %s", 
             Layer, 
             GRAAL_SYMMETRY_NAME_TABLE[ Layer ][0],
             GRAAL_SYMMETRY_NAME_TABLE[ Layer ][1],
             GRAAL_SYMMETRY_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nRDS_LAYER TABLE\n" );

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s %s %s", 
             Layer, 
             GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][0],
             GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][1],
             GRAAL_RDS_LAYER_NAME_TABLE[ Layer ][2] );
  }

  fprintf( stdout, "\n\nEND\n" );

  rdsend();
}

# endif
