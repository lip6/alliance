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
/*																		*/
/* file		: bvl_bedef.h												*/
/* date		: Jun 15 1992												*/
/* version	: v102														*/
/* author	: P.BAZARGAN, M.HANAFI										*/
/* content	: declaration of define used by yacc						*/
/*																		*/
/* ###--------------------------------------------------------------### */

#define CTP_LG_MC 15
#define CTP_NB_MC ((tab_mc -1 + sizeof(tab_mc)/ (sizeof(tab_mc[0])) - tab_mc+1))

#define CTP_CSTDFN     1		/* constant			 */
#define CTP_TPEDFN     2		/* type				 */
#define CTP_FORDFN     3		/* formula			 */
#define CTP_VARDFN     4                /* variable                      */
#define CTP_LABDFN     5                /* label                         */

#define CTP_MODDFN     0                /* mod_val field of dct_recrd    */
#define CTP_SYMDFN     1                /* sym_val field of dct_recrd    */
#define CTP_TYPDFN     2                /* type_val field of dct_recrd    */
#define CTP_SUNDFN     3                /* sign_val                      */
#define CTP_LBLDFN     4                /* lbl_val field of dct_recrd    */
#define CTP_WMXDFN     5                /* wmx_val field of dct_recrd    */
#define CTP_WMNDFN     6                /* wmn_val field of dct_recrd    */
#define CTP_ATLDFN     7                /* attr_left_val */
#define CTP_ATRDFN     8                /* attr_left_val */
#define CTP_PNTDFN     9                /* pnt_val field of dct_recrd    */

#define CTP_UPTDFN     1		/* direction is up		 */
#define CTP_DWTDFN     2		/* direction is down		 */
#define CTP_IDXDFN     3

#define CTP_EVENT      1
#define CTP_STABLE     2
#define CTP_LEFT       3
#define CTP_RIGHT      4
#define CTP_HIGH       5
#define CTP_LOW        6
#define CTP_LENGTH     7
#define CTP_RANGE      8
#define CTP_REV_RANGE  9

/* ###---------------------------------------------------------------### */

#define NE 	  109
#define EQ 	  110
#define GT 	  111
#define GE 	  112
#define LT 	  113
#define LE 	  114
#define NOPI 	  115
#define NOPS 	  116
#define ANDM 	  117
#define CONC 	  118
#define CONVRT 	  119
#define SUBVAR 	  120
#define EMPTYOP   121

#define CTP_UNGDFN 	 0
#define CTP_GRDDFN 	 1

/* ###---------------------------------------------------------------### */

#define CTP_INTDFN     0		/* int_val field of dtc_recrd    */

#define CTP_ALODFN     60		/* nbr of allocations		*/
#define CTP_HSZDFN     97		/* size of dictionnary: entries	*/
