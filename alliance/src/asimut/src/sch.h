
/* ###--------------------------------------------------------------### */
/* file		: sch110.h						*/
/* date		: Aug 10 1999						*/
/* version	: v110							*/
/* author	: Nizar ABDALLAH, Pirouz BAZARGAN SABET			*/
/* contents	: defines and structure definitions used in SCH library	*/
/* ###--------------------------------------------------------------### */

#ifndef SCH_SCHDEF
#define SCH_SCHDEF

#include "mut.h"

	/* ###------------------------------------------------------### */
	/*    basic defines						*/
	/* ###------------------------------------------------------### */

#define SCH_DFN_TRABLOCK	1024	/* size of block		*/
#define SCH_DFN_ENTBLOCK	 256	/* size of block		*/
#define SCH_DFN_WAVBLOCK	 256	/* size of block		*/

#define SCH_TRA_DELNONE		0x00	/* transcation flag: no action	*/
#define SCH_TRA_DELAFTER	0x01	/* transcation flag: del after	*/
#define SCH_TRA_DELBEFORE	0x02	/* transcation flag: del before	*/

#define SCH_TRA_ENTFIRST	0x04	/* transcation flag: 1 of list	*/

	/* ###------------------------------------------------------### */
	/*    complexe defines						*/
	/* ###------------------------------------------------------### */

#define SCH_TRA_DELALL		SCH_TRA_DELAFTER | SCH_TRA_DELBEFORE

	/* ###------------------------------------------------------### */
	/*    structure definitions					*/
	/* ###------------------------------------------------------### */

struct shdul			/* scheduler				*/
  {
  struct shdul   *NEXT     ;	/* next scheduler			*/
  void           *LABEL    ;	/* scheduler's label			*/
  struct shent  **TTABLE   ;	/* time table				*/
  struct shwav  **ITABLE   ;	/* identifier table			*/
  unsigned int    CURTIME  ;	/* current absolute time		*/
  };

struct shtra			/* transaction				*/
  {
  struct shtra   *NEXT     ;	/* next     transaction			*/
  struct shtra   *PREV     ;	/* previous transaction			*/
  struct shtra   *WAVE     ;	/* waveform for the same identifier	*/
  unsigned int    DATE     ;	/* transaction's date			*/
  void           *IDENT    ;	/* main   identifier			*/
  void           *SCDID    ;	/* second identifier			*/
  unsigned int    UFLAG    ;	/* user's flags				*/
  unsigned char   COND     ;	/* condition value			*/
  unsigned char   VALU     ;	/* value				*/
  unsigned short  FLAGS    ;	/* flags				*/
  };

struct shent			/* scheduler entry			*/
  {
  struct shent   *NEXT     ;	/* next entry				*/
  unsigned int    DATE     ;	/* entry's date (high order bits)	*/
  struct shtra   *TRANS [8];	/* list of transactions			*/
  };

struct shwav			/* waveform				*/
  {
  struct shwav   *NEXT     ;	/* next waveform			*/
  void           *IDENT    ;	/* main identifier			*/
  struct shtra   *TRANS    ;	/* list of transactions			*/
  };

	/* ###------------------------------------------------------### */
	/*    global variables						*/
	/* ###------------------------------------------------------### */

extern struct shtra  *SCH_FREE_SHTRA   ;
extern struct shent  *SCH_FREE_SHENT   ;
extern struct shwav  *SCH_FREE_SHWAV   ;

	/* ###------------------------------------------------------### */
	/*    functions							*/
	/* ###------------------------------------------------------### */

extern void          sch_bug      ();
extern int           sch_error    ();
extern void          sch_message  ();

extern void          sch_debug    ();

extern unsigned int  sch_hash     ();
extern struct shtra *sch_addshtra ();
extern struct shwav *sch_addshwav ();
extern struct shent *sch_addshent ();
extern struct shtra *sch_rmventry ();
extern void          sch_insentry ();
extern void          sch_inswave  ();

extern struct shdul *sch_CreateScheduler        ();
extern void          sch_AddTranscation         ();
extern struct shtra *sch_GetCurrentTransactions ();
extern int           sch_GotoNextTime           ();
extern int           sch_GetNextTime            ();
extern void          sch_FreeTransactions       ();
extern unsigned int  sch_GetCurrentTime         ();

#endif
