#!/usr/local/bin/perl
$TMP=">/tmp/toto.123" ;
open sotie, $TMP ;
print "Content-type: text/html\n\n";
while (($key, $val) = each %ENV) {
	print "$key = $val<BR>\n";
}

