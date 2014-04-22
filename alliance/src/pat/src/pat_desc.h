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

/* ###--------------------------------------------------------------### */
/* file		: pat_desc.h						*/
/* date		: Mar  9 2000						*/
/* version	: v109							*/
/* author	: Pirouz BAZARGAN SABET					*/
/* content	: parser-driver level library				*/
/* ###--------------------------------------------------------------### */

static struct paseq     *PAT_SEQPNT ;
static struct paini     *PAT_INIPNT ;
static struct pacom     *PAT_COMPNT ;
static struct papat     *PAT_PATPNT ;

static struct paini     *FRE_INIPNT = NULL;
static struct pacom     *FRE_COMPNT = NULL;
static struct papat     *FRE_PATPNT = NULL;
static struct paevt     *FRE_EVTPNT = NULL;

static float             PAT_CNVFS  = 1.0 ;/* convert Fs                   */
static float             PAT_CNVPS  = 1.0 ;/* convert Ps                   */
static float             PAT_CNVNS  = 1.0 ;/* convert Ns                   */
static float             PAT_CNVUS  = 1.0 ;/* convert Us                   */
static float             PAT_CNVMS  = 1.0 ;/* convert Ms                   */

static char             *VALUE      = NULL;
static unsigned short    STT_NBR    ;
static unsigned int      ERR_FLG    = 0   ;
static int               DECVAL     ;
static int               MAXPAT     = 1   ;
static unsigned int      CURDATE    = 0   ;
static unsigned char     PAT_MODE   = 0   ;

static short             NAM_IDX    ;
static char             *NAMTAB [30];
