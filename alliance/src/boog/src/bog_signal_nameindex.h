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

/*
 * Tool    : BooG - give unique index
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef SIGNAL_NAMEINDEX_H
#define SIGNAL_NAMEINDEX_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/***************************************************************************/
/*  free index referencing                                                 */
/***************************************************************************/
extern void free_nameindex __P (());

/***************************************************************************/
/* return 1 if radical name can generate conflict with automatic indexation*/
/***************************************************************************/
extern int forbid_radical __P ((char* name));

/***************************************************************************/
/* return name concatenated with an index and seperated by '_'             */
/* this index is : how many times I have sent this name to getnameindex()  */
/* unicity is guaranteed until you run free_nameindex() below              */
/***************************************************************************/
extern char* getnameindex __P ((char* name));

/******************************************************************************/
/*		build a generic opposite name							                        */
/******************************************************************************/
extern char* getoppositename __P ((char* name));

/******************************************************************************/
/*		build a generic name with an incremented index				               */
/******************************************************************************/
extern char* getautoname __P ((char* name));

/******************************************************************************/
/* return an artificial name to distinguish loins from signal                 */
/******************************************************************************/
extern char* losig_name(char* name);

/******************************************************************************/
/* return an artificial name to distinguish loins from signal                 */
/* only used to hash values for internal treatment                            */
/******************************************************************************/
extern char* loins_name __P ((char* name));

/******************************************************************************/
/* return an artificial name to distinguish input datas from output datas in  */
/* the case of inout port                                                     */
/* only used to hash values for internal treatment                            */
/******************************************************************************/
extern char* output_name __P ((char* name));

#endif
