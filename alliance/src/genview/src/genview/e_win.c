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

# include <stdio.h>
# include <string.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_global.h"

# include "d_asm.h"
# include "v_view.h"
# include "v_extern.h"
# include "e_menu.h"
# include "m_mes_id.h"

extern GC E_gc;
extern int E_wb_x;

char E_LoadStr[50];
int mainArgc = 0;       /* define eventual main parameters */
#define MAX_ARGS 20
static char *internalArgv[MAX_ARGS] = {"GenVieW Executable"};
char **mainArgv = NULL;

/* forward declarations */
int E_event();
void E_load();
void E_Save();
void E_SetTrace();
void E_Shell();
void E_UnSetTrace();
static int actionPress();
static int actionRelease();
static void clear_break_points();
static void compile();
static void next();
static void run();
static void set_break_point();
static void start();
static void step();
static void set_parameters();
static char *genview_basename();

int E_event()
{
KeySym key;
static int icon;
static int pressed;
char text;
XWindowAttributes WAttr;

  switch (V_event.type) {
    case Expose:
      E_setMenu();
      E_drawMenu();
      E_Expose_Edit();
      E_ConfigNotify_Edit();

      XGetWindowAttributes( display, E_window, &WAttr );

      if ( WAttr.map_state == IsViewable )
      {
        XSetInputFocus(display, E_window, RevertToNone, CurrentTime);
      }

      break;
    case ButtonPress:
      pressed = 1;

      XGetWindowAttributes( display, E_window, &WAttr );

      if ( WAttr.map_state == IsViewable )
      {
        XSetInputFocus(display, E_window, RevertToNone, CurrentTime);
      }

      icon = actionPress(V_event.xbutton.x, V_event.xbutton.y);
      break;
    case ConfigureNotify:
      E_ConfigNotify_Edit();
      break;
    case ButtonRelease:
      if (!pressed)
        break;
      pressed = 0;
      if (actionRelease(icon) == OPEN_WINDOW_MESSAGE)
        return OPEN_WINDOW_MESSAGE;
      break;
    case KeyPress:
      (void)XLookupString(&V_event.xkey, &text, 1, &key, 0);
      E_KeyPress(key);
      break;
    case KeyRelease:
      (void)XLookupString(&V_event.xkey, &text, 1, &key, 0);
      E_KeyRelease(key);
      break;
  }
  return NO_WINDOW_ACTION;
}

static int actionPress(x, y)
int x, y;
{
int icon = E_clickedIcon(x,y);

  switch(icon) {
    case E_S_BKPT :
    case E_SETTRACE :
    case E_PARAM :
    case E_LOAD :
    case E_SAVE :
    case E_EXIT :
    case E_COMPILE :
    case E_SHELL :
    case E_C_BKPT :
    case E_START :
      E_drawIcon(icon, ON);
      break;
    case E_UNSTRACE :
      if (D_existTrace())
        D_displayTrace();
      E_drawIcon(E_UNSTRACE, ON);
      break;
    case E_RUN :
      E_drawIcon(E_RUN, ON);
      V_changeCursor(0);
      break;
    case E_STEP :
      E_drawIcon(E_STEP, ON);
      V_changeCursor(0);
      break;
    case E_NEXT :
      E_drawIcon(E_NEXT, ON);
      V_changeCursor(0);
      break;
  }
  return icon; 
}

static int actionRelease(icon)
{
  switch (icon) {
    case E_S_BKPT :
      set_break_point();
      E_drawIcon(E_S_BKPT, OFF);
      break;
    case E_SETTRACE :
      E_drawIcon(E_SETTRACE, OFF);
      M_id = M_TRACE;
      return OPEN_WINDOW_MESSAGE;
    case E_PARAM :
      M_id = M_PARAM;
      E_drawIcon(E_PARAM, OFF);
      return OPEN_WINDOW_MESSAGE;
    case E_UNSTRACE :
      if (D_existTrace())
        M_id = M_UNTRACE;
      else
        M_id = M_NO_UNTRACE;
      E_drawIcon(E_UNSTRACE, OFF);
      return OPEN_WINDOW_MESSAGE;
    case E_RUN :
      run();
      V_fitscreen();
      V_changeCursor(1);
      E_drawIcon(E_RUN, OFF);
      break;
    case E_STEP :
      step();
      V_changeCursor(1);
      E_drawIcon(E_STEP, OFF);
      break;
    case E_LOAD :
      M_id = M_ELOAD;
      E_drawIcon(E_LOAD, OFF);
      return OPEN_WINDOW_MESSAGE;
    case E_SAVE :
      M_id = M_SAVE;
      E_drawIcon(E_SAVE, OFF);
      return OPEN_WINDOW_MESSAGE;
    case E_EXIT :
      M_id = M_EXIT;
      E_drawIcon(E_EXIT, OFF);
      return OPEN_WINDOW_MESSAGE;
    case E_COMPILE :
      compile();
      E_drawIcon(E_COMPILE, OFF);
      break;
    case E_SHELL :
      M_id = M_SHELL;
      E_drawIcon(E_SHELL, OFF);
      return OPEN_WINDOW_MESSAGE;
    case E_NEXT :
      next();
      V_changeCursor(1);
      E_drawIcon(E_NEXT, OFF);
      break;
    case E_C_BKPT :
      clear_break_points();
      E_drawIcon(E_C_BKPT, OFF);
      break;
    case E_START :
      start();
      E_drawIcon(E_START, OFF);
      break;
  }
  return NO_WINDOW_ACTION; 
}

/*******************************************************************************
* actions ascociated to choosen icon. self explanatory                         *
*******************************************************************************/
void E_load(str)
char *str;
{
extern int E_NewFile;
extern int E_Modif;
extern BOOL D_FicCharge;
extern char run_mode;
char buffer[200] ;

  strcpy(E_LoadStr, str);
  strcpy(buffer, E_LoadStr);
  if(!OpenFic(buffer)) {
    sprintf(buffer, "Source file '%s' not found\n", E_LoadStr);
    T_print(buffer);
    return;
  }

  sprintf(buffer, "cp %s /usr/tmp/%d_%s 1>/dev/null 2>&1", 
        E_LoadStr, getpid(), genview_basename(E_LoadStr));
  if (system(buffer)) {
    T_print("impossible to create temporary files.\n");
    return;
  }
  run_mode = RUN;
  E_NewFile = TRUE;
  E_Modif = TRUE;
  DelAllBp();
  D_FicCharge = TRUE;
  sprintf(buffer,"Edit : %s",E_LoadStr);
  XStoreName(display, E_window, buffer);
}

static void step()
{
extern int E_Modif;
extern BOOL D_ModeTrace;
extern BOOL D_FinInterp;
extern PSYMB TabSymb;
extern char *I_Code;
extern int I_pc;
extern BOOL D_FicCharge;
extern BOOL D_FirstExec;
extern char run_mode;
char buffer[60];
int pos_main;

  if (!D_FicCharge && !E_Modif) {
    T_print("No source file loaded !\n");
    return;
  }
  D_ModeTrace = TRUE;
  run_mode = STEP;
  if (E_Modif) {
    T_print("Creating temporary files ...\n");
    if (!strcmp(E_LoadStr, ""))
      strcpy(E_LoadStr, "untitled.c");
    sprintf(buffer, "/usr/tmp/%d_%s", getpid(), genview_basename(E_LoadStr));
    E_SauveFic(buffer);
    if (!PrepInterp(buffer))
      return;
    E_Modif = FALSE;
    D_FinInterp = TRUE;
    D_FicCharge = TRUE;
  }
  if (D_FinInterp) {
    if ((pos_main = RechTabSymb("_main")) == -1) {
      T_print("No main() in the program !\n");
      return;
    }
    I_pc = TabSymb[pos_main].Pos + ((int)I_Code);
    D_FinInterp = FALSE;
    ReInitialise();
    D_mainArguments();
  }
  interp(-1);
  D_FirstExec = FALSE;
}

static void run()
{
extern int E_Modif;
extern BOOL D_ModeTrace;
extern BOOL D_FinInterp;
char buffer[60];
int pos_main;
extern PSYMB TabSymb;
extern char *I_Code;
extern int I_pc;
extern BOOL D_FicCharge;
extern BOOL D_FirstExec;
extern char run_mode;

  if (!D_FicCharge && !E_Modif) {
    T_print("No source file loaded !\n");
    return;
  }
  E_updateBp();
  D_ModeTrace = FALSE;
  run_mode = RUN;
  if (E_Modif) {
    T_print("Creating temporary files ...\n");
    if (!strcmp(E_LoadStr, ""))
      strcpy(E_LoadStr, "untitled.c");
    sprintf(buffer, "/usr/tmp/%d_%s", getpid(), genview_basename(E_LoadStr));
    E_SauveFic(buffer);
    if (!PrepInterp(buffer))
      return;
    E_Modif = FALSE;
    D_FinInterp = TRUE;
    D_FicCharge = TRUE;
  }
  if (D_FinInterp) {
    if ((pos_main = RechTabSymb("_main")) == -1) {
      T_print("No main() in the program !\n");
      return;
    }
    I_pc = TabSymb[pos_main].Pos + ((int)I_Code);
    D_FinInterp = FALSE;
    ReInitialise();
    D_mainArguments();
  }
  T_print("Interpreting the source file ...\n");
  interp(-1);
  D_FirstExec = FALSE;
}


void E_UnSetTrace(str)
char *str;
{
  D_deleteTrace(str);
  D_displayTrace();
}

void E_SetTrace(str)
char *str;
{
  D_addTrace(str);
  D_traceVariable(str);
}

void E_parameters(str)
char *str;
{
extern BOOL D_FinInterp;

  set_parameters(str);
  D_FinInterp = TRUE;
/*
  start();*/ /* we shall rerun */
}

static void set_parameters(str)
char *str;
{
char *s;
int i;

  if (mainArgv) {
    for (i = 1; i < MAX_ARGS; i++) {
      if (internalArgv[i]) {
        free((void *)internalArgv[i]);
        internalArgv[i] = NULL;
      }
    }
  }
  mainArgc = 0;
  mainArgv = internalArgv;
  if (!str || !strcmp(str, "")) {
    mainArgc = 1;
    return;
  }
  while (++mainArgc < MAX_ARGS) {
    if ((s = strchr(str, ' ')) != NULL) {
      *s = '\0';
      internalArgv[mainArgc] = mbkstrdup(str);
      *s = ' ';
      str = ++s;
    } else {
      internalArgv[mainArgc++] = mbkstrdup(str);
      break;
    }
  }
}

static void clear_break_points()
{
  clear_attribBreak();
  DelAllBp();
}

static void set_break_point()
{
  E_BreakPoint();
}

void E_Save(str)
char *str;
{
char buffer[50];
extern int E_NewFile;

  E_NewFile = FALSE;
  sprintf(buffer, "%s", str);
  E_SauveFic(buffer);
}


static void compile()
{
}


static void next()
{
extern int E_Modif;
extern BOOL D_ModeTrace;
extern BOOL D_FinInterp;
char buffer[60];
int pos_main;
extern PSYMB TabSymb;
extern char *I_Code;
extern int I_pc;
extern BOOL D_FicCharge;
extern BOOL D_FirstExec;
extern char run_mode;
#define NEXT 2

  if (!D_FicCharge && !E_Modif) {
    T_print("No source file loaded !\n");
    return;
  }
  D_ModeTrace = TRUE;
  run_mode = NEXT;
  if (E_Modif) {
    T_print("Creating temporary files ...\n");
    if (!strcmp(E_LoadStr, ""))
      strcpy(E_LoadStr, "untitled.c");
    sprintf(buffer, "/usr/tmp/%d_%s", getpid(), genview_basename(E_LoadStr));
    E_SauveFic(buffer);
    if (!PrepInterp(buffer))
      return;
    E_Modif = FALSE;
    D_FinInterp = TRUE;
    D_FicCharge = TRUE;
  }
  if (D_FinInterp) {
    if ((pos_main = RechTabSymb("_main")) == -1) {
      T_print("No main() in the program !\n");
      return;
    }
    I_pc = TabSymb[pos_main].Pos + ((int)I_Code);
    D_FinInterp = FALSE;
    ReInitialise();
    D_mainArguments();
  }
  interp(-1);
  D_FirstExec = FALSE;
}

void E_Shell(str)
char *str;
{
char buffer[200];

  if(T_execute(str) != 0) {
    sprintf(buffer,"sh failed trying to execute command '%s'\n",str);
    T_print(buffer);
  }
  T_print("\n");
}

static void start()
{
extern int I_pc;
extern PSYMB TabSymb;
extern char *I_Code;
int pos_main;
extern BOOL D_FicCharge;
extern BOOL D_ModeTrace;
extern BOOL D_FinInterp;
extern int E_Modif;

  if (!D_FicCharge && !E_Modif) {
    T_print("No source file loaded !\n");
    return;
  }
  if (D_FinInterp == TRUE) 
    return;
  pos_main = GereTabSymb("_main", -1, UNKNOWN);
  I_pc = TabSymb[pos_main].Pos + ((int)I_Code);
  D_ModeTrace = TRUE;
  interp(-1);
  ReInitialise();
  D_mainArguments();
}

static char *genview_basename(filename)
char *filename;
{
char *s;

  return (s = strrchr(filename, (int)'/')) != NULL ? s + 1 : filename;
}
