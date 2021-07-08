/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DEFMINMAX.C                      |
|                                                             |
| Authors :                Naohiko Shimizu                    |
|                                                             |
| Date    :                   04/03/20                        |
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
#include "defresiz.h"
/*------------------------------------------------------------\
|                                                             |
|                       DefMinMax  Fonctions                   |
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

void DrucDefineMinMax ( DrucFigureRds,
                        DrucRdsWindow ,
                        DrucInstructionCourante
                      )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucInstructionCourante;

{

         int          TmpLayerDest;
         int          TmpLayerSource1;
register rdsrec_list *TmpRectangleSource;
register rdsrec_list *TmpRectangleDest;
	long  MesureMax, MesureMin;
	long  Value;

  TmpLayerDest    = DrucInstructionCourante->LAYER_DEST;
  TmpLayerSource1 = DrucInstructionCourante->LAYER_1;
  Value = DrucInstructionCourante->DEF_VALEUR;
  /*\
   * modifier tous les rectangles du layer source 1
  \*/

    for ( TmpRectangleSource  = DrucFigureRds->LAYERTAB[ TmpLayerSource1 ];
          TmpRectangleSource != (rdsrec_list *)NULL;
          TmpRectangleSource  = TmpRectangleSource->NEXT
        )
    {
      /*\
       *  modif des dimensions X
      \*/
      MesureMax = ( TmpRectangleSource->DX > TmpRectangleSource->DY ?
                  TmpRectangleSource->DX :
                  TmpRectangleSource->DY
                  );
      MesureMin = ( TmpRectangleSource->DX < TmpRectangleSource->DY ?
                  TmpRectangleSource->DX :
                  TmpRectangleSource->DY
                  );
        switch ( DrucInstructionCourante->OP_COMPOSE )
		  {
	  case DRUC_MESURE_LARGEUR_MIN:
		  if(! (MesureMin > Value)) continue;
		  break;
	  case DRUC_MESURE_LARGEUR_MAX:
		  if(! (MesureMin < Value)) continue;
		  break;
	  case DRUC_MESURE_LONGUEUR_MIN:
		  if(! (MesureMax > Value)) continue;
		  break;
	  case DRUC_MESURE_LONGUEUR_MAX:
		  if(! (MesureMax < Value)) continue;
		  break;
	  defualt:
		    DRUC_EXIT( DRUC_ERROR_UNDEFINED );
		  }

//	printf("\n1:X,Y=%d,%d, DX,DY = %d,%dM:V=%d:%d, %s\n",TmpRectangleSource->X, TmpRectangleSource->Y,TmpRectangleSource->DX, TmpRectangleSource->DY, MesureMin, Value, TmpRectangleSource->NAME);
      TmpRectangleDest       = allocrdsrec ( sizeof ( DrucTypeStructPlusRect ));
      TmpRectangleDest->NEXT = ( rdsrec_list *)NULL;
      TmpRectangleDest->USER = (rdsrec_list *)NULL;
      TmpRectangleDest->NAME = TmpRectangleSource->NAME;
      TmpRectangleDest->X    = TmpRectangleSource->X   ;
      TmpRectangleDest->Y    = TmpRectangleSource->Y   ;
      TmpRectangleDest->DX   = TmpRectangleSource->DX  ;
      TmpRectangleDest->DY   = TmpRectangleSource->DY  ;
  
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
}
