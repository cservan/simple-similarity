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
#include "myIndex.h"
#include <boost/bind.hpp>
#include <boost/thread.hpp>

using namespace std;
using namespace Tools;

    class similarity
    {
        private:
            vector< string > m_similarityContent;
            vector< size_t > m_similarityContentIds;
            vector< vector <unsigned long> > m_similarityVector;
            vector< float > m_similarityResult;
            vector< string > m_docNames;
	    int m_documentSize;
	    bool m_lengthRatio;
	    boost::thread_group m_threads;
	    myIndex * m_data_index;
	    float m_sumSquareContent;
	    vector <unsigned long> m_contentSimVector;
	    

        public:
	    void addTfIdfData(vector<string> vs);
	    void addTfIdfDataIds(vector<size_t> vs);
	    void addDocNames(vector<string> vs);
	    void addDocNames(string vs);
	    void calculateSimilarity(vector< string > vs, string s);
	    void calculateSimilarity(vector< string > vs, string s, int ngramSize);
	    void calculateSimilarity(myIndex & data_index, string s, int ngramSize);
	    void sortResults();
	    string printResults();
	    string printResults(int nbest);
	    void setLengthRatio(bool b);
	    void evaluate(myIndex& data_index, int l_vsInc, std::vector< unsigned long > l_contentSimVector, float l_sumSquareContent);
	    void evaluate(int & l_vsInc);
	    string toString();
    };


#endif
