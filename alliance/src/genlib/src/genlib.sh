#!/bin/sh 
#
# This file is part of the Alliance CAD System
# Copyright (C) Laboratoire LIP6 - Département ASIM
# Universite Pierre et Marie Curie
# 
# Home page          : http://www-asim.lip6.fr/alliance/
# E-mail support     : mailto:alliance-support@asim.lip6.fr
# 
# This progam is  free software; you can redistribute it  and/or modify it
# under the  terms of the GNU  General Public License as  published by the
# Free Software Foundation;  either version 2 of the License,  or (at your
# option) any later version.
# 
# Alliance VLSI  CAD System  is distributed  in the hope  that it  will be
# useful, but WITHOUT  ANY WARRANTY; without even the  implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
# 
# You should have received a copy  of the GNU General Public License along
# with the GNU C Library; see the  file COPYING. If not, write to the Free
# Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
# $Id: genlib.sh,v 1.2 2002/04/16 07:18:31 fred Exp $

help() {
   echo "Syntax: `basename $0` [-vk] source-file (without extension)"
   echo "                       -v : verbose mode"
   echo "                       -k : keeps  the executable (whith debugging"
   echo "                            informations) along with the"
   echo "                            compilation Makefile after completion"
   exit 1
}


   if [ $# -lt 1 -o $# -gt 6 ] ; then
      echo "Syntax: `basename $0` [-vk] source-file (without extension)"
      exit 1
   fi

   talk=0
   devel=0
   keep=0
   debug=
   lib=""
   libpath=""
   name=""
   while [ $# -gt 0 ]
   do
      case $1 in
      -v)  talk=1;;
      -d)  devel=1;;
      -k)  keep=1;debug=-g;;
      -vk) keep=1;debug=-g; talk=1;;
      -kv) keep=1;debug=-g; talk=1;;
      -l*) lib=$1;;
      -L*) libpath=$1;;
      *)   if [ -z "$name" ]; then 
              name=$1;
          else
              help
          fi
      esac
      shift
    done
    trap "rm -f ./*.$$; exit 1" 1 2 3 6

    ALLIANCE_LIB=${ALLIANCE_TOP}/lib
    ALLIANCE_INCLUDE=${ALLIANCE_TOP}/include
    ALLIANCE_BIN=${ALLIANCE_TOP}/bin

    export ALLIANCE_LIB ALLIANCE_INCLUDE ALLIANCE_BIN

    ${ALLIANCE_BIN}/alcbanner "GenLib" "3.3" "Procedural Generation Language" "1991"

    if [ -z "$name" ] ; then
        help
    fi
    if [ ! -f $name.c ] ; then
        echo "There seems no to be a file called $name.c"
        help
    fi

    rm -f $name > /dev/null 2>&1
        
    if [ $talk -eq 1 ]; then
        echo "Generating the Makefile";
    fi

# see genpat for another way of creating Makefile ;-) Czo...

# --------------------------------------------------------------------
# Development hacks.

   if [ $devel -eq 1 ]; then
     ALLIANCE_INSTALL_DIR="__ALLIANCE_INSTALL_DIR__"
     ALLIANCE_DEVEL_INCLUDE="-I${ALLIANCE_INSTALL_DIR}/include"
     ALLIANCE_DEVEL_LIB="-L${ALLIANCE_INSTALL_DIR}/lib"
     export ALLIANCE_INSTALL_DIR ALLIANCE_DEVEL_INCLUDE ALLIANCE_DEVEL_LIB
   fi

   makefile="Makefile.$$"
   echo 'ALLIANCE_LIBRARY = '${ALLIANCE_DEVEL_LIB}' -L$(ALLIANCE_LIB) \
                -lMgn -lMlu -lMpu -lMcp -lMap -lMmg -lMcl -lMgl \
                -lMal -lMvg -lMel -lMsl -lMhl -lMlo -lMph -lMut -lRcn'                                       >> $makefile

   echo "ALLIANCE_INC = ${ALLIANCE_DEVEL_INCLUDE} -I\$(ALLIANCE_INCLUDE)" >> $makefile
    echo "$name : $name.c"                                       >> $makefile
    echo '	$(CC) '   \
         "$debug -o $name $name.c $libpath $lib" \
          '$(ALLIANCE_INC) $(ALLIANCE_LIBRARY)' >> $makefile

   if [ $talk -eq 1 ]; then 
        echo "Compiling, ..."
    fi

   make -f $makefile > $name.grr 2>&1

   if [ ! $? -eq 0 ]; then 
      echo "Compilation failed!"
      cat $name.grr
      rm $makefile
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
        rm $name;
    fi
   rm $name.o $name.grr $makefile > /dev/null 2>&1
   exit $exit_code
