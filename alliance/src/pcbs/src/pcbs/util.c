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
/****************************************************************************/
/****************************************************************************/
/* Calcul base elevee a la puissance exposant */
int pow(base, exposant)
  int base;
  int exposant;
{
  int i,resultat=1;
  for (i=0;i<exposant;i++) resultat=resultat*base;
  return(resultat);
} 
/****************************************************************************/
/* Copie s dans dupchaine */
char *dupchaine(s) 
  char *s;
{
  char *p;

  if (s==NULL) return NULL;
  p = (char *) mbkalloc(strlen(s) + 1);
  if (p != NULL)  strcpy(p, s);
  return p;
}
/*****************************************************************************/
/*concatene deux chaines en allouant un nouvel espace memoire dans l'environnement mbk de la facon suivante:   s1 + ' ' + s2  ------>impossible a desallouer*/
char *concat_espace(char *s1,char *s2)
{
char *res,*mbk_res;
int t_s1,t_s2,t_res;
int i=0,j=0;

/*tests chaines vides*/
if (s2==NULL && s1==NULL) return NULL;
if (s1==NULL || *s1=='\0') return namealloc(s2);
if (s2==NULL || *s2=='\0') return namealloc(s1);

	t_s1=strlen(s1);
	t_s2=strlen(s2);
	t_res=t_s1+t_s2+2;
	/*  2 --> espace entre s1 et s2 plus le '\0' de fin de chaine*/
	res=(char*) mbkalloc(t_res);
	if (res==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	for (i=0;i<t_s1;i++) res[i]=s1[i];
	res[i++]=' ';		/* 1 espace entre s1 et s2! */
	for (;i<t_res;i++) res[i]=s2[j++];
	res[i]='\0';		/*fin de la chaine*/

mbk_res=namealloc(res);
mbkfree(res);
return mbk_res;
}
/*****************************************************************************/
/*remplace tous les espaces d'un mot par des '_'  ------>     ne peut pas etre desallouer!! */
char *del_espace(char *s)
{
char *res,*mbk_res;
int i=0;

/*tests chaines vides*/
if (s==NULL || *s=='\0') return namealloc(s);

	res=(char*) mbkalloc(strlen(s)+1);
	if (res==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	for (i=0;i<=strlen(s);i++)
	     if (s[i]==' ') res[i]='_';
	     else res[i]=s[i];

mbk_res=namealloc(res);
mbkfree(res);
return mbk_res;
}
/*****************************************************************************/
/*remplace tous les espaces d'un mot par des '_' ----> pour mbkfree() */
char *del_espace_mbk(char *s)
{
char *res;
int i=0;

/*tests chaines vides*/
if (s==NULL || *s=='\0') return dupchaine(s);

	res=(char*) mbkalloc(strlen(s)+1);
	if (res==NULL) {fprintf(stderr,"Not enough memory!");exit(1);}
	for (i=0;i<=strlen(s);i++)
	     if (s[i]==' ') res[i]='_';
	     else res[i]=s[i];

return (res);
}
/*****************************************************************************/
/*concatene deux chaines en allouant un nouvel espace memoire dans l'environnement mbk  -----> impossible a deallouer*/
char *concat(char *s1,char *s2)
{
char *res,*mbk_res;

/*tests chaines vides*/
if (s2==NULL && s1==NULL) return NULL;
if (s1==NULL || *s1=='\0') return namealloc(s2);
if (s2==NULL || *s2=='\0') return namealloc(s1);

	res=(char*)mbkalloc(strlen(s1)+strlen(s2)+1);
        /*ne pas oublier le caractere de fin de chaine*/
	res=strcpy(res,s1);
	res=strcat(res,s2);

mbk_res=namealloc(res);
mbkfree(res);
return mbk_res;
}
/*****************************************************************************/
/*concatene deux chaines en allouant un nouvel espace memoire pour ensuite etre liberer par mbkfree() */
char *concat_mbk(char *s1,char *s2)
{
char *res;

/*tests chaines vides*/
if (s2==NULL && s1==NULL) return NULL;
if (s1==NULL || *s1=='\0') return dupchaine(s2);
if (s2==NULL || *s2=='\0') return dupchaine(s1);

	res=(char*)mbkalloc(strlen(s1)+strlen(s2)+1);
        /*ne pas oublier le caractere de fin de chaine*/
	res=strcpy(res,s1);
	res=strcat(res,s2);

return res;
}
/*****************************************************************************/
/* convertit un integer en string -------> impossible a desallouer*/
char *itoa(int nbr)
{
char tab[15];              /*espace suffisant pour un int sur 32 bits*/

sprintf(tab,"%d",nbr);

return namealloc(tab);
}
/*****************************************************************************/
/* convertit un integer en string pour mbkfree()*/
char *itoa_mbk(int nbr)
{
char tab[15];              /*espace suffisant pour un int sur 32 bits*/

sprintf(tab,"%d",nbr);

return dupchaine(tab);
}
/*****************************************************************************/
/* convertit un caractere en string -----> impossible a desallouer*/
char *ctoa(char c)
{
char tab[2];    
     tab[0]=c;
     tab[1]='\0';
return namealloc(tab);
}
/*****************************************************************************/
/* convertit un caractere en string -----> pour mbkfree() */
char *ctoa_mbk(char c)
{
char tab[2];    
     tab[0]=c;
     tab[1]='\0';
return dupchaine(tab);
}
/*****************************************************************************/
/* Conversion int en ASCII-binaire ----> impossible a desallouer*/
char * itoabin(valeur) 
  int valeur;
{
  int i;
  char abin[33];

  abin[32] = '\0';
  for(i = 0; i < 32; i++) 
    abin[31 - i] = (((valeur >> i) & 1)  + '0'); 
  return namealloc(abin);
}
/*****************************************************************************/
/* Conversion int en ASCII-binaire ----> pour mbkfree() */
char * itoabin_mbk(valeur) 
  int valeur;
{
  int i;
  char abin[33];

  abin[32] = '\0';
  for(i = 0; i < 32; i++) 
    abin[31 - i] = (((valeur >> i) & 1)  + '0'); 
  return dupchaine(abin);
}
/*****************************************************************************/
/* Conversion Ascii-binaire en int */
int abintoi(abin ) 
  char *abin;
{
  int valeur = 0;
  char *yp;

  for(yp = abin; *yp; yp++) {
    valeur <<= 1;
    valeur += *yp - '0';
  }
  return valeur;
}
/*****************************************************************************/
/*detruit tout une sequence de patterns*/
void erase_paseq(paseq_list *pat)
{
if (pat==NULL) return;
erase_paseq(pat->NEXT);
 /*les patterns*/
 if (pat->OLDPAT!=NULL) pat_frepapat(pat->OLDPAT);
 else if (pat->CURPAT!=NULL) pat_frepapat(pat->CURPAT);
 /*les commentaires*/
 if (pat->DECCOM!=NULL) pat_frepacom(pat->DECCOM);
 /*les array*/
 if (pat->PAGRP!=NULL) pat_frepacom(pat->PAGRP);
 /*les declarations in/out*/
 /*if (pat->PAIOL!=NULL) pat_frepacom(pat->PAIOL);
on ne detruit pas les paiol(declarations in/out)  par faute de core dump sans doute a cause des noms des paiol qui sont reserves par namealloc() */
 /*detruire la structure header*/
 mbkfree(pat);
}
/*****************************************************************************/
/* compte le nombre de pattern d'un fichier PAT  et retourne...
  -1 ---> erreur de chargement du fichier pattern
 =>0 ---> nombre de patterns*/
int compte_pat(char *pat)
{
int i=0;
char c;
FILE *pat_in;
int not_com=2;                                             /*il faut 2 tirets*/

     pat=concat_mbk(pat,".pat");
     pat_in=fopen(pat,"r");
     if (ferror(pat_in)!=0) {
	  fprintf(stderr,"Unable to load %s\n",pat);
	  return -1;
     }
/*compter le nbr de patterns c'est compter le nombre de ':' */
     while ((c=fgetc(pat_in))!=EOF) {
	  if (c=='-') not_com--;
	  else if (not_com==1) not_com=2;
	  if (c=='\n') not_com=2;
	  if (c==':' && not_com) i++;
     }

     fclose(pat_in);
     mbkfree(pat);
return i;
}
/*****************************************************************************/
/*recoit en parametre un temps en millseconds et l'affiche avec un format    */
void affiche_time(unsigned long ms)
{
  if (ms<=1)                                   /*ordre de la millisecondes*/
       {fprintf(stdout,"one millisecond");ms=1;}
  else if (ms<1000)                            /*ordre de la millisecondes*/
       fprintf(stdout,"%ld milliseconds",ms);
  else if (ms<2000)                                  /*ordre de la seconde*/
       fprintf(stdout,"1.%03ld second",ms%1000);
  else if (ms<60000)                                 /*ordre de + secondes*/
       fprintf(stdout,"%ld.%03ld seconds",ms/1000,ms%1000);
  else if (ms<120000)                                 /*ordre de la minute*/
       fprintf(stdout,"1 minute %02ld",(ms/1000)%60);
  else if (ms<3600000)                                /*ordre de + minutes*/
       fprintf(stdout,"%ld minutes %02ld",ms/60000,(ms/1000)%60);
  else if (ms<7200000)                                  /*ordre de l'heure*/
       fprintf(stdout,"1 hour %02ld",(ms/60000)%60);
  else                                                    /*ordre de + heures*/
       fprintf(stdout,"%ld hours %02ld",ms/3600000,(ms/60000)%60);
fflush(stdout);
}
/****************************************************************************/
/* Visualisation de la structure de donnees */
int AffichePaseq(pat)
  struct paseq *pat;
{
  struct paiol *ppaiol;
  struct papat *ppapat;
  struct paevt *ppaevt;
  int i = 1;

  fprintf(stdout,"Paseq -> Paiol :\n");
  for(ppaiol = pat -> PAIOL; ppaiol; ppaiol = ppaiol -> NEXT) {
    fprintf(stdout,"\tPaiol -> name : %s\n", (char*)ppaiol -> NAME);
    fprintf(stdout,"\tPaiol -> formmat : %c\n", ppaiol -> FORMAT);
    fprintf(stdout,"\tPaiol -> mode : %c\n\n", ppaiol -> MODE);
    getchar();
  }
  fprintf(stdout,"Paseq -> iolnbr : %d\n", pat -> IOLNBR);
  fprintf(stdout,"Paseq -> Papat :\n");
  for(ppapat = pat -> CURPAT; ppapat; ppapat = ppapat -> NEXT) {
    fprintf(stdout,"\tNumero : %d\n", i++);
    fprintf(stdout,"\tPapat -> label : %s\n", (char*)ppapat -> LABEL);
    fprintf(stdout,"\tPapat -> simflag : %c\n", ppapat -> SIMFLAG);
    fprintf(stdout,"\tPapat -> Paevt\n");
    getchar();
    for(ppaevt = ppapat -> PAEVT; ppaevt; ppaevt = ppaevt -> NEXT) {
      fprintf(stdout,"\t\tPaevt -> index  : %d\n", ppaevt -> INDEX);
      fprintf(stdout,"\t\tPaevt -> usrval : %c\n", ppaevt -> USRVAL);
      fprintf(stdout,"\t\tPaevt -> simval : %c\n\n", ppaevt -> SIMVAL);
    }
    getchar();
    fprintf(stdout,"\n");
  }
  fprintf(stdout,"Paseq -> errflg : %d\n", pat -> ERRFLG);
  return(1);
}
/****************************************************************************/
/****************************************************************************/
/* allocation memoire */
static chain_list *mem=NULL;                           /*zones memoire */
static char *fin_mem=0;
static char *p_mem=0;
static chain_list *courant=NULL;
static unsigned int zone_mem=128;
/****************************************************************************/
void init_mem(unsigned int p)
{
p_mem=NULL;
fin_mem=NULL;
courant=NULL;
mem=NULL;
zone_mem=p*128;  /*longueur moyenne multiple de 4*/
}
/****************************************************************************/
char *mon_malloc(char *s)
{
int taille=0;
chain_list *sup=NULL;

if (s==NULL) return NULL;

taille=strlen(s)+1;
if (taille>LABEL_BS_SIZE+1) taille=LABEL_BS_SIZE+1;
 if (p_mem+taille>fin_mem) {
     p_mem=(char*) mbkalloc(zone_mem);  
     fin_mem=p_mem+zone_mem;
     sup=(chain_list*) mbkalloc(sizeof(chain_list));
     sup->NEXT=NULL;
     sup->DATA=(char*) p_mem;
     if (mem==NULL) mem=sup;
     else courant->NEXT=sup;
     courant=sup;
 }

if (p_mem+taille>fin_mem) {                /*si encore depassement alors...*/
      fprintf(stderr,"Out of Memory!!!\n");
      exit(1);
 }
 s=strncpy(p_mem,s,taille);        /*taille=30 caractères+ fin de string*/
 p_mem+=taille;

return s;
}
/****************************************************************************/
void erase_chain(chain_list *l)
{
if (l==NULL) return;
erase_chain(l->NEXT);
mbkfree(l->DATA);
mbkfree(l);
l=NULL;
}
void mon_free()
{
p_mem=NULL;
fin_mem=NULL;
courant=NULL;

if (mem==NULL) return;

erase_chain (mem);
mem=NULL;
}

/*****************************************************************************/
/******************************* END OF FILE *********************************/
/*****************************************************************************/

