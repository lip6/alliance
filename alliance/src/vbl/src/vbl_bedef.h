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

#define VBL_LG_MC 15
#define VBL_NB_MC ((tab_mc -1 + sizeof(tab_mc)/ (sizeof(tab_mc[0])) - tab_mc+1))

#define VBL_ICNDFN     1		/* input 	port		 */
#define VBL_OCNDFN     2		/* output 	port		 */
#define VBL_BCNDFN     3		/* inout 	port		 */
#define VBL_CSTDFN     4		/* constant			 */
#define VBL_TPEDFN     5		/* type				 */
#define VBL_SGNDFN     6		/* signal			 */
#define VBL_FUNDFN     7		/* function       		 */
#define VBL_VARDFN     8                /* variable                      */
#define VBL_PARDFN     9                /* parameter                     */
#define VBL_PRODFN    10		/* procedure      		 */
#define VBL_ATTDFN    11                /* attribute                     */
#define VBL_COMDFN    12                /* component                     */
#define VBL_GENDFN    13                /* generic                       */
#define VBL_LABDFN    14                /* label                         */

#define VBL_MODDFN     0                /* mod_val field of dct_recrd    */
#define VBL_SYMDFN     1                /* sym_val field of dct_recrd    */
#define VBL_TYPDFN     2                /* type_val field of dct_recrd    */
#define VBL_SUNDFN     3                /* sign_val                      */
#define VBL_LBLDFN     4                /* lbl_val field of dct_recrd    */
#define VBL_WMXDFN     5                /* wmx_val field of dct_recrd    */
#define VBL_WMNDFN     6                /* wmn_val field of dct_recrd    */
#define VBL_ATLDFN     7                /* attr_left_val */
#define VBL_ATRDFN     8                /* attr_left_val */
#define VBL_PNTDFN     9                /* pnt_val field of dct_recrd    */
#define VBL_DYNDFN    10                /* dynamic_val                   */

#define VBL_UPTDFN     1		/* direction is up		 */
#define VBL_DWTDFN     2		/* direction is down		 */
#define VBL_IDXDFN     3

#define VBL_EVENT      1
#define VBL_STABLE     2
#define VBL_LEFT       3
#define VBL_RIGHT      4
#define VBL_HIGH       5
#define VBL_LOW        6
#define VBL_LENGTH     7
#define VBL_RANGE      8
#define VBL_REV_RANGE  9

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

#define VBL_UNGDFN 	 0
#define VBL_GRDDFN 	 1

/* ###---------------------------------------------------------------### */

#define VBL_INTDFN     0		/* int_val field of dtc_recrd    */

#define VBL_ALODFN     60		/* nbr of allocations		*/
#define VBL_HSZDFN     97		/* size of dictionnary: entries	*/
