

# include  <genlib.h>


#define    getbit(val,bit)    (((val) >> (bit))%2)


extern int main()
{
  long  i;


  /* Generate all the operators required for the register file. */
  GENLIB_MACRO (DPGEN_INV  , "model_inv_x8", F_PLACE, 4, 8);
  GENLIB_MACRO (DPGEN_SFF  , "model_sff"   , F_PLACE, 4);
  GENLIB_MACRO (DPGEN_NBUSE, "model_nbuse" , F_PLACE, 4);

  /* Generate all the operators required  */

  GENLIB_MACRO (DPGEN_MUX2  , "model_mux2", F_PLACE, 4, 2);
  GENLIB_MACRO (DPGEN_NAND2MASK, "model_nand2mask_0000", F_PLACE, 4,"0b0000");
  GENLIB_MACRO (DPGEN_XNOR2MASK, "model_xnor2mask_1111", F_PLACE, 4, "0b1111");
  GENLIB_MACRO (DPGEN_NAND2  , "model_nand2", F_PLACE, 4, 4); /* 1 ou 4  */
  GENLIB_MACRO (DPGEN_NOR2MASK  , "model_nor2mask_1111", F_PLACE, 4,"0b1111");
  GENLIB_MACRO (DPGEN_NMUX2  , "model_nmux2", F_PLACE, 4,2);
  GENLIB_MACRO (DPGEN_INV  , "model_inv", F_PLACE, 4,1);
  GENLIB_MACRO (DPGEN_NOR2  , "model_nor2", F_PLACE, 4, 4); /* 1 ou 4  */
  GENLIB_MACRO (DPGEN_XOR2  , "model_xor2", F_PLACE, 4, 4); /* 1 ou 4  */
  GENLIB_MACRO (DPGEN_XNOR2  , "model_xnor2", F_PLACE, 4, 4); /* 1 ou 4  */
  GENLIB_MACRO (DPGEN_SFFT  , "model_sfft", F_PLACE, 4); /* 1 ou 4  */


  /* Netlist description. */
  GENLIB_DEF_LOFIG ("amd2901_dpt");

/* Command for selecting operands R and S.*/
   GENLIB_LOCON ("ops_mx[2:0]" , IN , "ops_mx[2:0]");
   GENLIB_LOCON ("opr_mx[1:0]" , IN , "opr_mx[1:0]");

/* ALU commands and auxiliary terminals. */
   GENLIB_LOCON ("alu_k[4:0]" , IN , "alu_k[4:0]");
   GENLIB_LOCON ("alu_cin" , IN , "alu_cin") ;
   GENLIB_LOCON ("alu_cout", OUT , "alu_cout") ;
   GENLIB_LOCON ("alu_over" , INOUT , "alu_over");

/* RAM, ACCU shifter commands and auxiliary terminals.*/
   GENLIB_LOCON ("ram_sh[1:0]" , IN , "ram_sh[1:0]") ;
   GENLIB_LOCON ("acc_sh[1:0]" , IN , "acc_sh[1:0]") ;

/* RAM shifter inputs.*/
   GENLIB_LOCON ("ram_i_up"   , IN , "ram_i_up");
   GENLIB_LOCON ("ram_i_down" , IN , "ram_i_down");

/* ACCU shifter inputs.*/
   GENLIB_LOCON	("acc_i_up"   , IN , "acc_i_up" ) ;   
   GENLIB_LOCON	("acc_i_down" , IN , "acc_i_down") ;

/* ACCU shifter outputs ("acc_scout" is "acc_q_up").*/
   GENLIB_LOCON	("acc_q_down" , OUT , "acc_q_down");

/* Output multiplexer commnand (for X bus).*/
   GENLIB_LOCON	("out_mx" , IN , "out_mx");

/* ACCU controls terminals.*/
   GENLIB_LOCON	("acc_ck"   , IN , "acc_ck"  );
   GENLIB_LOCON	("acc_wen"  , IN , "acc_wen"  );
   GENLIB_LOCON	("acc_test" , IN ,  "acc_test" );
   GENLIB_LOCON	("acc_scin" , IN ,  "acc_scin") ; /* Scan-Path input.*/
   GENLIB_LOCON	("acc_scout", INOUT ,"acc_scout"); /* Scan-Path output.*/

/* Register file controls terminals.*/
  GENLIB_LOCON ("ram_ck[15:0]", IN ,"ram_ck[15:0]") ;  /* Register clocks (ck) */
  GENLIB_LOCON ("b_w[15:0]" , IN , "b_w[15:0]") ;  /* Write enable */
  GENLIB_LOCON ("a[15:0]" , IN , "a[15:0]") ;  /* Register A address. */
  GENLIB_LOCON ("b[15:0]" , IN , "b[15:0]") ;  /* Register B address. */

/* Data buses terminals.*/
GENLIB_LOCON ("opr_d[3:0]"  , IN ,"opr_d[3:0]");
GENLIB_LOCON ("alu_f[3:0]"  , INOUT ,"alu_f[3:0]");
GENLIB_LOCON ("alu_np[3:0]" , OUT ,"alu_np[3:0]");
GENLIB_LOCON ("alu_ng[3:0]" , OUT , "alu_ng[3:0]");
GENLIB_LOCON ("out_x[3:0]"  ,OUT , "out_x[3:0]");
 
  /* Power supply connectors. */
  GENLIB_LOCON ("vdd", IN   , "vdd");
  GENLIB_LOCON ("vss", IN   , "vss");


  /* Register file description. */
  for (i = 0; i < 16; i++)
  {
    /* Register part. */
    GENLIB_LOINS ("model_sff", GENLIB_NAME("ram_reg%ld",i),
                  GENLIB_ELM  ("b_w", i),
                  GENLIB_ELM  ("ram_ck"  , i),
                               "ram_d[3:0]",
                  GENLIB_NAME ("ram_q%ld[3:0]", i),
                  "vdd", "vss", NULL);

    /* Tristate for A output. */
    GENLIB_LOINS ("model_nbuse", GENLIB_NAME ("ram_ntsa%ld",i),
                  GENLIB_ELM  ("a", i),
                  GENLIB_NAME ("ram_q%ld[3:0]", i),
                               "ram_nra[3:0]",
                  "vdd", "vss", NULL);

    /* Tristate for B output. */
    GENLIB_LOINS ("model_nbuse", GENLIB_NAME("ram_ntsb%ld",i),
                  GENLIB_ELM  ("b", i),
                  GENLIB_NAME ("ram_q%ld[3:0]", i),
                               "ram_nrb[3:0]",
                  "vdd", "vss", NULL);
  }


  /* Output drivers for A & B output. */
  GENLIB_LOINS ("model_inv_x8", "inv_ra",
                "ram_nra[3:0]",
                 "ram_ra[3:0]",
                "vdd", "vss", NULL);
  GENLIB_LOINS ("model_inv_x8", "inv_rb",
                "ram_nrb[3:0]",
                 "ram_rb[3:0]",
                "vdd", "vss", NULL);

  /*  --------------------------------------------------------------
   *  RAM shifter.
   */

  GENLIB_LOINS ("model_nmux2", "ram_nmux_0",
                "ram_sh[0]",
                "ram_i_up", "alu_f[3:1]",                /* i1 */
                            "alu_f[2:0]", "ram_i_down",  /* i0 */
                       "ram_nmux_0[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_inv", "ram_inv_1",
                    "alu_f[3:0]",  /* i2 */
                "ram_inv_1[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nmux2", "ram_nmux_1",
                "ram_sh[1]",
                 "ram_inv_1[3:0]",
                "ram_nmux_0[3:0]",
                     "ram_d[3:0]",
                "vdd", "vss", NULL);



 /* *********************** Operand S ************************ */
  
  GENLIB_LOINS ("model_nmux2", "ops_nmux_0",
                "ops_mx[0]",
                             "ram_rb[3:0]",                /* i1 */
                "acc_scout",  "acc_q[2:1]", "acc_q_down",  /* i0 */
                         "ops_nmux_0[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_inv", "ops_inv_1",
                   "ram_ra[3:0]",          /* i2 */
                "ops_inv_1[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nmux2", "ops_nmux_1",
                "ops_mx[1]",
                 "ops_inv_1[3:0]",
                "ops_nmux_0[3:0]",
                    "ops_it[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nand2mask_0000", "ops_na2mask_0b0000",
                "ops_mx[2]"  ,
                "ops_it[3:0]",
                "ops_ns[3:0]",
                "vdd", "vss", NULL);


/* *********************** Operand R ************************ */


 GENLIB_LOINS ("model_mux2", "opr_mux",
               "opr_mx[0]",
               "opr_d[3:0]",       /* i1 */
               "ram_ra[3:0]",       /* i0 */
               "opr_it[3:0]",
               "vdd", "vss", NULL);

 GENLIB_LOINS ("model_nand2mask_0000", "opr_na2mask_0b0000",
               "opr_mx[1]"  ,
               "opr_it[3:0]",
               "opr_nr[3:0]",
               "vdd", "vss", NULL);

/* *********************** ALU Description ****************** */

 GENLIB_LOINS ("model_xnor2mask_1111", "alu_xr2_opnr",
               "alu_k[0]"  ,
               "opr_nr[3:0]",
               "alu_xr[3:0]",
               "vdd", "vss", NULL);

 GENLIB_LOINS ("model_xnor2mask_1111", "alu_xr2_opns",
                 "alu_k[1]"  ,
                "ops_ns[3:0]",
                "alu_xs[3:0]",
                "vdd", "vss", NULL);


/* Compute of "generate". */
  GENLIB_LOINS ("model_nand2", "alu_na2_ng",
                "alu_xr[3:0]",
                "alu_xs[3:0]",
                "alu_ng[3:0]",
                "vdd", "vss", NULL);
  
/* Compute of "propagate". */
  GENLIB_LOINS ("model_nor2",  "alu_no2_np",
                "alu_xr[3:0]",
                "alu_xs[3:0]",
                "alu_np[3:0]",
                "vdd", "vss", NULL);

GENLIB_LOINS ("model_inv",   "alu_n1_p" ,
                "alu_np[3:0]",
                "alu_p[3:0]",
                "vdd", "vss", NULL);
 /* Compute of carry. */
  GENLIB_LOINS ("model_nand2", "alu_na2_npc"  ,
                "alu_p[3:0]",
                "alu_over", "alu_carry[2:1]", "alu_cin",  
                "alu_npc[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nand2", "alu_na2_carry",
                 "alu_ng[3:0]",
                "alu_npc[3:0]", 
                "alu_cout", "alu_over", "alu_carry[2:1]",
                "vdd", "vss", NULL);
 
  /* Logical and arithmetical operators. */
  GENLIB_LOINS ("model_nor2mask_1111", "alu_no2_and",
                "alu_k[2]"  ,
                "alu_ng[3:0]",
                "alu_r_and_s[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nor2mask_1111", "alu_no2_or" ,
                "alu_k[3]"  ,
                "alu_np[3:0]",
                "alu_r_or_s[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nor2mask_1111", "alu_no2_add",
                "alu_k[4]"  , 
                "alu_over",   "alu_carry[2:1]", "alu_cin",
                "alu_r_add_s[3:0]",
                "vdd", "vss", NULL);

  /* Output. */
  GENLIB_LOINS ("model_xor2", "alu_nxr2_op",
                "alu_r_and_s[3:0]",
                "alu_r_or_s[3:0]",
                "alu_r_op_s[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_xnor2", "alu_nxr2_f" ,
                "alu_r_op_s[3:0]",
                "alu_r_add_s[3:0]",
                "alu_f[3:0]",
                "vdd", "vss", NULL);

 /* ******************** ACCU Description ******************** */

 GENLIB_LOINS ("model_nmux2", "accu_nmux_0",
               "acc_sh[0]",
               "acc_i_up", "acc_scout", "acc_q[2:1]",    /* i1 : down */
               "acc_q[2:1]", "acc_q_down", "acc_i_down", /* i0 :   up */
               "accu_nmux_0[3:0]",
               "vdd", "vss", NULL);

  GENLIB_LOINS ("model_inv", "accu_inv_1",
                "alu_f[3:0]",   /* i2:  no */
                "accu_inv_1[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_nmux2", "accu_nmux_1",
                "acc_sh[1]",
                "accu_inv_1[3:0]",
                "accu_nmux_0[3:0]",
                "acc_d[3:0]",
                "vdd", "vss", NULL);

  GENLIB_LOINS ("model_sfft", "acc_reg",
                "acc_test"   ,
                "acc_scin"   ,
                "acc_wen",
                "acc_ck"     ,
                "acc_d[3:0]",
                "acc_scout", "acc_q[2:1]", "acc_q_down",
                "vdd", "vss", NULL);
		  
 /* ******************* Output Multiplexer ******************* */

 GENLIB_LOINS ("model_mux2", "out_mx",
               "out_mx"     ,
               "ram_ra[3:0]",      /* i1 */
               "alu_f[3:0]",       /* i0 */
               "out_x[3:0]",
               "vdd", "vss", NULL);

  
  /* End of netlist description. */
  GENLIB_SAVE_LOFIG ();


  /* Partial placement description. */
  GENLIB_DEF_PHFIG ("amd2901_rf");


  for (i = 0; i < 16; i++)
  {
    /* Register part. */
    if (!(i % 8)) {
      if (!i) {
        GENLIB_PLACE ("model_sff", GENLIB_NAME ("ram_reg%ld",i), NOSYM, 0, 0);
      } else {
        GENLIB_DEF_PHINS (GENLIB_NAME ("ram_reg%ld", i - 8));
        GENLIB_PLACE_TOP ("model_sff", GENLIB_NAME ("ram_reg%ld",i), NOSYM);
      }
    } else {
      GENLIB_PLACE_RIGHT ("model_sff", GENLIB_NAME ("ram_reg%ld",i), NOSYM);
    }

    GENLIB_PLACE_RIGHT ("model_nbuse", GENLIB_NAME ("ram_ntsa%ld",i), NOSYM);
    GENLIB_PLACE_RIGHT ("model_nbuse", GENLIB_NAME ("ram_ntsb%ld",i), NOSYM);
  }


  GENLIB_PLACE_RIGHT ("model_inv_x8", "ram_ra", NOSYM);

  GENLIB_DEF_PHINS ("ram_ntsb7");
  GENLIB_PLACE_RIGHT ("model_inv_x8", "ram_rb", NOSYM);


  /* Add enougth place for the decoder : 2 slice). */
  GENLIB_DEF_AB (0, 0, 0, 100);


  /* End of placement description. */
  GENLIB_SAVE_PHFIG ();


  /* A good C program must always terminate by an "exit(0)". */
  exit(0);
}
