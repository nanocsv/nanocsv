#!/bin/bash

# run the NanoCSV
filepath=$1
bcmd=/test/runExperiments.sh

${filepath}${bcmd} ids5     true 32 none 1 ${filepath}
${filepath}${bcmd} issue    true 32 none 1 ${filepath}
${filepath}${bcmd} agents   true 32 none 1 ${filepath}
${filepath}${bcmd} hpatrain true 32 none 1 ${filepath}
${filepath}${bcmd} custom   true 32 none 1 ${filepath}
${filepath}${bcmd} higgs    true 32 none 1 ${filepath}
${filepath}${bcmd} 2019oct  true 32 none 1 ${filepath}
${filepath}${bcmd} 2019nov  true 32 none 1 ${filepath}
