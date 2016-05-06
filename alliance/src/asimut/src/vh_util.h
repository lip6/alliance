
/* ###--------------------------------------------------------------### */
/* file		: vh_util.h						*/
/* date		: Nov 13 1995						*/
/* version	: v3.0							*/
/* authors	: VUONG H.N., L.A. TABUSSE, P. BAZARGAN			*/
/* content	: contains defines, external variables and functions used*/
/*		  by utility functions					*/
/* ###--------------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*   defines							*/
	/* ###------------------------------------------------------### */

#define VHU_NOALIGN     0
#define VHU_ALIGN       1

#define VHU_MEMALC   4096

	/* ###------------------------------------------------------### */
	/*   functions							*/
	/* ###------------------------------------------------------### */

extern char *vhu_alloc     ();
extern void  vhu_splitname ();
extern char *vhu_avers     ();
extern void  vhu_warning   ();
extern void  vhu_toolbug   ();
extern void  vhu_message   ();
extern int   vhu_error     ();
