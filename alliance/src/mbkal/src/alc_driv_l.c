/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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

/*******************************************************************************
*                                                                              *
* Tool        : Driver al                                                     *
* Author(s)   : Gregoire AVOT and many others, ...                           *
* Updates     : June, 12th 1998                                               *
* $Log: alc_driv_l.c,v $
* Revision 1.1  2002/03/13 10:19:11  fred
* Initial revision
*                                                                        *
*******************************************************************************/

#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include <mut.h> 
#include <mlo.h> 
#include <rcn.h> 

#define TAILLE_CHAINE BUFSIZE

/************************************************************************
*                  get_date()                                           *
************************************************************************/

static void get_date(date)
char * date;
{
	time_t tim;
	struct tm *rest;
	
	(void)time(&tim);
	rest=localtime(&tim);
	(void)sprintf(date,"%02d/%02d/%04d",rest->tm_mday,rest->tm_mon+1,rest->tm_year+1900);
}

/************************************************************************
*                  alc_decodedirection()                                *
************************************************************************/

char * alc_decodedirection(dir)
char dir;
{
	switch(dir)
	{
		case IN :
			return "IN";
		case OUT :
			return "OUT";
		case INOUT :
			return "INOUT";
		case UNKNOWN :
			return "UNKNOWN";
		case TRISTATE :
			return "TRISTATE";
		case TRANSCV :
			return "TRANSCV";
		default :
			(void)fprintf(stderr,"\n*** mbk error *** savelofig (al) invalid connector direction : %c\n",dir);
			return "UNKNOWN";
	}
}

/************************************************************************
*                  alc_decodetype()                                     *
************************************************************************/

char * alc_decodetype(type)
char type;
{
	switch(type)
	{
		case INTERNAL :
			return "INTERNAL";
		case EXTERNAL :
			return "EXTERNAL";
		default :
			(void)fprintf(stderr,"\n*** mbk error *** savelofig (al) invalid connector/signal type : %c\n",type);
			return "INTERNAL";
	}
}

/************************************************************************
*                  rcn_wiretype()                                     *
************************************************************************/

char * rcn_wiretype(type)
long type;
{
	switch(type)
	{
		case RCN_WIRE_UNKNOW :
			return "X";
		case RCN_WIRE_POLY :
			return "PY";
		case RCN_WIRE_ALU1 :
			return "A1";
		case RCN_WIRE_ALU2 :
			return "A2";
		case RCN_WIRE_CONT_POLY :
			return "CY";
		case RCN_WIRE_CONT_DIF_N :
			return "CN";
		case RCN_WIRE_CONT_DIF_P :
			return "CP";
		case RCN_WIRE_CONT_VIA :
			return "CV";
		case RCN_WIRE_CONT_VIA2 :
			return "CW";
		case RCN_WIRE_CAPA :
			return "CA";
		case RCN_WIRE_RESI :
			return "RE";
		default :
			(void)fprintf(stderr,"\n*** rcn error *** savelofig (al) invalid node type : %ld\n",type);
			return "X";
	}
}

/************************************************************************
*                  alcsavelofig()                                       *
************************************************************************/
void alcsavelofig(pfig)
lofig_list * pfig;
{
	locon_list * pcon;
	lotrs_list * ptrs;
	loins_list * pins;
	losig_list * psig;
	lorcnet_list * ptrcnet;
	lowire_list * ptwire;
	chain_list * tmp;
	num_list * num;
	long index, indexg ,indexd, indexs, indexb;
	FILE *in;
	char nomfic[TAILLE_CHAINE];
	char nomp[TAILLE_CHAINE];
        num_list *tmpnum;
	loctc_list	*ptctc;
        chain_list      *scanchain;
        chain_list      *headctc;
	
	(void)sprintf(nomfic,"%s/%s.%s",WORK_LIB,pfig->NAME,OUT_LO);
	in=mbkfopen(pfig->NAME, OUT_LO, WRITE_TEXT);
	if(!in)
	{
		(void)fprintf(stderr,"\n*** mbk error *** savelofig (al) can't open file : %s\n",nomfic);
		EXIT(1);
	}
	if (TRACE_MODE == 'Y')
	{
		(void)printf("\n--- mbk --- writing the file : %s\n",nomfic);
	}
/*   reverse connectors, instances, etc.  */
        /* Pourquoi ???
	pfig->LOCON = (locon_list *)reverse((chain_list *)pfig->LOCON);
	pfig->LOINS = (loins_list *)reverse((chain_list *)pfig->LOINS);
	pfig->LOTRS = (lotrs_list *)reverse((chain_list *)pfig->LOTRS);
	for(pins = pfig->LOINS ; pins ; pins = pins->NEXT)
    	{
		pins->LOCON = (locon_list *)reverse((chain_list *)pins->LOCON);
    	}
        */
/* write header */
	(void)fprintf(in,"V ALLIANCE : 6\n");
	get_date(nomp);
	(void)fprintf(in,"H %s,L,%s\n",pfig->NAME,nomp);
/* write connectors of the figure */
	for (pcon = pfig->LOCON; pcon != NULL; pcon = pcon->NEXT)
	{
		(void)fprintf(in,"C %s,%s,%s,%ld",pcon->NAME,alc_decodedirection(pcon->DIRECTION),alc_decodetype(pcon->TYPE),pcon->SIG->INDEX);
                
		for( num = pcon->PNODE ; num ; num=num->NEXT)
                {
		  index = (long)num->DATA ;
		  (void)fprintf(in,",%ld",index);
		}
		(void)fprintf(in,"\n");
	}
	/* write transistors */
	for (ptrs = pfig->LOTRS; ptrs != NULL; ptrs = ptrs->NEXT)
	{
		(void)fprintf(in,"T %c,%g,%g,%ld,%ld,%ld,%ld,%g,%g,%g,%g,%g,%g", (ptrs->TYPE == TRANSN)?'N':'P', (float)((float)(ptrs->LENGTH)/(float)SCALE_X), (float)((float)(ptrs->WIDTH)/(float)SCALE_X), ptrs->DRAIN->SIG->INDEX, ptrs->GRID->SIG->INDEX, ptrs->SOURCE->SIG->INDEX,(ptrs->BULK->SIG?ptrs->BULK->SIG->INDEX:0), (float)((float)(ptrs->XS)/(float)SCALE_X), (float)((float)(ptrs->XD)/(float)SCALE_X), (float)((float)(ptrs->PS)/(float)SCALE_X), (float)((float)(ptrs->PD)/(float)SCALE_X), (float)((float)(ptrs->X)/(float)SCALE_X), (float)((float)(ptrs->Y)/(float)SCALE_X));
		if ( !(tmpnum = ptrs->DRAIN->PNODE) ) 
		   indexd = 0L;
		else
		   indexd = tmpnum->DATA ; 
		if ( !(tmpnum = ptrs->GRID->PNODE) ) 
		   indexg = 0L;
		else
		   indexg = tmpnum->DATA ; 
		if ( !(tmpnum = ptrs->SOURCE->PNODE) ) 
		   indexs = 0L;
		else
		   indexs = tmpnum->DATA ; 
		if ( !(tmpnum = ptrs->BULK->PNODE) ) 
		   indexb = 0L;
		else
		   indexb = tmpnum->DATA ; 

		if(indexd !=0L || indexg !=0L || indexs !=0L || indexb !=0l)
		  {
		    (void)fprintf(in,",%ld,%ld,%ld,%ld",indexd, indexg, indexs, indexb);
		  }

		(void)fprintf(in,",%s",ptrs->TRNAME?ptrs->TRNAME:"noname");
		(void)fprintf(in,"\n");
	}
/* write instances */
	for (pins = pfig->LOINS; pins != NULL; pins = pins->NEXT)
	{
		(void)fprintf(in,"I %s,%s\n",pins->FIGNAME,pins->INSNAME);
		for (pcon = pins->LOCON; pcon != NULL; pcon = pcon->NEXT)
		{
			(void)fprintf(in,"C %s,%s,%s,%ld",pcon->NAME,alc_decodedirection(pcon->DIRECTION),alc_decodetype(pcon->TYPE),pcon->SIG->INDEX);
			  for( num=pcon->PNODE ; num ; num = num->NEXT)
			  {
			    index = (long)((num_list *)num->DATA); 
		            (void)fprintf(in,",%ld",index);
		          }
		        (void)fprintf(in,"\n");
		}
	}
/* write signals */
	for (psig = pfig->LOSIG; psig != NULL; psig = psig->NEXT)
	{
		(void)fprintf(in,"S %ld,%s",psig->INDEX,alc_decodetype(psig->TYPE) );
		for (tmp = psig->NAMECHAIN; tmp; tmp = tmp->NEXT)
		{
			(void)fprintf(in,",%s",(char *)(tmp->DATA));
		}
		(void)fprintf(in,"\n");

	
                if( psig->PRCN )
                {
			ptrcnet = psig->PRCN;
			if( ptrcnet->CAPA >= 0.0 )
				(void)fprintf(in,"Q %g\n",ptrcnet->CAPA);


			for (ptwire = ptrcnet->PWIRE; ptwire != NULL ; ptwire = ptwire->NEXT)
				(void)fprintf(in,"W %ld,%ld,%s,%g,%g,%g,%g,%g,%g\n",ptwire->NODE1, ptwire->NODE2, rcn_wiretype(ptwire->LAYER), ptwire->RESI, ptwire->CAPA, (float)((float)(ptwire->X)/(float)SCALE_X), (float)((float)(ptwire->Y)/(float)SCALE_X), (float)((float)(ptwire->DX)/(float)SCALE_X), (float)((float)(ptwire->DY)/(float)SCALE_X));
		}
	}

        headctc = getallctc( pfig );
        for( scanchain = headctc ; scanchain ; scanchain = scanchain->NEXT )
        {
          ptctc = (loctc_list*)scanchain->DATA;
          (void)fprintf( in,"K %g,%ld,%ld,%ld,%ld\n", ptctc->CAPA, ptctc->SIG1->INDEX, ptctc->NODE1, ptctc->SIG2->INDEX, ptctc->NODE2 );
        }
        freechain( headctc );
        
	(void)fputs("EOF\n",in);
	if (fclose(in) != 0)
	{
		(void)fprintf(stderr,"\n*** mbk error *** can't close file : %s\n",nomfic);
		EXIT(1);
	}
	
/*   reverse back connectors, instances, etc. */
        /* iouqruoP ????
	pfig->LOCON = (locon_list *)reverse((chain_list *)pfig->LOCON);
	pfig->LOINS = (loins_list *)reverse((chain_list *)pfig->LOINS);
	pfig->LOTRS = (lotrs_list *)reverse((chain_list *)pfig->LOTRS);
	for(pins = pfig->LOINS ; pins ; pins = pins->NEXT)
	{
		pins->LOCON = (locon_list *)reverse((chain_list *)pins->LOCON);
	}
        */

}
