/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
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

/**********************************************************************
 *                                                                    *
 *               Definitions to tune the program                      *
 *                                                                    *
 **********************************************************************/

#ifndef MAX_WIN_SEG             /* max. number of segments( rectangles) */
#define MAX_WIN_SEG 10000       /* in a window */
#endif
#ifndef MIN_WIN_SEG             /* min. number of segments( rectangles) */
#define MIN_WIN_SEG 100         /* in a window */
#endif
#ifndef MRG_EXT_RATIO           /* the approx. ratio between the num.  */
#define MRG_EXT_RATIO 20        /* of operations done if a rect. is    */
    /* taken in a widow and the num of     */
                         /* operations done to extract rect.    */
#endif
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 		255
#endif
#ifndef MAX_STRING_SIZE
#define MAX_STRING_SIZE 	30
#endif
#ifndef MAX_FILE_NAME_SIZE
#define MAX_FILE_NAME_SIZE 	255
#endif
#ifndef MAX_PHYS_LAYER
#define MAX_PHYS_LAYER 		20
#endif
