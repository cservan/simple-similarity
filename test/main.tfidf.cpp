
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
#include <../src/tools.h>
#include <../src/tfidf.h>
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
//     bool outputAlignments;
//     string sourceFile;     // path to the resources
//     string targetFile;     // path to the configuration files
//     string dictionnaryTranslationFile;     // path to the configuration files
    string outputFileName;
    string inputFileName;
    string directoryDataName;
    int ngramSize;
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
    cerr << "Usage : " << endl << "\treformTranslationsUnsup --in inputFileName --data directoryDataName --out outPutFile [--size ngramSize]:\n\n\t\t --debugMode \t: print debug messages \n\t\t --help \t: print this help message.\n" << endl;
    exit ( 0 );
// 	System.exit(1);

}
void readCommandLineArguments ( unsigned int argc, char *argv[] , parametres & p )
{
    p.debugMode = false;
    p.outputFileName = "";
    p.directoryDataName = "";
    p.inputFileName="";
    p.ngramSize=1;
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
        else if ( s.find ( "--debugMode" ) == 0 )
        {
            p.debugMode = true;
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
    tfidf l_tfidf;
    
    list_directory(l_p.directoryDataName);
//     return 0;
    
    cerr << "Chargement des docs...";
//     vector<string> fileName=stringToVector(l_p.inputFileName, ",");
    vector<string> fileName=list_directory(l_p.directoryDataName);
    for (int i=0; i<(int)fileName.size(); i++)
    {
	cerr <<".";
	ifstream data ( fileName.at(i).c_str() );
	if ( !data.is_open() )
	{
	    cerr << "ERROR : main : can't open file : " <<  fileName.at(i)<< endl ;
	    exit ( 0 );
	}
	int cpt=0;
	string stringContent="";
	while ( getline ( data, buffer ) )
	{
	    stringContent+=tokenizePunct(lowerCase(buffer))+" ";
	    cpt++;
	}
	inputContent.push_back(stringContent);
	data.close();
    }
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
	stringContent+=tokenizePunct(lowerCase(buffer))+" ";
	cpt++;
    }
    cerr << "Ok !"<<endl;
    cerr << "Nbr de docs chargés : "<< inputContent.size() <<endl;
    l_tfidf.addDatas(stringContent, inputContent,l_p.ngramSize);
    l_tfidf.compileData();
//     cerr << "Affichage : "<< endl << l_tfidf.printDatas();
//     return 0;
    cerr << "Ecriture...";
    ofstream output((l_p.outputFileName+".out").c_str());
    output << l_tfidf.printDatasSorted();
//     for (cpt=0; cpt<(int)outputContent.size(); cpt++)
//     {
// 	output << l_tfidf.printDatas();
//     }
    output.close();
    cerr << "Ok !"<<endl;
    return 0;
}

