
/* ###--------------------------------------------------------------### */
/* file		: bvl_parse.c						*/
/* date		: Oct 30 1995						*/
/* version	: v11							*/
/* author	: L.A TABUSSE & H.N. VUONG & P. BAZARGAN-SABET		*/
/* content	: vhdlloadbefig						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"
#include "bvl.h"

/* ###--------------------------------------------------------------### */
/* function	: vhdlloadbefig						*/
/* description	: analyze a behavioural VHDL description and produce a	*/
/*		  set of data structure. The function returns a pointer	*/
/*		  on a BEFIG. Errors are repported in the returned	*/
/*		  structure.						*/
/* ###--------------------------------------------------------------### */

struct befig *vhdlloadbefig (pt_befig, figname, trace_mode)

struct befig *pt_befig  ;
char         *figname   ;
unsigned long  trace_mode;

  {
  extern FILE          *bvl_y_in      ;
  extern long            bvl_y_parse ();
  char                 *tok           ;
  char                 *str           ;
  unsigned long          check_mode    ;
  struct chain         *behsfx_lst    = NULL;
  struct chain         *suffix        = NULL;
  static unsigned long   call_nbr      = 0   ;

	/* ###------------------------------------------------------### */
	/*    read the environment variable VH_BEHSFX to create a list	*/
	/* of suffix for behavioural files.				*/
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
	/*    searching the root file					*/
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
	/*    call the compiler on the current file			*/
	/*      - print a message if the trace mode is actif		*/
	/*      - add internal signals to the primary input list if	*/
	/*        "keep auxiliary" mode is actif			*/
	/* ###------------------------------------------------------### */

  if ((trace_mode & BVL_TRACE) != 0)
    beh_message (3, BVL_CURFIL);

  if ((trace_mode & BVL_KEEPAUX) != 0)
    BVL_AUXMOD = 1;
  else
    BVL_AUXMOD = 0;

  BEH_LINNUM = 1;

  if (call_nbr != 0)
    bvl_y_restart (bvl_y_in);
  call_nbr++;

  bvl_y_parse ();
  fclose (bvl_y_in);
  bvl_y_in = NULL;

	/* ###------------------------------------------------------### */
	/*    check the consistency of the compiled description		*/
	/* ###------------------------------------------------------### */

  if ((trace_mode & BVL_CHECKEMPTY) != 0)
    {
    check_mode        = BEH_CHK_EMPTY;
    BVL_HEDFIG->FLAG |= BEH_ARC_C    ;
    }
  else
    {
    check_mode        = BEH_CHK_DRIVERS;
    BVL_HEDFIG->FLAG |= BEH_ARC_VHDL   ;
    }

  BVL_HEDFIG->ERRFLG = beh_chkbefig (BVL_HEDFIG, check_mode);

  return (BVL_HEDFIG);
  }
