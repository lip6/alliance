/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                   M_win.c                         |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   04.30.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <ctype.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "G_cache.h"

# include "v_view.h"
# include "v_extern.h"
# include "m_menu.h"
# include "v_menu.h"
# include "i_lines.h"
# include "m_mes_id.h"
# include "e_edit.h"

# include "V_drv.h"
# include "M_win.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

static char M_menuStr[80] = "";
static char M_menuPtr = 0;
static char M_menuScroll = 0;
static char M_viewLoadStr[80] = "";
static char M_editLoadStr[80] = "";
static char M_editSaveStr[80] = "";
static char M_editParamStr[80] = "";
static char M_editShellStr[80] = "";
static char M_editSetTraceStr[80] = "";
static char M_editUnsetTraceStr[80] = "";
static int firstPress;

int firstMove;
int E_NewFile;
int M_x, M_y, M_dx, M_dy;

/* spaces needed to center in olwm */

static char *M_win_name[] = {
  WELCOME,
  "View : LOAD  ",
  "View : ZOOM  ",
  "View : MOOZ  ",
  "Edit : LOAD  ",
  "Edit : SAVE  ",
  "Edit : TRACE  ",
  "Edit : UNTRACE  ",
  "View : SCROLL  ",
  "Edit : PARAMETERS  ",
  "Edit : SHELL  ",
  "GenView : EXIT  ",
  "View : MAP  ",
  "Edit : UNTRACE  "
};

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static void
  rince()
{
register i;

  for (i = 0; i < 77; i++)
    M_menuStr[i] = '\0';
  M_menuPtr = 0;
  XFillRectangle(display, M_window, C_gcText, 7, 70, 200, 16);
  XDrawRectangle(display, M_window, V_gcCao[GC_FGND], 7, 70, 199, 15);
}

static void
  suppress()
{
  if (M_menuPtr > 0) {
    M_menuStr[M_menuPtr + 1] = '\0';
    M_menuStr[M_menuPtr] = ' ';
    M_menuStr[--M_menuPtr] = '_';
    M_menuScroll = M_menuPtr < 31 ? 0 : M_menuPtr - 30;
    XDrawImageString(display, M_window, C_gcTextPop,
                10, 80, &M_menuStr[M_menuScroll],
                strlen(&M_menuStr[M_menuScroll]));
    M_menuStr[M_menuPtr] = '\0';
  } else
    XBell(display, 50);
}

static void
  get_text()
{
  switch (M_id) {
    case M_LOAD :
      strcpy(M_menuStr, M_viewLoadStr);
      break;
    case M_PARAM :
      strcpy(M_menuStr, M_editParamStr);
      break;
    case M_ELOAD :
      strcpy(M_menuStr, M_editLoadStr);
      break;
    case M_SAVE :
      strcpy(M_menuStr, M_editSaveStr);
      break;
    case M_SHELL :
      strcpy(M_menuStr, M_editShellStr);
      break;
    case M_TRACE :
      strcpy(M_menuStr, M_editSetTraceStr);
      break;
    case M_UNTRACE :
      strcpy(M_menuStr, M_editUnsetTraceStr);
      break;
  }
  M_menuPtr = strlen(M_menuStr);
  XFillRectangle(display, M_window, C_gcText, 7, 70, 200, 16);
  XDrawRectangle(display, M_window, V_gcCao[GC_FGND], 7, 70, 199, 15);
  M_menuStr[M_menuPtr++] = '_';
  M_menuStr[M_menuPtr] = '\0';
  M_menuScroll = M_menuPtr < 31 ? 0 : M_menuPtr - 30;
  XDrawImageString(display, M_window, C_gcTextPop,10,80,
              &M_menuStr[M_menuScroll],
              strlen(&M_menuStr[M_menuScroll]));
  M_menuStr[--M_menuPtr] = '\0';
}


static void
  valid()
{
extern int D_FicCharge;
extern int D_FinInterp;

  V_changeCursor(0);

  switch (M_id) {
    case M_LOAD :
      strcpy(M_viewLoadStr,M_menuStr);
      /*
      if(!M_isOnDisk(M_viewLoadStr)) {
      char fn[30];
      sprintf(fn,"Layout file '%s' not found\n",M_viewLoadStr);
        T_print(fn);
        break;
      }
      */
      E_Modif = TRUE;
      M_getFig(M_viewLoadStr);
      break;
    case M_PARAM :
      strcpy(M_editParamStr,M_menuStr);
      E_parameters(M_editParamStr);
      break;
    case M_ELOAD :
/*
      if (strlen(M_menuStr) > 20) {
        T_print("file name too long for UNIX, sorry\n");
        XBell(display, 50);
        break;
      }
*/
      strcpy(M_editLoadStr,M_menuStr);
      strcpy(M_editSaveStr, M_menuStr);
      E_load(M_editLoadStr);
      break;
    case M_SAVE :
      strcpy(M_editSaveStr,M_menuStr);
      E_Save(M_editSaveStr);
      break;
    case M_SHELL :
      strcpy(M_editShellStr,M_menuStr);
      if (strcmp(M_editShellStr, "") == 0) {
        strcpy(M_menuStr,
            "xterm -geometry 80x50+15-50 -title \"GenView : SHELL \"&");
        E_Shell(M_menuStr);
        break;
      }
      E_Shell(M_editShellStr);
      break;
    case M_TRACE :
      strcpy(M_editSetTraceStr,M_menuStr);
      E_SetTrace(M_editSetTraceStr);
      break;
    case M_UNTRACE :
      strcpy(M_editUnsetTraceStr,M_menuStr);
      E_UnSetTrace(M_editUnsetTraceStr);
      break;

  }
  V_changeCursor(1);
}

static lines *
  fill_message(gling, n)
int *gling, *n;
{
static int first;
static lines text[6];
extern int E_Modif;

  switch (M_id) {
    case M_LOAD :
      *gling = 0;
      *n = 1;
      text[0].line = "Please enter the layout filename :";
      text[0].size = strlen(text[0].line);
      get_text();
      break;
    case M_PARAM :
      *gling = 0;
      *n = 1;
      text[0].line = "Please enter the C arguments :";
      text[0].size = strlen(text[0].line);
      get_text();
      break;
    case M_SHELL :
      *gling = 0;
      *n = 1;
      text[0].line = "Please enter the shell command :";
      text[0].size = strlen(text[0].line);
      get_text();
      break;
    case M_ELOAD :
/*
      if (E_NewFile == TRUE && E_Modif == TRUE) {
        *gling = 1;
        *n = 3;
        text[0].line = "The file has been modified but not saved";
        text[0].size = strlen(text[0].line);
        text[1].line = "Click on cancel to avoid loosing the changes";
        text[1].size = strlen(text[1].line);
        text[2].line = "Please enter the source filename :";
        text[2].size = strlen(text[2].line);
      } else {
*/
        *gling = 0;
        *n = 1;
        text[0].line = "Please enter the source filename :";
        text[0].size = strlen(text[0].line);
/*
      }
*/
      get_text();
      break;
    case M_SAVE :
      *gling = 0;
      *n = 1;
      text[0].line = "Please enter the filename :";
      text[0].size = strlen(text[0].line);
      get_text();
      break;
    case M_TRACE :
      *gling = 0;
      *n = 1;
      text[0].line = "Please enter variable name :";
      text[0].size = strlen(text[0].line);
      get_text();
      break;
    case M_UNTRACE :
      *gling = 0;
      *n = 1;
      text[0].line = "Please enter variable name :";
      text[0].size = strlen(text[0].line);
      get_text();
      break;
    case M_ZOOM :
      *gling = 1;
      *n = 4;
      text[0].line = "ILLEGAL ZOOM";
      text[0].size = strlen(text[0].line);
      text[1].line = "If you want to see holes and";
      text[1].size = strlen(text[1].line);
      text[2].line = "electrons, buy spice,";
      text[2].size = strlen(text[2].line);
      text[3].line = "or a magnifing glass";
      text[3].size = strlen(text[3].line);
      break;
    case M_LOOM :
      *gling = 1;
      *n = 3;
      text[0].line = "ILLEGAL MOOZ";
      text[0].size = strlen(text[0].line);
      text[1].line = "your process doesn't probably";
      text[1].size = strlen(text[1].line);
      text[2].line = "allow 20 square cms circuits";
      text[2].size = strlen(text[2].line);
      break;
    case M_NONE :
      *gling = 1;
      *n = 1;
      text[0].line = "What you're seeing doesn't exist";
      text[0].size = strlen(text[0].line);
      break;
    case M_NO_UNTRACE :
      *gling = 1;
      *n = 1;
      text[0].line = "No variables traced yet!";
      text[0].size = strlen(text[0].line);
      break;
     case M_GENVIEW :
      *gling = 1;
      *n = 6;
      text[0].line = "Alliance CAD system";
      text[0].size = strlen(text[0].line);
      text[1].line = " ";
      text[1].size = strlen(text[1].line);
      text[2].line = "GenView";
      text[2].size = strlen(text[2].line);
      text[3].line = "version 4.2";
      text[3].size = strlen(text[3].line);
      text[4].line = "procedural layout Generator Viewer";
      text[4].size = strlen(text[4].line);
      text[5].line = "(c) copyright 1990-1997 ASIM, CAO-VLSI team";
      text[5].size = strlen(text[5].line);
      break;
    case M_MAP :
      *gling = 0;
      *n = 1;
      text[0].line = "A mouse click get you there";
      text[0].size = strlen(text[0].line);
      break;
     case M_SCROLL : {
      char c[50];
        *gling = 1;
        *n = 3;
        text[0].line = "ILLEGAL SCROLL `%' IN SETUP";
        text[0].size = strlen(text[0].line);
        sprintf (c, "Change it to %d", (int) (scale * 100 / dxWin + 2));
        text[1].line = c;
        text[1].size = strlen (c);
        text[2].line = "in order to be able to scroll";
        text[2].size = strlen(text[2].line);
      }
      break;
    case M_EXIT :
      *gling = 1;
      *n = 1;
      text[0].line = "Do you really want to quit ?";
      text[0].size = strlen(text[0].line);
      break;
  }
  return text;
}

static void
  draw_message()
{
int xerr;
int ytext, i, n, bell;
static lines *errMsg;
Window w;
int forget_me;
int dx;

  XGetGeometry(display, M_window, &w, &forget_me, &forget_me,
      &dx, &forget_me, &forget_me, &forget_me);

  errMsg = fill_message(&bell, &n);
  if (!errMsg)
    return;
  ytext = 40;
  for (i = 0; i < n; i++) {
      xerr = (int)((dx / 2) - (errMsg[i].size * 6 + 6) / 2);
      XDrawString(display, M_window, C_gcText, xerr + 2, ytext,
    errMsg[i].line, errMsg[i].size);
      ytext += (i == 0) ? 24 : 18;
  }
  if (bell)
    XBell(display, 50);
}

static void
  add(car)
KeySym car;
{
  if (M_menuPtr < 77) {
    M_menuStr[M_menuPtr] = (char)car;
    M_menuStr[++M_menuPtr] = '_';
    M_menuStr[++M_menuPtr] = ' ';
    M_menuStr[M_menuPtr + 1] = '\0';
    M_menuScroll = M_menuPtr < 32 ? 0 : M_menuPtr - 31;
    XDrawImageString(display, M_window, C_gcTextPop,
                10, 80, &M_menuStr[M_menuScroll], 
                strlen(&M_menuStr[M_menuScroll]));
    M_menuStr[--M_menuPtr] = '\0';
  } else
    XBell(display, 50);
}

void
  M_open()
{
XSizeHints hint;

  hint.x = 120;
  hint.y = 120;
  hint.width = 220;
  hint.height = 120;

  /* adapt the window size to the message need */
  switch (M_id) {
    case M_GENVIEW :
      hint.width += 80;
      hint.height += 60;
      break;
    case M_MAP :
      hint.height += 100;
      break;
    case M_EXIT :
    case M_NO_UNTRACE :
      hint.height -= 60;
  }

  hint.flags = PPosition | PSize;

  M_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
      hint.x, hint.y, hint.width, hint.height,
      5, V_colors[COLOR_TEXT], V_colors[COLOR_M_BGND]);

  XSelectInput(display, M_window, ButtonReleaseMask |
               KeyPressMask | ButtonPressMask | ExposureMask);

  XSetStandardProperties(display, M_window, M_win_name[M_id],
      "GenView", None, NULL, 0, &hint);
  M_createCursor();
  XMapRaised(display, M_window);

  firstMove = 0;
  firstPress = 0;

  XMapRaised(display, M_window);
  XRaiseWindow(display, M_window);
  XMoveWindow( display, M_window, hint.x, hint.y );
}

int
  M_event()
{
int x, y;
static int witchIcon;
char text;
KeySym key;
XWindowAttributes WAttr;

  switch (V_event.type) {

    case Expose:

      XGetWindowAttributes( display, M_window, &WAttr );

      if ( WAttr.map_state == IsViewable )
      {
        XSetInputFocus(display, M_window, RevertToParent, CurrentTime);
      }

      M_setMenu();
      M_drawMenu();
      draw_message();
      break;

    case ButtonPress:

      XGetWindowAttributes( display, M_window, &WAttr );

      if ( WAttr.map_state == IsViewable )
      {
        XSetInputFocus(display, M_window, RevertToParent, CurrentTime);
      }

      x = V_event.xbutton.x;
      y = V_event.xbutton.y;
      witchIcon = M_clickedIcon(x, y);
      switch (witchIcon) {
        case M_OK:
        case M_OK_OP:
        case M_CANCEL:
          M_drawIcon(witchIcon, ON);
          break;
        case INSCREEN:
          if (M_id == M_MAP)
            M_moveView(x,y);
          break;
      }
      break;

      case ButtonRelease:
        switch (witchIcon) {
          case M_CANCEL :
          case M_OK :
            M_drawIcon(witchIcon, OFF);
            XDestroyWindow(display, M_window);

            XGetWindowAttributes( display, V_window, &WAttr );

            if ( WAttr.map_state == IsViewable )
            {
              XSetInputFocus(display, V_window, RevertToNone, CurrentTime);
            }

            return CLOSE_WINDOW;
          case M_OK_OP :
            if (M_id == M_EXIT) 
              return CLOSE_GENVIEW;
            M_drawIcon(M_OK_OP, OFF);
            valid();  
            XDestroyWindow(display, M_window);

            XGetWindowAttributes( display, V_window, &WAttr );

            if ( WAttr.map_state == IsViewable )
            {
              XSetInputFocus(display, V_window, RevertToNone, CurrentTime);
            }

            return CLOSE_WINDOW;
          case INSCREEN:
            break;
       }
       break;

      case KeyPress :
        (void)XLookupString(&V_event.xkey, &text, 1, &key,0);

        if (M_id == M_GENVIEW) {
          M_drawIcon(M_OK, ON);
          XDestroyWindow(display, M_window);
          return CLOSE_WINDOW;
        }
          
        if (M_id == M_EXIT)
          if (key == XK_Escape) {
            M_drawIcon(M_CANCEL, ON);
            XDestroyWindow(display, M_window);
            return CLOSE_WINDOW;
          } else if (key == XK_Return) {
            M_drawIcon(M_OK_OP, ON);
            return CLOSE_GENVIEW;
          }

        if (M_id != M_LOAD && M_id != M_PARAM 
            && M_id != M_ELOAD && M_id != M_TRACE 
            && M_id != M_UNTRACE && M_id != M_SHELL 
            && M_id != M_SAVE) 
          break;
        
        switch (key) {
          case XK_Return:
          case XK_Linefeed:
          case XK_KP_Enter:
            M_drawIcon(M_OK_OP, ON);

            XGetWindowAttributes( display, V_window, &WAttr );

            if ( WAttr.map_state == IsViewable )
            {
              XSetInputFocus(display, V_window, RevertToNone, CurrentTime);
            }

            M_drawIcon(M_OK_OP, OFF);
            valid();
            XDestroyWindow(display, M_window);
            return CLOSE_WINDOW;
          case XK_BackSpace:
          case XK_Delete:
            suppress();
            break;
          case XK_Shift_R:
          case XK_Shift_L:
            break;
          case XK_Up:
          case XK_Down:
          case XK_Right:
            XBell(display, 50);
            break;
          case XK_Left:
            if (firstPress == 0)
              suppress();
            else
              XBell(display, 50);
            break;
          case XK_Escape:
            M_drawIcon(M_CANCEL, ON);
            XDestroyWindow(display, M_window);
            return CLOSE_WINDOW;
          default:
            if (firstPress == 0)
              rince();
            add(key);
            break;
        }
        firstPress = 1;
        break;

     default :
       break;
    }
  return NO_WINDOW_ACTION;
}

void
  M_getFig(str)
char *str;
{
char *buffer;
extern char run_mode;

  run_mode = STEP; /* should draw everithing */
  if (strlen(str)) {
    buffer = (char *)mbkalloc((10+strlen(str))*sizeof(char));
    Load_fig(str);
    sprintf(buffer,"View : %s",str);
    XStoreName(display, V_window, buffer); 
    V_fitscreen();
    Update_AB();
    mbkfree(buffer);
  }
}

int
  M_isOnDisk(figname)
char *figname;
{
FILE *flush;

  if ((flush = mbkfopen(figname, IN_PH, "r")) == NULL)
       return 0;
  (void)fclose(flush);
  return 1;
}
