/*
 *  $Id: dpgen_Defs.h,v 1.2 2002/07/15 22:19:25 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Header    :         "./dpgen_Defs.h"                          |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __dpgen_Defs__
#   define  __dpgen_Defs__




#   define  Y_SLICE              50


#   define  SLICE_TO_Y(s,N)      ((s) * Y_SLICE)
#   define  SLICE_TO_SYM(s,N)    (((s) % 2) ? SYM_Y : NOSYM)
#   define  SLICE_TO_BIT(s,N,m)  ((m) ? ((N) - 1 - (s)) : (s))
#   define  FULL_BUS(name,N,m)   (GENLIB_BUS(                        \
                                    (name),                          \
                                    SLICE_TO_BIT((N) - 1, (N), (m)), \
                                    SLICE_TO_BIT( 0     , (N), (m)) ))


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_Macro.c"
 */

 extern char *function_To_String __FP((long aFunction));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_Regular.c"
 */

 extern void  dpgen_Regular __FP((long aFunction, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_RF1.c"
 */

 extern void  dpgen_RF1 __FP((long aFunction, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_RF2.c"
 */

 extern void  dpgen_RF2 __FP((long aFunction, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_RAM.c"
 */

 extern void  dpgen_RAM __FP((long aFunction, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_ROM.c"
 */

 extern void  dpgen_ROM      __FP((long aFunction, va_list aAL));
 extern void  dpgen_ROM_code __FP(( char *s
                                  , long  nb
                                  , long  nw
                                  , long  r
                                  , long  msb
                                  , long *data
                                  ));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_Shifter.c"
 */

 extern void  dpgen_Shifter __FP((long aFunction, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_Adder.c"
 */


#   define        F_ADDER_CIN        0x00000001
#   define        F_ADDER_SIGN       0x00000002
#   define        F_ADDER_OVER       0x00000004


 extern void  dpgen_Adder __FP((long aFunction, long aFlags, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_uROM.c"
 */

 extern void  dpgen_uROM __FP((long aFunction, va_list aAL));


/*  ----------------------------------------------------------------------
 *  Module  :  "dpgen_Nul.c"
 */

 extern void  dpgen_Nul __FP((long aFunction, va_list aAL));


# endif
