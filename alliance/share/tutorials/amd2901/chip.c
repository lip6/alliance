# include <genlib.h>
main ()
{
DEF_LOFIG ("alu");
/* declaration of the cell's external connectors */
LOCON ("R[3:0]",'I',"r[3:0]");
LOCON ("S[3:0]",'I',"s[3:0]");
LOCON ("ALU_OUT[3:0]",'O',"alu_out[3:0]");
LOCON ("CIN",'I',"cin");
LOCON ("COUT",'O',"cout");
LOCON ("NP",'O',"np");
LOCON ("NG",'O',"ng");
LOCON ("SIGNE",'O',"signe");
LOCON ("ZERO",'O',"zero");
LOCON ("OVR",'O',"ovr");
LOCON ("I[5:3]",'I',"i[5:3]");
LOCON ("VDD",'I',"vdd");
LOCON ("VSS",'I',"vss");

/* bit slice 0 */ 

LOINS ("xr2_y","xor0_A0","r[0]","n0","x00","vdd","vss",0);
LOINS ("xr2_y","xor1_A0","s[0]","n1","x10","vdd","vss",0);
LOINS ("na2_y","nand0_A0","x00","x10","gb0","vdd","vss",0);
LOINS ("no2_y","nor1_A0","x00","x10","pb0","vdd","vss",0);
LOINS ("na2_y","nand1_A0","gb0","na20","cout0","vdd","vss",0);
LOINS ("n1_y","not_A0","pb0","not0","vdd","vss",0);
LOINS ("na2_y","nand2_A0","not0","cin","na20","vdd","vss",0);
LOINS ("no2_y","nor2_A0","n2","gb0","no20","vdd","vss",0);
LOINS ("no2_y","nor3_A0","n3","pb0","no30","vdd","vss",0);
LOINS ("no2_y","nor4_A0","n4","cin","no40","vdd","vss",0);
LOINS ("xr2_y","xor2_A0","no20","no30","x20","vdd","vss",0);
LOINS ("xr2_y","xor3_A0","x20","no40","fb0","vdd","vss",0);

/* bit slice 1 */

LOINS ("xr2_y","xor0_A1","r[1]","n0","x01","vdd","vss",0);
LOINS ("xr2_y","xor1_A1","s[1]","n1","x11","vdd","vss",0);
LOINS ("na2_y","nand0_A1","x01","x11","gb1","vdd","vss",0);
LOINS ("no2_y","nor1_A1","x01","x11","pb1","vdd","vss",0);
LOINS ("na2_y","nand1_A1","gb1","na21","cout1","vdd","vss",0);
LOINS ("n1_y","not_A1","pb1","not1","vdd","vss",0);
LOINS ("na2_y","nand2_A1","not1","cout0","na21","vdd","vss",0);
LOINS ("no2_y","nor2_A1","n2","gb1","no21","vdd","vss",0);
LOINS ("no2_y","nor3_A1","n3","pb1","no31","vdd","vss",0);
LOINS ("no2_y","nor4_A1","n4","cout0","no41","vdd","vss",0);
LOINS ("xr2_y","xor2_A1","no21","no31","x21","vdd","vss",0);
LOINS ("xr2_y","xor3_A1","x21","no41","fb1","vdd","vss",0);

/* bit slice 2 */

LOINS ("xr2_y","xor0_A2","r[2]","n0","x02","vdd","vss",0);
LOINS ("xr2_y","xor1_A2","s[2]","n1","x12","vdd","vss",0);
LOINS ("na2_y","nand0_A2","x02","x12","gb2","vdd","vss",0);
LOINS ("no2_y","nor1_A2","x02","x12","pb2","vdd","vss",0);
LOINS ("na2_y","nand1_A2","gb2","na22","cout2","vdd","vss",0);
LOINS ("n1_y","not_A2","pb2","not2","vdd","vss",0);
LOINS ("na2_y","nand2_A2","not2","cout1","na22","vdd","vss",0);
LOINS ("no2_y","nor2_A2","n2","gb2","no22","vdd","vss",0);
LOINS ("no2_y","nor3_A2","n3","pb2","no32","vdd","vss",0);
LOINS ("no2_y","nor4_A2","n4","cout1","no42","vdd","vss",0);
LOINS ("xr2_y","xor2_A2","no22","no32","x22","vdd","vss",0);
LOINS ("xr2_y","xor3_A2","x22","no42","fb2","vdd","vss",0);

/* bit slice 3 */

LOINS ("xr2_y","xor0_A3","r[3]","n0","x03","vdd","vss",0);
LOINS ("xr2_y","xor1_A3","s[3]","n1","x13","vdd","vss",0);
LOINS ("na2_y","nand0_A3","x03","x13","gb3","vdd","vss",0);
LOINS ("no2_y","nor1_A3","x03","x13","pb3","vdd","vss",0);
LOINS ("na2_y","nand1_A3","gb3","na23","couta","vdd","vss",0);
LOINS ("n1_y","not_A3","pb3","not3","vdd","vss",0);
LOINS ("na2_y","nand2_A3","not3","cout2","na23","vdd","vss",0);
LOINS ("no2_y","nor2_A3","n2","gb3","no23","vdd","vss",0);
LOINS ("no2_y","nor3_A3","n3","pb3","no33","vdd","vss",0);
LOINS ("no2_y","nor4_A3","n4","cout2","no43","vdd","vss",0);
LOINS ("xr2_y","xor2_A3","no23","no33","x23","vdd","vss",0);
LOINS ("xr2_y","xor3_A3","x23","no43","signea","vdd","vss",0);

/* microinsruction decoder */

LOINS ("n1_y","not5_D","i[5]","ni5","vdd","vss",0);
LOINS ("na3_y","nand3_D","i[3]","i[4]","ni5","nn3","vdd","vss",0);
LOINS ("no2_y","nor_D","i[4]","ni5","n2","vdd","vss",0);
LOINS ("nxr2_y","nxor4_D","i[4]","i[5]","nn1","vdd","vss",0);
LOINS ("nxr2_y","nxor3_D","i[3]","i[5]","nn0","vdd","vss",0);
LOINS ("na2_y","nand_D","ni5","nn3","n4","vdd","vss",0);
LOINS ("n1_y","not0_D","nn3","n3","vdd","vss",0);
LOINS ("np1_y","notp4_D","nn1","n1","vdd","vss",0);
LOINS ("np1_y","notp3_D","nn0","n0","vdd","vss",0);

/* output cicuitery */
/* "flag" command for signals used only in case of arithmetic operations */

LOINS ("na2_y","naflag1","i[4]","i[3]","flag1","vdd","vss",0);
LOINS ("a2_y","naflag","ni5","flag1","flag","vdd","vss",0);

/* generation of prop and gen) */

LOINS ("n1_y","not0_csh","pb0","npb0","vdd","vss",0); 
LOINS ("n1_y","not1_csh","gb0","ngb0","vdd","vss",0); 
LOINS ("n1_y","not2_csh","gb3","ngb3","vdd","vss",0);
LOINS ("n1_y","npf","nprop","propf","vdd","vss",0);
LOINS ("n1_y","npg","ngen","genf","vdd","vss",0);
LOINS ("a2_y","npflag","propf","flag","p","vdd","vss",0); 
LOINS ("a2_y","ngflag","genf","flag","g","vdd","vss",0); 

LOINS ("no3_y","nor0_csh","pb3","pb2","pb1","no30_csh","vdd","vss",0);
LOINS ("no3_y","nor1_csh","gb1","pb2","pb3","no31_csh","vdd","vss",0);
LOINS ("no3_y","nor2_csh","no2_csh","no31_csh","ngb3","no32_csh","vdd","vss",0);

LOINS ("na2_y","nand0_csh","npb0","no30_csh","nprop","vdd","vss",0);
LOINS ("na2_y","nand1_csh","ngb0","no30_csh","na_csh","vdd","vss",0);

LOINS ("no2_y","nor_csh","gb2","pb3","no2_csh","vdd","vss",0);
LOINS ("a2_y","and_csh","na_csh","no32_csh","ngen","vdd","vss",0);

/* others flags */

LOINS ("na2_y","nand0_csb","fb0","fb1","na0_csb","vdd","vss",0);
LOINS ("na2_y","nand1_csb","fb2","signea","na1_csb","vdd","vss",0);
LOINS ("no2_y","nor_csb","na0_csb","na1_csb","zero","vdd","vss",0);
LOINS ("xr2_y","xor_csb","cout2","couta","ovr","vdd","vss",0);

/* inversion of signals in conformity with the data sheet */

LOINS ("n1_y","not0_csb","fb0","alu_out[0]","vdd","vss",0);
LOINS ("n1_y","not1_csb","fb1","alu_out[1]","vdd","vss",0);
LOINS ("n1_y","not2_csb","fb2","alu_out[2]","vdd","vss",0);
LOINS ("n1_y","not3_csb","signea","alu_out[3]","vdd","vss",0);

LOINS ("n1_y","not4_csb","signea","signe","vdd","vss",0);
LOINS ("n1_y","not0_p","p","np","vdd","vss",0);
LOINS ("n1_y","not0_g","g","ng","vdd","vss",0);
LOINS ("a2_y","coutflag","couta","flag","cout","vdd","vss",0);

SAVE_LOFIG();

DEF_LOFIG ("accu");
/* declaration of the cell's external connectors */
LOCON ("ALU_OUT[3:0]",'I',"alu_out[3:0]");
LOCON ("Q0",'I',"q0");
LOCON ("Q3",'I',"q3");
LOCON ("FONC",'I',"fonc");
LOCON ("TEST",'I',"test");
LOCON ("FONC_MODE",'X',"fonc_mode");
LOCON ("SCIN",'I',"scin");
LOCON ("SCOUT",'O',"scout");
LOCON ("F0",'O',"f0");
LOCON ("F3",'O',"f3");
LOCON ("DECALD",'O',"decald");
LOCON ("DECALG",'O',"decalg");
LOCON ("Q[3:0]",'X',"q[3:0]");
LOCON ("I[8:6]",'I',"i[8:6]");
LOCON ("CK",'I',"ck");
LOCON ("VDD",'I',"vdd");
LOCON ("VSS",'I',"vss");

/* generation of the multiplexing and shifting commands */

LOINS ("n1_y","n11sa","i[8]","decaln","vdd","vss",0);
LOINS ("n1_y","n12sa","i[7]","ni7","vdd","vss",0);
LOINS ("n1_y","n13sa","i[6]","ni6","vdd","vss",0);
LOINS ("n1_y","n14sa","fonc","n14s","vdd","vss",0);
LOINS ("n1_y","n15sa","test","n15s","vdd","vss",0);
LOINS ("b1_y","BDECALD","decalda","decald","vdd","vss",0);
LOINS ("b1_y","BDECALG","decalga","decalg","vdd","vss",0);

LOINS ("a2_y","a21sa","i[8]","ni7","decalda","vdd","vss",0);
LOINS ("a2_y","a22sa","i[8]","i[7]","decalga","vdd","vss",0);
LOINS ("a3_y","a31sa","decaln","ni7","ni6","selalu","vdd","vss",0);
LOINS ("o2_y","o21sa","i[8]","ni7","o21s","vdd","vss",0);
LOINS ("a2_y","a24sa","fonc","n15s","fonc_mode","vdd","vss",0);
LOINS ("a2_y","a25sa","test","n14s","test_mode","vdd","vss",0);
LOINS ("a2_y","a26sa","o21s","ni6","waccu","vdd","vss",0);

/* taking in account of the above commands */

LOINS ("a2_y","a27sa","alu_out[3]","selalu","a27s","vdd","vss",0);
LOINS ("a2_y","a28sa","alu_out[2]","selalu","a28s","vdd","vss",0);
LOINS ("a2_y","a29sa","alu_out[1]","selalu","a29s","vdd","vss",0);
LOINS ("a2_y","a210sa","alu_out[0]","selalu","a210s","vdd","vss",0);

LOINS ("a2_y","a211sa","q[3]","i[8]","a211s","vdd","vss",0);
LOINS ("a2_y","a212sa","q[2]","i[8]","a212s","vdd","vss",0);
LOINS ("a2_y","a213sa","q[1]","i[8]","a213s","vdd","vss",0);
LOINS ("a2_y","a214sa","q[0]","i[8]","a214s","vdd","vss",0);

LOINS ("o2_y","o22sa","a27s","a211s","insh3","vdd","vss",0);
LOINS ("o2_y","o23sa","a28s","a212s","insh2","vdd","vss",0);
LOINS ("o2_y","o24sa","a29s","a213s","insh1","vdd","vss",0);
LOINS ("o2_y","o25sa","a210s","a214s","insh0","vdd","vss",0);


LOINS ("a2_y","a215sa","decalga","insh3","f3","vdd","vss",0);
LOINS ("a2_y","a228sa","decalda","insh0","f0","vdd","vss",0);


/* shifter */

/* bit slice 3 */
LOINS ("a2_y","a216sa","q3","decalda","a216s","vdd","vss",0);
LOINS ("a2_y","a217sa","insh3","decaln","a217s","vdd","vss",0);
LOINS ("a2_y","a218sa","insh2","decalga","a218s","vdd","vss",0);

/* bit slice 2 */
LOINS ("a2_y","a219sa","insh3","decalda","a219s","vdd","vss",0);
LOINS ("a2_y","a220sa","insh2","decaln","a220s","vdd","vss",0);
LOINS ("a2_y","a221sa","insh1","decalga","a221s","vdd","vss",0);

/* bit slice 1 */
LOINS ("a2_y","a222sa","insh2","decalda","a222s","vdd","vss",0);
LOINS ("a2_y","a223sa","insh1","decaln","a223s","vdd","vss",0);
LOINS ("a2_y","a224sa","insh0","decalga","a224s","vdd","vss",0);

/* bit slice 0 */
LOINS ("a2_y","a225sa","insh1","decalda","a225s","vdd","vss",0);
LOINS ("a2_y","a226sa","insh0","decaln","a226s","vdd","vss",0);
LOINS ("a2_y","a227sa","q0","decalga","a227s","vdd","vss",0);


/* register input */


LOINS ("o3_y","o31sa","a216s","a217s","a218s","shacc3","vdd","vss",0);
LOINS ("o3_y","o32sa","a219s","a220s","a221s","shacc2","vdd","vss",0);
LOINS ("o3_y","o33sa","a222s","a223s","a224s","shacc1","vdd","vss",0);
LOINS ("o3_y","o34sa","a225s","a226s","a227s","shacc0","vdd","vss",0);


/* writing in the register */

LOINS ("a2_y","a229sa","ck","w","ckin","vdd","vss",0);
LOINS ("a2_y","a230sa","test_mode","q[3]","scout","vdd","vss",0);
LOINS ("a2_y","a231sa","fonc_mode","waccu","a231s","vdd","vss",0);
LOINS ("o2_y","o26sa","test_mode","a231s","w","vdd","vss",0);


LOINS ("ms2dp2_y","m3","shacc3","q[2]","test_mode","ckin","q[3]","vdd","vss",0);
LOINS ("ms2dp2_y","m2","shacc2","q[1]","test_mode","ckin","q[2]","vdd","vss",0);
LOINS ("ms2dp2_y","m1","shacc1","q[0]","test_mode","ckin","q[1]","vdd","vss",0);
LOINS ("ms2dp2_y","m0","shacc0","scin","test_mode","ckin","q[0]","vdd","vss",0);

SAVE_LOFIG();

DEF_LOFIG ("muxe");
/* declaration of the cell's external connectors */
LOCON ("RA[3:0]",'I',"ra[3:0]");
LOCON ("RB[3:0]",'I',"rb[3:0]");
LOCON ("Q[3:0]",'I',"q[3:0]");
LOCON ("D[3:0]",'I',"d[3:0]");
LOCON ("R[3:0]",'O',"r[3:0]");
LOCON ("S[3:0]",'O',"s[3:0]");
LOCON ("I[2:0]",'I',"i[2:0]");
LOCON ("VDD",'I',"vdd");
LOCON ("VSS",'I',"vss");

/* generation of the multiplexing commands */

LOINS ("n1_y","n11me","i[2]","ni2","vdd","vss",0);
LOINS ("n1_y","n12me","i[1]","ni1","vdd","vss",0);
LOINS ("n1_y","n13me","i[0]","ni0","vdd","vss",0);

/* ra ->  ALU's S-input */

LOINS ("a2_y","a21me","ni1","i[2]","selas","vdd","vss",0);

/* rb ->  ALU's S-input */

LOINS ("a2_y","a22me","ni2","i[0]","selbs","vdd","vss",0);
LOINS ("o2_y","o21me","i[1]","ni2","o21s","vdd","vss",0);

/* ra ->  ALU's R-input */

LOINS ("a2_y","a23me","ni2","ni1","selar","vdd","vss",0);
LOINS ("o2_y","o22me","i[1]","i[0]","o22s","vdd","vss",0);

/* q ->  ALU's R-input */

LOINS ("a2_y","a24me","o21s","ni0","selqs","vdd","vss",0);

/* d ->  ALU's R-input */

LOINS ("a2_y","a25me","o22s","i[2]","seldr","vdd","vss",0);

/* taking in account of the above commands */

/* rb(3 downto 0) -> s(3 downto 0) */

LOINS ("a2_y","a26ms","rb[0]","selbs","srb0","vdd","vss",0);
LOINS ("a2_y","a27ms","rb[1]","selbs","srb1","vdd","vss",0);
LOINS ("a2_y","a28ms","rb[2]","selbs","srb2","vdd","vss",0);
LOINS ("a2_y","a29ms","rb[3]","selbs","srb3","vdd","vss",0);

/* ra(3 downto 0) -> s(3 downto 0) */

LOINS ("a2_y","a210ms","ra[0]","selas","sra0","vdd","vss",0);
LOINS ("a2_y","a211ms","ra[1]","selas","sra1","vdd","vss",0);
LOINS ("a2_y","a212ms","ra[2]","selas","sra2","vdd","vss",0);
LOINS ("a2_y","a213ms","ra[3]","selas","sra3","vdd","vss",0);

/* ra(3 downto 0) -> r(3 downto 0) */

LOINS ("a2_y","a214ms","ra[0]","selar","ssa0","vdd","vss",0);
LOINS ("a2_y","a215ms","ra[1]","selar","ssa1","vdd","vss",0);
LOINS ("a2_y","a216ms","ra[2]","selar","ssa2","vdd","vss",0);
LOINS ("a2_y","a217ms","ra[3]","selar","ssa3","vdd","vss",0);

/* q(3 downto 0) -> r(3 downto 0) */

LOINS ("a2_y","a218ms","q[0]","selqs","srq0","vdd","vss",0);
LOINS ("a2_y","a219ms","q[1]","selqs","srq1","vdd","vss",0);
LOINS ("a2_y","a220ms","q[2]","selqs","srq2","vdd","vss",0);
LOINS ("a2_y","a221ms","q[3]","selqs","srq3","vdd","vss",0);

/* d(3 downto 0) -> r(3 downto 0) */

LOINS ("a2_y","a222ms","d[0]","seldr","ssd0","vdd","vss",0);
LOINS ("a2_y","a223ms","d[1]","seldr","ssd1","vdd","vss",0);
LOINS ("a2_y","a224ms","d[2]","seldr","ssd2","vdd","vss",0);
LOINS ("a2_y","a225ms","d[3]","seldr","ssd3","vdd","vss",0);

/* last stage of the multiplexer : r and s ALU-inputs */

LOINS ("o3_y","o31ms","srb0","sra0","srq0","s[0]","vdd","vss",0);
LOINS ("o3_y","o32ms","srb1","sra1","srq1","s[1]","vdd","vss",0);
LOINS ("o3_y","o33ms","srb2","sra2","srq2","s[2]","vdd","vss",0);
LOINS ("o3_y","o34ms","srb3","sra3","srq3","s[3]","vdd","vss",0);

LOINS ("o2_y","o23ms","ssa0","ssd0","r[0]","vdd","vss",0);
LOINS ("o2_y","o24ms","ssa1","ssd1","r[1]","vdd","vss",0);
LOINS ("o2_y","o25ms","ssa2","ssd2","r[2]","vdd","vss",0);
LOINS ("o2_y","o26ms","ssa3","ssd3","r[3]","vdd","vss",0);

SAVE_LOFIG();

DEF_LOFIG ("muxout");

/* declaration of the cell's external connectors */

LOCON ("RA[3:0]",'I',"ra[3:0]");
LOCON ("ALU_OUT[3:0]",'I',"alu_out[3:0]");
LOCON ("NOE",'I',"noe");
LOCON ("OE",'O',"oe");
LOCON ("Y[3:0]",'O',"y[3:0]");
LOCON ("I[8:6]",'I',"i[8:6]");
LOCON ("VDD",'I',"vdd");
LOCON ("VSS",'I',"vss");

/* generation of the multiplexing commands */

LOINS ("n1_y","n11ms","i[8]","ni8","vdd","vss",0);
LOINS ("n1_y","n12ms","i[6]","ni6","vdd","vss",0);
/* ra -> y */ 
LOINS ("a3_y","a31ms","i[7]","ni6","ni8","selray","vdd","vss",0);
/* ALU's output -> y */ 
LOINS ("n1_y","n13ms","selray","selaluy","vdd","vss",0);

/* taking in account of the above commands */

/* ra(3 downto 0) -> y(3 downto 0) */ 

LOINS ("a2_y","a21ms","ra[0]","selray","syra0","vdd","vss",0);
LOINS ("a2_y","a22ms","ra[1]","selray","syra1","vdd","vss",0);
LOINS ("a2_y","a23ms","ra[2]","selray","syra2","vdd","vss",0);
LOINS ("a2_y","a24ms","ra[3]","selray","syra3","vdd","vss",0);

/* alu_out(3 downto 0) -> y(3 downto 0) */ 

LOINS ("a2_y","a25ms","alu_out[0]","selaluy","syalu0","vdd","vss",0);
LOINS ("a2_y","a26ms","alu_out[1]","selaluy","syalu1","vdd","vss",0);
LOINS ("a2_y","a27ms","alu_out[2]","selaluy","syalu2","vdd","vss",0);
LOINS ("a2_y","a28ms","alu_out[3]","selaluy","syalu3","vdd","vss",0);

/* last stage of the multiplexer : amd's y output */

LOINS ("o2_y","o21ms","syra0","syalu0","y[0]","vdd","vss",0);
LOINS ("o2_y","o22ms","syra1","syalu1","y[1]","vdd","vss",0);
LOINS ("o2_y","o23ms","syra2","syalu2","y[2]","vdd","vss",0);
LOINS ("o2_y","o24ms","syra3","syalu3","y[3]","vdd","vss",0);

/* oe is the command for the 3 state output pad */

LOINS ("n1_y","n1oe","noe","oe","vdd","vss",0);

SAVE_LOFIG();
DEF_LOFIG ("ram");
/* declaration of the cell's external connectors */
LOCON ("A[3:0]",'I',"a[3:0]");
LOCON ("B[3:0]",'I',"b[3:0]");
LOCON ("ALU_OUT[3:0]",'I',"alu_out[3:0]");
LOCON ("FONC_MODE",'I',"fonc_mode");
LOCON ("R0",'I',"r0");
LOCON ("R3",'I',"r3");
LOCON ("S0",'O',"s0");
LOCON ("S3",'O',"s3");
LOCON ("DECALDR",'O',"decaldr");
LOCON ("DECALGR",'O',"decalgr");
LOCON ("RA[3:0]",'O',"ra[3:0]");
LOCON ("RB[3:0]",'O',"rb[3:0]");
LOCON ("I[8:7]",'I',"i[8:7]");
LOCON ("CK",'I',"ck");
LOCON ("VDD",'I',"vdd");
LOCON ("VSS",'I',"vss");

/* generation of the shifting and writing commands */

LOINS ("n1_y","n11ra","i[8]","decalnr","vdd","vss",0);
LOINS ("n1_y","n12ra","i[7]","ni7","vdd","vss",0);

LOINS ("a2_y","a21r","i[8]","ni7","decaldra","vdd","vss",0);
LOINS ("b1_y","bufdecaldr","decaldra","decaldr","vdd","vss",0);
LOINS ("b1_y","bufdecalgr","decalgra","decalgr","vdd","vss",0);
LOINS ("a2_y","a22r","i[8]","i[7]","decalgra","vdd","vss",0);
LOINS ("o2_y","o21r","i[8]","i[7]","o21s","vdd","vss",0);
/* writing operations only have to occure when fonc_mode is chosen: */
LOINS ("a2_y","a215r","fonc_mode","o21s","wram","vdd","vss",0);
LOINS ("a2p_y","a216r","ck","wram","enable","vdd","vss",0);



LOINS ("a2_y","a232r","decalgra","alu_out[3]","s3","vdd","vss",0);
LOINS ("a2_y","a233r","decaldra","alu_out[0]","s0","vdd","vss",0);


/* shifter */

/* bit slice 3 */
LOINS ("a2_y","a23r","alu_out[2]","decalgra","a23sh","vdd","vss",0);
LOINS ("a2_y","a24r","alu_out[3]","decalnr","a24sh","vdd","vss",0);
LOINS ("a2_y","a25r","r3","decaldra","a25sh","vdd","vss",0);

/* bit slice 2 */
LOINS ("a2_y","a26r","alu_out[1]","decalgra","a26sh","vdd","vss",0);
LOINS ("a2_y","a27r","alu_out[2]","decalnr","a27sh","vdd","vss",0);
LOINS ("a2_y","a28r","alu_out[3]","decaldra","a28sh","vdd","vss",0);

/* bit slice 1 */
LOINS ("a2_y","a29r","alu_out[0]","decalgra","a29sh","vdd","vss",0);
LOINS ("a2_y","a210r","alu_out[1]","decalnr","a210sh","vdd","vss",0);
LOINS ("a2_y","a211r","alu_out[2]","decaldra","a211sh","vdd","vss",0);

/* bit slice 0 */
LOINS ("a2_y","a212r","r0","decalgra","a212sh","vdd","vss",0);
LOINS ("a2_y","a213r","alu_out[0]","decalnr","a213sh","vdd","vss",0);
LOINS ("a2_y","a214r","alu_out[1]","decaldra","a214sh","vdd","vss",0);

/* RAM input */
LOINS ("o3_y","o31r","a23sh","a24sh","a25sh","shram3","vdd","vss",0);
LOINS ("o3_y","o32r","a26sh","a27sh","a28sh","shram2","vdd","vss",0);
LOINS ("o3_y","o33r","a29sh","a210sh","a211sh","shram1","vdd","vss",0);
LOINS ("o3_y","o34r","a212sh","a213sh","a214sh","shram0","vdd","vss",0);

/* a and b address-decoders */


/* a address */
LOINS ("np1_y","n15r","a[0]","na0","vdd","vss",0);
LOINS ("np1_y","n16r","a[1]","na1","vdd","vss",0);
LOINS ("np1_y","n17r","a[2]","na2","vdd","vss",0);
LOINS ("np1_y","n18r","a[3]","na3","vdd","vss",0);

LOINS ("a4_y","a41r","na3","na2","na1","na0","a1","vdd","vss",0);
LOINS ("a4_y","a42r","na3","na2","na1","a[0]","a2","vdd","vss",0);
LOINS ("a4_y","a43r","na3","na2","a[1]","na0","a3","vdd","vss",0);
LOINS ("a4_y","a44r","na3","na2","a[1]","a[0]","a4","vdd","vss",0);
LOINS ("a4_y","a45r","na3","a[2]","na1","na0","a5","vdd","vss",0);
LOINS ("a4_y","a46r","na3","a[2]","na1","a[0]","a6","vdd","vss",0);
LOINS ("a4_y","a47r","na3","a[2]","a[1]","na0","a7","vdd","vss",0);
LOINS ("a4_y","a48r","na3","a[2]","a[1]","a[0]","a8","vdd","vss",0);
LOINS ("a4_y","a49r","a[3]","na2","na1","na0","a9","vdd","vss",0);
LOINS ("a4_y","a410r","a[3]","na2","na1","a[0]","a10","vdd","vss",0);
LOINS ("a4_y","a411r","a[3]","na2","a[1]","na0","a11","vdd","vss",0);
LOINS ("a4_y","a412r","a[3]","na2","a[1]","a[0]","a12","vdd","vss",0);
LOINS ("a4_y","a413r","a[3]","a[2]","na1","na0","a13","vdd","vss",0);
LOINS ("a4_y","a414r","a[3]","a[2]","na1","a[0]","a14","vdd","vss",0);
LOINS ("a4_y","a415r","a[3]","a[2]","a[1]","na0","a15","vdd","vss",0);
LOINS ("a4_y","a416r","a[3]","a[2]","a[1]","a[0]","a16","vdd","vss",0);

/* b address */
LOINS ("n1_y","n111r","b[0]","nb0","vdd","vss",0);
LOINS ("n1_y","n112r","b[1]","nb1","vdd","vss",0);
LOINS ("n1_y","n113r","b[2]","nb2","vdd","vss",0);
LOINS ("n1_y","n114r","b[3]","nb3","vdd","vss",0);

LOINS ("a4_y","b41r","nb3","nb2","nb1","nb0","b1","vdd","vss",0);
LOINS ("a4_y","b42r","nb3","nb2","nb1","b[0]","b2","vdd","vss",0);
LOINS ("a4_y","b43r","nb3","nb2","b[1]","nb0","b3","vdd","vss",0);
LOINS ("a4_y","b44r","nb3","nb2","b[1]","b[0]","b4","vdd","vss",0);
LOINS ("a4_y","b45r","nb3","b[2]","nb1","nb0","b5","vdd","vss",0);
LOINS ("a4_y","b46r","nb3","b[2]","nb1","b[0]","b6","vdd","vss",0);
LOINS ("a4_y","b47r","nb3","b[2]","b[1]","nb0","b7","vdd","vss",0);
LOINS ("a4_y","b48r","nb3","b[2]","b[1]","b[0]","b8","vdd","vss",0);
LOINS ("a4_y","b49r","b[3]","nb2","nb1","nb0","b9","vdd","vss",0);
LOINS ("a4_y","b410r","b[3]","nb2","nb1","b[0]","b10","vdd","vss",0);
LOINS ("a4_y","b411r","b[3]","nb2","b[1]","nb0","b11","vdd","vss",0);
LOINS ("a4_y","b412r","b[3]","nb2","b[1]","b[0]","b12","vdd","vss",0);
LOINS ("a4_y","b413r","b[3]","b[2]","nb1","nb0","b13","vdd","vss",0);
LOINS ("a4_y","b414r","b[3]","b[2]","nb1","b[0]","b14","vdd","vss",0);
LOINS ("a4_y","b415r","b[3]","b[2]","b[1]","nb0","b15","vdd","vss",0);
LOINS ("a4_y","b416r","b[3]","b[2]","b[1]","b[0]","b16","vdd","vss",0);

/* selection of the b-addressed word for the writing */

LOINS ("a2_y","c16","enable","b16","ck16","vdd","vss",0);
LOINS ("a2_y","c15","enable","b15","ck15","vdd","vss",0);
LOINS ("a2_y","c14","enable","b14","ck14","vdd","vss",0);
LOINS ("a2_y","c13","enable","b13","ck13","vdd","vss",0);
LOINS ("a2_y","c12","enable","b12","ck12","vdd","vss",0);
LOINS ("a2_y","c11","enable","b11","ck11","vdd","vss",0);
LOINS ("a2_y","c10","enable","b10","ck10","vdd","vss",0);
LOINS ("a2_y","c9","enable","b9","ck9","vdd","vss",0);
LOINS ("a2_y","c8","enable","b8","ck8","vdd","vss",0);
LOINS ("a2_y","c7","enable","b7","ck7","vdd","vss",0);
LOINS ("a2_y","c6","enable","b6","ck6","vdd","vss",0);
LOINS ("a2_y","c5","enable","b5","ck5","vdd","vss",0);
LOINS ("a2_y","c4","enable","b4","ck4","vdd","vss",0);
LOINS ("a2_y","c3","enable","b3","ck3","vdd","vss",0);
LOINS ("a2_y","c2","enable","b2","ck2","vdd","vss",0);
LOINS ("a2_y","c1","enable","b1","ck1","vdd","vss",0);

/* building of the "memory-space" */

/* bit slice 3 */
LOINS ("msdp2_y","m_16_3","shram3","ck16","s316","vdd","vss",0);
LOINS ("msdp2_y","m_15_3","shram3","ck15","s315","vdd","vss",0);
LOINS ("msdp2_y","m_14_3","shram3","ck14","s314","vdd","vss",0);
LOINS ("msdp2_y","m_13_3","shram3","ck13","s313","vdd","vss",0);
LOINS ("msdp2_y","m_12_3","shram3","ck12","s312","vdd","vss",0);
LOINS ("msdp2_y","m_11_3","shram3","ck11","s311","vdd","vss",0);
LOINS ("msdp2_y","m_10_3","shram3","ck10","s310","vdd","vss",0);
LOINS ("msdp2_y","m_9_3","shram3","ck9","s39","vdd","vss",0);
LOINS ("msdp2_y","m_8_3","shram3","ck8","s38","vdd","vss",0);
LOINS ("msdp2_y","m_7_3","shram3","ck7","s37","vdd","vss",0);
LOINS ("msdp2_y","m_6_3","shram3","ck6","s36","vdd","vss",0);
LOINS ("msdp2_y","m_5_3","shram3","ck5","s35","vdd","vss",0);
LOINS ("msdp2_y","m_4_3","shram3","ck4","s34","vdd","vss",0);
LOINS ("msdp2_y","m_3_3","shram3","ck3","s33","vdd","vss",0);
LOINS ("msdp2_y","m_2_3","shram3","ck2","s32","vdd","vss",0);
LOINS ("msdp2_y","m_1_3","shram3","ck1","s31","vdd","vss",0);

/* bit slice 2 */
LOINS ("msdp2_y","m_16_2","shram2","ck16","s216","vdd","vss",0);
LOINS ("msdp2_y","m_15_2","shram2","ck15","s215","vdd","vss",0);
LOINS ("msdp2_y","m_14_2","shram2","ck14","s214","vdd","vss",0);
LOINS ("msdp2_y","m_13_2","shram2","ck13","s213","vdd","vss",0);
LOINS ("msdp2_y","m_12_2","shram2","ck12","s212","vdd","vss",0);
LOINS ("msdp2_y","m_11_2","shram2","ck11","s211","vdd","vss",0);
LOINS ("msdp2_y","m_10_2","shram2","ck10","s210","vdd","vss",0);
LOINS ("msdp2_y","m_9_2","shram2","ck9","s29","vdd","vss",0);
LOINS ("msdp2_y","m_8_2","shram2","ck8","s28","vdd","vss",0);
LOINS ("msdp2_y","m_7_2","shram2","ck7","s27","vdd","vss",0);
LOINS ("msdp2_y","m_6_2","shram2","ck6","s26","vdd","vss",0);
LOINS ("msdp2_y","m_5_2","shram2","ck5","s25","vdd","vss",0);
LOINS ("msdp2_y","m_4_2","shram2","ck4","s24","vdd","vss",0);
LOINS ("msdp2_y","m_3_2","shram2","ck3","s23","vdd","vss",0);
LOINS ("msdp2_y","m_2_2","shram2","ck2","s22","vdd","vss",0);
LOINS ("msdp2_y","m_1_2","shram2","ck1","s21","vdd","vss",0);

/* bit slice 1 */
LOINS ("msdp2_y","m_16_1","shram1","ck16","s116","vdd","vss",0);
LOINS ("msdp2_y","m_15_1","shram1","ck15","s115","vdd","vss",0);
LOINS ("msdp2_y","m_14_1","shram1","ck14","s114","vdd","vss",0);
LOINS ("msdp2_y","m_13_1","shram1","ck13","s113","vdd","vss",0);
LOINS ("msdp2_y","m_12_1","shram1","ck12","s112","vdd","vss",0);
LOINS ("msdp2_y","m_11_1","shram1","ck11","s111","vdd","vss",0);
LOINS ("msdp2_y","m_10_1","shram1","ck10","s110","vdd","vss",0);
LOINS ("msdp2_y","m_9_1","shram1","ck9","s19","vdd","vss",0);
LOINS ("msdp2_y","m_8_1","shram1","ck8","s18","vdd","vss",0);
LOINS ("msdp2_y","m_7_1","shram1","ck7","s17","vdd","vss",0);
LOINS ("msdp2_y","m_6_1","shram1","ck6","s16","vdd","vss",0);
LOINS ("msdp2_y","m_5_1","shram1","ck5","s15","vdd","vss",0);
LOINS ("msdp2_y","m_4_1","shram1","ck4","s14","vdd","vss",0);
LOINS ("msdp2_y","m_3_1","shram1","ck3","s13","vdd","vss",0);
LOINS ("msdp2_y","m_2_1","shram1","ck2","s12","vdd","vss",0);
LOINS ("msdp2_y","m_1_1","shram1","ck1","s11","vdd","vss",0);

/* bit slice 0 */
LOINS ("msdp2_y","m_16_0","shram0","ck16","s016","vdd","vss",0);
LOINS ("msdp2_y","m_15_0","shram0","ck15","s015","vdd","vss",0);
LOINS ("msdp2_y","m_14_0","shram0","ck14","s014","vdd","vss",0);
LOINS ("msdp2_y","m_13_0","shram0","ck13","s013","vdd","vss",0);
LOINS ("msdp2_y","m_12_0","shram0","ck12","s012","vdd","vss",0);
LOINS ("msdp2_y","m_11_0","shram0","ck11","s011","vdd","vss",0);
LOINS ("msdp2_y","m_10_0","shram0","ck10","s010","vdd","vss",0);
LOINS ("msdp2_y","m_9_0","shram0","ck9","s09","vdd","vss",0);
LOINS ("msdp2_y","m_8_0","shram0","ck8","s08","vdd","vss",0);
LOINS ("msdp2_y","m_7_0","shram0","ck7","s07","vdd","vss",0);
LOINS ("msdp2_y","m_6_0","shram0","ck6","s06","vdd","vss",0);
LOINS ("msdp2_y","m_5_0","shram0","ck5","s05","vdd","vss",0);
LOINS ("msdp2_y","m_4_0","shram0","ck4","s04","vdd","vss",0);
LOINS ("msdp2_y","m_3_0","shram0","ck3","s03","vdd","vss",0);
LOINS ("msdp2_y","m_2_0","shram0","ck2","s02","vdd","vss",0);
LOINS ("msdp2_y","m_1_0","shram0","ck1","s01","vdd","vss",0);

/* reading of the a-addressed word */

/* bit slice 3 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","am316","a16","s316","a316s","vdd","vss",0);
LOINS ("na2_y","am315","a15","s315","a315s","vdd","vss",0);
LOINS ("na2_y","am314","a14","s314","a314s","vdd","vss",0);
LOINS ("na2_y","am313","a13","s313","a313s","vdd","vss",0);
LOINS ("na2_y","am312","a12","s312","a312s","vdd","vss",0);
LOINS ("na2_y","am311","a11","s311","a311s","vdd","vss",0);
LOINS ("na2_y","am310","a10","s310","a310s","vdd","vss",0);
LOINS ("na2_y","am39","a9","s39","a39s","vdd","vss",0);
LOINS ("na2_y","am38","a8","s38","a38s","vdd","vss",0);
LOINS ("na2_y","am37","a7","s37","a37s","vdd","vss",0);
LOINS ("na2_y","am36","a6","s36","a36s","vdd","vss",0);
LOINS ("na2_y","am35","a5","s35","a35s","vdd","vss",0);
LOINS ("na2_y","am34","a4","s34","a34s","vdd","vss",0);
LOINS ("na2_y","am33","a3","s33","a33s","vdd","vss",0);
LOINS ("na2_y","am32","a2","s32","a32s","vdd","vss",0);
LOINS ("na2_y","am31","a1","s31","a31s","vdd","vss",0);

LOINS ("a4_y","oa413","a316s","a315s","a314s","a313s","oa413s","vdd","vss",0);
LOINS ("a4_y","oa423","a312s","a311s","a310s","a39s","oa423s","vdd","vss",0);
LOINS ("a4_y","oa433","a38s","a37s","a36s","a35s","oa433s","vdd","vss",0);
LOINS ("a4_y","oa443","a34s","a33s","a32s","a31s","oa443s","vdd","vss",0);
LOINS ("na4_y","oa453","oa413s","oa423s","oa433s","oa443s","ra[3]","vdd","vss",0);

/* bit slice 2 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","am216","a16","s216","a216s","vdd","vss",0);
LOINS ("na2_y","am215","a15","s215","a215s","vdd","vss",0);
LOINS ("na2_y","am214","a14","s214","a214s","vdd","vss",0);
LOINS ("na2_y","am213","a13","s213","a213s","vdd","vss",0);
LOINS ("na2_y","am212","a12","s212","a212s","vdd","vss",0);
LOINS ("na2_y","am211","a11","s211","a211s","vdd","vss",0);
LOINS ("na2_y","am210","a10","s210","a210s","vdd","vss",0);
LOINS ("na2_y","am29","a9","s29","a29s","vdd","vss",0);
LOINS ("na2_y","am28","a8","s28","a28s","vdd","vss",0);
LOINS ("na2_y","am27","a7","s27","a27s","vdd","vss",0);
LOINS ("na2_y","am26","a6","s26","a26s","vdd","vss",0);
LOINS ("na2_y","am25","a5","s25","a25s","vdd","vss",0);
LOINS ("na2_y","am24","a4","s24","a24s","vdd","vss",0);
LOINS ("na2_y","am23","a3","s23","a23s","vdd","vss",0);
LOINS ("na2_y","am22","a2","s22","a22s","vdd","vss",0);
LOINS ("na2_y","am21","a1","s21","a21s","vdd","vss",0);

LOINS ("a4_y","oa412","a216s","a215s","a214s","a213s","oa412s","vdd","vss",0);
LOINS ("a4_y","oa422","a212s","a211s","a210s","a29s","oa422s","vdd","vss",0);
LOINS ("a4_y","oa432","a28s","a27s","a26s","a25s","oa432s","vdd","vss",0);
LOINS ("a4_y","oa442","a24s","a23s","a22s","a21s","oa442s","vdd","vss",0);
LOINS ("na4_y","oa452","oa412s","oa422s","oa432s","oa442s","ra[2]","vdd","vss",0);

/* bit slice 1 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","am116","a16","s116","a116s","vdd","vss",0);
LOINS ("na2_y","am115","a15","s115","a115s","vdd","vss",0);
LOINS ("na2_y","am114","a14","s114","a114s","vdd","vss",0);
LOINS ("na2_y","am113","a13","s113","a113s","vdd","vss",0);
LOINS ("na2_y","am112","a12","s112","a112s","vdd","vss",0);
LOINS ("na2_y","am111","a11","s111","a111s","vdd","vss",0);
LOINS ("na2_y","am110","a10","s110","a110s","vdd","vss",0);
LOINS ("na2_y","am19","a9","s19","a19s","vdd","vss",0);
LOINS ("na2_y","am18","a8","s18","a18s","vdd","vss",0);
LOINS ("na2_y","am17","a7","s17","a17s","vdd","vss",0);
LOINS ("na2_y","am16","a6","s16","a16s","vdd","vss",0);
LOINS ("na2_y","am15","a5","s15","a15s","vdd","vss",0);
LOINS ("na2_y","am14","a4","s14","a14s","vdd","vss",0);
LOINS ("na2_y","am13","a3","s13","a13s","vdd","vss",0);
LOINS ("na2_y","am12","a2","s12","a12s","vdd","vss",0);
LOINS ("na2_y","am11","a1","s11","a11s","vdd","vss",0);

LOINS ("a4_y","oa411","a116s","a115s","a114s","a113s","oa411s","vdd","vss",0);
LOINS ("a4_y","oa421","a112s","a111s","a110s","a19s","oa421s","vdd","vss",0);
LOINS ("a4_y","oa431","a18s","a17s","a16s","a15s","oa431s","vdd","vss",0);
LOINS ("a4_y","oa441","a14s","a13s","a12s","a11s","oa441s","vdd","vss",0);
LOINS ("na4_y","oa451","oa411s","oa421s","oa431s","oa441s","ra[1]","vdd","vss",0);

/* bit slice 0 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","am016","a16","s016","a016s","vdd","vss",0);
LOINS ("na2_y","am015","a15","s015","a015s","vdd","vss",0);
LOINS ("na2_y","am014","a14","s014","a014s","vdd","vss",0);
LOINS ("na2_y","am013","a13","s013","a013s","vdd","vss",0);
LOINS ("na2_y","am012","a12","s012","a012s","vdd","vss",0);
LOINS ("na2_y","am011","a11","s011","a011s","vdd","vss",0);
LOINS ("na2_y","am010","a10","s010","a010s","vdd","vss",0);
LOINS ("na2_y","am09","a9","s09","a09s","vdd","vss",0);
LOINS ("na2_y","am08","a8","s08","a08s","vdd","vss",0);
LOINS ("na2_y","am07","a7","s07","a07s","vdd","vss",0);
LOINS ("na2_y","am06","a6","s06","a06s","vdd","vss",0);
LOINS ("na2_y","am05","a5","s05","a05s","vdd","vss",0);
LOINS ("na2_y","am04","a4","s04","a04s","vdd","vss",0);
LOINS ("na2_y","am03","a3","s03","a03s","vdd","vss",0);
LOINS ("na2_y","am02","a2","s02","a02s","vdd","vss",0);
LOINS ("na2_y","am01","a1","s01","a01s","vdd","vss",0);

LOINS ("a4_y","oa410","a016s","a015s","a014s","a013s","oa410s","vdd","vss",0);
LOINS ("a4_y","oa420","a012s","a011s","a010s","a09s","oa420s","vdd","vss",0);
LOINS ("a4_y","oa430","a08s","a07s","a06s","a05s","oa430s","vdd","vss",0);
LOINS ("a4_y","oa440","a04s","a03s","a02s","a01s","oa440s","vdd","vss",0);
LOINS ("na4_y","oa450","oa410s","oa420s","oa430s","oa440s","ra[0]","vdd","vss",0);

/* reading of the b-addressed word */

/* bit slice 3 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","bm316","b16","s316","b316s","vdd","vss",0);
LOINS ("na2_y","bm315","b15","s315","b315s","vdd","vss",0);
LOINS ("na2_y","bm314","b14","s314","b314s","vdd","vss",0);
LOINS ("na2_y","bm313","b13","s313","b313s","vdd","vss",0);
LOINS ("na2_y","bm312","b12","s312","b312s","vdd","vss",0);
LOINS ("na2_y","bm311","b11","s311","b311s","vdd","vss",0);
LOINS ("na2_y","bm310","b10","s310","b310s","vdd","vss",0);
LOINS ("na2_y","bm39","b9","s39","b39s","vdd","vss",0);
LOINS ("na2_y","bm38","b8","s38","b38s","vdd","vss",0);
LOINS ("na2_y","bm37","b7","s37","b37s","vdd","vss",0);
LOINS ("na2_y","bm36","b6","s36","b36s","vdd","vss",0);
LOINS ("na2_y","bm35","b5","s35","b35s","vdd","vss",0);
LOINS ("na2_y","bm34","b4","s34","b34s","vdd","vss",0);
LOINS ("na2_y","bm33","b3","s33","b33s","vdd","vss",0);
LOINS ("na2_y","bm32","b2","s32","b32s","vdd","vss",0);
LOINS ("na2_y","bm31","b1","s31","b31s","vdd","vss",0);

LOINS ("a4_y","ob413","b316s","b315s","b314s","b313s","ob413s","vdd","vss",0);
LOINS ("a4_y","ob423","b312s","b311s","b310s","b39s","ob423s","vdd","vss",0);
LOINS ("a4_y","ob433","b38s","b37s","b36s","b35s","ob433s","vdd","vss",0);
LOINS ("a4_y","ob443","b34s","b33s","b32s","b31s","ob443s","vdd","vss",0);
LOINS ("na4_y","ob453","ob413s","ob423s","ob433s","ob443s","rb[3]","vdd","vss",0);

/* bit slice 2 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","bm216","b16","s216","b216s","vdd","vss",0);
LOINS ("na2_y","bm215","b15","s215","b215s","vdd","vss",0);
LOINS ("na2_y","bm214","b14","s214","b214s","vdd","vss",0);
LOINS ("na2_y","bm213","b13","s213","b213s","vdd","vss",0);
LOINS ("na2_y","bm212","b12","s212","b212s","vdd","vss",0);
LOINS ("na2_y","bm211","b11","s211","b211s","vdd","vss",0);
LOINS ("na2_y","bm210","b10","s210","b210s","vdd","vss",0);
LOINS ("na2_y","bm29","b9","s29","b29s","vdd","vss",0);
LOINS ("na2_y","bm28","b8","s28","b28s","vdd","vss",0);
LOINS ("na2_y","bm27","b7","s27","b27s","vdd","vss",0);
LOINS ("na2_y","bm26","b6","s26","b26s","vdd","vss",0);
LOINS ("na2_y","bm25","b5","s25","b25s","vdd","vss",0);
LOINS ("na2_y","bm24","b4","s24","b24s","vdd","vss",0);
LOINS ("na2_y","bm23","b3","s23","b23s","vdd","vss",0);
LOINS ("na2_y","bm22","b2","s22","b22s","vdd","vss",0);
LOINS ("na2_y","bm21","b1","s21","b21s","vdd","vss",0);

LOINS ("a4_y","ob412","b216s","b215s","b214s","b213s","ob412s","vdd","vss",0);
LOINS ("a4_y","ob422","b212s","b211s","b210s","b29s","ob422s","vdd","vss",0);
LOINS ("a4_y","ob432","b28s","b27s","b26s","b25s","ob432s","vdd","vss",0);
LOINS ("a4_y","ob442","b24s","b23s","b22s","b21s","ob442s","vdd","vss",0);
LOINS ("na4_y","ob452","ob412s","ob422s","ob432s","ob442s","rb[2]","vdd","vss",0);

/* bit slice 1 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","bm116","b16","s116","b116s","vdd","vss",0);
LOINS ("na2_y","bm115","b15","s115","b115s","vdd","vss",0);
LOINS ("na2_y","bm114","b14","s114","b114s","vdd","vss",0);
LOINS ("na2_y","bm113","b13","s113","b113s","vdd","vss",0);
LOINS ("na2_y","bm112","b12","s112","b112s","vdd","vss",0);
LOINS ("na2_y","bm111","b11","s111","b111s","vdd","vss",0);
LOINS ("na2_y","bm110","b10","s110","b110s","vdd","vss",0);
LOINS ("na2_y","bm19","b9","s19","b19s","vdd","vss",0);
LOINS ("na2_y","bm18","b8","s18","b18s","vdd","vss",0);
LOINS ("na2_y","bm17","b7","s17","b17s","vdd","vss",0);
LOINS ("na2_y","bm16","b6","s16","b16s","vdd","vss",0);
LOINS ("na2_y","bm15","b5","s15","b15s","vdd","vss",0);
LOINS ("na2_y","bm14","b4","s14","b14s","vdd","vss",0);
LOINS ("na2_y","bm13","b3","s13","b13s","vdd","vss",0);
LOINS ("na2_y","bm12","b2","s12","b12s","vdd","vss",0);
LOINS ("na2_y","bm11","b1","s11","b11s","vdd","vss",0);

LOINS ("a4_y","ob411","b116s","b115s","b114s","b113s","ob411s","vdd","vss",0);
LOINS ("a4_y","ob421","b112s","b111s","b110s","b19s","ob421s","vdd","vss",0);
LOINS ("a4_y","ob431","b18s","b17s","b16s","b15s","ob431s","vdd","vss",0);
LOINS ("a4_y","ob441","b14s","b13s","b12s","b11s","ob441s","vdd","vss",0);
LOINS ("na4_y","ob451","ob411s","ob421s","ob431s","ob441s","rb[1]","vdd","vss",0);

/* bit slice 0 */
/* selection of 1 out of the 16 outputs of the D-latches */
LOINS ("na2_y","bm016","b16","s016","b016s","vdd","vss",0);
LOINS ("na2_y","bm015","b15","s015","b015s","vdd","vss",0);
LOINS ("na2_y","bm014","b14","s014","b014s","vdd","vss",0);
LOINS ("na2_y","bm013","b13","s013","b013s","vdd","vss",0);
LOINS ("na2_y","bm012","b12","s012","b012s","vdd","vss",0);
LOINS ("na2_y","bm011","b11","s011","b011s","vdd","vss",0);
LOINS ("na2_y","bm010","b10","s010","b010s","vdd","vss",0);
LOINS ("na2_y","bm09","b9","s09","b09s","vdd","vss",0);
LOINS ("na2_y","bm08","b8","s08","b08s","vdd","vss",0);
LOINS ("na2_y","bm07","b7","s07","b07s","vdd","vss",0);
LOINS ("na2_y","bm06","b6","s06","b06s","vdd","vss",0);
LOINS ("na2_y","bm05","b5","s05","b05s","vdd","vss",0);
LOINS ("na2_y","bm04","b4","s04","b04s","vdd","vss",0);
LOINS ("na2_y","bm03","b3","s03","b03s","vdd","vss",0);
LOINS ("na2_y","bm02","b2","s02","b02s","vdd","vss",0);
LOINS ("na2_y","bm01","b1","s01","b01s","vdd","vss",0);

LOINS ("a4_y","ob410","b016s","b015s","b014s","b013s","ob410s","vdd","vss",0);
LOINS ("a4_y","ob420","b012s","b011s","b010s","b09s","ob420s","vdd","vss",0);
LOINS ("a4_y","ob430","b08s","b07s","b06s","b05s","ob430s","vdd","vss",0);
LOINS ("a4_y","ob440","b04s","b03s","b02s","b01s","ob440s","vdd","vss",0);
LOINS ("na4_y","ob450","ob410s","ob420s","ob430s","ob440s","rb[0]","vdd","vss",0);

SAVE_LOFIG();

DEF_LOFIG ("heart");
/* declaration of the cell's external connectors */
LOCON ("A[3:0]",'I',"a[3:0]");
LOCON ("B[3:0]",'I',"b[3:0]");
LOCON ("D[3:0]",'I',"d[3:0]");
LOCON ("CIN",'X',"cin");
LOCON ("COUT",'X',"cout");
LOCON ("NP",'O',"np");
LOCON ("NG",'O',"ng");
LOCON ("SIGNE",'X',"signe");
LOCON ("ZERO",'O',"zero");
LOCON ("OVR",'O',"ovr");
LOCON ("I[8:0]",'I',"i[8:0]");
LOCON ("Q0",'I',"q0");
LOCON ("Q3",'I',"q3");
LOCON ("F0",'O',"f0");
LOCON ("F3",'O',"f3");
LOCON ("FONC",'I',"fonc");
LOCON ("TEST",'I',"test");
LOCON ("SCIN",'I',"scin");
LOCON ("SCOUT",'O',"scout");
LOCON ("DECALD",'O',"decald");
LOCON ("DECALG",'O',"decalg");
LOCON ("DECALDR",'O',"decaldr");
LOCON ("DECALGR",'O',"decalgr");
LOCON ("R0",'I',"r0");
LOCON ("R3",'I',"r3");
LOCON ("S0",'O',"s0");
LOCON ("S3",'O',"s3");
LOCON ("CK",'I',"ck");
LOCON ("Y[3:0]",'O',"y[3:0]");
LOCON ("NOE",'I',"noe");
LOCON ("OE",'O',"oe");
LOCON ("VDD",'I',"vdd");
LOCON ("VSS",'I',"vss");

/* calling of all previously defined blocks */

LOINS ("muxe","block1","ra[3:0]","rb[3:0]","q[3:0]","d[3:0]","r[3:0]","s[3:0]","i[2:0]","vdd","vss",0);

LOINS ("muxout","block2","ra[3:0]","alu_out[3:0]","noe","oe","y[3:0]","i[8:6]","vdd","vss",0);

LOINS ("alu","block3","r[3:0]","s[3:0]","alu_out[3:0]","cin","cout","np","ng","signe","zero","ovr","i[5:3]","vdd","vss",0);

LOINS ("accu","block4","alu_out[3:0]","q0","q3","fonc","test","fonc_mode","scin","scout","f0","f3","decald","decalg","q[3:0]","i[8:6]","ck","vdd","vss",0);
LOINS ("ram","block5","a[3:0]","b[3:0]","alu_out[3:0]","fonc_mode","r0","r3","s0","s3","decaldr","decalgr","ra[3:0]","rb[3:0]","i[8:7]","ck","vdd","vss",0);

SAVE_LOFIG();

DEF_LOFIG ("chip");

LOCON ("A[3:0]",'I',"a[3:0]");
LOCON ("B[3:0]",'I',"b[3:0]");
LOCON ("D[3:0]",'I',"d[3:0]");
LOCON ("CIN",'X',"cin");
LOCON ("COUT",'X',"cout");
LOCON ("NP",'O',"np");
LOCON ("NG",'O',"ng");
LOCON ("SIGNE",'X',"signe");
LOCON ("ZERO",'O',"zero");
LOCON ("OVR",'O',"ovr");
LOCON ("I[8:0]",'I',"i[8:0]");
LOCON ("Q0",'X',"q0");
LOCON ("Q3",'X',"q3");
LOCON ("R0",'X',"r0");
LOCON ("R3",'X',"r3");
LOCON ("NOE",'I',"noe");
LOCON ("FONC",'I',"fonc");
LOCON ("TEST",'I',"test");
LOCON ("SCIN",'I',"scin");
LOCON ("SCOUT",'O',"scout");
LOCON ("CKE",'I',"cke");
LOCON ("VDDE",'I',"vdde");
LOCON ("VSSE",'I',"vsse");
LOCON ("VDDI",'I',"vddi");
LOCON ("VSSI",'I',"vssi");
LOCON ("Y[3:0]",'X',"y[3:0]");

/* ring of pads alimentation */

LOINS ("pvdde_sp","vdde_p0","ck","vdde","vddi","vsse","vssi",0); 
LOINS ("pvsse_sp","vsse_p0","ck","vdde","vddi","vsse","vssi",0); 
LOINS ("pvsseck_sp","vsse_ck","cko","ck","vdde","vddi","vsse","vssi",0); 
/* heart alimentation */

LOINS ("pvddi_sp","vddi_p1","ck","vdde","vddi","vsse","vssi",0); 
LOINS ("pvssi_sp","vssi_p1","ck","vdde","vddi","vsse","vssi",0); 

LOINS ("pi_sp","a3","a[3]","ai[3]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","a2","a[2]","ai[2]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","a1","a[1]","ai[1]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","a0","a[0]","ai[0]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","b3","b[3]","bi[3]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","b2","b[2]","bi[2]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","b1","b[1]","bi[1]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","b0","b[0]","bi[0]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","d3","d[3]","di[3]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","d2","d[2]","di[2]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","d1","d[1]","di[1]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","d0","d[0]","di[0]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","cin","cin","cini","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","scout","scoutc","scout","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","cout","coutc","cout","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","np","npc","np","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","ng","ngc","ng","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","signe","signec","signe","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","zero","zeroc","zero","ck","vdde","vddi","vsse","vssi",0);
LOINS ("po_sp","ovr","ovrc","ovr","ck","vdde","vddi","vsse","vssi",0);
LOINS ("piot_sp","q0","f0c","decaldc","q0i","q0","ck","vdde","vddi","vsse","vssi",0);
LOINS ("piot_sp","q3","f3c","decalgc","q3i","q3","ck","vdde","vddi","vsse","vssi",0);
LOINS ("piot_sp","r0","s0c","decaldrc","r0i","r0","ck","vdde","vddi","vsse","vssi",0);
LOINS ("piot_sp","r3","s3c","decalgrc","r3i","r3","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","noe","noe","noei","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","fonc","fonc","fonci","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","test","test","testi","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","scin","scin","scini","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i8","i[8]","ii[8]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i7","i[7]","ii[7]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i6","i[6]","ii[6]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i5","i[5]","ii[5]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i4","i[4]","ii[4]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i3","i[3]","ii[3]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i2","i[2]","ii[2]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i1","i[1]","ii[1]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pi_sp","i0","i[0]","ii[0]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pck_sp","ck","cke","ck","vdde","vddi","vsse","vssi",0);

LOINS ("pot_sp","y0","yc[0]","oec","y[0]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pot_sp","y1","yc[1]","oec","y[1]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pot_sp","y2","yc[2]","oec","y[2]","ck","vdde","vddi","vsse","vssi",0);
LOINS ("pot_sp","y3","yc[3]","oec","y[3]","ck","vdde","vddi","vsse","vssi",0);

LOINS ("heart","heart","ai[3:0]","bi[3:0]","di[3:0]","cini","coutc","npc","ngc","signec","zeroc","ovrc","ii[8:0]","q0i","q3i","f0c","f3c","fonci","testi","scini","scoutc","decaldc","decalgc","decaldrc","decalgrc","r0i","r3i","s0c","s3c","cko","yc[3:0]","noei","oec","vddi","vssi",0);

SAVE_LOFIG();
exit(0);
}
