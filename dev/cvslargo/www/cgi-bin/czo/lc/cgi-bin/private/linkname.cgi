#!/usr/local/bin/perl

# A free SSI utility for LOGCNT.CGI users.
# THIS program displays only the total of a counted link.
# linkcount.cgi displays the hyperlink PLUS the count.
# This script written (and ©) by Ron F Woolley, Melbourne Australia.
# Copyright 1998. This script can be altered for personal use, BUT
# all of this header text MUST REMAIN intact as is.
#
# Support Information is available via:  
# http://www.dtp-aus.com/scrpthlp.htm    
# File from http://www.dtp-aus.com/cgiscript/cntscrpt.shtml 
#  
#  This file can only be obtained via the above web addresses,
#  and MUST NOT be passed on to others in any form by any means 
#  what so ever.   
# 
# TO USE - SSI ONLY: If you would like to place text displaying the number of
# times one of your counted hyperlinks has been clicked, place this 
# call in the html text of an SSI page (.shtml etc) ie:
#
# "File downloaded <!--#exec cgi="/cgi-bin/linkname.cgi"--> times."
#
# where 'linkname' is one of the names you entered in the "hitlink.t" file.
# For each link count you want to access, simply copy this file with the new 
# name and place it in your cgi-bin (chmod 755). 
# ie link1.cgi, Contact.cgi, Page_One.cgi, etc. (must be legal file name format)
# So, to display 5 different counted link values, you will have, and call, 
# 5 different scripts - simple. I use them on my 'Webmasters page' so I don't
# have to run the Admin page every time I log on to the Internet. A result of 
# only "-" means the names do not match, "!" = a script or file access error.
# The supplied "webmster.shtml" includes examples.

# -- If you changed the default path, you will need to change this line also ---
	require "sets/cntcnfg.pl";
# ------------------------------------------------------------------------------
	$cnt = "!";
	if (!$ENV{'SCRIPT_NAME'}) {print "Content-type: text/html\n\n"; &errs;}
	$lnkName = $ENV{'SCRIPT_NAME'};
	$lnkName =~ s|^.*/||;
	$lnkName =~ s|^.*\\||;
		print "Content-type: text/html\n\n";
		print &get_lnk_dat;
exit;
sub get_lnk_dat {
	$cnt = "-";
	if (!open (COUNT, "<$log_path$lnks_name")) {&errs;}
     flock(COUNT,2);
		@lns = <COUNT>;
     flock(COUNT,8);
	close(COUNT);
	foreach $ln (@lns) {
		@TL = split(/\|/,$ln);
		if ($lnkName =~ /(^$TL[0]\.cgi)/) {
			$cnt = $TL[2];
			last;
	}	}
chomp($cnt);
return $cnt;
}
sub errs {print $cnt; exit; }
