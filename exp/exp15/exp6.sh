#!/bin/bash

filepath=$1

# In 1.0ver, we choose the best blocksize to every CSV file. 
# If you try to replication our experiment. 
# Please see `matchingBlock` function in "RecordLoader.cpp", 
# and change the `blockSize` parameter. 
