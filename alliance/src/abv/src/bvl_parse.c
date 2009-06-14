/*------------------------------------------------------------\
|                                                             |
| This file is part of the Alliance CAD System Copyright      |
| (C) Laboratoire LIP6 - Département ASIM Universite P&M Curie|
|                                                             |
| Home page      : http://www-asim.lip6.fr/alliance/          |
| E-mail         : mailto:alliance-users@asim.lip6.fr       |
|                                                             |
| This progam is  free software; you can redistribute it      |
| and/or modify it under the  terms of the GNU Library General|
| Public License as published by the Free Software Foundation |
| either version 2 of the License, or (at your option) any    |
| later version.                                              |
|                                                             |
| Alliance VLSI  CAD System  is distributed  in the hope that |
| it  will be useful, but WITHOUT  ANY WARRANTY;              |
| without even the  implied warranty of MERCHANTABILITY or    |
| FITNESS FOR A PARTICULAR PURPOSE. See the GNU General       |
| Public License for more details.                            |
|                                                             |
| You should have received a copy  of the GNU General Public  |
| License along with the GNU C Library; see the file COPYING. |
| If not, write to the Free Software Foundation, Inc.,        |
| 675 Mass Ave, Cambridge, MA 02139, USA.                     |
|                                                             |
\------------------------------------------------------------*/
/* ###--------------------------------------------------------------### */
/*                                                                      */
/* file         : bvl_parse.c                                           */
/* date         : jun 15 1994						*/
/* version	: v109							*/
/* author       : L.A TABUSSE & H.N. VUONG & P. BAZARGAN-SABET          */
/* description  : Parser VHDL --> BEH                                   */
/* called Func. : mbkalloc						*/
/*                                                                      */
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "aut.h"
#include "abl.h"
#include "bdd.h"
#include "abe.h"
#include "abv.h"
#include "bvl_utype.h"
#include "bvl_utdef.h"
#include "bvl_parse.h"

/* ###--------------------------------------------------------------### */
/*   Function : vhdlloadbefig						*/
/*   Content  : Parse a .vbe file to produce a befig              	*/
/*   Return   : a pointer on a befig 					*/
/* ###--------------------------------------------------------------### */

bddcircuit *BvlBddCircuit = (bddcircuit *)0;
bddsystem  *BvlBddSystem  = (bddsystem  *)0;

struct befig *vhdlloadbefig (pt_befig, figname, trace_mode)

struct befig *pt_befig;
char         *figname;
long           trace_mode;

{
  char                   *tok;
  char                   *str;
  struct chain           *behsfx_lst = NULL;
  struct chain           *suffix     = NULL;
  static unsigned long     call_nbr = 0;
  bddsystem              *SaveBddSystem;
  bddcircuit             *SaveBddCircuit;

  SaveBddSystem  = BddLocalSystem;
  SaveBddCircuit = BddLocalCircuit;

  if ( BvlBddSystem == (bddsystem *)0 )
  {
    BvlBddSystem  = createbddsystem( 50, 1000, 200, 50000000 );
    BvlBddCircuit = createbddcircuit( "-select-", 200, 0, BvlBddSystem );
  }

  BddLocalSystem  = BvlBddSystem;
  BddLocalCircuit = BvlBddCircuit;

	/* ###------------------------------------------------------### */
	/*    Create list of suffix for behavioural files from the	*/
	/* environment variable VH_BEHSFX				*/
	/* ###------------------------------------------------------### */

  if ((str = mbkgetenv ("VH_BEHSFX")) != NULL)
    {
    tok = strtok (str, ":");
    while (tok != NULL)
      {
      behsfx_lst = addchain (behsfx_lst, tok);
      tok        = strtok (NULL, ":");
      }
    behsfx_lst = (struct chain *) reverse (behsfx_lst);
    }
  else
    behsfx_lst = addchain (NULL, "vbe");

	/* ###------------------------------------------------------### */
	/*    Searching the root file					*/
	/* ###------------------------------------------------------### */

  suffix = behsfx_lst;
  while (suffix != NULL)
    {
    if ((bvl_y_in = mbkfopen (figname, suffix->DATA, READ_TEXT)) != NULL)
      {
      sprintf (BVL_CURFIL, "%s.%s", figname, (char *)suffix->DATA);
      break;
      }
    suffix = suffix->NEXT;
    }

  if (bvl_y_in == NULL)
    {
    beh_error (100, figname);
    autexit (1);
    }

	/* ###------------------------------------------------------### */
	/*    Running the behavioural compiler on the current file	*/
	/* ###------------------------------------------------------### */

  if ((trace_mode & ABV_TRACE_MASK) != 0)
    beh_message (3, BVL_CURFIL);

  if ((trace_mode & ABV_KEEP_AUX_MASK) == 0)
    BVL_AUXMOD = 0;
  else
    BVL_AUXMOD = 1;

  BVL_LINNUM = 1;

  if (call_nbr != 0)
    bvl_y_restart (bvl_y_in);
  call_nbr++;

  bvl_y_parse ();
  fclose (bvl_y_in);
  bvl_y_in = NULL;

/*\
  resetbddcircuit( BvlBddCircuit );
  resetbddsystem( BvlBddSystem );
\*/

  BddLocalSystem  = SaveBddSystem;
  BddLocalCircuit = SaveBddCircuit;

  return (BVL_HEDFIG);
}
