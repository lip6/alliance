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
/* file		: ctp_utype.h												*/
/* date		: Jun 15 1992												*/
/* version	: v100														*/
/* author	: P.BAZARGAN L.A.TABUSSE VUONG H.N.							*/
/* content	: declaration of external functions and global variables	*/
/*		  used by yacc													*/
/*																		*/
/* ###--------------------------------------------------------------### */

struct dct_entry
  {
  struct dct_entry *next;
  struct dct_recrd *data;
  char             *key;
  };

struct dct_recrd
  {
  struct dct_recrd *next;
  char             *key;
  long              fd0_val;
  long              fd1_val;
  long              fd2_val;
  long              fd3_val;
  long              fd4_val;
  long              fd5_val;
  long              fd6_val;
  long              fd7_val;
  long              fd8_val;
  long              pnt_val;
  };
