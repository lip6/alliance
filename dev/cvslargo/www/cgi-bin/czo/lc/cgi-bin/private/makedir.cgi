#!/usr/local/bin/perl

#### Once this script is used, DELETE IT immediatly.####
#### Once this script is used, DELETE IT immediatly.####
####     Input to this script is not protected      ####

##--Place in your cgi-bin directory and chmod 755 (7=r+w+x, 5=r+x).
##--Then run from browser command line, ie http://www.yourdomain.path/cgi-bin/makedir.cgi

$dir = "sets";
mkdir $dir,"766";
chmod(0766, "$dir");

$dir1 = "counters";
mkdir $dir1,"766";
chmod(0766, "$dir1");

if (!open (NEW, "+<$dir/gmtset.pl")) {open (NEW, ">$dir/gmtset.pl");}
close (NEW); chmod(0766, "$dir/gmtset.pl");
if (!open (NEW, "+<$dir/cntcnfg.pl")) {open (NEW, ">$dir/cntcnfg.pl");}
close (NEW); chmod(0766, "$dir/cntcnfg.pl");
if (!open (NEW, "+<$dir/lookcnfg.pl")) {open (NEW, ">$dir/lookcnfg.pl");}
close (NEW); chmod(0766, "$dir/lookcnfg.pl");
if (!open (NEW, "+<$dir1/a_dolink.pl")) {open (NEW, ">$dir1/a_dolink.pl");}
close (NEW); chmod(0766, "$dir1/a_dolink.pl");
if (!open (NEW, "+<$dir1/a_errlog.pl")) {open (NEW, ">$dir1/a_errlog.pl");}
close (NEW); chmod(0766, "$dir1/a_errlog.pl");
if (!open (NEW, "+<$dir1/hitbadlog.t")) {open (NEW, ">$dir1/hitbadlog.t");}
close (NEW); chmod(0766, "$dir1/hitbadlog.t");
if (!open (NEW, "+<$dir1/hitcount.t")) {open (NEW, ">$dir1/hitcount.t");}
close (NEW); chmod(0766, "$dir1/hitcount.t");
if (!open (NEW, "+<$dir1/hitlnk.t")) {open (NEW, ">$dir1/hitlnk.t");}
close (NEW); chmod(0766, "$dir1/hitlnk.t");
if (!open (NEW, "+<$dir1/hitlog.t")) {open (NEW, ">$dir1/hitlog.t");}
close (NEW); chmod(0766, "$dir1/hitlog.t");
if (!open (NEW, "+<$dir1/look.pwd")) {open (NEW, ">$dir1/look.pwd");}
close (NEW); chmod(0766, "$dir1/look.pwd");

print "Content-type: text/html\n\n";
print "<html><body bgcolor=\"FFFFFF\" text=\"000000\">\n";

if (!( -e "$dir")) {print "New $dir Directory not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1")) {print "New $dir1 Directory not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir/gmtset.pl")) {print "$dir/gmtset.pl File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir/cntcnfg.pl")) {print "$dir/cntcnfg.pl File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir/lookcnfg.pl")) {print "$dir/lookcnfg.pl File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/a_dolink.pl")) {print "$dir1/a_dolink.pl File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/a_errlog.pl")) {print "$dir1/a_errlog.pl File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/hitbadlog.t")) {print "$dir1/hitbadlog.t File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/hitcount.t")) {print "$dir1/hitcount.t File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/hitlnk.t")) {print "$dir1/hitlnk.t File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/hitlog.t")) {print "$dir1/hitlog.t File not created.<br>\n"; $s1 = 1;}
if (!( -e "$dir1/look.pwd")) {print "$dir1/look.pwd File not created.<br>\n"; $s1 = 1;}

if ($s1) {print "<br>Errors Occurred! Read the Readme file for options.\n";}
else {print "No Errors reported, so the sub directory and files should be ready for you to copy to (FTP). <em>See Readme file</em>.\n";}
print "<br>Script finished, ".localtime(time).", Server Local Time.\n";
print "<p>The absolute pathname of this script is $ENV{'SCRIPT_FILENAME'}.<br>\n";
print "The relative pathname of this script (via the root directory) is $ENV{'SCRIPT_NAME'}.<br><em>Delete this script NOW!</em>.</P>\n";
print "<body><html>\n";

exit;

#### Once this script is used, DELETE IT immediatly.####
#### Once this script is used, DELETE IT immediatly.####
