#ifndef _OCRTOPHFIG_H_
#define _OCRTOPHFIG_H_
/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:16 $
   $Log: ocrToPhFig.h,v $
   Revision 1.1  2002/03/15 14:37:16  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:13  hcl
   First commit in the new tree.

   Revision 1.2  2002/01/14 10:34:27  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.2  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */
void addViaToPhFig (
				 ocrRoutingParameters * i_pParam,
				 ocrNaturalInt i_uX,
				 ocrNaturalInt i_uY,
				 int i_uLayer,
				 phfig_list * io_pPhFig);

void dumpIntExtConToPhFig (
					ocrRoutingDataBase * i_pDataBase,
					ocrRoutingParameters * i_pParam,
					phfig_list * i_pPhFig);

void dumpAlimToPhFig (
				   ocrRoutingDataBase * i_pDataBase,
				   phfig_list * i_pPhFig);

ocrNaturalInt dumpGridToPhFig (
						  ocrRoutingParameters * i_pParam,
						  ocrWRoutingGrid * i_pGrid,
						  phfig_list * i_pPhFig);

void
addViaToPhFig (ocrRoutingParameters * i_pParam,
			ocrNaturalInt i_uX,
			ocrNaturalInt i_uY,
			int i_nLayer,
			phfig_list * io_pPhFig);

void
addSegmentToPhFig (ocrRoutingParameters * i_pParam,
			    ocrWSegment * i_pSegment,
			    phfig_list * io_pPhFig,
                            char *i_pName);

void
dumpPhFigToDisk (char *i_sName,
			  phfig_list * i_pPhFig);
#endif /* _OCRTOPHFIG_H_ */
