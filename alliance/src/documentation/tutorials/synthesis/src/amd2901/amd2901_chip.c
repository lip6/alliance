#include <genlib.h>
#define POWER "vdde","vdd","vsse","vss",0

int main ()
 {
  int i;

  GENLIB_DEF_LOFIG("amd2901_chip");
 

  GENLIB_LOCON("ck", IN     ,"ck");

  GENLIB_LOCON(  "cin", UNKNOWN,  "cin");
  GENLIB_LOCON( "cout", UNKNOWN, "cout");
  GENLIB_LOCON(   "np",   OUT  ,   "np");
  GENLIB_LOCON(   "ng",   OUT  ,   "ng");
  GENLIB_LOCON(  "ovr",   OUT  ,  "ovr");
  GENLIB_LOCON( "zero",   OUT  , "zero");
  GENLIB_LOCON("signe", UNKNOWN,"signe");


  GENLIB_LOCON("r0", UNKNOWN, "r0");
  GENLIB_LOCON("r3", UNKNOWN, "r3");
  GENLIB_LOCON("q0", UNKNOWN, "q0");
  GENLIB_LOCON("q3", UNKNOWN, "q3");

  GENLIB_LOCON( "fonc", IN     , "fonc");
  GENLIB_LOCON( "test", IN     , "test");
  GENLIB_LOCON( "scin", IN     , "scin");
  GENLIB_LOCON("scout",   OUT  ,"scout");

  GENLIB_LOCON("a[3:0]", IN     , "a[3:0]");
  GENLIB_LOCON("b[3:0]", IN     , "b[3:0]");
  GENLIB_LOCON("d[3:0]", IN     , "d[3:0]");
  GENLIB_LOCON("i[8:0]", IN     , "i[8:0]");
  GENLIB_LOCON("noe"   , IN     , "noe"   );
  GENLIB_LOCON("y[3:0]", UNKNOWN, "y[3:0]");

  GENLIB_LOCON("vdd" , IN     , "vdd" );
  GENLIB_LOCON("vss" , IN     , "vss" );
  GENLIB_LOCON("vdde", IN     , "vdde");
  GENLIB_LOCON("vsse", IN     , "vsse");
 
  GENLIB_LOINSE ( "amd2901_core", "core",
	"cin => cin_i",
	"cout => cout_i",
	"np => np_i",
	"ng => ng_i",
	"over => ovr_i",
	"zero => zero_i",

	"sh_right => sh_right",
	"sh_left => sh_left",
	"ram_o_down => ram_o_down",
	"ram_o_up   => ram_o_up",
	"ram_i_down => ram_i_down",
	"ram_i_up   => ram_i_up",

	"acc_o_down => acc_o_down",
	"acc_o_up   => acc_o_up",
	"acc_i_down => acc_i_down",
	"acc_i_up   => acc_i_up",

	"fonc => fonc_i",
	"test => test_i",
	"scin => scin_i",
  	"ck => ckc",

	"a[3:0] => a_i[3:0]",
	"b[3:0] => b_i[3:0]",
	"d[3:0] => d_i[3:0]",
	"i[8:0] => i_i[8:0]",

	"y[3:0] => y_i[3:0]",

	"noe => noe_i",
	"oe => oe",

	"vdd => vdd",
	"vss => vss",
	0);

  GENLIB_LOINS("pck_sp","p_ck","ck","cki", POWER);

  GENLIB_LOINS("pi_sp","p_fonc","fonc","fonc_i","cki", POWER );
  GENLIB_LOINS("pi_sp","p_test","test","test_i","cki", POWER );
  GENLIB_LOINS("pi_sp","p_scin","scin","scin_i","cki", POWER );
  GENLIB_LOINS("pi_sp","p_cin","cin","cin_i","cki", POWER );
  GENLIB_LOINS("pi_sp","p_noe","noe","noe_i","cki", POWER );

  for (i=0;i<4;i++)
   {
     GENLIB_LOINS("pi_sp",GENLIB_NAME("p_a%d",i), GENLIB_ELM("a",i), GENLIB_ELM("a_i",i), "cki", POWER );
     GENLIB_LOINS("pi_sp",GENLIB_NAME("p_b%d",i), GENLIB_ELM("b",i), GENLIB_ELM("b_i",i), "cki", POWER );
     GENLIB_LOINS("pi_sp",GENLIB_NAME("p_d%d",i), GENLIB_ELM("d",i), GENLIB_ELM("d_i",i), "cki", POWER );
   }

  for (i=0;i<9;i++)
    GENLIB_LOINS("pi_sp",GENLIB_NAME("p_i%d",i), GENLIB_ELM("i",i), GENLIB_ELM("i_i",i), "cki", POWER );

  GENLIB_LOINS("po_sp","p_cout","cout_i","cout","cki", POWER );
  GENLIB_LOINS("po_sp","p_np","np_i","np","cki", POWER );
  GENLIB_LOINS("po_sp","p_ng","ng_i","ng","cki", POWER );
  GENLIB_LOINS("po_sp","p_ovr","ovr_i","ovr","cki", POWER );
  GENLIB_LOINS("po_sp","p_zero","zero_i","zero","cki", POWER );
  GENLIB_LOINS("po_sp","p_signe","ram_o_up","signe","cki", POWER );
  GENLIB_LOINS("po_sp","p_scout","acc_o_up","scout","cki", POWER );

  for (i=0;i<4;i++)
    GENLIB_LOINS ("pot_sp",GENLIB_NAME("p_y%d",i),
	GENLIB_ELM("y_i",i), "oe", GENLIB_ELM("y",i), "cki", POWER );

  GENLIB_LOINS ("piot_sp","p_q0",
	"acc_o_down","sh_right","acc_i_down","q0","cki", POWER );
  GENLIB_LOINS ("piot_sp","p_q3",
	"acc_o_up","sh_left","acc_i_up","q3","cki", POWER );
  GENLIB_LOINS ("piot_sp","p_r0",
	"ram_o_down","sh_right","ram_i_down","r0","cki", POWER );
  GENLIB_LOINS ("piot_sp","p_r3",
	"ram_o_up","sh_left","ram_i_up","r3","cki", POWER );

  GENLIB_LOINS("pvddick_sp","p_vddick0","ckc","cki","vdde","vdd","vsse","vss",0);
  GENLIB_LOINS("pvssick_sp","p_vssick0","ckc","cki","vdde","vdd","vsse","vss",0);

  GENLIB_LOINS("pvddeck_sp","p_vddeck0","ckc","cki","vdde","vdd","vsse","vss",0);
  GENLIB_LOINS("pvddeck_sp","p_vddeck1","ckc","cki","vdde","vdd","vsse","vss",0);
  GENLIB_LOINS("pvsseck_sp","p_vsseck0","ckc","cki","vdde","vdd","vsse","vss",0);
  GENLIB_LOINS("pvsseck_sp","p_vsseck1","ckc","cki","vdde","vdd","vsse","vss",0);

  GENLIB_SAVE_LOFIG();

  exit (0);

 }
