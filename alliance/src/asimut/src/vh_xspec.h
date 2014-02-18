
/* ###--------------------------------------------------------------### */
/* file		: vh_xspec.h						*/
/* date		: Aug 20 1997						*/
/* version	: v3.0							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of functions and global variables used by	*/
/*		  the simulation funciotns				*/
/* ###--------------------------------------------------------------### */

#define CTXEVT	 6
#define CTXPRB   7
#define CTXLST   8
#define CTXSEV   9
#define CTXMEV  10

extern void           traceX        ();
extern unsigned int   getXcount     ();
extern void           vhx_execall   ();
extern void           vhx_addexec   ();
extern int            vhx_execute   ();
extern unsigned int   vhx_addtra    ();
extern void           vhx_redtra    ();
extern void           vhx_supdate   ();

extern void           st_wrtsta    ();
extern void           st_wrtstadly ();
extern void           st_wrtstabis ();
