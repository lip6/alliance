/* 
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail support     : mailto:alliance-support@asim.lip6.fr
 * 
 * This library is free software; you  can redistribute it and/or modify it
 * under the terms  of the GNU Library General Public  License as published
 * by the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
 * useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 * 
 * You should have received a copy  of the GNU General Public License along
 * with the GNU C Library; see the  file COPYING. If not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* ###--------------------------------------------------------------### */
/* file		: pat109.h						*/
/* date		: Mar 10 2000						*/
/* version	: v109							*/
/* authors	: Pirouz BAZARGAN SABET					*/
/* description	: header for PAT library				*/
/* ###--------------------------------------------------------------### */

#ifndef PAT_PATDEF
#define PAT_PATDEF

	/* ###------------------------------------------------------### */
	/*    defines							*/
	/* ###------------------------------------------------------### */

#define PAT_SEQ__IOLARRAY	0x01	/* paseq FLAGS: array of paiols	*/
#define PAT_SEQ__ZERODELAY	0x02	/* paseq FLAGS: zero delay desc	*/

#define PAT_IOL__SPY            0x01    /* paiol FLAG : spied		*/
#define PAT_IOL__TRACE          0x02    /* paiol FLAG : traced		*/

#define PAT_PAT__SPY            0x01    /* papat FLAG : spy pattern	*/

#define PAT_SEQ__TIMEUNIT	0x07	/* time unit			*/
#define PAT_TU__VU		   0	/* time unit : virtual unit	*/
#define PAT_TU__FS		   1	/* time unit : fs		*/
#define PAT_TU__PS		   2	/* time unit : ps		*/
#define PAT_TU__NS		   3	/* time unit : ns		*/
#define PAT_TU__US		   4	/* time unit : us		*/
#define PAT_TU__MS		   5	/* time unit : ms		*/

	/* ###------------------------------------------------------### */
	/*    global variables						*/
	/* ###------------------------------------------------------### */

extern unsigned int  PAT_LINNUM  ;

extern unsigned char PAT_TIMEUNIT;
extern unsigned int  PAT_TIMESTEP;

extern float         PAT_CNV_FS  ;
extern float         PAT_CNV_PS  ;
extern float         PAT_CNV_NS  ;
extern float         PAT_CNV_US  ;
extern float         PAT_CNV_MS  ;

	/* ###------------------------------------------------------### */
	/*    data structures						*/
	/* ###------------------------------------------------------### */

typedef struct paseq			/* sequence of patterns		*/
  {
  struct paseq   *NEXT     ;		/* next sequence		*/
  char           *NAME     ;		/* sequence's name		*/
  struct pagrp   *PAGRP    ;		/* list of declared arraies	*/
  struct paiol   *PAIOL    ;		/* list of inputs-outputs	*/
  struct pacom   *DECCOM   ;		/* declaration part comments	*/
  struct papat   *OLDPAT   ;		/* old list of patterns		*/
  struct pacom   *OLDCOM   ;		/* old list of comments		*/
  struct papat   *CURPAT   ;		/* current patterns		*/
  struct pacom   *CURCOM   ;		/* current comments		*/
  struct papat   *NXTPAT   ;		/* next patterns		*/
  char           *MODEL    ;		/* model of a pttern		*/
  char           *BUFFER   ;		/* accu. buffer to build pettern*/
  struct pains   *PAINS    ;		/* inspected instances		*/
  unsigned int    PATNBR   ;		/* # of patterns for simulation	*/
  unsigned int    LINENO   ;		/* current line number		*/
  unsigned short  IOLNBR   ;		/* # of inputs-outputs		*/
  unsigned short  SUBSEQ   ;		/* subsequence number		*/
  unsigned short  DRVSEQ   ;		/* last drived subsequence	*/
  char            SAVFLG   ;		/* save flag 'Y' or 'N'		*/
  char            ENDFLG   ;		/* end  flag 'Y' or 'N'		*/
  unsigned char   ERRFLG   ;		/* error flag			*/
  unsigned char   FLAGS    ;		/* flags			*/
  unsigned char   TIME_UNIT;		/* time unit			*/
  unsigned int    TIME_STEP;		/* time step			*/
  }
paseq_list;

typedef struct pains			/* inspected instance		*/
  {
  struct pains   *NEXT     ;		/* next inspected instance	*/
  char           *INSNAME  ;		/* instance name		*/
  char           *FILNAME  ;		/* file name			*/
  void           *INSTANCE ;		/* instance pointer		*/
  FILE           *FILPNT   ;		/* file pointer			*/
  char           *MODEL    ;		/* model of generated pattern	*/
  char          **VALUE    ;		/* instance's interface values	*/
  }
pains_list;

typedef struct pagrp			/* actual or virtual array	*/
  {
  struct pagrp   *NEXT     ;		/* next array			*/
  char           *NAME     ;		/* array's name			*/
  unsigned short  LENGTH   ;		/* array's number of bit	*/
  unsigned short  FINDEX   ;		/* index of the first paiol	*/
  short           FLAG     ;		/* actual or virtual (0,1)	*/
  }
pagrp_list;

typedef struct paiol			/* input-output signal		*/
  {
  struct paiol   *NEXT     ;		/* next input-output		*/
  char           *NAME     ;		/* input-output's name		*/
  void           *SIG      ;		/* pointer on the signal	*/
  char            FORMAT   ;		/* read-write format B,O,X	*/
  char            MODE     ;		/* mode I,B,O,Z,T,R,S,X,W,U	*/
  char            BLANK    ;		/* number of blank columns	*/
  char            VALUE    ;		/* (not used)			*/
  char            FLAG     ;		/* flag				*/
  }
paiol_list;

typedef struct papat			/* pattern			*/
  {
  struct papat   *NEXT     ;		/* next pattern			*/
  char           *LABEL    ;		/* pattern's label		*/
  struct paini   *PAINI    ;		/* list of initializations	*/
  struct paevt   *PAEVT    ;		/* list of events		*/
  unsigned int    TIME     ;		/* pattern's absolute time	*/
  unsigned short  LINE     ;		/* pattern's line number	*/
  char            BLANK    ;		/* number of blank lines	*/
  char            ACTFLAG  ;		/* action flag (U,I)		*/
  char            SIMFLAG  ;		/* simulation flag (U,S)	*/
  unsigned char   FLAG     ;		/* flag				*/
  }
papat_list;

typedef struct paini			/* initialized register		*/
  {
  struct paini   *NEXT     ;		/* next initialization		*/
  char           *NAME     ;		/* register's name		*/
  char            VALUE    ;		/* initialization's value	*/
  }
paini_list;

typedef struct paevt			/* event			*/
  {
  struct paevt   *NEXT     ;		/* next event			*/
  unsigned short  INDEX    ;		/* input-output's index		*/
  char            USRVAL   ;		/* value given by the user	*/
  char            SIMVAL   ;		/* calculated value		*/
  }
paevt_list;

typedef struct pacom			/* comment line			*/
  {
  struct pacom   *NEXT     ;		/* next comment			*/
  char           *TEXT     ;		/* text of the comment		*/
  unsigned short  POSITION ;		/* nbr of decl. or pat. before	*/
  }
pacom_list;

	/* ###------------------------------------------------------### */
	/*    low-level functions					*/
	/* ###------------------------------------------------------### */

extern struct paseq *pat_addpaseq ();	/* add a paseq structure	*/
extern struct papat *pat_addpapat ();	/* add a papat structure	*/
extern struct paevt *pat_addpaevt ();	/* add a paevt structure	*/
extern struct paini *pat_addpaini ();	/* add a paini structure	*/
extern struct pacom *pat_addpacom ();	/* add a pacom structure	*/
extern struct pagrp *pat_addpagrp ();	/* add a pagrp structure	*/
extern struct paiol *pat_addpaiol ();	/* add a paiol structure	*/
extern struct pains *pat_addpains ();	/* add a pains structure	*/

extern struct paiol *pat_crtpaiol ();	/* create a table of paiols	*/

extern void          pat_frepaiol ();	/* delete a list of paiols	*/
extern void          pat_frepaini ();	/* delete a list of painis	*/
extern void          pat_frepaevt ();	/* delete a list of paevts	*/
extern void          pat_frepapat ();	/* delete a list of papats	*/
extern void          pat_frepacom ();	/* delete a list of pacoms	*/

extern int           pat_error    ();	/* pat error function		*/
extern int           pat_warning  ();	/* pat warning function		*/
extern int           pat_message  ();	/* pat message function		*/

#endif
