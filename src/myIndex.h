/*
 * Generic hashmap manipulation functions
 */
#ifndef __INDEX_H__
#define __INDEX_H__
// #include <boost/functional/hash.hpp>
#include "tools.h"
// #include "stringHasher.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
// #include "element.h"

using namespace std;
using namespace Tools;

    class myIndex
    {
        private:
	    
	    multimap< size_t, multimap< unsigned long, unsigned long >  > m_mapIndex;
	    multimap< size_t , string > m_mapInfo;
	    multimap<unsigned long , multimap <size_t , unsigned long  > > m_mapReverseIndex;
	    multimap<unsigned long , unsigned long > m_mapDocSize;
// 	    size_t m_srcHash;
// 	    size_t m_tgtHash;
//             string m_srcContent;
//             string m_tgtContent;
// 	    vector <double> m_probs;
// 	    vector< string > m_rest;
	    
        public:
	    myIndex(string s, unsigned long docId);
	    myIndex();
	    void addIndex(string s, long unsigned int docId, bool direct, bool reverse);
	    multimap< size_t, multimap< unsigned long, unsigned long >  > & getMyIndex();
	    multimap< size_t , string > & getMyIndexMapInfo();
	    multimap<unsigned long , multimap <size_t , unsigned long  > > & getMyReverseIndex();
	    long unsigned int getReversInfos(long unsigned int docId, size_t hashValue);
	    void compileComplementDataForOkapi();
	    string toString();
	    multimap<unsigned long , unsigned long > getMapDocSize();
	    multimap< size_t, multimap< unsigned long, unsigned long >  > getMapIndex();
// 	    size_t getSrcHash();
// 	    size_t getTgtHash();
// 	    string getSrcContent();
// // 	    string getTgtContent();
// 	    vector <double> getProbs();
// 	    vector <string> getRest();
// 	    string toString();
    };


#endif
