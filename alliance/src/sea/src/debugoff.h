/*
 *  $Id: debugoff.h,v 1.1 2002/04/25 16:16:19 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./debugoff.h"                            |
 *  | ************************************************************** |
 *  |  Compile without debug specific code.                          |
 *  |                                                                |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifdef  __DBG__
#   undef  __DBG__
# endif
# ifdef  __DBG
#   undef  __DBG
# endif
# define  __DBG(code)
