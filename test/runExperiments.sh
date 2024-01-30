#!/bin/bash

dataset=$1   # the file path of data
linetitle=$2 # have or not have headline (True or False)
thread=$3    # available threads (number, eg: 32)
querytext=$4 # what do you want to query (return column and raw)
col=$5       # the column number start at 1, if 0, means global traversal
filepath=$6

i=1
for i in {1..5}; do
    SCRIPT="$CMD ./chunk_csv_test.out ${filepath}/dataset/${dataset}.csv\
    ${linetitle}\
    ${thread}\
    ${querytext}\
    ${col}"
    echo $SCRIPT

    echo 3 >/proc/sys/vm/drop_caches && sync
    sleep 3
    cd test
    make
    start=$(date +%s%N)
    $SCRIPT
    end=$(date +%s%N)
    cd ..
    echo "NanoCSV,"${dataset}","$((($end - $start) / 1000000)) >> ${filepath}/results/test.dat
done
