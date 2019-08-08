#include "Board.h"
#include "utils.h"
#include "Solver.h"
#include "TrieNodeTpl.h"

#include <chrono>
#include <iostream>
#include <stdexcept>

using namespace std::chrono;
using namespace std;
using namespace ppittawat::boggle;
using namespace ppittawat::trie;

namespace
{

unsigned score_counter( const set< string > & words )
{
    unsigned score = 0;
    for( auto it = words.begin(); it != words.end(); ++it )
    {
        score += Solver::scoreOf( *it );
    }

    return score;
}

string filename( const string & src )
{
    string s = src;
    auto lastslash = s.find_last_of( "\\/" );
    if( string::npos != lastslash )
    {
        s.erase( 0, lastslash + 1 );
    }
    return s;
}

} // end anonymous namespace

namespace test
{

void test_perf( const Solver & s )
{
    cout << "Start solving 1000 random board" << endl;
    auto start = high_resolution_clock::now();
    for( int i = 0; i < 1000; ++i )
    {
        Board b;
        s.getAllValidWords( b );
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>( stop - start );
    cout << "Take : " << duration.count() << " msec" << endl;
}

} // end test namespace

namespace bin
{

int test_trie( int argc, char * argv[] )
{
    if( argc != 2 )
    {
        cout << "usage: " << argv[0] << " [dictionary_file]" << endl;
        return 0;
    }

    try
    {
        ppittawat::trie::test_trie( argv[1] );
    }
    catch( const std::exception & ex )
    {
        cout << "Error : " << ex.what() << endl;
        return -1;
    }
    catch( const std::string & ex )
    {
        cout << "Error : " << ex << endl;
        return -1;
    }
    catch ( ... )
    {
        cout << "Error unknown exception" <<endl;
        return -1;
    }

    return 0;
}

int test_board( int argc, char * argv[] )
{
    if( argc != 2 )
    {
        cout << "usage: " << argv[0] << " [board_file]" << endl;
        return 0;
    }

    try
    {
        cout << loadBoard( argv[1] ) << endl;
    }
    catch( const std::exception & ex )
    {
        cout << "Error : " << ex.what() << endl;
    }
    catch( const std::string & ex )
    {
        cout << "Error : " << ex << endl;
    }
    catch ( ... )
    {
        cout << "Error unknown exception" <<endl;
    }

    return 0;
}

int test_perf( int argc, char * argv[] )
{
    if( argc != 2 )
    {
        cout << "usage: " << argv[0] << " [dictionary_file]" << endl;
        return 0;
    }

    try
    {
        Trie t = loadTrie( argv[1] );
        Solver s( move( t ) );
        test::test_perf( s );
    }
    catch( const std::exception & ex )
    {
        cout << "Error : " << ex.what() << endl;
    }
    catch( const std::string & ex )
    {
        cout << "Error : " << ex << endl;
    }
    catch ( ... )
    {
        cout << "Error unknown exception" <<endl;
    }

    return 0;
}

int boggle( int argc, char * argv[] )
{
    if( argc != 3 )
    {
        cout << "usage: " << argv[0] << " [dictionary_file] [board_file]" << endl;
        return 0;
    }

    try
    {
        Trie t = loadTrie( argv[1] );
        Board b = loadBoard( argv[2] );
        //use move because the Trie is huge.
        Solver s( move( t ) );
        auto words = s.getAllValidWords( b );

        cout << b << endl;
        cout << "Max points: " << score_counter( words ) << endl;
        cout << " --- " << endl;
        cout << " - All valid words -" << endl;

        for( auto it = words.begin(); it != words.end(); ++it )
        {
            cout << *it << endl;
        }
    }
    catch( const std::exception & ex )
    {
        cout << "Error : " << ex.what() << endl;
    }
    catch( const std::string & ex )
    {
        cout << "Error : " << ex << endl;
    }
    catch ( ... )
    {
        cout << "Error unknown exception" <<endl;
    }

    return 0;
}

} // end bin namespace

int main( int argc, char * argv[] )
{
    string fname = filename( argv[0] );
    cout << "The fname is " << fname << endl;
    if( fname == "test_trie" )
    {
        return bin::test_trie( argc, argv );
    }
    else if( fname == "test_board" )
    {
        return bin::test_board( argc, argv );
    }
    else if( fname == "test_perf" )
    {
        return bin::test_perf( argc, argv );
    }
    return bin::boggle( argc, argv );
}

