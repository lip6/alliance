#include <stdio.h>
#include "genpat.h"

#define UNIT_TIME 50

char *inttostr(entier)
int entier;
{
  char *str;
  str = (char *) mbkalloc (32 * sizeof (char));
  sprintf (str, "%d",entier);
  return(str);
}

main ()
{
int i;
int j;
int vect_date=0; /* this date is an absolute date, in ps */
int decalage; /* conservation de vect_date */


/* WARNING : begin by the end */
int tab_in[] = 
{
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* dt */   //32
0,0,1,1,0,1,  0,0,0,0,0,  0,0,0,1,0,  0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,  /* ir */   //32
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* lo */   //32
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* hi */   //32
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* bar */  //32
0,0,0,0, 0,0,0,0, 0,1,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* epc */  //32 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* ad */   //32
0,0,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* pc */   //32
1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,       
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* ep */        //99
0, /* ovf */									   // 1
0, /* ibe */									   // 1
0, /* ri */									   // 1
0, /* dbe */									   // 1
0, /* adel */									   // 1
0, /* cpu */									   // 1
0, /* ades */									   // 1
0, /* intrqs_sys */								   // 1
0, /* intrqs_br */								   // 1
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* cr */					   //16
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 /* sr */					   //16
};										  //396

int tab_out[] = 
{
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* dt */   //32
0,0,1,1,0,1,  0,0,0,0,0,  0,0,0,1,0,  0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,  /* ir */   //32
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* lo */   //32
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* hi */   //32
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* bar */  //32
0,0,0,0, 0,0,0,0, 0,1,0,0, 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, /* epc */  //32 
0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,1,1,1, 0,0,0,0, 1,1,1,1, /* ad */   //32
0,0,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,1,0,0, /* pc */   //32
0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,       
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* ep */        //99
0, /* ovf */									   // 1
0, /* ibe */									   // 1
0, /* ri */									   // 1
0, /* dbe */									   // 1
0, /* adel */									   // 1
0, /* cpu */									   // 1
0, /* ades */									   // 1
0, /* intrqs_sys */								   // 1
0, /* intrqs_br */								   // 1
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, /* cr */					   //16
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 /* sr */					   //16
};										  //396


DEF_GENPAT("mips_scan");

SETTUNIT ("ns");

/* interface */

DECLAR ("vdd",":2","B",IN,"","");
DECLAR ("vss",":2","B",IN,"","");
DECLAR ("ck", ":2", "B", IN, "", "");
DECLAR ("reset", ":2", "B", IN, "", "");
DECLAR ("frz", ":2", "B", IN, "", "");
DECLAR ("test", ":2", "B", IN, "", "");
DECLAR ("scin",":2","B",IN,"","");
DECLAR ("scout",":2","B",OUT,"","");

ARRAY  (
    "mips1.core.dpt.dt_31",
    "mips1.core.dpt.dt_30",
    "mips1.core.dpt.dt_29",
    "mips1.core.dpt.dt_28",
    "mips1.core.dpt.dt_27",
    "mips1.core.dpt.dt_26",
    "mips1.core.dpt.dt_25",
    "mips1.core.dpt.dt_24",
    "mips1.core.dpt.dt_23",
    "mips1.core.dpt.dt_22",
    "mips1.core.dpt.dt_21",
    "mips1.core.dpt.dt_20",
    "mips1.core.dpt.dt_19",
    "mips1.core.dpt.dt_18",
    "mips1.core.dpt.dt_17",
    "mips1.core.dpt.dt_16",
    "mips1.core.dpt.dt_15",
    "mips1.core.dpt.dt_14",
    "mips1.core.dpt.dt_13",
    "mips1.core.dpt.dt_12",
    "mips1.core.dpt.dt_11",
    "mips1.core.dpt.dt_10",
    "mips1.core.dpt.dt_9",
    "mips1.core.dpt.dt_8",
    "mips1.core.dpt.dt_7",
    "mips1.core.dpt.dt_6",
    "mips1.core.dpt.dt_5",
    "mips1.core.dpt.dt_4",
    "mips1.core.dpt.dt_3",
    "mips1.core.dpt.dt_2",
    "mips1.core.dpt.dt_1",
    "mips1.core.dpt.dt_0",
    ":1","X",SIGNAL,"","dt",0);


ARRAY  (
    "mips1.core.dpt.ir_31",
    "mips1.core.dpt.ir_30",
    "mips1.core.dpt.ir_29",
    "mips1.core.dpt.ir_28",
    "mips1.core.dpt.ir_27",
    "mips1.core.dpt.ir_26",
    "mips1.core.dpt.ir_25",
    "mips1.core.dpt.ir_24",
    "mips1.core.dpt.ir_23",
    "mips1.core.dpt.ir_22",
    "mips1.core.dpt.ir_21",
    "mips1.core.dpt.ir_20",
    "mips1.core.dpt.ir_19",
    "mips1.core.dpt.ir_18",
    "mips1.core.dpt.ir_17",
    "mips1.core.dpt.ir_16",
    "mips1.core.dpt.ir_15",
    "mips1.core.dpt.ir_14",
    "mips1.core.dpt.ir_13",
    "mips1.core.dpt.ir_12",
    "mips1.core.dpt.ir_11",
    "mips1.core.dpt.ir_10",
    "mips1.core.dpt.ir_9",
    "mips1.core.dpt.ir_8",
    "mips1.core.dpt.ir_7",
    "mips1.core.dpt.ir_6",
    "mips1.core.dpt.ir_5",
    "mips1.core.dpt.ir_4",
    "mips1.core.dpt.ir_3",
    "mips1.core.dpt.ir_2",
    "mips1.core.dpt.ir_1",
    "mips1.core.dpt.ir_0",
    ":1","X",SIGNAL,"","ir",0);

ARRAY  (
    "mips1.core.dpt.lo_31",
    "mips1.core.dpt.lo_30",
    "mips1.core.dpt.lo_29",
    "mips1.core.dpt.lo_28",
    "mips1.core.dpt.lo_27",
    "mips1.core.dpt.lo_26",
    "mips1.core.dpt.lo_25",
    "mips1.core.dpt.lo_24",
    "mips1.core.dpt.lo_23",
    "mips1.core.dpt.lo_22",
    "mips1.core.dpt.lo_21",
    "mips1.core.dpt.lo_20",
    "mips1.core.dpt.lo_19",
    "mips1.core.dpt.lo_18",
    "mips1.core.dpt.lo_17",
    "mips1.core.dpt.lo_16",
    "mips1.core.dpt.lo_15",
    "mips1.core.dpt.lo_14",
    "mips1.core.dpt.lo_13",
    "mips1.core.dpt.lo_12",
    "mips1.core.dpt.lo_11",
    "mips1.core.dpt.lo_10",
    "mips1.core.dpt.lo_9",
    "mips1.core.dpt.lo_8",
    "mips1.core.dpt.lo_7",
    "mips1.core.dpt.lo_6",
    "mips1.core.dpt.lo_5",
    "mips1.core.dpt.lo_4",
    "mips1.core.dpt.lo_3",
    "mips1.core.dpt.lo_2",
    "mips1.core.dpt.lo_1",
    "mips1.core.dpt.lo_0",
    ":1","X",SIGNAL,"","lo",0);

ARRAY  (
    "mips1.core.dpt.hi_31",
    "mips1.core.dpt.hi_30",
    "mips1.core.dpt.hi_29",
    "mips1.core.dpt.hi_28",
    "mips1.core.dpt.hi_27",
    "mips1.core.dpt.hi_26",
    "mips1.core.dpt.hi_25",
    "mips1.core.dpt.hi_24",
    "mips1.core.dpt.hi_23",
    "mips1.core.dpt.hi_22",
    "mips1.core.dpt.hi_21",
    "mips1.core.dpt.hi_20",
    "mips1.core.dpt.hi_19",
    "mips1.core.dpt.hi_18",
    "mips1.core.dpt.hi_17",
    "mips1.core.dpt.hi_16",
    "mips1.core.dpt.hi_15",
    "mips1.core.dpt.hi_14",
    "mips1.core.dpt.hi_13",
    "mips1.core.dpt.hi_12",
    "mips1.core.dpt.hi_11",
    "mips1.core.dpt.hi_10",
    "mips1.core.dpt.hi_9",
    "mips1.core.dpt.hi_8",
    "mips1.core.dpt.hi_7",
    "mips1.core.dpt.hi_6",
    "mips1.core.dpt.hi_5",
    "mips1.core.dpt.hi_4",
    "mips1.core.dpt.hi_3",
    "mips1.core.dpt.hi_2",
    "mips1.core.dpt.hi_1",
    "mips1.core.dpt.hi_0",
    ":1","X",SIGNAL,"","hi",0);

ARRAY  (
    "mips1.core.dpt.bar_31",
    "mips1.core.dpt.bar_30",
    "mips1.core.dpt.bar_29",
    "mips1.core.dpt.bar_28",
    "mips1.core.dpt.bar_27",
    "mips1.core.dpt.bar_26",
    "mips1.core.dpt.bar_25",
    "mips1.core.dpt.bar_24",
    "mips1.core.dpt.bar_23",
    "mips1.core.dpt.bar_22",
    "mips1.core.dpt.bar_21",
    "mips1.core.dpt.bar_20",
    "mips1.core.dpt.bar_19",
    "mips1.core.dpt.bar_18",
    "mips1.core.dpt.bar_17",
    "mips1.core.dpt.bar_16",
    "mips1.core.dpt.bar_15",
    "mips1.core.dpt.bar_14",
    "mips1.core.dpt.bar_13",
    "mips1.core.dpt.bar_12",
    "mips1.core.dpt.bar_11",
    "mips1.core.dpt.bar_10",
    "mips1.core.dpt.bar_9",
    "mips1.core.dpt.bar_8",
    "mips1.core.dpt.bar_7",
    "mips1.core.dpt.bar_6",
    "mips1.core.dpt.bar_5",
    "mips1.core.dpt.bar_4",
    "mips1.core.dpt.bar_3",
    "mips1.core.dpt.bar_2",
    "mips1.core.dpt.bar_1",
    "mips1.core.dpt.bar_0",
    ":1","X",SIGNAL,"","bar",0);

ARRAY  (
    "mips1.core.dpt.epc_31",
    "mips1.core.dpt.epc_30",
    "mips1.core.dpt.epc_29",
    "mips1.core.dpt.epc_28",
    "mips1.core.dpt.epc_27",
    "mips1.core.dpt.epc_26",
    "mips1.core.dpt.epc_25",
    "mips1.core.dpt.epc_24",
    "mips1.core.dpt.epc_23",
    "mips1.core.dpt.epc_22",
    "mips1.core.dpt.epc_21",
    "mips1.core.dpt.epc_20",
    "mips1.core.dpt.epc_19",
    "mips1.core.dpt.epc_18",
    "mips1.core.dpt.epc_17",
    "mips1.core.dpt.epc_16",
    "mips1.core.dpt.epc_15",
    "mips1.core.dpt.epc_14",
    "mips1.core.dpt.epc_13",
    "mips1.core.dpt.epc_12",
    "mips1.core.dpt.epc_11",
    "mips1.core.dpt.epc_10",
    "mips1.core.dpt.epc_9",
    "mips1.core.dpt.epc_8",
    "mips1.core.dpt.epc_7",
    "mips1.core.dpt.epc_6",
    "mips1.core.dpt.epc_5",
    "mips1.core.dpt.epc_4",
    "mips1.core.dpt.epc_3",
    "mips1.core.dpt.epc_2",
    "mips1.core.dpt.epc_1",
    "mips1.core.dpt.epc_0",
    ":1","X",SIGNAL,"","epc",0);

ARRAY  (
    "mips1.core.dpt.ad_31",
    "mips1.core.dpt.ad_30",
    "mips1.core.dpt.ad_29",
    "mips1.core.dpt.ad_28",
    "mips1.core.dpt.ad_27",
    "mips1.core.dpt.ad_26",
    "mips1.core.dpt.ad_25",
    "mips1.core.dpt.ad_24",
    "mips1.core.dpt.ad_23",
    "mips1.core.dpt.ad_22",
    "mips1.core.dpt.ad_21",
    "mips1.core.dpt.ad_20",
    "mips1.core.dpt.ad_19",
    "mips1.core.dpt.ad_18",
    "mips1.core.dpt.ad_17",
    "mips1.core.dpt.ad_16",
    "mips1.core.dpt.ad_15",
    "mips1.core.dpt.ad_14",
    "mips1.core.dpt.ad_13",
    "mips1.core.dpt.ad_12",
    "mips1.core.dpt.ad_11",
    "mips1.core.dpt.ad_10",
    "mips1.core.dpt.ad_9",
    "mips1.core.dpt.ad_8",
    "mips1.core.dpt.ad_7",
    "mips1.core.dpt.ad_6",
    "mips1.core.dpt.ad_5",
    "mips1.core.dpt.ad_4",
    "mips1.core.dpt.ad_3",
    "mips1.core.dpt.ad_2",
    "mips1.core.dpt.ad_1",
    "mips1.core.dpt.ad_0",
    ":1","X",SIGNAL,"","ad",0);

ARRAY  (
    "mips1.core.dpt.pc_31",
    "mips1.core.dpt.pc_30",
    "mips1.core.dpt.pc_29",
    "mips1.core.dpt.pc_28",
    "mips1.core.dpt.pc_27",
    "mips1.core.dpt.pc_26",
    "mips1.core.dpt.pc_25",
    "mips1.core.dpt.pc_24",
    "mips1.core.dpt.pc_23",
    "mips1.core.dpt.pc_22",
    "mips1.core.dpt.pc_21",
    "mips1.core.dpt.pc_20",
    "mips1.core.dpt.pc_19",
    "mips1.core.dpt.pc_18",
    "mips1.core.dpt.pc_17",
    "mips1.core.dpt.pc_16",
    "mips1.core.dpt.pc_15",
    "mips1.core.dpt.pc_14",
    "mips1.core.dpt.pc_13",
    "mips1.core.dpt.pc_12",
    "mips1.core.dpt.pc_11",
    "mips1.core.dpt.pc_10",
    "mips1.core.dpt.pc_9",
    "mips1.core.dpt.pc_8",
    "mips1.core.dpt.pc_7",
    "mips1.core.dpt.pc_6",
    "mips1.core.dpt.pc_5",
    "mips1.core.dpt.pc_4",
    "mips1.core.dpt.pc_3",
    "mips1.core.dpt.pc_2",
    "mips1.core.dpt.pc_1",
    "mips1.core.dpt.pc_0",
    ":1","X",SIGNAL,"","pc",0);

ARRAY  (
    "mips1.core.ctl.seq_ep_98",
    "mips1.core.ctl.seq_ep_97",
    "mips1.core.ctl.seq_ep_96",
    "mips1.core.ctl.seq_ep_95",
    "mips1.core.ctl.seq_ep_94",
    "mips1.core.ctl.seq_ep_93",
    "mips1.core.ctl.seq_ep_92",
    "mips1.core.ctl.seq_ep_91",
    "mips1.core.ctl.seq_ep_90",
    "mips1.core.ctl.seq_ep_89",
    "mips1.core.ctl.seq_ep_88",
    "mips1.core.ctl.seq_ep_87",
//    "mips1.core.ctl.seq_ep_86",
//    "mips1.core.ctl.seq_ep_85",
//    "mips1.core.ctl.seq_ep_84",
//    "mips1.core.ctl.seq_ep_83",
//    "mips1.core.ctl.seq_ep_82",
//    "mips1.core.ctl.seq_ep_81",
//    "mips1.core.ctl.seq_ep_80",
//    "mips1.core.ctl.seq_ep_79",
//    "mips1.core.ctl.seq_ep_78",
//    "mips1.core.ctl.seq_ep_77",
//    "mips1.core.ctl.seq_ep_76",
//    "mips1.core.ctl.seq_ep_75",
//    "mips1.core.ctl.seq_ep_74",
//    "mips1.core.ctl.seq_ep_73",
//    "mips1.core.ctl.seq_ep_72",
//    "mips1.core.ctl.seq_ep_71",
//    "mips1.core.ctl.seq_ep_70",
//    "mips1.core.ctl.seq_ep_69",
//    "mips1.core.ctl.seq_ep_68",
//    "mips1.core.ctl.seq_ep_67",
//    "mips1.core.ctl.seq_ep_66",
//    "mips1.core.ctl.seq_ep_65",
//    "mips1.core.ctl.seq_ep_64",
//    "mips1.core.ctl.seq_ep_63",
//    "mips1.core.ctl.seq_ep_62",
//    "mips1.core.ctl.seq_ep_61",
//    "mips1.core.ctl.seq_ep_60",
//    "mips1.core.ctl.seq_ep_59",
//    "mips1.core.ctl.seq_ep_58",
//    "mips1.core.ctl.seq_ep_57",
//    "mips1.core.ctl.seq_ep_56",
//    "mips1.core.ctl.seq_ep_55",
//    "mips1.core.ctl.seq_ep_54",
//    "mips1.core.ctl.seq_ep_53",
//    "mips1.core.ctl.seq_ep_52",
//    "mips1.core.ctl.seq_ep_51",
//    "mips1.core.ctl.seq_ep_50",
//    "mips1.core.ctl.seq_ep_49",
//    "mips1.core.ctl.seq_ep_48",
//    "mips1.core.ctl.seq_ep_47",
//    "mips1.core.ctl.seq_ep_46",
//    "mips1.core.ctl.seq_ep_45",
//    "mips1.core.ctl.seq_ep_44",
//    "mips1.core.ctl.seq_ep_43",
//    "mips1.core.ctl.seq_ep_42",
//    "mips1.core.ctl.seq_ep_41",
//    "mips1.core.ctl.seq_ep_40",
//    "mips1.core.ctl.seq_ep_39",
//    "mips1.core.ctl.seq_ep_38",
//    "mips1.core.ctl.seq_ep_37",
//    "mips1.core.ctl.seq_ep_36",
//    "mips1.core.ctl.seq_ep_35",
//    "mips1.core.ctl.seq_ep_34",
//    "mips1.core.ctl.seq_ep_33",
//    "mips1.core.ctl.seq_ep_32",
//    "mips1.core.ctl.seq_ep_31",
//    "mips1.core.ctl.seq_ep_30",
//    "mips1.core.ctl.seq_ep_29",
//    "mips1.core.ctl.seq_ep_28",
//    "mips1.core.ctl.seq_ep_27",
//    "mips1.core.ctl.seq_ep_26",
//    "mips1.core.ctl.seq_ep_25",
//    "mips1.core.ctl.seq_ep_24",
//    "mips1.core.ctl.seq_ep_23",
//    "mips1.core.ctl.seq_ep_22",
//    "mips1.core.ctl.seq_ep_21",
//    "mips1.core.ctl.seq_ep_20",
//    "mips1.core.ctl.seq_ep_19",
//    "mips1.core.ctl.seq_ep_18",
//    "mips1.core.ctl.seq_ep_17",
//    "mips1.core.ctl.seq_ep_16",
//    "mips1.core.ctl.seq_ep_15",
//    "mips1.core.ctl.seq_ep_14",
//    "mips1.core.ctl.seq_ep_13",
//    "mips1.core.ctl.seq_ep_12",
//    "mips1.core.ctl.seq_ep_11",
//    "mips1.core.ctl.seq_ep_10",
//    "mips1.core.ctl.seq_ep_9",
//    "mips1.core.ctl.seq_ep_8",
//
//    "mips1.core.ctl.seq_ep_7",
//    "mips1.core.ctl.seq_ep_6",
//    "mips1.core.ctl.seq_ep_5",
//    "mips1.core.ctl.seq_ep_4",
 //   "mips1.core.ctl.seq_ep_3",
 //   "mips1.core.ctl.seq_ep_2",
 //   "mips1.core.ctl.seq_ep_1",
 //   "mips1.core.ctl.seq_ep_0",
    ":1","B",SIGNAL,"","ep",0);


DECLAR ("mips1.core.ctl.sts_ovf_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_ibe_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_ri_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_dbe_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_adel_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_cpu_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_ades_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_intrqs_sys_r",":1","B",SIGNAL,"","");
DECLAR ("mips1.core.ctl.sts_intrqs_br_r",":1","B",SIGNAL,"","");

ARRAY (
    "mips1.core.ctl.sts_cr_s_15",
    "mips1.core.ctl.sts_cr_s_14",
    "mips1.core.ctl.sts_cr_s_13",
    "mips1.core.ctl.sts_cr_s_12",
    "mips1.core.ctl.sts_cr_s_11",
    "mips1.core.ctl.sts_cr_s_10",
    "mips1.core.ctl.sts_cr_s_9",
    "mips1.core.ctl.sts_cr_s_8",
    "mips1.core.ctl.sts_cr_s_7",
    "mips1.core.ctl.sts_cr_s_6",
    "mips1.core.ctl.sts_cr_s_5",
    "mips1.core.ctl.sts_cr_s_4",
    "mips1.core.ctl.sts_cr_s_3",
    "mips1.core.ctl.sts_cr_s_2",
    "mips1.core.ctl.sts_cr_s_1",
    "mips1.core.ctl.sts_cr_s_0",
    ":1","X",SIGNAL,"","cr",0);


ARRAY (
    "mips1.core.ctl.sts_sr_s_15",
    "mips1.core.ctl.sts_sr_s_14",
    "mips1.core.ctl.sts_sr_s_13",
    "mips1.core.ctl.sts_sr_s_12",
    "mips1.core.ctl.sts_sr_s_11",
    "mips1.core.ctl.sts_sr_s_10",
    "mips1.core.ctl.sts_sr_s_9",
    "mips1.core.ctl.sts_sr_s_8",
    "mips1.core.ctl.sts_sr_s_7",
    "mips1.core.ctl.sts_sr_s_6",
    "mips1.core.ctl.sts_sr_s_5",
    "mips1.core.ctl.sts_sr_s_4",
    "mips1.core.ctl.sts_sr_s_3",
    "mips1.core.ctl.sts_sr_s_2",
    "mips1.core.ctl.sts_sr_s_1",
    "mips1.core.ctl.sts_sr_s_0",
    ":1","X",SIGNAL,"","sr",0);

ARRAY  (
    "mips1.core.dpt.rf_o_31",
    "mips1.core.dpt.rf_o_30",
    "mips1.core.dpt.rf_o_29",
    "mips1.core.dpt.rf_o_28",
    "mips1.core.dpt.rf_o_27",
    "mips1.core.dpt.rf_o_26",
    "mips1.core.dpt.rf_o_25",
    "mips1.core.dpt.rf_o_24",
    "mips1.core.dpt.rf_o_23",
    "mips1.core.dpt.rf_o_22",
    "mips1.core.dpt.rf_o_21",
    "mips1.core.dpt.rf_o_20",
    "mips1.core.dpt.rf_o_19",
    "mips1.core.dpt.rf_o_18",
    "mips1.core.dpt.rf_o_17",
    "mips1.core.dpt.rf_o_16",
    "mips1.core.dpt.rf_o_15",
    "mips1.core.dpt.rf_o_14",
    "mips1.core.dpt.rf_o_13",
    "mips1.core.dpt.rf_o_12",
    "mips1.core.dpt.rf_o_11",
    "mips1.core.dpt.rf_o_10",
    "mips1.core.dpt.rf_o_9",
    "mips1.core.dpt.rf_o_8",
    "mips1.core.dpt.rf_o_7",
    "mips1.core.dpt.rf_o_6",
    "mips1.core.dpt.rf_o_5",
    "mips1.core.dpt.rf_o_4",
    "mips1.core.dpt.rf_o_3",
    "mips1.core.dpt.rf_o_2",
    "mips1.core.dpt.rf_o_1",
    ":1","B",SIGNAL,"","rf",0);



DECLAR ("rw",":2","B",OUT,"","");
DECLAR ("w",":2","B",OUT,"1 DOWNTO 0","");
DECLAR ("data", ":2", "X", INOUT, "31 DOWNTO 0", "");
DECLAR ("data_adr", ":2", "X", INOUT, "31 DOWNTO 0", "");



//ARRAY  (
//    "mips1.core.dpt.dt_31",
//    "mips1.core.dpt.dt_30",
//    "mips1.core.dpt.dt_29",
//    "mips1.core.dpt.dt_28",
//    "mips1.core.dpt.dt_27",
//    "mips1.core.dpt.dt_26",
//    "mips1.core.dpt.dt_25",
//    "mips1.core.dpt.dt_24",
//    "mips1.core.dpt.dt_23",
//    "mips1.core.dpt.dt_22",
//    "mips1.core.dpt.dt_21",
//    "mips1.core.dpt.dt_20",
//    "mips1.core.dpt.dt_19",
//    "mips1.core.dpt.dt_18",
//    "mips1.core.dpt.dt_17",
//    "mips1.core.dpt.dt_16",
//    "mips1.core.dpt.dt_15",
//    "mips1.core.dpt.dt_14",
//    "mips1.core.dpt.dt_13",
//    "mips1.core.dpt.dt_12",
//    "mips1.core.dpt.dt_11",
//    "mips1.core.dpt.dt_10",
//    "mips1.core.dpt.dt_9",
//    "mips1.core.dpt.dt_8",
//    "mips1.core.dpt.dt_7",
//    "mips1.core.dpt.dt_6",
//    "mips1.core.dpt.dt_5",
//    "mips1.core.dpt.dt_4",
//    "mips1.core.dpt.dt_3",
//    "mips1.core.dpt.dt_2",
//    "mips1.core.dpt.dt_1",
//    "mips1.core.dpt.dt_0",
//    ":1","X",SIGNAL,"","dt",0);

  AFFECT ("0", "vdd", "0b1");
  AFFECT ("0", "vss", "0b0");
  AFFECT ("0", "frz", "0b0");
  AFFECT ("0", "test", "0b0");
  AFFECT ("0","reset","0b1");
  AFFECT ("0", "ck", "0b1");
  AFFECT ("0", "scin", "0b0");

  vect_date += UNIT_TIME;
  AFFECT (inttostr(vect_date), "ck", "0b0");
  vect_date += UNIT_TIME;
  AFFECT (inttostr(vect_date), "ck", "0b1");
  vect_date += UNIT_TIME;
  AFFECT (inttostr(vect_date), "ck", "0b0");
  
for (i=2;i<42;i++)
{
  vect_date=(i*2)*UNIT_TIME;
  AFFECT (inttostr(vect_date),"reset","0b0");
  AFFECT (inttostr(vect_date), "ck", "0b1");
  vect_date=(i*2+1)*UNIT_TIME;
  AFFECT (inttostr(vect_date), "ck", "0b0");
}

LABEL("fin de reset");

decalage = vect_date + UNIT_TIME;
AFFECT (inttostr(decalage), "test", "0b1");

for (i=0;i<396;i++)
{
  vect_date = ( 2 * i ) * UNIT_TIME + decalage;
  AFFECT (inttostr(vect_date), "ck", "0b0");
  AFFECT (inttostr(vect_date), "scin", inttostr(tab_in[i]) );
  
  vect_date = ( 2 * i + 1) * UNIT_TIME + decalage;
  AFFECT (inttostr(vect_date), "ck", "0b1");
  AFFECT (inttostr(vect_date), "scin", inttostr(tab_in[i]) );

}
LABEL("fin_de_l_init");

vect_date += UNIT_TIME;
/* on fait un cycle */

AFFECT (inttostr(vect_date), "test", "0b0");
AFFECT (inttostr(vect_date), "scin", "0b0");
AFFECT (inttostr(vect_date), "ck", "0b0");

vect_date += UNIT_TIME;
AFFECT (inttostr(vect_date), "ck", "0b1");

vect_date += UNIT_TIME;
AFFECT (inttostr(vect_date), "ck", "0b0");
vect_date += UNIT_TIME;
AFFECT (inttostr(vect_date), "ck", "0b1");

LABEL("fin_du_cycle");

decalage = vect_date + UNIT_TIME;

for (i=0;i<396;i++)
{
  if (i == 0) AFFECT (inttostr(( 2 * i ) * UNIT_TIME + decalage), "test", "0b1");
  vect_date = ( 2 * i ) * UNIT_TIME + decalage;
  AFFECT (inttostr(vect_date), "ck", "0b0");
  AFFECT (inttostr(vect_date), "scout", inttostr(tab_out[i]) );
  
  vect_date = ( 2 * i + 1) * UNIT_TIME + decalage;
  AFFECT (inttostr(vect_date), "ck", "0b1");
  AFFECT (inttostr(vect_date), "scout", inttostr(tab_out[i+1]) );

}
















/*
for (i=0; i<16; i++)
    for (j=0; j<16; j++)
    {
        vect_date = 2 * (i*16 + j) * UNIT_TIME;
	AFFECT (inttostr(vect_date), "ck", "0b0");
        AFFECT (inttostr(vect_date), "a", inttostr(i));
        AFFECT (inttostr(vect_date), "b", inttostr(j));
        
	vect_date = (2 * (i*16 + j) + 1) * UNIT_TIME;
	AFFECT (inttostr(vect_date), "ck", "0b1");
        AFFECT (inttostr(vect_date), "a", inttostr(i));
        AFFECT (inttostr(vect_date), "b", inttostr(j));
    }

vect_date += UNIT_TIME;
orig_vect_date = vect_date;

AFFECT (inttostr(vect_date), "sel", "0b1");
AFFECT (inttostr(vect_date), "b", "0b1");


for (i=0; i<16; i++)
{
        vect_date = (2 * i) * UNIT_TIME + orig_vect_date;
	AFFECT (inttostr(vect_date), "ck", "0b0");
        AFFECT (inttostr(vect_date), "a", inttostr(i));
        
	vect_date = (2 * i + 1) * UNIT_TIME + orig_vect_date;
	AFFECT (inttostr(vect_date), "ck", "0b1");
        AFFECT (inttostr(vect_date), "a", inttostr(i));

}

vect_date += UNIT_TIME ;
orig_vect_date = vect_date;

AFFECT (inttostr(vect_date), "sel", "0b1");
AFFECT (inttostr(vect_date), "a", "0b1");

for (i=0; i<16; i++)
{
        vect_date = (2 * i) * UNIT_TIME + orig_vect_date;
	AFFECT (inttostr(vect_date), "ck", "0b0");
        AFFECT (inttostr(vect_date), "b", inttostr(i));
        
	vect_date = (2 * i + 1) * UNIT_TIME + orig_vect_date;
	AFFECT (inttostr(vect_date), "ck", "0b1");
        AFFECT (inttostr(vect_date), "b", inttostr(i));
}*/

SAV_GENPAT ();
exit(0);
}

