
/*
 *  $Id: util_Inter.c,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_Inter.c"                          |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"



/*  ------------------------------------------------------------------
 *  Local constants.
 */

# define        SHIFT_MAX            8
# define        F_MIN_IN             0x00000001
# define        F_MIN_LOWER          0x00000002
# define        F_MIN_GREATER        0x00000004
# define        F_MAX_IN            ((F_MIN_IN)      << SHIFT_MAX)
# define        F_MAX_LOWER         ((F_MIN_LOWER)   << SHIFT_MAX)
# define        F_MAX_GREATER       ((F_MIN_GREATER) << SHIFT_MAX)
# define        C_INTER_LOWER       (F_MIN_LOWER   | F_MAX_LOWER  )
# define        C_INTER_CROSS_MIN   (F_MIN_LOWER   | F_MAX_IN     )
# define        C_INTER_IN          (F_MIN_IN      | F_MAX_IN     )
# define        C_INTER_CROSS_MAX   (F_MIN_IN      | F_MAX_GREATER)
# define        C_INTER_GREATER     (F_MIN_GREATER | F_MAX_GREATER)
# define        C_INTER_OUT         (F_MIN_LOWER   | F_MAX_GREATER)


# define        INBOUNDS_INTER(i,v)  (inbounds ((i)->min, (i)->max, (v)))


/*  ------------------------------------------------------------------
 *  Local functions declarations.
 */

static long             inbounds    __FP((long aMin, long aMax, long aValue));
static struct eInter_s *alloceinter __FP((long aMin, long aMax));
static            void  freeeinter  __FP((struct eInter_s *apInter));
static            long  cmpeinter   __FP((struct eInter_s *apInterA,
                                          struct eInter_s *apInterB));



/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "inbounds()".
 */

static long  inbounds (aMin, aMax, aValue)
  long aMin, aMax, aValue;
{
  if ((aValue >= aMin) && (aValue <= aMax)) return (F_MIN_IN);

  if (aValue < aMin) return (F_MIN_LOWER);

  return (F_MIN_GREATER);
}


/*  ------------------------------------------------------------------
 *  Function  :  "alloceinter()".
 */

static struct eInter_s *alloceinter (aMin, aMax)
  long  aMin, aMax;
{
  struct eInter_s *newInter;


  if (!(newInter = (struct eInter_s*)mbkalloc(sizeof (struct eInter_s)))) {
    eprinth ("util_Inter.c");
    eprintf ("Not enough memory to allcate an interval element.\n");
    EXIT (1);
  }

  newInter->min  = aMin;
  newInter->max  = aMax;
  newInter->next = NULL;

  return (newInter);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freeeinter()".
 */

static void  freeeinter (apInter)
  struct eInter_s *apInter;
{
  mbkfree ((void*)apInter);
}


/*  ------------------------------------------------------------------
 *  Function  :  "cmpeinter()".
 */

static long  cmpeinter (apInterA, apInterB)
  struct eInter_s *apInterA, *apInterB;
{
  long  state;


  state  = INBOUNDS_INTER(apInterA, apInterB->min);
  state |= INBOUNDS_INTER(apInterA, apInterB->max) << SHIFT_MAX;

  return (state);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocinter()".
 */

static struct lInter_s *allocinter ()
{
  struct lInter_s *newInter;


  if (!(newInter = (struct lInter_s*)mbkalloc(sizeof (struct lInter_s)))) {
    eprinth ("util_Inter.c");
    eprintf ("Not enough memory to allocate an interval object.\n");
    EXIT (1);
  }

  newInter->min  = 0;
  newInter->max  = 0;
  newInter->len  = 0;
  newInter->l    = NULL;

  return (newInter);
}


/*  ------------------------------------------------------------------
 *  Function  :  "printinter()".
 */

extern void  printinter (apInter)
  struct lInter_s *apInter;
{
  struct eInter_s *pInter;


  fflush (stderr);
  fprintf (stdout, "  o  Interval set : (min = %ld, max = %ld, len = %ld)\n",
           apInter->min,
           apInter->max,
           apInter->len);
  fflush (stdout);

  for (pInter = apInter->l; pInter != NULL; pInter = pInter->next) {
    fprintf (stdout, "     [%6ld,%6ld]\n", pInter->min, pInter->max);
    fflush (stdout);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "addinter()".
 */

extern struct lInter_s *addinter (apInter, aMin, aMax)
  struct lInter_s *apInter;
             long  aMin;
             long  aMax;
{
  struct eInter_s  *currInter;
  struct eInter_s  *newInter;
  struct eInter_s   mergeInter;
  struct eInter_s **ppInter;


  /* Case of the first interval. */
  if (apInter == NULL) apInter = allocinter  ();


  /* Update the interval list bounds. */
  apInter->max = m_Max(apInter->max, aMax);
  apInter->min = m_Min(apInter->min, aMin);


  mergeInter.min = aMin;
  mergeInter.max = aMax;
  newInter = NULL;
  ppInter  = &(apInter->l);


  for (; *ppInter != NULL;) {
    switch (cmpeinter (&mergeInter, *ppInter)) {
      case C_INTER_LOWER: break;
      case C_INTER_OUT:   break;

      case C_INTER_CROSS_MIN:
        (*ppInter)->max = mergeInter.max;
        newInter        = *ppInter;
        break;

      case C_INTER_IN:
        if (!newInter) {
          (*ppInter)->min = mergeInter.min;
          (*ppInter)->max = mergeInter.max;
          newInter        = *ppInter;
        } else {
          currInter = *ppInter;
          *ppInter  = (*ppInter)->next;
          freeeinter (currInter);
          apInter->len -= 1;
          continue;
        }
        break;

      case C_INTER_CROSS_MAX:
        if (!newInter) {
          (*ppInter)->min = mergeInter.min;
          newInter        = *ppInter;
        } else {
          newInter->max  = (*ppInter)->max;
          mergeInter.max = (*ppInter)->max;

          currInter = *ppInter;
          *ppInter  = (*ppInter)->next;
          freeeinter (currInter);
          apInter->len -= 1;
          continue;
        }
        break;

      case C_INTER_GREATER:
        if (!newInter) {
          newInter       = alloceinter (mergeInter.min, mergeInter.max);
          newInter->next = *ppInter;
          *ppInter       = newInter;
          apInter->len  += 1;
        }
        break;
    }

    ppInter  = &((*ppInter)->next);
  }


  /* Tail of first added interval. */
  if (!newInter) {
    *ppInter      = alloceinter (aMin, aMax);
    apInter->len += 1;
  }


  return (apInter);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freeinter()".
 */

extern void  freeinter (apInter)
  struct lInter_s *apInter;
{
  struct eInter_s *pNextInter;
  struct eInter_s *pInter;


  for (pInter = apInter->l; pInter != NULL; ) {
    pNextInter = pInter->next;

    freeeinter (pInter);

    pInter = pNextInter;
  }

  mbkfree ((void*)apInter);
}
