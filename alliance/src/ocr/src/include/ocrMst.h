#ifndef _OCRMST_H_
#define _OCRMST_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:15 $
   $Log: ocrMst.h,v $
   Revision 1.1  2002/03/15 14:37:15  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:12  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   ### -------------------------------------------------- ###
 */

#define ocrNode ocrVirtualConnector

typedef struct ocrEdge
  {
    struct ocrEdge *NEXT;
    ocrNaturalInt COST;
    ocrNode *N1;
    ocrNode *N2;
  }
ocrEdge;


// Fonctions de base

ocrEdge * makingMst (ocrSignal * i_pSignal);
#endif /* _OCRMST_H_ */
