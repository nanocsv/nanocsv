#!/bin/bash

# run the NanoCSV
filepath=$1
bcmd=/test/runExperiments.sh

${filepath}${bcmd} ids5     true 32 21/02/2018\ 08:33:06 3 ${filepath}
${filepath}${bcmd} issue    true 32 TransportReverseLoadUnits 10 ${filepath}
${filepath}${bcmd} agents   true 32 -2173.760009765625 2 ${filepath}
${filepath}${bcmd} hpatrain true 32 5c27f04c-bb99-11e8-b2b9-ac1f6b6435d0 1 ${filepath}
${filepath}${bcmd} custom   true 32 35843 1 ${filepath}
${filepath}${bcmd} higgs    true 32 1.000000000000000000e+00 1 ${filepath}
${filepath}${bcmd} 2019oct  true 32 appliances.environment.water_heater 5 ${filepath}
${filepath}${bcmd} 2019nov  true 32 electronics.smartphone 5 ${filepath}
