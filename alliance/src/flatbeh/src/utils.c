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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mut.h>
#include <mlo.h>
#include <abl.h>
#include <abe.h>
#include <aut.h>
#include "utils.h"

long max_index;

char *itostr(str,nbr)
char *str;
int   nbr;
{
char *s = (char *)mbkalloc(strlen(str)+5);
strcpy(s,str);
strcat(s,"%d");
sprintf(s,s,nbr);
return ((char *)s);
}

void Usage(nom)
char *nom;
{              
printf("\n"); 
printf("Usage : %s <input_file> [-b -P] [-a <output_file>]  \n",nom);
printf("          [-R <ratio value>]  [-S <SEED value>]\n");
printf("          [-N <number of parts>]  \n");
printf("\n");
printf("-b       Loading Behavioral description .\n");
printf("-a       makes the abstraction of the structural desription.\n");
printf("-p       perform partitionning.\n");
printf("-S       SEED value .\n");
printf("-R       Partitionning ratio.\n");
printf("-N       number of parts (0->2, 1->4, 2->8, 3->16, 4->32 )\n");
printf("\n");
exit (1);
}

char *busname(name)
char *name;
{
char *tmp_name;
char *s, *t;

        
        if (!name) {
                     printf("error in procedure busname\n");
                     return NULL;
                   }
        tmp_name = (char *)mbkalloc(strlen(name)+1);
        s = name;
        t = tmp_name;
        while((*s != ' ') && (*s != '\0')) 
        {
                *t++ = *s++;
        }
        *t = '\0';
        return (tmp_name);
}

char *Changename(sufix,name)
char *sufix,*name;
{
char *tmp_name ;
char *s, *s1, *t;
        if (!name) {
                     printf("error in procedure busname\n");
                     return NULL;
                   }
                   
        tmp_name = (char *)mbkalloc(strlen(name)+strlen(sufix)+1);           
        s = name;
	s1 = sufix;
        t = tmp_name;
        while((*s != ' ') && (*s != '\0')) 
        {
                *t++ = *s++;
        }
        while(*s1 != '\0') 
        {
                *t++ = *s1++;
	}
	while(*s != '\0')
        {
                *t++ = *s++;
        }
        *t = '\0';
        return (tmp_name);
}

int busindex(name)
char *name;
{
	char *tmp_index; 
	char  *s, *t;
	char blank_found = 0;
	int  ind;

	tmp_index=(char *)mbkalloc(strlen(name)+1);
	
	if (!name) {
		printf("error in procedure busname\n");
	   exit(1);
   }
	t = tmp_index;
	s = name;
	while (*s != '\0') 
	{
		if (*s == ' ') {s++;blank_found =1;break;}
		else ++s;
	}
	
	if (blank_found)
        while (*s !='\0') { *t++=*s++;}

	*t = '\0';
	sscanf(tmp_index,"%d",&ind);
	
	return(ind);
}

char *RemoveIndex(name)
char *name;
{
        char *new_name;
        char  *s, *t;

        new_name=(char *)mbkalloc(strlen(name)+1);

        if (!name) {
                printf("error in procedure busname\n");
                return NULL;
        }
        t = new_name;
        s = name;
        while (*s != '\0')
        {
                if (*s == ' ') {*t++='_';s++;}
                else *t++ = *s++;
        }

        *t = '\0';
        return(namealloc(new_name));
}

/*------------------------------------------------------
                                                       
Useful function to display locon and loins lists    
                                                  
------------------------------------------------------*/
void display_locon_list(p)
chain_list *p;
{ 
	chain_list *ptr; 
	for(ptr = p ; ptr!=NULL ; ptr = ptr->NEXT)
	{
		printf(" %s\n",((locon_list *)(ptr->DATA))->NAME);
	}
}


void display_loins_list(p)
chain_list *p;
{ 
	chain_list *ptr; 
	for(ptr = p ; ptr!=NULL ;ptr = ptr->NEXT)
	{
		printf(" %s\n",((loins_list *)(ptr->DATA))->INSNAME);
	}
}

/*void display_lists()
{
    printf(" Liste des connecteurs en entree\n");
	display_locon_list(in_ptr);

	printf("Liste des connecteurs en sortie \n");
	display_locon_list(out_ptr);
	
	printf("Liste des points memoire de la netliste\n");
	display_loins_list(mem_ptr);

	printf("Liste des points memoire du chemin de test\n");
	display_loins_list(scan_ptr);
	
}*/

/***********************************************************************************/

/* This function Gets the pointer to the LOCON connector in the instance pointed by ptri */
/***********************************************************************************/
 
locon_list *get_con(ptri,connector)
loins_list *ptri;
char *connector;
{
    locon_list *ptr1;
        
                        for ( ptr1 = ptri->LOCON ; ptr1 != NULL ; ptr1 = ptr1->NEXT)
                                {
                                        if ( ptr1->NAME == namealloc(connector)) return ptr1;           
                                }
        printf("no connector %s in model %s \n",connector,ptri->FIGNAME);
        exit(1);
        
}       
 
/*******************************************************************************************************************/
/*                                                                                                                 */
/*   changes the old  SIG->TYPE to INTERNAL, appends bist_ to SIGNAMECHAIN and sets SIG to a new signal pointer    */
/*                                                                                                                 */
/*******************************************************************************************************************/

void update_con(ptrc,new_ptr_sig)
locon_list *ptrc;
losig_list	*new_ptr_sig;
{    
	chain_list *name_list=ptrc->SIG->NAMECHAIN;
		
	char *st = (char *)mbkalloc(strlen((char *)name_list->DATA) + strlen("_bist")+1) ;

	sprintf(st,"bist_%s",(char *)name_list->DATA);
	
	ptrc->SIG->TYPE = INTERNAL ; /* update signals TYPE */
	
	name_list = delchain(name_list,name_list);
	
	ptrc->SIG->NAMECHAIN = addchain(name_list,(void *)namealloc(st));/* update signals name */
	
	ptrc->SIG = new_ptr_sig;/* set SIG connectors field to a  new signal */
	
}

/*******************************************************************************************************************/
/*                                                                                                                 */
/*                              search of the maximum signals INDEX                                                */
/*                                                                                                                 */
/*******************************************************************************************************************/

long get_max_index(ptfig)
lofig_list *ptfig;
{
	losig_list *pts;
	long i = 0;
	
	for(pts = ptfig->LOSIG ; pts ; pts = pts->NEXT)
	{
		if ((pts->INDEX) > i) 
		{ 
			i = pts->INDEX; 
		}
		
	}
	return(i);
	
}


/*******************************************************************************************************************/
/*                                                                                                                 */
/*                          gets a new number of signal INDEX                                                      */
/*                                                                                                                 */
/*******************************************************************************************************************/

long get_index()
{
	max_index = max_index +1;
	/* printf("index \n",max_index);*/
	return(max_index-1);
}



/*------------------------------------------------------------------------------
restore_Indexes : reconstitue les signaux busses sur une befig 
-----------------------------------------------------------------
parametres       : une befig .
-----------------------------------------------------------------
return           : .
------------------------------------------------------------------------------*/
void RestoreIndexes(ptr_befig)
befig_list *ptr_befig;
{
struct bepor *ptr_bepor;
struct bepor *ptr_bepor1;
struct bepor *save_bepor;
struct bepor *new_bepor = NULL;

struct beaux *ptr_beaux;
struct beaux *ptr_beaux1;
struct beaux *save_beaux;
struct beaux *new_beaux = NULL;

struct bereg *ptr_bereg;
struct bereg *ptr_bereg1;
struct bereg *save_bereg;
struct bereg *new_bereg = NULL;

void *sort[512] ;
char *prefix;
authtable * porTH, *regTH, *auxTH, *buxTH;

int index, i, permut=0;


porTH = createauthtable(100);
regTH = createauthtable(100);
auxTH = createauthtable(100);
buxTH = createauthtable(100);
/* restore indexes for bepor */
for (ptr_bepor = ptr_befig->BEPOR; ptr_bepor; ptr_bepor = ptr_bepor->NEXT)
{
	index = 0;
	prefix = namealloc(busname(ptr_bepor->NAME));
	if (searchauthelem(porTH,prefix) == NULL)
	{
		addauthelem(porTH, prefix,1);
		sort[index++] = ptr_bepor;
		for (ptr_bepor1 = ptr_bepor->NEXT; ptr_bepor1; ptr_bepor1 = ptr_bepor1->NEXT)
		{
			if (strcmp(busname(ptr_bepor1->NAME),prefix) == 0 )
				sort[index++] = ptr_bepor1;
		}
		/* sort the connectors */
		do
		{
		permut =0;
		for (i=0; i<index-1; i++)
			{
			if (busindex(((bepor_list *)sort[i])->NAME) > busindex(((bepor_list *)sort[i+1])->NAME))
				{ 
				 save_bepor = sort[i];	
				 sort[i] = sort[i+1];	
				 sort[i+1] = save_bepor;	
				 permut =1;
			
				}
			}
		}
		while (permut);

	}
	for (i =index-1 ; i>=0 ;i--)
		{
		new_bepor = beh_addbepor(new_bepor,((bepor_list *)sort[i])->NAME,((bepor_list *)sort[i])->DIRECTION,((bepor_list *)sort[i])->TYPE);
		/*printf("------>%s\n",sort[i]->NAME);*/
		}

}
/* sorting internal simple signals */
for (ptr_beaux = ptr_befig->BEAUX; ptr_beaux; ptr_beaux = ptr_beaux->NEXT)
{
	index = 0;
	prefix = namealloc(busname(ptr_beaux->NAME));
	if (searchauthelem(auxTH,prefix) == NULL)
	{
		addauthelem(auxTH, prefix,1);
		sort[index++] = ptr_beaux;
		for (ptr_beaux1 = ptr_beaux->NEXT; ptr_beaux1; ptr_beaux1 = ptr_beaux1->NEXT)
		{
			if (strcmp(busname(ptr_beaux1->NAME),prefix) == 0 )
				sort[index++] = ptr_beaux1;
		}
		/* sort the connectors */
		do
		{
		permut =0;
		for (i=0; i<index-1; i++)
			{
			if (busindex(((beaux_list *)sort[i])->NAME) > busindex(((beaux_list *)sort[i+1])->NAME))
				{ 
				 save_beaux = sort[i];	
				 sort[i] = sort[i+1];	
				 sort[i+1] = save_beaux;	
				 permut =1;
			
				}
			}
		}
		while (permut);

	}
	for (i =index-1 ; i>=0 ;i--)
		{
		new_beaux = beh_addbeaux(new_beaux,((beaux_list *)sort[i])->NAME,((beaux_list *)sort[i])->ABL);
		/*printf("------>%s\n",sort[i]->NAME);*/
		}

}
/* sorting internal registers */
for (ptr_bereg = ptr_befig->BEREG; ptr_bereg; ptr_bereg = ptr_bereg->NEXT)
{
	index = 0;
	prefix = namealloc(busname(ptr_bereg->NAME));
	if (searchauthelem(regTH,prefix) == NULL)
	{
		addauthelem(regTH, prefix,1);
		sort[index++] = ptr_bereg;
		for (ptr_bereg1 = ptr_bereg->NEXT; ptr_bereg1; ptr_bereg1 = ptr_bereg1->NEXT)
		{
			if (strcmp(busname(ptr_bereg1->NAME),prefix) == 0 )
				sort[index++] = ptr_bereg1;
		}
		/* sort the connectors */
		do
		{
		permut =0;
		for (i=0; i<index-1; i++)
			{
			if (busindex(((bereg_list *)sort[i])->NAME) > busindex(((bereg_list *)sort[i+1])->NAME))
				{ 
				 save_bereg = sort[i];	
				 sort[i] = sort[i+1];	
				 sort[i+1] = save_bereg;	
				 permut =1;
			
				}
			}
		}
		while (permut);

	}
	for (i =index-1 ; i>=0 ;i--)
		{
		new_bereg = beh_addbereg(new_bereg,((bereg_list *)sort[i])->NAME,((bereg_list *)sort[i])->BIABL);
		/*printf("------>%s\n",sort[i]->NAME);*/
		}

}

destroyauthtable(porTH);
destroyauthtable(regTH);
destroyauthtable(auxTH);
destroyauthtable(buxTH);

ptr_befig->BEPOR = new_bepor;
ptr_befig->BEREG = new_bereg;
ptr_befig->BEAUX = new_beaux;

/*return(ptr_befig);*/
}
