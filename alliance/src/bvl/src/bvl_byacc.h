
/* ###--------------------------------------------------------------### */
/* file		: bvl_byacc.h						*/
/* date		: Mar  8 2000						*/
/* version	: v114							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/* ###--------------------------------------------------------------### */

static pNode          BVL_BDDPNT = NULL;/* BDD pointer			*/
static char          *BVL_MODNAM;	/* curnennt model name		*/
static bvl_ablstr     BVL_SLCEXP;	/* structure filled with the	*/

static bvl_ablstr     BVL_EMPSTR;	/* empty structure used with NOT*/
static char          *BVL_LBLNAM = NULL;/* label			*/
static struct chain  *BVL_NM1LST = NULL;/* 1-st name liste		*/
static struct chain  *BVL_GRDLST = NULL;/* list of guard's ABL		*/
static struct chain  *BVL_CNDLST = NULL;/* list of conditions (ABL)	*/
static struct chain  *BVL_VALLST = NULL;/* list of waveforms (ABL)	*/
static struct befig  *BVL_BEFPNT = NULL;/* current BEFIG pointer	*/
static struct begen  *BVL_GENPNT = NULL;
static struct beden **dic        = NULL;/* dictionary			*/

static float          BVL_CNVFS  = 1.0 ;/* convert Fs			*/
static float          BVL_CNVPS  = 1.0 ;/* convert Ps			*/
static float          BVL_CNVNS  = 1.0 ;/* convert Ns			*/
static float          BVL_CNVUS  = 1.0 ;/* convert Us			*/
static float          BVL_CNVMS  = 1.0 ;/* convert Ms			*/

static struct chain  *BVL_INTLST = NULL;
