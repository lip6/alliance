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

/*
 * Tool    : BooG Binding and optimizing on gates
 * Author  : Francois Donnet
 */



#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <mlu.h>
#include <abe.h>
#include <abv.h>
#include <aut.h>
#include <bdd.h>
#include "bog_lax_param.h"
#include "bog_normalize_DAG.h"
#include "bog_normalize_register.h"
#include "bog_normalize_DC.h"
#include "bog_normalize_power.h"
#include "bog_normalize_nameindex.h"
#include "bog_normalize_simplify.h"
#include "bog_normalize_ARITY.h"
#include "bog_signal_nameindex.h"
#include "bog_signal_delay.h"
#include "bog_map_abl.h"
#include "bog_map_befig.h"
#include "bog_map_delay.h"
#include "bog_unflatten_befig.h"
#include "bog_lib_reader.h"
#include "bog_lib_utils.h"
#include "bog_lib_cell.h"
#include "bog_lib_complete.h"
#include "bog_signal_utils.h"
#include "bog_signal_adapt.h"
#include "bog_xsch_driver.h"



#ifndef BOOG
#define BOOG "boog"
#endif

#define USAGE "\
usage: "BOOG" <input_file> <output_file> [<lax_file>]\n\
"

/*usage with options*/
#define OPT_USAGE USAGE "\
       "BOOG" -h\n\
       "BOOG" <input_file> [-o <output_file>] [-l <lax_file>] [-x <xsch_mode>] [-m <optim_mode>]\n\
       "BOOG" <input_file> -d <debug_file>\n\
"

#define HELP_USAGE OPT_USAGE "\n\
\t-h               Displays this help page.\n\
\t-m <optim_mode>  Optimization mode number. Area(0) to delay(4).\n\
\t-x <xsch_mode>   XSC color mode. Critical path(0) or delay gradient(1).\n\
\t   <input_file>  VBE input file name.\n\
\t-o <output_file> VST output file name.\n\
\t-l <lax_file>    LAX parameter file name. See lax(1) for more info.\n\
\t-d <debug_file>  VBE debug file name. Users aren't concerned.\n\
"

/*options*/
#define DEBUG_FILE 'd'
#define OUTPUT_FILE 'o'
#define LAX_FILE 'l'
#define HELP_MODE 'h'
#define OPTIM_MODE 'm'
#define XSCH_MODE 'x'

/*options variables*/
extern char *optarg;
static int verbose_mode=0, optim_mode=DEFAULT_OPTIM, xsch_mode=XSCH_CRITICAL_PATH;
static char *input_file=NULL, *output_file=NULL, *lax_file=NULL, *debug_file=NULL, *xsch_file=NULL;
/*alliance env*/
static char* TARGET_LIB;



/******************************************************************************/
/*              set variables to values of command line                       */
/******************************************************************************/
static void set_param(int argc, char* argv[])
{
   int i;

#if 0   /*incompatibilite entre Linux et Solaris */
   char opt;
   
   /*options*/
   while ((opt=getopt (argc, argv,"-d:l:o:m:x:h")) != EOF) {
      switch (opt) {
      case OPTIM_MODE: 
         if (strlen(optarg)==1 && isdigit((int) optarg[0])) {
            optim_mode=optarg[0]-'0';
            if (optim_mode<=OPTIM_DELAY4) break;
         }
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      case XSCH_MODE:
         if (strlen(optarg)==1 && isdigit((int) optarg[0])) {
            xsch_mode=optarg[0]-'0';
            if (xsch_mode<=XSCH_GRADIENT) break;
         }
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      case DEBUG_FILE: 
         if (!debug_file) {debug_file=optarg; break;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      case OUTPUT_FILE:
         if (!output_file) {output_file=optarg; break;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      case LAX_FILE: 
         if (!lax_file) {lax_file=optarg; break;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      case  1 : /*all the other parameters without options  (Linux only)*/
         if (!input_file || input_file==argv[i]) {input_file=optarg; break;}
         if (!output_file || output_file==argv[i]) {output_file=optarg; break;}
         if (!lax_file || lax_file==argv[i]) {lax_file=optarg; break;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      case HELP_MODE:
         fprintf(stdout,HELP_USAGE "\n");
         exit(0);
      case '?': default:  
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
   }

#endif
   
   /*verify for Sun and Solaris.  getopt() isn't the same than Linux_elf  */
   for (i=1; i<argc; i++) {
      if (!strcmp(argv[i],"-h")) {fprintf(stdout,HELP_USAGE "\n"); exit(0);}
      /*take option and argument*/
      if (!strcmp(argv[i],"-o")) {
         if (++i<argc) {output_file=argv[i]; continue;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
      if (!strcmp(argv[i],"-l")) {
         if (++i<argc) {lax_file=argv[i]; continue;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
      if (!strcmp(argv[i],"-d")) {
         if (++i<argc) {debug_file=argv[i]; continue;}
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
      if (!strcmp(argv[i],"-x")) {
         if (++i<argc && strlen(argv[i])==1 && isdigit((int) argv[i][0])) {
            xsch_mode=argv[i][0]-'0';
            if (xsch_mode<=XSCH_GRADIENT) continue;
         }
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
      if (!strcmp(argv[i],"-m")) {
         if (++i<argc && strlen(argv[i])==1 && isdigit((int) argv[i][0])) {
            optim_mode=argv[i][0]-'0';
            if (optim_mode<=OPTIM_DELAY4) continue;
         }
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
   }  
   
   /*after parameters with options take parameters without '-'  */
   for (i=1; i<argc; i++) {
      /*skip option*/
      if (!strcmp(argv[i],"-h")) continue;
      /*skip option and argument*/
      if (!strcmp(argv[i],"-x") || !strcmp(argv[i],"-o") || !strcmp(argv[i],"-d")
       || !strcmp(argv[i],"-l") || !strcmp(argv[i],"-m")) {i++; continue;}
      /*take file without flag option (order is important)*/
      if (!input_file || input_file==argv[i]) {input_file=argv[i]; continue;}
      if (!output_file || output_file==argv[i]) {output_file=argv[i]; continue;}
      if (!lax_file || lax_file==argv[i]) {lax_file=argv[i]; continue;}
      fprintf(stderr,OPT_USAGE "\n");
      exit(1);
   }


   /*coherence in options*/
   if (!input_file) {
      fprintf(stderr,OPT_USAGE "\n");
      exit(1);
   }
   if (debug_file && !strcmp(debug_file,input_file)) {
      fprintf(stderr,"A different <debug_file> is needed for option '-%c'\n",DEBUG_FILE);
      fprintf(stderr,OPT_USAGE "\n");
      exit(1);
   }

   /*take default values*/
   if (!output_file) output_file=input_file;
   xsch_file=output_file;
   verbose_mode=1;
}



/******************************************************************************/
/*                               main BOOG                                    */
/******************************************************************************/
extern int main (int argc, char* argv[])
{
   lofig_list *lofig;
   befig_list *befig;
   ptype_list *long_path=NULL;
   losig_list *Losig;
   char       *LosigName;
   
   /*init*/
   mbkenv();   /*mbk*/
   ablenv();   /*abl*/
   autenv();  /*hash table*/
   bddenv();  /*for vhdlloadbefig() */
   
   alliancebanner_with_authors("BooG", VERSION " [2003/01/09]", "Binding and Optimizing On Gates", 
                               "2000", ALLIANCE_VERSION, "François Donnet");
    
   /*alliance env: extension netlist file (vst or al)*/
   fprintf(stdout,"        MBK_VDD        : %s\n",VDD);
   fprintf(stdout,"        MBK_VSS        : %s\n",VSS);
   fprintf(stdout,"        MBK_IN_LO      : %s\n",IN_LO);
   fprintf(stdout,"        MBK_OUT_LO     : %s\n",OUT_LO);
   fprintf(stdout,"        MBK_WORK_LIB   : %s\n",WORK_LIB);
   /*if no library defined*/
   TARGET_LIB=mbkgetenv("MBK_TARGET_LIB");
   if (TARGET_LIB) 
   fprintf(stdout,"        MBK_TARGET_LIB : %s\n",TARGET_LIB);
   else {
      fprintf(stderr,
      "Environment Error: no 'MBK_TARGET_LIB' defined for cells directory\n");
      exit(1);
   }

   /*set options*/
   set_param(argc,argv);

   /*separ*/
   fprintf(stdout,"\n");
   
   /*read lax parameter first*/
   if (lax_file) {
      fprintf(stdout,"Reading parameter file '%s.lax'...\n",lax_file);
      parsefilelax(lax_file);
   }   
   else {
      fprintf(stdout,"Reading default parameter...\n");
      defaultlax(optim_mode);
   }
   switch (getoptimlax()) {
      case OPTIM_DELAY0: fprintf(stdout,"100%% area optimization\n"); break;
      case OPTIM_DELAY1: fprintf(stdout,"75%% area - 25%% delay optimization\n"); break;
      case OPTIM_DELAY2: fprintf(stdout,"50%% area - 50%% delay optimization\n"); break;
      case OPTIM_DELAY3: fprintf(stdout,"25%% area - 75%% delay optimization\n"); break;
      case OPTIM_DELAY4: fprintf(stdout,"100%% delay optimization\n"); break;
      default: fprintf(stderr,"LAX: Optimization mode greater than 4\n"); exit(1);
   }
   

   /*read vbe and keep internal signals in befig*/
   fprintf(stdout,"Reading file '%s.vbe'...\n",input_file);
   befig=vhdlloadbefig(NULL, input_file, 2/*BVL_KEEPAUX*/);
   if (befig->ERRFLG) {
      fprintf(stderr,"BEH: error %d unknown in '%s.vbe'\n",
              befig->ERRFLG, input_file);
      exit(1);
   }
 
 
   /*check coherence between LAX and befig*/
   if (lax_file) {
      fprintf(stdout,"Controlling file '%s.lax'...\n",lax_file);
      if (!coherencelax(befig)) exit(1);
   }   
   
   /*check file error not detected by parser*/
   fprintf(stdout,"Controlling file '%s.vbe'...\n",input_file);
   /*detect power and forbid Vdd and Vss in expression*/
   if (!detect_power(befig)) exit(1);
   /*remove don't care and 'z'   -simplify_expr() will be better-*/
   remove_DC(befig);
   /*simplify abl expression   -needed by format_register()- */
   simplify_expr(befig);
   /*remove stable             -needed by DAG_control()-  */
   if (!format_register(befig)) exit(1);
   /*control cycles in befig and erase unused bebux and beaux*/
   if (!DAG_control(befig)) exit(1);


   /*read cell library*/
   fprintf(stdout,"Reading lib '%s'...\n",TARGET_LIB);
   library_reader(TARGET_LIB);
   /*control cell library and complete if we can*/
   fprintf(stdout,"Controlling lib '%s'...\n",TARGET_LIB);
   control_lib();
   
   

   /*all work to map without any problem*/
   fprintf(stdout,"Preparing file '%s.vbe'...\n",input_file);
   change_radical(befig);/*to avoid conflict with names -we need control_lib()*/
   /*put arity operator*/
   put_arity(befig);
   
   /*choose now the place of big cell, and adapt polarity of abl*/
   /*eval C for each signal: it is a sum of all sortance cell capacitance*/
   fprintf(stdout,"Capacitances on file '%s.vbe'...\n",input_file);
   adapt_for_cell(befig);
   
   /*divide the arity of big operator with optimization in area or time*/
   fprintf(stdout,"Unflattening file '%s.vbe'...\n",input_file);
   unflatten_befig(befig);      
   
   /*remove some unused beaux procuced by unflattening*/
   if (!DAG_control(befig)) exit(1);

   /*mapping of cells*/
   fprintf(stdout,"Mapping file '%s.vbe'...\n",input_file);
   lofig=map_befig(befig,output_file);
   
   /*free memory of last treatment*/
   free_nameindex();



   /*saving file*/
   if (output_file) {
      fprintf(stdout,"Saving file '%s.%s'...\n",output_file,OUT_LO);
      savelofig(lofig);  /*change la lofigchain en mode .al*/
      lofigchain(lofig);

      /*fill hash table: some signals are added, put default delay*/
      for ( Losig = lofig->LOSIG; Losig; Losig = Losig->NEXT )
      {
         LosigName = (char*) Losig->NAMECHAIN->DATA;
         if ( !is_signal( LosigName ) ) putdelay( LosigName, 0 );
      }
   }
   
   if (verbose_mode) {
      ptype_list *loins_num, *ptype, *pred;
      char* max_input=NULL, *max_output=NULL;
      int all=0;
      long delay;
      bereg_list* bereg;

      /*eval delay*/
      fprintf(stdout,"Quick estimated critical path (no warranty)...");
      fflush(stdout);
      long_path=max_delay_path(befig,lofig);  /*sig which obtains the latest value*/
      if (long_path) {
         max_input=(char*) long_path->DATA;
         /*search last*/
         pred=NULL;
         for (ptype=long_path; ptype->NEXT; ptype=ptype->NEXT) pred=ptype;
         max_output=(char*) ptype->DATA;
         delay=ptype->TYPE;
         fprintf(stdout,"%ld ps from '%s' to '%s'\n", delay, max_input, max_output);
         /*remove last if register, because it doesn't belong to critical path*/
         for (bereg=befig->BEREG; bereg; bereg=bereg->NEXT) {
            if (bereg->NAME==max_output && pred) {
               pred->NEXT=NULL;
               freeptype(ptype);
            }
         }
      }   
      else fprintf(stdout,"no output!\n");
      
      /*eval area*/
      fprintf(stdout,"Quick estimated area (with over-cell routing)...%d lambda²\n",
              count_area());

      /*be more precise*/
      loins_num=count_instance(lofig);
      if (!loins_num) fprintf(stdout,"No instance...\n");
      else {
         fprintf(stdout,"Details...\n");
         for (ptype=loins_num; ptype; ptype=ptype->NEXT) {
            fprintf(stdout,"\t%s: %ld\n",(char*)ptype->DATA,ptype->TYPE);
            all+=ptype->TYPE;
         }
         fprintf(stdout,"\tTotal: %d\n",all);
      }
      freeptype(loins_num);
   }

   /*colors and weight informations for xsch alliance displayer*/
   if (xsch_file) {
      FILE* xsch_stream;
      
      switch (xsch_mode) {
      case XSCH_GRADIENT:
         fprintf(stdout,
         "Saving delay gradient in xsch color file '%s.xsc'...\n",xsch_file);
         break;
      case XSCH_CRITICAL_PATH: default: 
         fprintf(stdout,
         "Saving critical path in xsch color file '%s.xsc'...\n",xsch_file);
      }
      xsch_stream=mbkfopen(xsch_file,"xsc",WRITE_TEXT);
      if (!xsch_stream){
         fprintf(stderr,"Cannot save file %s.xsc\n",xsch_file);
         exit(1);
      }
      save_xsch(xsch_stream,lofig,long_path,xsch_mode);
      fclose(xsch_stream);
   }
   
   /*for debugging extract a resulting vbe file to compare with source*/
   if (debug_file) {
      fprintf(stdout,"Formating debug file '%s.vbe'...\n",debug_file);
      put_back_STABLE(befig);       /*put in condition register STABLE*/
      normalize_nameindex(befig);  /*for bits of vectors*/
      sort_vector(befig);  /*needed to be recognized by parser/driver */
      binary_oper(befig);  /*to accept NXOR, NOR, NAND */
      befig->NAME=debug_file; /* no namealloc() not to change in lower letter*/
      fprintf(stdout,"Saving debug file '%s.vbe'...\n",debug_file);
      vhdlsavebefig(befig,0);      
   }

   
   
   fprintf(stdout,"End of %s...\n\n",BOOG);
   exit(0);
}

