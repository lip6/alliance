//
// $Id: dpgen_RF2.c,v 1.2 2004/09/06 13:48:43 jpc Exp $
//
// auteur Alain Greiner
//
// Derniere modification 08/04/2001 
//
// Banc de registre triple acces : deux lectures, une ecriture
// Le programme genere une net-list et un placement optimise.
//
// La largeur est egale a (nmot * 35) + 150
// La hauteur est egale a (nbit + 8) * 50 
//
// Pour la partie "donnees", on utilise les cellules suivantes :
//   colonne d'entree 
// - rf2_inmux_mem          
// - rf2_inmux_buf        
//   colonne centrale
// - rf2_mid_mem          
// - rf2_mid_mem_r0          
// - rf2_mid_buf      
//   colonne de sortie
// - rf2_out_mem          
// - rf2_out_buf        
//
// Pour le decodeur, on utilise les cellules suivantes :
// - rf2_dec_bufad0
// - rf2_dec_bufad1_l
// - rf2_dec_bufad2_l
// - rf2_dec_bufad1_r
// - rf2_dec_bufad2_r
// - rf2_dec_nbuf
// - rf2_dec_nor3
// - rf2_dec_nao3
// - rf2_dec_nand2
// - rf2_dec_nand3
// - rf2_dec_nand4

# include   "util_Defs.h"
# include  "dpgen_Defs.h"
# include  "dgn.h"
# include  "debugon.h"

//----------------------------------------------------------------
//	DEBUT PROGRAMME  PRINCIPAL			   
//----------------------------------------------------------------
// aFunction : le type  :
//       DPGEN_RF2            (sans decodeur)
//       DPGEN_RF2R0          (sand decodeur, avec R0 colle a 0)
//       DPGEN_RF2D           (avec decodeur)
//       DPGEN_RF2DR0         (avec decodeur, avec R0 colle a 0)
//
// aAL : liste des parametres :
//          model_name  :  nom de modele.
//          flags       :  indexation des connecteurs.
//          nbit        :  nombre de bits (largeur du bus).
//          nmot        :  nombre de mots. 
// - le nombre de mots doit etre pair, 6 =< nmot =< 32
// - le nombre de bits doit etre pair, 2 =< nbit =< 64
//----------------------------------------------------------------

extern void  dpgen_RF2 (aFunction, aAL)
     long  aFunction;
  va_list  aAL;

{
long nmot;	/* nombre de mots */
long nbit;	/* nombre de bits */
long type; 	/* avec ou sans decodeurs  */
long flags;     /* indexation : bit 0 en bas si flags = 0 */

long HCELL = 50;

long bottom;
long i,j;
long half,max;
long sym1,sym2;
long adrange;

char x1[10];
char x2[10];
char x3[10];
char x4[10];

char *model_name;   // le nom du modele.

//////////////////////////////////////////////////////////
//
// Lecture et verification des parametres
//
//////////////////////////////////////////////////////////

model_name =      va_arg (aAL, char*);
flags      =      va_arg (aAL, long );
nbit       =      va_arg (aAL, long );
nmot       =      va_arg (aAL, long );

switch(aFunction) {
	case    DPGEN_RF2 :
		type = 0;
		break;
	case    DPGEN_RF2R0 :
		type = 1;
		break;
	case    DPGEN_RF2D :
		type = 2;
		break;
	case    DPGEN_RF2DR0 :
		type = 3;
		break;
	default : 
		printf("\n\n!!! DPGEN register file : illegal value for the type \n\n");
    		EXIT (1);
	}

if ((nmot%2 == 1) || (nmot > 32) || (nmot < 4)) {
printf("\n!!! DPGEN Register file : the word number must be even    !!!\n"); 	
printf("\n!!!                   larger than 4 and no larger than 32 !!!\n\n"); 	
EXIT (1);
}

if ((nmot == 4) && ((type == 2) || (type == 3))) {
printf("\n!!! DPGEN Register file with internal decoder  !!!\n"); 	
printf("\n!!! The word number must be larger than 4      !!!\n\n"); 	
EXIT (1);
}

if ((nbit%2 == 1) || (nbit > 64) || (nbit < 2)) {
printf("\n!!! DPGEN Register file : the bit number must be even    !!!\n"); 	
printf("\n!!!                          and no larger than 64       !!!\n\n"); 	
EXIT (1);
}

GENLIB_DEF_PHFIG(model_name);
GENLIB_DEF_LOFIG(model_name);

///////////////////////////////////////////////////////////////////
//
//           Definition de l'interface logique
//
///////////////////////////////////////////////////////////////////

adrange = 2;
if (nmot > 4) adrange = 3;
if (nmot > 8) adrange = 4;
if (nmot > 16) adrange = 5;

if(type < 2){   /* c'est un banc de registres sans decodeur  */

GENLIB_LOCON("ck",IN,"ckok");
GENLIB_LOCON("sel",IN,"sel");
GENLIB_LOCON(GENLIB_BUS("sela",nmot-1,0),IN,GENLIB_BUS("sela",nmot-1,0));
GENLIB_LOCON(GENLIB_BUS("selb",nmot-1,0),IN,GENLIB_BUS("selb",nmot-1,0));
GENLIB_LOCON(GENLIB_BUS("selw",nmot-1,0),IN,GENLIB_BUS("selw",nmot-1,0));
GENLIB_LOCON(GENLIB_BUS("datain0",nbit-1,0),IN,GENLIB_BUS("datain0",nbit-1,0));
GENLIB_LOCON(GENLIB_BUS("datain1",nbit-1,0),IN,GENLIB_BUS("datain1",nbit-1,0));
GENLIB_LOCON(GENLIB_BUS("dataouta",nbit-1,0),OUT,GENLIB_BUS("dataouta",nbit-1,0));
GENLIB_LOCON(GENLIB_BUS("dataoutb",nbit-1,0),OUT,GENLIB_BUS("dataoutb",nbit-1,0));
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");
}

else {   /* c'est un banc de registres avec decodeur  */

GENLIB_LOCON("ck",IN,"ckok");
GENLIB_LOCON("sel",IN,"sel");
GENLIB_LOCON("wen",IN,"wen");
GENLIB_LOCON("rena",IN,"rena");
GENLIB_LOCON("renb",IN,"renb");
GENLIB_LOCON(GENLIB_BUS("adra",adrange-1,0),IN,GENLIB_BUS("adra",adrange-1,0));
GENLIB_LOCON(GENLIB_BUS("adrb",adrange-1,0),IN,GENLIB_BUS("adrb",adrange-1,0));
GENLIB_LOCON(GENLIB_BUS("adw",adrange-1,0),IN,GENLIB_BUS("adw",adrange-1,0));
GENLIB_LOCON(GENLIB_BUS("datain0",nbit-1,0),IN,GENLIB_BUS("datain0",nbit-1,0));
GENLIB_LOCON(GENLIB_BUS("datain1",nbit-1,0),IN,GENLIB_BUS("datain1",nbit-1,0));
GENLIB_LOCON(GENLIB_BUS("dataouta",nbit-1,0),OUT,GENLIB_BUS("dataouta",nbit-1,0));
GENLIB_LOCON(GENLIB_BUS("dataoutb",nbit-1,0),OUT,GENLIB_BUS("dataoutb",nbit-1,0));
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");
}


///////////////////////////////////////////////////////////////////
//
// placement des lignes de bit en dessous des buffers
//
///////////////////////////////////////////////////////////////////

bottom = 0;
sym1 = NOSYM;
sym2 = SYM_X;
for(i=0;i<nbit;i++){
switch(i%2){	
	case 0:	//  lignes paires 
	sym1 = NOSYM;
	sym2 = SYM_X;
	break;
	case 1: //  lignes impaires
	sym1 = SYM_Y;
	sym2 = SYMXY;
	break;
	}
GENLIB_PLACE("rf2_inmux_mem",GENLIB_NAME("in%d",nbit-1-i),sym1,0,bottom+i*HCELL);
GENLIB_LOINS("rf2_inmux_mem",GENLIB_NAME("in%d",nbit-1-i),
	GENLIB_NAME("datain0[%d]",nbit-1-i),
	GENLIB_NAME("datain1[%d]",nbit-1-i),
	"sel0",
	"sel1",
	GENLIB_NAME("dinx[%d]",nbit-1-i),
	"vdd",
	"vss",NULL);
for (j=0;j<nmot;j+=2){
	if ((j == 0) && (type % 2 == 1)) {
	GENLIB_PLACE_RIGHT("rf2_mid_mem_r0",GENLIB_NAME("bit%d_%d",nbit-1-i,j),sym1);
	GENLIB_LOINS("rf2_mid_mem_r0",GENLIB_NAME("bit%d_%d",nbit-1-i,j),
	GENLIB_NAME("dinx[%d]",nbit-1-i),
	GENLIB_NAME("write%d",j),
	GENLIB_NAME("reada%d",j),
	GENLIB_NAME("readb%d",j),
	GENLIB_NAME("busa[%d]",nbit-1-i),
	GENLIB_NAME("busb[%d]",nbit-1-i),
	"vdd",
	"vss",NULL);
	}
	else {
	GENLIB_PLACE_RIGHT("rf2_mid_mem",GENLIB_NAME("bit%d_%d",nbit-1-i,j),sym1);
	GENLIB_LOINS("rf2_mid_mem",GENLIB_NAME("bit%d_%d",nbit-1-i,j),
	GENLIB_NAME("dinx[%d]",nbit-1-i),
	GENLIB_NAME("write%d",j),
	GENLIB_NAME("reada%d",j),
	GENLIB_NAME("readb%d",j),
	GENLIB_NAME("busa[%d]",nbit-1-i),
	GENLIB_NAME("busb[%d]",nbit-1-i),
	"vdd",
	"vss",NULL);
	}
	GENLIB_PLACE_RIGHT("rf2_mid_mem",GENLIB_NAME("bit%d_%d",nbit-1-i,j+1),sym2);
	GENLIB_LOINS("rf2_mid_mem",GENLIB_NAME("bit%d_%d",nbit-1-i,j+1),
	GENLIB_NAME("dinx[%d]",nbit-1-i),
	GENLIB_NAME("write%d",j+1),
	GENLIB_NAME("reada%d",j+1),
	GENLIB_NAME("readb%d",j+1),
	GENLIB_NAME("busa[%d]",nbit-1-i),
	GENLIB_NAME("busb[%d]",nbit-1-i),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_out_mem",GENLIB_NAME("out%d",nbit-1-i),sym1);
GENLIB_LOINS("rf2_out_mem",GENLIB_NAME("out%d",nbit-1-i),
	GENLIB_NAME("busa[%d]",nbit-1-i),
	GENLIB_NAME("busb[%d]",nbit-1-i),
	"xcks",
	GENLIB_NAME("dataouta[%d]",nbit-1-i),
	GENLIB_NAME("dataoutb[%d]",nbit-1-i),
	"vdd",
	"vss",NULL);
}

///////////////////////////////////////////////////////////////////
//
// placement de la ligne contenant les buffers
//
///////////////////////////////////////////////////////////////////

bottom = nbit * HCELL;
GENLIB_PLACE("rf2_inmux_buf","inbuf",NOSYM,0,bottom);
GENLIB_LOINS("rf2_inmux_buf","inbuf",
	"sel",
	"sel0",
	"sel1",
	"vdd",
	"vss",NULL);
for (j=0;j<nmot;j+=2){	
	GENLIB_PLACE_RIGHT("rf2_mid_buf",GENLIB_NAME("buf%d",j),NOSYM);
	GENLIB_LOINS("rf2_mid_buf",GENLIB_NAME("buf%d",j),
	GENLIB_ELM("selra",j),
	GENLIB_ELM("selrb",j),
	GENLIB_ELM("selw",j),
	"nck",
	GENLIB_NAME("reada%d",j),
	GENLIB_NAME("readb%d",j),
	GENLIB_NAME("write%d",j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("rf2_mid_buf",GENLIB_NAME("buf%d",j+1),SYM_X);
	GENLIB_LOINS("rf2_mid_buf",GENLIB_NAME("buf%d",j+1),
	GENLIB_ELM("selra",j+1),
	GENLIB_ELM("selrb",j+1),
	GENLIB_ELM("selw",j+1),
	"nck",
	GENLIB_NAME("reada%d",j+1),
	GENLIB_NAME("readb%d",j+1),
	GENLIB_NAME("write%d",j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_out_buf","outbuf",NOSYM);
	GENLIB_LOINS("rf2_out_buf","outbuf",
	"ckok",
	"nck",
	"xcks",
	"vdd",
	"vss",NULL);

///////////////////////////////////////////////////////////////////
//
// placement des cellules du decodeur ecriture 
//
///////////////////////////////////////////////////////////////////

if((type == 2) || (type == 3)) {

half = nmot >> 1;

// Premiere ligne decodeur ecriture

bottom = (nbit+2) * HCELL;
GENLIB_PLACE("rf2_dec_bufad0","adw0",NOSYM,0,bottom);
GENLIB_LOINS("rf2_dec_bufad0","adw0",
	"adw[0]",
	"nadw0x",
	"adw0x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
	GENLIB_PLACE_RIGHT("rf2_dec_nor3",GENLIB_NAME("selw%d",2*j),SYM_X);
	GENLIB_LOINS("rf2_dec_nor3",GENLIB_NAME("selw%d",2*j),
	GENLIB_NAME("nandw%d",j),
	"adw0x",
	"nwen",
	GENLIB_ELM("selw",2*j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("rf2_dec_nor3",GENLIB_NAME("selw%d",2*j+1),NOSYM);
	GENLIB_LOINS("rf2_dec_nor3",GENLIB_NAME("selw%d",2*j+1),
	GENLIB_NAME("nandw%d",j),
	"nadw0x",
	"nwen",
	GENLIB_ELM("selw",2*j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_nbuf","wen",NOSYM);
GENLIB_LOINS("rf2_dec_nbuf","wen",
	"wen",
	"nwen",
	"vdd",
	"vss",NULL);

// Deuxieme ligne decodeur ecriture

bottom = (nbit+3) * HCELL;

if (adrange == 5) {    /* adresse sur 5 bits  */

GENLIB_PLACE("rf2_dec_bufad2_l","adw12",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad2_l","adw12",
	"adw[1]",
	"adw[2]",
	"nadw1x",
	"adw1x",
	"nadw2x",
	"adw2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadw1x");
	else                  strcpy(x1,"adw1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadw2x");
	else                  strcpy(x2,"adw2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadw3x");
	else                  strcpy(x3,"adw3x");
        if ((j >> 3) % 2 == 0) strcpy(x4,"nadw4x");
	else                  strcpy(x4,"adw4x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand4",GENLIB_NAME("nand4w%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand4",GENLIB_NAME("nand4w%d",j),
	x1,
	x2,
	x3,
	x4,
	GENLIB_NAME("nandw%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad2_l","adw34",SYMXY);
GENLIB_LOINS("rf2_dec_bufad2_l","adw34",
	"adw[3]",
	"adw[4]",
	"nadw3x",
	"adw3x",
	"nadw4x",
	"adw4x",
	"vdd",
	"vss",NULL);
}

if (adrange == 4) {    /* adresse sur 4 bits  */

GENLIB_PLACE("rf2_dec_bufad2_l","adw12",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad2_l","adw12",
	"adw[1]",
	"adw[2]",
	"nadw1x",
	"adw1x",
	"nadw2x",
	"adw2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadw1x");
	else                  strcpy(x1,"adw1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadw2x");
	else                  strcpy(x2,"adw2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadw3x");
	else                  strcpy(x3,"adw3x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand3",GENLIB_NAME("nand3w%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand3",GENLIB_NAME("nand3w%d",j),
	x1,
	x2,
	x3,
	GENLIB_NAME("nandw%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad1_r","adw3",SYMXY);
GENLIB_LOINS("rf2_dec_bufad1_r","adw3",
	"adw[3]",
	"nadw3x",
	"adw3x",
	"vdd",
	"vss",NULL);
}

if (adrange == 3) {    /* adresse sur 3 bits  */

GENLIB_PLACE("rf2_dec_bufad1_l","adw1",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad1_l","adw1",
	"adw[1]",
	"nadw1x",
	"adw1x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadw1x");
	else                  strcpy(x1,"adw1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadw2x");
	else                  strcpy(x2,"adw2x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand2",GENLIB_NAME("nand2w%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand2",GENLIB_NAME("nand2w%d",j),
	x1,
	x2,
	GENLIB_NAME("nandw%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad1_r","adw2",SYMXY);
GENLIB_LOINS("rf2_dec_bufad1_r","adw2",
	"adw[2]",
	"nadw2x",
	"adw2x",
	"vdd",
	"vss",NULL);
}
}

///////////////////////////////////////////////////////////////////
//
// placement des cellules du decodeur lecture A
//
///////////////////////////////////////////////////////////////////

if((type == 2) || (type == 3)) {

half = nmot >> 1;

// Premiere ligne decodeur lecture A

bottom = (nbit+4) * HCELL;
GENLIB_PLACE("rf2_dec_bufad0","adra0",NOSYM,0,bottom);
GENLIB_LOINS("rf2_dec_bufad0","adra0",
	"adra[0]",
	"nadra0x",
	"adra0x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j == 0) {
	GENLIB_PLACE_RIGHT("rf2_dec_nao3",GENLIB_NAME("selra%d",2*j),NOSYM);
	GENLIB_LOINS("rf2_dec_nao3",GENLIB_NAME("selra%d",2*j),
	GENLIB_NAME("nandra%d",j),
	"adra0x",
	"rena",
	GENLIB_ELM("selra",2*j),
	"vdd",
	"vss",NULL);
        } else {
	GENLIB_PLACE_RIGHT("rf2_dec_nor3",GENLIB_NAME("selra%d",2*j),NOSYM);
	GENLIB_LOINS("rf2_dec_nor3",GENLIB_NAME("selra%d",2*j),
	GENLIB_NAME("nandra%d",j),
	"adra0x",
	"nrena",
	GENLIB_ELM("selra",2*j),
	"vdd",
	"vss",NULL);
	}
	GENLIB_PLACE_RIGHT("rf2_dec_nor3",GENLIB_NAME("selra%d",2*j+1),SYM_X);
	GENLIB_LOINS("rf2_dec_nor3",GENLIB_NAME("selra%d",2*j+1),
	GENLIB_NAME("nandra%d",j),
	"nadra0x",
	"nrena",
	GENLIB_ELM("selra",2*j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_nbuf","rena",NOSYM);
GENLIB_LOINS("rf2_dec_nbuf","rena",
	"rena",
	"nrena",
	"vdd",
	"vss",NULL);

// Deuxieme ligne decodeur lecture A

bottom = (nbit+5) * HCELL;

if (adrange == 5) {    /* adresse sur 5 bits  */

GENLIB_PLACE("rf2_dec_bufad2_l","adra12",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad2_l","adra12",
	"adra[1]",
	"adra[2]",
	"nadra1x",
	"adra1x",
	"nadra2x",
	"adra2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadra1x");
	else                  strcpy(x1,"adra1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadra2x");
	else                  strcpy(x2,"adra2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadra3x");
	else                  strcpy(x3,"adra3x");
        if ((j >> 3) % 2 == 0) strcpy(x4,"nadra4x");
	else                  strcpy(x4,"adra4x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand4",GENLIB_NAME("nand4ra%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand4",GENLIB_NAME("nand4ra%d",j),
	x1,
	x2,
	x3,
	x4,
	GENLIB_NAME("nandra%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad2_r","adra34",SYMXY);
GENLIB_LOINS("rf2_dec_bufad2_r","adra34",
	"adra[3]",
	"adra[4]",
	"nadra3x",
	"adra3x",
	"nadra4x",
	"adra4x",
	"vdd",
	"vss",NULL);
}

if (adrange == 4) {    /* adresse sur 4 bits  */

GENLIB_PLACE("rf2_dec_bufad2_l","adra12",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad2_l","adra12",
	"adra[1]",
	"adra[2]",
	"nadra1x",
	"adra1x",
	"nadra2x",
	"adra2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadra1x");
	else                  strcpy(x1,"adra1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadra2x");
	else                  strcpy(x2,"adra2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadra3x");
	else                  strcpy(x3,"adra3x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand3",GENLIB_NAME("nand3ra%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand3",GENLIB_NAME("nand3ra%d",j),
	x1,
	x2,
	x3,
	GENLIB_NAME("nandra%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad1_r","adra3",SYMXY);
GENLIB_LOINS("rf2_dec_bufad1_r","adra3",
	"adra[3]",
	"nadra3x",
	"adra3x",
	"vdd",
	"vss",NULL);
}

if (adrange == 3) {    /* adresse sur 3 bits  */

GENLIB_PLACE("rf2_dec_bufad1_l","adra1",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad1_l","adra1",
	"adra[1]",
	"nadra1x",
	"adra1x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadra1x");
	else                  strcpy(x1,"adra1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadra2x");
	else                  strcpy(x2,"adra2x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand2",GENLIB_NAME("nand2ra%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand2",GENLIB_NAME("nand2ra%d",j),
	x1,
	x2,
	GENLIB_NAME("nandra%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad1_r","adra2",SYMXY);
GENLIB_LOINS("rf2_dec_bufad1_r","adra2",
	"adra[2]",
	"nadra2x",
	"adra2x",
	"vdd",
	"vss",NULL);
}
}

///////////////////////////////////////////////////////////////////
//
// placement des cellules du decodeur lecture B
//
///////////////////////////////////////////////////////////////////

if((type == 2) || (type == 3)) {

half = nmot >> 1;

// Premiere ligne decodeur lecture A

bottom = (nbit+6) * HCELL;
GENLIB_PLACE("rf2_dec_bufad0","adrb0",NOSYM,0,bottom);
GENLIB_LOINS("rf2_dec_bufad0","adrb0",
	"adrb[0]",
	"nadrb0x",
	"adrb0x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j == 0) {
	GENLIB_PLACE_RIGHT("rf2_dec_nao3",GENLIB_NAME("selrb%d",2*j),NOSYM);
	GENLIB_LOINS("rf2_dec_nao3",GENLIB_NAME("selrb%d",2*j),
	GENLIB_NAME("nandrb%d",j),
	"adrb0x",
	"renb",
	GENLIB_ELM("selrb",2*j),
	"vdd",
	"vss",NULL);
        } else {
	GENLIB_PLACE_RIGHT("rf2_dec_nor3",GENLIB_NAME("selrb%d",2*j),NOSYM);
	GENLIB_LOINS("rf2_dec_nor3",GENLIB_NAME("selrb%d",2*j),
	GENLIB_NAME("nandrb%d",j),
	"adrb0x",
	"nrenb",
	GENLIB_ELM("selrb",2*j),
	"vdd",
	"vss",NULL);
	}
	GENLIB_PLACE_RIGHT("rf2_dec_nor3",GENLIB_NAME("selrb%d",2*j+1),SYM_X);
	GENLIB_LOINS("rf2_dec_nor3",GENLIB_NAME("selrb%d",2*j+1),
	GENLIB_NAME("nandrb%d",j),
	"nadrb0x",
	"nrenb",
	GENLIB_ELM("selrb",2*j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_nbuf","renb",NOSYM);
GENLIB_LOINS("rf2_dec_nbuf","renb",
	"renb",
	"nrenb",
	"vdd",
	"vss",NULL);

// Deuxieme ligne decodeur lecture A

bottom = (nbit+7) * HCELL;

if (adrange == 5) {    /* adresse sur 5 bits  */

GENLIB_PLACE("rf2_dec_bufad2_l","adrb12",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad2_l","adrb12",
	"adrb[1]",
	"adrb[2]",
	"nadrb1x",
	"adrb1x",
	"nadrb2x",
	"adrb2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadrb1x");
	else                  strcpy(x1,"adrb1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadrb2x");
	else                  strcpy(x2,"adrb2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadrb3x");
	else                  strcpy(x3,"adrb3x");
        if ((j >> 3) % 2 == 0) strcpy(x4,"nadrb4x");
	else                  strcpy(x4,"adrb4x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand4",GENLIB_NAME("nand4rb%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand4",GENLIB_NAME("nand4rb%d",j),
	x1,
	x2,
	x3,
	x4,
	GENLIB_NAME("nandrb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad2_r","adrb34",SYMXY);
GENLIB_LOINS("rf2_dec_bufad2_r","adrb34",
	"adrb[3]",
	"adrb[4]",
	"nadrb3x",
	"adrb3x",
	"nadrb4x",
	"adrb4x",
	"vdd",
	"vss",NULL);
}

if (adrange == 4) {    /* adresse sur 4 bits  */

GENLIB_PLACE("rf2_dec_bufad2_l","adrb12",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad2_l","adrb12",
	"adrb[1]",
	"adrb[2]",
	"nadrb1x",
	"adrb1x",
	"nadrb2x",
	"adrb2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadrb1x");
	else                  strcpy(x1,"adrb1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadrb2x");
	else                  strcpy(x2,"adrb2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadrb3x");
	else                  strcpy(x3,"adrb3x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand3",GENLIB_NAME("nand3rb%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand3",GENLIB_NAME("nand3rb%d",j),
	x1,
	x2,
	x3,
	GENLIB_NAME("nandrb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad1_r","adrb3",SYMXY);
GENLIB_LOINS("rf2_dec_bufad1_r","adrb3",
	"adrb[3]",
	"nadrb3x",
	"adrb3x",
	"vdd",
	"vss",NULL);
}

if (adrange == 3) {    /* adresse sur 3 bits  */

GENLIB_PLACE("rf2_dec_bufad1_l","adrb1",SYM_Y,0,bottom);
GENLIB_LOINS("rf2_dec_bufad1_l","adrb1",
	"adrb[1]",
	"nadrb1x",
	"adrb1x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadrb1x");
	else                  strcpy(x1,"adrb1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadrb2x");
	else                  strcpy(x2,"adrb2x");
	GENLIB_PLACE_RIGHT("rf2_dec_nand2",GENLIB_NAME("nand2rb%d",j),SYM_Y);
	GENLIB_LOINS("rf2_dec_nand2",GENLIB_NAME("nand2rb%d",j),
	x1,
	x2,
	GENLIB_NAME("nandrb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf2_dec_bufad1_r","adrb2",SYMXY);
GENLIB_LOINS("rf2_dec_bufad1_r","adrb2",
	"adrb[2]",
	"nadrb2x",
	"adrb2x",
	"vdd",
	"vss",NULL);
}
}

///////////////////////////////////////////////////////////////////
//
// Placement des rappels d'alimentation
//
///////////////////////////////////////////////////////////////////

if((type == 0) || (type == 1)) {
	max = nbit/2 + 2;
	GENLIB_PHSEG(CALU3,12,"vss", 10, 0, 10, (nbit + 2) * HCELL);
	GENLIB_PHSEG(CALU3,12,"vdd", (nmot * 35) + 140, 0,
			(nmot * 35) + 140, (nbit + 2) * HCELL);
	}
if((type == 2) || (type == 3)) {
	max = nbit/2 + 5;
	GENLIB_PHSEG(CALU3,12,"vss", 10, 0, 10, (nbit + 8) * HCELL);
	GENLIB_PHSEG(CALU3,12,"vdd", (nmot * 35) + 140, 0,
			(nmot * 35) + 140, (nbit + 8) * HCELL);
	}
for(i = 0 ; i < max ; i++) {
GENLIB_PHBIGVIA(VIA12, 10, i*100, 12, 2, "vss");
GENLIB_PHBIGVIA(VIA23, 10, i*100, 12, 2, "vss");
GENLIB_PHSEG(TALU2, 2, "obs", 5, i*100, 15, i*100);
}
for(i = 0 ; i < max-1 ; i++) {
GENLIB_PHBIGVIA(VIA12, nmot*35 + 140, 50 + i*100, 12, 2, "vdd");
GENLIB_PHBIGVIA(VIA23, nmot*35 + 140, 50 + i*100, 12, 2, "vdd");
GENLIB_PHSEG(TALU2, 2, "obs", nmot*35 + 135, 50 + i*100, nmot*35 + 145, 50 + i*100);
}

GENLIB_DEF_AB(0,0,0,0);

GENLIB_SAVE_PHFIG(model_name);
GENLIB_SAVE_LOFIG(model_name);
}
