/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
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
/*									*/
/* file		: mvl_drive.c						*/
/* date		: Sep 24 1993						*/
/* author	: VUONG Huu Nghia					*/
/* description	: This file contains a MBK-->VHDL driver :		*/
/* functions    : vhdlsavelofig()					*/
/*									*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <mut.h>
#include <mlo.h>
#include <stdlib.h>
#include "mvl_utype.h"
#include "mvl_utdef.h"
#include "mvl_drive.h"

/* ###--------------------------------------------------------------### */
/* function	: vhdlsavelofig						*/
/* description	: print out a texte file containing a structural VHDL	*/
/*		  description						*/
/* called func.	: mvl_vhdlname   , getsigname, mvl_message, mvl_error  ,*/
/*		  mvl_toolbug, getptype, reverse, mvl_abl2str		*/
/*									*/
/* ###--------------------------------------------------------------### */
void vhdlsavelofig (ptfig)

struct lofig *ptfig;
  {
  extern char  *getsigname ();
  struct loins *ptins        = NULL;
  struct locon *ptcon        = NULL;
  struct locon *ptscan       = NULL;
  struct losig *ptsig        = NULL;
  struct chain *ptmodel      = NULL;
  struct chain *ptchain      = NULL;
  struct chain *sig_list     = NULL;
  FILE	       *ptfile       ;
  time_t        clock        ;
  char 	       *mode         = NULL;
  char         *name         ;
  char         *sig_name     ;
  int           left         ;
  int           right        ;
  int           i            ;
  char          new_name[200];
  char          first        = 1   ;
  char* bit_type, *vect_type, *bus_type;


  if (ptfig == NULL)
    mvl_toolbug (10, "mvl_decomp", NULL, 0);

	/* ###------------------------------------------------------### */
	/*    Opening result file					*/
	/* ###------------------------------------------------------### */

  if ((ptfile = mbkfopen (ptfig->NAME, "vst", "w")) == NULL)
    {
    mvl_error (107, NULL);
    exit (1);
    }

  time (&clock);
  fprintf (ptfile,"-- VHDL structural description generated from `%s`\n",
           ptfig->NAME);
  fprintf (ptfile,"--\t\tdate : %s\n\n",ctime(&clock));

	/* ###------------------------------------------------------### */
	/*    Entity declaration					*/
	/* ###------------------------------------------------------### */

  fprintf (ptfile,"-- Entity Declaration\n\n");
  fprintf (ptfile,"ENTITY %s IS\n", mvl_vhdlname (ptfig->NAME));

	/* ###------------------------------------------------------### */
	/*    Port declaration						*/
	/* ###------------------------------------------------------### */

  if (ptfig->LOCON != NULL)
    {
    fprintf (ptfile,"  PORT (\n");
    ptfig->LOCON = (struct locon *) reverse ((chain_list *)ptfig->LOCON);
    ptcon        = ptfig->LOCON;
    while (ptcon != NULL)
      {/*modified by Francois Donnet 29/08/2000 */
      switch (ptcon->DIRECTION)
        {
        case IN:
          mode = namealloc ("in");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        case OUT:
          mode = namealloc ("out");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        case INOUT:
          mode = namealloc ("inout");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        case TRANSCV:   
          mode = namealloc ("inout");
          vect_type = "MUX_VECTOR";
          bit_type = "MUX_BIT";
          bus_type = "bus";
          break;
        case TRISTATE: 
          mode = namealloc ("out");
          vect_type = "MUX_VECTOR";
          bit_type = "MUX_BIT";
          bus_type = "bus";
          break;
        case 'X': default:
          mode = namealloc ("linkage");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        }
      ptcon = (struct locon *) mvl_vectnam (ptcon,&left,&right,&name,1);
      if (left != -1)
        {
        fprintf (ptfile,"  %s : %s %s(%d %s %d) %s",
                      mvl_vhdlname (name), mode, vect_type, left,
                      (left >= right)? "DOWNTO":"TO",right,bus_type);
        }
      else
        {
        fprintf (ptfile,"  %s : %s %s %s",mvl_vhdlname(name),mode,bit_type,bus_type);
        }

      if (ptcon->NEXT != NULL)
        fprintf (ptfile,";\t-- %s\n",name);
      else
        fprintf (ptfile,"\t-- %s\n",name);

      ptcon = ptcon->NEXT;
      mbkfree (name);
      }
    fprintf (ptfile, "  );\n");
    }
  fprintf (ptfile,"END %s;\n\n",mvl_vhdlname(ptfig->NAME));
  ptfig->LOCON = (struct locon *) reverse ((chain_list *)ptfig->LOCON);

	/* ###------------------------------------------------------### */
	/*    Architecture declaration					*/
	/* ###------------------------------------------------------### */

  fprintf (ptfile,"-- Architecture Declaration\n\n");
  fprintf (ptfile,"ARCHITECTURE VST OF %s IS\n",
                  mvl_vhdlname(ptfig->NAME));

	/* ###------------------------------------------------------### */
	/*    Component declaration : first make a list of models, then	*/
	/* for each model print out a "component declaration"		*/
	/* ###------------------------------------------------------### */

  ptmodel = NULL;
  for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
    {
    for (ptchain = ptmodel ; ptchain ; ptchain = ptchain->NEXT)
      {
      if (((struct loins *)ptchain->DATA)->FIGNAME == ptins->FIGNAME)
        break;
      }
    if (ptchain == NULL)
      ptmodel = addchain(ptmodel,ptins);
    }

  for (ptchain = ptmodel ; ptchain ; ptchain = ptchain->NEXT)
    {
    ptins = (struct loins *)ptchain->DATA;
    fprintf (ptfile,"  COMPONENT %s\n    port (\n",
                    mvl_vhdlname(ptins->FIGNAME));

    ptins->LOCON = (struct locon *) reverse ((chain_list *)ptins->LOCON);
    ptcon = ptins->LOCON;
    while (ptcon != NULL)
      {
      switch (ptcon->DIRECTION)
        {
        case IN:
          mode = namealloc ("in");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        case OUT:
          mode = namealloc ("out");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        case INOUT:
          mode = namealloc ("inout");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        case TRANSCV:   
          mode = namealloc ("inout");
          vect_type = "MUX_VECTOR";
          bit_type = "MUX_BIT";
          bus_type = "bus";
          break;
        case TRISTATE: 
          mode = namealloc ("out");
          vect_type = "MUX_VECTOR";
          bit_type = "MUX_BIT";
          bus_type = "bus";
          break;
        case 'X': default:
          mode = namealloc ("linkage");
          vect_type = "BIT_VECTOR";
          bit_type = "BIT";
          bus_type = "";
          break;
        }
      ptcon = (struct locon *) mvl_vectnam (ptcon,&left,&right,&name,1);
      if (left != -1)
        {
        fprintf (ptfile,"    %s : %s %s(%d %s %d) %s",
                      mvl_vhdlname (name), mode, vect_type, left,
                      (left >= right)? "DOWNTO":"TO",right,bus_type);
        }
      else
        {
        fprintf (ptfile,"    %s : %s %s %s",mvl_vhdlname(name),mode,bit_type,bus_type);
        }

      if (ptcon->NEXT != NULL)
         fprintf(ptfile, ";\t-- %s\n",name);
      else
         fprintf(ptfile, "\t-- %s\n",name);

      ptcon = ptcon->NEXT;
      free(name);
      }
    ptins->LOCON = (struct locon *) reverse ((chain_list *)ptins->LOCON);
    fprintf (ptfile, "    );\n  END COMPONENT;\n\n");
    }

  freechain (ptmodel);

	/* ###------------------------------------------------------### */
	/*    Signal declaration					*/
	/* ###------------------------------------------------------### */

    ptsig = ptfig->LOSIG;
    while (ptsig != NULL)
      {
      if (ptsig->TYPE == 'I')
        {
        fprintf (ptfile,"  SIGNAL %s : BIT;\t-- %s\n",
                 mvl_vhdlname(getsigname(ptsig)),getsigname(ptsig));
        }
      ptsig = ptsig->NEXT;       
      }

	/* ###------------------------------------------------------### */
	/*    Description block						*/
	/* ###------------------------------------------------------### */

  fprintf (ptfile,"\nBEGIN\n\n");
  ptfig->LOINS = (struct loins *) reverse ((chain_list *)ptfig->LOINS);

  for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
    {
	/* ###------------------------------------------------------### */
        /*   Instantiation of a model					*/
	/* ###------------------------------------------------------### */

    fprintf (ptfile,"  %s : %s\n    PORT MAP (\n",
             mvl_vhdlname(ptins->INSNAME),mvl_vhdlname(ptins->FIGNAME));
    ptcon = ptins->LOCON; 
    while (ptcon != NULL)
      {
      mvl_vectnam (ptcon,&left,&right,&name,1);
      fprintf (ptfile,"    %s => ",mvl_vhdlname (name));

      if (left != -1)
        {
	/* ###------------------------------------------------------### */
        /*    The connected signals are bused				*/
	/* ###------------------------------------------------------### */

        for (i=abs(left-right) ; i>=0 ; i--)
          {
          ptsig = ptcon->SIG;
          if (ptsig->TYPE == 'I')
            {
	    /* ###-------------------------------------------------### */
            /*    The signal is internal			       */
	    /* ###-------------------------------------------------### */

            sig_name = namealloc (mvl_vhdlname(getsigname(ptsig)));
            sig_list = addchain  (sig_list, sig_name);
            }
          else
            {
	    /* ###-------------------------------------------------### */
            /*    The signal is external                               */
	    /* ###-------------------------------------------------### */

            for (ptscan = ptfig->LOCON ; ptscan ; ptscan = ptscan->NEXT)
	      {
	      if (ptscan->SIG == ptsig) 
                break;
	      }
            if (ptscan == NULL)
              {
	      printf ("\n*** mbk error *** no external connector  \n");
	      printf ("     driving vhdl file %s\n", ptfig->NAME);
	      }
            else
              {
              mvl_name (ptscan->NAME,new_name);
              sig_name = namealloc (new_name);
              sig_list = addchain  (sig_list, sig_name);
	      }
            }
          if (i > 0)
            ptcon = ptcon->NEXT;
          }

        first = 1;
        while (sig_list != NULL)
          {
          if (first != 1)
            {
            fprintf (ptfile,"& %s",(char *)(sig_list->DATA));
            }
          else
            {
            fprintf (ptfile,"%s",(char *)(sig_list->DATA));
            first = 0;
            }
          sig_list = sig_list->NEXT;
          }
        }
      else
        {
	/* ###------------------------------------------------------### */
        /*    The connected signals is simple				*/
	/* ###------------------------------------------------------### */

        ptsig = ptcon->SIG;
        if (ptsig->TYPE == 'I')
          {
          fprintf (ptfile, "%s", mvl_vhdlname (getsigname(ptsig)));
          }
        else
          {
          for (ptscan = ptfig->LOCON ; ptscan ; ptscan = ptscan->NEXT)
	    {
	    if (ptscan->SIG == ptsig) 
              break;
	    }
          if (ptscan == NULL)
            {
	    printf ("\n*** mbk error *** no external connector  \n");
	    printf ("     driving vhdl file %s\n", ptfig->NAME);
	    }
          else
            {
	    mvl_name (ptscan->NAME,new_name);
	    fprintf(ptfile, "%s", new_name);
	    }
          }
        }
      if (ptcon->NEXT != NULL)
        fprintf (ptfile, ",\n");
      ptcon = ptcon->NEXT;
      free(name);
      }
    fprintf(ptfile, ");\n");
    }
  ptfig->LOINS = (struct loins *) reverse ((chain_list *)ptfig->LOINS);
  fprintf (ptfile, "\nend VST;\n");
  fclose  (ptfile);
  }
