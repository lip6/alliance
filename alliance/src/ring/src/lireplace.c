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
/*    Fichier : lireplace.c                                                 */
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
/* RING 29 avril 92           LIREPLACE.C                                            */
/* PHASE 3                                                                           */
/*                                                                                   */
/* Lecture du fichier de placement afin de connaitre les plots, leur orientation,    */
/* ainsi que les signaux de largeur specifique.                                      */
/*                                                                                   */
/* Pour la lecture et l'interpretation du fichier les outils lex et yacc sont        */
/* utilises                                                                          */
/*-----------------------------------------------------------------------------------*/

#include <stdio.h>

#include "lireplace.h"
#include "sesame.h"

/*-----------------------------------------------------------------------------------*/
/* Lecture et verification du fichier de parametres , suffixe .rin. Lancement de     */
/* l'analyseur syntaxique                                                            */
/*-----------------------------------------------------------------------------------*/

void lecture_fic(char *nomfic, lofig_list *circuit_lo,
	COEUR lecoeur, int nb_inst_plots)
{
	FILE * fichier;
	long	longueur;
	chain_list * liste;
	int	i, cpt_liste = 0, cpt_plots = 0, larg_con;
	char	*nom_du_plot, *nom_con;
	loins_list * ptinst;

	/* ------------------------------------------------------------------------- */
	/* Ouverture et verification du fichier de parametre (existence et longueur) */
	/* ------------------------------------------------------------------------- */

	if ((fichier = mbkfopen(nomfic, NULL, READ_TEXT)) == NULL) 
		ringerreur(ERR_FICPARAM, nomfic, NULL);

	if (fseek(fichier, (long)0, SEEK_END))   
		ringerreur(ERR_FICPARAM, nomfic, NULL);

	if ((longueur = ftell(fichier)) == -1)     
		ringerreur(ERR_FICPARAM, nomfic, NULL);

	if (longueur == 0)                       
		ringerreur(ERR_VIDEFICPARAM, nomfic, NULL);

	fclose(fichier);

	if (mode_debug) 
		printf("Longueur fichier parametres: %ld\n", longueur);

	for (i = 0; i < NB_FACES; i++)
		nom_plot[i] = NULL;

	liste_width = NULL;                 /* Initialisations des listes globales utilisees
	                                       par lex et yacc */

	if (mode_debug) 
		printf("Avant analyse lex et yacc \n");

	yyin = mbkfopen(nomfic, NULL, READ_TEXT);

	/* ------------------------------------------------------------------ */
	/* lancement de lex et yacc pour interpreter le fichier de parametres */
	/* ------------------------------------------------------------------ */

	yyparse();
	if (mode_debug) 
		printf("Apres analyse lex et yacc\n");

	/* ------------------------------------------------------------------ */
	/* inversion des listes pour respecter l'ordre du placement des plots */
	/* verification de la coherence des informations receuillies          */
	/* ------------------------------------------------------------------ */

	for (i = 0; i < NB_FACES; i++) {
		nom_plot[i] = reverse(nom_plot[i]);
		if (nom_plot[i] != NULL) 
			cpt_liste++;
	}

	if (!cpt_liste) 
		ringerreur(ERR_AUCUNPLOTPARAM, NULL, NULL);

	/* -------------------------------------------------------------------------- */
	/* parcours des noms d'instances des plots et verif si existent ds circuit_lo */
	/* -------------------------------------------------------------------------- */

	for (i = 0; i < NB_FACES; i++) {
		if (mode_debug) 
			printf("---------numero face :%d\n", i);

		liste = nom_plot[i];

		/*--------------------------------------------------------------------------- */
		/* De part la modification des chain_list de nom_plot, je verifie d'abord sur */
		/* l'ancienne structure (noms uniquement) qu'il y a une seule occurrence      */
		/*--------------------------------------------------------------------------- */

		while (liste != NULL) {
			nom_du_plot = (char *)(liste->DATA);
			if (mode_debug) 
				printf("VERIF nom plot: --%s--\n", nom_du_plot);

			/* ---------------------------------------------------- */
			/* une seule occurence dans la liste des faces de plots */
			/* ---------------------------------------------------- */

			if (!uneseuleoccurence(nom_du_plot, nom_plot))
				ringerreur(ERR_PLUSPLOTPARAM, nom_du_plot, NULL);

			liste = liste->NEXT;
		}

		liste = nom_plot[i];

		while (liste != NULL) {
			nom_du_plot = (char *)(liste->DATA);
			if (mode_debug) 
				printf("VERIF nom plot: --%s--\n", nom_du_plot);

			/* --------------------------------------------------- */
			/* existence  du plot dans la liste du circuit logique */
			/* --------------------------------------------------- */

			if ((ptinst = existe_plot_circuit(nom_du_plot, circuit_lo)) == NULL)
				ringerreur(ERR_NONPLOTPARAM, nom_du_plot, NULL);

			else /* Chain_list modifiee pointe maintenant sur l'instance logique */
				liste->DATA = (void * ) ptinst;

			cpt_plots++;

			liste = liste->NEXT;
		}
	}

	if (cpt_plots != nb_inst_plots)
		ringerreur(ERR_NBPLOTPARAM, circuit_lo, NULL);

	liste = liste_width;
	if (mode_debug) 
		printf("VERIF Liste width\n");

	/* --------------------------------------------------------------- */
	/* verification sur la liste des connecteurs de largeur specifique */
	/* --------------------------------------------------------------- */

	while (liste != NULL) {
		nom_con  = ((PTCEL)(liste->DATA))->chaine;
		larg_con = ((PTCEL)(liste->DATA))->largeur;
		if (mode_debug) 
			printf("Connecteur --%s-- largeur %d\n", nom_con, larg_con);

		/* -------------------------------------------------------------------------- */
		/* existence  du connecteur dans la liste des connecteurs logiques du circuit */
		/* -------------------------------------------------------------------------- */

		if (!existe_signal_circuit(liste, circuit_lo, lecoeur))
			ringerreur(ERR_NONCONWIDTHPARAM, nom_con, NULL);

		if (larg_con < SCALE_X) 
			ringerreur(ERR_WIDTHPARAM, (void * )nom_con, (void * ) & larg_con);

		liste = liste->NEXT;
	}

}

/*-----------------------------------------------------------------------------------*/
/* Erreur dans la syntaxe du fichier de parametre                                    */
/*-----------------------------------------------------------------------------------*/

void
yyerror(s)
char	*s;
{
    ringerreur(ERR_SYNPARAM, NULL, NULL);
}

/*-----------------------------------------------------------------------------------*/
/* Fabrication de la liste des equipotentielles a partir de la vue logique du coeur  */
/* et circuit                                                                        */
/*-----------------------------------------------------------------------------------*/

void fabrique_equipo(chain_list *nomplot[NB_FACES], COEUR lecoeur, 
	LST_EQUIPO *lst_equipo, chain_list *liste_plotsph)
{

	locon_list * pt_coeur, *tete_coeur, *con_coeur = (lecoeur.coeur_lo)->LOCON ;
	locon_list * con_plot, *liste_con, *liste_con2;
	losig_list * sig_coeur;
	loins_list * plot, *plot2;
	int	face, i, j,  signal_ok;
	chain_list * liste_plots, *liste_plots2;
	LST_EQUIPO liste, pt_eq;

	(*lst_equipo) = NULL;

	if (mode_debug) 
		printf("Fabrication des equipo\n");

	/* ------------------------- */
	/* boucle des locon du coeur */
	/* ------------------------- */

	while (con_coeur != NULL) {
		sig_coeur = con_coeur->SIG;  /* un signal unique par connecteur */
		if (mode_debug) 
			printf("pointeur Sigcoeur %ld\n", (long) sig_coeur);
		if (mode_debug) 
			printf("*******NOM concoeur %s signal %ld*********\n", con_coeur->NAME, sig_coeur->INDEX);

		/* ---------------------------- */
		/* Parcours des instances plots */
		/* ---------------------------- */

		for (face = 0; face < NB_FACES; face++) {

			liste_plots = nomplot[face];

			while (liste_plots != NULL) {

				plot = (loins_list * ) liste_plots->DATA;

				/* ----------------------------------------------------------------------------------------------------------- */
				/* S'il s'agit d'un plot on regarde les signaux des connecteurs du plot pour trouver meme signal que con_coeur */
				/* ----------------------------------------------------------------------------------------------------------- */

				if (mode_debug) printf("PLOT  %s de la face %d\n",plot->INSNAME,face); 

				if ((con_plot = existe_sig_plot(sig_coeur, plot, liste_plotsph)) != NULL) {
					maj_equipo(con_coeur, con_plot, lst_equipo, COEUR_PLOT);
				}

				liste_plots = liste_plots->NEXT;
			} /* fin parcours instances plots d'une face */

		} /* fin  de parcours de toutes les faces */

		con_coeur = con_coeur->NEXT;

	} /* fin de parcours des con du coeur  */

	/*------------------------- equipo PLOT_PLOT -----------------------------------------*/

	for (i = 0; i < NB_FACES; i++) {
		liste_plots = nomplot[i];
		while (liste_plots != NULL) {

			plot = (loins_list * ) liste_plots->DATA;
			liste_con = plot->LOCON;

			while (liste_con != NULL) {
				pt_eq = existe_sig_equipo((*lst_equipo), (liste_con->SIG)->INDEX);
				signal_ok = ((pt_eq == NULL) || (pt_eq->type == PLOT_PLOT));

				if ((signal_ok) 
				&& existe_leconnecteur_faceplot(plot->FIGNAME, liste_con->NAME, SUD, liste_plotsph)){
					for (j = 0; j < NB_FACES; j++) {
						liste_plots2 = nomplot[j];

						while (liste_plots2 != NULL) {
							plot2 = (loins_list * ) liste_plots2->DATA;
							liste_con2 = plot2->LOCON;

							while (liste_con2 != NULL) {
								if (existe_leconnecteur_faceplot(plot->FIGNAME, liste_con->NAME, SUD, liste_plotsph)
								&& (liste_con2 != liste_con) 
								&& ((liste_con->SIG)->INDEX == (liste_con2->SIG)->INDEX)) {
								if (mode_debug) 
									printf("TROUVE liste_con ->%s *** %s\n", liste_con->NAME, liste_con2->NAME);
									maj_equipo(liste_con, liste_con2, lst_equipo, PLOT_PLOT);
								}
								liste_con2 = liste_con2->NEXT;
							}
							liste_plots2 = liste_plots2->NEXT;
						}
					}  /* ffin du for */
				} /* fin du if */
				liste_con = liste_con->NEXT;
			}
			liste_plots = liste_plots->NEXT; /* fin du  while */
		}

	}	 /* fin du 1er for */

	/*-------------------- equipo de type COEUR_COEUR  ----------------------------------*/

	tete_coeur = con_coeur = (lecoeur.coeur_lo)->LOCON;

	while (con_coeur != NULL) {
		pt_eq = existe_sig_equipo((*lst_equipo), (con_coeur->SIG)->INDEX);
		signal_ok = ((pt_eq == NULL) || (pt_eq->type == COEUR_COEUR));

		pt_coeur = tete_coeur;
		if (signal_ok)

			while (pt_coeur != NULL) {
				if ((pt_coeur != con_coeur) && ((con_coeur->SIG)->INDEX == (pt_coeur->SIG)->INDEX))
					maj_equipo(con_coeur, pt_coeur, lst_equipo, COEUR_COEUR);

				pt_coeur = pt_coeur->NEXT;
			}
		con_coeur = con_coeur->NEXT;
	}

	/*-----------------------------------------------------------------------------------*/
	/* on  inverse les chain list de pseudo connecteurs associes a chaque equipo,        */
	/* ce qui a pour but de mettre le connecteur du coeur en tete de liste               */
	/*-----------------------------------------------------------------------------------*/

	liste = (*lst_equipo);

	while (liste != NULL) {
		liste->lst_con = reverse(liste->lst_con);
		liste = liste->suiv;
	}

}

/*-----------------------------------------------------------------------------------*/
/* Parcours des connecteurs de plots et des signaux associes                         */
/* Retourne le connecteur si signal identique                                        */
/*-----------------------------------------------------------------------------------*/

locon_list *existe_sig_plot(losig_list *sig_coeur, loins_list *plot,
	chain_list *liste_plotsph)
{

	locon_list * con_plot = plot->LOCON;
	losig_list * sig_plot;

	if (mode_debug) printf("Existe sig_plot\n");

	/* -------------------------------- */
	/* Parcours des connecteurs du plot */
	/* -------------------------------- */

	while (con_plot != NULL) {

		if (mode_debug) 
			printf("\tpointeur nom %ld\n",(long)con_plot); 

		sig_plot = con_plot->SIG; /* un signal unique par connecteur */

		if (mode_debug) 
			printf("\tnom conplot %s signal %ld sigcoeur %ld\n",con_plot->NAME,sig_plot->INDEX, sig_coeur->INDEX);

		if (sig_plot->INDEX == sig_coeur->INDEX)
			if (  ( !isvdd(con_plot->NAME) && !isvss(con_plot->NAME) )
			|| (  (  isvdd(con_plot->NAME) ||  isvss(con_plot->NAME) )
			   && (  existe_leconnecteur_faceplot(plot->FIGNAME, con_plot->NAME, SUD, liste_plotsph)
			      || existe_leconnecteur_faceplot(plot->FIGNAME, con_plot->NAME, NORD, liste_plotsph))))
				return(con_plot);

		con_plot = con_plot->NEXT;
	}

	return(NULL);
}
