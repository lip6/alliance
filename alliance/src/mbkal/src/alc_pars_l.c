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
*                                                                              *
* Tool        : Parser al                                                     *
* Author(s)   : Gregoire AVOT                                                 *
* Updates     : June, 12th 1998                                               *
* $Log: alc_pars_l.c,v $
* Revision 1.2  2002/09/30 16:20:52  czo
* support/users
*
* Revision 1.1.1.1  2002/03/13 10:19:11  fred
* Importing MBKAL sources into the new CVS tree
*
*                                                                              *
*******************************************************************************/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <mut.h> 
#include <mlo.h> 
#include <rcn.h> 
#include <mlu.h> 

#ident "@(#)Parser alliance logic v6 27 March 1999 by Gregoire AVOT"

#define TAILLE_CHAINE BUFSIZE

extern void alcloadlofig6 (lofig_list*, char*, char, FILE*, int );

/************************************************************************
*                  alc_directiondecode()                                *
************************************************************************/
char alc_directiondecode(dir)
char * dir;
{
	switch (dir[2])
	{
		case 'O' :
			return 'B';
		case 'K' :
			return 'X';
		case 'I' :
			return 'Z';
		case 'A' :
			return 'T';
		default :
			return dir[0];
	}
}

/************************************************************************
*                  alcloadlofig()                                       *
************************************************************************/

void alcloadlofig(pfig,fname,mode)
lofig_list * pfig;
char * fname;
char mode;
{
	FILE *in;
	char ligne[50];
	char nomfic[TAILLE_CHAINE];
	long i;
	
	(void)sprintf(nomfic,"%s/%s.%s",WORK_LIB,fname,IN_LO);
	if ((in=mbkfopen(fname, IN_LO ,READ_TEXT)) == NULL)
	{
		(void)fprintf(stderr,"\n*** mbk error *** Unable to open file : %s\n",nomfic);
		EXIT(1);
	}
	if (TRACE_MODE == 'Y')
	{
		(void)printf("\n--- mbk --- parsing file : %s\n",nomfic);
	}
	(void)fgets(ligne,50,in);
	(void)sscanf(ligne,"V ALLIANCE : %ld", &i);
        if( i== 4 )
	        alcloadlofig6( pfig, fname, mode, in, 4 );
        else
	if( i == 5 )
	        alcloadlofig6( pfig, fname, mode, in, 5 );
        else
	if( i == 6 )
	        alcloadlofig6( pfig, fname, mode, in, 6 );
	else
	{
	      (void)fprintf(stderr,"\n*** mbk error *** unable to load file %s, too old version\n", fname);
	      EXIT(1);
	}
	if (fclose(in) != 0)
	{
		(void)fprintf(stderr,"\n*** mbk error *** can't close file : %s\n",nomfic);
		EXIT(1);
	}
}

