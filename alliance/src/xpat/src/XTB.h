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
| Tool    :                   XPAT                           |
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

# ifndef XPAT_XTB
# define XPAT_XTB

/*------------------------------------------------------------\
|                                                             |
|                        MouseCursors                         |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_NORMAL_CURSOR     0
# define XPAT_WATCH_CURSOR      1
# define XPAT_PIRATE_CURSOR     2 
# define XPAT_CROSS_CURSOR      3 
# define XPAT_NO_CURSOR         4

# define XPAT_MAX_CURSOR        5

/*------------------------------------------------------------\
|                                                             |
|                        DialogBoxType                        |
|                                                             |
\------------------------------------------------------------*/

# define XPAT_DIALOG_MESSAGE   0
# define XPAT_DIALOG_ERROR     1
# define XPAT_DIALOG_INFO      2
# define XPAT_DIALOG_QUESTION  3
# define XPAT_DIALOG_WARNING   4
# define XPAT_DIALOG_WORKING   5
# define XPAT_DIALOG_FILE      6
# define XPAT_DIALOG_PROMPT    7
 
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XpatDialogBox                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XpatDialogItem

  {
    char       *LABEL;
    char        TYPE;
    Widget      WIDGET;
    void        (*CALLBACK_OK)(); 
    XtPointer   CALLDATA_OK;
    void        (*CALLBACK_CANCEL)(); 
    XtPointer   CALLDATA_CANCEL;

  } XpatDialogItem;

  
/*------------------------------------------------------------\
|                                                             |
|                         XpatMenuItem                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XpatMenuItem

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
    struct XpatMenuItem *NEXT;

  } XpatMenuItem;

/*------------------------------------------------------------\
|                                                             |
|                      XpatPanelButtonItem                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XpatPanelButtonItem 

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

  } XpatPanelButtonItem;

/*------------------------------------------------------------\
|                                                             |
|                          XpatPanelItem                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XpatPanelItem

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
    XpatPanelButtonItem  *LIST; 

  } XpatPanelItem;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

 extern char *XpatPanelButtonDisable;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void   XpatDestroyDialogCallback();
  extern void   XpatExitDialogCallback();

  extern void   XpatSetMouseCursor();
  extern Pixmap XpatCreatePixmap();
  extern void   XpatSetIcon();
  extern void   XpatBuildMenus();
  extern void   XpatBuildPanel();
  extern void   XpatEnterPanel();
  extern void   XpatExitPanel();
  extern void   XpatBuildDialog();
  extern void   XpatEnterDialog();
  extern void   XpatReEnterDialog();
  extern void   XpatExitDialog();
  extern void   XpatWarningMessage();
  extern void   XpatErrorMessage();

# endif
