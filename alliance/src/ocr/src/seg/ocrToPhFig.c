/*
   ### -------------------------------------------------- ### 
   $Author: uid809 $
   $Date: 2002/07/03 13:17:14 $
   $Log: ocrToPhFig.c,v $
   Revision 1.3  2002/07/03 13:17:14  uid809
   merci Laurent D.

   Revision 1.2  2002/07/03 12:38:16  hcl
   rien

   Revision 1.1  2002/03/15 14:37:22  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:20  hcl
   First commit in the new tree.

   Revision 1.8  2002/01/18 14:04:48  hcl
   Bug connector

   Revision 1.7  2002/01/14 10:34:34  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.6  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.5  2001/12/19 14:22:49  hcl
   Moult petits changements.

   Revision 1.4  2001/12/17 14:29:36  hcl
   VDD/VSS rail bug, new ripUp.

   Revision 1.3  2001/12/14 15:58:39  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.2  2001/11/20 09:41:34  hcl
   segmentation fault

   Revision 1.1  2001/09/26 14:27:51  hcl
   premier commit....

   Revision 1.3  2000/02/26 00:24:09  root
   2 points OK

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"
#include <string.h>
#include <stdlib.h>
#include "ocr.h"
#include "ocrUtil.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "mbk_tree.h"
#include "ocrWRoutingUtil.h"
#include "ocrToPhFig.h"
#include "display.h"

#define PITCH		5

static char *res_id =
    "$Id: ocrToPhFig.c,v 1.3 2002/07/03 13:17:14 uid809 Exp $";

extern ocrOption *g_pOption;
#define LEVEL (g_pOption->LEVEL)

/* 
   ### --------------------------- ###
   ### Internal Functions
   ### --------------------------- ###
 */

/**
 * Retourne le VIA physique correspondant à la coordonée z
**/
int getPhVia(int i_uZ)
{
    switch (i_uZ) {
    case -1:
        return CONT_VIA;
    case 0:
        return CONT_VIA2;
    case 1:
        return CONT_VIA3;
    case 2:
        return CONT_VIA4;
    case 3:
        return CONT_VIA5;
    case 4:
        return CONT_VIA6;
    case 5:
        return CONT_VIA7;
    }
    display(LEVEL, ERROR, "getPhVia in %s:%d\n", __FILE__, __LINE__);
    exit(1);

    // pour éviter un Warning ...  
    return 0;
}

/**
 * Retourne le LAYER physique correspondant à la coordonée z
**/

ocrNaturalInt getPhLayer(ocrNaturalInt i_uZ)
{
    switch (i_uZ) {
    case 0:
        return ALU2;
    case 1:
        return ALU3;
    case 2:
        return ALU4;
    case 3:
        return ALU5;
    case 4:
        return ALU6;
    case 5:
        return ALU7;
    case 6:
        return ALU8;
    }

    display(LEVEL, ERROR, "getPhLayer in %s:%d\n", __FILE__, __LINE__);
    exit(1);

    // pour éviter un Warning ...  
    return 0;
}

/**
 * Ajoute un via dans la io_pPhFig
 * i_uLayer est le Layer de niveau le plus bas par rapport au via
**/
void
addViaToPhFig(ocrRoutingParameters * i_pParam,
              ocrNaturalInt i_uX,
              ocrNaturalInt i_uY, int i_nLayer, phfig_list * io_pPhFig)
{
#if 0
    addphvia(io_pPhFig,
             getPhVia(i_nLayer),
             i_uX * i_pParam->PITCH_H * SCALE_X,
             i_uY * i_pParam->PITCH_H * SCALE_X);
    //printf ("Add via %d en %ld %ld\n",i_nLayer,i_uX,i_uY);
#else


    addphvia(io_pPhFig,
             getPhVia(i_nLayer),
             i_uX * i_pParam->PITCH_H * SCALE_X,
             i_uY * i_pParam->PITCH_V * SCALE_X, 0, 0, NULL);
#endif
}

/**
 * Ajoute un segment dans la io_pPhFig
**/

void
addSegmentToPhFig(ocrRoutingParameters * i_pParam,
                  ocrWSegment * i_pSegment, phfig_list * io_pPhFig,
                  char *i_pName)
{
    char l_sName[7];

    sprintf(l_sName, "seg%ld", i_pSegment->SIGNAL_INDEX);

    if (getWSegDirection(i_pParam, i_pSegment) == ocrHorizontal) {
        addphseg(io_pPhFig,
                 getPhLayer(i_pSegment->LAYER),
                 i_pParam->PITCH_V * 4 * SCALE_X / 10,
                 i_pSegment->P_MIN * i_pParam->PITCH_H * SCALE_X,
                 i_pSegment->OFFSET * i_pParam->PITCH_V * SCALE_X,
                 i_pSegment->P_MAX * i_pParam->PITCH_H * SCALE_X,
                 i_pSegment->OFFSET * i_pParam->PITCH_V * SCALE_X,
                 (i_pName ? i_pName : l_sName));
                 //l_sName);
    } else {
        addphseg(io_pPhFig, getPhLayer(i_pSegment->LAYER),
                 // FIXME width of phseg ??? 2 or 1 ?
                 i_pParam->PITCH_H * 4 * SCALE_X / 10,
                 i_pSegment->OFFSET * i_pParam->PITCH_H * SCALE_X,
                 i_pSegment->P_MIN * i_pParam->PITCH_V * SCALE_X,
                 i_pSegment->OFFSET * i_pParam->PITCH_H * SCALE_X,
                 i_pSegment->P_MAX * i_pParam->PITCH_V * SCALE_X,
                 (i_pName ? i_pName : l_sName));
                 //l_sName);
    }
}


/**
 * Dump le signal en (x,y,z) en segment physique
**/

/*

a implanter un jour ou l'autre...


   changer dumpSignal et addSegmentToPhFig :

   * utiliser les TAGs plutot que le champ SIGNAL_INDEX
   * tenir compte des ocrWSegment de longueur 1 :
     pas de VIA si les segment precedents et suivants sont sur
     le meme layer.


=-> IDEE

dans l'expansion des segments, autoriser le saut d'une piste a une
autre piste immediatement voisine dans le meme layer.
-> pour expansion : assez immediat
-> pour kost : c'est plus delicat...
-> pour make_segments : argh !

ATTENTION ! ne pas autoriser trop de ce genre de propagation !
1/
--------|
        |-------------------- ok

2/
--------|
        |
        |-------------------- not ok

3/
--------|
        -|
         -|
          ------------------- not ok

====> comment faire ?

post-traitement lors du make_segments ?

lors de l'expansion : ne pas etendre aux pistes adjacentes par le P d'arrivee. (P_MIN < P_MAX)
le cas 3 n'arrivera pas (ou peu) car il est probable que soient preferes des segments amenant
plus rapidement au but.

*/



void
dumpSignal(ocrRoutingParameters * i_pParam,
           ocrWRoutingGrid * i_pGrid,
           phfig_list * io_pPhFig,
           ocrNaturalInt x, ocrNaturalInt y, ocrNaturalInt z)
{

    ocrWSegment *l_pSegment;
    ocrWSegment *l_pSegmentVoisin;
    ocrNaturalInt l_uNoSignal;

    // segment concerné
    l_pSegment = getWSegment(i_pGrid, x, y, z);

    // sauvegarde du numéro du signal
    l_uNoSignal = l_pSegment->SIGNAL_INDEX;

// printf("dumpSignal %ld %ld %ld\n",x,y,z);
    //   printf("Index = %ld\n",l_uNoSignal);

    // quand on parcours un signal il devient libre
    // pour ne pas parcourir 2 fois le meme signal
    l_pSegment->SIGNAL_INDEX = WSEGMENT_FREE;

    if (getWSegDirection(i_pParam, l_pSegment) == ocrHorizontal) {
        ocrNaturalInt i;

        for (i = l_pSegment->P_MIN; i <= l_pSegment->P_MAX; i++) {
            if (z > 0) {
                l_pSegmentVoisin = getWSegment(i_pGrid, i, y, z - 1);
                // connexion avec LAYER - 1 ?
                if (l_pSegmentVoisin /*&& (!l_pSegmentVoisin->TAG) */ )
                    if (l_pSegmentVoisin->SIGNAL_INDEX == l_uNoSignal) {
                        //l_pSegmentVoisin->TAG = 1;
                        dumpSignal(i_pParam, i_pGrid, io_pPhFig, i, y,
                                   z - 1);
                        // ajout d'un via
                        addViaToPhFig(i_pParam, i, y, z - 1, io_pPhFig);
                    }
            }
            if (z < i_pGrid->NB_OF_LAYERS - 1) {
                l_pSegmentVoisin = getWSegment(i_pGrid, i, y, z + 1);
                // connexion avec LAYER + 1 ?
                if (l_pSegmentVoisin /* && (!l_pSegmentVoisin->TAG) */ )
                    if (l_pSegmentVoisin->SIGNAL_INDEX == l_uNoSignal) {
                        //l_pSegmentVoisin->TAG = 1;
                        dumpSignal(i_pParam, i_pGrid, io_pPhFig, i, y,
                                   z + 1);
                        // ajout d'un via
                        addViaToPhFig(i_pParam, i, y, z, io_pPhFig);
                    }
            }
        }
    } else {
        ocrNaturalInt i;

        for (i = l_pSegment->P_MIN; i <= l_pSegment->P_MAX; i++) {
            if (z > 0) {
                l_pSegmentVoisin = getWSegment(i_pGrid, x, i, z - 1);
                //         printf("  VERT %ld %ld %ld index = %ld TAG = %ld\n",
                //             x,i,z-1,l_pSegmentVoisin->SIGNAL_INDEX,
                //                  l_pSegmentVoisin->TAG);
                if (l_pSegmentVoisin /*&& (!l_pSegmentVoisin->TAG) */ )
                    if (l_pSegmentVoisin->SIGNAL_INDEX == l_uNoSignal) {
                        //l_pSegmentVoisin->TAG = 1;
                        dumpSignal(i_pParam, i_pGrid, io_pPhFig, x, i,
                                   z - 1);
                        // ajout d'un via
                        addViaToPhFig(i_pParam, x, i, z - 1, io_pPhFig);
                    }
            }
            if (z < i_pGrid->NB_OF_LAYERS - 1) {
                l_pSegmentVoisin = getWSegment(i_pGrid, x, i, z + 1);
                if (l_pSegmentVoisin /*&& (!l_pSegmentVoisin->TAG) */ )
                    if (l_pSegmentVoisin->SIGNAL_INDEX == l_uNoSignal) {
                        //l_pSegmentVoisin->TAG = 1;
                        dumpSignal(i_pParam, i_pGrid, io_pPhFig, x, i,
                                   z + 1);
                        // ajout d'un via
                        addViaToPhFig(i_pParam, x, i, z, io_pPhFig);
                    }
            }
        }
    }

// ajoute le segment ainsi parcouru avec comme nom le numéro du signal
    l_pSegment->SIGNAL_INDEX = l_uNoSignal;
    addSegmentToPhFig(i_pParam, l_pSegment, io_pPhFig, NULL);
    l_pSegment->SIGNAL_INDEX = WSEGMENT_FREE;
}

char *indexSignal(char *i_sRacine, ocrNaturalInt i_uIndex)
{
    char l_sSignal[40];

    sprintf(l_sSignal, "%s %ld", i_sRacine, i_uIndex);
    return namealloc(l_sSignal);
}

void
dumpIntExtConToPhFig(ocrRoutingDataBase * i_pDataBase,
                     ocrRoutingParameters * i_pParam,
                     phfig_list * i_pPhFig)
{
    ocrNaturalInt i;
    ocrConnector *l_pCon;

    for (i = 0; i < i_pDataBase->NB_GSIGNAL; i++) {
        for (l_pCon = i_pDataBase->GSIGNAL[i]->CON_LIST;
             l_pCon; l_pCon = l_pCon->NEXT) {

            if (l_pCon->INTEXT == INTERNAL) {
                if (l_pCon->CON)
                    addViaToPhFig(i_pParam,
                                  l_pCon->CON->X,
                                  l_pCon->CON->Y,
                                  l_pCon->CON->Z - 1, // VIA ALUx<->ALU(x-1)
                                  i_pPhFig);
            } else {
                if (l_pCon->CON)
                    addphcon(i_pPhFig,
                             l_pCon->CON->ORIENT,
                             l_pCon->NAME,
                             l_pCon->CON->X * (PITCH * SCALE_X),
                             l_pCon->CON->Y * (PITCH * SCALE_X),
                             ALU2,
                             2 * SCALE_X);
            }
        }
    }
}

void
dumpAlimToPhFig(ocrRoutingDataBase * i_pDataBase, phfig_list * i_pPhFig)
{
    ocrNaturalInt l_uIndex = 0;
    ocrNaturalInt i;
    ocrNaturalInt start_with_vss = 0;
    ocrNaturalInt sym_y = 0;
    char *vss = namealloc ("vss");
    char *vdd = namealloc ("vdd");

    display(LEVEL, VERBOSE, "o Dumping Alim to PhFig...\n");
    // Routage des alimentations ...
    // ajout des connecteurs vdd vss

    // VDD-VSS 
    

    sym_y = (
             (i_pPhFig->PHINS->TRANSF == SYM_Y)
             ||
             (i_pPhFig->PHINS->TRANSF == SYMXY)
            )
        ;
    
    start_with_vss =  (
                       (
                        ((i_pPhFig->PHINS->YINS / SCALE_X) % SCALE_X) == 0
                       )
                       ^
                       (
                        sym_y
                       )
                      )
        ;


//    start_with_vss = (
//                       (!((i_pPhFig->PHINS->YINS == 0) && (i_pPhFig->PHINS->TRANSF == SYM_Y)))
//                       ||
//                       (
//                        (i_pPhFig->PHINS->YINS != 0)
//                        &&
//                        (
//                         ( 
//                          (i_pPhFig->PHINS->YINS % 100)
//                          &&(!(i_pPhFig->PHINS->YINS % 50))
//                          && (i_pPhFig->PHINS->TRANSF == SYM_Y)
//                         )
//                         ||
//                         (
//                          (!(i_pPhFig->PHINS->YINS % 100))
//                          && (i_pPhFig->PHINS->TRANSF == NOSYM)
//                         )
//                        )
//                       )
//                      )
//        ;
//

    addphseg(i_pPhFig, ALU1, 6 * SCALE_X,
             i_pDataBase->XAB1, i_pDataBase->YAB2 - 3 * SCALE_X,
             i_pDataBase->XAB2, i_pDataBase->YAB2 - 3 * SCALE_X, NULL);

    addphseg(i_pPhFig, ALU1, 6 * SCALE_X,
             i_pDataBase->XAB1, 3 * SCALE_X, i_pDataBase->XAB2, 300, NULL);


    addphcon(i_pPhFig, WEST,
             (start_with_vss ? vss : vdd), i_pDataBase->XAB1, 3 * SCALE_X, ALU1, 600);

    addphcon(i_pPhFig, EAST,
             (start_with_vss ? vss : vdd), i_pDataBase->XAB2, 3 * SCALE_X, ALU1, 600);

    l_uIndex++;


    for (i = 2 * PITCH * 10 * SCALE_X; i < i_pDataBase->YAB2;
         i += 2 * PITCH * 10 * SCALE_X) {

        addphseg(i_pPhFig, ALU1, 12 * SCALE_X,
                 i_pDataBase->XAB1, i,
                 i_pDataBase->XAB2, i, (start_with_vss ? vss : vdd) );

        addphcon(i_pPhFig, WEST,
                 (start_with_vss ? vss : vdd), i_pDataBase->XAB1, i, ALU1, 12 * SCALE_X);

        //addphcon(i_pPhFig, WEST,
        //         (start_with_vss ? vss : vdd), i_pDataBase->XAB1, i + 3 * SCALE_X, ALU1, 600);

        addphcon(i_pPhFig, EAST,
                 (start_with_vss ? vss : vdd), i_pDataBase->XAB2, i, ALU1, 12 * SCALE_X);

        //addphcon(i_pPhFig, EAST,
        //         (start_with_vss ? vss : vdd), i_pDataBase->XAB2, i + 3 * SCALE_X, ALU1, 600);

        l_uIndex++;

    }

    for (i = PITCH * 10 * SCALE_X; i < i_pDataBase->YAB2;
         i += 2 * PITCH * 10 * SCALE_X) {

        addphseg(i_pPhFig, ALU1, 12 * SCALE_X,
                 i_pDataBase->XAB1, i,
                 i_pDataBase->XAB2, i, (start_with_vss ? vdd : vss) );

        addphcon(i_pPhFig, WEST,
                 (start_with_vss ? vdd : vss), i_pDataBase->XAB1, i, ALU1, 12 * SCALE_X);

        //addphcon(i_pPhFig, WEST,
        //         (start_with_vss ? vdd : vss), i_pDataBase->XAB1, i + 3 * SCALE_X, ALU1, 600);

        addphcon(i_pPhFig, EAST,
                 (start_with_vss ? vdd : vss), i_pDataBase->XAB2, i, ALU1, 12 * SCALE_X);

        //addphcon(i_pPhFig, EAST,
        //         (start_with_vss ? vdd : vss), i_pDataBase->XAB2, i + 3 * SCALE_X, ALU1, 600);


        l_uIndex++;
    }

    addphcon(i_pPhFig, WEST,
             ( (
                start_with_vss
                ^ (((i_pDataBase->YAB2 - i_pDataBase->YAB1) / (5 * SCALE_X) / 10) % 2)
               ) ? vss : vdd),
             i_pDataBase->XAB1,
             i_pDataBase->YAB2 - 3 * SCALE_X,
             ALU1, 6 * SCALE_X);
    addphcon(i_pPhFig, EAST,
             ( (
                start_with_vss
                ^ (((i_pDataBase->YAB2 - i_pDataBase->YAB1) / (5 * SCALE_X) / 10) % 2)
               ) ? vss : vdd),
             i_pDataBase->XAB2,
             i_pDataBase->YAB2 - 3 * SCALE_X,
             ALU1, 6 * SCALE_X);

}

void dumpPhFigToDisk(char *i_sName, phfig_list * i_pPhFig)
{
    char l_sName[50];

    strcpy(l_sName, i_sName);
    //strcat(l_sName, "_ocr");
    i_pPhFig->NAME = namealloc(l_sName);
    savephfig(i_pPhFig);
//       viewphfig(i_pPhFig);

    display(LEVEL, VERBOSE, "o Dumping PhFig to %s.ap ...\n", l_sName);

}

ocrNaturalInt
dumpGridToPhFig(ocrRoutingParameters * i_pParam,
                ocrWRoutingGrid * i_pGrid, phfig_list * i_pPhFig)
{
    ocrNaturalInt x, y, z;
    ocrNaturalInt n = 0;
    ocrWSegment *l_pSegment;

    display(LEVEL, VERBOSE, "o Dumping Grid in phfig ...\n");

    // parcours de la grille à la recherche des segments ...
    for (x = 0; x < i_pGrid->SIZE_H; x++)
        for (y = 0; y < i_pGrid->SIZE_V; y++)
            for (z = 0; z < i_pGrid->NB_OF_LAYERS; z++) {
                l_pSegment = getWSegment(i_pGrid, x, y, z);

                if (l_pSegment
                    && (l_pSegment->SIGNAL_INDEX != WSEGMENT_FREE)
                    && (l_pSegment->SIGNAL_INDEX != WSEGMENT_OBSTACLE)) {
                    // parcours le signal pour le stocker dans i_pPhFig
                    dumpSignal(i_pParam, i_pGrid, i_pPhFig, x, y, z);
                    // incrémentation
                    n++;
                }
            }

    display(LEVEL, VERBOSE, "o %ld signals\n", n);

    return 0;
}
