/*
 *  $Id: sxlib2LEF.c,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./LEF_drive_sxlib.c"                     |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "LEF_drive_sxlib.h"


/*  ----------------------------------------------------------------------
 *  Internal functions declarations.
 */

  static   void       printHelp     __FP((void));


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
  printf("  o  Usage := \"sxlib2lef [options] <source>\"\n\n");
  printf("  o  Options :\n");
  printf("     -v            := Be verbose.\n");
  printf("     -V            := Be very verbose.\n");
  printf("     -h            := Print this message.\n");
  printf("     -o            := Allow offgrid terminals.\n");
  printf("     -p            := Do not generate power PORT geometries.\n");
  printf("\n" );
}


/*  ----------------------------------------------------------------------
 *  Function  :  "main()".
 */

extern int main(argc, argv)
   int  argc;
  char *argv[];
{
  struct lofig *pLofig;
  struct phfig *pPhfig;
          long  VL, sxFlags;
           int  argC, i;


  /* Read MBK environment. */
  mbkenv();

  /* Initialise the "Ut" module. */
  util_init(C_VerboseLevel2, F_DUMPCORE, "sx2LEF");


  argC = argc;

  /* Default options. */
  VL      = C_VerboseLevel0;
  sxFlags = FALSE;

  /* Read the options. */
  for(i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-h")) { printHelp(); exit(0); }
    if (!strcmp(argv[i], "-v")) { VL       = C_VerboseLevel1; continue; }
    if (!strcmp(argv[i], "-V")) { VL       = C_VerboseLevel2; continue; }
    if (!strcmp(argv[i], "-o")) { sxFlags |= F_ALLOW_OFFGRID; continue; }
    if (!strcmp(argv[i], "-p")) { sxFlags |= F_NO_POWER_GEOM; continue; }

    break;
  }

  if ((argC - i) != 1) { printHelp(); exit(1); }


  setVL(VL);

  if (VL >  C_VerboseLevel0)
    alliancebanner(
      "sx2LEF", "V.RR", "SxLib to LEF MACRO converter",
      "2000", ALLIANCE_VERSION );

  printMBKEnv();

  mprintf1 ("  o  Processing cell \"%s\".\n", argv[i]);

  pPhfig  = getphfig(argv[i], 'A');
  pLofig  = getlofig(argv[i], 'P');


  lefsavesxlophfig (pLofig, pPhfig, sxFlags);


  exit(0);
}
