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
| Tool    :                   XFSM                           |
|                                                             |
| File    :                   Event.h                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef XFSM_EVENT
# define XFSM_EVENT

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_B1DN         0
# define XFSM_B3DN         1
# define XFSM_MOTION       2
# define XFSM_ENTER        3
# define XFSM_LEAVE        4

# define XFSM_KEY_UP       5
# define XFSM_KEY_DN       6
# define XFSM_KEY_LEFT     7
# define XFSM_KEY_RIGHT    8

# define XFSM_KEY_MORE     9
# define XFSM_KEY_LESS    10

# define XFSM_KEY_FIT     11
# define XFSM_KEY_REFRESH 12
# define XFSM_KEY_UNZOOM  13

# define XFSM_KEY_ZOOM_IN 14
# define XFSM_KEY_CENTER  15
# define XFSM_KEY_PAN     16

# define XFSM_MAX_EVENT_BUFFER 128

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
  
  extern void XfsmInitializeEvent();
  extern void XfsmDisplayEditMeasure();

# endif 
