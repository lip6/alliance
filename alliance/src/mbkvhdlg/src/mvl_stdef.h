/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_stdef.h						*/
/* date		: Nov 6 1991						*/
/* author	: P.BAZARGAN, L.A.TABUSSE, VUONG H.N., D.HOMMAIS        */
/* content	: declaration of defines used in asimut 's structural   */
/* 		  function						*/
/*									*/
/* ###--------------------------------------------------------------### */

#define MVL_LG_MC 15
#define MVL_NB_MC ((tab_mc-1 + sizeof(tab_mc)/(sizeof(tab_mc[0])) - tab_mc+1))

/* ###--------------------------------------------------------------### */

#define MVL_UKNDFN     0		/* unknown port map mode	*/
#define MVL_EXPDFN     1		/* explicit port map		*/
#define MVL_IMPDFN     2		/* implicit port map		*/

/* ###--------------------------------------------------------------### */

#define MVL_ROTDFN     1		/* root model			*/	
#define MVL_CHDDFN     2		/* child model			*/

#define MVL_ICNDFN     1		/* input port			*/
#define MVL_OCNDFN     2		/* output port			*/
#define MVL_BCNDFN     3		/* inout port			*/
#define MVL_XCNDFN     4		/* linkage port			*/

#define MVL_BITDFN     8		/* bit type			*/
#define MVL_MUXDFN    16		/* mux_bit type			*/
#define MVL_WORDFN    24		/* wor_bit type			*/
#define MVL_BTVDFN    40                /* bit_vector   type            */
#define MVL_MXVDFN    48                /* mux_vector   type            */
#define MVL_WRVDFN    56                /* wor_vector   type            */
#define MVL_RGVDFN    64                /* reg_vector   type            */
#define MVL_CVTDFN    72                /* convertion   type            */
#define MVL_BOLDFN    80                /* boolean      type            */

#define MVL_NATDFN    88                /* natural      type            */
#define MVL_NAVDFN    96                /* nat_vector   type            */
#define MVL_STRDFN   104                /* string       type            */
#define MVL_LSTDFN   112                /* list         type            */
#define MVL_ARGDFN   120                /* arg          type            */

#define MVL_NORDFN   128		/* non guarded signal		*/
#define MVL_BUSDFN   256		/* guarded signal (bus)		*/

#define MVL_MODMSK     7                /* signal mode  mask (3 bits)   */
#define MVL_TYPMSK   120                /* signal type  mask (4 bits)   */
#define MVL_GRDMSK   896                /* signal guard mask (3 bits)   */

#define MVL_MODDFN     0		/* mod_val field of dct_recrd	*/
#define MVL_SIGDFN     1		/* sig_val field of dct_recrd	*/
#define MVL_CCNDFN     2		/* ccn_val field of dct_recrd	*/
#define MVL_RNGDFN     3		/* rng_val field of dct_recrd	*/
#define MVL_LBLDFN     4		/* lbl_val field of dct_recrd	*/
#define MVL_RGTDFN     5                /* rgt_val field of dct_recrd   */
#define MVL_LFTDFN     6                /* lft_val field of dct_recrd   */
#define MVL_PNTDFN     7		/* pnt_val field of dct_recrd	*/

#define MVL_UPTDFN     1                /* direction is up               */
#define MVL_DWTDFN     0                /* direction is down             */
