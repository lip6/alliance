// auteur Alain Greiner
//
// Derniere modification 20/05/2001
//
// Memoire statique simple acces respectant le gabarit SXLIB
// pour pouvoir etre utilise dans un chemin de donnees.
// Le programme genere une net-list et un placement optimise.
// Il y a 2 parametres :
// - nmot definit le nombre de mots 
//          il doit etre multiple de 8, >= 32  et =< 256
// - nbit definit le nombre de bits 
//          il doit etre pair, non nul et =< 64
//
// On utilise les cellules suivantes :
//   colonne d'entree 
// - ram_prech_data        
// - ram_prech_buf0        
// - ram_prech_buf1        
// - ram_prech_dec        
// - ram_mem_data         
// - ram_mem_buf0         
// - ram_mem_buf1         
// - ram_mem_deci         
// - ram_mem_dec2         
// - ram_mem_dec3         
// - ram_mem_dec4         
// - ram_mem_dec5         
// - ram_sense_data       
// - ram_sense_buf0       
// - ram_sense_buf1       
// - ram_sense_decad12       
// - ram_sense_decad2       
// - ram_sense_decad3       
// - ram_sense_decad4       
// - ram_sense_decad5       

#include   "util_Defs.h"
#include  "dpgen_Defs.h"


#define    genNAME  s64printf


//----------------------------------------------------------------
//	DEBUT PROGRAMME  PRINCIPAL			   
//----------------------------------------------------------------
// aFunction : le type  :
//       DPGEN_RAM
//
// aAL : liste des parametres :
//          model_name  :  nom de modele.
//          flags       :  indexation des connecteurs.
//          nbit        :  nombre de bits (largeur du bus).
//          nmot        :  nombre de mots. 
// - le nombre de mots doit etre un multiple de 8 compris entre 32 et 256
// - le nombre de bits doit etre pair, non nul et =< 64
//----------------------------------------------------------------

extern void  dpgen_RAM (aFunction, aAL)
     long  aFunction;
  va_list  aAL;
{
int   nmot;      	/* nombre de mots */
int   nbit;      	/* nombre de bits */
char *model_name;	/* nom de modele */
long  flags;

int HCELL = 50;

int bottom,i,j,k;
int sym1,sym2;
int adrange;

char x3[10];
char x4[10];
char x5[10];
char x6[10];
char x7[10];

//////////////////////////////////////////////////////////
//
// acquisition et verification des parametres
//
//////////////////////////////////////////////////////////

model_name = va_arg (aAL, char*);
flags      = va_arg (aAL, long );
nbit       = va_arg (aAL, long );
nmot       = va_arg (aAL, long );


if ((nmot%8 != 0) || (nmot > 256) || (nmot < 32)) {
  eprinth ("GENLIB_MACRO (DPGEN_RAM, ...)");
  eprintf ("\n  The word number (nword := %d) ", nmot);
  eprints ("must be multiple of 8, at least 32");
  eprints ("\n  and no larger than 256.\n");
  EXIT (1);
}
if ((nbit%2 == 1) || (nbit > 64) || (nbit < 2)) {
  eprinth ("GENLIB_MACRO (DPGEN_RAM, ...)");
  eprintf ("\n  The bus width (nbit := %d) ", nbit);
  eprints ("must be even and no larger than 64.\n");
  EXIT (1);
}

GENLIB_DEF_PHFIG(model_name);
GENLIB_DEF_LOFIG(model_name);

///////////////////////////////////////////////////////////////////
//
//           Definition de l'interface logique
//
///////////////////////////////////////////////////////////////////

if (nmot > 128) adrange = 8;
else if (nmot > 64) adrange = 7;
else if (nmot > 32) adrange = 6;
else adrange = 5;

GENLIB_LOCON("ck",IN,"ck");
GENLIB_LOCON("w",IN,"w");
GENLIB_LOCON("selram",IN,"selram");
for (i=0;i<adrange;i++){
	GENLIB_LOCON(GENLIB_NAME("ad[%d]",adrange-1-i),IN,
	GENLIB_NAME("ad[%d]",adrange-1-i));
	}
for (i=0;i<nbit;i++){
	GENLIB_LOCON(GENLIB_NAME("datain[%d]",nbit-1-i),IN,
	GENLIB_NAME("datain[%d]",nbit-1-i));
	}
for (i=0;i<nbit;i++){
	GENLIB_LOCON(GENLIB_NAME("dataout[%d]",nbit-1-i),TRISTATE,
	GENLIB_NAME("dataout[%d]",nbit-1-i));
	}
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");

///////////////////////////////////////////////////////////////////
//
// placement des lignes de bit
//
///////////////////////////////////////////////////////////////////

bottom = 0;
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
GENLIB_PLACE("ram_prech_data",GENLIB_NAME("prech%d",i),sym1,0,bottom+i*HCELL);
GENLIB_LOINS("ram_prech_data",GENLIB_NAME("prech%d",i),
	"prech",
	GENLIB_NAME("bus0_%d",i),
	GENLIB_NAME("nbus0_%d",i),
	GENLIB_NAME("bus1_%d",i),
	GENLIB_NAME("nbus1_%d",i),
	"vdd",
	"vss",NULL);
for (j=0 ; j<nmot ; j+=4){
	GENLIB_PLACE_RIGHT("ram_mem_data",GENLIB_NAME("bit_%d_%d",i,j),sym2);
	GENLIB_LOINS("ram_mem_data",GENLIB_NAME("bit_%d_%d",i,j),
	GENLIB_NAME("selx%d",j),
	GENLIB_NAME("bus0_%d",i),
	GENLIB_NAME("nbus0_%d",i),
	GENLIB_NAME("bus1_%d",i),
	GENLIB_NAME("nbus1_%d",i),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("ram_mem_data",GENLIB_NAME("bit_%d_%d",i,j+2),sym1);
	GENLIB_LOINS("ram_mem_data",GENLIB_NAME("bit_%d_%d",i,j+2),
	GENLIB_NAME("selx%d",j+2),
	GENLIB_NAME("bus0_%d",i),
	GENLIB_NAME("nbus0_%d",i),
	GENLIB_NAME("bus1_%d",i),
	GENLIB_NAME("nbus1_%d",i),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_data",GENLIB_NAME("sense%d",i),sym1);
GENLIB_LOINS("ram_sense_data",GENLIB_NAME("sense%d",i),
	GENLIB_NAME("bus0_%d",i),
	GENLIB_NAME("nbus0_%d",i),
	GENLIB_NAME("bus1_%d",i),
	GENLIB_NAME("nbus1_%d",i),
	"ad0x",
	"nad0x",
	"sensex",
	"nsensex",
	"prechbis",
	"writex",
	GENLIB_NAME("datain[%d]",i),
	GENLIB_NAME("dataout[%d]",i),
	"vdd",
	"vss",NULL);
}

///////////////////////////////////////////////////////////////////
//
// placement de la premiere ligne de la zone des buffers
//
///////////////////////////////////////////////////////////////////

bottom = nbit * HCELL;
GENLIB_PLACE("ram_prech_buf0","prechbuf0",NOSYM,0,bottom);
GENLIB_LOINS("ram_prech_buf0","prechbuf0",
	"nckx",
	"prech",
	"vdd",
	"vss",NULL);
for (j=0 ; j<nmot ; j+=4){
	GENLIB_PLACE_RIGHT("ram_mem_buf0",GENLIB_NAME("buf0_%d",j),SYM_X);
	GENLIB_LOINS("ram_mem_buf0",GENLIB_NAME("buf0_%d",j),
	GENLIB_NAME("nsel%d",j),
	GENLIB_NAME("selx%d",j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("ram_mem_buf0",GENLIB_NAME("buf0_%d",j+2),NOSYM);
	GENLIB_LOINS("ram_mem_buf0",GENLIB_NAME("buf0_%d",j+2),
	GENLIB_NAME("nsel%d",j+2),
	GENLIB_NAME("selx%d",j+2),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_buf0","sensebuf0",NOSYM);
GENLIB_LOINS("ram_sense_buf0","sensebuf0",
	"ad[0]",
	"nsense",
	"nckxbis",
	"nwrite",
	"ad0x",
	"nad0x",
	"sensex",
	"nsensex",
	"prechbis",
	"writex",
	"vdd",
	"vss",NULL);

///////////////////////////////////////////////////////////////////
//
// placement de la deuxieme ligne de la zone des buffers
//
///////////////////////////////////////////////////////////////////

bottom = (nbit+1) * HCELL;
GENLIB_PLACE("ram_prech_buf1","prechbuf1",SYM_Y,0,bottom);
GENLIB_LOINS("ram_prech_buf1","prechbuf1",
	"nck",
	"nckx",
	"vdd",
	"vss",NULL);
for (j=0 ; j<nmot ; j+=4){
	GENLIB_PLACE_RIGHT("ram_mem_buf1",GENLIB_NAME("buf1_%d",j),SYMXY);
	GENLIB_LOINS("ram_mem_buf1",GENLIB_NAME("buf1_%d",j),
	GENLIB_NAME("sel%d",j),
	"nck",
	"selramx",
	GENLIB_NAME("nsel%d",j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("ram_mem_buf1",GENLIB_NAME("buf1_%d",j+2),SYM_Y);
	GENLIB_LOINS("ram_mem_buf1",GENLIB_NAME("buf1_%d",j+2),
	GENLIB_NAME("sel%d",j+2),
	"nck",
	"selramx",
	GENLIB_NAME("nsel%d",j+2),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_buf1","sensebuf1",SYM_Y);
GENLIB_LOINS("ram_sense_buf1","sensebuf1",
	"ck",
	"selram",
	"w",
	"nck",
	"selramx",
	"nsense",
	"nwrite",
	"nckxbis",
	"vdd",
	"vss",NULL);

///////////////////////////////////////////////////////////////////
//
// placement de la premiere ligne de la zone du decodeur
//
///////////////////////////////////////////////////////////////////

bottom = (nbit+2) * HCELL;
GENLIB_PLACE("ram_prech_dec0","prechdecb",NOSYM,0,bottom);
GENLIB_LOINS("ram_prech_dec0","prechdecb",
	"vdd",
	"vss",NULL);
for (j=0 ; j<nmot ; j+=8){
	GENLIB_PLACE_RIGHT("ram_mem_deci",GENLIB_NAME("dec_%d",j),SYM_X);
	GENLIB_LOINS("ram_mem_deci",GENLIB_NAME("dec_%d",j),
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"ndec00",
	GENLIB_NAME("sel%d",j),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("ram_mem_deci",GENLIB_NAME("dec_%d",j+2),NOSYM);
	GENLIB_LOINS("ram_mem_deci",GENLIB_NAME("dec_%d",j+2),
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"ndec01",
	GENLIB_NAME("sel%d",j+2),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("ram_mem_deci",GENLIB_NAME("dec_%d",j+4),SYM_X);
	GENLIB_LOINS("ram_mem_deci",GENLIB_NAME("dec_%d",j+4),
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"ndec10",
	GENLIB_NAME("sel%d",j+4),
	"vdd",
	"vss",NULL);
	GENLIB_PLACE_RIGHT("ram_mem_deci",GENLIB_NAME("dec_%d",j+6),NOSYM);
	GENLIB_LOINS("ram_mem_deci",GENLIB_NAME("dec_%d",j+6),
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"ndec11",
	GENLIB_NAME("sel%d",j+6),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_decad12","sensedecad12",NOSYM);
GENLIB_LOINS("ram_sense_decad12","sensedecad12",
	"ad[1]",
	"ad[2]",
	"ndec00",
	"ndec01",
	"ndec10",
	"ndec11",
	"vdd",
	"vss",NULL);

///////////////////////////////////////////////////////////////////
//
// placement de la deuxieme ligne de la zone du decodeur
//
///////////////////////////////////////////////////////////////////

bottom = (nbit+3) * HCELL;
if (adrange == 8) {    /* adresse sur 8 bits  : on decode 5 bits */

GENLIB_PLACE("ram_prech_dec0","prechdecu",SYM_Y,0,bottom);
GENLIB_LOINS("ram_prech_dec0","prechdecu",
	"vdd",
	"vss",NULL);
for(j=0 ; j<nmot ; j+=8) {
        if ((j >> 3) % 2 == 0) strcpy(x3,"nad3x");
	else                   strcpy(x3,"ad3x");
        if ((j >> 4) % 2 == 0) strcpy(x4,"nad4x");
	else                   strcpy(x4,"ad4x");
        if ((j >> 5) % 2 == 0) strcpy(x5,"nad5x");
	else                   strcpy(x5,"ad5x");
        if ((j >> 6) % 2 == 0) strcpy(x6,"nad6x");
	else                   strcpy(x6,"ad6x");
        if ((j >> 7) % 2 == 0) strcpy(x7,"nad7x");
	else                   strcpy(x7,"ad7x");
	GENLIB_PLACE_RIGHT("ram_mem_dec5",GENLIB_NAME("memdec%d",j),SYM_Y);
	GENLIB_LOINS("ram_mem_dec5",GENLIB_NAME("memdec%d",j),
	x3,
	x4,
	x5,
	x6,
	x7,
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_decad5","sensedecad",SYM_Y);
GENLIB_LOINS("ram_sense_decad5","sensedecad",
	"ad[3]",
	"ad[4]",
	"ad[5]",
	"ad[6]",
	"ad[7]",
	"ad3x",
	"nad3x",
	"ad4x",
	"nad4x",
	"ad5x",
	"nad5x",
	"ad6x",
	"nad6x",
	"ad7x",
	"nad7x",
	"vdd",
	"vss",NULL);
}

if (adrange == 7) {    /* adresse sur 7 bits : on decode 4 bits  */

GENLIB_PLACE("ram_prech_dec0","prechdecu",SYM_Y,0,bottom);
GENLIB_LOINS("ram_prech_dec0","prechdecu",
	"vdd",
	"vss",NULL);
for(j=0 ; j<nmot ; j+=8) {
        if ((j >> 3) % 2 == 0) strcpy(x3,"nad3x");
	else                   strcpy(x3,"ad3x");
        if ((j >> 4) % 2 == 0) strcpy(x4,"nad4x");
	else                   strcpy(x4,"ad4x");
        if ((j >> 5) % 2 == 0) strcpy(x5,"nad5x");
	else                   strcpy(x5,"ad5x");
        if ((j >> 6) % 2 == 0) strcpy(x6,"nad6x");
	else                   strcpy(x6,"ad6x");
	GENLIB_PLACE_RIGHT("ram_mem_dec4",GENLIB_NAME("memdec%d",j),SYM_Y);
	GENLIB_LOINS("ram_mem_dec4",GENLIB_NAME("memdec%d",j),
	x3,
	x4,
	x5,
	x6,
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_decad4","sensedecad",SYM_Y);
GENLIB_LOINS("ram_sense_decad4","sensedecad",
	"ad[3]",
	"ad[4]",
	"ad[5]",
	"ad[6]",
	"ad3x",
	"nad3x",
	"ad4x",
	"nad4x",
	"ad5x",
	"nad5x",
	"ad6x",
	"nad6x",
	"vdd",
	"vss",NULL);
}

if (adrange == 6) {    /* adresse sur 6 bits : on decode 3 bits  */

GENLIB_PLACE("ram_prech_dec0","prechdecu",SYM_Y,0,bottom);
GENLIB_LOINS("ram_prech_dec0","prechdecu",
	"vdd",
	"vss",NULL);
for(j=0 ; j<nmot ; j+=8) {
        if ((j >> 3) % 2 == 0) strcpy(x3,"nad3x");
	else                   strcpy(x3,"ad3x");
        if ((j >> 4) % 2 == 0) strcpy(x4,"nad4x");
	else                   strcpy(x4,"ad4x");
        if ((j >> 5) % 2 == 0) strcpy(x5,"nad5x");
	else                   strcpy(x5,"ad5x");
	GENLIB_PLACE_RIGHT("ram_mem_dec3",GENLIB_NAME("memdec%d",j),SYM_Y);
	GENLIB_LOINS("ram_mem_dec3",GENLIB_NAME("memdec%d",j),
	x3,
	x4,
	x5,
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_decad3","sensedecad",SYM_Y);
GENLIB_LOINS("ram_sense_decad3","sensedecad",
	"ad[3]",
	"ad[4]",
	"ad[5]",
	"ad3x",
	"nad3x",
	"ad4x",
	"nad4x",
	"ad5x",
	"nad5x",
	"vdd",
	"vss",NULL);
}

if (adrange == 5) {    /* adresse sur 5 bits : on decode 2 bits  */

GENLIB_PLACE("ram_prech_dec0","prechdecu",SYM_Y,0,bottom);
GENLIB_LOINS("ram_prech_dec0","prechdecu",
	"vdd",
	"vss",NULL);
for(j=0 ; j<nmot ; j+=8) {
        if ((j >> 3) % 2 == 0) strcpy(x3,"nad3x");
	else                   strcpy(x3,"ad3x");
        if ((j >> 4) % 2 == 0) strcpy(x4,"nad4x");
	else                   strcpy(x4,"ad4x");
	GENLIB_PLACE_RIGHT("ram_mem_dec2",GENLIB_NAME("memdec%d",j),SYM_Y);
	GENLIB_LOINS("ram_mem_dec2",GENLIB_NAME("memdec%d",j),
	x3,
	x4,
	GENLIB_NAME("ndeca%d",j),
	GENLIB_NAME("ndecb%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("ram_sense_decad2","sensedecad",SYM_Y);
GENLIB_LOINS("ram_sense_decad2","sensedecad",
	"ad[3]",
	"ad[4]",
	"ad3x",
	"nad3x",
	"ad4x",
	"nad4x",
	"vdd",
	"vss",NULL);
}

///////////////////////////////////////////////////////////////////
//
//    connecteurs d'alimemtation
//
///////////////////////////////////////////////////////////////////

// Metal 1 face ouest
GENLIB_COPY_UP_SEG("vss","prechdecu","vss");
GENLIB_COPY_UP_SEG("vdd","prechdecu","vdd");
GENLIB_COPY_UP_SEG("vss","prechdecb","vss");
GENLIB_COPY_UP_SEG("vdd","prechdecb","vdd");
GENLIB_COPY_UP_SEG("vss","prechbuf1","vss");
GENLIB_COPY_UP_SEG("vdd","prechbuf1","vdd");
GENLIB_COPY_UP_SEG("vss","prechbuf0","vss");
GENLIB_COPY_UP_SEG("vdd","prechbuf0","vdd");
for(i=0;i<nbit;i++){
GENLIB_COPY_UP_SEG("vss",GENLIB_NAME("prech%d",i),"vss");
GENLIB_COPY_UP_SEG("vdd",GENLIB_NAME("prech%d",i),"vdd");
}

// Metal 1 face est
GENLIB_COPY_UP_SEG("vss","sensedecad","vss");
GENLIB_COPY_UP_SEG("vdd","sensedecad","vdd");
GENLIB_COPY_UP_SEG("vss","sensedecad12","vss");
GENLIB_COPY_UP_SEG("vdd","sensedecad12","vdd");
GENLIB_COPY_UP_SEG("vss","sensebuf1","vss");
GENLIB_COPY_UP_SEG("vdd","sensebuf1","vdd");
GENLIB_COPY_UP_SEG("vss","sensebuf0","vss");
GENLIB_COPY_UP_SEG("vdd","sensebuf0","vdd");
for(i=0;i<nbit;i++){
GENLIB_COPY_UP_SEG("vss",GENLIB_NAME("sense%d",i),"vss");
GENLIB_COPY_UP_SEG("vdd",GENLIB_NAME("sense%d",i),"vdd");
}

// Rappels verticaux Metal 3
GENLIB_PHSEG(CALU3, 12, "vdd", 10, 0, 10, (nbit+4)*HCELL);
GENLIB_PHSEG(CALU3, 12, "vss", 30, 0, 30, (nbit+4)*HCELL);
for(i=0 ; i<(nmot/2) ; i+=2) {
GENLIB_PHSEG(CALU3, 12, "vdd", 55+(25*i), 0, 55+(25*i), (nbit+4)*HCELL);
GENLIB_PHSEG(CALU3, 12, "vss", 80+(25*i), 0, 80+(25*i), (nbit+4)*HCELL);
}
GENLIB_PHSEG(CALU3, 12, "vdd", 155+25*(nmot/2), 0, 155+(25*nmot/2), (nbit+4)*HCELL);

// Rappels horizontaux metal 2
for(i=0;i<nbit;i+=2){
GENLIB_PHSEG(CALU2, 2, "vss", 0, HCELL*i, 200+25*(nmot/2), HCELL*i);
GENLIB_PHSEG(CALU2, 2, "vdd", 0, HCELL*(i+1), 200+25*(nmot/2), HCELL*(i+1));
}
GENLIB_PHSEG(CALU2,  2, "vss", 0, HCELL*nbit, 200+25*(nmot/2), HCELL*nbit);
GENLIB_PHSEG(CALU2, 12, "vdd", 0, HCELL*(nbit+1), 200+25*(nmot/2), HCELL*(nbit+1));
GENLIB_PHSEG(CALU2, 12, "vss", 0, HCELL*(nbit+2), 200+25*(nmot/2), HCELL*(nbit+2));
GENLIB_PHSEG(CALU2,  2, "vdd", 0, HCELL*(nbit+3), 200+25*(nmot/2), HCELL*(nbit+3));
GENLIB_PHSEG(CALU2,  2, "vss", 0, HCELL*(nbit+4), 200+25*(nmot/2), HCELL*(nbit+4));

// la premiere colonne de bigviasa vdd
for(i=0 ; i<nbit ; i+=2) {
GENLIB_PHBIGVIA(VIA12, 10, 50+i*HCELL, 12, 2, "vdd");
GENLIB_PHBIGVIA(VIA23, 10, 50+i*HCELL, 12, 2, "vdd");
}
GENLIB_PHBIGVIA(VIA12, 10, 50+nbit*HCELL, 12, 12, "vdd");
GENLIB_PHBIGVIA(VIA23, 10, 50+nbit*HCELL, 12, 12, "vdd");
GENLIB_PHBIGVIA(VIA12, 10, 150+nbit*HCELL, 12, 2, "vdd");
GENLIB_PHBIGVIA(VIA23, 10, 150+nbit*HCELL, 12, 2, "vdd");

// la premiere colonne de bigviasa vss
for(i=0 ; i<nbit ; i+=2) {
GENLIB_PHBIGVIA(VIA12, 30, i*HCELL, 12, 2, "vss");
GENLIB_PHBIGVIA(VIA23, 30, i*HCELL, 12, 2, "vss");
}
GENLIB_PHBIGVIA(VIA12, 30, nbit*HCELL, 12, 2, "vss");
GENLIB_PHBIGVIA(VIA23, 30, nbit*HCELL, 12, 2, "vss");
GENLIB_PHBIGVIA(VIA12, 30, 100+nbit*HCELL, 12, 12, "vss");
GENLIB_PHBIGVIA(VIA23, 30, 100+nbit*HCELL, 12, 12, "vss");
GENLIB_PHBIGVIA(VIA12, 30, 200+nbit*HCELL, 12, 2, "vss");
GENLIB_PHBIGVIA(VIA23, 30, 200+nbit*HCELL, 12, 2, "vss");

// les colonnes alternees de bigvia vss et vdd
for(j=0 ; j<nmot/2 ; j+=2) {
  for(i=0 ; i<nbit ; i+=2) {
  GENLIB_PHBIGVIA(VIA12, 55+j*25, 50+i*HCELL, 12, 2, "vdd");
  GENLIB_PHBIGVIA(VIA23, 55+j*25, 50+i*HCELL, 12, 2, "vdd");
  GENLIB_PHBIGVIA(VIA12, 80+j*25, i*HCELL, 12, 2, "vss");
  GENLIB_PHBIGVIA(VIA23, 80+j*25, i*HCELL, 12, 2, "vss");
  }
  GENLIB_PHBIGVIA(VIA12, 55+j*25, 50+nbit*HCELL, 12, 12, "vdd");
  GENLIB_PHBIGVIA(VIA23, 55+j*25, 50+nbit*HCELL, 12, 12, "vdd");
  GENLIB_PHBIGVIA(VIA12, 55+j*25, 150+nbit*HCELL, 12, 2, "vdd");
  GENLIB_PHBIGVIA(VIA23, 55+j*25, 150+nbit*HCELL, 12, 2, "vdd");

  GENLIB_PHBIGVIA(VIA12, 80+j*25, nbit*HCELL, 12, 2, "vss");
  GENLIB_PHBIGVIA(VIA23, 80+j*25, nbit*HCELL, 12, 2, "vss");
  GENLIB_PHBIGVIA(VIA12, 80+j*25, 100+nbit*HCELL, 12, 12, "vss");
  GENLIB_PHBIGVIA(VIA23, 80+j*25, 100+nbit*HCELL, 12, 12, "vss");
  GENLIB_PHBIGVIA(VIA12, 80+j*25, 200+nbit*HCELL, 12, 2, "vss");
  GENLIB_PHBIGVIA(VIA23, 80+j*25, 200+nbit*HCELL, 12, 2, "vss");
}

// la derniere colonne de bigviasa vdd
for(i=0 ; i<nbit ; i+=2) {
GENLIB_PHBIGVIA(VIA12, 155+(nmot/2*25), 50+i*HCELL, 12, 2, "vdd");
GENLIB_PHBIGVIA(VIA23, 155+(nmot/2*25), 50+i*HCELL, 12, 2, "vdd");
}
GENLIB_PHBIGVIA(VIA12, 155+(nmot/2*25), 50+nbit*HCELL, 12, 12, "vdd");
GENLIB_PHBIGVIA(VIA23, 155+(nmot/2*25), 50+nbit*HCELL, 12, 12, "vdd");
GENLIB_PHBIGVIA(VIA12, 155+(nmot/2*25), 150+nbit*HCELL, 12, 2, "vdd");
GENLIB_PHBIGVIA(VIA23, 155+(nmot/2*25), 150+nbit*HCELL, 12, 2, "vdd");


GENLIB_DEF_AB(0,0,0,0);

GENLIB_SAVE_PHFIG(model_name);
GENLIB_SAVE_LOFIG(model_name);
}

