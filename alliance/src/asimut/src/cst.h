
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: cst100.h						*/
/* date		: Dec 20 1994						*/
/* version	: v107							*/
/* author	: DUNOYER Julien					*/
/* contents	: defines and structure definitions used in CST library	*/
/*									*/
/* ###--------------------------------------------------------------### */

#ifndef CST_CSTDEF
#define CST_CSTDEF

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define CST_SUCCEED             0x00	/* function call : succeed      */
#define CST_FAILED              0x01	/* function call : failed       */
#define CST_WARNING             0x80	/* function call : warning      */
#define CST_ERROR               0x40	/* function call : error        */
#define CST_BUG                 0x20	/* function call : bug          */
#define CST_EXIT                0x10	/* function call : exit         */

#define CST_MSG__BUG            0x01	/* message level : bug		*/
#define CST_MSG__ERR            0x02	/* message level : error	*/
#define CST_MSG__WARN           0x04	/* message level : warning	*/
#define CST_MSG__MESG           0x08	/* message level : message	*/

	/* ###------------------------------------------------------### */
	/*    global variables definition                               */
	/* ###------------------------------------------------------### */

extern unsigned long  CST_STATUS;	/* function calls status	*/

extern unsigned long  CST_MSGLVL;	/* message level		*/

extern unsigned int   CST_ERRCOD;	/* error code			*/
extern unsigned int   CST_ERRCNT;	/* error count			*/
extern unsigned int   CST_MAXERR;	/* maximum error count		*/

	/* ###------------------------------------------------------### */
	/*    structure definitions					*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    macros definitions					*/
	/* ###------------------------------------------------------### */

#define CST_ELEMENT_COMPARE( elt1, elt2 )                    \
         (long)((unsigned long)(elt1) - (unsigned long)(elt2))

	/* ###------------------------------------------------------### */
	/*    functions							*/
	/* ###------------------------------------------------------### */

extern void          cst_error ();

extern chain_list   *cst_AddSetElement ();
extern chain_list   *cst_RemSetElement ();

extern int           cst_IsInSetElement ();
extern int           cst_SetCompare ();
extern int           cst_DisjunctiveSet ();

extern chain_list   *cst_CreateUnionSet ();
extern chain_list   *cst_CreateIntersectionSet ();
extern chain_list   *cst_CreateComplementSet ();

extern void         *cst_GetFirstElement ();
extern void         *cst_GetNextElement ();
extern void         *cst_GetLastElement ();
extern void         *cst_GetPreviousElement ();
#endif
