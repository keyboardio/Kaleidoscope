use warnings;
use strict;

my @data = <STDIN>;

use Data::Dumper;

my $name = 'Unnamed';
my @map;
for my $line (@data) {
	chomp($line);
	if ($line =~ /^#NAME: (.*)/i) {
		$name = $1;
		next
	}

	my @keys = split(/\s+/, $line);
	push @map, join(', ', map{ 'Key_'.lookup($_).'' } @keys);
}

print "#define KEYMAP_$name { /* Generated keymap for $name */ ";
for my $line (@map) {
	print "\t{". $line ."},\\\n";
}
print "},\n";

sub lookup {

my %table = (
'{' => 'LCurlyBracket',
'}' => 'RCurlyBracket',
'['=> 'LSquareBracket',
']'=> 'RSquareBracket',
'|'=> 'Pipe',
'\\' => 'Backslash',

	';' => 'Semicolon',
	',' => 'Comma',
	'.' => 'Period',
	'/' => 'Slash',
	"'" => 'Quote',
	'`' => 'Backtick',
	'-' => 'Minus',
	'=' => 'Equals');

	my $x = shift;
	return $x unless defined $table{$x};
	return $table{$x};
}
