/* ###--------------------------------------------------------------### */
/*                                                                      */
/* file         : mvl_parse.h 	                                     */
/* date         : Nov  12 1991                                          */
/* author       : L.A. TABUSSE & H.G. VUONG & P. BAZARGAN-SABET         */
/* description  : variable for Parser VHDL --> MBK                      */
/*                                                                      */
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */

char          MVL_MBKMOD;               /* The mode of getlofig         */
char          MVL_ERRFLG = 0;           /* if = 1 no structure is made  */
int           MVL_LINNUM = 1;           /* file's line number           */
char          MVL_CURFIL[200];          /* current file's name          */

struct dct_entry **MVL_HSHTAB;          /* dictionnary's entry points   */

extern void          mvl_error();
extern struct lofig *mvl_fill();

extern struct dct_entry **mvl_initab();
extern void               mvl_fretab();


/* ###--------------------------------------------------------------### */

struct lofig   *MVL_LOFPNT = NULL;      /* current LOFIG pointer        */

extern FILE    *mvl_y_in;
extern int      mvl_y_parse();
extern void 	mvl_decomp();

/* ###--------------------------------------------------------------### */
