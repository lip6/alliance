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

/*
 * Tool    : BooG - library cells utilities
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
	float T; 		/* (Tphh+Tpll+Tplh+Tphl)/4	 in ps */
   float C;		    /* Cin   in pF  */
   float R;		    /* Rout  in Ohm   */
	chain_list* ABL;	/* abl of leaf  for mapping*/
	int      STABLE;   /*non-zero if pointed by a STABLE expr in abl*/
   int	DIRECTION;	/* VDD(char*),VSS, IN(const),OUT,INOUT,TRISTATE,TRANSCV*/
   int   NEGATIV;   /*flag set to show if ABL field must be inverted or not*/
} port_list;

/*image of a befig from the cell library*/
typedef struct cell {
	struct cell* NEXT;
	port_list*	 PORT;	  /*image of bepor*/
	chain_list*	 ABL;	  /*it isn't an abl, it's leaves point on port element*/
	biabl_list*	 BIABL;	  /*for cell with condition block*/
	char*		    NAME;	  /*name of befig*/
   int          AREA;
   befig_list*  BEFIG;    /*befig represented*/
   char         MODE;     /*if 'A' (all), cell is developped in lofig*/
                          /*if 'P' (partial), it's known only as a loins name*/
   float        DELAY;    /*to memorize a temporary delay on a cell*/
} cell_list;


/***************************************************************************/
/* return a new port taken from the heap                                   */
/***************************************************************************/
extern port_list* newport __P (());

/******************************************************************************/
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern cell_list* addCell __P ((befig_list* befig));

/******************************************************************************/
/*build a new cell in our library                                             */
/* port of cell is the inverted list of bepor                                 */
/******************************************************************************/
extern cell_list* copyCell __P ((cell_list* old));

/******************************************************************************/
/*insert a new cell in a list of cells                                        */
/* the sort is the biggest to the lowest cell                                 */
/*  (AND a b) is after (AND a (NOT b))                                        */
/* because is always better to take the biggest cell to match for example:    */
/*  (AND (NOT a) (NOT b))  will be matched by (AND a (NOT b))                 */
/*return the new list if inserted else return NULL                            */
/******************************************************************************/
extern cell_list* classCell __P ((cell_list* new_cell, cell_list* list));

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


#endif
