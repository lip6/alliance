/*
 *  $Id: util_Genlib.c,v 1.2 2002/09/30 16:20:23 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./util_Genlib.c"                         |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"


/*  ------------------------------------------------------------------
 *  Global variables (declared `extern' in "util_Defs.h").
 */


/*  ------------------------------------------------------------------
 *  Internal definitions.
 */

 typedef struct eStack_s {
   struct lofig    *WORK_LOFIG;
   struct phfig    *WORK_PHFIG;
   struct phins    *WORK_PHINS;
           long     num_index;
   struct eStack_s *Prev;
 } eStack_t;


 eStack_t *genlibStack = NULL;


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "GENLIB_PUSH()".
 */

extern void  GENLIB_PUSH()
{
  eStack_t *pNew;


  pNew = (eStack_t*)malloc(sizeof(eStack_t));

  if (pNew == NULL) {
    eprinth(NULL);
    eprints("GENLIB_PUSH: Cannot allocate new stack element.\n");
    EXIT(1);
  }

  pNew->WORK_LOFIG = WORK_LOFIG;
  pNew->WORK_PHFIG = WORK_PHFIG;
  pNew->WORK_PHINS = WORK_PHINS;
  pNew->num_index  = num_index;
  pNew->Prev       = genlibStack;

  WORK_LOFIG = NULL;
  WORK_PHFIG = NULL;
}


/*  ------------------------------------------------------------------
 *  Function  :  "GENLIB_POP()".
 */

extern void  GENLIB_POP()
{
  eStack_t *pDel;


  if (genlibStack == NULL) {
    eprinth(NULL);
    eprints("GENLIB_POP: Attempt to pop from an empty stack.\n");
    EXIT(1);
  }

  WORK_LOFIG = genlibStack->WORK_LOFIG;
  WORK_PHFIG = genlibStack->WORK_PHFIG;
  WORK_PHINS = genlibStack->WORK_PHINS;
  num_index  = genlibStack->num_index;

  pDel = genlibStack;
  genlibStack = genlibStack->Prev;

  free((void*)pDel);
}
