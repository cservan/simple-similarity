
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
// #include <boost/regex.hpp>
// #include <../src/srilm/File.h>
// #include <../src/srilm/Vocab.h>
#include <tools.h>
#include <tfidf.h>
#include <similarity.h>
#include <myIndex.h>
// #include <../src/toolsLM.h>
// #include <../src/hashMap.h>
// #include <../src/hashMapSimple.h>
// #include <../src/hashMapStringInfos.h>
#include <time.h>
// #include <ext/hash_map.h>
using namespace std;
using namespace Tools;
// using namespace ToolsLM;
// using namespace HashMapSpace;
// using namespace lm::ngram;

namespace fs = boost::filesystem;

struct parametres
{
    bool debugMode;
    bool TfIdfCalculation;
    bool SimilarityCalulation;
    bool SbyS;
    bool noSort;
//     bool outputAlignments;
//     string sourceFile;     // path to the resources
//     string targetFile;     // path to the configuration files
//     string dictionnaryTranslationFile;     // path to the configuration files
    string outputFileName;
    string inputFileName;
    string directoryDataName;
    string stopWordsList;
    vector <string> pos;
    int ngramSize;
    int nbestReturned;
    int nbestSimReturned;
    bool lengthRatio;
    bool printFullResults;
//     string unmodFile;
//     string nbestmodFile;
//     string nbestunmodFile;
//     string listeFile;
//     string alignmentFile;
};

// string traiter ( vector< vecString > s, vector< vecString > t, parametres l_paramtres );
string traiterSource ( vector< vecString > s, parametres l_paramtres);
string afficher_hypothesis ( vector<vecString> hyp_full, int pos );
void usage()
{
// 	cerr<<"tercpp [-N] [-s] [-P] -r ref -h hyp [-a alter_ref] [-b beam_width] [-S trans_span_prefix] [-o out_format -n out_pefix] [-d max_shift_distance] [-M match_cost] [-D delete_cost] [-B substitute_cost] [-I insert_cost] [-T shift_cost]"<<endl;
    cerr << "Usage : " << endl << "\tsimilarity --in inputFileName --data directoryDataName --out outPutFile [--stopWordsList file] [--lengthRatio] [--ngramSize ngramSize] [--POS nom,adv,adj,...] [--TfIdfOnly|SimilarityOnly]:\nor \n \tsimilarity --SbyS --in inputFileName --data fileDataName --out outPutFile [--stopWordsList file] [--lengthRatio] [--ngramSize ngramSize] [--nbestTfIdf nbestSize] [--nbestSimilarity nbestSize] [--POS nom,adv,adj,...] [--TfIdfOnly|SimilarityOnly] [--noSort] [--printFullResults] :\n\n\t\t --debugMode \t\t\t: print debug messages \n\t\t --SbyS \t\t\t: considers each sentence as document in the inputFile and in the Data. \n\t\t ";
    cerr << "--TfIdfOnly|SimilarityOnly \t: calculate only TF.IDF OR calculate only the similarity and take the input as full request\n\t\t --POS nom,adv,adj,... \t\t: if you want to filer the TF.IDF calculation and the similarity calculation by some POS. WARNING: do not use it if documents do not contain any POS!\n\t\t [--lengthRatio]\t\t\t: considers the numbers of words in the cosine calculation.\n\t\t  --help \t\t\t: print this help message.\n" << endl;
    exit ( 0 );
// 	System.exit(1);

}
void readCommandLineArguments ( unsigned int argc, char *argv[] , parametres & p )
{
    p.debugMode = false;
    p.SbyS = false;
    p.stopWordsList = "";
    p.TfIdfCalculation = true;
    p.SimilarityCalulation = true;
    p.lengthRatio = false;
    p.noSort = false;
    p.outputFileName = "";
    p.directoryDataName = "";
    p.inputFileName="";
    p.ngramSize=1;
    p.printFullResults = false;
    p.nbestReturned=50000;
    p.nbestSimReturned=50000;
//     p.nbestmodFile = "";
//     p.nbestunmodFile="";
//     p.listeFile="";
//     p.outputAlignments=false;
//     p.caseOn=false;
//     p.noPunct=false;
//     p.normalize=false;
//     p.tercomLike=false;
//     p.sgmlInputs=false;
//     p.noTxtIds=false;
    if (argc==1)
    {
        usage();
    }
    for ( unsigned int i = 1; i < argc; i++ )
    {
        // Command line string
        string s ( argv[i] );
        string infos ( "" );
        if ( i < argc - 1 )
        {
            infos = argv[i+1];
        }

        // Configuration files
        if ( s.find ( "--in" ) == 0 )
        {
            p.inputFileName = infos;
        }
//         else if ( s.find ( "--unmod" ) == 0 )
//         {
//             p.unmodFile = infos;
//         }
//         else if ( s.find ( "--nbestmod" ) == 0 )
//         {
//             p.nbestmodFile = infos;
//         }
//         else if ( s.find ( "--nbestunmod" ) == 0 )
//         {
//             p.nbestunmodFile = infos;
//         }
//         else if ( s.find ( "--list" ) == 0 )
//         {
//             p.listeFile = infos;
//         }
        else if ( s.find ( "--out" ) == 0 )
        {
            p.outputFileName = infos;
        }
        else if ( s.find ( "--SbyS" ) == 0 )
        {
            p.SbyS = true;
        }
        else if ( s.find ( "--lengthRatio" ) == 0 )
        {
            p.lengthRatio = true;
        }
        else if ( s.find ( "--data" ) == 0 )
        {
            p.directoryDataName = infos;
        }
        else if ( s.find ( "--stopWordsList" ) == 0 )
        {
            p.stopWordsList = infos;
        }
        else if ( s.find ( "--ngramSize" ) == 0 )
        {
            p.ngramSize = atoi(infos.c_str());
        }
        else if ( s.find ( "--nbestTfIdf" ) == 0 )
        {
            p.nbestReturned = atoi(infos.c_str());
        }
        else if ( s.find ( "--nbestSimilarity" ) == 0 )
        {
            p.nbestSimReturned = atoi(infos.c_str());
        }
        else if ( s.find ( "--POS" ) == 0 )
        {
            p.pos = stringToVector(infos,",");
        }
        else if ( s.find ( "--debugMode" ) == 0 )
        {
            p.debugMode = true;
        }
        else if ( s.find ( "--printFullResults" ) == 0 )
        {
            p.printFullResults = true;
        }
        else if ( s.find ( "--noSort" ) == 0 )
        {
            p.noSort = true;
        }
        else if ( s.find ( "--TfIdfOnly" ) == 0 )
        {
            p.SimilarityCalulation = false;
            p.TfIdfCalculation = true;
        }
        else if ( s.find ( "--SimilarityOnly" ) == 0 )
        {
            p.SimilarityCalulation = true;
            p.TfIdfCalculation = false;
        }
        else if ( s.find ( "--help" ) == 0 )
        {
            usage();
        }
        else if ( s.find ( "--" ) == 0 )
        {
            cerr << "Error switch \""<< s <<"\" unknown"<<endl;
            usage();
        }
    }
//     cerr <<"     p.debugMode "<<     p.debugMode <<endl;
//     cerr <<"     p.modFile "<<     p.modFile <<endl;
//     cerr <<"     p.unmodFile "<<     p.unmodFile<<endl;
//     cerr <<"     p.nbestmodFile "<<     p.nbestmodFile <<endl;
//     cerr <<"     p.nbestunmodFile "<<     p.nbestunmodFile<<endl;
//     cerr <<"     p.listeFile "<<     p.listeFile<<endl;
}

void verifyParameters ( parametres & p )
{
    if ( ( int ) p.outputFileName.size() == 0 )
    {
        cerr << "ERROR : OutputFileName is not set" << endl;
        usage();
    }
    if ( ( int ) p.inputFileName.size() == 0 )
    {
        cerr << "ERROR : inputFileName is not set" << endl;
        usage();
    }
    if ( ( int ) p.directoryDataName.size() == 0 )
    {
        cerr << "ERROR : directoryDataName is not set" << endl;
        usage();
    }
//     cerr << p.directoryDataName <<endl;
    if (!p.SbyS)
    {
        p.directoryDataName=p.directoryDataName+"/";
    }
//     cerr << p.directoryDataName <<endl;
//     if ( ( int ) p.unmodFile.size() == 0 )
//     {
//         cerr << "ERROR : unmodFile is not set" << endl;
//         usage();
//     }
//     if ( ( int ) p.nbestmodFile.size() == 0 )
//     {
//         cerr << "ERROR : nbestmodFile is not set" << endl;
//         usage();
//     }
//     if ( ( int ) p.nbestunmodFile.size() == 0 )
//     {
//         cerr << "ERROR : nbestunmodFile is not set" << endl;
//         usage();
//     }
//     if ( ( int ) p.listeFile.size() == 0 )
//     {
//         cerr << "ERROR : listeFile is not set" << endl;
//         usage();
//     }

    cerr << "Parameters OK !" << endl;

}


// typedef std::ifstream::pos_type pos_type;
//
// pos_type get_file_size( const std::string& filename )
// {
// std::ifstream InFile( filename.c_str(),
// std::ios_base::binary | std::ios_base::ate );
//
// if( !InFile )
// throw std::runtime_error( "ERROR: Could not obtain file size "
// "for [" + filename + "]\n" );
//
// return InFile.tellg();
// }
vector <string> list_directory(string dir)
{
    vector <string> to_return;
    boost::progress_timer t( std::clog );

    fs::path full_path( fs::initial_path<fs::path>() );

//   if ( argc > 1 )
    full_path = fs::system_complete( fs::path( dir.c_str() ) );
//   else
//     std::cout << "\nusage:   simple_ls [path]" << std::endl;

    unsigned long file_count = 0;
    unsigned long dir_count = 0;
    unsigned long other_count = 0;
    unsigned long err_count = 0;

    if ( !fs::exists( full_path ) )
    {
        std::cout << "\nNot found: " << full_path.string() << std::endl;
        return to_return;
    }

    if ( fs::is_directory( full_path ) )
    {
        std::cout << "\nIn directory: "
                  << full_path.string() << "\n\n";
        fs::directory_iterator end_iter;
        for ( fs::directory_iterator dir_itr( full_path );dir_itr != end_iter; ++dir_itr )
        {
            try
            {
//         if ( fs::is_directory( dir_itr->status() ) )
//         {
//           ++dir_count;
//           std::cout << dir_itr->path().filename() << " [directory]\n";
//         }
//         else
                if ( fs::is_regular_file( dir_itr->status() ) )
                {
                    ++file_count;
//           std::cout << dir_itr->path().filename() << "\n";
//                     to_return.push_back(full_path.string()+"/"+dir_itr->path().filename());
		    string l_full_path=full_path.string();
		    string l_filename=dir_itr->path().filename();
                    to_return.push_back(l_full_path+"/"+l_filename);
                }
                else
                {
                    ++other_count;
//           std::cout << dir_itr->path().filename() << " [other]\n";
                }

            }
            catch ( const std::exception & ex )
            {
                ++err_count;
                cerr << dir_itr->path().filename() << " " << ex.what() << std::endl;
            }
        }
//     std::cout << "\n" << file_count << " files\n"
//               << dir_count << " directories\n"
//               << other_count << " others\n"
//               << err_count << " errors\n";
    }
    else // must be a file
    {
//     std::cout << "\nFound: " << full_path.file_string() << "\n";
    }
    sort(to_return.begin(),to_return.end());
    return to_return;
}

string getTime()
{
    time_t rawtime;
    stringstream s;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    s << "Current local time and date : " <<  asctime (timeinfo);
    return s.str();
}


void run_thread(myIndex & l_myIndex, string test, int i, parametres l_p)
{
	l_myIndex.addIndex(test, i, l_p.TfIdfCalculation, l_p.SimilarityCalulation);
}

bool fileByFile_similarity_calculation(parametres l_p )
{
    string buffer;
    vector<int> liste;
    vector<int> listeCorres;
    vector<string> outputContent;
    vector<string> inputContent;
    vector<vecString> nbestmodContent;
    vector<vecString> nbestoutputContent;
    tfidf l_tfidf;
    similarity l_similarity;
    vector<string> stopWords;
    myIndex l_myIndex;
    myIndex l_myIndexQuery;
    if (l_p.debugMode)
    {
	cerr << "WARNING: DEBUGMODE ON, it could be very verbose !!! "<< endl;
    }
    l_tfidf.setDebugMode(l_p.debugMode);
    if ((int)l_p.stopWordsList.length()>0)
    {
        ifstream stopWordsFile(l_p.stopWordsList.c_str());
        if ( !stopWordsFile.is_open() )
        {
            cerr << "ERROR : main : can't open stop-words file : " <<  l_p.stopWordsList<< endl ;
            exit ( 0 );
        }
        while ( getline ( stopWordsFile, buffer ) )
        {
            stopWords.push_back (lowerCase(buffer));
        }
    }
    l_similarity.setLengthRatio(l_p.lengthRatio);
    l_similarity.setSortOptions(l_p.noSort);

//     list_directory(l_p.directoryDataName);
//     return 0;
    vector<string> fileNames=list_directory(l_p.directoryDataName);

    cerr << "Chargement des docs...\n";
//     vector<string> fileNames=stringToVector(l_p.inputFileName, ",");

//     copy(fileNames.begin(),fileNames.end(),ostream_iterator<string>(cerr,"|\n"));cerr<<endl;

    l_similarity.addDocNames(fileNames);
    l_similarity.setSortOptions(l_p.noSort);

//     boost::regex regEx ( "[\\.\\,\\;\\:\\/\\!\?\\(\\)\\[\\]\"\'\\+\\=\\*]+" );
    boost::regex regEx ( "[\\.\\,\\;\\:\\/\\!\?\\(\\)\\[\\]\"\'\\+\\=\\*\\<\\>\\_\\$\\^\\-’»«]+" );
    for (int i=0; i<(int)fileNames.size(); i++)
    {
//         cerr <<".";
	cerr << "Chargement de " <<  fileNames.at(i) << "\t";
	boost::progress_timer t( std::clog );
        ifstream data ( fileNames.at(i).c_str() );
        if ( !data.is_open() )
        {
            cerr << "ERROR : main : can't open file : " <<  fileNames.at(i) << " "<< endl ;
            exit ( 0 );
        }
        int cpt=0;
        string stringContent="";
        while ( getline ( data, buffer ) )
        {
            stringContent+=lowerCase(buffer)+" ";
            cpt++;
        }
        vector<string> to_keep;
        if ((int)stopWords.size()>0)
        {
            vector <string> tmp=stringToVector(stringContent," ");
            to_keep.clear();
            for (int i =0; i< (int) tmp.size(); i++)
            {
                bool doWekeep=true;
                for (int k =0; k< (int) stopWords.size(); k++)
                {
                    // 	    boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
                    // 		boost::match_results<string::const_iterator> vectorFilter;
                    if (tmp.at(i).compare(stopWords.at(k))==0)
                    {
                        doWekeep=false;
                        //         	    cerr << tmp.at(i) << "|" << stopWords.at(k) << "|" << (int)tmp.at(i).compare(stopWords.at(k)) << endl;
                    }
                }
                if (doWekeep)
                {
                    to_keep.push_back(tmp.at(i));
                }
            }
            stringContent=vectorToString(to_keep," ");
        }
        vector <string> analyse_content=stringToVector(stringContent," ");
        vector <string> to_keep_content;
        for (int i =0; i< (int) analyse_content.size(); i++)
        {
            boost::match_results<string::const_iterator> vectorFilter;
            if (!boost::regex_match ( analyse_content.at(i), vectorFilter, regEx, boost::match_default) && analyse_content.at(i).size()>1)
            {
                to_keep_content.push_back(analyse_content.at(i));
            }
        }
        stringContent=vectorToString(to_keep_content," ");


	int tenPercent=(int)to_keep_content.size()/10;
	int onePercent=(int)to_keep_content.size()/100;
	if (onePercent==0)
	{
	    onePercent=1;
	}
	if (tenPercent==0)
	{
	    tenPercent=1;
	}
	int fullSize=( int ) to_keep_content.size() - l_p.ngramSize-1;
 	boost::thread_group m_threads;
// 	cerr << "onePercent :" << onePercent << endl;
// 	m_threads.create_thread(boost::bind(&similarity::evaluate , this, l_vsInc));
//	#pragma omp parallel for shared (l_myIndex) num_threads(2)
//	rien
        for ( int l_pos = 0; l_pos <= fullSize; l_pos++ )
        {
	    if (fullSize>100)
	    {
// 	    boost::progress_timer t2( std::clog );
		if ( l_pos  % onePercent  == 0 )
		{
		    cerr << ".";
// 		    m_threads.join_all();
		}
		if ( l_pos  % tenPercent  == 0 )
                {
//                    cerr << ".";
                  m_threads.join_all();
                }
	    }
            string l_ngram_test = vectorToString ( subVector ( to_keep_content, l_pos, l_pos + l_p.ngramSize ), " " );
// 	    string l_ngram_test2 = vectorToString ( subVector ( to_keep_content, l_pos+1, l_pos+1 + l_p.ngramSize ), " " );
//		void run_thread(myIndex & l_myIndex, string test, int i, parametres l_p)
// 	    m_threads.create_thread(boost::bind(&myIndex:F:addIndex,l_myIndex,l_ngram_test, i, l_p.TfIdfCalculation, l_p.SimilarityCalulation));
// 	    m_threads.create_thread(boost::bind(&myIndex::addIndex,l_myIndex,l_ngram_test2, i, l_p.TfIdfCalculation, l_p.SimilarityCalulation));
//            l_myIndex.addIndex(l_ngram_test, i, l_p.TfIdfCalculation, l_p.SimilarityCalulation);
           l_myIndex.addIndex(l_ngram_test, i, false, l_p.SimilarityCalulation); //correction made for TF calculation.
        }
	m_threads.join_all();
	inputContent.push_back(stringContent);
        data.close();
    }
    cerr << "Taille : "<<l_myIndex.getMyIndex().size() << endl;
    ifstream inputs ( l_p.inputFileName.c_str() );
    if ( !inputs.is_open() )
    {
        cerr << "ERROR : main : can't open file : " <<  l_p.inputFileName<< endl ;
        exit ( 0 );
    }
    int cpt=0;
    string stringContent="";
    while ( getline ( inputs, buffer ) )
    {
        stringContent+=lowerCase(buffer)+" ";
        cpt++;
    }
    cerr << "Ok !"<<endl;
    cerr << "Nbr de docs chargés : "<< inputContent.size() <<endl;
    vector <string> analyse=stringToVector(stringContent," ");
    vector <string> to_keep;
    for (int i =0; i< (int) analyse.size(); i++)
    {
        boost::match_results<string::const_iterator> vectorFilter;
        if (!boost::regex_match ( analyse.at(i), vectorFilter, regEx, boost::match_default) && analyse.at(i).size()>1 )
        {
            to_keep.push_back(analyse.at(i));
        }
    }
    stringContent=vectorToString(to_keep," ");
    if ((int)l_p.pos.size()>0)
    {
        vector <string> tmp=stringToVector(stringContent," ");
        to_keep.clear();
        for (int k =0; k< (int) l_p.pos.size(); k++)
        {
            boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
            for (int i =0; i< (int) tmp.size(); i++)
            {
                boost::match_results<string::const_iterator> vectorFilter;
                if (boost::regex_match ( tmp.at(i), vectorFilter, regEx, boost::match_default))
                {
                    to_keep.push_back(tmp.at(i));
                }
            }
        }
        stringContent=vectorToString(to_keep," ");
    }
    if ((int)stopWords.size()>0)
    {
        vector <string> tmp=stringToVector(stringContent," ");
        to_keep.clear();
        for (int i =0; i< (int) tmp.size(); i++)
        {
            bool doWekeep=true;
            for (int k =0; k< (int) stopWords.size(); k++)
            {
// 	    boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
// 		boost::match_results<string::const_iterator> vectorFilter;
                if (tmp.at(i).compare(stopWords.at(k))==0)
                {
                    doWekeep=false;
//         	    cerr << tmp.at(i) << "|" << stopWords.at(k) << "|" << (int)tmp.at(i).compare(stopWords.at(k)) << endl;
                }
            }
            if (doWekeep)
            {
                to_keep.push_back(tmp.at(i));
            }
        }
        stringContent=vectorToString(to_keep," ");
    }
    vector <string> l_indexVec=stringToVector(stringContent," ");
//     to_keep.clear();
    for ( int l_pos = 0; l_pos + l_p.ngramSize <= ( int ) l_indexVec.size(); l_pos++ )
    {
        string l_ngram_test = vectorToString ( subVector ( l_indexVec, l_pos, l_pos + l_p.ngramSize ), " " );
// 	    myIndex l_myIndex(l_ngram_test, i);
        l_myIndexQuery.addIndex(l_ngram_test, 0,true,true);
// 	    size_t l_ngramHash = hashValueBoost ( l_ngram_test );
    }

//     exit(0);
//      stringContent+=" ";
// cerr << "Taille : "<<l_myIndexQuery.getMyIndex().size() << endl;
// cerr << "Taille : "<<l_myIndexQuery.getMyIndexMapInfo().size() << endl;
// cerr << "DATA :" << endl << l_myIndexQuery.toString() <<endl<<"======================================"<<endl;
// cerr << "DATA :" << endl << l_myIndex.toString() <<endl;
    ofstream output((l_p.outputFileName+".out").c_str());
    if (!l_p.TfIdfCalculation)
    {
        cerr << "Pass the TF.IDF calculation"<<endl;

    }
    else
    {
        cerr << "Add data for TF.IDF calculation"<<endl;
// 	copy(inputContent.begin(),inputContent.end(),ostream_iterator<string>(cerr,"|\n"));cerr<<endl;
// 	exit(0);
	
        l_tfidf.addDatas(l_myIndexQuery, l_myIndex,l_p.ngramSize, (unsigned long)fileNames.size()+1);
        cerr << "Compile data and made the TF.IDF calculation"<<endl;
        l_tfidf.compileData();
        //     cerr << "Affichage : "<< endl << l_tfidf.printDatas();
        //     return 0;
        cerr << "Ok !"<<endl;
        cerr << "Ecriture...";
        output << l_tfidf.printDatasSorted(l_p.nbestReturned);
        //     for (cpt=0; cpt<(int)outputContent.size(); cpt++)
        //     {
        // 	output << l_tfidf.printDatas();
        //     }
        cerr << "Ok !"<<endl;
    }
    if (!l_p.SimilarityCalulation)
    {
        cerr << "Pass the Similarity calculation"<<endl;
    }
    else
    {
        cerr << "Calcul de la similarité :"<<endl;
	if (!l_p.TfIdfCalculation)
	{
	    l_similarity.addTfIdfData(l_tfidf.getContent(l_p.nbestReturned));
	    l_similarity.addTfIdfDataIds(l_tfidf.getContentIds(l_p.nbestReturned));
	}
	else
	{
	    l_similarity.addTfIdfData(l_tfidf.getContent(l_p.nbestReturned));
	    l_similarity.addTfIdfDataIds(l_tfidf.getContentIds(l_p.nbestReturned));
	}
//         if (!l_p.TfIdfCalculation)
//         {
//             l_similarity.addTfIdfData(stringToVector(stringContent," "));
//         }
//         else
//         {
//             l_similarity.addTfIdfData(l_tfidf.getContent(l_p.nbestReturned));
//         }
        cerr << "Fin d'ajout des données"<<endl;
        l_similarity.calculateSimilarity(l_myIndex, stringContent,l_p.ngramSize);
	if (l_p.printFullResults)
	{
	    output << l_similarity.printResults(l_p.nbestSimReturned);
	}
	else
	{
	    output << l_similarity.printShortResults(l_p.nbestSimReturned);
	}
        cerr << "Ok !"<<endl;
    }
    output.close();
    return true;
}

bool sentenceBysentence_similarity_calculation(parametres l_p )
{
    string buffer;
    vector<int> liste;
    vector<int> listeCorres;
    vector<string> outputContent;
    vector<string> inputContent;
    vector<vecString> nbestmodContent;
    vector<vecString> nbestoutputContent;
    vector <string> docNames;
    myIndex l_myIndex;
//     list_directory(l_p.directoryDataName);
//     return 0;
    vector<string> stopWords;
    if ((int)l_p.stopWordsList.length()>0)
    {
        ifstream stopWordsFile(l_p.stopWordsList.c_str());
        if ( !stopWordsFile.is_open() )
        {
            cerr << "ERROR : main : can't open stop-words file : " <<  l_p.stopWordsList<< endl ;
            exit ( 0 );
        }
        while ( getline ( stopWordsFile, buffer ) )
        {
            stopWords.push_back (lowerCase(buffer));
        }
    }

    cerr << "Chargement des docs...\n";
//     vector<string> fileNames=stringToVector(l_p.inputFileName, ",");
    string fileNames=l_p.directoryDataName;
//     boost::regex regEx ( "[\\.\\,\\;\\:\\/\\!\?\\(\\)\\[\\]\"\'\\+\\=\\*]+" );
    boost::regex regEx ( "[\\.\\,\\;\\:\\/\\!\?\\(\\)\\[\\]\"\'\\+\\=\\*\\<\\>\\_\\$\\^\\-]+" );

//     for (int i=0; i<(int)fileNames.size(); i++)
//     {
    cerr <<".";
    ifstream data ( fileNames.c_str() );
    if ( !data.is_open() )
    {
        cerr << "ERROR : main : can't open file : " <<  fileNames<< endl ;
        exit ( 0 );
    }
    int cpt=0;
    string stringContent="";
    while ( getline ( data, buffer ) )
    {
        stringContent=lowerCase(buffer)+" ";
        stringstream s;
        s << cpt;
        cpt++;
        docNames.push_back(s.str());
        cerr <<".";
// 	if ((int)l_p.pos.size()>0)
// 	{
// 	    vector <string> tmp=stringToVector(stringContent," ");
// 	    vector <string> to_keep;
// 	    for (int k =0; k< (int) l_p.pos.size(); k++)
// 	    {
// 		boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
// 		for (int i =0; i< (int) tmp.size(); i++)
// 		{
// 		    boost::match_results<string::const_iterator> vectorFilter;
// 	// 	    cerr << "TEST : "<< regEx << "\t"<< tmp.at(i)<<endl;
// 		    if (boost::regex_match ( tmp.at(i), vectorFilter, regEx, boost::match_default))
// 		    {
// 			to_keep.push_back(tmp.at(i));
// 	// 		cerr << regEx << "\t"<< tmp.at(i)<<endl;
// 		    }
// 		}
// 	    }
// 	    stringContent=vectorToString(to_keep," ");
// 	}
// 	vector<string> to_keep;
// 	if ((int)stopWords.size()>0)
// 	{
// 	    vector <string> tmp=stringToVector(stringContent," ");
// 	    to_keep.clear();
// 	    for (int k =0; k< (int) stopWords.size(); k++)
// 	    {
//     // 	    boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
// 		for (int i =0; i< (int) tmp.size(); i++)
// 		{
//     // 		boost::match_results<string::const_iterator> vectorFilter;
// 		    if (tmp.at(i).compare(stopWords.at(k))!=0)
// 		    {
// 			to_keep.push_back(tmp.at(i));
// 		    }
// 		}
// 	    }
// 	    stringContent=vectorToString(to_keep," ");
// 	}
        vector <string> analyse_content=stringToVector(stringContent," ");
        vector <string> to_keep_content;
        for (int i =0; i< (int) analyse_content.size(); i++)
        {
            boost::match_results<string::const_iterator> vectorFilter;
            if (!boost::regex_match ( analyse_content.at(i), vectorFilter, regEx, boost::match_default))
            {
                to_keep_content.push_back(analyse_content.at(i));
            }
        }
        stringContent=vectorToString(to_keep_content," ");
        for ( int l_pos = 0; l_pos + l_p.ngramSize <= ( int ) to_keep_content.size(); l_pos++ )
        {
            string l_ngram_test = vectorToString ( subVector ( to_keep_content, l_pos, l_pos + l_p.ngramSize ), " " );
// 	    myIndex l_myIndex(l_ngram_test, i);
//     if (!l_p.TfIdfCalculation)
//     {

//     }

//             l_myIndex.addIndex(l_ngram_test, cpt, l_p.TfIdfCalculation, l_p.SimilarityCalulation); 
            l_myIndex.addIndex(l_ngram_test, cpt, false, l_p.SimilarityCalulation); // correction made on TF calculation
// 	    size_t l_ngramHash = hashValueBoost ( l_ngram_test );
        }

// 	inputContent.push_back(stringContent);
    }
    data.close();
//     }
    ifstream inputs ( l_p.inputFileName.c_str() );
    if ( !inputs.is_open() )
    {
        cerr << "ERROR : main : can't open file : " <<  l_p.inputFileName<< endl ;
        exit ( 0 );
    }
    cpt=0;
    stringContent="";
    ofstream output((l_p.outputFileName+".out").c_str());
    while ( getline ( inputs, buffer ) )
    {
	myIndex l_myIndexQuery;
        tfidf l_tfidf;
        stringContent=lowerCase(buffer)+" ";
        stringstream l_tmp_s;
        l_tmp_s<< cpt;
        cpt++;
        cerr << "Ok !"<<endl;
        cerr << "Nbr de docs chargés : "<< docNames.size() <<endl;
        vector <string> analyse=stringToVector(stringContent," ");
        vector <string> to_keep;
        for (int i =0; i< (int) analyse.size(); i++)
        {
            boost::match_results<string::const_iterator> vectorFilter;
            if (!boost::regex_match ( analyse.at(i), vectorFilter, regEx, boost::match_default))
            {
                to_keep.push_back(analyse.at(i));
                // 	    cerr << analyse.at(i) << " ";
            }
        }
        stringContent=vectorToString(to_keep," ");
        if ((int)l_p.pos.size()>0)
        {
            vector <string> tmp=stringToVector(stringContent," ");
            to_keep.clear();
            for (int k =0; k< (int) l_p.pos.size(); k++)
            {
                boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
                for (int i =0; i< (int) tmp.size(); i++)
                {
                    boost::match_results<string::const_iterator> vectorFilter;
                    if (boost::regex_match ( tmp.at(i), vectorFilter, regEx, boost::match_default))
                    {
                        to_keep.push_back(tmp.at(i));
                    }
                }
            }
            stringContent=vectorToString(to_keep," ");
        }
        if ((int)stopWords.size()>0)
        {
            vector <string> tmp=stringToVector(stringContent," ");
            to_keep.clear();
            for (int i =0; i< (int) tmp.size(); i++)
            {
                bool doWekeep=true;
                for (int k =0; k< (int) stopWords.size(); k++)
                {
                    // 	    boost::regex regEx ( "([œa-zéàùuïîêè\\-0-9]+\\["+l_p.pos.at(k)+"\\])" );
                    // 		boost::match_results<string::const_iterator> vectorFilter;
                    if (tmp.at(i).compare(stopWords.at(k))==0)
                    {
                        doWekeep=false;
                        //         	    cerr << tmp.at(i) << "|" << stopWords.at(k) << "|" << (int)tmp.at(i).compare(stopWords.at(k)) << endl;
                    }
                }
                if (doWekeep)
                {
                    to_keep.push_back(tmp.at(i));
                }
            }
            stringContent=vectorToString(to_keep," ");

        }
        vector <string> l_indexVec=stringToVector(stringContent," ");
	vector <string> l_testVec;
	vector <size_t> l_testVecIds;
// 	vector <size_t> l_indexVecIds(l_indexVec.size());
//     to_keep.clear();

        for ( int l_pos = 0; l_pos + l_p.ngramSize <= ( int ) l_indexVec.size(); l_pos++ )
        {
            string l_ngram_test = vectorToString ( subVector ( l_indexVec, l_pos, l_pos + l_p.ngramSize ), " " );
// 	    myIndex l_myIndex(l_ngram_test, i);
            l_myIndexQuery.addIndex(l_ngram_test, 0,true,true);
	    l_testVec.push_back(l_ngram_test);
	    l_testVecIds.push_back(hashValueBoost(l_ngram_test));
	    
// 	    size_t l_ngramHash = hashValueBoost ( l_ngram_test );
        }
	
// 	cerr << "DATA :" << endl;
// 	cerr << l_myIndex.toString() << endl;
// 	cerr << "QUERY :" << endl;
// 	cerr << l_myIndexQuery.toString() << endl;

        //     exit(0);
        //      stringContent+=" ";

        output << "========== BEGIN OF SCORES ==========" <<endl;
        if (!l_p.TfIdfCalculation)
        {
            cerr << "Pass the TF.IDF calculation"<<endl;
        }
        else
        {
//             cerr << "DO the TF.IDF calculation"<<endl;
//             l_tfidf.addDatas(l_myIndexQuery, l_myIndex,l_p.ngramSize, );

	    l_tfidf.addDatas(l_myIndexQuery,l_myIndex,l_p.ngramSize,(int)docNames.size()+1);
//             l_tfidf.addDatas(stringContent, inputContent,l_p.ngramSize);
            l_tfidf.compileData();
            //     cerr << "Affichage : "<< endl << l_tfidf.printDatas();
            //     return 0;
            cerr << "Ok !"<<endl;
            cerr << "Ecriture...";
            output << "========== BEGIN OF TF.IDF SCORES ==========" <<endl;
            output << l_tfidf.printDatasSorted(l_p.nbestReturned);
            output << "========== END OF TF.IDF SCORES ==========" <<endl;
            //     for (cpt=0; cpt<(int)outputContent.size(); cpt++)
            //     {
            // 	output << l_tfidf.printDatas();
            //     }
            cerr << "Ok !"<<endl;
        }
        if (!l_p.SimilarityCalulation)
        {
            cerr << "Pass the Similarity calculation"<<endl;
        }
        else
        {
            cerr << "Calcul de la similarité :" << endl;
            output << "========== BEGIN OF SIMILARITY SCORES ==========" <<endl;
            output << "Similarity Calculation:\t" << l_tmp_s.str() +"\t"+stringContent<< endl;
            similarity l_similarity;
            l_similarity.setLengthRatio(l_p.lengthRatio);
            l_similarity.addDocNames(docNames);
            if (!l_p.TfIdfCalculation)
            {
                l_similarity.addTfIdfData(l_tfidf.getContent(l_p.nbestReturned));
		l_similarity.addTfIdfDataIds(l_tfidf.getContentIds(l_p.nbestReturned));
            }
            else
            {
                l_similarity.addTfIdfData(l_tfidf.getContent(l_p.nbestReturned));
		l_similarity.addTfIdfDataIds(l_tfidf.getContentIds(l_p.nbestReturned));
            }
            if (!l_p.TfIdfCalculation)
	    {
                l_similarity.addTfIdfData(l_testVec);
		l_similarity.addTfIdfDataIds(l_testVecIds);
	    }
            l_similarity.calculateSimilarity(l_myIndex, stringContent,l_p.ngramSize, l_p.nbestSimReturned);

	if (l_p.printFullResults)
	{
	    output << l_similarity.printResults(l_p.nbestSimReturned);
	}
	else
	{
	    output << l_similarity.printShortResults(l_p.nbestSimReturned);
	}
            output << "========== END OF SIMILARITY SCORES ==========" <<endl;
        }
        output << "========== END OF SCORES ==========" <<endl;
    }

    cerr << "Ok !"<<endl;
    output.close();
    return true;
}


int main ( int argc, char *argv[] )
{
    parametres l_p;
    readCommandLineArguments ( argc, argv, l_p );
    verifyParameters ( l_p );
    if (l_p.SbyS)
    {
        sentenceBysentence_similarity_calculation(l_p);
    }
    else
    {
        fileByFile_similarity_calculation(l_p);
    }
    return 0;
}

