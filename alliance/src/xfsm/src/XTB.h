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
| Tool    :                   XFSM                           |
|                                                             |
| File    :                   XTB.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   04.12.96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef XFSM_XTB
# define XFSM_XTB

/*------------------------------------------------------------\
|                                                             |
|                        MouseCursors                         |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_NORMAL_CURSOR     0
# define XFSM_WATCH_CURSOR      1
# define XFSM_PIRATE_CURSOR     2 
# define XFSM_CROSS_CURSOR      3 
# define XFSM_NO_CURSOR         4

# define XFSM_MAX_CURSOR        5

/*------------------------------------------------------------\
|                                                             |
|                        DialogBoxType                        |
|                                                             |
\------------------------------------------------------------*/

# define XFSM_DIALOG_MESSAGE   0
# define XFSM_DIALOG_ERROR     1
# define XFSM_DIALOG_INFO      2
# define XFSM_DIALOG_QUESTION  3
# define XFSM_DIALOG_WARNING   4
# define XFSM_DIALOG_WORKING   5
# define XFSM_DIALOG_FILE      6
# define XFSM_DIALOG_PROMPT    7
 
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XfsmDialogBox                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XfsmDialogItem

  {
    char       *LABEL;
    char        TYPE;
    Widget      WIDGET;
    void        (*CALLBACK_OK)(); 
    XtPointer   CALLDATA_OK;
    void        (*CALLBACK_CANCEL)(); 
    XtPointer   CALLDATA_CANCEL;

  } XfsmDialogItem;

  
/*------------------------------------------------------------\
|                                                             |
|                         XfsmMenuItem                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XfsmMenuItem

  {
    char                 *LABEL;
    char                  MNEMONIC; 
    char                 *SHORT_KEY;
    char                 *SHORT_KEY_TEXT;
    WidgetClass          *CLASS; 
    Boolean               SEPARATOR; 
    Boolean               TITLE;  
    Boolean               HELP;
    void                  (*CALLBACK)(); 
    XtPointer             CALLDATA;
    Widget                BUTTON;
    Widget                MENU;
    struct XfsmMenuItem *NEXT;

  } XfsmMenuItem;

/*------------------------------------------------------------\
|                                                             |
|                      XfsmPanelButtonItem                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XfsmPanelButtonItem 

  {
    char                       **LABEL;
    char                        *TEXT;
    char                        *BITMAP;
    int                          WIDTH;
    int                          HEIGHT;
    char                       **FOREGROUND;
    char                       **BACKGROUND;
    unsigned char                X;
    unsigned char                Y;
    unsigned char                DX;
    unsigned char                DY;
    void                         (*CALLBACK)(); 
    XtPointer                    CALLDATA;
    Widget                       BUTTON;

  } XfsmPanelButtonItem;

/*------------------------------------------------------------\
|                                                             |
|                          XfsmPanelItem                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XfsmPanelItem

  {
    char                  *TITLE;
    char                   COMPUTE;
    char                   MANAGED;
    int                    X;
    int                    Y;
    int                    WIDTH;
    int                    HEIGHT;
    unsigned char          COLUMN;
    unsigned char          ROW;
    Widget                 PANEL;
    Widget                 PANEL_FORM;
    Widget                 FRAME;
    Widget                 FORM;
    XfsmPanelButtonItem  *LIST; 

  } XfsmPanelItem;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

 extern char *XfsmPanelButtonDisable;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void   XfsmDestroyDialogCallback();
  extern void   XfsmExitDialogCallback();

  extern void   XfsmSetMouseCursor();
  extern Pixmap XfsmCreatePixmap();
  extern void   XfsmSetIcon();
  extern void   XfsmBuildMenus();
  extern void   XfsmBuildPanel();
  extern void   XfsmEnterPanel();
  extern void   XfsmExitPanel();
  extern void   XfsmBuildDialog();
  extern void   XfsmEnterDialog();
  extern void   XfsmReEnterDialog();
  extern void   XfsmExitDialog();
  extern void   XfsmWarningMessage();
  extern void   XfsmErrorMessage();

# endif
