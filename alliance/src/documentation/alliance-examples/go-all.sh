#!/bin/sh

for B in addaccu16 adder4 amd2901 amd2901-vasy digi divcas4 \
    multi16b-reg multi4b multi8 multi8b pgcd sqrt32 sqrt8 \
    mipsR3000 
do
  if [ -d $B ]
  then echo "----- BENCH $B ----- "
       (cd $B && make clean && make ALLIANCE_EXAMPLES=/dsk/l1/alliance/alliance/src/documentation/alliance-examples)
  fi
done
