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
	push @map, join(', ', map{ ''.lookup($_).'' } @keys);
}

print "#define KEYMAP_$name { /* Generated keymap for $name */ ";
for my $line (@map) {
	print "\t{". $line ."},\\\n";
}
print "},\n";

sub lookup {

my %table = (
'{' => 'Key_LCurlyBracket',
'}' => 'Key_RCurlyBracket',
'['=> 'Key_LSquareBracket',
']'=> 'Key_RSquareBracket',
'|'=> 'Key_Pipe',
'\\' => 'Key_Backslash',

	';' => 'Key_Semicolon',
	',' => 'Key_Comma',
	'.' => 'Key_Period',
	'/' => 'Key_Slash',
	"'" => 'Key_Quote',
	'`' => 'Key_Backtick',
	'-' => 'Key_Minus',
	'=' => 'Key_Equals');

  my $x = shift;
  return $x if $x =~ /\(.*\)/;
	return 'Key_'.$x unless defined $table{$x};
	return $table{$x};
}
