/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* 
 * Purpose : constantes, externs, and data-structures
 * Date    : 05/08/93
 * Author  : Frederic Petrot <Frederic.Petrot@lip6.fr>
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1997,98
 * $Id: mlo.h,v 1.2 2002/05/14 09:10:29 xtof Exp $
 */

#ifndef _MLO_H_
#define _MLO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/* transistor type */
#define TRANSN      0
#define TRANSP      1

#define TRANSFAST   (1 << 1)
#define TRANSHVIO   (1 << 3)

#define TRANSN_FAST (TRANSN & TRANSFAST)
#define TRANSP_FAST (TRANSP & TRANSFAST)

#define TRANSN_HVIO (TRANSN & TRANSHVIO)
#define TRANSP_HVIO (TRANSP & TRANSHVIO)

#define IsTransP(type)    (type & 1)
#define IsTransN(type)    (!IsTransP(type))
#define IsTransFast(type) ((type >> 1) & 1)
#define IsTransSlow(type) (!IsTransFast(type))
#define IsTransHvio(type) ((type >> 3) & 1)

/* connector & signal direction */
#define INTERNAL    'I' /* instance connector or signal on instances only */
#define EXTERNAL    'E' /* figure connector or signal on figure and instances */

/* connector direction */
#define IN          'I'
#define OUT         'O'
#define INOUT       'B'
#define UNKNOWN     'X'
#define TRISTATE    'Z'
#define TRANSCV     'T'

/*******************************************************************************
* netlist structures types                                                     *
*******************************************************************************/
typedef struct lofig                          /* logical figure               */
{
struct lofig  *NEXT;                          /* next figure                  */
struct chain  *MODELCHAIN;                    /* list of models               */
struct locon  *LOCON;                         /* connector list head          */
struct losig  *LOSIG;                         /* signal list head             */
struct ptype  *BKSIG;                         /* signal block list head       */
struct loins  *LOINS;                         /* instance list head           */
struct lotrs  *LOTRS;                         /* transistor list head         */
char          *NAME;                          /* figure name (unique)         */
char           MODE;                           /* 'A' or 'P'                  */
struct ptype  *USER;                          /* Application specific         */
}
lofig_list;

typedef struct lotrs                           /* logical transistor          */
{
struct lotrs  *NEXT;                           /* next transistor             */
struct locon  *DRAIN;                          /* drain connector             */
struct locon  *GRID;                           /* grid connector              */
struct locon  *SOURCE;                         /* source connector            */
struct locon  *BULK;                           /* bulk connector              */
char          *TRNAME;                         /* transistor instance name    */
long           WIDTH,LENGTH;                   /* transistor width & length   */
long           PS, PD;                         /* source & drain perimeters   */
long           XS, XD;                         /* values for area computation */
long           X,Y;                            /* transistor coordinates      */
char           TYPE;                           /* transistor type             */
struct ptype  *USER;                           /* application specific        */
}
lotrs_list;

typedef struct loins                           /* logical instance            */
{
struct loins  *NEXT;                           /* next figure                 */
struct locon  *LOCON;                          /* connector list head         */
char          *INSNAME;                        /* instance name               */
char          *FIGNAME;                        /* model name                  */
struct ptype  *USER;                           /* Application specific        */
}
loins_list;

typedef struct locon                           /* logical connector           */
{
struct locon  *NEXT;                           /* next connector              */
char          *NAME;                           /* connector name (unique)     */
struct losig  *SIG;                            /* pointer on signal           */
void          *ROOT;                           /* pointer on figure/instance  */
char          DIRECTION;                       /* see defines for information */
char          TYPE;                            /* instance I  / figure E      */
struct num   *PNODE;                           /* nodes for physical connector*/
struct ptype  *USER;                           /* Application specific        */
}
locon_list;

typedef struct losig                           /* logical signal              */
{
struct losig  *NEXT;                           /* next signal                 */
struct chain  *NAMECHAIN;                      /* list of alias name          */
long          INDEX;                           /* signal index (unique)       */
char          TYPE;                            /* internal I  / external E    */
struct lorcnet *PRCN;                          /* pointer on parasitic RC     */
struct ptype  *USER;                           /* application specific        */
}
losig_list;

/*******************************************************************************
* externals for mbk netlist view                                               *
*******************************************************************************/
  extern lofig_list *HEAD_LOFIG;                 /* physical figure list head   */
  extern    lofig_list * addlofig __P((const char *name));
  extern    lofig_list * addlomodel __P((lofig_list *model, const char *name));

  extern    loins_list * addloins __P((lofig_list *ptfig, const char *insname, lofig_list *ptnewfig, chain_list *sigchain));
  extern    lotrs_list * addlotrs __P((lofig_list *ptfig, char type, long x, long y, long width, long length, long ps, long pd, long xs, long xd, losig_list *ptgrid, losig_list *ptsource, losig_list *ptdrain, losig_list *ptbulk, const char *name));
  extern    locon_list * addlocon __P((lofig_list *ptfig, const char *name, losig_list *ptsig, char dir));
  extern    losig_list * addlosig __P((lofig_list *ptfig, long index, chain_list *namechain, char type));
  /* addlosig : field capa is not used */
  /* addcapa disparait */
  extern            int  dellofig __P((const char *name));
  extern           void  freelomodel __P((lofig_list *ptmodel));
  extern            int  dellosig __P((lofig_list *ptfig, long index));
  extern            int  delloins __P((lofig_list *ptfig, const char *insname));
  extern            int  dellotrs __P((lofig_list *ptfig, lotrs_list *pttrs));
  extern            int  dellocon __P((lofig_list *ptfig, const char *name));
  extern    lofig_list * getlofig __P((const char *figname, char mode));
  extern    lofig_list * getlomodel __P((lofig_list *ptmodel, const char *name));
  extern    loins_list * getloins __P((lofig_list *ptfig, const char *name));
  extern    locon_list * getlocon __P((lofig_list *ptfig, const char *name));
  extern    losig_list * getlosig __P((lofig_list *ptfig, long index));
  extern    lotrs_list * getlotrs __P((lofig_list *ptfig, const char *name));
  extern          char * getsigname __P((losig_list *ptsig));
  extern           void  lofigchain __P((lofig_list *ptfig));
  extern           void  viewlo __P(());
  extern           void  setsigsize __P((lofig_list*, int));
  extern            int  getsigsize __P((lofig_list* ));
  extern           void  dellosiguser __P((losig_list*));
  extern           void  dellotrsuser __P((lotrs_list*));
  extern           void  delloinsuser __P((loins_list*));
  extern           void  delloconuser __P((locon_list*));
  extern           void  dellofiguser __P((lofig_list*));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MLO */
