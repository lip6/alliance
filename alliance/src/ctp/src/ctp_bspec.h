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
/*									*/
/* file		: bvl_bspec.h						*/
/* date		: Jun 15 1992						*/
/* author	: TABUSSE L.A.						*/
/* content	: declaration of functions and global variables used by	*/
/*		  bvl_bspec.c						*/
/*									*/
/* ###--------------------------------------------------------------### */

typedef struct ctp_expr
  {
  char          *IDENT;                 /* identifier or constant name  */
  vexexpr       *VEX;                   /* pointer on bvl_abllst list   */
  short          TYPE;
  int            WIDTH;                 /* width of bit vector          */
  char           SIGNED;
  char           AGGREG;
  }
ctp_vexstr;

typedef struct
  {
  char          *NAME;                  /* identifier name              */
  long           LEFT;                  /* vector's left index          */
  long           RIGHT;                 /* vector's right index         */
  char           FLAG;                  /* scalar ('S') or array 'A'    */
  char           TYPE;                  /* scalar ('S') or array 'A'    */
  char           SIGNED;
  }
ctp_name;

extern ctp_vexstr    CTP_EMPSTR;
extern ctlfig_list  *CTP_HEADFIG;
extern char         *CTP_FILENAME;

extern ctp_vexstr          ctp_crtvex ();
extern char 		  *ctp_numtobin ();
extern char	          *ctp_enumbitstring();
extern int                 ctp_codeSize();
extern vexexpr            *ctp_dynamicvexatom();
extern int                 ctp_intSize();
extern int                 ctp_vextonum();
extern ctltype_list       *ctp_getbasetype();
  extern void              ctp_error();
  extern int               ctp_tobin();

