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
| File    :                   C_utils.c                       |
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

# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"
# include "G_parse.h"

# include "v_view.h"
# include "c_menu.h"
# include "v_extern.h"

# include "C_utils.h"


/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  int C_atomicValue = 0;

static FILE *C_File;
static YRectangle *C_icon_pos;
static int C_PeekValue;
static int C_ViewValue;

static char C_menu[ 12 ][ 32 ] = {
      "SAVE RAM",
      "READ RAM",
      "SAVE DISK",
      "READ DISK",
      "NO ATOMIC PEEK",
      "OK",
      "CANCEL",
      "VIEW :",
      "PEEK :",
      "% SCROLL :",
      "ALL VISIBLE",
      "ALL INVISIBLE"
};

/***************************************************************/
/***   Variables globales utilisees par les routines config  ***/
/***************************************************************/

/* Noms des objets destines a apparaitre dans la fenetre de config         */
/* La longueur de la chaine determine la taille du pave associe a  l'objet */

char *C_ObjectName[C_MAX_NB_OBJECT];

/* Flags donnant la configuration des Layers et Textes visibles ou non */
/* Ce tableau est charge par la structure C_Config lors de l'initialisation
   de la fenetre de config */

int C_ObjectVisibility[C_MAX_NB_OBJECT];

/* Valeur du pourcentage de scroll en string charge par la structure C_Config
   lors de l'initialisation de la fenetre de config */

char C_ScrollValue[32];

/* Contient la valeur provisoire avant la validation par return */

char C_NewScrollValue[32];


/* "Graphics Contexts" associes aux differents types d'affichage a l'ecran */

GC C_gcIcon,C_gcIconInv,C_gcBgnd, C_gcTextPop;
GC C_gcIconText,C_gcIconTextInv,C_gcText;
GC C_ObjectGc[C_MAX_NB_OBJECT];

struct C_Config C_Config_List;

/* Flag indiquant l'icone active */

int C_IconFlag;

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

void C_InitializeObjectName()
{
  int    layer;
  int    staticlayer;
  int    j;
  char   buffer[ 64 ];

  for ( layer = 0; layer < RDS_MAX_LAYER; layer++ )
  {
    staticlayer = RDS_STATIC_LAYER[ layer ];
    buffer[ 0 ] = '\0';

    if ( GENVIEW_RDS_LAYER_NAME_TABLE[ staticlayer ][ 0 ] != (char *)NULL )
    {
      sprintf( buffer, " %-15s",
               GENVIEW_RDS_LAYER_NAME_TABLE[ staticlayer ][ 0 ] );
    }
    
    C_ObjectName[ layer ] = mbkstrdup( buffer );
  }
  C_ObjectName[ OBJ_INSTANCE       ] = " INSTANCE       ";
  C_ObjectName[ OBJ_INSTANCE_TEXT  ] = " INSTANCETEXT   ";
  C_ObjectName[ OBJ_CONNECTOR_TEXT ] = " CONNECTORTEXT  ";
  C_ObjectName[ OBJ_SEGMENT_TEXT   ] = " SEGMENTTEXT    ";
  C_ObjectName[ OBJ_REFERENCE_TEXT ] = " REFERENCETEXT  ";
}


void C_setMenu()
{
static int first;

  if (first == 0) {
    C_icon_pos = (YRectangle *)mbkalloc(12 * sizeof(YRectangle));
    first++;
  }
  C_icon_pos[C_SR].x = C_SR_X;
  C_icon_pos[C_SR].y = C_SR_Y;
  C_icon_pos[C_SR].width = C_SR_DX;
  C_icon_pos[C_SR].height = C_SR_DY;
  C_icon_pos[C_SR].t = C_SR_X + C_SR_T;

  C_icon_pos[C_RR].x = C_RR_X;
  C_icon_pos[C_RR].y = C_RR_Y;
  C_icon_pos[C_RR].width = C_RR_DX;
  C_icon_pos[C_RR].height = C_RR_DY;
  C_icon_pos[C_RR].t = C_RR_X + C_RR_T;

  C_icon_pos[C_SD].x = C_SD_X;
  C_icon_pos[C_SD].y = C_SD_Y;
  C_icon_pos[C_SD].width = C_SD_DX;
  C_icon_pos[C_SD].height = C_SD_DY;
  C_icon_pos[C_SD].t = C_SD_X + C_SD_T;

  C_icon_pos[C_RD].x = C_RD_X;
  C_icon_pos[C_RD].y = C_RD_Y;
  C_icon_pos[C_RD].width = C_RD_DX;
  C_icon_pos[C_RD].height = C_RD_DY;
  C_icon_pos[C_RD].t = C_RD_X + C_RD_T;

  C_icon_pos[C_GEOM].x = C_GEOM_X;
  C_icon_pos[C_GEOM].y = C_GEOM_Y;
  C_icon_pos[C_GEOM].width = C_GEOM_DX;
  C_icon_pos[C_GEOM].height = C_GEOM_DY;
  C_icon_pos[C_GEOM].t = C_GEOM_X + C_GEOM_T;

  C_icon_pos[C_EX].x = C_EX_X;
  C_icon_pos[C_EX].y = C_EX_Y;
  C_icon_pos[C_EX].width = C_EX_DX;
  C_icon_pos[C_EX].height = C_EX_DY;
  C_icon_pos[C_EX].t = C_EX_X + C_EX_T;

  C_icon_pos[C_CA].x = C_CA_X;
  C_icon_pos[C_CA].y = C_CA_Y;
  C_icon_pos[C_CA].width = C_CA_DX;
  C_icon_pos[C_CA].height = C_CA_DY;
  C_icon_pos[C_CA].t = C_CA_X + C_CA_T;

  C_icon_pos[C_VI].x = C_VI_X;
  C_icon_pos[C_VI].y = C_VI_Y;
  C_icon_pos[C_VI].width = C_VI_DX;
  C_icon_pos[C_VI].height = C_VI_DY;
  C_icon_pos[C_VI].t = C_VI_X + C_VI_T;

  C_icon_pos[C_PE].x = C_PE_X;
  C_icon_pos[C_PE].y = C_PE_Y;
  C_icon_pos[C_PE].width = C_PE_DX;
  C_icon_pos[C_PE].height = C_PE_DY;
  C_icon_pos[C_PE].t = C_PE_X + C_PE_T;

  C_icon_pos[C_SC].x = C_SC_X;
  C_icon_pos[C_SC].y = C_SC_Y;
  C_icon_pos[C_SC].width = C_SC_DX;
  C_icon_pos[C_SC].height = C_SC_DY;
  C_icon_pos[C_SC].t = C_SC_X + C_SC_T;

  C_icon_pos[C_AV].x = C_AV_X;
  C_icon_pos[C_AV].y = C_AV_Y;
  C_icon_pos[C_AV].width = C_AV_DX;
  C_icon_pos[C_AV].height = C_AV_DY;
  C_icon_pos[C_AV].t = C_AV_X + C_AV_T;

  C_icon_pos[C_AI].x = C_AI_X;
  C_icon_pos[C_AI].y = C_AI_Y;
  C_icon_pos[C_AI].width = C_AI_DX;
  C_icon_pos[C_AI].height = C_AI_DY;
  C_icon_pos[C_AI].t = C_AI_X + C_AI_T;
}

void C_drawIcon(i, state)
int i, state;
{
  XFillRectangle(display, C_window, C_gcIcon, C_icon_pos[i].x,
            C_icon_pos[i].y, C_icon_pos[i].width, C_icon_pos[i].height);
  V_surround(C_window,C_icon_pos[i].x, C_icon_pos[i].y, C_icon_pos[i].width,
          C_icon_pos[i].height, state);
  XDrawImageString(display, C_window, C_gcIconText, C_icon_pos[i].t-state,
              C_icon_pos[i].y+C_DY_TXT-state,
              C_menu[i], strlen (C_menu[i]));
}

void C_setVP(str,v)
char *str;
int v;
{
  switch (v) {
    case FILL:
      if (*str == 'V')
        C_icon_pos[C_VI].t = C_VI_X + C_VI_T + 9;
      else 
        C_icon_pos[C_PE].t = C_PE_X + C_PE_T + 9;
      strcat(str, "FILL  ");
      break;
    case PATTERN:
      if (*str == 'V')
        C_icon_pos[C_VI].t = C_VI_X + C_VI_T;
      else 
        C_icon_pos[C_PE].t = C_PE_X + C_PE_T;
      strcat(str, "PATTERN");
      break;
    case OUTLINE:
      if (*str == 'V')
        C_icon_pos[C_VI].t = C_VI_X + C_VI_T;
      else 
        C_icon_pos[C_PE].t = C_PE_X + C_PE_T;
      strcat(str, "OUTLINE");
      break;
  }
}

void C_setValues()
{
  strcpy(C_menu[C_PE], "PEEK : ");
  C_setVP(C_menu[C_PE],C_PeekValue);
  strcpy(C_menu[C_VI], "VIEW : ");
  C_setVP(C_menu[C_VI], C_ViewValue);
  strcpy(C_menu[C_SC], "% SCROLL : ");
  strcat(C_menu[C_SC], C_ScrollValue);
  C_atomicValue ^= 1;
  C_atomic();
}


void C_drawMenu()
{
register int i;

  XFillRectangle(display, C_window, C_gcBgnd, 0, 0, C_W_WIDTH, C_W_HEIGHT);

  C_setValues();

  for (i = 0; i <= C_AI; i++)
    C_drawIcon(i, OFF);

  for (i = 0; i < C_NB_OBJECT; i++)
    SwitchRefresh(i);
}

int C_clickedIcon(xClick, yClick)
int xClick, yClick;
{
register int i;

   for (i = 0; i <= 11; i++)
      if (((xClick > C_icon_pos[i].x) &&
            (xClick < C_icon_pos[i].x + C_icon_pos[i].width)) &&
            ((yClick > C_icon_pos[i].y) &&
            (yClick < C_icon_pos[i].y + C_icon_pos[i].height)))
         return i;
   return C_NO_ICON; /* INSCREEN */
}


void SwitchRefresh (i)
int i;
{
register int y;

  y = C_Y_Switch + (i * C_DY_Switch);
  XFillRectangle (display, C_window, C_gcText, C_Xrect, y, C_DXrect, C_DYrect);
  XDrawImageString(display, C_window, C_ObjectGc[i], 
              C_ObjectVisibility[i] == C_SWITCH_ON ? C_Xon : C_Xoff, 
               y + C_DYstr, C_ObjectName[i], strlen (C_ObjectName[i]));
}

void C_AllVisible ()
{
register int i;

  for (i = 0; i < C_NB_OBJECT; i++) {
      C_ObjectVisibility[i] = C_SWITCH_ON;
      SwitchRefresh(i);
  }
}

void C_AllInvisible ()
{
register int i;

  for (i = 0; i < C_NB_OBJECT; i++) {
      C_ObjectVisibility[i] = C_SWITCH_OFF;
      SwitchRefresh (i);
  }
}

void C_ActionKey (car)
KeySym car;
{
register int i;

  if ((car == XK_Return) || (car == XK_Linefeed) || (car == XK_KP_Enter)) {
    if (strlen (C_NewScrollValue) < 3) {
      i = atoi (C_NewScrollValue);
      if ((i < 1) || (i > 99)) {
        XBell (display, 50);
        return;
      }
      strcpy (C_ScrollValue, C_NewScrollValue);
      strcpy (C_NewScrollValue, "");
      strcpy (C_menu[C_SC], "% SCROLL : ");
      strcat (C_menu[C_SC], C_ScrollValue);
      C_IconFlag = C_NO_ICON;
      C_drawIcon(C_SC, OFF);
      return;
    }
  }
  if ((car == XK_BackSpace) || (car == XK_Delete)) {
    i = strlen (C_NewScrollValue) - 1;
    if (i < 0) {
      i = 0;
      XBell (display, 50);
      return;
    }
    C_NewScrollValue[i] = '\0';
    strcpy (C_menu[C_SC], "NEW VALUE : ");
    strcat (C_menu[C_SC], C_NewScrollValue);
    C_drawIcon(C_SC, ON);
    return;
  }
  if (isdigit(car)) {
    if ((i = strlen (C_NewScrollValue)) < 2) {
      C_NewScrollValue[i] = car;
      C_NewScrollValue[i + 1] = '\0';
      strcpy (C_menu[C_SC], "NEW VALUE : ");
      strcat (C_menu[C_SC], C_NewScrollValue);
      C_drawIcon(C_SC, ON);
      return;
    } else {
      XBell (display, 50);
      return;
    }
  }
}


void C_WriteDisk ()
{
register int i;

  if ((C_File = fopen(".genview.stp", "w")) == NULL) {
      printf("Cannot open .genview.stp\n");
      return;
  }

  for ( i = 0; i < C_NB_OBJECT; i++)
  {
    fprintf( C_File, "%d ", C_Config_List.C_VISIBLE[i] );
  }

  fprintf(C_File, "%d %d %d",
        C_Config_List.C_SCROLL,
        C_Config_List.C_VIEW,
        C_Config_List.C_PEEK);
  fclose (C_File);
}

int C_ReadDisk ()
{
  register int i;
  int Value;

  if ((C_File = fopen(".genview.stp", "r")) == NULL) 
  {
    for (i = 0; i < C_NB_OBJECT; i++)
    {
      C_ObjectVisibility[i] = C_SWITCH_ON;
    }
    strcpy(C_ScrollValue, "25");
    C_WriteConf();
    return C_SWITCH_ON;
  }

  for ( i = 0; i < C_NB_OBJECT; i++)
  {
    fscanf( C_File, "%d ", &Value );

    C_Config_List.C_VISIBLE[i] = Value;
  }

  fscanf(C_File, "%d %d %d",
        &C_Config_List.C_SCROLL,
        &C_Config_List.C_VIEW,
        &C_Config_List.C_PEEK);
  fclose(C_File);
  return C_SWITCH_OFF;
}

void C_WriteConf()
{
  register int i;

  for ( i = 0; i < C_NB_OBJECT; i++ )
  {
    C_Config_List.C_VISIBLE[ i ] = C_ObjectVisibility[ i ];
  }

  C_Config_List.C_SCROLL = atoi (C_ScrollValue);
  C_Config_List.C_VIEW   = C_ViewValue;
  C_Config_List.C_PEEK   = C_PeekValue;

  if (dxWin < dyWin)
  {
    V_pixDep = (long) ((dxWin * C_Config_List.C_SCROLL) / 100 - 1);
  }
  else
  {
    V_pixDep = (long) ((dyWin * C_Config_List.C_SCROLL) / 100 - 1);
  }
}

void C_ReadConf()
{
  register int i;

  for (i = 0; i < C_NB_OBJECT; i++)
  {
    C_ObjectVisibility[i] = C_Config_List.C_VISIBLE[ i ];
  }

  sprintf(C_ScrollValue, "%d", C_Config_List.C_SCROLL);
  C_ViewValue = C_Config_List.C_VIEW;
  C_PeekValue = C_Config_List.C_PEEK;

  C_InitValues();

  C_setMenu();
  C_drawMenu();
}

void C_getDiskInfo ()
{
  register int i;

  if (C_ReadDisk() == C_SWITCH_OFF) 
  {
    for (i = 0; i < C_NB_OBJECT; i++)
    {
      C_ObjectVisibility[ i ] = C_Config_List.C_VISIBLE[ i ];
    }

    sprintf(C_ScrollValue, "%d", C_Config_List.C_SCROLL);
    C_ViewValue = C_Config_List.C_VIEW;
    C_PeekValue = C_Config_List.C_PEEK;
  }

  C_InitValues ();
}


void C_Peek()
{
  if (++C_PeekValue == 3)
    C_PeekValue = 0;
  strcpy(C_menu[C_PE], "PEEK : ");
  C_setVP(C_menu[C_PE],C_PeekValue);
}

void C_View()
{
  if (++C_ViewValue == 3)
    C_ViewValue = 0;
  strcpy(C_menu[C_VI], "VIEW : ");
  C_setVP(C_menu[C_VI],C_ViewValue);
}

void C_askScroll()
{
  strcpy(C_menu[C_SC], "NEW VALUE : ");
}

void C_newScroll()
{
  strcpy(C_menu[C_SC], "% SCROLL : ");
  strcat(C_menu[C_SC], C_ScrollValue);
}

void C_InitValues()
{
int i;

  for (i = 0; i < C_NB_OBJECT; i++)
  {
    if ((C_ObjectVisibility[i] != C_SWITCH_ON) &&
        (C_ObjectVisibility[i] != C_SWITCH_OFF))
      C_ObjectVisibility[i] = C_SWITCH_ON;
  }

  i = atoi (C_ScrollValue);
  if ((i < 1) || (i > 99))
    strcpy(C_ScrollValue, "25");

  if ((C_ViewValue != FILL) && (C_ViewValue != OUTLINE) &&
      (C_ViewValue != PATTERN))
    C_ViewValue = PATTERN;

  if ((C_PeekValue != FILL) && (C_PeekValue != OUTLINE) &&
      (C_PeekValue != PATTERN))
    C_PeekValue = PATTERN;
}

void C_atomic()
{
int i;

  C_atomicValue ^= 1;
  if (C_atomicValue == 1) 
  {
    strcpy(C_menu[C_GEOM], "ATOMIC PEEK");
    C_icon_pos[C_GEOM].t = C_GEOM_X + C_GEOM_T + 9;
/*\
    C_ObjectVisibility[8] = 0;
    C_ObjectVisibility[10] = 0;
    C_ObjectVisibility[17] = 0;
    C_ObjectVisibility[20] = 0;
    SwitchRefresh(8);
    SwitchRefresh(10);
    SwitchRefresh(17);
    SwitchRefresh(20);
\*/
  }
  else 
  {
    strcpy(C_menu[C_GEOM], "NO ATOMIC PEEK");
    C_icon_pos[C_GEOM].t = C_GEOM_X + C_GEOM_T;
  }
}
