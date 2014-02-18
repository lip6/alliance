/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DEFDEFIN.C                       |
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
#include "vmcunify.h"
#include "deftools.h"
#include "defunion.h"
#include "definter.h"
#include "defexclu.h"
#include "definclu.h"
#include "defresiz.h"
#include "defdefin.h"
/*------------------------------------------------------------\
|                                                             |
|                       DefDefin  Fonctions                   |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                         DrucDefine                          |
| Definit un layer temporaire caracterise par les parametres  |
| contenus dans la StructInstruction pointee par son numero   |
| puis chaine le layer en equipotentielles                    |
\------------------------------------------------------------*/

void DrucDefine ( DrucFigureRds,
                  DrucRdsWindow ,
                  DrucInstructionCourante
                )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucInstructionCourante;

{

/*\
 * DrucViewInstruction ( DrucInstructionCourante );
\*/
  switch ( DrucInstructionCourante->OP_COMPOSE )
  {
  case DRUC_COMPOSE_UNION :
    {
      DrucDefineUnion ( DrucFigureRds,
                        DrucRdsWindow ,
                        DrucInstructionCourante
                      );
      break;
    }
  case DRUC_COMPOSE_INTERSECT :
    {
      DrucDefineIntersection ( DrucFigureRds,
                               DrucRdsWindow,
                               DrucInstructionCourante
                             );
      break;
    }
  case DRUC_COMPOSE_EXCLUSION :
    {
      DrucDefineExclusion ( DrucFigureRds,
                            DrucRdsWindow,
                            DrucInstructionCourante
                          );
/*\
DrucViewString ("\nfin EXCLUSION\n");
viewrdsfig ( DrucFigureRds
           );
DrucViewString ("\n");
\*/
      break;
    }
  case DRUC_COMPOSE_RESIZE :
    {
      DrucDefineResize ( DrucFigureRds,
                         DrucRdsWindow,
                         DrucInstructionCourante
                       );

/*\
DrucViewString ("\nfin RESIZE\n");
viewrdsfig ( DrucFigureRds
           );
DrucViewString ("\n");
saverdsfig ( DrucFigureRds );
\*/

      break;
    }
  case DRUC_COMPOSE_INCLUSION :
    {
      DrucDefineInclusion ( DrucFigureRds,
                            DrucRdsWindow,
                            DrucInstructionCourante
                          );
/*\
DrucViewString ("\nfin INCLUSION\n");
viewrdsfig ( DrucFigureRds
           );
DrucViewString ("\n");
\*/
      break;
    }
  }/* switch */

  DrucComputeEquiLayer ( DrucFigureRds ,
                         DrucInstructionCourante->LAYER_DEST
                       );
/*\
DrucViewString      (" DEFINE: COMPUTE EQUI AVANT UNIFICATION \n"
                    );
DrucVerifEquiLayer  ( DrucFigureRds,
                      DrucInstructionCourante->LAYER_DEST
                    );
DrucViewString      (" DEFINE: FIN COMPUTE EQUI AVANT UNIFICATION \n"
                    );
\*/


/*\
 * Patrick le 22/03/94 la largeur min d'un layer construit ne peut pas etre 
 * le max des largeurs des layers sources. Il pourrait etre le min des 2 
 * mais en cas d,intersection on pourrait avoir des rectangles de taille 
 * inferieure 
 * L'unificateur supprime tous les rectangles de taille inf au min donc on met 0
 *
 *  DrucStructRdsLayer [ DrucInstructionCourante->LAYER_DEST ].WIDTH = 
 *          ( DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].WIDTH > 
 *            DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].WIDTH ? 
 *            DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].WIDTH :
 *            DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].WIDTH 
 *          );
\*/
  DrucStructRdsLayer [ DrucInstructionCourante->LAYER_DEST ].WIDTH = 0;

  DrucStructRdsLayer [ DrucInstructionCourante->LAYER_DEST ].CTM = 
          ( DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].CTM > 
               DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].CTM ? 
            DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].CTM :
            DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].CTM 
          );

  DrucUnifyLayer         ( DrucFigureRds,
                           DrucRdsWindow,
                           DrucInstructionCourante->LAYER_DEST
                         );
  DrucDestroyLayerWindow ( DrucFigureRds,
                           DrucRdsWindow,
                           DrucInstructionCourante->LAYER_DEST
                         );
  DrucBuiltLayerWindow   ( DrucFigureRds,
                           DrucRdsWindow,
                           DrucInstructionCourante->LAYER_DEST
                         ); 
/*\
 *  On reconstruit les equi car l'unificateur a modifie les rectangles
 *  il en a modifie, detruit et cree
\*/
  DrucComputeEquiLayer   ( DrucFigureRds ,
                           DrucInstructionCourante->LAYER_DEST
                         );
/*\
viewrdsfig ( DrucFigureRds
           );
DrucViewString     ("\nDEFINE: FIN COMPUTE EQUI APRES UNIFICATION \n");
DrucViewString     (" DEFINE: COMPUTE EQUI APRES UNIFICATION \n"
                   );
DrucViewString     ( RDS_LAYER_NAME[DrucInstructionCourante->LAYER_1]
                   );
DrucViewString     (": c'est le Layer source \n");
DrucViewString     ( RDS_LAYER_NAME[DrucInstructionCourante->LAYER_2]
                   );
DrucViewString     (": c'est le Layer source \n");

DrucVerifEquiLayer ( DrucFigureRds,
                     DrucInstructionCourante->LAYER_DEST
                   );
DrucViewString     (" DEFINE: FIN COMPUTE EQUI APRES UNIFICATION \n");
\*/
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucUndefine                        |
| Detruit le layer temporaire caracterise par les parametres  |
| contenus dans la StructInstruction pointee par son numero   |
| on parcourt tous les rectangles du layer defini dans la     |
|   fenetre et on les remet dans le tas gere par rdswin       |
\------------------------------------------------------------*/

void DrucUndefine ( DrucFigureRds ,
                    DrucRdsWindow,
                    DrucInstructionCourante
                  )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucInstructionCourante;

{

register rdsrec_list            *TmpRectangleDest;
register rdsrec_list            *TmpRectangleSuivant;
         DrucTypeStructPlusRect *TmpPlusRec;
         DrucTypeStructPlusRect *TmpPlusRecSuivant;
  /*\
   * detruire la window associee au layer
  \*/
  for ( TmpRectangleDest  = DrucFigureRds->LAYERTAB[ DrucInstructionCourante->LAYER_1 ];
        TmpRectangleDest != (rdsrec_list *)NULL;
        TmpRectangleDest  = TmpRectangleDest->NEXT 
      )
  {
    delrdsrecwindow ( TmpRectangleDest, 
                      DrucRdsWindow 
                    );
  }
  /*\
   * detruire les rectangles du layer
  \*/
  TmpRectangleDest  = DrucFigureRds->LAYERTAB[ DrucInstructionCourante->LAYER_1 ];

  while ( TmpRectangleDest != (rdsrec_list *)NULL 
        )
  {
    if ( MACRO_NEXT_DEFINE ( TmpRectangleDest ) != ( DrucTypeStructPlusRect *)NULL
       )
    {
      /*\
       * destruction des StructPlusRect du rectangle scourant
      \*/
      TmpPlusRec = MACRO_NEXT_DEFINE ( TmpRectangleDest );

      while ( TmpPlusRec != ( DrucTypeStructPlusRect *)NULL
            )
      {
        TmpPlusRecSuivant = TmpPlusRec->NEXT_DEFINE;

        rdsfreeheap ( TmpPlusRec ,
                  MACRO_PLUS_TAILLE
                );

        TmpPlusRec = TmpPlusRecSuivant;
      }
    }
    TmpRectangleSuivant = TmpRectangleDest->NEXT;

      /*\
       * destruction du rectangle courant dans la fenetre
      \*/
    delrdsrecwindow ( TmpRectangleDest,
                      DrucRdsWindow
                    );
      /*\
       * destruction du rectangle courant dans la figure
      \*/
    freerdsrec ( TmpRectangleDest,
                 sizeof ( DrucTypeStructPlusRect )
               );

    TmpRectangleDest = TmpRectangleSuivant;
  }

  DrucFigureRds->LAYERTAB[ DrucInstructionCourante->LAYER_1 ] = 
  
      (rdsrec_list *)NULL;
}
/*
 * algo de NotInclus:
 * 
 * Pour tout rectangle a
 *  si a n,est pas marque
 *   alors
 *    Pour tout rectangle b
 *      si a est entierement inclus dans b
 *       alors
 *        je le marque
 *      fsi
 *   fpour b
 *  fsi
 *  si a n'est pas marque 
 *   alors 
 *     je le met dans le layer utilisateur
 * fpour a
 */
/*------------------------------------------------------------\
|                                                             |
|                         VMCDEFIN MODIFS                     |
|                                                             |
| PATRICK 22/03/94: ligne 739                                 |
|   mise a 0 du with min des layers construits                |
|                                                             |
|                                                             |
\------------------------------------------------------------*/
