#!/usr/bin/env bash

set -euo pipefail

# Ignore warning about double backslashes
#shellcheck disable=SC1117

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source "${scriptdir}"/common_functions.sh

#Set the analyser to use
CODECHECKER=cppcheck

#Check it's actually installed
if ! command -v ${CODECHECKER} >/dev/null 2>&1
then
    echo -e "\n\t${RED}ERROR${RESTORE}: Code check tool is set to ${CODECHECKER} but it isn't installed\n"
    exit 1
fi

#Use absolute paths
rootdir="${scriptdir}/../"
sources="${rootdir}/src"
includes="${rootdir}/include"


#Run the analysis
${CODECHECKER} \
    --enable=all \
    --inconclusive \
    --std=c++20 \
    --suppress=missingIncludeSystem \
    -I"${includes}" \
    "${sources}"/*.cpp

exit $?
