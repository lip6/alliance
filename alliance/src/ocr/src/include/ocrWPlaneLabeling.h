#ifndef _OCRWPLANELABELING_H_
#define _OCRWPLANELABELING_H_
/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:16 $
   $Log: ocrWPlaneLabeling.h,v $
   Revision 1.1  2002/03/15 14:37:16  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:13  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

extern void freeNextDeltaSubWSegList(chain_list **list,
                                     ocrNaturalInt *nb_of_elements,
                                     ocrNaturalInt layer);

extern ocrNaturalInt processNextDelta(ocrRoutingParameters *param,
                                      ocrWRoutingGrid *grid,
                                      ocrNaturalInt xtarget,
                                      ocrNaturalInt ytarget,
                                      ocrNaturalInt delta,
                                      chain_list **labeled_segment_list,
                                      chain_list **next_delta_sub_segment_list,
                                      ocrNaturalInt *nb_of_sub_segments,
                                      chain_list **segment_garbage);

extern void labelPlane(ocrRoutingParameters *param,
                       ocrWRoutingGrid *grid,
                       ocrNaturalInt xtarget,
                       ocrNaturalInt ytarget,
                       ocrNaturalInt ztarget,
                       ocrNaturalInt delta_sce,
                       ocrNaturalInt plane_sce,
                       ocrNaturalInt plane_dest,
                       chain_list **next_delta_sub_segment_list,
                       ocrNaturalInt *nb_of_sub_segments,
                       chain_list **labeled_segment_list);

extern void initNextDeltaSubWSegList(ocrRoutingParameters *param,
                                     ocrWRoutingGrid *grid,
                                     ocrNaturalInt xtarget,
                                     ocrNaturalInt ytarget,
                                     chain_list ***list,
                                     ocrNaturalInt **nb_of_elements,
                                     ocrWSegment *segment_source,
                                     ocrNaturalInt xsource,
                                     ocrNaturalInt ysource);

extern void initLabeledWSegList(ocrWRoutingGrid *grid,
                                chain_list ***list,
                                ocrWSegment *segment_source);

extern void cleanLabeledWSegments(ocrWRoutingGrid *grid,
                                  chain_list **labeled_segment_list,
                                  chain_list **segment_garbage);

extern int isNextDeltaSubWSegListNotEmpty(ocrWRoutingGrid *grid,
                                          chain_list **list);

#endif /* _OCRWPLANELABELING_H_ */
