#include "Board.h"

#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

namespace ppittawat::boggle
{

Board loadBoard( const char * file )
{
    ifstream in( file );
    if( ! in.is_open() )
    {
        throw invalid_argument( string( "can not open " ) + file );
    }

    string word;
    unsigned row, col;
    in >> row >> col;
    vector< char > val( row * col * 2 );
    string tmp;

    Board::row_t ridx = 0;
    Board::col_t cidx= 0;
    while( in >> tmp )
    {
        unsigned vidx = ( cidx + ( ridx * col ) ) * 2;
        val[ vidx ] = toupper( tmp[0] );
        val[ vidx + 1] = toupper( tmp[1] );
        cidx++;
        if( cidx == col )
        {
            cidx = 0;
            ridx++;
        }
    }

    return Board{ val.data(), row, col };
}

} // end namespace ppittawat:boggle
