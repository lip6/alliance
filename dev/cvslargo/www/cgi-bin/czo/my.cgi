#!/usr/local/bin/perl -w

#============================================================================
# Copyright 1999 Mikodocs Guide to HTML, http://www.mikodocs.com/
# You may distribute this program freely, but keep 
# this notice in place.
#============================================================================


#============================================================================
# modules
#
use strict;
use CGI;
use CGI::Carp 'fatalsToBrowser', 'croak';
$CGI::POST_MAX = 10240; # set maximum size of post
#
# modules
#============================================================================


#============================================================================
# variables
#
my (
    $query,       # CGI query object
    $maxFields,   # maximum number of fields to accept
    $rand,        # random number for banners
    $paramKey,    # a single parameter name
    $inputdata,   # filehandle for uploaded files
    @inputVals,   # values for a field (they can send more than one)
    $inputVal,    # a single values for a field
    $modval,      # modified value of 
    $fileinfo,    # uploaded file information
    @params,      # array of all fields sent
    $key,$val,    # key an value for $fileinfo hash
    );
$query=CGI->new;           # get CGI object
$maxFields=30;             # maximum number of fields to accept
$rand = int(rand(100000)); # random number for ad banner
#
# variables
#============================================================================


#============================================================================
# top of page
#
print $query->header;
print <<"(TOPOFPAGE)";
<HTML>
<HEAD>
<TITLE>Mikodocs Guide to HTML: My CGI</TITLE>
</HEAD>
<BODY>

<H1>Env</H1>

(TOPOFPAGE)
#
# top of page
#============================================================================


#============================================================================
# list
#

# get list of fields
@params = $query->param;
if (@params > $maxFields)
    {croak "No more than $maxFields fields please"}

# open table if necessary
print "<HR><P><TABLE BORDER CELLPADDING=4>\n" if @params;

foreach $paramKey (@params)
    {
    print "<TR VALIGN=TOP>";

    #----------------------------------------------------------------
    # value
    #
    $inputdata=$query->param($paramKey);
    $fileinfo=$query->uploadInfo($inputdata);

    # if it's an uploaded file
    if (defined($fileinfo))
        {
        print "<TH>", tablesc($paramKey), "</TH>"; # name of field
        print "<TD><TABLE BORDER BGCOLOR=\"#FFFFCC\" CELLPADDING=4>\n";
        while (($key,$val) = each %{$fileinfo})
            {
            # content-disposition gives several pieces of 
            # information about the file, so let's parse it up
            if (lc($key) eq "content-disposition")
                {
                my ($dis,@dispieces);
                foreach $dis (split(m|\s*;\s*|,$val))
                    {
                    @dispieces=split("\s*=\s*",$dis,2);
                    next unless defined $dispieces[1];
                    next if $dispieces[0] =~ m|^name$|i;
                    print 
                    "<TR><TD>",
                        tablesc($dispieces[0]),
                        "</TD><TD><CODE>",
                        tablesc($dispieces[1]),
                        "</CODE></TD></TR>\n";
                    }
                }

            # else just print the field
            else
                {
                print 
                    "<TR><TD>",
                    tablesc($key),
                    "</TD><TD><CODE>",
                    tablesc($val),
                    "</CODE></TD></TR>\n";
                }
            }

        # output the file size
        seek($inputdata,0,2);   # go to end of file handle
        print "<TR><TD>size</TD><TD>", 
            tell($inputdata),
            "</TD></TR>\n";
        print "</TD></TABLE>";  # 
        }

    # else it's not an uploaded file
    else
        {
        # get array of input values
        @inputVals=$query->param($paramKey);
        @inputVals=grep(tablesc($_),@inputVals);

        # name of field
        print "<TH ROWSPAN=", 
            scalar(@inputVals) ,">", 
            tablesc($paramKey), "</TH><TD><PRE>";

        #out put value(s) for field
        foreach $inputVal (@inputVals)
            {$inputVal = tablesc($inputVal)}
        print join("</PRE></TD></TR>\n<TR><TD><PRE>",@inputVals), 
            "</PRE></TD>";
        }
    #
    # value
    #----------------------------------------------------------------

    print "</TR>\n";
    }

# close table if necessary
print "</TABLE><P>\n" if @params;

#
# list
#============================================================================


#============================================================================
# bottom of document
#

# ************************************************************************
# *** IF YOU COPY My CGI, ABSOLUTELY PLEASE DO NOT CHANGE THIS SECTION ***
# ************************************************************************

print <<"(END BOTTOM OF PAGE)";
<HR><P>
</BODY>
</HTML>
(END BOTTOM OF PAGE)
#
# bottom of document
#============================================================================


#============================================================================
# tablesc
# return &nbsp; undefined or for space-only/empty string,
# otherwise change < > and & to character entities
#
sub tablesc
{
return "&nbsp;" if (! defined $_[0]) || ($_[0] !~ m|\S|);
$_[0] =~ s|&|&#38;|gso;
$_[0] =~ s|<|&#60;|gso;
$_[0] =~ s|>|&#62;|gso;
$_[0];
}
#
# tablesc
#============================================================================
