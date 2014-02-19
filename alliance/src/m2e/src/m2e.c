/*  Procedures de base				11/02/95			*/


/*  10/03/95  Cree une liste des noms d'alim recontres dans /ALIMENTATIONS
	      si un connecteur de la rubrique /CONNECTEURS pointe sur un
	      signal present dans la liste des noms d'alim, on genere :
	      "PAL repere,broche" dans /ALIMENTATIONS.
	      Noms indexes : exemple "i(3)" devient "i 3" conforme a MBK
    14/03/95  Ajout d'un return 0 dans TraitConnect()
    31/03/95  Ajout warning si nom de connecteur absent du fichier .pin
    03/04/95  Detection alimentation utilisee et non declaree
	      Declaration alim en "plan" ou "piste" dans figure.pin et 
	      generation de PAL ou ALP selon le type d'alim
	      Detection des connecteurs nommes dans les .vst et absents des .pin
    05/05/95  Ajout verification valeur non nulle d'un pointeur avant usage, dans 
              GenPatteAlim() et SauvPinCon(). Message d'erreur si manque nom de
              pad ou nom de connecteur dans fichier ".pin".
    19/05/95  Lors de la generation des equipotentielles, comptage du nombre de
              connecteurs raccordes, si un seul, suppression de l'equipotentielle

    03/03/98  (Jean-Jacques) 
              Parcours de l'arbre M2D_PIN_LIB pour l'ouverture des fichiers .pin

    04/03/98  (Jean-Jacques)
              Parse de la ligne de commande

    14/04/02  Adaptation au logiciel Eagle
              Tout ce qui est a droite de # considere comme commentaires
	      Declaration des composants : ok
              Equipotentielles : ok
	      Noms de signaux, " " devient "_" 
    17/06/02  Pb pour les alim sur les connecteurs de la carte, un seul point 
              de connexion est listé, même s'il y en a plusieurs. Les alim sont 
              traitées comme les autres signaux.
    18/06/02  Pb corrige. A chaque connecteur est associé une liste des broches.
    26/06/02  Les signaux appartiennent par defaut a la classe 0. On peut affecter
              a un signal un autre classe en le mentionnant dans le fichier
              figure.cls. 
    30/06/02  Ajout dans figure.cls de la directive /PLACEMENT, obligatoire.
              permet de definir la rotation et les coordonnées de placement
	      de chaque instances.
	      Pour les connecteurs de la figure, la rotation et les coordonnée
	      sont definies dans figure.pin.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mut.h"
#include "mlo.h"
#include "m2e.h"

#define VMERROR 0
#define VMWARNING 1
#define VMINFO 2
#define VMTRACE 3

static chain_list *planhead = NULL;	/* tete de liste Plans  alimentations	*/
static chain_list *pisthead = NULL;	/* tete de liste Pistes alimentations	*/
static char       ct[]=" ,\t\n\0";
static char     coor[]="\t\n\0";
static	FILE      *pi,*pt,*po,*pa;

static char PathString[1024];
static int  PathStringC;
static char *PathStringV[32];

static char *LoFigName;
static char *OutputName;
static int  VerboseMode;
#define DEBUG  0	/* a 1 permet de conserver le fich temporaire	*/

/*----------------------------------------------------------------------
Parse de la ligne de commande
*/

int ParseCommand(int argc,char *argv[])
{
  int i;
  int failed;

  VerboseMode = VMWARNING;

  failed = 0;
  OutputName = NULL;

  if(VerboseMode>=VMTRACE) 
    fprintf(stderr,"[m2e] Tracing : Parsing Command Line\n");
  
  if(argc <2) 
  { 
    failed=1;
    if (VerboseMode>=VMERROR) 
      fprintf(stderr,"[m2e] Error : Too Few Arguments \n");
  }

  if(!failed)
  {
    for(i=1;(i<argc)&&(!failed);i++)
     if(argv[i][0]=='-')
     switch(argv[i][1])
     {
       case 'v':
	 if (argc <= i + 1)
		 break;
         i++;
         sscanf(argv[i],"%11d",&VerboseMode);
         if(VerboseMode>=VMINFO) 
           fprintf(stderr,"[m2e] Info : Verbose Level Set to %d \n",VerboseMode);
         break;

       case 'o':
	 if (argc <= i + 1)
		 break;
         i++;
         OutputName=argv[i];
         if(VerboseMode>=VMINFO) 
           fprintf(stderr,"[m2e] Info : Output File set to %s \n",OutputName);
         break;

       default : 
         failed = 1;
         if(VerboseMode>=VMERROR) 
           fprintf(stderr,"[m2e] Error : Invalid Option -%c\n",argv[i][1]);
         break;
     }
     else
     if (!LoFigName&&!failed)
     {
        LoFigName = argv[i];
        if(VerboseMode>=VMINFO) 
          fprintf(stderr,"[m2e] Info : Input File set to %s \n",LoFigName);
     }
     else 
     {
       failed=1; 
       if(VerboseMode>=VMERROR) 
         fprintf(stderr,"[m2e] Error : Too Many Arguments\n");
     }
  }

  if(failed) 
  {
    fprintf(stderr,"Usage :\n");
    fprintf(stderr,"m2e [-o OutputFile] [-v VerboseLevel] InputFile \n");
    return 1;
  }  
  return 0;
}

/*----------------------------------------------------------------------
Renvoie dans pbuf la prochaine ligne non vide
Elimine les lignes commencant par #
Code retour non nul si fin de fichier
*/
int LigneSuiv(char *pbuf)
{
  char	*p,*f;
  int		n;

  while(1) 
  {
    if((f = fgets(pbuf,MAXL,pi)) != NULL)
    {
      n = strlen(pbuf);
      p = pbuf;
      if(*p == '#') continue ;
      while(*p == ' ') 
      { 
        p++;  n--; 
      }
      if(n != 0) return 0;
    }
    else
      return 1;
  }
}
/*----------------------------------------------------------------------
Renvoie dans pbuf la prochaine ligne commencant par "/" en col 1
Code retour non nul si fin de fichier
*/
int LigneSlash(char *pbuf)
{
  char *f;

  while(1) 
  {
    if((f = fgets(pbuf,MAXL,pi)) != NULL) 
    {
      if(*pbuf == '/') return 0;
    }
    else
      return 1;
  }
}
/*----------------------------------------------------------------------
Renvoie 1 si le nom  appartient  a la liste des noms
d'alimentation 0 dans le cas contraire
*/
int InListAlim(char *pn,chain_list	*pch)
{
  chain_list *pchain;

  pchain = pch;
  while(pchain) 
  {
    if(pn == pchain->DATA)  return 1;
    pchain = pchain->NEXT;
  }
  return 0;
}
/*----------------------------------------------------------------------
Recoit  : Rien
Renvoie : -0 si M2D_PIN_LIB est present,
          -1 si M2D_PIN_LIB est absent.
*/
int LoadPinPath(void)
{
  char *Source;
  int i,j;

  if(!(Source = getenv("M2E_PIN_LIB")))
  {
    if(VerboseMode>=VMERROR)
      fprintf(stderr,"[m2e] Error : M2E_PIN_LIB undefined\n");
    return 1;
  }

  strcpy(PathString,Source);

  PathStringV[0]=Source;

  i=0; j=1;
  for(i=0;Source[i];i++)
  {
    if(Source[i]==':')
    {
      Source[i]='\0';
      PathStringV[j]=&Source[i+1];
      j++;
    } 
  } 

  PathStringC = j;

  
  if(VerboseMode>=VMTRACE)
    for(j=0;j<PathStringC;j++)
      fprintf(stderr,"[m2e] Trace : Adding %s as Pin Library\n",PathStringV[j]);
  
  return 0;

}

/*----------------------------------------------------------------------
Recoit  : un pointeur sur un "nom" (chaine C), Ajoute ".pin au nom recu
          et ouvre ce fichier
Renvoie : -  0 si fichier present
          -  1 si fichier absent
*/
int GetPtrFichPin(char * pnom)
{
  char	buf[128];
  int   j;

  for(j=0;j<PathStringC;j++)
  {
    sprintf(buf,"%s/%s.pin",PathStringV[j],pnom);
    if((pi = fopen(buf,"r")))
    {
      if(VerboseMode>=VMINFO)
        fprintf(stderr,"[m2e] Info : Loading %s\n",buf);
      return 0;
    }
  }
  if(VerboseMode>=VMERROR)
    fprintf(stderr,"[m2e] Error : %s.pin not found\n",pnom);
  return 1;	
}
/*----------------------------------------------------------------------
Traite une ligne /MODELE du fichier "figure.pin"
Recoit  : - un pointeur sur la ligne lue commencant par "/"
          - un pointeur sur une zone resultat
Renvoie : 0  si correct, avec le nom de repere dans la zone resultat
          -1 si pas ligne /MODELE
*/
int GenRepFig(char *pbuf,char *prep)
{
  char  *p;

  if((p = strtok(pbuf,&ct[0]))) 
  {
    if(strcmp(MODEL_PIN,p) == 0) 
    {		/* chaine reconnue	*/
      if((p = strtok(NULL,&ct[0]))) 
      {
        fprintf(po,"ADD '%s'",p) ;
        strcpy(prep,p);
      }
      if((p = strtok(NULL,&ct[0]))) fprintf(po,"  %s",p) ;
      if((p = strtok(NULL,&coor[0]))) fprintf(po,"  %s",p) ;
      if((p = strtok(NULL,&coor[0]))) fprintf(po,"  %s",p) ;
      fprintf(po,";\n");
      return 0;
    }
  }
  return 1;
} 
/*----------------------------------------------------------------------
Traite une ligne /PLAN_ALIM ou /PISTE_ALIM du fichier "figure.pin"
Recoit  : - un pointeur sur un tampon
Renvoie : - 0 si correct, -1 si pas de ligne /xxx_ALIM
*/
int GenAlimFig(char *pbuf)
{
  char  *p,*pn;

  if(*pbuf == '/') 
  {
    if((p = strtok(pbuf,&ct[0]))) 
    {
      if((strcmp(PL_ALIM_PIN,p)) == 0) 
      {		/* chaine reconnue	*/
        fprintf(pt,"/ALIMENTATIONS ") ;
        while((p = strtok(NULL,&ct[0]))) 
	{
          pn = namealloc(p);
          if(InListAlim(pn,planhead) == 0)
            planhead = addchain(planhead,pn);
          fprintf(pt," %s",p) ;
          if((InListAlim(pn,pisthead) != 0)) 
	  {
            if(VerboseMode>=VMWARNING)
              fprintf(stderr,"[m2e] Warning : Power %s declared as plane and track\n",pn);
          }
        }
      }
      else if(strcmp(PS_ALIM_PIN,p) == 0) 
      { /* chaine reconnue	*/
        fprintf(pt,"/ALIMENTATIONS ") ;
        while((p = strtok(NULL,&ct[0]))) 
	{
          pn = namealloc(p);
          if(InListAlim(pn,pisthead) == 0)
            pisthead = addchain(pisthead,pn);
          fprintf(pt," %s",p) ;
          if(InListAlim(pn,planhead) != 0) 
	  {
            if(VerboseMode>=VMWARNING)
              fprintf(stderr,"[m2e] Warning : Power %s declared as plane and track\n",pn);
          }
        }
      }
      else 
        return 1;	
      fprintf(pt,"\n");
      return 0;
    }
  }
  return 1;
}
/*----------------------------------------------------------------------
Traite une ligne /ALIM  des fichiers "instance.pin"
Recoit  : - un pointeur sur un tampon
Renvoie : - 0 si correct, - 1 si pas de ligne /xxx_ALIM
*/
int GenAlimInst(char *pbuf)
{
  char  *p,*pn;

  if(*pbuf == '/') 
  {
    if((p = strtok(pbuf,&ct[0]))) 
    {
      if(strcmp(ALIM_PIN,p) == 0) 
      {		/* chaine reconnue	*/
        fprintf(pt,"/ALIMENTATIONS") ;
        while((p = strtok(NULL,&ct[0]))) 
        {
          pn = namealloc(p);
          if(InListAlim(pn,planhead) == 0)
            if(InListAlim(pn,pisthead) == 0) 
	    {
              if(VerboseMode>=VMWARNING)
                 fprintf(stderr,"[m2e] Warning : Power %s not declared in figure.pin\n",pn);
            }
          fprintf(pt," %s",p) ;
        }
        fprintf(pt,"\n");
        return 0;
      }
    }
  }
  return 1;
}
/*----------------------------------------------------------------------
Traite chaque ligne de la rubrique /ALIM des fichiers "xxx.pin"
Recoit  : - un pointeur sur un tampon
          - une constante indiquant figure ou instance
*/
void GenPatteAlim(char *pbuf,char *prep,char instance)
{
  char  *p,*pb,*pn;

  do 
  {
    if(*pbuf != '/')
    {
      p = strtok(pbuf,&ct[0]) ;
      if((p != NULL) && (*pbuf != '\n'))
      {
        pb = p ;
/*fprintf(stderr,"pbuf :%s ** ",pbuf);*/
/*fprintf(stderr,"PAL  %s,%s=",prep,p) ;*/		/* repere,nom de broche	*/
        if(instance)  p = strtok(NULL,&ct[0]);	/* saute le nom de pad	*/
        if((p = strtok(NULL,&ct[0])) == NULL)
        {
          if(VerboseMode>=VMERROR)
            fprintf(stderr,"[m2e] Error : Pad or Connector name missing (%s,%s)\n",pbuf,p); 
          return ;
        }
        pn = namealloc(p);
        if(InListAlim(pn,planhead))
	  fprintf(pt,"PAL  %s,%s=",prep,pb) ;	/* repere,nom de broche	*/
        else if(InListAlim(pn,pisthead))
          fprintf(pt,"ALP  %s,%s=",prep,pb) ;	/* repere,nom de broche	*/
        else 
        {
          if(VerboseMode>=VMWARNING)
            fprintf(stderr,"[m2e] Warning : Power %s not declared\n",p);
	}
        fprintf(pt,"%s\n",p) ;	/* nom de l'alim		*/
        if(VerboseMode>=VMTRACE)
          fprintf(stderr,"[m2e] Trace : Power %s\n",p) ;
      }
    } 
    else
      return ;
  } while(LigneSuiv(pbuf) == 0) ;
}
/*----------------------------------------------------------------------
Renvoie 1 si le nom du signal n'appartient pas a la liste des noms
d'alimentation 0 dans le cas contraire
*/
int NotAlimName(losig_list *ps)
{
  chain_list *pchain;

  pchain = planhead;
  while(pchain) 
  {
    if(ps->NAMECHAIN->DATA == pchain->DATA)  return 0;
    pchain = pchain->NEXT;
  }
  pchain = pisthead;
  while(pchain) 
  {
    if(ps->NAMECHAIN->DATA == pchain->DATA)  return 0;
    pchain = pchain->NEXT;
  }
  return 1;
}
/*----------------------------------------------------------------------
Recoit : pcon un pointeur sur un connecteur
         pbuf un pointeur sur la ligne courante du fichier
         pb   un pointeur sur fin de zone contenant "repere," pour insertion
         pd   un pointeur sur debut de zone contenant "repere,"
Supprime les parentheses des signaux indexes, met un blanc
Insere "'repere' 'broche'" 
Insere un element ptype pointe par le champ USER du connecteur ; Insere "'repere' 'broche'" 
dans une liste chainee à l'element ptype
*/
void SauvPinCon(locon_list *pcon,char *pbuf,char *pb,char *pd)
{
  locon_list *pc;
  ptype_list *pt;
  char       *pname,*p,*pn;

  if((p = strtok(pbuf,&ct[0])) && (*pbuf != '\n')) 
  {
    strcpy(pb,p);
/*fprintf(stderr,"sauvPinCon : %s\n",pb);*/
    pn = pb + strlen(p);
    *pn++ = '\'';
    *pn = '\0';
/*    ppad = strtok(NULL,&ct[0]);*/
    if((p = strtok(NULL,&ct[0])) == NULL)
    {
/*fprintf(stderr,"sauvPinCon : %s\n",pb);*/
      if(VerboseMode>=VMERROR) 
	fprintf(stderr,"[m2e] Error : Pad or Connector name missing (%s)\n",pbuf);
      return ;
    }
    if((pn = strchr(p,'('))) 
    {
      *pn = ' ';
      if((pn = strchr(pn,')')))
        *pn = '\0';
      else
      {
        if(VerboseMode>=VMERROR)
          fprintf(stderr,"[m2e] Error : Bad Connector Name (%s) \n",p);
      }
    }
/*fprintf(stderr,"sauvPinCon: p : %s\n",p);*/
    pname = namealloc(p);
    pc = pcon;
    while(pc) 
    {
      if(pname == pc->NAME)
      {
        if(VerboseMode>=VMTRACE)
          fprintf(stderr,"[m2e] Trace : Connector %s, %s\n",pc->NAME,pd);
        pn = namealloc(pd);
	if((pt = getptype(pc->USER,(long)DEDALE)) == NULL) 
	{ /* pas encore de broche associee     */
	  pc->USER = addptype(NULL,(long)DEDALE,(void*)NULL);
          pc->USER->DATA = addchain(NULL,pn);
	}
	else /* deja au moins une broche associee     */
          pt->DATA = addchain(pt->DATA,pn);
        return ;
      }
      pc = pc->NEXT;
    }
    if(VerboseMode>=VMTRACE)
      fprintf(stderr,"[m2e] Trace : Connector not found, %s\n",pname);
  }
  return ;
}
/*----------------------------------------------------------------------
Traitement de la rubrique /CONNECTEURS
Recoit : - ptr sur liste des connecteurs de l'instance
         - ptr sur la ligne contenant /CONNECTEURS 
         - ptr sur le nom d'instance
Pour chaque connecteur rencontre, ajoute "repere patte" dans le champ USER

*/
int TraitConnect(locon_list *pcon,char *pbuf,char *prep)
{
  char	b[64],*p,*pb;
/*  locon_list *pc=pcon;
  
  while(pc != NULL)
  {
     pc->USER = NULL;
     pc = pc->NEXT;
   }*/
  if((p = strtok(pbuf,&ct[0]))) 
  {
    if(strcmp(CONN_PIN,p) == 0) 
    {
      b[0] = '\'';
      strcpy(&b[1],prep);
      pb = &b[1] + strlen(prep);
      *pb++ = '\'';
      *pb++ = ' ';
      *pb++ = '\'';
      while(LigneSuiv(pbuf) == 0) 
      {
/*fprintf(stderr,"%s\n",pbuf);*/
        if(*pbuf != '/') 
          SauvPinCon(pcon,pbuf,pb,&b[0]);
        else
          return 0;
      }
      return 0;
    }
  }
  return 1;	
}
/*----------------------------------------------------------------------
*/
int ChercheSignal(losig_list *psig,char *pname)
{
  chain_list	*pc;

  pc = psig->NAMECHAIN;
  do {
    if((char *)pc->DATA == pname)
    {
      return 0;  /* signal trouvé */
    }
  } while((pc = pc->NEXT) != 0);
  return 1;  /* signal non trouvé */
}
/*----------------------------------------------------------------------
*/
loins_list* ChercheInst(loins_list *pins,char *pname)
{
  while(pins != NULL) 
  {
    if(pins->INSNAME == pname)
    {
      return pins;  /* instance trouvé */
    }
  pins = pins->NEXT;
  }
  return NULL;  /* instance non trouvé */
}
/*----------------------------------------------------------------------
Traitement du fichier "figure.cls"
Pour chacun des signaux du fichier, ajoute au champ USER de losig,
un element ptype contenant le numero de class dans le champ DATA
*/
int TraitFichClass(lofig_list *ptfig)
{
  ptype_list    *pt;
  losig_list 	*psig=0;
  loins_list    *pins;
  int           trouve;
  char	buf[128],*ps,*pcl,*pname,*pi,*pr,*pc;
    
  if(LigneSlash(&buf[0]) != 0)	return 1;

  if((ps = strtok(buf,&ct[0])) == 0) 	return 1;
  if(strcmp(CLASSE,buf) != 0) 	return 1;
  while(LigneSuiv(buf) == 0) 
  {
    if(buf[0] == '/')  break ; /* nouvelle directive */
    if((ps = strtok(buf,&ct[0])) == 0)  continue ; /* ligne vide */
    if((pcl = strtok(NULL,&ct[0])) == 0)  return 1;
    if(VerboseMode>=VMTRACE)
      fprintf(stderr,"[m2e] Trace : signal : %s, class : %s\n",ps,pcl);
    pname = namealloc(ps);
    psig = ptfig->LOSIG;
    trouve = 0;
    while(psig)
    {
      if(ChercheSignal(psig,pname) == 0)
      {  /* signal trouve */
	if((pt = getptype(psig->USER,(long)DEDALE)) == NULL) 
	{ /* pas encore de class associee     */
	  psig->USER = addptype(psig->USER,(long)DEDALE,(void*)NULL);
	  pt = psig->USER;
	}
        sscanf(pcl,"%11d",(int*)&(pt->DATA));
/*fprintf(stderr,"class %d\n",(int)pt->DATA);*/
        trouve = 1;
        break ;
      }
      psig = psig->NEXT;
    }
  if(!trouve && (VerboseMode>=VMINFO))
    fprintf(stderr,"[m2e] Info : Signal %s, absent\n",ps);
  }
/* exploite /PLACEMENT    */
  if((strtok(buf,&ct[0])) == 0) 	return 1;
  if(strcmp(PLACEMENT,buf) != 0) 	return 1;
  while(LigneSuiv(buf) == 0) 
  {
    if((pi = strtok(buf,&ct[0])) == 0)  continue ; /* ligne vide */
    if((pr = strtok(NULL,&ct[0])) == 0)  return 2;
    if((pc = strtok(NULL,&coor[0])) == 0)  return 3;
    if(VerboseMode>=VMTRACE)
      fprintf(stderr,"[m2e] Trace : instance : %s, Rotation : %s, coord : %s\n",
	      pi,pr,pc);
    *(pc-1) = ' ';  /*retablit l'espace entre rt et coord   */
    pname = namealloc(pi);
    if((pins = ChercheInst(ptfig->LOINS,pname)) != NULL)
    {  /* instance trouvee */
      if((pt = getptype(pins->USER,(long)DEDALE)) == NULL) 
      { /* pas encore de coord associee     */
        pins->USER = addptype(pins->USER,(long)DEDALE,(void*)NULL);
        pt = pins->USER;
      }
      pt->DATA = namealloc(pr);
/*fprintf(stderr,"rot coo : %s\n",(char*)(pins->USER->DATA));*/
    }
    else
      if(VerboseMode>=VMINFO)
        fprintf(stderr,"[m2e] Info : Instance %s, absente\n",pi);
  }
  return  0;	
}
/*----------------------------------------------------------------------
Traitement du fichier "figure.pin"
*/
int TraitFichFig(lofig_list *ptfig)
{
  char	rep[32];
  char	buf[128];

  if(ptfig->NAME == NULL)
  {
    if(VerboseMode>=VMERROR)
      fprintf(stderr,"[m2e] Error :  Figure Name Missing\n");
    return (1);
  }
  if(GetPtrFichPin(ptfig->NAME)) return (1);
  fprintf(po,"###  COMPOSANTS\n\n") ;
  buf[0] = 0;
  if(LigneSlash(&buf[0]) != 0)	return 1;
  do 
  {
    if(GenRepFig(&buf[0],&rep[0]))
    {
      if(VerboseMode>=VMERROR)
        fprintf(stderr,"[m2e] Error :  /MODELE missing in file %s.pin\n",ptfig->NAME);
      fclose(pi);
      fclose(pt);
      return (1);
    }
    if(LigneSlash(&buf[0]) != 0) return 1;
    if(TraitConnect(ptfig->LOCON,&buf[0],&rep[0]))
    {
      if(VerboseMode>=VMERROR)
        fprintf(stderr,"[m2e] Error : /CONNECTEURS missing in %s.pin\n",ptfig->NAME);
      fclose(pi);
      fclose(pt);
      return (1);
    }
  } 
  while(buf[0] == '/');
  fclose(pi);
  return (0);
}
/*----------------------------------------------------------------------
Traite la ligne /MODELE du fichier "instance.pin"
Genere : "nom d'instance", MG et Label
Recoit  : - un pointeur sur la ligne courante du fichier "instance.pin"
          - un pointeur sur le nom de l'instance
Renvoie : - 0 si correct, -1 si anomalie
*/
int GenRepIns(char *pbuf,loins_list *ptins)
{
  ptype_list    *pt;
  char	*p;
  
  if((p = strtok(pbuf,&ct[0])) == 0)  return 1;
  if(strcmp(MODEL_PIN,p) != 0)  return 1;
  fprintf(po,"ADD '%s'", ptins->INSNAME) ;
/*  fprintf(stderr,"%s\n", ptins->INSNAME) ;*/
  if((p = strtok(NULL,&ct[0])) == 0)  return 2;
  fprintf(po,"  %s", p) ;
  if((pt = getptype(ptins->USER,(long)DEDALE)) == NULL)  return 3;
  if(pt->DATA == NULL)  return 4;
  fprintf(po,"  %s;\n",(char *)pt->DATA) ;
  return 0;
}
/*----------------------------------------------------------------------
Traitement d'un fichier "instance.pin"
*/
int TraitFichInst(loins_list *ptins)
{
  char   buf[128];
  int    err;

  if(GetPtrFichPin(ptins->FIGNAME))	  return 1;
  if(VerboseMode>=VMTRACE)
    fprintf(stderr,"[m2e] Trace : Instance %s\n",ptins->INSNAME);
  buf[0] = 0;
  if(LigneSlash(&buf[0]) != 0)	return 1 ;
  if((err = GenRepIns(&buf[0],ptins)) != 0)
  {
    if(VerboseMode>=VMERROR)
    {
      switch(err)
      {
        case 1:
         fprintf(stderr,"[m2e] Warning : /MODELE missing in %s.pin\n",ptins->FIGNAME);
	 break;
        case 2:
         fprintf(stderr,"[m2e] Warning : lib missing in %s.pin\n",ptins->FIGNAME);
	 break;
        case 3:
         fprintf(stderr,"[m2e] Warning : rotation or coordinate missing in %s.pin\n",ptins->FIGNAME);
	 break;
        case 4:
         fprintf(stderr,"[m2e] Warning : coordinate missing in %s.pin\n",ptins->FIGNAME);
	 break;
      }
    }
  }
  if(LigneSlash(&buf[0]) != 0)	return 1 ;
  if(TraitConnect(ptins->LOCON,&buf[0],ptins->INSNAME))
  {
    if(VerboseMode>=VMERROR)
      fprintf(stderr,"[m2e] /CONNECTEURS missing in %s.pin\n",ptins->INSNAME);
    fclose(pi);
    fclose(pt);
    return (1);
  }
  
  if(VerboseMode>=VMTRACE)
    fprintf(stderr,"[m2e] Trace : Connectors Completed \n");

  fclose(pi);
  return (0);
}
/*----------------------------------------------------------------------
Parcoure la liste des instances
*/
int TraitListInst(lofig_list *ptfig)
{
  loins_list	*ptins;

  ptins = ptfig->LOINS;
  while(ptins)
  {
    if(TraitFichInst(ptins)) return 1;
    ptins = ptins->NEXT;
  }
  fprintf(po,"\n");
  return 0;
}
/*----------------------------------------------------------------------
Concatene le fichier temporaire au fichier de sortie
Ferme le fichier temporaire
*/
void ConcatFich()
{
  char	tmp[SIZE_BUF];
  size_t	n;

  fflush(pt);
#if DEBUG
  fclose(pt);
  pt = fopen("temp","r");
#else
  rewind(pt);
#endif

  while(feof(pt) == 0)
  {
    n = fread(&tmp[0],1,SIZE_BUF,pt);
    fwrite(&tmp[0],1,n,po);
  }	
  fclose(pt);
}
/*----------------------------------------------------------------------
Liste les alias des signaux
*/
void ListSigAlias(losig_list *psig)
{
  chain_list	*pc;
  short		i;

  if(pa == NULL)  return;
  i = 0 ;
  fprintf(pa,"<%s> : ",(char *)psig->NAMECHAIN->DATA) ;
  pc = psig->NAMECHAIN;
  while((pc = pc->NEXT))
  {
    fprintf(pa,"<%s> ",(char *)pc->DATA) ;
    i = i + 1 ;
    if(i == 4 )
    {
      i = 0;
      fprintf(pa,"\n<%s> : ",(char *)psig->NAMECHAIN->DATA) ;
    } 
  }
  fprintf(pa,"\n");
}
/*----------------------------------------------------------------------
Parcours la liste des signaux et genere la commande SIGNAL.
Pour chaque signal, parcoure la liste des connecteurs attaches à ce signal,
pour chaque connecteur, parcoure la liste des broches attaches à ce connecteur.
Genere le texte dans un buffer. Le buffer n'est recopie dans le fichier
de sortie que si au moins deux broches sont attachees au signal.
Gere la classe des signaux.
----------------------------------------------------------------------*/
void TraitSignaux(lofig_list *ptfig)
{
  ptype_list    *pt;
  losig_list 	*psig;
  chain_list	*pchcon,*pchbro;
  locon_list	*pcon=NULL;
  loins_list	*pins=NULL;
  lofig_list	*pfig;
  int           class=0;
  short		n,nbc,nomsig,j,k;
  char          *p,tmpsig[4096];

/*pa = fopen("alias","w");*/
  fprintf(po,"###  EQUIPOTENTIELLES\n\n") ;
  fprintf(po,"Change Class 0;\n") ;
  psig = ptfig->LOSIG;
  while(psig != NULL)
  { /* pour chaque signal   */
/*	ListSigAlias(psig) ;*/
/*fprintf(stderr,"[m2e] Signal : %s\n",(char *)psig->NAMECHAIN->DATA) ;*/
    if((p = strchr((char *)psig->NAMECHAIN->DATA,' ')) != 0) 
      *p = '_'; /* remplace l'espace par _ dans les signaux indices  */
    if((pt = getptype(psig->USER,(long)DEDALE)) != NULL)
    {
       if((long)pt->DATA != class) {
           fprintf(po,"Change Class %ld;\n",(long)pt->DATA) ;
           class = (long)pt->DATA;
       }
    }
    else
    {
       if(class != 0) {
           fprintf(po,"Change Class 0;\n") ;
           class = 0;
       }
    }
	
    if((pt = getptype(psig->USER,(long)LOFIGCHAIN)) == NULL) {
      fprintf(stderr,"[m2e] pas de connecteur associe au signal : %s\n",
              (char *)psig->NAMECHAIN->DATA) ;
      return;
      }
    pchcon = pt->DATA;
    n = 0;
    k = 0;
    nbc = 0;
    nomsig = 0;
    pt = NULL;
    while(pchcon != NULL)
    { /* pour chaque connecteur   */
      if(nomsig == 0) {
	nomsig = 1;
	k = sprintf(tmpsig,"Signal '%s'",(char *)psig->NAMECHAIN->DATA);
      }
      if((pcon = (locon_list*)(pchcon->DATA)) == NULL)
      {
        fprintf(stderr,"[m2e] Warning : no connector\n");
	break;
      }
/*fprintf(stderr,"[m2e] Connecteur : %s\n",pcon->NAME) ;*/
      if((pt = getptype(pcon->USER,(long)DEDALE)) == NULL) 
      {
          if(pcon->TYPE == 'I')
          {
            pins = (loins_list*)pcon->ROOT ;
            if(VerboseMode>=VMWARNING)
            {
               fprintf(stderr,"[m2e] Warning : Instance <%s>, connector <%s> ",pins->INSNAME,pcon->NAME);
               fprintf(stderr,"missing in %s.pin\n",pins->FIGNAME);
            }
          }
          else
          {
            pfig = (lofig_list*)pcon->ROOT ;
            if(VerboseMode>=VMWARNING)
            {
              fprintf(stderr,"[m2e] Warning : Figure <%s>, connector <%s> ",pfig->NAME,pcon->NAME);
              fprintf(stderr,"missing in %s.pin\n",pfig->NAME);
             }
          }
	break;
      }
      pchbro = (chain_list*)(pt->DATA);
      while(pchbro != NULL)
      {  /* pour chaque broche   */
        if(n >= MAXREP)
	{
          j = sprintf(&tmpsig[k],"\\\n");
          k = k + j;
	  n = 0;
	}
	if((n == 0)&&(nbc != 0)) {
	  j = sprintf(&tmpsig[k],"            ") ;
          k = k + j;
	}
	if(pchbro->DATA == NULL) {
          fprintf(stderr,"[m2e] Warning : Instance <%s>, connector <%s> no repere\n",pins->INSNAME,pcon->NAME);
	  break;
	}
        j = sprintf(&tmpsig[k],"  %s",(char *)pchbro->DATA);
        k = k + j;
        pchbro = pchbro->NEXT;
	n++;
        nbc++;
      } /* fin pour chaque broche   */
      pchcon = pchcon->NEXT;
   } /* fin pour chaque connecteur   */
   if(nbc == 0) /* pas de broche associe au signal 
      {
          if(pcon->TYPE == 'I')
          {
            pins = (loins_list*)pcon->ROOT ;
            if(VerboseMode>=VMWARNING)
            {
               fprintf(stderr,"[m2e] Warning : Instance <%s>, connector <%s> ",pins->INSNAME,pcon->NAME);
               fprintf(stderr,"missing in %s.pin\n",pins->FIGNAME);
            }
          }
          else
          {
            pfig = (lofig_list*)pcon->ROOT ;
            if(VerboseMode>=VMWARNING)
            {
              fprintf(stderr,"[m2e] Warning : Figure <%s>, connector <%s>\n",pfig->NAME,pcon->NAME);
              fprintf(stderr,"[m2e]            missing in %s.pin\n",pfig->NAME);
             }
          }
      }  * end if*/
  if(VerboseMode>=VMTRACE)
    fprintf(stderr,"[m2e] Trace : signal : %s, nombre de broches : %d\n",(char *)psig->NAMECHAIN->DATA,nbc);        
   if(nbc != 0)  
   { /* termine la ligne de texte  */
     j = sprintf(&tmpsig[k],";\n");  
     k = k + j;
   }
   if(nbc > 1)  /* si plus d'une broche, recopie la ligne */
     fwrite(tmpsig,1,k,po);
   psig = psig->NEXT;
  } /* fin pour chaque signal   */
  fclose(po);
}
/*----------------------------------------------------------------------

*/
int main(int argc,char *argv[])
/*int main()*/
{
  lofig_list	*ptfig;
  time_t        tp;
  int           err;
  char          *tim,buf[128];

  mbkenv();
  alliancebanner("m2e","1.1","Schematic Entry to Eagle PCB Router 4.08","2th July 2002","1.10");

  if (ParseCommand(argc,argv)) return 1;

  if (LoadPinPath()) return 1;

  ptfig = getlofig(LoFigName,'A');

  lofigchain(ptfig);

  if ((po = fopen((OutputName?OutputName:ptfig->NAME),"w")) == NULL) 
  {
    if(VerboseMode>=VMERROR) 
      fprintf(stderr,"[m2e] Error : Can't open output file %s\n",
             (OutputName?OutputName:ptfig->NAME));
    return 1;
  }
  
  sprintf(buf,"%s.cls",LoFigName);
  if((pi = fopen(buf,"r")) > 0) 
  {
    if(VerboseMode>=VMINFO) 
      fprintf(stderr,"[m2e] Info : Parsing Class File %s\n",buf);
    if((err = TraitFichClass(ptfig)) != 0) 
    {
      fprintf(stderr,"[m2e] Info : TraitFichClass error : %d\n",err);
    }
    fclose(pi);
  }
  else {
    if(VerboseMode>=VMERROR)
      fprintf(stderr,"[m2e] Info : missing file <%s>\n",buf);
      exit(1);
  }

#if DEBUG
  if((pt = fopen("temp","w")) == NULL) 
  {
#else
  if((pt = tmpfile()) == NULL) 
  {
#endif
    if(VerboseMode>=VMERROR) 
      fprintf(stderr,"[m2e] Error : Can't open temporary file\n");
    return 1;
  }
  if((tp = time(NULL)) < 0) 
      fprintf(stderr,"[m2e] Error : Can't get time\n");
  tim = ctime(&tp);
  fprintf(po,"#  Fichier genere par m2e, pour %s,  %s\n",getenv("USER"),tim);

  if(VerboseMode>=VMINFO) 
    fprintf(stderr,"[m2e] Info : Parsing Figure File \n");
  if(TraitFichFig(ptfig))  return 1;

  if(VerboseMode>=VMINFO) 
    fprintf(stderr,"[m2e] Info : Parsing Instance Files \n");

  if(TraitListInst(ptfig)) return 1;

  ConcatFich();

  if(VerboseMode>=VMINFO) 
    fprintf(stderr,"[m2e] Info : Parsing Signals \n");
  TraitSignaux(ptfig);
return 0;
}
