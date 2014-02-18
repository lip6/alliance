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
| Tool    :                   DREAL                           |
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

# ifndef DREAL_EDIT_PANEL
# define DREAL_EDIT_PANEL

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_WINDOW_GLOBAL_X         690
# define DREAL_WINDOW_GLOBAL_Y         205

# define DREAL_EDIT_GLOBAL_X           890
# define DREAL_EDIT_GLOBAL_Y           205

# define DREAL_EDIT_SEARCH_X           890
# define DREAL_EDIT_SEARCH_Y           450
# define DREAL_EDIT_VIEW_SEARCH_X      790
# define DREAL_EDIT_VIEW_SEARCH_Y      450

# define DREAL_EDIT_SELECT_X           330 
# define DREAL_EDIT_SELECT_Y           280

# define DREAL_EDIT_IDENTIFY_X         330 
# define DREAL_EDIT_IDENTIFY_Y         280

# define DREAL_MODIFY_RECTANGLE_X      790
# define DREAL_MODIFY_RECTANGLE_Y      330

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

  extern void CallbackEditGlobalUndo();
  extern void CallbackEditGlobalRedo();
  extern void CallbackEditGlobalCopy();
  extern void CallbackEditGlobalMove();
  extern void CallbackEditGlobalDelete();
  extern void CallbackEditGlobalStretch();
  extern void CallbackEditGlobalModify();
  extern void CallbackEditGlobalIdentify();
  extern void CallbackEditCloseGlobal();

  extern void CallbackEditSearchRectangle();
  extern void CallbackEditCloseSearch();

  extern void CallbackEditCloseIdentify();

  extern void CallbackEditSelectList();
  extern void CallbackEditSelectAccept();
  extern void CallbackEditSelectCancel();

  extern void CallbackEditSearchViewContinue();
  extern void CallbackEditSearchViewAbort();

  extern void CallbackModifyRectangleLayer();
  extern void CallbackModifyRectangleName();
  extern void CallbackModifyRectangleApply();
  extern void CallbackModifyRectangleCancel();

  extern void DrealDisplayIdentifyMessage();
  extern void DrealDisplaySelectList();
  extern void DrealBuildPanelModify();

# endif 
