#!/bin/sh
#
# $Id: seroute.sh,v 1.3 2002/09/30 16:21:18 czo Exp $
#
# /------------------------------------------------------------------\
# |                                                                  |
# |        A l l i a n c e   C A D   S y s t e m                     |
# |  S i l i c o n   E n s e m b l e / A l l i a n c e               |
# |                                                                  |
# |  Author    :                      Jean-Paul CHAPUT               |
# |  E-mail    :         alliance-users@asim.lip6.fr               |
# | ================================================================ |
# |  Script    :         "./seroute.sh"                              |
# | **************************************************************** |
# |  U p d a t e s                                                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


# --------------------------------------------------------------------
# Constants definitions.

                VL=0
               LOG="./seroute.log"
         DEF_UNITS=100


# /------------------------------------------------------------------\
# |                                                                  |
# |                      Functions Definitions                       |
# |                                                                  |
# \------------------------------------------------------------------/
#
# --------------------------------------------------------------------
#  Function : "vecho()".

 vecho()
 {
   if [ $1 -le $VL ]; then
     if [ "$2" = "-n" ]; then
       printf "$3"
     else
       echo   "$2"
     fi
   fi
 }


# --------------------------------------------------------------------
# Function : "title()".

 title()
 {
  #if [ $VL -ge 1 ]; then
  #  echo ""
     alcbanner "SEroute" "1.00" \
               "Alliance Silicon Ensemble Router Front End" "2000"
     echo ""
  #fi
 }


# --------------------------------------------------------------------
# Function : `print_usage()'.

 print_usage()
 {
   echo ""
   echo ""
   echo "Usage: seroute [--verbose|-v] [--very-verbose|-V] [--help|-h]    \\"
   echo "               [--memory=<MEM_SIZE>]                             \\"
   echo "               [--core] [--ring] [--ioc] [--all] [--4]           \\"
   echo "               [--trust-orient] [--no-power] [--generator]       \\"
   echo "               [--place=<placement>]                             \\"
   echo "               <netlist> <layout>"
   echo ""
   echo "Options:"
   echo "    o [--help]           : Print this help."
   echo "    o [--verbose]        : Be verbose."
   echo "    o [--very-verbose]   : Be very verbose :-)."
   echo "    o [--memory=<MEM_SIZE>] : Sets the amount of memory used by"
   echo "        Silicon Ensemble. Avalaible <MEM_SIZE> are : "
   echo "          12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 150, 200, 250,"
   echo "          300, 400, 500, 800, 1200, 1600, 1900."
   echo "    o [--ioc]            : Load a terminal placement file."
   echo "        (named <netlist>.ioc)."
   echo "    o [--all]            : Use all 6 layers for routing."
   echo "    o [--4]              : Use only ALU2 to ALU4 for routing."
   echo "    o [--core]           : Route terminals to the edge of the"
   echo "        abutment-box."
   echo "    o [--ring]           : Produce a layout suitable for ring."
   echo "        This option implies \"--core\"."
   echo "    o [--trust-orient]   : Trust MBK orientation, i.e. allow cells"
   echo "        without regular vdd/vss power rails."
   echo "    o [--no-power]       : Do not route power rails, this implies"
   echo "        that they must be pre-routed by the user."
   echo "    o [--generator]      : Route a user build block like a RAM"
   echo "        or a ROM. This allow the generator leaf cells not to"
   echo "        comply strictly to the SxLib requirements. This option is"
   echo "        equivalent to --no-power --trust-orient."
   echo "    o [--place=<placement>] : The name of the placement file,"
   echo "        (if different from the netlist)."
   echo "    o <netlist>          : The netlist name (mandatory)."
   echo "    o <layout>           : The resulting layout name (mandatory)."
   echo ""
   echo ""
 }


# --------------------------------------------------------------------
# Function : `get_number()'.

 get_number()
 {
   number=`echo $1 | cut -d '=' -f 2`

  #notDigit=`echo $number | sed 's,[0-9],,'`
  #if [ "$notDigit" != "" ]; then echo $noDigit >&2; exit 1; fi

   echo $number
 }


# --------------------------------------------------------------------
# Function : `log_exec()'.

 log_exec()
 {
   eval "$1" >> $LOG 2>&1
   value="$?"
   if [ $value -ne 0 ]; then
     echo "seroute.sh: \"$1\"."
     echo "seroute.sh:error: command failed."
     exit  1
   fi
   return $value
 }


# --------------------------------------------------------------------
# Function : `MBK_env()'.

 MBK_env()
 {
   vecho 1    "  o  Current Alliance environment:"
   vecho 1    "     - ALLIANCE_TOP    : ${ALLIANCE_TOP-not set}"
   vecho 1    "  o  Current MBK environment:"
   vecho 1    "     - MBK_IN_LO       : ${MBK_IN_LO-not set}"
   vecho 1    "     - MBK_OUT_LO      : ${MBK_OUT_LO-not set}"
   vecho 1    "     - MBK_IN_PH       : ${MBK_IN_PH-not set}"
   vecho 1    "     - MBK_OUT_PH      : ${MBK_OUT_PH-not set}"
   vecho 1    "     - MBK_WORK_LIB    : ${MBK_WORK_LIB-not set}"
   vecho 1 -n "     - MBK_CATA_LIB    : "

   if [ -z "$MBK_CATA_LIB" ]; then
     vecho 1 "not set"
   else
     STRING=`echo ${MBK_CATA_LIB} |                            \
               awk 'BEGIN { FS=":"; }                          \
                    { for( i=1; i<=NF; i++) {                  \
                        printf("%s\n", $i);                    \
                        if (i < NF)                            \
                          printf("                         "); \
                      }                                        \
                    }' -`
     vecho 1 "$STRING"
   fi

   vecho 1    "     - MBK_CATAL_NAME  : ${MBK_CATAL_NAME-not set}"
   vecho 1    "     - MBK_VDD         : ${MBK_VDD-not set}"
   vecho 1    "     - MBK_VSS         : ${MBK_VSS-not set}"
   vecho 1    "     - RDS_TECHNO_NAME : ${RDS_TECHNO_NAME-not set}"
   vecho 1    ""
   vecho 1    ""
 }


# --------------------------------------------------------------------
# Function : `rm_SEroute_tmp()'.

 rm_SEroute_tmp()
 {
   RM_PREV_RUN="n"

   while [ $# -gt 0 ]; do
     case $1 in
       "--previous-run") RM_PREV_RUN="y";;
     esac

     shift
   done

   rm -f ${netlistName}_r.mac
   rm -f ${netlistName}.def
   rm -f ${netlistName}_r.def

   if [ "$RM_PREV_RUN" = "y" ]; then
     rm -f ${layoutName}_r.$MBK_IN_PH
   fi
 }


# --------------------------------------------------------------------
# Function : `get_string()'.

 get_string()
 {
   string=`echo $1 | cut -d '=' -f 2`

   echo $string
 }


# /------------------------------------------------------------------\
# |                                                                  |
# |                  Main Part of the Shell Script                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


 rm -f $LOG


 title


 if [ "`uname`" != "SunOS" ]; then
   echo "seplace.sh:error: This progam must be run under Solaris."
   exit 1
 fi


# --------------------------------------------------------------------
# Process the command line.


       keepTmp="n"
     memNumber="150"
   netlistName=""
 placementName=""
    layoutName=""
           ioc="n"
          core="n"
          ring="n"
         argVL=""
     argLayers="-3"
    argFillTie=""
     argsA2DEF="-V -p"
     argsDEF2A="-V -p"
#    argsA2DEF="-V -p -E"
#    argsDEF2A="-V -p -S"
       argsSEA="--batch --ansi"
#      argsSEA="-V"


 if [ $# -eq 0 ]; then
   print_usage
   exit 0
 fi


 while [ $# -gt 0 ]; do

   case $1 in
     --devel)         argsSEA="$argsSEA --devel";;
     --help)          print_usage; exit 0;;
     --verbose)       VL=1; argVL="-v";;
     --very-verbose)  VL=2; argVL="-V";;
     --keep-tmp)      keepTmp="y";;
     --ioc)           ioc="y";;
     --core)          core="y";;
     --ring)          ring="y"; core="y";;
     --all)           argLayers="-6";;
     --4)             argLayers="-4";;
     --fill-tie)      argFillTie="-t";;
     --trust-orient)  argsA2DEF="$argsA2DEF -T";;
     --no-power)      argsA2DEF="$argsA2DEF -n";;
     --generator)     argsA2DEF="$argsA2DEF -T -n";;
     --place=*)       placementName=`get_string $1`;;
     --memory=*)      memNumber=`get_number $1`
                      if [ $? -ne 0 ]; then
                        echo -n "seplace.sh:error: Bad number"
                        echo    " in argument \"$1\"."
                        print_usage
                        exit 1
                      fi;;

     --*) echo "seroute.sh:error: Invalid option \`$1'."
          print_usage; exit 1;;

     -*) lSwitch="$1"; NB=2; CH=`echo $lSwitch | cut -c$NB`

         while [ "$CH" != "" ]; do
           case $CH in
             h) print_usage; exit 0;;
             k) keepTmp="y";;
             v) VL=1; argVL="-v";;
             V) VL=2; argVL="-V";;

             *) echo "sea.sh:error: Invalid option \`$CH'."
                print_usage; exit 1;;
           esac

           NB=`expr $NB + 1`
           CH=`echo $lSwitch | cut -c$NB`
         done;;

     *) # Unmatched argument, this is the netlist name.
        if [ "$netlistName" = "" ]; then
	  netlistName=$1
        else if [ "$layoutName" = "" ]; then
	  layoutName=$1
        else
          echo "seroute.sh:warning: Ignored extra argument \`$1'."
        fi; fi
        ;;
   esac

   shift
 done


# --------------------------------------------------------------------
# Consistency checks.


 if [ "$netlistName" = "" ]; then
   echo "seplace.sh:error: Missing <netlist> argument."
   print_usage
   exit 1
 else
    if [ ! -f "$netlistName.$MBK_IN_LO" ]; then
      echo -n "seroute.sh:error: Can't find netlist file"
      echo    " \"$netlistName.$MBK_IN_LO\"."
      print_usage; exit 1
    fi
 fi


 if [ "$placementName" = "" ]; then placementName=$netlistName; fi
    
 if [ ! -f "$placementName".$MBK_IN_PH ]; then
   echo -n "seroute.sh:error: Can't find placement file"
   echo    " \"$placementName.$MBK_IN_PH\"."
   print_usage; exit 1
 fi

 argsA2DEF="$argsA2DEF --place=$placementName"


 if [ "$layoutName" = "" ]; then
   echo "seplace.sh:error: Missing <layout> argument."
   print_usage
   exit 1
 fi


 if [ "$ioc" = "y" -a ! -f "$netlistName.ioc" ]; then
   echo -n "seroute.sh:error: Can't find terminal placement file"
   echo    " \"$netlistName.ioc\"."
   print_usage; exit 1
 fi


# --------------------------------------------------------------------
# Script at work.


 MBK_env
 rm_SEroute_tmp --previous-run


 argsA2DEF="$argsA2DEF $argLayers $argFillTie"


 vecho 1 "  o  Netlist file is := \"$netlistName.$MBK_IN_LO\"."
 argsA2DEF="$argsA2DEF --mac-route=${netlistName}_r"


 if [ "$ioc" = "y" ]; then
   vecho 1 "  o  IO placement file is := \"$netlistName.ioc\"."
   argsA2DEF="$argsA2DEF --ioc"
 fi


 if [ "$core" = "y" ]; then
   vecho 1 "  o  The design will have pins."
   argsA2DEF="$argsA2DEF -c"
 fi


 if [ "$ring" = "y" ]; then
   vecho 1 "  o  Merge power terminals for ring."
   argsDEF2A="$argsDEF2A -r"
 fi


 vecho 1 "  o  Silicon ensemble memory used := $memNumber."
 argsSEA="$argsSEA --memory $memNumber"


 vecho 2 ""
 vecho 2 "  o  Translating \"$netlistName\" into DEF format."
 log_exec "a2def $argsA2DEF $netlistName"


 vecho 2 ""
 vecho 2 "  o  Silicon Ensemble."


 vecho 2 "     - Running SEA/SE..."
 sea $argVL $argsSEA $netlistName.def ${netlistName}_r.mac
 cat sea.jnl >> $LOG
 rm  sea.jnl


 vecho 2 ""
 vecho 2 "  o  Translating \"$netlistName\" back into Alliance format."
 log_exec "def2a $argsDEF2A ${netlistName}_r ${layoutName}"


 if [ "$keepTmp" = "n" ]; then
   rm_SEroute_tmp
 fi


 exit 0
