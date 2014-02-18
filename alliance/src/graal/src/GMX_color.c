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
| Tool    :                   GRAAL                           |
|                                                             |
| File    :                  Colors.c                         |
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

# include <stdio.h>
# include <Xm/Xm.h>
 
# include "mut.h"
# include "mph.h"
# include "rds.h"
# include "rpr.h"
# include "rds.h"
# include "GSB.h"
# include "GMX.h"
# include "GMX_color.h" 

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

  GC   GraalLayerFillGC   [ RDS_ALL_LAYER ];
  GC   GraalLayerDrawGC   [ RDS_ALL_LAYER ];
  GC   GraalLayerAcceptGC [ RDS_ALL_LAYER ];
  GC   GraalLayerEquiGC   [ RDS_ALL_LAYER ];
  GC   GraalLayerDrucGC   [ RDS_ALL_LAYER ];
  GC   GraalAcceptDrawGC;
  GC   GraalDrucDrawGC;
  GC   GraalEquiDrawGC;
  GC   GraalBackgroundGC;
  GC   GraalGridGC;
  GC   GraalSmallTextGC;
  GC   GraalMediumTextGC;
  GC   GraalLargeTextGC;
  GC   GraalXorGCInvert;
  GC   GraalXorGCXor;
  GC   GraalXorGC;

  XFontStruct *GraalLargeTextFont;
  XFontStruct *GraalMediumTextFont;
  XFontStruct *GraalSmallTextFont;

  Colormap GraalColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

  static char GraalLayerPatternDefine[ RDS_ALL_LAYER ] =

  {
      0,  /* RDS_NWELL  */
      0,  /* RDS_PWELL  */
      0,  /* RDS_NIMP   */
      0,  /* RDS_PIMP   */
      0,  /* RDS_ACTIV  */
      1,  /* RDS_NDIF   */
      1,  /* RDS_PDIF   */
      1,  /* RDS_NTIE   */
      1,  /* RDS_PTIE   */
      3,  /* RDS_POLY   */
      2,  /* RDS_GATE   */
      3,  /* RDS_VPOLY  */
     12,  /* RDS_TPOLY  */
      2,  /* RDS_POLY2  */
      5,  /* RDS_CONT   */
      3,  /* RDS_TPOLY2 */
      5,  /* RDS_TCONT  */
      5,  /* RDS_CONT2  */
      4,  /* RDS_ALU1   */
      4,  /* RDS_VALU1  */
     12,  /* RDS_TALU1  */
      5,  /* RDS_VIA1   */
      5,  /* RDS_TVIA1  */
      6,  /* RDS_ALU2   */
      6,  /* RDS_VALU2  */
     12,  /* RDS_TALU2  */
      5,  /* RDS_VIA2   */
      5,  /* RDS_TVIA2  */
      8,  /* RDS_ALU3   */
      8,  /* RDS_VALU3  */
     12,  /* RDS_TALU3  */
      5,  /* RDS_VIA3   */
      5,  /* RDS_TVIA3  */
      9,  /* RDS_ALU4   */
      9,  /* RDS_VALU4  */
     14,  /* RDS_TALU4  */
      5,  /* RDS_VIA4   */
      5,  /* RDS_TVIA4  */
     10,  /* RDS_ALU5   */
     10,  /* RDS_VALU5  */
     12,  /* RDS_TALU5  */
     13,  /* RDS_VIA5   */
      5,  /* RDS_TVIA5  */
     11,  /* RDS_ALU6   */
     11,  /* RDS_VALU6  */
     12,  /* RDS_TALU6  */
      5,  /* RDS_VIA6   */
      5,  /* RDS_TVIA6  */
      8,  /* RDS_ALU7   */
      8,  /* RDS_VALU7  */
     12,  /* RDS_TALU7  */
      5,  /* RDS_VIA7   */
      5,  /* RDS_TVIA7  */
      9,  /* RDS_ALU8   */
      9,  /* RDS_VALU8  */
     12,  /* RDS_TALU8  */
      5,  /* RDS_VIA8   */
      5,  /* RDS_TVIA8  */
     10,  /* RDS_ALU9   */
      7,  /* RDS_CPAS   */
      7,  /* RDS_REF    */
      4,  /* RDS_USER0  */
      0,  /* RDS_USER1  */
      0,  /* RDS_USER2  */
      0,  /* RDS_USER3  */
      0,  /* RDS_USER4  */
      0,  /* RDS_USER5  */
      0,  /* RDS_USER6  */
      0,  /* RDS_USER7  */
      0,  /* RDS_USER8  */
      0,  /* RDS_USER9  */
      0   /* RDS_ABOX   */
  };


  static char GraalPatternBits[ GRAAL_MAX_PATTERN ][ GRAAL_PATTERN_SQUARE ] =

  {                             /* FOR SUBST 0 */
    { 
      0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf7, 0xff, 0xff,
      0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
      0xf7, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    }
    ,                           /* FOR DIF & TIE 1  */
    { 
      0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb,
      0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee,
      0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee
    }
    ,                           /* FOR GATE 2  */
    {
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55
    }
    ,                           /* FOR POLY 3  */
    { 
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa
    }
    ,                           /* FOR ALU1 4 */
    { 
      0x88, 0x88, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11, 0x88, 0x88, 0x44, 0x44,
      0x22, 0x22, 0x11, 0x11, 0x88, 0x88, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11,
      0x88, 0x88, 0x44, 0x44, 0x22, 0x22, 0x11, 0x11
    }
    ,                           /* FOR CONT 5 */
    { 
      0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff,
      0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff,
      0x55, 0x55, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff
    }
    ,                           /* FOR ALU2 6 */
    { 
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }
    ,                           /* FOR REF  7 */
    { 
      0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x11, 0x11,
      0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44,
      0xaa, 0xaa, 0x11, 0x11, 0xaa, 0xaa, 0x44, 0x44
    }
    ,                           /* FOR ALU3 8 */
    {
      0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11,
      0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11,
      0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11
    }
    ,                          /* FOR ALU4  9 */
    {
      0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22,
      0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22,
      0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22
    }
    ,                          /* FOR ALU5  10 */
    {
      0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88,
      0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88,
      0x22, 0x22, 0x88, 0x88, 0x22, 0x22, 0x88, 0x88
    }
    ,                          /* FOR ALU6  11 */ 
    {
      0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44,
      0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44,
      0x11, 0x11, 0x44, 0x44, 0x11, 0x11, 0x44, 0x44
    }
    ,                        /* FOR TPOLY, TALU1 12 */
    { 
      0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00
    }
    ,                        /* FOR VIA 13 */
    {
      0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa,
      0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa,
      0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa
    }
    ,                           /* FOR TALUx 14  */
    {
      0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }
    ,                           /* FOR USER  15  */
    {
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x11, 0x11, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00
    } 
    ,
    { 
      0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77,
      0xbb, 0xbb, 0xdd, 0xdd, 0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd,
      0xee, 0xee, 0x77, 0x77, 0xbb, 0xbb, 0xdd, 0xdd
    }
    ,
    {
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa
    }
    ,
    {
      0xdd, 0xdd, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0xd5, 0xd5, 0xaa, 0xaa,
      0x55, 0x55, 0xaa, 0xaa, 0xdd, 0xdd, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
      0xdd, 0xd5, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa
    }
    ,
    {
      0x77, 0x77, 0xaa, 0xaa, 0xd5, 0xd5, 0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa,
      0xdd, 0x5d, 0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa, 0xd5, 0xd5, 0xaa, 0xaa,
      0x77, 0x77, 0xaa, 0xaa, 0x5d, 0xdd, 0xaa, 0xaa
    }
    ,
    {
      0x55, 0x55, 0xbb, 0xbb, 0x55, 0x55, 0xfe, 0xfe, 0x55, 0x55, 0xbb, 0xbb,
      0x55, 0x55, 0xee, 0xef, 0x55, 0x55, 0xbb, 0xbb, 0x55, 0x55, 0xfe, 0xfe,
      0x55, 0x55, 0xbb, 0xbb, 0x55, 0x55, 0xef, 0xef
    }
    ,
    {
      0xff, 0xff, 0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa,
      0x77, 0x7f, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa, 0x77, 0x77, 0xaa, 0xaa,
      0xff, 0xff, 0xaa, 0xaa, 0x7f, 0x7f, 0xaa, 0xaa
    }
    ,
    {
      0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa,
      0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa,
      0xff, 0xff, 0xaa, 0xaa, 0xff, 0xff, 0xaa, 0xaa
    }
    ,
    {
      0x55, 0x55, 0xff, 0xff, 0xdd, 0xdd, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff,
      0x5d, 0xdd, 0xff, 0xff, 0x55, 0x55, 0xff, 0xff, 0xdd, 0xdd, 0xff, 0xff,
      0x55, 0x55, 0xff, 0xff, 0x5d, 0x5d, 0xff, 0xff
    }
    ,
    {
      0xee, 0xee, 0xff, 0xff, 0xbb, 0xba, 0xff, 0xff, 0xee, 0xee, 0xff, 0xff,
      0xab, 0xbb, 0xff, 0xff, 0xee, 0xee, 0xff, 0xff, 0xbb, 0xba, 0xff, 0xff,
      0xee, 0xee, 0xff, 0xff, 0xbb, 0xab, 0xff, 0xff
    }
    ,
    {
      0xff, 0xff, 0xee, 0xee, 0xff, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xee, 0xee,
      0xff, 0xff, 0xbf, 0xbb, 0xff, 0xff, 0xee, 0xee, 0xff, 0xff, 0xfb, 0xfb,
      0xff, 0xff, 0xee, 0xee, 0xff, 0xff, 0xbf, 0xbf
    }
    ,
    {
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbb, 0xbb, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xfb, 0xfb, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xbb, 0xbb,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xfb
    }
    ,
    {
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x88,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08
    }
    ,
    {
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x77, 0x77, 0xff, 0xff, 0xff, 0xff,
      0xff, 0xff, 0xf7, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x77, 0x77,
      0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf7
    }
    ,
    {
      0x10, 0x10, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00,
      0x44, 0x44, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00,
      0x01, 0x01, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00
    }
    ,
    {
      0x00, 0x00, 0x11, 0x51, 0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x15, 0x15,
      0x00, 0x00, 0x44, 0x44, 0x00, 0x00, 0x51, 0x11, 0x00, 0x00, 0x44, 0x44,
      0x00, 0x00, 0x15, 0x15, 0x00, 0x00, 0x44, 0x44
    }
    ,
    {
      0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x8a, 0x88, 0x00, 0x00, 0xaa, 0xaa,
      0x00, 0x00, 0x88, 0x88, 0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x8a, 0x8a,
      0x00, 0x00, 0xaa, 0xaa, 0x00, 0x00, 0x88, 0x88
    }
    ,
    {
      0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00,
      0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00,
      0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x00, 0x00
    }
    ,
    {
      0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x88, 0x88, 0x55, 0x55, 0x00, 0x00,
      0x55, 0x55, 0x80, 0x80, 0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x88, 0x88,
      0x55, 0x55, 0x00, 0x00, 0x55, 0x55, 0x88, 0x80
    }
    ,
    {
      0x22, 0x22, 0x55, 0x55, 0x80, 0x80, 0x55, 0x55, 0x22, 0x22, 0x55, 0x55,
      0x88, 0x08, 0x55, 0x55, 0x22, 0x22, 0x55, 0x55, 0x80, 0x80, 0x55, 0x55,
      0x22, 0x22, 0x55, 0x55, 0x08, 0x08, 0x55, 0x55
    }
    ,
    {
      0x88, 0x88, 0x55, 0x55, 0x22, 0xa2, 0x55, 0x55, 0x88, 0x88, 0x55, 0x55,
      0x2a, 0x2a, 0x55, 0x55, 0x88, 0x88, 0x55, 0x55, 0xa2, 0x22, 0x55, 0x55,
      0x88, 0x88, 0x55, 0x55, 0x2a, 0x2a, 0x55, 0x55
    }
    ,
    {
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x54, 0x54, 0xaa, 0xaa, 0x55, 0x55,
      0xaa, 0xaa, 0x44, 0x44, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x44, 0x54,
      0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x44, 0x44
    }
  };

  static Pixmap GraalPattern[ GRAAL_MAX_PATTERN ];

  static char  *GraalColorName[ 7 ] =

  {
    GRAAL_BLACK_COLOR,
    GRAAL_WHITE_COLOR,
    GRAAL_TEXT_COLOR ,
    GRAAL_ACCEPT_COLOR,
    GRAAL_EQUI_COLOR,
    GRAAL_DRUC_COLOR,
    GRAAL_CURSOR_COLOR
  };

  static int GraalColor[ GRAAL_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     GraalSwitchColormap                     |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeColorMap()
{
  Display *ADisplay;
  Screen  *AScreen;

  ADisplay      = XtDisplay( GraalTopLevel );
  AScreen       = XtScreen( GraalTopLevel );
  GraalColorMap = DefaultColormapOfScreen( AScreen );

  if ( GRAAL_SWITCH_COLOR_MAP )
  {
    GraalColorMap = XCopyColormapAndFree( ADisplay, GraalColorMap );
    XInstallColormap( ADisplay, GraalColorMap );
    XtVaSetValues( GraalTopLevel, XmNcolormap, GraalColorMap, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     GraalInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  Window   Root;
  int      Counter;
  char     Error;
  char    *ColorName;

  rdsbegin();

  if ( GRAAL_CURSOR_COLOR_NAME != (char *)NULL )
  {
    GraalColorName[ 6 ] = GRAAL_CURSOR_COLOR_NAME;
  }

  Error = False;

  for ( Counter = 0; Counter < GRAAL_MAX_COLOR; Counter++ )
  {
    if ( Counter >= 7 )
    {
      ColorName = GRAAL_RDS_LAYER_NAME_TABLE[ Counter - 7 ][ 1 ];
    }
    else
    {
      ColorName = GraalColorName[ Counter ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( GraalGraphicDisplay, 
                               GraalColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf( stderr, "\nGmx: Unable to allocate color '%s' !\n",
                 ColorName );

        Error = True;
      }

      GraalColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      GraalColor[ Counter ] = GraalColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  Root = RootWindowOfScreen ( XtScreen ( GraalGraphicWindow ) );

  for ( Counter = 0; Counter < GRAAL_MAX_PATTERN; Counter++ )
  {
    GraalPattern[ Counter ]  =

      XCreateBitmapFromData( GraalGraphicDisplay,
                             Root,
                             GraalPatternBits[ Counter ],
                             GRAAL_PATTERN_WIDTH,
                             GRAAL_PATTERN_HEIGHT );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalChangeCursorContext                    |
|                                                             |
\------------------------------------------------------------*/

void GraalChangeCursorContext()
{
  if ( GRAAL_XOR_CURSOR == GRAAL_TRUE )
  {
    GraalXorGC = GraalXorGCXor;
  }
  else
  {
    GraalXorGC = GraalXorGCInvert;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 GraalInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void GraalInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;
  char      Pattern;

  rdsbegin();

  GraalLargeTextFont = XLoadQueryFont( GraalGraphicDisplay,
                                       GRAAL_LARGE_TEXT_FONT_NAME );

  GraalMediumTextFont = XLoadQueryFont( GraalGraphicDisplay,
                                        GRAAL_MEDIUM_TEXT_FONT_NAME );

  GraalSmallTextFont = XLoadQueryFont( GraalGraphicDisplay,
                                       GRAAL_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = GraalColor[ 0 ];  /* Black */
  GraphicValue.background = GraalColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( GraalGraphicWindow ) ),

  GraalBackgroundGC = XCreateGC ( GraalGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = GraalColor[ 1 ];  /* White */

  GraalGridGC = XCreateGC ( GraalGraphicDisplay, 
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = GraalColor[ 3 ];
 
  GraalAcceptDrawGC = XCreateGC ( GraalGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  GraphicValue.foreground = GraalColor[ 4 ];

  GraalEquiDrawGC = XCreateGC ( GraalGraphicDisplay,
                                Root,
                                ( GCForeground |
                                  GCBackground |
                                  GCPlaneMask ),
                                 &GraphicValue );

  GraphicValue.foreground = GraalColor[ 5 ];

  GraalDrucDrawGC = XCreateGC ( GraalGraphicDisplay,
                                Root,
                                ( GCForeground |
                                  GCBackground |
                                  GCPlaneMask ),
                                 &GraphicValue );

  GraphicValue.foreground = GraalColor[ 2 ];  /* Text  */

  GraalSmallTextGC = XCreateGC ( GraalGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  GraalMediumTextGC = XCreateGC ( GraalGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  GraalLargeTextGC = XCreateGC ( GraalGraphicDisplay, 
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( GraalGraphicDisplay,
            GraalSmallTextGC, GraalSmallTextFont->fid );

  XSetFont( GraalGraphicDisplay,
            GraalMediumTextGC, GraalMediumTextFont->fid );

  XSetFont( GraalGraphicDisplay,
            GraalLargeTextGC, GraalLargeTextFont->fid );

  GraphicValue.background = GraalColor[ 6 ];  /* Xor Color */
  GraphicValue.foreground = GraalColor[ 6 ];  /* Xor Color */
  GraphicValue.function   = GXxor;

  GraalXorGCXor = XCreateGC ( GraalGraphicDisplay, 
                              Root,
                              ( GCFunction   |
                                GCForeground |
                                GCBackground |
                                GCPlaneMask ),
                                &GraphicValue );

  GraphicValue.function = GXinvert;

  GraalXorGCInvert = XCreateGC ( GraalGraphicDisplay, 
                                 Root,
                                 ( GCFunction   |
                                   GCPlaneMask ),
                                  &GraphicValue );

  GraalChangeCursorContext();

  GraphicValue.background = GraalColor[ 0 ];  /* Black */
  GraphicValue.foreground = GraalColor[ 1 ];  /* White */

  for ( Counter = 0; Counter < RDS_ALL_LAYER; Counter++ )
  {
    if ( GRAAL_RDS_LAYER_NAME_TABLE[ Counter ][ 0 ] != (char *)NULL )
    {
      Pattern = GRAAL_RDS_LAYER_PATTERN_TABLE[ Counter ];

      if ( ( Pattern < 0                  ) ||
           ( Pattern >= GRAAL_MAX_PATTERN ) )
      {
        Pattern = GraalLayerPatternDefine[ Counter ];
      }
  
      GraphicValue.stipple    = GraalPattern[ (int)Pattern ];
      GraphicValue.fill_style = FillStippled;
      GraphicValue.foreground = GraalColor[ 3 ];
  
      GraalLayerAcceptGC[ Counter ] =
  
           XCreateGC ( GraalGraphicDisplay,
                                  Root,
                                  ( GCStipple    |
                                    GCForeground |
                                    GCBackground |
                                    GCFillStyle  |
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
      GraphicValue.foreground = GraalColor[ 4 ];
   
      GraalLayerEquiGC[ Counter ] =
  
           XCreateGC ( GraalGraphicDisplay,
                                  Root,
                                  ( GCStipple    |
                                    GCForeground |
                                    GCBackground |
                                    GCFillStyle  |
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
      GraphicValue.foreground = GraalColor[ 5 ];
  
      GraalLayerDrucGC[ Counter ] =
  
           XCreateGC ( GraalGraphicDisplay,
                                  Root,
                                  ( GCStipple    |
                                    GCForeground |
                                    GCBackground |
                                    GCFillStyle  |
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
      GraphicValue.foreground = GraalColor[ Counter + 7 ];
  
      GraalLayerFillGC[ Counter ] = 
  
           XCreateGC ( GraalGraphicDisplay, 
                                  Root,
                                  ( GCStipple    | 
                                    GCForeground |
                                    GCBackground |
                                    GCFillStyle  | 
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
      GraalLayerDrawGC[ Counter ] = 
  
           XCreateGC ( GraalGraphicDisplay, 
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
    }
  }

  rdsend();
}
