/*
 *  $Id: LEF_drive_sxlib.h,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./LEF_drive_sxlib.h"                     |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __LEF_drive_sxlib__
#   define  __LEF_drive_sxlib__

    /* SxLib to LEF conversion options. */
#   define  F_ALLOW_OFFGRID        0x00000001
#   define  F_NO_POWER_GEOM        0x00000002
#   define  F_POWER_ABUTMENT       0x00000004
#   define  F_POWER_FEEDTHRU       0x00000008


  extern void  lefsavesxlophfig (struct lofig *apLofig,
                                 struct phfig *apPhfig,
                                         long  aFlags);


# endif
