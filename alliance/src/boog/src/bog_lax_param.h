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
 * Tool    : SCmap and Boog - parameter file
 * Date    : 1991,92
 * Author  : Luc Burgun
 * Modified by Czo <Olivier.Sirol@lip6.fr> 1996,97
 * Modified: Francois Donnet 2000
 */




#ifndef LAX_PARAM_H
#define LAX_PARAM_H

#ifndef __P
# ifdef __STDC__ || __GNUC__
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif

/*default and neutral value don't move them without looking*/
#define OPTIM_DELAY0 0     /*optimization in area*/
#define OPTIM_DELAY1 1
#define OPTIM_DELAY2 2     /* 50% delay - 50% area */
#define OPTIM_DELAY3 3
#define OPTIM_DELAY4 4     /*optimization in delay*/
#define DEFAULT_OPTIM OPTIM_DELAY2  /*optimization 50% for delay by default*/

/*physical values*/
#define DEFAULT_DELAY 0             /*pico second   exp-12 s. (around 200)*/
#define DEFAULT_CAPACITANCE 0       /*pico Farad   exp-12 F. (around 0.009)*/
#define DEFAULT_IMPEDANCE 0         /*Ohm    (around 1620)*/
#define DEFAULT_BUFFER 0            /*num of buffer to put after input*/


/***************************************************************************/
/*  parse the .lax file and save the internal caracteristics               */
/***************************************************************************/
extern void parsefilelax __P ((char *filename));

/******************************************************************************/
/*  set default caracteristics if no file is given                            */
/******************************************************************************/
extern void defaultlax __P ((int mode_optim));

/***************************************************************************/
/* save the internal caracteristics  in file                               */
/***************************************************************************/
extern void savefilelax __P ((char *filename));

/***************************************************************************/
/*     return the delay (in ps exp-12) of name for an input in lax file    */
/***************************************************************************/
extern double getdelaylax __P ((char *name));

/***************************************************************************/
/*    return the impedance(R in Ohm) of name for an input in lax file      */
/***************************************************************************/
extern double getimpedancelax __P ((char *name));

/***************************************************************************/
/*   return the fanout(C in pF exp-12) of name for an output in lax file   */
/***************************************************************************/
extern double getcapacitancelax __P ((char *name));

/***************************************************************************/
/*      return the number of buffers to put after input in lax file        */
/***************************************************************************/
extern int getbufferlax __P ((char *name));

/***************************************************************************/
/*       return 1 if the internal signal shouldn't be erased               */
/***************************************************************************/
extern int signalsavelax __P ((char *signame));

/***************************************************************************/
/*                return the optim param in lax file                       */
/* 0,1: optimize in area                                                   */
/* 3,4: optimize in delay                                                  */
/* 2: middle                                                               */
/***************************************************************************/
extern int getoptimlax __P (());

/***************************************************************************/
/*          return 1 if lax matches with lofig                             */
/* look if all referenced names are in lofig                               */
/***************************************************************************/
extern int coherencelaxlofig __P ((lofig_list* lofig));

/***************************************************************************/
/*          return 1 if lax matches with befig                             */
/* lokk if all referenced names are in befig                               */
/***************************************************************************/
extern int coherencelaxbefig __P ((befig_list* befig));


#endif
