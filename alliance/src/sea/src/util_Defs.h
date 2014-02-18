/*
 *  $Id: util_Defs.h,v 1.5 2005/02/17 15:34:44 jpc Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./util_Defs.h"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __util_Defs__
#   define  __util_Defs__


#   include  <unistd.h>
#   include  <stdlib.h>
#   include  <stdio.h>
#   include  <signal.h>
#   include  <ctype.h>
#   include  <string.h>
#   include  <strings.h>
#   include  <math.h>


/*  ----------------------------------------------------------------------
 *  Looks if functions prototypes are availables.
 */

#   ifndef __FP
#     ifdef  __STDC__
        /* Compiler is ANSI C compliant. */
#       include  <unistd.h>
#       include  <stdarg.h>

#       define   __FP(  arguments )      arguments
#       define   __FPV( arguments )      arguments

#       define   __KR_C(   code )
#       define   __ANSI_C( code )        code
#     else
        /* Compiler is K&R C. */
#       include  <unistd.h>
#       include  <varargs.h>

#       define   __FP(  arguments )     ()
#       define   __FPV( arguments )     (va_alist) va_dcl

#       define   __KR_C(   code )        code
#       define   __ANSI_C( code )
#     endif
#   endif


/*  ----------------------------------------------------------------------
 *  Missing GNU functions prototypes under GCC/SunOS 4.1.4.
 */

#   include  "GNU_Defs.h"


/*  ----------------------------------------------------------------------
 *  Miscellaneous.
 */

#   define        TRUE        1
#   define       FALSE        0

#   define    m_Max(a,b)      (((a) > (b)) ? (a) :  (b))
#   define    m_Min(a,b)      (((a) > (b)) ? (b) :  (a))
#   define      ABS(a)        (((a) >  0 ) ? (a) : -(a))


  /* Usual types shortcut. */




/*  ----------------------------------------------------------------------
 *  Alliance Includes.
 */

#   include   <mut.h>
#   include   <mlo.h>
#   include   <mph.h>
#   include   <mut.h>
#   include   <mlu.h>
#   include   <mpu.h>
#   include   <aut.h>




/*  ----------------------------------------------------------------------
 *  Module  :  "util_Sys.c"
 */

#   define        F_DUMPCORE                  0
#   define        F_NOCORE                    1

#   define        C_SIGTFLT             ((int)1)
#   define        C_SIGERR              ((int)2)

#   define        C_VerboseLevel0           (0L)
#   define        C_VerboseLevel1           (1L)
#   define        C_VerboseLevel2           (2L)

#   define        SIZE_S64                 2048


  /* External variables from the various modules. */
  extern  long    util_VL;
  extern  long    util_flags;
  extern  char    util_binName[1024];

  /* Interruptions handling functions. */
  extern void  trapInit    __FP((void));
  /*extern void  sendGSignal __FP((int aSig));*/
  extern void  setBinName  __FP((char *asName));

  /* Errors printing functions. */
  extern void  stdflush    __FP((void));
  extern void  errMBK      __FP((char *asName));
  extern void  eprinth     __FP((char *asName));
  extern  int  eprintf     __FPV((char *aFormat, ...));
  extern  int  eprints     __FP((char *aS));

  /* Warnings printing functions. */
  extern void  warnMBK     __FP((char *asName));
  extern void  wprinth     __FP((char *asName));
  extern  int  wprintf     __FPV((char *aFormat, ...));
  extern  int  wprints     __FP((char *aS));

  /* Messages printing functions. */
  extern void  setVL       __FP((long aVL));
  extern long  getVL       __FP((void));
  extern  int  vmprintf    __FP((long aVL, char *aFormat, va_list aArgList));
  extern  int  mprintf     __FPV((long aVL, char *aFormat, ...));
  extern  int  mprints     __FP((long aVL, char *aS));
  extern  int  mprintf0    __FPV((char *aFormat, ...));
  extern  int  mprintf1    __FPV((char *aFormat, ...));
  extern  int  mprintf2    __FPV((char *aFormat, ...));

  /* "util_init" global initialisation function. */
  extern void  util_init   __FP((long aVL, long aFlags, char *asName));

  /* Persistent strings printing function (up to 64 strings). */
  extern char *s64printf   __FPV((char *aFormat, ...));

  /* Miscellaneous. */
  extern char *strtoup     __FP((char *aS));




/*  ----------------------------------------------------------------------
 *  Module  :  "util_Inter.c"
 */


  /* Interval list element. */
  typedef struct eInter_s {
                 long  min;
                 long  max;
      struct eInter_s *next;
  } eInter_t;

  /* Interval list head. */
  typedef struct lInter_s {
                 long  min;
                 long  max;
                 long  len;
      struct eInter_s *l;
  } lInter_t;


  extern            void  printinter   __FP((struct lInter_s *apInter));
  extern struct lInter_s *addinter     __FP((struct lInter_s *apInter,
                                                        long  aMin,
                                                        long  aMax));
  extern            void  freeinter    __FP((struct lInter_s *apInter));


/*  ----------------------------------------------------------------------
 *  Module  :  "util_MBK.c"
 */


#   define        F_EQUAL            (1L)
#   define        F_CALU             (2L)
#   define        F_TALU             (4L)
#   define        F_EQUAL_M          (F_EQUAL)
#   define        F_EQUAL_C          (F_EQUAL | F_CALU)
#   define        F_EQUAL_T          (F_EQUAL | F_TALU)

#   define        PTYPE_LOSEG        512
#   define        PTYPE_TLOSEG       513
#   define        PTYPE_ULOCON       514

#   define        LOSEG_VIA          1
#   define        LOSEG_SEG          2
#   define        LOSEG_CON          3
#   define        LOSEG_SEGCON       4

#   define        F_HAS_SUBNET       (1L<<0)
#   define        F_NET_ROUTED       (1L<<1)
#   define        F_SUBNET_ROUTED    (1L<<2)


#   define  MBKSCALE(l)    ((l) * SCALE_X)
#   define  MBKUNSCALE(l)  ((l) / SCALE_X)
#   define  NEWSIGINDEX    (++MBK_sigIndex)


  /* Model of physical VIA. */
  typedef struct eMVIA_s {
              char  type;
              long  width;
              long  height;
    struct eMVIA_s *next;
  } eMVIA_t;


  /* Added to each segment (USER pointer). */
  typedef struct eLoseg_s {
               long  type;
               void *MBKobj;
    struct eLoseg_s *next;
  } eLoseg_t;


  /* Added to the phfig (USER pointer). */
  typedef struct tLoseg_s {
                long   sigNB;    /* Number of signals.         */
                long   tSize;    /* Real table size.           */
    struct authtable  *tHT;      /* Sig name --> tLoseg index. */
                long  *tAccess;  /* Number of signal accesses. */
                long  *tFlags;   /* Some flags...              */
    struct eLoseg_s  **tLoseg;   /* Head of signal seg list.   */
    struct eMVIA_s    *tMVIA;    /* list of model VIAS.        */
  } tLoseg_t;


  extern long    MBK_sigIndex;


  extern long               isobs  __FP((struct phseg *apSeg));
  extern long   getsigaliasnumber  __FP((struct losig *apLosig));
  extern long          signamecmp  __FP((char *aLoname, char *aPhname));
  extern char   **makesegnamelist  __FP((char *aName));
  extern void         printMBKEnv  __FP((void));
  extern char      getAltVIALayer  __FP((char aVIA, char aLayer));
  extern char      getTopVIALayer  __FP((char aVIA));
  extern char   getBottomVIALayer  __FP((char aVIA));
  extern char          getTurnVIA  __FP((char aLayer));
  extern long          isVIALayer  __FP((char aVIA, char aLayer));
  extern long       getUpVIALayer  __FP((char aVIA, char aLayer));
  extern char          getUpLayer  __FP((char aLayer));
  extern long              cmpALU  __FP((char aLayer1, char aLayer2));
  extern char              getALU  __FP((char aLayer));
  extern char             getCALU  __FP((char aLayer));
  extern long              isCALU  __FP((char aLayer));
  extern char             getTALU  __FP((char aLayer));
  extern long  getLayerTrackWidth  __FP((char aLayer));
  extern void           xyflatseg  __FP((struct phseg *apFlatSeg,
                                         struct phseg *apSeg,
                                         long aXINS, long aYINS,
                                         long aXAB1, long aYAB1,
                                         long aXAB2, long aYAB2,
                                         char aTransf));
  extern void           xyflatvia  __FP((struct phvia *apFlatVIA,
                                         struct phvia *apVIA,
                                         long aXINS, long aYINS,
                                         long aXAB1, long aYAB1,
                                         long aXAB2, long aYAB2,
                                        char aTransf));

  /* Utilities for pad power splitting. */
  extern long                isPad  __FP((char *asCell));
  extern long         initSigIndex  __FP((struct losig *apLoSig));
  extern long           cmpSigName  __FP((struct losig *apLoSig, char *asName));
  extern struct locon *addInsLoCon  __FP((struct loins *apIns,
                                                  char *asName,
                                          struct losig *apSig,
                                                  char  aDir));
  extern void        splitPowerNet  __FP((struct lofig *apLoFig,
                                                  char *asPower));
  extern struct phins    *findphins __FP((struct phfig *apPhfig,
                                                  char *insName));
  extern struct phins  *findphmodel __FP((struct phfig *apPhfig,
                                                  char *modelName));
  extern struct locon    *findlocon __FP((struct lofig *apLofig,
                                                  char *conName));

  extern loins_list *addloins_noSig  __FP((lofig_list *apFig,
                                                 char *asIns,
                                           lofig_list *apModel));

  extern locon_list *addlosig_insCon __FP((loins_list *apIns,
                                                 char *asCon,
                                           losig_list *apSig));

  extern void      checkLofig  __FP((struct lofig *apLofig));
  extern void         addfeed  __FP((struct lofig *apLofig,
                                     struct phfig *apPhfig));
  extern void         delfeed  __FP((struct lofig *apLofig));
  extern void      copyUpCALU  __FP((struct lofig *apLofig,
                                     struct phfig *apPhfig,
                                             long  aExit));

  extern struct eMVIA_s        *addmvia __FP((struct  eMVIA_s *apHead,
                                              struct    phvia *apVIA));
  extern struct eMVIA_s        *getmvia __FP((struct  eMVIA_s *apHead,
                                              struct    phvia *apVIA));
  extern struct eLoseg_s      *addloseg __FP((struct eLoseg_s *apHead,
                                                         long  aType,
                                                         void *apSegvia));
  extern struct eLoseg_s      *delloseg __FP((struct eLoseg_s *apLoseg));
  extern struct tLoseg_s     *gettloseg __FP((struct    phfig *apPhfig));
  extern struct tLoseg_s     *addtloseg __FP((struct    phfig *apPhfig));
  extern struct authelem *gettlosegitem __FP((struct tLoseg_s *apTLoseg,
                                                         char *asKey));
  extern void                 freeloseg __FP((struct    phfig *apPhfig));
  extern void                phsegchain __FP((struct    lofig *apLofig,
                                              struct    phfig *apPhfig));
  extern struct eLoseg_s  *getloseglist __FP((struct    phfig *apPhfig,
                                                         char *aName));
  extern void          checklosegaccess __FP((struct    phfig *apPhfig));
  extern void                shiftphfig __FP((struct    phfig *apPhfig,
                                                         long  DX,
                                                         long  DY));




/*  ----------------------------------------------------------------------
 *  Module  :  "util_Power.c"
 */


#  define   F_POWER_COMPLETE              0x00000001
#  define   C_POWER_ISNONE                ((char)0)
#  define   C_POWER_ISVDD                 ((char)1)
#  define   C_POWER_ISVSS                 ((char)2)
#  define   C_POWER_VERTICAL              ((char)1)
#  define   C_POWER_HORIZONTAL            ((char)2)

#  define   F_POWER_MERGE                 0x00000001


 typedef struct ePow_s {
              long  flags;
              long  y;
              long  w;
              long  xMin;
              long  xMax;
              long  min;
              long  max;
              char *Name;
              char  Type;
   struct lInter_s *inter;
   struct ePow_s   *Next;
 } ePow_t;


  extern ePow_t *addPow     __FP((ePow_t **applPow,
                                    long   aY,
                                    long   aW,
                                    char  *aName,
                                    char   aType,
                                    char   aDir));
  extern void           freePow    __FP((struct ePow_s *aplPow));
  extern struct ePow_s *getPow     __FP((struct ePow_s *aplPow, long aY));
  extern void           mergeXPow  __FP((struct ePow_s *apPow,
                                                  long  axMin,
                                                  long  axMax,
                                                  long  aW));
  extern ePow_t *buildPow   __FP((phfig_list *apPhfig,
                                        char  aLayer,
                                        char  aDir,
                                        char *asSuffix));
  extern          void  powToCon   __FP(( struct phfig *apPhfig,
                                                  char  aLayer,
                                                  char  aDir,
                                         struct ePow_s *aplPow,
                                                  long  aFlags));




/*  ----------------------------------------------------------------------
 *  Module  :  "util_RTU.c"
 */


  extern void  rtu __FP((struct phfig *aPhfig));




/*  ----------------------------------------------------------------------
 *  Module  :  "util_Floorplan.c"
 */

#  define        F_FLOOR_LAYERS_3           0x00000010  /*   16. */
#  define        F_FLOOR_LAYERS_4           0x00000020  /*   16. */
#  define        F_FLOOR_LAYERS_6           0x00000040  /*   32. */
#  define        F_FLOOR_MARGIN             0x00000080  /*   64. */
#  define        F_FLOOR_ROW_NUMBER         0x00000100  /*  128. */
#  define        F_FLOOR_X_SIZE             0x00000200  /*  256. */
#  define        F_FLOOR_Y_SIZE             0x00000400  /*  512. */
#  define        F_FLOOR_ASPECT_RATIO       0x00000800  /* 1024. */

#  define        FLOOR_XY_EXPAND            MBKSCALE(50)


  extern         void  placeTerms      __FP((struct lofig *apLofig,
                                             struct phfig *apPhfig));
  extern struct phfig *makeFloorplan   __FP((struct lofig *apLofig,
                                                     long  aMargin,
                                                     long  aXSize,
                                                     long  aYSize,
                                                   double  aAspectRatio,
                                                     long  aPower,
                                                     long  aFlags));
  extern         void  expandFloorplan __FP((struct phfig *apPhfig));
  extern         void  shrinkFloorplan __FP((struct phfig *apPhfig));
  extern         void    protectPowmid __FP((struct phfig *apPhfig));




/*  ----------------------------------------------------------------------
 *  Module  :  "util_LEFDEF.c"
 */


#  include  "DEF_actions.h"


  /* LEF/DEF terminal direction. */
#   define        C_DIRECTION_NONE      ((char)0)
#   define        C_DIRECTION_INPUT     ((char)1)
#   define        C_DIRECTION_OUTPUT    ((char)2)
#   define        C_DIRECTION_TRISTATE  ((char)3)
#   define        C_DIRECTION_INOUT     ((char)4)
#   define        C_DIRECTION_FEEDTHRU  ((char)5)

#   define            X_GRID        5
#   define            Y_SLICE       50
#   define            WIDTH_VDD     6
#   define            WIDTH_VSS     6
#   define             VIA_SIDE     2
#   define         ALU1_SPACING     3
#   define        MBK_X_GRID        (X_GRID    * SCALE_X)
#   define        MBK_Y_SLICE       (Y_SLICE   * SCALE_X)
#   define        MBK_WIDTH_VDD     (WIDTH_VDD * SCALE_X)
#   define        MBK_WIDTH_VSS     (WIDTH_VSS * SCALE_X)
#   define        MBK_VIA_SIDE      (MBKSCALE (VIA_SIDE))
#   define        MBK_ALU1_SPACING  (MBKSCALE (ALU1_SPACING))
#   define        DEF_X_GRID        (X_GRID    * DEF_UNITS_MICRONS)
#   define        DEF_Y_SLICE       (Y_SLICE   * DEF_UNITS_MICRONS)


  extern long    DEF_UNITS_MICRONS;


  extern         long  DEF2MBK_length     __FP((long  aL));
  extern         char *DEF2MBK_name       __FP((char *aS));
  extern         char  DEF2MBK_transf     __FP((double aO));
  extern         char  DEF2MBK_direction  __FP((double aDEFdir));
  extern struct phref *DEF2MBK_row        __FP((struct phfig *apPhfig,
                                                        char *aRowName,
                                                        char *aRowType,
                                                        long  aOrient,
                                                        long  aDoNumber,
                                                        long  aByNumber,
                                                        long  aStepX,
                                                        long  aStepY,
                                                        long  aX,
                                                        long  aY));
  extern struct phref *DEF2MBK_track      __FP((struct phfig *apPhfig,
                                                        char *aAxisName,
                                                        long  aStart,
                                                        long  aDoNumber,
                                                        long  aStep,
                                                        char *asLayers,
                                                        long  aX,
                                                        long  aY));
  extern struct phref *DEF2MBK_blockage   __FP((struct phfig *apPhfig,
                                                        long aXB1,
                                                        long aYB1,
                                                        long aWidth,
                                                        long aHeight));

  extern long    MBK2DEF_length      __FP((long  aL));
  extern char   *MBK2DEF_name        __FP((char *asLEF, char *asMBK));
  extern char    MBK2DEF_direction   __FP((char  acDir));
  extern char    MBK2DEF_transf      __FP((char  aO));
  extern void    MBK2DEF_row         __FP((struct phref  *apPhref,
                                                   char **appRowName,
                                                   char **appRowType,
                                                   long  *apOrient,
                                                   long  *apDoNumber,
                                                   long  *apByNumber,
                                                   long  *apStepX,
                                                   long  *apStepY,
                                                   long  *apX,
                                                   long  *apY));
  extern void    MBK2DEF_track       __FP((struct phref  *apPhref,
                                                   char  *apAxisName,
                                                   long  *apStart,
                                                   long  *apDoNumber,
                                                   long  *apStep,
                                                   char **apsLayers,
                                                   long  *apX,
                                                   long  *apY));
  extern void    MBK2DEF_blockage    __FP((struct phref  *apPhref,
                                                   long  *apXB1,
                                                   long  *apYB1,
                                                   long  *apWidth,
                                                   long  *apHeight));
  extern char    MBK2DEF_locondir    __FP((struct locon *apLocon));
  extern char       *DEF_orient2a    __FP((long aT));
  extern long        DEF_a2orient    __FP((char *aT));
  extern char         *DEF_side2a    __FP((char aSide));
  extern char        *DEF_layer2a    __FP((char aLayer));
  extern char          *DEF_via2a    __FP((char  aType,
                                           long  aDX,
                                           long  aDY,
                                           char *aViaName));

  extern char    BEH2DEF_direction   __FP((char  acDir));


# endif
