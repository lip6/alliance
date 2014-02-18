
/* ###--------------------------------------------------------------### */
/* file		: sch_debug.c						*/
/* date		: Aug 10 1997						*/
/* version	: v101							*/
/* authors	: Nizar ABDALLAH, Pirouz BAZARGAN SABET			*/
/* content	: scheduler						*/
/* ###--------------------------------------------------------------### */

#include <stdio.h>
#include <string.h>
#include "mut.h"
#include "sch.h"
#include "sch_debug.h"


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
  unsigned int    i         ;

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
	/*    shdul							*/
	/* ###------------------------------------------------------### */

    case shdul_DFN :

      shdul_pnt = (struct shdul *) curpnt.data;

      typ [next_DFN]             = POINTER_DFN   | s_DFN | shdul_DFN  ;
      typ [label_DFN]            = POINTER_DFN   | s_DFN | void_DFN  ;
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
            printf ("%s", (char*)pnt[i].dat);
          printf ("\n");
          break;

        case f_DFN :
          printf ("%ld\n", pnt[i].imd);
          break;

        case u_DFN :
          printf ("%ld\n", pnt[i].imd);
          break;

        case c_DFN :
          printf ("%c\n", (char)pnt[i].imd);
          break;

        case d_DFN :
          printf ("%ld\n", pnt[i].imd);
          break;

        case l_DFN :
          printf ("%ld\n", pnt[i].imd);
          break;

        case x_DFN :
          printf ("0x%x\n", (unsigned int)pnt[i].imd);
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
    sch_message (20, "sch_debug", 0);
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
      sch_error (120, "sch_debug", NULL, 0);
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

struct chain *pnt  ;
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

  siz [shent_DFN]     = sizeof (struct shent);
  siz [shwav_DFN]     = sizeof (struct shwav);
  siz [shtra_DFN]     = sizeof (struct shtra);

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
      printf ("%ld\n", pnt.imd);
      break;

    case character_DFN :
      printf ("%c\n", (char)pnt.imd);
      break;

    case short_DFN     :
    case integer_DFN   :
    case long_DFN      :
      printf ("0x%x\n", (unsigned int)pnt.imd);
      break;

    default :
      sch_error (118, "sch_debug", NULL, 0);
    }
  }


/* ###--------------------------------------------------------------### */
/* function	: sch_debug						*/
/* description	: display any structure					*/
/* called func.	: none							*/
/* ###--------------------------------------------------------------### */

void sch_debug (head_pnt, type)

void *head_pnt;				/* structure's pointer		*/
char *type;				/* structure's type		*/

  {
  size_t        lline  = 128;
  char         *line   = NULL;		/* buffer to read a cmd line	*/
  char          buffer [lline];		/* buffer to split the cmd line	*/

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

                         "shdul"     , "shent"     , "shtra"     , "shwav"     ,

                         "cond"      , "curtime"   , "date"      , "flags"     ,
                         "ident"     , "itable"    , "label"     , "next"      ,
                         "prev"      , "scdid"     , "trans"     , "ttable"    ,
                         "uflag"     , "valu"      , "wave"      
                         };

	/* ###------------------------------------------------------### */
	/*    initialisation :						*/
	/*    - allocate a buffer for read words			*/
	/*    - break the argument that identifies the structure (type)	*/
	/*      into words						*/
	/*    - search that words among recognized strings		*/
	/* ###------------------------------------------------------### */

  line = (char*)malloc( lline*sizeof(char) );
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
      sch_error (118, "sch_debug", NULL, 0);
    else
      {

	/* ###------------------------------------------------------### */
	/*    define the type of the structure that may be pushed on	*/
	/* the stack :							*/
	/*    - if the pointer is a VOID pointer the type must be	*/
	/*      defined on the command line (last word of the command)	*/
	/* ###------------------------------------------------------### */

      pshtype = POINTER_DFN | s_DFN | void_DFN;
      if ((typ [idx] & TYPE_DFN) == void_DFN)
        {
        if ((wrdcnt >= 2) && (indxs [wrdcnt - 1] != _error_DFN))
          pshtype = POINTER_DFN | s_DFN | indxs [wrdcnt - 1];
        else
          sch_error (119, "sch_debug", NULL, 0);
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
              sch_message (19, "sch_debug", 0);
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
                sch_error (118, "sch_debug", NULL, 0);
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
                  sch_error (118, "sch_debug", NULL, 0);
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
                sch_error (118, "sch_debug", NULL, 0);
              break;

	/* ###------------------------------------------------------### */
	/*    _display COMMAND : display a specified field as an	*/
	/* immediate							*/
	/* ###------------------------------------------------------### */

            case _display_DFN :
              if ((wrdcnt == 3) && (indxs [1] != _error_DFN))
                disp_immd (str, pnt [indxs [1]], indxs [2]);
              else
                sch_error (118, "sch_debug", NULL, 0);
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
                sch_error (118, "sch_debug", NULL, 0);
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
                   (((unsigned long) pnt [idx].dat) + (size * nmbrs [1]));
            push (stk, &stkpnt, pntr, pshtype);
            readflg = 1;
            }
          else
            sch_error (118, "sch_debug", NULL, 0);
          break;

	/* ###------------------------------------------------------### */
	/*   ARRAIES of pointers					*/
	/* ###------------------------------------------------------### */

        case ARYOFPNT_DFN :
          if ((wrdcnt > 1) && (flags [1] == 1))
            {
            size = sizeof (void *);
            pntr = * (void **)
                     (((unsigned long) pnt [idx].dat) + (size * nmbrs [1]));
            push (stk, &stkpnt, pntr, pshtype);
            readflg = 1;
            }
          else
            sch_error (118, "sch_debug", NULL, 0);
          break;

        }

      if (readflg == 1)
        {
        readflg = 0;
        read_field (stk [stkpnt], pnt, typ, str);
        }

      }

    printf ("\n\nCOMMAND > ");

    getline (&line, &lline, stdin);
    if (strcmp (line ,"."))
      {
      wrdcnt = splitline (words, line);
      idx    = translate (words, wrdcnt, str, nmbrs, flags, indxs);
      }
    }

  }
