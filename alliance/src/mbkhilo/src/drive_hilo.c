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
 
/*******************************************************************************
* mbk : hilo driver, mainly used as format for hardware test machine interface *
*       written by Alain Greiner in october 1990                               *
*                                                                              *
*       mbk vector support by Fred Petrot, june 1992                           *
*                                                                              *
*       signal names are are prefixed by 'hilosig_'     Olivier, September 94  *
*       instances names are are prefixed by 'hilog_'    Olivier, September 94  *
* version : 403                                                                *
* date    : 20 septembre 1994                                                  *
*******************************************************************************/

#include <mut.h> 
#include <mlo.h>
#include <string.h>
#include <ctype.h>
/*******************************************************************************
* function legal_instance_name : prefixe le nom de l'instance avec "hilo_"
* et remplace '[',']' et '.' avec `_`
*******************************************************************************/
static char *legal_instance_name(s)
char *s;
{
char buffer[255];
int i;

sprintf(buffer,"hilo_%s",s);

i = 0;
while (buffer[i] != '\0') {
	if ((buffer[i] == '[') || (buffer[i] == ']') || (buffer[i] == '.'))
		buffer[i] = '_' ;
	i++;
	}
return namealloc( buffer );
}

/*******************************************************************************
* function legal_signal_name : prefixe le nom du signal avec "hilosig_"
* et remplace '[',']',' ' et '.' avec `_`
*******************************************************************************/
static char *legal_signal_name(s)
char *s;
{
char buffer[255];
int i;

sprintf(buffer,"hilosig_%s",s);

i = 0;
while (buffer[i] != '\0') {
	if ((buffer[i] == '[') || (buffer[i] == ']') || 
            (buffer[i] == ' ') || (buffer[i] == '.'))
		buffer[i] = '_' ;
	i++;
	}
return namealloc(buffer);
}

/*******************************************************************************
* function busname : returns a legal hilo vector                               *
*******************************************************************************/
static char *busname(name)
char *name;
{
char buffer[255], *s, *t;
char one = 1;

	if (!name)
		return NULL;

	s = name;
	t = buffer;
	while (*s) {
		if (*s == ' ')
			if (one) {
				*t++ = '[';
				s++;
				one = 0;
			} else {
				*t++ = ']';
				*t++ = '[';
				s++;
			}
		*t++ = *s++;
	}
	if (!one)
		*t++ = ']';
	*t = '\0';
	return namealloc(buffer);
}

/*******************************************************************************
* function hilosavelofig                                                       *
*******************************************************************************/

void hilosavelofig(ptfig)
lofig_list *ptfig;
{
loins_list *ptins = NULL;
locon_list *ptcon = NULL;
locon_list *ptscan = NULL;
locon_list *pt1 = NULL;
locon_list *pt2 = NULL;
locon_list *pthead = NULL;
locon_list *ptsav = NULL;
losig_list *ptsig = NULL;
losig_list *ptsigdif = NULL;
lofig_list *ptnewfig = NULL;
FILE *ptfile;
char *amatla;
char *SigName = NULL;
char NetName[100];
char Buffer[256];

if ((ptfile = mbkfopen(ptfig->NAME, OUT_LO, WRITE_TEXT)) == NULL)
  {
  (void)fprintf(stderr, "*** mbk error ***\n");
  (void)fprintf(stderr, "cannot open file %s.%s\n", ptfig->NAME, OUT_LO);
  EXIT(1);
  }

/* check signal name unicity  */

for (ptsig = ptfig->LOSIG ; ptsig->NEXT ; ptsig = ptsig->NEXT)
  {
  amatla = getsigname(ptsig);
  for (ptsigdif = ptsig->NEXT ; ptsigdif ; ptsigdif = ptsigdif->NEXT)
    if(amatla == getsigname(ptsigdif))
      ptsig->NAMECHAIN = ptsigdif->NAMECHAIN = NULL;
  }

/* reverse external connectors & instances  */

ptfig->LOCON = (locon_list *)reverse((chain_list *)ptfig->LOCON);
ptfig->LOINS = (loins_list *)reverse((chain_list *)ptfig->LOINS);

/* restore connector order for instances */

for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT) 
  {
  pthead = NULL;
  ptnewfig = getlofig(ptins->FIGNAME, 'P');
  for (pt1 = ptnewfig->LOCON; pt1; pt1 = pt1->NEXT) 
    {
    for (pt2 = ptins->LOCON; pt2; pt2 = pt2->NEXT) 
      {
      if (pt2->NAME == pt1->NAME) break;
      ptsav = pt2;
      }
    if (pt2 == NULL)
      {
      (void)fprintf(stderr, "*** mbk error ***\n");
      (void)fprintf(stderr, "interface discrepancy between ");
      (void)fprintf(stderr, "instance %s and model %s",
				ptins->INSNAME, ptins->FIGNAME);
      (void)fprintf(stderr, " driving file %s.cct\n", ptfig->NAME);
      EXIT(1);
      }
    else if (pt2 == ptins->LOCON)
      {
      ptins->LOCON = pt2->NEXT;
      }
    else
      {
      ptsav->NEXT = pt2->NEXT;
      }
    pt2->NEXT = pthead;
    pthead = pt2;
    }
  if (ptins->LOCON != NULL)
    {
    (void)fprintf(stderr, "*** mbk error ***\ninterface discrepancy");
    (void)fprintf(stderr, "between instance %s and", ptins->INSNAME);
    (void)fprintf(stderr, " model %s", ptins->INSNAME, ptins->FIGNAME);
    (void)fprintf(stderr, " driving file %s.cct\n", ptfig->NAME);
    EXIT(1);
    }
  ptins->LOCON = pthead;
  }

(void)fprintf(ptfile, "CIRCUIT %s(\n", ptfig->NAME);

for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT) {
 if (isvdd(ptcon->NAME)) {
  (void)fprintf(ptfile, "   supply1 %s", busname(ptcon->NAME));
 }
 else 
  if (isvss(ptcon->NAME)) {
   (void)fprintf(ptfile, "   supply0 %s", busname(ptcon->NAME));
  }
  else {
  switch (ptcon->DIRECTION)
    {

	case UNKNOWN :
	case TRANSCV :
	case INOUT :
		(void)fprintf(ptfile, "   bidir %s", busname(ptcon->NAME));
	break;
	case IN :
		(void)fprintf(ptfile, "   input %s", busname(ptcon->NAME));
	break;
	case TRISTATE :
	case OUT :
		(void)fprintf(ptfile, "   output %s", busname(ptcon->NAME));
	break;
	default :
		(void)fprintf(stdout,"Unknown direction on terminal %s :\n",
		ptcon->NAME);
    }
   }
  if (ptcon->NEXT != NULL) (void)fputs(",\n", ptfile);
  }

(void)fputs(");\n", ptfile);

for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
  {
  strcpy(Buffer,legal_instance_name(ptins->INSNAME));
  fprintf(ptfile, "%s  %s(\n", ptins->FIGNAME, Buffer);
  for (ptcon = ptins->LOCON; ptcon; ptcon = ptcon->NEXT) 
    {
    ptsig = getlosig(ptfig, ptcon->SIG->INDEX);
    if (ptsig->TYPE == 'I')
      {
      SigName = getsigname(ptsig);
      fprintf(ptfile, "   %s", legal_signal_name(SigName));
      }
    else 
      {
      for (ptscan = ptfig->LOCON; ptscan; ptscan = ptscan->NEXT)
        {
        if (ptscan->SIG == ptsig) break;
        }
      if (ptscan == NULL) 
        {
        (void)fprintf(stderr, "*** mbk error ***\nno external connec");
        (void)fprintf(stderr, "tor driving file %s.cct\n", ptfig->NAME);
        }
      else
        {
        fprintf(ptfile, "   %s", busname(ptscan->NAME));
        }
      }
    if (ptcon->NEXT != NULL)
      {
      (void)fputs(",\n", ptfile);
      }
    }
  (void)fputs(");\n", ptfile);
  }
(void)fputs("ENDCIRCUIT\n", ptfile);
dellofig(ptfig->NAME);
(void)fclose(ptfile);
}
