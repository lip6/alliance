
/* ###--------------------------------------------------------------### */
/* file		: vh_lspec.h						*/
/* date		: Aug 20 1997						*/
/* version	: v3.0							*/
/* authors	: VUONG H.N., Pirouz BAZARGAN SABET			*/
/* content	: contains defines, external variables and funtions used*/
/*		  by the linker						*/
/* ###--------------------------------------------------------------### */

extern struct prjbvl       *vhl_addprjbvl    ();
extern struct prjrvl       *vhl_addprjrvl    ();
extern struct redlst       *vhl_addredlst    ();
extern struct simsig       *vhl_addsimsig    ();
extern struct bussig       *vhl_addbussig    ();
extern struct wrbsig       *vhl_addwrbsig    ();
extern struct wrireg       *vhl_addwrireg    ();
extern struct wribux       *vhl_addwribux    ();
extern struct wrimsg       *vhl_addwrimsg    ();
extern struct wriaux       *vhl_addwriaux    ();
extern struct wrssig       *vhl_addwrssig    ();
extern struct lkdfig       *vhl_addlkdfig    ();
extern struct lkdins       *vhl_addlkdins    ();
extern struct lkdspy       *vhl_addlkdspy    ();
extern struct ptype        *vhl_makdepend    ();
extern struct lofig        *vhl_makgst       ();
extern int                  vhl_linker       ();
extern int                  vhl_restor       ();
extern void                 vhl_resord       ();
void                     ( *vhl_iscmodel ()) ();
