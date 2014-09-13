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

/*-----------------------------------------------------------------------------------*/

/*  RING.  27 avril 92 */

/*  Definition des types et structures de donnees utilisees par RING */


#ifndef __STRUCT_H
#define __STRUCT_H

#include <mut.h>
#include <mlo.h>
#include <mph.h>

#define NB_FACES 4
#define NB_ITERATIONS 50 /* nb iterations a effectuer pour bon placement barre de plots */ 

#define SEG_PISTE   0 /* type segment parallele a face coeur        */
#define SEG_COL     1 /* type segment perpendiculaire face coeur    */

#define VRAI_SEG    0 /* vrai segment qui sera a poser physiquement */
#define FAUX_SEG    1 /* faux segment utile uniquement au routage   */

#define AVEC_VIA    0 /* pose d'un via uniquement pour alim         */
#define SANS_VIA    1 /* pas de via uniquement pour alim            */

#define AVEC_TESTLAYER 0 /* on teste le layer dans la fonction croisementok_alimequi() */
#define SANS_TESTLAYER 1 /* on ne	teste pas le layer dans la fonction croisementok_alimequi() */

#define AVEC_COMPRESS 0 /* coordonee deja compressee                */
#define SANS_COMPRESS 1 /* coordonne non compresse (par defaut)     */
#define DEPORTG     0 /* localisation deport gauche droite face par rapport au coeur */
#define DEPORTD     1 /* des connecteurs plots ou coeur (centre uniquement)          */
#define DEPORTC     2

#define COEUR_PLOT  0
#define COEUR_COEUR 1
#define PLOT_PLOT   2
#define ALIM        3         /* equipo de type alim   */
#define AUTRE       4         /* equipo de type autre que les precedents */

#define FBARRE_N "fzigopadn1992" /* Noms futiles pour definir les figures barres de plots*/
#define FBARRE_S "fzigopads1992"
#define FBARRE_E "fzigopade1992"
#define FBARRE_O "fzigopado1992"

#define IBARRE_N "izigopadn1992" /* Noms futiles pour definir les instances barres de plots*/
#define IBARRE_S "izigopads1992"
#define IBARRE_E "izigopade1992"
#define IBARRE_O "izigopado1992"

#define NORD   0
#define SUD    1
#define OUEST  2
#define EST    3
/* Nombres entiers obligatoires ! */
#define WMIN_ALU1      MBK_TRACK_WIDTH_ALU2 /* largeur minimum de l'alu1,l'alu2,dist mini alu1<->alu2 */
#define WMIN_ALU2      MBK_TRACK_WIDTH_ALU2 /* et largeur du via                                      */
#define DMIN_ALU1_ALU1 ((MBK_TRACK_SPACING_ALU1+(MBK_TRACK_SPACING_ALU1)%2)/2) /* dmin en a1 a1  2.5 arrondi a 3 */
#define DMIN_ALU2_ALU2 ((MBK_TRACK_SPACING_ALU2+(MBK_TRACK_SPACING_ALU2)%2)/2)
#define WVIA_ALU1      MBK_TRACK_WIDTH_ALU1 /* largeur du via pour l'alu1 */
#define WVIA_ALU2      MBK_TRACK_WIDTH_ALU2 /* largeur du via pour l'alu2 */

#define EXTENSION_ALU2 1 /* extension alu2 pour fignoler coin couronne */

#define BV_VIA_VIA     (MBK_TRACK_WIDTH_ALU2+MBK_TRACK_WIDTH_ALU2%2)  /* must be even, whatever! */
#define BV_VIASIZE     WVIA_ALU2  /* design rule for equipotential vias */

#define WALIM          60  /* largeur prdefinie des alim                           */

/*#define PISTE_DEP_ALIMPLOT 10 Nombre de piste a considerer pour une deport alim plot */
/* code des ringerreurs traitees par ringerreur(code)                                      */
#define ERR_ARGUMENT   1
#define ERR_NONCOEUR   2
#define ERR_PLUSCOEUR  3
#define ERR_COEURINSTLO 4
#define ERR_COEURINSTPH 5
#define ERR_CIRCUITLO   6
#define ERR_CIRCUITINST 7
#define ERR_FICPARAM    8
#define ERR_VIDEFICPARAM 9
#define ERR_SYNPARAM    10
#define ERR_AUCUNPLOTPARAM 11
#define ERR_NONPLOTPARAM 12
#define ERR_PLUSPLOTPARAM 13
#define ERR_NBPLOTPARAM 14
#define ERR_NONCONWIDTHPARAM 15
#define ERR_INTERNE_CON 16
#define ERR_CONCOEUR_ENLAIR 17
#define ERR_CONPLOT_ENLAIR 18
#define ERR_CONLARGEUR     19
#define ERR_CONDISTANCE    20
#define ERR_MANQUEALIM     21
#define ERR_BARREALIM      22
#define ERR_ECHECDEPORT    23
#define ERR_INTERNE_EQ     24
#define ERR_SEGDEPORT      25
#define ERR_NOPISTE        26
#define ERR_NOCOL          27
#define ERR_INSTPH_PLOT    28
#define ERR_NOCONPH_PLOT   29
#define ERR_FIGPH_PLOT     30
#define ERR_CONEXT_ENLAIR  31
#define ERR_INTERNE_FIG    32
#define ERR_NBCONESTOUEST  33
#define ERR_NOCONPH        34
#define ERR_CATAL          35
#define ERR_WIDTHPARAM     36
#define ERR_EQINTERNE      37
#define ERR_EQEXTERNE      38
#define ERR_TIREALIM       39
#define ERR_FICSTAT        40
#define ERR_GRILLEINTERNE  41
#define ERR_CONCOEUREMPTY  42
#define ERR_CONLO_SANS_PH  43
#define ERR_CONPH_SANS_LO  44

/*-----------------------------------------------------------------------------------*/

/* type liste de pseudo-connecteurs. Il sert  pour les listes de plots et du
   coeur */

#define COEUR_CON 1
#define PLOT_CON  0

/* type coordonnees : contient les coordonnees absolues et symboliques */

typedef struct coord {
	long	piste;  /* coordonnee piste   (horizontal par rapport a la face) */
	long	xabs;   /* coordonnees x,y absolues dans la figure circuit physique */
	long	yabs;
	void * proprio;      /* pointeur sur le pseudo-connecteur appartenant */
	int	compress;       /* coordonnees compressee ou pas */
	struct coord *suiv;  /* chainage avant  arriere sur la grille ou appartient */
	struct coord *prec;  /* le point courant                                    */
} COORDONNEES, *PT_COORDONNEES;

typedef struct pseudo_con {
	char	coeur_plot;     /* soit COEUR, soit PLOT */
	PT_COORDONNEES coord;   /* coordonnees symboliques et relatives */
	PT_COORDONNEES deport; /* pointeur sur coordonnees symboliques et relatives
                            d'un eventuel point de deport, si pointeur null
                            alors pas de deport , 
		                            deport-> piste = 1 si deport uniquement pour layer */
	int	situe_deport;    /* deport gauche droit ou centre                        */
	int	face;           /* face a laquelle appartient le pseudo_connecteur */
	long	largeur;        /* largeur du connecteur */
	char	layer;          /* niveau physique du connecteur */
	char	*nom_con;       /* nom du connecteur */
	locon_list * con_lo;  /* pointeur sur connecteur logique de la figure */

	struct pseudo_con *suiv;  /* pointeur sur structure suivante */
	struct pseudo_con *prec;  /* pointeur sur structure precedente */
} PSEUDO_CON, *LST_PSEUDO_CON;

/* type liste de segments. Il sert pour les placements des segments physiques
	 pour chaque equipotentielle et chaque direction (horizontal/vertical). Il renseigne
   aussi pour les segments occupes. ON ne sert que exclusivement que des coordonnees
	 en xabs ou yabs. Il y a un champ piste associe au segment et a sa direction dans la
	 face. */

typedef struct segment {
	int	face;       /* face a laquelle est associee le segment                   */
	long	piste1;     /* piste associee au segment de type SEG_COL et SEG_PISTE    */
	long	piste2;     /* 2eme piste associee au segment de type SEG_COL            */
	PT_COORDONNEES c1;      /* coordonnees symboliques et absolues du debut segment      */
	PT_COORDONNEES c2;      /* coordonnees symboliques et absolues de fin   segment      */
	long	largeur;    /* largeur physique du segment                               */
	char	layer;      /* layer segment                                             */
	int	vraifaux;   /* vraifaux segment. c'est un faux si largeur  speciale      */
	int	via;        /* pose d' un via ou pas, utile uniquement pour alim          */
	struct segment *suiv;   /* pointeur sur structure suivante ds liste segments occupes */
	struct segment *prec;   /* pointeur sur structure precedente    "     "        "     */
} SEGMENT, *LST_SEGMENT;

/* type couronne: couronne (complete ou incomplete necessaires au
	 routage, les segments sont completes au fur et a mesure (projetes ...-> segments */

typedef struct couronne {
	ptype_list  *lst_projetes;       /* liste de coordonnees projetees sur la cour
		                                      avec leur valeur curviligne                      */
	long	perimetre;      /* perimetre curviligne de la couronne       */
} COURONNE;

/* type stat: caracteristiques de l'equipotentielles en
   longueur, surface, nb vias */

typedef struct stat {
	long	lg_a1; /* longueur en lambdas en alu1 alu2 de l'equipo */
	long	lg_a2;
	long	su_a1; /* surface en lamdas carres en alu1 alu2 de l'equipo */
	long	su_a2;
	long	nb_vias;
} STAT;

/* type equipotentielle: liste de pseudo-connecteurs portant le meme signal (nom+index) */

typedef struct equipo {
	int	type;             /* type de l'eq: normal, alim, autre) */
	long	index;
	chain_list     * lst_con; /* tete de chain-list pointant sur des pseudo connecteurs */
	chain_list     * lst_seg; /* tete de liste des segments associes  */
	ptype_list     * lst_visavis; /* liste de pointeurs sur des connecteurs en vis a vis */
	COURONNE       cour;     /* structure couronne associee a l'equipo                  */
	STAT           status; /* stats en longueur et surface */
	struct equipo *suiv;    /* pointeur sur structure suivante */
} EQUIPO, *LST_EQUIPO;

/* type lst_piste_col: liste de segments occupes pour piste ou colonne */

typedef struct piste_col {
	long	numero;           /* numero de la piste ou de la colonne */
	chain_list * lst_seg;   /* liste de pointeurs sur segments */
	struct piste_col *suiv;  /* pointeur sur structure suivante */
	struct piste_col *prec;  /* pointeur sur structure precedente */
} PISTE_COL, *LST_PISTE_COL;

/* type barre_plots: contient les donnees necessaires a la construction de
   la barre de plots */

typedef struct barre_plots {
	LST_PSEUDO_CON lst_con;   /* liste de pointeurs sur connecteurs de plots */
	COORDONNEES coord;     /* coordonnees symboliques et absolues de la barre de plots */
	long	width;   /* longueur de la barre de plots */
	long	height;     /* hauteur de la barre de plots */
	int	nb_deport;  /* nombre de deports prevus par le placement de la barre */
	char	*nomfig;    /* nom de la figure physique */
	char	*nominst;   /* nom de l'instance physique */
} BARRE_PLOTS;

/* type grille_symbo_piste: grille des pistes (pas fixes definis par le pitch) */

typedef struct symbo_piste {
	long	premiere;         /* coordonnee absolue de la premiere piste  */
	int	largeur_pas;      /* valeur du pas de grille                  */
	long	nb_pas;           /* nombre de pas de grilles definis maximum */
} GRILLE_SYMBO_PISTE;

/* type grille: regroupe les 2 types precedents pour obtenir la grille qui permettra de
	 transformer les coordonnees symboliques en absolues et vice-versa */

typedef struct grille {
	PT_COORDONNEES lst_pas;    /* liste des points formant la grille      */
	PT_COORDONNEES lst_deportg;/* liste des deports gauches de la grille  */
	PT_COORDONNEES lst_deportd;/* liste des deports droits  de la grille  */
	GRILLE_SYMBO_PISTE piste;  /* donnees pour le calcul des pas de piste */
} GRILLE;

/* type coeur: regroupe les informations le concernant */

typedef struct coeur {
	loins_list *coeur_lo;   /* pointeur sur figure logique du coeur */
	phfig_list *coeur_ph;   /* pointeur sur figure physique du coeur */
	COORDONNEES coord;      /* coordonnees symboliques(0,0) et absolues */
	long	width;
	long	height;
	char	rotation;    /* rotation du coeur */

} COEUR;

/* type cel : cellule contient la largeur specifique d'un signal, fera parti de la
   chain_list liste_width, remplie par l'analyseur syntaxique */

typedef struct cel {
	char	*chaine;
	long	largeur;
	locon_list * con_lo;
} CEL, *PTCEL;

/*-----------------------------------------------------------------------------------*/

/* Variables globales */

extern char	mode_debug;                  /* si debug alors mode_debug=1, 0 sinon */
extern char	mode_stat;                  /* si stat alors mode_stat=1, 0 sinon */
extern char	*nomfic_stat;               /* nom du fichier statistique */

extern char	xmetal;                      /* metal utilise dans les x             */
extern char	ymetal;                      /* metal utilise dans les y             */
extern long	xmetal_width, ymetal_width;  /* largeur des metaux x, y              */
extern long	xmetal_dmin, ymetal_dmin;    /* dist minimum entre 2 meme metaux     */
extern long	xmetal_wvia, ymetal_wvia;    /* taille des contacts pour chaque metal */
extern long	pitch;                       /* pitch a calculer                     */
extern long	lambda;                      /* lambda (= SCALE_X)                   */
extern long	taille_via;                  /* taille du via a calculer             */
extern long	vdd_width;                   /* largeur couronnes d'alim             */
extern long	vss_width;
extern char	*eq_vdd;                     /* nom equipo alim              */
extern char	*eq_vss;                     /* nom equipo alim              */
extern char	*n_vdde;
extern char	*n_vsse;
extern char	*n_vddi;
extern char	*n_vssi;
extern char	*n_pad;
extern char	*pvddi_p;
extern char	*pvssi_p;

extern char	*nom_fic_param;              /* nom du fichier parametre            */

extern FILE *yyin;                       /* pointeur sur fichier lex */
extern int	yylineno;                     /* no de la ligne traitee par lex */
extern chain_list *nom_plot[NB_FACES];   /* liste pour l'analyseur des noms de 
                                            plots */
extern chain_list *liste_width;          /* liste pour l'analyseur des noms
                                            et largeurs des connecteurs speciaux */

extern long	bigvias , ringvias ;   /* nombres de vias poses en total par bigvia
                                            ou en couronne (mode debug). car pb versatil
                                            si bcp de vias (fichier .cif enorme)       */

/*-----------------------------------------------------------------------------------*/

/* Declarations des fonctions */

/* Fichier struct.c */
extern void ajout_listeplotsph();
extern int	existe_connecteur_faceplot();
extern phfig_list*appartient_listeplotsph();
extern void affic_listeplotsph();
extern loins_list*existe_plot_circuit();
extern int	existe_signal_circuit();
extern int	uneseuleoccurence();
extern void declaration_plot_n();
extern void declaration_plot_s();
extern void declaration_plot_w();
extern void declaration_plot_e();
extern void declaration_width();
extern void affic_plotwidth();
extern int	reorientation_con();
extern void ajout_pseudo_con_coeur();
extern void ajout_pseudo_con_plot();
extern void inserer_apres_lstcon();
extern void inserer_avant_lstcon();
extern void affic_lstcon();
extern LST_PSEUDO_CON existe_concoeur_lstequipo();
extern LST_EQUIPO existe_concoeur_lstequipo2();
extern LST_PSEUDO_CON existe_conplot_lstequipo();
extern void maj_equipo();
extern LST_EQUIPO existe_sig_equipo();
extern void affic_equipo();
extern locon_list*vraifaux_conlogic();
extern LST_PSEUDO_CON existe_conlogicplot_lstequipo();
extern void rename_vddxvssx();
extern void ignoble_multiple_ck_delete_from_south_side();

/* Fichier struct2.c   */
extern int	moins_dun_pitch_preccon();
extern void ajout_coordonnees_grille();
extern void ajout_visavis();
extern int	existe_visavis();
extern void affic_grille();
extern void alloue_coord();
extern PT_COORDONNEES cherche_deport_grille();
extern void chaine_deportcon_grille();
extern void alloue_etchaine_segment();
extern int	segment_libre();
extern void affic_listeseg();
extern phcon_list *cherche_conph_circuit();
extern long	croisement_con_alim();
extern void largeur_vddvss();
extern long	croisementok_alimequi();
extern long	compte_nbplotsface();

#endif /* __STRUCT_H */
