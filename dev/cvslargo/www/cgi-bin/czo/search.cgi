#!/usr/local/bin/perl
# (C) Copyright 1997,98 Czo <mailto:Olivier.Sirol@asim.lip6.fr>
# Parts (C) A.Fenyo
use CGI;
use POSIX;

# SCRIPT_NAME = /cgi-bin/test.cgi
# SCRIPT_FILENAME = /users/largo2/webmastr/wwwroot/cgi-bin/printenv.cgi


$ASIM_HEADER=`/bin/cat /users/largo2/webmastr/wwwroot/ssi/asim.header/index.gb.html`;
$ASIM_FOOTER=`/bin/cat /users/largo2/webmastr/wwwroot/ssi/asim.footer/index.gb.html`;
$SEARCH_HEADER=`/bin/cat /users/largo2/webmastr/wwwroot/ssi/search.header/index.gb.html`;
$SEARCH_MIDDLE=`/bin/cat /users/largo2/webmastr/wwwroot/ssi/search.middle/index.gb.html`;
$SEARCH_FOOTER=`/bin/cat /users/largo2/webmastr/wwwroot/ssi/search.footer/index.gb.html`;

$lock = "/tmp/search.lock";
$log = "/users/largo2/webmastr/wwwroot/cgi-bin/private/search.log";
  
print "Content-type: text/html\n\n";

$query = new CGI;

$str = $query->param('searchstring');

# Czo 990607 : autorise . et _

$str =~ s/[^a-zA-Z0-9-_ \*\.]//g;
$str =~ s/ -/ /g;
$str =~ s/^-/ /;

$grepstr=$str;

$grepstr =~ tr/A-Z/a-z/;
@words = split / +/, $grepstr;
foreach $word (@words) {
    $word =~ s/ *//;
    next if (!$word);
    next if (($word eq 'or') || ($word eq 'and'));
    @allwords = (@allwords, $word);
};



$scope = $query->param('ComboSearchList');

  if ( $scope eq "Alliance Mailling Lists" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/alml.dat";
  } elsif ( $scope eq "Alliance" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/alliance.dat";
  } elsif ( $scope eq "Education" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/education.dat";
  } elsif ( $scope eq "Mpc" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/mpc.dat";
  } elsif ( $scope eq "Multimedia" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/multimedia.dat";
  } elsif ( $scope eq "Publications" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/publications.dat";
  } elsif ( $scope eq "Personal pages" ) {
  $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/users.dat";
  } else { $scopefile = "/users/largo2/webmastr/wwwroot/cgi-bin/allsite.dat";
  }


if (-e $lock) {
print "$ASIM_HEADER\n$SEARCH_HEADER\n";
print '<INPUT NAME=searchstring size=40 maxlength=800 name="searchstring" value="'.$str.'" > ';
print "$SEARCH_MIDDLE\n";
 print '<font face="verdana,arial,helvetica" size=2><b>Sorry</b>, server was busy, please try again.</font>';
print "<SCRIPT>Server is busy, please try again.</SCRIPT>\n";
print "$SEARCH_FOOTER\n$ASIM_FOOTER\n";
	      }
else {

print "$ASIM_HEADER\n$SEARCH_HEADER\n";
print '<INPUT NAME=searchstring size=40 maxlength=800 name="searchstring" value="'.$str.'" > ';
print "$SEARCH_MIDDLE\n";

#system (date);
#print system ( "/usr/local/home/czo/local/FreeBSD/bin/swish-e -f /usr/local/home/czo/public_html/cgi-bin/index.swish-e -w $str" );
#$RET = `/usr/local/home/czo/local/FreeBSD/bin/swish-e -c /usr/local/home/czo/public_html/cgi-bin/index.swish-e -w $str`;
#print "<PRE>";

open RESULT, "/usr/local/bin/swish-e -m 40 -f $scopefile -w $str |";

$cpt = 0;
while (<RESULT>) {
#    chop;
#print ;

    m/err: no results/ && do
    {
      $nmatch=0;
      print '<font face="verdana,arial,helvetica" size=2><b>No</b> match found in "'.$scope.'". </font>';
      print '<font face="verdana,arial,helvetica" size=-1>';
     };

    m/^# Number of hits: (.*)/ && do 
    {
      $nmatch=$1;
      print '<font face="verdana,arial,helvetica" size=-1><b>'.$1.'</b> matches were found in "'.$scope.'". </font>';
      print '<font face="verdana,arial,helvetica" size=-1>';
     };

    m/^#/ && next;

    
    m/^(.*) (.*) "(.*)" (.*)/ && (($res1, $res2, $res3, $res4) = ($1, $2, $3, $4)) && do
      {
         $cpt++;
         $tmp1 = $res1/10; $tmp1 =~ s/^(.*)\..*/\1/;
         $tmp = $res4/1024; $tmp =~ s/^(.*\..).*/\1/;

print '<P><b>'.$cpt.'. </b><font color="#0000FF"> ['.$tmp1.'%] </font> <a href="'.$res2.'"><b>'.$res3.'</b></a><br>
<font face="arial,helvetica" size=-2>[<b>URL:</b> '.$res2.']</font><br>
<font face="verdana,arial,helvetica" size=-2>Page size '.$tmp.' Kb</font><P>' ;
#Last modified 7-Oct-97 - 

# $file=$res2;
# $file =~ s%http://www-asim.lip6.fr/%/users/largo2/webmastr/wwwroot/%;
# 
# # print "<P>$file\n<P>";
# 
# #$FILE = `/asim/gnu/bin/lynxx -dump -reload -nolist  $file`;
# $FILE = `cat $file`;
# 
#     $FILE =~ s/<BR>/ - /sg;
#     $FILE =~ s/<HR>/ - /sg;
#     $FILE =~ s/<LI>/ - /sg;
#     $FILE =~ s/<DT>/ - /sg;
#     $FILE =~ s/<DD>/ - /sg;
#     $FILE =~ s/<P>/ - /sg;
#     $FILE =~ s/-( -)*/-/sg;
# 
#     $FILE =~ s/<.*?>/ /sg;
# 
#     foreach $word (@allwords) {
# 	$FILEinsensitive = $FILE;
# 	$FILEinsensitive =~ tr/A-Z/a-z/;
# 	$wordinsensitive = $word;
# 	$wordinsensitive =~ tr/A-Z/a-z/;
# 	$fileindex = index $FILEinsensitive, $wordinsensitive;
# 	next if ($fileindex == -1);
# 
# 	$fileindex = 160 if ($fileindex < 160);
# 	$grep = substr $FILE, $fileindex - 160, 320;
# 	$grep =~ s/^[^ ]* //s;
# 	$grep =~ s/ [^ ]*$//s;
# 
# 	foreach $w (@allwords) {
# 	    $grep =~ s%($w)%<B>\1</B>%si;
# 	};
# 
# 	print "<SMALL>... $grep ...</SMALL><P>\n";
# 
# 	last;
#     };
# 
# 
# 
# 
        };

} ;

close(RESULT);

#print "$scopefile\n";
print "$SEARCH_FOOTER\n$ASIM_FOOTER\n";


# REMOTE_ADDR = 132.227.103.10
# HTTP_X_FORWARDED_FOR = 132.227.103.10

$CLIENT = $ENV{"HTTP_X_FORWARDED_FOR"};

if ( $CLIENT eq "" ) {
$CLIENT = $ENV{"REMOTE_ADDR"};
}


$TIME=strftime("%Y/%m/%d %H:%M:%S",localtime());
open(LOCK,">$lock") || die "Can't open lock : $!";
close (LOCK);
open(SORTIE, ">>$log") || die "Can't open log : $!";
print SORTIE "$TIME	$CLIENT	\"$scope\"	\"$str\"	$nmatch\n";;
close (SORTIE);
unlink("$lock") || die "Can't delete lock : $!";

}

exit 0
__END__


