
 version="5.0"
  obsDir="../../../../coriolis-obs/home:jpc-lip6/alliance"

 printHelp () {
   echo ""
   echo "  Usage: uploadOBSs.sh [--sources] [--commit] [--run]"
   echo ""
   echo "  Options:"
   echo "    [--sources] : Build an archive from the HEAD of the current branch."
   echo "    [--commit]  : Push the files (commit) on the remote builder repository."
   echo "                  This will effectively triggers the rebuild of the packages."
   echo "                  OBS local repository is hardwired to:"
   echo "                      \"${obsDir}\""
   echo "    [--run]     : Perform all actions at once."
   echo ""

 }

 if [ $# -eq 0 ]; then printHelp; fi

    githash=`git log -1 --pretty=format:%h`
  doSources="false"
   doCommit="false"
 badAgument=""
 while [ $# -gt 0 ]; do
   case $1 in
     --sources) doSources="true";;
     --commit)  doCommit="true";;
     --run)     doSources="true"
                doCommit="true";;
     *)         badArgument="$1";;
   esac
   shift
 done
 if [ ! -z "${badArgument}" ]; then
   echo "[ERROR] patchenv.sh: Unknown argument \"${badArgument}\"."
   exit 1
 fi

 echo "Running uploadOBSs.sh"
 echo "* Using HEAD githash as release: ${githash}."
 if [ "${doSources}" = "true" ]; then
   echo "* Making source file archive from Git HEAD ..."
   git archive --prefix=alliance-5.0/ --format=tar.gz -o alliance-${version}.tar.gz HEAD
 fi

 echo "* Update files in OBS project directory."
 echo "  OBS package directory: \"${obsDir}\"."
 for distribFile in distrib/alliance.spec         \
                    coriolis-eda-${version}.tar.gz; do
   if [ ! -f "${distribFile}" ]; then continue; fi
   if [[ "${distribFile}" == distrib* ]]; then
     echo "  - copy ${distribFile}."
     cp ${distribFile} ${obsDir}
   else
     echo "  - move ${distribFile}."
     mv ${distribFile} ${obsDir}
   fi
 done
 
 sed -i "s,^Release: *1,Release:        <CI_CNT>.<B_CNT>.${githash}," ${obsDir}/alliance.spec
 sed -i "s,^%define docGithash .*,%define docGithash ${docGithash},"  ${obsDir}/alliance.spec
 if [ "${doCommit}" = "true" ]; then
   pushd ${obsDir}
   osc commit
   popd
 fi

