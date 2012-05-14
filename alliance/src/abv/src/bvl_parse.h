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
/* file		: bvl_parse.h       	                                */
/* date		: May 31 1994						*/
/* version	: v109							*/
/* author	: L.A. TABUSSE & H.G. VUONG & P. BAZARGAN-SABET         */
/* description	:							*/
/*									*/
/* ###--------------------------------------------------------------### */

char         BVL_ERRFLG = 0;		/* if = 1 no structure is made  */
long         BVL_LINNUM = 1;		/* file's line number           */
long         BVL_AUXMOD;		/* simplify internal sig (= 1)	*/
char         BVL_CURFIL[200];		/* current file's name          */
befig_list  *BVL_HEDFIG = NULL;

extern FILE *bvl_y_in;
extern int   bvl_y_parse();
extern void  bvl_y_restart();

extern char *getenv();
