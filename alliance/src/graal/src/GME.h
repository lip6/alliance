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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Gme101.h                         |
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

# ifndef GRAAL_GME 
# define GRAAL_GME 

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

  extern GraalMenuItem  GraalEditMenu[];
  extern GraalMenuItem  GraalWindowMenu[];
  extern GraalPanelItem GraalEditGlobalPanel;
  extern GraalPanelItem GraalWindowGlobalPanel;
  extern GraalPanelItem GraalEditSearchPanel;
  extern GraalPanelItem GraalEditSearchViewPanel;
  extern GraalPanelItem GraalEditIdentifyPanel;
  extern GraalPanelItem GraalEditSelectPanel;
  extern GraalPanelItem GraalModifySegmentPanel;
  extern GraalPanelItem GraalModifyTransistorPanel;
  extern GraalPanelItem GraalModifyConnectorPanel;
  extern GraalPanelItem GraalModifyReferencePanel;
  extern GraalPanelItem GraalModifyInstancePanel;
  extern GraalPanelItem GraalModifyViaPanel;
  extern GraalPanelItem GraalModifyBigViaPanel;

  extern int GraalEditGlobalDefaultValues[ 5 ];
  extern int GraalWindowGlobalDefaultValues[ 5 ];
  extern int GraalEditSearchDefaultValues[ 5 ];
  extern int GraalEditIdentifyDefaultValues[ 5 ];
  extern int GraalEditSelectDefaultValues[ 5 ];
  extern int GraalEditSearchViewDefaultValues[ 5 ];
  extern int GraalModifySegmentDefaultValues[ 5 ];
  extern int GraalModifyTransistorDefaultValues[ 5 ];
  extern int GraalModifyConnectorDefaultValues[ 5 ];
  extern int GraalModifyReferenceDefaultValues[ 5 ];
  extern int GraalModifyInstanceDefaultValues[ 5 ];
  extern int GraalModifyViaDefaultValues[ 5 ];
  extern int GraalModifyBigViaDefaultValues[ 5 ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void GraalBuidlPanelModify();
  extern void GraalBuildEditDialog();

  extern void GraalEditSelectPointDelete();
  extern void GraalEditSelectWindowDelete();
  extern void GraalEditSelectPointCopy();
  extern void GraalEditSelectWindowCopy();
  extern void GraalEditSelectPointMove();
  extern void GraalEditSelectWindowMove();
  extern void GraalEditSelectPointModify();
  extern void GraalEditSelectWindowModify();

  extern void GraalEditIdentify();
  extern void GraalEditUnselectAll();

  extern void GraalEditSelectPoint();
  extern void GraalEditSelectWindow();
  extern void GraalEditSelectFence();

  extern void GraalEditUndo();
  extern void GraalEditRedo();
  extern void GraalEditCopy();
  extern void GraalEditMove();
  extern void GraalEditDelete();
  extern void GraalEditModify();
  extern void GraalEditStretch();

  extern void GraalToolsSelectEqui();
  extern void GraalToolsSelectFlatten();
  extern void GraalToolsSelectUnflatten();
  extern void GraalToolsSelectRealFlatten();
  extern void GraalToolsSelectHierarchyDown();

# endif
