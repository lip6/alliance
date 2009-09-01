#!/bin/sh

for B in place_and_route simulation synthesis
do
  if [ -d $B/src ]
  then echo "--- BENCH $B ----"
       (cd $B/src && make clean)
  fi
done
