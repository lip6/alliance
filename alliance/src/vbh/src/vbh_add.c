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
| 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                     |
|                                                             |
\------------------------------------------------------------*/
/* ###--------------------------------------------------------------### */
/* 									*/
/* file		: vbh_addvbasg.c					*/
/* date		: Aug 28 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to create a		*/
/*		  behaviour data structure				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "vex.h" 
#include "vbh.h" 

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbmod						*/
/* description	: create a VBMOD structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbmod *vbh_addvbmod (lastvbmod, name, line)

struct vbmod *lastvbmod;
char         *name;
long          line;
{
  struct vbmod *ptmod;

  ptmod       = (struct vbmod *) autallocheap (sizeof(struct vbmod));
  ptmod->NAME = namealloc( name );;
  ptmod->LINE = line;
  ptmod->NEXT = lastvbmod;

  return (ptmod);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbins						*/
/* description	: create a VBINS structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbins *vbh_addvbins (lastvbins, name, model, line)

struct vbins *lastvbins;
char         *name;
char         *model;
long          line;
{
  struct vbins *ptins;

  ptins        = (struct vbins *) autallocheap (sizeof(struct vbins));
  ptins->NAME  = namealloc( name );;
  ptins->MODEL = namealloc( model );;
  ptins->LINE  = line;
  ptins->NEXT  = lastvbins;

  return (ptins);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbgnr						*/
/* description	: create a VBGNR structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbgnr *vbh_addvbgnr (lastvbgnr, label, line)

struct vbgnr *lastvbgnr;
char         *label;
long          line;
{
  struct vbgnr *ptgnr;

  ptgnr        = (struct vbgnr *) autallocheap (sizeof(struct vbgnr));
  ptgnr->LABEL = namealloc( label );;
  ptgnr->LINE  = line;
  ptgnr->NEXT  = lastvbgnr;

  return (ptgnr);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbmap						*/
/* description	: create a VBMAP structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbmap *vbh_addvbmap (lastvbmap, formal, actual, line)

struct vbmap *lastvbmap;
vexexpr      *formal;
vexexpr      *actual;
long          line;
{
  struct vbmap *ptmap;

  ptmap         = (struct vbmap *) autallocheap (sizeof(struct vbmap));
  ptmap->FORMAL = formal;
  ptmap->ACTUAL = actual;
  ptmap->LINE   = line;
  ptmap->NEXT   = lastvbmap;

  return (ptmap);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbagr						*/
/* description	: create a VBAGR structure at the top the list		*/
/* called func.	: addptype, autallocheap, namealloc			*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbagr (lastptype, assign_lst, line)

struct ptype  *lastptype;	/* pointer on the last ptype structure	*/
struct ptype  *assign_lst;
long           line;

{
  struct ptype *ptptype;
  struct vbagr *ptvbagr;

  ptvbagr           = (struct vbagr *) autallocheap (sizeof(struct vbagr));
  ptvbagr->ASSIGN   = assign_lst;
  ptvbagr->LINE     = line;

  ptptype           = addptype (lastptype, VBH_BEAGR, (void *)ptvbagr);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbasg						*/
/* description	: create a VBASG structure at the top the list		*/
/* called func.	: addptype, autallocheap, namealloc			*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbasg (lastptype, target, vex, type, line)

struct ptype  *lastptype;	/* pointer on the last ptype structure	*/
vexexpr       *target;		/* signal's name			*/
vexexpr       *vex;		/* condition's expression		*/
vbtyp_list    *type;		/* signal's type (cannot be 0)		*/
long           line;

{
  struct ptype *ptptype;
  struct vbasg *ptvbasg;

  ptvbasg           = (struct vbasg *) autallocheap (sizeof(struct vbasg));
  ptvbasg->TARGET   = target;
  ptvbasg->VEX      = vex;
  ptvbasg->TYPE     = type;
  ptvbasg->LINE     = line;

  ptptype           = addptype (lastptype, VBH_BEASG, (void *)ptvbasg);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbcst						*/
/* description	: create a VBCST structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbcst *vbh_addvbcst (lastvbcst, target, vex_expr, type,line)

struct vbcst *lastvbcst;	/* pointer on the last vbaux structure	*/
vexexpr      *target;		/* constant name			*/
vexexpr      *vex_expr;		/* constant expression (ABL)		*/
vbtyp_list    *type;		/* constant type 			*/
long           line;

{
  struct vbcst *ptcst;

  ptcst       = (struct vbcst *) autallocheap (sizeof(struct vbcst));
  ptcst->TARGET = target;
  ptcst->VEX  = vex_expr;
  ptcst->TYPE = type;
  ptcst->LINE = line;
  ptcst->NEXT = lastvbcst;

  return (ptcst);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbgen						*/
/* description	: create a VBGEN structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbgen *vbh_addvbgen (lastvbgen, target, vex_expr, type,line)

struct vbgen *lastvbgen;	/* pointer on the last vbaux structure	*/
vexexpr      *target;		/* generic name			*/
vexexpr      *vex_expr;		/* generic expression (ABL)		*/
vbtyp_list    *type;		/* generic type 			*/
long           line;

{
  struct vbgen *ptgen;

  ptgen       = (struct vbgen *) autallocheap (sizeof(struct vbgen));
  ptgen->TARGET = target;
  ptgen->VEX  = vex_expr;
  ptgen->TYPE = type;
  ptgen->LINE = line;
  ptgen->NEXT = lastvbgen;

  return (ptgen);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbatr						*/
/* description	: create a VBATR structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbatr *vbh_addvbatr (lastvbatr, name, entity, type, value, line )

struct vbatr *lastvbatr;	/* pointer on the last vbatr structure	*/
char         *name;
char         *entity;
int           type;
char         *value;
long          line;
{
  struct vbatr *ptatr;

  ptatr         = (struct vbatr *) autallocheap (sizeof(struct vbatr));
  ptatr->NAME   = name;
  ptatr->ENTITY = entity;
  ptatr->TYPE   = type;
  ptatr->VALUE  = value;
  ptatr->LINE   = line;
  ptatr->NEXT   = lastvbatr;

  return (ptatr);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbaux						*/
/* description	: create a VBAUX structure at the top the list		*/
/* called func.	: namealloc, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct vbaux *vbh_addvbaux (lastvbaux, target, vex_expr, kind, type,line)

struct vbaux *lastvbaux;	/* pointer on the last vbaux structure	*/
vexexpr      *target;		/* signal's name			*/
vexexpr      *vex_expr;		/* signal's expression (ABL)		*/
unsigned char  kind;   		/* signal's kind BUS                    */
vbtyp_list    *type;		/* signal's type 			*/
long           line;

{
  struct vbaux *ptaux;

  ptaux       = (struct vbaux *) autallocheap (sizeof(struct vbaux));
  ptaux->TARGET = target;
  ptaux->VEX  = vex_expr;
  ptaux->KIND = kind;
  ptaux->TYPE = type;
  ptaux->LINE = line;
  ptaux->NEXT = lastvbaux;

  return (ptaux);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbcas						*/
/* description	: create a VBCAS structure at the top the list		*/
/* called func.	: addptype, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbcas (lastptype, Vex,type,line)

struct ptype *lastptype;	/* pointer on the last ptype structure	*/
vexexpr      *Vex;		/* condition's expression		*/
vbtyp_list   *type;		/*  expression's type i			*/ 
long          line;
{
  struct ptype *ptptype;
  struct vbcas *ptvbcas;

  ptvbcas        = (struct vbcas *) autallocheap (sizeof(struct vbcas));
  ptvbcas->VEX   = Vex;
  ptvbcas->TYPE  = type;
  ptvbcas->LINE  = line;

  ptptype        = addptype (lastptype, VBH_BECAS, (void *)ptvbcas);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbext						*/
/* description	: create a VBEXT (sequential exit statement)		*/
/* called func.	: autallocheap						*/
/* ###--------------------------------------------------------------### */

struct vbext *vbh_addvbext (loop_label, cnd,line)

  char    *loop_label;     /* loop label name                      */
  vexexpr *cnd;            /* condition clause                     */
  long     line;
{
  struct vbext *ptext;

  ptext          = (struct vbext *) autallocheap (sizeof(struct vbext));

  ptext->LABEL = loop_label;
  ptext->CND   = cnd;
  ptext->LINE  = line;

  return (ptext);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbfig						*/
/* description	: create an empty BEFIG structure at the top of the list*/
/*		  The VBH_HEDFIG list is not modified.			*/
/* called func.	: namealloc, autallocblock					*/
/* ###--------------------------------------------------------------### */

struct vbfig *vbh_addvbfig (lastvbfig, name)

struct vbfig *lastvbfig;	/* pointer on the last vbfig structure	*/
char         *name;		/* figure's name			*/

{
  struct vbfig *ptfig;

  name          = namealloc (name);

  ptfig         = (struct vbfig *) autallocblock (sizeof(struct vbfig));
  ptfig->NAME   = name;
  ptfig->HASH_TYPE = createauthtable( 50 );
  ptfig->NEXT   = lastvbfig;

  vbh_prdeftyp( ptfig );

  return (ptfig);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbifs						*/
/* description	: create a VBIFS structure at the top the list		*/
/* called func.	: addptype, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbifs (lastptype, vex,line)

struct ptype *lastptype;	/* pointer on the last ptype structure	*/
vexexpr      *vex;		/* condition's expression		*/
long          line;
{
  struct ptype *ptptype;
  struct vbifs *ptvbifs;

  ptvbifs           = (struct vbifs *) autallocheap (sizeof(struct vbifs));
  ptvbifs->CND      = vex;
  ptvbifs->LINE     = line;
  ptptype           = addptype (lastptype, VBH_BEIFS, (void *)ptvbifs);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbfor						*/
/* description	: create a VBLOP structure at the top the list		*/
/* called func.	: addptype, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbfor (lastptype,name,variable,left,right,up,line)

struct ptype *lastptype;	/* pointer on the last object		*/
char         *name;		/* name of loop          		*/
vexexpr      *variable;		/* name of variable          		*/
vexexpr      *left;		/* low  of for               		*/
vexexpr      *right;		/* up   of for               		*/
unsigned char up;
long          line;
{

  struct ptype *ptptype;
  struct vbfor *ptvblop;

  ptvblop           = (struct vbfor *) autallocheap (sizeof(struct vbfor));
  ptvblop->LABEL    = namealloc(name);
  ptvblop->VARIABLE = variable;
  ptvblop->LEFT     = left;
  ptvblop->RIGHT    = right;
  ptvblop->UP       = up;
  ptvblop->LINE     = line;
  
  ptptype	    =addptype(lastptype,VBH_BEFOR,(void *)ptvblop);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvblop						*/
/* description	: create a VBLOP structure at the top the list		*/
/* called func.	: addptype, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvblop (lastptype,name,line)

struct ptype *lastptype;	/* pointer on the last object		*/
char         *name;		/* name of loop          		*/
long          line;
{

  struct ptype *ptptype;
  struct vblop *ptvblop;

  ptvblop           = (struct vblop *) autallocheap (sizeof(struct vblop));
  ptvblop->LABEL    = namealloc(name);
  ptvblop->LINE     = line;
  
  ptptype	    =addptype(lastptype,VBH_BELOP,(void *)ptvblop);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbwhi						*/
/* description	: create a VBLOP structure at the top the list		*/
/* called func.	: addptype, autallocheap				*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbwhi (lastptype,name,cnd,line)

struct ptype *lastptype;	/* pointer on the last object		*/
char         *name;		/* name of loop          		*/
vexexpr      *cnd;
long          line;
{

  struct ptype *ptptype;
  struct vbwhi *ptvblop;

  ptvblop           = (struct vbwhi *) autallocheap (sizeof(struct vbwhi));
  ptvblop->LABEL    = namealloc(name);
  ptvblop->CND      = cnd;
  ptvblop->LINE     = line;
  
  ptptype	    =addptype(lastptype,VBH_BEWHI,(void *)ptvblop);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbret						*/
/* description	: create a VBRET (sequential return statement)		*/
/* called func.	: autallocheap						*/
/* ###--------------------------------------------------------------### */

struct vbret *vbh_addvbret (ret_value,line)

vexexpr      *ret_value;            /* return value */
long          line;
{
  struct vbret *ptret;

  ptret = (struct vbret *) autallocheap (sizeof(struct vbret));

  ptret->RET  = ret_value;
  ptret->LINE = line;

  return (ptret);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbcal						*/
/* description	: create a VBRET (sequential call statement)		*/
/* called func.	: autallocheap						*/
/* ###--------------------------------------------------------------### */

struct vbcal *vbh_addvbcal (cal_vex,line)

vexexpr      *cal_vex;            /* call */
long          line;
{
  struct vbcal *ptcal;

  ptcal = (struct vbcal *) autallocheap (sizeof(struct vbcal));

  ptcal->CALL = cal_vex;
  ptcal->LINE = line;

  return (ptcal);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbnxt						*/
/* description	: create a VBNXT (sequential next statement)		*/
/* called func.	: autallocheap						*/
/* ###--------------------------------------------------------------### */

struct vbnxt *vbh_addvbnxt (loop_label, cnd,line)

       char  *loop_label;     /* loop label name                      */
vexexpr      *cnd;            /* condition clause                     */
long          line;
{
  struct vbnxt *ptnxt;

  ptnxt          = (struct vbnxt *) autallocheap (sizeof(struct vbnxt));

  ptnxt->LABEL = loop_label;
  ptnxt->CND   = cnd;
  ptnxt->LINE  = line;

  return (ptnxt);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbpcs						*/
/* description	: create a VBPCS structure at the top the list		*/
/* called func.	: namealloc, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct vbpcs *vbh_addvbpcs (lastvbpcs, label, sens, inst, line )

struct vbpcs *lastvbpcs;	/* pointer on the last vbpcs structure	*/
char         *label;		/* process's label			*/
struct chain *sens;		/* sensitivity list			*/
struct ptype *inst;		/* list of instructions			*/
long          line;
{
  struct vbpcs *ptvbpcs;

  label                = namealloc (label);

  ptvbpcs              = (struct vbpcs *) autallocheap (sizeof(struct vbpcs));
  ptvbpcs->LABEL       = label;
  ptvbpcs->SENSITIVITY = sens;
  ptvbpcs->INSTRUCTION = inst;
  ptvbpcs->LINE        = line;

  ptvbpcs->NEXT        = lastvbpcs;

  return (ptvbpcs);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbfun						*/
/* description	: create a VBFUN structure at the top the list		*/
/* called func.	: namealloc, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct vbfun *vbh_addvbfun (lastvbfun, name, ret_param, lst_param, inst, line )

struct vbfun *lastvbfun;	/* pointer on the last vbfun structure	*/
char         *name;
struct vbarg *ret_param;
struct vbarg *lst_param;
struct ptype *inst;
long          line;
{
  struct vbfun *ptvbfun;

  name                 = namealloc(name);

  ptvbfun              = (struct vbfun *) autallocheap (sizeof(struct vbfun));
  ptvbfun->NAME        = name;
  ptvbfun->RETURN      = ret_param;
  ptvbfun->ARGUMENT    = lst_param;
  ptvbfun->INSTRUCTION = inst;
  ptvbfun->LINE        = line;

  ptvbfun->NEXT        = lastvbfun;

  return(ptvbfun);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbarg						*/
/* description	: create a VBPAR structure at the top the list		*/
/* called func.	: namealloc, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct vbarg *vbh_addvbarg (lastvbarg, target, dir, class, type,line)

struct vbarg  *lastvbarg;	/* pointer on the last vbarg structure	*/
vexexpr       *target;		/* signal's name			*/
char           dir;    		/* signal's mode (I, O, B, Z, or T)	*/
char           class;    	/* signal's class (C, V, S)		*/
vbtyp_list    *type;     	/* signal's type          	        */
long           line;
{
  struct vbarg *ptvbarg;

  ptvbarg            = (struct vbarg *) autallocheap (sizeof(struct vbarg));
  ptvbarg->TARGET    = target;
  ptvbarg->DIR       = dir;
  ptvbarg->CLASS     = class;
  ptvbarg->TYPE      = type;
  ptvbarg->LINE      = line;
  ptvbarg->NEXT      = lastvbarg;

  return (ptvbarg);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbpor						*/
/* description	: create a VBPOR structure at the top the list		*/
/* called func.	: namealloc, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct vbpor *vbh_addvbpor (lastvbpor, target, vex, dir, kind, type,line)

struct vbpor  *lastvbpor;	/* pointer on the last vbpor structure	*/
vexexpr       *target;		/* signal's name			*/
vexexpr       *vex;		/* signal's init	 		*/
unsigned char  dir;    		/* signal's dir (I, O, B, Z, or T)	*/
unsigned char  kind;   		/* signal's kind BUS                    */
vbtyp_list   *type;     	/* signal's type          	        */
long          line;

{
  struct vbpor *ptvbpor;

  ptvbpor            = (struct vbpor *) autallocheap (sizeof(struct vbpor));
  ptvbpor->TARGET    = target;
  ptvbpor->VEX       = vex;
  ptvbpor->DIR       = dir;
  ptvbpor->KIND      = kind;
  ptvbpor->TYPE      = type;
  ptvbpor->LINE      = line;
  ptvbpor->NEXT      = lastvbpor;

  return (ptvbpor);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbtyp						*/
/* description	: add an BETYP structure at the top of a list		*/
/* called func.	: namealloc, autallocheap					*/
/* ###--------------------------------------------------------------### */

struct vbtyp *vbh_addvbtyp ( Figure, name , left , right , value ,
                             size     , byte , base , typeid, resolv, class,line)

vbfig_list    *Figure;
char          *name;		/* type's name				*/
long           left;		/* left bound				*/
long           right;		/* right bound				*/
char         **value;		/* list of values			*/
unsigned int   size;		/* number of values			*/
unsigned int   byte;		/* number of bytes needed to code values*/
vbtyp_list    *base;		/* the base type or element's type	*/
unsigned char  typeid;		/* the type of the index for an array	*/
vbfun_list    *resolv;		/* index of resolution function		*/
char           class;		/* type's class (E, I, A, S)		*/
long           line;
{
  struct vbtyp *ptvbtyp;

  name            = namealloc (name);
  ptvbtyp         = (struct vbtyp *) autallocheap (sizeof(struct vbtyp));

  ptvbtyp->CLASS  = class;
  ptvbtyp->NAME   = name;
  ptvbtyp->LEFT   = left;
  ptvbtyp->RIGHT  = right;
  ptvbtyp->VALUE  = value;
  ptvbtyp->SIZE   = size;
  ptvbtyp->BYTE   = byte;
  ptvbtyp->BASE   = base;
  ptvbtyp->INDEX  = typeid;
  ptvbtyp->RESOLV = resolv;

  ptvbtyp->NEXT   = Figure->BETYP;;
  ptvbtyp->LINE   = line;
  Figure->BETYP   = ptvbtyp;

  addauthelem( Figure->HASH_TYPE, name, (long)ptvbtyp );

  return (ptvbtyp);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbvar						*/
/* description	: create a VBVAR structure at the top the list		*/
/* called func.	: addptype, autallocheap, namealloc			*/
/* ###--------------------------------------------------------------### */

struct ptype *vbh_addvbvar (lastptype, target, vex, type,line)

struct ptype  *lastptype;	/* pointer on the last ptype structure	*/
vexexpr       *target;		/* signal's name			*/
vexexpr       *vex;		/* condition's expression		*/
vbtyp_list    *type;		/* signal's type (cannot be 0)		*/
long           line;
{
  struct ptype *ptptype;
  struct vbvar *ptvbvar;

  ptvbvar           = (struct vbvar *) autallocheap (sizeof(struct vbvar));
  ptvbvar->TARGET   = target;
  ptvbvar->VEX      = vex;
  ptvbvar->TYPE     = type;
  ptvbvar->LINE     = line;

  ptptype           = addptype (lastptype, VBH_BEVAR, (void *)ptvbvar);

  return (ptptype);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_addvbwas						*/
/* description	: create a VBWAS (sequential wait statement)		*/
/* called func.	: autallocheap						*/
/* ###--------------------------------------------------------------### */

struct vbwas *vbh_addvbwas (sen, cnd,timeunit, timeout,line)

struct chain *sen;            /* senisitvity list                     */
vexexpr      *cnd;            /* condition clause                     */
unsigned char timeunit;       /* timeout clause                       */
vexexpr      *timeout;        /* timeout clause                       */
long          line;
{
  struct vbwas *ptwas;

  ptwas          = (struct vbwas *) autallocheap (sizeof(struct vbwas));
  ptwas->SEN       = sen;
  ptwas->CND       = cnd;
  ptwas->TIME_UNIT = timeunit;
  ptwas->TIMEOUT   = timeout;
  ptwas->LINE      = line;

  return (ptwas);
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_prdeftyp						*/
/* description	: create BETYPs for predefined types			*/
/* called func.	: vbh_addvbtyp, namealloc				*/
/* ###--------------------------------------------------------------### */

struct vbtyp *vbh_prdeftyp( VbFigure )

  vbfig_list *VbFigure;
{
  static char         *bit_vl[2];
  static char         *boolean_vl[2];
  static char         *severity_vl[2];
  static char         *std_ulogic_vl[9];

  struct vbtyp        *TypeBit;
  struct vbtyp        *TypeReg;
  struct vbtyp        *TypeMux;
//struct vbtyp        *TypeWor;
  struct vbtyp        *TypeInt;
  struct vbtyp        *TypeStd_logic;
  struct vbtyp        *TypeStd_ulogic;
  struct vbtyp        *TypeCharacter;

  boolean_vl[0] = namealloc ("false");
  boolean_vl[1] = namealloc ("true");

  bit_vl[0] = namealloc ("'0'");
  bit_vl[1] = namealloc ("'1'");

  severity_vl[0] = namealloc ("warning");
  severity_vl[1] = namealloc ("error");

  std_ulogic_vl[ 0 ] = namealloc("'u'");
  std_ulogic_vl[ 1 ] = namealloc("'x'");
  std_ulogic_vl[ 2 ] = namealloc("'0'");
  std_ulogic_vl[ 3 ] = namealloc("'1'");
  std_ulogic_vl[ 4 ] = namealloc("'z'");
  std_ulogic_vl[ 5 ] = namealloc("'w'");
  std_ulogic_vl[ 6 ] = namealloc("'l'");
  std_ulogic_vl[ 7 ] = namealloc("'h'");
  std_ulogic_vl[ 8 ] = namealloc("'-'");

	/* ###------------------------------------------------------### */
	/*    Enumeration types						*/
	/* ###------------------------------------------------------### */

  vbh_addvbtyp( VbFigure,"severity_level",0,1,severity_vl,
                2,1,NULL,VBH_TYPE_SEVERITY,NULL,'E',-1);
  vbh_addvbtyp( VbFigure,"boolean"       ,0,1,boolean_vl,
                2,1,NULL,VBH_TYPE_BOOLEAN,NULL,'E',-1);

  TypeBit = vbh_addvbtyp( VbFigure,"bit" ,0,1,bit_vl,
                2,1,NULL,VBH_TYPE_BIT,NULL,'E',-1);

  TypeCharacter = vbh_addvbtyp( VbFigure,"character"     ,0,127,NULL,
                               0,1,NULL,VBH_TYPE_CHARACTER,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"string",0,0x7fffffff, NULL,
                0,1,TypeCharacter,VBH_TYPE_STRING,NULL,'U',-1);

	/* ###------------------------------------------------------### */
	/*    Integer types						*/
	/* ###------------------------------------------------------### */

  TypeInt = vbh_addvbtyp(VbFigure,"integer",0x80000000,0x7fffffff,NULL,
               0,32,NULL,VBH_TYPE_INTEGER,NULL,'I',-1);

	/* ###------------------------------------------------------### */
	/*    Subtypes							*/
	/* ###------------------------------------------------------### */

  vbh_addvbtyp( VbFigure,"natural",0,0x7fffffff,NULL,
                0,31,TypeInt,VBH_TYPE_NATURAL,NULL,'I',-1);

	/* ###------------------------------------------------------### */
	/*    Unconstraint array types					*/
	/* ###------------------------------------------------------### */

  vbh_addvbtyp( VbFigure,"bit_vector",0,0x7fffffff, NULL,
                0,1,TypeBit,VBH_TYPE_BIT_VECTOR,NULL,'U',-1);

	/* ###------------------------------------------------------### */
	/*    stdlogic_1164 package types				*/
	/* ###------------------------------------------------------### */

  TypeStd_ulogic = vbh_addvbtyp( VbFigure, "std_ulogic" ,0,8,std_ulogic_vl,
                9,1,NULL,VBH_TYPE_STD_ULOGIC,NULL,'E',-1);

  TypeStd_logic = vbh_addvbtyp( VbFigure,"std_logic",0,1,NULL,
                0,1,TypeStd_ulogic,VBH_TYPE_STD_LOGIC,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"std_ulogic_vector",0,0x7fffffff, NULL,
                0,1,TypeStd_ulogic,VBH_TYPE_STD_ULOGIC_VECTOR,NULL,'U',-1);

  vbh_addvbtyp( VbFigure,"std_logic_vector",0,0x7fffffff, NULL,
                0,1,TypeStd_logic,VBH_TYPE_STD_LOGIC_VECTOR,NULL,'U',-1);

  vbh_addvbtyp( VbFigure,"x01",1,3,NULL,
                0,1,TypeStd_logic,VBH_TYPE_X01,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"x01z",1,4,NULL,
                0,1,TypeStd_logic,VBH_TYPE_X01Z,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"ux01",0,3,NULL,
                0,1,TypeStd_logic,VBH_TYPE_UX01,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"ux01z",0,4,NULL,
                0,1,TypeStd_logic,VBH_TYPE_UX01Z,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"unsigned",0,0x7fffffff, NULL,
                0,1,TypeStd_logic,VBH_TYPE_UNSIGNED,NULL,'U',-1);

  vbh_addvbtyp( VbFigure,"signed",0,0x7fffffff, NULL,
                0,1,TypeStd_logic,VBH_TYPE_SIGNED,NULL,'U',-1);

  vbh_addvbtyp( VbFigure,"small_int",0,1, NULL,
                0,1,TypeInt,VBH_TYPE_SMALL_INT,NULL,'I',-1);

  TypeReg = vbh_addvbtyp( VbFigure,"reg_bit" ,0,1,bit_vl,
                          2,1,NULL,VBH_TYPE_REG_BIT,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"reg_vector",0,0x7fffffff, NULL,
                0,1,TypeReg,VBH_TYPE_REG_VECTOR,NULL,'U',-1);

  TypeMux = vbh_addvbtyp( VbFigure,"mux_bit" ,0,1,bit_vl,
                          2,1,NULL,VBH_TYPE_MUX_BIT,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"mux_vector",0,0x7fffffff, NULL,
                0,1,TypeMux,VBH_TYPE_MUX_VECTOR,NULL,'U',-1);

/*TypeWor =*/ vbh_addvbtyp( VbFigure,"wor_bit" ,0,1,bit_vl,
                            2,1,NULL,VBH_TYPE_WOR_BIT,NULL,'E',-1);

  vbh_addvbtyp( VbFigure,"wor_vector",0,0x7fffffff, NULL,
                0,1,TypeMux,VBH_TYPE_WOR_VECTOR,NULL,'U',-1);

  return ( VbFigure->BETYP );
}

vbtyp_list *vbh_getvbtyp( VbFigure, Name )

  vbfig_list *VbFigure;
  char       *Name;
{
  authelem *Element;

  if ( Name == (char *)0 )
  {
    return( (vbtyp_list *)0 );
  }

  Name    = namealloc( Name );
  Element = searchauthelem( VbFigure->HASH_TYPE, Name );

  if ( Element != (authelem *)0 )
  {
    return( (vbtyp_list *)Element->VALUE );
  }

  return( (vbtyp_list *)0 );
}

vbtyp_list *vbh_getvbtypbyindex( VbFigure, Index )

  vbfig_list *VbFigure;
  int         Index;
{
  vbtyp_list *VbType;

  for ( VbType  = VbFigure->BETYP;
        VbType != (vbtyp_list *)0;
        VbType  = VbType->NEXT )
  {
    if ( VbType->INDEX == Index ) return( VbType );
  }

  return( (vbtyp_list *)0 );
}
