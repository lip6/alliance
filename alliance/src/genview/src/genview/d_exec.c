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

#ifdef pc
#undef pc
#endif
#define READ_CHAR(X)        *((char*)(X))
#define READ_SHORT(X)       *((short*)(X))
#define READ_INT(X)         *((int*)(X))
#define READ_UNSCHAR(X)     *((unsigned char*)(X))
#define READ_UNSSHORT(X)    *((unsigned short*)(X))
#define READ_UNSINT(X)      *((unsigned int*)(X))
#define READ_FLOAT(X)       *((float*)(X))
#define READ_DOUBLE(X)      *((double*)(X))

#define QI 0      /* code QI 0 */
#define HI 1      /* code HI 1 */
#define SI 2      /* code SI 2 */
#define DF 3      /* code DF 3 */
#define SF 4      /* code SF 4 */
#define UNSQI 5  /* code UNSQI 5 */
#define UNSHI 6  /* code UNSHI 6 */
#define UNSSI 7  /* code UNSSI 7 */

#define REG 0  /* sauvegarde dans reg    */
#define DATA 1  /* --------------- memoire */
#define SP 2  /* --------------- I_sp   */

#define TRUE 1
#define FALSE 0

#define DEST 0
#define SRC 1

extern int I_pc;  /* program counter */
extern int I_cc0;  /* code condition   */
extern int I_reg[32];  /* registers      */
extern int taille_data;  /* indicateur de taille de data */

#include <math.h>
#include <stdio.h>
#include "d_fonc.h"
#include "d_asm.h"
#include "d_interp_lib.h"
#include "d_dbx.h"

/* flags :
   code condition flags are set with these values, on an int.
   self explanatory definitions. */
#define EQ 1
#define SUP 2
#define INF 4
#define SUP_EQ 8
#define INF_EQ 16
#define UNSEQ 32
#define UNSSUP 64
#define UNSINF 128
#define UNSSUP_EQ 256
#define UNSINF_EQ 512
#define DIFF 1024

extern int I_cc0;  /* code condition   */
extern int I_reg[32];  /* registers      */
extern int I_Debut_Code;/* debut du bloc code */

extern FONC TabFonc[];
extern int taille_TabFonc;

extern D_traceAll();

BOOL D_FinInterp = TRUE;

/* find the destination of an operation :
   the function finds where to put the result of an operation, and returns
   the address or the register number needed.
   pc :  indicates the address mode of the destination
   pc1:  indicates the end of the current opcode :
            - operand for immediat addressing
            - memory address for any memory mode
            - unused when registers are involved

   adr:  memory address or register number of the destination */
int
  dest(pc, pc1, adr)
int pc, *pc1, *adr;
{
int num_reg;

  /* mask :
     3 first bits are the addressing mode */
  switch ((READ_UNSCHAR (pc) & 0xE0) >> 5) {
    case 1:  /* direct memory addressing */
      *adr = READ_INT (*pc1); /* address points on INT, not on char */
      /*VerifAdr(DEST, *adr);*/
      *pc1 = *pc1 + 4;
      return DATA;
    case 5:  /* direct register addressing */
      *adr = (READ_UNSCHAR (pc) & 0x1F); /* register number on 5 bits:1F */
      return REG;
    case 6:  /* indirect register addressing */
      num_reg = READ_UNSCHAR (pc) & 0x1F;
      *adr = I_reg[num_reg];
      if (num_reg == 30 || num_reg == 31)
        return SP;
      else {
        /*VerifAdr(DEST, *adr);*/
        return DATA;
      }
    case 7:  /* indirect register + displacement */
      *adr = READ_INT (*pc1);  /* displacement */
      *pc1 = *pc1 + 4;
      num_reg = READ_UNSCHAR (pc) & 0x1F;
      *adr = *adr + I_reg[num_reg];
      if (num_reg == 30 || num_reg == 31)
        return SP;
      else {
        /*VerifAdr(DEST, *adr);*/
        return DATA;
      }
    default:
      Erreur("Unknown addressing mode given by the compiler!!!\n");
  }
  /* NOT REACHED */
}

/* finds operands of all types using addressing mode stressed by pc.
   A double is used as return value since it's the wider type we handle.
   type_op indicates the operand type :
      CHAR   : QI
      SHORT  : HI
      INT    : SI
      DOUBLE : DF
      float  : SF */
double
  oper(pc, pc1, type_op)
int pc, *pc1, type_op;
{
double a;
int num_reg, deplace, mem;

  /* mask :
     3 first bits are addressing mode */
  switch ((READ_UNSCHAR(pc) & 0xE0) >> 5) {
      case 0:  /* immediat addressing mode */
             /* quelque soit le type_op en mode immediat l'operande
             est ecrit comme un int par l'assembleur, le cas ou
             l'operande est un double ou float ne se produit jamais
             en mode immediat */
      a = READ_INT(*pc1);
      *pc1 = *pc1 + 4;
      return a;
    case 1:  /* direct memory addressing mode */
      /*VerifAdr(SRC, READ_INT(*pc1));*/
      switch (type_op) {
        case QI:
          a = READ_CHAR(READ_INT(*pc1));
          *pc1 = *pc1 + 4;
          return a;
        case HI:
          a = READ_SHORT(READ_INT(*pc1));
          *pc1 = *pc1 + 4;
          return a;
        case SI:
          a = READ_INT(READ_INT(*pc1));
          *pc1 = *pc1 + 4;
          return a;
        case DF:
          a = READ_DOUBLE(READ_INT(*pc1));
          *pc1 = *pc1 + 4;
          return a;
        case SF:
          a = READ_FLOAT(READ_INT(*pc1));
          *pc1 = *pc1 + 4;
          return a;
        default:
          Erreur("Cannot found operand\n");
      }
    case 5:  /* direct register addressing mode */
        a = I_reg[READ_UNSCHAR(pc) & 0x1F]; /* register number 5 bits */
        return a;
    case 6:  /* indirect register addressing mode */
      num_reg = READ_UNSCHAR(pc) & 0x1F;
      mem = (num_reg == 30 || num_reg == 31) ? FALSE : TRUE;
      /* if (mem == TRUE)
        VerifAdr(SRC, I_reg[num_reg]);*/
      switch (type_op) {
        case QI:
          a = READ_CHAR(I_reg[num_reg]);
          return a;
        case HI:
          a = READ_SHORT(I_reg[num_reg]);
          return a;
        case SI:
          a = READ_INT(I_reg[num_reg]);
          return a;
        case DF:
          a = READ_DOUBLE(I_reg[num_reg]);
          return a;
        case SF:
          a = READ_FLOAT(I_reg[num_reg]);
          return a;
        default:
          Erreur("Cannot found the operand\n");
      }
    case 7:  /* indirect register  + displacement addressing mode */
      num_reg = READ_UNSCHAR(pc) & 0x1F;
      mem = (num_reg == 30 || num_reg == 31) ? FALSE : TRUE;
      deplace = READ_INT(*pc1);
      /*if (mem == TRUE)
        VerifAdr(SRC, I_reg[num_reg] + deplace);*/
      *pc1 = *pc1 + 4;
      switch (type_op) {
        case QI:
          a = READ_CHAR(I_reg[num_reg] + deplace);
          return a;
        case HI:
          a = READ_SHORT(I_reg[num_reg] + deplace);
          return a;
        case SI:
          a = READ_INT(I_reg[num_reg] + deplace);
          return a;
        case DF:
          a = READ_DOUBLE(I_reg[num_reg] + deplace);
          return a;
        case SF:
          a = READ_FLOAT(I_reg[num_reg] + deplace);
          return a;
        default:
          Erreur("Cannot found operand\n");
      }
    default:
      Erreur("Unknown adressing mode\n");
  }
  /* NOT REACHED */
}

/* save result :
   stores the result of an operation in the proper place.
   See comments on parameters and variables in binary interpretation
   file. */
void
  save(type_dest, r_double, mode_dest, adr)
int type_dest, adr, mode_dest;
double r_double;
{
  /* type conversion :
     this function is called with a double because it's the wider data
     size we've got to deal with, and it is converted back to its proper
     type before use. */
  switch (type_dest) {
    case QI: {
      char r1;
        r1 = r_double;
        if (mode_dest == DATA || mode_dest == SP)
          READ_CHAR(adr) = r1;
        else
          I_reg[adr] = r1;
        break;
      }
    case HI: {
      short r1;
        r1 = r_double;
        if (mode_dest == DATA || mode_dest == SP)
          READ_SHORT(adr) = r1;
        else
          I_reg[adr] = r1;
        break;
      }
    case SI: {
      int r1;
        r1 = r_double;
        if (mode_dest == DATA || mode_dest == SP)
          READ_INT(adr) = r1;
        else
          I_reg[adr] = r1;
        break;
      }
    case UNSQI: {
      unsigned char r1;
        r1 = r_double;
        if (mode_dest == DATA || mode_dest == SP)
          READ_UNSCHAR(adr) = r1;
        else
          I_reg[adr] = r1;
        break;
      }
    case UNSHI: {
      unsigned short r1;
        r1 = r_double;
        if (mode_dest == DATA || mode_dest == SP)
          READ_UNSSHORT(adr) = r1;
        else
          I_reg[adr] = r1;
        break;
      }
    case UNSSI: {
      unsigned int r1;
        r1 = r_double;
        if (mode_dest == DATA || SP)
          READ_UNSINT(adr) = r1;
        else
          I_reg[adr] = r1;
        break;
      }
    case DF: {
        if (mode_dest == DATA || mode_dest == SP)
          READ_DOUBLE(adr) = r_double;
        else
          Erreur("Compiler affected a double in a register!");
        break;
      }
    case SF: {
      float r1;
        r1 = r_double;
        if (mode_dest == DATA || mode_dest == SP)
          READ_FLOAT(adr) = r_double;
        else
          READ_FLOAT(I_reg[adr]) = r1;
        break;
      }
  }
}

/* logical operators :
   each operator take two operands a and b, retrieved by the oper function,
   and puts the result into the address/register returned by dest.
   the value of the pc change is returned.
   pc :  indicates the byte containing the addressing mode of an operand
   pc1:  indicates the end of the current opcode :
            - operande for immediat addressing
            - memory address, for any memory mode
            - is not used when involving registers
   a  :  first operand converted to the right machine mode
   b  :  second operand converted to the right machine mode
   r  :  result obtained in the right machine mode of operator
   adr:  address or register number used to store the result
  r_double:
         the result is converted to double in order to use the same function
         for each machine mode
   mode_dest:
         the destination is
              - 0 : register
              - 1 : memory
              - 2 : stack
  All coments apply to all functions, so they appear only once! */
int
  andqi()
{
int pc, pc1, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;                       /* goto first argument, the result   */
  pc++;                               /* address mode of the result        */
  mode_dest = dest(pc, &pc1, &adr);
  pc++;                               /* address mode of the 1st operand   */
  a = oper(pc, &pc1, QI);             /* 1st operand with QI machine mode  */
  pc++;                               /* address mode of the 2nd operand   */
  b = oper(pc, &pc1, QI);             /* 2nd operand with QI machine mode  */
  r = a & b;                          /* operation                         */
  r_double = r;
  save(QI, r_double, mode_dest, adr); /* put the result where needed       */
  return pc1 - pc + 3;                /* compute the right pc displacement */
}

int
  andhi()
{
int pc, pc1, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a & b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  andsi()
{
int pc, pc1, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a & b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  orqi()
{
int pc, pc1, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r = a | b;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  orhi()
{
int pc, pc1, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a | b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  orsi()
{
int pc, pc1, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a | b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}


int
  xorqi()
{
int pc, pc1, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r = a ^ b;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  xorhi()
{
int pc, pc1, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a ^ b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  xorsi()
{
int pc, pc1, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a ^ b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}


int
  notqi()
{
int pc, pc1, adr, mode_dest;
char a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  r = ~a;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  nothi()
{
int pc, pc1, adr, mode_dest;
short a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  r = ~a;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  notsi()
{
int pc, pc1, a, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  r = ~a;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* load and store :
   the mov operators load from places to places datas.
   Here only two operands are expected, an therefor only one call
   to oper is performed. */
int
  movsi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a_double = oper(pc, &pc1, 2);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  movqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a_double = oper(pc, &pc1, QI);
  save(QI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  movhi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a_double = oper(pc, &pc1, 1);
  save(HI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  movsf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a_double = oper(pc, &pc1, 4);
  save(SF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}


int
  movdf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a_double = oper(pc, &pc1, 3);
  save(DF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* comparisons :
   checks two operands and position code condition register arcordingly. */
int
  cmpqi()
{
int pc, pc1;
double a_double, b_double;
unsigned char uns_a, uns_b;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  uns_a = a_double;
  pc++;
  b_double = oper(pc, &pc1, QI);
  uns_b = b_double;

  /* tests :
     In order to set all the code condition register, comparisons should
     be done with signed and unsigned values.
      first signed checks : */
  if (a_double == b_double)
    I_cc0 = EQ | SUP_EQ | INF_EQ;
  else if (a_double > b_double)
    I_cc0 = SUP | SUP_EQ | DIFF;
  else if (a_double < b_double)
    I_cc0 = INF | INF_EQ | DIFF;
  /* and second :
     unsigned ones. */
  if (uns_a == uns_b)
    I_cc0 = I_cc0 | UNSEQ | UNSSUP_EQ | UNSINF_EQ;
  else if (uns_a > uns_b)
    I_cc0 = I_cc0 | UNSSUP | UNSSUP_EQ | DIFF;
  else if (uns_a < uns_b)
    I_cc0 = I_cc0 | UNSINF | UNSINF_EQ | DIFF;
  return pc1 - pc + 2;
}

int
  cmphi()
{
int pc, pc1;
double a_double, b_double;
unsigned short uns_a, uns_b;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, HI);
  uns_a = a_double;
  pc++;
  b_double = oper(pc, &pc1, HI);
  uns_b = b_double;
  if (a_double == b_double)
    I_cc0 = EQ | SUP_EQ | INF_EQ;
  else if (a_double > b_double)
    I_cc0 = SUP | SUP_EQ | DIFF;
  else if (a_double < b_double)
    I_cc0 = INF | INF_EQ | DIFF;
  if (uns_a == uns_b)
    I_cc0 = I_cc0 | UNSEQ | UNSSUP_EQ | UNSINF_EQ;
  else if (uns_a > uns_b)
    I_cc0 = I_cc0 | UNSSUP | UNSSUP_EQ | DIFF;
  else if (uns_a < uns_b)
    I_cc0 = I_cc0 | UNSINF | UNSINF_EQ | DIFF;
  return pc1 - pc + 2;
}

int
  cmpsi()
{
int pc, pc1;
double a_double, b_double;
unsigned uns_a, uns_b;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SI);
  uns_a = a_double;
  pc++;
  b_double = oper(pc, &pc1, SI);
  uns_b = b_double;
  if (a_double == b_double)
    I_cc0 = EQ | SUP_EQ | INF_EQ;
  else if (a_double > b_double)
    I_cc0 = SUP | SUP_EQ | DIFF;
  else if (a_double < b_double)
    I_cc0 = INF | INF_EQ | DIFF;
  if (uns_a == uns_b)
    I_cc0 = I_cc0 | UNSEQ | UNSSUP_EQ | UNSINF_EQ;
  else if (uns_a > uns_b)
    I_cc0 = I_cc0 | UNSSUP | UNSSUP_EQ | DIFF;
  else if (uns_a < uns_b)
    I_cc0 = I_cc0 | UNSINF | UNSINF_EQ | DIFF;
  return pc1 - pc + 2;
}

int
  cmpsf()
{
int pc, pc1;
double a_double, b_double;

  I_cc0 = 0;
  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  b_double = oper(pc, &pc1, SF);

  if (a_double == b_double)
    I_cc0 = EQ | SUP_EQ | INF_EQ;
  else if (a_double > b_double)
    I_cc0 = SUP | SUP_EQ | DIFF;
  else if (a_double < b_double)
    I_cc0 = INF | INF_EQ | DIFF;
  return pc1 - pc + 2;
}

int
  cmpdf()
{
int pc, pc1;
double a_double, b_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  b_double = oper(pc, &pc1, DF);

  if (a_double == b_double)
    I_cc0 = EQ | SUP_EQ | INF_EQ;
  else if (a_double > b_double)
    I_cc0 = SUP | SUP_EQ | DIFF;
  else if (a_double < b_double)
    I_cc0 = INF | INF_EQ | DIFF;
  return pc1 - pc + 2;
}

/* arithmetic operators :
   the behave much like logical onces. */
int
  addsi()
{
int pc, pc1, dep, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a + b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  addqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r = a + b;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  adddf()
{
int pc, pc1, dep, adr, mode_dest;
double a, b, r;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, DF);
  pc++;
  b = oper(pc, &pc1, DF);
  r = a + b;
  save(DF, r, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  addhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a + b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  addsf()
{
int pc, pc1, dep, adr, mode_dest;
float a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SF);
  pc++;
  b = oper(pc, &pc1, SF);
  r = a + b;
  r_double = r;
  save(SF, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  subsi()
{
int pc, pc1, dep, a, b, r, adr, mode_dest;
double r_double;
extern int I_reg[];

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a - b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  subqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r = a - b;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  subdf()
{
int pc, pc1, dep, adr, mode_dest;
double a, b, r;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, DF);
  pc++;
  b = oper(pc, &pc1, DF);
  r = a - b;
  save(DF, r, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  subhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a - b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  subsf()
{
int pc, pc1, dep, adr, mode_dest;
float a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SF);
  pc++;
  b = oper(pc, &pc1, SF);
  r = a - b;
  r_double = r;
  save(SF, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  mulsi()
{
int pc, pc1, dep, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a * b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  mulqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r = a * b;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  muldf()
{
int pc, pc1, dep, adr, mode_dest;
double a, b, r;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, DF);
  pc++;
  b = oper(pc, &pc1, DF);
  r = a * b;
  save(DF, r, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  mulhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a * b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  mulsf()
{
int pc, pc1, dep, adr, mode_dest;
float a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SF);
  pc++;
  b = oper(pc, &pc1, SF);
  r = a * b;
  r_double = r;
  save(SF, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  modsi()
{
int pc, pc1, dep, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r = a % b;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  modqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r = a % b;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  modhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r = a % b;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  divsi()
{
int pc, pc1, dep, a, b, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  b = oper(pc, &pc1, SI);
  r =(int)(a / b);
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  divqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  b = oper(pc, &pc1, QI);
  r =(char)(a / b);
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  divdf()
{
int pc, pc1, dep, adr, mode_dest;
double a, b, r;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, DF);
  pc++;
  b = oper(pc, &pc1, DF);
  r = a / b;
  save(DF, r, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  divhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  b = oper(pc, &pc1, HI);
  r =(short)(a / b);
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  divsf()
{
int pc, pc1, dep, adr, mode_dest;
float a, b, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SF);
  pc++;
  b = oper(pc, &pc1, SF);
  r = a / b;
  r_double = r;
  save(SF, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  negqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  r = -a;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 2;
  return dep;
}

int
  neghi()
{
int pc, pc1, dep, adr, mode_dest;
short a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  r = -a;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 2;
  return dep;
}

int
  negsi()
{
int pc, pc1, dep, a, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  r = -a;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 2;
  return dep;
}

/* shifts :
      logic :
      the space freed by the shift is filled with zero.
      arithmetic :
         the space left by the shift is filled with the value of the most
         or least significant bit, depending on the shift. */
int
  lslqi()
{
int pc, pc1, n, adr, mode_dest;
char a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  n = oper(pc, &pc1, QI);
  r = a << n;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}


int
  lslhi()
{
int pc, pc1, n, adr, mode_dest;
short a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  n = oper(pc, &pc1, HI);
  r = a << n;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  lslsi()
{
int pc, pc1, a, n, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  n = oper(pc, &pc1, SI);
  r = a << n;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  lsrqi()
{
int pc, pc1, n, adr, mode_dest;
char a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  n = oper(pc, &pc1, QI);
  r = a >> n;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  lsrhi()
{
int pc, pc1, n, adr, mode_dest;
short a, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  n = oper(pc, &pc1, HI);
  r = a >> n;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  lsrsi()
{
int pc, pc1, a, n, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  n = oper(pc, &pc1, SI);
  r = a >> n;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  return pc1 - pc + 3;
}

int
  aslqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, n, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  n = oper(pc, &pc1, QI);
  r = a << n;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  aslhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, n, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  n = oper(pc, &pc1, HI);
  r = a << n;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  aslsi()
{
int pc, pc1, dep, a, n, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  n = oper(pc, &pc1, SI);
  r = a << n;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  asrqi()
{
int pc, pc1, dep, adr, mode_dest;
char a, n, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, QI);
  pc++;
  n = oper(pc, &pc1, QI);
  r = a >> n;
  if (a < 0)
    r = -r;
  r_double = r;
  save(QI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  asrhi()
{
int pc, pc1, dep, adr, mode_dest;
short a, n, r;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, HI);
  pc++;
  n = oper(pc, &pc1, HI);
  r = a >> n;
  if (a < 0)
    r = -r;
  r_double = r;
  save(HI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

int
  asrsi()
{
int pc, pc1, dep, a, n, r, adr, mode_dest;
double r_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  pc++;
  a = oper(pc, &pc1, SI);
  pc++;
  n = oper(pc, &pc1, SI);
  r = a >> n;
  if (a < 0)
    r = -r;
  r_double = r;
  save(SI, r_double, mode_dest, adr);
  dep = pc1 - pc + 3;
  return dep;
}

/* conversions :
   the host machine does all conversion for us, only by affecting the
   right types. */

/* extMode1Mode2 :
   extend an integral value so it fits into a wider mode.
   fill the resulting space with most significant bit. */
int
  extqisi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  extqihi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(HI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  exthisi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, HI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  extsfdf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(DF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* ext0Mode1Mode2 :
   extend an integral value so it fits into a wider mode.
   fill the resulting space with zeros. */
int
  ext0qisi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  a_double = abs(a_double);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  ext0qihi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  a_double = abs(a_double);
  save(HI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  ext0hisi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, HI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  a_double = abs(a_double);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  ext0sfdf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  a_double = abs(a_double);
  save(DF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* truncMode1Mode2 :
   truncates an integral value so it fits in a shorter machine mode. */
int
  truncsiqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(QI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  truncsihi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(HI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  trunchiqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, HI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(QI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  truncdfsf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* floatMode1Mode2 :
   convert an integal value into a float. */
int
  floatqisf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  floatsisf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  floathisf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, HI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  floatqidf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, QI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(DF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  floatsidf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(DF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  floathidf()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, HI);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(DF, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* fixtruncMode1Mode2 :
   converts a float into an integral value. */
int
  fix_truncsfqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(QI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_truncsfhi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(HI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_truncsfsi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_truncdfqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(QI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_truncdfhi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(HI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_truncdfsi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(SI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* fixunsMode1Mode2 :
   converts a float in an unsigned integer. */
int
  fix_unssfqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(UNSQI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_unssfhi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(UNSHI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_unssfsi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, SF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(UNSSI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_unsdfqi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(UNSQI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_unsdfhi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(UNSHI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

int
  fix_unsdfsi()
{
int pc, pc1, adr, mode_dest;
double a_double;

  pc = I_pc;
  pc1 = pc + 4;
  pc++;
  a_double = oper(pc, &pc1, DF);
  pc++;
  mode_dest = dest(pc, &pc1, &adr);
  save(UNSSI, a_double, mode_dest, adr);
  return pc1 - pc + 2;
}

/* functions :
   the interpreter needs a list of fonctions that have been entered. */
PTFUNC
  FUNC_malloc()
{
PTFUNC pt_func;

  pt_func = (PTFUNC)mbkalloc(sizeof(FUNC));
  pt_func->name_func = '\0';
  pt_func->adr_func = 0;
  pt_func->next_func = '\0';
  return pt_func;
}

/* State of the running mode :
   When the Next button is pressed when calling a function, all
   sub functions should be executed in run mode.
   When NbCall is negatif, then we're exiting _main. */
#define STEP 0
#define RUN  1
#define NEXT 2
int NbCall;                /* total number of calls */
static int NbCallWhenNext; /* number of calls when hitting Next */

extern PTFUNC first_func;
extern ptfonc First_fonction;
/* call to a subroutine :
   The return address is saved on the stack, the the pc changed to the
   function address.
   The number of imbricated calls is incremented, because depending on the
   state of the interpreter at call time -- STEP, NEXT --, actions differ. */
int
  call()
{
char *nom_de_fonc;
int pc, pc1;
PTFUNC list_func;
PTFUNC save_list_func;
ptfonc fonction;
extern char run_mode;
extern BOOL D_ModeTrace;

  NbCall++;
  if (run_mode == NEXT) {
     if (D_ModeTrace == TRUE)
        NbCallWhenNext = NbCall;
     D_ModeTrace = FALSE;
  }

  I_reg[31] -= 4; /* saving return address */
  READ_INT(I_reg[31]) = I_pc + 8;

  for (list_func = first_func; list_func; list_func = list_func->next_func)
    save_list_func = list_func;
  save_list_func->next_func = FUNC_malloc();
  list_func = save_list_func->next_func;

  if (READ_UNSCHAR(I_pc + 1) == 0xC0) { /* indirect register call */
    *((unsigned char *)(I_pc + 1)) = 0xA0; /* change to direct register */
    pc = I_pc + 1;
    pc1 = I_pc + 4;
    I_pc = oper(pc, &pc1, SI);
  } else
    I_pc = *((int *)&(READ_CHAR(I_pc + 4))); /* direct memory call */

  for (fonction = First_fonction; fonction; fonction = fonction->next_fonc)
    if (I_pc == fonction->code_adr + I_Debut_Code) {
      nom_de_fonc = fonction->name;
      break;
    }

  list_func->name_func = nom_de_fonc;
  list_func->adr_func = I_reg[31] - 4;
  return 0;
}

/* returns from subroutines :
   The return address is restored, if this is not the return from _main. */
int
  ret()
{
extern int LigneCourante;
PTFUNC list_func, save_list_func, s_save_list_func;
#define STEP 0
#define RUN  1
#define NEXT 2
extern char run_mode;
extern BOOL D_ModeTrace;

  NbCall--;
  if (NbCall < 0) {
    D_FinInterp = TRUE;
    E_Ligne(LigneCourante);
    T_print("End of interpretation\n");
    return D_EXIT;
  }

  for (list_func = first_func; list_func; list_func = list_func->next_func) {
    s_save_list_func = save_list_func;
    save_list_func = list_func;
  }
  free(save_list_func);
  s_save_list_func->next_func = 0;

  I_pc = READ_INT(I_reg[31]); /* restore return address */
  I_reg[31] += 4;

  if (run_mode == NEXT && NbCall < NbCallWhenNext)
    D_ModeTrace = TRUE;
  return 0;
}

/* jumps :
   all the branch operations are treated here.
   they directly modifiy the pc and return 0 if the conditions are
   ok, advance to next instruction if not. */
int
  bra()
{
int pc, pc1;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  I_pc = oper(pc, &pc1, SI);
  return 0;
}

int
  beq()
{
int pc, pc1, dep, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & EQ) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bgt()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & SUP) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  blt()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & INF) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bge()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & SUP_EQ) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  ble()
{
int pc, pc1, dep, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & INF_EQ) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bne()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & DIFF) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bgtu()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & UNSSUP) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bltu()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & UNSINF) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bgeu()
{
int pc, pc1, dep, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & UNSSUP_EQ) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

int
  bleu()
{
int pc, pc1, adr_br;

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  adr_br = oper(pc, &pc1, SI);
  if (I_cc0 & UNSINF_EQ) {
    I_pc = adr_br;
    return 0;
  }
  return pc1 - pc + 1;
}

/* ligne :
   this pretty tricky instruction is the result of an .stabd indicating
   the current line number under the editor. */
int
  ligne()
{
int num_ligne;
extern BOOL D_ModeTrace;  /* booleen true if STEP false if RUN */
extern int LigneCourante;

  LigneCourante = num_ligne = *((unsigned short *)(I_pc + 2));
  if (D_ModeTrace || VerifBp(num_ligne)) {
    D_traceAll();
    E_Ligne(num_ligne);
    return D_EXIT + 4;
  }
  return 4;
}

/* nop :
   this one was easy. */
int
  nop()
{
  return 4;
}

/* unresolved externals :
   at compile time, the functions are referenced as label,
   and when assembling, the call to ``external'' functions is change to
   int, to indicate the interpreter to search in its library for them. */
int
  interrupt()
{
int pc, pc1;
int index;
char *nom_fonc;
char buffer[80];

  pc = I_pc + 1;
  pc1 = I_pc + 4;
  index = oper(pc, &pc1, SI);
  nom_fonc = SymbInTab(index);
  index = RechInTable(nom_fonc, TabFonc, taille_TabFonc, sizeof(FONC));
  if (strcmp(nom_fonc, TabFonc[index].nom_fonc)) {
    sprintf(buffer, "%s unknown function\n", nom_fonc);
    T_print(buffer);
    return D_EXIT;
  }
  TabFonc[index].fonc();
  return 8;
}
