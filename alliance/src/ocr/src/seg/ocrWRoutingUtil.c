/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/07/16 12:03:55 $
   $Log: ocrWRoutingUtil.c,v $
   Revision 1.7  2002/07/16 12:03:55  hcl
   bug...

   Revision 1.6  2002/07/03 12:38:16  hcl
   rien

   Revision 1.5  2002/06/27 12:58:43  hcl
   Nettoyage

   Revision 1.4  2002/06/27 09:09:05  hcl
   Code d'erreur si tous les signaux ne sont pas routés.

   Revision 1.3  2002/04/25 13:41:33  hcl
   New ripup/reroute loop, bug-kill (CALU&TALU).

   Revision 1.2  2002/03/20 13:25:56  hcl
   SymX bug.

   Revision 1.1  2002/03/15 14:37:23  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:21  hcl
   First commit in the new tree.

   Revision 1.13  2002/02/25 11:41:26  hcl
   A la chasse au bug bug bug

   Revision 1.12  2002/02/12 15:14:29  hcl
   *** empty log message ***

   Revision 1.11  2002/01/30 13:12:50  hcl
   Grig initialisation.

   Revision 1.10  2002/01/14 10:34:34  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.9  2001/12/20 15:44:22  hcl
   ALU5 et ALU6 : distance min respectee.

   Revision 1.8  2001/12/20 13:04:03  hcl
   Cosmetique.

   Revision 1.7  2001/12/19 14:22:49  hcl
   Moult petits changements.

   Revision 1.6  2001/12/14 15:58:39  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.5  2001/12/10 13:09:02  hcl
   Un bigbug en moins.

   Revision 1.4  2001/12/03 16:45:23  hcl
   Corrections dans ripUp2

   Revision 1.3  2001/12/03 14:31:23  hcl
   Pour la route.

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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mut.h"
#include "mlo.h"
#include "mlu.h"
#include "mph.h"
#include "mpu.h"
#include "ocr.h"
#include "ocrUtil.h"
#include "ocrWRoutingDataBase.h"
#include "ocrWindow.h"
#include "mbk_tree.h"
#include "ocrWRoutingUtil.h"

#define LAYER_PARITY_MASK  (ocrNaturalInt)1


ocrWSegment *HEAD_OCRWSEGMENT = NULL;   /* segment buffer head */

/** 
 * Fonction de comparaison entre Entier
 * pour les fonctions sur les arbres RBTREE
**/

static int deltaCompare(void *first, void *second)
{

    return ((ocrNaturalInt) first == (ocrNaturalInt) second) ?
        0 : (((ocrNaturalInt) first > (ocrNaturalInt) second) ? 1 : -1);
}

/** 
 * Création de la Grille de Routage
 * Réservation de la place mémoire nécessaire
**/

ocrWRoutingGrid *createWGrid(ocrNaturalInt size_h,
                             ocrNaturalInt size_v,
                             ocrNaturalInt nb_of_layers)
{
    ocrWRoutingGrid *pt =
        (ocrWRoutingGrid *) mbkalloc(sizeof(ocrWRoutingGrid));

    pt->SIZE_H = size_h;
    pt->SIZE_V = size_v;

    //printf ("SIZE_H=%ld ; SIZE_V=%ld\n", size_h, size_v);
    
    pt->NB_OF_LAYERS = nb_of_layers;
    pt->DATA =
        (ocrWSegment **) mbkalloc(size_h * size_v * nb_of_layers *
                                  sizeof(ocrWSegment *));
    return pt;
}

/** 
 * initialisation de la grille complétement libre
 * de contraintes
**/
void initWGrid(ocrWRoutingGrid * i_pGrid, ocrRoutingParameters * i_pParam)
{
    ocrNaturalInt l_uLayer, i, j;
    ocrWSegment *l_pSegment;


    // création de segments libres de longueur MAX pour chaque noeud
    for (l_uLayer = 0; l_uLayer < i_pGrid->NB_OF_LAYERS; l_uLayer++)
        if (getDirection(i_pParam, l_uLayer) == ocrHorizontal)
            for (i = 0; i < i_pGrid->SIZE_V; i++) {
                l_pSegment =
                    createWSegment(i, l_uLayer, 0, i_pGrid->SIZE_H - 1,
                                   WSEGMENT_FREE);
                for (j = l_pSegment->P_MIN; j <= l_pSegment->P_MAX; j++)
                    setWGrid(i_pGrid, l_pSegment, j, i, l_uLayer);
        } else                  // vertical
            for (j = 0; j < i_pGrid->SIZE_H; j++) {
                l_pSegment =
                    createWSegment(j, l_uLayer, 0, i_pGrid->SIZE_V - 1,
                                   WSEGMENT_FREE);
                for (i = l_pSegment->P_MIN; i <= l_pSegment->P_MAX; i++)
                    setWGrid(i_pGrid, l_pSegment, j, i, l_uLayer);
            }

#if 0
        
    // contraintes NORTH ALU 2
    i = i_pGrid->SIZE_V - 1;
    l_uLayer = 0;
    for (j = 0; j < i_pGrid->SIZE_H; j++) {
        l_pSegment = createWSegment(i, l_uLayer, j, j, WSEGMENT_FREE);
        setWGrid(i_pGrid, l_pSegment, j, i, l_uLayer);
    }

     contraintes NORTH en ALU 4
    if (i_pGrid->NB_OF_LAYERS >= 3) {
        l_pSegment = getWSegment(i_pGrid, 0, 0, 2);
        l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
    }
     contraintes SOUTH en ALU 4
    if (i_pGrid->NB_OF_LAYERS >= 3) {
        l_pSegment = getWSegment(i_pGrid, 0, i_pGrid->SIZE_V - 1, 2);
        l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
    }
     contraintes EAST
    l_pSegment = getWSegment(i_pGrid, i_pGrid->SIZE_H - 1, 0, 1);
    l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;

    return; // XXX DEBUG
#endif

    // contraintes ON NE ROUTE PAS SUR LES BORDS !
    for (i = 0; i < i_pGrid->NB_OF_LAYERS; i++) {
        if (getDirection (i_pParam, i) == ocrVertical) {
            // Layer vertical
            l_pSegment = getWSegment(i_pGrid, 0, 0, i);
            l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
            l_pSegment = getWSegment(i_pGrid, i_pGrid->SIZE_H - 1, 0, i);
            l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
        } else {
            // Layer horizontal
            l_pSegment = getWSegment(i_pGrid, 0, 0, i);
            l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
            l_pSegment = getWSegment(i_pGrid, 0, i_pGrid->SIZE_V - 1, i);
            l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
        }
    }

#if 0
    /* contraintes de distances min entre ALU5-ALU5 et ALU6-ALU6 */
    if (i_pGrid->NB_OF_LAYERS > 2) {
        for (l_uLayer = 3; l_uLayer < i_pGrid->NB_OF_LAYERS; l_uLayer++)

            if (getDirection(i_pParam, l_uLayer) == ocrHorizontal)
                for (i = 0; i < i_pGrid->SIZE_V; i += 2) {
                    //if (i % 2) {
                        l_pSegment = getWSegment(i_pGrid, 0, i, l_uLayer);
                        l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
                    //}
                }
            else                  // vertical

                for (j = 0; j < i_pGrid->SIZE_H; j += 2) {
                    //if (i % 2) {
                        l_pSegment = getWSegment(i_pGrid, j, 0, l_uLayer);
                        l_pSegment->SIGNAL_INDEX = WSEGMENT_OBSTACLE;
                    //}
                }
    }
#endif

}


/**
 * Mise à jour de la grille avec les données de la phfig
 *
**/

void add_phseg_to_grid(ocrRoutingDataBase * i_pDataBase,
                       ocrWindow * i_pWindow, phfig_list * i_pPhModel,
                       phins_list * i_pPhIns, phseg_list * i_pPhSeg,
                       phfig_list * i_pPhFig)

{

    ocrNaturalInt x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    double fx1, fx2, fy1, fy2;
    ocrNaturalInt layer, mode;
    ocrNaturalInt i;
    ocrWSegment *seg;

    switch (i_pPhSeg->LAYER) {
    case TALU1:
        /* rien a faire */
        return;
    case TALU2:
        layer = 0;
        mode = 0;
        break;
    case TALU3:
        layer = 1;
        mode = 0;
        break;
    case TALU4:
        layer = 2;
        mode = 0;
        break;
    case TALU5:
        layer = 3;
        mode = 0;
        break;
    case TALU6:
        layer = 4;
        mode = 0;
        break;
    case TALU7:
        layer = 5;
        mode = 0;
        break;
    case TALU8:
        layer = 6;
        mode = 0;
        break;
    case CALU1:
        /* rien a faire */
        return;
    case CALU2:
        layer = 0;
        mode = 1;
        break;
    case CALU3:
        layer = 1;
        mode = 1;
        break;
    case CALU4:
        layer = 2;
        mode = 1;
        break;
    case CALU5:
        layer = 3;
        mode = 1;
        break;
    case CALU6:
        layer = 4;
        mode = 1;
        break;
    case CALU7:
        layer = 5;
        mode = 1;
        break;
    case CALU8:
        layer = 6;
        mode = 1;
        break;
    case ALU1:
        /* rien a faire */
        return;
    case ALU2:
        layer = 0;
        mode = 2;
        break;
    case ALU3:
        layer = 1;
        mode = 2;
        break;
    case ALU4:
        layer = 2;
        mode = 2;
        break;
    case ALU5:
        layer = 3;
        mode = 2;
        break;
    case ALU6:
        layer = 4;
        mode = 2;
        break;
    case ALU7:
        layer = 5;
        mode = 2;
        break;
    case ALU8:
        layer = 6;
        mode = 2;
        break;
    default:
        /* rien a faire */
        return;
    }

    if (!i_pPhIns) {
        x1 = i_pPhSeg->X1;
        y1 = i_pPhSeg->Y1;
        x2 = i_pPhSeg->X2;
        y2 = i_pPhSeg->Y2;

        if (
            (mode == 2)
            &&
            !((isvdd(i_pPhSeg->NAME)) || (isvss(i_pPhSeg->NAME)))
           ) {
            fprintf(stderr, "o No pre-routing accepted\n");
            exit (1);
        }
        //printf ("1/ x1=%d; y1=%d; x2=%d; y2=%d; layer=%d\n", x1/500, y1/500, x2/500, y2/500, layer);
    } else {
        switch (i_pPhIns->TRANSF) {
        case NOSYM:
#ifdef OCR_DEBUG
            printf("NOSYM\n");
#endif
            x1 = (i_pPhIns->XINS - i_pPhModel->XAB1) + i_pPhSeg->X1;
            y1 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhSeg->Y1;
            x2 = (i_pPhIns->XINS - i_pPhModel->XAB1) + i_pPhSeg->X2;
            y2 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhSeg->Y2;
                                                   
            break;                                 
        case SYM_Y:                                
#ifdef OCR_DEBUG                                   
            printf("SYM_Y\n");                     
#endif                                             
            x1 = (i_pPhIns->XINS - i_pPhModel->XAB1) + i_pPhSeg->X1;
            y1 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhModel->YAB2 - i_pPhSeg->Y1;
            x2 = (i_pPhIns->XINS - i_pPhModel->XAB1) + i_pPhSeg->X2;
            y2 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhModel->YAB2 - i_pPhSeg->Y2;
            break;                                 
        case SYM_X:                                
#ifdef OCR_DEBUG                                   
            printf("SYM_X\n");                     
#endif                                             
            x1 = (i_pPhIns->XINS) + i_pPhModel->XAB2 - i_pPhSeg->X1;
            y1 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhSeg->Y1;
            x2 = (i_pPhIns->XINS) + i_pPhModel->XAB2 - i_pPhSeg->X2;
            y2 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhSeg->Y2;
            break;                                 
        case SYMXY:                                
#ifdef OCR_DEBUG                                   
            printf("SYMXY\n");                     
            printf ("XINS = %ld; XAB1 = %ld; XAB2 = %ld; X1 = %ld --> x1 = %ld\n", i_pPhIns->XINS, i_pPhModel->XAB1, i_pPhModel->XAB2, i_pPhSeg->X1, x1);
#endif                                             
            x1 = i_pPhIns->XINS + i_pPhModel->XAB2 - i_pPhSeg->X1;
            y1 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhModel->YAB2 - i_pPhSeg->Y1;
            x2 = i_pPhIns->XINS + i_pPhModel->XAB2 - i_pPhSeg->X2;
            y2 = (i_pPhIns->YINS - i_pPhModel->YAB1) + i_pPhModel->YAB2 - i_pPhSeg->Y2;
            break;
        default:
            fprintf(stderr, "o Error add_phseg_to_grid");
            exit(1);
        }
    }
#ifdef OCR_DEBUG
    printf("1/ x1=%d; y1=%d; x2=%d; y2=%d; layer=%d\n", x1, y1, x2, y2,
           layer);
#endif
    switch (i_pPhSeg->TYPE) {
    case LEFT:
    case RIGHT:
        fy1 = y1 - (i_pPhSeg->WIDTH / 2.0);
        fy2 = y2 + (i_pPhSeg->WIDTH / 2.0);
        fx1 = x1;
        fx2 = x2;
        break;
    case DOWN:
    case UP:
        fx1 = x1 - (i_pPhSeg->WIDTH / 2.0);
        fx2 = x2 + (i_pPhSeg->WIDTH / 2.0);
        fy1 = y1;
        fy2 = y2;
        break;
    default:
        exit(1);
        break;
    }

    // conversion des coord
#ifdef OCR_DEBUG
    printf("2/ x1=%d; y1=%d; x2=%d; y2=%d\n", x1, y1, x2, y2);
#endif

    fx1 = 0.5 + fx1 / (5.0 * SCALE_X);
    fx2 = 0.5 + fx2 / (5.0 * SCALE_X);
    fy1 = 0.5 + fy1 / (5.0 * SCALE_X);
    fy2 = 0.5 + fy2 / (5.0 * SCALE_X);

    x1 = fx1;
    x2 = fx2;
    y1 = fy1;
    y2 = fy2;


    // controler par rapport a la fenetre
#ifdef OCR_DEBUG
    printf("3/ X1=%ld; Y1=%ld; X2=%ld; Y2=%ld\n", x1, y1, x2, y2);
    printf("WIN x1=%d; y1=%d; x2=%d; y2=%d\n", i_pWindow->XMIN,
           i_pWindow->YMIN, i_pWindow->XMAX, i_pWindow->YMAX);
#endif


    // Segment entierement en dehors de la fenetre ?
    if ((x1 < i_pWindow->XMIN) && (x2 < i_pWindow->XMIN))
        return;
    if ((x1 > i_pWindow->XMAX) && (x2 > i_pWindow->XMAX))
        return;
    if ((y1 > i_pWindow->YMAX) && (y2 > i_pWindow->YMAX))
        return;
    if ((y1 > i_pWindow->YMAX) && (y2 > i_pWindow->YMAX))
        return;

    // Segment partiellement en dehors
    if (x1 < i_pWindow->XMIN)
        x1 = i_pWindow->XMIN;
    if (x2 > i_pWindow->XMAX)
        x2 = i_pWindow->XMAX;
    if (y1 < i_pWindow->YMIN)
        y1 = i_pWindow->YMIN;
    if (y2 > i_pWindow->YMAX)
        y2 = i_pWindow->YMAX;



#ifdef OCR_DEBUG
    printf("3/ X1=%d; Y1=%d; X2=%d; Y2=%d\n", x1, y1, x2, y2);
#endif

    if (!
        ((((layer % 2))
          && (i_pDataBase->PARAM->EVEN_LAYERS_DIRECTION == ocrHorizontal))
         ||
         ((!(layer % 2)
           && (i_pDataBase->PARAM->EVEN_LAYERS_DIRECTION == ocrVertical)))))
    {
        ocrNaturalInt j;
        // Layer Horizontal

        for (i = y1; i <= y2; i++) {
#ifdef OCR_DEBUG
            printf("Adding horiz X=%d; Y1=%d; layer=%d\n", x1, i, layer);
#endif

            if (mode) { // CALU
                seg = getWSegment(i_pDataBase->GRID, x1, i, layer);
                seg = splitWSegment
                    (
                     i_pDataBase->PARAM,
                     i_pDataBase->GRID,
                     seg, x1, x2,
                     WSEGMENT_OBSTACLE
                    )
                    ;
            } else { // TALU

                for (j = x1; j <= x2; j++) {
                    seg = getWSegment(i_pDataBase->GRID, j, i, layer);
                    if (seg->SIGNAL_INDEX == WSEGMENT_FREE)
                        seg = splitWSegment
                            (
                             i_pDataBase->PARAM,
                             i_pDataBase->GRID,
                             seg, j, j,
                             WSEGMENT_OBSTACLE
                            )
                           ;
                }
            }

            //if (mode) {
            // CALU
            //      setWSegmentName (seg, i_pPhSeg->NAME);
            //}
        }


    } else {
        ocrNaturalInt j;
        // Layer Vertical
        for (i = x1; i <= x2; i++) {
#ifdef OCR_DEBUG
            printf("Adding vert X=%d; Y1=%d; layer=%d\n", i, y1, layer);
#endif
            for (j = y1; j <= y2; j++) {
                seg = getWSegment(i_pDataBase->GRID, i, j, layer);
                if (seg->SIGNAL_INDEX == WSEGMENT_FREE)
                    seg = splitWSegment
                        (
                         i_pDataBase->PARAM,
                         i_pDataBase->GRID,
                         seg, j, j,
                         WSEGMENT_OBSTACLE
                        )
                        ;
            }
            //if (mode) {
            // CALU
            //      setWSegmentName (seg, i_pPhSeg->NAME);
            //}
        }
    }

    return;

}


void
updateWGrid(ocrWindow * i_pWindow, ocrRoutingDataBase * i_pDataBase,
            phfig_list * i_pPhFig /*, lofig_list * i_pLoFig */ )
{
    phins_list *l_pPhInst = NULL;
    phfig_list *l_pPhModel = NULL;
    phseg_list *l_pPhSeg = NULL;

    /* parcours des segments dans la phfig */
    for (l_pPhSeg = i_pPhFig->PHSEG; l_pPhSeg; l_pPhSeg = l_pPhSeg->NEXT) {
#ifdef OCR_DEBUG
        printf("Treating segment of phfig\n");
#endif

        add_phseg_to_grid(i_pDataBase, i_pWindow, l_pPhModel, NULL,
                          l_pPhSeg, i_pPhFig);

    }
    /* Parcours des instances physiques */
    for (l_pPhInst = i_pPhFig->PHINS; l_pPhInst;
         l_pPhInst = l_pPhInst->NEXT) {

        /* recup du modele */

        l_pPhModel = getphfig(l_pPhInst->FIGNAME, 'X'); // FIXME

        for (l_pPhSeg = l_pPhModel->PHSEG; l_pPhSeg;
             l_pPhSeg = l_pPhSeg->NEXT) {

            add_phseg_to_grid(i_pDataBase, i_pWindow, l_pPhModel,
                              l_pPhInst, l_pPhSeg, i_pPhFig);

        }

    }

    return;
}


void dumpWGrid(char *output, ocrRoutingDataBase * i_pDataBase,
               ocrWindow * win)
{
    char *name = (char *) mbkalloc((strlen(output) + 6) * sizeof(char *));
    FILE *dump;
    int x, y, z;

    ocrWRoutingGrid *grid = i_pDataBase->GRID;
    ocrWSegment *seg, *prev;


    name = strdup(output);
    strcat(name, ".gpl");
    dump = fopen(name, "w");

    fprintf(dump, "set noxtics\nset noytics\nset noborder\n");
    fprintf(dump, "set nokey\nset title '%s'\n", "toto");
    fprintf(dump, "#set terminal postscript eps color solid\n");
    fprintf(dump, "#set output '%s.ps'", output);
    fprintf(dump, "set xrange[%ld:%ld]\n", win->XMIN, win->XMAX);
    fprintf(dump, "set yrange[%ld:%ld]\n", win->YMIN, win->YMAX);
    fprintf(dump,
            "plot [:][:][:][:] '-' w l, '-' w l 2, '-' w l 3, '-' w l 4\n");
    fprintf(dump, "%ld %ld\n", win->XMIN, win->YMIN);
    fprintf(dump, "%ld %ld\n", win->XMAX, win->YMIN);
    fprintf(dump, "%ld %ld\n", win->XMAX, win->YMAX);
    fprintf(dump, "%ld %ld\n", win->XMIN, win->YMAX);
    fprintf(dump, "%ld %ld\n", win->XMIN, win->YMIN);
    fprintf(dump, "\n");

    prev = seg = NULL;
    for (z = 0; z < grid->NB_OF_LAYERS - 1; z++) {
        fprintf(dump, "#Layer %i\n", z);
        for (x = 0; x < grid->SIZE_H; x++)
            for (y = 0; y < grid->SIZE_V; y++) {
                prev = seg;
                seg = getWSegment(grid, x, y, z);
                if (seg == prev)
                    continue;
                if (seg->SIGNAL_INDEX == WSEGMENT_OBSTACLE) {
                    if (getWSegDirection(i_pDataBase->PARAM, seg) !=
                        ocrHorizontal) {
                        fprintf(dump, "%ld %ld\n", seg->OFFSET,
                                seg->P_MIN);
                        fprintf(dump, "%ld %ld\n", seg->OFFSET,
                                seg->P_MAX);
                        fprintf(dump, "\n");
                    } else {
                        fprintf(dump, "%ld %ld\n",
                                seg->P_MIN, seg->OFFSET);
                        fprintf(dump, "%ld %ld\n",
                                seg->P_MAX, seg->OFFSET);
                        fprintf(dump, "\n");
                    }
                }
            }
        fprintf(dump, "EOF\n");
    }
    fprintf(dump, "\n");
    //fprintf (dump, "pause -1 'Press any key'\n");



    fclose(dump);
}

#if 0
void dumpSigs (char *output, ocrRoutingDataBase *i_pDataBase) {
      char *name = (char *) malloc ((strlen (output) + 5) * sizeof(char *));
      FILE *dump;
      int x, y, z;

      ocrWRoutingGrid *grid = i_pDataBase->GRID;
      ocrWSegment *seg;
      

      name = strdup(output);
      strcat(name, ".gpl");
      dump = fopen (name, "w");

      fprintf (dump, "set noxtics\nset noytics\nset noborder\n");
      fprintf (dump, "set nokey\nset title '%s'\n", "toto");
      fprintf (dump, "#set terminal postscript eps color solid\n");
      fprintf (dump, "#set output '%s.ps'", output);
        fprintf (dump, "set xrange[%d:%d]\n", win->XMIN, win->XMAX);
        fprintf (dump, "set yrange[%d:%d]\n", win->YMIN, win->YMAX);
      fprintf (dump, "plot [:][:][:] '-' w l, '-' w l 2, '-' w l 3\n");
        fprintf (dump, "%d %d\n", win->XMIN, win->YMIN);
        fprintf (dump, "%d %d\n", win->XMAX, win->YMIN);
        fprintf (dump, "%d %d\n", win->XMAX, win->YMAX);
        fprintf (dump, "%d %d\n", win->XMIN, win->YMAX);
        fprintf (dump, "%d %d\n", win->XMIN, win->YMIN);
      fprintf (dump, "\n");

      for (x = win->XMIN ; x < win->XMAX ; x++)
              for (y = win->YMIN ; y < win->YMAX ; y++)
                      for (z = 0 ; z < 6 ; z++) {
                              seg = grid->DATA[z +
                                       y * grid->NB_OF_LAYERS +
                                       x * grid->SIZE_V * grid->NB_OF_LAYERS];
                              if (seg->SIGNAL_INDEX == WSEGMENT_OBSTACLE) {
                                      if ( ((!(z % 2)) && (i_pDataBase->PARAM->EVEN_LAYERS_DIRECTION == ocrHorizontal))
                                      || (((z % 3) && (i_pDataBase->PARAM->EVEN_LAYERS_DIRECTION == ocrVertical))) )
                                      {
                                              fprintf (dump, "%d %d\n",
                                                      seg->P_MIN,
                                                      seg->OFFSET
                                                      );
                                              fprintf (dump, "%d %d\n",
                                                      seg->P_MAX,
                                                      seg->OFFSET
                                                      );
                                      } else {
                                              fprintf (dump, "%d %d\n",
                                                      seg->OFFSET,
                                                      seg->P_MIN
                                                      );
                                              fprintf (dump, "%d %d\n",
                                                      seg->OFFSET,
                                                      seg->P_MAX
                                                      );
                                      }
                              }
                              fprintf (dump, "\n");
      }
      
      

      fclose (dump);
}
#endif


/**
 * Destruction de la Grille de Routage 
**/

void freeWGrid(ocrWRoutingGrid * i_pGrid)
{
/*
   ocrNaturalInt l_uLayer, i, j;
   ocrWSegment *l_pSegment;

   // destruction des segments.
   for (l_uLayer = 0; l_uLayer < i_pGrid->NB_OF_LAYERS; l_uLayer++)
   for (i = 0; i < i_pGrid->SIZE_V; i++)
   for (j = 0; j < i_pGrid->SIZE_H; j++)
   {
   l_pSegment = getWSegment (i_pGrid, i, j, l_uLayer);
   if (l_pSegment)
   {
   freeWSegment (l_pSegment);
   setWGrid (i_pGrid, NULL, i, j, l_uLayer);
   }
   }
 */
    mbkfree(i_pGrid->DATA);
    mbkfree(i_pGrid);
}

/** 
 * Création d'un segment pour un noeud de la Grille de Routage
**/

ocrWSegment *createWSegment(ocrNaturalInt offset,
                            ocrNaturalInt layer,
                            ocrNaturalInt p_min,
                            ocrNaturalInt p_max,
                            ocrNaturalInt signal_index)
{
    ocrWSegment *pt;
    register int i;

    if (HEAD_OCRWSEGMENT == NULL) {
        pt = (ocrWSegment *) mbkalloc(BUFSIZE * sizeof(ocrWSegment));
        HEAD_OCRWSEGMENT = pt;
        for (i = 1; i < BUFSIZE; i++) {
            (ocrWSegment *) pt->LABELS_LEFT_TOP = pt + 1;
            pt++;
        }
        (ocrWSegment *) pt->LABELS_LEFT_TOP = NULL;
    }

    pt = HEAD_OCRWSEGMENT;
    HEAD_OCRWSEGMENT = (ocrWSegment *) HEAD_OCRWSEGMENT->LABELS_LEFT_TOP;
    pt->OFFSET = offset;
    pt->LAYER = layer;
    pt->P_MIN = p_min;
    pt->P_MAX = p_max;
    pt->TAG = 0;
    pt->RB = 0;
    pt->LT = 0;
    pt->SIGNAL_INDEX = signal_index;
    pt->LABELS_LEFT_TOP = RBTREENULL;
    pt->LABELS_RIGHT_BOTTOM = RBTREENULL;

    // obstacle list
    //pt->OBSTACLE = NULL;

    //pt->AUX = NULL;
    pt->NEXT = NULL;
    pt->ROOT = NULL;

    return pt;
}

/** 
 * Supprime un segment
**/

void freeWSegment(ocrWSegment * segment)
{

    (ocrWSegment *) segment->LABELS_LEFT_TOP = HEAD_OCRWSEGMENT;
    HEAD_OCRWSEGMENT = segment;
}

/** 
 * Retourne le segment de la grille aux coord (x,y,z) 
**/

ocrWSegment *getWSegment(ocrWRoutingGrid * grid,
                         ocrNaturalInt x, ocrNaturalInt y, ocrNaturalInt z)
{
    //if (z == 0)
    return (grid->DATA[z +
                       y * grid->NB_OF_LAYERS +
                       x * grid->SIZE_V * grid->NB_OF_LAYERS]);
    //else
    //      return (grid->DATA[(z-1) +
    //                      y * grid->NB_OF_LAYERS +
    //                      x * grid->SIZE_V * grid->NB_OF_LAYERS]);
}

ocrWSegment *getWSegmentCV(ocrWRoutingGrid * grid,
                           ocrVirtualConnector * i_pVirCon)
{

    return (grid->DATA[i_pVirCon->Z +
                       i_pVirCon->Y * grid->NB_OF_LAYERS +
                       i_pVirCon->X * grid->SIZE_V * grid->NB_OF_LAYERS]);
}


/** 
 * Mise à jour de la Grille de Routage
 * Association d'un segment a un noeud de la Grille
**/

void
setWGrid(ocrWRoutingGrid * grid,
         ocrWSegment * segment,
         ocrNaturalInt x, ocrNaturalInt y, ocrNaturalInt z)
{
    grid->DATA[z +
               y * grid->NB_OF_LAYERS +
               x * grid->SIZE_V * grid->NB_OF_LAYERS] = segment;
}

/** **/

int isLabeled(ocrWSegment * segment)
{

    return !((segment->LABELS_LEFT_TOP == RBTREENULL) &&
             (segment->LABELS_RIGHT_BOTTOM == RBTREENULL));
}

/** 
 * Est-ce que le segment est occupé
**/

int isObstructed(ocrWSegment * segment)
{

    return (segment->SIGNAL_INDEX != WSEGMENT_FREE);
}

/** 
 * Renvoie la direction du segment 
**/

ocrRoutingDirection
getWSegDirection(ocrRoutingParameters * i_param, ocrWSegment * i_segment)
{
    //if (i_segment == NULL)
    //    puts("ERROR getWSegDirection");
    //fflush (stdout);

    return ((i_segment->LAYER & LAYER_PARITY_MASK) ?
            (ocrRoutingDirection) (!(i_param->EVEN_LAYERS_DIRECTION))
            : i_param->EVEN_LAYERS_DIRECTION);
}

/** 
 * Renvoie la coordonnée X du segment 
**/

ocrNaturalInt
getWSegXCoord(ocrRoutingParameters * i_param,
              ocrWSegment * i_segment, ocrNaturalInt i_point)
{
    return ((getWSegDirection(i_param, i_segment) == ocrHorizontal) ?
            i_point : i_segment->OFFSET);
}

/** 
 * Renvoie la coordonnée Y du segment 
**/

ocrNaturalInt
getWSegYCoord(ocrRoutingParameters * i_param,
              ocrWSegment * i_segment, ocrNaturalInt i_point)
{
// Y dépend de la direction du LAYER
    return ((getWSegDirection(i_param, i_segment) == ocrHorizontal) ?
            i_segment->OFFSET : i_point);
}



/**
* Fusion de segment
* */
void
merge_min(ocrRoutingParameters * param, ocrWRoutingGrid * grid,
          ocrWSegment * segment)
{
    ocrNaturalInt i;
    ocrWSegment *seg1;

    if ((segment->P_MIN) > 0) {
        if (getWSegDirection(param, segment) == ocrHorizontal) {
            seg1 =
                getWSegment(grid, segment->P_MIN - 1, segment->OFFSET,
                            segment->LAYER);
            if (seg1->SIGNAL_INDEX != segment->SIGNAL_INDEX)
                return;
            segment->P_MIN = seg1->P_MIN;
            for (i = segment->P_MIN; i <= seg1->P_MAX; i++)
                setWGrid(grid, segment, i, segment->OFFSET,
                         segment->LAYER);
        } else {
            seg1 =
                getWSegment(grid, segment->OFFSET, segment->P_MIN - 1,
                            segment->LAYER);
            if (seg1->SIGNAL_INDEX != segment->SIGNAL_INDEX)
                return;
            segment->P_MIN = seg1->P_MIN;
            for (i = segment->P_MIN; i <= seg1->P_MAX; i++)
                setWGrid(grid, segment, segment->OFFSET, i,
                         segment->LAYER);
        }
        //freeWSegment (seg1);
        //mbkfree (seg1);

        //merge_min (param, grid, segment);
    }


    return;
}

void
merge_max(ocrRoutingParameters * param, ocrWRoutingGrid * grid,
          ocrWSegment * segment)
{
    ocrNaturalInt i;
    ocrWSegment *seg1;

    if ((getWSegDirection(param, segment) == ocrHorizontal)
        && (segment->P_MAX < (grid->SIZE_H - 1))) {
        seg1 =
            getWSegment(grid, segment->P_MAX + 1, segment->OFFSET,
                        segment->LAYER);
        if (seg1->SIGNAL_INDEX != segment->SIGNAL_INDEX)
            return;
        for (i = segment->P_MAX; i <= seg1->P_MAX; i++)
            setWGrid(grid, segment, i, segment->OFFSET, segment->LAYER);
        segment->P_MAX = seg1->P_MAX;
        //freeWSegment (seg1);
        merge_max(param, grid, segment);
    } else if ((getWSegDirection(param, segment) == ocrVertical)
               && (segment->P_MAX < grid->SIZE_V - 1)) {
        seg1 =
            getWSegment(grid, segment->OFFSET, segment->P_MAX + 1,
                        segment->LAYER);
        if (seg1->SIGNAL_INDEX != segment->SIGNAL_INDEX)
            return;
        for (i = segment->P_MAX; i <= seg1->P_MAX; i++)
            setWGrid(grid, segment, segment->OFFSET, i, segment->LAYER);
        segment->P_MAX = seg1->P_MAX;
        //freeWSegment (seg1);
        //merge_max (param, grid, segment);
    }

    return;
}

void
mergeWSegment(ocrRoutingParameters * param,
              ocrWRoutingGrid * grid, ocrWSegment * segment)
{
    merge_min(param, grid, segment);
    merge_max(param, grid, segment);
    return;
}

/**
 * drc_update_grid : apres creation d'un segment dans la grille,
 * on met en place des segments obstacles pour que le routage respecte
 * la techno (les regles de dessin).
**/
void drc_update_grid (ocrRoutingDataBase *i_pDataBase, ocrWSegment *segment) {
    int first_point, second_point;

    /* empty */
    return;


    if (segment->LAYER >= 3) // >= ALU5
    {
       /* adding obstacles at each extremities of the segment */
        splitWSegment(i_pDataBase->PARAM, i_pDataBase->GRID, segment,
               first_point, second_point,
               WSEGMENT_OBSTACLE);
        splitWSegment(i_pDataBase->PARAM, i_pDataBase->GRID, segment,
               first_point, second_point,
               WSEGMENT_OBSTACLE);
    }



    return;
}


/** 
 * Création d'un segment physique représentant le signal index
 * entre first_point et second_point dans le LAYER considéré 
 * (ici segment->LAYER)
 *
 * et mise à jour de la Grille car
 * Les noeuds alignés avec ce nouveau segment sont tronqués
 *
 * Il est supposé que :
 * first_point < second_point
**/

ocrWSegment *splitWSegment(ocrRoutingParameters * param,
                           ocrWRoutingGrid * grid,
                           ocrWSegment * segment,
                           ocrNaturalInt first_point,
                           ocrNaturalInt second_point, ocrNaturalInt index)
{
    ocrNaturalInt seg1_length = first_point - segment->P_MIN;
    ocrNaturalInt seg2_length = second_point - first_point + 1;
    ocrNaturalInt seg3_length = segment->P_MAX - second_point;
    ocrWSegment *new_segment1;
    ocrWSegment *new_segment2;
    ocrNaturalInt i;

    if (seg1_length > seg3_length) {
        if (seg1_length > seg2_length) {
            new_segment1 = createWSegment(segment->OFFSET,
                                          segment->LAYER,
                                          first_point, second_point,
                                          index);
            if (getWSegDirection(param, new_segment1) == ocrHorizontal)
                for (i = first_point; i <= second_point; i++)
                    setWGrid(grid, new_segment1, i, new_segment1->OFFSET,
                             new_segment1->LAYER);
            else
                for (i = first_point; i <= second_point; i++)
                    setWGrid(grid, new_segment1, new_segment1->OFFSET, i,
                             new_segment1->LAYER);
            if (seg3_length) {
                new_segment2 = createWSegment(segment->OFFSET,
                                              segment->LAYER,
                                              second_point + 1,
                                              segment->P_MAX,
                                              segment->SIGNAL_INDEX);
                if (getWSegDirection(param, new_segment2) == ocrHorizontal)
                    for (i = second_point + 1; i <= segment->P_MAX; i++)
                        setWGrid(grid, new_segment2, i,
                                 new_segment2->OFFSET,
                                 new_segment2->LAYER);
                else
                    for (i = second_point + 1; i <= segment->P_MAX; i++)
                        setWGrid(grid, new_segment2, new_segment2->OFFSET,
                                 i, new_segment2->LAYER);
            }
            segment->P_MAX = first_point - 1;
            return new_segment1;
        }
    } else {
        if (seg3_length > seg2_length) {        //here

            new_segment1 = createWSegment(segment->OFFSET,
                                          segment->LAYER,
                                          first_point, second_point,
                                          index);
            if (getWSegDirection(param, new_segment1) == ocrHorizontal)
                for (i = first_point; i <= second_point; i++)
                    setWGrid(grid, new_segment1, i, new_segment1->OFFSET,
                             new_segment1->LAYER);
            else
                for (i = first_point; i <= second_point; i++)
                    setWGrid(grid, new_segment1, new_segment1->OFFSET, i,
                             new_segment1->LAYER);
            if (seg1_length) {
                new_segment2 = createWSegment(segment->OFFSET,
                                              segment->LAYER,
                                              segment->P_MIN,
                                              first_point - 1,
                                              segment->SIGNAL_INDEX);
                if (getWSegDirection(param, new_segment2) == ocrHorizontal)
                    for (i = segment->P_MIN; i <= first_point - 1; i++)
                        setWGrid(grid, new_segment2, i,
                                 new_segment2->OFFSET,
                                 new_segment2->LAYER);
                else
                    for (i = segment->P_MIN; i <= first_point - 1; i++)
                        setWGrid(grid, new_segment2, new_segment2->OFFSET,
                                 i, new_segment2->LAYER);
            }
            segment->P_MIN = second_point + 1;
            return new_segment1;
        }
    }
    if (seg1_length) {
        new_segment1 = createWSegment(segment->OFFSET,
                                      segment->LAYER,
                                      segment->P_MIN,
                                      first_point - 1,
                                      segment->SIGNAL_INDEX);
        if (getWSegDirection(param, new_segment1) == ocrHorizontal)
            for (i = segment->P_MIN; i <= first_point - 1; i++)
                setWGrid(grid, new_segment1, i, new_segment1->OFFSET,
                         new_segment1->LAYER);
        else
            for (i = segment->P_MIN; i <= first_point - 1; i++)
                setWGrid(grid, new_segment1, new_segment1->OFFSET, i,
                         new_segment1->LAYER);
    }
    if (seg3_length) {
        new_segment2 = createWSegment(segment->OFFSET,
                                      segment->LAYER,
                                      second_point + 1,
                                      segment->P_MAX,
                                      segment->SIGNAL_INDEX);
        if (getWSegDirection(param, new_segment2) == ocrHorizontal)
            for (i = second_point + 1; i <= segment->P_MAX; i++)
                setWGrid(grid, new_segment2, i, new_segment2->OFFSET,
                         new_segment2->LAYER);
        else
            for (i = second_point + 1; i <= segment->P_MAX; i++)
                setWGrid(grid, new_segment2, new_segment2->OFFSET, i,
                         new_segment2->LAYER);
    }
    segment->P_MIN = first_point;
    segment->P_MAX = second_point;
    segment->SIGNAL_INDEX = index;

    /* Post traitement pour respecter les regles de dessin */
    drc_update_grid (grid, segment);

    
    return segment;
}

/** get L1 distance (manhattan distance) in a segment **/

ocrNaturalInt
manhattan1Distance(ocrRoutingParameters * param,
                   ocrWSegment * segment, ocrNaturalInt p1,
                   ocrNaturalInt p2)
{

    return ((getWSegDirection(param, segment) == ocrHorizontal) ?
            param->PITCH_H * ABSDIFF(p1, p2)
            : param->PITCH_V * ABSDIFF(p1, p2));
}

/** 
 * Get induced delta in a segment 
**/

ocrNaturalInt
inducedDelta(ocrRoutingParameters * param,
             ocrWSegment * segment,
             ocrNaturalInt xtarget,
             ocrNaturalInt ytarget, rbtree * node, ocrNaturalInt point)
{
//printf("-> noeud %ld point = %ld point(node) = %ld\n",
    //   DELTA (node),point,POINT (node));

    return (DELTA(node) +
            manhattan1Distance(param,
                               segment,
                               POINT(node),
                               point) -
            manhattan2Distance(param,
                               getWSegXCoord(param, segment, POINT(node)),
                               getWSegYCoord(param, segment, POINT(node)),
                               xtarget,
                               ytarget) +
            manhattan2Distance(param,
                               getWSegXCoord(param, segment, point),
                               getWSegYCoord(param, segment, point),
                               xtarget, ytarget));
}

/** **/

rbtree *getLabel(ocrRoutingParameters * param,
                 ocrNaturalInt xtarget,
                 ocrNaturalInt ytarget,
                 ocrWSegment * segment, ocrNaturalInt delta,
                 ocrNaturalInt point)
{
    rbtree *label_LT;
    rbtree *label_RB;
    rbtree *previous;
    rbtree *next;
    ocrNaturalInt coord_target =
        (getWSegDirection(param, segment) ==
         ocrHorizontal) ? xtarget : ytarget;

    if (point <= coord_target) {
        label_LT = getrbtree2(segment->LABELS_LEFT_TOP,
                              (void *) delta, &previous, &next,
                              deltaCompare);

        if (previous == RBTREENULL) {
            if (label_LT == RBTREENULL) {
                return RBTREENULL;
            } else {
                if (inducedDelta(param,
                                 segment,
                                 xtarget, ytarget, label_LT,
                                 point) != delta)
                    return RBTREENULL;
            }
        } else {
            if (label_LT == RBTREENULL) {
                if (inducedDelta(param,
                                 segment,
                                 xtarget, ytarget, previous,
                                 point) != delta)
                    return RBTREENULL;
                label_LT = previous;
            } else {
                if ((inducedDelta(param,
                                  segment,
                                  xtarget,
                                  ytarget,
                                  label_LT,
                                  point) != delta) ||
                    (inducedDelta(param,
                                  segment,
                                  xtarget, ytarget, previous,
                                  point) < delta))
                    return RBTREENULL;
                return label_LT;
            }
        }

        if ((POINT(label_LT) == coord_target) &&
            ((label_RB = getmostrbtree(segment->LABELS_RIGHT_BOTTOM)) !=
             RBTREENULL) && (DELTA(label_RB) == DELTA(label_LT))) {

            return label_RB;
        } else {
            return label_LT;
        }

    } else {
        label_RB = getrbtree2(segment->LABELS_RIGHT_BOTTOM,
                              (void *) delta, &previous, &next,
                              deltaCompare);

        if (previous == RBTREENULL) {
            if (label_RB == RBTREENULL) {
                return RBTREENULL;
            } else {
                if (inducedDelta(param,
                                 segment,
                                 xtarget, ytarget, label_RB,
                                 point) != delta)
                    return RBTREENULL;
            }
        } else {
            if (label_RB == RBTREENULL) {

                if (inducedDelta(param,
                                 segment,
                                 xtarget, ytarget, previous,
                                 point) != delta)
                    return RBTREENULL;
                label_RB = previous;
            } else {
                if ((inducedDelta(param,
                                  segment,
                                  xtarget,
                                  ytarget,
                                  label_RB,
                                  point) != delta) ||
                    (inducedDelta(param,
                                  segment,
                                  xtarget, ytarget, previous,
                                  point) < delta))
                    return RBTREENULL;
                return label_RB;
            }
        }

        if ((POINT(label_RB) == coord_target + 1) &&
            ((label_LT = getmostrbtree(segment->LABELS_LEFT_TOP)) !=
             RBTREENULL) && (DELTA(label_LT) < DELTA(label_RB)))
            return label_LT;
        else
            return label_RB;

    }
}

/** **/

ocrNaturalInt
getNextPlane(ocrWRoutingGrid * grid,
             ocrNaturalInt ztarget, ocrNaturalInt plane)
{
    ocrInt symplane;

    if (plane == ztarget)
        // retourne le plus proche entre 0 et NB_OF_LAYERS - 1
        return ((ABSDIFF(grid->NB_OF_LAYERS - 1, ztarget) >
                 ABSDIFF(0, ztarget))
                ? grid->NB_OF_LAYERS - 1 : 0);

    symplane = (plane > ztarget) ? ztarget - (ABSDIFF(plane, ztarget) - 1)
        : ztarget + ABSDIFF(plane, ztarget);
    if (symplane < 0)
        return (plane - 1);
    else {
        if (symplane >= grid->NB_OF_LAYERS)
            return (plane + 1);
        else
            return symplane;
    }
}
