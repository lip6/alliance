/*
 *  $Id: util_Defs.h,v 1.4 2002/09/30 16:20:23 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./util_Defs.h"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __util_Defs__
#   define  __util_Defs__


#   include  <unistd.h>
#   include  <stdlib.h>
#   include  <stdio.h>
#   include  <signal.h>
#   include  <ctype.h>
#   include  <string.h>
#   include  <strings.h>
#   include  <math.h>


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
 *  Missing GNU functions prototypes under GCC/SunOS 4.1.4.
 */

#   include  "GNU_Defs.h"


/*  ----------------------------------------------------------------------
 *  Miscellaneous.
 */

#   define        TRUE        1
#   define       FALSE        0

#   define    m_Max(a,b)      (((a) > (b)) ? (a) :  (b))
#   define    m_Min(a,b)      (((a) > (b)) ? (b) :  (a))
#   define      ABS(a)        (((a) >  0 ) ? (a) : -(a))


  /* Usual types shortcut. */


/*  ----------------------------------------------------------------------
 *  Alliance Includes.
 */

#   include   <mut.h>
#   include   <mlo.h>
#   include   <mph.h>
#   include   <mpu.h>
#   include   <mlu.h>
#   include   "mgn.h"
#   include   "mbkgen.h"


/*  ----------------------------------------------------------------------
 *  Module  :  "util_Sys.c"
 */

#   define        F_DUMPCORE                  0
#   define        F_NOCORE                    1

#   define        C_SIGTFLT             ((int)1)
#   define        C_SIGERR              ((int)2)

#   define        C_VerboseLevel0           (0L)
#   define        C_VerboseLevel1           (1L)
#   define        C_VerboseLevel2           (2L)

#   define        SIZE_S64                 2048


  /* External variables from the various modules. */
  extern  long    util_VL;
  extern  long    util_flags;
  extern  char    util_binName[1024];

  /* Interruptions handling functions. */
  extern void  trapInit    __FP((void));
  /*extern void  sendGSignal __FP((int aSig));*/
  extern void  setBinName  __FP((char *asName));

  /* Errors printing functions. */
  extern void  stdflush    __FP((void));
  extern void  errMBK      __FP((char *asName));
  extern void  eprinth     __FP((char *asName));
  extern  int  eprintf     __FPV((char *aFormat, ...));
  extern  int  eprints     __FP((char *aS));

  /* Warnings printing functions. */
  extern void  warnMBK     __FP((char *asName));
  extern void  wprinth     __FP((char *asName));
  extern  int  wprintf     __FPV((char *aFormat, ...));
  extern  int  wprints     __FP((char *aS));

  /* Messages printing functions. */
  extern void  setVL       __FP((long aVL));
  extern long  getVL       __FP((void));
  extern  int  vmprintf    __FP((long aVL, char *aFormat, va_list aArgList));
  extern  int  mprintf     __FPV((long aVL, char *aFormat, ...));
  extern  int  mprints     __FP((long aVL, char *aS));
  extern  int  mprintf0    __FPV((char *aFormat, ...));
  extern  int  mprintf1    __FPV((char *aFormat, ...));
  extern  int  mprintf2    __FPV((char *aFormat, ...));

  /* "util_init" global initialisation function. */
  extern void  util_init   __FP((long aVL, long aFlags, char *asName));

  /* Persistent strings printing function (up to 64 strings). */
  extern char *s64printf   __FPV((char *aFormat, ...));

  /* Miscellaneous. */
  extern char *strtoup     __FP((char *aS));


/*  ----------------------------------------------------------------------
 *  Module  :  "util_Const.c"
 */


#   define        XLONG_SIZE        8


  typedef struct xlong_s {
    long  size;
    long  l[XLONG_SIZE];
  } xlong_t;


  extern struct xlong_s *newxl      __FP((void));
  extern           void  freexl     __FP((struct xlong_s *apXl));
  extern           void  atoxl      __FP((struct xlong_s *apXl,
                                                   char *asConst));
  extern           long  getxlbit   __FP((struct xlong_s *apXl, long  aBit));
  extern           long  getxlhexa  __FP((struct xlong_s *apXl, long  aBit));
  extern           char *xltovhdl   __FP((struct xlong_s *apXl, char *aS));


/*  ----------------------------------------------------------------------
 *  Module  :  "util_Misc.c"
 */


  /* Some useful macros. They relies on modules local variables
   * 'LV_N' & 'LV_flags'.
   */
# define  DPGEN_PORT_BIT(s) \
            SLICE_TO_BIT((s), LV_N, LV_flags & F_MSB_FIRST)

# define  DPGEN_LOCON_BUS(name,dir)  \
            GENLIB_LOCON ( FULL_BUS(name, LV_N, LV_flags & F_MSB_FIRST), dir, \
                           FULL_BUS(name, LV_N, LV_flags & F_MSB_FIRST))

# define  VHDL_BUS_WIDE(bus,msb,lsb) \
            sprintf (bus,                   \
                     "(%ld %s %ld)",        \
                     DPGEN_PORT_BIT (msb),  \
                     (LV_flags & F_MSB_FIRST) ? "to" : "downto" , \
                     DPGEN_PORT_BIT (lsb))


  extern  int  ilog2               __FP((long x));
  extern  int  iexp2               __FP((long x));
  extern long  get_depth           __FP((long x, long depth));
  extern void  DPGEN_PLACE         __FP((char *aModelName,
                                         char *aInsName,
                                         long  aX,
                                         long  aY));


# endif
