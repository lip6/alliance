
/* ###--------------------------------------------------------------### */
/* file		: bvl114.h						*/
/* date		: Feb 28 2000						*/
/* version	: v114							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* contents	: defines and structure definitions used in BVL library	*/
/* ###--------------------------------------------------------------### */

#ifndef BEH_BVLDEF
#define BEH_BVLDEF

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define	BVL_TRACE        0x00000001	/* print messages when parsing	*/
#define	BVL_KEEPAUX      0x00000002	/* keep internal signals	*/
#define	BVL_CHECKEMPTY   0x00000004	/* check for empty architecture	*/

	/* ###------------------------------------------------------### */
	/*    data structures						*/
	/* ###------------------------------------------------------### */

typedef struct bvl_expr			/* a signal's expression	*/
  {
  char          *IDENT   ;		/* identifier or constant name	*/
  unsigned int   TIME    ;		/* waveform's delay		*/
  struct chain  *LIST_ABL;		/* pointer on a list of ABL	*/
  short          WIDTH   ;		/* width of bit vector		*/
  }
bvl_ablstr;

typedef struct				/* an object name		*/
  {
  char          *NAME   ;		/* identifier name		*/
  short          LEFT   ;		/* vector's left index		*/
  short          RIGHT  ;		/* vector's right index		*/
  char           FLAG   ;		/* scalar ('S') or array 'A'	*/
  }
bvl_name;

struct g_type
  {
  int  VALU;
  char FLAG;
  };


	/* ###------------------------------------------------------### */
	/*    global variables						*/
	/* ###------------------------------------------------------### */

extern int           BVL_AUXMOD   ;	/* don't keep internal sig (=1)	*/
extern char          BVL_ERRFLG   ;	/* error flag                   */
extern char          BVL_CURFIL [];	/* current file name		*/
extern struct befig *BVL_HEDFIG   ;	/* list of descriptions		*/

	/* ###------------------------------------------------------### */
	/*    functions							*/
	/* ###------------------------------------------------------### */

extern char         *bvl_getvers   ();
extern void          bvl_error     ();
extern char         *bvl_abl2str   ();
extern struct bepor *bvl_vpor      ();
extern struct beaux *bvl_vaux      ();
extern struct bebux *bvl_vbux      ();
extern struct bereg *bvl_vreg      ();

extern char         *bvl_printabl  ();
extern void          bvl_message   ();

extern struct befig *vhdlloadbefig ();
extern int           vhdlsavebefig ();

#endif
