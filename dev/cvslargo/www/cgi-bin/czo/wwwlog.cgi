#!/usr/local/bin/perl

if (!open (COUNT,">>/users/largo2/webmastr/wwwroot/cgi-bin/log.db")) {
                exit 1;
}
while (($key, $val) = each %ENV) {
	print COUNT "$key = $val; ";
}
	print COUNT "\n";
        close COUNT;

