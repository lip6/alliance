/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DEFTOOLS.C                       |
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
/*------------------------------------------------------------\
|                                                             |
|                       DefTools  Fonctions                   |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         DrucChaineCompose                   |
| Accroche le rectangle source au rectangle dest par le champ |
| NEXT_DEFINE pour connaite la liste des rectangles ayant     |
| contribue a la creation du rectangle dest                   |
| on chaine des structures PlusRect entre elles a chaque      |
| nouveau rectangle composant le layer virtuel                |
| Le champ NEXT_EQUI de la structure ajoutee sert a retrouver |
| le rectangle ayant ete utilise pour composer le rectangle   |
| dest                                                        |
\------------------------------------------------------------*/

void DrucChaineCompose ( RectangleDest ,
                         RectangleSource,
                         IndexLayer
                       )

     rdsrec_list *RectangleDest;
     rdsrec_list *RectangleSource;
     int          IndexLayer;

{

DrucTypeStructPlusRect *TmpPlusRec;

  TmpPlusRec = ( DrucTypeStructPlusRect *)rdsalloc ( MACRO_PLUS_TAILLE ,
                                                     DRUC_ON_HEAP
                                                   );

  if ( MACRO_INSTANCE_NUM ( RectangleDest ) == (rdsins_list*)NULL )
  {
    MACRO_INSTANCE_NUM ( RectangleDest ) = MACRO_INSTANCE_NUM ( RectangleSource );
  }

  TmpPlusRec->NEXT_EQUI               = RectangleSource;
  TmpPlusRec->NOMBRE                  = IndexLayer;
  TmpPlusRec->NEXT_DEFINE             = MACRO_NEXT_DEFINE ( RectangleDest );
  MACRO_NEXT_DEFINE ( RectangleDest ) = TmpPlusRec;
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucInclusionRectangle              |
| Cree     la copie du rectangle 2 s'il est inclus entierement|
| dans le rectangle 1 puis chaine les 2 rectangles source     |
| par le pointeur USER au rectangle cree                      |
| ou retoune NULL                                             |
\------------------------------------------------------------*/

void DrucInclusionRectangle ( DrucFigureRds,
                              DrucRdsWindow,
                              DrucInstructionCourante,
                              Rectangle1,
                              Rectangle2
                            )

              rdsfig_list               *DrucFigureRds;
              rdswindow                 *DrucRdsWindow;
              DrucTypeStructInstruction *DrucInstructionCourante;
     register rdsrec_list               *Rectangle1;
     register rdsrec_list               *Rectangle2;

{

register int          TmpLayerDest    = DrucInstructionCourante->LAYER_DEST;
         rdsrec_list *TmpRectangleDest;

  if ( ( ( Rectangle2->X                  <=
           Rectangle1->X                     ) &&
         ( Rectangle2->X + Rectangle2->DX >=
           Rectangle1->X + Rectangle1->DX    )    ) &&
       ( ( Rectangle2->Y                  <=
           Rectangle1->Y                     ) &&
         ( Rectangle2->Y + Rectangle2->DY >=
           Rectangle1->Y + Rectangle1->DY    )    )
     )
  {
    TmpRectangleDest       = allocrdsrec( sizeof ( DrucTypeStructPlusRect ));
    TmpRectangleDest->X    = Rectangle1->X;
    TmpRectangleDest->Y    = Rectangle1->Y;
    TmpRectangleDest->DX   = Rectangle1->DX;
    TmpRectangleDest->DY   = Rectangle1->DY;

    SetRdsLayer       ( TmpRectangleDest ,
                        TmpLayerDest
                      );
    DrucChaineCompose ( TmpRectangleDest ,
                        Rectangle2,
                        DrucInstructionCourante->LAYER_2
                      );
    DrucChaineCompose ( TmpRectangleDest ,
                        Rectangle1,
                        DrucInstructionCourante->LAYER_1
                      );
    addrdsrecwindow   ( TmpRectangleDest,
                        DrucRdsWindow
                      );
    DrucAddRecFigure  ( TmpRectangleDest ,
                        DrucFigureRds ,
                        TmpLayerDest
                      );
  }
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucIntersectRectangle              |
| Cree un rectangle de dimensions et coordonnees              |
| correspondant a l'intersection des 2 rectangles passes en   |
| parametre                                                   |
| ou retoune NULL si les 2 rectangles ne sont pas en          |
| intersection                                                |
\------------------------------------------------------------*/

void DrucIntersectRectangle ( DrucFigureRds,
                              DrucRdsWindow,
                              DrucInstructionCourante,
                              Rectangle1,
                              Rectangle2
                            )
              rdsfig_list               *DrucFigureRds;

              rdswindow                 *DrucRdsWindow;
              DrucTypeStructInstruction *DrucInstructionCourante;
     register rdsrec_list               *Rectangle1;
     register rdsrec_list               *Rectangle2;

{

register long         TmpDx1          = Rectangle1->X + Rectangle1->DX;
register long         TmpDy1          = Rectangle1->Y + Rectangle1->DY;
register long         TmpDx2          = Rectangle2->X + Rectangle2->DX;
register long         TmpDy2          = Rectangle2->Y + Rectangle2->DY;
register int          TmpLayerDest    = DrucInstructionCourante->LAYER_DEST;
         rdsrec_list *TmpRectangleDest;

  if ( ( TmpDx1 < Rectangle2->X ) ||
       ( TmpDx2 < Rectangle1->X )
     )
  {
    return;
  }

  if ( ( TmpDy1 < Rectangle2->Y ) ||
       ( TmpDy2 < Rectangle1->Y )
     )
  {
    return;
  }

  TmpRectangleDest       = allocrdsrec( sizeof 
                                        ( DrucTypeStructPlusRect )
                                      );

  TmpRectangleDest->X    = ( ( Rectangle1->X < Rectangle2->X ) ?
                             Rectangle2->X :
                             Rectangle1->X
                           );

  TmpRectangleDest->Y    = ( ( Rectangle1->Y < Rectangle2->Y ) ?
                             Rectangle2->Y :
                             Rectangle1->Y
                           );

  TmpRectangleDest->DX   = ( ( TmpDx1 < TmpDx2 ) ?
                             TmpDx1 - TmpRectangleDest->X :
                             TmpDx2 - TmpRectangleDest->X
                           );
  TmpRectangleDest->DY   = ( ( TmpDy1 < TmpDy2 ) ?
                             TmpDy1 - TmpRectangleDest->Y :
                             TmpDy2 - TmpRectangleDest->Y
                           );
  SetRdsLayer       ( TmpRectangleDest ,
                      TmpLayerDest
                    );
  DrucChaineCompose ( TmpRectangleDest ,
                      Rectangle1,
                      DrucInstructionCourante->LAYER_1
                    );
  DrucChaineCompose ( TmpRectangleDest ,
                      Rectangle2,
                      DrucInstructionCourante->LAYER_2
                    );
  addrdsrecwindow   ( TmpRectangleDest ,
                      DrucRdsWindow
                    );
  DrucAddRecFigure  ( TmpRectangleDest ,
                      DrucFigureRds ,
                      TmpLayerDest
                    );
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucExclusionRectangle              |
| Retourne la copie du rectangle 2 si il n'est pas en         |
| intersection ou en contact avec le rectangle 1              |
| ou retoune NULL                                             |
\------------------------------------------------------------*/

void DrucExclusionRectangle ( DrucFigureRds,
                              DrucRdsWindow,
                              TmpRectangle1,
                              DrucInstructionCourante,
                              UnaireX,
                              UnaireDX,
                              UnaireY,
                              UnaireDY
                            )
         rdsfig_list               *DrucFigureRds;
         rdswindow                 *DrucRdsWindow;
         DrucTypeStructInstruction *DrucInstructionCourante;
register rdsrec_list               *TmpRectangle1;
register long                       UnaireX;
register long                       UnaireDX;
register long                       UnaireY;
register long                       UnaireDY;


{

register rdsrec_list *TmpRectangleDest;
register int          TmpLayerDest    = DrucInstructionCourante->LAYER_DEST;
register int          TmpLayerSource  = DrucInstructionCourante->LAYER_1;

  TmpRectangleDest       = allocrdsrec( sizeof ( DrucTypeStructPlusRect ));
  TmpRectangleDest->X    = UnaireX ;
  TmpRectangleDest->Y    = UnaireY ;
  TmpRectangleDest->DX   = UnaireDX ;
  TmpRectangleDest->DY   = UnaireDY ;

  SetRdsLayer       ( TmpRectangleDest ,
                      TmpLayerDest
                    );
  DrucChaineCompose ( TmpRectangleDest ,
                      TmpRectangle1,
                      DrucInstructionCourante->LAYER_1
                    );
  addrdsrecwindow   ( TmpRectangleDest ,
                      DrucRdsWindow
                    );
  DrucAddRecFigure  ( TmpRectangleDest ,
                      DrucFigureRds ,
                      TmpLayerDest
                    );
/*\
DrucViewString ("\nCREATION DE RECTANGLE EXCLUS\n"
               );
viewrdsrec (TmpRectangleDest
           );
\*/
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucMarqueInclusion                 |
| Marque le rectangle 1 si il est en contact avec             |
|      le rectangle 2                                         |
\------------------------------------------------------------*/

void DrucMarqueInclusion ( UnaireX,
                           UnaireDX,
                           UnaireY,
                           UnaireDY,
                           Rectangle1,
                           Rectangle2
                         )
register long         UnaireX;
register long         UnaireDX;
register long         UnaireY;
register long         UnaireDY;
register rdsrec_list *Rectangle1;
register rdsrec_list *Rectangle2;

{


register long TmpDx2 = Rectangle2->X + Rectangle2->DX;
register long TmpDy2 = Rectangle2->Y + Rectangle2->DY;

  if ( ( UnaireX + UnaireDX             < Rectangle2->X ) ||
       ( Rectangle2->X + Rectangle2->DX < UnaireX )
     )
  {
    return;
  }

  if ( ( UnaireY + UnaireDY             < Rectangle2->Y ) ||
       ( Rectangle2->Y + Rectangle2->DY < UnaireY )
     )
  {
    return;
  }

  Rectangle1->FLAGS |= DRUC_FLAG_REC_INCLUS;
}

/*------------------------------------------------------------\
|                                                             |
|                         DrucCalculOperationUnaire           |
| Retourne les dimensions d'un rectangle ayant subi l'ope     |
| ration unaire contenue dans la struct instruction           |
\------------------------------------------------------------*/
void DrucCalculOperationUnaire ( DrucRdsWindow,
                                 DrucInstructionCourante,
                                 TmpRectangleSource,
                                 UnaireX,
                                 UnaireDX,
                                 UnaireY,
                                 UnaireDY
                              )
rdswindow                 *DrucRdsWindow;
DrucTypeStructInstruction *DrucInstructionCourante;
rdsrec_list               *TmpRectangleSource;
long                      *UnaireX;
long                      *UnaireDX;
long                      *UnaireY;
long                      *UnaireDY;

{

  if ( DrucInstructionCourante->OP_UNAIRE == DRUC_UNDEFINED_OP_COMPOSE )
  {
    *UnaireX  = TmpRectangleSource->X;
    *UnaireDX = TmpRectangleSource->DX;
    *UnaireY  = TmpRectangleSource->Y;
    *UnaireDY = TmpRectangleSource->DY;
    return;
  }

  switch ( DrucInstructionCourante->OP_UNAIRE )
  {
  case DRUC_COMPOSE_RESIZE :
    {
      /*\
       *  modif des dimensions X
      \*/

      if ( ( DrucInstructionCourante->DEF_VALEUR < 0 ) &&
           ( TmpRectangleSource->DX < abs ( DrucInstructionCourante->DEF_VALEUR ) * 2 )
         )
      {
        *UnaireX    = TmpRectangleSource->X + ( TmpRectangleSource->DX / 2 );
        *UnaireDX   = 0;
      }
      else 
      {
        *UnaireX    = ( ( TmpRectangleSource->X - 
                          DrucInstructionCourante->DEF_VALEUR ) < 
                          DrucRdsWindow->XMIN ?
                          DrucRdsWindow->XMIN :
                          TmpRectangleSource->X - 
                          DrucInstructionCourante->DEF_VALEUR 
                     );
        *UnaireDX   = ( ( TmpRectangleSource->X + 
                          TmpRectangleSource->DX + 
                          DrucInstructionCourante->DEF_VALEUR ) >
                          DrucRdsWindow->XMAX ?
                          DrucRdsWindow->XMAX - *UnaireX :
                          TmpRectangleSource->X +
                          TmpRectangleSource->DX + 
                          DrucInstructionCourante->DEF_VALEUR -
                          *UnaireX
                     );
      }
      /*\
       *  modif des dimensions Y
      \*/

      if ( ( DrucInstructionCourante->DEF_VALEUR < 0 ) &&
           ( TmpRectangleSource->DY < abs ( DrucInstructionCourante->DEF_VALEUR ) * 2 )
         )
      {
        *UnaireY    = TmpRectangleSource->Y + ( TmpRectangleSource->DY / 2 );
        *UnaireDY   = 0;
      }
      else 
      {
        *UnaireY    = ( ( TmpRectangleSource->Y - 
                          DrucInstructionCourante->DEF_VALEUR ) <
                          DrucRdsWindow->YMIN ?
                          DrucRdsWindow->YMIN :
                          TmpRectangleSource->Y - 
                          DrucInstructionCourante->DEF_VALEUR
                     );
        *UnaireDY   = ( ( TmpRectangleSource->Y +
                          TmpRectangleSource->DY + 
                          DrucInstructionCourante->DEF_VALEUR ) >
                          DrucRdsWindow->YMAX ?
                          DrucRdsWindow->YMAX - *UnaireY :
                          TmpRectangleSource->Y +
                          TmpRectangleSource->DY + 
                          DrucInstructionCourante->DEF_VALEUR -
                          *UnaireY
                     );
      }
    }
  }
}

