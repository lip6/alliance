/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_parse.c						*/
/* date		: Feb 15 1995						*/
/* author	: L.A TABUSSE & H.N. VUONG & P. BAZARGAN-SABET		*/
/* description	: Parser VHDL --> MBK					*/
/*									*/
/* ###--------------------------------------------------------------### */
#ident "$Id: mvl_parse.c,v 1.1 2002/04/26 09:51:07 ludo Exp $"
#include <stdio.h>
#include <sys/types.h>
#include <mut.h>
#include <mlo.h>
#include "mvl_parse.h"
#include "mvl_utdef.h"

void vhdlloadlofig (pt_lofig, figname, mode)

struct lofig *pt_lofig;
char         *figname ;
char          mode    ;

  {
  struct lofig *pt_lofig_tmp;
  struct loins *pt_loins   ;
  struct loins *pt_loinsaux;
  struct locon *pt_locon   ;
  struct locon *pt_loconaux;
  struct losig *pt_losig   ;
  struct losig *pt_losigaux;
  char          filename[120];
  char          local_mbk_fast_mode;
  static int    call_nbr = 0;

  /* Initialization of some variables */
  MVL_LINNUM = 1;
  MVL_MBKMOD = mode;
  strcpy (MVL_CURFIL, figname);

  /* FAST_MODE asked for MBK */
  local_mbk_fast_mode = FAST_MODE;
  FAST_MODE = 'Y';

  if ((mode != 'A') && (mode != 'P') && (mode != 'C'))
    {
    printf("vhdlloadfig : Bad mode '%c' asked\n", mode);
    FAST_MODE = local_mbk_fast_mode;
    EXIT(1);
    }

  /* Opening file */
  mvl_y_in = (FILE *) mbkfopen(figname, IN_LO, READ_TEXT);
  if((mvl_y_in == NULL) && (mode == 'P'))
    {
    strcpy(IN_LO, "vbe");
    mvl_y_in = (FILE *) mbkfopen(figname, IN_LO, READ_TEXT);
    }

  if(mvl_y_in == NULL)
    {
    (void)fprintf(stderr,"\n*** mbk error *** can't open file : %s.%s\n",
						figname, IN_LO);
    FAST_MODE = local_mbk_fast_mode;
    EXIT(1);
    }

  /* TRACE_MODE asked for MBK */
  if(TRACE_MODE == 'Y')
    {
    (void)printf("\n--- mbk --- parsing file : %s.%s in mode : %c\n",
                figname, IN_LO, mode);
    }

  MVL_LOFPNT = pt_lofig; /* passing main parameter */

  if (call_nbr != 0)
    mvl_y_restart (mvl_y_in);

  call_nbr ++;

  /* -------------------------------------------------------------------*/
  /* Parsing  : If mode is P or A, then normal parsing, if mode is C	*/
  /* then parsing of a new figure, then from the new one, we fill the   */
  /* old one.								*/
  /* -------------------------------------------------------------------*/

  if((mode == 'P') || (mode == 'A'))
    {
    if(mvl_y_parse() != 0)
      {
      (void)fprintf(stderr,"\n*** mbk error *** abnormal parsing for : %s\n",filename);
      FAST_MODE = local_mbk_fast_mode;
      EXIT(1);
      }
    }

  if(mode == 'C')
    {
    /* ----------------------------------------------------------------	*/
    /* Saving the lofig pointer, creating a new one to allow the 	*/
    /* parsing of the figure in 'A' mode.				*/
    /* ----------------------------------------------------------------	*/
    pt_lofig_tmp            = pt_lofig; 
    MVL_LOFPNT              = (lofig_list *)mbkalloc(sizeof(lofig_list));
    MVL_LOFPNT->MODE        = 'A';
    MVL_LOFPNT->NAME        = namealloc(figname);
    MVL_LOFPNT->MODELCHAIN  = NULL;
    MVL_LOFPNT->LOINS       = NULL;
    MVL_LOFPNT->LOTRS       = NULL;
    MVL_LOFPNT->LOCON       = NULL;
    MVL_LOFPNT->LOSIG       = NULL;
    MVL_LOFPNT->BKSIG       = NULL;
    MVL_LOFPNT->USER        = NULL;
    MVL_LOFPNT->NEXT        = NULL;

    MVL_MBKMOD = 'A';

    if(mvl_y_parse() != 0)
      {
      (void)fprintf(stderr,"\n*** mbk error *** abnormal parsing for : %s\n",filename);
      FAST_MODE = local_mbk_fast_mode;
      EXIT(1);
      }
    /* ----------------------------------------------------------------	*/
    /* Now, with the new figure, we duplicate the new informations	*/
    /* to fill the old one.						*/
    /* ----------------------------------------------------------------	*/
    pt_lofig = mvl_fill(pt_lofig_tmp, MVL_LOFPNT);
    }

  MVL_MBKMOD = mode;
  /* Closing file */
  if(fclose(mvl_y_in) != 0)
    {
    (void)fprintf(stderr,"\n*** mbk error *** can't close file : %s\n",filename);
    FAST_MODE = local_mbk_fast_mode;
    EXIT(1);
    }

  if(strcmp(IN_LO,"vbe") == 0)
    {
    strcpy(IN_LO,"vst");
    return;
    }

 if (mode == 'P' )
   {
   pt_locon = MVL_LOFPNT->LOCON;
   while (pt_locon != NULL)
     {
     if (pt_locon->TYPE == 'I')
       {
       pt_loconaux = pt_locon;
       pt_locon = pt_locon->NEXT;
       dellocon(MVL_LOFPNT, pt_loconaux->NAME);
       }
     else
       {
       /* pt_locon->SIG = NULL; */
       pt_locon = pt_locon->NEXT;
       }
     }
   pt_losig = MVL_LOFPNT->LOSIG;
   while (pt_losig != NULL)  
     {
     if (pt_losig->TYPE == 'E') 
       {
       pt_losig = pt_losig->NEXT;
       continue;
       }
     pt_losigaux = pt_losig;
     pt_losig = pt_losig->NEXT;
     dellosig(MVL_LOFPNT, pt_losigaux->INDEX);
     }
   pt_loins = MVL_LOFPNT->LOINS;
   while (pt_loins != NULL)
     {
     pt_loinsaux = pt_loins;
     pt_loins = pt_loins->NEXT;
     delloins(MVL_LOFPNT, pt_loinsaux->INSNAME);
     }
   }
 FAST_MODE = local_mbk_fast_mode;
 }
