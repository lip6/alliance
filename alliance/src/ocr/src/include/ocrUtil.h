#ifndef _OCRUTIL_H_
#define _OCRUTIL_H_
/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:16 $
   $Log: ocrUtil.h,v $
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

#define ABSDIFF(a,b)      ( (a) > (b) ? (a) - (b) : (b) - (a) )
#define MIN(a,b)          ( (a) <= (b) ? (a) : (b) )
#define MAX(a,b)          ( (a) >= (b) ? (a) : (b) )

extern ocrRoutingParameters *setParameters(ocrNaturalShort pitch_h,
                                           ocrNaturalShort pitch_v,
                                           ocrNaturalShort via_cost,
                                           ocrRoutingDirection dir_of_even_layers);

extern ocrNaturalInt manhattan3Distance(ocrRoutingParameters *param,
                                        ocrNaturalInt x1,
                                        ocrNaturalInt y1,
                                        ocrNaturalInt z1,
                                        ocrNaturalInt x2,
                                        ocrNaturalInt y2,
                                        ocrNaturalInt z2);

extern ocrNaturalInt manhattan2Distance(ocrRoutingParameters *param,
                                        ocrNaturalInt x1,
                                        ocrNaturalInt y1,
                                        ocrNaturalInt x2,
                                        ocrNaturalInt y2);

extern ocrRoutingDirection getDirection(ocrRoutingParameters *param,
                                        ocrNaturalInt z);

#endif /* _OCRUTIL_H_ */
