/*####==============================================================####*/
/*##                                                                  ##*/
/*##  This file is part of BOUNDARY SCAN TESTER for PC/Linux Systems  ##*/
/*##  (C) 1995, 1996 MASI, CAO-VLSI Team                              ##*/
/*##                                                                  ##*/
/*##  Authors            : Philippe OLEK, Olivier SIROL (1995)        ##*/
/*##  Authors            : Eric MECHIN (1996)                         ##*/
/*##                                                                  ##*/
/*##  E-mail support:    cao-vlsi@masi.ibp.fr                         ##*/
/*##                                                                  ##*/
/*####==============================================================####*/
/****************************************************************************/
#include "global.h"
/* Pointeurs dans la structure de donnees informations */
static Codes *InstCodeCourant=NULL;  
static Instructions *InstCourante=NULL;
static Cellules *CelCourante=NULL;
/****************************************************************************/
/* Avale les espaces et es retours chariot */
static int Separateurs(chaine, index) 
  char *chaine;
  int index;
{
  while(chaine[index] == ' ' || chaine[index] == '\n')
    index++;
  return index;
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant le nom de l'instructions */
static int NomInst(ligne, indexe) 
  char *ligne;
  int indexe;
{
  char *chaine;
  int j = 0, longueur = strlen(ligne);

  chaine = (char*)mbkalloc(longueur + 1);
  while(ligne[indexe] != ' ' && ligne[indexe] != '\n' && ligne[indexe] != '(') {
    chaine[j++] = ligne[indexe++];
    if(indexe == longueur) {
      fprintf(stderr,"position %d: incomplete instruction line!\n", indexe);
      mbkfree(chaine);
      return(0);
    }
  }
  if(j == 0) {
    fprintf(stderr,"position %d: instruction name is missing!\n", indexe);
    mbkfree(chaine);
    return(0);
  }
  chaine[j] = '\0';
  InstCourante -> nom = (char*)namealloc(chaine);
  mbkfree(chaine);
  return indexe++;
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant un code de l'instructions */
static int CodesInst(ligne, indexe)
  char *ligne;
  int indexe;
{
  static int NbBit = 0; /*nbr de bits de l'instruction precedente, pour verifier que les instructions ont toutes la meme longueur*/
  int j = 0, longueur = strlen(ligne);
  char *valeur;

  valeur = (char*)mbkalloc(256);
  while(TRUE) {
    while(ligne[indexe] == '0' || ligne[indexe] == '1') {
      valeur[j++] = ligne[indexe++];
      if(indexe >= longueur) {
	fprintf(stderr,"code instruction line incomplete!\n");
	mbkfree(valeur);
	return(0);
      }
    }
    valeur[j] = '\0';
    if(valeur[0] == '\0') {
      fprintf(stderr,"position %d: binary value for coding instruction is missing\n", indexe);
      mbkfree(valeur);
      return(0);
    }
    if(InstCodeCourant -> valeur == NULL)
      InstCodeCourant -> valeur = (char*)namealloc(valeur);
    else {
      InstCodeCourant -> suivant = (Codes*)mbkalloc(sizeof(Codes));
      InstCodeCourant = InstCodeCourant -> suivant;
      InstCodeCourant -> valeur = (char*) namealloc(valeur);
      InstCodeCourant -> suivant = NULL;
    }
    valeur[0] = '\0';
    if(NbBit == 0)
      NbBit = j;
    else if(NbBit != j) {
      fprintf(stderr,"instruction codes have different sizes!\n");
      mbkfree(valeur);
      return(0);
    }
    j = 0;
    indexe = Separateurs(ligne, indexe);
    if(indexe == longueur) {
      fprintf(stderr,"incomplete line!\n");
      mbkfree(valeur);
      return(0);
    }
    if(ligne[indexe] != ')') {
      if(ligne[indexe] == ',') {
	indexe++;
	if(indexe == longueur) {
	  fprintf(stderr,"instructions code line Erreur incomplete!\n");
	  mbkfree(valeur);
	  return(0);
	}
	indexe = Separateurs(ligne, indexe);
	if(indexe == longueur) {
	  fprintf(stderr,"instructions code line Erreur incomplete!\n");
	  mbkfree(valeur);
	  return(0);
	}
      }
      else {
	fprintf(stderr,"position %d: a comma is missing in instruction code!\n", indexe);
	mbkfree(valeur);
	return(0);
      }
    }
    else {mbkfree(valeur);return indexe;}
  }
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant les codes de l'instructions */
static int CodeOpInst(ligne)
  char *ligne;
{
  int i = 0, longueur = strlen(ligne);

  if(ligne[i++] != '"') {
    fprintf(stderr,"position %d: '\"' is missing in instruction encoding!\n", i);
    return(0);
  }
  if(i == longueur) {
    fprintf(stderr,"position %d: line incomplete in instruction encoding!\n", i);
    return(0);
  }
  i = Separateurs(ligne, i);
  if(i == longueur) {
    fprintf(stderr,"position %d: line incomplete in instruction encoding!\n", i);
    return(0);
  }

  while(i < longueur - 3) {
    if(InstCourante -> code -> valeur != NULL) {
      InstCourante -> suivant = (Instructions*)mbkalloc(sizeof(Instructions));
      InstCourante -> suivant -> nom = NULL;
      InstCourante -> suivant -> code = NULL;
      InstCourante -> suivant -> suivant = NULL;

      InstCourante -> suivant -> code = (Codes*)mbkalloc(sizeof(Codes));
      InstCourante -> suivant -> code -> valeur = NULL;
      InstCourante -> suivant -> code -> suivant = NULL;
      InstCourante = InstCourante -> suivant;
      InstCodeCourant = InstCourante -> code;
    }
    if(!(i = NomInst(ligne, i))) {
      fprintf(stderr,"instruction code error!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: incomplete lione in code!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
      return(0);
    }
    if(ligne[i++] != '(') {
      fprintf(stderr,"position %d: '(' is missing in encoding instructions!\n", i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
      return(0);
    }
    if(!(i = CodesInst(ligne, i))) {
      fprintf(stderr,"Error in encoding instructin codes!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
      return(0);
    }
    if(ligne[i++] != ')') {
      fprintf(stderr,"position %d: ')' is missing in encoding instructions!\n", i);
      return(0);
    }
    if(i < longueur - 3) {
      i = Separateurs(ligne, i);
      if(ligne[i++] != ',') {
	fprintf(stderr,"position %d: a comma is missing in encoding instructions!\n", i);
	return(0);
      }
      if(i == longueur) {
	fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
	return(0);
      }
      i = Separateurs(ligne, i);
      if(i == longueur) {
	fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
	return(0);
      }
    }
  }
  if(ligne[i++] != ';') {
    fprintf(stderr,"position %d: ';' is missing in encoding instructions!\n", i);
    return(0);
   }
  if(i == longueur) {
    fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
    return(0);
  }
  i = Separateurs(ligne, i);
  if(i == longueur) {
    fprintf(stderr,"position %d: incomplete line in encoding instructions!\n", i);
    return(0);
  }
  if(ligne[i] != '"') {
    fprintf(stderr,"position %d: '\"' is missing in encoding instructions!\n", i);
    return(0);
  }
  
  return(1);
}
/****************************************************************************/
/* Initialisation de la structure de donnees associee au fichier 
   informations */
static int InitStruct() {
  int i = 0;

  InfosVbe=(Infos*) mbkalloc(sizeof(Infos));
  InfosVbe->lg_inst = 0;
  InfosVbe->inst = NULL;
  InfosVbe->lg_bs = 0;
  InfosVbe->cel = NULL;
  InfosVbe->next=NULL;  /*le .bsdl ne peut decrire qu'un seul circuit BS*/

  InfosVbe->inst = (Instructions*)mbkalloc(sizeof(Instructions));
  if (InfosVbe->inst == NULL) {
    fprintf(stderr,"Out of memory!\n");
    exit(1);
  }
  InfosVbe->inst -> nom = NULL;
  InfosVbe->inst -> code = NULL;
  InfosVbe->inst -> suivant = NULL;

  InfosVbe->inst -> code = (Codes*)mbkalloc(sizeof(Codes));
  if (InfosVbe->inst -> code == NULL) {
    fprintf(stderr,"Out of memory!\n");
    exit(1);
  }
  InfosVbe->inst -> code -> valeur = NULL;
  InfosVbe->inst -> code -> suivant = NULL;

  InfosVbe->cel = (Cellules*)mbkalloc(sizeof(Cellules));
  if (InfosVbe->cel == NULL) {
    fprintf(stderr,"Out of memory!\n");
    exit(1);
  }
  InfosVbe->cel -> nom = NULL;
  for(i = 0; i < 5; i++)
    InfosVbe->cel -> donnees[i] = -1;
  InfosVbe->cel -> suivant = NULL;

  InstCodeCourant = (Codes*)(InfosVbe->inst -> code);

  InstCourante = (Instructions*)InfosVbe->inst;

  CelCourante = (Cellules*)InfosVbe->cel;

  return(1);
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant le nom de la cellule Boundary 
   Scan */
static int Nom(ligne, indexe) 
  char *ligne;
  int indexe;
{
  char *chaine;
  int j = 0, longueur = strlen(ligne);

  chaine = (char *)mbkalloc(longueur + 1);

/*controle des separateurs*/
  while(ligne[indexe] != ' ' && ligne[indexe] != '\n' &&
	ligne[indexe] != ',' && ligne[indexe] != '(') {
    chaine[j++] = ligne[indexe++];
    if(indexe == longueur) {
      fprintf(stderr,"position %d: incomplete line in name!\n", indexe);
      mbkfree(chaine);
      return(0);
    }
  }
/*elimination des separateurs*/
  if(ligne[indexe] == ' ' || ligne[indexe] == '\n') {
    indexe = Separateurs(ligne, indexe);
    if(indexe == longueur) {
      fprintf(stderr,"position %d: incomplete line in name!\n", indexe);
      mbkfree(chaine);
      return(0);
    }
  } 
  if(ligne[indexe] == '(') {
    indexe++;
    indexe = Separateurs(ligne, indexe);
    if(indexe == longueur) {
      fprintf(stderr,"position %d: incomplete line in name!\n", indexe);
      mbkfree(chaine);
      return(0);
    } 
    if(ligne[indexe] == ')') {
      fprintf(stderr,"position %d: a value is missing in name!\n", indexe);
      mbkfree(chaine);
      return(0);
    }
    if(ligne[indexe] == ',') {
      fprintf(stderr,"position %d: a value and ')' are missing in name!\n", indexe);
      mbkfree(chaine);
      return(0);
    }
    chaine[j++] = ' ';
    while(ligne[indexe] != ')') { 
      chaine[j++] = ligne[indexe++];
      if(indexe == longueur) {
	fprintf(stderr,"position %d: incomplete line in name!\n", indexe);
	mbkfree(chaine);
	return(0);
      } 
    }
    indexe++; 
    indexe = Separateurs(ligne, indexe);
    if(indexe == longueur) {
      fprintf(stderr,"position %d: incomplete line in name!\n", indexe);
      mbkfree(chaine);
      return(0);
    } 
  }
  chaine[j] = '\0';
  CelCourante -> nom = namealloc(chaine);
  mbkfree(chaine);
  return indexe++;
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant le mode la cellule Boundary Scan */
static int Mode(ligne, indexe) 
  char *ligne;
  int indexe;
{
  char *chaine;
  int j = 0, longueur = strlen(ligne);

  chaine = (char*)mbkalloc(longueur + 1);
  while(ligne[indexe] != ' ' && ligne[indexe] != '\n' && ligne[indexe] != ',') {
    chaine[j++] = toupper(ligne[indexe++]);
    if(indexe == longueur) {
      fprintf(stderr,"position %d: incomplete line in mode!\n", indexe);
      mbkfree(chaine);
      return(0);
    }
  }
  chaine[j] = '\0';
  if (!strcmp(chaine, "INPUT"))
    CelCourante -> donnees[DMODE] = INPUT;
  else if(!strcmp(chaine, "OUTPUT3"))
    CelCourante -> donnees[DMODE] = OUTPUT3;
  else if(!strcmp(chaine, "OUTPUT2"))
    CelCourante -> donnees[DMODE] = OUTPUT2;
  else if(!strcmp(chaine, "CONTROL"))
    CelCourante -> donnees[DMODE] = CONTROL;
  else if(!strcmp(chaine, "CLOCK"))
    CelCourante -> donnees[DMODE] = CLOCK;
  else {
    fprintf(stderr,"position %d: mode unknown!\n", indexe);
    mbkfree(chaine);
    return(0);
  }
  mbkfree(chaine);
  return indexe++;
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant le libelle de la cellule qui 
   controle la presente cellule Boundary Scan 3 etats */
static int Control(ligne, indexe) 
  char *ligne;
  int indexe;
{
  int longueur = strlen(ligne);

  if(indexe == longueur) {
    fprintf(stderr,"position %d: incomplete line in binary!\n", indexe);
    return(0);
  }
  if(ligne[indexe] == '*') {
    CelCourante -> donnees[DCONTROLE] = -1;
    indexe++;
  }
  else if(ligne[indexe] >= '0' && ligne[indexe] <= '9') {
    CelCourante -> donnees[DCONTROLE] = 0;
    while(ligne[indexe] >= '0' && ligne[indexe] <= '9')
      CelCourante -> donnees[DCONTROLE] = CelCourante -> donnees[DCONTROLE] * 10
					 + ligne[indexe++] - '0';
  }  
  else {
    fprintf(stderr,"position %d: value unknown for control!\n", indexe);
    return(0);
  }
  return indexe;
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant soit la valeur de defaut soit la 
   valeur de commande de la cellule Boundary Scan */
static int Binaire(ligne, indexe, pos) 
  char *ligne;
  int indexe;
  int pos;
{
  int longueur = strlen(ligne);

  if(indexe == longueur) {
    fprintf(stderr,"position %d: incomplete line in binary!\n", indexe);
    return(0);
  }
  if(ligne[indexe] == '0')
    CelCourante -> donnees[pos] = 0;
  else if(ligne[indexe] == '1')
    CelCourante -> donnees[pos] = 1;
  else if(ligne[indexe] == '*')
    CelCourante -> donnees[pos] = -1;
  else {
    fprintf(stderr,"position %d: binary value unknown!\n", indexe);
    return(0);
  }
  indexe++;
  return indexe;
}
/****************************************************************************/
/* Analyse la chaine de caracteres formant l'information d'inversion ou non 
   de la cellule Boundary Scan */
static int Inversion(ligne, indexe) 
  char *ligne;
  int indexe;
{
  int longueur = strlen(ligne);

  if(indexe == longueur) {
    fprintf(stderr,"position %d: incomplete line in inversion!\n", indexe);
    return(0);
  }
  if(toupper(ligne[indexe]) == 'O')
    CelCourante -> donnees[DINVERSION] = 1;
  else if(toupper(ligne[indexe]) == 'N')
    CelCourante -> donnees[DINVERSION] = 0;
  else if(ligne[indexe] == '*')
    CelCourante -> donnees[DINVERSION] = -1;
  else {
    fprintf(stderr,"position %d: character unknown in inversion!\n", indexe);
    return(0);
  }
  indexe++;
  return indexe;
}
/****************************************************************************/
/* Analyse la ligne donnant les informations sur la cellule Boundary Scan */
static int CelluleBS(ligne) 
  char *ligne;
{
  int i = 0, longueur = strlen(ligne);

  while(i < longueur) {
    if(ligne[i++] != '"') {
      fprintf(stderr,"position %d: '\"' is missing in BS cell!\n", i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
/*nom du connecteur*/
    if(!(i = Nom(ligne, i))) {
      fprintf(stderr,"Error BS cell!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != ',') {
      fprintf(stderr,"position %d: a comma is missing in BS cell!\n", i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
/*mode de la cellule*/
    if(!(i = Mode(ligne, i))) {
      fprintf(stderr,"Error BS cell!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != ',') {
      fprintf(stderr,"position %d: a comma is missing in BS cell!\n", i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
/*defaut*/
    if(!(i = Binaire(ligne, i, DDEFAUT))) {
      fprintf(stderr,"Error BS cell!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != ',') {
      fprintf(stderr,"position %d: a comma is missing in BS cell!\n", i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
/*control*/
    if(!(i = Control(ligne, i))) {
      fprintf(stderr,"Error BS cell!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != ',') {
     fprintf(stderr,"position %d: a comma is missing in BS cell!\n", i);
     return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
/*commande*/
    if(!(i = Binaire(ligne, i, DCOMMANDE))) {
      fprintf(stderr,"Error BS cell!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != ',') {
      fprintf(stderr,"position %d: a comma is missing in BS cell!\n", i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
/*inversion*/
    if(!(i = Inversion(ligne, i))) {
      fprintf(stderr,"Error BS cell!\n");
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != ';') {
      fprintf(stderr,"position %d: ';' is missing in BS cell!\n",i);
      return(0);
    }
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    i = Separateurs(ligne, i);
    if(i == longueur) {
      fprintf(stderr,"position %d: BS cell line incomplete!\n", i);
      return(0);
    }
    if(ligne[i++] != '"') {
      fprintf(stderr,"position %d: '\"' is missing in BS cell!\n", i);
      return(0);
    }
  }
  return(1);
}
/****************************************************************************/
/* Creation de la structure de donnees associee au fichier informations */
static int CreerInfos(vbe) 
struct befig *vbe;
{
  int i = 0;
  int j = 0;
  int lg_bs = 0;
  int lg_inst_bs = 0;
  int inst_bs = 0;
  int cel_bs = 0;
  int indice = 0;
  struct begen *gen;

  if(!InitStruct()) {
    fprintf(stderr,"Error!\n");
    return(0);
  }

  for(gen = vbe -> BEGEN; gen; gen = gen -> NEXT) {
    if(*gen -> TYPE == 'n') {
/*parseur de la ligne pour la taille du registre instruction*/
      if(!strcmp(gen->NAME, "bs_instruction_length")) {
	if(lg_inst_bs) {
	  fprintf(stderr,"duplicate 'instruction_length' !\n");
	  return(0);
	}
	InfosVbe->lg_inst = *(long*)gen -> VALUE;
	lg_inst_bs = 1;
      }
/*parseur de la ligne pour la taille de la chaine boundary scan*/
      else if(!strcmp(gen->NAME, "bs_length")) {
	if(lg_bs) {
	  fprintf(stderr,"duplicate 'bs_length' !\n");
	  return(0);
	}
	InfosVbe->lg_bs = *(long*)gen -> VALUE;
	lg_bs = 1;
      }
      else {
	fprintf(stdout,"Instruction '%s' unknown!\n",gen->NAME);
	return(0);
      }
    }
/*parseur de la ligne pour la liste des instructions*/
    else if(*gen -> TYPE == 's') {
      if(!strcmp(gen->NAME, "bs_instructions")) {
	if(inst_bs) {
	  fprintf(stderr,"duplicate 'bs_instructions' !\n");
	  return(0);
	}
	if(!CodeOpInst(gen -> VALUE)) {
	  fprintf(stderr,"Error!\n");
	  return(0);
	}
	inst_bs = 1;
      }
/*parseur des  lignes de description de la chaine BS*/
/*commence par BS_i.........*/
      else if(gen->NAME[0] == 'b' && gen->NAME[1] == 's' && gen->NAME[2] == '_') {
	for(i = 3; i < strlen(gen->NAME); i++) {
	  if(gen->NAME[i] >= '0' && gen->NAME[i] <= '9')
	    indice = indice * 10 + (gen->NAME[i] - '0');
	  else {
	    fprintf(stderr,"%s index unknown!\n",gen->NAME);
	    return(0);
	  }
	}
	CelCourante = InfosVbe -> cel;
	i = 0;
	while(i < indice) {
	  if(CelCourante -> suivant == NULL) {
	    CelCourante -> suivant = (Cellules*)mbkalloc(sizeof(Cellules));
	    if(CelCourante -> suivant == NULL) {
	      fprintf(stderr,"Out of memory!\n");
	      exit(1);
	    }
	    CelCourante -> suivant -> nom = NULL;
	    for(j = 0; j < 5; j++)
	      CelCourante -> suivant -> donnees[j] = -1;
	    CelCourante -> suivant -> suivant = NULL;
	    CelCourante = CelCourante -> suivant;
	  }
	  else
	    CelCourante = CelCourante -> suivant;
	  i++;
	}
	indice = 0;
	if(CelCourante -> nom == NULL) {
	  if(!CelluleBS(gen->VALUE)) {
	    fprintf(stderr,"Error!\n");
	    return 0;
	  }
	}
	else {
	  fprintf(stderr,"duplicate cell %s!\n",gen->NAME);
	  return(0);
	}
	cel_bs = 1;
      }
      else {
	fprintf(stderr,"Instruction unknown!\n");
	return(0);
      }
    }
    else {
      fprintf(stderr,"instruction type unknown!\n");
      return(0);
    }
  }
  if(!lg_bs) {
    fprintf(stderr,"file incomplete\n");
    fprintf(stderr,"BS register size is missing!\n");
    return(0);
  }
  if(!lg_inst_bs) {
    fprintf(stderr,"file incomplete\n");
    fprintf(stderr,"instruction register size is missing!\n");
    return(0);
  }
  if(!inst_bs) {
    fprintf(stderr,"file incomplete\n");
    fprintf(stderr,"BS instructions are missing!\n");
    return(0);
  }
  if(!cel_bs) {
    fprintf(stderr,"file incomplete\n");
    fprintf(stderr,"BS cells are missing!\n");
    return(0);
  }
  i = 0;
  for(CelCourante = InfosVbe -> cel; CelCourante; CelCourante = CelCourante -> suivant) {
    if(CelCourante -> nom == NULL) {
      fprintf(stderr,"the cell with the index %d is missing!\n", i);
      return(0);
    }
    i++;
  }

  return(1);
}
/****************************************************************************/
/*construction de la variable globale InfosVbe*/
/*Rq: on ne peut que decrire qu'un seul ensemble boundary scan malgre la presence d'un champ next dans la structure. Avec quelques modifications dans le parseur et dans la construction de la var. , on peut changer cet etat de faits.*/ 
int ReadBSDL(name)
char *name;
{
FILE *test;
struct befig *vbe=NULL;/*structure formee a partir du parsing du fichier bsdl*/

putenv("VH_BEHSFX=bsdl"); 

fprintf(stdout,"Loading BSDL file %s.bsdl...", name);
fflush(stdout);

/*test d'existence du fichier*/
test=fopen(concat(name,".bsdl"),"r");
if (ferror(test)!=0) {
     fprintf(stderr,"Unable to load %s.bsdl\n",name);
     return 1;
}
fclose(test);

/*chargement du fichier*/
vbe = vhdlloadbefig(NULL,name,0);
if(vbe==NULL || vbe -> ERRFLG != 0) {
     fprintf(stderr,"Unable to load %s.bsdl\n",name);
     return(1);
  }

if(vbe -> BEGEN != NULL) {
    /* Creation de la structure de donnees associees au fichier informations */
    fprintf(stdout,"Analysing BSDL file %s.bsdl\n", name);
    if(!CreerInfos(vbe)) {
      return(1);
    }
  }

else {
    fprintf(stderr,"Cannot find BSDL informations in %s.bsdl\n",name);
    return(1);
  }

return(0);
}
/****************************************************************************/
/* Visualise la structure de donnees associee au fichier informations */ 
void AfficheInfos(infos) 
  Infos infos;
{
  Instructions *ICourant;
  Codes *ICourantCode;
  Cellules *CCourant;

  fprintf(stdout,"Informations file vbe :\n");
  fprintf(stdout,"\tLength Instruction : %d\n", infos.lg_inst);
  fprintf(stdout,"\tInstructions :\n");
  ICourant = infos.inst;
  while(ICourant != NULL) {
    fprintf(stdout,"\t\tName : %s\n", (char*)(ICourant -> nom));
    ICourantCode = ICourant -> code;
    fprintf(stdout,"\t\tCodes :\n");
    while(ICourantCode != NULL) {
      fprintf(stdout,"\t\t\tValue : %s\n", (char*)ICourantCode -> valeur);
      ICourantCode = ICourantCode -> suivant;
    }
    fprintf(stdout,"\n");
    ICourant = ICourant -> suivant;
  }
  fprintf(stdout,"\tLength BS : %d\n", infos.lg_bs);
  getchar();
  fprintf(stdout,"\tCells :\n");
  CCourant = infos.cel;
  while(CCourant != NULL) {
    fprintf(stdout,"\t\tName : %s\n", CCourant -> nom);
    if(CCourant -> donnees[DMODE] == -1)
      fprintf(stdout,"\t\tMode : *\n");
    else if(CCourant -> donnees[DMODE] == INPUT)
      fprintf(stdout,"\t\tMode : INPUT\n");
    else if(CCourant -> donnees[0] == OUTPUT3)
      fprintf(stdout,"\t\tMode : OUTPUT3\n");
    else if(CCourant -> donnees[DMODE] == OUTPUT2)
      fprintf(stdout,"\t\tMode : OUTPUT2\n");
    else if(CCourant -> donnees[DMODE] == CONTROL)
      fprintf(stdout,"\t\tMode : CONTROL\n");
    else
      fprintf(stdout,"\t\tMode : CLOCK\n");
    if(CCourant -> donnees[DDEFAUT] == -1)
      fprintf(stdout,"\t\tDefault : *\n");
    else
      fprintf(stdout,"\t\tDefault : %d\n", CCourant -> donnees[DDEFAUT]);
    if(CCourant -> donnees[DCONTROLE] == -1)
      fprintf(stdout,"\t\tControl : *\n");
    else
      fprintf(stdout,"\t\tControl : %d\n", CCourant -> donnees[DCONTROLE]);
    if(CCourant -> donnees[DCOMMANDE] == -1)
      fprintf(stdout,"\t\tCommand : *\n");
    else
      fprintf(stdout,"\t\tCommand : %d\n", CCourant -> donnees[DCOMMANDE]);
    if(CCourant -> donnees[DINVERSION] == -1)
      fprintf(stdout,"\t\tInversion : *\n");
    else if(CCourant -> donnees[DINVERSION] == 1)
      fprintf(stdout,"\t\tInversion : OUI\n");
    else
      fprintf(stdout,"\t\tInversion : NON\n");
    CCourant = CCourant -> suivant;
    fprintf(stdout,"\n");
    getchar();
  }
}
/****************************************************************************/
/****************************************************************************/
