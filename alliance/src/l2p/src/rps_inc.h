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
/*    Fichier :  drive_ps.h                                                 */
/*                                                                          */
/*    (c) copyright 1991-94 Laboratoire MASI equipe CAO & VLSI              */
/*    Tous droits reserves                                                  */
/*    Support : e-mail cao-vlsi@masi.ibp.fr                                 */
/*                                                                          */
/*    Auteur(s) :  Gilles-Eric DESCAMPS                 le : 23/03/1993     */
/*                                                                          */
/*    Modifie par :  Gilles-Eric DESCAMPS               le : 30/01/1994     */
/*    Modifie par :  Etienne LACOUME                    le : 02/03/1995     */
/*                                                                          */
/****************************************************************************/

typedef struct list_rnom
{
        struct list_rnom * next;
        rdsrec_list * lirec;
        int x;
        int y;
	char rot;
}list_rnm;

typedef struct rps_pages
{
	struct rps_param	*param;
	struct rps_pages	*next;
        struct list_rnom        *prpnom;
	FILE			*f;
	char			*fname;
	long			nb_bytes_written;
	long			nb_last_written;
	long			ps_x;
	long			ps_y;
	long			ps_w;
	long			ps_h;
	long			rds_x;
	long			rds_y;
	long			rds_w;
	long			rds_h;
} rps_pge;

typedef struct rps_param
{
	char		*figname;
	rdsfig_list	*fig;
	long		draw_w;
	long		draw_h;
	long		paper_x;
	long		paper_y;
	long		paper_w;
	long		paper_h;
	char		papername[16];
	char		verbose;
	char		a3tray;
	char		couleur;
	char 		norectname;
	char		noconame;
	char		norefname;
	char		noinstname;
	char		nosegname;
	int		textsize;
	char		manualfeed;
	char		flatten;
	char		tilcatal;
	char		header;
	char		landscape;
	char		real;
	long		resol;
	int		nbx_pages;
	int		nby_pages;
	double		tops;
	char		dictfilename[256];
              /* The rest of the structure doesn't have to be completed */
	char		width_first;
	long		rds_x;
	long		rds_y;
	long		rds_w;
	long		rds_h;
	long		rds_dx;
	long		rds_dy;
	rps_pge		*headpages;
	char		err[128];
	char		cmdline[1024];
} rps_par;

#ifndef DRIVE2PS
#ifdef __STDC__		/* We're ANSI */
	extern void saveps (struct rps_param *param);
	extern void rps_GetBoundingBox (rdsfig_list *fig);
#else
	extern saveps ();
	extern rps_GetBoundingBox ();
#endif
#endif
