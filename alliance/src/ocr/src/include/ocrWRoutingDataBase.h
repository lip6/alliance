#ifndef _OCRWROUTINGDATABASE_H_
#define _OCRWROUTINGDATABASE_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/04/25 13:41:27 $
   $Log: ocrWRoutingDataBase.h,v $
   Revision 1.2  2002/04/25 13:41:27  hcl
   New ripup/reroute loop, bug-kill (CALU&TALU).

   Revision 1.1  2002/03/15 14:37:16  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:14  hcl
   First commit in the new tree.

   Revision 1.5  2002/02/21 13:17:46  hcl
   Introducing a new algo (A*, dont yet work...)

   Revision 1.4  2002/02/12 15:14:00  hcl
   verbosity fix.

   Revision 1.3  2001/12/03 14:31:11  hcl
   Pour la route.

   Revision 1.2  2001/11/20 09:39:57  hcl
   du bo, du bon, prefet dubonnet

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

#define WSEGMENT_FREE        OCRNATURALINT_MAX
#define WSEGMENT_OBSTACLE    (OCRNATURALINT_MAX - 1)

typedef enum rbtreecolour
  {
    rbtreered,
    rbtreeblack,
  }
rbtreecolour;

typedef struct rbtree
  {
    void *KEY;
    void *DATA;
    rbtreecolour COLOUR;
    struct rbtree *PARENT;
    struct rbtree *Left;
    struct rbtree *Right;
  }
rbtree;

/**
 * TYPE CONNECTEUR
**/

#define	OCR_TYPE_FACE		1
#define	OCR_TYPE_PONC		2
#define	OCR_TYPE_LINKED	4

#define MARK_AS_LINKED(x) 			(x)->TYPE |= OCR_TYPE_LINKED
#define IS_MARK_AS_LINKED(x)		((x)->TYPE & OCR_TYPE_LINKED)
#define MARK_AS_FACE_CON(x) 			(x)->TYPE |= OCR_TYPE_FACE
#define IS_MARK_AS_FACE_CON(x)		((x)->TYPE & OCR_TYPE_FACE)

#define  OCR_FACE_NORTH	1
#define  OCR_FACE_SOUTH	2
#define  OCR_FACE_EAST	4
#define  OCR_FACE_WEST	8

/**
 * TYPE SIGNAL
**/

// signal n'ayant pas de conecteurs ds la window
#define OCR_SIG_THROW	1

#define MARK_AS_SIG_THROW(x)		(x)->TYPE |= OCR_SIG_THROW
#define IS_MARK_AS_SIG_THROW(x)  ((x)->TYPE & OCR_SIG_THROW)


/*
   Enrichir cette structure pour tenir compte d'un cout intrisinseque ?
*/
typedef struct ocrWSegment
  {
    ocrNaturalInt OFFSET;
    ocrNaturalInt LAYER;
    ocrNaturalInt P_MIN;
    ocrNaturalInt P_MAX;
    ocrNaturalInt SIGNAL_INDEX;

    ocrNaturalInt TAG;
    ocrNaturalInt H, COST, HCOST;

    /* list of pointers to associated obstacle segments */
    //struct ocrWSegment *OBSTACLE;
    /* list of pointer used for backtracking */
    struct ocrWSegment *ROOT;
    /* temporarily chaining */
    //struct ocrWSegment *AUX;
    
    /* Pointer used for chaining equi */
    struct ocrWSegment *NEXT;



    ocrNaturalInt LT, RB;
    rbtree *LABELS_LEFT_TOP;
    rbtree *LABELS_RIGHT_BOTTOM;
  }
ocrWSegment;

// Grille de routage
typedef struct ocrWRoutingGrid
  {
    ocrWSegment **DATA;
    ocrNaturalInt SIZE_H;
    ocrNaturalInt SIZE_V;
    ocrNaturalInt NB_OF_LAYERS;
  }
ocrWRoutingGrid;

typedef struct ocrVirtualConnector
  {

    ocrNaturalInt WIDTH;
    ocrNaturalShort ORIENT;

    ocrNaturalShort TAG;
    ocrNaturalShort WIN;
    ocrNaturalInt DIST;

    ocrNaturalInt X;
    ocrNaturalInt Y;
    ocrNaturalInt Z;
    struct ocrVirtualConnector *NEXT;
  }
ocrVirtualConnector;

typedef struct ocrConnector
  {
    char *NAME;
    char *INSNAME;
    struct ocrConnector *NEXT;
    ocrNaturalShort TYPE;
    ocrNaturalShort INTEXT;

    ocrVirtualConnector *VIR_CON_LIST;
    ocrVirtualConnector *CON;

    ocrNaturalInt ORDER;

    ocrNaturalShort FACE;
    ocrNaturalShort WIN;
    ocrNaturalInt DIST;

    /* XXX HCl */
    ocrNaturalInt NB_VC;
    ocrVirtualConnector *critVC;
  }
ocrConnector;

/* ajout HCl */
typedef struct ocrPoint {

	ocrNaturalInt X;
	ocrNaturalInt Y;
	ocrNaturalInt Z;

	ocrNaturalInt SIGNAL_INDEX;

	struct ocrPoint *NEXT;
} ocrPoint;


typedef struct ocrSignal
  {
    char *NAME;
    ocrNaturalInt INDEX;
    char ROUTED;
    struct ocrSignal *NEXT;

    ocrWSegment *SEGMENT;
    ocrVirtualConnector *VIA;
    ocrVirtualConnector *GLOBAL;

    ocrNaturalInt NB_CON;
    ocrNaturalInt TYPE;
    ocrNaturalShort WIN;
    ocrNaturalShort HARD;
    ocrNaturalShort PRIORITY;
    ocrNaturalShort INTEXT;
    ocrConnector *CON_LIST;

    /* HCl XXX */
    ocrNaturalInt NICHT_ZU_ROUTIEREN;
  }
ocrSignal;


typedef struct ocrWindow
{
  ocrNaturalInt XMIN, XMAX;
  ocrNaturalInt YMIN, YMAX;
  ocrNaturalInt NUM;
  ocrNaturalInt NB_SIG_FACE[9];
  ocrNaturalInt NB_SIG;
  ocrNaturalInt NB_SIG_THROW;
}
ocrWindow;


typedef struct ocrObstacle
  {
  }
ocrObstacle;

typedef struct ocrRoutingDataBase
  {
    char *NAME;
    ocrNaturalInt XAB1, YAB1, XAB2, YAB2;

    ocrNaturalShort NB_OF_LAYERS;

    ocrNaturalInt NB_IT;
    ocrNaturalInt NB_UNROUTED;
    ocrNaturalInt NB_ROUTED;

    ocrNaturalInt NB_F;

    ocrSignal **GSIGNAL;
    ocrSignal **SIGNAL;
    ocrSignal **FSIGNAL;

    ocrNaturalInt NB_GSIGNAL;
    ocrNaturalInt NB_SIGNAL;
    ocrNaturalInt *NB_FSIGNAL;

    ocrWindow **WINDOWS;
    ocrPoint *POINTS;

    ocrObstacle *GOBSTACLE;
    ocrObstacle **MFOBSTACLE;
    ocrObstacle **FOBSTACLE;

    ht *HTABLE;
    ocrWRoutingGrid *GRID;
    ocrRoutingParameters *PARAM;
    chain_list *RIPUP;
  }
ocrRoutingDataBase;

#define OCR_NORM	1
#define OCR_RIPUP	2
#define OCR_ODYN	4

typedef struct ocrOption
  {
    ocrNaturalInt LEVEL;
    ocrNaturalInt ORDER;
    ocrNaturalInt PRIORITY;
    ocrNaturalInt WINDOW;
    ocrNaturalInt ALGO;
  }
ocrOption;


/* Fonctions de base */

ocrRoutingDataBase *createDataBase (
							  phfig_list * i_pPhFig);

void deleteDataBase (
				  ocrRoutingDataBase * i_pDataBase);

ocrSignal *createSignal (
					 losig_list * i_pLoSig);

ocrSignal *dupSignal (
				   ocrSignal * i_pSignal);

void createGlobalSignalList (
						ocrRoutingDataBase * i_pDataBase,
						ocrNaturalInt i_uNbSignaux);

void addSignalGlobal (
				   ocrRoutingDataBase * i_pDataBase,
				   ocrSignal * i_pSignal);

ocrConnector *createConnector (
						  char *i_sName,
						  char *i_sInsName,
						  ocrNaturalShort i_uType,
						  ocrNaturalShort i_uIntExt,
						  ocrNaturalShort i_uFace,
						  ocrNaturalInt i_uOrder,
						  ocrNaturalInt i_uNumFMF);

void addConnector (
				ocrSignal * i_pSignal,
				ocrConnector * i_pCon);

ocrVirtualConnector *createVirtualConnector (
									 ocrNaturalInt i_uX,
									 ocrNaturalInt i_uY,
									 ocrNaturalInt i_uZ,
									 ocrNaturalInt i_uWidth,
									 ocrNaturalShort i_uOrient);

ocrVirtualConnector *dupVirtualConnector (
								   ocrVirtualConnector * i_pVirCon);

void addVirtualConnector (
					  ocrVirtualConnector ** i_pVirConList,
					  ocrVirtualConnector * i_pVirCon);

void deleteVirtualConnectorList (ocrVirtualConnector * i_pVirCon);
void deleteSegmentSignalList (ocrSignal * i_pSignal,
						ocrWSegment * i_pSegment);
void addSegmentSignalList (ocrSignal * i_pSignal,
					  ocrWSegment * i_pSegment);
void dumpDataBase (ocrRoutingDataBase * i_pDataBase,
			    FILE * i_pFile);

#endif /* _OCRWROUTINGDATABASE_H_ */
