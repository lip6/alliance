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
 * Tool    : LooN - special netlist for timing dependancies
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "lon_lax_param.h"
#include "lon_lib_utils.h"
#include "lon_signal_utils.h"
#include "lon_signal_name.h"
#include "lon_signal_netlist.h"



/***************************************************************************/
/* increment(if 1) or decrement(if 0) the capacitance on signals branched  */
/* to loins from the values of its ports                                   */
/***************************************************************************/
extern void loins_capacitance(loins_list* loins, int increment)
{
   port_list *port;
   cell_list* cell;
   locon_list* locon;
   losig_list* losig;
   char* signame;

   cell=getCell(loins->FIGNAME);
   if (!cell) {
      fprintf(stderr,"library error: no model '%s.vbe' found\n",
      loins->FIGNAME);
      exit(1);
   }
   
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->INSNAME);
         exit(1);
      }
      /*seek port which match with netlist*/
      for (port=cell->PORT; port; port=port->NEXT) {
         if (port->NAME==locon->NAME) break;
      }
      if (!port) {
         fprintf(stderr,
         "loins_capacitance: connector discrepancy between '%s' and '%s'\n",
         cell->NAME,loins->FIGNAME);
         exit(1);
      }
      
      losig=locon->SIG;
      if (!losig->NAMECHAIN) {
         fprintf(stderr,"loins_capacitance: no losig name\n");
         exit(1);
      }
      signame=losig->NAMECHAIN->DATA;
      if (!isvdd(locon->NAME) && !isvss(locon->NAME) 
       && (locon->DIRECTION==IN || locon->DIRECTION==INOUT 
        || locon->DIRECTION==TRANSCV)) 
         inccapacitance(signame, increment!=0 ? port->C : -port->C );
   }
}


/***************************************************************************/
/* return the Time of a loins                                              */
/***************************************************************************/
extern double loins_max_T(loins_list* loins)
{
   cell_list* cell;
   char* ck=NULL;
   int flip_flop=0;
   double delay, max_delay=-1;
   ptype_list* ptype;
   port_list* port;
   locon_list* locon;
   char* input;
   
   if (!loins) {
      fprintf(stderr,"loins_max_T: NULL pointer\n");
      exit(1);
   }
   
   /*cell properties*/
   cell=getCell(loins->FIGNAME);
   if (!cell) {
      fprintf(stderr,"library error: no cell '%s.vbe' found\n",
      loins->FIGNAME);
      exit(1);
   }
   
   /*is-it a flip-flop?*/
   if (cell->BEFIG->BEREG) {
      ptype=getptype(cell->BEFIG->BEREG->BIABL->USER,ABL_STABLE);
      if (ptype) {
         flip_flop=1;
         ck=(char*) ptype->DATA;
      }
   }
   
   /*eval delay*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->INSNAME);
         exit(1);
      }
      /*eval delay on input dependancy*/
      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
      if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE) continue;
      /*only clock accepted for flip-flop*/
      if (flip_flop && ck!=locon->NAME) continue;
      
      /*seek port which match with netlist*/
      for (port=cell->PORT; port; port=port->NEXT) {
         if (port->NAME==locon->NAME) break;
      }
      if (!port) {
         fprintf(stderr,
         "loins_max_T: connector discrepancy between '%s' and '%s'\n",
         cell->NAME,loins->FIGNAME);
         exit(1);
      }
      
      if (!locon->SIG->NAMECHAIN) {
         fprintf(stderr,"loins_max_T: no losig name\n");
         exit(1);
      }
      input=(char*) locon->SIG->NAMECHAIN->DATA;
      
      /*finaly eval delay*/
      delay=port->T;
      if (delay>max_delay) max_delay=delay;
   }

   return max_delay;
}


/***************************************************************************/
/* return the RC of a loins. output is the name of the signal drived       */
/***************************************************************************/
extern double loins_max_RC(loins_list* loins, char* output)
{
   cell_list* cell;
   char* ck=NULL;
   int flip_flop=0;
   double delay, max_delay=-1;
   ptype_list* ptype;
   port_list* port;
   locon_list* locon;
   char* input;
   
   if (!loins || !output) {
      fprintf(stderr,"loins_max_RC: NULL pointer\n");
      exit(1);
   }
   
   /*cell properties*/
   cell=getCell(loins->FIGNAME);
   if (!cell) {
      fprintf(stderr,"library error: no cell '%s.vbe' found\n",
      loins->FIGNAME);
      exit(1);
   }
   
   /*is-it a flip-flop?*/
   if (cell->BEFIG->BEREG) {
      ptype=getptype(cell->BEFIG->BEREG->BIABL->USER,ABL_STABLE);
      if (ptype) {
         flip_flop=1;
         ck=(char*) ptype->DATA;
      }
   }
   
   /*eval delay*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->INSNAME);
         exit(1);
      }
      /*eval delay on input dependancy*/
      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
      if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE) continue;
      /*only clock accepted for flip-flop*/
      if (flip_flop && ck!=locon->NAME) continue;
      
      /*seek port which match with netlist*/
      for (port=cell->PORT; port; port=port->NEXT) {
         if (port->NAME==locon->NAME) break;
      }
      if (!port) {
         fprintf(stderr,
         "loins_max_RC: connector discrepancy between '%s' and '%s'\n",
         cell->NAME,loins->FIGNAME);
         exit(1);
      }
      
      if (!locon->SIG->NAMECHAIN) {
         fprintf(stderr,"loins_max_RC: no losig name\n");
         exit(1);
      }
      input=(char*) locon->SIG->NAMECHAIN->DATA;
      
      /*finaly eval delay*/
      delay=port->R*getcapacitance(output);
      if (delay>max_delay) max_delay=delay;
   }

   return max_delay;
}


/***************************************************************************/
/* return the delay of a loins. output is the name of the signal drived    */
/***************************************************************************/
extern double loins_delay(loins_list* loins, char* output)
{
   cell_list* cell;
   char* ck=NULL;
   int flip_flop=0;
   double delay, max_delay=-1;
   ptype_list* ptype;
   port_list* port;
   locon_list* locon;
   char* input;
   
   if (!loins || !output) {
      fprintf(stderr,"loins_delay: NULL pointer\n");
      exit(1);
   }
   
   /*cell properties*/
   cell=getCell(loins->FIGNAME);
   if (!cell) {
      fprintf(stderr,"library error: no cell '%s.vbe' found\n",
      loins->FIGNAME);
      exit(1);
   }
   
   /*is-it a flip-flop?*/
   if (cell->BEFIG->BEREG) {
      ptype=getptype(cell->BEFIG->BEREG->BIABL->USER,ABL_STABLE);
      if (ptype) {
         flip_flop=1;
         ck=(char*) ptype->DATA;
      }
   }
   
   /*eval delay*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->INSNAME);
         exit(1);
      }
      /*eval delay on input dependancy*/
      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
      if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE) continue;
      /*only clock accepted for flip-flop*/
      if (flip_flop && ck!=locon->NAME) continue;
      
      /*seek port which match with netlist*/
      for (port=cell->PORT; port; port=port->NEXT) {
         if (port->NAME==locon->NAME) break;
      }
      if (!port) {
         fprintf(stderr,
         "loins_delay: connector discrepancy between '%s' and '%s'\n",
         cell->NAME,loins->FIGNAME);
         exit(1);
      }
      
      if (!locon->SIG) {
         fprintf(stderr,"loins_delay: no losig\n");
         exit(1);
      }
      if (!locon->SIG->NAMECHAIN) {
         fprintf(stderr,"loins_delay: no losig name\n");
         exit(1);
      }
      input=(char*) locon->SIG->NAMECHAIN->DATA;
      
      /*finaly eval delay*/
      delay=getdelay(input)+port->T+port->R*getcapacitance(output);
      if (delay>max_delay) max_delay=delay;
   }

   return max_delay;
}


/***************************************************************************/
/* return the delay of  a losig                                            */
/***************************************************************************/
extern double losig_delay(losig_list* losig)
{
   ptype_list* ptype;
   loins_list* loins;
   locon_list* locon;
   double delay, max_delay=-1;
   char* signame;
   chain_list* lofigchain;

   if (!losig) {
      fprintf(stderr,"losig_delay: NULL pointer\n");
      exit(1);
   }
   
   if (!losig->NAMECHAIN) {
      fprintf(stderr,"losig_delay: no losig name\n");
      exit(1);
   }
   signame=losig->NAMECHAIN->DATA;

   ptype=getptype(losig->USER,LOFIGCHAIN);
   if (!ptype) {
      fprintf(stderr,"losig_delay: no lofigchain on signal '%s'\n",
      signame);
      exit(1);
   }
   
   /*search drivers*/
   for (lofigchain=ptype->DATA; lofigchain; lofigchain=lofigchain->NEXT) {
      locon= (locon_list*) lofigchain->DATA;
      if (!locon) {
         fprintf(stderr,"losig_delay: NULL pointer in lofigchain\n");
      }
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' isn't accepted\n",
         locon->NAME);
         exit(1);
      }
      /*real inputs*/
      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
      if (locon->DIRECTION==IN) continue;
      if (locon->TYPE!=EXTERNAL) {
         loins=locon->ROOT;
         delay=loins_delay(loins,signame);
         if (delay>max_delay) max_delay=delay;
      }
   }   

   return max_delay;
}


/***************************************************************************/
/* propagate modification of a loins for delay                             */
/***************************************************************************/
extern void propagate_loins_delay(loins_list *loins)
{
   locon_list* locon;
   
   /*input first*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->INSNAME);
         exit(1);
      }
      if (locon->DIRECTION==IN || locon->DIRECTION==INOUT || locon->DIRECTION==TRANSCV)
         propagate_losig_delay(locon->SIG);
   }

   /*then output*/
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==IN) continue;
      propagate_losig_delay(locon->SIG);
   }
}


/***************************************************************************/
/* dispatch delay on losig modification                                   */
/***************************************************************************/
static void loc_propagate_losig_delay(loins_list *loins)
{
   locon_list* locon;

   if (!loins) {
      fprintf(stderr,"propagate_loins_delay: NULL pointer\n");
      exit(1);
   }
   
   for (locon=loins->LOCON; locon; locon=locon->NEXT) {
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
         locon->NAME,loins->INSNAME);
         exit(1);
      }
      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
      if (locon->DIRECTION==IN) continue;
      if (!locon->SIG) {fprintf(stdout,"\n+++++%s %s\n",loins->INSNAME,loins->FIGNAME);locon->SIG->NAMECHAIN=NULL;}
      propagate_losig_delay(locon->SIG); 
   }

}


/***************************************************************************/
/* propagate modification of a losig for delay                             */
/***************************************************************************/
extern void propagate_losig_delay(losig_list *losig)
{
   ptype_list* ptype;
   loins_list* loins;
   locon_list* locon;
   double delay, old_delay;
   char* signame;
   chain_list* lofigchain;

   if (!losig) {
      fprintf(stderr,"propagate_losig_delay: NULL pointer\n");
      exit(1);
   }
  
   
   if (!losig->NAMECHAIN) {
      fprintf(stderr,"propagate_losig_delay: no losig name\n");
      exit(1);
   }
   signame=losig->NAMECHAIN->DATA;
   delay=losig_delay(losig);
   
   if (losig->TYPE==EXTERNAL) {
      /*delay for output (memorize in special ident)*/
      putdelay(output_name(signame),delay);
      /*delay for input - T and R user  (return 0 for signame)*/
      delay=getdelaylax(signame)
           +getimpedancelax(signame)*getcapacitance(signame);
   }
   
   /*no use to propagate if unchanged*/
   old_delay=getdelay(signame);
   if (old_delay==delay) return;
   putdelay(signame,delay);

   /*propagate new delay on loins connected*/
   ptype=getptype(losig->USER,LOFIGCHAIN);
   if (!ptype) {
      fprintf(stderr,"propagate_losig_delay: no lofigchain on signal '%s'\n",
      signame);
      exit(1);
   }
   for (lofigchain=ptype->DATA; lofigchain; lofigchain=lofigchain->NEXT) {
      locon= (locon_list*) lofigchain->DATA;
      if (locon->DIRECTION==UNKNOWN) {
         fprintf(stderr,"BEH: 'linkage %s' isn't accepted\n",
         locon->NAME);
         exit(1);
      }
      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue; 
      if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE) continue;
      if (locon->TYPE==EXTERNAL) continue;
      loins=locon->ROOT;
      loc_propagate_losig_delay(loins);
   }   
   
}


/***************************************************************************/
/* set all capacitances of signals                                         */
/***************************************************************************/
extern void set_circuit_capacitance(lofig_list *lofig)
{
   loins_list* loins;
   losig_list* losig;
   char* signame;

   for (losig=lofig->LOSIG; losig; losig=losig->NEXT) {
      if (!losig->NAMECHAIN) {
         fprintf(stderr,"set_circuit_capacitance: no losig name\n");
         exit(1);
      }
      signame=losig->NAMECHAIN->DATA;
      /*for circuit output take user propertie if defined else return 0*/
      putcapacitance(signame,getcapacitancelax(signame));
      putdelay(signame,0);
   }
   
   for (loins=lofig->LOINS; loins; loins=loins->NEXT) {
      loins_capacitance(loins,1);
   }
}   
   
   
/***************************************************************************/
/*set all circuit delays with recursiv mode                                */
/***************************************************************************/
extern void set_circuit_delay(lofig_list* lofig)
{
   losig_list* losig;

   for (losig=lofig->LOSIG; losig; losig=losig->NEXT) {
      propagate_losig_delay(losig);
   }
}


