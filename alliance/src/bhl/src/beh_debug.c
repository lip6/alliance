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
/* file		: beh_debug.c						*/
/* date		: Feb 29 2000						*/
/* version	: v111							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "log.h"
#include "beh.h"
#include "beh_debug.h"

static char           *buffer      = NULL;
static unsigned int    buff_size   = 0;
static struct circuit *circuit_pnt = NULL;

/* ###--------------------------------------------------------------### */
/* function	: beh_debug						*/
/* description	: dispaly any mad structure				*/
/* called func.	: goforward, translate, dispaly, splitline, pop, push	*/
/* ###--------------------------------------------------------------### */

void beh_debug (head_pnt, type)

void *head_pnt;				/* structure's pointer		*/
char *type;				/* structure's type		*/

  {
  static void          disp_immd ();
  static void          getsize   ();
  static void          display   ();
  static void          push      ();
  static void          pop       ();
  static struct chain *goforward ();
  static int           splitline ();
  static int           translate ();

  char          line   [128];		/* buffer to read a cmd line	*/
  char          heap   [128];		/* buffer to split the cmd line	*/

  char         *words  [ 10];		/* number of words on a line	*/
  int           nmbrs  [ 10];		/* words translated into number	*/
  char          flags  [ 10];		/* set if words is a number	*/
  int           indxs  [ 10];		/* index of words		*/

  struct stack  jtab   [ 10];		/* list of memorized addresses	*/
  int           idx, i;
  int           dispflg = 0;
  int           code;
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
                          "chain"    , "ptype"    , "befig"    , "bereg"    ,
                          "bemsg"    , "berin"    , "beout"    , "bebus"    ,
                          "beaux"    , "bedly"    , "bebux"    , "bepor"    ,
                          "begen"    , "biabl"    , "binode"   , "bequad"   ,
                          "beder"    ,
                          "next"     , "name"     , "user"     , "errflg"   ,
                          "type"     , "level"    , "label"    , "message"  ,
                          "abl"      , "node"     , "direction", "cndabl"   ,
                          "valabl"   , "cndnode"  , "valnode"  , "out_ref"  ,
                          "bus_ref"  , "aux_ref"  , "reg_ref"  , "bux_ref"  ,
                          "msg_ref"  , "dly_ref"  , "value"    , "data"     ,
                          "time"     , "bdd"      , "gex"      , "varlist"  ,
                          "derivate" , "index"    , "flag"     , "time_unit",
                          "time_step"
                          };

	/* ###------------------------------------------------------### */
	/*    initialisation :						*/
	/*    - allocate a buffer for read words			*/
	/*    - break the argument that identifies the structure (type)	*/
	/*      into words						*/
	/*    - search that words among recognized strings		*/
	/* ###------------------------------------------------------### */

  words [0] = heap;
  getsize (siz);

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
      fprintf (stderr ,"unrecognized command\n");
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
          fprintf (stderr ,"structure has unknown type (void *)\n");
        }
      else
       pshtype = typ [idx];

	/* ###------------------------------------------------------### */
	/*    depending on the kind of the first word of the command	*/
	/* activate actions :						*/
	/*    - COMMAND (_top, _up, ...)				*/
	/*    - POINTER							*/
	/*        - for NEXT go forward until the Nth element of the	*/
	/*          list. Then, push it on the stack and dispaly it	*/
	/*        - for others push and display				*/
	/*    - ARRAY							*/
	/*        - push and display the Nth element of the array	*/
	/* ###------------------------------------------------------### */

      switch (typ [idx] & KIND_DFN)
        {

        case COMMAND_DFN :
          switch (typ [idx] & TYPE_DFN)
            {
	/* ###------------------------------------------------------### */
	/*    _top COMMAND : reset the stack pointer, call read_field	*/
	/* to read the structure on the top of stack			*/
	/* ###------------------------------------------------------### */

            case _top_DFN :
              stkpnt  = 0;
              dispflg = 1;
              break;

	/* ###------------------------------------------------------### */
	/*    _stop COMMAND : set the stop mark for the structure on	*/
	/* the top of stack						*/
	/* ###------------------------------------------------------### */

            case _stop_DFN :
              stk [stkpnt].mark = 1;
              fprintf (stdout, "beh_debug : stop mark set\n");
              break;

	/* ###------------------------------------------------------### */
	/*    _back COMMAND : pop structures from the stack until a	*/
	/* marked structure is found. Call read_field to read the	*/
	/* structure on the top of stack				*/
	/* ###------------------------------------------------------### */

            case _back_DFN :
              while ((stkpnt != 0) && (stk[--stkpnt].mark != 1));
              dispflg = 1;
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
                dispflg = 1;
                }
              else
                fprintf (stderr ,"unrecognized command\n");
              break;

	/* ###------------------------------------------------------### */
	/*    _up COMMAND : pop structure from the stack. Call		*/
	/* read_field to read the structure on the top of stack		*/
	/* ###------------------------------------------------------### */

            case _up_DFN :
              if (wrdcnt == 1)
                {
                pop (&stkpnt, 1);
                dispflg = 1;
                }
              else
                {
                if ((wrdcnt == 2) && (flags [1] == 1))
                  {
                  pop (&stkpnt, nmbrs [1] + 1);
                  dispflg = 1;
                  }
                else
                  fprintf (stderr ,"unrecognized command\n");
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
                fprintf (stderr ,"unrecognized command\n");
              break;

	/* ###------------------------------------------------------### */
	/*    _display COMMAND : display a specified field as an	*/
	/* immediate							*/
	/* ###------------------------------------------------------### */

            case _display_DFN :
              if ((wrdcnt == 3) && (indxs [1] != _error_DFN))
                disp_immd (str, pnt [indxs [1]], indxs [2]);
              else
                fprintf (stderr ,"unrecognized command\n");
              break;

            }
          break;

        case POINTER_DFN :
          if (idx == next_DFN)
            {
            if ((wrdcnt == 1) || ((wrdcnt == 2) && (flags [1] == 0)))
              {
              push (stk, &stkpnt, pnt [idx].dat, pshtype);
              dispflg = 1;
              }
            else
              {
              if (((wrdcnt == 3) || (wrdcnt == 2)) && (flags [1] == 1))
                {
                pnt [idx].dat = (void *) goforward (pnt [idx].dat, nmbrs [1]);
                push (stk, &stkpnt, pnt [idx].dat, pshtype);
                dispflg = 1;
                }
              else
                fprintf (stderr ,"unrecognized command\n");
              }
            }
          else
            {
            push (stk, &stkpnt, pnt [idx].dat, pshtype);
            dispflg = 1;
            }
          break;

        case ARRAY_DFN :
          if ((wrdcnt > 1) && (flags [1] == 1))
            {
            size = siz [(typ [idx] & TYPE_DFN)];
            pntr = (void *)
                   (((unsigned int) pnt [idx].dat) + (size * nmbrs [1]));
            push (stk, &stkpnt, pntr, pshtype);
            dispflg = 1;
            }
          else
            fprintf (stderr ,"unrecognized command\n");
          break;

        case ARYOFPNT_DFN :
          if ((wrdcnt > 1) && (flags [1] == 1))
            {
            size = sizeof (void *);
            pntr = * (void **)
                     (((unsigned int) pnt [idx].dat) + (size * nmbrs [1]));
            push (stk, &stkpnt, pntr, pshtype);
            dispflg = 1;
            }
          else
            fprintf (stderr ,"unrecognized command\n");
          break;

        }

      if (dispflg == 1)
        {
        dispflg = 0;
        display (stk [stkpnt], pnt, typ, str);
        }

      }

    printf ("\n\nCOMMAND > ");

    gets (line);
    if (strcmp (line ,"."))
      {
      wrdcnt = splitline (words, line);
      idx    = translate (words, wrdcnt, str, nmbrs, flags, indxs);
      }
    }
  }

/* ###--------------------------------------------------------------### */
/* function	: display						*/
/* description	: display the fields of a structure			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void display (curpnt, pnt, typ, str)

struct stack   curpnt;			/* current struct. for display	*/
union value   *pnt;			/* child structures or data	*/
long          *typ;			/* child struct. or data type	*/
char         **str;			/* recognized strings		*/

  {
  extern char         *exprToCharInt ();
  extern struct chain *bddToAbl ();

  int                     i;
  struct chain           *ptr_abl;

  struct chain           *chain_pnt;
  struct ptype           *ptype_pnt;

  struct befig           *befig_pnt;
  struct bereg           *bereg_pnt;
  struct bemsg           *bemsg_pnt;
  struct berin           *berin_pnt;
  struct beout           *beout_pnt;
  struct bebus           *bebus_pnt;
  struct beaux           *beaux_pnt;
  struct beaux           *bedly_pnt;
  struct bebux           *bebux_pnt;
  struct bepor           *bepor_pnt;
  struct begen           *begen_pnt;
  struct biabl           *biabl_pnt;
  struct binode          *binode_pnt;
  struct bequad          *bequad_pnt;
  struct beder           *beder_pnt;

  char                  **string_pnt;
  char                   *character_pnt;
  int                    *int_pnt;

	/* ###------------------------------------------------------### */
	/*    initialization						*/
	/* ###------------------------------------------------------### */

  if (buffer == NULL)
    {
    buff_size = 1024;
    buffer    = (char *) mbkalloc (buff_size);
    }

  for (i=0 ; i<MAXCMD_DFN ; i++)
    {
    typ [i]     = _error_DFN;
    pnt [i].dat = NULL;
    }

  typ [_exit_DFN]     = COMMAND_DFN | _exit_DFN;
  typ [_up_DFN]       = COMMAND_DFN | _up_DFN;
  typ [_top_DFN]      = COMMAND_DFN | _top_DFN;
  typ [_stop_DFN]     = COMMAND_DFN | _stop_DFN;
  typ [_back_DFN]     = COMMAND_DFN | _back_DFN;
  typ [_jump_DFN]     = COMMAND_DFN | _jump_DFN;
  typ [_save_DFN]     = COMMAND_DFN | _save_DFN;
  typ [_display_DFN]  = COMMAND_DFN | _display_DFN;

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
	/*    befig							*/
	/* ###------------------------------------------------------### */

    case befig_DFN :

      befig_pnt = (struct befig *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | befig_DFN;
      typ [name_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [bereg_DFN]     = POINTER_DFN   | s_DFN | bereg_DFN;
      typ [bemsg_DFN]     = POINTER_DFN   | s_DFN | bemsg_DFN;
      typ [berin_DFN]     = POINTER_DFN   | s_DFN | berin_DFN;
      typ [beout_DFN]     = POINTER_DFN   | s_DFN | beout_DFN;
      typ [bebus_DFN]     = POINTER_DFN   | s_DFN | bebus_DFN;
      typ [beaux_DFN]     = POINTER_DFN   | s_DFN | beaux_DFN;
      typ [bedly_DFN]     = POINTER_DFN   | s_DFN | beaux_DFN;
      typ [bebux_DFN]     = POINTER_DFN   | s_DFN | bebux_DFN;
      typ [bepor_DFN]     = POINTER_DFN   | s_DFN | bepor_DFN;
      typ [begen_DFN]     = POINTER_DFN   | s_DFN | begen_DFN;
      typ [user_DFN]      = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [errflg_DFN]    = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [type_DFN]      = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [flag_DFN]      = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [time_unit_DFN] = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [time_step_DFN] = IMMEDIATE_DFN | u_DFN | integer_DFN;

      pnt [next_DFN].dat      = befig_pnt->NEXT     ;
      pnt [name_DFN].dat      = befig_pnt->NAME     ;
      pnt [bereg_DFN].dat     = befig_pnt->BEREG    ;
      pnt [bemsg_DFN].dat     = befig_pnt->BEMSG    ;
      pnt [berin_DFN].dat     = befig_pnt->BERIN    ;
      pnt [beout_DFN].dat     = befig_pnt->BEOUT    ;
      pnt [bebus_DFN].dat     = befig_pnt->BEBUS    ;
      pnt [beaux_DFN].dat     = befig_pnt->BEAUX    ;
      pnt [bedly_DFN].dat     = befig_pnt->BEDLY    ;
      pnt [bebux_DFN].dat     = befig_pnt->BEBUX    ;
      pnt [bepor_DFN].dat     = befig_pnt->BEPOR    ;
      pnt [begen_DFN].dat     = befig_pnt->BEGEN    ;
      pnt [user_DFN].dat      = befig_pnt->USER     ;
      pnt [errflg_DFN].imd    = befig_pnt->ERRFLG   ;
      pnt [type_DFN].imd      = befig_pnt->TYPE     ;
      pnt [flag_DFN].imd      = befig_pnt->FLAG     ;
      pnt [time_unit_DFN].imd = befig_pnt->TIME_UNIT;
      pnt [time_step_DFN].imd = befig_pnt->TIME_STEP;

      circuit_pnt = befig_pnt->CIRCUI;

      break;

	/* ###------------------------------------------------------### */
	/*    bereg							*/
	/* ###------------------------------------------------------### */

    case bereg_DFN :

      bereg_pnt = (struct bereg *) curpnt.data;

      typ [next_DFN]   = POINTER_DFN   | s_DFN | bereg_DFN;
      typ [name_DFN]   = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [biabl_DFN]  = POINTER_DFN   | s_DFN | biabl_DFN;
      typ [binode_DFN] = POINTER_DFN   | s_DFN | binode_DFN;

      pnt [next_DFN].dat   = bereg_pnt->NEXT;
      pnt [name_DFN].dat   = bereg_pnt->NAME;
      pnt [biabl_DFN].dat  = bereg_pnt->BIABL;
      pnt [binode_DFN].dat = bereg_pnt->BINODE;

      break;

	/* ###------------------------------------------------------### */
	/*    bemsg							*/
	/* ###------------------------------------------------------### */

    case bemsg_DFN :

      bemsg_pnt = (struct bemsg *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN   | bemsg_DFN;
      typ [level_DFN]   = IMMEDIATE_DFN | c_DFN   | character_DFN;
      typ [label_DFN]   = IMMEDIATE_DFN | s_DFN   | string_DFN;
      typ [message_DFN] = IMMEDIATE_DFN | s_DFN   | string_DFN;
      typ [abl_DFN]     = IMMEDIATE_DFN | ABL_DFN | abl_DFN;
      typ [node_DFN]    = POINTER_DFN   | s_DFN   | void_DFN;

      pnt [next_DFN].dat    = bemsg_pnt->NEXT;
      pnt [level_DFN].imd   = bemsg_pnt->LEVEL;
      pnt [label_DFN].dat   = bemsg_pnt->LABEL;
      pnt [message_DFN].dat = bemsg_pnt->MESSAGE;
      pnt [abl_DFN].dat     = bemsg_pnt->ABL;
      pnt [node_DFN].dat    = bemsg_pnt->NODE;

      break;

	/* ###------------------------------------------------------### */
	/*    beout							*/
	/* ###------------------------------------------------------### */

    case beout_DFN :

      beout_pnt = (struct beout *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN   | beout_DFN    ;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN   | string_DFN   ;
      typ [time_DFN]    = IMMEDIATE_DFN | l_DFN   | long_DFN     ;
      typ [flag_DFN]    = IMMEDIATE_DFN | x_DFN   | character_DFN;
      typ [abl_DFN]     = IMMEDIATE_DFN | ABL_DFN | abl_DFN      ;
      typ [node_DFN]    = POINTER_DFN   | s_DFN   | void_DFN     ;

      pnt [next_DFN].dat    = beout_pnt->NEXT;
      pnt [name_DFN].dat    = beout_pnt->NAME;
      pnt [time_DFN].imd    = beout_pnt->TIME;
      pnt [flag_DFN].imd    = beout_pnt->FLAG;
      pnt [abl_DFN].dat     = beout_pnt->ABL ;
      pnt [node_DFN].dat    = beout_pnt->NODE;

      break;

	/* ###------------------------------------------------------### */
	/*    bebus							*/
	/* ###------------------------------------------------------### */

    case bebus_DFN :

      bebus_pnt = (struct bebus *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN | bebus_DFN;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [biabl_DFN]   = POINTER_DFN   | s_DFN | biabl_DFN;
      typ [binode_DFN]  = POINTER_DFN   | s_DFN | binode_DFN;
      typ [type_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat    = bebus_pnt->NEXT;
      pnt [name_DFN].dat    = bebus_pnt->NAME;
      pnt [biabl_DFN].dat   = bebus_pnt->BIABL;
      pnt [binode_DFN].dat  = bebus_pnt->BINODE;
      pnt [type_DFN].imd    = bebus_pnt->TYPE;

      break;

	/* ###------------------------------------------------------### */
	/*    beaux							*/
	/* ###------------------------------------------------------### */

    case beaux_DFN :

      beaux_pnt = (struct beaux *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN   | beaux_DFN    ;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN   | string_DFN   ;
      typ [time_DFN]    = IMMEDIATE_DFN | l_DFN   | long_DFN     ;
      typ [flag_DFN]    = IMMEDIATE_DFN | x_DFN   | character_DFN;
      typ [abl_DFN]     = IMMEDIATE_DFN | ABL_DFN | abl_DFN      ;
      typ [node_DFN]    = POINTER_DFN   | s_DFN   | void_DFN     ;

      pnt [next_DFN].dat    = beaux_pnt->NEXT;
      pnt [name_DFN].dat    = beaux_pnt->NAME;
      pnt [time_DFN].imd    = beaux_pnt->TIME;
      pnt [flag_DFN].imd    = beaux_pnt->FLAG;
      pnt [abl_DFN].dat     = beaux_pnt->ABL ;
      pnt [node_DFN].dat    = beaux_pnt->NODE;

      break;

	/* ###------------------------------------------------------### */
	/*    bebux							*/
	/* ###------------------------------------------------------### */

    case bebux_DFN :

      bebux_pnt = (struct bebux *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN | bebux_DFN;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [biabl_DFN]   = POINTER_DFN   | s_DFN | biabl_DFN;
      typ [binode_DFN]  = POINTER_DFN   | s_DFN | binode_DFN;
      typ [type_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat    = bebux_pnt->NEXT;
      pnt [name_DFN].dat    = bebux_pnt->NAME;
      pnt [biabl_DFN].dat   = bebux_pnt->BIABL;
      pnt [binode_DFN].dat  = bebux_pnt->BINODE;
      pnt [type_DFN].imd    = bebux_pnt->TYPE;

      break;

	/* ###------------------------------------------------------### */
	/*    bepor							*/
	/* ###------------------------------------------------------### */

    case bepor_DFN :

      bepor_pnt = (struct bepor *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | bepor_DFN;
      typ [name_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [direction_DFN] = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [type_DFN]      = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat      = bepor_pnt->NEXT;
      pnt [name_DFN].dat      = bepor_pnt->NAME;
      pnt [direction_DFN].imd = bepor_pnt->DIRECTION;
      pnt [type_DFN].imd      = bepor_pnt->TYPE;

      break;

	/* ###------------------------------------------------------### */
	/*    biabl							*/
	/* ###------------------------------------------------------### */

    case biabl_DFN :

      biabl_pnt = (struct biabl *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN   | biabl_DFN    ;
      typ [label_DFN]     = IMMEDIATE_DFN | s_DFN   | string_DFN   ;
      typ [time_DFN]      = IMMEDIATE_DFN | l_DFN   | long_DFN     ;
      typ [cndabl_DFN]    = IMMEDIATE_DFN | ABL_DFN | abl_DFN      ;
      typ [valabl_DFN]    = IMMEDIATE_DFN | ABL_DFN | abl_DFN      ;
      typ [flag_DFN]      = IMMEDIATE_DFN | x_DFN   | character_DFN;
      typ [user_DFN]      = POINTER_DFN   | s_DFN   | ptype_DFN    ;

      pnt [next_DFN].dat   = biabl_pnt->NEXT  ;
      pnt [label_DFN].dat  = biabl_pnt->LABEL ;
      pnt [time_DFN].imd   = biabl_pnt->TIME  ;
      pnt [cndabl_DFN].dat = biabl_pnt->CNDABL;
      pnt [valabl_DFN].dat = biabl_pnt->VALABL;
      pnt [flag_DFN].imd   = biabl_pnt->FLAG  ;
      pnt [user_DFN].dat   = biabl_pnt->USER  ;

      break;

	/* ###------------------------------------------------------### */
	/*    binode							*/
	/* ###------------------------------------------------------### */

    case binode_DFN :

      binode_pnt = (struct binode *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN   | binode_DFN   ;
      typ [time_DFN]      = IMMEDIATE_DFN | l_DFN   | long_DFN     ;
      typ [flag_DFN]      = IMMEDIATE_DFN | x_DFN   | character_DFN;
      typ [cndnode_DFN]   = POINTER_DFN   | s_DFN   | void_DFN     ;
      typ [valnode_DFN]   = POINTER_DFN   | s_DFN   | void_DFN     ;

      pnt [next_DFN].dat      = binode_pnt->NEXT   ;
      pnt [time_DFN].imd      = binode_pnt->TIME   ;
      pnt [flag_DFN].imd      = binode_pnt->FLAG   ;
      pnt [cndnode_DFN].dat   = binode_pnt->CNDNODE;
      pnt [valnode_DFN].dat   = binode_pnt->VALNODE;

      break;

	/* ###------------------------------------------------------### */
	/*    berin							*/
	/* ###------------------------------------------------------### */

    case berin_DFN :

      berin_pnt = (struct berin *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN | berin_DFN;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [out_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [bus_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [aux_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [reg_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [bux_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [msg_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [dly_ref_DFN] = POINTER_DFN   | s_DFN | chain_DFN;

      pnt [next_DFN].dat    = berin_pnt->NEXT;
      pnt [name_DFN].dat    = berin_pnt->NAME;
      pnt [out_ref_DFN].dat = berin_pnt->OUT_REF;
      pnt [bus_ref_DFN].dat = berin_pnt->BUS_REF;
      pnt [aux_ref_DFN].dat = berin_pnt->AUX_REF;
      pnt [reg_ref_DFN].dat = berin_pnt->REG_REF;
      pnt [bux_ref_DFN].dat = berin_pnt->BUX_REF;
      pnt [msg_ref_DFN].dat = berin_pnt->MSG_REF;
      pnt [dly_ref_DFN].dat = berin_pnt->DLY_REF;

      break;

	/* ###------------------------------------------------------### */
	/*    begen							*/
	/* ###------------------------------------------------------### */

    case begen_DFN :

      begen_pnt = (struct begen *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN | begen_DFN;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [type_DFN]    = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [value_DFN]   = POINTER_DFN   | s_DFN | void_DFN;

      pnt [next_DFN].dat    = begen_pnt->NEXT;
      pnt [name_DFN].dat    = begen_pnt->NAME;
      pnt [type_DFN].dat    = begen_pnt->TYPE;
      pnt [value_DFN].dat   = begen_pnt->VALUE;

      break;

	/* ###------------------------------------------------------### */
	/*    bequad							*/
	/* ###------------------------------------------------------### */

    case bequad_DFN :

      bequad_pnt = (struct bequad *) curpnt.data;

      typ [bdd_DFN]      = IMMEDIATE_DFN | BDD_DFN | bdd_DFN;
      typ [gex_DFN]      = IMMEDIATE_DFN | GEX_DFN | gex_DFN;
      typ [varlist_DFN]  = POINTER_DFN   | s_DFN   | chain_DFN;
      typ [derivate_DFN] = POINTER_DFN   | s_DFN   | beder_DFN;
      typ [user_DFN]     = POINTER_DFN   | s_DFN   | void_DFN;

      pnt [bdd_DFN].dat      = bequad_pnt->BDD     ;
      pnt [gex_DFN].dat      = bequad_pnt->GEX     ;
      pnt [varlist_DFN].dat  = bequad_pnt->VARLIST ;
      pnt [derivate_DFN].dat = bequad_pnt->DERIVATE;
      pnt [user_DFN].dat     = bequad_pnt->USER    ;

      break;

	/* ###------------------------------------------------------### */
	/*    beder							*/
	/* ###------------------------------------------------------### */

    case beder_DFN :

      beder_pnt = (struct beder *) curpnt.data;

      typ [next_DFN]    = POINTER_DFN   | s_DFN   | beder_DFN;
      typ [bdd_DFN]     = IMMEDIATE_DFN | BDD_DFN | bdd_DFN  ;
      typ [index_DFN]   = IMMEDIATE_DFN | u_DFN   | short_DFN;

      pnt [next_DFN].dat    = beder_pnt->NEXT ;
      pnt [bdd_DFN].dat     = beder_pnt->BDD  ;
      pnt [index_DFN].imd   = beder_pnt->INDEX;

      break;

	/* ###------------------------------------------------------### */
	/*    ptype							*/
	/* ###------------------------------------------------------### */

    case ptype_DFN :

      ptype_pnt = (struct ptype *) curpnt.data;

      typ [next_DFN] = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [data_DFN] = POINTER_DFN   | s_DFN | void_DFN;
      typ [type_DFN] = IMMEDIATE_DFN | l_DFN | long_DFN;

      pnt [next_DFN].dat = ptype_pnt->NEXT;
      pnt [data_DFN].dat = ptype_pnt->DATA;
      pnt [type_DFN].imd = ptype_pnt->TYPE;

      break;

	/* ###------------------------------------------------------### */
	/*    chain							*/
	/* ###------------------------------------------------------### */

    case chain_DFN :

      chain_pnt = (struct chain *) curpnt.data;

      typ [next_DFN] = POINTER_DFN | s_DFN | chain_DFN;
      typ [data_DFN] = POINTER_DFN | s_DFN | void_DFN;

      pnt [next_DFN].dat = chain_pnt->NEXT;
      pnt [data_DFN].dat = chain_pnt->DATA;

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
	/*    integer							*/
	/* ###------------------------------------------------------### */

    case integer_DFN :

      int_pnt = (int *) curpnt.data;

      typ [integer_DFN]     = IMMEDIATE_DFN | d_DFN | integer_DFN;
      pnt [integer_DFN].imd = *int_pnt;

      break;

	/* ###------------------------------------------------------### */
	/*    character							*/
	/* ###------------------------------------------------------### */

    case character_DFN :

      character_pnt = (char *) curpnt.data;

      typ [character_DFN]     = IMMEDIATE_DFN | x_DFN | character_DFN;
      pnt [character_DFN].imd = *character_pnt;

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

  for (i=0 ; i<MAXCMD_DFN ; i++)
    {
    if ((typ [i] & KIND_DFN) == IMMEDIATE_DFN)
      {
      printf ("   %-15s : ", str [i]);
      switch (typ [i] & FORMAT_DFN)
        {
        case s_DFN :
          if (pnt[i].dat != NULL)
            printf ("%s", pnt[i].dat);
          printf ("\n");
          break;

        case u_DFN :
          printf ("%u\n", pnt[i].imd);
          break;

        case c_DFN :
          printf ("'%c'\n", pnt[i].imd);
          break;

        case d_DFN :
          printf ("%d\n", pnt[i].imd);
          break;

        case l_DFN :
          printf ("%ld\n", pnt[i].imd);
          break;

        case x_DFN :
          printf ("0x%x\n", pnt[i].imd);
          break;

        case ABL_DFN :
          if (pnt[i].dat != NULL)
            {
            buffer [0] = '\0';
            buffer = exprToCharInt (pnt[i].dat, 1, buffer, &buff_size);
            printf ("%s", buffer);
            }
          printf ("\n");
          break;

        case BDD_DFN :
          if ((pnt[i].dat != NULL) && (circuit_pnt != NULL))
            {
            buffer [0] = '\0';
            ptr_abl = bddToAbl (pnt[i].dat, circuit_pnt->pNameI);
            buffer  = exprToCharInt (ptr_abl, 1, buffer, &buff_size);
            printf ("%s", buffer);
            }
          printf ("\n");
          break;

        case GEX_DFN :
          if (pnt[i].dat != NULL)
            printf ("Gex displayer not yet available\n");
          break;
        }
      }

    if ((typ [i] & KIND_DFN) == POINTER_DFN)
      {
      printf ("-> %-15s : ", str [i]);
      if (pnt[i].dat != NULL)
        printf ("available");
      printf ("\n");
      }

    if ( ((typ [i] & KIND_DFN) == ARRAY_DFN   ) ||
         ((typ [i] & KIND_DFN) == ARYOFPNT_DFN) )
      {
      printf ("=> %-15s : ", str [i]);
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
    fprintf (stdout, "beh_debug :\tSTACK OVERFLOW !!\tSTACK OVERFLOW !!\n");
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
      fprintf (stderr ,"cannot push NULL pointer on the stack\n");
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
/* function	: goforward						*/
/* description	: go forward through the NEXT field to find the Nth	*/
/*		  structure in the list and return it			*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static struct chain *goforward (pnt, count)

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

char **words;				/* list of words		*/
int    wrdcnt;				/* number of words		*/
char **strgs;				/* list of recognized strings	*/
int   *nmbrs;				/* words translated to numbers	*/
char  *flags;				/* set if word is a number	*/
int   *indxs;				/* words' index in strgs table	*/

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
	/*    for each word :						*/
	/*      - check if it is a number				*/
	/*      - try to locate the word among the recognized strings.	*/
	/*        If found register it's index				*/
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

static void getsize (siz)

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

  siz [befig_DFN]     = sizeof (struct befig);
  siz [bereg_DFN]     = sizeof (struct bereg);
  siz [bemsg_DFN]     = sizeof (struct bemsg);
  siz [berin_DFN]     = sizeof (struct berin);
  siz [beout_DFN]     = sizeof (struct beout);
  siz [bebus_DFN]     = sizeof (struct bebus);
  siz [beaux_DFN]     = sizeof (struct beaux);
  siz [bedly_DFN]     = sizeof (struct beaux);
  siz [bebux_DFN]     = sizeof (struct bebux);
  siz [bepor_DFN]     = sizeof (struct bepor);
  siz [begen_DFN]     = sizeof (struct begen);
  siz [biabl_DFN]     = sizeof (struct biabl);
  siz [binode_DFN]    = sizeof (struct binode);
  siz [bequad_DFN]    = sizeof (struct bequad);
  siz [beder_DFN]     = sizeof (struct beder);
  }

/* ###--------------------------------------------------------------### */
/* function	: disp_immd						*/
/* description	: display a specific field as an immediate		*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

static void disp_immd (str, pnt, type)

char        **str;
union value pnt  ;
int         type ;

  {
  extern char  *exprToCharInt ();
  char         *lcl_str         ;
  struct chain *ptr_abl         ;

  printf ("   %-15s: ", str [type]);
  switch (type)
    {
    case string_DFN :
      if (((char *) pnt.dat) != NULL)
        printf ("%s", (char *) pnt.dat);
      printf ("\n");
      break;

    case character_DFN :
      printf ("%c\n", pnt.imd);
      break;

    case short_DFN     :
    case integer_DFN   :
    case long_DFN      :
      printf ("0x%x\n", pnt.imd);
      break;

    case abl_DFN :
      if (pnt.dat != NULL)
        {
        buffer [0] = '\0';
        buffer = exprToCharInt (pnt.dat, 1, buffer, &buff_size);
        printf ("%s", buffer);
        }
      printf ("\n");
      break;

    case bdd_DFN :
      if ((pnt.dat != NULL) && (circuit_pnt != NULL))
        {
        buffer [0] = '\0';
        ptr_abl = bddToAbl (pnt.dat, circuit_pnt->pNameI);
        buffer  = exprToCharInt (ptr_abl, 1, buffer, &buff_size);
        printf ("%s", buffer);
        }
      printf ("\n");
      break;

    case gex_DFN :
      if (pnt.dat != NULL)
        printf ("Gex displayer not yet available\n");
      break;

    default :
      fprintf (stderr ,"unrecognized command\n");
    }
  }
