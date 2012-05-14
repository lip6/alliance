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

/***************************************************************/
/* module   : array.c                                          */
/* date     : 28/07/92                                         */
/* auteurs  : RENAUD & VENOT & PITON                           */
/***************************************************************/

#ident "$Id: ARRAY.c,v 1.4 2012/05/14 14:20:14 alliance Exp $"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"


/* ###--------------------------------------------------------------### */
/* function     : ARRAY                                                 */
/* description  : Create a PAGRP structure and the referenced PAIOL	*/
/* called func. : Erreur, SplitIdent, KillEsp, CheckIdent, TraiterChaine*/
/*          CheckWidth, FindIdent, EmpileParam, CheckSeparate, 		*/
/*          CheckFormat, CheckType, DefileParam, DupPaiol, CreatePagrp	*/
/* ###--------------------------------------------------------------### */


void d_ARRAY (char *param, ...)
{
va_list  pa;
char     buffer [80];
char     buffer_anx [64];
char     name [64];
char     buffer_name [80];
char     blk;
char     base;
char     i_o;
char     flg;
short    dim;
int      tmp_ind_paiol;
struct   alpha_num decomp;


    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

    if (racine_paseq == NULL)
      Erreur ("ARRAY must declare output file's name first\n");

    if (GNP_TAB_PAIOL_CREE == GNP_ACTIF)
      Erreur ("ARRAY already in descriptive part\n");

    if (param == NULL) 
       Erreur ("ARRAY needs any parameters\n");

    va_start(pa, param);

    strcpy (buffer,param);
    KillEsp (buffer);

    if (*buffer == ':')
      Erreur ("ARRAY must have an element at least\n");
 
    sp_pile_param = 0;
    while ( *buffer != ':') 
    {
        strcpy     (buffer,param);
        SplitIdent (buffer,buffer_anx);
        CheckIdent (buffer);

        if (strlen (buffer_anx) != 0) 
        {
            TraiterChaine (buffer_anx, &decomp);
            CheckWidth (&decomp);
            if (strlen(decomp.alpha) ==0) 
                sprintf(buffer,"%s %s",buffer,decomp.gauche);
            else 
                Erreur ("ARRAY must separate each element of a vector\n");
        }
        else 
            if  ( FindIdent (buffer,hash_pagrp) != -1) 
                Erreur ("ARRAY yet declared identifier as a pagrp\n");
        EmpileParam(param);


        param = va_arg(pa, char *);
        if (param == NULL) 
           Erreur ("ARRAY needs more parameters\n");
        strcpy (buffer,param);
        KillEsp (buffer);
    }

    blk = CheckSeparate(buffer);

    dim = sp_pile_param;

    param = va_arg(pa, char *);
    if (param == NULL) 
      Erreur ("ARRAY needs more parameters\n");
    strcpy (buffer,param);
    base = CheckFormat (buffer);

    param     = va_arg(pa, char *);
    if (param == NULL) 
      Erreur ("ARRAY needs more parameters\n");
    strcpy (buffer,param);
    i_o = CheckType (buffer);

    param     = va_arg(pa, char *);
    if (param == NULL) 
      Erreur ("ARRAY needs more parameters\n");
    strcpy (buffer,param);
    flg = CheckOption (buffer);

    param = va_arg(pa, char *);
    if (param == NULL) 
      Erreur ("ARRAY needs group name\n");
    strcpy (name,param);
    if  ( FindIdent (name,hash_paiol) != -1) 
        Erreur ("ARRAY duplicate declared identifier\n");
    if  ( FindIdent (name,hash_pagrp) != -1) 
        Erreur ("ARRAY yet declared identifier as a pagrp\n");

    param = va_arg(pa, char *);
    if (param != NULL) 
        Erreur ("ARRAY NULL parameter expected\n");

    va_end (pa);

    sp_file_param = 0;

/* test si les parametres sont de meme mode 'O', 'R', 'S' ou non declares */
    while (sp_file_param < sp_pile_param) 
    {
        param = DefileParam();
        strcpy     (buffer,param);
        SplitIdent (buffer,buffer_anx);
        if (strlen (buffer_anx) != 0) 
        {
        sprintf (buffer, "%s %s", buffer, buffer_anx);
        }

        sprintf (buffer_name, "%s %d", name, GNP_PARAM_COURANT);

        switch (i_o) 
        {
          case 'I': case 'T' : 
          {
             if  ( FindIdent (buffer,hash_paiol) != -1) 
                 Erreur ("ARRAY duplicate declared identifier\n");
             DupPaiol (-1,base,buffer,buffer_name,i_o,blk,flg);
             break;
          }
          case 'O': case 'R': case 'S' : 
          {
             tmp_ind_paiol = FindIdent (buffer,hash_paiol);
             if  ( tmp_ind_paiol == -1)
                  DupPaiol (-1,base,buffer,buffer_name,i_o,blk,flg);
             else 
             {
               if (((struct paiol*)tab_ident[tmp_ind_paiol].pointeur)->MODE != i_o) 
                  Erreur ("ARRAY /port mode not matched\n");
               DupPaiol (tmp_ind_paiol,base,buffer,buffer_name,i_o,blk,flg);
               break;
             }
          }
        }
    }
    CreatePagrp (name,dim,-1,GNP_VECTEUR_VIRTUEL);
}
