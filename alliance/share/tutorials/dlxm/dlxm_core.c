/*
### ----------------------------------------------------------------- ###
#									#
# file	: dlxm_core.c							#
# date	: February 1995							#
# version	: v0.0							#
#									#
# origin	: this description has been developed by CAO-VLSI team	#
#		  at MASI laboratory, University Pierre et Marie Curie	#
#		  URA CNRS 818, Institut Blaise Pascal			#
#		  4 Place Jussieu 75252 Paris Cedex 05 - France		#
# E-mail      :      cao-vlsi@masi.ibp.fr                               #
#									#
# descr.	: Placement file for control and datapath		#
#									#
### ----------------------------------------------------------------- ###
*/

#include <genlib.h>

main()
  {
  DEF_PHFIG ("dlxm_core");
  PLACE ("dlxm_dpt","dpt",NOSYM, 0, 0);
  PLACE ("dlxm_ctl","ctl",NOSYM,
         (WIDTH("dlxm_dpt") - WIDTH("dlxm_ctl")) /2, HEIGHT("dlxm_dpt"));
  DEF_AB (0, 0, 0, 0);
  SAVE_PHFIG();
  exit(0);
  }


