#!/usr/local/bin/perl

#- Configurable Variables ---------------------------------------------------#

#Path to SWISH-E executable
$swish = "/usr/local/bin/swish-e";

#Path to this CGI script
$swishcgi = "/cgi-bin/searchres.cgi";

#Path to the SWISH-E index
$index = "/local/largo2/wwwpages/cgi-bin/index.swish-e";

#Path to the SWISH-E configuration file
$config = "/opt/swish/swish-e/ConfigFiles/SwishTest.conf";

#- Main Program -------------------------------------------------------------#

#Parse the form data
&parse_form_data (*FORM);

#Construct a simple query
$query = $FORM{'query'};

$results = $FORM{'results'};

if ($query) {
   &search_parse;
}else{
   &search_error("You must enter a keyword or phrase in one or more of the text boxes");
}

#- Subroutines ---------------------------------------------------------------#

#Subroutine for checking and parsing the incoming form data.

sub parse_form_data {

local (*FORM_DATA) = @_;
local ($request_method, $query_string, @key_value_pairs, $key_value, $key, $value);

$request_method = $ENV{'REQUEST_METHOD'};

if ($request_method eq "GET") {
   $query_string = $ENV{'QUERY_STRING'};
} elsif ($request_method eq "POST") {
   read (STDIN, $query_string, $ENV{'CONTENT_LENGTH'});
} else {
   &search_error ("Forms must use either GET or POST.");
}

@key_value_pairs = split (/&/, $query_string);

foreach $key_value (@key_value_pairs) {
   ($key,  $value) = split (/=/, $key_value);
   $value =~ tr/+/ /;
   $value =~ s/%([\dA-Fa-f][\dA-Fa-f])/pack ("C", hex ($1))/eg;
   if (defined ($FORM_DATA{$key})) {
      $FORM_DATA{$key} = join ("\0", $FORM_DATA{$key}, $value);
   } else {
      $FORM_DATA{$key} = $value;
   }
}

}

#Subroutine for constructing the Swish-E search request and formating the results.

sub search_parse {

#Create your SWISH-E query command

$count=0;

open(SWISH, "$swish -w $query -m $results -f $index|");

#Check for errors

while (<SWISH>) {
   chop;

   if ($_ eq "err: no results") 
   {&search_error("There were no items that matched your search request");}

   if ($_ eq "err: could not open index file") 
   {&search_error("Could not open SWISH Index File $index");}
   
   if ($_ eq "err: no search words specified") 
   {&search_error("Please Enter at least one Search Word");}

   if ($_ eq "err: a word is too common") 
   {&search_error("One of your search terms is too common, please try again");}
  
   next if /^\D/;
   $count++;
   push(@results, $_);

}

#Print the results page

&html_header("Search Results");

print <<Search_Results;
<P ALIGN=CENTER>
Swish found the following items, listed in order of computed relevance<BR>
Your Search for <strong>$query</strong>, returned $count Items
</P>
<HR>
<UL>
Search_Results

foreach (@results) {
   select(STDOUT); 
   ($stringone, $title, $filesize) = split(/\"/, $_);
   ($rank, $url) = split(/ /, $stringone);
   print "<LI><A HREF=\"$url\">$title</A><BR>\n";
}

print "</UL>\n";
&html_trailer;

}

#Subroutine for printing a generic HTML header.

sub html_header {

$document_title = $_[0];

print <<HTML_Header;
Content-type: text/html

<HTML>
<HEAD>
<TITLE>$document_title</TITLE>
</HEAD>
<BODY BGCOLOR="#ffffff">
<H1 ALIGN=CENTER>$document_title</H1>
HTML_Header
}

#Subroutine for printing a generic HTML trailer.

sub html_trailer {

print <<HTML_Trailer;
<HR>
</BODY>
</HTML>
HTML_Trailer

exit;

}

#Subroutine for printing error messages.

sub search_error {

&html_header("Search Error");
$error_message = $_[0];
print "<P ALIGN=CENTER>\n$error_message</P>\n";
&html_trailer;

}
