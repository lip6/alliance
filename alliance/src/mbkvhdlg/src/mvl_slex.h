
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_slex.h						*/
/* date		: Nov  6 91						*/
/* author	: TABUSSE L.A.						*/
/*									*/
/* content	: declaration of functions and global variables used by	*/
/*		  lex							*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mvl_stdef.h"

extern int   MVL_LINNUM;		/* file's line number		*/

typedef struct {
	char nom[MVL_LG_MC];
	int kval;
	} el_mc;

