/*
 *  $Id: util_Const.c,v 1.2 2002/09/30 16:20:23 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |                      D p G e n                                 |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Module  :         "./util_Const.c"                          |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# include  "util_Defs.h"
# include  "debugoff.h"


/*  ------------------------------------------------------------------
 *  Local variables.
 */

  static char *LV_const;


/*  ------------------------------------------------------------------
 *  Internal functions declarations.
 */

  static long  btol  __FP((char aDigit));
  static long  otol  __FP((char aDigit));
  static long  xtol  __FP((char aDigit));
  static char  ltox  __FP((long aDigit));


/*
 *  /----------------------------------------------------------------\
 *  |                   Functions Definitions                        |
 *  \----------------------------------------------------------------/
 */

/*  ------------------------------------------------------------------
 *  Function  :  "btol ()".
 */

static long  btol(aDigit)
  char  aDigit;
{
  long  digit;

  digit = 0;

  switch (aDigit) {
    case '0': digit = 0; break;
    case '1': digit = 1; break;
    default:
      eprinth (NULL);
      eprintf ("\n  Invalid digit \'%c\' in boolean string \"%s\".\n",
               aDigit, LV_const);
      EXIT (1);
  }

  return (digit);
}


/*  ------------------------------------------------------------------
 *  Function  :  "otol ()".
 */

static long  otol(aDigit)
  char  aDigit;
{
  long  digit;


  digit = -1;


  if ((aDigit >= '0') && (aDigit <= '7')) digit = (long)(aDigit - '0');

  if (digit < 0) {
    eprinth (NULL);
    eprintf ("\n  Invalid digit \'%c\' in octal string \"%s\".\n",
             aDigit, LV_const);
    EXIT (1);
  }

  return (digit);
}


/*  ------------------------------------------------------------------
 *  Function  :  "xtol ()".
 */

static long  xtol(aDigit)
  char  aDigit;
{
  long  digit;


  digit = -1;


  if ((aDigit >= '0') && (aDigit <= '9')) digit = (long)(aDigit - '0');
  if ((aDigit >= 'a') && (aDigit <= 'z')) digit = (long)(aDigit - 'a') + 10;
  if ((aDigit >= 'A') && (aDigit <= 'Z')) digit = (long)(aDigit - 'A') + 10;

  if (digit < 0) {
    eprinth (NULL);
    eprintf ("\n  Invalid digit \'%c\' in hexadecimal string \"%s\".\n",
             aDigit, LV_const);
    EXIT (1);
  }

  return (digit);
}


/*  ------------------------------------------------------------------
 *  Function  :  "ltox ()".
 */

static char  ltox(aDigit)
  long  aDigit;
{
  char  digit;


  digit = '\0';


  if ((aDigit >=  0) && (aDigit <=  9)) digit = (char)(aDigit      + '0');
  if ((aDigit >= 10) && (aDigit <= 16)) digit = (char)(aDigit - 10 + 'a');

  if (digit == '\0') {
    eprinth (NULL);
    eprintf ("\n  Invalid number %d in hexadecimal conversion.\n",
             aDigit);
    EXIT (1);
  }

  return (digit);
}


/*  ------------------------------------------------------------------
 *  Function  :  "newxl ()".
 */

extern struct xlong_s *newxl()
{
  struct xlong_s *pXl;
            long  i;


  pXl = (struct xlong_s*)mbkalloc (sizeof (struct xlong_s));
  pXl->size = 0L;

  for (i = 0; i < XLONG_SIZE; i++) (pXl->l)[i] = 0L;

  return (pXl);
}


/*  ------------------------------------------------------------------
 *  Function  :  "freexl ()".
 */

extern void  freexl(apXl)
  struct xlong_s *apXl;
{
  mbkfree (apXl);
}


/*  ------------------------------------------------------------------
 *  Function  :  "atoxl ()".
 */

extern void  atoxl(apXl, asConst)
  struct xlong_s *apXl;
            char *asConst;
{
  long  base, length, offset, i, xl_size, iRight, iLeft, power;


  LV_const = asConst;

  base   = 1;
  offset = 0;

  if (asConst[0] == '0') {
    switch (asConst[1]) {
      case 'b':
      case 'B':
        base = 1; offset = 2; break;
      case 'x':
      case 'X':
        base = 4; offset = 2; break;
      default:
        base = 3; offset = 1; break;
    }
  }


  __DBG (fprintf (stderr, "base := %ld\n", base);)


  length = strlen (asConst);

  apXl->size = (length - offset) * base;

  xl_size = apXl->size / 32 + ((apXl->size % 32) ? 1 : 0);

  __DBG (fprintf (stderr, "xl_size := %ld\n", xl_size);)

  for (i = 0; i < xl_size ; i++) {
    iRight = length - 1 - (32 / base) *  i;
    iLeft  = length     - (32 / base) * (i + 1);

    if (iLeft < offset) iLeft = offset;

    __DBG (fprintf (stderr, "iRight := %ld\n", iRight);)
    __DBG (fprintf (stderr, "iLeft  := %ld\n", iLeft);)

    power = 1;

    for (; iRight >= iLeft; iRight--) {
      __DBG (fprintf (stderr,
                      "asConst[%ld]  := %c\n",
                      iRight,
                      asConst[iRight]);)
      switch (base) {
        case 1: (apXl->l)[i] += btol(asConst[iRight]) << (power - 1); break;
        case 3: (apXl->l)[i] += otol(asConst[iRight]) << (power - 1); break;
        case 4: (apXl->l)[i] += xtol(asConst[iRight]) << (power - 1); break;
      }

      power += base;
    }
  }


  __DBG(
    fprintf (stderr, "size := %ld\n", apXl->size);
    for (i = 0; i < XLONG_SIZE; i++)
      fprintf (stderr, "l[%ld] := %ld\n", i, apXl->l[i]);
    fflush (stderr);
  )
}


/*  ------------------------------------------------------------------
 *  Function  :  "getxlbit ()".
 */

extern long  getxlbit(apXl, aBit)
  struct xlong_s *apXl;
            long  aBit;
{
  long  i, mask;


  i    =              aBit / 32;
  mask = (long)(1 << (aBit % 32));


  if (i >= XLONG_SIZE) {
    eprinth ("getxlbit:");
    eprintf ("\n  Requested bit %ld is out of range.\n", aBit);
    EXIT (1);
  }


  return (((apXl->l[i]) & mask) ? 1L : 0L);
}


/*  ------------------------------------------------------------------
 *  Function  :  "getxlhexa ()".
 */

extern long  getxlhexa(apXl, aBit)
  struct xlong_s *apXl;
            long  aBit;
{
  long  i, shift;


  i     = aBit / 32;
  shift = aBit % 32;


  if (i >= XLONG_SIZE) {
    eprinth ("getxlhexa:");
    eprintf ("\n  Requested hexa %ld is out of range.\n", aBit);
    EXIT (1);
  }


  return ((((apXl->l[i]) >> shift) & 15));
}


/*  ------------------------------------------------------------------
 *  Function  :  "xltovhdl ()".
 */

extern char *xltovhdl(apXl, aS)
  struct xlong_s *apXl;
            char *aS;
{
  long  i;
  char *pS;


  pS = aS;
  i  = apXl->size;

  __DBG (fprintf (stderr, "size := %ld\n", i);)

  if (i % 4) {
    strcpy (pS, "B\""); pS += 2;

    for (; i % 4; i--) {
      if (getxlbit (apXl, i - 1))
        *pS = '1';
      else
        *pS = '0';

      __DBG(
        fprintf (stderr, "iB := %ld, (%c)\n", i, *pS);
        fflush (stderr);
      )

      pS++;
    }
  }


  if (i >= 4) {
    if (pS != aS) { strcpy (pS, "\" & "); pS += 4; }

    strcpy (pS, "X\""); pS += 2;

    for (i -= 4; i >= 0; i -= 4) {
      *pS = ltox (getxlhexa (apXl, i));

      __DBG(
      fprintf (stderr, "iX := %ld, (%c)\n", i, *pS);
      fflush (stderr);
      )

        pS++;
    }
  }

  strcpy (pS, "\"");


  return (aS);
}
