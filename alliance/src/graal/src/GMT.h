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
| File    :                 GMT101.h                          |
|                                                             |
| Author  :                Jacomme Ludovic                    |
|                                                             |
| Date    :                  28.03.95                         |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_GMT
# define GRAAL_GMT 

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

  extern GraalMenuItem  GraalToolsMenu[];
  extern GraalPanelItem GraalToolsGlobalPanel;
  extern GraalPanelItem GraalToolsMessagePanel;
  extern GraalPanelItem GraalToolsDrucPanel;
  extern GraalPanelItem GraalToolsHierarchyPanel;
  extern GraalPanelItem GraalToolsHierarchyShowPanel;

  extern int GraalToolsGlobalDefaultValues[ 5 ];
  extern int GraalToolsMessageDefaultValues[ 5 ];
  extern int GraalToolsDrucDefaultValues[ 5 ];
  extern int GraalToolsHierarchyDefaultValues[ 5 ];
  extern int GraalToolsHierarchyShowDefaultValues[ 5 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalToolsPeek();
  extern void GraalToolsEqui();
  extern void GraalToolsFlatten();
  extern void GraalToolsUnflatten();
  extern void GraalToolsRealFlatten();
  extern void GraalToolsHierarchyDown();
  extern void GraalToolsHierarchyShow();

  extern void GraalDisplayToolsMessage();
  extern void GraalBuildToolsDialog();

# endif
