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

#ident "$Id: rcn.h,v 1.1 2002/03/08 13:51:05 fred Exp $"

/*******************************************************************************
* rcn     : constantes, externs, and data-structures                           *
*                                                                              *
* version : 2.00                                                               *
* date    : 09/01/98                                                           *
*******************************************************************************/

/*******************************************************************************
* Gregoire AVOT                                                                *
* Laboratoire LIP6 - Departement ASIM                                          *
* Universite Paris 6 - Pierre et Marie Curie                                   *
* 4, Place Jussieu                                                             *
* Paris Cedex 05.                                                              *
*******************************************************************************/

#ifndef RCNH
#define RCNH

/* layer of wire */
#define RCN_WIRE_UNKNOW       ((unsigned char) 0x00000001)
#define RCN_WIRE_POLY         ((unsigned char) 0x00000002)
#define RCN_WIRE_ALU1         ((unsigned char) 0x00000003)
#define RCN_WIRE_ALU2         ((unsigned char) 0x00000004)
#define RCN_WIRE_CONT_POLY    ((unsigned char) 0x00000005)
#define RCN_WIRE_CONT_DIF_N   ((unsigned char) 0x00000006)
#define RCN_WIRE_CONT_DIF_P   ((unsigned char) 0x00000007)
#define RCN_WIRE_CONT_VIA     ((unsigned char) 0x00000008)
#define RCN_WIRE_CONT_VIA2    ((unsigned char) 0x00000009)
#define RCN_WIRE_CAPA         ((unsigned char) 0x00000010)
#define RCN_WIRE_RESI         ((unsigned char) 0x00000020)

/* Heap allocation */
#define RCNBUFSIZE 64

/* State of a losig */
#define MBK_RC_A ((int) 1)
#define MBK_RC_B ((int) 2)
#define MBK_RC_C ((int) 3)
#define MBK_RC_D ((int) 4)
#define MBK_RC_E ((int) 5)

/* node type for lonode ptype */
#define RCN_LOCON             ((long) 19981201)
#define RCN_REPORTED          ((long) 19981301)

/* flags for lonode */

#define RCN_FLAG_ONE     ((unsigned char) 0x00000001) /* Terminal node        */
#define RCN_FLAG_TWO     ((unsigned char) 0x00000002) /* two wires connected  */
#define RCN_FLAG_CROSS   ((unsigned char) 0x00000004) /* many wires connected */
#define RCN_FLAG_LOCON   ((unsigned char) 0x00000008) /* the node is a locon  */
#define RCN_FLAG_PASS    ((unsigned char) 0x00000010) /* yet treated          */

#define RCN_FLAG_TRUE    ((unsigned char) 0x0000000F) /* true node            */

/* flags for lowire */

#define RCN_FLAG_PASS    ((unsigned char) 0x00000010) /* yet treated          */
#define RCN_FLAG_DEL     ((unsigned char) 0x00000020) /* not a valid wire     */

/* macro to use with flags */

#define RCN_SETFLAG(m,b)        (m=(m)|(b))
#define RCN_CLEARFLAG(m,b)      (m=(m)&(~b))
#define RCN_GETFLAG(m,b)        ((m)&(b))

/* Macro to get the node table */
#define RCN_LONODE              15543
#define RCN_SIZETAB		15544

/* Ptype in lofig for order of physical order of locon on figure */
#define PH_INTERF 19980318

/* Ptype in locon for name of physical connector (spice parser) */
#define PNODENAME 19981103

/* Ptype in lonode for retreive the component connexe */
#define CONNEXE 19990808
/*******************************************************************************
* interconnect network  structures                                             *
*******************************************************************************/

typedef struct lorcnet                        /* logical rc network           */
{
float              CAPA;                      /* total capacitance            */
struct lowire     *PWIRE;                     /* wire list                    */
long               NBNODE;                    /* number of lonode             */
chain_list        *PCTC;                      /* cross talk capacitance       */
struct ptype      *USER;                      /* Application specific         */
}
lorcnet_list;

typedef struct lowire                         /* logical wire                 */
{
struct lowire           *NEXT;          /* next wire                          */
long                     NODE1;         /* interconnect network node1         */
long                     NODE2;         /* interconnect network node2         */
unsigned char            LAYER;         /* physical layer                     */
unsigned char            FLAG;          /* flags                              */
float                    RESI;          /* wire resistance                    */
float                    CAPA;          /* wire capacitance                   */
long                     X;             /* x position                         */
long                     Y;             /* y position                         */
long                     DX;            /* x rectangle wide                   */
long                     DY;            /* y rectangle length                 */
struct ptype            *USER;          /* Application specific               */
}
lowire_list;

typedef struct lonode                   /* logical node                       */
{
struct chain            *WIRELIST;      /* wire list                          */
struct chain            *CTCLIST;       /* cross talk capacitance list        */
long                     FLAG;          /* Flags                              */
long                     INDEX;         /* node index                         */
struct ptype            *USER;          /* wire list                          */
}
lonode_list;

typedef struct loctc                           /* cross talk capacitance      */
{
struct losig  *SIG1;                           /* First losig                 */
struct losig  *SIG2;                           /* Second losig                */
long           NODE1;                          /* first lonode                */
long           NODE2;                          /* first lonode                */
float          CAPA;                           /* cross talk capacitance      */
struct ptype  *USER;                           /* application specific        */
} loctc_list;

/*******************************************************************************
* externals for rcn view                                                       *
*******************************************************************************/

extern int               rclevel __P((losig_list*));
extern chain_list  *gettabnode __P((lorcnet_list*));
extern lorcnet_list     *addlorcnet __P((losig_list *ptsig));
extern lorcnet_list     *givelorcnet __P((losig_list *ptsig));
extern int               freelorcnet __P((losig_list *ptsig));
                        /* Il faut qu'il y ait un lofigchain */

extern float             addcapa   __P((losig_list *ptsig,float value));
extern float             setcapa   __P((losig_list *ptsig,float value));
extern float             getcapa   __P((losig_list *ptsig));
extern float             rcncalccapa __P((losig_list *ptsig));
extern long              addonode  __P((losig_list *ptsig,locon_list *ptlocon));
extern long              addlonode __P((losig_list *ptsig,locon_list *ptlocon));
extern lonode_list      *getlonode __P((losig_list *ptsig, long index));
extern lowire_list      *addlowire __P((losig_list *ptsig, unsigned char layer,
                                       unsigned char flag, float resi,
                                       float capa, long x, long y, long dx,
                                       long dy, long n1, long n2 ));
extern int               dellowire __P((losig_list *ptsig, long node1,
                                       long node2 ));
extern lowire_list      *getlowire __P((losig_list *ptsig, long node1,
                                       long node2 ));
extern loctc_list       *addloctc __P((losig_list *ptsig1, long node1,
                                       losig_list *ptsig2, long node2,
                                       float value ));
extern int               delloctc __P(( losig_list *ptsig1, long node1,
                                        losig_list *ptsig2, long node2 ));
extern loctc_list       *getloctc __P(( losig_list *ptsig1, long node1,
                                        losig_list *ptsig2, long node2 ));
extern chain_list       *getallctc __P(( lofig_list *ptfig ));
extern void              freectclist __P(( lofig_list*, chain_list* ));
extern void              freectcsig  __P(( chain_list* ));
extern void              setloconnode __P((locon_list *ptlocon, long node));
extern void              delloconnode __P((locon_list *ptcon, long index));
extern chain_list       *getloconnode __P((lonode_list *node));
extern void              delrcnlocon __P((locon_list *ptloc));
extern void              addcapawire __P((lowire_list *ptwire,float capa));
extern void              setcapawire __P((lowire_list *ptwire,float capa));
extern void              addresiwire __P((lowire_list *ptwire,float resi));
extern void              setresiwire __P((lowire_list *ptwire,float resi));
extern void              addpararesiwire __P((lowire_list *ptwire, float resi));

extern void              buildtable __P((losig_list *ptsig));
extern void              freetable __P((losig_list *ptsig));
extern int               chkloop __P((losig_list *ptsig));
/* 0 : no loop, 1 no node table, 2 : loop */
extern lonode_list      *givelonode __P((losig_list *ptsig, long index));

extern void              chkrcn __P((losig_list *ptsig));
void		         clearallwireflag __P(( lorcnet_list *ptrcnet,
		                                unsigned char flag ));
extern void              statrcn __P((void));
extern void              setdellowire __P(( losig_list*, lowire_list* ));
extern void              mergedellowire __P(( losig_list* ));

/*******************************************************************************
* Include for rcn_lo_util.c                                                    *
*******************************************************************************/

extern void             duplorcnet __P((losig_list*, losig_list*));
extern int              breakloop  __P(( losig_list* ));
extern int              reduce_rcn __P(( losig_list* ));
extern int              connexe    __P(( losig_list* ));
extern int              cleanrcnet __P(( losig_list* ));
#endif /* !RCNH */
