/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:20 $
   $Log: ocrWeightedTree.c,v $
   Revision 1.1  2002/03/15 14:37:20  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:18  hcl
   First commit in the new tree.

   Revision 1.3  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.2  2001/12/14 15:58:36  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

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
#include "display.h"
#include "ocrConnectorUtil.h"
#include "ocrMst.h"
#include "ocrWeightedTree.h"

#define SCALE_X 100
#define PITCH 5

extern ocrOption *g_pOption;

#define LEVEL (g_pOption->LEVEL)

static ocrNaturalInt g_uNbF;
static float g_fMaxCost;

// Table de Densité
static float *g_tCostGrid;

static ocrNaturalInt *g_tEdge;
static ocrNaturalInt *g_tEdgeCopy;
static ocrNaturalInt *g_tSignal;
static ocrNaturalInt *g_tMerge;

/**
 * calcul le cout d'un arbre
**/
float getCost(ocrNaturalInt * i_tGrid, float *i_tCostGrid)
{
    ocrNaturalInt i;
    float som = 0.0;

    for (i = 0; i < g_uNbF; i++)
        som += i_tGrid[i] * i_tCostGrid[i];
    return som;
}

/**
 * Merge 2 arbres
**/
void
mergeGrid(ocrNaturalInt * o_tMerge,
          ocrNaturalInt * i_tGrid1, ocrNaturalInt * i_tGrid2)
{
    ocrNaturalInt i;

    for (i = 0; i < g_uNbF; i++)
        o_tMerge[i] = i_tGrid1[i] || i_tGrid2[i];
}

/**
 * Copy un arbre
**/
void makeCopy(ocrNaturalInt * i_tGridDest, ocrNaturalInt * i_tGridSrc)
{
    ocrNaturalInt i;

    for (i = 0; i < g_uNbF; i++)
        i_tGridDest[i] = i_tGridSrc[i];
}

/**
 * Initialise un arbre
**/
void initialiseGrid(ocrNaturalInt * i_tGrid)
{
    ocrNaturalInt i;

    for (i = 0; i < g_uNbF; i++)
        i_tGrid[i] = 0;
}

/**
 * renvoie les 2 noeuds qui définissent le halfPerimeter
**/
void
halfPerimeter(ocrNode ** o_pNode1,
              ocrNode ** o_pNode2, ocrSignal * i_pSignal)
{
    ocrNode *l_pNode;
    ocrNode l_NodeMin = *(i_pSignal->GLOBAL);
    ocrNode l_NodeMax = *(i_pSignal->GLOBAL);

    for (l_pNode = i_pSignal->GLOBAL; l_pNode; l_pNode = l_pNode->NEXT) {

        if (l_pNode->X < l_NodeMin.X)
            l_NodeMin.X = l_pNode->X;
        if (l_pNode->Y < l_NodeMin.Y)
            l_NodeMin.Y = l_pNode->Y;

        if (l_pNode->X > l_NodeMax.X)
            l_NodeMax.X = l_pNode->X;
        if (l_pNode->Y > l_NodeMax.Y)
            l_NodeMax.Y = l_pNode->Y;
    }
    *o_pNode1 = dupVirtualConnector(&l_NodeMin);
    *o_pNode2 = dupVirtualConnector(&l_NodeMax);
}

/**
 * renvoie le cout d'un noeud
**/
float computeDensity(ocrNode * i_pNodeMin, ocrNode * i_pNodeMax)
{
    float l_fX, l_fY;

    l_fX = i_pNodeMax->X - i_pNodeMin->X;
    l_fY = i_pNodeMax->Y - i_pNodeMin->Y;

    return ((float) (l_fX + l_fY + 1)) / ((float) (l_fX + 1) * (l_fY + 1));
}

/**
 * Incrémente/Décrémente la grille de Densité avec l'incrément +1/-1
 * sur toute la surface du demi périmetre.
**/
void
incDensityTable(ocrNode * i_pNodeMin, ocrNode * i_pNodeMax, int i_nInc)
{
    ocrNaturalInt i, j;
    ocrNaturalInt l_uFace = sqrt(g_uNbF);
    float l_fDens;

    l_fDens = computeDensity(i_pNodeMin, i_pNodeMax);
    for (i = i_pNodeMin->X; i <= i_pNodeMax->X; i++)
        for (j = i_pNodeMin->Y; j <= i_pNodeMax->Y; j++)
            g_tCostGrid[j * l_uFace + i] = g_tCostGrid[j * l_uFace + i] +
                (float) i_nInc *l_fDens;
}

void adjustDensityTable(ocrNaturalInt * i_pSignalGrid)
{
    ocrNaturalInt i;

    for (i = 0; i < g_uNbF; i++)
        g_tCostGrid[i] += i_pSignalGrid[i];

}

/**
 * Affiche la grille de Densité
**/
void dumpDensityTable()
{
    int i, j;

    for (i = 0; i < sqrt(g_uNbF); i++) {
        for (j = 0; j < sqrt(g_uNbF); j++)
            printf("%f ", g_tCostGrid[i * (int) (sqrt(g_uNbF)) + j]);
        puts("");
    }
}

/**
 * Affiche la un signal
**/
void dumpSignalTable(ocrNaturalInt * i_tGrid)
{
    int i, j;

    for (i = 0; i < sqrt(g_uNbF); i++) {
        for (j = 0; j < sqrt(g_uNbF); j++)
            printf("%ld ", i_tGrid[i * (int) (sqrt(g_uNbF)) + j]);
        puts("");
    }
    puts("");
}



/**
 * Création de la Grille de Densité
**/
void makeDensityTable(ocrRoutingDataBase * i_pDataBase)
{
    ocrNaturalInt i;
    ocrSignal *l_pSignal;
    ocrNode *l_pNodeMin, *l_pNodeMax;

    // Création
    g_tCostGrid = (float *) mbkalloc(sizeof(float) * i_pDataBase->NB_F);

    // Initialisation
    for (i = 0; i < i_pDataBase->NB_F; i++)
        g_tCostGrid[i] = 0.0;

    // Calcul pour chaque signal
    for (l_pSignal = i_pDataBase->FSIGNAL[0];
         l_pSignal; l_pSignal = l_pSignal->NEXT) {
        // Calcul du demi prérimetre
        halfPerimeter(&l_pNodeMin, &l_pNodeMax, l_pSignal);
        // Incrémente sur la surface du demi périmetre
        incDensityTable(l_pNodeMin, l_pNodeMax, +1);
        // free les 2 noeuds
        deleteVirtualConnectorList(l_pNodeMin);
        deleteVirtualConnectorList(l_pNodeMax);
    }

}


/**
 * Initialisation pour WeightedTree
**/
void initWeightedTree(ocrRoutingDataBase * i_pDataBase)
{
    g_uNbF = i_pDataBase->NB_F;

    // Création de la grille de Densité 
    makeDensityTable(i_pDataBase);

    // Création & Initialisation des grilles

    g_tEdge =
        (ocrNaturalInt *) mbkalloc(sizeof(ocrNaturalInt) *
                                   i_pDataBase->NB_F);
    g_tEdgeCopy =
        (ocrNaturalInt *) mbkalloc(sizeof(ocrNaturalInt) *
                                   i_pDataBase->NB_F);
    g_tSignal =
        (ocrNaturalInt *) mbkalloc(sizeof(ocrNaturalInt) *
                                   i_pDataBase->NB_F);
    g_tMerge =
        (ocrNaturalInt *) mbkalloc(sizeof(ocrNaturalInt) *
                                   i_pDataBase->NB_F);

    initialiseGrid(g_tEdge);
    initialiseGrid(g_tEdgeCopy);
    initialiseGrid(g_tSignal);
    initialiseGrid(g_tMerge);
}

/**
 * free pour WeightedTree
**/
void freeWeightedTree()
{
    mbkfree(g_tEdge);
    mbkfree(g_tEdgeCopy);
    mbkfree(g_tSignal);
    mbkfree(g_tMerge);
    mbkfree(g_tCostGrid);

    g_tCostGrid = NULL;
    g_tEdge = NULL;
    g_tEdgeCopy = NULL;
    g_tSignal = NULL;
    g_tMerge = NULL;
}

void
searchingStairCase(ocrNaturalInt i_uX1,
                   ocrNaturalInt i_uY1,
                   ocrNaturalInt i_uX2, ocrNaturalInt i_uY2)
{
    float l_fCost;
    ocrNaturalInt l_uNbFace = sqrt(g_uNbF);

    g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 1;

    if ((i_uX1 == i_uX2) && (i_uY1 == i_uY2)) {
        mergeGrid(g_tMerge, g_tEdge, g_tSignal);
        l_fCost = getCost(g_tMerge, g_tCostGrid);
        if (l_fCost < g_fMaxCost) {
            makeCopy(g_tEdgeCopy, g_tEdge);
            g_fMaxCost = l_fCost;
        }
    } else {
        if (i_uX1 < i_uX2) {
            i_uX1++;
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 1;
            searchingStairCase(i_uX1, i_uY1, i_uX2, i_uY2);
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 0;
            i_uX1--;
        }

        if (i_uX2 < i_uX1) {
            i_uX1--;
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 1;
            searchingStairCase(i_uX1, i_uY1, i_uX2, i_uY2);
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 0;
            i_uX1++;
        }

        if (i_uY1 < i_uY2) {
            i_uY1++;
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 1;
            searchingStairCase(i_uX1, i_uY1, i_uX2, i_uY2);
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 0;
            i_uY1--;
        }

        if (i_uY2 < i_uY1) {
            i_uY1--;
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 1;
            searchingStairCase(i_uX1, i_uY1, i_uX2, i_uY2);
            g_tEdge[i_uY1 * l_uNbFace + i_uX1] = 0;
            i_uY1++;
        }
    }

}

/**
 * Supprime les doublons des segments
**/
void clearSegList(ocrSignal * i_pSignal)
{
    ocrWSegment *l_pSegment;
    ocrWSegment *l_pSegment2;

    if (i_pSignal->SEGMENT == NULL)
        return;

    for (l_pSegment = i_pSignal->SEGMENT; l_pSegment->NEXT;
         l_pSegment = l_pSegment->NEXT) {
        for (l_pSegment2 = l_pSegment->NEXT; l_pSegment2;
             l_pSegment2 = l_pSegment2->NEXT) {
            if (l_pSegment->LAYER == l_pSegment2->LAYER)
                if (l_pSegment->OFFSET == l_pSegment2->OFFSET) {
                    if ((l_pSegment->P_MAX <= l_pSegment2->P_MIN) ||
                        (l_pSegment2->P_MAX <= l_pSegment->P_MIN)) {
                    } else {
                        addSegmentSignalList(i_pSignal,
                                             createWSegment(l_pSegment->
                                                            OFFSET,
                                                            l_pSegment->
                                                            LAYER,
                                                            MIN
                                                            (l_pSegment2->
                                                             P_MIN,
                                                             l_pSegment->
                                                             P_MIN),
                                                            MAX
                                                            (l_pSegment2->
                                                             P_MAX,
                                                             l_pSegment->
                                                             P_MAX),
                                                            i_pSignal->
                                                            INDEX));
                        deleteSegmentSignalList(i_pSignal, l_pSegment);
                        deleteSegmentSignalList(i_pSignal, l_pSegment2);
                    }
                }
        }
    }
}

#define GET_NEXT_X(x) (x+1)
#define GET_PREV_X(x) (x-1)
#define GET_NEXT_Y(y) (ocrNaturalInt)(y+sqrt (g_uNbF))
#define GET_PREV_Y(y) (ocrNaturalInt)(y-sqrt (g_uNbF))

/**
 * Recherche des segments hor dans la grille
**/
void
translateGridToHorSegments(ocrSignal * i_pSignal, ocrNaturalInt * i_tGrid)
{
    ocrNaturalInt y, ok = 0, max, x;
    ocrNaturalInt l_uXMin = 0, l_uXMax = 0;
    ocrWSegment *l_pSegment;

    for (y = 0; y < g_uNbF; y = GET_NEXT_Y(y)) {
        ok = 0;
        x = y;
        max = x + sqrt(g_uNbF) - 1;
        while ((x < max) && (i_tGrid[x] == 0))
            x = GET_NEXT_X(x);
        if ((x != max) && (i_tGrid[x]) && (i_tGrid[GET_NEXT_X(x)])) {
            ok = 1;
            l_uXMin = x;
            while ((x < max) && (i_tGrid[x]))
                x = GET_NEXT_X(x);
            if (x < max)
                l_uXMax = GET_PREV_X(x);
            else {
                if (i_tGrid[x])
                    l_uXMax = x;
                else
                    l_uXMax = GET_PREV_X(x);
            }
        }

        if (ok) {
            // création du segment [ l_uXMin, l_uXMax] hor
            l_pSegment =
                createWSegment(y / (int) sqrt(g_uNbF), 0,
                               l_uXMin % (int) sqrt(g_uNbF),
                               l_uXMax % (int) sqrt(g_uNbF), 0);
            // insertion dans la liste
            addSegmentSignalList(i_pSignal, l_pSegment);
        }
    }
}

void
translateGridToVerSegments(ocrSignal * i_pSignal, ocrNaturalInt * i_tGrid)
{
    ocrNaturalInt x, ok = 0, max, y;
    ocrNaturalInt l_uYMin = 0, l_uYMax = 0;
    ocrWSegment *l_pSegment;

    for (x = 0; x < sqrt(g_uNbF); x = GET_NEXT_X(x)) {
        ok = 0;
        y = x;
        max = y + (sqrt(g_uNbF) - 1) * sqrt(g_uNbF);
        while ((y < max) && (i_tGrid[y] == 0))
            y = GET_NEXT_Y(y);
        if ((y != max) && (i_tGrid[y]) && (i_tGrid[GET_NEXT_Y(y)])) {
            ok = 1;
            l_uYMin = y;
            while ((y < max) && (i_tGrid[y]))
                y = GET_NEXT_Y(y);
            if (y < max)
                l_uYMax = GET_PREV_Y(y);
            else {
                if (i_tGrid[y])
                    l_uYMax = y;
                else
                    l_uYMax = GET_PREV_Y(y);
            }
        }

        if (ok) {
            // création du segment [ l_uYMin, l_uYMax] hor
            l_pSegment = createWSegment(x % (int) sqrt(g_uNbF), 1,
                                        l_uYMin / (int) sqrt(g_uNbF),
                                        l_uYMax / (int) sqrt(g_uNbF), 0);
            // insertion dans la liste
            l_pSegment->NEXT = i_pSignal->SEGMENT;
            i_pSignal->SEGMENT = l_pSegment;
        }
    }
}

/**
 * ajoute dans le champ SEGMENT du signal
 * les segments issus du routage global d'une arrete
**/
void
translateGridToSegments(ocrSignal * i_pSignal, ocrNaturalInt * i_tGrid)
{
    translateGridToHorSegments(i_pSignal, i_tGrid);
    translateGridToVerSegments(i_pSignal, i_tGrid);
}

/**
 * Création du WeightedTree
**/
void
makingWeightedTree(ocrRoutingDataBase * i_pDataBase,
                   ocrSignal * i_pSignal, ocrEdge * i_pEdge)
{
    ocrEdge *l_pEdge;
    ocrNode *l_pNodeMin, *l_pNodeMax;

    initialiseGrid(g_tSignal);
    initialiseGrid(g_tEdge);
    initialiseGrid(g_tEdgeCopy);

    g_fMaxCost = OCRNATURALINT_MAX;
//  dumpVirConList (i_pSignal->GLOBAL, stdout);
    //  dumpSignalTable (g_tSignal);

    for (l_pEdge = i_pEdge; l_pEdge; l_pEdge = l_pEdge->NEXT) {
        //  initialiseGrid (g_tEdgeCopy);
        initialiseGrid(g_tEdge);
        searchingStairCase(l_pEdge->N1->X, l_pEdge->N1->Y,
                           l_pEdge->N2->X, l_pEdge->N2->Y);
        // grille vers segments
        translateGridToSegments(i_pSignal, g_tEdgeCopy);
        // Merge des arretes
        mergeGrid(g_tSignal, g_tEdgeCopy, g_tSignal);
        g_fMaxCost = OCRNATURALINT_MAX;
//                                                                               dumpSignalTable (g_tSignal);
        clearSegList(i_pSignal);
//                                                                               dumpSegList (i_pSignal->SEGMENT, stdout);
    }

    // Calcul du demi prérimetre
    halfPerimeter(&l_pNodeMin, &l_pNodeMax, i_pSignal);
    // Décrémente sur la surface du demi périmetre
    incDensityTable(l_pNodeMin, l_pNodeMax, -1);
    // Ajustement de la table de densité avec le signal réel
    adjustDensityTable(g_tSignal);
    // Création des segments
}
