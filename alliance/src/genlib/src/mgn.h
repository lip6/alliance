/*
 * This file is part of the Alliance CAD System
 * Copyright (C) Laboratoire LIP6 - Département ASIM
 * Universite Pierre et Marie Curie
 * 
 * Home page          : http://www-asim.lip6.fr/alliance/
 * E-mail             : mailto:alliance-users@asim.lip6.fr
 * 
 * This progam is  free software; you can redistribute it  and/or modify it
 * under the  terms of the GNU  General Public License as  published by the
 * Free Software Foundation;  either version 2 of the License,  or (at your
 * option) any later version.
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

/*******************************************************************************
* mgn.h                                                                        *
* header for the definition of genlib functions, and macros                    *
* author  : Frederic Petrot                                                    *
* version : 3.18                                                               *
* date    : 03/09/93                                                           *
* version : 3.26                                                               *
* date    : 10/06/97                                                           *
*******************************************************************************/
/* Types for the ptype lists as required in version 3.26
   As the types are all exclusives from another there is no need for
   styled values. */

/*******************************************************************************
* physical placement functions                                                 *
*******************************************************************************/
extern void genDEF_PHFIG();
extern void genLOAD_PHFIG();
extern void genSAVE_PHFIG();
extern void genDEF_AB();
extern void genDEF_PHINS();
extern ptype_list *genPHCON();
extern ptype_list *genCOPY_UP_CON();
extern ptype_list *genCOPY_UP_CON_FACE();
extern ptype_list *genCOPY_UP_ALL_CON();
extern ptype_list *genPHSEG();
extern ptype_list *genCOPY_UP_SEG();
extern ptype_list *genTHRU_H();
extern ptype_list *genTHRU_V();
extern ptype_list *genTHRU_CON_H();
extern ptype_list *genTHRU_CON_V();
extern ptype_list *genWIRE1();
extern ptype_list *genWIRE2();
extern ptype_list *genWIRE3();
extern ptype_list *genPHVIA();
extern ptype_list *genPHBIGVIA();
extern ptype_list *genPLACE();
extern ptype_list *genPLACE_RIGHT();
extern ptype_list *genPLACE_TOP();
extern ptype_list *genPLACE_LEFT();
extern ptype_list *genPLACE_BOTTOM();
extern ptype_list *genPLACE_REF();
extern ptype_list *genPLACE_ON();
extern ptype_list *genPHREF();
extern void        genDEL_PHREF();
extern ptype_list *genCOPY_UP_REF();
extern ptype_list *genCOPY_UP_ALL_REF();
extern ptype_list *genPLACE_VIA_REF();
extern ptype_list *genPLACE_CON_REF();
extern ptype_list *genPLACE_SEG_REF();
extern void genFLATTEN_PHFIG();
extern void genFLATTEN_ALL_PHINS();
extern long genGET_PHFIG_REF_X();
extern long genGET_PHFIG_REF_Y();
extern long genGET_REF_X();
extern long genGET_REF_Y();
extern long genGET_CON_X();
extern long genGET_CON_Y();
extern long genGET_INS_X();
extern long genGET_INS_Y();
extern long genHEIGHT();
extern long genWIDTH();
extern void genREVERSE_PHCON();
extern void genOUTLINE();
/*******************************************************************************
* netlist description functions                                                *
*******************************************************************************/
extern void genDEF_LOFIG();
extern void genLOAD_LOFIG();
extern void genSAVE_LOFIG();
extern void genLOINS(char *, ...);
extern void genLOINSE(char *, ...);
extern void genLOINSA();
extern void genLOTRS( char type, unsigned short width, unsigned short length, char *grid, char *source, char *drain );
extern void genLOCON( char *conname, char direction, char *signame );
extern void genLOSIGMERGE( char *sig1, char *sig2 );
extern void genFLATTEN_LOFIG( char *insname, char concat );
extern void genFLATTEN_ALL_LOINS( char concat, int catal );
extern void genDEF_PHSC( char *name );
extern void genSAVE_PHSC();
extern void genSC_PLACE( char  *insname, int   sym, long  x, long y );
extern void genSC_RIGHT( char  *insname, int   sym );
extern void genSC_LEFT( char  *insname, int   sym );
extern void genSC_TOP( char *insname, int sym );
extern void genSC_BOTTOM( char *insname, int sym );
extern void genSC_CHANNEL();
extern void genSC_CON_CHANNEL();
extern void genLOGEN(char *, int, ...);
extern void genSETLOGEN(char *, char *, ...);

/********************************* Analogical world ************************************/

extern void genLOCAP(char type,double capa,char *tcon,char *bcon,char *name)    ;
extern void genLORES(char type,double resi,char *rcon1,char *rcon2,char *name)  ;
extern void genLOSELF(char type,double self,char *scon1,char *scon2,char *name) ;

extern void genSET_LOCAP(char *figname,char *capname,double newcap)    ;
extern void genSET_LORES(char *figname,char *resname,double newres)    ;
extern void genSET_LOSELF(char *figname,char *selfname,double newself) ;

/***************************************************************************************/


/*******************************************************************************
* name forming functions, for both views                                       *
*******************************************************************************/
extern char *genBUS();
extern char *genELM();
extern char *genNAME(char *, ...);

/*******************************************************************************
* macro definitions for allowed stdlib functions                               *
*******************************************************************************/
#define PRINTF       printf
#define STRLEN(a)    (long)(a == NULL ? 0 : strlen(a))

/*******************************************************************************
* macro definitions in order to allows float and integer typed parameters      *
*******************************************************************************/
#ifndef  __GENLIB__
#define GENLIB_DEF_PHFIG(name)                      genDEF_PHFIG(name)
#define GENLIB_LOAD_PHFIG(name)                     genLOAD_PHFIG(name)
#define GENLIB_SAVE_PHFIG                           genSAVE_PHFIG
#define GENLIB_DEF_AB(dx1,dy1,dx2,dy2) \
            genDEF_AB((long)((dx1)*SCALE_X),(long)((dy1)*SCALE_X), \
                        (long)((dx2)*SCALE_X),(long)((dy2)*SCALE_X))
#define GENLIB_DEF_PHINS(name) \
            genDEF_PHINS(name)
#define GENLIB_PHCON(layer,width,name,orient,x,y) \
            genPHCON(layer,(long)((width)*SCALE_X),name,orient, \
                     (long)((x)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_COPY_UP_CON(index,locname,insname,newname) \
            genCOPY_UP_CON(index,locname,insname,newname)
#define GENLIB_COPY_UP_ALL_CON(orient,insname,cheminom) \
            genCOPY_UP_ALL_CON(orient,insname,cheminom)
#define GENLIB_COPY_UP_CON_FACE(orient,locname,insname,newname) \
            genCOPY_UP_CON_FACE(orient,locname,insname,newname)
#define GENLIB_PHSEG(layer,width,name,x1,y1,x2,y2) \
            genPHSEG(layer,(long)((width)*SCALE_X),name,(long)((x1)*SCALE_X), \
                     (long)((y1)*SCALE_X), (long)((x2)*SCALE_X), \
                     (long)((y2)*SCALE_X))
#define GENLIB_COPY_UP_SEG(locname,insname,newname) \
            genCOPY_UP_SEG(locname,insname,newname)
#define GENLIB_WIRE1(layer,width,ins1,con1,index1,ins2,con2,index2) \
            genWIRE1(layer,(long)((width)*SCALE_X),ins1,con1,index1,ins2, \
                     con2,index2)
#define GENLIB_WIRE2(layer,width,ins1,con1,index1,ins2,con2,index2,x,y) \
            genWIRE2(layer,(long)((width)*SCALE_X),ins1,con1,index1,ins2, \
                     con2,index2, (long)((x)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_WIRE3(layer,width,ins1,con1,index1,ins2,con2,index2,x1,y1,x2,y2)  \
            genWIRE3(layer,(long)((width)*SCALE_X),ins1,con1,index1,ins2, \
                     con2,index2, (long)((x1)*SCALE_X),(long)((y1)*SCALE_X), \
                     (long)((x2)*SCALE_X),(long)((y2)*SCALE_X))
#define GENLIB_PHVIA(type,x,y) \
            genPHVIA(type,(long)((x)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_PHBIGVIA(type,x,y,dx,dy,name) \
            genPHBIGVIA(type,(long)((x)*SCALE_X),(long)((y)*SCALE_X), \
                             (long)((dx)*SCALE_X),(long)((dy)*SCALE_X),name)
#define GENLIB_PLACE(figname,insname,sym,x,y) \
            genPLACE(figname,insname,sym,(long)((x)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_PLACE_RIGHT(figname,insname,sym)  genPLACE_RIGHT(figname,insname,sym)
#define GENLIB_PLACE_TOP(figname,insname,sym)    genPLACE_TOP(figname,insname,sym)
#define GENLIB_PLACE_LEFT(figname,insname,sym)   genPLACE_LEFT(figname,insname,sym)
#define GENLIB_PLACE_BOTTOM(figname,insname,sym) genPLACE_BOTTOM(figname,insname,sym)
#define GENLIB_PLACE_REF(figname,insname,sym,insref,refname) \
            genPLACE_REF(figname,insname,sym,insref,refname)
#define GENLIB_PLACE_ON(figname,insname1,con1,index1,sym,insname2,con2,index2) \
            genPLACE_ON(figname,insname1,con1,index1,sym,\
                     insname2,con2,index2)
#define GENLIB_THRU_H(layer,width,y) \
            genTHRU_H(layer,(long)((width)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_THRU_V(layer,width,x) \
            genTHRU_V(layer,(long)((width)*SCALE_X),(long)((x)*SCALE_X))
#define GENLIB_THRU_CON_H(layer,width,name,y) \
            genTHRU_CON_H(layer,(long)((width)*SCALE_X),name,(long)((y)*SCALE_X))
#define GENLIB_THRU_CON_V(layer,width,name,x) \
            genTHRU_CON_V(layer,(long)((width)*SCALE_X),name,(long)((x)*SCALE_X))
#define GENLIB_PHREF(type,name,x,y) \
            genPHREF(type,name,(long)((x)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_DEL_PHREF genDEL_PHREF
#define GENLIB_COPY_UP_REF(refname,insname,newname) \
            genCOPY_UP_REF(refname,insname,newname)
#define GENLIB_COPY_UP_ALL_REF(reftype,insname,cheminom) \
            genCOPY_UP_ALL_REF(reftype,insname,cheminom)
#define GENLIB_PLACE_VIA_REF(insname,refname,type) \
            genPLACE_VIA_REF(insname,refname,type)
#define GENLIB_PLACE_CON_REF(insname,refname,conname,layer,width,face) \
            genPLACE_CON_REF(insname,refname,conname,layer,\
                           (long)((width)*SCALE_X),face)
#define GENLIB_PLACE_SEG_REF(insname,refname,layer,width,face) \
            genPLACE_SEG_REF(insname,refname,layer,(long)((width)*SCALE_X),face)
#define GENLIB_GET_PHFIG_REF_X(refname) \
            ((genGET_PHFIG_REF_X(refname)/SCALE_X))
#define GENLIB_GET_PHFIG_REF_Y(refname) \
            ((genGET_PHFIG_REF_Y(refname)/SCALE_X))
#define GENLIB_GET_REF_X(insname,refname) \
            ((genGET_REF_X(insname,refname)/SCALE_X))
#define GENLIB_GET_REF_Y(insname,refname) \
            ((genGET_REF_Y(insname,refname)/SCALE_X))
#define GENLIB_GET_INS_X(insname)                ((genGET_INS_X(insname)/SCALE_X))
#define GENLIB_GET_INS_Y(insname)                ((genGET_INS_Y(insname)/SCALE_X))
#define GENLIB_GET_INS_XDX(insname)              ((genGET_INS_XDX(insname)/SCALE_X))
#define GENLIB_GET_INS_YDY(insname)              ((genGET_INS_YDY(insname)/SCALE_X))
#define GENLIB_GET_CON_X(insname,conname,index) \
            ((genGET_CON_X(insname,conname,index)/SCALE_X))
#define GENLIB_GET_CON_Y(insname,conname,index) \
            ((genGET_CON_Y(insname,conname,index)/SCALE_X))
#define GENLIB_GET_CON_LAYER(insname,conname,index) \
            (genGET_CON_LAYER(insname,conname,index))
#define GENLIB_HEIGHT(cellName)                  ((genHEIGHT(cellName)/SCALE_X))
#define GENLIB_WIDTH(cellName)                   ((genWIDTH(cellName)/SCALE_X))
#define GENLIB_FLATTEN_PHFIG(insname,concat)     genFLATTEN_PHFIG(insname,concat)
#define GENLIB_FLATTEN_ALL_PHINS(concat, catal)  genFLATTEN_ALL_PHINS(concat, catal)
#define GENLIB_REVERSE_PHCON(conname)            genREVERSE_PHCON(conname)
#define GENLIB_OUTLINE                           genOUTLINE

#define GENLIB_DEF_LOFIG(name)                   genDEF_LOFIG(name)
#define GENLIB_LOAD_LOFIG(name)                  genLOAD_LOFIG(name)
#define GENLIB_SAVE_LOFIG                        genSAVE_LOFIG
#define GENLIB_LOSIG(name)                       genLOSIG(name)
#define GENLIB_LOINS                             genLOINS
#define GENLIB_LOINSE                            genLOINSE
#define GENLIB_LOINSA                            genLOINSA
#define GENLIB_LOGEN                             genLOGEN
#define GENLIB_SETLOGEN                          genSETLOGEN
#define GENLIB_LOTRS(type,width,length,grid,source,drain) \
            genLOTRS(type,(long)((width)*SCALE_X), \
                     (long)((length)*SCALE_X),grid,source,drain)
#define GENLIB_LOCON(conname,direction,signame)  genLOCON(conname,direction,signame)
#define GENLIB_LOSIGMERGE(signame1, signame2)    genLOSIGMERGE(signame1, signame2)
#define GENLIB_FLATTEN_LOFIG(insname,concat)     genFLATTEN_LOFIG(insname,concat)
#define GENLIB_FLATTEN_ALL_LOINS(concat, catal)  genFLATTEN_ALL_LOINS(concat, catal)
#define GENLIB_UNFLATTEN_LOFIG                   genUNFLATTEN_LOFIG

#define GENLIB_USED(s, c)                        genUSED(s, c)

#define GENLIB_DEF_PHSC(name)                    genDEF_PHSC(name)
#define GENLIB_SAVE_PHSC                         genSAVE_PHSC
#define GENLIB_SC_PLACE(insname,sym,x,y) \
            genSC_PLACE(insname,sym,(long)((x)*SCALE_X),(long)((y)*SCALE_X))
#define GENLIB_SC_RIGHT(insname,sym)             genSC_RIGHT(insname,sym)
#define GENLIB_SC_LEFT(insname,sym)              genSC_LEFT(insname,sym)
#define GENLIB_SC_TOP(insname,sym)               genSC_TOP(insname,sym)
#define GENLIB_SC_BOTTOM(insname,sym)            genSC_BOTTOM(insname,sym)
#define GENLIB_SC_CHANNEL(direction,width,insname) \
            genSC_CHANNEL(direction,(long)((width)*SCALE_X),insname)
#define GENLIB_SC_CON_CHANNEL(conname,orient,layer,width,insname) \
            genSC_CON_CHANNEL(conname,orient,layer,\
                              (long)((width)*SCALE_X),insname)

#define GENLIB_BUS(signame,from,to)              genBUS(signame,from,to)
#define GENLIB_ELM(signame,number)               genELM(signame,number)
#define GENLIB_NAME                              genNAME


/********************************* Analogical world ************************************/

#define GENLIB_LOCAP(type,capa,tcon,bcon,name)      genLOCAP(type,capa,tcon,bcon,name)
#define GENLIB_LORES(type,resi,rcon1,rcon2,name)    genLORES(type,resi,rcon1,rcon2,name)
#define GENLIB_LOSELF(type,self,scon1,scon2,name)   genLOSELF(type,self,scon1,scon2,name)

#define GENLIB_SET_LOCAP(figname,capname,newcap)    genSET_LOCAP(figname,capname,newcap)
#define GENLIB_SET_LORES(figname,resname,newres)    genSET_LORES(figname,resname,newres)
#define GENLIB_SET_LOSELF(figname,selfname,newself) genSET_LOSELF(figname,selfname,newself)

/***************************************************************************************/

#endif

/*
 * New allowed names for layers and contacts
 */
#define METAL   ALU1
#define ALLOWM  TALU1
#define METAL2  ALU2
#define ALLOWM2 TALU2
#define METAL3  ALU3
#define ALLOWM3 TALU3
#define METAL4  ALU4
#define ALLOWM4 TALU4
#define METAL5  ALU5
#define ALLOWM5 TALU5
#define METAL6  ALU6
#define ALLOWM6 TALU6
#define METAL7  ALU7
#define ALLOWM7 TALU7
#define METAL8  ALU8
#define ALLOWM8 TALU8
#define METAL9  ALU9
#define ALLOWM9 TALU9

#define VIA12 CONT_VIA
#define VIA23 CONT_VIA2
#define VIA34 CONT_VIA3
#define VIA45 CONT_VIA4
#define VIA56 CONT_VIA5
#define VIA67 CONT_VIA6
#define VIA78 CONT_VIA7
#define VIA89 CONT_VIA8

/* Names usable that macro-expand into several contacts: they have no
 * internal counterparts.
 */
#define VIA13 (LAST_CONTACT + 1)
#define VIA14 (LAST_CONTACT + 2)
#define VIA15 (LAST_CONTACT + 3)
#define VIA16 (LAST_CONTACT + 4)
#define VIA17 (LAST_CONTACT + 5)
#define VIA18 (LAST_CONTACT + 6)
#define VIA19 (LAST_CONTACT + 7)
#define VIA24 (LAST_CONTACT + 8)
#define VIA25 (LAST_CONTACT + 9)
#define VIA26 (LAST_CONTACT + 10)
#define VIA27 (LAST_CONTACT + 11)
#define VIA28 (LAST_CONTACT + 12)
#define VIA29 (LAST_CONTACT + 13)
#define VIA35 (LAST_CONTACT + 14)
#define VIA36 (LAST_CONTACT + 15)
#define VIA37 (LAST_CONTACT + 16)
#define VIA38 (LAST_CONTACT + 17)
#define VIA39 (LAST_CONTACT + 18)
#define VIA46 (LAST_CONTACT + 19)
#define VIA47 (LAST_CONTACT + 20)
#define VIA48 (LAST_CONTACT + 21)
#define VIA49 (LAST_CONTACT + 22)
#define VIA57 (LAST_CONTACT + 23)
#define VIA58 (LAST_CONTACT + 24)
#define VIA59 (LAST_CONTACT + 25)
#define VIA68 (LAST_CONTACT + 26) 
#define VIA69 (LAST_CONTACT + 27) 
#define VIA79 (LAST_CONTACT + 28) 

#define CONT_VIA13 VIA13
#define CONT_VIA14 VIA14
#define CONT_VIA15 VIA15
#define CONT_VIA16 VIA16
#define CONT_VIA17 VIA17
#define CONT_VIA18 VIA18
#define CONT_VIA19 VIA19
#define CONT_VIA24 VIA24
#define CONT_VIA25 VIA25
#define CONT_VIA26 VIA26
#define CONT_VIA27 VIA27
#define CONT_VIA28 VIA28
#define CONT_VIA29 VIA29
#define CONT_VIA35 VIA35
#define CONT_VIA36 VIA36
#define CONT_VIA37 VIA37
#define CONT_VIA38 VIA38
#define CONT_VIA39 VIA39
#define CONT_VIA46 VIA46
#define CONT_VIA47 VIA47
#define CONT_VIA48 VIA48
#define CONT_VIA49 VIA49
#define CONT_VIA57 VIA57
#define CONT_VIA58 VIA58
#define CONT_VIA59 VIA59
#define CONT_VIA68 VIA68
#define CONT_VIA69 VIA69
#define CONT_VIA79 VIA79

/* Quick and dirty hack as required */
#define LOGEN 21

#define GENTYPE_EMPTY 0
#define GENTYPE_BIT   1
#define GENTYPE_VAL   2
#define GENTYPE_ARG   3
#define GENTYPE_TEXT  4
#define GENTYPE_LIST  5
#define GENTYPE_HEX   6
#define GENTYPE_MAX   6


typedef struct logen
{
   struct logen *NEXT;
   char         *NAME;
   char          TYPE;
   union {
      char          CHAR;
      long           VAL;
      char         *TEXT;
      struct logen *LIST;
   }            VALUE;
   char           TAG;
} logen_list;

extern logen_list *addlogen(logen_list *,char *);
extern logen_list *getlogen(logen_list *,char *);
extern logen_list *duplogen(logen_list *,logen_list *, char *);
extern logen_list *duplogenlist(logen_list *,logen_list *, char *);
extern logen_list *chklogen(logen_list *,logen_list *, char *, char *);
extern logen_list *dellogen(logen_list *, char *name);

/* End of hack */
