/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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
 
/*******************************************************************************
* version 1.1 : mbk2mg or how to translate symbolic leaf cells to modgen format*
* Written by Frederic Petrot, to be used on leafs cells for idps generators    *
* This update uses macros for contacts and transistors as described in         *
* idps.atoms                                                                   *
*                                                                              *
* version 1.2 : now includes instances also                                    *
*                                                                              *
* e-mail : cao-vlsi@masi.ibp.fr                                                *
*                                                                              *
* version : 1.2                                                                *
* date    : 22/01/92                                                           *
*******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <mut.h>
#include <mph.h>

static float RTrans = 1.5; 	/* o_ps_aa : Poly overlap on Dif */
static float MWTrans = 1.0;	/* w_ps    : Min transistor width */

static int number;

static int Ext[LAST_LAYER + 1] = {
	/*	NWELL	 */ 0,
	/*	PWELL	 */ 0,
	/*	NTIE	 */ 1,
	/*	PTIE	 */ 1,
	/*	NDIF	 */ 1,
	/*	PDIF	 */ 1,
	/*	NTRANS */ 0, /* meant for N transistor grid */
	/*	PTRANS */ 0, /* meant for P transistor grid */
	/*	POLY	 */ 1,
	/*	ALU1	 */ 1,
	/*	ALU2	 */ 2,
	/*	ALU3	 */ 2,
	/*	TPOLY	 */ 1,
	/*	TALU1	 */ 1,
	/*	TALU2	 */ 2,
	/*	TALU3	 */ 2 };

/*******************************************************************************
* Macro                                                                        *
*******************************************************************************/
#define HALF(X)	((X) >> 1)

/*******************************************************************************
* Functions declaration                                                        *
*******************************************************************************/
static void segments();
static void connectors();
static void virtualconnectors();
static void vias();
static void instances();
static void abutmentbox();
static void figure();

/*******************************************************************************
* Layers, Vias, Orientation & Gemetrical operation translation table           *
*******************************************************************************/
static char *
	busname(name)
char *name;
{
static char buffer[255];
char *s, *t;
char one = 1;

	if (!name || *name == '*')
		return NULL;

	s = name;
	t = buffer;
	while (*s) {
      if (*s == ' ' || (*s == '_' && !one)) {
        if (one) {
          *t++ = '[';
          s++;
          one = 0;
        } else {
          *t++ = ']';
          *t++ = '[';
          s++;
        }
      }
      if ((*s == '/' || *s == '_') && !one) {
        *t++ = ']';
        one = 1;
      }
      *t++ = *s++;
	}
	if (!one)
		*t++ = ']';
	*t = '\0';
	return buffer;
}

static char *
layer(mbk_layer)
char mbk_layer;
{
static char *mg_layer[] = {
	/* NWELL  */ "nw",
	/* PWELL  */ "pw",
	/* NTIE   */ "xn",
	/* PTIE   */ "xp",
	/* NDIF   */ "nn",
	/* PDIF   */ "pp",
	/* NTRANS */ "NULL",
	/* PTRANS */ "NULL",
	/* POLY   */ "ps",
	/* ALU1   */ "m1",
	/* ALU2   */ "m2",
	/* ALU3   */ "NULL",
	/* TPOLY  */ "NULL",
	/* TALU1  */ "am1",
	/* TALU2  */ "am2",
	/* TALU3  */ "NULL"
	};
	return mg_layer[(int)mbk_layer]; 	
}

static char *
	via(mbk_via)
char mbk_via;
{
static char *mg_via[] = {
	/* CONT_POLY   */ "cops",
	/* CONT_VIA    */ "pvia",
	/* CONT_DIFN   */ "con",
	/* CONT_DIFP   */ "cop",
	/* CONT_BODY_N */ "conw",
	/* CONT_BODY_P */ "copw",
	/* C_X_N       */ "cxn",
	/* C_X_P       */ "cxp",
	/* CONT_VIA2   */ "NULL"
	};
	return mg_via[(int)mbk_via]; 	
}

static char *
	orient(face)
char face;
{
	switch (face) {
		case NORTH :
			return "top";
		case SOUTH : 
			return "bottom";
		case EAST : 
			return "right";
		case WEST : 
			return "left";
		default : 
			return "allside";
	}
}

static char *
	geop(trsf, s)
char trsf;
char **s;
{

	switch (trsf) {
		case NOSYM :
			*s = "ll";
			return NULL;
		case SYM_X : 
			*s = "lr";
			return "my";
		case SYM_Y : 
			*s = "ul";
			return "mx";
		case SYMXY : 
			*s = "ur";
			return "mx my";
		case ROT_P :
			*s = "ul";
			return "r 90";
		case ROT_M : 
			*s = "lr";
			return "r 270";
		case SY_RP : 
			*s = "ll";
			return "mx r 90";
		case SY_RM : 
			*s = "ur";
			return "mx r 270";
		default : 
			fflush(stdout);
			fprintf(stderr, "Unknown geometrical operation\n");
			exit(1);
	}
}

void
	mgnsavephfig(Cell)
phfig_list *Cell;
{
chain_list *Model;
char *CellName;
int i;
FILE *file;
static int notfirst;

	if (!notfirst) {
		for (i = 0; i < LAST_LAYER + 1; i++)
			Ext[i] = HALF(Ext[i] * SCALE_X);
		notfirst++;
	}

	CellName = Cell->NAME;
	if ((file = mbkfopen(CellName, "mg", WRITE_TEXT)) == NULL) {
		fflush(stdout);
		fprintf(stderr, "Can't open %s.%s for writing\n", CellName, "mg");
		exit(1);
	}
	Model = Cell->MODELCHAIN;

	fputs("/", file);
	for (i = 1; i < 80; i++)
		fputs("*", file);
	fputs("\n", file);
	i = fprintf(file, "* file %s.mg has been generated by mbk2mg translator", 
						CellName);
	for (; i < 79; i++)
		fputs(" ", file);
	fputs("*\n", file);
	i = fprintf(file,
		"* for cao-vlsi generators in order to fit IDPS standard modgen");
	for (; i < 79; i++)
		fputs(" ", file);
	fputs("*\n", file);
	for (i = 0; i < 79; i++)
		fputs("*", file);
	fputs("/\n", file);
	fprintf(file, "#ifndef BULLATOMS\n");
	fprintf(file, "#define BULLATOMS\n");
	fprintf(file, "#include \"%s/tech/bull.atoms\"\n", mbkgetenv("MODGEN"));
	fprintf(file, "#endif\n");

	while (Model) {
		fprintf(file, "#include \"%s\"\n",
					filepath((char *)Model->DATA, "mg"));
		Model = Model->NEXT;
	}
	/* last figure :
	   this is the real one, boys. */
	figure(file, Cell);
}

static void
	rect(file, mg_layer, x0, y0, x1, y1)
FILE *file;
char *mg_layer;
long x0, y0, x1, y1;
{

	if (!strcmp(mg_layer, "NULL"))
		return;
	fprintf(file, "\tra(%s, %.2f, %.2f, %.2f, %.2f);\n",
				mg_layer, (float)x0 / SCALE_X, (float)x1 / SCALE_X,
				(float)y0 / SCALE_X, (float)y1 / SCALE_X);
}

static void
	figure(file, Cell)
FILE *file;
phfig_list *Cell;
{
	fprintf(file, "\ncell %s\n{\n", Cell->NAME);
	abutmentbox(file, Cell);
	connectors(file, Cell->PHCON);
	virtualconnectors(file, Cell->PHREF);
	instances(file, Cell->PHINS);
	segments(file, Cell->PHSEG);
	vias(file, Cell->PHVIA);
	fputs("}\n", file);
}

static void
	abutmentbox(file, Cell)
FILE *file;
phfig_list *Cell;
{
	rect(file, "zd", Cell->XAB1, Cell->YAB1, Cell->XAB2, Cell->YAB2);
	fprintf(file, "\tterminal ll (WIDTH = 1.00) left zd at (%.2f, %.2f);\n",
				(float)Cell->XAB1/SCALE_X, (float)Cell->YAB1/SCALE_X);
	fprintf(file, "\tterminal lr (WIDTH = 1.00) right zd at (%.2f, %.2f);\n",
				(float)Cell->XAB2/SCALE_X, (float)Cell->YAB1/SCALE_X);
	fprintf(file, "\tterminal ul (WIDTH = 1.00) left zd at (%.2f, %.2f);\n",
				(float)Cell->XAB1/SCALE_X, (float)Cell->YAB2/SCALE_X);
	fprintf(file, "\tterminal ur (WIDTH = 1.00) right zd at (%.2f, %.2f);\n",
				(float)Cell->XAB2/SCALE_X, (float)Cell->YAB2/SCALE_X);
}

static void
	segments(file, HeadOfList)
FILE *file;
phseg_list	*HeadOfList;
{
register phseg_list *Ptr;
register long Xbl, Ybl, Xtr, Ytr;	/* bl : bottom left, tr : top right */
register long Twidth, Tlenght;
register long Xcenter, Ycenter;

	for (Ptr = HeadOfList; Ptr; Ptr = Ptr->NEXT) {
		if (Ptr->TYPE == LEFT || Ptr->TYPE == RIGHT) {
			switch (Ptr->LAYER) {
				case NTRANS :
				case PTRANS :
					Xbl = Ptr->X1;
					Ybl = Ptr->Y1;
					Xtr = Ptr->X2;
					Twidth = Xtr - Xbl - 2 * (int)((RTrans) * SCALE_X);
					Tlenght = MWTrans * Ptr->WIDTH;
					Xcenter = Xbl + HALF(Xtr - Xbl);
					Ycenter = Ybl;

					(void)fprintf(file,"\tplace x%x$ = ", number++);
					(void)fprintf(file,"%s(%.2f, %.2f) ", 
										Ptr->LAYER == NTRANS ? "ntr" : "ptr",
										(float)Twidth / SCALE_X,
										(float)Tlenght / SCALE_X);
					(void)fprintf(file,"at (%.2f,%.2f);\n",  
										(float)Xcenter / SCALE_X,
										(float)Ycenter / SCALE_X);
					break;
				case TPOLY :
				case TALU3 :
				case ALU3 :
					break;
				default :
					(void)fprintf(file,
							"\tstretch(%s, %.2f, %s, %.2f, %.2f, %.2f, %.2f);\n",
								layer(Ptr->LAYER),
								(float)Ptr->WIDTH / SCALE_X, "hor",
								(float)(Ptr->X1 - Ext[(int)Ptr->LAYER]) / SCALE_X,
								(float)Ptr->Y1 / SCALE_X,
								(float)(Ptr->X2 + Ext[(int)Ptr->LAYER]) / SCALE_X,
								(float)Ptr->Y2 / SCALE_X); 
			}
		} else {
			switch (Ptr->LAYER) {
				case NTRANS :
				case PTRANS :
					Xbl = Ptr->X1;
					Ybl = Ptr->Y1;
					Ytr = Ptr->Y2;
					Twidth = Ytr - Ybl - 2 * (int)((RTrans) * SCALE_X);
					Tlenght = MWTrans * Ptr->WIDTH;
					Xcenter = Xbl;
					Ycenter = Ybl + HALF(Ytr - Ybl);

					(void)fprintf(file,"\tplace x%x$ = ", number++);
					(void)fprintf(file,"%s(%.2f, %.2f) r 90 ", 
										Ptr->LAYER == NTRANS ? "ntr" : "ptr",
										(float)Twidth / SCALE_X,
										(float)Tlenght / SCALE_X);
					(void)fprintf(file,"at (%.2f,%.2f);\n",  
										(float)Xcenter / SCALE_X, 
										(float)Ycenter / SCALE_X);
					break;
				case TPOLY :
				case TALU3 :
				case ALU3 :
					break;
				default :
					(void)fprintf(file,
							"\tstretch(%s, %.2f, %s, %.2f, %.2f, %.2f, %.2f);\n",
								layer(Ptr->LAYER), (float)Ptr->WIDTH/SCALE_X, "ver",
								(float)Ptr->X1 / SCALE_X,
								(float)(Ptr->Y1 - Ext[(int)Ptr->LAYER]) / SCALE_X,
								(float)Ptr->X2 / SCALE_X,
								(float)(Ptr->Y2 + Ext[(int)Ptr->LAYER]) / SCALE_X); 
			}
		}
	}
}

static void
	connectors(file, HeadOfList)
FILE *file;
phcon_list *HeadOfList;
{
register phcon_list *pt;

	for (pt = HeadOfList; pt; pt = pt->NEXT) {
	char *l = layer(pt->LAYER);
		if (!strcmp("NULL", l))
			continue;
		/* text because we don't know how to print anything */
		fprintf(file, "\ttext %s \"%s\" ", l, busname(pt->NAME));
  		fprintf(file, "at (%.2f, %.2f);\n",
					(float)pt->XCON/SCALE_X, (float)pt->YCON/SCALE_X);
		fprintf(file, "\tterminal %s[%ld] ", busname(pt->NAME), pt->INDEX);
 		fprintf(file, "(WIDTH = %.2f) %s %s ",
				(float)pt->WIDTH/SCALE_X, orient(pt->ORIENT), l);
  		fprintf(file, "at (%.2f, %.2f);\n",
					(float)pt->XCON/SCALE_X, (float)pt->YCON/SCALE_X);
	}
}

static void
virtualconnectors(file, HeadOfList)
FILE *file;
phref_list *HeadOfList;
{
register phref_list *pt;
static int refindex;

	for (pt = HeadOfList; pt; pt = pt->NEXT) {
		/* text because we don't know how to print anything */
		(void)fprintf(file, "\ttext zd \"%s\" ", busname(pt->NAME));
  		(void)fprintf(file, "at (%.2f, %.2f);\n",
									(float)pt->XREF / SCALE_X,
									(float)pt->YREF / SCALE_X);
		/* object itself */
		(void)fprintf(file,"\tplace %s[%d] = ", busname(pt->NAME), refindex++);
		(void)fprintf(file,"%s at (%.2f,%.2f);\n", pt->FIGNAME,
									(float)pt->XREF / SCALE_X,
									(float)pt->YREF / SCALE_X);
	}
}

static void
	vias(file, HeadOfViasList)
FILE *file;
phvia_list *HeadOfViasList;
{
#define SIZE_C_X (5 * SCALE_X)
register phvia_list *ViaPtr;

	for (ViaPtr = HeadOfViasList; ViaPtr; ViaPtr = ViaPtr->NEXT) {
		switch (ViaPtr->TYPE) {
#ifdef NOTLABOTEC
			case C_X_N :
				rect(file, layer(POLY),
					ViaPtr->XVIA - HALF(SCALE_X), ViaPtr->YVIA - HALF(SCALE_X),
					ViaPtr->XVIA + HALF(SCALE_X), ViaPtr->YVIA + HALF(SCALE_X));
				rect(file, layer(NDIF),
					ViaPtr->XVIA - HALF(SIZE_C_X), ViaPtr->YVIA - HALF(SIZE_C_X),
					ViaPtr->XVIA + HALF(SIZE_C_X), ViaPtr->YVIA + HALF(SIZE_C_X));
				continue;
			case C_X_P  :
				rect(file, layer(POLY),
					ViaPtr->XVIA - HALF(SCALE_X), ViaPtr->YVIA - HALF(SCALE_X),
					ViaPtr->XVIA + HALF(SCALE_X), ViaPtr->YVIA + HALF(SCALE_X));
				rect(file, layer(PDIF),
					ViaPtr->XVIA - HALF(SIZE_C_X), ViaPtr->YVIA - HALF(SIZE_C_X),
					ViaPtr->XVIA + HALF(SIZE_C_X), ViaPtr->YVIA + HALF(SIZE_C_X));
				continue;
#endif
			default :
				(void)fprintf(file,"\tplace %s%d = ", via(ViaPtr->TYPE), number++);
				(void)fprintf(file,"%s at (%.2f,%.2f);\n", via(ViaPtr->TYPE),
									(float)ViaPtr->XVIA / SCALE_X,
									(float)ViaPtr->YVIA / SCALE_X);
		}
	}
}

static void
	instances(file, List)
FILE *file;
phins_list *List;
{
register phins_list *insPtr;
char *trsf, *corner;

	for (insPtr = List; insPtr; insPtr = insPtr->NEXT) {
		trsf = geop(insPtr->TRANSF, &corner);
		(void)fprintf(file,"\tplace %s.%s = ", insPtr->INSNAME, corner);
		if (trsf)
			(void)fprintf(file,"%s %s ", insPtr->FIGNAME, trsf);
		else
			(void)fprintf(file,"%s ", insPtr->FIGNAME);
		(void)fprintf(file,"at (%.2f,%.2f);\n",
							(float)insPtr->XINS / SCALE_X,
							(float)insPtr->YINS / SCALE_X);
	}
} 
