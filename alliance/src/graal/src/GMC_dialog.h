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

# ifndef GRAAL_CREATE_DIALOG
# define GRAAL_CREATE_DIALOG

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

  extern char            GraalCreateDialogCancel;

  extern GraalDialogItem GraalChangeSegmentWidthDialog;
  extern GraalDialogItem GraalChangeSegmentNameDialog;
  extern GraalDialogItem GraalChangeViaNameDialog;
  extern GraalDialogItem GraalChangeBigViaNameDialog;
  extern GraalDialogItem GraalChangeTransistorWidthDialog;
  extern GraalDialogItem GraalChangeTransistorNameDialog;
  extern GraalDialogItem GraalChangeConnectorWidthDialog;
  extern GraalDialogItem GraalChangeConnectorNameDialog;
  extern GraalDialogItem GraalChangeReferenceNameDialog;
  extern GraalDialogItem GraalChangeInstanceNameDialog;
  extern GraalDialogItem GraalChangeInstanceModelDialog;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void CallbackChangeSegmentWidthOk();
  extern void CallbackChangeSegmentWidthCancel();
  extern void CallbackChangeSegmentNameOk();
  extern void CallbackChangeSegmentNameCancel();

  extern void CallbackChangeViaNameOk();
  extern void CallbackChangeViaNameCancel();

  extern void CallbackChangeBigViaNameOk();
  extern void CallbackChangeBigViaNameCancel();

  extern void CallbackChangeTransistorWidthOk();
  extern void CallbackChangeTransistorWidthCancel();
  extern void CallbackChangeTransistorNameOk();
  extern void CallbackChangeTransistorNameCancel();

  extern void CallbackChangeConnectorWidthOk();
  extern void CallbackChangeConnectorWidthCancel();
  extern void CallbackChangeConnectorNameOk();
  extern void CallbackChangeConnectorNameCancel();

  extern void CallbackChangeReferenceNameOk();
  extern void CallbackChangeReferenceNameCancel();

  extern void CallbackChangeInstanceNameOk();
  extern void CallbackChangeInstanceNameCancel();
  extern void CallbackChangeInstanceModelOk();
  extern void CallbackChangeInstanceModelCancel();

  extern void GraalEnterChangeInstanceModelDialog();

# endif 
