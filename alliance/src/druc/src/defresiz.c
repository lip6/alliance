/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DEFRESIZ.C                       |
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
#include "defresiz.h"
/*------------------------------------------------------------\
|                                                             |
|                       DefResiz  Fonctions                   |
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

void DrucDefineResize ( DrucFigureRds,
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
   * modifier tous les rectangles du layer source 1
  \*/

  if ( TmpLayerSource1 == TmpLayerDest )
  {
    for ( TmpRectangleSource  = DrucFigureRds->LAYERTAB[ TmpLayerSource1 ];
          TmpRectangleSource != (rdsrec_list *)NULL;
          TmpRectangleSource  = TmpRectangleSource->NEXT
        )
    {
      /*\
       *  modif des dimensions X
      \*/
/*\
printf ("\nTmpRectangleSource %d %d %d %d\n",
           TmpRectangleSource->X,
           TmpRectangleSource->Y,
           TmpRectangleSource->DX,
           TmpRectangleSource->DY);
\*/

      if ( ( DrucInstructionCourante->DEF_VALEUR < 0 ) &&
           ( TmpRectangleSource->DX < abs ( DrucInstructionCourante->DEF_VALEUR ) * 2 )  
         )
      {
        TmpRectangleSource->X    = TmpRectangleSource->X + ( TmpRectangleSource->DX / 2 );
        TmpRectangleSource->DX   = 0;
      }
      else 
      {
        TmpRectangleSource->X    = TmpRectangleSource->X - 
                                   DrucInstructionCourante->DEF_VALEUR;

        TmpRectangleSource->DX   = TmpRectangleSource->DX + 
                                   (DrucInstructionCourante->DEF_VALEUR * 2);
      }

      /*\
       *  modif des dimensions Y
      \*/
      if ( ( DrucInstructionCourante->DEF_VALEUR < 0 ) &&
           ( TmpRectangleSource->DY < abs ( DrucInstructionCourante->DEF_VALEUR ) * 2 )
         )
      {
        TmpRectangleSource->Y    = TmpRectangleSource->Y + ( TmpRectangleSource->DY / 2 );
        TmpRectangleSource->DY   = 0;
      }
      else 
      {
        TmpRectangleSource->Y    = TmpRectangleSource->Y - 
                                   DrucInstructionCourante->DEF_VALEUR;

        TmpRectangleSource->DY   = TmpRectangleSource->DY + 
                                   (DrucInstructionCourante->DEF_VALEUR * 2);
      }
/*\
printf ("TmpRectangleSource %d %d %d %d\n",
           TmpRectangleSource->X,
           TmpRectangleSource->Y,
           TmpRectangleSource->DX,
           TmpRectangleSource->DY);
\*/

    }
  }
  else
  /*\
   * dupliquer tous les rectangles du layer source 1
  \*/

  {
    for ( TmpRectangleSource  = DrucFigureRds->LAYERTAB[ TmpLayerSource1 ];
          TmpRectangleSource != (rdsrec_list *)NULL;
          TmpRectangleSource  = TmpRectangleSource->NEXT
        )
    {
      TmpRectangleDest       = allocrdsrec ( sizeof ( DrucTypeStructPlusRect ));
      TmpRectangleDest->NEXT = ( rdsrec_list *)NULL;
      /*\
       *  modif des dimensions X
      \*/
      if ( ( DrucInstructionCourante->DEF_VALEUR < 0 ) &&
           ( TmpRectangleSource->DX < abs ( DrucInstructionCourante->DEF_VALEUR ) * 2 )  
         )
      {
        TmpRectangleDest->X    = TmpRectangleSource->X + ( TmpRectangleSource->DX / 2 );
        TmpRectangleDest->DX   = 0;
      }
      else 
      {
        TmpRectangleDest->X    = TmpRectangleSource->X - 
                                 DrucInstructionCourante->DEF_VALEUR;

        TmpRectangleDest->DX   = TmpRectangleSource->DX + 
                                 (DrucInstructionCourante->DEF_VALEUR * 2);
      }

      /*\
       *  modif des dimensions Y
      \*/
      if ( ( DrucInstructionCourante->DEF_VALEUR < 0 ) &&
           ( TmpRectangleSource->DY < abs ( DrucInstructionCourante->DEF_VALEUR ) * 2 )
         )
      {
        TmpRectangleDest->Y    = TmpRectangleSource->Y + ( TmpRectangleSource->DY / 2 );
        TmpRectangleDest->DY   = 0;
      }
      else 
      {
        TmpRectangleDest->Y    = TmpRectangleSource->Y - 
                                 DrucInstructionCourante->DEF_VALEUR;

        TmpRectangleDest->DY   = TmpRectangleSource->DY + 
                                 (DrucInstructionCourante->DEF_VALEUR * 2);
      }
      TmpRectangleDest->USER = (rdsrec_list *)NULL;
      TmpRectangleDest->NAME = TmpRectangleSource->NAME;
  
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
}
