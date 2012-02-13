/*
 * Generic hashmap manipulation functions
 */
#ifndef __PTABLE_H__
#define __PTABLE_H__
#include <boost/functional/hash.hpp>
#include "tools.h"
// #include "stringHasher.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "phrase.h"

using namespace std;
using namespace Tools;

    class ptable
    {
        private:
	    vector < phrase > m_phrases;
// 	    size_t m_srcHash;
// 	    size_t m_tgtHash;
//             string m_srcContent;
//             string m_tgtContent;
        public:
	    ptable(string fileName);
	    ptable(string fileName,vector<size_t> ngramHash, bool direction);
	    ptable(string fileName,vector<size_t> ngramHash, bool direction, string outputFile);
	    ptable(string fileName,vector<size_t> ngramHash, bool direction, string outputFile, int size);
	    ptable(string fileName,vector<size_t> ngramHash, bool direction, string outputFile, vector<vecString> inputFile);
	    ptable(string fileName,vector<size_t> ngramHashSrc, vector<size_t> ngramHashTgt, bool direction, string outputFile, vector<vecString> inputFile);
	    string toString();
	    
    };
    

#endif
