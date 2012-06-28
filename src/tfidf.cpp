#include "tfidf.h"
// The following class defines a hash function for strings


using namespace std;
using namespace Tools;
using namespace boost::xpressive;

tfidf::tfidf()
{

}

void tfidf::setDebugMode ( bool b )
{
    m_debugMode = b;
}

bool mySortingFunction ( const pair<size_t, string>& i, const pair<size_t, string>& j )
{
    if ( i.first > j.first ) return false;
    if ( j.first >= i.first ) return true;
    return true;
// 	return j.second < i.second;
}
bool mySortingFunction2 ( const pair <size_t, vector <unsigned long> >& i, const pair <size_t, vector <unsigned long> >& j )
{
    if ( i.first > j.first ) return false;
    if ( j.first >= i.first ) return true;
    return true;
// 	return j.second < i.second;
}
bool mySortingFunction3 ( const pair <size_t, int >& i, const pair <size_t, int >& j )
{
    if ( i.first > j.first ) return false;
    if ( j.first >= i.first ) return true;
    return true;
// 	return j.second < i.second;
}
//vector<pair <string, vector<float> > >
bool mySortingFunction4 ( const pair <string, vector<float> >& i, const pair <string, vector<float> >& j )
{
// 	cerr << "taille : ";
    if ( ( int ) i.second.size() > 0 && ( int ) j.second.size() > 0 )
    {
// 	    cerr << "on compare "+i.first+" "<< i.second.at(0) <<" et "+j.first+" "<< j.second.at(0) << endl;
        if ( i.second.at ( 0 ) < j.second.at ( 0 ) ) return false;
        if ( j.second.at ( 0 ) <= i.second.at ( 0 ) ) return true;
    }
    return true;
// 	return j.second < i.second;
}
void tfidf::sortHash()
{
    sort ( m_tester.begin(), m_tester.end() );
    std::sort ( m_testerContent.begin(), m_testerContent.end(), mySortingFunction );
    std::sort ( m_testerCount.begin(), m_testerCount.end(), mySortingFunction2 );
    std::sort ( m_testerNgramInfos.begin(), m_testerNgramInfos.end(), mySortingFunction3 );
}

/**
 * int hashMapSimple::chercher ( size_t searchKey )
 * @param searchKey
 * @return
 */
bool tfidf::chercher ( size_t searchKey )
{
    bool trouve = binary_search ( m_tester.begin(), m_tester.end(), searchKey );
// 	vector<size_t>::iterator low,up;
// 	low=lower_bound (m_tester.begin(), m_tester.end(), searchKey);
// 	up= upper_bound (m_tester.begin(), m_tester.end(), searchKey);
// 	cerr << "lower_bound at position " << int(low) << endl;
// 	cerr << "upper_bound at position " << int(up) << endl;
// 	cerr << "Binary Search : "<<trouve<<endl;
// 	cerr << "lower_bound at position " << int(low- m_tester.begin()) << endl;
// 	cerr << "upper_bound at position " << int(up - m_tester.begin()) << endl;

// 	if ((size_t)m_tester->size() < searchKey)
// 	{
// 	    return false;
// 	}
// 	else
// 	{
// 	    return m_tester->at(searchKey);
// 	}
    return trouve;
}
int tfidf::donnerPos ( size_t searchKey )
{
    vector<size_t>::iterator low, up;
// 	cerr << "recherche de "<< searchKey <<endl;
    low = lower_bound ( m_tester.begin(), m_tester.end(), searchKey );
    up = upper_bound ( m_tester.begin(), m_tester.end(), searchKey );
// 	cerr << "lower_bound at position " << (*low) << endl;
// 	cerr << "upper_bound at position " << (*up) << endl;
// 	cerr << "Binary Search : "<<trouve<<endl;
    int lower = int ( low - m_tester.begin() ) ;
// 	int upper=int(up - m_tester.begin()) ;
// 	cerr << "lower_bound at position " << lower << endl;
// 	cerr << "upper_bound at position " << upper<< endl;
// 	cerr << m_testerContent.at(lower).first << "|" <<m_testerContent.at(lower).second << endl;
// 	cerr << m_testerContent.at(upper).first << "|" <<m_testerContent.at(upper).second << endl;

    return lower;
}
bool tfidf::supprimerPos ( size_t searchKey )
{
    vector<size_t>::iterator low, up;
// 	cerr << "recherche de "<< searchKey <<endl;
    low = lower_bound ( m_tester.begin(), m_tester.end(), searchKey );
    up = upper_bound ( m_tester.begin(), m_tester.end(), searchKey );
// 	cerr << "lower_bound at position " << (*low) << endl;
// 	cerr << "upper_bound at position " << (*up) << endl;
// 	cerr << "Binary Search : "<<trouve<<endl;
    int lower = int ( low - m_tester.begin() ) ;
// 	int upper=int(up - m_tester.begin()) ;
// 	cerr << "lower_bound at position " << lower << endl;
// 	cerr << "upper_bound at position " << upper<< endl;
// 	cerr << m_testerContent.at(lower).first << "|" <<m_testerContent.at(lower).second << endl;
// 	cerr << m_testerContent.at(upper).first << "|" <<m_testerContent.at(upper).second << endl;
    m_tester.erase ( m_tester.begin() + lower );
    m_testerContent.erase ( m_testerContent.begin() + lower );
    m_testerCount.erase ( m_testerCount.begin() + lower );
    m_testerNgramInfos.erase ( m_testerNgramInfos.begin() + lower );
    return true;
}

void tfidf::addDatas ( vector< string > s, int ngramSize )
{
    int incFile = 0;
    m_documentSize = ( int ) s.size();
    vector<int> cpt_ngramVec ( ngramSize + 1 );
    for ( incFile = 0; incFile < ( int ) cpt_ngramVec.size() ; incFile++ )
    {
        cpt_ngramVec.at ( incFile ) = 0;
    }

    for ( incFile = 0; incFile < m_documentSize; incFile++ )
    {
        cerr << ".";
	boost::progress_timer t( std::clog );
	vector<string> sentence = stringToVector ( tokenizePunct ( s.at ( incFile ) ), " " );
// 	    cerr << "On traite "<< vectorToString(sentence)<<endl;

        for ( int l_ngram = 1; l_ngram <= ngramSize; l_ngram++ )
        {
            int cpt_ngram = 0;
            for ( int l_pos = 0; l_pos + l_ngram <= ( int ) sentence.size(); l_pos++ )
            {
                cpt_ngram++;
                string l_ngram_test = vectorToString ( subVector ( sentence, l_pos, l_pos + l_ngram ), " " );
                size_t l_ngramHash = hashValueBoost ( l_ngram_test );
                if ( l_ngramHash != 0 )
                {
                    if ( !chercher ( l_ngramHash ) )
                    {
                        pair <size_t, string> l_pair ( l_ngramHash, l_ngram_test );
                        pair <size_t, int> l_pairNgramInfos ( l_ngramHash, l_ngram );
                        vector <unsigned long> l_count ( m_documentSize + 1 );
                        l_count.at ( 0 ) = 1;
                        l_count.at ( incFile + 1 ) = 1;
                        pair <size_t, vector<unsigned long> > l_pairCount ( l_ngramHash, l_count );
                        m_tester.push_back ( l_ngramHash );
                        m_testerContent.push_back ( l_pair );
                        m_testerCount.push_back ( l_pairCount );
                        m_testerNgramInfos.push_back ( l_pairNgramInfos );
                        sortHash();
                        // 			cerr << "\""<<l_ngram_test << "\" a été ajouté" << endl;
                    }
                    else
                    {
                        // 			cerr << "on cherche  "<< l_ngram_test << endl;
                        int l_pairPos = donnerPos ( l_ngramHash );
                        m_testerCount.at ( l_pairPos ).second.at ( 0 ) = m_testerCount.at ( l_pairPos ).second.at ( 0 ) + 1;
                        m_testerCount.at ( l_pairPos ).second.at ( incFile + 1 ) = m_testerCount.at ( l_pairPos ).second.at ( incFile + 1 ) + 1;
                    }
                }
            }
// 		cerr << "on arrive ici"  <<endl;
            cpt_ngramVec.at ( l_ngram ) = cpt_ngramVec.at ( l_ngram ) + cpt_ngram;
        }

    }
    m_ngramCount = cpt_ngramVec;
}


void tfidf::addDatas ( string inputs, vector< string > data, int ngramSize )
{
    int incFile = 0;
    vector <string> vecInputs = stringToVector ( inputs, " " );
    m_documentSize = ( int ) data.size();
    vector<int> cpt_ngramVec ( ngramSize + 1 );
    cerr << "Load Input...";
    for ( int l_ngram = 1; l_ngram <= ngramSize; l_ngram++ )
    {
	cerr << ".";
        int cpt_ngram = 0;
        for ( int l_pos = 0; l_pos + l_ngram <= ( int ) vecInputs.size(); l_pos++ )
        {
            cpt_ngram++;
	    if (l_ngram==ngramSize)
	    {
		string l_ngram_test = vectorToString ( subVector ( vecInputs, l_pos, l_pos + l_ngram ), " " );
		size_t l_ngramHash = hashValueBoost ( l_ngram_test );
		if ( l_ngramHash != 0 )
		{
		    if ( !chercher ( l_ngramHash ) )
		    {
			pair <size_t, string> l_pair ( l_ngramHash, l_ngram_test );
			pair <size_t, int> l_pairNgramInfos ( l_ngramHash, l_ngram );
			vector <unsigned long> l_count ( m_documentSize + 1 );
			l_count.at ( 0 ) = 1;
			l_count.at ( incFile + 1 ) = 1;
			pair <size_t, vector<unsigned long> > l_pairCount ( l_ngramHash, l_count );
			m_tester.push_back ( l_ngramHash );
			m_testerContent.push_back ( l_pair );
			m_testerCount.push_back ( l_pairCount );
			m_testerNgramInfos.push_back ( l_pairNgramInfos );
			sortHash();
			// 			cerr << "\""<<l_ngram_test << "\" a été ajouté" << endl;
		    }
		    else
		    {
			// 			cerr << "on cherche  "<< l_ngram_test << endl;
			// 			int l_pairPos=donnerPos(l_ngramHash);
			// 			m_testerCount.at(l_pairPos).second.at(0)=m_testerCount.at(l_pairPos).second.at(0)+1;
			// 			m_testerCount.at(l_pairPos).second.at(incFile+1)=m_testerCount.at(l_pairPos).second.at(incFile+1)+1;
		    }
		}
	    }
        }
// 		cerr << "on arrive ici"  <<endl;
// 		cpt_ngramVec.at(l_ngram)=cpt_ngramVec.at(l_ngram)+cpt_ngram;
    }
    cerr << ".OK!"<<endl;
    cerr << "Load data...";
    for ( incFile = 0; incFile < ( int ) cpt_ngramVec.size() ; incFile++ )
    {
        cpt_ngramVec.at ( incFile ) = 0;
    }

    for ( incFile = 0; incFile < m_documentSize; incFile++ )
    {
        cerr << ".";
// 	cerr << "document "<< incFile <<endl;
        vector<string> sentence = stringToVector ( tokenizePunct ( data.at ( incFile ) ), " " );
	if ((int)sentence.size()>ngramSize)
	{
	    for ( int l_ngram = 1; l_ngram <= ngramSize; l_ngram++ )
	    {
		int cpt_ngram = 0;
		for ( int l_pos = 0; l_pos + l_ngram <= ( int ) sentence.size(); l_pos++ )
		{
		    cpt_ngram++;
		    if (l_ngram==ngramSize)
		    {
			string l_ngram_test = vectorToString ( subVector ( sentence, l_pos, l_pos + l_ngram ), " " );
			size_t l_ngramHash = hashValueBoost ( l_ngram_test );
// 			cerr << "On traite "<< l_ngram_test <<endl;
			if ( l_ngramHash != 0 )
			{
			    if ( !chercher ( l_ngramHash ) )
			    {
				// 			pair <size_t,string> l_pair(l_ngramHash,l_ngram_test);
				// 			pair <size_t,int> l_pairNgramInfos(l_ngramHash,l_ngram);
				// 			vector <unsigned long> l_count(m_documentSize+1);
				// 			l_count.at(0)=1;
				// 			l_count.at(incFile+1)=1;
				// 			pair <size_t,vector<int> > l_pairCount(l_ngramHash,l_count);
				// 			m_tester.push_back(l_ngramHash);
				// 			m_testerContent.push_back(l_pair);
				// 			m_testerCount.push_back(l_pairCount);
				// 			m_testerNgramInfos.push_back(l_pairNgramInfos);
				// 			sortHash();
				// // 			cerr << "\""<<l_ngram_test << "\" a été ajouté" << endl;
			    }
			    else
			    {
				// 			cerr << "on cherche  "<< l_ngram_test << endl;
				int l_pairPos = donnerPos ( l_ngramHash );
				m_testerCount.at ( l_pairPos ).second.at ( 0 ) = m_testerCount.at ( l_pairPos ).second.at ( 0 ) + 1;
				m_testerCount.at ( l_pairPos ).second.at ( incFile + 1 ) = m_testerCount.at ( l_pairPos ).second.at ( incFile + 1 ) + 1;
			    }
			}
		    }
		}
    // 		cerr << "on arrive ici"  <<endl;
		cpt_ngramVec.at ( l_ngram ) = cpt_ngramVec.at ( l_ngram ) + cpt_ngram;
	    }
	}
    }
    cerr << ".OK!"<<endl;
    m_ngramCount = cpt_ngramVec;
}


void tfidf::addDatas ( vector< string > s )
{
    addDatas ( s, 1 );
}
string tfidf::printDatas()
{
    stringstream s;
    for ( int l_pos = 0; l_pos < ( int ) m_tester.size(); l_pos++ )
    {
        s << l_pos << "\t" << m_tester.at ( l_pos ) << "\t" << m_testerContent.at ( l_pos ).second ;
        vector <unsigned long>l_int = m_testerCount.at ( l_pos ).second;
//	    for (int i=0; i< (int)l_int.size(); i++)
//	    {
//		s  << "\t" << l_int.at(i);
//	    }
        if ( ( int ) vecTfidf.size() != 0 )
        {
            char* charTfidf = new char[30];
            char* charTf = new char[30];
            char* charIdf = new char[30];
            int intTfidf = round ( vecTfidf.at ( l_pos ) * 10000000 );
            int intTf = round ( vecTf.at ( l_pos ) * 10000000 );
            int intIdf = round ( vecIdf.at ( l_pos ) * 10000000 );
            float floatTfidf = intTfidf / 10000000.0;
            float floatTf = intTf / 10000000.0;
            float floatIdf = intIdf / 10000000.0;
            sprintf ( charTfidf, "%.10f", floatTfidf );
            sprintf ( charTf, "%.10f", floatTf );
            sprintf ( charIdf, "%.10f", floatIdf );

            s  << "\t" << charTfidf << "\t" << charTf << "\t" << charIdf;
        }
        s  << endl;
    }
    return s.str();
}
string tfidf::printDatasSorted()
{
    stringstream s;
    m_vec_infos_pairs.clear();
// 	vector<pair <string, vector<float> > > vec_infos_pairs;
    for ( int l_pos = 0; l_pos < ( int ) m_tester.size(); l_pos++ )
    {
        s << l_pos << "\t" << m_tester.at ( l_pos ) << "\t" << m_testerContent.at ( l_pos ).second ;
        vector <unsigned long>l_int = m_testerCount.at ( l_pos ).second;
//	    for (int i=0; i< (int)l_int.size(); i++)
//	    {
//		s  << "\t" << l_int.at(i);
//	    }
        if ( ( int ) vecTfidf.size() != 0 )
        {
// 		char* charTfidf = new char[30];
// 		char* charTf = new char[30];
// 		char* charIdf = new char[30];
            int intTfidf = round ( vecTfidf.at ( l_pos ) * 10000000 );
            int intTf = round ( vecTf.at ( l_pos ) * 10000000 );
            int intIdf = round ( vecIdf.at ( l_pos ) * 10000000 );
            float floatTfidf = intTfidf / 10000000.0;
            float floatTf = intTf / 10000000.0;
            float floatIdf = intIdf / 10000000.0;
            vector <float> l_scores ( 3, 0.0 );
            l_scores.at ( 0 ) = floatTfidf;
            l_scores.at ( 1 ) = floatTf;
            l_scores.at ( 2 ) = floatIdf;
            string l_infos = s.str();
            pair <string, vector<float> > l_infos_pair ( l_infos, l_scores );
            m_vec_infos_pairs.push_back ( l_infos_pair );
// 		l_infos
// 		sprintf(charTfidf, "%.10f", floatTfidf );
// 		sprintf(charTf, "%.10f", floatTf );
// 		sprintf(charIdf, "%.10f", floatIdf );

// 		s  << "\t" << charTfidf << "\t" << charTf << "\t" << charIdf;
        }
        s.str ( "" );
// 	    s  << endl;
    }
    s.str ( "" );
    vector <float> l_scores ( 3, -1.0 );
    pair <string, vector<float> > l_infos_pair ( "RIEN", l_scores );
    m_vec_infos_pairs.push_back ( l_infos_pair );
    if ( ( int ) m_vec_infos_pairs.size() == 0 )
    {
        cerr << "ERROR tfidf::printDatasSorted : m_vec_infos_pairs size is null" << endl;
        exit ( 1 );
    }
// 	cerr << "tri de "<< (int)m_vec_infos_pairs.size() <<endl;
    std::sort ( m_vec_infos_pairs.begin(), m_vec_infos_pairs.end(), mySortingFunction4 );
    s << "Position\tIds\thash\tmot\tTF.IDF\tTF\tIDF" << endl;
    for ( int l_pos = 0; l_pos < ( int ) m_vec_infos_pairs.size(); l_pos++ )
    {
        if ( m_vec_infos_pairs.at ( l_pos ).first.compare ( "RIEN" ) )
        {
            s << l_pos << "\t" << m_vec_infos_pairs.at ( l_pos ).first << "\t" << m_vec_infos_pairs.at ( l_pos ).second.at ( 0 ) << "\t" << m_vec_infos_pairs.at ( l_pos ).second.at ( 1 ) << "\t" << m_vec_infos_pairs.at ( l_pos ).second.at ( 2 ) << endl ;
        }
    }
    return s.str();
}

string tfidf::printDatasSorted(int nbest)
{
    stringstream s;
    m_vec_infos_pairs.clear();
    if (m_debugMode)
    {
	cerr << "DEBUGMODE tfidf::printDatasSorted"<<endl<<"m_tester.size() : "<<(int)m_tester.size()<<endl<<"vecTfidf.size() : "<<( int ) vecTfidf.size()<<endl<<"END_DEBUGMODE"<<endl;
      
    }
// 	vector<pair <string, vector<float> > > vec_infos_pairs;
    
    for ( int l_pos = 0; l_pos < ( int ) m_tester.size(); l_pos++ )
    {
        s << l_pos << "\t" << m_tester.at ( l_pos ) << "\t" << m_testerContent.at ( l_pos ).second ;
	if (m_debugMode)
	{
	    cerr << "DEBUGMODE tfidf::printDatasSorted"<<endl<< l_pos << "\t" << m_tester.at ( l_pos ) << "\t" << m_testerContent.at ( l_pos ).second <<endl<<"END_DEBUGMODE"<<endl;
	    
	}
        vector <unsigned long>l_int = m_testerCount.at ( l_pos ).second;
//	    for (int i=0; i< (int)l_int.size(); i++)
//	    {
//		s  << "\t" << l_int.at(i);
//	    }
// 		char* charTfidf = new char[30];
// 		char* charTf = new char[30];
// 		char* charIdf = new char[30];
	    if ( l_pos >= ( int ) vecTfidf.size() )
	    {
		cerr << "ERROR tfidf::printDatasSorted : vecTfidf size "<<  ( int ) vecTfidf.size() << " indice asked: " << l_pos << endl;
		exit ( 1 );	    
	    }
	    if ( l_pos >= ( int ) vecTf.size() )
	    {
		cerr << "ERROR tfidf::printDatasSorted : vecTf size "<<  ( int ) vecTf.size() << " indice asked: " << l_pos << endl;
		exit ( 1 );	    
	    }
	    if ( l_pos >= ( int ) vecIdf.size() )
	    {
		cerr << "ERROR tfidf::printDatasSorted : vecIdf size "<<  ( int ) vecIdf.size() << " indice asked: " << l_pos << endl;
		exit ( 1 );	    
	    }
            int intTfidf = round ( vecTfidf.at ( l_pos ) * 10000000 );
            int intTf = round ( vecTf.at ( l_pos ) * 10000000 );
            int intIdf = round ( vecIdf.at ( l_pos ) * 10000000 );
            float floatTfidf = intTfidf / 10000000.0;
            float floatTf = intTf / 10000000.0;
            float floatIdf = intIdf / 10000000.0;
            vector <float> l_scores ( 3, 0.0 );
            l_scores.at ( 0 ) = floatTfidf;
            l_scores.at ( 1 ) = floatTf;
            l_scores.at ( 2 ) = floatIdf;
            string l_infos = s.str();
            pair <string, vector<float> > l_infos_pair ( l_infos, l_scores );
            m_vec_infos_pairs.push_back ( l_infos_pair );
	    if (m_debugMode)
	    {
		cerr << "DEBUGMODE tfidf::printDatasSorted"<<endl;
		cerr<< l_pos << "\t" ;
		cerr<< l_infos_pair.first << "\t" <<  l_infos_pair.second.at(0) << " " <<  l_infos_pair.second.at(1)  << " " <<  l_infos_pair.second.at(2) <<endl<<"END_DEBUGMODE"<<endl;
	    }	    
// 		l_infos
// 		sprintf(charTfidf, "%.10f", floatTfidf );
// 		sprintf(charTf, "%.10f", floatTf );
// 		sprintf(charIdf, "%.10f", floatIdf );

// 		s  << "\t" << charTfidf << "\t" << charTf << "\t" << charIdf;
//         }
        s.str ( "" );
// 	    s  << endl;
    }
    
    s.str ( "" );
    vector <float> l_scores ( 3, -1.0 );
    pair <string, vector<float> > l_infos_pair ( "RIEN", l_scores );
    m_vec_infos_pairs.push_back ( l_infos_pair );
    if ( ( int ) m_vec_infos_pairs.size() == 0 )
    {
        cerr << "ERROR tfidf::printDatasSorted : m_vec_infos_pairs size is null" << endl;
        exit ( 1 );
    }
	cerr << "tri de "<< (int)m_vec_infos_pairs.size() <<endl;
    std::sort ( m_vec_infos_pairs.begin(), m_vec_infos_pairs.end(), mySortingFunction4 );
    s << "Position\tIds\thash\tmot\tTF.IDF\tTF\tIDF" << endl;
    for ( int l_pos = 0; l_pos < ( int ) m_vec_infos_pairs.size() && l_pos < nbest; l_pos++ )
    {
        if ( m_vec_infos_pairs.at ( l_pos ).first.compare ( "RIEN" ) )
        {
            s << l_pos << "\t" << m_vec_infos_pairs.at ( l_pos ).first << "\t" << m_vec_infos_pairs.at ( l_pos ).second.at ( 0 ) << "\t" << m_vec_infos_pairs.at ( l_pos ).second.at ( 1 ) << "\t" << m_vec_infos_pairs.at ( l_pos ).second.at ( 2 ) << endl ;
        }
    }
    return s.str();
}




void tfidf::compileData()
{
    for ( int i = 0; i < ( int ) m_testerCount.size(); i++ )
    {
// 	cerr << m_testerCount.at ( i ).second.at ( 0 ) <<"|";
// 	cerr << m_ngramCount.at ( m_testerNgramInfos.at ( i ).second) <<endl;
        float calc_tf = 1.0 * m_testerCount.at ( i ).second.at ( 0 ) / m_ngramCount.at ( m_testerNgramInfos.at ( i ).second );
        vecTf.push_back ( calc_tf );
        int presDoc = 0;
        vector <unsigned long> infos = m_testerCount.at ( i ).second;
        for ( int j = 0; j < ( int ) infos.size(); j++ )
        {
            if ( infos.at ( j ) != 0 )
            {
                presDoc++;
            }
        }
        float calc_idf = log ( 1.0 * m_documentSize / presDoc );
	if (m_debugMode)
	{
	    cerr << "DEBUGMODE tfidf::printDatasSorted"<<endl;
	    cerr<< i << "\t" ;
	    cerr<<"calc_tf: "<< calc_tf <<" = " << m_testerCount.at ( i ).second.at ( 0 ) << " / " <<  m_ngramCount.at ( m_testerNgramInfos.at ( i ).second) <<endl;
	    cerr << "calc_idf: " << calc_idf << " = log( " << m_documentSize << " / " << presDoc << endl;
	    cerr<<"END_DEBUGMODE"<<endl;
	}	
        vecIdf.push_back ( calc_idf );
        vecTfidf.push_back ( calc_tf*calc_idf );
    }

}

void tfidf::compileData ( vector< string > vs )
{
    vector <size_t> to_erase;
    for ( int k = 0; k < ( int ) vs.size(); k++ )
    {
        boost::regex regEx ( "(" + vs.at ( k ) + ")" );
        for ( int i = 0; i < ( int ) m_testerContent.size(); i++ )
        {
            boost::match_results<string::const_iterator> vectorFilter;
            if ( !boost::regex_match ( m_testerContent.at ( i ).second, vectorFilter, regEx, boost::match_partial ) )
            {
                to_erase.push_back ( m_testerContent.at ( i ).first );
            }
        }
    }
    for ( int k = 0; k < ( int ) to_erase.size(); k++ )
    {
// 	    cerr << to_erase.at(k) << endl;
        supprimerPos ( to_erase.at ( k ) );
    }
    compileData();
// 	exit(0);
}

float tfidf::getScore ( string s )
{
    size_t l_hash = hashValueBoost ( s );
    if ( chercher ( l_hash ) )
    {
        int l_pairPos = donnerPos ( l_hash );
        int intTfidf = round ( vecTfidf.at ( l_pairPos ) * 10000000 );
        int intTf = round ( vecTf.at ( l_pairPos ) * 10000000 );
        int intIdf = round ( vecIdf.at ( l_pairPos ) * 10000000 );
        float floatTfidf = intTfidf / 10000000.0;
        float floatTf = intTf / 10000000.0;
        float floatIdf = intIdf / 10000000.0;
        vector <float> l_scores ( 3, 0.0 );
        l_scores.at ( 0 ) = floatTfidf;
        l_scores.at ( 1 ) = floatTf;
        l_scores.at ( 2 ) = floatIdf;
        return floatTfidf;
    }
    return 0.0;
}

vector< string > tfidf::getContent ( int n )
{
    vector<string> to_return;
    for ( int i = 0; i < n && i < ( int ) m_vec_infos_pairs.size(); i++ )
    {
        vector<string> l_vecString = stringToVector ( m_vec_infos_pairs.at ( i ).first, "\t" );
        to_return.push_back ( l_vecString.at ( ( int ) l_vecString.size() - 1 ) );
    }
    return to_return;
}

vector< size_t > tfidf::getContentIds ( int n )
{
    vector<size_t> to_return;
    for ( int i = 0; i < n && i < ( int ) m_vec_infos_pairs.size(); i++ )
    {
        vector<string> l_vecString = stringToVector ( m_vec_infos_pairs.at ( i ).first, "\t" );
        to_return.push_back (hashValueBoost( l_vecString.at ( ( int ) l_vecString.size() - 1 ) ));
    }
    return to_return;
}



void tfidf::compileDataOkapibm25()
{
  
    multimap< unsigned long, unsigned long > :: iterator & element=m_mapDocSize.begin();
    float avgDocSize=0;
    long nbrdoc=0;
    for (element=m_mapDocSize.begin();element!=m_mapDocSize.end(); element++)
    {
      avgDocSize=+(*element).second;
      nbrdoc++;
    }
    avgDocSize=avgDocSize/(1.0*nbrdoc);
    for ( int i = 0; i < ( int ) m_testerCount.size(); i++ )
    {
// 	cerr << m_testerCount.at ( i ).second.at ( 0 ) <<"|";
// 	cerr << m_ngramCount.at ( m_testerNgramInfos.at ( i ).second) <<endl;
// 	unsigned long tf;
// 	multimap< size_t, multimap< unsigned long, unsigned long >  >:: iterator it = m_mapIndex.find(m_testerCount.at ( i ).first);
// 	multimap< unsigned long, unsigned long > & element=(*it).second;
// 	multimap< unsigned long, unsigned long > ::iterator it2;
// 	for (it2= element.begin();it2!=element.end(); it2++)
// 	{
	    
// 	    to_return << "\t\t"<< (*it2).first <<" | "<<(*it2).second<<endl;
// 	}
// 	(*m_mapIndex.find(m_testerCount.at ( i ).first))
	float k=2;
	float b=0.75;
	float calc_tf = 1.0 * m_testerCount.at ( i ).second.at ( 0 ) / m_documentQuerySize ;
        float calc_tf_bm25 = (1.0 * calc_tf * (k+1)) / (calc_tf + k * ( 1-b + b*( m_documentQuerySize / avgDocSize )));
        vecTf.push_back ( calc_tf );
        int presDoc = 0;
        vector <unsigned long> infos = m_testerCount.at ( i ).second;
        for ( int j = 0; j < ( int ) infos.size(); j++ )
        {
            if ( infos.at ( j ) != 0 )
            {
                presDoc++;
            }
        }
        float calc_idf_bm25 = log ( (1.0 * m_documentSize - presDoc + 0.5) / (presDoc + 0.5) );
	if (m_debugMode)
	{
	    cerr << "DEBUGMODE tfidf::printDatasSorted"<<endl;
	    cerr<< i << "\t" ;
	    cerr<<"calc_tf: "<< calc_tf <<" = " << m_testerCount.at ( i ).second.at ( 0 ) << " / " <<  m_ngramCount.at ( m_testerNgramInfos.at ( i ).second) <<endl;
	    cerr<<"calc_tf_bm25: "<< calc_tf_bm25 <<" = " << m_testerCount.at ( i ).second.at ( 0 ) << " / " <<  m_ngramCount.at ( m_testerNgramInfos.at ( i ).second) <<endl;
	    cerr << "calc_idf_bm25: " << calc_idf_bm25 << " = log( " << m_documentSize << " / " << presDoc << endl;
	    cerr<<"END_DEBUGMODE"<<endl;
	}	
        vecIdf.push_back ( calc_idf_bm25 );
        vecTfidf.push_back ( calc_tf_bm25*calc_idf_bm25 );
    }

}



void tfidf::addDatas(myIndex & query, myIndex & l_index, int ngramSize, unsigned long docNbr)
{
  
    int incFile = 0;
    m_documentSize = docNbr;
    int l_ngram = ngramSize;
    vector<int> cpt_ngramVec ( ngramSize + 1 );
    cerr << "Load Input...";
    m_mapDocSize=l_index.getMapDocSize();
    multimap< size_t, multimap< unsigned long, unsigned long >  > & query_map = query.getMyIndex();
    multimap< size_t, multimap< unsigned long, unsigned long >  >::iterator l_it;
    m_documentQuerySize=0;
    for (l_it=query_map.begin();l_it!=query_map.end();l_it++)
    {
	pair <size_t, int> l_pairNgramInfos ( (*l_it).first, ngramSize );
	vector <unsigned long> l_count ( m_documentSize + 1 ,0);
	l_count.at ( 0 ) = 1;
	multimap< unsigned long, unsigned long > ::iterator l_l_it;
	multimap< unsigned long, unsigned long > & element=(*l_it).second;
// 	multimap< unsigned long, unsigned long > ::iterator it2;
// 	int cpt_tmp=0;
	l_count.at(0)=(*(element.begin())).second;
	m_documentQuerySize=m_documentQuerySize+l_count.at(0);
	pair <size_t, vector<unsigned long> > l_pairCount ( (*l_it).first, l_count );
	m_tester.push_back ( (*l_it).first );
	m_testerCount.push_back ( l_pairCount );
	m_testerNgramInfos.push_back ( l_pairNgramInfos );
    }
    multimap< size_t, string >::iterator l_it2;
    for (l_it2=query.getMyIndexMapInfo().begin();l_it2!=query.getMyIndexMapInfo().end();l_it2++)
    {
	pair <size_t, string> l_pair ( (*l_it2).first, (*l_it2).second );
	m_testerContent.push_back ( l_pair );
    }
    sortHash();
    cerr << "Load data...";
    multimap<unsigned long , multimap <size_t , unsigned long  > >  & data_reverse_map = l_index.getMyReverseIndex();
    multimap<unsigned long , multimap <size_t , unsigned long  > > ::iterator l_rit;
    for (l_rit=data_reverse_map.begin();l_rit!=data_reverse_map.end();l_rit++)
    {
	incFile=(*l_rit).first;
	int cpt_ngram=(int)(*l_rit).second.size();
	vector < pair< size_t, string > > ::iterator it;
	for (it= m_testerContent.begin();it!=m_testerContent.end(); it++)
	{
	    unsigned long value = l_index.getReversInfos(incFile,(*it).first);
	    int l_pairPos = donnerPos ( (*it).first);
// 	    m_testerCount.at ( l_pairPos ).second.at ( 0 ) = m_testerCount.at ( l_pairPos ).second.at ( 0 ) + value;
// 	    m_testerCount.at ( l_pairPos ).second.at ( incFile + 1 ) = m_testerCount.at ( l_pairPos ).second.at ( incFile + 1 ) + value;
	}
	cpt_ngramVec.at ( l_ngram ) = cpt_ngramVec.at ( l_ngram ) + cpt_ngram;
    }    
    cerr << ".OK!"<<endl;
    m_ngramCount = cpt_ngramVec;
}


// size_t tfidf::hashValue ( string key )
// {
// 
//     boost::hash<string> hasher;
//     return hasher ( key );
// 
// }
