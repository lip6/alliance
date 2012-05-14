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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  GSB101.h                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef DREAL_GSB 
# define DREAL_GSB 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_TRUE   1
# define DREAL_FALSE  0

# define DREAL_ERROR  1
# define DREAL_OK     0

# define DREAL_NORTH        0
# define DREAL_SOUTH        1
# define DREAL_EAST         2
# define DREAL_WEST         3

# define DREAL_NORTH_MASK   1
# define DREAL_SOUTH_MASK   2
# define DREAL_WEST_MASK    4
# define DREAL_EAST_MASK    8

# define DREAL_DEFAULT_FIGURE_NAME      "NONAME"

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_EDIT_MEASURE           (char)0x00

# define DREAL_CREATE_RECTANGLE       (char)0x01
# define DREAL_CREATE_ABUTMENTBOX     (char)0x02
# define DREAL_SELECT_POINT_IDENTIFY  (char)0x03
# define DREAL_SELECT_POINT_COPY      (char)0x04
# define DREAL_SELECT_POINT_MOVE      (char)0x05
# define DREAL_SELECT_POINT_DELETE    (char)0x06
# define DREAL_SELECT_POINT_STRETCH   (char)0x07
# define DREAL_SELECT_POINT_MODIFY    (char)0x08
# define DREAL_SELECT_WINDOW_COPY     (char)0x09
# define DREAL_SELECT_WINDOW_MOVE     (char)0x0A
# define DREAL_SELECT_WINDOW_DELETE   (char)0x0B
# define DREAL_SELECT_WINDOW_MODIFY   (char)0x0C
# define DREAL_SELECT_WINDOW_IDENTIFY (char)0x0D
# define DREAL_EDIT_COPY_POINT        (char)0x0E
# define DREAL_EDIT_COPY_WINDOW       (char)0x0F
# define DREAL_EDIT_MOVE_POINT        (char)0x10
# define DREAL_EDIT_MOVE_WINDOW       (char)0x11
# define DREAL_EDIT_STRETCH_POINT     (char)0x12

# define DREAL_ZOOM_CENTER            (char)0x80
# define DREAL_ZOOM_IN                (char)0x81
# define DREAL_ZOOM_PAN               (char)0x82

# define DREAL_ZOOM_MARK              (char)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_INPUT_POINT     0
# define DREAL_INPUT_ORTHO     1
# define DREAL_INPUT_LINE      2
# define DREAL_INPUT_LSTRING   3
# define DREAL_INPUT_BOX       4
# define DREAL_INPUT_SORTHO    5
# define DREAL_INPUT_HALF_BOX  6

/*------------------------------------------------------------\
|                                                             |
|                           Active Name                       |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_FIGURE_NAME                 0
# define DREAL_INSTANCE_NAME               1
# define DREAL_FIGURE_CONNECTOR_NAME       2
# define DREAL_INSTANCE_CONNECTOR_NAME     3
# define DREAL_FIGURE_SEGMENT_NAME         4
# define DREAL_INSTANCE_SEGMENT_NAME       5
# define DREAL_FIGURE_REFERENCE_NAME       6
# define DREAL_INSTANCE_REFERENCE_NAME     7

# define DREAL_MAX_ACTIVE_NAME             8

/*------------------------------------------------------------\
|                                                             |
|                           Fill Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_FILL_MODE_OUTLINE   0
# define DREAL_FILL_MODE_FILL      1
# define DREAL_FILL_MODE_PATTERN   2

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

  extern char  *DREAL_RDS_LAYER_NAME_TABLE[ RDS_ALL_LAYER ][ 3 ];
  extern int    DREAL_RDS_LAYER_PATTERN_TABLE[ RDS_ALL_LAYER ];
  extern char   DREAL_RDS_ACTIVE_LAYER_TABLE[ RDS_ALL_LAYER ];
  extern char   DREAL_RDS_ACTIVE_NAME_TABLE[ DREAL_MAX_ACTIVE_NAME ];
  extern float  DREAL_LOWER_GRID_STEP;
  extern float  DREAL_LOWER_FIGURE_STEP;
  extern float  DREAL_LOWER_INSTANCE_STEP;
  extern float  DREAL_LOWER_REFERENCE_STEP;
  extern float  DREAL_LOWER_SEGMENT_STEP;
  extern float  DREAL_LOWER_CONNECTOR_STEP;
  extern char  *DREAL_CURSOR_COLOR_NAME;
  extern long   DREAL_CURSOR_SIZE;
  extern float  DREAL_RDS_UNIT_TO_MICRON;
  extern float  DREAL_PHYSICAL_GRID_TO_MICRON;

  extern char   DREAL_INSTANCE_INTERFACE;
  extern char   DREAL_FILL_MODE;
  extern char   DREAL_XOR_CURSOR;
  extern char   DREAL_FORCE_DISPLAY;
  extern char   DREAL_BLACK_BOX_STRING;
  extern char   DREAL_SWITCH_COLOR_MAP;

  extern char  *DREAL_TECHNO_NAME;
  extern char  *DREAL_GMS_FILE_NAME;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *DrealPostTreatString();
  extern void DrealLoadParameters();
  extern void DrealViewParameters();
  extern void Drealenv();


# endif
