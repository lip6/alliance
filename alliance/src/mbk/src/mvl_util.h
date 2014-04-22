/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_util.h						*/
/* date		: Oct  4 1991						*/
/* author	: P. BAZARGAN-SABET					*/
/*									*/
/* description	: This file contains declaration of global and external	*/
/*		  variables and, functions used in `mvl_util.c` 		*/
/*									*/
/* ###--------------------------------------------------------------### */
	
extern int                MVL_LINNUM;	/* file's line number		*/
extern char               MVL_ERRFLG;	/* Error flag                   */
extern char               MVL_CURFIL[];	/* current file name		*/

static struct dct_entry  *MVL_DCEHED;	/* free dct_entry's head	*/
static struct dct_recrd  *MVL_DCRHED;	/* free dct_recrd's head	*/

static struct dct_entry *mvl_addent();
static struct dct_recrd *mvl_addrcd();
