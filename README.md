<!--
**nanocsv/nanocsv** is a ✨ _special_ ✨ repository because its `README.md` (this file) appears on your GitHub profile.

Here are some ideas to get you started:

- 🔭 I’m currently working on ...
- 🌱 I’m currently learning ...
- 👯 I’m looking to collaborate on ...
- 🤔 I’m looking for help with ...
- 💬 Ask me about ...
- 📫 How to reach me: ...
- 😄 Pronouns: ...
- ⚡ Fun fact: ...
-->
# Reproducibility Submission DASFAA Paper#819

## Basic Environment

Paper Name: NanoCSV: Enabling Fast and Programmable CSV Processing with Hierarchical Finite-State Transducer

DatasetsUsed:

|Dataset|URL|Size|
|:-----:|:-:|:--:|
|ids5|https://www.kaggle.com/datasets/solarmainframe/ids-intrusion-csv|328M|
|issue|https://www.kaggle.com/datasets/yanivaknin/fafdata|1.39GB|
|agents|https://www.kaggle.com/datasets/kneroma/lyft-motion-prediction-autonomous-vehicles-as-csv|1.64GB|
|hpatrain|https://www.kaggle.com/datasets/dschettler8845/hpa-train-data-with-additional-metadata|2.57GB|
|custom|https://www.kaggle.com/datasets/zanjibar/100-million-data-csv|4.54GB|
|higgs|https://archive.ics.uci.edu/dataset/280/higgs|8.04GB|
|2019oct|https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-datafrom-multi-category-store|5.67GB|
|2019nov|https://www.kaggle.com/datasets/mkechinov/ecommerce-behavior-datafrom-multi-category-store|9.01GB|
|kindle|https://www.kaggle.com/datasets/bharadwaj6/kindlereviews|701MB|
|bgg15|https://www.kaggle.com/datasets/jvanelteren/boardgamegeek-reviews|1.39GB|
|meta|https://www.kaggle.com/datasets/allen-institute-for-ai/CORD-19-research-challenge|1.65GB|
|yelp|https://www.yelp.com/dataset|4.72GB|
|canvas|https://www.kaggle.com/datasets/antoinecarpentier/redditrplacecsv|21.7GB|

Environmental requirement: 

GCC: v11.4.0

Compile Option: -mavx -mavx2 -mfma -mpopcnt -msse4 -mavx512bw -mavx512vl -lpthread -fopenmp

## Quick use by testdata

Please enter to the `test` folder. 
Then use `make`, and please complete the required dependencies. 
We will see `chunk_csv_test.out` file. 

`./chunk_csv_test.out ../testdata/yelp_30000.csv true $threads $query_text $which_column`

The first parameter `true` represents whether there is a header row. And in yelp\_30000.csv it has. 
The second parameter represents the number of available threads. 
The third parameter represents the string of the query. 
The fourth argument represents which column the string is in. 

The result will be `column: x. record: y. text: z.`. 
And the parse time `|--IO time: x--|--Parse time: y--|--Total time: z--|`. 
