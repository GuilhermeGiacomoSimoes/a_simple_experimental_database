#!/usr/bin/perl

use warnings;
use strict;

use Test::More;

use constant DARWIN_KERNEL => 'darwin';
use constant LINUX_KERNEL => 'linux';

my $KERNEL = "linux";

my $output_uname = lc `uname -a`;
if(index $output_uname, DARWIN_KERNEL != -1) {
	$KERNEL = "darwin";
}

if($KERNEL eq DARWIN_KERNEL) {
	system "" ;
}

is();

__END__
