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

/************************************************************************/
/* module   : getcpat.c							*/
/* date     : 02/09/93							*/
/* auteurs  : PITON							*/
/************************************************************************/

#ident "$Id: GETCPAT.c,v 1.1 2002/05/30 13:28:15 xtof Exp $"

#include <stdio.h>
#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/* ###--------------------------------------------------------------### */
/* function     : GETCPAT						*/
/* description  : return current pattern date				*/
/* called func. : none							*/
/* ###--------------------------------------------------------------### */


unsigned long d_GETCPAT ()
{

  return (GNP_PATTERN_COURANT);

}
