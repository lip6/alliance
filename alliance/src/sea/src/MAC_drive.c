/*
 *  $Id: MAC_drive.c,v 1.3 2003/06/19 16:36:58 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./MAC_drive.c"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "MAC_drive.h"


# define        SIZE_SNAME        1024


/*  ------------------------------------------------------------------
 *  Local variables.
 */

  static              FILE *MAC_FILE;


/*  ------------------------------------------------------------------
 *  Local functions.
 */

  static void  fillTie  __FP((struct phfig *apPhfig));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "fillTie()".
 */

static void  fillTie(apPhfig)
  struct phfig *apPhfig;
{
  /* Fill with spare space with "tie_x0", two pitchs wide with polarization
   * contacts for P bulk and N bulk.
   */
  fprintf (MAC_FILE, " SROUTE ADDCELL\n");
  fprintf (MAC_FILE, "   MODEL tie_x0 PREFIX tiex0\n");
  fprintf (MAC_FILE, "   SPIN vdd NET vdd\n");
  fprintf (MAC_FILE, "   SPIN vss NET vss\n");
  fprintf (MAC_FILE, "   AREA (%ld %ld) (%ld %ld)\n",
                     MBK2DEF_length (apPhfig->XAB1),
                     MBK2DEF_length (apPhfig->YAB1),
                     MBK2DEF_length (apPhfig->XAB2),
                     MBK2DEF_length (apPhfig->YAB2));
  fprintf (MAC_FILE, "   ;\n\n");

  /* Fill with the one pitch cell "rowend_x0". */
  fprintf (MAC_FILE, " SROUTE ADDCELL\n");
  fprintf (MAC_FILE, "   MODEL rowend_x0 PREFIX rowendx0\n");
  fprintf (MAC_FILE, "   SPIN vdd NET vdd\n");
  fprintf (MAC_FILE, "   SPIN vss NET vss\n");
  fprintf (MAC_FILE, "   AREA (%ld %ld) (%ld %ld)\n",
                     MBK2DEF_length (apPhfig->XAB1),
                     MBK2DEF_length (apPhfig->YAB1),
                     MBK2DEF_length (apPhfig->XAB2),
                     MBK2DEF_length (apPhfig->YAB2));
  fprintf (MAC_FILE, "   ;\n\n");
}

/*  ------------------------------------------------------------------
 *  Function  :  "macPlace()".
 */

extern void  macPlace(apPhfig, apLofig, macName, aPower, aFlags)
  struct phfig *apPhfig;
  struct lofig *apLofig;
          char *macName;
          long  aPower;
          long  aFlags;
{
  MAC_FILE = mbkfopen (macName, "mac", "w+");
  if (!MAC_FILE) {
    errMBK  (NULL);
    eprintf ("Can't open SE mac file \"%s.mac\".", macName);
    EXIT (1);
  }


  /* Set a nice viewport. */
  fprintf (MAC_FILE, " SET VARIABLE OUTPUT.LEF.DEF.BUSBITCHARACTERS \"()\" ;\n");
  fprintf (MAC_FILE, " SET VARIABLE OUTPUT.LEF.DEF.DELIMITERS . ;\n\n");
  fprintf (MAC_FILE, " WINDOW FIT ;\n\n");


  if (aFlags & F_MAC_FIXED_PINS) {
    fprintf (MAC_FILE, " IOPLACE\n");
    if (aFlags & F_MAC_IOC) {
      fprintf (MAC_FILE, "   FILENAME %s.ioc\n", apLofig->NAME);
    }
    fprintf (MAC_FILE, "   TOPBOTTOMLAYER L_ALU2\n");
    fprintf (MAC_FILE, "   RIGHTLEFTLAYER L_ALU2\n");
    fprintf (MAC_FILE, "   ;\n\n");
  }


  /* Power stripes made of "powmid_x0". */
  if (aPower > 0) {
    fprintf (MAC_FILE, " SROUTE ADDCELL\n");
    fprintf (MAC_FILE, "   MODEL powmid_x0 PREFIX powmidx0\n");
    fprintf (MAC_FILE, "   SPIN vdd NET vdd\n");
    fprintf (MAC_FILE, "   SPIN vss NET vss\n");
    fprintf (MAC_FILE, "   AREA (%ld %ld) (%ld %ld)\n",
                       MBK2DEF_length (apPhfig->XAB1),
                       MBK2DEF_length (apPhfig->YAB1),
                       MBK2DEF_length (apPhfig->XAB2),
                       MBK2DEF_length (apPhfig->YAB2));
    fprintf (MAC_FILE, "   XNUM %ld\n", aPower);
    fprintf (MAC_FILE, "   ;\n\n");
  }


  /* Placement itself. */
  fprintf (MAC_FILE, " SET VARIABLE Qplace.Place.Pin \"concurrent\" ;\n" );
  fprintf (MAC_FILE, " QPLACE NOCONFIG ;\n\n");


  /* Fill with tie cells. */
  if (aFlags & F_MAC_FILL_TIE) fillTie (apPhfig);


  /* Save the placement file. */
  fprintf (MAC_FILE, " OUTPUT DEF FILENAME %s_p.def ;\n\n", apLofig->NAME);


  /* Fast QUIT : do not save the internal SE data-base. */
  fprintf (MAC_FILE, " FQUIT ;\n");


  fclose (MAC_FILE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "macRoute()".
 */

extern void  macRoute(apPhfig, apLofig, macName, aPower, aFlags)
  struct    phfig *apPhfig;
  struct    lofig *apLofig;
             char *macName;
             long  aPower;
             long  aFlags;
{
  struct eLoseg_s *pLoseg;
  struct    losig *pLosig;
  struct    phseg *pPhseg;
             long  isRouted, isFirst /*, dx */;
             char *sigName;
             char  defSigName[SIZE_SNAME];


  MAC_FILE = mbkfopen (macName, "mac", "w+");
  if (!MAC_FILE) {
    errMBK  (NULL);
    eprintf ("Can't open SE mac file \"%s.mac\".", macName);
    EXIT (1);
  }


  /* Set a nice viewport. */
  fprintf (MAC_FILE, " SET VARIABLE OUTPUT.LEF.DEF.BUSBITCHARACTERS \"()\" ;\n");
  fprintf (MAC_FILE, " SET VARIABLE OUTPUT.LEF.DEF.DELIMITERS . ;\n\n");
  fprintf (MAC_FILE, " WINDOW FIT ;\n\n");


  /* Fill with tie cells. */
  if (aFlags & F_MAC_FILL_TIE) fillTie (apPhfig);


  if (aFlags & F_MAC_FIXED_PINS) {
    fprintf (MAC_FILE, " IOPLACE\n");
    if (aFlags & F_MAC_IOC) {
      fprintf (MAC_FILE, "   FILENAME %s.ioc\n", apLofig->NAME);
    }
    fprintf (MAC_FILE, "   TOPBOTTOMLAYER L_ALU2\n");
    fprintf (MAC_FILE, "   RIGHTLEFTLAYER L_ALU2\n");
    fprintf (MAC_FILE, "   ;\n\n");
  }


  if (! (aFlags & F_MAC_NO_POWER)) {
    /* Special routing : ALU1 horizontal stripes. */
    fprintf (MAC_FILE, " SROUTE FOLLOWPINS\n");
    fprintf (MAC_FILE, "   NET vdd NET vss\n");
    fprintf (MAC_FILE, "   LAYER L_ALU1 WIDTH 600 FILL DIRECTION HORIZONTAL\n");
    fprintf (MAC_FILE, "   AREA (%ld %ld) (%ld %ld)\n",
                       MBK2DEF_length (apPhfig->XAB1),
                       MBK2DEF_length (apPhfig->YAB1),
                       MBK2DEF_length (apPhfig->XAB2),
                       MBK2DEF_length (apPhfig->YAB2));
    fprintf (MAC_FILE, "   ;\n\n");


    /* Special routing : ALU3 vertical stripes. */
    if (findphmodel (apPhfig, namealloc ("powmid_x0"))) {
      fprintf (MAC_FILE, " SROUTE FOLLOWPINS\n");
      fprintf (MAC_FILE, "   NET vdd NET vss\n");
      fprintf (MAC_FILE, "   LAYER L_ALU3 WIDTH 1200 FILL DIRECTION VERTICAL\n");
      fprintf (MAC_FILE, "   AREA (%ld %ld) (%ld %ld)\n",
                         MBK2DEF_length (apPhfig->XAB1),
                         MBK2DEF_length (apPhfig->YAB1),
                         MBK2DEF_length (apPhfig->XAB2),
                         MBK2DEF_length (apPhfig->YAB2));
      fprintf (MAC_FILE, "   ;\n\n");
    }
  }


  /* Global routing of regular nets. */
  fprintf (MAC_FILE, " GROUTE ;\n\n");


  /* Final routing of regular nets. */
  fprintf (MAC_FILE, " FROUTE ");

  isFirst = TRUE;

  for(pLosig = apLofig->LOSIG;
      pLosig != (losig_list*)NULL; pLosig = pLosig->NEXT) {
    sigName = getsigname (pLosig);

    if (isvdd (sigName) || isvss (sigName)) continue;

    if (apPhfig) {
      isRouted = FALSE;
      pLoseg = getloseglist (apPhfig, getsigname (pLosig));

      for (; pLoseg != NULL; pLoseg = pLoseg->next) {
        if (pLoseg->type == LOSEG_SEG) {
          pPhseg = (struct phseg*)(pLoseg->MBKobj);

          /* Is this segment touching the AB. */
          /*if (   (   (pPhseg->X1 == apPhfig->XAB1) 
           *        && (pPhseg->Y1 >  apPhfig->YAB1)
           *        && (pPhseg->Y1 <  apPhfig->YAB2))
           *    || (   (pPhseg->X2 == apPhfig->XAB1) 
           *        && (pPhseg->Y1 >  apPhfig->YAB1)
           *        && (pPhseg->Y1 <  apPhfig->YAB2))
           *    || (   (pPhseg->X1 == apPhfig->XAB2) 
           *        && (pPhseg->Y1 >  apPhfig->YAB1)
           *        && (pPhseg->Y1 <  apPhfig->YAB2))
           *    || (   (pPhseg->X2 == apPhfig->XAB2) 
           *        && (pPhseg->Y1 >  apPhfig->YAB1)
           *        && (pPhseg->Y1 <  apPhfig->YAB2))
           *
           *    || (   (pPhseg->Y1 == apPhfig->YAB1) 
           *        && (pPhseg->X1 >  apPhfig->XAB1)
           *        && (pPhseg->X1 <  apPhfig->XAB2))
           *    || (   (pPhseg->Y2 == apPhfig->YAB1) 
           *        && (pPhseg->X1 >  apPhfig->XAB1)
           *        && (pPhseg->X1 <  apPhfig->XAB2))
           *    || (   (pPhseg->Y1 == apPhfig->YAB2) 
           *        && (pPhseg->X1 >  apPhfig->XAB1)
           *        && (pPhseg->X1 <  apPhfig->XAB2))
           *    || (   (pPhseg->Y2 == apPhfig->YAB2) 
           *        && (pPhseg->X1 >  apPhfig->XAB1)
           *        && (pPhseg->X1 <  apPhfig->XAB2))) {
           *  continue;
           */
            /*
             * dx = 0;
             *
             * switch (pPhseg->TYPE) {
             *   case UP:
             *   case DOWN:  dx = pPhseg->Y2 - pPhseg->Y1; break;
             *   case LEFT:
             *   case RIGHT: dx = pPhseg->X2 - pPhseg->X1; break;
             * }
             *
             * if (dx == MBKSCALE (5)) continue;
             */
          /*} */

          isRouted = TRUE; break;
        }
      }

      if (isRouted) {
        if (!isFirst) {
          fprintf (MAC_FILE, "        ");
        }

        isFirst = FALSE;

        fprintf (MAC_FILE, "NONETS \"%s\"\n",
                           MBK2DEF_name (defSigName, sigName));
      }
    }
  }
  fprintf (MAC_FILE, "   ;\n\n");


  /* Save the routed file. */
  fprintf (MAC_FILE, " OUTPUT DEF FILENAME %s_r.def ;\n\n", apLofig->NAME);
          

  /* Fast QUIT : do not save the internal SE data-base. */
  fprintf (MAC_FILE, " FQUIT ;\n");


  fclose (MAC_FILE);
}
