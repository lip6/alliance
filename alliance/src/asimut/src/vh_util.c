
/* ###--------------------------------------------------------------### */
/* file		: vh_util.c						*/
/* date		: Dec 13 1999						*/
/* author	: Pirouz BAZARGAN SABET					*/
/* version	: v3.02							*/
/* description	: This file contains some utility functions :		*/
/*		  vhu_splitname, vhu_alloc, vhu_message, vhu_warning,	*/
/*		  vhu_toolbug  , vhu_error, vhu_avers			*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "vh_util.h"

/* ###--------------------------------------------------------------### */
/* function	: vhu_splitname						*/
/* description	: split a name onto an instance name and a signal name	*/
/* called func.	: namalloc						*/
/* ###--------------------------------------------------------------### */

void vhu_splitname (name, ins_name, sig_name)

char  *name    ;
char **ins_name;
char **sig_name;

  {
  unsigned int i         = 0;
  unsigned int j         = 0;
  char         buf [256] ;

  for (i=0 ; name [i] != '\0' ; i++)
    {
    if (name [i] == '.')
      j = i + 1;
    }

  if ((j != 0) && (j != i))
    {
    strcpy (buf, name);
    buf [j-1] = '\0';
    *ins_name = namealloc (buf    );
    *sig_name = namealloc (buf + j);
    }
  else
    {
    *ins_name = NULL;
    *sig_name = name;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhu_alloc						*/
/* description	: make memory allocation				*/
/* called func.	: mbkalloc						*/
/* ###--------------------------------------------------------------### */

char *vhu_alloc (size, mode)

unsigned int size;
char         mode;

  {
  static char         *mem_pt  = NULL;
  static unsigned int  mem_idx = 0   ;
  char                *res_pt  = NULL;

	/* ###------------------------------------------------------### */
	/*    align the size on a multiple of 4				*/
	/* ###------------------------------------------------------### */

  if (size >= VHU_MEMALC)
    res_pt = (char *) mbkalloc (size);
  else
    {
    if (mode == VHU_ALIGN)
      {

      #ifdef SYSTEM64
        mem_idx = (mem_idx + 7) & 0xfffffff8;
        size    = (size    + 7) & 0xfffffff8;
      #else
        mem_idx = (mem_idx + 3) & 0xfffffffc;
        size    = (size    + 3) & 0xfffffffc;
      #endif

      }

    if ((mem_pt == NULL) || ((mem_idx + size) > VHU_MEMALC))
      {
      mem_pt  = (char *) mbkalloc (VHU_MEMALC);
      mem_idx = 0;
      }

    res_pt   = mem_pt + mem_idx;
    mem_idx += size   ;
    }

  return (res_pt);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhu_avers						*/
/* description	: return the current version				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

char *vhu_avers ()
  {
  return ("v3.02");
  }

/* ###--------------------------------------------------------------### */
/* function	: vhu_warning						*/
/* description	: print a warning message corresponding to the argument	*/
/*		  code							*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhu_warning (code, str1, str2)

int   code;
char *str1;
char *str2;

  {
  fprintf (stderr, "Warning %d : ", code);
  switch (code)
    {
    case 1:
      fprintf (stderr, "cannot find register `%s`\n", str1);
      break;
    case 2:
      fprintf (stderr, "consistency checks will be disabled\n");
      break;
    case 3:
      fprintf (stderr, "signal `%s` never used\n", str1);
      break;
    case 4:
      fprintf (stderr, "cannot restore the mode of port `%s`\n", str1);
      break;
    case 5:
      fprintf (stderr, "pattern is loaded while the scheduler isn't empty\n");
      break;

    case 112:
      fprintf (stderr, "driver conflict on `%s`\n", str1);
      break;
    case 121:
      fprintf (stderr, "driver conflict on `%s` in `%s`\n", str2, str1);
      break;
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhu_toolbug						*/
/* description	: print a fatal error message corresponding to the	*/
/*		  argument code and exit				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhu_toolbug (code, str1, str2, nbr1)

int   code;
char *str1;
char *str2;
int   nbr1;

  {
  fprintf (stderr, "Fatal error %d executing `%s`: ", code, str1);
  switch (code)
    {
    case 1:
      fprintf (stderr, "unknown operator\n");
      break;
    case 2:
      fprintf (stderr, "cannot create empty atom\n");
      break;
    case 7:
      fprintf (stderr, "unknown type `%c` for IO `%s`\n", nbr1, str2);
      break;
    case 8:
      fprintf (stderr, "illegal signal value : hex `%x`\n", nbr1);
      break;
    case 13 :
      fprintf (stderr, "cannot find model of `%s`\n", str2);
      break;
    case 18:
      fprintf (stderr, "unknown mode `%c` for port `%s`\n", nbr1, str2);
      break;
    case 19:
      fprintf (stderr, "unknown type `%c` for input `%s`\n", nbr1, str2);
      break;
    case 20:
      fprintf (stderr, "`%s` no such locon\n", str2);
      break;
    case 21:
      fprintf (stderr, "illegal direction `%c` for `%s`\n", nbr1, str2);
      break;
    case 22:
      fprintf (stderr, "`%s`: `%c` unknown resolution func.\n", str2, nbr1);
      break;
    default:
      fprintf (stderr, "BUG\n");
    }
  exit (1);
  }

/* ###--------------------------------------------------------------### */
/* function	: vhu_message						*/
/* description	: print a message corresponding to the argument code	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void vhu_message (code, str1, nmb1)

int   code;
char *str1;
int   nmb1;

  {
  switch (code)
    {
    case 0:
      fprintf (stdout, "\nasimut %s\n", vhu_avers());
      break;
    case 1:
      fprintf (stdout, "usage : asimut [options] [files]\n");
      fprintf (stdout, "      : asimut -h\n");
      break;
    case 2:
      fprintf (stdout, "initializing ...\n");
      break;
    case 3:
      fprintf (stdout, "compiling `%s` (Behaviour) ...\n",str1);
      break;
    case 4:
      fprintf (stdout, "compiling `%s` (Structural) ...\n\n",str1);
      break;
    case 5:
      fprintf (stdout, "searching `%s` ...\n",str1);
      break;
    case 6:
      fprintf (stdout, "searching pattern file : `%s` ...\n",str1);
      break;
    case 8:
      fprintf (stdout, "compiling `%s` (Pattern) ...\n\n",str1);
      break;
    case 9:
      fprintf (stdout, "linking ...\n");
      break;
    case 10:
      fprintf (stdout, "###----- processing pattern %d -----###\n",nmb1);
      break;
    case 11:
      fprintf (stdout, "executing ...\n");
      break;
    case 12:
      fprintf (stdout, "generating the file '%s' ...\n",str1);
      break;
    case 13:
      fprintf (stdout, "of the figure '%s'.\n",str1);
      break;
    case 14:
      fprintf (stdout, "file '%s' has been generated.\n",str1);
      break;
    case 15:
      fprintf (stdout, "making %s ...\n\n", str1);
      break;
    case 16:
      fprintf (stdout, "restoring ...\n\n");
      break;
    case 17:
      fprintf (stdout, "creating the core file ...\n\n");
      break;
    case 18:
      fprintf (stdout, "flattening the root figure ...\n\n");
      break;
    case 19:
      fprintf (stdout, "stop mark set on the current structure\n");
      break;
    case 20:
      fprintf (stdout, "--- STACK OVERFLOW !! ---\n");
      break;
    case 21:
      fprintf (stdout, "\tParis, France, Europe, Earth, Solar system, Milky Way, ...\n");
      break;
    case 22:
      fprintf (stdout, "###----- processing pattern %d : %s -----###\n",nmb1, str1);
      break;
    case 100:
      fprintf (stdout, "\nasimut %s\n", vhu_avers());
      break;
    case 101:
      fprintf (stdout, "usage : asimut [options] [files]\n");
      fprintf (stdout, "      : asimut -h\n");
      break;
    default:
      fprintf (stderr, "vhu_message : code %d unknown.\n",code);
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: vhu_error						*/
/* description	: print an error message depending on the argument code	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

int vhu_error (code, str1, str2)

int   code;
char *str1;
char *str2;

  {
  if (code < 200)
    fprintf (stderr, "Error %d: ", code);

  switch (code)
    {
    case 100:
      fprintf (stderr, "cannot find `%s`\n", str1);
      break;
    case 101:
      fprintf (stderr, "cannot find signal `%s`\n", str1);
      break;
    case 102:
      fprintf (stderr, "instance and model of `%s` mismatch\n", str1);
      break;
    case 103:
      fprintf (stderr, "incompatible input/output mode for `%s`\n", str1);
      break;
    case 104:
      fprintf (stderr, "input/output `%s` doesn't exist\n", str1);
      break;
    case 105:
      fprintf (stderr, "possible combinatory loop. Simulation aborted\n");
      break;
    case 106:
      fprintf (stderr, "too many errors. Simulation aborted\n");
      break;
    case 107:
      fprintf (stderr, "cannot open result file\n");
      break;
    case 109:
      fprintf (stderr, "signal `%s` never assigned\n", str1);
      break;
    case 110:
      fprintf (stderr, "signal `%s` not connected to any port\n", str1);
      break;
    case 111:
      fprintf (stderr, "illegal connections on signal `%s`\n", str1);
      break;
    case 112:
      fprintf (stderr, "drivers conflict on `%s`\n", str1);
      break;
    case 113:
      fprintf (stderr, "expected value differs from the simulation's result on `%s`\n", str1);
      break;
    case 114:
      fprintf (stderr, "multiple driver on unguarded signal `%s`\n", str1);
      break;
    case 115:
      fprintf (stderr, "save file doesn't match with the description\n");
      break;
    case 116:
      fprintf (stderr, "cannot open file `%s`\n", str1);
      break;
    case 117:
      fprintf (stderr, "instance `%s` doesn't exist or cannot be inspected\n", str1);
      break;
    case 118:
      fprintf (stderr, "command not understood\n");
      break;
    case 119:
      fprintf (stderr, "cannot display VOID pointer\n");
      break;
    case 120:
      fprintf (stderr, "cannot push NULL pointer\n");
      break;
    case 121:
      fprintf (stderr, "driver conflict on `%s` in `%s`\n", str2, str1);
      break;

    case 200:
      fprintf (stderr, "\n\tcannot continue further more.\n");
      fprintf (stderr, "\n\t\thave a nice day...\n");
      break;

    default:
      fprintf (stderr, "unknown error code\n");
      break;
    }

  return (1);
  }
