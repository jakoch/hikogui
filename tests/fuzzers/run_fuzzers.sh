#!/bin/bash

dir="$( dirname "$0" )"

data="$dir/../data"

if [ ! -e "$data/corpus" ]
then
    echo "Fuzzer Input Data folder not found ("$data/corpus")."
    echo "This script must be run from the fuzzers folder."
    read -p "Press any key..."
    exit 2
fi

"$dir/fuzzers" -print_final_stats=1 -artifact_prefix="$data/artifacts/" -jobs=$(( $( nproc ) / 2 )) "$data/corpus"