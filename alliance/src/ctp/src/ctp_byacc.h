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
/* file		: ctp_byacc.h						*/
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
static void *ctp_addstr ();
static ctltype_list *val_type();
static ctltype_list *get_type();
static struct ptype *reversetype();

extern char          CTP_ERRFLG;        /* set to 1 in case of error    */
extern ctlfig_list  *CTP_HEADFIG;       /* head of vbfigs               */
extern long          CTP_LINNUM;
extern int           CTP_NUMPTP;
extern char         *CTP_MODNAM;

/*\
pNode         CTP_BDDPNT;
\*/
		/* BDD pointer			*/
ctp_vexstr    CTP_SLCEXP;		/* structure filled with the	*/
ctp_vexstr    CTP_EMPSTR;		/* empty structure used with NOT*/
 
static int           CTP_NUMTYP = 0;	/* nombre de type			*/
static struct chain *CTP_NM1LST = NULL;	/* 1-st name liste		*/
static struct chain *CTP_NM2LST = NULL;	/* 2-st name liste		*/
static struct chain *CTP_INSLST = NULL;	/* 3-st name liste		*/
static struct chain *CTP_LINLST = NULL; /* list of line                 */
static struct chain *CTP_VALLST = NULL;	/* list of waveforms (ABL)	*/

static struct ptype *CTP_PTYPE = NULL;

static struct dct_entry  *CTP_DCEHED;	/* free dct_entry's head	*/
static struct dct_recrd  *CTP_DCRHED;	/* free dct_recrd's head	*/

static struct dct_entry **hshtab;
 
extern ctp_vexstr         ctp_crtvex();
extern ctp_vexstr         ctp_cpyvexstr();
extern ctp_vexstr         ctp_select();
extern char              *ctp_stostr();
extern void               ctp_error();

