#!/bin/sh

for I in *0*.u
do
  BENCH=`basename $I .u`
  echo $BENCH
  ./go-bench.sh $BENCH
done

