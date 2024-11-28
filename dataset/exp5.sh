#!/bin/bash

# run the NanoCSV
filepath=$1
bcmd=/test/runExperiments.sh

${filepath}${bcmd} kindle   true 32 none 1 ${filepath}
${filepath}${bcmd} bgg15    true 32 none 1 ${filepath}
${filepath}${bcmd} meta     true 32 none 1 ${filepath}
${filepath}${bcmd} yelp     true 32 none 1 ${filepath}
${filepath}${bcmd} canvas   true 32 none 1 ${filepath}
