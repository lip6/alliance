/*
 *  $Id: a2DEF.c,v 1.2 2002/09/30 16:21:17 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module    :         "./a2DEF.c"                             |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "DEF_drive.h"
# include  "MAC_drive.h"


/*  ------------------------------------------------------------------
 *  Local constants.
 */

#  define        A_PHFIG              0x00000001  /*     1 */
#  define        A_FIXED_PINS         0x00000002  /*     2 */
#  define        A_PAD                0x00000004  /*     4 */
#  define        A_LAYERS_3           0x00000008  /*     8 */
#  define        A_LAYERS_4           0x00000010  /*     8 */
#  define        A_LAYERS_6           0x00000020  /*    16 */
#  define        A_FLOORPLAN          0x00000040  /*    32 */
#  define        A_MARGIN             0x00000080  /*    64 */
#  define        A_ROW_NUMBER         0x00000100  /*   128 */
#  define        A_X_SIZE             0x00000200  /*   256 */
#  define        A_Y_SIZE             0x00000400  /*   512 */
#  define        A_ASPECT_RATIO       0x00000800  /*  1024 */
#  define        A_POWER              0x00001000  /*  2048 */
#  define        A_EXPAND_PLACE       0x00002000  /*  4096 */
#  define        A_EXPAND_ROUTE       0x00004000  /*  4096 */
#  define        A_MAC_PLACE          0x00008000  /*  8192 */
#  define        A_MAC_ROUTE          0x00010000  /* 16384 */
#  define        A_MAC_IOC            0x00020000  /* 16384 */
#  define        A_MAC_NO_POWER       0x00040000  /* 16384 */
#  define        A_MAC_FILL_TIE       0x00080000  /* 16384 */
#  define        A_TRUST_ORIENT       0x00100000  /* 16384 */


/*  ----------------------------------------------------------------------
 *  Local functions.
 */

static   void       printHelp        __FP((void));


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
  printf ("  o  Usage := \"a2def [-h] [-v] [-V]\n");
  printf ("                      [-b] [-c] [-P] [-3] [-6] [-e] [-E]\n");
  printf ("                      [--mac-place=<MACPLACE>]\n");
  printf ("                      [--mac-route=<MACROUTE>]\n");
  printf ("                      [--ioc] [-n] [-t]\n");
  printf ("                      [-F [--margin=<MARGIN>  |\n");
  printf ("                           --row-number=<NR>  |\n");
  printf ("                           --x-size=<XSIZE>   |\n");
  printf ("                           --y-size=<YSIZE>   |\n");
  printf ("                           --aspect-ratio=<FF>|\n");
  printf ("                           --power=<NP>       ]]\n");
  printf ("                      [--place=<LAYOUT>]\n");
  printf ("                       <netlist> [<def>]\"\n\n");
  printf ("  o  Options :\n");
  printf ("     [-v]          := Be verbose.\n");
  printf ("     [-V]          := Be very verbose.\n");
  printf ("     [-h]          := Print this message.\n");
  printf ("     [-b]          := Do not put terminals on the abutment box.\n");
  printf ("     [-c]          := Put terminals on the abutment box.\n");
  printf ("     [-P]          := The design have pads.\n");
  printf ("     [-3]          := Uses only 3 levels of metal.\n");
  printf ("     [-6]          := Uses all 6 levels of metal.\n");
  printf ("     [-e]          := Expand AB of the physical figure (place).\n");
  printf ("     [-E]          := Expand AB of the physical figure (route).\n");
  printf ("     [-F]          := Generate a floorplan.\n");
  printf ("     [-n]          := Do not route power supplies (in .mac).\n");
  printf ("     [-T]          := Trust MBK instance orientations.\n");
  printf ("     [-t]          := Fill the remaining space with tie (in .mac).\n");
  printf ("     [--ioc]       := Terminal placement file, used by the mac\n");
  printf ("                      generator.\n");
  printf ("     [--mac-place=<MACPLACE>] :=\n");
  printf ("                      Generate a Silicon Ensemble script (.mac)\n");
  printf ("                      for the placement step.\n");
  printf ("     [--mac-route=<MACROUTE>] :=\n");
  printf ("                      Generate a Silicon Ensemble script (.mac)\n");
  printf ("                      for the routing step.\n");
  printf ("     [--margin=<MARGIN>] :=\n");
  printf ("                      Set the free area in percentage of the\n");
  printf ("                      cell area. The resulting area will be\n");
  printf ("                      equal to CELL_AREA * (1 + <MARGIN>/100).\n");
  printf ("                      Default value 10.\n");
  printf ("     [--row-number=<NR>] :=\n");
  printf ("                      Set the preferred number of rows of the\n");
  printf ("                      design to <NR>. The width is computed.\n");
  printf ("     [--x-size=<XSIZE>] :=\n");
  printf ("                      Set the preferred width of the design to\n");
  printf ("                      <XSIZE>. <XSIZE> is rounded to the pitch\n");
  printf ("                      (currently 5 lambdas). The height is\n");
  printf ("                      computed.\n");
  printf ("     [--y-size=<YSIZE>] :=\n");
  printf ("                      Set the preferred height of the design to\n");
  printf ("                      <YSIZE>. <YSIZE> is rounded to the pitch\n");
  printf ("                      (currently 50 lambdas). The width is\n");
  printf ("                      computed.\n");
  printf ("     [--aspect-ratio=<FF>] :=\n");
  printf ("                      Set the preferred form factor of the design\n");
  printf ("                      to <FF>.\n");
  printf ("     [--power=<NP>] :=\n");
  printf ("                      Set the number of vertical power rail to <NR>\n");
  printf ("                      (default : 1).\n");
  printf ("     [--place=<LAYOUT>] :=\n");
  printf ("                      The Design is preplaced/prerouted.\n");
  printf ("     <netlist>         := The input netlist (mandatory).\n");
  printf ("     <def>             := The output DEF file.\n");
  printf ("\n" );
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
  struct loins *pLoins;
          char *netlistName, *layoutName, *defName;
           int  argC, i, optCount;
          long  VL, argFlags, defFlags, floFlags, macFlags;
          long  margin, xSize, ySize, power;
        double  formFactor;
          char  macPlaceName[1024], macRouteName[1024];


  /* Read MBK environment. */
  mbkenv ();
  autenv ();

  /* Initialise the "util" module. */
  util_init (C_VerboseLevel2, F_DUMPCORE, "a2DEF");


  optCount = 0;
  argC     = argc;
  pPhfig   = NULL;

  /* Default options. */
  layoutName = NULL;
  defName    = NULL;
  VL         = C_VerboseLevel0;
  defFlags   = F_LAYERS_6;
  floFlags   = F_FLOOR_ASPECT_RATIO;
  argFlags   = 0L;
  macFlags   = 0L;
  margin     = 10L;
  xSize      = 0L;
  ySize      = 0L;
  formFactor = 1.0;
  power      = 1;

  /* Read the options. */
  for(i = 1; i < argc; i++) {
    /*
     * fflush (stdout);
     * fprintf (stderr, "argFlags := %ld (%s)\n", argFlags, argv[i]);
     */
    if (!strcmp (argv[i], "-h")) { printHelp (); exit (0); }
    if (!strcmp (argv[i], "-v")) { VL = C_VerboseLevel1; continue; }
    if (!strcmp (argv[i], "-V")) { VL = C_VerboseLevel2; continue; }
    if (!strcmp (argv[i], "-e")) { argFlags |= A_EXPAND_PLACE;
                                   defFlags |= F_EXPAND_PLACE;
                                   continue; }
    if (!strcmp (argv[i], "-E")) { argFlags |= A_EXPAND_ROUTE;
                                   defFlags |= F_EXPAND_ROUTE;
                                   continue; }

    /* Short options. */
    if (!strcmp (argv[i], "-c")) {
      argFlags |=  A_FIXED_PINS;
      defFlags |=  F_FIXED_PINS;
      /*macFlags |=  F_MAC_FIXED_PINS;*/
      continue;
    }

    if (!strcmp (argv[i], "-b")) {
      argFlags &= ~A_FIXED_PINS;
      defFlags &= ~F_FIXED_PINS;
      continue;
    }

    if (!strcmp (argv[i], "-p")) {
      continue;
    } 
    if (!strncmp (argv[i], "--place=", 8)) {
      argFlags |= A_PHFIG;
      defFlags |= F_PHFIG;
      power = 0;
      layoutName = namealloc (argv[i] + 8);
      continue;
    }

    if (!strcmp (argv[i], "-P")) {
      argFlags |=  A_PAD;
      argFlags &= ~A_FIXED_PINS;
      defFlags |=  F_PAD;
      defFlags &= ~F_FIXED_PINS;
      continue;
    } 

    if (!strcmp (argv[i], "-3")) {
      argFlags |=  A_LAYERS_3;
      argFlags &= ~A_LAYERS_4;
      argFlags &= ~A_LAYERS_6;
      defFlags |=  F_LAYERS_3;
      defFlags &= ~F_LAYERS_4;
      defFlags &= ~F_LAYERS_6;
      floFlags |=  F_FLOOR_LAYERS_3;
      floFlags &= ~F_FLOOR_LAYERS_4;
      floFlags &= ~F_FLOOR_LAYERS_6;
      continue;
    } 

    if (!strcmp (argv[i], "-4")) {
      argFlags |=  A_LAYERS_4;
      argFlags &= ~A_LAYERS_3;
      argFlags &= ~A_LAYERS_6;
      defFlags |=  F_LAYERS_4;
      defFlags &= ~F_LAYERS_3;
      defFlags &= ~F_LAYERS_6;
      floFlags |=  F_FLOOR_LAYERS_4;
      floFlags &= ~F_FLOOR_LAYERS_3;
      floFlags &= ~F_FLOOR_LAYERS_6;
      continue;
    } 

    if (!strcmp (argv[i], "-6")) {
      argFlags |=  A_LAYERS_6;
      argFlags &= ~A_LAYERS_3;
      defFlags |=  F_LAYERS_6;
      defFlags &= ~F_LAYERS_3;
      floFlags |=  F_FLOOR_LAYERS_6;
      floFlags &= ~F_FLOOR_LAYERS_3;
      continue;
    } 

    if (!strcmp (argv[i], "--ioc")) {
      argFlags |= A_MAC_IOC;
      macFlags |= F_MAC_IOC;
      continue;
    }

    if (!strcmp (argv[i], "-n")) {
      argFlags |= A_MAC_NO_POWER;
      macFlags |= F_MAC_NO_POWER;
      continue;
    }

    if (!strcmp (argv[i], "-t")) {
      argFlags |= A_MAC_FILL_TIE;
      macFlags |= F_MAC_FILL_TIE;
      continue;
    }

    if (!strcmp (argv[i], "-T")) {
      argFlags |= A_TRUST_ORIENT;
      defFlags |= F_TRUST_ORIENT;
      continue;
    }

    if (!strncmp (argv[i], "--mac-place=", 12)) {
      argFlags |= A_MAC_PLACE;
      strcpy (macPlaceName, argv[i] + 12);
      continue;
    }

    if (!strncmp (argv[i], "--mac-route=", 12)) {
      argFlags |= A_MAC_ROUTE;
      strcpy (macRouteName, argv[i] + 12);
      continue;
    }


    if (!strcmp (argv[i], "-F")) { argFlags |= A_FLOORPLAN; continue; } 

    /* Long options : associated with floorplan. */
    if (!strncmp (argv[i], "--margin=", 9)) {
      argFlags |= A_MARGIN;
      if (1 != sscanf (argv[i] + 9, "%ld", &margin)) {
        eprinth (NULL);
        eprintf ("Invalid <MARGIN> value.\n");
        printHelp ();
        exit (2);
      }
      if (margin < 0) {
        eprinth (NULL);
        eprintf ("Negative <MARGIN> value (%ld).\n", margin);
        printHelp ();
        exit (2);
      }
      continue;
    }

    if (!strncmp (argv[i], "--row-number=", 13)) {
      optCount +=  1;
      argFlags |=  A_ROW_NUMBER;
      floFlags |=  F_FLOOR_Y_SIZE;
      floFlags &= ~F_FLOOR_ASPECT_RATIO;
      if (1 != sscanf (argv[i] + 13, "%ld", &ySize)) {
        eprinth (NULL);
        eprintf ("Invalid <NR> value.\n\n");
        printHelp ();
        exit (2);
      }
      if (ySize <= 0) {
        eprinth (NULL);
        eprintf ("Negative or null <NR> value (%ld).\n", margin);
        printHelp ();
        exit (2);
      }
      continue;
    }

    if (!strncmp (argv[i], "--y-size=", 9)) {
      optCount +=  1;
      argFlags |=  A_Y_SIZE;
      floFlags |=  F_FLOOR_Y_SIZE;
      floFlags &= ~F_FLOOR_ASPECT_RATIO;
      if (1 != sscanf (argv[i] + 9, "%ld", &ySize)) {
        eprinth (NULL);
        eprintf ("Invalid <YSIZE> value.\n\n");
        printHelp ();
        exit (2);
      }
      if (ySize <= 0) {
        eprinth (NULL);
        eprintf ("Negative or null <YSIZE> value (%ld).\n", ySize);
        printHelp ();
        exit (2);
      }
      ySize /= Y_SLICE;
      continue;
    }

    if (!strncmp (argv[i], "--x-size=", 9)) {
      optCount +=  1;
      argFlags |=  A_X_SIZE;
      floFlags |=  F_FLOOR_X_SIZE;
      floFlags &= ~F_FLOOR_ASPECT_RATIO;
      if (1 != sscanf (argv[i] + 9, "%ld", &xSize)) {
        eprinth (NULL);
        eprintf ("Invalid <XSIZE> value.\n\n");
        printHelp ();
        exit (2);
      }
      if (xSize <= 0) {
        eprinth (NULL);
        eprintf ("Negative or null <XSIZE> value (%ld).\n", xSize);
        printHelp ();
        exit (2);
      }
      xSize /= X_GRID;
      continue;
    }

    if (!strncmp (argv[i], "--aspect-ratio=", 15)) {
      optCount += 1;
      argFlags |=  A_ASPECT_RATIO;
      floFlags |=  F_FLOOR_ASPECT_RATIO;
      if (1 != sscanf (argv[i] + 15, "%lf", &formFactor)) {
        eprinth (NULL);
        eprintf ("Invalid <FF> value.\n\n");
        printHelp ();
        exit (2);
      }
      if (formFactor <= 0.0) {
        eprinth (NULL);
        eprintf ("Negative or null <FF> value (%f).\n", formFactor);
        printHelp ();
        exit (2);
      }
      continue;
    }

    if (!strncmp (argv[i], "--power=", 8)) {
      argFlags |= A_POWER;
      if (1 != sscanf (argv[i] + 8, "%ld", &power)) {
        eprinth (NULL);
        eprintf ("Invalid <NP> value.\n\n");
        printHelp ();
        exit (2);
      }
      if (power < 0) {
        eprinth (NULL);
        eprintf ("Negative <NP> value (%ld).\n", power);
        printHelp ();
        exit (2);
      }
      continue;
    }

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
    eprintf ("Missing netlist name <netlist>.\n\n");
    printHelp ();
    exit (1);
  } else {
    netlistName = namealloc (argv[i]);

    if ((argC - i) == 2)
      defName = namealloc (argv[i + 1]);
    else
      defName = netlistName;
  }


  if (argFlags & A_FLOORPLAN) {
    if (argFlags & A_PHFIG) {
      eprinth (NULL);
      eprintf ("\n  Options -F and -p are mutually exclusive : either you");
      eprintf (" want to make\n  a floorplan or you supply a placement file.");
      eprintf ("\n");
      exit (1);
    }

    if (optCount > 1) {
      eprinth (NULL);
      eprintf ("\n  Options --row-number, --x-size, --y-size and");
      eprintf (" --aspect-ratio\n");
      eprintf (  "  are mutually exclusive.\n)");
      exit (1);
    }

    if (!optCount) argFlags |= A_ASPECT_RATIO;
  } else {
    if (argFlags & A_MARGIN) {
      wprinth (NULL);
      wprintf (" ignored --margin argument, must be used with -F\n");
    }

    if (argFlags & A_X_SIZE) {
      wprinth (NULL);
      wprintf (" ignored --x-size argument, must be used with -F\n");
    }

    if (argFlags & A_Y_SIZE) {
      wprinth (NULL);
      wprintf (" ignored --y-size argument, must be used with -F\n");
    }

    if (argFlags & A_ASPECT_RATIO) {
      wprinth (NULL);
      wprintf (" ignored --aspect-ratio argument, must be used with -F\n");
    }

    if ((argFlags & A_EXPAND_PLACE) && !(argFlags & A_PHFIG)) {
      wprinth (NULL);
      wprintf (" ignored -e argument, no physical layout.\n");
    }

    if ((argFlags & A_EXPAND_ROUTE) && !(argFlags & A_PHFIG)) {
      wprinth (NULL);
      wprintf (" ignored -E argument, no physical layout.\n");
    }
  }


  setVL (VL);

  if (VL >  C_VerboseLevel0)
    alliancebanner(
      "a2DEF", "V.RR", "Alliance to DEF converter",
      "2000", ALLIANCE_VERSION );

  printMBKEnv ();
  if (argFlags & A_PAD)   mprintf1 ("  o  Forced pad design.\n");
  if (argFlags & A_PHFIG) mprintf1 ("  o  Preplaced/prerouted design.\n");


  mprintf2 ("  o  Loading \"%s\" (logical).\n", netlistName);

  pLofig = getlofig (netlistName, 'A');

  if (defFlags & F_PHFIG) {
    mprintf2 ("  o  Loading \"%s\" (physical).\n", layoutName);
    pPhfig  = getphfig (layoutName, 'A');
  } else
    pPhfig  =  (phfig_list *)NULL;


  mprintf2 ("  o  Flattening \"%s\" to the catalog.\n", netlistName);
  mprintf2 ("     (using MBK_SEPAR := \'%c\' as separator)\n", SEPAR);
  if (defFlags & F_PHFIG)
    rflattenphfig (pPhfig, YES, YES);
  rflattenlofig (pLofig, YES, YES);


  /* Build a floorplan. */
  if (argFlags & A_FLOORPLAN) {
    mprintf2 ("  o  Making floorplan.\n");

    pPhfig = makeFloorplan (pLofig,
                            margin,
                            xSize,
                            ySize,
                            formFactor,
                            power,
                            floFlags);

    if (layoutName) pPhfig->NAME = layoutName;

    /*savephfig (pPhfig);*/
  }


  /* Add ROWS & TRACKS references, if needed. */
  if ((argFlags & A_PHFIG) && (!hasROW (pPhfig))) {
    mprintf2 ("  o  Building default ROWS and TRACKS.\n");

    buildROWS (pPhfig);
    buildTRACKS (pPhfig, defFlags);
  }


  /* Place terminals. */
  mprintf2 ("  o  Placing terminals.\n");
  /* placeTerms (pLofig, pPhfig); */


  if ((   (argFlags & A_PHFIG)
       || (argFlags & A_FLOORPLAN)) && (   (argFlags & A_EXPAND_PLACE)
                                        || (argFlags & A_EXPAND_ROUTE))) {
    mprintf2 ("  o  Expanding the abutment box.\n");

    expandFloorplan (pPhfig);
    /* savephfig (pPhfig); */
  }


  /* Try to guess if the design have pads. */
  for (pLoins = pLofig->LOINS; pLoins != NULL; pLoins = pLoins->NEXT) {
    if (isPad (pLoins->FIGNAME)) {
      defFlags |=  F_PAD;
      defFlags &= ~F_FIXED_PINS;

      mprintf1 ("  o  Pads found in this design (%s).\n",
                pLoins->FIGNAME);
      break;
    }
  }


  /* Generate a separate "vdd/vss" for the core and the pads. */
  lofigchain (pLofig);
  initSigIndex (pLofig->LOSIG);
  if (defFlags & F_PAD) {
    splitPowerNet (pLofig, namealloc("vdd"));
    splitPowerNet (pLofig, namealloc("vss"));
    lofigchain (pLofig);
  }


  /* Build the loseg list. */
  if (defFlags & F_PHFIG) {
    phsegchain (pLofig, pPhfig);
  }


  /* Prevent SE to put terminal on power cells. */
  if (pPhfig) protectPowmid (pPhfig);


  /* Drive the DEF file. */
  defsavelophfig (pLofig, pPhfig, defName, defFlags);


  /* Drive ".mac" files, if any. */
  if (argFlags & A_MAC_PLACE) {
    if (pPhfig)
      macPlace (pPhfig, pLofig, macPlaceName, power, macFlags);
    else {
      eprinth (NULL);
      eprintf (" --mac-place=<MACPLACE> option must be given with");
      eprintf (" [--place] or [-F].\n");
      exit (2);
    }
  }
  if (argFlags & A_MAC_ROUTE) {
    if (pPhfig)
      macRoute (pPhfig, pLofig, macRouteName, power, macFlags);
    else {
      eprinth (NULL);
      eprintf (" --mac-route=<MACPLACE> option must be given with");
      eprintf (" [-place] or [-F].\n");
      exit (2);
    }
  }


  /* free the loseg list. */
  if (defFlags & F_PHFIG) {
    freeloseg (pPhfig);
  }


  exit(0);
}
