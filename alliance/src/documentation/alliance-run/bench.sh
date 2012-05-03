#!/bin/sh

 run=0

 while [ $run -lt 3 ]; do
   echo ""
   echo ""
   echo ""
   echo ""
   echo "=================================================================="
   echo "Run: $run (in 4s)"
   echo ""
   sleep 4
   make clean && make
   run=`expr $run + 1`
 done
