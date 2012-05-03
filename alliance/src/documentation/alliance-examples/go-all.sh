#!/bin/sh

 if [ -z "$ALLIANCE_TOP" ]; then
   ALLIANCE_TOP="${HOME}/alliance/Linux.slsoc6x/install"
 fi
 if [ "$ALLIANCE_TOP" != "/usr/lib/alliance" ]; then
   ALLIANCE_BIN=${ALLIANCE_TOP}/bin
      CELLS_TOP=${ALLIANCE_TOP}/cells
 else
   ALLIANCE_BIN=/usr/bin
      CELLS_TOP=/usr/share/alliance/cells
 fi
 TARGET_LIB=${CELLS_TOP}/sxlib

 export ALLIANCE_BIN CELLS_TOP TARGET_LIB

 for B in addaccu16 adder4 amd2901 amd2901-vasy digi divcas4 \
          multi16b-reg multi4b multi8 multi8b pgcd sqrt32 sqrt8 \
          mipsR3000 hadamard
 do
   if [ -d $B ]; then
     echo "----- BENCH $B ----- "
     (cd $B && \
      make clean && \
      make      ALLIANCE_BIN=${ALLIANCE_BIN} \
                   CELLS_TOP=${CELLS_TOP}    \
                  TARGET_LIB=${TARGET_LIB}   \
           ALLIANCE_EXAMPLES=/dsk/l1/alliance/alliance/src/documentation/alliance-examples \
     )
   fi
 done
