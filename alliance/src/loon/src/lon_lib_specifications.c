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
 * Tool    : LooN - cell properties
 * Date    : 2000
 * Author  : Francois Donnet
 */


#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include "lon_lib_specifications.h"



/*caracteristics of cell*/
#ifndef AREA
#define AREA "area"
#endif

#ifndef TRANSISTORS
#define TRANSISTORS "transistors"
#endif

/*for inputs*/
#ifndef Cin                        /*    cin_i */
#define Cin "cin"
#endif

/*for output*/
#ifndef Rup                   /*  rup_i_t  */
#define Rup "rup"
#endif

#ifndef Rdown                 /*  rdown_i_t  */
#define Rdown "rdown"
#endif

/*for inputs to output:   Tphl_i_t */
#ifndef Tphl
#define Tphl "tphl"
#endif

#ifndef Tpll
#define Tpll "tpll"
#endif

#ifndef Tplh
#define Tplh "tplh"
#endif

#ifndef Tphh
#define Tphh "tphh"
#endif


/*for flip-flop*/
#ifndef Tsr                  /*  tsr_i_ck	*/
#define Tsr "tsr"
#endif

#ifndef Tsf                /*  tsf_i_ck	*/
#define Tsf "tsf"
#endif

#ifndef Thr                /*    thr_i_ck	*/
#define Thr "thr"
#endif

#ifndef Thf                  /* thf_i_ck*/
#define Thf "thf"
#endif

#ifndef Tar             /* tar_ck_q */
#define Tar "tar"
#endif

#ifndef Taf             /* taf_ck_q  */
#define Taf "taf"
#endif



/******************************************************************************/
/* to avoid manip of pointer                                                  */
/******************************************************************************/
static int getvalue(begen_list* begen)
{
   int *pt_val;
   pt_val=begen->VALUE;
   return *pt_val;
}


/******************************************************************************/
/* to avoid manip of pointer                                                  */
/******************************************************************************/
static void* putvalue(int value)
{
   int *pt_val;
   pt_val=mbkalloc(sizeof(int));
   *pt_val=value;
   return pt_val;
}


/*****************************************************************************/
/*       return the value of a generic named name in befig                   */
/*****************************************************************************/
static int getgeneric(befig_list* befig, char* name)
{
   begen_list* begen;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==name) return getvalue(begen);
   }
   
   fprintf(stderr,"Library Error: generic '%s' not found in cell %s\n",
      name,befig->NAME);
   exit(1);
}


/******************************************************************************/
/*                      return the area cell in lambda² size                  */
/******************************************************************************/
extern int getgenericarea(befig_list* befig)
{
   return getgeneric(befig,namealloc(AREA)); 
}


/******************************************************************************/
/*                         set the area cell in lamda² size                   */
/******************************************************************************/
extern void putgenericarea(befig_list* befig, int value)
{
   befig->BEGEN=beh_addbegen(befig->BEGEN, namealloc(AREA), 
                             namealloc("natural"), putvalue(value));
}


/******************************************************************************/
/*             return the setup Time of an entry in pico second               */
/******************************************************************************/
extern double getgenericT(befig_list* befig, char* name)
{
   begen_list* begen;
   char* Thl, *Tll, *Tlh, *Thh, *tar, *taf;
   int T_setup=0;
   int count=0;
   char memo;
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   Thl=concatname(Tphl,name);
   Tll=concatname(Tpll,name);
   Thh=concatname(Tphh,name);
   Tlh=concatname(Tplh,name);
   tar=concatname(Tar,name);
   taf=concatname(Taf,name);
   Thl=concatname(Thl,output);
   Tll=concatname(Tll,output);
   Thh=concatname(Thh,output);
   Tlh=concatname(Tlh,output);
   tar=concatname(tar,output);
   taf=concatname(taf,output);
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==Thl || begen->NAME==Thh || begen->NAME==Tlh 
       || begen->NAME==Tll || begen->NAME==tar || begen->NAME==taf) {
         T_setup+=getvalue(begen); 
         count++;
      }
   }
   
   if (count==0) return 0;
   return (double) T_setup/count;
}


/******************************************************************************/
/*             return the setup in high value of an entry in pico second      */
/******************************************************************************/
extern double getgenericTh(befig_list* befig, char* name)
{
   begen_list* begen;
   char *Tlh, *Thh;
   int T_setup=0;
   int count=0;
   char memo;
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   Thh=concatname(Tphh,name);
   Tlh=concatname(Tplh,name);
   Thh=concatname(Thh,output);
   Tlh=concatname(Tlh,output);
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==Thh || begen->NAME==Tlh) {
         T_setup+=getvalue(begen); 
         count++;
      }
   }
   
   if (count==0) return 0;
   return (double) T_setup/count;
}


/******************************************************************************/
/*             return the setup in low value of an entry in pico second       */
/******************************************************************************/
extern double getgenericTl(befig_list* befig, char* name)
{
   begen_list* begen;
   char* Thl, *Tll;
   int T_setup=0;
   int count=0;
   char memo;
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   Thl=concatname(Tphl,name);
   Tll=concatname(Tpll,name);
   Thl=concatname(Thl,output);
   Tll=concatname(Tll,output);
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==Thl || begen->NAME==Tll) {
         T_setup+=getvalue(begen); 
         count++;
      }
   }
   
   if (count==0) return 0;
   return (double) T_setup/count;
}


/******************************************************************************/
/*                set the setup Times of an entry in pico second  (exp-12)    */
/******************************************************************************/
extern void putgenericT(befig_list* befig, char* name, double Thl_value, double Tll_value, double Tlh_value, double Thh_value)
{
   char* Thl, *Tll, *Tlh, *Thh;
   char memo;
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   Thl=concatname(Tphl,name);
   Tll=concatname(Tpll,name);
   Thh=concatname(Tphh,name);
   Tlh=concatname(Tplh,name);
   Thl=concatname(Thl,output);
   Tll=concatname(Tll,output);
   Thh=concatname(Thh,output);
   Tlh=concatname(Tlh,output);
   SEPAR=memo;

   befig->BEGEN=beh_addbegen(befig->BEGEN,Thl,namealloc("natural"),
               putvalue((int)Thl_value)); 
   befig->BEGEN=beh_addbegen(befig->BEGEN,Tll,namealloc("natural"),
               putvalue((int)Tll_value)); 
   befig->BEGEN=beh_addbegen(befig->BEGEN,Tlh,namealloc("natural"),
               putvalue((int)Tlh_value)); 
   befig->BEGEN=beh_addbegen(befig->BEGEN,Thh,namealloc("natural"),
               putvalue((int)Thh_value)); 
}


/******************************************************************************/
/*             return the Capacitance of an entry in pico Farad  (exp-12)     */
/******************************************************************************/
extern double getgenericC(befig_list* befig, char* name)
{
   begen_list* begen;
   char* C_name;
   char memo;   

   memo=SEPAR;
   SEPAR='_';
   C_name=concatname(Cin,name);   /*namealloc() hidden*/
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
     /* femto Farad (exp-15)  ->   pico Farad (exp-12) */
     if (begen->NAME==C_name) return (double)getvalue(begen)/1000;
   }

   return 0;
}


/******************************************************************************/
/*                set the Capacitance of an entry in pico Farad               */
/******************************************************************************/
extern void putgenericC(befig_list* befig, char* name, double C_value)
{
   char* C_name;
   char memo;   

   memo=SEPAR;
   SEPAR='_';
   C_name=concatname(Cin,name);   /*namealloc() hidden*/
   SEPAR=memo;

   /* pico Farad (exp-12)  ->   femto Farad (exp-15) */
   befig->BEGEN=beh_addbegen(befig->BEGEN,C_name,namealloc("natural"),
               putvalue((int)(C_value*1000))); 
}


/******************************************************************************/
/*return the average of Resistivity in Ohm of path from input to output cell  */
/******************************************************************************/
extern int getgenericR(befig_list* befig, char* name)
{
   begen_list* begen;
   char* R_up, *R_down;
   int R=0;
   int count=0;
   char memo;   
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   R_up=concatname(Rup,name);
   R_down=concatname(Rdown,name);
   R_up=concatname(R_up,output);
   R_down=concatname(R_down,output);
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==R_up || begen->NAME==R_down) {
         R+=getvalue(begen); 
         count++;
      }
   }
   
   if (count==0) return 0;
   return (double) R/count;
}


/******************************************************************************/
/*         set Resistivity in Ohm of path from input to output cell           */
/******************************************************************************/
extern void putgenericR(befig_list* befig, char* name, double Rup_value, double Rdown_value)
{
   char* R_up, *R_down;
   char memo;   
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   R_up=concatname(Rup,name);
   R_down=concatname(Rdown,name);
   R_up=concatname(R_up,output);
   R_down=concatname(R_down,output);
   SEPAR=memo;

   befig->BEGEN=beh_addbegen(befig->BEGEN,R_up,namealloc("natural"),
               putvalue((int)Rup_value)); 
   befig->BEGEN=beh_addbegen(befig->BEGEN,R_down,namealloc("natural"),
               putvalue((int)Rdown_value)); 
}


/******************************************************************************/
/*return the Rup in Ohm of path from input to output cell                     */
/******************************************************************************/
extern int getgenericRup(befig_list* befig, char* name)
{
   begen_list* begen;
   char* R_up;
   int R=0;
   int count=0;
   char memo;   
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   R_up=concatname(Rup,name);
   R_up=concatname(R_up,output);
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==R_up) {
         R+=getvalue(begen); 
         count++;
      }
   }
   
   if (count==0) return 0;
   return (double) R/count;
}


/******************************************************************************/
/*return the Rdown in Ohm of path from input to output cell                   */
/******************************************************************************/
extern int getgenericRdown(befig_list* befig, char* name)
{
   begen_list* begen;
   char* R_down;
   int R=0;
   int count=0;
   char memo;   
   char *output;

   if (befig->BEOUT) output=befig->BEOUT->NAME;
   else if (befig->BEBUS) output=befig->BEBUS->NAME;
   else {
      fprintf(stderr,"BEH: output is missing in %s\n",befig->NAME);
      exit(1);
   }

   memo=SEPAR;
   SEPAR='_';
   R_down=concatname(Rdown,name);
   R_down=concatname(R_down,output);
   SEPAR=memo;

   for (begen=befig->BEGEN; begen; begen=begen->NEXT) {
      if (begen->NAME==R_down) {
         R+=getvalue(begen); 
         count++;
      }
   }
   
   if (count==0) return 0;
   return (double) R/count;
}


/******************************************************************************/
/*         return the average of Capacitance of all inputs in pico Farad      */
/******************************************************************************/
extern int getaverageC(befig_list* befig)
{
   bepor_list* bepor;
   int C=0;
   int count=0;

   for (bepor=befig->BEPOR; bepor; bepor=bepor->NEXT) {
      /*only input*/
      if (bepor->DIRECTION!=IN && bepor->DIRECTION!=INOUT 
       && bepor->DIRECTION!=TRANSCV) continue;
      if (isvdd(bepor->NAME) || isvss(bepor->NAME)) continue;
      count++;
      C+=getgenericC(befig,bepor->NAME);    /* in fF */
   }

   if (count==0) return 0;
   return (double)C/count/1000;  /* femto Farad (exp-15) -> pico Farad (exp-12)*/
}



