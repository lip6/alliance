#! /usr/local/bin/perl
#
# HITMAN - The HITMATIC Manager
# Displays and reports various hit count statistics.
#
#     Filename: hitman.pl
# Last revised: March 15, 1998 
#    Copyright: 1997,1998 by Joe DePasquale
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

sub hitman {

@labels = ('Foo','Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec');
@dyinMon = (0,31,28,31,30,31,30,31,31,30,31,30,31);

# Parse form input
read (STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
@cgiPairs = split(/&/,$buffer);
foreach $cgiPair (@cgiPairs)
{
  ($name, $value) = split(/=/,$cgiPair);
  $name =~ s/\+/ /g;  $value =~ s/\+/ /g;
  $name =~ s/%(..)/pack("c",hex($1))/ge;
  $value =~ s/%(..)/pack("c",hex($1))/ge;
  $Form{$name} .= "\0" if (defined($Form{$name}));
  $Form{$name} .= "$value";
}
undef $name; undef $value;

print "Content-type: text/html\n\n";
print "<html>\n<head><title>$headTitle</title></head>\n";
print "$bodyTag\n$bodyTitle<p>\n";

&passtest ($HITPWD);
print qq|<form action="$scriptUrl" method=POST>\n|;
print qq|<input type=hidden name=password value="$Form{'password'}">\n|;

########################################################################
# Case: View Hit stats by year

if ($Form{'hittable'})
{
  if ($Form{'detail'} && $Form{'detail'} =~ /^(M|G|P)\|(.+)/)
  {
    if ($1 eq "M") # month
    { $month = $2;
      print "<font size=4><b>Hits By MONTH for: $labels[$month] ";
      if ($monthStamp >= $month) {$year = $yearStamp +0;} else {$year = $yearStamp -1;}
      print "$year</b></font><br>\n";
      if ($year %4 == 0) { $dyinMon[2] = 29; }

      # Read all data into hashes
      $total = 0;

      $HITDAT = "$HITMON$month.dat";
      open (DAT,"<$HITDAT") || &endMan ("Read: $HITDAT");
      flock (DAT,1); seek (DAT,0,0);

      while ($line = <DAT>)
      { @hits = split (/\|/,$line);
        $page = $hits[0];
        if ($page =~ /(.+)\+(.+)/) { $group =$1; }

        foreach $day (1..$dyinMon[$month])
        { $Tbl{$page}{$day} += $hits[$day];
          $PageTtl{$page} += $hits[$day];
          if ($group)
          { $Tbl{$group}{$day} += $hits[$day];
            $GroupTtl{$group} += $hits[$day];
          }
          $DayTtl{$day} += $hits[$day];
        }
        $total += $PageTtl{$page};
      }
      close (DAT);

      @pages = sort {$PageTtl{$b} <=> $PageTtl{$a}} keys %PageTtl;
      @groups = sort {$GroupTtl{$b} <=> $GroupTtl{$a}} keys %GroupTtl;

      print qq|<table border=$border width=50%><tr align=center>\n|;
      print qq|<td align=left><input type=reset value="RESET"><br><font color="#009900"><b>Days:</b></font></td>\n|;

      foreach $day (1..$dyinMon[$month]) { print qq|<td><font color="#009900"><b>$day</b></font></td>|; }
      print qq|<td><font color="#0000FF"><b>Group<br>/Page<br>Totals</b></font></td>|;

      # print group hit rows
      print qq|\n</tr><tr>\n<td><b>GROUPS:</b></td><td colspan=|,($dyinMon[$month]-1),qq|></td>|;

      foreach $group (@groups)
      { print qq|\n</tr><tr align=right>\n<td align=left> <input type=radio name=detail value="G\|$group"><font color="#0000FF">$group</font></td>|;
        foreach $day (1..$dyinMon[$month]) { print qq|<td>$Tbl{$group}{$day}</td>|; }
        print qq|<td>$GroupTtl{$group}</td>|;
      }
      # print page hit rows
      print qq|\n</tr><tr>\n<td><b>PAGES:</b></td><td colspan=|,($dyinMon[$month]-1),qq|></td>|;

      foreach $page (@pages)
      { print qq|\n</tr><tr align=right>\n<td align=left> <input type=radio name=detail value="P\|$page"><font color="#0000FF">$page</font></td>|;
        foreach $day (1..$dyinMon[$month]) { print qq|<td>$Tbl{$page}{$day}</td>|; }
        print qq|<td>$PageTtl{$page}</td>|;
      }
      print qq|\n</tr><tr align=right>\n<td nowrap align=center><font color="#009900"><b>Day Totals</b></font></td>|;

      foreach $day (1..$dyinMon[$month]) { print qq|<td>$DayTtl{$day}</td>|; }
      print qq|<th>$total</th>|;
      print qq|\n</tr></table><br>Completed on $timeStamp\n<p>\n|;
    }
    else # group or page
    {
      print "<font size=4><b>Hits By ";
      if ($1 eq "G")
      { $group = $2; 
        print "GROUP for &quot;$group&quot;";
      } else
      { $page = $2;
        print "PAGE for &quot;$page&quot;";
      }
      print "</b></font><br>\n";

      # Read all data into hashes
      $total = 0;

      foreach $month (1..12)
      { $MonthTtl{$month} =0;
        $HITDAT = "$HITMON$month.dat";
        open (DAT,"<$HITDAT") || &endMan ("Read: $HITDAT");
        flock (DAT,1); seek (DAT,0,0);
        while ($line = <DAT>)
        { @hits = split (/\|/,$line);
          if ($hits[0] eq $page || $hits[0] =~ /^$group\+/)
          { foreach $day (1..31)
            { $Tbl{$month}{$day} += $hits[$day];
              $DayTtl{$day} += $hits[$day];
              $MonthTtl{$month} += $Tbl{$month}{$day};
            }
          }
        }
        close (DAT);
        $total += $MonthTtl{$month};
      }
      print "<table border=$border><tr align=center>\n";
      print "<td align=left><input type=reset value=\"RESET\"><br><font color=\"#009900\"><b>Months:</b></font></td>\n";

      # Print col headers and order the months
      $month = $monthStamp;
      do
      { if ($month == 12) {$month =1;} else {$month++;}
        print "<td><input type=radio name=detail value=\"M\|$month\"><br><font color=\"#009900\"><b>$labels[$month]</b><br>";
        if ($month > $monthStamp) {$year = $yearStamp -1;} else {$year = $yearStamp +0;}
        print "$year</font></td>\n";
        push @monthCol, $month;
      } until ($month == $monthStamp);
      print "<td><font color=\"#0000FF\"><b>Day<br>Totals</b></font></td>";
 
      foreach $day (1..31)
      { print "\n</tr><tr align=right>\n<td><font color=\"#0000FF\"><b>$day</b></font></td>";
        foreach $month (@monthCol) { print "<td>$Tbl{$month}{$day}</td>"; }
        print "<td>$DayTtl{$day}</td>";
      }
      print "\n</tr><tr align=right>\n<td nowrap align=center><font color=\"#009900\"><b>Month Totals</b></font></td>";

      foreach $month (@monthCol) { print "<td>$MonthTtl{$month}</td>"; }
      print "<th>$total</th>";
      print "\n</tr></table><br>Completed on $timeStamp\n<p>\n";
    }
  }
  else # default by year
  {
    print "<font size=4><b>Hits By YEAR as of $timeStamp</b></font><br>\n";
    # Read all data into hashes
    $total = 0;
    foreach $month (1..12)
    { $MonthTtl{$month} =0;
     
      $HITDAT = "$HITMON$month.dat";
      open (DAT,"<$HITDAT") || &endMan ("Read: $HITDAT");
      flock (DAT,1); seek (DAT,0,0);
      while ($line = <DAT>)
      { @hits = split (/\|/,$line);
        $page = $hits[0];
        if ($page =~ /(.+)\+(.+)/) { $group =$1; }
        $Tbl{$month}{$page} =0;

        foreach $day (1..31)
        { $Tbl{$month}{$page} += $hits[$day];
        }
        $PageTtl{$page} += $Tbl{$month}{$page};
        $MonthTtl{$month} += $Tbl{$month}{$page};

        if ($group)
        { $Tbl{$month}{$group} += $Tbl{$month}{$page};
          $GroupTtl{$group} += $Tbl{$month}{$page};
        }
      }
      close (DAT);
      $total += $MonthTtl{$month};
    }
    # Sort by descending page total
    @pages = sort {$PageTtl{$b} <=> $PageTtl{$a}} keys %PageTtl;
    @groups = sort {$GroupTtl{$b} <=> $GroupTtl{$a}} keys %GroupTtl;

    print "<table border=$border><tr align=center>\n";
    print "<td align=left><input type=reset value=\"RESET\"><br><font color=\"#009900\"><b>Months:</b></font></td>\n";

    # Print col headers and order the months
    $month = $monthStamp;
    do
    { if ($month == 12) {$month =1;} else {$month++;}
      print "<td><input type=radio name=detail value=\"M\|$month\"><br><font color=\"#009900\"><b>$labels[$month]</b><br>";
      if ($month > $monthStamp) {$year = $yearStamp -1;} else {$year = $yearStamp +0;}
      print "$year</font></td>\n";
      push @monthCol, $month;
    } until ($month == $monthStamp);
    print "<td><font color=\"#0000FF\"><b>Group<br>/Page<br>Totals</b></font></td>";

    # print group hit rows
    print "\n</tr><tr>\n<td><b>GROUPS:</b></td><td colspan=13></td>";
    foreach $group (@groups)
    { print "\n</tr><tr align=right>\n<td align=left><input type=radio name=detail value=\"G\|$group\"><font color=\"#0000FF\">$group</font></td>";
      foreach $month (@monthCol) { print "<td>$Tbl{$month}{$group}</td>"; }
      print "<td>$GroupTtl{$group}</td>";
    }

    # print page hit rows
    print "\n</tr><tr>\n<td><b>PAGES:</b></td><td colspan=13></td>";
    foreach $page (@pages)
    { print "\n</tr><tr align=right>\n<td align=left><input type=radio name=detail value=\"P\|$page\"><font color=\"#0000FF\">$page</font></td>";
      foreach $month (@monthCol) { print "<td>$Tbl{$month}{$page}</td>"; }
      print "<td>$PageTtl{$page}</td>";
    }
    # print month subtotal row
    print "\n</tr><tr align=right>\n<td nowrap align=center><font color=\"#009900\"><b>Month Totals</b></font></td>";

    foreach $month (@monthCol) { print "<td>$MonthTtl{$month}</td>"; }
    print "<th>$total</th>";
    print "\n</tr></table><p>\n";
  } # end default by year

  print <<__HITFORM;
<table border=0 width=300>
<tr><td><font color="#0000FF" size=5><b>Hit Tables</b></font></td></tr>
<tr><td>For more detail select any <b>MONTH, GROUP or PAGE</b> button above, then..</td></tr>
<tr><td>PRESS: <input type=submit name=hittable value="   HIT TABLES   "></td></tr>
<tr><td><font color="#0000FF" size=5><b>Hit Counter</b></font></td></tr>
<tr><td>For the count <b>DISPLAYED</b> to users or to <b>RESET</b> counter date..</td></tr>
<tr><td>PRESS: <input type=submit value="HIT COUNTER"></td></tr>
</table>
__HITFORM
  &manFooter;
} # end case hittable

########################################################################
# Case: Flag to reset hitcount at next daily maintenance

elsif ($Form{'resetcount'})
{
  open (LOCK,">$HITFLK") || &endMan ("Write: $HITFLK");
  if (!flock (LOCK,2)) { &endMan ("Lock: $HITFLK"); }

  # Write new cnt date to CFG
  open (CFG,"+<$HITCFG") || &endMan ("Read\/Write: $HITCFG");
  flock (CFG,2); seek (CFG,0,0);
  $cfgFile = <CFG>;
  ($logDay,$datMonth,$cntMonth,$cntFlag,$eol) = split (/\|/,$cfgFile);

  $cntMonth = $Form{'setmonth'} +0;
  $cntFlag = 'Y';
  $cfgFile = join ("\|",$logDay,$datMonth,$cntMonth,$cntFlag,$eol);

  seek (CFG,0,0);
  print (CFG $cfgFile);
  truncate (CFG,tell(CFG)); close (CFG);
  close (LOCK);

  print "<b>Hit Counter</b> will be reset at next daily maintenance<br>\n to <b>$labels[$cntMonth] 1, $year</b>\n<p>\n";

  &hitCount;
  &manFooter;
} # end resetcount

########################################################################
# Default Case: Display hitcount

else
{ &hitCount;
  &manFooter;
} # end default
} # end hitman

########################################################################

sub hitCount # Display hitcount
{
  open (CNT,"<$HITCNT") || &endMan ("Read: $HITCNT");
  flock (CNT,1); seek (CNT,0,0);
  @hitFile = <CNT>;
  close (CNT);

  print "<table border=$border><tr>\n<th>HITS</th><th>by PAGECODE</th>\n";
  $hitTtl =0;
  for ($x=0; $x<=$#hitFile; $x++)
  { print "\n</tr><tr>\n";
    ($count,$code,$eol) = split (/\|/,$hitFile[$x]);
    if ($code =~/\+/)
    { ($group,$page) = split (/\+/,$code);
      $GroupTtl{$group} += $count;
    }
    print "<td align=right>$count</td><td>$code</td>";
    $hitTtl = $hitTtl + $count;
  }
  print "\n</tr></table><p>\n";

  if ($group)
  { print "<table border=$border><tr>\n<th>HITS</th><th>by GROUP</th>\n";
    @groups = sort {$GroupTtl{$b} <=> $GroupTtl{$a}} keys %GroupTtl;
    foreach $group (@groups)
    { print "\n</tr><tr>\n";
      print "<td align=right>$GroupTtl{$group}</td><td>$group</td>";
    }
    print "\n</tr></table><p>\n";
  }
  open (CFG,"<$HITCFG") || &endMan ("Read: $HITCNT");
  flock (CFG,1); seek (CFG,0,0);
  $cfgFile = <CFG>;
  close (CFG);

  ($logDay,$datMonth,$cntMonth,$cntFlag,$eol) = split (/\|/,$cfgFile);

  print "<b>Total: $hitTtl hits for $x pages since $labels[$cntMonth] 1, ";
  if ($monthStamp >= $cntMonth) {$year = $yearStamp +0;} else {$year = $yearStamp -1;}
  print "$year</b>\n<br>Completed at $timeStamp\n<p>\n";

  print qq|<table border=0 width=300>\n<tr><td><font color="#0000FF"><font size=5><b>Hit Counter</b></font></font></td</tr>\n|;
  print qq|<tr><td>To reset the counter and starting date, select a starting <b>MONTH</b> and press <b>CHANGE</b>. A new image of the <b>SINCE</b> date will also be created.</td></tr>\n|;
  print qq|<tr><td>SINCE: <select name=setmonth size=1>\n|;
  for $x (1..12)
  { if ($x == $cntMonth)
    { print "<option value=$x selected>$labels[$x]\n";
    } else
    { print "<option value=$x>$labels[$x]\n";
    }
  }
  print qq|</select> \n<input type=submit name=resetcount value="CHANGE"> \n|;
  print qq|<input type=submit value="  CLEAR  "></td></tr>\n|;

  print qq|<tr><td><font color="#0000FF"><font size=5><b>Hit Tables</b></font></font></td></tr>\n|;
  print qq|<tr><td>For ALL hits by <b>MONTH, GROUP or PAGE</b></td></tr>\n|;
  print qq|<tr><td>PRESS: <input type=submit name=hittable value="  HIT TABLES  "></td></tr>\n</table>\n|;
} # end hitCount

sub manFooter # print hitman footer
{
  print qq|</form><p>\n<a href="$exitUrl">Exit $headTitle</a>\n|;
  print qq|<p><font size=2><i><b>HITMATIC</b> is one of <a href="http://www.GetCruising.com/crypt" target="_top">Joe's CGI Scripts From The Crypt!</a></i></font>\n|;
  print qq|<p>\n</body></html>\n|;
} # end manFooter

sub passtest  # password routines
{
  $PASSFILE = $_[0];
  if (-s $PASSFILE)
  { if ($Form{'password'})
    { open (PASS,"<$PASSFILE");
      $passcode = <PASS>;
      close (PASS);
      chop ($passcode) if $passcode =~ /\n$/;
      $lp = length $Form{'password'};
      $salt = substr($Form{'password'},$lp-2,1).substr($Form{'password'},1,1);
      if ($passcode eq crypt ($Form{'password'},$salt))
      { if ($Form{'newpassword'})
        { $lp = length $Form{'newpassword'};
          if ($Form{'newpassword'} =~ /^\w{$lp}\b/)
          { $Form{'password'} = $Form{'newpassword'};
            $salt = substr($Form{'password'},$lp-2,1).substr($Form{'password'},1,1);
            $passcode = crypt ($Form{'password'},$salt);
            open (PASS,">$PASSFILE");
            print PASS "$passcode";
            close (PASS);
          } else
          { &passbox ("Bad choice, Creep!!");
          }
        }
      } else
      { &passbox ("Wrong, Creep!!");
      }
    } else
    { &passbox ("What's the Password, Creep?");
    }
  } else
  { &endMan ("Password File $PASSFILE Not Found");
  }
}
sub passbox
{
  print <<__PASSBOX;
  <form action="$scriptUrl" method="POST">
  <input type=hidden name=manager value=1><table border=4>
  <tr><th colspan=2><font color="#FF0000"><b><blink>$_[0]</blink></b></font></th></tr>
  <tr><th> Enter Password:</td><th><input type=password name=password size=12 maxlength=12></th></tr>
  <tr><th>Change Password:</td><th><input type=text name=newpassword size=12 maxlength=12></th></tr>
  <th colspan=2><input type=submit value="  DO IT!  "><input type=reset value="  CLEAR  "></th></tr>
  </table><br>
  Password must be 4 to 12 alpha-numeric characters.</form>
  <p></body></html>
__PASSBOX
  exit;
} # end passbox

sub endMan # Error handling for manager scripts
{ 
  print "<font color=\"#FF0000\"><b>ERROR:</font><br>$_[0]</b>\n<p>";
  if ($!) { print "Server made a Boo-Boo! $!\n"; }
  else { print "Use your browser's [BACK] button and try again.\n"; }
  print "<p></body></html>\n";
  exit;
} # end endMan

1; # return true
