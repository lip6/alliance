
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: bvl_utdef.h						*/
/* date		: Jun 18 1992						*/
/* version	: v103							*/
/* author	: L.A.TABUSSE						*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*									*/
/* ###--------------------------------------------------------------### */

#define BVL_ICNDFN     1                /* input 	port             */
#define BVL_OCNDFN     2                /* output 	port             */
#define BVL_BCNDFN     3                /* inout 	port             */

#define BVL_BITDFN     8                /* bit 		type             */
#define BVL_MUXDFN    16                /* mux_bit 	type             */
#define BVL_WORDFN    24                /* wor_bit 	type             */
#define BVL_RBIDFN    32                /* reg_bit 	type             */

#define BVL_NORDFN   128                /* non guarded 	signal           */
#define BVL_BUSDFN   256                /* guarded 	signal (bus)     */
#define BVL_REGDFN   384                /* guarded 	signal (register)*/

#define BVL_NAMDFN 0
#define BVL_NEWDFN 1

#define BVL_MODDFN     0		/* mod_val field of dct_recrd	 */
#define BVL_SIGDFN     1		/* sig_val field of dct_recrd	 */
#define BVL_CCNDFN     2		/* ccn_val field of dct_recrd	 */
#define BVL_RNGDFN     3		/* rng_val field of dct_recrd	 */
#define BVL_LBLDFN     4		/* lbl_val field of dct_recrd	 */
#define BVL_WMXDFN     5                /* wmx_val field of dct_recrd    */
#define BVL_WMNDFN     6                /* wmn_val field of dct_recrd    */
#define BVL_PNTDFN     7                /* pnt_val field of dct_recrd    */

#define BVL_MXRDFN 30
