/*
 *  $Id: MAC_drive.h,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./MAC_drive.h"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __MAC_drive__
#   define  __MAC_drive__


#   define        F_MAC_IOC         0x00000001
#   define        F_MAC_NO_POWER    0x00000002
#   define        F_MAC_FIXED_PINS  0x00000004
#   define        F_MAC_FILL_TIE    0x00000008


  extern void  macPlace __FP((struct phfig *apPhfig,
                              struct lofig *apLofig,
                                      char *macName,
                                      long  aPower,
                                      long  aFlags));
  extern void  macRoute __FP((struct phfig *apPhfig,
                              struct lofig *apLofig,
                                      char *macName,
                                      long  aPower,
                                      long  aFlags));


# endif
