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
| Tool    :                    XVPN                           |
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

# ifndef XVPN_XSB 
# define XVPN_XSB 

/*------------------------------------------------------\
|                                                       |
|                        Alloc                          |
|                                                       |
\------------------------------------------------------*/

# define XVPN_ALLOC_BLOCK  0
# define XVPN_ALLOC_HEAP   1

/*------------------------------------------------------\
|                                                       |
|                        Others                         |
|                                                       |
\------------------------------------------------------*/

# define XVPN_TRUE   1
# define XVPN_FALSE  0

# define XVPN_ERROR  1
# define XVPN_OK     0

# define XVPN_NORTH        0
# define XVPN_SOUTH        1
# define XVPN_EAST         2
# define XVPN_WEST         3

# define XVPN_NORTH_MASK   1
# define XVPN_SOUTH_MASK   2
# define XVPN_WEST_MASK    4
# define XVPN_EAST_MASK    8

# define XVPN_DEFAULT_FIGURE_NAME  "NONAME"

/*------------------------------------------------------\
|                                                       |
|                        Layer                          |
|                                                       |
\------------------------------------------------------*/
  
# define XVPN_LAYER_PROCESS        0
# define XVPN_LAYER_FUNCTION       1
# define XVPN_LAYER_WAIT           2
# define XVPN_LAYER_ASSIGN         3
# define XVPN_LAYER_GUARDED        4
# define XVPN_LAYER_ACT_GUARDED    5
# define XVPN_LAYER_TOKEN          6

# define XVPN_MAX_LAYER           10

/*------------------------------------------------------\
|                                                       |
|                        Active Name                    |
|                                                       |
\------------------------------------------------------*/

# define XVPN_ACTIVE_NAME_PLACE   0
# define XVPN_ACTIVE_NAME_TRANS   1
# define XVPN_MAX_ACTIVE_NAME     2

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define XVPN_EDIT_MEASURE           (short)0x00
# define XVPN_EDIT_IDENTIFY          (short)0x01
# define XVPN_EDIT_CONNECTED         (short)0x02

# define XVPN_ZOOM_CENTER            (short)0x80
# define XVPN_ZOOM_IN                (short)0x81
# define XVPN_ZOOM_PAN               (short)0x82

# define XVPN_ZOOM_MARK              (short)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define XVPN_INPUT_POINT     0
# define XVPN_INPUT_ORTHO     1
# define XVPN_INPUT_LINE      2
# define XVPN_INPUT_LSTRING   3
# define XVPN_INPUT_BOX       4
# define XVPN_INPUT_SORTHO    5
# define XVPN_INPUT_HALF_BOX  6

/*------------------------------------------------------------\
|                                                             |
|                           Fill Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define XVPN_FILL_MODE_OUTLINE   0
# define XVPN_FILL_MODE_FILL      1
# define XVPN_FILL_MODE_PATTERN   2

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

  extern short  XVPN_ACTIVE_LAYER_TABLE[ XVPN_MAX_LAYER ];
  extern short  XVPN_ACTIVE_NAME_TABLE[ XVPN_MAX_ACTIVE_NAME ];

  extern char  *XVPN_CURSOR_COLOR_NAME;
  extern char  *XVPN_BACKGROUND_COLOR_NAME;
  extern char  *XVPN_FOREGROUND_COLOR_NAME;
  extern char  *XVPN_ACCEPT_COLOR_NAME;
  extern char  *XVPN_CONNECT_COLOR_NAME;

  extern long   XVPN_CURSOR_SIZE;
  extern float  XVPN_LOWER_GRID_STEP;
  extern long   XVPN_UNIT;
  extern char  *XVPN_LAYER_NAME_TABLE[ XVPN_MAX_LAYER ][ 3 ];
  extern char  *XVPN_PARAM_NAME;
  extern char   XVPN_DEBUG_ON;

  extern char  *XVPN_VHDL_LIBLST;
  extern char  *XVPN_VHDL_BEHSFX;
  extern char  *XVPN_XMS_FILE_NAME;

  extern char  *XvpnFirstFileName;

  extern char   XVPN_FORCE_DISPLAY;
  extern char   XVPN_XOR_CURSOR;
  extern char   XVPN_SWITCH_COLOR_MAP;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *XvpnPostTreatString();
  extern void  XvpnLoadParameters();
  extern void  Xvpngetenv();

# endif
