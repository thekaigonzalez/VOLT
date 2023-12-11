#!/usr/bin/env perl

# $Id: analyzer.pl

my $VERSION = "v1.0.0";

use v5.14;
use strict;
use warnings;

use Getopt::Long;

my $red = "\e[1;31m";
my $grn = "\e[1;32m";
my $yel = "\e[1;33m";
my $blu = "\e[1;34m";
my $mag = "\e[1;35m";
my $cyn = "\e[1;36m";
my $end = "\e[0m";

my $yeln = "\e[33m";

my $italic = "\e[3m";
my $bold   = "\e[1m";

GetOptions(
    'help'               => \my $help,
    'find-gotos|g!'      => \my $find_gotos,
    'vulnerabilities|v!' => \my $vulnerabilities,
);

sub usage {
    printf( "usage: %s [options...] filename\n", $0 );
    printf( "
options:
  -h, --help             show this help message and exit
  -g, --find-gotos       shows as much information on subroutine calls as possible
  -v, --vulnerabilities  enables vulnerability analysis
" );
    exit $_[0];
}

if ($help) {
    usage(0);
}

if ( @ARGV < 1 ) {
    printf("error: missing filename\n");
    usage(1);
}

my $file = $ARGV[0];
open( my $fh, '<:raw', $file ) or die "Unable to open file $file";

my $len = -s $file;

print "VOLT Binary Analyzer $VERSION\n";
print "Copyright (C) 2023 Fake VOLT Foundation\n\n";

# print basic information about the file
printf("${bold}basic information${end}\n");
printf( "  analyzer version  : ${yel}%s${end}\n", $VERSION );
printf( "  file              : %s\n",             $file );
printf( "  length            : {%d} bytes\n",     $len );

my $width = 1;

if ( $len % 4 == 0 ) {
    $width = 4;
    printf("  architecture      : 32-bit\n");
}
elsif ( $len % 1 == 0 ) {
    printf("  architecture      : 8-bit\n");
}

printf("deep analysis:\n");

my $type_info = "unknown";

# note that mercury line endings are 0xAF,
# unlike 0 in other bytecode formats.
my $last_byte;

# save each subroutine address in an array
my @subroutine_addresses;

while ( read( $fh, my $byte, $width ) ) {
    my $current_unpacked;

    if    ( $width == 4 ) { $current_unpacked = unpack( 'I', $byte ); }
    elsif ( $width == 1 ) { $current_unpacked = unpack( 'C', $byte ); }

    if ( $current_unpacked == 0xAF and $type_info eq "unknown" ) {
        $type_info = "Mercury PIC ${italic}(potentially 32-bit)${end}";
    }
    elsif ( $current_unpacked == 0 and $type_info eq "unknown" ) {
        $type_info = "OpenLUD/NexFUSE";
    }
    elsif ( $current_unpacked == 10 )
    {    # SUB definition, store the label into @subroutine_addresses
        read( $fh, $byte, 4 );

        if ( $width == 1 ) {
            $current_unpacked = unpack( 'C', $byte );
        }
        else {
            $current_unpacked = unpack( 'I', $byte );
        }

        push( @subroutine_addresses, $current_unpacked );

        if ( $find_gotos and $current_unpacked > 0 ) {
            print
"  ${mag}note${end}: `$current_unpacked' subroutine defined here\n";

            if ( $current_unpacked > 256 ) {
                print(
"    ${yeln}problematic${end}: subroutine label too high (segmentation fault)\n"
                );
                print
"    ${yeln}problematic${end}: the label in question: `$current_unpacked'\n";
                print "\n";
            }
        }
    }
    elsif ( $current_unpacked == 15 ) {
        read( $fh, $byte, $width );

        if ( $width == 1 ) {
            $current_unpacked = unpack( 'C', $byte );
        }
        else {
            $current_unpacked = unpack( 'I', $byte );
        }

        if ($find_gotos) {
            print
"  ${mag}note${end}: `$current_unpacked' subroutine called here\n";
            if ( $current_unpacked > 256 and $vulnerabilities ) {
                print
"    ${yeln}problematic${end}: gosub calls label too high, could potentially cause segmentation fault\n";
                print
"    ${yeln}problematic${end}: the label in question: `$current_unpacked'\n";
            }

            # check if this exists in @subroutine_addresses
            if (
                not(
                    grep {
                        $_ == unpack( 'I', $byte ) if $width == 4;
                        $_ == unpack( 'C', $byte ) if $width == 1;
                    } @subroutine_addresses
                )
              )
            {
                print "${red}problem${end}: `jmp' to non-existent address\n";
            }
        }
    }

    if ( $current_unpacked > 256 and $vulnerabilities )
    {    # if its above 256 and vulnerabilities is on
        printf(
"  (-v): ${bold} problem could arise from problematic byte: `%d'${end}\n",
            $current_unpacked );
        printf "     near: ";

        printf( "  ${end}${italic}%d %d${end}\n",
            $last_byte, $current_unpacked )
          if $last_byte;

        if ( $last_byte == 15 ) {    # (GOSUB) on OpenLUD/NexFUSE/MercuryPIC
            print "             ^~~~~~~~~~\n";
            print "              ${mag}gosub${end} definition starts here\n";
        }

        printf(
"  (-v):  ${red}standards say byte sizes should stay below 256${end}\n"
        );

        print "\n";
    }
    $last_byte = $current_unpacked;
}

printf( "  my guess is this file is designed for `%s'\n", $type_info );

close($fh);
