/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DEFINCLU.C                       |
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
#include "definclu.h"
/*------------------------------------------------------------\
|                                                             |
|                       DefInclu Fonctions                    |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrucDefineInclusion                 |
| Definit un layer temporaire caracterise par l'operateur     |
| contenu dans l'instruction ayant les deux layers source     |
| comme operandes                                             |
| range le resultat dans le tableau des layers userdefini par |
| l'instruction                                               |
\------------------------------------------------------------*/

void DrucDefineInclusion ( DrucFigureRds,
                           DrucRdsWindow,
                           DrucInstructionCourante
                         )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucInstructionCourante;

{

         rdsrec_list     *TmpRectangleSource;
         rdsrecwin_list  *TmpWindowSource2;
register int              TabIndex2;
register rdswinrec_list  *TabRectangleSource2 = NULL;

  for ( TmpRectangleSource  = DrucFigureRds->LAYERTAB [ DrucInstructionCourante->LAYER_1 ];
        TmpRectangleSource != (rdsrec_list *)NULL;
        TmpRectangleSource  = TmpRectangleSource->NEXT
      )
  {
   /*\
    *  recuperer toutes les fenetres liees au rectangle, marquer le rectangle visite
   \*/
    TmpWindowSource2 = DrucChercheWindow ( TmpRectangleSource
                                         );
    /*\
     * parcours de chaque fenetre
    \*/
    while ( TmpWindowSource2 != (rdsrecwin_list *)NULL
          )
    {
      TabRectangleSource2 = ( TmpWindowSource2->WINDOW )->LAYERTAB
                                 [ DrucInstructionCourante->LAYER_2 ];
      /*\
       * parcours des tableaux de la fenetre du layer 1
      \*/
      while ( TabRectangleSource2 != (rdswinrec_list *)NULL
            )
      {
        /*\
         * parcourir tous les rectangles du layer 1
        \*/
        TabIndex2 = 0;

        while ( TabIndex2 < RWI_MAX_REC
              )
        {
          if ( MACRO_RECT_SOURCE2 [ TabIndex2 ] != ( rdsrec_list *)NULL )
          {
            DrucInclusionRectangle ( DrucFigureRds,
                                     DrucRdsWindow,
                                     DrucInstructionCourante,
                                     TmpRectangleSource,
                                     MACRO_RECT_SOURCE2 [ TabIndex2 ]
                                   );
          }

          TabIndex2 ++;
        } /* while les rectangles d'un tableau du layer 2 */
        TabRectangleSource2 = TabRectangleSource2->NEXT;
      } /* while les tableaux du layer 2 */
      TmpWindowSource2 = TmpWindowSource2->NEXT;
    } /* while les fenetres du layer 2 */
  } /* for des rectangles du layer 1 */
}
