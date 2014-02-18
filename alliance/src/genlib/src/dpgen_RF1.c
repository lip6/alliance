//
// $Id: dpgen_RF1.c,v 1.1 2002/04/29 13:40:54 jpc Exp $
//
// auteur Alain Greiner
//
// Derniere modification 13/03/2001 pour ajout rappels alim
//
// Banc de registre double acces : une lecture, une ecriture
// Le programme genere une net-list et un placement optimise.
// On peut generer trois types d'operateurs:
//	- banc de registres sans decodeur
//	- banc de registres avec decodeur
//	- banc de registre de type fifo
//
// La largeur est egale a (nmot * 25) + 100
// La hauteur est egale a (nbit + 2) * 50 sans decodeur
//                        (nbit + 6) * 50 avec decodeur
//                        (nbit + 8) * 50 pour la fifo 
//
// Pour la partie "donnees", on utilise les cellules suivantes :
//   colonne d'entree 
// - rf_inmux_mem          
// - rf_inmux_buf_2        
// - rf_inmux_buf_4        
//   colonne centrale
// - rf_mid_mem          
// - rf_mid_mem_r0          
// - rf_mid_buf_2        
// - rf_mid_buf_4        
//   colonne de sortie
// - rf_out_mem          
// - rf_out_buf_2        
// - rf_out_buf_4        
//
// Pour le controleur fifo, on utilise les cellules suivantes :
// - rf_fifo_buf
// - rf_fifo_clock
// - rf_fifo_empty
// - rf_fifo_full
// - rf_fifo_inc
// - rf_fifo_nop
// - rf_fifo_ok
// - rf_fifo_orand4
// - rf_fifo_orand5
// - rf_fifo_ptreset
// - rf_fifo_ptset
//
// Pour le decodeur, on utilise les cellules suivantes :
// - rf_dec_bufad0
// - rf_dec_bufad1
// - rf_dec_bufad2
// - rf_dec_nbuf
// - rf_dec_nor3
// - rf_dec_nao3
// - rf_dec_nand2
// - rf_dec_nand3
// - rf_dec_nand4


# include   "util_Defs.h"
# include  "dpgen_Defs.h"
# include  "dgn.h"
# include  "debugon.h"


//----------------------------------------------------------------
//	DEBUT PROGRAMME  PRINCIPAL			   
//----------------------------------------------------------------
// aFunction : le type  :
//       DPGEN_RF1            (sans decodeur)
//       DPGEN_RF1R0          (sand decodeur, avec R0 colle a 0)
//       DPGEN_RF1D           (avec decodeur)
//       DPGEN_RF1DR0         (avec decodeur, avec R0 colle a 0)
//       DPGEN_FIFO           (une fifo)
//
// aAL : liste des parametres :
//          model_name  :  nom de modele.
//          flags       :  indexation des connecteurs.
//          nbitd       :  nombre de bits (largeur du bus).
//          nmot        :  nombre de mots. 
// - le nombre de mots doit etre pair et =< 32
// - le nombre de bits doit etre pair, non nul et =< 64
//----------------------------------------------------------------

extern void  dpgen_RF1 (aFunction, aAL)
     long  aFunction;
  va_list  aAL;
{
long nmot;	/* nombre de mots */
long nbitd;	/* nombre de bits en-dessous des buffers */
long type = 0; 	/* fifo, avec ou sans decodeurs  */
/*long flags; * UNUSED indexation : bit 0 en bas si flags = 0 */

long nbitu=0;   /* nombre de bits au-dessus des buffers : non utilise */
long hbuf=2;    /* hauteur des buffers (2 ou 4 slices) : non utilise */

long HCELL = 50;

long bottom;
long ntot;
long i,j;
long half;
long max = 0;
long sym1,sym2;
long adrange;

char x1[10];
char x2[10];
char x3[10];
char x4[10];

char *model_name;   // le nom du modele.
char *cell_in_buf;  //  cellule buffer colonne entree
char *cell_in_mem;  //  cellule interne colonne entree
char *cell_mid_buf; //  cellule buffer colonne interne
char *cell_mid_mem; //  cellule interne colonne interne
char *cell_out_buf; //  cellule buffer colonne sortie
char *cell_out_mem; //  cellule interne colonne sortie

cell_in_buf=(char*)malloc(sizeof(char)*512);
cell_in_mem=(char*)malloc(sizeof(char)*512);
cell_out_buf=(char*)malloc(sizeof(char)*512);
cell_out_mem=(char*)malloc(sizeof(char)*512);
cell_mid_buf=(char*)malloc(sizeof(char)*512);
cell_mid_mem=(char*)malloc(sizeof(char)*512);

//////////////////////////////////////////////////////////
//
// Lecture et verification des parametres
//
//////////////////////////////////////////////////////////

model_name =      va_arg (aAL, char*);
                  va_arg (aAL, long );
nbitd      =      va_arg (aAL, long );
nmot       =      va_arg (aAL, long );

switch(aFunction) {
	case    DPGEN_RF1 :
		type = 0;
		break;
	case    DPGEN_RF1R0 :
		type = 1;
		break;
	case    DPGEN_RF1D :
		type = 2;
		break;
	case    DPGEN_RF1DR0 :
		type = 3;
		break;
	case    DPGEN_FIFO :
		type = 4;
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

if ((nbitd%2 == 1) || (nbitd > 64) || (nbitd < 2)) {
printf("\n!!! DPGEN Register file : the bit number must be even    !!!\n"); 	
printf("\n!!!                          and no larger than 64       !!!\n\n"); 	
EXIT (1);
}

// choix des cellules buffer

switch(hbuf){
	case 2 :			/* buffers sur 2 slices */
	cell_in_buf=strdup("rf_inmux_buf_2");
	cell_mid_buf=strdup("rf_mid_buf_2");
	cell_out_buf=strdup("rf_out_buf_2");
	break;
	case 4 :			/* buffers sur 4 slices */
	cell_in_buf=strdup("rf_inmux_buf_4");
	cell_mid_buf=strdup("rf_mid_buf_4");
	cell_out_buf=strdup("rf_out_buf_4");
	break;
	default : 
	printf("\n!!! DPGEN Register file : illegal buffer     !!!\n"); 	
    	EXIT (1);
	}

ntot = nbitu + nbitd;

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
GENLIB_LOCON(GENLIB_BUS("selr",nmot-1,0),IN,GENLIB_BUS("selr",nmot-1,0));
GENLIB_LOCON(GENLIB_BUS("selw",nmot-1,0),IN,GENLIB_BUS("selw",nmot-1,0));
GENLIB_LOCON(GENLIB_BUS("datain0",ntot-1,0),IN,GENLIB_BUS("datain0",ntot-1,0));
GENLIB_LOCON(GENLIB_BUS("datain1",ntot-1,0),IN,GENLIB_BUS("datain1",ntot-1,0));
GENLIB_LOCON(GENLIB_BUS("dataout",ntot-1,0),OUT,GENLIB_BUS("dataout",ntot-1,0));
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");
}

else if(type < 4){   /* c'est un banc de registres avec decodeur  */

GENLIB_LOCON("ck",IN,"ckok");
GENLIB_LOCON("sel",IN,"sel");
GENLIB_LOCON("wen",IN,"wen");
GENLIB_LOCON("ren",IN,"ren");
GENLIB_LOCON(GENLIB_BUS("adr",adrange-1,0),IN,GENLIB_BUS("adr",adrange-1,0));
GENLIB_LOCON(GENLIB_BUS("adw",adrange-1,0),IN,GENLIB_BUS("adw",adrange-1,0));
GENLIB_LOCON(GENLIB_BUS("datain0",ntot-1,0),IN,GENLIB_BUS("datain0",ntot-1,0));
GENLIB_LOCON(GENLIB_BUS("datain1",ntot-1,0),IN,GENLIB_BUS("datain1",ntot-1,0));
GENLIB_LOCON(GENLIB_BUS("dataout",ntot-1,0),OUT,GENLIB_BUS("dataout",ntot-1,0));
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");
}

else {		/* c'est une fifo  */

GENLIB_LOCON("ck",IN,"ck");
GENLIB_LOCON("reset",IN,"reset");
GENLIB_LOCON("r",IN,"r");
GENLIB_LOCON("w",IN,"w");
GENLIB_LOCON("rok",INOUT,"rok");
GENLIB_LOCON("wok",INOUT,"wok");
GENLIB_LOCON("sel",IN,"sel");
GENLIB_LOCON(GENLIB_BUS("datain0",ntot-1,0),IN,GENLIB_BUS("datain0",ntot-1,0));
GENLIB_LOCON(GENLIB_BUS("datain1",ntot-1,0),IN,GENLIB_BUS("datain1",ntot-1,0));
GENLIB_LOCON(GENLIB_BUS("dataout",ntot-1,0),OUT,GENLIB_BUS("dataout",ntot-1,0));
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");
}


///////////////////////////////////////////////////////////////////
//
// placement des lignes de bit en dessous des buffers
//
///////////////////////////////////////////////////////////////////
strcpy(cell_in_mem, "rf_inmux_mem");
strcpy(cell_out_mem, "rf_out_mem");
bottom = 0;
sym1 = NOSYM;
sym2 = SYM_X;
for(i=0;i<nbitd;i++){
	switch(i%2){	
		case 1:	//  lignes paires 
			sym1 = NOSYM;
			sym2 = SYM_X;
			break;
		case 0: //  lignes impaires
			sym1 = SYM_Y;
			sym2 = SYMXY;
		break;
	}
	GENLIB_PLACE(cell_in_mem,GENLIB_NAME("in%d",ntot-1-i),sym1,0,bottom+(ntot-1-i)*HCELL);
	GENLIB_LOINS(cell_in_mem,GENLIB_NAME("in%d",ntot-1-i),
		GENLIB_NAME("datain0[%d]",ntot-1-i),
		GENLIB_NAME("datain1[%d]",ntot-1-i),
		"sel0",
		"sel1",
		GENLIB_NAME("dinx[%d]",ntot-1-i),
		"vdd", "vss",NULL);
	for (j=0;j<nmot;j+=2){
		if ((j == 0) && (type % 2 == 1))  cell_mid_mem = strdup("rf_mid_mem_r0");
		else                              cell_mid_mem = strdup("rf_mid_mem");
		GENLIB_PLACE_RIGHT(cell_mid_mem,GENLIB_NAME("bit%d_%d",ntot-1-i,j),sym1);
		GENLIB_LOINS(cell_mid_mem,GENLIB_NAME("bit%d_%d",ntot-1-i,j),
			GENLIB_NAME("dinx[%d]",ntot-1-i),
			GENLIB_NAME("write%d",j),
			GENLIB_NAME("read%d",j),
			GENLIB_NAME("rbus[%d]",ntot-1-i),
			"vdd", "vss",NULL);
		cell_mid_mem = strdup("rf_mid_mem");
		GENLIB_PLACE_RIGHT(cell_mid_mem,GENLIB_NAME("bit%d_%d",ntot-1-i,j+1),sym2);
		GENLIB_LOINS(cell_mid_mem,GENLIB_NAME("bit%d_%d",ntot-1-i,j+1),
			GENLIB_NAME("dinx[%d]",ntot-1-i),
			GENLIB_NAME("write%d",j+1),
			GENLIB_NAME("read%d",j+1),
			GENLIB_NAME("rbus[%d]",ntot-1-i),
			"vdd", "vss",NULL);
	}
	GENLIB_PLACE_RIGHT(cell_out_mem,GENLIB_NAME("out%d",ntot-1-i),sym1);
		GENLIB_LOINS(cell_out_mem,GENLIB_NAME("out%d",ntot-1-i),
		GENLIB_NAME("rbus[%d]",ntot-1-i),
		"xck",
		GENLIB_NAME("dataout[%d]",ntot-1-i),
		"vdd", "vss",NULL);
}

///////////////////////////////////////////////////////////////////
//
// placement de la ligne contenant les buffers
//
///////////////////////////////////////////////////////////////////
bottom = nbitd * HCELL;
GENLIB_PLACE(cell_in_buf,"inbuf",NOSYM,0,bottom);
GENLIB_LOINS(cell_in_buf,"inbuf",
	"ckok",
	"sel",
	"nck",
	"sel0",
	"sel1",
	"vdd",
	"vss",NULL);
for (j=0;j<nmot;j+=2){	
	GENLIB_PLACE_RIGHT(cell_mid_buf,GENLIB_NAME("buf%d",j),NOSYM);
	GENLIB_LOINS(cell_mid_buf,GENLIB_NAME("buf%d",j),
	GENLIB_ELM("selr",j),
	GENLIB_ELM("selw",j),
	"nck",
	GENLIB_NAME("read%d",j),
	GENLIB_NAME("write%d",j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT(cell_mid_buf,GENLIB_NAME("buf%d",j+1),SYM_X);
	GENLIB_LOINS(cell_mid_buf,GENLIB_NAME("buf%d",j+1),
	GENLIB_ELM("selr",j+1),
	GENLIB_ELM("selw",j+1),
	"nck",
	GENLIB_NAME("read%d",j+1),
	GENLIB_NAME("write%d",j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT(cell_out_buf,"outbuf",NOSYM);
	GENLIB_LOINS(cell_out_buf,"outbuf",
	"nck",
	"xck",
	"vdd",
	"vss",NULL);

///////////////////////////////////////////////////////////////////
//
// placement des lignes de bit au dessus des buffers
//
///////////////////////////////////////////////////////////////////
strcpy(cell_in_mem, "rf_inmux_mem");
 strcpy(cell_out_mem, "rf_out_mem");
bottom = (nbitd+hbuf) * HCELL;
for(i=0;i<nbitu;i++){
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
GENLIB_PLACE(cell_in_mem,GENLIB_NAME("in%d",nbitu-1-i),sym1,0,bottom+i*HCELL);
GENLIB_LOINS(cell_in_mem,GENLIB_NAME("in%d",nbitu-1-i),
	GENLIB_NAME("datain0[%d]",nbitu-1-i),
	GENLIB_NAME("datain1[%d]",nbitu-1-i),
	"sel0",
	"sel1",
	GENLIB_NAME("dinx[%d]",nbitu-1-i),
	"vdd",
	"vss",NULL);
for (j=0;j<nmot;j+=2){
	if ((j == 0) && (type % 2 == 1))  cell_mid_mem = strdup("rf_mid_mem_r0");
	else                              cell_mid_mem = strdup("rf_mid_mem");
	GENLIB_PLACE_RIGHT(cell_mid_mem,GENLIB_NAME("bit%d_%d",nbitu-1-i,j),sym1);
	GENLIB_LOINS(cell_mid_mem,GENLIB_NAME("bit%d_%d",nbitu-1-i,j),
	GENLIB_NAME("dinx[%d]",nbitu-1-i),
	GENLIB_NAME("write%d",j),
	GENLIB_NAME("read%d",j),
	GENLIB_NAME("rbus[%d]",nbitu-1-i),
	"vdd",
	"vss",NULL);
	cell_mid_mem = strdup("rf_mid_mem");
	GENLIB_PLACE_RIGHT(cell_mid_mem,GENLIB_NAME("bit%d_%d",nbitu-1-i,j+1),sym2);
	GENLIB_LOINS(cell_mid_mem,GENLIB_NAME("bit%d_%d",nbitu-1-i,j+1),
	GENLIB_NAME("dinx[%d]",nbitu-1-i),
	GENLIB_NAME("write%d",j+1),
	GENLIB_NAME("read%d",j+1),
	GENLIB_NAME("rbus[%d]",nbitu-1-i),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT(cell_out_mem,GENLIB_NAME("out%d",nbitu-1-i),sym1);
	GENLIB_LOINS(cell_out_mem,GENLIB_NAME("out%d",nbitu-1-i),
	GENLIB_NAME("rbus[%d]",nbitu-1-i),
	"xck",
	GENLIB_NAME("dataout[%d]",nbitu-1-i),
	"vdd",
	"vss",NULL);
}

///////////////////////////////////////////////////////////////////
//
// placement des cellules du decodeur 
//
///////////////////////////////////////////////////////////////////

if((type == 2) || (type == 3)) {

half = nmot >> 1;

// Premiere ligne decodeur ecriture

bottom = (nbitd+nbitu+hbuf+2) * HCELL;
GENLIB_PLACE("rf_dec_bufad0","adw0",NOSYM,0,bottom);
GENLIB_LOINS("rf_dec_bufad0","adw0",
	"adw[0]",
	"nadw0x",
	"adw0x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
	GENLIB_PLACE_RIGHT("rf_dec_nor3",GENLIB_NAME("selw%d",2*j),SYM_X);
	GENLIB_LOINS("rf_dec_nor3",GENLIB_NAME("selw%d",2*j),
	GENLIB_NAME("nandw%d",j),
	"adw0x",
	"nwen",
	GENLIB_ELM("selw",2*j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("rf_dec_nor3",GENLIB_NAME("selw%d",2*j+1),NOSYM);
	GENLIB_LOINS("rf_dec_nor3",GENLIB_NAME("selw%d",2*j+1),
	GENLIB_NAME("nandw%d",j),
	"nadw0x",
	"nwen",
	GENLIB_ELM("selw",2*j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_nbuf","wen",NOSYM);
GENLIB_LOINS("rf_dec_nbuf","wen",
	"wen",
	"nwen",
	"vdd",
	"vss",NULL);

// Deuxieme ligne decodeur ecriture

bottom = (nbitd+nbitu+hbuf+3) * HCELL;

if (adrange == 5) {    /* adresse sur 5 bits  */

GENLIB_PLACE("rf_dec_bufad2","adw12",SYM_Y,0,bottom);
GENLIB_LOINS("rf_dec_bufad2","adw12",
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
	GENLIB_PLACE_RIGHT("rf_dec_nand4",GENLIB_NAME("nand4w%d",j),SYM_Y);
	GENLIB_LOINS("rf_dec_nand4",GENLIB_NAME("nand4w%d",j),
	x1,
	x2,
	x3,
	x4,
	GENLIB_NAME("nandw%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_bufad2","adw34",SYMXY);
GENLIB_LOINS("rf_dec_bufad2","adw34",
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

GENLIB_PLACE("rf_dec_bufad2","adw12",SYM_Y,0,bottom);
GENLIB_LOINS("rf_dec_bufad2","adw12",
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
	GENLIB_PLACE_RIGHT("rf_dec_nand3",GENLIB_NAME("nand3w%d",j),SYM_Y);
	GENLIB_LOINS("rf_dec_nand3",GENLIB_NAME("nand3w%d",j),
	x1,
	x2,
	x3,
	GENLIB_NAME("nandw%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_bufad1","adw3",SYMXY);
GENLIB_LOINS("rf_dec_bufad1","adw3",
	"adw[3]",
	"nadw3x",
	"adw3x",
	"vdd",
	"vss",NULL);
}

if (adrange == 3) {    /* adresse sur 3 bits  */

GENLIB_PLACE("rf_dec_bufad1","adw1",SYM_Y,0,bottom);
GENLIB_LOINS("rf_dec_bufad1","adw1",
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
	GENLIB_PLACE_RIGHT("rf_dec_nand2",GENLIB_NAME("nand2w%d",j),SYM_Y);
	GENLIB_LOINS("rf_dec_nand2",GENLIB_NAME("nand2w%d",j),
	x1,
	x2,
	GENLIB_NAME("nandw%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_bufad1","adw2",SYMXY);
GENLIB_LOINS("rf_dec_bufad1","adw2",
	"adw[2]",
	"nadw2x",
	"adw2x",
	"vdd",
	"vss",NULL);
}

// Premiere ligne decodeur lecture

bottom = (nbitd+nbitu+hbuf) * HCELL;
GENLIB_PLACE("rf_dec_bufad0","adr0",NOSYM,0,bottom);
GENLIB_LOINS("rf_dec_bufad0","adr0",
	"adr[0]",
	"nadr0x",
	"adr0x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j == 0) {
	GENLIB_PLACE_RIGHT("rf_dec_nao3",GENLIB_NAME("selr%d",2*j),NOSYM);
	GENLIB_LOINS("rf_dec_nao3",GENLIB_NAME("selr%d",2*j),
	GENLIB_NAME("nandr%d",j),
	"adr0x",
	"ren",
	GENLIB_ELM("selr",2*j),
	"vdd",
	"vss",NULL);
        } else {
	GENLIB_PLACE_RIGHT("rf_dec_nor3",GENLIB_NAME("selr%d",2*j),NOSYM);
	GENLIB_LOINS("rf_dec_nor3",GENLIB_NAME("selr%d",2*j),
	GENLIB_NAME("nandr%d",j),
	"adr0x",
	"nren",
	GENLIB_ELM("selr",2*j),
	"vdd",
	"vss",NULL);
	}
	GENLIB_PLACE_RIGHT("rf_dec_nor3",GENLIB_NAME("selr%d",2*j+1),SYM_X);
	GENLIB_LOINS("rf_dec_nor3",GENLIB_NAME("selr%d",2*j+1),
	GENLIB_NAME("nandr%d",j),
	"nadr0x",
	"nren",
	GENLIB_ELM("selr",2*j+1),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_nbuf","ren",NOSYM);
GENLIB_LOINS("rf_dec_nbuf","ren",
	"ren",
	"nren",
	"vdd",
	"vss",NULL);

// Deuxieme ligne decodeur lecture

bottom = (nbitd+nbitu+hbuf+1) * HCELL;

if (adrange == 5) {    /* adresse sur 5 bits  */

GENLIB_PLACE("rf_dec_bufad2","adr12",SYM_Y,0,bottom);
GENLIB_LOINS("rf_dec_bufad2","adr12",
	"adr[1]",
	"adr[2]",
	"nadr1x",
	"adr1x",
	"nadr2x",
	"adr2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadr1x");
	else                  strcpy(x1,"adr1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadr2x");
	else                  strcpy(x2,"adr2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadr3x");
	else                  strcpy(x3,"adr3x");
        if ((j >> 3) % 2 == 0) strcpy(x4,"nadr4x");
	else                  strcpy(x4,"adr4x");
	GENLIB_PLACE_RIGHT("rf_dec_nand4",GENLIB_NAME("nand4r%d",j),SYM_Y);
	GENLIB_LOINS("rf_dec_nand4",GENLIB_NAME("nand4r%d",j),
	x1,
	x2,
	x3,
	x4,
	GENLIB_NAME("nandr%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_bufad2","adr34",SYMXY);
GENLIB_LOINS("rf_dec_bufad2","adr34",
	"adr[3]",
	"adr[4]",
	"nadr3x",
	"adr3x",
	"nadr4x",
	"adr4x",
	"vdd",
	"vss",NULL);
}

if (adrange == 4) {    /* adresse sur 4 bits  */

GENLIB_PLACE("rf_dec_bufad2","adr12",SYM_Y,0,bottom);
GENLIB_LOINS("rf_dec_bufad2","adr12",
	"adr[1]",
	"adr[2]",
	"nadr1x",
	"adr1x",
	"nadr2x",
	"adr2x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadr1x");
	else                  strcpy(x1,"adr1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadr2x");
	else                  strcpy(x2,"adr2x");
        if ((j >> 2) % 2 == 0) strcpy(x3,"nadr3x");
	else                  strcpy(x3,"adr3x");
	GENLIB_PLACE_RIGHT("rf_dec_nand3",GENLIB_NAME("nand3r%d",j),SYM_Y);
	GENLIB_LOINS("rf_dec_nand3",GENLIB_NAME("nand3r%d",j),
	x1,
	x2,
	x3,
	GENLIB_NAME("nandr%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_bufad1","adr3",SYMXY);
GENLIB_LOINS("rf_dec_bufad1","adr3",
	"adr[3]",
	"nadr3x",
	"adr3x",
	"vdd",
	"vss",NULL);
}

if (adrange == 3) {    /* adresse sur 3 bits  */

GENLIB_PLACE("rf_dec_bufad1","adr1",SYM_Y,0,bottom);
GENLIB_LOINS("rf_dec_bufad1","adr1",
	"adr[1]",
	"nadr1x",
	"adr1x",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
        if (j % 2 == 0)       strcpy(x1,"nadr1x");
	else                  strcpy(x1,"adr1x");
        if ((j >> 1) % 2 == 0) strcpy(x2,"nadr2x");
	else                  strcpy(x2,"adr2x");
	GENLIB_PLACE_RIGHT("rf_dec_nand2",GENLIB_NAME("nand2r%d",j),SYM_Y);
	GENLIB_LOINS("rf_dec_nand2",GENLIB_NAME("nand2r%d",j),
	x1,
	x2,
	GENLIB_NAME("nandr%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_dec_bufad1","adr2",SYMXY);
GENLIB_LOINS("rf_dec_bufad1","adr2",
	"adr[2]",
	"nadr2x",
	"adr2x",
	"vdd",
	"vss",NULL);
}
}

///////////////////////////////////////////////////////////////////
//
// placement des cellules du controleur FIFO 
//
///////////////////////////////////////////////////////////////////

if (type == 4) {

half = nmot >> 1;

// Premiere ligne

bottom = (nbitd+nbitu+hbuf) * HCELL;
GENLIB_PLACE("rf_fifo_nop","inspwnop",SYM_X,0,bottom);
GENLIB_LOINS("rf_fifo_nop","inspwnop",
	"ckm",
	"nreset",
	"w",
	"wok",
	"nwval",
	"pwnop",
	"vdd",
	"vss",NULL);
GENLIB_PLACE_RIGHT("rf_fifo_ptset","pw0",NOSYM);
GENLIB_LOINS("rf_fifo_ptset","pw0",
	"pwnop",
	"pwinc",
	"xcks",
	"nreset",
	GENLIB_NAME("selw[%d]",nmot-1),
	"selw[0]",
	"vdd",
	"vss",NULL);
for(j=1 ; j<half ; j++) {
	GENLIB_PLACE_RIGHT("rf_fifo_ptreset",GENLIB_NAME("pw%d",j),NOSYM);
	GENLIB_LOINS("rf_fifo_ptreset",GENLIB_NAME("pw%d",j),
	"pwnop",
	"pwinc",
	"xcks",
	"xreset",
	GENLIB_NAME("selw[%d]",j-1),
	GENLIB_NAME("selw[%d]",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_fifo_buf","fifo_buf",SYM_X);
GENLIB_LOINS("rf_fifo_buf","fifo_buf",
	"cks",
	"ckm",
	"r",
	"w",
	"reset",
	"xcks",
	"xckm",
	"nr",
	"nw",
	"xreset",
	"nreset",
	"vdd",
	"vss",NULL);

// Deuxieme ligne

bottom = (nbitd+nbitu+hbuf+1) * HCELL;
GENLIB_PLACE("rf_fifo_inc","inspwinc",SYMXY,0,bottom);
GENLIB_LOINS("rf_fifo_inc","inspwinc",
	"ckm",
	"nreset",
	"nwval",
	"pwinc",
	"vdd",
	"vss",NULL);
for(j=0 ; j<half ; j++) {
	GENLIB_PLACE_RIGHT("rf_fifo_ptreset",GENLIB_NAME("pw%d",nmot-1-j),SYMXY);
	GENLIB_LOINS("rf_fifo_ptreset",GENLIB_NAME("pw%d",nmot-1-j),
	"pwnop",
	"pwinc",
	"xcks",
	"xreset",
	GENLIB_NAME("selw[%d]",nmot-2-j),
	GENLIB_NAME("selw[%d]",nmot-1-j),
	"vdd",
	"vss",NULL);
	}

// Troisieme ligne

bottom = (nbitd+nbitu+hbuf+2) * HCELL;
GENLIB_PLACE("rf_fifo_ok","inswok",NOSYM,0,bottom);
GENLIB_LOINS("rf_fifo_ok","inswok",
	"w",
	"orfull0",
	"nr",
	"full",
	"fullnext",
	"wok",
	"vdd",
	"vss",NULL);
for(j=0 ; j < (half-1) ; j++) {
	GENLIB_PLACE_RIGHT("rf_fifo_orand5",GENLIB_NAME("insorfull%d",j),NOSYM);
	GENLIB_LOINS("rf_fifo_orand5",GENLIB_NAME("insorfull%d",j),
	GENLIB_NAME("selw[%d]",j),
	GENLIB_NAME("selr[%d]",j+1),
	GENLIB_NAME("selw[%d]",nmot-1-j),
	GENLIB_NAME("selr[%d]",(nmot-j)%nmot),
	GENLIB_NAME("orfull%d",j+1),
	GENLIB_NAME("orfull%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_fifo_orand4",GENLIB_NAME("insorfull%d",half-1),NOSYM);
GENLIB_LOINS("rf_fifo_orand4",GENLIB_NAME("insorfull%d",half-1),
	GENLIB_NAME("selw[%d]",half-1),
	GENLIB_NAME("selr[%d]",half),
	GENLIB_NAME("selw[%d]",half),
	GENLIB_NAME("selr[%d]",half+1),
	GENLIB_NAME("orfull%d",half-1),
	"vdd",
	"vss",NULL);
GENLIB_PLACE_RIGHT("rf_fifo_empty","insempty",SYM_X);
GENLIB_LOINS("rf_fifo_empty","insempty",
	"xckm",
	"nreset",
	"emptynext",
	"xcks",
	"empty",
	"vdd",
	"vss",NULL);

// Quatrieme ligne

bottom = (nbitd+nbitu+hbuf+3) * HCELL;
GENLIB_PLACE("rf_fifo_full","insfull",SYM_Y,0,bottom);
GENLIB_LOINS("rf_fifo_full","insfull",
	"xckm",
	"xreset",
	"fullnext",
	"xcks",
	"full",
	"vdd",
	"vss",NULL);
GENLIB_PLACE_RIGHT("rf_fifo_orand4","insorempty0",SYMXY);
GENLIB_LOINS("rf_fifo_orand4","insorempty0",
	GENLIB_NAME("selr[%d]",0),
	GENLIB_NAME("selw[%d]",1),
	GENLIB_NAME("selr[%d]",nmot-1),
	GENLIB_NAME("selw[%d]",0),
	"orempty0",
	"vdd",
	"vss",NULL);
for(j=1 ; j < half ; j++) {
	GENLIB_PLACE_RIGHT("rf_fifo_orand5",GENLIB_NAME("insorempty%d",j),SYMXY);
	GENLIB_LOINS("rf_fifo_orand5",GENLIB_NAME("insorempty%d",j),
	GENLIB_NAME("selr[%d]",j),
	GENLIB_NAME("selw[%d]",j+1),
	GENLIB_NAME("selr[%d]",nmot-1-j),
	GENLIB_NAME("selw[%d]",nmot-j),
	GENLIB_NAME("orempty%d",j-1),
	GENLIB_NAME("orempty%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_fifo_ok","insrok",SYMXY);
GENLIB_LOINS("rf_fifo_ok","insrok",
	"r",
	GENLIB_NAME("orempty%d",half-1),
	"nw",
	"empty",
	"emptynext",
	"rok",
	"vdd",
	"vss",NULL);

// Cinquieme ligne

bottom = (nbitd+nbitu+hbuf+4) * HCELL;
GENLIB_PLACE("rf_fifo_clock","insclock",NOSYM,0,bottom);
GENLIB_LOINS("rf_fifo_clock","insclock",
	"ck",
	"wok",
	"cks",
	"ckm",
	"ckok",
	"vdd",
	"vss",NULL);
GENLIB_PLACE_RIGHT("rf_fifo_ptset","pr0",NOSYM);
GENLIB_LOINS("rf_fifo_ptset","pr0",
	"prnop",
	"princ",
	"xcks",
	"nreset",
	GENLIB_NAME("selr[%d]",nmot-1),
	"selr[0]",
	"vdd",
	"vss",NULL);
for(j=1 ; j<half ; j++) {
	GENLIB_PLACE_RIGHT("rf_fifo_ptreset",GENLIB_NAME("pr%d",j),NOSYM);
	GENLIB_LOINS("rf_fifo_ptreset",GENLIB_NAME("pr%d",j),
	"prnop",
	"princ",
	"xcks",
	"xreset",
	GENLIB_NAME("selr[%d]",j-1),
	GENLIB_NAME("selr[%d]",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_fifo_nop","insprnop",NOSYM);
GENLIB_LOINS("rf_fifo_nop","insprnop",
	"ckm",
	"nreset",
	"r",
	"rok",
	"nrval",
	"prnop",
	"vdd",
	"vss",NULL);
	
// Sixieme ligne

bottom = (nbitd+nbitu+hbuf+5) * HCELL;
GENLIB_PLACE("rf_fifo_ptreset",GENLIB_NAME("pr%d",nmot-1),SYMXY,50,bottom);
GENLIB_LOINS("rf_fifo_ptreset",GENLIB_NAME("pr%d",nmot-1),
	"prnop",
	"princ",
	"xcks",
	"xreset",
	GENLIB_NAME("selr[%d]",nmot-2),
	GENLIB_NAME("selr[%d]",nmot-1),
	"vdd",
	"vss",NULL);
for(j=1 ; j<half ; j++) {
	GENLIB_PLACE_RIGHT("rf_fifo_ptreset",GENLIB_NAME("pr%d",nmot-1-j),SYM_Y);
	GENLIB_LOINS("rf_fifo_ptreset",GENLIB_NAME("pr%d",nmot-1-j),
	"prnop",
	"princ",
	"xcks",
	"xreset",
	GENLIB_NAME("selr[%d]",nmot-2-j),
	GENLIB_NAME("selr[%d]",nmot-1-j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rf_fifo_inc","insprinc",SYM_Y);
GENLIB_LOINS("rf_fifo_inc","insprinc",
	"ckm",
	"nreset",
	"nrval",
	"princ",
	"vdd",
	"vss",NULL);
} 

///////////////////////////////////////////////////////////////////
//
// Placement des rappels d'alimentation
//
///////////////////////////////////////////////////////////////////

if((type == 0) || (type == 1)) {
	max = ntot/2 + 2;
	GENLIB_PHSEG(CALU3,12,"vss", 10, 0, 10, (ntot + 2) * HCELL);
	GENLIB_PHSEG(CALU3,12,"vdd", (nmot * 25) + 90, 0,
			(nmot * 25) + 90, (ntot + 2) * HCELL);
	}
if((type == 2) || (type == 3)) {
	max = ntot/2 + 4;
	GENLIB_PHSEG(CALU3,12,"vss", 10, 0, 10, (ntot + 6) * HCELL);
	GENLIB_PHSEG(CALU3,12,"vdd", (nmot * 25) + 90, 0,
			(nmot * 25) + 90, (ntot + 6) * HCELL);
	}
if(type == 4) {
	max = ntot/2 + 5;
	GENLIB_PHSEG(CALU3,12,"vss", 10, 0, 10, (ntot + 8) * HCELL);
	GENLIB_PHSEG(CALU3,12,"vdd", (nmot * 25) + 90, 0,
			(nmot * 25) + 90, (ntot + 8) * HCELL);
	}
for(i = 0 ; i < max ; i++) {
GENLIB_PHBIGVIA(VIA12, 10, i*100, 12, 2, "vss");
GENLIB_PHBIGVIA(VIA23, 10, i*100, 12, 2, "vss");
GENLIB_PHSEG(TALU2, 2, "obs", 5, i*100, 15, i*100);
}
for(i = 0 ; i < max-1 ; i++) {
GENLIB_PHBIGVIA(VIA12, nmot*25 + 90, 50 + i*100, 12, 2, "vdd");
GENLIB_PHBIGVIA(VIA23, nmot*25 + 90, 50 + i*100, 12, 2, "vdd");
GENLIB_PHSEG(TALU2, 2, "obs", nmot*25 + 85, 50 + i*100, nmot*25 + 95, 50 + i*100);
}

GENLIB_DEF_AB(0,0,0,0);

GENLIB_SAVE_PHFIG(model_name);
GENLIB_SAVE_LOFIG(model_name);
}
