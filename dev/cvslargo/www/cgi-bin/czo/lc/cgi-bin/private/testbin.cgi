#!/usr/local/bin/perl

# DO NOT EDIT code below this line!!

# The first line above must be a call to the location of your PERL 5 program.
# This MUST BE CORRECT - view installed scripts or consult Hosts Manual.
# FTP upload as an ASCII file to your CGI-BIN and set permissions to chmod 755
# CALL as "http://www.yourdomain.name/cgi-bin/testbin.cgi 
# CHANGE this next line to the correct URL to this program

	$sriptURL = "http://asim.lip6.fr/cgi-bin/czo/lc/cgi-bin/testbin.cgi";

# DO NOT LEAVE this program on your site - delete after use!!!
# Australian/International copyright Ron F Woolley 2000

	$field{'mailpth'} = "/usr/lib/sendmail"; my $cnvrt; my $boo = "&nbsp;<br>"; &GetFormInput; 
		&is_mail if $field{'mailpth'} && $field{'email'} && $field{'send'};
	print "Content-Type: text/html\n\n";
open (FLE, $0);@p1 = <FLE>;close (FLE);($pl = $p1[0]) =~s/\n//g if $p1[0] =~ /^#!\//;
if (-e "C:/" || $^O =~ /MSWin32/i) {$is32 = " on Win32:NT!";}
	if (($ENV{'REMOTE_ADDR'} eq $ENV{'REMOTE_HOST'} || !$ENV{'REMOTE_HOST'}) && $ENV{'REMOTE_ADDR'} =~ /(\d+)\.(\d+)\.(\d+)\.(\d+)/) {
		$pk = pack('C4', $1, $2, $3, $4); $cnvrt = (gethostbyaddr($pk, 2))[0]; if ($cnvrt) {$ENV{'REMOTE_HOST'} = $cnvrt;}	}
	if ($] =~ /^5/ && !$is32) {
$t1 = "<p>If you can read this page then the cgi-bin and<br>
this script have executed without errors. Your<br>
CGI-BIN is set up and working correctly.

<p>If installing dtp-aus.com scripts, you can now <br>
try the makedir.cgi script to create directories<br>
and empty files, ready for FTP uploading of all<br>
the files supplied (see readme.htm)."; }
	elsif ($] && $] !~ /^5/ && !$is32) {
$t1 = "<p>If you can read this page then the cgi-bin and<br>
this script have executed without errors. Your<br>
CGI-BIN is set up and working correctly.

<p><font color=#FF0000>However, $pl is<br>
is NOT calling Perl versions 5. My dtp-aus.com <br>
scripts are written for Perl Version 5. Contact<br>
your Host Service for the correct path to Perl 5<BR>
on your server.</font>"; }
	elsif ($is32) {
$t1 = "<p><font color=#FF0000>My dtp-aus.com Perl 5 scripts are not written for<br>
<B>MS NT</B>, nor do they currently receive any support<br>
for that operating system.</font>

<p>But, if you can read this page then the cgi-bin<br>
and this script have executed without errors. Your<br>
CGI-BIN is set up and working correctly."; }
	elsif (!$]) {
$t1 = "<p><font color=#FF0000><B>Your Perl Version could not be ascertained<br>
for installation verification.</B></font> 

<p>But, if you can read this page then the cgi-bin<br>
and this script have executed without errors. Your<br>
CGI-BIN is set up and working correctly."; }
	else {$t1 = "<p><font color=#FF0000><B>Details could not be ascertained.</B></font>";}
print qq~<html><head><title>Unix CGI-BIN Perl 5 Test</title><meta HTTP-EQUIV="Pragma" CONTENT="no-cache"></head><body bgcolor="#d5d5d5" text="#000000">
<center>$boo<table width="550" border=0 cellspacing=0 cellpadding=2><tr><th bgcolor="#cc0000"><font face="courier new,courier" size="3" color="#ffffff"><b>$pl<BR>Perl $]$is32</b></font></th></tr><tr><td align=center 
bgcolor="#cc0000"><table width="100%" border=0 cellspacing=0 cellpadding=6><tr><td nowrap bgcolor="#ffffff">
<font face="courier new,courier" size="3"><small>Absolute path : $ENV{'SCRIPT_FILENAME'}</small><br>$t1<p><em>Thank You</em><BR>Ron Woolley - <small>www.dtp-aus.com / www.hostingnet.com</small></font></td></tr></table></td></tr></table></font>
<center><table width="550" cellspacing="0" cellpadding="4" border="0"><tr><td width="100%" align="center"><font face="arial, geneva, helvetica" size="2">&nbsp;<br>
If the latest v5 of perl is not reported above, try another (<i>first line, top of the script</i>).<br>
&nbsp;&nbsp;&nbsp;<b>**</b> <b>From <u>most</u> servers a list should have been created below</b>.<br>&nbsp;</font></td></tr><tr><td width="100%" valign="TOP" bgcolor="#EFEFEF">
<font face="arial, geneva, helvetica" size="2"><b>To check your Sendmail program, enter the path in the first box.</b><br>IF A CRASH OCCURS then use the back arrow to return and try another. BUT IF this<br>
page reappears then check your e-mail where a simple letter should arrive within minutes.<br>&nbsp;</font></td></tr><tr>
<td width="100%" valign="TOP" align="CENTER" bgcolor="#EFEFEF"><font face="arial, geneva, helvetica" size="2"></font><form method="POST" action="$sriptURL">
<font face="arial, geneva, helvetica" size="2">The <b>PATH</b> to your servers &quot;Sendmail&quot; program</font> 
<input type="text" name="mailpth" size="35" maxlength="65" value="$field{'mailpth'}"><br><font face="arial, geneva, helvetica" size="2"><b>Your E-Mail</b> address ( <i>double check it</i> )</font> 
<input type="text" name="email" value="" size="35" maxlength="65"><br><input type="submit" name="send" value="Test Sendmail Path">
<input type="reset" value="reset form"></form></td></tr><tr><td width="100%" valign="TOP"><font face="arial, geneva, helvetica" size="2">&nbsp;&nbsp;<b>*&nbsp;&nbsp;&nbsp;</b><i>malfunctioning server Sendmail set up can receive without error BUT not send!!</i><br>
NOTE: Some servers might report the Sendmail path below.<br>&nbsp;&nbsp;** <i>perl paths may be listed  below from some servers but not the sendmail path.</i></font></td></tr></table></center>~;
print qq~<p><table width=550 border=0 cellspacing=0 cellpadding=2><tr><td bgcolor=#E5E5E5><blockquote>~;
	$mprog =`whereis sendmail`;
	$path	=`whereis perl`;
	@chked1 = split(" ",$mprog);
	@chked2 = split(" ",$path);
	$is = `which perl`;
	$mis = `which sendmail`;
print qq~<p><b>Perl Path</b>(s)<br>~; foreach $s2 (@chked2) {print "&#149; $s2<br>";}
print qq~<b>Default Perl</b> : $is<br><b>Sendmail Path</b>(s)<br>~; foreach $s2 (@chked1) {print "&#149; $s2<br>";}
print qq~<b>Default Sendmail</b> : $mis<br></blockquote></td></tr></table><hr width=\"550\"></center></body></html>~;

exit(0);
sub is_mail {
	my ($mprog,$rec,$tm); $tm = localtime(time);
	$mprog = "$field{'mailpth'}";
		$rec = "$field{'email'}";
	if (open (MAIL, "|$mprog -t")) {
		print MAIL "To: $rec\n";
		print MAIL "From: $rec\n";
		print MAIL "Subject: TESTBIN.CGI\n";
		print MAIL "\n\n";
		print MAIL "--------------------------------\n" ;
		print MAIL " This is just test blurb.\n   Your Sendmail path worked!!\n   \"$field{'mailpth'}\"\n" ;
		print MAIL "--------------------------------\n" ;
		print MAIL " IP# < $ENV{'REMOTE_ADDR'} >\n Host < $ENV{'REMOTE_HOST'} >\n $tm\n\n";
	close (MAIL);
	$boo = "<p><b>Mail 'attempt' passed</b> - check your mail</p>";	}
	else {$boo = "<p><b>Could not open Sendmail!</p>";}
}
sub GetFormInput {
	if ( $ENV{'REQUEST_METHOD'} eq 'GET' ) { $buffa = $ENV{'QUERY_STRING'}; } 
  	elsif ( $ENV{'REQUEST_METHOD'} eq 'POST' ) { read(STDIN,$buffa,$ENV{'CONTENT_LENGTH'}); }
  	else { print "Content-Type: text/html\n\n"; print "<html><head><title>OOOPs!</title><meta HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\"></head><body bgcolor=#FFFFFF text=#CC0000><center><h3>Use ONLY GET or POST method!</h3></center></body></html>"; exit(0);}
 		@bvals = split(/&/,$buffa);
		foreach $i (0 .. $#bvals){
			($name,$val) = split (/=/,$bvals[$i],2);
			$val=~tr/+/ /;
			$val=~ s/%(..)/pack("c",hex($1))/ge;
			$name=~tr/+/ /;
			$name=~ s/%(..)/pack("c",hex($1))/ge;
			$val =~ s/~!/ ~!/g;
			$val =~ s/`//g;
			$val =~ s/<([^>]|\n)*>//g; 
			$name =~ s/<!--#(.|\n)*-->//g;
			$val =~ s/<!--#(.|\n)*-->//g;
		$field{$name} = $val;
		}
}
