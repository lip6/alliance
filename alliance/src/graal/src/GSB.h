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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  GSB101.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_GSB 
# define GRAAL_GSB 

# define GRAAL_TRUE   1
# define GRAAL_FALSE  0

# define GRAAL_ERROR  1
# define GRAAL_OK     0

# define GRAAL_NORTH        0
# define GRAAL_SOUTH        1
# define GRAAL_EAST         2
# define GRAAL_WEST         3

# define GRAAL_NORTH_MASK   1
# define GRAAL_SOUTH_MASK   2
# define GRAAL_WEST_MASK    4
# define GRAAL_EAST_MASK    8

# define GRAAL_DEFAULT_FIGURE_NAME      "NONAME"

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_EDIT_MEASURE             (char)0x00
# define GRAAL_CREATE_SEGMENT           (char)0x01
# define GRAAL_CREATE_SEGMENT_WIRE      (char)0x02
# define GRAAL_CREATE_VIA               (char)0x03
# define GRAAL_CREATE_TRANSISTOR        (char)0x04
# define GRAAL_CREATE_TRANSISTOR_WIRE   (char)0x05
# define GRAAL_CREATE_CONNECTOR         (char)0x06
# define GRAAL_CREATE_INSTANCE          (char)0x07
# define GRAAL_CREATE_REFERENCE         (char)0x08
# define GRAAL_CREATE_ABUTMENTBOX       (char)0x09
# define GRAAL_SELECT_POINT_IDENTIFY    (char)0x0A
# define GRAAL_SELECT_POINT_COPY        (char)0x0B
# define GRAAL_SELECT_POINT_MOVE        (char)0x0C
# define GRAAL_SELECT_POINT_DELETE      (char)0x0D
# define GRAAL_SELECT_POINT_STRETCH     (char)0x0E
# define GRAAL_SELECT_POINT_MODIFY      (char)0x0F
# define GRAAL_SELECT_WINDOW_COPY       (char)0x10
# define GRAAL_SELECT_WINDOW_MOVE       (char)0x11
# define GRAAL_SELECT_WINDOW_DELETE     (char)0x12
# define GRAAL_SELECT_WINDOW_STRETCH    (char)0x13
# define GRAAL_SELECT_WINDOW_MODIFY     (char)0x14
# define GRAAL_SELECT_WINDOW_IDENTIFY   (char)0x15
# define GRAAL_EDIT_COPY_POINT          (char)0x16
# define GRAAL_EDIT_COPY_WINDOW         (char)0x17
# define GRAAL_EDIT_MOVE_POINT          (char)0x18
# define GRAAL_EDIT_MOVE_WINDOW         (char)0x19
# define GRAAL_EDIT_STRETCH_POINT       (char)0x1A
# define GRAAL_EDIT_STRETCH_WINDOW      (char)0x1B
# define GRAAL_TOOLS_EQUI               (char)0x1C
# define GRAAL_TOOLS_FLATTEN            (char)0x1D
# define GRAAL_TOOLS_UNFLATTEN          (char)0x1E
# define GRAAL_TOOLS_PEEK               (char)0x1F
# define GRAAL_TOOLS_DRUC               (char)0x20
# define GRAAL_TOOLS_REAL_FLATTEN       (char)0x21
# define GRAAL_TOOLS_HIERARCHY_DOWN     (char)0x22
# define GRAAL_TOOLS_HIERARCHY_SHOW     (char)0x23
# define GRAAL_CREATE_BIGVIA            (char)0x24


# define GRAAL_ZOOM_CENTER            (char)0x80
# define GRAAL_ZOOM_IN                (char)0x81
# define GRAAL_ZOOM_PAN               (char)0x82

# define GRAAL_ZOOM_MARK              (char)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_INPUT_POINT     0
# define GRAAL_INPUT_ORTHO     1
# define GRAAL_INPUT_LINE      2
# define GRAAL_INPUT_LSTRING   3
# define GRAAL_INPUT_BOX       4
# define GRAAL_INPUT_SORTHO    5
# define GRAAL_INPUT_HALF_BOX  6

/*------------------------------------------------------------\
|                                                             |
|                           Active Name                       |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_FIGURE_NAME                 0
# define GRAAL_INSTANCE_NAME               1
# define GRAAL_FIGURE_CONNECTOR_NAME       2
# define GRAAL_INSTANCE_CONNECTOR_NAME     3
# define GRAAL_FIGURE_SEGMENT_NAME         4
# define GRAAL_INSTANCE_SEGMENT_NAME       5
# define GRAAL_FIGURE_REFERENCE_NAME       6
# define GRAAL_INSTANCE_REFERENCE_NAME     7

# define GRAAL_MAX_ACTIVE_NAME             8

/*------------------------------------------------------------\
|                                                             |
|                           Fill Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_FILL_MODE_OUTLINE   0
# define GRAAL_FILL_MODE_FILL      1
# define GRAAL_FILL_MODE_PATTERN   2

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

  extern char  *GRAAL_SEGMENT_NAME_TABLE[ MBK_MAX_LAYER ][ 3 ];
  extern char  *GRAAL_TRANSISTOR_NAME_TABLE[ MBK_MAX_LAYER ][ 3 ];
  extern char  *GRAAL_CONNECTOR_NAME_TABLE[ MBK_MAX_LAYER ][ 3 ];
  extern float  GRAAL_SEGMENT_VALUE_TABLE[ MBK_MAX_LAYER ][ 2 ];
  extern char  *GRAAL_VIA_NAME_TABLE[ MBK_MAX_VIA ][ 3 ];
  extern char  *GRAAL_BIGVIA_NAME_TABLE[ MBK_MAX_VIA ][ 3 ];
  extern float  GRAAL_BIGVIA_VALUE_TABLE[ MBK_MAX_LAYER ][ 1 ];
  extern char  *GRAAL_REFERENCE_NAME_TABLE[ MBK_MAX_REFERENCE ][ 3 ];
  extern char  *GRAAL_ORIENT_NAME_TABLE[ MBK_MAX_ORIENT ][ 3 ];
  extern char  *GRAAL_SYMMETRY_NAME_TABLE[ MBK_MAX_SYMMETRY ][ 3 ];
  extern char  *GRAAL_RDS_LAYER_NAME_TABLE[ RDS_ALL_LAYER ][ 3 ];
  extern int    GRAAL_RDS_LAYER_PATTERN_TABLE[ RDS_ALL_LAYER ];
  extern long   GRAAL_PEEK_BOUND;
  extern char  *GRAAL_CURSOR_COLOR_NAME;
  extern long   GRAAL_CURSOR_SIZE;
  extern float  GRAAL_LOWER_GRID_STEP;
  extern float  GRAAL_LOWER_FIGURE_STEP;
  extern float  GRAAL_LOWER_INSTANCE_STEP;
  extern float  GRAAL_LOWER_REFERENCE_STEP;
  extern float  GRAAL_LOWER_SEGMENT_STEP;
  extern float  GRAAL_LOWER_CONNECTOR_STEP;
  extern long   GRAAL_RDS_LAMBDA;
  extern float  GRAAL_SCALE_X;
  extern long   GRAAL_SCALE;
  extern char   GRAAL_XOR_CURSOR;

  extern char   GRAAL_INSTANCE_INTERFACE;
  extern char   GRAAL_FILL_MODE;
  extern char   GRAAL_CONNECTOR_INDEX;
  extern char   GRAAL_FORCE_DISPLAY;
  extern char   GRAAL_BLACK_BOX_STRING;
  extern char   GRAAL_SWITCH_COLOR_MAP;

  extern char   GRAAL_RDS_ACTIVE_LAYER_TABLE[ RDS_ALL_LAYER ];
  extern char   GRAAL_RDS_ACTIVE_NAME_TABLE[ GRAAL_MAX_ACTIVE_NAME ];
  extern long  *GRAAL_CUT_C_X_ARRAY [ MBK_MAX_LAYER ];
  extern char   GRAAL_CUT_C_X_LIST[ MBK_MAX_VIA ];

  extern char  *GRAAL_TECHNO_NAME;
  extern char  *GRAAL_GMS_FILE_NAME;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *GraalPostTreatString();
  extern void GraalLoadParameters();
  extern void GraalViewParameters();
  extern void GraalInitializeCutCxTable();
  extern void Graalenv();

# endif
