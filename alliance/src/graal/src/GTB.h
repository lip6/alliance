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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                   gtb101.h                        |
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
/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

# ifndef GRAAL_GTB
# define GRAAL_GTB

/*------------------------------------------------------------\
|                                                             |
|                        MouseCursors                         |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_NORMAL_CURSOR     0
# define GRAAL_WATCH_CURSOR      1
# define GRAAL_PIRATE_CURSOR     2 
# define GRAAL_CROSS_CURSOR      3 
# define GRAAL_NO_CURSOR         4

# define GRAAL_MAX_CURSOR        5

/*------------------------------------------------------------\
|                                                             |
|                        DialogBoxType                        |
|                                                             |
\------------------------------------------------------------*/

# define GRAAL_DIALOG_MESSAGE   0
# define GRAAL_DIALOG_ERROR     1
# define GRAAL_DIALOG_INFO      2
# define GRAAL_DIALOG_QUESTION  3
# define GRAAL_DIALOG_WARNING   4
# define GRAAL_DIALOG_WORKING   5
# define GRAAL_DIALOG_FILE      6
# define GRAAL_DIALOG_PROMPT    7
 
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         GraalDialogBox                      |
|                                                             |
\------------------------------------------------------------*/

  typedef struct GraalDialogItem

  {
    char       *LABEL;
    char        TYPE;
    Widget      WIDGET;
    void        (*CALLBACK_OK)(); 
    XtPointer   CALLDATA_OK;
    void        (*CALLBACK_CANCEL)(); 
    XtPointer   CALLDATA_CANCEL;

  } GraalDialogItem;

  
/*------------------------------------------------------------\
|                                                             |
|                         GraalMenuItem                       |
|                                                             |
\------------------------------------------------------------*/

  typedef struct GraalMenuItem

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
    struct GraalMenuItem *NEXT;

  } GraalMenuItem;

/*------------------------------------------------------------\
|                                                             |
|                      GraalPanelButtonItem                   |
|                                                             |
\------------------------------------------------------------*/

  typedef struct GraalPanelButtonItem 

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

  } GraalPanelButtonItem;

/*------------------------------------------------------------\
|                                                             |
|                          GraalPanelItem                     |
|                                                             |
\------------------------------------------------------------*/

  typedef struct GraalPanelItem

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
    GraalPanelButtonItem  *LIST; 

  } GraalPanelItem;

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

  extern void   GraalDestroyDialogCallback();
  extern void   GraalExitDialogCallback();

  extern void   GraalSetMouseCursor();
  extern Pixmap GraalCreatePixmap();
  extern void   GraalSetIcon();
  extern void   GraalBuildMenus();
  extern void   GraalBuildPanel();
  extern void   GraalEnterPanel();
  extern void   GraalExitPanel();
  extern void   GraalBuildDialog();
  extern void   GraalEnterDialog();
  extern void   GraalReEnterDialog();
  extern void   GraalExitDialog();
  extern void   GraalWarningMessage();
  extern void   GraalErrorMessage();

# endif
