#!/usr/local/bin/perl
use CGI;

$WAISDIR="/usr/local/home/alex/cvs/alex/MPC-OS/docs/www/waissrc";
$HEADER=`/bin/cat /usr/local/home/alex/cvs/alex/MPC-OS/docs/www/header.html`;
$FOOTER=`/bin/cat /usr/local/home/alex/cvs/alex/MPC-OS/docs/www/footer.html`;
$START=`/bin/cat /usr/local/home/alex/cvs/alex/MPC-OS/docs/www/start.html`;
$STOP=`/bin/cat /usr/local/home/alex/cvs/alex/MPC-OS/docs/www/stop.html`;

print "Content-type: text/html\n\n";

$query = new CGI;

$str = $query->param('searchstring');

$str =~ s/[^a-zA-Z0-9- ]//g;
$str =~ s/ -/ /g;
$str =~ s/^-/ /;

print "$START\n$HEADER\n";

print '<H2><FONT COLOR="#660000">Search result for query "'.$str.'"</FONT></H2><P>  <FONT FACE="ARIAL,HELVETICA">';

open RESULT, "/usr/local/bin/waisq -f - -g -s $WAISDIR -S mpc.src -g $str | grep :headline |";

print '<TABLE BORDER="0">';

print "<PRE>";
$cpt = 0;
while (<RESULT>) {
    chop;
    m/^#/ && next;
    m/:headline "(.*) (.*)"/ && do { $file = $2.$1; };
    $file =~ s%.*cvs/alex/MPC-OS/docs/www%%;
    $cpt++;
    print '<TR><TD align=right>'.$cpt.'- </TD><TD> <A HREF="http://mpc.lip6.fr'.$file.'">http://mpc.lip6.fr'.$file.'</A><BR></TR>';

}

close(RESULT);

print '</TABLE></FONT></P>
    <form method="POST" action="/cgi-bin/search.cgi">
      <CENTER><STRONG>Search:</STRONG> <input type=text name="searchstring" value="" size=50>
	<!-- <input src="images/search.gif" type=submit value="  Search  "> -->
</CENTER>
    </form>
';
print "$FOOTER\n$STOP\n";

__END__

