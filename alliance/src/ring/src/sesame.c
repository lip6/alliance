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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : ring router                                                 */
/*    Fichier : sesame.c                                                    */
/*                                                                          */
/*    (c) copyright 1992 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Ecrit   par : Olivier Van Hautte                  le : 01/08/92       */
/*    Modifie par : Franck Wajsburt                     le :  9/11/94       */
/*                                                                          */
/****************************************************************************/

/*---------------------------------------------------------------------------------*/
/* RING 28 avril 92                       SESAME.C                                 */
/* sesame: verification de la coherence des donnees fournies (parametres, netlist, */
/* fichier parametre) et affichage de message d'ringerreur                         */
/*---------------------------------------------------------------------------------*/

#include <stdlib.h>

#include "sesame.h"

/*---------------------------------------------------------------------------------*/

void ringerreur(int code, void *pt_liste, void *pt_liste2)
{
	loins_list * circuit_inst, *liste_inst;
	locon_list * con_lo;
	chain_list * liste;
	int	i, trouve, face;
	char	car;
	LST_PSEUDO_CON con;
	long	largeurmax;

	switch (code) {
	case ERR_CONPH_SANS_LO:
		fprintf(stderr, "The physical connector <%s> in model <%s> has no logical corresponding\n",(char *)pt_liste,(char *)pt_liste2);
		break;
		
	case ERR_CONLO_SANS_PH:
		fprintf(stderr, "The logical connector <%s> in model <%s> has no physical corresponding\n",(char *)pt_liste,(char *)pt_liste2);
		break;
		
	case ERR_CONCOEUREMPTY:
		fprintf(stderr, "The physical core <%s> has no connectors\n",(char *)(((COEUR *)pt_liste)->coeur_lo->INSNAME));
		break;
		
	case ERR_ARGUMENT:
		usage();
		break;

	case ERR_CATAL:
		fprintf(stderr, "The catalog file <%s> hasn't been found in MBK_WORK_LIB, and MBK_CATA_LIB path.\n", (char
		*) pt_liste);
		fprintf(stderr, "See your environment (command \"env\").\n");
		break;

	case ERR_NONCOEUR:
		fprintf(stderr, "The instance of the core wasn't found.\n");
		fprintf(stderr, "be carefull: logical figure core and physical figure core\n");
		fprintf(stderr, "             must have the same name.\n");
		fprintf(stderr, "The core must not be in the catalog (see MBK_CATAL_NAME in your environment).\n");
		break;

	case ERR_PLUSCOEUR:
		fprintf(stderr, "Several instances of the core have been found:\n");

		circuit_inst = (loins_list * ) pt_liste;

		while (circuit_inst != NULL) {
			if (!incatalog(circuit_inst->FIGNAME))
				fprintf(stderr, "\tcore name: %s\n", circuit_inst->INSNAME);

			circuit_inst = circuit_inst->NEXT;
		}
		break;

	case ERR_CIRCUITLO:
		fprintf(stderr, "The figure named <%s> hasn't been found.\n", (char *) pt_liste);
		break;

	case ERR_CIRCUITINST:
		fprintf(stderr, "The figure named <%s> doesn't own any instance.\n", (char *) pt_liste);
		break;

	case ERR_COEURINSTLO:
		fprintf(stderr, "The logical instance core which name is <%s>, hasn't been found.\n", (char *) pt_liste);
		fprintf(stderr, "Logical and physical figure core must have the same name.\n");

		break;

	case ERR_COEURINSTPH:
		fprintf(stderr, "The physical instance core which name is <%s>, hasn't been found.\n", (char *) pt_liste);
		fprintf(stderr, "Logical and physical figure core must have the same name.\n");
		break;

	case ERR_FICPARAM:
		fprintf(stderr, "The parameter file <%s> doesn't exist.\n", (char *) pt_liste);
		fprintf(stderr, "It must be present in your MBK path.\n");
		break;

	case ERR_VIDEFICPARAM:
		fprintf(stderr, "The parameter file <%s> is empty.\n", (char *) pt_liste);
		break;

	case ERR_SYNPARAM:
		fprintf(stderr, "Syntax error from the analysis of the parameter file .rin <%s>\n: ",
		     		     nom_fic_param);
#if 0
		if ((fichier = mbkfopen(nom_fic_param, NULL, READ_TEXT)) != NULL) {
			tmp = (char *) mbkalloc (800 * (unsigned int) sizeof(char));
			for (i = 0; i < (*((int *) pt_liste)); i++)
				ret = fgets(tmp, 799, fichier);
			if (NULL != ret)
				fprintf(stderr, "%s\n", tmp);
			fclose(fichier);
		}
#endif
		fprintf(stderr, "Warning: if you use # for comments, the rest of the line will be ignored.\n");

		break;

	case ERR_WIDTHPARAM:
		fprintf(stderr, "the width track <%d> of <%s> is too small.\n", *((int *)pt_liste2), (char *)pt_liste);
		break;

	case ERR_AUCUNPLOTPARAM:
		fprintf(stderr, "No pad has been founded in the paramater file .rin.\n");
		fprintf(stderr, "You must define at least one group of pads.\n");
		break;

	case ERR_NONPLOTPARAM:
		fprintf(stderr, "The instance pad <%s> hasn't been found in the logical figure.\n", (char *) pt_liste);
		break;

	case ERR_PLUSPLOTPARAM:
		fprintf(stderr, "The instance of the pad <%s>, has been found\n", (char *) pt_liste);
		fprintf(stderr, "in different sections of pads in the parameter file .rin.\n");
		break;

	case ERR_NBPLOTPARAM:
		fprintf(stderr, "Some instance pads of the logical figure\n");
		fprintf(stderr, "haven't been found in the parameter file.\n");

		/* ---------------------------------------------------------------------------------- */
		/* Affichage des plots de la figure logique non trouves dans le fichier de parametres */
		/* ---------------------------------------------------------------------------------- */

		liste_inst = ((lofig_list * ) pt_liste)->LOINS;

		/* ------------------------------- */
		/* Parcours des instances logiques */
		/* ------------------------------- */

		while (liste_inst != NULL) {
			trouve = 0;

			/* -------------------------------------------------------- */
			/* Parcours des noms d'instances definis dans fichier param */
			/* -------------------------------------------------------- */

			for (i = 0; ((i < NB_FACES) && (!trouve)); i++) {
				liste = nom_plot[i];

				while (liste != NULL) {
					
					/* ------------------------------------------------------------------------- */
					/* si trouve , on sort du while et du for, et on passe a l'instance suivante */
					/* modif : chain_list pointe sur des loins maintenant                        */
					/* ------------------------------------------------------------------------- */

					if ((trouve = liste_inst->INSNAME ==  ((loins_list * ) liste->DATA)->INSNAME ))
						break;

					liste = liste->NEXT;
				}
			}

			if ((!trouve) && (incatalog(liste_inst->FIGNAME)))
				fprintf(stderr, "\tInstance not found: <%s>\n", liste_inst->INSNAME);

			liste_inst = liste_inst->NEXT;
		}

		break;

	case ERR_NONCONWIDTHPARAM:
		fprintf(stderr, "The connector <%s> from the parameter file hasn't been found\n", (char *) pt_liste);
		fprintf(stderr, "as a logical connector of a pad, or as a physical connector\n");
		fprintf(stderr, "of the core.\n");
		break;

	case ERR_INTERNE_CON:
		fprintf(stderr, "Internal error from the connector list.\n");
		break;

	case ERR_INTERNE_FIG:
		fprintf(stderr, "Internal error from the physical figure list.\n");
		break;

	case ERR_INTERNE_EQ:
		fprintf(stderr, "Internal error from the equipotential list.\n");
		break;

	case ERR_GRILLEINTERNE:
		fprintf(stderr, "Internal error from the gate list.\n");
		break;

	case ERR_CONCOEUR_ENLAIR :
		fprintf(stderr, "Connector <%s> of the core isn't connected.\n", (char *) pt_liste);
		break;

	case ERR_CONPLOT_ENLAIR :
		fprintf(stderr, "Connector <%s> of the pad isn't connected.\n", (char *) pt_liste);
		break;

	case ERR_CONLARGEUR:
		if (ymetal_width > xmetal_width)
			largeurmax = ymetal_width;
		else
			largeurmax = xmetal_width;

		if (((LST_PSEUDO_CON) pt_liste)->coeur_plot == COEUR_CON) {
			fprintf(stderr, "Width of the connector <%s> is <%ld> of the core ", ((LST_PSEUDO_CON) pt_liste)->nom_con,
			     			     ((LST_PSEUDO_CON) pt_liste)->largeur / SCALE_X);
			if (((LST_PSEUDO_CON) pt_liste)->largeur > largeurmax)
				fprintf(stderr, "is too big.\n");
			else
				fprintf(stderr, "isn't big enough.\n");
		}  else {
			fprintf(stderr, "Width of the connector <%s> is <%ld> of the pad <%s> ", ((LST_PSEUDO_CON) pt_liste)->nom_con,
			     			     ((LST_PSEUDO_CON) pt_liste)->largeur / SCALE_X, ((loins_list * )((LST_PSEUDO_CON) pt_liste)->con_lo->ROOT)->INSNAME);
			if (((LST_PSEUDO_CON) pt_liste)->largeur > largeurmax)
				fprintf(stderr, "is too big.\n");
			else
				fprintf(stderr, "isn't big enough.\n");
		}

		break;
	case ERR_CONDISTANCE:
		con = (LST_PSEUDO_CON) pt_liste;
		if (con->coeur_plot == COEUR_CON) {
			if (con->prec != NULL)
				fprintf(stderr, "Distance between connector <%s> and connector <%s> of the core isn't big enough.\n",
					(con->prec)->nom_con, con->nom_con);  
				/* con->prec si pas d'ringerreur c'est lesieur */
			else
				fprintf(stderr, "Distance between connector <%s> and... of the core isn't big enough.\n", con->nom_con);
		} else {
			if (con->prec != NULL)
				fprintf(stderr, "Distance between connector <%s> of the pad <%s>, and connector <%s> of the pad <%s> isn't big enough.\n",
					(con->prec)->nom_con, ((loins_list * )(con->prec)->con_lo->ROOT)->INSNAME, con->nom_con,
					((loins_list * )con->con_lo->ROOT)->INSNAME);  
				/* con->prec si pas d'ringerreur c'est lesieur */
			else
				fprintf(stderr, "Distance between connector <%s> of the pad <%s> and... of the core isn't big enough.\n",
					con->nom_con, ((loins_list * ) con->con_lo->ROOT)->INSNAME);
		}

		break;

	case ERR_MANQUEALIM:
		fprintf(stderr, "Power supply <vdd> and/or <vss> weren't found in the chip.\n");
		break;

	case ERR_BARREALIM :
		face = *((int *) pt_liste);
		switch (face) {
		case NORD:
			car = 'N';
			break;
		case SUD :
			car = 'S';
			break;
		case EST:
			car = 'E';
			break;
		case OUEST:
			car = 'W';
			break;
		default:
			car = 'X';
			break;
		}

		fprintf(stderr, "The pads <vdd> and/or <vss> aren't place in front of the side '%c' of the core.\n", car);
		fprintf(stderr, "You may place again the supply pads, in order to center them in front of the core.\n");
		break;

	case ERR_ECHECDEPORT :
		con = (LST_PSEUDO_CON) pt_liste;
		fprintf(stderr, "No space hasn't been found for the connector <%s> of the instance <%s>.\n", con->nom_con,
		     		     ((loins_list * )(con->con_lo)->ROOT)->INSNAME);
		break;

	case ERR_SEGDEPORT :
		con = (LST_PSEUDO_CON) pt_liste;
		fprintf(stderr, "No space hasn't been found for the deport of the connector <%s> of the instance <%s>.\n",
		     		     		con->nom_con, ((loins_list * )(con->con_lo)->ROOT)->INSNAME);
		break;

	case ERR_NOPISTE :
		face = *((int *) pt_liste);
		switch (face) {
		case NORD:
			car = 'N';
			break;
		case SUD :
			car = 'S';
			break;
		case EST:
			car = 'E';
			break;
		case OUEST:
			car = 'W';
			break;
		default:
			car = 'X';
			break;
		}

		fprintf(stderr, "No space has been found in side %c to put the power supply <vdd> and <vss> around the core.\n",
		     		     car);
		break;

	case ERR_NOCOL :
		con = (LST_PSEUDO_CON) pt_liste;
		face = con->face;
		switch (face) {
		case NORD:
			car = 'N';
			break;
		case SUD :
			car = 'S';
			break;
		case EST:
			car = 'E';
			break;
		case OUEST:
			car = 'W';
			break;
		default:
			car = 'X';
			break;
		}

		fprintf(stderr, "No space has been found in side '%c', in order to put the connector <%s> of the instance <%s> on the ring.\n",
		     		     		car, con->nom_con, ((loins_list * ) (con->con_lo)->ROOT)->INSNAME);
		break;

	case ERR_INSTPH_PLOT:
		fprintf(stderr, "The physical instance of the pad <%s> hasn't been found in the circuit.\n", (char *) pt_liste);
		break;

	case ERR_NOCONPH_PLOT:
		con_lo = (locon_list * ) pt_liste;
		fprintf(stderr, "The connector <%s> hasn't been found as an external connector of the pad <%s>.\n", con_lo->NAME,
		     		     ((loins_list * )con_lo->ROOT)->INSNAME);
		break;

	case ERR_FIGPH_PLOT:
		fprintf(stderr, "The physical figure of pad <%s> hasn't been found.\n", (char *) pt_liste);
		break;

	case ERR_CONEXT_ENLAIR:
		fprintf(stderr, "The connector <%s> of the circuit isn't connected to a pad.\n", (char *) pt_liste);
		break;

	case ERR_NBCONESTOUEST:
		fprintf(stderr, "There isn't the same number of physical connectors on side east and west from the model of pad <%s>\n",
		     		     (char *) ((phfig_list * ) pt_liste)->NAME) ;
		fprintf(stderr, "It's impossible to link this pad to the others.\n");
		break;

	case ERR_NOCONPH:
		fprintf(stderr, "The connector <%s> of the figure pad <%s> hasn't been found or doesn't have the physical features required.\n",
		     		     (char *) ((phcon_list * )pt_liste)->NAME, (char *) ((phfig_list * )pt_liste2)->NAME);
		break;

	case ERR_EQINTERNE:
		liste = (chain_list * ) pt_liste;
		fprintf(stderr, "Some errors have been found in the internal equipotential list, pad connectors must be on the internal side of pads.\n");
		fprintf(stderr, "That's to say, your C file using GENLIB functions may be wrong.\n");
		fprintf(stderr, "The following connector names do not belong to the SOUTH side of the figure of pads.\n");

		while (NULL != liste) {
			fprintf(stderr, "- The connector <%s> of the instance pad <%s> isn't placed on the SOUTH side.\n",
			     			     (char *)liste->NEXT->DATA, (char *)liste->DATA);
			liste = liste->NEXT->NEXT;
		}

		break;

	case ERR_EQEXTERNE:
		liste = (chain_list * ) pt_liste;
		fprintf(stderr, "Some errors have been found in the external equipotential list, pad connectors must be on the external side of pads.\n");
		fprintf(stderr, "That's to say, your C file using GENLIB functions may be wrong.\n");
		fprintf(stderr, "The following connector names do not belong to the NORTH side of the figure of pads.\n");

		while (NULL != liste) {
			fprintf(stderr, "- The connector <%s> of the instance pad <%s> isn't placed on the NORTH side.\n",
			     			     (char *)liste->NEXT->NEXT->DATA, (char *)liste->NEXT->DATA);
			fprintf(stderr, "- It isn't possible to link it with the connector <%s> of the circuit.\n", (char
			*)liste->DATA);
			liste = liste->NEXT->NEXT->NEXT;
		}

		break;

	case ERR_TIREALIM:
		fprintf(stderr, "The supply connector <%s> of the pad <%s> cannot be routed, there's no place.\n", ((LST_PSEUDO_CON) pt_liste)->nom_con,
		     		     ((loins_list * )((LST_PSEUDO_CON) pt_liste)->con_lo->ROOT)->INSNAME);
		fprintf(stderr, "You may center this pad, in the pad section of your parameter file .rin.\n");
		break;

	default:
		fprintf(stderr, "Unknown error code: <%d>.\n", code);
	}

	exit(code);
}

/*---------------------------------------------------------------------------------*/
void usage ()
{
    fprintf(stderr, "Usage : ring source result [stat]\n");
    EXIT(1);
}

/*------------------------------------------------------------------------*/
void banner()
{
    alliancebanner("RinG", VERSION, "PAD ring router", "1991", ALLIANCE_VERSION);
}

/*----------------------------------------------------------------------*/
/* cette procedure verifie les equipotentielles internes, c'est a dire  */
/* que tous les connecteurs de plot doivent etre des connnecteurs       */
/* sud du modele physique, sinon ca veut dire qu'il a des equipo        */
/* interdites qui ont ete fabriquees par genlib dans le fichier         */
/* de description  C.                                                   */
/*----------------------------------------------------------------------*/

void verif_eq_interne(LST_EQUIPO lst_equipo, chain_list *liste_plotsph)
{
	chain_list * lst_con, *lsterr = NULL;
	LST_PSEUDO_CON con;

	while (NULL != lst_equipo) {
		lst_con = lst_equipo->lst_con;

		while (NULL != lst_con) {
			con = (LST_PSEUDO_CON) lst_con->DATA;

			if (con->coeur_plot == PLOT_CON) {
				if (!existe_leconnecteur_faceplot(((loins_list * )con->con_lo->ROOT)->FIGNAME, con->con_lo->NAME, SUD, liste_plotsph)) {
					lsterr = addchain(lsterr, (void * ) con->con_lo->NAME);
					lsterr = addchain(lsterr, (void * ) (((loins_list * )con->con_lo->ROOT)->INSNAME));
				}
			}
			lst_con = lst_con->NEXT;
		}
		lst_equipo = lst_equipo->suiv;
	}

	if (NULL != lsterr)
	    ringerreur(ERR_EQINTERNE, (void * )lsterr, (void*) NULL);

}

/* ---------------------------------------------------------------------------------------- */
/* Fonction qui regarde si le modele physique possede des connecteurs sur une certaine face */
/* ---------------------------------------------------------------------------------------- */

int existe_leconnecteur_faceplot(char* figure, char* conname, int face,
	chain_list *liste_plotsph)
{
	phfig_list * modele;
	phcon_list * liste_con;
	char	orient = '0';

	if (mode_debug)
		printf("Existe-t-il le con %s au %d figure %s\n", conname, face, figure);

	switch (face) {
	case NORD:
		orient = 'N';
		break;

	case SUD :
		orient = 'S';
		break;

	case OUEST:
		orient = 'W';
		break;

	case EST :
		orient = 'E';
		break;
	}

	if ((modele = appartient_listeplotsph(figure, liste_plotsph)) != NULL) {

		liste_con = modele->PHCON;

		while (liste_con != NULL) {
			if ((liste_con->NAME == conname) && (liste_con->ORIENT == orient)) {
				if (mode_debug)
					printf("Il existe ok\n.");
				return (1);
			}

			liste_con = liste_con->NEXT;
		}

		if (mode_debug)
			printf("Il n'existe PAS\n.");
		return(0); /* connecteur de la face non trouve */
	}

	if (mode_debug)
		printf("Il n'existe PAS\n.");
	return(0); /* Modele physique non existant ! */

}

/*-------------------------------------------------------------*/
/*  testcon_modelfig()                                         */
/*  check for all instanciated models in logic figure:         */
/*  - if each logical connector has at least one corresponding */
/*  physical connector.                                        */
/*  - if each physical connector has one corresponding         */
/*  logical connector.                                         */
/*-------------------------------------------------------------*/

void testcon_modelfig(lofig_list* ptfiglo)
{
	chain_list * ptcmodel;
	char	*ptmodel;
	lofig_list * ptfigins_lo;
	phfig_list * ptfigins_ph;
	locon_list * ptconlo;
	phcon_list * ptconph;
	char	*loconname;
	char	*phconname;

	for (ptcmodel = ptfiglo->MODELCHAIN; ptcmodel; ptcmodel = ptcmodel->NEXT) {
		ptmodel = (char *)ptcmodel->DATA;
		ptfigins_lo = getlofig(ptmodel, 'P');
		ptfigins_ph = getphfig(ptmodel, 'P');
		for (ptconlo = ptfigins_lo->LOCON; ptconlo; ptconlo = ptconlo->NEXT) {
			loconname = ptconlo->NAME;
			if (isvdd(loconname)) loconname = eq_vdd;
			if (isvss(loconname)) loconname = eq_vss;
			for (ptconph = ptfigins_ph->PHCON; ptconph; ptconph = ptconph->NEXT) {
				phconname = ptconph->NAME;
				if (isvdd(phconname)) phconname = eq_vdd;
				if (isvss(phconname)) phconname = eq_vss;
				if (phconname == loconname) break;
			}
			if (!ptconph) {
				ringerreur(ERR_CONLO_SANS_PH, loconname, ptmodel);
			}
		}
		for (ptconph = ptfigins_ph->PHCON; ptconph; ptconph = ptconph->NEXT) {
			phconname = ptconph->NAME;
			if (isvdd(phconname)) phconname = eq_vdd;
			if (isvss(phconname)) phconname = eq_vss;
			for (ptconlo = ptfigins_lo->LOCON; ptconlo; ptconlo = ptconlo->NEXT) {
				loconname = ptconlo->NAME;
				if (isvdd(loconname)) loconname = eq_vdd;
				if (isvss(loconname)) loconname = eq_vss;
				if (phconname == loconname) break;
			}
			if (!ptconlo) {
				ringerreur(ERR_CONPH_SANS_LO, phconname, ptmodel);
			}
		}
	}
}
