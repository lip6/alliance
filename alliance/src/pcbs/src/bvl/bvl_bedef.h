
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: bvl_bedef.h						*/
/* date		: Jan 18 1993						*/
/* version	: v106							*/
/* author	: P.BAZARGAN, L.A.TABUSSE, VUONG H.N.			*/
/* content	: declaration of define used by yacc			*/
/*									*/
/* ###--------------------------------------------------------------### */

#define BVL_ICNDFN     1		/* input 	port		 */
#define BVL_OCNDFN     2		/* output 	port		 */
#define BVL_BCNDFN     3		/* inout 	port		 */
#define BVL_CSTDFN     4		/* constant			 */

#define BVL_BITDFN     8		/* bit 		type		 */
#define BVL_MUXDFN    16		/* mux_bit 	type		 */
#define BVL_WORDFN    24		/* wor_bit 	type		 */
#define BVL_RBIDFN    32		/* reg_bit 	type		 */
#define BVL_BTVDFN    40		/* bit_vector 	type		 */
#define BVL_MXVDFN    48		/* mux_vector 	type		 */
#define BVL_WRVDFN    56		/* wor_vector 	type		 */
#define BVL_RGVDFN    64		/* reg_vector 	type		 */
#define BVL_NATDFN    88 		/* natural	type		 */
#define BVL_NTVDFN    96 		/* nat_vector	type		 */

#define BVL_NORDFN   128		/* non guarded 	signal		 */
#define BVL_BUSDFN   256		/* guarded 	signal (bus)	 */
#define BVL_REGDFN   384		/* guarded 	signal (register)*/

#define BVL_MODDFN     0                /* field # 0 of dictionnary	*/
#define BVL_SIGDFN     1                /* field # 1 of dictionnary	*/
#define BVL_STBDFN     3                /* field # 3 of dictionnary	*/
#define BVL_LBLDFN     4                /* field # 4 of dictionnary	*/
#define BVL_WMXDFN     5                /* field # 5 of dictionnary	*/
#define BVL_WMNDFN     6                /* field # 6 of dictionnary	*/
#define BVL_PNTDFN     7                /* field # 7 of dictionnary	*/

#define BVL_UPTDFN     1		/* direction is up		 */
#define BVL_DWTDFN     0		/* direction is down		 */

/* ###---------------------------------------------------------------### */

#define NE 		 9
#define EQ 		10 
#define NOPI 		11
#define NOPS 		12
#define ANDM 		13
#define CONC 		14
#define CONVRT 		15

#define BVL_UNGDFN 	 0
#define BVL_GRDDFN 	 1

/* ###---------------------------------------------------------------### */

/* #define BVL_INTDFN     0 */		/* int_val field of dtc_recrd    */
