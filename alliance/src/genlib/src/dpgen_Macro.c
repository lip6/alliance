/*
 *  $Id: dpgen_Macro.c,v 1.2 2002/06/17 09:40:27 fred Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-support@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  Module    :         "./dpgen_Macro.c"                         |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include   "util_Defs.h"
# include  "dpgen_Defs.h"
# include  "dgn.h"
# include  "debugoff.h"


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "function_To_String()".
 */

extern char *function_To_String(aFunction)
  long  aFunction;
{
  switch (aFunction) {
    case DPGEN_INV:       return ("DPGEN_INV");
    case DPGEN_BUFF:      return ("DPGEN_BUFF");
    case DPGEN_NAND2:     return ("DPGEN_NAND2");
    case DPGEN_NAND3:     return ("DPGEN_NAND3");
    case DPGEN_NAND4:     return ("DPGEN_NAND4");
    case DPGEN_AND2:      return ("DPGEN_AND2");
    case DPGEN_AND3:      return ("DPGEN_AND3");
    case DPGEN_AND4:      return ("DPGEN_AND4");
    case DPGEN_NOR2:      return ("DPGEN_NOR2");
    case DPGEN_NOR3:      return ("DPGEN_NOR3");
    case DPGEN_NOR4:      return ("DPGEN_NOR4");
    case DPGEN_OR2:       return ("DPGEN_OR2");
    case DPGEN_OR3:       return ("DPGEN_OR3");
    case DPGEN_OR4:       return ("DPGEN_OR4");
    case DPGEN_XNOR2:     return ("DPGEN_XNOR2");
    case DPGEN_XOR2:      return ("DPGEN_XOR2");
    case DPGEN_NMUX2:     return ("DPGEN_NMUX2");
    case DPGEN_MUX2:      return ("DPGEN_MUX2");
    case DPGEN_NBUSE:     return ("DPGEN_NBUSE");
    case DPGEN_BUSE:      return ("DPGEN_BUSE");
    case DPGEN_NAND2MASK: return ("DPGEN_NAND2MASK");
    case DPGEN_NOR2MASK:  return ("DPGEN_NOR2MASK");
    case DPGEN_XNOR2MASK: return ("DPGEN_XNOR2MASK");
    case DPGEN_CONST:     return ("DPGEN_CONST");
    case DPGEN_DFF:       return ("DPGEN_DFF");
    case DPGEN_DFFT:      return ("DPGEN_DFFT");
    case DPGEN_SFF:       return ("DPGEN_SFF");
    case DPGEN_SFFT:      return ("DPGEN_SFFT");
    case DPGEN_ADSB2F:    return ("DPGEN_ADSB2F");
    case DPGEN_SHIFT:     return ("DPGEN_SHIFT");
    case DPGEN_NUL:       return ("DPGEN_NUL");
    case DPGEN_RF1:       return ("DPGEN_RF1");
    case DPGEN_FIFO:      return ("DPGEN_FIFO");
    default:              return ("unknown?!");
  }
}


/*  ------------------------------------------------------------------
 *  Function  :  "GENLIB_MACRO()".
 */

extern void  GENLIB_MACRO __FPV((long  aFunction, ...))
{
              va_list  aAL;
  __KR_C(        long  aFunction;        )
          static long  firstCall = TRUE;


  if (firstCall) {
    firstCall = FALSE;
    util_init (C_VerboseLevel0, F_DUMPCORE, "dpgen");
  }


  __ANSI_C( va_start (aAL, aFunction); )
  __KR_C(   va_start (aAL); )
  __KR_C(   aFunction = va_arg (aAL, long); )


  switch (aFunction) {
    case DPGEN_INV:
    case DPGEN_BUFF:
    case DPGEN_NAND2:
    case DPGEN_NAND3:
    case DPGEN_NAND4:
    case DPGEN_AND2:
    case DPGEN_AND3:
    case DPGEN_AND4:
    case DPGEN_NOR2:
    case DPGEN_NOR3:
    case DPGEN_NOR4:
    case DPGEN_OR2:
    case DPGEN_OR3:
    case DPGEN_OR4:
    case DPGEN_XNOR2:
    case DPGEN_XOR2:
    case DPGEN_NMUX2:
    case DPGEN_MUX2:
    case DPGEN_NBUSE:
    case DPGEN_BUSE:
    case DPGEN_NAND2MASK:
    case DPGEN_NOR2MASK:
    case DPGEN_XNOR2MASK:
    case DPGEN_CONST:
    case DPGEN_DFF:
    case DPGEN_DFFT:
    case DPGEN_SFF:
    case DPGEN_SFFT:
      dpgen_Regular (aFunction, aAL);
      break;
    case DPGEN_ROM2:
    case DPGEN_ROM4:
      dpgen_uROM (aFunction, aAL);
      break;
    case DPGEN_ADSB2F:
      dpgen_Adder (aFunction, F_ADDER_SIGN, aAL);
      break;
    case DPGEN_SHIFT:
      dpgen_Shifter (aFunction, aAL);
      break;
    case DPGEN_SHROT:
      dpgen_Shifter (aFunction, aAL);
      break;
    case DPGEN_NUL:
      dpgen_Nul (aFunction, aAL);
      break;
    case DPGEN_RF1:
      dpgen_RF1 (aFunction, aAL);
      break;
    case DPGEN_RF1R0:
      dpgen_RF1 (aFunction, aAL);
      break;
    case DPGEN_RF1D:
      dpgen_RF1 (aFunction, aAL);
      break;
    case DPGEN_RF1DR0:
      dpgen_RF1 (aFunction, aAL);
      break;
    case DPGEN_FIFO:
      dpgen_RF1 (aFunction, aAL);
      break;
    case DPGEN_RF2:
      dpgen_RF2 (aFunction, aAL);
      break;
    case DPGEN_RF2R0:
      dpgen_RF2 (aFunction, aAL);
      break;
    case DPGEN_RF2D:
      dpgen_RF2 (aFunction, aAL);
      break;
    case DPGEN_RF2DR0:
      dpgen_RF2 (aFunction, aAL);
      break;
    default:
      eprinth (NULL);
      eprintf ("GENLIB_MACRO: Unknown function Id: %ld.\n", aFunction);
      eprints ("  Please have a look to \"dgnVRR.h\" in which all ");
      eprints (  "authorized\n   values are listed.\n");
      EXIT (1);
  }

  va_end(aAL);
}
