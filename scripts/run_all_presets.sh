#!/usr/bin/env bash

scriptdir=$(readlink -f "${BASH_SOURCE%/*}")
#shellcheck source=/dev/null
source "${scriptdir}"/common_functions.sh

VERBOSE=0

# Assume that the same name is used for config, build and test
# If it isn't this script will fail with the assumption
PRESETS=(
  "gcc-Debug" \
  "gcc-Release" \
  "clang-Debug" \
  "clang-Release"
)

#TODO: Allow the preset(s) to be passed as arguments
function usage {
  echo -e "
  ${BLUE}USAGE${RESTORE}: ${BASH_SOURCE##*/} [-v]

  Optional Arguments:
    -v  Show the result status of the individual steps (not the full output), use as ON/OFF flag [default: OFF]
  "

  exit 1
}

#Check cmake is installed
if ! command -v cmake >/dev/null 2>&1
then
    echo -e "\n\t${RED}ERROR${RESTORE}: The command cmake isn't installed\n"
    exit 2
fi

root_dir=$(readlink -f "${BASH_SOURCE%/*}/../")

# Make sure the caller is at the root_dir so the presets can be found
if [[ "${PWD}" != "${root_dir}" ]]
then
  echo -e "\t${RED}ERROR${RESTORE} : You must run this script from the root of the project (i.e. ${root_dir})"
  exit 3
fi

while getopts ":hv" OPTIONS
do
    case "${OPTIONS}" in
        h | \? | : )
            usage
            ;;
        v )
            VERBOSE=1
            ;;
    esac
done
shift $((OPTIND-1))

function run_step {
  # 1 = preset name
  # 2 = console string
  # 3 = command
  # 4 = output file
  local ERROR_CODE
  local MESSAGE="Running ${1} ${2}"

  if [[ $VERBOSE -eq 1 ]]
  then
    echo -en "[ -- ] ${MESSAGE}"
  fi

  ${3} "${1}" >> "${4}" 2>&1 ; ERROR_CODE=$?

  if [[ $VERBOSE -eq 1 ]]
  then
    if [[ ${ERROR_CODE} -eq 0 ]]
    then
      echo -e "\r[${GREEN}PASS${RESTORE}] ${MESSAGE}"
    else
      echo -e "\r[${RED}FAIL${RESTORE}] ${MESSAGE}"
    fi
  fi
}

function run_config {
  run_step "${1}" "config" "cmake --preset" "${2}"
}

function run_build {
  run_step "${1}" "build" "cmake --build --preset" "${2}"
}

function run_test {
  run_step "${1}" "tests" "ctest --preset" "${2}"
}

function run_CI {
  run_config "${1}" "${2}" && \
  run_build "${1}" "${2}" && \
  run_test "${1}" "${2}"
}

function summary {
  if [[ "${1}" -eq 0 ]]
  then
    echo -en "[${GREEN}PASS${RESTORE}] "
  else
    echo -en "[${RED}FAIL${RESTORE}] "
  fi

  echo "${2}"
}

for p in "${PRESETS[@]}"
do
  outfile="cmake_output_${p}.log"
  # Without 'true', linters complain
  true > "${outfile}"

  run_CI "${p}" "${outfile}" ; RETURNVAL=$?

  if [[ ${VERBOSE} -eq 0 ]]
  then
    summary "${RETURNVAL}" "${p}"
  else
    echo " ---- "
  fi
done

exit 0
