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
 * Tool    : LooN - Light optimizing on Nets 
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
#include "lon_lax_param.h"
#include "lon_signal_utils.h"
#include "lon_signal_netlist.h"
#include "lon_signal_critical.h"
#include "lon_optim_capa.h"
#include "lon_lib_utils.h"
#include "lon_lax_param.h"
#include "lon_xsch_driver.h"
#include "lon_lib_reader.h"
#include "lon_optim_stats.h"


#ifndef LOON
#define LOON "loon"
#endif

#define USAGE \
"usage: "LOON" <input_file> <output_file> [<lax_file>]\n"


/*usage with options*/
#define OPT_USAGE USAGE \
"       "LOON" -h\n"\
"       "LOON" -o <input_file>\n"\
"       "LOON" <input_file> <output_file> [-l <lax_file>] [-x <xsch_mode>] [-m <optim_mode>]\n"


#define HELP_USAGE OPT_USAGE "\n"\
"\t-h               Displays this help page.\n"\
"\t-o <input_file>  Overwrites <input_file> with new netlist.\n"\
"\t-m <optim_mode>  Optimization mode number. Area(0) to delay(4).\n"\
"\t-x <xsch_mode>   XSC color mode. Critical path(0) or delay gradient(1).\n"\
"\t   <input_file>  VST input file name.\n"\
"\t   <output_file> VST output file name.\n"\
"\t-l <lax_file>    LAX parameter file name. See lax(1) for more info.\n"



/*options*/
#define LAX_FILE 'l'
#define HELP_MODE 'h'
#define OPTIM_MODE 'm'
#define XSCH_MODE 'x'
#define OVERWRITE_MODE 'o'

/*options variables*/
extern char *optarg;
static int verbose_mode=0, optim_mode=DEFAULT_OPTIM, xsch_mode=XSCH_CRITICAL_PATH, overwrite_mode=0;
static char *input_file=NULL, *output_file=NULL, *lax_file=NULL, *xsch_file=NULL;
static char *mbk_target_lib=NULL, *mbk_in_lo=NULL, *mbk_out_lo=NULL;



/******************************************************************************/
/*              set variables to values of command line                       */
/******************************************************************************/
static void set_param(int argc, char* argv[])
{
   int i;

#if 0   /*incompatibilite entre Linux et Solaris */
   char opt;
   
   /*options*/
   while ((opt=getopt (argc, argv,"-l:m:x:ho")) != EOF) {
      switch (opt) {
      case OVERWRITE_MODE: overwrite_mode=1; break;
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
      if (!strcmp(argv[i],"-o")) {overwrite_mode=1; continue;}
      if (!strcmp(argv[i],"-h")) {fprintf(stdout,HELP_USAGE "\n"); exit(0);}
      /*take option and argument*/
      if (!strcmp(argv[i],"-l")) {
         if (++i<argc) {lax_file=argv[i]; continue;}
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
      if (!strcmp(argv[i],"-h") || !strcmp(argv[i],"-o")) continue;
      /*skip option and argument*/
      if (!strcmp(argv[i],"-x")  
       || !strcmp(argv[i],"-l") || !strcmp(argv[i],"-m")) {i++; continue;}
      /*take file without flag option (order is important)*/
      if (!input_file || input_file==argv[i]) {input_file=argv[i]; continue;}
      if (!output_file || output_file==argv[i]) {output_file=argv[i]; continue;}
      if (!lax_file || lax_file==argv[i]) {lax_file=argv[i]; continue;}
      fprintf(stderr,OPT_USAGE "\n");
      exit(1);
   }


   /*extension netlist file (vst or al)*/
   mbk_in_lo=mbkgetenv ("MBK_IN_LO");
   if (mbk_in_lo) fprintf(stdout,"\tMBK_IN_LO\t: %s\n",mbk_in_lo);
   else {
      fprintf(stderr,
     "Environment Error: no 'MBK_IN_LO' defined for <input_file> extension\n");
      exit(1);
   }
   mbk_out_lo=mbkgetenv ("MBK_OUT_LO");
   if (mbk_out_lo) fprintf(stdout,"\tMBK_OUT_LO\t: %s\n",mbk_out_lo);
   else {
      fprintf(stderr,
      "Environment Error: no 'MBK_OUT_LO' defined for <output_file> extension\n");
      exit(1);
   }

   /*if no library defined*/
   mbk_target_lib=mbkgetenv ("MBK_TARGET_LIB");
   if (mbk_target_lib) fprintf(stdout,"\tMBK_TARGET_LIB\t: %s\n",mbk_target_lib);
   else {
      fprintf(stderr,
      "Environment Error: no 'MBK_TARGET_LIB' defined for cells directory\n");
      exit(1);
   }


   /*coherence in options*/
   if (!input_file) {
      fprintf(stderr,OPT_USAGE "\n");
      exit(1);
   }
   if (!output_file) {
      if (overwrite_mode) output_file=input_file;
      else {
         fprintf(stderr,OPT_USAGE "\n");
         exit(1);
      }
   }
   
   if (!strcmp(output_file,input_file) && !strcmp(mbk_in_lo,mbk_out_lo) && !overwrite_mode) {
      fprintf(stderr,"<output_file> must be different from <input_file>\n");
      fprintf(stderr,OPT_USAGE "\n");
      exit(1);
   }
   
   /*take default values*/
   xsch_file=output_file;
   verbose_mode=1;
}



/******************************************************************************/
/*                               main LOON                                    */
/******************************************************************************/
extern int main (int argc, char* argv[])
{
   lofig_list *lofig;
   ptype_list *long_path=NULL; 
  
   /*init*/
   mbkenv();   /*mbk*/
   ablenv();   /*abl*/
   autenv();  /*hash table*/
   bddenv();  /*for vhdlloadbefig() in library reader*/
   

   alliancebanner_with_authors("LooN", VERSION " [2003/12/07]", "Local optimization on Nets", 
                               "2000", ALLIANCE_VERSION, "François Donnet");
    
   /*set options*/
   set_param(argc,argv);

   /*clear*/
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
   case OPTIM_DELAY1: 
      fprintf(stdout,"75%% area - 25%% delay optimization\n"); break;
   case OPTIM_DELAY2: 
      fprintf(stdout,"50%% area - 50%% delay optimization\n"); break;
   case OPTIM_DELAY3: 
      fprintf(stdout,"25%% area - 75%% delay optimization\n"); break;
   case OPTIM_DELAY4: fprintf(stdout,"100%% delay optimization\n"); break;
   default: fprintf(stderr,"LAX: Optimization mode greater than 4\n"); exit(1);
   }
   

   /*read vst*/
   fprintf(stdout,"Reading file '%s.%s'...\n",input_file,mbk_in_lo);
   lofig=getlofig(input_file, 'A'/*all the figure*/);
   if (!lofig) {
      fprintf(stderr,"Cannot find '%s.%s'\n",input_file,mbk_in_lo);
      exit(1);
   }
   /*flatten all instance at cells level*/
   rflattenlofig(lofig, YES, YES);
   
  
   /*check coherence between LAX and lofig*/
   if (lax_file) {
      fprintf(stdout,"Controlling file '%s.lax'...\n",lax_file);
      if (!coherencelax(lofig)) exit(1);
   }   
  
   /*read cell library*/
   fprintf(stdout,"Reading lib '%s'...\n",mbk_target_lib);
   library_reader(mbk_target_lib);
   
   
   /*set signal properties*/
   fprintf(stdout,"Capacitances on file '%s.%s'...\n",input_file,mbk_in_lo);
   if (!lofig->LOINS || !lofig->LOSIG) {
      fprintf(stderr,"Figure '%s' is empty!\n",lofig->NAME);
      exit(1);
   }

   /*prepare source*/
   lofigchain(lofig);
   set_circuit_capacitance(lofig);
   fprintf(stdout,"Delays on file '%s.%s'...",input_file,mbk_in_lo);
   fflush(stdout);
   set_circuit_delay(lofig);
   fprintf(stdout,"%ld ps\n",(long) critical_delay(lofig));
    
   /*eval area*/
   if (verbose_mode) {
      ptype_list* ptype, *ptype2, *old_percentage, *old_model;
      
      fprintf(stdout,"Area on file '%s.%s'...",input_file,mbk_in_lo);
      fflush(stdout);
      old_model=count_instance(lofig);
      old_percentage=percent_instance(old_model);
      fprintf(stdout,"%ld lamda² (with over-cell routing)\n",getarea(lofig));
      
      /*Details of use*/
      if (!old_model) fprintf(stdout,"No instance...\n");
      else {
         fprintf(stdout,"Details...\n");
         ptype2=old_percentage;
         for (ptype=old_model; ptype; ptype=ptype->NEXT) {
            fprintf(stdout,"\t%s: %ld (%ld%%)\n",(char*)ptype->DATA,
            ptype->TYPE,ptype2->TYPE);
            ptype2=ptype2->NEXT;
         }
         fprintf(stdout,"\tTotal: %ld\n",getnum_ins(lofig));
      }
      freeptype(old_model);
      freeptype(old_percentage);

      fprintf(stdout,"Worst RC on file '%s.%s'...",input_file,mbk_in_lo);
      fflush(stdout);
      fprintf(stdout,"%ld ps\n",(long) getmaxRC(lofig));
   }

   
   /*inserting buffer on critical path*/
   if (getoptimlax()>=OPTIM_DELAY2) {
      int buffers;
      fprintf(stdout,"Inserting buffers on critical path for file '%s.%s'...",
            output_file,mbk_out_lo);
      fflush(stdout);
      buffers=insert_buffer_critical_path(lofig,getoptimlax());
      if (buffers==0) fprintf(stdout,"None inserted\n");
      else if (buffers==1) fprintf(stdout,"%d buffer inserted -> %ld ps\n",
                                   buffers,(long) critical_delay(lofig));
      else fprintf(stdout,"%d buffers inserted -> %ld ps\n",
                   buffers,(long) critical_delay(lofig));
   }
   
   /*improve RC*/
   fprintf(stdout,"Improving RC on critical path for file '%s.%s'...",
            output_file,mbk_out_lo);
   fflush(stdout);
   improve_capa_critical_path(lofig,getoptimlax());
   fprintf(stdout,"%ld ps\n",(long) critical_delay(lofig));
   
   
   /*global optimization*/
   fprintf(stdout,"Improving all RC for file '%s.%s'...\n",
            output_file,mbk_out_lo);
   fprintf(stdout,"Worst RC on file '%s.%s'...",output_file,mbk_out_lo);
   fflush(stdout);
   fprintf(stdout,"%ld ps\n",(long) getmaxRC(lofig));
 
 
   if (verbose_mode) {
      ptype_list* ptype, *ptype2, *new_percentage, *new_model;
      losig_list* losig;
      locon_list* locon;
      chain_list* lofigchain;
      loins_list* loins;
      
      /*eval area*/
      fprintf(stdout,"Area on file '%s.%s'...",output_file,mbk_out_lo);
      fflush(stdout);
      new_model=count_instance(lofig);
      new_percentage=percent_instance(new_model);
      fprintf(stdout,"%ld lamda² (with over-cell routing)\n",getarea(lofig));
      
      /*Details of use*/
      if (!new_model) fprintf(stdout,"No instance...\n");
      else {
         fprintf(stdout,"Details...\n");
         ptype2=new_percentage;
         for (ptype=new_model; ptype; ptype=ptype->NEXT) {
            fprintf(stdout,"\t%s: %ld (%ld%%)\n",(char*)ptype->DATA,
            ptype->TYPE,ptype2->TYPE);
            ptype2=ptype2->NEXT;
         }
         fprintf(stdout,"\tTotal: %ld\n",getnum_ins(lofig));
      }
      freeptype(new_model);
      freeptype(new_percentage);

   
      /*get list of signal in critical path*/
      long_path=critical_path_without_clock(lofig);
      if (long_path) {
         losig=long_path->DATA;
         if (!losig->NAMECHAIN) {
            fprintf(stderr,"main: no name on signal\n");
            exit(1);
         }
         fprintf(stdout,"Critical path (no warranty)...%ld ps from '%s' to ",
         (long) critical_delay(lofig),(char*) losig->NAMECHAIN->DATA);
         fflush(stdout);
         /*seek last*/
         for (ptype=long_path; ptype->NEXT; ptype=ptype->NEXT) ;
         losig=(losig_list*) ptype->DATA;
         if (!losig->NAMECHAIN) {
            fprintf(stderr,"main: no name on signal\n");
            exit(1);
         }
         if (losig->TYPE==EXTERNAL) {
            fprintf(stdout,"'%s'\n",(char*) losig->NAMECHAIN->DATA);
         }
         else {
            /*it is a register entry, search name of the register*/
            ptype=getptype(losig->USER,LOFIGCHAIN);
            for (lofigchain=ptype->DATA; lofigchain;
               lofigchain=lofigchain->NEXT) {
               locon=lofigchain->DATA;
               if (locon->DIRECTION==IN) {
                  loins=locon->ROOT;
                  fprintf(stdout,"'%s'\n", loins->INSNAME);
                  break;
               }   
            }
         }
      }
   }
   
   /*saving file*/
   if (output_file) {
      fprintf(stdout,"Saving file '%s.%s'...\n",output_file,mbk_out_lo);
      lofig->NAME=output_file;
      savelofig(lofig);  /*change la lofigchain en mode .al*/
      lofigchain(lofig);
   }
   
   
   /*colors and weight informations for xsch alliance displayer*/
   if (xsch_file) {
      FILE* xsch_stream;
      ptype_list* ptype;
      char* signame;
      losig_list* losig;
      
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
      /*replace losig by names*/
      for (ptype=long_path; ptype; ptype=ptype->NEXT) {
         losig=(losig_list*) ptype->DATA;
         signame=losig->NAMECHAIN->DATA;
         ptype->DATA=signame;
      }
      save_xsch(xsch_stream,lofig,long_path,xsch_mode);
      fclose(xsch_stream);
   }
   
   
   
   fprintf(stdout,"End of %s...\n\n",LOON);
   exit(0);
}

