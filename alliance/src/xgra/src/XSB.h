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
| Tool    :                    XGRA                           |
|                                                             |
| File    :                    XSB.h                          |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XGRA_XSB 
# define XGRA_XSB 

/*------------------------------------------------------\
|                                                       |
|                        Others                         |
|                                                       |
\------------------------------------------------------*/

# define XGRA_TRUE   1
# define XGRA_FALSE  0

# define XGRA_ERROR  1
# define XGRA_OK     0

# define XGRA_NORTH        0
# define XGRA_SOUTH        1
# define XGRA_EAST         2
# define XGRA_WEST         3

# define XGRA_NORTH_MASK   1
# define XGRA_SOUTH_MASK   2
# define XGRA_WEST_MASK    4
# define XGRA_EAST_MASK    8

# define XGRA_DEFAULT_FIGURE_NAME  "NONAME"

/*------------------------------------------------------\
|                                                       |
|                        Layer                          |
|                                                       |
\------------------------------------------------------*/

# define XGRA_MAX_LAYER  10

/*------------------------------------------------------\
|                                                       |
|                        Active Name                    |
|                                                       |
\------------------------------------------------------*/

# define XGRA_ACTIVE_NAME_NODE 0
# define XGRA_ACTIVE_NAME_ARC 1
# define XGRA_MAX_ACTIVE_NAME   2

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_EDIT_MEASURE           (char)0x00
# define XGRA_EDIT_IDENTIFY          (char)0x01
# define XGRA_EDIT_CONNECTED         (char)0x02

# define XGRA_ZOOM_CENTER            (char)0x80
# define XGRA_ZOOM_IN                (char)0x81
# define XGRA_ZOOM_PAN               (char)0x82

# define XGRA_ZOOM_MARK              (char)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_INPUT_POINT     0
# define XGRA_INPUT_ORTHO     1
# define XGRA_INPUT_LINE      2
# define XGRA_INPUT_LSTRING   3
# define XGRA_INPUT_BOX       4
# define XGRA_INPUT_SORTHO    5
# define XGRA_INPUT_HALF_BOX  6

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

  extern char   XGRA_ACTIVE_LAYER_TABLE[ XGRA_MAX_LAYER ];
  extern char   XGRA_ACTIVE_NAME_TABLE[ XGRA_MAX_ACTIVE_NAME ];

  extern char  *XGRA_CURSOR_COLOR_NAME;
  extern char  *XGRA_BACKGROUND_COLOR_NAME;
  extern char  *XGRA_FOREGROUND_COLOR_NAME;
  extern char  *XGRA_ACCEPT_COLOR_NAME;
  extern char  *XGRA_CONNECT_COLOR_NAME;

  extern long   XGRA_CURSOR_SIZE;
  extern float  XGRA_LOWER_GRID_STEP;
  extern long   XGRA_UNIT;
  extern char  *XGRA_LAYER_NAME_TABLE[ XGRA_MAX_LAYER ][ 3 ];
  extern char  *XGRA_PARAM_NAME;

  extern char  *XGRA_XMS_FILE_NAME;
  extern char  *XgraFirstFileName;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *XgraPostTreatString();
  extern void  XgraLoadParameters();
  extern void  Xgragetenv();

# endif
