/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:18 $
   $Log: display.c,v $
   Revision 1.1  2002/03/15 14:37:18  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:16  hcl
   First commit in the new tree.

   Revision 1.3  2002/02/12 15:14:14  hcl
   New algo.

   Revision 1.2  2001/12/14 15:58:35  hcl
   indent *.c, quelques bugs en moins, non-placement des connecteurs.

   Revision 1.1  2001/09/26 14:27:48  hcl
   premier commit....

   ### -------------------------------------------------- ### 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "display.h"

static char *res_id = "$Id: display.c,v 1.1 2002/03/15 14:37:18 hcl Exp $";

/**
 * fonction d'affichage standard
 * avec 5 niveaux 
**/
void display(int i_nLevel, int i_nType, char *fmt, ...)
{
    va_list l_pParam;

    va_start(l_pParam, fmt);
    switch (i_nType) {
    case ERROR:
        fprintf(stderr, "ERROR : ");
        vfprintf(stderr, fmt, l_pParam);
        break;
    case WARNING:
        fprintf(stderr, "WARNING : ");
        vfprintf(stderr, fmt, l_pParam);
        break;
    case INFO:
        vfprintf(stdout, fmt, l_pParam);
        break;
    case VVERB:
        if ((i_nLevel == DEBUG) || (i_nLevel == VVERB))
            vfprintf(stdout, fmt, l_pParam);
        break;
    case VERBOSE:
        if ((i_nLevel == VERBOSE) || (i_nLevel == DEBUG) || (i_nLevel == VVERB))
            vfprintf(stdout, fmt, l_pParam);
        break;
    case DEBUG:
        if (i_nLevel == DEBUG)
            vfprintf(stdout, fmt, l_pParam);
    }
    va_end(l_pParam);
}
