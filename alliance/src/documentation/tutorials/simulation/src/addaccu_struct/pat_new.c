#include <genpat.h>
#include <stdio.h>

char *inttostr(entier)
     int entier;
{
  char *str;
  str = (char *) mbkalloc (32 * sizeof (char));
  sprintf (str, "%d",entier);
  return(str);
}

/*------------------------------*/
/* end of the description       */
/*------------------------------*/

main ()
{
  int i;
  int j;
  int k;
  int cur_vect;
  
  DEF_GENPAT("pat_new");
  
  /* interface */
  DECLAR ("a", ":2", "X", IN, "3  downto 0", "");
  DECLAR ("b", ":2", "X", IN, "3  downto 0", "");
  DECLAR ("sel", ":2", "B", IN, "", "");
  DECLAR ("ck", ":2", "B", IN, "", "");
  DECLAR ("s", ":2", "X", OUT, "3 downto 0", "");
  DECLAR ("vdd", ":2", "B", IN, "", "");
  DECLAR ("vss", ":2", "B", IN, "", "");
  

  LABEL ("additionneur");
  AFFECT ("0", "ck"  , "0b0");
  AFFECT ("0",  "a"  , "0x0");
  AFFECT ("0",  "b"  , "0x0");
  AFFECT ("0", "sel"  , "0b0");
  AFFECT ("0", "vdd"  , "0b1");
  AFFECT ("0", "vss"  , "0b0");
  cur_vect = 0;
  
  for (k=0; k<2; k++)
    {
      if (k==1)
	     AFFECT ( inttostr(cur_vect),"sel","0b1" );
      for (i=0; i<16; i++)
	  {
	    for (j=0; j<16; j++)
	      {
	        AFFECT ( inttostr(cur_vect),"ck","0b0" );
	        AFFECT ( inttostr(cur_vect),"a", inttostr(i));
	        AFFECT ( inttostr(cur_vect),"b", inttostr(j));
	        cur_vect += 50000;
	        AFFECT ( inttostr(cur_vect),"ck","0b1" );
	        cur_vect += 50000;
	      }
	  }
    }
  AFFECT ( inttostr(cur_vect), "a", "0b0");
  AFFECT ( inttostr(cur_vect), "b", "0b0");
  AFFECT ( inttostr(cur_vect), "sel", "0b0");
  AFFECT ( inttostr(cur_vect), "ck", "0b0");
  
  SAV_GENPAT ();
  exit(0);	
}


