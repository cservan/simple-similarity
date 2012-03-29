#ifndef __TERCPPTOOLS_H__
#define __TERCPPTOOLS_H__

#include <omp.h>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <boost/xpressive/xpressive.hpp>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/file.hpp> 
#include <map>

using namespace std;

namespace Tools
{
    typedef vector<double> vecDouble;
    typedef vector<char> vecChar;
    typedef vector<int> vecInt;
    typedef vector<float> vecFloat;
    typedef vector<string> vecString;
    typedef vector<string> alignmentElement;
    typedef vector<alignmentElement> WERalignment;
    typedef pair <long, long> pair_countPerDoc;
//     typedef multimap < pair <long, long> > vecULong;
//     typedef multimap< pair < long, int > > map_countPerDoc;
//     typedef pair<size_t, multimap< pair < long, int > > > pair_hashInfo;
//     typedef multimap< pair<size_t, pair<size_t, multimap< pair < unsigned long, int > > > > > map_hashIndex;
    
    
struct param
{
    bool debugMode;
    bool outputAlignments;
    string referenceFile;     // path to the resources
    string hypothesisFile;     // path to the configuration files
    string outputFileExtension;
    string outputFileName;
    string alignmentFile;
    bool noPunct;
    bool caseOn;
    bool normalize;
    bool tercomLike;
    bool sgmlInputs;
    bool noTxtIds;
};
// param = { false, "","","","" };

// class tools{
// private:
// public:

    string vectorToString ( vector<string> vec );
    string vectorToString ( vector<string> vec, string s );
    string vectorToString ( vector<float> vec, string s );
    string vectorToString ( vector<int> vec, string s );
    string vectorToString ( vector<string> vec, string s , int start, int end);
    vector<string> combinerVector ( vector<string> vec1, vector<string> vec2);
    vector<string> subVector ( vector<string> vec, int start, int end );
    vector<int> subVector ( vector<int> vec, int start, int end );
    vector<float> subVector ( vector<float> vec, int start, int end );
    vector<string> copyVector ( vector<string> vec );
    vector<int> copyVector ( vector<int> vec );
    vector<float> copyVector ( vector<float> vec );
    vector<string> stringToVector ( string s, string tok );
    vector<string> stringToVectorV2 ( string s, string tok );
    vector<int> stringToVectorInt ( string s, string tok );
    vector<float> stringToVectorFloat ( string s, string tok );
    string lowerCase(string str);
    string removePunct(string str);
    string tokenizePunct(string str);
    string removePunctTercom(string str);
    string normalizeStd(string str);
    string printParams(param p);
// };
    param copyParam(param p);
    void read_gzip(string fileName, stringstream & to_return);
    size_t hashValueBoost(string s);
//     ifstream openCompressedFile(ifstream file);
    bool write_gzip(stringstream data, string fileName);
//     multimap< pair < size_t, pair<size_t, multimap< pair < unsigned long, int > > > > > index(vector<string> s);
//     multimap< pair < size_t, pair<size_t, multimap< pair < unsigned long, int > > > > > index(vector<vecString> vs);
}
#endif