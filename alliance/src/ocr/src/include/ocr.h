#ifndef _OCR_H_
#define _OCR_H_

/*
   ### -------------------------------------------------- ### 
   $Author: hcl $
   $Date: 2002/03/15 14:37:14 $
   $Log: ocr.h,v $
   Revision 1.1  2002/03/15 14:37:14  hcl
   Ca roule.

   Revision 1.1  2002/03/15 09:54:11  hcl
   First commit in the new tree.

   Revision 1.1  2001/09/26 14:27:45  hcl
   premier commit....

   Revision 1.3  2000/01/25 15:49:00  root
   *** empty log message ***

   ### -------------------------------------------------- ###
 */

#define OCRNATURALINT_MAX    0xFFFFFFFF
#define OCRNATURALSHORT_MAX  0xFF

#define OCR_OK	1

typedef unsigned long ocrNaturalInt;
typedef unsigned char ocrNaturalShort;
typedef long ocrInt;

typedef enum ocrRoutingDirection {
  ocrVertical = 0,
  ocrHorizontal = 1
} ocrRoutingDirection;

typedef struct ocrRoutingParameters {
  ocrNaturalShort      PITCH_H;
  ocrNaturalShort      PITCH_V;
  ocrNaturalShort      VIA_COST;
  ocrRoutingDirection  EVEN_LAYERS_DIRECTION;
} ocrRoutingParameters;

#endif /* _OCR_H_ */
