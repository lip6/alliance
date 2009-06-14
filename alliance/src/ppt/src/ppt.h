
/* ###--------------------------------------------------------------### */
/* file		: ppt109.h						*/
/* date		: Mar  8 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: header for PPT library				*/
/* ###--------------------------------------------------------------### */

#ifndef PAT_PPTDEF
#define PAT_PPTDEF

	/* ###------------------------------------------------------### */
	/*    parser-driver defines					*/
	/* ###------------------------------------------------------### */

#define PAT_SIMU_RESULT	     1
#define PAT_USER_PREDICT     0

#define PAT_DLY__STRICT   0x01		/* comp. mode: increasing time	*/
#define PAT_DLY__IGNORE   0x02		/* comp. mode: ignore delays	*/

	/* ###------------------------------------------------------### */
	/*    global variables						*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    parser-driver functions					*/
	/* ###------------------------------------------------------### */

extern struct paseq  *pat_lodpaseq ();	/* parser			*/
extern struct paseq  *pat_prspat   ();	/* pat format parser		*/

extern long            pat_savpaseq ();	/* driver			*/
extern long            pat_drvpat   ();	/* pat format driver		*/

extern char          *pat_pattostr ();
#endif
