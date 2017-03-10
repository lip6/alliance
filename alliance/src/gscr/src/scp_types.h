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
 
/******************************************************************************/
/*                                                                            */
/*                    CAO & VLSI's cad tools chain Alliance                   */
/*                                                                            */
/*     Product  : Standard Cell Router                                        */
/*     File     : @(#)                                                        */
/*     Contents :                                                             */
/*                                                                            */
/*     (c) Copyright 1992 Laboratoire MASI equipe CAO & VLSI                  */
/*     All rights reserved                                                    */
/*     Hot line  : cao-vlsi@masi.ibp.fr (e-mail)                              */
/*                                                                            */
/*     Author(s)   :                                    Date : ../../....     */
/*     Modified by : El housseine REJOUAN               Date : 26/02/1993     */
/*     Modified by : Xavier Picat                       Date : 09/07/1993     */
/*     Modified by :                                    Date : ../../....     */
/*                                                                            */
/******************************************************************************/

#ifndef SCP_TYPES_H
#define SCP_TYPES_H
/*---------------------------------------------------------\
					Les includes
\---------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <apr.h>

/*---------------------------------------------------------\
					Les constantes
\---------------------------------------------------------*/
#define NONE              -1
/* cette valeur de 10 000 est suffisante pour des circuits <10000 cellules, mais peut etre
 * trop importante pour des circuits plus gros, en effet, lors du calcul de cout dans modif
 * les calculs se font en long et apparemment avec 100 000, le cout depasse parfois les
 * 2<<30 maximaux que peu contenir un long et donc tous les calculs sont fausses.
 * apparemment le fait de designer CoutTotal comme un float ne ralenti pas les calculs, se
 * serait peut etre la solution : CoutTotal += (float)((long)CoutSignal*(long)Net.coeff);
 * CoutSignal et coeff etant des longs. le fait de caster en long, evite que le C convertisse
 * tout en flottant (ce qu'il doit faire normalement je crois), on n'a donc plus qu'une
 * conversion et une addition en flottant (on gagne une multiplication en flottant).
 * il est moins probable qu'UN signal occasionne une valeur > 2<<30, mais si c le cas, alors
 * il faudrait tout mettre en flottant.
 */
#define FLOAT_TO_LONG  10000

#define max(a,b) ((a)<(b) ? (b):(a))

#endif /* SCP_TYPES_H */
