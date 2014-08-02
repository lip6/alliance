/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rprparse.c                      |
|                                                             |
| Authors :                Jacomme  Ludovic                   |
|                          Picault Stephane     *4p*          |
|                                                             |
| Date    :                   27.06.95                        |
|                             15.07.98          *4p*          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <math.h>
# include <ctype.h>
# include <stdlib.h>
# include <string.h>
# include <libgen.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rwi.h"
# include "rut.h"

# include "rprerror.h"
# include "rprparse.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

#define   NO_FLAGS       (0x00000000)
#define   CAN_REACH_EOF  (0x00000001)

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

  long   RDS_PHYSICAL_GRID;
  long   RDS_LAMBDA;
  long   RDS_LAMBDA2;
  long   RDS_UNIT;
  long   RDS_UNIT2;

/*------------------------------------------------------------\
|                                                             |
|                       Table variables                       |
|                                                             |
\------------------------------------------------------------*/

  unsigned char RDS_S2R_POST_TREAT_TABLE  [ RDS_ALL_LAYER ][ RDS_S2R_POST_TREAT_FIELD  ];
  long          RDS_S2R_OVERSIZE_TABLE    [ RDS_ALL_LAYER ][ RDS_S2R_OVERSIZE_FIELD    ];
  long          RDS_S2R_RING_WIDTH_TABLE  [ RDS_ALL_LAYER ][ RDS_S2R_RING_WIDTH_FIELD  ];
  long          RDS_S2R_LAYER_WIDTH_TABLE [ RDS_ALL_LAYER ][ RDS_S2R_LAYER_WIDTH_FIELD ];

  long  RDS_SEGMENT_TABLE     [ MBK_MAX_LAYER     ][ RDS_SEGMENT_FIELD   ];
  long  RDS_VIA_TABLE         [ MBK_MAX_VIA       ][ RDS_VIA_FIELD       ];
  long  RDS_CONNECTOR_TABLE   [ MBK_MAX_LAYER     ][ RDS_CONNECTOR_FIELD ];
  long  RDS_REFERENCE_TABLE   [ MBK_MAX_REFERENCE ][ RDS_REFERENCE_FIELD ];

  long  RDS_BIGVIA_HOLE_TABLE[ MBK_MAX_VIA ][ RDS_BIGVIA_HOLE_FIELD ];
  long  RDS_BIGVIA_METAL_TABLE[ MBK_MAX_VIA ][ RDS_BIGVIA_METAL_FIELD ];
  long  RDS_TURNVIA_TABLE[ MBK_MAX_VIA ][ RDS_TURNVIA_FIELD ];

  char          *RDS_CIF_LAYER_TABLE   [ RDS_ALL_LAYER ][ RDS_CIF_LAYER_FIELD ];
  unsigned char  RDS_GDS_LAYER_TABLE   [ RDS_ALL_LAYER ][ RDS_GDS_LAYER_FIELD ];

  unsigned char  RDS_LYNX_BULK_IMPLICIT_TABLE                         /*4p*/
                 [ RDS_ALL_LAYER ][ RDS_LYNX_BULK_IMPLICIT_FIELD ];   /*4p*/

  unsigned char  RDS_LYNX_GRAPH_TABLE      [ RDS_ALL_LAYER ][ RDS_LYNX_GRAPH_FIELD      ];
  float          RDS_LYNX_CAPA_TABLE       [ RDS_ALL_LAYER ][ RDS_LYNX_CAPA_FIELD       ];
  float          RDS_LYNX_RESISTOR_TABLE   [ RDS_ALL_LAYER ][ RDS_LYNX_RESISTOR_FIELD   ];
  unsigned char  RDS_LYNX_TRANSISTOR_TABLE [ MBK_MAX_LAYER ][ RDS_LYNX_TRANSISTOR_FIELD ];
  unsigned char  RDS_LYNX_DIFFUSION_TABLE  [ RDS_ALL_LAYER ][ RDS_LYNX_DIFFUSION_FIELD  ];

extern long MBK_X_GRID, MBK_Y_GRID, MBK_Y_SLICE, MBK_WIDTH_VSS, MBK_WIDTH_VDD,
	MBK_TRACK_WIDTH_ALU1,	MBK_TRACK_WIDTH_ALU2,
	MBK_TRACK_WIDTH_ALU3,	MBK_TRACK_WIDTH_ALU4,
	MBK_TRACK_WIDTH_ALU5,	MBK_TRACK_WIDTH_ALU6,
	MBK_TRACK_WIDTH_ALU7,	MBK_TRACK_WIDTH_ALU8,
	MBK_TRACK_SPACING_ALU1,	MBK_TRACK_SPACING_ALU2,
	MBK_TRACK_SPACING_ALU3,	MBK_TRACK_SPACING_ALU4,
	MBK_TRACK_SPACING_ALU5,	MBK_TRACK_SPACING_ALU6,
	MBK_TRACK_SPACING_ALU7,	MBK_TRACK_SPACING_ALU8;
  long  *RDS_WIRESETTING_TABLE   [ MBK_MAX_WIRESETTING ] = 
{
	&MBK_TRACK_SPACING_ALU1,	&MBK_TRACK_SPACING_ALU2,
	&MBK_TRACK_SPACING_ALU3,	&MBK_TRACK_SPACING_ALU4,
	&MBK_TRACK_SPACING_ALU5,	&MBK_TRACK_SPACING_ALU6,
	&MBK_TRACK_SPACING_ALU7,	&MBK_TRACK_SPACING_ALU8,
	&MBK_TRACK_WIDTH_ALU1,	&MBK_TRACK_WIDTH_ALU2,
	&MBK_TRACK_WIDTH_ALU3,	&MBK_TRACK_WIDTH_ALU4,
	&MBK_TRACK_WIDTH_ALU5,	&MBK_TRACK_WIDTH_ALU6,
	&MBK_TRACK_WIDTH_ALU7,	&MBK_TRACK_WIDTH_ALU8,
	&MBK_WIDTH_VDD,
	&MBK_WIDTH_VSS,
	&MBK_X_GRID, &MBK_Y_GRID, &MBK_Y_SLICE
};
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
  static char *LambdaKeyword;
  static char *PhysicalGridKeyword;
  static char *SegmentKeyword;
  static char *ViaKeyword;
  static char *ConnectorKeyword;
  static char *ReferenceKeyword;
  static char *CifLayerKeyword;
  static char *GdsLayerKeyword;
  static char *S2RPostTreatKeyword;
  static char *S2ROversizeKeyword;
  static char *S2RRingWidthKeyword;
  static char *S2RLayerWidthKeyword;
  static char *LynxGraphKeyword;
  static char *LynxCapaKeyword;
  static char *LynxResistorKeyword;
  static char *LynxTransistorKeyword;
  static char *LynxDiffusionKeyword;
  static char *LynxBulkImplicitKeyword;       /*4p*/
  static char *BigViaHoleKeyword;
  static char *BigViaMetalKeyword;
  static char *TurnViaKeyword;

  static char *DrcRulesKeword;
  static char *WireSettingKeyword;

  static keyword KeywordDefine [ RPR_MAX_KEYWORD ] =
 
  {
    { "alcw",         RDS_TRANS_ALCW   },
    { "all",          RDS_USE_ALL      },
    { "alu1",         ALU1             },
    { "alu2",         ALU2             },
    { "alu3",         ALU3             },
    { "alu4",         ALU4             },
    { "alu5",         ALU5             },
    { "alu6",         ALU6             },
    { "alu7",         ALU7             },
    { "alu8",         ALU8             },
    { "alu9",         ALU9             },
    { "arcw",         RDS_TRANS_ARCW   },
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
    { "drc",          RDS_USE_DRC      },
    { "explicit",     RDS_EXPLICIT     },        /*4p*/
    { "ext",          RDS_USE_EXTRACT  },
    { "implicit",     RDS_IMPLICIT     },        /*4p*/
    { "lcw",          RDS_TRANS_LCW    },
    { "ndif",         NDIF             },
    { "notreat",      S2R_NOTREAT      },
    { "ntie",         NTIE             },
    { "ntrans",       NTRANS           },
    { "null",         S2R_NOTREAT      },
    { "nwell",        NWELL            },
    { "pdif",         PDIF             },
    { "poly",         POLY             },
    { "poly2",        POLY2            },
    { "ptie",         PTIE             },
    { "ptrans",       PTRANS           },
    { "pwell",        PWELL            },
    { "rcw",          RDS_TRANS_RCW    },
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
    { "treat",        S2R_TREAT        },
    { "vw",           RDS_TRANS_VW     },
    { "vws",          RDS_TRANS_VWS    }
  };
  
/*------------------------------------------------------------\
|                                                             |
|                            Name                             |
|                                                             |
\------------------------------------------------------------*/

  char RDS_TRANS_NAME [ RDS_MAX_TRANS ][ 4 ] =
  
  {
    "VW ",
    "LCW",
    "RCW"
  };
  
  char RDS_USE_NAME [ RDS_MAX_USE ][ 4 ] =
  
  {
    "ALL",
    "EXT",
    "DRC"
  };
   
  char MBK_VIA_NAME [ MBK_MAX_VIA ][ 12 ] =
  
  {  
    "CONT_POLY  ",
    "CONT_DIF_N ",
    "CONT_DIF_P ",
    "CONT_BODY_N",
    "CONT_BODY_P",
    "CONT_VIA   ",
    "CONT_VIA2  ",
    "CONT_VIA3  ",
    "CONT_VIA4  ",
    "CONT_VIA5  ",
    "CONT_VIA6  ",
    "CONT_VIA7  ",
    "CONT_VIA8  ",
    "CONT_POLY2 ",
    "C_X_N      ",
    "C_X_P      ",
    "CONT_TURN1 ",
    "CONT_TURN2 ",
    "CONT_TURN3 ",
    "CONT_TURN4 ",
    "CONT_TURN5 ",
    "CONT_TURN6 ",
    "CONT_TURN7 ",
    "CONT_TURN8 ",
    "CONT_TURN9 "
  };
  
  char MBK_LAYER_NAME [ MBK_MAX_LAYER ][ 12 ] =
  
  {
    "NWELL      ",
    "PWELL      ",
    "NTIE       ",
    "PTIE       ",
    "NDIF       ",
    "PDIF       ",
    "NTRANS     ",
    "PTRANS     ",
    "NTRANS_FAST",
    "PTRANS_FAST",
    "NTRANS_HVIO",
    "PTRANS_HVIO",
    "POLY       ",
    "POLY2      ",
    "ALU1       ",
    "ALU2       ",
    "ALU3       ",
    "ALU4       ",
    "ALU5       ",
    "ALU6       ",
    "ALU7       ",
    "ALU8       ",
    "ALU9       ",
    "TPOLY      ",
    "TPOLY2     ",
    "TALU1      ",
    "TALU2      ",
    "TALU3      ",
    "TALU4      ",
    "TALU5      ",
    "TALU6      ",
    "TALU7      ",
    "TALU8      ",
    "TALU9      ",
    "CALU1      ",
    "CALU2      ",
    "CALU3      ",
    "CALU4      ",
    "CALU5      ",
    "CALU6      ",
    "CALU7      ",
    "CALU8      ",
    "CALU9      "
  };

#define MBK_MAX_WIRESETTING_TLEN 23
  char MBK_WIRESETTING_NAME [ MBK_MAX_WIRESETTING ][ MBK_MAX_WIRESETTING_TLEN ] =
  
  {
    "track_spacing_alu1",
    "track_spacing_alu2",
    "track_spacing_alu3",
    "track_spacing_alu4",
    "track_spacing_alu5",
    "track_spacing_alu6",
    "track_spacing_alu7",
    "track_spacing_alu8",
    "track_width_alu1",
    "track_width_alu2",
    "track_width_alu3",
    "track_width_alu4",
    "track_width_alu5",
    "track_width_alu6",
    "track_width_alu7",
    "track_width_alu8",
    "width_vdd",
    "width_vss",
    "x_grid",
    "y_grid",
    "y_slice"
  };

/*------------------------------------------------------------\
|                                                             |
|                       File variables                        |
|                                                             |
\------------------------------------------------------------*/

  static  FILE *RprFile;
  static  char  RprBuffer[ RPR_MAX_BUFFER ];
  static  long  RprCurrentLine = 0;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|              Rpr Ascii to float (small numbers)             |
|                                                             |
\------------------------------------------------------------*/

double RprAtof( integer )

  char *integer;
{
  char *decimal;
  long shift;
  long i;

  if ((decimal = strchr( integer, '.')) != (char *)0 )
  {
    *decimal = '\0';

    decimal++;

    for(i=strlen(decimal),shift=1 ; i; shift*=10, i--);

    if (atol(integer) < 0)
    {
      return (double)(atol(integer)*shift - atol(decimal))/(double)(shift);
    }
    else
    {
      return (double)(atol(integer)*shift + atol(decimal))/(double)(shift);
    }
  } 
  else 
  {
    return (double)(atol(integer));
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Upper Case                        |
|                                                             |
\------------------------------------------------------------*/

char *RprUpperCase( Name )

  char *Name;
{
  char *Target;
  int   Index;

  if ( Name == (char *)NULL ) return( (char *)NULL );

  Target = (char *)rdsallocblock( strlen( Name ) + 1 );

  for ( Index = 0; Name[ Index ]; Index++ )
  {
    Target[ Index ] = (char)toupper( Name[ Index ] );
  }
  Target[ Index ] = '\0';

  return( Target );
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr File Get String                   |
|                                                             |
\------------------------------------------------------------*/

char *RprFileGetString( String, Size )

  char *String;
  int   Size;
{
  register char *RegisterString;
  register int   Register;
 
  RegisterString = String;
  Register       = 0;
 
  while (--Size > 0 && (Register = getc( RprFile )) != EOF )
  {
    if ((*RegisterString++ = Register) == '\\') 
    {
      if ((Register = getc( RprFile )) == '\n') 
      {
        RprCurrentLine++;
 
        *(RegisterString - 1) = ' ';
      } 
      else
      {
        ungetc( Register, RprFile );
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
|                       Rpr File Get Line                     |
|                                                             |
\------------------------------------------------------------*/

void RprGetLine( Buffer, flags )

  char   *Buffer;
  int     flags;
{
  char   *RprBuffer = Buffer;
  char   *Check;
  char   *String;
  char    OneComment;
 
  do 
  {
    OneComment = 0;
 
    Check = RprFileGetString( Buffer, RPR_MAX_BUFFER );
 
    if ( Check != (char *)NULL ) 
    {
      RprCurrentLine++;
    }
    else 
    {
      if (flags & CAN_REACH_EOF) {
        *RprBuffer = '\0';
        return;
      }
      rprerror( RPR_UNEXPECTED_EOF, (char *)NULL, RprCurrentLine );
    }
 
    if ( ( String = strchr( Buffer, RPR_COMMENT_CHAR )) != (char *)0 )
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
  
    while (*Buffer != '\0' && strchr( RPR_SEPARATORS_STRING, *Buffer))
    {
      Buffer = Buffer + 1;
    }
 
    if (*Buffer == '\0') OneComment = 1;
 
  } 
  while ( OneComment == 1);
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Check Keyword                    |
|                                                             |
\------------------------------------------------------------*/

char RprCheckKeyword( Word )

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
|                        Rpr Get First word                   |
|                                                             |
\------------------------------------------------------------*/

char *RprGetFirstWord( Buffer, IsKeyword )
 
  char *Buffer;
  char  IsKeyword;
{
  char   *String;
 
  if ( ( String = (char *)strtok( Buffer, RPR_SEPARATORS_STRING ))  != (char *)0 )
  {
    String = namealloc( String );
 
    if ( ( IsKeyword ) && RprCheckKeyword( String ) )
    {
      rprerror( RPR_UNEXPECTED_LINE, String, RprCurrentLine );
    }
  }
  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Get Next word                    |
|                                                             |
\------------------------------------------------------------*/

char *RprGetNextWord( IsKeyword )
 
     char IsKeyword;
{
  char   *String;
 
  if ( ( String = (char *)strtok( (char *)NULL, RPR_SEPARATORS_STRING ))  != (char *)0 )
  {
    String = namealloc( String );
 
    if ( ( IsKeyword ) && RprCheckKeyword( String ) )
    {
      rprerror( RPR_UNEXPECTED_LINE, String, RprCurrentLine );
    }
  }
 
  return( String );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rpr Translate Param                    |
|                                                             |
\------------------------------------------------------------*/

long RprTranslateParam( Param )

    double   Param;
{
  double Multiple; 
  
  Param    = Param * RDS_UNIT;
  Multiple = Param / RDS_PHYSICAL_GRID;
 
  if ( RDS_UNIT == -1 )
  {
    rprerror( RPR_NOT_DEFINED, PhysicalGridKeyword, RprCurrentLine );
  }
  
  if ( (Multiple < (floor(Multiple + (double).5) - RPR_EPSILON )) ||
       (Multiple > (floor(Multiple + (double).5) + RPR_EPSILON )) ) 
  {
    sprintf( RprBuffer, "%g", Param / RDS_UNIT );
 
    rprerror( RPR_MULTIPLE_GRID, RprBuffer, RprCurrentLine );
  }
 
/*
 *fprintf( stderr, "RprTranslateParam() - Param:%.18g rouding:%ld truncated:%ld\n"
 *       , (Param), lround(Param), (long)(Param) );
 */
  return( lround(Param) );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rpr Compute Rds Unit                   |
|                                                             |
\------------------------------------------------------------*/

void RprComputeRdsUnit( PhysicalGrid )

    double PhysicalGrid;
{
  long Number;
  long RdsUnit;
 
  if ( PhysicalGrid <= 0 )
  {
    rprerror( RPR_NEGATIVE_VALUE, PhysicalGridKeyword, RprCurrentLine );
  }
 
  RdsUnit = 1;
 
  for ( Number = 1; 
        ! RprEqual( PhysicalGrid * (double)RdsUnit, (double)Number ); 
        Number = Number + 1 )
  {
    if ( RdsUnit > RPR_MAX_RDS_UNIT ) 
    {
      rprerror( RPR_TOO_SMAL, PhysicalGridKeyword, RprCurrentLine );
    }
 
    for ( RdsUnit = RdsUnit - 1; 
          PhysicalGrid * (double)RdsUnit + RPR_EPSILON <  (double)Number; 
          RdsUnit = RdsUnit + 1 );
 
    if ( RprEqual( PhysicalGrid * (double)RdsUnit, (double)Number )) 
 
       break;
  }
 
  RDS_UNIT          = RdsUnit << 1;
  RDS_PHYSICAL_GRID = Number  << 1;
  RDS_UNIT2         = RDS_UNIT * RDS_UNIT;
}

/*------------------------------------------------------------\
|                                                             |
|                      Rpr Keyword Compare                    |
|                                                             |
\------------------------------------------------------------*/

int RprKeywordCompare( FirstKey, SecondKey )

    keyword *FirstKey;
    keyword *SecondKey;
{
  return strcmp( FirstKey->NAME, SecondKey->NAME );
}

/*------------------------------------------------------------\
|                                                             |
|                      Rpr Get String Value                   |
|                                                             |
\------------------------------------------------------------*/

long RprGetStringValue( String )

  char     *String;
{
  double    FloatValue;
  long      Value;
  keyword  *Keyword;
  keyword   Entry;
 
  if ( strchr( String, '.' )) 
  {
    if ( sscanf( String, "%32lf", &FloatValue) ) 
     
      return( RprTranslateParam( FloatValue ) );
 
    rprerror( RPR_ILLEGAL_FLOAT, String, RprCurrentLine );
  } 
  else
  {
    if ( sscanf( String, "%22ld", &Value) )
 
     return ( RprTranslateParam( (double)Value) );
 
    Entry.NAME = String;

    Keyword = (keyword *)bsearch( (char *)(&Entry), 
                                  (char *)KeywordDefine, 
                                  RPR_MAX_KEYWORD, sizeof( keyword ),
                                  RprKeywordCompare );
 
    if ( Keyword != (keyword *)NULL )
    {

      if ( ! strncmp( String, "rds", 3 ) )
      {
        RDS_DYNAMIC_LAYER[ (int)Keyword->VALUE ] = 1;
      }

      return( Keyword->VALUE );
    }
 
    rprerror( RPR_UNEXPECTED_LINE, String, RprCurrentLine );
  }
 
  return( -1 ); 
}

/*------------------------------------------------------------\
|                                                             |
|                      Rpr Get String Float                   |
|                                                             |
\------------------------------------------------------------*/

float RprGetStringFloat( String )

  char     *String;
{
  float FloatValue;
 
  if ( sscanf( String, "%32f", &FloatValue) )
 
    return( FloatValue );
 
  rprerror( RPR_ILLEGAL_FLOAT, String, RprCurrentLine );
 
  return( 0.0 ); 
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Get Number                       |
|                                                             |
\------------------------------------------------------------*/

long RprGetNumber( String )

     char *String;
{
  long Value;

  if ( sscanf( String, "%22ld", &Value )) return Value;

  rprerror( RPR_UNEXPECTED_LINE, "number", RprCurrentLine );

  return( -1 );
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Skip Table                       |
|                                                             |
\------------------------------------------------------------*/

void RprSkipTable()

{
  char  EndTable;
  char *FirstWord;

  EndTable = 0;

  while ( EndTable != 1 )
  {
    RprGetLine( RprBuffer, NO_FLAGS );

    FirstWord = RprGetFirstWord( RprBuffer, 1 );

    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Segment                     |
|                                                             |
\------------------------------------------------------------*/

void RprReadSegment()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord; 
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
 
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                 ) &&
              ( Field     <  RDS_SEGMENT_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field % 6 != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_SEGMENT_TABLE [ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_SEGMENT_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Connector                   |
|                                                             |
\------------------------------------------------------------*/

void RprReadConnector()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_CONNECTOR_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field % 3 != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_CONNECTOR_TABLE [ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_CONNECTOR_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Reference                   |
|                                                             |
\------------------------------------------------------------*/

void RprReadReference()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1                 ) &&
          ( LayerCount <= MBK_MAX_REFERENCE ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_REFERENCE )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_REFERENCE_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( ( Field & 1 ) != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_REFERENCE_TABLE [ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_REFERENCE_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          Rpr Read Via                       |
|                                                             |
\------------------------------------------------------------*/

void RprReadVia()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1               ) &&
              ( Field     <  RDS_VIA_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field % 3 != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_VIA_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_VIA_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        } 
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Rpr Read Big Via Hole               |
|                                                             |
\------------------------------------------------------------*/

void RprReadBigViaHole()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                     ) &&
              ( Field     <  RDS_BIGVIA_HOLE_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field % 4 != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_BIGVIA_HOLE_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_BIGVIA_HOLE_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        } 
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         Rpr Read Big Via Metal              |
|                                                             |
\------------------------------------------------------------*/

void RprReadBigViaMetal()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                     ) &&
              ( Field     <  RDS_BIGVIA_METAL_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field % 4 != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_BIGVIA_METAL_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_BIGVIA_METAL_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        } 
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                          Rpr Read Turn Via                  |
|                                                             |
\------------------------------------------------------------*/

void RprReadTurnVia()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1           ) &&
          ( LayerCount <= MBK_MAX_VIA ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_VIA )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                 ) &&
              ( Field     <  RDS_TURNVIA_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field % 3 != 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_TURNVIA_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_TURNVIA_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        } 
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Cif Layer                    |
|                                                             |
\------------------------------------------------------------*/

void RprReadCifLayer()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_CIF_LAYER_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_CIF_LAYER_TABLE[ Layer ][ Field ] = RprUpperCase( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_CIF_LAYER_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Gds Layer                   |
|                                                             |
\------------------------------------------------------------*/

void RprReadGdsLayer()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_GDS_LAYER_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_GDS_LAYER_TABLE[ Layer ][ Field ] = atoi( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_GDS_LAYER_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}



/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Wire Settings               |
|                                                             |
\------------------------------------------------------------*/

int RprReadWireComp(const void *x , const void *y) {
	return strncmp((char*)x, (char*)y,MBK_MAX_WIRESETTING_TLEN);
}

void RprReadWireSetting()

{
  int   Layer;
  int   LayerCount;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
  void *res;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_WIRESETTING ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    Layer = (char (*)[MBK_MAX_WIRESETTING_TLEN])bsearch(FirstWord, MBK_WIRESETTING_NAME[0],
	MBK_MAX_WIRESETTING, MBK_MAX_WIRESETTING_TLEN, RprReadWireComp) - MBK_WIRESETTING_NAME;
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_WIRESETTING )
    {
      EndRecord = 0;
 
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword || Layer > MBK_MAX_WIRESETTING || Layer < 0)
        {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
  
          EndRecord = 1;
        }
        else
        {
          *RDS_WIRESETTING_TABLE[ Layer ] = atol( FirstWord );
  
        }
  
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Post Treat                  |
|                                                             |
\------------------------------------------------------------*/

void RprReadS2RPostTreat()

{
  int          Layer;
  int          LayerCount;
  int          EndTable;
  int          EndRecord;
  int          Field;
  char        *FirstWord; 
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
 
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER ) 
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                    ) &&
              ( Field     <  RDS_S2R_POST_TREAT_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
 
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 ) 
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
 
          EndRecord = 1;
        }
        else 
        {
          RDS_S2R_POST_TREAT_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
 
          Field = Field + 1;
        }
      } 
 
      if ( Field == RDS_S2R_POST_TREAT_FIELD ) 
      {
        FirstWord = RprGetNextWord( 0 );
 
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      } 
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Over Size                   |
|                                                             |
\------------------------------------------------------------*/

void RprReadS2ROversize()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_S2R_OVERSIZE_FIELD  ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_S2R_OVERSIZE_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_S2R_OVERSIZE_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                        Rpr Read Ring Width                  |
|                                                             |
\------------------------------------------------------------*/

void RprReadS2RRingWidth()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                     ) &&
              ( Field     <  RDS_S2R_RING_WIDTH_FIELD  ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_S2R_RING_WIDTH_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_S2R_RING_WIDTH_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Layer Width                  |
|                                                             |
\------------------------------------------------------------*/

void RprReadS2RLayerWidth()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                      ) &&
              ( Field     <  RDS_S2R_LAYER_WIDTH_FIELD  ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_S2R_LAYER_WIDTH_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_S2R_LAYER_WIDTH_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Lynx Graph                   |
|                                                             |
\------------------------------------------------------------*/

void RprReadLynxGraph()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                    ) &&
              ( Field     <  RDS_LYNX_GRAPH_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_LYNX_GRAPH_TABLE [ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_LYNX_GRAPH_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Lynx Diffusion               |
|                                                             |
\------------------------------------------------------------*/

void RprReadLynxDiffusion()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                        ) &&
              ( Field     <  RDS_LYNX_DIFFUSION_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field & 0x1 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          if ( Field & 0x1 )
          {
            RDS_LYNX_DIFFUSION_TABLE [ Layer ][ Field ] = atoi( FirstWord );
          }
          else
          {
            RDS_LYNX_DIFFUSION_TABLE [ Layer ][ Field ] = RprGetStringValue( FirstWord );
          }
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_LYNX_DIFFUSION_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Lynx Capa                    |
|                                                             |
\------------------------------------------------------------*/

void RprReadLynxCapa()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_LYNX_CAPA_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          if ( Field == 0 )
          {
            RDS_LYNX_CAPA_TABLE [ Layer ][ 0 ] = 
 
              RprGetStringFloat( FirstWord ) / RDS_UNIT2;
          }
          else
          {
            RDS_LYNX_CAPA_TABLE [ Layer ][ Field ] = 
 
              RprGetStringFloat( FirstWord ) / RDS_UNIT;
          }
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_LYNX_CAPA_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Lynx Resistor                |
|                                                             |
\------------------------------------------------------------*/

void RprReadLynxResistor()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                       ) &&
              ( Field     <  RDS_LYNX_RESISTOR_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          EndRecord = 1;
        }
        else
        {
          RDS_LYNX_RESISTOR_TABLE [ Layer ][ Field ] = 

            RprGetStringFloat( FirstWord );

          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_LYNX_RESISTOR_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       Rpr Read Lynx Transistor              |
|                                                             |
\------------------------------------------------------------*/

void RprReadLynxTransistor()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= MBK_MAX_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < MBK_MAX_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                         ) &&
              ( Field     <  RDS_LYNX_TRANSISTOR_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field < RDS_LYNX_TRANSISTOR_FIELD )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_LYNX_TRANSISTOR_TABLE [ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_LYNX_TRANSISTOR_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 Rpr Read Lynx Bulk Implicit                 |
|                                                             |
\------------------------------------------------------------*/

void RprReadLynxBulkImplicit()

{
  int   Layer;
  int   LayerCount;
  int   Field;
  int   EndTable;
  int   EndRecord;
  char *FirstWord;
 
  EndTable   = 0;
  LayerCount = 0;
 
  while ( ( EndTable   != 1             ) &&
          ( LayerCount <= RDS_ALL_LAYER ) )
  {
    RprGetLine( RprBuffer, NO_FLAGS );
 
    FirstWord  = RprGetFirstWord( RprBuffer, 1 );
    
    if ( FirstWord == EndTableKeyword )
    {
      EndTable = 1;
    }
    else
    if ( LayerCount < RDS_ALL_LAYER )
    {
      Layer     = RprGetStringValue( FirstWord );
      EndRecord = 0;
      Field     = 0;
 
      while ( ( EndRecord != 1                   ) &&
              ( Field     <  RDS_LYNX_BULK_IMPLICIT_FIELD ) )
      {
        FirstWord = RprGetNextWord( 1 );
  
        if ( FirstWord == EndRecordKeyword )
        {
          if ( Field == 0 )
          {
            rprerror( RPR_MISSING_VALUE, (char *)NULL, RprCurrentLine );
          }
  
          EndRecord = 1;
        }
        else
        {
          RDS_LYNX_BULK_IMPLICIT_TABLE[ Layer ][ Field ] = RprGetStringValue( FirstWord );
  
          Field = Field + 1;
        }
      }
  
      if ( Field == RDS_LYNX_BULK_IMPLICIT_FIELD )
      {
        FirstWord = RprGetNextWord( 0 );
  
        if ( FirstWord != EndRecordKeyword )
        {
          rprerror( RPR_TOO_MANY_WORDS, FirstWord, RprCurrentLine );
        }
      }
    }
  
    LayerCount = LayerCount + 1;
  }
 
  if ( EndTable == 0 )
  {
    rprerror( RPR_LINE_EXPECTED, EndTableKeyword, RprCurrentLine);
  }
}
/*------------------------------------------------------------\
|                                                             |
|                      Rpr Read Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void RprReadParam()

{
  char   *FirstWord;
  char   *SecondWord;
  long    Continue;
  double  FloatValue;

  Continue = 0;

  while ( Continue != RPR_ALL_DEFINED_MASK )  
  {
    RprGetLine( RprBuffer, CAN_REACH_EOF );
    if (    ( (Continue & RPR_ALL_REQUIRED_MASK) == RPR_ALL_REQUIRED_MASK ) 
         && ( RprBuffer[0] == '\0' ) ) break;

    FirstWord = RprGetFirstWord( RprBuffer, 0);

    if ( FirstWord == DefineKeyword ) 
    {
      FirstWord = RprGetNextWord( 1 );

      if (! FirstWord )
      {
        rprerror( RPR_MISSING_NAME, DefineKeyword, RprCurrentLine );
      }

      SecondWord = RprGetNextWord(1);

      if (! SecondWord ) 
      {
        rprerror( RPR_MISSING_VALUE, SecondWord, RprCurrentLine );
      }

      if ( FirstWord == PhysicalGridKeyword )
      {
        sscanf( SecondWord, "%32lf", &FloatValue); 

        RprComputeRdsUnit( FloatValue );

        Continue |= RPR_PHYSICAL_GRID_MASK;
      }
      else
      if ( FirstWord == LambdaKeyword )
      {
        sscanf( SecondWord, "%32lf", &FloatValue); 

        if ( FloatValue <= 0 )
        {
          rprerror( RPR_NEGATIVE_VALUE, LambdaKeyword, RprCurrentLine );
        }

        RDS_LAMBDA  = RprTranslateParam( FloatValue );
        RDS_LAMBDA2 = RDS_LAMBDA * RDS_LAMBDA;

        Continue |= RPR_LAMBDA_MASK;
      }
      else
      {
        rprerror( RPR_UNKNOWN_DEFINE, FirstWord, RprCurrentLine );
      }
    } 
    else 
    if ( FirstWord == TableKeyword )
    {
      if (!( FirstWord = RprGetNextWord(1)))
      {
        rprerror( RPR_MISSING_NAME, TableKeyword, RprCurrentLine );
      }

      if ( FirstWord == S2RPostTreatKeyword )
      {
        RprReadS2RPostTreat();

        Continue |= RPR_S2R_POST_TREAT_MASK;
      }
      else
      if ( FirstWord == SegmentKeyword )
      {
        RprReadSegment();

        Continue |= RPR_SEGMENT_MASK;
      } 
      else
      if ( FirstWord == ConnectorKeyword )
      {
        RprReadConnector();

        Continue |= RPR_CONNECTOR_MASK;
      }else
      if ( FirstWord == ReferenceKeyword )
      {
        RprReadReference();

        Continue |= RPR_REFERENCE_MASK;
      }  
      else
      if ( FirstWord == ViaKeyword )
      {
        RprReadVia();

        Continue |= RPR_VIA_MASK;
      }  
      else
      if ( FirstWord == CifLayerKeyword )
      {
        RprReadCifLayer();

        Continue |= RPR_CIF_LAYER_MASK;
      }  
      else
      if ( FirstWord == GdsLayerKeyword )  
      {
        RprReadGdsLayer();

        Continue |= RPR_GDS_LAYER_MASK;
      }
      else
      if ( FirstWord == S2ROversizeKeyword )
      {
        RprReadS2ROversize();

        Continue |= RPR_S2R_OVERSIZE_MASK;
      }
      else
      if ( FirstWord == S2RRingWidthKeyword )
      {
        RprReadS2RRingWidth();

        Continue |= RPR_S2R_RING_WIDTH_MASK;
      }
      else
      if ( FirstWord == S2RLayerWidthKeyword )
      {
        RprReadS2RLayerWidth();

        Continue |= RPR_S2R_LAYER_WIDTH_MASK;
      }
      else
      if ( FirstWord == LynxGraphKeyword )
      {
        RprReadLynxGraph();

        Continue |= RPR_LYNX_GRAPH_MASK;
      }
      else
      if ( FirstWord == LynxDiffusionKeyword )
      {
        RprReadLynxDiffusion();

        Continue |= RPR_LYNX_DIFFUSION_MASK;
      }
      else
      if ( FirstWord == LynxCapaKeyword )
      {
        RprReadLynxCapa();

        Continue |= RPR_LYNX_CAPA_MASK;
      }
      else
      if ( FirstWord == LynxResistorKeyword )
      {
        RprReadLynxResistor();

        Continue |= RPR_LYNX_RESISTOR_MASK;
      }
      else
      if ( FirstWord == LynxTransistorKeyword )
      {
        RprReadLynxTransistor();

        Continue |= RPR_LYNX_TRANSISTOR_MASK;
      }
      else                                            /*4p*/
      if ( FirstWord == LynxBulkImplicitKeyword )     /*  */
      {                                               /*  */
        RprReadLynxBulkImplicit();                    /*  */
                                                      /*  */
        Continue |= RPR_LYNX_BULK_IMPLICIT_MASK;      /*  */
      }                                               /*4p*/
      else                                            /*4p*/
      if ( FirstWord == BigViaHoleKeyword )
      {
        RprReadBigViaHole();

        Continue |= RPR_BIGVIA_HOLE_MASK;
      }
      else
      if ( FirstWord == BigViaMetalKeyword )
      {
        RprReadBigViaMetal();

        Continue |= RPR_BIGVIA_METAL_MASK;
      }
      else
      if ( FirstWord == TurnViaKeyword )
      {
        RprReadTurnVia();

        Continue |= RPR_TURNVIA_MASK;
      }
      else
      if ( FirstWord == WireSettingKeyword )
      {
        RprReadWireSetting();

        Continue |= RPR_WIRESETTING_MASK;
      }
      else RprSkipTable();
    }
    else
    {
      if ( FirstWord != DrcRulesKeword )
      {
        rprerror( RPR_SYNTAX_ERROR, FirstWord, RprCurrentLine  );
      }
      else
      {
	if ( (Continue & RPR_ALL_REQUIRED_MASK) == RPR_ALL_REQUIRED_MASK ) return;
        rprerror( RPR_UNEXPECTED_EOF, (char *)0, RprCurrentLine  );
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Rpr Post Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void RprPostParam()

{
  int  Field;
  int  Layer;
  int  Dynamic;
  int  Convert;

  for ( Layer = RDS_LAYER_USER0; Layer <= RDS_LAYER_ABOX; Layer++ )
  {
    RDS_DYNAMIC_LAYER[ Layer ] = 1;
  }

  Dynamic = 0;

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    if ( RDS_DYNAMIC_LAYER[ Layer ] == 1 )
    {
      RDS_DYNAMIC_LAYER[ Layer ] = Dynamic++;
    }
  }

  RDS_MAX_LAYER  = Dynamic;

  if ( RDS_LAYER_NAME != RDS_ALL_LAYER_NAME )
  {
    rdsfreeblock( RDS_LAYER_NAME );
  }

  if ( RDS_STATIC_LAYER != RDS_DYNAMIC_LAYER )
  {
    rdsfreeblock( RDS_STATIC_LAYER );
  }

  RDS_LAYER_NAME   = (char          **)rdsallocblock( sizeof( char *) * RDS_MAX_LAYER );
  RDS_STATIC_LAYER = (unsigned char  *)rdsallocblock( sizeof( char  ) * RDS_MAX_LAYER );

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    Dynamic = RDS_DYNAMIC_LAYER[ Layer ];

    if ( Dynamic != RDS_LAYER_UNUSED )
    {
      RDS_LAYER_NAME[ Dynamic ]   = RDS_ALL_LAYER_NAME[ Layer ];
      RDS_STATIC_LAYER[ Dynamic ] = Layer;

      for ( Field = 0; Field < RDS_S2R_POST_TREAT_FIELD; Field++ )
      {
        Convert = RDS_S2R_POST_TREAT_TABLE [ Layer ][ Field ];

        if ( ( Field    == 1                       ) &&
             ( Convert != RDS_S2R_POST_TREAT_EMPTY ) )
        {
          Convert = RDS_DYNAMIC_LAYER[ Convert ];
        }

        RDS_S2R_POST_TREAT_TABLE [ Dynamic ][ Field ] = Convert;
      }

      for ( Field = 0; Field < RDS_GDS_LAYER_FIELD; Field++ )
      {
        RDS_GDS_LAYER_TABLE [ Dynamic ][ Field ] =
        RDS_GDS_LAYER_TABLE [ Layer   ][ Field ];
      }

      for ( Field = 0; Field < RDS_CIF_LAYER_FIELD; Field++ )
      {
        RDS_CIF_LAYER_TABLE [ Dynamic ][ Field ] =
        RDS_CIF_LAYER_TABLE [ Layer   ][ Field ];
      }

      for ( Field = 0; Field < RDS_LYNX_BULK_IMPLICIT_FIELD; Field++ )   /*4p*/
      {                                                                  /*  */
        RDS_LYNX_BULK_IMPLICIT_TABLE [ Dynamic ][ Field ] =              /*  */
        RDS_LYNX_BULK_IMPLICIT_TABLE [ Layer   ][ Field ];               /*  */
      }                                                                  /*4p*/

      for ( Field = 0; Field < RDS_S2R_OVERSIZE_FIELD; Field++ )
      {
        RDS_S2R_OVERSIZE_TABLE [ Dynamic ][ Field ] =
        RDS_S2R_OVERSIZE_TABLE [ Layer   ][ Field ];
      }

      for ( Field = 0; Field < RDS_S2R_RING_WIDTH_FIELD; Field++ )
      {
        RDS_S2R_RING_WIDTH_TABLE [ Dynamic ][ Field ] =
        RDS_S2R_RING_WIDTH_TABLE [ Layer   ][ Field ];
      }

      for ( Field = 0; Field < RDS_S2R_LAYER_WIDTH_FIELD; Field++ )
      {
        RDS_S2R_LAYER_WIDTH_TABLE [ Dynamic ][ Field ] =
        RDS_S2R_LAYER_WIDTH_TABLE [ Layer   ][ Field ];
      }

      for ( Field = 0; Field < RDS_LYNX_GRAPH_FIELD; Field++ )
      {
        Convert = RDS_LYNX_GRAPH_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_LYNX_GRAPH_EMPTY )
        {
          Convert = RDS_DYNAMIC_LAYER[ Convert ];
        }

        RDS_LYNX_GRAPH_TABLE [ Dynamic ][ Field ] = Convert;
      }

      for ( Field = 0; Field < RDS_LYNX_DIFFUSION_FIELD; Field++ )
      {
        Convert = RDS_LYNX_DIFFUSION_TABLE [ Layer ][ Field ];

        if ( ( Field & 0x1 ) == 0 )
        {
          if ( Convert != RDS_LYNX_DIFFUSION_EMPTY )
          {
            Convert = RDS_DYNAMIC_LAYER[ Convert ];
          }
        }

        RDS_LYNX_DIFFUSION_TABLE [ Dynamic ][ Field ] = Convert;
      }

      for ( Field = 0; Field < RDS_LYNX_CAPA_FIELD; Field++ )
      {
        RDS_LYNX_CAPA_TABLE[ Dynamic ][ Field ] =
        RDS_LYNX_CAPA_TABLE[ Layer   ][ Field ];
      }

      for ( Field = 0; Field < RDS_LYNX_RESISTOR_FIELD; Field++ )
      {
        RDS_LYNX_RESISTOR_TABLE[ Dynamic ][ Field ] =
        RDS_LYNX_RESISTOR_TABLE[ Layer   ][ Field ];
      }
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    for ( Field = 0; Field < RDS_SEGMENT_FIELD; Field++ )
    {
      if ( Field % 6 == 0 )
      {
        Convert = RDS_SEGMENT_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_SEGMENT_EMPTY )
        {
          RDS_SEGMENT_TABLE[ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }

    for ( Field = 0; Field < RDS_LYNX_TRANSISTOR_FIELD; Field++ )
    {
      if ( Field >= 2 )
      {
        Convert = RDS_LYNX_TRANSISTOR_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_LYNX_TRANSISTOR_EMPTY )
        {
          RDS_LYNX_TRANSISTOR_TABLE[ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }

    for ( Field = 0; Field < RDS_CONNECTOR_FIELD; Field++ )
    {
      if ( Field == 0 )
      {
        Convert = RDS_CONNECTOR_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_CONNECTOR_EMPTY )
        {
          RDS_CONNECTOR_TABLE [ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    for ( Field = 0; Field < RDS_REFERENCE_FIELD; Field++ )
    {
      if ( ( Field & 1 ) == 0 )
      {
        Convert = RDS_REFERENCE_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_REFERENCE_EMPTY )
        {
          RDS_REFERENCE_TABLE [ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_VIA_FIELD; Field++ )
    {
      if ( Field % 3 == 0 )
      {
        Convert = RDS_VIA_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_VIA_EMPTY )
        {
          RDS_VIA_TABLE [ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_BIGVIA_HOLE_FIELD; Field++ )
    {
      if ( Field % 4 == 0 )
      {
        Convert = RDS_BIGVIA_HOLE_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_BIGVIA_HOLE_EMPTY )
        {
          RDS_BIGVIA_HOLE_TABLE [ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_BIGVIA_METAL_FIELD; Field++ )
    {
      if ( Field % 4 == 0 )
      {
        Convert = RDS_BIGVIA_METAL_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_BIGVIA_METAL_EMPTY )
        {
          RDS_BIGVIA_METAL_TABLE [ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_TURNVIA_FIELD; Field++ )
    {
      if ( Field % 3 == 0 )
      {
        Convert = RDS_TURNVIA_TABLE [ Layer ][ Field ];

        if ( Convert != RDS_TURNVIA_EMPTY )
        {
          RDS_TURNVIA_TABLE [ Layer ][ Field ] = RDS_DYNAMIC_LAYER[ Convert ];
        }
      }
    }
  }
}

/*------------------------------------------------------------\
|                                                             |
|                      Rds Load Parameters                    |
|                                                             |
\------------------------------------------------------------*/

void loadrdsparam()

{
  int  Field;
  int  Layer;

  if ( ! KeywordDefined )
  {
    DefineKeyword         = namealloc( DEFINE_KEYWORD          );
    TableKeyword          = namealloc( TABLE_KEYWORD           );
    EndTableKeyword       = namealloc( END_TABLE_KEYWORD       );
    EndRecordKeyword      = namealloc( END_RECORD_KEYWORD      );
    LambdaKeyword         = namealloc( LAMBDA_KEYWORD          );
    PhysicalGridKeyword   = namealloc( PHYSICAL_GRID_KEYWORD   );
    SegmentKeyword        = namealloc( SEGMENT_KEYWORD         );
    ViaKeyword            = namealloc( VIA_KEYWORD             );
    ConnectorKeyword      = namealloc( CONNECTOR_KEYWORD       );
    ReferenceKeyword      = namealloc( REFERENCE_KEYWORD       );
    CifLayerKeyword       = namealloc( CIF_LAYER_KEYWORD       );
    GdsLayerKeyword       = namealloc( GDS_LAYER_KEYWORD       );
    S2ROversizeKeyword    = namealloc( S2R_OVERSIZE_KEYWORD    );
    S2RRingWidthKeyword   = namealloc( S2R_RING_WIDTH_KEYWORD  );
    S2RLayerWidthKeyword  = namealloc( S2R_LAYER_WIDTH_KEYWORD );
    S2RPostTreatKeyword   = namealloc( S2R_POST_TREAT_KEYWORD  );
    LynxGraphKeyword      = namealloc( LYNX_GRAPH_KEYWORD      );
    LynxCapaKeyword       = namealloc( LYNX_CAPA_KEYWORD       );
    LynxResistorKeyword   = namealloc( LYNX_RESISTOR_KEYWORD   );
    LynxTransistorKeyword = namealloc( LYNX_TRANSISTOR_KEYWORD );
    LynxDiffusionKeyword  = namealloc( LYNX_DIFFUSION_KEYWORD  );
    LynxBulkImplicitKeyword = namealloc( LYNX_BULK_IMPLICIT_KEYWORD );  /*4p*/
    BigViaHoleKeyword       = namealloc( BIGVIA_HOLE_KEYWORD  );
    BigViaMetalKeyword      = namealloc( BIGVIA_METAL_KEYWORD );
    TurnViaKeyword          = namealloc( TURNVIA_KEYWORD      );

    DrcRulesKeword = namealloc( DRC_RULES_KEYWORD );
    WireSettingKeyword      = namealloc( WIRESETTING_KEYWORD );

    KeywordDefined = 1;
  }

  RDS_UNIT = -1;

  for ( Layer = 0; Layer < RDS_ALL_LAYER; Layer++ )
  {
    RDS_DYNAMIC_LAYER[ Layer ] = RDS_LAYER_UNUSED;

    for ( Field = 0; Field < RDS_S2R_POST_TREAT_FIELD; Field++ )
    {
      RDS_S2R_POST_TREAT_TABLE [ Layer ][ Field ] = RDS_S2R_POST_TREAT_EMPTY;
    }

    for ( Field = 0; Field < RDS_GDS_LAYER_FIELD; Field++ )
    {
      RDS_GDS_LAYER_TABLE [ Layer ][ Field ] = RDS_GDS_LAYER_EMPTY;
    }

    for ( Field = 0; Field < RDS_CIF_LAYER_FIELD; Field++ )
    {
      RDS_CIF_LAYER_TABLE [ Layer ][ Field ] = RDS_CIF_LAYER_EMPTY;
    }

    for ( Field = 0; Field < RDS_S2R_OVERSIZE_FIELD; Field++ )
    {
      RDS_S2R_OVERSIZE_TABLE [ Layer ][ Field ] = RDS_S2R_OVERSIZE_EMPTY;
    }

    for ( Field = 0; Field < RDS_S2R_RING_WIDTH_FIELD; Field++ )
    {
      RDS_S2R_RING_WIDTH_TABLE [ Layer ][ Field ] = RDS_S2R_RING_WIDTH_EMPTY;
    }

    for ( Field = 0; Field < RDS_S2R_LAYER_WIDTH_FIELD; Field++ )
    {
      RDS_S2R_LAYER_WIDTH_TABLE [ Layer ][ Field ] = RDS_S2R_LAYER_WIDTH_EMPTY;
    }


    for ( Field = 0; Field < RDS_LYNX_BULK_IMPLICIT_FIELD; Field++ )    /*4p*/
    {                                                                   /*  */
      RDS_LYNX_BULK_IMPLICIT_TABLE [ Layer ][ Field ] =                 /*  */
                RDS_LYNX_BULK_IMPLICIT_EMPTY;                           /*  */
    }                                                                   /*4p*/

    for ( Field = 0; Field < RDS_LYNX_GRAPH_FIELD; Field++ )
    {
      RDS_LYNX_GRAPH_TABLE [ Layer ][ Field ] = RDS_LYNX_GRAPH_EMPTY;
    }

    for ( Field = 0; Field < RDS_LYNX_DIFFUSION_FIELD; Field++ )
    {
      RDS_LYNX_DIFFUSION_TABLE [ Layer ][ Field ] = RDS_LYNX_DIFFUSION_EMPTY;
    }

    for ( Field = 0; Field < RDS_LYNX_CAPA_FIELD; Field++ )
    {
      RDS_LYNX_CAPA_TABLE[ Layer ][ Field ] = RDS_LYNX_CAPA_EMPTY;
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_LAYER; Layer++ )
  {
    for ( Field = 0; Field < RDS_SEGMENT_FIELD; Field++ )
    {
      RDS_SEGMENT_TABLE [ Layer ][ Field ] = RDS_SEGMENT_EMPTY;
    }

    for ( Field = 0; Field < RDS_LYNX_TRANSISTOR_FIELD; Field++ )
    {
      RDS_LYNX_TRANSISTOR_TABLE [ Layer ][ Field ] = RDS_LYNX_TRANSISTOR_EMPTY;
    }

    for ( Field = 0; Field < RDS_CONNECTOR_FIELD; Field++ )
    {
      RDS_CONNECTOR_TABLE [ Layer ][ Field ] = RDS_CONNECTOR_EMPTY;
    }
  }
  
  for ( Layer = 0; Layer < MBK_MAX_REFERENCE; Layer++ )
  {
    for ( Field = 0; Field < RDS_REFERENCE_FIELD; Field++ )
    {
      RDS_REFERENCE_TABLE [ Layer ][ Field ] = RDS_REFERENCE_EMPTY;
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_VIA_FIELD; Field++ )
    {
      RDS_VIA_TABLE [ Layer ][ Field ] = RDS_VIA_EMPTY;
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_BIGVIA_HOLE_FIELD; Field++ )
    {
      RDS_BIGVIA_HOLE_TABLE [ Layer ][ Field ] = RDS_BIGVIA_HOLE_EMPTY;
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_BIGVIA_METAL_FIELD; Field++ )
    {
      RDS_BIGVIA_METAL_TABLE [ Layer ][ Field ] = RDS_BIGVIA_METAL_EMPTY;
    }
  }

  for ( Layer = 0; Layer < MBK_MAX_VIA; Layer++ )
  {
    for ( Field = 0; Field < RDS_TURNVIA_FIELD; Field++ )
    {
      RDS_TURNVIA_TABLE [ Layer ][ Field ] = RDS_TURNVIA_EMPTY;
    }
  }

  if ( !( RprFile = fopen( RDS_TECHNO_NAME, "r")))

    rprerror( RPR_OPEN_FILE, NULL, 0);

  RprReadParam();
  RprPostParam();
 
  fclose( RprFile );
}
