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
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*
 * Tool    : LooN - library cells utilities
 * Date    : 2000
 * Author  : Francois Donnet
 */




#ifndef LIB_UTILS_H
#define LIB_UTILS_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif


/*image of a port of a befig*/
typedef struct port {
	struct port* NEXT;
   char  *NAME;      /*name of bepor*/
	double T; 		/* (Tphh+Tpll+Tplh+Tphl)/4	 in ps */
   double C;		    /* Cin   in pF  */
   double R;		    /* Rout  in Ohm   */
} port_list;

/*image of a befig from the cell library*/
typedef struct cell {
	struct cell* NEXT;
	port_list*	 PORT;	  /*image of bepor*/
	char*		    NAME;	  /*name of befig*/
   int          AREA;
   char         MODE;      /*'P' for partial  ;  'A' for all */
   befig_list*  BEFIG;    /*befig represented*/
} cell_list;


/******************************************************************************/
/*build a new cell in our library                                             */
/******************************************************************************/
extern cell_list* createCell __P ((befig_list* befig));

/******************************************************************************/
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern void addCell __P ((befig_list* befig));

/******************************************************************************/
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern cell_list* copyCell __P ((cell_list* old));

/******************************************************************************/
/*duplicate a list of port but not its contents                               */
/******************************************************************************/
extern port_list* copyport __P ((port_list* head));

/******************************************************************************/
/*  copy values of head to new recursively                                    */
/******************************************************************************/
extern void moveport __P ((port_list* head, port_list* new_head));

/******************************************************************************/
/*                    free a list of port                                     */
/******************************************************************************/
extern void delport __P ((port_list* head));

/******************************************************************************/
/* return the cell named                                                      */
/******************************************************************************/
extern cell_list* getCell __P ((char *name));

/******************************************************************************/
/* return a buffer of our library                                             */
/******************************************************************************/
extern cell_list* getCellbuffer __P (());

/******************************************************************************/
/* return the list of cells with the same behaviour than named                */
/******************************************************************************/
extern cell_list* sameCells __P ((char *name));


#endif
