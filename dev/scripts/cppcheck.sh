#!/bin/bash

  scriptsDir="${HOME}/alliance/dev/scripts"
 cppCheckDir="${scriptsDir}/cppcheck"

# Look for missing includes.
#action="--check-config"

# Perform the actual check.
 action=""

 cppcheck -j3 $action --enable=all --verbose \
          --includes-file=${cppCheckDir}/alliance.incs \
          --file-list=${cppCheckDir}/alliance.srcs > ${cppCheckDir}/cppcheck.log 2>&1 &
