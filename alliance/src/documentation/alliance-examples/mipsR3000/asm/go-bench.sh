#!/bin/sh

LOG_RES=/tmp/res_mips_"$$"
LOG_MSG=/tmp/msg_mips_"$$"

if [ $# -lt 1 ]; then
    echo "usage: $0 <bench_file>"
    exit 1
fi
if [ ! -f $1.u ]; then
    echo "error: can't find '$1.u'"
    exit 1
fi

TOP=$ALLIANCE_TOP
MBK_CATA_LIB=.:$TOP/cells/sclib:$TOP/cells/sxlib:$TOP/cells/padlib:$TOP/cells/dplib:$TOP/cells/dp_sxlib
MBK_CATA_LIB=$MBK_CATA_LIB:$TOP/cells/fplib:$TOP/cells/rfg:$TOP/cells/dplib:$TOP/cells/rflib
MBK_CATA_LIB=$MBK_CATA_LIB:../sce/
export MBK_CATA_LIB
MBK_IN_LO=vst
export MBK_IN_LO

rm -f $LOG_RES $LOG_MSG

echo "################# ---- Test de : $1 ---- #################"

if [ -f $1.e ]; then
    cp -f $1.e rome.e
     $TOP/bin/mips_asm rome.e rome foo  1> /dev/null
fi

if [ -f $1.u ]; then
    cp -f $1.u romu.u
    $TOP/bin/mips_asm romu.u romu foo  1> /dev/null
fi



$TOP/bin/asimut -zerodelay -i 0 -bdd -p 100 mips_cpu mips_cpu $1 1> $LOG_MSG 2> $LOG_RES

#asimut -zerodelay -bdd -p 100 mips_cpu mips_cpu $1 2>$LOG_RES

#asimut -zerodelay -bdd -p 100 mips_cpu mips_cpu $1



grep -i bad $LOG_RES >/dev/null

#grep -i bad $LOG_RES

if [ $? -eq 0 ] ; then
	echo "ERROR !"
fi


grep -i good $LOG_RES  > /dev/null

#grep -i good $LOG_RES

if [ $? -eq 0 ] ; then
        echo "No error, you're lucky !"
fi




grep -i "No such file or directory" $LOG_RES  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= By jove ! a file is missing (asimut) ... ============== "
	grep -i "No such file or directory" $LOG_RES
fi


grep -i "No such file or directory" $LOG_MSG  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= By jove ! Il manque un fichier (asimut) ... ============== "
	grep -i "No such file or directory" $LOG_MSG
fi




grep -i "core dump" $LOG_RES  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= core dump (asimut) ... ========= "
fi

grep -i "core dump" $LOG_MSG  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= core dump (asimut) ... ========= "
fi



grep "Error" $LOG_RES  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= Error (asimut) ... ==== "
	grep -i "Error" $LOG_RES
fi

grep "Error" $LOG_MSG  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= Error ! (asimut) ... ==== "
	grep -i "Error" $LOG_MSG
fi



grep "can't open file" $LOG_RES  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= A file is missing (asimut) ... ========== "
	grep -i "can't open file" $LOG_RES
fi

grep "can't open file" $LOG_MSG  > /dev/null

if [ $? -eq 0 ] ; then
        echo "================= A file is missing (asimut) ... ========== "
	grep -i "can't open file" $LOG_MSG
fi



grep "exception occured" $LOG_RES  > /dev/null

if [ $? -eq 0 ] ; then
grep "exc" $1.u  > /dev/null
if [ ! $? -eq 0 ] ; then
        echo "================= Exception (asimut) ... ======== "
fi
fi

grep "exception occured" $LOG_MSG  > /dev/null

if [ $? -eq 0 ] ; then
grep "exc" $1.u  > /dev/null
if [ ! $? -eq 0 ] ; then
        echo "================= Exception (asimut) ... ======== "
fi
fi

rm -f toto.vbe
rm -f romu.u
rm -f rome.e
rm -f $LOG_RES
rm -f $LOG_MSG




