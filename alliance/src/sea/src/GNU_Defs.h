
/*
 *  $Id: GNU_Defs.h,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./GNU_Defs.h"                            |
 *  | ************************************************************** |
 *  |                                                                |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifdef  __GNUC__
#   if  defined(__sparc__) && !defined(__svr4__)
#     define  __MISS_GNU_PROTO__  1
#   else
#     define  __MISS_GNU_PROTO__  0
#   endif 


/*  ----------------------------------------------------------------------
 *  Functions from "<errno.h>".
 */


#   ifdef  _ERRNO_H
#     define  _errno_h  1
#   endif
#   ifdef  _errno_h
#     if  __MISS_GNU_PROTO__
        extern void   perror    __FP(( char* ));
#     endif
#   endif


/*  ----------------------------------------------------------------------
 *  Functions from "<signal.h>".
 */


#   ifdef  _SIGNAL_H
#     define  __signal_h  1
#   endif
#   ifdef  __signal_h
#     if  __MISS_GNU_PROTO__
        extern void (*ssignal())();
        extern  int   gsignal    __FP(( int ));
#     endif
#   endif


/*  ----------------------------------------------------------------------
 *  Functions from "<unistd.h>".
 */


#   ifdef  _SYS_UNISTD_H
#     define  __sys_unistd_h  1
#   endif
#   ifdef  __sys_unistd_h
#     if  __MISS_GNU_PROTO__
        extern  int   read    __FP(( int, void*, __SIZE_TYPE__ ));
#     endif
#   endif


/*  ----------------------------------------------------------------------
 *  Functions from "<stdio.h>".
 */


#   ifdef  _STDIO_H
#     define  __stdio_h  1
#   endif
#   ifdef  FILE
#     define  __stdio_h  1
#   endif
#   ifdef  __stdio_h
#     if  __MISS_GNU_PROTO__
        extern  int   fclose     __FP(( FILE* ));
        extern  int   fflush     __FP(( FILE* ));
        extern  int    printf    __FP(( char*, ... ));
        extern  int   fprintf    __FP(( FILE*, char*, ... ));
        extern  int  vfprintf    __FP(( FILE*, char*, va_list ));
        extern  int  vsprintf    __FP(( char*, char*, va_list ));
        extern  int   fputs      __FP(( char*, FILE* ));
#     endif
#   endif


# endif
