
/* ###--------------------------------------------------------------### */
/*									*/
/* file		: mvl_stype.h						*/
/* date		: Oct 15 1991						*/
/* author	: P. BAZARGAN, L.A. TABUSSE, VUONG H.N.			*/
/*									*/
/* contents	: This file contains defines and structure definitions	*/
/*		  for the structural compiler				*/
/*									*/
/* ###--------------------------------------------------------------### */

typedef struct
  {
  char            *NAME;          /* identifier name */
  short            LEFT;          /* vector's left index */
  short            RIGHT;         /* vector's right index */
  short            ERR_FLG;
  }
mvl_name;

typedef struct
  {
  short         WIDTH;			/* expression's width		*/
  struct chain *LIST;			/* list of losig pointers	*/
  }
mvl_expr;
