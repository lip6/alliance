/*
 *  $Id: DEF_drive.h,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./DEF_drive.h"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __DEF_drive__
#   define  __DEF_drive__


#   define        F_PHFIG         0x00000001
#   define        F_FIXED_PINS    0x00000002
#   define        F_PAD           0x00000004
#   define        F_LAYERS_3      0x00000008
#   define        F_LAYERS_4      0x00000010
#   define        F_LAYERS_6      0x00000020
#   define        F_EXPAND_PLACE  0x00000040
#   define        F_EXPAND_ROUTE  0x00000080
#   define        F_TRUST_ORIENT  0x00000100


  extern long          hasROW (struct phfig *apPhfig);
  extern long       buildROWS (struct phfig *apPhfig);
  extern long     buildTRACKS (struct phfig *apPhfig, long aFlags);
  extern void  defsavelophfig (struct lofig *apLofig,
                                       struct phfig *apPhfig,
                                               char *aDefName,
                                               long  aFlags);

# endif
