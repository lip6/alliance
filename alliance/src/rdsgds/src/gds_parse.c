/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSGDS                         |
|                                                             |
| File    :                  gds_parse.c                      |
|                                                             |
| Authors :                 Pierre Vittet                     |
|                    Modified by Jacomme Ludovic              |
| Date    :                   04/07/93                        |
|                         Modified by Frederic Petrot:        |
|              now reads text and is endiannes independant    |
| Date    :                   05/04/96                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include <string.h>
# include <malloc.h>

# include <mut.h>
# include <mph.h>
# include <rds.h>
# include <rtl.h>
# include <rpr.h>
# include <rfm.h>
# include <rwi.h>
# include <rut.h>

# include "gds.h"
# include "gds_error.h"
# include "gds_parse.h"
# include "gds_swap.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/

static double    pv_scale;    /* facteur d'echelle pour la correspondance*/
                              /* des coordonnees entre GDS et RDS.   */
static rdsfig_list *pv_model; /* modele en cours d'elaboration.      */
static int ErrNo;

static node_list *HEAD_NODE;  /* text list in the GDS file */

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

/* These functions are moreorless verbatim copies of the CIF ones, written by
   Ludovic Jacomme.
   I've made slight modification for GDS adaptation.
   Fred */

/*------------------------------------------------------------\
|                                                             |
|                         Gds Add Node                        |
|                                                             |
\------------------------------------------------------------*/

  static node_list *GdsAddNode( Name, X, Y, Layer )

    char *Name;
    long  X;
    long  Y;
    char  Layer;
  {
    node_list *Node;

    Node = (node_list *)rdsalloc( sizeof(node_list), RDS_ALLOC_HEAP );

    Node->NEXT  = HEAD_NODE;
    Node->NAME  = Name;
    Node->X     = X;
    Node->Y     = Y;
    Node->LAYER = Layer;
    HEAD_NODE   = Node;

    return( Node );
  }

/*------------------------------------------------------------\
|                                                             |
|                         Gds Free Node                       |
|                                                             |
\------------------------------------------------------------*/

  static void GdsFreeNode()
  {
    node_list *Scan;
    node_list *Del;

    Scan = HEAD_NODE;

    while ( Scan != (node_list *)NULL )
    {
      Del  = Scan;
      Scan = Scan->NEXT;

      rdsfreeheap( Del, sizeof( node_list ) );
    }

    HEAD_NODE = (node_list *)NULL;
  }

/*------------------------------------------------------------\
|                                                             |
|                      Gds Get Rectangle                      |
|                                                             |
\------------------------------------------------------------*/

  static rdsrec_list *GdsGetRectangle( Figure, X, Y, Layer )

     rdsfig_list  *Figure;
     long          X;
     long          Y;
     char          Layer;
  {
    rdsrec_list *Scan;

    for ( Scan  = Figure->LAYERTAB[ (int)Layer ];
          Scan != (rdsrec_list * )NULL;
          Scan  = Scan->NEXT)
    {
      if ( ( X >= Scan->X            ) &&  
           ( Y >= Scan->Y            ) &&  
           ( X <= Scan->X + Scan->DX ) &&
           ( Y <= Scan->Y + Scan->DY ) &&
           ( Scan->NAME == NULL      ) &&
           IsRdsReference (Scan))
      {
        return Scan;
      }
    }
    for ( Scan  = Figure->LAYERTAB[ (int)Layer ];
          Scan != (rdsrec_list * )NULL;
          Scan  = Scan->NEXT)
    {
      if ( ( X >= Scan->X            ) &&  
           ( Y >= Scan->Y            ) &&  
           ( X <= Scan->X + Scan->DX ) &&
           ( Y <= Scan->Y + Scan->DY ) ) 
      {
        return Scan;
      }
    }
    return ( (rdsrec_list * )NULL );
  }
 

/*******************************************************************************
*                                                                              *
*  REMARQUE IMPORTANTE :                                                       *
*                                                                              *
*  Tous les reels, simple ou double precision, dans un fichier GDS sont        *
*  exprimes en base 16. Ne pas oublier d'en tenir compte lors de l'uti-        *
*  lisation de variables de ce type.                                           *
*                                                                              *
*******************************************************************************/
/***********
*
*  Preliminaires sur les nombres flottants en GDS2.
*
*  Un flottant gds est represente par :
*  - 1 bit de signe     |  ces 8 bits occupent le 1er octet
*  - 7 bits d'exposant biaise a 64  |
*  - 24 ou 56 bits de mantisse selon la precision (les 3,7 octets suivants)
*
*  SEEEEEEE MMMMMMMM MMMMMMMM MMMMMMMM
*  SEEEEEEE MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM MMMMMMMM
*
*  L'exposant doit etre diminue de 64 pour avoir sa valeur reelle.
*  Decrit sur 7 bits, cela implique que l'exposant reel est compris entre -64
*  et +63 inclus.
*  Il s'applique a une puissance de 16.
*
*  Donc la valeur absolue du flottant correspondant a ce format est egal a :
*
*     mantisse * (16 ^ [exposant -64])
*
*  La mantisse est telle que   1/16 <= mantisse < 1 (du fait de la base 16).
*
*  En consequence de quoi, la valeur absolue d'un flottant GDS est comprise
*  entre
*     1/16.16^(-64)    et  ~16^63
*  donc entre
*     5.397605347e-79  et  7.237005577e75
*
*  A ce domaine, il faut ajouter la valeur 0 qui se represente par "tout a zero"
*
***/

/*
*  POW, a replacement for -lm function
*/
static double
   mypow(n, exposant)
double n, exposant;
{
   int i;
   double res;

   res = 1.0;
   if (exposant > 0)
      for (i = 0; i < exposant; i++)
         res *= n;
   else  for (i = 0; i < -exposant; i++)
         res /= n;
   return (res);
}

/***********
*
*  GDSREAL_TO_DOUBLE
*
*  Cette fonction prend un nombre flottant exprime, en logique GDS2, sur 4 ou  8
*  octets (ce qui correspond a simple ou double precision) et retourne un nombre
*  flottant en double precision ("long float") propre a l'ordinateur.
*
*  ENTREE : pointeur sur la chaine de caracteres contenant le flottant
*     taille de la chaine (4 ou 8)
*
*  RETOUR : nombre flottant correspondant
*
**/
static double
   pv_gdsreal_to_double(value_str,size)
char  *value_str;
short size;
{
   register int i, j, k;
   short   signe;
   short   exposant = 0;
   double  mantisse = 0;
   char    buffer[64];

      /* On va commencer par mettre tous les bits du gdsreal dans un */
   k = 0;   /* tableau de caracteres ou chaque caractere represente un bit.   */
   for (i = 0; i < size; i++) /* Pour les 4 (ou 8) octets   */
      for (j = 7; j >= 0; j--)/* Pour chaque bit de l'octet */
         buffer[k++] = (((value_str[i] >> j) & 0x0001) != 0) ? '1' : '0'; /* Stocker sa valeur dans le tableau */

   signe = (buffer[0] == '0') ? 1 : -1;
   for (i = 1; i < 8; i++)
      if (buffer[i] != '0')   /* sous-entendu == '1'     */
         exposant += 1 << (7-i);
   exposant -= 64;         /* pour oter le biaisage      */
   for (i = 8; i < k; i++)
      if (buffer[i] != '0')   /* sous-entendu == '1'     */
         mantisse += mypow(2.0, 7.0-i);

   return(signe * mantisse * mypow(16.0,(double)exposant));
}

/***********
*
*  APPROXIMATION
*
*  Cette fonction est utilisee pour savoir si une valeur est l'approximation
*  d'une autre valeur a Epsilon pres. Elle sera surtout utilisee pour savoir
*  si un un nombre flottant, qui a ete passe d'une representation a l'autre,
*  est egale a un autre nombre (generalement une constante) dans un test. Il
*  serait en effet dommage qu'un nombre, initialement a 1, prenne une  autre
*  valeur, meme tres proche (1.000001), lors du tranfert et qu'on ne  puisse
*  plus constater son egalite a 1, ce qui nous induirait en erreur.
*
***/

#define EPSILON 1.0e-3  /* definition de l'approximation permise */

static int
    pv_approximation(val1, val2)
double val1, val2;
{
   if (val1 < val2 - EPSILON) return 0;
   if (val1 > val2 + EPSILON) return 0;
   return 1;
}

#undef EPSILON

/***********
*
*         GENERE_NOM
*
*   Cette fonction genere un nom unique a partir d'un motif de caracteres passe
*   comme parametre.
*
*  ENTREE : motif de caracteres
*
*  RETOUR : pointeur sur le nom genere
*         NULL si manque d'espace memoire
*
***/

static char *pv_genere_nom(motif)
char *motif;
{
   static int  suffixe = 0;
   register char  *pt;

   /* 10 is the length of the biggest integer in ascii on a 32 bit machine */
   pt = (char *)mbkalloc((strlen(motif) + 1 + 10) * sizeof(char));
   (void)sprintf(pt, "%s_%d", motif, suffixe);
   suffixe++;
   return(pt);
}


/***********
*
*  GDS_TO_RDS_SYM
*
* Cette fonction prend certaines operations de transformation graphique GDS
* appliquees aux instances et retourne leur equivalent dans la logique RDS.
*
*  ENTREE : flag indiquant si il y a une reflection par rapport a l'axe des x
*     (symetrie en y) appliquee avant une eventuelle rotation.
*     angle de rotation
*
*  SORTIE : caractere indiquant l'operation correspondante en RDS
*     -1 si operation inconnue. (Souvent si angle non droit)
*
*  REMARQUE : Etant donne que l'angle est passe comme un  flottant  et  qu'il
*     est caste dans un long afin d'etre traduit en RDS  (RDS ne traite
*     que les angles droits ou plats donc entiers), on va admettre  que
*     cette transformation peut introduire des erreurs dans une  valeur
*     d'angle qui serait au depart entiere (on ne parle pas  evidemment
*     d'angles non entiers qui ne peuvent que perdre en precision).  On
*     regardera si l'angle obtenu est un multiple de 90' a EPSILON pres
*
***/

#define EPSILON 1L

static int
   pv_gds_to_rds_sym(ref_x_axis, angle)
short ref_x_axis;
double   angle;
{
   long  ang;

   ang = (long)angle;
   ang %= 360; /* On ramene l'angle a des valeurs comprises entre 0 et 360'         */
         /* On augmente l'angle de EPSILON afin d'avoir le resultat du modulo compris  */
   if (((ang + EPSILON) % 90) > (2 * EPSILON))
      return(-1); /* entre 0 et 2 x EPSILON pour des angles multiples de 90 a EPSILON pres   */
   switch ((ang + EPSILON) / 90) {
   case 0 : /* Apres avoir determine si l'angle etait a peu pres multiple de 90, on fixe */
   case 4 : ang = 0;/* sa valeur au multiple le plus proche.   */
      break;
   case 1 : ang = 90;
      break;
   case 2 : ang = 180;
      break;
   case 3 : ang = 270;
      break;
   }
   if (ref_x_axis)
      switch (ang) {
      case 0   : return(RDS_SYM_Y);
      case 90  : return(RDS_SY_RP);
      case 180 : return(RDS_SYM_X);
      case 270 : return(RDS_SY_RM);
   } else switch (ang) {
      case 0   : return(RDS_NOSYM);
      case 90  : return(RDS_ROT_P);
      case 180 : return(RDS_SYMXY);
      case 270 : return(RDS_ROT_M);
   }
   return(-1); /* aucune raison d'arriver ici. */
}

#undef EPSILON
/**************************
*
* This make the conversion between a gds layer and an internal rds layer
* based upon the specified technological file.
*
**************/
static int
   pv_gdslayer_to_symb(layer)
int layer;
{
int rds_layer;
char texte[8];

   if (layer != -1)
      for (rds_layer = 0; rds_layer < RDS_MAX_LAYER; rds_layer++) {
         if (GET_GDS_LAYER(rds_layer) == layer
               || GET_GDS_CONNECTOR_LAYER(rds_layer) == layer)
            return rds_layer;
      }
   (void)sprintf(texte, "%d", layer);
   pv_emet_warning(pv_model->NAME, "Unknown GDS layer in technology file :", texte);
   return -1;
}

static int
   pv_gdslayer_iscon(layer)
int layer;
{
int rds_layer;
char texte[8];

   if (layer != -1)
      for (rds_layer = 0; rds_layer < RDS_MAX_LAYER; rds_layer++) {
         if (GET_GDS_CONNECTOR_LAYER(rds_layer) == layer)
            return 1;
      }
   return 0;
}


/*********
*
*  Un decalage de 3 a droite equivaut a une division par 8.
*  Sert a determiner le nombre de paires de coordonnees (x,y) a partir du nombre
*  d'octets elles occupent (chaque paire occupe 8 octets i.e. 2 entiers longs).
*
***/
#define NSHIFT 3  /* LOG2(sizeof(coord_t))*/

#define TRASHSIZE 4096  /* taille de la zone ou on va mettre tout ce qui ne nous interesse pas */


/***********
*
*  MACRO LECTUREx
*
* Ces macros sont utilisees pour effectuer et controler la lecture du fichier GDS.
*
* Si error passe a TRUNCERR, c'est que la fin du fichier est arrivee prematurement :
* on n'a pas pu lire autant d'octets qu'il en etait prevu par t
*
*        0 <= retour < t ==> error = TRUNCERR
*
* remarque : par contre, si retour < 0 c'est qu'il y a eu une erreur d'E/S.
*
***/

#define lecture1(z,t)                                \
         if (t > 0)                               \
            retour = fread((char *)z, 1, t, fp); \
         if (retour == 0) {                       \
            if (ErrNo == 0)                      \
               error = TRUNCERR;                \
            break;                               \
         }                                        \

#define lecture2(z,t)                                \
         if (t > 0)                               \
            retour = fread((char *)z, 1, t, fp); \
         if ((retour == 0) && (ErrNo == 0))       \
            error = TRUNCERR;                    \

/*********
*
*  MACRO TRAITEBOOL
*
* La macro suivante permet d'appeler la fonction pv_set_pv_error avec le bon parametre.
*
* Si a est different de 0, c'est qu'il y a eu une erreur autre que d'E/S. a contient
* alors le numero d'erreur.
* Si a = 0 et ErrNo != 0 ==> erreur d'E/S
* Si a = 0 et ErrNo = 0  ==> on a atteint la fin de fichier prematurement et ceci
* alors qu'on se trouvait dans la partie "controle" de la boucle WHILE (donc "a"
* n'a pas encore pu etre positionne a la valeur TRUNCERR)
*
*  SI a != 0 ALORS pv_set_pv_error(a)
*  SINON SI ErrNo != 0 ALORS pv_set_pv_error(IOERR)
*  SINON pv_set_pv_error(TRUNCERR)
*
***/

#define traitebool(a) if (a != 0) pv_set_pv_error(a) ; else if (ErrNo != 0) pv_set_pv_error(IOERR) ; else pv_set_pv_error(TRUNCERR)

/***********
*
*  SKIP_STRUCTURE
*
*  Cette fonction est utilisee pour passer tout le code correspondant a un modele
*  loin, a l'endroit ou commence la structure (modele en RDS) suivante.
*
*  ENTREE : pointeur de fichier ouvert
*
*  RETOUR : 0 si tout va bien
*     -1 sinon
*
***/

static int
   pv_skip_structure(fp)
FILE *fp;
{
   register int   retour;
   hinfo_type     infobuf;
   int      error = FALSE;
   char     poubelle[TRASHSIZE];
   static char texte[64];

   while ((retour = fread((char *)&infobuf, sizeof(hinfo_type), 1, fp)) == 1) {
      if (islittle()) {
         infobuf.gdscode = swaps(infobuf.gdscode);
         infobuf.size    = swaps(infobuf.size);
      }
      switch (infobuf.gdscode) {
      case TEXT        :
      case BOUNDARY    :
      case PATH        :
      case SREF        :
      case AREF        :
      case LAYER0      :
      case DATATYPE    :
      case WIDTH       :
      case XY          :
      case ENDEL       :
      case SNAME       :
      case COLROW      :
      case NODE        :
      case TEXTTYPE    :
      case PRESENTATION:
      case STRING      :
      case STRANS      :
      case MAG         :
      case ANGLE       :
      case PATHTYPE    :
      case ELFLAGS     :
      case ELKEY       :
      case NODETYPE    :
      case PROPATTR    :
      case PROPVALUE   :
      case BOX         :
      case BOXTYPE     :
      case PLEX        :
      case BGNEXTN     :
      case ENDEXTN     :
         lecture2(poubelle, infobuf.size - sizeof(hinfo_type));
         break;
      case ENDSTR :
         return(0);
      default :
         pv_init_error();
         pv_error.v_error = ENOCODE;
         error = TRUE;
         break;
      }  /* fin du SWITCH */

   /* on eu une erreur, donc on sort du while prematurement */
      if ((retour < 1) || error) break;

   }  /* fin du WHILE */

   traitebool(error);        
   sprintf(texte, "skip_structure(dernier code = %04X)", infobuf.gdscode);
   pv_give_error(texte);
   return(-1);
}


/***********
*
*  EVITE_ELEMENT
*
* Cette fonction passe un "element" GDS qui n'a pas son equivalent dans la BD rds.
*
* Retour : -1 si on a eu une erreur
*      0 si tout s'est bien passe
*
***/

static int
   pv_evite_element(fp, type_element)
FILE *fp;
int type_element;
{
   register int   retour;
   hinfo_type     infobuf;
   int      error = FALSE;
   char     poubelle[TRASHSIZE];
   char     *texte;
   static char *name[] = {
      "Illegal type name!",
      "Unable to parse aref (array of subcells)",
      "Unable to parse box (regular geometry)",
      "Unable to parse node (electrical net)",
      "Unable to parse path (wire)",
      "Unable to parse text (documentation)",
   };

   switch (type_element) {
   case AREF : texte = name[1];
      break;
   case BOX :  texte = name[2];
      break;
   case NODE : texte = name[3];
      break;
   case PATH : texte = name[4];
      break;
   case TEXT : texte = name[5];
      break;
   default :   texte = name[0];
      break;
   }

   pv_emet_warning(pv_model->NAME, texte,(char *)NULL);

   while ((retour = fread((char *)&infobuf, sizeof(hinfo_type), 1, fp)) == 1) {
      if (islittle()) {
         infobuf.gdscode = swaps(infobuf.gdscode);
         infobuf.size    = swaps(infobuf.size);
      }
      switch (infobuf.gdscode) {
      case TEXTTYPE    :
      case BOXTYPE     :
      case PATHTYPE    :
      case DATATYPE    :
      case NODETYPE    :
      case PRESENTATION:
      case WIDTH       :
      case STRANS      :
      case MAG         :
      case ANGLE       :
      case STRING      :
      case BGNEXTN     :
      case ENDEXTN     :
      case LAYER0      :
      case ELFLAGS     :
      case PLEX        :
      case XY          :
      case SNAME       :
      case COLROW      :
      case ELKEY       :
      case PROPATTR    :
      case PROPVALUE   :
         lecture2(poubelle, infobuf.size - sizeof(hinfo_type));
         break;
      case ENDEL :
         return(0);
      default :
         pv_init_error();
         pv_error.v_error = ENOCODE;
         error = TRUE;
         break;
      }  /* fin du SWITCH */

   /* on eu une erreur, donc on sort du while prematurement */
      if ((retour < 1) || error) break;

   }  /* fin du WHILE */
   traitebool(error);        
   pv_give_error("evite_element");
   return(-1);
}


/*********
*
*  CONSTRUIT_RECTANGLE
*
* Cette fonction reconstitue un polygone (boundary) decrivant une partie du
* modele. De plus, elle debite ce polygone en rectangles,  en ayant verifie
* au prealable que celui-ci est bien un polygone rectangle (sans fil a 45').
*
* Retour : -2 si on a eu une erreur bloquante
*     -1 si le polygone n'etait pas rectangle (erreur non bloquante)
*      0 si tout s'est bien passe
*
***/

static int
   pv_construit_rectangle(fp)
FILE *fp;
{
   hinfo_type   infobuf;
   int          retour;
   int          taille;
   char         poubelle[TRASHSIZE], texte[64];
   coord_t      *coord_tab, *new_coord_tab;
   unsigned     coord_nb, new_coord_nb;
   short        gds_layer;
   char         rds_layer = 0;
   long         rds_type = 0;
   int          i, FLAG = 0;
   int          error = FALSE;
   rdsrec_list *Rec, *Tangle;

   while ((retour = fread((char *)&infobuf, sizeof(hinfo_type), 1, fp)) == 1) {
      if (islittle()) {
         infobuf.gdscode = swaps(infobuf.gdscode);
         infobuf.size    = swaps(infobuf.size);
      }
      switch (infobuf.gdscode) {
      case LAYER0 :
         lecture1(&gds_layer, sizeof(short));
         if (islittle())
            gds_layer = swaps(gds_layer);
         rds_layer = pv_gdslayer_to_symb(gds_layer);
         if (rds_layer == (char)-1) FLAG = -1;
         if (pv_gdslayer_iscon(gds_layer))
            rds_type = MBK_CONNECTOR_MASK;
         break;
      case XY :
         taille = infobuf.size - sizeof(hinfo_type);
         if (FLAG == -1) {
            lecture2(poubelle, taille);
            break;
         }
         coord_nb = taille >> NSHIFT;
         coord_tab = (coord_t *)mbkalloc(coord_nb * sizeof(coord_t));
         lecture1(coord_tab, taille);
         if (islittle()) {
         int i;

            for (i = 0; i < coord_nb; i++) {
                  coord_tab[i].X = swapl(coord_tab[i].X);
                  coord_tab[i].Y = swapl(coord_tab[i].Y);
            }
         }
         if ((coord_nb < 5) || (ispolyrec(coord_tab, coord_nb) == 0)) {
            sprintf(texte, "(%ld, %ld", coord_tab[0].X, coord_tab[0].Y);
            for (i = 1; i < coord_nb; i++)
               sprintf(&texte[strlen(texte)], ", %ld, %ld", coord_tab[i].X, coord_tab[i].Y);
            strcat(texte, ")");
            pv_emet_warning(pv_model->NAME, "Non rectangle polygon here", texte);
            FLAG = -1;
            free((char *)coord_tab);
            break;
         }
         
         poly2diag(coord_tab, coord_nb, pv_scale, &new_coord_tab, &new_coord_nb);
         if (new_coord_tab == (coord_t *)NULL) {
            pv_init_error();
            pv_error.v_error = ENOMEM;
            pv_error.v_textp = "(poly2diag)";
            pv_give_error("construit_rectangle");
            error = TRUE;
            free((char *)coord_tab);
            break;
         }
         free((char *)coord_tab);
         Tangle = pv_model->LAYERTAB[(int)rds_layer];
         diag2rec(pv_model, rds_layer, new_coord_tab, new_coord_nb);
         if (pv_model->LAYERTAB[(int)rds_layer] == Tangle) {
            pv_init_error();
            pv_error.v_error = ENOMEM;
            pv_error.v_textp = "(diag2rec)";
            pv_give_error("construit_rectangle");
            error = TRUE;
         } else {
           if (rds_type == MBK_CONNECTOR_MASK)
              for (Rec = pv_model->LAYERTAB[(int)rds_layer];
                   Rec != Tangle;
                   Rec = Rec->NEXT) {
                 SetRdsReference(Rec);
                 SetRdsRefCon(Rec);
              }
         }
         free((char *)new_coord_tab);
         break;
      case ELKEY      :
      case PROPATTR   :
      case PROPVALUE  :
      case ELFLAGS    :
      case DATATYPE   :
      case PLEX       :
         lecture2(poubelle, infobuf.size - sizeof(hinfo_type));
         break;
      case ENDEL :
         return(FLAG);
      default :
         pv_init_error();
         pv_error.v_error = ENOCODE;
         error = TRUE;
         break;
      }  /* fin du SWITCH */

   /* on eu une erreur, donc on sort du while prematurement */
   if ((retour < 1) || error) break;
   }  /* fin du WHILE */

   traitebool(error);        
   pv_give_error("fin_construit_rectangle");
   return(-2);
}

/***********
*
*  CONSTRUIT_INSTANCE
*
* Cette fonction reconstitue une instance (sref) decrivant une partie du modele.
*
* Retour : -2 si on a eu une erreur bloquante
*     -1 si presence de loupe, (erreur non bloquante)
*      0 si tout s'est bien passe
*
***/
static int
   pv_construit_instance(fp)
FILE *fp;
{
hinfo_type infobuf;
int retour;
int error = FALSE;
char nom_modele[33];
char *nom_instance = (char *)0;
char gds_real[sizeof(mag_type)];
short ref_x_axis, abs_mag, abs_ang;
ushort strans;
double mag, angle;
long x, y;
char sym;
char poubelle[TRASHSIZE];
int FLAG = 0;


   angle = ref_x_axis = abs_mag = abs_ang = 0;
   while ((retour = fread((char *)&infobuf, sizeof(hinfo_type), 1, fp)) == 1) {
      if (islittle()) {
         infobuf.gdscode = swaps(infobuf.gdscode);
         infobuf.size    = swaps(infobuf.size);
      }
      switch (infobuf.gdscode) {
         case XY :
            lecture1(&x, sizeof(long));
            if (islittle())
               x = swapl(x);
            x = (long)(pv_scale * (double)x);
            lecture1(&y, sizeof(long));
            if (islittle())
               y = swapl(y);
            y = (long)(pv_scale * (double)y);
            break;
         case STRANS :
            lecture1(&strans, sizeof(ushort));
            if (islittle())
               strans = swaps(strans);
            ref_x_axis = strans & GREFLECT;
            abs_mag = strans & GABSMAG;
            abs_ang = strans & GABSANG;
            if (abs_mag | abs_ang) {
               pv_emet_warning(pv_model->NAME, "Unable to cope with magnification or absolute angle on",nom_modele);
               FLAG = -1;
            }
            break;
         case MAG :
            lecture1(gds_real, sizeof(mag_type));
            if (FLAG == 0) {
               mag = pv_gdsreal_to_double(gds_real, sizeof(mag_type));
               if (pv_approximation(mag,1.0) == 0) {
                  pv_emet_warning(pv_model->NAME,
                                        "Cannot apply maginification to",
                                        nom_modele);
                  FLAG = -1;
               }
            }
            break;
         case ANGLE :
            lecture1(gds_real, sizeof(angle_type));
            if (FLAG == 0)
               angle = pv_gdsreal_to_double(gds_real, sizeof(angle_type));
            break;
         case SNAME :
            lecture1(nom_modele, infobuf.size - sizeof(hinfo_type));
            nom_modele[retour] = '\0';
            nom_instance = pv_genere_nom(nom_modele);
            break;
         case ELFLAGS   :
         case PLEX      :
         case ELKEY     :
         case PROPATTR  :
         case PROPVALUE :
            lecture2(poubelle, infobuf.size - sizeof(hinfo_type));
            break;
         case ENDEL :
            if (FLAG == 0) {
               sym = pv_gds_to_rds_sym(ref_x_axis, angle);
               if (sym == -1) {
                  pv_emet_warning(pv_model->NAME,
                                       "Unknown geometric operation applied to",
                                        nom_modele);
                  free(nom_instance);
                  return(-1);
               }
               if ((CheckPhysicalGridAligned(x)
                     | CheckPhysicalGridAligned(x)) != 0) {
               pv_emet_warning(pv_model->NAME,
                                    "Gds Instance not aligned on grid", NULL);
               pv_model->INSTANCE = addrdsins(pv_model, nom_modele,
                                              nom_instance, sym,
                                       RfmRoundLow(x), RfmRoundLow(y));
               } else 
                  pv_model->INSTANCE = addrdsins(pv_model, nom_modele, nom_instance, sym, x, y);
            }
            free(nom_instance);
            return(FLAG);
         default :
            pv_init_error();
            pv_error.v_error = ENOCODE;
            error = TRUE;
            break;
      }

   /* on eu une erreur, donc on sort du while prematurement */
   if ((retour < 1) || error) break;
   }  /* fin du WHILE */
      
   traitebool(error);        
   pv_give_error("construit_instance");
   return(-2);
}
/*******************************
 *
 * Done the 23/09/93 in order to read the texts of GDS files
 * This is a copy of one of the other function, with slight
 * modifications for reading text.
 * Author: Frederic Petrot
 ***********************/
static int
   pv_read_text(fp)
FILE *fp;
{
   hinfo_type    infobuf;
   int    retour;
   int    taille;
   coord_t   *coord_tab = (coord_t *)0;
   unsigned coord_nb;
   short    gds_layer;
   char     rds_layer = 0;
   int    FLAG = 0, XYFLAG = 0;
   int    error = FALSE;
   char     string[512];
   char poubelle[TRASHSIZE];

   *string = '\0'; /* has a string been found ? */
   while ((retour = fread((char *)&infobuf, sizeof(hinfo_type), 1, fp)) == 1) {
      if (islittle()) {
         infobuf.gdscode = swaps(infobuf.gdscode);
         infobuf.size    = swaps(infobuf.size);
      }
      switch (infobuf.gdscode) {
      case LAYER0 :
         lecture1(&gds_layer, sizeof(short));
         if (islittle())
            gds_layer = swaps(gds_layer);
         rds_layer = pv_gdslayer_to_symb(gds_layer);
         if (rds_layer == (char)-1) FLAG = -1;
         break;
      case XY :
         taille = infobuf.size - sizeof(hinfo_type);
         if (FLAG == -1) {
            lecture2(poubelle, taille);
            break;
         }
         coord_nb = taille >> NSHIFT;
         if (XYFLAG || coord_nb > 1) {
            pv_emet_warning(pv_model->NAME,
                           "More than one XY for a TEXT entry",
                           (char *)NULL);
            break;
         }
         coord_tab = (coord_t *)mbkalloc(coord_nb * sizeof(coord_t));
         lecture1(coord_tab, taille);
         if (islittle()) {
            coord_tab[0].X = swapl(coord_tab[0].X);
            coord_tab[0].Y = swapl(coord_tab[0].Y);
         }
         coord_tab[0].X = (long)(pv_scale * (double)coord_tab[0].X);
         coord_tab[0].Y = (long)(pv_scale * (double)coord_tab[0].Y);
         XYFLAG = 1;
         break;
      case TEXTTYPE    :
      case ELFLAGS     :
      case PLEX        :
      case PRESENTATION:
      case PATHTYPE    :
      case WIDTH       :
      case STRANS      :
      case MAG         :
      case ANGLE       :
         lecture2(poubelle, infobuf.size - sizeof(hinfo_type));
         break;
      case ENDEL :
         /* Run thru the rectangles to attach them a name if needed
            Fred */
         if (FLAG == 0 && *string != '\0')
            GdsAddNode(namealloc(string),
                     coord_tab[0].X, coord_tab[0].Y, rds_layer);
         return FLAG;
      case STRING :
         lecture1(string, infobuf.size - sizeof(hinfo_type));
         string[infobuf.size - sizeof(hinfo_type)] = '\0';
         break;
      default :
         pv_init_error();
         pv_error.v_error = ENOCODE;
         error = TRUE;
         break;
      }   /* fin du SWITCH */

   /* on eu une erreur, donc on sort du while prematurement */
   /* if ( (retour < 1) || error) break; */
   }   /* fin du WHILE */

   /* traitebool(error);
   pv_give_error("fin_lit_texte");
   return(-2); */
   return 0;
}

rdsfig_list *gdsloadrdsfig(Figure, Name, Mode)
rdsfig_list *Figure;
char        *Name;
char         Mode;
{
register int retour;
hinfo_type infobuf;
FILE *fp;
int flag = 0;
int error = FALSE;
char poubelle[TRASHSIZE];
char gds_real[sizeof(unit_type)];
char nom_modele[33];
node_list *ScanNode;
rdsrec_list  *Rec;


   Figure->MODE = 'L';

   pv_model = (rdsfig_list *)NULL;
   pv_warning = 1;      /* 1 valeur par defaut, envoi de messages. */

   if ((fp = mbkfopen(Name, "gds", "r")) == (FILE *)NULL) {
      pv_init_error();
      pv_error.v_error = ENOFILE;
      pv_error.v_textp = Name;
      pv_give_error("loadgds");
      return(NULL);
   }

   ErrNo = 0;             /* pour etre sur d'avoir zero au depart */
   pv_init_error();       /* remise a zero de v */

   while ((retour = fread((char *)&infobuf, sizeof(hinfo_type), 1, fp)) == 1) {
      if (islittle()) {
         infobuf.gdscode = swaps(infobuf.gdscode);
         infobuf.size    = swaps(infobuf.size);
      }
      switch (infobuf.gdscode) {
      case UNITS :
         lecture1(poubelle, sizeof(unit_type));
         lecture1(gds_real, sizeof(unit_type));
         pv_scale = pv_gdsreal_to_double(gds_real,sizeof(unit_type))
                     * RDS_UNIT * MICRONS_IN_METER; 
         break;
      case STRNAME :
         lecture1(nom_modele, infobuf.size - sizeof(hinfo_type));
         nom_modele[retour] = '\0';
         if (namealloc(nom_modele) == Figure->NAME)
            pv_model = Figure;
         else 
            pv_model = addrdsfig(nom_modele, Figure->SIZE);
         break;
      case SREF :
         switch (pv_construit_instance(fp)) {
         case 0 :       /* deroulement normal   */
            break;
         case -1 :         /* erreur non bloquante.*/
            if (flag != -1) flag = -2;
         pv_emet_warning(pv_model->NAME,
               "This model will not be included into the data structure",
               (char *)NULL);
         delrdsfig(pv_model->NAME); 
         if (pv_skip_structure(fp) < 0)   error = SREFERR;
            break;
         case -2 :
         default :
            error = SREFERR;
            break;
         }
         break;
      case BOUNDARY :
         switch (pv_construit_rectangle(fp)) {
         case  0 :   /* deroulement normal      */
            break;
         case -1 :   /* erreur non bloquante.   */
            if (flag != -1) flag = -2;
               pv_emet_warning(pv_model->NAME,
               "This model will not be included into the data structure",
               (char *)NULL);
         delrdsfig(pv_model->NAME);
         if (pv_skip_structure(fp) < 0) error = BOUNDARYERR;
            break;
         case -2 :
         default :
            flag = -1;
            error = BOUNDARYERR;
            break;
         }
         break;
      case AREF :
      case TEXT : /* Well, this is added from the top of my head.
         Fred, 23/09/93 */
         pv_read_text(fp);
         error = 0;
         break;

      case NODE :
      case BOX  :
      case PATH :
         if (flag == 0) flag = -3;
/* on ne peut positionner ce flag que si il n'y a pas eu d'erreur plus grave avant */
         if (pv_evite_element(fp, infobuf.gdscode) < 0)
            error = PATHERR;
         break;
      case BGNSTR     :
      case REFLIBS    :
      case FONTS      :
      case ATTRTABLE  :
      case STRCLASS   :
      case GENERATIONS:
      case FORMAT     :
      case MASK       :
      case HEADER     :
      case BGNLIB     :
      case LIBNAME    :
         lecture2(poubelle, infobuf.size - sizeof(hinfo_type));
         break;
      case ENDSTR : /* On a rien a faire, on peut passer a la structure suivante. */
         break;
      case ENDMASKS :         /* il n'y a rien a faire   */
         break;
      case ENDLIB :        /* tout s'est bien passe   */
         (void)fclose(fp);
         /***********************
         * Name the rectangles that deserve it.
         *****************************************/
         for ( ScanNode  = HEAD_NODE; 
            ScanNode != (node_list *)NULL;
            ScanNode  = ScanNode->NEXT )
         {
            Rec = GdsGetRectangle( Figure, ScanNode->X, ScanNode->Y, ScanNode->LAYER );
            if ( Rec == (rdsrec_list *)NULL )
            {
               sprintf(poubelle, "X: %ld, Y: %ld, Rds Layer: %d Name: %s",
                     ScanNode->X, ScanNode->Y,
                     ScanNode->LAYER, ScanNode->NAME);
               pv_emet_warning(Figure->NAME, "Unconnected node :", poubelle);
            }
            else
               Rec->NAME = ScanNode->NAME;
         }
         GdsFreeNode();

         applyrdsmode(Figure, Mode);
         return(Figure);   /* Avant retournait flag ! */
      case TEXTNODE :         /* not yet released  */
         error = TEXTNODERR;
         break;
      case STYPTABLE :     /* unreleased feature   */
         error = STYPTABLERR;
         break;
      case STRTYPE :       /* unreleased feature   */
         error = STRTYPERR;
         break;
      default :
         error = CODEINCONNU;
         break;
      }  /* fin du SWITCH */

   /* on eu une erreur, donc on sort du while prematurement */
      if ((retour < 1) || error) break;

   }     /* fin du WHILE */

   (void)fclose(fp); /* Si on arrive jusqu'ici, c'est qu'on a eu  */
   traitebool(error);   /* un probleme quelconque dans le traitement */
   pv_give_error("loadgds");/* du fichier GDS.           */
   return (rdsfig_list *)NULL;
}
