/*------------------------------------------------------------\
|                                                             |
| Tool    :                     3DD                           |
|                                                             |
| File    :                  d3d_graphic.c                    |
|                                                             |
| Author  :                 Jacomme Ludovic                   |
|                                                             |
| Date    :                   14.10.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

# include <X11/Xlib.h>
# include <X11/Xutil.h>
# include <X11/keysym.h>

# include "d3d_graphic.h"

  Display     *D3dDisplay;
  Window       D3dWindow;
  int          D3dScreen;
  XSizeHints   D3dHint;

  GC           D3dGC[ D3D_MAX_GC ];
  int          D3dColors[ D3D_MAX_COLOR ];

  Colormap     D3dColorMap;
  Font         D3dFont;

  XEvent       D3dEvent;
  int          D3dWindowDx;
  int          D3dWindowDy;

  static char *D3dColorName[ D3D_MAX_COLOR ] =
  {
    "Black",
    "White",
    "Red"
  };

#define GSIZE  10
#define CSIZE  2

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
void XschAnd(x0, y0, scale, n, cflag)
{
int gs = GSIZE * scale; 
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i;
int x = x0, y = y0 - gs/2;

   /* And shape alone */
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y, x + gs/2, y);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs, x + gs/2, y + gs);
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y, gs, gs, 90 * 64, -180 * 64);

   if ((cflag & 1) == 0)
      /* Output line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else 
   /* Output circle (for a NAND) */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   /* Input lines */
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * n, x, y + gs/2 + gs/4 * n);
   /* Circle for inversion */
   for (i = 0; i < n; i++) {
      if ((cflag & (1 << i + 1)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2,
               x,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2 - cs/2,
               cs, cs, 0 * 64, 360 * 64);
   }
}

void XschOr(x0, y0, scale, n, cflag)
{
int gs = GSIZE * scale; 
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i;
int x, y, x1, y1;

   /* Or shape alone */
   x = x0;
   y = y0 - gs/2;
   x1 = x0 - 2*gs + (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - gs;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, -30 * 64, 60 * 64);

   x1 = x0 - gs + (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - 3*gs/2;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, -90 * 64, 60 * 64);
   x1 = x0;
   y1 = y0 - gs/2;
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, x1 + (int)((double)gs * (1.0 - sqrt(3)/2)), y1);

   x1 = x0 - gs + (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - gs/2;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, 90 * 64, -60 * 64);
   x1 = x0;
   y1 = y0 + gs/2;
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, x1 + (int)((double)gs * (1.0 - sqrt(3)/2)), y1);

   if ((cflag & 1) == 0)
      /* Output line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else
      /* Output circle */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   /* Input lines */
   /* Vertical bars */
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * n,
             x, y);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs,
             x, y + gs/2 + gs/4 * n);
   /* Circle for complementation */
   for (i = 0; i < n; i++) {
      if ((cflag & (1 << i + 1)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2,
               x,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2 - cs/2,
               cs, cs, 0 * 64, 360 * 64);
   }
}

void XschXor(x0, y0, scale, n, cflag)
{
int gs = GSIZE * scale;
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i;
int x, y, x1, y1;

   /* Or shape alone */
   x = x0;
   y = y0 - gs/2;

   x1 = x0 - 2*gs + (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - gs;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, -30 * 64, 60 * 64);

   x1 = x0 - 2*gs + 2 * (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - gs;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, -30 * 64, 60 * 64);

   x1 = x0 - gs + (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - 3*gs/2;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, -90 * 64, 60 * 64);

   x1 = x0 - gs + (int)((double)gs * (1.0 - sqrt(3)/2)); 
   y1 = y0 - gs/2;
   XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x1, y1, 2*gs, 2*gs, 90 * 64, -60 * 64);

   if ((cflag & 1) == 0)
      /* Output line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else
      /* Output circle */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   /* Input lines */
   /* Vertical bars */
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * n,
             x, y);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs,
             x, y + gs/2 + gs/4 * n);
   /* Circle for complementation */
   for (i = 0; i < n; i++) {
      if ((cflag & (1 << i + 1)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2,
               x,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (n - 2) * gs/4 + i * gs/2 - cs/2,
               cs, cs, 0 * 64, 360 * 64);
   }
}

void XschBuf(x0, y0, scale, cflag)
{
int gs = GSIZE * scale;
int is = (2 * GSIZE * scale) / 3;
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i;
int x, y, x1, y1;

   x = x0;
   y = y0 - gs/2;

   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y0 - is/2,
             x, y0 + is/2);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y0 - is/2,
             x + is, y0);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y0 + is/2,
             x + is, y0);
   if ((cflag & 1) == 0)
      /* input line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x - cs, y + gs/2, x, y + gs/2);
   else
      /* Input circle */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs, y + gs/2, cs, cs, 0 * 64, 360 * 64);
   if ((cflag & 2) == 0)
      /* Output line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + is, y + gs/2, x + is + cs, y + gs/2);
   else
      /* Output circle */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + is, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);
}

void XschMux(x0, y0, scale, ns, sflag, ni, iflag)
{
int gs = GSIZE * scale; 
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i;
int x = x0, y = y0 - gs/2;

   /* Circle for inversion */
   for (i = 0; i < ns; i++) {
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs - gs/4 * ni - gs/2 * (ns - i - 2),
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs + gs/2 - gs/4 * ni);
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x - cs,
            y - cs - gs/4 * ni - gs/2 * (ns - i - 2),
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs - gs/4 * ni - gs/2 * (ns - i - 2));
      if ((sflag & (1 << i)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs - gs/ns/2 - i * gs/ns,
               y - cs + gs/2 - gs/4 * ni,
               x + gs - gs/ns/2 - i * gs/ns,
               y + gs/2 - gs/4 * ni);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs/2 + gs - gs/ns/2 - i * gs/ns,
               y - cs + gs/2 - gs/4 * ni,
               cs, cs, 0 * 64, 360 * 64);
   }

   if ((iflag & 1) == 0)
      /* Output line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else
      /* Output circle (for a NAND) */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   /* Shape */
   XDrawRectangle(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * ni, gs, gs + (ni - 2) * gs/2);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * ni, x + gs, y + gs/2);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 + gs/4 * ni, x + gs, y + gs/2);
   /* Circle for inversion */
   for (i = 0; i < ni; i++) {
      if ((iflag & (1 << i + 1)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (ni - 2) * gs/4 + i * gs/2,
               x,
               y + gs/4 - (ni - 2) * gs/4 + i * gs/2);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (ni - 2) * gs/4 + i * gs/2 - cs/2,
               cs, cs, 0 * 64, 360 * 64);
   }
}

/* pflag: presence: d q ck we set reset 
   iflag: inversion
*/
void XschReg(x0, y0, scale, pflag, iflag)
{
int gs = GSIZE * scale; 
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i, j;
int x = x0, y = y0 - gs/2;
int ns;

   if ((iflag & 1) == 0)
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x - cs, y + gs/2, x, y + gs/2);
   else
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   if ((iflag & (1 << 1)) == 0)
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   if ((iflag & (1 << 3)) == 0)
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs/2,
            y + gs,
            x + gs/2,
            y + gs + cs);
   else
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs/2 - cs/2,
            y + gs,
            cs, cs, 0 * 64, 360 * 64);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
         x + gs/2,
         y + gs + cs,
         x - cs/2,
         y + gs + cs);

   /* Input lines and shape */
   XDrawRectangle(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y, gs, gs);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x + gs/4, y + gs, x + gs/2, y + 3*gs/4);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x + gs/2, y + 3*gs/4, x + 3*gs/4, y + gs);

   ns = ((pflag & (1 << 3)) != 0) +
        ((pflag & (1 << 4)) != 0) +
        ((pflag & (1 << 5)) != 0);
   for (j = 0, i = 0; i < ns; i++) {
      if ((pflag & (1 << 3 + i)) == 0)
         j++;
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs - gs/2 * (ns - i - 2) - gs/2,
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs + gs/2 - gs/2);
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x - cs,
            y - cs - gs/2 * (ns - i - 2) - gs/2,
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs - gs/2 * (ns - i - 2) - gs/2);
      if ((iflag & (1 << i + 3 + j)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs - gs/ns/2 - i * gs/ns,
               y - cs + gs/2 - gs/2,
               x + gs - gs/ns/2 - i * gs/ns,
               y + gs/2 - gs/2);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs/2 + gs - gs/ns/2 - i * gs/ns,
               y - cs + gs/2 - gs/2,
               cs, cs, 0 * 64, 360 * 64);
   }
}

void XschMuxReg(x0, y0, scale, ns, sflag, ni, iflag, pflag, nflag)
{
int gs = GSIZE * scale; 
int cs = CSIZE * scale; /* Size of the circle when an io is complemented */
int i, j, n;
int x = x0, y = y0 - gs/2;

   /* Multiplexer part */
   for (i = 0; i < ns; i++) {
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs - gs/4 * ni - gs/2 * (ns - i - 2),
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs + gs/2 - gs/4 * ni);
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x - cs,
            y - cs - gs/4 * ni - gs/2 * (ns - i - 2),
            x + gs - gs/ns/2 - i * gs/ns,
            y - cs - gs/4 * ni - gs/2 * (ns - i - 2));
      if ((sflag & (1 << i)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs - gs/ns/2 - i * gs/ns,
               y - cs + gs/2 - gs/4 * ni,
               x + gs - gs/ns/2 - i * gs/ns,
               y + gs/2 - gs/4 * ni);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs/2 + gs - gs/ns/2 - i * gs/ns,
               y - cs + gs/2 - gs/4 * ni,
               cs, cs, 0 * 64, 360 * 64);
   }

   if ((iflag & 1) == 0)
      /* Output line : centered vertically */
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else
      /* Output circle (for a NAND) */
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   /* Shape */
   XDrawRectangle(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * ni, gs, gs + (ni - 2) * gs/2);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 - gs/4 * ni, x + gs, y + gs/2);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y + gs/2 + gs/4 * ni, x + gs, y + gs/2);
   /* Circle for inversion */
   for (i = 0; i < ni; i++) {
      if ((iflag & (1 << i + 1)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (ni - 2) * gs/4 + i * gs/2,
               x,
               y + gs/4 - (ni - 2) * gs/4 + i * gs/2);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs,
               y + gs/4 - (ni - 2) * gs/4 + i * gs/2 - cs/2,
               cs, cs, 0 * 64, 360 * 64);
   }

   /* Register part */
   x += gs + 2*cs;
   if ((nflag & 1) == 0)
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x - cs, y + gs/2, x, y + gs/2);
   else
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   if ((nflag & (1 << 1)) == 0)
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
                x + gs, y + gs/2, x + gs + cs, y + gs/2);
   else
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs, y + gs/2 - cs/2, cs, cs, 0 * 64, 360 * 64);

   if ((nflag & (1 << 3)) == 0)
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs/2,
            y + gs/2,
            x + gs/2,
            y + 3*gs/4 - gs/4 * ni + gs + (ni - 2) * gs/2);
   else {
      XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs/2 - cs/2,
            y + gs,
            cs, cs, 0 * 64, 360 * 64);
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs/2,
            y + gs + cs,
            x + gs/2,
            y + 3*gs/4 - gs/4 * ni + gs + (ni - 2) * gs/2);
   }
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
         x + gs/2,
         y + 3*gs/4 - gs/4 * ni + gs + (ni - 2) * gs/2,
         x - 2*cs - gs - cs, /* - gs -cs is the mux contribution */
         y + 3*gs/4 - gs/4 * ni + gs + (ni - 2) * gs/2);

   /* Input lines and shape */
   XDrawRectangle(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x, y, gs, gs);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x + gs/4, y + gs, x + gs/2, y + 3*gs/4);
   XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
             x + gs/2, y + 3*gs/4, x + 3*gs/4, y + gs);

   n = ((pflag & (1 << 3)) != 0) +
       ((pflag & (1 << 4)) != 0) +
       ((pflag & (1 << 5)) != 0);
   for (j = 0, i = 0; i < n; i++) {
      if ((pflag & (1 << 3 + i)) == 0)
         j++;
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs - gs/n/2 - i * gs/n,
            y - cs - gs/2 * (n - i - 2) - gs/2 - (ni * gs)/4 - (ns - 1) * gs/2,
            x + gs - gs/n/2 - i * gs/n,
            y - cs);
      XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
            x + gs - gs/n/2 - i * gs/n,
            y - cs - gs/2 * (n - i - 2) - gs/2 - (ni * gs)/4 - (ns - 1) * gs/2,
            x - 2*cs - gs - cs,
            y - cs - gs/2 * (n - i - 2) - gs/2 - (ni * gs)/4 - (ns - 1) * gs/2);
      if ((nflag & (1 << i + 3 + j)) == 0)
         XDrawLine(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x + gs - gs/n/2 - i * gs/n,
               y - cs + gs/2 - gs/2,
               x + gs - gs/n/2 - i * gs/n,
               y);
      else
         XDrawArc(D3dDisplay, D3dWindow, D3dGC[ D3D_COLOR_WHITE ],
               x - cs/2 + gs - gs/n/2 - i * gs/n,
               y - cs + gs/2 - gs/2,
               cs, cs, 0 * 64, 360 * 64);
   }
}

void D3dDisplayAllObject()
{
#if 0
   XschAnd(100, 75, 8, 2, (1 | (1<<1)));

   XschOr(200, 200, 8, 3, (1<<2));

   XschXor(300, 400, 8, 8, 1);


   XschAnd(100, 325, 8, 2, (1 | (1<<1)));
   XschReg(250, 325, 8, 0x3F, 0x3F);
#endif
   XschMuxReg(250, 325, 5, 5, 0, 5, 0, 0x3F, 0xFF);
#if 0
   XschMux(300, 300, 8, 3, 7, 8, 0xAAA);
   XschReg(500, 325, 10, 5);
#endif
}
/*------------------------------------------------------------\
|                                                             |
|                       D3dGraphicClear                       |
|                                                             |
\------------------------------------------------------------*/

void D3dGraphicClear()
{
  XClearWindow( D3dDisplay, D3dWindow );
}

/*------------------------------------------------------------\
|                                                             |
|                       D3dGraphicResize                      |
|                                                             |
\------------------------------------------------------------*/

void D3dGraphicResize()
{
  Window MyWindow;
  int    ForgetMe;

  XGetGeometry( D3dDisplay  , D3dWindow, &MyWindow, 
                &ForgetMe   , &ForgetMe,
                &D3dWindowDx, &D3dWindowDy,
                &ForgetMe   , &ForgetMe );
}

/*------------------------------------------------------------\
|                                                             |
|                       D3dGraphicLoadFonts                   |
|                                                             |
\------------------------------------------------------------*/

static void D3dGraphicLoadFonts()
{
static char fn[] = "-adobe-courier-bold-r-normal--18-180-75-75-m-110-iso8859-1";

  if ( ( D3dFont = XLoadFont( D3dDisplay, fn ) ) == 0 )
  {
    fprintf( stderr, "Font %s not available !\n", fn);

    exit( 2 );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       D3dGraphicLoadColors                  |
|                                                             |
\------------------------------------------------------------*/

static void D3dGraphicLoadColors()
{
  XColor Define;
  XColor ForgetIt;
  int    Counter;

  for ( Counter = 0; Counter < D3D_MAX_COLOR; Counter++ )
  {
    if ( ! XAllocNamedColor( D3dDisplay, D3dColorMap,
                             D3dColorName[ Counter ], &Define, &ForgetIt ) )
    {
      fprintf( stderr, "Color '%s' not available !\n", D3dColorName[ Counter ] );

      exit( 1 );
    }

    D3dColors[ Counter ] = Define.pixel;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         D3dGraphicSetGC                     |
|                                                             |
\------------------------------------------------------------*/

static void D3dGraphicSetGC()
{
  XGCValues ValueGC;
  int       Counter;

  for ( Counter = 0; Counter < D3D_MAX_COLOR; Counter++ )
  {
    ValueGC.fill_style = FillSolid;
    ValueGC.line_width = 2;
    ValueGC.foreground = D3dColors[ Counter         ];
    ValueGC.background = D3dColors[ D3D_COLOR_BLACK ];
    ValueGC.plane_mask = AllPlanes;
    ValueGC.font       = D3dFont;

    D3dGC[ Counter ] = XCreateGC( D3dDisplay, D3dWindow,
                                  ( GCForeground | GCBackground |
                                    GCFillStyle  | GCPlaneMask  |
                                    GCFont | GCLineWidth),
                                 &ValueGC );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                       D3dGraphicOpen                        |
|                                                             |
\------------------------------------------------------------*/

void D3dGraphicOpen( Argc, Argv )

  int    Argc;
  char **Argv;
{
  D3dDisplay  = XOpenDisplay( (char *)0 );
  D3dScreen   = DefaultScreen( D3dDisplay );
  D3dColorMap = DefaultColormap( D3dDisplay, D3dScreen );

  D3dHint.x      = 4;
  D3dHint.y      = 4;
  D3dHint.width  = DisplayWidth( D3dDisplay, D3dScreen ) - 20;
  D3dHint.height = DisplayHeight( D3dDisplay, D3dScreen ) - 240;
  D3dHint.flags  = PPosition | PSize | PMinSize;

  D3dHint.min_width  = 400;
  D3dHint.min_height = 224;

  D3dWindowDx = D3dHint.width;
  D3dWindowDy = D3dHint.height;

  D3dGraphicLoadColors();
  D3dGraphicLoadFonts();

  D3dWindow = XCreateSimpleWindow( D3dDisplay, 
                                   DefaultRootWindow( D3dDisplay ),
                                   D3dHint.x    , D3dHint.y,
                                   D3dHint.width, D3dHint.height, 5,
                                   D3dColors[ D3D_COLOR_WHITE ], 
                                   D3dColors[ D3D_COLOR_BLACK ] );

  XSetStandardProperties( D3dDisplay, D3dWindow, "D3D", "D3D", 
                          None, Argv, Argc, &D3dHint);
  D3dGraphicSetGC();

  XSelectInput( D3dDisplay, D3dWindow, 
                ( ButtonPressMask   | KeyPressMask      |
                  EnterWindowMask   |
                  ExposureMask      | ButtonReleaseMask | 
                  PointerMotionMask | Button1MotionMask ) );

  XMapRaised( D3dDisplay, D3dWindow );
}

/*------------------------------------------------------------\
|                                                             |
|                     D3dGraphicTreatEvent                    |
|                                                             |
\------------------------------------------------------------*/

static int D3dGraphicTreatEvent()
{
  KeySym ValueKey;
  char   CodeKey;
  int    Stop;

  Stop = 0;

  switch ( D3dEvent.type )
  {
    case Expose : 
      D3dDisplayAllObject();
    break;

    case KeyPress :

      XLookupString( &D3dEvent.xkey, &CodeKey, 1, &ValueKey, 0 );

      switch ( ValueKey )
      {
        case XK_q    : Stop = 1;

        break;
      }

      D3dGraphicClear();
      D3dDisplayAllObject();

    break;

    case EnterNotify :

      XSetInputFocus( D3dDisplay, D3dWindow, RevertToNone, CurrentTime );

    break;

    case ButtonPress :

      XSetInputFocus( D3dDisplay, D3dWindow, RevertToNone, CurrentTime );

    break;

/*\
    case MotionNotify :

      fprintf( stdout, "MotionNotify\n" );
      
    break;
\*/
  }

  return( ! Stop );
}

/*------------------------------------------------------------\
|                                                             |
|                        D3dGraphicLoop                       |
|                                                             |
\------------------------------------------------------------*/

void D3dGraphicLoop()
{
  do
  {
    XNextEvent( D3dDisplay, &D3dEvent );
  }
  while ( D3dGraphicTreatEvent() );
}

/*------------------------------------------------------------\
|                                                             |
|                    D3dGraphicClose                          |
|                                                             |
\------------------------------------------------------------*/

void D3dGraphicClose()
{
  XDestroyWindow( D3dDisplay, D3dWindow );
  XCloseDisplay( D3dDisplay );
}
