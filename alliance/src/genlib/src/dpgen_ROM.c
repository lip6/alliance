//
//
// auteur Alain Greiner
//
// Derniere modification 08/05/2001 
//
// Generateur de ROM au gabarit SXLIB
// Le programme genere une net-list et un placement optimise.
//
// Le nombre de mots peut prendre cinq valeurs : 64, 128, 256, 512, 1024
// Le nombre de bits est un multiple de 4 et doit etre inferieur ou egal a 64
// On peut avoir une sortie tri-state en option (pour construire des ROMs
// de capacite superieure a 1024 mots.
// La largeur est egale a (nmot/64 * 50) + 150 (+ 20 si option tristate)
// La hauteur est egale a (nbit + 8) * 50 pour 64 ou 128 mots 
//                        (nbit + 9) * 50 pour 256, 512 ou 1024 mots 
//
// Les valeurs a encoder dans la ROM sont recuperees dans le tableau data[2048]
// qui contient des "mots" de 32 bits : une valeur est stockee dans deux cases
// consecutives de ce tableau, ce qui permet d'avoir des mots de 64 bits.
// Les poids faibles de la valeur i sont a l'adresse data[2*i]
// Les poids forts de la valeur i sont a l'adresse data[2*i+1]
//
// Les lignes de bit horizontales en ALU2 sont hors grilles et pre-routees
// la ROM respecte le gabarit SXLIB pour ce qui concerne les caissons et les
// alimentations en ALU1 sur les faces est et ouest, mais il n'y a pas 
// de continuite des alimentations en ALU1 dans la zone data.
// Pour la partie "donnees", on utilise les cellules suivantes :
// - rom_data_invss
// - rom_data_insel
// - rom_data_midvss
// - rom_data_midsel
// - rom_data_outvss
// - rom_data_outsel
// - rom_data_outvss_ts
// - rom_data_outsel_ts
//
// Pour le decodeur, on utilise les cellules suivantes :
// - rom_dec_adbuf
// - rom_dec_nop
// - rom_dec_prech
// - rom_dec_col2 
// - rom_dec_col3 
// - rom_dec_col4 
// - rom_dec_colbuf 
// - rom_dec_line01
// - rom_dec_line23
// - rom_dec_line45
// - rom_dec_line67
// - rom_dec_selmux01
// - rom_dec_selmux23
// - rom_dec_selmux45
// - rom_dec_selmux67
// - rom_dec_selmux67_128
// - rom_dec_selmux01_ts
// - rom_dec_selmux23_ts
// - rom_dec_selmux45_ts
// - rom_dec_selmux67_ts
// - rom_dec_selmux67_128_ts


# include   "util_Defs.h"
# include  "dpgen_Defs.h"
# include  "dgn.h"


# define    genNAME  s64printf

//----------------------------------------------------------------
//	DEBUT PROGRAMME  PRINCIPAL			   
//----------------------------------------------------------------
// aFunction : le type  :
//       DPGEN_ROM            (sans tri-state de sortie)
//       DPGEN_ROMTS          (avec tri-state de sortie)
//
// aAL : liste des parametres :
//          model_name  :  nom de modele.
//          flags       :  indexation des connecteurs.
//          nbit        :  nombre de bits (largeur du bus).
//          nmot        :  nombre de mots. 
//----------------------------------------------------------------

extern void  dpgen_ROM (aFunction, aAL)
       long  aFunction;
    va_list  aAL;
{
long nmot;	/* nombre de mots */
long nbit; 	/* nombre de bits */
long flags;    	/* indexation : bit 0 en bas si flags = 0 */
long type;

long HCELL = 50;

long i,j,k,l,iter;
long x1,y1;
long x2,y2;
long sym;
long adrange;

long data[2048];
long value;

char *model_name;  
char signame[30];  

char cellin[30];
char cellmid[30];
char cellout[30];

char x6[30];
char x7[30];
char x8[30];
char x9[30];

//////////////////////////////////////////////////////////
//
// Lecture et verification des parametres
//
//////////////////////////////////////////////////////////


model_name = va_arg (aAL, char*);
flags      = va_arg (aAL, long );
nbit       = va_arg (aAL, long );
nmot       = va_arg (aAL, long );

switch(aFunction) {
  case DPGEN_ROM:   type = 0; break;
  case DPGEN_ROMTS: type = 1; break;
  default : 
    eprinth ("GENLIB_MACRO (DPGEN_ROM, ...)");
    eprintf ("\n  Internal error invalid function code := %d.\n", type);
    EXIT (1);
}


switch(nmot) {
  case   64: adrange =  6; break;
  case  128: adrange =  7; break;
  case  256: adrange =  8; break;
  case  512: adrange =  9; break;
  case 1024: adrange = 10; break;
  default : 
    eprinth ("GENLIB_MACRO (DPGEN_ROM, ...)");
    eprintf ("\n  The word number (nword := %d) ", nmot);
    eprints ("can only takes values\n  32, 64, 128, 256, 512 or 1024.\n");
    EXIT (1);
}

if ((nbit%4 != 0) || (nbit > 64) || (nbit < 4)) {
  eprinth ("GENLIB_MACRO (DPGEN_ROM, ...)");
  eprintf ("\n  The bus width (nbit := %d) ", nbit);
  eprints ("must be even and no larger than 64.\n");
  EXIT (1);
}

///////////////////////////////////////////////////////////////////
//
//           Acquisition du tableau des valeurs a encoder
//
///////////////////////////////////////////////////////////////////

/* ICI IL FAUT APPELER LE PARSER DE FICHIER .vbe  */
/*
  for(i=0 ; i<1024 ; i++) {
  data[2*i] = i;
  data[2*i+1] = 1024 - i;
  }
*/

 dpgen_ROM_code (model_name, nbit, nmot, 0, 0, data);

///////////////////////////////////////////////////////////////////
//
//           Definition de l'interface logique
//
///////////////////////////////////////////////////////////////////

GENLIB_DEF_PHFIG(model_name);
GENLIB_DEF_LOFIG(model_name);

GENLIB_LOCON("ck",IN,"ck");
GENLIB_LOCON("selrom",IN,"selrom");
GENLIB_LOCON(GENLIB_BUS("ad",adrange-1,0),IN,GENLIB_BUS("ad",adrange-1,0));
GENLIB_LOCON(GENLIB_BUS("data",nbit-1,0),IN,GENLIB_BUS("data",nbit-1,0));
GENLIB_LOCON("vdd",IN,"vdd");
GENLIB_LOCON("vss",IN,"vss");

///////////////////////////////////////////////////////////////////
//
//           Placement des lignes du plan memoire              
//
///////////////////////////////////////////////////////////////////

for(i=0;i<nbit;i++){

switch(i%4){	
	case 0:	
	sym = NOSYM;
	strcpy(cellin,"rom_data_invss");
	strcpy(cellmid,"rom_data_midvss");
	if (type == 0) strcpy(cellout,"rom_data_outvss");
	else           strcpy(cellout,"rom_data_outvss_ts");
	break;
	case 1:	
	sym = SYM_Y;
	strcpy(cellin,"rom_data_insel");
	strcpy(cellmid,"rom_data_midsel");
	if (type == 0) strcpy(cellout,"rom_data_outsel");
	else           strcpy(cellout,"rom_data_outsel_ts");
	break;
	case 2:	
	sym = NOSYM;
	strcpy(cellin,"rom_data_insel");
	strcpy(cellmid,"rom_data_midsel");
	if (type == 0) strcpy(cellout,"rom_data_outsel");
	else           strcpy(cellout,"rom_data_outsel_ts");
	break;
	case 3:	
	sym = SYM_Y;
	strcpy(cellin,"rom_data_invss");
	strcpy(cellmid,"rom_data_midvss");
	if (type == 0) strcpy(cellout,"rom_data_outvss");
	else           strcpy(cellout,"rom_data_outvss_ts");
	break;
	}

GENLIB_PLACE(cellin,GENLIB_NAME("in%d",i),sym,0,i*HCELL);
GENLIB_LOINS(cellin,GENLIB_NAME("in%d",i),
	"prech",
	GENLIB_NAME("bit0_%d",i),
	GENLIB_NAME("bit1_%d",i),
	GENLIB_NAME("bit2_%d",i),
	GENLIB_NAME("bit3_%d",i),
	GENLIB_NAME("bit4_%d",i),
	GENLIB_NAME("bit5_%d",i),
	GENLIB_NAME("bit6_%d",i),
	GENLIB_NAME("bit7_%d",i),
	"vdd",
	"vss",NULL);
for (j=0;j<nmot;j+=64){
	GENLIB_PLACE_RIGHT(cellmid,GENLIB_NAME("bit%d_%d",i,j),sym);
	GENLIB_LOINS(cellmid,GENLIB_NAME("bit%d_%d",i,j),
	GENLIB_NAME("line0_%d",j),
	GENLIB_NAME("line2_%d",j),
	GENLIB_NAME("line4_%d",j),
	GENLIB_NAME("line6_%d",j),
	GENLIB_NAME("bit0_%d",i),
	GENLIB_NAME("bit1_%d",i),
	GENLIB_NAME("bit2_%d",i),
	GENLIB_NAME("bit3_%d",i),
	GENLIB_NAME("bit4_%d",i),
	GENLIB_NAME("bit5_%d",i),
	GENLIB_NAME("bit6_%d",i),
	GENLIB_NAME("bit7_%d",i),
	"vss",NULL);
	GENLIB_PLACE_RIGHT(cellmid,GENLIB_NAME("bit%d_%d",i,j+32),sym);
	GENLIB_LOINS(cellmid,GENLIB_NAME("bit%d_%d",i,j+32),
	GENLIB_NAME("line1_%d",j),
	GENLIB_NAME("line3_%d",j),
	GENLIB_NAME("line5_%d",j),
	GENLIB_NAME("line7_%d",j),
	GENLIB_NAME("bit0_%d",i),
	GENLIB_NAME("bit1_%d",i),
	GENLIB_NAME("bit2_%d",i),
	GENLIB_NAME("bit3_%d",i),
	GENLIB_NAME("bit4_%d",i),
	GENLIB_NAME("bit5_%d",i),
	GENLIB_NAME("bit6_%d",i),
	GENLIB_NAME("bit7_%d",i),
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT(cellout,GENLIB_NAME("out%d",i),sym);
if (type == 0) {
	GENLIB_LOINS(cellout,GENLIB_NAME("out%d",i),
	"nprech",
	"mux0",
	"mux1",
	"mux2",
	"mux3",
	"mux4",
	"mux5",
	"mux6",
	"mux7",
	GENLIB_NAME("bit0_%d",i),
	GENLIB_NAME("bit1_%d",i),
	GENLIB_NAME("bit2_%d",i),
	GENLIB_NAME("bit3_%d",i),
	GENLIB_NAME("bit4_%d",i),
	GENLIB_NAME("bit5_%d",i),
	GENLIB_NAME("bit6_%d",i),
	GENLIB_NAME("bit7_%d",i),
	GENLIB_NAME("data[%d]",i),
	"vdd",
	"vss",NULL);
	}
else    {
	GENLIB_LOINS(cellout,GENLIB_NAME("out%d",i),
	"enx",
	"nenx",
	"nprech",
	"mux0",
	"mux1",
	"mux2",
	"mux3",
	"mux4",
	"mux5",
	"mux6",
	"mux7",
	GENLIB_NAME("bit0_%d",i),
	GENLIB_NAME("bit1_%d",i),
	GENLIB_NAME("bit2_%d",i),
	GENLIB_NAME("bit3_%d",i),
	GENLIB_NAME("bit4_%d",i),
	GENLIB_NAME("bit5_%d",i),
	GENLIB_NAME("bit6_%d",i),
	GENLIB_NAME("bit7_%d",i),
	GENLIB_NAME("data[%d]",i),
	"vdd",
	"vss",NULL);
	}
}

///////////////////////////////////////////////////////////////////
//
// Placement des cellules decodeur
//
///////////////////////////////////////////////////////////////////

// Lignes 0 et 1

GENLIB_PLACE("rom_dec_prech","prech",NOSYM,0,nbit*HCELL);
GENLIB_LOINS("rom_dec_prech","prech",
"nck",
"prech",
"nprech",
"vdd",
"vss",NULL);
for (j=0;j<nmot;j+=64) {
	if (nmot != 64) strcpy(signame,GENLIB_NAME("col_%d",j));
	else            strcpy(signame,"vdd"); 
	GENLIB_PLACE_RIGHT("rom_dec_line01",GENLIB_NAME("and01_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_line01",GENLIB_NAME("and01_%d",j),
	"nck",
	"nck",
	"sel0",
	"sel1",
	signame,
	GENLIB_NAME("line0_%d",j),
	GENLIB_NAME("line1_%d",j),
	"vdd",
	"vss",NULL);
	}
if(type == 0) {
	GENLIB_PLACE_RIGHT("rom_dec_selmux01","selmux01",NOSYM);
	GENLIB_LOINS("rom_dec_selmux01","selmux01",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux0",
	"sel0",
	"mux1",
	"sel1",
	"vdd",
	"vss",NULL);
	}
else    {
	GENLIB_PLACE_RIGHT("rom_dec_selmux01_ts","selmux01",NOSYM);
	GENLIB_LOINS("rom_dec_selmux01_ts","selmux01",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux0",
	"sel0",
	"mux1",
	"sel1",
	"enx",
	"nenx",
	"vdd",
	"vss",NULL);
	}

// Lignes 2 et 3

GENLIB_PLACE("rom_dec_adbuf","ad4",SYM_Y,0,(nbit+3)*HCELL);
GENLIB_LOINS("rom_dec_adbuf","ad4",
"ad[4]",
"a4",
"na4",
"vdd",
"vss",NULL);
GENLIB_PLACE("rom_dec_adbuf","ad5",NOSYM,0,(nbit+2)*HCELL);
GENLIB_LOINS("rom_dec_adbuf","ad5",
"ad[5]",
"a5",
"na5",
"vdd",
"vss",NULL);
for (j=0;j<nmot;j+=64) {
	if (nmot != 64) strcpy(signame,GENLIB_NAME("col_%d",j));
	else            strcpy(signame,"vdd"); 
	GENLIB_PLACE_RIGHT("rom_dec_line23",GENLIB_NAME("and23_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_line23",GENLIB_NAME("and23_%d",j),
	"nck",
	"nck",
	"sel2",
	"sel3",
	signame,
	GENLIB_NAME("line2_%d",j),
	GENLIB_NAME("line3_%d",j),
	"vdd",
	"vss",NULL);
	}
if(type == 0) {
	GENLIB_PLACE_RIGHT("rom_dec_selmux23","selmux23",NOSYM);
	GENLIB_LOINS("rom_dec_selmux23","selmux23",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux2",
	"sel2",
	"mux3",
	"sel3",
	"vdd",
	"vss",NULL);
	}
else    {
	GENLIB_PLACE_RIGHT("rom_dec_selmux23_ts","selmux23",NOSYM);
	GENLIB_LOINS("rom_dec_selmux23_ts","selmux23",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux2",
	"sel2",
	"mux3",
	"sel3",
	"enx",
	"nenx",
	"vdd",
	"vss",NULL);
	}

// Lignes 4 et 5

GENLIB_PLACE("rom_dec_adbuf","ad2",SYM_Y,0,(nbit+5)*HCELL);
GENLIB_LOINS("rom_dec_adbuf","ad2",
"ad[2]",
"a2",
"na2",
"vdd",
"vss",NULL);
GENLIB_PLACE("rom_dec_adbuf","ad3",NOSYM,0,(nbit+4)*HCELL);
GENLIB_LOINS("rom_dec_adbuf","ad3",
"ad[3]",
"a3",
"na3",
"vdd",
"vss",NULL);
for (j=0;j<nmot;j+=64) {
	if (nmot != 64) strcpy(signame,GENLIB_NAME("col_%d",j));
	else            strcpy(signame,"vdd"); 
	GENLIB_PLACE_RIGHT("rom_dec_line45",GENLIB_NAME("and45_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_line45",GENLIB_NAME("and45_%d",j),
	"nck",
	"nck",
	"sel4",
	"sel5",
	signame,
	GENLIB_NAME("line4_%d",j),
	GENLIB_NAME("line5_%d",j),
	"vdd",
	"vss",NULL);
	}
if(type == 0) {
	GENLIB_PLACE_RIGHT("rom_dec_selmux45","selmux45",NOSYM);
	GENLIB_LOINS("rom_dec_selmux45","selmux45",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux4",
	"sel4",
	"mux5",
	"sel5",
	"vdd",
	"vss",NULL);
	}
else    {
	GENLIB_PLACE_RIGHT("rom_dec_selmux45_ts","selmux45",NOSYM);
	GENLIB_LOINS("rom_dec_selmux45_ts","selmux45",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux4",
	"sel4",
	"mux5",
	"sel5",
	"enx",
	"nenx",
	"vdd",
	"vss",NULL);
	}

// Lignes 6 et 7

GENLIB_PLACE("rom_dec_adbuf","ad0",SYM_Y,0,(nbit+7)*HCELL);
GENLIB_LOINS("rom_dec_adbuf","ad0",
"ad[0]",
"a0",
"na0",
"vdd",
"vss",NULL);
GENLIB_PLACE("rom_dec_adbuf","ad1",NOSYM,0,(nbit+6)*HCELL);
GENLIB_LOINS("rom_dec_adbuf","ad1",
"ad[1]",
"a1",
"na1",
"vdd",
"vss",NULL);
for (j=0;j<nmot;j+=64) {
	if (nmot != 64) strcpy(signame,GENLIB_NAME("col_%d",j));
	else            strcpy(signame,"vdd"); 
	GENLIB_PLACE_RIGHT("rom_dec_line67",GENLIB_NAME("and67_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_line67",GENLIB_NAME("and67_%d",j),
	"nck",
	"nck",
	"sel6",
	"sel7",
	signame,
	GENLIB_NAME("line6_%d",j),
	GENLIB_NAME("line7_%d",j),
	"vdd",
	"vss",NULL);
	}
if((type == 0) && (nmot != 128)) {  // pas de sortie tristate
	GENLIB_PLACE_RIGHT("rom_dec_selmux67","selmux67",NOSYM);
	GENLIB_LOINS("rom_dec_selmux67","selmux67",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux6",
	"sel6",
	"mux7",
	"sel7",
	"vdd",
	"vss",NULL);
	}
if((type != 0) && (nmot != 128)) {  // avec sortie tristate
	GENLIB_PLACE_RIGHT("rom_dec_selmux67_ts","selmux67",NOSYM);
	GENLIB_LOINS("rom_dec_selmux67_ts","selmux67",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ck",
	"selrom",
	"nck",
	"mux6",
	"sel6",
	"mux7",
	"sel7",
	"enx",
	"nenx",
	"vdd",
	"vss",NULL);
	}
if((type == 0) && (nmot == 128)) {  // pas de sortie tristate - 128 mots
	GENLIB_PLACE_RIGHT("rom_dec_selmux67_128","selmux67",NOSYM);
	GENLIB_LOINS("rom_dec_selmux67_128","selmux67",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ad[6]",
	"selrom",
	"col_64",
	"col_0",
	"mux6",
	"sel6",
	"mux7",
	"sel7",
	"vdd",
	"vss",NULL);
	}
if((type != 0) && (nmot == 128)) {  // avec sortie tristate - 128 mots
	GENLIB_PLACE_RIGHT("rom_dec_selmux67_128_ts","selmux67",NOSYM);
	GENLIB_LOINS("rom_dec_selmux67_128_ts","selmux67",
	"a0",
	"na0",
	"a1",
	"na1",
	"a2",
	"na2",
	"a3",
	"na3",
	"a4",
	"na4",
	"a5",
	"na5",
	"ad[6]",
	"selrom",
	"col_64",
	"col_0",
	"mux6",
	"sel6",
	"mux7",
	"sel7",
	"enx",
	"nenx",
	"vdd",
	"vss",NULL);
	}

// Ligne 9 dans le cas 1024 mots

if (nmot == 1024) {
GENLIB_PLACE("rom_dec_nop","nop",NOSYM, 0, (nbit+8)*HCELL);
GENLIB_LOINS("rom_dec_nop","nop", "vdd", "vss", NULL);

for (j=0;j<nmot;j+=64) {
        if ((j >> 6) % 2 == 0) strcpy(x6,"na6x");
	else                  strcpy(x6,"a6x");
        if ((j >> 7) % 2 == 0) strcpy(x7,"na7x");
	else                  strcpy(x7,"a7x");
        if ((j >> 8) % 2 == 0) strcpy(x8,"na8x");
	else                  strcpy(x8,"a8x");
        if ((j >> 9) % 2 == 0) strcpy(x9,"na9x");
	else                  strcpy(x9,"a9x");
	GENLIB_PLACE_RIGHT("rom_dec_col4",GENLIB_NAME("col_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_col4",GENLIB_NAME("col_%d",j),
	x6,
	x7,
	x8,
	x9,
	GENLIB_NAME("col_%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf6",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf6",
"ad[6]",
"a6x",
"na6x",
"vdd",
"vss",NULL);
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf7",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf7",
"ad[7]",
"a7x",
"na7x",
"vdd",
"vss",NULL);
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf8",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf8",
"ad[8]",
"a8x",
"na8x",
"vdd",
"vss",NULL);
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf9",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf9",
"ad[9]",
"a9x",
"na9x",
"vdd",
"vss",NULL);
}

// Ligne 9 dans le cas 512 mots

if (nmot == 512) {
GENLIB_PLACE("rom_dec_nop","nop",NOSYM, 0, (nbit+8)*HCELL);
GENLIB_LOINS("rom_dec_nop","nop", "vdd", "vss", NULL);

for (j=0;j<nmot;j+=64) {
        if ((j >> 6) % 2 == 0) strcpy(x6,"na6x");
	else                  strcpy(x6,"a6x");
        if ((j >> 7) % 2 == 0) strcpy(x7,"na7x");
	else                  strcpy(x7,"a7x");
        if ((j >> 8) % 2 == 0) strcpy(x8,"na8x");
	else                  strcpy(x8,"a8x");
	GENLIB_PLACE_RIGHT("rom_dec_col3",GENLIB_NAME("col_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_col3",GENLIB_NAME("col_%d",j),
	x6,
	x7,
	x8,
	GENLIB_NAME("col_%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf6",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf6",
"ad[6]",
"a6x",
"na6x",
"vdd",
"vss",NULL);
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf7",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf7",
"ad[7]",
"a7x",
"na7x",
"vdd",
"vss",NULL);
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf8",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf8",
"ad[8]",
"a8x",
"na8x",
"vdd",
"vss",NULL);
}

// Ligne 9 dans le cas 256 mots

if (nmot == 256) {
GENLIB_PLACE("rom_dec_nop","nop",NOSYM, 0, (nbit+8)*HCELL);
GENLIB_LOINS("rom_dec_nop","nop", "vdd", "vss", NULL);

for (j=0;j<nmot;j+=64) {
        if ((j >> 6) % 2 == 0) strcpy(x6,"na6x");
	else                  strcpy(x6,"a6x");
        if ((j >> 7) % 2 == 0) strcpy(x7,"na7x");
	else                  strcpy(x7,"a7x");
	GENLIB_PLACE_RIGHT("rom_dec_col2",GENLIB_NAME("col_%d",j),NOSYM);
	GENLIB_LOINS("rom_dec_col2",GENLIB_NAME("col_%d",j),
	x6,
	x7,
	GENLIB_NAME("col_%d",j),
	"vdd",
	"vss",NULL);
	}
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf6",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf6",
"ad[6]",
"a6x",
"na6x",
"vdd",
"vss",NULL);
GENLIB_PLACE_RIGHT("rom_dec_colbuf","colbuf7",NOSYM);
GENLIB_LOINS("rom_dec_colbuf","colbuf7",
"ad[7]",
"a7x",
"na7x",
"vdd",
"vss",NULL);
}

///////////////////////////////////////////////////////////////////
//
//  Routage des rappels d'alimentation et des signaux hors grille
//
///////////////////////////////////////////////////////////////////

y1 = nbit*HCELL; 

if(nmot > 128) y2 = (nbit + 9)*HCELL;
else           y2 = (nbit + 8)*HCELL;

if(type == 0)  x2 = (nmot/64*50) + 150;
else           x2 = (nmot/64*50) + 170;

// alimentations verticales ALU3

GENLIB_PHSEG(CALU3,12,"vdd", 10, 0, 10, y2);
GENLIB_PHSEG(CALU3, 2,"vss", 20, 0, 20, y2);
GENLIB_PHSEG(CALU3, 2,"vss", 30, 0, 30, y2);

for(j=0 ; j<nmot ; j+=64) {
  GENLIB_PHSEG(CALU3, 2,"vss", 55+(50*j/64), 0, 55+(50*j/64), y2);
  GENLIB_PHSEG(CALU3, 2,"vss", 80+(50*j/64), 0, 80+(50*j/64), y2);
  }

if(type == 0) {
  GENLIB_PHSEG(CALU3, 2,"vss", 125+(nmot/64*50), 0, 125+(nmot/64*50), y1);
  GENLIB_PHSEG(CALU3, 2,"vdd", 135+(nmot/64*50), 0, 135+(nmot/64*50), y1);
  GENLIB_PHSEG(CALU3, 2,"vdd", 145+(nmot/64*50), 0, 145+(nmot/64*50), y1);
  }
else {
  GENLIB_PHSEG(CALU3, 2,"vss", 135+(nmot/64*50), 0, 135+(nmot/64*50), y1);
  GENLIB_PHSEG(CALU3, 2,"vdd", 145+(nmot/64*50), 0, 145+(nmot/64*50), y1);
  GENLIB_PHSEG(CALU3, 2,"vss", 155+(nmot/64*50), 0, 155+(nmot/64*50), y2);
  GENLIB_PHSEG(CALU3, 2,"vdd", 165+(nmot/64*50), 0, 165+(nmot/64*50), y2);
  }

// alimemtations horizontales ALU2

for(i = 0 ; i < nbit ; i+=4) {
  GENLIB_PHSEG(CALU2, 2,"vss", 0, i*HCELL, x2, i*HCELL);
  }
for(l=0 ; l<8 ; l+=2) {
  GENLIB_PHSEG(CALU2, 2,"vss", 0, (nbit+l  )*HCELL, x2, (nbit+l  )*HCELL);
  GENLIB_PHSEG(CALU2, 2,"vdd", 0, (nbit+l+1)*HCELL, x2, (nbit+l+1)*HCELL);
  }
GENLIB_PHSEG(CALU2, 2,"vss", 0, (nbit+8)*HCELL, x2, (nbit+8)*HCELL);
if(nmot > 128) GENLIB_PHSEG(CALU2, 2,"vdd", 0, (nbit+9)*HCELL, x2, (nbit+9)*HCELL);

// alimentations horizontales ALU1

for(i=0 ; i<nbit ; i+=2) {
  GENLIB_PHSEG(CALU1, 6,"vss", 0   , HCELL*i+ 3,  5, HCELL*i+ 3);
  GENLIB_PHSEG(CALU1, 6,"vss", x2-5, HCELL*i+ 3, x2, HCELL*i+ 3);
  GENLIB_PHSEG(CALU1, 6,"vdd", 0   , HCELL*i+47,  5, HCELL*i+47);
  GENLIB_PHSEG(CALU1, 6,"vdd", x2-5, HCELL*i+47, x2, HCELL*i+47);
  GENLIB_PHSEG(CALU1, 6,"vdd", 0   , HCELL*i+53,  5, HCELL*i+53);
  GENLIB_PHSEG(CALU1, 6,"vdd", x2-5, HCELL*i+53, x2, HCELL*i+53);
  GENLIB_PHSEG(CALU1, 6,"vss", 0   , HCELL*i+97,  5, HCELL*i+97);
  GENLIB_PHSEG(CALU1, 6,"vss", x2-5, HCELL*i+97, x2, HCELL*i+97);
  }
for(l=0 ; l<8 ; l+=2) {
  GENLIB_PHSEG(CALU1, 6,"vss", 0, y1+HCELL*l+ 3, x2, y1+HCELL*l+ 3);
  GENLIB_PHSEG(CALU1, 6,"vdd", 0, y1+HCELL*l+47, x2, y1+HCELL*l+47);
  GENLIB_PHSEG(CALU1, 6,"vdd", 0, y1+HCELL*l+53, x2, y1+HCELL*l+53);
  GENLIB_PHSEG(CALU1, 6,"vss", 0, y1+HCELL*l+97, x2, y1+HCELL*l+97);
  }
if(nmot > 128) {
  GENLIB_PHSEG(CALU1, 6,"vss", 0, y1+403, x2, y1+403);
  GENLIB_PHSEG(CALU1, 6,"vdd", 0, y1+447, x2, y1+447);
  }

// routage des lignes de bit horizontales hors grille

for(i=0 ; i<nbit ; i++) {
  for(l=0 ; l<8 ; l++) {
    x1 = GENLIB_GET_REF_X(GENLIB_NAME("in%d",i),GENLIB_NAME("bit%d",l));
    y1 = GENLIB_GET_REF_Y(GENLIB_NAME("in%d",i),GENLIB_NAME("bit%d",l));
    x2 = GENLIB_GET_REF_X(GENLIB_NAME("out%d",i),GENLIB_NAME("bit%d",l));
    y2 = GENLIB_GET_REF_Y(GENLIB_NAME("out%d",i),GENLIB_NAME("bit%d",l));
    GENLIB_PHSEG(ALU2,2,GENLIB_NAME("bit%d_%d",l,i),x1,y1,x2,y2);
  }
}

// cablage a 1 de l'entree col dans le cas 64 mots

if(nmot == 64) {
    x1 = GENLIB_GET_REF_X("and01_0","refcol");
    y1 = GENLIB_GET_REF_Y("and01_0","refcol");
    GENLIB_PHBIGVIA(CONT_VIA2,x1,y1,0,0,"vdd");
    x1 = GENLIB_GET_REF_X("and23_0","refcol");
    y1 = GENLIB_GET_REF_Y("and23_0","refcol");
    GENLIB_PHBIGVIA(CONT_VIA2,x1,y1,0,0,"vdd");
    x1 = GENLIB_GET_REF_X("and45_0","refcol");
    y1 = GENLIB_GET_REF_Y("and45_0","refcol");
    GENLIB_PHBIGVIA(CONT_VIA2,x1,y1,0,0,"vdd");
    x1 = GENLIB_GET_REF_X("and67_0","refcol");
    y1 = GENLIB_GET_REF_Y("and67_0","refcol");
    GENLIB_PHBIGVIA(CONT_VIA2,x1,y1,0,0,"vdd");
    }

///////////////////////////////////////////////////////////////////
//
//   Codage Plan Memoire (codage diffusion)                
//
///////////////////////////////////////////////////////////////////

/* a modifier pour creer ces segments dans une figure separee

for(j=0 ; j<nmot ; j+=32) {            // boucle sur les cellules 
  for(k=0 ; k<4 ; k++) {               // boucle sur les colonnes 
    for(l=0 ; l<8 ; l++) {             // boucle sur le multiplexeur de sortie
      for(i=0 ; i<nbit ; i++) {        // boucle sur les bits du mot
        x1 = GENLIB_GET_REF_X(GENLIB_NAME("bit%d_%d",i,j),GENLIB_NAME("ref%d%d",k,l));
        y1 = GENLIB_GET_REF_Y(GENLIB_NAME("bit%d_%d",i,j),GENLIB_NAME("ref%d%d",k,l));
        if(i < 32) {
          value = data[2*(j + 8*k + l)];
          if((value>>i) % 2 == 0) GENLIB_PHSEG(NTRANS,1,"",x1,y1-3,x1,y1+3);
          else                    GENLIB_PHSEG(POLY  ,1,"",x1,y1-3,x1,y1+3);
          }
        else {
          value = data[2*(j + 8*k + l)+1];
          if((value>>(i-32)) % 2 == 0) GENLIB_PHSEG(NTRANS,1,"",x1,y1-3,x1,y1+3);
          else                         GENLIB_PHSEG(POLY  ,1,"",x1,y1-3,x1,y1+3);
          }
         
      }
    }
  }
}

*/

GENLIB_DEF_AB(0,0,0,0);

GENLIB_SAVE_PHFIG(model_name);
GENLIB_SAVE_LOFIG(model_name);

}


