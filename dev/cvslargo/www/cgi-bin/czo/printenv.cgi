#!/usr/local/bin/perl
$TMP=">/tmp/toto.123" ;
open sotie, $TMP ;
print "Content-type: text/html\n\n";
print "PrintEnv:\n\n<PRE>\n";
while (($key, $val) = each %ENV) {
	print "$key = $val<BR>\n";
}
print "</PRE>\n";
