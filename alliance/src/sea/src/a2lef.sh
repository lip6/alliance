#!/bin/sh
#
# $Id: a2lef.sh,v 1.3 2003/04/02 13:42:18 jpc Exp $
#
# /------------------------------------------------------------------\
# |                                                                  |
# |        A l l i a n c e   C A D   S y s t e m                     |
# |  S i l i c o n   E n s e m b l e / A l l i a n c e               |
# |                                                                  |
# |  Author    :                      Jean-Paul CHAPUT               |
# |  E-mail    :         alliance-users@asim.lip6.fr               |
# | ================================================================ |
# |  Script    :         "./a2LEF.sh"                                |
# | **************************************************************** |
# |  U p d a t e s                                                   |
# |                                                                  |
# \------------------------------------------------------------------/
#


# --------------------------------------------------------------------
# Constants definitions.

                VL=0
          techFile=""
           lLibDir=""
           lefFile=""
       sx2lef_args=""
          cellName=""


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
     alcbanner "a2LEF" "V.RR" "Alliance Library to LEF converter" "2000"
     echo ""
   fi
 }


# --------------------------------------------------------------------
# Function : `print_usage()'.

 print_usage()
 {
   echo ""
   echo ""
   echo "Usage: a2LEF.sh [--verbose] [--very-verbose] [--help]             \\"
   echo "                [--allow-offgrid] [--fake-power]                  \\"
   echo "                <--techno <tech_file> >                           \\"
   echo "                <--library <lib_dir> [--library <lib_dir>]>       \\"
   echo "                <--lef <lef_file> >                               \\"
   echo "                <--cell <cell_name> >"
   echo "       a2LEF.sh [-vVhop] <-t <tech_file> >                        \\"
   echo "                         <-l <lib_dir> [-l <lib_dir>]>            \\"
   echo "                         <-L <lef_file> >                         \\"
   echo "                         <-c <cell_name> > "
   echo ""
   echo "Options:"
   echo "    o [--help|-h]                : print this help."
   echo "    o [--verbose|-v]             : be verbose."
   echo "    o [--very-verbose|-V]        : be very verbose :-)."
   echo "    o [--allow-offgrid|-o]       : allow terminals to be offgrid."
   echo "    o [--fake-power|-p]          : do not attempt to generate"
   echo "        PORT geometries for power pins."
   echo "    o <--techno|-t <tech_file> > : location of the LEF techno header."
   echo "    o <--library|-l <lib_dir> >  : Alliance library directory."
   echo "    o <--lef|-L <lef_file> >     : output LEF file."
   echo "    o <--cell|-c <cell_name> >   : process a single (macro) cell."
   echo ""
   echo ""
 }


# --------------------------------------------------------------------
# Function : `setTechno()'.

 setTechno()
 {
   aTechFile="$1"
        aLEF="$2"

   rm -f $aLEF > /dev/null 2>&1
   cp $aTechFile $aLEF
 }


# --------------------------------------------------------------------
# Function : `buildLib()'.

 buildLib()
 {
   aLibDir="$1"
      aLEF="$2"

   if [ ! -d $aLibDir ]; then
     echo "a2LEF:error: Library \"$aLibDir\" doesn't exist."
     exit 1
   fi


   MBK_CATA_LIB="$aLibDir"; export MBK_CATA_LIB


   lCellAp=`(cd $aLibDir; \ls *.vbe)`

   if [ "$lCellAp" = "" ]; then
     echo "a2LEF:error: Library \"$aLibDir\" is empty."
     exit 1
   fi


   libName="`echo $aLibDir | sed 's,.*/,,'`"
   vecho 1 ""
   vecho 1 "  o  Processing library \"$libName\"."


     success="$libName.success"
      failed="$libName.failed"
         log="$libName.log"
    failedNB=0
   successNB=0

   rm -f $success $failed $log > /dev/null 2>&1

   for cellAp in $lCellAp; do

     cell=`echo $cellAp | sed 's,\.vbe$,,'`
     vecho 2 -n "     - \"$cell\"... "

     case $cell in
       *_sp) pxlib2lef -V $cell              >> $log 2>&1; RV=$?;;
          *) sxlib2lef -V $sx2lef_args $cell >> $log 2>&1; RV=$?;;
     esac
     if [ $RV -ne 0 ]; then

       failedNB=`expr $failedNB + 1`
       echo "$cell" >> $failed

       vecho 2 "KO."

     else

       echo  ""         >> $aLEF
       cat    $cell.lef >> $aLEF
       rm  -f $cell.lef

       successNB=`expr $successNB + 1`
       echo "$cell"    >> $success

       vecho 2 "OK."

     fi

   done


   vecho 1 "  o  $successNB successful cells."


   if [ $failedNB -gt 0 ]; then

     echo ""
     echo "a2LEF:error:"
     echo "  $failedNB failed cells in library \"$libName\"."
     echo "  Please read \"$log\" for detailed explanations."
     exit 1

   fi
 }


# --------------------------------------------------------------------
# Function : `openLib()'.

 openLib()
 {
   aLEF="$1"

   echo ""                            >> $aLEF
   echo "VERSION            5.2 ;"    >> $aLEF
   echo "NAMECASESENSITIVE  ON ;"     >> $aLEF
   echo "BUSBITCHARS        \"()\" ;" >> $aLEF
   echo "BIVIDERCHAR        \".\" ;"  >> $aLEF
   echo ""                            >> $aLEF
 }


# --------------------------------------------------------------------
# Function : `closeLib()'.

 closeLib()
 {
   aLEF="$1"

   echo ""            >> $aLEF
   echo "END LIBRARY" >> $aLEF
 }


# /------------------------------------------------------------------\
# |                                                                  |
# |                  Main Part of the Shell Script                   |
# |                                                                  |
# \------------------------------------------------------------------/
#
# --------------------------------------------------------------------
# Process the command line.

 while [ $# -gt 0 ]; do

   case $1 in

     --help)          print_usage; exit 0;;
     --verbose)       VL=1;;
     --very-verbose)  VL=2;;
     --allow-offgrid) sx2lef_args="$sx2lef_args -o";;
     --fake-power)    sx2lef_args="$sx2lef_args -p";;

     --techno)
             if [ $# -ge 2 ]; then
               techFile="$2"
               shift
             else
               echo "a2LEF.sh:error: Missing <tech_file> argument."
               print_usage; exit 1
             fi;;

     --library)
             if [ $# -ge 2 ]; then
               lLibDir="$lLibDir $2"
               shift
             else
               echo "a2LEF.sh:error: Missing <libDir> argument."
               print_usage; exit 1
             fi
             if [ ! -z "$cellName" ]; then
               echo -n "a2LEF.sh:error: --library and --cell options are"
               echo    " mutually exclusive."
               print_usage; exit 1
             fi;;

     --lef)
             if [ $# -ge 2 ]; then
               lefFile="$2"
               shift
             else
               echo "a2LEF.sh:error: Missing <lef_file> argument."
               print_usage; exit 1
             fi;;

     --cell)
             if [ $# -ge 2 ]; then
               cellName="$2"
               shift
             else
               echo "a2LEF.sh:error: Missing <cell_name> argument."
               print_usage; exit 1
             fi
             if [ ! -z "$lLibDir" ]; then
               echo -n "a2LEF.sh:error: --library and --cell options are"
               echo    " mutually exclusive."
               print_usage; exit 1
             fi;;

     --*) echo "a2LEF.sh:error: Invalid argument \`$1'."
          print_usage; exit 1;;

     -*) lSwitch="$1"; NB=2; CH=`echo $lSwitch | cut -c$NB`

         while [ "$CH" != "" ]; do

           case $CH in

             h) print_usage; exit 0;;
             v) VL=1;;
             V) VL=2;;
             o) sx2lef_args="$sx2lef_args -o";;
             p) sx2lef_args="$sx2lef_args -p";;

             t) if [ $# -ge 2 ]; then
                  techFile="$2"
                  shift
                else
                  echo "a2LEF.sh:error: Missing <tech_file> argument."
                  print_usage; exit 1
                fi;;

             l) if [ $# -ge 2 ]; then
                  lLibDir="$lLibDir $2"
                  shift
                else
                  echo "a2LEF.sh:error: Missing <libDir> argument."
                  print_usage; exit 1
                fi
                if [ ! -z "$cellName" ]; then
                  echo -n "a2LEF.sh:error: --library and --cell options are"
                  echo    " mutually exclusive."
                  print_usage; exit 1
                fi;;

             L)
                if [ $# -ge 2 ]; then
                  lefFile="$2"
                  shift
                else
                  echo "a2LEF.sh:error: Missing <lef_file> argument."
                  print_usage; exit 1
                fi;;

             c)
                if [ $# -ge 2 ]; then
                  cellName="$2"
                  shift
                else
                  echo "a2LEF.sh:error: Missing <cell_name> argument."
                  print_usage; exit 1
                fi
                if [ ! -z "$lLibDir" ]; then
                  echo -n "a2LEF.sh:error: --library and --cell options are"
                  echo    " mutually exclusive."
                  print_usage; exit 1
                fi;;

             *) echo "a2LEF:error: Invalid option \`$CH'."
                print_usage; exit 1;;

           esac

           NB=`expr $NB + 1`
           CH=`echo $lSwitch | cut -c$NB`

         done;;

     *) echo "a2LEF:error: Invalid option \`$1'."
        print_usage; exit 1;;

   esac

   shift

 done


 title
  

 MBK_IN_PH=ap  ; export MBK_IN_PH
 MBK_IN_LO=vst ; export MBK_IN_LO
 MBK_OUT_PH=ap ; export MBK_OUT_PH
 MBK_OUT_LO=vst; export MBK_OUT_LO
  

 if [ "$techFile" != "" ]; then
   if [ ! -f $techFile ]; then
     echo "a2LEF:error: Technological file \"$techFile\" doesn't exist."
     exit 1
   fi
   setTechno $techFile $lefFile
 fi


 if [ -z "$cellName" ]; then
   if [ "$lLibDir" = "" ]; then
     echo "a2LEF:error: Missing mandatory argument <--library <lib_dir> >."
     print_usage; exit 1
   fi
  
   if [ "$lefFile" = "" ]; then
     echo "a2LEF:error: Missing mandatory argument <--lef <lef_file> >."
     print_usage; exit 1
   fi

  # ------------------------------------------------------------------
  # Library Generation.
  
   rm -f $lefFile > /dev/null 2>&1

   openLib $lefFile
  
   for libDir in $lLibDir; do
     buildLib $libDir $lefFile
   done
  
   closeLib $lefFile
 else
   if [ ! -f ${cellName}.ap ]; then
     echo "a2LEF:error: Can't find layout file \"${cellName}.ap\"."
     print_usage; exit 1
   fi

   if [ ! -f ${cellName}.vbe -a ! -f ${cellName}.vst ]; then
     echo "a2LEF:error: Can't find netlist/behavioral file :"
     echo "             \"${cellName}.vst or \"${cellName}.vbe\"."
     print_usage; exit 1
   fi

   log="$cellName.log"
   rm -f $log

   vecho 2 -n "     - \"$cellName\"... "
   sxlib2lef -V $sx2lef_args $cellName >> $log 2>&1; RV=$?

   if [ $RV -ne 0 ]; then
     vecho 2 "KO."
   else
     vecho 2 "OK."
   fi

   closeLib ${cellName}.lef
 fi



# --------------------------------------------------------------------
# That's all Folks.

 vecho 2 ""
 vecho 2 ""
 exit 0
