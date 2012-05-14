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

/*
 * 
 * mbkgen.h : header for the definition of genlib external variable
 * version  : 1.00
 * date     : 05/02/92
 * author   : Frederic Petrot
 * $Id: mbkgen.h,v 1.3 2012/05/14 14:20:13 alliance Exp $
 */

#ifndef _MBKGEN_H_
#define _MBKGEN_H_

#ifdef _MPH_H_
extern phfig_list *WORK_PHFIG;
extern phins_list *WORK_PHINS;
#endif
#ifdef _MLO_H_
extern lofig_list *WORK_LOFIG;
extern long        num_index;
#endif

enum {MBK_INSTANCE, MBK_SEGMENT, MBK_CONNECTOR, MBK_REFERENCE, MBK_VIA};

extern int is_bus();
extern int bus_decod();

#endif

