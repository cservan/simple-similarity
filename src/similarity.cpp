#include "similarity.h"

// The following class defines a hash function for strings


using namespace std;
using namespace Tools;


void similarity::addTfIdfData(vector< string > vs)
{
    m_similarityContent=vs;
}
void similarity::addDocNames(vector< string > vs)
{
    m_docNames=vs;
    m_documentSize=(int)m_docNames.size();
}
// void similarity::addDocNames(string vs)
// {
//     m_docNames.clear();
//     m_docNames.push_back(vs);
//     m_documentSize=(int)m_docNames.size();
// }

void similarity::calculateSimilarity(std::vector< string > vs, string s)
{
    if ((int)m_similarityContent.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "m_similiratyContent is empty" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if ((int)m_docNames.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "m_docNames is empty" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if ((int)vs.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "empty datas" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
//     copy(m_similarityContent.begin(), m_similarityContent.end(), ostream_iterator<string>(cerr,"\n"));cerr <<endl;
    if (m_documentSize>1)
    {
	int l_vsInc=0;
	int l_docInc=0;
	int l_contentInc=0;
	int l_docSimVecInc=0;
	float l_sumSquareContent=0.0;
	vector <string> l_doc;
	vector <int> l_contentSimVector((int)m_similarityContent.size(),0);
	// initialisation du vecteur de similarité coté source (on y mets les comptes de l'input)
	for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	{
	    l_doc=stringToVector(s," ");
	    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
	    {
		if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
		{
		    l_contentSimVector.at(l_contentInc)=l_contentSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
		}
	    }
	}    
	
	for (l_docSimVecInc=0; l_docSimVecInc< (int)l_contentSimVector.size(); l_docSimVecInc++)
	{
	    l_sumSquareContent=l_sumSquareContent+1.0*(l_contentSimVector.at(l_docSimVecInc)*l_contentSimVector.at(l_docSimVecInc));
	}
	l_sumSquareContent=sqrt(l_sumSquareContent);
	
	for (l_vsInc=0; l_vsInc< (int)vs.size(); l_vsInc++)
	{
	    l_doc=stringToVector(vs.at(l_vsInc)," ");
	    vector <unsigned long> l_docSimVector((int)m_similarityContent.size(),0);
	    for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	    {
    // 	    if (l_docSimVector.at(l_contentInc)==0)
    // 	    {
		    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
		    {
			if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
			{
			    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
			}
		    }
    // 	    }
	    }
	    m_similarityVector.push_back(l_docSimVector);
	    float l_prodVect=0.0;
	    float l_sumSquare=0.0;
	    int l_countContent=0;
	    int l_countDoc=0;
	    for (l_docSimVecInc=0; l_docSimVecInc< (int)l_docSimVector.size(); l_docSimVecInc++)
	    {
		l_prodVect=l_prodVect+1.0*l_contentSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_sumSquare=l_sumSquare+1.0*l_docSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_countContent=l_countContent+l_contentSimVector.at(l_docSimVecInc);
		l_countDoc=l_countDoc+l_docSimVector.at(l_docSimVecInc);
	    }
	    l_sumSquare=sqrt(l_sumSquare);
	    float l_result=0.0;
    // 	l_result=(1.0*l_sum);
	    if (l_sumSquareContent*l_sumSquare != 0)
	    {
		    l_result=(1.0*l_prodVect)/(1.0*l_sumSquareContent*l_sumSquare);
	    }
	    else
	    {
		    l_result=0.0;
	    }
	    if (m_lengthRatio)
	    {
		if (l_countDoc==0)
		{
// 		    cerr << "error no similarity calculation possible"<<endl;
		}
		else
		{
// 		    cerr << l_result << "|"<< l_countContent<<"|"<<l_countDoc<<"|"<< ((float)l_countContent/(float)l_countDoc) << "|";
		    if (l_countDoc > l_countContent)
		    {
			l_result=l_result*((float)l_countContent/(float)l_countDoc);
		    }
		}
	    }
	    m_similarityResult.push_back(l_result*100.0);
	    cerr << ".";
	    
	}
    //     int l_simVecInc=0;
    //     for (l_simVecInc=0; l_simVecInc< (int)m_similarityVector.size(); l_simVecInc++)
    //     {
    // 	vector <int> l_docSimVector=m_similarityVector.at(l_simVecInc);
    // 	
    //     }
    }
    else
    {
	int l_vsInc=0;
	int l_docInc=0;
	int l_contentInc=0;
	int l_docSimVecInc=0;
	float l_sumSquareContent=0.0;
	vector <string> l_doc;
	vector <int> l_contentSimVector((int)m_similarityContent.size(),0);
	for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	{
	    l_doc=stringToVector(s," ");
	    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
	    {
		if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
		{
		    l_contentSimVector.at(l_contentInc)=l_contentSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
		}
	    }
	}    
	for (l_docSimVecInc=0; l_docSimVecInc< (int)l_contentSimVector.size(); l_docSimVecInc++)
	{
	    l_sumSquareContent=l_sumSquareContent+1.0*(l_contentSimVector.at(l_docSimVecInc)*l_contentSimVector.at(l_docSimVecInc));
	}
	l_sumSquareContent=sqrt(l_sumSquareContent);
	
	for (l_vsInc=0; l_vsInc< (int)vs.size(); l_vsInc++)
	{
	    l_doc=stringToVector(vs.at(l_vsInc)," ");
	    vector <unsigned long> l_docSimVector((int)m_similarityContent.size(),0);
	    for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	    {
    // 	    if (l_docSimVector.at(l_contentInc)==0)
    // 	    {
		    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
		    {
			if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
			{
			    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
			}
		    }
    // 	    }
	    }
	    m_similarityVector.push_back(l_docSimVector);
	    float l_prodVect=0.0;
	    float l_sumSquare=0.0;
	    int l_countContent=0;
	    int l_countDoc=0;
	    for (l_docSimVecInc=0; l_docSimVecInc< (int)l_docSimVector.size(); l_docSimVecInc++)
	    {
		l_prodVect=l_prodVect+1.0*l_contentSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_sumSquare=l_sumSquare+1.0*l_docSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_countContent=l_countContent+l_contentSimVector.at(l_docSimVecInc);
		l_countDoc=l_countDoc+l_docSimVector.at(l_docSimVecInc);
	    }
	    l_sumSquare=sqrt(l_sumSquare);
	    float l_result=0.0;
    // 	l_result=(1.0*l_sum);
	    if (l_sumSquareContent*l_sumSquare != 0)
	    {
		    l_result=(1.0*l_prodVect)/(1.0*l_sumSquareContent*l_sumSquare);
	    }
	    else
	    {
		    l_result=0.0;
	    }
	    if (m_lengthRatio)
	    {
		l_result=l_result*(float)(l_countContent/l_countDoc);
	    }
	    m_similarityResult.push_back(l_result*100.0);
	    cerr << ".";
	    
	}
    //     int l_simVecInc=0;
    //     for (l_simVecInc=0; l_simVecInc< (int)m_similarityVector.size(); l_simVecInc++)
    //     {
    // 	vector <int> l_docSimVector=m_similarityVector.at(l_simVecInc);
    // 	
    //     }
    }
    sortResults();
}


void similarity::calculateSimilarity(std::vector< string > vs, string s, int ngramSize)
{
    if ((int)m_similarityContent.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "m_similiratyContent is empty" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if ((int)m_docNames.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "m_docNames is empty" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if ((int)vs.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "empty datas" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if (ngramSize<1)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "wrong ngramSize" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
//     copy(m_similarityContent.begin(), m_similarityContent.end(), ostream_iterator<string>(cerr,"\n"));cerr <<endl;
    if (m_documentSize>1)
    {
	int l_vsInc=0;
	int l_docInc=0;
	int l_contentInc=0;
	int l_docSimVecInc=0;
	float l_sumSquareContent=0.0;
	vector <string> l_doc;
	vector <string> l_fullInputContent;
	vector <int> l_contentSimVector((int)m_similarityContent.size(),0);
	// initialisation du vecteur de similarité coté source (on y mets les comptes de l'input)
	cerr << "ngramSize " << ngramSize <<endl;
	for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	{
	    l_fullInputContent=stringToVector(s," ");
// 	    for ( int l_ngram = 1; l_ngram <= ngramSize; l_ngram++ )
// 	    {
// 		int cpt_ngram = 0;
		for ( int l_docInc = 0; l_docInc + ngramSize <= ( int ) l_fullInputContent.size(); l_docInc++ )
		{
// 		    cpt_ngram++;
// 		    if (l_ngram==ngramSize)
// 		    {
			string l_ngram_test = vectorToString ( subVector ( l_fullInputContent, l_docInc, l_docInc + ngramSize  ), " " );
			if (m_similarityContent.at(l_contentInc).compare(l_ngram_test)==0)
			{
			    l_contentSimVector.at(l_contentInc)=l_contentSimVector.at(l_contentInc)+1;
			}
// 		    }
		}
// 	    }
	}	
	for (l_docSimVecInc=0; l_docSimVecInc< (int)l_contentSimVector.size(); l_docSimVecInc++)
	{
	    l_sumSquareContent=l_sumSquareContent+1.0*(l_contentSimVector.at(l_docSimVecInc)*l_contentSimVector.at(l_docSimVecInc));
	}
	l_sumSquareContent=sqrt(l_sumSquareContent);
	
	for (l_vsInc=0; l_vsInc< (int)vs.size(); l_vsInc++)
	{
	    l_doc=stringToVector(vs.at(l_vsInc)," ");
	    vector <unsigned long> l_docSimVector((int)m_similarityContent.size(),0);
	    for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	    {
		for (l_docInc=0; l_docInc + ngramSize < (int)l_doc.size() ; l_docInc++)
		{
		    if (ngramSize>1)
		    {
// 			for ( int l_ngram = 2; l_ngram <= ngramSize; l_ngram++ )
// 			{
// 			    if (l_ngram==ngramSize)
// 			    {
				string l_ngram_test = vectorToString ( subVector ( l_doc, l_docInc, l_docInc + ngramSize ), " " );
				if (m_similarityContent.at(l_contentInc).compare(l_ngram_test)==0)
				{
				    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
				}
// 			    }
// 			}
		    }
		    else
		    {
			string l_ngram_test = l_doc.at(l_docInc);
			if (m_similarityContent.at(l_contentInc).compare(l_ngram_test)==0)
			{
			    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
			}
		    }
		    
		}
	    }
	    m_similarityVector.push_back(l_docSimVector);
	    float l_prodVect=0.0;
	    float l_sumSquare=0.0;
	    int l_countContent=0;
	    int l_countDoc=0;
	    for (l_docSimVecInc=0; l_docSimVecInc< (int)l_docSimVector.size(); l_docSimVecInc++)
	    {
		l_prodVect=l_prodVect+1.0*l_contentSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_sumSquare=l_sumSquare+1.0*l_docSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_countContent=l_countContent+l_contentSimVector.at(l_docSimVecInc);
		l_countDoc=l_countDoc+l_docSimVector.at(l_docSimVecInc);
	    }
	    l_sumSquare=sqrt(l_sumSquare);
	    float l_result=0.0;
    // 	l_result=(1.0*l_sum);
	    if (l_sumSquareContent*l_sumSquare != 0)
	    {
		    l_result=(1.0*l_prodVect)/(1.0*l_sumSquareContent*l_sumSquare);
	    }
	    else
	    {
		    l_result=0.0;
	    }
	    if (m_lengthRatio)
	    {
		if (l_countDoc==0)
		{
// 		    cerr << "error no similarity calculation possible"<<endl;
		}
		else
		{
// 		    cerr << l_result << "|"<< l_countContent<<"|"<<l_countDoc<<"|"<< ((float)l_countContent/(float)l_countDoc) << "|";
		    if (l_countDoc > l_countContent)
		    {
			l_result=l_result*((float)l_countContent/(float)l_countDoc);
		    }
		}
	    }
	    m_similarityResult.push_back(l_result*100.0);
	    cerr << ".";
	    
	}
    //     int l_simVecInc=0;
    //     for (l_simVecInc=0; l_simVecInc< (int)m_similarityVector.size(); l_simVecInc++)
    //     {
    // 	vector <int> l_docSimVector=m_similarityVector.at(l_simVecInc);
    // 	
    //     }
    }
    else
    {
	int l_vsInc=0;
	int l_docInc=0;
	int l_contentInc=0;
	int l_docSimVecInc=0;
	float l_sumSquareContent=0.0;
	vector <string> l_doc;
	vector <int> l_contentSimVector((int)m_similarityContent.size(),0);
	for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	{
	    l_doc=stringToVector(s," ");
	    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
	    {
		if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
		{
		    l_contentSimVector.at(l_contentInc)=l_contentSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
		}
	    }
	}    
	for (l_docSimVecInc=0; l_docSimVecInc< (int)l_contentSimVector.size(); l_docSimVecInc++)
	{
	    l_sumSquareContent=l_sumSquareContent+1.0*(l_contentSimVector.at(l_docSimVecInc)*l_contentSimVector.at(l_docSimVecInc));
	}
	l_sumSquareContent=sqrt(l_sumSquareContent);
	
	for (l_vsInc=0; l_vsInc< (int)vs.size(); l_vsInc++)
	{
	    l_doc=stringToVector(vs.at(l_vsInc)," ");
	    vector <unsigned long> l_docSimVector((int)m_similarityContent.size(),0);
	    for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	    {
    // 	    if (l_docSimVector.at(l_contentInc)==0)
    // 	    {
		    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
		    {
			if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
			{
			    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
			}
		    }
    // 	    }
	    }
	    m_similarityVector.push_back(l_docSimVector);
	    float l_prodVect=0.0;
	    float l_sumSquare=0.0;
	    int l_countContent=0;
	    int l_countDoc=0;
	    for (l_docSimVecInc=0; l_docSimVecInc< (int)l_docSimVector.size(); l_docSimVecInc++)
	    {
		l_prodVect=l_prodVect+1.0*l_contentSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_sumSquare=l_sumSquare+1.0*l_docSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_countContent=l_countContent+l_contentSimVector.at(l_docSimVecInc);
		l_countDoc=l_countDoc+l_docSimVector.at(l_docSimVecInc);
	    }
	    l_sumSquare=sqrt(l_sumSquare);
	    float l_result=0.0;
    // 	l_result=(1.0*l_sum);
	    if (l_sumSquareContent*l_sumSquare != 0)
	    {
		    l_result=(1.0*l_prodVect)/(1.0*l_sumSquareContent*l_sumSquare);
	    }
	    else
	    {
		    l_result=0.0;
	    }
	    if (m_lengthRatio)
	    {
		l_result=l_result*(float)(l_countContent/l_countDoc);
	    }
	    m_similarityResult.push_back(l_result*100.0);
	    cerr << ".";
	    
	}
    //     int l_simVecInc=0;
    //     for (l_simVecInc=0; l_simVecInc< (int)m_similarityVector.size(); l_simVecInc++)
    //     {
    // 	vector <int> l_docSimVector=m_similarityVector.at(l_simVecInc);
    // 	
    //     }
    }
    sortResults();
}

void similarity::sortResults()
{
    
//     int indiceMax=0;
    for (int j=0; j< (int)m_similarityResult.size(); j++)
    {
	for (int i=0; i< (int)m_similarityResult.size(); i++)
	{
		if (m_similarityResult.at(i)<m_similarityResult.at(j))
		{
// 		    stringstream s1;
// 		    s1<<i;
		    
		    string tmp_s = m_docNames.at(i);
		    m_docNames.at(i)= m_docNames.at(j);
		    m_docNames.at(j) = tmp_s;
		    
		    float tmp_f=m_similarityResult.at(i);
		    m_similarityResult.at(i) = m_similarityResult.at(j);
		    m_similarityResult.at(j)=tmp_f;
		    
		    
		    vector<unsigned long> tmp_i=m_similarityVector.at(i);
		    m_similarityVector.at(i)=m_similarityVector.at(j);
		    m_similarityVector.at(j)=tmp_i;
		    
		}
// 		s << m_docNames.at(i)<< "\t" << m_similarityResult.at(i) << "\t";
// 		copy(m_similarityVector.at(i).begin(), m_similarityVector.at(i).end(), ostream_iterator<int>(s," "));
// 		s <<endl;
	}
    }

}

string similarity::printResults()
{
    stringstream s;
    for (int i=0; i< (int)m_similarityResult.size(); i++)
    {
// 	for (int j=0; j< (int)m_similarityResult.at(i).size(); j++)
// 	{
	    s << m_docNames.at(i)<< "\t" << m_similarityResult.at(i) << "\t";
	    copy(m_similarityVector.at(i).begin(), m_similarityVector.at(i).end(), ostream_iterator<int>(s," "));
	    s <<endl;
// 	}
    }
    return s.str();
}

string similarity::printResults(int nbest)
{
    stringstream s;
    for (int i=0; i< (int)m_similarityResult.size() && i < nbest; i++)
    {
// 	for (int j=0; j< (int)m_similarityResult.at(i).size(); j++)
// 	{
	    s << m_docNames.at(i)<< "\t" << m_similarityResult.at(i) << "\t";
	    copy(m_similarityVector.at(i).begin(), m_similarityVector.at(i).end(), ostream_iterator<int>(s," "));
	    s <<endl;
// 	}
    }
    return s.str();
}

void similarity::setLengthRatio(bool b)
{
    m_lengthRatio=b;
  
}

void similarity::calculateSimilarity(myIndex & data_index, string s, int ngramSize)
{
    if ((int)m_similarityContent.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "m_similiratyContent is empty" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if ((int)m_docNames.size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "m_docNames is empty" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
    if ((int)data_index.getMyReverseIndex().size()==0)
    {
	cerr << "ERROR similarity::calculateSimilarity : "<< endl << "empty datas" <<endl << "END ERROR MESSAGE" <<endl;
	exit(0);
    }
//     copy(m_similarityContent.begin(), m_similarityContent.end(), ostream_iterator<string>(cerr,"\n"));cerr <<endl;
//     if (m_documentSize>1)
//     {
	int l_vsInc=0;
	int l_docInc=0;
	int l_contentInc=0;
	int l_docSimVecInc=0;
	float l_sumSquareContent=0.0;
	vector <string> l_doc;
	vector <unsigned long> l_contentSimVector((int)m_similarityContent.size(),0);
	// initialisation du vecteur de similarité coté source (on y mets les comptes de l'input)
	for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	{
	    l_doc=stringToVector(s," ");
	    for (l_docInc=0; l_docInc + ngramSize < (int)l_doc.size() ; l_docInc++)
	    {
		string l_ngram_test = vectorToString ( subVector ( l_doc, l_docInc, l_docInc + ngramSize ), " " );
		if (m_similarityContentIds.at(l_contentInc)==hashValueBoost(l_ngram_test))
		{
		    l_contentSimVector.at(l_contentInc)=l_contentSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
		}
	    }
	} 
// 	    copy(l_contentSimVector.begin(), l_contentSimVector.end(), ostream_iterator<unsigned long>(cerr,"\n"));cerr <<endl;
	for (l_docSimVecInc=0; l_docSimVecInc< (int)l_contentSimVector.size(); l_docSimVecInc++)
	{
	    l_sumSquareContent=l_sumSquareContent+1.0*(l_contentSimVector.at(l_docSimVecInc)*l_contentSimVector.at(l_docSimVecInc));
	}
	l_sumSquareContent=sqrt(l_sumSquareContent);
	
// 	vector<vecInt> tmp((int)data_index.getMyReverseIndex().size());
	m_similarityVector.resize((int)data_index.getMyReverseIndex().size());
// 	vector<float> tmp2((int)data_index.getMyReverseIndex().size());
	m_similarityResult.resize((int)data_index.getMyReverseIndex().size());
// 	
	m_data_index = & data_index;
	m_contentSimVector = l_contentSimVector;
	m_sumSquareContent = l_sumSquareContent;
	cerr << "l_sumSquareContent "<< l_sumSquareContent << endl;
// 	cerr << "on fait des trucs " << (int)data_index.getMyReverseIndex().size()<< " de fois"<<endl;
	#pragma omp parallel for 
	for (l_vsInc=0; l_vsInc< (int)data_index.getMyReverseIndex().size(); l_vsInc++)
	{
// 	    m_threads.create_thread(boost::bind(&similarity::evaluate , this, l_vsInc));
	     evaluate(l_vsInc);
// 	    boost::thread t(boost::bind(&similarity::evaluate , this, l_vsInc));
// 	    t.join();
// 	     evaluate(l_vsInc);
// 	    m_threads.create_thread(boost::bind(&evaluate, l_vsInc));
// // 	    l_doc=stringToVector(vs.at(l_vsInc)," ");
// 	    vector <unsigned long> l_docSimVector((int)m_similarityContent.size(),0);
// 	    for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
// 	    {
//     // 	    if (l_docSimVector.at(l_contentInc)==0)
//     // 	    {
// 		    l_docSimVector.at(l_contentInc)=data_index.getReversInfos(l_vsInc,m_similarityContentIds.at(l_contentInc));
// // 		    multimap <size_t , unsigned long  > & element=(*r_it).second;
// // 		    multimap <size_t , unsigned long  > :: iterator r_it_element=element.find(hashvalue);
// // 		    if (data_index.getMyReverseIndex().find(l_vsInc).second.find(m_similarityContentIds.at(l_contentInc)!=;
// // 		    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
// // 		    {
// // 			if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
// // 			{
// // 			    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
//     // 			l_docSimVector.at(l_contentInc)=1;
// // 			}
// // 		    }
//     // 	    }
// 	    }
// 	    m_similarityVector.at(l_vsInc)=l_docSimVector;
// 	    float l_prodVect=0.0;
// 	    float l_sumSquare=0.0;
// 	    int l_countContent=0;
// 	    int l_countDoc=0;
// 	    for (l_docSimVecInc=0; l_docSimVecInc< (int)l_docSimVector.size(); l_docSimVecInc++)
// 	    {
// 		l_prodVect=l_prodVect+1.0*l_contentSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
// 		l_sumSquare=l_sumSquare+1.0*l_docSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
// 		l_countContent=l_countContent+l_contentSimVector.at(l_docSimVecInc);
// 		l_countDoc=l_countDoc+l_docSimVector.at(l_docSimVecInc);
// 	    }
// 	    l_sumSquare=sqrt(l_sumSquare);
// 	    float l_result=0.0;
//     // 	l_result=(1.0*l_sum);
// 	    if (l_sumSquareContent*l_sumSquare != 0)
// 	    {
// 		    l_result=(1.0*l_prodVect)/(1.0*l_sumSquareContent*l_sumSquare);
// 	    }
// 	    else
// 	    {
// 		    l_result=0.0;
// 	    }
// 	    if (m_lengthRatio)
// 	    {
// 		if (l_countDoc==0)
// 		{
// // 		    cerr << "error no similarity calculation possible"<<endl;
// 		}
// 		else
// 		{
// // 		    cerr << l_result << "|"<< l_countContent<<"|"<<l_countDoc<<"|"<< ((float)l_countContent/(float)l_countDoc) << "|";
// 		    if (l_countDoc > l_countContent)
// 		    {
// 			l_result=l_result*((float)l_countContent/(float)l_countDoc);
// 		    }
// 		}
// 	    }
// 	    m_similarityResult.at(l_vsInc)=(l_result*100.0);
// // 	    m_similarityResult.push_back(l_result*100.0);
// 	    cerr << ".";
	    
	}
	m_threads.join_all();
    sortResults();
}

void similarity::evaluate(int & l_vsInc)
{
	
// 	vector<vecInt> tmp((int)data_index.getMyReverseIndex().size());
// 	m_similarityVector.resize((int)m_data_index->getMyReverseIndex().size());
// 	vector<float> tmp2((int)data_index.getMyReverseIndex().size());
// 	m_similarityResult.resize((int)m_data_index->getMyReverseIndex().size());
	int l_contentInc;
	int l_docSimVecInc;
	for (l_vsInc=0; l_vsInc< (int)m_data_index->getMyReverseIndex().size(); l_vsInc++)
	{
	    
// 	    l_doc=stringToVector(vs.at(l_vsInc)," ");
	    vector <unsigned long> l_docSimVector((int)m_similarityContent.size(),0);
	    for (l_contentInc=0; l_contentInc< (int)m_similarityContent.size(); l_contentInc++)
	    {
    // 	    if (l_docSimVector.at(l_contentInc)==0)
    // 	    {
		    l_docSimVector.at(l_contentInc)=m_data_index->getReversInfos(l_vsInc,m_similarityContentIds.at(l_contentInc));
// 		    multimap <size_t , unsigned long  > & element=(*r_it).second;
// 		    multimap <size_t , unsigned long  > :: iterator r_it_element=element.find(hashvalue);
// 		    if (data_index.getMyReverseIndex().find(l_vsInc).second.find(m_similarityContentIds.at(l_contentInc)!=;
// 		    for (l_docInc=0; l_docInc < (int)l_doc.size() ; l_docInc++)
// 		    {
// 			if (m_similarityContent.at(l_contentInc).compare(l_doc.at(l_docInc))==0)
// 			{
// 			    l_docSimVector.at(l_contentInc)=l_docSimVector.at(l_contentInc)+1;
    // 			l_docSimVector.at(l_contentInc)=1;
// 			}
// 		    }
    // 	    }
	    }
	    m_similarityVector.at(l_vsInc)=l_docSimVector;
	    float l_prodVect=0.0;
	    float l_sumSquare=0.0;
	    int l_countContent=0;
	    int l_countDoc=0;
	    for (l_docSimVecInc=0; l_docSimVecInc< (int)l_docSimVector.size(); l_docSimVecInc++)
	    {
		l_prodVect=l_prodVect+1.0*m_contentSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_sumSquare=l_sumSquare+1.0*l_docSimVector.at(l_docSimVecInc)*l_docSimVector.at(l_docSimVecInc);
		l_countContent=l_countContent+m_contentSimVector.at(l_docSimVecInc);
		l_countDoc=l_countDoc+l_docSimVector.at(l_docSimVecInc);
	    }
	    l_sumSquare=sqrt(l_sumSquare);
	    float l_result=0.0;
    // 	l_result=(1.0*l_sum);
	    if (m_sumSquareContent*l_sumSquare != 0)
	    {
		    l_result=(1.0*l_prodVect)/(1.0*m_sumSquareContent*l_sumSquare);
	    }
	    else
	    {
		    l_result=0.0;
	    }
// 	    cerr << l_result <<"|"<< l_prodVect <<"|"<< m_sumSquareContent <<"|"<<l_sumSquare<<"|"<< endl;
	    if (m_lengthRatio)
	    {
		if (l_countDoc==0)
		{
// 		    cerr << "error no similarity calculation possible"<<endl;
		}
		else
		{
// 		    cerr << l_result << "|"<< l_countContent<<"|"<<l_countDoc<<"|"<< ((float)l_countContent/(float)l_countDoc) << "|";
		    if (l_countDoc > l_countContent)
		    {
			l_result=l_result*((float)l_countContent/(float)l_countDoc);
		    }
		}
	    }
	    m_similarityResult.at(l_vsInc)=(l_result*100.0);
// 	    m_similarityResult.push_back(l_result*100.0);
	    cerr << ".";
	    
	}

}


void similarity::addTfIdfDataIds(vector< size_t > vs)
{
    m_similarityContentIds=vs;
}
