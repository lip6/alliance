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

/****************************************************************************/
/*                                                                          */
/*                      ALLIANCE CAO & VLSI CAD                             */
/*                                                                          */
/*    Product : (S)ymbolic (2)to (R)eal translater                          */
/*    File    : rdsx2y.c                                                    */
/*                                                                          */
/*    (c) copyright 1992 MASI laboratory CAO & VLSI team                    */
/*    All Right Reserved                                                    */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*              Main of cif or gds translater                               */
/*                                                                          */
/****************************************************************************/

#include "generic.h"
#include  <mph.h>
#include  <mut.h>
#include  <rds.h>
#include  <rfm.h>
#include  <rpr.h>
#include  <rwi.h>
#include  <rut.h>
#include  <rtl.h>

getarg (argc, argv, model_name)
     int argc;
     char **argv;
     char **model_name;
{
   char *option1;
   char *option2;

   if (argc != 2)
   {
      fprintf (stderr, "usage : %s <file> \n", argv[0]);
      exit (1);
   }
   *model_name = namealloc (argv[1]);
}


main (argc, argv)
     int argc;
     char **argv;
{
   char *model_name;
   rdsfig_list *pf;

  /**** banner and parameters ****/
   getarg (argc, argv, &model_name);

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
   mbkenv ();
   rdsenv ();
   loadrdsparam ();
   pf = getrdsfig (model_name, 'A', 0);
   saverdsfig (pf);

   return (0);
}
