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
| File    :                   C_win.h                         |
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

# ifndef C_WIN_H  
# define C_WIN_H  

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


/* Noms des objets destines a apparaitre dans la fenetre de config         */
/* La longueur de la chaine determine la taille du pave associe a  l'objet */
extern char *C_ObjectName[C_MAX_NB_OBJECT];

/* Flags donnant la configuration des Layers et Textes visibles ou non */
/* Ce tableau est charge par la structure C_Config lors de l'initialisation
   de la fenetre de config */
extern int C_ObjectVisibility[C_MAX_NB_OBJECT];

/* Valeur du pourcentage de scroll en string charge par la structure C_Config
   lors de l'initialisation de la fenetre de config */
extern char C_ScrollValue[3];

/* Contient la valeur provisoire avant la validation par return */
extern char C_NewScrollValue[5];

extern struct C_Config C_Config_List;
/* Buffer contenant le texte associe a l'icone scroll */
extern char *C_ScrollText[20];

/* Flag indiquant l'icone active */
extern int C_IconFlag;

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/


  extern void C_open();
  extern int  C_event();
  extern int C_actionMouse();

# endif
