/*-----------------------------------------------------------*\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  VMCRELAT.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   11/01/94                        |
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
#include <math.h>
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
#include "vmcerror.h"
#include "druchier.h"
#include "vmcasmld.h"
#include "vmcmesur.h"
#include "vmcrelat.h"
/*------------------------------------------------------------\
|                                                             |
|                          Fonctions                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationDilateRectangle        |
|                                                             |
\------------------------------------------------------------*/
void  DrucRelationDilateRectangle ( DrucRdsWindow,
                                    DrucInstructionCourante,
                                    RectangleOrigine,
                                    DilateX,
                                    DilateY,
                                    DilateDX,
                                    DilateDY
                                  )
      rdswindow                 *DrucRdsWindow;
      DrucTypeStructInstruction *DrucInstructionCourante;
      rdsrec_list               *RectangleOrigine;
      long                      *DilateX;
      long                      *DilateY;
      long                      *DilateDX;
      long                      *DilateDY;
{
int IndexMesure;
long DistanceInfluence;
long Ctm1;
long Ctm2;
long PositionDx = RectangleOrigine->X + RectangleOrigine->DX; 
long PositionDy = RectangleOrigine->Y + RectangleOrigine->DY;

  IndexMesure = 0;
  while ( ( DrucInstructionCourante->RELATION [ IndexMesure ] != 
            DRUC_UNDEFINED_MESURE 
          ) &&
          ( IndexMesure != DRUC_MAX_MESURES
          )
        )
  {
    if ( DrucInstructionCourante->RELATION [ IndexMesure ] == 
         DRUC_RELATION_DISTANCE 
       ) 
    {
      Ctm1 = DrucStructRdsLayer [ DrucInstructionCourante->LAYER_1 ].CTM;
      Ctm2 = DrucStructRdsLayer [ DrucInstructionCourante->LAYER_2 ].CTM;
      DistanceInfluence = ( Ctm1 > Ctm2 ?
                            Ctm1 :
                            Ctm2
                          );
      *DilateX = ( ( RectangleOrigine->X  - DistanceInfluence ) < 
                             DrucRdsWindow->XMIN ?
                             DrucRdsWindow->XMIN :
                             RectangleOrigine->X  - DistanceInfluence
                           );
      *DilateY = ( ( RectangleOrigine->Y  - DistanceInfluence ) < 
                             DrucRdsWindow->YMIN ?
                             DrucRdsWindow->YMIN :
                             RectangleOrigine->Y  - DistanceInfluence
                           );
      *DilateDX = ( ( PositionDx + DistanceInfluence      ) > 
                              DrucRdsWindow->XMAX ?
                              DrucRdsWindow->XMAX - *DilateX :
                              PositionDx +  DistanceInfluence - *DilateX
                            ); 
      *DilateDY = ( ( PositionDy + DistanceInfluence      ) > 
                              DrucRdsWindow->YMAX ?
                              DrucRdsWindow->YMAX - *DilateY :
                              PositionDy +  DistanceInfluence - *DilateY
                            ); 
      return;
    }
    IndexMesure ++;
  }
  *DilateX  = RectangleOrigine->X;
  *DilateY  = RectangleOrigine->Y;
  *DilateDX = RectangleOrigine->DX;
  *DilateDY = RectangleOrigine->DY;
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucCompareDistance                |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareDistance ( DrucStructMesure,
                                   DrucInstructionCourante,
                                   IndexMesure
                                 )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
long TmpHypotenuse;

  if ( ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_MARGE     ) ||
       ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_EXTENSION )
     )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_AXIALE :
    case DRUC_MESURE_GEODESIQUE :
      {
        DrucRelationTesteDistance ( DrucStructMesure,
                                    DrucInstructionCourante,
                                    IndexMesure,
                                    MACRO_LONG_DISTANCE
                                  );
        break;
      }
    } /* switch */
  } /* if orientation */
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_INTERSECTION
     )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_AXIALE :
      {
        DrucRelationTesteDistance ( DrucStructMesure,
                                    DrucInstructionCourante,
                                    IndexMesure,
                                    MACRO_LONG_DISTANCE
                                  );
        break;
      }
    case DRUC_MESURE_GEODESIQUE :
      {
        TmpHypotenuse = (long) sqrt ( ( MACRO_LONG_DISTANCE * MACRO_LONG_DISTANCE )  +
                                      ( MACRO_LARGE_DISTANCE * MACRO_LARGE_DISTANCE )
                                    );
        DrucRelationTesteDistance ( DrucStructMesure,
                                    DrucInstructionCourante,
                                    IndexMesure,
                                    TmpHypotenuse
                                  );
        break;
      }
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucCompareEnveloppe               |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareEnveloppe ( DrucStructMesure,
                                    DrucInstructionCourante,
                                    IndexMesure
                                  )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_ENVELOPPE )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_SURFACE_INTER :
      {
        DrucMesureInter ( DrucStructMesure,   
                          DrucInstructionCourante,
                          IndexMesure,
                          MACRO_SURFACE_INTER
                        );
        break;
      }
    case DRUC_MESURE_LONGUEUR_INTER :
      {
        DrucMesureInter ( DrucStructMesure,
                          DrucInstructionCourante,
                          IndexMesure,
                          MACRO_LONG_INTER
                        );
        break;
      }
    case DRUC_MESURE_LARGEUR_INTER :
      {
        DrucMesureInter ( DrucStructMesure,
                          DrucInstructionCourante,
                          IndexMesure,
                          MACRO_LARGE_INTER
                        );
        break;
      }
    case DRUC_MESURE_LONGUEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MIN,
                            MACRO_RECT_LONG_MIN
                          );
        break;
      }
    case DRUC_MESURE_LONGUEUR_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MAX,
                            MACRO_RECT_LONG_MAX
                          );
        break;
      }
    case DRUC_MESURE_LARGEUR_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LARGE_MAX,
                            MACRO_RECT_LARGE_MAX
                          );
        break;
      }
    case DRUC_MESURE_LARGEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LARGE_MIN,
                            MACRO_RECT_LARGE_MIN
                          );
        break;
      }
    case DRUC_MESURE_SUP :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_SUP,
                            MACRO_RECT_SUP
                          );
        break;
      }
    case DRUC_MESURE_INF :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_INF,
                            MACRO_RECT_INF
                          );
        break;
      }
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationCompareMarge           |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareMarge ( DrucStructMesure,
                                DrucInstructionCourante,
                                IndexMesure
                         )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_MARGE )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_SURFACE_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure,    
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_SURFACE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LONG_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LARGEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LARGE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_PENETRE_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_PENETRE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MAX,
                            MACRO_RECT_LONG_MAX
                          );
        break;
      }
    case DRUC_MESURE_LONGUEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MIN,
                            MACRO_RECT_LONG_MIN
                          );
        break;
      }
    case DRUC_MESURE_LARGEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LARGE_MIN,
                            MACRO_RECT_LARGE_MIN
                          );
        break;
      }
    case DRUC_MESURE_LARGEUR_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LARGE_MAX,
                            MACRO_RECT_LARGE_MAX
                          );
        break;
      }
    case DRUC_MESURE_LATERALE_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LATER_MIN,
                            MACRO_RECT_LATER_MIN
                          );
        break;
      }
    case DRUC_MESURE_LATERALE_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LATER_MAX,
                            MACRO_RECT_LATER_MAX
                          );
        break;
      }
    case DRUC_MESURE_FRONTALE :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_FRONT,
                            MACRO_RECT_FRONT
                          );
        break;
      }
    case DRUC_MESURE_SUP :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_SUP,
                            MACRO_RECT_SUP
                          );
        break;
      }
    case DRUC_MESURE_INF :
      {
/*\
printf ("\nDRUC_MESURE_SUP %d \n",(DrucInstructionCourante->MESURE) [ IndexMesure ]);
\*/
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_INF,
                            MACRO_RECT_INF
                          );
        break;
      }
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationCompareCroix           |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareCroix ( DrucStructMesure,
                                DrucInstructionCourante,
                                IndexMesure
                         )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_CROIX )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_SURFACE_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure,    
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_SURFACE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LONG_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LARGEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LARGE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_PARALEL_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_PARAL_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_PERPEND_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_PERPEN_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MIN,
                            MACRO_RECT_LONG_MIN
                          );
        break;
      }
    case DRUC_MESURE_LONGUEUR_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MAX,
                            MACRO_RECT_LONG_MAX
                          );
        break;
      }
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationCompareIntersection    |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareIntersection ( DrucStructMesure,
                                       DrucInstructionCourante,
                                       IndexMesure
                         )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_INTERSECTION )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_SURFACE_INTER :
      {
        DrucMesureInter ( DrucStructMesure,
                          DrucInstructionCourante,
                          IndexMesure,
                          MACRO_SURFACE_INTER
                        );
        break;
      }
    case DRUC_MESURE_LONGUEUR_INTER :
      {
        DrucMesureInter ( DrucStructMesure,
                          DrucInstructionCourante,
                          IndexMesure,
                          MACRO_LONG_INTER
                        );
        break;
      }
    case DRUC_MESURE_LARGEUR_INTER :
      {
        DrucMesureInter ( DrucStructMesure,
                          DrucInstructionCourante,
                          IndexMesure,
                          MACRO_LARGE_INTER
                        );
        break;
      }
    case DRUC_MESURE_LONGUEUR_MAX :
    case DRUC_MESURE_LONGUEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MAX,
                            MACRO_RECT_LONG_MAX
                          );
        break;
      }
    case DRUC_MESURE_LARGEUR_MAX :
    case DRUC_MESURE_LARGEUR_MIN :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LARGE_MAX,
                            MACRO_RECT_LARGE_MAX
                          );
        break;
      }
    case DRUC_MESURE_SUP :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_SUP,
                            MACRO_RECT_SUP
                          );
        break;
      }
    case DRUC_MESURE_INF :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_INF,
                            MACRO_RECT_INF
                          );
        break;
      }
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationCompareExtension       |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareExtension ( DrucStructMesure,
                                    DrucInstructionCourante,
                                    IndexMesure
                         )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_EXTENSION )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_SURFACE_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure,    
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_SURFACE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LONG_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LARGEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LARGE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_PENETRE_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_PENETRE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_MIN :
    case DRUC_MESURE_LONGUEUR_MAX :
      {
        DrucRelationTeste ( DrucStructMesure,
                            DrucInstructionCourante,
                            IndexMesure,
                            MACRO_LONG_MAX,
                            MACRO_RECT_LONG_MAX
                          );
        break;
      }
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucRelationCompareInclusion       |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompareInclusion ( DrucStructMesure,
                                    DrucInstructionCourante,
                                    IndexMesure
                         )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
              int                        IndexMesure;
{
  if ( DrucStructMesure->ORIENTATION == DRUC_ORIENTATION_INCLUSION )
  {
    switch ( ( DrucInstructionCourante->MESURE ) [ IndexMesure ] )
    {
    case DRUC_MESURE_SURFACE_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure,    
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_SURFACE_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LONGUEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LONG_INTER 
                        ); 
        break; 
      } 
    case DRUC_MESURE_LARGEUR_INTER : 
      { 
        DrucMesureInter ( DrucStructMesure, 
                          DrucInstructionCourante, 
                          IndexMesure, 
                          MACRO_LARGE_INTER 
                        ); 
        break; 
      } 
    } /* while */
  } /* if orientation */
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucRelationCompare                 |
|                                                             |
\------------------------------------------------------------*/
void DrucRelationCompare (  DrucStructMesure,
                            DrucInstructionCourante
                         )
     register DrucTypeStructMesure      *DrucStructMesure;
              DrucTypeStructInstruction *DrucInstructionCourante;
{
int IndexMesure;

  if ( DrucIsRectanglesValides ( DrucStructMesure,
                                 DrucInstructionCourante
                               )
     )
  {
    DrucRelationMesure ( DrucStructMesure
                       );
    IndexMesure = 0;
    while ( ( DrucInstructionCourante->RELATION [ IndexMesure ] != 
              DRUC_UNDEFINED_MESURE
            ) &&
            ( IndexMesure != DRUC_MAX_MESURES
            )
          )
    {
      switch ( DrucInstructionCourante->RELATION [ IndexMesure ] )
      {
      case DRUC_RELATION_DISTANCE :
        {
          if ( DrucInstructionCourante->LAYER_1 == DrucInstructionCourante->LAYER_2 )
          { 
            if ( DrucIsRectangleEquipot ( DrucStructMesure
                                        )
               )
            {
              break;
            }
          }
          if ( MACRO_BOOL_DISTANCE == DRUC_TRUE )
          {
            DrucRelationCompareDistance ( DrucStructMesure ,
                                          DrucInstructionCourante,
                                          IndexMesure
                                        );
          }
          break;
        }
      case DRUC_RELATION_ENVELOPPE :
        {
          if ( MACRO_BOOL_DISTANCE == DRUC_FALSE )
          {
            DrucRelationCompareEnveloppe ( DrucStructMesure ,
                                           DrucInstructionCourante,
                                           IndexMesure
                                         );
          }
          break;
        }
      case DRUC_RELATION_MARGE :
        {
          if ( MACRO_BOOL_DISTANCE == DRUC_FALSE )
          {
            DrucRelationCompareMarge ( DrucStructMesure ,
                                       DrucInstructionCourante,
                                       IndexMesure
                                     );
          }
          break;
        }
      case DRUC_RELATION_CROIX :
        {
          if ( MACRO_BOOL_DISTANCE == DRUC_FALSE )
          {
            DrucRelationCompareCroix ( DrucStructMesure ,
                                       DrucInstructionCourante,
                                       IndexMesure
                                     );
          }
          break;
        }
      case DRUC_RELATION_INTERSECTION :
        {
          if ( MACRO_BOOL_DISTANCE == DRUC_FALSE )
          {
            DrucRelationCompareIntersection( DrucStructMesure ,
                                             DrucInstructionCourante,
                                             IndexMesure
                                           );
          }
          break;
        }
      case DRUC_RELATION_EXTENSION :
        {
          if ( MACRO_BOOL_DISTANCE == DRUC_FALSE )
          {
            DrucRelationCompareExtension ( DrucStructMesure ,
                                           DrucInstructionCourante,
                                           IndexMesure
                                         );
          }
          break;
        }
      case DRUC_RELATION_INCLUSION :
        {
          if ( MACRO_BOOL_DISTANCE == DRUC_FALSE )
          {
            DrucRelationCompareInclusion ( DrucStructMesure ,
                                           DrucInstructionCourante,
                                           IndexMesure
                                         );
          }
          break;
        }
      } /* switch */
      IndexMesure ++;
    } /* while */
  }  /* if */
}
/*------------------------------------------------------------\
|                                                             |
|                         DrucRelation                        |
|                                                             |
\------------------------------------------------------------*/
void DrucRelation ( DrucFigureRds,
                    DrucRdsWindow,
                    DrucInstructionCourante
                  )
     rdsfig_list               *DrucFigureRds;
     rdswindow                 *DrucRdsWindow;
     DrucTypeStructInstruction *DrucInstructionCourante;
{
         rdsrec_list          *TmpRectangleSource2;
         rdswin_list       *TmpWindowSource;
register int                   TabIndex;
register rdswinrec_list       *TabRectangleSource = NULL;
         DrucTypeStructMesure  DrucStructMesure;
chain_list                    *PtChaine;
chain_list                    *HeadChaine;
long                           DilateX;
long                           DilateY;
long                           DilateDX;
long                           DilateDY;


  DrucStructMesure.LAYER_2   = DrucInstructionCourante->LAYER_2;
  DrucStructMesure.LAYER_1   = DrucInstructionCourante->LAYER_1;
  for ( TmpRectangleSource2  = DrucFigureRds->LAYERTAB 
                      [ DrucInstructionCourante->LAYER_2 ];
        TmpRectangleSource2 != (rdsrec_list *)NULL;
        TmpRectangleSource2  = TmpRectangleSource2->NEXT
      )
  {
    DrucStructMesure.RECTANGLE2 = TmpRectangleSource2;

/*\
DrucViewString     ("\nBOUCLE"
                   );
DrucViewNumber     ( DrucStructMesure.LAYER_1
                   );
DrucViewNumber     ( DrucStructMesure.LAYER_2
                   );
viewrdsrec         ( TmpRectangleSource2
                   );
DrucVerifEquiLayer ( DrucFigureRds,
                     DrucStructMesure.LAYER_1
                   );
printf ("\n fin de viewrdsfig\n");
viewrdsfig (DrucFigureRds);
\*/

    DrucRelationDilateRectangle ( DrucRdsWindow,
                                  DrucInstructionCourante,
                                  TmpRectangleSource2,
                                  & DilateX,
                                  & DilateY,
                                  & DilateDX,
                                  & DilateDY
                                );
    PtChaine = getrdswindow  ( DilateX,
                               DilateY,
                               DilateDX,
                               DilateDY,
                               DrucRdsWindow
                             );
    HeadChaine = PtChaine;

    while ( PtChaine != (chain_list *)NULL
          )
    {

      TmpWindowSource    = ( rdswin_list *) PtChaine->DATA;
      PtChaine           = PtChaine ->NEXT;
      TabRectangleSource = TmpWindowSource->LAYERTAB
                                [ DrucInstructionCourante->LAYER_1 ];

      while ( TabRectangleSource != (rdswinrec_list *)NULL
            )
      {

        TabIndex = 0;

        while ( TabIndex < RWI_MAX_REC )
        {
          if ( ( TabRectangleSource->RECTAB [ TabIndex ] != 
                 ( rdsrec_list *)NULL 
               ) &&
               ( ! ( TabRectangleSource->RECTAB [ TabIndex ]->FLAGS & 
                      DRUC_FLAG_REC_VERIFIE 
                   )
               ) )
          {
            DrucStructMesure.RECTANGLE1 = 
                TabRectangleSource->RECTAB [ TabIndex ];

/*\
viewrdsrec ( TabRectangleSource->RECTAB [ TabIndex ]
           );
DrucViewString ("FIN BOUCLE");
\*/

            DrucStructMesure.RECTANGLE1->FLAGS |= DRUC_FLAG_REC_VERIFIE;
            DrucRelationCompare ( &DrucStructMesure ,
                                   DrucInstructionCourante
                                );
           }

           TabIndex ++;

        } /* while les rectangles d'un tableau du layer A */
        TabRectangleSource = TabRectangleSource->NEXT;
      } /* while les tableaux du layer A */
    } /* while les fenetres du layer A */
    PtChaine = HeadChaine;
    while ( PtChaine != (chain_list *)NULL
          )
    {
      TmpWindowSource    = ( rdswin_list *) PtChaine->DATA;
      PtChaine = PtChaine ->NEXT;
      TabRectangleSource = TmpWindowSource->LAYERTAB
                                [ DrucInstructionCourante->LAYER_1 ];
      while ( TabRectangleSource != (rdswinrec_list *)NULL
            )
      {
        TabIndex = 0;
        while ( TabIndex < RWI_MAX_REC
              )
        {
          if ( TabRectangleSource->RECTAB [ TabIndex ] != 
               ( rdsrec_list *)NULL ) 
          {
            TabRectangleSource->RECTAB [ TabIndex ]->FLAGS &= 
                ~DRUC_FLAG_REC_VERIFIE;
          }

          TabIndex ++;
        } /* while les rectangles d'un tableau du layer A */
        TabRectangleSource = TabRectangleSource->NEXT;
      } /* while les tableaux du layer A */
    } /* while les fenetres du layer A */
  } /* for le parcours des rectangles du layer B */
}
