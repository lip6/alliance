/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
| File    :                   V_drv.c                         |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   12.04.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

# include "v_view.h"
# include "i_lines.h"
# include "v_menu.h"
# include "v_extern.h"
# include "m_mes_id.h"
# include "V_drv.h"
# include "G_peek.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

/* definition of all useful globals for xV_visu application
  declaration in drv.c to clean up the main programm */

  Display *display;
  Window E_window;
  Window T_window;
  Window V_window;
  Window I_window;
  Window C_window;
  Window M_window;
  XRectangle V_I_pos;
  XEvent V_event;
  Colormap V_cmap;
  int V_depth;
  int V_class;
  int V_width, V_height ;
  Visual *V_vis;
  int V_screen;

  /* these variables are comming from the V_colors.c file */
  int V_colors[ CMAP_MAX_SIZE ];
  char *V_colorName[ GENVIEW_MAX_COLOR ] = 
  {
    "White",          /* fgnd          */
    "Black",          /* bgnd          */
    "Wheat",          /* M_BGND        */
    "Dodger Blue",    /* ICON          */
    "yellow",         /* ICON_TEXT     */
    "yellow",         /* ICON_INV      */
    "Dodger Blue",    /* ICON_TEXT_INV */
    "Black",          /* TEXT          */
    "cyan",           /* button        */
    "sea green",      /* button        */
    "red",            /* RED           */
    "blue"            /* BLUE          */
  };

  Pixmap V_PixSave;
  GC V_gcCao[ GENVIEW_ALL_GC ];
  GC V_gcBox, C_gcV1, C_gcV2, C_gcV3, C_gcV4;
  Font V_vr[6]; /* font for textual output */
  char *V_fontName[] = {
                "6x10",
                "9x15",
                "9x15",
                "9x15",
                "9x15",
                "9x15"
  };
  Font V_menuFont;

/* as it is many times used, the V_windows size attributes
  are passed to global variables. */
  int V_xWin,V_yWin,dxWin,dyWin;
  int V_xZoom,V_yZoom,V_dxZoom,V_dyZoom;
  long dxChip,dyChip;

/* Some constants for scaling purposes */
  int V_fact = 5;
  int V_pixDep = 16; /* scrolling is considered in terms of lambda */
  YRectangle *V_icon_pos; /* icon position for menu */
  int M_id; /* Message id */
  int V_lock = OFF;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/


static int xRef, yRef;
/* flag to know if button press(1) before button release(0)
   side effect in mouse peek pan and inspect */
static unsigned int pressed;

int V_view_event()
{
static XWindowAttributes attribs;
/* action stack for menu's control
   nextAction is used only with zoom and peek because it's necessary
   to click the V_icon and then moving to the screen */
static int action = NOP;
static int nextAction = NOP;
static int x1, y1, x2, y2, xtmp, ytmp; /* pointer coordinates for boxes */
/* flag needed to know if it's the first move in drawing a zoompeek box
   in order to delete or not the previously drawn box */
static int V_he_moves;
/* return values fron XQueryPointer. Needed only to fill the call */
Window qpwindow;
int qpxy;
unsigned int qpmask;
register int V_i;                       /* loops */
int V_witchIcon;                        /* returns clicked icon */
static int V_fact_y = I_FACT_Y;       /* scale for scale factor calculation */
static char string[25];                 /* xy and load input values */
static int V_length;
static unsigned char kInput = NOP;
static int xOrg;
static int yOrg;
static int V_len;
char text;
KeySym key;
static int V_lockChange;
XEvent event;
short x0X = 2000, y0X = 2000, x1X = 0, y1X = 0;
static int first = 1; /* tells if it's the first time in the call */
XWindowAttributes WAttr;

  switch (V_event.type) 
  {
    case GraphicsExpose:
      /* V_refresh(); */
      V_refreshArea(V_event.xgraphicsexpose.x,
                V_event.xgraphicsexpose.y,
                V_event.xgraphicsexpose.width,
                V_event.xgraphicsexpose.height);
      break;
    case ConfigureNotify :
        XGetWindowAttributes(display, V_window, &attribs);
        V_xWin = attribs.x;
        V_yWin = attribs.y + YMENU;
        dxWin = attribs.width;
        dyWin = attribs.height;
        XFreePixmap(display, V_PixSave);
        V_PixSave = XCreatePixmap(display, V_window, dxWin, dyWin, V_depth);
        if (!V_PixSave) {
           fprintf(stderr, "Cannot create Pixmap in " __FUNCTION__ "\n");
           exit(2);
        }
        dyWin -= YMENU;
        resize(); /* action associated with this event */
        /* no break here boys, that needed so for openwin */
    case Expose:
      /* do not execute something like a :
         while (XCheckTypedWindowEvent(display, V_window, Expose, &V_event));
         because some window managers construct a window in many rectangles
         and so generate more than one expose for an overlap */
/*
      if (XCheckTypedWindowEvent(display, V_window,
                        ConfigureNotify, &event)) {
        XGetWindowAttributes(display, V_window, &attribs);
        V_xWin = attribs.x;
        V_yWin = attribs.y + YMENU;
        dxWin = attribs.width;
        dyWin = attribs.height;
        XFreePixmap(display, V_PixSave);
        V_PixSave = XCreatePixmap(display, V_window,
              (int)dxWin,(int)dyWin, 8);
        dyWin -= YMENU;
        resize();
      }
       else {
        while (XCheckTypedWindowEvent(display, V_window, Expose, &event)) {
          x0X = event.x < x0Area ? event.x : x0Area;
          y0X = event.y < y0Area ? event.y : y0Area;
          x1X = event.x + event.dx > x1X ? event.x + event.dx : x1X;
          y1X = event.y + event.dy > y1X ? event.y + event.dy : y1X;
        }
        x1X -= x0X;
        y1X -= y0X;
      }
*/
      /* redrawing the menu bar if needed */
      if (first || V_event.xexpose.y < YMENU) {
        V_setMenu();
        V_drawMenu();
      }
      /* adjusting scroll */
      if (dxWin < dyWin)
        V_pixDep =(long)((dxWin * C_Config_List.C_SCROLL) / 100 - 1);
      else
        V_pixDep =(long)((dyWin * C_Config_List.C_SCROLL) / 100 - 1);
      if (V_pixDep < 32)
        V_pixDep = 32;
      V_pixDep = (V_pixDep / 32) * 32;

      V_setClip(); /* reset clip mask in any case */
      if (action != NOP) 
        V_drawIcon(action, ON);
      else if (nextAction != NOP)
        V_drawIcon(nextAction, ON);
      else
        action = nextAction = NOP;
      if (V_lock == 1)
        V_drawIcon(I_LOCK, ON);

      V_drawFactIcon((int)V_fact_y, 0);
      if (XQueryPointer(display, DefaultRootWindow(display), &qpwindow, &qpwindow,
                    &qpxy, &qpxy, &x2, &y2, &qpmask))
        V_drawCoords((int)x2,(int)y2);
      V_lockChange = 1;
      /* V_refresh(); */
      /* refresh what's need to be refreshed */
      /* V_refreshArea(V_event.xexpose.x, V_event.xexpose.y,
                V_event.xexpose.width, V_event.xexpose.height); */
      V_refreshArea(V_event.xexpose.x, V_event.xexpose.y,
                V_event.xexpose.width, V_event.xexpose.height);
      first = 0;
      break;

    case MappingNotify:
      XRefreshKeyboardMapping(&V_event.xmapping);
      break;

    case ButtonPress:

      XGetWindowAttributes( display, V_window, &WAttr );

      if ( WAttr.map_state == IsViewable )
      {
        XSetInputFocus(display, V_window, RevertToNone, CurrentTime);
      }

      if (V_event.xbutton.button == 1) {       /* if button one ... */
        pressed = 1;
        xtmp = ytmp = 0;
        x1 = V_event.xbutton.x;
        y1 = V_event.xbutton.y;
        V_witchIcon = V_clickedIcon((int)x1,(int)y1);
        /* if not inside the screen, unselect previously selected event
           only peek, zoom, inspect and pan icons can be in this state */
        if (V_witchIcon != INSCREEN) {
          if (nextAction == I_MOUSE) {
            V_drawIcon(I_MOUSE, OFF);
            action = NOP;
          }
          if (nextAction == I_PEEK) {
            V_drawIcon(I_PEEK, OFF);
            action = NOP;
          }
          if (nextAction == I_PAN) {
            V_drawIcon(I_PAN, OFF);
            action = NOP;
          }
          if (nextAction == I_INSPECT) {
            V_drawIcon(I_INSPECT, OFF);
            action = NOP;
          }
        }
        if (kInput != NOP) {
          V_drawXY(OFF);
          V_drawCoords((int)x1,(int)y1);
        }
          
        switch (V_witchIcon) {
          case INSCREEN:
            if ((action == I_MOUSE)
              ||(action == I_PEEK)
              ||(action == I_INSPECT)
              ||(action == I_PAN)) {
              if (action == I_PAN) {
                XDrawLine(display, V_window, V_gcCao[GC_FGND],
                        (int) x1 - 5,(int) y1,
                        (int) x1 + 5,(int) y1);
                XDrawLine(display, V_window, V_gcCao[GC_FGND],
                        (int) x1,(int) y1 - 5,
                        (int) x1,(int) y1 + 5);
               }
              /* preparing the zoompeek box or pan line */
              x2 = y2 = 0;
              V_he_moves = 0;
            } 
            V_drawPointer(x1,y1);
            break;
  
          case I_MOUSE:
            if (nextAction == I_MOUSE) {
              /* double click the zoom icon should abord and unselect */
              action = NOP;
              nextAction = NOP;
              V_drawIcon(I_MOUSE, OFF);
              break;
            }
            V_drawIcon(I_MOUSE, ON);
            nextAction = I_MOUSE;
            action = NOP;
            break;

          case I_FIT:
            action = I_FIT;
            nextAction = NOP;
            V_drawIcon(I_FIT, ON);
            break;
  
          case I_LEFT:
            V_drawSC(ON);
            V_coverIcon(I_LEFT);
            action = I_LEFT;
            break;
  
          case I_UP:
            V_drawSC(ON);
            V_coverIcon(I_UP);
            action = I_UP;
            break;
  
          case I_RIGHT:
            V_drawSC(ON);
            V_coverIcon(I_RIGHT);
            action = I_RIGHT;
            break;
  
          case I_DOWN:
            V_drawSC(ON);
            V_coverIcon(I_DOWN);
            action = I_DOWN;
            break;
  
          case I_UNDOMOUSE:
            action = I_UNDOMOUSE;
            nextAction = NOP;
            V_drawIcon(I_UNDOMOUSE, ON);
            break;
  
          case I_PEEK:
            if (nextAction == I_PEEK) {
              /* double click the peek icon should abord and unselect */
              action = NOP;
              nextAction = NOP;
              V_drawIcon(I_PEEK, OFF);
              break;
            }
            V_drawIcon(I_PEEK, ON);
            nextAction = I_PEEK;
            action = NOP;
            break;
  
          case I_PAN:
            if (nextAction == I_PAN) {
              /* double click the pan icon should abord and unselect */
              action = NOP;
              nextAction = NOP;
              V_drawIcon(I_PAN, OFF);
              break;
            }
            V_drawIcon(I_PAN, ON);
            nextAction = I_PAN;
            action = NOP;
            break;
  
          case I_UNPEEK:
            action = I_UNPEEK;
            nextAction = NOP;
            V_drawIcon(I_UNPEEK, ON);
            break;
  
          case I_INSPECT:
            if (nextAction == I_INSPECT) {
              /* double click inspect icon should abord and unselect */
              action = NOP;
              nextAction = NOP;
              V_drawIcon(I_INSPECT, OFF);
              break;
            }
            V_drawIcon(I_INSPECT, ON);
            nextAction = I_INSPECT;
            action = NOP;
            break;
  
          case I_PLUS:
            action = I_PLUS;
            nextAction = NOP;
            V_drawIcon(I_PLUS, ON);
            break;
      
          case I_MINUS:
            action = I_MINUS;
            nextAction = NOP;
            V_drawIcon(I_MINUS, ON);
            break;
  
          case I_SETUP:
            action = I_SETUP;
            nextAction = NOP;
            V_drawIcon(I_SETUP, ON);
            break;
      
          case I_LOAD:
            action = I_LOAD;
            nextAction = NOP;
            V_drawIcon(I_LOAD, ON);
            break;
      
          case I_MAP:
            action = I_MAP;
            nextAction = NOP;
            V_drawIcon(I_MAP, ON);
            break;
  
          case I_LOCK:
            action = I_LOCK;
            nextAction = NOP;
            V_drawIcon(I_LOCK, ON);
            break;
  
          case I_FACT:
            action = I_FACT;
            nextAction = NOP;
            V_drawFactIcon((int)V_fact_y, 1);
            break;
      
          case I_XY:
            kInput = I_XY;
            action = NOP;
            nextAction = NOP;
            V_drawXY(ON);
            XDrawImageString(display, V_window, C_gcIconText,
                        V_icon_pos[I_XY].x + 2, 
                        V_icon_pos[I_XY].y + 12, 
                        "X:_", 3);
            XDrawImageString(display, V_window, C_gcIconText,
                        V_icon_pos[I_XY].x + 2, 
                        V_icon_pos[I_XY].y + 18 + 12, 
                        "Y:", 2);
            *string = '\0';
            break;
      
          default:
            action = NOP;
            nextAction = NOP;
        }
      }
      break;

    case LeaveNotify:
      V_drawPointer(0,0);
      break;

    case MotionNotify:
      /* actually coords are always refreshed */
      if (!XQueryPointer(display, V_window,
        &qpwindow, &qpwindow,
        &qpxy, &qpxy,
        &x2, &y2, &qpmask))
        break;
      V_drawPointer(x2,y2);
      if ((y2 > YMENU) &&(kInput != I_XY))
        V_drawCoords(x2, y2);
      if ((pressed == 1) &&(V_event.xmotion.state == Button1Mask))
        if ((action == I_MOUSE)
          ||(action == I_PEEK)
          ||(action == I_INSPECT)
          ||(action == I_PAN)
          ||(action == I_FACT)) {
          while (XCheckMaskEvent(display, Button1MotionMask, &V_event));
          /* getting the actual position */
          if (!XQueryPointer(display, V_window,
            &qpwindow, &qpwindow,
            &qpxy, &qpxy,
            &x2, &y2, &qpmask))
            break;
          /* line or box to be drawn ? */
          if ((action == I_MOUSE)
            ||(action == I_INSPECT)
            ||(action == I_PEEK)) {
            if (V_he_moves != 0)
              V_drawZoomBox(V_gcBox, x1, y1, xtmp, ytmp);
            V_drawZoomBox(V_gcBox, x1, y1, x2, y2);
            V_he_moves = 1;
            xtmp = x2;
            ytmp = y2;
          }
          else if (action == I_PAN) {
            if (V_he_moves != 0)
              XDrawLine(display, V_window, V_gcBox,
                      (int) x1,(int) y1,
                      (int) xtmp,(int) ytmp);
            XDrawLine(display, V_window, V_gcBox,
                    (int) x1,(int) y1,
                    (int) x2,(int) y2);
            V_he_moves = 1;
            xtmp = x2;
            ytmp = y2;
          }
          else if (action == I_FACT) {
            if (V_he_moves != 0)
              V_drawFactIcon(V_fact_y, 1);
            V_fact_y += y2 - y1 - ytmp;
            if ((V_fact_y < I_FACT_MAX) &&(V_fact_y > I_FACT_MIN)) {
              V_drawFactIcon(V_fact_y, 1);
              V_he_moves = 1;
              V_icon_pos[I_FACT].y = V_fact_y;
              ytmp = y2 - y1;
            } else {
              V_fact_y = V_icon_pos[I_FACT].y;
              V_drawFactIcon(V_fact_y, 1);
            }
          }
        }
      break;

    case ButtonRelease:
      /* if button one ... */
      if ((pressed == 1) &&(V_event.xbutton.button == 1)) 
      {
        pressed = 0;
        switch (action) {
          case I_MOUSE:
            if (V_he_moves == 1) {
              /* cheating with parameters to erase the cross */
              V_drawPointer(0,0);
              /* pointer has been moved
                 clearing the box and getting the new scale */
              V_drawZoomBox(V_gcBox, x1, y1, x2, y2);
              if (((x1 - x2) == 0) ||((y1 - y2) == 0)) {
                V_drawIcon(I_MOUSE, OFF);
                nextAction = NOP;
                V_he_moves = 0;
                break;
              }
              V_he_moves = 0;
              /* calculating the new V_screen origin and scaling factor
                 float allowed only for small numbers
                 because of undesired side effects */
              if (zoom_mouse((long)(V_xZoom / scale),
                  (long)((dyWin + YMENU - V_yZoom) / scale),
                  (long)(V_dxZoom / scale),
                  (long)(V_dyZoom / scale)) == 0) {
                M_id = M_ZOOM;
                V_drawIcon(I_MOUSE, OFF);
                action = NOP;
                nextAction = NOP;
                return OPEN_WINDOW_MESSAGE;
              }
              /* event dependant action
                 first calculating the new coordinates
                 and then refreshing */
              V_lockChange = 1;
              V_refresh();
              /* unselecting the icon */
              V_drawIcon(I_MOUSE, OFF);
              nextAction = NOP;
            }
            break;

          case I_FIT:
            V_lockChange = 1;
            V_drawPointer(0,0);
            V_fitscreen();
            M_moveCurs();
            V_drawIcon(I_FIT, OFF);
            break;

          case I_UNDOMOUSE:
            V_lockChange = 1;
            V_drawPointer(0,0);
            loom_mouse();
            V_refresh();
            M_moveCurs();
            V_drawIcon(I_UNDOMOUSE, OFF);
            break;

          case I_PEEK:
            V_lockChange = 1;
            V_drawPointer(0,0);
            V_changeCursor(0);
            if (V_he_moves == 1) {
              V_drawZoomBox(V_gcBox, x1, y1, x2, y2);
              V_he_moves = 0;
              /* action needed after a peek to be inserted here */
              if ( Peek(xscreen +(long)(V_I_pos.x / scale - 4),
                   yscreen + dyscreen +(long)(YMENU / scale - 4)
                  -(long)(((V_I_pos.y + V_I_pos.height) / scale)),
                  (long)(V_I_pos.width / scale + 8),
                  (long)(V_I_pos.height / scale + 8)) == 1)   
              {
                V_i = V_lock;
                V_lock = 1;
                V_refreshArea(V_I_pos.x, V_I_pos.y,
                          V_I_pos.width, V_I_pos.height);
                V_lock = V_i;
/*
                if (C_atomicValue == 0)
                  put_it(V_window,
                    xscreen +(long)(V_I_pos.x / scale - 4),
                    yscreen + dyscreen +(long)(YMENU / scale - 4)
                    -(long)(((V_I_pos.y + V_I_pos.height) / scale)),
                    (long)(V_I_pos.width / scale + 8),
                    (long)(V_I_pos.height / scale + 8));
                Unpeek();
*/
              }
              V_drawIcon(I_PEEK, OFF);
              nextAction = NOP;
            }
            V_changeCursor(1);
            break;

          case I_INSPECT:
            V_drawPointer(0,0);
            if (V_he_moves == 1) {
              V_drawZoomBox(V_gcBox, x1, y1, x2, y2);
              V_he_moves = 0;
              V_drawIcon(I_INSPECT, OFF);
              nextAction = NOP;
              action = NOP;
              return OPEN_WINDOW_INSPECT;
            }
            break;

          case I_LEFT:
            V_lockChange = 1;
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            V_drawSC(OFF);
            V_coverIcon(I_LEFT);
            if (V_scrollH(V_pixDep) == M_SCROLL) 
              return OPEN_WINDOW_MESSAGE;
            M_moveCurs();
            break;

          case I_UP:
            V_lockChange = 1;
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            V_drawSC(OFF);
            V_coverIcon(I_UP);
            if (V_scrollV(-V_pixDep) == M_SCROLL) 
              return OPEN_WINDOW_MESSAGE;
            M_moveCurs();
            break;

          case I_RIGHT:
            V_lockChange = 1;
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            V_drawSC(OFF);
            V_coverIcon(I_RIGHT);
            if (V_scrollH(-V_pixDep) == M_SCROLL) 
              return OPEN_WINDOW_MESSAGE;
            M_moveCurs();
            break;
      
          case I_DOWN:
            V_lockChange = 1;
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            V_drawSC(OFF);
            V_coverIcon(I_DOWN);
            if (V_scrollV(V_pixDep) == M_SCROLL) 
              return OPEN_WINDOW_MESSAGE;
            M_moveCurs();
            break;
      
          case I_PAN:
            V_lockChange = 1;
            V_drawPointer(0,0);
            if (V_he_moves != 0) {
              XDrawLine(display, V_window, V_gcBox,(int) x1, 
                      (int) y1,(int) x2,(int) y2);
              V_pan(x1 - xtmp, ytmp - y1);
              V_drawIcon(I_PAN, OFF);
              nextAction = NOP;
            } else {
              V_refresh();
              V_drawIcon(I_PAN, OFF);
            }
            M_moveCurs();
            break;
      
          case I_UNPEEK:
            V_drawPointer(0,0);
            nextAction = NOP;
            V_drawIcon(I_UNPEEK, OFF);
            V_refresh();
            break;
      
          case I_PLUS:
            V_lockChange = 1;
            V_drawPointer(0,0);
            nextAction = NOP;
            if (zoom(V_fact) == 0) {
              M_id = M_ZOOM;
              V_drawIcon(I_PLUS, OFF);
              action = NOP;
              return OPEN_WINDOW_MESSAGE;
            }
            V_refresh();
            M_moveCurs();
            V_drawIcon(I_PLUS, OFF);
            break;
      
          case I_MINUS:
            V_lockChange = 1;
            V_drawPointer(0,0);
            nextAction = NOP;
            if (loom(V_fact) == 0) {
              M_id = M_LOOM;
              action = NOP;
              V_drawIcon(I_MINUS, OFF);
              return OPEN_WINDOW_MESSAGE;
            }
            V_refresh();
            M_moveCurs();
            V_drawIcon(I_MINUS, OFF);
            break;

          case I_SETUP:
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            V_drawIcon(I_SETUP, OFF);
            return OPEN_WINDOW_SETUP;
      
          case I_LOAD:
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            M_id = M_LOAD;
            V_drawIcon(I_LOAD, OFF);
            return OPEN_WINDOW_MESSAGE;
      
          case I_MAP:
            V_drawPointer(0,0);
            action = NOP;
            nextAction = NOP;
            M_id = M_MAP;
            V_drawIcon(I_MAP, OFF);
            return OPEN_WINDOW_MESSAGE;

          case I_LOCK:
            V_drawPointer(0,0);
            V_lock ^= 1;
            if (V_lock == 0)
              V_drawIcon(I_LOCK, OFF);
            else 
            if (V_lockChange == 1)
              V_refresh();
            V_lockChange = 0;
            action = NOP;
            nextAction = NOP;
            break;
      
          case I_FACT:
            V_drawPointer(0,0);
            nextAction = NOP;
            V_drawFactIcon(V_fact_y, 0);
            for (V_i = I_FACT_MIN, V_fact = 10;
                V_i < I_FACT_MAX; V_i += 5, V_fact--)
              if ((V_fact_y >= V_i) &&(V_fact_y < V_i + 5))
                break;
            break;
      
          case I_XY:
            V_drawPointer(0,0);
            nextAction = I_XY;
            break;
        }
        action = nextAction;
      }
      break;
      
    case KeyPress:
      V_i = XLookupString(&V_event.xkey, &text, 1, &key, 0);
      /* modifing the X Y inputs coordinates */
      if (kInput == I_XY) 
      {
static int getxy;

        if (getxy == 0) 
        {
          getxy = V_icon_pos[I_XY].y; 
        }
        if ( ( key == XK_Return   ) ||
             ( key == XK_Linefeed ) ||
             ( key == XK_KP_Enter ) ) 
        {
          if (getxy == V_icon_pos[I_XY].y) 
          {
            if (strlen(string) == 0)
            {
              xOrg = xscreen / 2;
            }
            else
            {
              xOrg = atoi(string);
            }
            getxy = V_icon_pos[I_XY].y + 18;
            kInput = I_XY;
            sprintf(string, "X:%d", xOrg);
            V_length = strlen(string);
            XDrawImageString(display, V_window, C_gcIconText,
                    V_icon_pos[I_XY].x + 2, V_icon_pos[I_XY].y + 12, 
                    string, V_length);
            *string = '\0';
            XDrawImageString(display, V_window, C_gcIconText,
                    V_icon_pos[I_XY].x + 2, 
                    V_icon_pos[I_XY].y + 18 + 12, "Y:_", 3);
            V_drawPointer(0,0);
          }
          else
          {
            kInput = NOP;
            getxy = V_icon_pos[I_XY].y;

            if (strlen(string) == 0)
            {
              yOrg = yscreen / 2;
            }
            else
            {
              yOrg = atoi(string);
            }

            *string = '\0';
            xy_move((int)(xOrg * RDS_LAMBDA),(yOrg * RDS_LAMBDA));
            M_moveCurs();
            V_refresh();
            XWarpPointer(display, V_window, V_window,0 ,0 ,0 ,0 , dxWin / 2,
                    (dyWin / 2) + YMENU);
            V_drawXY(OFF);
            sprintf(string, "X:%d", xOrg);
            XDrawImageString(display, V_window, C_gcIconText,
                      V_icon_pos[I_XY].x + 2, V_icon_pos[I_XY].y + 12, 
                      string, strlen(string));
            sprintf(string, "Y:%d", yOrg);
            XDrawImageString(display, V_window, C_gcIconText,
                        V_icon_pos[I_XY].x + 2, 
                        V_icon_pos[I_XY].y + 18 + 12, string, 
                        strlen(string));
            *string = '\0';
          }
          break;
        }
        else 
        if ((key >= '0') &&(key <= '9') ||(key == '-')) 
        {
          if ((V_length = strlen(string)) < 8) 
          {
            string[V_length] = text;
            string[++V_length] = '\0';
            XDrawImageString(display, V_window, C_gcIconText,
                        V_icon_pos[I_XY].x + 14,(int) getxy + 12,
                        string, V_length);
          }
          else
          {
            XBell(display, 50);
          }
        }
        else
        if ((key == XK_BackSpace) ||(key == XK_Delete)) 
        {
          if ((V_length = strlen(string)) > 0) 
          {
            string[V_length - 1] = ' ';
            XDrawImageString(display, V_window, C_gcIconText,
                        V_icon_pos[I_XY].x + 14,(int) getxy + 12, 
                        string, V_length);
            string[V_length - 1] = '\0';
          }
          else
          {
            XBell(display, 50);
          }
        }
      }
      else
      {
         switch (key) {
            case XK_Left:
            case XK_h:
               V_drawSC(ON);
               V_coverIcon(I_LEFT);
               V_lockChange = 1;
               V_drawPointer(0,0);
               if (V_scrollH(V_pixDep) == M_SCROLL) {
                  V_drawSC(OFF);
                  V_coverIcon(I_LEFT);
                  return OPEN_WINDOW_MESSAGE;
               }
               M_moveCurs();
               V_drawSC(OFF);
               V_coverIcon(I_LEFT);
               break;
            case XK_Up:
            case XK_k:
               V_drawSC(ON);
               V_coverIcon(I_UP);
               V_lockChange = 1;
               V_drawPointer(0,0);
               if (V_scrollV(-V_pixDep) == M_SCROLL) {
                  V_drawSC(OFF);
                  V_coverIcon(I_UP);
                  return OPEN_WINDOW_MESSAGE;
               }
               M_moveCurs();
               V_drawSC(OFF);
               V_coverIcon(I_UP);
               break;
            case XK_Right:
            case XK_l:
               V_drawSC(ON);
               V_coverIcon(I_RIGHT);
               V_lockChange = 1;
               V_drawPointer(0,0);
               if (V_scrollH(-V_pixDep) == M_SCROLL) {
                  V_drawSC(OFF);
                  V_coverIcon(I_RIGHT);
                  return OPEN_WINDOW_MESSAGE;
               }
               M_moveCurs();
               V_drawSC(OFF);
               V_coverIcon(I_RIGHT);
               break;
            case XK_Down:
            case XK_j:
               V_drawSC(ON);
               V_coverIcon(I_DOWN);
               V_lockChange = 1;
               V_drawPointer(0,0);
               if (V_scrollV(V_pixDep) == M_SCROLL) {
                  V_drawSC(OFF);
                  V_coverIcon(I_DOWN);
                  return OPEN_WINDOW_MESSAGE;
               }
               M_moveCurs();
               V_drawSC(OFF);
               V_coverIcon(I_DOWN);
               break;
         }
      }
  }
  return NO_WINDOW_ACTION;
}

void V_drawZoomBox(gc, xb1, yb1, xb2, yb2)
GC gc;
int xb1, yb1, xb2, yb2;
{
int x, y, dx, dy;

  if (xb1 < xb2) {
    x = xb1;
    dx = xb2 - xb1;
  } else {
    x = xb2;
    dx = xb1 - xb2;
  }
  if (yb1 < yb2) {
    V_yZoom = yb2;
    y = yb1;
    dy = yb2 - yb1;
  } else {
    V_yZoom = yb1;
    y = yb2;
    dy = yb1 - yb2;
  }
  /* calculating the new origin coordinates and scale factor.*/
  V_xZoom = x;
  if ((dx == 0) ||(dy == 0)) {
    V_dxZoom = dxWin;
    V_dyZoom = dyWin;
    return;
  }
  XDrawRectangle(display, V_window, gc,(int) x,(int) y,(int) dx,(int) dy);
  V_I_pos.x =(int) x;
  V_I_pos.y =(int) y;
  V_dxZoom = V_I_pos.width =(int) dx;
  V_dyZoom = V_I_pos.height =(int) dy;
}

void V_drawFactIcon(V_fact_y, o)
int V_fact_y;
int o;
{
static int oldV_fact_y;

  XSetFunction(display, C_gcIconText, GXcopy);
  if (oldV_fact_y != 0) {
    XSetForeground(display, C_gcIconText, V_colors[COLOR_M_BGND]);
    XFillRectangle(display, V_window, C_gcIconText, V_icon_pos[I_FACT].x,
                oldV_fact_y, V_icon_pos[I_FACT].width, 
                I_FACT_DY);
    XDrawLine(display, V_window, C_gcIconText, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 1,
            oldV_fact_y + 6, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 1, 
            oldV_fact_y + 6 + 5);
    XDrawLine(display, V_window, C_gcIconText, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 2,
            oldV_fact_y + 7, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 2, 
            oldV_fact_y + 7 + 3);
    XDrawLine(display, V_window, C_gcIconText, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 3,
            oldV_fact_y + 8, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 4, 
            oldV_fact_y + 8);
    XSetForeground(display, C_gcIconText, V_colors[COLOR_ICON_TEXT]);
  }
  if (o == 0) {
    V_drawVol(OFF);
    XFillRectangle(display, V_window, C_gcIcon, V_icon_pos[I_FACT].x,
              V_fact_y, V_icon_pos[I_FACT].width, I_FACT_DY);
    XDrawLine(display, V_window, C_gcIcon, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 1,
            V_fact_y + 6, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 1, 
            V_fact_y + 6 + 5);
    XDrawLine(display, V_window, C_gcIcon, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 2,
            V_fact_y + 7, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 2, 
            V_fact_y + 7 + 3);
    XDrawLine(display, V_window, C_gcIcon, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 3,
            V_fact_y + 8, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 4, 
            V_fact_y + 8);
  } else {
    V_drawVol(ON);
    XFillRectangle(display, V_window, C_gcIcon, V_icon_pos[I_FACT].x,
                V_fact_y, V_icon_pos[I_FACT].width, I_FACT_DY);
    XDrawLine(display, V_window, C_gcIconInv, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 1,
            V_fact_y + 6, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 1, 
            V_fact_y + 6 + 5);
    XDrawLine(display, V_window, C_gcIconInv, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 2,
            V_fact_y + 7, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 2, 
            V_fact_y + 7 + 3);
    XDrawLine(display, V_window, C_gcIconInv, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 3,
            V_fact_y + 8, 
            V_icon_pos[I_FACT].x + V_icon_pos[I_FACT].width + 4, 
            V_fact_y + 8);
  }
  oldV_fact_y = V_fact_y;
}

/* gives the new clipping region needed because of menu */
void V_setClip()
{
static XRectangle clipRect;
register int j;

  clipRect.x = 0;
  clipRect.y = 0;
  clipRect.width = dxWin;
  clipRect.height = dyWin;
  for (j = 0; j < GENVIEW_MAX_GC; j++)
    XSetClipRectangles(display, V_gcCao[j], 0, YMENU, &clipRect, 1, Unsorted);
}

void V_drawPointer(x,y)
int x, y;
{
static int xft,yft;
static int drawn;
int xf =(((long)xscreen +(long)(x / scale)) 
        / ( RDS_LAMBDA >> 1 ) ) * ( RDS_LAMBDA >> 1 );
int yf =(((long)yscreen +(long)((dyWin + YMENU - y) / scale + .5))
        / ( RDS_LAMBDA >> 1 ) ) * ( RDS_LAMBDA >> 1 );

  if (pressed == 1) {
     xRef = xf;
     yRef = yf;
     V_drawRelCoords(xf,yf);
  }
  xf =  (int)((xf - xscreen) * scale);
  yf =  YMENU - 1 +(int)((yscreen - yf + dyscreen) * scale);

  if (drawn == 1 && yf < YMENU) {
    XDrawLine(display, V_window, V_gcBox, xft - 5, yft, xft + 5, yft);
    XDrawLine(display, V_window, V_gcBox, xft, yft - 5, xft, yft + 5);
    drawn = 0;
    return;
  }
  if (yf < YMENU) return;
  if (drawn == 0) {
    XDrawLine(display, V_window, V_gcBox, xf - 5, yf, xf + 5, yf);
    XDrawLine(display, V_window, V_gcBox, xf, yf - 5, xf, yf + 5);
    drawn = 1;
    xft = xf; yft = yf;
    return;
  }
  XDrawLine(display, V_window, V_gcBox, xft - 5, yft, xft + 5, yft);
  XDrawLine(display, V_window, V_gcBox, xft, yft - 5, xft, yft + 5);
  XDrawLine(display, V_window, V_gcBox, xf - 5, yf, xf + 5, yf);
  XDrawLine(display, V_window, V_gcBox, xf, yf - 5, xf, yf + 5);
  xft = xf; yft = yf;
}

void V_fitscreen()
{
  fit();
  V_refresh();
}

void V_pan(dx, dy)
int dx, dy;
{
  pan((long)(dx / scale),(long)(dy / scale));
  V_refresh();
}

void V_drawCoords(x, y)
int x, y;
{
register int i;
char c[20];
int xf =(((long)xscreen +(long)(x / scale))
        / ( RDS_LAMBDA >> 1 ) ) * ( RDS_LAMBDA >> 1 );
int yf =(((long)yscreen +(long)((dyWin + YMENU - y) / scale + .5))
        / ( RDS_LAMBDA >> 1 ) ) * ( RDS_LAMBDA >> 1 );

  sprintf(c, "%.1f ",(float)(xf) / RDS_LAMBDA );
  for (i = strlen(c); i < 10; c[i] = ' ', i++)
    c[i] = '\0';
  XDrawImageString(display, V_window, C_gcIconText,
              V_icon_pos[I_XY].x + 14, V_icon_pos[I_XY].y + 12, c, i);
  sprintf(c, "%.1f ", (float)(yf) / RDS_LAMBDA );
  for (i = strlen(c); i < 10; c[i] = ' ', i++)
    c[i] = '\0';
  XDrawImageString(display, V_window, C_gcIconText,
            V_icon_pos[I_XY].x + 14, V_icon_pos[I_XY].y + 18 + 12, c, i);
  V_drawRelCoords(xf,yf);
}

void V_drawRelCoords(xf, yf)
int xf, yf;
{
register int i;
char c[20];
  sprintf(c, "%.1f",(float)( xf - xRef ) / RDS_LAMBDA );
  for (i = strlen(c); i < 8; c[i] = ' ', i++)
    c[i] = '\0';
  XDrawImageString(display, V_window, C_gcIconText,
            V_icon_pos[I_DXDY].x + 20, V_icon_pos[I_DXDY].y + 12, c, i);
  sprintf(c, "%.1f",(float)( yf - yRef ) / RDS_LAMBDA );
  for (i = strlen(c); i < 8; c[i] = ' ', i++)
    c[i] = '\0';
  XDrawImageString(display, V_window, C_gcIconText,
        V_icon_pos[I_DXDY].x + 20, V_icon_pos[I_DXDY].y + 18 + 12, c, i);
}
