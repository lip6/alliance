 
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_syacc.h						*/
/* date		: Nov 07 1991						*/
/* author	: P.BAZARGAN, L.A.TABUSSE, VUONG H.N.			*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*									*/
/* ###--------------------------------------------------------------### */

extern char          MVL_MBKMOD;        /* 'A' or 'P' or 'C')           */
extern char          MVL_ERRFLG;	/* if = 1 no structure is made	*/
extern struct lofig *MVL_LOFPNT;        /* current LOFIG pointer        */ 


extern struct dct_entry **MVL_HSHTAB;	/* dictionnary's entry points	*/

extern struct dct_entry **mvl_initab();
extern void          mvl_addtab();
extern int           mvl_chktab();
extern void          mvl_fretab();
extern void          mvl_error();
extern void          mvl_warning();

/* ###--------------------------------------------------------------### */

extern losig_list *mvl_addlosig ();
extern locon_list *mvl_addlocon ();

/* ###--------------------------------------------------------------### */
