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

/*******************************************************************************
* Grog : generic rom generator                                                 *
*                                                                              *
* Architecture and leaf cells defined at Bull's research center at les Clayes  *
*                                                                              *
* Leaf cells modifications to meet idps design rules                           *
* version 0.0 on April/June 1992, Frederic Petrot                              *
*                                                                              *
* All programming in Genlib                                                    *
* version 0.0 on April/June 1992, Frederic Petrot                              *
*                                                                              *
* version 0.1, by Frederic Petrot                                              *
*                                                                              *
*******************************************************************************/

#include <stdio.h>
#include <math.h>
#include MUT_H
#include MPH_H
#include <mbkgen.h>
#include "grog.h"

#define ES2_12

#ifdef ES2_15
#define TECHNO                "ES2 1.5"
#define LAMBDA                1.6
#define T0_PRECHARGE          (11.42e-9 + (8.15e-11 * nb) + (4.83e-13 * nw))
#define T0_SETUP              (00.00)
#define T0_EVALUATION         (9.34e-9 + (1.08e-10 * nb) + (4.60e-13 * nw))
#define RESISTOR_EVALUATION   (8.03e-10)
#define T0_OE_TO_LZ           (3.05e-9 + (8.53e-13 * nb) + (1.03e-13 * nw))
#define RESISTOR_OE_TO_LZ     (2.42e-9)
#define T0_OE_TO_HZ           (5.57e-9 + (1.24e-12 * nb) + (1.34e-14 * nw))
#define RESISTOR_OE_TO_HZ     (1.12e-9)
#define T0_EVAL_WITH_HZ       (12.03e-9 + (1.08e-10 * nb) + (3.60e-13 * nw))
#define RESISTOR_EVAL_WITH_HZ (1.05e-9)
#endif
#ifdef ES2_12
#define TECHNO                "ES2 1.2"
#define LAMBDA                1.05
#define T0_PRECHARGE          (9.02e-9 + (8.58e-11 * nb) + (3.07e-13 * nw))
#define T0_SETUP              (00.00)
#define T0_EVALUATION         (7.18e-9 + (1.05e-10 * nb) + (4.29e-13 * nw))
#define RESISTOR_EVALUATION   (9.13e-10) 
#define T0_OE_TO_LZ           (2.13e-9 + (5.16e-13 * nb) + (9.31e-14 * nw))
#define RESISTOR_OE_TO_LZ     (2.85e-9)
#define T0_OE_TO_HZ           (4.83e-9 + (1.81e-13 * nb) + (4.27e-15 * nw))
#define RESISTOR_OE_TO_HZ     (1.09e-9)
#define T0_EVAL_WITH_HZ       (9.42e-9 + (1.05e-10 * nb) + (3.15e-13 * nw))
#define RESISTOR_EVAL_WITH_HZ (1.11e-9)
#endif
#ifdef ES2_10
#define TECHNO                "PROL 1.0"
#define LAMBDA                0.875
#define T0_PRECHARGE          (5.65e-9 + (4.80e-11 * nb) + (2.49e-13 * nw))
#define T0_SETUP              (00.00)
#define T0_EVALUATION         (4.91e-9 + (8.42e-11 * nb) + (2.79e-13 * nw))
#define RESISTOR_EVALUATION   (6.01e-10)
#define T0_OE_TO_LZ           (1.01e-9 + (4.46e-13 * nb) + (7.82e-14 * nw))
#define RESISTOR_OE_TO_LZ     (1.94e-9)
#define T0_OE_TO_HZ           (3.72e-9 + (1.32e-14 * nb) + (1.51e-15 * nw))
#define RESISTOR_OE_TO_HZ     (1.06e-9)
#define T0_EVAL_WITH_HZ       (5.97e-9 + (8.42e-11 * nb) + (2.75e-13 * nw))
#define RESISTOR_EVAL_WITH_HZ (7.48e-10)
#endif
#define CAPA_POLY             (44.0e-6 * LAMBDA * LAMBDA * 1e-12)
#define CAPA_GRID             (1254.0e-6 * LAMBDA * LAMBDA * 1e-12)
#define CAPA_METAL1           (21.0e-6 * LAMBDA * LAMBDA * 1e-12)
#define CAPA_METAL2           (10.0e-6 * 2 * LAMBDA * LAMBDA * 1e-12)
#define CAPA_ADDRESS          (ln2p(nw) * (70  * CAPA_METAL2 + \
                                           300 * CAPA_METAL1 + \
                                           80  * CAPA_POLY + \
                                           75  * CAPA_GRID))
#define CAPA_CLOCK            (ln2p(nw) * (100 * CAPA_METAL2 + \
                                           29  * CAPA_POLY + \
                                           24  * CAPA_GRID))
#define CAPA_OE               (ln2p(nw) * (300 * CAPA_METAL2 + \
                                           25  * CAPA_METAL1 + \
                                           30  * CAPA_POLY + \
                                           74  * CAPA_GRID))

void grogdata(char *name, int nb, int nw, int tr, int zh, int msb, biglong *data)
{
FILE *f = stdout;
int transistors;

   fprintf(f, "Name                  : %s\n", name);
   fprintf(f, "Function              : ROM of %d words of %d bits\n", nw, nb);
   if (zh)
   fprintf(f, "                        with tristate buffers on outputs\n");
   fprintf(f, "Technology            : %s\n", TECHNO);
   fprintf(f, "Dimensions            :\n");
   fprintf(f, "                        Height       Width        Area\n");
   fprintf(f, "Lambdas                 %-12ld %-12ld %-12ld\n", 
               (WORK_PHFIG->YAB2 - WORK_PHFIG->YAB1) / SCALE_X,
               (WORK_PHFIG->XAB2 - WORK_PHFIG->XAB1) / SCALE_X,
               ((WORK_PHFIG->XAB2 - WORK_PHFIG->XAB1) / SCALE_X)
               * ((WORK_PHFIG->YAB2 - WORK_PHFIG->YAB1) / SCALE_X));
   fprintf(f, "Microns                 %-12.3f %-12.3f %-12.3f\n", 
               ((WORK_PHFIG->YAB2 - WORK_PHFIG->YAB1) / SCALE_X) * LAMBDA,
               ((WORK_PHFIG->XAB2 - WORK_PHFIG->XAB1) / SCALE_X) * LAMBDA,
               (((WORK_PHFIG->XAB2 - WORK_PHFIG->XAB1) / SCALE_X) * LAMBDA)
               * (((WORK_PHFIG->YAB2 - WORK_PHFIG->YAB1) / SCALE_X) * LAMBDA));
   switch (nw) { /* approximation */
      case 64 : transistors = 85 * nb + 283; break;
      case 128 : transistors = 149 * nb + 464; break;
      case 256 : transistors = 300 * nb + 485; break;
      case 512 : transistors = 578 * nb + 519; break;
      case 1024 : transistors = 1157 * nb + 1000; break;
      case 1536 : transistors = 1731 * nb + 1458; break;
      case 2048 : transistors = 2305 * nb + 1900; break;
      case 2560 : transistors = 2879 * nb + 2364; break;
      case 3072 : transistors = 3453 * nb + 2808; break;
      case 3584 : transistors = 4027 * nb + 3252; break;
      case 4096 : transistors = 4601 * nb + 3696; break;
   }
   fprintf(f, "Number of transistors : %d\n", transistors);

   fprintf(f, "Input capacitances    :\n");
   fprintf(f, "adr                   : %5.3g\n", CAPA_ADDRESS);
   fprintf(f, "ck                    : %5.3g\n", CAPA_CLOCK);
   fprintf(f, "oe                    : %5.3g\n", CAPA_OE);
   /* how :
      so much sweat for so many printfs... */
   fprintf(f, "Timing (worst case)   :\n");
   fprintf(f, "precharge             : %5.3g s\n", T0_PRECHARGE);
   fprintf(f, "setup                 : %5.3g s\n", T0_SETUP);
   fprintf(f, "evaluation            : %5.3g + %5.3g C s\n", T0_EVALUATION,
                                                         RESISTOR_EVALUATION);
   fprintf(f, "oe to lz              : %5.3g + %5.3g C s\n", T0_OE_TO_LZ, 
                                                         RESISTOR_OE_TO_LZ);
   fprintf(f, "oe to hz              : %5.3g + %5.3g C s\n", T0_OE_TO_HZ,
                                                         RESISTOR_OE_TO_HZ);
   fprintf(f, "eval with hz buffers  : %5.3g + %5.3g C s\n",
                                                         T0_EVAL_WITH_HZ,
                                                         RESISTOR_EVAL_WITH_HZ);
}
