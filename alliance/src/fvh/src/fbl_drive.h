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
/*																		*/
/* file		: fbl_drive.h												*/
/* date		: Nouv 05 1992												*/
/* version	: v100														*/
/* author	: VUONG H.N.												*/
/* content	: declaration of functions and global variables used by		*/
/*		  the fbhavioural decompiler									*/
/*																		*/
/* ###--------------------------------------------------------------### */


extern char *fbl_vhdlname();
  extern           void  fbl_printablfile __P((FILE *AblFile, ablexpr *Expr));
extern char *fbl_printabl();
  extern           void  fbl_toolbug __P((int code, char *str1, char *str2, int nbr1));
  extern           void  fbl_message __P((int code, char *str1, int nmb1));
  extern           void  loc_fbl_error __P((int code, char *str1));
  extern          void * fbl_vectnam __P((void *pt_list, int *left, int *right, char **name, char type));
  extern          char * fbl_vectorize __P((char *Name));

# define fbl_error( C, S ) \
  fprintf( stderr, "%s.%d: ", autbasename(__FILE__,NULL),__LINE__ ); loc_fbl_error( C, S )
