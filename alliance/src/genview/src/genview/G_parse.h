/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
| File    :                   G_parse.h                       |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   05.03.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                             Constants                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef G_PARSE_H 
# define G_PARSE_H 

# define GPARSE_MAX_BUFFER        255
# define GPARSE_MAX_KEYWORD       149

# define GPARSE_SEPARATORS_STRING " \t\n"
# define GPARSE_COMMENT_CHAR      '#'

/*------------------------------------------------------------\
|                                                             |
|                        Keyword Constants                    |
|                                                             |
\------------------------------------------------------------*/

# define DEFINE_KEYWORD               "DEFINE"
# define TABLE_KEYWORD                "TABLE" 
# define END_TABLE_KEYWORD            "END"
# define END_RECORD_KEYWORD           NULL

# define PEEK_BOUND_KEYWORD           "GENVIEW_PEEK_BOUND"

# define SEGMENT_NAME_KEYWORD         "GENVIEW_SEGMENT_NAME"
# define CONNECTOR_NAME_KEYWORD       "GENVIEW_CONNECTOR_NAME"
# define VIA_NAME_KEYWORD             "GENVIEW_VIA_NAME"
# define REFERENCE_NAME_KEYWORD       "GENVIEW_REFERENCE_NAME"
# define SYMMETRY_NAME_KEYWORD        "GENVIEW_SYMMETRY_NAME"
# define ORIENT_NAME_KEYWORD          "GENVIEW_ORIENT_NAME"
# define RDS_LAYER_NAME_KEYWORD       "GENVIEW_RDS_LAYER_NAME"

# define GPARSE_SEGMENT_NAME_MASK           0x0001
# define GPARSE_CONNECTOR_NAME_MASK         0x0002
# define GPARSE_VIA_NAME_MASK               0x0004
# define GPARSE_REFERENCE_NAME_MASK         0x0008
# define GPARSE_SYMMETRY_NAME_MASK          0x0010
# define GPARSE_ORIENT_NAME_MASK            0x0020
# define GPARSE_RDS_LAYER_NAME_MASK         0x0040
# define GPARSE_PEEK_BOUND_MASK             0x0080

# define GPARSE_ALL_DEFINED_MASK            0x00FF

/*------------------------------------------------------------\
|                                                             |
|                            Errors                           |
|                                                             |
\------------------------------------------------------------*/


# define  DEFINE_MISSING  0 
# define  TABLE_MISSING   1 
# define  LINE_EXPECTED   2
# define  UNKNOWN_DEFINE  3 
# define  NOT_DEFINED     4 
# define  TOO_SMAL        5
# define  MISSING_VALUE   6
# define  MISSING_NAME    7
# define  UNEXPECTED_LINE 8 
# define  UNEXPECTED_EOF  9 
# define  TOO_MANY_WORDS  10
# define  MISSING_TABLE   11
# define  OPEN_FILE       12
# define  UNKNOWN_TABLE   13
# define  SYNTAX_ERROR    14
# define  ILLEGAL_FLOAT   15

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct keyword 
  {
    char *NAME; 
    char  VALUE;

  } keyword;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char  *GENVIEW_VIA_NAME_TABLE[ MBK_MAX_VIA ][ 1 ];
  extern char  *GENVIEW_SYMMETRY_NAME_TABLE[ MBK_MAX_SYMMETRY ][ 1 ];
  extern char  *GENVIEW_ORIENT_NAME_TABLE[ MBK_MAX_ORIENT ][ 1 ];
  extern char  *GENVIEW_REFERENCE_NAME_TABLE[ MBK_MAX_REFERENCE ][ 1 ];
  extern char  *GENVIEW_SEGMENT_NAME_TABLE[ MBK_MAX_LAYER ][ 1 ];
  extern char  *GENVIEW_CONNECTOR_NAME_TABLE[ MBK_MAX_LAYER ][ 1 ];
  extern char  *GENVIEW_RDS_LAYER_NAME_TABLE[ RDS_ALL_LAYER ][ 2 ];

  extern long   GENVIEW_PEEK_BOUND;
  extern float  GENVIEW_LOWER_FIGURE_STEP;
  extern float  GENVIEW_LOWER_INSTANCE_STEP;
  extern float  GENVIEW_LOWER_CONNECTOR_STEP;
  extern float  GENVIEW_LOWER_SEGMENT_STEP;
  extern float  GENVIEW_LOWER_REFERENCE_STEP;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

# endif
