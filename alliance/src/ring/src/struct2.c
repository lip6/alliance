/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ring router                                                 */
/*    Fichier : struct2.c                                                   */
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
/* RING 29 mai 92                       STRUCT2.C                                    */
/* Fonctions qui manipulent les structures de donnees definies ds struct.h           */
/*-----------------------------------------------------------------------------------*/

#include <stdio.h>

#include "struct.h"
#include "distance.h"
#include "sesame.h"


/*-----------------------------------------------------------------------------------*/
/* Cette fonction retourne VRAI (1) si la distance entre le connecteur courant et    */
/* le precedent est < au pitch, FAUX (0) sinon                                       */
/*-----------------------------------------------------------------------------------*/

int	moins_dun_pitch_preccon(con)
LST_PSEUDO_CON con;
{
	long	coordcon = 0, coordconprec = 0, largcon = 0, largconprec = 0, dist = 0, largeurmin = 0, dminmetalmetal = 0;

	if (con->prec == NULL) 
		return(0);  /* pas de connect precedent donc pas de
	                                      pb de voisinage */
	if (isvdd(con->con_lo->NAME) || isvss(con->con_lo->NAME))
	    return (0);

	switch (con->face) {
	case NORD:
	case SUD :
		coordcon = (con->coord)->xabs;
		coordconprec = ((con->prec)->coord)->xabs;
		dminmetalmetal = ymetal_dmin;

		if ((con->layer != ymetal) || (con->prec->layer != ymetal)) {
			if (ymetal_width < xmetal_width) 
				largeurmin = ymetal_width;
			else 
				largeurmin = xmetal_width;
		} else {
			largeurmin = ymetal_width;
		}
		break;
	case EST  :
	case OUEST:
		coordcon = (con->coord)->yabs;
		coordconprec = ((con->prec)->coord)->yabs;
		dminmetalmetal = xmetal_dmin;

		if ((con->layer != xmetal) || (con->prec->layer != xmetal)) {
			if (ymetal_width < xmetal_width) 
				largeurmin = ymetal_width;
			else 
				largeurmin = xmetal_width;
		} else {
			largeurmin = xmetal_width;
		}
		break;
	}

	largcon      = con->largeur;
	largconprec  = (con->prec)->largeur;

	dist = distance_con_con(coordcon, coordcon, largcon, con->layer, coordconprec, coordconprec, largconprec, con->prec->layer,
	     con->face);

	if ((largcon > largeurmin) || (largconprec > largeurmin)) {
		if ((largcon < taille_via) || (largconprec < taille_via))
			dminmetalmetal = DMIN_ALU2_ALU2; /* distance via-via */

		if (mode_debug) 
			printf("%s coordprec %ld larg %ld ---- %s coordcon %ld larg %ld dist %ld dminmetal %ld\n", (con->prec)->nom_con,
			     coordconprec, largconprec, con->nom_con, coordcon, largcon, dist, dminmetalmetal);
		return ((int) (dist < dminmetalmetal));
	}  else {
		if (mode_debug) 
			printf("%s coordprec %ld larg %ld ---- %s coordcon %ld larg %ld\n", (con->prec)->nom_con, coordconprec,
			     largconprec, con->nom_con, coordcon, largcon);

		return ((int) (dist < pitch));
	}
}

/*-----------------------------------------------------------------------------------*/
/*  Cette procedure ajoute les coordonnees en fin de liste dans la liste des         */
/*  pas de la grille symbolique                                                      */
/*-----------------------------------------------------------------------------------*/

void ajout_coordonnees_grille(pt_coord, liste, face)
PT_COORDONNEES pt_coord;
PT_COORDONNEES *liste;
int	face;
{

	PT_COORDONNEES cell, liste_coor;

	/* -------------------------------------------------------------------------------- */
	/* Plus besoin d'allouer on prend directement les coord du connecteur qu'on chaine  */
   	/* dans la grille                                                                   */
	/* -------------------------------------------------------------------------------- */

	cell = pt_coord;

	if ((*liste) == NULL) {
		cell->prec  = NULL;
		cell->suiv  = NULL;
		(*liste) = cell;
	}  else {
		switch (face) {
		case NORD:
		case SUD:
			liste_coor = *liste;
			while (liste_coor->suiv != NULL) {
				if ((liste_coor)->xabs  > pt_coord->xabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;

					if (liste_coor->prec == NULL) /* tete de liste modifiee */
						*liste = cell;
					else
						(cell->prec)->suiv = cell;

					liste_coor->prec = cell;
					break;
				}

				liste_coor = liste_coor->suiv;
			}

			/* fin de liste */
			if (liste_coor->suiv == NULL)
			{
				if ((liste_coor)->xabs > pt_coord->xabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;
					if (liste_coor->prec == NULL) /* tete de liste modifiee */
						*liste = cell;
					else
						(cell->prec)->suiv = cell;

					liste_coor->prec = cell;
					break;
				} else {
					liste_coor->suiv = cell;
					cell->prec  = liste_coor;
					cell->suiv  = NULL;
				}
			}
			break;

		case EST  :
		case OUEST:
			liste_coor = *liste;
			while (liste_coor->suiv != NULL) {
				if ((liste_coor)->yabs  > pt_coord->yabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;

					if (liste_coor->prec == NULL) /* tete de liste modifiee */
						*liste = cell;
					else
						(cell->prec)->suiv = cell;

					liste_coor->prec = cell;
					break;
				}

				liste_coor = liste_coor->suiv;
			}

			/* fin de liste */
			if (liste_coor->suiv == NULL)
			{
				if ((liste_coor)->yabs > pt_coord->yabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;
					if (liste_coor->prec == NULL) /* tete de liste modifiee */
						*liste = cell;
					else
						(cell->prec)->suiv = cell;

					liste_coor->prec = cell;
					break;
				} else {
					liste_coor->suiv = cell;
					cell->prec  = liste_coor;
					cell->suiv  = NULL;
				}
			}

			break;

		} /* fin du switch */

	} /* fin du else */

}

/*--------------------------------------------------------------------------------*/
/* Ajout du vis a vis dans l'equipo de type index. Il s'agit d'une ptype liste    */
/* avec noindex et pointeur sur pseudo_con                                        */
/*--------------------------------------------------------------------------------*/

void  ajout_visavis(lst_equipo, coeurcon, indexplot)
LST_EQUIPO     lst_equipo;
LST_PSEUDO_CON coeurcon;
long	indexplot;
{

	while (lst_equipo != NULL) {
		if (lst_equipo->index == indexplot) {
			/* on ajoute le signal du connecteur coeur (indexplot) dans la liste des vis a vis du connecteur plot */

			if (mode_debug) 
				printf("Ajout vis a vis con %s root %s indexplot %ld %s\n", coeurcon->nom_con, ((loins_list
				    *)(coeurcon->con_lo)->ROOT)->INSNAME, indexplot, ((LST_PSEUDO_CON) ((lst_equipo->lst_con)->NEXT)->DATA)->nom_con);

			lst_equipo->lst_visavis = addptype(lst_equipo->lst_visavis, ((coeurcon->con_lo)->SIG)->INDEX , (void
			    *) coeurcon);
			break;
		}

		lst_equipo = lst_equipo->suiv;
	}

}

/*--------------------------------------------------------------------------------*/
/*  Retourne 1 si presence d'un cycle 0 sinon                                     */
/*--------------------------------------------------------------------------------*/

int	existe_visavis(indexatrouver, indexequipo, lst_equipo)
long	indexatrouver;
long	indexequipo;
LST_EQUIPO lst_equipo;
{
	ptype_list * liste;

	while (lst_equipo != NULL) {
		if (lst_equipo->index == indexequipo) {
			liste = lst_equipo->lst_visavis;

			while (liste != NULL) {
				if (liste->TYPE == indexatrouver) 
					return(1); /* cycle */

				liste = liste->NEXT;
			}

			return(0); /* pas de cycle */
		}
		lst_equipo = lst_equipo->suiv;
	}

	return(0);
}

/*------------------------------------------------------------------------------------*/

void affic_grille(grille)
GRILLE grille[NB_FACES];
{
	PT_COORDONNEES lp , lg, ld, der;
	int	face;

	printf("AFFIC GRILLE\n");

	for (face = 0; face < NB_FACES; face++) {

		printf("FACE %d ----- Piste 1ere %ld Largeur pas %d Nb pas %ld\n", face, grille[face].piste.premiere, grille[face].piste.largeur_pas,
		     grille[face].piste.nb_pas);

		printf("Grille deportg\n");
		lg = grille[face].lst_deportg;

		while (lg != NULL) {
			printf("x %5ld ----- y %5ld\n", lg->xabs, lg->yabs);
			lg = lg->suiv;
		}

		printf("Grille pas\n");
		lp = grille[face].lst_pas;

		while (lp != NULL) {
			printf("x %5ld ----- y %5ld proprio %ld\n", (lp)->xabs, (lp)->yabs, (long)lp->proprio);
			if (lp->suiv == NULL) 
				der = lp;
			lp = lp->suiv;
		}

		lp = der;
		/*	printf("Parcours en sens inverse\n");

	while (lp != NULL)
		{
		printf("x %5ld ----- y %5ld\n", (lp)->xabs, (lp)->yabs);
		lp = lp->prec;
		}
*/
		printf("Grille deportd\n");
		ld = grille[face].lst_deportd;

		while (ld != NULL) {
			printf("x %5ld ----- y %5ld\n", (ld)->xabs, (ld)->yabs);
			ld = ld->suiv;
		}

	}

}

/*--------------------------------------------------------------------------------*/

void alloue_coord(val, face, nouv)
long	val;
int	face;
PT_COORDONNEES *nouv;
{

	(*nouv) = (PT_COORDONNEES) mbkalloc ((unsigned int) sizeof(COORDONNEES));
	(*nouv)->suiv = NULL;
	(*nouv)->prec = NULL;
	(*nouv)->proprio = NULL;
	(*nouv)->compress = SANS_COMPRESS;
	(*nouv)->piste = 0;

	switch (face) {
	case NORD:
	case SUD:
		(*nouv)->xabs = val;
		(*nouv)->yabs = 0;
		break;

	case EST  :
	case OUEST:
		(*nouv)->xabs = 0;
		(*nouv)->yabs = val;
		break;
	}

}

/*-------------------------------------------------------------------------------*/
/* recherche d'un connecteur libre cad proprio == null, pas besoin de verifier   */
/* largeur speciale, car les pas calcules en tiennet compte normalement          */
/*-------------------------------------------------------------------------------*/

PT_COORDONNEES cherche_deport_grille(con, lst_conalim)
LST_PSEUDO_CON con;
chain_list     *lst_conalim;
{
	int	trouve = 0;
	PT_COORDONNEES ptcoor;

	ptcoor = (con->coord)->prec;
	/* gauche toute */

	while (ptcoor != NULL) {
		trouve = ((ptcoor->proprio == NULL) && (!croisement_con_alim(ptcoor, con, lst_conalim)));
		if (trouve) 
			break;
		ptcoor = ptcoor->prec;
	}

	if (!trouve) {
		ptcoor = (con->coord)->suiv;

		/* droite toute */
		while (ptcoor != NULL) {
			trouve = ((ptcoor->proprio == NULL) && (!croisement_con_alim(ptcoor, con, lst_conalim)));
			if (trouve) 
				break;
			ptcoor = ptcoor->suiv;
		}
	}

	if (!trouve)
		ringerreur(ERR_ECHECDEPORT, con, NULL);
	else {
		if (mode_debug) 
			printf("deport trouve con %ld %ld\n", ptcoor->xabs, ptcoor->yabs);
		return(ptcoor);
	}
	return NULL;

}

/*-----------------------------------------------------------------------------------*/
/* Cette procedure pose une 'pince' sur les coordonnees les plus proches du deport.  */
/* C'est comme si on inserait le deport dans la grille des pas, deport pointe sur    */
/* les coordonnees immediatement inferieur et superieur, mais le deport n'appartient */
/* pas a la grille, donc la grille ou liste de pas n'est PAS MODIFIEE.               */
/*-----------------------------------------------------------------------------------*/

void chaine_deportcon_grille(pt_coord, liste, face)
PT_COORDONNEES pt_coord;
PT_COORDONNEES liste;
int	face;
{

	PT_COORDONNEES cell, liste_coor;
	int	type = 0;

	cell = pt_coord;

	if (liste == NULL) {
		cell->prec  = NULL;
		cell->suiv  = NULL;
	}  else {
		switch (face) {
		case NORD:
		case SUD:
			liste_coor = liste;
			while (liste_coor->suiv != NULL) {
				if ((liste_coor)->xabs  > pt_coord->xabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;
					break;
				}

				liste_coor = liste_coor->suiv;
			}

			/* fin de liste */
			if (liste_coor->suiv == NULL)
			{
				if ((liste_coor)->xabs > pt_coord->xabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;
					break;
				} else {
					cell->prec  = liste_coor;
					cell->suiv  = NULL;
				}
			}

			break;

		case EST  :
		case OUEST:
			liste_coor = liste;
			while (liste_coor->suiv != NULL) {
				if ((liste_coor)->yabs  > pt_coord->yabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;
					break;
				}

				liste_coor = liste_coor->suiv;
			}

			/* fin de liste */
			if (liste_coor->suiv == NULL)
			{
				if ((liste_coor)->yabs > pt_coord->yabs) {
					cell->suiv = liste_coor;
					cell->prec = liste_coor->prec;
					break;
				} else {
					cell->prec  = liste_coor;
					cell->suiv  = NULL;
				}
			}

			break;

		} /* fin du switch */

		/* post traitement, on evite les coord dont proprio = coeur */
		if ((cell->suiv != NULL) && (NULL != cell->suiv->proprio))
			if (COEUR_CON == ((LST_PSEUDO_CON) cell->suiv->proprio)->coeur_plot) {
				liste_coor = cell->suiv->suiv;

				while (NULL != liste_coor) {
					if (NULL != liste_coor->proprio)
						type = ((LST_PSEUDO_CON) liste_coor->proprio)->coeur_plot;

					if ((NULL == liste_coor->proprio) || (COEUR_CON != type)) {
						cell->suiv = liste_coor;
						break;
					}
					liste_coor = liste_coor->suiv;
				}
			}

		if ((cell->prec != NULL) && (NULL != cell->prec->proprio))
			if (COEUR_CON == ((LST_PSEUDO_CON) cell->prec->proprio)->coeur_plot) {
				liste_coor = cell->prec->prec;

				while (NULL != liste_coor) {
					if (NULL != liste_coor->proprio)
						type = ((LST_PSEUDO_CON) liste_coor->proprio)->coeur_plot;

					if ((NULL == liste_coor->proprio) || (COEUR_CON != type)) {
						cell->prec = liste_coor;
						break;
					}
					liste_coor = liste_coor->prec;
				}
			}

	} /* fin du else */

	if (mode_debug) {
		if (cell->suiv) { 
			if (cell->suiv->proprio)
				printf("proprio -> %s\n", ((LST_PSEUDO_CON) cell->suiv->proprio)->nom_con);
			printf("chaine deportcon: suiv x %ld y %ld\t", cell->suiv->xabs, cell->suiv->yabs);
		}
		if (cell->prec) { 
			if (cell->suiv->proprio)
				printf("proprio -> %s\n", ((LST_PSEUDO_CON) cell->suiv->proprio)->nom_con);
			printf("chaine deportcon: prec x %ld y %ld\n", cell->prec->xabs, cell->prec->yabs);
		}
	}

}

/*-----------------------------------------------------------------------------------------*/
/* Allocation d'un segment , chainage dans le tableau de segments occupes associes a       */
/* la face, et chainage dans la liste des segments de l'equipotentielle.                   */
/* Le tableau de segments occupes est organise de 2 manieres:                              */
/*  - pour les seg de type SEG_PISTE le tableau de segment est ordonne par piste. De plus  */
/* les segments de meme piste sont contigus ds la liste.                                   */
/*  - pour les seg de type SEG_COL le tableau n'est pas ordonnee. Seulement les segments   */
/* de meme colonne sont contigus dans la liste                                             */
/*-----------------------------------------------------------------------------------------*/

void alloue_etchaine_segment(c1, c2, piste1, piste2, face, typeseg, tab_seg, largeur, layer, vraifaux, via, equipo)
PT_COORDONNEES c1;
PT_COORDONNEES c2;
long	piste1;
long	piste2;
int	face;
int	typeseg;
LST_SEGMENT    tab_seg[NB_FACES];
long	largeur;
char	layer;
int	vraifaux;
int	via;
LST_EQUIPO     equipo;
{
	static int	first = 1;
	int	i;
	LST_SEGMENT seg, liste_seg;

	if (first) {
		for (i = 0; i < NB_FACES; i++) 
			tab_seg[i] = NULL;
		first = 0;
	}

	seg = (LST_SEGMENT) mbkalloc ((unsigned int) sizeof (SEGMENT));

	seg->vraifaux = vraifaux; /* le segment est un vrai ou un faux segment (cf alim) */
	seg->via      = via;      /* pose d'un via ou pas (cf alim)                      */

	/* on classe les colonnes et les pistes */

	switch (face) {
	case NORD:
	case SUD :
		if (c1->xabs <= c2->xabs) { 
			seg->c1 = c1; 
			seg->c2 = c2; 
		} else { 
			seg->c1 = c2; 
			seg->c2 = c1; 
		}
		break;
	case EST  :
	case OUEST:
		if (c1->yabs <= c2->yabs) { 
			seg->c1 = c1; 
			seg->c2 = c2; 
		} else { 
			seg->c1 = c2; 
			seg->c2 = c1; 
		}
		break;
	}

	seg->face = face;

	if (piste1 <= piste2) {
		seg->piste1 = piste1;
		seg->piste2 = piste2;
	} else {
		seg->piste1 = piste2;
		seg->piste2 = piste1;
	}

	if (mode_debug) 
		printf("alloue et chaine face %ld  c1x %ld c2 x %ld c1pt %ld c2pt%ld\n", (long)face, seg->c1->xabs, seg->c2->xabs,
		     (long)seg->c1, (long)seg->c2);
	seg->largeur = largeur;
	seg->layer = layer;

	/* Chainage dans la liste des segments occupes */

	if (NULL == tab_seg[face]) {
		seg->prec  = NULL;
		seg->suiv  = NULL;
		tab_seg[face] = seg;
	}  else {
		liste_seg = tab_seg[face];

		switch (typeseg) {
		case SEG_PISTE:	/* trie sur numero de piste */

			while (NULL != liste_seg->suiv) {
				if ((liste_seg)->piste1  > seg->piste1) {
					seg->suiv = liste_seg;
					seg->prec = liste_seg->prec;

					if (NULL == liste_seg->prec) /* tete de liste modifiee */
						tab_seg[face] = seg;
					else
						(seg->prec)->suiv = seg;

					liste_seg->prec = seg;
					break;
				}

				liste_seg = liste_seg->suiv;
			}

			/* fin de liste */
			if (NULL == liste_seg->suiv)
			{
				if ((liste_seg)->piste1 > seg->piste1) {
					seg->suiv = liste_seg;
					seg->prec = liste_seg->prec;
					if (NULL == liste_seg->prec) /* tete de liste modifiee */
						tab_seg[face] = seg;
					else
						(seg->prec)->suiv = seg;

					liste_seg->prec = seg;
				} else {
					liste_seg->suiv = seg;
					seg->prec  = liste_seg;
					seg->suiv  = NULL;
				}
			}
			break;

		case SEG_COL:
			/* c1 = c2 au niveau des pointeurs (equivaut a c1->xabs = c2->xabs ou c2->yabs = c1->yabs) insertion meme pointeur colonne */

			while (NULL != liste_seg) {
				if (liste_seg->c1 == c1) /*  meme colonne */ {
					/* insertion apres liste_seg */
					seg->prec = liste_seg;
					seg->suiv = liste_seg->suiv;
					liste_seg->suiv = seg;
					if (NULL != seg->suiv) 
						(seg->suiv)->prec = seg;
					break;
				}

				liste_seg = liste_seg->suiv;
			}

			if (NULL == liste_seg) /* colonne n'existe pas */ {
				/* chainage avant */
				seg->prec = NULL;
				seg->suiv = tab_seg[face];
				(tab_seg[face])->prec = seg;
				tab_seg[face] = seg;
			}
			break;
		} /* fin du switch */
	} /* fin du else */

	/* chainage en tete de la liste des segments de l'equipo */

	equipo->lst_seg = addchain(equipo->lst_seg, (void * ) seg);

}

/*------------------------------------------------------------------------------------*/
/* Cette fonction retourne 1 si le segment qu'on veut poser est libre dans la face    */
/* sinon 0 => parcours des listes de segments occupes                                 */
/*------------------------------------------------------------------------------------*/

int	segment_libre(coor1, coor2, piste1, piste2, face, typeseg, tab_seg, layer)
PT_COORDONNEES coor1;
PT_COORDONNEES coor2;
long	piste1;
long	piste2;
int	face;
int	typeseg;
LST_SEGMENT    tab_seg[NB_FACES];
char	layer;
{

	PT_COORDONNEES c1 = NULL, c2 = NULL;
	LST_SEGMENT liste_seg = tab_seg[face];
	int	trouve = 0;
	long	aux;

	/*
	if (mode_debug) printf("SEGMENT LIBRE tabseg %ld  listeseg %ld\n",(long) tab_seg, (long) liste_seg);
	*/

	/* on classe les colonnes et les pistes */

	if (piste1 > piste2) { 
		aux = piste1; 
		piste1 = piste2; 
		piste2 = aux;
	}

	switch (face) {
	case NORD:
	case SUD :
		if (coor1->xabs <= coor2->xabs) { 
			c1 = coor1; 
			c2 = coor2; 
		} else { 
			c1 = coor2; 
			c2 = coor1; 
		}
		break;
	case EST  :
	case OUEST:
		if (coor1->yabs <= coor2->yabs) { 
			c1 = coor1; 
			c2 = coor2; 
		} else { 
			c1 = coor2; 
			c2 = coor1; 
		}
		break;
	}

	switch (typeseg) {
	case SEG_PISTE:
		while ((NULL != liste_seg) && (!trouve)) {
			while ((NULL != liste_seg) && (liste_seg->piste1 == piste1)) {
				trouve = 1;
				switch (face) {
				case NORD:
				case SUD :
					if (liste_seg->layer == layer) {
						if (((c1->xabs >= (liste_seg->c1)->xabs) && (c1->xabs <= (liste_seg->c2)->xabs))
						    || ((c2->xabs >= (liste_seg->c1)->xabs) && (c2->xabs <= (liste_seg->c2)->xabs))
						    || ((c1->xabs < (liste_seg->c1)->xabs) && (c2->xabs > (liste_seg->c2)->xabs)))
							return(0); /* un des 2 points (c1,c2) appartient  au segment */
					} else if /* layer different, les seg ne doivent pas se superposer */
					(((c1->xabs > (liste_seg->c1)->xabs) && (c1->xabs < (liste_seg->c2)->xabs)) || ((c2->xabs >
					    (liste_seg->c1)->xabs) && (c2->xabs < (liste_seg->c2)->xabs)) || ((c1->xabs < (liste_seg->c1)->xabs) &&
					    (c2->xabs > (liste_seg->c2)->xabs)))
						return(0); /* un des 2 points (c1,c2) appartient  au segment */
					break;

				case EST  :
				case OUEST:
					if (liste_seg->layer == layer) {
						if (((c1->yabs >= (liste_seg->c1)->yabs) && (c1->yabs <= (liste_seg->c2)->yabs))
						    || ((c2->yabs >= (liste_seg->c1)->yabs) && (c2->yabs <= (liste_seg->c2)->yabs))
						    || ((c1->yabs < (liste_seg->c1)->yabs) && (c2->yabs > (liste_seg->c2)->yabs)))

							return(0); /* un des 2 points (c1,c2) appartient  au segment */
					} else if (((c1->yabs > (liste_seg->c1)->yabs) && (c1->yabs < (liste_seg->c2)->yabs))
					    || ((c2->yabs > (liste_seg->c1)->yabs) && (c2->yabs < (liste_seg->c2)->yabs)) ||
					    ((c1->yabs < (liste_seg->c1)->yabs) && (c2->yabs > (liste_seg->c2)->yabs)))
						return(0); /* un des 2 points (c1,c2) appartient  au segment */

					break;
				} /* fin  switch */

				liste_seg = liste_seg->suiv;
			} /* fin 2nd while */

			if (NULL != liste_seg) 
				liste_seg = liste_seg->suiv;
		}
		break;

	case SEG_COL:

		while ((NULL != liste_seg) && (!trouve)) {
			while ((NULL != liste_seg) && (liste_seg->c1 == c1)) {
				trouve = 1;
				if (liste_seg->layer == layer) {
					if (((piste1 >= liste_seg->piste1) && (piste1 <= liste_seg->piste2)) ||  ((piste2 >=
					    liste_seg->piste1) && (piste2 <= liste_seg->piste2)) ||  ((piste1 <  liste_seg->piste1) &&
					    (piste2 >  liste_seg->piste2)))
						return(0); /* une des 2 pistes appartient  au segment ou seg + grand */
				} else if /* layer <> les seg ne doivent pas se superposer */
				(((piste1 > liste_seg->piste1) && (piste1 < liste_seg->piste2)) ||  ((piste2 > liste_seg->piste1) &&
				    (piste2 < liste_seg->piste2)) ||  ((piste1 <  liste_seg->piste1) && (piste2 >  liste_seg->piste2)))
					return(0); /* une des 2 pistes appartient  au segment ou seg + grand */

				liste_seg = liste_seg->suiv;
			} /* fin 2nd while */

			if (NULL != liste_seg) 
				liste_seg = liste_seg->suiv;
		}
		break;
	}

	return(1); /* eh oui elle est libre */

}

/*-----------------------------------------------------------------------------------*/

void affic_listeseg(tab_seg)
LST_SEGMENT tab_seg[NB_FACES];
{
    LST_SEGMENT lseg;
	int	face;
	static char	a1[3] = "A1";
	static char	a2[3] = "A2";
	static char	er[3] = "ER";
	char	*metal;

	printf("LISTES DES SEGMENTS OCCUPES");
	for (face = 0; face < NB_FACES; face++) {
		lseg = tab_seg[face];

		printf("\nFACE %d\n", face);

		while (lseg) {
			if (lseg->layer == ALU1) 
				metal = a1;
			else if (lseg->layer == ALU2) 
				metal = a2; 
			else 
				metal = er;

			printf("face %d x1 %5ld y1 %5ld x2 %5ld y2 %5ld -- piste1 %3ld piste2 %3ld -- larg %3ld layer %2s \n",
			       lseg->face, lseg->c1->xabs, lseg->c1->yabs, lseg->c2->xabs, lseg->c2->yabs, lseg->piste1, lseg
			    ->piste2, lseg->largeur, metal);
          //if (NULL  == lseg->suiv) 
          //	der = lseg;

			lseg = lseg->suiv;
		}

		/* printf("PAR DERRIERE\n");
  		while (der) {
   			printf("face %d x1 %5ld y1 %5ld x2 %5ld y2 %5ld ---- piste1 %3ld piste2 %3ld --- larg %3ld layer %d \n",
   			der->face,der->c1->xabs,der->c1->yabs,der->c2->xabs,der->c2->yabs,der->piste1,der->piste2,der->largeur,(long)der->layer);
  			der = der->prec;
    		}
		*/
	}

}

/*--------------------------------------------------------------------------------*/

phcon_list *cherche_conph_circuit(con_lo, liste_plotsph, circuit_ph)
locon_list *con_lo;
chain_list *liste_plotsph;
phfig_list *circuit_ph;
{
	phfig_list * modele;
	phins_list * lst_instph = circuit_ph->PHINS;
	phcon_list * liste_con;
	char	orientnord = 'N', *nominst = ((loins_list*)con_lo->ROOT)->INSNAME;
	char	/*orientplot,*/ *figure =  ((loins_list*)con_lo->ROOT)->FIGNAME;

	while (NULL != lst_instph) {
		if (lst_instph->INSNAME == nominst) {
        //orientplot = lst_instph->TRANSF;
			break;
		}

		lst_instph = lst_instph->NEXT;
	}

	if (NULL == lst_instph) 
		ringerreur(ERR_INSTPH_PLOT, nominst, NULL);

	if ((modele = appartient_listeplotsph(figure, liste_plotsph)) != NULL) {

		liste_con = modele->PHCON;

		while (liste_con != NULL) {
			if ((liste_con->ORIENT == orientnord) && (liste_con->NAME == con_lo->NAME)) {
				return(liste_con);
			}
			liste_con = liste_con->NEXT;
		}

		ringerreur(ERR_NOCONPH_PLOT, con_lo, NULL); /* connecteur de la face non trouve */
	}  else 
		ringerreur(ERR_FIGPH_PLOT, figure, NULL); /* Modele physique non existant ! */
	return NULL;

}

/*-----------------------------------------------------------------------------------*/

long	croisement_con_alim(ptcoor, concoeur, ptalim)
PT_COORDONNEES ptcoor;
LST_PSEUDO_CON concoeur;
chain_list     *ptalim;
{
	PT_COORDONNEES lst_coor;
	LST_PSEUDO_CON con;
	long	position = 0, positioncour = 0, dminmetalmetal = 0, diff = 0;
	long	widthcour    = concoeur->largeur;
	int	i, nbcolatrouver;

	if (mode_debug) 
		printf("\tCroisement con avec alim  %s larg %ld face %d x %ld y %ld\n", concoeur->nom_con, concoeur->largeur,
		     concoeur->face, ptcoor->xabs, ptcoor->yabs);

	nbcolatrouver = (concoeur->largeur + pitch) / (2 * pitch);

	/* on regarde si les colonnes voisines necessaire pour la largeur du
   deport sont disponibles */

	lst_coor = ptcoor->suiv;
	for (i = 0; i < nbcolatrouver; i++) {
		if ((NULL != lst_coor) && (NULL == lst_coor->proprio))
			lst_coor = lst_coor->suiv;
		else 
			return ((long) 1);
	}

	lst_coor = ptcoor->prec;
	for (i = 0; i < nbcolatrouver; i++) {
		if ((NULL != lst_coor) && (NULL == lst_coor->proprio))
			lst_coor = lst_coor->prec;
		else 
			return ((long) 1);
	}

	switch (concoeur->face) {
	case NORD:
	case SUD :
		positioncour = ptcoor->xabs;
		dminmetalmetal = ymetal_dmin;
		break;
	case EST  :
	case OUEST:
		positioncour = ptcoor->yabs;
		dminmetalmetal = xmetal_dmin;
		break;
	}

	while (ptalim != NULL) {
		con = (LST_PSEUDO_CON) ptalim->DATA;

		if ((con->face == concoeur->face) && (con->coeur_plot != concoeur->coeur_plot)) {
			switch (concoeur->face) {
			case NORD:
			case SUD :
				position = (con->coord)->xabs;
				break;
			case EST  :
			case OUEST:
				position = (con->coord)->yabs;
				break;
			}

			diff = distance_con_con(position, position, con->largeur, concoeur->layer, positioncour, positioncour,
			     widthcour, concoeur->layer, concoeur->face);

			if ((diff < 0) || (diff < dminmetalmetal))
				if (mode_debug) 
					printf("\tposition cour %ld nom %s position %ld ***diffcrois %ld larg %ld\n", positioncour,
					     con->nom_con, position, diff, con->largeur);

			if (diff < 0)  
				return((long) -diff);
			if (diff < dminmetalmetal) 
				return ((long) pitch);

		}

		ptalim = ptalim->NEXT;
	}

	return((long) 0);
}

/*-----------------------------------------------------------------------------------------*/

void largeur_vddvss(equipo_vdd, equipo_vss, largvdd, largvss)
LST_EQUIPO equipo_vdd, equipo_vss;
long	*largvdd, *largvss;
{
	chain_list * ptvdd, *ptvss, *liste;
	LST_PSEUDO_CON con;
	char	*nom_con;
	long	larg_con, largeurvdd, largeurvss;
	int	existevdd = 0, existevss = 0;
  //static int	mess = 1;

	ptvdd = equipo_vdd->lst_con;
	ptvss = equipo_vss->lst_con;
	largeurvdd = 0; 
	largeurvss = 0;

	while (NULL != ptvdd) {
		con = (LST_PSEUDO_CON) ptvdd->DATA;
		if (con->coeur_plot == PLOT_CON)
			largeurvdd += con->largeur;

		ptvdd = ptvdd->NEXT;
	}

	while (NULL != ptvss) {
		con = (LST_PSEUDO_CON) ptvss->DATA;
		if (con->coeur_plot == PLOT_CON)
			largeurvss += con->largeur;

		ptvss = ptvss->NEXT;
	}

	if (mode_debug) 
		printf("Prise en compte width fic param\n");

	/* verification sur la liste des connecteurs de largeur specifique */

	liste = liste_width;

	while (liste != NULL) {
		nom_con  = ((PTCEL)(liste->DATA))->chaine;
		larg_con = ((PTCEL)(liste->DATA))->largeur;
		if (mode_debug) 
			printf("Connecteur --%s-- largeur %ld\n", nom_con, larg_con);

		if (eq_vss == nom_con) {
			existevss = 1;
      if (mode_debug) {
				fprintf(stderr, "the default width track for <vss> is <%ld> and your request is <%ld>.\n",
				     (largeurvss / 2) / SCALE_X, larg_con);
      }
			(*largvss) = vss_width = larg_con * SCALE_X; 
		}

		if (eq_vdd == nom_con) {
			existevdd = 1;
      if (mode_debug) {
				fprintf(stderr, "the default width track for <vdd> is <%ld> and your request is <%ld>.\n",
				     (largeurvdd / 2) / SCALE_X, larg_con);
      }
			(*largvdd) = vdd_width = larg_con * SCALE_X; 
		}

		liste = liste->NEXT;
	}

  //mess = 0;
	if (!existevdd)  
		vdd_width = (*largvdd) = largeurvdd / 2;
	if (!existevss)  
		vss_width = (*largvss) = largeurvss / 2;
}

/*-----------------------------------------------------------------------------------*/
/* Cette fonction retourne 1 si le croisement est suffisant entre le fil d'alim      */
/* coeur et son homologue plot, sinon 0 (deport a faire gag!)                        */
/* Modif: on verifie que le conplot alim est du meme layer le metalcol de routage    */
/* si testlayer =0                                                                   */
/*-----------------------------------------------------------------------------------*/

long	croisementok_alimequi(concoeur, ptalim, testlayer)
LST_PSEUDO_CON concoeur;
chain_list     *ptalim;
char	testlayer;
{
	LST_PSEUDO_CON con;
	long	position = 0, positioncour = 0;
	char	metalcol = 0;

	if (mode_debug) 
		printf("\tCroisement con avec alim equi %s larg %ld face %d x %ld y %ld\n", concoeur->nom_con, concoeur->largeur,
		     concoeur->face, concoeur->coord->xabs, concoeur->coord->yabs);

	switch (concoeur->face) {
	case NORD:
	case SUD :
		positioncour = concoeur->coord->xabs;
		metalcol = ymetal;
		break;
	case EST  :
	case OUEST:
		positioncour = concoeur->coord->yabs;
		metalcol = xmetal;
		break;
	}

	while (ptalim != NULL) {
		con = (LST_PSEUDO_CON) ptalim->DATA;

		if ((con->face == concoeur->face) && (con->coeur_plot != concoeur->coeur_plot)) {
			switch (concoeur->face) {
			case NORD:
			case SUD :
				position = (con->coord)->xabs;
				break;
			case EST  :
			case OUEST:
				position = (con->coord)->yabs;
				break;
			}

			if (mode_debug) 
				printf("\tCroisement conplot avec alim equi %s larg %ld face %d position%ld\n", con->nom_con,
				     con->largeur, con->face, position);

			if (position < positioncour) { 
				if (((position + con->largeur / 2) >= positioncour) && ((con->layer == metalcol) || (testlayer)) ) { 
					if (mode_debug) 
						printf("croisement trouve\n");
					return((long) 1);
				}
			} else if (position > positioncour) { 
				if (((position - con->largeur / 2) <= positioncour) && ((con->layer == metalcol) || (testlayer)) ) {
					if (mode_debug) 
						printf("croisement trouve\n");
					return((long) 1);
				}
			} else if ((con->layer == metalcol) || (testlayer)) {  
				if (mode_debug) 
					printf("croisement trouve\n");
				return ((long) 1);
			}
		}

		ptalim = ptalim->NEXT;
	}

	return((long) 0);
}

/*----------------------------------------------------------------------------------*/
/* Cette procedure compte le nombre de plots dans la liste                          */
/*----------------------------------------------------------------------------------*/

long	compte_nbplotsface(liste)
chain_list *liste;
{

	long	cpt = 0;

	while (NULL != liste) { 
		cpt++; 
		liste = liste->NEXT; 
	}

	return(cpt);
}

