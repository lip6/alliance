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
 * Tool    : LooN - read all cells
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
#include "lon_normalize_register.h"
#include "lon_normalize_DAG.h"
#include "lon_normalize_simplify.h"
#include "lon_normalize_power.h"
#include "lon_lib_utils.h"
#include "lon_lib_negativ.h"
#include "lon_lib_format.h"
#include "lon_lib_permute.h"
#include "lon_lib_specifications.h"
#include "lon_lib_matching.h"
#include "lon_lib_reader.h"


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
  int filenamelength, extensionlength, pathlength;
  chain_list *head;

  if (!path || !ext) return NULL;

  dir = opendir (path);
  if (!dir) return NULL;

  extensionlength = strlen (ext);
  pathlength = strlen (path); 
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
   
   if (!vbe_list /*list of name*/) {
      fprintf(stderr,
              "Library Error: no cell in directory '%s'\n", 
              cell_directory?cell_directory:".");
      exit(1);
   }
   
   /*search only in our library not to interfer with others*/
   memo_work=WORK_LIB;
   memo_cata=CATA_LIB;
   WORK_LIB=cell_directory;  
   CATA_LIB=new_cata;
   new_cata[0]=cell_directory;
   new_cata[1]=NULL;

   for (chain=vbe_list; chain; chain=chain->NEXT) {

      /*put off extension*/
      name=chain->DATA;
      name[strlen(name)-strlen(".vbe")]='\0';
      
      /* mode: BVL_KEEPAUX , BVL_TRACE ... */
      /*do not keep aux*/
      befig=vhdlloadbefig(NULL,name,0/*don't dump message*/);
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

      /*simplify abl expression    */
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
      
      /*memorize cell if not already exists*/
      addCell(befig);
  
   }

   freechain(vbe_list);
   
   /*relax old env*/
   WORK_LIB=memo_work;
   CATA_LIB=memo_cata;
}

