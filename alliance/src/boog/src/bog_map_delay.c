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
 * Tool    : BooG - evaluate delay of an abl
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdio.h>
#include <mut.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include <mlo.h>
#include "bog_lax_param.h"
#include "bog_lib_utils.h"
#include "bog_signal_utils.h"
#include "bog_signal_nameindex.h"
#include "bog_map_prepare.h"
#include "bog_map_pattern.h"
#include "bog_map_delay.h"



/***************************************************************************/
/*eval delay of a node of an abl                                           */ 
/*  node of an abl (sortance is 1)                                         */
/***************************************************************************/
extern float loc_eval_delay(chain_list *abl, float C)
{
   float delay, latest=0;
   port_list* port, *top;
   cell_list* cell;
   int master;

   if (!abl) {
      fprintf(stderr,"loc_eval_delay: NULL pointer\n");
      exit(1);
   }

   /*constant '1' and '0' are treated as variables*/
   if (ABL_ATOM(abl)) return getdelay(ABL_ATOM_VALUE(abl));

   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern(abl);

   latest=0;
   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) continue;
      if (isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE:
         /*no delay for output*/
         break;
      case IN: case INOUT: case TRANSCV:
        if (!port->ABL) {
            fprintf(stderr,
            "loc_eval_delay: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
        }
        delay=loc_eval_delay(port->ABL, port->C);
        delay+=port->T + port->R*C;
        if (latest<delay) latest=delay;
        break;
      }
   }

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();
   
   return latest;
}


/***************************************************************************/
/*  return the delay of an abl                                             */
/* memorize for this name the delay and the impedance                      */
/***************************************************************************/
extern float eval_delay(char *name, chain_list *abl)
{
   float delay, latest=0;
   port_list* port, *top;
   cell_list* cell;
   int master;

   if (!abl) {
      fprintf(stderr,"eval_delay: NULL pointer\n");
      exit(1);
   }
   if (!name) {
      fprintf(stderr,"eval_delay: signal has no name\n");
      exit(1);
   }

   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern(abl);

   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME) || isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE:
         /*no delay for output*/
         break;
      case IN: case INOUT: case TRANSCV:
        if (!port->ABL) {
            fprintf(stderr,
            "eval_delay: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
        }
        delay=loc_eval_delay(port->ABL, port->C);
        delay+=port->T + port->R * getcapacitance(name);
        if (latest<delay) latest=delay;
        break;
      }
   }

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   putdelay(name,latest);
   return latest;
}


/******************************************************************************/
/*                            eval delay of a tristate                        */
/* memorize for this name the delay and the impedance                         */
/******************************************************************************/
extern float eval_bus(char* name, biabl_list* biabl)
{
   cell_list* cell;
   float delay, latest=0;
   port_list* port, *top;
   int master;
   biabl_list* biabl_aux;

   if (!name) {
      fprintf(stderr,"eval_bus: bus has no name\n");
      exit(1);
   }
   
   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern_bus(biabl);

   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME) || isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (!port->ABL) {
            fprintf(stderr,
            "eval_bus: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         delay=loc_eval_delay(port->ABL, port->C);
         delay+=port->T + port->R * getcapacitance(name);  
         if (latest<delay) latest=delay;
         break;
      }
   }

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      delay=eval_bus(name,biabl);
      if (latest<delay) latest=delay; 
   }
   
   /*take the latest of tristates on this bus*/
   delay=getdelay(name);
   if (latest<delay) latest=delay; 
   putdelay(name,latest);
   return latest;
}


/******************************************************************************/
/*                           eval a register delay                            */
/* delay evaluated from the setup command                                     */
/* memorize for this name the delay and the impedance                         */
/******************************************************************************/
extern float eval_reg(char* name, biabl_list* biabl)
{
   cell_list* cell;
   float delay, latest=0;
   port_list* port, *top;
   ptype_list* ptype;
   int master;
   biabl_list* biabl_aux;

   if (!biabl) {
      fprintf(stderr,"eval_reg: NULL pointer\n");
      exit(1);
   }
   
   if (!name) {
      fprintf(stderr,"eval_reg: reg has no name\n");
      exit(1);
   }
   
   /*first recursion?*/
   master=first_eval();
   
   /*search if flip-flop or latch*/
   for (biabl_aux=biabl; biabl_aux; biabl_aux=biabl_aux->NEXT) {
      ptype=getptype(biabl_aux->USER,ABL_STABLE);
      if (ptype) break;
   }
   
   if (ptype) cell=cell_prepare_reg(biabl);  /*no time evaluation*/
   else cell=cell_pattern_reg(biabl);   /*time evaluation*/

   /*cell->PORT contains the result of pattern matching*/
   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   
   /*delay calculated on ck setup for flip-flop*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) continue;
      if (isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (port->STABLE) { 
            if (!port->ABL) {
               fprintf(stderr,
               "eval_reg: compute error %s not really match on port %s\n",
               cell->NAME,port->NAME);
               exit(1);
            }
            delay=loc_eval_delay(port->ABL, port->C); 
            delay+=port->T + port->R * getcapacitance(name);
            if (latest<delay) latest=delay; 
     
            delport(top);
            /*free pre-evaluation mecanismus*/
            if (master) free_eval();

            putdelay(name,latest);
            return latest;
         }
      }
   }
   
   /*delay calculate as a tristate for latch */
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) continue;
      if (isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (!port->ABL) {
            fprintf(stderr,
            "eval_reg: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         delay=loc_eval_delay(port->ABL, port->C); 
         delay+=port->T + port->R * getcapacitance(name);
         if (latest<delay) latest=delay; 
         break;
      }
   }
   

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      delay=eval_reg(name,biabl);
      if (latest<delay) latest=delay; 
   }
   
   putdelay(name,latest);
   return latest;
}


/******************************************************************************/
/*eval the delay value of a biabl register                                    */
/******************************************************************************/
extern float eval_value_reg(biabl_list* biabl)
{
   cell_list* cell;
   float delay, latest=0;
   port_list* port, *top;
   int master;
   biabl_list* biabl_aux;

   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern_reg(biabl);

   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME) || isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (!port->STABLE) { 
            if (!port->ABL) {
               fprintf(stderr,
               "eval_value_reg: compute error %s not really match on port %s\n",
               cell->NAME,port->NAME);
               exit(1);
            }
            delay=loc_eval_delay(port->ABL, port->C); 
            if (latest<delay) latest=delay; 
         }
         break;
      }
   }

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      delay=eval_value_reg(biabl);
      if (latest<delay) latest=delay; 
   }
   
   return latest;
}


/******************************************************************************/
/*                           eval clock delay                                 */
/******************************************************************************/
extern float eval_clock_reg(biabl_list* biabl)
{
   cell_list* cell;
   float delay, latest=0;
   port_list* port, *top;
   ptype_list* ptype=NULL;
   int master;
   biabl_list* biabl_aux;

   /*first recursion?*/
   master=first_eval();
   
   /*search if flip-flop or latch*/
   for (biabl_aux=biabl; biabl_aux; biabl_aux=biabl_aux->NEXT) {
      ptype=getptype(biabl_aux->USER,ABL_STABLE);
      if (ptype) break;
   }
   
   if (ptype) cell=cell_prepare_reg(biabl);  /*no time evaluation*/
   else cell=cell_pattern_reg(biabl);   /*time evaluation*/

   /*cell->PORT contains the result of pattern matching*/
   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   
   /*delay calculated on ck setup for flip-flop*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) continue;
      if (isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (port->STABLE) { 
            if (!port->ABL) {
               fprintf(stderr,
               "eval_clock_reg: compute error %s not really match on port %s\n",
               cell->NAME,port->NAME);
               exit(1);
            }
            delay=loc_eval_delay(port->ABL, port->C); 
            if (latest<delay) latest=delay; 
            
            delport(top);
            /*free pre-evaluation mecanismus*/
            if (master) free_eval();

            return latest;
         }
         break;
      }
   }
   
   /*delay calculate as tristate for latch */
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME)) continue;
      if (isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (!port->ABL) {
            fprintf(stderr,
            "eval_clock_reg: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         delay=loc_eval_delay(port->ABL, port->C); 
         if (latest<delay) latest=delay; 
         
         break;
      }
   }

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      delay=eval_clock_reg(biabl);
      if (latest<delay) latest=delay; 
   }
   
   return latest;
}


/******************************************************************************/
/*eval the delay value of a biabl bus                                         */
/******************************************************************************/
extern float loc_eval_bus(biabl_list* biabl, float C)
{
   cell_list* cell;
   float delay, latest=0;
   port_list* port, *top;
   int master;
   biabl_list* biabl_aux;

   /*first recursion?*/
   master=first_eval();
   
   /*cell->PORT contains the result of pattern matching*/
   cell=cell_pattern_bus(biabl);

   top=copyport(cell->PORT);  /*not to be disturb by recursion*/
   for (port=top; port; port=port->NEXT) {
      if (isvss(port->NAME) || isvdd(port->NAME)) continue;
      switch (port->DIRECTION) {
      case OUT: case TRISTATE: 
         /*no delay for output and power supply*/
         break;
      case IN: case INOUT: case TRANSCV:
         if (!port->ABL) {
            fprintf(stderr,
            "eval_value_bus: compute error %s not really match on port %s\n",
            cell->NAME,port->NAME);
            exit(1);
         }
         delay=loc_eval_delay(port->ABL, port->C);
         delay+=port->T + port->R * C;  
         if (latest<delay) latest=delay; 
         break;
      }
   }

   delport(top);
   /*free pre-evaluation mecanismus*/
   if (master) free_eval();

   /*skip conditions matched by cell*/
   for (biabl_aux=cell->BIABL; biabl_aux; biabl_aux=biabl_aux->NEXT) 
      biabl=biabl->NEXT;
   /*look the other blocks*/
   if (biabl) {
      delay=loc_eval_bus(biabl,C);
      if (latest<delay) latest=delay; 
   }
   
   return latest;
}


