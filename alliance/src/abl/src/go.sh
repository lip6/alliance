#!/bin/sh
for I in *.[ch]
do
  sed -e 's/MUT_H/"mut.h"/g' -e 's/AUT_H/"aut.h"/g' -e 's/ABL_H/"abl.h"/g' $I > 1
  mv 1 $I
done
