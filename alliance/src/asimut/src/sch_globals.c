
/* ###--------------------------------------------------------------### */
/* file		: sch_globals.c						*/
/* date		: Aug  5 1997						*/
/* version	: v101							*/
/* author	: Nizar ABDALLAH, Pirouz BARAGAN SABET			*/
/* description	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "sch.h"

struct shtra  *SCH_FREE_SHTRA   = NULL;
struct shent  *SCH_FREE_SHENT   = NULL;
struct shwav  *SCH_FREE_SHWAV   = NULL;

struct shent **SCH_TIME_ENTRY   = NULL;
struct shwav **SCH_IDENT_ENTRY  = NULL;
