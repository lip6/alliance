/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

# include <stdio.h>
# include <math.h>
# include MUT_H
# include MPH_H
# include RDS_H
# include RWI_H
# include RUT_H
# include RPR_H
# include RFM_H
# include "G_global.h"

# include "d_asm.h"
# include "d_codes.h"
# include "d_fonc.h"

#define READ_CHAR(X)     *((unsigned char*)(X))

int I_pc;                /* compteur ordinal */
int I_cc0;               /* code condition  */
extern int I_Debut_Data; /* adresse absolue de debut data  */
extern int I_Fin_Data;   /* adresse absolue de fin data   */
extern int I_Debut_Code; /* adresse absolue de debut code  */
extern int I_Fin_Code;   /* adresse absolue de fin code   */
char *I_sp;              /* stack    */
int I_reg[32];           /* registers   */

BOOL D_ModeTrace;        /* vrai si STEP faux si RUN */
BOOL D_FicCharge = FALSE;/* vrai si un fichier a ete charge */
BOOL D_FirstExec;        /* vrai si l'on a pas encore execute */
HBP PtBp = NULL;         /* racine de la liste des BPs */
HBP RacBp = NULL;        /* pointeur courant de la liste des BPs */

int LigneCourante = -1;  /* derniere ligne executee */

#define  DEST 0

/* address validity check :
   this would work if I'd keep track of the mallocated chuncks of memory
   needed for the NAME function, but it's too complicated and better have
   the core dumping signals trapped.
void
  VerifAdr(type_adr, adr)
char type_adr ;
int adr;
{
  if (adr <= I_Fin_Data && adr >= I_Debut_Data)
    return;

  if (adr <= I_Fin_Code && adr >= I_Debut_Code)
    if (type_adr == DEST) {
      Erreur("Stuff written in the code");
      EXIT(-6);
    } else
      return;
  Erreur("Illegal memory address");
  EXIT(-7);
}
*/

/* heart of the interpretor :
   this function is the micro-program interpretor, as it makes the
   calls to the instructions or interruptions, and increases the pc.
   It's kind of a sequencer. */
int
  interp(depart)
int depart;
{
extern int (*fonc[256])();
extern BOOL Interrupt;
extern int LigneCourante;
int dep;

  if (depart != -1)
    I_pc = depart;

  while (1) {
    dep = (*fonc[READ_CHAR(I_pc)])();
    if (dep >= D_EXIT) {
      I_pc += dep - D_EXIT;
      return D_EXIT;
    }
    if (Interrupt) {
      Interrupt = FALSE;
      return D_EXIT;
    }
    I_pc += dep;
  }
}

/* Break points :
   these functions allocate, add, delete, break points for programm
   trace. They are self explanatory. */
HBP
  NewBp()
{
HBP NewBp;

  NewBp = (HBP)mbkalloc(sizeof(BP));
  NewBp->NumLig = 0;
  NewBp->Prec = NULL;
  NewBp->Suiv = NULL;

  return NewBp;
}

void
  DelBp(NumLig)
int NumLig;
{
HBP ExBp;
HBP PtBp = RacBp;
HBP ParcoursBp = RacBp;

  if (!PtBp)
    return;

  while (ParcoursBp)
    ParcoursBp = ParcoursBp->Suiv;

  ExBp = NULL;
  while (PtBp->NumLig != NumLig) {
    ExBp = PtBp;
    PtBp = PtBp->Suiv;
  }

  if (ExBp)
    ExBp->Suiv = PtBp->Suiv;

  if (PtBp->Suiv)
    PtBp->Suiv->Prec = PtBp->Prec;

  if (RacBp == PtBp)
    RacBp = PtBp->Suiv;

  mbkfree(PtBp);
  PtBp = RacBp;
}

void
  AddBp(NumLig)
unsigned int NumLig;
{
HBP ExBp;
HBP PtBp = RacBp;

  if (!RacBp) {
    PtBp = NewBp();
    PtBp->NumLig = NumLig;
    RacBp = PtBp;
    return;
  }

  if (PtBp->NumLig < NumLig) {
    while (PtBp && PtBp->NumLig < NumLig) {
      ExBp = PtBp;
      PtBp = PtBp->Suiv;
    }
    ExBp->Suiv = NewBp();
    ExBp->Suiv->Prec = ExBp;
    ExBp->Suiv->Suiv = PtBp;
    ExBp->Suiv->NumLig = NumLig;
    PtBp = ExBp;
  } else {
    while (PtBp && PtBp->NumLig > NumLig) {
      ExBp = PtBp;
      PtBp = PtBp->Prec;
    }
    ExBp->Prec = NewBp();
    ExBp->Prec->Prec = PtBp;
    ExBp->Prec = ExBp->Prec;
    ExBp->Prec->Suiv = ExBp;
    ExBp->Prec->NumLig = NumLig;
    if (!PtBp) {
      PtBp = NULL;
      RacBp = NULL;
    }
  }
}

int
  VerifBp(NumLig)
{
HBP PtBp = RacBp;

  if (!RacBp)
    return 0;

  while (PtBp && PtBp->NumLig < NumLig)
    PtBp = PtBp->Suiv;

  if (PtBp && PtBp->NumLig == NumLig)
    return 1; /* bp exists */

  return 0; /* bp doesn't exist */
}

void
  DelAllBp()
{
HBP ParcoursBp = RacBp;
HBP ExBp;

  while (ParcoursBp) {
      ExBp = ParcoursBp;
      ParcoursBp = ParcoursBp->Suiv;
      mbkfree(ExBp);
  }

  RacBp = NULL;
  PtBp = NULL;
}
