/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ring router                                                 */
/*    Fichier : compress.c                                                  */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*-----------------------------------------------------------------------------------*/
/* RING  24 juillet 92              COMPRESS.C                                       */
/* compression des canaux libres + mise a jour des coordonnees de tous les segments  */
/*-----------------------------------------------------------------------------------*/

#include "compress.h"

#include <stdio.h>

/*----------------------------------------------------------------------------------*/
/* cette procedure retourne la 1ere et la derniere instance du mode physique        */
/* selon la face                                                                    */
/*----------------------------------------------------------------------------------*/

void firstlastinst_barre(phfig_list *barre, phins_list **firstinst, phins_list **lastinst)
{

	phins_list * lst = barre->PHINS;
	long	min, max;

	max = min = lst->XINS; 
	*firstinst = *lastinst = lst; 
	lst = lst->NEXT;

	while (NULL != lst) {
		if (lst->XINS < min) {
			min = lst->XINS; 
			*firstinst = lst;
		}
		if (lst->XINS > max) {
			max = lst->XINS; 
			*lastinst = lst;
		}
		lst = lst->NEXT;
	}
	if (mode_debug) 
		printf("firstinst %s x %ld lastinst %s x %ld\n", (*firstinst)->INSNAME, (*firstinst)->XINS, (*lastinst)->INSNAME,
		     (*lastinst)->XINS);

}

/*------------------------------------------------------------------------------------*/
/* Cette procedure calcul le nombre de pistes libres pour chaque canal de chaque face */
/* et range dans:                                                                     */
/* - nblibres: le nombre total de piste libres entre la derniere piste occupee par    */
/*             le routage et les pistes d'alim.                                       */
/* - firstlibre: le numero de la 1ere piste libre dans la face concernee              */
/*------------------------------------------------------------------------------------*/

void calcul_nbpiste_libre(BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur, 
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long nblibres[NB_FACES], long firstlibre[NB_FACES])
{
	PT_COORDONNEES debut, fin;
	long	piste, pistelibre, maxpiste = 0;
	char	niveaupiste = 0;
	LST_SEGMENT    * segpiste = NULL;
	int	face;

	alloue_coord(0L, 0, &debut);
	alloue_coord(0L, 0, &fin);

	debut->xabs = lecoeur.coord.xabs - 10;
	debut->yabs = lecoeur.coord.yabs - 10;
	fin->xabs   = lecoeur.coord.xabs + lecoeur.width + 10;
	fin->yabs   = lecoeur.coord.yabs + lecoeur.height + 10;

	for (face = 0; face < NB_FACES; face++)
		if ((0 != tab_plots[face].width) && (tab_plots[face].coord.piste > maxpiste))
			maxpiste = tab_plots[face].coord.piste;

	for (face = 0; face < NB_FACES; face++) { 
		firstlibre[face] = maxpiste; 
		nblibres[face] = 0;
	}

	for (face = 0; face < NB_FACES; face++) {
		switch (face) {
		case NORD:
		case SUD: 
			segpiste = segx_occ; 
			niveaupiste = xmetal;
			break;
		case EST:
		case OUEST: 
			segpiste = segy_occ; 
			niveaupiste = ymetal;
			break;
		}

		for (piste = 2; piste < maxpiste; piste++)
			if (segment_libre(debut, fin, piste, piste, face, SEG_PISTE, segpiste, niveaupiste)) {
				firstlibre[face] = piste;

				for (pistelibre = piste; pistelibre < maxpiste; pistelibre++)
					if (!segment_libre(debut, fin, pistelibre, pistelibre, face, SEG_PISTE, segpiste,
					     niveaupiste)) { 
						nblibres[face] = pistelibre - piste; 
						break; 
					}

				if (mode_debug) 
					printf("face %d firstpistelibre %ld nbpistelibres %ld\n", (int)face, firstlibre[face],
					     nblibres[face]);

				break;
			}
	}

}

/*--------------------------------------------------------------------------------*/
/* Compression du canal de chaque face en prenant la contrainte la plus forte:    */
/* - le nombre minimum  de pistes libres entre le nb total de pistes libres et    */
/*   la difference de pistes entre 2 barres de plots mitoyennes.                  */
/* - les coordonnees sont mises a jour, et un champ compress marque la compression*/
/*   deja effectuee.                                                              */
/*--------------------------------------------------------------------------------*/

void compression_canaux(BARRE_PLOTS tab_plots[NB_FACES], COEUR lecoeur,
	LST_SEGMENT segx_occ[NB_FACES], LST_SEGMENT segy_occ[NB_FACES],
	long nblibres[NB_FACES], long firstlibre[NB_FACES])
{
	long	nblibresenv[NB_FACES];
	long	nblibresfin[NB_FACES];
	long	diff1, diff2;
	LST_SEGMENT lstsegx, lstsegy;

	int	face;

	for (face = 0; face < NB_FACES; face++) {
		diff1 = diff2 = nblibresenv[face] = nblibres[face];

		if (0 != tab_plots[face].width)
			switch (face) {
			case NORD:
				if (mode_debug) 
					printf("nord");
				if (0 != tab_plots[EST].width)
					diff1 = ((lecoeur.coord.yabs + lecoeur.height + tab_plots[NORD].coord.piste * pitch)
					    -(tab_plots[EST].coord.yabs + tab_plots[EST].width)) / pitch;
				if (0 != tab_plots[OUEST].width)
					diff2 = ((lecoeur.coord.yabs + lecoeur.height + tab_plots[NORD].coord.piste * pitch)
					    -(tab_plots[OUEST].coord.yabs + tab_plots[OUEST].width)) / pitch;

				if (diff1 < diff2) 
					nblibresenv[NORD] = diff1;
				else 
					nblibresenv[NORD] = diff2;

				break;

			case SUD:
				if (mode_debug) 
					printf("sud");
				if (0 != tab_plots[EST].width)
					diff1 = ( -(lecoeur.coord.yabs - tab_plots[SUD].coord.piste * pitch) + (tab_plots[EST].coord.yabs))
					    / pitch;
				if (0 != tab_plots[OUEST].width)
					diff2 = ( -(lecoeur.coord.yabs - tab_plots[SUD].coord.piste * pitch) + (tab_plots[OUEST].coord.yabs))
					    / pitch;

				if (diff1 < diff2) 
					nblibresenv[SUD] = diff1;
				else 
					nblibresenv[SUD] = diff2;

				break;

			case EST:
				if (mode_debug) 
					printf("est");
				if (0 != tab_plots[NORD].width)
					diff1 = ((lecoeur.coord.xabs + lecoeur.width + tab_plots[EST].coord.piste * pitch) -
					    (tab_plots[NORD].coord.xabs + tab_plots[NORD].width)) / pitch;
				if (0 != tab_plots[SUD].width)
					diff2 = ((lecoeur.coord.xabs + lecoeur.width + tab_plots[EST].coord.piste * pitch) -
					    (tab_plots[SUD].coord.xabs + tab_plots[SUD].width)) / pitch;

				if (diff1 < diff2) 
					nblibresenv[EST] = diff1;
				else 
					nblibresenv[EST] = diff2;
				break;

			case OUEST:
				if (mode_debug) 
					printf("ouest");
				if (0 != tab_plots[NORD].width)
					diff1 = (-(lecoeur.coord.xabs - tab_plots[OUEST].coord.piste * pitch ) + (tab_plots[NORD].coord.xabs))
					    / pitch;
				if (0 != tab_plots[SUD].width)
					diff2 = (-(lecoeur.coord.xabs - tab_plots[OUEST].coord.piste * pitch ) + (tab_plots[SUD].coord.xabs))
					    / pitch;

				if (diff1 < diff2) 
					nblibresenv[OUEST] = diff1;
				else 
					nblibresenv[OUEST] = diff2;
				break;
			}

		/* ------------------------------------------------------ */
		/* on prend la contrainte la plus forte donc le min des 2 */
		/* ------------------------------------------------------ */

		if (nblibres[face] < nblibresenv[face]) 
			nblibresfin[face] = nblibres[face];
		else 
			nblibresfin[face] = nblibresenv[face];

		if (mode_debug) 
			printf("nblibresenv face %d nb %ld nblibresfin %ld\n", (int)face, nblibresenv[face], nblibresfin[face]);
	}

	/* --------------------------------------------------------- */
	/* compression des canaux avec la liste des segments occupes */
	/* --------------------------------------------------------- */

	for (face = 0; face < NB_FACES; face++) {
		lstsegx  = segx_occ[face];
		lstsegy  = segy_occ[face];

		/* --------------------------------- */
		/* rapprochement des barres de plots */
		/* --------------------------------- */

		tab_plots[face].coord.piste -= nblibresfin[face];

		/* ------------------------------- */
		/* parcours des listes de segments */
		/* ------------------------------- */

		if (mode_debug) 
			printf("compression face %d des segx\n", (int)face);

		while (NULL != lstsegx) {
			switch (face) {
			case NORD:
			case SUD:
				if ((lstsegx->piste1 > firstlibre[face]) && (lstsegx->piste2 > firstlibre[face])) {
					if ((lstsegx->c1->xabs < lecoeur.coord.xabs) && (SANS_COMPRESS == lstsegx->c1->compress) &&
					    (NULL == lstsegx->c1->proprio)) {
						lstsegx->c1->xabs += nblibresfin[OUEST] * pitch;
						lstsegx->c1->compress = AVEC_COMPRESS; 
					}

					if ((lstsegx->c2->xabs > (lecoeur.coord.xabs + lecoeur.width)) && (SANS_COMPRESS ==
					    lstsegx->c2->compress) && (NULL == lstsegx->c2->proprio)) {
						lstsegx->c2->xabs -= nblibresfin[EST] * pitch;
						lstsegx->c2->compress = AVEC_COMPRESS; 
					}
				}
				break;

			case EST:
			case OUEST:
				if ((lstsegx->piste1 > firstlibre[face]) && (lstsegx->piste2 > firstlibre[face])) {
					if ((lstsegx->c1->yabs < lecoeur.coord.yabs) && (SANS_COMPRESS == lstsegx->c1->compress) &&
					    (NULL == lstsegx->c1->proprio)) {
						lstsegx->c1->yabs += nblibresfin[SUD] * pitch;
						lstsegx->c1->compress = AVEC_COMPRESS; 
					}

					if ((lstsegx->c2->yabs > (lecoeur.coord.yabs + lecoeur.height)) && (SANS_COMPRESS ==
					    lstsegx->c2->compress) && (NULL == lstsegx->c2->proprio)) {
						lstsegx->c2->yabs -= nblibresfin[NORD] * pitch;
						lstsegx->c2->compress = AVEC_COMPRESS; 
					}
				}
				break;
			}
			if (lstsegx->piste1 > firstlibre[face]) 
				lstsegx->piste1 -= nblibresfin[face];
			if (lstsegx->piste2 > firstlibre[face]) 
				lstsegx->piste2 -= nblibresfin[face];

			lstsegx = lstsegx->suiv;
		}

		if (mode_debug) 
			printf("compression face %d des segy\n", (int)face);

		while (NULL != lstsegy) {
			switch (face) {
			case NORD:
			case SUD:
				if ((lstsegy->piste1 > firstlibre[face]) && (lstsegy->piste2 > firstlibre[face])) {
					if ((lstsegy->c1->xabs < lecoeur.coord.xabs) && (SANS_COMPRESS == lstsegy->c1->compress) &&
					    (NULL == lstsegy->c1->proprio)) {
						lstsegy->c1->xabs += nblibresfin[OUEST] * pitch;
						lstsegy->c1->compress = AVEC_COMPRESS; 
					}

					if ((lstsegy->c2->xabs > (lecoeur.coord.xabs + lecoeur.width)) && (SANS_COMPRESS ==
					    lstsegy->c2->compress) && (NULL == lstsegy->c2->proprio)) {
						lstsegy->c2->xabs -= nblibresfin[EST] * pitch;
						lstsegy->c2->compress = AVEC_COMPRESS; 
					}
				}
				break;

			case EST:
			case OUEST:
				if ((lstsegy->piste1 > firstlibre[face]) && (lstsegy->piste2 > firstlibre[face])) {
					if ((lstsegy->c1->yabs < lecoeur.coord.yabs) && (SANS_COMPRESS == lstsegy->c1->compress) &&
					    (NULL == lstsegy->c1->proprio)) {
						lstsegy->c1->yabs += nblibresfin[SUD] * pitch;
						lstsegy->c1->compress = AVEC_COMPRESS; 
					}

					if ((lstsegy->c2->yabs > (lecoeur.coord.yabs + lecoeur.height)) && (SANS_COMPRESS ==
					    lstsegy->c2->compress) && (NULL == lstsegy->c2->proprio)) {
						lstsegy->c2->yabs -= nblibresfin[NORD] * pitch;
						lstsegy->c2->compress = AVEC_COMPRESS; 
					}
				}
				break;
			}
			if (lstsegy->piste1 > firstlibre[face]) 
				lstsegy->piste1 -= nblibresfin[face];
			if (lstsegy->piste2 > firstlibre[face]) 
				lstsegy->piste2 -= nblibresfin[face];

			lstsegy = lstsegy->suiv;
		}
	} /* fin du for */
}
