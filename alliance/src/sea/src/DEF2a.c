/*
 *  $Id: DEF2a.c,v 1.3 2003/06/26 17:00:39 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                            Mael Nagat             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./DEF2a.c"                               |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


#  include  "util_Defs.h"
#  include  "DEF_grammar.h"


/*  ------------------------------------------------------------------
 *  Local constants.
 */

#  define        F_PHFIG              0x00000001
#  define        F_LOFIG              0x00000002
#  define        F_SPLIT_POWER        0x00000004
#  define        F_SHRINK             0x00000008
#  define        F_NO_IOS             0x00000010
#  define        F_NO_INTERF          0x00000020
#  define        F_MERGE_TERM         0x00000040
#  define        F_KEEP_IOS           0x00000080
#  define        M_OUTPUT            (F_PHFIG | F_LOFIG)


/*  ------------------------------------------------------------------
 *  Local variables.
 */

  static     long  LV_VL;          /* Verbose level.           */
  static     long  LV_flags;       


/*  ----------------------------------------------------------------------
 *  Internal functions declarations.
 */

  static   void        printHelp   __FP((void));
  static   void        getlophfig  __FP((lofig_list **appLoFig,
                                         phfig_list **appPhFig,
                                               char  *aDefName,
                                               char  *aNetlayName,
                                               char   aMode,
                                               long   aVL,
                                               long   aFlags));


/*
 *  /--------------------------------------------------------------------\
 *  |                     Functions Definitions                          |
 *  \--------------------------------------------------------------------/
 */

/*  ----------------------------------------------------------------------
 *  Function  :  "printHelp()".
 */

static void  printHelp()
{
  printf ("  o  Usage := \"DEF2a <-l|-p|-l -p>");
  printf (                " [-v] [-V] [-h] [-s] [-b] [-i] [-r] [-k]\n");
  printf ("                 <def> [<net_lay>]\"\n\n");
  printf ("  o  Options :\n");
  printf ("     [-v]          := Be verbose.\n");
  printf ("     [-V]          := Be very verbose.\n");
  printf ("     [-h]          := Print this message.\n");
  printf ("     [-l]          := Drive the logical figure.\n");
  printf ("     [-p]          := Drive the physical figure.\n");
  printf ("     [-b]          := Suppress AB terminals.\n");
  printf ("     [-s]          := Split the power nets.\n");
  printf ("     [-S]          := Shrink the abutment box.\n");
  printf ("     [-i]          := Do not build physical interface.\n");
  printf ("     [-r]          := Merge power terminals for ring.\n");
  printf ("     [-k]          := Keep terminals.\n");
  printf ("     <def>         := Name of the input DEF file (mandatory).\n");
  printf ("     <net_lay>     := Name of the output netlist and/or layout.\n");
  printf ("\n" );
}


/*  ----------------------------------------------------------------------
 *  Function  :  "getlophfig()".
 */

static void  getlophfig (appLoFig,
                         appPhFig,
                         aDefName,
                         aNetlayName,
                         aMode,
                         aVL,
                         aFlags)
  lofig_list **appLoFig;
  phfig_list **appPhFig;
        char  *aDefName;
        char  *aNetlayName;
        char   aMode;
        long   aVL;
        long   aFlags;
{
  (*appLoFig) = addlofig (aNetlayName);
  (*appPhFig) = addphfig (aNetlayName);

  defloadlophfig ((*appLoFig), (*appPhFig), aDefName, aMode, aVL, aFlags);
}



/*  ----------------------------------------------------------------------
 *  Function  :  "main()".
 */

extern int main (argc, argv)
   int  argc;
  char *argv[];
{
  struct phfig *pPhFig;
  struct lofig *pLoFig;
          char *defName;
          char *netlayName;
          long  defFlags;
           int  argC, i;


  /* Read MBK environment. */
  mbkenv();

  argC = argc;

  /* Default options. */
  defFlags = 0L;
  LV_flags = 0L;


  /* Initialise the "Ut" module. */
  util_init (C_VerboseLevel0, F_DUMPCORE, "DEF2a");


  /* Read the options. */
  for (i = 1; i < argc; i++) {
    if (!strcmp (argv[i], "-h"))  { printHelp (); exit (0); }
    if (!strcmp (argv[i], "-v"))  { LV_VL = C_VerboseLevel1; continue; }
    if (!strcmp (argv[i], "-V"))  { LV_VL = C_VerboseLevel2; continue; }
    if (!strcmp (argv[i], "-p"))  { defFlags |= F_PHFIG; continue; }
    if (!strcmp (argv[i], "-l"))  { defFlags |= F_LOFIG; continue; } 
    if (!strcmp (argv[i], "-s"))  { defFlags |= F_SPLIT_POWER; continue; } 
    if (!strcmp (argv[i], "-S"))  { defFlags |= F_SHRINK;
                                    LV_flags |= F_DEF_SHRINK;continue; } 
    if (!strcmp (argv[i], "-b"))  { defFlags |= F_NO_IOS;
                                    LV_flags |= F_DEF_NO_IOS; continue; } 
    if (!strcmp (argv[i], "-i"))  { defFlags |= F_NO_INTERF;
                                    LV_flags |= F_DEF_NO_INTERF; continue; } 
    if (!strcmp (argv[i], "-r"))  { defFlags |= F_MERGE_TERM;
                                    LV_flags |= F_DEF_MERGE_TERM; continue; } 
    if (!strcmp (argv[i], "-k"))  { defFlags |= F_KEEP_IOS;
                                    LV_flags |= F_DEF_KEEP_IOS; continue; } 

    if ((argC - i) > 2) {
      eprinth (NULL);
      eprintf ("Unknown argument \"%s\"\n\n", argv[argC - 1]);
      printHelp ();
      exit (1);
    }
    else
      break;
  }


  if ((argC - i) < 1) {
    eprinth (NULL);
    eprintf ("Missing DEF file name <def>.\n\n");
    printHelp ();
    exit (1);
  } else {
    defName = namealloc (argv[i]);

    if ((argC - i) >= 2)
      netlayName = namealloc (argv[i + 1]);
    else
      netlayName = defName;
  }


  if (!(defFlags & M_OUTPUT)) {
    eprinth ("DEF2a");
    eprints ("Neither <-p> nor <-l> is present.\n\n");
    printHelp ();

    exit (1);
  }


  setVL (LV_VL);

  if (LV_VL > 0) {
    alliancebanner(
      "DEF2a", "V.RR", "Alliance to DEF converter",
      "2000", ALLIANCE_VERSION);
  }

  printMBKEnv ();
  if (defFlags & F_PHFIG) mprintf1 ("  o  Extract physical datas.\n");
  if (defFlags & F_LOFIG) mprintf1 ("  o  Extract logical datas.\n");
  if (defFlags & F_SPLIT_POWER) {
    mprintf1 ("  o  Split the power nets.\n");
    LV_flags |= F_DEF_SPLIT_POWER;
  }


  mprintf1 ("  o  Parsing \"%s\".\n", defName);
  getlophfig (&pLoFig, &pPhFig, defName, netlayName, 'A', LV_VL, LV_flags);

  if ((defFlags & F_SHRINK) && (defFlags & F_PHFIG)) {
    mprintf1 ("  o  Shrinking abutment-box.\n");
    shrinkFloorplan (pPhFig);
  }

  mprintf1 ("  o  Building obstacles.\n");
  rtu (pPhFig);

  if (defFlags & F_PHFIG) {
    mprintf1 ("  o  Driving \"%s\" (physical).\n", netlayName);
    savephfig (pPhFig);
  }

  if (defFlags & F_LOFIG) {
    mprintf1 ("  o  Driving \"%s\" (logical).\n", netlayName);
    savelofig (pLoFig);
  }


  exit(0);
}
