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
| Tool    :                   XSCH                           |
|                                                             |
| File    :                   XTB.h                           |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                   01.06.96                        |
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

# ifndef XSCH_XTB
# define XSCH_XTB

/*------------------------------------------------------------\
|                                                             |
|                        MouseCursors                         |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_NORMAL_CURSOR     0
# define XSCH_WATCH_CURSOR      1
# define XSCH_PIRATE_CURSOR     2 
# define XSCH_CROSS_CURSOR      3 
# define XSCH_NO_CURSOR         4

# define XSCH_MAX_CURSOR        5

/*------------------------------------------------------------\
|                                                             |
|                        DialogBoxType                        |
|                                                             |
\------------------------------------------------------------*/

# define XSCH_DIALOG_MESSAGE   0
# define XSCH_DIALOG_ERROR     1
# define XSCH_DIALOG_INFO      2
# define XSCH_DIALOG_QUESTION  3
# define XSCH_DIALOG_WARNING   4
# define XSCH_DIALOG_WORKING   5
# define XSCH_DIALOG_FILE      6
# define XSCH_DIALOG_PROMPT    7
 
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         XschDialogBox                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XschDialogItem

  {
    char       *LABEL;
    char        TYPE;
    Widget      WIDGET;
    void        (*CALLBACK_OK)(); 
    XtPointer   CALLDATA_OK;
    void        (*CALLBACK_CANCEL)(); 
    XtPointer   CALLDATA_CANCEL;

  } XschDialogItem;

  
/*------------------------------------------------------------\
|                                                             |
|                         XschMenuItem                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XschMenuItem

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
    struct XschMenuItem *NEXT;

  } XschMenuItem;

/*------------------------------------------------------------\
|                                                             |
|                      XschPanelButtonItem                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XschPanelButtonItem 

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

  } XschPanelButtonItem;

/*------------------------------------------------------------\
|                                                             |
|                          XschPanelItem                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct XschPanelItem

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
    XschPanelButtonItem  *LIST; 

  } XschPanelItem;

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

 extern char *XschPanelButtonDisable;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void   XschDestroyDialogCallback();
  extern void   XschExitDialogCallback();

  extern void   XschSetMouseCursor();
  extern Pixmap XschCreatePixmap();
  extern void   XschSetIcon();
  extern void   XschBuildMenus();
  extern void   XschBuildPanel();
  extern void   XschEnterPanel();
  extern void   XschExitPanel();
  extern void   XschBuildDialog();
  extern void   XschEnterDialog();
  extern void   XschReEnterDialog();
  extern void   XschExitDialog();
  extern void   XschWarningMessage();
  extern void   XschErrorMessage();
  extern void   XschLimitedLoop();

# endif
