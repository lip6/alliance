/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: pat_debug.c						*/
/* date		: Mar 13 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "pat.h"
#include "pat_debug.h"

/* ###--------------------------------------------------------------### */
/* function	: pat_debug						*/
/* description	: display any pat structure				*/
/* called func.	: pat_error, pat_message				*/
/*		  go_forward, translate, splitline, pop, push		*/
/*		  read_field, get_size					*/
/* ###--------------------------------------------------------------### */

void pat_debug (head_pnt, type)

void *head_pnt;				/* structure's pointer		*/
char *type;				/* structure's type		*/

  {
  static void          disp_immd  ();
  static void          get_size   ();
  static void          read_field ();
  static void          push       ();
  static void          pop        ();
  static struct chain *go_forward ();
  static int           translate  ();
  static int           splitline  ();

  char          line   [128];		/* buffer to read a cmd line	*/
  char          buffer [128];		/* buffer to split the cmd line	*/

  char         *words  [ 10];		/* number of words on a line	*/
  int           nmbrs  [ 10];		/* words translated into number	*/
  char          flags  [ 10];		/* set if words is a number	*/
  int           indxs  [ 10];		/* index of words		*/

  struct stack  jtab   [ 10];		/* list of memorized addresses	*/
  int           idx;
  int           readflg = 0;
  unsigned int  size;
  char         *pntr   = NULL;
  long          pshtype;
  int           wrdcnt = 1;

  struct stack  stk [STKSIZ_DFN];
  int           stkpnt = -1;

  union value   pnt [MAXCMD_DFN];
  long          typ [MAXCMD_DFN];
  unsigned int  siz [MAXCMD_DFN];

  static char  *str [] = {
                          "_back"    , "_exit"    , "_jump"    , "_save"    ,
                          "_stop"    , "_top"     , "_up"      , "_display" ,

                          "character", "short"    , "integer"  , "long"     ,
                          "void"     , "string"   ,

                          "chain"    , "ptype"    ,

                          "paseq"    , "pagrp"    , "paiol"    , "papat"    ,
                          "pacom"    , "paini"    , "paevt"    , "pains"    ,

                          "actflag"  , "blank"    , "buffer"   , "curcom"   ,
                          "curpat"   , "deccom"   , "drvseq"   , "endflg"   ,
                          "errflg"   , "filname"  , "filpnt"   , "findex"   ,
                          "flag"     , "format"   , "index"    , "insname"  ,
                          "instance" , "iolnbr"   , "label"    , "length"   ,
                          "line"     , "lineno"   , "mode"     , "model"    ,
                          "name"     , "next"     , "nxtpat"   , "oldcom"   ,
                          "oldpat"   , "patnbr"   , "position" , "savflg"   ,
                          "sig"      , "simflag"  , "simval"   , "subseq"   ,
                          "text"     , "time"     , "usrval"   , "value"    ,
                          "flags"    , "time_unit", "time_step"
                          };

	/* ###------------------------------------------------------### */
	/*    initialisation :						*/
	/*    - allocate a buffer for read words			*/
	/*    - break the argument that identifies the structure (type)	*/
	/*      into words						*/
	/*    - search that words among recognized strings		*/
	/* ###------------------------------------------------------### */

  words [0] = buffer;
  get_size (siz);

  wrdcnt        = splitline (words, type);
  idx           = translate (words, wrdcnt, str, nmbrs, flags, indxs);

  typ [idx]     = POINTER_DFN | s_DFN | idx;
  pnt [idx].dat = head_pnt;

	/* ###------------------------------------------------------### */
	/*    process the command line until the _exit command		*/
	/* ###------------------------------------------------------### */

  while ((idx & TYPE_DFN) != _exit_DFN)
    {
	/* ###------------------------------------------------------### */
	/*    if the first word of the line has not been recognized,	*/
	/* print an error message. Otherwise, proccess the command line	*/
	/* (generally it is a request for displaying a specific field).	*/
	/*								*/
	/* At this point :						*/
	/*    - pnt [] contains all available pointers			*/
	/*    - typ [] contains the type of availabale pointers		*/
	/* ###------------------------------------------------------### */

    if (idx == _error_DFN)
      pat_error (103, NULL, NULL, 0);
    else
      {

	/* ###------------------------------------------------------### */
	/*    define the type of the structure that may be pushed on	*/
	/* the stack :							*/
	/*    - if the pointer is a VOID pointer the exact type must be	*/
	/*      defined on the command line (last word of the command)	*/
	/* ###------------------------------------------------------### */

      pshtype = POINTER_DFN | s_DFN | void_DFN;
      if ((typ [idx] & TYPE_DFN) == void_DFN)
        {
        if ((wrdcnt >= 2) && (indxs [wrdcnt - 1] != _error_DFN))
          pshtype = POINTER_DFN | s_DFN | indxs [wrdcnt - 1];
        else
          pat_error (105, NULL, NULL, 0);
        }
      else
       pshtype = typ [idx];

	/* ###------------------------------------------------------### */
	/*    depending on the kind of the first word of the command	*/
	/* activate actions :						*/
	/*    - COMMAND (_top, _up, ...)				*/
	/*    - POINTER							*/
	/*        - for NEXT go forward until the Nth element of the	*/
	/*          list. Then, push it on the stack and read fields	*/
	/*        - for others push and read fields			*/
	/*    - ARRAY							*/
	/*        - push the Nth element of the array and read its	*/
	/*          fields						*/
	/*    - ARRAY OF POINTER					*/
	/*        - push the object which address is the Nth element of	*/
	/*          the array and read its fields			*/
	/* ###------------------------------------------------------### */

      switch (typ [idx] & KIND_DFN)
        {

	/* ###------------------------------------------------------### */
	/*    COMMANDS ...						*/
	/* ###------------------------------------------------------### */

        case COMMAND_DFN :

          switch (typ [idx] & TYPE_DFN)
            {
	/* ###------------------------------------------------------### */
	/*    _top COMMAND : reset the stack pointer, call read_field	*/
	/* to read the structure on the top of stack			*/
	/* ###------------------------------------------------------### */

            case _top_DFN :
              stkpnt  = 0;
              readflg = 1;
              break;

	/* ###------------------------------------------------------### */
	/*    _stop COMMAND : set the stop mark for the structure on	*/
	/* the top of stack						*/
	/* ###------------------------------------------------------### */

            case _stop_DFN :
              stk [stkpnt].mark = 1;
              pat_message (1, "pat_debug", NULL, 0);
              break;

	/* ###------------------------------------------------------### */
	/*    _back COMMAND : pop structures from the stack until a	*/
	/* marked structure is found. Call read_field to read the	*/
	/* structure on the top of stack				*/
	/* ###------------------------------------------------------### */

            case _back_DFN :
              while ((stkpnt != 0) && (stk[--stkpnt].mark != 1));
              readflg = 1;
              break;

	/* ###------------------------------------------------------### */
	/*    _jump COMMAND : push on the stack the structure saved in	*/
	/* the jump table. Call read_field to read the structure on the	*/
	/* top of stack							*/
	/* ###------------------------------------------------------### */

            case _jump_DFN :
              if ((wrdcnt == 2) && (flags [1] == 1) && (nmbrs [1] < 10))
                {
                push (stk, &stkpnt, jtab[nmbrs[1]].data, jtab[nmbrs[1]].type);
                readflg = 1;
                }
              else
                pat_error (103, NULL, NULL, 0);
              break;

	/* ###------------------------------------------------------### */
	/*    _up COMMAND : pop structure from the stack. Call		*/
	/* read_field to read the structure on the top of stack		*/
	/* ###------------------------------------------------------### */

            case _up_DFN :
              if (wrdcnt == 1)
                {
                pop (&stkpnt, 1);
                readflg = 1;
                }
              else
                {
                if ((wrdcnt == 2) && (flags [1] == 1))
                  {
                  pop (&stkpnt, nmbrs [1] + 1);
                  readflg = 1;
                  }
                else
                  pat_error (103, NULL, NULL, 0);
                }
              break;

	/* ###------------------------------------------------------### */
	/*    _save COMMAND : save the structure on the top of stack in	*/
	/* the jump table						*/
	/* ###------------------------------------------------------### */

            case _save_DFN :
              if ((wrdcnt == 2) && (flags [1] == 1) && (nmbrs [1] < 10))
                jtab [nmbrs[1]] = stk [stkpnt];
              else
                pat_error (103, NULL, NULL, 0);
              break;

	/* ###------------------------------------------------------### */
	/*    _display COMMAND : display a specified field as an	*/
	/* immediate							*/
	/* ###------------------------------------------------------### */

            case _display_DFN :
              if ((wrdcnt == 3) && (indxs [1] != _error_DFN))
                disp_immd (str, pnt [indxs [1]], indxs [2]);
              else
                pat_error (103, NULL, NULL, 0);
              break;

            }
          break;

	/* ###------------------------------------------------------### */
	/*   POINTERS ...						*/
	/* ###------------------------------------------------------### */

        case POINTER_DFN :
          if (idx == next_DFN)
            {
            if ((wrdcnt == 1) || ((wrdcnt == 2) && (flags [1] == 0)))
              {
              push (stk, &stkpnt, pnt [idx].dat, pshtype);
              readflg = 1;
              }
            else
              {
              if (((wrdcnt == 3) || (wrdcnt == 2)) && (flags [1] == 1))
                {
                pnt [idx].dat = (void *) go_forward (pnt [idx].dat, nmbrs [1]);
                push (stk, &stkpnt, pnt [idx].dat, pshtype);
                readflg = 1;
                }
              else
                pat_error (103, NULL, NULL, 0);
              }
            }
          else
            {
            push (stk, &stkpnt, pnt [idx].dat, pshtype);
            readflg = 1;
            }
          break;

	/* ###------------------------------------------------------### */
	/*   ARRAIES of structure					*/
	/* ###------------------------------------------------------### */

        case ARRAY_DFN :
          if ((wrdcnt > 1) && (flags [1] == 1))
            {
            size = siz [(typ [idx] & TYPE_DFN)];
            pntr = (void *)
                   (((unsigned int) pnt [idx].dat) + (size * nmbrs [1]));
            push (stk, &stkpnt, pntr, pshtype);
            readflg = 1;
            }
          else
            pat_error (103, NULL, NULL, 0);
          break;

	/* ###------------------------------------------------------### */
	/*   ARRAIES of pointers					*/
	/* ###------------------------------------------------------### */

        case ARYOFPNT_DFN :
          if ((wrdcnt > 1) && (flags [1] == 1))
            {
            size = sizeof (void *);
            pntr = * (void **)
                     (((unsigned int) pnt [idx].dat) + (size * nmbrs [1]));
            push (stk, &stkpnt, pntr, pshtype);
            readflg = 1;
            }
          else
            pat_error (103, NULL, NULL, 0);
          break;

        }

      if (readflg == 1)
        {
        readflg = 0;
        read_field (stk [stkpnt], pnt, typ, str);
        }

      }

    printf ("\n\nCOMMAND > ");

    gets (line);
    wrdcnt = splitline (words, line);
    idx    = translate (words, wrdcnt, str, nmbrs, flags, indxs);
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: read_field						*/
/* description	: read the content of a data structure			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void read_field (curpnt, pnt, typ, str)

struct stack   curpnt;			/* current struct. to be read	*/
union value   *pnt   ;			/* child structures or data	*/
long          *typ   ;			/* child struct. or data type	*/
char         **str   ;			/* recognized strings		*/
  {

  int            i;

  struct paseq  *paseq_pnt;
  struct pagrp  *pagrp_pnt;
  struct paiol  *paiol_pnt;
  struct papat  *papat_pnt;
  struct pacom  *pacom_pnt;
  struct paini  *paini_pnt;
  struct paevt  *paevt_pnt;
  struct pains  *pains_pnt;

  char         **string_pnt;
  char          *character_pnt;
  int           *integer_pnt;

	/* ###------------------------------------------------------### */
	/*    initialization						*/
	/* ###------------------------------------------------------### */

  for (i=0 ; i<MAXCMD_DFN ; i++)
    {
    typ [i]     = _error_DFN;
    pnt [i].dat = NULL;
    }

  typ [_exit_DFN]    = COMMAND_DFN | _exit_DFN   ;
  typ [_up_DFN]      = COMMAND_DFN | _up_DFN     ;
  typ [_top_DFN]     = COMMAND_DFN | _top_DFN    ;
  typ [_stop_DFN]    = COMMAND_DFN | _stop_DFN   ;
  typ [_back_DFN]    = COMMAND_DFN | _back_DFN   ;
  typ [_jump_DFN]    = COMMAND_DFN | _jump_DFN   ;
  typ [_save_DFN]    = COMMAND_DFN | _save_DFN   ;
  typ [_display_DFN] = COMMAND_DFN | _display_DFN;

  switch (curpnt.type & TYPE_DFN)
    {
	/* ###------------------------------------------------------### */
	/*    depending on the type of the current structure :		*/
	/*								*/
	/*       - define the type of each field :			*/
	/*          - the field contains an immediate or a pointer	*/
	/*          - what format must be used to print it		*/
	/*          - kind of data it contains				*/
	/*								*/
	/*       - register the data contained in the field		*/
	/* ###------------------------------------------------------### */

	/* ###------------------------------------------------------### */
	/*    paseq							*/
	/* ###------------------------------------------------------### */

    case paseq_DFN :

      paseq_pnt = (struct paseq *) curpnt.data;

      typ [next_DFN]       = POINTER_DFN   | s_DFN | paseq_DFN;
      typ [name_DFN]       = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [pagrp_DFN]      = POINTER_DFN   | s_DFN | pagrp_DFN;
      typ [paiol_DFN]      = POINTER_DFN   | s_DFN | paiol_DFN;
      typ [deccom_DFN]     = POINTER_DFN   | s_DFN | pacom_DFN;
      typ [oldpat_DFN]     = POINTER_DFN   | s_DFN | papat_DFN;
      typ [oldcom_DFN]     = POINTER_DFN   | s_DFN | pacom_DFN;
      typ [curpat_DFN]     = POINTER_DFN   | s_DFN | papat_DFN;
      typ [curcom_DFN]     = POINTER_DFN   | s_DFN | pacom_DFN;
      typ [nxtpat_DFN]     = POINTER_DFN   | s_DFN | papat_DFN;
      typ [model_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [buffer_DFN]     = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [pains_DFN]      = POINTER_DFN   | s_DFN | pains_DFN;
      typ [patnbr_DFN]     = IMMEDIATE_DFN | u_DFN | integer_DFN;
      typ [lineno_DFN]     = IMMEDIATE_DFN | u_DFN | integer_DFN;
      typ [iolnbr_DFN]     = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [subseq_DFN]     = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [drvseq_DFN]     = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [savflg_DFN]     = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [endflg_DFN]     = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [errflg_DFN]     = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [flags_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [time_unit_DFN]  = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [time_step_DFN]  = IMMEDIATE_DFN | u_DFN | integer_DFN;

      pnt [next_DFN].dat      = paseq_pnt->NEXT     ;
      pnt [name_DFN].dat      = paseq_pnt->NAME     ;
      pnt [pagrp_DFN].dat     = paseq_pnt->PAGRP    ;
      pnt [paiol_DFN].dat     = paseq_pnt->PAIOL    ;
      pnt [deccom_DFN].dat    = paseq_pnt->DECCOM   ;
      pnt [oldpat_DFN].dat    = paseq_pnt->OLDPAT   ;
      pnt [oldcom_DFN].dat    = paseq_pnt->OLDCOM   ;
      pnt [curpat_DFN].dat    = paseq_pnt->CURPAT   ;
      pnt [curcom_DFN].dat    = paseq_pnt->CURCOM   ;
      pnt [nxtpat_DFN].dat    = paseq_pnt->NXTPAT   ;
      pnt [model_DFN].dat     = paseq_pnt->MODEL    ;
      pnt [buffer_DFN].dat    = paseq_pnt->BUFFER   ;
      pnt [pains_DFN].dat     = paseq_pnt->PAINS    ;
      pnt [patnbr_DFN].imd    = paseq_pnt->PATNBR   ;
      pnt [lineno_DFN].imd    = paseq_pnt->LINENO   ;
      pnt [iolnbr_DFN].imd    = paseq_pnt->IOLNBR   ;
      pnt [subseq_DFN].imd    = paseq_pnt->SUBSEQ   ;
      pnt [drvseq_DFN].imd    = paseq_pnt->DRVSEQ   ;
      pnt [savflg_DFN].imd    = paseq_pnt->SAVFLG   ;
      pnt [endflg_DFN].imd    = paseq_pnt->ENDFLG   ;
      pnt [errflg_DFN].imd    = paseq_pnt->ERRFLG   ;
      pnt [flags_DFN].imd     = paseq_pnt->FLAGS    ;
      pnt [time_unit_DFN].imd = paseq_pnt->TIME_UNIT;
      pnt [time_step_DFN].imd = paseq_pnt->TIME_STEP;

      break;

	/* ###------------------------------------------------------### */
	/*    pains							*/
	/* ###------------------------------------------------------### */

    case pains_DFN :

      pains_pnt = (struct pains *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | pains_DFN;
      typ [insname_DFN]   = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [filname_DFN]   = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [instance_DFN]  = POINTER_DFN   | s_DFN | void_DFN;
      typ [model_DFN]     = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [value_DFN]     = ARYOFPNT_DFN  | s_DFN | character_DFN;

      pnt [next_DFN].dat     = pains_pnt->NEXT;
      pnt [insname_DFN].dat  = pains_pnt->INSNAME;
      pnt [filname_DFN].dat  = pains_pnt->FILNAME;
      pnt [instance_DFN].dat = pains_pnt->INSTANCE;
      pnt [model_DFN].dat    = pains_pnt->MODEL;
      pnt [value_DFN].dat   = pains_pnt->VALUE;

      break;

	/* ###------------------------------------------------------### */
	/*    pagrp							*/
	/* ###------------------------------------------------------### */

    case pagrp_DFN :

      pagrp_pnt = (struct pagrp *) curpnt.data;

      typ [next_DFN]   = POINTER_DFN   | s_DFN | pagrp_DFN;
      typ [name_DFN]   = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [length_DFN] = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [findex_DFN] = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [flag_DFN]   = IMMEDIATE_DFN | d_DFN | short_DFN;

      pnt [next_DFN].dat   = pagrp_pnt->NEXT;
      pnt [name_DFN].dat   = pagrp_pnt->NAME;
      pnt [length_DFN].imd = pagrp_pnt->LENGTH;
      pnt [findex_DFN].imd = pagrp_pnt->FINDEX;
      pnt [flag_DFN].imd   = pagrp_pnt->FLAG;

      break;

	/* ###------------------------------------------------------### */
	/*    paiol							*/
	/* ###------------------------------------------------------### */

    case paiol_DFN :

      paiol_pnt = (struct paiol *) curpnt.data;

      typ [next_DFN]     = POINTER_DFN   | s_DFN | paiol_DFN;
      typ [name_DFN]     = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [format_DFN]   = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [mode_DFN]     = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [blank_DFN]    = IMMEDIATE_DFN | u_DFN | character_DFN;
      typ [value_DFN]    = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [sig_DFN]      = POINTER_DFN   | s_DFN | void_DFN;
      typ [flag_DFN]     = IMMEDIATE_DFN | d_DFN | character_DFN;

      pnt [next_DFN].dat    = paiol_pnt->NEXT;
      pnt [name_DFN].dat    = paiol_pnt->NAME;
      pnt [format_DFN].imd  = paiol_pnt->FORMAT;
      pnt [mode_DFN].imd    = paiol_pnt->MODE;
      pnt [blank_DFN].imd   = paiol_pnt->BLANK;
      pnt [value_DFN].imd   = paiol_pnt->VALUE;
      pnt [sig_DFN].dat     = paiol_pnt->SIG;
      pnt [flag_DFN].imd    = paiol_pnt->FLAG;

      break;

	/* ###------------------------------------------------------### */
	/*    papat							*/
	/* ###------------------------------------------------------### */

    case papat_DFN :

      papat_pnt = (struct papat *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | papat_DFN    ;
      typ [label_DFN]     = IMMEDIATE_DFN | s_DFN | string_DFN   ;
      typ [paini_DFN]     = POINTER_DFN   | s_DFN | paini_DFN    ;
      typ [paevt_DFN]     = POINTER_DFN   | s_DFN | paevt_DFN    ;
      typ [time_DFN]      = IMMEDIATE_DFN | u_DFN | integer_DFN  ;
      typ [line_DFN]      = IMMEDIATE_DFN | u_DFN | short_DFN    ;
      typ [blank_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;
      typ [actflag_DFN]   = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [simflag_DFN]   = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [flag_DFN]      = IMMEDIATE_DFN | x_DFN | character_DFN;

      pnt [next_DFN].dat    = papat_pnt->NEXT   ;
      pnt [label_DFN].dat   = papat_pnt->LABEL  ;
      pnt [paini_DFN].dat   = papat_pnt->PAINI  ;
      pnt [paevt_DFN].dat   = papat_pnt->PAEVT  ;
      pnt [time_DFN].imd    = papat_pnt->TIME   ;
      pnt [line_DFN].imd    = papat_pnt->LINE   ;
      pnt [blank_DFN].imd   = papat_pnt->BLANK  ;
      pnt [actflag_DFN].imd = papat_pnt->ACTFLAG;
      pnt [simflag_DFN].imd = papat_pnt->SIMFLAG;
      pnt [flag_DFN].imd    = papat_pnt->FLAG   ;

      break;

	/* ###------------------------------------------------------### */
	/*    paini							*/
	/* ###------------------------------------------------------### */

    case paini_DFN :

      paini_pnt = (struct paini *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | paini_DFN    ;
      typ [name_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN   ;
      typ [value_DFN]     = IMMEDIATE_DFN | x_DFN | character_DFN;

      pnt [next_DFN].dat  = paini_pnt->NEXT ;
      pnt [name_DFN].dat  = paini_pnt->NAME ;
      pnt [value_DFN].imd = paini_pnt->VALUE;

      break;

	/* ###------------------------------------------------------### */
	/*    paevt							*/
	/* ###------------------------------------------------------### */

    case paevt_DFN :

      paevt_pnt = (struct paevt *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | paevt_DFN;
      typ [index_DFN]     = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [usrval_DFN]    = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [simval_DFN]    = IMMEDIATE_DFN | x_DFN | character_DFN;

      pnt [next_DFN].dat   = paevt_pnt->NEXT;
      pnt [index_DFN].imd  = paevt_pnt->INDEX;
      pnt [usrval_DFN].imd = paevt_pnt->USRVAL;
      pnt [simval_DFN].imd = paevt_pnt->SIMVAL;

      break;

	/* ###------------------------------------------------------### */
	/*    pacom							*/
	/* ###------------------------------------------------------### */

    case pacom_DFN :

      pacom_pnt = (struct pacom *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | pacom_DFN;
      typ [text_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [position_DFN]  = IMMEDIATE_DFN | u_DFN | short_DFN;

      pnt [next_DFN].dat     = pacom_pnt->NEXT;
      pnt [text_DFN].dat     = pacom_pnt->TEXT;
      pnt [position_DFN].imd = pacom_pnt->POSITION;

      break;

	/* ###------------------------------------------------------### */
	/*    string							*/
	/* ###------------------------------------------------------### */

    case string_DFN :

      string_pnt = (char **) curpnt.data;

      typ [string_DFN]     = IMMEDIATE_DFN | s_DFN | string_DFN;
      pnt [string_DFN].dat = *string_pnt;

      break;

	/* ###------------------------------------------------------### */
	/*    character							*/
	/* ###------------------------------------------------------### */

    case character_DFN :

      character_pnt = (char *) curpnt.data;

      typ [character_DFN]     = IMMEDIATE_DFN | x_DFN | character_DFN;
      pnt [character_DFN].imd = *character_pnt;

      break;

	/* ###------------------------------------------------------### */
	/*    integer							*/
	/* ###------------------------------------------------------### */

    case integer_DFN :

      integer_pnt = (int *) curpnt.data;

      typ [integer_DFN]     = IMMEDIATE_DFN | d_DFN | integer_DFN;
      pnt [integer_DFN].imd = *integer_pnt;

      break;

    }

	/* ###------------------------------------------------------### */
	/*    scan the list of recongnized strings to print those	*/
	/* values that are not empty (different form _error_DFN) :	*/
	/*								*/
	/*   - if it contains an immediate, print it following the	*/
	/*     required format						*/
	/*								*/
	/*   - if it contains a pointer, just print the pointer		*/
	/*     indication, ->, and "availbale"				*/
	/*								*/
	/*   - if it contains an array, just print the array		*/
	/*     indication, =>, and "availbale"				*/
	/* ###------------------------------------------------------### */

  printf ("--------- current structure : %s ---------\n\n",
          str [curpnt.type & TYPE_DFN]);

  for (i=0 ; i<MAXCMD_DFN ; i++)
    {
    if ((typ [i] & KIND_DFN) == IMMEDIATE_DFN)
      {
      printf ("   %-15s: ", str [i]);
      switch (typ [i] & FORMAT_DFN)
        {
        case s_DFN :
          if (pnt[i].dat != NULL)
            printf ("%s", (char *) pnt[i].dat);
          printf ("\n");
          break;

        case u_DFN :
          printf ("%u\n", (unsigned int) pnt[i].imd);
          break;

        case c_DFN :
          printf ("%c\n", (unsigned char) pnt[i].imd);
          break;

        case d_DFN :
          printf ("%d\n", (int) pnt[i].imd);
          break;

        case l_DFN :
          printf ("%ld\n", (long) pnt[i].imd);
          break;

        case x_DFN :
          printf ("0x%x\n", (unsigned int) pnt[i].imd);
          break;
        }
      }

    if ((typ [i] & KIND_DFN) == POINTER_DFN)
      {
      printf ("-> %-15s: ", str [i]);
      if (pnt[i].dat != NULL)
        printf ("available");
      printf ("\n");
      }

    if ( ((typ [i] & KIND_DFN) == ARRAY_DFN   ) ||
         ((typ [i] & KIND_DFN) == ARYOFPNT_DFN) )
      {
      printf ("=> %-15s: ", str [i]);
      if (pnt[i].dat != NULL)
        printf ("available");
      printf ("\n");
      }
    }

  }

/* ###--------------------------------------------------------------### */
/* function	: splitline						*/
/* description	: read a line (the space must have been reserved by the	*/
/*		  caller - *words) from the standard input and split it	*/
/*		  into seperate words. Return the number of words read.	*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static int splitline (words, line)

char **words;
char  *line ;
  {

  char *heap   = *words;
  int   new    =  1;
  int   wrdcnt =  0;
  int   i      =  0;
  int   j      =  0;

	/* ###------------------------------------------------------### */
	/*    copy the line read from input into the words until a	*/
	/* space has been found. Then, increment the word count and	*/
	/* begin filling the next word					*/
	/* ###------------------------------------------------------### */

  while ((heap[j] = line[i++]) != '\0')
    {
    if ((heap[j] != ' ') && (heap[j] != '\t'))
      {
      new = 0;
      j++;
      }
    else
      {
      if (new != 1)
        {
        heap  [j++]      = '\0';
        words [++wrdcnt] = heap + j;
        new = 1;
        }
      }
    }

  if (new != 1)
    wrdcnt++;

  return (wrdcnt);
  }

/* ###--------------------------------------------------------------### */
/* function	: push							*/
/* description	: push a structure on the stack				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void push (stk, stkpnt, data, type)

struct stack *stk;
int          *stkpnt;
void         *data;
short         type;

  {
  int stkidx = *stkpnt;

  if (stkidx == STKSIZ_DFN)
    pat_message (2, "pat_debug", NULL, 0);
  else
    {
    if (data != NULL)
      {
      stkidx++;
      stk [stkidx].data = data;
      stk [stkidx].type = type;
      stk [stkidx].mark = 0;
      *stkpnt           = stkidx;
      }
    else
      pat_error (104, NULL, NULL, 0);
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: pop							*/
/* description	: pop N structures from the stack			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void pop (stkpnt, count)

int          *stkpnt;
int           count;

  {
  int stkidx = *stkpnt;

  if (stkidx <= count)
    stkidx = 0;
  else
    stkidx -= count;

  *stkpnt = stkidx;
  }

/* ###--------------------------------------------------------------### */
/* function	: go_forward						*/
/* description	: go forward through the NEXT field to find the Nth	*/
/*		  structure in the list and return it			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static struct chain *go_forward (pnt, count)

struct chain *pnt;
int           count;

  {
  while ((count > 0) && (pnt != NULL) && (pnt->NEXT != NULL))
    {
    pnt = pnt->NEXT;
    count--;
    }
  return (pnt);
  }

/* ###--------------------------------------------------------------### */
/* function	: translate						*/
/* description	: find the index of each word read from the command	*/
/*		  line. For each word check if it is a number. Return	*/
/*		  the index of the first word.				*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static int translate (words, wrdcnt, strgs, nmbrs, flags, indxs)

char **words ;				/* list of words		*/
int    wrdcnt;				/* number of words		*/
char **strgs ;				/* list of recognized strings	*/
int   *nmbrs ;				/* words translated to numbers	*/
char  *flags ;				/* set if word is a number	*/
int   *indxs ;				/* words' index in strgs table	*/

  {
  int i , j;

	/* ###------------------------------------------------------### */
	/*    initialize flags and indxs				*/
	/* ###------------------------------------------------------### */

  for (i=0 ; i<10 ; i++)
    {
    flags [i] = 0;
    indxs [i] = _error_DFN;
    }

	/* ###------------------------------------------------------### */
	/*   for each word :						*/
	/*      - check if it is a number				*/
	/*      - try to locate the word among the recognized strings.	*/
	/*        If found save it's index				*/
	/* ###------------------------------------------------------### */

  for (i=0 ; i<wrdcnt ; i++)
    {
    flags [i] = sscanf (words [i], "%u", &nmbrs [i]);
    for (j=0; j<MAXCMD_DFN ; j++)
      {
      if (!strcmp (strgs [j], words [i]))
        {
        indxs [i] = j;
        break;
        }
      }
    }
  return (indxs [0]);
  }

static void get_size (siz)

unsigned int siz [];

  {
  int i;
 
  for (i=0 ; i<MAXCMD_DFN ; i++)
    siz [i] = 0;


  siz [character_DFN] = sizeof (char);
  siz [short_DFN]     = sizeof (short);
  siz [integer_DFN]   = sizeof (int);
  siz [long_DFN]      = sizeof (long);
  siz [string_DFN]    = sizeof (char *);

  siz [chain_DFN]     = sizeof (struct chain);
  siz [ptype_DFN]     = sizeof (struct ptype);

  siz [paseq_DFN]     = sizeof (struct paseq);
  siz [pagrp_DFN]     = sizeof (struct pagrp);
  siz [paiol_DFN]     = sizeof (struct paiol);
  siz [papat_DFN]     = sizeof (struct papat);
  siz [pacom_DFN]     = sizeof (struct pacom);
  siz [paini_DFN]     = sizeof (struct paini);
  siz [paevt_DFN]     = sizeof (struct paevt);
  siz [pains_DFN]     = sizeof (struct pains);

  }

/* ###--------------------------------------------------------------### */
/* function	: disp_immd						*/
/* description	: display a specific field as an immediate		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void disp_immd (str, pnt , type)

char        **str;
union value pnt  ;
int         type ;

  {
  printf ("   %-15s: ", str [type]);
  switch (type)
    {
    case string_DFN :
      if (((char *) pnt.dat) != NULL)
        printf ("%s", (char *) pnt.dat);
      printf ("\n");
      break;

    case character_DFN :
      printf ("%c\n", (unsigned char) pnt.imd);
      break;

    case short_DFN     :
    case integer_DFN   :
    case long_DFN      :
      printf ("0x%x\n", (unsigned int) pnt.imd);
      break;

    default :
      pat_error (103, NULL, NULL, 0);
    }
  }
