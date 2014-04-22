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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/* 
 * Purpose : verilog driver, mainly used to enable simulating with Cadence Verilog.
 * Version : 002
 * Date    : Jul 1995
 * Author  : Aldo Mozzi <aldo.mozzi@libero.it>
 * Modified by Mourad Aberbour in Sept. 1997 
 */

#ident "$Id: drive_vlog.c,v 1.3 2012/05/14 14:20:26 alliance Exp $"

#include <mut.h> 
#include <mlo.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

extern char *mvl_vhdlname(); /*see ../mbkvhdl/mbkvhdl/mvl_drive.h*/

static char name_buffer[255];
static char vec_name_buffer[255];

/******************************************************************
* function vlg_name: gets a name and returns a legal verilog name *
* in name_buffer. Characteres not allowed in a valid verilog name *
* are replaced by "_"<ascii code in hex>"_".                      *
* If the name is indexed the indexing syntax is translated:       *
*     <name><space><number> --> <name>[<number>]                  *
******************************************************************/
char *vlg_name (name)
char *name;
{
	char *scan_new;
	int i, indexed;
	static char hextable[] = "0123456789ABCDEF";
#ifdef mark_in_out
printf ("entering vlg_name %s\n", name);
#endif
	scan_new = name_buffer;
	i = 0;
	indexed = 0;
	while (*name) 
	{
		if (i != 0 && *name == ' ') 
		{
			*scan_new++ = '[';
			indexed = 1;
			name++;
		}
		else 
		{
            if (isalpha(*name) || *name == '_' || (((i != 0) && (isdigit(*name))) || *name == '$')) 
			{
				*scan_new = *name++;
		/* modif mourad : le 03/12/97
			Le driver verilog pour les netlists alliance doit garder les noms de models et des coneecteurs
			en minuscule pour une compatibilite avec les cellules ScLib et PadLib exportees vers Cadence.
			Les noms de signaux et d'instances peuvent rester tel quel.

				if (islower(*scan_new)) *scan_new = toupper(*scan_new);
		----*/	
				if (isupper(*scan_new)) *scan_new = tolower(*scan_new);
				scan_new++;
			}
			else 
			{
				*scan_new++ = '_';
				*scan_new++ = hextable[(*name >> 4) & 0x0f];
				*scan_new++ = hextable[*name++ & 0x0f];
				*scan_new++ = '_';
			}
		}
		++i;
	}
	if (indexed != 0) *scan_new++ = ']';
	*scan_new = '\0';

#ifdef mark_in_out
printf ("exiting vlg_name %s\n", name_buffer);
#endif

	return (name_buffer);
}


/***************************************************************************
* function vlg_busname: gets a pointer to a list of connectors and returns *
* the next connector name in *name (that points to vec_buffer), the left   *
* and right indices of the vector (-1 if signal is a scalar) and the       *
* pointer to the next vector as the return value. Performs vectorization   *
* of locon names, that are always scalar.                                  *
***************************************************************************/
locon_list *vlg_busname(pt_list,left,right,name)
locon_list *pt_list;
int *left, *right;
char **name;
{
	char *blank_space;
	locon_list *ptcon;
	char END = 0;

#ifdef mark_in_out
printf("entering vlg_busname %s\n", pt_list->NAME);
#endif

	ptcon = pt_list;
/* Extract the name and number of an element */
	*left = *right = -1;
	strcpy(vec_name_buffer, ptcon->NAME);
	blank_space = strchr(vec_name_buffer, ' ');
	if (blank_space != NULL) 
	{
		*right = atoi(blank_space);
		*left = *right;
		*blank_space = '\0';
 
		while(END != 1) 
		{
			if(ptcon->NEXT != NULL) 
			{
				blank_space = strchr(ptcon->NEXT->NAME, ' ');
				if(blank_space != NULL) 
				{
					*blank_space = '\0';
					if(!strcmp(vec_name_buffer, ptcon->NEXT->NAME)) 
					{
						*right = atoi(blank_space + 1);
						ptcon = ptcon->NEXT; 
					}
					else END = 1;
					*blank_space = ' ';
				}
				else END = 1;
			}
			else END = 1;
		}
	}
	*name = vec_name_buffer;

#ifdef mark_in_out
printf("exiting vlg_busname %s\n", *name);
#endif

	return(ptcon->NEXT);
}


/*******************************************************************************
* function vlogsavelofig                                                       *
*******************************************************************************/

void vlogsavelofig(ptfig)
lofig_list *ptfig;
{
	loins_list *ptins = NULL;
	locon_list *ptcon = NULL;
	locon_list *ptscan = NULL;
	losig_list *ptsig = NULL;
	FILE *ptfile;
	time_t clock;
	int flag, left, right, i, first;
	char *name;
	char *sig_name;
	char direc_flag;
	chain_list *sig_list = NULL;

#ifdef mark_in_out
printf("entering vlogsavelofig\n");
#endif

	if ((ptfile = mbkfopen(ptfig->NAME, OUT_LO, WRITE_TEXT)) == NULL)
	{
		(void)fprintf(stderr, "*** mbk error ***\n");
		(void)fprintf(stderr, "cannot open file %s.%s\n", ptfig->NAME, OUT_LO);
		EXIT(1);
	}

	time (&clock);
	(void) fprintf (ptfile,"// Verilog structural description generated from `%s`\n", ptfig->NAME);
	(void) fprintf (ptfile,"// Date : %s\n\n", ctime(&clock));

/*********************
* Module declaration *
*********************/
	(void) fprintf (ptfile,"// Module Declaration.\n\n");
	(void) fprintf (ptfile,"module %s (\n", vlg_name(ptfig->NAME));
 
	if (ptfig->LOCON != NULL) {
	ptfig->LOCON = (struct locon *) reverse ((chain_list *)ptfig->LOCON);
	/*list inputs without indices first*/
	ptcon = ptfig->LOCON;
	flag = 0;
	while (ptcon != NULL) 
	{
		if (ptcon->DIRECTION == 'I') 
		{
			ptcon = vlg_busname(ptcon,&left,&right,&name);
			/*
			printf("list input %s\n", name);
			*/
			if (flag == 4) 
			{
				(void)fprintf(ptfile, ",\n");
				flag = 0;
			}
			if (flag != 0)
				(void)fprintf(ptfile, ", ");
			(void)fprintf(ptfile, "%s", vlg_name(name));
			++flag;
			}
			else ptcon = ptcon->NEXT;
		}
		/*then list I/Os*/
		ptcon = ptfig->LOCON;
		while (ptcon != NULL) 
		{
			direc_flag = ptcon->DIRECTION;
			if ((direc_flag != 'I') && (direc_flag != 'O') && (direc_flag != 'B')) 	
			{
				ptcon = vlg_busname(ptcon,&left,&right,&name);
				/*
				printf("list input-output %s %c\n", name, direc_flag);
				*/
				if (flag == 4) 
				{
					(void)fprintf(ptfile, ",\n");
					flag = 0;
				}
				if (flag != 0)
					(void)fprintf(ptfile, ", ");
				(void)fprintf(ptfile, "%s", vlg_name(name));
				++flag;
			}
			else ptcon = ptcon->NEXT;
		}
		/*then list outputs*/
		ptcon = ptfig->LOCON;
		while (ptcon != NULL) 
		{
			direc_flag = ptcon->DIRECTION;
			if ((direc_flag == 'O') || (direc_flag == 'B')) 
			{
				ptcon = vlg_busname(ptcon,&left,&right,&name);
				/*
				printf("list output %s\n", name);
				*/
				if (flag == 4) 
				{
					(void)fprintf(ptfile, ",\n");
					flag = 0;
				}
				if (flag != 0)
					(void)fprintf(ptfile, ", ");
				(void)fprintf(ptfile, "%s", vlg_name(name));
				++flag;
			}
			else ptcon = ptcon->NEXT;
		}
		(void) fprintf(ptfile, ");\n\n\n");

		(void) fprintf (ptfile,"// Declaration of external signals.\n\n");
		/*now list inputs and outputs with indices; bidirs are both listed as
		inputs and outputs*/
		ptcon = ptfig->LOCON;
		while (ptcon != NULL) 
		{
			direc_flag = ptcon->DIRECTION;
			ptcon = vlg_busname(ptcon, &left, &right, &name);
			if ((direc_flag != 'O') && (direc_flag != 'B')) 
			{
				if (left != -1)
					(void)fprintf(ptfile, "input [%d:%d] %s;\n", left, right, vlg_name(name));
				else
					(void)fprintf(ptfile, "input %s;\n", vlg_name(name));
			}
			if (direc_flag != 'I') 
			{
				if (left != -1)
					(void)fprintf(ptfile, "output [%d:%d] %s;\n", left, right, vlg_name(name));
				else
					(void)fprintf(ptfile, "output %s;\n", vlg_name(name));
			}
		}
	}
	(void) fprintf(ptfile, "\n\n");

	ptfig->LOCON = (struct locon *) reverse ((chain_list *)ptfig->LOCON);

/***********************************************************
* End of module declarations, now declare internal signals *
***********************************************************/

	(void) fprintf(ptfile, "// Declaration of internal signals.\n\n");
	ptsig = ptfig->LOSIG;
	while(ptsig != NULL) 
	{
		if (ptsig->TYPE == 'I') 
		{
			(void) fprintf (ptfile, "wire %s;\n", vlg_name(getsigname(ptsig)));
		}
		ptsig = ptsig->NEXT;
	}

/*********************************************************
* End of signal declarations, now comes the real netlist *
*********************************************************/

	(void) fprintf(ptfile, "\n\n// Declaration of instances.\n\n");
	ptfig->LOINS = (struct loins *) reverse ((chain_list *)ptfig->LOINS);
	for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT) 
	{
		/* name the module name and the instance name */
		(void) fprintf (ptfile, "%s ", vlg_name(ptins->FIGNAME));
		(void) fprintf (ptfile, "i_%s (\n  ", vlg_name(ptins->INSNAME));
		ptcon = ptins->LOCON;
		while(ptcon != NULL) 
		{
			vlg_busname(ptcon, &left, &right, &name);
			(void) fprintf (ptfile, ".%s(",vlg_name(name));
			if(left != -1) 
			{
				/* The connected signals are bused */
				for(i = abs(left - right); i >= 0; i--) 
				{
					ptsig = ptcon->SIG;
					if (ptsig->TYPE == 'I') 
					{
						/* The signal is internal */
						sig_name = namealloc(mvl_vhdlname(getsigname(ptsig)));
						sig_list = addchain(sig_list,(char *)sig_name);
					}
					else 
					{
						/* The signal is external */
						for (ptscan = ptfig->LOCON ; ptscan ; ptscan = ptscan->NEXT)
						{
							if (ptscan->SIG == ptsig)
							break;
						}
						if (ptscan == NULL)
						{
							(void) printf("\n*** mbk error *** no external connector  \n");
							(void) printf("     driving verilog file %s\n", ptfig->NAME);
						}
						else 
						{
							sig_name = namealloc(vlg_name(ptscan->NAME));
							sig_list = addchain(sig_list,(char *)sig_name);
						}
					}
					if(i > 0)
						ptcon = ptcon->NEXT;
				}
				first = 1;
				while(sig_list != NULL) 
				{
					if(first!=1) 
					{
						(void)fprintf(ptfile,"& %s",(char *)(sig_list->DATA));
					}
					else 
					{
						(void)fprintf(ptfile,"%s",(char *)(sig_list->DATA));
						first = 0;
					}
					sig_list = sig_list->NEXT;
				}
			}
			else 
			{
				/* The connected signals is simple */
				ptsig = ptcon->SIG;
				if (ptsig->TYPE == 'I') 
				{
					(void) fprintf(ptfile, "%s", vlg_name (getsigname(ptsig)));
				}
				else 
				{
					for (ptscan = ptfig->LOCON ; ptscan ; ptscan = ptscan->NEXT) 
					{
						if (ptscan->SIG == ptsig) break;
					}
					if (ptscan == NULL) 
					{
						(void) printf("\n*** mbk error *** no external connector  \n");
						(void) printf("     driving verilog file %s\n", ptfig->NAME);
					}
					else 
					{
						(void) fprintf(ptfile, "%s", vlg_name(ptscan->NAME));
					}
				}
			}
			if (ptcon->NEXT != NULL)
				(void) fprintf (ptfile, "),");
			ptcon = ptcon->NEXT;
		}
		(void) fprintf(ptfile, "));\n");
	}
	ptfig->LOINS = (struct loins *) reverse ((chain_list *)ptfig->LOINS);
	(void) fprintf(ptfile, "endmodule\n");
	(void) fclose  (ptfile);
}
