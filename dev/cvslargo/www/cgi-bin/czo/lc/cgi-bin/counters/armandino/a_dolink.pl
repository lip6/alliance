#LogCount v3.8 link count and redirect
sub do_link {
($scrap, $lnkNumb) = split(/\=/,$query_string);
	if (!open (COUNT, "+<$log_path$lnks_name")) {$err = "Links File Access"; &err_log;}
   eval"flock(COUNT,2)";
	@lns = <COUNT>;
	$cnts = 0;
	foreach $ln (@lns){
		@TL = split(/\|/,$ln);
		if ($TL[0] eq $lnkNumb) {
			$the_link = $TL[1];
			if ($pn eq "l") {
				$TL[2]++;
				$lns[$cnts] = "$TL[0]|$TL[1]|$TL[2]\n";
				seek(COUNT,0,0);
				print COUNT @lns;
				last;
		}	}
	$cnts++;
	}
   eval"flock(COUNT,8)";
close(COUNT);
	if (!($the_link eq "")) {print "Location: $the_link\n\n";}
	else {
		print "Content-type: text/html\n\n"; print qq~<html><head><meta name="author" content="Ron F Woolley 1998-99 - www.dtp-aus.com"><meta HTTP-EQUIV="Pragma" CONTENT="no-cache"><title>Error Response</title></head><body bgcolor="#FFFFFD" text="#333333" link="#0000CC"><center><p>&nbsp;</p><table cellpadding="1" cellspacing="3" border="0"><tr bgcolor="#F5F5FF"><td><b><font face="verdana,arial,geneva,helvetica" size="3">&nbsp;Link <font color="#CC0000">Error Response</font></font></b></td></tr><tr><td align=center bgcolor="#F5F5F5"><font size="2" face="arial,helvetica,geneva"><p><strong>&nbsp;<br>The link you requested cannot be found.</strong></p><p>&nbsp;Please report this error to the <a href="$webmstr">webmaster</a>. <em>Thank you&nbsp;</em></p></font></td></tr><tr bgcolor="#666666"><td align=center><font size="2" face="arial,helvetica,geneva"><em><font color="#FFFFFF"><b>return with your</b></font></em><font color="#FFFFFF"><b> Back Arrow</b></font></font></td></tr></table></center></body></html>~;
		exit;
	}
exit;
}
1;  #line must remain last and as is
