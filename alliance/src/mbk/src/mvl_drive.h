/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_drive.h						*/
/* date		: Nov 12 91						*/
/* author	: L.A. TABUSSE & P. BAZARGAN-SABET & VUONG H.N.		*/
/*									*/
/* content	: declaration of functions and global variables used by	*/
/*		  the structural MBK-> VHDL driver			*/
/*									*/
/* ###--------------------------------------------------------------### */


extern void	mvl_addtab();
extern int	mvl_chktab();
extern void     mvl_toolbug();
extern void     mvl_message();
extern void     mvl_error();
extern char     *mvl_vhdlname();
extern void     *mvl_name();
extern char     *mvl_abl2str();
extern void     *mvl_vectnam();
extern char     mvl_chkconsistency();
