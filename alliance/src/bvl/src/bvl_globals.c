
/* ###--------------------------------------------------------------### */
/* file		: bvl_globals.c						*/
/* date		: Feb 28 2000						*/
/* version	: v114							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* description	: This file contains declaration of global variables	*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

long           BVL_AUXMOD      ;		/* don't keep internal sig (=1)	*/
char          BVL_ERRFLG      = 0   ;	/* error flag                  	*/
char          BVL_CURFIL [256];		/* current file name		*/
struct befig *BVL_HEDFIG      = NULL;	/* list of descriptions		*/
