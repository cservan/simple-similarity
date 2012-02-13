
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/progress.hpp"

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
    bool noTfIdf;
    bool noSimilarity;
//     bool outputAlignments;
//     string sourceFile;     // path to the resources
//     string targetFile;     // path to the configuration files
//     string dictionnaryTranslationFile;     // path to the configuration files
    string outputFileName;
    string inputFileName;
    string directoryDataName;
    vector <string> pos;
    int ngramSize;
    int nbestReturned;
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
    cerr << "Usage : " << endl << "\tsimilarity --in inputFileName --data fileDataName --out outPutFile [--size ngramSize] [--nbest nbestSize] [--POS nom,adv,adj,...] [--noTfIdf|noSimilarity]:\n\n\t\t --debugMode \t: print debug messages \n\t\t --noTfIdf \t: do not calculate TF.IDF take the input as full request \n\t\t --noSimilarity \t: do not calculate the similarity calculate only the TF.IDF score\n\t\t --help \t: print this help message.\n" << endl;
    exit ( 0 );
// 	System.exit(1);

}
void readCommandLineArguments ( unsigned int argc, char *argv[] , parametres & p )
{
    p.debugMode = false;
    p.noTfIdf = false;
    p.noSimilarity = false;
    p.outputFileName = "";
    p.directoryDataName = "";
    p.inputFileName="";
    p.ngramSize=1;
    p.nbestReturned=10000;
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
        else if ( s.find ( "--data" ) == 0 )
        {
            p.directoryDataName = infos;
        }
        else if ( s.find ( "--size" ) == 0 )
        {
            p.ngramSize = atoi(infos.c_str());
        }
        else if ( s.find ( "--nbest" ) == 0 )
        {
            p.nbestReturned = atoi(infos.c_str());
        }
        else if ( s.find ( "--POS" ) == 0 )
        {
            p.pos = stringToVector(infos,",");
        }
        else if ( s.find ( "--debugMode" ) == 0 )
        {
            p.debugMode = true;
        }
        else if ( s.find ( "--noTfIdf" ) == 0 )
        {
            p.noTfIdf = true;
	    p.noSimilarity = false;
        }
        else if ( s.find ( "--noSimilarity" ) == 0 )
        {
            p.noTfIdf = false;
	    p.noSimilarity = true;
        }
        else if ( s.find ( "--help" ) == 0 )
        {
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

void verifyParameters ( parametres p )
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
    std::cout << "\nNot found: " << full_path.file_string() << std::endl;
    return to_return;
  }

  if ( fs::is_directory( full_path ) )
  {
    std::cout << "\nIn directory: "
              << full_path.directory_string() << "\n\n";
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
	  to_return.push_back(full_path.directory_string()+dir_itr->path().filename());
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


int main ( int argc, char *argv[] )
{
    parametres l_p;
    readCommandLineArguments ( argc, argv, l_p );
    string buffer;
    verifyParameters ( l_p );
    vector<int> liste;
    vector<int> listeCorres;
    vector<string> outputContent;
    vector<string> inputContent;
    vector<vecString> nbestmodContent;
    vector<vecString> nbestoutputContent;
//     list_directory(l_p.directoryDataName);
//     return 0;
    
    cerr << "Chargement des docs...";
//     vector<string> fileNames=stringToVector(l_p.inputFileName, ",");
    string fileNames=l_p.directoryDataName;
    boost::regex regEx ( "[\\.\\,\\;\\:\\/\\!\?\\(\\)\\[\\]\"\'\\+\\=\\*]+" );
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
    vector <string> docNames;
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
	inputContent.push_back(stringContent);
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
	tfidf l_tfidf;
	stringContent=lowerCase(buffer)+" ";
	stringstream l_tmp_s;
	l_tmp_s<< cpt;
	cpt++;
	cerr << "Ok !"<<endl;
	cerr << "Nbr de docs chargés : "<< inputContent.size() <<endl;
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
    //     exit(0);
    //      stringContent+=" ";
	output << "========== BEGIN OF SCORES ==========" <<endl;
	if (l_p.noTfIdf)
	{
	  cerr << "Pass the TF.IDF calculation"<<endl;
	}
	else
	{
	    l_tfidf.addDatas(stringContent, inputContent,l_p.ngramSize);
	    l_tfidf.compileData();
	//     cerr << "Affichage : "<< endl << l_tfidf.printDatas();
	//     return 0;
	    cerr << "Ok !"<<endl;
	    cerr << "Ecriture...";
	    output << "========== BEGIN OF TF.IDF SCORES ==========" <<endl;
	    output << l_tfidf.printDatasSorted();
	    output << "========== END OF TF.IDF SCORES ==========" <<endl;
	//     for (cpt=0; cpt<(int)outputContent.size(); cpt++)
	//     {
	// 	output << l_tfidf.printDatas();
	//     }
	    cerr << "Ok !"<<endl;
	}
	if (l_p.noSimilarity)
	{
	    cerr << "Pass the Similarity calculation"<<endl;
	}
	else
	{
	    cerr << "Calcul de la similarité :" << endl;
	    output << "========== BEGIN OF SIMILARITY SCORES ==========" <<endl;
	    output << "Similarity Calculation:\t" << l_tmp_s.str() +"\t"+stringContent<< endl;
	    similarity l_similarity;
	    l_similarity.addDocNames(docNames);
	    if (l_p.noTfIdf)
	    {
		l_similarity.addTfIdfData(stringToVector(stringContent," "));
	    }   
	    else
	    {
		l_similarity.addTfIdfData(l_tfidf.getContent(l_p.nbestReturned));
	    }
	    l_similarity.calculateSimilarity(inputContent, stringContent);
	    
	    output << l_similarity.printResults();
	    output << "========== END OF SIMILARITY SCORES ==========" <<endl;
	}
	output << "========== END OF SCORES ==========" <<endl;
    }

    cerr << "Ok !"<<endl;
    output.close();
    return 0;
}

