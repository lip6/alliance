#!/usr/local/bin/perl
#######################################################################
# Audric's Counter v1.0 FREEWARE
# Full copyright and legal speak at:
# http://www.aimsweb.com/goodies/counter2.htm
#
# Inspired on Matt's Wright Counter program
# Matt's Script Archive http://www.worldwidemart.com/scripts/
#######################################################################

# Define Variables

### FILE AND DIRECTORY LOCATIONS, REFERERS ###
$count_file = "/users/cao7/czo/public_html/cgi-bin/count.txt";  # must be chmod 777
$digit_dir  = "/ssi/";
$access_log = "/users/cao7/czo/public_html/cgi-bin/access_log"; # must be chmod 777
$error_log  = "/users/cao7/czo/public_html/cgi-bin/error_log";  # must be chmod 777

# If someone wants to use your counter from theri pages and their domain
# is not contained into @refers show them this gif.
$bad_referer_img = "http://www.aimsweb.com/images/bad_referer.gif";

# Domains you want to allow to use this script
@referers=(
	"lip6.fr",
	"127.0.0.1"
);

### IMAGE SETTINGS ###
# If you change the 0-9.gif set make sure you chage those accordingly
$width = "9";
$height = "20";

### OPTIONS ###
$uselog = "1";  # 1 = YES; 0 = NO

# Done - You shouldn't need to change anything below this line..
#######################################################################

#################
# Get the Date For Logging Purposes
if ($uselog == 1) {
   ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime(time);
   $mon++; # fixes Matt's Counter bug
   if ($sec < 10)  { $sec = "0$sec";   }
   if ($min < 10)  { $min = "0$min";   }
   if ($hour < 10) { $hour = "0$hour"; }
   if ($mday < 10) { $mday = "0$mday"; }
   if ($mon < 10)  { $monc = "0$mon";  }
   $date = "$hour\:$min\:$sec $mon/$mday/$year";
}

#################
# Make Sure People Aren't Messing With the Counter From Other Web Pages
#&check_referer;

#################
# Get the Counter Number And Write New One to File
&get_num;

#################
# Determine Length of Counter Number
# Set Individual Counter Numbers Into Associative Array
$savedcount=$count;
$num = $length = length($count);
while ($num > 0) {
   $CHAR{$num} = chop($count);
   $num--;
}

#################
# Generates HTML to use pre-generated GIFs
$j = 1;
$myNumber="";
while ($j <= $length) {
	$myNumber .= "<img src=\"$digit_dir$CHAR{$j}\.gif\" border=0 width=$width height=$height>";
	$j++;
}

#################
# Write the generated HTML to STDOUT
print "Content-Type: text/html\n\n";
print "<table border=0 cellspacing=0 cellpadding=0><tr><td>$myNumber</td></tr></table>\n";


#################
# Log the Counter Access
if ($uselog == 1) {
   &log_access;
}

exit;


######################################################
# SUBROUTINES
######################################################


#################
sub check_referer {
   if (@referers && $ENV{'HTTP_REFERER'}) {
      foreach $referer (@referers) {
	 if ($ENV{'HTTP_REFERER'} =~ /$referer/) {
	    $ref = 1;
	    last;
	 }
      }
   }
   else {
      $ref = 1;
   }

   if ($ref != 1) {
      print "Content-Type: text/html\n\n $bad_referer_img\n\n";

      if ($uselog == 1) {
	 open(LOG,">>$error_log") || die "Can't Open User Error Log: $!\n";
	 print LOG "$error: $ENV{'REMOTE_HOST'} [$date] $ENV{'HTTP_REFERER'} - $ENV{'HTTP_USER_AGENT'}\n";
	 close(LOG);
      }
      exit;
   }
}

#################
sub get_num {
   open(COUNT,"$count_file") || die "Can't Open Count Data File: $!\n"; 
   $count = <COUNT>;
   close(COUNT);
   if ($count =~ /\n$/) {
      chop($count);
   }

   $count++;

   open(COUNT,">$count_file") || die "Can't Open Count Data File For Writing: $!\n";
   print COUNT "$count";
   close(COUNT);
}

#################
sub log_access {
   open(LOG,">>$access_log") || die "Can't Open User Access Log: $!\n";
   print LOG "[$date] $ENV{'HTTP_REFERER'} - $ENV{'REMOTE_HOST'} -  $ENV{'HTTP_USER_AGENT'}\n";
   close(LOG);
}
