/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:27 $
   $Log: ocrInitDataBase.c,v $
   Revision 1.1  2002/03/15 14:37:27  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:25  hcl
   First commit in the new tree.

   Revision 1.4  2001/12/20 13:04:04  hcl
   Cosmetique.

   Revision 1.3  2001/12/14 15:58:40  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

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

#include "ocrConnectorUtil.h"
#include "ocrSignalUtil.h"
#include "ocrInitDataBase.h"

static char *res_id =
    "$Id: ocrInitDataBase.c,v 1.1 2002/03/15 14:37:27 hcl Exp $";

extern ocrOption *g_pOption;
#define LEVEL (g_pOption->LEVEL)

/**
 * Initialisation de la base de donnée
**/
ocrRoutingDataBase *initDataBase(phfig_list * i_pPhFig,
                                 lofig_list * i_pLoFig)
{
    ocrRoutingDataBase *l_pDataBase;

    // Création de la base de donnée
    l_pDataBase = createDataBase(i_pPhFig);

    // Création d'une vue duale de la connectique
    lofigchain(i_pLoFig);
    display(LEVEL, DEBUG, "%s\n", "o Dual connectique ...");

    // paramtères physiques de la grille 5*5 viacost=2 Layer paires = HORZ
    l_pDataBase->PARAM = setParameters(5, 5, 2, ocrHorizontal);

    // Initialisation des signaux a router
    initGlobalSignalList(l_pDataBase, i_pLoFig);

    // Création des listes de connecteurs virtuels
    initConnectorList(l_pDataBase, i_pPhFig, i_pLoFig);

//  dumpDataBase (l_pDataBase, stdout);
    return l_pDataBase;
}
