#!/bin/bash

  scriptsDir="${HOME}/alliance/dev/scripts"
 cppCheckDir="${scriptsDir}/cppcheck"

   tools="`cat ${scriptsDir}/tools.lst`"

# Find all include directories
 toolDirs=""
 for tool in ${tools}; do
   toolDirs="${toolDirs} ${HOME}/alliance/alliance/src/${tool}"
 done

 echo "Building the include directories list..."
 find ${toolDirs} -name '*.h' -exec dirname {} \; | sort | uniq > ${cppCheckDir}/alliance.incs

 toolsMax="`wc -l ${scriptsDir}/tools.lst | cut -d ' ' -f1`"
 toolsNb="0"
 echo "$toolsMax tools to process."
 for tool in ${tools}; do
   toolsNb=`expr ${toolsNb} + 1`
   if [ ${toolsNb} -gt ${toolsMax} ]; then exit 0; fi

   toolDir="${HOME}/alliance/alliance/src/${tool}"
   sources=`find ${toolDir} -name \*.h -o -name \*.c -o -name \*.hpp -o -name \*.cpp`

   echo "Checking tool <${tool}>..."
   cppcheck -j3 $action --enable=all                               \
           --includes-file=${cppCheckDir}/alliance.incs            \
           ${sources} 2>&1                                         \
           | grep -v 'The scope of the variable .* can be reduced' \
           > ${cppCheckDir}/${tool}.log

 done

 exit 0
# Look for missing includes.
#action="--check-config"

# Perform the actual check.
 action=""

 cppcheck -j3 $action --enable=all --verbose \
          --includes-file=${cppCheckDir}/alliance.incs \
          --file-list=${cppCheckDir}/alliance.srcs > ${cppCheckDir}/cppcheck.log 2>&1 &
