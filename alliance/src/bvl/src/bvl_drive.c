
/* ###--------------------------------------------------------------### */
/* file		: bvl_drive.c						*/
/* date		: Feb 28 2000						*/
/* version	: v114							*/
/* author	: VUONG H.N., Pirouz BAZARGAN SABET			*/
/* description	: This file contains VHDL drivers :			*/
/*		  vhdlsavebefig						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "bhl.h"
#include "bvl.h"

/* ###--------------------------------------------------------------### */
/* function	: vhdlsavebefig						*/
/* description	: print out a text file containing a data-flow VHDL	*/
/*		  description						*/
/* called func.	: beh_vhdlname, mbkalloc, beh_message, beh_error  ,	*/
/*		  beh_toolbug , getptype, reverse    , bvl_abl2str	*/
/*		  bvl_getvers						*/
/* ###--------------------------------------------------------------### */

int vhdlsavebefig (pt_fig, trace_mode)

struct befig *pt_fig    ;
int           trace_mode;

  {
  struct begen *pt_gen    = NULL;	/* current ptype pnt (generic)	*/
  struct bereg *pt_reg    = NULL;	/* current BEREG pointer	*/
  struct bemsg *pt_msg    = NULL;	/* current BEMSG pointer	*/
  struct beout *pt_out    = NULL;	/* current BEOUT pointer	*/
  struct bebus *pt_bus    = NULL;	/* current BEBUS pointer	*/
  struct beaux *pt_aux    = NULL;	/* current BEAUX pointer	*/
  struct bebux *pt_bux    = NULL;	/* current BEBUX pointer	*/
  struct bepor *pt_por    = NULL;	/* correctly ordered port list	*/
  struct biabl *ptbiabl   = NULL;	/* current BIABL pointer   	*/
  char         *suffix    ;
  char         *str       ;
  char         *mode      ;
  char         *type      ;
  char         *kind      ;
  char         *direct    ;
  char         *transp    ;
  int           nrlabel   =   0;
  int           buff_size = 128;
  char         *buffer    ;
  FILE         *fp        ;
  time_t        clock     ;
  int           left      ;
  int           right     ;
  char          name [256];
  char         *time_unit ;
  unsigned int  time_step ;
  int           err_flg   =   0;

  if ((pt_fig == NULL) || (pt_fig->ERRFLG != 0))
    beh_toolbug (10, "vhdlsavebefig", NULL, 0);

  time_step  = pt_fig->TIME_STEP;

  buffer     = mbkalloc (buff_size);
  buffer [0] = '\0';

	/* ###------------------------------------------------------### */
	/*    open result file						*/
	/* ###------------------------------------------------------### */

  if ((str = mbkgetenv ("VH_BEHSFX")) != NULL)
    suffix = strtok (str, ":");
  else
    suffix = "vbe";

  if ((fp = mbkfopen (pt_fig->NAME, suffix, WRITE_TEXT)) == NULL)
    err_flg = beh_error (107, NULL);
  else
    {
    if (trace_mode == 1)
      beh_message (13, pt_fig->NAME);

    time (&clock);
    fprintf (fp, "-- VHDL description generated from `%s`\n", pt_fig->NAME);
    fprintf (fp, "--\t\tdate     : %s"    , ctime (&clock));
    fprintf (fp, "--\t\trevision : %s\n\n", bvl_getvers ());

	/* ###------------------------------------------------------### */
	/*    entity declaration					*/
	/* ###------------------------------------------------------### */

    fprintf (fp, "-- Entity Declaration\n\n");
    fprintf (fp, "ENTITY %s IS\n", beh_vhdlname (pt_fig->NAME));

	/* ###------------------------------------------------------### */
	/*    generic declaration					*/
	/* ###------------------------------------------------------### */

    if ((pt_gen = pt_fig->BEGEN) != NULL)
      {
      pt_gen = (struct begen *) reverse ((struct chain *) pt_gen);

      fprintf (fp, "  GENERIC (\n");
      while (pt_gen != NULL)
        {
        fprintf (fp, "    CONSTANT %s : NATURAL := %ld",
                 beh_vhdlname (pt_gen->NAME), *((long *)pt_gen->VALUE));

        if (pt_gen->NEXT != NULL)
          fprintf (fp, ";\t-- %s\n", pt_gen->NAME);
        else
          fprintf (fp, "\t-- %s\n" , pt_gen->NAME);

        pt_gen = pt_gen->NEXT;
        }

      fprintf (fp, "  );\n");
      }

	/* ###------------------------------------------------------### */
	/*    port declaration						*/
	/* ###------------------------------------------------------### */

    pt_por = pt_fig->BEPOR;
    if (pt_por != NULL)
      {
      fprintf (fp, "  PORT (\n");
      pt_fig->BEPOR = (struct bepor *) reverse ((struct chain *) pt_fig->BEPOR);

      pt_por = pt_fig->BEPOR;
      while (pt_por != NULL)
        {
        switch (pt_por->DIRECTION)
          {
          case 'I':
            mode    = "IN"   ; break;
          case 'O':
          case 'Z':
            mode    = "OUT"  ; break;
          case 'B':
          case 'T':
            mode    = "INOUT"; break;
          default :
            err_flg = beh_error (69, pt_por->NAME);
            mode    = "IN"   ;
          }

        pt_por = bvl_vpor (pt_por, &left, &right, name);

        if (left != -1)
          {
          switch (pt_por->TYPE)
            {
            case 'B':
              type    = "BIT_VECTOR"; 
              kind    = "";
              break;
            case 'W':
              type    = "WOR_VECTOR"; 
              kind    = "BUS";
              break;
            case 'M':
              type    = "MUX_VECTOR"; 
              kind    = "BUS";
              break;
            default :
              err_flg = beh_error (68, pt_por->NAME);
              type    = "BIT_VECTOR"; 
              kind    = "";
            }

          if (left >= right)
            direct = "DOWNTO";
          else
            direct = "TO"    ;

          fprintf (fp, "  SIGNAL %s : %s %s (%d %s %d) %s", beh_vhdlname (name),
                       mode, type, left, direct, right, kind);
          }
        else
          {
          switch (pt_por->TYPE)
            {
            case 'B':
              type    = "BIT"        ; break;
            case 'W':
              type    = "WOR_BIT BUS"; break;
            case 'M':
              type    = "MUX_BIT BUS"; break;
            default :
              err_flg = beh_error (68, pt_por->NAME);
              type    = "BIT"        ;
            }

          fprintf (fp, "  SIGNAL %s : %s %s", beh_vhdlname (name), mode, type);
          }

        if (pt_por->NEXT != NULL)
          fprintf (fp, ";\t-- %s\n"     , name);
        else
          fprintf (fp, "\t-- %s\n  );\n", name);

        pt_por = pt_por->NEXT;
        }

      pt_fig->BEPOR = (struct bepor *) reverse ((struct chain *) pt_fig->BEPOR);
      }

    fprintf (fp, "END %s;\n\n\n", beh_vhdlname (pt_fig->NAME));

	/* ###------------------------------------------------------### */
	/*    architecture declaration					*/
	/* ###------------------------------------------------------### */

    fprintf (fp, "-- Architecture Declaration\n\n");
    fprintf (fp, "ARCHITECTURE behavioural OF %s IS\n",
                 beh_vhdlname (pt_fig->NAME));

	/* ###------------------------------------------------------### */
          /*  registers' decalration					*/
	/* ###------------------------------------------------------### */

    pt_fig->BEREG = (struct bereg *) reverse ((struct chain *) pt_fig->BEREG);

    pt_reg = pt_fig->BEREG;
    while (pt_reg != NULL)
      {
      pt_reg = bvl_vreg (pt_reg, &left, &right, name);

      if (left != -1)
        {
        if (left >= right)
          direct = "DOWNTO";
        else
          direct = "TO"    ;

        fprintf (fp, "  SIGNAL %s : REG_VECTOR (%d %s %d) REGISTER;\t-- %s\n",
                     beh_vhdlname (name), left, direct, right, name);
        }
      else
        {
        fprintf (fp, "  SIGNAL %s : REG_BIT REGISTER;\t-- %s\n",
                     beh_vhdlname (name), name);
        }
      pt_reg = pt_reg->NEXT;
      }

    pt_fig->BEREG = (struct bereg *) reverse ((struct chain *) pt_fig->BEREG);

	/* ###------------------------------------------------------### */
	/*  internal buses' declaration					*/
	/* ###------------------------------------------------------### */

    pt_fig->BEBUX = (struct bebux *) reverse ((struct chain *) pt_fig->BEBUX);

    pt_bux = pt_fig->BEBUX;
    while (pt_bux != NULL)
      {
      pt_bux = bvl_vbux (pt_bux, &left, &right, name);

      if (left != -1)
        {
        switch (pt_bux->TYPE)
          {
          case 'W':
            type    = "WOR_VECTOR"; 
            break;
          case 'M':
            type    = "MUX_VECTOR"; 
            break;
          default :
            err_flg = beh_error (67, pt_bux->NAME);
            type    = "MUX_VECTOR"; 
          }

        if (left >= right)
          direct = "DOWNTO";
        else
          direct = "TO"    ;

        fprintf (fp, "  SIGNAL %s : %s (%d %s %d) BUS;\t-- %s\n",
                     beh_vhdlname (name), type, left, direct, right, name);
        }
      else
        {
        switch (pt_bux->TYPE)
          {
          case 'W':
            type    = "WOR_BIT"; 
            break;
          case 'M':
            type    = "MUX_BIT"; 
            break;
          default :
            err_flg = beh_error (67, pt_bux->NAME);
            type    = "MUX_BIT"; 
          }
        fprintf (fp, "  SIGNAL %s : %s BUS;\t\t-- %s\n", beh_vhdlname (name),
                     type, name);
        }

      pt_bux = pt_bux->NEXT;
      }

    pt_fig->BEBUX = (struct bebux *) reverse ((struct chain *) pt_fig->BEBUX);

	/* ###------------------------------------------------------### */
          /*  simple internal signals' declaration			*/
	/* ###------------------------------------------------------### */

    pt_fig->BEAUX = (struct beaux *) reverse ((struct chain *) pt_fig->BEAUX);

    pt_aux = pt_fig->BEAUX;
    while (pt_aux != NULL)
      {
      pt_aux = bvl_vaux (pt_aux, &left, &right, name);

      if (left != -1)
        {
        if (left >= right)
          direct = "DOWNTO";
        else
          direct = "TO"    ;

        fprintf (fp, "  SIGNAL %s : BIT_VECTOR (%d %s %d);\t-- %s\n",
                     beh_vhdlname (name), left, direct, right, name);
        }
      else
        {
        fprintf (fp, "  SIGNAL %s : BIT;\t\t-- %s\n", beh_vhdlname(name), name);
        }

      pt_aux = pt_aux->NEXT;
      }

    pt_fig->BEAUX = (struct beaux *)reverse ((struct chain *) pt_fig->BEAUX);

	/* ###------------------------------------------------------### */
	/*    description part						*/
	/* ###------------------------------------------------------### */

    fprintf (fp, "\nBEGIN\n");

    time_unit = beh_gettimeunit (pt_fig);

	/* ###------------------------------------------------------### */
	/*    print out a concurrent assert statement for each BEMSG	*/
	/* ###------------------------------------------------------### */

    pt_msg = pt_fig->BEMSG;
    while (pt_msg != NULL)
      {
      if (pt_msg->LABEL != NULL)
        fprintf (fp, "  %s :", pt_msg->LABEL);

      buffer = bvl_abl2str (pt_msg->ABL, buffer, &buff_size); 

      fprintf (fp, "  ASSERT (%s = '1')\n", bvl_printabl (buffer));
      buffer [0] = '\0';

      if (pt_msg->MESSAGE != NULL)
        fprintf (fp, "    REPORT %s\n", pt_msg->MESSAGE);

      if (pt_msg->LEVEL == 'W')
        fprintf (fp, "    SEVERITY WARNING;\n\n");
      else
        fprintf (fp, "    SEVERITY ERROR;\n\n"  );

      pt_msg = pt_msg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    concurrent signal assignment for simple internal signals	*/
	/* ###------------------------------------------------------### */

    pt_aux = pt_fig->BEAUX;
    while (pt_aux != NULL)
      {
      if (pt_aux->ABL == NULL)
        err_flg = beh_error (40, pt_aux->NAME);
      else
        {
        buffer = bvl_abl2str (pt_aux->ABL, buffer, &buff_size);

        if ((pt_aux->FLAG & BEH_ASG_TRANSPORT) != 0)
          transp = "TRANSPORT";
        else
          transp = ""         ;

        fprintf (fp, "  %s <= %s %s", beh_vhvector (pt_aux->NAME), transp,
                     bvl_printabl (buffer));

        if (pt_aux->TIME == 0)
          fprintf (fp, ";\n");
        else
          fprintf (fp, " after %u %s;\n", pt_aux->TIME*time_step, time_unit);

        buffer [0] = '\0';
        }
      pt_aux = pt_aux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   print out a block statement with one guarded concurrent	*/
	/* signal assignment for each driver of each register		*/
	/* ###------------------------------------------------------### */

    pt_reg = pt_fig->BEREG;
    while (pt_reg != NULL)
      {
      ptbiabl = pt_reg->BIABL;
      while (ptbiabl != NULL)
        {
        fprintf (fp, "  label%d : BLOCK ", nrlabel);

        if (ptbiabl->CNDABL == NULL)
          beh_toolbug (19, "vhdlsavebefig", pt_reg->NAME, 0);
        else
          {
          buffer = bvl_abl2str (ptbiabl->CNDABL, buffer, &buff_size);
          fprintf (fp, "(%s = '1')\n", bvl_printabl (buffer));
          buffer [0] = '\0';
          }
    
        if ((ptbiabl->FLAG & BEH_ASG_TRANSPORT) != 0)
          transp = "TRANSPORT";
        else
          transp = ""         ;

        fprintf (fp, "  BEGIN\n");
        fprintf (fp, "    %s <= ", beh_vhvector (pt_reg->NAME));

        if (ptbiabl->VALABL == NULL)
          beh_toolbug (20, "vhdlsavebefig", pt_reg->NAME, 0);
        else
          {
          buffer = bvl_abl2str (ptbiabl->VALABL, buffer, &buff_size);
          fprintf (fp, "GUARDED %s %s ", transp, bvl_printabl (buffer));

          if (ptbiabl->TIME == 0)
            fprintf (fp, ";\n");
          else
            fprintf (fp, "after %u %s;\n", ptbiabl->TIME*time_step, time_unit);

          buffer[0] = '\0';
          }

        fprintf (fp, "  END BLOCK label%d;\n", nrlabel);
        ptbiabl = ptbiabl->NEXT;
        nrlabel++;
        }
    
      pt_reg = pt_reg->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    Print out a block statement with one guarded concurrent	*/
	/* signal assignment for each driver of each internal bus	*/
	/* ###------------------------------------------------------### */

    pt_bux = pt_fig->BEBUX;
    while (pt_bux != NULL)
      {
      ptbiabl = pt_bux->BIABL;
      while (ptbiabl != NULL)
        {
        fprintf (fp, "  label%d : BLOCK ", nrlabel);

        if (ptbiabl->CNDABL == NULL)
          beh_toolbug (19, "vhdlsavebefig", pt_bux->NAME, 0);
        else
          {
          buffer = bvl_abl2str (ptbiabl->CNDABL, buffer, &buff_size);
          fprintf (fp, "(%s = '1')\n", bvl_printabl (buffer));
          buffer[0] = '\0';
          }
    
        if ((ptbiabl->FLAG & BEH_ASG_TRANSPORT) != 0)
          transp = "TRANSPORT";
        else
          transp = ""         ;

        fprintf (fp, "  BEGIN\n");
        fprintf (fp, "    %s <= ", beh_vhvector (pt_bux->NAME));

        if (ptbiabl->VALABL == NULL)
          beh_toolbug (20, "vhdlsavebefig",pt_bux->NAME,0);
        else
          {
          buffer = bvl_abl2str (ptbiabl->VALABL, buffer, &buff_size);
          fprintf (fp, "GUARDED %s %s ", transp, bvl_printabl (buffer));

          if (ptbiabl->TIME == 0)
            fprintf (fp, ";\n");
          else
            fprintf (fp, "after %u %s;\n", ptbiabl->TIME*time_step, time_unit);

          buffer[0] = '\0';
          }

        fprintf (fp, "  END BLOCK label%d;\n",nrlabel);
        ptbiabl = ptbiabl->NEXT;
        nrlabel++;
        }
    
      pt_bux = pt_bux->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*    Print out a block statement with one guarded concurrent	*/
	/* signal assignment for each driver of each bussed output	*/
	/* signal							*/
	/* ###------------------------------------------------------### */

    pt_bus = pt_fig->BEBUS;
    while (pt_bus != NULL)
      {
      ptbiabl = pt_bus->BIABL;
      while (ptbiabl != NULL)
        {
        fprintf (fp, "  label%d : BLOCK ", nrlabel);

        if (ptbiabl->CNDABL == NULL)
          beh_toolbug (19, "vhdlsavebefig", pt_bus->NAME, 0);
        else
          {
          buffer = bvl_abl2str (ptbiabl->CNDABL, buffer, &buff_size);
          fprintf (fp, "(%s = '1')\n", bvl_printabl (buffer));
          buffer [0] = '\0';
          }
    
        if ((ptbiabl->FLAG & BEH_ASG_TRANSPORT) != 0)
          transp = "TRANSPORT";
        else
          transp = ""         ;

        fprintf (fp, "  BEGIN\n");
        fprintf (fp, "    %s <= ", beh_vhvector (pt_bus->NAME));
        if (ptbiabl->VALABL == NULL)
          beh_toolbug (20, "vhdlsavebefig", pt_bus->NAME, 0);
        else
          {
          buffer = bvl_abl2str (ptbiabl->VALABL, buffer, &buff_size);
          fprintf (fp, "GUARDED %s %s ", transp, bvl_printabl (buffer));

          if (ptbiabl->TIME == 0)
            fprintf (fp, ";\n");
          else
            fprintf (fp, "after %u %s;\n", ptbiabl->TIME*time_step, time_unit);
          buffer [0] = '\0';
          }

        fprintf (fp, "\tEND BLOCK label%d;\n", nrlabel);
        ptbiabl = ptbiabl->NEXT;
        nrlabel++;
        }
    
      pt_bus = pt_bus->NEXT;
      }

	/* ###------------------------------------------------------### */
	/*   print a concurrent signal assignment for each ouput port	*/
	/* ###------------------------------------------------------### */

    pt_out = pt_fig->BEOUT;
    while (pt_out != NULL)
      {
      if (pt_out->ABL == NULL)
        err_flg = beh_error (40, pt_out->NAME);
      else
        {
        buffer = bvl_abl2str (pt_out->ABL, buffer, &buff_size);

        if ((pt_out->FLAG & BEH_ASG_TRANSPORT) != 0)
          transp = "TRANSPORT";
        else
          transp = ""         ;

        fprintf (fp, "  %s <= %s %s", beh_vhvector (pt_out->NAME), transp,
                     bvl_printabl (buffer));

        if (pt_out->TIME == 0)
          fprintf (fp, ";\n");
        else
          fprintf (fp, " after %u %s;\n", pt_out->TIME*time_step, time_unit);

        buffer [0] = '\0';
        }

      pt_out = pt_out->NEXT;
      }
    
    fprintf (fp, "END;\n");
    fclose  (fp);
    }

  return (err_flg);
  }
