
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: bvl_bspec.h						*/
/* date		: Jun 15 1992						*/
/* author	: TABUSSE L.A.						*/
/* content	: declaration of functions and global variables used by	*/
/*		  bvl_bspec.c						*/
/*									*/
/* ###--------------------------------------------------------------### */

extern bvl_ablstr    BVL_EMPSTR;

extern chain_list *bddToAbl () ;

struct begen      *bvl_addgen ();
struct chain      *bvl_cpyabllst ();
bvl_ablstr         bvl_cpyablstr ();
bvl_ablstr         bvl_crtabl ();
void               bvl_select ();
