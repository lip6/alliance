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
/* 									*/
/* file		: beh_addbeaux.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "abe.h"

/* ###--------------------------------------------------------------### */
/* function	: beh_addbeaux						*/
/* description	: create a BEAUX structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct beaux *beh_addbeaux (lastbeaux, name, abl_expr, bdd_expr)

struct beaux *lastbeaux;	/* pointer on the last beaux structure	*/
char         *name;		/* signal's name			*/
struct chain *abl_expr;		/* signal's expression (ABL)		*/
struct bddnode *bdd_expr;	/* signal's expression (BDD)		*/

  {
  struct beaux *ptaux;

  name        = namealloc (name);

  ptaux       = (struct beaux *) autallocblock (sizeof(struct beaux));
  ptaux->NAME = name;
  ptaux->ABL  = abl_expr;
  ptaux->NODE = bdd_expr;
  ptaux->NEXT = lastbeaux;

  return (ptaux);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbebus.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbebus						*/
/* description	: create a BEBUS structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct bebus *beh_addbebus (lastbebus, name, biabl, binode, type)

struct bebus  *lastbebus;	/* pointer on the last bebus structure	*/
char          *name;		/* port's name				*/
struct biabl  *biabl;		/* port's drivers (ABL)			*/
struct binode *binode;		/* port's drivers (BDD)			*/
char           type;		/* port type mark (M or W)		*/

  {
  struct bebus *ptbus;

  name          = namealloc (name);

  ptbus         = (struct bebus *) autallocblock (sizeof(struct bebus));
  ptbus->NAME   = name;
  ptbus->BIABL  = biabl;
  ptbus->BINODE = binode;
  ptbus->TYPE   = type;
  ptbus->NEXT   = lastbebus;

  return (ptbus);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbebux.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbebux						*/
/* description	: create a BEBUX structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct bebux *beh_addbebux (lastbebux, name, biabl, binode, type)

struct bebux  *lastbebux;	/* pointer on the last bebux structure	*/
char          *name;		/* signal's name			*/
struct biabl  *biabl;		/* signal's expression (ABL)		*/
struct binode *binode;		/* signal's expression (BDD)		*/
char           type;		/* signal's type mark (M or W)		*/

  {
  struct bebux *ptbux;

  name          = namealloc (name);

  ptbux         = (struct bebux *) autallocblock (sizeof(struct bebux));
  ptbux->NAME   = name;
  ptbux->BIABL  = biabl;
  ptbux->BINODE = binode;
  ptbux->TYPE   = type;
  ptbux->NEXT   = lastbebux;

  return (ptbux);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbefig.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbefig						*/
/* description	: create an empty BEFIG structure at the top of the list*/
/*		  The ABE_HEDFIG list is not modified.			*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct befig *beh_addbefig (lastbefig, name)

struct befig *lastbefig;	/* pointer on the last befig structure	*/
char         *name;		/* figure's name			*/

  {
  struct befig *ptfig;

  name          = namealloc (name);

  ptfig         = (struct befig *) autallocblock (sizeof(struct befig));
  ptfig->NAME   = name;
  ptfig->BEREG  = NULL;
  ptfig->BEMSG  = NULL;
  ptfig->BERIN  = NULL;
  ptfig->BEOUT  = NULL;
  ptfig->BEBUS  = NULL;
  ptfig->BEAUX  = NULL;
  ptfig->BEDLY  = NULL;
  ptfig->BEBUX  = NULL;
  ptfig->BEPOR  = NULL;
  ptfig->CIRCUI = NULL;
  ptfig->USER   = NULL;
  ptfig->BEGEN  = NULL;
  ptfig->BEPGM  = NULL;
  ptfig->ERRFLG = 0;
  ptfig->TYPE   = 0;
  ptfig->NEXT   = lastbefig;

  return (ptfig);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbegen.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbegen						*/
/* description	: create a BEGEN structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct begen *beh_addbegen (lastbegen, name, type, value)

struct begen *lastbegen;	/* pointer on the last begen structure	*/
char         *name;		/* generic's name			*/
char         *type;		/* generic's type mark			*/
void         *value;		/* generic's value			*/

  {

  struct begen *ptgen;

  name         = namealloc (name);
  type         = namealloc (type);
  
  ptgen        = (struct begen *) autallocblock (sizeof(struct begen));
  ptgen->NAME  = name;
  ptgen->TYPE  = type;
  ptgen->VALUE = value;
  ptgen->NEXT  = lastbegen;

  return (ptgen);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbemsg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbemsg						*/
/* description	: create BEMSG structure at the top the list		*/
/* called func.	: autallocblock, namealloc					*/
/* ###--------------------------------------------------------------### */

struct bemsg *beh_addbemsg (lastbemsg,label,level,message,abl_expr,bdd_expr)

struct bemsg *lastbemsg;	/* pointer on the last bemsg structure	*/
char         *label;		/* assert's label			*/
char          level;		/* severity level (E or W)		*/
char         *message;		/* reported message			*/
struct chain *abl_expr;		/* assert's condition (ABL)		*/
struct bddnode *bdd_expr;	/* assert's condition (BDD)		*/

  {

  struct bemsg *ptmsg;

  label          = namealloc (label);

  ptmsg          = (struct bemsg *) autallocblock (sizeof(struct bemsg));
  ptmsg->LABEL   = label;
  ptmsg->LEVEL   = level;
  ptmsg->MESSAGE = message;
  ptmsg->ABL     = abl_expr;
  ptmsg->NODE    = bdd_expr;
  ptmsg->NEXT    = lastbemsg;

  return (ptmsg);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbeout.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbeout						*/
/* description	: create a BEOUT structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct beout *beh_addbeout (lastbeout, name, abl_expr, bdd_expr)

struct beout *lastbeout;	/* pointer on the last beout structure	*/
char         *name;		/* port's name				*/
struct chain *abl_expr;		/* port's expression (ABL)		*/
struct bddnode  *bdd_expr;	/* port's expression (BDD)		*/

  {
  struct beout *ptout;

  name         = namealloc (name);

  ptout        = (struct beout *) autallocblock (sizeof(struct beout));
  ptout->NAME  = name;
  ptout->ABL   = abl_expr;
  ptout->NODE  = bdd_expr;
  ptout->NEXT  = lastbeout;

  return (ptout);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbepgm.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function     : beh_addbepgm                              */
/* description     : add an BEPGM structure at the top of a list          */
/* called func.     : namealloc, autallocblock                         */
/* ###--------------------------------------------------------------### */

struct bepgm *beh_addbepgm (lastbepgm, type , name , value)
struct bepgm  *lastbepgm;     /* pointer on the last bepgm          */
char          *type;          /* pragma's type                         */
char          *name;          /* pragma's name               */
void          *value;          /* pragma's content                              */
  {
  struct bepgm *ptbepgm;

  if(type)
       type            = namealloc (type);
  if(name)
       name            = namealloc (name);

  ptbepgm         = (struct bepgm *) autallocblock (sizeof(struct bepgm));

  ptbepgm->TYPE   = type;
  ptbepgm->NAME   = name;
  ptbepgm->VALUE  = value;

  ptbepgm->NEXT   = lastbepgm;

  return (ptbepgm);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbepor.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbepor						*/
/* description	: create a BEPOR structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct bepor *beh_addbepor (lastbepor, name, dir, type)

struct bepor *lastbepor;	/* pointer on the last bepor structure	*/
char         *name;		/* signal's name			*/
char          dir;     		/* signal's mode (I, O, B, Z, or T)	*/
char          type;     	/* signal'type mark (M or W)		*/

  {
  struct bepor *ptbepor;

  name               = namealloc (name);

  ptbepor            = (struct bepor *) autallocblock (sizeof(struct bepor));
  ptbepor->NAME      = name;
  ptbepor->DIRECTION = dir;
  ptbepor->TYPE      = type;
  ptbepor->NEXT      = lastbepor;

  return (ptbepor);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbereg.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbereg						*/
/* description	: create a BEREG structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct bereg *beh_addbereg (lastbereg, name, biabl, binode)

struct bereg  *lastbereg;	/* pointer on the last bereg structure	*/
char          *name;		/* register's name			*/
struct biabl  *biabl;		/* register's drivers (ABL)		*/
struct binode *binode;		/* register's drivers (BDD)		*/

  {
  struct bereg *ptreg;

  name          = namealloc (name);

  ptreg         = (struct bereg *) autallocblock (sizeof(struct bereg));
  ptreg->NAME   = name;
  ptreg->BIABL  = biabl;
  ptreg->BINODE = binode;
  ptreg->NEXT   = lastbereg;

  return (ptreg);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addberin.c					*/
/* date		: Sep 20 1994						*/
/* version	: v107							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addberin						*/
/* description	: create a BERIN structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct berin *beh_addberin (lastberin, name)

struct berin *lastberin;	/* pointer on the last berin structure	*/
char         *name;		/* signal's name			*/

  {
  struct berin *ptrin;

  name         = namealloc (name);

  ptrin        = (struct berin *) autallocblock (sizeof(struct berin));
  ptrin->NAME  = name;
  ptrin->NEXT  = lastberin;

  ptrin->REG_REF = NULL;
  ptrin->MSG_REF = NULL;
  ptrin->AUX_REF = NULL;
  ptrin->BUX_REF = NULL;
  ptrin->BUS_REF = NULL;
  ptrin->OUT_REF = NULL;
  ptrin->DLY_REF = NULL;

  return (ptrin);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbiabl.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbiabl						*/
/* description	: create a BIABL structure at the top the list		*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct biabl *beh_addbiabl (lastbiabl, label, condition, value)

char         *label;		/* block's label			*/
struct biabl *lastbiabl;	/* pointer on the last biabl structure	*/
struct chain *condition;	/* guard expression (ABL)		*/
struct chain *value;		/* value expression (ABL)		*/

  {
  struct biabl  *ptbiabl;

  ptbiabl         = (struct biabl *) autallocblock (sizeof(struct biabl));
  label           = namealloc (label);

  ptbiabl->LABEL  = label;
  ptbiabl->CNDABL = condition;
  ptbiabl->VALABL = value;
  ptbiabl->USER   = NULL;
  ptbiabl->NEXT   = lastbiabl;

  return (ptbiabl);
  }

/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: beh_addbinod.c					*/
/* date		: Sep  3 1993						*/
/* version	: v106							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: low-level function					*/
/*									*/
/* ###--------------------------------------------------------------### */

/* ###--------------------------------------------------------------### */
/* function	: beh_addbinode						*/
/* description	: create a BINODE structure at the top the list		*/
/* called func.	: autallocblock						*/
/* ###--------------------------------------------------------------### */

struct binode *beh_addbinode (lastbinode, condition, value)

struct binode  *lastbinode;	/* pointer on the last binode structure	*/
struct bddnode *condition;	/* guard expression (BDD)		*/
struct bddnode *value;		/* value expression (BDD)		*/

  {
  struct binode *ptbinode;

  ptbinode          = (struct binode *) autallocblock (sizeof(struct binode));
  ptbinode->CNDNODE = condition;
  ptbinode->VALNODE = value;
  ptbinode->NEXT    = lastbinode;

  return (ptbinode);
  }
