
/* ###--------------------------------------------------------------### */
/* file		: phl109.h						*/
/* date		: Mar  9 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: header for PHL library				*/
/* ###--------------------------------------------------------------### */

#ifndef PAT_PHLDEF
#define PAT_PHLDEF

	/* ###------------------------------------------------------### */
	/*    data structures						*/
	/* ###------------------------------------------------------### */

struct pause				/* structures' usage		*/
  {
  unsigned int PASEQ    ;		/* # of sequence of patterns	*/
  unsigned int PAGRP    ;		/* # of input-output arraies	*/
  unsigned int PAIOL    ;		/* # of input-outputs		*/
  unsigned int PACOM    ;		/* # of comments		*/
  unsigned int PAPAT    ;		/* # of patterns		*/
  unsigned int PAEVT    ;		/* # of events			*/
  unsigned int PAINI    ;		/* # of register's init.	*/
  unsigned int PAINS    ;		/* # of inspected instances	*/
  unsigned int CHARACTER;		/* # of characters (in strings)	*/
  };

	/* ###------------------------------------------------------### */
	/*    high-level functions					*/
	/* ###------------------------------------------------------### */

extern void          pat_debug    ();	/* pat structure displayer	*/
extern void          pat_getusage ();	/* get structures' usage	*/

#endif
