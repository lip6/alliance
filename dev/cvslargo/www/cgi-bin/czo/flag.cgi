#!/usr/local/bin/bash

# disable filename globbing
set -f

echo Content-type: text/plain
echo

# echo "argc is '"$#"' argv is '"$*"'"
# pwd

# echo $REQUEST_URI

URI="${DOCUMENT_ROOT}${REQUEST_URI}"

res=`echo $URI | grep \/$`
if [ X"$res"Y != XY ]
then 
 URI=${URI}index.html
fi

URI_ROOT=`echo $URI | sed 's,\/[^\/]*$,/,'`
URI_DOC=`echo $URI | sed 's,^.*\/,,' | sed 's,\.gb\.html$,,' | sed 's,\.fr\.html$,,' | sed 's,\.html$,,'`

#echo $URI_ROOT
#echo $URI_DOC

FILE0="${URI_ROOT}${URI_DOC}.html"
FILE1="${URI_ROOT}${URI_DOC}.fr.html"
FILE2="${URI_ROOT}${URI_DOC}.gb.html"


if [ -f "$FILE0" -a -f "$FILE1" -a -f "$FILE2" ]
then

cat << EOF

<Center>
<A href=${URI_DOC}.fr.html><img src="http://www-asim.lip6.fr/ssi/flag_fr.gif" alt="[FRENCH]" width="34" height="27" border=0 ></A> <A href=${URI_DOC}.gb.html><img src="http://www-asim.lip6.fr/ssi/flag_gb.gif" alt="[ENGLISH]" width="34" height="27" border=0></A>
<br clear=all>
</Center>

EOF

fi

