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
#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/* ###--------------------------------------------------------------### */
/*									*/
/* file		: fbl_bspec.h						*/
/* date		: Jun 15 1992						*/
/* author	: TABUSSE L.A.						*/
/* content	: declaration of functions and global variables used by	*/
/*		  fbl_bspec.c						*/
/*									*/
/* ###--------------------------------------------------------------### */

typedef struct fbl_expr
  {
  char     *IDENT;   /* identifier or constant name    */
  struct chain  *LIST_ABL;  /* pointer on fbl_abllst list    */
  char      TYPE;
  short     WIDTH;         /* width of bit vector      */
  }
fbl_ablstr;

extern fbl_ablstr    FBL_EMPSTR;

extern ablexpr     *bddToAbl () ;

  extern  struct chain * fbl_cpyabllst __P((struct chain *abllst));
  extern     fbl_ablstr  fbl_cpyablstr __P((fbl_ablstr ablstr));
  extern     fbl_ablstr  fbl_crtabl __P((short oper, fbl_ablstr expr1, fbl_ablstr expr2, int left, int right));
  extern          char * fbl_numtobin __P((int num));
  extern            int  fbl_tobin __P((char *trg, char *src, int left, int right));
  extern           void  loc_fbl_error __P((int code, char *str1));
  extern           void  fbl_toolbug __P((int code, char *str1, char *str2, int nbr1));

# define fbl_error( C, S ) \
  fprintf( stderr, "%s.%d: ", basename(__FILE__),__LINE__ ); loc_fbl_error( C, S );
