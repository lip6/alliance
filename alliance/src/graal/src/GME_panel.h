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

# ifndef GRAAL_EDIT_PANEL
# define GRAAL_EDIT_PANEL

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_WINDOW_GLOBAL_X         690
# define GRAAL_WINDOW_GLOBAL_Y         205

# define GRAAL_EDIT_GLOBAL_X           890
# define GRAAL_EDIT_GLOBAL_Y           205

# define GRAAL_EDIT_SEARCH_X           890
# define GRAAL_EDIT_SEARCH_Y           450
# define GRAAL_EDIT_VIEW_SEARCH_X      790
# define GRAAL_EDIT_VIEW_SEARCH_Y      450

# define GRAAL_EDIT_SELECT_X           330 
# define GRAAL_EDIT_SELECT_Y           280

# define GRAAL_EDIT_IDENTIFY_X         330 
# define GRAAL_EDIT_IDENTIFY_Y         280

# define GRAAL_MODIFY_SEGMENT_X        790
# define GRAAL_MODIFY_SEGMENT_Y        230
# define GRAAL_MODIFY_VIA_X            790
# define GRAAL_MODIFY_VIA_Y            330
# define GRAAL_MODIFY_BIGVIA_X         790
# define GRAAL_MODIFY_BIGVIA_Y         330
# define GRAAL_MODIFY_TRANSISTOR_X     790
# define GRAAL_MODIFY_TRANSISTOR_Y     330
# define GRAAL_MODIFY_CONNECTOR_X      790
# define GRAAL_MODIFY_CONNECTOR_Y      330
# define GRAAL_MODIFY_REFERENCE_X      790
# define GRAAL_MODIFY_REFERENCE_Y      330
# define GRAAL_MODIFY_INSTANCE_X       790
# define GRAAL_MODIFY_INSTANCE_Y       330

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

  extern void CallbackEditSearchConnector();
  extern void CallbackEditSearchInstance();
  extern void CallbackEditSearchReference();
  extern void CallbackEditSearchVia();
  extern void CallbackEditSearchSegment();
  extern void CallbackEditCloseSearch();

  extern void CallbackEditCloseIdentify();

  extern void CallbackEditSelectList();
  extern void CallbackEditSelectAccept();
  extern void CallbackEditSelectCancel();

  extern void CallbackEditSearchViewContinue();
  extern void CallbackEditSearchViewAbort();

  extern void CallbackModifySegmentLayer();
  extern void CallbackModifySegmentWidth();
  extern void CallbackModifySegmentName();
  extern void CallbackModifySegmentApply();
  extern void CallbackModifySegmentCancel();

  extern void CallbackModifyConnectorLayer();
  extern void CallbackModifyConnectorWidth();
  extern void CallbackModifyConnectorOrient();
  extern void CallbackModifyConnectorName();
  extern void CallbackModifyConnectorApply();
  extern void CallbackModifyConnectorCancel();

  extern void CallbackModifyReferenceType();
  extern void CallbackModifyReferenceName();
  extern void CallbackModifyReferenceApply();
  extern void CallbackModifyReferenceCancel();

  extern void CallbackModifyInstanceSym();
  extern void CallbackModifyInstanceName();
  extern void CallbackModifyInstanceModel();
  extern void CallbackModifyInstanceApply();
  extern void CallbackModifyInstanceCancel();

  extern void CallbackModifyTransistorType();
  extern void CallbackModifyTransistorWidth();
  extern void CallbackModifyTransistorName();
  extern void CallbackModifyTransistorApply();
  extern void CallbackModifyTransistorCancel();

  extern void CallbackModifyViaType();
  extern void CallbackModifyViaName();
  extern void CallbackModifyViaApply();
  extern void CallbackModifyViaCancel();

  extern void CallbackModifyBigViaType();
  extern void CallbackModifyBigViaName();
  extern void CallbackModifyBigViaApply();
  extern void CallbackModifyBigViaCancel();

  extern void GraalDisplayIdentifyMessage();
  extern void GraalDisplaySelectList();

# endif 
