
/* ###--------------------------------------------------------------### */
/* file		: vh_debug.c						*/
/* date		: Aug 20 1997						*/
/* version	: v3.0							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* content	: high level function					*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include "mut.h"
#include "mlo.h"
#include "log.h"
#include "beh.h"
#include "pat.h"
#include "sch.h"
#include "vh_ltype.h"
#include "vh_debug.h"

/* ###--------------------------------------------------------------### */
/* function	: dast_dbg						*/
/* description	: display any structure					*/
/* called func.	: vhu_error, vhu_message				*/
/* ###--------------------------------------------------------------### */

void dast_dbg (head_pnt, type)

void *head_pnt;				/* structure's pointer		*/
char *type;				/* structure's type		*/

  {
  static int           translate  ();
  static int           splitline  ();
  static void          disp_immd  ();
  static void          get_size   ();
  static void          read_field ();
  static void          push       ();
  static void          pop        ();
  static struct chain *go_forward ();

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
                         "_back"     , "_exit"     , "_jump"     , "_save"     ,
                         "_stop"     , "_top"      , "_up"       , "_display"  ,

                         "character" , "short"     , "integer"   , "long"      ,
                         "float"     , "void"      , "string"    ,

                         "chain"     , "ptype"     ,

                         "lofig"     , "locon"     , "losig"     , "loins"     ,

                         "befig"     , "bereg"     , "bemsg"     , "berin"     ,
                         "beout"     , "bebus"     , "beaux"     , "bedly"     ,
                         "bebux"     , "bepor"     , "begen"     , "biabl"     ,
                         "binode"    ,

                         "paseq"     , "pagrp"     , "paiol"     , "papat"     ,
                         "pacom"     , "paini"     , "paevt"     , "pains"     ,

                         "bussig"    , "lkdfig"    , "lkdins"    , "simsig"    ,
                         "wrbsig"    , "wriaux"    , "wribux"    , "wrireg"    ,
                         "wrssig"    , "wrimsg"    ,

                         "shdul"     , "shent"     , "shtra"     , "shwav"     ,

                         "abl"       , "bdd"       , "gex"       ,

                         "actflag"   , "aux_ref"   , "blank"     , "buffer"    ,
                         "bus_ref"   , "bux_ref"   , "capa"      , "cndabl"    ,
                         "cndnode"   , "cndval"    , "cond"      , "curcom"    ,
                         "curpat"    , "curtime"   , "curval"    , "data"      ,
                         "date"      , "deccom"    , "depend"    , "direction" ,
                         "dly_ref"   , "drvseq"    , "drvval"    , "endflg"    ,
                         "errflg"    , "eval"      , "figname"   , "filname"   ,
                         "findex"    , "flag"      , "flags"     , "format"    ,
                         "glnext"    , "ident"     , "index"     , "insname"   ,
                         "instance"  , "iolnbr"    , "itable"    , "label"     ,
                         "length"    , "level"     , "line"      , "lineno"    ,
                         "message"   , "mode"      , "model"     , "modelchain",
                         "msg_ref"   , "name"      , "namechain" , "next"      ,
                         "node"      , "nxtpat"    , "oldcom"    , "oldpat"    ,
                         "out_ref"   , "patnbr"    , "position"  , "prev"      ,
                         "prjbvl"    , "prjrvl"    , "prjval"    , "redins"    ,
                         "redlst"    , "reg_ref"   , "root"      , "savflg"    ,
                         "scdid"     , "sig"       , "simflag"   , "simval"    ,
                         "subseq"    , "text"      , "time"      , "time_unit" ,
                         "trans"     , "ttable"    , "type"      , "uflag"     ,
                         "user"      , "usrval"    , "valabl"    , "valnode"   ,
                         "valsiz"    , "valu"      , "value"     , "wave" 
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
      vhu_error (118, NULL, NULL, 0);
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
          vhu_error (119, NULL, NULL, 0);
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
              vhu_message (19, "vhu_debug", 0);
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
                vhu_error (118, NULL, NULL, 0);
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
                  vhu_error (118, NULL, NULL, 0);
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
                vhu_error (118, NULL, NULL, 0);
              break;

	/* ###------------------------------------------------------### */
	/*    _display COMMAND : display a specified field as an	*/
	/* immediate							*/
	/* ###------------------------------------------------------### */

            case _display_DFN :
              if ((wrdcnt == 3) && (indxs [1] != _error_DFN))
                disp_immd (str, pnt [indxs [1]], indxs [2]);
              else
                vhu_error (118, NULL, NULL, 0);
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
                vhu_error (118, NULL, NULL, 0);
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
            vhu_error (118, NULL, NULL, 0);
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
            vhu_error (118, NULL, NULL, 0);
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
    if (strcmp (line ,"."))
      {
      wrdcnt = splitline (words, line);
      idx    = translate (words, wrdcnt, str, nmbrs, flags, indxs);
      }
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
  extern char         *exprToCharInt ();
  extern struct chain *bddToAbl ();

  int                    i;
  struct chain	        *ptr_abl;
  static char           *buffer      = NULL;
  static unsigned int    buffer_size = 0;
  static struct circuit *circuit_pnt = NULL;

  struct chain   *chain_pnt;
  struct ptype   *ptype_pnt;

  struct lofig   *lofig_pnt;
  struct locon   *locon_pnt;
  struct losig   *losig_pnt;
  struct loins   *loins_pnt;

  struct befig   *befig_pnt;
  struct bereg   *bereg_pnt;
  struct bemsg   *bemsg_pnt;
  struct berin   *berin_pnt;
  struct beout   *beout_pnt;
  struct bebus   *bebus_pnt;
  struct beaux   *beaux_pnt;
  struct bebux   *bebux_pnt;
  struct bepor   *bepor_pnt;
  struct begen   *begen_pnt;
  struct biabl   *biabl_pnt;
  struct binode  *binode_pnt;

  struct paseq   *paseq_pnt;
  struct pagrp   *pagrp_pnt;
  struct paiol   *paiol_pnt;
  struct papat   *papat_pnt;
  struct pacom   *pacom_pnt;
  struct paini   *paini_pnt;
  struct paevt   *paevt_pnt;
  struct pains   *pains_pnt;

  struct lkdfig  *lkdfig_pnt;
  struct lkdins  *lkdins_pnt;
  struct simsig  *simsig_pnt;
  struct bussig  *bussig_pnt;
  struct wrbsig  *wrbsig_pnt;
  struct wriaux  *wriaux_pnt;
  struct wrireg  *wrireg_pnt;
  struct wribux  *wribux_pnt;
  struct wrssig  *wrssig_pnt;
  struct wrimsg  *wrimsg_pnt;
  struct redlst  *redlst_pnt;
  struct prjbvl  *prjbvl_pnt;
  struct prjrvl  *prjrvl_pnt;

  struct shdul   *shdul_pnt ;
  struct shent   *shent_pnt ;
  struct shtra   *shtra_pnt ;
  struct shwav   *shwav_pnt ;

  char          **string_pnt;
  char           *character_pnt;
  int            *integer_pnt;

	/* ###------------------------------------------------------### */
	/*    initialization						*/
	/* ###------------------------------------------------------### */

  for (i=0 ; i<MAXCMD_DFN ; i++)
    {
    typ [i]     = _error_DFN;
    pnt [i].dat = NULL;
    }

  if (buffer == NULL)
    {
    buffer      = (char *) mbkalloc (1024);
    buffer_size = 1024;
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
	/*    lofig							*/
	/* ###------------------------------------------------------### */

    case lofig_DFN :

      lofig_pnt = (struct lofig *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | lofig_DFN;
      typ [modelchain_DFN]       = POINTER_DFN   | s_DFN | chain_DFN;
      typ [locon_DFN]            = POINTER_DFN   | s_DFN | locon_DFN;
      typ [losig_DFN]            = POINTER_DFN   | s_DFN | losig_DFN;
      typ [loins_DFN]            = POINTER_DFN   | s_DFN | loins_DFN;
      typ [name_DFN]             = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [mode_DFN]             = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [user_DFN]             = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [next_DFN]             = POINTER_DFN   | s_DFN | lofig_DFN;

      pnt [modelchain_DFN].dat = lofig_pnt->MODELCHAIN;
      pnt [locon_DFN].dat      = lofig_pnt->LOCON;
      pnt [losig_DFN].dat      = lofig_pnt->LOSIG;
      pnt [loins_DFN].dat      = lofig_pnt->LOINS;
      pnt [name_DFN].dat       = lofig_pnt->NAME;
      pnt [mode_DFN].imd       = lofig_pnt->MODE;
      pnt [user_DFN].dat       = lofig_pnt->USER;

      break;

	/* ###------------------------------------------------------### */
	/*    locon							*/
	/* ###------------------------------------------------------### */

    case locon_DFN :

      locon_pnt = (struct locon *) curpnt.data;

      typ [next_DFN]          = POINTER_DFN   | s_DFN | locon_DFN;
      typ [name_DFN]          = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [sig_DFN]           = POINTER_DFN   | s_DFN | losig_DFN;
      typ [root_DFN]          = POINTER_DFN   | s_DFN | void_DFN;
      typ [direction_DFN]     = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [type_DFN]          = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [user_DFN]          = POINTER_DFN   | s_DFN | ptype_DFN;

      pnt [next_DFN].dat      = locon_pnt->NEXT;
      pnt [name_DFN].dat      = locon_pnt->NAME;
      pnt [sig_DFN].dat       = locon_pnt->SIG;
      pnt [root_DFN].dat      = locon_pnt->ROOT;
      pnt [direction_DFN].imd = locon_pnt->DIRECTION;
      pnt [type_DFN].imd      = locon_pnt->TYPE;
      pnt [user_DFN].dat      = locon_pnt->USER;

      break;

	/* ###------------------------------------------------------### */
	/*    losig							*/
	/* ###------------------------------------------------------### */

    case losig_DFN :

      losig_pnt = (struct losig *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | losig_DFN;
      typ [namechain_DFN]        = POINTER_DFN   | s_DFN | chain_DFN;
      typ [capa_DFN]             = IMMEDIATE_DFN | f_DFN | float_DFN;
      typ [index_DFN]            = IMMEDIATE_DFN | l_DFN | long_DFN;
      typ [type_DFN]             = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [user_DFN]             = POINTER_DFN   | s_DFN | ptype_DFN;

      pnt [next_DFN].dat      = losig_pnt->NEXT;
      pnt [namechain_DFN].dat = losig_pnt->NAMECHAIN;
      pnt [index_DFN].imd     = losig_pnt->INDEX;
      pnt [type_DFN].imd      = losig_pnt->TYPE;
      pnt [user_DFN].dat      = losig_pnt->USER;

      break;

	/* ###------------------------------------------------------### */
	/*    loins							*/
	/* ###------------------------------------------------------### */

    case loins_DFN :

      loins_pnt = (struct loins *) curpnt.data;

      typ [next_DFN]         = POINTER_DFN   | s_DFN | loins_DFN;
      typ [locon_DFN]        = POINTER_DFN   | s_DFN | locon_DFN;
      typ [insname_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [figname_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [user_DFN]         = POINTER_DFN   | s_DFN | ptype_DFN;

      pnt [next_DFN].dat     = loins_pnt->NEXT;
      pnt [locon_DFN].dat    = loins_pnt->LOCON;
      pnt [insname_DFN].dat  = loins_pnt->INSNAME;
      pnt [figname_DFN].dat  = loins_pnt->FIGNAME;
      pnt [user_DFN].dat     = loins_pnt->USER;

      break;

	/* ###------------------------------------------------------### */
	/*    befig							*/
	/* ###------------------------------------------------------### */

    case befig_DFN :

      befig_pnt = (struct befig *) curpnt.data;

      typ [next_DFN]   = POINTER_DFN   | s_DFN | befig_DFN;
      typ [name_DFN]   = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [bereg_DFN]  = POINTER_DFN   | s_DFN | bereg_DFN;
      typ [bemsg_DFN]  = POINTER_DFN   | s_DFN | bemsg_DFN;
      typ [berin_DFN]  = POINTER_DFN   | s_DFN | berin_DFN;
      typ [beout_DFN]  = POINTER_DFN   | s_DFN | beout_DFN;
      typ [bebus_DFN]  = POINTER_DFN   | s_DFN | bebus_DFN;
      typ [beaux_DFN]  = POINTER_DFN   | s_DFN | beaux_DFN;
      typ [bedly_DFN]  = POINTER_DFN   | s_DFN | beaux_DFN;
      typ [bebux_DFN]  = POINTER_DFN   | s_DFN | bebux_DFN;
      typ [bepor_DFN]  = POINTER_DFN   | s_DFN | bepor_DFN;
      typ [begen_DFN]  = POINTER_DFN   | s_DFN | begen_DFN;
      typ [user_DFN]   = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [errflg_DFN] = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [type_DFN]   = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [flag_DFN]   = IMMEDIATE_DFN | x_DFN | character_DFN;

      pnt [next_DFN].dat   = befig_pnt->NEXT;
      pnt [name_DFN].dat   = befig_pnt->NAME;
      pnt [bereg_DFN].dat  = befig_pnt->BEREG;
      pnt [bemsg_DFN].dat  = befig_pnt->BEMSG;
      pnt [berin_DFN].dat  = befig_pnt->BERIN;
      pnt [beout_DFN].dat  = befig_pnt->BEOUT;
      pnt [bebus_DFN].dat  = befig_pnt->BEBUS;
      pnt [beaux_DFN].dat  = befig_pnt->BEAUX;
      pnt [bedly_DFN].dat  = befig_pnt->BEDLY;
      pnt [bebux_DFN].dat  = befig_pnt->BEBUX;
      pnt [bepor_DFN].dat  = befig_pnt->BEPOR;
      pnt [begen_DFN].dat  = befig_pnt->BEGEN;
      pnt [user_DFN].dat   = befig_pnt->USER;
      pnt [errflg_DFN].imd = befig_pnt->ERRFLG;
      pnt [type_DFN].imd   = befig_pnt->TYPE;
      pnt [flag_DFN].imd   = befig_pnt->FLAG;

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
      typ [node_DFN]    = IMMEDIATE_DFN | BDD_DFN | node_DFN;

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

      typ [next_DFN]    = POINTER_DFN   | s_DFN   | beout_DFN;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN   | string_DFN;
      typ [abl_DFN]     = IMMEDIATE_DFN | ABL_DFN | abl_DFN;
      typ [node_DFN]    = IMMEDIATE_DFN | BDD_DFN | node_DFN;
      typ [time_DFN]    = IMMEDIATE_DFN |   l_DFN | long_DFN;

      pnt [next_DFN].dat    = beout_pnt->NEXT;
      pnt [name_DFN].dat    = beout_pnt->NAME;
      pnt [abl_DFN].dat     = beout_pnt->ABL;
      pnt [node_DFN].dat    = beout_pnt->NODE;
      pnt [time_DFN].imd    = beout_pnt->TIME;

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

      typ [next_DFN]    = POINTER_DFN   | s_DFN   | beaux_DFN;
      typ [name_DFN]    = IMMEDIATE_DFN | s_DFN   | string_DFN;
      typ [abl_DFN]     = IMMEDIATE_DFN | ABL_DFN | abl_DFN;
      typ [node_DFN]    = IMMEDIATE_DFN | BDD_DFN | node_DFN;
      typ [time_DFN]    = IMMEDIATE_DFN |   l_DFN | long_DFN;

      pnt [next_DFN].dat    = beaux_pnt->NEXT;
      pnt [name_DFN].dat    = beaux_pnt->NAME;
      pnt [abl_DFN].dat     = beaux_pnt->ABL;
      pnt [node_DFN].dat    = beaux_pnt->NODE;
      pnt [time_DFN].imd    = beaux_pnt->TIME;

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

      typ [next_DFN]      = POINTER_DFN   | s_DFN   | biabl_DFN;
      typ [label_DFN]     = IMMEDIATE_DFN | s_DFN   | string_DFN;
      typ [cndabl_DFN]    = IMMEDIATE_DFN | ABL_DFN | abl_DFN;
      typ [valabl_DFN]    = IMMEDIATE_DFN | ABL_DFN | abl_DFN;
      typ [user_DFN]      = POINTER_DFN   | s_DFN   | ptype_DFN;
      typ [flag_DFN]      = IMMEDIATE_DFN | x_DFN   | character_DFN;
      typ [time_DFN]      = IMMEDIATE_DFN | l_DFN   | long_DFN;

      pnt [next_DFN].dat    = biabl_pnt->NEXT;
      pnt [label_DFN].dat   = biabl_pnt->LABEL;
      pnt [cndabl_DFN].dat  = biabl_pnt->CNDABL;
      pnt [valabl_DFN].dat  = biabl_pnt->VALABL;
      pnt [user_DFN].dat    = biabl_pnt->USER;
      pnt [flag_DFN].imd    = biabl_pnt->FLAG;
      pnt [time_DFN].imd    = biabl_pnt->TIME;

      break;

	/* ###------------------------------------------------------### */
	/*    binode							*/
	/* ###------------------------------------------------------### */

    case binode_DFN :

      binode_pnt = (struct binode *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN   | binode_DFN;
      typ [cndnode_DFN]   = IMMEDIATE_DFN | BDD_DFN | node_DFN;
      typ [valnode_DFN]   = IMMEDIATE_DFN | BDD_DFN | node_DFN;
      typ [time_DFN]      = IMMEDIATE_DFN | l_DFN   | long_DFN;

      pnt [next_DFN].dat    = binode_pnt->NEXT;
      pnt [cndnode_DFN].dat = binode_pnt->CNDNODE;
      pnt [valnode_DFN].dat = binode_pnt->VALNODE;
      pnt [time_DFN].imd    = binode_pnt->TIME;

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
      typ [time_unit_DFN]  = IMMEDIATE_DFN | x_DFN | character_DFN;

      pnt [next_DFN].dat      = paseq_pnt->NEXT;
      pnt [name_DFN].dat      = paseq_pnt->NAME;
      pnt [pagrp_DFN].dat     = paseq_pnt->PAGRP;
      pnt [paiol_DFN].dat     = paseq_pnt->PAIOL;
      pnt [deccom_DFN].dat    = paseq_pnt->DECCOM;
      pnt [oldpat_DFN].dat    = paseq_pnt->OLDPAT;
      pnt [oldcom_DFN].dat    = paseq_pnt->OLDCOM;
      pnt [curpat_DFN].dat    = paseq_pnt->CURPAT;
      pnt [curcom_DFN].dat    = paseq_pnt->CURCOM;
      pnt [nxtpat_DFN].dat    = paseq_pnt->NXTPAT;
      pnt [model_DFN].dat     = paseq_pnt->MODEL;
      pnt [buffer_DFN].dat    = paseq_pnt->BUFFER;
      pnt [pains_DFN].dat     = paseq_pnt->PAINS;
      pnt [patnbr_DFN].imd    = paseq_pnt->PATNBR;
      pnt [lineno_DFN].imd    = paseq_pnt->LINENO;
      pnt [iolnbr_DFN].imd    = paseq_pnt->IOLNBR;
      pnt [subseq_DFN].imd    = paseq_pnt->SUBSEQ;
      pnt [drvseq_DFN].imd    = paseq_pnt->DRVSEQ;
      pnt [savflg_DFN].imd    = paseq_pnt->SAVFLG;
      pnt [endflg_DFN].imd    = paseq_pnt->ENDFLG;
      pnt [errflg_DFN].imd    = paseq_pnt->ERRFLG;
      pnt [time_unit_DFN].imd = paseq_pnt->TIME_UNIT;

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
/*----------
      typ [value_DFN]     = ARYOFPNT_DFN  | s_DFN | character_DFN;
------------*/

      pnt [next_DFN].dat     = pains_pnt->NEXT;
      pnt [insname_DFN].dat  = pains_pnt->INSNAME;
      pnt [filname_DFN].dat  = pains_pnt->FILNAME;
      pnt [instance_DFN].dat = pains_pnt->INSTANCE;
      pnt [model_DFN].dat    = pains_pnt->MODEL;
/*----------
      pnt [value_DFN].dat    = pains_pnt->VALUE;
------------*/

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

      pnt [next_DFN].dat    = paiol_pnt->NEXT;
      pnt [name_DFN].dat    = paiol_pnt->NAME;
      pnt [format_DFN].imd  = paiol_pnt->FORMAT;
      pnt [mode_DFN].imd    = paiol_pnt->MODE;
      pnt [blank_DFN].imd   = paiol_pnt->BLANK;
      pnt [value_DFN].imd   = paiol_pnt->VALUE;
      pnt [sig_DFN].dat     = paiol_pnt->SIG;

      break;

	/* ###------------------------------------------------------### */
	/*    papat							*/
	/* ###------------------------------------------------------### */

    case papat_DFN :

      papat_pnt = (struct papat *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | papat_DFN;
      typ [label_DFN]     = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [paini_DFN]     = POINTER_DFN   | s_DFN | paini_DFN;
      typ [paevt_DFN]     = POINTER_DFN   | s_DFN | paevt_DFN;
      typ [line_DFN]      = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [blank_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;
      typ [actflag_DFN]   = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [simflag_DFN]   = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat    = papat_pnt->NEXT;
      pnt [label_DFN].dat   = papat_pnt->LABEL;
      pnt [paini_DFN].dat   = papat_pnt->PAINI;
      pnt [paevt_DFN].dat   = papat_pnt->PAEVT;
      pnt [line_DFN].imd    = papat_pnt->LINE;
      pnt [blank_DFN].imd   = papat_pnt->BLANK;
      pnt [actflag_DFN].imd = papat_pnt->ACTFLAG;
      pnt [simflag_DFN].imd = papat_pnt->SIMFLAG;

      break;

	/* ###------------------------------------------------------### */
	/*    paini							*/
	/* ###------------------------------------------------------### */

    case paini_DFN :

      paini_pnt = (struct paini *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | paini_DFN;
      typ [name_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [value_DFN]     = IMMEDIATE_DFN | x_DFN | character_DFN;

      pnt [next_DFN].dat  = paini_pnt->NEXT;
      pnt [name_DFN].dat  = paini_pnt->NAME;
      pnt [value_DFN].imd = paini_pnt->VALUE;

      break;

	/* ###------------------------------------------------------### */
	/*    paevt							*/
	/* ###------------------------------------------------------### */

    case paevt_DFN :

      paevt_pnt = (struct paevt *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | paevt_DFN;
      typ [index_DFN]     = IMMEDIATE_DFN | u_DFN | short_DFN;
      typ [usrval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [simval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;

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
	/*    lkdfig							*/
	/* ###------------------------------------------------------### */

    case lkdfig_DFN :

      lkdfig_pnt = (struct lkdfig *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | lkdfig_DFN;
      typ [name_DFN]      = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [lkdins_DFN]    = POINTER_DFN   | s_DFN | lkdins_DFN;
      typ [simsig_DFN]    = POINTER_DFN   | s_DFN | simsig_DFN;
      typ [bussig_DFN]    = POINTER_DFN   | s_DFN | bussig_DFN;
      typ [valsiz_DFN]    = IMMEDIATE_DFN | u_DFN | integer_DFN;

      pnt [next_DFN].dat   = lkdfig_pnt->NEXT;
      pnt [name_DFN].dat   = lkdfig_pnt->NAME;
      pnt [lkdins_DFN].dat = lkdfig_pnt->LKDINS;
      pnt [simsig_DFN].dat = lkdfig_pnt->SIMSIG;
      pnt [bussig_DFN].dat = lkdfig_pnt->BUSSIG;
      pnt [valsiz_DFN].imd = lkdfig_pnt->VALSIZ;


      break;

	/* ###------------------------------------------------------### */
	/*    lkdins							*/
	/* ###------------------------------------------------------### */

    case lkdins_DFN :

      lkdins_pnt = (struct lkdins *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | lkdins_DFN;
      typ [insname_DFN]   = IMMEDIATE_DFN | s_DFN | string_DFN;
      typ [befig_DFN]     = POINTER_DFN   | s_DFN | befig_DFN;
      typ [wrssig_DFN]    = POINTER_DFN   | s_DFN | wrssig_DFN;
      typ [wriaux_DFN]    = POINTER_DFN   | s_DFN | wriaux_DFN;
      typ [wribux_DFN]    = POINTER_DFN   | s_DFN | wribux_DFN;
      typ [wrbsig_DFN]    = POINTER_DFN   | s_DFN | wrbsig_DFN;
      typ [wrireg_DFN]    = POINTER_DFN   | s_DFN | wrireg_DFN;
      typ [wrimsg_DFN]    = POINTER_DFN   | s_DFN | wrimsg_DFN;
      typ [redlst_DFN]    = ARRAY_DFN     | s_DFN | redlst_DFN;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;

      pnt [next_DFN].dat    = lkdins_pnt->NEXT;
      pnt [insname_DFN].dat = lkdins_pnt->INSNAME;
      pnt [befig_DFN].dat   = lkdins_pnt->BEFIG;
      pnt [wrssig_DFN].dat  = lkdins_pnt->WRSSIG;
      pnt [wriaux_DFN].dat  = lkdins_pnt->WRIAUX;
      pnt [wribux_DFN].dat  = lkdins_pnt->WRIBUX;
      pnt [wrbsig_DFN].dat  = lkdins_pnt->WRBSIG;
      pnt [wrireg_DFN].dat  = lkdins_pnt->WRIREG;
      pnt [wrimsg_DFN].dat  = lkdins_pnt->WRIMSG;
      pnt [redlst_DFN].dat  = lkdins_pnt->REDLST;
      pnt [eval_DFN].imd    = lkdins_pnt->EVAL;

      break;

	/* ###------------------------------------------------------### */
	/*    simsig							*/
	/* ###------------------------------------------------------### */

    case simsig_DFN :

      simsig_pnt = (struct simsig *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | simsig_DFN;
      typ [namechain_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [depend_DFN]    = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [curval_DFN]    = POINTER_DFN   | c_DFN | character_DFN;
      typ [prjval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat      = simsig_pnt->NEXT;
      pnt [namechain_DFN].dat = simsig_pnt->NAMECHAIN;
      pnt [depend_DFN].dat    = simsig_pnt->DEPEND;
      pnt [curval_DFN].dat    = simsig_pnt->CURVAL;
      pnt [prjval_DFN].imd    = simsig_pnt->PRJVAL;

      break;

	/* ###------------------------------------------------------### */
	/*    bussig							*/
	/* ###------------------------------------------------------### */

    case bussig_DFN :

      bussig_pnt = (struct bussig *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | bussig_DFN;
      typ [namechain_DFN] = POINTER_DFN   | s_DFN | chain_DFN;
      typ [depend_DFN]    = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [curval_DFN]    = POINTER_DFN   | c_DFN | character_DFN;
      typ [prjbvl_DFN]    = POINTER_DFN   | s_DFN | prjbvl_DFN;
      typ [type_DFN]      = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat      = bussig_pnt->NEXT;
      pnt [namechain_DFN].dat = bussig_pnt->NAMECHAIN;
      pnt [depend_DFN].dat    = bussig_pnt->DEPEND;
      pnt [curval_DFN].dat    = bussig_pnt->CURVAL;
      pnt [prjbvl_DFN].dat    = bussig_pnt->PRJBVL;
      pnt [type_DFN].imd      = bussig_pnt->TYPE;

      break;

	/* ###------------------------------------------------------### */
	/*    wrbsig							*/
	/* ###------------------------------------------------------### */

    case wrbsig_DFN :

      wrbsig_pnt = (struct wrbsig *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | wrbsig_DFN   ;
      typ [bebus_DFN]     = POINTER_DFN   | s_DFN | bebus_DFN    ;
      typ [bussig_DFN]    = POINTER_DFN   | s_DFN | bussig_DFN   ;
      typ [prjbvl_DFN]    = POINTER_DFN   | s_DFN | prjbvl_DFN   ;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [level_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;

      pnt [next_DFN].dat   = wrbsig_pnt->NEXT  ;
      pnt [bebus_DFN].dat  = wrbsig_pnt->BEBUS ;
      pnt [bussig_DFN].dat = wrbsig_pnt->BUSSIG;
      pnt [prjbvl_DFN].dat = wrbsig_pnt->PRJBVL;
      pnt [eval_DFN].imd   = wrbsig_pnt->EVAL  ;
      pnt [level_DFN].imd  = wrbsig_pnt->LEVEL ;

      break;

	/* ###------------------------------------------------------### */
	/*    wriaux							*/
	/* ###------------------------------------------------------### */

    case wriaux_DFN :

      wriaux_pnt = (struct wriaux *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | wriaux_DFN   ;
      typ [lkdins_DFN]    = POINTER_DFN   | s_DFN | lkdins_DFN   ;
      typ [depend_DFN]    = POINTER_DFN   | s_DFN | ptype_DFN    ;
      typ [beaux_DFN]     = POINTER_DFN   | s_DFN | beaux_DFN    ;
      typ [curval_DFN]    = POINTER_DFN   | c_DFN | character_DFN;
      typ [prjval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [level_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;
      typ [flag_DFN]      = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat    = wriaux_pnt->NEXT  ;
      pnt [lkdins_DFN].dat  = wriaux_pnt->LKDINS;
      pnt [depend_DFN].dat  = wriaux_pnt->DEPEND;
      pnt [beaux_DFN].dat   = wriaux_pnt->BEAUX ;
      pnt [curval_DFN].dat  = wriaux_pnt->CURVAL;
      pnt [prjval_DFN].imd  = wriaux_pnt->PRJVAL;
      pnt [eval_DFN].imd    = wriaux_pnt->EVAL  ;
      pnt [level_DFN].imd   = wriaux_pnt->LEVEL ;
      pnt [flag_DFN].imd    = wriaux_pnt->FLAG  ;

      break;

	/* ###------------------------------------------------------### */
	/*    wrireg							*/
	/* ###------------------------------------------------------### */

    case wrireg_DFN :

      wrireg_pnt = (struct wrireg *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | wrireg_DFN   ;
      typ [lkdins_DFN]    = POINTER_DFN   | s_DFN | lkdins_DFN   ;
      typ [depend_DFN]    = POINTER_DFN   | s_DFN | ptype_DFN    ;
      typ [bereg_DFN]     = POINTER_DFN   | s_DFN | bereg_DFN    ;
      typ [curval_DFN]    = POINTER_DFN   | c_DFN | character_DFN;
      typ [prjrvl_DFN]    = POINTER_DFN   | s_DFN | prjrvl_DFN   ;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;

      pnt [next_DFN].dat    = wrireg_pnt->NEXT  ;
      pnt [lkdins_DFN].dat  = wrireg_pnt->LKDINS;
      pnt [depend_DFN].dat  = wrireg_pnt->DEPEND;
      pnt [bereg_DFN].dat   = wrireg_pnt->BEREG ;
      pnt [curval_DFN].dat  = wrireg_pnt->CURVAL;
      pnt [prjrvl_DFN].dat  = wrireg_pnt->PRJRVL;
      pnt [eval_DFN].imd    = wrireg_pnt->EVAL  ;

      break;

	/* ###------------------------------------------------------### */
	/*    wribux							*/
	/* ###------------------------------------------------------### */

    case wribux_DFN :

      wribux_pnt = (struct wribux *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | wribux_DFN   ;
      typ [lkdins_DFN]    = POINTER_DFN   | s_DFN | lkdins_DFN   ;
      typ [depend_DFN]    = POINTER_DFN   | s_DFN | ptype_DFN    ;
      typ [bebux_DFN]     = POINTER_DFN   | s_DFN | bebux_DFN    ;
      typ [curval_DFN]    = POINTER_DFN   | c_DFN | character_DFN;
      typ [prjbvl_DFN]    = POINTER_DFN   | s_DFN | prjbvl_DFN   ;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [level_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;

      pnt [next_DFN].dat    = wribux_pnt->NEXT  ;
      pnt [lkdins_DFN].dat  = wribux_pnt->LKDINS;
      pnt [depend_DFN].dat  = wribux_pnt->DEPEND;
      pnt [bebux_DFN].dat   = wribux_pnt->BEBUX ;
      pnt [curval_DFN].dat  = wribux_pnt->CURVAL;
      pnt [prjbvl_DFN].dat  = wribux_pnt->PRJBVL;
      pnt [eval_DFN].imd    = wribux_pnt->EVAL  ;
      pnt [level_DFN].imd   = wribux_pnt->LEVEL ;

      break;

	/* ###------------------------------------------------------### */
	/*    wrssig							*/
	/* ###------------------------------------------------------### */

    case wrssig_DFN :

      wrssig_pnt = (struct wrssig *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | wrssig_DFN   ;
      typ [beout_DFN]     = POINTER_DFN   | s_DFN | beout_DFN    ;
      typ [simsig_DFN]    = POINTER_DFN   | s_DFN | simsig_DFN   ;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [level_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;

      pnt [next_DFN].dat   = wrssig_pnt->NEXT  ;
      pnt [beout_DFN].dat  = wrssig_pnt->BEOUT ;
      pnt [simsig_DFN].dat = wrssig_pnt->SIMSIG;
      pnt [eval_DFN].imd   = wrssig_pnt->EVAL  ;
      pnt [level_DFN].imd  = wrssig_pnt->LEVEL ;

      break;

	/* ###------------------------------------------------------### */
	/*    wrimsg							*/
	/* ###------------------------------------------------------### */

    case wrimsg_DFN :

      wrimsg_pnt = (struct wrimsg *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | wrimsg_DFN;
      typ [bemsg_DFN]     = POINTER_DFN   | s_DFN | bemsg_DFN ;
      typ [lkdins_DFN]    = POINTER_DFN   | s_DFN | lkdins_DFN;
      typ [eval_DFN]      = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [errflg_DFN]    = IMMEDIATE_DFN | d_DFN | character_DFN;
      typ [level_DFN]     = IMMEDIATE_DFN | u_DFN | character_DFN;

      pnt [next_DFN].dat   = wrimsg_pnt->NEXT  ;
      pnt [bemsg_DFN].dat  = wrimsg_pnt->BEMSG ;
      pnt [lkdins_DFN].dat = wrimsg_pnt->LKDINS;
      pnt [eval_DFN].imd   = wrimsg_pnt->EVAL  ;
      pnt [errflg_DFN].imd = wrimsg_pnt->ERRFLG;
      pnt [level_DFN].imd  = wrimsg_pnt->LEVEL ;

      break;

	/* ###------------------------------------------------------### */
	/*    redlst							*/
	/* ###------------------------------------------------------### */

    case redlst_DFN :

      redlst_pnt = (struct redlst *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | redlst_DFN;
      typ [valu_DFN]      = POINTER_DFN   | c_DFN | character_DFN;

      pnt [next_DFN].dat = redlst_pnt->NEXT;
      pnt [valu_DFN].dat = redlst_pnt->VALU;

      break;

	/* ###------------------------------------------------------### */
	/*    prjbvl							*/
	/* ###------------------------------------------------------### */

    case prjbvl_DFN :

      prjbvl_pnt = (struct prjbvl *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | prjbvl_DFN;
      typ [glnext_DFN]    = POINTER_DFN   | s_DFN | prjbvl_DFN;
      typ [cndval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [drvval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat   = prjbvl_pnt->NEXT;
      pnt [glnext_DFN].dat = prjbvl_pnt->GLNEXT;
      pnt [cndval_DFN].imd = prjbvl_pnt->CNDVAL;
      pnt [drvval_DFN].imd = prjbvl_pnt->DRVVAL;

      break;

	/* ###------------------------------------------------------### */
	/*    prjrvl							*/
	/* ###------------------------------------------------------### */

    case prjrvl_DFN :

      prjrvl_pnt = (struct prjrvl *) curpnt.data;

      typ [next_DFN]      = POINTER_DFN   | s_DFN | prjrvl_DFN;
      typ [cndval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;
      typ [drvval_DFN]    = IMMEDIATE_DFN | c_DFN | character_DFN;

      pnt [next_DFN].dat   = prjrvl_pnt->NEXT;
      pnt [cndval_DFN].imd = prjrvl_pnt->CNDVAL;
      pnt [drvval_DFN].imd = prjrvl_pnt->DRVVAL;

      break;

	/* ###------------------------------------------------------### */
	/*    shdul							*/
	/* ###------------------------------------------------------### */

    case shdul_DFN :

      shdul_pnt = (struct shdul *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | shdul_DFN  ;
      typ [label_DFN]            = POINTER_DFN   | s_DFN | void_DFN   ;
      typ [ttable_DFN]           = ARYOFPNT_DFN  | s_DFN | shent_DFN  ;
      typ [itable_DFN]           = ARYOFPNT_DFN  | s_DFN | shwav_DFN  ;
      typ [curtime_DFN]          = IMMEDIATE_DFN | u_DFN | integer_DFN;

      pnt [next_DFN].dat         = shdul_pnt->NEXT   ;
      pnt [label_DFN].dat        = shdul_pnt->LABEL  ;
      pnt [ttable_DFN].dat       = shdul_pnt->TTABLE ;
      pnt [itable_DFN].dat       = shdul_pnt->ITABLE ;
      pnt [curtime_DFN].imd      = shdul_pnt->CURTIME;

      break;

	/* ###------------------------------------------------------### */
	/*    shtra							*/
	/* ###------------------------------------------------------### */

    case shtra_DFN :

      shtra_pnt = (struct shtra *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | shtra_DFN    ;
      typ [prev_DFN]             = POINTER_DFN   | s_DFN | void_DFN     ;
      typ [wave_DFN]             = POINTER_DFN   | s_DFN | shtra_DFN    ;
      typ [date_DFN]             = IMMEDIATE_DFN | u_DFN | integer_DFN  ;
      typ [ident_DFN]            = POINTER_DFN   | s_DFN | void_DFN     ;
      typ [scdid_DFN]            = POINTER_DFN   | s_DFN | void_DFN     ;
      typ [uflag_DFN]            = IMMEDIATE_DFN | x_DFN | integer_DFN  ;
      typ [cond_DFN]             = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [valu_DFN]             = IMMEDIATE_DFN | x_DFN | character_DFN;
      typ [flags_DFN]            = IMMEDIATE_DFN | x_DFN | short_DFN    ;

      pnt [next_DFN].dat         = shtra_pnt->NEXT ;
      pnt [prev_DFN].dat         = shtra_pnt->PREV ;
      pnt [wave_DFN].dat         = shtra_pnt->WAVE ;
      pnt [date_DFN].imd         = shtra_pnt->DATE ;
      pnt [ident_DFN].dat        = shtra_pnt->IDENT;
      pnt [scdid_DFN].dat        = shtra_pnt->SCDID;
      pnt [uflag_DFN].imd        = shtra_pnt->UFLAG;
      pnt [cond_DFN].imd         = shtra_pnt->COND ;
      pnt [valu_DFN].imd         = shtra_pnt->VALU ;
      pnt [flags_DFN].imd        = shtra_pnt->FLAGS;

      break;

	/* ###------------------------------------------------------### */
	/*    shwav							*/
	/* ###------------------------------------------------------### */

    case shwav_DFN :

      shwav_pnt = (struct shwav *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | shwav_DFN  ;
      typ [ident_DFN]            = POINTER_DFN   | s_DFN | void_DFN   ;
      typ [trans_DFN]            = POINTER_DFN   | s_DFN | shtra_DFN  ;

      pnt [next_DFN].dat         = shwav_pnt->NEXT ;
      pnt [ident_DFN].dat        = shwav_pnt->IDENT;
      pnt [trans_DFN].dat        = shwav_pnt->TRANS;

      break;

	/* ###------------------------------------------------------### */
	/*    shent							*/
	/* ###------------------------------------------------------### */

    case shent_DFN :

      shent_pnt = (struct shent *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | shent_DFN  ;
      typ [date_DFN]             = IMMEDIATE_DFN | u_DFN | integer_DFN;
      typ [trans_DFN]            = ARYOFPNT_DFN  | s_DFN | shtra_DFN  ;

      pnt [next_DFN].dat         = shent_pnt->NEXT;
      pnt [date_DFN].imd         = shent_pnt->DATE;
      pnt [trans_DFN].dat        = shent_pnt->TRANS;

      break;
	/* ###------------------------------------------------------### */
	/*    ptype							*/
	/* ###------------------------------------------------------### */

    case ptype_DFN :

      ptype_pnt = (struct ptype *) curpnt.data;

      typ [next_DFN]       = POINTER_DFN   | s_DFN | ptype_DFN;
      typ [data_DFN]       = POINTER_DFN   | s_DFN | void_DFN;
      typ [type_DFN]       = IMMEDIATE_DFN | l_DFN | long_DFN;

      pnt [next_DFN].dat   = ptype_pnt->NEXT;
      pnt [data_DFN].dat   = ptype_pnt->DATA;
      pnt [type_DFN].imd   = ptype_pnt->TYPE;

      break;

	/* ###------------------------------------------------------### */
	/*    chain							*/
	/* ###------------------------------------------------------### */

    case chain_DFN :

      chain_pnt = (struct chain *) curpnt.data;

      typ [next_DFN]       = POINTER_DFN   | s_DFN | chain_DFN;
      typ [data_DFN]       = POINTER_DFN   | s_DFN | void_DFN;

      pnt [next_DFN].dat   = chain_pnt->NEXT;
      pnt [data_DFN].dat   = chain_pnt->DATA;

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
            printf ("%s", (char *)pnt[i].dat);
          printf ("\n");
          break;

        case f_DFN :
          printf ("%f\n", (float)pnt[i].imd);
          break;

        case u_DFN :
          printf ("%u\n", (unsigned int)pnt[i].imd);
          break;

        case c_DFN :
          printf ("%c\n", (char)pnt[i].imd);
          break;

        case d_DFN :
          printf ("%d\n", (int)pnt[i].imd);
          break;

        case l_DFN :
          printf ("%ld\n", (long int)pnt[i].imd);
          break;

        case x_DFN :
          printf ("0x%x\n", (int)pnt[i].imd);
          break;

        case ABL_DFN :
          if (pnt[i].dat != NULL)
            {
            buffer [0] = '\0';
            buffer = exprToCharInt (pnt[i].dat, 1, buffer, &buffer_size);
            printf ("%s", buffer);
            }
          printf ("\n");
          break;

        case BDD_DFN :
          if ((pnt[i].dat != NULL) && (circuit_pnt != NULL))
            {
            buffer [0] = '\0';
            ptr_abl = bddToAbl (pnt[i].dat, circuit_pnt->pNameI);
            buffer  = exprToCharInt (ptr_abl, 1, buffer, &buffer_size);
            printf ("%s", buffer);
            }
          printf ("\n");
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
    vhu_message (20, NULL, 0);
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
      vhu_error (120, NULL, NULL, 0);
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

  siz [lofig_DFN]     = sizeof (struct lofig);
  siz [locon_DFN]     = sizeof (struct locon);
  siz [losig_DFN]     = sizeof (struct losig);
  siz [loins_DFN]     = sizeof (struct loins);

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

  siz [paseq_DFN]     = sizeof (struct paseq);
  siz [pagrp_DFN]     = sizeof (struct pagrp);
  siz [paiol_DFN]     = sizeof (struct paiol);
  siz [papat_DFN]     = sizeof (struct papat);
  siz [pacom_DFN]     = sizeof (struct pacom);
  siz [paini_DFN]     = sizeof (struct paini);
  siz [paevt_DFN]     = sizeof (struct paevt);
  siz [pains_DFN]     = sizeof (struct pains);

  siz [lkdfig_DFN]    = sizeof (struct lkdfig);
  siz [lkdins_DFN]    = sizeof (struct lkdins);
  siz [simsig_DFN]    = sizeof (struct simsig);
  siz [bussig_DFN]    = sizeof (struct bussig);
  siz [wrbsig_DFN]    = sizeof (struct wrbsig);
  siz [wriaux_DFN]    = sizeof (struct wriaux);
  siz [wrireg_DFN]    = sizeof (struct wrireg);
  siz [wribux_DFN]    = sizeof (struct wribux);
  siz [wrssig_DFN]    = sizeof (struct wrssig);
  siz [redlst_DFN]    = sizeof (struct redlst);
  siz [prjbvl_DFN]    = sizeof (struct prjbvl);
  siz [prjrvl_DFN]    = sizeof (struct prjrvl);

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

    case float_DFN :
      printf ("%f\n", (float)pnt.imd);
      break;

    case character_DFN :
      printf ("%c\n", (char)pnt.imd);
      break;

    case short_DFN     :
    case integer_DFN   :
    case long_DFN      :
      printf ("0x%x\n", (int)pnt.imd);
      break;

    default :
      vhu_error (118, NULL, NULL, 0);
    }
  }
