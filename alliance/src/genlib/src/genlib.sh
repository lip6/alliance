#!/bin/sh
#
# $Id: genlib.sh,v 1.9 2002/10/24 10:28:32 hcl Exp $
#                                                                        
# /------------------------------------------------------------------\
# |                                                                  |
# |        A l l i a n c e   C A D   S y s t e m                     |
# |                      D p G e n                                   |
# |                                                                  |
# |  Author    :                      Jean-Paul CHAPUT               |
# |  E-mail    :         alliance-users@asim.lip6.fr               |
# | ================================================================ |
# |  sh script :         "./dpgen/dpgen.sh"                          |
# | **************************************************************** |
# |  U p d a t e s                                                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


# /------------------------------------------------------------------\
# |                                                                  |
# |                      Functions Definitions                       |
# |                                                                  |
# \------------------------------------------------------------------/


# --------------------------------------------------------------------
# Function  :  `print_usage()'.

 print_usage()
 {
   echo ""
   echo "  usage: genlib [-cdlknv] [--keep-exec] [--no-exec] [--no-rm-core] \\"
   echo "               [--keep-log] [--verbose] \\"
   echo "               <program> [-e [args]]"
   echo ""
   echo "  Options:"
   echo "    [-c|--no-rm-core]    : Don't remove core file."
   echo "    [-l|--keep-log]      : Keep log file (.grr) after execution."
   echo "    [-k|--keep-exec]     : Keep binary after execution."
   echo "    [-m|--keep-makefile] : Keep makefile after execution."
   echo "    [-n|--no-exec]       : Do not execute the binary."
   echo "    [-v|--verbose]       : Issue more information."
   echo "    [-e [args]]          : [args] are passed to the binary."
   echo ""
 }


# --------------------------------------------------------------------
# Function  :  `vecho()'.

 vecho()
 { 
   ECHOFLAGS="" 
   if [ $1 -gt $VERBOSE ]; then return 0; fi
   if [ "$2" = "-n" ]; then
     ECHOFLAGS="-n"; shift;
   fi
   echo $ECHOFLAGS "$2" 2>&1 | tee -a $CSOURCE.grr
 }


# /------------------------------------------------------------------\
# |                                                                  |
# |                  Main Part of the Shell Script                   |
# |                                                                  |
# \------------------------------------------------------------------/

 if [ -z "$ALLIANCE_TOP" ]; then
   alcbanner "GenLib" "4.0" "Procedural Generation Language" "1991"
 else
   ${ALLIANCE_TOP}/bin/alcbanner "GenLib" "4.0" "Procedural Generation Language" "1991"
 fi



# --------------------------------------------------------------------
# Process the command line.

# If no arguments, print usage.
 if [ $# -eq 0 ]; then print_usage; exit 0; fi


  KEEP_BIN="n"
  KEEP_LOG="n"
 KEEP_CORE="n"
 KEEP_MAKEFILE="n"
   VERBOSE=0
   EXECUTE="y"
 ARGUMENTS=""
   PROGRAM="not set"
     DEVEL="n"


# Parse the arguments.
 while [ $# -gt 0 ]; do

   case $1 in

     --no-rm-core) KEEP_CORE="y";;
     --keep-makefile)  KEEP_MAKEFILE="y";;
     --keep-exec)  KEEP_BIN="y";;
     --keep-log)   KEEP_LOG="y";;
     --no-exec)    EXECUTE="n";;
     --verbose)    VERBOSE=1;;
     --devel)      DEVEL="y";;

     -*) NB=2; CH=`echo $1 | cut -c$NB`
	 while [ "$CH" != "" ]; do

	   case $CH in

             m) KEEP_MAKEFILE="y";;
             c) KEEP_CORE="y";;
             k) KEEP_BIN="y";;
             l) KEEP_LOG="y";;
             n) EXECUTE="n";;
             v) VERBOSE=1;;
             d) DEVEL="y";;
             e) echo "genlib:warning: misplaced -e option.";
                print_usage; exit 1;;
             *) echo "genlib:warning: Invalid option \`$CH' (ignored)."
                print_usage;;

	   esac

           NB=`expr $NB + 1`
           CH=`echo $1 | cut -c$NB`

         done;;

     *)            PROGRAM=$1; shift; break;;

   esac

   shift

 done


# Parse optionals program arguments.
 if [ $# -gt 0 ]; then
   if [ "$1" != "-e" ]; then

     echo "genlib:warning: ignored \`$1' parameter after program name."
     print_usage

   else

     shift; ARGUMENTS="$*"

   fi
 fi


# Check arguments coherency.
 if [ "$PROGRAM" = "not set" ]; then

   echo "genlib:error: Missing <program> argument."
   print_usage; exit 1;

 fi

 CSOURCE=`basename $PROGRAM .c`


# Removing previously generated files.
 rm -f $CSOURCE.grr $CSOURCE.o > /dev/null 2>&1


# --------------------------------------------------------------------
# Print the current MBK environment.

 if [ ! -z "$ALLIANCE_TOP" ]; then ALLIANCE_TOP=$ALLIANCE_TOP; fi

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


# --------------------------------------------------------------------
# Development hacks.

 if [ "$DEVEL" = "y" ]; then
   ALLIANCE_INSTALL_DIR="__ALLIANCE_INSTALL_DIR__"
   ALLIANCE_DEVEL_INCLUDE="-I${ALLIANCE_INSTALL_DIR}/include"
   ALLIANCE_DEVEL_LIB="-L${ALLIANCE_INSTALL_DIR}/lib"
   export ALLIANCE_INSTALL_DIR ALLIANCE_DEVEL_INCLUDE ALLIANCE_DEVEL_LIB

   vecho 0    "  o  Switching to development environment :"
   vecho 0    "     - ALLIANCE_INSTALL_DIR : ${ALLIANCE_INSTALL_DIR-not set}"
 fi


# --------------------------------------------------------------------
# Build and execute the Makefile.

 vecho 1 "  o  Program generation."
 vecho 1 "     - Creating Makefile."

 MAKEFILE="Makefile-$$"


# --------------------------------------------------------------------
# Start of the Makefile text.

cat << EOF > $MAKEFILE
  ALLIANCE_INCLUDE = $ALLIANCE_TOP/include
  ALLIANCE_LIB     = $ALLIANCE_TOP/lib

  LOCAL_ALC_IDIR = $ALLIANCE_DEVEL_INCLUDE -I\$(ALLIANCE_INCLUDE)
  LOCAL_ALC_DEFS = 
  LOCAL_ALC_INCS = \$(LOCAL_ALC_IDIR) \$(LOCAL_ALC_DEFS)

  LOCAL_ALC_LDIR = $ALLIANCE_DEVEL_LIB -L\$(ALLIANCE_LIB)
  LOCAL_ALC_LVBE = -lBvl \\
                   -lBeh \\
                   -lBhl \\
                   -lLog
  LOCAL_ALC_LMGN = -lMgn
  LOCAL_ALC_LMUT = -lMut
  LOCAL_ALC_LMLO = -lMlu \\
                   -lMlo \\
                   -lRcn
  LOCAL_ALC_LMPH = -lMpu \\
                   -lMph
  LOCAL_ALC_LIBS = \$(LOCAL_ALC_LDIR) \\
                   \$(LOCAL_ALC_LVBE) \\
                   \$(LOCAL_ALC_LMGN) \\
                   \$(LOCAL_ALC_LMLO) \\
                   \$(LOCAL_ALC_LMPH) \\
                   \$(LOCAL_ALC_LMUT)

  LOCAL_STD_LIBS = -lm

#  C Flags for normal compilation.
 LOCAL_CFLAGS = \$(CFLAGS) -c -g
 LOCAL_OFLAGS = \$(OFLAGS) -o


all: $CSOURCE

$CSOURCE: $CSOURCE.o
	\$(CC) \$(LOCAL_OFLAGS) \$@ \$? \$(LOCAL_ALC_LIBS) \$(LOCAL_STD_LIBS)
	rm -f \$?

$CSOURCE.o: $CSOURCE.c
	\$(CC) \$(LOCAL_CFLAGS) \$(LOCAL_ALC_INCS) \$?


clean:
	rm -f $CSOURCE $CSOURCE.o $CSOURCE.exe


EOF


# --------------------------------------------------------------------
# End of the Makefile text.


 vecho 1 "     - Running Makefile."

 make -f $MAKEFILE ALLIANCE_TOP=$ALLIANCE_TOP \
         CSOURCE=$CSOURCE clean all >> $CSOURCE.grr 2>&1

 exit_code=$?

 if [ $exit_code -ne 0 ]; then

   if [ $KEEP_MAKEFILE != "y" ]; then rm -f $MAKEFILE; fi

   echo "genlib:error: Compilation failed."
   echo "error messages:"
   cat $CSOURCE.grr
   exit $exit_code

 else

   if [ "$EXECUTE" = "y" ]; then

     vecho 1 "  o  Execute program..."
     ./$CSOURCE $ARGUMENTS; exit_code=$?

     vecho 1 "     - Exit code := $exit_code."
     if [ $exit_code -ne 0 ]; then
       if [ $KEEP_MAKEFILE != "y" ]; then rm -f $MAKEFILE; fi
       echo "genlib:error: Execution failed."
     fi

     if [ "$KEEP_CORE" != "y" ]; then rm -f core >> $CSOURCE.grr 2>&1; fi

   else
     vecho 1 "  o  Execution step skipped."
     exit_code=0
   fi

 fi


 vecho 1 ""
 vecho 1 "  o  Remove temporary file."

 rm -f $MAKEFILE

 if [ "$KEEP_BIN"  != "y" ]; then rm -f $CSOURCE $CSOURCE.exe >> $CSOURCE.grr 2>&1; fi

 if [ \( $exit_code -eq 0 \) -a \( "$KEEP_LOG" != "y" \) ]; then
   rm -f $CSOURCE.grr > /dev/null 2>&1
 fi

 echo ""


 exit $exit_code
