#!/bin/sh

printf '%s\n\n' '## Examplaire du code'

for file in "$@"; do
    title=$(basename $file)
    printf '%s\n\n' "### $title"
    printf '\lstinputlisting[language=c]{%s}\n\n' "$file"
done
