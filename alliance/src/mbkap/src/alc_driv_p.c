/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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
 
/************************************************************************
*        Alliance VLSI CAD system                                       *
*                                                                       *
*   fichier : alc_driv_p.c                                              *
*   author(s)   : V. POUILLET           on : 09/07/92                   *
*   modified by : L. MAURIN\R. PREVOT   on : 12/07/93                   *
*   modified by : F. PETROT             on : 04/08/93                   *
*   version     : 4.00                                                  *
*   $Log: alc_driv_p.c,v $
*   Revision 1.2  2002/09/30 16:20:53  czo
*   support/users
*
*   Revision 1.1.1.1  2002/03/13 10:18:47  fred
*   Importing MBKAP sources into the new CVS tree
*                                                               *
************************************************************************/

#include <time.h>
#include <mut.h>
#include <mph.h>

#ident "@(#)Driver alliance physic v4.00 04/08/92" 

#define EOPEN        -4
#define ECLOSE       -5
#define ELAYER       -7
#define EOPGEO       -8
#define ETYPEVIA     -14
#define EFILENAME    -17
#define EREF         -23
#define MAXLBUFFER   64


#define  mc_isthrough(layer) \
        (   ((layer)==TPOLY) \
          ||((layer)==TALU1) \
          ||((layer)==TALU2) \
          ||((layer)==TALU3) \
          ||((layer)==TALU4) \
          ||((layer)==TALU5) \
          ||((layer)==TALU6) \
          ||((layer)==TALU7) \
          ||((layer)==TALU8) \
          ||((layer)==TALU9) )

static void alc_printwar(warn_code)
long  warn_code;
{
   fprintf( stderr, "\n*** mbk warning *** alcavephfig : ");
   switch( warn_code ) {
       case ELAYER    :
          fprintf( stderr, "layer out of range for alc\n"); break;
       case EOPGEO    :
       fprintf( stderr, "unknown transformation"); break;
       case EOPEN    :
       fprintf( stderr, "can\'t open tempory file"); break;
   case ETYPEVIA  :
       fprintf( stderr, "unknown via"); break;
      default : fprintf( stderr, "unknown warning");
      }
}


static void get_date(date)
char *date;
{
time_t tim;
struct tm *rest;

   (void)time(&tim);
   rest=localtime(&tim);
   (void)sprintf(date, "%2d/%2d/%2d", rest->tm_mday,
                 rest->tm_mon+1, rest->tm_year+1900);
}

char * alc_decodelayer(layer)
char layer;
{
static char *alc_decode[] = {
   "NWELL",
   "PWELL",
   "NTIE",
   "PTIE",
   "NDIF",
   "PDIF",
   "NTRANS",
   "PTRANS",
   "NTRANS_FAST",
   "PTRANS_FAST",
   "NTRANS_HVIO",
   "PTRANS_HVIO",
   "POLY",
   "POLY2",
   "ALU1",
   "ALU2",
   "ALU3",
   "ALU4",
   "ALU5",
   "ALU6",
   "ALU7",
   "ALU8",
   "ALU9",
   "TPOLY",
   "TPOLY2",
   "TALU1",
   "TALU2",
   "TALU3",
   "TALU4",
   "TALU5",
   "TALU6",
   "TALU7",
   "TALU8",
   "TALU9",
   "CALU1",
   "CALU2",
   "CALU3",
   "CALU4",
   "CALU5",
   "CALU6",
   "CALU7",
   "CALU8",
   "CALU9"
};
        
   if (layer < 0 || layer > LAST_LAYER)
      {
      alc_printwar(ELAYER);
      return "NULL";
      }
   return alc_decode[(int)layer];
}


/****************************** alc_decodevia() *******************************/
char * alc_decodevia(type)
int type;
{
static char *alc_decode[] = {
          "CONT_POLY",
          "CONT_DIF_N",
          "CONT_DIF_P",
          "CONT_BODY_N",
          "CONT_BODY_P",
          "CONT_VIA",
          "CONT_VIA2",
          "CONT_VIA3",
          "CONT_VIA4",
          "CONT_VIA5",
          "CONT_VIA6",
          "CONT_VIA7",
          "CONT_VIA8",
          "CONT_POLY2",
          "C_X_N",
          "C_X_P",
          "CONT_TURN1",
          "CONT_TURN2",
          "CONT_TURN3",
          "CONT_TURN4",
          "CONT_TURN5",
          "CONT_TURN6",
          "CONT_TURN7",
          "CONT_TURN8",
          "CONT_TURN9"
        };
   
   if (type < 0 || type > LAST_CONTACT) {
      alc_printwar(ETYPEVIA);
      EXIT(1);
   }
   return alc_decode[type];
}


char *alc_decoderef(figname)
char *figname;
{
#define MAXREF 2
static char *alc_decode[MAXREF] = { 
      "ref_ref", 
      "ref_con", 
};
static char *alc_return[MAXREF] = { 
      "REF_REF", 
      "REF_CON", 
};
int i;

   for(i = 0; i < MAXREF; i++)
      if (strcmp(figname, alc_decode[i]) == 0)
         return alc_return[i];

   return NULL;
}


char *alc_decodetransf(transf)
char transf;
{
static char *alc_decode[8] = { "NOSYM", "ROT_P", "SYMXY", 
           "ROT_M", "SYM_X", "SY_RM", "SYM_Y", "SY_RP" };

   if (transf < 0 || transf > 7) {
      alc_printwar(EOPGEO);
      return "NOSYM";
   }

   return alc_decode[(int)transf];
}

static void  alc_printerr2(err_code, word)
long  err_code;
char *word;
{
   fprintf( stderr, "\n*** mbk error *** alcsavephfig : ");
   switch( err_code )
    {   case EFILENAME :
       fprintf( stderr, "bad file name : %s .\n", word); break;
   case EOPEN     :
       fprintf( stderr, "can\'t open file : %s .\n", word); break;
   case EREF    :
       fprintf( stderr, " inconsistant reference :%s.\n", word); break;
   case ECLOSE    :
       fprintf( stderr, "can\'t close file : %s.\n", word); break;
   default : fprintf( stderr, "unknown error");
    }
    EXIT(1);
}



/******************************************************************************/
/******************************** alcsavephfig() ******************************/
/******************************************************************************/

void alcsavephfig(pfig)
phfig_list *pfig;
{
phcon_list *pcon;
phseg_list *pseg;
phref_list *pref;
phins_list *pins;
phvia_list *pvia;
FILE *f;
char nomp[MAXLBUFFER];

   if(!(f = mbkfopen(pfig->NAME, OUT_PH, WRITE_TEXT)))
      alc_printerr2(EOPEN, filepath(pfig->NAME, OUT_PH));

   if (TRACE_MODE == 'Y')
      (void) fprintf(stderr, "\n---mbk--- writing the file : %s\n",
							filepath(pfig->NAME, OUT_PH));

   get_date(nomp);
   (void)fprintf(f, "V ALLIANCE : 6\n");
   (void)fprintf(f, "H %s,P,%s,%ld\n", pfig->NAME, nomp,SCALE_X);

   (void)fprintf(f, "A %ld,%ld,%ld,%ld\n", pfig->XAB1, pfig->YAB1, pfig->XAB2, pfig->YAB2);

   for(pcon = pfig->PHCON; pcon; pcon = pcon->NEXT) {
      (void)fprintf(f, "C %ld,%ld,%ld,%s,%ld,",
                        pcon->XCON, pcon->YCON,
                        pcon->WIDTH, pcon->NAME, pcon->INDEX);
      switch(pcon->ORIENT) {
         case NORTH :
            (void)fprintf(f, "NORTH");
            break;
         case SOUTH :
            (void)fprintf(f, "SOUTH");
            break;
         case EAST :
            (void)fprintf(f, "EAST");
            break;
         case WEST :
            (void)fprintf(f, "WEST");
            break;
      }
      (void)fprintf(f, ",%s\n", alc_decodelayer(pcon->LAYER));
   }

   for(pref = pfig->PHREF; pref; pref = pref->NEXT) {
      if (!alc_decoderef(pref->FIGNAME))
          alc_printerr2(EREF, pref->FIGNAME);
      (void)fprintf(f, "R %ld,%ld,%s,%s\n",
                        pref->XREF, pref->YREF,
                        pref->FIGNAME, pref->NAME);
   }

   for(pseg = pfig->PHSEG; pseg; pseg = pseg->NEXT) {
      (void)fprintf(f, "S %ld,%ld,%ld,%ld,%ld,%s,",
                     pseg->X1, pseg->Y1,
                        pseg->X2, pseg->Y2,
                        pseg->WIDTH,
                        pseg->NAME == NULL ? "*" : pseg->NAME);
      switch(pseg->TYPE) {
         case LEFT :
            (void)fprintf(f, "LEFT");
            break;
         case UP :
            (void)fprintf(f, "UP");
            break;
         case RIGHT :
            (void)fprintf(f, "RIGHT");
            break;
         case DOWN :
            (void)fprintf(f, "DOWN");
            break;
         default :
            (void)fprintf(f, "UNKNOWN");
      }
      (void)fprintf(f, ",%s\n", alc_decodelayer(pseg->LAYER));
   }

   for(pins = pfig->PHINS; pins; pins = pins->NEXT)
      (void)fprintf(f, "I %ld,%ld,%s,%s,%s\n",
                        pins->XINS, pins->YINS,
                        pins->FIGNAME, pins->INSNAME,
                        alc_decodetransf(pins->TRANSF));

   for(pvia = pfig->PHVIA; pvia; pvia = pvia->NEXT)
   {
     /* MODIF Ludo 2/4/00 */
     if ((pvia->DX == 0) && 
         (pvia->DY == 0))
     {
      (void)fprintf(f, "V %ld,%ld,%s,%s\n",
                        pvia->XVIA, pvia->YVIA, alc_decodevia(pvia->TYPE),
                        pvia->NAME == NULL ? "*" : pvia->NAME ); 
     }
     else
     {
      (void)fprintf(f, "B %ld,%ld,%ld,%ld,%s,%s\n",
                        pvia->XVIA, pvia->YVIA, pvia->DX, pvia->DY,
                        alc_decodevia(pvia->TYPE),
                        pvia->NAME == NULL ? "*" : pvia->NAME ); 
     }
   }

   (void)fputs("EOF\n", f);

   if (fclose(f) != 0)
      alc_printerr2(ECLOSE, filepath(pfig->NAME, OUT_PH));
}
