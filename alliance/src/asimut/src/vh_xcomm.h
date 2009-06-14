
/* ###--------------------------------------------------------------### */
/* file		: vh_xcomm.h						*/
/* date		: Aug 20 1997						*/
/* version	: v3.0							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of functions and global variables used by	*/
/*		  the simulation funciotns				*/
/* ###--------------------------------------------------------------### */

extern unsigned long   vhx_getvalu         ();
extern unsigned long   vhx_getsize         ();
extern char           vhx_gexeval         ();
extern char           vhx_muxbit          ();
extern char           vhx_worbit          ();
extern char           vhx_regbit          ();
extern char           vhx_bddeval         ();
extern void           vhx_savall          ();
extern void           vhx_inireg          ();
extern void           vhx_redpat          ();
extern int            vhx_wrtpat          ();
extern struct papat  *vhx_insertspypat    ();
extern void           vhx_update          ();
extern int            vhx_chkerr          ();
extern char           vhx_initfig         ();
extern void           vhx_initval         ();
extern void           vhx_core            ();
extern unsigned int   vhx_readinput       ();
extern void           vhx_writeout        ();
extern void           vhx_writebus        ();
extern char          *vhx_GetInstanceName ();
extern void           vhx_vectorstat      ();
