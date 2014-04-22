
/* ###---------------------------------------------------------------### */
/*                                                                       */
/* file         : mvl_utdef.h                                             */
/* date         : Nov  6 91                                              */
/* author       : L.A.TABUSSE 						 */
/*                                                                       */
/* content      : declaration of external functions and global variables */
/*                used by yacc                                           */
/*                                                                       */
/* ###---------------------------------------------------------------### */


#define MVL_ROTDFN    1			/* root model			 */	
#define MVL_CHDDFN    2			/* child model			 */


#define MVL_ICNDFN     1                /* input 	port             */
#define MVL_OCNDFN     2                /* output 	port             */
#define MVL_BCNDFN     3                /* inout 	port             */

#define MVL_BITDFN     8                /* bit 		type             */
#define MVL_MUXDFN    16                /* mux_bit 	type             */
#define MVL_WORDFN    24                /* wor_bit 	type             */
#define MVL_RBIDFN    32                /* reg_bit 	type             */
#define MVL_BTVDFN    40                /* bit_vector 	type             */
#define MVL_MXVDFN    48                /* mux_vector 	type             */
#define MVL_WRVDFN    56                /* wor_vector 	type             */
#define MVL_RGVDFN    64                /* reg_vector 	type             */
#define MVL_CVTDFN    72                /* convertion 	type             */
#define MVL_BOLDFN    80                /* boolean	type           	 */

#define MVL_NORDFN   128                /* non guarded 	signal           */
#define MVL_BUSDFN   256                /* guarded 	signal (bus)     */
#define MVL_REGDFN   384                /* guarded 	signal (register)*/

#define MVL_MODMSK     7                /* signal mode  mask (3 bits)    */
#define MVL_TYPMSK   120                /* signal type  mask (4 bits)    */
#define MVL_GRDMSK   896                /* signal guard mask (3 bits)    */

#define MVL_NAMDFN 0
#define MVL_NEWDFN 1

#define MVL_MODDFN     0		/* mod_val field of dct_recrd	 */
#define MVL_SIGDFN     1		/* sig_val field of dct_recrd	 */
#define MVL_CCNDFN     2		/* ccn_val field of dct_recrd	 */
#define MVL_RNGDFN     3		/* rng_val field of dct_recrd	 */
#define MVL_LBLDFN     4		/* lbl_val field of dct_recrd	 */
#define MVL_WMXDFN     5                /* wmx_val field of dct_recrd    */
#define MVL_WMNDFN     6                /* wmn_val field of dct_recrd    */
#define MVL_PNTDFN     7                /* pnt_val field of dct_recrd    */

#define MVL_ALODFN     60		/* minimal size of allocation	 */
					/* for dct_entry and dct_recrd	 */
#define MVL_HSZDFN     97		/* number of entry points in the */
					/* dictionnary			 */
#define MVL_MXRDFN 30
