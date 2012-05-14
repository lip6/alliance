/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* 
 * Purpose : constantes, externs, and data-structures
 * Date    : 05/08/93
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * $Id: mph.h,v 1.5 2012/05/14 14:20:23 alliance Exp $
 */

#ifndef _MPH_H_
#define _MPH_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__) || defined(__cplusplus)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/* physical connector orientation  */
#define NORTH       'N' 
#define EAST        'E' 
#define SOUTH       'S' 
#define WEST        'W' 

/* physical segment type */
#define LEFT         'L'
#define UP           'U'
#define RIGHT        'R'
#define DOWN         'D'

/* physical symetry type  */
#define NOSYM        0  /* vti : standart  */
#define ROT_P        1  /* vti : Rot90cc   */
#define SYMXY        2  /* vti : Rot180    */
#define ROT_M        3  /* vti : Rot90c    */
#define SYM_X        4  /* vti : mx(x=-x)  */
#define SY_RM        5  /* vti : mxRot90cc */
#define SYM_Y        6  /* vti : my(y=-y)  */
#define SY_RP        7  /* vti : myRot90cc */

enum {
/* physical contact type  */
   CONT_POLY,
   CONT_DIF_N,
   CONT_DIF_P,
   CONT_BODY_N,
   CONT_BODY_P,
   CONT_VIA,
   CONT_VIA2,
   CONT_VIA3,
   CONT_VIA4,
   CONT_VIA5,
   CONT_VIA6,
   CONT_VIA7,
   CONT_VIA8,
   CONT_POLY2,

/* physical transistor flood fill macros */
   C_X_N,
   C_X_P,

/* turn via */
   CONT_TURN1,
   CONT_TURN2,
   CONT_TURN3,
   CONT_TURN4,
   CONT_TURN5,
   CONT_TURN6,
   CONT_TURN7,
   CONT_TURN8,
   CONT_TURN9
};

#define LAST_CONTACT  CONT_TURN9

enum {
/* physical layer code */
   NWELL,
   PWELL,
   NTIE,
   PTIE,
   NDIF,
   PDIF,
   NTRANS,
   PTRANS,
   NTRANS_FAST,
   PTRANS_FAST,
   NTRANS_HVIO,
   PTRANS_HVIO,
   POLY,
   POLY2,
   ALU1,
   ALU2,
   ALU3,
   ALU4,
   ALU5,
   ALU6,
   ALU7,
   ALU8,
   ALU9,
/* lack of layer */
   TPOLY,
   TPOLY2,
   TALU1,
   TALU2,
   TALU3,
   TALU4,
   TALU5,
   TALU6,
   TALU7,
   TALU8,
   TALU9,
/* connector segment layer code */
   CALU1,
   CALU2,
   CALU3,
   CALU4,
   CALU5,
   CALU6,
   CALU7,
   CALU8,
   CALU9
};

#define LAST_LAYER   CALU9

/*******************************************************************************
* layout structures types                                                      *
*******************************************************************************/
typedef struct phfig                           /* physical figure             */
{
struct phfig *NEXT;                            /* next figure                 */
struct chain *MODELCHAIN;                      /* list of models              */
struct phseg *PHSEG;                           /* segment list head           */
struct phvia *PHVIA;                           /* contact list head           */
struct phref *PHREF;                           /* reference list head         */
struct phins *PHINS;                           /* instance list head          */
struct phcon *PHCON;                           /* connector list head         */
char         *NAME;                            /* figure name (unique)        */
char         MODE;                             /* if 'A' all , else perif     */
long         XAB1,YAB1,XAB2,YAB2;              /* abutment box coordinates    */
struct ptype *USER;                            /* Application specific        */
}
phfig_list;

typedef struct phcon                           /* physical connector          */
{
struct phcon *NEXT;                            /* next connector              */
char         *NAME;                            /* connector name              */
long         INDEX;                            /* connector index(unique)     */
long         XCON,YCON;                        /* connector coordinates       */
long         WIDTH;                            /* connector width             */
char         ORIENT;                           /* possible values: N E S W    */
char         LAYER;                            /* physical layer              */
struct ptype *USER;                            /* Application specific        */
}
phcon_list;

typedef struct phseg                           /* physical (symbolic) segment */
{
struct phseg    *NEXT;                         /* next segment                */
char            *NAME;                         /* segment name                */
long             X1,Y1,X2,Y2;                  /* segment coordinates         */
long             WIDTH;                        /* segment width               */
char             TYPE;                         /* possibles values: L U R D   */
char             LAYER;                        /* physical layer              */
struct ptype    *USER;                         /* Application specific        */
}
phseg_list;

typedef struct phvia                           /* physical contact            */
{
struct phvia    *NEXT;                         /* next contact                */
char            *NAME;                         /* via name                    */
char             TYPE;                         /* contact type: see define    */
long             XVIA,YVIA;                    /* coordinates                 */
long             DX,DY;                        /* size of big vias            */
struct ptype    *USER;                         /* Application specific        */
}
phvia_list;

typedef struct phref                           /* physical reference          */
{
struct phref    *NEXT;                         /* next reference              */
char            *FIGNAME;                      /* reference type              */
char            *NAME;                         /* reference name (unique)     */
long             XREF,YREF;                    /* coordinates                 */
struct ptype    *USER;                         /* Application specific        */
}
phref_list;

typedef struct phins                           /* physical instance           */
{
struct phins    *NEXT;                         /* next instance               */
char            *INSNAME;                      /* instance name (unique)      */
char            *FIGNAME;                      /* model name                  */
long             XINS,YINS;                    /* coordinates                 */
char             TRANSF;                       /* possible values: cf define  */
struct ptype    *USER;                         /* Application specific        */
}
phins_list;

/*******************************************************************************
* externals for physical view                                                  *
*******************************************************************************/
extern phfig_list *HEAD_PHFIG;                 /* physical figure list head   */
  extern    phfig_list * addphfig __P((const char *figname));
  extern    phins_list * addphins __P((phfig_list *ptfig, const char *figname, const char *insname, char sym, long x, long y));
  extern    phvia_list * addphvia __P((phfig_list *ptfig, char viatype, long x, long y, long dx, long dy, const char *name));
  extern    phref_list * addphref __P((phfig_list *ptfig, const char *type, const char *name, long x, long y));
  extern    phseg_list * addphseg __P((phfig_list *ptfig, char layer, long width, long x1, long y1, long x2, long y2, const char *nodename));
  extern    phcon_list * addphcon __P((phfig_list *ptfig, char orient, const char *conname, long x, long y, char layer, long width));
  extern           void  defab __P((phfig_list *ptfig, long x1, long y1, long x2, long y2));
  extern    phfig_list * getphfig __P((const char *figname, char mode));
  extern    phins_list * getphins __P((phfig_list *ptfig, const char *insname));
  extern    phcon_list * getphcon __P((phfig_list *ptfig, const char *conname, long index));
  extern    phref_list * getphref __P((phfig_list *ptfig, const char *refname));
  extern            int  delphcon __P((phfig_list *ptfig, phcon_list *ptdelcon));
  extern            int  delphins __P((phfig_list *ptfig, const char *insname));
  extern            int  delphfig __P((const char *name));
  extern            int  delphvia __P((phfig_list *ptfig, phvia_list *ptvia));
  extern            int  delphseg __P((phfig_list *ptfig, phseg_list *ptseg));
  extern            int  delphref __P((phfig_list *ptfig, phref_list *ptref));
  extern           void  xyflat __P((long *xout, long *yout, long x, long y, long xins, long yins, long x1, long y1, long x2, long y2, char trsf));
  extern           void  invxyflat __P((long *x, long *y, long xout, long yout, long xins, long yins, long x1, long y1, long x2, long y2, char trsf));
  extern           void  viewph __P(());
  
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MPH */
