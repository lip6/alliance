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

/*
 * Tool    : ABL, BDD, HT Librarie
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 */



/* $Id: log.h,v 1.7 2012/05/14 14:20:19 alliance Exp $ */

#ifndef LUC_LOG_H
#define LUC_LOG_H

#undef OR
#undef AND
#undef XOR
#undef NOT
#undef NOR
#undef NAND
#undef NXOR
#undef CONTRAINT
#undef STABLE
#undef RESTRICT
#undef CNST

#define OR 0
#define AND 1
#define XOR 2
#define NOT 3
#define NOR 4
#define NAND 5
#define NXOR 6
#define CONTRAINT 7
#define STABLE 8
#define RESTRICT 9
#define CNST 10

/* ================================================================== 
   Gestion de table de hachage                    Version du 16.07.91
   Les structures de donnees
   Burgun L. 
   ================================================================== */


#define EMPTYTH -1
#define VIDETH -1
#define DELETETH -2

/*------ les structures pour la table de hachage des entiers -----*/

typedef struct elemTH
  {
    char *key;
    long value;
  }
 *pElemTH;

/* table de hachage 
   length est la longueur de la table,
   pElemTH le pointeur sur le debut de table,
   count le nombre d'elements deja rentres. */

typedef struct tableTH
  {
    long length;
    pElemTH pElem;
    long count;
  }
 *pTH;


/* ==============================================================
   La bibliotheque des Arbres binaires Lisp-like version 23/08/91
   Burgun L.
   Structures de donnees
   ============================================================== */



/*-----------  Fonctions Lisp-Like de bas-niveau  -------------*/

#define CDR(expr) (expr->NEXT)
#define CAR(expr) ((chain_list *) expr->DATA)
#define CADR(expr) CAR(CDR(expr))
#define ATOM(expr) (!expr->NEXT)
#define VALUE_ATOM(expr) (char *) expr->DATA
#define OPER(expr) ((long)(CAR(expr))->DATA)



/* ==============================================================
   La bibliotheque des graphes de decision binaires version 06/09/91
   Burgun L.
   Structures de donnees
   ============================================================== */

#undef OUI
#undef NON
#undef TRUE
#undef FALSE
#undef INPUT
#undef OUTPUT
#undef SMALL
#undef MEDIUM
#undef LARGE

#undef VIDE
#undef DELETE
#undef TABLE_PLEINE
#undef BDDDELETE
#undef BDDTABLE_PLEINE
#undef MAX_SIZE_BDD

#undef DONTCARE0
#undef DONTCARE1
#undef DONTCARE2


#define OUI 1
#define NON 0
#define TRUE 1
#define FALSE 0
#define INPUT 0
#define OUTPUT 1
#define SMALL 999
#define MEDIUM 9999
#define LARGE 99999
#define SMALL_BDD 0
#define MEDIUM_BDD 1
#define LARGE_BDD 2

#define VIDE -1
#define DELETE -2
#define TABLE_PLEINE -3
#define BDDDELETE (pNode) DELETE
#define BDDTABLE_PLEINE (pNode) TABLE_PLEINE
#define MAX_SIZE_BDD 50000000	/* 50 Mega de noeuds Bdd */

#define DONTCARE0 11
#define DONTCARE1 12
#define DONTCARE2 13



/*---------------- structure d'un noeud de BDD -------------------*/

typedef struct node
  {
    struct node *high, *low;	/* les noeuds fils */
    short index;		/* index de la variable */
    short mark;			/* nombre de peres pointant le noeud */
  }
 *pNode;


/*--------------- La table de hachage pour des BDD ------------ */

/* table de hachage qui retourne des pointeurs de BDD
   lenTableBdd est la longueur de la table,
   pBddT le pointeur sur le debut de table,
   compteur le nombre d'elements deja rentres. */

typedef struct tableBdd
  {
    long lenTableBdd;
    pNode *pBdd;
    long compteur;
  }
 *pTableBdd;



/*------ les structures pour la table de hachage local -----*/

typedef struct vertexLoc
  {
    pNode high, low, father;
    short oper;
  }
 *pVertexLoc;

/* table de hachage pour la recuperation d'operation locale. 
   lenTable est la longueur de la table,
   pElemT le pointeur sur le debut de table,
   compteur le nombre d'elements deja rentres. */

typedef struct tableLoc
  {
    long lenTabLoc;
    pVertexLoc pLoc;
  }
 *pTableLoc;

/*------------- structure pour les circuits -------------*/

typedef struct circuit
  {
    pTH pTI;
    pTH pTO;
    short countI;
    char **pNameI;
    char *name;
  }
 *pCircuit;

#define MAX_PACK 1000

/* structure systeme pour la generation de GDB . */

struct systemBdd
  {
    chain_list *lpAT;
    pTableBdd pRT;
    pNode pAT;
    long indiceAT;
    pTableLoc pMC;
  };
extern struct systemBdd sysBdd;

extern pNode one, zero;

/**************** DECLARATION DES FONCTIONS *******/

#ifndef __P
# if defined(__STDC__) || defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/* Prototypes from log_bdd0.c */

  extern           pNode initVertexBdd __P((long index, pNode high, pNode low));
  extern           pNode createNodeTermBdd __P((short index));
  extern            void initializeBdd __P((long size));
  extern            void destroyBdd __P((long level));
  extern            void resetBdd __P(());
  extern             long numberNodeAllBdd __P(());
  extern             long numberNodeBdd __P((pNode pBdd));
  extern             long countNode __P((pNode pt));
  extern             long countNodeTdg __P((pNode pt));
  extern    chain_list * muxAbl __P((pNode high, pNode low, chain_list *a, char **tabName));
  extern    chain_list * bddToAbl __P((pNode pt, char **tabName));
  extern            void displayBddLoc __P((short level, pNode pt));
  extern            void displayBdd __P((pNode pBdd, long level));
  extern            void assignNumNodeBdd __P((pNode bdd, pTH vTable, long *pNodeNumber));
  extern            void displayGraphicBdd __P((pNode pBdd));
  extern            void displayBddName __P((short level, pNode pt, char **TabName));
  extern           pNode notBdd __P((pNode pBdd));
  extern           pNode applyTerm __P((long oper, short index, pNode pBdd));
  extern           pNode applyBinBdd __P((short oper, pNode pBdd1, pNode pBdd2));
  extern           pNode applyBdd __P((short oper, chain_list *pt));
  extern           pNode cnstBdd __P((pNode pBdd1, pNode pBddGc));
  extern           pNode restrictBdd __P((pNode pBdd1, pNode pBddGc));
  extern           pNode constraintBdd __P((pNode pBdd1, pNode pBddGc));
  extern           pNode simplifDcZeroBdd __P((pNode pGon, pNode pGdc));
  extern           pNode simplifPlusDcZeroBdd __P((pNode pGon, pNode pGdc));
  extern           pNode simplifDcOneBdd __P((pNode pGon, pNode pGdc));
  extern           pNode simplifDcOneFPGABdd __P((pNode pGon, pNode pGdc));
  extern           pNode composeBdd __P((pNode pBdd1, pNode pBdd2, long index));
  extern    chain_list * addListBdd __P((chain_list *pt, pNode pBdd));
  extern             long oneBdd __P((pNode pBdd));
  extern             long zeroBdd __P((pNode pBdd));
  extern             long equalBdd __P((pNode pBdd1, pNode pBdd2));
  extern            void markBdd __P((pNode pBdd, short value));
  extern           pNode upVarBdd __P((pNode pF, pNode pFoldIndex, short newIndex));
  extern            void markAllBdd __P((short value));
  extern            void supportBddInt __P((pNode pt, chain_list **ppCL));
  extern    chain_list * supportChain_listBdd __P((pNode pBdd));
  extern           pNode initVertexBddAux __P((short index, pNode high, pNode low, struct systemBdd *sysCible));
  extern           pNode regenereBdd __P((pNode pBdd, struct systemBdd *sysCible, pTH pTHNode));
  extern            void gcNodeBdd __P((chain_list *pt));
  extern            void rempTabIndex __P((pNode pt, char *tabIndex));
  extern    chain_list * supportIndexBdd __P((pNode pt, long sens));


/* Prototypes from log_bdd1.c */

  extern        pCircuit initializeCct __P((char *name, long nbI, long nbO));
  extern            void resetCct __P((pCircuit pC));
  extern            void destroyCct __P((pCircuit pC));
  extern           pNode searchOutputCct __P((pCircuit pC, char *name));
  extern            void addOutputCct __P((pCircuit pC, char *name, pNode pt));
  extern          char * searchIndexCct __P((pCircuit pC, short index));
  extern           short searchInputCct __P((pCircuit pC, char *name));
  extern           short addInputCct __P((pCircuit pC, char *name));
  extern            void delInputCct __P((pCircuit pC, char *name));
  extern            void displayCct __P((pCircuit pC, long mode));
  extern            void composeCct __P((pCircuit pC, char *name, pNode pt));
  extern            void constraintCct __P((pCircuit pC, pNode pt));
  extern            void proofCct __P((pCircuit pC1, pCircuit pC2));
  extern           pNode ablToBddCct __P((pCircuit pC, chain_list *expr));
  extern            void cpOrderCct __P((pCircuit CC1, pCircuit CC2));
  extern            void upVarCct __P((pCircuit pC, pNode ptOldIndex, short newIndex));
  extern             long numberNodeCct __P((pCircuit pC));
  extern             long numberNodeTdgCct __P((pCircuit pC));
  extern    chain_list * bddToAblCct __P((pCircuit pC, pNode pBdd));
  extern            void gcNodeCct __P((pCircuit pC));


/* Prototypes from log_prefbib.c */

  extern          char * gensym_abl __P((char *name, long num));
  extern            void ablError __P((chain_list *expr, char *func));
  extern    chain_list * createAtom __P((char *name));
  extern    chain_list * createExpr __P((short oper));
  extern    chain_list * notExpr __P((chain_list *expr));
  extern    chain_list * createBinExpr __P((short oper, chain_list *expr1, chain_list *expr2));
  extern            void addQExpr __P((chain_list *expr1, chain_list *expr2));
  extern            void addHExpr __P((chain_list *expr1, chain_list *expr2));
  extern            void freeExpr __P((chain_list *expr));
  extern          char * operToChar __P((short oper));
  extern           short charToOper __P((char *name));
  extern            void displayExprInt __P((chain_list *expr));
  extern            void displayExpr __P((chain_list *expr));
  extern            void displayInfExpr __P((chain_list *expr));
  extern          char * exprToCharInt __P((chain_list *expr, long mode, char *chaine, long *taille));
  extern          char * exprToChar __P((chain_list *expr, long mode));
  extern          char * identExprInt __P((chain_list *expr, char *chaine, long *taille));
  extern          char * identExpr __P((chain_list *expr));
  extern             long profExpr __P((chain_list *expr));
  extern             long profAOExpr __P((chain_list *expr));
  extern    chain_list * mapCarExpr __P((chain_list *(*func)(), short oper, chain_list *expr));
  extern            void mapExpr __P((void (*func)(), chain_list *expr));
  extern             long anyExpr __P((long (*func)(), chain_list *expr));
  extern             long everyExpr __P((long (*func)(), chain_list *expr));
  extern             long searchOperExpr __P((chain_list *expr, short oper));
  extern           short searchExprLow __P((chain_list *expr, char *name));
  extern             long searchExpr __P((chain_list *expr, char *name));
  extern             long equalExpr __P((chain_list *expr1, chain_list *expr2));
  extern             long equalVarExpr __P((chain_list *expr1, chain_list *expr2));
  extern             long lengthExpr __P((chain_list *expr));
  extern             long numberOperBinExpr __P((chain_list *expr));
  extern             long numberAtomExpr __P((chain_list *expr));
  extern    chain_list * copyExpr __P((chain_list *expr));
  extern            void substPhyExpr __P((chain_list *expr1, char *name, chain_list *expr2));
  extern    chain_list * substExpr __P((chain_list *expr1, char *name, chain_list *expr2));
  extern    chain_list * devXorExpr __P((chain_list *expr));
  extern    chain_list * devXor2Expr __P((chain_list *expr));
  extern    chain_list * flatPolarityExpr __P((chain_list *expr, long signe));
  extern            void flatArityExpr __P((chain_list *expr));
  extern            void supportChain_listExprInt __P((chain_list *expr, chain_list **ppCL));
  extern    chain_list * supportChain_listExpr __P((chain_list *expr));
  extern            void supportPtype_listExprInt __P((chain_list *expr, ptype_list **ppCL));
  extern    ptype_list * supportPtype_listExpr __P((chain_list *expr));
  extern    chain_list * maxExpr __P((chain_list *expr, long (*func)()));
  extern    chain_list * minExpr __P((chain_list *expr, long (*func)()));
  extern            void sortExpr __P((chain_list *expr, long (*func)(), long direction));
  extern             long funcNormExpr __P((chain_list *expr));
  extern            void normExpr __P((chain_list *expr));
  extern            void deleteNumExpr __P((chain_list *expr, long i));
  extern    chain_list * searchNumExpr __P((chain_list *expr, long i));
  extern             long numberOccExpr __P((chain_list *exp, char *name));
  extern            void changeOperExpr __P((chain_list *expr, short oper));
  extern    chain_list * simplif10Expr __P((chain_list *expr));
  extern    chain_list * simplifNotExpr __P((chain_list *exp));
  extern    chain_list * charToExprInt __P((char *stringExpr, long *cptCar));
  extern    chain_list * charToExpr __P((char *stringExpr));
  extern          char * tokenExpr __P((char *stringExpr, long *cptCar));
  extern             long PMExprInt __P((chain_list *expr, chain_list *pattern, ptype_list **bind));
  extern             long PMExpr __P((chain_list *expr, chain_list *pattern));


/* Prototypes from log_thash.c */

  extern             long hashTH __P((char *pn));
  extern             pTH createTH __P((long len));
  extern            void destroyTH __P((pTH pTable));
  extern             long searchTH __P((pTH pTable, char *key));
  extern             long addTH __P((pTH pTable, char *key, long value));
  extern             long addExistTH __P((pTH pTable, char *key, long value));
  extern             long deleteTH __P((pTH pTable, char *key));
  extern            void displayTH __P((pTH pTable));
  extern            void reAllocTH __P((pTH pTable));


/* Prototypes from log_thashbdd.c */

  extern             long hashBdd __P((long index, pNode high, pNode low));
  extern             long newKeyBdd __P((long index, pNode high, pNode low));
  extern       pTableBdd createTableBdd __P((long len));
  extern            void destroyTableBdd __P((pTableBdd pTab));
  extern       pTableBdd reAllocTableBdd __P((pTableBdd pTab));
  extern           pNode searchTableBdd __P((pTableBdd pTab, long index, pNode high, pNode low));
  extern             long addTableBdd __P((pTableBdd pTab, pNode pBdd));
  extern             long deleteTableBdd __P((pTableBdd pTab, pNode pBdd));
  extern            void displayHashBdd __P((pTableBdd pTab));


/* Prototypes from log_thashloc.c */

  extern             long hashLoc __P((pNode high, pNode low));
  extern       pTableLoc createTabLoc __P((long len));
  extern            void destroyTabLoc __P((pTableLoc pTab));
  extern           pNode searchTabLoc __P((pTableLoc pTab, pNode high, pNode low, short oper));
  extern             long addTabLoc __P((pTableLoc pTab, pNode high, pNode low, pNode father, short oper));
  extern            void displayLoc __P((pTableLoc pTab));
  extern            void videTabLoc __P((pTableLoc pTab));
#endif
