/* Structure de donnees pour les vecteurs Boundary Scan */
typedef struct Data {
  char ancien, courant;  /* Vecteur precedent et courant                    */
  int index;             /* Indexe de la cellule par rapport a la structure */
  char *nom;             /* nom                                             */
  Cellules *cel;         /* de donnees informations, mode de la cellule     */
			 /* (entree ou sortie)                              */
} Data;


/****************************************************************************/

/* Creation des vecteurs Boundary Scan en n sequences*/
int traduire(char *sce, char *destSRC, char *destBS, unsigned int sequence);

/****************************************************************************/

