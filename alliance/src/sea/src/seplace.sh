#!/bin/sh
#
# $Id: seplace.sh,v 1.1 2002/04/25 16:16:20 jpc Exp $
#
# /------------------------------------------------------------------\
# |                                                                  |
# |        A l l i a n c e   C A D   S y s t e m                     |
# |  S i l i c o n   E n s e m b l e / A l l i a n c e               |
# |                                                                  |
# |  Author    :                      Jean-Paul CHAPUT               |
# |  E-mail    :         alliance-support@asim.lip6.fr               |
# | ================================================================ |
# |  Script    :         "./seplace.sh"                              |
# | **************************************************************** |
# |  U p d a t e s                                                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


# --------------------------------------------------------------------
# Constants definitions.

                VL=0
               LOG="./seplace.log"
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
     alcbanner "SEplace" "1.00" \
               "Alliance Silicon Ensemble Placer Front End" "2000"
     echo ""
  #fi
 }


# --------------------------------------------------------------------
# Function : `print_usage()'.

 print_usage()
 {
   echo ""
   echo ""
   echo "Usage: sea.sh [--verbose|-v] [--very-verbose|-V] [--help|-h]    \\"
   echo "              [--memory=<MEM_SIZE>]                             \\"
   echo "              [--ioc] [--4] [--all] [--partial-place=<partial>] \\"
   echo "              [--rows=<NR>] [--power=<NP>] [--margin=<MARGIN>]  \\"
   echo "              <netlist> <placement>"
   echo ""
   echo "Options:"
   echo "    o [--help]           : Print this help."
   echo "    o [--verbose]        : Be verbose."
   echo "    o [--very-verbose]   : Be very verbose :-)."
   echo "    o [--all]            : Sets the number of routing layers avalai-"
   echo "        bles to the router to 6. By default only 3 will be used."
   echo "    o [--4]              : Uses ALU2, ALU3 & ALU4."
   echo "    o [--memory=<MEM_SIZE>] : Sets the amount of memory used by"
   echo "        Silicon Ensemble. Avalaible <MEM_SIZE> are : "
   echo "          12, 24, 36, 48, 60, 72, 84, 96, 108, 120, 150, 200, 250,"
   echo "          300, 400, 500, 800, 1200, 1600, 1900."
   echo "    o [--partial-place=<partial>] :"
   echo "        The design is already partially placed (there must be a"
   echo "        physical view describing this partial placement)."
   echo "    o [--ioc]            : Load a terminal placement file."
   echo "        (named <netlist>.ioc)."
   echo "    o [--rows=<NR>]      : The placement will have <NR> rows."
   echo "        (by default the layout will have an aspect ratio of 1)"
   echo "    o [--power=<NP>]     : The design will have <NP> vertical power"
   echo "        stripes (each stripe have a width of 35l)."
   echo "    o [--margin=<MARGIN>] :"
   echo "        The amount of free area added, in percentage of the cells"
   echo "        area. The resulting area will be equal to"
   echo "        CELL_AREA * (1 + <MARGIN>/100). Default value : 10."
   echo "    o <netlist>          : The netlist name (mandatory)."
   echo "    o <placement>        : The resulting layout name (mandatory)."
   echo "        By default the same as the netlist name, will erase any"
   echo "        pre-placement."
   echo ""
   echo ""
 }


# --------------------------------------------------------------------
# Function : `log_exec()'.

 log_exec()
 {
   eval "$1" >> $LOG 2>&1
   value="$?"
   if [ $value -ne 0 ]; then
     echo "seplace.sh: \"$1\"."
     echo "seplace.sh:error: command failed."
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
   vecho 1    "     - ALLIANCE_OS     : ${ALLIANCE_OS-not set}"
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
# Function : `get_number()'.

 get_number()
 {
   number=`echo $1 | cut -d '=' -f 2`

  #notDigit=`echo $number | sed 's,[0-9],,'`
  #if [ "$notDigit" != "" ]; then echo $noDigit >&2; exit 1; fi

   echo $number
 }


# --------------------------------------------------------------------
# Function : `get_string()'.

 get_string()
 {
   string=`echo $1 | cut -d '=' -f 2`

   echo $string
 }


# --------------------------------------------------------------------
# Function : `rm_SEplace_tmp()'.

 rm_SEplace_tmp()
 {
   RM_PREV_RUN="n"

   while [ $# -gt 0 ]; do
     case $1 in
       "--previous-run") RM_PREV_RUN="y";;
     esac

     shift
   done

   rm -f ${netlistName}_p.mac
   rm -f ${netlistName}.def
   rm -f ${netlistName}_p.def

   if [ "$RM_PREV_RUN" = "y" ]; then
     rm -f ${placementName}_p.$MBK_IN_PH
   fi
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
        margin="10"
   powerNumber="1"
     rowNumber=""
       argRows=""
     memNumber="150"
   netlistName=""
   partialName=""
 placementName=""
           ioc="n"
         argVL=""
     argLayers="-3"
     argsA2DEF="-V -t -c -e"
     argsDEF2A="-V -p -S -i -b"
       argsSEA="--batch --ansi"
#      argsSEA="-V"


 if [ $# -eq 0 ]; then
   print_usage
   exit 0
 fi


 while [ $# -gt 0 ]; do

   case $1 in
     --keep-tmp)      keepTmp="y";;
     --devel)         argsSEA="$argsSEA --devel";;
     --help)          print_usage; exit 0;;
     --verbose)       VL=1; argVL="-v";;
     --very-verbose)  VL=2; argVL="-V";;
     --ioc)           ioc="y";;
     --all)           argLayers="-6";;
     --4)             argLayers="-4";;
     --partial-place=*) partialName=`get_string $1`;;
     --rows=*)        rowNumber=`get_number $1`
                      if [ $? -ne 0 ]; then
                        echo -n "seplace.sh:error: Bad number `get_number $1`"
                        echo    " in argument \"$1\"."
                        print_usage
                        exit 1
                      fi;;
     --margin=*)      margin=`get_number $1`
                      if [ $? -ne 0 ]; then
                        echo -n "seplace.sh:error: Bad number"
                        echo    " in argument \"$1\"."
                        print_usage
                        exit 1
                      fi;;
     --power=*)       powerNumber=`get_number $1`
                      if [ $? -ne 0 ]; then
                        echo -n "seplace.sh:error: Bad number"
                        echo    " in argument \"$1\"."
                        print_usage
                        exit 1
                      fi;;
     --memory=*)      memNumber=`get_number $1`
                      if [ $? -ne 0 ]; then
                        echo -n "seplace.sh:error: Bad number"
                        echo    " in argument \"$1\"."
                        print_usage
                        exit 1
                      fi;;

     --*) echo "seplace.sh:error: Invalid option \`$1'."
          print_usage; exit 1;;

     -*) lSwitch="$1"; NB=2; CH=`echo $lSwitch | cut -c$NB`

         while [ "$CH" != "" ]; do
           case $CH in
             h) print_usage; exit 0;;
             v) VL=1; argVL="-v";;
             V) VL=2; argVL="-V";;
             k) keepTmp="y";;

             *) echo "sea.sh:error: Invalid option \`$CH'."
                print_usage; exit 1;;
           esac

           NB=`expr $NB + 1`
           CH=`echo $lSwitch | cut -c$NB`
         done;;

     *) # Unmatched argument, this is the netlist name.
        if [ "$netlistName" = "" ]; then
	  netlistName=$1
        else if [ "$placementName" = "" ]; then
	  placementName=$1
        else
          echo "seplace.sh:warning: Ignored extra argument \`$1'."
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
   if [ ! -f "$netlistName".$MBK_IN_LO ]; then
     echo -n "seplace.sh:error: Can't find netlist file"
     echo    " \"$netlistName.$MBK_IN_LO\"."
     print_usage; exit 1
   fi
 fi


 if [ "$placementName" = "" ]; then
   echo "seplace.sh:error: Missing <placement> argument."
   print_usage
   exit 1
 fi


 if [ "$partialName" != "" -a ! -f "$partialName".$MBK_IN_PH ]; then
   echo -n "seplace.sh:error: Can't find pre-placement file"
   echo    " \"$partialName.$MBK_IN_PH\"."
   print_usage; exit 1
 fi


 if [ "$ioc" = "y" -a ! -f "$netlistName.ioc" ]; then
   echo -n "seplace.sh:error: Can't find terminal placement file"
   echo    " \"$netlistName.ioc\"."
   print_usage; exit 1
 fi


 if [ "$partialName" != "" -a "$rowNumber" != "" ]; then
   echo "seplace.sh:warning: As a partial placement is supplied, the row"
   echo "                    number will be ignored."
   print_usage; exit 1
 fi


# --------------------------------------------------------------------
# Script at work.


 MBK_env
 rm_SEplace_tmp --previous-run


 argsA2DEF="$argsA2DEF $argLayers"


 vecho 1 "  o  Netlist file is := \"$netlistName.$MBK_IN_LO\"."
 argsA2DEF="$argsA2DEF --mac-place=${netlistName}_p"


 if [ "$partialName" != "" ]; then
   vecho 1 "  o  Partial placement file is := \"$partialName.$MBK_IN_PH\"."
   argsA2DEF="$argsA2DEF --place=$partialName"
   rowNumber=""
      margin="0"
 else
   vecho 1 "  o  Automatic floorplan generation."
   argsA2DEF="$argsA2DEF -F"
 fi


 if [ "$rowNumber" != "" ]; then
   vecho 2 "     - placement will have $rowNumber rows."
   argsA2DEF="$argsA2DEF --row-number=$rowNumber"
 fi


 if [ "$margin" != "0" ]; then
   vecho 2 "     - Increasing cell area by $margin percent."
   argsA2DEF="$argsA2DEF --margin=$margin"
 fi


 if [ "$powerNumber" != "1" ]; then
   vecho 2 "     - placement will have $powerNumber power."
   argsA2DEF="$argsA2DEF --power=$powerNumber"
 fi


 if [ "$ioc" = "y" ]; then
   vecho 1 "  o  IO placement file is := \"$netlistName.ioc\"."
   argsA2DEF="$argsA2DEF --ioc"
 fi


 vecho 1 "  o  Silicon ensemble memory used := $memNumber."
 argsSEA="$argsSEA --memory $memNumber"


 vecho 2 ""
 vecho 2 "  o  Translating \"$netlistName\" into DEF format."
 log_exec "a2def $argsA2DEF $netlistName"


 vecho 2 ""
 vecho 2 "  o  Silicon Ensemble."


 vecho 2 "     - Running SEA/SE..."
 sea $argVL $argsSEA $netlistName.def ${netlistName}_p.mac
 cat sea.jnl >> $LOG
 rm  sea.jnl


 vecho 2 ""
 vecho 2 "  o  Translating \"$netlistName\" back into Alliance format."
 log_exec "def2a $argsDEF2A ${netlistName}_p ${placementName}"


 if [ "$keepTmp" = "n" ]; then
   rm_SEplace_tmp
 fi


 exit 0
