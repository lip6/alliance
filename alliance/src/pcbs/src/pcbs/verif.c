/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##  Authors            : Francois DONNET (1998)                     ##*/
/*##                                                                  ##*/
/*##  E-mail        :    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/****************************************************************************/
#include "global.h"
/****************************************************************************/
/* Coherence entre longueurs des instructions et longueur du RI */  
static int LgI_NbBit(Infos *inf)
{
Instructions *PInst;
Codes *PCode;
for(PInst = inf->inst; PInst; PInst = PInst -> suivant)
  for(PCode = PInst -> code; PCode; PCode = PCode -> suivant) {
       if(strlen(PCode->valeur) > inf->lg_inst) {fprintf(stderr,"[BS ERROR] : the instruction length is taller than the size of instruction register!\n");return(0);}
       if(strlen(PCode->valeur) < inf->lg_inst) {fprintf(stderr,"[BS ERROR] : the size of instruction register is bigger than the instruction length!\n");return(0);}
  }
return(1);
}
/****************************************************************************/
/* Coherence entre le nombre d'instructions et la longueur du registre 
   instruction */
static int LgI_NbI(Infos *inf) 
{
  Instructions *PInst;
  Codes *PCode;
  int i = 0, NbInst = 0;

  for(PInst = inf->inst; PInst; PInst = PInst -> suivant)
    for(PCode = PInst -> code; PCode; PCode = PCode -> suivant)
      i++;
  NbInst = pow(2, inf->lg_inst);
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
static int PresenceI(Infos *inf, char *inst) 
{
  Instructions *pinst;
  Codes *pcode;
  char *code;
  int i = 0;

  for(pinst=inf->inst; pinst && strcmp(pinst->nom,inst); pinst=pinst->suivant);
  if(pinst == NULL) {
    fprintf(stderr,"[BS ERROR] %s : instruction is unknown!\n", (char*)inst);
    return(0);
  }
  if(!strcmp(inst, "extest")) {
    code = (char*)mbkalloc(inf -> lg_inst + 1);
    for(i = 0; i < inf -> lg_inst; i++)
      code[i] = '0';
    code[i] = '\0';
    for(pcode = pinst -> code; pcode; pcode = pcode -> suivant)
      if(!strcmp(pcode -> valeur, code)) {
	free(code);
	return(1);
      }
    free(code);
    fprintf(stderr,"[BS ERROR] %s : instruction code doesn' match with instruction EXTEST!\n", (char*)code);
    return(0);
  }
  if(!strcmp(inst, "bypass")) {
    code = (char*)mbkalloc(inf -> lg_inst +1);
    for(i = 0; i < inf -> lg_inst; i++)
      code[i] = '1';
    code[i] = '\0';
    for(pcode = pinst -> code; pcode; pcode = pcode -> suivant)
      if(!strcmp(pcode -> valeur, code)) {
	free(code);
	return(1);
      }
    free(code);
    fprintf(stderr,"[BS ERROR] %s : instruction code doesn' match with instruction BYPASS!\n", (char*)code);
    return(0);
  }
  return(1);
}
/****************************************************************************/
/* Verification si des instructions ont ete dupliquees au niveau des codes */
static int EgaliteI(Infos *inf) 
{
  Instructions *P1Inst, *P2Inst;
  Codes *P1Code,*P2Code;

for(P1Inst = inf->inst; P1Inst; P1Inst = P1Inst -> suivant) 
  for(P1Code = P1Inst -> code; P1Code; P1Code = P1Code -> suivant){
    for(P2Code=P1Code->suivant; P2Code; P2Code=P2Code->suivant)
      if(P1Code->valeur == P2Code->valeur) {fprintf(stderr,"[BS ERROR] %s : duplcated code!\n", P1Code->valeur);return(0);}
    for(P2Inst = P1Inst->suivant; P2Inst; P2Inst = P2Inst -> suivant) 
      for(P2Code = P2Inst -> code; P2Code; P2Code = P2Code -> suivant)
	if(P1Code->valeur == P2Code->valeur) {fprintf(stderr,"[BS ERROR] %s : duplcated code!\n", P1Code->valeur);return(0);}
  }
return(1);
}
/****************************************************************************/
/* Coherence entre la longueur du registre Boundary Scan et le nombre de
   cellules enumerees */
static int LgBS_NbCel(Infos *inf) 
{
  Cellules *PCel;
  int i = 0;

  for(PCel = inf->cel; PCel; PCel = PCel -> suivant)
    i++;
  if(inf->lg_bs > i) {
    fprintf(stderr,"[BS ERROR] : tere are more BS cells than authorized!\n");
    return(0);
  }
  else if(inf->lg_bs < i) {
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
static int EgaliteNomCel(Infos *inf) 
{
  Cellules *P1Cel, *P2Cel;
  
  for(P1Cel = inf->cel; P1Cel; P1Cel = P1Cel -> suivant) {
    if(P1Cel->nom!=NULL && strcmp(P1Cel -> nom, "*"))
	 for(P2Cel = P1Cel -> suivant; P2Cel; P2Cel = P2Cel -> suivant){
	  if(P2Cel->nom!=NULL && !strcmp(P2Cel -> nom, P1Cel -> nom) &&
	     P1Cel -> donnees[DMODE] == P2Cel -> donnees[DMODE]) {
	    fprintf(stderr,"[BS ERROR] %s : duplicate cell!\n", P1Cel -> nom);
	    return(0);
	  }
    }
  }
  return(1);
}
/****************************************************************************/
/* Les cellules de mode control commandent-elles bien les bonnes cellules ? */
static int CelControl(Infos *inf) 
{
  Cellules *P1Cel, *P2Cel;
  int i = 0;

  for(P1Cel = inf->cel; P1Cel; P1Cel = P1Cel -> suivant)
    if(P1Cel -> donnees[DCONTROLE] != -1)
      for(P2Cel = inf->cel; P2Cel; P2Cel = P2Cel -> suivant)
	if(P1Cel -> donnees[DCONTROLE] == i++)
	  if(P2Cel -> donnees[DMODE] != CONTROL) {
	    fprintf(stderr,"[BS ERROR] : the cell named %s is controled by a cell %s wich isn't CONTROL!\n", P1Cel -> nom, P2Cel -> nom);
	    return(0);
	  }
  return(1);
}
/****************************************************************************/
/* Coherence entre le nombre d'entrees/sorties dans la structure des vecteurs
   paralleles et la longueur du registre Boundary Scan de la structure 
   informations */
static int Iolnbr_LgBS(struct paseq *pat)
{
  struct paiol *ppaiol;
  Cellules *cel;
  Infos *inf;
  int nb_io = 0, nb_cell_valid=0;

/* utilisons une propriete de namealloc() qui rend toujours le meme vecteur*/
char *tck=namealloc("tck");
char *trst=namealloc("trst");
char *tms=namealloc("tms");
char *tdi=namealloc("tdi");
char *tdo=namealloc("tdo");

/*pour toutes les entrees sorties sauf l'alim. qui ne passe par la chaine BS*/
  for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT)
    if (ppaiol->NAME!=tck && ppaiol->NAME!=trst && ppaiol->NAME!=tms &&
	ppaiol->NAME!=tdi && ppaiol->NAME!=tdo  &&
       !isvdd(ppaiol->NAME) && !isvss(ppaiol->NAME) && 
       (ppaiol->MODE=='I' || ppaiol->MODE=='O' || ppaiol->MODE=='T'))
	  nb_io++;
  
  for (inf=InfosVbe;inf;inf=inf->next)
       for(cel=inf->cel;cel;cel=cel->suivant)
	    if (cel->nom!=tck && cel->nom!=trst && cel->nom!=tms &&
		cel->nom!=tdi && cel->nom!=tdo  &&
		!isvdd(cel->nom) && !isvss(cel->nom) && 
		(cel->donnees[DMODE]==INPUT || cel->donnees[DMODE]==OUTPUT2 ||
		cel->donnees[DMODE]==INOUTPUT))
		    nb_cell_valid++;

  if(verbose_flag && nb_io > nb_cell_valid) {
    fprintf(stderr,"[PAT WARNING] : more data vector(%d) in PAT than in BS chain(%d)\n",nb_io,nb_cell_valid);
  }
  if(verbose_flag && nb_io < nb_cell_valid) {
    fprintf(stderr,"[PAT WARNING] : not enough data vector(%d) in PAT for BS chain(%d)\n",nb_io,nb_cell_valid);
  }
return(1);
}
/****************************************************************************/
/* Presence du nom et mode des entrees/sorties de la structure des vecteurs
   paralleles dans la structure informations */
static int NamePaiol(struct paseq *pat) 
{
  struct paiol *ppaiol;
  Cellules *pvbe,*c;
  Infos *inf,*i;
  int mode;

/* utilisons une propriete de namealloc() qui rend toujours le meme vecteur*/
char *tck=namealloc("tck");
char *trst=namealloc("trst");
char *tms=namealloc("tms");
char *tdi=namealloc("tdi");
char *tdo=namealloc("tdo");
ht *hsh_nom=addht(200);

/*pour toutes les entrees sorties sauf l'alim. qui ne passe par la chaine BS*/
  for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) {
    if (ppaiol->NAME!=tck && ppaiol->NAME!=trst && ppaiol->NAME!=tms &&
	ppaiol->NAME!=tdi && ppaiol->NAME!=tdo  &&
	!isvdd(ppaiol -> NAME) && !isvss(ppaiol -> NAME) &&
	(ppaiol->MODE == 'I' || ppaiol->MODE == 'O' || ppaiol->MODE == 'T'))
    {

/*recherche du connecteur dans la chaine boundary scan*/
      for(inf=InfosVbe; inf; inf=inf->next) {
	   for(pvbe = inf -> cel; pvbe; pvbe = pvbe -> suivant)
		if((pvbe->donnees[DMODE]==INPUT || 
		    pvbe->donnees[DMODE]==OUTPUT2 || 
		    pvbe->donnees[DMODE]==OUTPUT3 || 
		    pvbe->donnees[DMODE]==INOUTPUT) && pvbe->nom!=NULL && 
		   ppaiol->NAME!=NULL && pvbe->nom==ppaiol->NAME) break;
	   if (pvbe!=NULL) break;
      }


      if(pvbe == NULL) {
	fprintf(stderr,
	"[PAT ERROR] : connector named '%s' in PAT isn't declared as a BS cell!\n",
	ppaiol -> NAME);
	return(0);
      }

      mode=gethtitem(hsh_nom,pvbe->nom);
      switch (pvbe->donnees[DMODE]) {
      case INPUT:
	   if (ppaiol->MODE=='T' && mode!=OUTPUT3) {
		for (c=pvbe->suivant;c;c=c->suivant) 
		     if (c->nom==ppaiol->NAME) break;
		if (c!=NULL && c->donnees[DMODE]==OUTPUT3) {
		     addhtitem(hsh_nom,pvbe->nom,OUTPUT2);
		     break;}
		for(i=inf->next; i; i=i->next) { 
		     for(c = i -> cel; c; c = c->suivant) 
			  if (c->nom==ppaiol->NAME) break;
		     if (c!=NULL && c->donnees[DMODE]==OUTPUT3) break;}
		if (c!=NULL && c->donnees[DMODE]==OUTPUT3) {
		     addhtitem(hsh_nom,pvbe->nom,OUTPUT2);
		     break;}
		fprintf(stderr,
		"[PAT ERROR]  : 'in %s' is defined as 'inout %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
		return(0);}
	   else if (ppaiol->MODE=='O') {
		fprintf(stderr,
		"[PAT ERROR]  : 'in %s' is defined as 'out %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
		return(0);}
	   else if (ppaiol->MODE!='I' && ppaiol->MODE!='T') {
		fprintf(stderr,
		"[PAT ERROR] : 'in %s' has incompatible types in files!\n"
		,pvbe->nom);
		return(0);}
	   if (mode!=EMPTYHT) {
		if (mode==OUTPUT3) {
		     addhtitem(hsh_nom,pvbe->nom,OUTPUT2);
		     break;}
		fprintf(stderr,
		"[PAT ERROR] : '%s' already used!\n",pvbe->nom);
		return(0);}
	   addhtitem(hsh_nom,pvbe->nom,pvbe->donnees[DMODE]);
	   break;
      case OUTPUT2:
	   if (ppaiol->MODE=='I') {
		fprintf(stderr,
		"[PAT ERROR]  : 'out %s' is defined as 'in %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
		return(0);}
	   else if (ppaiol->MODE=='T') {
		fprintf(stderr,
		"[PAT ERROR]  : 'out %s' is defined as 'inout %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
		return(0);}
	   else if (ppaiol->MODE!='O') {
		fprintf(stderr,
		"[PAT ERROR] : 'out %s' has incompatible types in files!\n"
		,pvbe->nom);
		return(0);}
	   if (mode!=EMPTYHT) {
		fprintf(stderr,
		"[PAT ERROR] : '%s' already used!\n",pvbe->nom);
		return(0);}
	   addhtitem(hsh_nom,pvbe->nom,pvbe->donnees[DMODE]);
	   break;
      case OUTPUT3:
	   if (verbose_flag && ppaiol->MODE=='I') {
		fprintf(stderr,
		"[PAT WARNING]  : 'inout %s' is defined as 'in %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
	   }
	   else if (verbose_flag && ppaiol->MODE=='O') {
		fprintf(stderr,
		"[PAT WARNING]  : 'inout %s' is defined as 'out %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
	   }
	   else if (ppaiol->MODE!='T'&& ppaiol->MODE!='I'&& ppaiol->MODE!='O'){
		fprintf(stderr,
		"[PAT ERROR] : 'inout %s' has incompatible types in files!\n"
		,pvbe->nom);
		return(0);}
	   if (mode!=EMPTYHT) {
		if (mode==INPUT) {
		     addhtitem(hsh_nom,pvbe->nom,OUTPUT2);
		     break;}
		fprintf(stderr,
		"[PAT ERROR] : '%s' already used!\n",pvbe->nom);
		return(0);}
	   addhtitem(hsh_nom,pvbe->nom,pvbe->donnees[DMODE]);
	   break;
      case INOUTPUT:
	   if (verbose_flag && ppaiol->MODE=='I') {
		fprintf(stderr,
		"[PAT WARNING]  : 'inout %s' is defined as 'in %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
	   }
	   else if (verbose_flag && ppaiol->MODE=='O') {
		fprintf(stderr,
		"[PAT WARNING]  : 'inout %s' is defined as 'out %s' in PAT!\n"
		,pvbe->nom,pvbe->nom);
	   }
	   else if(ppaiol->MODE!='T'&& ppaiol->MODE!='I'&& ppaiol->MODE!='O') {
		fprintf(stderr,
		"[PAT ERROR] : 'inout %s' has incompatible types in files!\n"
		,pvbe->nom);
		return(0);}
	   if (mode!=EMPTYHT) {
		fprintf(stderr,
		"[PAT ERROR] : '%s' already used!\n",pvbe->nom);
		return(0);}
	   addhtitem(hsh_nom,pvbe->nom,pvbe->donnees[DMODE]);
	   break;
      }
    }
  }

/*recherche du cote de la description*/
  for(inf=InfosVbe; inf; inf=inf->next) {
       for(pvbe = inf -> cel; pvbe; pvbe = pvbe -> suivant)
	    if ((pvbe->donnees[DMODE]==INPUT || pvbe->donnees[DMODE]==OUTPUT2
		 || pvbe->donnees[DMODE]==OUTPUT3 || 
		 pvbe->donnees[DMODE]==INOUTPUT) && pvbe->nom!=NULL && 
	       !isvdd(pvbe->nom) && !isvss(pvbe->nom) &&
	       gethtitem(hsh_nom,pvbe->nom)==EMPTYHT) {
		 fprintf(stderr,"BS cell connector named '%s' isn't declared in PAT!\n",pvbe->nom);
		 return 0;
	    }
  }


delht(hsh_nom);
return(1);
}
/****************************************************************************/
/* Coherences dans la structure informations */
int VerificationsInfos()
{
Infos *inf;

/*pour tous les ensembles boundary scan chaines*/
for (inf=InfosVbe;inf;inf=inf->next) {
/* Coherence entre le nombre d'instructions et la longueur du registre 
   instruction */
  if(!LgI_NbI(inf))  return(1);
/* Coherence entre longueurs des instructions et longueur du RI */  
  if(!LgI_NbBit(inf))  return(1);
/*dans tout circuit boundary scan il existe intest="?...?" */
  if(!PresenceI(inf,"intest"))  return(1);
/*dans tout circuit boundary scan il existe bypass="1...1" */
  if(!PresenceI(inf,"bypass"))  return(1);
/*dans tout circuit boundary scan sauf EMULBS il existe extest="0...0" */
  if(!emulbs_flag && !PresenceI(inf,"extest"))  return(1);
/* Verification si des instructions ont ete dupliquees au niveau des codes */
  if(!EgaliteI(inf))  return(1);
/* Coherence entre la longueur du registre Boundary Scan et le nombre de
   cellules enumerees */
  if(!LgBS_NbCel(inf))  return(1);
/* Verification si des cellules ont ete dupliquees au niveau du nom et du
   mode */
  if(!EgaliteNomCel(inf))  return(1);
/* Les cellules de mode control commandent-elles bien les bonnes cellules ? */
  if(!CelControl(inf))  return(1);
}
return(0);
}
/****************************************************************************/
/* Coherences entre structures : des vecteurs paralleles et informations */ 
int VerificationsPat(struct paseq *pat)
{

/* Presence du nom et mode des entrees/sorties de la structure des vecteurs
   paralleles dans la structure informations */
  if(!NamePaiol(pat)) return(1);

/* Coherence entre le nombre d'entrees/sorties dans la structure des vecteurs
   paralleles et la longueur du registre Boundary Scan de la structure 
   informations */
  if(!Iolnbr_LgBS(pat)) return(1); 
 
  return(0);
}
/****************************************************************************/










