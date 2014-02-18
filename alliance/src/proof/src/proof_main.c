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

/*
 * Tool    : Logic Synthesis - formal proof
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 * Modified by Ludo 2002
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

#include "proof_compile.h"
#include "proof_util.h"

char *GLOB_ABL_DC;
char *GLOB_ABL_ONE;
char *GLOB_ABL_ZERO;
   
extern befig_list *VHB_HEDFIG;


/*AFAC*/
char *nameIntVect (char *name);

		/* on ne verifie pas si les fichiers existent ...*/


/*----------------------------------------------------------------------------
verifArgsProof	: verifie les arguments de la ligne de commande.
------------------------------------------------------------------------------
retour		: un entier a 1 si OK, 1 sinon.
------------------------------------------------------------------------------*/

int verifArgsProof(argc,argv)
int argc;
char *argv[];
{

if (argc < 3  || argc > 6)
   {
   printf("\n");
   printf("proof    \nusage  :   proof -option");
   printf(" <file_entity1> <file_entity2> \n");
   printf("option :  -d   display the logical functions\n");
   printf("          -a   the common auxiliary signals are kept\n");
   printf("          -p   the polarity of the triggered edges is inversed \n");
   printf("               within the first behavioural description\n");
   return(0) ;
   }
if (argc > 3)
   {
   if(strcmp(argv[1],"-d") && strcmp(argv[1],"-a") && strcmp(argv[1],"-p"))
   	{
	printf("\nproof     : unknown option '%s'\n\n",argv[1]) ;
	return(0) ;
	}
   }
if (argc > 4)
   {
   if(strcmp(argv[2],"-d") && strcmp(argv[2],"-a") && strcmp(argv[2],"-p"))
   	{
	printf("\nproof     : unknown option '%s'\n\n",argv[2]) ;
	return(0) ;
	}
   }
if (argc > 5)
   {
   if(strcmp(argv[3],"-d") && strcmp(argv[3],"-a") && strcmp(argv[3],"-p"))
   	{
	printf("\nproof     : unknown option '%s'\n\n",argv[2]) ;
	return(0) ;
	}
   }

return(1);
}

/*======================================================================/
/		                MAIN					/
/======================================================================*/
int
main(argc,argv)
int argc ;
char *argv[];
{
char varaux;
befig_list *bef1,*bef2;
char *path;
char *auxpath;
int elimBusBef1,elimBusBef2;

		/* verification de la validite des arguments */

mbkenv();
alliancebanner("Proof",VERSION,"Formal Proof","1990", ALLIANCE_VERSION);

  GLOB_ABL_DC = namealloc ("'d'");
  GLOB_ABL_ONE = namealloc ("'1'");
  GLOB_ABL_ZERO = namealloc ("'0'");

//path = (char *)mbkalloc(5);
auxpath = (char *) mbkalloc (5);

path = WORK_LIB;

if (!mbkgetenv("MBK_CATA_LIB"))
   strcpy(auxpath,".");
else
   auxpath = mbkstrdup(mbkgetenv("MBK_CATA_LIB"));

if (argc > 2)
   {
   char *auxFile1 = mbkalloc(strlen(argv[argc-2]) + 5);
   char *auxFile2 = mbkalloc(strlen(argv[argc-1]) + 5);

   sprintf(auxFile1, "%s.vbe",argv[argc-2]);
   argv[argc-2] = auxFile1;

   sprintf(auxFile2, "%s.vbe",argv[argc-1]);
   argv[argc-1] = auxFile2;
   }

if (!verifArgsProof(argc,argv))
   exit(-1);


printf("================================  Environment  ================================\n");
printf("MBK_WORK_LIB		= %s\n",path);
printf("MBK_CATA_LIB		= %s\n",auxpath);
printf("=======================  Files, Options and Parameters  =======================\n");
printf("First VHDL file		= %s\n",argv[argc-2]);
printf("Second VHDL file	= %s\n",argv[argc-1]);

if(!strcmp(argv[1],"-a") || !strcmp(argv[2],"-a") || 
   (argc > 4 && !strcmp(argv[3],"-a")))
   printf("The common auxiliary signals are kept\n");
else
   printf("The auxiliary signals are erased\n");

if(!strcmp(argv[1],"-d") || !strcmp(argv[2],"-d") || 
   (argc > 4 && !strcmp(argv[3],"-d")))

printf("Errors are displayed\n");

printf("===============================================================================\n");
printf("\n");


if (argc > 2 && argc < 6)
   {

   initializeBdd(2);

		/* compilation du VHDL du premier circuit */
		/* on ne calcule pas les graphes tout de suite */

   if (compileVHDL(argv[argc-2],NULL,0,NULL,1,0,0,0,1,nameIntVect,NULL) == NULL)
      {
      printf("*** Compilation aborted...\n");
      exit(-1);
      }

   bef1 = VHB_HEDFIG;

   if(!strcmp(argv[1],"-a") || !strcmp(argv[2],"-a") || 
      (argc > 4 && !strcmp(argv[3],"-a")))
     varaux = 'A';
   else
     varaux = 'G';

		/* compilation du VHDL du deuxieme circuit */
		/* on ne calcule pas les graphes tout de suite */

   if (compileVHDL(argv[argc-1],NULL,0,NULL,1,0,0,0,1,nameIntVect,NULL) == NULL)
      {
      printf("compilation aborted\n");
      printf("*** Compilation aborted...\n");
      exit(-1);
      }

   bef2 = VHB_HEDFIG;


		/* changement de polarite des BD de bef1 */

   if(!strcmp(argv[1],"-p") || !strcmp(argv[2],"-p") || 
      (argc > 4 && !strcmp(argv[3],"-p")))
      polarBDBeh(bef1);

		/* elimination des INOUT */



   elimInoutBeh(bef1);
   elimInoutBeh(bef2);

		/* calcul d'ordonnancement */

   orderProofBeh(bef1,bef2,varaux);

		/* re-ecriture des bus multiplex */
   
   elimBusBef1 = elimBusMuxBeh(bef1);
   elimBusBef2 = elimBusMuxBeh(bef2);

   if (elimBusBef1 || elimBusBef2) 
      {
			/* re-ordonnancement */

      orderProofBeh(bef1,bef2,varaux);
      }

		/*  renomage des befig */

   {
   char *auxBeh1 = mbkalloc(strlen(path) + strlen(bef1->NAME) + 2);
   char *auxBeh2 = mbkalloc(strlen(path) + strlen(bef2->NAME) + 2);

   sprintf(auxBeh1, "%s/%s",path,bef1->NAME);
   bef1->NAME = auxBeh1;

   sprintf(auxBeh2, "%s/%s",path,bef2->NAME);
   bef2->NAME = auxBeh2;
   }

   if(!strcmp(argv[1],"-d") || !strcmp(argv[2],"-d") || 
      (argc > 4 && !strcmp(argv[3],"-d")))
      {
      int ret = proofBeh(bef1,bef2,1,0); 
      printf("\n\n\n");
      if (ret)
         exit(-1);
      else
         exit(0);
      }
   else
      {
      int ret = proofBeh(bef1,bef2,0,0); 
      printf("                      Formal Proof : ");
      if (ret > 0) 
         {
         printf("%d error%s\n",ret,(ret > 1)?"s":"");
         exit(-1);
         }
      else 
         {
         printf("OK\n\n\n");
         exit(0);
         }
      }
   }
   printf("\n\n\n");
   exit(-1);
}
