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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   Panel.h                         |
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

# ifndef GRAAL_TOOLS_PANEL
# define GRAAL_TOOLS_PANEL

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_TOOLS_GLOBAL_X         990
# define GRAAL_TOOLS_GLOBAL_Y         505
# define GRAAL_TOOLS_MESSAGE_X        330
# define GRAAL_TOOLS_MESSAGE_Y        280
# define GRAAL_TOOLS_DRUC_X           100
# define GRAAL_TOOLS_DRUC_Y           230
# define GRAAL_TOOLS_HIERARCHY_X      890
# define GRAAL_TOOLS_HIERARCHY_Y       80
# define GRAAL_TOOLS_HIERARCHY_SHOW_X 330
# define GRAAL_TOOLS_HIERARCHY_SHOW_Y 280

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

  extern void CallbackToolsGlobalEqui();
  extern void CallbackToolsGlobalUnEqui();
  extern void CallbackToolsGlobalFlat();
  extern void CallbackToolsGlobalUnflat();
  extern void CallbackToolsGlobalPeek();
  extern void CallbackToolsGlobalUnpeek();
  extern void CallbackToolsGlobalDruc();
  extern void CallbackToolsGlobalRealFlat();
  extern void CallbackToolsCloseGlobal();

  extern void CallbackToolsCloseMessage();

  extern void CallbackToolsHierarchyUp();
  extern void CallbackToolsHierarchyDown();
  extern void CallbackToolsHierarchyReload();
  extern void CallbackToolsHierarchyShow();
  extern void CallbackToolsCloseHierarchy();

  extern void CallbackToolsDrucContinue();
  extern void CallbackToolsDrucAbort();

  extern void CallbackDisplayDrucMessage();

  extern void CallbackToolsCloseHierarchyShow();

  extern void GraalDisplayHierarchyShowMessage();

# endif 
