/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:20 $
   $Log: ocrMst.c,v $
   Revision 1.1  2002/03/15 14:37:20  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:17  hcl
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

#define SCALE_X 100
#define PITCH 5

extern ocrOption *g_pOption;

ocrEdge **g_tEdge;
ocrNaturalInt g_uNode;

#define LEVEL (g_pOption->LEVEL)

ocrEdge *createEdge(ocrNode * i_pNode1, ocrNode * i_pNode2)
{
    ocrEdge *l_pEdge;

    l_pEdge = (ocrEdge *) mbkalloc(sizeof(ocrEdge));

    l_pEdge->N1 = i_pNode1;
    l_pEdge->N2 = i_pNode2;
    l_pEdge->COST = distBetween2VirCon(i_pNode1, i_pNode2);
    return l_pEdge;
}

void deleteEdge(ocrEdge * i_pEdge)
{
    if (i_pEdge)
        mbkfree(i_pEdge);
}

ocrNaturalInt nbOfEdge(ocrNaturalInt i_uNode)
{
    return i_uNode * (i_uNode - 1) / 2;
}

ocrNaturalInt isNodeInSet(ocrNode * i_pNode, chain_list * i_pNodeList)
{
    if (i_pNodeList == NULL)
        return 0;
    else if (i_pNode == i_pNodeList->DATA)
        return 1;
    else
        return isNodeInSet(i_pNode, i_pNodeList->NEXT);
}

ocrNaturalInt computeEdge(ocrEdge * i_pEdge, chain_list ** i_pSet)
{
    ocrNaturalInt i;
    ocrNaturalInt l_uNode1 = 0, l_uNode2 = 0;
    chain_list *l_pTmpList, *l_pList1;

    for (i = 0; i < g_uNode; i++) {
        if (isNodeInSet(i_pEdge->N1, i_pSet[i]))
            l_uNode1 = i;

        if (isNodeInSet(i_pEdge->N2, i_pSet[i]))
            l_uNode2 = i;
    }

    if (l_uNode1 != l_uNode2) {
        // Merge des 2 Set
        l_pList1 = i_pSet[l_uNode1];
        i_pSet[l_uNode1] = NULL;
        for (l_pTmpList = i_pSet[l_uNode2]; l_pTmpList->NEXT;
             l_pTmpList = l_pTmpList->NEXT);
        l_pTmpList->NEXT = l_pList1;

        return 1;
    }
    return 0;
}

void dumpCost()
{
    ocrNaturalInt i;

    printf("Nodes : %ld -> : Edges %ld\n", g_uNode, nbOfEdge(g_uNode));
    for (i = 0; i < nbOfEdge(g_uNode); i++)
        printf("%ld\n", g_tEdge[i]->COST);
}

int costSort(const void *a, const void *b)
{
    return (*(ocrEdge **) a)->COST - (*(ocrEdge **) b)->COST;
}

void makingEdges(ocrNode * i_NodeList)
{
    ocrNaturalInt n = 0;
    ocrNode *l_pNode, *l_pFirst, *l_pSecond;

    for (l_pNode = i_NodeList, g_uNode = 0; l_pNode;
         l_pNode = (ocrNode *) l_pNode->NEXT, g_uNode++);

    g_tEdge = (ocrEdge **) mbkalloc(sizeof(ocrEdge *) * nbOfEdge(g_uNode));

    for (l_pFirst = i_NodeList; l_pFirst->NEXT; l_pFirst = l_pFirst->NEXT)
        for (l_pSecond = l_pFirst->NEXT; l_pSecond;
             l_pSecond = l_pSecond->NEXT)
            g_tEdge[n++] = createEdge(l_pFirst, l_pSecond);

    qsort(g_tEdge, nbOfEdge(g_uNode), sizeof(ocrEdge *), costSort);

//  dumpCost ();
}

void dumpSet()
{
}

ocrEdge *chooseEdges(ocrNode * i_NodeList)
{
    chain_list **l_pSet;
    ocrNode *l_pNode = i_NodeList;
    ocrNaturalInt i;
    ocrEdge *l_pReturnEdge = NULL;
    ocrEdge *l_pEdge = NULL;

    // Initialisation
    l_pSet = (chain_list **) mbkalloc(g_uNode * sizeof(chain_list *));

    for (i = 0; i < g_uNode; i++) {
        l_pSet[i] = NULL;
        l_pSet[i] = addchain(l_pSet[i], l_pNode);
        l_pNode = l_pNode->NEXT;
    }

    // construction du MST
    for (i = 0; i < nbOfEdge(g_uNode); i++)
        if (computeEdge(g_tEdge[i], l_pSet)) {
            if (!l_pEdge) {
                l_pEdge = g_tEdge[i];
                l_pEdge->NEXT = NULL;
                l_pReturnEdge = l_pEdge;
            } else {
                l_pEdge->NEXT = g_tEdge[i];
                l_pEdge = l_pEdge->NEXT;
                l_pEdge->NEXT = NULL;
            }
        } else {
            deleteEdge(g_tEdge[i]);
            g_tEdge[i] = NULL;
        }

    // Free Set
    for (i = 0; i < g_uNode; i++)
        freechain(l_pSet[i]);
    mbkfree(l_pSet);
    l_pSet = NULL;

    // Return
    return l_pReturnEdge;
}

/**
 * Construit le Minimum Spaning Tree
**/
ocrEdge *makingMst(ocrSignal * i_pSignal)
{
    ocrEdge *l_pEdge = NULL;

    // Construction de toutes les arretes
    makingEdges(i_pSignal->GLOBAL);
    // Choix des arretes pour le MST
    l_pEdge = chooseEdges(i_pSignal->GLOBAL);

//  mbkfree (g_tEdge);
    //  g_tEdge = NULL;

    return l_pEdge;
}
