#!/usr/bin/perl
#
# $Id: map-idx.pl,v 1.1 2013/01/29 13:36:23 urs Exp $

@max = split(',', shift);
@req = split(',', shift);

die unless ($#max == $#req);

$n = $#req + 1;

lookup_init(@max);
lookup_dump();

printf("%d\n", lookup(@req));

sub lookup_init {
    my (@max) = @_;
    my ($i, $j, $h, $s);

    # Indices for heap 0
    for ($i = 0; $i <= $max[0] + 1; $i++) {
	$offset{0, $i} = $i;
    }

    # Indices for heap 1 to n-1
    for ($h = 1; $h <= $#max; $h++) {
	for ($i = 0; $i <= $max[$h] + 1; $i++) {
	    my ($s);
	    for ($j = 0; $j <= $i; $j++) {
		if ($j <= $max[$h - 1]) {
		    $s += $offset{$h - 1, $j};
		} else {
		    $s += $offset{$h - 1, $max[$h - 1] + 1};
		}
	    }
	    $offset{$h, $i} = $s;
	}
    }
}

sub lookup_dump {
    my ($i, $h);

    for ($h = 0; $h < $n; $h++) {
	for ($i = 0; $i <= $max[$h] + 1; $i++) {
	    print " ", $offset{$h, $i};
	}
	print "\n";
    }
}

sub lookup {
    my (@heaps) = @_;
    my ($h, $idx);

    $idx = 0;
    for ($h = 0; $h <= $#heaps; $h++) {
	$idx += $offset{$h, $heaps[$h]};
    }
    return $idx;
}
