#### Path to Records and Counter Images, SEE readme.txt file ------#
$lookcfg_pth = "$log_path/lookcnfg.pl";
$pwrd_name = "look.pwd";

$grf1_url = "$ssi_url"."grf0.gif";
$grf0_url = "$ssi_url"."grf1.gif";

#### URL of the prefered return page (Home) -----------------------#
$hm_url = "http://armandino.free.fr/";

#### URL of the LOGLOOK.CGI log viewer script ---------------------#
$logScrpt = "http://www-asim.lip6.fr/cgi-bin/czo/lc/cgi-bin/loglook.cgi";

#### URL of the LOGCNT.CGI counter script -------------------------#
$countScrpt = "http://www-asim.lip6.fr/cgi-bin/czo/lc/cgi-bin/logcnt.cgi";

#### Enable display of last 7 days ("1") --------------------------#
$shwDays7 = "1";

#### Enter a Password or leave blank(""), SEE readme.txt file -----#
$theword = "log";

1;	# this line MUST remain in all 'require' files.
