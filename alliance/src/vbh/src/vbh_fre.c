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
/* file		: vbh_free.c						*/
/* date		: Aug 24 1992						*/
/* version	: v102							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: contains a specific function used to delete a list	*/
/*		  of behaviour data structures				*/
/*									*/
/* ###--------------------------------------------------------------### */

#include "mut.h" 
#include "aut.h"
#include "vex.h" 
#include "vbh.h" 

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbfig						*/
/* description	: delete a list of BEFIG structures and all objects	*/
/*		  pointed by any os BEFIGs in the list			*/
/* called func.	: autfreeblock						*/
/* ###--------------------------------------------------------------### */

extern struct vbfig *VBL_HEADFIG;

void vbh_delvbfig ( ptvbfig )

struct vbfig *ptvbfig;	/* the vbfig to be deleted */

{
  struct vbfig  *scanvbfig;
  struct vbfig **prevvbfig;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  prevvbfig = &VBL_HEADFIG;
  
  for ( scanvbfig  = VBL_HEADFIG;
        scanvbfig != (vbfig_list *)0;
        scanvbfig  = scanvbfig->NEXT )
  {
    if ( scanvbfig == ptvbfig )
    {
      *prevvbfig = scanvbfig->NEXT;
      scanvbfig->NEXT = (vbfig_list *)0;

      break;
    }

    prevvbfig = &scanvbfig->NEXT;
  }

  vbh_frevbfig( scanvbfig );
}

void vbh_frevbfig (listvbfig)

struct vbfig *listvbfig;	/* list of vbfig to be deleted		*/

{
  struct vbfig *ptvbfig;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbfig != NULL)
  {
    vbh_frevbatr (listvbfig->BEATR);
    vbh_frevbaux (listvbfig->BEAUX);
    vbh_frevbcst (listvbfig->BECST);
    vbh_frevbgen (listvbfig->BEGEN);
    vbh_frevbpor (listvbfig->BEPOR);
    vbh_frevbpcs (listvbfig->BEPCS);
    vbh_frevbfun (listvbfig->BEFUN);
    vbh_frevbtyp (listvbfig->BETYP);
    vbh_frevbmod (listvbfig->BEMOD);
    vbh_frevbins (listvbfig->BEINS);
    vbh_frevbgnr (listvbfig->BEGNR);

    destroyauthtable( listvbfig->HASH_TYPE );

    freechain( listvbfig->PACK_LIST );

    ptvbfig   = listvbfig;
    listvbfig = listvbfig->NEXT;
    autfreeblock (ptvbfig);
  }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbmap						*/
/* description	: delete a list of BEMOD structures and all objects	*/
/*		  pointed by any os BEMODs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbmap (listvbmap)

struct vbmap *listvbmap;	/* list of vbmap to be deleted		*/

{
  struct vbmap *ptvbmap;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbmap != NULL)
    {
      freevexexpr( listvbmap->FORMAL );
      freevexexpr( listvbmap->ACTUAL );

    ptvbmap   = listvbmap;
    listvbmap = listvbmap->NEXT;
    autfreeheap (ptvbmap, sizeof(struct vbmap));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbgnr						*/
/* description	: delete a list of BEMOD structures and all objects	*/
/*		  pointed by any os BEMODs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbgnr (listvbgnr)

struct vbgnr *listvbgnr;	/* list of vbgnr to be deleted		*/

{
  struct vbgnr *ptvbgnr;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */
  while (listvbgnr != NULL)
    {
      freevexexpr( listvbgnr->FOR_VARIABLE );
      freevexexpr( listvbgnr->FOR_LEFT     );
      freevexexpr( listvbgnr->FOR_RIGHT    );
      freevexexpr( listvbgnr->IF_COND      );

      vbh_frevbpcs( listvbgnr->BEPCS );
      vbh_frevbins( listvbgnr->BEINS );
      vbh_frevbgnr( listvbgnr->BEGNR );

    ptvbgnr   = listvbgnr;
    listvbgnr = listvbgnr->NEXT;
    autfreeheap (ptvbgnr, sizeof(struct vbgnr));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbins						*/
/* description	: delete a list of BEMOD structures and all objects	*/
/*		  pointed by any os BEMODs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbins (listvbins)

struct vbins *listvbins;	/* list of vbins to be deleted		*/

{
  struct vbins *ptvbins;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbins != NULL)
    {
      vbh_frevbmap( listvbins->PORT_MAP );
      vbh_frevbmap( listvbins->GEN_MAP );

    ptvbins   = listvbins;
    listvbins = listvbins->NEXT;
    autfreeheap (ptvbins, sizeof(struct vbins));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbmod						*/
/* description	: delete a list of BEMOD structures and all objects	*/
/*		  pointed by any os BEMODs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbmod (listvbmod)

struct vbmod *listvbmod;	/* list of vbmod to be deleted		*/

{
  struct vbmod *ptvbmod;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbmod != NULL)
    {
      vbh_frevbpor( listvbmod->BEPOR );
      vbh_frevbgen( listvbmod->BEGEN );

    ptvbmod   = listvbmod;
    listvbmod = listvbmod->NEXT;
    autfreeheap (ptvbmod, sizeof(struct vbmod));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbcst						*/
/* description	: delete a list of BECST structures and all objects	*/
/*		  pointed by any os BECSTs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbcst (listvbcst)

struct vbcst *listvbcst;	/* list of vbcst to be deleted		*/

{
  struct vbcst *ptvbcst;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbcst != NULL)
    {
    freevexexpr (listvbcst->VEX);
    freevexexpr (listvbcst->TARGET);

    ptvbcst   = listvbcst;
    listvbcst = listvbcst->NEXT;
    autfreeheap (ptvbcst, sizeof(struct vbcst));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbgen						*/
/* description	: delete a list of BEGEN structures and all objects	*/
/*		  pointed by any os BEGENs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbgen (listvbgen)

struct vbgen *listvbgen;	/* list of vbgen to be deleted		*/

{
  struct vbgen *ptvbgen;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbgen != NULL)
    {
    freevexexpr (listvbgen->VEX);
    freevexexpr (listvbgen->TARGET);

    ptvbgen   = listvbgen;
    listvbgen = listvbgen->NEXT;
    autfreeheap (ptvbgen, sizeof(struct vbgen));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbatr						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbatr (listvbatr)

struct vbatr *listvbatr;	/* list of vbatr to be deleted		*/

{
  struct vbatr *ptvbatr;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbatr != NULL)
    {
    ptvbatr   = listvbatr;
    listvbatr = listvbatr->NEXT;
    autfreeheap (ptvbatr, sizeof(struct vbatr));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbaux						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbaux (listvbaux)

struct vbaux *listvbaux;	/* list of vbaux to be deleted		*/

{
  struct vbaux *ptvbaux;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, first delete pointed objects	*/
	/* then, delete the object itself				*/
	/* ###------------------------------------------------------### */

  while (listvbaux != NULL)
    {
    freevexexpr (listvbaux->VEX);
    freevexexpr (listvbaux->TARGET);

    ptvbaux   = listvbaux;
    listvbaux = listvbaux->NEXT;
    autfreeheap (ptvbaux, sizeof(struct vbaux));
    }

}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbarg						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbarg (listvbarg)

struct vbarg *listvbarg;	/* list of vbarg to be deleted		*/

{
  struct vbarg *ptvbarg;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listvbarg != NULL)
  {
    freevexexpr (listvbarg->TARGET);

    ptvbarg   = listvbarg;
    listvbarg = listvbarg->NEXT;
    autfreeheap (ptvbarg, sizeof(struct vbarg));
  }
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbpor						*/
/* description	: delete a list of BEAUX structures and all objects	*/
/*		  pointed by any os BEAUXs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbpor (listvbpor)

struct vbpor *listvbpor;	/* list of vbpor to be deleted		*/

{
  struct vbpor *ptvbpor;

	/* ###------------------------------------------------------### */
	/*    for each object of the list, delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listvbpor != NULL)
  {
    freevexexpr (listvbpor->TARGET);

    ptvbpor   = listvbpor;
    listvbpor = listvbpor->NEXT;
    autfreeheap (ptvbpor, sizeof(struct vbpor));
  }
}

void vbh_frevbifs( ScanIfs )

  vbifs_list *ScanIfs;
{
  freevexexpr( ScanIfs->CND );
  vbh_frevbinst( ScanIfs->CNDTRUE  );
  vbh_frevbinst( ScanIfs->CNDFALSE );

  autfreeheap( ScanIfs, sizeof(struct vbifs) );
}

void vbh_frevbfor( ScanFor )

  vbfor_list *ScanFor;
{
  freevexexpr( ScanFor->VARIABLE );
  freevexexpr( ScanFor->LEFT     );
  freevexexpr( ScanFor->RIGHT    );

  vbh_frevbinst( ScanFor->INSTRUCTION );
  autfreeheap( ScanFor, sizeof(struct vbfor) );
}

void vbh_frevblop( ScanLop )

  vblop_list *ScanLop;
{
  vbh_frevbinst( ScanLop->INSTRUCTION );
  autfreeheap( ScanLop, sizeof(struct vblop) );
}

void vbh_frevbwhi( ScanWhi )

  vbwhi_list *ScanWhi;
{
  freevexexpr( ScanWhi->CND );
  vbh_frevbinst( ScanWhi->INSTRUCTION );
  autfreeheap( ScanWhi, sizeof(struct vbwhi) );
}

void vbh_frevbext( ScanExit )

  vbext_list *ScanExit;
{
  freevexexpr( ScanExit->CND );
  autfreeheap( ScanExit, sizeof(struct vbext) );
}

void vbh_frevbnxt( ScanNext )

  vbnxt_list *ScanNext;
{
  freevexexpr( ScanNext->CND );
  autfreeheap( ScanNext, sizeof(struct vbnxt) );
}

void vbh_frevbret( ScanReturn )

  vbret_list *ScanReturn;
{
  freevexexpr( ScanReturn->RET );
  autfreeheap( ScanReturn, sizeof(struct vbret) );
}

void vbh_frevbcal( ScanCall )

  vbcal_list *ScanCall;
{
  freevexexpr( ScanCall->CALL );
  autfreeheap( ScanCall, sizeof(struct vbcal) );
}

void vbh_frevbasg( ScanAsg )

  vbasg_list *ScanAsg;
{
  freevexexpr( ScanAsg->TARGET );
  freevexexpr( ScanAsg->VEX    );

  autfreeheap( ScanAsg, sizeof(struct vbasg) );
}

void vbh_frevbvar( ScanVar )

  vbvar_list *ScanVar;
{
  freevexexpr( ScanVar->TARGET );
  freevexexpr( ScanVar->VEX    );

  autfreeheap( ScanVar, sizeof(struct vbvar) );
}

void vbh_frevbwas( ScanWas )

  vbwas_list *ScanWas;
{
  freechain( ScanWas->SEN    );
  freevexexpr( ScanWas->CND );
  freevexexpr( ScanWas->TIMEOUT );

  autfreeheap( ScanWas, sizeof(struct vbwas) );
}

void vbh_frevbcas( ScanCas )

  vbcas_list *ScanCas;
{
  vbcho_list *ScanCho;
  int         Offset;

  freevexexpr( ScanCas->VEX );

  for ( Offset = 0; Offset < ScanCas->SIZE; Offset++ )
  {
    ScanCho = &ScanCas->CHOICE[ Offset ];
    freechain( ScanCho->VALUES );
    vbh_frevbinst( ScanCho->INSTRUCTION );
  }

  autfreeblock( ScanCas->CHOICE );
  autfreeheap( ScanCas, sizeof(struct vbcas) );
}

void vbh_frevbagr( ScanAgr )

  vbagr_list *ScanAgr;
{
  vbh_frevbinst( ScanAgr->ASSIGN );
  autfreeheap( ScanAgr, sizeof(struct vbagr) );
}

void vbh_frevbinst( HeadType )

  ptype_list *HeadType;
{
  ptype_list *ScanType;

  for ( ScanType  = HeadType;
        ScanType != (ptype_list *)0;
        ScanType  = ScanType->NEXT )
  {
    switch ( ScanType->TYPE )
    {
      case VBH_BEIFS : vbh_frevbifs( ScanType->DATA );
      break;
  
      case VBH_BEASG : vbh_frevbasg( ScanType->DATA );
      break;
  
      case VBH_BEVAR : vbh_frevbvar( ScanType->DATA );
      break;
  
      case VBH_BECAS : vbh_frevbcas( ScanType->DATA );
      break;
  
      case VBH_BEWAS : vbh_frevbwas( ScanType->DATA );
      break;
  
      case VBH_BEWHI : vbh_frevbwhi( ScanType->DATA );
      break;

      case VBH_BELOP : vbh_frevblop( ScanType->DATA );
      break;
  
      case VBH_BEFOR : vbh_frevbfor( ScanType->DATA );
      break;
  
      case VBH_BENXT : vbh_frevbnxt( ScanType->DATA );
      break;
  
      case VBH_BEEXT : vbh_frevbext( ScanType->DATA );
      break;

      case VBH_BECAL : vbh_frevbcal( ScanType->DATA );
      break;

      case VBH_BERET : vbh_frevbret( ScanType->DATA );
      break;

      case VBH_BEAGR : vbh_frevbagr( ScanType->DATA );
    }
  }

  freeptype( HeadType );
}

/* ###--------------------------------------------------------------### */
/* function     : vbh_frevbpcs                                          */
/* description  : delete a list of BEPCS structures and all objects     */
/*                pointed by any os BEPCSs in the list                  */
/* called func. : autfreeheap                                          */
/* ###--------------------------------------------------------------### */

void vbh_frevbpcs (listvbpcs)

struct vbpcs *listvbpcs;        /* list of vbpcs to be deleted          */

{
  struct vbpcs *ptvbpcs;

        /* ###------------------------------------------------------### */
        /*    for each object of the list, first delete pointed objects */
        /* then, delete the object itself                               */
        /* ###------------------------------------------------------### */

  while (listvbpcs != NULL)
  {
    freechain( listvbpcs->SENSITIVITY );

    vbh_frevbinst( listvbpcs->VARIABLE );
    vbh_frevbinst( listvbpcs->INSTRUCTION );
 
    ptvbpcs   = listvbpcs;
    listvbpcs = listvbpcs->NEXT;
    autfreeheap (ptvbpcs, sizeof(struct vbpcs) );
  }
}

/* ###--------------------------------------------------------------### */
/* function     : vbh_frevbfun                                          */
/* description  : delete a list of BEFUN structures and all objects     */
/*                pointed by any os BEFUNs in the list                  */
/* called func. : autfreeheap                                          */
/* ###--------------------------------------------------------------### */

void vbh_frevbfun (listvbfun)

struct vbfun *listvbfun;        /* list of vbfun to be deleted          */

{
  struct vbfun *ptvbfun;

        /* ###------------------------------------------------------### */
        /*    for each object of the list, first delete pointed objects */
        /* then, delete the object itself                               */
        /* ###------------------------------------------------------### */

  while (listvbfun != NULL)
  {
    vbh_frevbarg( listvbfun->RETURN   );
    vbh_frevbarg( listvbfun->ARGUMENT );

    vbh_frevbinst( listvbfun->VARIABLE );
    vbh_frevbinst( listvbfun->INSTRUCTION );
 
    ptvbfun   = listvbfun;
    listvbfun = listvbfun->NEXT;
    autfreeheap (ptvbfun, sizeof(struct vbfun) );
  }
}

/* ###--------------------------------------------------------------### */
/* function	: vbh_frevbtyp						*/
/* description	: delete a list of BETYP structures and all objects	*/
/*		  pointed by any os BETYPs in the list			*/
/* called func.	: autfreeheap						*/
/* ###--------------------------------------------------------------### */

void vbh_frevbtyp (listvbtyp)

struct vbtyp *listvbtyp;	/* list of vbtyp to be deleted		*/

{
  struct vbtyp *ptvbtyp;

	/* ###------------------------------------------------------### */
	/*    for each object of the list delete the object itself	*/
	/* ###------------------------------------------------------### */

  while (listvbtyp != NULL)
    {
    ptvbtyp   = listvbtyp;
    freevexexpr( ptvbtyp->DYNAMIC_LEFT  );
    freevexexpr( ptvbtyp->DYNAMIC_RIGHT );
    listvbtyp = listvbtyp->NEXT;
    autfreeheap (ptvbtyp, sizeof(struct vbtyp) );
    }

}
