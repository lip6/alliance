/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
* mbk : logical vti v4r11 driver (hns, fne, hdn, fdn formats)                  *
*                                                                              *
* version : 4.11                                                               *
* date    : 12/06/1998                                                         *
*                                                                              *
* Modified, rewritten and maintained by Frederic Petrot since september 1990   *
* Modified by Gregoire Avot : RC wire support.                                 *
*******************************************************************************/

#define RCN_FLAG_USER_27 ((unsigned char) 0x00000020)
#ident "@(#)vti logical views driver version 8.00"

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "drive_vti_l.h"
#include MUT_H
#include MLO_H
#include MLU_H
#include RCN_H

/*******************************************************************************
* needed to allow edition with vi on big interfaces instances                  *
*******************************************************************************/
#define LINEBREAK if (linelength > 70) { \
							(void)fputs("-\n", ptfile); \
							linelength = 0; \
						}

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
static int sigindex=0;
char *s, *t;
char one = 1;

	if (!name) {
		sprintf(buffer,"SIG_%d",sigindex++);
		return buffer;
        }

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
		if (*s == SEPAR && !one) {
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
* function env()                                                               *
*******************************************************************************/
static void env(tn, tp)
char *tn, *tp;
{
char *s, *mbkgetenv(); /* what include is needed? stdlib.h but no on pc */

	s = mbkgetenv("MBK_VTI_TN");
	*tn = s ? *s : 'e';
	if (islower(*tn))
		*tn = toupper(*tn);
	s = mbkgetenv("MBK_VTI_TP");
	*tp = s ? *s : 'p';
	if (islower(*tp))
		*tp = toupper(*tp);
}

void	wire_point(ptsig,node,ptfile)
losig_list	*ptsig;
int		 node;
FILE		*ptfile;
{
lonode_list *ptnode;
ptype_list *ptl;
struct rcnhns *org;

	ptnode=getlonode(ptsig, node);
	ptl=getptype(ptnode->USER,HNSRCN_X);

	if(ptl) {
		fprintf(ptfile,"X %d ",(int)ptl->DATA);
		return;
	}

	ptl=getptype(ptnode->USER,HNSRCN_I);
	if(ptl) {
		org=(struct rcnhns*)(((chain_list*)(ptl->DATA))->DATA);
		fprintf(ptfile,"I %s %d ",org->insname,org->position);
		return;
	}
	fprintf(ptfile,"P %ld ",ptnode->INDEX);
                
}

/*******************************************************************************
* function savelofig()                                                         *
*******************************************************************************/
void vtisavelofig(ptfig)
lofig_list *ptfig;
{
loins_list *ptins;
lotrs_list *pttrs;
locon_list *ptcon;
losig_list *ptsig;
chain_list *ptscan, *ptlist, *scanchain;
FILE *ptfile;
long nbio;
long con_id;
float f1, f2;
char date[30];
long sig_vdd = -1, sig_vss = -1;
int linelength;
char *s;
int *old_index; /* table indexed by the file indexes giving mbk's one */
static char tn, tp;
int idx_rcn=1;
lorcnet_list *ptrcnet;
num_list *scannum;
lonode_list *ptnode;
ptype_list *ptptype;
struct rcnhns *elem_rcnhns, *org;
lowire_list *scanwire;

  lofigchain( ptfig );
	if (!tn) /* either one, don't matter */
		env(&tn, &tp);

	if ((ptfile = mbkfopen(ptfig->NAME, OUT_LO, WRITE_TEXT)) == NULL) {
		(void)fflush(stdout);
		(void)fprintf(stderr, "*** mbk error ***\n");
		(void)fprintf(stderr, "vtisavelofig can't open file %s.%s\n",
							ptfig->NAME, OUT_LO);
		EXIT(1);
	}  else if (TRACE_MODE == YES)
		(void)printf("--- mbk --- writing the file : %s.%s\n",
							ptfig->NAME, OUT_LO);

	/* reverse instances, connectors and parameters order */
	ptfig->LOCON = (locon_list *)reverse((chain_list *)ptfig->LOCON);
	ptfig->LOINS = (loins_list *)reverse((chain_list *)ptfig->LOINS);
	ptfig->LOTRS = (lotrs_list *)reverse((chain_list *)ptfig->LOTRS);
	for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
		ptins->LOCON = (locon_list *)reverse((chain_list *)ptins->LOCON);

	/* space for old indexes to be restored at the end */
	con_id = 1;
	for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT)
		con_id++;
	old_index = (int *)mbkalloc((unsigned int)(con_id * sizeof(int)));

	/* changing signals indexes : externals signals must be first for vti */
	con_id = 0;
	for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT) {
		old_index[++con_id] = ptcon->SIG->INDEX;
		ptcon->SIG->INDEX = con_id;
		ptcon->SIG->TYPE = EXTERNAL;
		if (isvdd(ptcon->NAME))
			sig_vdd = con_id;
		else if (isvss(ptcon->NAME))
			sig_vss = con_id;
		ptrcnet=ptcon->SIG->PRCN;
		if(ptrcnet)
			if(!gettabnode(ptrcnet))
				buildtable(ptcon->SIG);
	}
	nbio = con_id;
	for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT) {
		if (ptsig->TYPE != EXTERNAL) {
			old_index[++con_id] = ptsig->INDEX;
			ptsig->INDEX = con_id;
			s = getsigname(ptsig);
			if (isvdd(s))
				sig_vdd = con_id;
			else if (isvss(s))
				sig_vss = con_id;
			ptrcnet=ptsig->PRCN;
			if(ptrcnet)
				if(!gettabnode(ptrcnet))
					buildtable(ptsig);
		}
	}

	if (sig_vdd == -1) {
		sig_vdd = ++con_id;
		(void)fprintf(stderr, "*** mbk warning ***\n vti %s driver detected",
							OUT_LO);
		(void)fprintf(stderr, " no '*vdd*' signal in figure %s (assigned to %ld)\n",
									ptfig->NAME, sig_vdd);
	}
	if (sig_vss == -1) {
		sig_vss = ++con_id;
		(void)fprintf(stderr, "*** mbk warning ***\n vti %s driver detected",
							OUT_LO);
		(void)fprintf(stderr, " no '*vss*' signal in figure %s (assigned to %ld)\n",
									ptfig->NAME, sig_vss);
	}

	/* header */
	vtigetdate(date);
	if (strcmp(OUT_LO, "hns") == 0) {
		(void)fprintf(ptfile, "#cell1 %s %s hnSchematic 0 v7r5.6\n",
							 ptfig->NAME, TECHNO);
		(void)fprintf(ptfile, "# %s %s mbkvti400 * .\n", date, date);
		(void)fprintf(ptfile, "H 1 ;\n");
	} else {
		(void)fprintf(ptfile, "#cell1 %s %s fnExtracted 4096 v7r5.6\n",
							ptfig->NAME, TECHNO);
		(void)fprintf(ptfile, "# %s %s mbkvti400 * .\n", date, date);
		(void)fprintf(ptfile, "H 3 ;\n");
	}
	(void)fprintf(ptfile, "B 0 %ld ;\n", nbio);

	/* connectors */
	for (ptcon = ptfig->LOCON; ptcon; ptcon = ptcon->NEXT) {
		if( (scannum = ptcon->PNODE) ) {
			for( ; scannum ; scannum=scannum->NEXT ) {
				(void)fprintf(ptfile, "X "); /* unable to represent direction in hns */
				(void)fprintf(ptfile, "%ld ", ptcon->SIG->INDEX);
				(void)fprintf(ptfile, "%d ", idx_rcn);
				ptnode=getlonode(ptcon->SIG,scannum->DATA);
				ptnode->USER=addptype(ptnode->USER,HNSRCN_X,(void*)idx_rcn);
				(void)fprintf(ptfile, "%s;\n", busname(ptcon->NAME));
				idx_rcn++;
			}
		}
		else {
			(void)fprintf(ptfile, "X "); /* unable to represent direction in hns */
			(void)fprintf(ptfile, "%ld ", ptcon->SIG->INDEX);
			(void)fprintf(ptfile, "%d ", idx_rcn++);
			(void)fprintf(ptfile, "%s;\n", busname(ptcon->NAME));
		}
		
	}

	/* local models derived from the first instances */
	ptlist = NULL;
	for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT) {
		idx_rcn=1;
		for (ptscan = ptlist; ptscan; ptscan = ptscan->NEXT)
			if (ptins->FIGNAME == (char *)ptscan->DATA)  
				break;
		if (ptscan == NULL) {
			ptlist = addchain(ptlist, (void *)ptins->FIGNAME);
			linelength = fprintf(ptfile, "M HNS ");
			linelength += fprintf(ptfile, "%s ", ptins->FIGNAME);
			linelength += fprintf(ptfile, "| ");
			linelength += fprintf(ptfile, "| ");
			for (ptcon = ptins->LOCON; ptcon; ptcon = ptcon->NEXT) {
				if( (scannum = ptcon->PNODE) ) {
					for( ; scannum ; scannum=scannum->NEXT ) {
						linelength += fprintf(ptfile, "%s ", busname(ptcon->NAME));
					}
				}
				else {
					linelength += fprintf(ptfile, "%s ", busname(ptcon->NAME));
				}
				LINEBREAK;
			}
			linelength += fprintf(ptfile, "| ");
			LINEBREAK;
			for (ptcon = ptins->LOCON; ptcon != NULL; ptcon = ptcon->NEXT) {
				if( (scannum = ptcon->PNODE) ) {
					for( ; scannum ; scannum=scannum->NEXT ) {
						(void)fprintf(ptfile, "%d ", idx_rcn++);
					}
				}
				else {
					(void)fprintf(ptfile, "%d ", idx_rcn++);
				}
				LINEBREAK;
			}
			(void)fprintf(ptfile, ";\n");
		}
	}
	freechain(ptlist);

	/* instances */
	for (ptins = ptfig->LOINS; ptins != NULL; ptins = ptins->NEXT) {

		idx_rcn=1;
		linelength = fprintf(ptfile, "I HNS ");
		linelength += fprintf(ptfile, "%s ", ptins->FIGNAME);
		linelength += fprintf(ptfile, "%s ", ptins->INSNAME);
		linelength += fprintf(ptfile, "| ");
		linelength += fprintf(ptfile, "| ");

		for (ptcon = ptins->LOCON; ptcon; ptcon = ptcon->NEXT) {
			if( (scannum = ptcon->PNODE) ) {
				for( ; scannum ; scannum=scannum->NEXT ) {
					linelength += fprintf(ptfile, "%ld ", ptcon->SIG->INDEX);

					ptnode=getlonode(ptcon->SIG,scannum->DATA);
					if( !(ptptype=getptype(ptnode->USER,HNSRCN_I)) ) {
						ptnode->USER=addptype(ptnode->USER,HNSRCN_I,NULL);
						ptptype=ptnode->USER;
					}
					elem_rcnhns=mbkalloc(sizeof(struct rcnhns));
					ptptype->DATA=addchain(((chain_list*)(ptptype->DATA)),elem_rcnhns);
					elem_rcnhns->insname=ptins->INSNAME;
					elem_rcnhns->position=idx_rcn;
					RCN_CLEARFLAG(ptnode->FLAG,RCN_FLAG_USER_27);
					idx_rcn++;
				}
			}
			else {
				linelength += fprintf(ptfile, "%ld ", ptcon->SIG->INDEX);
			}
			LINEBREAK;
		}
		(void)fprintf(ptfile, ";\n");
	}

	/* transistors */
	for (pttrs = ptfig->LOTRS; pttrs; pttrs = pttrs->NEXT) {
		(void)fprintf(ptfile, "T ");
		(void)fprintf(ptfile, "%c ", pttrs->TYPE == TRANSN ? tn : tp);
		(void)fprintf(ptfile, "* ");
		f1 = (float)pttrs->X / SCALE_X;
		f2 = (float)pttrs->Y / SCALE_X;
		(void)fprintf(ptfile, "[%.2f,%.2f] ", f1, f2);
		f1 = (float)pttrs->WIDTH / SCALE_X;
		f2 = (float)pttrs->LENGTH / SCALE_X;
		(void)fprintf(ptfile, "%.2f %.2f ", f1, f2);
		(void)fprintf(ptfile, "0 ");
		(void)fprintf(ptfile, "%ld ", pttrs->GRID->SIG->INDEX);
		(void)fprintf(ptfile, "%ld ", pttrs->SOURCE->SIG->INDEX);
		(void)fprintf(ptfile, "%ld ", pttrs->DRAIN->SIG->INDEX);
		(void)fprintf(ptfile, "%ld ", pttrs->TYPE == TRANSN ? sig_vss : sig_vdd);
		(void)fprintf(ptfile, ";\n");
	}

	/* signals */
	for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT) {
		ptrcnet=ptsig->PRCN;
		if ((ptsig->TYPE == INTERNAL && ptsig->NAMECHAIN != NULL) || ptrcnet) {
			s=getsigname(ptsig);
			(void)fprintf(ptfile, "N ");
			(void)fprintf(ptfile, "%ld ", ptsig->INDEX);
			(void)fprintf(ptfile, "%s ", busname(s));
			(void)fprintf(ptfile, ";\n");
			if (ptsig->PRCN && ptsig->PRCN->CAPA && strcmp(OUT_LO, "hns")) {
				(void)fprintf(ptfile, "C 0 * * ");           /* type */
				(void)fprintf(ptfile, "%.3e ", ptsig->PRCN->CAPA); /* val  */
				(void)fprintf(ptfile, "%ld ", ptsig->INDEX); /* equi */
				(void)fprintf(ptfile, "%ld ", sig_vss);      /* bulk */
				(void)fprintf(ptfile, ";\n");
			}
			if(ptrcnet) {
				for(scanwire=ptrcnet->PWIRE;scanwire;scanwire=scanwire->NEXT) {
					(void)fprintf(ptfile, "W ");
					wire_point(ptsig,scanwire->NODE1,ptfile);
					wire_point(ptsig,scanwire->NODE2,ptfile);
					(void)fprintf(ptfile,"%.3e %.3e\n",scanwire->RESI,scanwire->CAPA);
				}

				for(scanchain=getptype(ptsig->USER,LOFIGCHAIN)->DATA;scanchain;scanchain=scanchain->NEXT) {
					ptcon=(locon_list*)(scanchain->DATA);
					for(scannum=ptcon->PNODE;scannum;scannum=scannum->NEXT) {
						ptnode=getlonode(ptsig,scannum->DATA);
							if( !RCN_GETFLAG(ptnode->FLAG,RCN_FLAG_USER_27) ) {
								if( (ptptype=getptype(ptnode->USER,HNSRCN_I)) ) {
								org=((struct rcnhns*)(((chain_list*)(ptptype->DATA))->DATA));
								for(ptscan=((chain_list*)(ptptype->DATA))->NEXT;ptscan;ptscan=ptscan->NEXT) {
									elem_rcnhns=(struct rcnhns*)(ptscan->DATA);
									fprintf(ptfile,"W I %s %d I %s %d 0.0 0.0\n",elem_rcnhns->insname,elem_rcnhns->position,org->insname,org->position);
								}
							}
							RCN_SETFLAG(ptnode->FLAG,RCN_FLAG_USER_27);
						}
					}
				}
			}
		}
	}

	(void)fprintf(ptfile, "E ;\n");

	/* put back indexes in their place */
	for (ptsig = ptfig->LOSIG; ptsig; ptsig = ptsig->NEXT)
		ptsig->INDEX = old_index[ptsig->INDEX];
	mbkfree(old_index);

	/* reverse instances, connectors and transistor order */
	ptfig->LOCON = (locon_list *)reverse((chain_list *)ptfig->LOCON);
	ptfig->LOTRS = (lotrs_list *)reverse((chain_list *)ptfig->LOTRS);
	ptfig->LOINS = (loins_list *)reverse((chain_list *)ptfig->LOINS);
	for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
		ptins->LOCON = (locon_list *)reverse((chain_list *)ptins->LOCON);

	if (fclose(ptfile)) {
		(void)fflush(stdout);
		(void)fprintf(stderr, "*** mbk error ***\n");
		(void)fprintf(stderr, "vtisavelofig can't close file %s.%s\n",
							ptfig->NAME, OUT_LO);
		EXIT(1);
	}
}
