/*****************************************************************************/
/* charge le fichier de description de la chaine boundary scan
carte_emul--> nom fichier connexions PGA / EMULBS   et retourne...
   0 ---> pas d'erreur le resultat est dans la variable globale InfosVbe 
   (cf. globale.h)
  -1 ---> erreur de chargement du fichier emul
  -2 ---> erreur de chargement du fichier pga
   1 ---> warnings*/
int description_cartes(char *emul,char *pga);

/*creer les n EMUL31 chaines*/
void creer_infos();

/* affichage de la variable globale InfosVbe*/
void affich_infos();

/* suppression en memoire de la variable globale InfosVbe*/
void del_infos();
/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/




