#include "ptable.h"

// The following class defines a hash function for strings


using namespace std;
using namespace Tools;


ptable::ptable(string fileName)
{
    string line;
    ifstream l_fileIn(fileName.c_str());
    vector<string> tmp;
    while (getline(l_fileIn, line))
    {
	phrase p(line);
// 	cerr << line<<endl; 
	m_phrases.push_back(p);
// 	tmp.push_back();
	
    }
//     copy(m_phrases.begin(),m_phrases.end(),ostream_iterator<string>(cerr,"\n"));cerr<<endl;
}
ptable::ptable(string fileName, std::vector< size_t> ngramHash, bool direction)
{
    string line;
    ifstream l_fileIn(fileName.c_str());
    
    vector<string> tmp;
    while (getline(l_fileIn, line))
    {
	phrase p(line);
// 	cerr << line<<endl; 
	if (direction==0)
	{
// 	    if (p.getSrcHash()
	    if (binary_search(ngramHash.begin(), ngramHash.end(), p.getSrcHash()))
	    {
		m_phrases.push_back(p);
	    }
	}
	else
	{
	    if (binary_search(ngramHash.begin(), ngramHash.end(), p.getTgtHash()))
	    {
		m_phrases.push_back(p);
	    }
	}
// 	tmp.push_back();
	
    }
//     copy(m_phrases.begin(),m_phrases.end(),ostream_iterator<string>(cerr,"\n"));cerr<<endl;
}

ptable::ptable(string fileName, std::vector< size_t > ngramHash, bool direction, string outputFile)
{
    string line;
    ifstream l_fileIn(fileName.c_str());
    ofstream l_fileOut(outputFile.c_str());
    vector<string> tmp;
    while (getline(l_fileIn, line))
    {
	phrase p(line);
// 	cerr << line<<endl; 
	if (direction==0) //false = source ; true = target
	{
// 	    if (p.getSrcHash()
	    if (binary_search(ngramHash.begin(), ngramHash.end(), p.getSrcHash()))
	    {
// 		m_phrases.push_back(p);
// 		l_fileOut << line << endl;
		l_fileOut << p.toString() << endl;
	    }
	}
	else
	{
	    if (binary_search(ngramHash.begin(), ngramHash.end(), p.getTgtHash()))
	    {
// 		m_phrases.push_back(p);
// 		l_fileOut << line << endl;
		l_fileOut << p.toString() << endl;
	    }
	}
// 	tmp.push_back();
	
    }
//     copy(m_phrases.begin(),m_phrases.end(),ostream_iterator<string>(cerr,"\n"));cerr<<endl;
    l_fileOut.close();
}

ptable::ptable(string fileName, vector< size_t > ngramHash, bool direction, string outputFile, int size)
{
    string line;
    ifstream l_fileIn(fileName.c_str());
    ofstream l_fileOut(outputFile.c_str());
    vector<string> tmp;
    vector < pair < string,vecString > > vecTranslationsPairs;
    while (getline(l_fileIn, line))
    {
	phrase p(line);
// 	cerr << line<<endl; 
	if (direction==0) //false = source ; true = target
	{
// 	    if (p.getSrcHash()
	    if (binary_search(ngramHash.begin(), ngramHash.end(), p.getSrcHash()))
	    {
// 		m_phrases.push_back(p);
// 		l_fileOut << line << endl;
		string tmp_s=p.getTgtContent();
		vector<string> tmp_vecS=stringToVector(tmp_s," ");
		if ((int)(tmp_vecS.size())==size)
		{
		    bool trouve=false;
		    for (int i=0; i< (int)vecTranslationsPairs.size() && trouve==false; i++)
		    {
			if (vecTranslationsPairs.at(i).first.compare(p.getSrcContent())==0)
			{
			    vecTranslationsPairs.at(i).second.push_back(p.getTgtContent());
			    trouve=true;
			}
		    }
		    if (trouve==false)
		    {
			vector <string> tmp_create;
			tmp_create.push_back(p.getTgtContent());
			pair < string,vecString > tmp_pair_create(p.getSrcContent(),tmp_create);
			vecTranslationsPairs.push_back(tmp_pair_create);
		    }
		}
// 		l_fileOut << p.toString() << endl;
	    }
	}
	else
	{
	    if (binary_search(ngramHash.begin(), ngramHash.end(), p.getTgtHash()))
	    {
// 		m_phrases.push_back(p);
// 		l_fileOut << line << endl;
// 		l_fileOut << p.toString() << endl;
// 		if ((int)(stringToVector(p.getSrcContent()," ").size())==size)
		string tmp_s=p.getTgtContent();
		vector<string> tmp_vecS=stringToVector(tmp_s," ");
		if ((int)(tmp_vecS.size())==size)
		{
		    bool trouve=false;
		    for (int i=0; i< (int)vecTranslationsPairs.size() && trouve==false; i++)
		    {
			if (vecTranslationsPairs.at(i).first.compare(p.getTgtContent())==0)
			{
			    vecTranslationsPairs.at(i).second.push_back(p.getSrcContent());
			    trouve=true;
			}
		    }
		    if (trouve==false)
		    {
			vector <string> tmp_create;
			tmp_create.push_back(p.getSrcContent());
			pair < string,vecString > tmp_pair_create(p.getTgtContent(),tmp_create);
			vecTranslationsPairs.push_back(tmp_pair_create);
		    }
		}
	    }
	}
// 	tmp.push_back();
	
    }
    for (int i=0; i< (int)vecTranslationsPairs.size(); i++)
    {
	l_fileOut << vecTranslationsPairs.at(i).first << "\t" << vectorToString(vecTranslationsPairs.at(i).second,"\t") << endl;
    }
//     copy(m_phrases.begin(),m_phrases.end(),ostream_iterator<string>(cerr,"\n"));cerr<<endl;
    l_fileOut.close();
}


ptable::ptable(string fileName, vector< size_t > ngramHash, bool direction, string outputFile, vector< vecString > inputFile)
{
    stringstream l_fileOutContent;
    vector<vecString> translations;
    if (fileName.find(".gz")<0)
    {
	string line;
	ifstream l_fileIn(fileName.c_str());
	vector<string> tmp;
	while (getline(l_fileIn, line))
	{
	    phrase p(line);
    // 	cerr << line<<endl; 
	    vector<string> tmp_trans;
	    if (direction==0) //false = source ; true = target
	    {
    // 	    if (p.getSrcHash()
		if (binary_search(ngramHash.begin(), ngramHash.end(), p.getSrcHash()))
		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
		    tmp_trans.push_back(p.getSrcContent());
		    tmp_trans.push_back(p.getTgtContent());
		    translations.push_back(tmp_trans);
		}
	    }
	    else
	    {
		if (binary_search(ngramHash.begin(), ngramHash.end(), p.getTgtHash()))
		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
		    tmp_trans.push_back(p.getSrcContent());
		    tmp_trans.push_back(p.getTgtContent());
		    translations.push_back(tmp_trans);
		}
	    }
	}
// 	tmp.push_back();
	
    }
    else
    {
	string line;
	stringstream l_content;
	read_gzip(fileName, l_content);
// 	rea
// 	ifstream l_fileIn(fileName.c_str());
// 	vector<string> tmp;
	while (getline(l_content, line))
	{
	    phrase p(line);
    // 	cerr << line<<endl; 
	    vector<string> tmp_trans;
	    if (direction==0) //false = source ; true = target
	    {
    // 	    if (p.getSrcHash()
		if (binary_search(ngramHash.begin(), ngramHash.end(), p.getSrcHash()))
		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
		    tmp_trans.push_back(p.getSrcContent());
		    tmp_trans.push_back(p.getTgtContent());
		    translations.push_back(tmp_trans);
		}
	    }
	    else
	    {
		if (binary_search(ngramHash.begin(), ngramHash.end(), p.getTgtHash()))
		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
		    tmp_trans.push_back(p.getSrcContent());
		    tmp_trans.push_back(p.getTgtContent());
		    translations.push_back(tmp_trans);
		}
	    }
	}
// 	tmp.push_back();
	
    }

//     copy(m_phrases.begin(),m_phrases.end(),ostream_iterator<string>(cerr,"\n"));cerr<<endl;

    int incLine=0;
    int incWords=0;
    int incTrans=0;
    for (incLine=0; incLine < (int)inputFile.size(); incLine++)
    {
	for (incWords=0; incWords < (int)inputFile.at(incLine).size(); incWords++)
	{
	    bool trouve=false;
	    string l_trans="";
	    string l_weights="";
	    for (incTrans=0; incTrans < (int)translations.size(); incTrans++)
	    {
		if (translations.at(incTrans).at(0).compare(inputFile.at(incLine).at(incWords))==0)
		{
		    if ((int)l_trans.length()==0)
		    {
			l_trans=translations.at(incTrans).at(1);
		    }
		    else
		    {
			l_trans=l_trans+"|||"+translations.at(incTrans).at(1);
		    }
		    trouve=true;
// 		  translation="dwelling" prob="0.8"
// 		    inputFile.at(incLine).at(incWords)="<T"
		}
	    }
	    if (trouve)
	    {
		inputFile.at(incLine).at(incWords)="<n translation=\""+l_trans+"\">"+inputFile.at(incLine).at(incWords)+"</n>";
	    }
	    
	}
	l_fileOutContent << vectorToString(inputFile.at(incLine)," ") << endl;
    }
    ofstream l_fileOut(outputFile.c_str());
    l_fileOut << l_fileOutContent.str();
    l_fileOut.close();
//     l_fileOutContent.close();
}

ptable::ptable(string fileName, vector< size_t > ngramHashSrc, vector< size_t > ngramHashTgt, bool direction, string outputFile, vector< vecString > inputFile)
{
    stringstream l_fileOutContent;
    vector<vecString> translations;
    if (fileName.find(".gz")<0)
    {
	string line;
	ifstream l_fileIn(fileName.c_str());
	vector<string> tmp;
	while (getline(l_fileIn, line))
	{
	    phrase p(line);
    // 	cerr << line<<endl; 
	    vector<string> tmp_trans;
// 	    if (direction==0) //false = source ; true = target
// 	    {
    // 	    if (p.getSrcHash()
		if ((binary_search(ngramHashSrc.begin(), ngramHashSrc.end(), p.getSrcHash())) && (binary_search(ngramHashTgt.begin(), ngramHashTgt.end(), p.getTgtHash())))
		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
		    tmp_trans.push_back(p.getSrcContent());
		    tmp_trans.push_back(p.getTgtContent());
		    translations.push_back(tmp_trans);
		}
// 	    }
// 	    else
// 	    {
// 		if (binary_search(ngramHashTgt.begin(), ngramHashTgt.end(), p.getTgtHash()))
// 		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
// 		    tmp_trans.push_back(p.getSrcContent());
// 		    tmp_trans.push_back(p.getTgtContent());
// 		    translations.push_back(tmp_trans);
// 		}
// 	    }
	}
// 	tmp.push_back();
	
    }
    else
    {
	string line;
	stringstream l_content;
	read_gzip(fileName, l_content);
// 	rea
// 	ifstream l_fileIn(fileName.c_str());
// 	vector<string> tmp;
	while (getline(l_content, line))
	{
	    phrase p(line);
    // 	cerr << line<<endl; 
	    vector<string> tmp_trans;
// 	    if (direction==0) //false = source ; true = target
// 	    {
    // 	    if (p.getSrcHash()
		if ((binary_search(ngramHashSrc.begin(), ngramHashSrc.end(), p.getSrcHash())) && (binary_search(ngramHashTgt.begin(), ngramHashTgt.end(), p.getTgtHash())))
		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
		    tmp_trans.push_back(p.getSrcContent());
		    tmp_trans.push_back(p.getTgtContent());
		    translations.push_back(tmp_trans);
		}
// 	    }
// 	    else
// 	    {
// 		if (binary_search(ngramHashTgt.begin(), ngramHashTgt.end(), p.getTgtHash()))
// 		{
    // 		m_phrases.push_back(p);
    // 		l_fileOut << line << endl;
    // 		l_fileOut << p.toString() << endl;
// 		    tmp_trans.push_back(p.getSrcContent());
// 		    tmp_trans.push_back(p.getTgtContent());
// 		    translations.push_back(tmp_trans);
// 		}
// 	    }
	}
// 	tmp.push_back();
	
    }

//     copy(m_phrases.begin(),m_phrases.end(),ostream_iterator<string>(cerr,"\n"));cerr<<endl;

    int incLine=0;
    int incWords=0;
    int incTrans=0;
    for (incLine=0; incLine < (int)inputFile.size(); incLine++)
    {
	for (incWords=0; incWords < (int)inputFile.at(incLine).size(); incWords++)
	{
	    bool trouve=false;
	    string l_trans="";
	    string l_weights="";
	    for (incTrans=0; incTrans < (int)translations.size(); incTrans++)
	    {
		if (translations.at(incTrans).at(0).compare(inputFile.at(incLine).at(incWords))==0)
		{
		    if ((int)l_trans.length()==0)
		    {
			l_trans=translations.at(incTrans).at(1);
		    }
		    else
		    {
			l_trans=l_trans+"|||"+translations.at(incTrans).at(1);
		    }
		    trouve=true;
// 		  translation="dwelling" prob="0.8"
// 		    inputFile.at(incLine).at(incWords)="<T"
		}
	    }
	    if (trouve)
	    {
		inputFile.at(incLine).at(incWords)="<n translation=\""+l_trans+"\">"+inputFile.at(incLine).at(incWords)+"</n>";
	    }
	    
	}
	l_fileOutContent << vectorToString(inputFile.at(incLine)," ") << endl;
    }
    ofstream l_fileOut(outputFile.c_str());
    l_fileOut << l_fileOutContent.str();
    l_fileOut.close();
//     l_fileOutContent.close();
}



std::string ptable::toString()
{
    stringstream s;
    for (int i=0; i<(int)m_phrases.size(); i++)
    {
	s<< m_phrases.at(i).toString()<< endl;
    }
    return s.str();
}
