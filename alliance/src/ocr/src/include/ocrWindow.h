#ifndef _OCRWINDOW_H_
#define _OCRWINDOW_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:17 $
   $Log: ocrWindow.h,v $
   Revision 1.1  2002/03/15 14:37:17  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:14  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:46  hcl
   premier commit....


   ### -------------------------------------------------- ###
 */
/*
typedef struct ocrWindow
{
  ocrNaturalInt XMIN, XMAX;
  ocrNaturalInt YMIN, YMAX;
  ocrNaturalInt NUM;
  ocrNaturalInt NB_SIG_FACE[9];
  ocrNaturalInt NB_SIG;
  ocrNaturalInt NB_SIG_THROW;
}
ocrWindow;
*/
ocrWindow *newWindow (ocrNaturalInt i_nNumWin);
void deleteWindow (ocrWindow * i_pWindow);

void createWindow (ocrRoutingDataBase * i_pDataBase);
void translatingRealToWindows (ocrRoutingDataBase * i_pDataBase);

void
createWindowArray (ocrRoutingDataBase * i_pDataBase, 
					 ocrNaturalInt i_uWinNum);

void
deleteWindowArray (ocrRoutingDataBase * i_pDataBase,
					 ocrNaturalInt i_uWinNum);
void
translatingWindowsToReal (ocrRoutingDataBase * i_pDataBase);

void
WintoXY (ocrRoutingDataBase * i_pDataBase,
         ocrNaturalInt * o_pX, 
			ocrNaturalInt * o_pY,
			ocrNaturalInt i_uWin);

void
XYtoWin (ocrRoutingDataBase * i_pDataBase,
         ocrNaturalInt i_uX,
			ocrNaturalInt i_uY,
			ocrNaturalInt * o_pWin);

void
insertSignalWindow (ocrRoutingDataBase * i_pDataBase,
					 ocrSignal * i_pSignal);

void
linkConnector (ocrRoutingDataBase * i_pDataBase,
               ocrNaturalInt i_uIndex,
					ocrConnector * i_pCon);
#endif /* _OCRWINDOW_H_ */
