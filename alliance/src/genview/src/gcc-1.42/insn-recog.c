/* Generated automatically by the program `genrecog'
from the machine description file `md'.  */

#include "config.h"
#include "rtl.h"
#include "insn-config.h"
#include "recog.h"
#include "real.h"

/* `recog' contains a decision tree
   that recognizes whether the rtx X0 is a valid instruction.

   recog returns -1 if the rtx is not valid.
   If the rtx is valid, recog returns a nonnegative number
   which is the insn code number for the pattern that matched.
   This is the same as the order in the machine description of
   the entry that matched.  This number can be used as an index into
   insn_templates and insn_n_operands (found in insn-output.c)
   or as an argument to output_insn_hairy (also in insn-output.c).  */

rtx recog_operand[MAX_RECOG_OPERANDS];

rtx *recog_operand_loc[MAX_RECOG_OPERANDS];

rtx *recog_dup_loc[MAX_DUP_OPERANDS];

char recog_dup_num[MAX_DUP_OPERANDS];

extern rtx recog_addr_dummy;

#define operands recog_operand

int
recog_1 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L415:
  x1 = XEXP (x0, 1);
  x2 = XEXP (x1, 0);
 switch (GET_CODE (x2))
  {
  case EQ:
  if (1)
    goto L416;
  break;
  case NE:
  if (1)
    goto L425;
  break;
  case GT:
  if (1)
    goto L434;
  break;
  case GTU:
  if (1)
    goto L443;
  break;
  case LT:
  if (1)
    goto L452;
  break;
  case LTU:
  if (1)
    goto L461;
  break;
  case GE:
  if (1)
    goto L470;
  break;
  case GEU:
  if (1)
    goto L479;
  break;
  case LE:
  if (1)
    goto L488;
  break;
  case LEU:
  if (1)
    goto L497;
  break;
  }
  goto ret0;
 L416:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L417;
  goto ret0;
 L417:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L418;
  goto ret0;
 L418:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L419;
  if (x2 == pc_rtx && 1)
    goto L509;
  goto ret0;
 L419:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L420; }
  goto ret0;
 L420:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 91;
  goto ret0;
 L509:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L510;
  goto ret0;
 L510:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 101; }
  goto ret0;
 L425:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L426;
  goto ret0;
 L426:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L427;
  goto ret0;
 L427:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L428;
  if (x2 == pc_rtx && 1)
    goto L518;
  goto ret0;
 L428:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L429; }
  goto ret0;
 L429:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 92;
  goto ret0;
 L518:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L519;
  goto ret0;
 L519:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 102; }
  goto ret0;
 L434:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L435;
  goto ret0;
 L435:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L436;
  goto ret0;
 L436:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L437;
  if (x2 == pc_rtx && 1)
    goto L527;
  goto ret0;
 L437:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L438; }
  goto ret0;
 L438:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 93;
  goto ret0;
 L527:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L528;
  goto ret0;
 L528:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 103; }
  goto ret0;
 L443:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L444;
  goto ret0;
 L444:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L445;
  goto ret0;
 L445:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L446;
  if (x2 == pc_rtx && 1)
    goto L536;
  goto ret0;
 L446:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L447; }
  goto ret0;
 L447:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 94;
  goto ret0;
 L536:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L537;
  goto ret0;
 L537:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 104; }
  goto ret0;
 L452:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L453;
  goto ret0;
 L453:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L454;
  goto ret0;
 L454:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L455;
  if (x2 == pc_rtx && 1)
    goto L545;
  goto ret0;
 L455:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L456; }
  goto ret0;
 L456:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 95;
  goto ret0;
 L545:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L546;
  goto ret0;
 L546:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 105; }
  goto ret0;
 L461:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L462;
  goto ret0;
 L462:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L463;
  goto ret0;
 L463:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L464;
  if (x2 == pc_rtx && 1)
    goto L554;
  goto ret0;
 L464:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L465; }
  goto ret0;
 L465:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 96;
  goto ret0;
 L554:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L555;
  goto ret0;
 L555:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 106; }
  goto ret0;
 L470:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L471;
  goto ret0;
 L471:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L472;
  goto ret0;
 L472:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L473;
  if (x2 == pc_rtx && 1)
    goto L563;
  goto ret0;
 L473:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L474; }
  goto ret0;
 L474:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 97;
  goto ret0;
 L563:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L564;
  goto ret0;
 L564:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 107; }
  goto ret0;
 L479:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L480;
  goto ret0;
 L480:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L481;
  goto ret0;
 L481:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L482;
  if (x2 == pc_rtx && 1)
    goto L572;
  goto ret0;
 L482:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L483; }
  goto ret0;
 L483:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 98;
  goto ret0;
 L572:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L573;
  goto ret0;
 L573:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 108; }
  goto ret0;
 L488:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L489;
  goto ret0;
 L489:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L490;
  goto ret0;
 L490:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L491;
  if (x2 == pc_rtx && 1)
    goto L581;
  goto ret0;
 L491:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L492; }
  goto ret0;
 L492:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 99;
  goto ret0;
 L581:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L582;
  goto ret0;
 L582:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 109; }
  goto ret0;
 L497:
  x3 = XEXP (x2, 0);
  if (x3 == cc0_rtx && 1)
    goto L498;
  goto ret0;
 L498:
  x3 = XEXP (x2, 1);
  if (x3 == const0_rtx && 1)
    goto L499;
  goto ret0;
 L499:
  x2 = XEXP (x1, 1);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L500;
  if (x2 == pc_rtx && 1)
    goto L590;
  goto ret0;
 L500:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; goto L501; }
  goto ret0;
 L501:
  x2 = XEXP (x1, 2);
  if (x2 == pc_rtx && 1)
    return 100;
  goto ret0;
 L590:
  x2 = XEXP (x1, 2);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L591;
  goto ret0;
 L591:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[0] = x3; return 110; }
  goto ret0;
 ret0: return -1;
}

int
recog_2 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L42:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, QImode))
    { recog_operand[1] = x1; return 10; }
 L85:
  if (GET_MODE (x1) != QImode)
    goto ret0;
 switch (GET_CODE (x1))
  {
  case TRUNCATE:
  if (1)
    goto L86;
  break;
  case FIX:
  if (1)
    goto L126;
  break;
  case UNSIGNED_FIX:
  if (1)
    goto L156;
  break;
  case PLUS:
  if (1)
    goto L186;
  break;
  case MULT:
  if (1)
    goto L211;
  break;
  case DIV:
  if (1)
    goto L236;
  break;
  case MINUS:
  if (1)
    goto L261;
  break;
  case AND:
  if (1)
    goto L286;
  break;
  case IOR:
  if (1)
    goto L301;
  break;
  case XOR:
  if (1)
    goto L316;
  break;
  case NEG:
  if (1)
    goto L331;
  break;
  case NOT:
  if (1)
    goto L343;
  break;
  case ASHIFT:
  if (1)
    goto L355;
  break;
  case ASHIFTRT:
  if (1)
    goto L370;
  break;
  case LSHIFT:
  if (1)
    goto L385;
  break;
  case LSHIFTRT:
  if (1)
    goto L400;
  break;
  }
  goto ret0;
 L86:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 22; }
 L94:
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 24; }
  goto ret0;
 L126:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  if (GET_MODE (x2) == SFmode && 1)
    goto L127;
  if (GET_MODE (x2) == DFmode && 1)
    goto L142;
  goto ret0;
 L127:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; return 32; }
  goto ret0;
 L142:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; return 35; }
  goto ret0;
 L156:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  if (GET_MODE (x2) == SFmode && 1)
    goto L157;
  if (GET_MODE (x2) == DFmode && 1)
    goto L172;
  goto ret0;
 L157:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; return 38; }
  goto ret0;
 L172:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; return 41; }
  goto ret0;
 L186:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L187; }
  goto ret0;
 L187:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 44; }
  goto ret0;
 L211:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L212; }
  goto ret0;
 L212:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 49; }
  goto ret0;
 L236:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L237; }
  goto ret0;
 L237:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 54; }
  goto ret0;
 L261:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L262; }
  goto ret0;
 L262:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 59; }
  goto ret0;
 L286:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L287; }
  goto ret0;
 L287:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 64; }
  goto ret0;
 L301:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L302; }
  goto ret0;
 L302:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 67; }
  goto ret0;
 L316:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L317; }
  goto ret0;
 L317:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 70; }
  goto ret0;
 L331:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 73; }
  goto ret0;
 L343:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 76; }
  goto ret0;
 L355:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L356; }
  goto ret0;
 L356:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 79; }
  goto ret0;
 L370:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L371; }
  goto ret0;
 L371:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 82; }
  goto ret0;
 L385:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L386; }
  goto ret0;
 L386:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 85; }
  goto ret0;
 L400:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; goto L401; }
  goto ret0;
 L401:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[2] = x2; return 88; }
  goto ret0;
 ret0: return -1;
}

int
recog_3 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L45:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, HImode))
    { recog_operand[1] = x1; return 11; }
 L57:
  if (GET_MODE (x1) != HImode)
    goto ret0;
 switch (GET_CODE (x1))
  {
  case SIGN_EXTEND:
  if (1)
    goto L58;
  break;
  case ZERO_EXTEND:
  if (1)
    goto L74;
  break;
  case TRUNCATE:
  if (1)
    goto L90;
  break;
  case FIX:
  if (1)
    goto L131;
  break;
  case UNSIGNED_FIX:
  if (1)
    goto L161;
  break;
  case PLUS:
  if (1)
    goto L191;
  break;
  case MULT:
  if (1)
    goto L216;
  break;
  case DIV:
  if (1)
    goto L241;
  break;
  case MINUS:
  if (1)
    goto L266;
  break;
  case AND:
  if (1)
    goto L291;
  break;
  case IOR:
  if (1)
    goto L306;
  break;
  case XOR:
  if (1)
    goto L321;
  break;
  case NEG:
  if (1)
    goto L335;
  break;
  case NOT:
  if (1)
    goto L347;
  break;
  case ASHIFT:
  if (1)
    goto L360;
  break;
  case ASHIFTRT:
  if (1)
    goto L375;
  break;
  case LSHIFT:
  if (1)
    goto L390;
  break;
  case LSHIFTRT:
  if (1)
    goto L405;
  break;
  }
  goto ret0;
 L58:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 15; }
  goto ret0;
 L74:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 19; }
  goto ret0;
 L90:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 23; }
  goto ret0;
 L131:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  if (GET_MODE (x2) == SFmode && 1)
    goto L132;
  if (GET_MODE (x2) == DFmode && 1)
    goto L147;
  goto ret0;
 L132:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; return 33; }
  goto ret0;
 L147:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; return 36; }
  goto ret0;
 L161:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  if (GET_MODE (x2) == SFmode && 1)
    goto L162;
  if (GET_MODE (x2) == DFmode && 1)
    goto L177;
  goto ret0;
 L162:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; return 39; }
  goto ret0;
 L177:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; return 42; }
  goto ret0;
 L191:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L192; }
  goto ret0;
 L192:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 45; }
  goto ret0;
 L216:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L217; }
  goto ret0;
 L217:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 50; }
  goto ret0;
 L241:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L242; }
  goto ret0;
 L242:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 55; }
  goto ret0;
 L266:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L267; }
  goto ret0;
 L267:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 60; }
  goto ret0;
 L291:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L292; }
  goto ret0;
 L292:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 65; }
  goto ret0;
 L306:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L307; }
  goto ret0;
 L307:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 68; }
  goto ret0;
 L321:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L322; }
  goto ret0;
 L322:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 71; }
  goto ret0;
 L335:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 74; }
  goto ret0;
 L347:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 77; }
  goto ret0;
 L360:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L361; }
  goto ret0;
 L361:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 80; }
  goto ret0;
 L375:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L376; }
  goto ret0;
 L376:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 83; }
  goto ret0;
 L390:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L391; }
  goto ret0;
 L391:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 86; }
  goto ret0;
 L405:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; goto L406; }
  goto ret0;
 L406:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[2] = x2; return 89; }
  goto ret0;
 ret0: return -1;
}

int
recog_4 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L48:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SImode))
    { recog_operand[1] = x1; return 12; }
 L61:
  if (GET_MODE (x1) != SImode)
    goto ret0;
 switch (GET_CODE (x1))
  {
  case SIGN_EXTEND:
  if (1)
    goto L62;
  break;
  case ZERO_EXTEND:
  if (1)
    goto L70;
  break;
  case FIX:
  if (1)
    goto L136;
  break;
  case UNSIGNED_FIX:
  if (1)
    goto L166;
  break;
  case PLUS:
  if (1)
    goto L196;
  break;
  case MULT:
  if (1)
    goto L221;
  break;
  case DIV:
  if (1)
    goto L246;
  break;
  case MINUS:
  if (1)
    goto L271;
  break;
  case AND:
  if (1)
    goto L296;
  break;
  case IOR:
  if (1)
    goto L311;
  break;
  case XOR:
  if (1)
    goto L326;
  break;
  case NEG:
  if (1)
    goto L339;
  break;
  case NOT:
  if (1)
    goto L351;
  break;
  case ASHIFT:
  if (1)
    goto L365;
  break;
  case ASHIFTRT:
  if (1)
    goto L380;
  break;
  case LSHIFT:
  if (1)
    goto L395;
  break;
  case LSHIFTRT:
  if (1)
    goto L410;
  break;
  }
  goto ret0;
 L62:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 16; }
 L66:
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 17; }
  goto ret0;
 L70:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 18; }
 L78:
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 20; }
  goto ret0;
 L136:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  if (GET_MODE (x2) == SFmode && 1)
    goto L137;
  if (GET_MODE (x2) == DFmode && 1)
    goto L152;
  goto ret0;
 L137:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; return 34; }
  goto ret0;
 L152:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; return 37; }
  goto ret0;
 L166:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) != FIX)
    goto ret0;
  if (GET_MODE (x2) == SFmode && 1)
    goto L167;
  if (GET_MODE (x2) == DFmode && 1)
    goto L182;
  goto ret0;
 L167:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, SFmode))
    { recog_operand[1] = x3; return 40; }
  goto ret0;
 L182:
  x3 = XEXP (x2, 0);
  if (general_operand (x3, DFmode))
    { recog_operand[1] = x3; return 43; }
  goto ret0;
 L196:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L197; }
  goto ret0;
 L197:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 46; }
  goto ret0;
 L221:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L222; }
  goto ret0;
 L222:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 51; }
  goto ret0;
 L246:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L247; }
  goto ret0;
 L247:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 56; }
  goto ret0;
 L271:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L272; }
  goto ret0;
 L272:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 61; }
  goto ret0;
 L296:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L297; }
  goto ret0;
 L297:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 66; }
  goto ret0;
 L311:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L312; }
  goto ret0;
 L312:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 69; }
  goto ret0;
 L326:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L327; }
  goto ret0;
 L327:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 72; }
  goto ret0;
 L339:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 75; }
  goto ret0;
 L351:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 78; }
  goto ret0;
 L365:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L366; }
  goto ret0;
 L366:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 81; }
  goto ret0;
 L380:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L381; }
  goto ret0;
 L381:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 84; }
  goto ret0;
 L395:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L396; }
  goto ret0;
 L396:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 87; }
  goto ret0;
 L410:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; goto L411; }
  goto ret0;
 L411:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[2] = x2; return 90; }
  goto ret0;
 ret0: return -1;
}

int
recog_5 (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L1:
  x1 = XEXP (x0, 0);
  if (x1 == cc0_rtx && 1)
    goto L2;
  if (x1 == pc_rtx && 1)
    goto L414;
 L607:
  if (1)
    { recog_operand[0] = x1; goto L608; }
 L41:
 switch (GET_MODE (x1))
  {
  case QImode:
  if (general_operand (x1, QImode))
    { recog_operand[0] = x1; goto L42; }
  break;
 L44:
  case HImode:
  if (general_operand (x1, HImode))
    { recog_operand[0] = x1; goto L45; }
  break;
  case SImode:
  if (general_operand (x1, SImode))
    { recog_operand[0] = x1; goto L48; }
  break;
  case SFmode:
  if (general_operand (x1, SFmode))
    { recog_operand[0] = x1; goto L51; }
  break;
  case DFmode:
  if (general_operand (x1, DFmode))
    { recog_operand[0] = x1; goto L54; }
  break;
  }
  goto ret0;
 L2:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == COMPARE && 1)
    goto L3;
 L27:
  if (general_operand (x1, QImode))
    { recog_operand[0] = x1; return 5; }
 L30:
  if (general_operand (x1, HImode))
    { recog_operand[0] = x1; return 6; }
 L33:
  if (general_operand (x1, SImode))
    { recog_operand[0] = x1; return 7; }
 L36:
  if (general_operand (x1, SFmode))
    { recog_operand[0] = x1; return 8; }
 L39:
  if (general_operand (x1, DFmode))
    { recog_operand[0] = x1; return 9; }
  x1 = XEXP (x0, 0);
  goto L607;
 L3:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[0] = x2; goto L4; }
 L8:
  if (general_operand (x2, HImode))
    { recog_operand[0] = x2; goto L9; }
 L13:
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L14; }
 L18:
  if (general_operand (x2, SFmode))
    { recog_operand[0] = x2; goto L19; }
 L23:
  if (general_operand (x2, DFmode))
    { recog_operand[0] = x2; goto L24; }
  goto L27;
 L4:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 0; }
  x2 = XEXP (x1, 0);
  goto L8;
 L9:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 1; }
  x2 = XEXP (x1, 0);
  goto L13;
 L14:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 2; }
  x2 = XEXP (x1, 0);
  goto L18;
 L19:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; return 3; }
  x2 = XEXP (x1, 0);
  goto L23;
 L24:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; return 4; }
  goto L27;
 L414:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == IF_THEN_ELSE && 1)
    goto L415;
  if (GET_CODE (x1) == LABEL_REF && 1)
    goto L595;
  x1 = XEXP (x0, 0);
  goto L607;
 L415:
  tem = recog_1 (x0, insn);
  if (tem >= 0) return tem;
  x1 = XEXP (x0, 0);
  goto L607;
 L595:
  x2 = XEXP (x1, 0);
  if (1)
    { recog_operand[0] = x2; return 111; }
  x1 = XEXP (x0, 0);
  goto L607;
 L608:
  x1 = XEXP (x0, 1);
  if (GET_CODE (x1) == CALL && 1)
    goto L609;
  x1 = XEXP (x0, 0);
  goto L41;
 L609:
  x2 = XEXP (x1, 0);
  if (ever_good (x2, SImode))
    { recog_operand[1] = x2; goto L610; }
  x1 = XEXP (x0, 0);
  goto L41;
 L610:
  x2 = XEXP (x1, 1);
  if (ever_good (x2, SImode))
    { recog_operand[2] = x2; return 114; }
  x1 = XEXP (x0, 0);
  goto L41;
 L42:
  tem = recog_2 (x0, insn);
  if (tem >= 0) return tem;
  goto L44;
 L45:
  return recog_3 (x0, insn);
 L48:
  return recog_4 (x0, insn);
 L51:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, SFmode))
    { recog_operand[1] = x1; return 13; }
 L97:
  if (GET_MODE (x1) != SFmode)
    goto ret0;
 switch (GET_CODE (x1))
  {
  case FLOAT_TRUNCATE:
  if (1)
    goto L98;
  break;
  case FLOAT:
  if (1)
    goto L102;
  break;
  case PLUS:
  if (1)
    goto L201;
  break;
  case MULT:
  if (1)
    goto L226;
  break;
  case DIV:
  if (1)
    goto L251;
  break;
  case MINUS:
  if (1)
    goto L276;
  break;
  }
  goto ret0;
 L98:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; return 25; }
  goto ret0;
 L102:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 26; }
 L106:
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 27; }
 L110:
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 28; }
  goto ret0;
 L201:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L202; }
  goto ret0;
 L202:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; return 47; }
  goto ret0;
 L226:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L227; }
  goto ret0;
 L227:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; return 52; }
  goto ret0;
 L251:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L252; }
  goto ret0;
 L252:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; return 57; }
  goto ret0;
 L276:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; goto L277; }
  goto ret0;
 L277:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SFmode))
    { recog_operand[2] = x2; return 62; }
  goto ret0;
 L54:
  x1 = XEXP (x0, 1);
  if (general_operand (x1, DFmode))
    { recog_operand[1] = x1; return 14; }
 L81:
  if (GET_MODE (x1) != DFmode)
    goto ret0;
 switch (GET_CODE (x1))
  {
  case FLOAT_EXTEND:
  if (1)
    goto L82;
  break;
  case FLOAT:
  if (1)
    goto L114;
  break;
  case PLUS:
  if (1)
    goto L206;
  break;
  case MULT:
  if (1)
    goto L231;
  break;
  case DIV:
  if (1)
    goto L256;
  break;
  case MINUS:
  if (1)
    goto L281;
  break;
  }
  goto ret0;
 L82:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, SFmode))
    { recog_operand[1] = x2; return 21; }
  goto ret0;
 L114:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, QImode))
    { recog_operand[1] = x2; return 29; }
 L118:
  if (general_operand (x2, HImode))
    { recog_operand[1] = x2; return 30; }
 L122:
  if (general_operand (x2, SImode))
    { recog_operand[1] = x2; return 31; }
  goto ret0;
 L206:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L207; }
  goto ret0;
 L207:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; return 48; }
  goto ret0;
 L231:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L232; }
  goto ret0;
 L232:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; return 53; }
  goto ret0;
 L256:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L257; }
  goto ret0;
 L257:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; return 58; }
  goto ret0;
 L281:
  x2 = XEXP (x1, 0);
  if (general_operand (x2, DFmode))
    { recog_operand[1] = x2; goto L282; }
  goto ret0;
 L282:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, DFmode))
    { recog_operand[2] = x2; return 63; }
  goto ret0;
 ret0: return -1;
}

int
recog (x0, insn)
     register rtx x0;
     rtx insn;
{
  register rtx x1, x2, x3, x4, x5;
  rtx x6, x7, x8, x9, x10, x11;
  int tem;
 L0:
 switch (GET_CODE (x0))
  {
  case SET:
  if (1)
    goto L1;
  break;
  case PARALLEL:
  if (XVECLEN (x0, 0) == 2 && 1)
    goto L597;
  break;
  case CALL:
  if (1)
    goto L604;
  break;
  case CONST_INT:
  if (x0 == const0_rtx && 1)
    return 115;
  break;
  }
  goto ret0;
 L1:
  return recog_5 (x0, insn);
 L597:
  x1 = XVECEXP (x0, 0, 0);
  if (GET_CODE (x1) == SET && 1)
    goto L598;
  goto ret0;
 L598:
  x2 = XEXP (x1, 0);
  if (x2 == pc_rtx && 1)
    goto L599;
  goto ret0;
 L599:
  x2 = XEXP (x1, 1);
  if (general_operand (x2, SImode))
    { recog_operand[0] = x2; goto L600; }
  goto ret0;
 L600:
  x1 = XVECEXP (x0, 0, 1);
  if (GET_CODE (x1) == USE && 1)
    goto L601;
  goto ret0;
 L601:
  x2 = XEXP (x1, 0);
  if (GET_CODE (x2) == LABEL_REF && 1)
    goto L602;
  goto ret0;
 L602:
  x3 = XEXP (x2, 0);
  if (1)
    { recog_operand[1] = x3; return 112; }
  goto ret0;
 L604:
  x1 = XEXP (x0, 0);
  if (ever_good (x1, SImode))
    { recog_operand[0] = x1; goto L605; }
  goto ret0;
 L605:
  x1 = XEXP (x0, 1);
  if (ever_good (x1, SImode))
    { recog_operand[1] = x1; return 113; }
  goto ret0;
 ret0: return -1;
}
