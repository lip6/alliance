/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
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
| Tool    :                    XSCH                           |
|                                                             |
| File    :                    XSB.h                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XSCH_XSB 
# define XSCH_XSB 

/*------------------------------------------------------\
|                                                       |
|                        Others                         |
|                                                       |
\------------------------------------------------------*/

# define XSCH_TRUE   1
# define XSCH_FALSE  0

# define XSCH_ERROR  1
# define XSCH_OK     0

# define XSCH_NORTH        0
# define XSCH_SOUTH        1
# define XSCH_EAST         2
# define XSCH_WEST         3

# define XSCH_NORTH_MASK   1
# define XSCH_SOUTH_MASK   2
# define XSCH_WEST_MASK    4
# define XSCH_EAST_MASK    8

# define XSCH_DEFAULT_FIGURE_NAME  "NONAME"

/*------------------------------------------------------\
|                                                       |
|                        Layer                          |
|                                                       |
\------------------------------------------------------*/

# define XSCH_MAX_LAYER         38

# define XSCH_SCHBOX_LAYER        0
# define XSCH_SCHCONIN_LAYER      1
# define XSCH_SCHCONOUT_LAYER     2
# define XSCH_SCHNET_LAYER        3
# define XSCH_CONSTRUCTION_LAYER  4
# define XSCH_FIRST_USER_LAYER    5

/*------------------------------------------------------\
|                                                       |
|                        Active Name                    |
|                                                       |
\------------------------------------------------------*/

# define XSCH_CONNECTOR_NAME        0
# define XSCH_INSTANCE_NAME         1
# define XSCH_INS_CON_NAME          2
# define XSCH_NET_NAME              3
# define XSCH_MAX_ACTIVE_NAME       4

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_EDIT_MEASURE           (char)0x00
# define XSCH_EDIT_IDENTIFY          (char)0x01
# define XSCH_EDIT_CONNECTED         (char)0x02
# define XSCH_TOOLS_HIERARCHY_DOWN   (char)0x03

# define XSCH_ZOOM_CENTER            (char)0x80
# define XSCH_ZOOM_IN                (char)0x81
# define XSCH_ZOOM_PAN               (char)0x82

# define XSCH_ZOOM_MARK              (char)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_INPUT_POINT     0
# define XSCH_INPUT_ORTHO     1
# define XSCH_INPUT_LINE      2
# define XSCH_INPUT_LSTRING   3
# define XSCH_INPUT_BOX       4
# define XSCH_INPUT_SORTHO    5
# define XSCH_INPUT_HALF_BOX  6

/*------------------------------------------------------------\
|                                                             |
|                            Icon Cell                        |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_ICON_CELL_BUFFER    0
# define XSCH_ICON_CELL_AND       1
# define XSCH_ICON_CELL_OR        2
# define XSCH_ICON_CELL_XOR       3
# define XSCH_ICON_CELL_TRISTATE  4
# define XSCH_ICON_CELL_REGISTER  5
# define XSCH_ICON_CELL_CONSTANT  6
# define XSCH_ICON_CELL_ANDOR     7
# define XSCH_ICON_CELL_ORAND     8
# define XSCH_ICON_CELL_MUX       9

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

  typedef struct xschicon
  {
    char          *NAME;
    chain_list    *CON_IN;
    chain_list    *CON_OUT;
    unsigned char  TYPE;
    short          ARGS[ 6 ];

  } xschicon;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  extern char   XSCH_ACTIVE_LAYER_TABLE[ XSCH_MAX_LAYER ];
  extern char   XSCH_ACTIVE_NAME_TABLE[ XSCH_MAX_ACTIVE_NAME ];

  extern char  *XSCH_CURSOR_COLOR_NAME;
  extern char  *XSCH_BACKGROUND_COLOR_NAME;
  extern char  *XSCH_FOREGROUND_COLOR_NAME;
  extern char  *XSCH_ACCEPT_COLOR_NAME;
  extern char  *XSCH_CONNECT_COLOR_NAME;

  extern long   XSCH_CURSOR_SIZE;
  extern float  XSCH_LOWER_GRID_STEP;
  extern long   XSCH_UNIT;
  extern char  *XSCH_LAYER_NAME_TABLE[ XSCH_MAX_LAYER ][ 3 ];
  extern long   XSCH_LAYER_XFIG_COLOR[ XSCH_MAX_LAYER ];
  extern char  *XSCH_PARAM_NAME;

  extern char  *XSCH_XMS_FILE_NAME;
  extern char  *XschFirstFileName;
  extern char  *XSCH_IN_SCHEM;

  extern char   XSCH_VECTORIZED_MODE;
  extern char   XSCH_FORCE_DISPLAY;
  extern char   XSCH_XOR_CURSOR;
  extern char   XSCH_SWITCH_COLOR_MAP;

  extern authtable *XSCH_HASH_ICON;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *XschPostTreatString();
  extern void  XschLoadParameters();
  extern void  Xschgetenv();

# endif
