
/*
 *  $Id: util_MBK.c,v 1.1 2002/04/25 16:16:20 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_MBK.c"                            |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"


# define        C_POWER_VDD           (0)
# define        C_POWER_VSS           (1)
# define        C_POWER_UNKNOWN    (1024)

# define        TLOSEG_SIZE         (512)

# define        SIZE_SNAME         (1024)


/*  ------------------------------------------------------------------
 *  Internal types.
 */

  typedef struct sigalias_s {
    char *name;   /* The main signal name (getsigname()). */
    long  flags;  /* The subnet field. */
  } sigalias_t;



/*  ------------------------------------------------------------------
 *  Global variables (declared `extern' in "util_Defs.h").
 */

       long  MBK_sigIndex = 0L;


/*  ------------------------------------------------------------------
 *  Local variables.
 */

  static authtable         *LV_htSigAlias = NULL;
  static struct sigalias_s *LV_tSigAlias  = NULL;


/*  ------------------------------------------------------------------
 *  Local functions declarations.
 */

  static void             buildtsigalias  __FP((struct losig *apLosig));
  static void             freetsigalias   __FP((void));

  static char            *getlosegname    __FP((struct eLoseg_s *apLoseg));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "isobs()".
 */

extern long  isobs(apSeg)
  struct phseg *apSeg;
{
  switch (apSeg->LAYER) {
    case TALU1:
    case TALU2:
    case TALU3:
    case TALU4:
    case TALU5:
    case TALU6:
    case TALU7:
    case TALU8:
    case TALU9:
      return (TRUE);
  }

  return (FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getsigaliasnumber()".
 */

extern long  getsigaliasnumber(apLosig)
  struct losig *apLosig;
{
  struct chain *pChain;
          long  aliasNumber;


  aliasNumber = 0;

  for (pChain = apLosig->NAMECHAIN; pChain != NULL; pChain = pChain->NEXT) {
    aliasNumber++;
  }

  return (aliasNumber - 1);
}


/*  ------------------------------------------------------------------
 *  Function  :  "signamecmp()".
 */

extern long  signamecmp(aLoname, aPhname)
  char *aLoname, *aPhname;
{
  char *pS, *pVector;
  char  losigName[SIZE_SNAME];


  if (aLoname == aPhname) return (TRUE);

  /* Try to guess if the losig name was a vector name. */
  strcpy (losigName, aLoname);

  pVector = NULL;
  for (pS = losigName; pS != '\0'; pS++)
    if (*pS == '_') pVector = pS;

  if (!pVector) return (FALSE);

  for (pS = pVector + 1; pS != '\0'; pS++)
    if (!isdigit ((int)*pS)) return (FALSE);

  *pVector = ' ';

  if (!strcmp (losigName, aPhname)) return (TRUE);

  return (FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "makesegnamelist()".
 */

extern char **makesegnamelist(aName)
  char *aName;
{
  static char *tSegAlias[1024];
         long  mSegAlias;
         char *pS, *pVector;
         char  losegName[SIZE_SNAME];
# if 0
         char  pathName[SIZE_SNAME];
# endif
         long  iEnd;


  mSegAlias = 0;
  strcpy (losegName, aName);
  iEnd = strlen (losegName) - 1;


  /* Add the signal name "as is". */
  tSegAlias[mSegAlias++] = namealloc (losegName);

  /* Try to find if it is a vector. */
  pVector = NULL;
  for (pS = losegName + iEnd; pS != losegName; pS--) {
    if (*pS == ' ') {
      *pS = '_';
      tSegAlias[mSegAlias++] = namealloc (losegName);
    }
  }


# if  0
  /* This code is no longer needed, for we. */
  /* Loop on the path name, using SEPAR. */
  for (; iEnd > 0; iEnd--) {
    if (losegName[iEnd] == SEPAR) {
      strncpy (pathName, losegName, iEnd);
      pathName[iEnd] = '\0';

      /* Add the path name "as is". */
      tSegAlias[mSegAlias++] = pathName;

      /* Try to find if it is a vector. */
      pVector = NULL;
      for (pS = pathName + iEnd; pS != pathName; pS--) {
        if (*pS == ' ') {
          *pS = '_';
          tSegAlias[mSegAlias++] = pathName;
        }
      }
    }
  } /* End of "iEnd" loop. */
# endif


  tSegAlias[mSegAlias++] = NULL;

  return (tSegAlias);
}


/*  ------------------------------------------------------------------
 *  Function  :  "printMBKEnv()".
 */

extern void  printMBKEnv()
{
   int  i;

  mprintf2 ("  o  MBK environment :\n\n" );
  mprintf2 ("     MBK_IN_LO       := %s\n", IN_LO   );
  mprintf2 ("     MBK_OUT_LO      := %s\n", OUT_LO  );
  mprintf2 ("     MBK_IN_PH       := %s\n", IN_PH   );
  mprintf2 ("     MBK_OUT_PH      := %s\n", OUT_PH  );
  mprintf2 ("     MBK_WORK_LIB    := %s\n", WORK_LIB);

  for(i = 0; CATA_LIB[i] != (char *)NULL; i++) {
    if (i == 0) mprintf2 ("     MBK_CATA_LIB    := ");
    else        mprintf2 ("                        ");
    mprintf2 ("%s\n", CATA_LIB[i]);
  }

  mprintf2 ("     MBK_CATAL_NAME  := %s\n", CATAL);
  mprintf2 ("     MBK_VDD         := %s\n", VDD  );
  mprintf2 ("     MBK_VSS         := %s\n", VSS  );
  mprintf2 ("     MBK_SEPAR       := %c\n", SEPAR);
  mprintf2 ("\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "getAltVIALayer()".
 */

extern char  getAltVIALayer(aVIA, aLayer)
  char  aVIA, aLayer;
{
  char  topLayer, bottomLayer, topCLayer, bottomCLayer, dualLayer;


  topLayer     =  ALU1;
  bottomLayer  =  ALU2;
  topCLayer    = CALU1;
  bottomCLayer = CALU2;

  switch (aVIA) {
    case CONT_POLY:
      topLayer  =  ALU1; bottomLayer  = POLY;
      topCLayer = CALU1; bottomCLayer = POLY;
      break;
    case CONT_VIA:
      topLayer  =  ALU2; bottomLayer  =  ALU1;
      topCLayer = CALU2; bottomCLayer = CALU1;
      break;
    case CONT_VIA2:
      topLayer =   ALU3; bottomLayer  =  ALU2;
      topCLayer = CALU3; bottomCLayer = CALU2;
      break;
    case CONT_VIA3:
      topLayer   = ALU4; bottomLayer  =  ALU3;
      topCLayer = CALU4; bottomCLayer = CALU3;
      break;
    case CONT_VIA4:
      topLayer  =  ALU5; bottomLayer  =  ALU4;
      topCLayer = CALU5; bottomCLayer = CALU4;
      break;
    case CONT_VIA5:
      topLayer  =  ALU6; bottomLayer  =  ALU5;
      topCLayer = CALU6; bottomCLayer = CALU5;
      break;
    case CONT_VIA6:
      topLayer  =  ALU7; bottomLayer  =  ALU6;
      topCLayer = CALU7; bottomCLayer = CALU6;
      break;
    case CONT_VIA7:
      topLayer  =  ALU8; bottomLayer  =  ALU7;
      topCLayer = CALU8; bottomCLayer = CALU7;
      break;
    case CONT_VIA8:
      topLayer  =  ALU9; bottomLayer  =  ALU8;
      topCLayer = CALU9; bottomCLayer = CALU8;
      break;
    case CONT_TURN1:
      topLayer  =  ALU1; bottomLayer  =  ALU1;
      topCLayer = CALU1; bottomCLayer = CALU1;
      break;
    case CONT_TURN2:
      topLayer  =  ALU2; bottomLayer  =  ALU2;
      topCLayer = CALU2; bottomCLayer = CALU2;
      break;
    case CONT_TURN3:
      topLayer  =  ALU3; bottomLayer  =  ALU3;
      topCLayer = CALU3; bottomCLayer = CALU3;
      break;
    case CONT_TURN4:
      topLayer  =  ALU4; bottomLayer  =  ALU4;
      topCLayer = CALU4; bottomCLayer = CALU4;
      break;
    case CONT_TURN5:
      topLayer  =  ALU5; bottomLayer  =  ALU5;
      topCLayer = CALU5; bottomCLayer = CALU5;
      break;
    case CONT_TURN6:
      topLayer  =  ALU6; bottomLayer  =  ALU6;
      topCLayer = CALU6; bottomCLayer = CALU6;
      break;
    case CONT_TURN7:
      topLayer  =  ALU7; bottomLayer  =  ALU7;
      topCLayer = CALU7; bottomCLayer = CALU7;
      break;
    case CONT_TURN8:
      topLayer  =  ALU8; bottomLayer  =  ALU8;
      topCLayer = CALU8; bottomCLayer = CALU8;
      break;
    case CONT_TURN9:
      topLayer  =  ALU9; bottomLayer  =  ALU9;
      topCLayer = CALU9; bottomCLayer = CALU9;
      break;
    default:
      errMBK  ("getAltVIALayer");
      eprintf ("Unknown VIA id %d.\n", (int)aVIA);
      EXIT (1);
  }

  
  /* ALU connectivity. */
  dualLayer =  (aLayer == topLayer) ? bottomLayer :
              ((aLayer == bottomLayer) ? topLayer : (LAST_LAYER + 1));

  /* CALU connectivity. */
  if (dualLayer == LAST_LAYER + 1) {
    __DBG(
      fprintf (stderr, "This is a CALU %d %d | ", (int)aVIA, (int)aLayer);
      fprintf (stderr, "topC = %d, bottomC = %d, ", (int)topCLayer,
               (int)bottomCLayer);
    )
    dualLayer =  (aLayer == topCLayer) ? bottomCLayer :
                ((aLayer == bottomCLayer) ? topCLayer : (LAST_LAYER + 1));
    __DBG( fprintf (stderr, "Layer is now %d\n", (int)dualLayer); )
  }

  if (dualLayer == LAST_LAYER + 1) {
    errMBK  ("getAltVIALayer");
    eprintf ("VIA id %d is not contiguous with layer id %d\n",
             (int)aVIA, (int)aLayer);
    EXIT (1);
  }

  return (dualLayer);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getTopVIALayer()".
 */

extern char  getTopVIALayer(aVIA)
  char  aVIA;
{
  char  topLayer;


  topLayer = ALU1;

  switch (aVIA) {
    case CONT_BODY_N: topLayer = ALU1; break;
    case CONT_BODY_P: topLayer = ALU1; break;
    case CONT_DIF_N:  topLayer = ALU1; break;
    case CONT_DIF_P:  topLayer = ALU1; break;
    case CONT_POLY:   topLayer = ALU1; break;
    case CONT_POLY2:  topLayer = ALU1; break;
    case CONT_VIA:    topLayer = ALU2; break;
    case CONT_VIA2:   topLayer = ALU3; break;
    case CONT_VIA3:   topLayer = ALU4; break;
    case CONT_VIA4:   topLayer = ALU5; break;
    case CONT_VIA5:   topLayer = ALU6; break;
    case CONT_VIA6:   topLayer = ALU7; break;
    case CONT_VIA7:   topLayer = ALU8; break;
    case CONT_VIA8:   topLayer = ALU9; break;
    case CONT_TURN1:  topLayer = ALU1; break;
    case CONT_TURN2:  topLayer = ALU2; break;
    case CONT_TURN3:  topLayer = ALU3; break;
    case CONT_TURN4:  topLayer = ALU4; break;
    case CONT_TURN5:  topLayer = ALU5; break;
    case CONT_TURN6:  topLayer = ALU6; break;
    case CONT_TURN7:  topLayer = ALU7; break;
    case CONT_TURN8:  topLayer = ALU8; break;
    case CONT_TURN9:  topLayer = ALU9; break;
    default:
      errMBK  ("getTopVIALayer");
      eprintf ("Unknown VIA id %d.\n", (int)aVIA);
      EXIT (1);
  }


  return (topLayer);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getBottomVIALayer()".
 */

extern char  getBottomVIALayer(aVIA)
  char  aVIA;
{
  char  bottomLayer;


  bottomLayer = ALU1;

  switch (aVIA) {
    case CONT_BODY_N: bottomLayer = NWELL; break;
    case CONT_BODY_P: bottomLayer = PWELL; break;
    case CONT_DIF_N:  bottomLayer = NDIF; break;
    case CONT_DIF_P:  bottomLayer = PDIF; break;
    case CONT_POLY:   bottomLayer = POLY; break;
    case CONT_POLY2:  bottomLayer = POLY2; break;
    case CONT_VIA:    bottomLayer = ALU1; break;
    case CONT_VIA2:   bottomLayer = ALU2; break;
    case CONT_VIA3:   bottomLayer = ALU3; break;
    case CONT_VIA4:   bottomLayer = ALU4; break;
    case CONT_VIA5:   bottomLayer = ALU5; break;
    case CONT_VIA6:   bottomLayer = ALU6; break;
    case CONT_VIA7:   bottomLayer = ALU7; break;
    case CONT_VIA8:   bottomLayer = ALU8; break;
    case CONT_TURN1:  bottomLayer = ALU1; break;
    case CONT_TURN2:  bottomLayer = ALU2; break;
    case CONT_TURN3:  bottomLayer = ALU3; break;
    case CONT_TURN4:  bottomLayer = ALU4; break;
    case CONT_TURN5:  bottomLayer = ALU5; break;
    case CONT_TURN6:  bottomLayer = ALU6; break;
    case CONT_TURN7:  bottomLayer = ALU7; break;
    case CONT_TURN8:  bottomLayer = ALU8; break;
    case CONT_TURN9:  bottomLayer = ALU9; break;
    default:
      errMBK  ("getBottomVIALayer");
      eprintf ("Unknown VIA id %d.\n", (int)aVIA);
      EXIT (1);
  }


  return (bottomLayer);
}


/*  ------------------------------------------------------------------
 *  Function  :  "isVIALayer()".
 */

extern long  isVIALayer(aVIA, aLayer)
  char  aVIA;
  char  aLayer;
{
  switch (aVIA) {
    case CONT_BODY_N:
      if (aLayer == NWELL) return (TRUE);
    case CONT_BODY_P:
      if (aLayer == PWELL) return (TRUE);
      if (aLayer ==  ALU1) return (TRUE);
      if (aLayer == CALU1) return (TRUE);
      break;
    case CONT_DIF_N:
      if (aLayer ==  NDIF) return (TRUE);
    case CONT_DIF_P:
      if (aLayer ==  PDIF) return (TRUE);
      if (aLayer ==  ALU1) return (TRUE);
      if (aLayer == CALU1) return (TRUE);
      break;
    case CONT_POLY:
      if (aLayer ==  POLY) return (TRUE);
      if (aLayer ==  ALU1) return (TRUE);
      if (aLayer == CALU1) return (TRUE);
      break;
    case CONT_POLY2:
      if (aLayer == POLY2) return (TRUE);
      if (aLayer ==  ALU1) return (TRUE);
      if (aLayer == CALU1) return (TRUE);
      break;
    case CONT_VIA8:
      if (aLayer ==  ALU9) return (TRUE);
      if (aLayer == CALU9) return (TRUE);
    case CONT_VIA7:
      if (aLayer ==  ALU8) return (TRUE);
      if (aLayer == CALU8) return (TRUE);
    case CONT_VIA6:
      if (aLayer ==  ALU7) return (TRUE);
      if (aLayer == CALU7) return (TRUE);
    case CONT_VIA5:
      if (aLayer ==  ALU6) return (TRUE);
      if (aLayer == CALU6) return (TRUE);
    case CONT_VIA4:
      if (aLayer ==  ALU5) return (TRUE);
      if (aLayer == CALU5) return (TRUE);
    case CONT_VIA3:
      if (aLayer ==  ALU4) return (TRUE);
      if (aLayer == CALU4) return (TRUE);
    case CONT_VIA2:
      if (aLayer ==  ALU3) return (TRUE);
      if (aLayer == CALU3) return (TRUE);
    case CONT_VIA:
      if (aLayer ==  ALU2) return (TRUE);
      if (aLayer == CALU2) return (TRUE);
      if (aLayer ==  ALU1) return (TRUE);
      if (aLayer == CALU1) return (TRUE);
      break;
    case CONT_TURN9:
      if (aLayer ==  ALU9) return (TRUE);
      if (aLayer == CALU9) return (TRUE);
      break;
    case CONT_TURN8:
      if (aLayer ==  ALU8) return (TRUE);
      if (aLayer == CALU8) return (TRUE);
      break;
    case CONT_TURN7:
      if (aLayer ==  ALU7) return (TRUE);
      if (aLayer == CALU7) return (TRUE);
      break;
    case CONT_TURN6:
      if (aLayer ==  ALU6) return (TRUE);
      if (aLayer == CALU6) return (TRUE);
      break;
    case CONT_TURN5:
      if (aLayer ==  ALU5) return (TRUE);
      if (aLayer == CALU5) return (TRUE);
      break;
    case CONT_TURN4:
      if (aLayer ==  ALU4) return (TRUE);
      if (aLayer == CALU4) return (TRUE);
      break;
    case CONT_TURN3:
      if (aLayer ==  ALU3) return (TRUE);
      if (aLayer == CALU3) return (TRUE);
      break;
    case CONT_TURN2:
      if (aLayer ==  ALU2) return (TRUE);
      if (aLayer == CALU2) return (TRUE);
      break;
    case CONT_TURN1:
      if (aLayer ==  ALU1) return (TRUE);
      if (aLayer == CALU1) return (TRUE);
      break;
    default:
      errMBK  ("isVIALayer");
      eprintf ("Unknown VIA id %d.\n", (int)aVIA);
      EXIT (1);
  }

  return (FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getUpVIALayer()".
 */

extern long  getUpVIALayer(aVIA, aLayer)
  char  aVIA;
  char  aLayer;
{
  char  upLayer;


  if (!isVIALayer (aVIA, aLayer)) {
    errMBK  ("getUpVIALayer");
    eprintf ("Layer id %d do not belong to VIA id %d.\n",
             (int)aLayer, (int)aVIA);
    EXIT (1);
  }

  upLayer = getUpLayer (aLayer);
  if (!isVIALayer (aVIA, upLayer)) upLayer = aLayer;

  return (upLayer);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getTurnVIA()".
 */

extern char  getTurnVIA(aLayer)
  char  aLayer;
{
  char  turnVIA;


  turnVIA = CONT_TURN1;

  switch (aLayer) {
    case  ALU1:
    case CALU1:  turnVIA = CONT_TURN1; break;
    case  ALU2:
    case CALU2:  turnVIA = CONT_TURN3; break;
    case  ALU3:
    case CALU3:  turnVIA = CONT_TURN4; break;
    case  ALU4:
    case CALU4:  turnVIA = CONT_TURN5; break;
    case  ALU5:
    case CALU5:  turnVIA = CONT_TURN6; break;
    case  ALU6:
    case CALU6:  turnVIA = CONT_TURN7; break;
    case  ALU7:
    case CALU7:  turnVIA = CONT_TURN8; break;
    case  ALU8:
    case CALU8:  turnVIA = CONT_TURN9; break;
    case  ALU9:
    case CALU9:  turnVIA = CONT_TURN9; break;
    default:
      errMBK  ("getTurnVIA");
      eprintf ("No turn VIA for layer id %d.\n", (int)aLayer);
      EXIT (1);
  }

  return (turnVIA);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getUpLayer()".
 */

extern char  getUpLayer(aLayer)
  char  aLayer;
{
  char  upLayer;


  upLayer = ALU1;

  switch (aLayer) {
    case NWELL:  upLayer = ALU1; break;
    case PWELL:  upLayer = ALU1; break;
    case NTIE:   upLayer = ALU1; break;
    case PTIE:   upLayer = ALU1; break;
    case NDIF:   upLayer = ALU1; break;
    case PDIF:   upLayer = ALU1; break;
    case NTRANS: upLayer = ALU1; break;
    case PTRANS: upLayer = ALU1; break;
    case POLY:   upLayer = ALU1; break;
    case ALU1:   upLayer = ALU2; break;
    case CALU1:  upLayer = ALU2; break;
    case TALU1:  upLayer = ALU2; break;
    case ALU2:   upLayer = ALU3; break;
    case CALU2:  upLayer = ALU3; break;
    case TALU2:  upLayer = ALU3; break;
    case ALU3:   upLayer = ALU4; break;
    case CALU3:  upLayer = ALU4; break;
    case TALU3:  upLayer = ALU4; break;
    case ALU4:   upLayer = ALU5; break;
    case CALU4:  upLayer = ALU5; break;
    case TALU4:  upLayer = ALU5; break;
    case ALU5:   upLayer = ALU6; break;
    case CALU5:  upLayer = ALU6; break;
    case TALU5:  upLayer = ALU6; break;
    case ALU6:   upLayer = ALU7; break;
    case CALU6:  upLayer = ALU7; break;
    case TALU6:  upLayer = ALU7; break;
    case ALU7:   upLayer = ALU8; break;
    case CALU7:  upLayer = ALU8; break;
    case TALU7:  upLayer = ALU8; break;
    case ALU8:   upLayer = ALU9; break;
    case CALU8:  upLayer = ALU9; break;
    case TALU8:  upLayer = ALU9; break;
    case ALU9:   upLayer = ALU9; break;
    case CALU9:  upLayer = ALU9; break;
    case TALU9:  upLayer = ALU9; break;
    default:
      errMBK  ("getUpLayer");
      eprintf ("Unknown Layer id %d.\n", (int)aLayer);
      EXIT (1);
  }

  return (upLayer);
}


/*  ------------------------------------------------------------------
 *  Function  :  "cmpALU()".
 */

extern long  cmpALU(aLayer1, aLayer2)
  char  aLayer1, aLayer2;
{
  switch(aLayer1) {
    case ALU1:
      switch(aLayer2) {
        case  ALU1: return(F_EQUAL_M);
        case CALU1: return(F_EQUAL_C);
        case TALU1: return(F_EQUAL_T);
      }
      break;

    case ALU2:
      switch(aLayer2) {
        case  ALU2: return(F_EQUAL_M);
        case CALU2: return(F_EQUAL_C);
        case TALU2: return(F_EQUAL_T);
      }
      break;

    case ALU3:
      switch(aLayer2) {
        case  ALU3: return(F_EQUAL_M);
        case CALU3: return(F_EQUAL_C);
        case TALU3: return(F_EQUAL_T);
      }
      break;

    case ALU4:
      switch(aLayer2) {
        case  ALU4: return(F_EQUAL_M);
        case CALU4: return(F_EQUAL_C);
        case TALU4: return(F_EQUAL_T);
      }
      break;

    case ALU5:
      switch(aLayer2) {
        case  ALU5: return(F_EQUAL_M);
        case CALU5: return(F_EQUAL_C);
        case TALU5: return(F_EQUAL_T);
      }
      break;

    case ALU6:
      switch(aLayer2) {
        case  ALU6: return(F_EQUAL_M);
        case CALU6: return(F_EQUAL_C);
        case TALU6: return(F_EQUAL_T);
      }
      break;
  }

  return(FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getALU()".
 */

extern char  getALU(aLayer)
  char  aLayer;
{
  char  ALU;


  ALU = CALU1;

  switch (aLayer) {
    case CALU1:
    case  ALU1:  ALU = ALU1; break;
    case CALU2:
    case  ALU2:  ALU = ALU2; break;
    case CALU3:
    case  ALU3:  ALU = ALU3; break;
    case CALU4:
    case  ALU4:  ALU = ALU4; break;
    case CALU5:
    case  ALU5:  ALU = ALU5; break;
    case CALU6:
    case  ALU6:  ALU = ALU6; break;
    case CALU7:
    case  ALU7:  ALU = ALU7; break;
    case CALU8:
    case  ALU8:  ALU = ALU8; break;
    case CALU9:
    case  ALU9:  ALU = ALU9; break;
    default:
      errMBK  ("getALU");
      eprintf ("No ALU for layer id %d.\n", (int)aLayer);
      EXIT (1);
  }


  return (ALU);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getCALU()".
 */

extern char  getCALU(aLayer)
  char  aLayer;
{
  char  CALU;


  CALU = CALU1;

  switch (aLayer) {
    case CALU1:
    case  ALU1:  CALU = CALU1; break;
    case CALU2:
    case  ALU2:  CALU = CALU2; break;
    case CALU3:
    case  ALU3:  CALU = CALU3; break;
    case CALU4:
    case  ALU4:  CALU = CALU4; break;
    case CALU5:
    case  ALU5:  CALU = CALU5; break;
    case CALU6:
    case  ALU6:  CALU = CALU6; break;
    case CALU7:
    case  ALU7:  CALU = CALU7; break;
    case CALU8:
    case  ALU8:  CALU = CALU8; break;
    case CALU9:
    case  ALU9:  CALU = CALU9; break;
    default:
      errMBK  ("getCALU");
      eprintf ("No CALU for layer id %d.\n", (int)aLayer);
      EXIT (1);
  }


  return (CALU);
}


/*  ------------------------------------------------------------------
 *  Function  :  "isCALU()".
 */

extern long  isCALU(aLayer)
  char  aLayer;
{
  long  flag;


  flag = FALSE;
  switch (aLayer) {
    case CALU1:
    case CALU2:
    case CALU3:
    case CALU4:
    case CALU5:
    case CALU6:
    case CALU7:
    case CALU8:
    case CALU9: flag = TRUE; break;
  }


  return (flag);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getTALU()".
 */

extern char  getTALU(aLayer)
  char  aLayer;
{
  char  TALU;


  TALU = TALU1;

  switch (aLayer) {
    case TALU1:
    case CALU1:
    case  ALU1:  TALU = TALU1; break;
    case TALU2:
    case CALU2:
    case  ALU2:  TALU = TALU2; break;
    case TALU3:
    case CALU3:
    case  ALU3:  TALU = TALU3; break;
    case TALU4:
    case CALU4:
    case  ALU4:  TALU = TALU4; break;
    case TALU5:
    case CALU5:
    case  ALU5:  TALU = TALU5; break;
    case TALU6:
    case CALU6:
    case  ALU6:  TALU = TALU6; break;
    case TALU7:
    case CALU7:
    case  ALU7:  TALU = TALU7; break;
    case TALU8:
    case CALU8:
    case  ALU8:  TALU = TALU8; break;
    case TALU9:
    case CALU9:
    case  ALU9:  TALU = TALU9; break;
    default:
      errMBK  ("getTALU");
      eprintf ("No TALU for layer id %d.\n", (int)aLayer);
      EXIT (1);
  }


  return (TALU);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getLayerTrackWidth()".
 */

extern long  getLayerTrackWidth(aLayer)
  char  aLayer;
{
  long  trackWidth;


  trackWidth = MBKSCALE(2);

  switch (aLayer) {
    case CALU1:
    case TALU1:
    case  ALU1:  trackWidth = MBKSCALE (1); break;
    case CALU2:
    case TALU2:
    case  ALU2:
    case CALU3:
    case TALU3:
    case  ALU3:
    case CALU4:
    case TALU4:
    case  ALU4:
    case CALU5:
    case TALU5:
    case  ALU5:
    case CALU6:
    case TALU6:
    case  ALU6:
    case CALU7:
    case TALU7:
    case  ALU7:
    case CALU8:
    case TALU8:
    case  ALU8:
    case CALU9:
    case TALU9:
    case  ALU9:  trackWidth = MBKSCALE (2); break;
    default:
      errMBK  ("getLayerTrackWidth");
      eprintf ("No minimal track width for layer id %d.\n", (int)aLayer);
      EXIT (1);
  }

  return (trackWidth);
}


/*  ------------------------------------------------------------------
 *  Function  :  "xyflatseg()".
 */

extern void  xyflatseg(apFlatSeg, apSeg,
                       aXINS, aYINS,
                       aXAB1, aYAB1,
                       aXAB2, aYAB2,
                       aTransf)
  struct phseg *apFlatSeg, *apSeg;
          long  aXINS, aYINS, aXAB1, aYAB1, aXAB2, aYAB2;
          char  aTransf;
{
  long  swap;


  apFlatSeg->NAME  = apSeg->NAME;
  apFlatSeg->LAYER = apSeg->LAYER;
  apFlatSeg->WIDTH = apSeg->WIDTH;


  xyflat (&apFlatSeg->X1, &apFlatSeg->Y1,
               apSeg->X1,      apSeg->Y1,
          aXINS, aYINS,
          aXAB1, aYAB1,
          aXAB2, aYAB2,
          aTransf);

  xyflat (&apFlatSeg->X2, &apFlatSeg->Y2,
               apSeg->X2,      apSeg->Y2,
          aXINS, aYINS,
          aXAB1, aYAB1,
          aXAB2, aYAB2,
          aTransf);


  if (apFlatSeg->X1 > apFlatSeg->X2) {
    swap = apFlatSeg->X1;
    apFlatSeg->X1 = apFlatSeg->X2;
    apFlatSeg->X2 = swap;
  }

  if (apFlatSeg->Y1 > apFlatSeg->Y2) {
    swap = apFlatSeg->Y1;
    apFlatSeg->Y1 = apFlatSeg->Y2;
    apFlatSeg->Y2 = swap;
  }

  if (apFlatSeg->Y1 == apFlatSeg->Y2) {
    if (apFlatSeg->X1 < apFlatSeg->X2) apFlatSeg->TYPE = LEFT;
    else                               apFlatSeg->TYPE = RIGHT;
  } else {
    if (apFlatSeg->Y1 < apFlatSeg->Y2) apFlatSeg->TYPE = UP;
    else                               apFlatSeg->TYPE = DOWN;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "isPad()".
 */

extern long  isPad (asCell)
  char *asCell;
{
  char *i, *ext;

  ext = NULL;
  for(i = asCell; *i != (char)0; i++)
    if (*i == '_') ext = i;

  if (ext == NULL) return (FALSE);
  if (strcmp (ext, "_sp")) return (FALSE);

  return (TRUE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "initSigIndex()".
 */

extern long  initSigIndex(apLoSig)
  losig_list *apLoSig;
{
  losig_list *pLoSig;
        long  iMax;

  iMax = 0L;
  for(pLoSig = apLoSig; pLoSig != (losig_list*)NULL; pLoSig = pLoSig->NEXT)
    if (pLoSig->INDEX > iMax) iMax = pLoSig->INDEX;

  return(MBK_sigIndex = iMax);
}


/*  ------------------------------------------------------------------
 *  Function  :  "cmpSigName()".
 */

extern long  cmpSigName(apLoSig, asName)
  losig_list *apLoSig;
        char *asName;
{
  chain_list *pChain;


  for(pChain  = apLoSig->NAMECHAIN;
      pChain != (chain_list*)NULL;
      pChain  = pChain->NEXT) {
    if ((char*)(pChain->DATA) == asName) return(TRUE);
  }

  return(FALSE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "addInsLoCon()".
 */

extern locon_list *addInsLoCon(apIns, asName, apSig, aDir)
  loins_list *apIns;
        char *asName;
  losig_list *apSig;
        char  aDir;
{
  locon_list *pCon;
        char *sName;


  sName = namealloc(asName);
  for(pCon = apIns->LOCON; pCon != (locon_list*)NULL; pCon = pCon->NEXT) {
    if (pCon->NAME == sName) {
      pCon->SIG  = apSig;
      return(pCon);
    }
  }

  pCon = (locon_list*)malloc(sizeof(locon_list));
  pCon->NAME      = sName;
  pCon->SIG       = apSig;
  pCon->ROOT      = (void*)apIns;
  pCon->TYPE      = INTERNAL;
  pCon->DIRECTION = aDir;

  pCon->NEXT      = apIns->LOCON;
  apIns->LOCON    = pCon;

  return(pCon);
}


/*  ------------------------------------------------------------------
 *  Function  :  "splitPowerNet()".
 */

extern void  splitPowerNet(apLoFig, asPower)
  lofig_list *apLoFig;
        char *asPower;
{
  ptype_list *pType;
  chain_list *pChain;
  locon_list *pLoCon;
  loins_list *pLoIns;
  losig_list *pSigPower, *pSig;
        char *tsSig[2], *sVDD, *sVSS, *sPOW;
        long  powerType;


  sVDD = namealloc("vdd");
  sVSS = namealloc("vss");

  tsSig[C_POWER_VDD] = namealloc("vddi");
  tsSig[C_POWER_VSS] = namealloc("vssi");

  powerType = C_POWER_UNKNOWN; sPOW = (char*)NULL;
  if (!strcmp(asPower, sVDD)) { powerType = C_POWER_VDD; sPOW = sVDD; }
  if (!strcmp(asPower, sVSS)) { powerType = C_POWER_VSS; sPOW = sVSS; }

  if (powerType == C_POWER_UNKNOWN) {
    eprinth((char*)NULL);
    eprintf("\n  Only \"vdd\" and \"vss\" can be splitted (%s).\n", asPower);
    EXIT (1);
  }


  pChain    = addchain((chain_list*)NULL, (void*)tsSig[powerType]);
  pSigPower = addlosig(apLoFig, NEWSIGINDEX, pChain, INTERNAL);


  /* Find the power net to be splitted. */
  for(pSig = apLoFig->LOSIG; pSig != (losig_list*)NULL; pSig = pSig->NEXT)
    if (cmpSigName(pSig, sPOW)) break;

  if (pSig == (losig_list*)NULL) {
    eprinth((char*)NULL);
    eprintf("\n  Signal \"%s\" not found for splitting.\n", sPOW);
    EXIT (1);
  }

  pType = getptype(pSig->USER, (long)LOFIGCHAIN);
  if (pType == (ptype_list*)NULL) {
    eprinth((char*)NULL);
    eprintf("\n  Signal \"%s\" have no LOFIGCHAIN.\n", sPOW);
    EXIT (1);
  }


  /* Split the power between core cells and pad cells. */
  pChain = (chain_list*)(pType->DATA);

  for(; pChain != (chain_list*)NULL;
        pChain  = pChain->NEXT) {
    pLoCon = (locon_list*)(pChain->DATA);

    if (pLoCon->TYPE == INTERNAL) {
      pLoIns = (loins_list*)(pLoCon->ROOT);

      if (isPad(pLoIns->FIGNAME)) pLoCon->SIG = pSigPower;

      if (    (powerType == C_POWER_VDD)
          && !strcmp(pLoIns->FIGNAME, "pvddi_sp")) {
        addInsLoCon(pLoIns, sPOW, pSig, pLoCon->DIRECTION);
      }

      if (    (powerType == C_POWER_VSS)
          && !strcmp(pLoIns->FIGNAME, "pvssi_sp")) {
        addInsLoCon(pLoIns, sPOW, pSig, pLoCon->DIRECTION);
      }
    }
  } /* End of "pChain" loop. */
}


/*  ------------------------------------------------------------------
 *  Function  :  "addPowerNet()".
 */

extern void  addPowerNet(apLoFig, asPower)
  lofig_list *apLoFig;
        char *asPower;
{
  chain_list *pChain;
  losig_list *pSigPower;
        char *sPOW;


  sPOW = namealloc(asPower);

  pChain    = addchain((chain_list*)NULL, (void*)sPOW);
  pSigPower = addlosig(apLoFig, NEWSIGINDEX, pChain, EXTERNAL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "xyflatvia()".
 */

extern void  xyflatvia(apFlatVIA, apVIA,
                       aXINS, aYINS,
                       aXAB1, aYAB1,
                       aXAB2, aYAB2,
                       aTransf)
  struct phvia *apFlatVIA, *apVIA;
          long  aXINS, aYINS, aXAB1, aYAB1, aXAB2, aYAB2;
          char  aTransf;
{
  apFlatVIA->NAME = apVIA->NAME;
  apFlatVIA->TYPE = apVIA->TYPE;

  xyflat (&apFlatVIA->XVIA, &apFlatVIA->YVIA,
               apVIA->XVIA,      apVIA->YVIA,
          aXINS, aYINS,
          aXAB1, aYAB1,
          aXAB2, aYAB2,
          aTransf);

  switch (aTransf) {
    case ROT_M:
    case ROT_P:
    case SY_RM:
    case SY_RP:
      apFlatVIA->DX = apVIA->DY;
      apFlatVIA->DY = apVIA->DX;
      break;
    case NOSYM:
    case SYM_X:
    case SYM_Y:
    case SYMXY:
    default:
      apFlatVIA->DX = apVIA->DX;
      apFlatVIA->DY = apVIA->DY;
      break;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "findphins()".
 */

extern struct phins *findphins(apPhfig, insName)
  struct phfig *apPhfig;
          char *insName;
{
  struct phins *pIns;

  for (pIns = apPhfig->PHINS; pIns != NULL; pIns = pIns->NEXT)
    if (pIns->INSNAME == insName) return (pIns);

  return (NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "findphmodel()".
 */

extern struct phins *findphmodel(apPhfig, modelName)
  struct phfig *apPhfig;
          char *modelName;
{
  struct phins *pIns;

  for (pIns = apPhfig->PHINS; pIns != NULL; pIns = pIns->NEXT)
    if (pIns->FIGNAME == modelName) return (pIns);

  return (NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "findlocon()".
 */

extern struct locon *findlocon(apLofig, conName)
  struct lofig *apLofig;
          char *conName;
{
  struct locon *pCon;

  for (pCon = apLofig->LOCON; pCon != NULL; pCon = pCon->NEXT)
    if (pCon->NAME == conName) return (pCon);

  return (NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "addloins_noSig()".
 */

extern loins_list *addloins_noSig(apFig, asIns, apModel)
  lofig_list *apFig, *apModel;
        char *asIns;
{
  loins_list *pIns    = NULL;
  locon_list *pCon    = NULL;
  locon_list *pInsCon = NULL;
  chain_list *pChain;
        char *figName;
        char *insName;


  insName = namealloc (asIns);
  figName = apModel->NAME;


  /* Check insName  unicity */
  if (strcmp (insName, "*") && FAST_MODE != 'Y') {
    for (pIns = apFig->LOINS; pIns; pIns = pIns->NEXT) {
      if (pIns->INSNAME == insName) {
        errMBK  ("defloadlophig");
        eprintf ("\n  addloins_noSig impossible, ");
        eprintf ("instance %s already exist in figure %s\n", 
                 insName, apFig->NAME);
        EXIT(1);
      }
    }
  }


  if (apFig->NAME == figName) {
    errMBK  ("defloadlophig");
    eprintf ("\n  addloins_noSig impossible for %s,", insName);
    eprintf ("\n  instance model is the figure %s itself\n", figName);
    EXIT(1);
  }


  pIns = (loins_list *)mbkalloc (sizeof (loins_list));

  pIns->INSNAME = insName;
  pIns->FIGNAME = figName;
  pIns->LOCON   = NULL;
  pIns->USER    = NULL;
  pIns->NEXT    = apFig->LOINS;
  apFig->LOINS  = pIns;


  /* Update model list   */
  for (pChain = apFig->MODELCHAIN; pChain; pChain = pChain->NEXT)
    if (pChain->DATA == (void *)figName) break;

  if (!pChain)
    apFig->MODELCHAIN = addchain (apFig->MODELCHAIN, (void *)figName);


  for (pCon = apModel->LOCON; pCon; pCon = pCon->NEXT) {
     pInsCon = (locon_list *)mbkalloc (sizeof (locon_list));

     pInsCon->NAME      = pCon->NAME;
     pInsCon->DIRECTION = pCon->DIRECTION;
     pInsCon->TYPE      = 'I';
     /* We do not known the signals for the time beeing. */
     pInsCon->SIG       = NULL;
     pInsCon->ROOT      = (void *)pIns;
     pInsCon->USER      = NULL;
     pInsCon->PNODE     = NULL;
     pInsCon->NEXT      = pIns->LOCON;
     pIns->LOCON        = pInsCon;
  }

  pIns->LOCON = (locon_list *)reverse((chain_list *)pIns->LOCON);


  return (pIns);
}


/*  ------------------------------------------------------------------
 *  Function  :  "addlosig_insCon()".
 */

extern locon_list *addlosig_insCon(apIns, asCon, apSig)
  loins_list *apIns;
  losig_list *apSig;
        char *asCon;
{
  locon_list *pCon;
        char *sCon;


  sCon = namealloc (asCon);

  for (pCon = apIns->LOCON; pCon != NULL; pCon = pCon->NEXT) {
    if (pCon->NAME == sCon) {
      pCon->SIG = apSig;

      return (pCon);
    }

  }


  if (!pCon) {
    errMBK  ("defloadlophig");
    eprintf ("\n  addlosig_insCon impossible, ");
    eprintf ("instance %s of model %s has no terminal %s.\n",
             apIns->INSNAME, apIns->FIGNAME, sCon);
    EXIT(1);
  }

  return (pCon);
}


/*  ------------------------------------------------------------------
 *  Function  :  "addfeed()".
 */

extern void  addfeed (apLofig, apPhfig)
  struct lofig *apLofig;
  struct phfig *apPhfig;
{
  struct locon *pLocon;
  struct phins *pPhins;
  struct lofig *pFeedfig;
  struct losig *sig, *sigVDD, *sigVSS;
  struct chain *pSighead, **ppSigtail;


  /* Find VDD & VSS signals. */
  for (sigVDD = apLofig->LOSIG;
       !isvdd (getsigname (sigVDD)); sigVDD = sigVDD->NEXT);

  if (!sigVDD) {
    errMBK  (NULL);
    eprintf ("The netlist have no VDD net.\n");
    EXIT (1);
  }

  for (sigVSS = apLofig->LOSIG;
       !isvss (getsigname (sigVSS)); sigVSS = sigVSS->NEXT);

  if (!sigVSS) {
    errMBK  (NULL);
    eprintf ("The netlist have no VSS net.\n");
    EXIT (1);
  }


  /* Find the feedthru instances. */
  for (pPhins = apPhfig->PHINS; pPhins != NULL; pPhins = pPhins->NEXT) {
    if (incatalogfeed (pPhins->FIGNAME)) {
      pFeedfig = getlofig (pPhins->FIGNAME, 'P');

      pSighead  = NULL;
      ppSigtail = &pSighead;

      /* Build the signal list. */
      for (pLocon = pFeedfig->LOCON; pLocon != NULL; pLocon = pLocon->NEXT) {
        sig = NULL; 

        if (isvdd (pLocon->NAME)) sig = sigVDD;
        if (isvss (pLocon->NAME)) sig = sigVSS;

        if (!sig) {
          errMBK  (NULL);
          eprintf ("\n  Terminal \"%s\" of feedthru model \"%s\" is not a\n",
                   pLocon->NAME,
                   pPhins->FIGNAME);
          eprintf ("  power terminal!\n");
          EXIT (1);
        }

        *ppSigtail = addchain (*ppSigtail, (void*)sig);
         ppSigtail = &((*ppSigtail)->NEXT);
      }

      /* Add the instance. */
      addloins (apLofig, pPhins->INSNAME, pFeedfig, pSighead);
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "delfeed()".
 */

extern void  delfeed (apLofig)
  struct lofig *apLofig;
{
  struct loins *pIns;
  struct chain *pFeedIns, *pChain;

  pFeedIns = NULL;

  /* Find the feedthru instances. */
  for (pIns = apLofig->LOINS; pIns != NULL; pIns = pIns->NEXT) {
    if (incatalogfeed (pIns->FIGNAME))
      pFeedIns = addchain (pFeedIns, (void*)pIns->INSNAME);
  }


  /* Remove thoses instances. */
  for (pChain = pFeedIns; pChain != NULL; pChain = pChain->NEXT) {
    delloins (apLofig, (char*)(pChain->DATA));
  }
  

  freechain (pFeedIns);
}


/*  ------------------------------------------------------------------
 *  Function  :  "checkLofig()".
 */

extern void  checkLofig(apLofig)
  struct lofig *apLofig;
{
  struct loins *pIns;
  struct locon *pCon;
          long  unassigned;


  unassigned = 0;

  for (pIns = apLofig->LOINS; pIns != NULL; pIns = pIns->NEXT) {
    for(pCon = pIns->LOCON; pCon != NULL; pCon = pCon->NEXT) {
      if (!pCon->SIG) {
        unassigned++;

        warnMBK ("defloadlophfig");
        wprintf ("Terminal %s of instance %s is not assigned.\n",
                 pCon->NAME, pIns->INSNAME);
      }
    }
  }

  if (unassigned) {
    errMBK  ("defloadlophig");
    eprintf ("There is %ld terminals unassigned.\n", unassigned);
    EXIT (1);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "copyUpCALU()".
 */

extern void  copyUpCALU(apLofig, apPhfig, aExit)
  struct lofig *apLofig;
  struct phfig *apPhfig;
          long  aExit;
{
          long  flag;
  struct locon *pLocon, *pLoconIns;
  struct losig *pLosig;
  struct chain *pChain, *pChainLocon;
  struct loins *pLoins;
  struct phseg *pPhseg;
  struct phins *pPhins;
  struct phfig *pPhfigIns;
          char *sigName;
          long  X1, Y1, X2, Y2;


  lofigchain (apLofig);


  for (pLocon = apLofig->LOCON; pLocon != NULL; pLocon = pLocon->NEXT) {
    pLosig = pLocon->SIG;

    flag = FALSE;

    for(pChain = pLosig->NAMECHAIN; pChain != NULL; pChain = pChain->NEXT) {
      sigName = (char*)pChain->DATA;

      if (isvdd (sigName) || isvss (sigName)) continue;

      /* Looks if the terminal already have physical segments in CALU on
       * the top level of the design.
       * This is a bit slow but I have no time for optimization.
       */
      for (pPhseg = apPhfig->PHSEG; pPhseg != NULL; pPhseg = pPhseg->NEXT) {
        if (   (pPhseg->NAME == sigName)
            && (isCALU(pPhseg->LAYER))  ) {
          flag = TRUE;
          break;
        }
      }


      if (!flag) {
        /* No CALU in the top level design. We have to copy up the
         * terminals from the instances (in fact there should be only one
         * instance).
         */
        flag = FALSE;
        
        pChainLocon = (chain_list*)(getptype (pLosig->USER,
                                              LOFIGCHAIN)->DATA);

        for (; pChainLocon != NULL; pChainLocon = pChainLocon->NEXT) {
          /* Copy only the first CALU segment to avoid separate segments
           * for one equi (this is not valid for lynx).
           */
          pLoconIns = (locon_list*) pChainLocon->DATA;

          if (pLoconIns->TYPE == INTERNAL) {
            pLoins = (loins_list*)(pLoconIns->ROOT);

            pPhfigIns = getphfig (pLoins->FIGNAME, 'A');
            pPhins    = getphins (apPhfig, pLoins->INSNAME);

            for (pPhseg = pPhfigIns->PHSEG;
                 pPhseg != NULL; pPhseg = pPhseg->NEXT) {
              if (   (pPhseg->NAME == pLoconIns->NAME)
                  && (isCALU(pPhseg->LAYER))          ) {
                flag = TRUE;

                xyflat (          &X1  ,           &Y1  ,
                           pPhseg->X1  ,    pPhseg->Y1  ,
                           pPhins->XINS   , pPhins->YINS,
                        pPhfigIns->XAB1, pPhfigIns->YAB1,
                        pPhfigIns->XAB2, pPhfigIns->YAB2,
                        pPhins->TRANSF);

                xyflat (          &X2  ,           &Y2  ,
                           pPhseg->X2  ,    pPhseg->Y2  ,
                           pPhins->XINS   , pPhins->YINS,
                        pPhfigIns->XAB1, pPhfigIns->YAB1,
                        pPhfigIns->XAB2, pPhfigIns->YAB2,
                        pPhins->TRANSF);

                addphseg (apPhfig,
                          pPhseg->LAYER,
                          pPhseg->WIDTH,
                          X1, Y1, X2, Y2,
                          sigName
                          );
              }
            }
          }
        } /* End of "for(pChainLocon...)". */

        if (!flag) {
          if (aExit) {
            errMBK  ("copyUpCALU");
            eprintf ("\n  No instances CALUx for terminal \"%s\".\n",
                     pLocon->NAME);
            EXIT (1);
          } else {
            warnMBK ("copyUpCALU");
            wprintf ("\n  No instances CALUx for terminal \"%s\".\n",
                     pLocon->NAME);
          }
        }
      } /* End of "for(pChain...)". */
    }
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "addloseg()".
 */

extern struct eLoseg_s *addloseg (apHead, aType, apMBKobj)
  struct eLoseg_s *apHead;
             long  aType;
             void *apMBKobj;
{
  struct eLoseg_s  *pLoseg;
  struct ptype    **ppUser;


  ppUser = NULL;
  pLoseg = (struct eLoseg_s*)mbkalloc (sizeof (struct eLoseg_s));

  switch (aType) {
    case LOSEG_VIA:    ppUser = &(((struct phvia*)apMBKobj)->USER); break;
    case LOSEG_CON:    ppUser = &(((struct phcon*)apMBKobj)->USER); break;
    case LOSEG_SEG:
    case LOSEG_SEGCON: ppUser = &(((struct phseg*)apMBKobj)->USER); break;
    default:
      wprinth ("addloseg");
      eprintf ("\n  Bad loseg type id %ld.\n", aType);
      EXIT (1);
  }

  *ppUser = addptype (*ppUser, PTYPE_LOSEG, (void*)pLoseg);

  pLoseg->type   = aType;
  pLoseg->MBKobj = apMBKobj;
  pLoseg->next   = apHead;


  return (pLoseg);
}


/*  ------------------------------------------------------------------
 *  Function  :  "delloseg()".
 */

extern struct eLoseg_s *delloseg (apLoseg)
  struct eLoseg_s *apLoseg;
{
  struct ptype    **ppUser;


  ppUser = NULL;

  switch (apLoseg->type) {
    case LOSEG_VIA:    ppUser = &(((struct phvia*)apLoseg->MBKobj)->USER); break;
    case LOSEG_CON:    ppUser = &(((struct phcon*)apLoseg->MBKobj)->USER); break;
    case LOSEG_SEG:
    case LOSEG_SEGCON: ppUser = &(((struct phseg*)apLoseg->MBKobj)->USER); break;
    default:
      wprinth ("delloseg");
      eprintf ("\n  Bad loseg type id %ld.\n", apLoseg->type);
      EXIT (1);
  }

  *ppUser = delptype (*ppUser, PTYPE_LOSEG);

  return (apLoseg->next);
}


/*  ------------------------------------------------------------------
 *  Function  :  "addtloseg()".
 */

extern struct tLoseg_s *addtloseg (apPhfig)
  struct phfig *apPhfig;
{
  struct tLoseg_s *ptLoseg;
              int   tLoseg_size;


  if (getptype (apPhfig->USER, PTYPE_TLOSEG)) {
    eprinth ("addtloseg");
    eprintf ("\n  Attempt to re-create loseglist on figure \"%s\".",
             apPhfig->NAME);
    EXIT (1);
  }


  ptLoseg = (struct tLoseg_s*)mbkalloc (sizeof (struct tLoseg_s));

  apPhfig->USER = addptype (apPhfig->USER, PTYPE_TLOSEG, (void*)ptLoseg);

  tLoseg_size = sizeof (struct eLoseg_s*) * TLOSEG_SIZE;

  ptLoseg->sigNB   = 0;
  ptLoseg->tSize   = TLOSEG_SIZE;
  ptLoseg->tHT     = createauthtable (TLOSEG_SIZE);
  ptLoseg->tAccess = (long*) malloc (sizeof (long) * TLOSEG_SIZE);
  ptLoseg->tFlags  = (long*) malloc (sizeof (long) * TLOSEG_SIZE);
  ptLoseg->tLoseg  = (struct eLoseg_s**) malloc (tLoseg_size);

  memset (ptLoseg->tLoseg , 0, tLoseg_size);
  memset (ptLoseg->tAccess, 0, sizeof(long) * TLOSEG_SIZE);
  memset (ptLoseg->tFlags , 0, sizeof(long) * TLOSEG_SIZE);
                      
  ptLoseg->tMVIA  = NULL;


  return (ptLoseg);
}


/*  ------------------------------------------------------------------
 *  Function  :  "gettloseg()".
 */

extern struct tLoseg_s *gettloseg (apPhfig)
  struct phfig *apPhfig;
{
  struct ptype *pType;

  pType = getptype (apPhfig->USER, PTYPE_TLOSEG);

  if (pType) return ((struct tLoseg_s*)(pType->DATA));

  return (NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "gettlosegitem()".
 */

extern struct authelem *gettlosegitem (apTLoseg, asKey)
  struct tLoseg_s *apTLoseg;
             char *asKey;
{
  struct authelem *pElem;
           size_t  eLoseg_size;
             long  sigIndex;


  eLoseg_size = sizeof (struct eLoseg_s*);

  pElem = searchauthelem (apTLoseg->tHT, asKey);

  if (!pElem) {
    sigIndex = apTLoseg->sigNB;

    __DBG(
      fflush (stdout);
      fprintf (stderr, "Adding loseg \"%s\" (id %ld)\n", asKey, sigIndex);
    )

    if (apTLoseg->sigNB >= apTLoseg->tSize) {
      apTLoseg->tSize += TLOSEG_SIZE;

      /* Re-allocate the 'tLoseg' table. */
      apTLoseg->tLoseg = (struct eLoseg_s**)realloc (
                           apTLoseg->tLoseg, eLoseg_size * apTLoseg->tSize);

      if (!apTLoseg->tLoseg) {
        eprinth ("phsegchain");
        eprintf ("\n  Not enougth memory to re-allocate \"tLoseg\".\n");
        EXIT (1);
      }

      memset (apTLoseg->tLoseg + (apTLoseg->tSize - TLOSEG_SIZE),
              0, eLoseg_size * TLOSEG_SIZE);


      /* Re-allocate the 'tAccess' & 'tFlags' tables. */
      apTLoseg->tAccess = (long*)realloc (
                            apTLoseg->tAccess, sizeof(long) * apTLoseg->tSize);
      apTLoseg->tFlags  = (long*)realloc (
                            apTLoseg->tFlags , sizeof(long) * apTLoseg->tSize);

      if (!apTLoseg->tAccess || !apTLoseg->tFlags) {
        eprinth ("phsegchain");
        eprintf ("\n  Not enougth memory to re-allocate \"tLoseg\".\n");
        EXIT (1);
      }

      memset (apTLoseg->tAccess + (apTLoseg->tSize - TLOSEG_SIZE),
              0, sizeof(long) * TLOSEG_SIZE);
    }

    pElem = addauthelem (apTLoseg->tHT, asKey, sigIndex);

    apTLoseg->sigNB += 1;
  }


  return (pElem);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getlosegname()".
 */

static char *getlosegname (apLoseg)
  struct eLoseg_s *apLoseg;
{
  switch (apLoseg->type) {
    case LOSEG_SEG: return (((struct phseg*)(apLoseg->MBKobj))->NAME);
    case LOSEG_VIA: return (((struct phvia*)(apLoseg->MBKobj))->NAME);
    case LOSEG_CON: return (((struct phcon*)(apLoseg->MBKobj))->NAME);
  }


  return ("Invalid loseg object! This should never happens!\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "freeloseg()".
 */

extern void  freeloseg (apPhfig)
  struct phfig *apPhfig;
{
  struct phseg    *pPhseg;
  struct ptype    *pType;
  struct tLoseg_s *ptLoseg;
  struct eMVIA_s  *pMVIA, *pMVIA_next;


  for (pPhseg = apPhfig->PHSEG; pPhseg != NULL; pPhseg = pPhseg->NEXT) {
    if ((pType = getptype (pPhseg->USER, PTYPE_LOSEG))) {
      mbkfree (pType->DATA);
      pPhseg->USER = delptype (pPhseg->USER, PTYPE_LOSEG);
    }
  }



  if ((pType = getptype (apPhfig->USER, PTYPE_TLOSEG))) {
    ptLoseg = (struct tLoseg_s*)(pType->DATA);

    destroyauthtable (ptLoseg->tHT);
                free (ptLoseg->tLoseg);

    for (pMVIA = ptLoseg->tMVIA; pMVIA != NULL;) {
      pMVIA_next = pMVIA->next;

      mbkfree (pMVIA);

      pMVIA = pMVIA_next;
    }


    mbkfree (ptLoseg->tAccess);
    mbkfree (ptLoseg->tFlags);
    mbkfree (ptLoseg);

    apPhfig->USER = delptype (apPhfig->USER, PTYPE_TLOSEG);
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "addmvia()".
 */

extern struct eMVIA_s *addmvia (apHead, apVIA)
  struct eMVIA_s *apHead;
  struct phvia   *apVIA;
{
  struct eMVIA_s  *pMVIA;


  pMVIA = (struct eMVIA_s*)mbkalloc (sizeof (struct eMVIA_s));

  pMVIA->type   = apVIA->TYPE;
  pMVIA->width  = apVIA->DX;
  pMVIA->height = apVIA->DY;
  pMVIA->next   = apHead;


  return (pMVIA);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getmvia()".
 */

extern struct eMVIA_s *getmvia (apHead, apVIA)
  struct eMVIA_s *apHead;
  struct phvia   *apVIA;
{
  struct eMVIA_s  *pMVIA;


  for (pMVIA = apHead; pMVIA != NULL; pMVIA = pMVIA->next) {
    if (   (pMVIA->type   == apVIA->TYPE)
        && (pMVIA->width  == apVIA->DX  )
        && (pMVIA->height == apVIA->DY  ))
      break;
  }

  return (pMVIA);
}


/*  ------------------------------------------------------------------
 *  Function  :  "buildtsigalias()".
 */

static void  buildtsigalias(apLosig)
  struct losig *apLosig;
{
  struct losig *pLosig;
  struct chain *pChain;
          long  sigNumber;


  if (LV_htSigAlias || LV_tSigAlias) {
    eprinth ("buildtsigalias");
    eprintf ("\n  Attempt to re-allocate the signal alias name table.\n");
    EXIT (1);
  }


  sigNumber = 0;

  for (pLosig = apLosig; pLosig != NULL; pLosig = pLosig->NEXT) sigNumber++;


  LV_tSigAlias  = (struct sigalias_s*) mbkalloc (
                    sigNumber * sizeof (struct sigalias_s));
  LV_htSigAlias = createauthtable (sigNumber);


  sigNumber = 0;

  for (pLosig = apLosig; pLosig != NULL; pLosig = pLosig->NEXT) {
    LV_tSigAlias[sigNumber].name  = getsigname (pLosig);
    LV_tSigAlias[sigNumber].flags = 0L;

    __DBG( fprintf (stderr, "  o  Main signal name := \"%s\".\n",
                            LV_tSigAlias[sigNumber].name); )

    for (pChain = pLosig->NAMECHAIN; pChain != NULL; pChain = pChain->NEXT) {
      addauthelem (LV_htSigAlias, pChain->DATA, sigNumber);

      __DBG(
        fprintf (stderr, "     - Alias := \"%s\".\n", (char*)pChain->DATA); )

      if (pChain != pLosig->NAMECHAIN)
        LV_tSigAlias[sigNumber].flags = F_HAS_SUBNET;
    }

    sigNumber++;
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "freetsigalias()".
 */

static void  freetsigalias()
{
  if (!LV_htSigAlias) destroyauthtable (LV_htSigAlias);
  if (!LV_tSigAlias)  mbkfree (LV_tSigAlias);

  LV_htSigAlias = NULL;
  LV_tSigAlias  = NULL;
}


/*  ------------------------------------------------------------------
 *  Function  :  "matchsigname()".
 */

static struct sigalias_s *matchsigname(aSegName)
  char *aSegName;
{
  static struct sigalias_s   noSigAlias;
                  authelem  *pElem;
                      char **tSegAlias;
                      long   iSegAlias;


  tSegAlias = makesegnamelist (aSegName);

  for (iSegAlias = 0; tSegAlias[iSegAlias] != NULL; iSegAlias++) {
    pElem = searchauthelem (LV_htSigAlias, tSegAlias[iSegAlias]);

    if (pElem) 
      return (&(LV_tSigAlias[pElem->VALUE]));
  }

  /* This name doesn't match any net name (losig).
   * This is probably an error.
   */
  noSigAlias.name  = aSegName;
  noSigAlias.flags = 0L;
  
  return (&noSigAlias);
}


/*  ------------------------------------------------------------------
 *  Function  :  "phsegchain()".
 */

extern void  phsegchain(apLofig, apPhfig)
  struct lofig *apLofig;
  struct phfig *apPhfig;
{
  struct tLoseg_s   *ptLoseg;
  struct phseg      *pPhseg;
  struct phvia      *pPhvia;
  struct phcon      *pPhcon;
  struct sigalias_s *pSigAlias;
         authelem   *pElem;


  ptLoseg = addtloseg (apPhfig);


  /* Building alias table name. */
  buildtsigalias (apLofig->LOSIG);


  /* Sorting segments by names. */
  for (pPhseg = apPhfig->PHSEG; pPhseg != NULL; pPhseg = pPhseg->NEXT) {
    __DBG(
      fprintf (stdout, "seg (%ld,%ld) (%ld,%ld) width %ld layer id %d\n",
               pPhseg->X1,
               pPhseg->Y1,
               pPhseg->X2,
               pPhseg->Y2,
               pPhseg->WIDTH,
               (int)pPhseg->LAYER);
      fflush (stdout);
    )
    if (pPhseg->NAME == NULL) {
      eprinth ("phsegchain");
      eprintf ("\n  Segment (%ld,%ld) (%ld,%ld) width %ld layer id %d",
               pPhseg->X1,
               pPhseg->Y1,
               pPhseg->X2,
               pPhseg->Y2,
               pPhseg->WIDTH,
               (int)pPhseg->LAYER);
      eprintf ("\n  doesn't have a name.\n");
      EXIT (1);
    }

    if (pPhseg->NAME[0] == (char)0) {
      eprinth ("phsegchain");
      eprintf ("\n  Segment (%ld,%ld) (%ld,%ld) width %ld layer id %d",
               pPhseg->X1,
               pPhseg->Y1,
               pPhseg->X2,
               pPhseg->Y2,
               pPhseg->WIDTH,
               (int)pPhseg->LAYER);
      eprintf ("\n  have an empty string for name.\n");
      EXIT (1);
    }

    if (isobs (pPhseg)) continue;

    pSigAlias = matchsigname (pPhseg->NAME);

    pElem = gettlosegitem (ptLoseg, pSigAlias->name);

    ptLoseg->tLoseg[pElem->VALUE] = addloseg (ptLoseg->tLoseg[pElem->VALUE],
                                              LOSEG_SEG,
                                              (void*)pPhseg);
  }


  /* Sorting vias by names. */
  for (pPhvia = apPhfig->PHVIA; pPhvia != NULL; pPhvia = pPhvia->NEXT) {
    if (pPhvia->NAME == NULL) {
      eprinth ("phsegchain");
      eprintf ("\n  VIA (%ld,%ld) type id %d",
               pPhvia->XVIA,
               pPhvia->YVIA,
               (int)pPhvia->TYPE);
      eprintf ("\n  doesn't have a name.\n");
      EXIT (1);
    }

    if (!getmvia (ptLoseg->tMVIA, pPhvia)) {
      ptLoseg->tMVIA = addmvia (ptLoseg->tMVIA, pPhvia);
      __DBG(
        fflush (stdout);
        fprintf (stderr, "tMVIA := 0x%08lx\n", (unsigned long)ptLoseg->tMVIA);
        fflush (stderr);
      )
    }

    pSigAlias = matchsigname (pPhvia->NAME);

    pElem = gettlosegitem (ptLoseg, pSigAlias->name);

    ptLoseg->tLoseg[pElem->VALUE] = addloseg (ptLoseg->tLoseg[pElem->VALUE],
                                              LOSEG_VIA,
                                              (void*)pPhvia);

    __DBG(
      fprintf (stderr, "    - New loseg := 0x%08ld\n",
                       (unsigned long)ptLoseg->tLoseg[pElem->VALUE]);
    )
  }


  /* Sorting terminals by names. */
  for (pPhcon = apPhfig->PHCON; pPhcon != NULL; pPhcon = pPhcon->NEXT) {
    if (pPhcon->NAME == NULL) {
      eprinth ("phsegchain");
      eprintf ("\n  CON (%ld,%ld) type id %d",
               pPhcon->XCON,
               pPhcon->YCON,
               (int)pPhseg->TYPE);
      eprintf ("\n  doesn't have a name.\n");
      EXIT (1);
    }

    pSigAlias = matchsigname (pPhcon->NAME);

    pElem = gettlosegitem (ptLoseg, pSigAlias->name);

    ptLoseg->tLoseg[pElem->VALUE] = addloseg (ptLoseg->tLoseg[pElem->VALUE],
                                              LOSEG_CON,
                                              (void*)pPhcon);

    __DBG(
      fprintf (stderr, "    - New loseg := 0x%08ld\n",
                       (unsigned long)ptLoseg->tLoseg[pElem->VALUE]);
    )
  }


  freetsigalias ();

  __DBG(
    fprintf (stderr, "ptLoseg := 0x%08lx\n", (unsigned long)ptLoseg);
    fflush (stderr);
  )
}


/*  ------------------------------------------------------------------
 *  Function  :  "getloseglist()".
 */

extern struct eLoseg_s *getloseglist(apPhfig, aName)
  struct phfig *apPhfig;
          char *aName;
{
  struct tLoseg_s *ptLoseg;
  struct eLoseg_s *pLoseg;
  struct ptype    *pType;
         authelem *pElem;


  if (!(pType = getptype (apPhfig->USER, PTYPE_TLOSEG))) {
    /*
     * eprinth ("getloseglist");
     * eprintf ("\n  Figure \"%s\" doesn't have a loseg list.\n",
     *          apPhfig->NAME);
     * EXIT (1);
     */
    return (NULL);
  }
  ptLoseg = (struct tLoseg_s*)(pType->DATA);


  pLoseg = NULL;

  pElem = searchauthelem (ptLoseg->tHT, aName);

  if (pElem) {
    __DBG(
      fflush (stdout);
      fprintf (stderr, "getloseglist:Loseg found (id %ld)\n", pElem->VALUE);
    )

    pLoseg = ptLoseg->tLoseg [pElem->VALUE];
             ptLoseg->tAccess[pElem->VALUE]++;
  }


  return (pLoseg);
}


/*  ------------------------------------------------------------------
 *  Function  :  "checklosegaccess()".
 */

extern void  checklosegaccess(apPhfig)
  struct phfig *apPhfig;
{
  struct tLoseg_s *ptLoseg;
  struct ptype    *pType;
             long  iLoseg;
             long  flag;


  if (!(pType = getptype (apPhfig->USER, PTYPE_TLOSEG))) {
    /*
     * eprinth ("getloseglist");
     * eprintf ("\n  Figure \"%s\" doesn't have a loseg list.\n",
     *          apPhfig->NAME);
     * EXIT (1);
     */
    return;
  }
  ptLoseg = (struct tLoseg_s*)(pType->DATA);


  flag = FALSE;
  for (iLoseg = 0; iLoseg < ptLoseg->sigNB; iLoseg++) {
    if (ptLoseg->tAccess[iLoseg] == 0) {
      if (!flag) {
        /* Print the head error message. */
        flag = TRUE;

        eprinth (NULL);
        eprintf ("The following physical net segments are not in the");
        eprintf (" netlist :\n");
        flag = TRUE;
      }

      eprintf ("- \"%s\".\n", getlosegname (ptLoseg->tLoseg[iLoseg]));
    }
  }


  if (flag) EXIT (1);
}


/*  ------------------------------------------------------------------
 *  Function  :  "shiftphfig()".
 */

extern void  shiftphfig(apPhfig, DX, DY)
  struct phfig *apPhfig;
          long  DX, DY;
{
  struct phseg *pSeg;
  struct phins *pIns;
  struct phvia *pVIA;
  struct phref *pRef;
  struct phcon *pCon;


  apPhfig->XAB1 -= DX;
  apPhfig->XAB2 -= DX;
  apPhfig->YAB1 -= DY;
  apPhfig->YAB2 -= DY;


  for (pSeg = apPhfig->PHSEG; pSeg != NULL; pSeg = pSeg->NEXT) {
    pSeg->X1 -= DX;
    pSeg->X2 -= DX;
    pSeg->Y1 -= DY;
    pSeg->Y2 -= DY;
  }

  for (pIns = apPhfig->PHINS; pIns != NULL; pIns = pIns->NEXT) {
    pIns->XINS -= DX;
    pIns->YINS -= DY;
  }

  for (pVIA = apPhfig->PHVIA; pVIA != NULL; pVIA = pVIA->NEXT) {
    pVIA->XVIA -= DX;
    pVIA->YVIA -= DY;
  }

  for (pRef = apPhfig->PHREF; pRef != NULL; pRef = pRef->NEXT) {
    pRef->XREF -= DX;
    pRef->YREF -= DY;
  }

  for (pCon = apPhfig->PHCON; pCon != NULL; pCon = pCon->NEXT) {
    pCon->XCON -= DX;
    pCon->YCON -= DY;
  }
}
