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
# define GSB_MAX_KEYWORD       74

# define GSB_SEPARATORS_STRING " \t\n"
# define GSB_COMMENT_CHAR      '#'

/*------------------------------------------------------------\
|                                                             |
|                        Keyword Constants                    |
|                                                             |
\------------------------------------------------------------*/

# define DEFINE_KEYWORD          "DEFINE"
# define TABLE_KEYWORD           "TABLE" 
# define END_TABLE_KEYWORD       "END"
# define END_RECORD_KEYWORD       NULL

# define RDS_LAYER_NAME_KEYWORD       "DREAL_RDS_LAYER_NAME"
# define LOWER_GRID_STEP_KEYWORD      "DREAL_LOWER_GRID_STEP"
# define LOWER_FIGURE_STEP_KEYWORD    "DREAL_LOWER_FIGURE_STEP"
# define LOWER_INSTANCE_STEP_KEYWORD  "DREAL_LOWER_INSTANCE_STEP"
# define LOWER_CONNECTOR_STEP_KEYWORD "DREAL_LOWER_CONNECTOR_STEP"
# define LOWER_SEGMENT_STEP_KEYWORD   "DREAL_LOWER_SEGMENT_STEP"
# define LOWER_REFERENCE_STEP_KEYWORD "DREAL_LOWER_REFERENCE_STEP"
# define CURSOR_COLOR_NAME_KEYWORD    "DREAL_CURSOR_COLOR_NAME"
# define CURSOR_SIZE_KEYWORD          "DREAL_CURSOR_SIZE"

# define GSB_RDS_LAYER_NAME_MASK         0x0001
# define GSB_LOWER_GRID_STEP_MASK        0x0002
# define GSB_LOWER_FIGURE_STEP_MASK      0x0004
# define GSB_LOWER_INSTANCE_STEP_MASK    0x0008
# define GSB_LOWER_REFERENCE_STEP_MASK   0x0010
# define GSB_LOWER_SEGMENT_STEP_MASK     0x0020
# define GSB_LOWER_CONNECTOR_STEP_MASK   0x0040
# define GSB_CURSOR_COLOR_NAME_MASK      0x0080
# define GSB_CURSOR_SIZE_MASK            0x0100

# define GSB_ALL_DEFINED_MASK            0x01FF

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
