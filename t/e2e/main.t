#!/usr/bin/perl

use warnings;
use strict;

use Test::More;

use constant DARWIN_KERNEL => 'darwin';
use constant LINUX_KERNEL => 'linux';
use constant CPU_ARCH_x86 => 'x86';
use constant CPU_ARCH_ARM => 'arm';

my $KERNEL = "linux";
my $SRC_PATH = "../../src";

my $output_uname = lc `uname -a`;
if(index $output_uname, DARWIN_KERNEL != -1) {
	$KERNEL = "darwin";
}

my $compile = "clang $SRC_PATH/main.c $SRC_PATH/b_tree.c $SRC_PATH/disk_operation.c -g ";
if($KERNEL eq DARWIN_KERNEL) {
	$compile .= "-fdebug-macro -arch arm64 ";
}
$compile .= " -o test.db";

ok(-e "test.db", "The file exists");

__END__
