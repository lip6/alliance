/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/04/25 13:41:32 $
   $Log: ocrRouter.c,v $
   Revision 1.6  2002/04/25 13:41:32  hcl
   New ripup/reroute loop, bug-kill (CALU&TALU).

   Revision 1.4  2002/04/03 09:52:19  hcl
   A little bit of C++.

   Revision 1.3  2002/03/22 09:41:09  hcl
   ALLIANCE_VERSION

   Revision 1.2  2002/03/20 13:25:50  hcl
   SymX bug.

   Revision 1.1  2002/03/15 14:37:20  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:18  hcl
   First commit in the new tree.

   Revision 1.18  2002/02/22 15:20:49  hcl
   A* is ready

   Revision 1.17  2002/02/21 13:17:57  hcl
   Introducing a new algo (A*, dont yet work...)

   Revision 1.16  2002/02/20 16:31:47  xtof
   syntax errors ....

   Revision 1.15  2002/02/12 15:14:15  hcl
   New algo.

   Revision 1.14  2002/01/14 10:34:33  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.13  2001/12/20 15:44:17  hcl
   ALU5 et ALU6 : distance min respectee.

   Revision 1.12  2001/12/20 13:05:09  hcl
   Un oubli.

   Revision 1.11  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.10  2001/12/19 14:22:49  hcl
   Moult petits changements.

   Revision 1.9  2001/12/17 14:29:32  hcl
   VDD/VSS rail bug, new ripUp.

   Revision 1.8  2001/12/14 15:58:36  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.7  2001/12/12 14:50:58  hcl
   Prise en compte du placement des connecteurs par OCP.

   Revision 1.6  2001/12/10 13:08:53  hcl
   Un bigbug en moins.

   Revision 1.5  2001/12/03 16:45:04  hcl
   Corrections dans ripUp2

   Revision 1.4  2001/12/03 14:46:01  hcl
   ripUp plus agressif.

   Revision 1.3  2001/12/03 14:31:21  hcl
   Pour la route.

   Revision 1.2  2001/11/20 09:41:09  hcl
   chez Francis, on y route le mips ...

   Revision 1.1  2001/09/26 14:27:49  hcl
   premier commit....

   ### -------------------------------------------------- ###
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
#include "display.h"
#include "ocrNpoints.h"
#include "ocrGlobalRouting.h"
#include "ocrWRoutingUtil.h"
#include "ocrInitDataBase.h"
#include "ocrSignalUtil.h"
#include "ocrToPhFig.h"


#define FINDNPATH findPathNPoints
#define FINDPATH findPathBetween2Points

/*ocrNaturalInt ITMAX = OCRNATURALINT_MAX;*/
ocrNaturalInt ITMAX = 15;
ocrNaturalInt g_OptLayer = 2;
ocrNaturalInt g_OptWindow = 1;
ocrOption *g_pOption;

#define LEVEL (g_pOption->LEVEL)

/*
 * readPhFig : 
 *  1 - Lecture de la figure i_sPhFigName
 *  2 - Translation vers (0,0)
 */
phfig_list *readPhFig(char *i_sPhFigName)
{
    phfig_list *l_pPhFig;
    phins_list *l_pInst;
    phseg_list *l_pSeg;
    phcon_list *l_pCon;
    phvia_list *l_pVia;
    phref_list *l_pRef;
    ocrNaturalInt l_uDeltaX;
    ocrNaturalInt l_uDeltaY;

    display(LEVEL, VERBOSE, "%s\n", "o Read .ap ...");
    l_pPhFig = getphfig(i_sPhFigName, 'A');

//FIXME::pour l'instant pas de reentrance: donc flat jusqu'au CATAL

    rflattenphfig(l_pPhFig, YES, YES);

    if ((l_pPhFig->XAB1 != 0) || (l_pPhFig->YAB1 != 0)) {
        display(LEVEL, VERBOSE, "  %s\n", "o Translating .ap ...\n");

        l_uDeltaX = 0 - l_pPhFig->XAB1;
        l_uDeltaY = 0 - l_pPhFig->YAB1;

        // phins
        for (l_pInst = l_pPhFig->PHINS; l_pInst; l_pInst = l_pInst->NEXT) {
            l_pInst->XINS += l_uDeltaX;
            l_pInst->YINS += l_uDeltaY;
        }
        for (l_pSeg = l_pPhFig->PHSEG; l_pSeg; l_pSeg = l_pSeg->NEXT) {
            l_pSeg->X1 += l_uDeltaX;
            l_pSeg->X2 += l_uDeltaX;
            l_pSeg->Y1 += l_uDeltaY;
            l_pSeg->Y2 += l_uDeltaY;
        }
        for (l_pVia = l_pPhFig->PHVIA; l_pVia; l_pVia = l_pVia->NEXT) {
            l_pVia->XVIA += l_uDeltaX;
            l_pVia->YVIA += l_uDeltaY;
        }
        for (l_pRef = l_pPhFig->PHREF; l_pRef; l_pRef = l_pRef->NEXT) {
            l_pRef->XREF += l_uDeltaX;
            l_pRef->YREF += l_uDeltaY;
        }
        for (l_pCon = l_pPhFig->PHCON; l_pCon; l_pCon = l_pCon->NEXT) {
            l_pCon->XCON += l_uDeltaX;
            l_pCon->YCON += l_uDeltaY;
        }

        // phfig abutment box
        l_pPhFig->XAB1 += l_uDeltaX;
        l_pPhFig->XAB2 += l_uDeltaX;
        l_pPhFig->YAB1 += l_uDeltaY;
        l_pPhFig->YAB2 += l_uDeltaY;

    }

    return l_pPhFig;
}

ocrSignal *findSignal(ocrRoutingDataBase * i_pDataBase,
                      ocrNaturalInt i_uIndex)
{
    ocrNaturalInt i;

    if (i_uIndex == OCRNATURALINT_MAX)
        return NULL;

    for (i = 0; i < i_pDataBase->NB_GSIGNAL - 1; i++) {
        if (i_pDataBase->GSIGNAL[i]->INDEX == i_uIndex)
            return i_pDataBase->GSIGNAL[i];
    }
    return NULL;
}

void noCritVC(ocrRoutingDataBase * i_pDataBase)
{

    ocrNaturalInt i;
    ocrWRoutingGrid *l_pGrid = i_pDataBase->GRID;

    // tous les sigs
    for (i = 0; i < i_pDataBase->NB_SIGNAL; i++) {
        ocrConnector *l_pCon;

        // tous les cons
        for (l_pCon = i_pDataBase->SIGNAL[i]->CON_LIST; l_pCon;
             l_pCon = l_pCon->NEXT) {
            l_pCon->critVC = NULL;
        }
    }
// maj con->NB_VC


    return;
}


/**
 * route les signaux dans l'ordre :
 *  1. i_pSignal
 *  2. les signaux qui ont ete supprimes par ripUp
 *
 *  return :
 *   1 OK
 *   0 ECHEC
 **/
ocrNaturalInt
reRoute(ocrRoutingDataBase * i_pDataBase,
       ocrRoutingParameters * i_pParam, ocrSignal * i_pSignal)
{
    ocrSignal *l_pSignal;
    ocrNaturalInt l_uLength;
    ocrNaturalInt l_uReturnValue = 1;
    chain_list *l_pSignalList;
    ocrConnector *l_pCon;

    if (i_pSignal == NULL)
        goto skip;
    // routage du signal désigné par i_pSignal
    //  fprintf (stdout, "reRoute : Routage du signal %ld \n",
    //         i_pSignal->INDEX);
    markSegmentAsFree(i_pDataBase, i_pSignal, i_pSignal->INDEX);
    for (l_pCon = i_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
        if (l_pCon->critVC) {
            unProtectVC(i_pDataBase, l_pCon->critVC);
        }
    }
    l_uLength = FINDNPATH(i_pParam, i_pDataBase->GRID,
                          i_pSignal, i_pDataBase->WINDOWS[i_pSignal->WIN]);

    if (l_uLength != OCRNATURALINT_MAX) {
        // Succes du routage
        i_pDataBase->NB_ROUTED++;

        i_pSignal->ROUTED = 1;

        display(LEVEL, VVERB, "o rerouted %s\n", i_pSignal->NAME);

    } else {
        l_uReturnValue = 0;
        display(LEVEL, VVERB, "o Failed to reroute %s\n", i_pSignal->NAME);

        i_pSignal->ROUTED = 2;
        deleteEquipotentielle(i_pParam, i_pDataBase->GRID, i_pSignal);
        i_pSignal->SEGMENT = NULL;

        i_pDataBase->NB_UNROUTED++;
        i_pSignal->PRIORITY = i_pDataBase->NB_IT;
    }
    unMarkSegmentAsFree(i_pDataBase, i_pSignal, i_pSignal->INDEX);      // XXX

skip:

    // tant qu'il reste des signaux à re router
    l_pSignalList = i_pDataBase->RIPUP;
    while (l_pSignalList) {

        l_pSignal = (ocrSignal *) l_pSignalList->DATA;
        if (l_pSignal->ROUTED == 1) {
            l_pSignalList = l_pSignalList->NEXT;
            continue;
        }

//    fprintf (stdout, "reRoute : Routage du signal %ld \n",
        //             l_pSignal->INDEX);
        markSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);
        for (l_pCon = l_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
            if (l_pCon->critVC) {
                unProtectVC(i_pDataBase, l_pCon->critVC);
            }
        }
        display(LEVEL, VVERB, "o Rerouting %s\n", l_pSignal->NAME);
        l_uLength = FINDNPATH(i_pParam, i_pDataBase->GRID,
                              l_pSignal,
                              i_pDataBase->WINDOWS[l_pSignal->WIN]);

        l_pSignalList = i_pDataBase->RIPUP;

        if (l_uLength == OCRNATURALINT_MAX) {
            l_uReturnValue = 0;
            display(LEVEL, VVERB, "\n--- failed to reroute %s\n", l_pSignal->NAME);
            l_pSignal->ROUTED = 2;

            ripUp2 (i_pDataBase, i_pParam, l_pSignal);
            //deleteEquipotentielle(i_pParam, i_pDataBase->GRID, l_pSignal);

            //l_pSignal->SEGMENT = NULL;

            //i_pDataBase->NB_UNROUTED++;
            l_pSignal->PRIORITY = i_pDataBase->NB_IT+1;
        } else {
            i_pDataBase->NB_ROUTED++;
            l_pSignal->ROUTED = 1;
            l_pSignal->HARD ++;
            display(LEVEL, VVERB, "+++ %s rerouted\n", l_pSignal->NAME);
        }
        unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
        countFreeVC(i_pDataBase);

        //l_pSignalList = l_pSignalList->NEXT;
    }

    freechain(i_pDataBase->RIPUP);
    i_pDataBase->RIPUP = NULL;
    return l_uReturnValue;
}

/**
 * supprime les equipotentielles qui empeche le routage
 * du signal i_pSignal
 **/
void
ripUp(ocrRoutingDataBase * i_pDataBase,
      ocrRoutingParameters * i_pParam, ocrSignal * i_pSignal)
{
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirCon;
    ocrSignal *l_pSignal;

    // recherche du connecteur qui pose un problème
    for (l_pCon = i_pSignal->CON_LIST;
         l_pCon && l_pCon->CON; l_pCon = l_pCon->NEXT);

    if (l_pCon) {
        // parcours des connecteurs virtuels
        for (l_pVirCon = l_pCon->VIR_CON_LIST;
             l_pVirCon;
             l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
            // recherche du signal qui gene
            l_pSignal = findSignal(i_pDataBase,
                                   getWSegmentCV(i_pDataBase->GRID,
                                                 l_pVirCon)->SIGNAL_INDEX);
            if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                // ajoute le signal dans la liste à re-router.
                i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                              (void *) l_pSignal);

                // suppression de son equipotentielle
                //                        fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                //                                 l_pSignal->INDEX);

                deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                      l_pSignal);
                l_pSignal->SEGMENT = NULL;
                l_pSignal->ROUTED = 0;
                i_pDataBase->NB_ROUTED--;
                unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
            }
            l_pSignal = findSignal(i_pDataBase,
                                   getWSegment(i_pDataBase->GRID,
                                               l_pVirCon->X,
                                               l_pVirCon->Y,
                                               l_pVirCon->Z +
                                               1)->SIGNAL_INDEX);
            if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                // ajoute le signal dans la liste à re-router.
                i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                              (void *) l_pSignal);

                // suppression de son equipotentielle
                //                        fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                //                                 l_pSignal->INDEX);

                deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                      l_pSignal);
                l_pSignal->SEGMENT = NULL;
                l_pSignal->ROUTED = 0;
                i_pDataBase->NB_ROUTED--;
                unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
            }
        }
        // supression du signal lui meme
        //    fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
        //             i_pSignal->INDEX);
        deleteEquipotentielle(i_pParam, i_pDataBase->GRID, i_pSignal);
        unMarkSegmentAsFree(i_pDataBase, i_pSignal, i_pSignal->INDEX);  // XXX
        i_pSignal->SEGMENT = NULL;
        i_pSignal->ROUTED = 0;
        countFreeVC(i_pDataBase);
    }
}


/* On rippe plus de signaux */
void
ripUp2(ocrRoutingDataBase * i_pDataBase,
       ocrRoutingParameters * i_pParam, ocrSignal * i_pSignal)
{
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirCon;
    ocrSignal *l_pSignal;

    for (l_pCon = i_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
        // parcours des connecteurs virtuels
        for (l_pVirCon = l_pCon->VIR_CON_LIST;
             l_pVirCon;
             l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
            //printf ("(%ld,%ld,%ld)\n", l_pVirCon->X, l_pVirCon->Y, l_pVirCon->Z);
            // altitude du connecteur
            if (l_pVirCon->Z == 0) {
                // on déroute sur les 2 niveaux au dessus (ALU2 et ALU3)

                // ALU2
                l_pSignal = findSignal(i_pDataBase,
                                       getWSegmentCV(i_pDataBase->GRID,
                                                     l_pVirCon)->
                                       SIGNAL_INDEX);
                if (l_pSignal)
                    if ( (l_pSignal != i_pSignal) && (!(l_pSignal->HARD)) ) {
                        // ajoute le signal dans la liste à re-router.
                        i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                      (void *) l_pSignal);


                        //fprintf (stdout, "RIPUP : Suppression du signal (ALU2) %ld \n", l_pSignal->INDEX);
                        deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                              l_pSignal);
                        l_pSignal->SEGMENT = NULL;
                        l_pSignal->ROUTED = 0;
                        i_pDataBase->NB_ROUTED--;
                        unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                    }
                // ALU3
                l_pSignal = findSignal(i_pDataBase,
                                       getWSegment(i_pDataBase->GRID,
                                                   l_pVirCon->X,
                                                   l_pVirCon->Y,
                                                   l_pVirCon->Z +
                                                   1)->SIGNAL_INDEX);
                if (l_pSignal)
                    if (l_pSignal != i_pSignal) {
                        // ajoute le signal dans la liste à re-router.
                        i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                      (void *) l_pSignal);


                        //fprintf (stdout, "RIPUP : Suppression du signal (ALU3) %ld \n", l_pSignal->INDEX);
                        deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                              l_pSignal);
                        l_pSignal->SEGMENT = NULL;
                        l_pSignal->ROUTED = 0;
                        i_pDataBase->NB_ROUTED--;
                        unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                    }


            } else {
                // altitude du connecteur > 0
                // premiere version : on ne fait rien.
                //
                // future version :
                // on déroute immédiatement au dessus si possible
                // et sur chaque côté en suivant la direction du plan.
            }

        }
        // supression du signal lui meme
    }
    //fprintf (stdout, "RIPUP : Suppression du signal %ld \n", i_pSignal->INDEX);
    deleteEquipotentielle(i_pParam, i_pDataBase->GRID, i_pSignal);
    unMarkSegmentAsFree(i_pDataBase, i_pSignal, i_pSignal->INDEX);      // XXX
    i_pSignal->SEGMENT = NULL;
    i_pSignal->ROUTED = 0;
    countFreeVC(i_pDataBase);
}

/* ripUp plus subtil */
void
ripUp3(ocrRoutingDataBase * i_pDataBase,
      ocrRoutingParameters * i_pParam, ocrSignal * i_pSignal)
{
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirCon;
    ocrSignal *l_pSignal;
    ocrNaturalInt i;

    // recherche du connecteur qui pose un problème
    //for (l_pCon = i_pSignal->CON_LIST;
    //     l_pCon && l_pCon->CON; l_pCon = l_pCon->NEXT);
    //
    //if (l_pCon) {
    for (l_pCon = i_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
        // parcours des connecteurs virtuels
        for (l_pVirCon = l_pCon->VIR_CON_LIST;
             l_pVirCon;
             l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
            ocrWSegment *l_pSeg;
            // recherche du signal qui gene
            l_pSeg = getWSegmentCV (i_pDataBase->GRID, l_pVirCon);
          
            if (l_pSeg->SIGNAL_INDEX != WSEGMENT_FREE) {
           
                l_pSignal = findSignal(i_pDataBase,
                                     l_pSeg->SIGNAL_INDEX);
                if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                    // ajoute le signal dans la liste à re-router.
                    i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                  (void *) l_pSignal);

                    // suppression de son equipotentielle
                    //                        fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                    //                                 l_pSignal->INDEX);

                    deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                          l_pSignal);
                    l_pSignal->SEGMENT = NULL;
                    l_pSignal->ROUTED = 0;
                    i_pDataBase->NB_ROUTED--;
                    unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                }
                l_pSignal = findSignal(i_pDataBase,
                                       getWSegment(i_pDataBase->GRID,
                                                   l_pVirCon->X,
                                                   l_pVirCon->Y,
                                                   l_pVirCon->Z +
                                                   1)->SIGNAL_INDEX);
                if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                    // ajoute le signal dans la liste à re-router.
                    i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                  (void *) l_pSignal);

                    // suppression de son equipotentielle
                    //                        fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                    //                                 l_pSignal->INDEX);

                    deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                          l_pSignal);
                    l_pSignal->SEGMENT = NULL;
                    l_pSignal->ROUTED = 0;
                    i_pDataBase->NB_ROUTED--;
                    unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                }
            } else {
                ocrWSegment *segaux;

                // Segment libre : on efface tout ce qui passe au dessus et en dessous
                for (i = l_pSeg->P_MIN ; i <= l_pSeg->P_MAX ; i++) {

                    if (getWSegDirection(i_pParam, l_pSeg) == ocrHorizontal) {
                        if (l_pSeg->LAYER + 1 < i_pDataBase->NB_OF_LAYERS) {
                            segaux = getWSegment (i_pDataBase->GRID, i, l_pSeg->OFFSET, l_pSeg->LAYER + 1);
                            l_pSignal = findSignal(i_pDataBase, segaux->SIGNAL_INDEX);
                            if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                                // ajoute le signal dans la liste à re-router.
                                i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                              (void *) l_pSignal);

                                // suppression de son equipotentielle
                                //fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                                //         l_pSignal->INDEX);

                                deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                                      l_pSignal);
                                l_pSignal->SEGMENT = NULL;
                                l_pSignal->ROUTED = 0;
                                i_pDataBase->NB_ROUTED--;
                                unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                            }
                        }
                        if (l_pSeg->LAYER - 1 >= 0) {
                            segaux = getWSegment (i_pDataBase->GRID, i, l_pSeg->OFFSET, l_pSeg->LAYER - 1);
                            l_pSignal = findSignal(i_pDataBase, segaux->SIGNAL_INDEX);
                            if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                                // ajoute le signal dans la liste à re-router.
                                i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                              (void *) l_pSignal);

                                // suppression de son equipotentielle
                                //fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                                //         l_pSignal->INDEX);

                                deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                                      l_pSignal);
                                l_pSignal->SEGMENT = NULL;
                                l_pSignal->ROUTED = 0;
                                i_pDataBase->NB_ROUTED--;
                                unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                            }
                        }
                    } else {
                        // Vertical
                        if (l_pSeg->LAYER + 1 < i_pDataBase->NB_OF_LAYERS) {
                            segaux = getWSegment (i_pDataBase->GRID, l_pSeg->OFFSET, i, l_pSeg->LAYER + 1);
                            l_pSignal = findSignal(i_pDataBase, segaux->SIGNAL_INDEX);
                            if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                                // ajoute le signal dans la liste à re-router.
                                i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                              (void *) l_pSignal);

                                // suppression de son equipotentielle
                                //fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                                //         l_pSignal->INDEX);

                                deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                                      l_pSignal);
                                l_pSignal->SEGMENT = NULL;
                                l_pSignal->ROUTED = 0;
                                i_pDataBase->NB_ROUTED--;
                                unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                            }
                        }
                        if (l_pSeg->LAYER - 1 >= 0) {
                            segaux = getWSegment (i_pDataBase->GRID, l_pSeg->OFFSET, i, l_pSeg->LAYER - 1);
                            l_pSignal = findSignal(i_pDataBase, segaux->SIGNAL_INDEX);
                            if ((l_pSignal) && (l_pSignal->PRIORITY == 0)) {
                                // ajoute le signal dans la liste à re-router.
                                i_pDataBase->RIPUP = addchain(i_pDataBase->RIPUP,
                                                              (void *) l_pSignal);

                                // suppression de son equipotentielle
                                //fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
                                //         l_pSignal->INDEX);

                                deleteEquipotentielle(i_pParam, i_pDataBase->GRID,
                                                      l_pSignal);
                                l_pSignal->SEGMENT = NULL;
                                l_pSignal->ROUTED = 0;
                                i_pDataBase->NB_ROUTED--;
                                unMarkSegmentAsFree(i_pDataBase, l_pSignal, l_pSignal->INDEX);  // XXX
                            }
                        }

                    }
                }
            }
        }
        // supression du signal lui meme
        //    fprintf (stdout, "RIPUP : Suppression du signal %ld \n",
        //             i_pSignal->INDEX);
        deleteEquipotentielle(i_pParam, i_pDataBase->GRID, i_pSignal);
        unMarkSegmentAsFree(i_pDataBase, i_pSignal, i_pSignal->INDEX);  // XXX
        i_pSignal->SEGMENT = NULL;
        i_pSignal->ROUTED = 0;
        countFreeVC(i_pDataBase);
    }
}






void
addViaForIntersection(ocrRoutingParameters * i_pParam,
                      ocrWSegment * i_pSegment, phfig_list * i_pPhFig)
{
    ocrWSegment *l_pSegment;

    if (i_pSegment->NEXT != NULL) {
        for (l_pSegment = i_pSegment->NEXT;
             l_pSegment; l_pSegment = l_pSegment->NEXT) {
            if ((i_pSegment->LAYER - 1 == l_pSegment->LAYER) ||
                (i_pSegment->LAYER + 1 == l_pSegment->LAYER)) {
                if ((i_pSegment->OFFSET >= l_pSegment->P_MIN) &&
                    (i_pSegment->OFFSET <= l_pSegment->P_MAX) &&
                    (l_pSegment->OFFSET >= i_pSegment->P_MIN) &&
                    (l_pSegment->OFFSET <= i_pSegment->P_MAX)) {
                    if (getWSegDirection(i_pParam, i_pSegment) ==
                        ocrHorizontal) {
                        addViaToPhFig(i_pParam,
                                      l_pSegment->OFFSET,
                                      i_pSegment->OFFSET,
                                      MIN(i_pSegment->LAYER,
                                          l_pSegment->LAYER), i_pPhFig);
                    } else {
                        addViaToPhFig(i_pParam,
                                      i_pSegment->OFFSET,
                                      l_pSegment->OFFSET,
                                      MIN(i_pSegment->LAYER,
                                          l_pSegment->LAYER), i_pPhFig);
                    }
                }
            }
        }
    }
}

void
dumpSignalsToPhFig(ocrSignal * i_pSignal,
                   ocrRoutingParameters * i_pParam, phfig_list * i_pPhFig)
{
    ocrSignal *l_pSignal;
    ocrWSegment *l_pSegment;
    ocrNaturalInt nseg;

    for (l_pSignal = i_pSignal; l_pSignal; l_pSignal = l_pSignal->NEXT) {
        //nseg = 0;
        for (l_pSegment = l_pSignal->SEGMENT;
             l_pSegment; l_pSegment = l_pSegment->NEXT) {
            addSegmentToPhFig(i_pParam, l_pSegment, i_pPhFig, l_pSignal->NAME);
            addViaForIntersection(i_pParam, l_pSegment, i_pPhFig);
            //nseg ++;
        }
        //display (LEVEL, VVERB, "o %ld segments for %s\n", nseg, l_pSignal->NAME);
    }
}

ocrNaturalInt
getNewSignal(ocrRoutingDataBase * i_pDataBase, ocrNaturalInt i_uOldSignal)
{
    static ocrNaturalInt l_uRangSignal;
    ocrNaturalInt l_uNewSignal;

//  display (LEVEL, DEBUG, "%s\n", "getNewSignal");

    if (g_pOption->ORDER & OCR_ODYN) {
        // Initialisation
        if (i_uOldSignal == OCRNATURALINT_MAX) {
            l_uRangSignal = 0;
            return 0;
        }
        // Signal Critique ?
        l_uNewSignal = isCriticalSignal(i_pDataBase, i_uOldSignal);
        if (l_uNewSignal == OCRNATURALINT_MAX) {
            // non. rang suivant si possible
            while (l_uRangSignal < i_pDataBase->NB_SIGNAL - 1) {
                l_uRangSignal++;
                if (i_pDataBase->SIGNAL[l_uRangSignal]->ROUTED == 0) {
                    display(LEVEL, DEBUG, "o Signal %ld ", l_uRangSignal);
                    return l_uRangSignal;
                }
            }
            return OCRNATURALINT_MAX;
        } else {
            // oui signal critique; on ne modifie pas le rang !
            display(LEVEL, DEBUG, "o Signal %ld critical\n", l_uNewSignal);
            return l_uNewSignal;
        }
    } else {
        l_uNewSignal = i_uOldSignal + 1;
        if (l_uNewSignal == i_pDataBase->NB_SIGNAL)
            return OCRNATURALINT_MAX;
        else
            return l_uNewSignal;
    }
}






void dumpSigs(char *output, ocrRoutingDataBase * i_pDataBase,
              ocrSignal * i_pSignal, ocrWindow * win)
{
    char *name = (char *) mbkalloc((strlen(output) + 5) * sizeof(char *));
    FILE *dump;

    ocrVirtualConnector *l_pVirCon;
    ocrConnector *l_pCon;
    ocrWSegment *l_pSeg;


    name = strdup(output);
    strcat(name, ".gpl");
    dump = fopen(name, "w");

    if (!dump) {
        printf("erreur fopen\n");
        exit(-1);
    }

    fprintf(dump, "set noxtics\nset noytics\nset noborder\n");
    fprintf(dump, "set nokey\nset title '%s'\n", "toto");
    fprintf(dump, "#set terminal postscript eps color solid\n");
    fprintf(dump, "#set output '%s.ps'", output);
    fprintf(dump, "set xrange[%ld:%ld]\n", win->XMIN, win->XMAX);
    fprintf(dump, "set yrange[%ld:%ld]\n", win->YMIN, win->YMAX);
    fprintf(dump, "plot [:][:][:][:] '-' w l, '-' w l 2, '-' w l 3\n");
    fprintf(dump, "%ld %ld\n", win->XMIN, win->YMIN);
    fprintf(dump, "%ld %ld\n", win->XMAX, win->YMIN);
    fprintf(dump, "%ld %ld\n", win->XMAX, win->YMAX);
    fprintf(dump, "%ld %ld\n", win->XMIN, win->YMAX);
    fprintf(dump, "%ld %ld\n", win->XMIN, win->YMIN);
    fprintf(dump, "\n");

    for (l_pSeg = i_pSignal->SEGMENT; l_pSeg; l_pSeg = l_pSeg->NEXT) {
        if (getWSegDirection(i_pDataBase->PARAM, l_pSeg) == ocrHorizontal) {
            fprintf(dump, "%ld %ld\n", l_pSeg->P_MIN, l_pSeg->OFFSET);
            fprintf(dump, "%ld %ld\n", l_pSeg->P_MAX, l_pSeg->OFFSET);
        } else {
            fprintf(dump, "%ld %ld\n", l_pSeg->OFFSET, l_pSeg->P_MIN);
            fprintf(dump, "%ld %ld\n", l_pSeg->OFFSET, l_pSeg->P_MAX);
        }
    }
    fprintf(dump, "\n");

    for (l_pCon = i_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
        for (l_pVirCon = l_pCon->VIR_CON_LIST; l_pVirCon;
             l_pVirCon = l_pVirCon->NEXT) {
            fprintf(dump, "%ld %ld\n", l_pVirCon->X, l_pVirCon->Y);
            fprintf(dump, "%ld %ld\n", l_pVirCon->X, l_pVirCon->Y + 1);
            fprintf(dump, "\n");
        }
    }



    fclose(dump);
}


ocrNaturalInt
routingWindow(ocrRoutingDataBase * i_pDataBase, phfig_list * i_pPhFig)
{
    ocrConnector *l_pCon;
    ocrWindow *l_pWindow;
    ocrSignal *l_pSignal;
    ocrVirtualConnector *l_pVirCon;
    ocrNaturalInt i, l_uWin, wire_len;
    //ocrNaturalInt l_uNbf = sqrt (i_pDataBase->NB_F);

    init_Astar (i_pDataBase);

    wire_len = 0;

    i_pDataBase->NB_UNROUTED = 1;
    i_pDataBase->NB_IT = 0;
    i_pDataBase->NB_ROUTED = 0;

    // allocation de la grille
    display(LEVEL, DEBUG, "%s ", "o Grid allocation ...\n");
    l_uWin = i_pDataBase->SIGNAL[0]->WIN;
    l_pWindow = i_pDataBase->WINDOWS[l_uWin];

    i_pDataBase->GRID = createWGrid(l_pWindow->XMAX - l_pWindow->XMIN + 1,
                                    l_pWindow->YMAX - l_pWindow->YMIN + 1,
                                    g_OptLayer);

    while ((i_pDataBase->NB_UNROUTED != 0) && (i_pDataBase->NB_IT < ITMAX)) {
        ocrNaturalInt length;

        i_pDataBase->NB_IT++;
        i_pDataBase->NB_UNROUTED = 0;
        i_pDataBase->NB_ROUTED = 0;


        // initialisation de la grille
        initWGrid(i_pDataBase->GRID, i_pDataBase->PARAM);
        //dumpWGrid ("prosper", i_pDataBase, l_pWindow);



        // Initialisation des TAG & ROUTED
        for (i = 0; i < i_pDataBase->NB_SIGNAL; i++) {
            i_pDataBase->SIGNAL[i]->ROUTED = 0;

            //dumpSigs (i_pDataBase->SIGNAL[i]->NAME, i_pDataBase, i_pDataBase->SIGNAL[i], l_pWindow);

            for (l_pCon = i_pDataBase->SIGNAL[i]->CON_LIST;
                 l_pCon; l_pCon = l_pCon->NEXT) {
                // suppression de la liste des segments
                i_pDataBase->SIGNAL[i]->SEGMENT = NULL;

                if (!IS_MARK_AS_LINKED(l_pCon)) {
                    // on ne détruit pas la position du connecteur facial associé
                    //             l_pCon->CON = NULL;
                }

                deleteVirtualConnectorList(i_pDataBase->SIGNAL[i]->VIA);
                i_pDataBase->SIGNAL[i]->VIA = NULL;

                for (l_pVirCon = l_pCon->VIR_CON_LIST;
                     l_pVirCon;
                     l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
                    l_pVirCon->TAG = 0;
                    l_pVirCon->DIST = 0;
                }
            }
        }

        if (g_pOption->ORDER & OCR_RIPUP)
            display(LEVEL, VERBOSE, "%s\n", "o Rip Up & Re Route selected\n");

        if (g_pOption->ORDER & OCR_ODYN)
            display(LEVEL, VERBOSE, "%s\n", "o Dynamic scheduling\n");

        i = OCRNATURALINT_MAX;
        updateWGrid(l_pWindow, i_pDataBase, i_pPhFig);

        // signaux avec des connecteurs critiques
        noCritVC(i_pDataBase);
        countFreeVC(i_pDataBase);
        //dumpWGrid ("hoplaboum", i_pDataBase, l_pWindow);


        while ((i = getNewSignal(i_pDataBase, i)) != OCRNATURALINT_MAX)
            if (!g_pOption->PRIORITY ||
                (g_pOption->PRIORITY && i_pDataBase->SIGNAL[i]->PRIORITY))
            {
                l_pSignal = i_pDataBase->SIGNAL[i];
                if (i_pDataBase->SIGNAL[i]->NICHT_ZU_ROUTIEREN)
                    continue;
                display(LEVEL, VVERB, "o Routing %s:%ld",
                        l_pSignal->NAME, l_pSignal->INDEX);

                // signaux avec des connecteurs critiques
                markSegmentAsFree(i_pDataBase, i_pDataBase->SIGNAL[i], i);
                for (l_pCon = l_pSignal->CON_LIST; l_pCon;
                     l_pCon = l_pCon->NEXT) {
                    if (l_pCon->critVC) {
                        unProtectVC(i_pDataBase, l_pCon->critVC);
                    }
                }

                if (l_pSignal->ROUTED != 0)
                    exit(1);

                display(LEVEL, DEBUG,
                        "o %ld points, Index : %ld, Pri : %d\n",
                        l_pSignal->NB_CON, l_pSignal->INDEX,
                        l_pSignal->PRIORITY);

                // Routage N points

                length = FINDNPATH(i_pDataBase->PARAM, i_pDataBase->GRID,
                                   l_pSignal, l_pWindow);

                if (length != OCRNATURALINT_MAX) {
                    wire_len += length;
                    l_pSignal->ROUTED = 1;
                    i_pDataBase->NB_ROUTED++;
                    display(LEVEL, VVERB, "\t\t\tok\n");

                    //return 1; // XXX for debug...
                    
                } else {
                  
                    display(LEVEL, VVERB, "\t\t\tfailed\n");
                    
                    if (g_pOption->ORDER & OCR_RIPUP) {
                      
                        display(LEVEL, VVERB,
                                "o Rip up Sig %ld, %ld points, Index : %ld, Pri : %d :\n",
                                i, l_pSignal->NB_CON, l_pSignal->INDEX,
                                l_pSignal->PRIORITY);

                        dig_around ();
                        countFreeVC (i_pDataBase);

                        /*ripUp3(i_pDataBase, i_pDataBase->PARAM, l_pSignal);*/

                        reRoute(i_pDataBase, i_pDataBase->PARAM, l_pSignal);
//                    if (reRoute (i_pDataBase, i_pDataBase->PARAM, l_pSignal) == 0)
//                      {
//                      dumpWGrid ("houlala", i_pDataBase, l_pWindow);
//
//                      return;
//                      }

                    }
                    else {
                        //if (l_pSignal->INDEX == 1523) {
                        //dumpSigs ("22", i_pDataBase, l_pSignal, l_pWindow);
                        //dumpWGrid ("houlala", i_pDataBase, l_pWindow);
                        //}
                        l_pSignal->ROUTED = 2;

                        i_pDataBase->NB_UNROUTED++;
                        l_pSignal->PRIORITY = i_pDataBase->NB_IT;
                        //return;
                    }

                }
                unMarkSegmentAsFree(i_pDataBase, i_pDataBase->SIGNAL[i], i);

                // LOOP pour reperer les connecteurs isoles.
                countFreeVC(i_pDataBase);
            }
        reRoute(i_pDataBase, i_pDataBase->PARAM, NULL);

        display(LEVEL, VERBOSE, "o It = %ld Sig = %ld Failures : %ld\n",
                i_pDataBase->NB_IT,
                i_pDataBase->NB_ROUTED, i_pDataBase->NB_UNROUTED);

        // Ordonnancement les + PRIORITAIRES en premier
        orderingSignals(i_pDataBase->SIGNAL,
                        //i_pDataBase->NB_SIGNAL, ORDER_PRIORITY_RANDOM);
                        i_pDataBase->NB_SIGNAL, ORDER_PRIORITY_CON);

        {
            ocrNaturalInt PRIO_MAX = i_pDataBase->SIGNAL[0]->PRIORITY;
            if (PRIO_MAX > 8)
                for (i = 0; i < i_pDataBase->NB_SIGNAL - 1; i++)
                    if (i_pDataBase->SIGNAL[i]->PRIORITY == PRIO_MAX) {
                        if (i_pDataBase->SIGNAL[i]->INTEXT == INTERNAL) {
                            display(LEVEL, DEBUG,
                                    "o I Prio Max = %ld Sig = %ld Name = %s\n",
                                    PRIO_MAX, i,
                                    i_pDataBase->SIGNAL[i]->NAME);
                        } else
                            display(LEVEL, DEBUG,
                                    "o E Prio Max = %ld Sig = %ld Name = %s\n",
                                    PRIO_MAX, i,
                                    i_pDataBase->SIGNAL[i]->NAME);
                    }
        }

    }
    freeWGrid(i_pDataBase->GRID);
    i_pDataBase->GRID = NULL;

    display(LEVEL, INFO, "o Iterations = %ld\n", i_pDataBase->NB_IT);
    display(LEVEL, INFO, "o Signals = %ld\n", i_pDataBase->NB_ROUTED);
    display(LEVEL, VERBOSE, "o Total wirelength = %ld lambdas\n", wire_len);
    display(LEVEL, INFO, "o Failures = %ld\n", i_pDataBase->NB_UNROUTED);

    {
        ocrNaturalInt chk;
        display (LEVEL, INFO, "o Unrouted signals :\n");
        chk = 0;
        for (i = 0; i < i_pDataBase->NB_SIGNAL - 1; i++)
            if (i_pDataBase->SIGNAL[i]->ROUTED != 1) {
                chk = 1;
                if (i_pDataBase->SIGNAL[i]->INTEXT == INTERNAL) {
                    display(LEVEL, INFO,
                            " %s\n",
                            i_pDataBase->SIGNAL[i]->NAME);
                } else
                    display(LEVEL, INFO,
                            " %s\n",
                            i_pDataBase->SIGNAL[i]->NAME);
            }
        if (!chk)
            display (LEVEL, INFO, " none\n");
    }
    return wire_len;
}

void
dumpConToPhFig(ocrSignal * i_pSignal,
               ocrRoutingParameters * i_pParam, phfig_list * i_pPhFig)
{
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVia;
    ocrSignal *l_pSignal;

    display(LEVEL, VERBOSE, "o Dumping Connectors to PhFig ...\n");

    for (l_pSignal = i_pSignal; l_pSignal; l_pSignal = l_pSignal->NEXT) {
        // CON
        for (l_pCon = l_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
            if (l_pCon->INTEXT == EXTERNAL)
                if (!IS_MARK_AS_FACE_CON(l_pCon))
                    if (l_pCon->CON)
                        //addphcon(i_pPhFig,
                        //       l_pCon->CON->ORIENT,
                        //       l_pCon->NAME,
                        //       l_pCon->CON->X * 5 * SCALE_X,
                        //       l_pCon->CON->Y * 5 * SCALE_X, ALU2, 2 * SCALE_X);
                        ;
        }

        // VIA & CON    
        for (l_pVia = l_pSignal->VIA; l_pVia; l_pVia = (ocrVirtualConnector *) l_pVia->NEXT) {  // HCl XXX
            addViaToPhFig(i_pParam, l_pVia->X, l_pVia->Y, l_pVia->Z - 1,
                          i_pPhFig);
        }
    }
}

void linkWindow(ocrRoutingDataBase * i_pDataBase, ocrNaturalInt i_uWindow)
{
    ocrSignal *l_pSignal;

    display(LEVEL, INFO, "o Linking Window %ld\n", i_uWindow);
    for (l_pSignal = i_pDataBase->FSIGNAL[i_uWindow];
         l_pSignal; l_pSignal = l_pSignal->NEXT)
        linkSignal(i_pDataBase, l_pSignal);
}

/**
 * affiche l'aide
**/
void dumpOption()
{
    printf("Usage : ocr [options] -L logical_view -P placed_view -O output_routed_view\n");
    printf("Options :\n");
    printf("  -v  :  verbose mode\n");
    printf(" -vv  : very verbose mode\n");
    printf("  -d  :  can't stop chatting mode\n");
    printf(" -i n :  max number of iterations\n");
    printf(" -l n :  use n layers of metal for routing purpose\n");
    //printf(" -w n :  n windows");
    printf("\n");

}

int main(int argc, char **argv)
{
    phfig_list *l_pPhFig;
    lofig_list *l_pLoFig;
    ocrRoutingDataBase *l_pDataBase;
    int Index;
    int chk_files = 0;
    char *l_sPhFigName;
    char *l_sLoFigName;
    char *out_name;
    ocrNaturalInt start_with_vss = 0;
    ocrNaturalInt sym_y = 0;


    // initialisation de MBK
    mbkenv();
    
    alliancebanner ("OCR", VERSION, "An Over-the-cell router for standard cells", "2001", ALLIANCE_VERSION);

    if (argc < 2) {
        dumpOption();
        exit(1);
    }
    l_sPhFigName = argv[1];
    l_sLoFigName = argv[2];

    // Option de la ligne de commande
    //g_pOption = getOption (argc - 2, argv + 2);

    g_pOption = (ocrOption *) mbkalloc(sizeof(ocrOption));
    LEVEL = INFO;
    g_pOption->ORDER = OCR_NORM;
    g_pOption->WINDOW = 1;
    g_pOption->PRIORITY = 0;
    g_pOption->ALGO = 0;



    for (Index = 1; Index < argc; Index++) {
        if (!strcmp(argv[Index], "--algo")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                g_pOption->ALGO = 0;
                if (!strcmp(argv[Index], "astar")) {
                    g_pOption->ALGO = 1;
                } else if (!strcmp(argv[Index], "min")) {
                    g_pOption->ALGO = 0;
                } else {
                    display(LEVEL, ERROR, "option --algo must be follwed by min or astar\n");
                    exit(1);
                }
                display (LEVEL, INFO, "o Algorithm : %s\n", argv[Index]);
            }
        }
        if (!strcmp(argv[Index], "-l")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                g_OptLayer = atoi(argv[Index]);
                if (g_OptLayer < 2) {
                    display(LEVEL, ERROR, "option -l n : n >= 2\n");
                    exit(1);
                }
                display (LEVEL, INFO, "o Metal layers : %d\n", g_OptLayer);
            }
        }
        if (!strcmp(argv[Index], "-w")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                g_pOption->WINDOW = atoi(argv[Index]);
            }
            display (LEVEL, INFO, "o Windows : %d\n", g_pOption->WINDOW);
        }
        if (!strcmp(argv[Index], "-i")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                ITMAX = atoi(argv[Index]);
                display (LEVEL, INFO, "o Iterations: %d\n", ITMAX);
            } else {
                display (LEVEL, ERROR, "the -i parameter requires you to specify the number of iterations\n");
                return -1;
            }
        } else if (!strcmp(argv[Index], "-L")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                l_sLoFigName = argv[Index];
                chk_files++;
                display (LEVEL, INFO, "o Logical view : %s\n", argv[Index]);
            } else {
                display (LEVEL, ERROR, "Intput logical-netlist name not provided !\n");
                return -1;
            }
        } else if (!strcmp(argv[Index], "-P")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                l_sPhFigName = argv[Index];
                chk_files++;
                display (LEVEL, INFO, "o Physical placed view : %s\n", argv[Index]);
            } else {
                display (LEVEL, ERROR, "Intput placed-netlist name not provided !\n");
                return -1;
            }
        } else if (!strcmp(argv[Index], "-O")) {
            if ((Index + 1) < argc) {
                Index = Index + 1;
                out_name = argv[Index];
                chk_files++;
                display (LEVEL, INFO, "o Output physical routed view : %s\n", argv[Index]);
            } else {
                display (LEVEL, ERROR, "Output netlist name not provided !\n");
                return -1;
            }
        } else if (!strcmp(argv[Index], "-d")) {
            LEVEL = DEBUG;
        } else if (!strcmp(argv[Index], "-vv")) {
            LEVEL = VVERB;
        } else if (!strcmp(argv[Index], "-v")) {
            LEVEL = VERBOSE;
        } else if (!strcmp(argv[Index], "-o")) {
            g_pOption->ORDER |= OCR_ODYN;
        }
        if (!strcmp(argv[Index], "-r")) {
            g_pOption->ORDER |= OCR_RIPUP;
        }
        if (!strcmp(argv[Index], "-z")) {
            g_pOption->PRIORITY = 1;
        }
    }

    if (chk_files != 3) {
        display (LEVEL, ERROR, "Mandatory parameter missing (-L, -P, -O)\n");
        return -1;
    }

    switch (LEVEL) {
        case DEBUG:
            display (LEVEL, DEBUG, "o debug mode\n");
            break;
        case VERBOSE:
            display (LEVEL, VERBOSE, "o verbose mode\n");
            break;
        case VVERB:
            display (LEVEL, VVERB, "o very verbose mode\n");
            break;
    }


    // Lecture de la vue physique
    l_pPhFig = readPhFig(l_sPhFigName);

    // Lecture de la vue logique
    l_pLoFig = getlofig(l_sLoFigName, 'A');

//FIXME::pas de reentrance
    rflattenlofig(l_pLoFig, YES, YES);

    display(LEVEL, VERBOSE, "%s\n", "o Read .vst ...");

    l_pDataBase = initDataBase(l_pPhFig, l_pLoFig);

    // un peu de ménage.
//  delphfig (l_pPhFig->NAME);
    dellofig(l_pLoFig->NAME);

//  // paramtères physiques de la grille 5*5 viacost=2 Layer paires = HORZ
//  l_pDataBase->PARAM = setParameters (5, 5, 2, ocrHorizontal);
    {
        ocrNaturalInt i;

        createWindow(l_pDataBase);
//    dumpDataBase (l_pDataBase, stdout);
        translatingRealToWindows(l_pDataBase);
        globalRouting(l_pDataBase);

        // Routage de chaque Window
        for (i = 1; i <= l_pDataBase->NB_F; i++) {
            createWindowArray(l_pDataBase, i);
            display(LEVEL, INFO, "o Routing Win %ld : %ld signaux ...\n",
                    i, l_pDataBase->NB_SIGNAL);
            orderingSignals(l_pDataBase->SIGNAL,
                            //l_pDataBase->NB_SIGNAL, ORDER_PRIORITY_RANDOM);
                            l_pDataBase->NB_SIGNAL, ORDER_PRIORITY_CON);

            // dumpDataBase (l_pDataBase,stdout);
            routingWindow(l_pDataBase, l_pPhFig);
            //linkWindow(l_pDataBase, i);

            deleteWindowArray(l_pDataBase, i);
        }

        // dumpToDisk
        translatingWindowsToReal(l_pDataBase);
        display(LEVEL, VERBOSE, "o Dumping Signals to PhFig ...\n", i);
        for (i = 1; i <= l_pDataBase->NB_F; i++) {
            dumpSignalsToPhFig(l_pDataBase->FSIGNAL[i],
                               l_pDataBase->PARAM, l_pPhFig);
            dumpConToPhFig(l_pDataBase->FSIGNAL[i],
                           l_pDataBase->PARAM, l_pPhFig);
        }

        dumpAlimToPhFig(l_pDataBase, l_pPhFig);

        dumpPhFigToDisk(out_name, l_pPhFig);
    }

    deleteDataBase(l_pDataBase);
    return 0;
}
