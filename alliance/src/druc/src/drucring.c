/*------------------------------------------------------------\
|                                                             |
| Tool    :                    DRUC                           |
|                                                             |
| File    :                  DRUCRING.C                       |
|                                                             |
| Authors :                Renaud Patrick                     |
|                                                             |
| Date    :                   03/07/95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
| |                                                           |
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
#include "druchier.h"
#include "drucring.h"
/*------------------------------------------------------------\ 
|                                                             |
|                         DRUCRING Variables                  |
|                                                             |
\------------------------------------------------------------*/ 
extern long DrucCtmMax; 
/*------------------------------------------------------------\ 
|                                                             |
|                         DRUCRING Fonctions                  |
|                                                             |
\------------------------------------------------------------*/ 
/*------------------------------------------------------------\
|                                                             |
|                          DrucDelLayer                       |
|                                                             |
\------------------------------------------------------------*/
void DrucDelLayer ( DrucFigureRds,
                    DeleteLayer
                  )
     rdsfig_list *DrucFigureRds;
     int          DeleteLayer;
{
rdsrec_list *TmpRectangle;

  while ( DrucFigureRds->LAYERTAB [ DeleteLayer ] != ( rdsrec_list *)NULL
        )
  {
    TmpRectangle = DrucFigureRds->LAYERTAB [ DeleteLayer ];
    DrucFigureRds->LAYERTAB [ DeleteLayer ] = TmpRectangle->NEXT;
    freerdsrec ( TmpRectangle,
                 MACRO_PLUS_TAILLE
               );
  }
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucModifieBboxInstance            |
|                                                             |
\------------------------------------------------------------*/
void      DrucModifieBboxInstance ( RectangleCourant,
                                    PositionBboxX,
                                    PositionBboxY,
                                    PositionBboxDX,
                                    PositionBboxDY
                                  )
rdsrec_list    *RectangleCourant;
long           *PositionBboxX;
long           *PositionBboxY;
long           *PositionBboxDX;
long           *PositionBboxDY;

{
  *PositionBboxX  = ( *PositionBboxX < RectangleCourant->X ?
                      *PositionBboxX :
                       RectangleCourant->X
                    );
  *PositionBboxY =  ( *PositionBboxY < RectangleCourant->Y ?
                      *PositionBboxY :
                       RectangleCourant->Y
                    );
  *PositionBboxDX = ( *PositionBboxDX > ( RectangleCourant->X + 
                                          RectangleCourant->DX  ) ?
                      *PositionBboxDX :
                       ( RectangleCourant->X + 
                         RectangleCourant->DX  )
                    );
  *PositionBboxDY = ( *PositionBboxDY > ( RectangleCourant->Y +
                                          RectangleCourant->DY  ) ?
                      *PositionBboxDY :
                       ( RectangleCourant->Y +
                         RectangleCourant->DY  )
                    );
}
/*------------------------------------------------------------\ 
|                                                             |
|                         DrucFlattenInstanceRings            |
|                                                             |
\------------------------------------------------------------*/ 
void DrucFlattenInstanceRings ( DrucFigureRds
                              )
     rdsfig_list *DrucFigureRds;
{
rdsfig_list               *InstanceModele;
rdsins_list               *InstanceCourante;
int                        IndexLayer;
char                      *TmpName;
rdsrec_list               *RectangleCourant;
rdsrec_list               *NouveauRectangle;

DrucViewString ("Flatten Instance Rings:");
    for ( InstanceCourante  = DrucFigureRds->INSTANCE;
          InstanceCourante != (rdsins_list *)NULL;
          InstanceCourante  = InstanceCourante->NEXT
        )
    {
/*\
 *  pour toutes les instances de la figure inserer les rectangles
 *  du modele de l'instance dans le bloc
\*/
      TmpName = DrucAddSuffixeName ( InstanceCourante->FIGNAME,
                                     DRUC_COURONNE_EXTEND
                                   );
DrucViewString ("\r");
DrucViewString (TmpName);
DrucViewString ("                      ");
      InstanceModele = searchrdsfig( TmpName
                                   );
/*\
DrucViewString ("\nFIN_FLATTEN_INSTANCE:\n");
\*/
      for ( IndexLayer  = 0;
            IndexLayer  < RDS_MAX_LAYER;
            IndexLayer ++
          )
      {
        if ( ( IndexLayer == DRUC_BBOX ) ||
             ( IndexLayer == RDS_ABOX  ) ||
             ( IndexLayer == RDS_USER0 ) ||
             ( IndexLayer == RDS_USER1 ) ||
             ( IndexLayer == RDS_USER2 ) ||
             ( IndexLayer == RDS_USER3 ) ||
             ( IndexLayer == RDS_USER4 ) ||
             ( IndexLayer == RDS_USER5 ) ||
             ( IndexLayer == RDS_USER6 ) ||
             ( IndexLayer == RDS_USER7 ) ||
             ( IndexLayer == RDS_USER8 ) ||
             ( IndexLayer == RDS_USER9 )
           )
        {
          continue;
        }
        for ( RectangleCourant  = InstanceModele->LAYERTAB [ IndexLayer ];
              RectangleCourant != (rdsrec_list *)NULL;
              RectangleCourant  = RectangleCourant->NEXT
            )
        {
          NouveauRectangle = addrdsfigrec ( DrucFigureRds,
				            RectangleCourant->NAME,
                                            IndexLayer,
				            RectangleCourant->X,
				            RectangleCourant->Y,
		 		            RectangleCourant->DX,
				            RectangleCourant->DY
			                  );
          applyrdssym ( & (NouveauRectangle->X),
                        & (NouveauRectangle->Y),
                        & (NouveauRectangle->DX),
                        & (NouveauRectangle->DY),
                          InstanceCourante->X,
                          InstanceCourante->Y,
                          InstanceCourante->TRANSF
                      );
          MACRO_INSTANCE_NUM ( NouveauRectangle ) = InstanceCourante;
/*\
DrucViewRectangle ( &NouveauRectangle );
\*/
        } 
      }/* for tous les layers */
/*\
DrucViewString ("\nFIN_FLATTEN_INSTANCE:\n");
\*/

      for ( RectangleCourant  = InstanceModele->LAYERTAB [ RDS_ABOX ];
            RectangleCourant != (rdsrec_list *)NULL;
            RectangleCourant  = RectangleCourant->NEXT
          )
      {
        NouveauRectangle = addrdsfigrec ( DrucFigureRds,
                                          RectangleCourant->NAME,
                                          DRUC_BBOX,
                                          RectangleCourant->X,
                                          RectangleCourant->Y,
                                          RectangleCourant->DX,
                                          RectangleCourant->DY
                                        );
        applyrdssym ( & (NouveauRectangle->X),
                      & (NouveauRectangle->Y),
                      & (NouveauRectangle->DX),
                      & (NouveauRectangle->DY),
                        InstanceCourante->X,
                        InstanceCourante->Y,
                        InstanceCourante->TRANSF
                    );
        MACRO_INSTANCE_NUM ( NouveauRectangle ) = InstanceCourante;
      } /* for le ABOX de la figure */
    }/* for toutes les instances */
DrucViewString ("\n");
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucIsLayerTransparence            |
|                                                             |
\------------------------------------------------------------*/
boolean DrucIsLayerTransparence ( IndexLayer 
                                )
        int IndexLayer;
{
int IndexTableau;

  for ( IndexTableau  = 0;
        IndexTableau  < DRUC_MAX_TRANSPARENCES;
        IndexTableau ++
      )
  {
    if ( IndexLayer == ( TabTransEquivalence [ IndexTableau ] ).LAYER_TRANSPARENCE
       )
    {
      return DRUC_TRUE;
    }
  }
  return DRUC_FALSE;
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucIsLayerVirtuel                 |
|                                                             |
\------------------------------------------------------------*/
boolean DrucIsLayerVirtuel ( IndexLayer
                           )
        int  IndexLayer;
{
int Index;

  for ( Index  = 0;
        Index  < DRUC_MAX_VIRTUELS;
        Index ++
      )
  {
    if ( IndexLayer == ( TabVirtuEquivalence [ Index ] ).LAYER_VIRTUEL
       )
    {
      return DRUC_TRUE;
    }
  }
  return DRUC_FALSE;
}
/*------------------------------------------------------------\
|                                                             |
|                          DrucCreateRectangleInCouronne      |
|                                                             |
\------------------------------------------------------------*/
void DrucCreateRectangleInCouronne ( DrucCouronneRds,
                                     IndexLayer,
                                     RectangleCourant,
                                     PositionInterneCouronneX,
                                     PositionInterneCouronneY,
                                     PositionInterneCouronneDX,
                                     PositionInterneCouronneDY
                                   )
    rdsfig_list *DrucCouronneRds;
    int          IndexLayer;
    rdsrec_list *RectangleCourant;
    long         PositionInterneCouronneX;
    long         PositionInterneCouronneY;
    long         PositionInterneCouronneDX;
    long         PositionInterneCouronneDY;

{
      /*\
       *  si le rectangle est en contact avec la couronne
      \*/
      if ( ( RectangleCourant->X                        <= 
             PositionInterneCouronneX  )                   ||
           ( RectangleCourant->Y                        <= 
             PositionInterneCouronneY  )                   ||
           ( RectangleCourant->X + RectangleCourant->DX >= 
             PositionInterneCouronneDX )                   ||
           ( RectangleCourant->Y + RectangleCourant->DY >= 
             PositionInterneCouronneDY ) 
         )
      {
      /*\
       *  creer un nouveau rectangle, 
       *  l'inserer dans la figure couronne sur son layer
      \*/
        addrdsfigrec ( DrucCouronneRds,
                       RectangleCourant->NAME,
                       IndexLayer,
                       RectangleCourant->X,
                       RectangleCourant->Y,
                       RectangleCourant->DX,
                       RectangleCourant->DY
                     );
  }
} 

/*------------------------------------------------------------\
|                                                             |
|                          DrucCreateCouron                   |
|                                                             |
\------------------------------------------------------------*/
void DrucCreateCouron ( DrucFigureRds,
                        DrucRdsWindow,
                        DrucCouronneName
                      )
rdsfig_list         *DrucFigureRds;
rdswindow           *DrucRdsWindow;
char                *DrucCouronneName;
{
rdsfig_list         *DrucCouronneRds;
rdsrec_list         *RectangleCourant;
rdsrec_list         *RectangleABox;
long                 IndexLayer;
long                 IndexVirtuel;
long                 DemiConnecteur;
long                 PositionInterneCouronneX;
long                 PositionInterneCouronneY;
long                 PositionInterneCouronneDX;
long                 PositionInterneCouronneDY;
long                 CouroneRefX;
long                 CouroneRefY;
long                 CouroneRefDX;
long                 CouroneRefDY;

  /*\
   *  calculer les positions de la couronne interne
  \*/
  DrucCouronneRds = addrdsfig ( DrucCouronneName,
                                MACRO_PLUS_TAILLE
                              );
  DemiConnecteur  = DRUC_DEMI_CONNECTEUR;
  RectangleABox   = DrucFigureRds->LAYERTAB [ RDS_ABOX ];

  /*\
   * calcul des limites de la couronne
  \*/
  if ( RectangleABox != (rdsrec_list*)NULL )
  {
    CouroneRefX  = RectangleABox->X + DemiConnecteur;
    CouroneRefY  = RectangleABox->Y + DemiConnecteur;
    CouroneRefDX = RectangleABox->X + RectangleABox->DX - DemiConnecteur;
    CouroneRefDY = RectangleABox->Y + RectangleABox->DY - DemiConnecteur;
  }
  else
  {
    CouroneRefX  = DrucRdsWindow->XMIN + DrucCtmMax;
    CouroneRefY  = DrucRdsWindow->YMIN + DrucCtmMax;
    CouroneRefDX = DrucRdsWindow->XMAX - DrucCtmMax;
    CouroneRefDY = DrucRdsWindow->YMAX - DrucCtmMax;
  }

  for ( IndexLayer  = 0;
        IndexLayer  < RDS_MAX_LAYER;
        IndexLayer ++
      )
  {
    /*\
     * on ne remonte pas les layers user ni les BBOX des instances ni les ABOX
    \*/
    if ( ( IndexLayer == DRUC_BBOX ) ||
         ( IndexLayer == RDS_ABOX  ) ||
         ( IndexLayer == RDS_USER0 ) ||
         ( IndexLayer == RDS_USER1 ) ||
         ( IndexLayer == RDS_USER2 ) ||
         ( IndexLayer == RDS_USER3 ) ||
         ( IndexLayer == RDS_USER4 ) ||
         ( IndexLayer == RDS_USER5 ) ||
         ( IndexLayer == RDS_USER6 ) ||
         ( IndexLayer == RDS_USER7 ) ||
         ( IndexLayer == RDS_USER8 ) ||
         ( IndexLayer == RDS_USER9 )
       )
    {
      continue;
    } 
    /*\
     * La position de la couronne est l'ABOX erodee de 1/2 connecteur et de
     * 1 ctm du niveau
    \*/
    PositionInterneCouronneX  = ( CouroneRefX +
                                  DrucStructRdsLayer[ IndexLayer ].CTM
                                );
    PositionInterneCouronneDX = ( CouroneRefDX -
                                  DrucStructRdsLayer[ IndexLayer ].CTM
                                );
    PositionInterneCouronneY  = ( CouroneRefY +
                                  DrucStructRdsLayer[ IndexLayer ].CTM
                                );
    PositionInterneCouronneDY = ( CouroneRefDY -
                                  DrucStructRdsLayer[ IndexLayer ].CTM
                                );

    if ( DrucIsLayerTransparence ( IndexLayer ) == DRUC_TRUE
       )
    {
    /*\
     *  on ne garde que les transparences du bloc (pere)
    \*/
      for ( RectangleCourant    = DrucFigureRds->LAYERTAB [ IndexLayer ];
            RectangleCourant   != (rdsrec_list *)NULL;
            RectangleCourant    = RectangleCourant->NEXT
          )
      {
        if ( MACRO_INSTANCE_NUM ( RectangleCourant ) == DRUC_BLOC_PERE
           )
        {
          DrucCreateRectangleInCouronne ( DrucCouronneRds,
                                          IndexLayer,
                                          RectangleCourant,
                                          PositionInterneCouronneX,
                                          PositionInterneCouronneY,
                                          PositionInterneCouronneDX,
                                          PositionInterneCouronneDY
                                        );
        }
      }
      continue;
    }

    if ( DrucIsLayerVirtuel ( IndexLayer ) == DRUC_TRUE
       )
    {
      /*\
       *  on ne garde que les virtuels du bloc (pere) et on les transforme en transparence
      \*/
      IndexVirtuel = DrucAsTransparenceEquivalent ( IndexLayer );
      if ( IndexVirtuel != DRUC_UNDEFINED_LAYER )
      {
        for ( RectangleCourant    = DrucFigureRds->LAYERTAB [ IndexLayer ];
              RectangleCourant   != (rdsrec_list *)NULL;
              RectangleCourant    = RectangleCourant->NEXT
            )
        {
          if ( MACRO_INSTANCE_NUM ( RectangleCourant ) == DRUC_BLOC_PERE
             )
          {
            DrucCreateRectangleInCouronne ( DrucCouronneRds,
                                            IndexVirtuel,
                                            RectangleCourant,
                                            PositionInterneCouronneX,
                                            PositionInterneCouronneY,
                                            PositionInterneCouronneDX,
                                            PositionInterneCouronneDY
                                          );
          }
        }
      }
      continue;
    }
    for ( RectangleCourant    = DrucFigureRds->LAYERTAB [ IndexLayer ];
          RectangleCourant   != (rdsrec_list *)NULL;
          RectangleCourant    = RectangleCourant->NEXT
        )
    {
      DrucCreateRectangleInCouronne ( DrucCouronneRds,
                                      IndexLayer,
                                      RectangleCourant,
                                      PositionInterneCouronneX,
                                      PositionInterneCouronneY,
                                      PositionInterneCouronneDX,
                                      PositionInterneCouronneDY
                                    );
    }
  }

  /*\
   * sauver l'ABOX du pere seulement
  \*/
    for ( RectangleCourant    = DrucFigureRds->LAYERTAB [ RDS_ABOX ];
          RectangleCourant   != (rdsrec_list *)NULL;
          RectangleCourant    = RectangleCourant->NEXT
        )
    {
      if ( MACRO_INSTANCE_NUM ( RectangleCourant ) == DRUC_BLOC_PERE
         )
      {
        DrucCreateRectangleInCouronne ( DrucCouronneRds,
                                        RDS_ABOX,
                                        RectangleCourant,
                                        PositionInterneCouronneX,
                                        PositionInterneCouronneY,
                                        PositionInterneCouronneDX,
                                        PositionInterneCouronneDY
                                      );
      }
    }

  /*\
   *  sauver la couronne sur disque
viewrdsfig ( DrucCouronneRds );
  \*/
   saverdsfig ( DrucCouronneRds );
}
