#!/bin/sh
#
# $Id: sea.sh,v 1.4 2002/10/03 16:53:52 jpc Exp $
#
# /------------------------------------------------------------------\
# |                                                                  |
# |        A l l i a n c e   C A D   S y s t e m                     |
# |  S i l i c o n   E n s e m b l e / A l l i a n c e               |
# |                                                                  |
# |  Author    :                      Jean-Paul CHAPUT               |
# |  E-mail    :         alliance-users@asim.lip6.fr               |
# | ================================================================ |
# |  Script    :         "./sea.sh"                                  |
# | **************************************************************** |
# |  U p d a t e s                                                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


# --------------------------------------------------------------------
# Constants definitions.

                VL=0
    script_SE_init="./se_script_init.mac"
    script_SE_load="./se_script_load.mac"


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
   if [ $VL -ge 1 ]; then
     echo ""
     alcbanner "Sissi" "1.00" "Alliance Silicon Ensemble Front End" "2000"
     echo ""
   fi
 }


# --------------------------------------------------------------------
# Function : `print_usage()'.

 print_usage()
 {
   echo ""
   echo ""
   echo "Usage: sea.sh [--verbose] [--very-verbose] [--help]             \\"
   echo "              [--keep-se] [--keep-tmp] [--no-deflib]            \\"
   echo "              [--memory <SIZE>] [--ansi] [--batch]              \\"
   echo "              [lef_def_file [lef_def_file]]                     \\"
   echo "       sea.sh [-vVhkKNb] [-m <SIZE>]                            \\"
   echo ""
   echo "Options:"
   echo "    o [--help|-h]                : print this help."
   echo "    o [--verbose|-v]             : be verbose."
   echo "    o [--very-verbose|-V]        : be very verbose :-)."
   echo "    o [--batch|-b]               : batch mode."
   echo "    o [--memory|-m <SIZE>]       : uses <SIZE> Mo for memory. Legal"
   echo "        memory sizes are 12, 24, 36, 48, 60, 72, 84, 96, 108, 120,"
   echo "        150, 200, 250, 300, 400, 500, 800, 1200, 1600, 1900."
   echo "    o [--ansi]                   :"
   echo "        run in text mode only (do not start the graphic interface)."
   echo "    o [--keep-se|-K]             : keep SE temporary files."
   echo "    o [--keep-tmp|-k]            : keep SEA temporary files."
   echo "    o [--no-deflib|-N]           :"
   echo "        Do not load defaults technology, libraries and settings"
   echo "        (i.e. sxlib & cmos)."
   echo "    o [lef_def_file]             :"
   echo "        LEF or DEF file to load.  The files should be given with"
   echo "        their extention (.lef or .def)."
   echo ""
   echo ""
 }


# --------------------------------------------------------------------
# Function : `set_SE_env()'.

 set_SE_env()
 {
   OPUS_DIR=/users/soft6/opus/DSM_SE5.2; export  OPUS_DIR

   PATH=${PATH}:${OPUS_DIR}/tools/bin
   PATH=${PATH}:${OPUS_DIR}/tools.sun4v/bin
   PATH=${PATH}:${OPUS_DIR}/tools.sun4v/dfII/bin
   PATH=${PATH}:${OPUS_DIR}/tools.sun4v/dsm/bin
   PATH=${PATH}:${OPUS_DIR}/frame/bin
   PATH=${PATH}:${OPUS_DIR}/bin
   export PATH; hash -r
 }


# --------------------------------------------------------------------
# Function : `load_LEF_TECHNO_NAME()'.

 load_LEF_TECHNO_NAME()
 {
   if [ "$LEF_TECHNO_NAME" != "" ]; then
     vecho 2 "  o  Loading LEF_TECHNO_NAME."
   else
     vecho 2 "  o  Loading default LEF_TECHNO_NAME."
     LEF_TECHNO_NAME=$ALLIANCE_TOP/etc/cmos.lef
   fi

   vecho 2 "     - $LEF_TECHNO_NAME"
   vecho 2 ""
   add_script_SE_load lef $LEF_TECHNO_NAME \
     $script_SE_init > /dev/null 2>&1
 }


# --------------------------------------------------------------------
# Function : `norm_CATA_DIR()'.

 norm_CATA_DIR()
 {
   aDir="$1"

   echo `echo $aDir | sed 's,//,/,g
                           s,/$,,'`
 }


# --------------------------------------------------------------------
# Function : `load_MBK_CATA_LIB()'.

 load_MBK_CATA_LIB()
 {
   vecho 1 "  o  Loading MBK_CATA_LIB."
   NDIR=1
   CATA_dir=`echo ${MBK_CATA_LIB}:END | cut -d ':' -f $NDIR`

   while [ "$CATA_dir" != "END" ]; do
     CATA_dir=`norm_CATA_DIR $CATA_dir`

     if [ "$CATA_dir" != "." ]; then
       CATA_name=`echo $CATA_dir | sed 's,.*/,,'`.lef

       vecho 2 "     - $CATA_dir/${CATA_name}"
       if [ ! -f "$CATA_dir/${CATA_name}" ]; then
         echo "sea:warning: LEF file not found \"$CATA_dir/${CATA_name}\"."
       else
         add_script_SE_load lef "$CATA_dir/${CATA_name}" \
           $script_SE_init > /dev/null 2>&1
       fi
     fi

     NDIR=`expr $NDIR + 1`
     CATA_dir=`echo ${MBK_CATA_LIB}:END | cut -d ':' -f $NDIR`
   done
   vecho 2 ""
 }


# --------------------------------------------------------------------
# Function : `rm_SE_db()'.

 rm_SE_db()
 {
   rm -f LBRARY.* *.dtp *.rpt *.opt.def *.opt.lef '&1'
 }


# --------------------------------------------------------------------
# Function : `rm_SEA_tmp()'.

 rm_SEA_tmp()
 {
   rm -f $script_SE_init $script_SE_load
 }


# --------------------------------------------------------------------
# Function : `clean_sea()'.

 clean_sea()
 {
   if [ "$TAIL_pid" != "" ]; then kill -KILL $TAIL_pid; fi

   if [ "$keepSE"  = "n" ]; then rm_SE_db; fi
   if [ "$keepTmp" = "n" ]; then rm_SEA_tmp; fi
 }


# --------------------------------------------------------------------
# Function : `abort_sea()'.

 abort_sea()
 {
   clean_sea

   exit 1
 }


# --------------------------------------------------------------------
# Function : `add_script_SE_init()'.

 add_script_SE_init()
 {
   aScript="$1"
      aMAC="$2"

   if [ ! -f "$aScript" ]; then
     echo "sea.sh:error: Script not found: \"$aScript\"."
     exit  1
   fi

   echo " EXEC $aScript ;" >> $aMAC
 }


# --------------------------------------------------------------------
# Function : `add_script_SE_load()'.

 add_script_SE_load()
 {
   aType="$1"
   aFile="$2"
    aMAC="$3"

   vecho 2 "     - LEF/DEF/mac to load : \"$aFile\"."

   if [ ! -f "$aFile" ]; then
     echo "sea.sh:error: LEF/DEF/mac not found: \"$aFile\"."
     exit  1
   fi

   case $aType in
     "lef") echo " INPUT LEF FILENAME \"$aFile\" ;" >> $aMAC;;
     "def") echo " INPUT DEF FILENAME \"$aFile\" ;" >> $aMAC;;
     "mac") echo " EXEC \"$aFile\" ;"               >> $aMAC;;
     *) echo "sea.sh(add_script_SE_load):error:"
        echo "  Invalid type \"$aType\"."
        exit 1;;
   esac
 }


# /------------------------------------------------------------------\
# |                                                                  |
# |                  Main Part of the Shell Script                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


# SIGINT  :  2 (^C).
# SIGQUIT :  3
# SIGTERM : 15 (sent from a kill).
 trap abort_sea 2 3 15


# --------------------------------------------------------------------
# Process the command line.


     devel="n"
    keepSE="n"
   keepTmp="n"
    defLib="y"
 argMemory=""
    argJNL="-j=sea.jnl"
     argGD=""
  argBatch=""


 rm -f $script_SE_init
 rm -f $script_SE_load


 while [ $# -gt 0 ]; do

   case $1 in
     --devel)        devel="y";;
     --help)         print_usage; exit 0;;
     --verbose)      VL=1;;
     --very-verbose) VL=2;;
     --keep-se)      keepSE="y";;
     --keep-tmp)     keepTmp="y";;
     --no-deflib)    defLib="n";;
     --ansi)         argGD="-gd=ansi";;
     --batch)        argBatch="-b";;
     --memory)
          if [ $# -ge 2 ]; then
            argMemory="-m=$2"
            shift
          else
            echo "sea.sh:error: Missing <SIZE> argument."
            print_usage; exit 1
          fi;;

     --*) echo "sea.sh:error: Invalid option \`$1'."
          print_usage; exit 1;;

     -*) lSwitch="$1"; NB=2; CH=`echo $lSwitch | cut -c$NB`

         while [ "$CH" != "" ]; do
           case $CH in
             D) devel="y";;
             h) print_usage; exit 0;;
             v) VL=1;;
             V) VL=2;;
             K) keepSE="y";;
             k) keepTmp="y";;
             N) defLib="n";;
             a) argGD="-gd=ansi";;
             b) argBatch="-b";;
             m) if [ $# -ge 2 ]; then
                  argMemory="-m=$2"
                  shift
                else
                  echo "sea.sh:error: Missing <SIZE> argument."
                  print_usage; exit 1
                fi;;

             *) echo "sea.sh:error: Invalid option \`$CH'."
                print_usage; exit 1;;
           esac

           NB=`expr $NB + 1`
           CH=`echo $lSwitch | cut -c$NB`
         done;;

     *) # Unmatched argument, find if this is a LEF, DEF or mac file.
        if echo "$1" | grep '\.lef$' > /dev/null 2>&1; then
          add_script_SE_load lef "$1" $script_SE_load
        else if echo "$1" | grep '\.def$' > /dev/null 2>&1; then
          add_script_SE_load def "$1" $script_SE_load
        else if echo "$1" | grep '\.mac$' > /dev/null 2>&1; then
          add_script_SE_load mac "$1" $script_SE_load
        else
          echo "sea.sh:error: Invalid argument: \"$1\"."
          print_usage; exit 1
        fi; fi; fi
        ;;
   esac

   shift
 done


 title


# --------------------------------------------------------------------
# Consistency checks.


 if [ -z "$ALLIANCE_TOP" ]; then
   echo "sea.sh:error: \$ALLIANCE_TOP is not set, please set your Alliance"
   echo "              environment."
   exit 1
 fi

 if [ "`uname`" != "SunOS" ]; then
   echo "sea.sh:error: Must be run only under Solaris OS."
   echo "              (current OS : \"$ALLIANCE_OS\")"
   exit 1
 fi


 if [ "$devel" = "y" ]; then
   ALLIANCE_TOP="/users/cao/jpc/alliance/Solaris"

   echo "WARNING:"
   echo "WARNING: You are using the developement version."
   echo "WARNING: Resetting \$ALLIANCE_TOP to \"$ALLIANCE_TOP\"."
   echo "WARNING:"
   echo ""
   echo ""
 fi


# --------------------------------------------------------------------
# Script at work.


 rm_SE_db
 set_SE_env

 if [ "$defLib" = "y" ]; then
   vecho 2 "  o  Loading defaults settings."
   vecho 2 ""
   add_script_SE_init $ALLIANCE_TOP/etc/se_defaults.mac $script_SE_init

   load_LEF_TECHNO_NAME
   load_MBK_CATA_LIB
 fi
 if [ -f "$script_SE_load" ]; then
   cat "$script_SE_load" >> "$script_SE_init"
 fi

 vecho 1 "  o  Running Silicon Ensemble."
 if [ ! -f "$script_SE_init" ]; then
   sedsm $argMemory $argJNL $argGD $argBatch &
   SE_pid="$!"

   if [ $VL -ge 2 ]; then
     sleep 2; tail -f sea.jnl &
     TAIL_pid="$!"
   fi
   wait $SE_pid
 else
   if [ "$argBatch" != "" ]; then
     if [ $VL -ge 2 ]; then
       echo  "  o  Silicon Ensemble output follow (in 2s)."

       (sleep 5; exec tail -f sea.jnl) &
       TAIL_pid="$!"
     fi

     SE_pid=`sedsm $argMemory \
                   $argJNL    \
                   $argGD     \
                   $argBatch  \
                   "EXEC $script_SE_init ;" 2>&1 > /dev/null |\
                   sed '1d
                        s,.*\[\([0-9]*\)\] Forking.*,\1,'`
   else
     sedsm $argMemory $argJNL $argGD "EXEC $script_SE_init ;"
   fi

  #echo "$SE_pid" > SE_pid
  #echo "  o  SE has forked."

 fi


 clean_sea


# --------------------------------------------------------------------
# That's all Folks.

 vecho 2 ""
 vecho 2 ""
 exit 0
