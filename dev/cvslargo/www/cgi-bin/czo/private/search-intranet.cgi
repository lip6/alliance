#!/asim/gnu/bin/perl
# (C) Copyright 1997,98 Czo <mailto:Olivier.Sirol@asim.lip6.fr>
# Parts (C) A.Fenyo
use CGI;

$ASIM_HEADER=`/bin/cat /users/soft5/webmastr/wwwroot/ssi/asim.header/index.gb.html`;
$ASIM_FOOTER=`/bin/cat /users/soft5/webmastr/wwwroot/ssi/asim.footer/index.gb.html`;
$SEARCH_HEADER=`/bin/cat /users/soft5/webmastr/wwwroot/ssi/search.header-intranet/index.gb.html`;
$SEARCH_MIDDLE=`/bin/cat /users/soft5/webmastr/wwwroot/ssi/search.middle-intranet/index.gb.html`;
$SEARCH_FOOTER=`/bin/cat /users/soft5/webmastr/wwwroot/ssi/search.footer/index.gb.html`;

print "Content-type: text/html\n\n";

$query = new CGI;

$str = $query->param('searchstring');

# Czo 990607 : autorise . et _

$str =~ s/[^a-zA-Z0-9-_ \*\.]//g;
$str =~ s/ -/ /g;
$str =~ s/^-/ /;

print "$ASIM_HEADER\n$SEARCH_HEADER\n";
print '<INPUT NAME=searchstring size=40 maxlength=800 name="searchstring" value="'.$str.'" > ';
print "$SEARCH_MIDDLE\n";




#system (date);
#print system ( "/usr/local/home/czo/local/FreeBSD/bin/swish-e -f /usr/local/home/czo/public_html/cgi-bin/index.swish-e -w $str" );
#$RET = `/usr/local/home/czo/local/FreeBSD/bin/swish-e -c /usr/local/home/czo/public_html/cgi-bin/index.swish-e -w $str`;
#print "<PRE>";
open RESULT, "/asim/gnu/bin/swish-e -m 40 -f /users/soft5/webmastr/wwwroot/cgi-bin/intranet.dat -w $str |";


$cpt = 0;
while (<RESULT>) {
#    chop;
#print ;

    m/err: no results/ && do
    {
      print '<font face="verdana,arial,helvetica" size=2><b>No</b> match found. </font>';
      print '<font face="verdana,arial,helvetica" size=-1>';
     };

    m/^# Number of hits: (.*)/ && do 
    {
      print '<font face="verdana,arial,helvetica" size=-1><b>'.$1.'</b> matches were found. </font>';
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

#        $RR =`/asim/gnu/bin/lynxx -dump $res2 | /asim/gnu/bin/grep -1 $str | /asim/gnu/bin/sed -e 's/\\($str\\)/\\<B\\>\\1\\<\\/B\\>/g'`;
#        print "<TD><SMALL><PRE>Containing : \n$RR\n</PRE></SMALL></TD>\n" ;
        };
#    print '<TR><TD align=right>'.$cpt.'- </TD><TD> <A HREF="http://mpc.lip6.fr'.$file.'">http://mpc.lip6.fr'.$file.'</A><BR></TR>';
#
}
#
close(RESULT);


print "$SEARCH_FOOTER\n$ASIM_FOOTER\n";
exit 0
__END__

