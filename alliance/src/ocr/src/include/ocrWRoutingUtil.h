/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:17 $
   $Log: ocrWRoutingUtil.h,v $
   Revision 1.1  2002/03/15 14:37:17  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:14  hcl
   First commit in the new tree.

   Revision 1.2  2001/11/20 09:39:57  hcl
   du bo, du bon, prefet dubonnet

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

#ifndef _OCRWROUTINGUTIL_H_
#define _OCRWROUTINGUTIL_H_


extern ocrWRoutingGrid *createWGrid (ocrNaturalInt size_h,
							  ocrNaturalInt size_v,
							  ocrNaturalInt nb_of_layers);

void initWGrid (ocrWRoutingGrid * i_pGrid,
			 ocrRoutingParameters * i_pParam);

extern void freeWGrid (ocrWRoutingGrid * grid);

extern void setWSegmentName (ocrWSegment *segment, char *NAME);
extern ocrWSegment *createWSegment (ocrNaturalInt offset,
							 ocrNaturalInt layer,
							 ocrNaturalInt p_min,
							 ocrNaturalInt p_max,
							 ocrNaturalInt signal_index);

extern void freeWSegment (ocrWSegment * segment);

extern ocrWSegment *getWSegment (ocrWRoutingGrid * grid,
						   ocrNaturalInt x,
						   ocrNaturalInt y,
						   ocrNaturalInt z);

extern ocrWSegment *getWSegmentCV (ocrWRoutingGrid * grid,
							ocrVirtualConnector * i_pVirCon);

extern void setWGrid (ocrWRoutingGrid * grid,
				  ocrWSegment * segment,
				  ocrNaturalInt x,
				  ocrNaturalInt y,
				  ocrNaturalInt z);

extern int isLabeled (ocrWSegment * segment);

extern int isObstructed (ocrWSegment * segment);

extern ocrRoutingDirection getWSegDirection (ocrRoutingParameters * param,
									ocrWSegment * segment);

extern ocrNaturalInt getWSegXCoord (ocrRoutingParameters * param,
							 ocrWSegment * segment,
							 ocrNaturalInt point);

extern ocrNaturalInt getWSegYCoord (ocrRoutingParameters * param,
							 ocrWSegment * segment,
							 ocrNaturalInt point);

extern ocrWSegment *splitWSegment (ocrRoutingParameters * param,
							ocrWRoutingGrid * grid,
							ocrWSegment * segment,
							ocrNaturalInt first_point,
							ocrNaturalInt second_point,
							ocrNaturalInt index);

extern ocrNaturalInt manhattan1Distance (ocrRoutingParameters * param,
								 ocrWSegment * segment,
								 ocrNaturalInt p1,
								 ocrNaturalInt p2);

#define DELTA(x) ( (ocrNaturalInt)(x)->KEY )
#define POINT(x) ( (ocrNaturalInt)(x)->DATA )

extern ocrNaturalInt inducedDelta (ocrRoutingParameters * param,
							ocrWSegment * segment,
							ocrNaturalInt xtarget,
							ocrNaturalInt ytarget,
							rbtree * node,
							ocrNaturalInt point);

extern rbtree *getLabel (ocrRoutingParameters * param,
					ocrNaturalInt xtarget,
					ocrNaturalInt ytarget,
					ocrWSegment * segment,
					ocrNaturalInt delta,
					ocrNaturalInt point);

extern ocrNaturalInt getNextPlane (ocrWRoutingGrid * grid,
							ocrNaturalInt ztarget,
							ocrNaturalInt plane);

#endif /* _OCRWROUTINGUTIL_H_ */
