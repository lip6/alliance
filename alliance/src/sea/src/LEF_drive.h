/*
 *  $Id: LEF_drive.h,v 1.2 2002/09/30 16:21:17 czo Exp $
 *
 *  /----------------------------------------------------------------\
 *  |                                                                |
 *  |        A l l i a n c e   C A D   S y s t e m                   |
 *  |  S i l i c o n   E n s e m b l e / A l l i a n c e             |
 *  |                                                                |
 *  |  Author    :                      Jean-Paul CHAPUT             |
 *  |  E-mail    :         alliance-users@asim.lip6.fr             |
 *  | ============================================================== |
 *  |  C Header  :         "./LEF_drive.h"                           |
 *  | ************************************************************** |
 *  |  U p d a t e s                                                 |
 *  |                                                                |
 *  \----------------------------------------------------------------/
 */


# ifndef  __LEF_drive__
#   define  __LEF_drive__


#   define        C_CLASS_NONE          ((char)0)
#   define        C_CLASS_COVER         ((char)1)
#   define        C_CLASS_RING          ((char)2)
#   define        C_CLASS_PAD           ((char)3)
#   define        C_CLASS_CORE          ((char)4)
#   define        C_CLASS_FEEDTHRU      ((char)5)
#   define        C_CLASS_ENDCAP        ((char)6)
#   define        C_CLASS_TOPRIGHT      ((char)7)
#   define        C_CLASS_TOPLEFT       ((char)8)

#   define        F_SYMMETRY_NONE       ((char)0)
#   define        F_SYMMETRY_X          ((char)1)
#   define        F_SYMMETRY_Y          ((char)2)
#   define        F_SYMMETRY_R90        ((char)4)
#   define        F_SYMMETRY_XY         ((char)(  F_SYMMETRY_X   \
                                                | F_SYMMETRY_Y   ))
#   define        F_SYMMETRY_XYR90      ((char)(  F_SYMMETRY_X   \
                                                | F_SYMMETRY_Y   \
                                                | F_SYMMETRY_R90 ))

#   define        C_USE_NONE            ((char)0)
#   define        C_USE_SIGNAL          ((char)1)
#   define        C_USE_ANALOG          ((char)2)
#   define        C_USE_POWER           ((char)3)
#   define        C_USE_GROUND          ((char)4)
#   define        C_USE_CLOCK           ((char)5)

#   define        C_SHAPE_NONE          ((char)0)
#   define        C_SHAPE_ABUTMENT      ((char)1)
#   define        C_SHAPE_RING          ((char)2)
#   define        C_SHAPE_FEEDTHRU      ((char)3)

#   define        C_PORTITEM_LAYER      ((char)1)
#   define        C_PORTITEM_WIDTH      ((char)2)
#   define        C_PORTITEM_PATH       ((char)3)
#   define        C_PORTITEM_RECT       ((char)4)
#   define        C_OBSITEM_LAYER       C_PORTITEM_LAYER
#   define        C_OBSITEM_WIDTH       C_PORTITEM_WIDTH
#   define        C_OBSITEM_PATH        C_PORTITEM_PATH
#   define        C_OBSITEM_RECT        C_PORTITEM_RECT


 typedef struct  XY_s { long x; long y; }  XY_t;


 typedef struct ePATH_s {
      struct XY_s  pt;
   struct ePATH_s *Next;
 } ePATH_t;


 typedef struct RECT_s {
   struct XY_s  pt0;
   struct XY_s  pt1;
 } RECT_t;


 typedef struct ePORT_s {
               char  Type;
               void *Data;
     struct ePORT_s *Next;
 } ePORT_t;


 typedef struct eOBS_s {
               char  Type;
               void *Data;
     struct eOBS_s *Next;
 } eOBS_t;


 typedef struct ePIN_s {
               char *pinName;
               char  DIRECTION;
               char  USE;
               char  SHAPE;
     struct ePORT_s *lPORT;
      struct ePIN_s *Next;
 } ePIN_t;


 typedef struct eMACRO_s {
                char *macroName;
                char  CLASS;
         struct XY_s  ORIGIN;
         struct XY_s  SIZE;
                char  SYMMETRY;
                char *SITE;
       struct ePIN_s *lPIN;
       struct eOBS_s *lOBS;
     struct eMACRO_s *Next;       /* The next element... */
 } eMACRO_t;


#   define  MICRONS(m)  (((float)(m)) / DEF_UNITS_MICRONS)

#   define  m_AddPin(p,n)     ((p) = allocPIN((p), (n)))
#   define  m_AddPort(p,t,d)  ((p) = allocPORT((p), (t), (void*)(d)))
#   define  m_RevPort(p)      ((p) = revPORT((p)))
#   define  m_AddObs(p,t,d)   ((p) = allocOBS((p), (t), (void*)(d)))
#   define  m_RevObs(p)       ((p) = revOBS((p)))

#   define  m_AddRect(x0,y0,x1,y1) (allocRECT(x0, y0, x1, y1))

#   define  m_AddPath(p,x,y)  ((p) = allocPATH((p), x, y))

#   define  m_RevPath(p)      ((p) = revPATH((p)))


 extern   RECT_t *allocRECT   __FP((long aX0, long aY0, long aX1, long aY1));
 extern  ePATH_t *allocPATH   __FP((ePATH_t *alPATH, long aX, long aY));
 extern  ePATH_t *revPATH     __FP((ePATH_t *apPATH));
 extern   eOBS_t *allocOBS    __FP((eOBS_t *alOBS, char aType, void *apData));
 extern   eOBS_t *revOBS      __FP((eOBS_t *apOBS));
 extern  ePORT_t *allocPORT   __FP((ePORT_t *alPORT, char aType, void *apData));
 extern  ePORT_t *revPORT     __FP((ePORT_t *apPORT));
 extern   ePIN_t *allocPIN    __FP((ePIN_t *alPin, char *asName));
 extern   ePIN_t *getPIN      __FP((eMACRO_t *apMACRO, char *asName));
 extern eMACRO_t *allocMACRO  __FP((char *asName));
 extern   void    fprintMACRO __FP((FILE *LEF, eMACRO_t *apMACRO));


# endif
