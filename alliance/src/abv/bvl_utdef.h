/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail support : mailto:alliance-support@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/

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
