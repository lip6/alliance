/****************************************************************************/
/* Calcul un nombre entier eleve a une puissance entiere */
int pow();

/* Duplication de chaine de caracteres */
char *dupchaine();

/*concatene deux chaines en allouant un nouvel espace memoire dans l'environnement mbk de la facon suivante:   s1 + ' ' + s2  ---> ne peut etre desallouer*/
char *concat_espace(char *s1,char *s2);

/*remplace tous les espaces d'un mot par des '_' ---> ne peut etre desallouer*/
char *del_espace(char *s);

/*remplace tous les espaces d'un mot par des '_' ---> pour mbkfree() */
char *del_espace_mbk(char *s);

/*concatene deux chaines en allouant un nouvel espace memoire dans l'environnement mbk---> ne peut etre desallouer*/
char *concat(char *s1,char *s2);

/*concatene deux chaines en allouant un nouvel espace memoire dans l'environnement mbk---> ne peut etre desallouer---> peut etre desallouer avec mbkfree*/
char *concat_mbk(char *s1,char *s2);

/* convertit un integer en string --->ne peut etre desallouer*/
char *itoa(int nbr);

/* convertit un integer en string--->peut etre desallouer a l'aide de mbkfree*/
char *itoa_mbk(int nbr);

/* convertit un caratere en string --->ne peut etre desallouer */
char *ctoa(char c);

/* convertit un caratere en string --->pour mbkfree() */
char *ctoa_mbk(char c);

/* Conversion int en ASCII-binaire ---> ne peut etre desallouer*/
char * itoabin(int valeur); 

/* Conversion int en ASCII-binaire ---> pour mbkfree() */
char * itoabin_mbk(int valeur); 

/* Conversion Ascii-binaire en int */
int abintoi(char *abin );

/*detruit tout une sequence de patterns*/
void erase_paseq(paseq_list *pat);

/* compte le nombre de pattern d'un fichier PAT et retourne...
  -1 ---> erreur de chargement du fichier pattern
 =>0 ---> nombre de patterns*/
int compte_pat(char *pat);

/*recoit en parametre un temps en millseconds et l'affiche avec un format*/
void affiche_time(unsigned long ms);

/* Visualisation de la structure de donnees */
int AffichePaseq(struct paseq *pat);

/*allocation memoire*/
/*p:sequence de patterns*/
void init_mem(unsigned int p);
char *mon_malloc(char *s);
void mon_free();
/*****************************************************************************/



