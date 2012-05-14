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


/* ###--------------------------------------------------------------### */
/* file		: mips_util.c						*/
/* date		: Sep  2 1994						*/
/* version	: v0.0							*/
/* author	: Pirouz BAZARGAN SABET & Julien DUNOYER		*/
/* ###--------------------------------------------------------------### */
	
#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"

/* ###--------------------------------------------------------------### */
/* function	: mips_allocstr						*/
/* description	: make a memory allocation for a string. This function	*/
/*		  insures that for a given string there will be a	*/
/*		  unique memory allocation				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char *mips_allocstr (src_str)

char *src_str;				/* source string		*/

  {
  static struct beden **lcl_dic = NULL;
  char                 *res_str = NULL;
  unsigned int          cod1    = 0   ;
  unsigned int          cod2    = 0   ;
  unsigned int          size    = 0   ;
  struct chain         *chn_pnt = NULL;
  struct chain         *str_lst = NULL;

	/* ###------------------------------------------------------### */
	/*    initialize the local static dictionary			*/
	/* ###------------------------------------------------------### */

  if (lcl_dic == NULL)
    lcl_dic = beh_initab ();

  if (src_str != NULL)
    {
	/* ###------------------------------------------------------### */
	/*    hash the source string. Two codes are produced to decrease*/
	/*  conflict probability. 					*/
	/* ###------------------------------------------------------### */

    while (src_str [size] != '\0')
      {
      cod1 = (cod1 <<  1) ^ (cod2 + (src_str [size] << (size % 8)));
      cod2 = (cod1 << 16) + (cod2 << 1)  +  src_str [size] ;
      size++;
      }
    size++;

	/* ###------------------------------------------------------### */
	/*    extract from the dictionary the strings that have the	*/
	/* same first and second code and the same size			*/
	/* ###------------------------------------------------------### */

    chn_pnt = (struct chain *) beh_chktab (lcl_dic, cod1, cod2, 7);
    str_lst = chn_pnt;

	/* ###------------------------------------------------------### */
	/*    scan the list comparing the strings found in the list	*/
	/* with the source string.					*/
	/* ###------------------------------------------------------### */

    while (chn_pnt != NULL)
      {
      if (!strcmp (chn_pnt->DATA, src_str))
        break;
      chn_pnt = chn_pnt->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    if no string has been found, create a new string and add	*/
	/* it into the dictionary.					*/
	/* ###------------------------------------------------------### */

    if (chn_pnt == NULL)
      {
      res_str = (char *) mbkalloc (size * sizeof (char));
      strcpy (res_str, src_str);
      chn_pnt = addchain (NULL, res_str);

      if (str_lst != NULL)
        {
        chn_pnt->NEXT = str_lst->NEXT;
        str_lst->NEXT = chn_pnt      ;
        }
      else
        beh_addtab (lcl_dic, cod1, cod2, 7, chn_pnt);
      }
    else
      res_str = (char *) chn_pnt->DATA;
    }

  return (res_str);
  }
