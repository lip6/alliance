#### adding/subtracting zeros alters length of visible counters ---#
$iszeros = "000000";

#### Select British ("0"), US ("1"), Internat ("2") date pattern --#
$dtUS = "2";

#### An array of accepted referers domain names / IP#s ------------#
@referers = ('armandino.free.fr', 'chantaco.lip6.fr','asim.lip6.fr','www-asim.lip6.fr','lip6.fr','132.227.86.2',);   

#### An array of rejected domain names / IP#s ---------------------#
$rjct = "|myfoobars.com|000.000.000.000|wallies.edu.ca/~bloggs|";

#### Path to Records and Counter Images, SEE readme.txt file ------#
$gmt_pth = "sets/gmtset.pl";
#$log_path = "counters/";     # - with forward slash
$count_name = "hitcount.t";
$log_name = "hitlog.t";
$lnks_name = "hitlnk.t";
$badlog_name = "hitbadlog.t";
$def_pge = "index";
$iwid = 13;
$ihgt = 17;
$ssi_zro = 1;

#### PATH OR URL to NON SSI img display images --------------------#
#-- this can be the same as the $ssi_url value! (see readme)
#	$img_url = "counters/";    # PATH - with forward slash
	#$img_url = "http://yourdomain.name/ssi_img/";    # URL - with forward slash

#### URL to the images for SSI img display (see readme) -----------#
	$ssi_url = "http://www-asim.lip6.fr/ssi/img/";    # - with forward slash

#### SSI Output (0 = hidden (blank), 1 = Text, 2 = Images ---------#
	$ssi_img = 1;

#### E-Mail address of the Webmaster ------------------------------#
	$webmstr = "mailto:Olivier.Sirol\@lip6.fr?subject=Link_Error";

#### Show Referer Domain Names, SEE readme.txt file ---------------#
	$showDoms = "1";

1;	# this line MUST remain in all 'require' files.
