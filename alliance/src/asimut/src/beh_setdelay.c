
/* ###--------------------------------------------------------------### */
/* file		: beh_setdelay.c					*/
/* date		: Aug 20 1997						*/
/* version	: v3.0							*/
/* author	: Pirouz BAZARAGAN SABET				*/
/* description	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"
#include "beh_delay.h"

char *beh_gettimeunit(time_unit)

unsigned char time_unit;
  {
  switch (time_unit)
    {
    case BEH_TU__FS:
      return(namealloc("fs"));
      break;
    case BEH_TU__PS:
      return(namealloc("ps"));
      break;
    case BEH_TU__NS:
      return(namealloc("ns"));
      break;
    case BEH_TU__US:
      return(namealloc("us"));
      break;
    case BEH_TU__MS:
      return(namealloc("ms"));
      break;
    default:
      return(namealloc("invalid time unit"));
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_chgatom						*/
/* description	: change some terminal operands of an expression by	*/
/*		  some other variables. The name of the variable that	*/
/*		  replace a given signal is registred in a dictionary.	*/
/*		  Only registered names are changed.			*/
/*									*/
/* warning	: this function modifies the expression passed as	*/
/*		  argument						*/
/* ###--------------------------------------------------------------### */

void beh_chgatom (pt_abl, pt_dic)

struct chain  *pt_abl;
struct beden **pt_dic;

  {
  char         *new_name;
  struct chain *pt_opr  ;

  if ((pt_abl != NULL) && (pt_dic != NULL))
    {
    if (pt_abl->NEXT == NULL)
      {
	/* ###------------------------------------------------------### */
	/*    when the expression is a terminal, check if the operand	*/
	/* is in the dictionary and extract the name of the variable	*/
	/* that replaces that operand.					*/
	/* ###------------------------------------------------------### */

      new_name = (char *) beh_chktab (pt_dic, pt_abl->DATA, NULL, BEH_DELAY_FIELD);
      if (new_name != NULL)
        pt_abl->DATA = new_name;
      }
    else
      {
      pt_opr = pt_abl->NEXT;
      while (pt_opr != NULL)
        {
        beh_chgatom (pt_opr->DATA, pt_dic);
        pt_opr = pt_opr->NEXT;
        }
      }
    }
  }


/* ###--------------------------------------------------------------### */
/* function	: beh_chkregdly						*/
/* description	: check registers delays consistency and add delayed	*/
/*		  buffers after register if non null delay.		*/
/*		  This is done to support the specification of a	*/
/*		  delay for such elements. (the driver of which can be	*/
/*		  instantaneously disconnected).			*/
/*		  3 different mode may be used :			*/
/*		    - mode 0 : check that all register delays are null	*/
/*		    - mode 1 : check and reset each non-null reg delay	*/
/*		    - mode 2 : check and add a delayed buffer for each	*/
/*                             non-null reg delay (the non-null reg	*/
/*                             delays are also reseted			*/
/* ###--------------------------------------------------------------### */

void beh_chkregdly (pt_fig, mode)

struct befig   *pt_fig    ;
unsigned char   mode;

  {
  struct beout  *pt_out        ;
  struct beaux  *pt_aux        ;
  struct bemsg  *pt_msg        ;
  struct bebus  *pt_bus        ;
  struct bereg  *pt_reg        ;
  struct bebux  *pt_bux        ;
  struct berin  *pt_rin        ;
  struct biabl  *pt_biabl      ;
  struct binode *pt_binode     ;
  struct chain  *pt_abl        ;
  struct beden **name_dic      ;
  char           new_name [256];
  char          *name          ;
  char           error_flag    ;
  unsigned int   time  = 0     ;

	/* ###------------------------------------------------------### */
	/*    create a dictionary that will associate to each element	*/
	/* (register) the name of its output buffer			*/
	/* ###------------------------------------------------------### */

  name_dic = beh_initab ();

	/* ###------------------------------------------------------### */
	/*    if buffers are to be added after registers, for each	*/
	/* register, create a name for its output buffer (the signal	*/
	/* will be added to the description later)			*/
	/* ###------------------------------------------------------### */

  pt_reg = pt_fig->BEREG;
  while (pt_reg != NULL)
    {

	/* ###------------------------------------------------------### */
	/*    Chek the consistency of different driver delays.		*/
	/* (if there is different delays then the buffer delay is null)	*/
	/* ###------------------------------------------------------### */

    pt_biabl = pt_reg->BIABL;
    time = pt_biabl->TIME;
    pt_biabl = pt_biabl->NEXT;
    error_flag = 0;

    while ((pt_biabl != NULL) && (error_flag == 0))
      {
      if (pt_biabl->TIME != time)
        {
        error_flag = 1;
        }
      pt_biabl        = pt_biabl->NEXT;
      }

    pt_binode = pt_reg->BINODE;
    while ((pt_binode != NULL) && (error_flag == 0))
      {
      if (pt_binode->TIME != time)
        {
        error_flag = 1;
        }
      pt_binode       = pt_binode->NEXT;
      }

    if (error_flag != 0)
      {
      printf("WARNING : Drivers of register %s have different delays\n", pt_reg->NAME);
      time = 0;
      }

    if (time != 0)
      {
      sprintf (new_name , "delay_%s", pt_reg->NAME);
      name = namealloc (new_name);
      beh_addtab (name_dic, pt_reg->NAME, NULL, BEH_DELAY_FIELD, name);
      beh_addtab (name_dic, name, NULL, BEH_DELAY_FIELD, time);
      }

    pt_reg = pt_reg->NEXT;
    }

	/* ###------------------------------------------------------### */
	/*    if the mode == 2 (check and create buffers), in all the	*/
	/* expressions, if needed replace the name of elements by	*/
	/* their output buffers						*/
	/* ###------------------------------------------------------### */

  if (mode == 2)
    {
    pt_out = pt_fig->BEOUT;
    while (pt_out != NULL)
      {
      beh_chgatom (pt_out->ABL, name_dic);
      pt_out = pt_out->NEXT;
      }

    pt_aux = pt_fig->BEAUX;
    while (pt_aux != NULL)
      {
      beh_chgatom (pt_aux->ABL, name_dic);
      pt_aux = pt_aux->NEXT;
      }

    pt_aux = pt_fig->BEDLY;
    while (pt_aux != NULL)
      {
      beh_chgatom (pt_aux->ABL, name_dic);
      pt_aux = pt_aux->NEXT;
      }

    pt_msg = pt_fig->BEMSG;
    while (pt_msg != NULL)
      {
      beh_chgatom (pt_msg->ABL, name_dic);
      pt_msg = pt_msg->NEXT;
      }

    pt_bus = pt_fig->BEBUS;
    while (pt_bus != NULL)
      {
      pt_biabl = pt_bus->BIABL;
      while (pt_biabl != NULL)
        {
        beh_chgatom (pt_biabl->CNDABL, name_dic);
        beh_chgatom (pt_biabl->VALABL, name_dic);
        pt_biabl = pt_biabl->NEXT;
        }
      pt_bus = pt_bus->NEXT;
      }

    pt_bux = pt_fig->BEBUX;
    while (pt_bux != NULL)
      {
      pt_biabl = pt_bux->BIABL;
      while (pt_biabl != NULL)
        {
        beh_chgatom (pt_biabl->CNDABL, name_dic);
        beh_chgatom (pt_biabl->VALABL, name_dic);
        pt_biabl = pt_biabl->NEXT;
        }
      pt_bux = pt_bux->NEXT;
      }

    pt_reg = pt_fig->BEREG;
    while (pt_reg != NULL)
      {
      pt_biabl = pt_reg->BIABL;
      while (pt_biabl != NULL)
        {
        beh_chgatom (pt_biabl->CNDABL, name_dic);
        beh_chgatom (pt_biabl->VALABL, name_dic);
        pt_biabl = pt_biabl->NEXT;
        }
      pt_reg = pt_reg->NEXT;
      }
    }

  pt_aux = pt_fig->BEAUX;
  pt_rin = pt_fig->BERIN;
  pt_reg = pt_fig->BEREG;

  while (pt_reg != NULL)
    {
    name = (char *) beh_chktab (name_dic, pt_reg->NAME, NULL, BEH_DELAY_FIELD);
    if (name != NULL)
      {
      time = (unsigned int) beh_chktab   (name_dic, name, NULL, BEH_DELAY_FIELD);

	/* ###------------------------------------------------------### */
	/*    if mode == 2 (check and create buffer), add a buffer	*/
	/* after each register if needed (if this register has been	*/
	/* previously registered as a non-null-delay register).		*/
	/* The created buffer is a simple internal signal.		*/
	/* The buffer delay is the delay of the register, and the	*/
	/* register delay becomes null.					*/
	/* ###------------------------------------------------------### */

      if (mode == 2)
        {
        pt_abl       =          addchain     (NULL    , pt_reg->NAME         );
        pt_rin       =          beh_addberin (pt_rin  , name                 );
        pt_aux       =          beh_addbeaux (pt_aux  , name, pt_abl, NULL   );
        pt_aux->TIME = time;

        pt_biabl = pt_reg->BIABL;
        while (pt_biabl != NULL)
          {
          pt_biabl->TIME = 0;
          pt_biabl       = pt_biabl->NEXT;
          }

        pt_binode = pt_reg->BINODE;
        while (pt_binode != NULL)
          {
          pt_binode->TIME = 0;
          pt_binode       = pt_binode->NEXT;
          }

        printf("WARNING : Buffer %s with delay %d %s is created after register %s.\n",
                          name, time, beh_gettimeunit(pt_fig->TIME_UNIT), pt_reg->NAME);
        }

	/* ###------------------------------------------------------### */
	/*    if mode == 1 (check and set null delays), force null	*/
	/* delays if needed (for each register which has been		*/
	/* previously registered as a non-null-delay register).		*/
	/* ###------------------------------------------------------### */

      if (mode == 1)
        {
        pt_biabl = pt_reg->BIABL;
        while (pt_biabl != NULL)
          {
          pt_biabl->TIME = 0;
          pt_biabl       = pt_biabl->NEXT;
          }

        pt_binode = pt_reg->BINODE;
        while (pt_binode != NULL)
          {
          pt_binode->TIME = 0;
          pt_binode       = pt_binode->NEXT;
          }

        printf("WARNING : Register %s with delay %d %s becomes a null-delay Register.\n",
                          pt_reg->NAME, time, beh_gettimeunit(pt_fig->TIME_UNIT));
        }

	/* ###------------------------------------------------------### */
	/*    if mode == 0 (check only), only print a WARNING for each	*/
	/* register which has been previously registered as a non-null	*/
	/* delay register.						*/
	/* ###------------------------------------------------------### */

      if (mode == 0)
        {
        printf("WARNING : Register %s with delay %d %s becomes a null-delay Register.\n",
                          pt_reg->NAME, time, beh_gettimeunit(pt_fig->TIME_UNIT));
        }
      }
    pt_reg = pt_reg->NEXT;
    }

  pt_fig->BERIN = pt_rin;
  pt_fig->BEAUX = pt_aux;

  beh_fretab (name_dic);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_backdelay						*/
/* ###--------------------------------------------------------------### */

char beh_backdelay ( delay_dict, delay_filename, delay_mode )

struct beden **delay_dict;
char          *delay_filename;
unsigned char  delay_mode;

  {
  FILE	*delay_file = NULL;
  char	 line[256];
  char   signame[256];
  char	 vector_radical[256];
  int	 index;
  int	 typ_delay;
  int	 min_delay;
  int	 max_delay;
  int	 nbr_delay;
  char                 *tok        ;
  char                 *str        ;
  struct chain         *dlysfx_lst = NULL;
  struct chain         *suffix     = NULL;

        /* ###------------------------------------------------------### */
        /*    read the environment variable VH_DLYSFX to create a list  */
        /* of suffix for delay files.                                   */
        /* ###------------------------------------------------------### */

  if ((str = mbkgetenv ("VH_DLYSFX")) != NULL)
    {
    tok = strtok (str, ":");
    while (tok != NULL)
      {
      dlysfx_lst = addchain (dlysfx_lst, tok);
      tok        = strtok (NULL, ":");
      }
    dlysfx_lst = (struct chain *) reverse (dlysfx_lst);
    }
  else
    dlysfx_lst = addchain (NULL, "dly");

        /* ###------------------------------------------------------### */
        /*    searching the root file                                   */
        /* ###------------------------------------------------------### */

  suffix = dlysfx_lst;
  while (suffix != NULL)
    {
    if ((delay_file = mbkfopen (delay_filename, suffix->DATA, READ_TEXT)) != NULL)
      {
      break;
      }
    suffix = suffix->NEXT;
    }

  if ( delay_file == NULL )
    {
    suffix = dlysfx_lst;
    while (suffix != NULL)
      {
      printf("ERROR : delay file %s.%s does not exist\n",delay_filename,(char *)(suffix->DATA));
      suffix = suffix->NEXT;
      }
    return(1);
    }
 
  while ( fgets(line, 256, delay_file) != NULL)
    {
    if ( sscanf( line, "%s : %d : %d : %d : %d", signame,
                      &typ_delay, &min_delay, &max_delay, &nbr_delay ) == 5 )
      {
      if ((delay_mode & BEH_DELAY_MAX) == BEH_DELAY_MAX)
        beh_addtab (delay_dict, namealloc(signame), NULL, BEH_DELAY_FIELD, max_delay);
      else if ((delay_mode & BEH_DELAY_MIN) == BEH_DELAY_MIN)
        beh_addtab (delay_dict, namealloc(signame), NULL, BEH_DELAY_FIELD, min_delay);
      else if ((delay_mode & BEH_DELAY_TYP) == BEH_DELAY_TYP)
        beh_addtab (delay_dict, namealloc(signame), NULL, BEH_DELAY_FIELD, typ_delay);
      }
    else if ( sscanf( line, "%s %d : %d : %d : %d : %d", vector_radical, &index,
                      &typ_delay, &min_delay, &max_delay, &nbr_delay ) == 6 )
      {
      sprintf(signame, "%s %d", vector_radical, index);
      if ((delay_mode & BEH_DELAY_MAX) == BEH_DELAY_MAX)
        beh_addtab (delay_dict, namealloc(signame), NULL, BEH_DELAY_FIELD, max_delay);
      else if ((delay_mode & BEH_DELAY_MIN) == BEH_DELAY_MIN)
        beh_addtab (delay_dict, namealloc(signame), NULL, BEH_DELAY_FIELD, min_delay);
      else if ((delay_mode & BEH_DELAY_TYP) == BEH_DELAY_TYP)
        beh_addtab (delay_dict, namealloc(signame), NULL, BEH_DELAY_FIELD, typ_delay);
      }
    else
      {
      sscanf( line, "%s", signame );
      printf("WARNING : invalid line in file %s.%s : \n",delay_filename,(char *)(suffix->DATA));
      printf("        : %s\n", line);
      }
    }
  fclose(delay_file);
  return(0);
  }

/* ###--------------------------------------------------------------### */
/* function	: beh_setdelay						*/
/* ###--------------------------------------------------------------### */

void beh_setdelay (pt_befig, max_delay, delay_mode, delay_dict)

struct befig  *pt_befig;
unsigned int   max_delay;
unsigned char  delay_mode;
struct beden **delay_dict;

  {
  struct beout         *pt_beout ;
  struct beaux         *pt_beaux ;
  struct bebus         *pt_bebus ;
  struct bebux         *pt_bebux ;
  struct bereg         *pt_bereg ;
  struct biabl         *pt_biabl ;
  struct binode        *pt_binode;
  static unsigned int   time     = 0;
 
  if ( delay_mode == BEH_DELAY_RANDOM ) 
    {
    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      time           += 5;
      pt_beout->TIME  = time % max_delay + 3;

      pt_beout = pt_beout->NEXT;
      }

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      time           += 5;
      pt_beaux->TIME  = time % max_delay + 3;

      pt_beaux = pt_beaux->NEXT;
      }

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      time           += 5;
      pt_biabl = pt_bereg->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = time % max_delay + 3;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bereg->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME = time % max_delay + 3;
        pt_binode       = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      time           += 5;
      pt_biabl = pt_bebus->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = time % max_delay + 3;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebus->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME  = time % max_delay + 3;
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      time           += 5;
      pt_biabl = pt_bebux->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = time % max_delay + 3;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebux->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME  = time % max_delay + 3;
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }
    }
 
  if ( delay_mode == BEH_DELAY_FIXED ) 
    {
    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      pt_beout->TIME  = max_delay ;

      pt_beout = pt_beout->NEXT;
      }

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      pt_beaux->TIME  = max_delay ;

      pt_beaux = pt_beaux->NEXT;
      }

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      pt_biabl = pt_bereg->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = max_delay;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bereg->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME = max_delay;
        pt_binode       = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      pt_biabl = pt_bebus->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = max_delay;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebus->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME  = max_delay;
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_biabl = pt_bebux->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = max_delay;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebux->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME  = max_delay;
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }
    }
 
  if (((delay_mode & BEH_DELAY_BACKANOTATED) == BEH_DELAY_BACKANOTATED) &&
      ((delay_mode & BEH_DELAY_DEFAULT)      != BEH_DELAY_DEFAULT))
    {
    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      pt_beout->TIME = beh_chktab (delay_dict, pt_beout->NAME,
                                   NULL, BEH_DELAY_FIELD) ;

      pt_beout = pt_beout->NEXT;
      }

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      pt_beaux->TIME  = beh_chktab (delay_dict, pt_beaux->NAME,
                                    NULL, BEH_DELAY_FIELD) ;

      pt_beaux = pt_beaux->NEXT;
      }

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      pt_biabl = pt_bereg->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = beh_chktab (delay_dict, pt_bereg->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bereg->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME = beh_chktab (delay_dict, pt_bereg->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        pt_binode       = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      pt_biabl = pt_bebus->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = beh_chktab (delay_dict, pt_bebus->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebus->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME = beh_chktab (delay_dict, pt_bebus->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        pt_binode       = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_biabl = pt_bebux->BIABL;
      while (pt_biabl != NULL)
        {
        pt_biabl->TIME  = beh_chktab (delay_dict, pt_bebux->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebux->BINODE;
      while (pt_binode != NULL)
        {
        pt_binode->TIME = beh_chktab (delay_dict, pt_bebux->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        pt_binode       = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }
    }
 
  if ( delay_mode == (BEH_DELAY_RANDOM | BEH_DELAY_DEFAULT)) 
    {
    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      if (pt_beout->TIME == 0)
        {
        time           += 5;
        pt_beout->TIME  = time % max_delay + 3;
        }
      pt_beout = pt_beout->NEXT;
      }

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      if (pt_beaux->TIME == 0)
        {
        time           += 5;
        pt_beaux->TIME  = time % max_delay + 3;
        }
      pt_beaux = pt_beaux->NEXT;
      }

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      pt_biabl = pt_bereg->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          time           += 5;
          pt_biabl->TIME  = time % max_delay;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bereg->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          time           += 5;
          pt_binode->TIME  = time % max_delay;
          }
        pt_binode        = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      pt_biabl = pt_bebus->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          time           += 5;
          pt_biabl->TIME  = time % max_delay;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebus->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          time           += 5;
          pt_binode->TIME  = time % max_delay;
          }
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_biabl = pt_bebux->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          time           += 5;
          pt_biabl->TIME  = time % max_delay;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebux->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          time           += 5;
          pt_binode->TIME  = time % max_delay;
          }
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }
    }
 
  if ( delay_mode == (BEH_DELAY_FIXED | BEH_DELAY_DEFAULT)) 
    {
    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      if ( pt_beout->TIME  == 0 )
        {
        pt_beout->TIME  = max_delay ;
        }
      pt_beout = pt_beout->NEXT;
      }

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      if ( pt_beaux->TIME  == 0 )
        {
        pt_beaux->TIME  = max_delay ;
        }
      pt_beaux = pt_beaux->NEXT;
      }

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      pt_biabl = pt_bereg->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          pt_biabl->TIME  = max_delay;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bereg->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          pt_binode->TIME  = max_delay;
          }
        pt_binode        = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      pt_biabl = pt_bebus->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          pt_biabl->TIME  = max_delay;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebus->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          pt_binode->TIME  = max_delay;
          }
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_biabl = pt_bebux->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          pt_biabl->TIME  = max_delay;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebux->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          pt_binode->TIME  = max_delay;
          }
        pt_binode        = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }
    }
 
  if (((delay_mode & BEH_DELAY_BACKANOTATED) == BEH_DELAY_BACKANOTATED) &&
      ((delay_mode & BEH_DELAY_DEFAULT)      == BEH_DELAY_DEFAULT))
    {
    pt_beout = pt_befig->BEOUT;
    while (pt_beout != NULL)
      {
      if (pt_beout->TIME == 0)
        {
        pt_beout->TIME = beh_chktab (delay_dict, pt_beout->NAME,
                                     NULL, BEH_DELAY_FIELD) ;
        }
      pt_beout = pt_beout->NEXT;
      }

    pt_beaux = pt_befig->BEAUX;
    while (pt_beaux != NULL)
      {
      if (pt_beaux->TIME == 0)
        {
        pt_beaux->TIME  = beh_chktab (delay_dict, pt_beaux->NAME,
                                      NULL, BEH_DELAY_FIELD) ;
        }
      pt_beaux = pt_beaux->NEXT;
      }

    pt_bereg = pt_befig->BEREG;
    while (pt_bereg != NULL)
      {
      pt_biabl = pt_bereg->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          pt_biabl->TIME  = beh_chktab (delay_dict, pt_bereg->NAME,
                                        NULL, BEH_DELAY_FIELD) ;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bereg->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          pt_binode->TIME  = beh_chktab (delay_dict, pt_bereg->NAME,
                                         NULL, BEH_DELAY_FIELD) ;
          }
        pt_binode       = pt_binode->NEXT;
        }
      pt_bereg = pt_bereg->NEXT;
      }

    pt_bebus = pt_befig->BEBUS;
    while (pt_bebus != NULL)
      {
      pt_biabl = pt_bebus->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          pt_biabl->TIME  = beh_chktab (delay_dict, pt_bebus->NAME,
                                        NULL, BEH_DELAY_FIELD) ;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebus->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          pt_binode->TIME  = beh_chktab (delay_dict, pt_bebus->NAME,
                                         NULL, BEH_DELAY_FIELD) ;
          }
        pt_binode       = pt_binode->NEXT;
        }
      pt_bebus = pt_bebus->NEXT;
      }

    pt_bebux = pt_befig->BEBUX;
    while (pt_bebux != NULL)
      {
      pt_biabl = pt_bebux->BIABL;
      while (pt_biabl != NULL)
        {
        if (pt_biabl->TIME == 0)
          {
          pt_biabl->TIME  = beh_chktab (delay_dict, pt_bebux->NAME,
                                        NULL, BEH_DELAY_FIELD) ;
          }
        pt_biabl        = pt_biabl->NEXT;
        }
      pt_binode = pt_bebux->BINODE;
      while (pt_binode != NULL)
        {
        if (pt_binode->TIME == 0)
          {
          pt_binode->TIME  = beh_chktab (delay_dict, pt_bebux->NAME,
                                         NULL, BEH_DELAY_FIELD) ;
          }
        pt_binode       = pt_binode->NEXT;
        }
      pt_bebux = pt_bebux->NEXT;
      }
    }
  }
