/* Structure de donnees pour les vecteurs Boundary Scan */
typedef struct Data {
  char NEW_VAL;
  char OLD_VAL;  /* Vecteur precedent et courant                    */
  int INDEX;             /* Indexe de la cellule par rapport a la structure */
  char *NAME;             /* nom                                             */
  Cellules *CELL;         /* de donnees informations, mode de la cellule     */
			 /* (entree ou sortie)                              */
  struct paevt *NEW_PAEV;
  struct paevt *OLD_PAEV;
} Data;


/****************************************************************************/

/* Creation des vecteurs Boundary Scan en n sequences*/
extern void traduire(char *sce, char *destSRC, char *destBS, unsigned int sequence, unsigned int burst_size);

/****************************************************************************/
/* Envoi un vecteur Boundary Scan (TDI, TDO, TMS, TRST, TCK) sur le circuit
   ou dans une structure de donnees */
/****************************************************************************/
extern struct papat *Envoi(struct papat *ppapat, char valeur[5], char *label);

/****************************************************************************\
 front montant puis descendant label sur 1ere pattern
\****************************************************************************/
extern struct papat *TAP_in(struct papat *ppapat, char valeur[5], char *label);


/****************************************************************************/

