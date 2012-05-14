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
| File    :                  Dialog.h                         |
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

# ifndef GRAAL_EDIT_DIALOG
# define GRAAL_EDIT_DIALOG


/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
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

  extern GraalDialogItem GraalSearchConnectorDialog;
  extern GraalDialogItem GraalSearchInstanceDialog;
  extern GraalDialogItem GraalSearchReferenceDialog;
  extern GraalDialogItem GraalSearchViaDialog;
  extern GraalDialogItem GraalSearchSegmentDialog;

  extern GraalDialogItem GraalModifySegmentWidthDialog;
  extern GraalDialogItem GraalModifySegmentNameDialog;
  extern GraalDialogItem GraalModifyTransistorWidthDialog;
  extern GraalDialogItem GraalModifyTransistorNameDialog;
  extern GraalDialogItem GraalModifyConnectorWidthDialog;
  extern GraalDialogItem GraalModifyConnectorNameDialog;
  extern GraalDialogItem GraalModifyReferenceNameDialog;
  extern GraalDialogItem GraalModifyInstanceNameDialog;
  extern GraalDialogItem GraalModifyInstanceModelDialog;
  extern GraalDialogItem GraalModifyViaNameDialog;
  extern GraalDialogItem GraalModifyBigViaNameDialog;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void CallbackModifySegmentWidthOk();
  extern void CallbackModifySegmentWidthCancel();
  extern void CallbackModifySegmentNameOk();
  extern void CallbackModifySegmentNameCancel();

  extern void CallbackModifyTransistorWidthOk();
  extern void CallbackModifyTransistorWidthCancel();
  extern void CallbackModifyTransistorNameOk();
  extern void CallbackModifyTransistorNameCancel();

  extern void CallbackModifyConnectorWidthOk();
  extern void CallbackModifyConnectorWidthCancel();
  extern void CallbackModifyConnectorNameOk();
  extern void CallbackModifyConnectorNameCancel();

  extern void CallbackModifyViaNameOk();
  extern void CallbackModifyViaNameCancel();
  extern void CallbackModifyBigViaNameOk();
  extern void CallbackModifyBigViaNameCancel();

  extern void CallbackModifyInstanceNameOk();
  extern void CallbackModifyInstanceNameCancel();

  extern void CallbackModifyInstanceModelOk();
  extern void CallbackModifyInstanceModelCancel();

  extern void CallbackModifyReferenceNameOk();
  extern void CallbackModifyReferenceNameCancel();

  extern void CallbackSearchConnectorOk();
  extern void CallbackSearchConnectorCancel();

  extern void CallbackSearchInstanceOk();
  extern void CallbackSearchInstanceCancel();

  extern void CallbackSearchReferenceOk();
  extern void CallbackSearchReferenceCancel();

  extern void CallbackSearchSegmentOk();
  extern void CallbackSearchSegmentCancel();

  extern void CallbackSearchViaOk();
  extern void CallbackSearchViaCancel();

# endif 
