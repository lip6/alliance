/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
| File    :                  C_Utils.h                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                             Constants                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef C_UTILS_H 
# define C_UTILS_H 

# define OBJ_INSTANCE       ( RDS_MAX_LAYER + 0 )
# define OBJ_INSTANCE_TEXT  ( RDS_MAX_LAYER + 1 )
# define OBJ_CONNECTOR_TEXT ( RDS_MAX_LAYER + 2 )
# define OBJ_SEGMENT_TEXT   ( RDS_MAX_LAYER + 3 )
# define OBJ_REFERENCE_TEXT ( RDS_MAX_LAYER + 4 )

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

  extern int C_atomicValue;

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/


  extern void C_InitializeObjectName();
  extern void C_setMenu();
  extern void C_drawIcon();
  extern void C_setVP();
  extern void C_setValues();
  extern void C_drawMenu();
  extern int  C_clickedIcon();
  extern void SwitchRefresh ();
  extern void C_AllVisible ();
  extern void C_AllInvisible ();
  extern void C_ActionKey ();
  extern void C_WriteDisk ();
  extern int  C_ReadDisk ();
  extern void C_WriteConf();
  extern void C_ReadConf();
  extern void C_getDiskInfo ();
  extern void C_Peek();
  extern void C_View();
  extern void C_askScroll();
  extern void C_newScroll();
  extern void C_InitValues();
  extern void C_atomic();

# endif
