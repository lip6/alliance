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
/* module   : init.c							*/
/* date     : 07/09/93							*/
/* auteurs  : RENAUD & VENOT & PITON					*/
/************************************************************************/

#ident "$Id: INIT.c,v 1.2 2002/09/30 16:20:24 czo Exp $"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/* ###--------------------------------------------------------------### */
/* function     : INIT                                                  */
/* description  : add a PAINI list to the good PAPAT structure if it    */
/*            does exist, else create PAPAT on the way                  */
/* called func. : FormatValue,  CheckPattern, MajPatternCourant,	*/
/*	      SplitIdent,CheckIdent, checkWidth, atoi, pat_addpaini,	*/
/*	      CreatePapat, CreateAction					*/
/* ###--------------------------------------------------------------### */


void d_INIT (pat_numb, ident, val)

char *pat_numb;
char *ident;
char *val;

  {
  char              buffer      [  80];
  char              buffer_anx  [  80];
  char              tmp_valeur  [1024];
  char              value       [1024];
  int               dim, borne_g = -1, borne_d = -1, sens = 0, i, j;
  struct alpha_num  decomp;
  struct paini     *tmp_paini  = NULL;

    /* namealloc must be performed after mbkenv() - Czo - */
    if (!CATA_LIB || !WORK_LIB)
      mbkenv();

  if (racine_paseq == NULL)
    Erreur ("INIT must declare output file name first\n");

  if (GNP_TAB_PAIOL_CREE == GNP_INACTIF)
    {
    racine_paiol = (struct paiol *) reverse ((struct chain *)racine_paiol);
    racine_paiol = pat_crtpaiol (racine_paiol);
    GNP_TAB_PAIOL_CREE = GNP_ACTIF;
    }

  if (pat_numb == NULL)
    Erreur ("INIT unvalid parameter");

  strcpy (buffer, pat_numb);
  CheckPattern (buffer);
  MajPatternCourant (buffer);


  if (ident == NULL)
    Erreur ("INIT unvalid parameter");

  strcpy (buffer, ident);
  SplitIdent (buffer, buffer_anx);
  CheckIdent (buffer);

  if (strlen (buffer_anx) == 0)
    dim = 0;
  else
    {
    TraiterChaine (buffer_anx, &decomp);
    CheckWidth (&decomp);
    if (strlen (decomp.gauche) == 0)
      dim = 0;
    else
      {
      if (strlen (decomp.alpha) == 0)
        dim = 1;
      else
        {
        borne_g = atoi (decomp.gauche);
        borne_d = atoi (decomp.droite);
        if (borne_g > borne_d)
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
    }

  if (val == NULL)
    Erreur ("INIT unvalid parameter");
  strcpy (tmp_valeur, val);

  switch (dim)
    {
    case 0:
      {
      FormatValue (value,tmp_valeur,1,GNP_UP,0);
      tmp_paini = pat_addpaini (tmp_paini, buffer, *value);
      break;
      }
    case 1:
      {
      strcat (buffer," ");
      strcat (buffer,decomp.gauche);
      FormatValue (value,tmp_valeur,1,GNP_UP,0);
      tmp_paini = pat_addpaini (tmp_paini, buffer, *value);
      break;
      }
    default:
      {
      FormatValue (value,tmp_valeur,dim,sens,0);
      for (i=borne_g, j=0; i != borne_d; i = i + sens, j++)
        {
        sprintf (buffer_anx, "%s %d", buffer, i);
        tmp_paini = pat_addpaini (tmp_paini, buffer_anx, value[j]);
        }
      tmp_paini = (struct paini *) reverse ((struct chain *)tmp_paini);
      }
    }

  CreatePapat (NULL, NULL, tmp_paini);
  CreateAction ("I");
  }
