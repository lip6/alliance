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

/****************************************************************************/
/*                                                                          */
/*                      ALLIANCE CAO & VLSI CAD                             */
/*                                                                          */
/*    Product : (S)ymbolic (2)to (R)eal translater                          */
/*    File    : main.c                                                      */
/*                                                                          */
/*    (c) copyright 1992 MASI laboratory CAO & VLSI team                    */
/*    All Right Reserved                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*              Main of Symbolic to Real                                    */
/*                                                                          */
/****************************************************************************/

#ident "@(#)s2r 3.8 30/01/02 ALLIANCE 4.5 (S)ymbolic (2)to (R)eal converter"

#include <string.h>

#include <mph.h>
#include <mut.h>
#include <rds.h>
#include <rwi.h>
#include <rut.h>
#include <rtl.h>
#include <rfm.h>
#include <rpr.h>

#include "generic.h"
#include "rdsacces.h"
#include "statistics.h"
#include "postrat.h"

void Usage (Name)
     char *Name;
{
   fprintf (stderr, "\tUsage : %s [-cs1trv] source [result]\n\n", Name);
   fprintf (stderr, "\tsource\t: filename of symbolic layout circuit\n");
   fprintf (stderr, "\t\t  to translate to real layout\n");
   fprintf (stderr, "\tresult\t: result filename of real layout circuit\n");
   fprintf (stderr, "\t\t  source name is used, if result is absent\n\n");
   fprintf (stderr, "\toptionnal options (any order, any occurence) :\n");
   fprintf (stderr, "\t-c\t: deletes top-level (c)onnectors, keeps all others\n");
   fprintf (stderr, "\t-C\t: keeps top-level (c)onnectors, deletes all others\n");
   fprintf (stderr, "\t-n\t: deletes all signal names, but connectors\n");
   fprintf (stderr, "\t-s\t: (s)cotchs are created\n");
   fprintf (stderr, "\t-1\t: only (1) level is translated, all otherwise.\n");
   fprintf (stderr, "\t-t\t: layout is just (t)ranslated, not post-treated\n");
   fprintf (stderr, "\t-r\t: black boxes are not (r)eplaced\n");
   fprintf (stderr, "\t-v\t: (v)erbose mode armed\n");
   exit (1);
}

int main (argc, argv)
     int argc;
     char **argv;
{
   rdsfig_list *rds_model;
   phfig_list *mbk_model;

/*\
 * banner and parameters
\*/

   char *source_name = NULL;    /* need default values */
   char *result_name = NULL;    /* need default values */
   int root_conn = 1;           /* keep top-level figure connectors by default */
   int sub_conn = 1;            /* keep sub-level figure connectors by default */
   int signal_name = 1;         /* keep signal names by default */
   int scotch_on = 0;           /* scotchs are not created by default */
   int one_level = 0;           /* all hierarchy level translated by default */
   int do_post_treat = 1;       /* post-treatment is done by default */
   int do_replace_cells = 1;    /* cells_replacement is done by default */
   int verbose = 0;             /* verbose mode desarmed by default */
   int flat = 0;                /* flat mode desarmed by default */
   int physical = 0;            /* physical mode desarmed by default */
   int split_layer = 0;         /* split_layer mode desarmed by default */
   int shift;

   alliancebanner ("s2r", VERSION, "Symbolic to Real layout converter", "2002", ALLIANCE_VERSION);

   {
      char *debug_env;
      if ((debug_env = getenv ("S2RDEBUG")) != NULL)
      {
         unsigned char v = *debug_env - '0';
         s2rdebug = (v <= 9) ? v : 0;
      }
   }

   if (argc < 2)
      Usage (argv[0]);

   mbkenv ();
   rdsenv ();

   for (shift = 1; shift < argc; shift++)
   {
      char *arg = argv[shift];
      if (*arg == '-')
      {
         while (*++arg)
         {
            switch (*arg)
            {
            case 'C':
               sub_conn = 0;
               break;
            case 'c':
               root_conn = 0;
               break;
            case 'n':
               signal_name = 0;
               break;
            case 's':
               scotch_on = 1;
               break;
            case '1':
               one_level = 1;
               break;
            case 't':
               do_post_treat = 0;
               break;
            case 'r':
               do_replace_cells = 0;
               break;
            case 'v':
               verbose = 1;
               break;
            case 'l':
               split_layer = 1;
               break;
            case 'f':
               flat = 1;
               break;
            case 'p':
               physical = 1;
               break;
            default:
               Usage (argv[0]);
            }
         }
      }
      else
      {
         if (!source_name)
            source_name = namealloc (argv[shift]);
         else if (!result_name)
         {
            result_name = namealloc (argv[shift]);
         }
         else
            Usage (argv[0]);
         continue;
      }
   }

   if (!source_name)
   {
      Usage (argv[0]);
   }

   if (!result_name)
   {
      result_name = source_name;
   }

/*\
 * setting environement
 * MBK_CATAL_NAME    : file where file name to be replaced are put
 * MBK_CATA_LIB      : where cells, catal file and techno file are
 * MBK_WORK_LIB      : where result file is written or where techno file is
 *                     WORK_LIB is watched before CATA_LIB
 * MBK_WORK_LIB      : where result file is placed
 * MBK_IN_PH         : file format of symbolic figure
 * RDS_TECHNO_NAME   : techno file name
 * RDS_IN            : file format of real figures to replace
 * RDS_OUT           : file format of result file
\*/

/*\
 * Read the techno file
\*/
   printf ("\n\to loading technology file : %s\n", RDS_TECHNO_NAME);
   fflush (stdout);
   loadrdsparam ();

/*\
 * Read the model file
\*/
   if (physical)
   {
      printf ("\to loading %s level of real layout : %s\n", one_level ? "one" : "all", source_name);
      fflush (stdout);
      rds_model = getrdsfig (source_name, one_level ? 'P' : 'A', 0);
   }
   else
   {
      printf ("\to loading %s level of symbolic layout : %s\n", one_level ? "one" : "all", source_name);
      fflush (stdout);

      mbk_model = getphfig (source_name, 'A');
      if (!root_conn)
         mbk_model->PHCON = NULL;
      rds_model = S2Rfigmbkrds (mbk_model, one_level ? 0 : 1, sub_conn);

      /* Removing symbolic data structure */

      printf ("\to removing symbolic data structure \n");
      rds_mbk_kill ();
   }

/*\
 * flattening
\*/
   if (flat)
   {
      printf ("\to flattening figure\n");
      rflattenrdsfig (rds_model, RDS_NO, RDS_NO);
   }

/*\
 * post treatement
\*/
   rds_model = rds_model_out (source_name);

   if (do_post_treat)
   {
      printf ("\to layout post-treating\n ");
      if (root_conn)
         printf ("\t\twith top connectors,\n ");
      else
         printf ("\t\twithout top connector,\n ");
      if (sub_conn)
         printf ("\t\twith sub connectors,\n ");
      else
         printf ("\t\twithout sub connector,\n ");
      if (signal_name)
         printf ("\t\twith signal names,\n ");
      else
         printf ("\t\twithout signal name,\n ");
      if (scotch_on)
         printf ("\t\twith scotchs.\n");
      else
         printf ("\t\twithout scotch.\n");
      post_treat (rds_model, scotch_on, verbose);
      fflush (stdout);
   }

/*\
 * Replace cells
\*/

   if (do_replace_cells)
   {
      replace_cells (verbose);
   }

/*\
 * save file
\*/
   printf ("\to saving %s.%s\n", result_name, RDS_OUT);
   rds_model->NAME = result_name;

   if (split_layer)
   {
      char *RDS_CIF_LAYER_TABLE_BAK[RDS_ALL_LAYER][RDS_CIF_LAYER_FIELD];
      unsigned char RDS_GDS_LAYER_TABLE_BAK[RDS_ALL_LAYER][RDS_GDS_LAYER_FIELD];
      char WORK_LIB_TMP[256];
      char WORK_LIB_BAK[256];
      int layer;

      strcpy (WORK_LIB_BAK, WORK_LIB);
      for (layer = 0; layer < RDS_ALL_LAYER; layer++)
      {
         *strchr (RDS_LAYER_NAME[layer], ' ') = '\0';
         RDS_CIF_LAYER_TABLE_BAK[layer][0] = RDS_CIF_LAYER_TABLE[layer][0];
         RDS_GDS_LAYER_TABLE_BAK[layer][0] = RDS_GDS_LAYER_TABLE[layer][0];
         RDS_GDS_LAYER_TABLE[layer][0] = RDS_GDS_LAYER_EMPTY;
         RDS_CIF_LAYER_TABLE[layer][0] = RDS_CIF_LAYER_EMPTY;
      }

      for (layer = 0; layer < RDS_ALL_LAYER; layer++)
      {
         if (!strcmp (RDS_OUT, "gds"))
         {
            if (RDS_GDS_LAYER_TABLE_BAK[layer][0] != RDS_GDS_LAYER_EMPTY)
            {
               sprintf (WORK_LIB_TMP, "%s/%s/", WORK_LIB_BAK, RDS_LAYER_NAME[layer]);
               WORK_LIB = WORK_LIB_TMP;
               if (verbose)
               {
                  printf ("\t  - %s\n", WORK_LIB);
                  fflush (stdout);
               }

               RDS_GDS_LAYER_TABLE[layer][0] = RDS_GDS_LAYER_TABLE_BAK[layer][0];
               saverdsfig (rds_model);
               RDS_GDS_LAYER_TABLE[layer][0] = RDS_GDS_LAYER_EMPTY;
            }
         }
         else
         {
            if (RDS_CIF_LAYER_TABLE_BAK[layer][0] != RDS_CIF_LAYER_EMPTY)
            {
               sprintf (WORK_LIB_TMP, "%s/%s/", WORK_LIB_BAK, RDS_LAYER_NAME[layer]);
               WORK_LIB = WORK_LIB_TMP;
               if (verbose)
               {
                  printf ("\t  - %s\n", WORK_LIB);
                  fflush (stdout);
               }

               RDS_CIF_LAYER_TABLE[layer][0] = RDS_CIF_LAYER_TABLE_BAK[layer][0];
               saverdsfig (rds_model);
               RDS_CIF_LAYER_TABLE[layer][0] = RDS_CIF_LAYER_EMPTY;
            }
         }
      }
   }
   else
      saverdsfig (rds_model);

   if (verbose)
      print_statistics (scotch_on);

   return (0);
}
