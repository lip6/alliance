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

/************************************************************************/
/* module   : declar.c							*/
/* date     : 02/09/93							*/
/* auteurs  : RENAUD & VENOT & PITON					*/
/************************************************************************/

#ident "$Id: DECLAR.c,v 1.2 2002/09/30 16:20:24 czo Exp $"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/* ###--------------------------------------------------------------### */
/* function     : DECLAR						*/
/* description  : create PAIOL or PAGRP structs according to parameter	*/
/* called func. :  Erreur, CreatePaiol, CreatePagrp, KillEsp, atoi	*/
/*        DefileParam, CheckIdent, TraiterChaine, CheckWidth, FindIdent */
/*        EmpileParam, CheckSeparate, CheckFormat			*/
/* ###--------------------------------------------------------------### */


void d_DECLAR (ident,space,format,mode,size,option)
char *ident;
char *space;
char *format;
char *mode;
char *size;
char *option;
{
char     buffer [80];
char     name [80];
char     blk,base,i_o,flg;
short    dim=0;
int      i = 0;
int      borne_g = -1;
int      borne_d = -1;
int      sens = 0;
struct   alpha_num decomp;

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

    if (racine_paseq == NULL)
      Erreur ("DECLAR must declare output file's name first\n");

    if (GNP_TAB_PAIOL_CREE == GNP_ACTIF)
      Erreur ("DECLAR already in descriptive part\n");

    if (space == NULL)
      Erreur ("DECLAR unvalid parameter\n");
    strcpy (buffer,space);
    KillEsp (buffer);
    blk = CheckSeparate(buffer);

    if (format == NULL)
      Erreur ("DECLAR unvalid parameter\n");
    strcpy (buffer,format);
    base = CheckFormat (buffer);

    if (mode == NULL)
      Erreur ("DECLAR unvalid parameter\n");
    strcpy (buffer,mode);
    i_o = CheckType (buffer);

    if (option == NULL)
      Erreur ("DECLAR unvalid parameter\n");
    strcpy (buffer,option);
    flg = CheckOption (buffer);

    if (ident == NULL)
      Erreur ("DECLAR unvalid parameter\n");
    strcpy (name,ident);
    KillEsp (name);
    CheckIdent (name);

    if  ( FindIdent (name,hash_paiol) != -1) 
        Erreur ("DECLAR duplicate declared identifier\n");
    if  ( FindIdent (name,hash_pagrp) != -1) 
        Erreur ("DECLAR yet declared identifier as a pagrp\n");

    if (size == NULL)
      Erreur ("DECLAR unvalid parameter\n");
    strcpy (buffer,size);
    TraiterChaine(buffer,&decomp);
    CheckWidth (&decomp);
    if (strlen (decomp.gauche) == 0)
        dim = 0;
    else 
    {
           if (strlen(decomp.alpha) ==0)
                dim = 1;
           else 
           {
                borne_g = atoi (decomp.gauche);
                borne_d = atoi (decomp.droite);
                if ( borne_g > borne_d ) 
		{
                    borne_d--;
                    sens = -1;
                    dim = borne_g - borne_d;
                }
                else 
                {
                    borne_d++;
                    sens = 1;
                    dim = borne_d - borne_g;
                }
           }
    }
    switch (dim) 
    {
        case 0: 
        {
	    if ( base != 'B' )
		Erreur ("DECLAR illegal format for one bit identifier\n");
            CreatePaiol (base,name,i_o,blk,flg);
            break;
        }
        case 1: 
	{
	    char x[80];
            sprintf(x," %s",decomp.gauche);
	    strcat(name,x);
            if  ( FindIdent (name,hash_paiol) != -1) 
                Erreur ("DECLAR duplicate declared identifier\n");
	    if ( base != 'B' )
		Erreur ("DECLAR illegal format for one bit identifier\n");
            CreatePaiol (base,name,i_o,blk,flg);
            break;
        }
        default: 
	{
            for (i = borne_g; i != borne_d; i = i + sens) 
            {
		char x[80];
                sprintf (x," %d",  i);
		strcpy(buffer,name);
		strcat(buffer,x);
                if  ( FindIdent (buffer,hash_paiol) != -1) 
                    Erreur ("DECLAR duplicate declared identifier\n");
                CreatePaiol (base,buffer,i_o,blk,flg);
            }
        }
    }
    if (dim > 1)
        CreatePagrp (name,dim,sens,GNP_VECTEUR_EFFECTIF);
}
