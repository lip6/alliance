/*
-- ### -------------------------------------------------------------- ###
-- #									#
-- # file	: dlxm_dpt.c						#
-- # date	: February 1995						#
-- # version	: v0.0							#
-- #									#
-- # origin	: this description has been developed by CAO-VLSI team	#
-- #		  at MASI laboratory, University Pierre et Marie Curie	#
-- #		  URA CNRS 818, Institut Blaise Pascal			#
-- #		  4 Place Jussieu 75252 Paris Cedex 05 - France		#
-- # E-mail      :      cao-vlsi@masi.ibp.fr                            #
-- #									#
-- # descr.	: source code for datapath generator FPGEN		#
-- #									#
-- ### -------------------------------------------------------------- ###
*/

#include <genlib.h>
#include <fpgen.h>

main()
{

DP_DEFLOFIG("dlxm_dpt", 32L, LSB_INDEX_ZERO);

/*        -- Power supply terminals.
*/
DP_LOCON("VDD", IN   ,"vdd");
DP_LOCON("VSS",   OUT,"vss");

/*    -- Control connectors of registers PC, IAR, AD, IR, DT.
**    -- Select the function mode (enable SCAN mode when set to one).
*/
DP_LOCON("pc_test", IN   ,"pc_test");
DP_LOCON("ad_test", IN   ,"ad_test");
DP_LOCON("tvr_test", IN   ,"tvr_test");
DP_LOCON("iar_test", IN   ,"iar_test");
DP_LOCON("sr_test", IN   ,"sr_test");
DP_LOCON("ir_test", IN   ,"ir_test");
DP_LOCON("dt_test", IN   ,"dt_test");

/*    -- Scan-Path connectors.
*/
DP_LOCON("pc_scin", IN   ,"pc_scin");
DP_LOCON("ad_scin", IN   ,"ad_scin");
DP_LOCON("tvr_scin", IN   ,"tvr_scin");
DP_LOCON("iar_scin", IN   ,"iar_scin");
DP_LOCON("sr_scin", IN   ,"sr_scin");
DP_LOCON("ir_scin", IN   ,"ir_scin");
DP_LOCON("dt_scin", IN   ,"dt_scin");
DP_LOCON("pc_scout",   OUT,"pc_s[31]");
DP_LOCON("ad_scout",   OUT,"ad_s[31]");
DP_LOCON("tvr_scout",   OUT,"tvr_s[31]");
DP_LOCON("iar_scout",   OUT,"iar_s[31]");
DP_LOCON("sr_scout",   OUT,"sr_s[31]");
DP_LOCON("ir_scout",   OUT,"ir_s[31]");
DP_LOCON("dt_scout",   OUT,"dt_s[31]");

/*    -- Clock connectors.
*/
DP_LOCON("pc_ck", IN   ,"pc_ck");
DP_LOCON("ad_ck", IN   ,"ad_ck");
DP_LOCON("tvr_ck", IN   ,"tvr_ck");
DP_LOCON("iar_ck", IN   ,"iar_ck");
DP_LOCON("sr_ck", IN   ,"sr_ck");
DP_LOCON("ir_ck", IN   ,"ir_ck");
DP_LOCON("dt_ck", IN   ,"dt_ck");

/*    -- Write enable, active only in normal mode.
**    -- (enable writing when set to one)
*/
DP_LOCON("pc_wen", IN   ,"pc_wen");
DP_LOCON("ad_wen", IN   ,"ad_wen");
DP_LOCON("tvr_wen", IN   ,"tvr_wen");
DP_LOCON("iar_wen", IN   ,"iar_wen");
DP_LOCON("sr_wen", IN   ,"sr_wen");
DP_LOCON("ir_wen", IN   ,"ir_wen");
DP_LOCON("dt_wen", IN   ,"dt_wen");

/*    -- Control connectors of the register file.
*/
DP_LOCON("rf_ck", IN   ,"rf_ck");
DP_LOCON("rf_wen", IN   ,"rf_wen");
DP_LOCON("rf_aw[4:0]", IN   ,"rf_aw[4:0]");
DP_LOCON("rf_ar[4:0]", IN   ,"rf_ar[4:0]");

/*    -- Controls connectors of the SR block.
*/
DP_LOCON("sr_mx", IN   ,"sr_mx");

/*    -- Flags to write in SR from the CTRL block.
*/
DP_LOCON("sr_mode",   OUT,"sr_s[0]");
DP_LOCON("sr_mask",   OUT,"sr_s[1]");
DP_LOCON("sr_cpurst", IN   ,"sr_cpurst");
DP_LOCON("sr_iav", IN   ,"sr_iav");
DP_LOCON("sr_ico", IN   ,"sr_ico");
DP_LOCON("sr_dav", IN   ,"sr_dav");
DP_LOCON("sr_ovr", IN   ,"sr_ovr");
DP_LOCON("sr_it0", IN   ,"sr_it0");
DP_LOCON("sr_it1", IN   ,"sr_it1");
DP_LOCON("sr_it2", IN   ,"sr_it2");
DP_LOCON("sr_it3", IN   ,"sr_it3");

/*    -- Control connectors of X multiplexer.
**    -- Select between CH , CB , C0 , R[i] , SR , TVR, IAR , AD , PC.
*/
DP_LOCON("opx_ts4", IN   ,"opx_ts4");
DP_LOCON("opx_ts3", IN   ,"opx_ts3");
DP_LOCON("opx_ts2", IN   ,"opx_ts2");
DP_LOCON("opx_ts1", IN   ,"opx_ts1");
DP_LOCON("opx_ts0", IN   ,"opx_ts0");
DP_LOCON("opx_mx4", IN   ,"opx_mx4");
DP_LOCON("opx_mx3", IN   ,"opx_mx3");
DP_LOCON("opx_mx2", IN   ,"opx_mx2");
DP_LOCON("opx_mx1", IN   ,"opx_mx1");

/*    -- Sign of X operand ( opx_out[31] )
*/
DP_LOCON("opx_sign",   OUT,"opx_ots[31]");

/*    -- Control connectors of Y multiplexer.
**    -- Select between C16, C4, C0, I16, I18, I28, DT, AD.
*/
DP_LOCON("opy_ts4", IN   ,"opy_ts4");
DP_LOCON("opy_ts3", IN   ,"opy_ts3");
DP_LOCON("opy_ts2", IN   ,"opy_ts2");
DP_LOCON("opy_ts1", IN   ,"opy_ts1");
DP_LOCON("opy_mx4", IN   ,"opy_mx4");
DP_LOCON("opy_mx3", IN   ,"opy_mx3");
DP_LOCON("opy_mx2", IN   ,"opy_mx2");
DP_LOCON("opy_mx1", IN   ,"opy_mx1");

/*    -- When one byte is read, tells which one in a word.
**    -- (according to the two last bits of "out_adr_n").
*/
DP_LOCON("adrw_byte", IN   ,"adrw_byte");
DP_LOCON("adrw_rb1", IN   ,"adrw_rb1");
DP_LOCON("adrw_rb0", IN   ,"adrw_rb0");

/*    -- Fields of the IR register.
*/
DP_LOCON("opy_codop[5:0]",   OUT,"opy_codop[5:0]");
DP_LOCON("opy_rs[4:0]",   OUT,"opy_rs[4:0]");
DP_LOCON("opy_rdrt[4:0]",   OUT,"opy_rdrt[4:0]");
DP_LOCON("opy_rd[4:0]",   OUT,"opy_rd[4:0]");
DP_LOCON("opy_tvr",   OUT,"opy_tvr");
DP_LOCON("opy_sr",   OUT,"opy_sr");
DP_LOCON("opy_iar",   OUT,"opy_iar");

/*    -- Sign of Y operand ( opy_out[31] )
*/
DP_LOCON("opy_sign", OUT,"opy_ots[31]");

/*    -- Main data bus input, from the non inverting pads.
*/
/* DP_LOCON("data_in_n[31:0]", IN   ,"data_in_n[31:0]");
*/
DP_LOCON("data_in_dpt[31:0]", IN   ,"data_in_dpt[31:0]");

/*    -- Control connectors of ALU multiplexer.
*/
DP_LOCON("alu_mx4i0", IN   ,"alu_mx4i0");
DP_LOCON("alu_mx3i0", IN   ,"alu_mx3i0");
DP_LOCON("alu_mx2i1", IN   ,"alu_mx2i1");
DP_LOCON("alu_mx2i0", IN   ,"alu_mx2i0");
DP_LOCON("alu_mx1i2", IN   ,"alu_mx1i2");
DP_LOCON("alu_mx1i1", IN   ,"alu_mx1i1");
DP_LOCON("alu_mx1i0", IN   ,"alu_mx1i0");
DP_LOCON("alu_mx0i0", IN   ,"alu_mx0i0");

/*    -- Auxiliary ALU inputs/outputs.
*/
DP_LOCON("alu_byte", IN   ,"alu_byte");
DP_LOCON("alu_shrot", IN   ,"alu_shrot");
DP_LOCON("alu_test_n", IN   ,"alu_test_n");
DP_LOCON("alu_c31",   OUT,"alu_c31");
DP_LOCON("alu_c30",   OUT,"alu_c30");
DP_LOCON("alu_nul",   OUT,"alu_nul");
DP_LOCON("alu_sign",   OUT,"sum_alu[31]");

/*    -- Main data bus output, to the non inverting pads.
*/
DP_LOCON("data_out_dpt[31:0]",   OUT,"data_out_dpt[31:0]");

/*    -- Control connectors of ADROUT multiplexer.
*/
DP_LOCON("out_mx0i0",   OUT,"out_mx0i0");


/*    -- Main address bus output, to the non inverting pads.
*/
DP_LOCON("out_adr[31:0]",   OUT,"out_adr[31:0]");

/*    -- Address bus inverted output, to the status and datapath.
*/

DP_LOCON("adr_n_31",   INOUT,"out_adr_n[31]");
DP_LOCON("adr_n_1",   INOUT,"out_adr_n[1]");
DP_LOCON("adr_n_0",   INOUT,"out_adr_n[0]");


/*********************************************************************
*
*     Bus ADR OUT Multiplexor 
*
*********************************************************************/

#ifdef DPT_DEBUG
printf(" INV_OUT_ADR \n");
#endif
DP_INV(		"INV_OUT_ADR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		1,
		"out_adr_n[31:0]",
		"out_adr[31:0]",
		EOL);

printf("\n Bus ADR OUT  Multiplexor \n");

DP_NMUX2CS(	"MX_ADR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"out_mx0i0",
		"pc_s[31:0]",
		"ad_s[31:0]",
		"out_adr_n[31:0]",
		EOL);


/*********************************************************************
*
*     Register File R0 = 0 , R1 --> R31
*
*********************************************************************/

printf("\n Register File R0 = 0 , R1 --> R31 \n");

DP_RFG1C0(	"RF",
		32,
		"rf_ar[4:0]",
		"rf_wen",
		"rf_aw[4:0]",
		"alu_out[31:0]",
		"rf_s[31:0]",
		"rf_ck",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X TR_RF_X \n");
#endif
DP_BUSE(	"TR_RF_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_ts0",
		"rf_s[31:0]",
		"opx_ots[31:0]",
		EOL);


/*********************************************************************
*
*     Register TVR
*
*********************************************************************/

printf("\n Register TVR \n");

DP_PDFFT(	"TVR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"tvr_scin",
		"tvr_test",
		"tvr_wen",
		"tvr_ck",
		"alu_out[31:0]",
		"tvr_s[31:0]",
        "tvr_ns[31:0]",
		EOL);


/*********************************************************************
*
*     Register IAR
*
*********************************************************************/

printf("\n Register IAR \n");

DP_PDFFT(	"IAR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"iar_scin",
		"iar_test",
		"iar_wen",
		"iar_ck",
		"alu_out[31:0]",
		"iar_s[31:0]",
		"iar_ns[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X MX_TVR_IAR_X \n");
#endif
DP_NMUX2CS(	"MX_TVR_IAR_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_mx2",
		"tvr_s[31:0]",
		"iar_s[31:0]",
		"x_tvr_iar[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X TR_TVR_IAR_X \n");
#endif
DP_NBUSE(	"TR_TVR_IAR_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_ts2",
		"x_tvr_iar[31:0]",
		"opx_ots[31:0]",
		EOL);


/*********************************************************************
*
*     Register PC
*
*********************************************************************/

printf("\n Register PC \n");

DP_PDFFT(	"PC",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"pc_scin",
		"pc_test",
		"pc_wen",
		"pc_ck",
		"alu_out[31:0]",
		"pc_s[31:0]",
		"pc_ns[31:0]",
		EOL);


/*********************************************************************
*
*     ALU
*
*********************************************************************/

printf("\n ALU \n");

#ifdef DPT_DEBUG
printf(" ADD_ALU \n");
#endif
DP_ADSB2F(	"ADD_ALU",
		"opx_ots[31:0]",
		"opy_ots[31:0]",
		"alu_c31",
		"alu_c30",
		"sum_alu[31:0]",
		"alu_mx0i0",
		EOL);

#ifdef DPT_DEBUG
printf(" NUL_ALU \n");
#endif
DP_NUL(	"NUL_ALU",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"sum_alu[31:0]",
		"alu_nul",
		EOL);

#ifdef DPT_DEBUG
printf(" XR_ALU \n");
#endif
DP_XOR2(	"XR_ALU",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_ots[31:0]",
		"opy_ots[31:0]",
		"xor_alu[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_SUM_XOR \n");
#endif
DP_NMUX2CS(	"MX_SUM_XOR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"alu_mx1i2",
		"sum_alu[31:0]",
		"xor_alu[31:0]",
		"sum_xor[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" NA_ALU \n");
#endif
DP_NAND2(	"NA_ALU",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		1,
		"opx_ots[31:0]",
		"opy_ots[31:0]",
		"nand_alu[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" NR_ALU \n");
#endif
DP_NOR2(	"NR_ALU",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		1,
		"opx_ots[31:0]",
		"opy_ots[31:0]",
		"nor_alu[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_AND_OR \n");
#endif
DP_MUX2CS(	"MX_AND_OR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"alu_mx1i1",
		"nand_alu[31:0]",
		"nor_alu[31:0]",
		"nand_nor[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_OP_ALU \n");
#endif
DP_NMUX2CS(	"MX_OP_ALU",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"alu_mx2i1",
		"sum_xor[31:0]",
		"nand_nor[31:0]",
		"al_op[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" SHF_ALU \n");
#endif
DP_SHIFT(	"SHF_ALU",
		"opy_ots[4:0]",
		"alu_mx2i0","alu_shrot","alu_mx1i0",
		"opx_ots[31:0]",
		"shift_alu[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_ALU_SH \n");
#endif
DP_NMUX2CS(	"MX_ALU_SH",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"alu_mx3i0",
		"al_op[31:0]",
		"shift_alu[31:0]",
		"op_shift[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_ALU_OUT \n");
#endif
DP_NMUX2CS(	"MX_ALU_OUT",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"alu_mx4i0",
		"op_shift[31:0]",
		"C_n0[31:1]","alu_test_n",
		"alu_out[31:0]",
		EOL);


/*********************************************************************
*
*     Constants
*
*********************************************************************/

printf("\n Constants \n");

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y TR_C16_C04_Y \n");
#endif
DP_NBUSE(	"TR_C16_C04_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_ts4",
		"y_c16_c4[31:0]",
		"opy_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y MX_C16_C04_Y \n");
#endif
DP_NMUX2CS(	"MX_C16_C04_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_mx4",
		"opy_C0[31:5]","C_n0[0]","opy_C0[3:0]",
		"opy_C0[31:3]","C_n0[0]","opy_C0[1:0]",
		"y_c16_c4[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Cst_n0 \n");
#endif
DP_CONST(	"Cst_n0",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"0xFFFFFFFF",
		"C_n0[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" C0_Y \n");
#endif
DP_CONST(	"C0_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"0x0",
		"opy_C0[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y TR_C0_I16_Y \n");
#endif
DP_NBUSE(	"TR_C0_I16_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_ts3",
		"y_c0_i16[31:0]",
		"opy_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y MX_C0_I16_Y \n");
#endif
DP_NMUX2CS(	"MX_C0_I16_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_mx3",
		"opy_C0[31:0]",
		"se_i16[31:16]","ir_s[15:0]",
		"y_c0_i16[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X TR_C0_SR_X \n");
#endif
DP_NBUSE(	"TR_C0_SR_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_ts3",
		"x_c0_sr[31:0]",
		"opx_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X TR_CH_CB_X \n");
#endif
DP_NBUSE(	"TR_CH_CB_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_ts4",
		"x_ch_cb[31:0]",
		"opx_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X MX_CH_CB_X \n");
#endif
DP_NMUX2CS(	"MX_CH_CB_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_mx4",
		"opx_C0[31:16]","C_n0[24:9]",
		"opx_C0[23:0]","C_n0[8:1]",
		"x_ch_cb[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" C0_X \n");
#endif
DP_CONST(	"C0_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"0x0",
		"opx_C0[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X MX_C0_SR_X \n");
#endif
DP_NMUX2CS(	"MX_C0_SR_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_mx3",
		"opx_C0[31:0]",
		"sr_s[31:0]",
		"x_c0_sr[31:0]",
		EOL);


/*********************************************************************
*
*     Register AD
*
*********************************************************************/

printf("\n Register AD \n");

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y TR_DT_AD_Y \n");
#endif
DP_NBUSE(	"TR_DT_AD_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_ts1",
		"y_dt_ad[31:0]",
		"opy_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X TR_AD_PC_X \n");
#endif
DP_NBUSE(	"TR_AD_PC_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_ts1",
		"x_ad_pc[31:0]",
		"opx_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. X MX_AD_PC_X \n");
#endif
DP_NMUX2CS(	"MX_AD_PC_X",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opx_mx1",
		"ad_s[31:0]",
		"pc_s[31:0]",
		"x_ad_pc[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y MX_DT_AD_Y \n");
#endif
DP_NMUX2CS(	"MX_DT_AD_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_mx1",
		"dt_s[31:0]",
		"ad_s[31:0]",
		"y_dt_ad[31:0]",
		EOL);

DP_PDFFT(	"AD",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"ad_scin",
		"ad_test",
		"ad_wen",
		"ad_ck",
		"alu_out[31:0]",
		"ad_s[31:0]",
		"ad_ns[31:0]",
		EOL);


/*********************************************************************
*
*     Register SR
*
*********************************************************************/

printf("\n Register SR \n");

#ifdef DPT_DEBUG
printf(" Cst_SR_0 \n");
#endif
DP_CONST(       "Cst_SR_0",
                7,
                DEFAULT_SLICE,
                "0x0",
                "C_SR0[6:0]",
                EOL);

#ifdef DPT_DEBUG
printf(" MX_SR \n");
#endif
DP_MUX2CS(	"MX_SR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"sr_mx",
		"alu_out[31:16]",	"sr_it3","sr_it2","sr_it1","sr_it0",
					"sr_ovr","sr_dav","sr_ico","sr_iav",
					"C_SR0[6:3]",
					"sr_cpurst","C_SR0[2:0]",
		"alu_out[31:0]",
		"sr_in[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" SR \n");
#endif
DP_PDFFT(	"SR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"sr_scin",
		"sr_test",
		"sr_wen",
		"sr_ck",
		"sr_in[31:0]",
		"sr_s[31:0]",
		"sr_ns[31:0]",
		EOL);


/*********************************************************************
*
*     Immediates
*
*********************************************************************/

printf("\n Immediates \n");

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y TR_I18_I28_Y \n");
#endif
DP_NBUSE(	"TR_I18_I28_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_ts2",
		"y_i18_i28[31:0]",
		"opy_ots[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" Multiplexor op. Y MX_I18_I28_Y \n");
#endif
DP_NMUX2CS(	"MX_I18_I28_Y",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"opy_mx2",
		"se_i16[31:18]","ir_s[15:0]","opy_C0[1:0]",
		"se_i28[31:28]","ir_s[25:0]","opy_C0[1:0]",
		"y_i18_i28[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" INV_I16_I28 \n");
#endif
DP_INV(		"INV_I16_I28",
		20,
		12,
		1,
		"s_n_i28","s_n_i28","s_n_i28","s_n_i28",
                "s_n_i16","s_n_i16","s_n_i16","s_n_i16",
		"s_n_i16","s_n_i16","s_n_i16","s_n_i16",
		"s_n_i16","s_n_i16","s_n_i16","s_n_i16",
		"s_n_i16","s_n_i16","s_n_i16","s_n_i16",
		"se_i28[31:28]","se_i16[31:16]",
		EOL);

#ifdef DPT_DEBUG
printf(" INV_IMD \n");
#endif
DP_INV(		"INV_IMD",
		2,
		20,
		3,
		"ir_s[25]","ir_s[15]",
		"s_n_i28","s_n_i16",
		EOL);


/*********************************************************************
*
*     Register IR
*
*********************************************************************/

printf("\n Register IR \n");

#ifdef DPT_DEBUG
printf(" INV_IR_N \n");
#endif
DP_INV(		"INV_IR_N",
		24,
		8,
		1,
		"ir_s[31:11]","ir_s[2:0]",
		"ir_s_n[31:11]","ir_s_n[2:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" INV_CODOP \n");
#endif
DP_INV(		"INV_CODOP",
		24,
		8,
		1,
		"ir_s_n[31:11]","ir_s_n[2:0]",
		"opy_codop[5:0]",
			"opy_rs[4:0]","opy_rdrt[4:0]","opy_rd[4:0]",
			"opy_tvr","opy_sr","opy_iar",
		EOL);

#ifdef DPT_DEBUG
printf(" IR \n");
#endif
DP_PDFFT(	"IR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"ir_scin",
		"ir_test",
		"ir_wen",
		"ir_ck",
		"data_in[31:0]",
		"ir_s[31:0]",
		"ir_ns[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" INV_IR \n");
#endif
DP_INV(		"INV_IR",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		1,
		"data_in_n[31:0]",
		"data_in[31:0]",
		EOL);


/************************************************************
*
*  Data in inversion
*
************************************************************/

#ifdef DPT_DEBUG
printf(" INV_IN_DPT \n");
#endif
DP_INV(		"INV_DATA_IN_DPT",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		1,
		"data_in_dpt[31:0]",
		"data_in_n[31:0]",
		EOL);

/*********************************************************************
*
*     Register DT
*
*********************************************************************/

printf("\n Register DT \n");

#ifdef DPT_DEBUG
printf(" MX_Hx_Lx_DT \n");
#endif
DP_NMUX2CS(	"MX_Hx_Lx_DT",
		16,
		8,
		"adrw_rb1",
		"data_in_n[31:24]","data_in_n[23:16]",
		"data_in_n[15:8]","data_in_n[7:0]",
		"data_in_hmx[7:0]","data_in_lmx[7:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_H_L_DT \n");
#endif
DP_NMUX2CS(	"MX_H_L_DT",
		8,
		12,
		"adrw_rb0",
		"data_in_hmx[7:0]",
		"data_in_lmx[7:0]",
		"data_in_mx[7:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" MX_LB_DT \n");
#endif
DP_NMUX2CS(	"MX_LB_DT",
		8,
		12,
		"adrw_byte",
		"data_in_mx[7:0]",
		"data_in_n[7:0]",
		"data_in_lb[7:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" DT \n");
#endif
DP_PDFFT(	"DT",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"dt_scin",
		"dt_test",
		"dt_wen",
		"dt_ck",
		"data_in[31:8]","data_in_lb[7:0]",
		"dt_s[31:0]",
		"dt_ns[31:0]",
		EOL);


/*********************************************************************
*
*     Multiplexor Bus DATA OUT
*
*********************************************************************/

printf("\n Multiplexor Bus DATA OUT \n");

DP_NMUX2CS(	"MX_DATA",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		"alu_byte",
		"alu_out[7:0]","alu_out[7:0]","alu_out[7:0]","alu_out[7:0]",
		"alu_out[31:0]",
		"data_out_n[31:0]",
		EOL);

#ifdef DPT_DEBUG
printf(" INV_DATA_OUT_DPT \n");
#endif
DP_INV(		"INV_OUT_DPT",
		DEFAULT_WIDTH,
		DEFAULT_SLICE,
		1,
		"data_out_n[31:0]",
		"data_out_dpt[31:0]",
		EOL);

DP_SAVLOFIG();
exit (0);
}
