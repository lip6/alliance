/* ###--------------------------------------------------------------### */
/*                                                                      */
/* file         : mvl_drive.c                                           */
/* date         : Sep 24 1993                                           */
/* author       : VUONG Huu Nghia, D. Hommais                           */
/* description  : This file contains a MBK-->VHDL driver :              */
/* functions    : vhdlsavelofig()                                       */
/*                                                                      */
/* ###--------------------------------------------------------------### */
#ident "$Id: mvl_drive.c,v 1.2 2006/03/29 17:10:38 xtof Exp $"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <mut.h>
#include <mlo.h>
#include "gen_generic.h"
#include "mvl_utype.h"
#include "mvl_utdef.h"
#include "mvl_drive.h"

static void drv_genvalue(FILE *ptfile, logen_list *ptgen)
{
   logen_list *l;

   switch (ptgen->TYPE) {
      case GENTYPE_BIT:
         fprintf(ptfile, "'%c'", ptgen->VALUE.CHAR);
         break;
      case GENTYPE_VAL:
         fprintf(ptfile, "%ld", ptgen->VALUE.VAL);
         break;
      case GENTYPE_ARG:
         fprintf(ptfile, "%ld", ptgen->VALUE.VAL);
         break;
      case GENTYPE_TEXT:
         fputs(ptgen->VALUE.TEXT, ptfile);
         break;
      case GENTYPE_LIST:
         fputc('(', ptfile);
         for (l=ptgen->VALUE.LIST; l; l=l->NEXT) {
            drv_genvalue(ptfile, l);
            if (l->NEXT)
               fputc(',', ptfile);
         }
         fputc(')', ptfile);
         break;
   }
}

static void drv_generic(FILE *ptfile, ptype_list *pttype)
{
struct logen *ptgen;

   if ( (pttype=getptype(pttype,LOGEN)) ) {
      fprintf (ptfile,"   generic (\n");
      for (ptgen=pttype->DATA;ptgen;ptgen=ptgen->NEXT) {
         fprintf (ptfile,"      CONSTANT %s : ",ptgen->NAME);
         switch(ptgen->TYPE) {
            case GENTYPE_BIT:
               fputs("bit     := ", ptfile);
               break;
            case GENTYPE_VAL:
               fputs("natural := ", ptfile);
               break;
            case GENTYPE_ARG:
               fputs("arg     := ", ptfile);
               break;
            case GENTYPE_TEXT:
               fputs("string  := ", ptfile);
               break;
            case GENTYPE_LIST:
               fputs("list    := ", ptfile);
               break;
         }
         drv_genvalue(ptfile, ptgen);
         if (ptgen->NEXT)
            fputs(";\n",ptfile);
      }
      fputs("\n   );\n",ptfile);
   }
}

/* ###--------------------------------------------------------------### */
/* function     : vhdlsavelofig                                         */
/* description  : print out a texte file containing a structural VHDL   */
/*                description                                           */
/* called func. : mvl_vhdlname   , getsigname, mvl_message, mvl_error  ,*/
/*                mvl_toolbug, getptype, reverse, mvl_abl2str           */
/*                                                                      */
/* ###--------------------------------------------------------------### */
void xxvhdlsavelofig (ptfig)

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
  FILE         *ptfile       ;
  time_t        clock        ;
  char         *mode         = NULL;
  char         *name         ;
  char         *sig_name     ;
  int           left         ;
  int           right        ;
  int           i            ;
  char          new_name[200];
  char          first        = 1   ;

  if (ptfig == NULL)
    mvl_toolbug (10, "mvl_decomp", NULL, 0);

        /* ###------------------------------------------------------### */
        /*    Opening result file                                       */
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
        /*    Entity declaration                                        */
        /* ###------------------------------------------------------### */

  fprintf (ptfile,"-- Entity Declaration\n\n");
  fprintf (ptfile,"ENTITY %s IS\n", mvl_vhdlname (ptfig->NAME));

        /* ###------------------------------------------------------### */
        /*    Generic declaration                                       */
        /* ###------------------------------------------------------### */

  drv_generic(ptfile, ptfig->USER);

        /* ###------------------------------------------------------### */
        /*    Port declaration                                          */
        /* ###------------------------------------------------------### */

  if (ptfig->LOCON != NULL)
    {
    fprintf (ptfile,"  PORT (\n");
    ptfig->LOCON = (struct locon *) reverse ((chain_list *)ptfig->LOCON);
    ptcon        = ptfig->LOCON;
    while (ptcon != NULL)
      {
      switch (ptcon->DIRECTION)
        {
        case 'I':
          mode = namealloc ("in");
          break;
        case 'O':
          mode = namealloc ("out");
          break;
        case 'B':
          mode = namealloc ("inout");
          break;
        case 'Z':
        case 'T':
        case 'X':
          mode = namealloc ("linkage");
          break;
        }
      ptcon = (struct locon *) mvl_vectnam (ptcon,&left,&right,&name,1);
      if (left != -1)
        {
        fprintf (ptfile,"  %s : %s BIT_VECTOR (%d %s %d)",
                      mvl_vhdlname (name), mode, left,
                      (left >= right)? "DOWNTO":"TO",right);
        }
      else
        {
        fprintf (ptfile,"  %s : %s BIT",mvl_vhdlname(name),mode);
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
        /*    Architecture declaration                                  */
        /* ###------------------------------------------------------### */

  fprintf (ptfile,"-- Architecture Declaration\n\n");
  fprintf (ptfile,"ARCHITECTURE structural_view OF %s IS\n",
                  mvl_vhdlname(ptfig->NAME));

        /* ###------------------------------------------------------### */
        /*    Component declaration : first make a list of models, then */
        /* for each model print out a "component declaration"           */
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
    fprintf (ptfile,"  COMPONENT %s\n",
                    mvl_vhdlname(ptins->FIGNAME));

    /* ###------------------------------------------------------------### */
    /*  For each model, I just look for an instance and write the generic */
    /* list. So the default value for a generic of the model is the value */
    /* of the first found instance generic.                               */
    /* ###------------------------------------------------------------### */

    drv_generic(ptfile, ((loins_list *)ptchain->DATA)->USER);

    fputs("   port (\n",ptfile);

    ptins->LOCON = (struct locon *) reverse ((chain_list *)ptins->LOCON);
    ptcon = ptins->LOCON;
    while (ptcon != NULL)
      {
      switch (ptcon->DIRECTION)
        {
        case 'I':
          mode = namealloc ("in");
          break;
        case 'O':
          mode = namealloc ("out");
          break;
        case 'B':
          mode = namealloc ("inout");
          break;
        case 'Z':
        case 'T':
        case 'X':
          mode = namealloc ("linkage");
          break;
        }
      ptcon = (struct locon *) mvl_vectnam (ptcon,&left,&right,&name,1);
      if (left != -1)
        {
        fprintf (ptfile,"    %s : %s BIT_VECTOR(%d %s %d)",
                     mvl_vhdlname(name), mode, left,
                     (left >= right)?"DOWNTO":"TO",right);
        }
      else
        fprintf(ptfile,"    %s : %s BIT", mvl_vhdlname(name),mode);

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
        /*    Signal declaration                                        */
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
        /*    Description block                                         */
        /* ###------------------------------------------------------### */

  fprintf (ptfile,"\nBEGIN\n\n");
  ptfig->LOINS = (struct loins *) reverse ((chain_list *)ptfig->LOINS);

  for (ptins = ptfig->LOINS; ptins; ptins = ptins->NEXT)
    {
        /* ###------------------------------------------------------### */
        /*   Instantiation of a model                                   */
        /* ###------------------------------------------------------### */

    fprintf (ptfile,"  %s : %s\n",
             mvl_vhdlname(ptins->INSNAME),mvl_vhdlname(ptins->FIGNAME));

        /* ###------------------------------------------------------### */
        /*   Generic map                                                */
        /* ###------------------------------------------------------### */

    {
    logen_list *g;
    ptype_list *p;

       p=getptype(ptins->USER,LOGEN);
       if (p)  {
          fputs("   Generic Map (\n",ptfile);
          for (g=p->DATA;g;g=g->NEXT) {
             fprintf(ptfile,"      %s => ",g->NAME);
             drv_genvalue(ptfile, g);
             if (g->NEXT)
                fputs(",\n",ptfile);
          }
       fputs("   )\n",ptfile);
       }
    }

        /* ###------------------------------------------------------### */
        /*   Port map                                                   */
        /* ###------------------------------------------------------### */

    fputs("   PORT MAP (\n",ptfile);
    ptcon = ptins->LOCON; 
    while (ptcon != NULL)
      {
      mvl_vectnam (ptcon,&left,&right,&name,1);
      fprintf (ptfile,"    %s => ",mvl_vhdlname (name));

      if (left != -1)
        {
        /* ###------------------------------------------------------### */
        /*    The connected signals are bused                           */
        /* ###------------------------------------------------------### */

        for (i=abs(left-right) ; i>=0 ; i--)
          {
          ptsig = ptcon->SIG;
          if (ptsig->TYPE == 'I')
            {
            /* ###-------------------------------------------------### */
            /*    The signal is internal                               */
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
        /*    The connected signals is simple                           */
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
  fprintf (ptfile, "\nend structural_view;\n");
  fclose  (ptfile);
  }
