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
 * Tool    : BooG - library cells
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "bog_normalize_ARITY.h"
#include "bog_lib_utils.h"
#include "bog_lib_complete.h"
#include "bog_map_pattern.h"
#include "bog_lib_cell.h"



/*table of cells*/
static cell_list* LOGIC_CELLS=NULL;
static cell_list* REG_CELLS=NULL;
static cell_list* BUS_CELLS=NULL;

/*flag to signal the presence of tristate inverter in library*/
static int TRISTATE_INVERTER=0;



/******************************************************************************/
/*       return the list of flip-flop and latch cells                         */
/******************************************************************************/
extern cell_list* getcell_register_lib()
{
   return REG_CELLS;
}

/******************************************************************************/
/*       return the list of tristate cells                                    */
/******************************************************************************/
extern cell_list* getcell_tristate_lib()
{
   return BUS_CELLS;
}

/******************************************************************************/
/*       return the list of pure logical cells                                */
/******************************************************************************/
extern cell_list* getcell_logic_lib()
{
   return LOGIC_CELLS;
}


/******************************************************************************/
/*          set the list of register  cells                                   */
/******************************************************************************/
extern void setcell_register_lib __P ((cell_list* cells_lib))
{
   REG_CELLS=cells_lib;
}


/******************************************************************************/
/*           set the list of tristate cells                                   */
/******************************************************************************/
extern void setcell_tristate_lib __P ((cell_list* cells_lib))
{
   BUS_CELLS=cells_lib;
}


/******************************************************************************/
/*          set the list of pure logical cells                                */
/******************************************************************************/
extern void setcell_logic_lib __P ((cell_list* cells_lib))
{
   LOGIC_CELLS=cells_lib;
}


/******************************************************************************/
/*add a pure logic cell                                                       */
/*return the new cell if inserted in library else return NULL                 */
/******************************************************************************/
extern cell_list* addCell_logic(befig_list* befig, chain_list* abl)
{
   cell_list* cell, *list;

   cell=addCell(befig);
   
   /*ABL field isn't a real abl!!!*/
   cell->ABL=build_reference(befig,cell->PORT,NULL,abl);   

   put_arity_abl(cell->ABL);

   /*memorize and sort in global variable*/
   list=classCell(cell,LOGIC_CELLS);
   if (list) LOGIC_CELLS=list;

   return list?cell:NULL;
}


/******************************************************************************/
/*build a new bus cell in our library                                         */
/*return the new cell if inserted in library else return NULL                 */
/******************************************************************************/
extern cell_list* addCell_bus(befig_list* befig, biabl_list* biabl)
{
   cell_list* cell;
   cell_list* list;

   cell=addCell(befig);

   cell->BIABL=biabl;
   
   for ( ; biabl; biabl=biabl->NEXT) {
      /*VALABL,CNDABL fields aren't real abl!!!*/
      biabl->VALABL=build_reference(befig,cell->PORT,NULL,biabl->VALABL);
      biabl->CNDABL=build_reference(befig,cell->PORT,NULL,biabl->CNDABL);

      put_arity_abl(biabl->VALABL);
      put_arity_abl(biabl->CNDABL);

      /*mark if it is a tristate inverter with a simple condition*/
      if (!ABL_ATOM(biabl->VALABL) && ABL_OPER(biabl->VALABL)==ABL_NOT) {
         TRISTATE_INVERTER=1;
      }
   }
   
   /*memorize and sort in global variable*/
   list=classCell(cell,BUS_CELLS);
   if (list) BUS_CELLS=list;

   return list?cell:NULL;
}


/******************************************************************************/
/*build a new flip-flop or  latch cell in our library                         */
/*return the new cell if inserted in library else return NULL                 */
/******************************************************************************/
extern cell_list* addCell_register(befig_list* befig, biabl_list *biabl)
{
   cell_list* cell, *list;
   port_list* internal_reg_port=NULL;

   cell=addCell(befig);

   cell->BIABL=biabl;
   
   for ( ; biabl; biabl=biabl->NEXT) {
      /*create internal reference to register*/
      internal_reg_port=internal_reference(befig,internal_reg_port,biabl->VALABL);
      internal_reg_port=internal_reference(befig,internal_reg_port,biabl->CNDABL);   
      
      /*VALABL,CNDABL fields aren't real abl!!!*/
      biabl->VALABL=build_reference(befig,cell->PORT,internal_reg_port,biabl->VALABL);
      biabl->CNDABL=build_reference(befig,cell->PORT,internal_reg_port,biabl->CNDABL);

      put_arity_abl(biabl->VALABL);
      put_arity_abl(biabl->CNDABL);
   }
   
   /*memorize and sort in global variable*/
   list=classCell(cell,REG_CELLS);
   if (list) REG_CELLS=list;

   return list?cell:NULL;
}


/******************************************************************************/
/*              return 1 if a tristate inverter exists in library             */
/******************************************************************************/
extern int is_tristate_inverter_lib()
{
   return TRISTATE_INVERTER;
}



