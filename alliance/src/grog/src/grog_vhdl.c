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

/*
* vhdl & pattern generation of grog
*/
#include <stdio.h>
#include <string.h>
#include MUT_H
#include "grog.h"

static void vbe();
static void pat();
static void vlg();

void grogvhdl(char *s, int nb, int nw, int zh, int r, int msb, biglong *data)
{
FILE *f;

   if ((f = mbkfopen(s, "vbe", "w")) == NULL) {
      fprintf(stderr, "grog error : cannot open file %s.vbe\n", s);
      EXIT(2);
   }
   vbe(f, s, nw, nb, zh, r, msb, data);
   fclose(f);
}

void grogpat(char *s, int nb, int nw, int zh, int r, int msb, biglong *data)
{
FILE *f;

   if ((f = mbkfopen(s, "pat", "w")) == NULL) {
      fprintf(stderr, "grog error : cannot open file %s.pat\n", s);
      EXIT(3);
   }
   pat(f, s, nw, nb, zh, r, msb, data);
   fclose(f);
}

void grogverilog(char *s, int nb, int nw, int zh, int r, int msb, biglong *data)
{
FILE *f;

   if ((f = mbkfopen(s, "v", "w")) == NULL) {
      fprintf(stderr, "grog error : cannot open file %s.v\n", s);
      EXIT(2);
   }
   vlg(f, s, nw, nb, zh, r, msb, data);
   fclose(f);
}

char *long_hex(char *s, long i, int n)
{
static char hexa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                        'a', 'b', 'c', 'd', 'e', 'f' };
long j, k, l;

   l = (n >> 2) + ((n % 4) == 0 ? 0 : 1);
   *(s + l) = '\0'; /* end of string */
   for (j = 0xF, k = 0; k < l; k ++, j <<= 4)
      *(s + l - k - 1) = hexa[(i & j) >> (k << 2)];
   return s;
}

char *long_bin(char *s, long i, int n)
{
long j, k;

   *(s + n) = '\0'; /* end of string */
   for (j = 1, k = 0; k < n; k++, j <<= 1)
      *(s + n - k - 1) = (i & j) ? '1' : '0';
   return s;
} 

char *biglong_hex(char *s, biglong i, int n)
{
static char hexa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                        'a', 'b', 'c', 'd', 'e', 'f' };
long j, k, l;

   l = (n >> 2) + ((n % 4) == 0 ? 0 : 1);
   *(s + l) = '\0'; /* end of string */
   for (j = 0xF, k = 0; k < l; k ++, j <<= 4) {
      if (k == 32) /* reset the mask value when switching word */
         j = 0xF;
      if (k < 32)
         *(s + l - k - 1) = hexa[(i.low & j) >> (k << 2)];
      else
         *(s + l - k - 1) = hexa[(i.high & j) >> (k << 2)];
   }
   return s;
}

char *biglong_bin(char *s, biglong i, int n)
{
long j, k;

   *(s + n) = '\0'; /* end of string */
   for (j = 1, k = 0; k < n; k++, j <<= 1) {
      if (k == 32) /* reset the mask value when switching word */
         j = 1;
      if (k < 32)
         *(s + n - k - 1) = (i.low & j) ? '1' : '0';
      else
         *(s + n - k - 1) = (i.high & j) ? '1' : '0';
   }

   return s;
} 

#define LAMBDA 1
static int area(int nw, int nb) /* feed through not taken into account */
{
long ddx = 318 + 10 * ln2p(nw);

   switch (nw) {
      case 64: return (302 * (ddx + 31 * nb + 8)) * LAMBDA * LAMBDA;
      case 128: return (446 * (ddx + 31 * nb + 8)) * LAMBDA * LAMBDA;
      case 256: return (470 * (ddx + 57 * nb + 7)) * LAMBDA * LAMBDA;
      case 512: return (472 * (ddx + 117 * nb + 7)) * LAMBDA * LAMBDA;
      default: return (((405 * nw) / 512 + 81)
                             * (ddx + 31 * nb + 7)) * LAMBDA * LAMBDA;
   }
}

static void vbe(FILE *f, char *s, int nw, int nb, int zh, int r, int msb, biglong *data)
{
int lnw = ln2p(nw); /* number of address lines */
char t[256], u[256]; /* max word and bit number */
long i, j, space, full = 0;
int hexa = 0;
char prefix = ' ';
char *(*long_string)(), *(*biglong_string)();
biglong v; /* needed to hold temporary static values to be passed to functions */

   for (i = 64; i <= 4096; i <<= 1)
      if (nw == i) { /* it is a power of 2 in the good range */
         full = 1;
         break;
      }
   if (hexa) {
      long_string = long_hex;
      biglong_string = biglong_hex;
      prefix = 'X';
   } else {
      long_string = long_bin;
      biglong_string = biglong_bin;
      prefix = 'B';
   }
   fputs("-- VHDL grog description\n", f);
   fprintf(f, "-- word number      : %d\n", nw);
   fprintf(f, "-- bit number       : %d\n", nb);
   fprintf(f, "-- tri-state output : %s\n", zh ? "yes" : "no");
   fputs("-- package to be used outside of cao-vlsi simulator\n", f);
   fputs("-- LIBRARY pkg;\n", f);
   fputs("-- USE pkg.p6b_pkg.all;\n", f);
   fprintf(f, "\n");
   fprintf(f, "\n");
   fprintf(f, "ENTITY %s IS\n", s);
   fputs(     "  GENERIC (\n", f);
   fprintf(f, "        CONSTANT area : NATURAL := %d\n", area(nw, nb));
   fputs(     "  );\n", f);
   if (!msb)
   fprintf(f, "  PORT( adr  : IN BIT_VECTOR(%d DOWNTO 0);\n", lnw - 1);
   else
   fprintf(f, "  PORT( adr  : IN BIT_VECTOR(0 TO %d);\n", lnw - 1);
   if (nw == 64)
   fprintf(f, "        ck   : IN BIT;\n");
   else if (nw == 128 || nw == 256)
   fprintf(f, "        ck   : IN BIT_VECTOR(0 TO 1);\n");
   else
   fprintf(f, "        ck   : IN BIT_VECTOR(0 TO %d);\n", nw / 1025);
   if (zh)
   fprintf(f, "        oe   : IN BIT;\n");
   fprintf(f, "        f    : OUT BIT_VECTOR(0 TO %d);\n", nb - 1);
   fprintf(f, "        vdd  : IN BIT;\n");
   fprintf(f, "        vss  : IN BIT );\n");
   fprintf(f, "END %s;\n", s);
   fprintf(f, "\n");
   fprintf(f, "\n");
   fprintf(f, "ARCHITECTURE VBE OF %s IS\n", s);
   fprintf(f, "  SIGNAL m_out : BIT_VECTOR (0 TO %d);\n", nb - 1);
   if (zh)
   fprintf(f, "  SIGNAL zh_in : BIT_VECTOR (0 TO %d);\n", nb - 1);
   fprintf(f, "\n");
   fprintf(f, "BEGIN\n");
   if (nw == 64)
   sprintf(t, "0");
   else if (nw == 128 || nw == 256)
   sprintf(t, "00");
   else
   for (t[0] = '\0', i = 0; i < nw / 512; i += 2)
   strcat(t, "0");
   if (!zh) {
   v.low = v.high = nw == 64 || nw == 128 ? ~0 : 0;
   fprintf(f, "   f     <= m_out WHEN (ck = B\"%s\") ELSE\n", t);
   fprintf(f, "            %c\"%s\";\n", prefix, biglong_string(u, v, nb));
   } else {
   v.low = v.high = 0;
   fprintf(f, "   f     <= zh_in WHEN (oe = '1') ELSE\n");
   fprintf(f, "            %c\"%s\";\n", prefix, biglong_string(u, v, nb));
   fprintf(f, "   zh_in <= m_out WHEN (ck = B\"%s\") ELSE\n", t);
   fprintf(f, "            %c\"%s\";\n", prefix, biglong_string(u, v, nb));
   }
   fprintf(f, "\n");
   if (!msb)
   fprintf(f, "   WITH adr(%d DOWNTO 0) SELECT\n", lnw - 1);
   else
   fprintf(f, "   WITH adr(0 TO %d) SELECT\n", lnw - 1);
   if (!r)
   space = fprintf(f, "      m_out(0 TO %d) <= ", nb - 1);
   else
   space = fprintf(f, "      m_out(%d DOWNTO 0) <= ", nb - 1);
   for (i = 0; i < nw; i++)
   if (i == 0)
   fprintf(f, "%c\"%s\" WHEN %c\"%s\",\n",
               prefix, biglong_string(t, data[i], nb),
               prefix, long_string(u, i, lnw));
   else if (i == nw - 1 && full) {
   for (j = 0; j < space; j++)
      putc(' ', f);
   fprintf(f, "%c\"%s\" WHEN %c\"%s\";\n",
               prefix, biglong_string(t, data[i], nb),
               prefix, long_string(u, i, lnw));
   } else {
   for (j = 0; j < space; j++)
      putc(' ', f);
   fprintf(f, "%c\"%s\" WHEN %c\"%s\",\n",
               prefix, biglong_string(t, data[i], nb),
               prefix, long_string(u, i, lnw));
   }
   if (!full) {
   for (j = 0; j < space; j++)
      putc(' ', f);
   fprintf(f, "%c\"%s\" WHEN OTHERS;\n",
                  prefix, long_string(u, 0, nb));
   }

   fprintf(f, "\n");
   fprintf(f, "  ASSERT (vss = '0')\n");
   fprintf(f, "  REPORT   \"Power supply is missing on vss\"\n");
   fprintf(f, "  SEVERITY ERROR;\n");
   fprintf(f, "  ASSERT (vdd = '1')\n");
   fprintf(f, "  REPORT   \"Power supply is missing on vdd\"\n");
   fprintf(f, "  SEVERITY ERROR;\n");
   fprintf(f, "\n");
   fprintf(f, "END VBE;\n");
}

static void pat(FILE *f, char *s, int nw, int nb, int zh, int r, int msb, biglong *data)
{
int lnw = ln2p(nw); /* number of address lines */
char t[256];
int i, j, k, l;
biglong v; /* needed to hold temporary static values to be passed to functions */

   fprintf(f, "# patterns for rom : %s test\n", s);
   if (!msb)
   fprintf(f, "in    adr(%d downto 0);;\n", lnw - 1);
   else
   fprintf(f, "in    adr(0 to %d);;\n", lnw - 1);
   if (nw == 64)
   fprintf(f, "in    ck;;\n");
   else if (nw == 128 || nw == 256)
   fprintf(f, "in    ck(0 to 1);;\n");
   else
   fprintf(f, "in    ck(0 to %d);;\n", nw / 1025);
   if (zh)
   fprintf(f, "in    oe;;\n");
   if (!r)
   fprintf(f, "out   f(0 to %d);;\n", nb - 1);
   else
   fprintf(f, "out   f(%d downto 0);;\n", nb - 1);
   fprintf(f, "in    vdd;;\n");
   fprintf(f, "in    vss;\n");
   fputs("begin\n", f);
   for (l = 0, i = 0; i < nw; i++) {
   for (k = 0; k < 2; k++) { /* clock pulse */
   fprintf(f, "pat%04d : ", l++);
   fprintf(f, "%s ", long_bin(t, i, lnw)); /* adr */
   if (nw == 64)
   fprintf(f, "%d ", k); /* ck */
   else if (nw == 128 || nw == 256)
   fprintf(f, "%d%d ", k, k); /* ck */
   else {
   for (j = 0; j < nw / 512; j += 2)
   fprintf(f, "%d", k);
   fprintf(f, " ");
   }
   if (zh)
   fprintf(f, "%d ", 1); /* zh */
   if (k == 0)
   fprintf(f, "?%s ", biglong_bin(t, data[i], nb)); /* f */
   else {
   v.low = v.high = !zh && (nw == 64 || nw == 128) ? ~0 : 0;
   fprintf(f, "?%s ", biglong_bin(t, v, nb));
   }
   fprintf(f, "%d ", 1); /* vdd */
   fprintf(f, "%d;\n", 0); /* vss */
   }
   }
   fputs("end;\n", f);
}

static void vlg(FILE *f, char *s, int nw, int nb, int zh, int r, int msb, biglong *data)
{
int lnw = ln2p(nw); /* number of address lines */
char t[256], u[256]; /* max word and bit number */
int i, j, space, full = 0;
int hexa = 0;
char cprefix[5], bprefix[5], wprefix[5]; /* clock, bit and word prefixes */
char *(*long_string)(), *(*biglong_string)();
biglong v; /* needed to hold temporary static values passed to functions */

   for (i = 64; i <= 4096; i <<= 1)
      if (nw == i) { /* it is a power of 2 in the good range */
         full = 1;
         break;
      }
   if (hexa) {
      long_string = long_hex;
      biglong_string = biglong_hex;
      sprintf(bprefix, "%d'x", nb/4);
      sprintf(wprefix, "%d'x", nw/4);
   } else {
      long_string = long_bin;
      biglong_string = biglong_bin;
      sprintf(bprefix, "%d'b", nb);
      sprintf(wprefix, "%d'b", lnw);
   }
   fputs("/* Verilog grog description\n", f);
   fprintf(f, "   word number      : %d\n", nw);
   fprintf(f, "   bit number       : %d\n", nb);
   fprintf(f, "   tri-state output : %s\n", zh ? "yes" : "no");
   fprintf(f, "*/\n");
   fprintf(f, "module %s (adr, ck,", s);
   if (zh) fprintf(f, " oe,");
   fprintf(f, " f, vdd, vss);\n");
   if (!msb)
   fprintf(f, "  input [%d:0] adr;\n", lnw - 1);
   else
   fprintf(f, "  input [0:%d] adr;\n", lnw - 1);
   if (nw == 64)
   fprintf(f, "  input ck;\n"), strcpy(cprefix, "1'b");
   else if (nw == 128 || nw == 256)
   fprintf(f, "  input [0:1] ck;\n"), strcpy(cprefix, "2'b");
   else
   fprintf(f, "  input [0:%d] ck;\n", nw / 1025), sprintf(cprefix, "%d'b", nw / 1025);
   if (zh)
   fprintf(f, "  input oe;\n");
   fprintf(f, "  output [0:%d] f;\n", nb - 1);
   fprintf(f, "  input vdd;\n");
   fprintf(f, "  input vss;\n");
   fprintf(f, "\n");

   fprintf(f, "  wire [0:%d] m_out;\n", nb - 1);
   fprintf(f, "  reg [0:%d] iv[0:%d];\n", nb - 1, nw - 1);
   if (zh)
   fprintf(f, "  wire [0:%d] zh_in;\n", nb - 1);
   fprintf(f, "\n");
   if (nw == 64)
   sprintf(t, "0");
   else if (nw == 128 || nw == 256)
   sprintf(t, "00");
   else
   for (t[0] = '\0', i = 0; i < nw / 512; i += 2)
   strcat(t, "0");
   if (!zh) {
   v.low = v.high = nw == 64 || nw == 128 ? ~0 : 0;
   fprintf(f, "   assign f = (ck == 0) ? m_out : %s%s;\n",
/*
              cprefix, biglong_string(t, ~0, nw == 64 ? 1
                                          : (nw == 128 || nw == 256) ? 2 : nw /1025),
*/
              bprefix, biglong_string(u, v, nb));
   } else {
   v.low = v.high = 0; /* Should be 'Z' ? */
   fprintf(f, "   assign f = (oe == 1) ? zh_in : %s%s;\n", bprefix, biglong_string(u, v, nb));
   fprintf(f, "   assign zh_in = (ck == 0) ? m_out : %s%s;\n",
/*
              cprefix, biglong_string(t, ~0, nw/1025),
*/
              bprefix, biglong_string(u, v, nb));
   }
   fprintf(f, "\n");
   fprintf(f, "   assign m_out = iv[adr];\n");
   fprintf(f, "initial\nbegin\n");
   for (i = 0; i < nw; i++)
   if (i == 0)
   fprintf(f, "iv[%d] = %s%s;\n", i,
               bprefix, biglong_string(t, data[i], nb));
   else if (i == nw - 1 && full) {
   for (j = 0; j < space; j++)
      putc(' ', f);
   fprintf(f, "iv[%d] = %s%s;\n", i,
               bprefix, biglong_string(t, data[i], nb));
   } else {
   for (j = 0; j < space; j++)
      putc(' ', f);
   fprintf(f, "iv[%d] = %s%s;\n", i,
               bprefix, biglong_string(t, data[i], nb));
   }
/* Should no enter there as of now, ...
   if (!full) {
   for (j = 0; j < space; j++)
      putc(' ', f);
   fprintf(f, "default:  m_out[%d:%d] = %s%s;\n",
                  r? nb - 1 : 0, r ? 0 : nb - 1,
                  bprefix, long_string(u, 0, nb));
   }
*/

   fprintf(f, "end\n");
   fprintf(f, "endmodule\n");
}
