#ifndef __P
# if defined(__STDC__) ||  defined(__GNUC__)
#  define __P(x) x
# else
#  define __P(x) ()
# endif
#endif
/*------------------------------------------------------------\
|                                                             |
| Tool    :                    RDSGDS                         |
|                                                             |
| File    :                     gds.h                         |
|                                                             |
| Authors :                 Pierre Vittet                     |
|                    Modified by Jacomme Ludovic              |
| Date    :                   04/07/93                        |
|                                                             |
\------------------------------------------------------------*/

# ifndef RDSGDS_GDS
# define RDSGDS_GDS

/*------------------------------------------------------------\
|                                                             |
|                           Constants                         |
|                                                             |
\------------------------------------------------------------*/

/*********
*
*  Voici la LISTE DES CODES GDSII correspondant aux differents types d'enregistrement.
*
* Un fichier GDSII est compose d'enregistrements de taille variable. Au debut de chacun
* de ces enregistrements, on trouve invariablement :
*   - 2 octets indiquant la taille de l'enregistrement (ces 2 octets compris).
*   - un code GDS egalement sur 2 octets (ou le 1er octet des deux donne le type
*   de l'enregistrement et le 2eme precise le type des donnees contenues dans cet
*   enregistrement).
*
***/

#define HEADER       0x0002
#define BGNLIB       0x0102
#define LIBNAME      0x0206
#define UNITS        0x0305
#define ENDLIB       0x0400
#define BGNSTR       0x0502
#define STRNAME      0x0606
#define ENDSTR       0x0700
#define BOUNDARY     0x0800
#define PATH         0x0900
#define SREF         0x0A00
#define AREF         0x0B00
#define TEXT         0x0C00
#define LAYER0       0x0D02
#define DATATYPE     0x0E02
#define WIDTH        0x0F03
#define XY           0x1003
#define ENDEL        0x1100
#define SNAME        0x1206
#define COLROW       0x1302
#define TEXTNODE     0x1400
#define NODE         0x1500
#define TEXTTYPE     0x1602
#define PRESENTATION 0x1701

#define STRING       0x1906
#define STRANS       0x1A01
#define MAG          0x1B05
#define ANGLE        0x1C05

#define REFLIBS      0x1F06
#define FONTS        0x2006
#define PATHTYPE     0x2102
#define GENERATIONS  0x2202
#define ATTRTABLE    0x2306
#define STYPTABLE    0x2406
#define STRTYPE      0x2502
#define ELFLAGS      0x2601
#define ELKEY        0x2703

#define NODETYPE     0x2A02
#define PROPATTR     0x2B02
#define PROPVALUE    0x2C06
#define BOX          0x2D00
#define BOXTYPE      0x2E02
#define PLEX         0x2F03
#define BGNEXTN      0x3003
#define ENDEXTN      0x3103
#define TAPENUM      0x3202
#define TAPECODE     0x3302
#define STRCLASS     0x3401

#define FORMAT       0x3602
#define MASK         0x3706
#define ENDMASKS     0x3800


/***********
*
* Autres constantes utilisees par le format GDS
* Les valeurs hexa etant a masquer dans les champs qui les necessitent
*
***/

#define LGPROP 126   /* maximum length of the string value associated with the attribute */

/* reserved attribute numbers */
#define PUSRINT    126   /* user integer property        */
#define PUSRSTR    127   /* user string or CSD property  */

#define LGSTRNAME 32   /* maximum length of a "structure"'s name */
#define LGSTRING 512   /* maximum length of a character string for text presentation */

/* text presentation flags */
#define THPRES    0x0003   /* horizontal presentation */
#define THLEFT    0x0000      /*   left      */
#define THCENT    0x0001          /*    center   */
#define THRIGHT    0x0002         /*     right   */
#define TVPRES    0x000C   /*  vertical presentation */
#define TVTOP    0x0000      /*   top       */
#define TVMID    0x0004          /*    middle   */
#define TVBOT    0x0008         /*     bottom  */
#define TFONT    0x0030   /* ---- type of font ---- */
#define TFONT0    0x0000      /*   font 0     */
#define TFONT1    0x0010          /*    font 1    */
#define TFONT2    0x0020         /*     font 2   */
#define TFONT3    0x0030        /*      font 3  */

/* graphic presentation flags */
#define GABSANG    0x0002   /* absolute angle */
#define GABSMAG    0x0004   /* absolute magnification */
#define GREFLECT 0x8000   /* reflection over the X-axis applied before */
          /* angular rotation */

/* element flags */
#define ETEMPL    0x0001   /* TEMPLATE data */
#define EXTERN    0x0002   /* EXTERNAL data */

/* pathtype flags */
#define PSQRFLH 0   /* square-ended path that ends flush with its endpoints */
#define PRND 1   /* round-ended paths */
#define PSQRHW 2   /* square-ended path that ends half-width beyond its endpoints */
#define PVSQR 4   /* STICKS products : path with variable square-ended   */
                          /* extensions */

/* Calma flags : internal use with STICKS products */
#define NOCALMA    0x0000   /* Stream tapes produced by non-Calma progams */

/* Autres constantes existant : nombre maximale de valeurs possibles */
#define NLAYER    64   /* The value of the layer must be in the range of 0 to 63.   */
#define NDATATYPE 64   /* The value of the datatype must be in the range of 0 to 63.   */
#define NTEXTTYPE 64   /* The value of the texttype must be in the range of 0 to 63.   */
#define NNODETYPE 64   /* The value of the nodetype must be in the range of 0 to 63.   */
#define NBOXTYPE 64   /* The value of the boxtype must be in the range of 0 to 63.   */

#define LGNAME    12   /* max length of library's name, according to CDOS file name conventions   */
#define LGLIBNAME 44   /* max length of each reference library's name   */
#define NFONTS    4   /* maximum authorized number of textfonts definitions   */
#define LGFONTNAME 44   /* names, each name is 44 (maximum) characters long   */
#define LGATTRNAME 44   /* max length of attribute definition file's name   */

/* format type values */
#define FARC 0   /* ARCHIVE format */
#define FILT 1   /* FILTERED format */

/*------------------------------------------------------------\
|                                                             |
|                            Macro                            |
|                                                             |
\------------------------------------------------------------*/

# define CheckPhysicalGridAligned(c) ((c) % RDS_PHYSICAL_GRID)

/*------------------------------------------------------------\
|                                                             |
|                            Types                            |
|                                                             |
\------------------------------------------------------------*/

#ifndef ushort
#define ushort unsigned short
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define  type_i   unsigned short
#define  type_l   unsigned short
#define  type_c   unsigned short

typedef struct {                 /* enregistrement de la date et de l'heure */
  short   year;                           /*  annee                 */
  short   month;                         /*   mois                 */
  short   day;                          /*    jour                */
  short   hour;                        /*     heure              */
  short   min;                        /*      minutes           */
  short   sec;                       /*       secondes         */
} date_type;

typedef struct {                        /* en-tete de l'enregistrement contient les infos :  */
        short size;                    /*       longueur de l'enregistrement                */
        short gdscode;                /*        record type & data type                    */
} hinfo_type;

typedef double unit_type;          /* database unit type */
typedef double angle_type;         /* angular rotation factor type */
typedef double mag_type;           /* magnification factor type */

/*------------------------------------------------------------\
|                                                             |
|                          Variables                          |
|                                                             |
\------------------------------------------------------------*/
/*------------------------------------------------------------\
|                                                             |
|                          Functions                          |
|                                                             |
\------------------------------------------------------------*/

# endif
