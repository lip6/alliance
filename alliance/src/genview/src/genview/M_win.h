/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                   M_win.h                         |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   30.04.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                             Constants                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef M_WIN_H
# define M_WIN_H

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Variables                       |
|                                                             |
\------------------------------------------------------------*/

  extern int firstMove;
  extern int E_NewFile;
  extern int M_x;
  extern int M_y;
  extern int M_dx;
  extern int M_dy;

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/


  extern void M_open();
  extern int  M_event();
  extern int  M_isOnDisk();
  extern void M_getFig();

# endif
