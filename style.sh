#!/usr/bin/env bash

echo "Running auto checkstyle!"

while read st file; do
    # skip deleted files
    if [ "$st" == 'D' ]; then continue; fi

    # do a check only on the cpp h files
    if [[ ( $file == *.cpp ) || ( $file == *.h ) ]]; then
        echo "✏️ styling: $file"
        clang-format -i $file
    fi

# enumerates modified files and pipe as input
done < <(git diff --cached --name-status)

# unfortunately, we can't commit again as it will either cause recursion due to hooks
# even if we set flags such as --no-verify, I can't seem to make git recognize the commits to be the same author...
echo "Done, please commit again to include the styled files! 🤠"

# exit 0 to ensure no issues
exit 0
