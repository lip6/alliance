/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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
| File    :                  G_rpeek.c                        |
|                                                             |
| Authors :                Frederic Petrot                    |
|                    Modified by Ludovic Jacomme              |
|                                                             |
| Date    :                   08.05.94                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Include File                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

# include "G_rpeek.h"

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/* computes the symetry for a given couple of points in a
   given instance in a given figure */

void peek_sym(xout, yout, x, y, xins, yins, x1, y1, x2, y2, trsf)
long *xout, *yout, x, y, xins, yins, x1, y1, x2, y2;
char trsf;
{
  switch (trsf) 
  {
    case NOSYM :
      *xout = xins + x - x1;
      *yout = yins + y - y1;
      return;
    case SYM_X :
      *xout = xins - x + x2;
      *yout = yins + y - y1;
      return;
    case SYM_Y :
      *xout = xins + x - x1;
      *yout = yins - y + y2;
      return;
    case SYMXY :
      *xout = xins - x + x2;
      *yout = yins - y + y2;
      return;
    default :
      *xout = xins + x - x1;
      *yout = yins + y - y1;
      return;
  }
}

/* flattens the mbk database for a duplicated structure for peeking
   obliged to do so, i can't see another way now 11/27/90 */
void peek_flat(ptfig, pt_ins)

  phfig_list *ptfig;
  phins_list *pt_ins;
{
phseg_list *ptseg = NULL;
phvia_list *ptvia = NULL;
phref_list *ptref = NULL;
phins_list *ptins = NULL, *ptinstbf = NULL;
phfig_list *ptfigtbf = NULL, *pt = NULL;
char *newname;
long x1, y1, x2, y2, xins, yins, xab1, yab1, xab2, yab2, dx, dy;
long vx1, vx2, vy1, vy2;
char trsf, newtrsf;

  ptinstbf = pt_ins;
  ptfigtbf = getphfig(ptinstbf->FIGNAME,'A');
  xins = ptinstbf->XINS;
  yins = ptinstbf->YINS;
  xab1 = ptfigtbf->XAB1;
  xab2 = ptfigtbf->XAB2;
  yab1 = ptfigtbf->YAB1;
  yab2 = ptfigtbf->YAB2;
  trsf = ptinstbf->TRANSF;
  /* segments */
  for (ptseg = ptfigtbf->PHSEG ; ptseg ; ptseg = ptseg->NEXT) {
    peek_sym(&x1,&y1,ptseg->X1,ptseg->Y1,xins,yins,xab1,yab1,xab2,yab2,trsf);
    peek_sym(&x2,&y2,ptseg->X2,ptseg->Y2,xins,yins,xab1,yab1,xab2,yab2,trsf);
    addphseg(ptfig,ptseg->LAYER,ptseg->WIDTH,x1,y1,x2,y2,ptseg->NAME);
  }
  /* vias */
  for (ptvia = ptfigtbf->PHVIA ; ptvia ; ptvia = ptvia->NEXT) {

		dx = ptvia->DX >> 1;
		dy = ptvia->DY >> 1;
		vx1 = ptvia->XVIA - dx;
		vy1 = ptvia->YVIA - dy;
		vx2 = ptvia->XVIA + dx;
		vy2 = ptvia->YVIA + dy;
		peek_sym(&x1, &y1, vx1, vy1, xins, yins, xab1, yab1, xab2, yab2, trsf);
		peek_sym(&x2, &y2, vx2, vy2, xins, yins, xab1, yab1, xab2, yab2, trsf);

		if ( x1 > x2 ) { vx1 = x2; vx2 = x1; }
		else           { vx1 = x1; vx2 = x2; }
		
		if ( y1 > y2 ) { vy1 = y2; vy2 = y1; }
		else           { vy1 = y1; vy2 = y2; }

		dx = vx2 - vx1;
		dy = vy2 - vy1;

		(void)addphvia(ptfig, ptvia->TYPE,
                               vx1 + (dx >> 1), vy1 + (dy >> 1), dx, dy ,NULL );
  }
  /* references */
  for (ptref = ptfigtbf->PHREF ; ptref ; ptref = ptref->NEXT) {
    peek_sym(&x1,&y1,ptref->XREF,ptref->YREF,xins,yins,
             xab1,yab1,xab2,yab2,trsf);
    addphref(ptfig,ptref->FIGNAME,ptref->NAME,x1,y1);
  }
  /*  instances */
  for (ptins = ptfigtbf->PHINS ; ptins ; ptins = ptins->NEXT) {
    peek_sym(&x1,&y1,ptins->XINS,ptins->YINS,xins,yins,
             xab1,yab1,xab2,yab2,trsf);
    pt = getphfig(ptins->FIGNAME,'P');
    dx = pt->XAB2 - pt->XAB1;
    dy = pt->YAB2 - pt->YAB1;
    newtrsf = ptins->TRANSF;
    switch (trsf) {
      case NOSYM :
        break;
      case SYM_X :
        x1 = x1 - dx;
        switch (newtrsf) {
          case NOSYM : newtrsf = SYM_X; break;
          case SYM_X : newtrsf = NOSYM; break;
          case SYM_Y : newtrsf = SYMXY; break;
          case SYMXY : newtrsf = SYM_Y; break;
        }
        break;
      case SYM_Y :
        y1 = y1 - dy;
        switch (newtrsf) {
          case NOSYM : newtrsf = SYM_Y; break;
          case SYM_X : newtrsf = SYMXY; break;
          case SYM_Y : newtrsf = NOSYM; break;
          case SYMXY : newtrsf = SYM_X; break;
        }
        break;
    case SYMXY :
        x1 = x1 - dx;
        y1 = y1 - dy;
        switch (newtrsf) {
          case NOSYM : newtrsf = SYMXY; break;
          case SYM_X : newtrsf = SYM_Y; break;
          case SYM_Y : newtrsf = SYM_X; break;
          case SYMXY : newtrsf = NOSYM; break;
        }
        break;
    }
    newname = (char *)mbkalloc(strlen(ptins->FIGNAME) +
            strlen(ptins->INSNAME) + 2);
    sprintf(newname, "%s.%s", ptins->FIGNAME, ptins->INSNAME); 
    addphins(ptfig,ptins->FIGNAME,newname,newtrsf,x1,y1);
  }
  delphins(ptfig,ptinstbf->INSNAME);
/*
** Modified by L.Jacomme 07/06/95
*/
  delphfig( ptfigtbf->NAME );
}

/* recursivly flattens the mbk database duplicated structure for peeking.  */
void r_peek_flat(ptfig)
phfig_list *ptfig;
{ 
struct phins *p;

  for (p = ptfig->PHINS; p != NULL; p = ptfig->PHINS) { 
    peek_flat(ptfig, p);
  }
}
/* duplicate a figure in 'P' mode */
phfig_list *dup_phfig(ptfig)
phfig_list *ptfig;
{
phfig_list *dup;
phins_list *dup_ins;
phseg_list *dup_seg;
phvia_list *dup_via;
phref_list *dup_ref;
char newname[25];

  sprintf(newname, "f_%s", ptfig->NAME);
  for (dup = HEAD_PHFIG; dup != NULL; dup = dup->NEXT) 
     if (!strcmp(dup->NAME, newname))
       return dup;

  dup = addphfig(newname);
  dup->XAB1 = ptfig->XAB1;
  dup->YAB1 = ptfig->YAB1;
  dup->XAB2 = ptfig->XAB2;
  dup->YAB2 = ptfig->YAB2;
  for (dup_ins = ptfig->PHINS; dup_ins != NULL; dup_ins = dup_ins->NEXT)
    addphins(dup,dup_ins->FIGNAME,dup_ins->INSNAME,dup_ins->TRANSF,
           dup_ins->XINS,dup_ins->YINS);
  for (dup_seg = ptfig->PHSEG; dup_seg != NULL; dup_seg = dup_seg->NEXT)
    addphseg(dup,dup_seg->LAYER,dup_seg->WIDTH,
           dup_seg->X1,dup_seg->Y1,dup_seg->X2,dup_seg->Y2,dup_seg->NAME);
  for (dup_via = ptfig->PHVIA; dup_via != NULL; dup_via = dup_via->NEXT)
    addphvia(dup,dup_via->TYPE,dup_via->XVIA,dup_via->YVIA,dup_via->DX,dup_via->DY,
        dup_via->NAME);
  for (dup_ref = ptfig->PHREF; dup_ref != NULL; dup_ref = dup_ref->NEXT)
    addphref(dup,dup_ref->FIGNAME,dup_ref->NAME,dup_ref->XREF,dup_ref->YREF);
  return dup;
}
