#ifndef _OCRNPOINTS_H_
#define _OCRNPOINTS_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:15 $
   $Log: ocrNpoints.h,v $
   Revision 1.1  2002/03/15 14:37:15  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:12  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....


   ### -------------------------------------------------- ###
 */

ocrNaturalInt
findPathNPoints (ocrRoutingParameters * i_pParam,
                 ocrWRoutingGrid * i_pGrid,
                 ocrSignal * i_pSignal, ocrWindow * i_pWindow);
void
unifyPoint (ocrWRoutingGrid * i_pGrid,
            ocrRoutingParameters * i_pParam,
            ocrSignal * i_pSignal,
            ocrNaturalInt i_uXPoint,
            ocrNaturalInt i_uYPoint, ocrNaturalInt i_uZPoint);

ocrVirtualConnector *makeEquipotentielle (ocrRoutingParameters * i_pParam,
                                          ocrWRoutingGrid * i_pGrid,
                                          ocrSignal * i_pSignal);

void
deleteEquipotentielle (ocrRoutingParameters * i_pParam,
                       ocrWRoutingGrid * i_pGrid, ocrSignal * i_pSignal);
#endif /* _OCRNPOINTS_H_ */
