/*
 * Generic hashmap manipulation functions
 */
#ifndef __PHRASE_H__
#define __PHRASE_H__
#include <boost/functional/hash.hpp>
#include "tools.h"
// #include "stringHasher.h"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;
using namespace Tools;

    class phrase
    {
        private:
	    size_t m_srcHash;
	    size_t m_tgtHash;
            string m_srcContent;
            string m_tgtContent;
	    vector <double> m_probs;
	    vector< string > m_rest;
	    
        public:
	    phrase(string s);
	    size_t getSrcHash();
	    size_t getTgtHash();
	    string getSrcContent();
	    string getTgtContent();
	    vector <double> getProbs();
	    vector <string> getRest();
	    string toString();
    };


#endif
