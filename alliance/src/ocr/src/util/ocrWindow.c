/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/06/27 09:09:09 $
   $Log: ocrWindow.c,v $
   Revision 1.3  2002/06/27 09:09:09  hcl
   Code d'erreur si tous les signaux ne sont pas routés.

   Revision 1.2  2002/03/20 13:25:57  hcl
   SymX bug.

   Revision 1.1  2002/03/15 14:37:27  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:25  hcl
   First commit in the new tree.

   Revision 1.5  2001/12/20 13:04:04  hcl
   Cosmetique.

   Revision 1.4  2001/12/19 14:22:49  hcl
   Moult petits changements.

   Revision 1.3  2001/12/14 15:58:40  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.2  2001/11/20 09:42:06  hcl
   last but not least

   Revision 1.1  2001/09/26 14:27:56  hcl
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
#include "ocrUtil.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "ocrPath.h"
#include "display.h"
#include "ocrConnectorUtil.h"

static char *res_id =
    "$Id: ocrWindow.c,v 1.3 2002/06/27 09:09:09 hcl Exp $";

extern ocrOption *g_pOption;

#define LEVEL (g_pOption->LEVEL)

#define MAX_HT 500
#define PITCH   5

/**
 * création d'une nouvelle fenetre
**/
ocrWindow *newWindow(ocrNaturalInt i_nNumWin)
{
    ocrWindow *l_pWindow;
    ocrNaturalInt i;

    l_pWindow = (ocrWindow *) mbkalloc(sizeof(ocrWindow));

    l_pWindow->XMIN = 0;
    l_pWindow->XMAX = 0;
    l_pWindow->YMIN = 0;
    l_pWindow->YMAX = 0;
    l_pWindow->NUM = i_nNumWin;
    l_pWindow->NB_SIG = 0;
    l_pWindow->NB_SIG_THROW = 0;

    for (i = 0; i < 9; i++)
        l_pWindow->NB_SIG_FACE[i] = 0;

    return l_pWindow;
}

/**
 * supprime une fenetre
**/
void deleteWindow(ocrWindow * i_pWindow)
{
    if (i_pWindow)
        mbkfree(i_pWindow);
}

/**
 * renvoie 1 si le connecteru virtuel se trouve dans la fentre
**/
ocrNaturalInt
isVCInWindow(ocrVirtualConnector * i_pVirCon, ocrWindow * i_pWindow)
{
    if ((i_pVirCon->X >= i_pWindow->XMIN) &&
        (i_pVirCon->X <= i_pWindow->XMAX) &&
        (i_pVirCon->Y >= i_pWindow->YMIN)
        && (i_pVirCon->Y <= i_pWindow->YMAX))
        return 1;
    display (LEVEL, INFO, "oulala : x:%ld?>%ld, y:%ld?>%ld, z:%ld\n", i_pVirCon->X, i_pWindow->XMAX, i_pVirCon->Y, i_pWindow->YMAX, i_pVirCon->Z);
    abort();
    return 0;
}

/**
 * renvoie la fenetre du connecteur virtuel
**/
void
computeVCWindow(ocrRoutingDataBase * i_pDataBase,
                ocrVirtualConnector * i_pVirCon)
{
    ocrNaturalInt i;

    for (i = 1; i <= i_pDataBase->NB_F; i++)
        if (isVCInWindow(i_pVirCon, i_pDataBase->WINDOWS[i])) {
            i_pVirCon->WIN = i;
            return;
        }
}

/**
 * Attribue une fenete à n connecteur interne
 * d'au plus deux fenetres differentes
**/
void computeCWindow(ocrConnector * i_pCon)
{
    ocrNaturalShort l_uWin, l_uWin2 = 0;
    ocrNaturalShort l_uOk = 1;
    ocrVirtualConnector *l_pVirCon;

    // fenetre du premier con vir
    if (!i_pCon->VIR_CON_LIST) {
        display (LEVEL, ERROR,
                 "o No virtual connector for connector:  %s of instance : %s\n",
                 i_pCon->NAME, i_pCon->INSNAME);
        exit(1);
    }
    l_uWin = i_pCon->VIR_CON_LIST->WIN;
    for (l_pVirCon = i_pCon->VIR_CON_LIST;
         l_pVirCon; l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
        if (l_pVirCon->WIN != l_uWin) {
            l_uWin2 = l_pVirCon->WIN;
            i_pCon->WIN = 0;
            l_uOk = 0;
            display(LEVEL, INFO, "Con cheval NAME %s INSNAME %s\n",
                    i_pCon->NAME, i_pCon->INSNAME);
        }
    }

    if (l_uOk) {
        // tous les connecteurs sont sur la meme verticale
        // et dans la meme fenetre
        i_pCon->WIN = l_uWin;
    } else {
        ocrNaturalShort l_uNb1 = 0;
        ocrNaturalShort l_uNb2 = 0;
        ocrNaturalShort l_uWinDel;
        // recherche des connecteurs majoritaires
        // les connecteurs sont dans les fenetres l_uWin et l_uWin2
        // il n'y a pas d'autres fenetres car nous sommes sur des frontières
        // de rangées

        for (l_pVirCon = i_pCon->VIR_CON_LIST;
             l_pVirCon;
             l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
            if (l_pVirCon->WIN == l_uWin)
                l_uNb1++;
            else
                l_uNb2++;
        }

        // les connecteurs virtuels de la fenetre l_uWinDel sont
        // a détruire.
        if (l_uNb1 > l_uNb2) {
            i_pCon->WIN = l_uWin;
            l_uWinDel = l_uWin2;
        } else {
            i_pCon->WIN = l_uWin2;
            l_uWinDel = l_uWin;
        }

        for (l_pVirCon = i_pCon->VIR_CON_LIST;
             l_pVirCon;
             l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
            if (l_pVirCon->WIN == l_uWinDel)
                deleteConVirInConVirList(&i_pCon->VIR_CON_LIST, l_pVirCon);
        }
    }
}

/**
 * création des listes de signaux 
**/
void initSignalList(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt j;

    i_pDataBase->FSIGNAL = (ocrSignal **)
        mbkalloc((i_pDataBase->NB_F + 1) * sizeof(ocrSignal *));

    for (j = 0; j <= i_pDataBase->NB_F; j++) {
        i_pDataBase->FSIGNAL[j] = NULL;
    }
}

/**
 * insert un signal dans une fenetre
**/
void
insertSignalWindow(ocrRoutingDataBase * i_pDataBase, ocrSignal * i_pSignal)
{
    // chainage du signal
    i_pSignal->NEXT = i_pDataBase->FSIGNAL[i_pSignal->WIN];
    i_pDataBase->FSIGNAL[i_pSignal->WIN] = i_pSignal;
    // mise à jour du nb de signaux de la fenetre.
    i_pDataBase->WINDOWS[i_pSignal->WIN]->NB_SIG++;
}


/**
 * XYtoWin : donne le numéro de la Window à la position (x,y)
**/
void
XYtoWin(ocrRoutingDataBase * i_pDataBase,
        ocrNaturalInt i_uX, ocrNaturalInt i_uY, ocrNaturalInt * o_pWin)
{
    ocrNaturalInt l_uNbf = sqrt(i_pDataBase->NB_F);

    *o_pWin = i_uY * l_uNbf + i_uX + 1;
}

/**
 * WintoXY : donne la position (x,y) de la Window numéro Win
**/
void
WintoXY(ocrRoutingDataBase * i_pDataBase,
        ocrNaturalInt * o_pX, ocrNaturalInt * o_pY, ocrNaturalInt i_uWin)
{
    ocrNaturalInt l_uNbf = sqrt(i_pDataBase->NB_F);

    *o_pX = (i_uWin - 1) % l_uNbf;
    *o_pY = (i_uWin - 1) / l_uNbf;
}

/**
 * Renvoie le connecteur FACIAL associé au connecteur i_pCon
 * face OPPOSÉE
 * index IDENTIQUE
 * window ADJACENTE dans la direction de la face
**/
ocrConnector *getAssociatedConnector(ocrRoutingDataBase *
                                     i_pDataBase,
                                     ocrNaturalInt i_uIndex,
                                     ocrConnector * i_pCon)
{
    ocrNaturalInt l_uFace = 0;
    ocrNaturalInt l_uWin;
    ocrNaturalInt l_uX, l_uY;
    ocrConnector *l_pCon;
    ocrConnector *l_pConRes = NULL;
    ocrSignal *l_pSignal;

    WintoXY(i_pDataBase, &l_uX, &l_uY, i_pCon->WIN);
//  printf ("Win %d -> x=%ld y=%ld\n", i_pCon->WIN, l_uX, l_uY);

    switch (i_pCon->FACE) {
    case OCR_FACE_EAST:
        l_uFace = OCR_FACE_WEST;
        l_uX++;
        break;
    case OCR_FACE_WEST:
        l_uFace = OCR_FACE_EAST;
        l_uX--;
        break;
    case OCR_FACE_NORTH:
        l_uFace = OCR_FACE_SOUTH;
        l_uY++;
        break;
    case OCR_FACE_SOUTH:
        l_uFace = OCR_FACE_NORTH;
        l_uY--;
    }

    XYtoWin(i_pDataBase, l_uX, l_uY, &l_uWin);

    if ((l_uWin <= 0) || (l_uWin > i_pDataBase->NB_F)) {
        display(LEVEL, ERROR, "getAssociatedConnector limit\n");
        exit(1);
    }

    for (l_pSignal = i_pDataBase->FSIGNAL[l_uWin];
         l_pSignal; l_pSignal = l_pSignal->NEXT) {
        if (l_pSignal->INDEX == i_uIndex) {
            for (l_pCon = l_pSignal->CON_LIST; l_pCon;
                 l_pCon = l_pCon->NEXT) {
                if (IS_MARK_AS_FACE_CON(l_pCon)) {
                    if (l_uFace == l_pCon->FACE)        // bonne face ?

                    {
                        l_pConRes = l_pCon;
                    }
                }
            }
        }
    }

    if (!l_pConRes) {
        display(LEVEL, ERROR, "l_pConRes NULL\n");
        exit(1);
    }

    return l_pConRes;
}


/**
 * lie le connecteur Facial i_pCon avec son connecteur associé
 * de la window adjacente
**/
void
linkConnector(ocrRoutingDataBase * i_pDataBase,
              ocrNaturalInt i_uIndex, ocrConnector * i_pCon)
{
    ocrNaturalInt l_uNbf = sqrt(i_pDataBase->NB_F);
    ocrConnector *l_pAssCon;

//  if (i_pCon->FACE == OCR_TYPE_FACE)
    if (IS_MARK_AS_FACE_CON(i_pCon)) {
        l_pAssCon = getAssociatedConnector(i_pDataBase, i_uIndex, i_pCon);

        // Est ce que le connecteur est déja lié = Fenetre déja routée ?
        if (!IS_MARK_AS_LINKED(l_pAssCon))
//    if (!l_pAssCon->CON)
        {
            // non, alors il faut le faire
            MARK_AS_LINKED(l_pAssCon);
            MARK_AS_LINKED(i_pCon);
            // Modification du connecteur pour prendre les bonnes contraintes
            l_pAssCon->VIR_CON_LIST = dupVirtualConnector(i_pCon->CON);
            l_pAssCon->CON = l_pAssCon->VIR_CON_LIST;

            switch (l_pAssCon->FACE) {
            case OCR_FACE_EAST:
                l_pAssCon->CON->X =
                    (i_pDataBase->XAB2 - i_pDataBase->XAB1) / 500 / l_uNbf;
                break;
            case OCR_FACE_WEST:
                l_pAssCon->CON->X = 0;
                break;
            case OCR_FACE_NORTH:
                l_pAssCon->CON->Y =
                    (i_pDataBase->YAB2 - i_pDataBase->YAB1) / 500 / l_uNbf;
                break;
            case OCR_FACE_SOUTH:
                l_pAssCon->CON->Y = 0;
            }
        }
    }
}


/**
 * supprime le "lien" entre les deux connecteurs.
 * si le signal n'est pas ROUTED alors on supprime la contrainte des 2 con
 * si il est ROUTED alors on supprime la contrainte sur i_pCon seulement
**/
void
unLinkConnector(ocrRoutingDataBase * i_pDataBase, ocrConnector * i_pCon)
{
}

/**
 * retourne 1 si le connecteur est dans une window 
 * en périphérie
**/
ocrNaturalInt
isABorderWindow(ocrRoutingDataBase * i_pDataBase, ocrNaturalInt i_uWin)
{
    ocrNaturalInt l_uNbf = sqrt(i_pDataBase->NB_F);
    ocrNaturalInt l_uX, l_uY;

    WintoXY(i_pDataBase, &l_uX, &l_uY, i_uWin);
    return ((l_uX == 0) || (l_uY == 0) ||
            (l_uX == l_uNbf - 1) || (l_uY == l_uNbf - 1));
}

/*
 * getFace
 * donne les faces externes possibles de la Window
 **/

void getFace(ocrRoutingDataBase * i_pDataBase, ocrConnector * i_pCon)
{
    ocrNaturalInt l_uNbf = sqrt(i_pDataBase->NB_F);
    ocrNaturalInt l_uX, l_uY;

    if (i_pCon->WIN != 0) {
        l_uX = (i_pCon->WIN - 1) % l_uNbf;
        l_uY = (i_pCon->WIN - 1) / l_uNbf;

        i_pCon->FACE = 0;

        if (l_uX == 0)
            i_pCon->FACE |= OCR_FACE_WEST;
        if (l_uX == l_uNbf - 1)
            i_pCon->FACE |= OCR_FACE_EAST;
        if (l_uY == 0)
            i_pCon->FACE |= OCR_FACE_SOUTH;
        if (l_uY == l_uNbf - 1)
            i_pCon->FACE |= OCR_FACE_NORTH;
    }
}

/**
 * XY fonctions
 * transformation des coordonnées relatives des points dans les fenetres
 * aux coordonnées réélles (et inv.)
**/

/**
 * transforme les coordonnées relatives à la fenetre 
 * en coordonnées absolue
**/
void
XYYWindowToReal(ocrNaturalInt i_uWin,
                ocrRoutingDataBase * i_pDataBase,
                ocrNaturalInt * o_uX,
                ocrNaturalInt * o_uY1, ocrNaturalInt * o_uY2)
{
    *o_uX += i_pDataBase->WINDOWS[i_uWin]->XMIN;
    *o_uY1 += i_pDataBase->WINDOWS[i_uWin]->YMIN;
    *o_uY2 += i_pDataBase->WINDOWS[i_uWin]->YMIN;
}

/**
 * transforme les coordonnées relatives à la fenetre 
 * en coordonnées absolue
**/
void
XYWindowToReal(ocrNaturalInt i_uWin,
               ocrRoutingDataBase * i_pDataBase,
               ocrNaturalInt * o_uX, ocrNaturalInt * o_uY)
{
    *o_uX += i_pDataBase->WINDOWS[i_uWin]->XMIN;
    *o_uY += i_pDataBase->WINDOWS[i_uWin]->YMIN;
}

/**
 * transforme les coordonnées relatives à la fenetre 
 * en coordonnées absolue
**/
void
XXYWindowToReal(ocrNaturalInt i_uWin,
                ocrRoutingDataBase * i_pDataBase,
                ocrNaturalInt * o_uX1,
                ocrNaturalInt * o_uX2, ocrNaturalInt * o_uY)
{
    *o_uX1 += i_pDataBase->WINDOWS[i_uWin]->XMIN;
    *o_uX2 += i_pDataBase->WINDOWS[i_uWin]->XMIN;
    *o_uY += i_pDataBase->WINDOWS[i_uWin]->YMIN;
}

/**
 * transforme les coordonnées absolue en coordonnées
 * relative à la fenetre
**/
void
XYRealToWindow(ocrNaturalInt i_uWin,
               ocrRoutingDataBase * i_pDataBase,
               ocrNaturalInt * o_uX, ocrNaturalInt * o_uY)
{
    *o_uX -= i_pDataBase->WINDOWS[i_uWin]->XMIN;
    *o_uY -= i_pDataBase->WINDOWS[i_uWin]->YMIN;
}

/**
 * transforme les coordonnées relatives à la fenetre 
 * en coordonnées absolue
**/
void translatingWindowsToReal(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt i;
    ocrSignal *l_pSignal;
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirCon;
    ocrWSegment *l_pSegment;

    display(LEVEL, VERBOSE,
            "o Translating Windows to Real : CON, VIA, SEGMENT ...\n");
    for (i = 1; i <= i_pDataBase->NB_F; i++) {
        for (l_pSignal = i_pDataBase->FSIGNAL[i];
             l_pSignal; l_pSignal = l_pSignal->NEXT) {
            // VIA
            for (l_pVirCon = l_pSignal->VIA;
                 l_pVirCon;
                 l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
                XYWindowToReal(l_pSignal->WIN, i_pDataBase, &l_pVirCon->X,
                               &l_pVirCon->Y);
            }

            // SEGMENT
            for (l_pSegment = l_pSignal->SEGMENT;
                 l_pSegment; l_pSegment = l_pSegment->NEXT) {
                if (getWSegDirection(i_pDataBase->PARAM, l_pSegment)
                    == ocrHorizontal) {
                    XXYWindowToReal(l_pSignal->WIN,
                                    i_pDataBase,
                                    &l_pSegment->P_MIN,
                                    &l_pSegment->P_MAX,
                                    &l_pSegment->OFFSET);
                } else {
                    XYYWindowToReal(l_pSignal->WIN,
                                    i_pDataBase,
                                    &l_pSegment->OFFSET,
                                    &l_pSegment->P_MIN,
                                    &l_pSegment->P_MAX);
                }
            }

            // CON 
            for (l_pCon = l_pSignal->CON_LIST; l_pCon;
                 l_pCon = l_pCon->NEXT) {
                for (l_pVirCon = l_pCon->VIR_CON_LIST; l_pVirCon;
                     l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
                    XYWindowToReal(l_pSignal->WIN, i_pDataBase,
                                   &l_pVirCon->X, &l_pVirCon->Y);
                }
            }
        }
    }
}

/**
 * transforme les connecteurs virtuels en position absolue
 **/
void translatingRealToWindows(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt i;
    ocrConnector *l_pCon;
    ocrVirtualConnector *l_pVirCon;

    for (i = 0; i <= i_pDataBase->NB_GSIGNAL - 1; i++) {
        for (l_pCon = i_pDataBase->GSIGNAL[i]->CON_LIST;
             l_pCon; l_pCon = l_pCon->NEXT) {
            for (l_pVirCon = l_pCon->VIR_CON_LIST;
                 l_pVirCon;
                 l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
                XYRealToWindow(l_pCon->WIN, i_pDataBase, &l_pVirCon->X,
                               &l_pVirCon->Y);
            }
        }
    }
}

/**
 * initialisation des fenetres
**/
void initWindow(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt i, l_uFace, l_uRange;
    ocrNaturalInt x, y;



    i_pDataBase->WINDOWS =
        (ocrWindow **) mbkalloc((i_pDataBase->NB_F + 1) *
                                sizeof(ocrWindow *));


    // création des fenetres
    for (i = 0; i <= i_pDataBase->NB_F; i++)
        i_pDataBase->WINDOWS[i] = newWindow(i);

    // affectation des coordonnées des fenetres pour respecter les rangées
    // l_uRange = nb de rangées
    // l_uFace = nb de fenetres par faces
    l_uFace = sqrt(i_pDataBase->NB_F);
    l_uRange = (i_pDataBase->YAB2 - i_pDataBase->YAB1) / (10 * SCALE_X * PITCH);
    display(LEVEL, VERBOSE, "o Layout has %ld rows\n", l_uRange);

    for (y = 1; y <= l_uFace; y++) {
        for (x = 1; x <= l_uFace; x++) {
            i = (y - 1) * l_uFace + x;
            i_pDataBase->WINDOWS[i]->XMIN =
                (x - 1) * (i_pDataBase->XAB2 -
                           i_pDataBase->XAB1) / 50.0 / (l_uFace);
            i_pDataBase->WINDOWS[i]->XMAX =
                x * (i_pDataBase->XAB2 -
                     i_pDataBase->XAB1) / 50.0 / (l_uFace) - 0;

            i_pDataBase->WINDOWS[i]->YMIN =
                10 * (int) ((y - 1) *
                            ((double) l_uRange / (double) l_uFace));
            i_pDataBase->WINDOWS[i]->YMAX =
                10 * (int) (y * ((double) l_uRange / (double) l_uFace)) -
                0;
        }
    }

    i_pDataBase->WINDOWS[0] = newWindow(0);
    i_pDataBase->WINDOWS[1] = newWindow(1);
    i_pDataBase->WINDOWS[0]->XMIN = 0;
    i_pDataBase->WINDOWS[0]->YMIN = 0;
    i_pDataBase->WINDOWS[0]->XMAX = (i_pDataBase->XAB2 - i_pDataBase->XAB1) / (PITCH * SCALE_X);
    i_pDataBase->WINDOWS[0]->YMAX = (i_pDataBase->YAB2 - i_pDataBase->YAB1) / (PITCH * SCALE_X);
    i_pDataBase->WINDOWS[1]->XMIN = 0;
    i_pDataBase->WINDOWS[1]->YMIN = 0;
    i_pDataBase->WINDOWS[1]->XMAX = (i_pDataBase->XAB2 - i_pDataBase->XAB1) / (PITCH * SCALE_X);
    i_pDataBase->WINDOWS[1]->YMAX = (i_pDataBase->YAB2 - i_pDataBase->YAB1) / (PITCH * SCALE_X);
}

/**
 * Attribue à chaque connecteur externe sa Window
**/
void computeExternalConnector(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt l_uWin;
    ocrConnector *l_pCon, *l_pConExt, *l_pBetterCon;
    ocrNaturalInt i, j, k;

    // Recherche de la meilleure Window.
    // Window en bordure qui contient un connecteur INTERNE
    // ou une Window en bordure la plus proche possible d'une autre Window

    for (k = 0; k <= i_pDataBase->NB_GSIGNAL - 1; k++) {
        //printf (" o treating signal: %s\n", i_pDataBase->GSIGNAL[k]->NAME);
        l_pConExt = NULL;
        l_pBetterCon = NULL;
        // Si le signal est EXTERNE
        if (i_pDataBase->GSIGNAL[k]->INTEXT == EXTERNAL) {
            for (l_pCon = i_pDataBase->GSIGNAL[k]->CON_LIST;
                 l_pCon; l_pCon = l_pCon->NEXT)
                // Si le connecteur est interne
                if (l_pCon->INTEXT == INTERNAL) {
                    // Si la Window est en bordure
                    if (isABorderWindow(i_pDataBase, l_pCon->WIN))
                        l_pBetterCon = l_pCon;
                } else
                    l_pConExt = l_pCon;

            if (l_pBetterCon) {
                l_pConExt->WIN = l_pBetterCon->WIN;
                getFace(i_pDataBase, l_pConExt);
            } else {
                ocrNaturalInt l_uNbf = sqrt(i_pDataBase->NB_F);
                ocrNaturalInt l_uDistMin = OCRNATURALINT_MAX;
                ocrNaturalInt l_uDist;

                //printf("signal %ld\n", i_pDataBase->GSIGNAL[k]->INDEX);
                // parcours des Window en bordure pour déterminer
                // la meilleure
                for (i = 0; i <= l_uNbf; i++)
                    for (j = 0; j <= l_uNbf; j += l_uNbf) {
                        for (l_pCon = i_pDataBase->GSIGNAL[k]->CON_LIST;
                             l_pCon; l_pCon = l_pCon->NEXT)
                            // Si le connecteur est interne
                            if (l_pCon->INTEXT == INTERNAL) {
                                ocrNaturalInt l_uX, l_uY;
                                WintoXY(i_pDataBase, &l_uX, &l_uY,
                                        l_pCon->WIN);
                                l_uDist =
                                    ABSDIFF(i, l_uX) + ABSDIFF(j, l_uY);

                                if (l_uDist < l_uDistMin) {
                                    XYtoWin(i_pDataBase, i, j, &l_uWin);
                                    l_uDistMin = l_uDist;
                                }
                            }
                    }

                for (i = 0; i <= l_uNbf; i += l_uNbf)
                    for (j = 0; j <= l_uNbf; j++) {
                        for (l_pCon = i_pDataBase->GSIGNAL[k]->CON_LIST;
                             l_pCon; l_pCon = l_pCon->NEXT)
                            // Si le connecteur est interne
                            if (l_pCon->INTEXT == INTERNAL) {
                                ocrNaturalInt l_uX, l_uY;
                                WintoXY(i_pDataBase, &l_uX, &l_uY,
                                        l_pCon->WIN);
                                l_uDist =
                                    ABSDIFF(i, l_uX) + ABSDIFF(j, l_uY);

                                if (l_uDist < l_uDistMin) {
                                    XYtoWin(i_pDataBase, i, j, &l_uWin);
                                    l_uDistMin = l_uDist;
                                }
                            }
                    }


                l_pConExt->WIN = l_uWin;
                getFace(i_pDataBase, l_pConExt);
            }
        }
    }
}

/**
 * Fenetrage
**/
void createWindow(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt i;
    ocrNaturalInt l_uWindow;
    ocrVirtualConnector *l_pVirCon;
    ocrConnector *l_pCon;

    initWindow(i_pDataBase);

    // Calcul pour chaque connecteur INT sa fenetre
    for (i = 0; i <= i_pDataBase->NB_GSIGNAL - 1; i++) {
        if ((i_pDataBase->GSIGNAL[i]))
            for (l_pCon = i_pDataBase->GSIGNAL[i]->CON_LIST;
                 l_pCon; l_pCon = l_pCon->NEXT) {
                //printf (" o treating signal %s\n", i_pDataBase->GSIGNAL[i]->NAME);
                for (l_pVirCon = l_pCon->VIR_CON_LIST;
                     l_pVirCon;
                     l_pVirCon = (ocrVirtualConnector *) l_pVirCon->NEXT) {
                    computeVCWindow(i_pDataBase, l_pVirCon);
                }
                if ((l_pCon->INTEXT == INTERNAL)
                    && (!isvdd(l_pCon->NAME))
                    && (!isvss(l_pCon->NAME)))
                    computeCWindow(l_pCon);
            }
    }

    // Calcul pour chaque connecteur EXT sa fenetre
    computeExternalConnector(i_pDataBase);

    // Calcul pour chaque signal sa fenetre
    for (i = 0; i <= i_pDataBase->NB_GSIGNAL - 1; i++) {
        l_uWindow = OCRNATURALINT_MAX;
        for (l_pCon = i_pDataBase->GSIGNAL[i]->CON_LIST;
             l_pCon; l_pCon = l_pCon->NEXT) {
//          if (l_pCon->INTEXT == INTERNAL)
            {
                if (l_uWindow == OCRNATURALINT_MAX)
                    l_uWindow = l_pCon->WIN;
                else {
                    if (l_uWindow != l_pCon->WIN)
                        l_uWindow = 0;
                }
            }
        }
        i_pDataBase->GSIGNAL[i]->WIN = l_uWindow;
    }

    // répartition des signaux locaux à une fenetre
    initSignalList(i_pDataBase);
    for (i = 0; i <= i_pDataBase->NB_GSIGNAL - 1; i++) {
        insertSignalWindow(i_pDataBase, i_pDataBase->GSIGNAL[i]);
    }
}


void
createWindowArray(ocrRoutingDataBase * i_pDataBase,
                  ocrNaturalInt i_uWinNum)
{
    ocrNaturalInt n = 0;
    ocrSignal *l_pSignal;

    // création du tableau
    i_pDataBase->SIGNAL = (ocrSignal **)
        mbkalloc(i_pDataBase->WINDOWS[i_uWinNum]->NB_SIG *
                 sizeof(ocrSignal *));

    // Mise à jour du tableau
    i_pDataBase->NB_SIGNAL = i_pDataBase->WINDOWS[i_uWinNum]->NB_SIG;

    for (l_pSignal = i_pDataBase->FSIGNAL[i_uWinNum];
         l_pSignal; l_pSignal = l_pSignal->NEXT)
        i_pDataBase->SIGNAL[n++] = l_pSignal;
}

void
deleteWindowArray(ocrRoutingDataBase * i_pDataBase,
                  ocrNaturalInt i_uWinNum)
{
    mbkfree(i_pDataBase->SIGNAL);
    i_pDataBase->SIGNAL = NULL;
}
