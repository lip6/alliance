/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
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
| Tool    :                     Scg                           |
|                                                             |
| File    :                  scglofig.c                       |
|                                                             |
| Date    :                   28.04.98                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include "mut.h"
# include "aut.h"
# include "mlo.h"
# include "scl.h"

# include <stdio.h>
# include "scglofig.h"

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
|                          Get Sch Figure From Lo fig         |
|                                                             |
\------------------------------------------------------------*/


schfig_list *lofig2schfig( LoFigure, Vectorized  )

  lofig_list   *LoFigure;
  int           Vectorized;
{
  locon_list    *LoCon;
  loins_list    *LoIns;
  losig_list    *LoSig;
  lotrs_list    *LoTrs;
  schcon_list   *SchCon;
  schbox_list   *SchBox;
  schnet_list   *SchNet;
  schfig_list   *SchFigure;
  ptype_list    *PType;

  SchFigure = addschfig( LoFigure->NAME );

  SchFigure->SOURCE      = (void *)LoFigure;
  SchFigure->SOURCE_TYPE = SCH_SOURCE_LOFIG;

  for ( LoSig  = LoFigure->LOSIG;
        LoSig != (losig_list *)0;
        LoSig  = LoSig->NEXT )
  {
    SchNet = addschnet( SchFigure );

    SchNet->SOURCE      = (void *)LoSig;
    SchNet->SOURCE_TYPE = SCH_SOURCE_LOSIG ;

    LoSig->USER = addptype( LoSig->USER, SCH_NET_TYPE, SchNet );
  }

  for ( LoCon  = LoFigure->LOCON;
        LoCon != (locon_list *)0;
        LoCon  = LoCon->NEXT )
  {
    if ( ( ! ( ( isvdd( LoCon->NAME ) ) || 
             ( isvss( LoCon->NAME ) ) )       ) ||
         ( LoFigure->LOTRS != (lotrs_list *)0 ) )
    {
      if ( ( LoCon->DIRECTION == IN             ) ||
           ( LoCon->DIRECTION == INOUT          ) || 
           ( LoCon->DIRECTION == UNKNOWN        ) ||
           ( LoFigure->LOTRS != (lotrs_list *)0 ) )
      {
        SchCon = addschfigcon( SchFigure, LoCon->NAME, SCH_CON_IN );

        SchCon->SOURCE      = (void *)LoCon;
        SchCon->SOURCE_TYPE = SCH_SOURCE_LOCON;

        LoSig  = LoCon->SIG;
        PType  = getptype( LoSig->USER, SCH_NET_TYPE );
        SchNet = (schnet_list *)PType->DATA;

        addschnetcon( SchNet, SchCon );
      }

      if ( ( LoCon->DIRECTION != IN              ) ||
           ( LoFigure->LOTRS  != (lotrs_list *)0 ) )
      {
        SchCon = addschfigcon( SchFigure, LoCon->NAME, SCH_CON_OUT );

        SchCon->SOURCE      = (void *)LoCon;
        SchCon->SOURCE_TYPE = SCH_SOURCE_LOCON;

        LoSig  = LoCon->SIG;
        PType  = getptype( LoSig->USER, SCH_NET_TYPE );
        SchNet = (schnet_list *)PType->DATA;

        addschnetcon( SchNet, SchCon );
      }
    }
  }

  for ( LoIns  = LoFigure->LOINS;
        LoIns != (loins_list *)0;
        LoIns  = LoIns->NEXT )
  {
    SchBox = addschbox( SchFigure, LoIns->INSNAME );
    SchBox->SOURCE = (void *)LoIns;
    SchBox->SOURCE_TYPE = SCH_SOURCE_LOINS;

    for ( LoCon  = LoIns->LOCON;
          LoCon != (locon_list *)0;
          LoCon  = LoCon->NEXT )
    {
      if ( ! ( ( isvdd( LoCon->NAME ) ) || 
               ( isvss( LoCon->NAME ) ) ) )
      {
        if ( ( LoCon->DIRECTION == IN      ) ||
             ( LoCon->DIRECTION == INOUT   ) || 
             ( LoCon->DIRECTION == UNKNOWN ) )
        {
          SchCon = addschboxcon( SchFigure, SchBox, LoCon->NAME, SCH_CON_IN );
  
          SchCon->SOURCE      = (void *)LoCon;
          SchCon->SOURCE_TYPE = SCH_SOURCE_LOCON;
  
          LoSig  = LoCon->SIG;
          PType  = getptype( LoSig->USER, SCH_NET_TYPE );
          SchNet = (schnet_list *)PType->DATA;
  
          addschnetcon( SchNet, SchCon );
        }
  
        if ( LoCon->DIRECTION != IN )
        {
          SchCon = addschboxcon( SchFigure, SchBox, LoCon->NAME, SCH_CON_OUT );
  
          SchCon->SOURCE      = (void *)LoCon;
          SchCon->SOURCE_TYPE = SCH_SOURCE_LOCON;
  
          LoSig  = LoCon->SIG;
          PType  = getptype( LoSig->USER, SCH_NET_TYPE );
          SchNet = (schnet_list *)PType->DATA;
  
          addschnetcon( SchNet, SchCon );
        }
      }
    }
  }

  for ( LoTrs  = LoFigure->LOTRS;
        LoTrs != (lotrs_list *)0;
        LoTrs  = LoTrs->NEXT )
  {
    if ( LoTrs->TRNAME != (char *)0 )
    {
      SchBox = addschbox( SchFigure, LoTrs->TRNAME );
    }
    else
    {
      SchBox = addschbox( SchFigure, ( LoTrs->TYPE == TRANSN ) ? "ntrans" : "ptrans " );
    }

    SchBox->SOURCE = (void *)LoTrs;
    SchBox->SOURCE_TYPE = SCH_SOURCE_LOTRS;
/*
** Transitor Source
*/
    LoCon = LoTrs->SOURCE;

    SchCon = addschboxcon( SchFigure, SchBox, "s", SCH_CON_IN );
    SchCon->SOURCE = (void *)LoTrs;
    SchCon->SOURCE_TYPE = SCH_SOURCE_LOTRS;

    LoSig  = LoCon->SIG;
    PType  = getptype( LoSig->USER, SCH_NET_TYPE );
    SchNet = (schnet_list *)PType->DATA;

    addschnetcon( SchNet, SchCon );
/*
** Transitor Drain 
*/
    LoCon = LoTrs->DRAIN;

    SchCon = addschboxcon( SchFigure, SchBox, "d", SCH_CON_IN );
    SchCon->SOURCE = (void *)LoTrs;
    SchCon->SOURCE_TYPE = SCH_SOURCE_LOTRS;

    LoSig  = LoCon->SIG;
    PType  = getptype( LoSig->USER, SCH_NET_TYPE );
    SchNet = (schnet_list *)PType->DATA;

    addschnetcon( SchNet, SchCon );
/*
** Transitor Grid 
*/
    LoCon = LoTrs->GRID;

    SchCon = addschboxcon( SchFigure, SchBox, "g", SCH_CON_OUT );
    SchCon->SOURCE = (void *)LoTrs;
    SchCon->SOURCE_TYPE = SCH_SOURCE_LOTRS;

    LoSig  = LoCon->SIG;
    PType  = getptype( LoSig->USER, SCH_NET_TYPE );
    SchNet = (schnet_list *)PType->DATA;

    addschnetcon( SchNet, SchCon );
  }

  for ( LoSig  = LoFigure->LOSIG;
        LoSig != (losig_list *)0;
        LoSig  = LoSig->NEXT )
  {
    LoSig->USER = delptype( LoSig->USER, SCH_NET_TYPE );
  }

  return( SchFigure );
}
