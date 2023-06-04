#!/usr/bin/env bash

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source "${scriptdir}"/common_functions.sh

#Check cmake is installed
if ! command -v cmake >/dev/null 2>&1
then
    echo -e "\n\t${RED}ERROR${RESTORE}: The command cmake isn't installed\n"
    exit 1
fi

root_dir=$(readlink -f "${BASH_SOURCE%/*}/../")

# Make sure the caller is at the root_dir so the presets can be found
if [[ "${PWD}" != "${root_dir}" ]]
then
  echo -e "\t${RED}ERROR${RESTORE} : You must run this script from the root of the project (i.e. ${root_dir})"
  exit 1
fi

function run_CI {
  cmake --preset "${1}" && \
  cmake --build --preset "${1}" && \
  ctest --preset "${1}"
}

presets=(
  "gcc-Debug" \
  "gcc-Release" \
  "clang-Debug" \
  "clang-Release"
)

for p in "${presets[@]}"
do
  outfile="cmake_output_${p}.log"
  run_CI "${p}" >"${outfile}" 2>&1 ; ERROR=$?
  if [[ "${ERROR}" -eq 0 ]]
  then
    echo -en "[${GREEN}PASS${RESTORE}] "
  else
    echo -en "[${RED}FAIL${RESTORE}] "
  fi
  echo "${p}"
done

exit 0
