/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/* ###--------------------------------------------------------------### */
/*																		*/
/* file		: fbl_bedef.h												*/
/* date		: Jun 15 1992												*/
/* version	: v102														*/
/* author	: P.BAZARGAN, M.HANAFI										*/
/* content	: declaration of define used by yacc						*/
/*																		*/
/* ###--------------------------------------------------------------### */

#define FBL_LG_MC 15
#define FBL_NB_MC ((tab_mc -1 + sizeof(tab_mc)/ (sizeof(tab_mc[0])) - tab_mc+1))

#define FBL_ICNDFN     1		/* input 	port		 */
#define FBL_OCNDFN     2		/* output 	port		 */
#define FBL_BCNDFN     3		/* inout 	port		 */
#define FBL_CSTDFN     4		/* constant			 */
#define FBL_TPEDFN     5		/* type				 */
#define FBL_SGNDFN     6		/* signal			 */

/* LUDO #define FBL_STBDFN  7 stable's signal 
#define FBL_VARDFN     7		 variable			 */

#define FBL_BITDFN     8		/* bit 		type		 */
#define FBL_MUXDFN    16		/* mux_bit 	type		 */
#define FBL_WORDFN    24		/* wor_bit 	type		 */
#define FBL_RBIDFN    32		/* reg_bit 	type		 */
#define FBL_BTVDFN    40		/* bit_vector 	type		 */
#define FBL_MXVDFN    48		/* mux_vector 	type		 */
#define FBL_WRVDFN    56		/* wor_vector 	type		 */
#define FBL_RGVDFN    64		/* reg_vector 	type		 */
#define FBL_NATDFN    88 		/* natural	type		 */
#define FBL_NTVDFN    96 		/* nat_vector	type		 */

#define FBL_NORDFN   128		/* non guarded 	signal		 */
#define FBL_BUSDFN   256		/* guarded 	signal (bus)	 */
#define FBL_REGDFN   384		/* guarded 	signal (register)*/

#define FBL_MODDFN     0                /* mod_val field of dct_recrd    */
#define FBL_SIGDFN     1                /* sig_val field of dct_recrd    */
#define FBL_TYPDFN     2                /* type_val field of dct_recrd    */
#define FBL_USRDFN     3                /* user_val field of dct_recrd    */
#define FBL_LBLDFN     4                /* lbl_val field of dct_recrd    */
#define FBL_WMXDFN     5                /* wmx_val field of dct_recrd    */
#define FBL_WMNDFN     6                /* wmn_val field of dct_recrd    */
#define FBL_PNTDFN     7                /* pnt_val field of dct_recrd    */

#define FBL_UPTDFN     1		/* direction is up		 */
#define FBL_DWTDFN     0		/* direction is down		 */

#define SEVERITY_LEVEL FBH_TYPE_SEVERITY
#define BOOLEAN  FBH_TYPE_BOOLEAN
#define BIT      FBH_TYPE_BIT
#define MUX_BIT  FBH_TYPE_MUX_BIT
#define WOR_BIT  FBH_TYPE_WOR_BIT
#define REG_BIT  FBH_TYPE_REG_BIT
#define BIT_VEC  FBH_TYPE_BIT_VEC
#define MUX_VEC  FBH_TYPE_MUX_VEC
#define WOR_VEC  FBH_TYPE_WOR_VEC
#define REG_VEC  FBH_TYPE_REG_VEC
#define NATURAL  FBH_TYPE_NATURAL 
#define INTEGER  FBH_TYPE_INTEGER	 

/* ###---------------------------------------------------------------### */

#define NE 	109
#define EQ 	110
#define GT 	111
#define GE 	112
#define LT 	113
#define LE 	114
#define NOPI 	115
#define NOPS 	116
#define ANDM 	117
#define CONC 	118
#define CONVRT 	119
#define SUBVAR 	120

#define FBL_UNGDFN 	 0
#define FBL_GRDDFN 	 1

#define FBL_STABLE       0
#define FBL_EVENT        1

/* ###---------------------------------------------------------------### */

#define FBL_INTDFN     0		/* int_val field of dtc_recrd    */

#define FBL_ALODFN     60		/* nbr of allocations		*/
#define FBL_HSZDFN     97		/* size of dictionnary: entries	*/
