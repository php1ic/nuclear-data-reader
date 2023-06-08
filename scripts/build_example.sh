#!/usr/bin/env bash

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source "${scriptdir}"/common_functions.sh

pushd . || exit

cd "${scriptdir}"/../example || exit
cmake -S. -Bbuild
cmake --build ./build
./build/ndr

popd || exit

exit 0
