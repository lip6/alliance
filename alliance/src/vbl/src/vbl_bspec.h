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
/* file		: bvl_bspec.h						*/
/* date		: Jun 15 1992						*/
/* author	: TABUSSE L.A.						*/
/* content	: declaration of functions and global variables used by	*/
/*		  bvl_bspec.c						*/
/*									*/
/* ###--------------------------------------------------------------### */

typedef struct vbl_expr
  {
  char          *IDENT;                 /* identifier or constant name  */
  vexexpr       *VEX;                   /* pointer on bvl_abllst list   */
  short          TYPE;
  int            WIDTH;                 /* width of bit vector          */
  char           SIGNED;
  char           AGGREG;
  }
vbl_vexstr;

typedef struct
  {
  char          *NAME;                  /* identifier name              */
  long           LEFT;                  /* vector's left index          */
  long           RIGHT;                 /* vector's right index         */
  char           FLAG;                  /* scalar ('S') or array 'A'    */
  char           TYPE;                  /* scalar ('S') or array 'A'    */
  char           DYNAMIC;               /* Left and Right are static    */
  char           SIGNED;
  char           AGGREG;
  }
vbl_name;

struct g_type
  {
  int  VALU;
  char FLAG;
  };

struct choice_chain
  {
  struct choice_chain* NEXT;
  chain_list  *VALUES;
  unsigned int  SIZE;
  struct ptype *INSTRUCTION;
  };


extern vbl_vexstr    VBL_EMPSTR;
extern struct vbfig *VBL_HEADFIG;
extern struct vbmap *VBL_GENMAP;


extern vbl_vexstr         vbl_crtvex ();
extern char 		  *vbl_numtobin ();
extern char	          *vbl_enumbitstring();
extern int                vbl_codeSize();
extern vexexpr           *vbl_dynamicvexatom();
extern int                vbl_intSize();
extern int                vbl_vextonum();
extern vbtyp_list *vbl_getbasetype();
  extern void               vbl_error();
  extern int                vbl_tobin();

