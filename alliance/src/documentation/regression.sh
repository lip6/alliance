#!/bin/sh

 if [ -z "$ALLIANCE_TOP" ]; then
   ALLIANCE_TOP="${HOME}/alliance/Linux.slsoc6x/install"

   alcEnv="${ALLIANCE_TOP}/etc/profile.d/alc_env.sh"
   if [ ! -d "${alcEnv}" ]; then
     alcEnv="${ALLIANCE_TOP}/etc/alc_env.sh"
   else
     echo "[ERROR] Cannot find alc_env.sh."
     exit 1
   fi
   . ${alcEnv}
 fi

 pushd alliance-run
 ./bench.sh
 popd
 pushd alliance-examples
 ./go-all.sh
 popd
