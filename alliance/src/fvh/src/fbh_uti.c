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
/***************************************************************************
* Fonctions utiles pour fbfig : optimisation des abl d'un fbfig, evaluation *
* du cout . . .                               *
*           Le 01/09/92.
****************************************************************************/
#include <stdio.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "fvh.h"

/*------------------------------------------------------------------------i
fsp_mapCarExprFbh  : applique une fonction de conversion sur chaque expression
                  d'une fbfig 
---------------------------------------------------------------------------
retour    : un void.
---------------------------------------------------------------------------*/
void fsp_mapCarExprFbh(fbh,func)

fbfig_list *fbh;
chain_list *(*func)();
{
fbout_list *out;
fbreg_list *reg;
fbbus_list *bus;
fbbux_list *bux;
biabl_list *biabl;
fbaux_list *aux;


aux = fbh->BEAUX;
while (aux) 
   {
   if (aux->ABL)
      {
      aux->ABL = (*func)(aux->ABL);
      }
   aux = aux->NEXT;
   }

out = fbh->BEOUT;
while (out) 
   {
   if (out->ABL)
      {
      out->ABL = (*func)(out->ABL);
      }
   out = out->NEXT;
   }

reg = fbh->BEREG;
while (reg) 
   {
   biabl = reg->BIABL;
   while (biabl)
      {
      if (biabl->CNDABL && biabl->VALABL)
         {
         biabl->CNDABL = (*func)(biabl->CNDABL);
         biabl->VALABL = (*func)(biabl->VALABL);
         }
      biabl = biabl->NEXT;
      }
   reg = reg->NEXT;
   }

bus = fbh->BEBUS;
while (bus) 
   {
   biabl = bus->BIABL;
   while (biabl)
      {
      if (biabl->CNDABL && biabl->VALABL)
         {
         biabl->CNDABL = (*func)(biabl->CNDABL);
         biabl->VALABL = (*func)(biabl->VALABL);
         }
      biabl = biabl->NEXT;
      }
   bus = bus->NEXT;
   }

bux = fbh->BEBUX;
while (bux) 
   {
   biabl = bux->BIABL;
   while (biabl)
      {
      if (biabl->CNDABL && biabl->VALABL)
         {
         biabl->CNDABL = (*func)(biabl->CNDABL);
         biabl->VALABL = (*func)(biabl->VALABL);
         }
      biabl = biabl->NEXT;
      }
   bux = bux->NEXT;
   }
}

/*-----------------------------------------------------------------------
fbrinToChain_list : forme une chaine liste des entrees.
          prend l'odre d'apparition.
-----------------------------------------------------------------------
parametres        : fbh.
---------------------------------------------------------------------------
retour      : un pointeur de chain_list.
---------------------------------------------------------------------------*/
chain_list *fbrinToChain_list(fbh)
fbfig_list *fbh;
{
fbrin_list *in;
chain_list *lst;
 
in = fbh->BERIN;    /* entrees primaires */
lst = 0;

while (in)
  {
     lst = addchain(lst,(void *)in->NAME);
   in = in->NEXT;
   };
  
  return(lst);
}

/*-------------------------------------------------------------------------
fsp_countInputFbh   : compte les entrees d'un circuit (fbrin)
---------------------------------------------------------------------------
retour    : un int.
---------------------------------------------------------------------------*/

int fsp_countInputFbh(fbh)
fbfig_list *fbh;
{
fbrin_list *in;
int count = 0;

in = fbh->BERIN;  /* entrees primaires */
while (in)
   {
   in = in->NEXT;
   count++;
   };

return(count);
}
