/*
 *  $Id: DEF_grammar.h,v 1.3 2003/06/26 17:00:39 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                            Mael Nagat             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./DEF_grammar.h"                         |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */

# ifndef  __DEF_grammar__
#   define  __DEF_grammar__

#   define        F_DEF_SPLIT_POWER        0x00000001
#   define        F_DEF_NO_IOS             0x00000002
#   define        F_DEF_NO_INTERF          0x00000004
#   define        F_DEF_MERGE_TERM         0x00000008
#   define        F_DEF_SHRINK             0x00000010
#   define        F_DEF_KEEP_IOS           0x00000020


  extern void  defloadlophfig (lofig_list *apLoFig,
                               phfig_list *apPhFig,
                                     char *aName,
                                     char  aMode,
                                     long  aVL,
                                     long  aFlags);


# endif
