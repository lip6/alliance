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
| Tool    :                   XGRA                           |
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

# ifndef XGRA_XTB
# define XGRA_XTB

/*------------------------------------------------------------\
|                                                             |
|                        MouseCursors                         |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_NORMAL_CURSOR     0
# define XGRA_WATCH_CURSOR      1
# define XGRA_PIRATE_CURSOR     2 
# define XGRA_CROSS_CURSOR      3 
# define XGRA_NO_CURSOR         4

# define XGRA_MAX_CURSOR        5

/*------------------------------------------------------------\
|                                                             |
|                        DialogBoxType                        |
|                                                             |
\------------------------------------------------------------*/

# define XGRA_DIALOG_MESSAGE   0
# define XGRA_DIALOG_ERROR     1
# define XGRA_DIALOG_INFO      2
# define XGRA_DIALOG_QUESTION  3
# define XGRA_DIALOG_WARNING   4
# define XGRA_DIALOG_WORKING   5
# define XGRA_DIALOG_FILE      6
# define XGRA_DIALOG_PROMPT    7
 
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XgraDialogBox                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XgraDialogItem

  {
    char       *LABEL;
    char        TYPE;
    Widget      WIDGET;
    void        (*CALLBACK_OK)(); 
    XtPointer   CALLDATA_OK;
    void        (*CALLBACK_CANCEL)(); 
    XtPointer   CALLDATA_CANCEL;

  } XgraDialogItem;

  
/*------------------------------------------------------------\
|                                                             |
|                         XgraMenuItem                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XgraMenuItem

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
    struct XgraMenuItem *NEXT;

  } XgraMenuItem;

/*------------------------------------------------------------\
|                                                             |
|                      XgraPanelButtonItem                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XgraPanelButtonItem 

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

  } XgraPanelButtonItem;

/*------------------------------------------------------------\
|                                                             |
|                          XgraPanelItem                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XgraPanelItem

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
    XgraPanelButtonItem  *LIST; 

  } XgraPanelItem;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

 extern char *XgraPanelButtonDisable;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void   XgraDestroyDialogCallback();
  extern void   XgraExitDialogCallback();

  extern void   XgraSetMouseCursor();
  extern Pixmap XgraCreatePixmap();
  extern void   XgraSetIcon();
  extern void   XgraBuildMenus();
  extern void   XgraBuildPanel();
  extern void   XgraEnterPanel();
  extern void   XgraExitPanel();
  extern void   XgraBuildDialog();
  extern void   XgraEnterDialog();
  extern void   XgraReEnterDialog();
  extern void   XgraExitDialog();
  extern void   XgraWarningMessage();
  extern void   XgraErrorMessage();

# endif
