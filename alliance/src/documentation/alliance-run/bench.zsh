#!/bin/zsh

true
while [ $? -eq 0 ]
do
make clean && make
done
