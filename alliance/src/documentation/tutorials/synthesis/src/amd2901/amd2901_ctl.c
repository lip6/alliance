#include  <genlib.h>

#define    POWER               "vdd", "vss", 0
#define    getbit(val,bit)    ( ((val) >> (bit))%2 )

/* + */
char *fastname(char *deb,char indice,char *reste)
{
  static char name[100];
  sprintf(name,"%s%c%s",deb,indice,reste);
  return name;
}
char *fastname1(char *deb,char indice,char *reste)
{
  static char name[100];
  sprintf(name,"%s%c%s",deb,indice,reste);
  return name;
}


/*******************************************************************/

main()
{
  long  i;


  GENLIB_DEF_LOFIG("amd2901_ctl");


  /* ***************** Terminal Declarations ****************** */


  /* Input/Output from and to the data-path. */

  /* Command for selecting operands R and S. */

  GENLIB_LOCON("ops_mx[2:0]",   OUT, "ops_mx[2:0]");
  GENLIB_LOCON("opr_mx[1:0]",   OUT, "opr_mx[1:0]");


  /* ALU commands and auxiliary terminals. */

  GENLIB_LOCON("alu_k[4:0]",   OUT, "alu_k[4:0]");
  GENLIB_LOCON("alu_cout"  , IN   , "alu_cout"  );
  GENLIB_LOCON("alu_over"  , IN   , "alu_over"  );


  /* RAM, ACCU shifter commands and auxiliary terminals.
   * ("acc_sh" is same as "ram_sh")
   */

  GENLIB_LOCON("ram_sh[1:0]",   OUT, "ram_sh[1:0]");

  /* Output multiplexer commnand (for X bus). */

  GENLIB_LOCON("out_mx",   OUT, "out_mx");


  /* ACCU controls terminals.
   * ("acc_ck", "acc_test" and "acc_scin" directly comes from the plots)
   */

  GENLIB_LOCON("acc_wen"  ,   OUT, "acc_wen"  );


  /* Data bus terminals. */

  GENLIB_LOCON( "alu_f[3:0]",  IN,  "alu_f[3:0]");
  GENLIB_LOCON("alu_np[3:0]",  IN, "alu_np[3:0]");
  GENLIB_LOCON("alu_ng[3:0]",  IN, "alu_ng[3:0]");


  /* Input/Output from and to the plots. */

  /* Test terminals from/to plots. */

  GENLIB_LOCON("core_test", IN   , "core_test");
  GENLIB_LOCON("core_fonc", IN   , "core_fonc");


  /* ALU terminals from/to plots. */

  /* GENLIB_LOCON("core_cout",   OUT, "core_cout"); */
  GENLIB_LOCON("core_np"  ,   OUT, "core_np"  );
  GENLIB_LOCON("core_ng"  ,   OUT, "core_ng"  );
  GENLIB_LOCON("core_over",   OUT, "core_over");
  GENLIB_LOCON("core_zero",   OUT, "core_zero");


  /* RAM, ACCU shifter terminals from/to plots.
   * RAM and ACCU I/O plots controls.
   */

  GENLIB_LOCON("core_sh_right"  ,   OUT, "core_sh_right"  );
  GENLIB_LOCON("core_sh_left"   ,   OUT, "core_sh_left"   );


  /* Data bus terminals from/to the plots. */

  GENLIB_LOCON("i[8:0]", IN   , "i[8:0]");

  GENLIB_LOCON("noe", IN   , "noe");
  GENLIB_LOCON("oe", OUT   , "oe");

  /* + */

  GENLIB_LOCON("a[3:0]",   IN, "a[3:0]");
  GENLIB_LOCON("b[3:0]",   IN, "b[3:0]");
//  GENLIB_LOCON("wb[3:0]",   OUT, "wb[3:0]");
  
  GENLIB_LOCON("deca[15:0]",   OUT, "deca[15:0]");
  GENLIB_LOCON("decb[15:0]",   OUT, "decb[15:0]");
  GENLIB_LOCON("decwb[15:0]",   OUT, "decwb[15:0]");

  /* - */

//  GENLIB_LOCON("ram_wri", OUT, "ram_wri");


  /* Power supply connectors. */

  GENLIB_LOCON("vdd", IN   , "vdd");
  GENLIB_LOCON("vss", IN   , "vss");


  /* + */
 
  /* decoders for RAM RA and RB addresses. */
  GENLIB_LOINS ("inv_x4", "inv_a0", "a[0]", "na[0]", "vdd", "vss", NULL);
  GENLIB_LOINS ("inv_x4", "inv_a1", "a[1]", "na[1]", "vdd", "vss", NULL);
  GENLIB_LOINS ("inv_x4", "inv_a2", "a[2]", "na[2]", "vdd", "vss", NULL);
  GENLIB_LOINS ("inv_x4", "inv_a3", "a[3]", "na[3]", "vdd", "vss", NULL);

  GENLIB_LOINS ("inv_x4", "inv_b0", "b[0]", "nb[0]", "vdd", "vss", NULL);
  GENLIB_LOINS ("inv_x4", "inv_b1", "b[1]", "nb[1]", "vdd", "vss", NULL);
  GENLIB_LOINS ("inv_x4", "inv_b2", "b[2]", "nb[2]", "vdd", "vss", NULL);
  GENLIB_LOINS ("inv_x4", "inv_b3", "b[3]", "nb[3]", "vdd", "vss", NULL);

  GENLIB_LOINS ("inv_x4", "inv_wen", "ram_wri", "ram_nwen", "vdd", "vss", NULL);
 for (i = 0; i < 16; i++)
  {
    GENLIB_LOINS ("na4_x1", GENLIB_NAME ("na4_ram_adrb_%d_0", i),
                  GENLIB_NAME ("%s[3]", (getbit (i, 3) ? "b" : "nb")),
                  GENLIB_NAME ("%s[2]", (getbit (i, 2) ? "b" : "nb")),
                  GENLIB_NAME ("%s[1]", (getbit (i, 1) ? "b" : "nb")),
                  GENLIB_NAME ("%s[0]", (getbit (i, 0) ? "b" : "nb")),
                  GENLIB_NAME ("ram_adrb_%d",i),
                  "vdd", "vss", NULL);

    GENLIB_LOINS ("inv_x2", GENLIB_NAME ("n1_ram_adrb_%d_1",i),
                  GENLIB_NAME ("ram_adrb_%d", i),
                  GENLIB_ELM  ("decb", i),
                  "vdd", "vss", NULL);
    
    GENLIB_LOINS ("no2_x1", GENLIB_NAME ("no2_ram_adri_%d_0",i),
                  GENLIB_NAME ("ram_adrb_%d", i),
                               "ram_nwen",
                  GENLIB_ELM  ("decwb", i),
                  "vdd", "vss", NULL);

    GENLIB_LOINS ("a4_x2", GENLIB_NAME ("a4_ram_adra_%d_0",i),
                  GENLIB_NAME("%s[3]", (getbit (i, 3) ? "a" : "na")),
                  GENLIB_NAME("%s[2]", (getbit (i, 2) ? "a" : "na")),
                  GENLIB_NAME("%s[1]", (getbit (i, 1) ? "a" : "na")),
                  GENLIB_NAME("%s[0]", (getbit (i, 0) ? "a" : "na")),
                  GENLIB_ELM("deca", i),
                  "vdd", "vss", NULL);
  }

  /* - */

  /* ***************** S multiplexer control ****************** */


  GENLIB_LOINS("inv_x1", "n1_i2", "i[2]", "ni[2]", POWER);
  GENLIB_LOINS("inv_x1", "n1_i1", "i[1]", "ni[1]", POWER);
  GENLIB_LOINS("inv_x1", "n1_i0", "i[0]", "ni[0]", POWER);

  GENLIB_LOINS("no2_x1", "no2_ops_mx0",  "i[2]", "ni[0]", "ops_mx[0]", POWER);
  GENLIB_LOINS("no2_x1", "no2_ops_mx1", "ni[2]",  "i[1]", "ops_mx[1]", POWER);
  GENLIB_LOINS( "a3_x2",  "a3_ops_mx2",  "i[2]",  "i[1]", "i[0]", "ops_mx[2]", POWER);


  /* ***************** R multiplexer control ****************** */


  GENLIB_LOINS( "o2_x2",  "o2_opr_mx0"  ,  "i[2]",  "i[1]", "opr_mx[0]", POWER);
  GENLIB_LOINS("na2_x1", "na2_opr_mx1_0", "ni[2]",  "i[1]", "opr_mx1_0", POWER);
  GENLIB_LOINS("na3_x1", "na3_opr_mx1_1",  "i[2]", "ni[1]", "ni[0]","opr_mx1_1", POWER);
  GENLIB_LOINS("na2_x1", "na2_opr_mx1_2", "opr_mx1_0","opr_mx1_1","opr_mx[1]", POWER);


  /* ***************** X multiplexer control ****************** */

  GENLIB_LOINS("inv_x1", "n1_i3", "i[7]", "ni[3]", POWER); 	
  GENLIB_LOINS("no3_x1", "no3_out_mx0", "i[8]", "ni[3]", "i[6]", "out_mx", POWER);


  /* ********************** ALU control *********************** */


  /* ALU commands. */
  GENLIB_LOINS("xr2_x1", "xr2_alu_k0",  "i[5]",  "i[3]", "alu_k[0]", POWER);
  GENLIB_LOINS("xr2_x1", "xr2_alu_k1",  "i[5]",  "i[4]", "alu_k[1]", POWER);
  GENLIB_LOINS( "inv_x1",      "n1_i4",  "i[4]", "ni[4]",             POWER);
  GENLIB_LOINS( "a2_x2",  "a2_alu_k2",  "i[5]", "ni[4]", "alu_k[2]", POWER);
  GENLIB_LOINS( "inv_x1",      "n1_i5",  "i[5]", "ni[5]",             POWER);
  GENLIB_LOINS( "a3_x2",  "a3_alu_k3", "ni[5]","i[4]","i[3]","alu_k[3]", POWER);
  GENLIB_LOINS( "a2_x2",  "a2_alu_k4",     "i[4]",  "i[3]", "alu_k4_0", POWER);
  GENLIB_LOINS( "o2_x2",  "o2_alu_k4", "alu_k4_0",  "i[5]", "alu_k[4]", POWER);

  /* Compute of ALU flags.
   * Propagate.
   */

  GENLIB_LOINS("no2_x1", "no2_alu_p_0", "alu_np[0]", "alu_np[1]", "alu_p_0", POWER);
  GENLIB_LOINS("no2_x1", "no2_alu_p_1", "alu_np[2]", "alu_np[3]", "alu_p_1", POWER);
  GENLIB_LOINS("na2_x1", "na2_alu_p", "alu_p_0", "alu_p_1", "core_np", POWER);

  /* Generate. */
  GENLIB_LOINS("no2_x1", "no2_alu_g_0", "alu_np[1]", "alu_ng[0]", "alu_g_0", POWER);
  GENLIB_LOINS("no2_x1", "no2_alu_g_1", "alu_np[3]", "alu_np[2]", "alu_g_1", POWER);
  GENLIB_LOINS( "inv_x1",  "n1_alu_g_2", "alu_ng[1]",              "alu_g_2", POWER);

  GENLIB_LOINS("noa22_x1", "noa3_alu_g_3", "alu_np[3]","alu_ng[2]","alu_ng[3]","alu_g_3"  , POWER);
  GENLIB_LOINS(  "inv_x1",   "n1_alu_g_4", "alu_g_3"  , "alu_g_4", POWER);

  GENLIB_LOINS("na2_x1", "na2_alu_g_5", "alu_g_0", "alu_g_1", "alu_g_5", POWER);
  GENLIB_LOINS("na2_x1", "na2_alu_g_6", "alu_g_1", "alu_g_2", "alu_g_6", POWER);

  GENLIB_LOINS("a3_x2", "na3_alu_g_7", "alu_g_4",
                  "alu_g_5",
                  "alu_g_6",
                  "core_ng" , POWER);

  /* Zero and overflow */

  GENLIB_LOINS("no2_x1", "no2_alu_zero_0", "alu_f[0]", "alu_f[1]", "alu_zero_0", POWER);
  GENLIB_LOINS("no2_x1", "no2_alu_zero_1", "alu_f[2]", "alu_f[3]", "alu_zero_1", POWER);
  GENLIB_LOINS("a2_x2", "a2_alu_zero", "alu_zero_0", "alu_zero_1", "core_zero", POWER);

  GENLIB_LOINS("xr2_x1", "nxr2_alu_nover", "alu_over",
                    "alu_cout",   "core_over", POWER);


  /* ********************* ACCU control *********************** */


  GENLIB_LOINS("inv_x1", "n1_i6", "i[8]", "ni[6]", POWER);

  /* Compute of ACCU write enable. */
  GENLIB_LOINS("noa22_x1", "nao3_acc_wen", "ni[6]",
                   "i[7]",
                   "i[6]",
                 "acc_wen"   , POWER);


  /* ********************** RAM control *********************** */


  /* ACCU and RAM shift multiplexer control. */
  GENLIB_LOINS("na2_x1", "na2_ram_sh0", "i[8]", "i[7]", "ram_sh[0]", POWER);
  GENLIB_LOINS( "inv_x1",  "n1_ram_sh1", "i[8]",         "ram_sh[1]", POWER);

  /* RAM and ACCU I/O plots controls. */
  GENLIB_LOINS("a2_x2","a2_core_sh_left" ,"i[8]", "i[7]","core_sh_left" ,POWER);
  GENLIB_LOINS("a2_x2","a2_core_sh_right","i[8]","ni[3]","core_sh_right",POWER);

  /* Compute of RAM write enable. */
  GENLIB_LOINS(  "inv_x1",  "n1_ram_nwri_0", "core_fonc", "core_nfonc", POWER);
  GENLIB_LOINS( "no2_x1", "no2_ram_nwri_1", "core_test", "core_nfonc",
                           "fonc_mode" , POWER);
  GENLIB_LOINS("noa22_x1", "noa3_ram_nwri_2",  "i[8]",
                    "i[7]",
                "fonc_mode"   ,
                   "ram_nwri"   , POWER);
  GENLIB_LOINS("inv_x1","inv_ram_wri","ram_nwri","ram_wri", POWER );

  GENLIB_LOINS( "inv_x1","inv_noe","noe","oe", POWER);

  GENLIB_SAVE_LOFIG();

  exit(0);
}


