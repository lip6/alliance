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
/*    Fichier : lirevues.c                                                  */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*----------------------------------------------------------------------------------------*/
/* RING 28 juillet 92                         LIREVUES.C                                  */
/* PHASE 2                                                                                */
/*                                                                                        */
/* Lecture des vues structurelles (circuit, coeur) et physiques (coeur, et chargement des */
/* plots                                                                                  */
/* Modif: suite a l'abandon d'utiliser des plots de fils, mais plutot tirer directement   */
/*        les fils, on verifie les modeles de plots physiques / con est <=> con ouest     */
/*----------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "lirevues.h"
#include "sesame.h"

/*--------------------------------------------------------------------------------*/
/* Lors d ela lecture des modeles physiques des plots, on renomme les connecteurs */
/* vdd* et vss* en vdd et vss, sauf pour les connecteurs nord, car il s'agit      */
/* de vdde ou vsse (alim de plots) a ne pas confondre avec alim de coeur          */
/*--------------------------------------------------------------------------------*/

void lecture_vues(char *nom_circuit_lo, COEUR *lecoeur, lofig_list **circuit_lo,
	chain_list **liste_plotsph, int	*nbplots, chain_list **lst_conestouest)
{
	chain_list * lstph;
	phfig_list * ptfig;
	loins_list * circuit_inst;		/* instances dans le circuit logique */
	loins_list * coeur_inst = NULL;		/* instance du coeur                 */
	int	nbcoeur = 0;			/* nombres d'instances susceptibles d'etre le coeur */
	int	retour;

	*circuit_lo = getlofig(nom_circuit_lo, 'A');

	if (!(*circuit_lo)) 
		ringerreur(ERR_CIRCUITLO, nom_circuit_lo, NULL);

	/* viewlo(); */

	circuit_inst = (*circuit_lo)->LOINS;

	if (!circuit_inst) 
		ringerreur(ERR_CIRCUITINST, nom_circuit_lo, NULL);

	/* ---------------------------------------------------------------------------------------------------------- */
	/* test la coherences des modeles logiques et physiques de toutes les modeles utilisee dans la figure logique */
	/* ---------------------------------------------------------------------------------------------------------- */

	testcon_modelfig(*circuit_lo);	
   
	/* ------------------------------------------------------------------------------------------------------------- */
	/* Parcours des instances pour distinguer celles qui sont des plots et les autres (normalement 1 seule: le coeur */
	/* ------------------------------------------------------------------------------------------------------------- */

	*nbplots = 0;

	(*liste_plotsph) = NULL;   /* Initialisation de la liste de plots physiques */

	while (circuit_inst != NULL) {
		retour = incatalog(circuit_inst->FIGNAME);
		if (mode_debug) 
			printf("FIGNAME %s incatalog %d\n", circuit_inst->FIGNAME, retour);

		if (retour) {
			(*nbplots)++;

			/* -------------------------------------- */
			/* Renommage des connecteurs vdd* et vss* */
			/* -------------------------------------- */

			ajout_listeplotsph(circuit_inst->FIGNAME, liste_plotsph);
		}  else /* le coeur est reconnu */		 {
			nbcoeur++;
			coeur_inst = circuit_inst;
		}

		circuit_inst = circuit_inst->NEXT;
	}

	if (mode_debug) 
		printf("Lirevues: nb inst plots %d\n", *nbplots);

	/* -------------------------------------------- */
	/* Verification sur le nombres de coeur trouves */
	/* -------------------------------------------- */

	switch (nbcoeur) {
	case 0: 
		ringerreur(ERR_NONCOEUR, NULL, NULL); /* aucun => ringerreur */
		break;

	case 1: 
		/* ------------------------------------------------------ */
		/* un seul coeur, on charge les vues  logique et physique */
		/* ------------------------------------------------------ */

		(*lecoeur).coeur_lo = coeur_inst;
		if ((*lecoeur).coeur_lo == NULL)
			ringerreur(ERR_COEURINSTLO, coeur_inst->FIGNAME, NULL);

		ptfig = (*lecoeur).coeur_ph = getphfig(coeur_inst->FIGNAME, 'A');
		(*lecoeur).coord.xabs = 0;
		(*lecoeur).coord.yabs = 0; /* Coordonnees du coeur fixees a 0,0 par defaut */
		(*lecoeur).width = ptfig->XAB2 - ptfig->XAB1;
		(*lecoeur).height = ptfig->YAB2 - ptfig->YAB1;
		(*lecoeur).coord.piste = 0;

		(*lecoeur).rotation = NOSYM; /* pas de rotation par defaut */

		/* viewphfig((*lecoeur).coeur_ph);  */

		if ((*lecoeur).coeur_ph == NULL)
			ringerreur(ERR_COEURINSTPH, coeur_inst->FIGNAME, NULL);

		break;
	default: 
		/* ------------------------------------------ */
		/* plusieurs candidats au coeur => ringerreur */
		/* ------------------------------------------ */

		ringerreur(ERR_PLUSCOEUR, (*circuit_lo)->LOINS, NULL);
	}

	/* ----------------------------------------------------- */
	/* verification des modeles pour connexion alim internes */
	/* ----------------------------------------------------- */

	lstph = *liste_plotsph;

	while (NULL != lstph) {
		verif_con_estouest(lstph, lst_conestouest);
		lstph = lstph->NEXT;
	}

	if (mode_debug) 
		affic_listeplotsph(*liste_plotsph);

}

/*---------------------------------------------------------------------------------*/
/* Verification du nombre de connecteurs de leur position en est et ouest des alim */
/* internse des plots. Une liste resultat lst_conestouest est fabriquee qui        */
/* contient:                                                                       */
/* - une liste de couples , connecteur ouest et connecteur est de meme nom et meme */
/*   position                                                                      */
/* Cette liste servira a relier les plots entre eux et finir la couronne de plots  */
/* Modif: les connecteurs lateraux de plots etant renommes, on a des               */
/* connecteurs de mem nom et meme index; il faut alors renumeroter                 */
/* les connecteurs (changement du champ INDEX dans tous les modeles                */
/* de plots physiques                                                              */
/*---------------------------------------------------------------------------------*/

void verif_con_estouest(chain_list *liste_plotsph, chain_list **lst_conestouest)
{

	static phfig_list *firstfig = NULL;
	phfig_list        * figcour;
	phcon_list        * lstcon, *con;
	chain_list        * lst, *lstwork;
	static int	cptest = 0, cptouest = 0;
	int	cptestcour = 0, cptouestcour = 0;
	int	index = 0; /* renumeroter les connecteurs physiques */

	if (mode_debug) 
		printf("modele plot %s\n", ((phfig_list * )liste_plotsph->DATA)->NAME);

	if (NULL == liste_plotsph) 
		ringerreur(ERR_INTERNE_CON, NULL, NULL);

	if (NULL == firstfig) {
		(*lst_conestouest ) = NULL;
		firstfig = (phfig_list * ) liste_plotsph->DATA;
		lstcon = firstfig->PHCON;

		while (NULL != lstcon) {
			if ('W' == lstcon->ORIENT) { 
				cptouest++;
				if (mode_debug) 
					printf("REFERENCE con plot %s\n", lstcon->NAME);
				(*lst_conestouest) = addchain((*lst_conestouest), (void * ) lstcon);
			}
			lstcon->INDEX = index;
			index++;
			lstcon = lstcon->NEXT;
		}

		lstcon = firstfig->PHCON;

		while (NULL != lstcon) {
			if ('E' == lstcon->ORIENT)	
				cptest++;
			lstcon->INDEX = index;
			index++;
			lstcon = lstcon->NEXT;
		}

		if (cptouest != cptest) 
			ringerreur(ERR_NBCONESTOUEST , firstfig, firstfig);

		lst = (*lst_conestouest);

		while (NULL != lst) {
			con = (phcon_list * ) lst->DATA;
			if (mode_debug) 
				printf("REFERENCE con plot figcour %s ycon %ld width %ld\n", con->NAME, con->YCON, con->WIDTH);

			lstcon = firstfig->PHCON;
			while (NULL != lstcon) {
				if (mode_debug) 
					printf("REFERENCE con plot figref %s ycon %ld width %ld\n", lstcon->NAME, lstcon->YCON, lstcon->WIDTH);
				if (('E' == lstcon->ORIENT)	 
        && (lstcon->NAME == con->NAME) 
        && (lstcon->YCON == con->YCON) 
        && (lstcon->WIDTH == con->WIDTH)) {
					lstwork = addchain(lst->NEXT, (void * ) lstcon);
					lst->NEXT = lstwork;
					lst = lst->NEXT;
					break;
				} else 
					lstcon = lstcon->NEXT;
			}
			if (NULL == lstcon) 
				ringerreur(ERR_NOCONPH , con, firstfig);
			lst = lst->NEXT;
		}

		if (mode_debug) 
			printf("LISTE DES CONNECTEURS EST OUEST modele de reference\n");

		lst = (*lst_conestouest);
		while (NULL != lst) {
			con = (phcon_list * ) lst->DATA;
			if (mode_debug) 
				printf("con %s YCON %ld width %ld INDEX %ld face %c\n", con->NAME, con->YCON, con->WIDTH,
				     con->INDEX, con->ORIENT);
			lst = lst->NEXT;
		}

	}  else {

		figcour = (phfig_list * ) liste_plotsph->DATA;
		lstcon = figcour->PHCON;

		while (NULL != lstcon) {
			if (mode_debug) 
				printf("PADCOUR lstcon plot %s ycon %ld width %ld index %ld\n", lstcon->NAME, (lstcon->YCON - figcour->YAB1) , lstcon->WIDTH, lstcon->INDEX);

			if (('W' == lstcon->ORIENT) || ('E' == lstcon->ORIENT)) { 
				if ('W' == lstcon->ORIENT) 
					cptouestcour++; 
				else 
					cptestcour++;

				lst = (*lst_conestouest);
				while (NULL != lst) {
					con = (phcon_list * ) lst->DATA;
					if (mode_debug) 
						printf("\tREFERENCEcon plot %s ycon %ld width %ld index %ld\n", con->NAME,
						     (con->YCON - firstfig->YAB1), con->WIDTH, con->INDEX);

					if ((con->ORIENT == lstcon->ORIENT)
          &&  (con->NAME == lstcon->NAME) 
          &&  ((con->YCON - firstfig->YAB1) == (lstcon->YCON - figcour->YAB1)) 
          && (con->WIDTH == lstcon->WIDTH)) { 
						lstcon->INDEX = con->INDEX;  
						break; 
					} else 
						lst = lst->NEXT;
				}
				if (NULL == lst) 
					ringerreur(ERR_NOCONPH , lstcon, figcour);
			}
			lstcon = lstcon->NEXT;
		}

		if ((cptest != cptestcour) || (cptouest != cptouestcour))
			ringerreur(ERR_NBCONESTOUEST , figcour, firstfig);
	}
}
