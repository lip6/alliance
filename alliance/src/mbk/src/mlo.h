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
 * Modified by pnt <Pierre.Nguyen-Tuong@lip6.fr> 2002
 * $Id: mlo.h,v 1.4 2002/08/08 19:47:37 pnt Exp $
 */

#ifndef _MLO_H_
#define _MLO_H_

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
#define TRANSCV2    'C'
#define TRANSCV3    'R'
#define TRANSCV4    'S'

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
struct locap  *LOCAP      ;                   /* capacitance list head        */
struct lores  *LORES      ;                   /* resistance list head         */
struct loself *LOSELF     ;                   /* inductor list head           */
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


/************************************************************************************/
/************************* Analogical specific structures ***************************/
/************************************************************************************/

/********************** Complementary transitor informations ************************/

/* The structure is put in the USER field of the transistor */
/* with the LOTRS_INFO ptype                                */

#define LOTRS_INFO 200208071                       /* USER field code               */

typedef struct s_lotrs_info
  {
    double AS,AD              ;                    /* Source and drain area         */
    double PS,PD              ;                    /* Source and drain perimeter    */
    int    M                  ;                    /* Number of fingers             */
  } lotrs_info                ;


/********************************* Capacitance **************************************/

#define CAPMIM    0                                /* Metal-Inter-Metal type        */
#define CAPPNWELL 1                                /* Poly-NWell type               */

#define IsCapMIM(type)    ((type == CAPMIM)    ? 1 : 0)
#define IsCapPNWELL(type) ((type == CAPPNWELL) ? 1 : 0)

typedef struct locap                               /* analog capacitance            */
  {
    struct locap  *NEXT       ;                    /* next capacitor                */
    struct locon  *TCON       ;                    /* top plate connector           */
    struct locon  *BCON       ;                    /* bottom plate connector        */
    char          *NAME       ;                    /* capacitor instance name       */
    double        CAPA        ;                    /* capacitor value (unite FF)    */
    char          TYPE        ;                    /* capacitor type                */
    struct ptype  *USER       ;                    /* application specific          */
  } locap_list                ;


/************************* Complementary capacitance information ********************/

/* The structure is put in the USER field of the capacitance */
/* with the LOCAP_INFO ptype                                 */

#define LOCAP_INFO 200208072                       /* USER field code               */

typedef struct s_locap_info
  {
    union
       {
         struct
          {
            double AT,AB      ;                    /* Top and bottom plate area     */
            double PT,PB      ;                    /* Top et bottom plate perimeter */
          } mim               ;
         struct
          {
            double L,W        ;                    /* POLY/NWELL capacitance        */
          } pnwell            ;
       } data                 ;
  } locap_info                ;


/*********************************** Resistor ***************************************/

#define RESMIM    0                                /* Metal-Inter-Metal type        */

#define IsResMIM(type)    ((type == RESMIM)    ? 1 : 0)


typedef struct lores                               /* analog resistor               */
  {
    struct lores  *NEXT       ;                    /* next resistor                 */
    struct locon  *RCON1      ;                    /* first connector               */
    struct locon  *RCON2      ;                    /* second connector              */
    char          *NAME       ;                    /* resistor instance name        */
    double        RESI        ;                    /* resistance value (ohms)       */
    char          TYPE        ;                    /* resistor type                 */
    struct ptype  *USER       ;                    /* application specific          */
} lores_list                  ;


/*************************** Complementary resistor information *********************/

/* The structure is put in the USER field of the resistor */
/* with the LORES_INFO ptype                              */

#define LORES_INFO 200208073                       /* USER field code               */

typedef struct s_lores_info
  {
    double L,W                ;
  } lores_info                ;


/************************************ Inductor **************************************/

#define SELFMIM    0                               /* Metal-Inter-Metal type        */

#define IsSelfMIM(type)    ((type == SELFMIM)    ? 1 : 0)

typedef struct loself                              /* analog inductor               */
  {
    struct loself *NEXT       ;                    /* next inductor                 */
    struct locon  *SCON1      ;                    /* first connector               */
    struct locon  *SCON2      ;                    /* second connector              */
    char          *NAME       ;                    /* inductor instance name        */
    double        SELF        ;                    /* inductance value (unite H)    */
    char          TYPE        ;                    /* inductor type                 */
    struct ptype  *USER       ;                    /* application specific          */
} loself_list                 ;



/************************************************************************************/
/************************************************************************************/
/************************************************************************************/


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

/*************************** Analogical world ***************************************/

extern locap_list *addlocap __P((lofig_list *ptfig,char type,double capa,losig_list *pttplate,
                                 losig_list *ptbplate,char *name))                                                       ;
extern int dellocap __P((lofig_list *ptfig,locap_list *ptcap))                                                           ;
extern locap_list *getlocap __P((lofig_list *ptfig,const char *name))                                                    ;
extern void dellocapuser __P((locap_list *))                                                                             ;

extern lores_list *addlores __P((lofig_list *ptfig,char type,double resi,losig_list *ptrcon1,
                                 losig_list *ptrcon2,char *name))                                                        ;
extern int dellores __P((lofig_list *ptfig,lores_list *ptres))                                                           ;
extern lores_list *getlores __P((lofig_list *ptfig,const char *name))                                                    ;
extern void delloresuser __P((lores_list *))                                                                             ;

extern loself_list *addloself __P((lofig_list *ptfig,char type,double self,losig_list *ptscon1,
                                   losig_list *ptscon2,char *name))                                                      ;
extern int delloself __P((lofig_list *ptfig,loself_list *ptself))                                                        ;
extern loself_list *getloself __P((lofig_list *ptfig,const char *name))                                                  ;
extern void delloselfuser __P((loself_list *))                                                                           ;

/************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !MLO */
