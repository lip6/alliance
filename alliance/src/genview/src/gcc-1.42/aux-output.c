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
 * Description de la machine virtuelle
 *
 * Ecrit par Arnaud COMPAN & Francois PECHEUX ** avril-juin 1990
 * Avec quelques modifications par Frederic Petrot.
 */

#ifndef FILE
#include <stdio.h>
#endif

#define AT_SP(mode) (gen_rtx (MEM, (mode), stack_pointer_rtx))
#define AT_BP(mode) (gen_rtx (MEM, (mode), frame_pointer_rtx))

#define RET return ""

#define RETCOM(X) return ""

extern FILE *asm_out_file;
static char *singlemove_string ();
static void output_movf ();
static void replace_float_constant ();
static int mentions_fp_top ();
static int call_top_dead_p ();
static int fp_top_dead_p1 ();
static rtx via_memory ();
static void output_asm_insn_double_reg_op ();

#define PRINT_REG(X, CODE, FILE) fprintf (FILE, "%s", reg_name[REGNO (X)])

notice_update_cc(exp)
rtx exp;
{
	if (GET_CODE (exp) == SET) {
		if (SET_DEST (exp) == pc_rtx)
			return;
		if (REG_P (SET_DEST (exp))
				&& (REG_P (SET_SRC (exp)) || GET_CODE (SET_SRC (exp)) == MEM)) {
			if (cc_status.value1
					&& reg_overlap_mentioned_p (SET_DEST (exp), cc_status.value1))
				cc_status.value1 = 0;
			if (cc_status.value2
					&& reg_overlap_mentioned_p (SET_DEST (exp), cc_status.value2))
				cc_status.value2 = 0;
			return;
		}
		if (GET_CODE (SET_DEST (exp)) == MEM && REG_P (SET_SRC (exp))) {
			if (cc_status.value1 && GET_CODE (cc_status.value1) == MEM)
					cc_status.value1 = 0;
			if (cc_status.value2 && GET_CODE (cc_status.value2) == MEM)
					cc_status.value2 = 0;
			return;
		} else if (GET_CODE (SET_SRC (exp)) == CALL) {
			CC_STATUS_INIT;
			return;
		} else if (SET_DEST (exp) == cc0_rtx) {
			CC_STATUS_INIT;
			cc_status.value1 = SET_SRC (exp);
			return;
		} else if (GET_MODE (SET_SRC (exp)) == SImode)
			switch (GET_CODE (SET_SRC (exp))) {
				case ASHIFTRT:
				case LSHIFTRT:
				case ASHIFT:
				case LSHIFT:
					if (GET_CODE (XEXP (SET_SRC (exp), 1)) != CONST_INT) {
						CC_STATUS_INIT;
						break;
					}
				case PLUS:
				case MINUS:
				case NEG:
				case AND:
				case IOR:
				case XOR:
					cc_status.flags = CC_NO_OVERFLOW;
					cc_status.value1 = SET_SRC (exp);
					cc_status.value2 = SET_DEST (exp);
					break;
				default:
					CC_STATUS_INIT;
			}
		else
			CC_STATUS_INIT;
	} else if (GET_CODE (exp) == PARALLEL
					&& GET_CODE (XVECEXP (exp, 0, 0)) == SET) {
		if (SET_DEST (XVECEXP (exp, 0, 0)) == pc_rtx)
			return;
		if (SET_DEST (XVECEXP (exp, 0, 0)) == cc0_rtx) {
			CC_STATUS_INIT;
			cc_status.value1 = SET_SRC (XVECEXP (exp, 0, 0));
			return;
		}
		CC_STATUS_INIT;
	} else
		CC_STATUS_INIT;
}

output_ascii(file, p, size)
FILE *file;
char *p;
int size;
{
int i;

	fprintf (file, "\t.string \"");
	for (i = 0; i < size; i++) {
	register int c = p[i];
		if (c == '\"' || c == '\\')
			putc ('\\', file);
		if (c >= ' ' && c < 0177)
			putc (c, file);
		else {
			fprintf (file, "\\%03o", c);
			if (i < size - 1 && p[i + 1] >= '0' && p[i + 1] <= '9')
				fprintf (file, "\"\n\tstring \"");
		}
	}
	fprintf (file, "\"\n");
}

int ever_good(op, mode)
register rtx op;
enum machine_mode mode;
{
	return 1;
}

int no_good(op, mode)
register rtx op;
enum machine_mode mode;
{
	return 0;
}
