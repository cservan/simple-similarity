/*
 * Generic hashmap manipulation functions
 */
#ifndef __TFIDF_H__
#define __TFIDF_H__
#include <boost/functional/hash.hpp>
#include "boost/regex.hpp"
#include "tools.h"
// #include "stringHasher.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <locale>
#include "myIndex.h"

using namespace std;
using namespace Tools;

    class tfidf
    {
        private:
            vector<size_t> m_tester;
            vector< pair <size_t,string> > m_testerContent;
            vector< pair <size_t,int> > m_testerNgramInfos;
            vector< pair <size_t,vector <unsigned long> > > m_testerCount;
	    vector<int> m_ngramCount;
	    unsigned long m_documentSize;
	    vector<float> vecTf;
	    vector<float> vecIdf;
	    vector<float> vecTfidf;
	    vector<pair <string, vector<float> > > m_vec_infos_pairs;
	    bool m_debugMode;
        public:

	    tfidf();
// 	    size_t hashValue ( string key );
	    bool chercher ( size_t searchKey );
	    int donnerPos ( size_t searchKey );
	    bool supprimerPos(size_t searchKey);
	    void sortHash();
	    void addDatas(vector<string> s, int ngramSize);
	    void addDatas(string inputs, vector<string> data, int ngramSize);
	    void addDatas(myIndex & query, myIndex & l_index, int ngramSize, unsigned long docNbr);
	    void addDatas(vector<string> s);
	    string printDatas();
	    string printDatasSorted();
	    string printDatasSorted(int nbest);
	    vector<string> getContent(int n);
	    vector<size_t> getContentIds(int n);
	    void compileData();
	    void compileData(vector<string> vs);
	    float getScore(string s) ;
	    void setDebugMode(bool b);
	    
//             bool trouve ( size_t searchKey );
//             bool trouve ( string key );
//             void addHasher ( string key, string value );
//             void addHasherUncheck ( string key, string value );
//             stringHasher getHasher ( string key );
//             string getValue ( string key );
//             string searchValue ( string key );
//             void setValue ( string key , string value );
//             void printHash();
//             vector<stringHasher> getHashMap();
//             string printStringHash();
//             string printStringHash2();
//             string printStringHashForLexicon();
    };


#endif
