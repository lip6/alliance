/* ###--------------------------------------------------------------### */
/*									*/
/* file		: bvl_parse.h       	                                */
/* date		: May 31 1994						*/
/* version	: v109							*/
/* author	: L.A. TABUSSE & H.G. VUONG & P. BAZARGAN-SABET         */
/* description	:							*/
/*									*/
/* ###--------------------------------------------------------------### */

#define BVL_TRCDFN 0x00000001
#define BVL_AUXDFN 0x00000002

char         BVL_ERRFLG = 0;		/* if = 1 no structure is made  */
int          BVL_LINNUM = 1;		/* file's line number           */
int          BVL_AUXMOD;		/* simplify internal sig (= 1)	*/
char         BVL_CURFIL[200];		/* current file's name          */
befig_list  *BVL_HEDFIG = NULL;

extern FILE *bvl_y_in;
extern int   bvl_y_parse();

extern char *getenv();
