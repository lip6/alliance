#ifndef _OCRWSEGLABELING_H_
#define _OCRWSEGLABELING_H_
/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:17 $
   $Log: ocrWSegLabeling.h,v $
   Revision 1.1  2002/03/15 14:37:17  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:14  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

extern int labelWSegment(ocrRoutingParameters *param,
                         ocrWSegment *segment,
                         ocrNaturalInt xtarget,
                         ocrNaturalInt ytarget,
                         ocrNaturalInt delta,
                         ocrNaturalInt point);

extern void unlabelWSegment(ocrWSegment *segment);

#endif /* _OCRWSEGLABELING_H_ */
