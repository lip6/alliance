/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/20 13:25:50 $
   $Log: ocrGlobalRouting.c,v $
   Revision 1.2  2002/03/20 13:25:50  hcl
   SymX bug.

   Revision 1.1  2002/03/15 14:37:19  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:17  hcl
   First commit in the new tree.

   Revision 1.6  2002/01/25 13:45:48  hcl
   Bug affichage.

   Revision 1.5  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.4  2001/12/14 15:58:36  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.3  2001/12/12 14:50:58  hcl
   Prise en compte du placement des connecteurs par OCP.

   Revision 1.2  2001/11/20 09:41:09  hcl
   chez Francis, on y route le mips ...

   Revision 1.1  2001/09/26 14:27:49  hcl
   premier commit....

   ### -------------------------------------------------- ###
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"

#include "ocr.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "ocrUtil.h"
#include "ocrPath.h"
#include "ocrMst.h"
#include "ocrWeightedTree.h"
#include "display.h"
#include "ocrGlobalRouting.h"
#include "ocrConnectorUtil.h"
#include "ocrNpoints.h"

#define SCALE_X 100
#define PITCH 5

extern ocrOption *g_pOption;

#define LEVEL (g_pOption->LEVEL)

void makeGlobalSignals(ocrRoutingDataBase * i_pDataBase)
{
    ocrSignal *l_pSignal;
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirConGlobal;
    ocrNaturalShort *l_pGrille;
    ocrNaturalInt i;

    // Création d'une grille de fenetre pour créer 1 seul connecteur 
    // Virtuel par fenetre.
    l_pGrille = (ocrNaturalShort *) mbkalloc((i_pDataBase->NB_F + 1) *
                                             sizeof(ocrNaturalShort));

    // les connecteurs virtuels (CV) pour le routage global, sont 
    // placés dans une liste : GLOBAL.
    for (l_pSignal = i_pDataBase->FSIGNAL[0];
         l_pSignal; l_pSignal = l_pSignal->NEXT) {
//      dumpSignalToFile (l_pSignal, stdout);
        // Initialise la grille
        for (i = 1; i <= i_pDataBase->NB_F; i++)
            l_pGrille[i] = 0;

        for (l_pCon = l_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
            if (l_pCon->INTEXT == INTERNAL) {
                // existe-t-il un CV dans cette fenetre
                if (!l_pGrille[l_pCon->VIR_CON_LIST->WIN]) {
                    // Calcul pour chaque CV les coordonnées de sa fenetre
                    l_pVirConGlobal =
                        dupVirtualConnector(l_pCon->VIR_CON_LIST);
                    l_pVirConGlobal->X =
                        (l_pVirConGlobal->WIN -
                         1) % ((int) sqrt(i_pDataBase->NB_F));
                    l_pVirConGlobal->Y =
                        (l_pVirConGlobal->WIN -
                         1) / ((int) sqrt(i_pDataBase->NB_F));

                    // ajout du CV à la liste
                    addVirtualConnector(&l_pSignal->GLOBAL,
                                        l_pVirConGlobal);
                    // notifie que la fenetre possede alors un CV
                    l_pGrille[l_pVirConGlobal->WIN] = 1;

                    l_pCon->CON = NULL;
#if 0
                    display(LEVEL, DEBUG, "%ld %ld %d %ld %ld\n",
                            l_pCon->VIR_CON_LIST->X,
                            l_pCon->VIR_CON_LIST->Y,
                            l_pCon->WIN, l_pVirConGlobal->X,
                            l_pVirConGlobal->Y);
#endif
                }
            } else {
                // existe-t-il un CV dans cette fenetre
                if (!l_pGrille[l_pCon->WIN]) {
                    ocrNaturalInt l_uX, l_uY;

                    WintoXY(i_pDataBase, &l_uX, &l_uY, l_pCon->WIN);
                    l_pVirConGlobal =
                        createVirtualConnector(l_uX, l_uY, 0, 0, 0);
                    // ajout du CV à la liste
                    addVirtualConnector(&l_pSignal->GLOBAL,
                                        l_pVirConGlobal);
                    // notifie que la fenetre possede alors un CV
                    l_pGrille[l_pVirConGlobal->WIN] = 1;
                }
            }
        }
    }
    mbkfree(l_pGrille);
}

ocrVirtualConnector *chooseConInEqui(ocrVirtualConnector * i_pEqui,
                                     ocrVirtualConnector * i_pVirCon)
{
    ocrVirtualConnector *l_pVirCon, *l_pRes = NULL;
    ocrNaturalInt l_uDistMin = OCRNATURALINT_MAX;
    ocrNaturalInt l_uDist = 0;

    for (l_pVirCon = i_pEqui;
         l_pVirCon; l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
        l_uDist = distBetween2VirCon(l_pVirCon, i_pVirCon);

        if (l_uDist < l_uDistMin) {
            l_uDistMin = l_uDist;
            l_pRes = l_pVirCon;
        }
    }
    return l_pRes;
}

void routingGlobalSignals(ocrRoutingDataBase * i_pDataBase)
{
    ocrSignal *l_pSignal;
    ocrVirtualConnector *l_pVirCon, *l_pVirCon2;
    ocrVirtualConnector *l_pEqui = NULL;

    // allocation de la grille
    i_pDataBase->GRID = createWGrid(i_pDataBase->NB_F + 1,
                                    i_pDataBase->NB_F + 1, 2);


    for (l_pSignal = i_pDataBase->FSIGNAL[0];
         l_pSignal; l_pSignal = l_pSignal->NEXT)
//    if (l_pSignal->INTEXT == INTERNAL)
    {
        // Initialiation de la Grille
        initWGrid(i_pDataBase->GRID, i_pDataBase->PARAM);

        l_pVirCon = l_pSignal->GLOBAL;
        l_pVirCon2 = (ocrVirtualConnector *) l_pVirCon->NEXT;
        while (l_pVirCon2) {
            if (isFreePoint(NULL, i_pDataBase->GRID, l_pVirCon2)) {
                findPathBetween2Points(i_pDataBase->PARAM,
                                       i_pDataBase->GRID,
                                       l_pVirCon->X,
                                       l_pVirCon->Y,
                                       l_pVirCon->Z,
                                       l_pVirCon2->X,
                                       l_pVirCon2->Y,
                                       l_pVirCon2->Z, l_pSignal->INDEX,
                                       l_pSignal);
                //if ( ((l_pVirCon->Z) > 0) || ((l_pVirCon2->Z) > 0) )

                unifyPoint(i_pDataBase->GRID,
                           i_pDataBase->PARAM,
                           l_pSignal,
                           l_pVirCon->X, l_pVirCon->Y, l_pVirCon->Z);

                unifyPoint(i_pDataBase->GRID,
                           i_pDataBase->PARAM,
                           l_pSignal, l_pVirCon2->X, l_pVirCon2->Y,
                           l_pVirCon2->Z);

                l_pEqui = makeEquipotentielle(i_pDataBase->PARAM,
                                              i_pDataBase->GRID,
                                              l_pSignal);
            }
            l_pVirCon2 = (ocrVirtualConnector *) l_pVirCon2->NEXT;
            if (l_pVirCon2) {
                l_pVirCon = chooseConInEqui(l_pEqui, l_pVirCon2);
            }
        }
        deleteEquipotentielle(i_pDataBase->PARAM,
                              i_pDataBase->GRID, l_pSignal);
    }

    // libération de la grille
    freeWGrid(i_pDataBase->GRID);
    i_pDataBase->GRID = NULL;
}

/**
 * makeSubSignal : crée un signal local à une window
 * le rajoute à la fenetre en question en début de liste.
**/
void
makeSubSignal(ocrRoutingDataBase * i_pDataBase,
              ocrSignal * i_pSignal, ocrNaturalInt i_uWindow)
{
    ocrSignal *l_pLocalSignal;

    l_pLocalSignal = dupSignal(i_pSignal);
    l_pLocalSignal->WIN = i_uWindow;
    l_pLocalSignal->PRIORITY = 1;
    insertSignalWindow(i_pDataBase, l_pLocalSignal);
}

/**
 * makeLocalSignals : "découpe" les signaux globaux en signaux locaux.
**/

void makeLocalSignals(ocrRoutingDataBase * i_pDataBase)
{
    ocrSignal *l_pSignal;
    ocrConnector *l_pCon;
    ocrConnector *l_pConNext;
    ocrWSegment *l_pSegment;
    ocrNaturalInt i, l_uWin;
    char l_sName[50];

    // découpage en signaux locaux :
    // un sous signal dans chaque window.
    // les connecteurs dans les bonnes window.
    // ajouter les connecteurs de face.

    for (l_pSignal = i_pDataBase->FSIGNAL[0];
         l_pSignal; l_pSignal = l_pSignal->NEXT) {
        // Chaque connecteur est ajouté au bon signal local
        l_pConNext = l_pCon = l_pSignal->CON_LIST;
        while (l_pConNext) {
            l_pConNext = l_pCon->NEXT;
            // Création du sous signal dans la Window l_pCon->WIN ?
            if (i_pDataBase->FSIGNAL[l_pCon->WIN]->INDEX !=
                l_pSignal->INDEX) {
                makeSubSignal(i_pDataBase, l_pSignal, l_pCon->WIN);
            }

            //addConnector(i_pDataBase->FSIGNAL[l_pCon->WIN], l_pCon);
            l_pCon = l_pConNext;
        }

#if 0

        // Ajout des connecteurs Faciaux
        for (l_pSegment = l_pSignal->SEGMENT;
             l_pSegment; l_pSegment = l_pSegment->NEXT) {
            if (getWSegDirection(i_pDataBase->PARAM,
                                 l_pSegment) == ocrHorizontal) {
                for (i = l_pSegment->P_MIN; i < l_pSegment->P_MAX; i++) {
                    XYtoWin(i_pDataBase, i, l_pSegment->OFFSET, &l_uWin);
                    sprintf(l_sName, "%ld_%ld", l_pSignal->INDEX, l_uWin);
                    l_pCon = createConnector(l_sName, NULL,     //INSNAME 
                                             OCR_TYPE_FACE,     // TYPE
                                             EXTERNAL,  // INTEXT
                                             OCR_FACE_EAST,     // FACE
                                             0, // ORDER
                                             l_uWin);   // NUM F

                    if (i_pDataBase->FSIGNAL[l_pCon->WIN]->INDEX !=
                        l_pSignal->INDEX) {
                        makeSubSignal(i_pDataBase, l_pSignal, l_pCon->WIN);
                        MARK_AS_SIG_THROW(i_pDataBase->
                                          FSIGNAL[l_pCon->WIN]);
                        i_pDataBase->WINDOWS[l_pCon->WIN]->NB_SIG_THROW++;
                    }

                    addConnector(i_pDataBase->FSIGNAL[l_pCon->WIN],
                                 l_pCon);

                    XYtoWin(i_pDataBase, i + 1, l_pSegment->OFFSET,
                            &l_uWin);
                    sprintf(l_sName, "%ld_%ld", l_pSignal->INDEX, l_uWin);
                    l_pCon = createConnector(l_sName, NULL,     //INSNAME 
                                             OCR_TYPE_FACE,     // TYPE
                                             EXTERNAL,  // INTEXT
                                             OCR_FACE_WEST,     // FACE
                                             0, // ORDER
                                             l_uWin);   // NUM F

                    if (i_pDataBase->FSIGNAL[l_pCon->WIN]->INDEX !=
                        l_pSignal->INDEX) {
                        makeSubSignal(i_pDataBase, l_pSignal, l_pCon->WIN);
                        MARK_AS_SIG_THROW(i_pDataBase->
                                          FSIGNAL[l_pCon->WIN]);
                        i_pDataBase->WINDOWS[l_pCon->WIN]->NB_SIG_THROW++;
                    }

                    addConnector(i_pDataBase->FSIGNAL[l_pCon->WIN],
                                 l_pCon);

                }
            } else {
                for (i = l_pSegment->P_MIN; i < l_pSegment->P_MAX; i++) {
                    XYtoWin(i_pDataBase, l_pSegment->OFFSET, i, &l_uWin);
                    sprintf(l_sName, "%ld_%ld", l_pSignal->INDEX, l_uWin);
                    l_pCon = createConnector(l_sName, NULL,     //INSNAME 
                                             OCR_TYPE_FACE,     // TYPE
                                             EXTERNAL,  // INTEXT
                                             OCR_FACE_NORTH,    // FACE
                                             0, // ORDER
                                             l_uWin);   // NUM F

                    if (i_pDataBase->FSIGNAL[l_pCon->WIN]->INDEX !=
                        l_pSignal->INDEX) {
                        makeSubSignal(i_pDataBase, l_pSignal, l_pCon->WIN);
                        MARK_AS_SIG_THROW(i_pDataBase->
                                          FSIGNAL[l_pCon->WIN]);
                        i_pDataBase->WINDOWS[l_pCon->WIN]->NB_SIG_THROW++;
                    }

                    addConnector(i_pDataBase->FSIGNAL[l_pCon->WIN],
                                 l_pCon);

                    XYtoWin(i_pDataBase, l_pSegment->OFFSET, i + 1,
                            &l_uWin);
                    sprintf(l_sName, "%ld_%ld", l_pSignal->INDEX, l_uWin);
                    l_pCon = createConnector(l_sName, NULL,     //INSNAME 
                                             OCR_TYPE_FACE,     // TYPE
                                             EXTERNAL,  // INTEXT
                                             OCR_FACE_SOUTH, 0, // ORDER
                                             l_uWin);   // NUM F

                    if (i_pDataBase->FSIGNAL[l_pCon->WIN]->INDEX !=
                        l_pSignal->INDEX) {
                        makeSubSignal(i_pDataBase, l_pSignal, l_pCon->WIN);
                        MARK_AS_SIG_THROW(i_pDataBase->
                                          FSIGNAL[l_pCon->WIN]);
                        i_pDataBase->WINDOWS[l_pCon->WIN]->NB_SIG_THROW++;
                    }

                    addConnector(i_pDataBase->FSIGNAL[l_pCon->WIN],
                                 l_pCon);

                }

            }
        }
#endif
    }
}

/**
 * Evaluation du Routage Global
 * Donne les contraintes sur les faces de tous les window
**/

void evalGlobalRouting(ocrRoutingDataBase * i_pDataBase)
{
    ocrSignal *l_pSignal;
    ocrConnector *l_pCon;
    ocrNaturalInt i;
    ocrNaturalInt n[10];

    display(LEVEL, DEBUG, "o Eval ...\n");
    for (i = 1; i <= i_pDataBase->NB_F; i++) {
        n[OCR_FACE_EAST] = 0;
        n[OCR_FACE_NORTH] = 0;
        n[OCR_FACE_SOUTH] = 0;
        n[OCR_FACE_WEST] = 0;

        for (l_pSignal = i_pDataBase->FSIGNAL[i];
             l_pSignal; l_pSignal = l_pSignal->NEXT) {
            for (l_pCon = l_pSignal->CON_LIST; l_pCon;
                 l_pCon = l_pCon->NEXT) {
                if (IS_MARK_AS_FACE_CON(l_pCon))
                    n[l_pCon->FACE]++;

            }
        }
        i_pDataBase->WINDOWS[i]->NB_SIG_FACE[OCR_FACE_EAST] =
            n[OCR_FACE_EAST];
        i_pDataBase->WINDOWS[i]->NB_SIG_FACE[OCR_FACE_WEST] =
            n[OCR_FACE_WEST];
        i_pDataBase->WINDOWS[i]->NB_SIG_FACE[OCR_FACE_NORTH] =
            n[OCR_FACE_NORTH];
        i_pDataBase->WINDOWS[i]->NB_SIG_FACE[OCR_FACE_SOUTH] =
            n[OCR_FACE_SOUTH];

        display(LEVEL, DEBUG,
                "win %ld : N %ld, S %ld, E %ld, W %ld. Through = %ld, loc sig %ld\n",
                i, n[OCR_FACE_NORTH], n[OCR_FACE_SOUTH], n[OCR_FACE_EAST],
                n[OCR_FACE_WEST], i_pDataBase->WINDOWS[i]->NB_SIG_THROW,
                i_pDataBase->WINDOWS[i]->NB_SIG);
    }
}


/**
 * Routage Global
 *  1. Agrégation des connecteurs de la meme fenetre
 *     Préparation des signaux globaux pour le routage
 *  2. Routage des signaux globaux
 *  3. Création des signaux locaux
**/
ocrNaturalInt globalRouting(ocrRoutingDataBase * i_pDataBase)
{
    ocrEdge *l_pEdge;
    ocrSignal *l_pSignal;

    display(LEVEL, VERBOSE, "o Global routing ...\n");
    //return;

    //makeGlobalSignals(i_pDataBase);
#if 1
#if 0
    initWeightedTree(i_pDataBase);
    //dumpDensityTable();

    for (l_pSignal = i_pDataBase->FSIGNAL[0];
         l_pSignal; l_pSignal = l_pSignal->NEXT) {
        l_pSignal->SEGMENT = NULL;
        l_pEdge = makingMst(l_pSignal);
        makingWeightedTree(i_pDataBase, l_pSignal, l_pEdge);
    }

    //dumpDensityTable();
    freeWeightedTree();
#endif
#else
    routingGlobalSignals(i_pDataBase);
    //  dumpDataBase (i_pDataBase, stdout);
#endif
    makeLocalSignals(i_pDataBase);
    evalGlobalRouting(i_pDataBase);

    return 1;
}
