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

# include <X11/Xlib.h>
# include <X11/cursorfont.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H

# include "G_parse.h"
# include "v_view.h"
# include "v_extern.h"

# include "V_colors.h"
# include "V_drv.h"

  static char GenviewLayerPatternDefine[] =
  {
  0, /* RDS_LAYER_NWELL */
  0, /* RDS_LAYER_PWELL */
  0, /* RDS_LAYER_NIMP */
  0, /* RDS_LAYER_PIMP */
  0, /* RDS_LAYER_ACTIV */
  1, /* RDS_LAYER_NDIF */
  1, /* RDS_LAYER_PDIF */
  1, /* RDS_LAYER_NTIE */
  1, /* RDS_LAYER_PTIE */
  3, /* RDS_LAYER_POLY */
  3, /* RDS_LAYER_GATE */
  3, /* RDS_LAYER_VPOLY */
  3, /* RDS_LAYER_TPOLY */
  3, /* RDS_LAYER_POLY2 */
  5, /* RDS_LAYER_CONT */
  3, /* RDS_LAYER_TPOLY2 */
  5, /* RDS_LAYER_TCONT */
  5, /* RDS_LAYER_CONT2 */
  4, /* RDS_LAYER_ALU1 */
  4, /* RDS_LAYER_VALU1 */
  4, /* RDS_LAYER_TALU1 */
  5, /* RDS_LAYER_VIA1 */
  5, /* RDS_LAYER_TVIA1 */
  6, /* RDS_LAYER_ALU2 */
  6, /* RDS_LAYER_VALU2 */
  6, /* RDS_LAYER_TALU2 */
  5, /* RDS_LAYER_VIA2 */
  5, /* RDS_LAYER_TVIA2 */
  8, /* RDS_LAYER_ALU3 */
  8, /* RDS_LAYER_VALU3 */
  8, /* RDS_LAYER_TALU3 */
  5, /* RDS_LAYER_VIA3 */
  5, /* RDS_LAYER_TVIA3 */
  9, /* RDS_LAYER_ALU4 */
  9, /* RDS_LAYER_VALU4 */
  9, /* RDS_LAYER_TALU4 */
  5, /* RDS_LAYER_VIA4 */
  5, /* RDS_LAYER_TVIA4 */
  10, /* RDS_LAYER_ALU5 */
  10, /* RDS_LAYER_VALU5 */
  10, /* RDS_LAYER_TALU5 */
  5, /* RDS_LAYER_VIA5 */
  5, /* RDS_LAYER_TVIA5 */
  11, /* RDS_LAYER_ALU6 */
  11, /* RDS_LAYER_VALU6 */
  11, /* RDS_LAYER_TALU6 */
  5, /* RDS_LAYER_VIA6 */
  5, /* RDS_LAYER_TVIA6 */
  12, /* RDS_LAYER_ALU7 */
  12, /* RDS_LAYER_VALU7 */
  12, /* RDS_LAYER_TALU7 */
  12, 5, /* RDS_LAYER_VIA7 */
  5, /* RDS_LAYER_TVIA7 */
  13, /* RDS_LAYER_ALU8 */
  13, /* RDS_LAYER_VALU8 */
  13, /* RDS_LAYER_TALU8 */
  5, /* RDS_LAYER_VIA8 */
  5, /* RDS_LAYER_TVIA8 */
  14, /* RDS_LAYER_ALU9 */
  7, /* RDS_LAYER_CPAS */
  7, /* RDS_LAYER_REF */
  0, /* RDS_LAYER_USER0 */
  0, /* RDS_LAYER_USER1 */
  0, /* RDS_LAYER_USER2 */
  0, /* RDS_LAYER_USER3 */
  0, /* RDS_LAYER_USER4 */
  0, /* RDS_LAYER_USER5 */
  0, /* RDS_LAYER_USER6 */
  0, /* RDS_LAYER_USER7 */
  0, /* RDS_LAYER_USER8 */
  0, /* RDS_LAYER_USER9 */
  0, /* RDS_LAYER_ABOX */
  12   /* FGND       */
};

  static char GenviewPatternBits[ GENVIEW_MAX_PATTERN ][ GENVIEW_PATTERN_SQUARE ] =

  {                             /* FOR SUBST */
    { 
      0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf7, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xf7, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    }
    ,                           /* FOR DIF   */
    { 
      0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb,
      0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee,
      0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee
    }
    ,                           /* FOR GATE  */
    {
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55
    }
    ,                           /* FOR POLY  */
    { 
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa
    }
    ,                           /* FOR ALU1  */
    { 
      0x88, 0x88, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11, 0x88, 0x88, 0x44, 0x44,
      0x22, 0x22, 0x11, 0x11, 0x88, 0x88, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11,
      0x88, 0x88, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11
    }
    ,                           /* FOR CONT  */
    { 
      0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff,
      0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff,
      0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff
    }
    ,                           /* FOR ALU2  */
    { 
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }
    ,                           /* FOR REF   */
    { 
      0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x11, 0x11,
      0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44,
      0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44
    }
    ,                           /* FOR ALU3  */
    {
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00
    } 
    ,                           /* FOR ALU4  */
    {
       0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
       0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
       0x11, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
    ,
    {       
      0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00
    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88
    }
    ,
    {
      0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20,
      0x40, 0x40, 0x80, 0x80, 0x01, 0x01, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08,
      0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80
    }
    ,
    {
      0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04,
      0x02, 0x02, 0x01, 0x01, 0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10,
      0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x01, 0x01
    }
    ,
    {
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    }
  };

static Pixmap GenviewPattern[ GENVIEW_MAX_PATTERN ];

static Cursor Csleep, arrow;
static char wait[] = {0x00, 0x00, 0x80, 0x03, 0xdf, 0xf7, 0x0f, 0xc1, 0xe7, 
                0xa7, 0x3b, 0x98, 0x19, 0x11, 0x0c, 0x21, 0x0c, 0x21, 
                0x0c, 0x21, 0x0c, 0x22, 0x0c, 0x24, 0x18, 0x10, 0x30, 
                0x08, 0xf0, 0x1f, 0xf8, 0x3f};


void V_createCursor()
{
/*
Pixmap pw;

  pw = XCreatePixmapFromBitmapData(display, V_window, wait, 16,16,
                      V_colors[COLOR_FGND], V_colors[COLOR_BGND],1);
  Csleep = XCreatePixmapCursor(display, pw, pw, V_colors[COLOR_FGND],
                      V_colors[COLOR_BGND], 1,1);
*/
  Csleep = XCreateFontCursor(display, XC_watch);
  arrow = XCreateFontCursor(display, XC_left_ptr);
  XDefineCursor(display, V_window, arrow);
  XDefineCursor(display, T_window, arrow);
  XDefineCursor(display, E_window, arrow);
}

void M_createCursor()
{
  XDefineCursor(display, M_window, arrow);
}

void M_changeCursor(i)
int i; 
{
  if (i)
    XDefineCursor(display, M_window, arrow);
  else
    XDefineCursor(display, M_window, Csleep);
}

void V_changeCursor(i)
int i; 
{
  
  if (i) {
    XDefineCursor(display, V_window, arrow);
    XDefineCursor(display, T_window, arrow);
    XDefineCursor(display, E_window, arrow);
  } else {
    XDefineCursor(display, V_window, Csleep);
    XDefineCursor(display, T_window, Csleep);
    XDefineCursor(display, E_window, Csleep);
  }
}
  
void V_getColors()
{
XColor def, forget_it;
int         i;
int         staticlayer;
char       *ColorName;

  for ( i = 0; i < CMAP_SIZE; i++ )
  {
    if ( i < GENVIEW_MAX_COLOR )
    {
      ColorName = V_colorName[ i ];
    }
    else
    {
      staticlayer = RDS_STATIC_LAYER[ i - GENVIEW_MAX_COLOR ];
      ColorName   = GENVIEW_RDS_LAYER_NAME_TABLE[ staticlayer ][ 1 ];
    }

    if ( ColorName == (char *)NULL )
    {
      V_colors[ i ] = V_colors[ COLOR_BGND ];
    }
    else
    {
      if (!XAllocNamedColor(display,V_cmap,ColorName,&def,&forget_it)) {
       fprintf (stderr, "colorname '%s' is not in X11 database.\nExiting in getColors.\n",
                ColorName);
        exit ( 2 );
      }
      V_colors[ i ] = def.pixel;
    }
  }
}

void V_setGc()
{
XGCValues vGC;
register int i;
int Pattern;

  /* the loop is strongly dependant of the #define in .h file, take care */

  for ( i = 0; i < GENVIEW_MAX_PATTERN; i++ )
  {
    GenviewPattern[ i ]  =

      XCreateBitmapFromData( display,
                             V_window,
                             GenviewPatternBits[ i ],
                             GENVIEW_PATTERN_WIDTH,
                             GENVIEW_PATTERN_HEIGHT );
  }

  for (i = 0; i < RDS_MAX_LAYER; i++) 
  {
    unsigned char StaticLayer = RDS_STATIC_LAYER[ i ];
    Pattern = GenviewLayerPatternDefine[ StaticLayer ];

    vGC.stipple    = GenviewPattern[ Pattern ];
    vGC.fill_style = FillStippled;
    vGC.foreground = V_colors[ i + GENVIEW_MAX_COLOR ];
    vGC.background = V_colors[ COLOR_BGND ];
    vGC.plane_mask = AllPlanes;
    vGC.font       = V_menuFont;

    V_gcCao[ i ] = XCreateGC( display, V_window,
    (GCStipple | GCForeground | GCBackground | GCFillStyle | GCPlaneMask),
    &vGC );
  }

  vGC.foreground = V_colors[ COLOR_FGND ];
  vGC.background = V_colors[ COLOR_BGND ];
  vGC.plane_mask = AllPlanes;
  vGC.font       = V_menuFont;

  V_gcCao[ GC_FGND ] = XCreateGC( display, V_window,
    (GCForeground | GCBackground | GCPlaneMask),
    &vGC );

  /* used to draw instance reference */

  vGC.fill_style = FillSolid;
  vGC.foreground = V_colors[ COLOR_RED ];
  vGC.background = V_colors[ COLOR_RED ];
  vGC.plane_mask = AllPlanes;
  vGC.font       = V_menuFont;

  V_gcCao[ GC_INSREF ] = XCreateGC( display, V_window,
    (GCForeground | GCBackground | GCPlaneMask | GCFillStyle ),
    &vGC );

  /* used to draw in, peek strechable box */
  vGC.fill_style = FillSolid;
  vGC.foreground = 1;
  vGC.background = 1;
  vGC.plane_mask = 0x0F;
  V_gcBox = XCreateGC (display, V_window,
      (GCForeground | GCBackground | GCFillStyle | GCPlaneMask),
      &vGC);
  XSetFunction (display, V_gcBox, GXxor);
}

void V_getFonts ()
{
register int i;

  for (i = 0; i <= 5; i++)
    if ((V_vr[i] = XLoadFont (display, V_fontName[i])) == 0) {
      fprintf (stderr, "font %s not available from X11 database.\n\
              Exiting in getFonts!\n", V_fontName[i]);
      exit ( 4 );
    }
  if ((V_menuFont = XLoadFont (display, "6x10")) == 0) {
      fprintf (stderr, "font 6x10 not available from X11 database.\n\
            Exiting in getFonts!\n");
      exit ( 5 );
  }
}

void C_InitGc ()
{
register int i;
XGCValues vGC;
extern GC C_gcV1, C_gcV2;
extern GC C_ObjectGc[];

  C_gcBgnd = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcBgnd, V_colors[COLOR_M_BGND]);
  XSetForeground (display, C_gcBgnd, V_colors[COLOR_M_BGND]);
  XSetFont (display, C_gcBgnd, V_menuFont);

  C_gcIconInv = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcIconInv, V_colors[COLOR_ICON_INV]);
  XSetForeground (display, C_gcIconInv, V_colors[COLOR_ICON_INV]);
  XSetFont (display, C_gcIconInv, V_menuFont);

  C_gcIcon = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcIcon, V_colors[COLOR_ICON]);
  XSetForeground (display, C_gcIcon, V_colors[COLOR_ICON]);
  XSetFont (display, C_gcIcon, V_menuFont);

  C_gcIconTextInv = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcIconTextInv, V_colors[COLOR_ICON_INV]);
  XSetForeground (display, C_gcIconTextInv, V_colors[COLOR_ICON_TEXT_INV]);
  XSetFont (display, C_gcIconTextInv, V_menuFont);

  C_gcIconText = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcIconText, V_colors[COLOR_ICON]);
  XSetForeground (display, C_gcIconText, V_colors[COLOR_ICON_TEXT]);
  XSetFont (display, C_gcIconText, V_menuFont);

  C_gcText = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcText, V_colors[COLOR_M_BGND]);
  XSetForeground (display, C_gcText, V_colors[COLOR_TEXT]);
  XSetFont (display, C_gcText, V_menuFont);

  C_gcTextPop = XCreateGC (display, V_window, 0, 0);
  XSetBackground (display, C_gcTextPop, V_colors[COLOR_BGND]);
  XSetForeground (display, C_gcTextPop, V_colors[COLOR_ICON_TEXT]);
  XSetFont (display, C_gcTextPop, V_menuFont);

  C_gcV1 = XCreateGC (display, V_window, 0, 0);
  XSetForeground (display, C_gcV1, V_colors[COLOR_BUTTON1]);

  C_gcV2 = XCreateGC (display, V_window, 0, 0);
  XSetForeground(display, C_gcV2, V_colors[COLOR_BUTTON2]);

  /* Creation d'autant de GraphicsContext que d'objets a afficher */
  /* obtention d'un simili inverse video */

  for (i = 0; i < C_NB_OBJECT; i++) 
  {
    vGC.foreground = V_colors[COLOR_BGND];

    if ( i < RDS_MAX_LAYER )
    {
      vGC.background = V_colors[ i + GENVIEW_MAX_COLOR ];
    }
    else
    {
      vGC.background = V_colors[ COLOR_FGND ];
    }
   
    vGC.font = V_menuFont;
    C_ObjectGc[i] = XCreateGC (display, V_window,
      (GCForeground | GCBackground | GCFont), &vGC);
  }
}
