#define BYPASS_NAME   "Bypass" 
#define INTEST_NAME   "Intest"
#define CONFIG_NAME   "Config"
#define EXTEST_NAME   "Extest"

/****************************************************************************/
/* Controle des informations a l'interieur de la structure de donnees 
   informations */
extern void VerificationsInfos();

/* Controle des informations entre les structures de donnees informations et
   vecteurs paralleles */
void VerificationsPat(struct paseq *pat);
/****************************************************************************/
