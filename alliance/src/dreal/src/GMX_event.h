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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                   Event.h                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef DREAL_EVENT
# define DREAL_EVENT

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_B1DN          0
# define DREAL_B3DN          1
# define DREAL_MOTION        2
# define DREAL_ENTER         3
# define DREAL_LEAVE         4
                            
# define DREAL_KEY_UP        5
# define DREAL_KEY_DN        6
# define DREAL_KEY_LEFT      7
# define DREAL_KEY_RIGHT     8
                            
# define DREAL_KEY_MORE      9
# define DREAL_KEY_LESS     10
                            
# define DREAL_KEY_FIT      11
# define DREAL_KEY_REFRESH  12
# define DREAL_KEY_UNZOOM   13
                            
# define DREAL_KEY_ZOOM_IN  14
# define DREAL_KEY_CENTER   15
# define DREAL_KEY_PAN      16

# define DREAL_KEY_DN_LAYER 17
# define DREAL_KEY_UP_LAYER 18

# define DREAL_MAX_EVENT_BUFFER 128

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
  
  extern void DrealInitializeEvent();
  extern void DrealDisplayEditMeasure();

# endif 
