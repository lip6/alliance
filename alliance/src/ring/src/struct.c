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
/*    Fichier : struct.c                                                    */
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
/* RING 15 juillet 92                      STRUCT.C                                  */
/* Fonctions qui manipulent les structures de donnees definies ds struct.h           */
/*-----------------------------------------------------------------------------------*/

#include <string.h>

#include "struct.h"
#include "sesame.h"
#include "placement.h"

/*-----------------------------------------------------------------------------------*/
/* Ajout d'un modele physique de plot dans la liste s'il n'existe pas deja           */
/* Renommage des connecyteurs vdd et vss                                             */
/*-----------------------------------------------------------------------------------*/

void ajout_listeplotsph(modele_plot, liste_plotsph)
char	*modele_plot;
chain_list **liste_plotsph;
{
	static char	first = 1;
	/* first indique que c'est la premiere fois: liste non initialisee */

	if (first) {
		first = 0;
		*liste_plotsph = NULL;
		ignoble_multiple_ck_delete_from_south_side(modele_plot);
		*liste_plotsph = addchain(*liste_plotsph, (void * ) getphfig(modele_plot, 'A'));
		rename_vddxvssx(modele_plot);
	}  else if (appartient_listeplotsph(modele_plot, *liste_plotsph) == NULL) {
		ignoble_multiple_ck_delete_from_south_side(modele_plot);
		*liste_plotsph = addchain(*liste_plotsph, (void * ) getphfig(modele_plot, 'A'));
		rename_vddxvssx(modele_plot);
	}
}

/*-----------------------------------------------------------------------------------*/
/* Cette fonction renomme tous les connecteurs physiques MBK vdd* vss*               */
/* en vdd et vss, ceci pour uniformiser les noms d'alim interne                      */
/*-----------------------------------------------------------------------------------*/

void rename_vddxvssx(modele_plot)
char	*modele_plot;
{
	phfig_list * ptplot = getphfig(modele_plot, 'A');
	phcon_list * ptcon;

	ptcon = ptplot->PHCON;

	while (NULL != ptcon) {
		if ((ptcon->ORIENT != 'S') && (ptcon->ORIENT != 'N'))  
		{
			if (isvdd(ptcon->NAME)) 
				ptcon->NAME = eq_vdd;
			else if (isvss(ptcon->NAME)) 
				ptcon->NAME = eq_vss;
		}
		ptcon = ptcon->NEXT;
	}

}


/*-----------------------------------------------------------------------------------*/
/* this function deletes multiple occurences of ck connector placed at the south     */
/* side. It should be suppressed from the future verions  (02/16/95)                 */
/*-----------------------------------------------------------------------------------*/

void ignoble_multiple_ck_delete_from_south_side(modele_plot)
char	*modele_plot;
{
	phfig_list * ptplot = getphfig(modele_plot, 'A');
	phcon_list * ptcon, * ptcon_next;
       
	int	already_one_alu1 = 0;
	int	already_one_alu2 = 0;

	ptcon = ptplot->PHCON;
	while (NULL != ptcon) {
		ptcon_next = ptcon->NEXT;
                if (mode_debug)
			printf("CONNECTOR %s %ld %ld\n",ptcon->NAME,ptcon->XCON,ptcon->YCON);
		if ((ptcon->ORIENT == 'S') &&  (ptcon->NAME == namealloc("cko"))) {
			if (ptcon->LAYER == ALU1) {
				if (already_one_alu1) {
					delphcon( ptplot, ptcon);
                			if (mode_debug)
						printf("delete %s %ld %ld\n",ptcon->NAME,ptcon->XCON,ptcon->YCON);
				} else {
					already_one_alu1++;
				}
			} else {
				if (ptcon->LAYER == ALU2) {
					if (already_one_alu2) {
						delphcon( ptplot, ptcon);
                				if (mode_debug)
						        printf("delete %s %ld %ld\n",ptcon->NAME,ptcon->XCON,ptcon->YCON);
					} else {
						already_one_alu2++;
					}
				}
			}
		}
		ptcon = ptcon_next;
	}
}

/*------------------------------------------------------------------------------------------*/
/* Fonction qui regarde si le modele physique possede des connecteurs sur une certaine face */
/*------------------------------------------------------------------------------------------*/

int	existe_connecteur_faceplot(figure, face, liste_plotsph)
char	*figure;
int	face;
chain_list *liste_plotsph;
{
	phfig_list * modele;
	phcon_list * liste_con;
	char	orient = 0;

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
			if (liste_con->ORIENT == orient) 
				return (1);
			liste_con = liste_con->NEXT;
		}

		return(0); /* connecteur de la face non trouve */
	}

	return(0); /* Modele physique non existant ! */

}

/*----------------------------------------------------------------------------------------*/
/* cette fonction retourne la figure physique si le modele appartient a la liste, 0 sinon */
/*----------------------------------------------------------------------------------------*/

phfig_list * appartient_listeplotsph(modele, liste)
char	*modele;
chain_list *liste;
{

	while (liste != NULL)  /* strcmp inutile, comparaison de pointeurs */ {
		if (((phfig_list * ) liste->DATA)->NAME == modele)
			return((phfig_list * )liste->DATA);
		liste = liste->NEXT;
	}

	return(NULL);
}

/*-----------------------------------------------------------------------------------*/
/* affichage de la liste des modeles de plots                                        */
/*-----------------------------------------------------------------------------------*/

void affic_listeplotsph(liste)
chain_list *liste;
{
	printf("\n");
	printf("Listeplotsph *****************************\n");
	while (liste != NULL) {
		printf("Plot: %s\n", ((phfig_list * )liste->DATA)->NAME);
		/*	viewphfig((phfig_list*)liste->DATA);  */
		liste = liste->NEXT;
	}

}

/*------------------------------------------------------------------------------------*/
/* cette fonction retourne le pointeur sur l'instance logique si le nom de l'instance */
/* du plot appartient a la figure logique et est un plot , NULL sinon                 */
/*------------------------------------------------------------------------------------*/

loins_list*existe_plot_circuit(nom, circuit_lo)
char	*nom;
lofig_list *circuit_lo;

{
	loins_list * circuit_inst = circuit_lo->LOINS;

	while (circuit_inst != NULL) {
		/* if (mode_debug) printf("nom %s %d instance %s %d\n",nom,(int) nom,circuit_inst->INSNAME,(int)
  			circuit_inst->INSNAME);  
		*/

		if ((nom == circuit_inst->INSNAME) && (incatalog(circuit_inst->FIGNAME)))
			return(circuit_inst);   /* plot et nom d'instance trouve */

		circuit_inst = circuit_inst->NEXT;
	}

	return(NULL);      /* plot avec nom d'instance non trouve */
}

/*-----------------------------------------------------------------------------------*/
/* cette fonction retourne 1 s'il existe une seule occurence du nom plot             */
/* dans le tableau des faces du plots, sinon 0 si plusieurs fois le meme nom         */
/*-----------------------------------------------------------------------------------*/

int	uneseuleoccurence(nom, tab)
char	*nom;
chain_list *tab[NB_FACES];
{

	int	i;
	chain_list * liste;
	char	*nomplot;
	int	cptoccur = 0;

	for (i = 0; i < NB_FACES; i++) {

		liste = tab[i];
		while (liste != NULL) {
			nomplot = (char *)(liste->DATA);

			if (nom == nomplot) 
				cptoccur++; /* nombre d'occurrence du nom */
			liste = liste->NEXT;
		}

	}

	if (cptoccur == 1) 
		return(1); /*  une seule occurence trouvee */

	return(0);                  /*  plusieurs occurences du meme nom */

}

/*--------------------------------------------------------------------------------------*/
/* cette fonction rend 1 si le nom correspond a un connecteur du coeur ou des instances */
/* plots. Si le nom n'existe pas retourne 0. On travaille avec les vues logiques coeur  */
/* et circuit                                                                           */
/*--------------------------------------------------------------------------------------*/

int	existe_signal_circuit(cell, circuit_lo, lecoeur)
chain_list *cell;
lofig_list *circuit_lo;
COEUR      lecoeur;
{

	locon_list * coeur_con = (lecoeur.coeur_lo)->LOCON;
	loins_list * circuit_instlo = circuit_lo->LOINS;
	locon_list * liste_con;
	char	*nom;

	nom = ((PTCEL) cell->DATA)->chaine;

	/* ------------------------ */
	/* On commence par le coeur */
	/* ------------------------ */

	while (coeur_con != NULL) {
		if (nom == coeur_con->NAME) {
			((PTCEL)cell->DATA)->con_lo = coeur_con;  /* pointeur sur connecteur physique */
			return(1);   /* connecteur trouve */
		}

		coeur_con = coeur_con->NEXT;
	}

	while (circuit_instlo != NULL) {
		liste_con = circuit_instlo->LOCON;

		while (liste_con != NULL) {
			if (nom == liste_con->NAME) {
				((PTCEL)cell->DATA)->con_lo = liste_con;
				return(1);  /* connecteur trouve */
			}

			liste_con = liste_con->NEXT;
		}
		circuit_instlo = circuit_instlo->NEXT;
	}

	return(0);  /* le connecteur correspondant n'a pas ete trouve */

}

/*-----------------------------------------------------------------------------------*/
/* procedure appellee par yacc, declaration de plots face nord, Tableau de listes    */
/* de chaine de caracteres declaree en globale                                       */
/*-----------------------------------------------------------------------------------*/

void declaration_plot_n(ident)
char	*ident;
{
	static int	first = 1;

	if (mode_debug) 
		printf("declaration plot nord\n");

	if (first) {
		first = 0;
		nom_plot[NORD] = NULL;
	}

	nom_plot[NORD] = addchain(nom_plot[NORD], (void * ) namealloc(ident));
}

/*-----------------------------------------------------------------------------------*/
/* procedure appellee par yacc, declaration de plots face sud, Tableau de listes     */
/* de chaine de caracteres declaree en globale                                       */
/*-----------------------------------------------------------------------------------*/

void declaration_plot_s(ident)
char	*ident;
{
	static int	first = 1;

	if (mode_debug) 
		printf("declaration plot sud\n");
	if (first) {
		first = 0;
		nom_plot[SUD] = NULL;
	}

	nom_plot[SUD] = addchain(nom_plot[SUD], (void * ) namealloc(ident));
}

/*-----------------------------------------------------------------------------------*/
/* procedure appellee par yacc, declaration de plots face west, Tableau de listes    */
/* de chaine de caracteres declaree en globale                                       */
/*-----------------------------------------------------------------------------------*/

void declaration_plot_w(ident)
char	*ident;
{
	static int	first = 1;

	if (mode_debug) 
		printf("declaration plot west\n");

	if (first) {
		first = 0;
		nom_plot[OUEST] = NULL;
	}

	nom_plot[OUEST] = addchain(nom_plot[OUEST], (void * ) namealloc(ident));
}

/*-----------------------------------------------------------------------------------*/
/* procedure appellee par yacc, declaration de plots face est, Tableau de listes     */
/* de chaine de caracteres declaree en globale                                       */
/*-----------------------------------------------------------------------------------*/

void declaration_plot_e(ident)
char	*ident;
{
	static int	first = 1;

	if (mode_debug) 
		printf("declaration plot est\n");

	if (first) {
		first = 0;
		nom_plot[EST] = NULL;
	}

	nom_plot[EST] = addchain(nom_plot[EST], (void * ) namealloc(ident));
}

/*-----------------------------------------------------------------------------------*/
/* procedure appellee par yacc, declaration de plots dont la taille des connecteurs  */
/* est speciale , liste_width variable globale: liste de cellules contenant nom      */
/* et largeur connecteur                                                             */
/*-----------------------------------------------------------------------------------*/

void declaration_width(ident, largeur)
char	*ident;
long	largeur;
{
	static int	first = 1;
	PTCEL cellule;

	if (mode_debug) 
		printf("declaration width \n");
	if (mode_debug) 
		printf("ident --%s-- width --%ld--\n", ident, (long) largeur);
	if (first) {
		first = 0;
		liste_width = NULL;
	}

	cellule = (PTCEL) mbkalloc ((unsigned int) sizeof(CEL));
	cellule->chaine = namealloc(ident);
	cellule->largeur   = (long) largeur;

	liste_width = addchain(liste_width, (void * ) cellule);
}

/*-----------------------------------------------------------------------------------*/
/* affichages des listes remplies par l'analyseur syntaxique                         */
/*-----------------------------------------------------------------------------------*/

void affic_plotwidth()
{
	chain_list * liste;
	int	i;

	if (mode_debug) 
		printf("tableau nom des  instances plots\n");

	for (i = 0; i < NB_FACES; i++) {
		if (mode_debug) 
			printf("---------numero face :%d\n", i);

		liste = nom_plot[i];
		while (liste != NULL) {

			/* ---------------------------------------------------------------- */
			/* chain_list modifiee pointe sur des instances logiques maintenant */
			/* ---------------------------------------------------------------- */

			if (mode_debug) 
				printf("nom plot: --%s--\n", ((loins_list * )liste->DATA)->INSNAME);
			liste = liste->NEXT;
		}

	}

	liste = liste_width;

	if (mode_debug) 
		printf("Liste width\n");

	while (liste != NULL) {
		if (mode_debug) 
			printf("Connecteur --%s-- largeur %ld\n", ((PTCEL)(liste->DATA))->chaine, ((PTCEL)(liste->DATA))->largeur);
		liste = liste->NEXT;
	}
}

/*-----------------------------------------------------------------------------------*/
/* Cette fonction reoriente la direction du connectueur en fonction de la rotation   */
/*  transmise.                                                                       */
/*-----------------------------------------------------------------------------------*/

int	reorientation_con(orientation, rotation)
char	orientation;
char	rotation;
{
	int	face = 0;

	switch (rotation) {
	case NOSYM :
		switch (orientation) {
		case 'N' :
			face = NORD;
			break;
		case 'E' :
			face = EST;
			break;
		case 'S' :
			face = SUD;
			break;
		case 'W' :
			face = OUEST;
			break;
		}
		break;

	case ROT_P :
		switch (orientation) {
		case 'N' :
			face = OUEST;
			break;
		case 'E' :
			face = NORD;
			break;
		case 'S' :
			face = EST;
			break;
		case 'W' :
			face = SUD;
			break;
		}
		break;
	case SYMXY :
		switch (orientation) {
		case 'N' :
			face = SUD;
			break;
		case 'E' :
			face = OUEST;
			break;
		case 'S' :
			face = NORD;
			break;
		case 'W' :
			face = EST;
			break;
		}
		break;
	case ROT_M :
		switch (orientation) {
		case 'N' :
			face = EST;
			break;
		case 'E' :
			face = SUD;
			break;
		case 'S' :
			face = OUEST;
			break;
		case 'W' :
			face = NORD;
			break;
		}
		break;
	case SYM_X :
		switch (orientation) {
		case 'N' :
			face = NORD;
			break;
		case 'E' :
			face = OUEST;
			break;
		case 'S' :
			face = SUD;
			break;
		case 'W' :
			face = EST;
			break;
		}
		break;
	case SY_RM :
		switch (orientation) {
		case 'N' :
			face = OUEST;
			break;
		case 'E' :
			face = SUD;
			break;
		case 'S' :
			face = EST;
			break;
		case 'W' :
			face = NORD;
			break;
		}
		break;
	case SYM_Y :
		switch (orientation) {
		case 'N' :
			face = SUD;
			break;
		case 'E' :
			face = EST;
			break;
		case 'S' :
			face = NORD;
			break;
		case 'W' :
			face = OUEST;
			break;
		}
		break;
	case SY_RP :
		switch (orientation) {
		case 'N' :
			face = EST;
			break;
		case 'E' :
			face = NORD;
			break;
		case 'S' :
			face = OUEST;
			break;
		case 'W' :
			face = SUD;
			break;
		}
		break;
	}

	return(face);
}

/*-----------------------------------------------------------------------------------*/
/* recherche d'un connecteur logique d'alim dans un liste d'equipo                   */
/*-----------------------------------------------------------------------------------*/

locon_list*vraifaux_conlogic(lst_con)
chain_list *lst_con;
{
	LST_PSEUDO_CON con;

	while (lst_con != NULL) {
		con = (LST_PSEUDO_CON) lst_con->DATA;
		if ((con->coeur_plot == COEUR_CON) && (con->con_lo != NULL))
			return (con->con_lo);

		lst_con = lst_con->NEXT;
	}

	if (mode_debug) 
		printf("\nWarning pas de connecteur alim logique coeur dans liste des equipo ?!\n");
	return(NULL);
}

/*-----------------------------------------------------------------------------------*/
/* 15 juilet bug corrige: rech con logique avant d'inserer le pseudo con dans le     */
/* tableau des con du coeur                                                          */
/*                                                                                   */
/* ajout d'un pseudo connecteur avec ses principales caracteristiques physiques      */
/* dans un tableau de listes. Une liste par face et soit coeur, soit plot.           */
/* Modification: si l'on trouve  des connecteurs coeurs en l'air de type             */
/* vdd.x ou vss.x, on change le nom dans la structure mbk et                         */
/* on ajoute ces connecteurs dans  la  liste des equipotentielles.                   */
/* nb: il existe 2 pseudo connecteurs qui existent deja qu'on utilise pour les       */
/* premier vdd et vss trouves                                                        */
/*-----------------------------------------------------------------------------------*/

void ajout_pseudo_con_coeur(x, y, liste_con, face, tab_coeur, lst_equipo)
long	x;
long	y;
phcon_list *liste_con;
int	face;
LST_PSEUDO_CON tab_coeur[NB_FACES];
LST_EQUIPO lst_equipo;
{
	long	width = liste_con->WIDTH;
	char	layer = liste_con->LAYER, *name = liste_con->NAME;

	static int	first = 1;
	static int	firstvdd = 1, firstvss = 1;  /* premiere fois ou l'on  rencontre ces eq */
	LST_PSEUDO_CON con, new_con, liste;
	LST_EQUIPO     equipo_vdd, equipo_vss, new_lst_equipo;
	locon_list     * con_lo;
	int	trouve = 0, i;

	/* --------------------------------------------------------------------------- */
	/* Les pseudo connecteurs ont deja ete alloues par la procedure de fabrication */
	/* des equipotentielles. Il suffit de retrouver le connecteur et de le chainer */
	/* dans la liste des pseudo connecteurs                                        */
	/* --------------------------------------------------------------------------- */

	if (first) {
		for (i = 0; i < NB_FACES; i++) 
			tab_coeur[i] = NULL;
		first = 0;
	}

	if (mode_debug) 
		printf("nom connecteur %s larg %ld face %d x %ld y %ld\n", name, width, face, x, y);

	if (((con = existe_concoeur_lstequipo(name, lst_equipo)) == NULL) /*  n'existe pas */ 
	|| (name == eq_vdd) || (name == eq_vss)) /* ajoute pour vti */
	{

		/* --------------------------------------- */
		/* Connecteur en l'air                     */
		/* on verifie si il s'agit d'un vdd ou vss */
		/* --------------------------------------- */

		if (!strncmp(name, eq_vdd, 3)) /* vddxx reconnu */ {
			if (mode_debug) 
				printf("vddxxxx nom connecteur %s larg %ld face %d\n", name, width, face);

			recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

			liste_con->NAME = eq_vdd;
			con_lo     = vraifaux_conlogic(equipo_vdd->lst_con);

			if (firstvdd) { /* on utilise la cellule deja cree */
				if (mode_debug) 
					printf("firstvdd\n");
				con = (LST_PSEUDO_CON) (equipo_vdd->lst_con)->DATA;
				firstvdd = 0;
			}  else /* creation d'un nouveau pseudo connecteur */			 {
				con = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
				alloue_coord(0L, 0, &(con->coord));

				/* --------------------- */
				/* chainage du con_coeur */
				/* --------------------- */

				equipo_vdd->lst_con = addchain(equipo_vdd->lst_con, (void * ) con);
			}

			con->coeur_plot = COEUR_CON;
			con->nom_con    = liste_con->NAME;

			/* ------------------------------------------------------------------------------ */
			/* Il n'existe pas de connecteur logique associe au con physique, mais on prendi  */
			/* celui du coeur existant , pour que les structures de donnees soient coherentes */
			/* ------------------------------------------------------------------------------ */

			con->con_lo     = con_lo;

		} else 
		if (!strncmp(name, eq_vss, 3)) /* connecteur vssxxx reconnu */ {
			if (mode_debug) 
				printf("vssxxx nom connecteur %s larg %ld face %d\n", name, width, face);

			recherche_equipo_alim(&equipo_vdd, &equipo_vss, lst_equipo);

			liste_con->NAME = eq_vss;
			con_lo     = vraifaux_conlogic(equipo_vss->lst_con);

			if (firstvss) { /* on utilise la cellule deja cree */
				if (mode_debug) 
					printf("firstvss\n");
				con = (LST_PSEUDO_CON) (equipo_vss->lst_con)->DATA;
				firstvss = 0;
			}  else /* creation d'un nouveau pseudo connecteur */			 {
				con = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
				alloue_coord(0L, 0, &(con->coord));

				/* --------------------- */
				/* chainage du con_coeur */
				/* --------------------- */

				equipo_vss->lst_con = addchain(equipo_vss->lst_con, (void * ) con);
			}

			con->coeur_plot = COEUR_CON;
			con->nom_con    = liste_con->NAME;

			/* ------------------------------------------------------------------------------ */
			/* Il n'existe pas de connecteur logique associe au con physique, mais on prend   */
			/* celui du coeur existant , pour que les structures de donnees soient coherentes */
			/* ------------------------------------------------------------------------------ */

			con->con_lo     = con_lo;

		}

		else
			ringerreur(ERR_CONCOEUR_ENLAIR, name, NULL);
	}

	if ((con->coord)->proprio == NULL) { /* Caracteristiques physiques completees */
		(con->coord)->xabs = x;
		(con->coord)->yabs = y;
		(con->coord)->suiv = NULL;
		(con->coord)->prec = NULL;
		(con->coord)->proprio = (void * ) con;
		con->deport     = NULL;
		con->face       = face;
		con->deport     = NULL;
		con->largeur    = width;
		con->layer      = layer;
		con->suiv       = NULL;
		con->prec       = NULL;
	} else {
		printf("Connecteurs multiples de nom %s sur le coeur\n", con->nom_con);
		new_con = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
    new_lst_equipo= (LST_EQUIPO) existe_concoeur_lstequipo2(name, lst_equipo);
    new_lst_equipo->lst_con = addchain(new_lst_equipo->lst_con, (void * ) new_con);
		alloue_coord(0L, 0, &(new_con->coord));
		(new_con->coord)->xabs = x;
		(new_con->coord)->yabs = y;
		(new_con->coord)->suiv = NULL;
		(new_con->coord)->prec = NULL;
		(new_con->coord)->proprio = (void * ) new_con;
		 new_con->deport     = NULL;
		 new_con->face       = face;
		 new_con->deport     = NULL;
		 new_con->largeur    = width;
		 new_con->layer      = layer;
		 new_con->suiv       = NULL;
		 new_con->prec       = NULL;
		 new_con->con_lo     = con->con_lo;
     new_con->coeur_plot = con->coeur_plot;
     new_con->nom_con    = con->nom_con;
     new_con->situe_deport = con->situe_deport;
     con=new_con;
	}
		
	if (mode_debug) 
		printf("INSNAME %s face %d ptcon %d\n", ((loins_list * ) (con->con_lo)->ROOT)->INSNAME, (int)face, (int)con);

	if (tab_coeur[face] == NULL) {
		if (mode_debug) 
			printf("Premier passage liste %d vide\n", face);
		tab_coeur[face] = con;
	}  else {
		liste = tab_coeur[face];

		switch (face) {
		case NORD :
		case SUD  :
			while (liste->suiv != NULL) {
				if ((trouve = x < (liste->coord)->xabs)) {
					inserer_avant_lstcon(con, liste, &(tab_coeur[face]));
					break;
				}

				liste = liste->suiv;
			}

			if (!trouve) {
				if (x < (liste->coord)->xabs)
					inserer_avant_lstcon(con, liste, &(tab_coeur[face]));
				else 
					inserer_apres_lstcon(con, liste);
			}

			break;

		case EST  :
		case OUEST:
			while (liste->suiv != NULL) {
				if ((trouve = y < (liste->coord)->yabs)) {
					inserer_avant_lstcon(con, liste, &(tab_coeur[face]));
					break;
				}

				liste = liste->suiv;
			}

			if (!trouve) {
				if (y < (liste->coord)->yabs)
					inserer_avant_lstcon(con, liste, &(tab_coeur[face]));
				else 
					inserer_apres_lstcon(con, liste);
			}

			break;

		}
	}
}

/*--------------------------------------------------------------------------------*/
/* ajout d'un pseudo connecteur avec ses principales caracteristiques physiques   */
/*  dans un tableau de listes. Une liste par face et  plot.                       */
/*--------------------------------------------------------------------------------*/

void ajout_pseudo_con_plot(x, y, width, layer, name, face, deport, tab_plots, lst_equipo)
long	x;
long	y;
long	width;
char	layer;
int	deport;
char	*name;
int	face;
BARRE_PLOTS tab_plots[NB_FACES];
LST_EQUIPO lst_equipo;
{

	static int	first = 1;
	LST_PSEUDO_CON con, liste;
	int	trouve = 0, i;
	long	aux;

	/* --------------------------------------------------------------------------- */
	/* Les pseudo connecteurs ont deja ete alloues par la procedure de fabrication */
	/* des equipotentielles. Il suffit de retrouver le connecteur et de le chainer */
	/* dans la liste des pseudo connecteurs                                        */
	/* --------------------------------------------------------------------------- */

	if (first) {
		for (i = 0; i < NB_FACES; i++) 
			tab_plots[i].lst_con = NULL;
		first =  0;
	}

	if ((con = existe_conplot_lstequipo(name, lst_equipo)) == NULL)
		ringerreur(ERR_CONPLOT_ENLAIR, name, NULL);

	/* ------------------------------------- */
	/* Caracteristiques physiques completees */
	/* ------------------------------------- */

	if (mode_debug) 
		printf("INSNAME %s\n", ((loins_list * ) (con->con_lo)->ROOT)->INSNAME);

	if ((EST == face) || (OUEST == face)) { 
		(con->coord)->xabs = y; 
		(con->coord)->yabs = x; 
		aux = x; 
		x = y; 
		y = aux;
		/* echange des coordonnees */ 	} else	 { 
		(con->coord)->xabs = x; 
		(con->coord)->yabs = y; 
	}

	(con->coord)->suiv = NULL;
	(con->coord)->prec = NULL;
	(con->coord)->proprio = (void * ) con;
	con->face       = face;
	if (!deport) 
		con->deport     = NULL;  /* sinon deport deja alloue ds remplir_tabplots() */
	con->largeur    = width;
	con->layer      = layer;
	con->suiv       = NULL;
	con->prec       = NULL;

	if (con->nom_con == eq_vdd)          /* mise a jour de vdd et vss width */
		vdd_width = width;

	else if (con->nom_con == eq_vss)
		vss_width = width;

	if (mode_debug) 
		printf("plot con %s x %ld y %ld larg %ld\n", con->nom_con, (con->coord)->xabs, (con->coord)->yabs, con->largeur);

	/* ----------------------------------------------------------------- */
	/* Les barres de plots physiques ont ete construites sans rotation,  */
	/* mais pour les barres est et ouest on prend les y en consideration */
	/* ----------------------------------------------------------------- */

	if (tab_plots[face].lst_con == NULL) {
		if (mode_debug) 
			printf("Premier passage liste %d vide\n", face);
		tab_plots[face].lst_con = con;
	}  else
	 {
		if (mode_debug) 
			printf("trouve %s par pseudo %s\n", name, con->nom_con);
		liste = tab_plots[face].lst_con;

		switch (face) {
		case NORD:
		case SUD:
			while (liste->suiv != NULL) {
				if ((trouve = x < (liste->coord)->xabs)) {
					inserer_avant_lstcon(con, liste, &(tab_plots[face].lst_con));
					break;
				}

				liste = liste->suiv;
			}

			if (!trouve) {
				if (x < (liste->coord)->xabs)
					inserer_avant_lstcon(con, liste, &(tab_plots[face].lst_con));
				else 
					inserer_apres_lstcon(con, liste);
			}
			break;

		case OUEST:
		case EST:
			while (liste->suiv != NULL) {
				if ((trouve = y < (liste->coord)->yabs)) {
					inserer_avant_lstcon(con, liste, &(tab_plots[face].lst_con));
					break;
				}

				liste = liste->suiv;
			}

			if (!trouve) {
				if (y < (liste->coord)->yabs)
					inserer_avant_lstcon(con, liste, &(tab_plots[face].lst_con));
				else 
					inserer_apres_lstcon(con, liste);
			}
			break;
		} /* fin du switch */

	} /* Fin du else  */
}

/*-----------------------------------------------------------------------------------*/

void inserer_apres_lstcon(con, pt)
LST_PSEUDO_CON con;
LST_PSEUDO_CON pt;
{

	LST_PSEUDO_CON ptsuiv;

	if (pt == NULL)
		ringerreur(ERR_INTERNE_CON, NULL, NULL);

	ptsuiv = pt->suiv;

	/* ------------ */
	/* fin de liste */
	/* ------------ */

	if (ptsuiv == NULL) {
		pt->suiv = con;
		con->prec = pt;
	}  else
	 {
		pt->suiv = con;
		con->prec = pt;
		con->suiv = ptsuiv;
		ptsuiv->prec = con;
	}

}

/*-----------------------------------------------------------------------------------*/

void inserer_avant_lstcon(con, pt, liste)
LST_PSEUDO_CON con;
LST_PSEUDO_CON pt;
LST_PSEUDO_CON *liste;
{

	LST_PSEUDO_CON ptprec;

	if ((pt == NULL) || ((*liste) == NULL))
		ringerreur(ERR_INTERNE_CON, NULL, NULL);

	/* -------------------------------------------- */
	/* si pt==null ou *liste ==null ringerreur      */
	/* insertion dans l'ordre ptprec <-> con <-> pt */
	/* -------------------------------------------- */

	ptprec = pt->prec;  /* cellule precedente */

	/* ------------------------------------------ */
	/* si ptprec == null changement tete de liste */
	/* ------------------------------------------ */

	if (ptprec == NULL) {
		con->suiv = *liste;
		(*liste)->prec = con;
		(*liste) = con;
	}  else {
		con->suiv = pt;
		con->prec = ptprec;
		ptprec->suiv = con;
		pt->prec  = con;
	}

}

/*-----------------------------------------------------------------------------------*/

void affic_lstcon(liste)
LST_PSEUDO_CON liste;
{
	LST_PSEUDO_CON fin_liste;

	while (liste != NULL) {
		if (liste->coeur_plot == COEUR_CON)
			printf("COEUR *** ");
		else 
			printf("PLOT ***");

		printf("Con %20s face %d xabs %5ld yabs %5ld largeur %3ld layer %d root %10s\n", liste->nom_con, liste->face,
		     liste->coord->xabs, liste->coord->yabs, liste->largeur, (int) liste->layer, ((loins_list * )liste->con_lo->ROOT)->INSNAME);

		if (liste->suiv == NULL) 
			fin_liste = liste;
		liste = liste->suiv;
	}

	liste = fin_liste;
	/*
	printf("Parcours de la meme liste en sens inverse\n");
	
	while (liste!= NULL) {
  		if (liste->coeur_plot == COEUR_CON)
			printf("COEUR *** ");
		else printf("PLOT ***");
	
		printf("Con %s face %d xabs %ld yabs %ld largeur %d layer %d\n",
		liste->nom_con, liste->face, liste->coord->xabs, liste->coord->yabs, liste->largeur,(int) liste->layer);

  		liste= liste->prec;
	}
	*/
}

/*-----------------------------------------------------------------------------------*/
/* Recherche d'un connecteur du coeur dans la liste des equipo. Le connecteur coeur  */
/* est toujous en tete d'equipo. Modification: il faut considerer qu'il peut         */
/* avoir plusieurs connecteurs du coeur qui portent le meme signal; alors            */
/* l'equipo contient +sieurs concoeur, rend la liste d'equipotentielle               */
/* ou le connecteur a ete trouve                                                     */
/*-----------------------------------------------------------------------------------*/

LST_EQUIPO existe_concoeur_lstequipo2(name, lst_equipo)
char	*name;
LST_EQUIPO lst_equipo;
{
	LST_PSEUDO_CON pseudo_con;
	chain_list     * lst_con;

	while (lst_equipo != NULL) {
		lst_con = lst_equipo->lst_con;

		while (lst_con != NULL) {
			pseudo_con = (LST_PSEUDO_CON) lst_con->DATA;
			if ((pseudo_con->coeur_plot == COEUR_CON) && (pseudo_con->nom_con == name))
				return( lst_equipo);

			lst_con = lst_con->NEXT;
		}

		lst_equipo = lst_equipo->suiv;
	}

	return(NULL);
}

/*-----------------------------------------------------------------------------------*/
/* Recherche d'un connecteur coeur dans la liste des equipo. Le connecteur coeur     */
/* est toujous en tete d'equipo. Modification: il faut considerer qu'il peut         */
/* avoir plusieurs connecteurs du coeur qui portent le meme signal; alors            */
/* l'equipo contient +sieurs concoeur                                                */
/*-----------------------------------------------------------------------------------*/

LST_PSEUDO_CON existe_concoeur_lstequipo(name, lst_equipo)
char	*name;
LST_EQUIPO lst_equipo;
{
	LST_PSEUDO_CON pseudo_con;
	chain_list     * lst_con;

	while (lst_equipo != NULL) {
		lst_con = lst_equipo->lst_con;

		while (lst_con != NULL) {
			pseudo_con = (LST_PSEUDO_CON) lst_con->DATA;
			/*
     			if (mode_debug) printf("existeconcoeur equiponom_con %s name %s\n",pseudo_con->nom_con, name);
			*/
			if ((pseudo_con->coeur_plot == COEUR_CON) && (pseudo_con->nom_con == name))
				return(pseudo_con);

			lst_con = lst_con->NEXT;
		}

		lst_equipo = lst_equipo->suiv;
	}

	return(NULL);
}

/*---------------------------------------------------------------------------------*/
/* Recherche d'un connecteur logique plot dans la liste des equipo. par son nom    */
/* d'instance et son nom de connecteur                                             */
/*---------------------------------------------------------------------------------*/

LST_PSEUDO_CON existe_conlogicplot_lstequipo(conlo, lst_equipo)
locon_list *conlo;
LST_EQUIPO lst_equipo;
{
	char	*nom_inst, *nom_inst2;
	LST_PSEUDO_CON pseudo_con;
	chain_list     * lst_con;

	nom_inst2 = ((loins_list * )conlo->ROOT)->INSNAME;

	while (lst_equipo != NULL) {
		lst_con = lst_equipo->lst_con;

		while (lst_con != NULL) {
			pseudo_con = (LST_PSEUDO_CON) lst_con->DATA;

			if (pseudo_con->coeur_plot == PLOT_CON) {

				nom_inst = ((loins_list * )(pseudo_con->con_lo)->ROOT)->INSNAME;

				if (mode_debug) 
					printf("nom_instance %s name %s nominst2 %s name2 %s\n", nom_inst, pseudo_con->nom_con,
					     nom_inst2, conlo->NAME);

				if ((nom_inst == nom_inst2) && (pseudo_con->nom_con == conlo->NAME))
					return(pseudo_con);
			}

			lst_con = lst_con->NEXT;

		} /* fin de la liste coeurante d'equipo */

		lst_equipo = lst_equipo->suiv;
	}

	return(NULL);
}

/*-----------------------------------------------------------------------------------*/
/* Recherche d'un connecteur plot dans la liste des equipo. avec concatenation       */
/* du nom d'instance                                                                 */
/*-----------------------------------------------------------------------------------*/

LST_PSEUDO_CON existe_conplot_lstequipo(name, lst_equipo)
char	*name;
LST_EQUIPO lst_equipo;
{
	char	*nom_con;
	LST_PSEUDO_CON pseudo_con;
	chain_list     * lst_con;

	while (lst_equipo != NULL) {
		lst_con = lst_equipo->lst_con;

		while (lst_con != NULL) {

			/* ----------------------------------------------------------------------- */
			/* on concatene le nom d'instance avec le nom du connecteur separe         */
			/* par MBK_SEPAR, pointeur rendu est dans la hash table, pour comparaison  */
			/* avec nom dans la figure de la barre de plots                            */
			/* ----------------------------------------------------------------------- */

			pseudo_con = (LST_PSEUDO_CON) lst_con->DATA;

			if (pseudo_con->coeur_plot == PLOT_CON) {

				nom_con = concatname(((loins_list * )(pseudo_con->con_lo)->ROOT)->INSNAME, pseudo_con->nom_con);

				/*
				if (mode_debug) 
					printf("EXISTE PLOT ? nom_con %s name %s***  %d ***** %d\n ",nom_con, name, (int) nom_con, (int) name); 
				*/

				if (nom_con == name)
					return(pseudo_con);
			}

			lst_con = lst_con->NEXT;

		} /* fin de la liste coeurante d'equipo */

		lst_equipo = lst_equipo->suiv;
	}

	return(NULL);
}

/*-----------------------------------------------------------------------------------*/
/* Mise a jour de la liste des eq avec les connecteurs plots ou coeur                */
/*-----------------------------------------------------------------------------------*/

void maj_equipo(conlo1, conlo2, lst_equipo, type)
locon_list *conlo1;
locon_list *conlo2;
LST_EQUIPO *lst_equipo;
int	type;
{
	static int	first = 1;
	LST_EQUIPO eq, pt_eq;
	LST_PSEUDO_CON con1, con2;

	int	type1 = 0, type2 = 0;
	LST_PSEUDO_CON ret1 = NULL, ret2 = NULL;

	if (mode_debug) 
		printf("Maj equipo\n");

	switch (type) {
	case COEUR_PLOT: 
		type1 = COEUR_CON;
		type2 = PLOT_CON;
		break;
	case COEUR_COEUR: 
		type1 = COEUR_CON;
		type2 = COEUR_CON;
		break;
	case PLOT_PLOT: 
		type1 = PLOT_CON;
		type2 = PLOT_CON;
		break;
	}

	if (first) {
		(*lst_equipo) = NULL;
		first = 0;
	}

	if ((pt_eq = existe_sig_equipo((*lst_equipo), (conlo1->SIG)->INDEX)) == NULL) {

		/* -------------------------- */
		/* creation d'une nouvelle eq */
		/* -------------------------- */

		if (mode_debug) 
			printf("Nouvelle equipo index %ld con_coeur %s con_plot %s\n", (conlo1->SIG)->INDEX, conlo1->NAME,
			     conlo2->NAME);

		eq          = (LST_EQUIPO) mbkalloc ((unsigned int) sizeof (EQUIPO));
		eq->index   = (conlo1->SIG)->INDEX;
		eq->lst_con = NULL;
		eq->lst_seg = NULL;
		eq->lst_visavis = NULL;
		(eq->cour).lst_projetes = NULL;

		/* bug corrige, apres le malloc ! */

		if (mode_stat) { 
			eq->status.lg_a1 = 0; 
			eq->status.lg_a2 = 0; 
			eq->status.su_a1 = 0; 
			eq->status.su_a2 = 0; 
			eq->status.nb_vias = 0;
		}

		if ((conlo1->NAME == eq_vdd) || (conlo1->NAME == eq_vss))
			eq->type = ALIM;
		else 
			eq->type = type;

		/* ------------------------- */
		/* creation pseudo_con coeur */
		/* ------------------------- */

		con1 = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
		alloue_coord(0L, 0, &(con1->coord));
		con1->deport     = NULL;
		con1->coeur_plot = type1;
		con1->nom_con    = conlo1->NAME;
		con1->con_lo     = conlo1;
		con1->largeur    = 0;
		con1->suiv       = NULL;
		con1->prec       = NULL;

		/* ------------------------- */
		/* creation pseudo_con plot  */
		/* ------------------------- */

		con2 = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
		alloue_coord(0L, 0, &(con2->coord));
		con2->deport     = NULL;
		con2->coeur_plot = type2;
		con2->nom_con    = conlo2->NAME;
		con2->con_lo     = conlo2;
		con2->largeur    = 0;
		con2->suiv       = NULL;
		con2->prec       = NULL;

		/* --------------------- */
		/* chainage du con_coeur */
		/* --------------------- */
		eq->lst_con = addchain(eq->lst_con, (void * ) con1);

		/* -------------------- */
		/* chainage du con_plot */
		/* -------------------- */
		eq->lst_con = addchain(eq->lst_con, (void * ) con2);

		/* -------------------------------- */
		/* chainage avant de la nouvelle eq */
		/* -------------------------------- */

		eq->suiv = (*lst_equipo);

		(*lst_equipo) = eq;
	}   else {

		/* ---------------------------------------------------------------------------- */
		/* Modification: il faut considerer qu'il peut avoir plusieurs connecteurs      */
           	/* du coeur qui portent le meme signal; alors l'equipo contient +sieurs concoeur*/
		/* ---------------------------------------------------------------------------- */

		switch (type) {
		case COEUR_PLOT:
			ret1 = existe_concoeur_lstequipo(conlo1->NAME, pt_eq);
			ret2 = existe_conlogicplot_lstequipo(conlo2, pt_eq);
			break;

		case COEUR_COEUR:
			ret1 = existe_concoeur_lstequipo(conlo1->NAME, pt_eq);
			ret2 = existe_concoeur_lstequipo(conlo2->NAME, pt_eq);
			break;

		case PLOT_PLOT:
			ret1 = existe_conlogicplot_lstequipo(conlo1, pt_eq);
			ret2 = existe_conlogicplot_lstequipo(conlo2, pt_eq);
			break;
		}

		if (ret1 == NULL) {  /* N'existe pas */

			if (mode_debug) 
				printf("Deuxieme con coeur dans l'equipo %s \n", conlo1->NAME);

			/* ------------------------- */
			/* creation pseudo_con coeur */
			/* ------------------------- */

			con1 = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
			alloue_coord(0L, 0, &(con1->coord));
			con1->deport     = NULL;
			con1->coeur_plot = type1;
			con1->nom_con    = conlo1->NAME;
			con1->con_lo     = conlo1;
			con1->largeur    = 0;
			con1->suiv       = NULL;
			con1->prec       = NULL;

			/* --------------------- */
			/* chainage du con_coeur */
			/* --------------------- */

			pt_eq->lst_con = addchain(pt_eq->lst_con, (void * ) con1);
		}

		if (mode_debug) 
			printf("L'equipo index %ld conplot %s existe deja\n", (conlo1->SIG)->INDEX, conlo2->NAME);

		if (ret2 == NULL) {  /* N'existe pas */

			/* ------------------------ */
			/* creation pseudo_con plot */
			/* ------------------------ */

			con2 = (LST_PSEUDO_CON) mbkalloc ((unsigned int) sizeof (PSEUDO_CON));
			alloue_coord(0L, 0, &(con2->coord));
			con2->deport     = NULL;
			con2->coeur_plot = type2;
			con2->nom_con    = conlo2->NAME;
			con2->con_lo     = conlo2;
			con2->largeur    = 0;
			con2->suiv       = NULL;
			con2->prec       = NULL;

			/* -------------------- */
			/* chainage du con_plot */
			/* -------------------- */

			pt_eq->lst_con = addchain(pt_eq->lst_con, (void * ) con2);
		}

	}  /* fin du else */

}

/*-----------------------------------------------------------------------------------*/
/* Fonction retourne l'equi si signal existe deja NULL sinon                         */
/*-----------------------------------------------------------------------------------*/

LST_EQUIPO existe_sig_equipo(lst_equipo, index)
LST_EQUIPO lst_equipo;
long	index;
{

	while (lst_equipo != NULL) {
		if (lst_equipo->index == index)
			return(lst_equipo);

		lst_equipo = lst_equipo->suiv;
	}

	return(NULL);

}

/*-----------------------------------------------------------------------------------*/

void affic_equipo(lst_equipo)
LST_EQUIPO lst_equipo;
{
	LST_PSEUDO_CON con;
	chain_list * liste;
	ptype_list * listevav, *lst_proj;
	chain_list * listeseg;
	LST_SEGMENT lseg;
	int	cpt_equipo = 0;
	static char	a1[3] = "A1";
	static char	a2[3] = "A2";
	static char	er[3] = "ER";
	char	*metal;
	char	car = 0;

	if (mode_debug) 
		printf("EQUIPO AFFIC\n");

	while (lst_equipo != NULL) {
		printf("\n\t------\n");
		printf("\tEQUIPO type %d index %5ld\n", lst_equipo->type, lst_equipo->index);
		cpt_equipo++;

		liste = lst_equipo->lst_con;

		while (liste != NULL) {
			con = (LST_PSEUDO_CON) liste->DATA;
			if (con->coeur_plot == COEUR_CON)
				printf("COEUR  ");
			else 
				printf("PLOT   ");

			if (con->con_lo != NULL) {

				switch (con->face) {
				case NORD: 
					car = 'N'; 
					break;
				case SUD : 
					car = 'S'; 
					break;
				case EST : 
					car = 'E'; 
					break;
				case OUEST: 
					car = 'O'; 
					break;
				}

				if (con->layer == ALU1) 
					metal = a1;
				else if (con->layer == ALU2) 
					metal = a2; 
				else 
					metal = er;

				printf("\tface %c Con %16s root %10s * ", car, con->nom_con, ((loins_list * )(con->con_lo)->ROOT)->INSNAME);
				printf("x %5ld y %5ld piste %5ld larg %3ld layer %s\n", (con->coord)->xabs, (con->coord)->yabs,
				     (con->coord)->piste, con->largeur, metal);
			} else
				printf("\tCon %s WARNING: pas de connecteur logique associe\n", con->nom_con);

			if (con->deport) {
				if ((con->coord)->suiv) 
					printf("suiv x%ld y%ld\n", ((con->coord)->suiv)->xabs, ((con->coord)->suiv)->yabs);
				if ((con->coord)->prec) 
					printf("prec x%ld y%ld\n", ((con->coord)->prec)->xabs, ((con->coord)->prec)->yabs);

				printf("\tDEPORT coord xabs %ld  yabs %ld  piste %ld\n", (con->deport)->xabs, (con->deport)->yabs,
				     (con->deport)->piste);
			}

			liste = liste->NEXT;
		}

		listevav = lst_equipo->lst_visavis;
		if (listevav) 
			if (mode_debug) 
				printf("\tLISTE VISAVIS\n");

		while (listevav != NULL) {
			con = (LST_PSEUDO_CON) listevav->DATA;
			printf("***VISAVIS %16s coeur_plot  %d root %10s * ", con->nom_con, con->coeur_plot, ((loins_list
			    *)(con->con_lo)->ROOT)->INSNAME);
			printf("x %5ld y %5ld larg %5ld face %d\n", (con->coord)->xabs, (con->coord)->yabs, con->largeur,
			     con->face);
			listevav = listevav->NEXT;
		}

		listeseg = lst_equipo->lst_seg;

		if (listeseg) 
			printf("\tLISTE SEGMENTS\n");

		while (listeseg) {

			lseg = (LST_SEGMENT) listeseg->DATA;
			if (lseg->layer == ALU1) 
				metal = a1;
			else if (lseg->layer == ALU2) 
				metal = a2; 
			else 
				metal = er;

			switch (lseg->face) {
			case NORD: 
				car = 'N'; 
				break;
			case SUD : 
				car = 'S'; 
				break;
			case EST : 
				car = 'E'; 
				break;
			case OUEST: 
				car = 'O'; 
				break;
			}

			printf("face %c x1 %5ld y1 %5ld x2 %5ld y2 %5ld -- piste1 %3ld piste2 %3ld -- larg %3ld layer %2s \n",
			       car, lseg->c1->xabs, lseg->c1->yabs, lseg->c2->xabs, lseg->c2->yabs, lseg->piste1, lseg->piste2,
			     lseg->largeur, metal);
			listeseg = listeseg->NEXT;
		}

		if ((lst_equipo->cour).lst_projetes) { 
			printf("\tLISTE PROJETES perimetre %ld\n", (lst_equipo->cour).perimetre);
			lst_proj = (lst_equipo->cour).lst_projetes;
			while (lst_proj) {
				if (((PT_COORDONNEES)lst_proj->DATA)->proprio)
					printf("valcurv %5ld con %s \n", lst_proj->TYPE, ((LST_PSEUDO_CON) ((PT_COORDONNEES) lst_proj->DATA)->proprio)->nom_con);
				else 
					printf("valcurv%5ld \n",lst_proj->TYPE);
				lst_proj = lst_proj->NEXT;
			}
		}

		lst_equipo = lst_equipo->suiv;
	}

	printf("\tNOMBRE D'EQUIPO %d\n", cpt_equipo);

}

