#ifndef _OCRCONNECTORUTIL_H_
#define _OCRCONNECTORUTIL_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/04/25 13:41:27 $
   $Log: ocrConnectorUtil.h,v $
   Revision 1.2  2002/04/25 13:41:27  hcl
   New ripup/reroute loop, bug-kill (CALU&TALU).

   Revision 1.1  2002/03/15 14:37:14  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:12  hcl
   First commit in the new tree.

   Revision 1.2  2001/11/20 09:39:57  hcl
   du bo, du bon, prefet dubonnet

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

#define OCR_BAD_CON1	2
#define OCR_BAD_CON2	3

ocrNaturalInt distBetween2VirCon (
							ocrVirtualConnector * l_pVirCon1,
							ocrVirtualConnector * l_pVirCon2);

void ChooseExtConnector (
					 ocrRoutingDataBase * i_pDataBase);

void ChooseIntConnector (
					 ocrRoutingDataBase * i_pDataBase);

void createHashTable (
				   ocrRoutingDataBase * i_pDataBase,
				   phfig_list * i_pPhFig);

void deleteHashTable (
				   ocrRoutingDataBase * i_pDataBase);

ocrNaturalInt isFreePoint (ocrConnector *l_pCon, ocrWRoutingGrid * i_pGrid,
					  ocrVirtualConnector * l_pVirCon);

ocrNaturalShort chooseInternalConnector (ocrWRoutingGrid * i_pGrid,
								 ocrConnector * i_pCon,
								 ocrConnector * i_pCon2,
								 int mode);

ocrNaturalShort
chooseInternalConnector2(ocrWRoutingGrid * i_pGrid,
                         ocrConnector * i_pCon);
void initConnectorList (ocrRoutingDataBase * i_pDataBase,
				    phfig_list * i_pPhFig,
				    lofig_list * i_pLoFig);

ocrNaturalInt
distBetween2VirCon (ocrVirtualConnector * l_pVirCon1,
                    ocrVirtualConnector * l_pVirCon2);

void
deleteConVirInConVirList (ocrVirtualConnector ** io_VirConList,
                          ocrVirtualConnector * i_pConVir);

ocrNaturalInt
isCriticalConnector (ocrWRoutingGrid * i_pGrid,
					 ocrConnector * i_pCon);

void
markSegmentAsFree(ocrRoutingDataBase * i_pDataBase, ocrSignal * i_pSignal,
                  ocrNaturalInt INDEX);

void
unMarkSegmentAsFree(ocrRoutingDataBase * i_pDataBase,
                    ocrSignal * i_pSignal, ocrNaturalInt INDEX);

void
protectVC(ocrRoutingDataBase * i_pDataBase,
          ocrVirtualConnector * i_pVirCon);
void
unProtectVC(ocrRoutingDataBase * i_pDataBase,
          ocrVirtualConnector * i_pVirCon);
void countFreeVC(ocrRoutingDataBase * i_pDataBase);
#endif /* _OCRCONNECTORUTIL_H_ */
