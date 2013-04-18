similarity Version 0.6


- predicate :
	* boost library
	* cmake
	* autoconf
	* automake

- to compile and install :

modify the cbuild.sh and set the -DCMAKE_INSTALL_PREFIX= to the directory you want the tercpp and library be installed (i.e. : /usr).
Then run ./cbuild.sh to install

Usage : 
        similarity --in inputFileName --data directoryDataName --out outPutFile [--stopWordsList file] [--lengthRatio] [--ngramSize ngramSize] [--POS nom,adv,adj,...] [--TfIdfOnly|SimilarityOnly]  [--bm25] [--vectSim devDir]:
or 
        similarity --SbyS --in inputFileName --data fileDataName --out outPutFile [--stopWordsList file] [--lengthRatio] [--ngramSize ngramSize] [--nbestTfIdf nbestSize] [--nbestSimilarity nbestSize] [--POS nom,adv,adj,...] [--TfIdfOnly|SimilarityOnly] [--bm25] [--vectSim devDir] [--noSort] [--printFullResults] :

                 --debugMode                    : print debug messages 
                 --SbyS                         : considers each sentence as document in the inputFile and in the Data. 
                 --TfIdfOnly|SimilarityOnly     : calculate only TF.IDF OR calculate only the similarity and take the input as full request
                 --POS nom,adv,adj,...          : if you want to filer the TF.IDF calculation and the similarity calculation by some POS. WARNING: do not use it if documents do not contain any POS!
                 [--lengthRatio]                        : considers the numbers of words in the cosine calculation.
                 [--bm25]                       : use the okapi bm25 instead of the classical tf.idf measure.
                 [--vectSim devDir]                     : use the files contained into the devDir as docs for the vectorisation process of the similarity
                  --help                        : print this help message.


Enjoy !!!

Christophe
(christophe.servan@lium.univ-lemans.fr)
