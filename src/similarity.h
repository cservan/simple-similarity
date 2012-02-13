/*
 * Generic hashmap manipulation functions
 */
#ifndef __SIMILARITY_H__
#define __SIMILARITY_H__
#include <boost/functional/hash.hpp>
#include "tools.h"
// #include "stringHasher.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <locale>


using namespace std;
using namespace Tools;

    class similarity
    {
        private:
            vector< string > m_similarityContent;
            vector< vector <int> > m_similarityVector;
            vector< float > m_similarityResult;
            vector< string > m_docNames;
	    int m_documentSize;
	    bool m_lengthRatio;
        public:
	    void addTfIdfData(vector<string> vs);
	    void addDocNames(vector<string> vs);
	    void addDocNames(string vs);
	    void calculateSimilarity(vector< string > vs, string s);
	    void calculateSimilarity(vector< string > vs, string s, int ngramSize);
	    void sortResults();
	    string printResults();
	    void setLengthRatio(bool b);
    };


#endif
