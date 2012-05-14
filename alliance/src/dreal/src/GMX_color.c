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
| Tool    :                   DREAL                           |
|                                                             |
| File    :                  Colors.c                         |
|                                                             |
| Authors :              Jacomme Ludovic                      |
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

  GC   DrealLayerFillGC   [ RDS_ALL_LAYER ];
  GC   DrealLayerDrawGC   [ RDS_ALL_LAYER ];
  GC   DrealLayerAcceptGC [ RDS_ALL_LAYER ];
  GC   DrealAcceptDrawGC;
  GC   DrealBackgroundGC;
  GC   DrealGridGC;
  GC   DrealSmallTextGC;
  GC   DrealMediumTextGC;
  GC   DrealLargeTextGC;
  GC   DrealXorGCXor;
  GC   DrealXorGCInvert;
  GC   DrealXorGC;

  XFontStruct *DrealLargeTextFont;
  XFontStruct *DrealMediumTextFont;
  XFontStruct *DrealSmallTextFont;

  Colormap DrealColorMap;

/*------------------------------------------------------------\
|                                                             |
|                      Layer Color Define                     |
|                                                             |
\------------------------------------------------------------*/

  static char DrealLayerPatternDefine[ RDS_ALL_LAYER ] =

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
      0,  /* RDS_USER0  */
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

  static char DrealPatternBits[ DREAL_MAX_PATTERN ][ DREAL_PATTERN_SQUARE ] =

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

  static Pixmap DrealPattern[ DREAL_MAX_PATTERN ];

  static char  *DrealColorName[ 5 ] =

  {
    DREAL_BLACK_COLOR,
    DREAL_WHITE_COLOR,
    DREAL_TEXT_COLOR ,
    DREAL_ACCEPT_COLOR,
    DREAL_CURSOR_COLOR
  };

  static int DrealColor[ DREAL_MAX_COLOR ];

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                     DrealSwitchColormap                     |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeColorMap()
{
  Display *ADisplay;
  Screen  *AScreen;

  ADisplay      = XtDisplay( DrealTopLevel );
  AScreen       = XtScreen( DrealTopLevel );
  DrealColorMap = DefaultColormapOfScreen( AScreen );

  if ( DREAL_SWITCH_COLOR_MAP )
  {
    DrealColorMap = XCopyColormapAndFree( ADisplay, DrealColorMap );
    XInstallColormap( ADisplay, DrealColorMap );
    XtVaSetValues( DrealTopLevel, XmNcolormap, DrealColorMap, NULL );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                     DrealInitializeColors                   |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeColors()

{
  XColor   ForgetIt;
  XColor   GetIt;
  Window   Root;
  int      Counter;
  char     Error;
  char    *ColorName;

  rdsbegin();

  if ( DREAL_CURSOR_COLOR_NAME != (char *)NULL )
  {
    DrealColorName[ 4 ] = DREAL_CURSOR_COLOR_NAME;
  }

  Error = False;

  for ( Counter = 0; Counter < DREAL_MAX_COLOR; Counter++ )
  {
    if ( Counter >= 5 )
    {
      ColorName = DREAL_RDS_LAYER_NAME_TABLE[ Counter - 5 ][ 1 ];
    }
    else
    {
      ColorName = DrealColorName[ Counter ];
    }

    if ( ColorName != (char *)NULL )
    {
      if ( ! XAllocNamedColor( DrealGraphicDisplay,
                               DrealColorMap, ColorName, 
                              &GetIt, &ForgetIt ) ) 
      {
        fprintf( stderr, "\nGmx: Unable to allocate color '%s' !\n",
                 ColorName );

        Error = True;
      }

      DrealColor[ Counter ] = GetIt.pixel;
    }
    else 
    {
      DrealColor[ Counter ] = DrealColor[ 0 ];
    }
  }

  if ( Error == True ) exit( 1 );

  Root = RootWindowOfScreen ( XtScreen ( DrealGraphicWindow ) );

  for ( Counter = 0; Counter < DREAL_MAX_PATTERN; Counter++ )
  {
    DrealPattern[ Counter ]  =

      XCreateBitmapFromData( DrealGraphicDisplay,
                             Root,
                             DrealPatternBits[ Counter ],
                             DREAL_PATTERN_WIDTH,
                             DREAL_PATTERN_HEIGHT );
  }

  rdsend();
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealChangeCursorContext                    |
|                                                             |
\------------------------------------------------------------*/

void DrealChangeCursorContext()
{
  XGCValues GraphicValue;

  if ( DREAL_XOR_CURSOR == DREAL_TRUE )
  {
    DrealXorGC = DrealXorGCXor;
  }
  else
  {
    DrealXorGC = DrealXorGCInvert;
  }
}

/*------------------------------------------------------------\
|                                                             |
|                 DrealInitializeGraphicContext               |
|                                                             |
\------------------------------------------------------------*/

void DrealInitializeGraphicContext()

{
  XGCValues GraphicValue;
  int       Counter;
  Window    Root;
  char      Pattern;

  rdsbegin();

  DrealLargeTextFont = XLoadQueryFont( DrealGraphicDisplay,
                                       DREAL_LARGE_TEXT_FONT_NAME );

  DrealMediumTextFont = XLoadQueryFont( DrealGraphicDisplay,
                                        DREAL_MEDIUM_TEXT_FONT_NAME );

  DrealSmallTextFont = XLoadQueryFont( DrealGraphicDisplay,
                                       DREAL_SMALL_TEXT_FONT_NAME );

  GraphicValue.foreground = DrealColor[ 0 ];  /* Black */
  GraphicValue.background = DrealColor[ 0 ];
  GraphicValue.plane_mask = AllPlanes;

  Root = RootWindowOfScreen ( XtScreen ( DrealGraphicWindow ) ),

  DrealBackgroundGC = XCreateGC ( DrealGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                    &GraphicValue );

  GraphicValue.foreground = DrealColor[ 1 ];  /* White */

  DrealGridGC = XCreateGC ( DrealGraphicDisplay,
                            Root,
                            ( GCForeground |
                              GCBackground |
                              GCPlaneMask ),
                              &GraphicValue );

  GraphicValue.foreground = DrealColor[ 3 ];
 
  DrealAcceptDrawGC = XCreateGC ( DrealGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  GraphicValue.foreground = DrealColor[ 2 ];  /* Text  */

  DrealSmallTextGC = XCreateGC ( DrealGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                   &GraphicValue );

  DrealMediumTextGC = XCreateGC ( DrealGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask ),
                                   &GraphicValue );

  DrealLargeTextGC = XCreateGC ( DrealGraphicDisplay,
                                 Root,
                                 ( GCForeground |
                                   GCBackground |
                                   GCPlaneMask ),
                                  &GraphicValue );

  XSetFont( DrealGraphicDisplay,
            DrealSmallTextGC, DrealSmallTextFont->fid );

  XSetFont( DrealGraphicDisplay,
            DrealMediumTextGC, DrealMediumTextFont->fid );

  XSetFont( DrealGraphicDisplay,
            DrealLargeTextGC, DrealLargeTextFont->fid );

  GraphicValue.background = DrealColor[ 4 ];  /* Xor Color */
  GraphicValue.foreground = DrealColor[ 4 ];  /* Xor Color */
  GraphicValue.function   = GXxor;

  DrealXorGCXor = XCreateGC ( DrealGraphicDisplay, 
                              Root,
                              ( GCFunction   |
                                GCForeground |
                                GCBackground |
                                GCPlaneMask ),
                                &GraphicValue );

  GraphicValue.function = GXinvert;

  DrealXorGCInvert = XCreateGC ( DrealGraphicDisplay, 
                                 Root,
                                 ( GCFunction   |
                                   GCPlaneMask ),
                                   &GraphicValue );
  DrealChangeCursorContext();

  GraphicValue.background = DrealColor[ 0 ];  /* Black */
  GraphicValue.foreground = DrealColor[ 1 ];  /* White */

  for ( Counter = 0; Counter < RDS_ALL_LAYER; Counter++ )
  {
    if ( DREAL_RDS_LAYER_NAME_TABLE[ Counter ][ 0 ] != (char *)NULL )
    {
      Pattern = DREAL_RDS_LAYER_PATTERN_TABLE[ Counter ];

      if ( ( Pattern < 0                  ) ||
           ( Pattern >= DREAL_MAX_PATTERN ) )
      {
        Pattern = DrealLayerPatternDefine[ Counter ];
      }
  
      GraphicValue.stipple    = DrealPattern[ Pattern ];
      GraphicValue.fill_style = FillStippled;
      GraphicValue.foreground = DrealColor[ 3 ];
  
      DrealLayerAcceptGC[ Counter ] =
  
           XCreateGC ( DrealGraphicDisplay,
                                  Root,
                                  ( GCStipple    |
                                    GCForeground |
                                    GCBackground |
                                    GCFillStyle  |
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
      GraphicValue.foreground = DrealColor[ Counter + 5 ];
  
      DrealLayerFillGC[ Counter ] = 
  
           XCreateGC ( DrealGraphicDisplay,
                                  Root,
                                  ( GCStipple    | 
                                    GCForeground |
                                    GCBackground |
                                    GCFillStyle  | 
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
      DrealLayerDrawGC[ Counter ] = 
  
           XCreateGC ( DrealGraphicDisplay,
                                  Root,
                                  ( GCForeground |
                                    GCBackground |
                                    GCPlaneMask  ),
                                  &GraphicValue  );
  
    }
  }

  rdsend();
} 
