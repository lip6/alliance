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
#include "beh.h"
#include "bvl_utype.h"
#include "bvl_utdef.h"
#include "bvl_parse.h"

/* ###--------------------------------------------------------------### */
/*   Function : vhdlloadbefig						*/
/*   Content  : Parse a .vbe file to produce a befig              	*/
/*   Return   : a pointer on a befig 					*/
/* ###--------------------------------------------------------------### */

struct befig *vhdlloadbefig (pt_befig, figname, trace_mode)

struct befig *pt_befig;
char         *figname;
int           trace_mode;

  {
  char                   *tok;
  char                   *str;
  struct chain           *behsfx_lst = NULL;
  struct chain           *suffix     = NULL;
  static unsigned int     call_nbr = 0;

	/* ###------------------------------------------------------### */
	/*    Create list of suffix for behavioural files from the	*/
	/* environment variable VH_BEHSFX				*/
	/* ###------------------------------------------------------### */

  if ((str = getenv ("VH_BEHSFX")) != NULL)
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
    EXIT (1);
    }

	/* ###------------------------------------------------------### */
	/*    Running the behavioural compiler on the current file	*/
	/* ###------------------------------------------------------### */

  if ((trace_mode & BVL_TRCDFN) != 0)
    beh_message (3, BVL_CURFIL);

  if ((trace_mode & BVL_AUXDFN) == 0)
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

  return (BVL_HEDFIG);
  }
