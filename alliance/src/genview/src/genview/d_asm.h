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

#define BOOL char
#define TRUE 1
#define FALSE 0
#undef  UNKNOWN
#define UNKNOWN -1

#define TAILLE_TAB_SYMB 2000 /* symbol table size */
#define TAILLE_TAB_REF  2000 /* reference table size */
#define BLOC_CODE       2000 /* code default page size */
#define BLOC_DATA       500  /* data default page size */
#define NB_TAB_CODES    104  /* number of instructions */
#define TAILLE_PILE     5000 /* stack size     */

#define REF_DATA 2

#define D_EXIT  255 /* end of interpretation value */

/* Element of the symbol table */
typedef struct {
		char *Symb; /* Symbol name */
		int  Pos;   /* Offset from code or data starting address */
		BOOL Data;  /* Is it a reference to a data? */
} SYMB, *PSYMB;

/* Element of the reference table */
typedef struct {
		int ref;
		int dep;    /* used in "_label + dep" context */
		char type;
} D_REF, *D_PREF;


/* Structure used for external variables (system or mbk) */
typedef struct {
		char *var;
		union {
			char *var;
			char **ptr;
		} adr;
		char type;
} VAR;

#define VAR_DAT 0
#define VAR_PTR 1

/* Element of the break points list */
typedef struct bp {
		int NumLig;
		 struct bp *Prec;
		 struct bp *Suiv;
} BP, *HBP;

/* fonctions de asm.c */
void Erreur ();  /* gestion des erreurs */
int GereTabSymb ();  /* gestion de la table des symboles */
int RechInTable ();  /* recherche dichotomique dans une table */
char *SymbInTab ();  /* rend le symbole dans la table a l'index n */
int RechCodeInst ();  /* recherche du code d'une instruction */ 
int IncTailleTab();  /* incrementation de la taille du code */
void AddRef();  /* ajout d'une reference dans la table */

/* fonction de interp.c */
int interp();   /* procedure principale de l'interpreteur */ 

/* fonction de interp_dic.c */
char *symb_alloc();
