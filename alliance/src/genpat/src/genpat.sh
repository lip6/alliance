#!/bin/sh 
# $Id: genpat.sh,v 1.3 2004/07/27 18:24:48 fred Exp $

#set -v 
#set -x

help() {
      echo "Syntax: `basename $0` [-vk] source-file (without extension)"
      echo "                       -v : verbose mode"
      echo "                       -k : keeps  the executable (whith debugging"
      echo "                            information) along with the"
      echo "                            compilation Makefile after completion"
      exit 1
}

   if [ $# -lt 1 -o $# -gt 4 ] ; then
    help 1 2 
   fi

   debug=
   talk=0
   keep=0
   name=""
	while [ $# -gt 0 ]
	do
		case $1 in
      -v)  talk=1;;
      -k)  keep=1;debug=-g;;
      -vk) keep=1;debug=-g; talk=1;;
      -kv) keep=1;debug=-g; talk=1;;
      *)  if [ -z "$name" ]; then 
             name=$1;
          else
	    help
	  fi
      esac
      shift
	done
 	trap "rm -f ./*.$$; exit 0" 1 2 3 6

	alcbanner "GenPat" "3.1" "Procedural GENeration of test PATterns" "1991"

	if [ -z "$name" ] ; then
	    help
	fi
	if [ ! -f $name.c ] ; then
		echo "There seems no to be a file called $name.c"
		help
	fi
		
	if [ $talk -eq 1 ]; then
		echo "Generating the Makefile";
	fi

	makefile="Makefile.$$"


cat << EOF  > $makefile

ALLIANCE_LIBRARY = -L\$(ALLIANCE_TOP)/lib \\
                     -lPgn \\
                     -lPat \\
                     -lMut \\
                     -lRcn


ALLIANCE_INC = -I\$(ALLIANCE_TOP)/include 

$name : $name.c
	\$(CC) $debug -o $name $name.c \$(ALLIANCE_INC) \$(ALLIANCE_LIBRARY)

EOF

   if [ $talk -eq 1 ]; then 
		echo "Compiling, ..."
	fi

   make -f $makefile > $name.gpt 2>&1

   if [ ! $? -eq 0 ]; then 
      echo "Compilation failed!"
      cat $name.gpt
		exit 1
	fi

   if [ $talk -eq 1 ] ; then
	   echo "Current execution environment"
		echo "MBK_CATA_LIB   : ${MBK_CATA_LIB-no cell library specified}"
		echo "MBK_WORK_LIB   : ${MBK_WORK_LIB-:}"
		echo "MBK_IN_LO      : ${MBK_IN_LO-vst}"
		echo "MBK_OUT_LO     : ${MBK_OUT_LO-vst}"
		echo "MBK_IN_PH      : ${MBK_IN_PH-ap}"
		echo "MBK_OUT_PH     : ${MBK_OUT_PH-ap}"
		echo "MBK_CATAL_NAME : ${MBK_CATAL_NAME-CATAL}"
   fi

   if [ $talk -eq 1 ]; then
      echo "Executing ..."
	fi
   ./$name
   exit_code=$?

   if [ $talk -eq 1 ]; then
		echo "Removing tmp files ..."
	fi

   if [ $keep -eq 0 ] ; then
		rm -f $name $name.exe;
	fi

   rm -f $name.o $name.gpt $makefile > /dev/null 2>&1

   exit $exit_code

