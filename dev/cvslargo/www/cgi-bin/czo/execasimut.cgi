#!/usr/local/bin/perl -w
use CGI;

#start_multipart_form();
$query = new CGI;


$TMP="/tmp/ExecAsimut.$$" ;

mkdir ($TMP , 0777) ;

print "Content-type: text/html\n\n";
#while (($key, $val) = each %ENV) {
#	print "$key = $val<BR>\n";
#}

print "<HTML>\n";
print "<head>\n";
print "<Title> Simulation Asimut</Title>\n";
print "</head>\n";
print "<body>\n";
print "<PRE>\n";

       # Read a text file and print it out
 #                 while (<$filename>) {
  #                   print "$_<BR>\n\n";
   #               }

#                 $type = $query->uploadInfo($filename)->{'Content-Type'};
#                 unless ($type eq 'text/html') {
#                    die "HTML FILES ONLY!";
#                 }                 
                 
$filename = $query->param('vbe');
#print "<BR><BR>$filename<BR>\n\n";

# Copy a binary file to somewhere safe
                  open (OUTFILE,">>$TMP/description.vbe");
                  while ($bytesread=read($filename,$buffer,1024)) {
                     print OUTFILE $buffer;
                  }
                  close (OUTFILE);

$filename = $query->param('pat');
                 
                  open (OUTFILE,">>$TMP/unproc_patterns.pat");
                  while ($bytesread=read($filename,$buffer,1024)) {
                     print OUTFILE $buffer;
                  }
                  close (OUTFILE);
                 
                 

#exec ( "( sleep 50 ; /asim/gnu/bin/rm -fr $TMP ) & " ) ;
#print "<BR>Exec done...\n<BR>";

open ( RESULT,  "( cd $TMP ; . /users/soft5/newlabo/alliance/share/etc/alc_env.sh  ; asimut -b description unproc_patterns res_patterns ) 2>&1 |" );

while (<RESULT>) {
print ;
}
close RESULT ;
print "<BR>done...\n<BR>";
print "<BR>Vous pouvez recuper le fichier de pat simulé <a href=$TMP/res_patterns.pat>ici</a>\n<BR>";
print "</PRE>\n";


print "</body>\n";
print "</HTML>\n";

