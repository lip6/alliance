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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * Tool    : BooG - read all cells
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <mut.h>
#include <abl.h>
#include <mlo.h>
#include <abe.h>
#include <mlu.h>
#include <abv.h>
#include "bog_normalize_register.h"
#include "bog_normalize_ARITY.h"
#include "bog_normalize_DAG.h"
#include "bog_normalize_simplify.h"
#include "bog_normalize_power.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_lib_negativ.h"
#include "bog_lib_format.h"
#include "bog_lib_permute.h"
#include "bog_lib_specifications.h"
#include "bog_lib_matching.h"
#include "bog_map_pattern.h"
#include "bog_lib_reader.h"



/***************************************************************************/
/*     return a list of files in directory path with the extension ext     */
/* if error in path then return NULL                                       */
/***************************************************************************/
extern chain_list *getfiles_with_ext (char *path, char *ext)
{
  DIR *dir;
  struct dirent *entry;
  char *filename;
  char *completefilename;
  int filenamelength, extensionlength;
  chain_list *head;

  if (!path || !ext) return NULL;

  dir = opendir (path);
  if (!dir) return NULL;

  extensionlength = strlen (ext);
  head=NULL;
  
  while ((entry = readdir (dir)) != NULL) {

      filename = entry->d_name;
      filenamelength = strlen (filename);
     
      if (filenamelength <= extensionlength) continue;
      /* is extension of filename accepted */
      if (strncmp (filename + filenamelength - extensionlength,
                  ext, extensionlength)) continue;

      completefilename = mbkalloc(filenamelength + 1);
      sprintf(completefilename, "%s", filename);
      head = addchain (head, completefilename);
  }

  return head;
}


/******************************************************************************/
/*   for each aspect of abl, build a cell                                     */
/* return 1 if included in library                                            */
/******************************************************************************/
static int distribCell(befig_list* befig)
{
   chain_list *permutations=NULL;
   chain_list *chain;
   cell_list *cells_lib, *cell, *pred;
   char *old_cell;
   biabl_list *biabl_befig, *biabl_cell, *biabl; 

   /*register*/
   if (befig->BEREG) {
   
      for (biabl_befig=befig->BEREG->BIABL; biabl_befig; biabl_befig=biabl_befig->NEXT) {
         if ( internal_reference(befig,NULL,biabl_befig->VALABL) ) return 0;
         if ( internal_reference(befig,NULL,biabl_befig->CNDABL) ) return 0;
         /*in negativ logic, propagate NOT*/
         biabl_befig->CNDABL=inv_oper(biabl_befig->CNDABL,0);
         biabl_befig->VALABL=inv_oper(biabl_befig->VALABL,0);
         /*minmize number of NOT*/
         biabl_befig->CNDABL=build_negativ(biabl_befig->CNDABL);
         biabl_befig->VALABL=build_negativ(biabl_befig->VALABL);
      }
      
      cells_lib=getcell_register_lib();
      
      /*check the use of inserting this cell*/ 
      for (cell=cells_lib; cell; cell=cell->NEXT) {
         
         /*new match a cell description?*/
         biabl_befig=befig->BEREG->BIABL;
         for (biabl_cell=cell->BIABL; biabl_cell; biabl_cell=biabl_cell->NEXT)
         {
            if (!biabl_befig
             || !pattern_matching(biabl_befig->CNDABL,biabl_cell->CNDABL)
             || !pattern_matching(biabl_befig->VALABL,biabl_cell->VALABL)) break;
            biabl_befig=biabl_befig->NEXT;
         }

         /*not the good cell to compare*/ 
         if (biabl_cell) continue;
         
         /* patterns are equal? */
         biabl_cell=cell->BIABL;
         for (biabl_befig=befig->BEREG->BIABL; biabl_befig; biabl_befig=biabl_befig->NEXT)
         {
            if (!biabl_cell
             || !pattern_matching(biabl_cell->CNDABL,biabl_befig->CNDABL)
             || !pattern_matching(biabl_cell->VALABL,biabl_befig->VALABL)) break;
            biabl_cell=biabl_cell->NEXT;
         }

         /*patterns aren't equal, new is more precise*/
         if (biabl_befig) continue;
         
         /*patterns are equal -->comparison*/
         if (cell->AREA<getgenericarea(befig)) return 0;  /*don't use the new*/
         if (cell->AREA==getgenericarea(befig)) break; /*insert new*/

         /*remove old cells because they are bigger*/
#if 0
         fprintf(stderr,"Mapping Warning: a smaller cell than '%s' exists\n",cell->NAME);
#endif         
         old_cell=cell->NAME;
         pred=NULL;
         for (cell=cells_lib ; cell; cell=cell->NEXT) {
            if (cell->NAME==old_cell && pred) pred->NEXT=cell->NEXT;
            else if (cell->NAME==old_cell) cells_lib=cell->NEXT;
            else pred=cell;
         }
         setcell_register_lib(cells_lib);
         
         break;   /*insert new*/
      }      
      
      /*give the list of different permutations*/
      permutations=different_biabl(befig,befig->BEREG->BIABL);
      
      /*put in cell*/
      for (chain=permutations; chain; chain=chain->NEXT) {
         biabl= (biabl_list*) chain->DATA;
         addCell_register(befig, biabl);
      }

   }


   /*bus*/
   else if (befig->BEBUS) {
   
      for (biabl_befig=befig->BEBUS->BIABL; biabl_befig; biabl_befig=biabl_befig->NEXT) {
         /*in negativ logic, propagate NOT*/
         biabl_befig->CNDABL=inv_oper(biabl_befig->CNDABL,0);
         biabl_befig->VALABL=inv_oper(biabl_befig->VALABL,0);
         /*minmize number of NOT*/
         biabl_befig->CNDABL=build_negativ(biabl_befig->CNDABL);
         biabl_befig->VALABL=build_negativ(biabl_befig->VALABL);
      }
      cells_lib=getcell_tristate_lib();
      
      /*check the use of inserting this cell*/ 
      for (cell=cells_lib; cell; cell=cell->NEXT) {
         
         /*new match a cell description?*/
         biabl_befig=befig->BEBUS->BIABL;
         for (biabl_cell=cell->BIABL; biabl_cell; biabl_cell=biabl_cell->NEXT)
         {
            if (!biabl_befig
             || !pattern_matching(biabl_befig->CNDABL,biabl_cell->CNDABL)
             || !pattern_matching(biabl_befig->VALABL,biabl_cell->VALABL)) break;
            biabl_befig=biabl_befig->NEXT;
         }

         /*not the good cell to compare*/ 
         if (biabl_cell) continue;
         
         /* patterns are equal? */
         biabl_cell=cell->BIABL;
         for (biabl_befig=befig->BEBUS->BIABL; biabl_befig; biabl_befig=biabl_befig->NEXT)
         {
            if (!biabl_cell
             || !pattern_matching(biabl_cell->CNDABL,biabl_befig->CNDABL)
             || !pattern_matching(biabl_cell->VALABL,biabl_befig->VALABL)) break;
            biabl_cell=biabl_cell->NEXT;
         }

         /*patterns aren't equal, new is more precise*/
         if (biabl_befig) continue;
         
         /*patterns are equal -->comparison*/
         if (cell->AREA<getgenericarea(befig)) return 0;  /*don't use the new*/
         if (cell->AREA==getgenericarea(befig)) break; /*insert new*/

         /*remove old cells because they are bigger*/
#if 0
         fprintf(stderr,"Mapping Warning: a smaller cell than '%s' exists\n",cell->NAME);
#endif         
         old_cell=cell->NAME;
         pred=NULL;
         for (cell=cells_lib ; cell; cell=cell->NEXT) {
            if (cell->NAME==old_cell && pred) pred->NEXT=cell->NEXT;
            else if (cell->NAME==old_cell) cells_lib=cell->NEXT;
            else pred=cell;
         }
         setcell_tristate_lib(cells_lib);
         
         break;   /*insert new*/
      }      
      
      
      /*give the list of different permutations*/
      permutations=different_biabl(befig,befig->BEBUS->BIABL);
      
      /*put in cell*/
      for (chain=permutations; chain; chain=chain->NEXT) {
         biabl= (biabl_list*) chain->DATA;
         addCell_bus(befig, biabl);
      }

   }
   
   /*logic*/
   else if (befig->BEOUT) {/*pure logical*/
   
      /*in negativ logic, propagate NOT*/
      befig->BEOUT->ABL=inv_oper(befig->BEOUT->ABL,0);
      /*minmize number of NOT*/
      befig->BEOUT->ABL=build_negativ(befig->BEOUT->ABL);
      cells_lib=getcell_logic_lib();
      
      /*check the use to insert this cell*/ 
      for (cell=cells_lib; cell; cell=cell->NEXT) {
         /*new more precise than old -> put new before*/
         if (pattern_matching(befig->BEOUT->ABL,cell->ABL)) {
            /*patterns are equal*/
            if (pattern_matching(cell->ABL,befig->BEOUT->ABL)) {
               if (cell->AREA<getgenericarea(befig)) return 0;
               if (cell->AREA==getgenericarea(befig)) break; /*insert new*/
               /*remove old cells because they are bigger*/
#if 0
               fprintf(stderr,"Mapping Warning: a smaller cell than '%s' exists\n",cell->NAME);
#endif         
               old_cell=cell->NAME;
               pred=NULL;
               for (cell=cells_lib ; cell; cell=cell->NEXT) {
                  if (cell->NAME==old_cell && pred) pred->NEXT=cell->NEXT;
                  else if (cell->NAME==old_cell) cells_lib=cell->NEXT;
                  else pred=cell;
               }
               setcell_logic_lib(cells_lib);
            }   
            /*insert new*/
            break;
         }   
      }      
      
      /*give the list of different permutations*/
      permutations=different_abl(befig,befig->BEOUT->ABL);
      for (chain=permutations; chain; chain=chain->NEXT) {
         /*several abl for a same behavior!!! ex: and(or ...) == nor(nor...)*/
         addCell_logic(befig, chain->DATA);
      }      
   }
   
   if (permutations) freechain(permutations);
   
   return 1;
}


/*****************************************************************************/
/*                   build the cell library                                  */
/*****************************************************************************/
extern void library_reader(char* cell_directory)
{
   char *name;
   chain_list* vbe_list, *chain;
   befig_list* befig;
   char* memo_work;
   char** memo_cata;
   char* new_cata[2];

   if (!cell_directory) {
      fprintf(stderr,"library_reader: no Cell directory\n");
      exit(1);
   }
   
   /*seek vbe files which describe the lib*/
   vbe_list=getfiles_with_ext (cell_directory, ".vbe");  
   
   /*search only in our library not to interfer with others*/
   memo_work=WORK_LIB;
   memo_cata=CATA_LIB;
   WORK_LIB=cell_directory;  
   CATA_LIB=new_cata;
   new_cata[0]=cell_directory;
   new_cata[1]=NULL;

   if (!vbe_list /*list of name*/) {
      fprintf(stderr,
              "Library Error: no cell in directory '%s'\n", 
              cell_directory?cell_directory:".");
      exit(1);
   }
   
   for (chain=vbe_list; chain; chain=chain->NEXT) {

      /*put off extension*/
      name=chain->DATA;
      name[strlen(name)-strlen(".vbe")]='\0';
      
      /* mode: BVL_KEEPAUX , BVL_TRACE ... */
      /*do not keep aux*/
      befig=vhdlloadbefig(NULL,name,'P');
      if (befig->ERRFLG) {
         fprintf(stderr,
                 "BEH: unknown error %d in %s.vbe\n",befig->ERRFLG,name);
         fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",name);
         continue;
      }

      /*detect power and forbid Vdd and Vss in expression*/
      if (!detect_power(befig)) {
         fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",befig->NAME);
         continue;
      }    

      /*simplify abl expression   -needed by format_register()- */
      simplify_expr(befig);

      /*remove stable             -needed by DAG_control()-  */
      if (!format_register(befig)) {
         fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",befig->NAME);
         continue;
      }    

      /*control cycles in befig and erase unused bebux and beaux*/
      if (!DAG_control(befig)) {
         fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",befig->NAME);
         continue;
      }    

      /*if constraints are respected*/
      if (!format_cell(befig)) {
         fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",befig->NAME);
         continue;
      }    
   
      /*put the locon of lofig in the same order than bepor of befig*/
      if (sortbepor(befig)) {
         fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",befig->NAME);
         continue;
      }    
      
      /*put arity oper*/
      put_arity(befig);
      
      /*memorize cell if not already exists*/
      if (!distribCell(befig)) {
        fprintf(stderr,"Mapping Warning: Cell '%s' isn't supported\n",befig->NAME);
      }
   
   }

   freechain(vbe_list);
   
   /*relax old env*/
   WORK_LIB=memo_work;
   CATA_LIB=memo_cata;
}

