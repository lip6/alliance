
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: bvl_byacc.h						*/
/* date		: Oct  4 1993						*/
/* version	: v108							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*								 	*/
/* ###--------------------------------------------------------------### */

typedef struct bvl_expr
  {
  char          *IDENT;			/* identifier or constant name	*/
  struct chain  *LIST_ABL;		/* pointer on bvl_abllst list	*/
  short          WIDTH;			/* width of bit vector		*/
  }
bvl_ablstr;

typedef struct
  {
  char          *NAME;			/* identifier name		*/
  short          LEFT;			/* vector's left index		*/
  short          RIGHT;			/* vector's right index		*/
  char           FLAG;			/* scalar ('S') or array 'A'	*/
  }
bvl_name;

struct g_type
  {
  int  VALU;
  char FLAG;
  };

pNode         BVL_BDDPNT;		/* BDD pointer			*/
char         *BVL_MODNAM;		/* curnennt model name		*/
bvl_ablstr    BVL_SLCEXP;		/* structure filled with the	*/
bvl_ablstr    BVL_EMPSTR;		/* empty structure used with NOT*/

extern int    BVL_AUXMOD;		/* simplify internal sig (= 1)	*/
 
static char         *BVL_LBLNAM = NULL;	/* label			*/
static struct chain *BVL_NM1LST = NULL;	/* 1-st name liste		*/
static struct chain *BVL_GRDLST = NULL;	/* list of guard's ABL		*/
static struct chain *BVL_CNDLST = NULL;	/* list of conditions (ABL)	*/
static struct chain *BVL_VALLST = NULL;	/* list of waveforms (ABL)	*/
static struct befig *BVL_BEFPNT = NULL;	/* current BEFIG pointer	*/

static struct beden **dic = NULL;	/* dictionary			*/

struct chain *BVL_INTLST = NULL;
struct begen *BVL_GENPNT = NULL;

extern char          BVL_ERRFLG;	/* set to 1 in case of error	*/
extern struct befig *BVL_HEDFIG;	/* head of befigs		*/

extern struct begen      *bvl_addgen();

extern bvl_ablstr         bvl_crtabl();
extern bvl_ablstr         bvl_cpyablstr();
extern void               bvl_select();
extern void               bvl_error();
extern void              *addstr ();
static struct dct_entry **initab ();
static void               addtab ();
static int                chktab ();
static void               fretab ();
