#! /usr/bin/perl -wpi
# pi.bak
# Filename: mysed.pl
# Copyright (C) 1999, 2000 Czo <Olivier.Sirol@lip6.fr>
# License: GPL (http://www.gnu.org/copyleft/gpl.html)
# Created: April 2000
# Last Change: Monday 21 May 2001, 16:59
# Edit Time: 1:38:09
# Description:
#
# $Id: mysed.pl,v 1.1 2002/04/29 14:15:32 czo Exp $
#

#use POSIX;
#$TIME=strftime("%Y/%m/%d at %H:%M",localtime());
#undef $/;           # read in whole file, not just one line or paragraph
#s§§§smig;

s§\bgetenv\b§mbkgetenv§g;

#    $ perl -pi'old/*.bak' -e 's/bar/baz/' fileA # backup to 'old/fileA.bak'
#    $ perl -pi'.bak' -e 's/bar/baz/' fileA      # backup to 'fileA.bak'
#    $ perl -pi'*.bak' -e 's/bar/baz/' fileA     # backup to 'fileA.bak'


