#!/usr/local/bin/perl -w

use Fcntl ':flock'; # import LOCK_* constants

$res=flock("/tmp/my.lock", LOCK_EX);
print $res;
system ("ls -alrt /tmp");
sleep 10;

exit 0
__END__




sub lock {
    flock(MBOX,LOCK_EX);
    # and, in case someone appended
    # while we were waiting...
    seek(MBOX, 0, 2);
}


 sub unlock {
    flock(MBOX,LOCK_UN);
}

open(MBOX, ">>/usr/spool/mail/$ENV{'USER'}")
        or die "Can't open mailbox: $!";

lock();
print MBOX $msg,"\n\n";
unlock();



