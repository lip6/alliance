
/* ###--------------------------------------------------------------### */
/* file		: vh_globals.c						*/
/* date		: Nov 15 1995						*/
/* version	: v2.01							*/
/* author	: Pirouz BARAGAN SABET					*/
/* description	: VHDL compiler and simulation tool.			*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "mlo.h"
#include "log.h"
#include "beh.h"
#include "sch.h"
#include "vh_ltype.h"

struct lofig  *VHS_LOFPNT  = NULL;
struct lkdfig *VHL_HEDLKF  = NULL;
struct shdul  *VHX_SHDUL   = NULL;

struct ptype  *VHX_EXECUTE = NULL;

struct chain  *VHX_SIGUPD  = NULL;
struct chain  *VHX_BUSUPD  = NULL;
struct chain  *VHX_BUXUPD  = NULL;
struct chain  *VHX_REGUPD  = NULL;
struct chain  *VHX_AUXUPD  = NULL;

unsigned int   VHX_BUSERR  = 0   ;
unsigned int   VHX_BUXERR  = 0   ;
unsigned int   VHX_REGERR  = 0   ;
unsigned int   VHX_MSGERR  = 0   ;

unsigned int  *VHX_VECTOR_TRANS = NULL;
unsigned char *VHX_VECTOR_VALUE = NULL;
unsigned int  *VHX_VECTOR_PROBA = NULL;

struct lkdmdl *VHL_MODELS  = NULL;

unsigned char  VHX_SCH_TRA_MODE  = SCH_TRA_DELAFTER | SCH_TRA_DELBEFORE ;
