#ifndef _OCRSIGNALUTIL_H_
#define _OCRSIGNALUTIL_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:15 $
   $Log: ocrSignalUtil.h,v $
   Revision 1.1  2002/03/15 14:37:15  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:13  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....


   ### -------------------------------------------------- ###
 */

#define  ORDER_RANDOM				1
#define  ORDER_CON					2
#define  ORDER_PRIORITY				3
#define  ORDER_PRIORITY_CON		4
#define  ORDER_PRIORITY_RANDOM	5

void initGlobalSignalList (ocrRoutingDataBase * i_pDataBase,
                           lofig_list * i_pLoFig);

void orderingSignals (ocrSignal ** i_pSignal,
                      ocrNaturalInt i_uNbSig, ocrNaturalShort i_uType);

ocrNaturalInt
isCriticalSignal (ocrRoutingDataBase * i_pDataBase, ocrNaturalInt i_uRang);

void linkSignal (ocrRoutingDataBase * i_pDataBase,
					 ocrSignal * i_pSignal);
#endif /* _OCRSIGNALUTIL_H_ */
