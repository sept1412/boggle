#include "TrieNodeTpl.h"

#include <fstream>
#include <stdexcept>
#include <string>
#include <iostream>

using namespace std;

namespace ppittawat::trie
{

Trie loadTrie( const char * file )
{
    std::ifstream in( file );
    if( ! in.is_open() )
    {
        throw std::invalid_argument( std::string( "can not open " ) + file );
    }

    Trie t;
    std::string word;
    while( std::getline( in, word ) )
    {
        t.insert( word );
    }

    return t;
}

void test_trie( const char * file )
{
    std::ifstream in( file );
    if( ! in.is_open() )
    {
        throw std::invalid_argument( std::string( "can not open " ) + file );
    }

    int fcounter = 0;
    Trie t;
    std::string word;
    while( std::getline( in, word ) )
    {
        fcounter++;
        t.insert( word );
    }

    cout << fcounter << " words is read from the "
        << file << " to a Trie" << endl;

    int tcounter = 0;
    for( auto it = t.begin(); it != t.end(); ++it )
    {
        tcounter++;
    }

    cout << tcounter << " words is stored in the Trie " << endl;
}

}// end namespace ppittawat::trie
