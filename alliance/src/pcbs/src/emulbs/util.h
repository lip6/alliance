/****************************************************************************/

/*****************************************************************************\
affichage des temps d'acces, de io etc...
\*****************************************************************************/
extern void statistic( unsigned long time_prg, unsigned long time_tck, unsigned long time_io, unsigned long nbr_pat );


/*remplace tous les espaces d'un mot par des '_' ---> ne peut etre desallouer*/
char *del_espace(char *s);


/* compte le nombre de pattern d'un fichier PAT et retourne...
  ---> nombre de patterns*/
extern int Compte_file_pat(char *pat);


/* Visualisation de la structure de donnees */
extern int AffichePaseq(struct paseq *pat);

/*allocation memoire des labels papat */
/*pour une sequence de patterns*/
extern char* block_malloc(char *s);
extern void  block_free();
/*****************************************************************************/



