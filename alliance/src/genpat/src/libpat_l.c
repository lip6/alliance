/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
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

/***************************************************************/
/* module   : libpat_l.c                                       */
/* date     : 28/07/93                                         */
/* auteurs  : RENAUD & VENOT & PITON                           */
/***************************************************************/

#ident "$Id: libpat_l.c,v 1.1 2002/05/30 13:28:15 xtof Exp $"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <sys/signal.h>

#include "mut.h"
#include "pat.h"
#include "libpat_l.h"

/*------------------------------------------------------------
 *      Numero de ligne dans le fichier source
 *------------------------------------------------------------*/
unsigned int GNP_LINE = 1;

/*------------------------------------------------------------
 *      STRUCTURE PAT
 *------------------------------------------------------------*/
struct  paseq  *racine_paseq;

/*------------------------------------------------------------
 *      LABEL 
 *------------------------------------------------------------*/
unsigned int    MAX_LABEL = 7;

/*------------------------------------------------------------
 *      NUMERO DE PATTERN COURANT
 *------------------------------------------------------------*/
unsigned long   GNP_PATTERN_COURANT;

/*------------------------------------------------------------
 *      TABLE DES PAIOLs
 *------------------------------------------------------------*/
struct paiol *tab_paiol[GNP_MAXTAB_PAIOL];
int      GNP_PAIOL_INDEX=0;
int      GNP_TAB_PAIOL_CREE;

/*------------------------------------------------------------
 *      TABLE DES IDENTIFICATEURS
 *------------------------------------------------------------*/
Tptab_ident tab_ident[GNP_MAXTAB_IDENT];
int     ind_tab_ident;

/*------------------------------------------------------------
 *      GESTION DES PARAMETRES A NOMBRE INDETERMINE
 *------------------------------------------------------------*/
char    *pile_param[GNP_MAXPILE_PARAM];
int      sp_pile_param ;
int      sp_file_param ;

/*-------------------------------------------------------------------*/
/*      TABLES DE HACHAGE                                            */
/*-------------------------------------------------------------------*/
elt    *hash_paiol[GNP_HASHSIZE];
elt    *hash_pagrp[GNP_HASHSIZE];

/* ###--------------------------------------------------------------### */
/* function     : Erreur						*/
/* description  : print error message & exit 				*/
/* called func. : none							*/
/* ###--------------------------------------------------------------### */
 
void Erreur (chaine)
char *chaine;
{
  fprintf (stderr, "GENPAT : line %d : %s\n", GNP_LINE, chaine);

#ifdef DEBUG
  EXIT(1);
#else
  exit(1);
#endif

}

/*-------------------------------------------------------------------*/
static int Hash(nom)
char *nom;
{
int code = 0;
char *p;
  p = nom;
  while (*p) 
     code = (code << 1) ^ *p++;
  if ( code < 0)
     code =- code;
  code %= GNP_HASHSIZE;
  return( code);
}
/*-------------------------------------------------------------------*/
void InitHash(tab)
elt *tab[];
{
int i;
  for (i=0;i< GNP_HASHSIZE ; i++)
    tab[i] = NULL ;
}
/*-------------------------------------------------------------------
 * retourne le pointeur sur la structure ELT
 * le champ name est mis a jour
 * le champs num_index n'est pas mis  a jour
 * il vaut -1
 */
/*-------------------------------------------------------------------*/
elt *SymbAlloc(ident,tab)
char *ident;
elt  *tab[] ;
{
int num_case;
elt *ptr_aux;
char *allocname;
   allocname = (char *)namealloc (ident);
   num_case = Hash(allocname);
   ptr_aux = tab[num_case];
   while (ptr_aux != NULL) 
      if ( strcmp( ptr_aux->name , allocname ) == 0 )
          return(ptr_aux);
      else
          ptr_aux = ptr_aux->next;
   ptr_aux = (elt*) mbkalloc ( sizeof (elt) );
   ptr_aux->name = allocname;
   ptr_aux->num_index = -1;
   ptr_aux->sens = 0;
   ptr_aux->next = tab[num_case] ;
   tab[num_case] = ptr_aux ;
   return ( ptr_aux );
}
/*-------------------------------------------------------------------*/
elt *ExisteIdent(ident,tab)
char *ident;
elt  *tab[] ;
{
elt *ptr_aux;
char *allocname;
   allocname = (char *)namealloc (ident);
   for(ptr_aux = tab[Hash(allocname)];
       (ptr_aux==NULL) || ( strcmp( ptr_aux->name , allocname ) != 0 );
       ptr_aux = ptr_aux->next
       );
   if ( ptr_aux == NULL) 
       Erreur ("identifier not found");
   return (ptr_aux);
}
/*-------------------------------------------------------------------
 * retourne soit -1 si le nom n'existe pas
 *          soit le champ num_index si le nom est trouve dans la table
 */
int FindIdent(ident,tab)
char *ident;
elt  *tab[] ;
{
int num_case;
elt *ptr_aux;
char *allocname;
   allocname = (char *)namealloc (ident);
    num_case = Hash(allocname);
    ptr_aux = tab[num_case] ;
    while (ptr_aux != NULL) 
    if ( strcmp( ptr_aux->name , allocname ) == 0 ) 
      return (ptr_aux->num_index);
    else 
      ptr_aux = ptr_aux->next;
    return(-1);
}

/* ###--------------------------------------------------------------### */
/* function     : namei							*/
/* description  : add a number to a name				*/
/* called func. : none							*/
/* ###--------------------------------------------------------------### */

char *namei(str,n)
char *str;
int n;
{
	char *p;

	if (str == NULL) 
	  return (NULL);
	p = (char *)malloc((strlen(str)+11) * sizeof(char));
	sprintf(p,"%s_%d",str,n);
	return(p);
}

/* ###--------------------------------------------------------------### */
/* function     : KillEsp                                               */
/* description  : remove every space character of a string              */
/* called func. : isspace                                               */
/* ###--------------------------------------------------------------### */

void KillEsp (name)
char *name;
{
int i = 0;
int j = 0;

   while (name [i] != '\0')
   {
      name [j] = name [i];
      if (isspace (name[i]) == 0)
        j++;
      i++;
   }
   name [j] = '\0';
}

/* ###--------------------------------------------------------------### */
/* function     : CheckIdent                                            */
/* description  : Check if ident is of type IDENT                       */
/* called func. : isspace, isalpha, isalnum, isupper, Erreur            */
/* ###--------------------------------------------------------------### */

void    CheckIdent (ident)
char *ident;
{
char  *cpt;
int i;
   cpt = ident;
   while ( isspace (*cpt) != 0 ) 
       cpt ++;
   if (isalpha(*cpt++) == 0)   
         Erreur ("IDENT error not an identifier");
   for (i = strlen(cpt); i >0; i--) 
   {
      if (isalnum(*cpt) == 0)
        if ((*cpt != '_') && (*cpt != '.'))
           Erreur ("IDENT error not an identifier");
      cpt ++;
   }
}


/* ###--------------------------------------------------------------### */
/* function     : CheckSeparate                                         */
/* description  : Check `# of blank between value` string declaration   */
/* called func. : isdigit                                               */
/* ###--------------------------------------------------------------### */
 
char CheckSeparate(chn_blk)
char *chn_blk;
{
   char *pt = chn_blk;

   if (*pt == ':')
   {
     pt++;
     if  (strlen (pt) == 1 )
     {
       if (isdigit (*pt))
          return (*pt);
     }
     if  (strlen (pt) == 0 )
          return ('0');
   }
   Erreur ("SEPARATOR error in paiol declaration");
   return (*pt);
}

/* ###--------------------------------------------------------------### */
/* function     : CheckFormat						*/
/* description  : Check if format is : x, X, o, O, b, B			*/
/* called func. : KillEsp, Erreur					*/
/* ###--------------------------------------------------------------### */
 
char CheckFormat (format)

char *format;
{
char value;

  KillEsp (format);
  if (strlen (format) > 1 ) 
    Erreur ("FORMAT format error");

  switch (*format)
    {
    case 'x':
    case 'X': 
      value = 'X';
      break;

    case 'o':
    case 'O': 
      value = 'O';
      break;

    case 'b':
    case 'B': 
      value = 'B';
      break;

    default:
      Erreur ("FORMAT error");
    }

  return (value);
  }

/* ###--------------------------------------------------------------### */
/* function     : CheckType						*/
/* description  : Check connector type					*/
/* called func. : KillEsp, Erreur					*/
/* ###--------------------------------------------------------------### */

char CheckType (type)
char *type;
{
    char t;

    KillEsp (type);
    if (strlen (type) > 1 ) 
        Erreur ("TYPE error");
    switch (*type)
    {
      case 'O':case 'o': t = 'O'; break; 
      case 'I':case 'i': t = 'I'; break; 
      case 'T':case 't': t = 'T'; break; 
      case 'S':case 's': t = 'S'; break; 
      case 'R':case 'r': t = 'R'; break; 
           break;
      default:
           Erreur ("MODE error");
    }
    return(t);
}


/* ###--------------------------------------------------------------### */
/* function     : CheckOption						*/
/* description  : Check connector option				*/
/* called func. : KillEsp, Erreur					*/
/* ###--------------------------------------------------------------### */

char CheckOption (option)
char *option;
{
    char flg = 0;

    KillEsp (option);
    if (strlen (option) > 1 ) 
        Erreur ("OPTION error");
    if (strlen (option) == 0)
    {
      flg = 0;
    }
    else
    {
      switch (*option)
      {
        case 'S': case 's': flg |= PAT_IOL__SPY; break; 
        default:
             Erreur ("OPTION error");
      }
    }
    return(flg);
}


/* ###--------------------------------------------------------------### */
/* function     : CheckPattern                                          */
/* description  : Check if string "pattern" describe a pattern number   */
/* called func. : KillEsp, isdigit, Erreur                              */
/* ###--------------------------------------------------------------### */
 
void CheckPattern (pattern)
char *pattern;    
{
int lg,i;

     KillEsp (pattern);
     lg = strlen (pattern);
     if ( lg == 0) 
         Erreur("PATTERN NUMBER syntax error");
     if ( (*pattern != '+') && (isdigit(*pattern )== 0) )
         Erreur("PATTERN NUMBER syntax error");
     for (i = 1; i< lg ; i++)
     {
        pattern ++;
        if ( isdigit(*pattern)  == 0  )
         Erreur("PATTERN NUMBER syntax error");
     }
}

/* ###--------------------------------------------------------------### */
/* function     : CheckWidth                                            */
/* description  : Check vector field descriptor                         */
/* called func. : atoi, Erreur                                          */
/* ###--------------------------------------------------------------### */

void  CheckWidth (chaine)
struct alpha_num *chaine;
{
int gauche, droite, lg_gauche, lg_droite, lg_alpha;

   lg_gauche = strlen (chaine->gauche);
   gauche    = atoi(chaine->gauche);
   lg_droite = strlen (chaine->droite);
   droite    = atoi(chaine->droite);
   lg_alpha  = strlen (chaine->alpha);

   if ( ( lg_alpha  == 0 &&  lg_droite != 0 ) ||
        ( lg_alpha  != 0 &&  lg_droite == 0 ) ||
        ( lg_gauche == 0 && ( lg_alpha != 0   ||  lg_droite != 0) ) )
      Erreur ("CheckWidth :VECTOR field descriptor error, missing argument");

   if ( (( strcmp (chaine->alpha,"DOWNTO")==0) ||
         ( strcmp (chaine->alpha,"downto")==0)   ) && (gauche < droite))
      Erreur ("CheckWidth :VECTOR field descriptor error, incorrect direction");

   if ( ( (strcmp (chaine->alpha,"TO")==0) ||
          (strcmp (chaine->alpha,"to")==0)   ) &&  (droite < gauche) )
      Erreur ("CheckWidth :VECTOR field descriptor error, incorrect direction");
}



/* ###--------------------------------------------------------------### */
/* function     : SplitIdent                                            */
/* description  : Split Ident (ident) in two strings :                  */
/*               Ident name (ident) and Ident number (chaine)           */
/* called func. : isspace                                               */
/* ###--------------------------------------------------------------### */


void  SplitIdent (ident,chaine)
char *chaine;
char *ident;
{
char idtmp[30],reste[30], *cpt;
int i,j =0,k =0, flag1=0,flag2=0;
   cpt = ident;
   for (i = strlen(ident); i >0; i--) 
   {
       if (flag1 == 0)
       {
           if (isspace(*cpt) == 0  &&  *cpt != '(' ) 
	   {
               flag2 = 1;
               idtmp[j++] = *cpt;
           }
           else 
	   {
	     if (flag2 ==  1 )  
	     {
	      flag1      = 1;
	      if (*cpt  == '(')  reste[k++] = *cpt;
	     }
	     else idtmp[j++] = *cpt;
           }
       }
       else
       {
            reste[k++] = *cpt;
       }
       cpt ++;
   }
   idtmp[j] = '\0';
   reste[k] = '\0';
   strcpy (chaine,reste);
   strcpy (ident,idtmp);
}

/* ###--------------------------------------------------------------### */
/* function     : TraiterChaine                                         */
/* description  : find out left & right bound and direction of an array */
/* called func. : isdigit, isalpha, isspace                             */
/* ###--------------------------------------------------------------### */

void TraiterChaine (chaine,traite)
char * chaine;
struct alpha_num *traite;
{
char *cpt;
int i=0,j=0,k=0,l=0,flag=0,f=0;

   KillEsp(chaine);

   if ( *chaine == '(') 
   {
     f = 1;
     chaine++;
   }
  
   cpt = chaine;
   for (i = strlen(chaine); i >0; i--) 
   {
     switch (flag) 
     {
      case 0: 
      {
          if (isdigit(*cpt) )
              traite->gauche[j++] = *cpt;
          if (isalpha (*cpt)) 
	  {
              traite->alpha[k++] = *cpt;
              flag = 1;
          }
	  if (*cpt == ')' && f == 0)
	    Erreur ("BIT descriptor error");
          break;
      }
      case 1: 
      {
          if (isalpha (*cpt)) 
	  {
              traite->alpha[k++] = *cpt;
              flag = 1;
	      break;
          }
	  if (isdigit(*cpt))
	  {
	     traite->droite[l++] = *cpt;  
	     flag = 2;
	     break;
	  }
	  if ( ! ( (*cpt == ')') && (f==1) && (i==1) ) )
	      Erreur("TraiterChaine : VECTOR field descriptor error");
          break;
      }
      case 2: 
      {
          if (isdigit(*cpt))
              traite->droite[l++] = *cpt; 
	  else
	     if( ! ( (*cpt == ')') && (f==1) && (i==1) ) )
              Erreur ("TraiterChaine  :VECTOR field descriptor error");
          break;
      }
     }
     cpt++;
   }
   traite->gauche[j] = '\0';
   traite->alpha[k] = '\0';
   traite->droite[l] = '\0';
   
   return ;
}
/* ###--------------------------------------------------------------### */
/* function     : CreatePagrp                                           */
/* description  : add a new PAGRP, set hash tab and ident tab & index   */
/* called func. : SymbAlloc, pat_addpagrp, Erreur			*/
/* ###--------------------------------------------------------------### */

void CreatePagrp(ident,length,sens,type_vecteur)
char *ident;
int length,sens;
short type_vecteur;
{
int index;
elt *tmp_elt;

     index = GNP_PAIOL_INDEX - length;
     if (index < 0 ) 
         Erreur ( "PAGRP length doesnt match");

     if (ind_tab_ident >= GNP_MAXTAB_IDENT) 
         Erreur ("PAGRP too much declared identifiers");

     tmp_elt = SymbAlloc(ident,hash_pagrp);
     racine_pagrp = pat_addpagrp(racine_pagrp,
                              tmp_elt->name,
                              length,
                              index,
			      type_vecteur);

     tmp_elt->num_index = ind_tab_ident;
     tmp_elt->sens = sens;
     tab_ident[ind_tab_ident].duplicate  =ind_tab_ident;
     tab_ident[ind_tab_ident++].pointeur =(void *)racine_pagrp;
}


/* ###--------------------------------------------------------------### */
/* function     : DupPaiol						*/
/* description  : Create (duplicate = -1) or duplicate a PAIOL struct	*/
/*	set PASEQ struct. and ident tab & index				*/
/* called func. : SymbAlloc, pat_addpaiol, ExisteIdent, Erreur		*/
/* ###--------------------------------------------------------------### */

void DupPaiol (duplicate,format,param_effectif,param_virtuel,i_o,blk,flg)
int duplicate;
char format;
char *param_effectif;
char *param_virtuel;
char i_o, blk, flg;
{
elt *tmp_elt_effectif;
elt *tmp_elt_virtuel;

     tmp_elt_virtuel = SymbAlloc (param_virtuel,hash_paiol);

     if (duplicate == GNP_NO_DUPLICATE)
	  tmp_elt_effectif = SymbAlloc(param_effectif,hash_paiol);
     else
          tmp_elt_effectif = ExisteIdent (param_effectif,hash_paiol);

     if (duplicate != tmp_elt_effectif->num_index) 
	Erreur ("DupPaiol error unconstistent paiol references");

     if (ind_tab_ident >= GNP_MAXTAB_IDENT) 
         Erreur ("too mutch declared identifiers");

     tmp_elt_virtuel->num_index = ind_tab_ident;

     racine_paiol = pat_addpaiol(racine_paiol,
                              tmp_elt_effectif->name,
                              format,
                              i_o,
                              (char)((int)blk - (int)'0'));

     racine_paiol->FLAG = flg;

     IOLNUMBER++;

     if (duplicate != GNP_NO_DUPLICATE) 
     {
         tab_ident[ind_tab_ident].duplicate = tab_ident[duplicate].duplicate;
         tab_ident[duplicate].duplicate = ind_tab_ident;
     }
     else 
     {
         tmp_elt_effectif->num_index = ind_tab_ident;
         tab_ident[ind_tab_ident].duplicate = ind_tab_ident;
     }

     tab_ident[ind_tab_ident].num_paiol = GNP_PAIOL_INDEX++;
     tab_ident[ind_tab_ident++].pointeur =(void *)racine_paiol;
}


/* ###--------------------------------------------------------------### */
/* function     : CreatePaiol                                           */
/* description  : add a new PAIOL, set hash tab and ident tab & index   */
/* called func. : SymbAlloc, pat_addpaiol, Erreur                       */
/* ###--------------------------------------------------------------### */

void CreatePaiol (format,ident,mode,blk,flg)
char format;
char *ident;
char mode,blk,flg;
{
elt *tmp_elt;

     if (ind_tab_ident >= GNP_MAXTAB_IDENT) 
         Erreur ("too mutch declared identifiers");

     tmp_elt = SymbAlloc(ident,hash_paiol);

     racine_paiol = pat_addpaiol(racine_paiol,
                              tmp_elt->name,
                              format,
                              mode,
                              (char)((int)blk - (int)'0'));

     racine_paiol->FLAG = flg;

     IOLNUMBER++;

     tmp_elt->num_index = ind_tab_ident;
     tab_ident[ind_tab_ident].duplicate = ind_tab_ident;
     tab_ident[ind_tab_ident].num_paiol = GNP_PAIOL_INDEX++;
     tab_ident[ind_tab_ident++].pointeur =(void *)racine_paiol;
}


/* ###--------------------------------------------------------------### */
/* function     : EmpileParam                                           */
/* description  : push parameter in parameter stack			*/
/* called func. : Erreur						*/
/* ###--------------------------------------------------------------### */

void EmpileParam(param)
char *param;
{
    if ( sp_pile_param >= GNP_MAXPILE_PARAM )
	Erreur (" no more place in param stack");

    pile_param[sp_pile_param ++] = param ;
}

/* ###--------------------------------------------------------------### */
/* function     : DefileParam                                           */
/* description  : unchain parameter					*/
/* called func. : Erreur						*/
/* ###--------------------------------------------------------------### */

char *DefileParam()
{
    if (  sp_file_param > sp_pile_param )
	Erreur("empty parameter file");

    return ( pile_param[sp_file_param++] ) ;
}




/* ###--------------------------------------------------------------### */
/* function     : FusionPaevt                                           */
/* description  : add a list of PAEVT at the and of a PAEVT list        */
/* called func. : FusionPaevt, Erreur                                   */
/* ###--------------------------------------------------------------### */

struct paevt *FusionPaevt (lst_event,tete_event)
struct paevt *lst_event;
struct paevt *tete_event;
{
struct paevt *tmp_paevt = tete_event;

  if (tete_event == NULL) return(lst_event);
  if (lst_event == NULL) return(tete_event);

  while ( (tmp_paevt->NEXT) != NULL )
    tmp_paevt = tmp_paevt->NEXT;
 
  tmp_paevt->NEXT = lst_event;

  return (tete_event);  
}



/* ###--------------------------------------------------------------### */
/* function     : CreateAction                                          */
/* description  : Change ACTFLAG flag in PAPAT 				*/
/* called func. : none                                                  */
/* ###--------------------------------------------------------------### */

void CreateAction (act_type)
char *act_type;
{
char car_act;
struct papat *tmp_papat;

   tmp_papat = racine_papat;

/* recherche de l'emplacement du papat dans la liste des papat */

   if (tmp_papat == NULL) 
      Erreur("CreateAction error not yet created pattern");

   while ((tmp_papat != NULL) && (tmp_papat->TIME > GNP_PATTERN_COURANT)) 
      tmp_papat = tmp_papat->NEXT;

   if (tmp_papat == NULL) 
      Erreur("CreateAction error not yet created pattern");

   if (tmp_papat->TIME != GNP_PATTERN_COURANT)
     Erreur ("CreateAction  : error in data structure");

   car_act = *act_type;
   tmp_papat->ACTFLAG = car_act;
}


/* ###--------------------------------------------------------------### */
/* function     : FusionPaini                                           */
/* description  : merge two initialization lists			*/
/* called func. : none							*/
/* ###--------------------------------------------------------------### */

struct paini *FusionPaini (fst_ini, scd_ini)

struct paini *fst_ini;
struct paini *scd_ini;

  {
  struct paini *res_ini;

  if (fst_ini == NULL)
    res_ini = scd_ini;
  else
    {
    res_ini = fst_ini;
    while (res_ini->NEXT != NULL)
      res_ini = res_ini->NEXT;

    res_ini->NEXT = scd_ini;
    res_ini       = fst_ini;
    }

  return (res_ini);
  }


/* ###--------------------------------------------------------------### */
/* function     : CreatePapat                                           */
/* description  : add  or set a PAPAT struct in PAPAT list              */
/* called func. : pat_addpacom, TriFusion				*/
/* ###--------------------------------------------------------------### */

void CreatePapat (label,lst_event,lst_ini)

char         *label;
struct paevt *lst_event;
struct paini *lst_ini;

  {
  struct papat *fils_papat;
  struct papat *pere_papat;
  struct papat *tmp_papat;
  int           size;


  if (racine_papat == NULL) 
    {

/* first PAPAT to be created */

    racine_papat = pat_addpapat (NULL, namei (label,GNP_PATTERN_COURANT),
                                 GNP_PATTERN_COURANT);
    racine_papat->TIME = GNP_PATTERN_COURANT;
    racine_papat->PAEVT = lst_event;
    racine_papat->PAINI = lst_ini;
    PATNUMBER = 1;
    }
  else 
    {

/* putting fils_papat after or on the good papat */

    pere_papat = racine_papat;
    fils_papat = racine_papat;

    while ((fils_papat != NULL) && (fils_papat->TIME > GNP_PATTERN_COURANT)) 
      {
      pere_papat = fils_papat;
      fils_papat = fils_papat->NEXT;
      }

    if ( (fils_papat == NULL) || (fils_papat->TIME < GNP_PATTERN_COURANT) ) 
      {

/* fils_papat is after the good papat or NULL*/


      tmp_papat = pat_addpapat (fils_papat, namei(label, GNP_PATTERN_COURANT),
                                GNP_PATTERN_COURANT);
      tmp_papat->TIME = GNP_PATTERN_COURANT;
      tmp_papat->PAEVT = lst_event;
      tmp_papat->PAINI = lst_ini;
      PATNUMBER++;

      if (pere_papat == fils_papat )
        racine_papat     = tmp_papat;
      else
        pere_papat->NEXT = tmp_papat;

      }
    else 
      {

/* fils_papat is on the good papat */


      fils_papat->PAEVT = FusionPaevt (lst_event, fils_papat->PAEVT);
      fils_papat->PAINI = FusionPaini (lst_ini  , fils_papat->PAINI);
      if (label != NULL)
        fils_papat->LABEL = namei(label,GNP_PATTERN_COURANT);
      }
    }

  if ( label != NULL)  
    {
    size = strlen (namei (label, GNP_PATTERN_COURANT));
    if (MAX_LABEL < size)
    MAX_LABEL = size;
    }
  }


/* ###--------------------------------------------------------------### */
/* function     : CreatePaevt                                           */
/* description  : add a PAEVT struct at the end of a PAEVT list         */
/* warning	: parameters validity is not tested			*/
/* called func. : pat_addpacom, TriFusion				*/
/* ###--------------------------------------------------------------### */

struct paevt *CreatePaevt(tete_paevt,num_ident,valeur)
struct paevt *tete_paevt;
int num_ident;
char valeur;
{
struct paevt *tmp_paevt ;

  tmp_paevt = pat_addpaevt(NULL,tab_ident[num_ident].num_paiol,valeur);
  tete_paevt = FusionPaevt(tmp_paevt,tete_paevt);
  return (tete_paevt);
}


/* ###--------------------------------------------------------------### */
/* function     : MajPatternCourrant                                    */
/* description  : Set current pattern number 				*/
/* called func. : atoi                                                  */
/* ###--------------------------------------------------------------### */
 
void MajPatternCourant(pattern)
char *pattern;
{
unsigned long relatif = 0, valeur;

     if(*pattern == '+') 
     {
         relatif = 1;
         pattern++;
     }
     valeur = atoi (pattern);
     GNP_PATTERN_COURANT = (GNP_PATTERN_COURANT * relatif) + valeur;
}


/* ###--------------------------------------------------------------### */
/* function     : FormatValue						*/
/* description  : change inout value to binary according to format      */
/* called func. : Erreur						*/
/* ###--------------------------------------------------------------### */

void FormatValue (ret_val,value,nb_car,sens,i_o)
char *ret_val;
char *value;
int nb_car, sens;
int i_o;
{
char *valeur;
char *res;
int lg,i,li;
char  tmp [64];
char flg =' ';

   valeur = value ;
   if (i_o == 0)
     {
        lg = strlen (valeur);
        for (i = 0; i< lg ; i++)
        {
           if (*valeur == '*')
	    Erreur ("AFFECT/INIT incorrect input or init-value syntax");
           valeur ++;
        }
	valeur = value;
     }

   if (strlen (valeur) >1)
     valeur++;
   switch (*valeur)
   {
     case 'b': case 'B': 
     {
        valeur++;
        lg = strlen (valeur);
        strcpy (ret_val,"\0");
        for (i = 0 ; i < lg ; i++) 
        {
            switch (*valeur) 
            {
               case '0': { strcat (ret_val,"-");  break; }
               case '1': { strcat (ret_val,"+");  break; }
               case '*': { strcat (ret_val,"*");
			   flg='*';
			   break; }
               default : 
                  Erreur ("AFFECT/INIT incorrect binary format");
            }
            valeur++;
        }
        break;
     }
     case 'o': case 'O': 
     {
        valeur++;
        lg = strlen (valeur);
        strcpy (ret_val,"\0");
        for (i = 0 ; i < lg ; i++) 
        {
            switch (*valeur) 
            {
               case '0': { strcat (ret_val,"---");  break; }
               case '1': { strcat (ret_val,"--+");  break; }
               case '2': { strcat (ret_val,"-+-");  break; }
               case '3': { strcat (ret_val,"-++");  break; }
               case '4': { strcat (ret_val,"+--");  break; }
               case '5': { strcat (ret_val,"+-+");  break; }
               case '6': { strcat (ret_val,"++-");  break; }
               case '7': { strcat (ret_val,"+++");  break; }
               case '*': { strcat (ret_val,"***");
			   flg='*';
			   break; }
               default :  Erreur ("AFFECT/INIT incorrect octal format"); 
            }
            valeur++;
        }
        break;
     }
     case 'x': case 'X': 
     {
	valeur++;
        lg = strlen (valeur);
        strcpy (ret_val,"\0");
        for (i = 0 ; i < lg ; i++) 
        {
            switch (*valeur) 
            {
                case '0': { strcat (ret_val,"----"); break; }
                case '1': { strcat (ret_val,"---+"); break; }
                case '2': { strcat (ret_val,"--+-"); break; }
                case '3': { strcat (ret_val,"--++"); break; }
                case '4': { strcat (ret_val,"-+--"); break; }
                case '5': { strcat (ret_val,"-+-+"); break; }
                case '6': { strcat (ret_val,"-++-"); break; }
                case '7': { strcat (ret_val,"-+++"); break; }
                case '8': { strcat (ret_val,"+---"); break; }
                case '9': { strcat (ret_val,"+--+"); break; }
                case 'A': case 'a': { strcat (ret_val,"+-+-"); break; }
                case 'B': case 'b': { strcat (ret_val,"+-++"); break; }
                case 'C': case 'c': { strcat (ret_val,"++--"); break; }
                case 'D': case 'd': { strcat (ret_val,"++-+"); break; }
                case 'E': case 'e': { strcat (ret_val,"+++-"); break; }
                case 'F': case 'f': { strcat (ret_val,"++++"); break; }
                case '*': { strcat (ret_val,"****"); 
			    flg='*';
			    break; }
                default : Erreur ("AFFECT/INIT incorrect hexad. format");
            }
            valeur++;
        }
        break;
     }
     default : 
     {
	valeur = value;
        lg = strlen (valeur);
        for (i = 0; i< lg ; i++)
        {
           if ( isdigit(*valeur)  == 0  )
	    Erreur ("AFFECT/INIT incorrect decimal format");
           valeur ++;
        }
	li = atoi(value);

	valeur = ret_val;
	while(li != 0)
	{
	   if ( (li & 0x01) == 1)
	     *valeur++ = '+';
	   else
	     *valeur++ = '-';
	   li = li >> 1; 

	}
	if ( valeur == ret_val )
	  *valeur++ = '-';
	*valeur = '\0';

	valeur = ret_val;
	lg = strlen(ret_val);
	for(i = lg-1; i>=0; i--) tmp[i]=*valeur++;
	tmp[lg]= '\0';
	strcpy(ret_val,tmp);
	break;
     }
   }
/* ajuster la longueur de la chaine en moins */
   lg = strlen (ret_val);
   if (lg > nb_car) 
   {
       valeur = res = ret_val ;
       printf ("line %d :VALUE too large value: MSB are truncated\n",GNP_LINE);
       for (i = 0; i< (lg - nb_car) ; i++,res++);
       while ( *res != '\0')
       {
         *valeur = *res;
          valeur++;
	  res++;
       }
       *valeur = '\0'; 
   }
/* ajuster la longueur de la chaine en + avec des 0 */
   if (lg < nb_car) 
   {
      valeur = ret_val;
      for (i = 0 ; i < (nb_car - lg) ; i ++) tmp[i] = '-';
      
      for (i = i ; i < nb_car ; i ++)  tmp[i] = *valeur++; 

      tmp[i] = '\0';
      strcpy (ret_val,(char *)tmp);
   }
   if (sens == GNP_UP) 
   {
/* inverser la chaine */
      valeur = ret_val;
      for (i = nb_car-1 ; i >=0 ; i--)  tmp[i] = *valeur++; 
      tmp[nb_car] = '\0';
      strcpy (ret_val,(char *)tmp);
   }
/* ajuster le format */
   if ((i_o == 0) || (flg == '*'))
   {
    valeur = ret_val;
    for (i = nb_car-1 ; i>= 0 ; i--) 
     if (flg == '*')
	*valeur++ = '*';
     else
     {
        *valeur = ( (*valeur == '-') ? '0' : '1' );
	valeur++;
     }
   } 
}
