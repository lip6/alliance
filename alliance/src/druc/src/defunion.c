/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DEFUNION.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   26/07/94                        |
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
#include "deftools.h"
#include "defunion.h"
/*------------------------------------------------------------\
|                                                             |
|                       DefUnion  Fonctions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrucDefineUnion                     |
| Definit un layer temporaire caracterise par l'union         |
| des deux layers source                                      |
| range le resultat dans le tableau des layers user defini par|
| l'instruction                                               |
\------------------------------------------------------------*/

void DrucDefineUnion ( DrucFigureRds,
                       DrucRdsWindow ,
                       DrucInstructionCourante
                     )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucInstructionCourante;

{

         int          TmpLayerDest;
         int          TmpLayerSource1;
         int          TmpLayerSource2;
register rdsrec_list *TmpRectangleSource;
register rdsrec_list *TmpRectangleDest;

  TmpLayerDest    = DrucInstructionCourante->LAYER_DEST;
  TmpLayerSource1 = DrucInstructionCourante->LAYER_1;
  TmpLayerSource2 = DrucInstructionCourante->LAYER_2;
  /*\
   * dupliquer tous les rectangles du layer source 1
  \*/
  for ( TmpRectangleSource  = DrucFigureRds->LAYERTAB[ TmpLayerSource1 ];
        TmpRectangleSource != (rdsrec_list *)NULL;
        TmpRectangleSource  = TmpRectangleSource->NEXT
      )
  {
    TmpRectangleDest         = allocrdsrec ( sizeof ( DrucTypeStructPlusRect ));
    TmpRectangleDest->NEXT   = ( rdsrec_list *)NULL;
    TmpRectangleDest->X      = TmpRectangleSource->X;
    TmpRectangleDest->Y      = TmpRectangleSource->Y;
    TmpRectangleDest->DX     = TmpRectangleSource->DX;
    TmpRectangleDest->DY     = TmpRectangleSource->DY;
    TmpRectangleDest->USER   = ( rdsrec_list *)NULL;
    TmpRectangleDest->NAME   = TmpRectangleSource->NAME;

    SetRdsLayer       ( TmpRectangleDest ,
                        TmpLayerDest
                      );
    MACRO_NEXT_EQUI   ( TmpRectangleDest ) = (rdsrec_list *)NULL;
    DrucChaineCompose ( TmpRectangleDest ,
                        TmpRectangleSource,
                        TmpLayerSource1
                      );
    addrdsrecwindow   ( TmpRectangleDest ,
                        DrucRdsWindow
                      );
    DrucAddRecFigure  ( TmpRectangleDest ,
                        DrucFigureRds ,
                        TmpLayerDest
                      );
  }
  /*\
   * dupliquer tous les rectangles du layer source 2
  \*/

  for ( TmpRectangleSource  = DrucFigureRds->LAYERTAB[ TmpLayerSource2 ];
        TmpRectangleSource != (rdsrec_list *)NULL;
        TmpRectangleSource  = TmpRectangleSource->NEXT
      )
  {
    TmpRectangleDest       = allocrdsrec ( sizeof ( DrucTypeStructPlusRect ));
    TmpRectangleDest->NEXT = ( rdsrec_list *)NULL;
    TmpRectangleDest->X    = TmpRectangleSource->X;
    TmpRectangleDest->Y    = TmpRectangleSource->Y;
    TmpRectangleDest->DX   = TmpRectangleSource->DX;
    TmpRectangleDest->DY   = TmpRectangleSource->DY;
    TmpRectangleDest->USER = (rdsrec_list *)NULL;
    TmpRectangleDest->NAME = TmpRectangleSource->NAME;

    SetRdsLayer       ( TmpRectangleDest ,
                        TmpLayerDest
                      );

MACRO_INSTANCE_NUM ( TmpRectangleDest ) = MACRO_INSTANCE_NUM ( TmpRectangleSource );

    MACRO_NEXT_EQUI   ( TmpRectangleDest ) = (rdsrec_list *)NULL;
    DrucChaineCompose ( TmpRectangleDest ,
                        TmpRectangleSource,
                        TmpLayerSource2
                      );
    addrdsrecwindow   ( TmpRectangleDest ,
                        DrucRdsWindow
                      );
    DrucAddRecFigure  ( TmpRectangleDest ,
                        DrucFigureRds ,
                        TmpLayerDest
                      );
  }
}
