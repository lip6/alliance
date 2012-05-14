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

/* Simple barrel shifter generator.
   Author: Frédéric Pétrot
   Date  : 1/10/2000
   $Log: dpgen_Shifter.c,v $
   Revision 1.11  2012/05/14 14:20:13  alliance
   Updated GNU/FSF address (patch from Thibault North).

   Revision 1.10  2004/05/23 13:04:14  ludo
   - Bug: Back to the previous version without bufferized command cell. Those
   buffer cells (inv_x4 etc ..) are too big and do not fit in the first column area.

   Revision 1.9  2004/03/04 14:21:56  fred
   Adding more powerfull cells for the signals controlling the shift.

   Revision 1.8  2003/01/31 15:53:39  fred
   COPY_UP_SEG now copies all segments of a given name.
   This is particularly useful for CXXX layer types
   Makes two functions static in Shifter to avoid polluting the namespace

   Revision 1.7  2002/09/30 16:20:22  czo
   support/users

   Revision 1.6  2002/06/28 15:55:00  fred
   Fixing a bug (discovered by Christophe Alexandre): trying to compact a
   shifter even when no rotation was asked for not anymore performed.

   Revision 1.5  2002/06/17 09:40:27  fred
   Adding DPGEN_SHROT to support both shifts and rotations.
   Layout is quite larger when rotation is involved, so use only when
   necessary.

   Revision 1.4  2002/06/14 15:40:29  fred
   Adding rotation.
   Some test this week end and that should be it.
   Placement still to be done.

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

static char rcsid[]="$Id: dpgen_Shifter.c,v 1.11 2012/05/14 14:20:13 alliance Exp $";


#include  "util_Defs.h"
#include  "dpgen_Defs.h"
#include  "dgn.h"


static int ln2p(int n)
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

static void freestr()
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
   Shift: Shifter
   Shrot: Includes also rotation

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
   char *Cell;
   int sym, mys;

   modelName =      va_arg (aAL, char*);
   flags     =      va_arg (aAL, long );
   n         = (int)va_arg (aAL, long );

   Slices    = ln2p(n);

   
#define XX_NAME GENLIB_NAME

#define MX3(c0, c1, i0, i1, i2, q)  \
   GENLIB_LOINS("mx3_x2", XX_NAME("m_%d", SliceIndex * n + BitIndex), c0, c1, i0, i1, i2, q, "vdd", "vss", NULL)

#define A2(k, i0, i1, q)  \
   GENLIB_LOINS("a2_x2", XX_NAME("a_%d", k), i0, i1, q, "vdd", "vss", NULL)

#define A3(k, i0, i1, i2, q)  \
   GENLIB_LOINS("a3_x2", XX_NAME("a3_%d", k), i0, i1, i2, q, "vdd", "vss", NULL)

#define OA(i0, i1, i2, q)  \
   GENLIB_LOINS("oa22_x4", XX_NAME("oa_%d", SliceIndex * n + BitIndex), i0, i1, i2, q, "vdd", "vss", NULL)

#define O2(k, i0, i1, q)  \
   GENLIB_LOINS("o2_x2", XX_NAME("o_%d", k), i0, i1, q, "vdd", "vss", NULL)

#define INV(k, i, nq)  \
   GENLIB_LOINS("inv_x2", XX_NAME("i_%d", k), i, nq, "vdd", "vss", NULL)

#define SYM (BitIndex & 1 ? mys : sym)

   if (aFunction == DPGEN_SHIFT)
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

   /* Arithmetic or input extension */
   if (aFunction == DPGEN_SHROT) {
      INV(Slices, op(2), "op2b");
      A3(Slices, i(n - 1), "op2b", op(1), MSB);
   } else {
      A2(0, i(n - 1), op(1), MSB);
   }

   /* Normalizing the input signals names */
   for (BitIndex = 0; BitIndex < n; BitIndex++)
         GENLIB_LOSIGMERGE(i(BitIndex), MuxOutput(BitIndex));

   /* Connecting the mux matrix */
   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      for (BitIndex = 0; BitIndex < n; BitIndex++) {
         LeftIndex  = BitIndex - (1 << SliceIndex);
         RightIndex = BitIndex + (1 << SliceIndex);
         if (aFunction == DPGEN_SHIFT) {
            MuxInput0  = LeftIndex >= 0 ? MuxOutput(SliceIndex * n + LeftIndex) : LSB;
            MuxInput1  = MuxOutput(SliceIndex * n + BitIndex);
            MuxInput2  = RightIndex < n ? MuxOutput(SliceIndex * n + RightIndex) : MSB;
         } else {
            if (LeftIndex >= 0)
               MuxInput0  = MuxOutput(SliceIndex * n + LeftIndex);
            else {
               MuxInput0  = x(AndIndex);
               A2(SliceIndex * n + BitIndex, op(2), MuxOutput(SliceIndex * n + n + LeftIndex), x(AndIndex)); AndIndex++;
            }
            MuxInput1  = MuxOutput(SliceIndex * n + BitIndex);
            if (RightIndex < n)
               MuxInput2  = MuxOutput(SliceIndex * n + RightIndex);
            else {
               MuxInput2  = x(AndIndex);
               OA(op(2), MuxOutput(SliceIndex * n + RightIndex - n), MSB, x(AndIndex)); AndIndex++;
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

   BitIndex = n - 1;
   SliceIndex = 0;
   if (aFunction == DPGEN_SHROT) {
      GENLIB_SC_PLACE(XX_NAME("oa_%d", SliceIndex * n + BitIndex), SYM, 0, 0);
      GENLIB_SC_RIGHT(XX_NAME("m_%d", SliceIndex * n + BitIndex), SYM);
   } else 
      GENLIB_SC_PLACE(XX_NAME("m_%d", SliceIndex * n + BitIndex), SYM, 0, 0);

   for (SliceIndex = 1; SliceIndex < Slices; SliceIndex++) {
      if (aFunction == DPGEN_SHROT) {
         GENLIB_SC_RIGHT(XX_NAME("oa_%d", SliceIndex * n + BitIndex), SYM);
      }
      GENLIB_SC_RIGHT(XX_NAME("m_%d", SliceIndex * n + BitIndex), SYM);
   }

   for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
      if (aFunction == DPGEN_SHROT) {
         GENLIB_DEF_PHINS(XX_NAME("oa_%d", SliceIndex * n + n - 1));
         for (BitIndex = n-1; BitIndex >= 0; BitIndex--) {
         int l = 0, r = 0;
            LeftIndex  = BitIndex - (1 << SliceIndex);
            RightIndex = BitIndex + (1 << SliceIndex);
            if (RightIndex >= n) {
               if (BitIndex != n - 1)
                  GENLIB_SC_BOTTOM(XX_NAME("oa_%d", SliceIndex * n + BitIndex), SYM);
               r = 1;
            }
            if (LeftIndex < 0) {
               if (r == 1) {
                  GENLIB_SC_RIGHT(Cell = XX_NAME("a_%d", SliceIndex * n + BitIndex), SYM);
                  GENLIB_DEF_PHINS(XX_NAME("oa_%d", SliceIndex * n + BitIndex));
               } else
                  GENLIB_SC_BOTTOM(XX_NAME("a_%d", SliceIndex * n + BitIndex), SYM);
               l = 1;
            }
            if (l == 0 && r == 0)
               GENLIB_PLACE_BOTTOM("tie_x0", XX_NAME("tie_%d", SliceIndex * n + BitIndex), SYM);
         }
      }
      GENLIB_DEF_PHINS(XX_NAME("m_%d", SliceIndex * n + n - 1));
      for (BitIndex = n-2; BitIndex >= 0; BitIndex--) {
         GENLIB_SC_BOTTOM(XX_NAME("m_%d", SliceIndex * n + BitIndex), SYM);
      }
   }

   if (flags & F_PLACE) {
      BitIndex = n; /* Upper line, this is necessary for the computation of the symmetry */
      SliceIndex = 0;
      if (aFunction == DPGEN_SHROT) {
         GENLIB_DEF_PHINS(XX_NAME("oa_%d", SliceIndex * n + n - 1));
         GENLIB_SC_TOP(XX_NAME("i_%d", Slices), SYM);
         GENLIB_SC_RIGHT(XX_NAME("a3_%d", Slices), SYM);

      } else {
         GENLIB_DEF_PHINS(XX_NAME("m_%d", SliceIndex * n + n - 1));
         GENLIB_SC_TOP(XX_NAME("a_%d", 0), SYM);
      }
      for (SliceIndex = 0; SliceIndex < Slices; SliceIndex++) {
         if (SliceIndex > 0)
            if (aFunction == DPGEN_SHROT)
               GENLIB_DEF_PHINS(XX_NAME("oa_%d", SliceIndex * n + n - 1));
            else
               GENLIB_DEF_PHINS(XX_NAME("m_%d", SliceIndex * n + n - 1));
         if (SliceIndex == 0)
            GENLIB_SC_RIGHT(XX_NAME("i_%d", SliceIndex), SYM);
         else
            GENLIB_SC_TOP(XX_NAME("i_%d", SliceIndex), SYM);
         GENLIB_SC_RIGHT(XX_NAME("o_%d", SliceIndex), SYM);
      }

      /* I've done my best with genlib, but I need to realign overlapping
       * cells using some more powerfull tools, ...
       * This is done only for the last column, in order to keep the
       * vertical alignement constraints. */
      if (aFunction == DPGEN_SHROT) {
         phins_list *i, *f = getphins(WORK_PHFIG, Cell);
         phfig_list *p = getphfig(f->FIGNAME, 'P');

         for (i = WORK_PHFIG->PHINS; i; i = i->NEXT)
            if (i->XINS == f->XINS)
               if (!strncmp(i->INSNAME, "m_", 2))
                  i->XINS += p->XAB2 - p->XAB1;
      }

      GENLIB_SAVE_PHSC();
   }

   if (flags & F_BEHAV) {
      printf("Shifter behavior no yet implemented\n");
   }

   freestr();
}

#ifdef DEBUG_DPGEN_SHIFT
char *d2b(char *s, int x, int n)
{
   int i;

   for (i = 0; i < n; i++)
      s[i] = '0' + ((x >> (n - 1 - i)) & 1);
   s[i] = 0;
   return s;
}

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

int main(int argc, char *argv[])
{
   int a, b, sh, op, i, n, l, r;
   int mask;
   char s[33], t[33], u[33], v[33];

   if (argc != 3)
      exit(1);

   if (!strcmp(argv[1], "-s"))
      r = 1;
   else if (!strcmp(argv[1], "-r"))
      r = 2;
   else
      exit(1);

   n = atoi(argv[2]);

   if (n == 0)
      exit(1);

   if (n < 32)
      mask = ~(~0<<n); /* This bugs when n = 32! Why ? */
   else 
      mask = 0xffffffff; 
   l = ln2p(n);

   printf("in    op(%d downto 0);;\n", r);
   printf("in    shamt(%d downto 0);;\n", l - 1);
   printf("in    i(%d to 0);;\n", n - 1);
   printf("out   o(%d to 0);;\n", n - 1);
   printf("in    vdd;;\n");
   printf("in    vss;\n");
   printf("begin\n");

   for (i = 0; i < 10000; i++) {
      a  = random() & mask;
      sh = random() & ~(~0<<l);
      /* No tests for out of bound shift */
      if (sh > n - 1)
         sh = n - 1;
      op = random() & ~(~0<<r);
      switch (op) {
         case 0:
         case 2:
         printf("# sll ");
            b = (a << sh) & mask;
            break;
         case 1:
         printf("# srl ");
            b = (((unsigned int)a) >> sh) & mask;
            break;
         case 3:
         printf("# sra ");
            if (((1<<(n-1)) & a & mask) == (1<<(n-1)))
               b = ((a >> sh) | (~0<<(n-sh))) & mask;
            else 
               b = (a >> sh) & mask;
            break;
         case 4:
         case 6:
         printf("# rol ");
            b = ((a << sh) | (a >> (n - sh))) & mask;
            break;
         case 5:
         case 7:
         printf("# ror ");
            b = ((a >> sh) | (a << (n - sh))) & mask;
      }
      a &= mask;
      printf("\npat%04d: %s %s %s ?%s 1 0;\n", i,
                                             d2b(u, op, r + 1),
                                             d2b(v, sh, l),
                                             d2b(s, a, n),
                                             d2b(t, b, n));
   }
   printf("end;\n");
   exit(0);
}
#endif
