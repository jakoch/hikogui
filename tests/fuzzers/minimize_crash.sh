#!/bin/bash

set -e
[[ $1 ]]

dir="$( dirname "$0" )"

data="$dir/../data/fuzz"

if [ ! -e "$data/corpus" ]
then
    echo "Fuzzer Input Data folder not found ("$data/corpus")."
    echo "This script must be run from the fuzzers folder."
    read -p "Press any key..."
    exit 2
fi

read -p "Press any key..."

"$dir/fuzzer" -minimize_crash=1 -max_total_time=10 -artifact_prefix="$data/artifacts/" $1