/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:16 $
   $Log: ocrWRouting.h,v $
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
#ifndef _OCRWROUTING_H_
#define _OCRWROUTING_H_

extern ocrNaturalInt findPathBetween2Points(ocrRoutingParameters *param,
                                            ocrWRoutingGrid *grid,
                                            ocrNaturalInt xsource,
                                            ocrNaturalInt ysource,
                                            ocrNaturalInt zsource,
                                            ocrNaturalInt xtarget,
                                            ocrNaturalInt ytarget,
                                            ocrNaturalInt ztarget,
                                            ocrNaturalInt signal_index,
														  ocrSignal * i_pSignal);

extern void displayPath(ocrRoutingParameters *param,
                        ocrWRoutingGrid *grid,
                        ocrNaturalInt xsource,
                        ocrNaturalInt ysource,
                        ocrNaturalInt zsource,
                        ocrNaturalInt xtarget,
                        ocrNaturalInt ytarget,
                        ocrNaturalInt ztarget);

#endif /* _OCRWROUTING_H_ */
