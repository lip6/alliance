/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 *
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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

/*
 * Tool    : BooG - mapping standard cells on abl
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <stdio.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include <abl.h>
#include <abe.h>
#include "bog_lib_utils.h"
#include "bog_signal_utils.h"
#include "bog_signal_nameindex.h"
#include "bog_map_pattern.h"
#include "bog_map_befig.h"
#include "bog_map_abl.h"


static unsigned int AREA;     /*sum of cells area*/


/******************************************************************************/
/*                initialize area evaluation                                  */
/******************************************************************************/
extern void begin_count_area()
{
   AREA=0;
}


/******************************************************************************/
/*                return area evaluation                                      */
/******************************************************************************/
extern unsigned int count_area()
{
   return AREA;
}


/******************************************************************************/
/* add references to losig for instance loins                                 */
/******************************************************************************/
static void addlofigchain(loins_list* loins)
{
   locon_list *ptcon;
   losig_list *ptsig;
   ptype_list *ptype;
   
   for (ptcon=loins->LOCON; ptcon; ptcon=ptcon->NEXT) {
	      ptsig=ptcon->SIG;
	      ptype = getptype(ptsig->USER, LOFIGCHAIN);
         if (!ptype) {
            ptsig->USER=addptype(ptsig->USER,LOFIGCHAIN,NULL);
            ptype=ptsig->USER;
         }   
	      ptype->DATA = addchain((chain_list *)ptype->DATA, ptcon);
	}

}


/****************************************************************************/
/* build loins and its connections in lofig according to abl                */
/* output is the signal of the variable which the abl defines               */
/****************************************************************************/
static losig_list* loc_map_abl(lofig_list *lofig, chain_list *abl)
{
   losig_list* losig;
   locon_list* locon;
   loins_list* loinsStart;
   port_list* port, *top;
   chain_list* sigchain = NULL, *new, *pred=NULL;
   char* name;
   char* insname;
   char* signame;
   chain_list* namechain;
   cell_list* cell;
   losig_list* output_losig;
   loins_list* loins;
   char memo;
   int master;
   long delay;

   if (!abl || !lofig) {
      fprintf(stderr,"loc_map_abl: NULL pointer\n");
      exit(1);
   }
   
   /*constants are considered as signal names*/
   if (ABL_ATOM(abl)) return seeksignal(ABL_ATOM_VALUE(abl));

   /*first recursion?*/
   master=first_eval();
   
   /*map operator*/   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern(abl);

   name=getnameindex(cell->NAME);
   signame=losig_name(name);
   insname=loins_name(name);
   putdelay(insname,cell->DELAY); /*delay for loins*/
   putdelay(signame,cell->DELAY); /*delay for new signal*/
   delay = cell->DELAY;
  
   /*output_losig */
   namechain=addchain(NULL,signame);
   output_losig=addlosig(lofig, getindex(), namechain, INTERNAL);

   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) losig=seeksignal(VSS);
      else if (isvdd(port->NAME)) losig=seeksignal(VDD);
      else switch (port->DIRECTION) {
      case OUT: case TRISTATE: losig=output_losig; break;
      default: 
         if (!port->ABL) {
            fprintf(stderr,
            "loc_map_abl: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         losig=loc_map_abl(lofig,port->ABL); 
         break;
      }
      new=addchain(NULL,losig);
      if (pred) pred->NEXT=new;
      else sigchain=new;
      pred=new;
   }

   /*insert instance*/
   loins=addloins(lofig,insname,getlofig(cell->NAME,'P'),sigchain);
   addlofigchain(loins);
   freechain(sigchain);
   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*if cell doesn't exist but is composed by several cells, develop it*/
   if (cell->MODE=='A' || cell->MODE=='a') {
      /*insertion de nouvelles instances, la premiere est supprimee*/
      loinsStart=lofig->LOINS->NEXT;
      memo=SEPAR;
      SEPAR='_';
      flattenlofig(lofig,insname,YES/*change internal signal name*/);
      SEPAR=memo;
      /*put delay on new instances inserted*/
      for ( loins=lofig->LOINS; loins!=loinsStart; loins=loins->NEXT )
      {
         signame=loins->INSNAME;
         putdelay( signame, delay ); 
         /*put delay on new signals inserted*/
         for ( locon=loins->LOCON; locon; locon=locon->NEXT )
         {
            losig = locon->SIG;
            /*remove added name on old signal*/
            if ( losig->NAMECHAIN->NEXT ) 
            {
               losig->NAMECHAIN = losig->NAMECHAIN->NEXT;
               continue;
            }
            signame=(char*)losig->NAMECHAIN->DATA;
            putdelay( signame, delay ); 
         }
      }
      setindex( lofig->LOSIG->INDEX + 1 );
   }

   AREA+=cell->AREA;
   return output_losig;
}


/****************************************************************************/
/* build loins and its connections in lofig according to abl                */
/* output is the signal of the variable which the abl defines               */
/****************************************************************************/
extern void map_abl(lofig_list *lofig, chain_list *abl, losig_list *output_losig)
{
   losig_list* losig;
   locon_list* locon;
   loins_list* loinsStart;
   loins_list* loins;
   port_list* port, *top;
   chain_list* sigchain = NULL, *new, *pred=NULL;
   char* insname;
   char* signame;
   cell_list* cell;
   char memo;
   int master;
   long delay;

   if (!abl || !lofig) {
      fprintf(stderr,"map_abl: NULL pointer\n");
      exit(1);
   }

   if (!output_losig) {
      fprintf(stderr,"map_abl: no losig for output\n");
      exit(1);
   }

   /*first recursion?*/
   master=first_eval();
   
   /*search buffer, one or zero constant*/
   cell=cell_pattern(abl);

   signame=(char*)output_losig->NAMECHAIN->DATA;
   insname=loins_name(signame);
   putdelay(insname,cell->DELAY); /*delay for loins*/
   delay = cell->DELAY;
   
   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) losig=seeksignal(VSS);
      else if (isvdd(port->NAME)) losig=seeksignal(VDD);
      else switch (port->DIRECTION) {
      case OUT: case TRISTATE: losig=output_losig; break;
      default: 
         if (!port->ABL) {
            fprintf(stderr,
            "map_abl: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         losig=loc_map_abl(lofig,port->ABL); 
      }
      new=addchain(NULL,losig);
      if (pred) pred->NEXT=new;
      else sigchain=new;
      pred=new;
   }

   loins=addloins(lofig,insname,getlofig(cell->NAME,'P'),sigchain);
   addlofigchain(loins);
   freechain(sigchain);
   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();
   
   /*if cell doesn't exist but is composed by several cells, develop it*/
   if (cell->MODE=='A' || cell->MODE=='a') {
      /*insertion de nouvelles instances, la premiere est supprimee*/
      loinsStart=lofig->LOINS->NEXT;
      memo=SEPAR;
      SEPAR='_';
      flattenlofig(lofig,insname,YES/*change internal signal name*/);
      SEPAR=memo;
      /*put delay on new instances inserted*/
      for ( loins=lofig->LOINS; loins!=loinsStart; loins=loins->NEXT )
      {
         signame=loins->INSNAME;
         putdelay( signame, delay ); 
         /*put delay on new signals inserted*/
         for ( locon=loins->LOCON; locon; locon=locon->NEXT )
         {
            losig = locon->SIG;
            /*remove added name on old signal*/
            if ( losig->NAMECHAIN->NEXT ) 
            {
               losig->NAMECHAIN = losig->NAMECHAIN->NEXT;
               continue;
            }
            signame=(char*)losig->NAMECHAIN->DATA;
            putdelay( signame, delay ); 
         }
      }
      setindex( lofig->LOSIG->INDEX + 1 );
   }   

   AREA+=cell->AREA;
}


/******************************************************************************/
/*                            map a tristate                                  */
/******************************************************************************/
extern void map_bus(lofig_list* lofig, biabl_list* biabl, losig_list* output_losig)
{
   losig_list* losig;
   locon_list* locon;
   loins_list* loinsStart;
   loins_list* loins;
   chain_list* sigchain = NULL, *new, *pred=NULL;
   char* signame;
   char* insname;
   port_list* port, *top;
   cell_list* cell;
   char memo;
   int master;
   biabl_list* biabl_aux;
   long delay;

   if (!biabl || !lofig) {
      fprintf(stderr,"map_bus: NULL pointer\n");
      exit(1);
   }

   if (!output_losig) {
      fprintf(stderr,"map_bus: no losig for output\n");
      exit(1);
   }


   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern_bus(biabl);

   insname=biabl->LABEL;
   
   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
      
   /*give unique name*/
   if (!biabl) insname=(char*)output_losig->NAMECHAIN->DATA;
   else {
      memo=SEPAR;
      SEPAR='_';
      insname=concatname((char*)output_losig->NAMECHAIN->DATA,insname);
      SEPAR=memo;
   }  

   insname=loins_name(insname);
   putdelay(insname,cell->DELAY); /*delay for loins*/
   delay = cell->DELAY;
   
   top=copyport(cell->PORT);
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) losig=seeksignal(VSS);
      else if (isvdd(port->NAME)) losig=seeksignal(VDD);
      else switch (port->DIRECTION) {
      case OUT: case TRISTATE: losig=output_losig; break;
      default: 
         if (!port->ABL) {
            fprintf(stderr,
            "map_bus: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         losig=loc_map_abl(lofig,port->ABL);
         break;
      }
      new=addchain(NULL,losig);
      if (pred) pred->NEXT=new;
      else sigchain=new;
      pred=new;
   }

   loins=addloins(lofig,insname,getlofig(cell->NAME,'P'),sigchain);
   addlofigchain(loins);
   freechain(sigchain);
   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();
   
   /*if cell doesn't exist but is composed by several cells, develop it*/
   if (cell->MODE=='A' || cell->MODE=='a') {
      /*insertion de nouvelles instances, la premiere est supprimee*/
      loinsStart=lofig->LOINS->NEXT;
      memo=SEPAR;
      SEPAR='_';
      flattenlofig(lofig,insname,YES/*change internal signal name*/);
      SEPAR=memo;
      /*put delay on new instances inserted*/
      for ( loins=lofig->LOINS; loins!=loinsStart; loins=loins->NEXT )
      {
         signame=loins->INSNAME;
         putdelay( signame, delay ); 
         /*put delay on new signals inserted*/
         for ( locon=loins->LOCON; locon; locon=locon->NEXT )
         {
            losig = locon->SIG;
            /*remove added name on old signal*/
            if ( losig->NAMECHAIN->NEXT ) 
            {
               losig->NAMECHAIN = losig->NAMECHAIN->NEXT;
               continue;
            }
            signame=(char*)losig->NAMECHAIN->DATA;
            putdelay( signame, delay ); 
         }
      }
      setindex( lofig->LOSIG->INDEX + 1 );
   }
   
   AREA+=cell->AREA;
   
   /*look the other blocks*/
   if (biabl) {
      map_bus(lofig,biabl,output_losig);
   }
   
}


/******************************************************************************/
/*                    map on a simple register                                */
/******************************************************************************/
extern void map_register(lofig_list* lofig, biabl_list* biabl, losig_list* output_losig)   
{
   losig_list* losig;
   locon_list* locon;
   loins_list* loins;
   loins_list* loinsStart;
   chain_list* sigchain = NULL, *new, *pred=NULL;
   port_list* port, *top;
   char* signame;
   char* insname;
   char memo;
   cell_list* cell;
   int master;
   biabl_list* biabl_aux;
   long delay;
  
   if (!biabl || !lofig) {
      fprintf(stderr,"map_register: NULL pointer\n");
      exit(1);
   }

   if (!output_losig) {
      fprintf(stderr,"map_register: no losig for output\n");
      exit(1);
   }

   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern_reg(biabl);

   insname=biabl->LABEL;
   
   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
      
   /*give unique name*/
   if (!biabl) insname=(char*)output_losig->NAMECHAIN->DATA;
   else {
      memo=SEPAR;
      SEPAR='_';
      insname=concatname((char*)output_losig->NAMECHAIN->DATA,insname);
      SEPAR=memo;
   }   
  
   insname=loins_name(insname);
   putdelay(insname,cell->DELAY); /*delay for loins*/
   delay = cell->DELAY;
   
   top=copyport(cell->PORT);
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) losig=seeksignal(VSS);
      else if (isvdd(port->NAME)) losig=seeksignal(VDD);
      else switch (port->DIRECTION) {
      case OUT: case TRISTATE: losig=output_losig; break;
      default: 
         if (!port->ABL) {
            fprintf(stderr,
            "map_register: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         losig=loc_map_abl(lofig,port->ABL);
         break;
      }
      new=addchain(NULL,losig);
      if (pred) pred->NEXT=new;
      else sigchain=new;
      pred=new;
   }

   loins=addloins(lofig,insname,getlofig(cell->NAME,'P'),sigchain);
   addlofigchain(loins);
   freechain(sigchain);
   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*if cell doesn't exist but is composed by several cells, develop it*/
   if (cell->MODE=='A' || cell->MODE=='a') {
      /*insertion de nouvelles instances, la premiere est supprimee*/
      loinsStart=lofig->LOINS->NEXT;
      memo=SEPAR;
      SEPAR='_';
      flattenlofig(lofig,insname,YES/*change internal signal name*/);
      SEPAR=memo;
      /*put delay on new instances inserted*/
      for ( loins=lofig->LOINS; loins!=loinsStart; loins=loins->NEXT )
      {
         signame=loins->INSNAME;
         putdelay( signame, delay ); 
         /*put delay on new signals inserted*/
         for ( locon=loins->LOCON; locon; locon=locon->NEXT )
         {
            losig = locon->SIG;
            /*remove added name on old signal*/
            if ( losig->NAMECHAIN->NEXT ) 
            {
               losig->NAMECHAIN = losig->NAMECHAIN->NEXT;
               continue;
            }
            signame=(char*)losig->NAMECHAIN->DATA;
            putdelay( signame, delay ); 
         }
      }
      setindex( lofig->LOSIG->INDEX + 1 );
   }
   
   AREA+=cell->AREA;
   
   /*look the other blocks*/
   if (biabl) {
      map_register(lofig,biabl,output_losig);
   }
   
}



