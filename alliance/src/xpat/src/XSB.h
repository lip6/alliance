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
| Tool    :                    XPAT                           |
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

# ifndef XPAT_XSB 
# define XPAT_XSB 

/*------------------------------------------------------\
|                                                       |
|                        Others                         |
|                                                       |
\------------------------------------------------------*/

# define XPAT_TRUE   1
# define XPAT_FALSE  0

# define XPAT_ERROR  1
# define XPAT_OK     0

# define XPAT_NORTH        0
# define XPAT_SOUTH        1
# define XPAT_EAST         2
# define XPAT_WEST         3

# define XPAT_NORTH_MASK   1
# define XPAT_SOUTH_MASK   2
# define XPAT_WEST_MASK    4
# define XPAT_EAST_MASK    8

# define XPAT_DEFAULT_FIGURE_NAME  "NONAME"

/*------------------------------------------------------\
|                                                       |
|                        Layer                          |
|                                                       |
\------------------------------------------------------*/

# define XPAT_MAX_LAYER    10

/*------------------------------------------------------\
|                                                       |
|                        Active Name                    |
|                                                       |
\------------------------------------------------------*/

# define XPAT_ACTIVE_NAME_VECTOR 0
# define XPAT_ACTIVE_NAME_IO     1
# define XPAT_MAX_ACTIVE_NAME    2

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_EDIT_MEASURE           (char)0x00
# define XPAT_EDIT_IDENTIFY          (char)0x01
# define XPAT_EDIT_CONNECTED         (char)0x02
# define XPAT_EDIT_ADD_CURSOR        (char)0x03
# define XPAT_EDIT_DEL_CURSOR        (char)0x04

# define XPAT_ZOOM_CENTER            (char)0x80
# define XPAT_ZOOM_IN                (char)0x81
# define XPAT_ZOOM_PAN               (char)0x82

# define XPAT_ZOOM_MARK              (char)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_INPUT_POINT     0
# define XPAT_INPUT_ORTHO     1
# define XPAT_INPUT_LINE      2
# define XPAT_INPUT_LSTRING   3
# define XPAT_INPUT_BOX       4
# define XPAT_INPUT_SORTHO    5
# define XPAT_INPUT_HALF_BOX  6

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

  extern char   XPAT_ACTIVE_LAYER_TABLE[ XPAT_MAX_LAYER ];
  extern char   XPAT_ACTIVE_NAME_TABLE [ XPAT_MAX_ACTIVE_NAME ];

  extern char  *XPAT_CURSOR_COLOR_NAME;
  extern char  *XPAT_BACKGROUND_COLOR_NAME;
  extern char  *XPAT_FOREGROUND_COLOR_NAME;
  extern char  *XPAT_ACCEPT_COLOR_NAME;
  extern char  *XPAT_CONNECT_COLOR_NAME;

  extern long   XPAT_CURSOR_SIZE;
  extern float  XPAT_LOWER_GRID_STEP;
  extern long   XPAT_UNIT;
  extern char  *XPAT_LAYER_NAME_TABLE[ XPAT_MAX_LAYER ][ 3 ];
  extern char  *XPAT_PARAM_NAME;

  extern char  *XPAT_XMS_FILE_NAME;
  extern char  *XpatFirstFileName;

  extern char   XPAT_FORCE_DISPLAY;
  extern char   XPAT_XOR_CURSOR;
  extern char   XPAT_SWITCH_COLOR_MAP;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *XpatPostTreatString();
  extern void  XpatLoadParameters();
  extern void  Xpatgetenv();

# endif
