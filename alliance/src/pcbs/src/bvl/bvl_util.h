/* ###--------------------------------------------------------------### */
/*									*/
/* file		: bvl_util.h						*/
/* date		: Jun 15 1992						*/
/* version	: v102							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: This file contains declaration of global and external	*/
/*		  variables and, functions used in `bvl_util.c`		*/
/*									*/
/* ###--------------------------------------------------------------### */
	
extern int                BVL_LINNUM;	/* file's line number		*/
extern char               BVL_ERRFLG;	/* Error flag                   */
extern char               BVL_CURFIL[];	/* current file name		*/

static struct dct_entry  *BVL_DCEHED;	/* free dct_entry's head	*/
static struct dct_recrd  *BVL_DCRHED;	/* free dct_recrd's head	*/

static struct dct_entry *addent();
static struct dct_recrd *addrcd();
