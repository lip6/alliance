#!/bin/bash
#
# For Smart HTTP commit test #1
# For Smart HTTP commit test #2

 if [ -z "$ALLIANCE_TOP" ]; then
   alcEnv="/etc/profile.d/alc_env.sh"

   if [ ! -x "${alcEnv}" ]; then
     ALLIANCE_TOP="${HOME}/alliance/Linux.slsoc6x/install"

     alcEnv="${ALLIANCE_TOP}/etc/profile.d/alc_env.sh"
     if [ ! -f "${alcEnv}" ]; then
       alcEnv="${ALLIANCE_TOP}/etc/alc_env.sh"
     else
       echo "[ERROR] Cannot find alc_env.sh."
       exit 1
     fi
   fi
   . ${alcEnv}
 fi

 pushd alliance-run
 ./bench.sh
 popd
 pushd alliance-examples
 ./go-all.sh
 popd
