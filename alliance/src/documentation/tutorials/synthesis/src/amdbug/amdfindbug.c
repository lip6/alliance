#include <stdio.h>
#include "genpat.h"

char *inttostr(entier)
     int entier;
{
  char *str;
  str = (char *) mbkalloc (32 * sizeof (char));
  sprintf (str, "%d",entier);
  return(str);
}

main ()
{
  int i;
  int j;
  int k;
  int h, select;
  int vect_date; /* this date is an absolute date, in ps */
  
  DEF_GENPAT("amdfindbug");
  
  /* interface */
  DECLAR ("ck", ":2", "B", IN, "", "");
  DECLAR ("fonc", ":2", "B", IN, "", "");
  DECLAR ("test", ":2", "B", IN, "", "");
  DECLAR ("scin", ":2", "B", IN, "", "");
  DECLAR ("scout", ":2", "B", OUT, "", "");
  DECLAR ("i", ":2", "O", IN, "8 downto 0", "");
  DECLAR ("a", ":2", "X", IN, "3 downto 0", "");
  DECLAR ("b", ":2", "X", IN, "3 downto 0", "");
  DECLAR ("d", ":2", "X", IN, "3 downto 0", "");
  DECLAR ("noe", ":2", "B", IN, "", "");
  DECLAR ("r0", ":2", "B", INOUT, "", "");
  DECLAR ("r3", ":2", "B", INOUT, "", "");
  DECLAR ("q0", ":2", "B", INOUT, "", "");
  DECLAR ("q3", ":2", "B", INOUT, "", "");
  DECLAR ("ovr", ":2", "B", OUT, "", "");
  DECLAR ("zero", ":2", "B", OUT, "", "");
  DECLAR ("signe", ":2", "B", OUT, "", "");
  DECLAR ("np", ":2", "B", OUT, "", "");
  DECLAR ("ng", ":2", "B", OUT, "", "");
  DECLAR ("cin", ":2", "B", IN, "", "");
  DECLAR ("cout", ":2", "B", OUT, "", "");
  DECLAR ("y", ":2", "X", OUT, "3 downto 0", "");
  DECLAR ("vdd", ":2", "B", IN, "", "");
  DECLAR ("vss", ":2", "B", IN, "", "");
  DECLAR ("vddp", ":2", "B", IN, "", "");
  DECLAR ("vssp", ":2", "B", IN, "", "");
  
  LABEL ("v");
  
  /* Initialisation de toutes les entree */
  AFFECT("0", "ck", "0b0");
  AFFECT("0", "fonc", "0b0");
  AFFECT("0", "test", "0b0");
  AFFECT("0", "scin", "0b0");

  AFFECT("0", "i", "0o0");
  AFFECT("0", "a", "0x0");
  AFFECT("0", "b", "0x0");
  AFFECT("0", "d", "0x0");
  AFFECT("0", "noe", "0b0");
  AFFECT("0", "r0", "0b0");
  AFFECT("0", "r3", "0b0");
  AFFECT("0", "q0", "0b0");
  AFFECT("0", "q3", "0b0");
  
  AFFECT("0", "cin", "0b0");
  AFFECT("0", "vdd", "0b0");
  AFFECT("0", "vss", "0b0");
  AFFECT("0", "vddp", "0b0");
  AFFECT("0", "vssp", "0b0");
  vect_date = 0;


  /* Remplissage de l'accu */
/*  AFFECT(inttostr(vect_date), "i", "0o007");
  AFFECT(inttostr(vect_date), "d", "0x3");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  */
  /* Remplissage de la ram et visualisation du registre rempli precedement 
     sur la sortie */

  /* On n'est pas en mode test  */
  AFFECT(inttostr(vect_date), "fonc", "0b1");
  

  //vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o207");
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "b", "0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
 
  for (i = 1; i < 16; i++) {
    vect_date += 50000;
    AFFECT(inttostr(vect_date), "ck", "0");
    AFFECT(inttostr(vect_date), "b", inttostr(i));
    AFFECT(inttostr(vect_date), "d", inttostr(i));
    AFFECT(inttostr(vect_date), "a", inttostr(i-1));
    AFFECT(inttostr(vect_date), "y", inttostr(i-1));
    
    vect_date += 50000;
    AFFECT(inttostr(vect_date), "ck", "1");
  }

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "a", inttostr(15));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(15));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "a", inttostr(15));
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(15));

  /* Test du decaleur de la ram */
  /* decal droite */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o407");
  AFFECT(inttostr(vect_date), "r3", "0b0");
  AFFECT(inttostr(vect_date), "r0", "?0b0");
  AFFECT(inttostr(vect_date), "q3", "0b0");
  AFFECT(inttostr(vect_date), "q0", "?0b*");
  AFFECT(inttostr(vect_date), "d", inttostr(10));
  AFFECT(inttostr(vect_date), "b", inttostr(12));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
    
  /* decal droite */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o507");
  AFFECT(inttostr(vect_date), "r3", "0b0");
  AFFECT(inttostr(vect_date), "r0", "?0b0");
  AFFECT(inttostr(vect_date), "d", inttostr(10));
  AFFECT(inttostr(vect_date), "b", inttostr(13));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "y", inttostr(10));
  AFFECT(inttostr(vect_date), "ck", "1");
  
  /* decal gauche */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o607");
  AFFECT(inttostr(vect_date), "r0", "0b0");
  AFFECT(inttostr(vect_date), "r3", "?0b1");
  AFFECT(inttostr(vect_date), "q0", "0b0");
  AFFECT(inttostr(vect_date), "q3", "?0b*");
  AFFECT(inttostr(vect_date), "d", inttostr(10));
  AFFECT(inttostr(vect_date), "b", inttostr(14));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
    
  /* decal gauche */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o707");
  AFFECT(inttostr(vect_date), "r0", "0b0");
  AFFECT(inttostr(vect_date), "r3", "?0b*");
  AFFECT(inttostr(vect_date), "d", inttostr(10));
  AFFECT(inttostr(vect_date), "b", inttostr(15));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(10));
  
  /* Lecture des valeurs decalee de la ram */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o207");
  AFFECT(inttostr(vect_date), "a", inttostr(12));
  AFFECT(inttostr(vect_date), "b", inttostr(1));
  AFFECT(inttostr(vect_date), "d", inttostr(1));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(5));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(5));

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o207");
  AFFECT(inttostr(vect_date), "a", inttostr(13));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(5));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(5));

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o207");
  AFFECT(inttostr(vect_date), "a", inttostr(14));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(4));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "y", inttostr(4));
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "i", "0o207");
  AFFECT(inttostr(vect_date), "a", inttostr(15));
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "y", inttostr(4));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(4));


  /* Remplissage de l'accu */
  /* Ecriture de la valeur de d decale a droite */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "i", "0o007");
  AFFECT(inttostr(vect_date), "d", inttostr(7));
  AFFECT(inttostr(vect_date), "q3", "0b0");
  AFFECT(inttostr(vect_date), "q0", "?0b*");
  AFFECT(inttostr(vect_date), "r0", "?0b*");
  AFFECT(inttostr(vect_date), "y", inttostr(7));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(7));
  
  /* Ecriture dans l'accu de sa valeur + 0 decale a droite */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "i", "0o402");
  AFFECT(inttostr(vect_date), "y", inttostr(7));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(3));
 
  /* Ecriture dans l'accu de sa valeur + 0 decale a gauche */
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "q0", "0b0");
  AFFECT(inttostr(vect_date), "q3", "?0b0");
  AFFECT(inttostr(vect_date), "r3", "?0b*");
  AFFECT(inttostr(vect_date), "i", "0o602");
  AFFECT(inttostr(vect_date), "y", inttostr(3));
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");
  AFFECT(inttostr(vect_date), "y", inttostr(6));
 
  /* Test des operateurs arithmetiques et logiques */
  for(i=0;i<8;i++)
    {
     vect_date += 50000;
     AFFECT(inttostr(vect_date), "ck", "0");
     AFFECT(inttostr(vect_date),"q3","0b0");
     AFFECT(inttostr(vect_date),"i",inttostr(i*8+5));
     AFFECT(inttostr(vect_date),"d",inttostr(3));
     AFFECT(inttostr(vect_date),"a",inttostr(13));
     AFFECT(inttostr(vect_date),"b",inttostr(0));
     AFFECT(inttostr(vect_date),"y","?0x*");
     // pour les soustractions S-R et R-S on ne met pas cin a 1
     // mais on en tient compte pour le resultat.
     LABEL("test alu");
     vect_date += 50000;
     AFFECT(inttostr(vect_date), "ck", "1");
     LABEL("test alu");
    }  

  /* Test exhaustif du reste des fonctionnalitees */

  for(i=0;i<8;i++) {
    for (j = 0; j < 8; j++) {
      for (k = 0; k < 4; k++) {
 
	vect_date += 50000;
	AFFECT(inttostr(vect_date), "ck", "0");
 	AFFECT(inttostr(vect_date),"q3","0b0");
	AFFECT(inttostr(vect_date),"i",inttostr(k*16+i*8+j));
	AFFECT(inttostr(vect_date),"d",inttostr(3));
	AFFECT(inttostr(vect_date),"a",inttostr(13));
	AFFECT(inttostr(vect_date),"b",inttostr(0));
	AFFECT(inttostr(vect_date),"y","?0x*");
	// pour les soustractions S-R et R-S on ne met pas cin a 1
	// mais on en tient compte pour le resultat.
	LABEL("test alu");
	vect_date += 50000;
	AFFECT(inttostr(vect_date), "ck", "1");
	LABEL("test alu");
      }
    }
  }


  /* Test du chemin de test */

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "fonc", "0");
  AFFECT(inttostr(vect_date), "test", "1");
  AFFECT(inttostr(vect_date), "scin", "0b1");
  AFFECT(inttostr(vect_date), "scout", "0b*");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b1");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "0");
  AFFECT(inttostr(vect_date), "scin", "0b0");
  vect_date += 50000;
  AFFECT(inttostr(vect_date), "ck", "1");

  


  SAV_GENPAT ();
  exit(0);
}
