/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:27 $
   $Log: ocrSignalUtil.c,v $
   Revision 1.1  2002/03/15 14:37:27  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:25  hcl
   First commit in the new tree.

   Revision 1.5  2001/12/20 13:04:04  hcl
   Cosmetique.

   Revision 1.4  2001/12/14 15:58:40  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.3  2001/12/03 14:31:24  hcl
   Pour la route.

   Revision 1.2  2001/11/20 09:42:06  hcl
   last but not least

   Revision 1.1  2001/09/26 14:27:56  hcl
   premier commit....


   ### -------------------------------------------------- ###
 */

#include <stdlib.h>
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"
#include "ocr.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "ocrPath.h"
#include "display.h"

#include "ocrSignalUtil.h"
#include "ocrConnectorUtil.h"

static char *res_id =
    "$Id: ocrSignalUtil.c,v 1.1 2002/03/15 14:37:27 hcl Exp $";

extern ocrOption *g_pOption;

#define LEVEL (g_pOption->LEVEL)

/**
 * lie tous les connecteur d'un signal
**/
void linkSignal(ocrRoutingDataBase * i_pDataBase, ocrSignal * i_pSignal)
{
    ocrConnector *l_pCon;

    for (l_pCon = i_pSignal->CON_LIST; l_pCon; l_pCon = l_pCon->NEXT) {
        linkConnector(i_pDataBase, i_pSignal->INDEX, l_pCon);
    }
}

/**
 * détermine le nombre de signaux à router
**/
ocrNaturalInt getNumSignal(lofig_list * i_pLoFig)
{
    losig_list *l_pLoSig;
    ocrNaturalInt l_nNbSig = 0;

    // détermine le nombre de signaux à router
    for (l_pLoSig = i_pLoFig->LOSIG; l_pLoSig; l_pLoSig = l_pLoSig->NEXT)
        l_nNbSig++;

    return l_nNbSig;
}

/**
 * Initialisation des signaux à router : tous sauf VDD et VSS
 * création des signaux dans la bd
**/
void
initGlobalSignalList(ocrRoutingDataBase * i_pDataBase,
                     lofig_list * i_pLoFig)
{
    losig_list *l_pLoSig;

    createGlobalSignalList(i_pDataBase, getNumSignal(i_pLoFig));

    for (l_pLoSig = i_pLoFig->LOSIG; l_pLoSig; l_pLoSig = l_pLoSig->NEXT) {
        if ((!isvss(l_pLoSig->NAMECHAIN->DATA)) &&
            (!isvdd(l_pLoSig->NAMECHAIN->DATA))) {
            ocrSignal *l_pSignal;

            l_pSignal = createSignal(l_pLoSig);
            addSignalGlobal(i_pDataBase, l_pSignal);
        }
    }
}

/*
 * FONCTIONS D'ORDONNANCEMENT DE SIGNAUX
 */

int funcCon(const void *a, const void *b)
{
    //return (*(ocrSignal **) b)->NB_CON - (*(ocrSignal **) a)->NB_CON;
    return (*(ocrSignal **) a)->NB_CON - (*(ocrSignal **) b)->NB_CON;
}

int funcRandom(const void *a, const void *b)
{
    return (((double) random() / RAND_MAX) - 0.5) < 0;
}

int funcPriority(const void *a, const void *b)
{
    if ((*(ocrSignal **) b)->PRIORITY == (*(ocrSignal **) a)->PRIORITY)
        return 0;
    else
        return (*(ocrSignal **) b)->PRIORITY -
            (*(ocrSignal **) a)->PRIORITY;
}

int funcPriorityRandom(const void *a, const void *b)
{
    if ((*(ocrSignal **) b)->PRIORITY == (*(ocrSignal **) a)->PRIORITY)
        return (((double) random() / RAND_MAX) - 0.5) < 0;
    else
        return (*(ocrSignal **) b)->PRIORITY -
            (*(ocrSignal **) a)->PRIORITY;
}

int funcPriorityCon(const void *a, const void *b)
{
    if ((*(ocrSignal **) b)->PRIORITY == (*(ocrSignal **) a)->PRIORITY)
        return (*(ocrSignal **) b)->NB_CON - (*(ocrSignal **) a)->NB_CON;
    //return (*(ocrSignal **) a)->NB_CON - (*(ocrSignal **) b)->NB_CON;
    else
        return (*(ocrSignal **) b)->PRIORITY -
            (*(ocrSignal **) a)->PRIORITY;
}

/**
 * Détermine si un signal possède tous sauf un de ses connecteurs
 * de pris. (Urgent à router)
 * recherche à partir du rang i_uRang.
 *
 * retourne le numéro du signal
**/
ocrNaturalInt
isCriticalSignal(ocrRoutingDataBase * i_pDataBase, ocrNaturalInt i_uRang)
{
    ocrNaturalInt i;
    ocrConnector *l_pCon;

    for (i = i_uRang; i < i_pDataBase->NB_SIGNAL - 1; i++) {
        if (i_pDataBase->SIGNAL[i]->ROUTED == 0)
            for (l_pCon = i_pDataBase->SIGNAL[i]->CON_LIST;
                 l_pCon; l_pCon = l_pCon->NEXT) {
                if (l_pCon->INTEXT == INTERNAL)
                    if (isCriticalConnector(i_pDataBase->GRID, l_pCon))
                        return i;
            }
    }
    return OCRNATURALINT_MAX;
}

void
orderingSignals(ocrSignal ** i_pSignal,
                ocrNaturalInt i_uNbSig, ocrNaturalShort i_uType)
{
    switch (i_uType) {
    case ORDER_RANDOM:
        {
            display(LEVEL, VERBOSE, "%s\n", "o Ordering : Random ...");
            qsort(i_pSignal, i_uNbSig, sizeof(ocrSignal *), funcRandom);
            break;
        }
    case ORDER_CON:
        {
            display(LEVEL, VERBOSE, "%s\n", "o Ordering : Connectors ...");
            qsort(i_pSignal, i_uNbSig, sizeof(ocrSignal *), funcCon);
            break;
        }
    case ORDER_PRIORITY:
        {
            display(LEVEL, VERBOSE, "%s\n", "o Ordering : Priority ...");
            qsort(i_pSignal, i_uNbSig, sizeof(ocrSignal *), funcPriority);
            break;
        }
    case ORDER_PRIORITY_RANDOM:
        {
            display(LEVEL, VERBOSE, "%s\n",
                    "o Ordering : Priority Random ...");
            qsort(i_pSignal, i_uNbSig, sizeof(ocrSignal *),
                  funcPriorityRandom);
            break;
        }

    case ORDER_PRIORITY_CON:
        {
            display(LEVEL, VERBOSE, "%s\n",
                    "o Ordering : Priority, Connectors ...");
            qsort(i_pSignal, i_uNbSig, sizeof(ocrSignal *),
                  funcPriorityCon);
            break;
        }
    default:
        {
            display (LEVEL, ERROR, "orderingSignals in %s:%d\n", __FILE__, __LINE__);
            exit(1);
        }
    }
}
