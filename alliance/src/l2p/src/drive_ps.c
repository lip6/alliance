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

/****************************************************************************/
/*                                                                          */
/*                      Chaine de CAO & VLSI   Alliance                     */
/*                                                                          */
/*    Produit :  (R)ectangle (D)ata (S)tructure (2)to (P)ost(S)cript driver */
/*    Fichier :  drive_ps.c                                                 */
/*                                                                          */
/*    (c) copyright 1991-94 Laboratoire MASI equipe CAO & VLSI              */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Auteur(s) :  Gilles-Eric DESCAMPS                 le : 23/03/1993     */
/*                                                                          */
/*    Modifie par :  Gilles-Eric DESCAMPS               le : 30/01/1994     */
/* 1.03 introduces -landscape, automating centering of cell in drawing,     */
/* suppression of '-1x1' extension for monopages drawings, use of an        */
/* external PostScript dictionnary, ability to give its own dictionnary,    */
/*                                                                          */
/*    Modifie par :  Etienne LACOUME                    le : 02/03/1995     */
/* 1.10 introduces a better quality in drawing,the capability to display    */
/* the names of rectangles,connectors,references,instances,segments,the     */
/* ability to use several sizes for the font,the manualfeed mode for the    */
/* printer.                                                                 */
/****************************************************************************/

#ident "$Id: drive_ps.c,v 1.1 2002/04/03 13:55:21 ludo Exp $"

#define DRIVE2PS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mut.h"
#include "mph.h"
#include "rds.h"
#include "rwi.h"
#include "rut.h"
#include RPS_H
#include "rtl.h"
#include "rpr.h"
#include "rfm.h"

struct rps_param p;

enum {E_NOPARAM, E_NOFIG, E_OPEN, E_CLOSE, E_WRITE, E_READ, E_OUTBOX};
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_error (type, msg)
int	type;
char	*msg;
{
	fflush (stdout);
	fprintf (stderr, "RPS_ERROR: ");
	switch (type) {
		case E_NOPARAM :
			fprintf (stderr, "No parameters given to RPS !");
		break;
		case E_NOFIG :
			fprintf (stderr, "No figure given to RPS !");
		break;
		case E_OPEN :
			fprintf (stderr, "Problem while opening file ");
			fprintf (stderr, msg);
		break;
		case E_CLOSE :
			fprintf (stderr, "Problem while closing file ");
			fprintf (stderr, msg);
		break;
		case E_WRITE :
			fprintf (stderr, "Problem while writing file ");
			fprintf (stderr, msg);
		break;
		case E_READ :
			fprintf (stderr, "Problem while reading file ");
			fprintf (stderr, msg);
		break;
		case E_OUTBOX :
			fprintf (stderr, msg);
		break;
		default :
			fprintf (stderr, "Unknow internal error");
	};
	fprintf (stderr,"\n");
	fflush (stderr);
	exit (1);
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
long rps_getboundingbox( FigureRds, X, Y, dX, dY )
rdsfig_list     *FigureRds;
long            *X, *Y, *dX, *dY;
{
        rdsrec_list     *Rectangle;
        rdsins_list     *Instance;
        int                     Layer;
        long            X1, Y1, X2, Y2;
        long            XMinRds, YMinRds, XMaxRds, YMaxRds;
        long            MaskCounter;
        short           ComputeBound;
 
        MaskCounter  = 0;
        ComputeBound = RDS_YES;
        for ( Layer = 0; Layer < RDS_MAX_LAYER; Layer++ ) {
                for ( Rectangle  = FigureRds->LAYERTAB[Layer];
                Rectangle != (rdsrec_list *)NULL;
                Rectangle  = Rectangle->NEXT ) {
                        MaskCounter++;
                        X1 = Rectangle->X;
                        X2 = X1 + Rectangle->DX;
                        Y1 = Rectangle->Y;
                        Y2 = Y1 + Rectangle->DY;
                        if ( ComputeBound ) {
                                ComputeBound = RDS_NO;
                                XMaxRds = X2;
                                YMaxRds = Y2;
                                XMinRds = X1;
                                YMinRds = Y1;
                        } else {   
                                if ( XMaxRds < X2 ) XMaxRds = X2;
                                if ( YMaxRds < Y2 ) YMaxRds = Y2;
                                if ( XMinRds > X1 ) XMinRds = X1;
                                if ( YMinRds > Y1 ) YMinRds = Y1;
                        };
                };
        };
        for ( Instance  = FigureRds->INSTANCE;
        Instance != (rdsins_list *)NULL;
        Instance  = Instance->NEXT ) {
                for ( Layer = 0;
                Layer < RDS_MAX_LAYER;
                Layer++ ) {
                        for ( Rectangle  = Instance->LAYERTAB[Layer];
                        Rectangle != (rdsrec_list *)NULL;
                        Rectangle  = Rectangle->NEXT ) {
                                MaskCounter++;
                                X1 = Rectangle->X;
                                X2 = X1 + Rectangle->DX;
                                Y1 = Rectangle->Y;
                                Y2 = Y1 + Rectangle->DY;
                                if ( ComputeBound ) {
                                        ComputeBound = RDS_NO;
                                        XMaxRds = X2;
                                        YMaxRds = Y2;
                                        XMinRds = X1;
                                        YMinRds = Y1;
                                } else {
                                        if ( XMaxRds < X2 ) XMaxRds = X2;
                                        if ( YMaxRds < Y2 ) YMaxRds = Y2;
                                        if ( XMinRds > X1 ) XMinRds = X1;
                                        if ( YMinRds > Y1 ) YMinRds = Y1;
                                };
                        };
                };
        };
        *X      = XMinRds;
        *dX     = XMaxRds-XMinRds;
        *Y      = YMinRds;
        *dY     = YMaxRds-YMinRds;
        return( MaskCounter );
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_put (pg, s)
rps_pge	*pg;
char	*s;
{
	static char     buf[80];
	static int	res, lg;

	lg = strlen(s);
	res = fprintf(pg->f, "%s", s);
	if (res != lg) {
		rps_error(E_WRITE, pg->fname);
	}
	pg->nb_bytes_written += lg;
	if ((pg->nb_bytes_written / 16384) > pg->nb_last_written) {
		pg->nb_last_written = (pg->nb_bytes_written / 16384) ;
		sprintf (buf, "(%ldKo of %s)INFO\n",
			pg->nb_last_written*16, pg->param->figname);
		rps_put (pg, buf);
	};
}

extern rps_print_dict_color ();
extern rps_print_dict_bw ();
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_put_psdict (pg)
rps_pge	*pg;
{
	FILE	*dict;
	char	buffer[BUFSIZ+16];
	size_t	lu, ecr;

	if (strlen (p.dictfilename)) {
		dict = fopen (p.dictfilename, "r");
		if (!dict) {
			rps_error (E_OPEN, p.dictfilename);
		};
		while (!feof (dict)) {
			lu = fread (buffer, 1, BUFSIZ, dict);
			if (lu == -1) {
				rps_error (E_READ, p.dictfilename);
			};
			ecr = fwrite (buffer, 1, lu, pg->f);
			if (ecr == -1) {
				rps_error (E_WRITE, pg->fname);
			};
		};
		if (fclose (dict) == EOF) {
			rps_error (E_CLOSE, p.dictfilename);
		};
	} else {
		if (p.couleur) {
			rps_print_dict_color (pg);
		} else {
			rps_print_dict_bw (pg);
		};
	};
	if (p.a3tray) {
		rps_put (pg, "statusdict /a3tray known\n");
		rps_put (pg, "	{statusdict begin a3tray end}\n");
		rps_put (pg, "if\n\n");
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_put_header (pg)
rps_pge	*pg;
{
	char buf[512];
	char fname[80];

	sprintf (buf, "\n%ld %ld %ld %ld ER\n", pg->ps_x,
	pg->ps_y, pg->ps_w, pg->ps_h);
	rps_put (pg, buf);
	sprintf (buf, "%ld %ld moveto\n", pg->ps_x, pg->ps_y - 16);
	rps_put (pg, buf);
	rps_put (pg, "/Courier-Bold findfont 14 scalefont setfont\n");
	strcpy (fname, pg->fname);
	fname[strlen(fname)-3]='\0';
	sprintf (buf, "(%s)show\n", fname);
	rps_put (pg, buf);
	if ((p.nbx_pages!=1) || (p.nby_pages!=1)) {
		rps_put (pg, "/Helvetica findfont 10 scalefont setfont\n");
		sprintf (buf, "( %dx%d pages total)", p.nbx_pages,
		p.nby_pages);
		sprintf (buf, "%sshow\n", buf);
		rps_put (pg, buf);
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_open_output (p)
rps_par *p;
{
	int	nbxf, nbyf, x, y;
	double	topsw, topsh;
	char	fname[256];
	char	buf[512];
	rps_pge	*pg;

                                                   /* On calcule l'echelle */
	topsw = ((double) p->draw_w) / (double) (p->rds_w);
	topsh = ((double) p->draw_h) / (double) (p->rds_h);
	if (topsw < topsh) {
		p->width_first = RDS_YES;
		if (p->verbose) {
			printf ("\twidth first, ");
		};
	} else {
		p->width_first = RDS_NO;
		if (p->verbose) {
			printf ("\theight first, ");
		};
	};
	if (p->tops == 0.0) {
		if (p->width_first) {
			p->tops = topsw;
		} else {
			p->tops = topsh;
		};
	};
	if (p->verbose) {
		printf ("SCALE=%f, resol=%lddpi ", p->tops, p->resol);
	};
                                           /* recentrage de la fenetre RDS */
	p->rds_dx = ((long) ((double) p->draw_w / p->tops) - p->rds_w) / 2 ;
	printf (" Centering:(%ld", p->rds_dx);
	p->rds_dy = ((long) ((double) p->draw_h / p->tops) - p->rds_h) / 2 ;
	printf (",%ld)\n", p->rds_dy);
                                       /* On calcule le nombre de feuilles */
	if (p->draw_w % p->paper_w) {
		nbxf = (p->draw_w / p->paper_w) + 1;
	} else {
		nbxf = (p->draw_w / p->paper_w);
	};
	if (p->draw_h % p->paper_h) {
		nbyf = (p->draw_h / p->paper_h) + 1;
	} else {
		nbyf = (p->draw_h / p->paper_h);
	};
	p->nbx_pages = nbxf;
	p->nby_pages = nbyf;
                           /* On genere les infos sur cette feuille unique */
	if ((p->nbx_pages == 1) && (p->nby_pages == 1)) {
		if (p->verbose) {
			printf ("\tOne page  ");
		};
		pg = (rps_pge *) mbkalloc (sizeof (rps_pge));
		pg->param = p;
		pg->next = NULL;
		pg->prpnom = (list_rnm *) NULL;
		sprintf (fname,"%s.ps", p->figname);
		pg->fname = namealloc (fname);
		pg->f = fopen (pg->fname, "w");
		if (!pg->f) {
			rps_error (E_OPEN, pg->fname);
		};
		pg->nb_bytes_written = 0;
		pg->nb_last_written = 0;
                          /* On calcule la BoundingBox PostScript par page */
		pg->ps_x = p->paper_x;
		pg->ps_y = p->paper_y;
		if (p->draw_w % p->paper_w) {
			pg->ps_w = p->draw_w % p->paper_w;
		} else {
			pg->ps_w = p->paper_w;
		};
		if (p->draw_h % p->paper_h) {
			pg->ps_h = p->draw_h % p->paper_h;
		} else {
			pg->ps_h = p->paper_h;
		};
		if (p->verbose) {
			printf (" PS(%ld,%ld,%ld,%ld)  ", pg->ps_x,
			pg->ps_y, pg->ps_w, pg->ps_h);
		};
                                 /* On calcule la BoundingBox RDS par page */
		pg->rds_x = p->rds_x;
		pg->rds_y = p->rds_y;
		pg->rds_h = p->draw_h / p->tops;
		pg->rds_w = p->draw_w / p->tops;
		if (p->verbose) {
			printf (" RDS(%ld,%ld,%ld,%ld)\n", pg->rds_x,
			pg->rds_y, pg->rds_w, pg->rds_h);
		};
		p->headpages = pg;
		rps_put (pg, "%!PS-Adobe-2.0 EPSF-1.2\n");
		sprintf (buf, "%%%%Title: %s\n", pg->fname);
		rps_put (pg, buf);
		sprintf (buf, "%%%%BoundingBox: %ld %ld %ld %ld\n",
		pg->ps_x, pg->ps_y, pg->ps_x+pg->ps_w,
		pg->ps_y+pg->ps_h);
		rps_put (pg, buf);
		sprintf (buf, "%%%%Creator: Rps v%s", RPS_VERSION);
		sprintf (buf, "%s with%s\n", buf, p->cmdline);
		rps_put (pg, buf);
		sprintf (buf, "%%SCALE=%f\n", p->tops);
		rps_put (pg, buf);
		rps_put_psdict (pg);
		rps_put (pg, "%%Page: 1 1\n");
		if (p->header) {
			rps_put_header (pg);
		};
		sprintf (buf, "%f setlinewidth\n", p->tops / 2.0);
		rps_put (pg, buf);
		return;
	};
                                   /* On genere les infos sur ces feuilles */
	p->headpages = NULL;
	for (x=0 ; x<nbxf ; x++) {
		for (y=0 ; y<nbyf ; y++) {
			if (p->verbose) {
				printf ("\tpage (%d,%d)  ", x+1, y+1);
			};
			pg = (rps_pge *) mbkalloc (sizeof (rps_pge));
			pg->param = p;
			pg->next = p->headpages;
			pg->prpnom = (list_rnm *) NULL;
			sprintf (fname,"%s-%dx%d.ps", p->figname, x+1, y+1);
			pg->fname = namealloc (fname);
			pg->f = fopen (pg->fname, "w");
			pg->nb_bytes_written = 0;
			pg->nb_last_written = 0;
                          /* On calcule la BoundingBox PostScript par page */
			pg->ps_x = p->paper_x;
			pg->ps_y = p->paper_y;
			if (!pg->f) {
				rps_error (E_OPEN, pg->fname);
			};
			if ((x == nbxf-1) && (p->draw_w % p->paper_w)) {
				pg->ps_w = p->draw_w % p->paper_w;
			} else {
				pg->ps_w = p->paper_w;
			};
			if ((y == nbyf-1) && (p->draw_h % p->paper_h)) {
				pg->ps_h = p->draw_h % p->paper_h;
			} else {
				pg->ps_h = p->paper_h;
			};
			if (p->verbose) {
				printf (" PS(%ld,%ld,%ld,%ld)  ", pg->ps_x,
				pg->ps_y, pg->ps_w, pg->ps_h);
			};
                                 /* On calcule la BoundingBox RDS par page */
			if (p->width_first) {
				pg->rds_w = (p->rds_w/nbxf);
				pg->rds_h = (pg->rds_w*pg->ps_h)/pg->ps_w;
				pg->rds_x = p->rds_x + (x*pg->rds_w);
				pg->rds_y = p->rds_y + (y*pg->rds_h);
			} else {
				pg->rds_h = (p->rds_h/nbyf);
				pg->rds_w = (pg->rds_h*pg->ps_w)/pg->ps_h;
				pg->rds_y = p->rds_y + (y*pg->rds_h);
				pg->rds_x = p->rds_x + (x*pg->rds_w);
			};
			if (p->verbose) {
				printf (" RDS(%ld,%ld,%ld,%ld)\n", pg->rds_x,
				pg->rds_y, pg->rds_w, pg->rds_h);
			};
			p->headpages = pg;
			rps_put (pg, "%!PS-Adobe-2.0 EPSF-1.2\n");
			sprintf (buf, "%%%%Title: %s\n", pg->fname);
			rps_put (pg, buf);
			sprintf (buf, "%%%%BoundingBox: %ld %ld %ld %ld\n",
			pg->ps_x, pg->ps_y, pg->ps_x+pg->ps_w,
			pg->ps_y+pg->ps_h);
			rps_put (pg, buf);
			sprintf (buf, "%%%%Creator: Rps v%s", RPS_VERSION);
			sprintf (buf, "%s with%s\n", buf, p->cmdline);
			rps_put (pg, buf);
			sprintf (buf, "%%SCALE=%f\n", p->tops);
			rps_put (pg, buf);
			rps_put_psdict (pg);
			rps_put (pg, "%%Page: 1 1\n");
			if (p->header) {
				rps_put_header (pg);
			};
			sprintf (buf, "%f setlinewidth\n", p->tops / 2.0);
			rps_put (pg, buf);
		};
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_start_layer (l)
int l;
{
	static rps_pge	*pg;
	static char      buffer[ 128 ];

	if (p.verbose) {
		printf ("\t%02d layers to do...\r", RDS_MAX_LAYER-l);
		fflush (stdout);
	};
	for (pg=p.headpages ; pg ; pg=pg->next) {
		rps_put (pg, "\n%- - - - - - - - - - - - [NEW LAYER]\n");
		rps_put (pg, "/layer save def\n");
/*
** Modified by L.Jacomme 21/09/95
*/
		if ( (l >= RDS_MAX_LAYER ) ||
         (l <  0             ) )
		{
			rps_put (pg, "UNKNOW_init\n");
		}
    else
		{
			sprintf( buffer, "%s_init\n", RDS_LAYER_NAME[ l ] );
			rps_put (pg, buffer );
		}
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_finish_layer (l)
int l;
{
	static rps_pge	*pg;

	if (p.verbose) {
		printf ("\t                                  \r");
	};
	for (pg=p.headpages ; pg ; pg=pg->next) {
		rps_put (pg, "layer restore\n");
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_filter_rec (r)
rdsrec_list	*r;
{
	static long	ax, ay, aw, ah, tx, ty, tw, th;
	static rps_pge	*pg;
	static char	buf[512];
	static double	px, py, pw, ph;

	if (p.landscape) {
		ax = p.rds_x + (p.rds_x+p.rds_w) - (r->Y+r->DY);
		ay = r->X;
		aw = abs(r->DY);
		ah = abs(r->DX);
	} else {
		ax=r->X; ay=r->Y; aw=r->DX; ah=r->DY;
	};
	ax += p.rds_dx;
	ay += p.rds_dy;
	if ((ax < p.rds_x) || (ay < p.rds_y) || (aw > p.rds_w)
           || (ah > p.rds_h)) {
		sprintf (p.err, "(%d, %d, %d, %d) is", ax, ay, aw, ah);
		sprintf (p.err, "%s out of boundingbox:(%d, %d, %d, %d)",
		p.err, p.rds_x, p.rds_y, p.rds_w, p.rds_h);
		rps_error (E_OUTBOX, p.err);
	}
	for (pg=p.headpages ; pg ; pg=pg->next) {
                                     /* Est-on en-dehors de la page RDS ? */
		if ((ax - pg->rds_x) > pg->rds_w) continue;
		if ((ay - pg->rds_y) > pg->rds_h) continue;
		if (((ax - pg->rds_x) + aw) < 0) continue;
		if (((ay - pg->rds_y) + ah) < 0) continue;
                                   /* On tronque aux bords de la page RDS */
		tx = (ax < pg->rds_x ?
                      pg->rds_x :
		      ax);
		ty = (ay < pg->rds_y ?
		      pg->rds_y :
		      ay);
		tw = ((ax + aw) > (pg->rds_x + pg->rds_w) ?
		      ((pg->rds_x + pg->rds_w) - tx) :
		      (ax + aw) - tx);
		th = ((ay + ah) > (pg->rds_y + pg->rds_h) ?
		      ((pg->rds_y + pg->rds_h) - ty) :
		      (ay + ah) - ty);
                                               /* On recentre sur la page */
		tx -= pg->rds_x;
		ty -= pg->rds_y;
                                       /* On traduit en Unites PostScript */
		px = (tx * p.tops) + pg->ps_x;
		py = (ty * p.tops) + pg->ps_y;
		pw = (tw * p.tops);
		ph = (th * p.tops);
		if (((long) (pw*p.resol)/72) || ((long) (ph*p.resol)/72)) {
			sprintf (buf, "%f\t%f\t%f\t%f\tR\n", px, py, pw, ph);
			rps_put (pg, buf);
			stonrec(px,py,pw,ph,r,pg);
		};
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void stonrec (ax,ay,aw,ah,ar,ap)
double ax,ay,aw,ah;
rdsrec_list *ar;
rps_pge *ap;
{
	list_rnm        *recpno;

	if (!p.norectname && ar->NAME!=NULL && !IsRdsConInter(ar)) {
		if ((IsRdsConExter(ar) && p.noconame) ||
		    (IsRdsReference(ar) && p.norefname) ||
		    (IsRdsInstance(ar) && p.noinstname) ||
		    (IsRdsSegment(ar) && p.nosegname) ) {
		}
		else {
			recpno=(list_rnm *) mbkalloc (sizeof(list_rnm));
			recpno->x=(int)(ax+(aw/2.0));
			recpno->y=(int)(ay+(ah/2.0));
			recpno->rot=RDS_NO;
			if ((ah > aw) && IsRdsInstance(ar)) recpno->rot=RDS_YES;
			recpno->lirec=ar;
			recpno->next=ap->prpnom;
			ap->prpnom=recpno;
		};
	};
	return;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void addname()
{
	char buf[80];
	rps_pge *pg;
	list_rnm *pgl;
	float longlet,basewr;
	int scale,xc,yc,xrot;
	char * name;

	longlet=0.57;
	scale=p.textsize;
	basewr=longlet*(float)scale;

	for (pg=p.headpages; pg!=(rps_pge *)NULL; pg=pg->next) {
		sprintf(buf,"/Courier-Bold findfont %d scalefont setfont\n",scale);  
		rps_put(pg,buf);                                              
		for (pgl=pg->prpnom; pgl!=(list_rnm*) NULL; pgl=pgl->next){   
			name=pgl->lirec->NAME;
			if (pgl->rot) {
				xc=pgl->x + (int)(basewr/2.0);
				yc=pgl->y - (int)(basewr*(float)(strlen(name))/2.0);
				xrot=90;
			} else {
				xc=pgl->x - (int)(basewr*(float)(strlen(name))/2.0);
				yc=pgl->y - (int)(basewr/2.0);
				xrot = 0;
			};
			sprintf(buf,"%d %d moveto (%s) %d showstring\n",
			xc,yc,name,xrot);
			rps_put(pg,buf); 
		};
	};
	return;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void catinst(ap)
rps_par * ap; 
{
	rdsins_list	* Instance;
	rdsrec_list	* pcatr,*prabox;
	rdsfig_list	* FigureRds;

		FigureRds=ap->fig;
		for (Instance = FigureRds->INSTANCE;
		     Instance != (rdsins_list *) NULL;
		     Instance = Instance->NEXT) {
			prabox = Instance->LAYERTAB[RDS_ABOX];
			if (prabox == NULL) continue;
			pcatr = (rdsrec_list *) mbkalloc(sizeof(rdsrec_list));
			pcatr->X = prabox->X;
			pcatr->Y = prabox->Y;
			pcatr->DX = prabox->DX;
			pcatr->DY = prabox->DY;
			pcatr->FLAGS = prabox->FLAGS;
			pcatr->NAME = Instance->INSNAME;
			pcatr->NEXT = FigureRds->LAYERTAB[RDS_ABOX];
			FigureRds->LAYERTAB[RDS_ABOX] = pcatr;
		};
		return;
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_close_output (p)
rps_par *p;
{
	rps_pge	*pg;

	for (pg=p->headpages ; pg ; pg=pg->next) {
		if (p->manualfeed) rps_put(pg,"MF\n");
		rps_put (pg, "showpage\n");
		rps_put (pg, "%%EOF\n");
		if (fclose (pg->f) == EOF) {
			rps_error (E_CLOSE, pg->fname);
		};
	};
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void rps_walk_fig (fig, layer, param)
rdsfig_list	*fig;
int		layer;
rps_par		*param;
{
	rdsrec_list	*r;
/*
	rdsins_list	*i;
	rdsfig_list	*m;
*/

	for ( r = fig->LAYERTAB[layer] ;
	r != (rdsrec_list *) NULL ;
	r = r->NEXT) {
		rps_filter_rec (r);
	}
/*
	for (i=fig->INSTANCE ; i!=(rdsins_list *)NULL ; i=i->NEXT) {
		m = rps_get_fig (i->FIGNAME);
		if (incatalog(i->FIGNAME)) {
			
	}
*/
};

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void saveps (p)
rps_par *p;
{
	long nbmask, swap;
	int 	l;


	if (!p) {
		rps_error (E_NOPARAM,"");
	};
                                        /* On calcule la Bounding Box RDS */
	if (!(p->fig)) {
		rps_error (E_NOFIG,"");
	};
	nbmask = rps_getboundingbox (p->fig, &p->rds_x, &p->rds_y,
        &p->rds_w, &p->rds_h);
	if (p->verbose) {
		printf ("\t%ld masks, ", nbmask);
		printf ("BBox=(x=%ld,y=%ld,w=%ld,h=%ld)\n", p->rds_x,
		p->rds_y, p->rds_w, p->rds_h);
	};
	if (p->landscape) {
		swap = p->rds_y;
		p->rds_y = p->rds_x;
		p->rds_x = swap;
		swap = p->rds_h;
		p->rds_h = p->rds_w;
		p->rds_w = swap;
	};
                                       /* On revient en unites PostScript */
	if (p->verbose) {
		printf ("\tPaper=(%.3f,%.3f) %.3f inch(es) wide, %.3f ",
		p->paper_x/100.0, p->paper_y/100.0, p->paper_w/100.0,
		p->paper_h/100.0);
		printf ("inch(es) tall\n");
	};
	p->paper_x = (p->paper_x*72)/100;
	p->paper_y = (p->paper_y*72)/100;
	p->paper_w = (p->paper_w*72)/100;
	p->paper_h = (p->paper_h*72)/100;
	p->draw_w = (p->draw_w*72)/100;
	p->draw_h = (p->draw_h*72)/100;
                      /* On reduit de la taille necessaire a la cartouche */
	if (p->header) {
		p->paper_y += 18 ;
		p->paper_h -= 18 ;
	};
       /* On calcule la taille du dessin si elle nous est donnee en pages */
	if ((p->nbx_pages!=0) && (p->nbx_pages!=1)) {
		p->draw_w = p->paper_w * p->nbx_pages;
	};
	if ((p->nby_pages!=0) && (p->nby_pages!=1)) {
		p->draw_h = p->paper_h * p->nby_pages;
	};
	if (p->verbose) {
		printf ("\tDrawing= %.3f inch(es) wide, %.3f inch(es) tall\n",
		p->draw_w/72.0, p->draw_h/72.0);
	};
	catinst(p);
                                   /* On ouvre le(s) fichier(s) de sortie */
	rps_open_output (p);
	fflush (stdout);
	
	for (l=0 ; l<RDS_MAX_LAYER ; l++) {
		rps_start_layer (l);
		rps_walk_fig (p->fig, l, p);
		rps_finish_layer (l);
	};

	addname();

                                   /* On ferme le(s) fichier(s) de sortie */
	rps_close_output (p);

};
/****************************************************************************/
