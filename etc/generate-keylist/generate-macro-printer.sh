PROGRAM=/tmp/list-macros.cpp
echo '#include <stdio.h>' > ${PROGRAM}
echo '#include "kaleidoscope/key_defs.h"' >> ${PROGRAM}
echo '#include "kaleidoscope/HIDTables.h"' >> ${PROGRAM}
echo 'int main() {' >> ${PROGRAM}
echo 'printf("{\\n");' >> ${PROGRAM}

# Handle non-argument-taking macros
while IFS= read -r macro; do
    echo "printf(\"'$macro':  %d,\", $macro.getRaw());" >> ${PROGRAM}
    echo "printf(\"\\\n\");" >> ${PROGRAM}
done < /tmp/non_arg_macros.txt

# Handle argument-taking macros, generating 32 versions for each
while IFS= read -r macro; do
    # Strip off parentheses for the argument-taking macro names
    macro=$(echo $macro | sed 's/(.*)//')
    for i in {0..31}; do
        echo "printf(\"'$macro$i':  %d,\", $macro($i).getRaw());" >> ${PROGRAM}
        echo "printf(\"\\\n\");" >> ${PROGRAM}
    done
done < /tmp/arg_macros.txt

echo 'printf("};\\n");' >> ${PROGRAM}
echo 'return 0;' >> ${PROGRAM}
echo '}' >> ${PROGRAM}

