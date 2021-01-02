#!/usr/bin/env bash

set -euo pipefail

# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source "${scriptdir}"/common_functions.sh

#Check lcov is installed
if ! command -v lcov >/dev/null 2>&1
then
    echo -e "\n\t${RED}ERROR${RESTORE}: Code coverage tool is set to lcov but it isn't installed\n"
    exit 1
fi

#Check genhtml is installed
if ! command -v genhtml >/dev/null 2>&1
then
    echo -e "\n\t${RED}ERROR${RESTORE}: Webpage generation tool is set to genhtml but it isn't installed\n"
    exit 1
fi

# No need for absolute paths
OUTFILE="coverage.info"

# Process the files
lcov --directory . --capture --output-file "${OUTFILE}"
# Ignore/remove those we have no control over
lcov --remove "${OUTFILE}" '/usr/*' '*/tests/*' '*/external/*' --output-file "${OUTFILE}"

# Print the results to the terminal for a quick overview
lcov --list "${OUTFILE}"
# Create a webpage with more detail
genhtml "${OUTFILE}" --output-dir=coverage

exit $?
