#!/bin/bash

# run the NanoCSV
filepath=$1
bcmd=/test/runExperiments.sh

${filepath}${bcmd} kindle   true 32 Nice\ vintage\ story 9 ${filepath}
${filepath}${bcmd} bgg15    true 32 mitnachtKAUBO-I 2 ${filepath}
${filepath}${bcmd} meta     true 32 6b0567729c2143a66d737eb0a2f63f2dce2e5a7d 2 ${filepath}
${filepath}${bcmd} yelp     true 32 mh_-eMZ6K5RLWhZyISBhwA 2 ${filepath}
${filepath}${bcmd} canvas   true 32 6NSgFa1CvIPly1VniNhlbrmoN3vgDFbMSKqh+c4TTfrr3dMib91oUWONX96g5PPcioIxedF24ldNOu/g5yqDrg== 2 ${filepath}
