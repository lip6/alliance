#ifndef _OCRWEIGTHEDTREE_H_
#define _OCRWEIGTHEDTREE_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:17 $
   $Log: ocrWeightedTree.h,v $
   Revision 1.1  2002/03/15 14:37:17  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:14  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:46  hcl
   premier commit....

   ### -------------------------------------------------- ###
 */

void makingWeightedTree (ocrRoutingDataBase *,
					ocrSignal *,
					ocrEdge *);

void initWeightedTree (ocrRoutingDataBase * i_pDataBase);
void freeWeightedTree ();
void dumpDensityTable ();
#endif /* _OCRWEIGTHEDTREE_H_ */
