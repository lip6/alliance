#ifndef _OCR_DISPLAY_H_
#define _OCR_DISPLAY_H_
/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:13 $
   $Log: display.h,v $
   Revision 1.1  2002/03/15 14:37:13  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:11  hcl
   First commit in the new tree.

   Revision 1.3  2002/02/12 15:13:59  hcl
   verbosity fix.

   Revision 1.2  2002/01/14 10:34:27  hcl
   OCR should be MBK_SCALE_X - compliant

   Revision 1.1  2001/09/26 14:27:44  hcl
   premier commit....

   ### -------------------------------------------------- ### 
 */

// Type
enum {ERROR,WARNING,INFO,DEBUG,VERBOSE,VVERB};

void display (int i_nLevel, int i_nType, char *fmt,...);

#endif /* _OCR_DISPLAY_H_ */
