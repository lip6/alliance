#! /usr/local/bin/perl
#
# HITCFG.PL
# File maintenance module required by HITMATIC
#
#     Filename: hitcfg.pl
# Last revised: July 08, 1998
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

sub checkCfg # Maintain hit.log, hitx.dat and hit.cfg
{
  open (CFG,"+<$HITCFG") || &endIt;
  flock (CFG,2); seek (CFG,0,0);
  $cfgFile = <CFG>;
  ($logDay,$datMonth,$cntMonth,$cntFlag,$eol) = split (/\|/,$cfgFile);

  # If not done since midnight ...

  # If new day, add daily hits to month file
  if ($logDay != $dayStamp)
  { &newDay;
  }
  # If new month, empty last year's month file and update cfg
  if ($datMonth+0 != $month)
  { &newMonth;
  }
  # If new count, recount hits since cntMonth
  if ($cntFlag eq 'Y')
  { require "./hitxbm.pl";
    &newCount;
  }
  close (CFG);
} # end checkCfg

sub newDay
{
  $HITDAT = "$HITMON$datMonth.dat";
  @dyinMon = (0,31,28,31,30,31,30,31,31,30,31,30,31);
  $dyinMon = $dyinMon[$datMonth];

  open (DAT,"+<$HITDAT") || &endIt;
  flock (DAT,2); seek (DAT,0,0);
  @hitDat = <DAT>;

  # Backup monthly data if trigger
  if ($bakFlag eq 'Y' && ($jDate % $bakTime ==0) && !-e $HITBAK)
  { &backUp (@hitDat);
  }

  # add this month's hits to Tbl hash
  foreach $hitLine (@hitDat)
  { @hits = split (/\|/,$hitLine);
    $page = $hits[0];

    $Tbl{$page}[0] = $page;
    for $day (1..$dyinMon)
    { $Tbl{$page}[$day] += $hits[$day];
    }
    $Index{$page} = 'Y';
  }
  
  # Read log file
  open (LOG1,"+<$HITLOG1") || &endIt;
  flock (LOG1,2); seek (LOG1,0,0);
  @logFile = <LOG1>;

  # add yesterday's hits to Tbl hash
  foreach $logLine (@logFile)
  { @log = split (/\|/,$logLine,3);
    $page = $log[1];

    # must create new line if no previous hits
    if (!$Tbl{$page}[0])
    { $Tbl{$page}[0] = $page;
      for $day (1..$dyinMon)
      { $Tbl{$page}[$day] +=0;
      }
      $Index{$page} = 'Y';
    }
    $Tbl{$page}[$logDay]++;

# Count unique ip visitors per group on logday
#    $group,$foo = split (/\+/,$page);
#    if (!$Ip{$ENV{'REMOTE_ADDR'}}{$group}
#    { $Ip{$ENV{'REMOTE_ADDR'}}{$group} = 'Y';
#      $Tbl{"~IP\+$group"}[$logDay] ++;
#    }
#

  }

  # write updated Tbl back to file
  seek (DAT,0,0);

  foreach $page (sort {uc($a) cmp uc($b)} keys %Index)
  { $line = '';
    for $day (0..$dyinMon)
    { $line .= "$Tbl{$page}[$day]\|";
    }
    print DAT "$line\n";
  }
  truncate (DAT,tell(DAT)); close (DAT);

  open (LOG2,">$HITLOG2") || &endIt;
  flock (LOG2,2); seek (LOG2,0,0);
  print (LOG2 @logFile);
  truncate (LOG2,tell(LOG2)); close (LOG2);

  truncate (LOG1,0); close (LOG1);

  $logDay = $dayStamp;
  &changeCfg;

} # end newDay

sub newMonth
{
  # Empty last year's month file and update cfg if new month
  $datMonth = $month;
  $HITDAT = "$HITMON$datMonth.dat";

  open (DAT,">$HITDAT") || &endIt;
  flock (DAT,2);
  truncate (DAT,0); close (DAT);
  &changeCfg;

} # end newMonth

sub newCount
{
  foreach $x (1..12)
  { if (($cntMonth <= $monthStamp && $x >= $cntMonth && $x <= $monthStamp)
    || ($cntMonth > $monthStamp && ($x <= $monthStamp || $x >= $cntMonth)))
    {
      $HITDAT = "$HITMON$x.dat";
      open (DAT,"<$HITDAT") || &endIt;
      flock (DAT,1); seek (DAT,0,0);
      while ($hitLine = <DAT>)
      { @hits = split (/\|/,$hitLine);
        $page = $hits[0];
        foreach $day (1..31)
        { $PageTtl{$page} += $hits[$day];
        }
      }
      close (DAT);
    }
  }
  # Create a new hit.cnt file with this data
  while (($pagecode,$pagecount) = each %PageTtl)
  { $addLine = join ("\|",$pagecount,$pagecode,"\n");
    push @inFile, $addLine;
  }
  @outFile = sort {($b =~ /(\d+)/)[0] <=> ($a =~ /(\d+)/)[0]} @inFile;

  open (CNT,">$HITCNT") || &endIt;
  flock (CNT,2); seek (CNT,0,0);
  print (CNT @outFile);
  truncate (CNT,tell(CNT));
  close (CNT);

  if ($monthStamp >= $cntMonth)
  { $year = $yearStamp +0;
  } else
  { $year = $yearStamp -1;
  }
  &makeXbm ("$cntMonth".substr("$year",2,2),4,"$hitmatDir/$SINCEXBM");

  $cntFlag = '';
  &changeCfg;
} # end newCount

sub changeCfg  # update hit cfg file
{
  $cfgFile = join ("\|",$logDay,$datMonth,$cntMonth,$cntFlag,$eol);
  seek (CFG,0,0);
  print (CFG $cfgFile);
  truncate (CFG,tell(CFG));
} # end changeCfg

sub backUp # Backup data and delete old backups
{
  @bakData = @_;
  open (BAK,">$HITBAK") || &endIt;
  print (BAK @bakData);
  close (BAK);
  chmod 0666, $HITBAK;

  opendir (BAKDIR,$bakDir);
  @bakFiles = grep (/hit\d{3}\.bak/, readdir(BAKDIR));
  closedir (BAKDIR);

  foreach $bakFile (@bakFiles)
  { if (-M "$bakDir/$bakFile" > $bakMax) { unlink "$bakDir/$bakFile"; }
  }
} # end backup

1; # return true
