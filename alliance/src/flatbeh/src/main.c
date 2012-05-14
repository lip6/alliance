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

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit : Extracteur de vue comportementale                           */
/*    Fichier : main.c                                                      */
/*                                                                          */
/*    (c) copyright 1991 Laboratoire MASI equipe CAO & VLSI                 */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*                                                                          */
/****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <mut.h>
#include <mlo.h>
#include <mlu.h>
#include <abl.h>
#include <abe.h>
#include <abv.h>
#include <aut.h>
#include <bdd.h>
#include "utils.h"
#include "abstract.h"


	/* ********* MAIN ********* */
extern int main(argc, argv)
int argc;
char *argv[];
{
befig_list *befig;
lofig_list *lofig;
int trace = 0;
char *outputFile;






alliancebanner_with_authors("FlatBeh", VERSION " [2000/11/01]", "a netlist abstractor", "1993",
               ALLIANCE_VERSION, "François DONNET, Huu Nghia VUONG");

if ((argc < 2) || (argc > 3))
   {
   printf("\nflatbeh %s\n", VERSION);
   printf("\nUsage\t: flatbeh input_file [output_file]\n\n");
   printf("\t  input_file  = circuit at gate level\n");
   printf("\t  output_file = circuit at logic level (default name = input_file + abs)\n\n");
   exit(1);
   }

   /*init*/
   mbkenv();   /*mbk*/
   ablenv();   /*abl*/
   autenv();  /*hash table*/
   bddenv();  /*for vhdlloadbefig() */

if (mbkgetenv("MBK_TRACE"))
   trace = atoi(mbkgetenv("MBK_TRACE"));

if (argc == 2)
   {
   outputFile = mbkalloc(strlen(argv[1]) + 4);
    
   sprintf(outputFile, "%sabs", argv[1]);
   }
else
   outputFile = argv[2];


printf("     =========================  Environnement  =========================\n");
printf("     MBK_WORK_LIB        = %s\n", WORK_LIB);
printf("     MBK_CATA_LIB        = %s\n", mbkgetenv("MBK_CATA_LIB"));
printf("     MBK_CATAL_NAME      = %s\n", mbkgetenv("MBK_CATAL_NAME"));
if (trace > 0)
printf("     MBK_TRACE           = %d\n", trace);
printf("     ============================== Files ==============================\n");
printf("     Netlist file        = %s.%s\n", argv[1], IN_LO);
printf("     Output  file        = %s.vbe\n", outputFile);
printf("     ===================================================================\n");
printf("\n");

	/*  mise a plat de la figure */
lofig = getlofig(argv[1], 'A');
fprintf (stdout,"Loading '%s/%s.%s'\n", WORK_LIB, lofig->NAME, IN_LO);
/*------------------  mise a plat de la figure -------------------- */

printf("flattening figure %s \n",lofig->NAME);

rflattenlofig(lofig, YES, YES);
	
if ((!(lofig->LOINS)) || (lofig->LOTRS))
   {
   printf("ERROR : Transistor level description in %s : verify your CATAL file !\n",
          lofig->NAME);
#ifdef LAX
   LXU_Error("Transistor level description : verify your CATAL file !");
#endif
   exit(0);;
   }

/*------------------ chainage de la figure -------------------------*/

lofigchain(lofig);
befig = laxExtract(lofig, trace);
fprintf(stdout,"Restoring array's orders \n");
RestoreIndexes(befig);
if (!befig)
   {
   fprintf(stderr,"Some error has occurred\n");
   exit(1);
   }
   
   befig->NAME = outputFile;

/* if (trace) displayBehMap(befig, 1); */

   vhdlsavebefig(befig, 1);

   exit(0);

}
