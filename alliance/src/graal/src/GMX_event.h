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
| File    :                   Event.h                         |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_EVENT
# define GRAAL_EVENT

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_B1DN          0
# define GRAAL_B3DN          1
# define GRAAL_MOTION        2
# define GRAAL_ENTER         3
# define GRAAL_LEAVE         4
                            
# define GRAAL_KEY_UP        5
# define GRAAL_KEY_DN        6
# define GRAAL_KEY_LEFT      7
# define GRAAL_KEY_RIGHT     8
                            
# define GRAAL_KEY_MORE      9
# define GRAAL_KEY_LESS     10
                            
# define GRAAL_KEY_FIT      11
# define GRAAL_KEY_REFRESH  12
# define GRAAL_KEY_UNZOOM   13
                            
# define GRAAL_KEY_ZOOM_IN  14
# define GRAAL_KEY_CENTER   15
# define GRAAL_KEY_PAN      16

# define GRAAL_KEY_DN_LAYER 17
# define GRAAL_KEY_UP_LAYER 18

# define GRAAL_MAX_EVENT_BUFFER 128

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
  
  extern void GraalInitializeEvent();
  extern void GraalDisplayEditMeasure();

# endif 
