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
| File    :                  genview.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   05.02.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include MUT_H
# include MPH_H

# include RDS_H
# include RPR_H
# include RFM_H

# include <stdlib.h>
# include <string.h>
# include <setjmp.h>

# include "G_global.h"
# include "G_move.h"

# include "v_view.h"
# include "v_menu.h"
# include "c_menu.h"
# include "v_extern.h"
# include "e_edit.h"
# include "e_menu.h"
# include "m_mes_id.h"
# include "V_drv.h"

# include "genview.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  static char pvb[] = 
  {
    0x7c, 0x00, 0x00, 0x38, 0x82, 0x00, 0x00, 0x44, 0x01, 0x01, 0x00, 0x44,
    0x01, 0xc1, 0x1f, 0x44, 0x01, 0x39, 0xe0, 0x38, 0x01, 0x05, 0x00, 0x01,
    0x01, 0x03, 0x00, 0x06, 0x82, 0x00, 0x00, 0x08, 0xfc, 0x00, 0x00, 0x08,
    0x40, 0x00, 0x00, 0x10, 0x20, 0x00, 0x00, 0x20, 0x20, 0x00, 0x00, 0x20,
    0x20, 0x00, 0x00, 0x20, 0x10, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x40,
    0x10, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x40,
    0x10, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x40, 0x20, 0x00, 0x00, 0x20,
    0x20, 0x00, 0x00, 0x20, 0x20, 0x00, 0x00, 0x20, 0x40, 0x00, 0x00, 0x10,
    0x80, 0x00, 0x00, 0x08, 0x9c, 0x00, 0x00, 0x38, 0x22, 0x03, 0x00, 0x46,
    0x41, 0x04, 0x00, 0x83, 0x41, 0x38, 0xe0, 0x82, 0x41, 0xc0, 0x1f, 0x82,
    0x22, 0x00, 0x00, 0x44, 0x1c, 0x00, 0x00, 0x38
  };

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

static void getparam();
static void connection();
static void usage();
static void about();

/* Now getting to the point...  */
int
  main(argc, argv)
int argc;
char **argv;
{
char hello[50];            /* filename */
XSizeHints hv, he, htrace; /* good for winman when not overstuffed */
XWMHints wmh;              /* for iconifing purposes */
int i;
extern jmp_buf sjbuf;      /* when a problem arise...  */
Pixmap gIcon;
static int args_not_ok;    /* just once, for parameters, not for signals trap */
char *layout = NULL;       /* loading a layout */
char *c_file = NULL;       /* loading a C source file */
int pid = getpid();
char fargs[6][20];
extern char *D_compilerPath;
extern char *D_compilerInclude;
extern char *E_fontname;
  
  mbkenv();                /* prepare mbk globals                             */
  rdsenv();                /* prepare rds globals                             */
  loadrdsparam();          /* load parameters from technological file         */
  GenviewLoadParameters(); /* load parameters from genview technological file */

  if ((D_compilerPath = mbkgetenv("ALLIANCE_BIN")) == NULL)
    D_compilerPath = ALLIANCE_BIN;
  if ((D_compilerInclude = mbkgetenv("ALLIANCE_INCLUDE")) == NULL)
    D_compilerInclude = ALLIANCE_INCLUDE;

  hv.min_width = 410;
  hv.min_height = YMENU + 10;
  he.min_width = 440;
  he.min_height = 64;
  htrace.min_width = 440;
  htrace.min_height = 48;

  hv.flags = he.flags = htrace.flags = PPosition | PSize | PMinSize;

  getparam(argc, argv, &hv, &he, &htrace, &layout, &c_file, &E_fontname);
  alliancebanner_with_authors("Genview", GENVIEW,
                              "procedural Generator VieWer", "1990",
                              ALLIANCE_VERSION,
                              "Frédéric Pétrot, Ludovic Jacomme and François Pécheux");
  V_getColors();         /* now choosing and allocating colors */

  strcpy(hello, "View ");   /* Creating what's necessary for watching */

  V_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
      hv.x, hv.y, hv.width, hv.height, 5,
      V_colors[COLOR_FGND], V_colors[COLOR_BGND]);

  V_PixSave = XCreatePixmap(display, V_window, hv.width, hv.height, V_depth);

  gIcon =  XCreateBitmapFromData(display, V_window, pvb, 32, 32);

  XSetStandardProperties(display, V_window, hello,
                    "Watch it", gIcon, NULL, 0, &hv);

  wmh.initial_state = NormalState;
  wmh.flags = StateHint;
  XSetWMHints(display, V_window, &wmh);
  V_xWin = hv.x;
  V_yWin = hv.y + YMENU;
  dxWin = hv.width;
  dyWin = hv.height - YMENU;

  XSelectInput(display, V_window, ButtonReleaseMask | ButtonPressMask
      | KeyPressMask | ExposureMask | Button1MotionMask | LeaveWindowMask
      | PointerMotionMask | StructureNotifyMask | VisibilityChangeMask);

  V_getFonts();
  V_setGc();
  C_InitGc();
  C_InitializeObjectName();
  C_getDiskInfo(); /* default configuration values for setup window */

  /* clearing the pixmap used for screen saving purposes */
  XSetForeground(display, V_gcCao[GC_FGND], V_colors[COLOR_BGND]);
  XFillRectangle(display, V_PixSave, V_gcCao[GC_FGND],
            0, 0, hv.width, hv.height);
  XSetForeground(display, V_gcCao[GC_FGND], V_colors[COLOR_FGND]);
  /* end of watching initialisation */

  /* Creating what's necessary for edit */
  E_Set_Font_Edit();
  strcpy(hello, "Edit ");

  E_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                        he.x, he.y, he.width, he.height, 5,
                        V_colors[COLOR_TEXT], 
                        V_colors[COLOR_M_BGND]);

  XSetStandardProperties(display, E_window, hello, "Edit", gIcon, 
                  NULL, 0, &he);
  wmh.initial_state = NormalState;
  wmh.flags = StateHint;
  XSetWMHints(display, E_window, &wmh);

  XSelectInput(display, E_window, ButtonReleaseMask | ButtonPressMask
      | StructureNotifyMask | KeyPressMask | ExposureMask);
  E_Init_Edit();
  E_maxligne =(he.height) / E_interligne;

  E_setMenu();

  /* end of edit initialisation */
  /* Creating what's necessary for trace */

  strcpy(hello, "Trace ");

  T_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
      htrace.x, htrace.y, htrace.width, htrace.height, 5,
      V_colors[COLOR_TEXT], V_colors[COLOR_M_BGND]);

  XSetStandardProperties(display, T_window, hello, "Trace", gIcon, 
                  NULL, 0, &htrace);
  wmh.initial_state = NormalState;
  wmh.flags = StateHint;
  XSetWMHints(display, T_window, &wmh);
  XSelectInput(display, T_window, ButtonReleaseMask | ButtonPressMask
      | KeyPressMask | ExposureMask);
  T_std(); /* opens standard streams for me */
  T_setMenu();
  /*  end of trace initialisation */

  I_window = C_window = M_window = 0;
  V_createCursor(); /*  declaring the home made cursors */

  /*  mapping and raising the windows */

  XMapRaised(display, V_window);
  XRaiseWindow(display, V_window);
  XMoveWindow( display, V_window, hv.x, hv.y );

  XMapRaised(display, E_window);
  XRaiseWindow(display, E_window);
  XMoveWindow( display, E_window, he.x, he.y );

  XMapRaised(display, T_window);
  XRaiseWindow(display, T_window);
  XMoveWindow( display, T_window, htrace.x, htrace.y );

  M_id = M_GENVIEW;
  M_open();

   Visu_db = addrdsfig( "_VISU_DB", GENVIEW_SIZE );
   Peek_db = addrdsfig( "_PEEK_DB", GENVIEW_SIZE );

  if (!xvisufig) {
    xscreen = yscreen = 0;
    dxscreen = dxWin;
    dyscreen = dyWin;
    scale = 1;
    zoomTrack.x = xscreen;
    zoomTrack.y = yscreen;
    zoomTrack.dx = dxscreen;
    zoomTrack.dy = dyscreen;
    zoomTrack.scale = scale;
  }


  unix_trap();  
  setjmp(sjbuf);

  if (!args_not_ok) {
    args_not_ok = 1;
    if (c_file != NULL)
      E_load(c_file);

    if (layout != NULL) {
      if (M_isOnDisk(layout)) {
        run_mode = RUN;
        Load_fig( layout );
        if (xvisufig) {
          fit();
          run_mode = STEP;
          Update_AB();
        }
      } 
    }
  }

  while (1) { /* infinite event loop */
    XNextEvent(display, &V_event);
    allEvent();
  }
}

int
  allEvent()
{
static unsigned char answer; /* answer from event requested window */

  if (V_event.xany.window == V_window) {
    if (M_window && M_id != M_MAP && V_event.type != Expose)
      return;
    answer = V_view_event();
    XFlush(display);
    if (answer == OPEN_WINDOW_SETUP)
      if (!C_window)
        C_open();
      else
        XRaiseWindow(display, C_window);
    else if (answer == OPEN_WINDOW_INSPECT) {
      if (!I_window)
         I_open();
      else {
         I_out(1);
         i_free();
         XDestroyWindow(display, I_window);
         I_window = 0;
         I_open();
       }
    } else if (answer == OPEN_WINDOW_MESSAGE) {
      if (!M_window)
        M_open();
      else {
        XDestroyWindow(display, M_window);
        M_window = 0;
        M_open();
      }
    }
  } else if (V_event.xany.window == I_window) {
    if (M_window && M_id != M_MAP && V_event.type != Expose)
      return;
    answer = I_event();
    XFlush(display);
    if (answer == CLOSE_WINDOW) {
      I_window = 0;
      answer = 0;
    }
  } else if (V_event.xany.window == C_window) {
    if (M_window && M_id != M_MAP && V_event.type != Expose)
      return;
    answer = C_event();
    XFlush(display);
    if ((answer & WINDOW_MASK) == CLOSE_WINDOW) {
      XFlush(display); /* avoid two redraws */
      C_window = 0;
      if ((answer & REFRESH_MASK) == REFRESH_ALL) {
        while (XCheckTypedWindowEvent(display, V_window, Expose, &V_event));
        V_refresh();
      }
      answer = 0;
    }
  } else if (V_event.xany.window == M_window) {
    answer = M_event();
    XFlush(display);
    if (answer == CLOSE_WINDOW) {
      M_window = 0;
      answer = 0;
    } else if (answer == CLOSE_GENVIEW) {
      unlink("var.tmp.dat");
      XFreePixmap(display, V_PixSave);
      XFreeColormap(display, V_cmap);
      XDestroyWindow(display, V_window);
      XDestroyWindow(display, T_window);
      XDestroyWindow(display, E_window);
      XDestroyWindow(display, M_window);
      XCloseDisplay(display);
      exit(0);
    } 
  } else if (V_event.xany.window == E_window) {
    if (M_window && M_id != M_MAP && V_event.type != Expose)
      return;
    answer = E_event();
    XFlush(display);
    if (answer == OPEN_WINDOW_MESSAGE) {
      if (!M_window)
        M_open();
      else {
        XDestroyWindow(display, M_window);
        M_window = 0;
        M_open();
      }
    }
  } else if (V_event.xany.window == T_window) {
    if (M_window && M_id != M_MAP && V_event.type != Expose)
      return;
    T_event();
  }
}

static void
  getparam(argc, argv, shv, she, sht, layout, source, fontname)
int argc;
char **argv;
XSizeHints *shv, *she, *sht;
char **layout, **source, **fontname;
{
int i;
char *dn = NULL, *ge = NULL, *gt = NULL, *gv = NULL, *ft = NULL;

  for (i = 1; i < argc; i++) {
  char *arg = argv[i];
    if (*arg == '-') {
      switch (*++arg) {
        case 'a':
          about();
          continue;
        case 's':
          if (++i >= argc) 
            usage(argv[0]);
          *source = argv[i];
          continue;
        case 'd':
          if (++i >= argc) 
            usage(argv[0]);
          dn = argv[i];
          continue;
        case 'l':
          if (++i >= argc) 
            usage(argv[0]);
          *layout = argv[i];
          continue;
        case 'f':
          if (++i >= argc) 
            usage(argv[0]);
          *fontname = ft = argv[i];
          continue;
        case 'g':
          if (++i >= argc) 
            usage(argv[0]);
          *++arg;
          if (*arg == 'e')
            ge = argv[i];
          else if (*arg == 't')
            gt = argv[i];
          else if (*arg == 'v')
            gv = argv[i];
          else
            usage(argv[0]);
          continue;
        default:
          usage(argv[0]);
      }
    } else
      usage(argv[0]);
  }
  connection(dn, argv[0]); /* connect with appropriate server */
  shv->x =(V_width - 1) / 2 + 2;
  shv->y = 2;
  shv->width = V_width / 2 - 10;
  shv->height = V_height - 39;
  she->x = 2;
  she->y = 2;
  she->width =(V_width - 1) / 2 - 10;
  she->height = 3 *((V_height - 1) / 4) - 33;
  sht->x = 2;
  sht->y = 3 *((V_height - 1) / 4);
  sht->width =(V_width - 1) / 2 - 10;
  sht->height =(V_height - 1) / 4 - 33;
  if (!ft)
    if ((ft = XGetDefault(display, argv[0], "font")) != NULL)
      *fontname = ft;
    else
      *fontname = "7x13";
  if (!gv) {
    if ((gv = XGetDefault(display, argv[0], "gv")) != NULL) 
      XParseGeometry(gv,&(shv->x),&(shv->y),&(shv->width),&(shv->height));
  } else
    XParseGeometry(gv,&(shv->x),&(shv->y),&(shv->width),&(shv->height));

  if (!ge) {
    if ((ge = XGetDefault(display, argv[0], "ge")) != NULL)
      XParseGeometry(ge,&(she->x),&(she->y),&(she->width),&(she->height));
  } else
    XParseGeometry(ge,&(she->x),&(she->y),&(she->width),&(she->height));

  if (!gt) {
    if ((gt = XGetDefault(display, argv[0], "gt")) != NULL)
      XParseGeometry(gt,&(sht->x),&(sht->y),&(sht->width),&(sht->height));
  } else
    XParseGeometry(gt,&(sht->x),&(sht->y),&(sht->width),&(sht->height));

  if (shv->width < shv->min_width)
    shv->width = shv->min_width;
  if (she->width < she->min_width)
    she->width = she->min_width;
  if (sht->width < sht->min_width)
    sht->width = sht->min_width;
}

static void
  connection(displayName, progName)
char *displayName, *progName;
{
  if (!(display = XOpenDisplay(displayName))) {
    fprintf(stderr, "%s: unable to connect with server '%s'\n", progName,
          XDisplayName(NULL));
    exit(9);
  }
  V_screen = DefaultScreen(display);
  V_depth = DefaultDepth(display, V_screen);
  V_width = DisplayWidth(display, V_screen);
  V_height = DisplayHeight(display, V_screen);
  V_vis = DefaultVisual(display, V_screen);
  V_class = V_vis->class;
  V_cmap = DefaultColormap(display, V_screen); /* create colormap */
#if 0
  if (installColormap) {
    V_cmap = XCopyColormapAndFree(display, V_cmap);
    XInstallColormap(display, V_cmap);
  }
#endif
}

static void
  usage(progName)
char *progName;
{
  fprintf(stderr, "usage : %s [-s source] [-l layout] [-ge <height>x<width>[{+}x{+}y]]",
              progName);
  fprintf(stderr, " [-gv <height>x<width>[{+}x{+}y]]");
  fprintf(stderr, " [-gt <height>x<width>[{+}x{+}y]]");
  fprintf(stderr, " [-fn fixed-width-fonts-for-editing]\n");
  exit(10);
}

static void
  about()
{
  fprintf(stdout, "Hello, world\n");
}
