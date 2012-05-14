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
/* file		: fbl_utdef.h												*/
/* date		: Jun 18 1992												*/
/* version	: v100														*/
/* author	: L.A.TABUSSE												*/
/* content	: declaration of external functions and global variables	*/
/*		  used by yacc													*/
/*																		*/
/* ###--------------------------------------------------------------### */
#define FBL_ICNDFN     1                /* input        port             */
#define FBL_OCNDFN     2                /* output       port             */
#define FBL_BCNDFN     3                /* inout        port             */
#define FBL_CSTDFN     4                /* constant                      */
#define FBL_TPEDFN     5                /* type                          */
#define FBL_SGNDFN     6                /* signal                        */
#define FBL_STBDFN     7                /* stable's signal               */
#define FBL_VARDFN     7                /* variable                      */

#define FBL_BITDFN     8                /* bit          type             */
#define FBL_MUXDFN    16                /* mux_bit      type             */
#define FBL_WORDFN    24                /* wor_bit      type             */
#define FBL_RBIDFN    32                /* reg_bit      type             */
#define FBL_BTVDFN    40                /* bit_vector   type             */
#define FBL_MXVDFN    48                /* mux_vector   type             */
#define FBL_WRVDFN    56                /* wor_vector   type             */
#define FBL_RGVDFN    64                /* reg_vector   type             */
#define FBL_NATDFN    88                /* natural      type             */
#define FBL_NTVDFN    96                /* nat_vector   type             */

#define FBL_NORDFN   128                /* non guarded  signal           */
#define FBL_BUSDFN   256                /* guarded      signal (bus)     */
#define FBL_REGDFN   384                /* guarded      signal (register)*/

#define FBL_MODDFN     0                /* mod_val field of dct_recrd    */
#define FBL_SIGDFN     1                /* sig_val field of dct_recrd    */
#define FBL_TYPDFN     2                /* type_val field of dct_recrd    */
#define FBL_USRDFN     3                /* user_val field of dct_recrd    */
#define FBL_LBLDFN     4                /* lbl_val field of dct_recrd    */
#define FBL_WMXDFN     5                /* wmx_val field of dct_recrd    */
#define FBL_WMNDFN     6                /* wmn_val field of dct_recrd    */
#define FBL_PNTDFN     7                /* pnt_val field of dct_recrd    */

#define FBL_NAMDFN 0
#define FBL_NEWDFN 1

#define FBL_ALODFN     60		/* minimal size of allocation	 */
					/* for dct_entry and dct_recrd	 */
#define FBL_HSZDFN     97		/* number of entry points in the */
					/* dictionnary			 */
#define FBL_MXRDFN 30
