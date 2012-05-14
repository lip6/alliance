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
| File    :                   gtb101.h                        |
|                                                             |
| Authors :              Jacomme Ludovic                      |
|                                                             |
| Date    :                  28.03.95                         |
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

# ifndef DREAL_GTB
# define DREAL_GTB

/*------------------------------------------------------------\
|                                                             |
|                        MouseCursors                         |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_NORMAL_CURSOR     0
# define DREAL_WATCH_CURSOR      1
# define DREAL_PIRATE_CURSOR     2 
# define DREAL_CROSS_CURSOR      3 
# define DREAL_NO_CURSOR         4

# define DREAL_MAX_CURSOR        5

/*------------------------------------------------------------\
|                                                             |
|                        DialogBoxType                        |
|                                                             |
\------------------------------------------------------------*/

# define DREAL_DIALOG_MESSAGE   0
# define DREAL_DIALOG_ERROR     1
# define DREAL_DIALOG_INFO      2
# define DREAL_DIALOG_QUESTION  3
# define DREAL_DIALOG_WARNING   4
# define DREAL_DIALOG_WORKING   5
# define DREAL_DIALOG_FILE      6
# define DREAL_DIALOG_PROMPT    7
 
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrealDialogBox                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct DrealDialogItem

  {
    char       *LABEL;
    char        TYPE;
    Widget      WIDGET;
    void        (*CALLBACK_OK)(); 
    XtPointer   CALLDATA_OK;
    void        (*CALLBACK_CANCEL)(); 
    XtPointer   CALLDATA_CANCEL;

  } DrealDialogItem;

  
/*------------------------------------------------------------\
|                                                             |
|                         DrealMenuItem                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct DrealMenuItem

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
    struct DrealMenuItem *NEXT;

  } DrealMenuItem;

/*------------------------------------------------------------\
|                                                             |
|                      DrealPanelButtonItem                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct DrealPanelButtonItem 

  {
    char                        *LABEL;
    char                        *TEXT;
    char                        *BITMAP;
    int                          WIDTH;
    int                          HEIGHT;
    char                        *FOREGROUND;
    char                        *BACKGROUND;
    unsigned char                X;
    unsigned char                Y;
    unsigned char                DX;
    unsigned char                DY;
    void                         (*CALLBACK)(); 
    XtPointer                    CALLDATA;
    Widget                       BUTTON;

  } DrealPanelButtonItem;

/*------------------------------------------------------------\
|                                                             |
|                          DrealPanelItem                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct DrealPanelItem

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
    unsigned char          LIST_NUMBER;
    DrealPanelButtonItem  *LIST; 

  } DrealPanelItem;


/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

 extern char *DrealPanelButtonDisable;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

  extern void   DrealDestroyDialogCallback();
  extern void   DrealExitDialogCallback();

  extern void   DrealSetMouseCursor();
  extern Pixmap DrealCreatePixmap();
  extern void   DrealSetIcon();
  extern void   DrealBuildMenus();
  extern void   DrealBuildPanel();
  extern void   DrealEnterPanel();
  extern void   DrealExitPanel();
  extern void   DrealBuildDialog();
  extern void   DrealEnterDialog();
  extern void   DrealReEnterDialog();
  extern void   DrealExitDialog();
  extern void   DrealWarningMessage();
  extern void   DrealErrorMessage();

# endif
