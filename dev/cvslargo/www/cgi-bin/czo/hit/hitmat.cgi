#! /usr/local/bin/perl -w
#
# HITMATIC
# Maintains counts for a page or all pages and logs accesses.
# Can optionally output the count to the browser.
#
#     Filename: hitmat.cgi
# Last revised: July 08, 1998
#    Copyright: 1996-1998 by Joe DePasquale
#       E-Mail: crypt@getcruising.com
#      Website: http://www.GetCruising.com
#
########################################################################
#                                                                      #
# This script and accompanying files may be distributed freely         #
# and modified, provided this header with my name, E-Mail address and  #
# this notice remain intact. Ownership rights remain with me. You may  #
# not sell this script without my approval.                            #
#                                                                      #
# This script comes with no guarantee or warranty except for my good   #
# intentions. By using this code you agree to indemnify me from any    #
# liability that might arise from it's use.                            #
#                                                                      #
# There is no technical support for this script, neither am I a        #
# professional programmer. Refer to 'HELPME.TXT' for further guidance. #
#                                                                      #
########################################################################
#
# 2. CONFIGURE SCRIPT -
#
# Change these sample paths to the actual paths on your server:

# Your Unix system date command
$dateCmd = '/bin/date';

# Unix path to hitmat directory
$hitmatDir = '/users/largo1/czo/cvslargo/www/cgi-bin/czo/hit';

# URL path to hitmat.cgi
$scriptUrl = 'http://asim.lip6.fr/cgi-bin/czo/hit/hitmat.cgi';

# URL path to hitmat directory
$hitmatUrl = 'http://asim.lip6.fr/cgi-bin/czo/hit/';

# Go to this URL when exiting manager
$exitUrl = 'http://asim.lip6.fr/';

# Log will record hit but exclude domain names matching this tail
$myTail = '.li.fr';

# OPTIONAL - You can edit these variables if desired:

# File Maintenance is triggered each day when the FIRST HIT between
# midnight and $trigHour is received. This will 1) swap 'hit1.log' and
# 'hit2.log' and 2) initialize a 'hitx.dat' file on the first day of a
# month. You can use any 24-hour time from 1 (1 AM) to 23 (11 PM).
$trigHour = 06;

$headTitle = "HITMATIC Website Counter System";
$bodyTag   = qq|<body bgcolor="#FFFFFF" text="#000099" link="#0000FF" alink="#FFFF00" vlink="#990000">|;
$bodyTitle = qq|i><b><font size=5><font color="#0000FF">HIT</font><font color="#FF9900">MATIC</font></font></b></i>|;

# Change the table border size? (0 = no border)
$border = 0;

# If you DON'T want to backup the hit*.dat files, set $bakFlag to 'N',
# otherwise set the correct values in the 'if' loop ..
$bakFlag = 'Y';

if ($bakFlag eq 'Y')
{ $bakTime = 1; # days btwn backups
  $bakMax  = 7; # days to keep backups 

  # Unix path to backup directory
  $bakDir = '/usr/home/you/htdocs/bak';

# END OF INSTALLATION - SHOULD NOT CHANGE STUFF BELOW THIS LINE
########################################################################

  chop ($jDate = `$dateCmd +"%j"`);
  $HITBAK = "$bakDir/hit$jDate.bak";
}
chop ($dayStamp = `$dateCmd +"%e"`); $dayStamp =~ s/\s*//g;
chop ($hourStamp = `$dateCmd +"%H"`);
chop ($timeStamp = `$dateCmd +"%a %D %H%M%Z"`);
chop ($dateStamp = `$dateCmd +"%Y%m%d"`);
$yearStamp = substr ($dateStamp,0,4);
$monthStamp = substr ($dateStamp,4,2);
$month = $monthStamp +0;

$HITCFG = "$hitmatDir/hit.cfg";
$HITCNT = "$hitmatDir/hit.cnt";
$HITDAT = "$hitmatDir/hit$month.dat";
$HITFLK = "$hitmatDir/hit.flk";
$HITLOG1 = "$hitmatDir/hit1.log";
$HITLOG2 = "$hitmatDir/hit2.log";
$HITMON = "$hitmatDir/hit";
$HITXBM = "$hitmatDir/hit.xbm";
$SINCEXBM = "since.xbm";

########################################################################
# Read GET or POST input and setup stuff

if (-e "./referer.pl")
{ require "./referer.pl";
  &referer;
}
if ($ENV{'QUERY_STRING'} eq 'manager' || $ENV{'REQUEST_METHOD'} eq 'POST')
{ require "./hitman.pl";
  require "./hitxbm.pl";
  $HITPWD = "$hitmatDir/hitmat.pwd";
  &hitman;
  exit;
} elsif ($ENV{'QUERY_STRING'} =~ /(\S+?)(\+(\S))?=(\S+?)(&.*|$)/)
{ $pagecode =$1; $group =$3; $command =$4;
  if ($group) { $pagecode = "$group\+$pagecode"; }
} else
{ &endIt;
}

open (LOCK,">$HITFLK") || &endIt;
if (!flock (LOCK,2)) { &endIt; }

# Daily maintenance if trigger time
if ($hourStamp < $trigHour)
{ require "./hitcfg.pl";
  &checkCfg;
}

# Add hit to main counter
open (CNT,"+<$HITCNT") || &endIt;
flock (CNT,2); seek (CNT,0,0);
@inFile = <CNT>;
$recNbr = 0; $foundFlag = 'N';

while (($recNbr <= $#inFile) && ($foundFlag eq 'N'))
{ ($count,$code,$eol) = split(/\|/,$inFile[$recNbr]);
  if ($code eq $pagecode)
  { if ($count < 999999) {$count++;} else {$count =1;}
    $inFile[$recNbr] = join ("\|",$count,$pagecode,"\n");
    $foundFlag = 'Y';
  }
  $recNbr++;
}
if ($foundFlag eq 'N') # new page
{ $count =1;
  $addLine = join ("\|",$count,$pagecode,"\n");
  push @inFile, $addLine;
}
@outFile = sort {($b =~ /(\d+)/)[0] <=> ($a =~ /(\d+)/)[0]} @inFile;

seek (CNT,0,0);
print (CNT @outFile);
truncate (CNT,tell(CNT)); close (CNT);

# Make the XBM image according to COMMAND
if ($command eq "PAGE")
{ require "./hitxbm.pl";
  &makeXbm ($count,6,$HITXBM);
}
elsif ($command eq "GROUP")
{ require "./hitxbm.pl";
  open (CNT,"<$HITCNT") || &endIt;
  flock (CNT,1); seek (CNT,0,0);
  @inFile = <CNT>;
  close (CNT);
  $groupTtl = 0;
  foreach $inLine (@inFile)
  { if ($inLine =~ /^\d+\|$group\+/)
    { ($count,$code,$eol,) = split(/\|/,$inLine);
      $groupTtl += $count;
    }
  }
  &makeXbm ($groupTtl,6,$HITXBM);
}
elsif ($command eq "SITE")
{ require "./hitxbm.pl";
  open (CNT,"<$HITCNT") || &endIt;
  flock (CNT,1); seek (CNT,0,0);
  @inFile = <CNT>;
  close (CNT);
  $siteTtl = 0;
  foreach $inLine (@inFile)
  { ($count,$code,$eol,) = split(/\|/,$inLine);
    $siteTtl += $count;
  }
  &makeXbm ($siteTtl,6,$HITXBM);
}
elsif ($command eq "NONE")
{ open (XBM,">$HITXBM");
  flock (XBM,2); seek (XBM,0,0);
  print (XBM "#define hit_width 1\n");
  print (XBM "#define hit_height 1\n");
  print (XBM "static char hit_bits[] = { 0x00 };\n");
  truncate (XBM,tell(XBM));
  close (XBM);
}
# Send the XBM image to Stdout
if ($command ne "IMG")
{ print "Content-type: image/x-xbitmap\n\n";
  print `cat $HITXBM`;
}

# Record hit in the log
$hitLine = join ("\|",$timeStamp,$pagecode,$command,$banNbr,$ENV{'REMOTE_HOST'},$ENV{'HTTP_USER_AGENT'});
if ($ENV{'HTTP_REFERER'} !~ /$myTail/i)
{ $hitLine .= "\|$ENV{'HTTP_REFERER'}";
}
if (open (LOG,">>$HITLOG1"))
{ flock (LOG,2); seek (LOG,0,2);
  print (LOG "$hitLine\n");
  close (LOG);
}
close (LOCK);
exit;

# end main program
######################################################################

sub endIt # exit on error
{ exit;
} # end endIt

