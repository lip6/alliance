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

/***************************************************************/
/* module   : affect.c                                         */
/* date     : 28/07/92                                         */
/* auteurs  : RENAUD & VENOT & PITON                           */
/***************************************************************/

#ident "$Id: AFFECT.c,v 1.1 2002/05/30 13:28:15 xtof Exp $"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#include "mut.h"
#include "pat.h"
#include "phl.h"
#include "libpat_l.h"


/* ###--------------------------------------------------------------### */
/* function     : AFFECT                                                */
/* description  : set GNP_TAB_PAIOL_CREE to GNP_ACTIF if not done yet	*/ 
/*        create a PAEVT list according to value and connector  	*/
/*        Create or set the good PAPAT structure			*/
/* called func. : CheckPattern, pat_crtpaiol, Erreur, MajPatternCourant	*/
/*        SplitIdent, CheckIdent, TraiterChaine, CheckWidth, FindIdent  */
/*        FormatValue, ExisteIdent, CreatePaevt, CreatePapat		*/ 
/* ###--------------------------------------------------------------### */

void d_AFFECT (pat_numb,ident,val)
char *pat_numb;
char *ident;
char *val;
{
char    *valeur;
char     field_ident [64];
char	 buffer	     [80];
char     tmp_valeur  [1024];
char     value       [1024];
int      dim = 0,num_ident,sens,i,j;
struct alpha_num decomp;
struct paevt *tmp_paevt = NULL;

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

    if (racine_paseq == NULL)
      Erreur ("AFFECT must declare output file's name first\n");


    if ( GNP_TAB_PAIOL_CREE ==  GNP_INACTIF) 
    {
       racine_paiol = (struct paiol *) reverse ((struct chain *)racine_paiol);
       racine_paiol = pat_crtpaiol(racine_paiol);
       GNP_TAB_PAIOL_CREE = GNP_ACTIF;
    }

    if ( pat_numb == NULL) 
      Erreur ("AFFECT unvalid parameter");
    strcpy(buffer,pat_numb); 
    CheckPattern (buffer);
    MajPatternCourant(buffer);

    if ( ident == NULL) 
      Erreur ("AFFECT unvalid parameter");
    strcpy (buffer,ident);
    SplitIdent (buffer,field_ident);
    CheckIdent (buffer);
    if (strlen (field_ident) != 0) 
       {
          TraiterChaine (field_ident,&decomp );
          CheckWidth (&decomp);
          if (strlen (decomp.alpha) != 0) 
               Erreur ("AFFECT expected a group name or paiol name\n");
          if (strlen (decomp.gauche) != 0) 
             {
                strcat(buffer," ");
                strcat(buffer,decomp.gauche);
             }
       }

    if ( val == NULL) 
      Erreur ("AFFECT unvalid parameter");
    strcpy(tmp_valeur,val);

       num_ident = FindIdent (buffer,hash_paiol);
       if ( num_ident != -1) 
       {
         switch (
             ((struct paiol *)tab_ident[num_ident].pointeur)->MODE) 
         {
           case 'R': case 'S': case 'O': 
	   {
             valeur = tmp_valeur;
	     if (tmp_valeur[0] == '?')
		valeur++;
             FormatValue (value,valeur,1,GNP_UP,1);
             break;
           }
	   case 'T':
	   {
             valeur = tmp_valeur;
	     if (tmp_valeur[0] == '?')
	      {
		valeur++;
		FormatValue (value,valeur,1,GNP_UP,1);
	     }
	     else
		FormatValue (value,tmp_valeur,1,GNP_UP,0);
	     break;
	   }
           case 'I': 
           {
             FormatValue (value,tmp_valeur,1,GNP_UP,0);
             break;
           }
           default: 
               Erreur ("unconsistent internal structures\n");
         }
         tmp_paevt = CreatePaevt (tmp_paevt,num_ident,value[0]);
       }
       else 
       {
         num_ident = FindIdent (buffer,hash_pagrp);
         if  ( num_ident == -1) 
             Erreur ("AFFECT not yet declared identifier\n");
         else 
         {
           sens = ExisteIdent(buffer,hash_pagrp)-> sens;
           dim = ((struct pagrp *)tab_ident[num_ident].pointeur)->LENGTH;
           switch (
               ((struct paiol *)tab_ident[num_ident -1].pointeur)->MODE) 
           {
             case 'R': case 'S': case 'O': 
	     {
	       valeur = tmp_valeur;
	       if (tmp_valeur[0] == '?')
		valeur++;
               FormatValue (value,valeur,dim,sens,1);
               break;
             }
	     case 'T':
	     {
	       valeur = tmp_valeur;
	       if (tmp_valeur[0] == '?')
	       {
		   valeur++;
		   FormatValue (value,valeur,dim,sens,1);
	       }
	       else
		   FormatValue (value,tmp_valeur,dim,sens,0);
	       break;
	     }
             case 'I': 
             {
               FormatValue (value,tmp_valeur,dim,sens,0);
               break;
             }
             default: 
                 Erreur ("unconsistent internal structures\n");
           }
           for (j =0, i = dim; i > 0; i--, j++ )  
              tmp_paevt = CreatePaevt (tmp_paevt,num_ident -i,value[j]);
         }
	}
    CreatePapat (NULL,tmp_paevt,NULL);
}
