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

/* Simple barrel shifter generator.
   Author: Frédéric Pétrot
   Date  : 1/10/2000
   $Log: dpgen_Shifter.c,v $
   Revision 1.3  2002/06/13 15:38:39  fred
   Starting implementation of rotatation, not yet usable.

   Revision 1.2  2002/06/12 16:11:08  fred
   Minor editing modifications

   Revision 1.1  2002/04/29 13:40:54  jpc
   Merging GenLib & DpGen.

   Revision 1.5  2001/10/19 14:15:04  boris
   Retournement du placement physique des bits 0 à n-1

   Revision 1.4  2000/11/06 10:32:44  jpc

   * dpgen_Shifter.c :
     - Bug : l'ordre des connecteurs etait faux dans la doc. Donc on aligne
             dpgen_Shifter.c avec la doc.

   * dpgen_RF1.c/dpgen_Defs.h/dpgen_Macro.c/dgnvrr.h :
     - Suppression de l'argument 'aRFFlags' dans la fonction 'dpgen_RF1'
       (redondant avec 'aFunction').

   Revision 1.3  2000/11/03 15:08:28  jpc

   * dpgen_Shifter.c :
     - Bug : le nom du modele n'etait pas pris en compte dans la generation
             de la vue physique... (elle etait forcee a "sh%d").

   * dpgen_RF1.c :
     - Bug : le buffer generant les signaux de selection du multiplexeur
             d'entree (datain0/datain1) sortait "sel0" et "sel1" alors que
             dans la matrice les multiplexeurs lisaient "sel0[x]" et
             "sel1[x]". On n'utilise plus que "sel0" et "sel1".

   * dpgen_RF1.c/dpgen_Defs.h/dpgen_Macro.c/dgnvrr.h :
     - Creation de la circuiterie preparatoire a l'implementation des bancs
       de registres speciaux :

          1. Registre R0 "colle" a zero (pour le MIPS).
          2. Registre avec decodeur.
          3. Registre en mode FIFO (grace a un decodeur special).

   Revision 1.2  2000/10/26 12:20:30  fred
   Added a pre placement for the shifter.

   Revision 1.1  2000/10/02 15:33:15  jpc

   * dpgen.sh :
     - Bug : les options longues n'etaient pas activees.

   * docs/SPECS.html :
     - Mise a jour de la specification du shifter.
     - Correction de l'ordre des connecteurs de DFFT (inversion de "scan" et
       "scin").

   * dpgen_Shifter.c/dpgen_Defs.h/dpgen_Macro.c :
     - Integration du generateur de shifter (fournit seulement la netlist pour
       l'instant).

*/

static char rcsid[]="$Id: dpgen_Shifter.c,v 1.3 2002/06/13 15:38:39 fred Exp $";


#include  "util_Defs.h"
#include  "dpgen_Defs.h"
#include  "dgn.h"


int ln2p(int n)
{
int i = 0, j = n & 1;

   if (n)
      for (i = -1; n > 0; n >>= 1) {
         j += (n & 1);
         i++;
      }
   return i + (j > 1);
}

static chain_list *c; /* To free the allocated names */

void freestr()
{
   chain_list *cc;
   for (cc = c; cc; cc = cc->NEXT)
      mbkfree(cc->DATA);
   freechain(c);
}

#define namegen(x) \
char *x(int n)     \
{     \
   char *s = mbkalloc(16 * sizeof(char));     \
   c = addchain(c, s);     \
   sprintf(s, #x "[%d]", n);     \
   return s;     \
}

namegen(i)
namegen(o)
namegen(MuxOutput)
namegen(c0)
namegen(c1)
namegen(shamt)
namegen(op)
namegen(x)

/* Behavior:

   o = (i op shamt);

   where op is :
  0X0 : logical left shift
  001 : logical right shift
  011 : arithmetical right shift
  1X0 : left rotation
  1X1 : right rotation
*/
extern void dpgen_Shifter(aFunction, aAL)
     long  aFunction;
  va_list  aAL;
{
   char *modelName;
   long flags;
   int n;
   int Slices;
   int BitIndex, SliceIndex;
   int LeftIndex, RightIndex;
   int AndIndex = 0;
   int ops;
   char *LSB, *MSB; 
   char *MuxInput0, *MuxInput1, *MuxInput2;  /* Mux inputs */
   int sym, mys;

   modelName =      va_arg (aAL, char*);
   /* 0: Shifter
      1: Includes also rotation */
   flags     =      va_arg (aAL, long );
   flags     = 0;
   n         = (int)va_arg (aAL, long );

   Slices    = ln2p(n - 1);

   
#define XX_NAME GENLIB_NAME

#define MX3(c0, c1, i0, i1, i2, q)  \
   GENLIB_LOINS("mx3_x2", XX_NAME("m_%d", SliceIndex * n + BitIndex), c0, c1, i0, i1, i2, q, "vdd", "vss", NULL)

#define MX2(k, c0, i0, i1, q)  \
   GENLIB_LOINS("mx2_x2", XX_NAME("c_%d", 2 * SliceIndex + k), c0, i0, i1, q, "vdd", "vss", NULL)

#define A2(k, i0, i1, q)  \
   GENLIB_LOINS("a2_x2", XX_NAME("a_%d", k), i0, i1, q, "vdd", "vss", NULL)

#define O2(k, i0, i1, q)  \
   GENLIB_LOINS("o2_x2", XX_NAME("o_%d", k), i0, i1, q, "vdd", "vss", NULL)

#define INV(k, i, nq)  \
   GENLIB_LOINS("inv_x2", XX_NAME("i_%d", k), i, nq, "vdd", "vss", NULL)

   if (flags == 0)
      ops = 1;
   else 
      ops = 2;

   GENLIB_DEF_LOFIG(modelName);

   GENLIB_LOCON(GENLIB_BUS("op", ops, 0), 'I', GENLIB_BUS("op", ops, 0));
   GENLIB_LOCON(GENLIB_BUS("shamt", Slices - 1, 0), 'I', GENLIB_BUS("shamt", Slices - 1, 0));
   GENLIB_LOCON(GENLIB_BUS("i", n - 1, 0), 'I', GENLIB_BUS("i", n - 1, 0));
   GENLIB_LOCON(GENLIB_BUS("o", n - 1, 0), 'O', GENLIB_BUS("o", n - 1, 0));
   GENLIB_LOCON("vdd", 'I', "vdd");
   GENLIB_LOCON("vss", 'I', "vss");

   /* Forcing a zero on lsb for left shifts */
   LSB = op(0);
   MSB = "msb"; /* intermediate signal for arith sh */

   /* Arithmetic extension */
   if (flags == 0)
      A2(AndIndex++, MuxOutput(n - 1), op(1), MSB);

   /* Normalizing the input signals names */
   for (BitIndex = 0; BitIndex < n; BitIndex++)
         GENLIB_LOSIGMERGE(i(BitIndex), MuxOutput(BitIndex));

   /* Connecting the mux matrix */
   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      for (BitIndex = 0; BitIndex < n; BitIndex++) {
         LeftIndex  = BitIndex - (1 << SliceIndex);
         RightIndex = BitIndex + (1 << SliceIndex);
         if (flags == 0) {
            MuxInput0  = LeftIndex >= 0 ? MuxOutput(SliceIndex * n + LeftIndex) : LSB;
            MuxInput1  = MuxOutput(SliceIndex * n + BitIndex);
            MuxInput2  = RightIndex < n ? MuxOutput(SliceIndex * n + RightIndex) : MSB;
         } else {
            if (LeftIndex >= 0)
               MuxInput0  = MuxOutput(SliceIndex * n + LeftIndex);
            else {
               MuxInput0  = x(AndIndex);
               A2(AndIndex, op(2), MuxOutput(SliceIndex * n + n - 1), x(AndIndex)); AndIndex++;
            }
            MuxInput1  = MuxOutput(SliceIndex * n + BitIndex);
            if (RightIndex < n)
               MuxInput2  = MuxOutput(SliceIndex * n + RightIndex);
            else {
               MuxInput2  = x(AndIndex);
               A2(AndIndex, op(2), MuxOutput(SliceIndex * n + n - 1), x(AndIndex)); AndIndex++;
            }
         }
         MX3(c0(SliceIndex), c1(SliceIndex), MuxInput0,  MuxInput1, MuxInput2, MuxOutput((SliceIndex + 1) * n + BitIndex));
      }
   }

   /* Normalizing the output signals names */
   for (BitIndex = 0; BitIndex < n; BitIndex++)
         GENLIB_LOSIGMERGE(o(BitIndex), MuxOutput(Slices * n + BitIndex));

   /* Building the control signals */
   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      INV(SliceIndex, shamt(SliceIndex), c1(SliceIndex));
      O2 (SliceIndex, c1(SliceIndex),    op(0), c0(SliceIndex));
   }

   GENLIB_SAVE_LOFIG();

   /* Let's place this stuff approximatively */
   if (n & 1)
      sym = SYM_Y, mys = NOSYM;
   else
      sym = NOSYM, mys = SYM_Y;

   GENLIB_DEF_PHSC(modelName);
   GENLIB_SC_PLACE(XX_NAME("a_%d", 0), sym, 0, 0);
   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      GENLIB_SC_RIGHT(XX_NAME("i_%d", SliceIndex), sym);
      GENLIB_SC_RIGHT(XX_NAME("o_%d", SliceIndex), sym);
   }
   GENLIB_DEF_PHINS("a_0");
   BitIndex = n - 1;
   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      if (SliceIndex == 0)
         GENLIB_SC_BOTTOM(XX_NAME("m_%d", SliceIndex * n + BitIndex), mys);
      else
         GENLIB_SC_RIGHT(XX_NAME("m_%d", SliceIndex * n + BitIndex), mys);
   }
   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      GENLIB_DEF_PHINS(XX_NAME("m_%d", SliceIndex * n + n-1));
      for (BitIndex = n-2; BitIndex >= 0; BitIndex--) {
         GENLIB_SC_BOTTOM(XX_NAME("m_%d", SliceIndex * n + BitIndex),
               BitIndex & 1 ? mys : sym);
      }
   }
   GENLIB_SAVE_PHSC();
   freestr();
}
