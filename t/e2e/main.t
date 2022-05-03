#!/usr/bin/perl

use warnings;
use strict;

use Test::More;

use constant DARWIN_KERNEL => 'darwin';
use constant LINUX_KERNEL => 'linux';

my $KERNEL = "linux";
my $SRC_PATH = "../../src";

my $output_uname = lc `uname -a`;
if(index $output_uname, DARWIN_KERNEL != -1) {
	$KERNEL = "darwin";
}

if($KERNEL eq DARWIN_KERNEL) {
	system "clang $SRC_PATH/main.c $SRC_PATH/b_tree.c $SRC_PATH/disk_operation.c -g -fdebug-macro -arch arm64 -o REPL" ;
}
else {
	system "clang $SRC_PATH/main.c $SRC_PATH/b_tree.c $SRC_PATH/disk_operation.c -g -v -o REPL" ;
}

sub execute_main_c {
	my $o = `./REPL`;
}

is();

__END__
