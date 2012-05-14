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
| Tool    :                   LYNX                            |
|                                                             |
| File    :                  bulk.c                           |
|                                                             |
| Author  :              Picault Stephane         *4p*        |
|                                                             |
| Date    :                  15.07.98                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <string.h>

# include "mut.h"
# include "mlo.h"
# include "mph.h"
# include "rds.h"
# include "rwi.h"
# include "rut.h"
# include "rpr.h"
# include "rfm.h"

# include "mbkrds.h"
# include "box.h"
# include "bulk.h"
# include "error.h"

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
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                       LynxAddLayerImplicit                  |
|                                                             |
\------------------------------------------------------------*/

void LynxAddLayerImplicit( Figure, Window )

  rdsfig_list *Figure;
  rdswindow   *Window;
{
   long          Layer;
   rdsrec_list  *Rec;

   for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ )
   {
      if ( GET_LYNX_BULK_IMPLICIT ( Layer ) != RDS_LYNX_BULK_IMPLICIT_EMPTY )
      {
            Rec = addrdsfigrec( Figure,
                                        "implicit",
                                        Layer,
                                        Window->XMIN,
                                        Window->YMIN,
                                        Window->XMAX,
                                        Window->YMAX );

            SetLynxTreat( Rec );
            LYNX_LINK( Rec ) = Rec;
            addrdsrecwindow( Rec, Window );
      }
   }
}
