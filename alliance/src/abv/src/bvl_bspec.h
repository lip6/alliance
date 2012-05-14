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

struct begen      *bvl_addgen ();
struct chain      *bvl_cpyabllst ();
bvl_ablstr         bvl_cpyablstr ();
extern bvl_ablstr bvl_crtabl ( short      oper, bvl_ablstr expr1, bvl_ablstr expr2, long        left, long        right );
extern void bvl_select ( 
bvl_ablstr   *result,
struct chain *pt_str,		/* pointer on a list of choices		*/
bddnode     **pt_bdd,		/* used to check if a choice is legal	*/
bvl_ablstr   pt_ablstr ); /* tested expression			*/
extern void bvl_error();
extern long bvl_tobin ( char *trg, char *src, long   left, long   right );
