/*
 *  $Id: util_Misc.c,v 1.1 2002/04/29 13:40:55 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./util_Misc.c"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "ilog2 ()".
 */

extern int ilog2 (x)
  long  x;
{
  int i;

  for (i = 0; x; i++, x = x >> 1);

  return i;
} 


/*  ------------------------------------------------------------------
 *  Function  :  "iexp2 ()".
 */

extern int iexp2 (x)
  long  x;
{
  return (1 << x);
}


/*  ------------------------------------------------------------------
 *  Function  :  "get_depth ()".
 */

extern long get_depth (x, depth)
  long  x, depth;
{
  long  mDepth, i;


  mDepth = ilog2 (x);


  for (i = depth; i <= mDepth; i++)
    if (x & (1 << i)) return (i);

  return (0);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DPGEN_PLACE ()".
 */

extern void  DPGEN_PLACE (aModelName, aInsName, aX, aY)
  char *aModelName, *aInsName;
  long  aX, aY;
{
  char  sym;


  sym = NOSYM;
  if ((aY / 50) % 2) sym = SYM_Y;

  GENLIB_PLACE (aModelName, aInsName, sym, aX, aY);
}
