/*
 *  $Id: LEF_drive.c,v 1.4 2004/09/29 21:40:46 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./LEF_drive.c"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "LEF_drive.h"


/*  ------------------------------------------------------------------
 *  Global variables (declared `extern' in "UtDefs.h").
 */

       eMACRO_t *LEF_lMACRO        = (eMACRO_t*)NULL;


/*  ------------------------------------------------------------------
 *  Internal functions declarations.
 */

 static   char   *CLASStoa    __FP((char acCLASS));
 static   char   *DIRtoa      __FP((char acDIR));
 static   char   *USEtoa      __FP((char acUSE));
 static   char   *SHAPEtoa    __FP((char acSHAPE));
 static   char   *LAYERtoa    __FP((char acLAYER));
 static   char   *SYMMETRYtoa __FP((char acSYMMETRY));
 static   char   *pttoa       __FP((XY_t *apXY));
 static   char   *SIZEtoa     __FP((XY_t *apXY));
 static   void    fprintPATH  __FP((FILE *LEF, ePATH_t *alPATH));
 static   void    fprintPORT  __FP((FILE *LEF, ePORT_t *alPORT));
 static   void    fprintPIN   __FP((FILE *LEF, ePIN_t *apPIN));
 static   void    fprintOBS   __FP((FILE *LEF, eOBS_t *alOBS));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "allocRECT()".
 */

extern RECT_t *allocRECT(aX0, aY0, aX1, aY1)
  long  aX0, aY0, aX1, aY1;
{
  RECT_t *pRECT;

  pRECT            = (RECT_t*)malloc(sizeof(RECT_t));
  pRECT->pt0.x     = MBK2DEF_length (aX0);
  pRECT->pt0.y     = MBK2DEF_length (aY0);
  pRECT->pt1.x     = MBK2DEF_length (aX1);
  pRECT->pt1.y     = MBK2DEF_length (aY1);

  return(pRECT);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocPATH()".
 */

extern ePATH_t *allocPATH(alPATH, aX, aY)
  ePATH_t *alPATH;
     long  aX, aY;
{
  ePATH_t *pPATH;

  pPATH            = (ePATH_t*)malloc(sizeof(ePATH_t));
  pPATH->pt.x      = MBK2DEF_length (aX);
  pPATH->pt.y      = MBK2DEF_length (aY);

  /* Add the new element in head of list. */
  pPATH->Next      = alPATH;

  return(pPATH);
}


/*  ------------------------------------------------------------------
 *  Function  :  "revPATH()".
 */

extern ePATH_t *revPATH(apPATH)
  ePATH_t *apPATH;
{
  ePATH_t *pPATH, *lRevPATH, *pTmp;


  lRevPATH = (ePATH_t *)NULL;

  for(pPATH = apPATH; pPATH != (ePATH_t*)NULL;) {

    pTmp  = pPATH;
    pPATH = pPATH->Next;

    pTmp->Next = lRevPATH;
    lRevPATH   = pTmp;

  } /* End of "pPATH" loop. */

  return(lRevPATH);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocOBS()".
 */

extern eOBS_t *allocOBS(alOBS, aType, apData)
  eOBS_t *alOBS;
    char  aType;
    void *apData;
{
  eOBS_t *pOBS;

  pOBS            = (eOBS_t*)malloc(sizeof(eOBS_t));
  pOBS->Type      = aType;

  if (aType == C_OBSITEM_WIDTH)
    pOBS->Data    = (void*)(MBK2DEF_length ((long)apData));
  else
    pOBS->Data    = apData;

  /* Add the new element in head of list. */
  pOBS->Next      = alOBS;

  return(pOBS);
}


/*  ------------------------------------------------------------------
 *  Function  :  "revOBS()".
 */

extern eOBS_t *revOBS(apOBS)
  eOBS_t *apOBS;
{
  eOBS_t *pOBS, *lRevOBS, *pTmp;


  lRevOBS = (eOBS_t *)NULL;

  for(pOBS = apOBS; pOBS != (eOBS_t*)NULL;) {

    pTmp = pOBS;
    pOBS = pOBS->Next;

    pTmp->Next = lRevOBS;
    lRevOBS    = pTmp;

  } /* End of "pOBS" loop. */

  return(lRevOBS);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocPORT()".
 */

extern ePORT_t *allocPORT(alPORT, aType, apData)
  ePORT_t *alPORT;
    char  aType;
    void *apData;
{
  ePORT_t *pPORT;

  pPORT            = (ePORT_t*)malloc(sizeof(ePORT_t));
  pPORT->Type      = aType;

  if (aType == C_PORTITEM_WIDTH)
    pPORT->Data    = (void*)(MBK2DEF_length ((long)apData));
  else
    pPORT->Data    = apData;

  /* Add the new element in head of list. */
  pPORT->Next      = alPORT;

  return(pPORT);
}


/*  ------------------------------------------------------------------
 *  Function  :  "revPORT()".
 */

extern ePORT_t *revPORT(apPORT)
  ePORT_t *apPORT;
{
  ePORT_t *pPORT, *lRevPORT, *pTmp;


  lRevPORT = (ePORT_t *)NULL;

  for(pPORT = apPORT; pPORT != (ePORT_t*)NULL;) {

    pTmp  = pPORT;
    pPORT = pPORT->Next;

    pTmp->Next = lRevPORT;
    lRevPORT   = pTmp;

  } /* End of "pPORT" loop. */

  return(lRevPORT);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocPIN()".
 */

extern ePIN_t *allocPIN(alPin, asName)
  ePIN_t *alPin;
    char *asName;
{
  ePIN_t *pPIN;

  /* Allocate, and for security'sake, blank all the fields. */
  pPIN            = (ePIN_t*)malloc(sizeof(ePIN_t));
  pPIN->pinName   = namealloc(asName);
  pPIN->DIRECTION = C_DIRECTION_NONE;
  pPIN->USE       = C_USE_NONE;
  pPIN->SHAPE     = C_SHAPE_NONE;
  pPIN->lPORT     = (ePORT_t*)NULL;

  /* Add the new element in head of list. */
  pPIN->Next      = alPin;

  return(pPIN);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getPIN()".
 */

extern ePIN_t *getPIN(apMACRO, asName)
  eMACRO_t *apMACRO;
    char   *asName;
{
  ePIN_t *pPIN;


  for(pPIN = apMACRO->lPIN; pPIN != (ePIN_t*)NULL; pPIN = pPIN->Next) {
    if (pPIN->pinName == asName) return(pPIN);
  } /* End of "pPIN" loop. */

  return((ePIN_t*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "allocMACRO()".
 */

extern eMACRO_t *allocMACRO(asName)
  char *asName;
{
  eMACRO_t *pMACRO;

  /* Allocate, and for security'sake, blank all the fields. */
  pMACRO            = (eMACRO_t*)malloc(sizeof(eMACRO_t));
  pMACRO->macroName = namealloc(asName);
  pMACRO->CLASS     = C_CLASS_NONE;
  pMACRO->ORIGIN.x  = 0L;
  pMACRO->ORIGIN.y  = 0L;
  pMACRO->SIZE.x    = 0L;
  pMACRO->SIZE.y    = 0L;
  pMACRO->SYMMETRY  = F_SYMMETRY_NONE;
  pMACRO->SITE      = (char*)NULL;
  pMACRO->lPIN      = (ePIN_t*)NULL;
  pMACRO->lOBS      = (eOBS_t*)NULL;

  /* Add the new element in head of list. */
  pMACRO->Next      = LEF_lMACRO;
  LEF_lMACRO      = pMACRO;

  return(pMACRO);
}


/*  ------------------------------------------------------------------
 *  Function  :  "CLASStoa()".
 */

static char *CLASStoa(acCLASS)
  char  acCLASS;
{
  switch(acCLASS) {
    case C_CLASS_NONE:     return((char*)NULL);
    case C_CLASS_COVER:    return("COVER");
    case C_CLASS_RING:     return("RING");
    case C_CLASS_PAD:      return("PAD");
    case C_CLASS_CORE:     return("CORE");
    case C_CLASS_FEEDTHRU: return("CORE FEEDTHRU");
    case C_CLASS_ENDCAP:   return("ENDCAP");
    case C_CLASS_TOPRIGHT: return("ENDCAP TOPRIGHT");
    case C_CLASS_TOPLEFT:  return("ENDCAP TOPLEFT");
  }

  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "DIRtoa()".
 */

static char *DIRtoa(acDIR)
  char  acDIR;
{
  switch(acDIR) {
    case C_DIRECTION_NONE:     return((char*)NULL);
    case C_DIRECTION_INPUT:    return("INPUT");
    case C_DIRECTION_OUTPUT:   return("OUTPUT");
    case C_DIRECTION_TRISTATE: return("OUTPUT TRISTATE");
    case C_DIRECTION_INOUT:    return("INOUT");
    case C_DIRECTION_FEEDTHRU: return("FEEDTHRU");
  }

  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "USEtoa()".
 */

static char *USEtoa(acUSE)
  char  acUSE;
{
  switch(acUSE) {
    case C_USE_NONE:   return((char*)NULL);
    case C_USE_SIGNAL: return("SIGNAL");
    case C_USE_ANALOG: return("ANALOG");
    case C_USE_POWER:  return("POWER");
    case C_USE_GROUND: return("GROUND");
    case C_USE_CLOCK:  return("CLOCK");
  }

  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "SHAPEtoa()".
 */

static char *SHAPEtoa(acSHAPE)
  char  acSHAPE;
{
  switch(acSHAPE) {
    case C_SHAPE_NONE:     return((char*)NULL);
    case C_SHAPE_ABUTMENT: return("ABUTMENT");
    case C_SHAPE_RING:     return("RING");
    case C_SHAPE_FEEDTHRU: return("FEEDTHRU");
  }

  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "LAYERtoa()".
 */

static char *LAYERtoa(acLAYER)
  char  acLAYER;
{
  switch(acLAYER) {
    case  ALU1: return("ALU1");
    case  ALU2: return("ALU2");
    case  ALU3: return("ALU3");
    case  ALU4: return("ALU4");
    case  ALU5: return("ALU5");
    case  ALU6: return("ALU6");
    case CALU1: return("ALU1");
    case CALU2: return("ALU2");
    case CALU3: return("ALU3");
    case CALU4: return("ALU4");
    case CALU5: return("ALU5");
    case CALU6: return("ALU6");
  }

  return((char*)NULL);
}


/*  ------------------------------------------------------------------
 *  Function  :  "SYMMETRYtoa()".
 */

static char *SYMMETRYtoa(acSYMMETRY)
  char  acSYMMETRY;
{
  static char sSYMMETRY[32];
          int iSYMMETRY;

  iSYMMETRY = 0;

  if (acSYMMETRY & F_SYMMETRY_X) {
    sSYMMETRY[iSYMMETRY++] = 'X'; sSYMMETRY[iSYMMETRY++] = ' '; }

  if (acSYMMETRY & F_SYMMETRY_Y) {
    sSYMMETRY[iSYMMETRY++] = 'Y'; sSYMMETRY[iSYMMETRY++] = ' '; }

  if (acSYMMETRY & F_SYMMETRY_R90) {
    strncpy(sSYMMETRY + iSYMMETRY, "r90 ", 4);
    iSYMMETRY += 4;
  }

  sSYMMETRY[iSYMMETRY - 1] = (char)0;

  return(sSYMMETRY);
}


/*  ------------------------------------------------------------------
 *  Function  :  "pttoa()".
 */

static char *pttoa(apXY)
  XY_t *apXY;
{
  static char sXY[32];

  sprintf(sXY, "%.2f %.2f", MICRONS(apXY->x), MICRONS(apXY->y));

  return(sXY);
}


/*  ------------------------------------------------------------------
 *  Function  :  "SIZEtoa()".
 */

static char *SIZEtoa(apXY)
  XY_t *apXY;
{
  static char sXY[32];

  sprintf(sXY, "%.2f BY %.2f", MICRONS(apXY->x), MICRONS(apXY->y));

  return(sXY);
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintPATH()".
 */

static void  fprintPATH(LEF, alPATH)
     FILE *LEF;
  ePATH_t *alPATH;
{
  ePATH_t *pPATH;


  fprintf(LEF, "            PATH ");

  for(pPATH = alPATH; pPATH != (ePATH_t*)NULL; pPATH = pPATH->Next) {
    fprintf(LEF, "%s ", pttoa(&(pPATH->pt)));
  } /* End of "pPATH" loop. */

  fprintf(LEF, ";\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintPORT()".
 */

static void  fprintPORT(LEF, alPORT)
     FILE *LEF;
  ePORT_t *alPORT;
{
  ePORT_t *pPORT; 


  fprintf(LEF, "        PORT\n");

  for(pPORT = alPORT; pPORT != (ePORT_t*)NULL; pPORT = pPORT->Next) {
    switch(pPORT->Type) {

      case C_PORTITEM_LAYER:
        fprintf(LEF, "            LAYER %s ;\n",
                LAYERtoa((char)(long)((pPORT->Data))));
        break;

      case C_PORTITEM_WIDTH: 
        fprintf(LEF, "            WIDTH %.2f ;\n",
                MICRONS((long)(pPORT->Data)));
        break;

      case C_PORTITEM_RECT:
        fprintf(LEF, "            RECT %s ",
                pttoa(&((RECT_t*)pPORT->Data)->pt0));

        fprintf(LEF, "%s ;\n", pttoa(&((RECT_t*)pPORT->Data)->pt1));
        break;

      case C_PORTITEM_PATH:
        fprintPATH(LEF, (ePATH_t *)pPORT->Data);
        break;

    } /* End of "pPORT->Type" switch. */
  } /* End of "pPORT" loop. */

  fprintf(LEF, "        END\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintPIN()".
 */

static void  fprintPIN(LEF, apPIN)
    FILE *LEF;
  ePIN_t *apPIN;
{
  char  *sPin, sPinLEF[80];


  sPin = MBK2DEF_name(sPinLEF, apPIN->pinName);

  fprintf(LEF, "    PIN %s\n", sPin);

  if (apPIN->DIRECTION != C_DIRECTION_NONE) {
    fprintf(LEF, "        DIRECTION %s ;\n", DIRtoa(apPIN->DIRECTION)); }

  if (apPIN->USE != C_USE_NONE) {
    fprintf(LEF, "        USE %s ;\n", USEtoa(apPIN->USE)); }

  if (apPIN->SHAPE != C_SHAPE_NONE) {
    fprintf(LEF, "        SHAPE %s ;\n", SHAPEtoa(apPIN->SHAPE)); }

  if (apPIN->lPORT != (ePORT_t*)NULL)
    fprintPORT(LEF, apPIN->lPORT);

  fprintf(LEF, "    END %s\n", sPin);
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintOBS()".
 */

static void  fprintOBS(LEF, alOBS)
    FILE *LEF;
  eOBS_t *alOBS;
{
  eOBS_t *pOBS;


  if (alOBS != (eOBS_t*)NULL) fprintf(LEF, "    OBS\n");

  for(pOBS = alOBS; pOBS != (eOBS_t*)NULL; pOBS = pOBS->Next) {
    switch(pOBS->Type) {

      case C_OBSITEM_LAYER: 
        fprintf(LEF, "        LAYER %s ;\n",
                LAYERtoa((char)(long)((pOBS->Data))));
        break;

      case C_OBSITEM_WIDTH: 
        fprintf(LEF, "        WIDTH %.2f ;\n",
                MICRONS((long)(pOBS->Data)));
        break;

      case C_OBSITEM_RECT:
        fprintf(LEF, "        RECT %s ",
                pttoa(&((RECT_t*)pOBS->Data)->pt0));

        fprintf(LEF, "%s ;\n", pttoa(&((RECT_t*)pOBS->Data)->pt1));
        break;

      case C_OBSITEM_PATH:
        fprintPATH(LEF, (ePATH_t *)pOBS->Data);
        break;

    } /* End of "pOBS->Type" switch. */
  } /* End of "pOBS" loop. */

  if (alOBS != (eOBS_t*)NULL) fprintf(LEF, "    END\n");
}


/*  ------------------------------------------------------------------
 *  Function  :  "fprintMACRO()".
 */

extern void  fprintMACRO(LEF, apMACRO)
  FILE *LEF;
  eMACRO_t *apMACRO;
{
  ePIN_t *pPIN;


  fprintf(LEF, "MACRO %s\n", apMACRO->macroName);


  if (apMACRO->CLASS != C_CLASS_NONE) {
    fprintf(LEF, "    CLASS     %s ;\n", CLASStoa(apMACRO->CLASS)); }

  fprintf(LEF, "    ORIGIN    %s ;\n",   pttoa(&(apMACRO->ORIGIN)));
  fprintf(LEF, "    SIZE      %s ;\n", SIZEtoa(&(apMACRO->SIZE  )));

  if (apMACRO->SYMMETRY != F_SYMMETRY_NONE) {
    fprintf(LEF, "    SYMMETRY  %s ;\n", SYMMETRYtoa(apMACRO->SYMMETRY)); }

  if (apMACRO->SITE != (char*)NULL) {
    fprintf(LEF, "    SITE      %s ;\n", apMACRO->SITE); }


  for(pPIN = apMACRO->lPIN; pPIN != (ePIN_t *)NULL; pPIN = pPIN->Next)
    fprintPIN(LEF, pPIN);


  fprintOBS(LEF, apMACRO->lOBS);


  fprintf(LEF, "END %s\n\n", apMACRO->macroName);
}
