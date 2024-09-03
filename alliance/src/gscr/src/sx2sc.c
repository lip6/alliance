#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <mph.h>
#include <mut.h>
#include <mpu.h>
#include "apr.h"

extern int SXMODE;

void sx2sc (phfig_list * phfig)
{
  phref_list *phref;
  int used[100];
  int t;

  if (SXMODE == 0) return;
  
  if (phfig->USER) return; /* deja traite */
  
  phfig->USER = (void*)1;
  /*
   * initialisation du tableau des transparences
   */
  for (t=0; t <= (phfig->XAB2)/PITCH_X; t++)
  {
    used[t] = 0;
  }
  used[t] = -1;

  for (phref = phfig->PHREF; phref; phref = phref->NEXT)
  {
    if (phref->NAME && strstr (phref->NAME, "_25"))
    {
      char ConnName[32];
      strcpy (ConnName, phref->NAME);
      *(strchr (ConnName, '_')) = '\0';
      addphcon (phfig, NORTH,  ConnName,
                phref->XREF, phref->YREF, ALU2, 2*SCALE_X);
      addphcon (phfig, SOUTH,  ConnName,
                phref->XREF, phref->YREF, ALU2, 2*SCALE_X);
    }
    used[(phref->XREF)/PITCH_X] = 1;
  }

  for (t=1; used[t] != -1; t++)
  {
    if (!used[t])
    {
      char transname[32];
      sprintf (transname, "T%d", t);
      addphseg (phfig, TALU2, 2*SCALE_X, 
                t*PITCH_X, phfig->YAB1, t*PITCH_X, phfig->YAB2,
                transname);
    }
  } 
}
