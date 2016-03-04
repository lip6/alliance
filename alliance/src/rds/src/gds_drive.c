/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSGDS                         |
|                                                             |
| File    :                  gds_drive.c                      |
|                                                             |
| Authors :                 Pierre Vittet                     |
|                    Modified by Jacomme Ludovic              |
| Date    :                   07/06/04                        |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                         Include Files                       |
|                                                             |
\------------------------------------------------------------*/

# include <stdio.h>
# include <math.h>
# include <time.h>
# include <string.h>
# include <sys/types.h>
# include <sys/time.h>


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
# include "gds_drive.h"
# include "gds_swap.h"

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

  static short FIRST_MODEL;

  static short GDS_EXPERT_DRIVER_MODE = 0;
  static short GDS_FIRST_MODEL_MODE   = 0;
  
/*------------------------------------------------------------\
|                                                             |
|                             Macros                          |
|                                                             |
\------------------------------------------------------------*/

#define controle(a)                                                       \
         if (a != numb) {                                                 \
         pv_init_error();                                                 \
         pv_error.v_error = ENOSPACE;                                     \
         pv_give_error("controle");                                       \
         (void)fclose(fp);                                                \
         EXIT(1); }


#define entete(a,b)                                                       \
         infobuf.size = sizeof(hinfo_type) + b/sizeof(char);              \
         infobuf.gdscode = a;                                             \
         if (islittle()) {                                                \
            infobuf.size    = swaps(infobuf.size);                        \
            infobuf.gdscode = swaps(infobuf.gdscode);                     \
         }                                                                \
         numb = fwrite((char *)&infobuf, sizeof(hinfo_type), 1, fp);      \
         controle(1)

#define cadre numb = fwrite(&CADRE, sizeof(char), 1, fp); controle(1)

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

/* variables globales mais internes au fichier */

char CADRE = '\0';   /* pour recommencer a ecrire sur un octet pair, on cadre avec le  */
         /* caractere nul en comblant l'octet impair.          */

/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/
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
/***********
*
*  DOUBLE_TO_GDSREAL
*
*  Cette fonction prend un nombre flottant et le traduit dans un format propre
*  aux reels des fichiers GDS2.
*
*  ENTREE : nombre flottant a traduire
*  precision voulue 1 : simple precision
*         2 : double precision
*
*  RETOUR : pointeur sur une chaine de caracteres contenant la traduction.
*      NULL si manque de place memoire.
*        depassement de capacite du reel GDS
*
*  REMARQUE : Cette chaine sera sur 4 ou 8 octets et ne sera pas terminee
*     par un '\0'.
*
**/
static char *
   pv_double_to_gdsreal( value, precision )
double   value;
short precision;
{
   register int   i;
   char     buffer[56], *pt;

   pt = (char *)calloc((unsigned)precision * 4, sizeof(char));
   if (pt == (char *)NULL) {
      pv_init_error();
      pv_error.v_error = ENOMEM;
      pv_give_error("double_to_gdsreal");
      return((char *)NULL);
   }
   if (value == 0) return(pt); /* la valeur 0 est representee par une zone remplie de 0   */

   /* On commence par s'occuper du signe du nombre.   */
   *pt = (value > 0)? '\0' : '\300';
   value = fabs(value);

   /* on va chercher la valeur de l'exposant du nombre exprime en base 16.       */
   i = 0;
   while (value >= 1.0) {  /* si le nombre correspond a une puissance positive de 16   */
      value /= 16.0;
      i++;
   }
   while (value < 0.0625) {/* si le nombre correspond a une puissance negative de 16   */
      value *= 16.0;
      i--;
   }
   if (i > 63) {     /* On prevoit les cas d'overflow.   */
      pv_init_error();
      pv_error.v_error = EOVERFL;
      pv_error.v_textp = "La valeur absolue du nombre est trop grande pour etre traduite en GDS.\nLa limite est : 7.23 E+75";
      free(pt);
      pv_give_error("double_to_gdsreal");
      return((char *)NULL);
   } else if (i < -64) {         /* et d'underflow.                  */
      pv_init_error();
      pv_error.v_error = EOVERFL;
      pv_error.v_textp = "La valeur absolue du nombre est trop petite pour etre traduite en GDS.\nLa limite est : 5.4 E-79";
      free(pt);
      pv_give_error("double_to_gdsreal");
      return((char *)NULL);
   }

   i += 64; /* On biaise l'exposant a 64. Ici, on est sur que  0 <= exp <= 127 (donc sur 7 bits)   */
   *pt |= (char)i;   /* On vient d'ecrire le signe et la valeur de l'exposant biaise a 64       */

   /* On va maintenant decomposer, bit a bit, la mantisse dans un tableau de caracteres.        */
   for (i = 0; i < ( 24 + 32*(precision-1) ); i++) {
      value *= 2;
      if (value >= 1) {
         buffer[i] = '\1';
         value -= 1;
      } else buffer[i] = '\0';
   }  /* Puis on replace tous ces bits au sein de quelques octets (3 ou 7 selon la precision)   */
   for (i = 0; i < ( 24 + 32*(precision-1) ); i++)
      pt[1+i/8] |= buffer[i] << (7 - i%8);

   return(pt);
}

/***********
*
*  ECRIT_GDSREAL
*
* Cette fonction est utilisee pour afficher a l'ecran ce a quoi ressemble un reel au format GDS2.
* Elle ne sert en fait a rien si ce n'est a verifier ce que rend la fonction double_to_gdsreal.
*
****/

# if 0
static void
   pv_ecrit_gdsreal( value_str, size )
char  *value_str;
short size;
{
   register int   i, j, k;
   char     buffer[65];
   char     buffer2[72];

   k = 0;
   for (i = 0; i < size; i++)
      for (j = 7; j >= 0; j--) /*   Pour chaque bit de l'octet */

         buffer[k++] = (((value_str[i] >> j) & 0x0001) != 0) ? '1' : '0'; /* Stocker sa valeur dans le tableau */

   for (i = 0, j = 0; i < size*8; i++) {
      if ( (i != 0) && (i%8 == 0) ) buffer2[j++] = ' ';
      buffer2[j] = buffer[i];
      j++;
   }
   buffer2[j] = '\0';
   return;
}
# endif

/***********
*
*  RDS_TO_GDS_SYM
*
* Cette fonction prend la definition d'une symetrie en RDS et la transpose en
* son equivalent en GDS. En fait, il s'agit aussi d'une scission,  le  format
* GDS separant clairement la notion de reflection et la rotation.
*
*  ENTREE : la symetrie RDS
*     adresse du flag pour memoriser si il y a reflection autour de l'axe
*        des abcisses
*     adresse de stockage de l'angle de rotation
*
*  RETOUR : aucun
*
***/
static void
   pv_rds_to_gds_sym( rds_sym, refx_p, angle_p )
char rds_sym;
short *refx_p;
double *angle_p;
{
   switch (rds_sym) {
      case RDS_NOSYM :
         *refx_p  = 0;
         *angle_p = 0.0;
         break;
      case RDS_ROT_P :
         *refx_p  = 0;
         *angle_p = 90.0;
         break;
      case RDS_SYMXY :
         *refx_p  = 0;
         *angle_p = 180.0;
         break;
      case RDS_ROT_M :
         *refx_p  = 0;
         *angle_p = 270.0;
         break;
      case RDS_SYM_X :
         *refx_p  = 1;
         *angle_p = 180.0;
         break;
      case RDS_SY_RM :
         *refx_p  = 1;
         *angle_p = 270.0;
         break;
      case RDS_SYM_Y :
         *refx_p  = 1;
         *angle_p = 0.0;
         break;
      case RDS_SY_RP :
         *refx_p  = 1;
         *angle_p = 90.0;
   }
}



/***********
*
*  SAUVE_RECTANGLE
*
***/
static int
   pv_sauve_rectangle( rect, fp, layer )
rdsrec_list *rect;
FILE  *fp;
short layer;
{
register int numb;
hinfo_type   infobuf;
short        datatype = 0;
int          bool = FALSE;
coord_t      tab[6]; /* last one reserved for text */

   /* A connectors is written using a specific layer from now on:
    * this implies a simple change of layer */
   if ((IsRdsConExter(rect) || IsRdsRefCon (rect)) && !IsRdsVia(rect))
      layer = GET_GDS_CONNECTOR_LAYER(layer);
   else
      layer = GET_GDS_LAYER(layer);

   tab[0].X = rect->X;
   tab[0].Y = rect->Y;
   tab[1].X = rect->X;
   tab[1].Y = rect->Y + rect->DY;
   tab[2].X = rect->X + rect->DX;
   tab[2].Y = rect->Y + rect->DY;
   tab[3].X = rect->X + rect->DX;
   tab[3].Y = rect->Y;
   tab[4].X = rect->X;
   tab[4].Y = rect->Y;
   tab[5].X = rect->X + (rect->DX >> 2);
   tab[5].Y = rect->Y + (rect->DY >> 2);

   entete(BOUNDARY, 0);

   entete(LAYER0, sizeof(short));
   controle(1)
   if (islittle()) {
      layer    = swaps(layer);
      datatype = swaps(datatype);
      tab[0].X = swapl(tab[0].X);
      tab[0].Y = swapl(tab[0].Y);
      tab[1].X = swapl(tab[1].X);
      tab[1].Y = swapl(tab[1].Y);
      tab[2].X = swapl(tab[2].X);
      tab[2].Y = swapl(tab[2].Y);
      tab[3].X = swapl(tab[3].X);
      tab[3].Y = swapl(tab[3].Y);
      tab[4].X = swapl(tab[4].X);
      tab[4].Y = swapl(tab[4].Y);
      tab[5].X = swapl(tab[5].X);
      tab[5].Y = swapl(tab[5].Y);
   }
   numb = fwrite((char *)&layer, sizeof(short), 1, fp);
   controle(1);

   entete(DATATYPE, sizeof(short));
   numb = fwrite((char *)&datatype, sizeof(short), 1, fp);
   controle(1);

   entete(XY, 5 * sizeof(coord_t));
   numb = fwrite((char *)tab, sizeof(coord_t), 5, fp);
   controle(5);

   entete(ENDEL, 0);
   /* If the rectangle contains some text, even if it is not a connector,
      we drive the text: nodes should be sufficients.
      Frederic Petrot: 10/04/96 */
   /* sauve uniquement les noms du premier model (le pere) 18/04/2002 FW */
   if (FIRST_MODEL && rect->NAME != NULL) {
      /* on a besoin de mettre des crochets autour des index de vecteur
       * 26/06/2002 FW */
      char *pindex;
      char buff [16000];
      strncpy (buff, rect->NAME, 16000);
      buff[15999] = '\0';
      pindex = strchr (buff, (int)' ');
      if (pindex) {
         *pindex = '[';
         strcat (buff,"]");
      }

      entete(TEXT, 0);

      entete(LAYER0, sizeof(short));
      numb = fwrite((char *)&layer, sizeof(short), 1, fp);
      controle(1);

      /* TEXTTYPE values is set to 0, who cares */

      entete(TEXTTYPE, sizeof(short));
      layer = 0x00; 
      numb = fwrite((char *)&layer, sizeof(short), 1, fp);
      controle(1);

      entete(XY, sizeof(coord_t));
      numb = fwrite((char *)&tab[5], sizeof(coord_t), 1, fp);
      controle(1);

      numb = strlen(buff);
      if ((numb % 2) != 0) {
         numb += 1;
         bool = TRUE;
      }

      entete(STRING, numb * sizeof(char));
      if (fputs(buff, fp) < 0) {
         pv_init_error();
         pv_error.v_error = ENOSPACE;
         (void)fclose(fp);
         pv_give_error("text_string");
         return(-1);
      }
      if (bool) cadre;

      entete(ENDEL, 0);
   }
   return 0;
}

/***********
*
*  SAUVE_INSTANCE
*
***/

static int
   pv_sauve_instance( inst, fp )
rdsins_list *inst;
FILE  *fp;
{
   register int numb;
   hinfo_type infobuf;
   int     bool;
   char    *gds_angle;
   short   ref_x_axis;
   short   prop_attr;
   double  rds_angle;
   ushort  strans;

   entete(SREF, 0);

   bool = FALSE;
   numb = strlen(inst->FIGNAME);
   if ( (numb % 2) != 0 ) {/* Si la longueur du nom est impaire, on la rend paire   */
      numb += 1;  /* car chaque enregistrement doit comporter un nombre */
      bool = TRUE;   /* pair de caracteres dans un fichier GDS.      */
   }
   entete(SNAME, numb * sizeof(char));
   if (fputs(inst->FIGNAME, fp) < 0 ) {
      pv_init_error();
      pv_error.v_error = ENOSPACE;
      (void)fclose(fp);
      pv_give_error("sauve_instance");
      return(-1);
   }
   if (bool) cadre;

   pv_rds_to_gds_sym(inst->TRANSF, &ref_x_axis, &rds_angle);
   if (ref_x_axis || rds_angle) {
      strans = (ref_x_axis != 0) ? GREFLECT : 0;
      entete(STRANS, sizeof(ushort));
      if (islittle())
         strans = swaps(strans);
      numb = fwrite((char *)&strans, sizeof(ushort), 1, fp);
      controle(1);
   }

   if (rds_angle) {
      entete(ANGLE, sizeof(angle_type));
      gds_angle = pv_double_to_gdsreal(rds_angle, 2);
      if (gds_angle == (char *)NULL) {
         (void)fclose(fp);
         return(-1);
      }
      numb = fwrite(gds_angle, sizeof(angle_type), 1, fp);
      free(gds_angle);
      controle(1);
   }

   entete(XY, sizeof(coord_t));
   coord_t xy;
   xy.X = inst->X;
   xy.Y = inst->Y;

   if (islittle()) {
      xy.X = swapi(xy.X);
      xy.Y = swapi(xy.Y);

      numb = fwrite((char *)&xy.X, sizeof(int32_t), 1, fp);
      controle(1);
      numb = fwrite((char *)&xy.Y, sizeof(int32_t), 1, fp);
      controle(1);
   } else {
      numb = fwrite((char *)&xy.X, sizeof(int32_t), 1, fp);
      controle(1);
      numb = fwrite((char *)&xy.Y, sizeof(int32_t), 1, fp);
      controle(1);
   }

   if ( GDS_EXPERT_DRIVER_MODE )
   {
/* LUDO */
     entete(PROPATTR,sizeof(ushort));
     prop_attr = 120;
     if (islittle()) prop_attr = swaps( prop_attr );
     numb = fwrite((char *)&prop_attr, sizeof(ushort), 1, fp);
     controle(1);
   
     bool = FALSE;
     numb = strlen(inst->INSNAME);
     if ( (numb % 2) != 0 ) {/* Si la longueur du nom est impaire, on la rend paire   */
        numb += 1;  /* car chaque enregistrement doit comporter un nombre */
        bool = TRUE;   /* pair de caracteres dans un fichier GDS.      */
     }
     entete(PROPVALUE, numb * sizeof(char));
     if (fputs(inst->INSNAME, fp) < 0 ) {
        pv_init_error();
        pv_error.v_error = ENOSPACE;
        (void)fclose(fp);
        pv_give_error("sauve_instance");
        return(-1);
     }
     if (bool) cadre;
/* END_LUDO */
   }

   entete(ENDEL, 0);
   return(0);
}

/****************
*
*  SAUVE_MODELE
*
**********/
static int
   pv_sauve_modele( modele, fp, date )
rdsfig_list *modele;
FILE  *fp;
date_type  *date;
{
   register int   numb, i;
   register rdsrec_list *rect;
   register rdsins_list *inst = modele->INSTANCE;
   hinfo_type     infobuf;
   int      bool = FALSE;

   entete(BGNSTR, 2 * sizeof(date_type));
   numb = fwrite((char *)&date, sizeof(date_type), 1, fp);
   controle(1);
   numb = fwrite((char *)&date, sizeof(date_type), 1, fp);
   controle(1);

   numb = strlen(modele->NAME);
   if ( (numb % 2) != 0 ) {   /* Si la longueur du nom est impaire, on la rend paire */
      numb += 1;     /* car chaque enregistrement doit comporter un nombre  */
      bool = TRUE;      /* pair de caracteres dans un fichier GDS.             */
   }
   entete(STRNAME, numb * sizeof(char));
   if ( fputs(modele->NAME, fp) < 0 ) {
      pv_init_error();
      pv_error.v_error = ENOSPACE;
      (void)fclose(fp);
      pv_give_error("sauve_modele");
      return(-1);
   }
   if (bool) cadre;

   for (i = 0; i < RDS_MAX_LAYER; i++) {
      /* layer to be driven :
         Layers that the user does not want, or need, to drive in GDS are
         supposed to be set to -1, illegal GDS layer number. */
      if (GET_GDS_LAYER(i) != RDS_GDS_LAYER_EMPTY) {
         if (modele->LAYERTAB[i]) {
            rect = modele->LAYERTAB[i];
            while (rect) {
               if (pv_sauve_rectangle(rect, fp, i) < 0) return -1;
               rect = rect->NEXT;
            }
         }
      }
   }

   while (inst) {
      if (pv_sauve_instance(inst, fp) < 0) return -1;
      inst = inst->NEXT;
   }

   entete(ENDSTR, 0);

   return 0;              /* tout s'est bien passe pendant la sauvegarde sur disque de la structure. */
}


/*------------------------------------------------------------\
|                                                             |
|                         gdssaverdsfig                       |
|                                                             |
\------------------------------------------------------------*/

 void gdssaverdsfig( Figure )

    rdsfig_list *Figure;
{
int bool = FALSE;
register int numb;
register FILE *fp;
hinfo_type infobuf;
short version = 3;
date_type date;
struct tm *p;
time_t t;
char *u_unit, *m_unit;
ptype_list *model_list;

   pv_warning = 1;   /* 1 valeur par defaut, envoi de messages.   */
/*
*  recherche d'un modele RDS dans la liste et retour de son pointeur,
*  pointeur sur la figure pere a sauver; son nom servira a creer
*  le nom de la librairie GDS (nom interne)
*/
   GDS_FIRST_MODEL_MODE = 0;
   GDS_EXPERT_DRIVER_MODE = 0;

   if ( getenv( "RDS_GDS_FIRST_MODEL_MODE") ) GDS_FIRST_MODEL_MODE = 1;

   if ( getenv( "RDS_GDS_EXPERT_DRIVER" ) )
   {
     GDS_EXPERT_DRIVER_MODE = 1;
     GDS_FIRST_MODEL_MODE   = 0;
   }

   model_list = (ptype_list *)reverse((chain_list *)getrdsmodellist(Figure));

   if ( (fp = mbkfopen(Figure->NAME, "gds", "w")) == NULL ) {
      pv_init_error();
      pv_error.v_error = ENOFILE;
      pv_error.v_textp = Figure->NAME;
      pv_give_error("savegds");
      EXIT(1); 
   }

   tzset();
   if (time(&t) == (time_t)-1) { /* recuperation de la date GMT du jour */
      (void)fclose(fp);
      pv_init_error();
      pv_error.v_error = ECLOCK;
      pv_give_error("savegds");
      EXIT(1);   
   }
   p = localtime(&t);          /* decompactage de la date GMT en date locale */
   if (p == (struct tm *)NULL) {
      (void)fclose(fp);
      pv_init_error();
      pv_error.v_error = ENOMEM;
      pv_give_error("savegds");
      EXIT(1);
   }
   if (islittle()) {
      date.year  = swaps(p->tm_year);
      date.month = swaps(p->tm_mon);
      date.day   = swaps(p->tm_mday);
      date.hour  = swaps(p->tm_hour);
      date.min   = swaps(p->tm_min);
      date.sec   = swaps(p->tm_sec);
      version    = swaps(version);        
   } else {
      date.year  = p->tm_year;
      date.month = p->tm_mon;
      date.day   = p->tm_mday;
      date.hour  = p->tm_hour;
      date.min   = p->tm_min;
      date.sec   = p->tm_sec;
   }

   entete(HEADER, sizeof(short));
   numb = fwrite((char *)&version, sizeof(short), 1, fp);
   controle(1);

   entete(BGNLIB, 2 * sizeof(date_type));
   numb = fwrite((char *)&date, sizeof(date_type), 1, fp);
   controle(1);
   numb = fwrite((char *)&date, sizeof(date_type), 1, fp);
   controle(1);

   numb = strlen(Figure->NAME);
   if ((numb % 2) != 0) {/* Si la longueur du nom est impaire, on la rend paire  */
      numb += 1;  /* car chaque enregistrement doit comporter un nombre */
      bool = TRUE;   /* pair de caracteres dans un fichier GDS.      */
   }
   entete(LIBNAME, numb * sizeof(char));
   if ( fputs(Figure->NAME, fp) < 0 ) {
      pv_init_error();
      pv_error.v_error = ENOSPACE;
      (void)fclose(fp);
      pv_give_error("savegds");
      EXIT(1);
   }
   if (bool)
      cadre;

   entete(UNITS, 2 * sizeof(unit_type));
   /* who cares about user defined unit ? */
   u_unit = pv_double_to_gdsreal(1.0 / RDS_UNIT, 2);
   /* cas d'underflow, d'overflow ou de manque d'espace memoire */
   if (u_unit == (char *)NULL) {
      (void)fclose(fp);
      EXIT(1);
   }
   numb = fwrite(u_unit, sizeof(unit_type), 1, fp);
   free(u_unit);
   controle(1);
   m_unit = pv_double_to_gdsreal( 1.0e-6 / RDS_UNIT, 2);

   /* cas d'underflow, d'overflow ou de manque d'espace memoire */
   if (m_unit == (char *)NULL) {
      (void)fclose(fp);
      EXIT(1);
   }
   numb = fwrite(m_unit, sizeof(unit_type), 1, fp);
   free(m_unit);
   controle(1);

   FIRST_MODEL = 1;
   while (model_list) {
      if (pv_sauve_modele((rdsfig_list *)model_list->DATA, fp, &date) < 0)
         EXIT(1);
      model_list = model_list->NEXT;
      if ( GDS_FIRST_MODEL_MODE ) FIRST_MODEL = 0;
   }

   entete(ENDLIB, 0);

   if (fclose(fp) < 0) {
         pv_init_error();
         pv_error.v_error = EIO;
         pv_error.v_textp = "Probleme a la fermeture du fichier.";
         pv_give_error("savegds");
         EXIT(1);
      }
}
