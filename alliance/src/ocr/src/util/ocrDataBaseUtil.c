/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/04/25 13:41:34 $
   $Log: ocrDataBaseUtil.c,v $
   Revision 1.2  2002/04/25 13:41:34  hcl
   New ripup/reroute loop, bug-kill (CALU&TALU).

   Revision 1.1  2002/03/15 14:37:26  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:25  hcl
   First commit in the new tree.

   Revision 1.5  2001/12/20 13:04:04  hcl
   Cosmetique.

   Revision 1.4  2001/12/14 15:58:40  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.3  2001/11/20 11:02:04  hcl
   Derecursivation de deleteVirtualConnectorList

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

static char *res_id =
    "$Id: ocrDataBaseUtil.c,v 1.2 2002/04/25 13:41:34 hcl Exp $";
extern ocrOption *g_pOption;
#define LEVEL (g_pOption->LEVEL)

/*
 * Base de donnée du routage
 */

void dumpVirCon(ocrVirtualConnector * i_pVirConList, FILE * i_pFile)
{
    if (i_pVirConList) {
        fprintf(i_pFile, "      X : %ld  Y : %ld  Z : %ld TAG : %d\n",
                i_pVirConList->X,
                i_pVirConList->Y, i_pVirConList->Z, i_pVirConList->TAG);
        fflush(i_pFile);
    }
}

void dumpVirConList(ocrVirtualConnector * i_pVirConList, FILE * i_pFile)
{
    if (i_pVirConList) {
        fprintf(i_pFile, "      X : %ld  Y : %ld  Z : %ld TAG : %d\n",
                i_pVirConList->X,
                i_pVirConList->Y, i_pVirConList->Z, i_pVirConList->TAG);

        if (i_pVirConList->NEXT)
            dumpVirConList((ocrVirtualConnector *) i_pVirConList->NEXT,
                           i_pFile);

    } else
        puts("NULL");
    fflush(i_pFile);
}

void dumpSegList(ocrWSegment * i_pSegment, FILE * i_pFile)
{
    if (i_pSegment) {
        fprintf(i_pFile, "    PMIN : %ld PMAX : %ld OFFSET"
                " : %ld LAYER : %ld INDEX : %ld\n",
                i_pSegment->P_MIN * 5,
                i_pSegment->P_MAX * 5,
                i_pSegment->OFFSET * 5,
                i_pSegment->LAYER, i_pSegment->SIGNAL_INDEX);
        dumpSegList(i_pSegment->NEXT, i_pFile);
    }
}

/**
 * affiche la liste de connecteurs,
 * ainsi que ses connecteurs virtuels
**/
void dumpConList(ocrConnector * i_pConList, FILE * i_pFile)
{
    if (i_pConList) {
        fprintf(i_pFile, "    NAME   : %s\n", i_pConList->NAME);
        fprintf(i_pFile, "    INSNAME: %s\n", i_pConList->INSNAME);
        fprintf(i_pFile, "    FACE   : %d\n", i_pConList->FACE);
        fprintf(i_pFile, "    WIN    : %d\n", i_pConList->WIN);

        if (i_pConList->INTEXT == INTERNAL)
            fprintf(i_pFile, "    INT/EXT: INTERNAL\n");
        else
            fprintf(i_pFile, "    INT/EXT: EXTERNAL\n");

        if (i_pConList->CON) {
            fprintf(i_pFile, "    CON    : \n");
            fprintf(i_pFile, "      X : %ld  Y : %ld\n",
                    i_pConList->CON->X, i_pConList->CON->Y);
        }

        fflush(i_pFile);
        if (i_pConList->VIR_CON_LIST) {
            fprintf(i_pFile, "    CONVIRT: \n");
            dumpVirConList(i_pConList->VIR_CON_LIST, i_pFile);
        }
        if (i_pConList->NEXT)
            dumpConList(i_pConList->NEXT, i_pFile);
    }
}
void dumpSignalToFile(ocrSignal * i_pSignal, FILE * i_pFile)
{
    fprintf(i_pFile, "NAME	: %s\n", i_pSignal->NAME);
    fprintf(i_pFile, "  INDEX   : %ld\n", i_pSignal->INDEX);
    fprintf(i_pFile, "  NB CON  : %ld\n", i_pSignal->NB_CON);
    fprintf(i_pFile, "  PRIORITY: %d\n", i_pSignal->PRIORITY);

    if (i_pSignal->INTEXT == INTERNAL)
        fprintf(i_pFile, "  INTEXT  : INTERNAL\n");
    else
        fprintf(i_pFile, "  INTEXT  : EXTERNAL\n");

    if (i_pSignal->CON_LIST)
        dumpConList(i_pSignal->CON_LIST, i_pFile);
/*
  if (i_pSignal->SEGMENT)
    {
      fprintf (i_pFile, "  SEGMENTS :\n");
      fflush (i_pFile);
      dumpSegList (i_pSignal->SEGMENT, i_pFile);
    }

  if (i_pSignal->VIA)
    {
      fprintf (i_pFile, "  VIAS :\n");
      fflush (i_pFile);
      dumpVirConList (i_pSignal->VIA, i_pFile);
    }
*/
}

void dumpGlobalSignal(ocrRoutingDataBase * i_pDataBase, FILE * i_pFile)
{
    ocrNaturalInt i;

    display (LEVEL, VERBOSE, "o Number of signals : %ld\n", i_pDataBase->NB_GSIGNAL);
    for (i = 0; i < i_pDataBase->NB_GSIGNAL; i++) {
        fprintf(i_pFile, "NAME	: %s\n", i_pDataBase->GSIGNAL[i]->NAME);
        fprintf(i_pFile, "  INDEX   : %ld\n",
                i_pDataBase->GSIGNAL[i]->INDEX);
        fprintf(i_pFile, "  NB CON  : %ld\n",
                i_pDataBase->GSIGNAL[i]->NB_CON);
        fprintf(i_pFile, "  PRIORITY: %d\n",
                i_pDataBase->GSIGNAL[i]->PRIORITY);

        if (i_pDataBase->GSIGNAL[i]->INTEXT == INTERNAL)
            fprintf(i_pFile, "  INTEXT  : INTERNAL\n");
        else
            fprintf(i_pFile, "  INTEXT  : EXTERNAL\n");

        if (i_pDataBase->GSIGNAL[i]->CON_LIST)
            dumpConList(i_pDataBase->GSIGNAL[i]->CON_LIST, i_pFile);

        if (i_pDataBase->GSIGNAL[i]->SEGMENT) {
            fprintf(i_pFile, "  SEGMENTS :\n");
            fflush(i_pFile);
            dumpSegList(i_pDataBase->GSIGNAL[i]->SEGMENT, i_pFile);
        }

        if (i_pDataBase->GSIGNAL[i]->VIA) {
            fprintf(i_pFile, "  VIAS :\n");
            fflush(i_pFile);
            dumpVirConList(i_pDataBase->GSIGNAL[i]->VIA, i_pFile);
        }
    }
}

/**
 * affichage de la bd
**/
void dumpDataBase(ocrRoutingDataBase * i_pDataBase, FILE * i_pFile)
{
    if (i_pDataBase) {
        fprintf(i_pFile, "NAME	: %s\n", i_pDataBase->NAME);
        fprintf(i_pFile, "XAB1	: %ld\n", i_pDataBase->XAB1);
        fprintf(i_pFile, "XAB2	: %ld\n", i_pDataBase->XAB2);
        fprintf(i_pFile, "YAB1	: %ld\n", i_pDataBase->YAB1);
        fprintf(i_pFile, "YAB2	: %ld\n", i_pDataBase->YAB2);

        if (i_pDataBase->GSIGNAL)
            dumpGlobalSignal(i_pDataBase, i_pFile);
    }
    fprintf(i_pFile, "dump complete !\n");

    fflush(i_pFile);
}

/**
 * création de la base de donnée
**/
ocrRoutingDataBase *createDataBase(phfig_list * i_pPhFig)
{
    ocrRoutingDataBase *l_pDataBase;

    l_pDataBase =
        (ocrRoutingDataBase *) mbkalloc(sizeof(ocrRoutingDataBase));

    l_pDataBase->NAME = namealloc(i_pPhFig->NAME);
    l_pDataBase->XAB1 = i_pPhFig->XAB1;
    l_pDataBase->XAB2 = i_pPhFig->XAB2;
    l_pDataBase->YAB1 = i_pPhFig->YAB1;
    l_pDataBase->YAB2 = i_pPhFig->YAB2;

    l_pDataBase->NB_OF_LAYERS = 3;

    l_pDataBase->NB_F = g_pOption->WINDOW;

    l_pDataBase->GSIGNAL = NULL;
    l_pDataBase->SIGNAL = NULL;
    l_pDataBase->FSIGNAL = NULL;

    l_pDataBase->NB_GSIGNAL = 0;
    l_pDataBase->NB_SIGNAL = 0;
    l_pDataBase->NB_FSIGNAL = NULL;

    l_pDataBase->WINDOWS = NULL;
    l_pDataBase->POINTS = NULL;

    l_pDataBase->GOBSTACLE = NULL;
    l_pDataBase->MFOBSTACLE = NULL;
    l_pDataBase->FOBSTACLE = NULL;

    l_pDataBase->HTABLE = NULL;
    l_pDataBase->PARAM = NULL;
    l_pDataBase->GRID = NULL;
    l_pDataBase->RIPUP = NULL;

    l_pDataBase->NB_IT = 0;
    l_pDataBase->NB_UNROUTED = 0;
    l_pDataBase->NB_ROUTED = 0;
    return l_pDataBase;
}

void deleteDataBase(ocrRoutingDataBase * i_pDataBase)
{
    if (i_pDataBase) {
        if (i_pDataBase->WINDOWS) {
            mbkfree(i_pDataBase->WINDOWS);
            i_pDataBase->WINDOWS = NULL;
        }
        mbkfree(i_pDataBase);
    }
}

/**
 * création de la liste des signaux
**/
void
createGlobalSignalList(ocrRoutingDataBase * i_pDataBase,
                       ocrNaturalInt i_uNbSignaux)
{
    i_pDataBase->GSIGNAL = (ocrSignal **)
        mbkalloc(sizeof(ocrSignal) * i_uNbSignaux);
}

/*
 * Gestion des signaux
 */

#define setSignalPriority(x,v)	((x)->PRIORITY = (v))
#define incSignalPriority(x)		((x)->PRIORITY ++)

/**
 * création d'un signal
**/
ocrSignal *createSignal(losig_list * i_pLoSig)
{
    ocrSignal *l_pSignal;

    l_pSignal = (ocrSignal *) mbkalloc(sizeof(ocrSignal));

    l_pSignal->NAME = namealloc(i_pLoSig->NAMECHAIN->DATA);
    l_pSignal->INDEX = i_pLoSig->INDEX;

    l_pSignal->NEXT = NULL;
    l_pSignal->SEGMENT = NULL;
    l_pSignal->VIA = NULL;
    l_pSignal->GLOBAL = NULL;

    l_pSignal->HARD = 0;

    l_pSignal->WIN = 0;
    l_pSignal->TYPE = 0;

    l_pSignal->ROUTED = 0;
    l_pSignal->NB_CON = 0;
    l_pSignal->PRIORITY = 0;
    l_pSignal->INTEXT = i_pLoSig->TYPE;
    l_pSignal->CON_LIST = NULL;

    l_pSignal->NICHT_ZU_ROUTIEREN = 0;

    return l_pSignal;
}

ocrSignal *dupSignal(ocrSignal * i_pSignal)
{
    ocrSignal *l_pSignal;

    l_pSignal = (ocrSignal *) mbkalloc(sizeof(ocrSignal));

    l_pSignal->NAME = i_pSignal->NAME;
    l_pSignal->INDEX = i_pSignal->INDEX;

    l_pSignal->NEXT = NULL;
    l_pSignal->SEGMENT = NULL;
    l_pSignal->VIA = NULL;
    l_pSignal->GLOBAL = NULL;

    l_pSignal->WIN = i_pSignal->WIN;

    l_pSignal->TYPE = 0;
    l_pSignal->ROUTED = 0;
    l_pSignal->NB_CON = 0;
    l_pSignal->PRIORITY = i_pSignal->PRIORITY;
    l_pSignal->INTEXT = i_pSignal->INTEXT;
    l_pSignal->CON_LIST = NULL;

    return l_pSignal;
}

/**
 * ajoute le signal dans la bd
**/
void
addSignalGlobal(ocrRoutingDataBase * i_pDataBase, ocrSignal * i_pSignal)
{
    (i_pDataBase->GSIGNAL)[i_pDataBase->NB_GSIGNAL] = i_pSignal;
    i_pDataBase->NB_GSIGNAL++;
}

/**
 *  Création d'un connecteur
**/

ocrConnector *createConnector(char *i_sName,
                              char *i_sInsName,
                              ocrNaturalShort i_uType,
                              ocrNaturalShort i_uIntExt,
                              ocrNaturalShort i_uFace,
                              ocrNaturalInt i_uOrder,
                              ocrNaturalInt i_uNumFMF)
{
    ocrConnector *l_pCon;

    l_pCon = (ocrConnector *) mbkalloc(sizeof(ocrConnector));

    l_pCon->NAME = namealloc(i_sName);
    l_pCon->INSNAME = namealloc(i_sInsName);
    l_pCon->TYPE = i_uType;
    l_pCon->INTEXT = i_uIntExt;
    l_pCon->FACE = i_uFace;
    l_pCon->ORDER = i_uOrder;
    l_pCon->WIN = i_uNumFMF;

    l_pCon->VIR_CON_LIST = NULL;
    l_pCon->CON = NULL;

    /* XXX HCl */
    l_pCon->NB_VC = 0;
    l_pCon->critVC = NULL;

    return l_pCon;
}

/**
 * ajoute le connecteur au signal
**/
void addConnector(ocrSignal * i_pSignal, ocrConnector * i_pCon)
{
    i_pCon->NEXT = i_pSignal->CON_LIST;
    i_pSignal->CON_LIST = i_pCon;
    i_pSignal->NB_CON++;
}

/**
 * Ajoute le segment i_pSegment au signal i_pSignal
 **/
void addSegmentSignalList(ocrSignal * i_pSignal, ocrWSegment * i_pSegment)
{
    i_pSegment->NEXT = i_pSignal->SEGMENT;
    i_pSignal->SEGMENT = i_pSegment;
//  dumpSegList (i_pSignal->SEGMENT, stdout);
}

/**
 * Supprime le segment de la liste associée au signal i_pSignal
**/

void
deleteSegmentSignalList(ocrSignal * i_pSignal, ocrWSegment * i_pSegment)
{
    ocrWSegment *l_pSegList;
    ocrWSegment *l_pSegListOld = NULL;

    for (l_pSegList = i_pSignal->SEGMENT;
         l_pSegList; l_pSegList = l_pSegList->NEXT) {
        if (l_pSegList == i_pSegment) {
            if (l_pSegListOld == NULL) {
                i_pSignal->SEGMENT = l_pSegList->NEXT;
            } else {
                l_pSegListOld->NEXT = l_pSegList->NEXT;
            }
        }
        l_pSegListOld = l_pSegList;
    }
}

/*
 * Gestion des connecteurs Virtuels
 */
ocrVirtualConnector *dupVirtualConnector(ocrVirtualConnector * i_pVirCon)
{
    ocrVirtualConnector *l_pCon;

    l_pCon = (ocrVirtualConnector *) mbkalloc(sizeof(ocrVirtualConnector));

    l_pCon->X = i_pVirCon->X;
    l_pCon->Y = i_pVirCon->Y;
    l_pCon->Z = i_pVirCon->Z;

    l_pCon->WIDTH = i_pVirCon->WIDTH;
    l_pCon->ORIENT = i_pVirCon->ORIENT;

    l_pCon->TAG = i_pVirCon->TAG;
    l_pCon->WIN = i_pVirCon->WIN;
    l_pCon->DIST = i_pVirCon->DIST;

    l_pCon->NEXT = NULL;

    return l_pCon;
}

/**
 * création d'un connecteur virtuel
**/
ocrVirtualConnector *createVirtualConnector(ocrNaturalInt i_uX,
                                            ocrNaturalInt i_uY,
                                            ocrNaturalInt i_uZ,
                                            ocrNaturalInt i_uWidth,
                                            ocrNaturalShort i_uOrient)
{
    ocrVirtualConnector *l_pCon;

    l_pCon = (ocrVirtualConnector *) mbkalloc(sizeof(ocrVirtualConnector));

    l_pCon->X = i_uX;
    l_pCon->Y = i_uY;
    l_pCon->Z = i_uZ;

    l_pCon->WIDTH = i_uWidth;
    l_pCon->ORIENT = i_uOrient;

    l_pCon->TAG = 0;
    l_pCon->WIN = 0;
    l_pCon->DIST = 0;

    l_pCon->NEXT = NULL;

    return l_pCon;
}

/**
 * ajoute un connceteur virtuel à une liste
**/
void
addVirtualConnector(ocrVirtualConnector ** i_pVirConList,
                    ocrVirtualConnector * i_pVirCon)
{
    (ocrVirtualConnector *) i_pVirCon->NEXT = *i_pVirConList;
    *i_pVirConList = i_pVirCon;

}

void deleteVirtualConnectorList(ocrVirtualConnector * i_pVirCon)
{
    ocrVirtualConnector *vc1, *vc2;

    vc1 = vc2 = i_pVirCon;
    while (vc1) {
        vc2 = vc1;
        vc1 = vc1->NEXT;
        mbkfree(vc2);
    }
}
