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
| Tool    :                     GSB                           |
|                                                             |
| File    :                  Gsbparse.h                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   28.03.95                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GSB_PARSE
# define GSB_PARSE

# define GSB_MAX_BUFFER        255
# define GSB_MAX_KEYWORD       153

# define GSB_SEPARATORS_STRING " \t\n"
# define GSB_COMMENT_CHAR      '#'

/*------------------------------------------------------------\
|                                                             |
|                        Keyword Constants                    |
|                                                             |
\------------------------------------------------------------*/

# define DEFINE_KEYWORD               "DEFINE"
# define TABLE_KEYWORD                "TABLE" 
# define END_TABLE_KEYWORD            "END"
# define END_RECORD_KEYWORD           NULL

# define PEEK_BOUND_KEYWORD           "GRAAL_PEEK_BOUND"
# define LOWER_GRID_STEP_KEYWORD      "GRAAL_LOWER_GRID_STEP"
# define LOWER_FIGURE_STEP_KEYWORD    "GRAAL_LOWER_FIGURE_STEP"
# define LOWER_INSTANCE_STEP_KEYWORD  "GRAAL_LOWER_INSTANCE_STEP"
# define LOWER_CONNECTOR_STEP_KEYWORD "GRAAL_LOWER_CONNECTOR_STEP"
# define LOWER_SEGMENT_STEP_KEYWORD   "GRAAL_LOWER_SEGMENT_STEP"
# define LOWER_REFERENCE_STEP_KEYWORD "GRAAL_LOWER_REFERENCE_STEP"


# define SEGMENT_NAME_KEYWORD         "GRAAL_SEGMENT_NAME"
# define TRANSISTOR_NAME_KEYWORD      "GRAAL_TRANSISTOR_NAME"
# define CONNECTOR_NAME_KEYWORD       "GRAAL_CONNECTOR_NAME"
# define SEGMENT_VALUE_KEYWORD        "GRAAL_SEGMENT_VALUE"
# define VIA_NAME_KEYWORD             "GRAAL_VIA_NAME"
# define BIGVIA_NAME_KEYWORD          "GRAAL_BIGVIA_NAME"
# define BIGVIA_VALUE_KEYWORD         "GRAAL_BIGVIA_VALUE"
# define REFERENCE_NAME_KEYWORD       "GRAAL_REFERENCE_NAME"
# define SYMMETRY_NAME_KEYWORD        "GRAAL_SYMMETRY_NAME"
# define ORIENT_NAME_KEYWORD          "GRAAL_ORIENT_NAME"
# define RDS_LAYER_NAME_KEYWORD       "GRAAL_RDS_LAYER_NAME"
# define CURSOR_COLOR_NAME_KEYWORD    "GRAAL_CURSOR_COLOR_NAME"
# define CURSOR_SIZE_KEYWORD          "GRAAL_CURSOR_SIZE"

# define GSB_SEGMENT_NAME_MASK           0x00001
# define GSB_SEGMENT_VALUE_MASK          0x00002
# define GSB_TRANSISTOR_NAME_MASK        0x00004
# define GSB_CONNECTOR_NAME_MASK         0x00008
# define GSB_VIA_NAME_MASK               0x00010
# define GSB_REFERENCE_NAME_MASK         0x00020
# define GSB_SYMMETRY_NAME_MASK          0x00040
# define GSB_ORIENT_NAME_MASK            0x00080
# define GSB_RDS_LAYER_NAME_MASK         0x00100
# define GSB_PEEK_BOUND_MASK             0x00200
# define GSB_LOWER_GRID_STEP_MASK        0x00400
# define GSB_LOWER_FIGURE_STEP_MASK      0x00800
# define GSB_LOWER_INSTANCE_STEP_MASK    0x01000
# define GSB_LOWER_REFERENCE_STEP_MASK   0x02000
# define GSB_LOWER_SEGMENT_STEP_MASK     0x04000
# define GSB_LOWER_CONNECTOR_STEP_MASK   0x08000
# define GSB_CURSOR_COLOR_NAME_MASK      0x10000
# define GSB_CURSOR_SIZE_MASK            0x20000
# define GSB_BIGVIA_NAME_MASK            0x40000
# define GSB_BIGVIA_VALUE_MASK           0x80000

# define GSB_ALL_DEFINED_MASK            0xFFFFF

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

# endif
