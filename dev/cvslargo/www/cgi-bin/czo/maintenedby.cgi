#!/usr/local/bin/bash

# disable filename globbing
set -f

echo Content-type: text/plain
echo

# echo "argc is '"$#"' argv is '"$*"'"

if [ X"$USER_NAME"Y = XY ]
then
 USER_NAME=czo
fi

if [ X"$USER_NAME"Y = XistesY ]
then
 USER_NAME=czo
fi
 
FINGER=` finger -m $USER_NAME | head -1 | cut -d ":" -f3 | sed 's/-/ /g' `

echo '<A HREF="mailto:'$USER_NAME'@asim.lip6.fr">'$FINGER'</A>'

