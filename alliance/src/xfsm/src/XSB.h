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
| Tool    :                    XFSM                           |
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

# ifndef XFSM_XSB 
# define XFSM_XSB 

/*------------------------------------------------------\
|                                                       |
|                        Others                         |
|                                                       |
\------------------------------------------------------*/

# define XFSM_TRUE   1
# define XFSM_FALSE  0

# define XFSM_ERROR  1
# define XFSM_OK     0

# define XFSM_NORTH        0
# define XFSM_SOUTH        1
# define XFSM_EAST         2
# define XFSM_WEST         3

# define XFSM_NORTH_MASK   1
# define XFSM_SOUTH_MASK   2
# define XFSM_WEST_MASK    4
# define XFSM_EAST_MASK    8

# define XFSM_DEFAULT_FIGURE_NAME  "NONAME"

/*------------------------------------------------------\
|                                                       |
|                        Layer                          |
|                                                       |
\------------------------------------------------------*/

# define XFSM_STATE_LAYER   0
# define XFSM_STAR_LAYER    1
# define XFSM_FIRST_LAYER   2
# define XFSM_STACK_LAYER   3

# define XFSM_MAX_LAYER    10

/*------------------------------------------------------\
|                                                       |
|                        Active Name                    |
|                                                       |
\------------------------------------------------------*/

# define XFSM_ACTIVE_NAME_STATE 0
# define XFSM_ACTIVE_NAME_TRANS 1
# define XFSM_MAX_ACTIVE_NAME   2

/*------------------------------------------------------------\
|                                                             |
|                           Edit Mode                         |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_EDIT_MEASURE           (char)0x00
# define XFSM_EDIT_IDENTIFY          (char)0x01
# define XFSM_EDIT_CONNECTED         (char)0x02

# define XFSM_ZOOM_CENTER            (char)0x80
# define XFSM_ZOOM_IN                (char)0x81
# define XFSM_ZOOM_PAN               (char)0x82

# define XFSM_ZOOM_MARK              (char)0x80

/*------------------------------------------------------------\
|                                                             |
|                           Input Mode                        |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_INPUT_POINT     0
# define XFSM_INPUT_ORTHO     1
# define XFSM_INPUT_LINE      2
# define XFSM_INPUT_LSTRING   3
# define XFSM_INPUT_BOX       4
# define XFSM_INPUT_SORTHO    5
# define XFSM_INPUT_HALF_BOX  6

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

  extern char   XFSM_ACTIVE_LAYER_TABLE[ XFSM_MAX_LAYER ];
  extern char   XFSM_ACTIVE_NAME_TABLE[ XFSM_MAX_ACTIVE_NAME ];

  extern char  *XFSM_CURSOR_COLOR_NAME;
  extern char  *XFSM_BACKGROUND_COLOR_NAME;
  extern char  *XFSM_FOREGROUND_COLOR_NAME;
  extern char  *XFSM_ACCEPT_COLOR_NAME;
  extern char  *XFSM_CONNECT_COLOR_NAME;

  extern long   XFSM_CURSOR_SIZE;
  extern float  XFSM_LOWER_GRID_STEP;
  extern long   XFSM_UNIT;
  extern char  *XFSM_LAYER_NAME_TABLE[ XFSM_MAX_LAYER ][ 3 ];
  extern char  *XFSM_PARAM_NAME;

  extern char  *XFSM_XMS_FILE_NAME;
  extern char  *XfsmFirstFileName;

  extern char   XFSM_FORCE_DISPLAY;
  extern char   XFSM_XOR_CURSOR;
  extern char   XFSM_SWITCH_COLOR_MAP;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern char *XfsmPostTreatString();
  extern void  XfsmLoadParameters();
  extern void  Xfsmgetenv();

# endif
