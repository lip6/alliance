### ----------------------------------------------------------------- ###
#									#
# file	: dlxm_dpt.dpr							#
# date	: February 1995							#
# version	: v0.0							#
#									#
# origin	: this description has been developed by CAO-VLSI team	#
#		  at MASI laboratory, University Pierre et Marie Curie	#
#		  URA CNRS 818, Institut Blaise Pascal			#
#		  4 Place Jussieu 75252 Paris Cedex 05 - France		#
# E-mail      :      cao-vlsi@masi.ibp.fr                               #
#									#
# descr.	: Placement file for datapath block connectors		#
#									#
### ----------------------------------------------------------------- ###

DP_LOCON rf_wen NORTH DEFAULT DEFAULT
DP_LOCON rf_aw[4:0] NORTH DEFAULT DEFAULT
DP_LOCON rf_ar[4:0] NORTH DEFAULT DEFAULT
DP_LOCON sr_mode NORTH DEFAULT DEFAULT
DP_LOCON sr_mask NORTH DEFAULT DEFAULT
DP_LOCON sr_cpurst NORTH DEFAULT DEFAULT
DP_LOCON sr_iav NORTH DEFAULT DEFAULT
DP_LOCON sr_ico NORTH DEFAULT DEFAULT
DP_LOCON sr_dav NORTH DEFAULT DEFAULT
DP_LOCON sr_ovr NORTH DEFAULT DEFAULT
DP_LOCON sr_it0 NORTH DEFAULT DEFAULT
DP_LOCON sr_it1 NORTH DEFAULT DEFAULT
DP_LOCON sr_it2 NORTH DEFAULT DEFAULT
DP_LOCON sr_it3 NORTH DEFAULT DEFAULT
DP_LOCON opx_sign NORTH DEFAULT DEFAULT
DP_LOCON opy_codop[5:0] NORTH DEFAULT DEFAULT
DP_LOCON opy_rs[4:0] NORTH DEFAULT DEFAULT
DP_LOCON opy_rdrt[4:0] NORTH DEFAULT DEFAULT
DP_LOCON opy_rd[4:0] NORTH DEFAULT DEFAULT
DP_LOCON opy_tvr NORTH DEFAULT DEFAULT
DP_LOCON opy_sr NORTH DEFAULT DEFAULT
DP_LOCON opy_iar NORTH DEFAULT DEFAULT
DP_LOCON opy_sign NORTH DEFAULT DEFAULT
DP_LOCON alu_test_n NORTH DEFAULT DEFAULT
DP_LOCON alu_nul NORTH DEFAULT DEFAULT
DP_LOCON alu_sign NORTH DEFAULT DEFAULT

DP_LOCON adr_n_0 NORTH DEFAULT DEFAULT
DP_LOCON adr_n_1 NORTH DEFAULT DEFAULT
DP_LOCON adr_n_31 NORTH DEFAULT DEFAULT

DP_LOCON data_in_dpt[31:0] EAST DEFAULT DEFAULT
DP_LOCON data_out_dpt[31:0] EAST DEFAULT DEFAULT
DP_LOCON out_adr[31:0] WEST DEFAULT DEFAULT

DP_POWER 1 50

DP_DEFAB -10 +10

