/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                  G_parse.c                        |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   05.03.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "G_parse.h"

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

  static char *GenviewDefaultTechnoName = GENVIEW_DEFAULT_TECHNO_NAME;

/*------------------------------------------------------------\
|                                                             |
|                       Table variables                       |
|                                                             |
\------------------------------------------------------------*/

char  *GENVIEW_VIA_NAME_TABLE[ MBK_MAX_VIA ][ 1 ];
char  *GENVIEW_SYMMETRY_NAME_TABLE[ MBK_MAX_SYMMETRY ][ 1 ];
char  *GENVIEW_ORIENT_NAME_TABLE[ MBK_MAX_ORIENT ][ 1 ];
char  *GENVIEW_REFERENCE_NAME_TABLE[ MBK_MAX_REFERENCE ][ 1 ];
char  *GENVIEW_SEGMENT_NAME_TABLE[ MBK_MAX_LAYER ][ 1 ];
char  *GENVIEW_CONNECTOR_NAME_TABLE[ MBK_MAX_LAYER ][ 1 ];
char  *GENVIEW_RDS_LAYER_NAME_TABLE[ RDS_ALL_LAYER ][ 2 ];

long   GENVIEW_PEEK_BOUND;

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
static char *SegmentNameKeyword;
static char *ConnectorNameKeyword;
static char *ViaNameKeyword;
static char *SymmetryNameKeyword;
static char *OrientNameKeyword;
static char *ReferenceNameKeyword;
static char *RdsLayerNameKeyword;


static keyword KeywordDefine[ GPARSE_MAX_KEYWORD ] =
 
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
    { "east",         GENVIEW_EAST       }, 
    { "ndif",         NDIF             },
    { "north",        GENVIEW_NORTH      }, 
    { "nosym",        NOSYM            }, 
    { "ntie",         NTIE             },
    { "ntrans",       NTRANS           },
    { "nwell",        NWELL            },
    { "pdif",         PDIF             },
    { "poly",         POLY             },
    { "poly2",        POLY2            },
    { "ptie",         PTIE             },
    { "ptrans",       PTRANS           },
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
    { "south",        GENVIEW_SOUTH      }, 
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
    { "west",         GENVIEW_WEST       }
  };

/*------------------------------------------------------------\
|                                                             |
|                       File variables                        |
|                                                             |
\------------------------------------------------------------*/

static  FILE *GparseFile;
static  char  GparseBuffer[ GPARSE_MAX_BUFFER ];
static  long  GparseCurrentLine;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                        Gparse error                         |
|                                                             |
\------------------------------------------------------------*/


void GparseError( ErrorType, Message, Data )

     char  ErrorType; 
     char *Message;
     long  Data;
{
  fflush(stdout);
  fprintf( stderr, "Gparser: " );

  switch ( ErrorType ) 
  {
    case DEFINE_MISSING : 

      fprintf( stderr, "Some defines missing\n");
      break;

    case TABLE_MISSING  : 
 
      fprintf( stderr, "Some tables missing\n");
      break;

    case LINE_EXPECTED  :
 
      fprintf( stderr, "%s expected line %lu\n", 
      Message, Data );
      break;

    case UNKNOWN_DEFINE : 

      fprintf( stderr, "Unknown define %s line %lu\n",
      Message, Data );
      break;

    case NOT_DEFINED    : 

      fprintf( stderr, "%s not defined line %lu\n",
      Message, Data );
      break;

    case TOO_SMAL       :

      fprintf( stderr, "%s too smal line %lu\n",
      Message, Data );
      break;

    case MISSING_VALUE  :

      fprintf( stderr, "Missing value at line %lu\n",
      Data );
      break;

    case MISSING_NAME   :

      fprintf( stderr, "Missing name of %s line %lu\n",
      Message, Data );
      break;

    case UNEXPECTED_LINE :

      fprintf( stderr, "%s unexpected line %lu\n",
      Message, Data );
      break;

    case UNEXPECTED_EOF  :

      fprintf( stderr, "Unexpected end of file, missing definitions\n");
      break;

    case TOO_MANY_WORDS  :

      fprintf( stderr, "Too many words %s unexpected line %lu\n",
      Message, Data );
      break;

    case MISSING_TABLE  :

      fprintf( stderr, "Missing value in %s table line %lu\n",
      Message, Data );
      break;

    case OPEN_FILE      :

      fprintf( stderr, "Parameters file %s can't be opened\n", 
      Message );
      break;

    case UNKNOWN_TABLE  :

      fprintf( stderr, "Unknown table %s line %lu\n",
      Message , Data );
      break;

    case SYNTAX_ERROR   :

      fprintf( stderr, "Syntax Error %s at line %lu\n", 
      Message , Data );
      break;

    case ILLEGAL_FLOAT  :

      fprintf( stderr, "Illegal floating point number %s line %lu\n", 
      Message , Data );
      break;
  }

  exit( -15 );
}

/*------------------------------------------------------------\
|                                                             |
|                    Gparse File Get String                   |
|                                                             |
\------------------------------------------------------------*/

 char *GparseFileGetString( String, Size )

 char *String;
 int   Size;
 {
 register char *RegisterString;
 register       Register;

 RegisterString = String;

 while (--Size > 0 && (Register = getc( GparseFile )) != EOF )
 {
   if ((*RegisterString++ = Register) == '\\') 
   {
     if ((Register = getc( GparseFile )) == '\n') 
     {
       *(RegisterString - 1) = ' ';
     } 
     else
     {
       ungetc( Register, GparseFile );
     }
   }
   else
   {
     if ( Register == '\n') break;
   }
 }

 *RegisterString = '\0';

 return ( ( Register       == EOF    ) && 
          ( RegisterString == String ) ) ? (char *)NULL : String;
 }

/*------------------------------------------------------------\
|                                                             |
|                    Gparse File Get Line                     |
|                                                             |
\------------------------------------------------------------*/

 void GparseGetLine( Buffer )

 char   *Buffer;
 {
 char   *Check;
 char   *String;
 char    OneComment;

 do 
 {
   OneComment = 0;

   Check = GparseFileGetString( Buffer, GPARSE_MAX_BUFFER );

   if ( Check != (char *)NULL ) 
   {
     GparseCurrentLine++;
   }
   else 
   {
     GparseError( UNEXPECTED_EOF, (char *)NULL, GparseCurrentLine );
   }

   if ( String = strchr( Buffer, GPARSE_COMMENT_CHAR ))
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
 
   while (*Buffer != '\0' && strchr( GPARSE_SEPARATORS_STRING, *Buffer))
   {
     Buffer = Buffer + 1;;
   }

   if (*Buffer == '\0') OneComment = 1;

 } 
 while ( OneComment == 1);
 }      

/*------------------------------------------------------------\
|                                                             |
|                     Gparse Check Keyword                    |
|                                                             |
\------------------------------------------------------------*/

 char GparseCheckKeyword( Word )

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
|                     Gparse Get First word                   |
|                                                             |
\------------------------------------------------------------*/

 char *GparseGetFirstWord( Buffer, IsKeyword, Hash )
 
     char *Buffer;
     char  IsKeyword;
     char  Hash;
 {
 char   *String;
 char   *Text;
 int     Index;

 if ( String = (char *)strtok( Buffer, GPARSE_SEPARATORS_STRING )) 
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

   if ( ( IsKeyword ) && GparseCheckKeyword( String ) )
   {
     GparseError( UNEXPECTED_LINE, String, GparseCurrentLine );
   }
 }
 return( String );
 }

/*------------------------------------------------------------\
|                                                             |
|                     Gparse Get Next word                    |
|                                                             |
\------------------------------------------------------------*/

 char *GparseGetNextWord( IsKeyword, Hash )
 
     char IsKeyword;
     char Hash;
 {
 char   *String;
 char   *Text;
 int     Index;

 if ( String = (char *)strtok( (char *)NULL, GPARSE_SEPARATORS_STRING )) 
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

    if ( ( IsKeyword ) && GparseCheckKeyword( String ) )
    {
      GparseError( UNEXPECTED_LINE, String, GparseCurrentLine );
    }
 }

 return( String );
 }

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Keyword Compare                    |
|                                                             |
\------------------------------------------------------------*/

int GparseKeywordCompare( FirstKey, SecondKey )

    keyword *FirstKey;
    keyword *SecondKey;
{
  return strcmp( FirstKey->NAME, SecondKey->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Get String Value                   |
|                                                             |
\------------------------------------------------------------*/

 long GparseGetStringValue( String )

  char     *String;
 {
 long      Value;
 keyword  *Keyword;
 keyword   Entry;

 if ( sscanf( String, "%d", &Value) )

   return ( Value );

 Entry.NAME = String;

 Keyword = (keyword *)bsearch( (char *)(&Entry), 
                               (char *)KeywordDefine, 
                               GPARSE_MAX_KEYWORD, sizeof( keyword ),
                               GparseKeywordCompare );

 if ( Keyword != (keyword *)NULL ) return( Keyword->VALUE );

 GparseError( UNEXPECTED_LINE, String, GparseCurrentLine );
 }

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Get String Float                   |
|                                                             |
\------------------------------------------------------------*/

 float GparseGetStringFloat( String )

  char     *String;
 {
 float     Value;
 keyword  *Keyword;
 keyword   Entry;

 if ( sscanf( String, "%g", &Value) )

   return ( Value );

 GparseError( ILLEGAL_FLOAT, String, GparseCurrentLine );
 }

/*------------------------------------------------------------\
|                                                             |
|                     Gparse Get Number                       |
|                                                             |
\------------------------------------------------------------*/

long GparseGetNumber( String )

     char *String;
{
  long Value;

  if ( sscanf( String, "%d", &Value )) return Value;

  GparseError( UNEXPECTED_LINE, "number", GparseCurrentLine );
}

/*------------------------------------------------------------\
|                                                             |
|                 Gparse Read Segment Name                    |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadSegmentName()

 {
 char  Layer;
 char  Field;
 char  LayerCount;
 char  EndTable;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1             ) &&
         ( LayerCount <= MBK_MAX_LAYER ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < MBK_MAX_LAYER )
   {
     Layer     = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 1; Field++ )
     {
       FirstWord = GparseGetNextWord( 0, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_SEGMENT_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }

     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }

 if ( EndTable == 0 )
 {
   GparseError( LINE_EXPECTED, EndTableKeyword, GparseCurrentLine);
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                 Gparse Read Connector Name                  |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadConnectorName()

 {
 char  Layer;
 char  Field;
 char  LayerCount;
 char  EndTable;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1             ) &&
         ( LayerCount <= MBK_MAX_LAYER ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < MBK_MAX_LAYER )
   {
     Layer     = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 1; Field++ )
     {
       FirstWord = GparseGetNextWord( 0, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_CONNECTOR_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }

     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }

 if ( EndTable == 0 )
 {
   GparseError( LINE_EXPECTED, EndTableKeyword, GparseCurrentLine);
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                 Gparse Read Reference Name                  |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadReferenceName()

 {
 char  Layer;
 char  Field;
 char  LayerCount;
 char  EndTable;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1                 ) &&
         ( LayerCount <= MBK_MAX_REFERENCE ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < MBK_MAX_REFERENCE )
   {
     Layer     = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 1; Field++ ) 
     {
       FirstWord = GparseGetNextWord( 0, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_REFERENCE_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }
 
     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }

 if ( EndTable == 0 )
 {
   GparseError( LINE_EXPECTED, EndTableKeyword, GparseCurrentLine);
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Read Orient Name                   |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadOrientName()

 {
 char  Layer;
 char  Field;
 char  LayerCount;
 char  EndTable;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1              ) &&
         ( LayerCount <= MBK_MAX_ORIENT ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < MBK_MAX_ORIENT )
   {
     Layer     = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 1; Field++ ) 
     {
       FirstWord = GparseGetNextWord( 0, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_ORIENT_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }
 
     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }

 if ( EndTable == 0 )
 {
   GparseError( LINE_EXPECTED, EndTableKeyword, GparseCurrentLine);
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Read Symmetry Name                 |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadSymmetryName()

 {
 char  Layer;
 char  Field;
 char  LayerCount;
 char  EndTable;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1                ) &&
         ( LayerCount <= MBK_MAX_SYMMETRY ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < MBK_MAX_SYMMETRY )
   {
     Layer     = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 1; Field++ ) 
     {
       FirstWord = GparseGetNextWord( 0, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_SYMMETRY_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }
 
     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }

 if ( EndTable == 0 )
 {
   GparseError( LINE_EXPECTED, EndTableKeyword, GparseCurrentLine);
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                     Gparse Read Via Name                    |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadViaName()

 {
 char  Layer;
 char  LayerCount;
 char  EndTable;
 char  Field;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1           ) &&
         ( LayerCount <= MBK_MAX_VIA ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < MBK_MAX_VIA )
   {
     Layer     = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 1; Field++ )
     {
       FirstWord = GparseGetNextWord( 1, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_VIA_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }
  
     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }

 if ( EndTable == 0 )
 {
   GparseError( LINE_EXPECTED, EndTableKeyword, GparseCurrentLine);
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                 Gparse Read Rds Layer Name                  |
|                                                             |
\------------------------------------------------------------*/

 void GparseReadRdsLayerName()

 {
 char  Layer;
 char  LayerCount;
 char  EndTable;
 char  Field;
 char *FirstWord; 

 EndTable   = 0;
 LayerCount = 0;

 while ( ( EndTable   != 1             ) &&
         ( LayerCount <= RDS_ALL_LAYER ) )
 {
   GparseGetLine( GparseBuffer );

   FirstWord  = GparseGetFirstWord( GparseBuffer, 1, 1 );
   
   if ( FirstWord == EndTableKeyword )
   {
     EndTable = 1;
   }
   else
   if ( LayerCount < RDS_ALL_LAYER )
   {
     Layer = GparseGetStringValue( FirstWord );

     for ( Field = 0; Field < 2; Field++ )
     {
       FirstWord = GparseGetNextWord( 1, 0 );

       if ( FirstWord == EndRecordKeyword )
       {
         GparseError( MISSING_VALUE, (char *)NULL, GparseCurrentLine );
       }
       else
       {
         GENVIEW_RDS_LAYER_NAME_TABLE [ Layer ][ Field ] = FirstWord;
       }
     }
  
     FirstWord = GparseGetNextWord( 0, 1 );
 
     if ( FirstWord != EndRecordKeyword )
     {
       GparseError( TOO_MANY_WORDS, FirstWord, GparseCurrentLine );
     }
   }
 
   LayerCount = LayerCount + 1;
 }
 }

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Read Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void GparseReadParam()

{
  char *FirstWord;
  char *SecondWord;
  long  Continue;

  Continue = 0;

  while ( Continue != GPARSE_ALL_DEFINED_MASK )  
  {
    GparseGetLine( GparseBuffer );

    FirstWord = GparseGetFirstWord( GparseBuffer, 0, 1);

    if ( FirstWord == DefineKeyword ) 
    {
      FirstWord = GparseGetNextWord( 1, 1 );

      if (! FirstWord )
      {
        GparseError( MISSING_NAME, DefineKeyword, GparseCurrentLine );
      }

      SecondWord = GparseGetNextWord(1);

      if (! SecondWord ) 
      {
        GparseError( MISSING_VALUE, SecondWord );
      }

      if ( FirstWord == PeekBoundKeyword )
      {
        GENVIEW_PEEK_BOUND = GparseGetStringValue( SecondWord ) * RDS_LAMBDA;

        Continue |= GPARSE_PEEK_BOUND_MASK;
      }
    }
    else 
    if ( FirstWord == TableKeyword )
    {
      if (!( FirstWord = GparseGetNextWord(1, 1)))
      {
        GparseError( MISSING_NAME, TableKeyword );
      }

      if ( FirstWord == ConnectorNameKeyword )
      {
        GparseReadConnectorName();

        Continue |= GPARSE_CONNECTOR_NAME_MASK;
      }
      else
      if ( FirstWord == SegmentNameKeyword )
      {
        GparseReadSegmentName();

        Continue |= GPARSE_SEGMENT_NAME_MASK;
      }
      else
      if ( FirstWord == ViaNameKeyword )
      {
        GparseReadViaName();

        Continue |= GPARSE_VIA_NAME_MASK;
      } 
      else
      if ( FirstWord == ReferenceNameKeyword )
      {
        GparseReadReferenceName();

        Continue |= GPARSE_REFERENCE_NAME_MASK;
      }
      else
      if ( FirstWord == OrientNameKeyword )
      {
        GparseReadOrientName();
 
        Continue |= GPARSE_ORIENT_NAME_MASK;
      }
      else
      if ( FirstWord == SymmetryNameKeyword )
      {
        GparseReadSymmetryName();
 
        Continue |= GPARSE_SYMMETRY_NAME_MASK;
      }
      else
      if ( FirstWord == RdsLayerNameKeyword )
      {
        GparseReadRdsLayerName();
 
        Continue |= GPARSE_RDS_LAYER_NAME_MASK;
      }
      else
      {
        GparseError( UNKNOWN_TABLE, FirstWord, GparseCurrentLine );
      }
    } 
    else
    { 
      GparseError( SYNTAX_ERROR, FirstWord, GparseCurrentLine );
    } 
  }       
}

/*------------------------------------------------------------\
|                                                             |
|                   Gparse Load Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void GenviewLoadParameters()

{
  char  Layer;
  char *FileName;
  
  FileName = mbkgetenv( GENVIEW_TECHNO_NAME );

  if ( FileName == (char *)NULL )
  {
    FileName = GenviewDefaultTechnoName;
  }

  if ( ! KeywordDefined )
  {
    DefineKeyword             = namealloc( DEFINE_KEYWORD               );
    TableKeyword              = namealloc( TABLE_KEYWORD                );
    EndTableKeyword           = namealloc( END_TABLE_KEYWORD            );
    EndRecordKeyword          = namealloc( END_RECORD_KEYWORD           );
    PeekBoundKeyword          = namealloc( PEEK_BOUND_KEYWORD           );
    SegmentNameKeyword        = namealloc( SEGMENT_NAME_KEYWORD         );
    ConnectorNameKeyword      = namealloc( CONNECTOR_NAME_KEYWORD       );
    ViaNameKeyword            = namealloc( VIA_NAME_KEYWORD             );
    ReferenceNameKeyword      = namealloc( REFERENCE_NAME_KEYWORD       );
    OrientNameKeyword         = namealloc( ORIENT_NAME_KEYWORD          );
    SymmetryNameKeyword       = namealloc( SYMMETRY_NAME_KEYWORD        );
    RdsLayerNameKeyword       = namealloc( RDS_LAYER_NAME_KEYWORD       );

    KeywordDefined = 1;
  }

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    GENVIEW_CONNECTOR_NAME_TABLE[ Layer ][0] = (char *)NULL;
    GENVIEW_SEGMENT_NAME_TABLE  [ Layer ][0] = (char *)NULL;
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    GENVIEW_VIA_NAME_TABLE[ Layer ][0]  = (char *)NULL;
  }

  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    GENVIEW_REFERENCE_NAME_TABLE[ Layer ][0] = (char *)NULL;
  }

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    GENVIEW_RDS_LAYER_NAME_TABLE[ Layer ][0] = (char *)NULL;
    GENVIEW_RDS_LAYER_NAME_TABLE[ Layer ][1] = (char *)NULL;
    GENVIEW_RDS_LAYER_NAME_TABLE[ Layer ][2] = (char *)NULL;
  }
  
  for ( Layer = 0; Layer < MBK_MAX_SYMMETRY; Layer++ )
  {
    GENVIEW_SYMMETRY_NAME_TABLE[ Layer ][0] = (char *)NULL;
  }

  for ( Layer = 0; Layer < MBK_MAX_ORIENT; Layer++ )
  {
    GENVIEW_ORIENT_NAME_TABLE[ Layer ][0] = (char *)NULL;
  }

  GENVIEW_PEEK_BOUND = 0;

  if ( !( GparseFile = fopen( FileName, "r")))

    GparseError( OPEN_FILE, FileName, 0);

  GparseReadParam();

  fclose( GparseFile );
}

# ifdef GENVIEW_DEBUG

/*------------------------------------------------------------\
|                                                             |
|                      Gparse View Parameters                 |
|                                                             |
\------------------------------------------------------------*/

void GenviewViewParameters()

{
  char Layer;

  fprintf( stdout, "\nPEEK_BOUND DEFINE %d\n", 
                   GENVIEW_PEEK_BOUND / RDS_LAMBDA );

  fprintf( stdout, "\nSEGMENT TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s", 
             Layer,
             GENVIEW_SEGMENT_NAME_TABLE[ Layer ][0] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nCONNECTOR TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s",
             Layer,
             GENVIEW_CONNECTOR_NAME_TABLE[ Layer ][0] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nVIA TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    fprintf( stdout, "\nVia:%d %s", 
             Layer,
             GENVIEW_VIA_NAME_TABLE[ Layer ][0] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nREFERENCE TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    fprintf( stdout, "\nReference:%d %s", 
             Layer, 
             GENVIEW_REFERENCE_NAME_TABLE[ Layer ][0] );
  }

  fprintf( stdout, "\nORIENT TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_ORIENT; Layer++ )
  {
    fprintf( stdout, "\nSymmetry:%d %s", 
             Layer, 
             GENVIEW_ORIENT_NAME_TABLE[ Layer ][0] );
  }

  fprintf( stdout, "\nSYMMETRY TABLE\n" );

  for ( Layer = 0; Layer < MBK_MAX_SYMMETRY; Layer++ )
  {
    fprintf( stdout, "\nReference:%d %s", 
             Layer, 
             GENVIEW_SYMMETRY_NAME_TABLE[ Layer ][0] );
  }

  fprintf( stdout, "\n\nEND\n" );

  fprintf( stdout, "\nRDS_LAYER TABLE\n" );

  for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
  {
    fprintf( stdout, "\nLayer:%d %s %s", 
             Layer, 
             GENVIEW_RDS_LAYER_NAME_TABLE[ Layer ][0],
             GENVIEW_RDS_LAYER_NAME_TABLE[ Layer ][1] );
  }

  fprintf( stdout, "\n\nEND\n" );
}

# endif
