/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*------------------------------------------------------------\
|                                                             |
| Tool    :                   GenView                         |
|                                                             |
| File    :                  D_stdlib.h                       |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   30.04.94                        |
|                                                             |
\------------------------------------------------------------*/

/*------------------------------------------------------------\
|                                                             |
|                             Constants                       |
|                                                             |
\------------------------------------------------------------*/

# ifndef D_STDLIB_H 
# define D_STDLIB_H 

# ifdef pc
# undef pc
# endif

# define PREND_SHORT(X)  *((short*)(&(X)))
# define PREND_INT(X)    *((int*)(&(X)))
# define PREND_FLOAT(X)  *((float*)(&(X)))
# define PREND_DOUBLE(X) *((double*)(&(X)))

# define READ_SHORT(X)   *((short*)(X))
# define READ_INT(X)     *((int*)(X))
# define READ_FLOAT(X)   *((float*)(X))
# define READ_DOUBLE(X)  *((double*)(X))

# define QI 0            /* code QI 0 */
# define HI 1            /* code HI 1 */
# define SI 2            /* code SI 2 */
# define DF 3            /* code DF 3 */
# define SF 4            /* code SF 4 */

/*------------------------------------------------------------\
|                                                             |
|                            Macros                           |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                             Variables                       |
|                                                             |
\------------------------------------------------------------*/

/* Internally known functions. Alphanumerical ASCII order */
  extern int taille_TabFonc;
  extern FONC TabFonc[];

/*------------------------------------------------------------\
|                                                             |
|                             Functions                       |
|                                                             |
\------------------------------------------------------------*/
extern void app_atoi(void);
extern void app_malloc(void);
extern void app_bcopy(void);
extern void app_flsbuf(void);
extern void app_strlen(void);
extern void app_strcmp(void);

extern void app_namealloc(void);

/* dans interp_pr.c */
extern void app_printf(void);
extern void app_sprintf(void);
extern void app_NAME(void);

/* dans interp_mbk.c */
extern void app_DEF_AB(void);
extern void app_DEF_PHFIG(void);
extern void app_DEF_PHINS(void);
extern void app_PHCON(void);
extern void app_PHREF(void);
extern void app_PLACE(void);
extern void app_PLACE_BOTTOM(void);
extern void app_PLACE_LEFT(void);
extern void app_PLACE_REF(void);
extern void app_PLACE_RIGHT(void);
extern void app_PLACE_TOP(void);
extern void app_PLACE_ON(void);
extern void app_COPY_UP_ALL_CON(void);
extern void app_COPY_UP_CON(void);
extern void app_COPY_UP_CON_FACE(void);
extern void app_COPY_UP_ALL_REF(void);
extern void app_COPY_UP_REF(void);
extern void app_PHSEG(void);
extern void app_COPY_UP_SEG(void);
extern void app_THRU_H(void);
extern void app_THRU_V(void);
extern void app_THRU_CON_H(void);
extern void app_THRU_CON_V(void);
extern void app_HEIGHT(void);
extern void app_WIDTH(void);
extern void app_GET_PHFIG_REF_X(void);
extern void app_GET_PHFIG_REF_Y(void);
extern void app_GET_REF_X(void);
extern void app_GET_REF_Y(void);
extern void app_GET_INS_X(void);
extern void app_GET_INS_Y(void);
extern void app_GET_CON_X(void);
extern void app_GET_CON_Y(void);
extern void app_WIRE1(void);
extern void app_WIRE2(void);
extern void app_WIRE3(void);
extern void app_PHVIA(void);
extern void app_PLACE_VIA_REF(void);
extern void app_PLACE_SEG_REF(void);
extern void app_PLACE_CON_REF(void);
extern void app_FLATTEN_PHFIG(void);
extern void app_FLATTEN_ALL_PHINS(void);
#endif
