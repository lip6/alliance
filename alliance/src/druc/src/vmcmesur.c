/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  VMCMESUR.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   11/01/94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "mph.h"
#include "mut.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include "rtl.h"
#include "rfm.h"
#include "rpr.h"

#include "drucutil.h"
#include "vmctools.h"
#include "vmcerror.h"
#include "vmcmesur.h"
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Fonctions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationSetDistance            |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationSetDistance ( DrucStructMesure,
                               Mesure1,
                               Mesure2
                             )
     DrucTypeStructMesure *DrucStructMesure;
     long                  Mesure1;
     long                  Mesure2;
{
  if ( Mesure1 > Mesure2 )
  {
    MACRO_LONG_DISTANCE    = Mesure1;
    if ( Mesure2 >= 0 )
    {
      MACRO_LARGE_DISTANCE = Mesure2;
    }
    else
    {
      MACRO_LARGE_DISTANCE = Mesure1;
    }
  }
  else
  {
    MACRO_LONG_DISTANCE    = Mesure2;
    if ( Mesure1 >= 0 )
    {
      MACRO_LARGE_DISTANCE = Mesure1;
    }
    else
    {
      MACRO_LARGE_DISTANCE = Mesure2;
    }
  }
}
/*------------------------------------------------------------\
|                                                             |
|                                                             |
|    +--+     +--------+    +--------+      +--------+        |
|    |A |     |B       |    |B       |      |       B|        |
| +--|..|--+  |    +------+ |  +--+  |  +-------+    |        |
| |  |  |  |  |    |   . A| |  |  |  |  |A  .   |    |        |
| |  +--+  |  |    +------+ +--|..|--+  +-------+    |        |
| |B       |  |        |       | A|         |        |        |
| +--------+  +--------+       +--+         +--------+        |
|   cas 7        cas 13       cas 11          cas 14          |
|                                                             |
| +---+           +-----+   +----+           +-----+          |
| |A  |           |    A|   |B   |           |    B|          |
| | ..|--+     +--|...  |   | +-----+    +-----+   |          |
| | . |  |     |  |  .  |   | |  .  |    |   . |   |          |
| +---+  |     |  +-----+   +-|...  |    |   ..|---+          |
|   |   B|     |B    |        |    A|    |A    |              |
|   +----+     +-----+        +-----+    +-----+              |
|   cas 6         cas 5        cas 9       cas 10             |
|                                                             |
|                                                             |
|    +--+     +--------+    +--------+      +--------+        |
|    |B |     |A       |    |A       |      |       A|        |
| +--|..|--+  |    +------+ |  +--+  |  +-------+    |        |
| |  |  |  |  |    |   . B| |  |  |  |  |B  .   |    |        |
| |  +--+  |  |    +------+ +--|..|--+  +-------+    |        |
| |A       |  |        |       | B|         |        |        |
| +--------+  +--------+       +--+         +--------+        |
|   cas 8        cas 2        cas 4           cas 1           |
|                                                             |
|   +---+         +---+     +---------+  +---------+          |
|   |   |         |   |     |         |  |         |          |
| +---------+  +--------+   | +-----+ |  |  +---+  |          |
| | .   . A |  |B .   . |   | |     | |  |  |   |  |          |
| +---------+  +--------+   | |B    | |  |  |A  |  |          |
|   |   |         |   |     | +-----+ |  |  +---+  |          |
|   | B |         |A  |     | A       |  |B        |          |
|   +---+         +---+     +---------+  +---------+          |
|   cas 12        cas 3        cas 0       cas 15             |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationMesure                 |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationMesure ( DrucStructMesure
                        )
     DrucTypeStructMesure *DrucStructMesure;
{
long XA          =         MACRO_RECTANGLE1->X;
long YA          =         MACRO_RECTANGLE1->Y;
long XB          =         MACRO_RECTANGLE2->X;
long YB          =         MACRO_RECTANGLE2->Y;
long DXA         =         MACRO_RECTANGLE1->DX;
long DYA         =         MACRO_RECTANGLE1->DY;
long DXB         =         MACRO_RECTANGLE2->DX;
long DYB         =         MACRO_RECTANGLE2->DY;
long FinXA       = XA    + DXA;
long FinXB       = XB    + DXB;
long FinYA       = YA    + DYA;
long FinYB       = YB    + DYB;
long ExtenXAXB   = XA    - XB;
long ExtenDXAXB  = FinXA - XB;
long ExtenDXADXB = FinXA - FinXB;
long ExtenYAYB   = YA    - YB;
long ExtenDYAYB  = FinYA - YB;
long ExtenDYADYB = FinYA - FinYB;
long ExtenXBXA   = XB    - XA;
long ExtenDXBXA  = FinXB - XA;
long ExtenYBYA   = YB    - YA;
long ExtenDYBYA  = FinYB - YA;
long ExtenXADXB  = XA    - FinXB;
long ExtenYADYB  = YA    - FinYB;
long ExtenXBDXA  = XB    - FinXA;
long ExtenYBDYA  = YB    - FinYA;
int  TmpCas;

  TmpCas = 0;
  if ( XA >= XB )
  {
    TmpCas |= 1;
  }
  if ( FinXA <= FinXB )
  {
    TmpCas |= 2;
  }
  if ( YA >= YB )
  {
    TmpCas |= 4;
  }
  if ( FinYA <= FinYB )
  {
    TmpCas |= 8;
  }
  MACRO_TOPOLOGIE = TmpCas;
/*\
printf ("\nRECT: %d\n",TmpCas);
viewrdsrec (MACRO_RECTANGLE1);
viewrdsrec (MACRO_RECTANGLE2);
printf ("\nRECT\n");
\*/
  switch ( TmpCas )
  {
  case 0  :
    { /* B inclus dans A */
      MACRO_POSX_RECT_UP  = XB; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXB; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_DO  = XB; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXB; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YB; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYB; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YB; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_ENVELOPPE;
      MACRO_SURFACE_INTER = DXB * DYB;
      if ( DXB > DYB )
      {
        MACRO_LONG_INTER  = DXB;
        MACRO_LARGE_INTER = DYB;
        if ( ExtenDYADYB  > ExtenYBYA )
        {
          MACRO_LONG_MAX  = ExtenDYADYB;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
          MACRO_LONG_MIN  = ExtenYBYA;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_DO;
        }
        else
        {
          MACRO_LONG_MIN  = ExtenDYADYB;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_UP;
          MACRO_LONG_MAX  = ExtenYBYA;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_DO;
        }
        if ( ExtenDXADXB  > ExtenXBXA )
        {
          MACRO_LARGE_MAX = ExtenDXADXB;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_RI;
          MACRO_LARGE_MIN = ExtenXBXA;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_LE;
        }
        else
        {
          MACRO_LARGE_MIN = ExtenDXADXB;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_RI;
          MACRO_LARGE_MAX = ExtenXBXA;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_LE;
        }
      }
      else
      {
        MACRO_LONG_INTER  = DYB;
        MACRO_LARGE_INTER = DXB;
        if ( ExtenDXADXB  > ExtenXBXA )
        {
          MACRO_LONG_MAX  = ExtenDXADXB;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
          MACRO_LONG_MIN  = ExtenXBXA;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_LE;
        }
        else
        {
          MACRO_LONG_MIN  = ExtenDXADXB;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_RI;
          MACRO_LONG_MAX  = ExtenXBXA;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
        }
        if ( ExtenDYADYB  > ExtenYBYA )
        {
          MACRO_LARGE_MAX = ExtenDYADYB;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_UP;
          MACRO_LARGE_MIN = ExtenYBYA;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_DO;
        }
        else
        {
          MACRO_LARGE_MIN = ExtenDYADYB;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_UP;
          MACRO_LARGE_MAX = ExtenYBYA;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_DO;
        }
      }

      if ( MACRO_LONG_MAX > MACRO_LARGE_MAX
         )
      {
        MACRO_SUP = MACRO_LONG_MAX;
        MACRO_RECT_SUP = MACRO_RECT_LONG_MAX;
      }
      else
      {
        MACRO_SUP = MACRO_LARGE_MAX;
        MACRO_RECT_SUP = MACRO_RECT_LARGE_MAX;
      }

      if ( MACRO_LONG_MIN < MACRO_LARGE_MIN
         )
      {
        MACRO_INF = MACRO_LONG_MIN;
        MACRO_RECT_INF = MACRO_RECT_LONG_MIN;
      }
      else
      {
        MACRO_INF = MACRO_LARGE_MIN;
        MACRO_RECT_INF = MACRO_RECT_LARGE_MIN;
      }
      break;
    }

  case 1  :
    { /* B penetre A par X */
      MACRO_POSX_RECT_UP  = XA; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXB; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_DO  = XA; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXB; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YB; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_MARGE;
      MACRO_SURFACE_INTER = ExtenDXBXA * DYB;
      MACRO_LONG_DISTANCE = ExtenXADXB;
      MACRO_PENETRE_INTER = ExtenDXBXA;
      MACRO_FRONT         = ExtenDXADXB;
      MACRO_RECT_FRONT    = DRUC_RECTANGLE_RI;
      if ( ExtenDXBXA     > DYB )
      {
        MACRO_LONG_INTER  = ExtenDXBXA;
        MACRO_LARGE_INTER = DYB;
        MACRO_LARGE_MIN   = ExtenDXADXB;
        MACRO_LARGE_MAX   = ExtenDXADXB;
        MACRO_RECT_LARGE_MIN    = DRUC_RECTANGLE_RI;
        MACRO_RECT_LARGE_MAX    = DRUC_RECTANGLE_RI;
        if ( ExtenDYADYB  > ExtenYBYA )
        {
          MACRO_LATER_MAX = ExtenDYADYB;
          MACRO_LATER_MIN = ExtenYBYA;
          MACRO_RECT_LATER_MAX    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MIN    = DRUC_RECTANGLE_DO;
          MACRO_LONG_MAX  = ExtenDYADYB;
          MACRO_LONG_MIN  = ExtenYBYA;
          MACRO_RECT_LONG_MAX    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LONG_MIN    = DRUC_RECTANGLE_DO;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDYADYB;
          MACRO_LATER_MAX = ExtenYBYA;
          MACRO_RECT_LATER_MIN    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MAX    = DRUC_RECTANGLE_DO;
          MACRO_LONG_MIN  = ExtenDYADYB;
          MACRO_LONG_MAX  = ExtenYBYA;
          MACRO_RECT_LONG_MIN    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LONG_MAX    = DRUC_RECTANGLE_DO;
        }
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXBXA;
        MACRO_LONG_INTER  = DYB;
        MACRO_LONG_MIN    = ExtenDXADXB;
        MACRO_LONG_MAX    = ExtenDXADXB;
        MACRO_RECT_LONG_MIN    = DRUC_RECTANGLE_RI;
        MACRO_RECT_LONG_MAX    = DRUC_RECTANGLE_RI;
        if ( ExtenDYADYB  > ExtenYBYA )
        {
          MACRO_LATER_MAX = ExtenDYADYB;
          MACRO_LATER_MIN = ExtenYBYA;
          MACRO_RECT_LATER_MAX    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MIN    = DRUC_RECTANGLE_DO;
          MACRO_LARGE_MAX = ExtenDYADYB;
          MACRO_LARGE_MIN = ExtenYBYA;
          MACRO_RECT_LARGE_MAX    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LARGE_MIN    = DRUC_RECTANGLE_DO;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDYADYB;
          MACRO_LATER_MAX = ExtenYBYA;
          MACRO_RECT_LATER_MIN    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MAX    = DRUC_RECTANGLE_DO;
          MACRO_LARGE_MIN = ExtenDYADYB;
          MACRO_LARGE_MAX = ExtenYBYA;
          MACRO_RECT_LARGE_MIN    = DRUC_RECTANGLE_UP;
          MACRO_RECT_LARGE_MAX    = DRUC_RECTANGLE_DO;
        }
      }
      if ( MACRO_FRONT > MACRO_LATER_MAX
         )
      {
        MACRO_SUP = MACRO_FRONT;
        MACRO_RECT_SUP = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_SUP = MACRO_LATER_MAX;
        MACRO_RECT_SUP = MACRO_RECT_LATER_MAX;
      }

      if ( MACRO_FRONT < MACRO_LATER_MIN
         )
      {
        MACRO_INF = MACRO_FRONT;
        MACRO_RECT_INF = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_INF = MACRO_LATER_MIN;
        MACRO_RECT_INF = MACRO_RECT_LATER_MIN;
      }
      break;
    }
  case 2  :
    { /* B penetre A par DX */
      MACRO_POSX_RECT_UP  = XB; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXA; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_DO  = XB; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXA; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YB; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_MARGE;
      MACRO_SURFACE_INTER = ExtenDXAXB * DYB;
      MACRO_LONG_DISTANCE = ExtenXBDXA;
      MACRO_PENETRE_INTER = ExtenDXAXB;
      MACRO_FRONT         = ExtenXBXA;
      MACRO_RECT_FRONT    = DRUC_RECTANGLE_LE;
      if ( ExtenDXAXB     > DYB )
      {
        MACRO_LONG_INTER  = ExtenDXAXB;
        MACRO_LARGE_INTER = DYB;
        MACRO_LARGE_MIN   = ExtenXBXA;
        MACRO_LARGE_MAX   = ExtenXBXA;
        MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_LE;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_LE;
        if ( ExtenDYADYB  > ExtenYBYA )
        {
          MACRO_LATER_MAX = ExtenDYADYB;
          MACRO_LATER_MIN = ExtenYBYA;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_DO;
          MACRO_LONG_MAX  = ExtenDYADYB;
          MACRO_LONG_MIN  = ExtenYBYA;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_UP;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDYADYB;
          MACRO_LATER_MAX = ExtenYBYA;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_DO;
          MACRO_LONG_MIN  = ExtenDYADYB;
          MACRO_LONG_MAX  = ExtenYBYA;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_UP;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
        }
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXAXB;
        MACRO_LONG_INTER  = DYB;
        MACRO_LONG_MIN    = ExtenXBXA;
        MACRO_LONG_MAX    = ExtenXBXA;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_LE;
        if ( ExtenDYADYB  > ExtenYBYA )
        {
          MACRO_LATER_MAX = ExtenDYADYB;
          MACRO_LATER_MIN = ExtenYBYA;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_DO;
          MACRO_LARGE_MAX = ExtenDYADYB;
          MACRO_LARGE_MIN = ExtenYBYA;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_UP;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_DO;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDYADYB;
          MACRO_LATER_MAX = ExtenYBYA;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_UP;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_DO;
          MACRO_LARGE_MIN = ExtenDYADYB;
          MACRO_LARGE_MAX = ExtenYBYA;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_UP;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_DO;
        }
      }
      if ( MACRO_FRONT > MACRO_LATER_MAX
         )
      {
        MACRO_SUP = MACRO_FRONT;
        MACRO_RECT_SUP = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_SUP = MACRO_LATER_MAX;
        MACRO_RECT_SUP = MACRO_RECT_LATER_MAX;
      }

      if ( MACRO_FRONT < MACRO_LATER_MIN
         )
      {
        MACRO_INF = MACRO_FRONT;
        MACRO_RECT_INF = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_INF = MACRO_LATER_MIN;
        MACRO_RECT_INF = MACRO_RECT_LATER_MIN;
      }
      break;
    }

  case 3  :
    { /* B penetre et ressort de A par X */
      MACRO_POSX_RECT_UP  = XA; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXA; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_DO  = XA; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXA; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_CROIX;
      MACRO_SURFACE_INTER = DXA * DYB;
      MACRO_PARAL_INTER   = DYB;
      MACRO_PERPEN_INTER  = DXA;
      if ( ExtenDYADYB  > ExtenYBYA )
      {
        MACRO_LONG_MAX  = ExtenDYADYB;
        MACRO_LONG_MIN  = ExtenYBYA;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_DO;
      }
      else
      {
        MACRO_LONG_MIN  = ExtenDYADYB;
        MACRO_LONG_MAX  = ExtenYBYA;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_UP;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_DO;
      }
      if ( DXA            > DYB )
      {
        MACRO_LONG_INTER  = DXA;
        MACRO_LARGE_INTER = DYB;
      }
      else
      {
        MACRO_LARGE_INTER = DXA;
        MACRO_LONG_INTER  = DYB;
      }
      break;
    }
  case 4  :
    { /* B penetre A par Y */
      MACRO_POSX_RECT_UP  = XB; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXB; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YA; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYB; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YA; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_MARGE;
      MACRO_SURFACE_INTER = ExtenDYBYA * DXB;
      MACRO_LONG_DISTANCE = ExtenYADYB;
      MACRO_PENETRE_INTER = ExtenDYBYA;
      MACRO_FRONT         = ExtenDYADYB;
      MACRO_RECT_FRONT    = DRUC_RECTANGLE_UP; 
      if ( ExtenDYBYA     > DXB )
      {
        MACRO_LONG_INTER  = ExtenDYBYA;
        MACRO_LARGE_INTER = DXB;
        MACRO_LARGE_MIN   = ExtenDYADYB;
        MACRO_LARGE_MAX   = ExtenDYADYB;
        MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_UP;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_UP;
        if ( ExtenDXADXB  > ExtenXBXA )
        {
          MACRO_LATER_MAX = ExtenDXADXB;
          MACRO_LATER_MIN = ExtenXBXA;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_LE;
          MACRO_LONG_MAX  = ExtenDXADXB;
          MACRO_LONG_MIN  = ExtenXBXA;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_LE;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDXADXB;
          MACRO_LATER_MAX = ExtenXBXA;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_LE;
          MACRO_LONG_MIN  = ExtenDXADXB;
          MACRO_LONG_MAX  = ExtenXBXA;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
        }
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDYBYA;
        MACRO_LONG_INTER  = DXB;
        MACRO_LONG_MIN    = ExtenDYADYB;
        MACRO_LONG_MAX    = ExtenDYADYB;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_UP;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
        if ( ExtenDXADXB  > ExtenXBXA )
        {
          MACRO_LATER_MAX = ExtenDXADXB;
          MACRO_LATER_MIN = ExtenXBXA;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_LE;
          MACRO_LARGE_MAX = ExtenDXADXB;
          MACRO_LARGE_MIN = ExtenXBXA;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_LE;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDXADXB;
          MACRO_LATER_MAX = ExtenXBXA;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_LE;
          MACRO_LARGE_MIN = ExtenDXADXB;
          MACRO_LARGE_MAX = ExtenXBXA;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_LE;
        }
      }
      if ( MACRO_FRONT > MACRO_LATER_MAX
         )
      {
        MACRO_SUP = MACRO_FRONT;
        MACRO_RECT_SUP = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_SUP = MACRO_LATER_MAX;
        MACRO_RECT_SUP = MACRO_RECT_LATER_MAX;
      }

      if ( MACRO_FRONT < MACRO_LATER_MIN
         )
      {
        MACRO_INF = MACRO_FRONT;
        MACRO_RECT_INF = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_INF = MACRO_LATER_MIN;
        MACRO_RECT_INF = MACRO_RECT_LATER_MIN;
      }
      break;
    }
  case 5  :
    { /* B penetre A par XY */
      MACRO_POSX_RECT_UP  = XA; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXB; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YA; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_INTERSECTION;
      MACRO_SURFACE_INTER = ExtenDXBXA * ExtenDYBYA;
      if ( ExtenDXBXA     > ExtenDYBYA )
      {
        MACRO_LONG_INTER  = ExtenDXBXA;
        MACRO_LARGE_INTER = ExtenDYBYA;
        MACRO_LONG_MAX    = ExtenDYADYB;
        MACRO_LARGE_MAX   = ExtenDXADXB;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_RI;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXBXA;
        MACRO_LONG_INTER  = ExtenDYBYA;
        MACRO_LONG_MAX    = ExtenDXADXB;
        MACRO_LARGE_MAX   = ExtenDYADYB;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_UP;
      }
      if ( ExtenDXADXB > ExtenDYADYB
         )
      {
        MACRO_SUP = ExtenDXADXB;
        MACRO_INF = ExtenDYADYB;
        MACRO_RECT_SUP = DRUC_RECTANGLE_RI;
        MACRO_RECT_INF = DRUC_RECTANGLE_UP;
      }
      else
      {
        MACRO_SUP = ExtenDYADYB;
        MACRO_INF = ExtenDXADXB;
        MACRO_RECT_SUP = DRUC_RECTANGLE_UP;
        MACRO_RECT_INF = DRUC_RECTANGLE_RI;
      }
      DrucRelationSetDistance ( DrucStructMesure,
                                ExtenXADXB,
                                ExtenYADYB
                              );
      break;
    }
  case 6  :
    { /* B penetre A par DXY */
      MACRO_POSX_RECT_UP  = XB; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXA; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YA; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_INTERSECTION;
      MACRO_SURFACE_INTER = ExtenDXAXB * ExtenDYBYA;
      if ( ExtenDXAXB     > ExtenDYBYA )
      {
        MACRO_LONG_INTER  = ExtenDXAXB;
        MACRO_LARGE_INTER = ExtenDYBYA;
        MACRO_LONG_MAX    = ExtenDYADYB;
        MACRO_LARGE_MAX   = ExtenXBXA;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_LE;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXAXB;
        MACRO_LONG_INTER  = ExtenDYBYA;
        MACRO_LONG_MAX    = ExtenXBXA;
        MACRO_LARGE_MAX   = ExtenDYADYB;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_UP;
      }
      if ( ExtenXBXA > ExtenDYADYB
         )
      {
        MACRO_SUP = ExtenXBXA;
        MACRO_INF = ExtenDYADYB;
        MACRO_RECT_SUP = DRUC_RECTANGLE_LE;
        MACRO_RECT_INF = DRUC_RECTANGLE_UP;
      }
      else
      {
        MACRO_SUP = ExtenDYADYB;
        MACRO_INF = ExtenXBXA;
        MACRO_RECT_SUP = DRUC_RECTANGLE_UP;
        MACRO_RECT_INF = DRUC_RECTANGLE_LE;
      }
      DrucRelationSetDistance ( DrucStructMesure,
                                ExtenXBDXA,
                                ExtenYADYB
                              );
      break;
    }
  case 7  :
    { /* A penetre B par DY */
      MACRO_POSX_RECT_UP  = XA; 
      MACRO_POSY_RECT_UP  = FinYB; 
      MACRO_POSDX_RECT_UP = FinXA; 
      MACRO_POSDY_RECT_UP = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_EXTENSION;
      MACRO_SURFACE_INTER = ExtenDYBYA * DXA;
      MACRO_LONG_DISTANCE = ExtenYADYB;
      MACRO_PENETRE_INTER = ExtenDYBYA;
      MACRO_LONG_MAX      = ExtenDYADYB;
      MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_UP;
      if ( ExtenDYBYA     > DXA )
      {
        MACRO_LONG_INTER  = ExtenDYBYA;
        MACRO_LARGE_INTER = DXA;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDYBYA;
        MACRO_LONG_INTER  = DXA;
      }
      break;
    }
  case 8  :
    { /* B penetre A par DY */
      MACRO_POSX_RECT_DO  = XB; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXB; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YB; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYA; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YB; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_MARGE;
      MACRO_SURFACE_INTER = ExtenDYAYB * DXB;
      MACRO_LONG_DISTANCE = ExtenYBDYA;
      MACRO_PENETRE_INTER = ExtenDYAYB;
      MACRO_FRONT         = ExtenYBYA;
      MACRO_RECT_FRONT    = DRUC_RECTANGLE_DO;
      if ( ExtenDYAYB     > DXB )
      {
        MACRO_LONG_INTER  = ExtenDYAYB;
        MACRO_LARGE_INTER = DXB;
        MACRO_LARGE_MIN   = ExtenYBYA;
        MACRO_LARGE_MAX   = ExtenYBYA;
        MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_DO;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_DO;
        if ( ExtenDXADXB  > ExtenXBXA )
        {
          MACRO_LATER_MAX = ExtenDXADXB;
          MACRO_LATER_MIN = ExtenXBXA;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_LE;
          MACRO_LONG_MAX  = ExtenDXADXB;
          MACRO_LONG_MIN  = ExtenXBXA;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_LE;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDXADXB;
          MACRO_LATER_MAX = ExtenXBXA;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_LE;
          MACRO_LONG_MIN  = ExtenDXADXB;
          MACRO_LONG_MAX  = ExtenXBXA;
          MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
        }
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDYAYB;
        MACRO_LONG_INTER  = DXB;
        MACRO_LONG_MIN    = ExtenYBYA;
        MACRO_LONG_MAX    = ExtenYBYA;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_DO;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_DO;
        if ( ExtenDXADXB  > ExtenXBXA )
        {
          MACRO_LATER_MAX = ExtenDXADXB;
          MACRO_LATER_MIN = ExtenXBXA;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_LE;
          MACRO_LARGE_MAX = ExtenDXADXB;
          MACRO_LARGE_MIN = ExtenXBXA;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_RI;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_LE;
        }
        else
        {
          MACRO_LATER_MIN = ExtenDXADXB;
          MACRO_LATER_MAX = ExtenXBXA;
          MACRO_RECT_LATER_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LATER_MAX = DRUC_RECTANGLE_LE;
          MACRO_LARGE_MIN = ExtenDXADXB;
          MACRO_LARGE_MAX = ExtenXBXA;
          MACRO_RECT_LARGE_MIN = DRUC_RECTANGLE_RI;
          MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_LE;
        }
      }
      if ( MACRO_FRONT > MACRO_LATER_MAX
         )
      {
        MACRO_SUP = MACRO_FRONT;
        MACRO_RECT_SUP = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_SUP = MACRO_LATER_MAX;
        MACRO_RECT_SUP = MACRO_RECT_LATER_MAX;
      }

      if ( MACRO_FRONT < MACRO_LATER_MIN
         )
      {
        MACRO_INF = MACRO_FRONT;
        MACRO_RECT_INF = MACRO_RECT_FRONT;
      }
      else
      {
        MACRO_INF = MACRO_LATER_MIN;
        MACRO_RECT_INF = MACRO_RECT_LATER_MIN;
      }
      break;
    }
  case 9  :
    { /* B penetre A par XDY */
      MACRO_POSX_RECT_DO  = XA; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXB; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YB; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_INTERSECTION;
      MACRO_SURFACE_INTER = ExtenDXBXA * ExtenDYAYB;
      if ( ExtenDXBXA     > ExtenDYAYB )
      {
        MACRO_LONG_INTER  = ExtenDXBXA;
        MACRO_LARGE_INTER = ExtenDYAYB;
        MACRO_LONG_MAX    = ExtenYBYA;
        MACRO_LARGE_MAX   = ExtenDXADXB;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_DO;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_RI;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXBXA;
        MACRO_LONG_INTER  = ExtenDYAYB;
        MACRO_LONG_MAX    = ExtenDXADXB;
        MACRO_LARGE_MAX   = ExtenYBYA;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_DO;
      }

      if ( ExtenDXADXB > ExtenYBYA
         )
      {
        MACRO_SUP = ExtenDXADXB;
        MACRO_INF = ExtenYBYA;
        MACRO_RECT_SUP = DRUC_RECTANGLE_RI;
        MACRO_RECT_INF = DRUC_RECTANGLE_DO;
      }
      else
      {
        MACRO_SUP = ExtenYBYA;
        MACRO_INF = ExtenDXADXB;
        MACRO_RECT_SUP = DRUC_RECTANGLE_DO;
        MACRO_RECT_INF = DRUC_RECTANGLE_RI;
      }
      DrucRelationSetDistance ( DrucStructMesure,
                                ExtenXADXB,
                                ExtenYBDYA
                              );
      break;
    }
  case 10 :
    { /* B penetre A par DXDY */
      MACRO_POSX_RECT_DO  = XB; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXA; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YB; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_INTERSECTION;
      MACRO_SURFACE_INTER = ExtenDXAXB * ExtenDYAYB;
      if ( ExtenDXAXB     > ExtenDYAYB )
      {
        MACRO_LONG_INTER  = ExtenDXAXB;
        MACRO_LARGE_INTER = ExtenDYAYB;
        MACRO_LARGE_MAX   = ExtenXBXA;
        MACRO_LONG_MAX    = ExtenYBYA;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_LE;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_DO;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXAXB;
        MACRO_LONG_INTER  = ExtenDYAYB;
        MACRO_LONG_MAX    = ExtenXBXA;
        MACRO_LARGE_MAX   = ExtenYBYA;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
        MACRO_RECT_LARGE_MAX = DRUC_RECTANGLE_DO;
      }
      if ( ExtenXBXA > ExtenYBYA
         )
      {
        MACRO_SUP = ExtenXBXA;
        MACRO_INF = ExtenYBYA;
        MACRO_RECT_SUP = DRUC_RECTANGLE_LE;
        MACRO_RECT_INF = DRUC_RECTANGLE_DO;
      }
      else
      {
        MACRO_SUP = ExtenYBYA;
        MACRO_INF = ExtenXBXA;
        MACRO_RECT_SUP = DRUC_RECTANGLE_DO;
        MACRO_RECT_INF = DRUC_RECTANGLE_LE;
      }
      DrucRelationSetDistance ( DrucStructMesure,
                                ExtenYBDYA,
                                ExtenXBDXA
                              );
      break;
    }
  case 11 :
    { /* A penetre B par Y */
      MACRO_POSX_RECT_DO  = XA; 
      MACRO_POSY_RECT_DO  = YA; 
      MACRO_POSDX_RECT_DO = FinXA; 
      MACRO_POSDY_RECT_DO = YB; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_EXTENSION;
      MACRO_SURFACE_INTER = ExtenDYAYB * DXA;
      MACRO_LONG_DISTANCE = ExtenYBDYA;
      MACRO_PENETRE_INTER = ExtenDYAYB;
      MACRO_LONG_MAX      = ExtenYBYA;
      MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_DO;
      if ( ExtenDYAYB     > DXA )
      {
        MACRO_LONG_INTER  = ExtenDYAYB;
        MACRO_LARGE_INTER = DXA;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDYAYB;
        MACRO_LONG_INTER  = DXA;
      }
      break;
    }
  case 12 :
    { /* B penetre et ressort de A par Y */
      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YA; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYA; 

      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YA; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_CROIX;
      MACRO_SURFACE_INTER = DYA * DXB;
      MACRO_PARAL_INTER   = DXB;
      MACRO_PERPEN_INTER  = DYA;
      if ( DYA            > DXB )
      {
        MACRO_LONG_INTER  = DYA;
        MACRO_LARGE_INTER = DXB;
      }
      else
      {
        MACRO_LARGE_INTER = DYA;
        MACRO_LONG_INTER  = DXB;
      }
      if ( ExtenDXADXB  > ExtenXBXA )
      {
        MACRO_LONG_MAX  = ExtenDXADXB;
        MACRO_LONG_MIN  = ExtenXBXA;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_LE;
      }
      else
      {
        MACRO_LONG_MIN  = ExtenDXADXB;
        MACRO_LONG_MAX  = ExtenXBXA;
        MACRO_RECT_LONG_MIN = DRUC_RECTANGLE_RI;
        MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
      }
      break;
    }
  case 13 :
    { /* B penetre A par XYDY */
      MACRO_POSX_RECT_RI  = FinXB; 
      MACRO_POSY_RECT_RI  = YA; 
      MACRO_POSDX_RECT_RI = FinXA; 
      MACRO_POSDY_RECT_RI = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_EXTENSION;
      MACRO_SURFACE_INTER = ExtenDXBXA * DYA;
      MACRO_LONG_DISTANCE = ExtenXADXB;
      MACRO_PENETRE_INTER = ExtenDXBXA;
      MACRO_LONG_MAX      = ExtenDXADXB;
      MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_RI;
      if ( ExtenDXBXA     > DYA )
      {
        MACRO_LONG_INTER  = ExtenDXBXA;
        MACRO_LARGE_INTER = DYA;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXBXA;
        MACRO_LONG_INTER  = DYA;
      }
      break;
    }
  case 14 :
    { /* A penetre B par X */
      MACRO_POSX_RECT_LE  = XA; 
      MACRO_POSY_RECT_LE  = YA; 
      MACRO_POSDX_RECT_LE = XB; 
      MACRO_POSDY_RECT_LE = FinYA; 

      MACRO_ORIENTATION   = DRUC_ORIENTATION_EXTENSION;
      MACRO_SURFACE_INTER = ExtenDXAXB * DYA;
      MACRO_LONG_DISTANCE = ExtenXBDXA;
      MACRO_PENETRE_INTER = ExtenDXAXB;
      MACRO_LONG_MAX      = ExtenXBXA;
      MACRO_RECT_LONG_MAX = DRUC_RECTANGLE_LE;
      if ( ExtenDXAXB     > DYA )
      {
        MACRO_LONG_INTER  = ExtenDXAXB;
        MACRO_LARGE_INTER = DYA;
      }
      else
      {
        MACRO_LARGE_INTER = ExtenDXAXB;
        MACRO_LONG_INTER  = DYA;
      }
      break;
    }
  case 15 :
    { /* A penetre B par X */
      MACRO_ORIENTATION   = DRUC_ORIENTATION_INCLUSION;
      MACRO_SURFACE_INTER = DXA * DYA;
      if ( DXA            > DYA )
      {
        MACRO_LONG_INTER  = DXA;
        MACRO_LARGE_INTER = DYA;
      }
      else
      {
        MACRO_LARGE_INTER = DXA;
        MACRO_LONG_INTER  = DYA;
      }
    }
  }/* switch */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationTeste                  |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationTeste ( DrucStructMesure,
                         DrucInstructionCourante,
                         IndexMesure,
                         Mesure,
                         RectangleOrientation
                       )
     DrucTypeStructMesure      *DrucStructMesure;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
     long                       Mesure;
     int                        RectangleOrientation;
{
long PositionX;
long PositionY;
long PositionDX;
long PositionDY;

  DrucStructMesure->MESURE_EFFECTIVE = Mesure;
  switch ( DrucInstructionCourante->OP_COMPARE [ IndexMesure ] )
  {
  case DRUC_OPERATION_INF :
    {
      if ( Mesure >= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_INFEQ :
    {
      if ( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_SUP :
    {
      if ( Mesure <= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_SUPEQ :
    {
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_DIFF :
    {
      if ( Mesure == DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_MIN :
    {
/*\
viewrdsrec (MACRO_RECTANGLE1);
viewrdsrec (MACRO_RECTANGLE2);
printf ("\nPOSITIONS %d %d %d\n",DrucInstructionCourante->REGLE_NUM [ IndexMesure ], Mesure,DrucInstructionCourante->VALEUR [ IndexMesure ]);
\*/
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        switch ( RectangleOrientation
               )
        {
          case DRUC_RECTANGLE_UP:
          {
            PositionX  = MACRO_POSX_RECT_UP;
            PositionY  = MACRO_POSY_RECT_UP;
            PositionDX = MACRO_POSDX_RECT_UP;
            PositionDY = MACRO_POSDY_RECT_UP;
            break;
          }
          case DRUC_RECTANGLE_DO:
          {
            PositionX  = MACRO_POSX_RECT_DO;
            PositionY  = MACRO_POSY_RECT_DO;
            PositionDX = MACRO_POSDX_RECT_DO;
            PositionDY = MACRO_POSDY_RECT_DO;
            break;
          }
          case DRUC_RECTANGLE_LE:
          {
            PositionX  = MACRO_POSX_RECT_LE;
            PositionY  = MACRO_POSY_RECT_LE;
            PositionDX = MACRO_POSDX_RECT_LE;
            PositionDY = MACRO_POSDY_RECT_LE;
            break;
          }
          case DRUC_RECTANGLE_RI:
          {
            PositionX  = MACRO_POSX_RECT_RI;
            PositionY  = MACRO_POSY_RECT_RI;
            PositionDX = MACRO_POSDX_RECT_RI;
            PositionDY = MACRO_POSDY_RECT_RI;
            break;
          }
        }/* switch */
/*\
viewrdsrec (MACRO_RECTANGLE1);
viewrdsrec (MACRO_RECTANGLE2);
printf ("\nPOSITIONS %d %d %d %d %d\n",DrucInstructionCourante->REGLE_NUM [ IndexMesure ], PositionX,PositionY,PositionDX,PositionDY);
\*/
        DrucIsPositionInclusInEqui ( DrucStructMesure,
                                     DrucInstructionCourante,
                                     IndexMesure,
                                     PositionX,
                                     PositionY,
                                     PositionDX,
                                     PositionDY
                                   );
      }
      break;
    }
  case DRUC_OPERATION_MAX :
    {
      if (( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      ||  (( Mesure == 0 ) 
	  && (DrucInstructionCourante->VALEUR [ IndexMesure ] == 0)))
      {
        switch ( RectangleOrientation
               )
        {
          case DRUC_RECTANGLE_UP:
          {
            PositionX  = MACRO_POSX_RECT_UP;
            PositionY  = MACRO_POSY_RECT_UP;
            PositionDX = MACRO_POSDX_RECT_UP;
            PositionDY = MACRO_POSDY_RECT_UP;
            break;
          }
          case DRUC_RECTANGLE_DO:
          {
            PositionX  = MACRO_POSX_RECT_DO;
            PositionY  = MACRO_POSY_RECT_DO;
            PositionDX = MACRO_POSDX_RECT_DO;
            PositionDY = MACRO_POSDY_RECT_DO;
            break;
          }
          case DRUC_RECTANGLE_LE:
          {
            PositionX  = MACRO_POSX_RECT_LE;
            PositionY  = MACRO_POSY_RECT_LE;
            PositionDX = MACRO_POSDX_RECT_LE;
            PositionDY = MACRO_POSDY_RECT_LE;
            break;
          }
          case DRUC_RECTANGLE_RI:
          {
            PositionX  = MACRO_POSX_RECT_RI;
            PositionY  = MACRO_POSY_RECT_RI;
            PositionDX = MACRO_POSDX_RECT_RI;
            PositionDY = MACRO_POSDY_RECT_RI;
            break;
          }
        }/* switch */
/*\
viewrdsrec (MACRO_RECTANGLE1);
viewrdsrec (MACRO_RECTANGLE2);
printf ("\nDrucInstructionCourante->OP_COMPARE %d ",DrucInstructionCourante->OP_COMPARE[ IndexMesure ]);
printf ("\nPOSITIONS %d %d %d %d %d\n",DrucInstructionCourante->REGLE_NUM [ IndexMesure ], PositionX,PositionY,PositionDX,PositionDY);
\*/
        DrucIsPositionInclusInEqui ( DrucStructMesure,
                                     DrucInstructionCourante,
                                     IndexMesure,
                                     PositionX,
                                     PositionY,
                                     PositionDX,
                                     PositionDY
                                   );
      }
      break;
    }
  } /* switch */
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationTesteDistance          |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationTesteDistance ( DrucStructMesure,
                                 DrucInstructionCourante,
                                 IndexMesure,
                                 Mesure
                               )
     DrucTypeStructMesure      *DrucStructMesure;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
     long                       Mesure;
{

  DrucStructMesure->MESURE_EFFECTIVE = Mesure;
  switch ( DrucInstructionCourante->OP_COMPARE [ IndexMesure ] )
  {
  case DRUC_OPERATION_INF :
    {
      if ( Mesure >= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_INFEQ :
    {
      if ( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_SUP :
    {
      if ( Mesure <= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_SUPEQ :
    {
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_DIFF :
    {
      if ( Mesure == DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_MIN :
    {
/*\
printf ("\nDRUC_OPERATION_MIN: %d  %d\n",Mesure,DrucInstructionCourante->VALEUR [ IndexMesure ]);
\*/
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucIsPositionInclusInEqui ( DrucStructMesure,
                                     DrucInstructionCourante,
                                     IndexMesure,
                                     MACRO_RECTANGLE2->X,
                                     MACRO_RECTANGLE2->Y,
                                     MACRO_RECTANGLE2->X + MACRO_RECTANGLE2->DX,
                                     MACRO_RECTANGLE2->Y + MACRO_RECTANGLE2->DY
                                   );
      }
      break;
    }
  case DRUC_OPERATION_MAX :
    {
      if (( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      ||  (( Mesure == 0 ) 
	  && (DrucInstructionCourante->VALEUR [ IndexMesure ] == 0)))
      {
        DrucIsPositionInclusInEqui ( DrucStructMesure,
                                     DrucInstructionCourante,
                                     IndexMesure,
                                     MACRO_RECTANGLE2->X,
                                     MACRO_RECTANGLE2->Y,
                                     MACRO_RECTANGLE2->X + MACRO_RECTANGLE2->DX,
                                     MACRO_RECTANGLE2->Y + MACRO_RECTANGLE2->DY
                                   );
      }
      break;
    }
  } /* switch */
}

/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationTesteInter             |
|                                                             |
\------------------------------------------------------------*/
void DrucMesureTesteInter ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure
                          )
     DrucTypeStructMesure      *DrucStructMesure;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
{
rdsrec_list            *RectangleSource;
long PositionX;
long PositionY;
long PositionDX;
long PositionDY;

  RectangleSource            = DrucStructMesure->RECTANGLE1;
  switch ( MACRO_TOPOLOGIE )
  {
    case 0:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = PositionX + MACRO_RECTANGLE2->DX;
      PositionDY = PositionY + MACRO_RECTANGLE2->DY;
      break;
    }
    case 1:
    {
      PositionX  = RectangleSource->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = MACRO_RECTANGLE2->X + MACRO_RECTANGLE2->DX;
      PositionDY = PositionY + MACRO_RECTANGLE2->DY;
      break;
    }
    case 2:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = RectangleSource->X + RectangleSource->DX;
      PositionDY = PositionY + MACRO_RECTANGLE2->DY;
      break; 
    }
    case 3:
    {
      PositionX  = RectangleSource->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = RectangleSource->X + RectangleSource->DX;
      PositionDY = PositionY + MACRO_RECTANGLE2->DY;
      break; 
    }
    case 4:
    { 
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = RectangleSource->Y;
      PositionDX = PositionX + MACRO_RECTANGLE2->DX;
      PositionDY = MACRO_RECTANGLE2->Y + MACRO_RECTANGLE2->DY;
      break; 
    }
    case 5:
    {
      PositionX  = RectangleSource->X;
      PositionY  = RectangleSource->Y;
      PositionDX = MACRO_RECTANGLE2->X + MACRO_RECTANGLE2->DX;
      PositionDY = MACRO_RECTANGLE2->Y + MACRO_RECTANGLE2->DY;
      break; 
    }
    case 6:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = RectangleSource->Y;
      PositionDX = RectangleSource->X + RectangleSource->DX;
      PositionDY = MACRO_RECTANGLE2->Y + MACRO_RECTANGLE2->DY;
      break;
    }
    case 7:
    {
      PositionX  = RectangleSource->X;
      PositionY  = RectangleSource->Y;
      PositionDX = PositionX + RectangleSource->DX;
      PositionDY = MACRO_RECTANGLE2->Y + MACRO_RECTANGLE2->DY;
      break;  
    }
    case 8:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = PositionX + MACRO_RECTANGLE2->DX;
      PositionDY = RectangleSource->Y + RectangleSource->DY;
      break; 
    }
    case 9:
    {
      PositionX  = RectangleSource->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = MACRO_RECTANGLE2->X + MACRO_RECTANGLE2->DX;
      PositionDY = RectangleSource->Y + RectangleSource->DY;
      break;
    }
    case 10:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = RectangleSource->X + RectangleSource->DX;
      PositionDY = RectangleSource->Y + RectangleSource->DY;
      break;
    }
    case 11:
    {
      PositionX  = RectangleSource->X;
      PositionY  = MACRO_RECTANGLE2->Y;
      PositionDX = PositionX + RectangleSource->DX;
      PositionDY = RectangleSource->Y + RectangleSource->DY;
      break;  
    }
    case 12:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = RectangleSource->Y;
      PositionDX = PositionX + MACRO_RECTANGLE2->DX;
      PositionDY = PositionY + RectangleSource->DY;
      break;  
    }
    case 13:
    {
      PositionX  = RectangleSource->X;
      PositionY  = RectangleSource->Y;
      PositionDX = MACRO_RECTANGLE2->X + MACRO_RECTANGLE2->DX;
      PositionDY = PositionY + RectangleSource->DY;
      break;
    }
    case 14:
    {
      PositionX  = MACRO_RECTANGLE2->X;
      PositionY  = RectangleSource->Y;
      PositionDX = RectangleSource->X + RectangleSource->DX;
      PositionDY = PositionY + RectangleSource->DY;
      break; 
    }
    case 15:
    {
      PositionX  = RectangleSource->X;
      PositionY  = RectangleSource->Y;
      PositionDX = RectangleSource->X + RectangleSource->DX;
      PositionDY = PositionY + RectangleSource->DY;
      break; 
    }
  }
  DrucIsPositionInclusInEqui ( DrucStructMesure,
                               DrucInstructionCourante,
                               IndexMesure,
                               PositionX,
                               PositionY,
                               PositionDX,
                               PositionDY
                             );
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationSurfaceInter           |
|                                                             |
\------------------------------------------------------------*/
void DrucMesureInter ( DrucStructMesure,
                       DrucInstructionCourante,
                       IndexMesure,
                       Mesure
                     )
     DrucTypeStructMesure      *DrucStructMesure;
     DrucTypeStructInstruction *DrucInstructionCourante;
     int                        IndexMesure;
     long                       Mesure;
{

  DrucStructMesure->MESURE_EFFECTIVE = Mesure;
  switch ( DrucInstructionCourante->OP_COMPARE [ IndexMesure ] )
  {
  case DRUC_OPERATION_INF :
    {
      if ( Mesure >= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_INFEQ :
    {
      if ( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_SUP :
    {
      if ( Mesure <= DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_SUPEQ :
    {
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_DIFF :
    {
      if ( Mesure == DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
        DrucErreurDeuxCorps ( DrucStructMesure,
                              DrucInstructionCourante,
                              IndexMesure
                            );
      }
      break;
    }
  case DRUC_OPERATION_MIN :
    {
      if ( Mesure < DrucInstructionCourante->VALEUR [ IndexMesure ] )
      {
          DrucMesureTesteInter ( DrucStructMesure,
                                 DrucInstructionCourante,
                                 IndexMesure
                               );
      }
      break;
    }
  case DRUC_OPERATION_MAX :
    {
      if (( Mesure > DrucInstructionCourante->VALEUR [ IndexMesure ] )
      ||  (( Mesure == 0 ) 
	  && (DrucInstructionCourante->VALEUR [ IndexMesure ] == 0)))
      {
      DrucMesureTesteInter ( DrucStructMesure,
                             DrucInstructionCourante,
                             IndexMesure
                           );   
      }
      break;
    }
  } /* switch */
}
