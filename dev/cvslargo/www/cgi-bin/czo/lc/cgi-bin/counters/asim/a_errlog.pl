# LogCount v3.8 error logging
sub err_logs {
	if ((-e "$log_path"."$ENV{'REMOTE_ADDR'}.tf") || (-s "$log_path$badlog_name") > 2000000 || !open (LOG, ">>$log_path$badlog_name")) {exit;}
		open (LOGG, ">$log_path"."$ENV{'REMOTE_ADDR'}.tf") || exit;
   eval"flock(LOG, 2)";
	my $trim = date_time(1)."|$err|$ENV{'HTTP_REFERER'}|$ENV{'REMOTE_ADDR'}";
		$trim = substr($trim,0,150);
	print LOG "$trim\n";
   eval"flock(LOG, 8)";
	close(LOG);
	close(LOGG);
	if (!($pn eq "l")) {&sendFile($img_url."trans.gif", "image/gif");}
	else {print "Content-type: text/html\n\n";
		print "Pages are copyright. Remote link access refused!<br>Please link via the actual on-site page\n";}
	sleep 10;
	unlink ("$log_path"."$ENV{'REMOTE_ADDR'}.tf");
exit;
}
1;  #line must remain last and as is
