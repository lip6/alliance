#!/usr/local/bin/perl -w

# Czo : 2000/05/25 
#

use CGI;
$query = new CGI;


##############################################################################
#   An IMG SRC, SSI and A HREF(links) called script that saves to common     #
#   log files also used by the Loglook.cgi log viewer and SSI count viewers  #
#   DO NOT use this script without first reading the accompanying README     #
#   installation and help page IN DETAIL.                                    #
##############################################################################
# The script "LOGCNT.CGI" was written (c) by Ron F Woolley, Melbourne        #
# Australia. Copyright 1998'99 2000. This script can be altered for personal #
# site use or commercial site use EXCEPT THAT:                               #
# ALL COPYRIGHT NOTICES must remain in the code, visible on all output pages,#
# AND ALL of the header notices MUST REMAIN intact as is,                    #
# AND using the script without first reading the README.HTM, is prohibited.  #
#                                                                            #
# Australian copyright is recognised/supported in over 130 countries...      #
# per the Berne Convention and other treaties. (including USA!)              #
#                                                                            #
#  The scripts and associated files REMAIN the property of Ron F Woolley.    #
#  NO PROFIT what so ever is to be gained from users of these scripts by     #
#  installation of these scripts, except that a reasonable minimal charge    #
#  for installation MAY be allowed. Supply, per installation, is directly    #
#  from dtp-aus.com ONLY. Remote hosting of this program is strictly         #
#  not allowed.                                                              #
#                                                                            #
#  Ron Woolley, the author, MUST be notified via the addresses/URLs below    #
#  if any gain is to be made for the installation of these scripts.          #
#                                                                            #
##############################################################################
# NOTE: If you use these files, you do so entirely at your own risk, and     #
# take on full responsibility for the consequences of using the described    #
# files. You must first agree that Ron Woolley / HostingNet, the only        #
# permitted supplier of this and/or accompanying files is exempt from any    #
# responsibility for all or any resulting problems, losses or costs caused   #
# by your using these or any associated files. If you disagree with ANY of   #
# the included requirements, you must IMMEDIATELY DESTROY ALL FILES.         #
##############################################################################
#   This program script is free to use   Copyright notices must remain       #
##############################################################################
#  These program scripts are free-to-use, but if you use them, a donation to #
# the author would be appreciated and help in continuing support for         #
# LogCount and the creation of other scripts for users of the internet.      #
# An on-line Visa / MasterCard payment cart is available for support         #
# donations and low cost program installations.                              #
##############################################################################
# HELP/ADVICE page at:                                                       #
#    http://www.dtp-aus.com/cgiscript/scrpthlp.htm                           #
# Files from:                                                                #
#    http://www.dtp-aus.com/cgiscript/cntscrpt.shtml                         #
##############################################################################
# THESE FILES can only be obtained via the above web addresses,and MUST      #
# NOT BE PASSED ON TO OTHERS in any form by any means what so ever. This     #
# DOES NOT contradict any other statements above.                            #
# EACH USER SITE MUST OBTAIN THESE FILES from URL above.                     #
##############################################################################
#                     VERSION 3.8  Februrary 2000                            #

#--- Alter these two paths only, if needed! ---------------------#

$log_path = "counters/";     # - with forward slash
$img_url = "counters/";    # PATH - with forward slash

$sitename = $query->param('site');
if ( $sitename eq "" ) 
    {$sitename="asim";}
        
$log_path = $log_path . "$sitename/";
$img_url  = $img_url . "$sitename/";

	if (-s "$log_path/gmtset.pl") {require "$log_path/gmtset.pl";} else {print "Content-type: text/html\n\n"; print "Missing/Bad Path to GMTime file\n"; exit;}
	if (-s "$log_path/cntcnfg.pl") {require "$log_path/cntcnfg.pl";} else {print "Content-type: text/html\n\n"; print "Missing/Bad Path to Config file\n"; exit;}
        
        
#--- Do Not make any changes below this line. -------------------#

sub logged_as {
	$pnm = $ENV{'HTTP_REFERER'};
	$pnm =~ s/\#.*$//g ;
	$pnm =~ s/\?.*$//g;
	$pid = $pnm ;
		if ($pid =~ /https?:\/\/.*\/([^\/]*.*\/[^\.].*\..*)$/i) {$pid = $1;}
		elsif ($pid =~ /https?:\/\/.*(\/[^\.].*\..*)$/i) {$pid = $1;}
		elsif ($pid =~ /https?:\/\/.*\/(.*\/)$/) {$pid = $1;}
		elsif ($pid =~ /https?:\/\/.*(\/.*)$/) {$pid = $1;}
		else {$pid = "/$def_pge" if !$ENV{'DOCUMENT_URI'};}
	$pid =~ s/^\.+//;
}
sub sg_count {
	&do_count($log_path.$count_name);
		my $tmp;
		if ($pn eq "p" && (-s "$pnm")) {
		($tmp = $pid) =~ s/\./-/g;
		$tmp =~ s/\//_/g;
		&do_count($log_path.$tmp)	}
 	$cnb = (substr($iszeros,1,length($iszeros) - length($cnb)).$cnb);
}
sub do_count {
	my $fle = shift;
		if (!(-e "$fle")) {if (!open (COUNT, ">$fle")) {$err = "creating new count file"; &err_log;} close (COUNT);}
	if (!open (COUNT, "+<$fle")) {$err = "count file access"; &err_log;}
	eval"flock(COUNT,2)";
	$cnb = <COUNT>;
	if ( $pn eq "0" || $pn eq "t" || $pn eq "c" || substr($query_string,0,2) eq "p0" || $ENV{'DOCUMENT_URI'} ) {
		if ($rjct !~ /(\|$ENV{'REMOTE_HOST'}\||\|$ENV{'REMOTE_ADDR'}\|)/i) {$cnb = $cnb + 1;}
		seek (COUNT, 0, 0); 
		print COUNT $cnb; 
   	}
	eval"flock(COUNT,8)";
	close (COUNT);
	chomp($cnb);
}
sub do_log { 
	if ($rjct =~ /(\|$ENV{'REMOTE_HOST'}\||\|$ENV{'REMOTE_ADDR'}\|)/i) {return;}
	if (($ENV{'REMOTE_ADDR'} eq $ENV{'REMOTE_HOST'} || !$ENV{'REMOTE_HOST'}) && $ENV{'REMOTE_ADDR'} =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/) {
		$pk = pack('C4', $1, $2, $3, $4);
		$cnvrt = (gethostbyaddr($pk, 2))[0];
		if ($cnvrt) {$ENV{'REMOTE_HOST'} = $cnvrt;}
	}
    if (!open (MLOG, ">>$log_path$log_name")) {$err = "log file access"; &err_log;} 
   eval"flock(MLOG,2)";
      print MLOG date_time(1).", $ENV{'REMOTE_ADDR'}, $pid, $ENV{'HTTP_USER_AGENT'}, $ENV{'REMOTE_HOST'}\n";
   eval"flock(MLOG,8)";
	close(MLOG);
}
sub sendFile {
	if ($img_url eq $ssi_url) {print "Location: $_[0]\n\n";}
	else {
		die("$0: sendFile called w/o File\n") unless my $File = $_[0];
		die("$0: sendFile called on $File, no type\n") unless my $type = $_[1];
			my $size = 0;
		if ( open(FILE, $File)) {  
			$size = ( -s $File);
			print "Content-type: $type\n"; print "Content-length: $size\n\n"; print <FILE>;
			close (FILE);
		} else {$err = "send-image file access error"; &err_log;}
  	$size; 
	}
}
sub date_time {
	($which,$z) = ($_[0],"a"); my($sec,$min,$hour,$mday,$mon,$year);
	if (!$which) { 
		($min,$hour) = (gmtime(time + $gmtPlusMinus))[1,2]; 
		if ($hour > 11) {$z = "p";}
		if ($hour > 12) {$hour = $hour - 12;}
		return sprintf("%02d-%02d$z",$hour,$min);
	}
	elsif ($which)  { 
		($sec,$min,$hour,$mday,$mon,$year) = (gmtime(time + $gmtPlusMinus));
		if ($year > 99) {$year = 2000 + ($year - 100);}
		elsif ($year < 100) {$year = "19$year";}
	if ($dtUS eq "1") {return sprintf("%02d\/%02d\/%04d %02d:%02d:%02d",$mon + 1,$mday,$year,$hour,$min,$sec);}
	elsif ($dtUS eq "2") {return sprintf("%04d\/%02d\/%02d %02d:%02d:%02d",$year,$mon + 1,$mday,$hour,$min,$sec);}
	else {return sprintf("%02d\/%02d\/%04d %02d:%02d:%02d",$mday,$mon + 1,$year,$hour,$min,$sec);}
	}
}
sub do_ssi {
	&sg_count;  print "Content-type: text/html\n\n";
	if ($cnb eq 0 || $cnb eq "") {$cnb = "???";}
		$cnb =~ s/^(0+)// if !$ssi_zro;
	if (!$ssi_img) {print " ";}
	elsif ($ssi_img == 1) {print "$cnb";}
	else { 
		for($cnt = 0; $cnt <= length($cnb) - 1; $cnt++) { $digit = substr($cnb,$cnt,1);
		print "<img \n src=\"$ssi_url"."$digit.gif"."\" width=\"$iwid\" height=\"$ihgt\" border=\"0\">";}
	}
	$ENV{'HTTP_REFERER'} = $ENV{'DOCUMENT_URI'};
	&logged_as; &do_log;
exit;
}
sub do_clock {
	local($digit,$chk) = ($_[0],$_[0]);
	$digit = substr(date_time(0),$digit,1);
	&sendFile($img_url.$digit.".gif", "image/gif");
	if ($pn eq "c" && $chk eq 0) {&sg_count; &do_log;}
exit;	
}
sub err_log {require "$log_path"."a_errlog.pl"; &err_logs}

########## Main

	if ( $ENV{'DOCUMENT_URI'} ) { &do_ssi; }
	if ( $ENV{'REQUEST_METHOD'} eq 'GET' ) {;} 
	elsif ( $ENV{'REQUEST_METHOD'} eq 'POST' ) {;} 
	else {$err = "illegal method"; &err_log;}

        $query_string = $query->param('log');
	$query_string =~ s/(\0|`|..\\)//g; 
        $pn = substr($query_string,0,1);
        
	my $crf = 0;
        
	if ($ENV{'HTTP_REFERER'}) {
		foreach $referer (@referers) {
			if ($ENV{'HTTP_REFERER'} =~ m|\Ahttps?://([^/]*)$referer|i) {
				$crf = 1;
				last;
	}	}	}
    if ($crf == 0) {$err = "bad referrer, off-site access denied"; &err_log;}
			&logged_as;
	if ($pn =~ /\Al\Z/i) {require "$log_path"."a_dolink.pl"; &do_link;}
	elsif ($pn =~ /^(c|n)/) {&do_clock(substr($query_string,1,1));}
		&sg_count;
	if ($pn eq "p") {
		local($digit) = substr($cnb,substr($query_string,1,1),1);
		&sendFile($img_url.$digit.".gif", "image/gif");
		&do_log if $query_string =~ /^p0$/;
	} elsif ($pn eq "t") {
		&sendFile($img_url."trans.gif", "image/gif");
		&do_log ;
	} elsif ($pn eq "0" || $pn eq "v") {
		local($digit) = substr($cnb,0,1);
		&sendFile($img_url.$digit.".gif", "image/gif");
		&do_log if $pn eq "0";
	} elsif ($pn =~ /[1-7]/) {
		local($digit) = substr($cnb,$pn,1);
		&sendFile($img_url.$digit.".gif", "image/gif");
	}		
exit(0);
