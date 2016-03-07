/*------------------------------------------------------------\
|                                                             |
| Tool    :                     RDS                           |
|                                                             |
| File    :                   rutpoly.c                       |
|                                                             |
| Authors :                  Pierre Vitet                     |
|                                                             |
| Date    :                   27.06.95                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <math.h>
# include <stdlib.h>
# include <string.h>

# include <mut.h>
# include <mph.h>
# include "rds.h"
# include "rtl.h"
# include "rpr.h"
# include "rfm.h"
# include "rwi.h"
# include "rut.h"

# include "rutpoly.h"
# include "ruterror.h"

/*------------------------------------------------------------\
|                                                             |
|                          Tools                              |
|                                                             |
\------------------------------------------------------------*/
/***********
*
*                        QUICKSORT
*
* Cette procedure sert a trier un tableau d'entiers dans l'ordre croissant.
*
***/
static void quicksort(tab, deb, fin)
int32_t tab[];
unsigned        deb, fin;
{
register int32_t   m, aux;
register unsigned       i, j;

      i = deb;
      j = fin;
      m = tab[(deb+fin) >> 1];                         /* on repere le milieu du tableau */
      do {
              while (tab[i] < m)
                      i++;
              while (tab[j] > m)
                      j--;
              if ( i <= j ) {
                      aux = tab[i];
                      tab[i] = tab[j];
                      tab[j] = aux;
                      i++;
                      j--;
              }
      } while (i <= j);
      if (deb < j)
              quicksort(tab, deb, j);
      if (i < fin)
              quicksort(tab, i, fin);

      return;
}


/***********
*
*                        SIMPLIFICATION
*
*        Cette fonction est utilisee pour oter toutes les valeurs qui existent en
*        double (ou en triple...) dans un tableau deja trie (ordre croissant).
*        On va utiliser 2 index : un pour la lecture, l'autre pour l'ecriture.
*               On stocke le premier element (ce qui revient a commencer a lire a
*                partir du 2eme element.
*               POUR tous les elements du tableau FAIRE
*                Si l'element lu est different du dernier element a avoir ete stocke
*                Alors le sauver
*               FPOUR
*
*        ENTREE : l'adresse du tableau
*                       la longueur de cette zone (i.e nombre d'elements du tableau)
*        RETOUR : le nombre d'elements differents du tableau maintenant remanie.
*
***/
static int      simplification(tab, tab_lg)
int32_t    tab[];
unsigned        tab_lg;
{
register int i, j;  /* i indice de lecture, j celui d'ecriture */
             /* remarque : j indexe l'element qui vient d'etre sauve */
      j = 0;
      for (i = 1; i < tab_lg; i++) /* on scanne toutes les valeurs     */
              if (tab[i] != tab[j])
                      tab[++j] = tab[i]; /* et on sauve si necessaire         */
      return(j + 1);
}

/***********
*
*                        AJOUTE
*
*        Cette fonction realise l'insertion d'une valeur dans un        tableau d'entier.
*        Elle ne travaille reellement que sur un segment de ce pseudo-tableau.  Elle
*        insere cette valeur parmi celles deja presentes dans la zone. Tri par ordre
*        croissant.
*
*       ENTREE : l'adresse du debut de la zone dans laquelle on insere la valeur.
*                        taille de la zone.
*                        la valeur a inserer.
*       RETOUR : aucune
*
***/

static void ajoute(tab, tab_lg, val)
type_i  tab[];
type_l  tab_lg;
type_i  val;
{
register int    i, j;

      for (i = 0; i < tab_lg; i++) {
              if (tab[i] == (type_i) - 1) { /* ici, la valeur est mise a la fin de la zone            */
                      tab[i] = val;
                      break;
              }
              if (tab[i] > val) {                    /* Ici, la valeur doit etre inseree */
                      for (j = i + 1; j < tab_lg; j++)     /* On cherche la 1ere valeur a -1 dans la zone.  */
                              if (tab[j] == (type_i) - 1)
                                      break;
                      for ( ; j > i; j--)                  /* Des qu'on l'a, on peut commencer a redescendre */
                              tab[j] = tab[j-1];           /* les valeurs pour ceder la place a la nouvelle. */
                      tab[i] = val;
                      break;
              }
      }
      return;
}

/***********
*
*       OTE_DOUBLONS
*
*   Cette fonction est utilisee pour oter toutes les valeurs qui existent
*   en double dans un tableau trie par ordre croissant. En fait, on va se
*   borner a remplacer les doublons par -1.  On evitera simplement de les
*   interpreter par la suite lorsqu'on construira les pseudo-rectangles.
*
*  ENTREE : Adresse du debut de la zone dans laquelle on insere la valeur
*       La taille de la zone
*  RETOUR : Aucune
*
***/

static void ote_doublons(tab, tab_lg)
type_i  tab[];
type_l  tab_lg;
{
  register int  i;

  if (tab_lg == 2)
    return;           /* si il n'y a que 2 valeurs, il n'y a rien a faire */
  for (i = 1; i < tab_lg - 2; i++)
    if (tab[i] == tab[i+1]) {   /* Si les 2 valeurs sont identiques         */
      tab[i] = (type_i) - 1;
      tab[i+1] = (type_i) - 1;
    }
  return;
}


/***********
*
*       DO_V_RECTANGLE & DO_H_RECTANGLE
*
*   Ces fonctions servent a former les rectangles avec leurs coordonnees
*   non pas reelles mais exprimees en fonction de leur position relative
*   par rapport aux autres. En clair, on recupere les cordonnees reelles
*   dans les pseudo-tableaux xtab et ytab.
*
*  ENTREE : Pseudo-tableau de reception des pseudo-coordonnees des rectangles
*       Indice d'ecriture dans ce pseudo-tableau
*       Indice dans xtab (ou ytab) de l'abcisse gauche (ou ordonnee basse)
*       Pseudo-tableau des aretes horizontales (ou verticales)
*       Indice de debut de lecture dans le precedent tableau
*       Indice de fin de lecture dans le precedent tableau
*  RETOUR : Nombre de rectangles deja inscrit dans le 1er tableau (c'est donc
*         la valeur de l'indice en sortie).
*
***/

static type_l do_v_rectangle(rect_tab, idx, gauche, pos_tab, deb, fin)
rect_t  rect_tab[];
type_l  idx;
type_i  gauche;
type_i  pos_tab[];
type_l  deb;
type_l  fin;
{
  register int  i, j;

  register type_i bas, haut;
  register int  deja_vu;

  i = deb;
  do {
    deja_vu = 0;
    bas = pos_tab[i++];
    while (pos_tab[i] == (type_i) - 1) 
      i++;     /* on passe les eventuels -1 */
    haut = pos_tab[i++];

    /* on va verifier s'il n'existe pas deja un rectangle de meme hauteur qui soit */
    /* accole, a gauche, a celui qu'on cherche a former. Dans ce cas, on ne ferait */
    /* qu'un rectangle de ces 2 la -> unification.                 */
    for (j = 0; j < idx; j++)
      if ( rect_tab[j].r_d == gauche && rect_tab[j].r_b == bas && rect_tab[j].r_h == haut ) {
        rect_tab[j].r_d++;
        deja_vu = 1;
        break;
      }
    if (deja_vu) 
      continue;      /* si on vient justement d'elargir un rectangle, on n'en */
    /* cree pas un nouveau, on passe aux aretes suivantes.   */

    rect_tab[idx].r_g = gauche;
    rect_tab[idx].r_d = gauche + 1;
    rect_tab[idx].r_b = bas;
    rect_tab[idx].r_h = haut;
    idx++;
  } while (i < fin);

  return(idx);      /* on renvoie l'indice du nouveau poste de libre. */
}


static type_l do_h_rectangle(rect_tab, idx, bas, pos_tab, deb, fin)
rect_t  rect_tab[];
type_l  idx;
type_i  bas;
type_i  pos_tab[];
type_l  deb;
type_l  fin;
{
  register int  i, j;
  register type_i gauche, droite;
  register int  deja_vu;

  i = deb;
  do {
    deja_vu = 0;
    gauche = pos_tab[i++];
    while (pos_tab[i] == (type_i) - 1) 
      i++;   /* on passe les eventuels -1 */
    droite = pos_tab[i++];

    /* on va verifier s'il n'existe pas deja un rectangle de meme largeur qui  soit */
    /* accole en dessous de celui qu'on cherche a former. Dans ce cas, on ne ferait */
    /* qu'un rectangle de ces 2 la -> unification.                  */
    for (j = 0; j < idx; j++)
      if ( rect_tab[j].r_h == bas && rect_tab[j].r_g == gauche && rect_tab[j].r_d == droite ) {
        rect_tab[j].r_h++;
        deja_vu = 1;
        break;
      }
    if (deja_vu) 
      continue;    /* si on vient justement de rehausser un rectangle, on n'en */
    /* cree pas un nouveau, on passe aux aretes suivantes.    */

    rect_tab[idx].r_g = gauche;
    rect_tab[idx].r_d = droite;
    rect_tab[idx].r_b = bas;
    rect_tab[idx].r_h = bas + 1;
    idx++;
  } while (i < fin);

  return(idx);      /* on renvoie l'indice du nouveau poste de libre. */
}


/***********
*
*       D2RDS
*
*   Cette fonction prend un tableau de structures qui decrivent des  diagonales
*   de rectangles et produit les rectangles correspondant dans la base RDS.
*   Les deux 1eres paires decrivent la 1ere diagonale et ainsi de suite.
*
*  ENTREE : Layer de la liste ou il faut inserer les nouveaux rectangles
*       Tableau de structures contenant les coordonnees des diagonales
*       Taille du tableau
*  RETOUR : Liste des rdsrec_listtangles pointee par HeadFigureRds->layertab[layer]
*
***/

void diag2rec(model, layer, coord_tab, coord_numb)
rdsfig_list *model;
char  layer;
coord_t  coord_tab[];
unsigned  coord_numb;
{
      rdsrec_list *Rec;
  register unsigned int  i, j;
  register int32_t  x, y, dx, dy;

  for (i = 0; i < coord_numb; i += 2) {
    j = i + 1;
    x   = coord_tab[i].X;   
    y   = coord_tab[i].Y;
    dx = coord_tab[j].X - x; 
    dy = coord_tab[j].Y - y;
    if (CheckPhysicalGridAligned(x)
        || CheckPhysicalGridAligned(y)
        || CheckPhysicalGridAligned(dx)
        || CheckPhysicalGridAligned(dy)) {
      fprintf(stderr, "*** rds warning ***\nPolygon with points not ");
      fprintf(stderr, "aligned on the physical grid found in %s\n", model->NAME);
      fprintf(stderr, "%ld %ld %ld %ld\n", x, y, dx, dy);
      Rec = addrdsfigrec(model, (char *)NULL, layer, x, y, dx, dy );
      if ( layer == RDS_ABOX ) SetRdsFigure( Rec );
         roundrdsrec( Rec );
    }
    else
    {
      Rec = addrdsfigrec(model, (char *)NULL, layer, x, y, dx, dy );
      if ( layer == RDS_ABOX ) SetRdsFigure( Rec );
         roundrdsrec( Rec );
    }
  }
}


/***********
*
*       DO_DIAGONALE
*
*   Cette fonction prend un tableau de structures qui decrivent tous les rectangles
*   necessaire a la reconstitution du polygone.  Ces  structures ne contiennent pas
*   les coordonnees reelles des sommets.  Elles ne font qu'indiquer ou on peut con-
*   vertir les coordonnees fictives en coordonnees reelles. On va donc les chercher
*   dans les pseudo-tableaux xtab et ytab. La fonction cree et remplit  un  tableau
*   de structures coord_t dans lequel on trouve decrits les rectangles avec des co-
*   ordonnees reelles : les deux 1eres entrees du tableau concernent  la  diagonale
*   du 1er rectangle (le point en bas a gauche et celui en haut a droite) et  ainsi
*   de suite.
*
*  ENTREE : Pseudo-tableau de structures contenant les coordonnees fictives des rectangles
*       Taille du pseudo-tableau
*       Table de conversion abcisse fictive >> abcisse reelle
*       Table de conversion ordonnee fictive >> ordonnee reelle
*  RETOUR : Tableau de structures coord_t
*
***/

static coord_t *do_diagonale(rect_tab, nb_rect, xtab, ytab)
rect_t  rect_tab[];
type_l  nb_rect;
int32_t  xtab[], ytab[];
{
  register int  i, j;
  register coord_t  *tab;

  tab = (coord_t * )malloc(nb_rect * 2 * sizeof(coord_t));
  if (tab == (coord_t * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "do_diagonale");
    return((coord_t * )NULL);
  }

  for (i = 0; i < nb_rect; i++) {
    j = 2 * i;
    tab[j].X   = xtab[rect_tab[i].r_g];
    tab[j].Y   = ytab[rect_tab[i].r_b];
    tab[j+1].X = xtab[rect_tab[i].r_d];
    tab[j+1].Y = ytab[rect_tab[i].r_h];
  }
  return(tab);
}


/***********
*
*       P2D
*
*   Cette fonction prend la description d'un polygone et transforme  celui-ci
*   en un ensemble de diagonales de rectangles equivalent.
*
*  ENTREE: - coord_tab est un pointeur sur un tableau contenant les paires de
*      coordonnees.
*      - coord_numb est le nombre de coordonnees contenues dans ce pseudo-
*      tableau.
*      Permet de borner la zone utilisee pour stocker les coordonnees.
*      - Facteur de grossissement : pour changer de systeme  de  coordon-
*      neees.
*      - Adresse d'un pointeur de coord_t
*      - Pointeur sur un unsigned
*
*  RETOUR:  Un pointeur sur un tableau de paires  de  coordonnees  destines a
*       remplacer le polygone. Ce tableau ne contient que les coordonnees
*       des diagonales de ces rectangles.
*       NULL si manque d'espace memoire
*
*  REMARQUES IMPORTANTES :
*    1- Le polygone decrit par le tableau  de  coordonnees  coord_tab  DOIT
*     etre rectangle (il ne DOIT y avoir que des angles  a  90'  dans  le
*     dessin.
*    2- on va supposer que ce tableau contient n+1 couples  de  coordonnees
*     si le polygone a n sommets. Le 1er et le dernier couple sont  iden-
*     tiques (ils ferment le polygone).
*    3- xtab et ytab contiennent les coordonnees reelles du polygone. Dans
*     tout l'algorithme, on utilisera des coordonnees fictives qui seront
*     en fait des index sur xtab et ytab, et permettront de retrouver les
*     coordonnees reelles, a la fin, pour produire les rectangles finaux.
*    4- Pour economiser l'espace memoire, on va supposer qu'il n'y aura pas
*     plus de 65535 abcisses ou ordonnees differentes ( 2^16 de chaque au
*     maximum ). Ce qui permet de definir nb_x et nb_y comme type_i, donc
*     comme unsigned short.
*     - pos_h et pos_v sont 2 tableaux qui contiennent les positions verti-
*     cales  (resp. horiz.) des  aretes  horizontales (resp. vert.)  dans
*     chaque colonne (resp. ligne) du  dessin.  Ces  positions  sont  des
*     index dans ytab (resp. xtab) dont les valeurs sont  obligatoirement
*     comprises entre 0 et nb_y (resp. nb_x).  On definit donc  pos_h  et
*     pos_v comme des tableaux contenant des "type_i".
*     - index_h et index_v sont 2 tableaux qui permettent  d'indexer  pos_h
*     et pos_v.  Ils montrent ou se trouve la 1ere arete de  chacune  des
*     colonnes (resp. lignes). Une fois le polygone decoupe  en  morceaux
*     elementaires (on utilise un maillage fait de toutes les abcisses et
*     ordonnees differentes),  on obtient generalement (toujours ?)  plus
*     d'aretes horizontales (resp. vert.) qu'il  n'y a d'abcisses  (resp.
*     d'ordonnees) differentes : si le dessin a n+1 ordonnees differentes
*     cela fait n lignes (comprendre intervalles), on a donc au minimum n
*     aretes pour monter  +  n  pour descendre (pour un dessin simple, du
*     genre carre, pyramide maya, escalier...). On en conclut que si on a
*     65535 ordonnees differentes (=2^16), on aura au minimum 2 fois plus
*     d'aretes verticales. Ce qui veut dire que pos_v aurait  dans ce cas
*     plus de 130 000 entrees. Donc il faut que index_h et index_v soient
*     des tableaux d'entiers longs pour pouvoir indexer correctement dans
*     ces cas la. On en fera des tableaux de type_l, ce type_l etant plus
*     "grand" que le type_i (comprendre la : "permettant de composer  des
*     entiers plus grands").  Neanmoins, il parait raisonnable de definir
*     type_l comme "unsigned short", la probabilite d'avoir plus de 65535
*     aretes horizontales (resp. vert) au total me paraissant faible.
*     - col et lig sont 2 tableaux qui permettent de savoir combien  d'are-
*     tes horizontales (resp. vert) se trouvent dans chaque colonne (resp
*     ligne) du dessin. Si on a n ordonnees differentes, on ne peut avoir
*     plus de n aretes horizontales dans une colonne puisqu'au minimum si
*     on a n aretes horizontales on a n ordonnees  differentes.  Le  seul
*     probleme qu'on peut avoir est quand une arete  est  parcourue  dans
*     les 2 sens. Cela arrive quand on a un trou dans le dessin, on  cree
*     un fil qu'on parcourt dans un sens pour aller parcourir le bord  du
*     trou, et dans l'autre pour rejoindre le parcours exterieur. Donc la
*     on aurait 2 aretes pour une ordonnee unique dans la  meme  colonne.
*     Mais en fait, le parcours interieur implique au moins une  ordonnee
*     pour laquelle il n'y a pas d'arete correspondante dans la preceden-
*     te colonne.  Comme il parait fortement improbable d'avoir plus de
*     65535 ordonnees (ou abcisses) differentes, on definit des  tableaux
*     de type_c ou type_c est du type unsigned short.
*     - On remarque que  type_c < type_i < type_l.   Peut-etre  faudrait-il
*     prevoir des tests pour ne pas depasser la capacite  d'accueil  d'un
*     de ces types (unsigned short actuellement) et demander une nouvelle
*     compilation de ce module avec des nouvelles definitions?
*     En tout cas, il est vraisemblable qu'il faudra d'abord agrandir  le
*     type "type_l" avant les autres.
*
***/

void poly2diag(coord_tab, coord_numb, scale, new_coord_tab, new_coord_numb)
coord_t   coord_tab[];
unsigned  coord_numb;
double  scale;
coord_t   **new_coord_tab;
unsigned  *new_coord_numb;
{
  register int  i, h, v;         /* compteurs a tout faire  */

  int32_t  x1, y1, x2, y2;        /* auxiliaires (de coordonnees) en tout genre */

  int32_t  *tab;              /* pseudo-tableau auxiliaire */
  int32_t  *xtab, *ytab;          /* pseudo-tableaux des differentes coordonnees*/
  type_i nb_x, nb_y;          /* tailles des 2 tableaux et egalement nombres d'abcisses et d'ordonnees significatives. */
  type_c * col, *lig;          /* pseudo-tableaux contenant respectivement les nombres d'aretes par ligne, par colonne. */
  type_l * index_h, *index_v;      /* pseudo-tableaux permettant d'indexer les tableaux contenant les positions des aretes. */
  type_i * pos_h, *pos_v;        /* pseudo-tableaux contenant les aretes classees par colonne (ligne) et dans l'ordre   */
  /* croissant des ordonnees (abcisses).                           */
  rect_t * v_rect, *h_rect;      /* pseudo-tableaux contenant les rectangles presque formes (coordonnees fictives).     */
  coord_t * nct;             /* tableau contenant les paires de coordonnees resultantes : celles qui forment les    */
  /* diagonales.                                       */
  coord_t   * coordonnees;        /* Si on doit faire une mise a l'echelle, ce tableau recevra les nouvelles coordonnees   */
  /* et si ce n'est pas le cas il servira a pointer directement coord_tab.         */
  short  bool = 0;           /* Indique si coordonnees pointe sur une autre zone que coord_tab (si on a alloue une  */
  /* nouvelle zone pour y ranger les coordonnees mises a l'echelle.            */

  /*                                                           */
  /* On regarde d'abord si le polygone a decouper n'est pas deja un rectangle, auquel cas le probleme est vite resolu.   */
  /*                                                           */

  if (coord_numb == 5) {
    nct = (coord_t * )malloc(2 * sizeof(coord_t));
    *new_coord_tab = nct;
    *new_coord_numb = 0;
    if (nct == (coord_t * )NULL) {
      ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(nct)");
      EXIT(1);
    }
    *new_coord_numb = 2;
    if (scale == 1.0) {
      nct[0].X = coord_tab[0].X;
      nct[0].Y = coord_tab[0].Y;
      nct[1].X = coord_tab[2].X;
      nct[1].Y = coord_tab[2].Y;
    } else {
      nct[0].X = (int32_t)floor(((double)coord_tab[0].X * scale) + .5);
      nct[0].Y = (int32_t)floor(((double)coord_tab[0].Y * scale) + .5);
      nct[1].X = (int32_t)floor(((double)coord_tab[2].X * scale) + .5);
      nct[1].Y = (int32_t)floor(((double)coord_tab[2].Y * scale) + .5);
    }
  }

  /*                                      */
  /* On fait la mise a l'echelle ? Seulement si necessaire.           */
  /*                                      */
  if (scale == 1.0)
    coordonnees = coord_tab;
  else {
    coordonnees = (coord_t * )malloc(coord_numb * sizeof(coord_t));
    if (coordonnees == (coord_t * )NULL) {
      ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(coordonnees)");
      EXIT(1);
    }
    bool = 1;                            /* On vient d'allouer une nouvelle zone pour coordonnees*/
    for (i = 0; i < coord_numb; i++) {               /* On recopie les valeurs du tableau passe en parametre */
      coordonnees[i].X = (int32_t)floor(((double)coord_tab[i].X * scale) + .5);  /* en les mettant a l'echelle de la base RDS.       */
      coordonnees[i].Y = (int32_t)floor(((double)coord_tab[i].Y * scale) + .5);
    }
  }

  /*                                      */
  /* On extrait de la liste des coordonnees toutes les valeurs significatives */
  /*                                      */
  tab = (int32_t *)malloc(coord_numb * sizeof(int32_t));    /* on alloue un tableau auxiliaire pour y stocker */
  if (tab == (int32_t *)NULL) {                /* temporairement les abcisses et les ordonnees.  */
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(tab)");
    if (bool) 
      free((char *)coordonnees);
    EXIT(1);
  }

  for (i = 0; i < coord_numb; i++)            /* On commence par les abcisses                    */
    tab[i] = coordonnees[i].X;

  quicksort(tab, 0, coord_numb - 1);           /* On les trie                             */
  nb_x = simplification(tab, coord_numb);        /* On ne garde que les valeurs significatives            */
  xtab = (int32_t *)malloc(nb_x * sizeof(int32_t));       /* On alloue un pseudo-tableau pour y ranger les valeurs definitives */
  if (xtab == (int32_t *)NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(xtab)");
    if (bool) 
      free((char *)coordonnees);        /* traine pas d'imprecision : si la techno employee n'est pas assez  */
    free((char *)tab);                  /* precise (ex: 50 * 0.001 = 0 si caste en long)           */
    EXIT(1);
  }
  /* On recopie ces valeurs definitives dans le nouveau tableau   */
  (void)memcpy((char *)xtab, (char *)tab, (size_t)nb_x * sizeof(int32_t));


  for (i = 0; i < coord_numb; i++)            /* Puis on s'occupe des ordonnees de la meme facon           */
    tab[i] = coordonnees[i].Y;

  quicksort(tab, 0, coord_numb - 1);
  nb_y = simplification(tab, coord_numb);
  ytab = (int32_t *)malloc(nb_y * sizeof(int32_t));
  if (ytab == (int32_t *)NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(ytab)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)tab);
    free((char *)xtab);
    EXIT(1);
  }
  /* On recopie ces valeurs definitives dans le nouveau tableau   */
  (void)memcpy((char *)ytab, (char *)tab, (int)nb_y * sizeof(int32_t));

  free((char * )tab);

  /*                                       */
  /* Calcul du nombre d'aretes, formant le polygone, pour chaque ligne et    */
  /* chaque colonne.                               */
  /*                                       */
  col = (type_c * )calloc(nb_x - 1, sizeof(type_c));     /* on prepare le pseudo-tableau pour repertorier les aretes  */
  if (col == (type_c * )NULL) {              /* horizontales contenues dans chaque colonne du dessin.   */
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(col)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    EXIT(1);
  }
  lig = (type_c * )calloc(nb_y - 1, sizeof(type_c));     /* idem pour les aretes verticales contenues dans chaque ligne.  */
  if (lig == (type_c * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(lig)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col);
    EXIT(1);
  }
  x1 = coordonnees[0].X;              /* initialisation des 1eres coordonnees */
  y1 = coordonnees[0].Y;              /* (le point 1 = P1)          */
  h = v = 0;
  while (xtab[h] != x1) 
    h++;  /* on positionne h et v pour demarrer a partir de la 1ere coordonnee du tableau coordonnees */
  while (ytab[v] != y1) 
    v++;
  for ( i = 0; i < coord_numb - 1; i++) {
    x2 = coordonnees[i+1].X;        /* initialisation des coordonnees suivantes (le point 2 = P2) */
    y2 = coordonnees[i+1].Y;
    if ( x1 < x2 )                /* ------ si P1 a gauche de P2 --------- */
      while ( xtab[h] < x2 )
        col[h++]++;
    else if ( x1 > x2 )             /* ------ si P1 a droite de P2 --------- */
      while ( xtab[h] > x2 )
        col[--h]++;
    else if ( y1 < y2 )             /* ------ si P1 au dessous de P2 ------- */
      while ( ytab[v] < y2 )
        lig[v++]++;
    else if ( y1 > y2 )             /* ------ si P1 au dessus de P2 -------- */
      while ( ytab[v] > y2 )
        lig[--v]++;
    x1 = x2;              /* P2 devient P1 */
    y1 = y2;
  }

  /*                                       */
  /* Preparation de 2 tableaux qui vont servir a indexer 2 tableaux contenant  */
  /* les aretes classees par colonnes et par lignes              */
  /*                                       */
  index_h = (type_l * )calloc(nb_x, sizeof(type_l));       /* reservation de la place en memoire  */
  if (index_h == (type_l * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(index_h)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col); 
    free((char *)lig);
    EXIT(1);
  }
  index_v = (type_l * )calloc(nb_y, sizeof(type_l));       /* reservation de la place en memoire  */
  if (index_v == (type_l * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(index_v)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col); 
    free((char *)lig);
    free((char *)index_h);
    EXIT(1);
  }
  index_h[0] = index_v[0] = 0;
  for (i = 1; i < nb_x; i++)           /* ce tableau indexe le debut de chaque colonne. */
    index_h[i] = index_h[i-1] + col[i-1];
  for (i = 1; i < nb_y; i++)
    index_v[i] = index_v[i-1] + lig[i-1];     /* celui-ci de chaque ligne dans pos_v */

  /*                                       */
  /* Mise en place des aretes horizontales et verticales dans les pseudo-    */
  /* tableaux pos_h et pos_v.                          */
  /* Exemple : pos_h va contenir a la suite des couples d'indices de ytab    */
  /* Chaque couple permet de positionner une arete (horizontale) verticalement */
  /* dans une certaine colonne du dessin. On determine de quelle colonne il  */
  /* s'agit grace a index_h. Chaque entree de index_h indique (avec l'indice   */
  /* qu'elle contient) a quel endroit de pos_h on change de colonne.       */
  /*                                       */
  pos_h = (type_i * )malloc(index_h[nb_x-1] * sizeof(type_i));         /* reservation de la place en memoire */
  if (pos_h == (type_i * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(pos_h)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col); 
    free((char *)lig);
    free((char *)index_h); 
    free((char *)index_v);
    EXIT(1);
  }
  pos_v = (type_i * )malloc(index_v[nb_y-1] * sizeof(type_i));         /* reservation de la place en memoire */
  if (pos_v == (type_i * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(pos_v)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col); 
    free((char *)lig);
    free((char *)index_h); 
    free((char *)index_v);
    free((char *)pos_h);
    EXIT(1);
  }
  for (i = 0; i < index_h[nb_x-1]; i++) 
    pos_h[i] = (type_i) - 1;        /* on initialise a -1 ces 2 tableaux */
  for (i = 0; i < index_v[nb_y-1]; i++) 
    pos_v[i] = (type_i) - 1;
  x1 = coordonnees[0].X;              /* initialisation des 1eres coordonnees (le point 1 = P1) */
  y1 = coordonnees[0].Y;
  h = v = 0;
  while (xtab[h] != x1) 
    h++;   /* on positionne h et v pour demarrer a partir de la 1ere coordonnee du tableau coordonnees */
  while (ytab[v] != y1) 
    v++;
  for ( i = 0; i < coord_numb - 1; i++) {
    x2 = coordonnees[i+1].X;        /* initialisation des coordonnees suivantes (le point 2 = P2) */
    y2 = coordonnees[i+1].Y;
    if ( x1 < x2 )                /* ------ si P1 a gauche de P2 --------- */
      while ( xtab[h] < x2 ) {
        ajoute(&pos_h[index_h[h]], index_h[h+1] - index_h[h], (type_i)v);
        h++;
      }
    else if ( x1 > x2 )             /* ------ si P1 a droite de P2 --------- */
      while ( xtab[h] > x2 ) {
        h--;
        ajoute(&pos_h[index_h[h]], index_h[h+1] - index_h[h], (type_i)v);
      }
    else if ( y1 < y2 )             /* ------ si P1 au dessous de P2 ------- */
      while ( ytab[v] < y2 ) {
        ajoute(&pos_v[index_v[v]], index_v[v+1] - index_v[v], (type_i)h);
        v++;
      }
    else if ( y1 > y2 )             /* ------ si P1 au dessus de P2 -------- */
      while ( ytab[v] > y2 ) {
        v--;
        ajoute(&pos_v[index_v[v]], index_v[v+1] - index_v[v], (type_i)h);
      }
    x1 = x2;              /* P2 devient P1 */
    y1 = y2;
  }

  /*                                      */
  /* Simplification des 2 pseudo-tableaux pos_h et pos_v.             */
  /* Si une zone contient, par exemple, la sequence 1|2|2|3|5|6 dans le tableau */
  /* pos_h, cela signifie que dans la colonne, concernee par la zone, on a 3  */
  /* rectangles : entre les ordonnees 1 et 2, 2 et 3, 5 et 6. Cela arrive quand */
  /* on rencontre un polygone avec un trou au milieu; pour le former avec un  */
  /* seul contour (continu) on est obliger de dessiner un trait invisible qui   */
  /* le coupe pour rejoindre le contour interieur, et pour fermer ce contour on */
  /* est oblige de repasser dans l'autre sens sur ce trait. On va donc simpli-  */
  /* fier les 2 rectangles qui se touchent pour obtenir la sequence 1|3|5|6.  */
  /*                                      */
  for (i = 0; i < nb_x - 1; i++)
    ote_doublons(&pos_h[index_h[i]], index_h[i+1] - index_h[i]);
  for (i = 0; i < nb_y - 1; i++)
    ote_doublons(&pos_v[index_v[i]], index_v[i+1] - index_v[i]);

  /*                                       */
  /* Calcul des rectangles en fonction des colonnes : rectangles verticaux   */
  /*                                       */
  v_rect = (rect_t * )malloc(index_h[nb_x-1] / 2 * sizeof(rect_t));
  if (v_rect == (rect_t * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(v_rect)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col); 
    free((char *)lig);
    free((char *)index_h); 
    free((char *)index_v);
    free((char *)pos_h); 
    free((char *)pos_v);
    EXIT(1);
  }
  v = 0;                /* v sert a compter le nombre de rectangles verticaux */
  for (i = 0; i < nb_x - 1; i++)
    v = do_v_rectangle(v_rect, (type_l)v, (type_i)i, pos_h, index_h[i], index_h[i+1]);

  /*                                       */
  /* Calcul des rectangles en fonction des lignes : rectangles horizontaux   */
  /*                                       */
  h_rect = (rect_t * )malloc(index_v[nb_y-1] / 2 * sizeof(rect_t));
  if (h_rect == (rect_t * )NULL) {
    ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(h_rect)");
    if (bool) 
      free((char *)coordonnees);
    free((char *)xtab); 
    free((char *)ytab);
    free((char *)col); 
    free((char *)lig);
    free((char *)index_h); 
    free((char *)index_v);
    free((char *)pos_h); 
    free((char *)pos_v);
    free((char *)v_rect);
    EXIT(1);
  }
  h = 0;                /* h sert a compter le nombre de rectangles horizontaux */
  for (i = 0; i < nb_y - 1; i++)
    h = do_h_rectangle(h_rect, (type_l)h, (type_i)i, pos_v, index_v[i], index_v[i+1]);

  /*                                       */
  /* Choix du meilleur decoupage et production des rectangles finaux.      */
  /*                                       */
  if (h < v) {
    free((char *)v_rect);
    nct = do_diagonale(h_rect, (type_l)h, xtab, ytab);
    *new_coord_tab = nct;
    *new_coord_numb = 2 * h;
    if (nct == (coord_t * )NULL) {
      ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(nct)");
      if (bool) 
        free((char *)coordonnees);
      free((char *)xtab); 
      free((char *)ytab);
      free((char *)col); 
      free((char *)lig);
      free((char *)index_h); 
      free((char *)index_v);
      free((char *)pos_h); 
      free((char *)pos_v);
      free((char *)h_rect);
      EXIT(1);
    }
    free((char *)h_rect);
  } else {
    free((char *)h_rect);
    nct = do_diagonale(v_rect, (type_l)v, xtab, ytab);
    *new_coord_tab = nct;
    *new_coord_numb = 2 * v;
    if (nct == (coord_t * )NULL) {
      ruterror( RDS_NOT_ENOUGH_MEMORY, "p2d(nct)");
      if (bool) 
        free((char *)coordonnees);
      free((char *)xtab); 
      free((char *)ytab);
      free((char *)col); 
      free((char *)lig);
      free((char *)index_h); 
      free((char *)index_v);
      free((char *)pos_h); 
      free((char *)pos_v);
      free((char *)v_rect);
      EXIT(1);
    }
    free((char *)v_rect);
  }

  /*                                       */
  /* Liberation de la memoire                          */
  /*                                       */
  if (bool) 
    free((char *)coordonnees);
  free((char *)xtab); 
  free((char *)ytab);
  free((char *)col); 
  free((char *)lig);
  free((char *)index_h); 
  free((char *)index_v);
  free((char *)pos_h); 
  free((char *)pos_v);
}


/***********
*
*       ISPOLREC
*
*   Cette fonction prend la description d'un polygone et verifie si celui-ci est
*   rectangle, c'est-a-dire si les seuls angles qui ce retrouvent sur le  dessin
*   sont des angles droits (pas de fils a 45' ou autre).  Il suffit de  verifier
*   qu'il n'y a qu'une seule coordonnee qui change a la fois ( les seules direc-
*   tions permises sont donc N, E, S, W).
*
*  ENTREE : Tableau de structures contenant les coordonnees des points
*       Taille du tableau
*  RETOUR : 1 le polygone est rectangle
*       0 sinon
*
***/
int ispolyrec(coord_tab, coord_numb)
coord_t coord_tab[];
unsigned coord_numb;
{
register int i;

  for (i = 0; i < coord_numb - 1; i++)
    if (coord_tab[i].X != coord_tab[i+1].X &&
        coord_tab[i].Y != coord_tab[i+1].Y)
      return 0;
       
  return 1;
}
