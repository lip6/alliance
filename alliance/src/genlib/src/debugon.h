/*
 *  $Id: debugon.h,v 1.2 2002/09/30 16:20:21 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./debugon.h"                             |
 *  | ************************************************************** |
 *  |  Compile with debug specific code.                             |
 *  |                                                                |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __DBG__
#   define  __DBG__
# endif
# ifdef  __DBG
#   undef  __DBG
# endif
# define  __DBG(code)  code
