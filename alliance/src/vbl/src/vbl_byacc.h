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
/* file		: bvl_byacc.h						*/
/* date		: Jun 15 1992						*/
/* version	: v102							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: declaration of external functions and global variables*/
/*		  used by yacc						*/
/*								 	*/
/* ###--------------------------------------------------------------### */

static struct dct_entry *addent ();
static struct dct_recrd *addrcd ();
static struct dct_entry **initab ();
static void addtab ();
static int chktab ();
static void fretab ();
static void *vbl_addstr ();
static vbtyp_list *val_type();
static vbtyp_list *get_type();
static struct ptype *reversetype();
static struct choice_chain *addchoice();
static struct choice_chain *order_choice();

extern char          VBL_ERRFLG;        /* set to 1 in case of error    */
extern struct vbfig *VBL_HEADFIG;       /* head of vbfigs               */
extern long          VBL_LINNUM;
extern int           VBL_NUMPTP;
extern int           VBL_NUMCHOICE;

/*\
pNode         VBL_BDDPNT;
\*/
		/* BDD pointer			*/
char         *VBL_MODNAM;		/* curnennt model name		*/
char         *VBL_PROCNAM;              /* current process name         */
char         *VBL_FUNCNAM;              /* current function/procedure name */
long          VBL_PROCNUM;
long          VBL_LOOPNUM;
char         *VBL_COMPNAM;
char         *VBL_SAVCOMPNAM;
char         *VBL_GNRNAM;               /* current generate name        */
vbl_vexstr    VBL_SLCEXP;		/* structure filled with the	*/
vbl_vexstr    VBL_EMPSTR;		/* empty structure used with NOT*/
 
static int           VBL_NUMTYP = 0;	/* nombre de type			*/
static struct chain *VBL_NM1LST = NULL;	/* 1-st name liste		*/
static struct chain *VBL_NM2LST = NULL;	/* 2-st name liste		*/
static struct chain *VBL_INSLST = NULL;	/* 3-st name liste		*/
static struct chain *VBL_LOOPLBLLST = NULL;
static struct chain *VBL_GRDLST = NULL;	/* list of guard's ABL		*/
static struct chain *VBL_CNDLST = NULL;	/* list of conditions (ABL)	*/
static struct chain *VBL_LINLST = NULL; /* list of line                 */
static struct chain *VBL_VALLST = NULL;	/* list of waveforms (ABL)	*/
static struct vbfig *VBL_BEFPNT = NULL;	/* current BEFIG pointer	*/
static char         *VBL_LOOPLBL = NULL;
static struct vbmap *VBL_BEMAP  = NULL;

static struct vbins *VBL_BEINS  = NULL;
static struct vbgnr *VBL_BEGNR  = NULL;
static vbpcs_list   *VBL_BEPCS  = (vbpcs_list *)0;
static vbfun_list   *VBL_BEFUN  = (vbfun_list *)0;

static chain_list   *VBL_GNR_BEPCS = (chain_list *)0;
static chain_list   *VBL_GNR_BEINS = (chain_list *)0;
static chain_list   *VBL_GNR_BEGNR = (chain_list *)0;

static struct authtable *VBL_ENUMVAL = NULL;
static struct choice_chain  *VBL_CASPNT = NULL;
static int                   VBL_CASSIZE = 0;
static struct ptype *VBL_OTHPNT = NULL;
static struct ptype *VBL_PTYPE = NULL;

static struct dct_entry  *VBL_DCEHED;	/* free dct_entry's head	*/
static struct dct_recrd  *VBL_DCRHED;	/* free dct_recrd's head	*/

static struct dct_entry **hshtab;
 
extern struct vbgen      *bvl_addgen();

extern vbl_vexstr         vbl_crtvex();
extern vbl_vexstr         bvl_cpyvexstr();
extern vbl_vexstr         bvl_select();
extern char              *bvl_stostr();
extern void               bvl_error();

