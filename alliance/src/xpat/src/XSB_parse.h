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
| Tool    :                     XSB                           |
|                                                             |
| File    :                  Xsbparse.h                       |
|                                                             |
| Authors :                Jacomme Ludovic                    |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XSB_PARSE
# define XSB_PARSE

# define XSB_MAX_BUFFER        255
# define XSB_MAX_KEYWORD       1

# define XSB_SEPARATORS_STRING " \t\n"
# define XSB_COMMENT_CHAR      '#'

/*------------------------------------------------------------\
|                                                             |
|                        Keyword Constants                    |
|                                                             |
\------------------------------------------------------------*/

# define DEFINE_KEYWORD                "DEFINE"
# define TABLE_KEYWORD                 "TABLE" 
# define END_TABLE_KEYWORD             "END"
# define END_RECORD_KEYWORD            NULL

# define LOWER_GRID_STEP_KEYWORD       "XPAT_LOWER_GRID_STEP"
# define CURSOR_COLOR_NAME_KEYWORD     "XPAT_CURSOR_COLOR_NAME"
# define BACKGROUND_COLOR_NAME_KEYWORD "XPAT_BACKGROUND_COLOR_NAME"
# define FOREGROUND_COLOR_NAME_KEYWORD "XPAT_FOREGROUND_COLOR_NAME"
# define ACCEPT_COLOR_NAME_KEYWORD     "XPAT_ACCEPT_COLOR_NAME"
# define CONNECT_COLOR_NAME_KEYWORD    "XPAT_CONNECT_COLOR_NAME"
# define CURSOR_SIZE_KEYWORD           "XPAT_CURSOR_SIZE"
# define UNIT_KEYWORD                  "XPAT_UNIT"
# define LAYER_NAME_KEYWORD            "XPAT_LAYER_NAME"

# define XSB_LOWER_GRID_STEP_MASK        0x00001
# define XSB_CURSOR_COLOR_NAME_MASK      0x00002
# define XSB_BACKGROUND_COLOR_NAME_MASK  0x00004
# define XSB_FOREGROUND_COLOR_NAME_MASK  0x00008
# define XSB_ACCEPT_COLOR_NAME_MASK      0x00010
# define XSB_CONNECT_COLOR_NAME_MASK     0x00020
# define XSB_CURSOR_SIZE_MASK            0x00040
# define XSB_UNIT_MASK                   0x00080
# define XSB_LAYER_NAME_MASK             0x00100

# define XSB_ALL_DEFINED_MASK            0x001FF

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
