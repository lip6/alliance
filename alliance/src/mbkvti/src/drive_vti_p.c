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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
 
/*******************************************************************************
* mbk : physical vti v7r5 & v8 driver (cp format)                              *
*                                                                              *
* version : 3.05                                                               *
* date    : 13/05/92                                                           *
*                                                                              *
* Modified, rewritten and maintained by Frederic Petrot since september 1990   *
* $Log: drive_vti_p.c,v $
* Revision 1.3  2002/09/30 16:21:00  czo
* support/users
*
* Revision 1.2  2002/03/14 12:34:30  fred
* Using new headers
*                                                                        *
*******************************************************************************/

#ident "@(#)vti symbolic layout view driver version 4.00"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <mut.h>
#include <mph.h>
#include <mpu.h>
#include "drive_vti_p.h"

/*******************************************************************************
* function getdate()                                                           *
*******************************************************************************/
static void vtigetdate(date)
char *date;
{
time_t timer;
char day[4], month[4];
int year, nday, hour, minute, second;

	(void)time(&timer);
	(void)strcpy(date, ctime(&timer));
	(void)sscanf(date, "%s %s %d %d:%d:%d 19%d",
						day, month, &nday, &hour, &minute, &second, &year);
	(void)sprintf(date, "%02d-%s-%02d %02d:%02d",
						nday, month, year, hour, minute);
}

/*******************************************************************************
* function busname                                                             *
*******************************************************************************/
static char *busname(name)
char *name;
{
static char buffer[255];
char *s, *t;
char one = 1;

	if (!name)
		return NULL;

	s = name;
	t = buffer;
	while (*s) {
		if (*s == ' ')
			if (one) {
				*t++ = '[';
				s++;
				one = 0;
			} else {
				*t++ = ']';
				*t++ = '[';
				s++;
			}
		if (*s == '_' && !one) { /* was SEPAR and not / */
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

/*******************************************************************************
* function vtisavephfig()                                                      *
*******************************************************************************/
void vtisavephfig(ptfig)
phfig_list *ptfig;
{
chain_list *ptchain;
phcon_list *ptcon;
phins_list *ptins;
phseg_list *ptseg;
phvia_list *ptvia;
phref_list *ptref;
int npoint = 0;
int vix[LAST_CONTACT];
int conindex = 0;                                      /* connector index    */
char symm;
char type;                                             /* connector orient   */
char *filename = ptfig->NAME; /* namealloc unnecessary */
char figname[10];
char insname[10];
FILE *ptfile;
float f1, f2, f3, f4, f5;
char date[30];

	/* open file */
	if ((ptfile = mbkfopen(filename, OUT_PH, WRITE_TEXT)) == (FILE *)NULL) {
		(void)fflush(stdout);
		(void)fprintf(stderr, "*** mbk error ***\n");
		(void)fprintf(stderr, "vtisavephfig can't open file %s.%s\n",
							filename, OUT_PH);
		EXIT(1);
	} else if (TRACE_MODE == 'Y')
		(void)printf("--- mbk ---  writing file %s.%s\n", filename, OUT_PH);

   (void)memset(vix, (char)0, sizeof(vix));

	/* header */
	vtigetdate(date);
	(void)fprintf(ptfile, "#cell1 %s %s compose *\n", ptfig->NAME, TECHNO);
	(void)fprintf(ptfile, "# %s %s mbkvti400 * .cp\n", date, date);
	for (ptchain = ptfig->MODELCHAIN; ptchain; ptchain = ptchain->NEXT)
		(void)fprintf(ptfile, "# %s\n", (char *)ptchain->DATA);
	(void)fprintf(ptfile, "# .\n");
	(void)fprintf(ptfile, "V 4 VTIcompose 1.1\n");

	/* boxes */
	f1 = (float)ptfig->XAB1 / SCALE_X;
	f2 = (float)ptfig->YAB1 / SCALE_X;
	f3 = (float)ptfig->XAB2 / SCALE_X;
	f4 = (float)ptfig->YAB2 / SCALE_X;
	(void)fprintf(ptfile, "A %g %g %g %g\n", f1, f2, f3, f4);
	(void)fprintf(ptfile, "B %g %g %g %g\n", f1, f2, f3, f4);

	(void)fprintf(ptfile, "F F\n");

	/* connectors */
	for (ptcon = ptfig->PHCON; ptcon; ptcon = ptcon->NEXT) {
		switch (ptcon->ORIENT) {
			case NORTH :
				type = 'N';
				break;
			case EAST :
				type = 'E';
				break;
			case SOUTH :
				type = 'S';
				break;
			case WEST :
				type = 'W';
				break;
			default :
				break;
		}
#if 0
		if (ptcon->XCON != ptfig->XAB1 &&  ptcon->XCON != ptfig->XAB2
				&& ptcon->YCON != ptfig->YAB1 &&  ptcon->YCON != ptfig->YAB2) {
			(void)fprintf(stderr, "*** mbk warning ***\n");
			(void)fprintf(stderr, "vti connector not on abutment box : ");
			(void)fprintf(stderr, "x = %d , y = %d , name = %s , orient = %c\n",
								ptcon->XCON, ptcon->YCON,
			     ptcon->NAME, ptcon->ORIENT);
			(void)fprintf(stderr, "writing the file %s.%s\n", filename, OUT_PH);
		}
#endif
		f1 = (float)ptcon->XCON / SCALE_X;
		f2 = (float)ptcon->YCON / SCALE_X;
		f3 = (float)ptcon->WIDTH / SCALE_X;
		(void)fprintf(ptfile, "C %s %g %g %s %g %d *",
							busname(ptcon->NAME), f1, f2, decodelayer(ptcon->LAYER),
							f3, ++conindex);
		(void)fprintf(ptfile, " * %c %s\n", type, busname(ptcon->NAME));
	}

	/* instances */
	for (ptins = ptfig->PHINS; ptins; ptins = ptins->NEXT) {
		switch (ptins->TRANSF) {
			case NOSYM :
				symm = 0;
				break;
			case SYM_Y :
				symm = 6;
				break;
			case SYM_X :
				symm = 4;
				break;
			case SYMXY :
				symm = 2;
				break;
			case ROT_P :
				symm = 1;
				break;
			case ROT_M :
				symm = 3;
				break;
			case SY_RM :
				symm = 5;
				break;
			case SY_RP :
				symm = 7;
				break;
			default :
				symm = -15;
		}
		f1 = (float)ptins->XINS / SCALE_X;
		f2 = (float)ptins->YINS / SCALE_X;
		(void)fprintf(ptfile, "I %s %g %g %d \"%s\" cp * *\n",
							busname(ptins->INSNAME), f1, f2, symm, ptins->FIGNAME);
	}

	/* contacts */
	for (ptvia = ptfig->PHVIA; ptvia; ptvia = ptvia->NEXT) {
		switch (ptvia->TYPE) {
			case CONT_VIA :
				(void)strcpy(figname, "VIA12");
				(void)sprintf(insname, "via12x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA2 :
				(void)strcpy(figname, "VIA23");
				(void)sprintf(insname, "via23x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA3 :
				(void)strcpy(figname, "VIA34");
				(void)sprintf(insname, "via34x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA4 :
				(void)strcpy(figname, "VIA45");
				(void)sprintf(insname, "via45x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA5 :
				(void)strcpy(figname, "VIA56");
				(void)sprintf(insname, "via56x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA6 :
				(void)strcpy(figname, "VIA67");
				(void)sprintf(insname, "via67x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA7 :
				(void)strcpy(figname, "VIA78");
				(void)sprintf(insname, "via78x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_VIA8 :
				(void)strcpy(figname, "VIA89");
				(void)sprintf(insname, "via89x%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_POLY :
				(void)strcpy(figname, "CPF");
				(void)sprintf(insname, "cpf%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_POLY2 :
				(void)strcpy(figname, "CPF2");
				(void)sprintf(insname, "cpf2%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_DIF_N :
				(void)strcpy(figname, "CDN");
				(void)sprintf(insname, "cdn%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_DIF_P :
				(void)strcpy(figname, "CDP");
				(void)sprintf(insname, "cdp%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_BODY_N :
				(void)strcpy(figname, "CBN");
				(void)sprintf(insname, "cbn%d", ++vix[(int)ptvia->TYPE]);
				break;
			case CONT_BODY_P :
				(void)strcpy(figname, "CBP");
				(void)sprintf(insname, "cbp%d", ++vix[(int)ptvia->TYPE]);
				break;
			case C_X_N :
				(void)strcpy(figname, "CXN");
				(void)sprintf(insname, "cxn%d", ++vix[(int)ptvia->TYPE]);
				break;
			case C_X_P :
				(void)strcpy(figname, "CXP");
				(void)sprintf(insname, "cxp%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN1 :
				(void)strcpy(figname, "TURN1");
				(void)sprintf(insname, "turn1x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN2 :
				(void)strcpy(figname, "TURN2");
				(void)sprintf(insname, "turn2x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN3 :
				(void)strcpy(figname, "TURN3");
				(void)sprintf(insname, "turn3x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN4 :
				(void)strcpy(figname, "TURN4");
				(void)sprintf(insname, "turn4x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN5 :
				(void)strcpy(figname, "TURN5");
				(void)sprintf(insname, "turn5x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN6 :
				(void)strcpy(figname, "TURN6");
				(void)sprintf(insname, "turn6x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN7 :
				(void)strcpy(figname, "TURN7");
				(void)sprintf(insname, "turn7x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN8 :
				(void)strcpy(figname, "TURN8");
				(void)sprintf(insname, "turn8x%d", ++vix[(int)ptvia->TYPE]);
				break;
                        case CONT_TURN9 :
				(void)strcpy(figname, "TURN9");
				(void)sprintf(insname, "turn9x%d", ++vix[(int)ptvia->TYPE]);
				break;
		}
		f1 = (float)ptvia->XVIA / SCALE_X;
		f2 = (float)ptvia->YVIA / SCALE_X;
                if ( ( ptvia->DX == 0 ) &&
                     ( ptvia->DY == 0 ) )
                {
		  (void)fprintf(ptfile, "I %s %g %g %d \"%s\" ly * *\n",
			insname, f1, f2, NOSYM, namealloc( figname ) );
                }
                else
                {
                  f3 = (float)ptvia->DX / SCALE_X;
                  f4 = (float)ptvia->DY / SCALE_X;

		  (void)fprintf(ptfile, "D %g %g * %s 0 %g %g\n", f1, f2, figname, f3, f4 );
		  ++npoint;
                }
	}

	/* references */
	for (ptref = ptfig->PHREF; ptref; ptref = ptref->NEXT) {
		f1 = (float)ptref->XREF / SCALE_X;
		f2 = (float)ptref->YREF / SCALE_X;
		(void)fprintf(ptfile, "I %s %g %g %d \"%s\" ly * *\n",
		busname(ptref->NAME), f1, f2, NOSYM, ptref->FIGNAME);
	}

	/* segments */
	for (ptseg = ptfig->PHSEG; ptseg; ptseg = ptseg->NEXT) {
		f1 = (float)ptseg->X1 / SCALE_X;
		f2 = (float)ptseg->Y1 / SCALE_X;
		f3 = (float)ptseg->X2 / SCALE_X;
		f4 = (float)ptseg->Y2 / SCALE_X;
		f5 = (float)ptseg->WIDTH / SCALE_X;
		if (ptseg->TYPE == UP || ptseg->TYPE == RIGHT ) {
			(void)fprintf(ptfile, "P %g %g * %s\n", f1, f2,
								decodelayer(ptseg->LAYER));
			(void)fprintf(ptfile, "P %g %g * %s\n", f3, f4,
								decodelayer(ptseg->LAYER));
		} else {
			(void)fprintf(ptfile, "P %g %g * %s\n", f3, f4,
								decodelayer(ptseg->LAYER));
			(void)fprintf(ptfile, "P %g %g * %s\n", f1, f2,
								decodelayer(ptseg->LAYER));
		}
		if (ptseg->NAME == NULL || *ptseg->NAME == '*')
			(void)fprintf(ptfile, "W %g %g %g %g\n",
								f1 - f5, f2 - f5, f3 + f5, f4 + f5);
		else
			(void)fprintf(ptfile, "W %g %g %g %g %s\n",
								f1 - f5, f2 - f5, f3 + f5, f4 + f5,
								busname(ptseg->NAME));
		++npoint;
		(void)fprintf(ptfile, "S %g %s %s P %d P %d\n", f5,
							(ptseg->TYPE == UP || ptseg->TYPE == DOWN) ? "V" : "H",
							decodelayer(ptseg->LAYER), npoint, npoint + 1);
		++npoint;
	}

	/* end */
	(void)fprintf(ptfile, "E\n");

	/* close file */
	if (fclose(ptfile)) {
		(void)fflush(stdout);
		(void)fprintf(stderr, "*** mbk error ***\n");
		(void)fprintf(stderr, "vtisavephfig can't close file %s.%s\n",
							filename, OUT_PH);
		EXIT(1);
	}
}

/*******************************************************************************
* function decodelayer()                                                       *
*******************************************************************************/
static char *decodelayer(index)
char index;
{
   switch(index) {
      case NWELL:
         return "nwell";
      case PWELL:
         return "pwell";
      case NTIE:
         return "ntie";
      case PTIE:
         return "ptie";
      case NDIF:
         return "ndif";
      case PDIF:
         return "pdif";
      case NTRANS:
         return "ntrans";
      case PTRANS:
         return "ptrans";
      case NTRANS_FAST:
         return "ntrans_fast";
      case PTRANS_FAST:
         return "ptrans_fast";
      case NTRANS_HVIO:
         return "ntrans_hvio";
      case PTRANS_HVIO:
         return "ptrans_hvio";
      case POLY:
         return "poly";
      case POLY2:
         return "poly2";
      case ALU1:
         return "metal";
      case ALU2:
         return "metal2";
      case ALU3:
         return "metal3";
      case ALU4:
         return "metal4";
      case ALU5:
         return "metal5";
      case ALU6:
         return "metal6";
      case ALU7:
         return "metal7";
      case ALU8:
         return "metal8";
      case ALU9:
         return "metal9";
      case TALU1:
         return "allowM";
      case TALU2:
         return "allowM2";
      case TALU3:
         return "allowM3";
      case TALU4:
         return "allowM4";
      case TALU5:
         return "allowM5";
      case TALU6:
         return "allowM6";
      case TALU7:
         return "allowM7";
      case TALU8:
         return "allowM8";
      case TALU9:
         return "allowM9";
      case CALU1:
         return "conM";
      case CALU2:
         return "conM2";
      case CALU3:
         return "conM3";
      case CALU4:
         return "conM4";
      case CALU5:
         return "conM5";
      case CALU6:
         return "conM6";
      case CALU7:
         return "conM7";
      case CALU8:
         return "conM8";
      case CALU9:
         return "conM9";
      default:
         return "unknown";
   }
}
