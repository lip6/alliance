
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: cst_globals.c						*/
/* date		: Dec 17 1994						*/
/* version	: v000							*/
/* authors	: DUNOYER Julien & ABDALLAH Nizar			*/
/* description	: global variables					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "cst.h"

unsigned long  CST_STATUS  = CST_SUCCEED;	/* function call status	*/

unsigned long  CST_MSGLVL  = CST_MSG__ERR;	/* message level	*/

unsigned int   CST_ERRCOD  =  0;		/* error code		*/
unsigned int   CST_ERRCNT  =  0;		/* error count		*/
unsigned int   CST_MAXERR  = 10;		/* maximum error count	*/
