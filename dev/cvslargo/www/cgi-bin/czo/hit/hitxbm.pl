#! /usr/local/bin/perl
#
# HITXBM.PL - Module required by hitmat.cgi
#
#    Copyright: 1996,1997,1998 by Joe DePasquale
# Last revised: January 28, 1997
#       E-Mail: crypt@getcruising.com
#      Website: http://www.GetCruising.com
#
###########################################################################
# The bitmap data file @gened is used with the author's permission.
#     number.xbm  (C) 1994 Eugene E. Devereaux <gened@halcyon.com>
# Adapted for UUNET (NCSA httpd) by David MacKenzie <djm@va.pubnix.com>
###########################################################################

# Sub: Make an x-bitmap image of the count

sub makeXbm
{
  ($printTtl,$digits,$XBM) = @_;

  $printTtl = substr ($printTtl,0);
  $pad = $digits - length ($printTtl);
  $xbmWidth = $digits * 16;

  for ($x=0; $x<$digits; $x++)
  { if ($x<$pad) { $printTtl = "0".$printTtl; }
    push @xbmNbr, substr ($printTtl,$x,1);
  }

  # assign bitmap byte octals for numbers 0..9 to list of lists
  @gened = (
  ['00','00','e0','07','f0','0f','30','0c','30','0c','30','0c','30','0c','30','0c',
   '30','0c','30','0c','30','0c','30','0c','30','0c','f0','0f','e0','07','00','00'],
  ['00','00','80','01','c0','01','e0','01','80','01','80','01','80','01','80','01',
   '80','01','80','01','80','01','80','01','80','01','e0','07','e0','07','00','00'],
  ['00','00','e0','07','f8','1f','18','18','18','18','18','1c','00','0e','00','07',
   '80','03','c0','01','e0','00','70','00','38','00','f8','1f','f8','1f','00','00'],
  ['00','00','f0','07','f0','0f','30','0c','00','0c','00','0c','00','0c','c0','0f',
   'c0','0f','00','0c','00','0c','00','0c','30','0c','f0','0f','f0','07','00','00'],
  ['00','00','00','06','00','0f','80','0f','c0','0d','e0','0c','70','0c','f0','0f',
   'f0','0f','00','0c','00','0c','00','0c','00','0c','00','0c','00','0c','00','00'],
  ['00','00','e0','0f','f0','0f','30','00','30','00','30','00','30','00','f0','07',
   'e0','0f','00','0c','00','0c','00','0c','30','0c','f0','07','e0','03','00','00'],
  ['00','00','20','00','30','00','30','00','30','00','30','00','30','00','f0','07',
   'f0','0f','30','0c','30','0c','30','0c','30','0c','f0','0f','e0','0f','00','00'],
  ['00','00','f0','0f','f0','0f','30','0c','30','0c','00','06','00','03','00','03',
   '80','01','80','01','c0','00','c0','00','60','00','60','00','60','00','00','00'],
  ['00','00','e0','07','f0','0f','30','0c','30','0c','30','0c','60','06','c0','03',
   'c0','03','60','06','30','0c','30','0c','30','0c','f0','0f','e0','07','00','00'],
  ['00','00','e0','07','f0','0f','30','0c','30','0c','30','0c','e0','0f','c0','0f',
   '00','0e','00','0e','00','07','80','03','c0','01','e0','00','60','00','00','00'],
  );

  # create image file of printTtl
  open (XBM,">$XBM");
  flock (XBM,2); seek (XBM,0,0);
  print (XBM "#define hit_width $xbmWidth\n");
  print (XBM "#define hit_height 16\n");
  print (XBM "static char hit_bits[] = {\n");

  for ($y=0; $y<=30; $y=$y+2)
  { foreach $x (@xbmNbr)
    { print (XBM "0x$gened[$x][$y],0x$gened[$x][$y+1],");
    }
    print (XBM "\n");
  }
  print (XBM "};\n");
  truncate (XBM,tell(XBM)); close (XBM);
} # end makeXbm

1; # return true
