# Find potential argument-taking macros
ggrep --no-filename -oP '^#define \K\w+\s*\([^)]*\)' src/kaleidoscope/key_defs/*.h > /tmp/arg_macros.txt

# Find non-argument-taking macros
ggrep --no-filename -oP '^#define \K\w+' src/kaleidoscope/key_defs/*.h | ggrep -vFf /tmp/arg_macros.txt > /tmp/non_arg_macros.txt

