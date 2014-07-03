/*
 *  $Id: util_LEFDEF.c,v 1.3 2004/09/29 21:40:46 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_LEFDEF.c"                         |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "debugoff.h"
# include  "util_Defs.h"


/*  ------------------------------------------------------------------
 *  Global variables (declared `extern' in "util_Defs.h").
 */

         long    DEF_UNITS_MICRONS = 100L;


  static char *getRowToken __FP((char *aS, char *aSepar, char *aRefName));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_length()".
 */

extern long DEF2MBK_length(aL)
  long  aL;
{
  return ((aL * SCALE_X) / DEF_UNITS_MICRONS);
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_length()".
 */

extern long  MBK2DEF_length(aL)
  long  aL;
{
  return ((aL * DEF_UNITS_MICRONS) / SCALE_X);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_name()".
 */

extern char *DEF2MBK_name(aS)
    char *aS;
{
   int  iS;
  char *sTmp, *sAlloc, *pDelim;


  iS = 0; if (aS[0] == '|') iS++;
  sTmp = strdup (&aS[iS]);

  /* Translate '|' (hierarchical separator) to SEPAR. */
  for (iS = 0; sTmp[iS] != '\0'; iS++)
    if (sTmp[iS] == '|') sTmp[iS] = SEPAR;

  /* Translate vectorisation : "name<2>" --> "name 2". */
  if ((pDelim = strchr (sTmp, '<'))) *pDelim = ' ';
  if ((pDelim = strchr (sTmp, '>'))) *pDelim = '\0';
  if ((pDelim = strchr (sTmp, '('))) *pDelim = ' ';
  if ((pDelim = strchr (sTmp, ')'))) *pDelim = '\0';

  sAlloc = namealloc (sTmp);
  free (sTmp);


  return (sAlloc);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_row()".
 */

extern struct phref *DEF2MBK_row(apPhfig,
                                 aRowName, aRowType,
                                 aOrient,
                                 aDoNumber, aByNumber,
                                 aStepX, aStepY,
                                 aX, aY)
  struct phfig *apPhfig;
          char *aRowName, *aRowType;
          long  aOrient;
          long  aDoNumber, aByNumber;
          long  aStepX, aStepY;
          long  aX, aY;
{
  static char  ref_name[16384];


  sprintf (ref_name,
           "%s.%s.%s_do_%ld_by_%ld_step_%ld_%ld",
           aRowName,
           aRowType,
           DEF_orient2a(aOrient),
           aDoNumber,
           aByNumber,
           aStepX,
           aStepY);

  return (addphref(apPhfig,
                   "ref_ref",
                   ref_name,
                   aX,
                   aY));
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_track()".
 */

extern struct phref *DEF2MBK_track(apPhfig,
                                   aAxisName,
                                   aStart,
                                   aDoNumber,
                                   aStep,
                                   asLayers,
                                   aX, aY)
  struct phfig *apPhfig;
          char *aAxisName;
          long  aStart;
          long  aDoNumber;
          long  aStep;
          char *asLayers;
          long  aX, aY;
{
  static char  ref_name[16384];


  sprintf (ref_name,
           "tracks.%s_%ld_do_%ld_step_%ld.%s",
           aAxisName,
           aStart,
           aDoNumber,
           aStep,
           asLayers);

  return (addphref(apPhfig,
                   "ref_ref",
                   ref_name,
                   aX,
                   aY));
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_blockage()".
 */

extern struct phref *DEF2MBK_blockage(apPhfig, aX, aY, aWidth, aHeight)
  struct phfig *apPhfig;
          long  aX, aY, aWidth, aHeight;
{
  static char  ref_name[16384];


  sprintf (ref_name,
           "blockage.%ld_%ld",
           aWidth,
           aHeight);

  return (addphref(apPhfig,
                   "ref_ref",
                   ref_name,
                   aX,
                   aY));
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_name()".
 */

extern char *MBK2DEF_name(asLEF, asMBK)
  char *asLEF, *asMBK;
{
  char  sTmp[1024], *pS, *pI;


  strncpy (sTmp, asMBK, 1023);

  /* Find the signal index separator ' '. */
  pI = (char*)NULL;
  for(pS = sTmp; (*pS) != (char)0; pS++) if ((*pS) == ' ') pI = pS;


  if (pI != (char*)NULL) {
    *pI = (char)0; pI += 1;
    sprintf (asLEF, "%s(%s)", sTmp, pI);
  } else {
    sprintf (asLEF, "%s", sTmp);
  }

  return (asLEF);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_transf()".
 */

extern char DEF2MBK_transf(aO)
  double  aO;
{
  switch ((long)aO) {
    case DEF_N:  return (NOSYM);
    case DEF_S:  return (SYMXY);
    case DEF_W:  return (ROT_P);
    case DEF_E:  return (ROT_M);
    case DEF_FN: return (SYM_X);
    case DEF_FS: return (SYM_Y);
    case DEF_FW: return (SY_RM);
    case DEF_FE: return (SY_RP);
  }

  return (NOSYM);
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_direction()".
 */

extern char  MBK2DEF_direction(acDir)
  char  acDir;
{
  switch(acDir) {
    case IN:       return (C_DIRECTION_INPUT);
    case OUT:      return (C_DIRECTION_OUTPUT);
    case INOUT:    return (C_DIRECTION_INOUT);
    case TRANSCV:
    case TRISTATE: return (C_DIRECTION_TRISTATE);
  }

  return (C_DIRECTION_NONE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_locondir()".
 */

extern char  MBK2DEF_locondir (apLocon)
  locon_list *apLocon;
{
  char  dir;


  dir = MBK2DEF_direction (apLocon->DIRECTION);

  if (dir == C_DIRECTION_NONE) {
    dir = C_DIRECTION_INPUT;
    wprinth (NULL);
    wprintf ("Unknown direction for terminal \"%s\" (forced \"INPUT\").\n",
             apLocon->NAME);
  }

  return (dir);
}


/*  ------------------------------------------------------------------
 *  Function  :  "BEH2DEF_direction()".
 */

extern char  BEH2DEF_direction(acDir)
  char  acDir;
{
  switch (acDir) {
    case 'I': return (C_DIRECTION_INPUT);
    case 'O': return (C_DIRECTION_OUTPUT);
    case 'B': return (C_DIRECTION_INOUT);
    case 'Z':
    case 'T': return (C_DIRECTION_TRISTATE);
  }

  return (C_DIRECTION_NONE);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF2MBK_direction()".
 */

extern char  DEF2MBK_direction(aDEFdir)
  double  aDEFdir;
{
  switch ((long)aDEFdir) {
    case DEF_INPUT:    return (IN);
    case DEF_OUTPUT:   return (OUT);
    case DEF_INOUT:    return (INOUT);
    case DEF_FEEDTHRU: return (INOUT);
  }

  return (UNKNOWN);
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_transf()".
 */

extern char MBK2DEF_transf(aO)
  char  aO;
{
  switch (aO) {
    case NOSYM: return (DEF_N);
    case SYMXY: return (DEF_S);
    case ROT_P: return (DEF_W);
    case ROT_M: return (DEF_E);
    case SYM_X: return (DEF_FN);
    case SYM_Y: return (DEF_FS);
    case SY_RP: return (DEF_FW);
    case SY_RM: return (DEF_FE);
  }

  return (DEF_N);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getRowToken()".
 */

static char *getRowToken(aS, aSepar, aRefName)
  char *aS, *aSepar, *aRefName;
{
  char *token;

  if ((token = strtok (aS, aSepar)) == NULL) {
    errMBK  ("defsavelophfig");
    eprintf ("Malformed row reference name :\n  \"%s\"\n",
             aRefName);
    EXIT (1);
  }

  return (token);
}



/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_row()".
 */

extern void  MBK2DEF_row(apPhref,
                         appRowName, appRowType,
                         apOrient,
                         apDoNumber, apByNumber,
                         apStepX, apStepY,
                         apX, apY)
  struct phref  *apPhref;
          char **appRowName, **appRowType;
          long  *apOrient;
          long  *apDoNumber, *apByNumber;
          long  *apStepX, *apStepY;
          long  *apX, *apY;
{
  static char  ref_name[16384];
         char *token;


  strcpy (ref_name, apPhref->NAME);

  token = getRowToken (ref_name, ".", apPhref->NAME);
  *appRowName = (char*)mbkalloc (sizeof (char) * strlen (token));
  strcpy  (*appRowName, token);
  strtoup (*appRowName);

  token = getRowToken (NULL, ".", apPhref->NAME);
  *appRowType = (char*)mbkalloc (sizeof (char) * strlen (token));
  strcpy  (*appRowType, token);

  token = strtoup (getRowToken (NULL, "_", apPhref->NAME));
  *apOrient = DEF_a2orient (token);

/*token =*/ strtoup (getRowToken (NULL, "_", apPhref->NAME));
  token = strtoup (getRowToken (NULL, "_", apPhref->NAME));
  *apDoNumber = atol (token);

/*token =*/ strtoup (getRowToken (NULL, "_", apPhref->NAME));
  token = strtoup (getRowToken (NULL, "_", apPhref->NAME));
  *apByNumber = atol (token);

/*token =*/ strtoup (getRowToken (NULL, "_", apPhref->NAME));
  token = strtoup (getRowToken (NULL, "_", apPhref->NAME));
  *apStepX = atol (token);

  token = strtoup (getRowToken (NULL, "_", apPhref->NAME));
  *apStepY = atol (token);

  *apX = apPhref->XREF;
  *apY = apPhref->YREF;
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_track()".
 */

extern void  MBK2DEF_track(apPhref,
                           apAxisName,
                           apStart,
                           apDoNumber,
                           apStep,
                           apsLayers,
                           apX, apY)
  struct phref  *apPhref;
          char  *apAxisName;
          long  *apStart;
          long  *apDoNumber;
          long  *apStep;
          char **apsLayers;
          long  *apX, *apY;
{
  static char  ref_name[16384];
         char *token;


  strcpy (ref_name, apPhref->NAME);

/*token =*/ getRowToken (ref_name, ".", apPhref->NAME);
  token = getRowToken (NULL, "_", apPhref->NAME);
  *apAxisName = (char)toupper ((int)*token);

/*token =*/ getRowToken (NULL, "_", apPhref->NAME);
  *apStart = atol (token);

/*token =*/ getRowToken (NULL, "_", apPhref->NAME);
  token = getRowToken (NULL, "_", apPhref->NAME);
  *apDoNumber = atol (token);

/*token =*/ getRowToken (NULL, "_", apPhref->NAME);
  token = getRowToken (NULL, ".", apPhref->NAME);
  *apStep = atol (token);

  token = getRowToken (NULL, ";", apPhref->NAME);
  *apsLayers = (char*)mbkalloc (sizeof (char) * strlen (token));
  strcpy  (*apsLayers, token);
  strtoup (*apsLayers);

  for (token = *apsLayers; *token != '\0'; token++)
    if (*token == '.') *token = ' ';

  *apX = apPhref->XREF;
  *apY = apPhref->YREF;
}


/*  ------------------------------------------------------------------
 *  Function  :  "MBK2DEF_blockage()".
 */

extern void  MBK2DEF_blockage(apPhref, apXB1, apYB1, apWidth, apHeight)
  struct phref *apPhref;
          long *apXB1, *apYB1, *apWidth, *apHeight;
{
  static char  ref_name[16384];
         char *token;


  strcpy (ref_name, apPhref->NAME);

  token = getRowToken (ref_name, ".", apPhref->NAME);
  token = getRowToken (NULL, "_", apPhref->NAME);
  *apWidth = atol (token);

  token = getRowToken (NULL, "_", apPhref->NAME);
  *apHeight = atol (token);

  *apXB1 = apPhref->XREF;
  *apYB1 = apPhref->YREF;
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF_orient2a()".
 */

extern char *DEF_orient2a(aT)
  long  aT;
{
  static char *tTransf[] = { "N", "S", "W", "E", "FN", "FS", "FW", "FE" };


  switch (aT) {
    case DEF_N:  return (tTransf[0]);
    case DEF_S:  return (tTransf[1]);
    case DEF_W:  return (tTransf[2]);
    case DEF_E:  return (tTransf[3]);
    case DEF_FN: return (tTransf[4]);
    case DEF_FS: return (tTransf[5]);
    case DEF_FW: return (tTransf[6]);
    case DEF_FE: return (tTransf[7]);
  }

  return (tTransf[0]);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF_a2orient()".
 */

extern long  DEF_a2orient(aT)
  char *aT;
{
  if (!strcmp (aT,  "N")) return (DEF_N);
  if (!strcmp (aT,  "S")) return (DEF_S);
  if (!strcmp (aT,  "W")) return (DEF_W);
  if (!strcmp (aT,  "E")) return (DEF_E);
  if (!strcmp (aT, "FN")) return (DEF_FN);
  if (!strcmp (aT, "FS")) return (DEF_FS);
  if (!strcmp (aT, "FW")) return (DEF_FW);
  if (!strcmp (aT, "FE")) return (DEF_FE);

  return (DEF_N);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF_side2a()".
 */

extern char *DEF_side2a(aSide)
  char  aSide;
{
  static char *tTransf[] = { "N", "S", "W", "E" };


  switch (aSide) {
    case SOUTH:  return (tTransf[0]);
    case NORTH:  return (tTransf[1]);
    case EAST:  return (tTransf[2]);
    case WEST:  return (tTransf[3]);
  }

  return (tTransf[0]);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF_layer2a()".
 */

extern char *DEF_layer2a(aLayer)
  char aLayer;
{
  char *layerName;


  __DBG (fprintf (stderr, "layer id %d\n", (int)aLayer); )
  switch (aLayer) {
    case ALU1:      layerName = "ALU1"; break;
    case ALU2:      layerName = "ALU2"; break;
    case ALU3:      layerName = "ALU3"; break;
    case ALU4:      layerName = "ALU4"; break;
    case ALU5:      layerName = "ALU5"; break;
    case ALU6:      layerName = "ALU6"; break;
    case ALU7:      layerName = "ALU7"; break;
    case ALU8:      layerName = "ALU8"; break;
    case CALU1:     layerName = "ALU1"; break;
    case CALU2:     layerName = "ALU2"; break;
    case CALU3:     layerName = "ALU3"; break;
    case CALU4:     layerName = "ALU4"; break;
    case CALU5:     layerName = "ALU5"; break;
    case CALU6:     layerName = "ALU6"; break;
    case CALU7:     layerName = "ALU7"; break;
    case CALU8:     layerName = "ALU8"; break;
    case CONT_VIA:  layerName = "VIA1"; break;
    case CONT_VIA2: layerName = "VIA2"; break;
    case CONT_VIA3: layerName = "VIA3"; break;
    case CONT_VIA4: layerName = "VIA4"; break;
    case CONT_VIA5: layerName = "VIA5"; break;
    case CONT_VIA6: layerName = "VIA6"; break;
    case CONT_VIA7: layerName = "VIA7"; break;
    default:   layerName = "LAYER_MBK_ERROR"; break;
  }

  return (layerName);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DEF_via2a()".
 */

extern char *DEF_via2a(aType, aDX, aDY, aViaName)
  char  aType;
  long  aDX, aDY;
  char *aViaName;
{
  long  flagDefault, length;


  switch (aType) {
    case CONT_VIA:  strcpy (aViaName, "CONT_VIA");  break;
    case CONT_VIA2: strcpy (aViaName, "CONT_VIA2"); break;
    case CONT_VIA3: strcpy (aViaName, "CONT_VIA3"); break;
    case CONT_VIA4: strcpy (aViaName, "CONT_VIA4"); break;
    case CONT_VIA5: strcpy (aViaName, "CONT_VIA5"); break;
    case CONT_VIA6: strcpy (aViaName, "CONT_VIA6"); break;
    case CONT_VIA7: strcpy (aViaName, "CONT_VIA7"); break;
    default:        strcpy (aViaName, "MBK_ERROR"); break;
  }
  length = strlen (aViaName);

  flagDefault =    (aDX <= MBKSCALE(2))
                && (aDY <= MBKSCALE(2));

  if (!flagDefault) {
    sprintf (aViaName + length, "_%ld_%ld",
             MBK2DEF_length (aDX),
             MBK2DEF_length (aDY));
  }

  return (aViaName);
}
