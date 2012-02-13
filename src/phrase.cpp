#include "phrase.h"

// The following class defines a hash function for strings


using namespace std;
using namespace Tools;
using namespace boost::xpressive;


phrase::phrase(string s)
{
//     cerr << "line to add: "<< s<<endl;
//     vector <string> l_tmpVecString=stringToVector(s,"|");
    vector <string> l_tmpVecString=stringToVector(s," ");
    int incPush=0;
    int separatorCount=0;
    m_srcContent="";
    m_tgtContent="";
    for (incPush=0; incPush < (int)l_tmpVecString.size(); incPush++)
    {
	if (l_tmpVecString.at(incPush).compare("|||")==0)
	{
	    separatorCount++;
	}
	else
	{
	    switch (separatorCount)
	    {
	      case 0:
		if (m_srcContent.length()==0)
		{
		    m_srcContent=l_tmpVecString.at(incPush);
		}
		else
		{
		    m_srcContent=m_srcContent+" "+l_tmpVecString.at(incPush);
		}
		break;
		
	      case 1:
		if (m_tgtContent.length()==0)
		{
		    m_tgtContent=l_tmpVecString.at(incPush);
		}
		else
		{
		    m_tgtContent=m_tgtContent+" "+l_tmpVecString.at(incPush);
		}
		break;

	      case 2:
		m_probs.push_back(atof(l_tmpVecString.at(incPush).c_str()));
		break;
	      default:
		m_rest.push_back(l_tmpVecString.at(incPush));
		break;
	    }
	}
	m_rest.push_back(l_tmpVecString.at(incPush));
// 	incPush=incPush+3;
    }
    
//     m_srcContent=l_tmpVecString.at(0);
//     m_tgtContent=l_tmpVecString.at(3);
//     m_rest.assign(l_tmpVecString.begin()+6,l_tmpVecString.end());
    
//     string l_probs=m_rest.at(0);
//     m_srcContent=l_tmpVecString.at(0);
//     m_tgtContent=l_tmpVecString.at(3);
//     string l_probs=l_tmpVecString.at(6);
//     cerr << l_probs<< endl;
//     int incPush=9;
//     for (incPush=9; incPush < (int)l_tmpVecString.size(); incPush=incPush+3)
//     {
// 	m_rest.push_back(l_tmpVecString.at(incPush));
// 	incPush=incPush+3;
//     }
//     while (incPush < (int)l_tmpVecString.size())
    
//     sregex rex;
//     string replace;
//     rex = sregex::compile ( "^[ ]+" );
//     replace = "";
//     m_srcContent = regex_replace ( m_srcContent, rex, replace );
//     m_tgtContent = regex_replace ( m_tgtContent, rex, replace );
//     l_probs = regex_replace ( l_probs, rex, replace );
//     rex = sregex::compile ( "[ ]+$" );
//     m_srcContent = regex_replace ( m_srcContent, rex, replace );
//     m_tgtContent = regex_replace ( m_tgtContent, rex, replace );
    m_srcHash=hashValueBoost(m_srcContent);
    m_tgtHash=hashValueBoost(m_tgtContent);
//     l_probs = regex_replace ( l_probs, rex, replace );
//     cerr << "m_srcContent :"<< m_srcContent <<"|"<<endl;
//     cerr << "m_tgtContent :"<< m_tgtContent <<"|"<<endl; 
    
//     for (int i=0; i< l_tmpVecString.size(); i++)
//     {
// 	cerr << i <<"|"<< l_tmpVecString.at(i)<<endl;
//     }
//     copy(l_tmpVecString.begin()+6,l_tmpVecString.end(),ostream_iterator<string>(cerr," "));cerr<<endl;
    
//     vector<string> l_vecProbs=stringToVector(l_probs," ");
//     for (int i=0; ((i < (int)l_vecProbs.size()) && (i < 5)); i++)
//     {
// 	cerr << atof(l_vecProbs.at(i).c_str())<<endl;
// 	m_probs.push_back(atof(l_vecProbs.at(i).c_str()));
//     }
//     m_probs.push_back(atof(m_rest.at(1)));
//     m_probs.push_back(atof(m_rest.at(2)));
//     m_probs.push_back(atof(m_rest.at(3)));
//     m_probs.push_back(atof(m_rest.at(4)));
//     copy(l_vecProbs.begin(),l_vecProbs.end(),ostream_iterator<string>(cerr," "));cerr<<endl;
//     copy(m_probs.begin(),m_probs.end(),ostream_iterator<double>(cerr," "));cerr<<endl;

}
std::vector< double, std::allocator< double > > phrase::getProbs()
{
    return m_probs;
}
vector< string > phrase::getRest()
{
    return m_rest;
}
string phrase::getSrcContent()
{
    return m_srcContent;
}
string phrase::getTgtContent()
{
    return m_tgtContent;
}
size_t phrase::getSrcHash()
{
    return m_srcHash;
}
size_t phrase::getTgtHash()
{
    return m_tgtHash;
}
string phrase::toString()
{
    stringstream to_return;
    
    to_return << m_srcContent << " ||| " << m_tgtContent << " ||| ";
    int i;
    for (i=0; ((i < (int)m_probs.size()) && (i < 5)); i++)
    {
// 	cerr << atof(l_vecProbs.at(i).c_str())<<endl;
	to_return << m_probs.at(i)<< " ";
    }   
    for (i=0; (i < (int)m_rest.size()) ; i++)
    {
// 	cerr << atof(l_vecProbs.at(i).c_str())<<endl;
	to_return << " ||| "<< m_rest.at(i);
    }   
//     to_return << " ||| ";
//     to_return << m_rest.at(1) << " ||| ";
//     to_return << m_rest.at(2);
    return to_return.str();
}



