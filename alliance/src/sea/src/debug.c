
/*
 *  $Id: debug.c,v 1.2 2002/09/30 16:21:18 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module    :         "./debug.c"                             |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"





/*
 *  /--------------------------------------------------------------------\
 *  |                     Functions Definitions                          |
 *  \--------------------------------------------------------------------/
 */

/*  ----------------------------------------------------------------------
 *  Function  :  "main()".
 */

extern int main(argc, argv)
   int  argc;
  char *argv[];
{
  struct lInter_s *pInter;


  pInter = NULL;

  /* Add the "B" segments. */
  pInter = addinter (pInter,  1,  3);
  pInter = addinter (pInter,  5,  7);
  pInter = addinter (pInter,  9, 11);
  pInter = addinter (pInter, 13, 15);
  pInter = addinter (pInter, 17, 19);
# if 0
# endif

  /* See if the B segments are OK. */
  printinter (pInter);

  /* Add the "A" interval. */
  pInter = addinter (pInter,  4, 16);
# if 0
  pInter = addinter (pInter,  6, 14);
  pInter = addinter (pInter,  2,  6);
# endif

  /* See if the "A" segments has been correctly merged. */
  printinter (pInter);
  

  /* Free all intervals. */
  freeinter (pInter);

  exit(0);
}
