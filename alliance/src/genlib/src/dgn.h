/*
 *  $Id: dgn.h,v 1.3 2002/07/15 22:19:25 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Header    :         "./dpgen.h"                               |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __dpgen__
#   define  __dpgen__


/*  ----------------------------------------------------------------------
 *  Looks if functions prototypes are availables.
 */

#   ifndef __FP
#     ifdef  __STDC__
        /* Compiler is ANSI C compliant. */
#       include  <unistd.h>
#       include  <stdarg.h>

#       define   __FP(  arguments )      arguments
#       define   __FPV( arguments )      arguments

#       define   __KR_C(   code )
#       define   __ANSI_C( code )        code
#     else
        /* Compiler is K&R C. */
#       include  <unistd.h>
#       include  <varargs.h>

#       define   __FP(  arguments )     ()
#       define   __FPV( arguments )     (va_alist) va_dcl

#       define   __KR_C(   code )        code
#       define   __ANSI_C( code )
#     endif
#   endif


/*  ----------------------------------------------------------------------
 *  Avalaibles values (i.e. kind of generators) for the "function"
 *  argument of "GENLIB_MACRO".
 */

#   define        DPGEN_INV          1
#   define        DPGEN_BUFF         2
#   define        DPGEN_NAND2        3
#   define        DPGEN_NAND3        4
#   define        DPGEN_NAND4        5
#   define        DPGEN_AND2         6
#   define        DPGEN_AND3         7
#   define        DPGEN_AND4         8
#   define        DPGEN_NOR2         9
#   define        DPGEN_NOR3        10
#   define        DPGEN_NOR4        11
#   define        DPGEN_OR2         12
#   define        DPGEN_OR3         13
#   define        DPGEN_OR4         14
#   define        DPGEN_XNOR2       15
#   define        DPGEN_XOR2        16
#   define        DPGEN_NMUX2       17
#   define        DPGEN_MUX2        18
#   define        DPGEN_NBUSE       19
#   define        DPGEN_BUSE        20
#   define        DPGEN_NAND2MASK   21
#   define        DPGEN_NOR2MASK    22
#   define        DPGEN_XNOR2MASK   23
#   define        DPGEN_ADSB2F      24
#   define        DPGEN_SHIFT       25
#   define        DPGEN_NUL         26
#   define        DPGEN_CONST       27
#   define        DPGEN_RF1         28
#   define        DPGEN_RF1R0       29
#   define        DPGEN_RF1D        30
#   define        DPGEN_RF1DR0      31
#   define        DPGEN_FIFO        32
#   define        DPGEN_RF2         33
#   define        DPGEN_RF2R0       34
#   define        DPGEN_RF2D        35
#   define        DPGEN_RF2DR0      36
#   define        DPGEN_RAM         37
#   define        DPGEN_ROM         38
#   define        DPGEN_ROMTS       39
#   define        DPGEN_DFF         40
#   define        DPGEN_DFFT        41
#   define        DPGEN_SFF         42
#   define        DPGEN_SFFT        43
#   define        DPGEN_ROM2        44
#   define        DPGEN_ROM4        45
#   define        DPGEN_SHROT       46
#   define        DPGEN_UNKNOWN     47


/*  ----------------------------------------------------------------------
 *  Avalaibles flags values (i.e. views & terminal index) for the "flags"
 *  argument of "GENLIB_MACRO".
 */

#   define        F_LSB_FIRST       0x00000000
#   define        F_MSB_FIRST       0x00000001
#   define        F_PLACE           0x00000002
#   define        F_BEHAV           0x00000004


/*  ----------------------------------------------------------------------
 */

 extern void  GENLIB_MACRO __FPV((long function, ...));


# endif
