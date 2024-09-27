typedef struct spimodel {
  struct spimodel *NEXT;
  char            *MODEL;
  char             TYPE;
  char             SUBCKT;
}spimodel ;

void spiloadmodel __P(( void ));
char spitranstype __P(( char* ));
char spitranssubckt __P(( char* ));
char* spitransmodel __P(( char ));

/* Valeur pour dire que le nom de modele de transistor passé à la fonction
 * spitranstype n'est pas défini. Ce nombre doit être une valeur impossible
 * pour le champs type des transistors */
#define SPI_UNK_TRANS_TYPE 64
