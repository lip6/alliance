# include  <genlib.h>
# include  <dgn.h>
# define  POWER "vdd", "vss", NULL

extern int main()
{

  int i;
  
  /*************Définition des MACROS**************************/
  
  GENLIB_MACRO(DPGEN_INV,   "model_inv_32", F_PLACE, 32, 1);
  GENLIB_MACRO(DPGEN_MUX2,  "model_mux2_32", F_PLACE, 32, 2);
  GENLIB_MACRO(DPGEN_NMUX2, "model_nmux2_32", F_PLACE, 32);
  GENLIB_MACRO(DPGEN_NBUSE, "model_nbuse_32", F_PLACE, 32);
  GENLIB_MACRO(DPGEN_BUSE,  "model_buse_32", F_PLACE, 32,4);

  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_32", F_PLACE, 32, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_27", F_PLACE, 27, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_26", F_PLACE, 26, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_19", F_PLACE, 19, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_16", F_PLACE, 16, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_14", F_PLACE, 14, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_5", F_PLACE, 5, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_4", F_PLACE, 4, 2);
  GENLIB_MACRO(DPGEN_BUFF,  "model_buff_2", F_PLACE, 2, 2);

  GENLIB_MACRO(DPGEN_CONST, "model_0_0000", F_PLACE, 16, "0x0000");
  GENLIB_MACRO(DPGEN_CONST, "model_0_00000000", F_PLACE, 32, "0x00000000");
  GENLIB_MACRO(DPGEN_CONST, "model_0_0000FFFF", F_PLACE, 32, "0x0000FFFF");
  GENLIB_MACRO(DPGEN_CONST, "model_0_BFC00000", F_PLACE, 32, "0xBFC00000");
  
  GENLIB_MACRO(DPGEN_ROM2,  "model_opx_its5", F_PLACE, 32, "0xFFFFFFFF", "0x7FFFFF7F");
  GENLIB_MACRO(DPGEN_ROM2,  "model_shamt", F_PLACE, 27, "000000000000000000000000000", "111111111111111111111111111");
  GENLIB_MACRO(DPGEN_ROM2,  "model_ir16", F_PLACE, 16, "0X0000", "0xFFFF");
  GENLIB_MACRO(DPGEN_ROM2,  "model_ir18", F_PLACE, 14, "00000000000000", "11111111111111");
  GENLIB_MACRO(DPGEN_ROM2,  "model_opy_its6", F_PLACE, 32, "0xFFFFFFF7", "0xFFFFFFE7");
  GENLIB_MACRO(DPGEN_ROM2,  "model_opy_its4", F_PLACE, 32, "0xFFFFFFFB", "0xFFFFFFEF");

  GENLIB_MACRO(DPGEN_NUL,   "model_nul", F_PLACE, 32);
  
  GENLIB_MACRO(DPGEN_SHIFT, "model_shift", F_PLACE, 32);

  GENLIB_MACRO(DPGEN_RF1DR0,"model_banc_reg", F_PLACE, 32, 32);
  GENLIB_MACRO(DPGEN_SFFT,  "model_sfft", F_PLACE, 32);
  
  GENLIB_MACRO(DPGEN_AND2,  "model_and2_32", F_PLACE, 32, 2);
  GENLIB_MACRO(DPGEN_NOR2,  "model_nor2_32", F_PLACE, 32, 1);
  GENLIB_MACRO(DPGEN_OR2,   "model_or2_32", F_PLACE, 32, 2);
  GENLIB_MACRO(DPGEN_XOR2,  "model_xor2_32", F_PLACE, 32, 1);
  GENLIB_MACRO(DPGEN_XNOR2, "model_xnor2_32", F_PLACE, 32, 1);
  
  GENLIB_MACRO(DPGEN_ADSB2F, "model_addition_32", F_PLACE, 32);

 


  /************FIN définition des MACROS***********************/
  
  GENLIB_DEF_LOFIG("mips_dpt");
  
  /**************Définition de l'interface*********************/

  /* Déclaration des signaux d'alimentation */
  GENLIB_LOCON("vdd", IN, "vdd");
  GENLIB_LOCON("vss", IN, "vss");

  /* Ajout du scanpath */
  GENLIB_LOCON("test", IN, "test");
  GENLIB_LOCON("dpt_scin", IN, "dpt_scin");
  GENLIB_LOCON("dpt_scout", OUT, "dpt_scout");

  /* Définition du signal d'horloge*/
  GENLIB_LOCON("ck", IN, "ck");

  /* Signaux de commandes d'écriture dans les registres */
  GENLIB_LOCON("pc_wen", IN, "pc_wen");
  GENLIB_LOCON("ad_wen", IN, "ad_wen");
  GENLIB_LOCON("epc_wen", IN, "epc_wen");
  GENLIB_LOCON("bar_wen", IN, "bar_wen");
  GENLIB_LOCON("ir_wen", IN, "ir_wen");
  GENLIB_LOCON("dt_wen", IN, "dt_wen");
  GENLIB_LOCON("hi_wen", IN, "hi_wen");
  GENLIB_LOCON("lo_wen", IN, "lo_wen");
  GENLIB_LOCON("rf_wen", IN, "rf_wen");
  GENLIB_LOCON("rf_aw[4:0]", IN, "rf_aw[4:0]");
  GENLIB_LOCON("rf_ar[4:0]", IN, "rf_ar[4:0]");

  /* Autorisation d'écriture sur le bus X */
  GENLIB_LOCON("opx_ts7", IN, "opx_ts7");
  GENLIB_LOCON("opx_ts6", IN, "opx_ts6");
  GENLIB_LOCON("opx_ts5", IN, "opx_ts5");
  GENLIB_LOCON("opx_ts4", IN, "opx_ts4");
  GENLIB_LOCON("opx_ts3", IN, "opx_ts3");
  GENLIB_LOCON("opx_ts2", IN, "opx_ts2");
  GENLIB_LOCON("opx_ts1", IN, "opx_ts1");
  GENLIB_LOCON("opx_ts0", IN, "opx_ts0");

  GENLIB_LOCON("opx_mx7", IN, "opx_mx7");
  GENLIB_LOCON("opx_mx6", IN, "opx_mx6");
  GENLIB_LOCON("opx_mx5", IN, "opx_mx5");
  GENLIB_LOCON("opx_mx4", IN, "opx_mx4");
  GENLIB_LOCON("opx_mx3", IN, "opx_mx3");
  GENLIB_LOCON("opx_mx2", IN, "opx_mx2");
  GENLIB_LOCON("opx_mx1", IN, "opx_mx1");

  /* Signe de l'opérande X */
  GENLIB_LOCON("opx_sign", OUT, "opx_sign");

  /* Signaux d'indication */
  GENLIB_LOCON("crsr_dpt_out[15:0]", OUT, "crsr_dpt_out[15:0]");

  /* Signaux de commandes */
  GENLIB_LOCON("crsr_sts_in[15:0]", IN, "crsr_sts_in[15:0]");

  /* Autorisation d'écriture sur le bus Y */
  GENLIB_LOCON("opy_ts6", IN, "opy_ts6");
  GENLIB_LOCON("opy_ts5", IN, "opy_ts5");
  GENLIB_LOCON("opy_ts4", IN, "opy_ts4");
  GENLIB_LOCON("opy_ts3", IN, "opy_ts3");
  GENLIB_LOCON("opy_ts2", IN, "opy_ts2");
  GENLIB_LOCON("opy_ts1", IN, "opy_ts1");

  GENLIB_LOCON("opy_mx6", IN, "opy_mx6");
  GENLIB_LOCON("opy_mx5", IN, "opy_mx5");
  GENLIB_LOCON("opy_mx4", IN, "opy_mx4");
  GENLIB_LOCON("opy_mx3", IN, "opy_mx3");
  GENLIB_LOCON("opy_mx2", IN, "opy_mx2");
  GENLIB_LOCON("opy_mx1", IN, "opy_mx1");

  GENLIB_LOCON("opy_codop[18:0]", OUT, "opy_codop[18:0]");
  GENLIB_LOCON("opy_rs[4:0]", OUT, "opy_rs[4:0]");
  GENLIB_LOCON("opy_rdrt[4:0]", OUT, "opy_rdrt[4:0]");
  GENLIB_LOCON("opy_rd[4:0]", OUT, "opy_rd[4:0]");
  GENLIB_LOCON("opy_sign", OUT, "opy_sign");
 
  /* Données en entrée */
  GENLIB_LOCON("data_in_dpt[31:0]", IN, "data_in_dpt[31:0]");

  /* Données en sortie */
  GENLIB_LOCON("data_out_dpt[31:0]", OUT, "data_out_dpt[31:0]");
  
  /* Commandes des multiplexeurs de l'ALU */
  GENLIB_LOCON("alu_mx5i0", IN, "alu_mx5i0");
  GENLIB_LOCON("alu_mx4i0", IN, "alu_mx4i0");
  GENLIB_LOCON("alu_mx3i0", IN, "alu_mx3i0");
  GENLIB_LOCON("alu_mx2i0", IN, "alu_mx2i0");
  GENLIB_LOCON("alu_mx2i1", IN, "alu_mx2i1");
  GENLIB_LOCON("alu_mx1i2", IN, "alu_mx1i2");
  GENLIB_LOCON("alu_mx1i1", IN, "alu_mx1i1");
  GENLIB_LOCON("alu_mx1i0", IN, "alu_mx1i0");
  GENLIB_LOCON("alu_mx0i0", IN, "alu_mx0i0");
  
  /* Signaux de l'ALU */
  GENLIB_LOCON("alu_byte", IN, "alu_byte");
  GENLIB_LOCON("alu_half", IN, "alu_half");
  GENLIB_LOCON("alu_test_n", IN, "alu_test_n");
  GENLIB_LOCON("alu_c31", OUT, "alu_c31");
  GENLIB_LOCON("alu_c30", OUT, "alu_c30");
  GENLIB_LOCON("alu_nul", OUT, "alu_nul");
  GENLIB_LOCON("alu_sign", OUT, "alu_sign");
  GENLIB_LOCON("alu_n_31", INOUT, "alu_n_31");
  GENLIB_LOCON("alu_n_1", INOUT, "alu_n_1");
  GENLIB_LOCON("alu_n_0", INOUT, "alu_n_0");
  GENLIB_LOCON("adr_n_1", INOUT, "adr_n_1");
  GENLIB_LOCON("adr_n_0", INOUT, "adr_n_0");
  
  /* Commande de selection entre adresse PC et adresse memoire */
  GENLIB_LOCON("out_mx0i0", IN, "out_mx0i0");

  /* Adresse en sortie */
  GENLIB_LOCON("out_adr[31:0]", OUT, "out_adr[31:0]");
  
  /**********FIN de la définition de l'interface*****************/

  
  
  
  /*****************Implémentation du DATA-PATH******************/
  
  /***************** Description de l'opérande de X ****************/
  /* Affectation de la valeur du registre sélectionné sur le bus X */
  GENLIB_LOINS("model_buff_32","buff_opx_ots",
               "opx_ots[31:0]", 
	       "opx_out[31:0]", 
	       POWER); 

  /* Récupération du signe du signal opx_out dans dans opx_sign*/
  GENLIB_LOINS("buf_x2","buff_opx_sign",
               "opx_out[31]", 
	       "opx_sign", 
	       POWER); 
 

  /* Création de pc4 */

  GENLIB_LOINS("model_0_00000000",
      		"ox00000000pc4",
		"pc4_zero[31:0]",
		POWER);

  GENLIB_LOINS("model_buff_32","buff_pc4", 
               "pc[31:28]", 
	       "pc4_zero[27:0]",
	       "pc4[31:0]", 
	       POWER); 
 
  /* 
   * Descriptions des MUX de X 
   */ 
  GENLIB_LOINS("model_0_0000FFFF","ox0000FFFF", 
               "const_0x0000FFFF[31:0]", 
	       POWER);
 
  GENLIB_LOINS("model_nmux2_32", "nmux2_32_7", 
               "opx_mx7",
	       "pc4_zero[31:0]", "pc4[31:0]", 
	       "opx_its7[31:0]", 
	       POWER);

  GENLIB_LOINS("model_nmux2_32", "nmux2_32_6", 
               "opx_mx6", 
	       "const_0x0000FFFF[31:0]", "dt[31:0]",
	       "opx_its6[31:0]", 
	       POWER);

  GENLIB_LOINS("model_opx_its5", "mux2_32_5", 
               "opx_mx5", 
	       "opx_its5[31:0]", 
	       POWER);

  GENLIB_LOINS("model_nmux2_32", "nmux2_32_4", 
               "opx_mx4", 
	       "epc[31:0]", "bar[31:0]", 
	       "opx_its4[31:0]", 
	       POWER);

  GENLIB_LOINS("model_nmux2_32", "nmux2_32_3", 
               "opx_mx3", 
	       "cr_s[31:0]", "sr_s[31:0]", 
	       "opx_its3[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nmux2_32", "nmux2_32_2", 
               "opx_mx2", 
	       "lo[31:0]", "hi[31:0]", 
	       "opx_its2[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nmux2_32", "nmux2_32_1", 
               "opx_mx1", 
	       "ad[31:0]", "pc[31:0]", 
	       "opx_its1[31:0]", 
	       POWER);
 
  /* 
   * Définition des signaux TRI STATE de X
   */
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_7", 
               "opx_ts7", 
	       "opx_its7[31:0]", 
	       "opx_ots[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_6", 
               "opx_ts6", 
	       "opx_its6[31:0]", 
	       "opx_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_5", 
               "opx_ts5", 
	       "opx_its5[31:0]", 
	       "opx_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_4", 
               "opx_ts4", 
	       "opx_its4[31:0]", 
	       "opx_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_3", 
               "opx_ts3", 
	       "opx_its3[31:0]", 
	       "opx_ots[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_2", 
	       "opx_ts2", 
	       "opx_its2[31:0]", 
	       "opx_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbuse_32_1", 
               "opx_ts1", 
	       "opx_its1[31:0]", 
	       "opx_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_buse_32" , "buse_32_0" , 
               "opx_ts0", 
	       "rf_o[31:0]", 
	       "opx_ots[31:0]", 
	       POWER); 
  
  
  
  /**********************Description des opérandes Y*********************/
   
  /* Chargement du code opération dans opy_codop à partir du registre ir */
  GENLIB_LOINS("model_buff_19", "buff_19_opy_codop", 
               "ir[31:25]", "ir[23]", "ir[13:11]", "ir[16]", "ir[20]", "ir[5:0]", 
	       "opy_codop[18:0]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_rs*/
  GENLIB_LOINS("model_buff_5", "buff_5_opy_rs", 
               "ir[25:21]", 
	       "opy_rs[4:0]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_rdrt*/
  GENLIB_LOINS("model_buff_5", "buff_5_opy_rdrt", 
               "ir[20:16]", 
	       "opy_rdrt[4:0]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_rd*/
  GENLIB_LOINS("model_buff_5", "buff_5_opy_rd", 
               "ir[15:11]", 
	       "opy_rd[4:0]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_shamt*/
  GENLIB_LOINS("model_buff_5", "buff_5_opy_shamt", 
               "ir[10:6]", 
	       "opy_shamt[4:0]", 
	       POWER);
  GENLIB_LOINS("model_buff_27", "buff_27_shamt", 
               "ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]","ir[10]",
	       "opy_shamt[31:5]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_ir16 */
  GENLIB_LOINS("model_buff_16", "buff_16_opy_ir16", 
               "ir[15:0]", 
	       "opy_ir16[15:0]", 
	       POWER);
  GENLIB_LOINS("model_buff_16", "buff_16_opy_ir16_2", 
               "ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]", 
	       "opy_ir16[31:16]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_ir18 */
  GENLIB_LOINS("model_buff_2","buff_2_opy_ir18", 
               "const_0x00000000[1:0]", 
	       "opy_ir18[1:0]", 
	       POWER);
  GENLIB_LOINS("model_buff_16","buff_16_opy_ir18", 
               "ir[15:0]",  
	       "opy_ir18[17:2]", 
	       POWER);
  GENLIB_LOINS("model_buff_14", "buff_14_opy_ir18", 
               "ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]","ir[15]",   
	       "opy_ir18[31:18]", 
	       POWER);
  
  /* Affectation de la valeur au signal opy_iru28 */
  GENLIB_LOINS("model_buff_2","buff_2_opy_iru28", 
               "const_0x00000000[1:0]", 
	       "opy_iru28[1:0]", 
	       POWER);
  GENLIB_LOINS("model_buff_26","buff_26_opy_iru28", 
               "ir[25:0]", 
	       "opy_iru28[27:2]", 
	       POWER);
  GENLIB_LOINS("model_buff_4","buff_4_opy_iru28", 
               "const_0x00000000[31:28]", 
	       "opy_iru28[31:28]", 
	       POWER);
  

  /* 
   * Définition des MUX de Y 
   */

  GENLIB_LOINS("model_opy_its6", "mux2_6", 
               "opy_mx6", 
	       "opy_its6[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_0_BFC00000","oxBFC00000", 
               "const_0xBFC00000[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nmux2_32", "nmux2_5", 
               "opy_mx5", 
	       "const_0xBFC00000[31:0]", "opy_shamt[31:0]", 
	       "opy_its5[31:0]", 
	       POWER);
 
  GENLIB_LOINS("model_opy_its4", "mux2_4", 
               "opy_mx4", 
	       "opy_its4[31:0]", 
	       POWER);
 
  GENLIB_LOINS("model_0_00000000","ox00000000", 
               "const_0x00000000[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nmux2_32", "nmux2_3", 
               "opy_mx3", 
	       "const_0x00000000[31:0]", "opy_ir16[31:0]", 
	       "opy_its3[31:0]", 
	       POWER);

  GENLIB_LOINS("model_nmux2_32", "nmux2_2", 
               "opy_mx2", 
	       "opy_ir18[31:0]", "opy_iru28[31:0]", 
	       "opy_its2[31:0]", 
	       POWER);

  GENLIB_LOINS("model_nmux2_32","nmux2_1", 
               "opy_mx1", 
	       "dt[31:0]", "ad[31:0]", 
	       "opy_its1[31:0]", 
	       POWER); 
  
  /*
   * Définition des TRI STATE Y
   */
  
  GENLIB_LOINS("model_nbuse_32", "nbusey_32_6", 
               "opy_ts6", 
	       "opy_its6[31:0]", 
	       "opy_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbusey_32_5", 
               "opy_ts5", 
	       "opy_its5[31:0]", 
	       "opy_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbusey_32_4", 
               "opy_ts4", 
	       "opy_its4[31:0]", 
	       "opy_ots[31:0]", 
	       POWER);
  
  GENLIB_LOINS("model_nbuse_32", "nbusey_32_3",  
               "opy_ts3", 
	       "opy_its3[31:0]", 
	       "opy_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbusey_32_2", 
               "opy_ts2", 
	       "opy_its2[31:0]", 
	       "opy_ots[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("model_nbuse_32", "nbusey_32_1", 
               "opy_ts1", 
	       "opy_its1[31:0]", 
	       "opy_ots[31:0]", 
	       POWER); 
  
  /*
   * Définition des signaux opy_out et opy_sign
   */
  GENLIB_LOINS("model_buff_32", "buff_32_opy_out", 
               "opy_ots[31:0]", 
	       "opy_out[31:0]", 
	       POWER); 
  GENLIB_LOINS("buf_x2", "buff_1_opy_sign", 
               "opy_ots[31]", 
	       "opy_sign", 
	       POWER); 
  
  
/*********************************************      ALU     ************************************************/


/* DECALEUR DE L'ALU */
  
/* inversion de la commande decalage a droite ou gauche */
GENLIB_LOINS("inv_x1",
    	     "inv_x1_nmx2i0",
	     "alu_mx2i0",
	     "nalu_mx2i0",
	     "vdd","vss",NULL);

/* decalage a gauche et a droite*/
GENLIB_LOINS("model_shift",
             "shift_alu",
	     "alu_mx1i0","nalu_mx2i0",
	     "opy_out[4:0]",
	     "opx_out[31:0]",
	     "alu_shout[31:0]",
	     "vdd","vss",NULL);


/* OPERATIONS ARITHMETIQUES DE L'ALU */

/* nor de X et Y */

GENLIB_LOINS("model_nor2_32",
	 "nor_alu_1",
	 "opx_out[31:0]", "opy_out[31:0]",
	 "alu_nor[31:0]",
	 "vdd","vss",NULL);

/* or de X et Y */

GENLIB_LOINS("model_or2_32",
	 "or_alu_1",
	 "opx_out[31:0]", "opy_out[31:0]",
	 "alu_or[31:0]",
	 "vdd","vss",NULL);

/* and de X et Y */

GENLIB_LOINS("model_and2_32",
	 "and_alu_1",
	 "opx_out[31:0]", "opy_out[31:0]",
	 "alu_and[31:0]",
	    "vdd","vss",NULL);

/* xor de X et Y */

GENLIB_LOINS("model_xnor2_32",
	 "xnor_alu_1",
	 "opx_out[31:0]", "opy_out[31:0]",
	 "alu_xnor[31:0]",
	 "vdd","vss",NULL);

/* additionneur */

/*GENLIB_LOINS( "inv_x2", "inv_alu_mx0i0", "alu_mx0i0", "nalu_mx0i0", POWER);
*/

GENLIB_LOINS(
     "model_addition_32",
     "additionneur",
     "alu_mx0i0",                         
     "alu_cry[32]",       
     "alu_cry[31]",                            
     "opx_out[31:0]", "opy_out[31:0]", "alu_sum[31:0]",
     "vdd", "vss", NULL);
   
/* alu_yop: inversion de opy_out si on veut soustraire */

 /* inversion de opy_out */
/*
 GENLIB_LOINS("model_inv_32",
 	 "inv_opy_out",
 	 "opy_out[31:0]",
 	 "nopy_out[31:0]",
 	 "vdd","vss",NULL);
*/
 /* alu_yop */
/*
 GENLIB_LOINS("model_nmux2_32",
	  "mux_alu_yop",
	  "alu_mx0i0",
	  "opy_out[31:0]","nopy_out[31:0]",
	    "alu_yop[31:0]",
	    "vdd","vss",NULL);
*/
/* alu_cry: propagation de la carry */

 /* remplissage de alu_cry(0) */
/*
  GENLIB_LOINS("buf_x2",
	   "buf_alu_cry0",
	   "alu_mx0i0",
	   "alu_cry[0]",
	   "vdd","vss",NULL);
  */
 /* remplissage de alu_cry[32:1] */
/*
  for (i=0; i<32; i++) {

    GENLIB_LOINS("oa2a2a23_x2",
     	     GENLIB_NAME("sortie_alu_cry%d",i),
	     GENLIB_NAME("opx_out[%d]",i), GENLIB_NAME("alu_yop[%d]",i), GENLIB_NAME("opx_out[%d]",i), GENLIB_NAME("alu_cry[%d]",i),
	     GENLIB_NAME("alu_yop[%d]",i), GENLIB_NAME("alu_cry[%d]",i),
	     GENLIB_NAME("alu_cry[%d]",i+1),
	     "vdd","vss",NULL);

  }
*/
/* somme de X et Y (ou not(Y)) */
/*
  GENLIB_LOINS("model_xor2_32",
	   "xor_1_alu_1",
	   "opx_out[31:0]", "alu_cry[31:0]",
	      "sortie_xor_1_alu_1[31:0]",
	      "vdd","vss",NULL);

  GENLIB_LOINS("model_xor2_32",
	   "xor_2_alu_1",
	   "sortie_xor_1_alu_1[31:0]", "alu_yop[31:0]",
	      "alu_sum[31:0]",
	      "vdd","vss",NULL);
*/
/* MULTIPLEXAGE DE SORTIE DE L'ALU */

/* inversion de alu_test_n */
  
  GENLIB_LOINS("inv_x1",
 	 "inv_alu_test_n",
 	 "alu_test_n",
 	    "nalu_test_n",
 	    "vdd","vss",NULL);

/* constante qui servira au signal de mode test */ 
  
/* signal rempli si mode test */

  GENLIB_LOINS("model_inv_32",
               "inv_32_nzalu_out",
	       "const_0x00000000[31:1]","nalu_test_n",
	       "nzalu_out[31:0]",
	       POWER);

/* serie de multiplexages pour obtenir le signal alu_out */

  
GENLIB_LOINS("model_nmux2_32",
	     "mux_nalu_s1",
	     "alu_mx1i2",
	     "alu_and[31:0]","alu_or[31:0]",
	     "nalu_s1[31:0]",
	     "vdd","vss",NULL);

GENLIB_LOINS("model_mux2_32",
	     "mux_alu_s3",
	     "alu_mx2i1",
	     "alu_sum[31:0]","alu_nor[31:0]",
	     "alu_s3[31:0]",
	     "vdd","vss",NULL);

GENLIB_LOINS("model_nmux2_32",
	     "mux_alu_s2",
	     "alu_mx2i1",
	     "alu_xnor[31:0]","nalu_s1[31:0]",
	     "alu_s2[31:0]",
	     "vdd","vss",NULL);

GENLIB_LOINS("model_mux2_32",
	     "mux_alu_s4",
	     "alu_mx3i0",
	     "alu_s3[31:0]","alu_s2[31:0]",
	     "alu_s4[31:0]",
	     "vdd","vss",NULL);

GENLIB_LOINS("model_nmux2_32",
	     "mux_nalu_s5",
	     "alu_mx4i0",
	     "alu_s4[31:0]","alu_shout[31:0]",
	     "nalu_s5[31:0]",
	     "vdd","vss",NULL);

GENLIB_LOINS("model_nmux2_32",
	     "mux_alu_out",
	     "alu_mx5i0",
	     "nalu_s5[31:0]","nzalu_out[31:0]",
	     "alu_out[31:0]",
	     "vdd","vss",NULL);

/* SORTIES AUXILIAIRES DE L'ALU */

/* alu_nul */

GENLIB_LOINS("model_nul",
             "m_alu_nul",
	     "alu_sum[31:0]",
	     "alu_nul",
	     "vdd","vss",NULL);

/* alu_sign */

GENLIB_LOINS("buf_x2",
	     "buf_x2_sign",
	     "alu_sum[31]",
	     "alu_sign",
	     "vdd","vss",NULL);

/* alu_c31 */

GENLIB_LOINS("inv_x1",
	     "inv_x1_c31",
	     "alu_cry[32]",
	     "alu_c31",
	     "vdd","vss",NULL);

/* alu_c30 */

GENLIB_LOINS("inv_x1",
	 "inv_x1_c30",
	 "alu_cry[31]",
	   "alu_c30",
	   "vdd","vss",NULL);

/* SELECTION DE BYTE HALF OU WORD */

 /* selection de byte */
  
  GENLIB_LOINS("model_nmux2_32",
      		"nmux2_byte",
		"alu_byte",
		"alu_out[7:0]","alu_out[7:0]","alu_out[7:0]","alu_out[7:0]","alu_out[31:0]",
		"data_outb_n[31:0]",
		"vdd","vss",NULL);

 /*selection de half */

  GENLIB_LOINS("model_inv_32",
      		"inv_32_half",
		"alu_out[15:0]","alu_out[15:0]",
		"data_outh_n[31:0]",
		"vdd","vss",NULL);

  /* sortie finale de l'alu */


  GENLIB_LOINS("model_nmux2_32",
      		"nmux2_word",
		"alu_half",
		"data_outh_n[31:0]","data_outb_n[31:0]",
		"data_out_dpt[31:0]",
		"vdd","vss",NULL);

/************************************************* FIN DE L'ALU *****************************************/

/* ADROUT Multiplexer Description */

 /* multiplexer */

  GENLIB_LOINS("model_nmux2_32",
      		"nmux2_adrout",
		"out_mx0i0",
		"pc[31:0]","ad[31:0]",
		"out_adr_n[31:0]",
		"vdd","vss",NULL);

 /* autres sorties */

  GENLIB_LOINS("model_inv_32",
      		"inv_32_adr",
		"out_adr_n[31:0]",
		  "out_adr[31:0]",
		  "vdd","vss",NULL);

  GENLIB_LOINS("inv_x1",
      		"inv_alun0",
		"alu_out[0]",
		  "alu_n_0",
		  "vdd","vss",NULL);

 
  GENLIB_LOINS("inv_x1",
      		"inv_alun1",
		"alu_out[1]",
		  "alu_n_1",
		  "vdd","vss",NULL);


  GENLIB_LOINS("inv_x1",
      		"inv_alun31",
		"alu_out[31]",
		  "alu_n_31",
		  "vdd","vss",NULL);

  GENLIB_LOINS("buf_x2",
      	       "buf_adrn0",
	       "out_adr_n[0]",
	       "adr_n_0",
	       "vdd","vss",NULL);

  GENLIB_LOINS("buf_x2",
      		"buf_adrn1",
		"out_adr_n[1]",
		  "adr_n_1",
		  "vdd","vss",NULL);

/* FIN DE ADROUT Multiplexer Description */


  
/* PC Register Description */

  GENLIB_LOINS("a2_x2","testpc",
               "test","pc_wen",
	       "test_pc",
	       POWER); 
  
  GENLIB_LOINS("model_sfft",
       		"sfft_pc",
		"test_pc",
		"dpt_scin",
		"pc_wen",
		"ck",
		"alu_out[31:0]",
		"pc[31:0]",
		"vdd","vss",NULL);

/* FIN PC Register Description */


/* AD Register Description */

   GENLIB_LOINS("a2_x2","testad",
               "test","ad_wen",
	       "test_ad",
	       POWER); 
  
   GENLIB_LOINS("model_sfft",
       		"sfft_ad",
		"test_ad",
		"pc[31]",
		"ad_wen",
		"ck",
		"alu_out[31:0]",
		"ad[31:0]",
		"vdd","vss",NULL);

/* FIN AD Register Description */

   
/* EPC Register Description */

   GENLIB_LOINS("a2_x2","testepc",
                "test","epc_wen",
	        "test_epc",
	        POWER);
   
   GENLIB_LOINS("model_sfft",
       		"sfft_epc",
		"test_epc",
		"ad[31]",
		"epc_wen",
		"ck",
		"alu_out[31:0]",
		"epc[31:0]",
		"vdd","vss",NULL);

/* FIN EPC Register Description */


/* BAR Register Description */

   GENLIB_LOINS("a2_x2","testbar",
               "test","bar_wen",
	       "test_bar",
	       POWER);
   
   GENLIB_LOINS("model_sfft",
       		"sfft_bar",
		"test_bar",
		"epc[31]",
		"bar_wen",
		"ck",
		"alu_out[31:0]",
		"bar[31:0]",
		"vdd","vss",NULL);

/* FIN BAR Register Description */


/* SR/CR register Description */

  GENLIB_LOINS("model_0_0000",
      		"ox0000",
		"const_0x0000[15:0]",
		POWER);

  GENLIB_LOINS("model_buff_32",
   	       "buff_32_sr",
	       "const_0x0000[15:0]","crsr_sts_in[15:0]",
	       "sr_s[31:0]",
	       "vdd","vss",NULL);
  
  GENLIB_LOINS("model_buff_32",
   	       "buff_32_cr",
	       "sr_s[31:0]",
	       "cr_s[31:0]",
	       "vdd","vss",NULL);

   GENLIB_LOINS("model_buff_16",
   		"buff_16_dpt",
	     	"alu_out[15:0]",
		"crsr_dpt_out[15:0]",
	  	"vdd","vss",NULL);

/* HI Register Description */

   GENLIB_LOINS("a2_x2","testhi",
                "test","hi_wen",
	        "test_hi",
	        POWER);
   
   GENLIB_LOINS("model_sfft",
       		"sfft_hi",
		"test_hi",
		"bar[31]",
		"hi_wen",
		"ck",
		"alu_out[31:0]",
		"hi[31:0]",
		"vdd","vss",NULL);

/* FIN HI Register Description */
   

/* LO Register Description */

   GENLIB_LOINS("a2_x2","testlo",
               "test","lo_wen",
	       "test_lo",
	       POWER);
   
   GENLIB_LOINS("model_sfft",
       		"sfft_lo",
		"test_lo",
		"hi[31]",
		"lo_wen",
		"ck",
		"alu_out[31:0]",
		"lo[31:0]",
		"vdd","vss",NULL);

/* FIN LO Register Description */


/* IR Register Description */

   GENLIB_LOINS("a2_x2","testir",
                "test","ir_wen",
	        "test_ir",
	        POWER);
   
   GENLIB_LOINS("model_sfft",
       		"sfft_ir",
		"test_ir",
		"lo[31]",
		"ir_wen",
		"ck",
		"data_in_dpt[31:0]",
		"ir[31:0]",
		"vdd","vss",NULL);

/* FIN IR Register Description */


/* DT Register Description */

   GENLIB_LOINS("a2_x2","testdt",
                "test","dt_wen",
	        "test_dt",
	        POWER);
   
   GENLIB_LOINS("model_sfft",
       		"sfft_dt",
		"test_dt",
		"ir[31]",
		"dt_wen",
		"ck",
		"data_in_dpt[31:0]",
		"dt[31:0]",
		"vdd","vss",NULL);


   GENLIB_LOINS("buf_x2", "scout", "dt[31]", "dpt_scout",POWER);
   /* FIN DT Register Description */

  /* Banc de registres */
  GENLIB_LOINS("model_banc_reg", "rf_i", 
               "ck", 
	       "un_1", 
	       "rf_wen", "un_1", 
	       "rf_ar[4:0]", "rf_aw[4:0]", 
	       "alu_out[31:0]", "alu_out[31:0]", 
	       "rf_o[31:0]", 
	       POWER); 
  
  GENLIB_LOINS("one_x0","un",
               "un_1",
	       POWER);
  
/***********FIN de l'implémentation du DATA-PATH***************/

  /* End of netlist description. */
  GENLIB_SAVE_LOFIG ();

/*******************DEBUT DU PREPLACEMENT**********************/
GENLIB_DEF_PHFIG("mips_dpt");
GENLIB_DEF_PHSC("mips_dpt");


/**************Placement du banc de registres******************/
GENLIB_SC_PLACE("rf_i",NOSYM,0,0);

/**************Placement de l'opérande X***********************/

/* tristate rf_o */
GENLIB_SC_RIGHT("buse_32_0",NOSYM);
/* epc mux bar */
GENLIB_SC_RIGHT("sfft_epc",NOSYM); 
GENLIB_SC_TOP("un",NOSYM);
GENLIB_SC_TOP("testepc",SYM_Y);
GENLIB_DEF_PHINS("sfft_epc");
GENLIB_SC_RIGHT("nmux2_32_4",NOSYM);
GENLIB_SC_RIGHT("nbuse_32_4",NOSYM);
GENLIB_SC_TOP("testbar",NOSYM);
GENLIB_DEF_PHINS("nbuse_32_4");
GENLIB_SC_RIGHT("sfft_bar",SYM_X);
GENLIB_SC_TOP("testhi",SYM_X);
GENLIB_DEF_PHINS("sfft_bar");
/* hi mux lo */
GENLIB_SC_RIGHT("sfft_hi",NOSYM); 
GENLIB_SC_RIGHT("nmux2_32_2",SYM_X);
GENLIB_SC_RIGHT("nbuse_32_2",NOSYM);
GENLIB_SC_TOP("testlo",NOSYM);
GENLIB_DEF_PHINS("nbuse_32_2");
GENLIB_SC_RIGHT("sfft_lo",SYM_X);
/********************************************* ALU *****************************************************/
GENLIB_SC_RIGHT("inv_32_half",SYM_X);
GENLIB_SC_RIGHT("nmux2_word",SYM_X);
GENLIB_PLACE_RIGHT("tie_x0","tie_alu_5",NOSYM);
GENLIB_SC_RIGHT("nmux2_byte",SYM_X);
GENLIB_SC_RIGHT("inv_alun0",SYM_X);
GENLIB_SC_TOP("inv_alun1",SYMXY);
GENLIB_SC_TOP("mux_alu_out",SYM_X);
GENLIB_SC_TOP("inv_alun31",SYM_X);
GENLIB_DEF_PHINS("inv_alun0");
GENLIB_PLACE_RIGHT("tie_x0","tie_alun_1",NOSYM);
GENLIB_PLACE_RIGHT("tie_x0","tie_alun_2",NOSYM);
GENLIB_SC_RIGHT("inv_32_nzalu_out",SYM_X);
GENLIB_SC_TOP("inv_alu_test_n",SYM_X);
GENLIB_DEF_PHINS("inv_32_nzalu_out");
GENLIB_SC_RIGHT("mux_nalu_s5",SYM_X);
GENLIB_SC_RIGHT("shift_alu",SYM_X);
GENLIB_SC_TOP("inv_x1_nmx2i0",SYMXY);
GENLIB_DEF_PHINS("shift_alu");
GENLIB_SC_RIGHT("mux_alu_s4",SYM_X);
GENLIB_SC_RIGHT("mux_alu_s3",SYM_X);
GENLIB_SC_RIGHT("nor_alu_1",SYM_X);
GENLIB_SC_RIGHT("m_alu_nul",SYM_X);
GENLIB_SC_TOP("buf_x2_sign",SYMXY);
GENLIB_DEF_PHINS("m_alu_nul");
GENLIB_SC_RIGHT("additionneur",SYM_X);
/*GENLIB_SC_RIGHT("xor_2_alu_1",SYM_X);
GENLIB_SC_RIGHT("xor_1_alu_1",SYM_X);
GENLIB_SC_RIGHT("mux_alu_yop",SYM_X);
GENLIB_SC_RIGHT("inv_opy_out",SYM_X);
GENLIB_SC_TOP("buf_alu_cry0",SYM_X);*/
GENLIB_SC_TOP("inv_x1_c30",SYM_X);
GENLIB_SC_TOP("inv_x1_c31",SYMXY);
/*GENLIB_SC_TOP("inv_alu_mx0i0",SYM_X);*/
GENLIB_DEF_PHINS("additionneur");
/*GENLIB_DEF_PHINS("inv_opy_out");*/
/*GENLIB_SC_RIGHT("sortie_alu_cry0",SYM_X);
for (i=1;i<16;i++){
  GENLIB_PLACE_TOP("tie_x0",GENLIB_NAME("tie_alu_cry_%d",i),SYM_Y);
  GENLIB_SC_TOP(GENLIB_NAME("sortie_alu_cry%d",i),SYM_X);
}
GENLIB_DEF_PHINS("sortie_alu_cry0");
GENLIB_SC_RIGHT("sortie_alu_cry16",SYM_X);
for (i=17;i<32;i++){
  GENLIB_PLACE_TOP("tie_x0",GENLIB_NAME("tie_alu_cry_%d",i),SYM_Y);
  GENLIB_SC_TOP(GENLIB_NAME("sortie_alu_cry%d",i),SYM_X);
}
GENLIB_DEF_PHINS("sortie_alu_cry16");*/
GENLIB_SC_RIGHT("mux_alu_s2",SYM_X);
GENLIB_PLACE_RIGHT("tie_x0","tie_alu_4",NOSYM);
GENLIB_SC_RIGHT("xnor_alu_1",SYM_X);
GENLIB_PLACE_RIGHT("tie_x0","tie_alu_3",NOSYM);
GENLIB_SC_RIGHT("mux_nalu_s1",SYM_X);
GENLIB_PLACE_RIGHT("tie_x0","tie_alu_2",NOSYM);
GENLIB_SC_RIGHT("and_alu_1",SYM_X);
GENLIB_PLACE_RIGHT("tie_x0","tie_alu_1",NOSYM);
GENLIB_SC_RIGHT("or_alu_1",SYM_X);
/* buff opx_out */
GENLIB_SC_RIGHT("buff_opx_ots",NOSYM);
GENLIB_SC_TOP("buff_opx_sign",SYM_X);
GENLIB_DEF_PHINS("buff_opx_ots");
/* rom opx_mx5 */
GENLIB_SC_RIGHT("mux2_32_5",NOSYM);
GENLIB_SC_RIGHT("nbuse_32_5",NOSYM);
/* sr_s mux cr_s */
GENLIB_SC_RIGHT("ox0000",NOSYM);
GENLIB_SC_RIGHT("buff_32_sr",NOSYM);
GENLIB_SC_RIGHT("buff_32_cr",NOSYM);
GENLIB_SC_RIGHT("nmux2_32_3",NOSYM);
GENLIB_SC_RIGHT("nbuse_32_3",NOSYM);
/* pc4 */
GENLIB_SC_RIGHT("nbuse_32_7",SYM_X);
GENLIB_SC_RIGHT("nmux2_32_7",SYM_X);
GENLIB_SC_RIGHT("buff_pc4",SYM_X);
GENLIB_SC_RIGHT("ox00000000pc4",SYM_X);
/* pc mux ad */
GENLIB_SC_RIGHT("sfft_pc",NOSYM);
GENLIB_SC_TOP("testpc",SYM_X);
GENLIB_DEF_PHINS("sfft_pc");
GENLIB_SC_RIGHT("nmux2_32_1",SYM_X);
GENLIB_SC_RIGHT("nbuse_32_1",NOSYM);
GENLIB_SC_TOP("testad",SYM_X);
GENLIB_DEF_PHINS("nbuse_32_1");
GENLIB_SC_RIGHT("sfft_ad",SYM_X);
/* out_adr */
GENLIB_SC_RIGHT("buf_adrn0",SYM_X);
GENLIB_SC_TOP("buf_adrn1",SYMXY);
GENLIB_SC_TOP("nmux2_adrout",SYM_X);
GENLIB_DEF_PHINS("buf_adrn0");
GENLIB_PLACE_RIGHT("tie_x0","tie_adrn_1",NOSYM);
GENLIB_PLACE_RIGHT("tie_x0","tie_adrn_2",NOSYM);
GENLIB_SC_RIGHT("inv_32_adr",SYM_X);
/* dt mux */
GENLIB_SC_RIGHT("ox0000FFFF",NOSYM);
GENLIB_SC_RIGHT("nmux2_32_6",SYM_X);
GENLIB_SC_RIGHT("nbuse_32_6",SYM_X);
GENLIB_SC_TOP("testdt",SYM_X);
GENLIB_DEF_PHINS("nbuse_32_6");
GENLIB_SC_RIGHT("sfft_dt",SYM_X);
GENLIB_SC_TOP("scout",SYM_X);
GENLIB_DEF_PHINS("sfft_dt");

/**************Placement de l'opérande Y***********************/
/* dt mux ad  */
GENLIB_SC_RIGHT("nmux2_1",NOSYM);
GENLIB_SC_RIGHT("nbusey_32_1",NOSYM);
/* mux 4 */
GENLIB_SC_RIGHT("mux2_4",NOSYM);
GENLIB_SC_RIGHT("nbusey_32_4",NOSYM);
/* opy_out */
GENLIB_SC_RIGHT("buff_32_opy_out",NOSYM);
GENLIB_SC_TOP("buff_1_opy_sign",NOSYM);
GENLIB_DEF_PHINS("buff_32_opy_out");

/* mux 6 */
GENLIB_SC_RIGHT("mux2_6",NOSYM);
GENLIB_SC_RIGHT("nbusey_32_6",NOSYM);

/* mux 5 */
GENLIB_SC_RIGHT("nbusey_32_5",SYM_X);
GENLIB_SC_RIGHT("nmux2_5",SYM_X);
GENLIB_SC_RIGHT("oxBFC00000",NOSYM);
GENLIB_PLACE_RIGHT("tie_x0","tie_shamt_rom_1",NOSYM);
GENLIB_PLACE_TOP("tie_x0","tie_shamt_rom_2",SYM_Y);
GENLIB_PLACE_TOP("tie_x0","tie_shamt_rom_3",NOSYM);
GENLIB_PLACE_TOP("tie_x0","tie_shamt_rom_4",SYM_Y);
GENLIB_SC_TOP("buff_27_shamt",SYM_X);
GENLIB_DEF_PHINS("tie_shamt_rom_1");
GENLIB_PLACE_RIGHT("tie_x0","tie_shamt_rom_5",NOSYM);
GENLIB_PLACE_RIGHT("tie_x0","tie_shamt_rom_6",NOSYM);
//GENLIB_PLACE_RIGHT("tie_x0","tie_shamt",SYM_X);

/* mux 3 */
GENLIB_SC_RIGHT("ox00000000",NOSYM);
GENLIB_SC_RIGHT("nbusey_32_3",SYM_X);
GENLIB_SC_RIGHT("nmux2_3",SYM_X);
GENLIB_SC_RIGHT("buff_16_opy_ir16",SYM_X);
GENLIB_SC_TOP("buff_16_opy_ir16_2",SYM_X);
GENLIB_SC_TOP("testir",SYM_X);
GENLIB_DEF_PHINS("buff_16_opy_ir16");
GENLIB_PLACE_RIGHT("tie_x0","tie_opy_ir16",NOSYM);
/* ir */
GENLIB_SC_RIGHT("sfft_ir",SYM_X);
/* mux2 */
GENLIB_SC_RIGHT("nbusey_32_2",SYM_X);
GENLIB_SC_RIGHT("nmux2_2",SYM_X);
GENLIB_SC_RIGHT("buff_2_opy_ir18",SYM_X);
GENLIB_SC_TOP("buff_16_opy_ir18",SYM_X);
GENLIB_SC_TOP("buff_14_opy_ir18",SYM_X);
GENLIB_DEF_PHINS("buff_2_opy_ir18");
GENLIB_PLACE_RIGHT("tie_x0","tie_opy_ir18_1",NOSYM);
GENLIB_SC_RIGHT("buff_2_opy_iru28",SYM_X);
GENLIB_SC_TOP("buff_26_opy_iru28",SYM_X);
GENLIB_SC_TOP("buff_4_opy_iru28",SYM_X);
GENLIB_DEF_PHINS("buff_2_opy_iru28");
/* opy_codop */
GENLIB_PLACE_RIGHT("tie_x0","tie_codop_1",NOSYM);
GENLIB_PLACE_TOP("tie_x0","tie_codop_2",SYM_Y);
GENLIB_SC_TOP("buff_19_opy_codop",SYM_X);
/* opy_shamt */
GENLIB_DEF_PHINS("tie_codop_1");
GENLIB_PLACE_RIGHT("tie_x0","tie_codop_3",NOSYM);
GENLIB_PLACE_RIGHT("tie_x0","tie_shamt_1",NOSYM);
GENLIB_PLACE_TOP("tie_x0","tie_shamt_2",SYM_Y);
GENLIB_PLACE_TOP("tie_x0","tie_shamt_3",NOSYM);
GENLIB_PLACE_TOP("tie_x0","tie_shamt_4",SYM_Y);
GENLIB_SC_TOP("buff_5_opy_shamt",SYM_X);
GENLIB_PLACE_TOP("tie_x0","tie_opy_rd",SYM_Y);
/* opy_rd */
GENLIB_SC_TOP("buff_5_opy_rd",SYM_X);
GENLIB_PLACE_TOP("tie_x0","tie_opy_rdrt",SYM_Y);
/* opy_rdrt */
GENLIB_SC_TOP("buff_5_opy_rdrt",SYM_X);
GENLIB_PLACE_TOP("tie_x0","tie_opy_rs",SYM_Y);
/* opy_rs */
GENLIB_SC_TOP("buff_5_opy_rs",SYM_X);

/* boite d'aboutement */
GENLIB_DEF_AB(0,0,0,0);


GENLIB_SAVE_PHFIG();
/********************FIN DU PREPLACEMENT***********************/
  
  return 0;
}
