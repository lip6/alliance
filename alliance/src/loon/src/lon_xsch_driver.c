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
 * You should have received a copy  of the GNU General Public License aint
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Tool    : Loon and Boog - xsch color driver
 * Date    : 2000
 * Author  : Francois Donnet, Ludovic Jacomme(for xsch)
 */




#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>

#include <mut.h>
#include <abl.h>
#include <mlo.h>
#include <abe.h>
#include "lon_lib_utils.h"
#include "lon_signal_utils.h"
#include "lon_signal_netlist.h"
#include "lon_signal_name.h"
#include "lon_xsch_driver.h"

#define XSCH_COLOR_MAX 32
#define XSCH_RED_COLOR 32
#define XSCH_NEUTRAL_COLOR (-1)
#define XSCH_PATH "P"
#define XSCH_CON "C"
#define XSCH_BOX "B"
#define XSCH_NET "N"
#define XSCH_SEPAR ":"
#define XSCH_END "\n"




/*****************************************************************************/
/* return the color in proportion of delay                                   */
/*****************************************************************************/
static long gradient_color(double delay, double gradient)
{
   long color;
   color=(long) (delay/gradient);
   
   if (color>XSCH_COLOR_MAX) return XSCH_COLOR_MAX;
   else return color;
}


/**********************************************************************/
/*            dump into strem .xsc color format                       */
/**********************************************************************/
static void flush_loins(FILE* stream, char* name, long color, double delay)
{

   fprintf(stream,XSCH_BOX XSCH_SEPAR "%s" XSCH_SEPAR "%ld" XSCH_SEPAR "%d ps" XSCH_END ,
                       name, color, (int)delay);
   
}


/**********************************************************************/
/*            dump into strem .xsc color format                       */
/**********************************************************************/
static void flush_path(FILE* stream, char* source, char* signame, char* dest, long color, double delay)
{

   fprintf(stream, XSCH_PATH XSCH_SEPAR "%s" XSCH_SEPAR "%s" XSCH_SEPAR
                  "%s" XSCH_SEPAR "%ld" XSCH_SEPAR "%d ps" XSCH_END ,
                       source, signame, dest, color, (int)delay);
   
}


/**********************************************************************/
/*            dump into strem .xsc color format                       */
/**********************************************************************/
static void flush_losig(FILE* stream, char* name, long color, double delay)
{

   fprintf(stream,XSCH_NET XSCH_SEPAR "%s" XSCH_SEPAR "%ld" XSCH_SEPAR "%d ps" XSCH_END ,
                       name, color, (int)delay);
   
}


/***********************************************************************/
/*            dump into strem .xsc color format                        */
/***********************************************************************/
static void flush_stream_special(FILE* stream, char* name, long color, char* mes)
{
   fprintf(stream,XSCH_CON XSCH_SEPAR "%s" XSCH_SEPAR "%ld" XSCH_SEPAR "%s" XSCH_END 
                 , name, color, mes);
   
}


/**********************************************************************/
/* dump in xsch_file with format .xsc the weight of signals in delay  */
/* if signals belong to long_path a warning color is chosen           */
/*lofigchain is needed in lofig                                       */
/**********************************************************************/
extern void save_xsch(FILE* xsch_stream, lofig_list* lofig, ptype_list* long_path, int color_mode)
{
   long color;
   ptype_list* ptype, *ptype2=NULL;
   double delay=0, delay_out=0;
   char mes[1024];
   char* signame;
   locon_list* locon;
   losig_list* losig;
   double gradient=1;
   int count;
   loins_list* loins;
   char *source, *dest=NULL, *last_sig;
   chain_list* chain, *loinschain=NULL;
   chain_list* lofigchain;
   losig_list* losig_aux;
   
   /*build gradient*/
   if (long_path && color_mode==XSCH_GRADIENT) {
      /*get time of last entry*/
      count=1;
      for (ptype=long_path; ptype->NEXT; ptype=ptype->NEXT) count++;
      
      if (!ptype->DATA) {
         fprintf(stderr,"save_xsch: compute error\n");
         exit(1);
      }
      delay=ptype->TYPE;
      gradient=delay/((double)XSCH_COLOR_MAX);
   }
   
   /*color for signals*/
   for (losig=lofig->LOSIG; losig; losig=losig->NEXT) {

      if (!losig->NAMECHAIN || !losig->NAMECHAIN->DATA) {
         fprintf(stderr,"save_xsch: no name for signal\n");
         exit(1);
      }
      signame=losig->NAMECHAIN->DATA;
      
      if (isvdd(signame) || isvss(signame)) continue;
      
      if (losig->TYPE==EXTERNAL) {
         /*search external output*/
         ptype=getptype(losig->USER,LOFIGCHAIN);
         for (lofigchain=ptype->DATA; lofigchain; lofigchain=lofigchain->NEXT) {
            locon=lofigchain->DATA;
            if (locon->TYPE==EXTERNAL && locon->DIRECTION!=IN) break;
         }
         if (lofigchain) delay=getdelay(output_name(signame));
         else delay=getdelay(signame);
      }   
      else delay=getdelay(signame);
      
      switch (color_mode) {
      case XSCH_GRADIENT: color=gradient_color(delay,gradient); break;
      case XSCH_CRITICAL_PATH: default: color=XSCH_NEUTRAL_COLOR; break;
      }
      flush_losig(xsch_stream, signame, color, delay);
      
   }
   
   
   /*color for instances*/
   for (loins=lofig->LOINS; loins; loins=loins->NEXT) {
      /*search signal output*/
      for (locon=loins->LOCON; locon; locon=locon->NEXT) {
         if (locon->DIRECTION==UNKNOWN) {
            fprintf(stderr,"BEH: 'linkage %s' in figure '%s' isn't accepted\n",
            locon->NAME,loins->INSNAME);
            exit(1);
         }
         if (locon->DIRECTION==OUT || locon->DIRECTION==INOUT 
           || locon->DIRECTION==TRISTATE || locon->DIRECTION==TRANSCV) break;
      }
      if (!locon) {
         fprintf(stderr,"save_xsch: no output found for '%s'\n",loins->INSNAME);
         exit(1);
      }
      losig=locon->SIG;
      if (!losig->NAMECHAIN) {
        fprintf(stderr,"save_xsch: no name on signal\n");
        exit(1);
      }
      signame=losig->NAMECHAIN->DATA;
      
      delay=loins_delay(loins,signame);
     
      switch (color_mode) {
      case XSCH_GRADIENT: color=gradient_color(delay,gradient); break;
      case XSCH_CRITICAL_PATH: default:
         /*is instance in critical path?*/
         for (ptype=long_path; ptype; ptype=ptype->NEXT) {
            if ((char*)ptype->DATA!=signame) continue;
            /*if output and input signals belong to critical path,*/
            /*then instance belongs*/
            for (locon=loins->LOCON; locon; locon=locon->NEXT) {
               if (locon->DIRECTION==OUT || locon->DIRECTION==TRISTATE) continue;
               losig_aux=locon->SIG;
               if (!losig_aux->NAMECHAIN) {
                 fprintf(stderr,"save_xsch: no name on signal\n");
                 exit(1);
               }
               /*is signal in critical path?*/
               for (ptype2=long_path; ptype2; ptype2=ptype2->NEXT) {
                  if ((char*)ptype2->DATA==losig_aux->NAMECHAIN->DATA) break;
               }
               if (ptype2) break;
            }
            ptype=ptype2;  /*found?*/
            break;
         }
         
         /*build critical path list*/
         if (ptype) loinschain=addchain(loinschain,loins);
     
         if (ptype) color=XSCH_RED_COLOR;
         else color=XSCH_NEUTRAL_COLOR;
      }
      flush_loins(xsch_stream, loins->INSNAME, color, delay);
      
   }
   


   /*color for connectors*/
   for (locon=lofig->LOCON; locon; locon=locon->NEXT) {

      if (isvdd(locon->NAME) || isvss(locon->NAME)) continue;
      
      switch (locon->DIRECTION) {
      case IN: 
         delay_out=getdelay(locon->NAME);
         sprintf(mes, "%d ps",(int)delay_out);
         break;
      case OUT: case TRISTATE: 
         delay_out=getdelay(output_name(locon->NAME));
         sprintf(mes, "%d ps",(int)delay_out);
         break;
      case INOUT: case TRANSCV: 
         delay=getdelay(locon->NAME);
         delay_out=getdelay(output_name(locon->NAME));
         sprintf(mes, "%d ps on input; %d ps on output",(int)delay,(int)delay_out);
         break;
      }

      switch (color_mode) {
      case XSCH_GRADIENT: color=gradient_color(delay_out,gradient); break;
      case XSCH_CRITICAL_PATH: default:
         /* seek if signal is in a long path*/
         for (ptype=long_path; ptype; ptype=ptype->NEXT) {
            if ((char*)ptype->DATA==locon->NAME) break;
         }
   
         if (ptype) color=XSCH_RED_COLOR;
         else color=XSCH_NEUTRAL_COLOR;
      }
      flush_stream_special(xsch_stream, locon->NAME, color, mes);
   }  


   /*critical path*/
   if (loinschain && long_path && color_mode==XSCH_CRITICAL_PATH) {
      source=NULL;
      last_sig=NULL;
      for (ptype=long_path; ptype; ptype=ptype->NEXT) {
         signame=ptype->DATA;
         for (chain=loinschain; chain; chain=chain->NEXT) {
            loins=chain->DATA;
            /*search signal output*/
            for (locon=loins->LOCON; locon; locon=locon->NEXT) {
               if (locon->DIRECTION==OUT || locon->DIRECTION==INOUT 
                || locon->DIRECTION==TRISTATE || locon->DIRECTION==TRANSCV) {
                  losig=locon->SIG;
                  if (!losig->NAMECHAIN) {
                    fprintf(stderr,"save_xsch: no name on signal\n");
                    exit(1);
                  }
                  if (losig->NAMECHAIN->DATA==signame) {
                     dest=loins->INSNAME;
                     break;
                  }
               }   
            }
            if (locon) break;
         }  
         /*if no instance found it is external connector. it has the same name than signal*/
         if (!locon) dest=signame;
      
         if (source)
            flush_path(xsch_stream, source, last_sig, dest, XSCH_RED_COLOR, delay);
      
         source=dest;
         last_sig=signame;
      }
      
      /*for last signal search output*/
      if (losig) {
         ptype=getptype(losig->USER,LOFIGCHAIN);
         for (chain=ptype->DATA; chain; chain=chain->NEXT) {
            locon=chain->DATA;
            /*port of circuit*/
            if (locon->TYPE==EXTERNAL) {dest=locon->NAME; break;}
            loins=locon->ROOT;
            /*register input if no error*/
            if (locon->DIRECTION==IN || locon->DIRECTION==INOUT || locon->DIRECTION==TRANSCV)
            {dest=loins->INSNAME; break;}
         }
         flush_path(xsch_stream, source, signame, dest, XSCH_RED_COLOR, delay);
      }
      
   }

   freechain(loinschain);
}



