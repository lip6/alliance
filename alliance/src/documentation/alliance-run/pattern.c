/**********************************************************************************************/
/******************************* Pierre Nguyen Tuong ******************************************/
/******************************* 03/10/1999          ******************************************/
/******************************* DEA ASIME - DESS    ******************************************/
/**********************************************************************************************/
/******************************* Generation des vecteurs de tests de l'am2901 *****************/
/**********************************************************************************************/
/**********************************************************************************************/


#include <genpat.h>
#include <stdio.h>
#include "mut.h" 


#define RplusS   0 
#define SmoinsR  1
#define RmoinsS  2
#define RouS     3
#define RetS     4
#define nonRetS  5 
#define RouxS    6
#define nonRouxS 7



/********************************* Variables globales *****************************************/

long vct      = 0     ; /* Moment de l'evenement      */
long interval = 100L  ; /* Interval                   */

int ck    = 0 ;
int fonc  = 0 ;
int test  = 0 ;
int scin  = 0 ;
int scout = 0 ;
int i     = 0 ;
int a     = 0 ;
int b     = 0 ;
int d     = 0 ;
int noe   = 0 ;
int r0    = 0 ;
int r3    = 0 ;
int q0    = 0 ;
int q3    = 0 ;
int ovr   = 0 ;
int zero  = 0 ;
int signe = 0 ;
int np    = 0 ;
int ng    = 0 ;
int cin   = 0 ;
int cout  = 0 ;
int y     = 0 ;

int r     = 0 ; /* Operande r                 */
int s     = 0 ; /* Operande s                 */
int res   = 0 ; /* Resultat de l'operation    */
int op        ; /* Code operatoire            */
int sa        ; /* Sortie a de la ram         */
int sb        ; /* Sortie b de la ram         */
int sq        ; /* Sortie q de l'accumulateur */
int mcode     ; /* Micro code                 */

short c[5]    ; /* La retenue */

/********************************* Fonctions utiles *******************************************/

/*** Transforme un entier en chaine ***/

char *entierVersChaine(long entier)
{
 char *chaineEntier ;
 chaineEntier = (char *) mbkalloc (32 * sizeof (char));
 sprintf (chaineEntier,"%i",entier);

 return(chaineEntier);
}

/*** Transforme un octal en chaine ***/

char *octVersChaine(int entier)
{
 int temp ;
 char *chaineEntier ;
 char *chaineOctal  ;

 chaineEntier = (char *)mbkalloc(32 * sizeof (char));
 chaineOctal  = (char *)mbkalloc(32 * sizeof (char)); 

 sprintf(chaineEntier,"%i",entier) ;                 /* Met le nombre octal dans la chaine  */
 temp = (int)strtol(chaineEntier,(char **)NULL,8) ;  /* Convertit le nombre octal en entier */
 sprintf(chaineOctal,"%i",temp) ;                    /* Met l'entier dans la chaine         */
 free(chaineEntier) ;

 return(chaineOctal);
}

/*** Fait avancer l'horloge de c coups ***/

void coupHorloge(long c)
{
 long i ;

 for(i = 1 ; i <= c ; i++)
   {
    AFFECT(entierVersChaine(vct),"ck","0") ;
    vct = vct + interval ;
    AFFECT(entierVersChaine(vct),"ck","1") ;
    vct = vct + interval ;
   }
}


/********************************* Fonctions de verifications *********************************/


/*** zero ***/

int calculZero(void)
{
 if(res == 0)
   zero = 1 ;
 else
   zero = 0 ;
}

/*** Retenue de sortie ***/

void calculCout(void)
{
 int opx ;
 int opy ;
 int x   ;
 int y   ;
 int i   ;
 int z   ;

/* Initialisation */
 switch(op)
   {
   case RplusS  : opx = r    ;
                  opy = s    ;
                  break      ;
   case SmoinsR : opx = s    ;
                  opy = ~(r) ;
                  break      ;
   case RmoinsS : opx = r    ;
                  opy = ~(s) ;
                  break      ;
   case RouS    :
   case RetS    : opx = r    ;
                  opy = s    ;
                  break      ;
   case nonRetS :
   case RouxS   :
   case nonRouxS: 

   default      : break      ;
   }

 c[0] =  cin ;

 /* Le calcul de la retenue */

 for (i = 1;i <= 4;i++)
   {
    x = (opx & 0x0001);
    y = (opy & 0x0001);
       if (((x == 1) && (y == 1))
          ||((x == 1) && (c[i-1] == 1))
          ||((y == 1) && (c[i-1] == 1)))
         c[i] = 1 ;
       else 
         c[i]=0;
    opx = opx >> 1 ;
    opy = opy >> 1 ;
  }

   if (c[4]==0)
     cout = 0 ;
   else
     cout = 1 ;
}

/*** Signe ***/

void calculSigne(void)
{
 if((res & 0x8) == 0)
   signe = 0 ;
 else
   signe = 1 ;
}

/*** Depassement de capacite ***/

void calculOvr(void)
{
 if (c[4] ^ c[3]) 
   ovr = 1 ;
 else 
   ovr = 0 ;
}

/*** Resultat d'une operation ***/

void calculResultat(void)
{
 switch(op)
   {
   case RplusS   : res = (r + s) % 16 ;
                   break   ;
   case SmoinsR  : res = ((s - r) & 0xf) % 16 ;
                   break ;
   case RmoinsS  : res = ((r - s) & 0xf) % 16 ;
                   break ;
   case RouS     : res = r | s ;
                   break ;
   case RetS     : res = r & s ;
                   break ;
   case nonRetS  : res = (~(r) & s) & 0xf ;
                   break ;
   case RouxS    : res = r ^ s ;
                   break ;
   case nonRouxS : res = (~(r ^ s)) & 0xf ;
                   break ;
   default       : break ;
   }
}

/*** Sorties ***/

void calculSortie(void)
{
 char *chaineEntier ;
 int temp ;

 chaineEntier = (char *) mbkalloc (32 * sizeof (char));

 sprintf(chaineEntier,"%i",mcode) ; 
 temp = (int)strtol(chaineEntier,(char **)NULL,8) ; 
 free(chaineEntier) ;

 temp = (temp & 0x1c0) >> 6 ;

 switch(temp)
   {
   case 2  : y = sa  ;
             break   ;
   default : y = res ;
             break   ;
   }
}

/*** Code operatoire ***/

void calculCodeOp(void)
{
 char *chaineEntier ;
 int temp ;

 chaineEntier = (char *) mbkalloc (32 * sizeof (char));

 sprintf(chaineEntier,"%i",mcode) ; 
 temp = (int)strtol(chaineEntier,(char **)NULL,8) ; 
 free(chaineEntier) ;

 temp = (temp & 0x38) >> 3 ;

 switch(temp)
   {
   case 0 : op = RplusS ;
            break ;
   case 1 : op = SmoinsR ;
            break ;
   case 2 : op = RmoinsS ;
            break ;
   case 3 : op = RouS ;
            break ;
   case 4 : op = RetS ;
            break ;
   case 5 : op = nonRetS ;
            break ;
   case 6 : op = RouxS ;
            break ;
   default : op = nonRouxS ;
             break ;
   }
}

void operation(long vect)
{
 calculResultat() ;
 calculSigne() ; 

 calculCout() ;
 calculOvr() ;
 calculZero() ;

 if((op == RplusS) || (op == SmoinsR) || (op == RmoinsS))
   {
    AFFECT(entierVersChaine(vect),"ovr",entierVersChaine(ovr)) ;
    AFFECT(entierVersChaine(vect),"zero",entierVersChaine(zero)) ;
    AFFECT(entierVersChaine(vect),"signe",entierVersChaine(signe)) ; 
    AFFECT(entierVersChaine(vect),"cout",entierVersChaine(cout)) ;
   }
 else
   {
    AFFECT(entierVersChaine(vect),"ovr",entierVersChaine(0)) ;
    AFFECT(entierVersChaine(vect),"zero",entierVersChaine(zero)) ;
    AFFECT(entierVersChaine(vect),"signe",entierVersChaine(signe)) ; 
    AFFECT(entierVersChaine(vect),"cout",entierVersChaine(0)) ;
   }
}

void sortie(long vect)
{
 calculSortie() ;

 AFFECT(entierVersChaine(vect),"y",entierVersChaine(y)) ;
}

void codeOp(long vect)
{
 calculCodeOp() ;

 AFFECT(entierVersChaine(vect),"i",octVersChaine(mcode)) ; 
}

void entree(long vect)
{
 AFFECT(entierVersChaine(vect),"scin",entierVersChaine(scin)) ;
 AFFECT(entierVersChaine(vect),"a",entierVersChaine(a)) ;
 AFFECT(entierVersChaine(vect),"b",entierVersChaine(b)) ;
 AFFECT(entierVersChaine(vect),"d",entierVersChaine(d)) ;
 AFFECT(entierVersChaine(vect),"noe",entierVersChaine(noe)) ;
 /*  AFFECT(entierVersChaine(vect),"r0",entierVersChaine(r0)) ; */
 AFFECT(entierVersChaine(vect),"r3",entierVersChaine(r3)) ;
 /*  AFFECT(entierVersChaine(vect),"q0",entierVersChaine(q0)) ; */
 AFFECT(entierVersChaine(vect),"q3",entierVersChaine(q3)) ;
 AFFECT(entierVersChaine(vect),"cin",entierVersChaine(cin)) ;

}


/**********************************************************************************************/
/********************************* Fonction principale ****************************************/
/**********************************************************************************************/

int main()
{
 int i ;
 int x ;
 int z ;

 DEF_GENPAT ("pattern");

 DECLAR ("a",":1","X",IN,"3 DOWNTO 0","");
 DECLAR ("b",":1","X",IN,"3 DOWNTO 0","");
 DECLAR ("d",":1","X",IN,"3 DOWNTO 0","");
 DECLAR ("i",":1","O",IN,"8 DOWNTO 0","");
 DECLAR ("fonc",":1","B",IN,"","");
 DECLAR ("test",":1","B",IN,"","");
 DECLAR ("scin",":1","B",IN,"","");
 DECLAR ("noe",":1","B",IN,"","");
 DECLAR ("ck",":1","B",IN,"","");
 DECLAR ("cin",":1","B",IN,"","");
 DECLAR ("r0",":1","B",INOUT,"","");
 DECLAR ("r3",":1","B",INOUT,"","");
 DECLAR ("q0",":1","B",INOUT,"","");
 DECLAR ("q3",":1","B",INOUT,"","");
 DECLAR ("y",":1","X",OUT,"3 DOWNTO 0","");
 DECLAR ("zero",":1","B",OUT,"","");
 DECLAR ("signe",":1","B",OUT,"","");
 DECLAR ("scout",":1","B",OUT,"","");
 DECLAR ("ovr",":1","B",OUT,"","");
 DECLAR ("np",":1","B",OUT,"","");
 DECLAR ("ng",":1","B",OUT,"","");
 DECLAR ("cout",":1","B",OUT,"","");
 DECLAR ("vdd",":1","B",IN,"","");
 DECLAR ("vss",":1","B",IN,"","");
 DECLAR ("vdde",":1","B",IN,"","");
 DECLAR ("vsse",":1","B",IN,"","");


 vct = 0 ;



 /****************** Initialiation ******************/ 
 AFFECT (entierVersChaine(vct),"a","0b0000");
 AFFECT (entierVersChaine(vct),"b","0b0000");
 AFFECT (entierVersChaine(vct),"fonc","0b1");
 AFFECT (entierVersChaine(vct),"test","0b0");
 AFFECT (entierVersChaine(vct),"scin","0b0");
 AFFECT (entierVersChaine(vct),"noe","0b0");
 AFFECT (entierVersChaine(vct),"cin","0b0");
 AFFECT (entierVersChaine(vct),"vdd","1") ;
 AFFECT (entierVersChaine(vct),"vss","0") ;
 AFFECT (entierVersChaine(vct),"vdde","1") ;
 AFFECT (entierVersChaine(vct),"vsse","0") ;
 AFFECT (entierVersChaine(vct),"i","0b0000") ;
 AFFECT (entierVersChaine(vct),"d","0b0000") ;
 AFFECT (entierVersChaine(vct),"ck","1") ;
 AFFECT (entierVersChaine(vct),"scout","?0b*") ;
 /***************************************************/




 /*****************************************/
 /***** Ecriture dans le registre Q *******/
 /*****************************************/

 /* Ecriture de 1010 */

 /* 1 **************/
 AFFECT (entierVersChaine(vct),"i",octVersChaine(007));
 AFFECT (entierVersChaine(vct),"d","0b1010");

 AFFECT (entierVersChaine(vct),"y","0b1010");
 AFFECT (entierVersChaine(vct),"signe","0b1");

 coupHorloge(1) ;

 /* Ecriture de 0101 */

 /* 2 **************/

 AFFECT (entierVersChaine(vct),"i","0o007");
 AFFECT (entierVersChaine(vct),"d","0b0101");
 AFFECT (entierVersChaine(vct),"y","0b0101");
 AFFECT (entierVersChaine(vct),"signe","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct),"y","0b0101");
 AFFECT (entierVersChaine(vct),"signe","0b0");

 /* Lecture de l'accu */

 /* 3 **************/

 AFFECT (entierVersChaine(vct),"i","0o032");
 AFFECT (entierVersChaine(vct),"d","0b1010");
 AFFECT (entierVersChaine(vct),"y","0b0101");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct),"y","0b0101");
 AFFECT (entierVersChaine(vct),"signe","0b0");



 /*****************************************************************/
 /***** Decalage droite du registre Q et ecriture dans la ram *****/
 /*****************************************************************/

 /* 4 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0000");
 AFFECT (entierVersChaine(vct),"r3","0b1");
 AFFECT (entierVersChaine(vct),"q3","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");

 /* 5 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0001");
 AFFECT (entierVersChaine(vct),"r3","0b0");
 AFFECT (entierVersChaine(vct),"q3","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");


 /* 6 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0010");
 AFFECT (entierVersChaine(vct),"r3","0b1");
 AFFECT (entierVersChaine(vct),"q3","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");


 /* 7 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0011");
 AFFECT (entierVersChaine(vct),"r3","0b0");
 AFFECT (entierVersChaine(vct),"q3","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");


 /* 8 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0100");
 AFFECT (entierVersChaine(vct),"r3","0b1");
 AFFECT (entierVersChaine(vct),"q3","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");


 /* 9 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0101");
 AFFECT (entierVersChaine(vct),"r3","0b0");
 AFFECT (entierVersChaine(vct),"q3","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");


 /* 10 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0110");
 AFFECT (entierVersChaine(vct),"r3","0b1");
 AFFECT (entierVersChaine(vct),"q3","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");


 /* 11 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b0111");
 AFFECT (entierVersChaine(vct),"r3","0b0");
 AFFECT (entierVersChaine(vct),"q3","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");

 /* 12 **************/

 AFFECT (entierVersChaine(vct),"i","0o462");
 AFFECT (entierVersChaine(vct),"b","0b1000");
 AFFECT (entierVersChaine(vct),"r3","0b1");
 AFFECT (entierVersChaine(vct),"q3","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");

 /*****************************************************/
 /***** Lecture de ce qui a ete ecrit dans la RAM *****/
 /*****************************************************/

 /* 12 **************/

 AFFECT (entierVersChaine(vct),"i","0o163");

 for(i = 0 ; i < 7 ; i = i+2)
   {
    AFFECT (entierVersChaine(vct),"b",entierVersChaine(i));
    AFFECT (entierVersChaine(vct),"y","0b1010");
    AFFECT (entierVersChaine(vct),"signe","0b1");

    coupHorloge(1) ;

    AFFECT (entierVersChaine(vct),"b",entierVersChaine(i+1));
    AFFECT (entierVersChaine(vct),"y","0b0101");
    AFFECT (entierVersChaine(vct),"signe","0b0");

    coupHorloge(1) ;
   }





 /*****************************************************************/
 /***** Decalage gauche du registre Q et ecriture dans la ram *****/
 /*****************************************************************/


 /* Ecriture de 0101 */

 /*   **************/


 AFFECT (entierVersChaine(vct),"r3","?0b*");
 AFFECT (entierVersChaine(vct),"q3","?0b*");
 AFFECT (entierVersChaine(vct),"r0","?0b*");
 AFFECT (entierVersChaine(vct),"q0","?0b*");


 /* Ecriture de 1010 */

 /* 1 **************/
 AFFECT (entierVersChaine(vct),"i",octVersChaine(007));
 AFFECT (entierVersChaine(vct),"d","0b1010");

 AFFECT (entierVersChaine(vct),"y","0b1010");
 AFFECT (entierVersChaine(vct),"signe","0b1");

 coupHorloge(1) ;

 /* Ecriture de 0101 */

 /* 2 **************/

 AFFECT (entierVersChaine(vct),"i","0o007");
 AFFECT (entierVersChaine(vct),"d","0b0101");
 AFFECT (entierVersChaine(vct),"y","0b0101");
 AFFECT (entierVersChaine(vct),"signe","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct),"y","0b0101");
 AFFECT (entierVersChaine(vct),"signe","0b0");

 /* Lecture de l'accu */

 /* 3 **************/

 AFFECT (entierVersChaine(vct),"i","0o032");
 AFFECT (entierVersChaine(vct),"d","0b1010");
 AFFECT (entierVersChaine(vct),"y","0b0101");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct),"y","0b0101");
 AFFECT (entierVersChaine(vct),"signe","0b0");



 /*   **************/
 /*   **************/


 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0000");
 AFFECT (entierVersChaine(vct),"r0","0b0");
 AFFECT (entierVersChaine(vct),"q0","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");

 /*   **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0001");
 AFFECT (entierVersChaine(vct),"r0","0b1");
 AFFECT (entierVersChaine(vct),"q0","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");


 /*   **************/


 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0010");
 AFFECT (entierVersChaine(vct),"r0","0b0");
 AFFECT (entierVersChaine(vct),"q0","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");


 /*   **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0011");
 AFFECT (entierVersChaine(vct),"r0","0b1");
 AFFECT (entierVersChaine(vct),"q0","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");


 /*   **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0100");
 AFFECT (entierVersChaine(vct),"r0","0b0");
 AFFECT (entierVersChaine(vct),"q0","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");


 /*   **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0101");
 AFFECT (entierVersChaine(vct),"r0","0b1");
 AFFECT (entierVersChaine(vct),"q0","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");


 /*    **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0110");
 AFFECT (entierVersChaine(vct),"r0","0b0");
 AFFECT (entierVersChaine(vct),"q0","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");


 /*    **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b0111");
 AFFECT (entierVersChaine(vct),"r0","0b1");
 AFFECT (entierVersChaine(vct),"q0","0b1");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b0101");
 AFFECT (entierVersChaine(vct-interval),"signe","0b0");

 /*    **************/

 AFFECT (entierVersChaine(vct),"i","0o662");
 AFFECT (entierVersChaine(vct),"b","0b1000");
 AFFECT (entierVersChaine(vct),"r0","0b0");
 AFFECT (entierVersChaine(vct),"q0","0b0");

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct-interval),"y","0b1010");
 AFFECT (entierVersChaine(vct-interval),"signe","0b1");

 /*****************************************************/
 /***** Lecture de ce qui a ete ecrit dans la RAM *****/
 /*****************************************************/

 /* 12 **************/

 AFFECT (entierVersChaine(vct),"i","0o163");

 for(i = 0 ; i < 7 ; i = i+2)
   {
    AFFECT (entierVersChaine(vct),"b",entierVersChaine(i));
    AFFECT (entierVersChaine(vct),"y","0b1010");
    AFFECT (entierVersChaine(vct),"signe","0b1");

    coupHorloge(1) ;

    AFFECT (entierVersChaine(vct),"b",entierVersChaine(i+1));
    AFFECT (entierVersChaine(vct),"y","0b0101");
    AFFECT (entierVersChaine(vct),"signe","0b0");

    coupHorloge(1) ;
   }


 /*****************************************************************/
 /***** Lectures et ecritures de la ram ***************************/
 /*****************************************************************/

 AFFECT (entierVersChaine(vct),"signe","?0b*");

 for(i = 0 ; i < 16 ; i++)
   {
    /* Ecriture aux adresses a et b */
    AFFECT (entierVersChaine(vct),"i","0o337");
    AFFECT (entierVersChaine(vct),"d",entierVersChaine(i));
    AFFECT (entierVersChaine(vct),"a",entierVersChaine(i));
    AFFECT (entierVersChaine(vct),"b",entierVersChaine(i));
    AFFECT (entierVersChaine(vct),"y",entierVersChaine(i));
    coupHorloge(1) ;
   }

 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct),"i","0o137");
 AFFECT (entierVersChaine(vct),"d","0b0000");
 AFFECT (entierVersChaine(vct),"a","0b0000");
 AFFECT (entierVersChaine(vct),"b","0b0000");
 
 AFFECT (entierVersChaine(vct),"y","?0b****");
 coupHorloge(1) ;

 /* Lecture a l'adresse a */
 for(i = 0 ; i < 16 ; i++)
   {
    AFFECT (entierVersChaine(vct),"i","0o134");
    AFFECT (entierVersChaine(vct),"a",entierVersChaine(i));
    AFFECT (entierVersChaine(vct),"y",entierVersChaine(i));
    coupHorloge(1) ;
   }

 AFFECT (entierVersChaine(vct-interval),"y","?0b****");
 coupHorloge(1) ;

 AFFECT (entierVersChaine(vct),"i","0o137");
 AFFECT (entierVersChaine(vct),"d","0b0000");
 AFFECT (entierVersChaine(vct),"a","0b0000");
 AFFECT (entierVersChaine(vct),"b","0b0000");
  AFFECT (entierVersChaine(vct),"y","?0b****");
 coupHorloge(1) ;



 /* Lecture a l'adresse b */

 for(i = 0 ; i < 16 ; i++)
   {
    AFFECT (entierVersChaine(vct),"i","0o133");
    AFFECT (entierVersChaine(vct),"b",entierVersChaine(i));

    coupHorloge(1) ;


    AFFECT (entierVersChaine(vct-interval),"y",entierVersChaine(i));

    coupHorloge(1) ;

    AFFECT (entierVersChaine(vct-interval),"y","?0b****");
   }

 /*****************************************************************/
 /***** Operations arithmetiques et booleennes ********************/
 /*****************************************************************/

 
 scin  = 0 ;
 scout = 0 ;
 a     = 0 ;
 b     = 0 ;
 d     = 0 ;
 noe   = 0 ;
 r0    = 0 ;
 r3    = 0 ;
 q0    = 0 ;
 q3    = 0 ;
 ovr   = 0 ;
 zero  = 0 ;
 signe = 0 ;
 cin   = 0 ;
 y     = 0 ;
 
 r     = 0   ;
 s     = 0   ;
 sa    = 0   ;
 sb    = 0   ;
 sq    = 0   ;
 mcode = 107 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0  D+0  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }

 
 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 cin   = 1   ;
 mcode = 117 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0  0-D  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }

 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 cin   = 1   ;
 mcode = 127 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0  D-0  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }
 
 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 cin   = 1   ;
 mcode = 127 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0  D-0  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }
 
 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 cin   = 0   ;
 mcode = 167 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0   D ou exclusif 0  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }
 
 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 cin   = 0   ;
 mcode = 167 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0   D ou exclusif 0  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }
 
 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 cin   = 0   ;
 mcode = 177 ;
 
 codeOp(vct) ;
 
 for(i = 0 ; i < 16 ; i++)   /* D,0   non D ou exclusif 0  */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }
 
 /* Chargement de 5 dans l'accu */
 d     = 5   ;
 r     = 5   ;
 s     = 0   ;
 sa    = 5   ;
 cin   = 0   ;
 mcode = 7   ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 3 ; i++)   /* D,0   D + 0 */
   {
    r = d ;
    s = 0 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
 
    coupHorloge(1) ;
   }
 
 d     = 0   ;
 r     = 0   ;
 s     = 0   ;
 sa    = 5   ;
 cin   = 0   ;
 mcode = 106 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0   D + Q avec ovf et tout le tintoin  */
   {
    r = d ;
    s = sa ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    d++ ;
 
    coupHorloge(1) ;
   }
 d     = 4   ;
 r     = 0   ;
 s     = 0   ;
 sa    = 5   ;
 cin   = 1   ;
 mcode = 116 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0   Q - D avec ovf et tout le tintoin  */
   {
    r = d ;
    s = 5 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    if (d < 15)
	d++ ;
    else 
	d = 0;
 
    coupHorloge(1) ;
   }
 
 d     = 8   ;
 r     = 0   ;
 s     = 0   ;
 sa    = 5   ;
 cin   = 1   ;
 mcode = 126 ;
 
 codeOp(vct) ;
 for(i = 0 ; i < 16 ; i++)   /* D,0   D - Q avec ovf et tout le tintoin  */
   {
    r = d ;
    s = 5 ;
    entree(vct) ;
    operation(vct) ;
    sortie(vct) ;
    if (d < 15)
	d++ ;
    else 
	d = 0;
 
    coupHorloge(1) ;
   }

 SAV_GENPAT ();
 exit(0);
}
 
 
 
