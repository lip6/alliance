/*
 *  $Id: util_Sys.c,v 1.2 2002/09/14 19:10:40 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_Sys.c"                            |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"


/*  ------------------------------------------------------------------
 *  Global variables (declared `extern' in "util_Defs.h").
 */

         long    util_VL            = 1L;
         long    util_flags         = 0L;
         char    util_binName[1024] = "noname";


/*  ------------------------------------------------------------------
 *  Internal functions declarations.
 */

 static void  trapSig __FP((int aSig));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "trapSig()".
 */

static void  trapSig(aSig)
  int  aSig;
{
  switch (aSig)
  {
    case C_SIGERR:
      /* A "normal" error occurs (produced by 'esignal'). */
      break;

    case C_SIGTFLT:
      /* "util_init" was unable to setup 'trapSig'. */
      eprinth ("util_Sys");
      eprints ("\n  Unable to trap SOFTSIG|SIGSEGV|SIGTERM.\n");
      break;

    case SIGTERM:
      /* This occurs on MBK errors. */
      eprinth ((char *)NULL);
      eprints ("\n  An error have occur in the MBK data-base.");
      eprints ("\n  Please check your Alliance environment.");
      eprints ("\n\n");
      eprintf ("\n  %s terminated.", util_binName);
      eprints ("\n" );
      break;

    case SIGFPE:
    case SIGBUS:
    case SIGSEGV:
      /* Ouch !! This may result from a program bug. */
      eprinth ((char *)NULL);
      eprintf ("\n  An %s internal bug have occur ", util_binName);

      if (aSig == SIGFPE ) eprints ("(SIGFPE)." );
      if (aSig == SIGBUS ) eprints ("(SIGBUS)." );
      if (aSig == SIGSEGV) eprints ("(SIGSEGV).");
      eprints ("\n  Please e-mail to \"alliance-support@asim.lip6.fr\".");
      eprints ("\n\n");
      eprintf ("\n  %s terminated ", util_binName);

      if (util_flags & F_NOCORE) {
        eprints ("(core not dumped).\n");
        exit (1);
      }
      else {
        eprints( "(core will be dumped).\n");
        if (   (signal(SIGFPE , SIG_DFL) == SIG_ERR)
            || (signal(SIGBUS , SIG_DFL) == SIG_ERR)
            || (signal(SIGSEGV, SIG_DFL) == SIG_ERR))
          exit (1);
        else {
          kill (getpid(), aSig);
          return;
        }
      }
      break;

    default:
      /* Unexpected signal. */
      eprinth ("util_Sys");
      eprintf ("\n  Unexpected signal \'%d\' in trap function.\n", aSig);
      break;
  }

  exit (1);
}


/*  ------------------------------------------------------------------
 *  Function  :  "trapInit()".
 */

extern void  trapInit()
{
  /* Set the trap function for the ERROR signal. */
  /* if (ssignal (C_SIGERR, trapSig) == SIG_ERR) { trapSig (C_SIGTFLT); } */

  /* Set the trap function for SIGTERM signal. */
  if (signal(SIGTERM, trapSig) == SIG_ERR) { trapSig (C_SIGTFLT); }

  /* Set the trap function for SIGFPE, SIGBUS and SIGSEGV signals. */
  if (   (signal(SIGFPE , trapSig) == SIG_ERR)
      || (signal(SIGBUS , trapSig) == SIG_ERR)
      || (signal(SIGSEGV, trapSig) == SIG_ERR)) trapSig (C_SIGTFLT);
}


/*  ------------------------------------------------------------------
 *  Function  :  "sendGSignal()".
 */

/* extern void  sendGSignal(aSig)
 *   int aSig;
 * {
 *   gsignal (aSig);
 * }
 */


/*  ------------------------------------------------------------------
 *  Function  :  "setBinName()".
 */

extern void  setBinName (asName)
     char *asName;
{
  strncpy (util_binName, asName, 1023);
}



/*  ------------------------------------------------------------------
 *  Function  :  "stdflush()".
 */

extern void  stdflush()
{
  fflush(stdout);
  fflush(stderr);
}


/*  ------------------------------------------------------------------
 *  Function  :  "errMBK()".
 */

extern void  errMBK(asName)
  char *asName;
{
  stdflush();

  fprintf(stderr, "\n*** MBK error ***:");

  if (asName != (char *)NULL) {
    fprintf(stderr, "%s:", asName);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "eprinth()".
 */

extern void  eprinth(asName)
  char *asName;
{
  stdflush();

  fprintf(stderr, "\n*** Error ***:");

  if (asName != (char *)NULL) {
    fprintf(stderr, "%s:", asName);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "eprintf()".
 */

extern int  eprintf __FPV((char *aFormat, ...))
{
          va_list  ArgList;
  __KR_C(    char *aFormat; )
              int  retVal;


  __ANSI_C( va_start(ArgList, aFormat); )
  __KR_C(   va_start(ArgList); )
  __KR_C(   aFormat = va_arg(ArgList, char*); )

  retVal = vfprintf(stderr, aFormat, ArgList);

  va_end(ArgList);

  return(retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "eprints()".
 */

extern int  eprints(aS)
  char *aS;
{
  return(fputs(aS, stderr));
}


/*  ------------------------------------------------------------------
 *  Function  :  "warnMBK()".
 */

extern void  warnMBK(asName)
  char *asName;
{
  stdflush();

  fprintf(stderr, "*** MBK warning ***:");

  if (asName != (char *)NULL) {
    fprintf(stderr, "%s:", asName);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "wprinth()".
 */

extern void  wprinth(asName)
  char *asName;
{
  stdflush();

  fprintf(stderr, "*** Warning ***:");

  if (asName != (char *)NULL) {
    fprintf(stderr, "%s:", asName);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "wprintf()".
 */

extern int  wprintf __FPV((char *aFormat, ...))
{
          va_list  AL;
  __KR_C(    char *aFormat; )
              int  RV;

  __ANSI_C( va_start(AL, aFormat);       )
  __KR_C(   va_start(AL);                )
  __KR_C(   aFormat = va_arg(AL, char*); )

  RV = vfprintf(stderr, aFormat, AL);
  va_end(AL);

  return(RV);
}


/*  ------------------------------------------------------------------
 *  Function  :  "wprints()".
 */

extern int  wprints(aS)
  char *aS;
{
  return(fputs(aS, stderr));
}


/*  ------------------------------------------------------------------
 *  Function  :  "setVL()".
 */

extern void  setVL(aVL)
  long  aVL;
{
  switch (aVL) {
    case C_VerboseLevel0:
    case C_VerboseLevel1:
    case C_VerboseLevel2: util_VL = aVL; break;
    default:              util_VL = C_VerboseLevel2; break;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "getVL()".
 */

extern long  getVL()
{
  return (util_VL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "vmprintf()".
 */

extern int  vmprintf(aVL, aFormat, aArgList)
  va_list  aArgList;
     long  aVL;
     char *aFormat;
{
  int  retVal;


  if (aVL <= util_VL) {
    retVal = vfprintf (stdout, aFormat, aArgList);
  } else {
    retVal = 0;
  }

  return (retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "mprintf()".
 */

extern int  mprintf __FPV((long aVL, char *aFormat, ...))
{
          va_list  ArgList;
  __KR_C(    char  aVL;     )
  __KR_C(    char *aFormat; )
              int  retVal;


  __ANSI_C( va_start (ArgList, aFormat);       )
  __KR_C(   va_start (ArgList);                )
  __KR_C(   aVL     = va_arg (ArgList, long ); )
  __KR_C(   aFormat = va_arg (ArgList, char*); )

  retVal = vmprintf (aVL, aFormat, ArgList);

  va_end(ArgList);

  return(retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "mprints()".
 */

extern int  mprints(aVL, aS)
  long  aVL;
  char *aS;
{
  int  retVal;


  if (aVL <= util_VL) {
    retVal = fputs (aS, stdout);
  } else {
    retVal = 0;
  }

  return (retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "mprintf0()".
 */

extern int  mprintf0 __FPV((char *aFormat, ...))
{
          va_list  ArgList;
  __KR_C(    char *aFormat; )
              int  retVal;


  __ANSI_C( va_start (ArgList, aFormat);       )
  __KR_C(   va_start (ArgList);                )
  __KR_C(   aFormat = va_arg (ArgList, char*); )

  retVal = vmprintf (C_VerboseLevel0, aFormat, ArgList);

  va_end (ArgList);

  return (retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "mprintf1()".
 */

extern int  mprintf1 __FPV((char *aFormat, ...))
{
          va_list  ArgList;
  __KR_C(    char *aFormat; )
              int  retVal;


  __ANSI_C( va_start (ArgList, aFormat);       )
  __KR_C(   va_start (ArgList);                )
  __KR_C(   aFormat = va_arg (ArgList, char*); )

  retVal = vmprintf (C_VerboseLevel1, aFormat, ArgList);

  va_end (ArgList);

  return (retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "mprintf2()".
 */

extern int  mprintf2 __FPV((char *aFormat, ...))
{
          va_list  ArgList;
  __KR_C(    char *aFormat; )
              int  retVal;


  __ANSI_C( va_start (ArgList, aFormat);       )
  __KR_C(   va_start (ArgList);                )
  __KR_C(   aFormat = va_arg (ArgList, char*); )

  retVal = vmprintf (C_VerboseLevel2, aFormat, ArgList);

  va_end (ArgList);

  return (retVal);
}


/*  ------------------------------------------------------------------
 *  Function  :  "util_init()".
 */

extern void  util_init(aVL, aFlags, asName)
  long  aVL;
  long  aFlags;
  char *asName;
{
  util_flags = aFlags;

  trapInit ();
  setVL (aVL);
  setBinName (asName);
}


/*  ------------------------------------------------------------------
 *  Function  :  "s64printf()".
 */

extern char *s64printf __FPV((char *aFormat, ...))
{
          va_list  aAL;
  __KR_C(    char *aFormat; )

  static     char  s64[64][SIZE_S64];
  static     long  iS = 0;


  __ANSI_C( va_start(aAL, aFormat);       )
  __KR_C(   va_start(aAL);                )
  __KR_C(   aFormat = va_arg(aAL, char*); )

  vsprintf(s64[(++iS) % 64] , aFormat, aAL);

  va_end(aAL);

  return(s64[iS % 64]);
}


/*  ------------------------------------------------------------------
 *  Function  :  "strtoup()".
 */

extern char *strtoup (aS)
  char *aS;
{
  char *p;

  for (p = aS; *p != '\0'; p++) *p = (char)toupper ((int)*p);

  return (aS);                              
}
