#include  <genlib.h>

main()
{
  GENLIB_DEF_LOFIG ("amd2901_core");
  GENLIB_DEF_PHFIG ("amd2901_core_place");


  /* ***************** Terminal Declarations ****************** */


  /* Pin terminals associated with ALU. */
  GENLIB_LOCON("cin",  UNKNOWN, "cin" );
  GENLIB_LOCON("cout", UNKNOWN, "cout");
  GENLIB_LOCON("np",   OUT    , "np"  );
  GENLIB_LOCON("ng",   OUT    , "ng"  );
  GENLIB_LOCON("over", OUT    , "over");
  GENLIB_LOCON("zero", OUT    , "zero");

  /* Pin terminals associated with the RAM and ACCU shifter. */
  /* RAM and ACCU I/O plots controls. */
  GENLIB_LOCON(   "sh_right",   OUT,    "sh_right");
  GENLIB_LOCON(   "sh_left" ,   OUT,    "sh_left" );
  /* RAM shifter I/O. */
  GENLIB_LOCON("ram_o_down" ,   OUT, "alu_f[0]" );
  GENLIB_LOCON("ram_o_up"   ,   OUT, "alu_f[3]"   );
  GENLIB_LOCON("ram_i_down" , IN   , "ram_i_down" );
  GENLIB_LOCON("ram_i_up"   , IN   , "ram_i_up"   );
  /* ACC shifter I/O. */
  GENLIB_LOCON("acc_o_down" ,   OUT, "acc_o_down" );
  GENLIB_LOCON("acc_o_up"   ,   OUT, "acc_scout"   );
  GENLIB_LOCON("acc_i_down" , IN   , "acc_i_down" );
  GENLIB_LOCON("acc_i_up"   , IN   , "acc_i_up"   );

  /* ACCU controls terminals. */
  GENLIB_LOCON(  "fonc", IN   ,  "fonc");
  GENLIB_LOCON(  "test", IN   ,  "test");
  GENLIB_LOCON(  "scin", IN   ,  "scin");
  GENLIB_LOCON(    "ck", IN   ,    "ck");

  /* Data bus terminals. */
  GENLIB_LOCON( "a[3:0]", IN   , "a[3:0]");
  GENLIB_LOCON( "b[3:0]", IN   , "b[3:0]");
  GENLIB_LOCON( "d[3:0]", IN   , "d[3:0]");
  GENLIB_LOCON( "i[8:0]", IN   , "i[8:0]");
  GENLIB_LOCON( "y[3:0]", OUT  , "y[3:0]");

  GENLIB_LOCON( "noe", IN, "noe");
  GENLIB_LOCON( "oe", OUT, "oe");

  /* Power suplies terminals. */
  GENLIB_LOCON("vdd", IN   ,"vdd");
  GENLIB_LOCON("vss", IN   ,"vss");


  /* **************** Data-Path Instantiation ***************** */


  GENLIB_LOINSE("amd2901_dpt", "amd2901_dpt",
	 /* ck */
	 "ram_ck[0] => ck",
	 "ram_ck[1] => ck",
	 "ram_ck[2] => ck",
	 "ram_ck[3] => ck",
	 "ram_ck[4] => ck",
	 "ram_ck[5] => ck",
	 "ram_ck[6] => ck",
	 "ram_ck[7] => ck",
	 "ram_ck[8] => ck",
	 "ram_ck[9] => ck",
	 "ram_ck[10] => ck",
	 "ram_ck[11] => ck",
	 "ram_ck[12] => ck",
	 "ram_ck[13] => ck",
	 "ram_ck[14] => ck",
	 "ram_ck[15] => ck",
	 "ops_mx[2:0] => ops_mx[2:0]",
	 "opr_mx[1:0] => opr_mx[1:0]",
	 "alu_k[4:0] => alu_k[4:0]",
	 "alu_cin    => cin",         /* plot */
	 "alu_cout   => cout",
	 "alu_over   => alu_over",

       "ram_sh[1:0] =>   ram_sh[1:0]",
       "acc_sh[1:0] =>   ram_sh[1:0]",
       "ram_i_up    => ram_i_up",
       "ram_i_down  => ram_i_down",
       "acc_i_up    => acc_i_up",
       "acc_i_down  => acc_i_down",
       "acc_q_down  => acc_o_down",

       "out_mx => out_mx",

       "acc_ck    => ck",
       "acc_wen   => acc_wen",
       "acc_test  => test",      /* plot */
       "acc_scin  => scin",      /* plot */
       "acc_scout => acc_scout",

       "a[15:0] => deca[15:0]",
       "b[15:0] => decb[15:0]",
       "b_w[15:0] => decwb[15:0]",


        "opr_d[3:0] =>      d[3:0]",
        "alu_f[3:0] =>  alu_f[3:0]",
       "alu_np[3:0] => alu_np[3:0]",
       "alu_ng[3:0] => alu_ng[3:0]",
       "out_x[3:0] =>     y[3:0]",

       "vdd => vdd",
       "vss => vss", 0);


  /* ***************** Control Instantiation ****************** */


  GENLIB_LOINSE("amd2901_ctl", "ctl",

       "ops_mx[2:0] => ops_mx[2:0]",
       "opr_mx[1:0] => opr_mx[1:0]",

       "alu_k[4:0] => alu_k[4:0]",
       "alu_cout   => cout",
       "alu_over   => alu_over",

/******************************/
       "deca[15:0] => deca[15:0]",
       "decb[15:0] => decb[15:0]",
       "decwb[15:0] => decwb[15:0]",

       "a[3:0] => a[3:0]",
       "b[3:0] => b[3:0]", // bw == b
	 
/**********************************/
       "ram_sh[1:0] => ram_sh[1:0]",

       "out_mx => out_mx",

       "acc_wen   => acc_wen",

        "alu_f[3:0] =>  alu_f[3:0]",
       "alu_np[3:0] => alu_np[3:0]",
       "alu_ng[3:0] => alu_ng[3:0]",

       "core_test   => test",   /* plot */
       "core_fonc   => fonc",   /* plot */

       "core_np     => np",       /* plot */
       "core_ng     => ng",       /* plot */
       "core_over => over",   /* plot */
       "core_zero => zero",   /* plot */

        "core_sh_right => sh_right",
        "core_sh_left  => sh_left",

       "i[8:0] => i[8:0]",

//       "ram_wri => ram_wri",

       "noe => noe",
       "oe => oe",

       "vdd => vdd",
       "vss => vss", 0);

  GENLIB_PLACE ("amd2901_dpt", "amd2901_dpt", NOSYM, 0, 0);
  GENLIB_DEF_AB (0, 0, 0, 100);
  GENLIB_SAVE_LOFIG();
  GENLIB_SAVE_PHFIG();


  exit(0);
}
