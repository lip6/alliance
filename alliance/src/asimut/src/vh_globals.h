
/* ###--------------------------------------------------------------### */
/* file		: vh_globals.h						*/
/* date		: Aug 20 1997						*/
/* version	: v3.0							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of functions and global variables used by	*/
/*		  the simulator						*/
/* ###--------------------------------------------------------------### */

extern struct lofig  *VHS_LOFPNT        ;
extern struct lkdfig *VHL_HEDLKF        ;

extern struct shdul  *VHX_SHDUL         ;

extern struct ptype  *VHX_EXECUTE       ;

extern struct chain  *VHX_SIGUPD        ;
extern struct chain  *VHX_BUSUPD        ;
extern struct chain  *VHX_BUXUPD        ;
extern struct chain  *VHX_REGUPD        ;
extern struct chain  *VHX_AUXUPD        ;

extern unsigned int   VHX_BUSERR        ;
extern unsigned int   VHX_BUXERR        ;
extern unsigned int   VHX_REGERR        ;
extern unsigned int   VHX_MSGERR        ;

extern unsigned int  *VHX_VECTOR_TRANS ;
extern unsigned char *VHX_VECTOR_VALUE ;
extern unsigned int  *VHX_VECTOR_PROBA ;

extern struct lkdmdl *VHL_MODELS        ;

extern unsigned char  VHX_SCH_TRA_MODE        ;
