/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/****************************************************************************/
#include <math.h>
#include "global.h"



/****************************************************************************/
/* Coherence entre longueurs des instructions et longueur du RI */  
/****************************************************************************/
static int LgI_NbBit(Infos *inf)
{
Instructions *PInst;
for(PInst = inf->INST; PInst; PInst = PInst ->NEXT)
{
       if(strlen(PInst->VALUE) > inf->INST_SIZE) {fprintf(stderr,"[BS ERROR] : the instruction length is taller than the size of instruction register!\n");return(0);}
       if(strlen(PInst->VALUE) < inf->INST_SIZE) {fprintf(stderr,"[BS ERROR] : the size of instruction register is bigger than the instruction length!\n");return(0);}
  }
return(1);
}


/****************************************************************************/
/* Coherence entre le nombre d'instructions et la longueur du registre 
   instruction */
/****************************************************************************/
static int LgI_NbI(Infos *inf) 
{
  Instructions *PInst;
  int i = 0, NbInst = 0;

  for(PInst = inf->INST; PInst; PInst = PInst ->NEXT)
      i++;
  NbInst = pow(2, inf->INST_SIZE);
  if(NbInst < i) {
    fprintf(stderr,"[BS ERROR] : there are more instruction codes than authorized by instruction register!\n");
    return(0);
  }
  else {
    return(1);
  }
}


/****************************************************************************/
/* Verification de la presence de l'instruction inst dans la structure 
   de donnees informations 
norme speciale pour le bypass="1...1" et le extest="0...0"*/
/****************************************************************************/
static int PresenceI(Infos *inf, char *inst) 
{
  Instructions *pinst;

  for(pinst=inf->INST; pinst; pinst=pinst->NEXT)
  {
    if (inst==pinst->NAME) return 1;
  }
  
  fprintf(stderr,"[BS ERROR] %s : instruction is unknown!\n", (char*)inst);
  return(0);
}


/****************************************************************************/
/* Verification si des instructions ont ete dupliquees au niveau des codes */
/****************************************************************************/
static int EgaliteI(Infos *inf) 
{
  Instructions *P1Inst, *P2Inst;

for(P1Inst = inf->INST; P1Inst; P1Inst = P1Inst ->NEXT) 
    for(P2Inst = P1Inst->NEXT; P2Inst; P2Inst = P2Inst ->NEXT) 
	if(P1Inst->VALUE==P2Inst->VALUE) 
   {
      fprintf(stderr,"[BS ERROR] \"%s\" : duplicated code!\n", P1Inst->VALUE);
      return(0);
   }
return(1);
}


/****************************************************************************/
/* Coherence entre la longueur du registre Boundary Scan et le nombre de
   cellules enumerees */
/****************************************************************************/
static int LgBS_NbCel(Infos *inf) 
{
  Cellules *PCel;
  int i = 0;

  for(PCel = inf->CELL; PCel; PCel = PCel ->NEXT)
    i++;
  if(inf->BS_SIZE > i) {
    fprintf(stderr,"[BS ERROR] : tere are more BS cells than authorized!\n");
    return(0);
  }
  else if(inf->BS_SIZE < i) {
    fprintf(stderr,"[BS ERROR] : BS register is taller than the number of cells!\n");
    return(0);
  }
  else {
    return(1);
  }
}


/****************************************************************************/
/* Verification si des cellules ont ete dupliquees au niveau du nom et du
   mode */
/****************************************************************************/
static int EgaliteNomCel(Infos *inf) 
{
  Cellules *P1Cel, *P2Cel;
  
  for(P1Cel = inf->CELL; P1Cel; P1Cel = P1Cel ->NEXT) 
  {
    if(!P1Cel->NAME) continue;
    
	 for(P2Cel = P1Cel->NEXT; P2Cel; P2Cel = P2Cel ->NEXT)
    {
      if(!P2Cel->NAME) continue;
	   if(P2Cel->NAME == P1Cel->NAME)
      {
	      fprintf(stderr,"[BS ERROR] %s : duplicate cell!\n", P1Cel ->NAME);
	      return(0);
	   }
    }  
    
  }
  return(1);
}


/****************************************************************************/
/* Les cellules de mode control commandent-elles bien les bonnes cellules ? */
/****************************************************************************/
static int CelControl(Infos *inf) 
{
  Cellules *P1Cel, *P2Cel;

  for(P1Cel = inf->CELL; P1Cel; P1Cel = P1Cel ->NEXT)
  {
    if(P1Cel -> MODE == MODE_TRISTATE )
    {
	   P2Cel = P1Cel->COMMAND_CELL;
      if (!P2Cel)
      {
	     fprintf(stderr,"[BS ERROR] : the cell named %s isn't controled by a cell!\n", P1Cel ->NAME);
	     return(0);
      }
      if(P2Cel -> MODE != MODE_COMMAND) 
      {
	     fprintf(stderr,
          "[BS ERROR] : the cell named %s is controled by a cell %s wich isn't CONTROL!\n", 
          P1Cel ->NAME, P2Cel ->NAME);
	     return(0);
	   }
    }
  }

  return(1);
}

/****************************************************************************/
/* Presence du nom et mode des entrees/sorties de la structure des vecteurs
   paralleles dans la structure informations */
/****************************************************************************/
static int NamePaiol(struct paseq *pat) 
{
  struct paiol *ppaiol;
  Cellules *pvbe;
  Infos *inf;
  int mode;

/* utilisons une propriete de namealloc() qui rend toujours le meme vecteur*/
char *tck=namealloc(TCK_NAME);
char *trst=namealloc(TRST_NAME);
char *tms=namealloc(TMS_NAME);
char *tdi=namealloc(TDI_NAME);
char *tdo=namealloc(TDO_NAME);

/*pour toutes les entrees sorties sauf l'alim. qui ne passe par la chaine BS*/
  for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) 
  {
    /*fait pas partie du boundary-scan*/
    if (ppaiol->NAME==tck || ppaiol->NAME==trst || ppaiol->NAME==tms 
	  || ppaiol->NAME==tdi || ppaiol->NAME==tdo  
	  || isvdd(ppaiol -> NAME) || isvss(ppaiol -> NAME))  continue;
     
    /*signal interne*/ 
	 if(ppaiol->MODE != 'I' && ppaiol->MODE != 'O' && ppaiol->MODE != 'T') continue;
    switch (ppaiol->MODE)
    {
      case 'I': mode = MODE_IN; break;
      case 'O': mode = MODE_OUT; break;
      case 'T': mode = MODE_INOUT; break;
    }

    /*recherche du connecteur dans la chaine boundary scan*/
    for(inf=InfosVbe; inf; inf=inf->NEXT) 
    {
	   for(pvbe = inf ->CELL; pvbe; pvbe = pvbe ->NEXT)
      {
		   if ( pvbe->NAME==ppaiol->NAME ) break;
      }    
      if (pvbe) break;
    }

    

    if(pvbe == NULL) 
    {
	     fprintf(stderr,
	       "[PAT ERROR] : connector named '%s' in PAT isn't declared as a BS cell!\n",
	       ppaiol -> NAME);
	     return(0);
    }

    if ( pvbe->MODE != mode)
    {
	     fprintf(stderr,
	       "[PAT ERROR] : not the same mode for connector '%s' between PAT and BS description!\n",
	       ppaiol -> NAME);
	     return(0);
    }
  }

return(1);
}


/****************************************************************************/
/* Coherences dans la structure informations */
/****************************************************************************/
extern void VerificationsInfos()
{
Infos *inf;

/*pour tous les ensembles boundary scan chaines*/
for (inf=InfosVbe;inf;inf=inf->NEXT) {
/* Coherence entre le nombre d'instructions et la longueur du registre 
   instruction */
  if(!LgI_NbI(inf))  exit(1);
/* Coherence entre longueurs des instructions et longueur du RI */  
  if(!LgI_NbBit(inf))  exit(1);
/*dans tout circuit boundary scan il existe intest="?...?" */
  if(!PresenceI(inf,namealloc(INTEST_NAME)))  exit(1);
/*dans tout circuit boundary scan il existe bypass="1...1" */
  if(!PresenceI(inf,namealloc(BYPASS_NAME)))  exit(1);
/* Verification si des instructions ont ete dupliquees au niveau des codes */
  if(!EgaliteI(inf))  exit(1);
/* Coherence entre la longueur du registre Boundary Scan et le nombre de
   cellules enumerees */
  if(!LgBS_NbCel(inf))  exit(1);
/* Verification si des cellules ont ete dupliquees au niveau du nom et du
   mode */
  if(!EgaliteNomCel(inf))  exit(1);
/* Les cellules de mode control commandent-elles bien les bonnes cellules ? */
  if(!CelControl(inf))  exit(1);
}
}


/****************************************************************************/
/* Coherences entre structures : des vecteurs paralleles et informations */ 
/****************************************************************************/
extern void VerificationsPat(struct paseq *pat)
{

/* Presence du nom et mode des entrees/sorties de la structure des vecteurs
   paralleles dans la structure informations */
  if(!NamePaiol(pat))
  {
    fprintf(stderr,"Loading failed!\n");
    exit(1);
  }

}
/****************************************************************************/

