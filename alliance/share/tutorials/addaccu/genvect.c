#include <genpat.h>
#include <stdio.h>
/*#include <mut315.h> */

char *
inttostr (entier)
     int entier;
{
  char *str;
  str = (char *) mbkalloc (32 * sizeof (char));
  sprintf (str, "%d", entier);
  return (str);
}
/*------------------------------*/
	/* end of the description       */
/*------------------------------*/
main ()
{
  int i;
  int j;
  int numvect;
  int CKcour;
  int test;
  numvect = 0;
  test = 0;
  CKcour = 0;


  DEF_GENPAT ("genvect");


/* Declaration de l'interface */

  DECLAR ("A", ":2", "X", IN, "3 downto 0");
  DECLAR ("B", ":2", "X", IN, "3 downto 0");
  DECLAR ("SEL", ":2", "B", IN, "");
  DECLAR ("s", ":2", "X", OUT, "3 downto 0");
  DECLAR ("CK", ":2", "B", IN, "");
  DECLAR ("vdd", ":2", "B", IN, "");
  DECLAR ("vss", ":2", "B", IN, "");
  LABEL ("additionneur");
  AFFECT ("0", "SEL", "0b0");
  AFFECT ("0", "A", "0X0");
  AFFECT ("0", "vdd", "0b1");
  AFFECT ("0", "vss", "0b0");
  AFFECT ("0", "B", "0X0");
/* Boucle d'affectation des vecteurs */

  while (numvect < 768)
    {
      if (test == 1)
	{
	  AFFECT (inttostr (numvect), "A", inttostr (i));
	  AFFECT (inttostr (numvect), "B", inttostr (j));
	  AFFECT (inttostr (numvect), "CK", "0b1");
	  CKcour = 1;
	  numvect++;
	  if (j == 16)
	    i++;
	  if (j == 16)
	    j = 0;
	  else
	    j++;
	  test = 0;
	}
      else
	{
	  if (CKcour == 0 && test == 0)
	    test = 1;
	  AFFECT (inttostr (numvect), "CK", "0b0");
	  CKcour = 0;
	  numvect++;
	}
    }
  i = 0;
  j = 0;
  AFFECT (inttostr (numvect), "SEL", "0b1");

/* Boucle d'affectation des vecteurs */


  while (numvect < 1536)
    {
      if (test == 1)
	{
	  AFFECT (inttostr (numvect), "A", inttostr (i));
	  AFFECT (inttostr (numvect), "B", inttostr (j));
	  AFFECT (inttostr (numvect), "CK", "0b1");
	  CKcour = 1;
	  numvect++;
	  if (j == 16)
	    i++;
	  if (j == 16)
	    j = 0;
	  else
	    j++;
	  test = 0;
	}
      else
	{
	  if (CKcour == 0 && test == 0)
	    test = 1;
	  AFFECT (inttostr (numvect), "CK", "0b0");
	  numvect++;
	  CKcour = 0;
	}
    }

  SAV_GENPAT ();
}

