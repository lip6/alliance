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

/*******************************************************************************
* as it seems, needed for assembling and controling yacc output                *
*******************************************************************************/
# include <stdio.h>
# include <string.h>
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
# include "d_dbx.h"

/* variables globales */

PSYMB TabSymb;  /* pointeur sur la table des symboles */
int TailleTabSymb = 0;  /* taille de la table des symbole */
int MaxTabSymb;  /* taille maximale de TabSymb */
int NbSymb = 0;  /* nombre de symboles dans la table */

unsigned char *I_Code;  /* pointeur sur le bloc du code */
int taille_max_code;  /* taille allouee pour le code */

unsigned char *I_Data;  /* pointeur sur le bloc data */
int taille_max_data;  /* taille allouee pour les data */

D_PREF TabRef;  /* pointeur sur la table des ref.
       pour la deuxieme passe          */
int TailleTabRef = 0;  /* taille de la table des references */
int MaxTabRef;  /* taille maximale de TabRef */
int NbRef = 0;  /* nombre de references */

int I_Debut_Code;  /* adresse de debut du code */
int I_Fin_Code;  /* adresse de fin du code */
int I_Debut_Data;  /* adresse de debut des donnees */
int I_Fin_Data;  /* adresse de fin des donnees */

char *D_compilerPath; /* search path of the gcc compiler */
char *D_compilerInclude;

PTFUNC first_func;
extern PTFUNC FUNC_malloc();
extern ptfonc First_fonction;

/* much like mbk namealloc :
   but dedicated to the assembler since C is case sensitive and mbk not.
   It warranties that two string equal at the strcmp sens have the same
   pointer. */

#define HSIZE 101
static chain_list *HTABLE[HSIZE];

char *
  uniquename(inputname)
char *inputname;
{
chain_list *pt;
char *name = inputname;
int code = 0;

  name = inputname;
  while (*name)
    code = code << 1 ^ *name++;
  if (code < 0)
    code = -code;
  code %= HSIZE;

  for (pt = HTABLE[code]; pt; pt = pt->NEXT)
    if (!strcmp(inputname, (char *)pt->DATA))
      return (char *)pt->DATA;

  HTABLE[code] = addchain(HTABLE[code], (void *)mbkstrdup(inputname));
  return (char *)HTABLE[code]->DATA;
}

/* delete all the dictionnary entries :
   the name dictionnary is useful during assembling only. */
void
  freeuniquenames()
{
int i;
chain_list *pt;

  for (i = 0; i < HSIZE; i++) {
    for (pt = HTABLE[i]; pt; pt = pt->NEXT)
      mbkfree(pt->DATA);
    freechain(HTABLE[i]);
    HTABLE[i] = NULL;
  }
}

/* GereTabSymb : fonction de gestion de la table des symboles.  Seules les
    adresses des symboles sont dans la table, la place pour ceux-ci
    est allouee au fur et a mesure dans LEX.
    La fonction cherche le symbole dans la table. Si celui-ci
    existe, elle rend son index et note sa position si ce n'est pas
    deja fait. Sinon elle le place dans la table avant de rendre
    l'index. On note de plus si le symbole est une reference
    a une donnee, ou si c'est une reference a du code,
    cela servira a calculer son adresse absolue dans SecondePasse()
 parametre : pointeur sur un symbole
       position du symbole ( -1 si inconnue )
       booleen indiquant si c'est une donnee ou du code
 rend : l'index du symbole dans la table.
*/
int GereTabSymb(Symb, Position, Data)
char *Symb;
int Position;
BOOL Data;
{
int PosInTable;
int NewTailleTab;

  if (!Symb)
    return -1;

  Symb = uniquename(Symb);
  PosInTable = RechTabSymb(Symb);

  if (PosInTable == -1) {
    NewTailleTab = IncTailleTab(&TailleTabSymb, sizeof(SYMB),
                      &MaxTabSymb, TAILLE_TAB_SYMB, &TabSymb, 1);
    PosInTable = NewTailleTab / sizeof (SYMB) - 1;
    TabSymb[PosInTable].Symb = Symb;
    TabSymb[PosInTable].Pos = Position;
    TabSymb[PosInTable].Data = UNKNOWN;
    TailleTabSymb = NewTailleTab;
    NbSymb++;
  } else if (Position != -1)
    TabSymb[PosInTable].Pos = Position;

  if (Data != UNKNOWN)
    TabSymb[PosInTable].Data = Data;

  return PosInTable;
}

/* RechTabSymb : recherche sequentielle d'un symbole dans la table
    des symboles ( la table est sequentielle car les
    symboles ne doivent pas bouger dans la tables entre
    les deux passes d'assemblage )
 parametre : le poiteur sur la chaine contenant le symbole
 rend : la place du symbole s'il est dans la table, -1 sinon
*/
int RechTabSymb(Symb)
char *Symb;
{
register int i;

  Symb = uniquename(Symb);
  for (i = 0; i < NbSymb; i++)
    if (Symb == TabSymb[i].Symb)
      return i;
  return -1;
}

/* RechInTable : recherche dichotomique d'un symbole dans une table
    on supose que le prmier champ de chaque element de
    la table commence par un pointeur sur chaine
 parametre : le pointeur sur le symbole a rechercher
       le pointeur sur la table ou l'on doit le chercher
       le nombre d'elements de la table
 rend : la place ou le symbole est place ou ou il devrait etre
*/
int RechInTable(Symb, Tab, TailleTab, TailleElem)
char *Symb;
char Tab[];
int TailleTab;
int TailleElem;
{
int Max, Min, Rang;
int Res;

  /* recherche dichotomique dans la table */
  Max = TailleTab;
  Min = -1;

  while (Max - Min > 1) {
    Rang = Min + ((Max - Min) / 2);
    if (!(Res = strcmp(*((char **) &(Tab[Rang * TailleElem])), Symb)))
      return Rang;
    if (Res > 0)
      Max = Rang;
    else
      Min = Rang;
  }
  return Max;
}

/* Erreur : fonction de gestion d'erreur, affiche la chaine passee et
    provoque l'arret par exit .
 parametre : pointeur sur la chaine a afficher
*/

void Erreur(Str)
char *Str;
{
  fprintf(stdout, "Debugger error :\n    %s", Str);
  T_print("\n");
  EXIT(-2);
}

/* AdrVarGlobal : fonction de recherche d'adresse pour une variable globale
    permet de completer la liste des variables pour le mode trace */
/* set addresses for everithing that is not relative to functions */
AdrVarGlobal()
{
extern PTVARIABLE firstVariable;
int i;
PTVARIABLE ParcoursVar;

  for (ParcoursVar = firstVariable; ParcoursVar;
      ParcoursVar = ParcoursVar->next) {
    if (ParcoursVar->scope == GLOBAL_SCOPE
        || ParcoursVar->scope == STATIC_FILE_SCOPE
      ) {
      for (i = 0; i < TailleTabSymb / sizeof (SYMB) &&
          strcmp(ParcoursVar->name, TabSymb[i].Symb +1); i++);
      if (TabSymb[i].Data)
        ParcoursVar->address = TabSymb[i].Pos + I_Debut_Data;
      else
        ParcoursVar->address = TabSymb[i].Pos + I_Debut_Code;
    }
  }
}

AdrVarStatic()
{
extern PTVARIABLE firstVariable;
int i, j = 0; /* j is the static variable counter, needed for strcmp */
PTVARIABLE var;
char buffer[255];

  for (var = firstVariable; var; var = var->next) {
    if (var->scope == STATIC_LOCAL_SCOPE) {
      sprintf(buffer, "%s_%d", var->name, j++);
      for (i = 0;
          i < TailleTabSymb / sizeof (SYMB)
          && strcmp(buffer, TabSymb[i].Symb + 1);
        i++);
      if (TabSymb[i].Data)
        var->address = TabSymb[i].Pos + I_Debut_Data;
      else
        var->address = TabSymb[i].Pos + I_Debut_Code;
    }
  }
}

/* AffTabSymb : print out symbol table for debug */
void AffTabSymb()
{
int i;

  for (i = 0; i < TailleTabSymb / sizeof (SYMB); i++)
    fprintf(stdout, "entry %d : %s @(%ld):(%d)\n", i, TabSymb[i].Symb,
          TabSymb[i].Pos, TabSymb[i].Data);
  T_print("\n");
}

/* SymbInTab : donne le pointeur sur le symbole dont on passe l'index
    dans la table des symboles
 parametre : index dans la table des symboles
 rend : le pointeur sur la chaine contenant le symbole
*/
char *SymbInTab(Index)
int Index;
{
  if (Index == -1)
    return NULL;

  if (Index < 0 || Index > NbSymb)
    Erreur("Internal Error -> SymbInTab : index is not correct");

  return TabSymb[Index].Symb;
}

/* RechCodeInst : fonction de recherche du code d'une instruction
    assembleur
 parametre : pointeur sur la chaine de caracteres contenenant
    le nom de l'instruction
 rend : 0 si la chaine de caractere ne contient pas un nom
    d'instruction, sinon le code de l'instruction
    dans un int
*/
int RechCodeInst(Inst)
char *Inst;
{
int rang;
extern SYMB TabCodes[];  /* correspondance inst. <--> code */
extern int NbTabCodes;  /* taille de la table de correspondance */

  rang = RechInTable(Inst, TabCodes, NbTabCodes, sizeof (SYMB));

  if (strcmp(TabCodes[rang].Symb, Inst))
    return 0;
  else
    return TabCodes[rang].Pos;
}

/* IncTailleTab : fonction d'incrementation de la taille d'une table,
      elle ajoute a la taille le second parametre et
      verifie que la taille est inferieure a la taille
      allouee (troisieme parametre). si elle ne l'est
      pas, on effectue alors reallocation d'une taille
      superieure .
 parametre : la taille avant augmentation
       le nombre d'octets dont on doit l'augmenter
       la taille deja allouee
       le pas d'incrementation de la taille
       le pointeur sur la table
       le facteur d'alignement
 rend : la nouvelle taille du code
*/
int IncTailleTab(taille_code, nb_oct, taille_max, step, Tab, alig)
int *taille_code, nb_oct;
int *taille_max;
int step;
char **Tab;
int alig;
{
unsigned taille_bloc;

  *taille_code = ((*taille_code + alig - 1) / alig) * alig;
  taille_bloc = *taille_code;
  taille_bloc += nb_oct;

  if (taille_bloc >= *taille_max) {
    *taille_max += step;
    *Tab = mbkrealloc(*Tab, *taille_max);
    if (*Tab == NULL)
      Erreur("IncTailleTab : reallocation impossible");
  }
  return taille_bloc;
}

/* AddRef : ajoute une reference dans la table des references
 parametre : la reference a ajouter
       deplacement par rapport a la reference
*/
void AddRef(Ref, Dep, type)
int Ref;
int Dep;
char type;
{
int NewTailleRef;

  NewTailleRef = IncTailleTab(&TailleTabRef, sizeof (D_REF),
                      &MaxTabRef, TAILLE_TAB_REF, &TabRef, 1);
  TabRef[NbRef].ref = Ref;
  TabRef[NbRef].dep = Dep;
  TabRef[NbRef].type = type;
  NbRef++;

  TailleTabRef = NewTailleRef;
}

/* SecondePasse : effectue la seconde passe de l'assemblage. resout
    les references connues, et transforme les appel
    a des references non connues par des interruptions.  */
void SecondePasse()
{
int i, AdrRef;
int IndSymb;
char *VerifVarExt();

  for (i = 0; i < TailleTabRef / sizeof (D_REF); i++) {
    if (!TabRef[i].type) {
      IndSymb = *((int *) &(I_Data[TabRef[i].ref]));
    } else {
      IndSymb = *((int *) &(I_Code[TabRef[i].ref]));
    }
    AdrRef = TabSymb[IndSymb].Pos;
    if (AdrRef == -1) {
      if (TabSymb[IndSymb].Data == FALSE) {
        I_Code[TabRef[i].ref - 4] = INT;
        I_Code[TabRef[i].ref - 3] = MODE_IMM;
      } else
      *((int *)&(I_Code[TabRef[i].ref])) =
                        (int)VerifVarExt(TabSymb[IndSymb].Symb);
    } else if (TabSymb[IndSymb].Data) {
      if (!TabRef[i].type)
        *((int *) &(I_Data[TabRef[i].ref])) =
                          ((int) I_Data) + AdrRef + TabRef[i].dep;
      else
        *((int *) &(I_Code[TabRef[i].ref])) =
                          ((int) I_Data) + AdrRef + TabRef[i].dep;
    } else if (!TabRef[i].type)
      *((int *) &(I_Data[TabRef[i].ref])) =
                          ((int) I_Code) + AdrRef + TabRef[i].dep;
    else
      *((int *) &(I_Code[TabRef[i].ref])) =
                          ((int) I_Code) + AdrRef + TabRef[i].dep;
  }
}

int AfterInterp()
{
extern int taille_code;
extern int taille_data;
extern char *I_sp;
extern FILE *asmin;

  fclose(asmin);

  mbkfree(I_sp);
  I_sp = NULL;
  mbkfree(I_Data);
  I_Data = NULL;
  taille_data = 0;
  taille_max_data = 0;

  mbkfree(I_Code);
  I_Code = NULL;
  taille_code = 0;
  taille_max_code = 0;

  mbkfree(TabSymb);
  TabSymb = NULL;
  MaxTabSymb = 0;
  TailleTabSymb = 0;
  NbSymb = 0;

  mbkfree(TabRef);
  TabRef = NULL;
  MaxTabRef = 0;
  TailleTabRef = 0;
  NbRef = 0;
}


int PrepInterp(c_file)
char *c_file;
{
extern FILE *asmin;
extern char *I_sp;
extern int I_reg[];
extern int I_pc;
extern BOOL InData;
extern BOOL D_FirstExec;
extern int taille_code;
extern int taille_data;
extern int NbCall;
int pos_main;
/* add -dr before -B for rtl file output debug purpose */
char *format = "%s/genview_gcc -o %s -DGENVIEW -I$PWD -I%s -B%s/ -fno-defer-pop -S -g %s";
char *cpp_format = "%s/genview_cpp -I$PWD -I%s %s %s";
char *buffer = (char *)mbkalloc((strlen(format)
                                + 2 * strlen(D_compilerPath)
                                + 2 * strlen(c_file)
                                + strlen(D_compilerInclude) + 1
                                ) * sizeof(char));
char *asm_file = (char *)mbkalloc((strlen(c_file) + 1) * sizeof(char));
char *cpp_file = (char *)mbkalloc((strlen(c_file) + 1) * sizeof(char));

FILE *fVar;
static BOOL FirstTime = TRUE;
int ret;
static int for_flex; /* needed in FLEX to call yyrestart */

  if (c_file[strlen(c_file) - 2] != '.' && c_file[strlen(c_file) - 1] != 'c') {
    T_print("File with unknown suffix : Not compiled\n");
    return 0;
  }

  T_print("Preprocessing ...\n");
  strcpy(cpp_file, c_file);
  cpp_file[strlen(c_file) - 1] = 'i'; /* change extension for assembly */
  sprintf(buffer, cpp_format, D_compilerPath, D_compilerInclude, c_file, cpp_file);

  ret = T_execute(buffer);
  if (unlink(c_file))
    T_print("Could not unlink temporary c_file!\n");
  if (ret == 256) {
      T_print ("Preprocessing aborted.\n");
      return 0;
  }

  T_print("Compiling ...\n");
  strcpy(asm_file, c_file);
  asm_file[strlen(c_file) - 1] = 's'; /* change extension for assembly */
  sprintf(buffer, format, D_compilerPath, asm_file, D_compilerInclude,
           D_compilerPath, cpp_file);

  ret = T_execute(buffer);
  if (unlink(cpp_file))
    T_print("Could not unlink temporary cpp_file!\n");
  if (ret == 256) {
      T_print ("Compilation aborted.\n");
      return 0;
  }

  /* initialisation in case the assembler failed for any reason */
  if (!FirstTime)
    AfterInterp ();
  FirstTime = FALSE;
  D_FirstExec = TRUE;

  T_print ("Assembling ...\n");

  InData = FALSE;  /* needed in the yacc file */

  if (!(asmin = fopen (asm_file, "r"))) {
    sprintf(buffer, "Cannot open %s\n", asm_file);
    Erreur(buffer);
  }
/*
  if (unlink(asm_file))
    T_print("Could not unlink temporary asm_file!\n");
*/
  mbkfree(buffer);
  mbkfree(asm_file);

  if (!(I_Data = (unsigned char *)mbkalloc(BLOC_DATA)))
    Erreur("Cannot allocate data segment");
  taille_max_data = BLOC_DATA;

  if (!(I_Code = (unsigned char *)mbkalloc(BLOC_CODE)))
    Erreur("Cannot allocate workspace");
  taille_max_code = BLOC_CODE;

  if (!(TabSymb = (PSYMB)mbkalloc(TAILLE_TAB_SYMB)))
    Erreur("Cannot allocate memory for symbol table");
  MaxTabSymb = TAILLE_TAB_SYMB;

  if (!(TabRef = (D_PREF)mbkalloc(TAILLE_TAB_REF)))
    Erreur("Cannot allocate memory for reference table");
  MaxTabRef = TAILLE_TAB_REF;

  freeuniquenames(); /* destroy old dictionnary contents */
  free_dbx_info(); /* erase old dbx informations */

#ifdef FLEX_SCANNER
  if (for_flex)
    asmrestart(asmin);
  else
    for_flex = 1;
#endif
  asmparse();
  RempFonc();

  T_print("Assembling (2nd Passe)...\n");
  SecondePasse();

  I_Debut_Code = (int)I_Code;
  I_Fin_Code = (int)I_Code + taille_code;
  I_Debut_Data = (int)I_Data;
  I_Fin_Data = (int)I_Data + taille_data;

  AdrVarGlobal(); /* global variable addresses are in the symbol table */
  AdrVarStatic(); /* and so for static, but an other way */

  if (!(I_sp = mbkalloc(TAILLE_PILE)))
    Erreur("Interpreter : cannot allocate the stack.");

  I_reg[31] = TAILLE_PILE + ((int)I_sp);

  if (!(fVar = fopen("var.tmp.dat", "wb")))
    if (!(fVar = fopen("var.tmp.dat", "w+b")))
      Erreur("impossible to open var.tmp.dat");

  /* sauvegarde des valeurs initiales des variables
    pour reexecution ulterieure */
  if (fwrite(I_Code, 1, I_Fin_Code - I_Debut_Code, fVar) <
      I_Fin_Code - I_Debut_Code)
    Erreur("write error on var.tmp.dat");

  fclose (fVar);

  if ((pos_main = RechTabSymb ("_main")) == -1) {
    T_print("No main() in the program !");
    return 0;
  }

  I_pc = TabSymb[pos_main].Pos + ((int) I_Code);
  NbCall = 0;

  return 1;
}

/* VerifVarExt : teste si une variable dont on ne connait pas
    l'adresse est une variable externe connue
    ( variable MBK ou systeme )
    provoque une erreur si ce n'en n'est pas une
 parametre : nom de la variable
 rend : son adresse absolue si elle est connue
*/

extern int NbVarExt;
extern VAR TabVarExt[];

char *VerifVarExt (var)
char *var;
{
char buffer[50];
int PosVar;

  PosVar = RechInTable(var, TabVarExt, NbVarExt, sizeof (VAR));

  if (!strcmp(var, TabVarExt[PosVar].var)) {
    if (TabVarExt[PosVar].type == VAR_DAT) {
      return TabVarExt[PosVar].adr.var;
    } else
      return (char *)TabVarExt[PosVar].adr.ptr;
  }

  sprintf(buffer, "%s unknown variable\n", var);
  Erreur(buffer);
  return NULL;
}

/* ReInitialise :
   provoque la reinitialisation complete du systeme
   utilisee quand on recommence l'execution */
ReInitialise()
{
extern int NbCall;
extern phfig_list *HEAD_PHFIG;
extern BOOL D_FirstExec;
extern int I_reg[];
FILE *fVar;
PTFUNC func;
void *next, **ppobj, **ppfig;
char *name;

  NbCall = 0;  /* pour la pile des appels de fonction */

  /* graphic cache :
    deleted. */
  Del_cache();
  V_clear();

  /* mbk :
     the entire data structure is erased. */
  for (ppfig = (void**)&HEAD_PHFIG; *ppfig != NULL;) {
    if (((phfig_list *)*ppfig)->MODELCHAIN)
      freechain(((phfig_list *)*ppfig)->MODELCHAIN);
    for (ppobj = (void**)&((phfig_list *)*ppfig)->PHSEG; *ppobj != NULL;) {
      next = (void *)((phseg_list *)*ppobj)->NEXT;
      mbkfree(*ppobj);
      *ppobj = next;
    }
    for (ppobj = (void**)&((phfig_list *)*ppfig)->PHINS; *ppobj != NULL;) {
    ptype_list *p = getptype(((phins_list *)*ppobj)->USER, PLACEABOX);
      if (p) {
        freenum((num_list *)p->DATA);
        freeptype(p);
      }
      next = (void *)((phins_list *)*ppobj)->NEXT;
      mbkfree(*ppobj);
      *ppobj = next;
    }
    for (ppobj = (void**)&((phfig_list *)*ppfig)->PHCON; *ppobj != NULL;) {
      next = (void *)((phcon_list *)*ppobj)->NEXT;
      mbkfree(*ppobj);
      *ppobj = next;
    }
    for (ppobj = (void**)&((phfig_list *)*ppfig)->PHREF; *ppobj != NULL;) {
      next = (void *)((phref_list *)*ppobj)->NEXT;
      mbkfree(*ppobj);
      *ppobj = next;
    }
    for (ppobj = (void**)&((phfig_list *)*ppfig)->PHVIA; *ppobj != NULL;) {
      next = (void *)((phvia_list *)*ppobj)->NEXT;
      mbkfree(*ppobj);
      *ppobj = next;
    }
    next = (void *)((phfig_list *)*ppfig)->NEXT;
    mbkfree(*ppfig);
    *ppfig = next;
  }
  HEAD_PHFIG = NULL;
  WORK_PHFIG = NULL;

  /* variables :
     get them back from disk. */
  if (!D_FirstExec) {
    if (!(fVar = fopen("var.tmp.dat", "rb")))
      if (!(fVar = fopen("var.tmp.dat", "r+b")))
        Erreur("impossible to open var.tmp.dat");

    if (fread(I_Code, 1, I_Fin_Code - I_Debut_Code, fVar) <
        I_Fin_Code - I_Debut_Code)
      Erreur("read problem on var.tmp.dat");
    fclose(fVar);
  }

  /* reinitialisation de la struct func dans d_interp_br.c */
  for (ppobj = (void**)&first_func; *ppobj != NULL;) {
    mbkfree(((PTFUNC)*ppobj)->name_func);
    next = (void *)((PTFUNC)*ppobj)->next_func;
    mbkfree(*ppobj);
    *ppobj = next;
  }

  /* initialisation de la liste func [d_interp_br.c] en traitant le
     cas particulier de la procedure principale _main. */

  first_func = FUNC_malloc();
  first_func->name_func = mbkstrdup("_main");
  first_func->adr_func = I_reg[31] - 4;

  D_freeTrace();
}
