#pragma once

#include "Board.h"
#include "Trie.h"

#include <set>
#include <string>

namespace ppittawat::boggle
{

//using Trie as dict
using Dictionary = trie::Trie;

class Solver
{
public:
    Solver( const Dictionary & dict ):
        m_dict( dict )
    {}

    Solver( Dictionary && dict ):
        m_dict( std::move( dict ) )
    {}

    std::set< std::string > getAllValidWords( const Board & board ) const;

    static constexpr int scoreOf( const std::string & word )
    {
        if( word.size() < 3 )
        {
            return 0;
        }
        else if( word.size() < 5 )
        {
            return 1;
        }
        else if( word.size() < 6 )
        {
            return 2;
        }
        else if( word.size() < 7 )
        {
            return 3;
        }
        else if( word.size() < 8 )
        {
            return 5;
        }
        else
        {
            return 11;
        }
    }
private:
    Dictionary m_dict;
};


} // end namespace ppittawat::boggle
